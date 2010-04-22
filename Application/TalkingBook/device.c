// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "./system/include/system_head.h"
#include "Include/talkingbook.h"
#include "Include/files.h"
#include "Include/macro.h"
#include "Include/containers.h"
#include "Include/Inbox.h"
#include "Include/audio.h"
#include "Include/device.h"

extern void _SystemOnOff(void);
extern int SystemIntoUDisk(unsigned int);
extern void KeyScan_ServiceLoop(void);
extern int SP_GetCh(void);

static void logKeystroke(int);
static void Log_ClockCtrl(void);
static void turnSDoff(void);
static void turnNORoff(void);

APP_IRAM static int volume, speed;
APP_IRAM static unsigned long voltage; // voltage sample 
APP_IRAM static int oldVolume;
extern APP_IRAM int vThresh_1;
extern APP_IRAM unsigned int vCur_1;
void set_voltmaxvolume();


void resetRTC(void) {
	*P_Second = 0;
	*P_Minute = 0;
	*P_Hour = 0;	
}

void logRTC(void) {
	long h,m,s;
	char time[12];

	s = (long) *P_Second;
	m = (long) *P_Minute;
	h = (long) *P_Hour;	

	longToDecimalString(h,time,4);
	time[4] = 'h';
	longToDecimalString(m,time+5,2);
	time[7] = 'm';
	longToDecimalString(m,time+8,2);
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

	if (volume > MAX_VOLUME)  
		volume = MAX_VOLUME;
	if (volume < 0)  
		volume = 0;
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
		SystemIntoUDisk(1);	
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
		}
	}
	return(ret);
}

int keyCheck(int longCheck) {
	// get key press or use macro
	int i, keystroke;
	
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
	if (MACRO_FILE)
		keystroke = nextMacroKey(keystroke);
	if (keystroke)
		logKeystroke(keystroke);
	return keystroke;
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
	stop(); //includes flushLog();
	fs_safexit(); // should close all open files
	*P_WatchDog_Ctrl &= ~0x4001; // clear bits 14 and 0 for resetting system and time=0.125 sec 	
	*P_WatchDog_Ctrl |= 0x8004; // set bits 2 and 15 for 0.125 sec, system reset, and enable watchdog
	while(1);	
}

static void logKeystroke(int intKey) {
	APP_IRAM static long secLastKey;
	long secNow;
	int diff, i;
	char str[10];

	if (LOG_KEYS) {
		secNow = getRTCinSeconds();
		diff = secNow - secLastKey;
		secLastKey = secNow;
		longToDecimalString(diff,str,3);
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
		logString(str,BUFFER);	
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
	  	stop();
		setLED(LED_ALL,FALSE);
	
		turnAmpOff();
		turnSDoff();
		turnNORoff();
	  	
	  	if (newmode == (int)P_HALT) 
		  	SysIntoHaltMode();
		else // newmode == (int)P_SLEEP
			_SystemOnOff();
	
		while(1);	
	    // cpu reset on exiting halt/sleep mode, so nothing below here executes
     }
}

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
	extern void playDing(void);
	playDing();
	playDing();
	if(die != 0) {
		setLED(LED_ALL, FALSE);
		wait(500);
		setLED(LED_RED, TRUE);
		wait(250);
		setLED(LED_RED, FALSE);
		wait(500);
		setLED(LED_RED, FALSE);
		setOperationalMode((int)P_HALT);
	} else {
		playDing();
		playDing();
	}
}
void
set_voltmaxvolume()
{
	int	wrk = V_MIN_VOL_VOLTAGE - vCur_1;
	if(wrk > 0) {
		wrk >>= 1;
		// for every .02 volt below V_MIN_VOL_VOLTAGE subtract 1 from MAX_VOLUME
		wrk = 16 - wrk;
		if(wrk <= 1) wrk = 2;
		if(wrk < MAX_VOLUME) {
			MAX_VOLUME = wrk;
			if (volume > MAX_VOLUME)  
				volume = MAX_VOLUME;
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
