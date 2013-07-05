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
APP_IRAM struct SystemData systemData;
APP_IRAM struct SystemCounts2 systemCounts2;	
APP_IRAM struct PtrsSystemDataCounts ptrsCounts;

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

extern
void initSystemData() {
	int i;
	ptrsCounts.systemData = (struct SystemData *)FindFirstFlashStruct(NOR_STRUCT_ID_SYSTEM);
	ptrsCounts.systemCounts = (struct SystemCounts2 *)FindFirstFlashStruct(NOR_STRUCT_ID_COUNTS);
	if (ptrsCounts.systemCounts != NULL) {
		ptrsCounts.period = (struct NORperiod *)&ptrsCounts.systemCounts->period;
		ptrsCounts.corruptionDay = (struct NORcorruption *)&ptrsCounts.systemCounts->corruptionDay;
		ptrsCounts.cumulativeDays = (struct NORcumulativeDays *)&ptrsCounts.systemCounts->cumulativeDays;
		for (i=0;i < MAX_WEEKS && ptrsCounts.systemCounts->weeks[i].structType == NOR_STRUCT_ID_WEEK;i++);
		ptrsCounts.latestWeek = &ptrsCounts.systemCounts->weeks[i];
		ptrsCounts.powerups = (struct NORpowerups *)&ptrsCounts.systemCounts->powerups;
	}
	if (ptrsCounts.period == NULL)
		ptrsCounts.period = (struct  NORperiod *)FindLastFlashStruct(NOR_STRUCT_ID_PERIOD);
	if (ptrsCounts.corruptionDay == NULL)
		ptrsCounts.corruptionDay = (struct NORcorruption *)FindLastFlashStruct(NOR_STRUCT_ID_CORRUPTION);
	if (ptrsCounts.cumulativeDays == NULL)
		ptrsCounts.cumulativeDays = (struct NORcumulativeDays *)FindLastFlashStruct(NOR_STRUCT_ID_CUMULATIVE_DAYS);
	if (ptrsCounts.latestWeek == NULL) 
		ptrsCounts.latestWeek = (struct NORweek *)FindLastFlashStruct(NOR_STRUCT_ID_WEEK);
	if (ptrsCounts.powerups == NULL)
		ptrsCounts.powerups = (struct NORpowerups *)FindLastFlashStruct(NOR_STRUCT_ID_POWERUPS);
}

char
getPeriod() {
	char ret = -1;
	if (ptrsCounts.period != NULL) {
		ret = ptrsCounts.period->period;
	} else {
		struct NORperiod *period = (struct NORperiod *)FindLastFlashStruct(NOR_STRUCT_ID_PERIOD);
		if (period != NULL) {
			ret = period->period;
			ptrsCounts.period = period;
		}
	}
	return ret;
}

extern void 
setPeriod(char periodNumber) {
	struct NORperiod period;
	
	period.structType = NOR_STRUCT_ID_PERIOD;
	period.period = periodNumber;
	ptrsCounts.period = (struct NORperiod *)AppendStructToFlash(&period);
}

char
getCumulativeDays() {
	char ret = -1;
	if (ptrsCounts.cumulativeDays != NULL) {
		ret = ptrsCounts.cumulativeDays->cumulativeDaysSinceUpdate;
	} else {
		struct NORcumulativeDays *days = (struct NORcumulativeDays *)FindLastFlashStruct(NOR_STRUCT_ID_CUMULATIVE_DAYS);
		if (days != NULL) {
			ret = days->cumulativeDaysSinceUpdate;
			ptrsCounts.cumulativeDays = days;
		}
	}
	return ret;
}

extern void
setCumulativeDays(char days) {
	struct NORcumulativeDays cumulativeDays;
	
	cumulativeDays.structType = NOR_STRUCT_ID_CUMULATIVE_DAYS;
	cumulativeDays.cumulativeDaysSinceUpdate = days;
	ptrsCounts.cumulativeDays = (struct NORcumulativeDays *)AppendStructToFlash(&cumulativeDays);	
}

char 
getCorruptionDay() {
	char ret = -1;
	if (ptrsCounts.corruptionDay != NULL) {
		ret = ptrsCounts.corruptionDay->daysAfterLastUpdate;
	} else {
		struct NORcorruption *corruption = (struct NORcorruption *)FindLastFlashStruct(NOR_STRUCT_ID_CORRUPTION);
		if (corruption != NULL) {
			ret = corruption->daysAfterLastUpdate;
			ptrsCounts.corruptionDay = corruption;
		}
	}
	return ret;
}

void
setCorruptionDay(char day) {
	struct NORcorruption corruption;
	
	corruption.structType = NOR_STRUCT_ID_CORRUPTION;
	corruption.daysAfterLastUpdate = day;
	ptrsCounts.corruptionDay = (struct NORcorruption *)AppendStructToFlash(&corruption);	
}

unsigned int 
getPowerups() {
	char ret = -1;
	if (ptrsCounts.powerups != NULL) {
		ret = ptrsCounts.powerups->powerups;
	} else {
		struct NORpowerups *powerups = (struct NORpowerups *)FindLastFlashStruct(NOR_STRUCT_ID_POWERUPS);
		if (powerups != NULL) {
			ret = powerups->powerups;
			ptrsCounts.powerups = powerups;
		}
	}
	return ret;
}

extern void
setPowerups(unsigned int powerupNumber) {
	struct NORpowerups powerups;
	
	powerups.structType = NOR_STRUCT_ID_POWERUPS;
	powerups.powerups = powerupNumber;
	ptrsCounts.powerups = (struct NORpowerups *)AppendStructToFlash(&powerups);		
}

struct NORweek *getLatestWeekStruct() {
	struct NORweek *ret = NULL;
	if (ptrsCounts.latestWeek != NULL) {
		ret = ptrsCounts.latestWeek;
	} else {
		struct NORweek *week = (struct NORweek *)FindLastFlashStruct(NOR_STRUCT_ID_WEEK);
		if (week != NULL) {
			ret = week;
			ptrsCounts.latestWeek = week;
		}
	}
	return ret;
}

char getWeek() {
	struct NORweek *week = getLatestWeekStruct(); 
	char ret = -1;
	if (week != NULL)
		ret = week->weekNumber;
	return ret;
}

extern void
setWeek(char weekNumber, char period, char days) {
	struct NORweek week;
	week.structType = NOR_STRUCT_ID_WEEK;
	week.weekNumber = weekNumber;
	week.periodNumber = period;
	week.daysAfterLastUpdate = days;
	ptrsCounts.latestWeek = (struct NORweek *)AppendStructToFlash(&week);
}

extern void 
setSystemData(char *srn, char *loc, char *pkg, char monthUpdated, char dayUpdated) {
	struct SystemData sd;
	struct SystemCounts2 sc;
	int size, totalSize, i;
	
	sd.structType = NOR_STRUCT_ID_SYSTEM;
	strcpy(sd.serialNumber,srn);
	strcpy(sd.location,loc);
	strcpy(sd.contentPackage,pkg);
	sd.monthLastUpdated = monthUpdated;
	sd.dateLastUpdated = dayUpdated;

	sc.structType = NOR_STRUCT_ID_COUNTS;
	sc.period = 0;
	sc.cumulativeDays = 0;
	sc.corruptionDay = 0;
	sc.powerups = 0;
	sc.weeks[0].structType = NOR_STRUCT_ID_WEEK;
	sc.weeks[0].weekNumber = 0;
	sc.weeks[0].periodNumber = 0;
	sc.weeks[0].daysAfterLastUpdate = 0;
	for (i=1; i< MAX_WEEKS; i++) {
		sc.weeks[i].structType = -1;
	}
	totalSize = 0;
	size = (sizeof(struct SystemData) + 1) >> 1;	// round up # of words		
	write_app_flash((int *)&sd, size, totalSize);
	totalSize += size;
	ptrsCounts.systemData = (struct SystemData *)(TB_SERIAL_NUMBER_ADDR);

	size = (sizeof(struct SystemCounts2) + 1) >> 1;	// round up # of words		
	write_app_flash((int *)&sc, size, totalSize);
	ptrsCounts.systemCounts = (struct SystemCounts2 *)(TB_SERIAL_NUMBER_ADDR + totalSize);
	ptrsCounts.period = (struct NORperiod *)&ptrsCounts.systemCounts->period;
	ptrsCounts.corruptionDay = (struct NORcorruption *)&ptrsCounts.systemCounts->corruptionDay;
	ptrsCounts.cumulativeDays = (struct NORcumulativeDays *)&ptrsCounts.systemCounts->cumulativeDays;
	ptrsCounts.powerups = (struct NORpowerups *)&ptrsCounts.systemCounts->powerups;
	ptrsCounts.latestWeek = &ptrsCounts.systemCounts->weeks[0];
}

extern char *
getPackageName(void) {
	char *ret;
	
	ret = ptrsCounts.systemData->contentPackage; 
	return ret;
}
