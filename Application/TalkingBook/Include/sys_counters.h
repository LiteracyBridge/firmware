// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__SYS_COUNTERS_h__
#define	__SYS_COUNTERS_h__

#include "filestats.h"

#define FILE_YEAR_MIN 2000
#define CLOCK_PERIOD	(systemCounts.year - FILE_YEAR_MIN)
#define PACKAGE_EXT		".pkg"
#define SRN_MAX_LENGTH				12
#define UPDATENUMBER_MAX_LENGTH		20
#define LOCATION_MAX_LENGTH			40
#define CONTENTPACKAGE_MAX_LENGTH	20

typedef struct SystemCounts SystemCounts;

struct SystemCounts {
//	unsigned long powerUpNumber;
//	unsigned long packageNumber;
//	unsigned long listNumber;
//	unsigned long lastLogErase;
//	unsigned long revdPkgNumber;
    unsigned short year;
    unsigned char monthday , month;
   	unsigned long recordingNumber;
//  	unsigned short poweredDays;
//   	char location[40];
};

extern SystemCounts systemCounts;

#define NOR_STRUCT_ID_SYSTEM	20  // this should be higher than all other NOR_STRUCT_ID_ ..
// 0x4b / 75d words 
struct SystemData {
	//
	// IMPORTANT: MUST SYNCH WITH JAVA CLASS THAT READS THIS STRUCT IN TB LOADER
	//
	char structType;  // can act as versioning field (next version could be == 23)
	unsigned int countReflashes;
	// updated just once
	char serialNumber[SRN_MAX_LENGTH];			// .srn  //transition to become one long 32-bit since most SRNs are useless prefix followed by 6-hex digits
	// updated monthly
	char updateNumber[UPDATENUMBER_MAX_LENGTH];
	char location[LOCATION_MAX_LENGTH]; 					// .loc
	char contentPackage[CONTENTPACKAGE_MAX_LENGTH];				// .pkg // should this be profile
	char dateLastUpdated; // 	last_updated.txt  -- important to know when corruption caused a mid-month reformat or update --could also use days since 1/1/2013
	char monthLastUpdated; // 	last_updated.txt  -- important to know when corruption caused a mid-month reformat or update --could also use days since 1/1/2013
	int yearLastUpdated;
};

//extern SystemData systemData;

#define NOR_STRUCT_ID_COUNTS	10
// 6+5*5=31 words or 0x1F words
struct SystemCounts2 {
	char structType;
	char period;
	unsigned int cumulativeDays;
	unsigned int corruptionDay;	
	unsigned int powerups;
	unsigned int lastInitVoltage;
	struct NORrotation rotations[MAX_ROTATIONS];
};

//extern struct SystemCounts2 systemCounts2;

struct PtrsSystemDataCounts {
	struct SystemData *systemData;
	struct SystemCounts2 *systemCounts;
	struct NORperiod *period;
	struct NORcumulativeDays *cumulativeDays;
	struct NORpowerups *powerups;
	struct NORcorruption *corruptionDay;
	struct NORrotation *latestRotation;
};

extern struct PtrsSystemDataCounts ptrsCounts;
extern char getPeriod(void);
extern char getCumulativeDays(void);
extern int incrementCumulativeDays(void);
extern char getCorruptionDay(void);
extern void setCorruptionDay(char);
extern unsigned int getPowerups(void); 
extern struct NORrotation *getLatestRotationStruct(void);
extern void setSystemData(struct SystemData *);
extern char *getPackageName(void);
extern void setPowerups(unsigned int);
extern int incrementPeriod(void);
extern char *getSerialNumber(void);
extern char *getPackageName(void);
extern char *getLocation(void);
extern char *getUpdateNumber(void);
//extern void saveSystemCounts(void);
extern void fixBadDate(SystemCounts *);
extern int loadSystemCounts(void);
extern void getPkgNumber(char *, BOOL);
extern void getrevdPkgNumber(char *, BOOL);
extern void initSystemData(void);
extern char getRotation(void);
extern char getReflashCount(void);
extern int getUpdateYear(void);
extern char getUpdateMonth(void);
extern char getUpdateDate(void);
extern void setRotation(char, char, unsigned int, int);
//extern void transitionOldToNewFlash(void);
extern void importNewSystemData(LPSTR);
extern struct NORrotation *getLatestRotationStruct(void);
extern unsigned int getLastInitVoltage(void);
extern int rotate(void);
extern void logDate(void);
extern void dumpSystemDataToLog(struct SystemData *);
#endif