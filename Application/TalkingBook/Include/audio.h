#ifndef	__AUDIO_h__
#define	__AUDIO_h__

#include "containers.h"

extern void stop(void);
extern void play(CtnrFile *, unsigned int);
extern void insertSound(CtnrFile *, CtnrBlock *, BOOL);
extern void insertSoundFile(int);
extern int createRecording(char *, int, char *);
extern void markEndPlay(long);
extern void markStartPlay(long, const char *);

#endif
