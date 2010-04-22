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

extern long USB_ISR_PTR;
extern long USB_INSERT_PTR;
extern void USB_ISR(), USB_Insert_TimeOut();
extern void initVoltage();

void BodyInit(void);
extern unsigned int CLOCK_RATE;

int main (void) {
#ifdef TB_CAN_WAKE
	// restart with system reset if waking from halt (caused by IOB key change interupt) 
	if (*P_INT_Status1 & 0x8000) {
 		*P_INT_Status1 |= 0x8000;
		*P_WatchDog_Ctrl &= ~0x4001; // clear bits 14 and 0 for system reset and time=0.125 sec 	
		*P_WatchDog_Ctrl |= 0x0004; // set bit 2 for 0.125 sec
		*P_WatchDog_Ctrl |= 0x8000; // set bit 15 to enable watchdog
		while(1);	
 	}
#endif

	if(SYS_OFF!=SysGetState()) {
		initVoltage();	// get initial voltage before SACM_Init in BodyInit - may never run BodyInit()
		BodyInit();
	}
	
	USB_ISR_PTR = (long)USB_ISR;
	USB_INSERT_PTR = (long)USB_Insert_TimeOut;


	__asm__("irq on");
	__asm__("fiq on");
	
#ifdef	TB_CAN_WAKE
	*P_Clock_Ctrl |= 0x0200; // enable key-change interupt
	*P_MINT_Ctrl |= 0x1400;  //enable key-change interupt for black button and Play 
	*P_INT_Priority1 |= 0x8000; // set key-change to FIQ
#endif
		
	fs_init(); 	// should include call to IOKey_Initial() within BodyInit.c 
	 			// to flip on a transistor early enough to power the microSD card

	_devicemount(0);
	ChangeCodePage(UNI_ENGLISH);
		
	startUp();
	return 0;
}
