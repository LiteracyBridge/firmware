// Copyright 2009,2010 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "Include/talkingbook.h"
#include "Include/files.h"
#include "Include/pkg_ops.h"
#include "Include/device.h"
#include "Include/containers.h"
#include "Include/audio.h"

extern unsigned long RES_DING_A18_SA;
extern unsigned long RES_BIP_A18_SA;
extern unsigned long User_GetCurDecodeLength(void);
extern int SACMFileHandle;
extern void User_SetDecodeLength(unsigned long);

static int getFileHandle (CtnrFile *);
static void playLongInt(CtnrFile *, unsigned long);
static int recordAudio(char *, char *);
APP_IRAM static char lastFilenameRecorded[FILE_LENGTH];
extern APP_IRAM unsigned int vCur_1;

// added for meta data 
#define CURRENT_POS 0xffffffffL
#define DC_IDENTIFIER 10 
#define METADATA_VERSION 1

int writeLE32(int filehandle,long value, long offset);
int writeLE16(int handle, unsigned int value, long offset);
int addField(int handle, unsigned int field_id, char *field_value);

APP_IRAM long metadata_start;
APP_IRAM long metadata_numfields;
#include "Include/sys_counters.h"
extern APP_IRAM SystemCounts systemCounts;
// end meta data additions

void playDing(void) {
	Snd_SACM_PlayMemory(C_CODEC_AUDIO1800,RES_DING_A18_SA);	
	while (SACM_Status());
}

void playBip(void) {
	Snd_SACM_PlayMemory(C_CODEC_AUDIO1800,RES_BIP_A18_SA);	
	while (SACM_Status());
}

void playBips(int count) {
	int i;

	for (i=0; i<count;i++)
		playBip();	
}

void playDings(int count) {
	int i;

	for (i=0; i<count;i++)
		playDing();	
}
unsigned long wordsFromFrames(unsigned long frames) {
	unsigned long words = frames * 40;
	return words;
}

unsigned long framesFromBytes(unsigned long bytes) {
	unsigned long frames = bytes / 80;	
	return frames;
}

unsigned long msecFromFrames(unsigned long frames) {
	unsigned long msec = frames * 20;
	return msec;
}

unsigned long framesFromMSec(unsigned long msec) {
	unsigned long frames = msec / 20;
	return frames;	
}

unsigned long getCurrentFrame(void) {
	unsigned long wordsPerFrame = SACM_A1800_Mode / 800;
	unsigned long word = ( unsigned long )User_GetCurDecodeLength();
	unsigned long frame = word / wordsPerFrame;
	return frame;	
}

int gotoFrame(unsigned long frameDest) {
	//const int bytesHeaderSize = 6;
	unsigned long wordsPerFrame = SACM_A1800_Mode / 800;
	unsigned long byteCurrent;
	unsigned long wordDiff;  ///, timeDiff;
	unsigned long wordCurrent, wordDest, frames; 
//	unsigned long timeNow, timeDest;
	
/*	timeNow = Snd_A1800_GetCurrentTime();	
	timeDest = frameDest * 20;
	timeDiff = timeDest - timeNow;
	if (timeDiff > 0)
		SACM_A1800FAT_SeekTime(timeDiff,0);
	else if (timeDiff < 0)
		SACM_A1800FAT_SeekTime(timeDiff,1);
*/	
	pause();
	byteCurrent = lseek(SACMFileHandle,0,SEEK_CUR);
	wordCurrent = ( unsigned long )User_GetCurDecodeLength();
	frames = wordCurrent / wordsPerFrame;
	wordCurrent = frames * wordsPerFrame;
	wordDest = frameDest * wordsPerFrame;
	if (wordDest > wordCurrent) {
		wordDiff = wordDest - wordCurrent;	
		byteCurrent = lseek(SACMFileHandle,wordDiff<<1,SEEK_CUR);
	} else {
		wordDiff = wordCurrent - wordDest;	
		byteCurrent = lseek(SACMFileHandle,-wordDiff<<1,SEEK_CUR);
	}
	User_SetDecodeLength(wordDest);
	resume();	
	return 0;
}

unsigned long setFileHeader(char *filePath, unsigned long frames) {
	//input: frames; return: bytes
	unsigned long bytes;
	char *header;
	int handle;
		
	handle = open((LPSTR)(filePath),O_RDWR); //read&write mode
	bytes = wordsFromFrames(frames)<<1;
	bytes += 2; // for extra SACM Mode word
	header = (char *)&bytes;
	lseek(handle,0,SEEK_SET);
	write(handle,(unsigned long)header<<1,4);
	close(handle);
	return bytes;
}

void playActionSound(EnumAction action) {
	switch (action) {
		case JUMP_TIME:
			playBip();
			break;
		default:
			playBip();
			break;
	}
}

void pause(void) {
	SACM_Pause();
	turnAmpOff();
}	

void resume(void) {
	turnAmpOn();
	SACM_Resume();
}

void stop(void) {
	context.isStopped = TRUE;
	Snd_Stop();
	turnAmpOff();
	if (GREEN_LED_WHEN_PLAYING) {	
		setLED(LED_GREEN,FALSE);
	}
	flushLog();
}

static int getFileHandle (CtnrFile *newFile) {
	int ret = 0; 
	char sTemp[PATH_LENGTH];
	BOOL getLastRecording = FALSE;
	CtnrPackage *pkg;
		
	pkg = getPackageFromFile(newFile); // get package that applied to file, rather than context package
	// This is necessary for user packages inserting system sounds.

	// check for list
	if (newFile->idxFirstBlockInFile == -1)
		strcpy(sTemp,LIST_PATH);
	else if (newFile->idxFilename == -1) {  // use last file recorded
		logException(0,0,USB_MODE); // this should never happen -- removed this feature
		// check for last file recorded (parsed from $L)
		//getLastRecording = TRUE;
		//ret = tbChdir((unsigned long)(DRAFT_PATH));
	} 
	else { // not a list or last recording; just a normal file	
		switch (pkg->pkg_type) {
			case PKG_SYS:
				strcpy(sTemp,SYSTEM_PATH);
				strcat(sTemp,pkg->strHeapStack + pkg->idxName);
				strcat(sTemp,"\\");
				break;		
			case PKG_APP:
				strcpy(sTemp, USER_PATH);
				strcat(sTemp,pkg->strHeapStack + pkg->idxName);
				strcat(sTemp,"\\");
				break;
			case PKG_MSG:	
				strcpy(sTemp, USER_PATH);
				break;
		}
	}
	if (getLastRecording)
		strcpy(sTemp,lastFilenameRecorded);
	else {
		strcat(sTemp,pkg->strHeapStack + newFile->idxFilename);
		strcat(sTemp,AUDIO_FILE_EXT);
	}
	ret = tbOpen((LPSTR)sTemp,O_RDONLY);
//	logString(sTemp,ASAP);
	return ret;
}

void play(CtnrFile *file, unsigned int startingPoint) {
	unsigned long l;
	
	l = extractTime(startingPoint, context.package->timePrecision);
	playLongInt(file,l);
}

static void playLongInt(CtnrFile *file, unsigned long lTimeNew) {
	// This fct is usually just called by play(), but is also used directly by insertSound()
	// so that a hyperlink sound can be inserted into playing audio and return as close as possible to
	// the last point played in the audio.
	APP_IRAM static int iFileHandle;
	unsigned long lTimeCurrent;
	long lDifference;
	unsigned long ulDifference;
	
	if (context.lastFile && (context.lastFile->idxFilename == file->idxFilename && SACM_Status())) {
		lTimeCurrent = Snd_A1800_GetCurrentTime();
		lDifference = lTimeNew - lTimeCurrent;
		if (lDifference >= 0)
			SACM_A1800FAT_SeekTime(lDifference,FORWARD_SKIP);
		else {
			ulDifference = (unsigned long)-lDifference;
			SACM_A1800FAT_SeekTime(ulDifference,BACKWARD_SKIP);
		}
		if (context.isPaused) {
			resume();  // todo: is it better to resume and then seek?
			context.isPaused = FALSE;
		}
		if (GREEN_LED_WHEN_PLAYING)
			setLED(LED_GREEN,TRUE);
	} else {
		stop();
		close(iFileHandle);
		if (file->idxFilename != PKG_HEAP_SIZE)  // don't remember "stack-stored temp" pointers
			context.lastFile = file;  
		else
			context.lastFile = NULL; // to force to assume new file when going back to non-list file
		iFileHandle = getFileHandle(file);
		if (iFileHandle >= 0) {  //allows mistakes or dummy files to pass without problem
			SACMGet_A1800FAT_Mode(iFileHandle,0);
			Snd_SACM_PlayFAT(iFileHandle, C_CODEC_AUDIO1800);	
			if (lTimeNew)
				SACM_A1800FAT_SeekTime(lTimeNew,FORWARD_SKIP);
			if (GREEN_LED_WHEN_PLAYING)
				setLED(LED_GREEN,TRUE);
			context.isStopped = FALSE;
		}
	}
	context.isPaused = FALSE;
	//TODO: jump to block from processButtonEvent info
}

void insertSound(CtnrFile *file, CtnrBlock *block, BOOL wait) {
	unsigned long lastPlayedPoint;
	CtnrFile *lastFilePlayed;
	unsigned long lBlockEndPoint;
	int keystroke = 0;
	int isPlayerStopped = !SACM_Status();
	BOOL wasPaused;
	
	wasPaused = context.isPaused;
	lastFilePlayed = context.file;
	context.file = file;
	if (block)
		lBlockEndPoint = extractTime(block->endTime,context.package->timePrecision);
	if (!isPlayerStopped) {
		lastPlayedPoint = Snd_A1800_GetCurrentTime();
		if (lastPlayedPoint < INSERT_SOUND_REWIND_MS)
			lastPlayedPoint = 0;
		else
			lastPlayedPoint -= INSERT_SOUND_REWIND_MS;
		//context.file should not be changed until we need them again in a few lines	
		play(context.file,block?block->startTime:0);
		context.isPaused = FALSE;
		while (SACM_Status() && !keystroke) {
			if (block && lBlockEndPoint < Snd_A1800_GetCurrentTime()) {
				stop();
			 	break;
			}
			if (!wait && (keystroke = keyCheck(0))) {
				if (keystroke == KEY_PLAY && !context.isPaused) {
					pause();
					context.isPaused = TRUE;
				} else if (keystroke == KEY_PLAY && context.isPaused) {
					resume();
					context.isPaused = FALSE;
				} else
					context.keystroke = keystroke;
			}
		} 
		context.file = lastFilePlayed;
		playLongInt(context.file,lastPlayedPoint);
		if (wasPaused) {
			pause();
			context.isPaused = TRUE;
		}
	}
	else {
		play(file,block?block->startTime:0);
		context.isPaused = FALSE;
		while (SACM_Status() && !keystroke) {
			if (block && lBlockEndPoint < Snd_A1800_GetCurrentTime()) {
				stop();
			 	break;
			}
			if (!wait && (keystroke = keyCheck(0))) {
				if (keystroke == KEY_PLAY && !context.isPaused) {
					pause();
					context.isPaused = TRUE;
				} else if (keystroke == KEY_PLAY && context.isPaused) {
					resume();
					context.isPaused = FALSE;
				} else
					context.keystroke = keystroke;
			}
		}
		context.file = lastFilePlayed;
	}
	if (!SACM_Status()) {
		context.isStopped = TRUE;
		context.isPaused = FALSE;
		turnAmpOff();
	}
}

void insertSoundFile(int idxFile) {
	insertSound(&pkgSystem.files[idxFile],NULL,FALSE);
}
	
static int recordAudio(char *pkgName, char *cursor) {
	int handle, ret = -1;
	char temp[PATH_LENGTH];
	char filepath[PATH_LENGTH];
    char unique_id[PATH_LENGTH], digits[8];
	long start, end, prev;
	CtnrFile *file;
	int key;
	int low_voltage, v;
	unsigned long wrk1;
	
	strcpy(filepath,USER_PATH);
	strcat(filepath,pkgName);
	strcat(filepath,AUDIO_FILE_EXT);
		
	file = getListFile(cursor);
	insertSound(file,NULL,TRUE);
	start = getRTCinSeconds();
	strcpy(temp,"\x0d\x0a");
	longToDecimalString(start,temp+2,8);
	strcat(temp,(const char *)": RECORD ");
	LBstrncat(temp,pkgName,60);
	LBstrncat(temp," -> ",60);
	LBstrncat(temp,cursor,60);	
	logString(temp,BUFFER);
	insertSound(&pkgSystem.files[SPEAK_SOUND_FILE_IDX],NULL,TRUE);
	stop();
	start = getRTCinSeconds();
	prev = end = start;
	handle = tbOpen((LPSTR)filepath,O_CREAT|O_RDWR);
	if (handle != -1) {
		setLED(LED_RED,TRUE);
		playBip();
		turnAmpOff();
		Snd_SACM_RecFAT(handle, C_CODEC_AUDIO1800, BIT_RATE);
		low_voltage = 0;
		do {
			end = getRTCinSeconds();	
			if (0==(end%2) && (prev != end)) { // blink LED every three seconds
				prev = end;
				setLED(LED_RED,FALSE);
				wait (100);
				setLED(LED_RED,TRUE);
				while((v = getCurVoltageSample()) == 0xffff);
				if(vCur_1 < V_MIN_SDWRITE_VOLTAGE) {
					low_voltage = 1;
				}
			}
			key = keyCheck(0);
			if (key == KEY_PLAY) { // pause  TODO: this key press to pause shouldn't be hard coded
				pause();
				setLED(LED_RED,FALSE);
				do
					key = keyCheck(0);
				while (key != KEY_PLAY && key != KEY_STAR);					
				setLED(LED_RED,TRUE);
				resume();
			}
		} while ((key != KEY_STAR) && (low_voltage == 0)); // TODO: this key press to stop shouldn't be hard coded
//		while ((end - start) < 3) { // must be at least 2.0 second recording
//			end = getRTCinSeconds();			
//		}
		SACM_Stop();		//Snd_Stop(); // no need to call stop() and flush the log
		//lseek(handle, 6, SEEK_SET );			//Seek to the start of the file input
		//write(handle,(LPSTR)header<<1,6);
 
// write meta data to end of file
               
        close(handle);	// rhm:  I think its already closed, I can't write to it here
        
       	handle = tbOpen((LPSTR)filepath,O_RDWR);
       	
       	metadata_start = lseek(handle, 0L, SEEK_END);  // offset to start of metadata
        metadata_numfields = 0L; // init num fields
       
		wrk1 = METADATA_VERSION;
        writeLE32(handle, wrk1, CURRENT_POS);  //meta data version = 1
        writeLE32(handle, metadata_numfields, CURRENT_POS); // 4 byte for num fields
        
        strcpy(unique_id, TB_SERIAL_NUMBER_ADDR);
        strcat(unique_id, "_");       
		longToDecimalString(systemCounts.packageNumber,digits,5);
        strcat(unique_id, digits);
        
        addField(handle, DC_IDENTIFIER, unique_id);
        
        metadata_numfields += 1;
        
        // add other fields here
        
        writeLE32(handle, metadata_numfields, metadata_start + 4); // write correct num meta data fields
        
		close(handle);
// done with meta data
        
//		*P_WatchDog_Ctrl &= ~0x8000; // clear bit 15 to disable
		setLED(LED_RED,FALSE);
		turnAmpOn();
		playDing();
		insertSound(file,NULL,TRUE);  // replay subject

		strcpy(temp,"TIME RECORDED (secs): ");
		longToDecimalString((long)end-start,temp+strlen(temp),4);
		strcat(temp,"\x0d\x0a");
		logString(temp,BUFFER);

		strcpy(lastFilenameRecorded,filepath);
		ret = 0;  // used to set this based on fileExists() check, but too slow
	} else {
		logException(16, filepath,RESET);  //can't open file for new recording
	}
	return ret;
}	

int createRecording(char *pkgName, int fromHeadphone, char *listName) {
	int SPINS; //from page 102 of GPL Progammers Manual v1.0/Dec20,2006 
	           //headphone amp audio driver input source select 
	
	markEndPlay(getRTCinSeconds());
		
	if (fromHeadphone) {
		SPINS = 2;
		*P_HQADC_MIC_PGA &= 0xFFE0; // only first 5 bits for mic pre-gain; others reserved
		*P_HQADC_MIC_PGA |= MIC_GAIN_HEADPHONE;
	}
	else {
		SPINS = 0;
		if (MIC_GAIN_NORMAL >= 0) {
			*P_HQADC_MIC_PGA &= 0xFFE0; // only first 5 bits for mic pre-gain; others reserved
			*P_HQADC_MIC_PGA |= MIC_GAIN_NORMAL;
		}
	}	
	*P_HPAMP_Ctrl &= 0xFFF3; // zero bits 2 and 3
	if (SPINS) { // no point in OR'ing with nothing
		SPINS <<= 2; // move SPINS into bits 2 and 3 position
		*P_HPAMP_Ctrl |= SPINS;	
	}

	recordAudio(pkgName,listName);
	if (SPINS)
		*P_HPAMP_Ctrl &= 0xFFF3; // zero bits 2 and 3, returning SPINS to 0
	packageRecording(pkgName,listName); // packageRecording turns it into single byte characters

	return 0;
}

void markEndPlay(long timeNow) {
	long timeDiff;
	char log[40];
	
	if (context.packageStartTime) {
		timeDiff = timeNow - context.packageStartTime;
		context.packageStartTime = 0;
		if (timeDiff > MINIMUM_PLAY_SEC_TO_LOG) {
			strcpy (log,"TIME PLAYED: ");
			longToDecimalString(timeDiff,log+strlen(log),4);
			strcat(log," sec at VOL=");
			longToDecimalString((long)getVolume(),log+strlen(log),2);
			strcat(log,"\x0d\x0a");
			logString(log,BUFFER);
		}
	}
}

void markStartPlay(long timeNow, const char * name) {
	const int LOG_LENGTH = PATH_LENGTH + 20;
	char log[LOG_LENGTH];
	
	context.packageStartTime = timeNow;
//	strcpy(log,"\x0d\x0a");
//	longToDecimalString(timeNow,log+2,8);
	longToDecimalString(timeNow,log,8);
	strcat((char *)log,(const char *)": PLAY ");
	if (LBstrncat((char *)log,name,LOG_LENGTH) == LOG_LENGTH-1)
		log[LOG_LENGTH-2] = '~';
	logString(log,BUFFER);	
}

int writeLE32(int handle, long value, long offset) {
	int ret = 0;
	unsigned char wrk;
	unsigned long wrkl;
    long curpos = lseek(handle, 0, SEEK_CUR);
    if(offset != CURRENT_POS) {
        wrkl = lseek(handle, offset, SEEK_SET);
    }
    wrk = value & 0xff;
    ret += write(handle, (unsigned long) &wrk << 1, 1);
    wrk = (value >> 8) & 0xff;
    ret += write(handle, (unsigned long) &wrk << 1, 1);
    wrk = (value >> 16) & 0xff;
    ret += write(handle, (unsigned long) &wrk << 1, 1);
    wrk = (value >> 24) & 0xff;
    ret += write(handle, (unsigned long) &wrk << 1, 1);
   
 	if(offset != CURRENT_POS) {
    	lseek(handle, curpos, SEEK_SET);
 	}
    
    return(ret);
}
int writeLE16(int handle, unsigned int value, long offset) {
	int ret = 0;
	unsigned char wrk;
	unsigned long wrkl;
	long curpos = lseek(handle, 0, SEEK_CUR);
	if(offset != CURRENT_POS) {
		wrkl = lseek(handle, offset, SEEK_SET);
	}
    wrk = value & 0xff;
    ret += write(handle, (unsigned long) &wrk << 1, 1);
    wrk = (value >> 8) & 0xff;
    ret += write(handle, (unsigned long) &wrk << 1, 1);
    
 	if(offset != CURRENT_POS) {
    	lseek(handle, curpos, SEEK_SET);
 	}
    
	return(ret);
}
int addField(int handle, unsigned int field_id, char *field_value) {
    int i;
	int ret = 0;
    long field_length = strlen(field_value);
    ret = writeLE16(handle, field_id, CURRENT_POS);
    ret += writeLE32(handle, field_length, CURRENT_POS);
    for(i=0; i<field_length; i++) {
       write(handle, (unsigned long)&field_value[i] << 1, 1);
       ret += 1;
   }
//    ret += write(handle, (unsigned long) &field_value << 1, field_length + 1);
    
    return(ret);
}
