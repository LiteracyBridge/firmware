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
static void turnNORoff(void);

APP_IRAM static int volume, speed;
APP_IRAM static unsigned long voltage; // voltage sample 
APP_IRAM static int oldVolume;
extern APP_IRAM int vThresh_1;
extern APP_IRAM unsigned int vCur_1;
APP_IRAM static unsigned int keydown_counter;

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
	saveSystemCounts();	
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

extern void getRTC(char * str) {
	unsigned long c,p,d,h,m,s;
	char time[25];
	
	c = (unsigned long)systemCounts.powerUpNumber;
	p = (unsigned long)CLOCK_PERIOD;  
	d = (unsigned long)systemCounts.poweredDays;
	h = (unsigned long) *P_Hour;	
	m = (unsigned long) *P_Minute;
	s = (unsigned long) *P_Second;

	if (c) {
		longToDecimalString(c,time,4);
		time[4] = 'c';
		longToDecimalString(p,time+5,3);
		time[8] = 'p';
		longToDecimalString(d,time+9,3);
		time[12] = 'd';
	} else 
		strcpy(time,"----c---p---d");	// if cold-start and sys-vars has not been loaded yet
	longToDecimalString(h,time+13,2);
	time[15] = 'h';
	longToDecimalString(m,time+16,2);
	time[18] = 'm';
	longToDecimalString(s,time+19,2);
	time[21] = 's';
	time[22] = 0;
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

int restoreVolume(BOOL normalVolume) {
	int ret;
	
	if (normalVolume)
		ret = adjustVolume(NORMAL_VOLUME,FALSE,FALSE);
	else
		ret = adjustVolume(oldVolume,FALSE,FALSE);
	return ret;
}

int adjustVolume (int amount, BOOL relative, BOOL rememberOldVolume) {
/*
	APP_IRAM static long timeLastVolChg = 0;
	long timeCurrent,diff;
	char stringLog[30];
	
	timeCurrent = getRTCinSeconds();
	diff = timeCurrent - timeLastVolChg;
	timeLastVolChg = timeCurrent;
	
	strcat(stringLog,"V:");
	longToDecimalString((long)volume,stringLog+3,5);
	strcat(stringLog,",");
	longToDecimalString(diff,stringLog+strlen(stringLog),5);
	strcat(stringLog,"sec   ");
	if (relative && amount > 0)
		strcat(stringLog,"+");
	else if (relative && amount < 0)
		strcat(stringLog,"-");
	logString(stringLog,BUFFER);
*/		
	if (rememberOldVolume)
		oldVolume = volume;
	
	if (relative)
		volume += amount;
	else
		volume = amount;
	
	set_voltmaxvolume(FALSE);

	if (volume > MAX_VOLUME) { 
		volume = MAX_VOLUME;
		playBip();	
	}
	if (volume < MIN_VOLUME)  
		volume = MIN_VOLUME;
	SACM_Volume(volume);	
	return volume;
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

void setUSBDevice (BOOL set) {		
	if (set) {
		Snd_Stop();
		flushLog();
		//cleanUpOldRevs(); // cleanup any old revs
		SystemIntoUDisk(USB_CLIENT_SVC_LOOP_CONTINUOUS);	
		SD_Initial();  // recordings are bad after USB device connection without this line (todo: figure out why)
		processInbox();
	}
}

void logVoltage() {
	int i;
	unsigned int sample;
	char buffer[40];
	unsigned long time = getRTCinSeconds();
	APP_IRAM static unsigned long timeLastSample = -1;
	
//	if ((context.isStopped || context.isPaused) && (time > (timeInitialized + 2)))     // 2-3 second delay)
	if (VOLTAGE_SAMPLE_FREQ_SEC && (context.isStopped || context.isPaused) && time > (timeLastSample+VOLTAGE_SAMPLE_FREQ_SEC)) { 	
		sample = getCurVoltageSample();
		strcpy(buffer,"V:");
	 	longToHexString(((long)vThresh_1 & 0xffff),buffer+strlen(buffer),1);
	 	strcat(buffer," | ");
	 	
		if(sample == 0xffff) {
			sample = getCurVoltageSample();
			if(sample == 0xffff)
				return;
		}
		
		longToDecimalString((long)sample,buffer+strlen(buffer),3);
		i = strlen(buffer);
		buffer[i+1] = 0;
		buffer[i] = buffer[i-1];
		buffer[i-1] = buffer[i-2];
		buffer[i-2] = '.'; 
		logString(buffer,ASAP,LOG_DETAIL);				
	 	voltage = sample;			 		
		timeLastSample = time;
	}
}

unsigned int
getCurVoltageSample() {	
	unsigned ret = 0xffff;
	APP_IRAM static BOOL wasSampleStarted = FALSE;
	unsigned long currentTimeInSec;
	unsigned int voltageDropTime;
	APP_IRAM static unsigned long timeLastVoltageMilestone = 0;
	char log[80];
	
	if (!wasSampleStarted) {
		*P_ADC_Setup |= 0x4000;
		*P_MADC_Ctrl |= 0x40; // set STRCNV, starting the voltage sample
		wasSampleStarted = TRUE;
	} else if (*P_MADC_Ctrl & 0x80) {  // checks CNVRDY (sample is ready)					
	 	ret = (unsigned int)*P_MADC_Data;
	 	
//rhm			 	sample >>= 4; // only bits 4-15
//rhm				f = 2 * 3.3 * (sample / (float)0x0fff); // ref = 3.3v; LINE1 measures 1/2 voltage
//rhm				sample = (unsigned int) (f * 100); // to give "x.xx" for total voltage

		ret /= 99;  //RHM heuristically determined to work on my board, replaced 3 lines above
		
		if (ret < V_MIN_POSSIBLE_VOLTAGE) // must be powered externally 
			ret = V_NORMAL_VOLTAGE; // set ret to ensure all voltage checks pass

	 	*P_ADC_Setup &= ~0x4000; // disable ADCEN to save power
		wasSampleStarted = FALSE;
		vThresh_1 <<= 1;
		if(ret < vCur_1)	// if voltage < vCur_1 set a bit in vThresh_1, all ones in vThresh means 16 samples in a row below 
			vThresh_1 |= 1;
		if(vThresh_1 == 0xffff) {
			unsigned int delta_voltage = vCur_1 - ret;
//			--vCur_1;	// drop current nominal voltage
			vCur_1 = ret;
			vThresh_1 = 0;	// reset threshold bits
			currentTimeInSec = getRTCinSeconds();
			if(delta_voltage >= V_VOLTAGE_DROP_CHECK_INTERVAL) {
//			if (!(vCur_1 % V_VOLTAGE_DROP_CHECK_INTERVAL)) {
				if (timeLastVoltageMilestone) {
					unsigned int voltageDropRate;
					voltageDropTime = currentTimeInSec - timeLastVoltageMilestone; 
					strcpy(log, (char *) "VOLTAGE DROP RATE: 0."); 
					longToDecimalString((long)delta_voltage,log+strlen(log),2);
					strcat(log,(char *) "v in ");
					longToDecimalString((long)voltageDropTime,log+strlen(log),4);
					strcat(log, (char *) " sec");
					logString(log,BUFFER, LOG_ALWAYS);
					
					if(voltageDropTime > 0) {  // don't divide by 0
						voltageDropRate = (delta_voltage * 10) / voltageDropTime;
						strcpy(log, (char *) "VOLTAGE DROP RATE: "); 
						longToDecimalString((long)voltageDropRate, log+strlen(log), 3);
						logString(log,BUFFER, LOG_ALWAYS);
					
						if((voltageDropRate > 25) && (vCur_1 < 250)) {  //heuristic constants, may need to change
							logString("VOLTAGE DROPPING FAST - SHUTTING DOWN" , ASAP, LOG_ALWAYS);
							flushLog();
							refuse_lowvoltage(1);
						}
					}
				
					if (voltageDropTime < V_FAST_VOLTAGE_DROP_TIME_SEC && vCur_1 < V_MIN_VOL_VOLTAGE) {
						// Maybe try an adjustment like the one below, but that might be too aggressive
						// vFast_Voltage_Drop_Time_Sec = voltageDropTime;
						set_voltmaxvolume(TRUE);						
					} 
				}  
				timeLastVoltageMilestone = currentTimeInSec;
			}


			if (1) {  // (DEBUG_MODE) {
				longToDecimalString(currentTimeInSec,log,5);
				strcat(log,(char *)": v");
				longToDecimalString(vCur_1, log+strlen(log), 3);
				logString(log,BUFFER,LOG_NORMAL);
			}
			if(vCur_1 < V_MIN_RUN_VOLTAGE) {
				refuse_lowvoltage(1);
			} else if (vCur_1 <= V_MIN_SDWRITE_VOLTAGE) {
				Snd_Stop(); // in case running audio causes a problem with logging
				strcpy(log,(char *)"Low voltage->Logging terminated.");
				logString(log,ASAP, LOG_ALWAYS);
			} 
		}
	}
	return(ret);
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
	unsigned long j;
	unsigned long int cyclesPerMilliSecond = (long)(*P_PLLN & 0x3f) * 1000L;  // 96000 at 96MHz	
	const unsigned int cyclesPerNOP = 70; // cycles for each no-operation instruction
	const unsigned int NOPsPerMilliSecond = cyclesPerMilliSecond / cyclesPerNOP; // loop count per millisecond
	for (i = 0; i < t; i++) 
		for (j = 0; j < NOPsPerMilliSecond; j++)  
			asm("nop\n");  // a CPU no-op instruction to pass the time
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
	stop(); 
	if (PLEASE_WAIT_IDX && context.package) {  // prevents trying to insert this sound before config & control files are loaded.
		insertSound(&pkgSystem.files[PLEASE_WAIT_IDX],NULL,TRUE); 
	}
	playBip();
	setLED(LED_ALL,FALSE);  
	logString((char *)"* RESET *",ASAP,LOG_ALWAYS);
	fs_safexit(); // should close all open files
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

void setOperationalMode(int newmode) {
	extern void buildMyStatsCSV();
	extern void saveLogFile(int);
	
  if(newmode == (int)P_WAIT) {
  	// stop();  --- should we see if we can WAIT while paused in an audio file?
  	SysIntoWaitMode();
    // when leaving wait mode, next instruction is executed, so we return here
    return;
  } else {
		// give visual feedback of shutting down (aural feedback when user causes shutdown in takeAction())
 		setLED(LED_ALL,TRUE);
  		buildMyStatsCSV();
		buildExchgOstats();
 		clearDeleteQueue();
  		write_config_bin();  // build a config.bin
		writeVersionToDisk(SYSTEM_PATH);  // make sure the version file is correct
		checkDoubleSRNprefix(); // this can be removed once the dup serial number prefixes are fixed
  		confirmSNonDisk(); // make sure the serial number file is correct 
    	// assume calling for sleep or halt
		*P_Clock_Ctrl |= 0x200;	//bit 9 KCEN enable IOB0-IOB2 key change interrupt
		if (newmode == (int)P_HALT)
			logString((char *)"Halting",BUFFER,LOG_NORMAL);
		else // newmode == (int)P_SLEEP
			logString((char *)"Sleeping",BUFFER,LOG_NORMAL);			
		
		saveLogFile(0);	
		
	  	Snd_Stop();    // no logging
		setLED(LED_ALL,FALSE);
	
		turnAmpOff();

		disk_safe_exit(0);
// try to get the sd card in a safe state - reversw what we do on startup		
		_deviceunmount(0);
		fs_uninit();
		SD_Uninitial();		

		turnSDoff();
		turnNORoff();
	  	
	  	if (newmode == (int)P_HALT)  {
/*
	  		setRTCalarmSeconds(61);		// device should come back on in 61 seconds
*/
		  	SysIntoHaltMode();
	  	}
		else { // newmode == (int)P_SLEEP
			disk_safe_exit(0);

			_SystemOnOff();
		}
	
		while(1);	
	    // cpu reset on exiting halt/sleep mode, so nothing below here executes
     }
}

/*
void
Log_ClockCtrl() {
	char buffer[80];
 	unsigned int r1 = (unsigned int)*P_Clock_Ctrl;
 	strcpy(buffer, "P_Clock_Ctrl = 0x");
	longToHexString((long)r1,buffer+strlen(buffer),1);
	logString(buffer,ASAP);
	
 	strcpy(buffer, "P_IOB_Buffer = 0x");
 	r1 = *P_IOB_Buffer;
	longToHexString((long)r1,buffer+strlen(buffer),1);
	logString(buffer,ASAP);
}
*/

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
		playBip();
		playBip();
	}
	if(die != 0) {
		setLED(LED_ALL, FALSE);
		wait(500);
		setLED(LED_RED, TRUE);
		wait(500);
		setLED(LED_RED, FALSE);
		wait(500);
		setLED(LED_RED, TRUE);
		wait(500);
		setLED(LED_RED, FALSE);
		setOperationalMode((int)P_SLEEP);
	}
}

void
set_voltmaxvolume(BOOL forceLower)
{
	int wrk = V_MIN_VOL_VOLTAGE - vCur_1;
	
	if (wrk <= 0 || forceLower) {
		if (forceLower)
			wrk = MAX_VOLUME - 1;			
		else {
			wrk >>= 3;
			// for every .08 volt below V_MIN_VOL_VOLTAGE subtract 1 from MAX_VOLUME
			wrk = 16 - wrk;
		}
		if(wrk < 1) wrk = 1;
		if(wrk < MAX_VOLUME) {
			MAX_VOLUME = wrk;
			if (volume > MAX_VOLUME) {
				volume = MAX_VOLUME;
				SACM_Volume(volume);
				playBip();	
			}
			if (TRUE) { // logging voltage for all devices in the field (DEBUG_MODE) {
				char log[15] = "v---,MV--,CV--";
				longToDecimalString(vCur_1, log+1, 3);
				log[4] = ',';
				longToDecimalString((long)MAX_VOLUME,log+7,2);
				log[9] = ',';
				longToDecimalString((long)volume,log+11,2);
				logString(log,BUFFER,LOG_NORMAL);
			}		
		}
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

static void 
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
getDeviceSN(int includePrefix) {
	if (strncmp(CONST_TB_SERIAL_PREFIX,P_TB_SERIAL_PREFIX,strlen(CONST_TB_SERIAL_PREFIX)))
		return (char *)0;
	if (includePrefix)
		return P_TB_SERIAL_PREFIX;
	else
		return P_TB_SERIAL_NUMBER;	
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
		loadSystemCounts();
		systemCounts.poweredDays += 1;
		systemCounts.monthday++;
		fixBadDate(&systemCounts);
		saveSystemCounts();
//		logRTC(); // remove
		while(*P_Second == 0) wait(100);
		strcpy(buffer,"poweredDays=");
		longToDecimalString(systemCounts.poweredDays, buffer+12, 4);
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
			logString(buf,BUFFER,LOG_NORMAL);
			 
			strcpy(buf,"   RTC = ");
			unsignedlongToHexString(curRTC, (char *)strbuf);
			strcat(buf, strbuf);
			logString(buf,BUFFER,LOG_NORMAL); 
		
			curAlarmSet = newAlarm;
			break;	
		}
	}

	if(curAlarmSet == 0 ) {
		logString("setRTCalarm(0,0,0)",BUFFER,LOG_NORMAL); 
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
	
	if (SYSTEM_PATH)
		strcpy(sysPath,SYSTEM_PATH);
	else 
		strcpy(sysPath,DEFAULT_SYSTEM_PATH);
	strcpy(fileSN,sysPath);
	strcat(fileSN, (char *)TB_SERIAL_NUMBER_ADDR + CONST_TB_SERIAL_PREFIX_LEN);
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
