#define		CREAT_DRIVERLAYER_STRUCT

#include	"Component/Include/FS/typedef.h"
#include	"DriverLayer.h"
#include	"./Driver/Include/SD/SDCDriver.h"
#include "./driver/include/driver_head.h"


INT16 SD_Initial(void)
{
	int i, ret;
	for(i=0;i<5;i++) {
		ret = DrvSDCInitial();
		if(ret == 0)
			break;
	}
	return(ret);
}

INT16 SD_Uninitial(void)
{
	 return 0;
}

void SD_GetDrvInfo(struct DrvInfo* info)
{
	info->nSectors = DrvSDCGetSize();
	info->nBytesPerSector = 512;
}

INT16 SD_ReadSector(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf)
{
	//return DrvSDCReadSector(blkno , buf);	
	return DrvSDCRead(blkno, buf, blkcnt);
}

INT16 SD_WriteSector(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf)
{		
	return  DrvSDCWrite(blkno, buf, blkcnt);	
}

const struct Drv_FileSystem FS_SD_driver = {
	"SD" ,
	DEVICE_READ_ALLOW|DEVICE_WRITE_ALLOW ,
	SD_Initial ,
	SD_Uninitial ,
	SD_GetDrvInfo ,
	SD_ReadSector ,
	SD_WriteSector ,
};

