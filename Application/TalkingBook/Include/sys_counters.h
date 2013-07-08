// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__SYS_COUNTERS_h__
#define	__SYS_COUNTERS_h__

#include "filestats.h"

#define FILE_YEAR_MIN 2000
#define CLOCK_PERIOD	(systemCounts.year - FILE_YEAR_MIN)

typedef struct SystemCounts SystemCounts;

struct SystemCounts {
	unsigned long powerUpNumber;
	unsigned long packageNumber;
	unsigned long listNumber;
	unsigned long lastLogErase;
	unsigned long revdPkgNumber;
    unsigned short year;
    unsigned char monthday , month;
   	unsigned long recordingNumber;
   	unsigned short poweredDays;
   	char location[40];
};

extern SystemCounts systemCounts;

#define NOR_STRUCT_ID_SYSTEM	0
// 62 words
struct SystemData {
	char structType;  // can act as versioning field (next version could be == 23)
	unsigned int countReflashes;
	// updated just once
	char serialNumber[10];			// .srn  //transition to become one long 32-bit since most SRNs are useless prefix followed by 6-hex digits
	// updated monthly
	char updateNumber[10];
	char location[40]; 					// .loc
	char contentPackage[10];				// .pkg
	char dateLastUpdated; // 	last_updated.txt  -- important to know when corruption caused a mid-month reformat or update --could also use days since 1/1/2013
	char monthLastUpdated; // 	last_updated.txt  -- important to know when corruption caused a mid-month reformat or update --could also use days since 1/1/2013
	char yearLastUpdated;
};

//extern SystemData systemData;

#define NOR_STRUCT_ID_COUNTS	10
// 13 words
struct SystemCounts2 {
	char structType;
	char period;
	char cumulativeDays;
	char corruptionDay;	
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
extern void saveSystemCounts(void);
extern void fixBadDate(SystemCounts *);
extern int loadSystemCounts(void);
extern void getPkgNumber(char *, BOOL);
extern void getrevdPkgNumber(char *, BOOL);
extern void initSystemData(void);
extern char getRotation(void);
extern char getReflashCount(void);
extern char getUpdateYear(void);
extern char getUpdateMonth(void);
extern char getUpdateDate(void);
extern void setRotation(char, char, char, int);
extern void transitionOldToNewFlash(void);
extern void importNewSystemData(LPSTR);
extern struct NORrotation *getLatestRotationStruct(void);
extern unsigned int getLastInitVoltage(void);
extern int rotate(void);

#endif