#ifndef	__SYS_COUNTERS_h__
#define	__SYS_COUNTERS_h__

typedef struct SystemCounts SystemCounts;

struct SystemCounts {
	unsigned long powerUpNumber;
	unsigned long packageNumber;
	unsigned long listNumber;
	unsigned long lastLogErase;
	unsigned long revdPkgNumber;
};

extern SystemCounts systemCounts;

extern void saveSystemCounts();
extern int loadSystemCounts(void);
extern void getPkgNumber(char *, BOOL);

#endif
