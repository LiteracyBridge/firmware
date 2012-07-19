#define HALT_ON_COLD_START
// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "./system/include/system_head.h"
#include "Include/talkingbook.h"
#include "Include/files.h"
#include "Include/device.h"
#include "Include/containers.h"
#include "Include/parsing.h"
#include "Include/macro.h"
#include "Include/sys_counters.h"
#include "Include/Inbox.h"
#include "Include/audio.h"
#include "Include/SD_reprog.h"
#include "Include/mainLoop.h"
#include "Include/filestats.h"
#include "Include/d2d_copy.h"
#include "Include/startup.h"
#include "Include/mainLoop.h"
#include <ctype.h>

extern int testPCB(void);
extern unsigned int SetSystemClockRate(unsigned int);
extern int SystemIntoUDisk(unsigned int);
extern INT16 SD_Initial(void);

static int setLocation(char *);
static int checkRTCFile(char *);	
static char * addTextToSystemHeap (char *);
static int loadConfigFile (void);
static int restore_config_bin();
static int disaster_config_strings();
static void fixnull_config_strings();

// These capitalized variables are set in the config file.
APP_IRAM int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;
APP_IRAM int ADMIN_COMBO_KEYS;	
APP_IRAM int LED_GREEN, LED_RED, LED_ALL;
APP_IRAM int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
APP_IRAM int NORMAL_VOLUME, MAX_VOLUME, MIN_VOLUME, VOLUME_INCREMENT;
APP_IRAM char *PROFILE_ORDER_FILE, *SYSTEM_PATH, *LANGUAGES_PATH, *UI_SUBDIR, *TOPICS_SUBDIR, *USER_PATH, *LISTS_PATH;
APP_IRAM char *INBOX_PATH, *OUTBOX_PATH,*NEW_PKG_SUBDIR, *SYS_UPDATE_SUBDIR;
APP_IRAM int MAX_PWR_CYCLES_IN_LOG;
APP_IRAM char *SYSTEM_VARIABLE_FILE, *LOG_FILE;
APP_IRAM char *LIST_MASTER;
APP_IRAM char *PKG_NUM_PREFIX, *LIST_NUM_PREFIX, *CUSTOM_PKG_PREFIX;
APP_IRAM  char *AUDIO_FILE_EXT;
APP_IRAM int DEFAULT_TIME_PRECISION;
APP_IRAM int DEFAULT_REWIND;
APP_IRAM int INSERT_SOUND_REWIND_MS;
APP_IRAM int HYPERLINK_SOUND_FILE_IDX, DELETED_FILE_IDX, PRE_COPY_FILE_IDX, POST_COPY_FILE_IDX, SPEAK_SOUND_FILE_IDX, 
    INACTIVITY_SOUND_FILE_IDX, ERROR_SOUND_FILE_IDX, EMPTY_LIST_FILE_IDX, REC_PAUSED_FILE_IDX, POST_REC_FILE_IDX, PLEASE_WAIT_IDX, POST_TRANSLATE_FILE_IDX,NO_TRANSLATION_FILE_IDX,
    NOT_YET_TRANSLATED_FILE_IDX,PLS_RECORD_TRANSLATION_FILE_IDX,NEW_RECORDING_FILE_IDX,ORIG_RECORDING_FILE_IDX,PLS_WAIT_FILE_IDX,OVERWRITE_WARNING_FILE_IDX,
    SUBJECT_NOW_LOCKED_IDX,SUBJECT_NOW_UNLOCKED_IDX,SORRY_LOCKED_SUBJECT_IDX,
    POST_PLAY_FILE_IDX;
APP_IRAM int BLOCK_START_LEADER, BLOCK_END_LEADER;
APP_IRAM long BIT_RATE;
APP_IRAM int GREEN_LED_WHEN_PLAYING;
APP_IRAM int INACTIVITY_SECONDS;
APP_IRAM int MIC_GAIN_NORMAL, MIC_GAIN_HEADPHONE;
APP_IRAM char *USER_CONTROL_TEMPLATE;
APP_IRAM char *MACRO_FILE;
APP_IRAM int VOLTAGE_SAMPLE_FREQ_SEC, USB_CLIENT_POLL_INTERVAL;
APP_IRAM int DEBUG_MODE, LOG_KEYS;
APP_IRAM unsigned int CLOCK_RATE;
APP_IRAM int LONG_LIST_NAMES;
APP_IRAM unsigned int LONG_KEYPRESS_COUNTER;
APP_IRAM unsigned int V_FAST_VOLTAGE_DROP_TIME_SEC, V_VOLTAGE_DROP_CHECK_INTERVAL;

//if most recent 16 voltage readings are below vCur_1 (vThresh_1 == 0xffff),
//   subtract 1 from vCur_1 and zero vThreah_1
APP_IRAM unsigned int vCur_1;
APP_IRAM int vThresh_1;
APP_IRAM unsigned int MEM_TYPE;   // sst or mx

void setDefaults(void) {
	// This function sets variables that are usually set in config file, 
	// but they need defaults in case config file doesn't list them or isn't loaded yet
		
	if(MEM_TYPE == MX_MID) {
		// BELOW ARE THE GOOD KEY CODES FOR SCH 3.1+
		KEY_DOWN   = V1_DEFAULT_KEY_DOWN;
		KEY_STAR   = V1_DEFAULT_KEY_STAR;
		KEY_RIGHT  = V1_DEFAULT_KEY_RIGHT;
		KEY_LEFT   = V1_DEFAULT_KEY_LEFT;
		KEY_UP     = V1_DEFAULT_KEY_UP;
		KEY_MINUS  = V1_DEFAULT_KEY_MINUS;
		KEY_PLUS   = V1_DEFAULT_KEY_PLUS;
	 	KEY_PLAY   = V1_DEFAULT_KEY_PLAY;
		KEY_SELECT = V1_DEFAULT_KEY_SELECT;
		KEY_HOME   = V1_DEFAULT_KEY_HOME;
	} else {
		// FOR OLD HARDWARE (SCH 2.2)
		KEY_DOWN   = V0_DEFAULT_KEY_DOWN;
		KEY_STAR   = V0_DEFAULT_KEY_STAR;
		KEY_RIGHT  = V0_DEFAULT_KEY_RIGHT;
		KEY_LEFT   = V0_DEFAULT_KEY_LEFT;
		KEY_UP     = V0_DEFAULT_KEY_UP;
		KEY_MINUS  = V0_DEFAULT_KEY_MINUS;
		KEY_PLUS   = V0_DEFAULT_KEY_PLUS;
	 	KEY_PLAY   = V0_DEFAULT_KEY_PLAY;
		KEY_SELECT = V0_DEFAULT_KEY_SELECT;
		KEY_HOME   = V0_DEFAULT_KEY_HOME;
	}
	
	LED_GREEN        = DEFAULT_LED_GREEN;
	LED_RED          = DEFAULT_LED_RED;
	MAX_SPEED        = DEFAULT_MAX_SPEED;
	NORMAL_SPEED     = DEFAULT_NORMAL_SPEED;
	MAX_VOLUME       = DEFAULT_MAX_VOLUME;
	MIN_VOLUME		 = DEFAULT_MIN_VOLUME;
	NORMAL_VOLUME    = DEFAULT_NORMAL_VOLUME;
	SPEED_INCREMENT  = DEFAULT_SPEED_INCREMENT;
	VOLUME_INCREMENT = DEFAULT_VOLUME_INCREMENT;
	
	MAX_PWR_CYCLES_IN_LOG  = DEFAULT_MAX_PWR_CYCLES_IN_LOG;
	
	DEFAULT_TIME_PRECISION          = DEFAULT_DEFAULT_TIME_PRECISION;
	DEFAULT_REWIND                  = DEFAULT_DEFAULT_REWIND;
	INSERT_SOUND_REWIND_MS          = DEFAULT_INSERT_SOUND_REWIND_MS;
	SPEAK_SOUND_FILE_IDX            = DEFAULT_SPEAK_SOUND_FILE_IDX;	
	REC_PAUSED_FILE_IDX             = DEFAULT_REC_PAUSED_FILE_IDX;
	POST_REC_FILE_IDX               = DEFAULT_POST_REC_FILE_IDX;
	PLEASE_WAIT_IDX                 = DEFAULT_PLEASE_WAIT_IDX;
	NO_TRANSLATION_FILE_IDX         = DEFAULT_NO_TRANSLATION_FILE_IDX;
	POST_TRANSLATE_FILE_IDX         = DEFAULT_POST_TRANSLATE_FILE_IDX;
	OVERWRITE_WARNING_FILE_IDX      = DEFAULT_OVERWRITE_WARNING_FILE_IDX;				
	SUBJECT_NOW_LOCKED_IDX			= DEFAULT_SUBJECT_NOW_LOCKED_IDX;
	SUBJECT_NOW_UNLOCKED_IDX		= DEFAULT_SUBJECT_NOW_UNLOCKED_IDX;
	SORRY_LOCKED_SUBJECT_IDX		= DEFAULT_SORRY_LOCKED_SUBJECT_IDX;
	NOT_YET_TRANSLATED_FILE_IDX     = DEFAULT_NOT_YET_TRANSLATED_FILE_IDX;
	PLS_RECORD_TRANSLATION_FILE_IDX = DEFAULT_PLS_RECORD_TRANSLATION_FILE_IDX;
	PLS_WAIT_FILE_IDX               = DEFAULT_PLS_WAIT_FILE_IDX;
	NEW_RECORDING_FILE_IDX          = DEFAULT_NEW_RECORDING_FILE_IDX;
	ORIG_RECORDING_FILE_IDX         = DEFAULT_ORIG_RECORDING_FILE_IDX;
	INACTIVITY_SOUND_FILE_IDX       = DEFAULT_INACTIVITY_SOUND_FILE_IDX;
	
	ERROR_SOUND_FILE_IDX          = DEFAULT_ERROR_SOUND_FILE_IDX;	
	EMPTY_LIST_FILE_IDX           = DEFAULT_EMPTY_LIST_FILE_IDX;
	DELETED_FILE_IDX              = DEFAULT_DELETED_FILE_IDX;
	PRE_COPY_FILE_IDX             = DEFAULT_PRE_COPY_FILE_IDX;
	POST_COPY_FILE_IDX            = DEFAULT_POST_COPY_FILE_IDX;
	POST_PLAY_FILE_IDX            = DEFAULT_POST_PLAY_FILE_IDX;				
	HYPERLINK_SOUND_FILE_IDX      = DEFAULT_HYPERLINK_SOUND_FILE_IDX;
	BLOCK_START_LEADER            = DEFAULT_BLOCK_START_LEADER;
	BLOCK_END_LEADER              = DEFAULT_BLOCK_END_LEADER;
	BIT_RATE                      = DEFAULT_BIT_RATE;
	GREEN_LED_WHEN_PLAYING        = DEFAULT_GREEN_LED_WHEN_PLAYING;
	INACTIVITY_SECONDS            = DEFAULT_INACTIVITY_SECONDS;
	MIC_GAIN_NORMAL               = DEFAULT_MIC_GAIN_NORMAL;
	MIC_GAIN_HEADPHONE            = DEFAULT_MIC_GAIN_HEADPHONE; 
	VOLTAGE_SAMPLE_FREQ_SEC       = DEFAULT_VOLTAGE_SAMPLE_FREQ_SEC;
	USB_CLIENT_POLL_INTERVAL      = DEFAULT_USB_CLIENT_POLL_INTERVAL;
	DEBUG_MODE                    = DEFAULT_DEBUG_MODE;
	LOG_KEYS                      = DEFAULT_LOG_KEYS;
	CLOCK_RATE                    = DEFAULT_CLOCK_RATE;
	LONG_LIST_NAMES               = DEFAULT_LONG_LIST_NAMES;
	V_FAST_VOLTAGE_DROP_TIME_SEC  = DEFAULT_V_FAST_VOLTAGE_DROP_TIME_SEC;
	V_VOLTAGE_DROP_CHECK_INTERVAL = DEFAULT_V_VOLTAGE_DROP_CHECK_INTERVAL;
	LONG_KEYPRESS_COUNTER         = KEY_LONG_DOWN_THRESH;
	
	LED_ALL            = LED_GREEN | LED_RED;
	BIT_RATE           = DEFAULT_BIT_RATE;
	INACTIVITY_SECONDS = DEFAULT_INACTIVITY_SECONDS;

	PLEASE_WAIT_IDX = 0; // prevents trying to insert this sound before config & control files are loaded.
	context.package = 0; // prevents trying to insert this sound before config & control files are loaded.
	
	ADMIN_COMBO_KEYS   = KEY_UP | KEY_DOWN;
	LOG_FILE = (char *)DEFAULT_LOG_FILE; // chicken & egg - we haven't read config.txt or config.bin to set LOG_FILE
	SYSTEM_VARIABLE_FILE = (char *)DEFAULT_SYSTEM_VARIABLE_FILE;
}

static int checkRTCFile(char *time) {	
	char *cp;
	int ret;
	struct f_info file_info;
	
	ret =_findfirst((LPSTR)SET_RTC_FILE_PATTERN, &file_info, D_FILE);
	if (ret >=0) {
		strcpy(time,file_info.f_name);
		cp = strrchr(time,'.');
		*cp = 0;
		unlink((LPSTR)file_info.f_name);	
	} 
	return ret;
}

static int setLocation(char *location) {
	char *cp;
	int ret;
	struct f_info file_info;
	char newPath[PATH_LENGTH];
		
	ret =_findfirst((LPSTR)LOCATION_FILE_PATTERN, &file_info, D_FILE);
	if (ret >=0) {
		strcpy(location,file_info.f_name);
		cp = strrchr(location,'.');
		*cp = 0;
		strcpy(newPath,DEFAULT_SYSTEM_PATH);
		strcat(newPath,file_info.f_name);
		rename((LPSTR)file_info.f_name,(LPSTR)newPath);	
	} 
	return ret;
}

// The following three functions were just experiments with vfs.h functions:
//  UpdataDir(), FileRepair(), and _deleteall().
//  The first two fucntions don't seem useful. deleteall() works fine in non-corrupted directories:
/*
static void dumpSystemDir(void) {
	int fd;
	int ret;
	
	ret = 0; //chdir((LPSTR)"a:\\system");
	if (ret >= 0) {
//		playDing();
		fd = open((LPSTR)"a:\\sysdirdump.txt", O_CREAT);
	 	if (fd >=0) {
//	 		playDing();
		 	UpdataDir(fd);
			close(fd);
	 	} else {
//	 		playBip();
	 	}
	} else {
//		playBip();
	}
}

static void repairSystemDir(void) {
	int fd;
	int ret;
	
	fd = open((LPSTR)"a:\\system", O_RDONLY);
	if (fd >=0) {
		playDing();
	 	ret = FileRepair(fd);
		close(fd);
		if (ret == 0)
			playDing();
		else if (ret == -1)
			playBip();
	} else 
		playBip();
}

static void clearDir(char* dir) {
	int hd,ret;
	
	ret = _deleteall((LPSTR)dir);
	if (ret == 0)
		playDing();
	else if (ret == -1) {
		playBip();
		ret = _getfserrcode();
		hd=open((LPSTR)"a:\\err.bin",O_CREAT);
		write(hd,(LPDATA)&ret<<1,4);
		close(hd);
	}
}
*/

void startUp(unsigned int bootType) {
	char buffer[400];
	char strCounts[32];
	char filename[FILE_LENGTH];
	int key, ret, callPushPull = 0,callProcessInbox = 0;
	int configExists = 0, normal_shutdown=1;
	int inspect = 0;

	setLED(LED_ALL,TRUE);  // start lights to indicate user should wait during startup until the device is ready
	systemCounts.month = 1;
	systemCounts.monthday = 1;
	systemCounts.year = FILE_YEAR_MIN;

	// Before any trouble is caused, allow forcing USB mode.		
	key = keyCheck(1);  // long keycheck 
	key &= ~LONG_KEY_STROKE;
	if (key == KEY_STAR || key == KEY_MINUS) {
		SystemIntoUDisk(1);	
		callProcessInbox = 1;
	} else if (key == KEY_PLUS)
		callPushPull = 1;  // call pushContentGetFeedback() at end up this fct.

	if (fileExists((LPSTR)INSPECT_TRIGGER_FILE))
		inspect = 1;  // used to check for .loc file and other changes that don't normally occur

	ret = loadSystemCounts();  // calling this before config means we rely on the default location for system-vars
	systemCounts.powerUpNumber++; 
	if (ret == -1 || inspect)
		setLocation(systemCounts.location);
	saveSystemCounts();	

	strcpy(buffer,"\x0d\x0a" "---------------------------------------------------\x0d\x0a" "Serial#:");		
	strcat(buffer,getDeviceSN(0));
	strcat(buffer,"\x0d\x0a" "Clock:");
	getRTC(buffer+strlen(buffer));
	logStringRTCOptional(buffer, ASAP, LOG_ALWAYS,0);  // calling this before config means we rely on default location
		
	if(bootType == BOOT_TYPE_COLD_RESET) {
		extern unsigned long rtcAlarm[];
		extern unsigned long curAlarmSet;    
		extern unsigned long rtc_fired;
		for(key=0; key<N_RTC_INIT; key++) {
			rtcAlarm[key] = 0;
		}
		curAlarmSet = 0;
		rtc_fired = 0;
		if (!(*P_Reset_Flag & 0x0010)) { 
			// if cold start was due to a reset, power was never removed
			systemCounts.year++;
			saveSystemCounts();	
			strcpy(buffer,"BOOT_TYPE_COLD_RESET -- NEW BATTERIES???");
			if (*P_Hour >= 24) {
				setRTC(0,1,0);  //  reset before saving anything to disk and running macros
				strcat(buffer,"\x0d\x0a" "Clock:Reset due to h>=24");
			}
			logStringRTCOptional(buffer, ASAP, LOG_ALWAYS,0);
		}
		if (isCorrupted((char *)"a:/system")) {
			logString((char *)"Corruption: system",BUFFER,LOG_NORMAL);
			replaceFromBackup("a:/system");
		}
		if (isCorrupted((char *)"a:/log-archive")) {
			logString((char *)"Corruption: log-archive",BUFFER,LOG_NORMAL);
			replaceFromBackup("a:/log-archive");
		}
		if (isCorrupted((char *)"a:/log")) {
			logString((char *)"Corruption: log",BUFFER,LOG_NORMAL);
			replaceFromBackup("a:/log");
		}
		if (isCorrupted((char *)"a:/languages")) {
			logString((char *)"Corruption: languages",BUFFER,LOG_NORMAL);			
			replaceFromBackup("a:/languages");
		}
		if (isCorrupted((char *)"a:/statistics")) {
			logString((char *)"Corruption: statistics",BUFFER,LOG_NORMAL);			
			replaceFromBackup("a:/statistics");
		}
		if (isCorrupted((char *)"a:/messages")) {
			logString((char *)"Corruption: messages",BUFFER,LOG_NORMAL);						
			replaceFromBackup("a:/messages");
		}

		forceflushLog();

#ifdef HALT_ON_COLD_START
		logStringRTCOptional((char *)"Halting after cold start",ASAP,LOG_NORMAL,0);
		setLED(LED_ALL,FALSE);  
		//setOperationalMode((int)P_SLEEP);  //DEVICE-90 - does too much fs activity
		*P_Clock_Ctrl |= 0x200;	//bit 9 KCEN enable IOB0-IOB2 key change interrupt
		turnAmpOff();
		
		//disable SD card
 		*P_IOA_Dir  |= 0x1000;
 		*P_IOA_Attrib |= 0x1000; 	
    	*P_IOA_Buffer  |= 0x1000;	
		// disable NOR flash
	 	*P_IOD_Dir  |= 0x0001;	 
	 	*P_IOD_Attrib |= 0x0001;
	    *P_IOD_Buffer  |= 0x0001;	
		//_SystemOnOff();  // go to P_SLEEP mode, does not return
		SysIntoHaltMode();
#endif
	}

	playDing();  // it is important to play a sound immediately to stop user from wondering if power is on

// for really low batteries the playDing() below will cause a low voltage reset
// handling that will go into Halt mode above
// pressing play or black circle will get back here and the cycle will repeat
// the user will see a long green flash then a short green flash and perhaps hear a partial ding
	


	/*
	adjustVolume(1,0,0);
	dumpSystemDir();
	// repairSystemDir();
	rename((LPSTR)"a:\\inbox",(LPSTR)"a:\\_inbox1");
	rename((LPSTR)"a:\\languages",(LPSTR)"a:\\_languages1");
	rename((LPSTR)"a:\\messages",(LPSTR)"a:\\_1messages");
	rename((LPSTR)"a:\\system",(LPSTR)"a:\\_1system");
	
	*/

	if (!SNexists()) {
		// This will update the version when the device has just been programmed with probe,
		// which wipes out the serial number.
		updateSN(DEFAULT_SYSTEM_PATH);  // use the srn file that might be left from pre-firmware update
		writeVersionToDisk(DEFAULT_SYSTEM_PATH);	
		strcpy(buffer,"Probe Firmware Update:");
		strcat(buffer,VERSION);
		logStringRTCOptional(buffer,ASAP,LOG_NORMAL,0);
	}

// try to load a saved config.bin if present
	configExists = (restore_config_bin() == -1?0:1);
	
	if(!configExists) {
		//config.bin does not exist or is corrupted so not a normal shutdown
		normal_shutdown = 0;
		
		// Try loading config.txt file. If doesn't load, still try firmware update, but then go to testPCB() afterwards.
		configExists = (loadConfigFile() == -1?0:1);
	}
	if (configExists) {
		fixnull_config_strings();
	} else {
		logStringRTCOptional((char *)"No config file bin,txt,or backup. Using defaults.",BUFFER,LOG_ALWAYS,0);
		disaster_config_strings();
	}
			
	// if inspect file was present, check for new firmware, but don't flash if voltage is low
	// Any .img file in the root will be found if inspect is set, but system.img can be found without needing the inspect file
	if((inspect || fileExists((LPSTR)"a:/system.img") )&& V_MIN_SDWRITE_VOLTAGE <= vCur_1) {
		inspect = 1;  
		updateSN(UPDATE_FP);
		if (check_new_sd_flash(filename)) {
			ret = tbOpen((LPSTR)INSPECT_TRIGGER_FILE,O_CREAT|O_RDWR|O_TRUNC);
			close(ret);
			ret = tbOpen((LPSTR)FIRMWARE_UPDATE_NOTIF_FILE,O_CREAT|O_RDWR|O_TRUNC);
			close(ret);
			strcpy(buffer,"Found new firmware:");
			strcat(buffer,filename);
			logStringRTCOptional((char *)buffer,ASAP,LOG_NORMAL,0);
			// inspect file will still be there since fw update will prevent this function from reaching the end.
			// where the unlink is.
			startUpdate(filename);
		}
	}
	
	if (!SNexists()) {
		logException(32,(const char *)"no serial number",LOG_ONLY);
		testPCB();	
	}
	
	if (inspect) {
		ret = checkRTCFile(buffer);
		if (ret >= 0) {
			setRTCFromText(buffer);
			strcpy(buffer,"Clock:");
			getRTC(buffer+strlen(buffer));
			strcat(buffer," (hms reset by *.rtc file)");
			logStringRTCOptional(buffer,BUFFER,LOG_ALWAYS,0);	
		}
	}
	if (!systemCounts.location[0] || !strncmp(systemCounts.location,(char *)"Non-",4))
		playBips(3);
	strcpy(buffer,"Location:");
	strcat(buffer,systemCounts.location);
	strcat(buffer,(const char *)"\x0d\x0a" "Version:" VERSION);
	if (inspect && fileExists((LPSTR)FIRMWARE_UPDATE_NOTIF_FILE)) {
		strcat(buffer," (New firmware)");
		unlink((LPSTR)FIRMWARE_UPDATE_NOTIF_FILE);
	}
	strcat(buffer,(char *)"\x0d\x0a" "Cycle:");
	longToDecimalString(systemCounts.powerUpNumber,(char *)(buffer+strlen(buffer)),4);
	strcat(buffer,(char *)"  Period:");
	longToDecimalString(CLOCK_PERIOD, (char *)(buffer+strlen(buffer)), 4);
	strcat(buffer,(char *)"  Powered Days:");
	longToDecimalString(systemCounts.poweredDays, (char *)(buffer+strlen(buffer)), 4);
	strcat(buffer,"\x0d\x0a" "Debug:");
	switch (DEBUG_MODE) {
		case 0:
			strcat(buffer,"Minimal"); 
			break;
		case 1:
			strcat(buffer,"Normal"); 
			break;
		default:
			strcat(buffer,"Detail"); 
			break;
	}
	if (inspect)
		strcat(buffer,"\x0d\x0a" "Inspect triggerd");
	if(normal_shutdown) {
		if (DEBUG_MODE == LOG_DETAIL) {
			strcat(buffer,(char *)"\x0d\x0a" "Restored configuration from config.bin successfully");
		}
	} else {
		strcat(buffer,(char *)"\x0d\x0a" "Apparently ABNORMAL shutdown (no or corrupt config.bin)");
	}
	logStringRTCOptional(buffer,BUFFER,LOG_ALWAYS,0);
		
	unlink ((LPSTR) (STAT_DIR SNCSV));
	strcpy(buffer,getDeviceSN(1));
	strcat(buffer, ",");
	longToDecimalString(systemCounts.powerUpNumber, strCounts, 4); 
	strcat(buffer, strCounts);
	
	{
		int ret, bytesToWrite;
		char line[80];
		ret = tbOpen((LPSTR)(STAT_DIR SNCSV), O_RDWR|O_CREAT);
		if (ret >= 0) {
			bytesToWrite = convertDoubleToSingleChar(line,buffer,TRUE);
			write(ret, (unsigned long)line<<1, bytesToWrite);
			close(ret);
		}
	}

	initializeProfiles(); 
	if (inspect)
		unlink((LPSTR)INSPECT_TRIGGER_FILE);	
	if (inspect || callProcessInbox)
		processInbox();
	if (callPushPull) { // copy outbox files to connecting device, get stats and audio feedback
		pushContentGetFeedback();
		resetSystem();  //TODO:not sure this is still necessary 
	}
	SD_Initial();  // recordings are bad after USB device connection without this line (todo: figure out why)

	setNextAlarm();	// be sure at least midnight alarm is set	
	ret = *P_RTC_INT_Status;	
	*P_RTC_INT_Status |= ret;	// clear all interrupt flags
	if (MACRO_FILE)	
		loadMacro();
	adjustVolume(NORMAL_VOLUME,FALSE,FALSE);
	adjustSpeed(NORMAL_SPEED,FALSE);
	loadPackage(PKG_SYS,currentProfileLanguage());
	logString("call mainLoop",BUFFER,LOG_DETAIL);
	SetSystemClockRate(CLOCK_RATE); // either set in config file or the default 48 MHz set at beginning of startUp()
	mainLoop();
}
static char * addTextToSystemHeap (char *line) {
	extern char systemHeap [SYSTEM_HEAP_SIZE];
	extern char *cursorSystemHeap;

	int length;
	char *startingHeap, *lineCursor;
	
	while (isspace(*line) && *line != 0)
		line++;
	lineCursor = strchr(line,0); 
	while (isspace(*(lineCursor-1)) && lineCursor >= line)
		lineCursor--;
	*lineCursor = 0;
	length = (lineCursor - line + 1);
	startingHeap = cursorSystemHeap;
	cursorSystemHeap = startingHeap + length;
	if (cursorSystemHeap-systemHeap >= SYSTEM_HEAP_SIZE) {
		logString(line,ASAP,LOG_ALWAYS);
		logException(15,0,USB_MODE); //todo:system heap is full
	}
	strcpy(startingHeap,line);
	return startingHeap;
}

int loadConfigFile(void) {
	int ret, handle;
	char *name, *value;
	char buffer[READ_LENGTH+1];
	int attempt, goodPass;
	const int MAX_RETRIES = 1;
	
	#define OLD_CONFIG_FW_UPDATE_FILENAME "a:/system/config-VERSION-CHANGED.txt"
	ret = 0;	
	buffer[READ_LENGTH] = '\0'; //prevents readLine from searching for \n past buffer
	
	for (attempt = 0,goodPass = 0;attempt < MAX_RETRIES && !goodPass;attempt++) {
		goodPass = 1;
		MACRO_FILE = 0; // default case if no MACRO_FILE listed
		handle = tbOpen((unsigned long)(CONFIG_FILE),O_RDONLY);
		if ((handle == -1) && fileExists((LPSTR)OLD_CONFIG_FW_UPDATE_FILENAME)) { 
			// this code is just for backward compat during the fw upgrade from v1.70 to v1.71
			rename((LPSTR)OLD_CONFIG_FW_UPDATE_FILENAME,(LPSTR)CONFIG_FILE);
			handle = tbOpen((unsigned long)(CONFIG_FILE),O_RDONLY);
		}
		if (handle == -1) {
			replaceFromBackup((char *)CONFIG_FILE);
			handle = tbOpen((unsigned long)(CONFIG_FILE),O_RDONLY);
			if (handle >= 0)
				logStringRTCOptional((char *)"Recovered config.txt from backup folder",BUFFER,LOG_ALWAYS,0);
		}
		if (handle == -1) {
			ret = -1;
			goodPass = 0; //NOTE:can't be logged if log file comes from config file
		} else if (goodPass)
			getLine(-1,0);  // reset in case at end from prior use
		while (goodPass && nextNameValuePair(handle, buffer, ':', &name, &value)) {
			if (!value)
				continue;
			// test there is a new line and that it isn't a comment (starting with "#")
			if(*name == '#')
				continue;
			if (!goodString(name,0) || !goodString(value,0)) { 
				goodPass = 0;
				logException(14,0,0);
				break; //out of while,but continue with for loop
			}
			//
			// to add an integer VALUE here be sure to
			//   1) add it to the struct config_bin in starup.h
			//   2) add a macro SAV_CONFIG_INT(VALUE) to the function write_config.bin
			//   3) add a macro SAV_CONFIG_INT(VALUE) to the function restore_config.bin
			//   4) specify a default value in startup.h and initialize in setDefaults()
			//
			// to add a string VALUE here be sure to
			//   1) add offset_VALUE to the struct config_bin in starup.h
			//   2) add a macro SET_CONFIG_INT(VALUE) to the function write_config.bin
			//   3) add a macro SET_CONFIG_STRING(VALUE) to the function restore_config.bin
			//   4) add the "strlen (VALUE) + 1 + \"  to the CONFIG_BUFLEN macro in startup.h
			//   5) specify a default value in startup.h and initialize in disaster_config_strings()
			//
			if (!strcmp(name,(char *)"KEY_PLAY")) KEY_PLAY=strToInt(value);
				else if (!strcmp(name,(char *)"KEY_LEFT")) KEY_LEFT=strToInt(value);
				else if (!strcmp(name,(char *)"KEY_RIGHT")) KEY_RIGHT=strToInt(value);
				else if (!strcmp(name,(char *)"KEY_UP")) KEY_UP=strToInt(value);
				else if (!strcmp(name,(char *)"KEY_DOWN")) KEY_DOWN=strToInt(value);
				else if (!strcmp(name,(char *)"KEY_SELECT")) KEY_SELECT=strToInt(value);
				else if (!strcmp(name,(char *)"KEY_STAR")) KEY_STAR=strToInt(value);
				else if (!strcmp(name,(char *)"KEY_HOME")) KEY_HOME=strToInt(value);
				else if (!strcmp(name,(char *)"KEY_PLUS")) KEY_PLUS=strToInt(value);
				else if (!strcmp(name,(char *)"KEY_MINUS")) KEY_MINUS=strToInt(value);			
				else if (!strcmp(name,(char *)"LED_GREEN")) LED_GREEN=strToInt(value);
				else if (!strcmp(name,(char *)"LED_RED")) LED_RED=strToInt(value);
				else if (!strcmp(name,(char *)"MAX_SPEED")) MAX_SPEED=strToInt(value);
				else if (!strcmp(name,(char *)"NORMAL_SPEED")) NORMAL_SPEED=strToInt(value);
				else if (!strcmp(name,(char *)"MAX_VOLUME")) MAX_VOLUME=strToInt(value);
				else if (!strcmp(name,(char *)"MIN_VOLUME")) MIN_VOLUME=strToInt(value);
				else if (!strcmp(name,(char *)"NORMAL_VOLUME")) NORMAL_VOLUME=strToInt(value);
				else if (!strcmp(name,(char *)"SPEED_INCREMENT")) SPEED_INCREMENT=strToInt(value);
				else if (!strcmp(name,(char *)"VOLUME_INCREMENT")) VOLUME_INCREMENT=strToInt(value);
				else if (!strcmp(name,(char *)"PROFILE_ORDER_FILE")) PROFILE_ORDER_FILE=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"SYSTEM_PATH")) SYSTEM_PATH=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"LANGUAGES_PATH")) LANGUAGES_PATH=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"UI_SUBDIR")) UI_SUBDIR=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"TOPICS_SUBDIR")) TOPICS_SUBDIR=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"USER_PATH")) USER_PATH=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"LISTS_PATH")) LISTS_PATH=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"INBOX_PATH")) INBOX_PATH=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"OUTBOX_PATH")) OUTBOX_PATH=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"NEW_PKG_SUBDIR")) NEW_PKG_SUBDIR=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"SYS_UPDATE_SUBDIR")) SYS_UPDATE_SUBDIR=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"LOG_FILE")) LOG_FILE=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"LIST_MASTER")) LIST_MASTER=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"MAX_PWR_CYCLES_IN_LOG")) MAX_PWR_CYCLES_IN_LOG=strToInt(value);
				else if (!strcmp(name,(char *)"SYSTEM_VARIABLE_FILE")) SYSTEM_VARIABLE_FILE=addTextToSystemHeap(value);
				// can we make the following prefixes be single-byte chars? 
				// it would make it easier to check list items starts with CUSTOM_PKG_PREFIX
				else if (!strcmp(name,(char *)"PKG_NUM_PREFIX")) PKG_NUM_PREFIX=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"LIST_NUM_PREFIX")) LIST_NUM_PREFIX=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"CUSTOM_PKG_PREFIX")) CUSTOM_PKG_PREFIX=addTextToSystemHeap(value);				
				else if (!strcmp(name,(char *)"AUDIO_FILE_EXT")) AUDIO_FILE_EXT=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"DEFAULT_TIME_PRECISION")) DEFAULT_TIME_PRECISION=strToInt(value);
				else if (!strcmp(name,(char *)"DEFAULT_REWIND")) DEFAULT_REWIND=strToInt(value);
				else if (!strcmp(name,(char *)"INSERT_SOUND_REWIND_MS")) INSERT_SOUND_REWIND_MS=strToInt(value);
				else if (!strcmp(name,(char *)"SPEAK_SOUND_FILE_IDX")) SPEAK_SOUND_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"REC_PAUSED_FILE_IDX")) REC_PAUSED_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"POST_REC_FILE_IDX")) POST_REC_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"PLEASE_WAIT_FILE_IDX")) PLEASE_WAIT_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"NO_TRANSLATION_FILE_IDX")) NO_TRANSLATION_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"POST_TRANSLATE_FILE_IDX")) POST_TRANSLATE_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"OVERWRITE_WARNING_FILE_IDX")) OVERWRITE_WARNING_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"SUBJECT_NOW_LOCKED_IDX")) SUBJECT_NOW_LOCKED_IDX=strToInt(value);				
				else if (!strcmp(name,(char *)"SUBJECT_NOW_UNLOCKED_IDX")) SUBJECT_NOW_UNLOCKED_IDX=strToInt(value);				
				else if (!strcmp(name,(char *)"SORRY_LOCKED_SUBJECT_IDX")) SORRY_LOCKED_SUBJECT_IDX=strToInt(value);				
				else if (!strcmp(name,(char *)"NOT_YET_TRANSLATED_FILE_IDX")) NOT_YET_TRANSLATED_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"PLS_RECORD_TRANSLATION_FILE_IDX")) PLS_RECORD_TRANSLATION_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"PLS_WAIT_FILE_IDX")) PLS_WAIT_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"NEW_RECORDING_FILE_IDX")) NEW_RECORDING_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"ORIG_RECORDING_FILE_IDX")) ORIG_RECORDING_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"INACTIVITY_SOUND_FILE_IDX")) INACTIVITY_SOUND_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"ERROR_SOUND_FILE_IDX")) ERROR_SOUND_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"EMPTY_LIST_FILE_IDX")) EMPTY_LIST_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"DELETED_FILE_IDX")) DELETED_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"PRE_COPY_FILE_IDX")) PRE_COPY_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"POST_COPY_FILE_IDX")) POST_COPY_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"POST_PLAY_FILE_IDX")) POST_PLAY_FILE_IDX=strToInt(value);				
				else if (!strcmp(name,(char *)"HYPERLINK_SOUND_FILE_IDX")) HYPERLINK_SOUND_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"BLOCK_START_LEADER")) BLOCK_START_LEADER=strToInt(value);
				else if (!strcmp(name,(char *)"BLOCK_END_LEADER")) BLOCK_END_LEADER=strToInt(value);
				else if (!strcmp(name,(char *)"BIT_RATE")) BIT_RATE=strToLong(value);
				else if (!strcmp(name,(char *)"GREEN_LED_WHEN_PLAYING")) GREEN_LED_WHEN_PLAYING=strToInt(value);
				else if (!strcmp(name,(char *)"INACTIVITY_SECONDS")) INACTIVITY_SECONDS=strToInt(value);
				else if (!strcmp(name,(char *)"MIC_GAIN_NORMAL")) MIC_GAIN_NORMAL=strToInt(value);
				else if (!strcmp(name,(char *)"MIC_GAIN_HEADPHONE")) MIC_GAIN_HEADPHONE=strToInt(value);
				else if (!strcmp(name,(char *)"USER_CONTROL_TEMPLATE")) USER_CONTROL_TEMPLATE=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"MACRO_FILE")) MACRO_FILE=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"VOLTAGE_SAMPLE_FREQ_SEC")) VOLTAGE_SAMPLE_FREQ_SEC=strToInt(value);
				else if (!strcmp(name,(char *)"USB_CLIENT_POLL_INTERVAL")) USB_CLIENT_POLL_INTERVAL=strToInt(value);
				else if (!strcmp(name,(char *)"DEBUG_MODE")) DEBUG_MODE=strToInt(value);
				else if (!strcmp(name,(char *)"LOG_KEYS")) LOG_KEYS=strToInt(value);
				else if (!strcmp(name,(char *)"CLOCK_RATE")) CLOCK_RATE = strToInt(value);
				else if (!strcmp(name,(char *)"LONG_LIST_NAMES")) LONG_LIST_NAMES=strToInt(value);
				else if (!strcmp(name,(char *)"V_FAST_VOLTAGE_DROP_TIME_SEC")) V_FAST_VOLTAGE_DROP_TIME_SEC=strToInt(value);
				else if (!strcmp(name,(char *)"V_VOLTAGE_DROP_CHECK_INTERVAL")) V_VOLTAGE_DROP_CHECK_INTERVAL=strToInt(value);
				else if (!strcmp(name,(char *)"LONG_KEYPRESS_COUNTER")) LONG_KEYPRESS_COUNTER=strToInt(value);
		}
	}
	if (!goodPass) {
		ret = -1;
		logString((char *)"CONFIG_FILE NOT READ CORRECTLY",BUFFER,LOG_ALWAYS);	  //logException(14,0,USB_MODE); 		
	}
	close(handle);
	LED_ALL = LED_GREEN | LED_RED;
	if (*(LOG_FILE+1) != ':')
		LOG_FILE = 0; // should be == "a://....." otherwise, logging is turned off
	return ret;
}

void initVoltage()
{
	APP_IRAM static unsigned long timeInitialized = -1;
	int i, j, k, sumv;

	// init battery voltage sensing	
	*P_ADC_Setup |= 0x8000;  // enable ADBEN
	*P_MADC_Ctrl &= ~0x05;  // clear CHSEL (channel select)
	*P_MADC_Ctrl |=  0x02;  // select LINEIN1 
	timeInitialized = 0;

	for(i=0, j=0, sumv=0; i<8; ) {	//establish startup voltage
		int v;
//		while((v = getCurVoltageSample()) == 0xffff)
//			;
		for(k=0; k<20; k++) {
			v = getCurVoltageSample();
			if(v != 0xffff)
				break;
			wait(20);
		}
		if(v == 0xffff)
			v = 0;
		
		if(j++ >= 4) {
			sumv += v;
			i++;
		}
	}
	vCur_1 = sumv >> 3; // average 8 readings
	vThresh_1 = 0;
	
	if((vCur_1 > V_MIN_POSSIBLE_VOLTAGE) && (vCur_1 < V_MIN_RUN_VOLTAGE)) {
		refuse_lowvoltage(1);
		// not reached
	}
	set_voltmaxvolume(FALSE);
}
unsigned int GetMemManufacturer()
{
	flash  FL = {0};
	int fl_size = USB_Flash_init((flash *)0, 0);
	int flash_execution_buf[fl_size];
	
	FL.flash_exe_buf = (void *) &flash_execution_buf[0];
	USB_Flash_init(&FL, 1);
	
	return(FL.Flash_type);


}


#define SAV_CONFIG_INT(arg) cfg. ## arg = arg
#define SAV_CONFIG_STRING(string) \
		if(string) { \
			strcpy(cfg_string_buf + offset, string); \
			cfg.offset_ ## string = offset; \
			offset += strlen(string) + 1; \
		} else { \
			cfg.offset_ ## string = offset; \
			cfg_string_buf[offset++] = 0; \
		}

int write_config_bin () {
	int offset = 0, ret = 0, handle;
	CONFIG_BIN cfg;
	int cfg_string_buf_len = CONFIG_BUFLEN;
	char cfg_string_buf[cfg_string_buf_len];

		SAV_CONFIG_INT (KEY_PLAY);
		SAV_CONFIG_INT (KEY_LEFT);
		SAV_CONFIG_INT (KEY_RIGHT);
		SAV_CONFIG_INT (KEY_UP);
		SAV_CONFIG_INT (KEY_DOWN);
		SAV_CONFIG_INT (KEY_SELECT);
		SAV_CONFIG_INT (KEY_STAR);
		SAV_CONFIG_INT (KEY_HOME);
		SAV_CONFIG_INT (KEY_PLUS);
		SAV_CONFIG_INT (KEY_MINUS);			
		SAV_CONFIG_INT (LED_GREEN);
		SAV_CONFIG_INT (LED_RED);
		SAV_CONFIG_INT (MAX_SPEED);
		SAV_CONFIG_INT (NORMAL_SPEED);
		SAV_CONFIG_INT (MAX_VOLUME);
		SAV_CONFIG_INT (MIN_VOLUME);
		SAV_CONFIG_INT (NORMAL_VOLUME);
		SAV_CONFIG_INT (SPEED_INCREMENT);
		SAV_CONFIG_INT (VOLUME_INCREMENT);		
		SAV_CONFIG_STRING (PROFILE_ORDER_FILE);
		SAV_CONFIG_STRING (SYSTEM_PATH);
		SAV_CONFIG_STRING (LANGUAGES_PATH);
		SAV_CONFIG_STRING (UI_SUBDIR);
		SAV_CONFIG_STRING (TOPICS_SUBDIR);
		SAV_CONFIG_STRING (USER_PATH);
		SAV_CONFIG_STRING (LISTS_PATH);
		SAV_CONFIG_STRING (INBOX_PATH);
		SAV_CONFIG_STRING (OUTBOX_PATH);
		SAV_CONFIG_STRING (NEW_PKG_SUBDIR);
		SAV_CONFIG_STRING (SYS_UPDATE_SUBDIR);
		SAV_CONFIG_STRING (LOG_FILE);
		SAV_CONFIG_STRING (LIST_MASTER);
		SAV_CONFIG_INT (MAX_PWR_CYCLES_IN_LOG);
		SAV_CONFIG_STRING (SYSTEM_VARIABLE_FILE);
		SAV_CONFIG_STRING (PKG_NUM_PREFIX);
		SAV_CONFIG_STRING (CUSTOM_PKG_PREFIX);
		SAV_CONFIG_STRING (LIST_NUM_PREFIX);
		SAV_CONFIG_STRING (AUDIO_FILE_EXT);
		SAV_CONFIG_INT (DEFAULT_TIME_PRECISION);
		SAV_CONFIG_INT (DEFAULT_REWIND);
		SAV_CONFIG_INT (INSERT_SOUND_REWIND_MS);
		SAV_CONFIG_INT (SPEAK_SOUND_FILE_IDX);
		SAV_CONFIG_INT (REC_PAUSED_FILE_IDX);
		SAV_CONFIG_INT (POST_REC_FILE_IDX);
		SAV_CONFIG_INT (PLEASE_WAIT_IDX);
		SAV_CONFIG_INT (NO_TRANSLATION_FILE_IDX);
		SAV_CONFIG_INT (POST_TRANSLATE_FILE_IDX);
		SAV_CONFIG_INT (OVERWRITE_WARNING_FILE_IDX);				
		SAV_CONFIG_INT (SUBJECT_NOW_LOCKED_IDX);
		SAV_CONFIG_INT (SUBJECT_NOW_UNLOCKED_IDX);		
		SAV_CONFIG_INT (SORRY_LOCKED_SUBJECT_IDX);
		SAV_CONFIG_INT (NOT_YET_TRANSLATED_FILE_IDX);
		SAV_CONFIG_INT (PLS_RECORD_TRANSLATION_FILE_IDX);
		SAV_CONFIG_INT (PLS_WAIT_FILE_IDX);
		SAV_CONFIG_INT (NEW_RECORDING_FILE_IDX);
		SAV_CONFIG_INT (ORIG_RECORDING_FILE_IDX);
		SAV_CONFIG_INT (INACTIVITY_SOUND_FILE_IDX);
		SAV_CONFIG_INT (ERROR_SOUND_FILE_IDX);
		SAV_CONFIG_INT (EMPTY_LIST_FILE_IDX);
		SAV_CONFIG_INT (DELETED_FILE_IDX);
		SAV_CONFIG_INT (PRE_COPY_FILE_IDX);
		SAV_CONFIG_INT (POST_COPY_FILE_IDX);
		SAV_CONFIG_INT (POST_PLAY_FILE_IDX);				
		SAV_CONFIG_INT (HYPERLINK_SOUND_FILE_IDX);
		SAV_CONFIG_INT (BLOCK_START_LEADER);
		SAV_CONFIG_INT (BLOCK_END_LEADER);
		SAV_CONFIG_INT (BIT_RATE);
		SAV_CONFIG_INT (GREEN_LED_WHEN_PLAYING);
		SAV_CONFIG_INT (INACTIVITY_SECONDS);
		SAV_CONFIG_INT (MIC_GAIN_NORMAL);
		SAV_CONFIG_INT (MIC_GAIN_HEADPHONE);		
 		SAV_CONFIG_STRING (USER_CONTROL_TEMPLATE);
		SAV_CONFIG_STRING (MACRO_FILE);
		SAV_CONFIG_INT (VOLTAGE_SAMPLE_FREQ_SEC);
		SAV_CONFIG_INT (USB_CLIENT_POLL_INTERVAL);
		SAV_CONFIG_INT (DEBUG_MODE);
		SAV_CONFIG_INT (LOG_KEYS);
		SAV_CONFIG_INT (CLOCK_RATE);
		SAV_CONFIG_INT (LONG_LIST_NAMES);
		SAV_CONFIG_INT (V_FAST_VOLTAGE_DROP_TIME_SEC);
		SAV_CONFIG_INT (V_VOLTAGE_DROP_CHECK_INTERVAL);
		SAV_CONFIG_INT (LONG_KEYPRESS_COUNTER);\
		
		cfg.len_string_buf = cfg_string_buf_len;
		
		handle = tbOpen((unsigned long)(CONFIG_BIN_FILE),O_WRONLY|O_CREAT|O_TRUNC);
		if(handle >= 0) {
			int byteswritten;
			byteswritten = sizeof(cfg);
			byteswritten = write(handle, (unsigned long)&cfg<< 1, sizeof(cfg)<< 1);
			byteswritten += write(handle, (unsigned long)&cfg_string_buf<<1, cfg_string_buf_len<<1);
			close(handle);
			ret = byteswritten;
		} else {
			ret = -1;
			logStringRTCOptional((char *)"CANNOT OPEN CONFIG_BIN_FILE" ,ASAP,LOG_ALWAYS,0);
		}
		
	return(ret);	
}

//restore from a saved config.bin if possible, unlink CONFIG_BIN_FILE before returning
static int restore_config_bin () {
	int ret = -1, handle, stringbuf_size, bytesread, stringsread;
	CONFIG_BIN cfg;
//	int debug = 0;
		
	handle = tbOpen((unsigned long)(CONFIG_BIN_FILE),O_RDONLY);
	if(handle < 0) {
		//The "Apparently ABNORMAL shutdown" log message covers the status listed below.
		//logStringRTCOptional((char *)"Binary config not present" ,BUFFER,LOG_DETAIL,0);
		return(ret);
	}
	bytesread = read(handle, (unsigned long)&cfg<< 1, sizeof(cfg)<< 1);
	if(bytesread != (sizeof(cfg)<< 1)) {
		logStringRTCOptional((char *)"config.bin too short" ,BUFFER,LOG_ALWAYS,0);
		close(handle);
		unlink((LPSTR)CONFIG_BIN_FILE);
		return(ret);
	}
	
#if 0	
	if(debug != 0) {
		ret = chkconfig_debug(&cfg, handle);
		close(handle);
		if(ret == 0) {
			logStringRTCOptional((char *)"config.bin good" ,BUFFER,LOG_ALWAYS,0);
		}else {
			logString((char *)"config.bin has errors" ,BUFFER,LOG_ALWAYS);
		}
		return(ret);
	}
#endif
	
#define SET_CONFIG_INT(key) key = cfg. ## key	
	SET_CONFIG_INT(KEY_PLAY);
	SET_CONFIG_INT(KEY_LEFT);
	SET_CONFIG_INT(KEY_RIGHT);
	SET_CONFIG_INT(KEY_UP);
	SET_CONFIG_INT(KEY_DOWN);
	SET_CONFIG_INT(KEY_SELECT);
	SET_CONFIG_INT(KEY_STAR);
	SET_CONFIG_INT(KEY_HOME);
	SET_CONFIG_INT(KEY_PLUS);
	SET_CONFIG_INT(KEY_MINUS);			
	SET_CONFIG_INT(LED_GREEN);
	SET_CONFIG_INT(LED_RED);
	SET_CONFIG_INT(MAX_SPEED);
	SET_CONFIG_INT(NORMAL_SPEED);
	SET_CONFIG_INT(MAX_VOLUME);
	SET_CONFIG_INT(MIN_VOLUME);
	SET_CONFIG_INT(NORMAL_VOLUME);
	SET_CONFIG_INT(SPEED_INCREMENT);
	SET_CONFIG_INT(VOLUME_INCREMENT);
	SET_CONFIG_INT(MAX_PWR_CYCLES_IN_LOG);
	SET_CONFIG_INT(DEFAULT_TIME_PRECISION);
	SET_CONFIG_INT(DEFAULT_REWIND);
	SET_CONFIG_INT(INSERT_SOUND_REWIND_MS);
	SET_CONFIG_INT(SPEAK_SOUND_FILE_IDX);
	SET_CONFIG_INT(REC_PAUSED_FILE_IDX);
	SET_CONFIG_INT(POST_REC_FILE_IDX);
	SET_CONFIG_INT(PLEASE_WAIT_IDX);
	SET_CONFIG_INT(NO_TRANSLATION_FILE_IDX);
	SET_CONFIG_INT(POST_TRANSLATE_FILE_IDX);
	SET_CONFIG_INT(OVERWRITE_WARNING_FILE_IDX);				
	SET_CONFIG_INT(SUBJECT_NOW_LOCKED_IDX);
	SET_CONFIG_INT(SUBJECT_NOW_UNLOCKED_IDX);
	SET_CONFIG_INT(SORRY_LOCKED_SUBJECT_IDX);
	SET_CONFIG_INT(NOT_YET_TRANSLATED_FILE_IDX);
	SET_CONFIG_INT(PLS_RECORD_TRANSLATION_FILE_IDX);
	SET_CONFIG_INT(PLS_WAIT_FILE_IDX);
	SET_CONFIG_INT(NEW_RECORDING_FILE_IDX);
	SET_CONFIG_INT(ORIG_RECORDING_FILE_IDX);
	SET_CONFIG_INT(INACTIVITY_SOUND_FILE_IDX);
	SET_CONFIG_INT(ERROR_SOUND_FILE_IDX);
	SET_CONFIG_INT(EMPTY_LIST_FILE_IDX);
	SET_CONFIG_INT(DELETED_FILE_IDX);
	SET_CONFIG_INT(PRE_COPY_FILE_IDX);
	SET_CONFIG_INT(POST_COPY_FILE_IDX);
	SET_CONFIG_INT(POST_PLAY_FILE_IDX);				
	SET_CONFIG_INT(HYPERLINK_SOUND_FILE_IDX);
	SET_CONFIG_INT(BLOCK_START_LEADER);
	SET_CONFIG_INT(BLOCK_END_LEADER);
	SET_CONFIG_INT(BIT_RATE);
	SET_CONFIG_INT(GREEN_LED_WHEN_PLAYING);
	SET_CONFIG_INT(INACTIVITY_SECONDS);
	SET_CONFIG_INT(MIC_GAIN_NORMAL);
	SET_CONFIG_INT(MIC_GAIN_HEADPHONE);
	SET_CONFIG_INT(VOLTAGE_SAMPLE_FREQ_SEC);
	SET_CONFIG_INT(USB_CLIENT_POLL_INTERVAL);
	SET_CONFIG_INT(DEBUG_MODE);
	SET_CONFIG_INT(LOG_KEYS);
	SET_CONFIG_INT(CLOCK_RATE);
	SET_CONFIG_INT(LONG_LIST_NAMES);
	SET_CONFIG_INT(V_FAST_VOLTAGE_DROP_TIME_SEC);
	SET_CONFIG_INT(V_VOLTAGE_DROP_CHECK_INTERVAL);
	SET_CONFIG_INT(LONG_KEYPRESS_COUNTER);

// read int string table		
	stringbuf_size = cfg.len_string_buf;
	{
		char cfg_string_buf[stringbuf_size];
		stringsread = read(handle, (unsigned long)&cfg_string_buf<< 1, stringbuf_size<< 1);
		close(handle);
		unlink((LPSTR)CONFIG_BIN_FILE);
		if(stringsread != (stringbuf_size<< 1)) {
			logString((char *)"config.bin stringtable too short" ,BUFFER,LOG_ALWAYS);
			return(ret);
		}
#define SET_CONFIG_STRING(str) if(cfg_string_buf[cfg.offset_ ## str]) \
	str = addTextToSystemHeap(cfg_string_buf + cfg.offset_ ## str)
				
		SET_CONFIG_STRING(PROFILE_ORDER_FILE);
		SET_CONFIG_STRING(SYSTEM_PATH);
		SET_CONFIG_STRING(LANGUAGES_PATH);
		SET_CONFIG_STRING(UI_SUBDIR);
		SET_CONFIG_STRING(TOPICS_SUBDIR);
		SET_CONFIG_STRING(USER_PATH);
		SET_CONFIG_STRING(LISTS_PATH);
		SET_CONFIG_STRING(INBOX_PATH);
		SET_CONFIG_STRING(OUTBOX_PATH);
		SET_CONFIG_STRING(NEW_PKG_SUBDIR);
		SET_CONFIG_STRING(SYS_UPDATE_SUBDIR);
		SET_CONFIG_STRING(LOG_FILE);
		SET_CONFIG_STRING(LIST_MASTER);
		SET_CONFIG_STRING(SYSTEM_VARIABLE_FILE);
		SET_CONFIG_STRING(PKG_NUM_PREFIX);
		SET_CONFIG_STRING(LIST_NUM_PREFIX);
		SET_CONFIG_STRING(CUSTOM_PKG_PREFIX);				
		SET_CONFIG_STRING(AUDIO_FILE_EXT);
		SET_CONFIG_STRING(USER_CONTROL_TEMPLATE);
		SET_CONFIG_STRING(MACRO_FILE);
	}
	
	LED_ALL = LED_GREEN | LED_RED;
	if (*(LOG_FILE+1) != ':')
		LOG_FILE = 0; // should be == "a://....." otherwise, logging is turned off

	ret = bytesread + stringsread;
	return(ret);

}

static int disaster_config_strings() {
		PROFILE_ORDER_FILE    = addTextToSystemHeap(DEFAULT_PROFILE_ORDER_FILE);
		SYSTEM_PATH           = addTextToSystemHeap(DEFAULT_SYSTEM_PATH);
		LANGUAGES_PATH        = addTextToSystemHeap(DEFAULT_LANGUAGES_PATH);
		UI_SUBDIR             = addTextToSystemHeap(DEFAULT_UI_SUBDIR);
		TOPICS_SUBDIR         = addTextToSystemHeap(DEFAULT_TOPICS_SUBDIR);
		USER_PATH             = addTextToSystemHeap(DEFAULT_USER_PATH);
		LISTS_PATH            = addTextToSystemHeap(DEFAULT_LISTS_PATH);
		INBOX_PATH            = addTextToSystemHeap(DEFAULT_INBOX_PATH);
		OUTBOX_PATH           = addTextToSystemHeap(DEFAULT_OUTBOX_PATH);
		NEW_PKG_SUBDIR        = addTextToSystemHeap(DEFAULT_NEW_PKG_SUBDIR);
		SYS_UPDATE_SUBDIR     = addTextToSystemHeap(DEFAULT_SYS_UPDATE_SUBDIR);
		LOG_FILE              = addTextToSystemHeap(DEFAULT_LOG_FILE);
		LIST_MASTER           = addTextToSystemHeap(DEFAULT_LIST_MASTER);
		SYSTEM_VARIABLE_FILE  = addTextToSystemHeap(DEFAULT_SYSTEM_VARIABLE_FILE);
		PKG_NUM_PREFIX        = addTextToSystemHeap(DEFAULT_PKG_NUM_PREFIX);
		LIST_NUM_PREFIX       = addTextToSystemHeap(DEFAULT_LIST_NUM_PREFIX);
		CUSTOM_PKG_PREFIX     = addTextToSystemHeap(DEFAULT_CUSTOM_PKG_PREFIX);				
		AUDIO_FILE_EXT        = addTextToSystemHeap(DEFAULT_AUDIO_FILE_EXT);
		USER_CONTROL_TEMPLATE = 0;
		MACRO_FILE            = 0;
}

#define FIXNULL(string) if(string == 0) string = addTextToSystemHeap(DEFAULT_ ## string)
static void fixnull_config_strings() {
		FIXNULL(PROFILE_ORDER_FILE);
		FIXNULL(SYSTEM_PATH);
		FIXNULL(LANGUAGES_PATH);
		FIXNULL(UI_SUBDIR);
		FIXNULL(TOPICS_SUBDIR);
		FIXNULL(USER_PATH);
		FIXNULL(LISTS_PATH);
		FIXNULL(INBOX_PATH);
		FIXNULL(OUTBOX_PATH);
		FIXNULL(NEW_PKG_SUBDIR);
		FIXNULL(SYS_UPDATE_SUBDIR);
//		FIXNULL(LOG_FILE);
		FIXNULL(LIST_MASTER);
		FIXNULL(SYSTEM_VARIABLE_FILE);
		FIXNULL(PKG_NUM_PREFIX);
		FIXNULL(LIST_NUM_PREFIX);
		FIXNULL(CUSTOM_PKG_PREFIX);				
		FIXNULL(AUDIO_FILE_EXT);
//	    FIXNULL(USER_CONTROL_TEMPLATE);
//		FIXNULL(MACRO_FILE);
}


// following used during debugging
#if 0
int
chkconfig_debug(CONFIG_BIN *cfg, int handle)
{
	int err = 0, offset = 0, stringbuf_size, bytesread;

#define CHKCFG(key) if(cfg-> ## key != key) logString( # key, BUFFER), err++
	CHKCFG(KEY_PLAY);
	CHKCFG(KEY_LEFT);
	CHKCFG(KEY_RIGHT);
	CHKCFG(KEY_UP);
	CHKCFG(KEY_DOWN);
	CHKCFG(KEY_SELECT);
	CHKCFG(KEY_STAR);
	CHKCFG(KEY_HOME);
	CHKCFG(KEY_PLUS);
	CHKCFG(KEY_MINUS);			
	CHKCFG(LED_GREEN);
	CHKCFG(LED_RED);
	CHKCFG(MAX_SPEED);
	CHKCFG(NORMAL_SPEED);
	CHKCFG(MAX_VOLUME);
	CHKCFG(MIN_VOLUME);
	CHKCFG(NORMAL_VOLUME);
	CHKCFG(SPEED_INCREMENT);
	CHKCFG(VOLUME_INCREMENT);
	CHKCFG(MAX_PWR_CYCLES_IN_LOG);
	CHKCFG(DEFAULT_TIME_PRECISION);
	CHKCFG(DEFAULT_REWIND);
	CHKCFG(INSERT_SOUND_REWIND_MS);
	CHKCFG(SPEAK_SOUND_FILE_IDX);
	CHKCFG(REC_PAUSED_FILE_IDX);
	CHKCFG(POST_REC_FILE_IDX);
	CHKCFG(PLEASE_WAIT_IDX);
	CHKCFG(NO_TRANSLATION_FILE_IDX);
	CHKCFG(POST_TRANSLATE_FILE_IDX);
	CHKCFG(OVERWRITE_WARNING_FILE_IDX);				
	CHKCFG(SUBJECT_NOW_LOCKED_IDX);
	CHKCFG(SUBJECT_NOW_UNLOCKED_IDX);
	CHKCFG(SORRY_LOCKED_SUBJECT_IDX);
	CHKCFG(NOT_YET_TRANSLATED_FILE_IDX);
	CHKCFG(PLS_RECORD_TRANSLATION_FILE_IDX);
	CHKCFG(PLS_WAIT_FILE_IDX);
	CHKCFG(NEW_RECORDING_FILE_IDX);
	CHKCFG(ORIG_RECORDING_FILE_IDX);
	CHKCFG(INACTIVITY_SOUND_FILE_IDX);
	CHKCFG(ERROR_SOUND_FILE_IDX);
	CHKCFG(EMPTY_LIST_FILE_IDX);
	CHKCFG(DELETED_FILE_IDX);
	CHKCFG(PRE_COPY_FILE_IDX);
	CHKCFG(POST_COPY_FILE_IDX);
	CHKCFG(POST_PLAY_FILE_IDX);				
	CHKCFG(HYPERLINK_SOUND_FILE_IDX);
	CHKCFG(BLOCK_START_LEADER);
	CHKCFG(BLOCK_END_LEADER);
	CHKCFG(BIT_RATE);
	CHKCFG(GREEN_LED_WHEN_PLAYING);
	CHKCFG(INACTIVITY_SECONDS);
	CHKCFG(MIC_GAIN_NORMAL);
	CHKCFG(MIC_GAIN_HEADPHONE);
	CHKCFG(VOLTAGE_SAMPLE_FREQ_SEC);
	CHKCFG(USB_CLIENT_POLL_INTERVAL);
	CHKCFG(DEBUG_MODE);
	CHKCFG(LOG_KEYS);
	CHKCFG(CLOCK_RATE);
	CHKCFG(LONG_LIST_NAMES);
	CHKCFG(V_FAST_VOLTAGE_DROP_TIME_SEC);
	CHKCFG(V_VOLTAGE_DROP_CHECK_INTERVAL);
	CHKCFG(LONG_KEYPRESS_COUNTER);

	stringbuf_size = cfg->len_string_buf;
	{
	char cfg_string_buf[stringbuf_size];
	bytesread = read(handle, (unsigned long)&cfg_string_buf<< 1, stringbuf_size<< 1);

#define CHKSTRING(str) if(cfg_string_buf[cfg->offset_ ## str]) \
	if(strcmp(str, cfg_string_buf + cfg->offset_ ## str)) err++
	
	CHKSTRING(PROFILE_ORDER_FILE);
	CHKSTRING(SYSTEM_PATH);
	CHKSTRING(LANGUAGES_PATH);
	CHKSTRING(UI_SUBDIR);
	CHKSTRING(TOPICS_SUBDIR);
	CHKSTRING(USER_PATH);
	CHKSTRING(LISTS_PATH);
	CHKSTRING(INBOX_PATH);
	CHKSTRING(OUTBOX_PATH);
	CHKSTRING(NEW_PKG_SUBDIR);
	CHKSTRING(SYS_UPDATE_SUBDIR);
	CHKSTRING(LOG_FILE);
	CHKSTRING(LIST_MASTER);
	CHKSTRING(SYSTEM_VARIABLE_FILE);
	CHKSTRING(PKG_NUM_PREFIX);
	CHKSTRING(LIST_NUM_PREFIX);
	CHKSTRING(CUSTOM_PKG_PREFIX);				
	CHKSTRING(AUDIO_FILE_EXT);
	CHKSTRING(USER_CONTROL_TEMPLATE);
	CHKSTRING(MACRO_FILE);
	}
	
	return(err);	
}
#endif
