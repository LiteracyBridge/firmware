// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__UTIL_h__
#define	__UTIL_h__

#include <string.h>
// extern char * strstr(const char *,const char *);
// extern char * strchr(const char *,char);
//extern int isspace(char);
//extern int isdigit(char);
// extern char * strcpy(char *, const char *);
// extern char * strcat(char *, const char *);
// extern unsigned long int strlen(const char *);
// extern unsigned long int strspn(const char *, const char *);
// extern unsigned long int strcspn(const char *, const char *);
extern void *memset(void *, int, unsigned long);
extern void *memcpy(void *, const void *, unsigned long);

extern int strIndex (const char *, char);
extern int getBitShift (unsigned int);
extern unsigned long extractTime (unsigned int, int);
extern signed long extractSignedTime (signed int, int);
extern unsigned int compressTime (unsigned long, int);
extern int lower (int);
extern int strToInt (char *);
extern void longToDecimalString(long, char *, int);
extern void unsignedlongToDecimalString(unsigned long, char *, int);
extern void longToHexString(long, char *, int);
extern void unsignedlongToHexString(unsigned long, char *);
extern void intToBinaryString(int, char *);
extern long strToLong (char *);
extern int goodChar(char,int);
extern int goodString(char *,int);
extern int LBstrncpy (char *, const char *, int);
extern int LBstrncat (char *, const char *, int);

extern unsigned long rand(void);
#endif
