// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "./system/include/System/GPL162002.h"
#include "./Application/TalkingBook/Include/talkingbook.h"
#include "./Application/TalkingBook/Include/sys_counters.h"
#include "./Application/TalkingBook/Include/device.h"
#include "./Application/TalkingBook/Include/files.h"
#include "./Application/TalkingBook/Include/audio.h"
#include "./Application/TalkingBook/Include/app_exception.h"
extern APP_IRAM unsigned int vCur_1;
extern void refuse_lowvoltage(int);
extern int SystemIntoUDisk(unsigned int);

void logException(unsigned int errorCode, const char * pStrError, int takeAction) {
	// errorcode == 1 means memory error from BodyInit() and ucBSInit()
	int i; 
	char errorString[160];
	
	if(vCur_1 < V_MIN_SDWRITE_VOLTAGE) {
	}
	
	if (takeAction || DEBUG_MODE) {
		strcpy(errorString,"\x0d\x0a" "*** ERROR! (cycle "); //cycle number
		longToDecimalString(systemCounts.powerUpNumber,(char *)(errorString+strlen(errorString)),4);
		strcat(errorString," - version " VERSION ")\x0d\x0a*** #");
		longToDecimalString((long)errorCode,(char *)(errorString+strlen(errorString)),3);
		if (takeAction) {
			strcat(errorString,"-fatal");
			stop();						
		}
		else 
			strcat(errorString,"-warning");

		if (LOG_FILE) {
			logString(errorString,ASAP);
			if (pStrError) {
				LBstrncpy(errorString,pStrError,80);
				logString(errorString,ASAP);
			}
		}
		else {
			appendStringToFile(ERROR_LOG_FILE,errorString);	
			if (pStrError) {
				LBstrncpy(errorString,pStrError,80);
				appendStringToFile(ERROR_LOG_FILE,errorString);
			}
		}
	}		
	//todo: put a parameter in fct to return instead of reset or USB
	//maybe a choice of the three RETURN, RESET, USB
	if (takeAction) {
// 		commenting out code to alert user of error -- just use lights and auto-reset to welcome msg
//		if (errorCode != 10 && errorCode != 14)  // can't access config or system boot 
//			insertSoundFile(ERROR_SOUND_FILE_IDX);
//		if (errorCode != 14) // LED_GREEN and LED_RED are not assigned without config file
			for (i=0; i < 5; i++) {
				setLED(LED_GREEN,FALSE);
				setLED(LED_RED,TRUE);
				wait(500);
				setLED(LED_RED,FALSE);
				setLED(LED_GREEN,TRUE);
				wait(500);
			}
		if (takeAction == USB_MODE) { // can't load config
			SystemIntoUDisk(1);	
			resetSystem();
		}
		else if (takeAction == RESET)
			resetSystem();
		else if (takeAction ==  SHUT_DOWN)
			setOperationalMode((int)P_SLEEP);
	}
}

