#ifndef	__DEVICE_h__
#define	__DEVICE_h__

#include ".\Component\Include\component_head.h"			

#define USB_Host		1
#define USB_Device		2


extern int SystemIntoUSB(unsigned int);

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
// extern int setUSBHost (BOOL); -- moved into SystemIntoUSB for now
extern void logVoltage(long);
extern int keyCheck(int);
extern int waitForButton(int);
extern void wait(int);
extern void resetSystem(void);

#endif
