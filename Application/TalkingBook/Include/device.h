// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__DEVICE_h__
#define	__DEVICE_h__

#include ".\Component\Include\component_head.h"

//V_MIN_USB_VOLTAGE refuse to record if voltage below this value (hundredths of volts)
#define V_MIN_USB_VOLTAGE 120
//V_MIN_RECORD_VOLTAGE refuse to record if voltage below this value (hundredths of volts)
#define V_MIN_RECORD_VOLTAGE 120
//V_MIN_SDWRITE_VOLTAGE refuse to record if voltage below this value (hundredths of volts)
#define V_MIN_SDWRITE_VOLTAGE 110
//V_MIN_RUN_VOLTAGE refuse to record if voltage below this value (hundredths of volts)
#define V_MIN_RUN_VOLTAGE 100
//V_VOL_BASE voltage at or below this value enforce maximum voilume settings
#define V_MIN_RUN_VOLTAGE 140

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
extern void logVoltage();
extern unsigned int getCurVoltageSample();
extern int keyCheck(int);
extern int waitForButton(int);
extern void wait(int);
extern void resetSystem(void);
extern void setOperationalMode(int);
#endif
