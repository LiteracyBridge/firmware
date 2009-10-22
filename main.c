// Copyright 2009 Literacy Bridge
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
#include "./Application/TalkingBook/Include/startup.h"

extern long USB_ISR_PTR;
extern long USB_INSERT_PTR;
extern void USB_ISR(), USB_Insert_TimeOut();

void BodyInit(void);
extern unsigned int CLOCK_RATE;

int main (void)
{
	if(SYS_OFF!=SysGetState())
		BodyInit();
	
	USB_ISR_PTR = (long)USB_ISR;
	USB_INSERT_PTR = (long)USB_Insert_TimeOut;


	__asm__("irq on");
	__asm__("fiq on");
	
	fs_init(); 	// should include call to IOKey_Initial() within BodyInit.c 
	 			// to flip on a transistor early enough to power the microSD card

	_devicemount(0);
	ChangeCodePage(UNI_ENGLISH);
		
	startUp();
	return 0;
}
