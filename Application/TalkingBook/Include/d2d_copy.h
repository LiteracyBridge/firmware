// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__D2D_COPY_h__
#define	__D2D_COPY_h__

extern int d2dCopy(const char *, const char *);
extern int cloneDevice(void);
extern int cloneDir(char *, char *);
extern int copyLanguage(char *);
extern int setUSBHost(BOOL);
extern void buildMyStatsCSV(void);
extern void pushContentGetFeedback(void);
#endif
