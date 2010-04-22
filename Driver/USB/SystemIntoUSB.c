/*-------------------------------------------------*
* Function		:
* Usage         :
* Version		:
* Return Code   :
* Revision      :
* Remark        :
*--------------------------------------------------*/
#define CREATEUSBVAR				
#define	USB_TIME_OUT	(128*60)

//for USB head file
#include "./system/include/system_head.h"
#include ".\driver\include\USB\USBD_Define.h"
#include ".\driver\include\USB\USBD.h"

//unsigned int R_Write_protect = 0;//add by haoyu for no err
extern unsigned int	_Nand_ErasePhysicalBlock(unsigned long BlockAddr);
extern void USB_Reset(void);
//extern void Sound_Stop( void );
extern void USB_ServiceLoop(unsigned int unUseLoop);
void USB_TimeOut_Disable(void);
void USB_TimeOut_Enable(void);
extern unsigned int	SetBadBlock(unsigned long BlockAddr);
unsigned int R_USB_DisplayTime;
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


#if SIMULATOR  //add by haoyu for no err 2006.2.6
int _NAND_ReadSector_USB(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr, unsigned int DMA_Channel) {};
int _NAND_WriteSector_USB(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr, unsigned int DMA_Channel,unsigned int DMA_RB_Flag) {};

#else
#endif
unsigned long  usb_time_out;
/////////////////////////////User setting initial value start//////////////////////////////////////////
//要与main.c中配置相同  
//#define USB_Nand_Part0_Offset	0x00002800
//#define USB_Nand_Part0_Size 	0x0000C800
//#define USB_Nand_Part1_Offset	0x0002F000
//#define USB_Nand_Part1_Size 	0x0004BE00
//#define USB_Nand_Part2_Offset	0x00000000
//#define USB_Nand_Part2_Size 	0x00000000


//要与main.c中配置相同
#define USB_Nand_Part0_Offset	0x00000000
#define USB_Nand_Part0_Size 	0x0003C000
#define USB_Nand_Part1_Offset	0x0003C000
#define USB_Nand_Part1_Size 	0x0003C000
#define USB_Nand_Part2_Offset	0x00000000
#define USB_Nand_Part2_Size 	0x00000000



str_USB_Lun_Info USB_Lun_Define[]=
{
//	{0x00, LunType_NandDrive	, USB_Nand_Part0_Offset,	USB_Nand_Part0_Size, 	LunStatus_Normal},
//	{0x01, LunType_NandDrive	, USB_Nand_Part1_Offset,	USB_Nand_Part1_Size, 	LunStatus_Normal},	
	{0x00, LunType_SDCardReader	, 0,	0, 	LunStatus_Normal},

};

int SystemIntoUDisk()
{

	int ret,i;
	SysDisableWaitMode(3);
	SysIntoHighSpeed();

	MaxLUN = -1;
	R_NAND_Present=0;
	R_SDC_Present=0;     //no have SD modify by haoyu 2007.1.29	

	for(i=0 ; i<10 ; i++)
	{
		ret = DrvSDCInitial();	
		if(ret==0)
		{
			R_SDC_Present=1;		// SD Card Initial Success
			MaxLUN++;  
			break;
		_deviceunmount(0);
		}
	}
	USB_TimeOut_Enable();		
	USB_Initial();
	USB_Reset();
	USB_ServiceLoop(1);

	*P_USBD_Config=0x00;
	*P_USBD_INTEN=0x00;	

    if(1 == R_SDC_Present)
    {
	    _devicemount(0);
	}
	SysEnableWaitMode( 3 );

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
	
	if( usb_time_out > 5 ) {
		if (!USBD_PlugIn)		//Check USB plug in
		{
			R_USB_Suspend=1;
		}
	*P_TimeBaseA_Ctrl = 0x0000;
	}
}


