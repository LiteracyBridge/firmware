// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__SYS_COUNTERS_h__
#define	__SYS_COUNTERS_h__

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

extern void saveSystemCounts(void);
extern void fixBadDate(SystemCounts *);
extern int loadSystemCounts(void);
extern void getPkgNumber(char *, BOOL);
extern void getrevdPkgNumber(char *, BOOL);
#endif
