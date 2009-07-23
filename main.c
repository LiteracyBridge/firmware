#ifndef USBRP
#define SYS_CREATEVAR
#define CREATE_BS_VAR
#define CREATGUIVAR
#define CREATEKEYDECODEVAR
#endif

#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./Application/TalkingBook/Include/mainloop.h"

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
	
	fs_init();
	_devicemount(0);
	ChangeCodePage(UNI_ENGLISH);
	
#ifdef USBRP
	
	check_new_sd_flash();

	CLOCK_RATE = 8;  // start with 24MHz clock
	SetSystemClockRate(CLOCK_RATE);

	
//	SystemIntoUDisk();
	
#endif //USBRP
	
	mainLoop();
	return 0;
}
