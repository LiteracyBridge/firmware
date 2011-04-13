// Copyright 2009,2010 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef USBRP
#define SYS_CREATEVAR
#define CREATE_BS_VAR
#define CREATGUIVAR
#define CREATEKEYDECODEVAR
#endif

#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./Application/TalkingBook/Include/talkingbook.h"
#include "./Application/TalkingBook/Include/startup.h"
#include "./Application/TalkingBook/Include/device.h"
#include "./Reprog/USB_Flash_reprog.h"

extern long USB_ISR_PTR;
extern long USB_INSERT_PTR;
extern void USB_ISR(), USB_Insert_TimeOut();
extern void initVoltage();

void BodyInit();
extern unsigned int CLOCK_RATE;
extern unsigned int MEM_TYPE;

//unsigned int sav_P_MINT_Ctrl;
//unsigned int sav_P_Clock_Ctrl;
APP_IRAM char systemHeap [SYSTEM_HEAP_SIZE];
APP_IRAM char *cursorSystemHeap = systemHeap;
extern int statINIT;
//unsigned int sav_Int_status1;
//unsigned int sav_Int_status2;

// rtc alarm testing unsigned int rtc_pending;

int main (unsigned int bootType) {
//	int wrk;
//	wrk = bootType;
// rtc alarm testing	rtc_pending = 0;
/*	wrk = sav_Int_status1 = *P_INT_Status1;*/
//	sav_Int_status2 = *P_INT_Status2;

	MEM_TYPE = GetMemManufacturer();
	
	if(bootType == BOOT_TYPE_RTC_ALARM) {
		backfromRTC();
	}
	
	initVoltage();	// get initial voltage before SACM_Init in BodyInit - may never run BodyInit()
	
	BodyInit();
	
	USB_ISR_PTR = (long)USB_ISR;
	USB_INSERT_PTR = (long)USB_Insert_TimeOut;

	__asm__("irq on");
	__asm__("fiq on");
		
	fs_init();
	_devicemount(0); // should include call to IOKey_Initial() within BodyInit.c 
	 				// to flip on a transistor early enough to power the microSD card

	ChangeCodePage(UNI_ENGLISH);
	
	if(bootType != BOOT_TYPE_COLD_RESET) {
		fixRegs();
	}
	
	startUp(bootType);
	return 0;
}

void
backfromRTC()
{
	char buf[64];
	unsigned int hr, min, sec;
	
// If we are here we are back from RTC int, and we have not initialized ram
//  - the RTC continued to run
//  Anything done in SysIntoHaltMode to cut power consumption needs to be reversed here
	
#define ASAP 0

	fixRegs();
	hr = *P_Hour;
	min = *P_Minute;
	sec = *P_Second;	
	
	IOKey_Initial();	
	SD_Initial();
			
	logRTC();
	
	__asm__("irq on");
	__asm__("fiq on");
	
//	if(sav_Int_status2 & 0x2) {  // rtc alarm fired
//		*P_RTC_INT_Status |= 0x50f;	//clear all possible RTC interrupts
	strcpy(buf, "back from Halt - RTC Alarm fired\n");
	logString(buf ,ASAP);
	RTC_Alarm_Fired();			// do isr work, will ste next alarm
	
//	setLED(LED_GREEN,TRUE);
//	wait(1000);
//	setLED(LED_GREEN,FALSE);

	if(hr == 0 && min == 0) {
		setOperationalMode((int)P_HALT);    //go back to HALT, does not return
	} else {
		Fake_Keypress();	// initialize memory, fake a key press wakeup
	}
}
void fixRegs() {
	*P_MINT_Ctrl = 0x8;
	*P_Clock_Ctrl = 0x8618;		
	
	*P_CHA_Ctrl = 0x8505;
	*P_CHB_Ctrl = 0x8505;
	*P_HPAMP_Ctrl = 0xf;
	*P_DAC_IIS_Ctrl = 1;
	*P_DAC_Ctrl = 0xc;
	*P_NF_INT_Ctrl= 0x1600;
	*P_LCD_Setup = 0x0400;
	*P_IrDA_Ctrl = 0x0400;
	*P_SD_Ctrl = 0x0302;
	*P_I2C_En = 0x083E;
}
