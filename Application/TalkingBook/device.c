// Copyright 2009-2011 Literacy Bridge
// Contact: info@literacybridge.org
#include <ctype.h>
#include "./system/include/system_head.h"
#include "Include/talkingbook.h"
#include "Include/files.h"
#include "Include/macro.h"
#include "Include/containers.h"
#include "Include/Inbox.h"
#include "Include/audio.h"
#include "Include/device.h"
#include "Include/startup.h"
#include "Include/sys_counters.h"
#include "Include/SD_reprog.h"
#include "Include/d2d_copy.h"
#include "Include/pkg_ops.h"

extern int write_config_bin();
extern void _SystemOnOff(void);
extern int SystemIntoUDisk(unsigned int);
extern void KeyScan_ServiceLoop(void);
extern int SP_GetCh(void);
extern INT16 SD_Initial(void);
extern void checkDoubleSRNprefix(void); 
extern void SD_Uninitial(void);		
static void setDate(unsigned int, unsigned int);
static char* findTimePart (char *, char);

static void logKeystroke(int);
//static void Log_ClockCtrl(void);
static void turnSDoff(void);
static void voltageShutdown(void);

APP_IRAM static unsigned int keydown_counter;
APP_IRAM unsigned int vCur_1;
APP_IRAM unsigned long tCur_1;
APP_IRAM int vThresh_1;
APP_IRAM int shuttingDown;
APP_IRAM int inUSBHostMode;

APP_IRAM static int v_high = 0;
APP_IRAM static int v_low = 0xFC;

// data stored between 0 and &rtc_fired+2 is not initialized by startup_Data.asm
//    data stored here survives going into and returning from HALT mode
//    it is initialized in startup.c for a cold reset bootType
asm("APP_RTCALARM: .SECTION .RAM, .ADDR = 0x0");
__attribute__((section(".APP_RTCALARM"))) unsigned long rtcAlarm[N_RTC_INIT];
__attribute__((section(".APP_RTCALARM"))) unsigned long curAlarmSet;
__attribute__((section(".APP_RTCALARM"))) unsigned long rtc_fired;

void resetRTC(void) {
	setRTC(0,0,1);
}

static char*
findTimePart (char *time, char timeCode) {
	char *ptr;	
	
	ptr = strchr(time,timeCode);
	if (ptr) {
		while(isdigit(*(ptr-1)))
			ptr--;
		if (!isdigit(*ptr))
			ptr = NULL;
	}
	return ptr;			
}


static void 
setDate(unsigned int month, unsigned int date) {
	systemCounts.month = month;
	systemCounts.monthday = date;
	//saveSystemCounts();	
}

void setRTCFromText(char *time) {
	// This function parses the date and or time using any of the following format:
	//  1. #h#m#s
	//  2. #m#d
	//  3. #m#d#h#m#s
	//  where '#' represents any valid positive integer.  
	//  Note that 'm' is used for month and for minute. 'm' will mean month, unless it follows an 'h'.
	//  Note that year is not set because it is used to indicate the time period (see cold_start code)
	
	int month = -1, date = -1, hour = -1, min = -1, sec = -1;
	char *ptr;
	
	if ((ptr = findTimePart(time,'m'))) {
		month = strToInt(ptr);
		if ((ptr = findTimePart(time,'d')))
			date = strToInt(ptr);
		else 
			month = -1;
	}
	if ((ptr = findTimePart(time,'h'))) {
		hour = strToInt(ptr);
		if ((ptr = findTimePart(ptr,'m')))
			min = strToInt(ptr);
		if ((ptr = findTimePart(ptr,'s')))
			sec = strToInt(ptr);
	}
	if (month >= 1 && date >= 1)
		setDate(month,date);
	if (hour >= 0 && min >= 0 && sec >= 0)
		setRTC(hour,min,sec);
}

void setRTC(unsigned int h, unsigned int m, unsigned int s) {
#define		P_RTC_HMSBusy                 (volatile unsigned int*)(P_RTC_Ctrl_Base+0x17)
	if (s < 60) {
		while (*P_RTC_HMSBusy) ; // wait till RTC is not busy 
		*P_Second = s;
	}
	if (m < 60) {
		while (*P_RTC_HMSBusy) ; // wait till RTC is not busy 
		*P_Minute = m;
	}
	if (h < 24) {
		while (*P_RTC_HMSBusy) ; // wait till RTC is not busy 
		*P_Hour = h;	
	}
}

extern void appendHiLoVoltage(char *string) {
	longToDecimalString(v_high,string+strlen(string),3);
	strcat(string,"/");
	longToDecimalString(vCur_1,string+strlen(string),3);
	strcat(string,"/");
	longToDecimalString(v_low,string+strlen(string),3);
	strcat(string,"V");	
	v_high = vCur_1;
	v_low = vCur_1;
}

extern void getRTC(char * str) {
	unsigned long r,c,p,d,h,m,s;
	char time[RTC_STRING_LENGTH];
	
	h = (unsigned long) *P_Hour;	
	m = (unsigned long) *P_Minute;
	s = (unsigned long) *P_Second;
	if (h >23) {
		h -= 24;
		setRTC(h,m,s);
		incrementCumulativeDays();
	}
	r = (long)getRotation();
	c = (long)getPowerups();
	p = (long)getPeriod();  
	d = (long)getCumulativeDays();

	if (r >= 0)
		time[0] = r + '0';
	else
		time[0] = '_';
	time[1] = 'r';
	longToDecimalString(c,time+2,4);
	time[6] = 'c';
	if (p >= 0)
		longToDecimalString(p,time+7,3);
	else
		strcpy(time+7,(char *)"___");
	time[10] = 'p';
	longToDecimalString(d,time+11,3);
	time[14] = 'd';
	longToDecimalString(h,time+15,2);
	time[17] = 'h';
	longToDecimalString(m,time+18,2);
	time[20] = 'm';
	longToDecimalString(s,time+21,2);
	time[23] = 's';
	time[24] = 0;
	strcpy(str,time);
}
	
/* NO LONGER USED SINCE RTC IS PRE-PENDED TO EVERY LOGSTRING
void logRTC(void) {
	unsigned long h,m,s;
	char time[12];

	h = (unsigned long) *P_Hour;	
	m = (unsigned long) *P_Minute;
	s = (unsigned long) *P_Second;

	longToDecimalString(h,time,4);
	time[4] = 'h';
	longToDecimalString(m,time+5,2);
	time[7] = 'm';
	longToDecimalString(s,time+8,2);
	time[10] = 's';
	time[11] = 0;
	logString(time,ASAP,LOG_ALWAYS);
}*/

long getRTCinSeconds(void) {
	unsigned long ret, secH;
	unsigned int secM, sec;
	
	sec = (unsigned int)*P_Second;
	secM = (unsigned int)*P_Minute * 60;
	secH = (unsigned long)*P_Hour * 3600;
	ret = sec + secM + secH;
	return (long)ret;
}

void setLED(unsigned int color, BOOL on) {
	struct GPIO {
		unsigned int nData;
		unsigned int nBuffer;
		unsigned int nDir;
		unsigned int nAttrib;
		unsigned int nDrv;
	};
	struct GPIO *LEDPort = (struct GPIO *)P_IOB_Data;
	// on = FALSE;
	if (on) {
 		LEDPort->nDir 	 |= color;
		LEDPort->nAttrib |= color;  // LEDPort->nData   |= color;
		LEDPort->nBuffer   |= color;
	} else
		LEDPort->nBuffer   &= ~color;  //LEDPort->nData   &= ~color;
}

void setUSBDevice (BOOL set) {		
	if (set) {
		Snd_Stop();
		flushLog();
		//cleanUpOldRevs(); // cleanup any old revs
		SystemIntoUDisk(USB_CLIENT_SVC_LOOP_CONTINUOUS);	
		fastShutdown();
//		SD_Initial();  // recordings are bad after USB device connection without this line (todo: figure out why)
//		checkVoltage();  // USB may have been supplying sole power -- need to check if voltage dropping fast		
//		processInbox();
//		checkVoltage();  // USB may have been supplying sole power -- need to check if voltage dropping fast
	}
}

unsigned int
getCurVoltageSample() {	
	unsigned sample;

	do {
		*P_ADC_Setup |= 0x4000;
		*P_MADC_Ctrl |= 0x40; // set STRCNV, starting the voltage sample
	
		while (!(*P_MADC_Ctrl & 0x80)); // wait until CNVRDY (sample is ready)					
		sample = (unsigned int)*P_MADC_Data;
		*P_ADC_Setup &= ~0x4000; // disable ADCEN to save power
		sample /= 99;  //RHM heuristically determined to work on my board, replaced 3 lines below
	} while (sample == 0);
	//	sample >>= 4; // only bits 4-15
	//	f = 2 * 3.3 * (sample / (float)0x0fff); // ref = 3.3v; LINE1 measures 1/2 voltage
	//	sample = (unsigned int) (f * 100); // to give "x.xx" for total voltage
	
	if (sample < V_MIN_POSSIBLE_VOLTAGE)
		sample = V_EXTERNAL_VOLTAGE;	
	return sample;
}

unsigned int
checkVoltage() {	
	int v;
	unsigned long currentTimeInSec;
	unsigned int tripRate;
	APP_IRAM static unsigned long timeStartedFalling;
	APP_IRAM static unsigned int voltageStartedFalling;
	APP_IRAM static unsigned long timePaused = 0;
	APP_IRAM static unsigned int voltagePaused;
	APP_IRAM static unsigned int voltageAvgPaused;
	APP_IRAM static unsigned int readingsPaused = 0;
	char log[80];
	int isPlaying;

	v = getCurVoltageSample();
	isPlaying = (SACM_Status() && !context.isPaused);
	currentTimeInSec = getRTCinSeconds();

	if (v < V_MIN_RUN_VOLTAGE_TRANS) {
		forceflushLog();  // to ensure the log msg below is not beyond buffer
		strcpy(log,"v < V_MIN_RUN (");
		longToDecimalString(v,log+strlen(log),3);
		strcat(log," < ");
		longToDecimalString(V_MIN_RUN_VOLTAGE_TRANS,log+strlen(log),3);		
		strcat(log,")");
		logString(log, BUFFER, LOG_ALWAYS);
		voltageShutdown();
	}
	
	if (isPlaying && timePaused)
		timePaused = 0;
	else if (!isPlaying && !timePaused) {
		timePaused = currentTimeInSec;
		voltageAvgPaused = voltagePaused = v;
		readingsPaused = 1;
	} else if (!isPlaying && timePaused) {
		readingsPaused++;
		if (readingsPaused >= 50 && ((v+10) < voltageAvgPaused)) {
			forceflushLog();  // to ensure the log msg below is not beyond buffer
			strcpy(log,(char *)"Paused v:");
			longToDecimalString(v,log+strlen(log),3);
			strcat(log,(char *)" Avg:");
			longToDecimalString(voltageAvgPaused,log+strlen(log),3);
			strcat(log,(char *)" R:");
			longToDecimalString(readingsPaused,log+strlen(log),5);
			logString(log , BUFFER, LOG_ALWAYS);
			voltageShutdown();
		} 
		voltageAvgPaused = ((long)voltageAvgPaused * (long)(readingsPaused-1) + v) / readingsPaused;
		voltagePaused = v;
	}
	
	if (v > v_high)
		v_high = v;
	if (v < v_low)
		v_low = v;
	if(v < vCur_1) {	// if voltage < vCur_1 set a bit in vThresh_1
		vThresh_1++;
	} else {
		// remember v & t for short-term voltage rate calculation when threshold is reached
		voltageStartedFalling = v;
		timeStartedFalling = currentTimeInSec;
		if (v > vCur_1)
			vThresh_1 = 0;
	}
	if((isPlaying && vThresh_1 == 8) || (!isPlaying && vThresh_1 == 2)) { //low samples when playing or paused
		unsigned int delta_voltage = vCur_1 - v;
//		unsigned int delta_voltage_trans = voltageStartedFalling - v;
		unsigned int voltageDropRateStatic = 0;
//		unsigned int voltageDropRateTrans = 0;
		unsigned int voltageDropTime = currentTimeInSec - tCur_1;
//		unsigned int voltageDropTimeTrans = currentTimeInSec - timeStartedFalling;
		
		if (delta_voltage >= 2)
			voltageDropRateStatic = (delta_voltage * 10) / (voltageDropTime?voltageDropTime:1);// don't divide by 0
/*		if (delta_voltage_trans >= 2)
			voltageDropRateTrans = (delta_voltage_trans * 10) / (voltageDropTimeTrans?voltageDropTimeTrans:1);// don't divide by 0
*/		vCur_1 = v;
		vThresh_1 = 0;	// reset threshold bits

		if (vCur_1 < V_MIN_RUN_VOLTAGE) {
			forceflushLog();  // to ensure the log msg below is not beyond buffer
			logString("vCur_1 < V_MIN_RUN_VOLTAGE" , BUFFER, LOG_ALWAYS);
			voltageShutdown();
		}
		
		if (delta_voltage >= 2 /*|| delta_voltage_trans >= 2*/) {
			if (isPlaying)
				strcpy(log,(char *)"PLAYING ");
			else
				strcpy(log,(char *)"PAUSED ");
			strcat(log, (char *) "VOLTAGE DROP: 0."); 
			longToDecimalString((long)delta_voltage,log+strlen(log),2);
/*			strcat(log,(char *)"v/0.");
			longToDecimalString((long)delta_voltage_trans,log+strlen(log),2);
*/			strcat(log,(char *) "v in ");
			longToDecimalString((long)voltageDropTime,log+strlen(log),4);
/*			strcat(log, (char *) " sec/");
			longToDecimalString((long)voltageDropTimeTrans,log+strlen(log),4);
*/			strcat(log, (char *) " sec");
			logString(log,BUFFER, LOG_ALWAYS);		
						
			strcpy(log, (char *) "VOLTAGE DROP RATE: "); 
			longToDecimalString((long)voltageDropRateStatic, log+strlen(log), 3);
/*			strcat(log, (char *) "/"); 
			longToDecimalString((long)voltageDropRateTrans, log+strlen(log), 3);
*/			logString(log,BUFFER, LOG_ALWAYS);
					
			if (isPlaying) {
				if (vCur_1 < 220) 
					tripRate = 75;
				else if (vCur_1 < 320)
					tripRate = 200;
				else 
					tripRate = 300;
			} else {
				if (vCur_1 < 220) 
					tripRate = 20;
				else if (vCur_1 < 320)
					tripRate = 75;
				else 
					tripRate = 150;
			}
						
			if (voltageDropRateStatic >= tripRate && v < 350) { 
				forceflushLog();  // to ensure the log msg below is not beyond buffer
				logString("Static voltage dropping fast" , BUFFER, LOG_ALWAYS);
				voltageShutdown();
			} else if (voltageDropRateStatic >= tripRate * 0.8) {
				adjustVolume(-1,TRUE,FALSE);  // getting too close; lower volume
				playBip();
			} else			
				set_voltmaxvolume(FALSE);				
		}		
		tCur_1 = currentTimeInSec;
	}
	return v;
}

int keyCheck(int longCheck) {
	// get key press or use macro
	extern int GetCurKey();
	int i, keystroke;
	unsigned int curkey;
	static int keydown = 0;

	if(keydown) {	// we are waiting for key up or long timer to expire
		
		curkey = GetCurKey();
		if(curkey != keydown) {		// the key is up - return it
			*P_TimeBaseB_Ctrl = 0;    // stop timer
//			logKeystroke(keydown | 0x8000);
			curkey = keydown;
			keydown = 0;
			return curkey;
		} else if (keydown_counter > KEY_LONG_DOWN_THRESH) {
			*P_TimeBaseB_Ctrl = 0;    // stop timer
			curkey = keydown | LONG_KEY_STROKE;
			keydown = 0;
//			logKeystroke(curkey | 0xc000);
			logKeystroke(curkey);
			return (curkey);  // return long key stroke
		}
	}
	// loop allows time for service loop to stabilize on keys
	// based on C_DebounceCnt = 8 in IOKeyScan.asm (i < 12 was too short)	
	KeyScan_ServiceLoop();
	if (longCheck) {
		for (i = 0; i < 15; i++) {
			KeyScan_ServiceLoop();
		}
	}
		
	keystroke = (int)SP_GetCh();
	// BUG: Combo keys are not working. Not sure why yet.
//	if (keystroke == ADMIN_COMBO_KEYS)
//		adminOptions();  //might also want to move this to control tracks
	if (MACRO_FILE) {
		keystroke = nextMacroKey(keystroke);
		logKeystroke(keystroke);
		return(keystroke);
	}
	
// we have debounced key down, start timer and wait for key up or long key press time to expire
	if (keystroke) {
		
		__asm__("irq off");
		__asm__("fiq off");

		*P_TimeBaseB_Ctrl = TIMEBASE_B_16HZ;  // enable int & divide second into 16 chunks (could be 8,16,32,64)
		keydown_counter = 0;
		
		__asm__("irq on");
		__asm__("fiq on");
		
		keydown = keystroke;
	}
	if(longCheck && keydown) {
		while(((curkey = GetCurKey()) == keydown) && (keydown_counter <= KEY_LONG_DOWN_THRESH)) 
			;
		curkey = keydown;
		if (keydown_counter > KEY_LONG_DOWN_THRESH)
			curkey |= LONG_KEY_STROKE;
		keydown = 0;
		logKeystroke(curkey);
		return(curkey);
	}
	return (0);
//	return keystroke;
}

int waitForButton(int targetedButton) {
	int pressedButton;
	
	do 
		pressedButton = keyCheck(0);
	while (!(pressedButton > 0 && ((pressedButton == targetedButton) || (targetedButton == 0))));
	return pressedButton;
}

void wait(int t) { //t=time in msec
	unsigned int i;
	unsigned long j, k;
	unsigned long int cyclesPerMilliSecond = (long)(*P_PLLN & 0x3f) * 1000L;  // 96000 at 96MHz	
	const unsigned int cyclesPerNOP = 70; // cycles for each no-operation instruction
	const unsigned int cyclesPerVoltCheck = cyclesPerNOP * 300; // cycles for each no-operation instruction
//	const unsigned int NOPsPerMilliSecond = cyclesPerMilliSecond / cyclesPerNOP; // loop count per millisecond
	const unsigned int VChecksPerMilliSecond = cyclesPerMilliSecond / cyclesPerVoltCheck; // loop count per millisecond
	for (i = 0; i < t; i++) 
		for (j = 0; j < VChecksPerMilliSecond; j++) {
//			asm("nop\n");  // a CPU no-op instruction to pass the time
			if (!shuttingDown) {
				checkVoltage();
			} else {  // shutting down - don't check voltage - instead use NOPs
				for (k=0; k< (cyclesPerVoltCheck/cyclesPerNOP);k++) {
					asm("nop\n");		 				
				}
			}
		} 
}


int waitAndCheckKeys(int t) { //t=time in msec
	unsigned int i;
	unsigned long j;
	int key;
	unsigned long int cyclesPerMilliSecond = (long)(*P_PLLN & 0x3f) * 1000L;  // 96000 at 96MHz	
	const unsigned int cyclesPerKeyCheck = 400; // cycles for each no-operation instruction
	const unsigned int KeyChecksPerMilliSecond = cyclesPerMilliSecond / cyclesPerKeyCheck; // loop count per millisecond
	for (i = 0; i < t; i++) {
		for (j = 0; j < KeyChecksPerMilliSecond; j++) {
			key = keyCheck(0);
			if (key)
				break;
		}
		if (key)
			break;
	}
	return key;	
}

void resetSystem(void) {
	// set watchdog timer to reset device; 0x780A (Watchdog Reset Control Register)
	// see GPL Programmer's Manual (V1.0 Dec 20,2006), Section 3.5, page 18
	checkVoltage();  // USB may have been supplying sole power -- need to check if voltage dropping fast
	stop(); 
	if (PLEASE_WAIT_IDX && context.package) {  // prevents trying to insert this sound before config & control files are loaded.
		insertSound(&pkgSystem.files[PLEASE_WAIT_IDX],NULL,TRUE); 
	}
	checkVoltage();  // USB may have been supplying sole power -- need to check if voltage dropping fast
	saveVolumeProfile();
	logString((char *)"* RESET *",ASAP,LOG_ALWAYS);
	saveLogFile(0);	
	fs_safexit(); // should close all open files
	disk_safe_exit(0);
// try to get the sd card in a safe state - reverse what we do on startup		
	_deviceunmount(0);
	fs_uninit();
	SD_Uninitial();		
	turnSDoff();

	playBip();
	setLED(LED_ALL,FALSE);  

	*P_WatchDog_Ctrl &= ~0x4001; // clear bits 14 and 0 for resetting system and time=0.125 sec 	
	*P_WatchDog_Ctrl |= 0x8004; // set bits 2 and 15 for 0.125 sec, system reset, and enable watchdog
	while(1);	
}

static void logKeystroke(int intKey) {
	int i;
	char str[20];

	if (LOG_KEYS) {
		longToDecimalString(keydown_counter,str,4);
		if(intKey & 0x8000) {   // key up
			if(intKey & 0x4000) {
				strcat(str," LONG ");
			} else {
				strcat(str,"  UP  ");
			}
		} else {
			strcat(str," DOWN ");
		}
		intKey &= 0x7fff;
		strcat(str,", ");
		i = strlen(str)-1;
		if (intKey == KEY_LEFT) str[i] = TEXT_EVENT_LEFT;
			else if (intKey == KEY_RIGHT) str[i] = TEXT_EVENT_RIGHT;
			else if (intKey == KEY_UP) str[i] = TEXT_EVENT_UP;
			else if (intKey == KEY_DOWN) str[i] = TEXT_EVENT_DOWN;
			else if (intKey == KEY_SELECT) str[i] = TEXT_EVENT_SELECT;
			else if (intKey == KEY_HOME) str[i] = TEXT_EVENT_HOME;
			else if (intKey == KEY_PLAY) str[i] = TEXT_EVENT_PLAY;
			else if (intKey == KEY_STAR) str[i] = TEXT_EVENT_STAR;
			else if (intKey == KEY_PLUS) str[i] = TEXT_EVENT_PLUS;
			else if (intKey == KEY_MINUS) str[i] = TEXT_EVENT_MINUS;
		logString(str,ASAP,LOG_ALWAYS);	
	}
}

void housekeeping() {
		// give visual feedback of shutting down (aural feedback when user causes shutdown in takeAction())
	setLED(LED_ALL,TRUE);
	saveVolumeProfile();
	exportFlashStats();
	write_config_bin();  // build a config.bin
	//checkDoubleSRNprefix(); // this can be removed once the dup serial number prefixes are fixed
	//confirmSNonDisk(); // make sure the serial number file is correct 
	buildMyStatsCSV();
	buildExchgOstats();
	clearDeleteQueue();
	saveLogFile(0);	
}

static void voltageShutdown() {
	SACM_Volume(1);
	playBip();
	fastShutdown();
}

void fastShutdown() {
	if (shuttingDown)
		return;
	setLED(LED_ALL,TRUE);
	shuttingDown = 1;
	logString("SHUTTING DOWN" , BUFFER, LOG_ALWAYS);
	forceflushLog();
	//saveLogFile(0);	
	//write_config_bin();  // build a config.bin
	shutdown();
	SysIntoHaltMode();
	while(1);	
}
		
void shutdown() {
	shuttingDown = 1; // prevents the call to wait() below from checking voltage
	*P_Clock_Ctrl |= 0x200;	//bit 9 KCEN enable IOB0-IOB2 key change interrupt		
	disk_safe_exit(0);
// try to get the sd card in a safe state - reverse what we do on startup		
	_deviceunmount(0);
	fs_uninit();
	SD_Uninitial();		
	turnSDoff();
	SACM_Volume(1);
	playDing();
	turnAmpOff();
	setLED(LED_ALL,FALSE);
	setLED(LED_RED,TRUE);
	wait(150);
	setLED(LED_RED,FALSE);
	setLED(LED_GREEN,TRUE);
	wait(50);
	setLED(LED_ALL,FALSE);
	turnNORoff();
}

void setOperationalMode(int newmode) {
	extern void buildMyStatsCSV();
	
  if(newmode == (int)P_WAIT) {
  	// stop();  --- should we see if we can WAIT while paused in an audio file?
  	SysIntoWaitMode();
    // when leaving wait mode, next instruction is executed, so we return here
    return;
  } else {
    	// assume calling for sleep or halt
		if (newmode == (int)P_HALT)
			logString((char *)"Halting",BUFFER,LOG_NORMAL);
		else // newmode == (int)P_SLEEP
			logString((char *)"Sleeping",BUFFER,LOG_NORMAL);			
		housekeeping();
		shutdown();		
	  	if (newmode == (int)P_HALT)  {
		  	SysIntoHaltMode();
	  	}
		else { // newmode == (int)P_SLEEP
			_SystemOnOff();
		}
		while(1);	
	    // cpu reset on exiting halt/sleep mode, so nothing below here executes
     }
}

int logLongHex(unsigned long data) {
	char strHex[7];
	int ret;

	longToHexString((long)data, (char *)strHex, 1);
	//bytesToWrite = convertDoubleToSingleChar(strHex,strHex,TRUE);
	//ret = write(handle,strHex<<1,bytesToWrite);
	ret=appendStringToFile((const char *)"data.txt",(char *)strHex);	
	return ret;
}


void
refuse_lowvoltage(int die)
{
	extern void playBip(void);
	int no_startup_done = ((LED_GREEN == 0) && (LED_RED == 0));
	if(no_startup_done == 1) { // haven't run startup, no sound possible
		LED_GREEN = DEFAULT_LED_GREEN;
		LED_RED = DEFAULT_LED_RED;
		LED_ALL = LED_GREEN | LED_RED;
	} else {	
		playBips(2);
	}
	if(die) {
		setLED(LED_GREEN, FALSE);
		setLED(LED_RED, TRUE);
		wait(200);
		setLED(LED_RED, FALSE);
		wait(200);
		setLED(LED_RED, TRUE);
		wait(200);
		voltageShutdown();
	}
}


void
turnAmpOff(void) {
	*P_IOA_Dir  |= 0x0800;
	*P_IOA_Attrib |= 0x0800; 	
	*P_IOA_Buffer  &= ~0x0800;    	
}

void
turnAmpOn(void) {
	*P_IOA_Dir  |= 0x0800;
	*P_IOA_Attrib |= 0x0800; 	
	*P_IOA_Buffer  |= 0x0800;    	
}

static void 
turnSDoff(void) {
	//disable SD card
 	*P_IOA_Dir  |= 0x1000;
 	*P_IOA_Attrib |= 0x1000; 	
    *P_IOA_Buffer  |= 0x1000;	
}

extern void 
turnNORoff(void) {
	// disable NOR flash
 	*P_IOD_Dir  |= 0x0001;	 
 	*P_IOD_Attrib |= 0x0001;
    *P_IOD_Buffer  |= 0x0001;	
}

int
SNexists(void) {
	int snCode;
	
	snCode = *P_TB_SERIAL_PREFIX;
	if (!strncmp(P_TB_SERIAL_PREFIX,CONST_TB_SERIAL_PREFIX,CONST_TB_SERIAL_PREFIX_LEN))
		return TRUE;
	else
		return FALSE;
}

char *
getDeviceSN(void) {
	char *ret;
	
	ret = getSerialNumber();
	return ret;
}

void writeVersionToDisk(char *path) {
	char fileVersion [PATH_LENGTH];
	int handle, ret;
	struct f_info file_info;
		
	strcpy(fileVersion,path);
	strcat(fileVersion,SVN_REVISION  FILE_REVISION_EXT);
	if (!fileExists((LPSTR)fileVersion)) {
		logStringRTCOptional((char *)"Replacing missing version file.",ASAP,LOG_NORMAL,0); 
		tbChdir((LPSTR)path);
		ret =_findfirst((LPSTR)"*" FILE_REVISION_EXT, &file_info, D_FILE);
		while (ret >= 0) {
			ret = unlink((LPSTR)file_info.f_name);		
			ret = _findnext(&file_info);
		}	
		mkdir((LPSTR)path); 
		handle = tbOpen((LPSTR)fileVersion,O_CREAT|O_RDWR|O_TRUNC);
		close(handle);	
	}
}

unsigned long
setRTCalarmSeconds(unsigned long seconds) {
	unsigned int h = *P_Hour;
	unsigned int m = *P_Minute;
	unsigned int s = *P_Second;
	unsigned int delta_min, delta_sec, delta_hour;
	
	delta_sec = (seconds + s) % 60;
	delta_min = (seconds + s) / 60;

	delta_min += m;
	delta_hour = delta_min / 60;
	delta_min = delta_min % 60;
	
	delta_hour += h;
	
	return(addAlarm(delta_hour, delta_min, delta_sec));
}
unsigned long
setRTCalarmMinutes(unsigned int minutes) {
	unsigned int h = *P_Hour;
	unsigned int m = *P_Minute;
	unsigned int delta_min, delta_hour;

	delta_min = minutes + m;
	delta_hour = delta_min / 60;
	delta_min = delta_min % 60;

	delta_hour += h;

	return(addAlarm(delta_hour, delta_min, *P_Second));
}
unsigned long
setRTCalarmHours(unsigned int hours) {
	return(addAlarm(*P_Hour + hours, *P_Minute, *P_Second));
}

void
setRTCalarm(unsigned int hour, unsigned int minute, unsigned int second) {
	char msg[32] = "set ALM ";
	
	*P_Alarm_Second = second;
	*P_Alarm_Minute = minute;
	*P_Alarm_Hour   = hour;
	
	*P_RTC_INT_Ctrl |= RTC_ALARM_INTERRUPT_ENABLE;
	*P_RTC_Ctrl     |= (RTC_MODULE_ENABLE | RTC_ALARM_FUNCTION_ENABLE);
	
	longToDecimalString(hour,msg+8,4);
	msg[12] = 'h';
	longToDecimalString(minute,msg+13,2);
	msg[15] = 'm';
	longToDecimalString(second,msg+16,2);
	msg[18] = 's';
	msg[19] = 0;
	logString(msg,ASAP,LOG_DETAIL);
}

/*  called from isr.asm when RTC alarm has fired
    running in interrupt service, so should minimize work done and return
*/
void
RTC_Alarm_Fired() {
	
	int wrk = *P_RTC_INT_Status;
	
	rtc_fired = getRTCinSeconds();
	
	if(rtc_fired == 0) {
		rtc_fired = 0xff000000;
	}
	*P_RTC_INT_Status |= wrk;	// clear all interrupt flags
//	rtcAlarmFired();
	
}
void rtcAlarmFired(unsigned long alarm) {
	char buffer[48], wrk[12];
	void setNextAlarm();
	void removeAlarm(unsigned long);
	APP_IRAM static unsigned long lastActivity;
	
	strcpy(buffer,"rtcAlarmFired() alarm=");
	unsignedlongToHexString((long)alarm, (char *)wrk);
	strcat(buffer, wrk);
	logString(buffer,ASAP,LOG_ALWAYS);
	
	removeAlarm(alarm);	// remove from table if present

	if((alarm & 0x00ffffffL) == 0) {
//	if(*P_Hour == 0 && *P_Minute == 0) { // bump systemcounters 
		incrementCumulativeDays();
//		loadSystemCounts();
		//TODO:Redundant with Flash system counter
//		systemCounts.poweredDays += 1;
//		systemCounts.monthday++;
//		fixBadDate(&systemCounts);
//		saveSystemCounts();
//		logRTC(); // remove
		while(*P_Second == 0) wait(100);
		strcpy(buffer,"poweredDays=");
		longToDecimalString(getCumulativeDays(), buffer+12, 4);
		logString(buffer,ASAP,LOG_ALWAYS);
		lastActivity = 0;  // if up when rollover prevent shutdown
//		resetRTC();
//		resetRTC23();  // test, really set rtc to 0,0,1sec
//		setRTCalarm(0, 0, 0);
	}
	setNextAlarm();
}
void removeAlarm(unsigned long al)
{
	int i, j;
	
	if(al & 0x00ffffffL) {
		for(i=0; i<N_RTC_ALARMS; i++) {  // find alarm
			if(rtcAlarm[i] == al) {
				rtcAlarm[i] = 0L;
				break;
			}
		}
		if(i < (N_RTC_ALARMS+1)) {
			for(j=i+1; j<N_RTC_ALARMS; i++,j++) { // pack the table
				rtcAlarm[i] = rtcAlarm[j];
			}
			rtcAlarm[N_RTC_ALARMS-1] = 0L;
		}
	}
	curAlarmSet = 0;
}
long getTimeinSeconds(unsigned int hour, unsigned int minute, unsigned int second) {
	unsigned long ret, secH;
	unsigned int secM;
	
	secM = (unsigned int)minute * 60;
	secH = (unsigned long)hour * 3600;
	ret = second + secM + secH;
	return (long)ret;
}
unsigned long
addAlarm(unsigned int hour, unsigned int minute, unsigned int second) {
	void setNextAlarm();
	char buf[48], strbuf[12];
	int i, insertpos;
	unsigned long newAlarm;
	
	while(hour >= 24)
		hour -= 24;
	while(minute >= 60)
		minute -= 60;
	while(second >= 60)
		second -= 60;
	
	newAlarm = getTimeinSeconds(hour, minute, second);
	
	for(i=0; i<N_RTC_ALARMS; i++) {
		if((rtcAlarm[i] == 0) || (newAlarm < rtcAlarm[i])) 
			break;
		if(newAlarm && (newAlarm == rtcAlarm[i])) {
			return(newAlarm);
		}
	}
	insertpos = i;
	if(rtcAlarm[insertpos] != 0) {
		for(i=N_RTC_ALARMS-1; i>=insertpos; i--) {
			rtcAlarm[i+1] = rtcAlarm[i];
		}
	}
	rtcAlarm[insertpos] = newAlarm;
	
	strcpy(buf,"add new alarm at position ");
	longToDecimalString(insertpos, (char *)strbuf, 2); 
	strcat(buf, strbuf);
	strcat(buf," alarm time ");
	unsignedlongToHexString((unsigned long)newAlarm, (char *)strbuf);
	strcat(buf, strbuf);
	logString(buf,BUFFER,LOG_NORMAL); 
	
	
	setNextAlarm();
	
	return(newAlarm);
}
void logAlarms(char *preface) {
	char buf[64], strbuf[12];
	int i;
	
	for(i=0; i<N_RTC_ALARMS; i++) {
		strcpy(buf, preface);
		strcat(buf, " pos=");
		longToDecimalString(i, (char *)strbuf, 2); 
		strcat(buf, strbuf);
		strcat(buf," alarm time ");
		unsignedlongToHexString((unsigned long)rtcAlarm[i], (char *)strbuf);
		strcat(buf, strbuf);
		logString(buf,BUFFER,LOG_NORMAL); 
	}	
}
void setNextAlarm() {
	unsigned int i, hour, minute, second;
	unsigned long newAlarm;
	unsigned long curRTC   = getRTCinSeconds();
	char buf[64], strbuf[12];
	
	curAlarmSet = 0;

	for(i=0; i<N_RTC_ALARMS; i++) {
		if(rtcAlarm[i] > curRTC) {
			newAlarm = rtcAlarm[i];
			hour = newAlarm / 3600;
			while(hour >= 24)
				hour = hour - 24;
			minute = (newAlarm % 3600) / 60;
			while(minute >= 60)
				minute = minute - 60;
			second = (newAlarm % 3600) % 60;
			
			setRTCalarm(hour, minute, second);
			newAlarm = (long)((long)hour * 3600L) + (long)((long)minute * 60L) + (long)second;
				
			strcpy(buf,"setRTCalarm to ");
			unsignedlongToHexString((unsigned long)newAlarm, (char *)strbuf);
			strcat(buf, strbuf);
			logString(buf,BUFFER,LOG_DETAIL);
			 
			strcpy(buf,"   RTC = ");
			unsignedlongToHexString(curRTC, (char *)strbuf);
			strcat(buf, strbuf);
			logString(buf,BUFFER,LOG_DETAIL); 
		
			curAlarmSet = newAlarm;
			break;	
		}
	}

	if(curAlarmSet == 0 ) {
		logString("setRTCalarm(0,0,0)",BUFFER,LOG_DETAIL); 
		while(*P_Hour == 0 && *P_Minute == 0 && *P_Second == 0);
		setRTCalarm(0, 0, 0);
// test		setPlus10();	
	}
	
}

void resetRTC23(void) {
	APP_IRAM static unsigned long lastActivity;
	int wrk;
#define		P_RTC_HMSBusy                 (volatile unsigned int*)(P_RTC_Ctrl_Base+0x17)
	wrk = *P_RTC_INT_Status;
	*P_RTC_INT_Status |= wrk;	// clear all interrupt flags
	
	while (*P_RTC_HMSBusy) ; // wait till RTC is not busy 
	*P_Second = 15;
	while (*P_RTC_HMSBusy) ; // wait till RTC is not busy 
	*P_Minute = 54;
	while (*P_RTC_HMSBusy) ; // wait till RTC is not busy 
	*P_Hour = 23;
	lastActivity = getRTCinSeconds();
	
//	setRTCalarm(0,0,0);

}


void
setPlus(int i) {
	unsigned int sec, min, hr;
	
	sec = *P_Second;
	min = *P_Minute;
	hr  = *P_Hour;
	min += i;
	if(min >= 60) {
		min = min - 60;
		hr++;
	}
	if(hr >= 24) {
		hr = 0;
		min = 0;
		sec = 2;
	}
	addAlarm(hr, min, sec);

}

void
KEY_TimeBase_B_isr() {	//TimerBase B fired
	unsigned int i;
	i = *P_TimeBaseB_Ctrl;
	*P_TimeBaseB_Ctrl = i;	// reset interrupt bit
	keydown_counter += 1;
}

extern void confirmSNonDisk(void) {
	int exists, handle, ret;
	char fileSN[PATH_LENGTH],filePattern[PATH_LENGTH];
	char sysPath[PATH_LENGTH];	
	struct f_info file_info;
	
//	if (!SNexists())
//		return; // no serial number - don't write to disk
	if (SYSTEM_PATH)
		strcpy(sysPath,SYSTEM_PATH);
	else 
		strcpy(sysPath,DEFAULT_SYSTEM_PATH);
	strcpy(fileSN,sysPath);
	strcat(fileSN, (char *)getSerialNumber());
	strcat(fileSN, (char *)SERIAL_EXT);
	exists = fileExists((LPSTR) fileSN);
	if (!exists) {
		mkdir((LPSTR)sysPath);
		tbChdir((LPSTR)sysPath);
		strcpy(filePattern,(char *)"*" SERIAL_EXT);
		ret =_findfirst((LPSTR)filePattern, &file_info, D_FILE);
		while (ret >= 0) { 			
			ret = unlink((LPSTR)file_info.f_name);
			ret = _findnext(&file_info);
		}
		handle = tbOpen((LPSTR)fileSN,O_CREAT|O_RDWR|O_TRUNC);
		close(handle);
	}
}

extern void confirmLocationonDisk(void) {
	int exists, handle, ret;
	char fileLoc[PATH_LENGTH],filePattern[PATH_LENGTH];
	char sysPath[PATH_LENGTH];	
	struct f_info file_info;
	
//	if (!SNexists())
//		return; // no serial number - don't write to disk
	if (SYSTEM_PATH)
		strcpy(sysPath,SYSTEM_PATH);
	else 
		strcpy(sysPath,DEFAULT_SYSTEM_PATH);
	strcpy(fileLoc,sysPath);
	strcat(fileLoc, (char *)getLocation());
	strcat(fileLoc, (char *)LOCATION_FILE_EXTENSION);
	exists = fileExists((LPSTR) fileLoc);
	if (!exists) {
		mkdir((LPSTR)sysPath);
		tbChdir((LPSTR)sysPath);
		strcpy(filePattern,(char *)DEFAULT_SYSTEM_PATH);
		strcat(filePattern,(char *)LOCATION_FILE_PATTERN);
		ret =_findfirst((LPSTR)filePattern, &file_info, D_FILE);
		while (ret >= 0) { 			
			ret = unlink((LPSTR)file_info.f_name);
			ret = _findnext(&file_info);
		}
		handle = tbOpen((LPSTR)fileLoc,O_CREAT|O_RDWR|O_TRUNC);
		close(handle);
	}
}

extern void confirmPackageNameonDisk(void) {
	int exists, handle, ret;
	char filePkg[PATH_LENGTH],filePattern[PATH_LENGTH];
	char sysPath[PATH_LENGTH];	
	struct f_info file_info;
	
//	if (!SNexists())
//		return; // no serial number - don't write to disk
	if (SYSTEM_PATH)
		strcpy(sysPath,SYSTEM_PATH);
	else 
		strcpy(sysPath,DEFAULT_SYSTEM_PATH);
	strcpy(filePkg,sysPath);
	strcat(filePkg, (char *)getPackageName());
	strcat(filePkg, (char *)PACKAGE_EXT);
	exists = fileExists((LPSTR) filePkg);
	if (!exists) {
		mkdir((LPSTR)sysPath);
		tbChdir((LPSTR)sysPath);
		strcpy(filePattern,(char *)"*" PACKAGE_EXT);
		ret =_findfirst((LPSTR)filePattern, &file_info, D_FILE);
		while (ret >= 0) { 			
			ret = unlink((LPSTR)file_info.f_name);
			ret = _findnext(&file_info);
		}
		handle = tbOpen((LPSTR)filePkg,O_CREAT|O_RDWR|O_TRUNC);
		close(handle);
	}
}

extern
void alertCorruption(void) {
	playBips(3);
}
