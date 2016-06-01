#define		CREAT_DRIVERLAYER_STRUCT

#include	"typedef.h"
#include	"DriverLayer.h"

#if NAND2_EN == 1

extern INT16 bNandInit;

INT16 NAND_Initial_2(void)
{	
	INT16 ret;
	
	if(bNandInit == -1)
	{
		ret = _Nand_Initial();	
		if(ret!=0)
		{
			bNandInit = -1;
			return ret;
		}
				
		ret = InitMapTable();
		if(ret!=0)
		{
			bNandInit = -1;
			return ret;
		}
		
		NAND_PowerOff_init();			
		bNandInit = 0;
	}
	return 0;
}

INT16 NAND_Uninitial_2(void)
{
	 return 0;
}

void NAND_GetDrvInfo_2(struct DrvInfo* info)
{
	info->nSectors = Nand_Part1_Size;
	info->nBytesPerSector = 512;
}

INT16 NAND_ReadSector_2(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf)
{
	return _NAND_ReadSector(blkno+Nand_Part1_Offset, blkcnt, buf);
}

INT16 NAND_WriteSector_2(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf)
{
	INT16 i, ret;

//	if((Nand_Part1_Mode&DEVICE_WRITE_ALLOW)==0)
//		return -1;
//
//	for(ret =0x00, i=0x00; i<blkcnt; i++, blkno++, buf += 0x100)
//	{		 
		ret = _NAND_WriteSector(blkno+Nand_Part1_Offset,blkcnt,buf);	// 每次只写一个扇区			
//		if(ret) 
//			break; 		// 失败，直接返回
//	}
	
	return ret;
}

struct Drv_FileSystem FS_NAND2_driver = {
	"NAND2" ,
	DEVICE_READ_ALLOW|DEVICE_WRITE_ALLOW ,
	NAND_Initial_2 ,
	NAND_Uninitial_2 ,
	NAND_GetDrvInfo_2 ,
	NAND_ReadSector_2 ,
	NAND_WriteSector_2 ,
};
#endif
