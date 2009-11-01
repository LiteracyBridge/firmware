// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__AUDIO_h__
#define	__AUDIO_h__

#include "containers.h"

extern void SACM_Stop(void);

extern void playDing(void);
extern void playBip(void);

extern void stop(void);
extern void play(CtnrFile *, unsigned int);
extern void insertSound(CtnrFile *, CtnrBlock *, BOOL);
extern void insertSoundFile(int);
extern int createRecording(char *, int, char *);
extern void markEndPlay(long);
extern void markStartPlay(long, const char *);

#endif
