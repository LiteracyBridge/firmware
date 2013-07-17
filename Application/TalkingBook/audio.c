// Copyright 2009-2011 Literacy Bridge
// Contact: info@literacybridge.org
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "Include/talkingbook.h"
#include "Include/files.h"
#include "Include/pkg_ops.h"
#include "Include/device.h"
#include "Include/containers.h"
#include "Include/files.h"
#include "Include/metadata.h"
#include "Include/filestats.h"
#include "Include/startup.h"
#include "Include/audio.h"

APP_IRAM unsigned int  stat_pkg_type;
APP_IRAM int statINIT = 0;
APP_IRAM unsigned long SACM_A1800_Bytes;
APP_IRAM unsigned long SACM_A1800_Msec;		 
APP_IRAM unsigned int msgNotPlayedSec;
APP_IRAM int pauseStarted;
APP_IRAM char msgName[FILE_LENGTH];
APP_IRAM VolumeProfile volumeProfile;
APP_IRAM long lastVolumeChangeRTC;
APP_IRAM static int volume, speed;
APP_IRAM static int oldVolume;
APP_IRAM int volumeMaxThisCycle;

static char STAT_FN[FILE_LENGTH];

extern unsigned long RES_DING_A18_SA;
extern unsigned long RES_BIP_A18_SA;
extern unsigned long User_GetCurDecodeLength(void);
extern int SACMFileHandle;
extern void User_SetDecodeLength(unsigned long);

static int getFileHandle (CtnrFile *);
static void playLongInt(CtnrFile *, unsigned long);
static int recordAudio(char *, char *, BOOL);
static void setVolume(int newVol);
static void initVolumeProfile(void);
static void	getVolumeProfileFilename(char *);	

extern APP_IRAM unsigned int vCur_1;

#include "Include/sys_counters.h"
extern APP_IRAM SystemCounts systemCounts;
// end meta data additions

void playDing(void) {
	Snd_SACM_PlayMemory(C_CODEC_AUDIO1800,RES_DING_A18_SA);	
	while (SACM_Status());
}

void playBip(void) {
	unsigned long lastPlayedPoint;
	int isPlayerStopped = !SACM_Status();

	if (!isPlayerStopped) {
		lastPlayedPoint = Snd_A1800_GetCurrentTime();
		if (lastPlayedPoint < INSERT_SOUND_REWIND_MS)
			lastPlayedPoint = 0;
		else
			lastPlayedPoint -= INSERT_SOUND_REWIND_MS;
	}
	Snd_SACM_PlayMemory(C_CODEC_AUDIO1800,RES_BIP_A18_SA);	
	while (SACM_Status());
	if (!isPlayerStopped) {
		playLongInt(context.file,lastPlayedPoint);
		if (context.isPaused) {
			pause();
		}
	} else {
		turnAmpOff();		
	}
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
	unsigned long words = frames * SACM_A1800_Mode / 800;
	return words;
}

unsigned long framesFromBytes(unsigned long bytes) {
	unsigned long frames = bytes / (SACM_A1800_Mode / 400);	
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

unsigned long getCurrentMsec(void) {
	unsigned long frame = getCurrentFrame();
	unsigned long msec = msecFromFrames(frame);
	return msec;
}

int gotoFrame(unsigned long frameDest) {
	unsigned long wordsPerFrame, bytesPerFrame;
	unsigned long byteCurrent, byteDiff, frameCurrent, frameDiff;
	unsigned long decodeLength; 

	// The function was rewritten, but it still seems to have a bug that shows up
	// unless the .a18 file was just opened.  It may have to do with going backwards.
	// The previous version of this function (r874) seemed to work better with backwards when file already open
	// but this one works fine when re-opening file (as done now in loopClip()).
	pause();
	wordsPerFrame = SACM_A1800_Mode / 800;
	bytesPerFrame = wordsPerFrame<<1;
	byteCurrent = lseek(SACMFileHandle,0,SEEK_CUR);
	frameCurrent = (byteCurrent / bytesPerFrame);
	decodeLength = User_GetCurDecodeLength();
	if (frameDest >= frameCurrent) {
		frameDiff = frameDest - frameCurrent;
		byteDiff = frameDiff * bytesPerFrame;	
		byteCurrent = lseek(SACMFileHandle,byteDiff,SEEK_CUR);
		decodeLength += (byteDiff>>1);
	} else {
		frameDiff = frameCurrent - frameDest;
		byteDiff = frameDiff * bytesPerFrame;	
		byteCurrent = lseek(SACMFileHandle,-byteDiff,SEEK_CUR);
		decodeLength -= (byteDiff>>1);
	}
	User_SetDecodeLength(decodeLength);
	resume();	
	return 0;
}

/*unsigned long setFileHeader(char *filePath, unsigned long frames) {
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
}*/

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
	pauseStarted = getRTCinSeconds();
}	

void resume(void) {
	turnAmpOn();
	SACM_Resume();
	if (pauseStarted != -1) {
		msgNotPlayedSec += getRTCinSeconds() - pauseStarted;
		pauseStarted = -1;
	}
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

extern
void updatePausedTime(void) {
	if (pauseStarted != -1) {
		msgNotPlayedSec += getRTCinSeconds() - pauseStarted;
		pauseStarted = -1;
	}
}

static int getFileHandle (CtnrFile *newFile) {
	int handle, ret = 0; 
	char sTemp[PATH_LENGTH];
	CtnrPackage *pkg;
		
	pkg = getPackageFromFile(newFile); // get package that applied to file, rather than context package
	// This is necessary for user packages inserting system sounds.

	switch (pkg->pkg_type) {
		case PKG_SYS:
		case SYS_MSG:
			strcpy(sTemp,LANGUAGES_PATH);
			if (pkg->transList.mode == '1')
				strcat(sTemp,TRANSLATE_TEMP_DIR);
			else
				strcat(sTemp,pkg->strHeapStack + pkg->idxName);
				
			strcat(sTemp,"/");
			if (newFile->idxFirstBlockInFile == -1) 	// check for list
				strcat(sTemp,TOPICS_SUBDIR);
			else
				strcat(sTemp,UI_SUBDIR);			
			break;		
		case PKG_APP:
			strcpy(sTemp, USER_PATH);
			strcat(sTemp,pkg->strHeapStack + pkg->idxName);
			strcat(sTemp,"/");
			//USERS CAN'T USE LISTS YET - ONLY THE SYSTEM PACKAGE
			//if (newFile->idxFirstBlockInFile == -1) 	// check for list
			//	strcat(sTemp,LISTS_SUBDIR);
			break;
		case PKG_MSG:	
			strcpy(sTemp, USER_PATH);
			break;
	}
	strcat(sTemp,pkg->strHeapStack + newFile->idxFilename);
	strcat(sTemp,AUDIO_FILE_EXT);

	handle = tbOpen((LPSTR)sTemp,O_RDONLY);
	if (handle == -1 && pkg->pkg_type == PKG_SYS)
		logException(35,sTemp,RESET);
	logString(sTemp,BUFFER,LOG_DETAIL);
	if (handle == -1) {
		strcpy(sTemp,"last file not found");
		logString(sTemp,LOG_NORMAL,LOG_DETAIL);
	}
	SACM_A1800_Bytes = 0;	
	if ((handle >= 0)) {  /* && (pkg->pkg_type > PKG_SYS)*/
		ret = read(handle, (unsigned long)&SACM_A1800_Bytes << 1, 4);
		if (ret == 4) {
			ret = read(handle, (unsigned long)&SACM_A1800_Mode << 1, 2);
			if (ret == 2) {
				SACM_A1800_Msec = msecFromFrames(framesFromBytes(SACM_A1800_Bytes));
				if (pkg->pkg_type == PKG_MSG || pkg->pkg_type == PKG_APP) {	
					context.msgLengthMsec = SACM_A1800_Msec;
					logNumber(SACM_A1800_Msec,8,BUFFER,LOG_DETAIL);
				}
			}
			lseek(handle, 0, SEEK_SET );			//Seek to the start of the file input when get over
		
			recordStats(sTemp, (long)handle, STAT_OPEN, pkg->pkg_type);
		}	
		if(SACM_A1800_Bytes <= MIN_AUDIO_FILE_SIZE) {
			char msg[100];
			close(handle);
			handle = -2;
			strcpy(msg,"Audio file too small ");
			strcat(msg, sTemp);
			logString(msg, BUFFER, LOG_ALWAYS);
		}
	}	
//	logString(sTemp,ASAP);
	return handle;
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

	// TODO: The code below will not get used when scanning forward in the same track because
	//       the playBip() is called to represent the scan move, and that causes SACM_Status()==0 as the bip ends.
	//       The other problem is that getCurrentMsec appears to also be affected by playBip() even though it is in NOR flash.
	if (context.lastFile && (context.lastFile->idxFilename == file->idxFilename && SACM_Status())) {
		lTimeCurrent = getCurrentMsec();
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
		
//		if (context.package->pkg_type > PKG_SYS) {
//			recordStats(NULL, (long)iFileHandle, STAT_CLOSE, (long)iFileHandle);
//		}
		
		stop();
		
		close(iFileHandle);
		
		if (file->idxFilename != PKG_HEAP_SIZE)  // don't remember "stack-stored temp" pointers
			context.lastFile = file;  
		else
			context.lastFile = NULL; // to force to assume new file when going back to non-list file
		iFileHandle = getFileHandle(file);
		if (iFileHandle >= 0) {  //allows mistakes or dummy files to pass without problem
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
		msgNotPlayedSec += ((INSERT_SOUND_REWIND_MS + 500)/1000);
		play(context.file,block?block->startTime:0);
		context.isPaused = FALSE;
		while (SACM_Status() && !keystroke) {
			checkVoltage();
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
				} else {
					context.keystroke = keystroke;
					stop();
				}
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
			checkVoltage();
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
				} else {
					context.keystroke = keystroke;
					stop();
				}
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
	
static int recordAudio(char *pkgName, char *cursor, BOOL relatedToLastPlayed) {
	unsigned long getAvailRand();
	int handle, ret = -1;
	char temp[PATH_LENGTH];
	char filepath[PATH_LENGTH];
    char unique_id[PATH_LENGTH], digits[16];
	long start, end, prev;
	CtnrFile *file;
	int key,i;
	int low_voltage;
	unsigned long wrk1;
	char *cp, *cp1, category[9];
	long metadata_start;
	long metadata_numfields;
	unsigned long rand1;
	long previousBitRate;
	
	previousBitRate = BIT_RATE; // set to return BIT_RATE to orig value at end of fct, in case BIT_RATE is changed below
	rand1 = getAvailRand();		// pick random value to identify this recording
    unsignedlongToHexString((long)rand1,digits);
	if (strcmp(cursor,TRANSLATE_TEMP_DIR) == 0) {
		strcpy(filepath,LANGUAGES_PATH);
		strcat(filepath,TRANSLATE_TEMP_DIR);
		strcat(filepath,"/");
		strcat(filepath,pkgName);
		strcat(filepath,AUDIO_FILE_EXT);
		BIT_RATE = MAX_BIT_RATE;
	} else if (*cursor == SYS_MSG_CHAR) {
		strcpy(filepath,LANGUAGES_PATH);
		catLangDir(filepath);	
//		strcat(filepath,pkgName);
		cp1 = filepath + strlen(filepath);	// save this position
		strcat(filepath, getSerialNumber());
		strcat(filepath, "_");
		strcat(filepath, digits);
		strcpy(pkgName, cp1);      // change pkgName to show file name we used
		strcat(filepath,AUDIO_FILE_EXT);
	} else {
		strcpy(filepath,USER_PATH);
//		strcat(filepath,pkgName);
		cp1 = filepath + strlen(filepath);	// save this position
		strcat(filepath, getSerialNumber());
		strcat(filepath, "_");
		strcat(filepath, cursor); //adding category code to filename, primarily to make is easy to find feedback category
		strcat(filepath, "_");
		strcat(filepath, digits);
		strcpy(pkgName, cp1);      // change pkgName to show file name we used
		strcat(filepath,AUDIO_FILE_EXT);
	}
		
	file = getTempFileFromName(cursor,0);
	if (strcmp(cursor,TRANSLATE_TEMP_DIR) != 0)
		insertSound(file,NULL,FALSE);
	if (context.keystroke == KEY_HOME) {
		//let context.keystroke  propogate through
		ret = 1; // signals no audio recorded but not necessary to throw exception
	} else {
		start = getRTCinSeconds();
		strcpy(temp,"RECORD ");
		LBstrncat(temp,pkgName,60);
		LBstrncat(temp," -> ",60);
		LBstrncat(temp,cursor,60);	
		logString(temp,BUFFER,LOG_NORMAL);
		// play record prompt message unless running translation app or if a button was just pressed 
		if (!context.keystroke && strcmp(cursor,TRANSLATE_TEMP_DIR) != 0) {
			insertSound(&pkgSystem.files[SPEAK_SOUND_FILE_IDX],NULL,FALSE);
			if (context.keystroke == KEY_HOME)
				ret = 1; // signals no audio recorded but not necessary to throw exception
			else
				context.keystroke = 0;
		} else
			context.keystroke = 0; // reset context.keystroke so a second action doesn't take place
		stop();
		start = getRTCinSeconds();
		prev = end = start;
		//asm("INT OFF"); // to prevent recordings with bad blocks
	}
	// only open file to record if HOME was not pressed and only proceed if file opens correctly 
	if (ret != 1 && ((handle = tbOpen((LPSTR)filepath,O_CREAT|O_RDWR)) != -1)) {
		setLED(LED_RED,TRUE);
		playBip();
		turnAmpOff();
		Snd_SACM_RecFAT(handle, C_CODEC_AUDIO1800, BIT_RATE);
		low_voltage = 0;
		do {
			checkVoltage();
			if(vCur_1 < V_MIN_SDWRITE_VOLTAGE) {
				low_voltage = 1;
			}
			end = getRTCinSeconds();	
			if (0==(end%2) && (prev != end)) { // blink LED every three seconds
				prev = end;
				setLED(LED_RED,FALSE);
				wait (100);
				setLED(LED_RED,TRUE);
			}
			key = keyCheck(0);
			if (key == KEY_PLAY) { // pause  TODO: this key press to pause shouldn't be hard coded
				pause();
				setLED(LED_RED,FALSE);
				// insertSound(&pkgSystem.files[REC_PAUSED_FILE_IDX],NULL,FALSE); 	---need to play this from memory				
				do
					key = keyCheck(0);
					// TODO: NEED CODE HERE TO SAFELY SAVE FILE WHEN PAUSED FOR EXTENDED PERIOD OF TIME (maybe 60 min?)
					// checkInactivity(key);   --- this would cause recording to be lost in just 30-300 sec inactivity
				while (!key);
				if (key == KEY_PLAY) {
					setLED(LED_RED,TRUE);
					resume();
				}
			}
		} while ((!key || (key == KEY_PLAY)) && (low_voltage == 0)); // TODO: this key press to stop shouldn't be hard coded
//		while ((end - start) < 3) { // must be at least 2.0 second recording
//			end = getRTCinSeconds();			
//		}
		SACM_Stop();		//Snd_Stop(); // no need to call stop() and flush the log
		setLED(LED_RED,FALSE);
		//lseek(handle, 6, SEEK_SET );			//Seek to the start of the file input
		//write(handle,(LPSTR)header<<1,6);
 
// write meta data to end of file
               
        close(handle);	// rhm:  I think its already closed, I can't write to it here
        
		if (low_voltage == 0) {
			turnAmpOn();
			playDing();
			// no need to advance the per-startup recording count if we are immediately shutting down now.
        systemCounts.recordingNumber++;  // bump global recording number
        //saveSystemCounts();
		}
        
       	handle = tbOpen((LPSTR)filepath,O_RDWR);
       	if (handle >= 0) {
	       	metadata_start = lseek(handle, 0L, SEEK_END);  // offset to start of metadata
	        metadata_numfields = 0L; // init num fields
	       
			wrk1 = METADATA_VERSION;
	        writeLE32(handle, wrk1, CURRENT_POS);  //meta data version = 1
	        writeLE32(handle, metadata_numfields, CURRENT_POS); // 4 byte for num fields
	        
	        strcpy(unique_id, getSerialNumber()); // skip serial number prefix
	        strcat(unique_id, "_");    
	       	strcat(unique_id, digits);
	            
	        addField(handle, DC_IDENTIFIER, unique_id, 1);       
	        metadata_numfields += 1;
	        
	        addField(handle, DTB_REVISION, (char *)"0", 1);       
	        metadata_numfields += 1;
	        
	        strcpy(unique_id, getSerialNumber()); // skip serial number prefix
	
	
	        strcat(unique_id, "_");    
	        longToDecimalString(getPowerups(),(char *)temp,4);
	        strcat(unique_id, temp);
	        strcat(unique_id, "_"); 
	        longToDecimalString(systemCounts.recordingNumber,(char *)temp,4);
	        strcat(unique_id, temp);
	        strcat(unique_id, "_"); 
	        strncat(unique_id, digits, 8);
	        addField(handle, DC_TITLE, unique_id, 1);       
	        metadata_numfields += 1;
	
	//        strcpy(unique_id, (char *)TB_SERIAL_NUMBER_ADDR + CONST_TB_SERIAL_PREFIX_LEN); // skip serial number prefix
	//        strcat(unique_id, "_");       
	//		strcat(unique_id, digits);		
	//        addField(handle, DC_AUDIO_ITEM_ID, unique_id, 1);       
	//        metadata_numfields += 1;
	
	        if (pkgSystem.idxLanguageCode != -1) {
				strcpy(unique_id,&pkgSystem.strHeapStack[pkgSystem.idxLanguageCode]);
				addField(handle, DC_LANGUAGE, unique_id, 1);
				metadata_numfields += 1;   
	        }
	         
	        cp = cursor;
	        if(cp != NULL) {
				if(*cp >= '0' && *cp <= '9') {
		        	strcpy(category, cursor);
				} else if(!strncmp(cp, "AGR", 3))
		        	strcpy(category, CAT_AGRICULTURE);
	        	else if(!strncmp(cp, "HEA", 3))
	        		strcpy(category, CAT_HEALTH);
	        	else if(!strncmp(cp, "EDU", 3))
	        		strcpy(category, CAT_EDUCATION);
	        	else if(!strncmp(cp, "STO", 3))
	        		strcpy(category, CAT_STORIES);
	        	else if(!strncmp(cp, "BUS", 3))
	        		strcpy(category, CAT_BUSINESS);
	        	else if(!strncmp(cp, "GOV", 3))
	        		strcpy(category, CAT_GOVERNANCE);
	        	else if(!strncmp(cp, "MUS", 3))
	        		strcpy(category, CAT_MUSIC);
	        	else if(!strncmp(cp, "DIA", 3))
	        		strcpy(category, CAT_DIARY);
	        	else
	        		strcpy(category, CAT_OTHER);
			}
	        
	       	addField(handle, DC_CATEGORY, category, 1);
	    	metadata_numfields += 1;
	        	       
	        if(relatedToLastPlayed) {
	        	strcpy(unique_id, STAT_FN); // DC_IDENTIFIER read at open
	        	if(strlen(unique_id)) {
	        		addField(handle,DC_RELATION,unique_id,1);
	        		metadata_numfields += 1;
	        	}
	        }
	
	        strcpy(unique_id, getSerialNumber()); // skip serial number prefix
	 		addField(handle,DC_PUBLISHER,unique_id,1);       
	        metadata_numfields += 1;
	
	        strcpy(unique_id, getLocation()); // skip serial number prefix
	 		addField(handle,DC_SOURCE,unique_id,1);       
	        metadata_numfields += 1;

	        longToDecimalString(getUpdateYear(),(char *)temp,4);
	        strcat(temp,(char *)"-");
	        i = getCumulativeDays() + getUpdateDate();
	        if (i>30) { // quick hack to not worry about exact number of days in month
	        	longToDecimalString(getUpdateMonth()+1,(char *)temp+strlen(temp),2);
	        	i -= 30;
	        } else 
	        	longToDecimalString(getUpdateMonth(),(char *)temp+strlen(temp),2);
	        strcat(temp,(char *)"-");
	        longToDecimalString(i,(char *)temp+strlen(temp),2);
	 		addField(handle,LB_DATE_RECORDED,temp,1);       
	        metadata_numfields += 1;

			strcpy(temp,getPackageName());
			strcat(temp,(char *)":");
			longToDecimalString(getPowerups(),(char *)(temp+strlen(temp)),4);
			addField(handle,LB_TIMING,temp,1);
	        metadata_numfields += 1;

			strcpy(temp,(char *)"HH Rotation 0");
			temp[strlen(temp)-1] += getRotation();
			strcat(temp,(char *)":Day ");
			i = getCumulativeDays() - (((struct NORrotation *)getLatestRotationStruct())->hoursAfterLastUpdate / 24) + 1; 
			longToDecimalString(i,(char *)(temp+strlen(temp)),2);
			addField(handle,LB_PRIMARY_SPEAKER,temp,1);
	        metadata_numfields += 1;

	        
	        // add other fields here
	        
	        writeLE32(handle, metadata_numfields, metadata_start + 4); // write correct num meta data fields
	        
			close(handle);
       	}
		// done with meta data
		//asm("INT FIQ, IRQ"); -- see INT OFF above used once to prevent corrupted recordings (now possibly handled by SD_Initial() after USB Device mode
        
//		*P_WatchDog_Ctrl &= ~0x8000; // clear bit 15 to disable
	
		if (strcmp(cursor,TRANSLATE_TEMP_DIR) != 0) {
			insertSound(&pkgSystem.files[POST_REC_FILE_IDX],NULL,FALSE);
			if (!context.keystroke)
				insertSound(file,NULL,FALSE);  // replay subject
		}
		// Leaving this out now, because I believe it gets created when it is first played if non-existent.  
		// The delay of having it in recordAudio is too long.
		// createStatsFile(rand1); 
								// Assumes no name collision from using a 32-bit semi-random number
								// Checking all past names was causing a long delay, but maybe (TODO:) we can
								// add in a check if the open() fails and then generate a new number in that unlikely case

		strcpy(temp,"TIME RECORDED (secs): ");
		longToDecimalString((long)end-start,temp+strlen(temp),4);
		strcat(temp,"\x0d\x0a");
		logString(temp,BUFFER,LOG_NORMAL);
		//logString(temp,ASAP);

		ret = 0;  // used to set this based on fileExists() check, but too slow
	} else if (ret == -1) {
		logException(16, filepath,RESET);  //can't open file for new recording
	}
	BIT_RATE = previousBitRate;
	return ret;
}	

int createRecording(char *pkgName, int fromHeadphone, char *listName, BOOL relatedToLastPlayed) {
	int ret, SPINS; //from page 102 of GPL Progammers Manual v1.0/Dec20,2006 
	           //headphone amp audio driver input source select
	ListItem *list; 
	
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
	
//	device-58		
	if (!relatedToLastPlayed) {
		list = &pkgSystem.lists[context.package->idxMasterList];
		if(list->isLocked) {
			logString((char *)"Attempted to record into locked category",BUFFER,LOG_NORMAL);
			return(ERR_CATEGORY_LOCKED);
		}
	}

	ret = recordAudio(pkgName,listName,relatedToLastPlayed);
	if (SPINS)
		*P_HPAMP_Ctrl &= 0xFFF3; // zero bits 2 and 3, returning SPINS to 0
	
	if((ret == 0) && strcmp(listName, TRANSLATE_TEMP_DIR) != 0)
		packageRecording(pkgName,listName); // packageRecording turns it into single byte characters

	return ret;
}

void markEndPlay(long timeNow) {
	long timeDiff;
	char log[80];

	updateVolumeProfile(getVolume(),timeNow);
	if (context.packageStartTime) {
		saveVolumeProfile();
		updatePausedTime();
		timeDiff = timeNow - context.packageStartTime - msgNotPlayedSec;
		if (context.package->pkg_type > PKG_SYS) {
			recordStats(NULL, 0xffffffff, STAT_TIMEPLAYED, timeDiff);
		}
		context.packageStartTime = 0;
		if (timeDiff > 0 /*RHM MINIMUM_PLAY_SEC_TO_LOG*/) {
			strcpy (log,"PLAYED ");
			strcat (log,msgName);
			strcat (log," ");
			longToDecimalString(timeDiff,log+strlen(log),4);
			strcat(log,"/");
			longToDecimalString(context.msgLengthMsec/1000,log+strlen(log),4);
			strcat(log,"sec @VOL=");
			longToDecimalString((long)getVolume(),log+strlen(log),2);
			strcat(log," @Volt=");
			longToDecimalString((long)vCur_1,log+strlen(log),3);
			if (context.msgAtEnd)
				strcat(log, "-Ended");
			strcat(log,"\x0d\x0a");
			logString(log,ASAP,LOG_NORMAL);
			logVoltageProfile();	
		}
	}
}

void markStartPlay(long timeNow, const char * name) {
	const int LOG_LENGTH = PATH_LENGTH + 20;
	char log[LOG_LENGTH];
	
	updateVolumeProfile(0,timeNow); // assume no audio playing since last start/stop
	
	if (context.queuedPackageType != PKG_SYS) {
		context.packageStartTime = timeNow;
		context.msgAtEnd = FALSE;
		context.msgLengthMsec = 0;
	//	strcpy(log,"\x0d\x0a");
	//	longToDecimalString(timeNow,log+2,8);
		strcpy((char *)log,(const char *)"PLAY ");
		if (LBstrncat((char *)log,name,LOG_LENGTH) == LOG_LENGTH-1)
			log[LOG_LENGTH-2] = '~';
		strcat(log," @VOL=");
		longToDecimalString((long)getVolume(),log+strlen(log),2);
		strcat(log," @Volt=");
		longToDecimalString((long)vCur_1,log+strlen(log),3);
		logString(log,ASAP,LOG_NORMAL);
		strcpy(msgName,name);
		msgNotPlayedSec = 0;	
	}
}

int writeLE32(int handle, long value, long offset) {
	int ret = 0;
	unsigned long wrkl;
    long curpos = lseek(handle, 0, SEEK_CUR);
    if(offset != CURRENT_POS) {
        wrkl = lseek(handle, offset, SEEK_SET);
    }
    
    ret += write(handle, (unsigned long) &value << 1, 4);
   
 	if(offset != CURRENT_POS) {
    	lseek(handle, curpos, SEEK_SET);
 	}
    
    return(ret);
}
int writeLE16(int handle, unsigned int value, long offset) {
	int ret = 0;
	unsigned long wrkl;
	long curpos = lseek(handle, 0, SEEK_CUR);
	if(offset != CURRENT_POS) {
		wrkl = lseek(handle, offset, SEEK_SET);
	}
    ret += write(handle, (unsigned long) &value << 1, 2);
    
 	if(offset != CURRENT_POS) {
    	lseek(handle, curpos, SEEK_SET);
 	}
    
	return(ret);
}
int addField(int handle, unsigned int field_id, char *field_value, int numfieldvalues) {
    int i;
	int ret = 0;
    long field_length = strlen(field_value);
    ret = writeLE16(handle, field_id, CURRENT_POS);
    field_length += 3;
    ret += writeLE32(handle, field_length, CURRENT_POS);
    field_length -= 3;
    ret += write(handle, (unsigned long)&numfieldvalues << 1, 1);  //numfields the 01 above
    ret = writeLE16(handle, field_length, CURRENT_POS);
    for(i=0; i<field_length; i++) {
       write(handle, (unsigned long)&field_value[i] << 1, 1);
       ret += 1;
   }
//    ret += write(handle, (unsigned long) &field_value << 1, field_length + 1);
    
    return(ret);
}

void setStatsHeader(struct ondisk_filestats *stats, char *msgId) {
	stats->headerCode = 0x12358D15;
	LBstrncpy(stats->msgId,msgId,MAX_MESSAGE_ID_LENGTH); 
}

void recordStats(char *filename, unsigned long handle, unsigned int why, unsigned long misc)
{
	int metaRead(int fd, unsigned int field_id, unsigned int *buf);
//	char msg[128];
	char statpath[PATH_LENGTH], *cp;
	int stathandle, ret, ret1;
	unsigned long wrk, msgTime;
	struct ondisk_filestats tmp_file_stats = {0};
	
	ret = ret1 = 0;
		
	switch(why) {
	case STAT_OPEN:
		if(misc > PKG_SYS) {
			statINIT = 1;
			stat_pkg_type = misc;
			
			statpath[0] = 0;
			
//			strcpy(msg, "STAT_OPEN call metaRead  ");
//			strcat(msg, filename);
//			logString(msg, ASAP);
			
        	ret1 = metaRead(handle, DC_IDENTIFIER, (unsigned int*)&statpath);
        	
        	if(ret1 > 0) {
 				strcpy(STAT_FN, statpath);
        	} else {
				strcpy(STAT_FN, strrchr(filename, '/') + 1);
				STAT_FN[strlen(STAT_FN) - 4] = 0; //chop off ".a18"			
        	}
        	
//   			strcpy(msg, "STAT_OPEN set STAT_FN  ");
//			strcat(msg, STAT_FN);
//			logString(msg, ASAP);

		} else {
			statINIT = 0;
		}
		setStatsHeader(&tmp_file_stats,STAT_FN);
		break;
	case STAT_CLOSE:
		msgTime = Snd_A1800_GetCurrentTime();  // in ms
		if(statINIT > 0) {   
			if(stat_pkg_type > PKG_SYS) {	
				char *category = getCurrentList(&pkgSystem.lists[0]);
				if (strcmp(category,(char *)FEEDBACK_CATEGORY) && strcmp(category,(char *)TB_CATEGORY)) {
					// do not keep flash stats on user feedback playbacks or TB category playbacks
					int code = -1;
					if (msgTime > (context.msgLengthMsec / 100 * 92)) // consider 92% as completed
						code = 4;
					else if (msgTime > (context.msgLengthMsec / 100 * 75))
						code = 3;
					else if (msgTime > (context.msgLengthMsec>>1))
						code = 2;
					else if (msgTime > (context.msgLengthMsec>>2))
						code = 1;
					else if (msgTime >= 10000)
						code = 0;
					if (code > -1)
						writeMsgEventToFlash ((char *)STAT_FN, code,(unsigned int)(msgTime/1000));
				}
				wrk = lseek(SACMFileHandle, 0L, SEEK_CUR);
				strcpy(statpath, STAT_DIR);
				strcat(statpath, STAT_FN); 
				
//				strcpy(msg, "STAT_CLOSE ");
//				strcat(msg, statpath);
//				logString(msg, ASAP);
						
				stathandle = tbOpen((LPSTR)statpath, O_CREAT|O_RDWR);
				if(stathandle >= 0) {
					ret = read(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
					setStatsHeader(&tmp_file_stats,STAT_FN);
					lseek(stathandle, 0L, SEEK_SET);
					if(wrk >= SACM_A1800_Bytes) {
						tmp_file_stats.stat_num_opens += 1; 
						tmp_file_stats.stat_num_completions += 1;
					} else if (msgTime > 10000) {
						tmp_file_stats.stat_num_opens += 1;
					}
					ret = write(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
					close(stathandle);
				}
			}
		}
		statINIT = 0;
		break;
		
	case STAT_COPIED:
	// how to eliminate PKG_SYS * PKG_NONE files ??
	
		strcpy(STAT_FN, strrchr(filename, '/') + 1);
		cp = strrchr(STAT_FN, '.');
		if(cp == NULL)
			break;
		if(strcmp("a18", cp+1))
			break;	// no a18 suffix
		
		STAT_FN[strlen(STAT_FN) - 4] = 0; //chop off ".a18"
		
		strcpy(statpath, STAT_DIR);
		strcat(statpath, STAT_FN); 
		
		stathandle = tbOpen((LPSTR)statpath, O_CREAT|O_RDWR);
		if(stathandle >= 0) {
			ret = read(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
			lseek(stathandle, 0L, SEEK_SET);
			tmp_file_stats.stat_num_copies += 1; 
			setStatsHeader(&tmp_file_stats,STAT_FN);
			ret = write(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
			close(stathandle);
		}
		break;
		
	case STAT_TIMEPLAYED:
/*		if(statINIT > 0) {

			sprintf(msg, "StatLog TIMEPLAYED: time=%ld: ", misc);
			strcat(msg, STAT_FN);
			strcat(msg,"\x0d\x0a");
			logString(msg, ASAP);

		}
*/
		break;

	case STAT_SURVEY1:
		strcpy(statpath, STAT_DIR);
	//		strcat(statpath, getDeviceSN(0));
	//		strcat(statpath, "~");
		strcat(statpath, STAT_FN); 
		stathandle = tbOpen((LPSTR)statpath, O_CREAT|O_RDWR);
		if(stathandle >= 0) {
			ret = read(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
			lseek(stathandle, 0L, SEEK_SET);
			setStatsHeader(&tmp_file_stats,STAT_FN);
			tmp_file_stats.stat_num_survey1 += 1; 
			ret = write(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
			close(stathandle);
		}
		break;
	
	case STAT_APPLY:
		strcpy(statpath, STAT_DIR);
	//		strcat(statpath, getDeviceSN(0));
	//		strcat(statpath, "~");
		strcat(statpath, STAT_FN); 
		
		if(stat_pkg_type > PKG_SYS) {	
			char *category = getCurrentList(&pkgSystem.lists[0]);
			if (strcmp(category,(char *)FEEDBACK_CATEGORY) && strcmp(category,(char *)TB_CATEGORY)) {
				// do not keep flash stats on user feedback playbacks or TB category playbacks
				writeMsgEventToFlash ((char *)STAT_FN, 5,0);
			}
		}

		stathandle = tbOpen((LPSTR)statpath, O_CREAT|O_RDWR);
		if(stathandle >= 0) {
			ret = read(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
			lseek(stathandle, 0L, SEEK_SET);
			tmp_file_stats.stat_num_apply += 1; 
			setStatsHeader(&tmp_file_stats,STAT_FN);
			ret = write(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
			close(stathandle);
		}
		break;
		
	case STAT_USELESS:
		strcpy(statpath, STAT_DIR);
	//		strcat(statpath, getDeviceSN(0));
	//		strcat(statpath, "~");
		strcat(statpath, STAT_FN); 
		
		if(stat_pkg_type > PKG_SYS) {	
			char *category = getCurrentList(&pkgSystem.lists[0]);
			if (strcmp(category,(char *)FEEDBACK_CATEGORY) && strcmp(category,(char *)TB_CATEGORY)) {
				// do not keep flash stats on user feedback playbacks or TB category playbacks
				writeMsgEventToFlash ((char *)STAT_FN, 6,0);
			}
		}
		stathandle = tbOpen((LPSTR)statpath, O_CREAT|O_RDWR);
		if(stathandle >= 0) {
			ret = read(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
			lseek(stathandle, 0L, SEEK_SET);
			tmp_file_stats.stat_num_useless += 1; 
			setStatsHeader(&tmp_file_stats,STAT_FN);
			ret = write(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
			close(stathandle);
		}
		break;
	}
}

// for now a uid is available if no stat file is present for that id
unsigned long getAvailRand() {
	unsigned long uid;

	uid = rand();

	return(uid);
}
/* NOT CURRENTLY USED
void createStatsFile(unsigned long uid) {
	char statpath[PATH_LENGTH], digits[16];
	struct ondisk_filestats tmp_file_stats = {0};
	int stathandle = -1, ret;
	
	strcpy(statpath, STAT_DIR);	
    strcat(statpath, (char *)TB_SERIAL_NUMBER_ADDR + CONST_TB_SERIAL_PREFIX_LEN); // skip serial number prefix
    strcat(statpath, "_"); 
	unsignedlongToHexString((unsigned long)uid, digits);
	strcat(statpath, digits);
	stathandle = open((LPSTR)statpath, O_CREAT|O_RDWR);
	ret = write(stathandle, (unsigned long) &(tmp_file_stats) << 1, STATSIZE);
	close(stathandle);
}
*/

int readLE32(int handle, long value, long offset) {
	int ret = 0;
	unsigned long wrkl;
    long curpos = lseek(handle, 0, SEEK_CUR);
    if(offset != CURRENT_POS) {
        wrkl = lseek(handle, offset, SEEK_SET);
    }
    
    ret += read(handle, (unsigned long) value << 1, 4);
   
 	if(offset != CURRENT_POS) {
    	lseek(handle, curpos, SEEK_SET);
 	}
    
    return(ret);
}
int readLE16(int handle, long value, long offset) {
	int ret = 0;
	unsigned long wrkl;
	long curpos = lseek(handle, 0, SEEK_CUR);
	if(offset != CURRENT_POS) {
		wrkl = lseek(handle, offset, SEEK_SET);
	}
    ret += read(handle, (unsigned long) value << 1, 2);
    
 	if(offset != CURRENT_POS) {
    	lseek(handle, curpos, SEEK_SET);
 	}
    
	return(ret);
}
int metaRead(int fd, unsigned int field_id, unsigned int *buf) {
	int convertTwoByteToSingleChar(unsigned int *, const unsigned int *, int);
	unsigned long mdversion, numfields, fieldlen, wrk, savpos;
	unsigned int fid, nfv=0, i, j, ret, ret1 = 0;
	unsigned char tmpbuf[128];
//	char msg[128], digits[16];;

	savpos = lseek(fd, 0, SEEK_CUR);  // save current position
	
	wrk = lseek(fd, SACM_A1800_Bytes + 4, SEEK_SET);
	ret = readLE32(fd, (long)&mdversion, CURRENT_POS);
	if((mdversion == 0) || (mdversion > META_CURRENT_VERSION)) {
		goto failed;
	}
	ret = readLE32(fd, (long)&numfields, CURRENT_POS);
//	printf("num fields=%ld\n", numfields);

//	strcpy(msg,"metaRead numfields ");
//	unsignedlongToHexString((long)numfields,digits);
//	strcat(msg, digits);
//	logString(msg, ASAP);
	
	for(i=0; i<(int)numfields; i++) {
		ret = readLE16(fd, (long)&fid, CURRENT_POS);
//		printf("\n  field id=%d\n",fid);
		ret = readLE32(fd, (long)&fieldlen, CURRENT_POS);
		if(field_id == fid) {
			ret = read(fd, (unsigned long)&nfv << 1, 1);
			nfv &= 1; // This will only ever return the first part of the metadata item, which is fine for now
			for(j=0; j<nfv; j++) {
				unsigned int fl;
				ret = readLE16(fd, (long)&fl, CURRENT_POS);
				ret = read(fd, (unsigned long) tmpbuf << 1, fl);
				ret1 = fl;
				goto foundit;
			}
		} else {
			lseek(fd,fieldlen, SEEK_CUR); //skip past this metadata item
		}
	}
failed:
//	strcpy(msg, "metaRead failed");
//	logString(msg, ASAP);
	
	wrk = lseek(fd, savpos, SEEK_SET);
	return(-1);
foundit:
//	strcpy(msg,"metaRead found item ");
//	unsignedlongToHexString((long)ret1,digits);
//	strcat(msg, digits);
//	logString(msg, ASAP);
	
	ret = convertTwoByteToSingleChar(buf,(const unsigned int *)tmpbuf,ret1);
	wrk = lseek(fd, savpos, SEEK_SET);
	return(ret1);
}
int convertTwoByteToSingleChar(unsigned int *buf, const unsigned int *tmpbuf, int count) {
	unsigned int j, wrk, *cp;
	
	cp = (unsigned int*)tmpbuf;
	for(j=0; j<count; ) {
		wrk = *cp++;
		buf[j++] = wrk & 0xff;
		if(j == count)
			break;
		buf[j++] = (wrk & 0xff00) >> 8;
	}
	buf[j] = 0;
//	{
//		char msg[128];
//		strcpy(msg, "convertTwoByteToSingleChar ");
//		strcat (msg, (char *)buf);
//		logString(msg, BUFFER);
//	}
	return(j);
}

int restoreVolume(BOOL normalVolume) {
	int v;	
	if (normalVolume)
		v = adjustVolume(NORMAL_VOLUME,FALSE,FALSE);
	else
		v = adjustVolume(oldVolume,FALSE,FALSE);
	if (v == -1) // adjustVolume returns -1 if already at max volume
		v = getVolume();
	return v;
}

int adjustVolume (int amount, BOOL relative, BOOL rememberOldVolume) {
	int ret;
	int v;
	
	v = getVolume();
	if (rememberOldVolume)
		oldVolume = v;
	
	if (relative)
		v += amount;
	else
		v = amount;
	
	ret = set_voltmaxvolume(FALSE);

	if (v > volumeMaxThisCycle) { 
		v = volumeMaxThisCycle;
		setLED(LED_RED,TRUE);
		playBip();
		setLED(LED_RED,FALSE);
		ret = -1;	
	}
	if (v < MIN_VOLUME)  
		v = MIN_VOLUME;
	setVolume(v);
	if (ret != -1)
		ret = v;
	return ret;
}

int adjustSpeed (int amount, BOOL relative) {
	if (relative)
		speed += amount;
	else
		speed = amount;

	if (speed > MAX_SPEED)  
		speed = MAX_SPEED;
	if (speed < 0)  
		speed = 0;
	SACM_Speed(speed);	
	return speed;
}

int getVolume(void) {
	return volume;
}

int getSpeed(void) {
	return speed;
}


int
set_voltmaxvolume(BOOL forceLower)
{
	const int MAX_ALLOWED_VOLUME = 12;
	const int MAX_ALLOWED_VOLUME_MIN_VOLTAGE = 320;
	const int MAX_MODERATE_VOLUME = 4;
	const int MAX_MODERATE_VOLUME_MIN_VOLTAGE = 240;
	const int MIN_VOLUME_VOLTAGE = 190;
	const int HIGH_VOLUME_VOLT_PER_VOLUME = (MAX_ALLOWED_VOLUME_MIN_VOLTAGE - MAX_MODERATE_VOLUME_MIN_VOLTAGE) / (MAX_ALLOWED_VOLUME - MAX_MODERATE_VOLUME);
	const int LOW_VOLUME_VOLT_PER_VOLUME = (MAX_MODERATE_VOLUME_MIN_VOLTAGE - MIN_VOLUME_VOLTAGE) / (MAX_MODERATE_VOLUME - 1);
	
	int vol, maxVol, ret = 0;
	
	vol = getVolume();
	if (vCur_1 == V_EXTERNAL_VOLTAGE || (vCur_1 >= MAX_ALLOWED_VOLUME_MIN_VOLTAGE))
		return ret;
	if (vCur_1 <= MIN_VOLUME_VOLTAGE)
		maxVol = 1;
	else if (vCur_1 >= MAX_MODERATE_VOLUME_MIN_VOLTAGE)
		maxVol = MAX_MODERATE_VOLUME + (vCur_1 - MAX_MODERATE_VOLUME_MIN_VOLTAGE)/HIGH_VOLUME_VOLT_PER_VOLUME;
	else
		maxVol = 1 + (vCur_1 - MIN_VOLUME_VOLTAGE)/LOW_VOLUME_VOLT_PER_VOLUME;
	
	if (maxVol < volumeMaxThisCycle || forceLower) {
		if (forceLower)
			maxVol = volumeMaxThisCycle - 1;			
		if(maxVol < 1) maxVol = 1;
		if(maxVol < volumeMaxThisCycle) {
			volumeMaxThisCycle = maxVol;
			if (vol > volumeMaxThisCycle) {
				vol = volumeMaxThisCycle;
				setVolume(vol);
				setLED(LED_RED,TRUE);
				playBip();
				setLED(LED_RED,FALSE);
				ret = -1;	
			}
			if (DEBUG_MODE >= LOG_NORMAL) { // logging voltage for all devices in the field (DEBUG_MODE) {
				char log[15] = "v---,MV--,CV--";
				longToDecimalString(vCur_1, log+1, 3);
				log[4] = ',';
				longToDecimalString((long)volumeMaxThisCycle,log+7,2);
				log[9] = ',';
				longToDecimalString((long)vol,log+11,2);
				logString(log,BUFFER,LOG_NORMAL);
			}		
		}
	}
	return ret; // -1 indicates that current volume had to be lowered, due to lower max
}

extern void updateVolumeProfile(int lastVolume, long currentRTCsec) {
	int diffSec;
	
	diffSec = currentRTCsec - lastVolumeChangeRTC;
	if (diffSec < 0) {
		// new day must have occurred
		diffSec = currentRTCsec + (86400-lastVolumeChangeRTC);
	}
	volumeProfile.volumeSeconds[lastVolume] += diffSec;
	getRTC(volumeProfile.lastUpdateRTC);
	lastVolumeChangeRTC = currentRTCsec;
}

static void setVolume(int newVol) {
	long time;
	
	time = getRTCinSeconds();
	if (SACM_Status())
		updateVolumeProfile(volume,time);  // log play at prior volume
	else
		updateVolumeProfile(0,time);  	// log non-play time
	volume = newVol;	
	SACM_Volume(volume);	
}

extern void logVoltageProfile(void) {
	int i;
	char log[200];
	long time, sum = 0;
	
	if (DEBUG_MODE < LOG_DETAIL)
		return; // not worth logging, according to config file
	flushLog(); //  clear buffer
	strcpy(log,(char *)"Volume Profile for: ");
	strcat(log,volumeProfile.periodStartRTC);
	strcat(log,(char *)"\x0d\x0a");
	strcat(log,(char *)" last updated     : ");
	strcat(log,volumeProfile.lastUpdateRTC);	
	logString(log,BUFFER,LOG_NORMAL);
	strcpy(log,(char *)" ");
	for (i=0;i<16;i++) { // this should add 160 characters to log[200]
		time = volumeProfile.volumeSeconds[i];
		sum += time;
		longToDecimalString(i,log+strlen(log),2);
		strcat(log,(char *)":");		
		longToDecimalString(time,log+strlen(log),5);
		strcat(log,(char *)"  ");		
	}	
	logString(log,BUFFER,LOG_NORMAL);

	strcpy(log,(char *)" SUM:");		
	longToDecimalString(sum,log+strlen(log),6);
	logString(log,ASAP,LOG_NORMAL);
}

static void initVolumeProfile(void) {
	int i;
	
	lastVolumeChangeRTC = getRTCinSeconds();
	getRTC(volumeProfile.periodStartRTC);
	strcpy(volumeProfile.lastUpdateRTC,"no update");	
	for (i=0;i<16;i++) 
	volumeProfile.volumeSeconds[i]=0;	
	logString((char *)"Initializing new Volume Profile",BUFFER,LOG_NORMAL);
}

extern void saveVolumeProfile(void) {
	int handle, ret;
	char filename[PATH_LENGTH];
 	
	getVolumeProfileFilename(filename);	
//	ret = unlink((LPSTR)(filename));
	handle = tbOpen((LPSTR)(filename),O_CREAT|O_TRUNC|O_RDWR);
	if (handle != -1) {
		ret = write(handle, (unsigned long)&volumeProfile<<1, sizeof(volumeProfile)<<1);
		close(handle);
	} else
		logString((char *)"failed to write volume profile",BUFFER,LOG_ALWAYS);
}

extern void loadVolumeProfile(void) {
	int handle, ret;
	char filename[PATH_LENGTH];

	getVolumeProfileFilename(filename);	
	handle = tbOpen((LPSTR)(filename),O_RDONLY);
	if (handle != -1) {
		ret = read(handle,(unsigned long)&volumeProfile<<1,sizeof(volumeProfile)<<1);
		close(handle);
	} else {
		initVolumeProfile();
	}
	lastVolumeChangeRTC = getRTCinSeconds();
	logVoltageProfile();
}

static void getVolumeProfileFilename(char *vpFilename) {
	strcpy(vpFilename,STAT_DIR);
	strcat(vpFilename,getDeviceSN());
	strcat(vpFilename,(char *)"_");
	longToDecimalString(CLOCK_PERIOD,vpFilename+strlen(vpFilename),3);
	strcat(vpFilename,(char *)".bin");
	logString(vpFilename,BUFFER,LOG_DETAIL);
	return;
}

