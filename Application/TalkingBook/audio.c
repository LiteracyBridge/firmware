// Copyright 2009 Literacy Bridge
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

static int getFileHandle (CtnrFile *);
static void playLongInt(CtnrFile *, unsigned long);
static int recordAudio(char *, char *);
APP_IRAM static char lastFilenameRecorded[FILE_LENGTH];

void playDing(void) {
	Snd_SACM_PlayMemory(C_CODEC_AUDIO1800,RES_DING_A18_SA);	
}

void playBip(void) {
	Snd_SACM_PlayMemory(C_CODEC_AUDIO1800,RES_BIP_A18_SA);	
}

void stop(void) {
	context.isStopped = TRUE;
	Snd_Stop();
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
	ret = tbOpen((LPSTR)sTemp,O_RDWR);			
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
			SACM_Resume();  // todo: is it better to resume and then seek?
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
			if (lTimeNew) {
				SACM_Pause();
				SACM_A1800FAT_SeekTime(lTimeNew,FORWARD_SKIP);
			}
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
			if (!wait && (keystroke = keyCheck(1))) {
				if (keystroke == KEY_PLAY && !context.isPaused) {
					SACM_Pause();
					context.isPaused = TRUE;
				} else if (keystroke == KEY_PLAY && context.isPaused) {
					SACM_Resume();
					context.isPaused = FALSE;
				} else
					context.keystroke = keystroke;
			}
		} 
		context.file = lastFilePlayed;
		playLongInt(context.file,lastPlayedPoint);
		if (wasPaused) {
			SACM_Pause();
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
			if (!wait && (keystroke = keyCheck(1))) {
				if (keystroke == KEY_PLAY && !context.isPaused) {
					SACM_Pause();
					context.isPaused = TRUE;
				} else if (keystroke == KEY_PLAY && context.isPaused) {
					SACM_Resume();
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
	}
}

void insertSoundFile(int idxFile) {
	insertSound(&pkgSystem.files[idxFile],NULL,FALSE);
}
	
static int recordAudio(char *pkgName, char *cursor) {
	int handle, ret = -1;
	char temp[PATH_LENGTH];
	char filepath[PATH_LENGTH];
	long start, end, prev;
	CtnrFile *file;
	int key;
	
	strcpy(filepath,USER_PATH);
	strcat(filepath,pkgName);
	strcat(filepath,AUDIO_FILE_EXT);
		
	file = getListFile(cursor);
	insertSound(file,NULL,TRUE);
	start = getRTCinSeconds();
	strcpy(temp,"\x0d\x0a");
	longToDecimalString(start,temp+2,5);
	strcat(temp,(const char *)": RECORD ");
	LBstrncat(temp,pkgName,60);
	LBstrncat(temp," -> ",60);
	LBstrncat(temp,cursor,60);	
	logString(temp,BUFFER);
	insertSound(&pkgSystem.files[SPEAK_SOUND_FILE_IDX],NULL,TRUE);
	stop();
	setLED(LED_RED,TRUE);
	start = getRTCinSeconds();
	end = start;
	prev = end;
	handle = tbOpen((LPSTR)filepath,O_CREAT|O_RDWR);
	if (handle != -1) {
		Snd_SACM_RecFAT(handle, C_CODEC_AUDIO1800, BIT_RATE);
		do {
			end = getRTCinSeconds();	
			if (0==(end%2) && (prev != end)) { // blink LED every three seconds
				prev = end;
				setLED(LED_RED,FALSE);
				wait (100);
				setLED(LED_RED,TRUE);
			}
			key = keyCheck(0);
			if (key == KEY_PLAY) { // pause  TODO: this key press to pause shouldn't be hard coded
				SACM_Pause();
				setLED(LED_RED,FALSE);
				do
					key = keyCheck(0);
				while (key != KEY_PLAY && key != KEY_STAR);					
				setLED(LED_RED,TRUE);
				SACM_Resume();
			}
		} while (key != KEY_STAR); // TODO: this key press to stop shouldn't be hard coded
		while ((end - start) < 3) { // must be at least 2.0 second recording
			end = getRTCinSeconds();			
		}
		SACM_Stop();		//Snd_Stop(); // no need to call stop() and flush the log
		//lseek(handle, 6, SEEK_SET );			//Seek to the start of the file input
		//write(handle,(LPSTR)header<<1,6);
		close(handle);
//		*P_WatchDog_Ctrl &= ~0x8000; // clear bit 15 to disable
		setLED(LED_RED,FALSE);
		insertSound(&pkgSystem.files[BEEP_SOUND_FILE_IDX],NULL,TRUE); 
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
	
	if (context.packageStartTime) // in case beginning recording while playing another pkg
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
	
	timeDiff = timeNow - context.packageStartTime;
	context.packageStartTime = 0;
	strcpy (log,"TIME PLAYED: ");
	longToDecimalString(timeDiff,log+strlen(log),4);
	strcat(log," sec at VOL=");
	longToDecimalString((long)getVolume(),log+strlen(log),2);
	strcat(log,"\x0d\x0a");
	logString(log,BUFFER);
}

void markStartPlay(long timeNow, const char * name) {
	const int LOG_LENGTH = PATH_LENGTH + 20;
	char log[LOG_LENGTH];
	
	context.packageStartTime = timeNow;
	strcpy(log,"\x0d\x0a");
	longToDecimalString(timeNow,log+2,5);
	strcat((char *)log,(const char *)": PLAY ");
	if (LBstrncat((char *)log,name,LOG_LENGTH) == LOG_LENGTH-1)
		log[LOG_LENGTH-2] = '~';
	logString(log,BUFFER);	
}
