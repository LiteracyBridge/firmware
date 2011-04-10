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
#include "Include/filestats.h"
#include <ctype.h>

extern int testPCB(void);
extern unsigned int SetSystemClockRate(unsigned int);
extern int SystemIntoUDisk(unsigned int);
extern INT16 SD_Initial(void);

static char * addTextToSystemHeap (char *);
static void loadDefaultUserPackage(void);
static int loadConfigFile (void);
static void loadSystemNames(void);
static char *currentSystem(void);
static void flagConfigFile(void);
static int resetConfigFile(void);

// These capitalized variables are set in the config file.
APP_IRAM int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;
APP_IRAM int ADMIN_COMBO_KEYS;	
APP_IRAM int LED_GREEN, LED_RED, LED_ALL;
APP_IRAM int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
APP_IRAM int NORMAL_VOLUME, MAX_VOLUME, VOLUME_INCREMENT;
APP_IRAM char *SYSTEM_ORDER_FILE, *SYSTEM_PATH, *LANGUAGES_PATH, *UI_SUBDIR, *TOPICS_SUBDIR, *USER_PATH, *LISTS_PATH;
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
    INACTIVITY_SOUND_FILE_IDX, ERROR_SOUND_FILE_IDX, EMPTY_LIST_FILE_IDX, REC_PAUSED_FILE_IDX, POST_REC_FILE_IDX,
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
//if most recent 16 voltage readings are below vCur_1 (vThresh_1 == 0xffff),
//   subtract 1 from vCur_1 and zero vThreah_1
APP_IRAM unsigned int vCur_1;
APP_IRAM int vThresh_1;
APP_IRAM unsigned int MEM_TYPE;   // sst or mx

APP_IRAM static char *pSystemNames[MAX_SYSTEMS];
APP_IRAM static int intCurrentSystem;
APP_IRAM static int totalSystems;

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
	INACTIVITY_SECONDS = DEFAULT_INACTIVITY_SECONDS;
	USB_CLIENT_POLL_INTERVAL = DEFAULT_USB_CLIENT_POLL_INTERVAL;
	
	ADMIN_COMBO_KEYS = KEY_UP | KEY_DOWN;
	LONG_KEYPRESS_COUNTER = KEY_LONG_DOWN_THRESH;
}

void startUp(unsigned int bootType) {
	char buffer[200];
	char strCounts[32];
	char filename[FILE_LENGTH];
	int key;
	
	SetSystemClockRate(MAX_CLOCK_SPEED); // to speed up initial startup -- set CLOCK_RATE later

	setDefaults();
	setLED(LED_RED,FALSE);  // red light can be left on after reprog restart
	setLED(LED_GREEN,TRUE);  // red light can be left on after reprog restart
	
	//to stop user from wondering if power is on and possibly cycling too quickly,
	playDing();  // it is important to play a sound immediately 
//	cleanUpOldRevs();	
	key = keyCheck(1);  // long keycheck 
	key &= ~LONG_KEY_STROKE;
	
	// voltage checks in SystemIntoUSB.c
	if (key == KEY_STAR || key == KEY_MINUS) {
		// allows USB device mode no matter what is on memory card
		Snd_Stop();
		cleanUpOldRevs(); // cleanup any old revs before someone sees the file system
		SystemIntoUDisk(1);	
		SD_Initial();  // recordings are bad after USB device connection without this line (todo: figure out why)
		loadConfigFile();
		processInbox();
		resetSystem();
	} else if (key == KEY_PLUS) {
		// outbox mode: copy outbox files to connecting device
		loadConfigFile();
		copyOutbox();
		resetSystem();
	}	

	if (!SNexists()) {
		// This will update the version when the device has just been programmed with probe,
		// which wipes out the serial number.
		writeVersionToDisk();	
	}
	// check for new firmware first, but don't flash if voltage is low
	if(V_MIN_SDWRITE_VOLTAGE <= vCur_1) {
		updateSN();
		if (check_new_sd_flash(filename)) {
			flagConfigFile();  //change config file name to indicate about to be reprogrammed
			startUpdate(filename);
		}
	}
	
	if (loadConfigFile() == -1) { // config.txt file not found
		testPCB();	
	}
	if (!SNexists()) {
		logException(32,(const char *)"no serial number",LOG_ONLY);
		testPCB();	
	}
	SysDisableWaitMode(WAITMODE_CHANNEL_A);
	adjustVolume(NORMAL_VOLUME,FALSE,FALSE);
	adjustSpeed(NORMAL_SPEED,FALSE);
	loadDefaultUserPackage();
	if (MACRO_FILE)	
		loadMacro();
	loadSystemCounts();
	systemCounts.powerUpNumber++;
	if (systemCounts.powerUpNumber - systemCounts.lastLogErase > MAX_PWR_CYCLES_IN_LOG) {
		systemCounts.lastLogErase = systemCounts.powerUpNumber;
		clearStaleLog();	
	}
//#ifndef TB_CAN_WAKE
//	if(MEM_TYPE == MX_MID) {
	if(bootType == BOOT_TYPE_COLD_RESET) {
		resetRTC();  //  reset before saving anything to disk and running macros
		systemCounts.month = 1;
		systemCounts.monthday = 1;
		systemCounts.poweredDays = 1;
	}
//	}
//#endif	
	saveSystemCounts();
	
	strcpy(buffer,"\x0d\x0a" "---------------------------------------------------\x0d\x0a");
	strcat(buffer,getDeviceSN(1));
	strcpy(strCounts,(char *)" counts:S");
	longToDecimalString(systemCounts.powerUpNumber, strCounts+9, 4); 
	strcat(strCounts,(char *)"P");
	longToDecimalString(systemCounts.packageNumber, strCounts+14, 4); 
	strcat(strCounts,(char *)"R");
	longToDecimalString(systemCounts.revdPkgNumber, strCounts+19, 4);
	strcat(strCounts,(char *)"PD");
	longToDecimalString(systemCounts.poweredDays, strCounts+25, 4);
	strcat(buffer,strCounts); 
	strcat(buffer,"\x0d\x0a" "CYCLE "); //cycle number
	longToDecimalString(systemCounts.powerUpNumber,(char *)(buffer+strlen(buffer)),4);
	strcat(buffer,(const char *)" - version " VERSION);
	logString(buffer,BUFFER);
	
//#ifdef TB_CAN_WAKE
	if(MEM_TYPE == MX_MID) {
		logRTC();  
	}
//#endif
	SetSystemClockRate(CLOCK_RATE); // either set in config file or the default 48 MHz set at beginning of startUp()

	unlink ((LPSTR) (STAT_DIR SNCSV));
	strcpy(buffer,getDeviceSN(1));
	strcat(buffer, ",");
	longToDecimalString(systemCounts.powerUpNumber, strCounts, 4); 
	strcat(buffer, strCounts);
	
	{
		int ret, bytesToWrite;
		char line[80];
		ret = open((LPSTR)(STAT_DIR SNCSV), O_RDWR|O_CREAT);
		if (ret >= 0) {
			bytesToWrite = convertDoubleToSingleChar(line,buffer,TRUE);
			write(ret, (unsigned long)line<<1, bytesToWrite);
			close(ret);
		}
	}
	loadSystemNames(); 
	SD_Initial();  // recordings are bad after USB device connection without this line (todo: figure out why)
	loadPackage(PKG_SYS,currentSystem());
	
//	resetRTC23();  // test 
	setRTCalarm(0, 0, 0);
	
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
		logString(line,ASAP);
		logException(15,0,USB_MODE); //todo:system heap is full
	}
	strcpy(startingHeap,line);
	return startingHeap;
}

static void flagConfigFile(void) {
	// This fct is used to cause a read error on control file open only when reprogramming has recently happened
	// This means that there is no time-costly check to disk to see if reprogramming has happened;
	// instead we know only when the config file isn't there.
	rename((LPSTR)CONFIG_FILE,(LPSTR)FLAGGED_CONFIG_FILE);
}

static int resetConfigFile(void) {
	// see flagConfigFile()
	int ret;
	ret = rename((LPSTR)FLAGGED_CONFIG_FILE,(LPSTR)CONFIG_FILE);
	return ret;	
}	

int loadConfigFile(void) {
	int ret, handle;
	char *name, *value;
	char buffer[READ_LENGTH+1];
	int attempt, goodPass;
	const int MAX_RETRIES = 3;

	ret = 0;	
	buffer[READ_LENGTH] = '\0'; //prevents readLine from searching for \n past buffer
	
	for (attempt = 0,goodPass = 0;attempt < MAX_RETRIES && !goodPass;attempt++) {
		goodPass = 1;
		LOG_FILE = 0; //default in case no logging location in config file (turns logging off)
		MACRO_FILE = 0; // default case if no MACRO_FILE listed
		handle = tbOpen((unsigned long)(CONFIG_FILE),O_RDONLY);
		if (handle == -1) {
			handle = tbOpen((unsigned long)(ALT_CONFIG_FILE),O_RDONLY);
			if (handle == -1) {
				// check if config file was renamed to indicate reprogramming was recently started
				if (resetConfigFile() >= 0) {
					writeVersionToDisk();
					handle = tbOpen((unsigned long)(CONFIG_FILE),O_RDONLY);
				}
			}
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
				else if (!strcmp(name,(char *)"SYSTEM_ORDER_FILE")) SYSTEM_ORDER_FILE=addTextToSystemHeap(value);
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
				else if (!strcmp(name,(char *)"LONG_KEYPRESS_COUNTER")) LONG_KEYPRESS_COUNTER=strToInt(value);
		}
	}
	if (!goodPass) {
		ret = -1;
		logString((char *)"CONFIG_FILE NOT READ CORRECTLY",BUFFER);	  //logException(14,0,USB_MODE); 		
	}
	close(handle);
	LED_ALL = LED_GREEN | LED_RED;
	if (*(LOG_FILE+1) != ':')
		LOG_FILE = 0; // should be == "a://....." otherwise, logging is turned off
	return ret;
}

static void loadDefaultUserPackage(void) {
	char sTemp[PATH_LENGTH];
	
	strcpy(sTemp,SYSTEM_PATH);
	strcat(sTemp,USER_CONTROL_TEMPLATE);
	strcat(sTemp,".txt"); //todo: move to config file	
	memset(&pkgDefault,0,sizeof(CtnrPackage));
	pkgDefault.pkg_type = PKG_MSG;
	parseControlFile (sTemp, &pkgDefault);
}
void initVoltage()
{
	extern void set_voltmaxvolume();
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
	set_voltmaxvolume();
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


static void loadSystemNames() {
	int handle;
	char *cursorRead;
	char systemOrderFile[PATH_LENGTH];
	char buffer[READ_LENGTH+1];
	
	strcpy(systemOrderFile,LANGUAGES_PATH);
	strcat(systemOrderFile,SYSTEM_ORDER_FILE);	
	strcat(systemOrderFile,".txt"); //todo: move to config file	
	handle = tbOpen((LPSTR)systemOrderFile,O_RDONLY);
	if (handle == -1) {
		logException(33,systemOrderFile,USB_MODE);
	}
	getLine(-1,0);  // reset in case at end from prior use
	while ((cursorRead = getLine(handle,buffer)) && (intCurrentSystem < MAX_SYSTEMS))	
		pSystemNames[intCurrentSystem++] = addTextToSystemHeap(cursorRead);
	totalSystems = intCurrentSystem;
	intCurrentSystem = 0;
}

static char *currentSystem() {
	char * ret;
	
	ret = pSystemNames[intCurrentSystem];
	return ret;
}

char *nextSystem() {
	char * ret;
	
	if (++intCurrentSystem == totalSystems)
		intCurrentSystem = 0;
	ret = pSystemNames[intCurrentSystem];
	return ret;
}

char *prevSystem() {
	char * ret;
	
	if (--intCurrentSystem == -1)
		intCurrentSystem = totalSystems - 1;
	ret = pSystemNames[intCurrentSystem];
	return ret;
}

void cleanUpOldRevs() {
	int ret;
	struct f_info file_info;
			
	if (dirExists((LPSTR)"a://Firmware")) {
		tbChdir((LPSTR)"a://Firmware");
		ret =_findfirst((LPSTR)"*.*", &file_info, D_FILE);
		while (ret >= 0) {
			ret = unlink((LPSTR)file_info.f_name);
			ret = _findnext(&file_info);	
		}	
		tbChdir((LPSTR)"a://");
		rmdir((LPSTR)"a://Firmware");
	}
}
