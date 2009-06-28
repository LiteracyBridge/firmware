#ifndef	__FILES_h__
#define	__FILES_h__

#include ".\Component\Include\FS\vfs.h"

#define LOG_BUFFER_SIZE		512
#define ASAP				0
#define BUFFER				1

extern void logString(char *, int);
extern char *getLine (int, char *);
extern BOOL readBuffer(int, char *, int);
extern BOOL nextNameValuePair (int, char *, char, char **, char **);
extern void clearStaleLog(void);
extern void flushLog(void);
extern int appendStringToFile(const char *, char *);
//extern int createControlFromTemplate(char *, char *);
extern int findDeleteStringFromFile(char *, char *, char *, BOOL);
extern long getFilePosition(void);
extern INT16 tbOpen(LPSTR, INT16);
extern INT16 tbChdir(LPSTR);
extern int fileExists(LPSTR);
extern int dirExists(LPSTR);
extern int convertDoubleToSingleChar(char *, const char *, BOOL);
#endif
