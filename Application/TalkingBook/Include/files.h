// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__FILES_h__
#define	__FILES_h__

#include ".\Component\Include\FS\vfs.h"
#include "./Application/TalkingBook/Include/lists.h"

#define LOG_BUFFER_SIZE		1024
#define COPY_BUFFER_SIZE    1024
#define ASAP				0
#define BUFFER				1

#define LOG_ALWAYS	0
#define	LOG_NORMAL 	1
#define LOG_DETAIL	2


// states in expandOstatFile, processing a csv from some other device
#define FIND_SRN      0
#define PROCESS_STATS 1
#define FIND_DELIM    2
// delimiter between systems in an ostats csv file
#define OSTATS_DELIM  "----"
//#define DELIM "\r\n------\r\n"
#define DELIM "\r\n" OSTATS_DELIM "\r\n"
// extension of ostats exchage file
#define OSTATS_EXCHG_EXT    ".txt"
#define LOG_ARCHIVE_PATH	"a:/log-archive/"
#define LOG_ARCHIVE_OTHERS_PATH		LOG_ARCHIVE_PATH "others/"
#define LOG_EXTENSION		".txt"
#define INSPECT_TRIGGER_FILE	"inspect"
#define OTHER_INSPECT_TRIGGER_FILE	"b:/" INSPECT_TRIGGER_FILE
#define BKPATH "a:/backup/"
#define LOST "a:/LOST"
#define LOG_ARCHIVE "a:/archive/"


extern void logNumber(long, int, int, int);
extern void logStringRTCOptional(char *, int, int,int);
extern void logString(char *, int, int);
extern char *getLine (int, char *);
extern BOOL readBuffer(int, char *, int);
extern BOOL nextNameValuePair (int, char *, char, char **, char **);
extern void clearStaleLog(void);
extern void flushLog(void);
extern int appendStringToFile(const char *, char *);
extern int insertStringInFile(const char *, char *, long);
extern int insertStringInNewFile (const char *, char *);
//extern int createControlFromTemplate(char *, char *);
extern int findDeleteStringFromFile(char *, char *, const char *, BOOL);
extern long getFilePosition(void);
extern INT16 tbOpen(LPSTR, INT16);
extern INT16 tbChdir(LPSTR);
extern int fileExists(LPSTR);
extern int dirExists(LPSTR);
extern int convertDoubleToSingleChar(char *, const char *, BOOL);
extern void trimFile(char *, unsigned long, unsigned long);
extern int fileCopy(char *, char *);
extern void dirCopy(char *, char *, BOOL);
extern int copyMovedir(char *, char *);
extern void copyAllFiles(char *, char *, BOOL);
extern void moveAllFiles(char *, char *);
extern void deleteAllFiles(char *);
extern void moveAudioFiles(char *, char *);
extern void categoryStringtoLong(char *, MLENTRY *);
extern void categoryLongtoString(char *, MLENTRY *);
extern unsigned int loadLanglisttoMemory(char *,  MLENTRY[], unsigned int);
extern int buildExchgOstats(void);
extern void forceflushLog(void);
extern void logStat(char *);
extern int isCorrupted(char *);
extern int replaceFromBackup(char *);
extern void triggerInspection(void);
extern void saveLogFile(int);
extern int setLocation(char *); 
#endif
