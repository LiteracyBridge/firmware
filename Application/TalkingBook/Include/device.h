// Copyright 2009-2011 Literacy Bridge
// Contact: info@literacybridge.org
#ifndef	__DEVICE_h__
#define	__DEVICE_h__

#include ".\Component\Include\component_head.h"

//refuse to go into usb mode if voltage below this value
#define V_MIN_USB_VOLTAGE 0 // 190

//refuse to record if voltage below this value (hundredths of volts)
#define V_MIN_RECORD_VOLTAGE 0 //200

//refuse to write to sd if voltage below this value (hundredths of volts)
#define V_MIN_SDWRITE_VOLTAGE 0 // 190

//refuse to run if voltage below this value (hundredths of volts)
#define V_MIN_RUN_VOLTAGE_TRANS 160
#define V_MIN_RUN_VOLTAGE 170
#define V_MIN_RUN_VOLTAGE_STARTUP 190

//voltage at or below this value enforce maximum volume settings
#define V_MIN_VOL_VOLTAGE 310

// must be powered externally if less than this (hundredths of volts)
#define V_MIN_POSSIBLE_VOLTAGE 120

//used when powered externally to ensure all voltage checks pass (hundredths of volts)
#define V_EXTERNAL_VOLTAGE 400 		// higher than possible with batteries -- used to indicate running on external power

#define TB_SERIAL_NUMBER_ADDR  		0x37000
#define TB_SERIAL_NUMBER_ADDR_TEXT	"0x37000" //must match TB_SERIAL_NUMBER_ADDR
#define TB_FLASH_SIZE               0x1000    // 4096
#define P_TB_SERIAL_PREFIX			(char *)(TB_SERIAL_NUMBER_ADDR)
#define CONST_TB_SERIAL_PREFIX		(char *)"srn."
#define CONST_TB_SERIAL_PREFIX_LEN	(strlen(CONST_TB_SERIAL_PREFIX))   // strlen of TB_SERIAL_PREFIX above
#define P_TB_SERIAL_NUMBER      	(char *)(TB_SERIAL_NUMBER_ADDR + CONST_TB_SERIAL_PREFIX_LEN)
#define FILE_REVISION_EXT			".rev"
#define NO_SRN						(char *)"NO_SRN"
// if change clock in TIMEBASE_B_16HZ need to think about changing KEY_LONG_DOWN_THRESH as well
#define TIMEBASE_B_16HZ       0xe001
#define KEY_LONG_DOWN_THRESH  24
#define LONG_KEY_STROKE       0x4000
#define RTC_STRING_LENGTH		40
extern unsigned int LONG_KEYPRESS_COUNTER;
extern int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;	
extern int LED_GREEN, LED_RED, LED_ALL;
extern int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
extern int NORMAL_VOLUME, MAX_VOLUME, MIN_VOLUME, VOLUME_INCREMENT;

extern void KeyScan_ServiceLoop(void);
extern void turnNORoff(void);
extern void setRTCFromText(char *);
extern void setRTC(unsigned int, unsigned int, unsigned int);
extern void getRTC(char *);
extern void appendHiLoVoltage(char *);
extern void resetRTC(void);
extern long getRTCinSeconds(void);
extern void setLED(unsigned int, BOOL);
extern void setUSBDevice (BOOL);
extern unsigned int getCurVoltageSample(void);
extern unsigned int checkVoltage(void);
extern int keyCheck(int);
extern int waitForButton(int);
extern void wait(int);
extern int waitAndCheckKeys(int);
extern void resetSystem(void);
extern void fastShutdown(void);
extern void shutdown(void);
extern void setOperationalMode(int);
extern int logLongHex(unsigned long);
extern void refuse_lowvoltage(int die);
extern void turnAmpOff(void);
extern void turnAmpOn(void);
extern char* getDeviceSN(void);
extern int SNexists(void);
extern void writeVersionToDisk(char *);

#define RTC_ALARM_INTERRUPT_ENABLE 0x0400
#define RTC_ALARM_FUNCTION_ENABLE  0x0400
#define RTC_MODULE_ENABLE          0x8000

extern void rtcAlarmFired(unsigned long);
extern unsigned long setRTCalarmSeconds(unsigned long);
extern unsigned long setRTCalarmMinutes(unsigned int);
extern unsigned long setRTCalarmHours(unsigned);
extern void setRTCalarm(unsigned int, unsigned int, unsigned int);
extern unsigned long addAlarm(unsigned int hour, unsigned int minute, unsigned int second);
extern void setNextAlarm(void);
extern void confirmSNonDisk(void);
extern void alertCorruption(void);
extern void	confirmPackageNameonDisk(void);
extern void confirmLocationonDisk(void);
#define N_RTC_ALARMS 6
#define N_RTC_INIT   N_RTC_ALARMS+1


#endif
