// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/talkingbook.h"
#include "Include/device.h"
#include "Include/audio.h"
#include "Include/files.h"
#include "Include/filestats.h"
#include "Include/sys_counters.h"

APP_IRAM static long filePosition;
APP_IRAM char logBuffer[LOG_BUFFER_SIZE];
APP_IRAM int idxLogBuffer;

extern APP_IRAM unsigned int vCur_1;
extern void refuse_lowvoltage(int);
static int copyfiles(char *, char *);
extern 	int convertTwoByteToSingleChar(unsigned int *, const unsigned int *, int);

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

void logString(char *string, int whenToWrite, int logPriority) {
	int len, available;

	if (logPriority > DEBUG_MODE)
		return; // not worth logging, according to config file
	if (LOG_FILE) {
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
		if (whenToWrite==ASAP && !SACM_Status())
			flushLog();
		if (available < (LOG_BUFFER_SIZE - 72))
			flushLog();
	}
}

void flushLog(void) {

	if (idxLogBuffer && LOG_FILE && !SACM_Status()) {	
		appendStringToFile(LOG_FILE,logBuffer);	
		idxLogBuffer = 0;
	}
}
void forceflushLog(void) {
	if(SACM_Status()) {
		Snd_Stop();
		while(SACM_Status())
			;
	}
	flushLog();
}
void
saveLogFile() {
	char newlogname[128], *cp, strwrk[8];
	int i;

	extern SystemCounts systemCounts;
	
	mkdir((LPSTR)"a:/archive");
	
	strcpy(newlogname, "a:/archive/");
	cp = strrchr(LOG_FILE, '/') + 1;
	longToDecimalString(systemCounts.powerUpNumber, strwrk, 4);
	strcat(newlogname, strwrk);
	strcat(newlogname,"_");
	strcat(newlogname, cp);
	
//	logString(newlogname,BUFFER,LOG_ALWAYS);
	forceflushLog();
	
	i = unlink((LPSTR)newlogname);
	i = rename((LPSTR)LOG_FILE, (LPSTR)newlogname);
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

	rHandle = tbOpen((LPSTR)(filename),O_RDONLY);
	if (rHandle == -1) { // if file to insert into doesn't exist, just create it
		ret = insertStringInNewFile(filename,strText);	
		return ret;
	}
	strcpy(wFilepath,"temp.txt");
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

int insertStringInNewFile (const char * filename, char * strText) {
	int handle, ret;
	unsigned int bytesToWrite;

	if(vCur_1 < V_MIN_SDWRITE_VOLTAGE) {
		refuse_lowvoltage(0);
		return -1;
	}

	ret = -1;
	handle = tbOpen((LPSTR)filename,O_CREAT|O_RDWR|O_TRUNC);
	if (handle == -1)
		ret = -1;
	else {
		bytesToWrite = convertDoubleToSingleChar(strText,strText,TRUE);
	 	ret = write(handle, (unsigned long)strText << 1, bytesToWrite);
		close(handle);
	}
	return ret;
}

int appendStringToFile(const char * filename, char * strText) {
	// WARNING: This function destroys strText. TODO: fix this by using a buffer 
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
	if (handle == -1)
		ret = insertStringInNewFile (filename,strText);
	else {
		lseek(handle,0,SEEK_END);
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
		
	rHandle = tbOpen((LPSTR)filename,O_RDONLY);
	if (rHandle == -1)
		return -1;
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
		if (wHandle == -1)
			logException(35,(const char *)"can't create new file",RESET);
	}
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
	return ret;
}

long getFilePosition() {
	return filePosition;
}

void trimFile(char * filePath, unsigned long frameStart, unsigned long frameEnd) {
	const char tempFilename[15] = "a:/trimmed.tmp";
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
	rHandle = tbOpen((LPSTR)filePath,O_RDONLY);	
	lseek(rHandle,wordStart<<1,SEEK_SET);
	
	wHandle = tbOpen((LPSTR)tempFilename,O_CREAT|O_RDWR|O_TRUNC);	
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
	//TODO: investigate need to have an array of the static variables below, indexed to filehandle
	//      As it stands now, it can only keep track of one filehandle at a time.
	//
	// Set buffer to 0 to reset DONE and move to BOF (unless fileHandle == -1, then skip BOF step)
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
			temp = cursor + strcspn(cursor,"\x0a\x0d");  //TODO: does this assume MS DOS/Win eol style?
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
	const int RETRIES = 2;
	int i;
	INT16 handle;

	if (SACM_Status())
		Snd_Stop(); // DO NOT use stop() here because that calls flushLog(), which eventually calls this fct.
	
	//todo: move number of attempts into config file, but have fall back number in define (since config has to be open)
	for (i = 0; i < RETRIES; i++) { 
		handle = open(path, open_flag);
		if (handle >= 0)
			break;
		wait(100);
	}
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

int fileCopy(char * from, char * to) {
	long buffer[COPY_BUFFER_SIZE];
	long rCount, wCount;
	int wHandle, rHandle, ret;
	unsigned int loopCount;
	
	ret = 0;	
	rHandle = tbOpen((LPSTR)from,O_RDONLY);
	wHandle = tbOpen((LPSTR)to,O_CREAT|O_TRUNC|O_WRONLY);
//	*P_WatchDog_Ctrl &= ~0x4007; // clear bits 0-2 for 2 sec and bit 14 to select system reset
//	*P_WatchDog_Ctrl |= 0x8000; // set bit 15 to enable watchdog

	if (rHandle >= 0 && wHandle >= 0) {
		do {
//			*P_WatchDog_Clear = 0xA005; 	
			if (!(loopCount++ % 64))
				playBip();
			rCount = read(rHandle,(unsigned long)&buffer<<1,COPY_BUFFER_SIZE<<2);
			if (rCount > 0) {
				wCount = write(wHandle,(unsigned long)&buffer<<1,rCount);
				if (wCount == -1 || (wCount != rCount)) {
					ret = -1;
					break;
				}
			} else if (rCount == -1) {
				ret = -1;
				break;
			}
		} while (rCount == (COPY_BUFFER_SIZE<<2));
	} else { // one of the handles < 0
		ret = -1;
	}
	close(wHandle);
	//	*P_WatchDog_Ctrl &= ~0x8000; // clear bit 15 to disable watchdog reset
	close(rHandle);
	playBip();
	return ret;
}

//dirCopy slightly modified version of copyMovedir.  Modified so doesn't delete original files.
void dirCopy(char *fromdir, char *todir, BOOL overwrite) {
// 	copy directory tree below fromdir (all subdirectories and files at all levels)
	int ret, r1, len_from, len_to;
	char from[PATH_LENGTH], fromfind[PATH_LENGTH], to[PATH_LENGTH], lastdir[FILE_LENGTH], first_dir[PATH_LENGTH];
	unsigned int i, k;
	struct f_info fi;
	
	strcpy(from, fromdir);
	len_from = strlen(from);
	
	if(from[len_from-1] != '/') {
		strcat(from, "/");
		len_from++;
	}
	strcat(from, "*");
	
	strcpy(to, todir);
	len_to = strlen(to);
	ret = mkdir((LPSTR)to);	// just to be safe
	if(to[len_to-1] != '/') {
		strcat(to, "/");
		len_to++;
	}
	strcpy(fromfind,from);
	
	ret =_findfirst((LPSTR)fromfind, &fi, D_DIR);
	from[len_from] = 0;
	lastdir[0] = 0;
	strcpy(first_dir,fi.f_name);
	
	i=0;
	while (ret >= 0) {
		i++;
		k=0;
		if(fi.f_name[0]!='.') {
		
			from[len_from] = 0;
			to[len_to]= 0;
			
			strcat(from, fi.f_name);
			strcat(to, fi.f_name);
			
			r1 = mkdir((LPSTR)to);
	
			dirCopy (from, to, overwrite);
		}

		ret =_findfirst((LPSTR)fromfind, &fi, D_DIR);  //necessary to reset after rmdir? 
		while(k < i){
			ret = _findnext(&fi);
			k++;
		}
	} 
	
	from[len_from] = 0;
	to[len_to]= 0;
	copyAllFiles(from, to, overwrite);
}
//copyAllFiles slightly modified version of copyfiles from inbox.c.  Modified so doesn't check for path "a:"
void copyAllFiles(char *fromdir, char *todir, BOOL overwrite)
{
	int ret, r1, len_from, len_to;
	char from[80], to[80];
	struct f_info fi;
	
	strcpy(from, fromdir);
	
	len_from = strlen(from);
	
	if(from[len_from-1] != '/') {
		strcat(from, "/");
		len_from++;
	}
	strcat(from, "*.*");
	
	strcpy(to, todir);
	len_to = strlen(to);
//	mkdir(to);	// just to be safe
	if(to[len_to-1] != '/') {
		strcat(to, "/");
		len_to++;
	}
			
	ret =_findfirst((LPSTR)from, &fi, D_FILE);
	while(ret >= 0) {
		if(fi.f_name[0] != '.') {
			from[len_from] = 0;
			to[len_to]= 0;
			strcat(from, fi.f_name);
			strcat(to, fi.f_name);
			
			setLED(LED_GREEN,FALSE);
			setLED(LED_RED,TRUE);
			if(overwrite || !fileExists((LPSTR)to)){
				unlink((LPSTR)to);
				r1 = _copy((LPSTR)from, (LPSTR)to);
				wait (500);
				setLED(LED_RED,FALSE);
				if (r1 != -1) {
					setLED(LED_GREEN,TRUE);
					wait(500);
				}
			}
		}
		ret = _findnext(&fi);
	}
}

void deleteAllFiles(char *fromdir)
{
	int ret, len_from;
	//int fret;
	char from[80];
	//char temp[80];
	struct f_info fi;
	
	
	logString("deleteAllFiles",BUFFER,LOG_NORMAL);
	logString(fromdir, BUFFER,LOG_NORMAL);
	
	strcpy(from, fromdir);
	len_from = strlen(from);
	
	if(from[len_from-1] != '/') {
		strcat(from, "/");
		len_from++;
	}
	strcat(from, "*.*");
	
	ret =_findfirst((LPSTR)from, &fi, D_FILE);
	while(ret >= 0) {
		if(fi.f_name[0] != '.') {
			from[len_from] = 0;
			strcat(from, fi.f_name);
			
			setLED(LED_GREEN,FALSE);
			setLED(LED_RED,TRUE);
			unlink((LPSTR)from);
			logString(from,ASAP,LOG_DETAIL);
//			wait (500);
//			setLED(LED_RED,FALSE);
//			if (r1 != -1) {
//				setLED(LED_GREEN,TRUE);
//				wait(500);
//			}
		}
		ret = _findnext(&fi);
	}
}

void moveAudioFiles(char *fromdir, char *todir)
{
	int ret, r1, len_from, len_to;
	char* cursor;
	char from[80], to[80];
	struct f_info fi;
	
	strcpy(from, fromdir);
	
	len_from = strlen(from);
	
	if(from[len_from-1] != '/') {
		strcat(from, "/");
		len_from++;
	}
	strcat(from, "*.*");
	
	strcpy(to, todir);
	len_to = strlen(to);
//	mkdir(to);	// just to be safe
	if(to[len_to-1] != '/') {
		strcat(to, "/");
		len_to++;
	}
			
	ret =_findfirst((LPSTR)from, &fi, D_FILE);
	while(ret >= 0) {
		cursor = strchr((char *)fi.f_name,'.');

		if(fi.f_name[0] != '.' && !strcmp(cursor,(char *)AUDIO_FILE_EXT)) {
			from[len_from] = 0;
			to[len_to]= 0;

			strcat(from, fi.f_name);
			strcat(to, fi.f_name);

			setLED(LED_GREEN,FALSE);
			setLED(LED_RED,TRUE);
			unlink((LPSTR)to);
			r1 = rename((LPSTR)from, (LPSTR)to);

			wait (500);
			setLED(LED_RED,FALSE);
			if (r1 != -1) {
				setLED(LED_GREEN,TRUE);
				wait(500);
			}
		}
		ret = _findnext(&fi);
	}
}
unsigned int
loadLanglisttoMemory(char *masterlist,  MLENTRY mla[], unsigned int mla_size)
{
	int mlfd, i;
	unsigned int ret = 0;
	char *line;
	char buffer[READ_LENGTH+1];
	MLENTRY *mlp;
	
	mlfd = tbOpen((LPSTR)masterlist, O_RDONLY);
	if(mlfd < 0) {
		return(ret);
	}
	getLine(-1,0); // reset
	for(i=0, mlp=mla; i<mla_size; i++, mlp++) {
		line = getLine(mlfd, buffer);
		while (*line == '!' || *line == '$') // skip lock symbol and system symbol
			line++;
		if(line == 0)
			break;
		categoryStringtoLong(line, mlp);
	}
	close(mlfd);
	ret = i;
	return(ret);
}
void
categoryStringtoLong(char *cat, MLENTRY *mlp)
{
	unsigned char *cp, *base;
	unsigned int i;
	unsigned long l = 0;
	
	for(i=24, base=cat; i>=0; i-=8) {
		cp = strchr(base, '-');
		if (cp) {
			*cp = 0;			
		}
		l |= (unsigned long) (((unsigned long)(strToInt(base))) << i);
		if (cp) {
			*cp = '-'; //restore the string
		} else {
			break;
		}
		base = cp + 1;
	}
	
	*mlp = l;		
}
void
categoryLongtoString(char *cat, MLENTRY *mlp)
{
	unsigned long wrk;
	char tmp[8], *cp;
	
	
	wrk = (*mlp & 0xff000000) >> 24;
	unsignedlongToDecimalString(wrk, tmp, 3);
	cp = tmp;
	if (*cp == '0') cp++;
	if (*cp == '0') cp++;
	strcpy(cat, cp);
	
	wrk = (*mlp & 0x00ff0000) >> 16;
	if(wrk == 0)
		return;
	unsignedlongToDecimalString(wrk, tmp, 3);
	strcat(cat, "-");
	cp = tmp;
	if (*cp == '0') cp++;
	if (*cp == '0') cp++;
	strcat(cat, cp);

	wrk = (*mlp & 0x0000ff00) >> 8;
	if(wrk == 0)
		return;
	unsignedlongToDecimalString(wrk, tmp, 3);
	strcat(cat, "-");
	cp = tmp;
	if (*cp == '0') cp++;
	if (*cp == '0') cp++;
	strcat(cat, cp);
	
	wrk = (*mlp & 0x000000ff) >> 8;
	if(wrk == 0)
		return;
	unsignedlongToDecimalString(wrk, tmp, 3);
	strcat(cat, "-");
	cp = tmp;
	if (*cp == '0') cp++;
	if (*cp == '0') cp++;
	strcat(cat, cp);
}

//
// copy all files in fromdir to todir
//
static int copyfiles(char *fromdir, char *todir)
{
	int ret, r1, len_from, len_to, fret;
	char from[80], to[80];
	struct f_info fi;
	
	fret = 0;
	
	strcpy(from, fromdir);
	len_from = strlen(from);
	
	if(from[len_from-1] != '/') {
		strcat(from, "/");
		len_from++;
	}
	strcat(from, "*.*");
	
	strcpy(to, todir);
	len_to = strlen(to);
//	mkdir(to);	// just to be safe
	if(to[len_to-1] != '/') {
		strcat(to, "/");
		len_to++;
	}
			
	ret =_findfirst((LPSTR)from, &fi, D_FILE);
	while(ret >= 0) {
		if(fi.f_name[0] != '.') {
			from[len_from] = 0;
			to[len_to]= 0;
			strcat(from, fi.f_name);
			strcat(to, fi.f_name);
			if((lower(from[0]) == 'a') && (lower(to[0]) == 'a')) {
				unlink((LPSTR)to);
				r1 = rename((LPSTR)from, (LPSTR)to);
			} else {
				setLED(LED_GREEN,FALSE);
				setLED(LED_RED,TRUE);
				r1 = _copy((LPSTR)from, (LPSTR)to);
// speed up 				wait (500);
				setLED(LED_RED,FALSE);
				if (r1 != -1) {
					setLED(LED_GREEN,TRUE);
// speed up					wait(500);
				}
			}
//			logString((char *)"FROM/TO:",BUFFER);
//			logString(from,BUFFER);
//			logString(to,ASAP);
		}
		ret = _findnext(&fi);
		fret++;
	}
	return(fret);
}

int 
copyMovedir(char *fromdir, char *todir) {
// 	copy directory tree below fromdir (all subdirectories and files at all levels)
	int ret, r1, len_from, len_to, len, fret;
	char from[PATH_LENGTH], fromfind[PATH_LENGTH], to[PATH_LENGTH], lastdir[FILE_LENGTH];

	struct f_info fi;
	
	fret = 0;
	
	strcpy(from, fromdir);
	len_from = strlen(from);
	
	if(from[len_from-1] != '/') {
		strcat(from, "/");
		len_from++;
	}
	strcat(from, "*");
	
	strcpy(to, todir);
	len_to = strlen(to);
	ret = mkdir((LPSTR)to);	// just to be safe
	if(to[len_to-1] != '/') {
		strcat(to, "/");
		len_to++;
	}
	strcpy(fromfind,from);
	ret =_findfirst((LPSTR)fromfind, &fi, D_DIR);
	from[len_from] = 0;
	lastdir[0] = 0;
	
	while (ret >= 0) {
		if(! (fi.f_attrib & D_DIR)) {
			ret = _findnext(&fi);
			continue;
		}
	
		if(fi.f_name[0]=='.') {
			ret = _findnext(&fi);
			continue;
		}
		from[len_from] = 0;
		to[len_to]= 0;
				
		strcat(from, fi.f_name);
		strcat(to, fi.f_name);
		
		r1 = mkdir((LPSTR)to);
		fret += copyMovedir (from, to);
		ret = rmdir((LPSTR)from);
				
		fret++;
		ret =_findfirst((LPSTR)fromfind, &fi, D_DIR);  //necessary to reset after rmdir? 
	};
	
	from[len_from] = 0;
	to[len_to]= 0;
	fret += copyfiles(from, to);
	cpyTopicPath(from);
	strcpy(lastdir,from+2);
	if ((len = strlen(lastdir)))
		lastdir[len-1] = 0; // remove last '\'
	if (strstr(fromdir,lastdir))
		fret = 0; // prevents system reset if only copying list files
	
	return(fret);
}

int concatFiles(int tofd, LPSTR fromname) {
	char strLog[PATH_LENGTH * 2], buf[1024];
	int fromfd, ret = 0, wrk, wrk1, ends_with_lf;
	
	wrk = sizeof(buf);
	
	fromfd = tbOpen(fromname, O_RDONLY);
	if(fromfd < 0) {
		strcpy(strLog, "concatFiles cannot open ");
		strcat(strLog, (char *)fromname);
		logString(strLog, BUFFER,LOG_NORMAL);
		return(-1);
	}
	while((wrk = read(fromfd,(unsigned long)&buf<<1,sizeof(buf)<<1)) > 0) {
		if(wrk & 1) {
			wrk1 = buf[(wrk-1)/2] & 0xff;
		} else {
			wrk1 = buf[(wrk-1)/2] >> 8;
		}
		if(wrk1 == 0xa) {
			ends_with_lf = 1;
		} else {
			ends_with_lf = 0;
		}
		wrk = write(tofd, (unsigned long)&buf<<1, wrk);
		if(wrk <= 0) {
			strcpy(strLog, "concatFiles write failed to ");
			strcat(strLog, (char *)fromname);
			logString(strLog, BUFFER,LOG_NORMAL);
		} else {
			ret += wrk;
		}
	}
	close(fromfd);

	return(ends_with_lf);
}
int
buildExchgOstats() {
	char strLog[PATH_LENGTH * 2], to[PATH_LENGTH], filename[PATH_LENGTH], delim[20];
	int myexchgfd, ret, delim_len, have_new = 0, ends_with_lf;
	struct f_info file_info;
	int expandOstatFile(char *);

	
	strcpy(delim,DELIM);
	delim_len = convertDoubleToSingleChar(delim,delim,FALSE);
	
	strcpy(to, OSTAT_DIR);
	strcat(to, (const char *)TB_SERIAL_NUMBER_ADDR);
	strcat(to, OSTATS_EXCHG_EXT);

	// process any ostats files from other devices	
	strcpy(filename,OSTAT_DIR); 
	strcat(filename, "*");
	strcat(filename, OSTATS_EXCHG_EXT);
	
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	
	for (;ret >= 0; ret = _findnext(&file_info)) {
	
		strcpy(filename,OSTAT_DIR); 
		strcat(filename,file_info.f_name);
		
		if(!strcmp(to, filename)) {	// don't process this device's concatenated ostats file
			continue;
		}
		
		ret = expandOstatFile( (char *)filename);	// will delete the file
		have_new++;
		strcpy(strLog, "buildExchgOstats processed ");
		strcat(strLog, (char *) filename);
		logString(strLog, BUFFER,LOG_NORMAL);
	}
	
	if(have_new == 0) {
		if(fileExists((LPSTR)to)) {
			strcpy(strLog, "buildExchgOstats returns, no new ostat data");
			logString(strLog, BUFFER,LOG_NORMAL);
			return(0);
		}
	}

	myexchgfd = tbOpen((LPSTR)to, O_CREAT|O_RDWR|O_TRUNC);
	if(myexchgfd < 0) {   // can't open exchange file
		strcpy(strLog, "buildExchgOstats can't open ");
		strcat(strLog, to);
		logString(strLog, BUFFER,LOG_ALWAYS);
		return(-1);
	}
//
//  concatenate all our ostats csv files into one OSTATS_EXCHG_EXT file
	strcpy(filename,OSTAT_DIR); 
	strcat(filename,"*.csv");

	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);

	for (;ret >= 0; ret = _findnext(&file_info)) {
		
		strcpy(filename,OSTAT_DIR); 
		strcat(filename,file_info.f_name);
		
		ends_with_lf = concatFiles(myexchgfd, (LPSTR)filename);
		if(ends_with_lf) {
			ret = write(myexchgfd, (unsigned long)(&delim[1])<<1, delim_len-2);
		} else {
			ret = write(myexchgfd, (unsigned long)&delim<<1, delim_len);
		}
		strcpy(strLog, "buildExchgOstats processed ");
		strcat(strLog, (char *) filename);
		logString(strLog, BUFFER,LOG_DETAIL);
	}
	
	strcpy(strLog, "buildExchgOstats created new ostat data file ");
	strcat(strLog, to);
	logString(strLog, BUFFER,LOG_DETAIL);

	close(myexchgfd);
}

// process an ostats exchange file from some other device
//     this file is a concatenation of all ostats files from the other device
//     a line beginning with OSTATS_DELIM separates different devices data in this file
// if the other device has stats from a third device:
//    add to our ostats if we have not seen the third device
//    replace our ostats for third device if the boot sequence number in tis file is larger than what we have now
// 
int
expandOstatFile(char *filename) {
	char strLog[PATH_LENGTH * 2], to[PATH_LENGTH], from[PATH_LENGTH], buffer[READ_LENGTH+1];
	char *line, *cp, *cp1, verbuf[PATH_LENGTH+1];
	int fromfd, ret, checkfd, len, state = FIND_SRN, bytestowrite;
	unsigned long newver, diskver;
	
	buffer[READ_LENGTH] = '\0';
	strcpy(from, filename);
	
	fromfd = tbOpen((LPSTR)filename, O_RDONLY);
	if(fromfd < 0) {
		strcpy(strLog,"expandOstatFile can't open ");
		strcat(strLog, filename);
		logString(strLog, BUFFER,LOG_DETAIL);
	} else {
		strcpy(strLog, "expandOstatFile processing ");
		strcat(strLog, filename);
		logString(strLog, BUFFER,LOG_DETAIL);
	}
	getLine(-1,0); // reset
	
	while((line = getLine(fromfd, buffer))) {   
		len = strlen(line);
		if(len <= 1)
			continue;
		
		switch(state) {
		case FIND_SRN:		// we are looking for a line starting with a device serial number
			// does line start with serial number prefix
			if(strncmp(CONST_TB_SERIAL_PREFIX, line, strlen(CONST_TB_SERIAL_PREFIX)))
				break;  // nope
				
			strcpy(from, line);
			cp = strchr(from, ',');
			if(cp == NULL)
				break;
			*cp = 0;
			if(!strcmp(from, (const char *)TB_SERIAL_NUMBER_ADDR)) {
				state = FIND_DELIM;
				break;   // its me, some other device reported this device's stats
			}
			newver = strToLong(cp+1);
			strcpy(to,OSTAT_DIR);
			strcat(to, from);
			strcat(to,".csv");
			checkfd = tbOpen((LPSTR)to, O_RDONLY);
			if(checkfd >= 0) {   //a csv from this third device exists on this device
				ret = read(checkfd,(unsigned long)&verbuf<<1,PATH_LENGTH);
				verbuf[PATH_LENGTH] = 0;
				ret = convertTwoByteToSingleChar((unsigned int *)strLog, (unsigned int *)&verbuf, PATH_LENGTH/2);
				close(checkfd);	
				cp1 = strchr(strLog, ',');
				if(cp1 == NULL)
					break;
				*cp1++;
				diskver = strToLong(cp1);
			} else {
				diskver = -1;
			}
			state = FIND_DELIM;  // may be reset shortly
			if(diskver < newver) {
				strcpy(strLog, to);
				strcat(strLog, " newer from other device");
				logString(strLog,BUFFER,LOG_DETAIL);  
				checkfd = tbOpen((LPSTR)to, O_WRONLY|O_CREAT|O_TRUNC);
				if(checkfd >= 0) {
					state = PROCESS_STATS;
					*cp = ',';
					bytestowrite = convertDoubleToSingleChar(strLog,line,TRUE);
					ret = write(checkfd, (unsigned long)&strLog<<1, bytestowrite);
				}
			} else {
				strcpy(strLog, to);
				strcat(strLog, " my stats are newer");
				logString(strLog,BUFFER,LOG_DETAIL);  
			}
			break;
			
		case PROCESS_STATS:
			// writing some other devices stats to our ostat dir
			if(!strncmp(line, OSTATS_DELIM, 3)) {	// line starts with the delimiter
				close(checkfd);
				state = FIND_SRN;
				break;
			}
			bytestowrite = convertDoubleToSingleChar(strLog,line,TRUE);
			ret = write(checkfd, (unsigned long)&strLog<<1, bytestowrite);
			break;
			
		case FIND_DELIM:
			// we are searching for a line beginning with our delimiter 
			if(!strncmp(line, OSTATS_DELIM, 3)) {
				state = FIND_SRN;
				break;
			}

		default:
			break;
		}
	}
	
	close(fromfd);
	unlink((LPSTR)filename);
	
	strcpy(strLog, "expandOstatFile deleting ");
	strcat(strLog, filename);
	logString(strLog, BUFFER,LOG_DETAIL);
	
	return(0);
}

int 
replaceFromBackup(char *path)
{
	int ret = 0, srcexists = 0;
	struct stat_t statbuf, bkstatbuf;
	char temp[12];
	char bkPath[PATH_LENGTH], mypath[PATH_LENGTH];
	char * wrkpath;
	char msg[128];
	
//	strcpy(msg, "REP 1 ");
//	strcat(msg, path);
//	logString(msg, BUFFER, LOG_ALWAYS);
	
	ret = stat((LPSTR)path, &statbuf);
	if(ret == 0) {  // doesn't exist
		srcexists = 1;
	}
	
	strcpy(bkPath, BKPATH);
	strcat(bkPath, strrchr(path, '/') + 1);
	
//	strcpy(msg, "REP 2 ");
//	strcat(msg, bkPath);
//	logString(msg, BUFFER, LOG_ALWAYS);
	
	ret = stat((LPSTR)bkPath, &bkstatbuf);
	if(ret != 0) {  // doesn't exist, we don't have a backup
		return(ret);
	}
	
	if(srcexists == 1) {
		mkdir((LPSTR) LOST);
		strcpy(mypath, LOST);
		wrkpath = path;
		if(path[0] == 'a' && path[1] == ':') {
			wrkpath += 2;
		}
		if(bkstatbuf.st_mode & 0x10) {  // a directory		
			strcat(mypath, wrkpath);
		} else {
			strcat(mypath, strrchr(path, '/'));
		}
		strcat(mypath, "_");
		
		longToDecimalString(systemCounts.powerUpNumber,(char *)temp,4);
		temp[4] = 0;
		strcat(mypath, temp);
		
		rename((LPSTR) path , (LPSTR) mypath);
			
//		strcpy(msg, "REP 3 ");
//		strcat(msg, mypath);
//		logString(msg, BUFFER, LOG_ALWAYS);
	}
	
	if(!(bkstatbuf.st_mode & 0x10))  {  //exists and not a dir
//		logString("REP 4 _copy", BUFFER, LOG_ALWAYS);
//		logString(path, BUFFER, LOG_ALWAYS);
//		logString(bkPath, BUFFER, LOG_ALWAYS);
///		_copy((LPSTR)bkPath, (LPSTR)path);  // src, dest
		fileCopy((LPSTR)bkPath, (LPSTR)path);  // from, to
	} else {
		mkdir((LPSTR) path);
		dirCopy((char *) bkPath, (char *)path, 1);  // from, to
	}
}
