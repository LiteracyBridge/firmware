// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#define CREATEUSBVAR				
#define	USB_TIME_OUT	120			// timeout was  10s

#define USB_Host		0x01
#define USB_Devices		0x02

//for USB head file
#include "./system/include/system_head.h"
#include "./component/include/component_head.h"
#include "./Reprog/USBD_Define.h"
#include "./Reprog/USBD.h"
#include "./Reprog/USB_Flash_reprog.h"

#include "./Component/FS/usb_host/USB_Host_Constant.h"
#include "./Component/FS/usb_host/USBHostMSDC.h"
#include "./Application/Talkingbook/Include/talkingbook.h"
#include "./Application/Talkingbook/Include/device.h"
#include "./Application/Talkingbook/Include/containers.h"
#include "./Application/Talkingbook/Include/files.h"

//unsigned int R_Write_protect = 0;//add by haoyu for no err
extern unsigned int	_Nand_ErasePhysicalBlock(unsigned long BlockAddr);
extern void USB_Reset(void);
//extern void Sound_Stop( void );
extern void USB_ServiceLoop(unsigned int unUseLoop);
void USB_TimeOut_Disable(void);
void USB_TimeOut_Enable(void);
extern unsigned int	SetBadBlock(unsigned long BlockAddr);
extern void USB_ISR(void);
extern void USB_Insert_TimeOut(void);
//key add by haoyu 2007.4.19
//extern void EnableKeyChangInt(void);
extern void DisableKeyChangeProcess(void);
//RTC add by haoyu 2007.4.19
extern void DrvRTCIntEnable( void );
extern void DrvRTCIntDisable( void );
extern void DrvEnableReminderISR( void );
extern void DrvDisableReminderISR( void );
extern unsigned int CLOCK_RATE;
extern APP_IRAM int inUSBHostMode;

extern APP_IRAM unsigned int vCur_1;
extern void refuse_lowvoltage(int);


#if SIMULATOR  //add by haoyu for no err 2006.2.6
int _NAND_ReadSector_USB(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr, unsigned int DMA_Channel) {};
int _NAND_WriteSector_USB(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr, unsigned int DMA_Channel,unsigned int DMA_RB_Flag) {};

#else
#endif

#ifdef USBRP
extern unsigned long  usb_time_out;
extern unsigned int R_USB_DisplayTime;
#else
unsigned long  usb_time_out;
#endif
/////////////////////////////User setting initial value start//////////////////////////////////////////
//Ҫ��main.c��������ͬ  
//#define USB_Nand_Part0_Offset	0x00002800
//#define USB_Nand_Part0_Size 	0x0000C800
//#define USB_Nand_Part1_Offset	0x0002F000
//#define USB_Nand_Part1_Size 	0x0004BE00
//#define USB_Nand_Part2_Offset	0x00000000
//#define USB_Nand_Part2_Size 	0x00000000


//Ҫ��main.c��������ͬ
#define USB_Nand_Part0_Offset	0x00000000
#define USB_Nand_Part0_Size 	0x0003C000
#define USB_Nand_Part1_Offset	0x0003C000
#define USB_Nand_Part1_Size 	0x0003C000
#define USB_Nand_Part2_Offset	0x00000000
#define USB_Nand_Part2_Size 	0x00000000

#define C_USBDiskPlugIn 		0x01
#define C_USBDiskPlugOut  		0x02
#define C_USBPlugInTimeOut		0x03
#define C_USBDevicesmountOK    	0x00
#define C_USBDevicesUnmount    	0x05
#define C_USBHUnsearchDevices	0xfffe
#define C_USBHostError  		0xffff
typedef enum 
{
		T_USBDiskPlugIn  	= 		C_USBDiskPlugIn,
		T_USBDiskPlugOut 	=		C_USBDiskPlugOut,
		USBPlugInTimeOut = 		C_USBPlugInTimeOut,
		USBDevicesUnmount	= 	C_USBDevicesUnmount,
		USBDevicesmountOK		= C_USBDevicesmountOK,
		USBHUnsearchDevices = 	C_USBHUnsearchDevices,
		USBHostError	=		C_USBHostError
}USBHostType ;

#ifdef USBRP
extern USBHostType  USBHost_Flag;
#include "Application/TalkingBook/Include/device.h"
#include "Reprog/USBD_Define.h"
/*
str_USB_Lun_Info USB_Lun_Define[] =
{
//	{0x00, LunType_NandDrive	, USB_Nand_Part0_Offset,	USB_Nand_Part0_Size, 	LunStatus_Normal},
//	{0x01, LunType_NandDrive	, USB_Nand_Part1_Offset,	USB_Nand_Part1_Size, 	LunStatus_Normal},	
	{0x00, LunType_SDCardReader	, 0L,	0L, 	LunStatus_Normal, 0L},
	//RHM
	{0x01, LunType_RAM	, 0L,	(long)(30 * 1024), 	LunStatus_Normal, 0L},
	{0x02, LunType_NOR  , 0x30000L, (long)(128 * 1024), LunStatus_Normal, 0L},
	//RHM
}; */
#define N_USB_LUN 3

//

#include "./Reprog/USB_Flash_reprog.h"
extern int fl_size;
extern unsigned int REPROG_IN_Progress;
extern unsigned int RHM_USBreprogBuf_Full;
extern unsigned int *RHM_DEBUG_LAST_LBA;
extern unsigned int *RHM_DEBUG_LAST_WRITE;
extern flash *RHM_FlashPtr;
extern unsigned int *RHM_FlashBuf;
extern long USB_INSERT_PTR;
extern long USB_ISR_PTR;
extern void (*ReprogJump)();
extern str_USB_Lun_Info 	*FP_USB_Lun_Define[];
#else  //USBRP
unsigned int R_USB_DisplayTime;
USBHostType  USBHost_Flag;
str_USB_Lun_Info USB_Lun_Define[]=
{
//	{0x00, LunType_NandDrive	, USB_Nand_Part0_Offset,	USB_Nand_Part0_Size, 	LunStatus_Normal},
//	{0x01, LunType_NandDrive	, USB_Nand_Part1_Offset,	USB_Nand_Part1_Size, 	LunStatus_Normal},	
	{0x00, LunType_SDCardReader	, 0,	0, 	LunStatus_Normal},

};
#define N_USB_LUN 1
str_USB_Lun_Info 	*FP_USB_Lun_Define[N_USB_LUN];
#endif

/**
 * This tries to enter Device mode, ie, "system into usb-disk'.
 *  functionMask - a bitmask of the functions to be performed.
 *      0x0001 (SYSTEM_UDISK_INITIALIZE) Run the initialization code. If this is the only bit, return 1 after initialization.
 *      0x0002 (SYSTEM_UDISK_TRY_CLIENT_MODE) Try to enter USB mode, but time out if not entered in ~5 seconds.
 *      0x0004 (SYSTEM_UDISK_RUNLOOP) Try forever to enter USB mode, and remain in USB mode forever.  The timeout attempt
 *        happens first, so if both TRY_CLIENT_MODE and RUNLOOP are passed, a timeout is still possible.
 *
 * There are some convenience defines that match historical usage of this function:
 *      2 (USB_CLIENT_SETUP_ONLY) initialize usb hardware & software to become a client but return without running usb serviceloop
 *      1 (USB_CLIENT_SVC_LOOP_CONTINUOUS) run usb serviceloop continuously until usb is inactive
 *      0 (USB_CLIENT_SVC_LOOP_WITH_TIMEOUT) check for timeout, then run serviceloop continuously until usb is inactive
 *
 * Returns:
 *  0 - entered, then exited USB device mode. The only return value if USB_CLIENT_LOOP_CONTINUOUS is passed
 *  1 - called with USB_CLIENT_SETUP_ONLY, or with LOOP_WITH_TIMEOUT, and a key was pressed
 *  2 - a timeout or state machine error occurred
 */
int SystemIntoUDisk(unsigned int functionMask)
{
	extern flash *RHM_FlashPtr; //RHM , *FP_RHM_FlashPtr;
	int i, serviceLoopCount=0;
    long serviceLoopEntryTime, serviceLoopExitTime;
	char strLog[60];
#ifdef USBRP
	int fl_size = USB_Flash_init((flash *)0, 0);
	flash FL;
	int flash_execution_buf[fl_size];
	RHM_FlashPtr = &FL;

	if(vCur_1 < V_MIN_USB_VOLTAGE) {
		refuse_lowvoltage(0);
		return(2);
	}

	for(i=0; i<N_USB_LUN; i++) {
		FP_USB_Lun_Define[i] = &USB_Lun_Define[i];
		if( USB_Lun_Define[i].unLunType == LunType_NOR) {
			USB_Lun_Define[i].rhmLunData = (unsigned long) RHM_FlashPtr;
		}
	}
	FL.flash_exe_buf = (void *) &flash_execution_buf[0];
	USB_Flash_init(&FL, 1);

#endif
	SysDisableWaitMode(3);
	SetSystemClockRate(48);

	if(functionMask & SYSTEM_UDISK_INITIALIZE) {
        logString("USB CLient setup; unmount SD", ASAP, LOG_ALWAYS);
		R_NAND_Present=0;
		MaxLUN = 0;
		R_SDC_Present=1;
		_deviceunmount(0);

		USB_TimeOut_Enable();
		USB_Initial();
		USB_Reset();

		if(functionMask == SYSTEM_UDISK_INITIALIZE) {
			return(1);
		}
	}

	if(functionMask & SYSTEM_UDISK_TRY_CLIENT_MODE) {
		int tmp;
		long j;
		tmp = R_USB_State_Machine;
        serviceLoopEntryTime = getRTCinSeconds();
		for(j=0; j<100000; j++) {
            serviceLoopCount++;
			USB_ServiceLoop(0); // 0 -> no loop
			if(R_USB_State_Machine > 0 && R_USB_State_Machine <= SCSI_CSW_Stage) {
                serviceLoopExitTime = getRTCinSeconds();
				goto xxx;
			} else {
				KeyScan_ServiceLoop();
				if (!(j % 0x40)) {
					context.keystroke = keyCheck(0);
					if (context.keystroke)
						break;
				}
			}
		}
		if (tmp == 0 && R_USB_State_Machine == 0) { // Timeout.
			SetSystemClockRate(CLOCK_RATE);
			_devicemount(0);
            logString("USB CLient state = 0; remounted SD", ASAP, LOG_ALWAYS);
			return(2);
		}

		if(R_USB_State_Machine == 0xf5f5) { // Undocumented (and random looking!) USB machine state. Error?
			SetSystemClockRate(CLOCK_RATE);
			_devicemount(0);
            logString("USB CLient state = f5f5; remounted SD", ASAP, LOG_ALWAYS);
			return(2);
		}
		if(!(R_USB_State_Machine > 0 && R_USB_State_Machine <= SCSI_CSW_Stage)) {
            // This initialization will set R_USB_State_Machine to 0, so tmp *should* be 0. If the machine state is 0, then,
            // we should have taken the timeout branch above. If the state is > SCSI_CSW_Stage, it is an undocumented
            // state (but then, so is 0xf5f5), so we still shouldn't be here. Possibly a vestige of some earlier logic
            // that was mooted when the 100,000 loop, above, was added??
			SysEnableWaitMode(3);
			RHM_FlashPtr = 0;
			SetSystemClockRate(CLOCK_RATE);
			return 1;
		}
	}
xxx:
	if (LED_GREEN)
		setLED(LED_GREEN,FALSE);
	else // for USB before reading config file, or if config corrupted
		setLED(0x040,FALSE);
	if (LED_RED)
		setLED(LED_RED,TRUE);
	else // for USB before reading config file, or if config corrupted
		setLED(0x200,TRUE);

    if (functionMask & SYSTEM_UDISK_RUNLOOP) {
        USB_ServiceLoop(1);  // 1 -> loop until USB suspend
    }

	*P_USBD_Config=0x00;
	*P_USBD_INTEN=0x00;

    if(1 == R_SDC_Present)
    {
	    _devicemount(0);
        logString("USB CLient exit; remounted SD", ASAP, LOG_ALWAYS);
	}
	SysEnableWaitMode( 3 );

	RHM_FlashPtr = 0;

	strcpy(strLog, "returned from USB Device Mode");
    if (serviceLoopCount > 0) {
        strcat(strLog, ", loop count: ");
        longToStr(serviceLoopCount, strLog+strlen(strLog));
        strcat(strLog, "(");
        longToStr(serviceLoopEntryTime, strLog+strlen(strLog));
        strcat(strLog, "-");
        longToStr(serviceLoopExitTime, strLog+strlen(strLog));
        strcat(strLog, ")");
    }
    logString(strLog, ASAP, LOG_ALWAYS);

	if (LED_RED)
		setLED(LED_RED,FALSE);
	else // for USB before reading config file, or if config corrupted
		setLED(0x200,FALSE);
	SetSystemClockRate(CLOCK_RATE);
	return 0;
}

void USB_TimeOut_Enable(void)
{
	usb_time_out = 0;
	
	*P_TimeBaseA_Ctrl = 0xE001;
}

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

///////////////////////////////////////////////////////////

int setUSBHost(BOOL enter) {
	int trials, x;
	const int maxTrials = 20;
	int whichLED = LED_RED;
	
	if(vCur_1 < V_MIN_USB_VOLTAGE) {
		refuse_lowvoltage(0);
		return(2);
	}
	
	if (enter){		
		SetSystemClockRate(48);  //48MHz for Host mode
		//setLED(LED_GREEN,FALSE);
		setLED(LED_ALL, FALSE);
		for (trials = 0; trials < maxTrials; trials++) {
			setLED(whichLED,TRUE);
			x = _devicemount(1); 
			setLED(whichLED,FALSE);
			if (x == C_USBDevicesmountOK)
				break;
			whichLED = (whichLED == LED_RED) ? LED_GREEN : LED_RED;
			wait (1000);			
		}
		if (trials < maxTrials) {
			setLED(LED_RED, FALSE);
			setLED(LED_GREEN,TRUE);
			wait(1000);
			USBHost_Flag = C_USBDevicesmountOK;
		} else {
			setLED(LED_GREEN,FALSE);
			setLED(LED_RED, TRUE);
		}
		inUSBHostMode = 1;
	} else {
			//  unmount USB devices;  
			if(_deviceunmount(1) != 0x00)
				USBHost_Flag = C_USBDiskPlugOut;
			else
				USBHost_Flag = C_USBDevicesUnmount;
			*P_USBH_Config = 0;
			*P_USBH_INTEN  = 0;	
			SetSystemClockRate(CLOCK_RATE);  //set clock to non usb host value
			setLED(LED_RED,FALSE);
			inUSBHostMode = 0;
	}
	return 0;
}

void
HandleHostISR()
{
	extern void USBDiskPlugOut();
	extern void USBDiskPlugIn();

	unsigned int r1;
	
	r1 = *P_USBH_INT;
	
	if(r1 & 0x800) {
		*P_USBH_INT = r1;
		USBDiskPlugOut();
	} else if( r1 & 0x1) {
		*P_USBH_INT = r1;
		USBDiskPlugIn();
	} else {
		USBHostISR();
	}
	
}
