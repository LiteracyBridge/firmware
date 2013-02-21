// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "Include/talkingbook.h"
#include "Include/device.h"
#include "Include/files.h"
#include "Include/sys_counters.h"

extern APP_IRAM unsigned int vCur_1;
extern void refuse_lowvoltage(int);

APP_IRAM SystemCounts systemCounts;

void saveSystemCounts() {
	int handle, ret, i;

	if(vCur_1 < V_MIN_SDWRITE_VOLTAGE) {
		refuse_lowvoltage(0);
		return;
	}
		
	i = 0;
	do {
 		ret = unlink((LPSTR)(SYSTEM_VARIABLE_FILE));
 		if (ret)
	 		wait(100);
	}
	while (ret && ++i < 3);
	
	handle = tbOpen((LPSTR)(SYSTEM_VARIABLE_FILE),O_CREAT|O_RDWR);
	if (handle != -1)
		ret = write(handle, (unsigned long)&systemCounts<<1, sizeof(SystemCounts)<<1);
	else {
		if (ret)
			logString((char *)"failed unlink of system var file",BUFFER,LOG_ALWAYS);
		close(handle);
		logException(17,SYSTEM_VARIABLE_FILE,SHUT_DOWN); //can't save SYSTEM_VARIABLE_FILE;
	}
	close(handle);
}

extern void
fixBadDate(SystemCounts *sc) {
	int advance = 0;
	
	if (sc->year < FILE_YEAR_MIN)
		sc->year = FILE_YEAR_MIN;
	if (sc->monthday < 1)
		sc->monthday = 1;

	switch (sc->month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10: // special handling for December
			if (sc->monthday > 31)
				advance = 1;
			break;
		case 2:
			if (sc->monthday > 28)
				advance = 1; // ignore leapyear calculation
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if (sc->monthday > 30)
				advance = 1;
			break;
		case 12:
			if (sc->monthday <= 31)
				break;
		default:
			sc->month = sc->monthday = 1;
			break;
	}
	if (advance) {
		sc->month++;
		sc->monthday = 1;
	}
}

int loadSystemCounts() {
	int handle, ret;

	handle = tbOpen((LPSTR)(SYSTEM_VARIABLE_FILE),O_RDONLY);
	if (handle != -1) {
		ret = read(handle,(unsigned long)&systemCounts<<1,sizeof(SystemCounts)<<1);
		fixBadDate(&systemCounts);
	}		
	else {
		logString((char *)"***CAN'T LOAD SYSTEM COUNTS***",BUFFER,LOG_NORMAL);
		ret = -1;
		systemCounts.powerUpNumber = 0;
		systemCounts.poweredDays = 0;
		systemCounts.year = FILE_YEAR_MIN;
		systemCounts.month = 1;
		systemCounts.monthday = 1;
		systemCounts.location[0] = 0;
		setLocation(systemCounts.location);
	}
	close(handle);
	return ret;
}

void getPkgNumber(char *name, BOOL shouldUpdate) {
	char digits[8];
	
	strcpy(name,PKG_NUM_PREFIX);
	if (shouldUpdate)
		systemCounts.packageNumber++;
	longToDecimalString(systemCounts.packageNumber,digits,5);
	strcat(name,digits);
	saveSystemCounts();
}

void getrevdPkgNumber(char *name, BOOL shouldUpdate) {
	char digits[8];
	
//	strcpy(name,PKG_NUM_PREFIX);
// TOTO: add a prefix to config.txt processing
	strcpy(name,"R");
	if (shouldUpdate)
		systemCounts.revdPkgNumber++;
	longToDecimalString(systemCounts.revdPkgNumber,digits,5);
	strcat(name,digits);
	saveSystemCounts();
}	

/*
void nextListNumber(char *name) {
	char digits[8];
	
	strcpy(name,LIST_NUM_PREFIX);
	longToDecimalString(++systemCounts.listNumber,digits,4);
	strcat(name,digits);
	saveSystemCounts();
}	
*/
