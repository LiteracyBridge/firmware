// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__DEVICE_h__
#define	__DEVICE_h__

#include ".\Component\Include\component_head.h"

//refuse to go into usb mode if voltage below this value
#define V_MIN_USB_VOLTAGE 190

//refuse to record if voltage below this value (hundredths of volts)
#define V_MIN_RECORD_VOLTAGE 200

//refuse to write to sd if voltage below this value (hundredths of volts)
#define V_MIN_SDWRITE_VOLTAGE 200

//refuse to run if voltage below this value (hundredths of volts)
#define V_MIN_RUN_VOLTAGE 175

//voltage at or below this value enforce maximum volume settings
#define V_MIN_VOL_VOLTAGE 220

// must be powered externally if less than this (hundredths of volts)
#define V_MIN_POSSIBLE_VOLTAGE 80

//used when powered externally to ensure all voltage checks pass (hundredths of volts)
#define V_NORMAL_VOLTAGE 330

#define TB_SERIAL_NUMBER_ADDR  		0x37000
#define TB_SERIAL_NUMBER_ADDR_TEXT	"0x37000" //must match TB_SERIAL_NUMBER_ADDR
#define P_TB_SERIAL_PREFIX			(char *)(TB_SERIAL_NUMBER_ADDR)
#define CONST_TB_SERIAL_PREFIX		(char *)"tsn."
#define CONST_TB_SERIAL_PREFIX_LEN	(strlen(CONST_TB_SERIAL_PREFIX))   // strlen of TB_SERIAL_PREFIX above
#define P_TB_SERIAL_NUMBER      	(char *)(TB_SERIAL_NUMBER_ADDR + CONST_TB_SERIAL_PREFIX_LEN)

extern int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;	
extern int LED_GREEN, LED_RED, LED_ALL;
extern int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
extern int NORMAL_VOLUME, MAX_VOLUME, VOLUME_INCREMENT;

extern void KeyScan_ServiceLoop(void);

extern void logRTC(void);
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
extern int logLongHex(unsigned long);
extern void refuse_lowvoltage(int die);
extern void turnAmpOff(void);
extern void turnAmpOn(void);
extern char* getDeviceSN(int);
extern int SNexists(void);


#endif
