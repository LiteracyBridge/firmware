// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifdef USBRP

//from main.c

#define SYS_CREATEVAR
#define CREATE_BS_VAR
#define CREATGUIVAR
#define CREATEKEYDECODEVAR

#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"


//from USB.c
#define		CREAT_DRIVERLAYER_STRUCT
#define		CREATUSBHOSTVAR

#include	"component/include/fs/typedef.h"
#include	"component/include/fs/DriverLayer.h"
#include	"./Driver/Include/USBHost/USBHostMSDC.h"	
#include	"./Driver/Include/USBHost/USB_Host_RAM.h"


#define C_DISK1_FLAG	0x0001
#define C_DISK2_FLAG	0x0002
#define C_DISK3_FLAG	0x0004
#define C_DISK4_FLAG	0x0008

INT16 bUSB_Init = -1;
INT16 bUSB_LUN_Init;

unsigned int arUSBHostBuffer[256];
unsigned int arUSBHostCSWBuffer[16];

#define USBHOST_SUPPROT_2K_BLOCK

#define USBHOST_BUFFER_SIZE_2K 		//netac need this

//=========================================================================
#ifdef	USBHOST_SUPPROT_2K_BLOCK

#ifdef USBHOST_BUFFER_SIZE_8K
	#define C_BUFFERSIZE				0x1000
	#define C_BUF2SECMASK				0x0f		//one buffer can load 16 sectors
#elif defined USBHOST_BUFFER_SIZE_4K
	#define C_BUFFERSIZE				0x800
	#define C_BUF2SECMASK				0x07		//one buffer can load 8 sectors
#elif defined USBHOST_BUFFER_SIZE_2K
	#define C_BUFFERSIZE				0x400		//2k
	#define C_BUF2SECMASK				0x03		//one buffer can load 4 sectors
#endif

#define C_EMPTY						0
#define C_VALID						1
#define C_MODIFIED					2

typedef struct 
{
	unsigned int Flag;	
	unsigned int Disk;				//usb host disk num, is 0, 1, 2...
	unsigned long BlkNo;			//the block number
	unsigned int Buffer[C_BUFFERSIZE];
}USBHOSTBUFFER;

USBHOSTBUFFER gUSBHostBuffer;
unsigned long gBlk2BufMask;
unsigned int gSec2BlkSft;
unsigned int gBlk2BufCnt;

#endif		//end of SUPPROT_2K_BLOCK

extern INT16 USBHost_Initial_1();
extern INT16 USBHost_Uninitial_1();
extern void USBHost_GetDrvInfo_1();
extern INT16 USBHost_ReadSector_1();
extern INT16 USBHost_WriteSector_1();


struct Drv_FileSystem FS_USB_driver = {
	"USB" ,
	DEVICE_READ_ALLOW|DEVICE_WRITE_ALLOW ,
	USBHost_Initial_1 ,
	USBHost_Uninitial_1 ,
	USBHost_GetDrvInfo_1 ,
	USBHost_ReadSector_1 ,
	USBHost_WriteSector_1 ,
};

// from SystemIntoUSB.c

unsigned long  usb_time_out;

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

#include "Reprog/USBD_Define.h"
#include "Reprog/USBD.h"

str_USB_Lun_Info USB_Lun_Define[] =
{
//	{0x00, LunType_NandDrive	, USB_Nand_Part0_Offset,	USB_Nand_Part0_Size, 	LunStatus_Normal},
//	{0x01, LunType_NandDrive	, USB_Nand_Part1_Offset,	USB_Nand_Part1_Size, 	LunStatus_Normal},	
	{0x00, LunType_SDCardReader	, 0L,	0L, 	LunStatus_Normal, 0L},
	//RHM
	{0x01, LunType_RAM	, 0L,	(long)(30 * 1024), 	LunStatus_Normal, 0L},
	{0x02, LunType_NOR  , 0x30000L, (long)(128 * 1024L), LunStatus_Normal, 0L},
	//RHM
}; 
str_USB_Lun_Info 	*FP_USB_Lun_Define[N_USB_LUN];

#include "./Reprog/USB_Flash_reprog.h"

// see USBD.inc for these vars
//
//USBHostType  USBHost_Flag;
//int fl_size;
//unsigned int REPROG_IN_Progress;
//unsigned int RHM_USBreprogBuf_Full;
//unsigned int *RHM_DEBUG_LAST_LBA;
//unsigned int *RHM_DEBUG_LAST_WRITE;
//flash *RHM_FlashPtr;
//unsigned int *RHM_FlashBuf;
//long USB_INSERT_PTR;
//long USB_ISR_PTR;
//void (*ReprogJump)() = 0;


// frome (replacing) DrvStruct.c

#define		CREAT_DRIVERLAYER_STRUCT
//#include	"DriverLayer.h"
//NAND FLASH partition define
const  UINT32 Nand_Part0_Offset = NAND_PART0_OFFSET;
const  UINT32 Nand_Part0_Size = NAND_PART0_SIZE;
const  UINT16 Nand_Part0_Mode = NAND_PART0_MODE;

const  UINT32 Nand_Part1_Offset = NAND_PART1_OFFSET;
const  UINT32 Nand_Part1_Size = NAND_PART1_SIZE;
const  UINT16 Nand_Part1_Mode = NAND_PART1_MODE;

const  UINT32 Nand_Part2_Offset = NAND_PART2_OFFSET;
const  UINT32 Nand_Part2_Size = NAND_PART2_SIZE;
const  UINT16 Nand_Part2_Mode = NAND_PART2_MODE;

const UINT32 Nand_Reserve_Offset = NAND_RESERVE_OFFSET;

int	bNandInit;
extern int bUSB_Init;

//定义不使用IRAM。在NAND BOOT的情况下不能使用IRAM（带初始值的全局变量）

struct Drv_FileSystem *FileSysDrv[NBLKDEV];

// from FS_DS.c

int SACMFileHandle = -1;
int MP3FileHandle = -1;			//added by chengye 2007/4/23
unsigned int DefaultBitRate = 32000;

#ifdef S320TTS_Used
int DataBase_fp = -1;			//added by chengye 2007/5/14
#endif

unsigned long MP3FATReadBuffer;		//从文件系统中读取数据暂时到Buffer中

// from FS_user_nos.c

#include ".\System\include\system\GPL162002.h"
#include ".\Component\Include\FS\typedef.h"  // for BOOL

#define	C_SEEK_SPEEDUP_SUPPORT

#ifdef C_SEEK_SPEEDUP_SUPPORT
int				gSeekSpeedupBufferFlag;
unsigned int	gSeekSpeedupBuffer[512];
#endif


//from scsi_table/asm
/*
_DeviceVersionInfo:
	.dw		0x00	//0~3: IC body
	.dw		0x16
	.dw		0x20
	.dw		0x02
	.dw		0xFF	//4~7: IC Version, 0xFFFFFF means mask rom
	.dw		0x00
	.dw		0x00
	.dw		0x00
	.dw		0x09	//Year
	.dw		0x05	//Month
	.dw		0x10	//Day
_DeviceVersionInfo_End:
*/
#include "application\talkingbook\include\talkingbook.h"
__attribute__((section(".code"))) char DeviceVersionInfo[11] = VERSION;
#endif //USBRP
