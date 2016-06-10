/*rhm
#define SYS_CREATEVAR
#define CREATE_BS_VAR
#define CREATGUIVAR
#define CREATEKEYDECODEVAR
rhm */
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"

#if	RTC_Key_TEST
#include "./Application/Include/TestPattern.h"
int RTCTest1=0,RTCTest2=0,KeyTest=0;
void APTest(void);
#endif

void BodyInit(void);
//rhmvoid fs_mount_all(void);

//rhmvoid Nor_WordWrite(unsigned int offset,unsigned int seg, int count,unsigned int *ptr);
//rhmvoid Nor_BlockErase(unsigned int blocknum, int, int);

#define	MX_BLOCK_START	9
#define	MX_BLOCK_END	23
#define	SST_BLOCK_START	2
#define	SST_BLOCK_END	16
#define	SST_MID			0xBF
#define	PROG_ADDR_START	0x40000L
#define	PROG_ADDR_END	0xB0000L
#define	NUM_WRITE_COUNT	((PROG_ADDR_END-PROG_ADDR_START)/BUF_SIZE)

#define	BUF_SIZE	(1024*4)

//unsigned int buffer[BUF_SIZE];
//rhm
#include ".\Driver\Include\USB\USBD_Define.h"
#include ".\Driver\Include\USB\USBD.h"

#include "./Reprog/USB_Flash_reprog.h"
#define C_USBDiskPlugIn 		0x01
#define C_USBDiskPlugOut  		0x02
#define C_USBPlugInTimeOut		0x03
#define C_USBDevicesmountOK    	0x00
#define C_USBDevicesUnmount    	0x05
#define C_USBHUnsearchDevices	0xfffe
#define C_USBHostError  		0xffff
typedef enum 
{
		USBDiskPlugIn  	= 		C_USBDiskPlugIn,
		USBDiskPlugOut 	=		C_USBDiskPlugOut,
		USBPlugInTimeOut = 		C_USBPlugInTimeOut,
		USBDevicesUnmount	= 	C_USBDevicesUnmount,
		USBDevicesmountOK		= C_USBDevicesmountOK,
		USBHUnsearchDevices = 	C_USBHUnsearchDevices,
		USBHostError	=		C_USBHostError
}USBHostType ;

extern USBHostType  USBHost_Flag;

extern unsigned long  usb_time_out;
#define USB_TIME_OUT 10
//rhm

int main (void)
{
//rhm never called in himem
	while (1);
}

//rhm from device.c
void wait(int t) { //t=time in msec
	unsigned int i;
	unsigned long j;
	unsigned long int cyclesPerMilliSecond = (long)(*P_PLLN & 0x3f) * 1000L;  // 96000 at 96MHz	
	const unsigned int cyclesPerNOP = 70; // cycles for each no-operation instruction
	const unsigned int NOPsPerMilliSecond = cyclesPerMilliSecond / cyclesPerNOP; // loop count per millisecond
	for (i = 0; i < t; i++) 
		for (j = 0; j < NOPsPerMilliSecond; j++)  
			asm("nop\n");  // a CPU no-op instruction to pass the time
}
//rhm
//rhm from systemintousb

void USB_Insert_TimeOut(void)
{
	int temp;

	usb_time_out += 1;
		temp=*P_TimeBaseA_Ctrl;
	*P_TimeBaseA_Ctrl=temp;	//clear timerA int flag	
	
	if( usb_time_out > USB_TIME_OUT ) {
		if(USBHost_Flag == C_USBDiskPlugIn){			
		   USBHost_Flag = C_USBPlugInTimeOut;					
		}else if (!USBD_PlugIn){		//Check USB plug in		
			R_USB_Suspend = 1;
		}
	*P_TimeBaseA_Ctrl = 0x0000;
	}
}
//rhm



