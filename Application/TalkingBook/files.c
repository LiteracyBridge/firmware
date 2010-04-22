// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/talkingbook.h"
#include "Include/device.h"
#include "Include/audio.h"
#include "Include/files.h"

APP_IRAM static long filePosition;
APP_IRAM char logBuffer[LOG_BUFFER_SIZE];
APP_IRAM int idxLogBuffer;

extern APP_IRAM unsigned int vCur_1;
extern void refuse_lowvoltage(int);

BOOL nextNameValuePair (int handle, char *buffer, char delimiter, char **name, char **value) {
	BOOL ret;
	char *cursor;
	
	cursor = getLine(handle, buffer);
	if (cursor) {
		ret = TRUE;
		*name = cursor;
		*value = strchr(cursor,delimiter);
		if (*value != NULL) {
			**value = 0;
			(*value)++;
		} 
	}
	else
		ret = FALSE;
	return ret;
}

void clearStaleLog() {
	int handle, ret;
	char buffer[LOG_CARRYOVER_BYTES / 2];
	
	if(vCur_1 < V_MIN_SDWRITE_VOLTAGE) {
		refuse_lowvoltage(0);
		return;
	}
	
	if (LOG_FILE) {
		handle = tbOpen((LPSTR)(LOG_FILE),O_RDONLY);
		if(handle < 0 )
			logException(13,0,0); 
		else {
			lseek(handle,-LOG_CARRYOVER_BYTES,SEEK_END);
			ret = read(handle,(unsigned long)&buffer<<1,LOG_CARRYOVER_BYTES);
			close(handle);
			unlink((LPSTR)(LOG_FILE));
			handle = tbOpen((LPSTR)(LOG_FILE),O_CREAT|O_RDWR|O_TRUNC);
			if (handle != -1) {
				write(handle,(unsigned long)&buffer<<1,LOG_CARRYOVER_BYTES);
				close(handle);
			} else
				logException(13,0,0); 
		}
	}
}

void logString(char *string, int whenToWrite) {
	int len, available;
	
	// WARNING: if !waitToWrite, this could destory *string
	// todo: add whenToWrite parameter to allow write "ASAP but preserve" 	
	if (LOG_FILE) {
		if (whenToWrite==ASAP && !SACM_Status()) {
			flushLog();
			appendStringToFile(LOG_FILE,string);  // destroys *string	
		} else {
			if (idxLogBuffer && (idxLogBuffer <= (LOG_BUFFER_SIZE - 3))) {
				// already one entry, separate with CRLF
				logBuffer[idxLogBuffer++] = 0x0d;
				logBuffer[idxLogBuffer++] = 0x0a;
				logBuffer[idxLogBuffer] = '\0';			 
			}
			available = LOG_BUFFER_SIZE - idxLogBuffer - 1;	
			len = strlen(string);		
			if ((available > 0) && (len > available)) {
				memcpy(&logBuffer[idxLogBuffer],string,available);
				idxLogBuffer = LOG_BUFFER_SIZE;
				logBuffer[LOG_BUFFER_SIZE - 1] = '\0';
			} else if (available >= len) {
				strcpy(&logBuffer[idxLogBuffer],string);
				idxLogBuffer += len;
				logBuffer[idxLogBuffer] = '\0';			 
			}
		}
	}
}

void flushLog(void) {

	if (idxLogBuffer && LOG_FILE && !SACM_Status()) {	
		appendStringToFile(LOG_FILE,logBuffer);	
		idxLogBuffer = 0;
	}
}


int insertStringInFile(const char * filename, char * strText, long posInsert) {
	//todo: create a version without a roundtrip between single/dbl-byte chars	
	int rHandle, wHandle, ret, i, bytesToWrite;
	char wFilepath[PATH_LENGTH];
	char buffer[READ_LENGTH+1];
	char tempLine[80];
	int MAX_BYTES = 2 * READ_LENGTH;

	if(vCur_1 < V_MIN_SDWRITE_VOLTAGE) {
		refuse_lowvoltage(0);
		return -1;
	}

	strcpy(wFilepath,"temp.txt");
	rHandle = tbOpen((LPSTR)(filename),O_RDONLY);
	wHandle = tbOpen((LPSTR)wFilepath,O_CREAT|O_TRUNC|O_WRONLY);
	ret = -1;
	if (rHandle != -1 && wHandle != -1) {
		buffer[READ_LENGTH] = '\0';
		ret = strcspn(strText,"\x0a\x0d");
		if (ret > 0) 
			*(strText + ret) = '\0';
		if (posInsert) {
			bytesToWrite = read(rHandle,(unsigned long)buffer << 1,posInsert % MAX_BYTES);
			ret = write(wHandle,(unsigned long)buffer << 1,bytesToWrite);
			for (i=bytesToWrite; i < posInsert; i+= MAX_BYTES) {
				bytesToWrite = read(rHandle,(unsigned long)buffer << 1,MAX_BYTES);
				ret = write(wHandle,(unsigned long)buffer << 1,bytesToWrite);
			}
		}
		bytesToWrite = convertDoubleToSingleChar(tempLine,strText,TRUE);
		ret = write(wHandle,(unsigned long)tempLine<<1,bytesToWrite);		
		do {
			bytesToWrite = read(rHandle,(unsigned long)buffer << 1,MAX_BYTES);
			ret = write(wHandle,(unsigned long)buffer << 1,bytesToWrite);
		} while (bytesToWrite == MAX_BYTES);
		close(wHandle);
		close(rHandle);
		i = unlink((LPSTR)filename);
		if (i != -1) {
			//todo: change this to rename instead of copy and unlink
			i = rename((LPSTR)wFilepath,(LPSTR)filename);
			//if (i != -1)
			//	i = unlink((LPSTR)wFilepath);
		}
		ret = i;
	}
	return ret;
}

int appendStringToFile(const char * filename, char * strText) {
	// WARNING: This function destroys strText.
	// WARNING: reading/writing to NAND while audio is playing/recording is bad!
	
	// This fct is used to ensure a fatal error can be logged to some file, 
	// even if the regular log file is not specified in the config file.
	// It is also used to append to a list.
	
	int handle, ret;
	unsigned int bytesToWrite;
//	char strCharText[READ_LENGTH+1];

/*	if (strlen(strText) > READ_LENGTH) {
		memcpy(strCharText,strText,READ_LENGTH);
		strCharText[READ_LENGTH] = '\0';
	} else
		strcpy(strCharText,strText);
*/
	if(vCur_1 < V_MIN_SDWRITE_VOLTAGE) {
		refuse_lowvoltage(0);
		return -1;
	}

	ret = -1;
	handle = tbOpen((LPSTR)filename,O_RDWR);
	if (handle == -1) {
		handle = tbOpen((LPSTR)filename,O_CREAT|O_RDWR|O_TRUNC);
		if (handle == -1)
			ret = -1;
	} else
		lseek(handle,0,SEEK_END);

	if(handle >= 0 ) {
		ret = 0;
		bytesToWrite = convertDoubleToSingleChar(strText,strText,TRUE);
	 	ret = write(handle, (unsigned long)strText << 1, bytesToWrite);
		close(handle);
	}
	return ret;
}

int convertDoubleToSingleChar(char * out, const char * in, BOOL addCRLF) {
	int bytesToWrite, i;
	char low, high;  
	
	for (i=0, high = 1; (low=*(in+i*2)) && high; i++) {
		high = *(in + i*2 + 1);
		*(out + i) = high<<8 | low;
	}
	bytesToWrite = i*2;
	if (addCRLF) {
		if (i && (*(out+i-1) < 0x0100)) {
			*(out+i-1) |= 0x0d00;
			*(out+i) = 0x000a;
			bytesToWrite++;
		} else { 
			*(out + i) = 0x0a0d;
			bytesToWrite += 2;
		}
	}
	return bytesToWrite;	
}
/*
int createControlFromTemplate(char *packageName, char * filename) {
	//todo: create a version without a roundtrip between single/dbl-byte chars	
	int rHandle, wHandle, ret, bytesToWrite;
	char buffer[READ_LENGTH+1];
	char *rCursor, *cursor2;
	char file1[40];
	char line[80];
	char tempLine[80];
	int templateCount;
	
	buffer[READ_LENGTH] = '\0';
	*line = 0;
	templateCount = 0;

	strcpy(file1,USER_PATH);
	strcat(file1,filename);
	wHandle = tbOpen((LPSTR)file1,O_CREAT|O_TRUNC|O_WRONLY);
	rHandle = tbOpen((LPSTR)CONTROL_TEMPLATE,O_RDONLY);
	if (rHandle == -1 || wHandle == -1)
		return -1;
	getLine(-1,0);  // reset in case at end from prior use
	while ((rCursor = getLine(rHandle,buffer))) {
		if (*rCursor != TEMPLATE_CHAR) {
			bytesToWrite = convertDoubleToSingleChar(tempLine,rCursor,TRUE);
			ret = write(wHandle,(unsigned long)tempLine<<1,bytesToWrite);
		} else {
			// found template line
			rCursor++;
			cursor2 = strchr(rCursor,TEMPLATE_CHAR);
			if (cursor2) {
				*cursor2 = 0;
				bytesToWrite = convertDoubleToSingleChar(tempLine,rCursor,FALSE);
				ret = write(wHandle,(unsigned long)tempLine<<1,bytesToWrite);
				if (templateCount == 0) {
					strcpy(line,packageName);  // todo: don't hard code the package/file items
					templateCount++;
				}
				else if (templateCount == 1) {
					strcpy(line,filename);  // todo: don't hard code the package/file items					
				}
				bytesToWrite = convertDoubleToSingleChar(tempLine,line,TRUE);
				ret = write(wHandle,(unsigned long)tempLine<<1,bytesToWrite);				
			}			
		}
	}
	close(rHandle);
	close(wHandle);
	return ret;
}
*/

int findDeleteStringFromFile(char *path, char *filename, const char * string, BOOL shouldDelete) {
	//todo: create a version without a roundtrip between single/dbl-byte chars	
	int rHandle, wHandle, ret, i, bytesToWrite;
	char tempFilename[10];
	char buffer[READ_LENGTH+1];
	char *rCursor;
	char tempLine[100];
	int unequal;
	char find[PATH_LENGTH];
	
	if(vCur_1 < V_MIN_SDWRITE_VOLTAGE) {
		refuse_lowvoltage(0);
		return -1;
	}
		
	LBstrncpy(find,string,PATH_LENGTH);
	buffer[READ_LENGTH] = '\0';
	
	ret = strcspn(find,"\x0a\x0d");
	if (ret > 0) 
		*(find + ret) = '\0';

	ret = -1;
	if(path != NULL) {
		// warning - no chdir back before exit ????
		tbChdir((LPSTR)path);
	}
	if (shouldDelete) {
		strcpy(tempFilename,"temp.txt");	
		wHandle = tbOpen((LPSTR)tempFilename,O_CREAT|O_TRUNC|O_WRONLY);
	}
	if (!shouldDelete || wHandle != -1) {
		rHandle = tbOpen((LPSTR)filename,O_RDONLY);
		if (rHandle == -1 || wHandle == -1)
			return -1;
		getLine(-1,0);  // reset in case at end from prior use
		while ((rCursor = getLine(rHandle,buffer))) {
			unequal = strcmp(rCursor, string);
			if (unequal && shouldDelete) {
				bytesToWrite = convertDoubleToSingleChar(tempLine,rCursor,TRUE);
				ret = write(wHandle,(unsigned long)tempLine<<1,bytesToWrite);
			}
			if (!unequal) {
				ret = 0;
				if (!shouldDelete)	// no need to continue while() 
					break;  		//if already found text and not copying file
			}	
		}
		close(rHandle);
		if (shouldDelete) {
			close(wHandle);
			i = unlink((LPSTR)filename);
			if (i != -1) {
				i = rename((LPSTR)tempFilename,(LPSTR)filename);
//				i = _copy((LPSTR)tempFilename,(LPSTR)filename);
				if (i != 0)
					i = unlink((LPSTR)tempFilename);
			}
			ret = i;
		}
	}
	return ret;
}

long getFilePosition() {
	return filePosition;
}

void trimFile(char * filePath, unsigned long frameStart, unsigned long frameEnd) {
	const char tempFilename[15] = "a:\\trimmed.tmp";
	const int wordsHeaderSize = 3;
	const int wordsPerFrame = 40;
	const int bigBufferSize = wordsPerFrame * 10;
	const int sacm_mode = 0x7d00;
	int wHandle,rHandle;
	int bufferBig[bigBufferSize];
	int *pBuffer;
	unsigned long wordStart, wordsNewSize,wordsRemaining;
	char header[3];
	char *pHeader;
	long *pSize;
	int ret;
		
	pBuffer = (int *)&bufferBig;
	pHeader = (char *)&header;
	pSize = (long *)&header;
	wordStart = wordsFromFrames(frameStart) + wordsHeaderSize;
	wordsNewSize = wordsFromFrames(frameEnd-frameStart+1);
	*pSize = (long)((wordsNewSize<<1) + 2);
	*(pHeader+2) = sacm_mode;
	rHandle = open((LPSTR)filePath,O_RDONLY);	
	lseek(rHandle,wordStart<<1,SEEK_SET);
	
	wHandle = open((LPSTR)tempFilename,O_CREAT|O_RDWR|O_TRUNC);	
	ret = write(wHandle,(unsigned long)pHeader<<1,6);
	
	for(wordsRemaining=wordsNewSize;wordsRemaining > bigBufferSize;wordsRemaining -= bigBufferSize) {
		ret = read(rHandle,(unsigned long)pBuffer<<1,bigBufferSize<<1);
		ret = write(wHandle,(unsigned long)pBuffer<<1,bigBufferSize<<1);
	}
	for(;wordsRemaining > 0; wordsRemaining -= wordsPerFrame) {
		ret = read(rHandle,(unsigned long)pBuffer<<1,wordsPerFrame<<1);
		ret = write(wHandle,(unsigned long)pBuffer<<1,wordsPerFrame<<1);
	}
	ret = close(rHandle);
	ret = close(wHandle);
	ret = unlink((LPSTR)filePath);
	wait(100);
	ret = unlink((LPSTR)filePath);
	ret = chdir((LPSTR)USER_PATH);
	pHeader = filePath + strlen(USER_PATH);
	ret = unlink((LPSTR)pHeader);
	ret = rename((LPSTR)tempFilename,(LPSTR)filePath);	
}


BOOL readBuffer(int handle, char *buffer, int bytesToRead) {
	// returns TRUE if still more to read and FALSE if last buffer
	//todo: adjust to UTF-16 and others
	int i, bytesRead, wordsRead;
	BOOL ret;
	
	bytesRead = read(handle, (unsigned long)buffer<<1,bytesToRead);
	wordsRead = bytesRead / 2;
	
	if ((bytesRead % 2) == 1) // odd
		*(buffer + wordsRead*2) = *(buffer + wordsRead) & 0x00FF;	//   
	for (i=wordsRead-1; i>=0; i--) {
		*(buffer + (i*2+1)) = *(buffer + i)>>8;	// right shift word by a byte to get MSByte
		*(buffer + i*2) = *(buffer + i) & 0x00FF;	// save LSByte
	} 

	if (bytesRead < bytesToRead) {					// if we just read the last chunk of data from file
		*(buffer + bytesRead) = 0x00;			
		ret = FALSE;
	} else
		ret = TRUE;
	return ret;
}

char * getLine (int fileHandle, char *buffer) {
	// set buffer to 0 to reset DONE and move to BOF (unless fileHandle == -1, then skip BOF step)
	APP_IRAM static BOOL firstBuffer = TRUE;
	APP_IRAM static BOOL lastBuffer;
	APP_IRAM static BOOL done;
	APP_IRAM static char * cursor;
	char *temp, *line, *ret;
	BOOL lastLine, needBuffer;
	int rewind;

	lastLine = FALSE;
	needBuffer = FALSE;
		
	if (!buffer) { // signal to reset to beginning of file
		if (fileHandle != -1)
			lseek(fileHandle,0,SEEK_SET);
		done = FALSE;
		firstBuffer = TRUE;
	}
	if (done)
		ret = 0;
	else if (buffer) {
		if (firstBuffer) {
			needBuffer = TRUE;
			filePosition = 0;
		}
		do {
			if (needBuffer) {
				needBuffer = FALSE;
				lastBuffer = !readBuffer(fileHandle, buffer, READ_LENGTH);
			    //skip any initial CRLFs
				for (cursor = buffer; *cursor == 0x0a || *cursor == 0x0d; cursor++);

 				if (firstBuffer) {
					firstBuffer = FALSE;
					//todo: make sure this really works
					//if (*cursor == 0xFEFF || *cursor == 0xFFFE)
					//	cursor++;
				} 
			}
			
		    //skip initial whitespace
			for (line = cursor; (*line == ' ') ||  (*line == '\t'); line++);
			// move past eol chars and check for end of buffer
			temp = cursor + strcspn(cursor,"\x0a\x0d");
			lastLine = (*temp == '\0');
			if (!lastLine) {
				*temp = '\0';
				for (cursor = temp+1;*cursor==0x0a || *cursor==0x0d;cursor++);
			} 
			else if (!lastBuffer) { // rewind to refresh buffer and get the line again
				needBuffer = TRUE;
				rewind = line - (buffer + READ_LENGTH); 
				if (rewind)
					filePosition = lseek(fileHandle,rewind, SEEK_CUR);
			} 
			else { // lastLine & lastBuffer
				done = TRUE;
				if (!*line) // return 0 if line is sitting on end-of-file marker
					line = 0;
			}
		} while (needBuffer); 
		ret = line;
		filePosition += (line - buffer);
	}
	return ret;
}

INT16 tbOpen(LPSTR path, INT16 open_flag) {
	const int RETRIES = 3;
	int i;
	INT16 handle;
	//todo: move number of attempts into config file, but have fall back number in define (since config has to be open)

	for (i = 0; i < RETRIES; i++) { 
		handle = open(path, open_flag);
		if (handle < 0)
			logException(22,(const char *)path,0);
		else
			break;
		wait(100);
	}
/*  commenting lines below to let caller decide how to handle inability to open particular file:

	if (handle < 0)
		logException(23,(const char *)path,RESET);
	*/
	return handle;
}

INT16 tbChdir(LPSTR path) {
	const int RETRIES = 3;
	int i;
	INT16 ret;
	//todo: move number of attempts into config file, but have fall back number in define (since config has to be open)

	for (i = 0; i < RETRIES; i++) { 
		ret = chdir(path);
		if (ret < 0)
			logException(24,(const char *)path,0);
		else
			break;
	}
	if (ret < 0)
		logException(25,(const char *)path,RESET);
	return ret;
	 	
}

int fileExists(LPSTR name) {
	int ret;
	struct stat_t statbuf;
	
	ret = stat(name, &statbuf);
//	if((ret == 0) && !(statbuf.st_mode & 0x10)) //exists and not a dir
	if(ret == 0)  // it exists - could be a directory - use line above for file only test
		ret = 1;
	else 
		ret = 0;
	return ret;
}

int dirExists(LPSTR name) {
	int ret;
	struct stat_t statbuf;
//	struct f_info f_info; 
	
//	ret = _findfirst(name, &f_info, D_DIR);
//	if (ret >= 0)
	ret = stat(name, &statbuf);
	if((ret == 0) && (statbuf.st_mode & 0x10)) // exists and is a directory
		ret = 1;
	else 
		ret = 0;
	return ret;
}

