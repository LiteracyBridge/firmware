//#define		CREAT_DRIVERLAYER_STRUCT
#include	"DriverLayer.h"
//NAND FLASH partition define
#ifndef USBRP
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

struct Drv_FileSystem *FileSysDrv[NBLKDEV];

#else
extern struct Drv_FileSystem *FileSysDrv[];

#endif //USBRP

extern int bUSB_Init;

//定义不使用IRAM。在NAND BOOT的情况下不能使用IRAM（带初始值的全局变量）

void FS_LoadDriver(void)
{	
	FileSysDrv[0] =&FS_SD_driver;//&FS_SD_driver;
	FileSysDrv[1] =&FS_USB_driver;
#if 0
	FileSysDrv[2] =&FS_NAND_driver;
#endif
//	bNandInit = -1;
//	bUSB_Init = -1;
}




