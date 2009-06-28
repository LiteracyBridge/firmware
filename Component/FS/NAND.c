#define		CREAT_DRIVERLAYER_STRUCT

#include	"typedef.h"
#include	"DriverLayer.h"
#include	".\Driver\Include\NandDriver\Nand2_Oper.h"


extern INT16	bNandInit;

/************************************************************************/
/* NAND_Initial 
func:	
		对物理器件进行使能
		使用复位指令，使器件进行强制复位动作
		初始化 驱动层 的转换表

input:	void

output:	0		成功
		－1		失败
   
note:	初始化成功后，设置了一个全局变量 bNandInit 

                                                                  */
/************************************************************************/
INT16 NAND_Initial(void)
{
	return 0;
}

INT16 NAND_Uninitial(void)
{

	return 0;
}

void NAND_GetDrvInfo(struct DrvInfo* info)
{
	info->nSectors = Nand_Part0_Size;
	info->nBytesPerSector = 512;
}

INT16 NAND_ReadSector(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf)
{
	return	_NAND_ReadSector(blkno + Nand_Part0_Offset, blkcnt, buf);
}

/************************************************************************/
/*		NAND_WriteSector
func:	往物理介质上写入数据
input:	
output:	0		成功
		其他值	失败

note:	增加了对电脑是否可以格式化的控制
		当定义了 _PC_UNABLE_FORMAT_UDISK 时，禁止了对逻辑盘 逻辑0 扇的写入动作
		PC 不能对U盘进行格式化  
                                                                     */
/************************************************************************/
INT16 NAND_WriteSector(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf)
{
	return _NAND_WriteSector(blkno + Nand_Part0_Offset, blkcnt, buf);	
}

CONST struct Drv_FileSystem FS_NAND_driver = {
	"NAND" ,
	DEVICE_READ_ALLOW|DEVICE_WRITE_ALLOW ,
	NAND_Initial ,
	NAND_Uninitial ,
	NAND_GetDrvInfo ,
	NAND_ReadSector ,
	NAND_WriteSector ,
};

