// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "./system/include/system_head.h"
#include "Include/talkingbook.h"
#include "Include/files.h"
#include "Include/device.h"
#include "Include/parsing.h"
#include "Include/macro.h"
#include "Include/sys_counters.h"
#include "Include/Inbox.h"
#include "Include/audio.h"
#include "Include/SD_reprog.h"
#include "Include/mainLoop.h"
#include "Include/startup.h"
#include <ctype.h>

#define SYSTEM_HEAP_SIZE 512	//config file values
#define CONFIG_FILE		"a://config.txt"

extern unsigned int SetSystemClockRate(unsigned int);
extern int SystemIntoUDisk(unsigned int);

static char * addTextToSystemHeap (char *);
static void loadDefaultPackage(void);
static void loadConfigFile (void);

// These capitalized variables are set in the config file.
APP_IRAM int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;
APP_IRAM int ADMIN_COMBO_KEYS;	
APP_IRAM int LED_GREEN, LED_RED, LED_ALL;
APP_IRAM int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
APP_IRAM int NORMAL_VOLUME, MAX_VOLUME, VOLUME_INCREMENT;
APP_IRAM char *BOOT_PACKAGE, *SYSTEM_PATH, *USER_PATH, *LIST_PATH;
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
    INACTIVITY_SOUND_FILE_IDX, ERROR_SOUND_FILE_IDX, EMPTY_LIST_FILE_IDX;
APP_IRAM int BLOCK_START_LEADER, BLOCK_END_LEADER;
APP_IRAM long BIT_RATE;
APP_IRAM int GREEN_LED_WHEN_PLAYING;
APP_IRAM int INACTIVITY_SECONDS;
APP_IRAM int MIC_GAIN_NORMAL, MIC_GAIN_HEADPHONE;
APP_IRAM char *CONTROL_TEMPLATE;
APP_IRAM char *MACRO_FILE;
APP_IRAM int VOLTAGE_SAMPLE_FREQ_SEC, USB_CLIENT_POLL_INTERVAL;
APP_IRAM int LOG_WARNINGS, LOG_KEYS;
APP_IRAM unsigned int CLOCK_RATE;
//if most recent 16 voltage readings are below vCur_1 (vThresh_1 == 0xffff),
//   subtract 1 from vCur_1 and zero vThreah_1
APP_IRAM unsigned int vCur_1;
APP_IRAM int vThresh_1;

void setDefaults(void) {
	// This function sets variables that are usually set in config file, 
	// but they need defaults in case config file doesn't list them or isn't loaded yet
	KEY_PLAY = DEFAULT_KEY_PLAY;
	KEY_LEFT = DEFAULT_KEY_LEFT;
	KEY_RIGHT = DEFAULT_KEY_RIGHT;
	KEY_UP = DEFAULT_KEY_UP;
	KEY_DOWN = DEFAULT_KEY_DOWN;
	KEY_SELECT = DEFAULT_KEY_SELECT;
	KEY_STAR = DEFAULT_KEY_STAR;
	KEY_HOME = DEFAULT_KEY_HOME;
	KEY_PLUS = DEFAULT_KEY_PLUS;
	KEY_MINUS = DEFAULT_KEY_MINUS;
	LED_GREEN = DEFAULT_LED_GREEN;
	LED_RED = DEFAULT_LED_RED;
	LED_ALL = LED_GREEN | LED_RED;
	MIC_GAIN_NORMAL = DEFAULT_MIC_GAIN_NORMAL;
	MIC_GAIN_HEADPHONE = DEFAULT_MIC_GAIN_HEADPHONE; 
	CLOCK_RATE = DEFAULT_CLOCK_RATE;
	NORMAL_VOLUME = DEFAULT_NORMAL_VOLUME;
	MAX_VOLUME = DEFAULT_MAX_VOLUME;
	VOLUME_INCREMENT = DEFAULT_VOLUME_INCREMENT;
	NORMAL_SPEED = DEFAULT_NORMAL_SPEED;
	MAX_SPEED = DEFAULT_MAX_SPEED;
	SPEED_INCREMENT = DEFAULT_SPEED_INCREMENT;
	BIT_RATE = DEFAULT_BIT_RATE;
	
	ADMIN_COMBO_KEYS = KEY_UP | KEY_DOWN;
}

void startUp(void) {
	char buffer[100];
	int key;
	
	SetSystemClockRate(MAX_CLOCK_SPEED); // to speed up initial startup -- set CLOCK_RATE later

	setDefaults();
	setLED(LED_RED,FALSE);  // red light can be left on after reprog restart
	setLED(LED_GREEN,TRUE);  // red light can be left on after reprog restart
	
	//to stop user from wondering if power is on and possibly cycling too quickly,
	playDing();  // it is important to play a sound immediately 
	
	key = keyCheck(1);  // long keycheck 
	
	// voltage checks in SystemIntoUSB.c
	if (key == KEY_STAR || key == KEY_MINUS) {
		// allows USB device mode no matter what is on memory card
		Snd_Stop();
		SystemIntoUDisk(1);	
		loadConfigFile();
		processInbox();
		resetSystem();
	} else if (key == KEY_PLUS) {
		// outbox mode: copy outbox files to connecting device
		loadConfigFile();
		copyOutbox();
		resetSystem();
	}	
	
	// check for new firmware first, but don't flash if voltage is low
	if(V_MIN_SDWRITE_VOLTAGE <= vCur_1) {
		check_new_sd_flash();
	}
	
	loadConfigFile();	
	if (!SNexists())
		logException(32,(const char *)"no serial number",SHUT_DOWN);		

	SysDisableWaitMode(WAITMODE_CHANNEL_A);
	adjustVolume(NORMAL_VOLUME,FALSE,FALSE);
	adjustSpeed(NORMAL_SPEED,FALSE);
	loadDefaultPackage();
	if (MACRO_FILE)	
		loadMacro();
	loadSystemCounts();
	systemCounts.powerUpNumber++;
	if (systemCounts.powerUpNumber - systemCounts.lastLogErase > MAX_PWR_CYCLES_IN_LOG) {
		systemCounts.lastLogErase = systemCounts.powerUpNumber;
		clearStaleLog();	
	}
#ifndef TB_CAN_WAKE
	resetRTC();  //  reset before saving anything to disk and running macros
#endif	
	saveSystemCounts();

	strcpy(buffer,"\x0d\x0a" "---------------------------------------------------------\x0d\x0a" "CYCLE "); //cycle number
	longToDecimalString(systemCounts.powerUpNumber,(char *)(buffer+strlen(buffer)),4);
	strcat(buffer,(const char *)" - version " VERSION);
	logString(buffer,BUFFER);
	logString(getDeviceSN(1),BUFFER);
	logSystemCounts();
#ifdef TB_CAN_WAKE
	logRTC();  
#endif
	loadPackage(PKG_SYS,BOOT_PACKAGE);	
	SetSystemClockRate(CLOCK_RATE); // either set in config file or the default 48 MHz set at beginning of startUp()
	mainLoop();
}

static char * addTextToSystemHeap (char *line) {
	APP_IRAM static char systemHeap [SYSTEM_HEAP_SIZE];
	APP_IRAM static char *cursorSystemHeap = systemHeap;
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
		logString(line,ASAP);
		logException(15,0,USB_MODE); //todo:system heap is full
	}
	strcpy(startingHeap,line);
	return startingHeap;
}

static void loadConfigFile(void) {
	int handle;
	char *name, *value;
	char buffer[READ_LENGTH+1];
	int attempt, goodPass;
	const int MAX_RETRIES = 3;
	
	buffer[READ_LENGTH] = '\0'; //prevents readLine from searching for \n past buffer
	
	for (attempt = 0,goodPass = 0;attempt < MAX_RETRIES && !goodPass;attempt++) {
		goodPass = 1;
		LOG_FILE = 0; //default in case no logging location in config file (turns logging off)
		MACRO_FILE = 0; // default case if no MACRO_FILE listed
		handle = tbOpen((unsigned long)(CONFIG_FILE),O_RDONLY);
		if (handle == -1)
			logException(14,0,USB_MODE); // can't open config file -- NOTE -- this can't be logged if log file comes from config
		getLine(-1,0);  // reset in case at end from prior use
		while (goodPass && nextNameValuePair(handle, buffer, ':', &name, &value)) {
			if (!value)
				continue;
			// test there is a new line and that it isn't a comment (starting with "//")
			if (*name == '/' && *(name+1) == '/')
				continue; // move to next line
			if (!goodString(name,0) || !goodString(value,0)) { 
				goodPass = 0;
				logException(14,0,0);
				break; //out of while,but continue with for loop
			}
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
				else if (!strcmp(name,(char *)"NORMAL_VOLUME")) NORMAL_VOLUME=strToInt(value);
				else if (!strcmp(name,(char *)"SPEED_INCREMENT")) SPEED_INCREMENT=strToInt(value);
				else if (!strcmp(name,(char *)"VOLUME_INCREMENT")) VOLUME_INCREMENT=strToInt(value);
				else if (!strcmp(name,(char *)"BOOT_PACKAGE")) BOOT_PACKAGE=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"SYSTEM_PATH")) SYSTEM_PATH=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"USER_PATH")) USER_PATH=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"LIST_PATH")) LIST_PATH=addTextToSystemHeap(value);
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
				else if (!strcmp(name,(char *)"INACTIVITY_SOUND_FILE_IDX")) INACTIVITY_SOUND_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"ERROR_SOUND_FILE_IDX")) ERROR_SOUND_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"EMPTY_LIST_FILE_IDX")) EMPTY_LIST_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"DELETED_FILE_IDX")) DELETED_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"PRE_COPY_FILE_IDX")) PRE_COPY_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"POST_COPY_FILE_IDX")) POST_COPY_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"HYPERLINK_SOUND_FILE_IDX")) HYPERLINK_SOUND_FILE_IDX=strToInt(value);
				else if (!strcmp(name,(char *)"BLOCK_START_LEADER")) BLOCK_START_LEADER=strToInt(value);
				else if (!strcmp(name,(char *)"BLOCK_END_LEADER")) BLOCK_END_LEADER=strToInt(value);
				else if (!strcmp(name,(char *)"BIT_RATE")) BIT_RATE=strToLong(value);
				else if (!strcmp(name,(char *)"GREEN_LED_WHEN_PLAYING")) GREEN_LED_WHEN_PLAYING=strToInt(value);
				else if (!strcmp(name,(char *)"INACTIVITY_SECONDS")) INACTIVITY_SECONDS=strToInt(value);
				else if (!strcmp(name,(char *)"MIC_GAIN_NORMAL")) MIC_GAIN_NORMAL=strToInt(value);
				else if (!strcmp(name,(char *)"MIC_GAIN_HEADPHONE")) MIC_GAIN_HEADPHONE=strToInt(value);
				else if (!strcmp(name,(char *)"CONTROL_TEMPLATE")) CONTROL_TEMPLATE=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"MACRO_FILE")) MACRO_FILE=addTextToSystemHeap(value);
				else if (!strcmp(name,(char *)"VOLTAGE_SAMPLE_FREQ_SEC")) VOLTAGE_SAMPLE_FREQ_SEC=strToInt(value);
				else if (!strcmp(name,(char *)"USB_CLIENT_POLL_INTERVAL")) USB_CLIENT_POLL_INTERVAL=strToInt(value);
				else if (!strcmp(name,(char *)"LOG_WARNINGS")) LOG_WARNINGS=strToInt(value);
				else if (!strcmp(name,(char *)"LOG_KEYS")) LOG_KEYS=strToInt(value);
				else if (!strcmp(name,(char *)"CLOCK_RATE")) CLOCK_RATE = strToInt(value);
		}
	}
	if (!goodPass)
		logException(14,0,USB_MODE); 		
	close(handle);
	LED_ALL = LED_GREEN | LED_RED;
	if (*(LOG_FILE+1) != ':')
		LOG_FILE = 0; // should be == "a://....." otherwise, logging is turned off
}

static void loadDefaultPackage(void) {
	
	pkgDefault.pkg_type = PKG_MSG;
	parseControlFile (CONTROL_TEMPLATE, &pkgDefault);
}
void initVoltage()
{
	extern void set_voltmaxvolume();
	APP_IRAM static unsigned long timeInitialized = -1;
	int i, j, sumv;

	// init battery voltage sensing	
	*P_ADC_Setup |= 0x8000;  // enable ADBEN
	*P_MADC_Ctrl &= ~0x05;  // clear CHSEL (channel select)
	*P_MADC_Ctrl |=  0x02;  // select LINEIN1 
	timeInitialized = 0;

	for(i=0, j=0, sumv=0; i<8; ) {	//establish startup voltage
		int v;
		while((v = getCurVoltageSample()) == 0xffff)
			;
		if(j++ >= 4) {
			sumv += v;
			i++;
		}
	}
	vCur_1 = sumv >> 3; // average 8 readings
	vThresh_1 = 0;
	
	if(vCur_1 < V_MIN_RUN_VOLTAGE) {
		refuse_lowvoltage(1);
		// not reached
	}
	set_voltmaxvolume();
}
