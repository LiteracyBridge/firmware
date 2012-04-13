// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__AUDIO_h__
#define	__AUDIO_h__

#include "containers.h"

extern unsigned long SACM_A1800_Bytes;
extern unsigned long SACM_A1800_Msec;		 
extern void SACM_Stop(void);

extern void playDing(void);
extern void playBip(void);
extern void playDings(int);
extern void playBips(int);

extern unsigned long wordsFromFrames(unsigned long);
extern unsigned long framesFromBytes(unsigned long);
extern unsigned long msecFromFrames(unsigned long);
extern unsigned long framesFromMSec(unsigned long);
extern unsigned long getCurrentFrame(void);
extern unsigned long getCurrentMsec(void);
extern int gotoFrame(unsigned long frameDest);
extern int gotoMsec(unsigned long msec);
//extern unsigned long setFileHeader(char *, unsigned long);
extern void playActionSound(EnumAction); 
extern void pause(void);
extern void resume(void);
extern void stop(void);
extern void play(CtnrFile *, unsigned int);
extern void insertSound(CtnrFile *, CtnrBlock *, BOOL);
extern void insertSoundFile(int);
extern int createRecording(char *, int, char *, BOOL);
#define ERR_CATEGORY_LOCKED         -2
#define MINIMUM_PLAY_SEC_TO_LOG 	3
#define MIN_AUDIO_FILE_SIZE         0		
extern void markEndPlay(long);
extern void markStartPlay(long, const char *);

#endif
