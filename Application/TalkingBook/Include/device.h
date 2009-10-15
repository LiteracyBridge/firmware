#ifndef	__DEVICE_h__
#define	__DEVICE_h__

#include ".\Component\Include\component_head.h"			

extern int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;	
extern int LED_GREEN, LED_RED, LED_ALL;
extern int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
extern int NORMAL_VOLUME, MAX_VOLUME, VOLUME_INCREMENT;

extern void resetRTC(void);
extern long getRTCinSeconds(void);
extern void setLED(unsigned int, BOOL);
extern int restoreVolume(BOOL);
extern int adjustVolume (int, BOOL, BOOL);
extern int adjustSpeed (int, BOOL);
extern int getVolume(void);
extern int getSpeed(void);
extern void setUSBDevice (BOOL);
extern void logVoltage(unsigned long);
extern int keyCheck(int);
extern int waitForButton(int);
extern void wait(int);
extern void resetSystem(void);
extern void setOperationalMode(int);
#endif
