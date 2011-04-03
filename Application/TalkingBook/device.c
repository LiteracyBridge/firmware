// Copyright 2009-2011 Literacy Bridge
// Contact: info@literacybridge.org
#include "./system/include/system_head.h"
#include "Include/talkingbook.h"
#include "Include/files.h"
#include "Include/macro.h"
#include "Include/containers.h"
#include "Include/Inbox.h"
#include "Include/audio.h"
#include "Include/device.h"
#include "Include/startup.h"

extern void _SystemOnOff(void);
extern int SystemIntoUDisk(unsigned int);
extern void KeyScan_ServiceLoop(void);
extern int SP_GetCh(void);
extern INT16 SD_Initial(void);

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
void set_voltmaxvolume();


void resetRTC(void) {
#define		P_RTC_HMSBusy                 (volatile unsigned int*)(P_RTC_Ctrl_Base+0x17)
	while (*P_RTC_HMSBusy) ; // wait till RTC is not busy 
	*P_Second = 0;
	while (*P_RTC_HMSBusy) ; // wait till RTC is not busy 
	*P_Minute = 0;
	while (*P_RTC_HMSBusy) ; // wait till RTC is not busy 
	*P_Hour = 0;	
}

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
	logString(time,ASAP);
}


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
	
	set_voltmaxvolume();

	if (volume > MAX_VOLUME) { 
		volume = MAX_VOLUME;
		playBip();	
	}
	if (volume < 1)  
		volume = 1;
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
		cleanUpOldRevs(); // cleanup any old revs
		SystemIntoUDisk(USB_CLIENT_SVC_LOOP_CONTINUOUS);	
		SD_Initial();  // recordings are bad after USB device connection without this line (todo: figure out why)
		processInbox();
		setLED(LED_ALL,FALSE);
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
		logString(buffer,ASAP);				
	 	voltage = sample;			 		
		timeLastSample = time;
	}
}

unsigned int
getCurVoltageSample() {	
	unsigned ret = 0xffff;
	APP_IRAM static BOOL wasSampleStarted = FALSE;
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
			--vCur_1;	// drop current nominal voltage
			vThresh_1 = 0;	// reset threshold bits
			if (DEBUG_MODE) {
				log[0]='v';
				longToDecimalString(vCur_1, log+1, 3);
				logString(log,BUFFER);
			}
			if(vCur_1 < V_MIN_RUN_VOLTAGE) {
				refuse_lowvoltage(1);
			} else if (vCur_1 == V_MIN_SDWRITE_VOLTAGE) {
				stop(); // in case running audio causes a problem with logging
				strcpy(log,(char *)"Low voltage->Logging terminated.");
				logString(log,ASAP);
			} 
		}
	}
	return(ret);
}

int keyCheck(int longCheck) {
	// get key press or use macro
	int i, keystroke;
	static int keydown = 0;
	
	if(keydown) {	// we are waiting for key up or long timer to expire
		extern int GetCurKey();
		int curkey;
		curkey = GetCurKey();
		if(curkey != keydown) {		// the key is up - return it
			*P_TimeBaseB_Ctrl = 0;    // stop timer
//			logKeystroke(keydown | 0x8000);
//			logRTC();
			curkey = keydown;
			keydown = 0;
			return curkey;
		} else if (keydown_counter > KEY_LONG_DOWN_THRESH) {
			*P_TimeBaseB_Ctrl = 0;    // stop timer
			curkey = keydown;
			keydown = 0;
//			logKeystroke(curkey | 0xc000);
//			logRTC();
			return (curkey | LONG_KEY_STROKE);  // return long key stroke
		}
	}
	// loop allows time for service loop to stabilize on keys
	// based on C_DebounceCnt = 8 in IOKeyScan.asm (i < 12 was too short)	
	KeyScan_ServiceLoop();
	if (longCheck)
		for (i = 0; i < 15; i++)
			KeyScan_ServiceLoop();
		
	keystroke = (int)SP_GetCh();
	// BUG: Combo keys are not working. Not sure why yet.
//	if (keystroke == ADMIN_COMBO_KEYS)
//		adminOptions();  //might also want to move this to control tracks
	if (MACRO_FILE) {
		keystroke = nextMacroKey(keystroke);
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
		
		logKeystroke(keystroke);
//		logRTC();
		keydown = keystroke;
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

void resetSystem(void) {
	// set watchdog timer to reset device; 0x780A (Watchdog Reset Control Register)
	// see GPL Programmer's Manual (V1.0 Dec 20,2006), Section 3.5, page 18
	stop(); 
	logString((char *)"Reset",BUFFER);
	logRTC();
	fs_safexit(); // should close all open files
	*P_WatchDog_Ctrl &= ~0x4001; // clear bits 14 and 0 for resetting system and time=0.125 sec 	
	*P_WatchDog_Ctrl |= 0x8004; // set bits 2 and 15 for 0.125 sec, system reset, and enable watchdog
	while(1);	
}

static void logKeystroke(int intKey) {
	APP_IRAM static long secLastKey;
	long secNow;
	int diff, i;
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
		logString(str,ASAP);	
	}
}

void setOperationalMode(int newmode) {
  if(newmode == (int)P_WAIT) {
  	// stop();  --- should we see if we can WAIT while paused in an audio file?
  	SysIntoWaitMode();
    // when leaving wait mode, next instruction is executed, so we return here
    return;
  } else {
     	// assume calling for sleep or halt
		*P_Clock_Ctrl |= 0x200;	//bit 9 KCEN enable IOB0-IOB2 key change interrupt
		if (newmode == (int)P_HALT)
			logString((char *)"Halting",BUFFER);
		else // newmode == (int)P_SLEEP
			logString((char *)"Sleeping",BUFFER);			
		logRTC();
	  	stop();
		setLED(LED_ALL,FALSE);
	
		turnAmpOff();
		turnSDoff();
		turnNORoff();
	  	
	  	if (newmode == (int)P_HALT)  {
/*
	  		setRTCalarmSeconds(61);		// device should come back on in 61 seconds
*/
		  	SysIntoHaltMode();
	  	}
		else // newmode == (int)P_SLEEP
			_SystemOnOff();
	
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
set_voltmaxvolume()
{
	int	wrk = V_MIN_VOL_VOLTAGE - vCur_1;
	if(wrk > 0) {
		wrk >>= 3;
		// for every .08 volt below V_MIN_VOL_VOLTAGE subtract 1 from MAX_VOLUME
		wrk = 16 - wrk;
		if(wrk < 1) wrk = 1;
		if(wrk < MAX_VOLUME) {
			MAX_VOLUME = wrk;
			if (volume > MAX_VOLUME) {
				volume = MAX_VOLUME;
				SACM_Volume(volume);
				playBip();	
			}
			if (DEBUG_MODE) {
				char log[15] = "v---,MV--,CV--";
				longToDecimalString(vCur_1, log+1, 3);
				log[4] = ',';
				longToDecimalString((long)MAX_VOLUME,log+7,2);
				log[9] = ',';
				longToDecimalString((long)volume,log+11,2);
				logString(log,BUFFER);
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
	if (includePrefix)
		return P_TB_SERIAL_PREFIX;
	else
		return P_TB_SERIAL_NUMBER;	
}

void writeVersionToDisk() {
	char fileVersion [PATH_LENGTH];
	int handle, ret;
	struct f_info file_info;
		
	strcpy(fileVersion,DEFAULT_SYSTEM_PATH  VERSION  FILE_VERSION_EXT);
	
	if (!fileExists((LPSTR)fileVersion)) {
		logString(fileVersion,ASAP);
		mkdir((LPSTR)DEFAULT_SYSTEM_PATH); 
		tbChdir((LPSTR)DEFAULT_SYSTEM_PATH);
		ret =_findfirst((LPSTR)"*" FILE_VERSION_EXT, &file_info, D_FILE);
		if (ret >= 0) {
			ret = unlink((LPSTR)file_info.f_name);		
		}	
		handle = tbOpen((LPSTR)fileVersion,O_CREAT|O_RDWR|O_TRUNC);
		close(handle);	
	}
}

void
setRTCalarmSeconds(unsigned int seconds) {
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
	
	setRTCalarm(delta_hour, delta_min, delta_sec);
}
void
setRTCalarmMinutes(unsigned int minutes) {
	unsigned int h = *P_Hour;
	unsigned int m = *P_Minute;
	unsigned int s = *P_Second;
	unsigned int delta_min, delta_hour;

	delta_min = minutes + m;
	delta_hour = delta_min / 60;
	delta_min = delta_min % 60;

	delta_hour += h;

	setRTCalarm(delta_hour, delta_min, *P_Second);
}
void
setRTCalarmHours(unsigned int hours) {
	setRTCalarm(*P_Hour + hours, *P_Minute, *P_Second);
}

void
setRTCalarm(unsigned int hour, unsigned int minute, unsigned int second) {
#define RTC_ALARM_INTERRUPT_ENABLE 0x0400
#define RTC_ALARM_FUNCTION_ENABLE  0x0400
	*P_Alarm_Second = second;
	*P_Alarm_Minute = minute;
	*P_Alarm_Hour   = hour;
	
	*P_RTC_INT_Ctrl |= RTC_ALARM_INTERRUPT_ENABLE;
	*P_RTC_Ctrl     |= RTC_ALARM_FUNCTION_ENABLE;
}

/*  called from isr.asm when RTC alarm has fired
    running in interrupt service, so should minimize work done and return
*/
void
RTC_Alarm_Fired() {
	extern int rtc_pending;
	int wrk = *P_RTC_INT_Status;
	
// rtc alarm testing	rtc_pending = 0;
	*P_RTC_INT_Status |= wrk;	// clear all interrupt flags
	
}
void
KEY_TimeBase_B_isr() {	//TimerBase B fired
	unsigned int i;
	i = *P_TimeBaseB_Ctrl;
	*P_TimeBaseB_Ctrl = i;	// reset interrupt bit
	keydown_counter += 1;
}
