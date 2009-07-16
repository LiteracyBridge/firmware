#ifndef USBRP
#define		CREAT_DRIVERLAYER_STRUCT
#define		CREATUSBHOSTVAR
#endif

#include	"typedef.h"
#include	"DriverLayer.h"
#include	"./Driver/Include/USBHost/USBHostMSDC.h"	
#include	"./Driver/Include/USBHost/USB_Host_RAM.h"


#define C_DISK1_FLAG	0x0001
#define C_DISK2_FLAG	0x0002
#define C_DISK3_FLAG	0x0004
#define C_DISK4_FLAG	0x0008

#ifdef USBRP
extern INT16 bUSB_Init /*rhm = -1*/;
extern INT16 bUSB_LUN_Init;

extern unsigned int arUSBHostBuffer[256];
extern unsigned int arUSBHostCSWBuffer[16];
#else
INT16 bUSB_Init = -1;
INT16 bUSB_LUN_Init;

unsigned int arUSBHostBuffer[256];
unsigned int arUSBHostCSWBuffer[16];

#endif  //USBRP

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

#ifdef USBRP
extern USBHOSTBUFFER gUSBHostBuffer;
extern unsigned long gBlk2BufMask;
extern unsigned int gSec2BlkSft;
extern unsigned int gBlk2BufCnt;
#else
USBHOSTBUFFER gUSBHostBuffer;
unsigned long gBlk2BufMask;
unsigned int gSec2BlkSft;
unsigned int gBlk2BufCnt;
#endif //USBRP

extern void MemPackByteCopyFarEE( ULONG  s ,  ULONG  d ,  UINT16 n);

INT16 USBHostBufferInit(UINT16 SectorSize)
{
    int ret = 0;
    
    gUSBHostBuffer.Flag = C_EMPTY;
    gUSBHostBuffer.Disk = 0;
    gUSBHostBuffer.BlkNo = 0;
    
    switch(SectorSize)
    {
    	case 1:				//sector size is 512 byte
    	#ifdef USBHOST_BUFFER_SIZE_8K
			gBlk2BufMask = 0xfffffff0;
			gSec2BlkSft = 0;
			gBlk2BufCnt = 16;
		#elif defined USBHOST_BUFFER_SIZE_4K
			gBlk2BufMask = 0xfffffff8;
			gSec2BlkSft = 0;
			gBlk2BufCnt = 8;
		#elif defined USBHOST_BUFFER_SIZE_2K
			gBlk2BufMask = 0xfffffffc;
			gSec2BlkSft = 0;
			gBlk2BufCnt = 4;
		#endif
    		break;
    		
    	case 2:				//sector size is 1024 byte
    	#ifdef USBHOST_BUFFER_SIZE_8K
			gBlk2BufMask = 0xfffffff8;
			gSec2BlkSft = 1;
			gBlk2BufCnt = 8;
		#elif defined USBHOST_BUFFER_SIZE_4K
			gBlk2BufMask = 0xfffffffc;
			gSec2BlkSft = 1;
			gBlk2BufCnt = 4;
		#elif defined USBHOST_BUFFER_SIZE_2K
			gBlk2BufMask = 0xfffffffe;
			gSec2BlkSft = 1;
			gBlk2BufCnt = 2;
		#endif		
    		break;
    		
    	case 4:				//sector size is 2048 byte
    	#ifdef USBHOST_BUFFER_SIZE_8K
			gBlk2BufMask = 0xfffffffc;
			gSec2BlkSft = 2;
			gBlk2BufCnt = 4;
		#elif defined USBHOST_BUFFER_SIZE_4K
			gBlk2BufMask = 0xfffffffe;
			gSec2BlkSft = 2;
			gBlk2BufCnt = 2;
		#elif defined USBHOST_BUFFER_SIZE_2K
			gBlk2BufMask = 0xffffffff;
			gSec2BlkSft = 2;
			gBlk2BufCnt = 1;
		#endif		
    		break;
    		
    	case 8:				//sector size is 4096 byte
    	#ifdef USBHOST_BUFFER_SIZE_8K
			gBlk2BufMask = 0xfffffffe;
			gSec2BlkSft = 3;
			gBlk2BufCnt = 2;
		#elif defined USBHOST_BUFFER_SIZE_4K
			gBlk2BufMask = 0xffffffff;
			gSec2BlkSft = 3;
			gBlk2BufCnt = 1;
		#elif defined USBHOST_BUFFER_SIZE_2K
			ret = -1;
		#endif		
    		break;
    	
    	default:
    		ret = -1;
    		break;
    }
    return ret;
}

INT16 USBHost_BufferFlush(void)
{
    INT16 ret;
    
    if(gUSBHostBuffer.Flag == (C_VALID + C_MODIFIED))
    {
    	ret = MSDC_Write2A(gUSBHostBuffer.BlkNo, gUSBHostBuffer.Buffer, 1, gUSBHostBuffer.Disk);
    	if(ret != 0)
    		return ret;
    }
    gUSBHostBuffer.Flag = C_EMPTY;
    return 0;
} 

INT16 USBHost_Initial_1(void)
{
	INT16 ret;
	UINT16	sectorsize;
	int trials;
	const int MAX_TRIALS = 10;
	
	if(bUSB_Init == -1)
	{
		ResetUSBHost = 0;
		bUSB_LUN_Init = 0;
		
		USBHostCSWBuffer=arUSBHostCSWBuffer;
		USBHostBuffer=arUSBHostBuffer;
		
		ptr_USBHostBuffer=USBHostBuffer;
		
		
		for (trials = 0; trials < MAX_TRIALS; trials++) {
			ret = DrvUSBHMSDCInitial();
			if (ret != 0)
				wait(300);
			else
				break;	
		}
		if(ret != 0) 		 // Initial Fail
		{
			USBHostBuffer = 0;
			USBHostCSWBuffer = 0;
			bUSB_Init = -1;
			return -1;
		}
		
		bUSB_Init = 0;
	}
	//debug 
/*
	BulkINEP = 0x81;
	BulkOUTEP = 2;
	INTINEP = 0x83;
	USBH_MaxLUN = 0;
	R_Device_Address = 1;
	InterfaceNumber = 0;
	DeviceClass = 8;
	InterfaceSubClass = 6;
	InterfaceProtocol = 0x50;
*/	
	//debug
	ret = DrvUSBHLUNInit(0);
	if(ret != 0)
	{
		bUSB_Init = -1;
		return ret;
	}
	
	bUSB_LUN_Init |= C_DISK1_FLAG;
	sectorsize = DrvUSBHGetSectorSize();
	ret = USBHostBufferInit(sectorsize);
	return ret;
}


INT16 USBHost_Uninitial_1(void)
{
	if(bUSB_Init == 0)
	{
		bUSB_LUN_Init &= (~C_DISK1_FLAG);
		if(bUSB_LUN_Init == 0)
		{
			DrvUSBHMSDCUninitial();
	
			USBHostBuffer = 0;
			USBHostCSWBuffer = 0;
			bUSB_Init = -1;
		}
	}
	
	return 0;
}

void USBHost_GetDrvInfo_1(struct DrvInfo* info)
{
	info->nSectors = 0xffffffff;		//unknow size
	info->nBytesPerSector = DrvUSBHGetSectorSize() << 9;
}

INT16 USBHost_ReadSector(UDWORD blkno , UWORD blkcnt ,  UINT32 buf, INT16 dsk)
{
//以下code为支持连续sector地址的读或者写
//支持的最大sector size为512 byte
	UINT16	i, j;
	INT16	ret;
	UINT32	blk;
	UINT16	cnt;

	if(1 == DrvUSBHGetSectorSize())		//512 byte的sector size
	{		
		i = blkcnt >> 6;
		j = blkcnt & 0x003f;
		
		while(i)
		{
			ret = MSDC_Read28(blkno, buf, 64, dsk);
			if(ret != 0)
				return ret;
				
			i--;
			buf += 64*256;
			blkno += 64;
		};
		
		if(j)
			ret = MSDC_Read28(blkno, buf, j, dsk);
		
		return ret;
	}
	else if(4 == DrvUSBHGetSectorSize())	//2k的sector size，通过buffer进行缓冲
	{
		while(blkcnt)
		{
			blk = blkno >> 2;
			if(blkno & 3)
			{
				if(!((gUSBHostBuffer.Flag & C_VALID) && (gUSBHostBuffer.BlkNo == blk) && (gUSBHostBuffer.Disk == dsk)))
				{
					ret = USBHost_BufferFlush();
				    if(ret != 0)
				    	return ret;
				    
				    ret = MSDC_Read28(blk, gUSBHostBuffer.Buffer, 1, dsk);
				    if(ret != 0)
		    			return ret;
				    
				    gUSBHostBuffer.Flag = C_VALID;
			    	gUSBHostBuffer.BlkNo = blk;
			    	gUSBHostBuffer.Disk = dsk;
				}	    	
		    	
		    	if(4 - (blkno & 3) > blkcnt)
		    		cnt = blkcnt;
		    	else
		    		cnt = 4 - (blkno & 3);
		    		
		    	MemPackByteCopyFarEE((UINT32)gUSBHostBuffer.Buffer + ((blkno & 3) << 8), (UINT32)buf, cnt << 8);
		    	
		    	blkcnt -= cnt;
		    	blkno += cnt;
		    	buf += (cnt << 8); 
			}
			else if(blkcnt < 4)
			{
				if(!((gUSBHostBuffer.Flag & C_VALID) && (gUSBHostBuffer.BlkNo == blk) && (gUSBHostBuffer.Disk == dsk)))
				{
					ret = USBHost_BufferFlush();
				    if(ret != 0)
				    	return ret;
				    
				    ret = MSDC_Read28(blk, gUSBHostBuffer.Buffer, 1, dsk);
				    if(ret != 0)
		    			return ret;
				    
				    gUSBHostBuffer.Flag = C_VALID;
			    	gUSBHostBuffer.BlkNo = blk;
			    	gUSBHostBuffer.Disk = dsk;
				}
						    	
		    	MemPackByteCopyFarEE((UINT32)gUSBHostBuffer.Buffer + ((blkno & 3) << 8), (UINT32)buf, blkcnt << 8);
		    	
		    	blkcnt = 0;
			}
			else
			{
				cnt = blkcnt >> 2;				
				i = cnt >> 4;
				j = cnt & 0x000f;
				while(i)
				{
					ret = MSDC_Read28(blk, buf, 16, dsk);
					if(ret != 0)
						return ret;
					
					i--;
					blk += 16;
					buf += 64*256;
					blkno += 64;
					blkcnt -= 64;
				}
				
				if(j)
				{
					ret = MSDC_Read28(blk, buf, j, dsk);
					cnt = j << 2;
					buf += (cnt << 8);
					blkno += cnt;
					blkcnt -= cnt;
				}
			}
		}
	}
	else				//other sector size is not support
		return -1;
		
	return 0;
/*
		blk = blkno >> 2;
		cnt = blkcnt >> 2;
		if(cnt >= 1)
		{
			//读大于2k的数据
			ret = USBHost_BufferFlush();
		    if(ret != 0)
		    	return ret;
			
			i = cnt >> 4;
			j = cnt & 0x000f;
			while(i)
			{
				ret = MSDC_Read28(blk, buf, 16, dsk);
				if(ret != 0)
					return ret;
				
				i--;
				buf += 64*256;
				blk += 16;
			}
			
			if(j)
				ret = MSDC_Read28(blk, buf, j, dsk);
			
			return ret;
		}
		else
		{
			//if the sector size is the multiple of 512 BYTE, read/write through a 2k BYTE buffer
			//if read blkno is in buffer, read direct from the buffer 
			if( (gUSBHostBuffer.Flag & C_VALID) && (gUSBHostBuffer.BlkNo == blk) && (gUSBHostBuffer.Disk == dsk) )
			{
				MemPackByteCopyFarEE((UINT32)gUSBHostBuffer.Buffer + ((blkno & C_BUF2SECMASK) << 8), (UINT32)buf, 0x100);
			}
		    else
		    //if read blkno is not in buffer, read data to global buffer, then copy the data to user buffer
		    {
		    	ret = USBHost_BufferFlush();
		    	if(ret != 0)
		    		return ret;
		    	
		    	ret = MSDC_Read28(blk, gUSBHostBuffer.Buffer, gBlk2BufCnt, dsk);
		    	if(ret != 0)
		    		return ret;
		    	
		    	gUSBHostBuffer.Flag = C_VALID;
		    	gUSBHostBuffer.BlkNo = blk;
		    	gUSBHostBuffer.Disk = dsk;
		    	
		    	MemPackByteCopyFarEE((UINT32)gUSBHostBuffer.Buffer + ((blkno & C_BUF2SECMASK) << 8), (UINT32)buf, 0x100);
		    }
		    return 0;
		}
	}
*/	
}

INT16 USBHost_WriteSector(UDWORD blkno , UWORD blkcnt ,  UINT32 buf, INT16 dsk)
{
//以下code为支持连续sector地址的读或者写
//支持的最大sector size为512 byte
	UINT16	i, j;
	INT16	ret;
	UINT32	blk;
	UINT16	cnt;

	if(1 == DrvUSBHGetSectorSize())		//512 byte的sector size
	{		
		i = blkcnt >> 6;
		j = blkcnt & 0x003f;
		
		while(i)
		{
			ret = MSDC_Write2A(blkno, buf, 64, dsk);
			if(ret != 0)
				return ret;
				
			i--;
			buf += 64*256;
			blkno += 64;
		};
		
		if(j)
			ret = MSDC_Write2A(blkno, buf, j, dsk);
		
		return ret;
	}
	else if(4 == DrvUSBHGetSectorSize())	//2k的sector size，通过buffer进行缓冲
	{
		while(blkcnt)
		{
			blk = blkno >> 2;
			if(blkno & 3)
			{
				if(!((gUSBHostBuffer.Flag & C_VALID) && (gUSBHostBuffer.BlkNo == blk) && (gUSBHostBuffer.Disk == dsk)))
				{
					ret = USBHost_BufferFlush();
				    if(ret != 0)
				    	return ret;
				    
				    ret = MSDC_Read28(blk, gUSBHostBuffer.Buffer, 1, dsk);
				    if(ret != 0)
		    			return ret;
				    
				    gUSBHostBuffer.Flag = C_VALID;
			    	gUSBHostBuffer.BlkNo = blk;
			    	gUSBHostBuffer.Disk = dsk;
				}	    	
		    	
		    	if(4 - (blkno & 3) > blkcnt)
		    		cnt = blkcnt;
		    	else
		    		cnt = 4 - (blkno & 3);
		    		
		    	MemPackByteCopyFarEE((UINT32)buf, (UINT32)gUSBHostBuffer.Buffer + ((blkno & 3) << 8), cnt << 8);
		    	gUSBHostBuffer.Flag |= C_MODIFIED;
		    	
		    	blkcnt -= cnt;
		    	blkno += cnt;
		    	buf += (cnt << 8); 
			}
			else if(blkcnt < 4)
			{
				if(!((gUSBHostBuffer.Flag & C_VALID) && (gUSBHostBuffer.BlkNo == blk) && (gUSBHostBuffer.Disk == dsk)))
				{
					ret = USBHost_BufferFlush();
				    if(ret != 0)
				    	return ret;
				    
				    ret = MSDC_Read28(blk, gUSBHostBuffer.Buffer, 1, dsk);
				    if(ret != 0)
		    			return ret;
				    
				    gUSBHostBuffer.Flag = C_VALID;
			    	gUSBHostBuffer.BlkNo = blk;
			    	gUSBHostBuffer.Disk = dsk;
				}
						    	
		    	MemPackByteCopyFarEE((UINT32)buf, (UINT32)gUSBHostBuffer.Buffer, blkcnt << 8);
		    	gUSBHostBuffer.Flag |= C_MODIFIED;
		    	
		    	blkcnt = 0;
			}
			else
			{
				cnt = blkcnt >> 2;				
				i = cnt >> 4;
				j = cnt & 0x000f;
				while(i)
				{
					ret = MSDC_Write2A(blk, buf, 16, dsk);
					if(ret != 0)
						return ret;
					
					i--;
					blk += 16;
					buf += 64*256;
					blkno += 64;
					blkcnt -= 64;
				}
				
				if(j)
				{
					ret = MSDC_Write2A(blk, buf, j, dsk);
					cnt = j << 2;
					buf += (cnt << 8);
					blkno += cnt;
					blkcnt -= cnt;
				}
			}
		}
	}
	else				//other sector size is not support
		return -1;
		
	return 0;


/*
	UINT16	i, j;
	INT16	ret;
	UINT32	blk;
	UINT16	cnt;

	if(!gSec2BlkSft)
	{	
		//如果sector size是512 byte的话，不使用2k的buffer
		i = blkcnt >> 6;
		j = blkcnt & 0x003f;
		
		while(i)
		{
			ret = MSDC_Write2A(blkno, buf, 64, dsk);
			if(ret != 0)
				return ret;
				
			i--;
			buf += 64*256;
			blkno += 64;
		};
		
		if(j)
			ret = MSDC_Write2A(blkno, buf, j, dsk);
		
		return ret;
	}
	else
	{
		//if the sector size is the multiple of 512 BYTE, read/write through a 2k BYTE buffer
		//if write blkno is in buffer, write direct to the buffer 
		blk = blkno >> 2;
		cnt = blkcnt >> 2;
		if(cnt >= 1)
		{
			//写大于2k的数据
			ret = USBHost_BufferFlush();
		    if(ret != 0)
		    	return ret;
			
			i = cnt >> 4;
			j = cnt & 0x000f;
			while(i)
			{
				ret = MSDC_Write2A(blk, buf, 16, dsk);
				if(ret != 0)
					return ret;
				
				i--;
				buf += 64*256;
				blk += 16;
			}
			
			if(j)
				ret = MSDC_Write2A(blk, buf, j, dsk);
			
			return ret;
		}
		else
		{
			//小于2k的sector size，通过buffer来操作
			if( (gUSBHostBuffer.Flag  & C_VALID) && (gUSBHostBuffer.BlkNo == blk) && (gUSBHostBuffer.Disk == dsk) )
			{
				MemPackByteCopyFarEE((UINT32)buf, (UINT32)gUSBHostBuffer.Buffer + ((blkno & C_BUF2SECMASK) << 8), 0x100);
				gUSBHostBuffer.Flag |= C_MODIFIED;
			}
		    else
		    //if write blkno is not in buffer, read data to global buffer, then copy the user data to global buffer
		    {
		    	ret = USBHost_BufferFlush();
		    	if(ret != 0)
		    		return ret;
		    	
		    	ret = MSDC_Read28(blk, gUSBHostBuffer.Buffer, gBlk2BufCnt, dsk);
		    	if(ret != 0)
		    		return ret;
		 
		    	MemPackByteCopyFarEE((UINT32)buf, (UINT32)gUSBHostBuffer.Buffer + ((blkno & C_BUF2SECMASK) << 8), 0x100);
		    	
		    	gUSBHostBuffer.Flag = C_VALID | C_MODIFIED;
		    	gUSBHostBuffer.BlkNo = blk;
		    	gUSBHostBuffer.Disk = dsk;
		    }
		    
		    return 0;	
		}
	}
*/
}

INT16 USBHost_ReadSector_1(UDWORD blkno , UWORD blkcnt ,  UINT32 buf)
{
	return USBHost_ReadSector(blkno , blkcnt ,  buf, 0);
}

INT16 USBHost_WriteSector_1(UDWORD blkno , UWORD blkcnt ,  UINT32 buf)
{
	return USBHost_WriteSector(blkno , blkcnt ,  buf, 0);
}

#else

//=========================================================================
INT16 USBHost_Initial_1(void)
{
	INT16 ret;
	
	if(bUSB_Init == -1)
	{
		USBHostCSWBuffer=arUSBHostCSWBuffer;
		USBHostBuffer=arUSBHostBuffer;
		ptr_USBHostBuffer=USBHostBuffer;
		
		ret = DrvUSBHMSDCInitial();
		
		if(ret != 0) 		 // Initial Fail
		{
			USBHostBuffer = 0;
			USBHostCSWBuffer = 0;
			bUSB_Init = -1;
			return -1;
		}
	
		bUSB_Init = 0;
	}
	
	return DrvUSBHLUNInit(0);
}

INT16 USBHost_Uninitial_1(void)
{
	if(bUSB_Init == 0)
	{
		DrvUSBHMSDCUninitial();

		USBHostBuffer = 0;
		USBHostCSWBuffer = 0;
		bUSB_Init = -1;
	}
	
	return 0;
}

void USBHost_GetDrvInfo_1(struct DrvInfo* info)
{
	info->nSectors = 0xffffffff;
	info->nBytesPerSector = DrvUSBHGetSectorSize() << 9;
}

INT16 USBHost_ReadSector_1(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf)
{
	UINT16	i, j;
	INT16	ret;
	
	i = blkcnt >> 6;
	j = blkcnt & 0x003f;
	
	while(i)
	{
		ret = MSDC_Read28(blkno, buf, 64, dsk);
		if(ret != 0)
			return ret;
			
		i--;
		buf += 64*256;
		blkno += 64;
	};
	
	if(j)
		ret = MSDC_Read28(blkno, buf, j, dsk);
	
	return ret;
	//return MSDC_Read28(blkno,buf, 1, 0);
}

INT16 USBHost_WriteSector_1(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf)
{
	UINT16	i, j;
	INT16	ret;
	
	i = blkcnt >> 6;
	j = blkcnt & 0x003f;
	
	while(i)
	{
		ret = MSDC_Write2A(blkno, buf, 64, dsk);
		if(ret != 0)
			return ret;
			
		i--;
		buf += 64*256;
		blkno += 64;
	};
	
	if(j)
		ret = MSDC_Write2A(blkno, buf, j, dsk);
	
	return ret;
	//return MSDC_Write2A(blkno,buf, 1, 0);
}

#endif		//end of SUPPROT_2K_BLOCK

#ifndef USBRP
struct Drv_FileSystem FS_USB_driver = {
	"USB" ,
	DEVICE_READ_ALLOW|DEVICE_WRITE_ALLOW ,
	USBHost_Initial_1 ,
	USBHost_Uninitial_1 ,
	USBHost_GetDrvInfo_1 ,
	USBHost_ReadSector_1 ,
	USBHost_WriteSector_1 ,
};
#endif //USBRP


