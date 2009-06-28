#ifndef	__DRIVERLAYER_H__
#define	__DRIVERLAYER_H__

#include "typedef.h"

//定义不使用IRAM。在NAND BOOT的情况下不能使用IRAM（带初始值的全局变量）
//#define	__NOT_USE_IRAM

#define SD_EN	  1
#ifdef USBRP
#define NAND1_EN  0
#else
#define NAND1_EN  1
#endif
#define NAND2_EN  1
#define USB_EN    1                //使用usb host设备

#define NBLKDEV     (SD_EN + NAND1_EN + USB_EN)


#define DEVICE_READ_ALLOW	0x1
#define DEVICE_WRITE_ALLOW	0x2

//===========================================================================
//nand flash partition config
#define	NAND_PART0_OFFSET		0
#define	NAND_PART0_SIZE			0x12000//0x3c000ul
#define	NAND_PART0_MODE			(DEVICE_READ_ALLOW | DEVICE_WRITE_ALLOW)

#define	NAND_PART1_OFFSET		0x12000ul
#define	NAND_PART1_SIZE			0x26000ul
#define	NAND_PART1_MODE			(DEVICE_READ_ALLOW | DEVICE_WRITE_ALLOW)

#define	NAND_PART2_OFFSET		0x3c000ul
#define	NAND_PART2_SIZE			0
#define	NAND_PART2_MODE			(DEVICE_READ_ALLOW | DEVICE_WRITE_ALLOW)

#define	UNITAB_SECTOR_OFFSET	0x3c000ul
#define	NAND_RESERVE_OFFSET		0x3C500ul


//定义不使用IRAM。在NAND BOOT的情况下不能使用IRAM（带初始值的全局变量）
#ifdef __NOT_USE_IRAM
	#define CONST		const
#else
	#define CONST
#endif


struct DrvInfo {
	UINT32 nSectors;
	UINT16 nBytesPerSector;
};

struct Drv_FileSystem {
	INT8 Name[8];
	INT8 Status;
    INT16	(*Drv_Initial)(void);
    INT16	(*Drv_Uninitial)(void);
    void (*Drv_GetDrvInfo)(struct DrvInfo* info);
    INT16	(*Drv_ReadSector)(UINT32, UINT16, UINT32);
    INT16	(*Drv_WriteSector)(UINT32, UINT16, UINT32);
};

extern const  UINT32 Nand_Part0_Offset;
extern const  UINT32 Nand_Part0_Size;
extern const  UINT16 Nand_Part0_Mode;

extern const  UINT32 Nand_Part1_Offset;
extern const  UINT32 Nand_Part1_Size;
extern const  UINT16 Nand_Part1_Mode;

extern const  UINT32 Nand_Part2_Offset;
extern const  UINT32 Nand_Part2_Size;
extern const  UINT16 Nand_Part2_Mode;

extern const UINT32 Nand_Reserve_Offset;

#ifndef	CREAT_DRIVERLAYER_STRUCT
extern CONST	struct Drv_FileSystem FS_SD_driver;
extern CONST	struct Drv_FileSystem FS_NAND_driver;
extern CONST	struct Drv_FileSystem FS_NAND2_driver;
extern CONST	struct Drv_FileSystem FS_NAND3_driver;
extern CONST	struct Drv_FileSystem FS_USB_driver;
extern CONST	struct Drv_FileSystem FS_USB2_driver;
extern CONST	struct Drv_FileSystem FS_USB3_driver;
extern CONST	struct Drv_FileSystem FS_USB4_driver;
extern CONST	struct Drv_FileSystem FS_RAMDISK_driver;
#endif		//end of '#ifndef	CREAT_DRIVERLAYER_STRUCT'

#endif		//end of '#ifndef	__DRIVERLAYER_H__'
