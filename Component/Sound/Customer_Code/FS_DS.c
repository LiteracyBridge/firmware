//==============================================
//include head file
//==============================================
#include "Component\Include\Component_head.h"
#include ".\System\Include\System_head.h"

#include "math.h"

#ifdef USBRP
extern int SACMFileHandle;
extern int MP3FileHandle;			//added by chengye 2007/4/23
extern unsigned int DefaultBitRate;

#ifdef S320TTS_Used
extern int DataBase_fp;			//added by chengye 2007/5/14
#endif

#else //USBRP
int SACMFileHandle = -1;
int MP3FileHandle = -1;			//added by chengye 2007/4/23
unsigned int DefaultBitRate = 32000;

#ifdef S320TTS_Used
int DataBase_fp = -1;			//added by chengye 2007/5/14
#endif

#endif //USBRP

#ifdef USBRP
	extern unsigned long MP3FATReadBuffer;		//从文件系统中读取数据暂时到Buffer中
#else //USBRP

#ifdef MP3_Used
	#ifdef MP3_FATREADBUFFER
	extern unsigned long MP3FATReadBuffer;		//从文件系统中读取数据暂时到Buffer中
	#endif
#else
#ifdef MP3_FATREADBUFFER
	unsigned long MP3FATReadBuffer;		//从文件系统中读取数据暂时到Buffer中
	#endif
#endif

#endif //USBRP

//For Record to FAT variables:
#define C_RECFATBUFFERSIZE		1024	//Record录音FAT的Buffer一半大小, 表示需要更换Buffer的Size.
#define C_RECFATFIRSTBUFFER		1		//表示现在将RecordFAT第一个Buffer的数据写入Nand
#define C_RECFATSECONDBUFFER	2		//表示现在将RecordFAT第二个Buffer的数据写入Nand
extern unsigned long RecordFATBuffer;		//For Record To NandFAT时作为缓冲Buffer
extern unsigned long CurSACMEncodeCount;

__attribute__((section(".code")))unsigned int SACM_A4800Mode[7] = { 32000, 36000, 40000, 44000, 48000, 52000, 56000 };

extern unsigned long User_GetCurDecodeLength(void);
void User_SetDecodeLength( unsigned long Decode_Length );

//========================================================
// Function Name:	NFAT_DS_GetData
// Syntax:		NFAT_DS_GetData(LPTR dest_addr,unsigned int length)
// Purpose: 	fetch resource data from FAT to Decode_In_Buffer for sacm(except MP3)			 
// Note:		NO
// Parameter : 	LPTR dest_addr: destination address
//				unsigned int length: length
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int NFAT_DS_GetData(LPTR dest_addr,unsigned int length)
{
//	unsigned int w;
	unsigned int ret;

	ret = read( SACMFileHandle, (unsigned long)dest_addr<<1,length<<1);
	if(ret == 0xffff)
	{
		//while(1);
		return -1;
	}
	else
	{
		return ret >> 1;
	}
}


#ifdef MP3_FATREADBUFFER
//========================================================
// Function Name:	Nand_GetFAT_MP3
// Syntax:		Nand_GetFAT_MP3(void)
// Purpose: 	read data from FAT to MP3NandBuffer			 
// Note:		must promise that there's 2048 words buffer is available.
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void Nand_GetFAT_MP3( void )
{
	unsigned int ret;
	unsigned long ultempBuffer;
//用两个PingPang_Buffer来控制MP3对文件的操作比较好，在穿过一个Buffer的时候开始变换Buffer，每次
//读取文件的长度固定为1K Words.
	if(C_MP3FATFIRSTBUFFER == uiMP3WriteFlag)	//Read指针已经到了Buffer的Middle位置，可以读取数据
	{											//每次读一个Buffer的大小(1024 words)
		ret = read(MP3FileHandle, (unsigned long)&MP3FATReadBuffer << 1, C_MP3FATBUFFERSIZE << 1);
		if(ret < 0)
		{
			SACM_Stop();
		}
		else
		{
			uiMP3WriteFlag = C_MP3FIRSTBUFFERFULL;						//以免重入
			MP3Filelength += C_MP3FATBUFFERSIZE << 1;			//Only for mp3 caculate.
		}
	}
	else if(C_MP3FATSECONDBUFFER == uiMP3WriteFlag)
	{
		ultempBuffer = &MP3FATReadBuffer;
		ultempBuffer += C_MP3FATBUFFERSIZE;
		ret = read( MP3FileHandle, (unsigned long)ultempBuffer << 1, C_MP3FATBUFFERSIZE << 1);
		if(ret < 0)
		{
			SACM_Stop();
		}
		else
		{
			uiMP3WriteFlag = C_MP3SECONDBUFFERFULL;						//以免重入
			MP3Filelength += C_MP3FATBUFFERSIZE << 1;			//Only for mp3 caculate.
		}
	}
}
#endif		//End of MP3_FATREADBUFFER definition

//========================================================
// Function Name:	NFAT_DS_GetData_MP3
// Syntax:		NFAT_DS_GetData_MP3( LPTR dest_addr,unsigned int length )
// Purpose: 	read data from MP3 FAT buffer to Decode_In_Buffer			 
// Note:		NO
// Parameter : 	LPTR dest_addr: destination address
//				unsigned int length: length
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int NFAT_DS_GetData_MP3( LPTR dest_addr,unsigned int length )
{
	int ret;
#ifdef MP3_FATREADBUFFER
	unsigned long ulTemplength, ulTempRead;
	int iTemp, iTemplength, iTemplength1, * pDestBufer;
	unsigned long pTempbuffer;

	//memcpy(dest_addr, pMP3Readptr, length);
//	pDestBufer ＝ &dest_addr;
	if(1 == MP3ReadModeFlag)			//已经得到MP3文件的Frame起始，可以通过FATBuffer缓冲读取.
	{
		ulTempRead = pMP3Readptr;
		ulTemplength = pMP3Readptr + length;
		pTempbuffer = &MP3FATReadBuffer;
		if( (pMP3Readptr <= C_MP3FATBUFFERSIZE << 1) && (ulTemplength > C_MP3FATBUFFERSIZE << 1) )
		{
			iTemplength = (C_MP3FATBUFFERSIZE << 1) - pMP3Readptr;
			iTemplength1 = iTemplength;
			if(iTemplength != 0)
			{
				memcpy((int)dest_addr, (int)(pTempbuffer + pMP3Readptr), iTemplength);
			}
			pMP3Readptr = 0;
			iTemplength = length - iTemplength;
			if(iTemplength != 0)
			{
				memcpy((int)(dest_addr + iTemplength1), (int)(pTempbuffer + pMP3Readptr), iTemplength);
			}
			pMP3Readptr += iTemplength;
		}
		else
		{
			iTemplength = length;
			if(iTemplength != 0)
			{
				memcpy((int)dest_addr, (int)(pTempbuffer + pMP3Readptr), iTemplength);
			}
			pMP3Readptr += iTemplength;
		}
		if((ulTemplength > C_MP3FATBUFFERSIZE) && (ulTempRead <= C_MP3FATBUFFERSIZE))//第一个Buffer已经读完
		{
			uiMP3WriteFlag = C_MP3FATFIRSTBUFFER;		//可以向第一个Buffer里写数据了
		}
		else if((ulTempRead <= C_MP3FATBUFFERSIZE << 1) && (ulTemplength > C_MP3FATBUFFERSIZE << 1))//第二个Buffer读完
		{
			uiMP3WriteFlag = C_MP3FATSECONDBUFFER;		//可以向第二个Buffer里写数据了
		}
	}
	else if(0 == MP3ReadModeFlag)				//找寻FS和MP3有效帧的起始
	{
		ret = read( MP3FileHandle,(unsigned long)dest_addr<<1,length<<1);
		if(ret < 0)
		{
			////while(1);
			return -1;
		}
		else
		{
			MP3Filelength += length << 1;			//Only for mp3 caculate.
		}
	}
//delete by chengye 2007/7/17	
//	if((1 == MP3DecodeErrorFlag) && ((C_MP3FATFIRSTBUFFER == uiMP3WriteFlag) || (C_MP3FATSECONDBUFFER == uiMP3WriteFlag)))//当缓冲Buffer读空并且解码出错的时候调用
//	{
//		Nand_GetFAT_MP3();
//	}
	return length;
#else
	ret = read( MP3FileHandle,(unsigned long)dest_addr<<1,length<<1);
	if(ret == 0xffff)
	{
		SACM_Stop();
		return -1;
		//while(1);
	}
	else
	{
		MP3Filelength += ret;			//Only for mp3 caculate.
		return ret >> 1;
	}
#endif			//End of MP3_FATREADBUFFER definition
}
//End of MP3_FATREADBUFFER definition

//========================================================
// Function Name:	NFAT_DS_WriteData
// Syntax:		NFAT_DS_WriteData(LPTR dest_addr,unsigned int length)
// Purpose: 	write encoded data from Encode_Out_Buffer to FAT 			 
// Note:		NO
// Parameter : 	LPTR dest_addr: destination address
//				unsigned int length: length
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void NFAT_DS_WriteData(LPTR dest_addr,unsigned int length)
{
	int ret;
	
//	*P_IOB_Dir |= 0x02;
//	*P_IOB_Attrib |= 0x02;
//	*P_IOB_Data &= 0xfffd;

	ret = write( SACMFileHandle,(unsigned long)dest_addr<<1,length<<1);

//	*P_IOB_Data |= 0x02;
	if (ret == 0xffff)
	{
//		while(1);
		return -1;
	}
}

//========================================================
// Function Name:	NFAT_DS_WriteData_Rec
// Syntax:		NFAT_DS_WriteData_Rec(LPTR dest_addr,unsigned int length)
// Purpose: 	write encoded data from Encode_Out_Buffer to FATBuffer 			 
// Note:		NO
// Parameter : 	LPTR dest_addr: destination address
//				unsigned int length: length
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void NFAT_DS_WriteData_Rec(LPTR dest_addr,unsigned int length)
{
	unsigned long ulTemplength, ulTempWrite;
	int ret, iTemp, iTemplength, iTemplength1;
	unsigned long pTempbuffer;

	ulTempWrite = pRecWriteptr;
	ulTemplength = pRecWriteptr + length;
	pTempbuffer = &RecordFATBuffer;
	if( (pRecWriteptr < C_RECFATBUFFERSIZE << 1) && (ulTemplength >= C_RECFATBUFFERSIZE << 1) )
	{
		iTemplength = (C_RECFATBUFFERSIZE << 1) - pRecWriteptr;
		iTemplength1 = iTemplength;
		if(iTemplength != 0)
		{
			memcpy((int)(pTempbuffer + pRecWriteptr), (int)dest_addr, iTemplength);
		}
		pRecWriteptr = 0;
		iTemplength = length - iTemplength;
		if(iTemplength != 0)
		{
			memcpy((int)(pTempbuffer + pRecWriteptr), (int)(dest_addr + iTemplength1), iTemplength);
		}
		pRecWriteptr += iTemplength;
	}
	else
	{
		iTemplength = length;
		if(iTemplength != 0)
		{
			memcpy((int)(pTempbuffer + pRecWriteptr), (int)dest_addr, iTemplength);
		}
		pRecWriteptr += iTemplength;
	}
	if((ulTempWrite < C_RECFATBUFFERSIZE)&&(ulTemplength >= C_RECFATBUFFERSIZE))////means first buffer has been empty by read
	{
		uiRecWriteFlag = C_RECFATFIRSTBUFFER;		//可以将第一个Buffer的数据写入Nand了
	}
	else if((ulTempWrite < C_RECFATBUFFERSIZE << 1) && (ulTemplength >= C_RECFATBUFFERSIZE << 1))//means second buffer has been empty by read
	{
		uiRecWriteFlag = C_RECFATSECONDBUFFER;		//可以将第二个Buffer的数据写入Nand了
	}
}

//========================================================
// Function Name:	Nand_WriteFAT_Rec
// Syntax:		Nand_WriteFAT_Rec(void)
// Purpose: 	write encoded data from FATBuffer to NandFAT 			 
// Note:		NO
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void Nand_WriteFAT_Rec(void)
{
	unsigned int ret;
	unsigned long ultempBuffer;
	
	if(C_RECFATFIRSTBUFFER == uiRecWriteFlag)	//Read指针已经到了Buffer的Middle位置，可以读取数据
	{											//每次读一个Buffer的大小(1024 words)
		uiRecWriteFlag = 0;						//avoid re-entry
		ret = write( SACMFileHandle, (unsigned long)&RecordFATBuffer << 1, C_RECFATBUFFERSIZE << 1);
		if(ret < 0)
		{
			//here means write error, user should do some solution by himself
			SACM_Stop();
		}
		if(C_RECFATFIRSTBUFFER == uiRecWriteFlag)
		{
//			while(1);
			return -1;
		}
	}
	else if(C_RECFATSECONDBUFFER == uiRecWriteFlag)
	{
		uiRecWriteFlag = 0;						//avoid re-entry
		ultempBuffer = &RecordFATBuffer;
		ultempBuffer += C_RECFATBUFFERSIZE;
		ret = write(SACMFileHandle, (unsigned long)ultempBuffer << 1, C_RECFATBUFFERSIZE << 1);
		if(ret < 0)
		{
			//here means write error, user should do some solution by himself
			SACM_Stop();
		}
		if(C_RECFATSECONDBUFFER == uiRecWriteFlag)
		{
//			while(1);
			return -1;
		}
	}
}


//========================================================
// Function Name:	SACMGet_A1800FAT_Mode
// Syntax:		SACMGet_A1800FAT_Mode( int handle )
// Purpose: 	Get the Mode of the A1800, the BitRate can be read from the Mode Flag
//				The user can get other A1800 infomations according to the return value. 			 
// Note:		NO
// Parameter : 	int handle: file handle
// Return:		the bitrate of the A1800
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
unsigned int SACMGet_A1800FAT_Mode( int handle, int offset )
{
	unsigned int uiTempBuf[3];
	unsigned long ulTempAddr;

	if( handle < 0 )
	{
		return RTN_FILEERROR;
	}
	lseek( handle, offset, SEEK_SET );			//Seek to the start of the file input
	ulTempAddr = ( unsigned long )uiTempBuf;
	read( handle, ulTempAddr << 1, 6);		//Read the first three words to get the Mode Flag and length
	lseek( handle, offset, SEEK_SET );			//Seek to the start of the file input when get over
	SACM_A1800_Mode = RVUI( uiTempBuf + 2 );	//Get the BitRate and put it into the globle variable.

	return SACM_A1800_Mode;
}

//========================================================
// Function Name:	SACMGet_A4800FAT_Mode
// Syntax:		SACMGet_A4800FAT_Mode( int handle )
// Purpose: 	Get the Mode of the A4800, the BitRate can be read from the Mode Flag
//				The user can get other A4800 infomations according to the return value. 			 
// Note:		NO
// Parameter : 	int handle: file handle
// Return:		the bitrate of the A1800
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
unsigned int SACMGet_A4800FAT_Mode( int handle )
{
	unsigned int uiID, uiTempBuf[3];
	unsigned long ulTempAddr;

	if( handle < 0 )
	{
		return RTN_FILEERROR;
	}
	lseek( handle, 0, SEEK_SET );			//Seek to the start of the file input
	ulTempAddr = ( unsigned long )uiTempBuf;
	read( handle, ulTempAddr << 1, 6);		//Read the first three words to get the Mode Flag and length
	lseek( handle, 0, SEEK_SET );			//Seek to the start of the file input when get over
	uiID = RVUI( uiTempBuf + 2 );			//Get the BitRate ID.
	SACM_A4800_Mode = SACM_A4800Mode[uiID];//Put it into the globle variable.

	return SACM_A4800_Mode;
}

//========================================================
// Function Name:	Snd_A1800_GetTotalTime
// Syntax:		Snd_A1800_GetTotalTime(unsigned long DataPtr)
// Purpose: 	The return unit is milisecond //A1800 Play One Frame's time == 20ms(320/16KSampleRate) 			 
// Note:		    length * 16bits(1word = 16bits)
//				-------------------------------------------------  ms
//					16bit(CompressRate Kbps) * pow(2, speed/12)
// Parameter : 	unsigned long DataPtr: must include the First three words of the A1800_File
// Return:		The total play time of the input A1800 File
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
unsigned long Snd_A1800_GetTotalTime(unsigned long DataPtr)
{
	int SACM_curSpeed, uiTempSpeed;
	unsigned int uiBitRateId;
	unsigned long ulTotalTime, ulTmp, ulDataLength;

	//Get the lengthe and the BitRate
	ulDataLength = ( (( unsigned long )RVUI( DataPtr + 1 )) << 16 ) + RVUI( DataPtr );
	uiBitRateId = RVUI( DataPtr + 2 );
	uiBitRateId = uiBitRateId / 400;		//ByteNumber Per Frame

	//The process before speed dispose
	ulTotalTime = ( ulDataLength / (unsigned long)uiBitRateId );
	SACM_curSpeed = ( unsigned int )SACM_Get_Speed();
	SACM_curSpeed -= 12;					//The Middle Speed is 12
	//The speed dispose
	if(SACM_curSpeed < 0)
	{
		uiTempSpeed = -SACM_curSpeed;
		ulTotalTime = (unsigned long)( (float)(ulTotalTime * 20) * pow((double)2, (double)uiTempSpeed/(double)12) );
	}
	else
	{
		ulTotalTime = (unsigned long)( (float)(ulTotalTime * 20) / pow((double)2, (double)SACM_curSpeed/(double)12) );
	}

	return ulTotalTime;
}

//========================================================
// Function Name:	Snd_A1800FAT_GetTotalTime
// Syntax:		Snd_A1800FAT_GetTotalTime( int handle )
// Purpose: 	get A1800 total play time 			 
// Note:		NO
// Parameter : 	int handle: file handle
// Return:		The total play time of the input A1800 File
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
unsigned long Snd_A1800FAT_GetTotalTime( int handle )
{
	unsigned int uiTempBuf[3];
	unsigned long ulTempAddr;

	if( handle < 0 )
	{
		return RTN_FILEERROR;
	}
	lseek( handle, 0, SEEK_SET );			//Seek to the start of the file input
	ulTempAddr = ( unsigned long )uiTempBuf;
	read( handle, ulTempAddr << 1, 6);		//Read the first three words to get the Mode Flag and length
	lseek( handle, 0, SEEK_SET );
	return Snd_A1800_GetTotalTime( uiTempBuf );
}

//========================================================
// Function Name:	Snd_A4800_GetTotalTime
// Syntax:		Snd_A4800_GetTotalTime(unsigned long DataPtr)
// Purpose: 	The return unit is milisecond //A1800 Play One Frame's time == 16ms(256/16KSampleRate) 			 
// Note:		    length * 16bits(1word = 16bits)
//				-------------------------------------------------  ms
//					16bit(CompressRate Kbps) * pow(2, speed/12)
// Parameter : 	unsigned long DataPtr: must include the First three words of the A4800_File
// Return:		The total play time of the input A4800 File
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
unsigned long Snd_A4800_GetTotalTime(unsigned long DataPtr)
{
	int SACM_curSpeed, uiTempSpeed;
	unsigned int uiBitRateId;
	unsigned long ulTotalTime, ulTmp, ulDataLength;

	//Get the lengthe and the BitRate
	ulDataLength = ( (( unsigned long )RVUI( DataPtr + 1 )) << 16 ) + RVUI( DataPtr );		//Byte
	uiBitRateId = RVUI( DataPtr + 2 );
	uiBitRateId = SACM_A4800Mode[uiBitRateId];		//Get the BitRate: bps
	uiBitRateId = uiBitRateId / 500;				//Byte Number per Frame
	//The process before speed dispose
	ulTotalTime = ( ulDataLength / ( unsigned long )uiBitRateId );
	SACM_curSpeed = ( unsigned int )SACM_Get_Speed();
	SACM_curSpeed -= 12;					//The Middle Speed is 12
	//The speed dispose
	if(SACM_curSpeed < 0)
	{
		uiTempSpeed = -SACM_curSpeed;
		ulTotalTime = (unsigned long)( (float)(ulTotalTime << 4) / pow((double)2, (double)uiTempSpeed/(double)12) );	//16ms
	}
	else
	{
		ulTotalTime = (unsigned long)( (float)(ulTotalTime << 4) * pow((double)2, (double)SACM_curSpeed/(double)12) );	//16ms
	}
	return ulTotalTime;
}

//========================================================
// Function Name:	Snd_A4800FAT_GetTotalTime
// Syntax:		Snd_A4800FAT_GetTotalTime( int handle )
// Purpose: 	get A4800 total play time 			 
// Note:		NO
// Parameter : 	int handle: file handle
// Return:		The total play time of the input A4800 File
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
unsigned long Snd_A4800FAT_GetTotalTime( int handle )
{
	unsigned int uiTempBuf[3];
	unsigned long ulTempAddr;

	if( handle < 0 )
	{
		return RTN_FILEERROR;
	}
	lseek( handle, 0, SEEK_SET );			//Seek to the start of the file input
	ulTempAddr = ( unsigned long )uiTempBuf;
	read( handle, ulTempAddr << 1, 6);		//Read the first three words to get the Mode Flag and length
	lseek( handle, 0, SEEK_SET );			//Seek to the start of the file input when get over
	return Snd_A4800_GetTotalTime( uiTempBuf );
}

//========================================================
// Function Name:	Snd_A1800_GetCurrentTime
// Syntax:		Snd_A1800_GetCurrentTime(void)
// Purpose: 	Get the current time of the A1800 play 			 
// Note:		NO
// Parameter : 	N/A
// Return:		the current time of the A1800 play
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
unsigned long Snd_A1800_GetCurrentTime(void)
{
	unsigned long ulFrame_Count, ulFrameNumber, ulReturnTime;	//返回时表示时间
	unsigned int FrameNum, uiWordsPerFrame;
	int SACM_curSpeed;
	float denom, expon;
	expon = 0;

	uiWordsPerFrame = SACM_A1800_Mode / 800;

	//A1800每一桢播放的时间是20ms: ulFrame_Count * 20
	ulFrame_Count = ( unsigned long )User_GetCurDecodeLength();
	ulFrameNumber = ( ulFrame_Count * 20 ) / uiWordsPerFrame;
	SACM_curSpeed = ( unsigned int )SACM_Get_Speed();
	SACM_curSpeed -= 12;					//The Middle Speed is 12
	//expon = (float)SACM_curSpeed/(float)12; -- testing if this actually helps
	denom = powf((float)2,expon);
	ulReturnTime = (unsigned long)( ulFrameNumber / denom );
	return ulReturnTime;
}

//========================================================
// Function Name:	Snd_A4800_GetCurrentTime
// Syntax:		Snd_A4800_GetCurrentTime(void)
// Purpose: 	Get the current time of the A4800 play 			 
// Note:		The A4800 plays 16ms per Frame
// Parameter : 	N/A
// Return:		the current time of the A4800 play
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
unsigned long Snd_A4800_GetCurrentTime()
{
	unsigned long ulFrame_Count, ulFrameNumber, ulReturnTime;	//返回时表示时间
	unsigned int FrameNum, uiWordsPerFrame;
	int SACM_curSpeed , uiTmpSpeed;
	uiWordsPerFrame = SACM_A4800_Mode / 1000;

	//A4800每一桢播放的时间是16ms: ulFrame_Count * 16
	ulFrame_Count = ( unsigned long )User_GetCurDecodeLength();
	ulFrameNumber = ( ulFrame_Count * 16 ) / uiWordsPerFrame;
	SACM_curSpeed = ( unsigned int )SACM_Get_Speed();
	SACM_curSpeed -= 12;					//The Middle Speed is 12
	if(SACM_curSpeed < 0)
	{
		uiTmpSpeed = -SACM_curSpeed;
		ulReturnTime = (unsigned long)( ulFrameNumber * pow((double)2, (double)uiTmpSpeed/(double)12) );
	}
	else
	{
		ulReturnTime = (unsigned long)( ulFrameNumber / pow((double)2, (double)SACM_curSpeed/(double)12) );
	}	return ulReturnTime;
}
//--------------------------------------------------------------------
// Syntax:		unsigned long Snd_Get_A1600SkipOffset(unsigned long StartTime)
// Parameter:	unsigned long StartTime
// Return:		NONE
// Description:	Must known _SACM_A1800_ModeCode first
//--------------------------------------------------------------------
unsigned long Snd_Get_A1800SkipOffset(unsigned long StartTime)
{
	unsigned int uiWordsPerFrame;
	unsigned long ulFrameNumber;
	int SACM_curSpeed , uiTmpSpeed;

	uiWordsPerFrame = SACM_A1800_Mode / 800;
	ulFrameNumber = StartTime * (unsigned long)uiWordsPerFrame;
	//
	SACM_curSpeed = ( unsigned int )SACM_Get_Speed();
	SACM_curSpeed -= 12;
	//
		if(SACM_curSpeed < 0)
	{
		uiTmpSpeed = -SACM_curSpeed;
		ulFrameNumber = (unsigned long) ( (double)(ulFrameNumber / 20) / pow((double)2, (double)uiTmpSpeed/(double)12) );
	}
	else
	{
		ulFrameNumber = (unsigned long) ( (double)(ulFrameNumber / 20) * pow((double)2, (double)SACM_curSpeed/(double)12) );
	}
	
	if(ulFrameNumber % uiWordsPerFrame)
	{
		ulFrameNumber = ulFrameNumber / uiWordsPerFrame;
		ulFrameNumber = ulFrameNumber * uiWordsPerFrame;
	}
	return ulFrameNumber;
}
//========================================================
// Function Name:	SACM_A1800FAT_SeekTime
// Syntax:		SACM_A1800FAT_SeekTime(long milisecond, int orientation)
// Purpose: 	seek forward or backward while playing only for A1800 			 
// Note:		NO
// Parameter : 	milisecond:	 the seek time backward or forward
//				orientation: 0: forward; 1: backward
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int SACM_A1800FAT_SeekTime( long milisecond, int orientation )
{
	unsigned long ulTempLength, ulDecodelength, ulNewDecodeLength , ulFormerLength;
	unsigned int uiTempBitRate, uiPerFrame;
	long ulAfterLength ;

	SACM_Pause();
	
	ulTempLength = Snd_Get_A1800SkipOffset(milisecond);

	if(0 == orientation){	
//		ulFormerLength = lseek(SACMFileHandle, 0, SEEK_CUR);
//		fstat(SACMFileHandle,&filestatus);
//		if(filestatus.st_size < ulFormerLength + (ulTempLength<<1)){
//			SACM_Resume();			
//			return -2;
//		}
		lseek(SACMFileHandle, ulTempLength << 1, SEEK_CUR);	//seek after the current position
	}
	else if(1 == orientation){		
		ulFormerLength = lseek(SACMFileHandle, 0, SEEK_CUR);
		ulAfterLength = ulFormerLength  - (ulTempLength<<1);
		if(ulAfterLength <= 0){
			SACM_Resume();
			return -1;
		}
		lseek(SACMFileHandle, -ulTempLength << 1, SEEK_CUR);//seek after the current position
	}
	ulDecodelength = ( unsigned long )User_GetCurDecodeLength();
	if( 0 == orientation )
	{
		ulNewDecodeLength = ulDecodelength + ulTempLength;
	}
	else if( 1 == orientation )
	{
		ulNewDecodeLength = ulDecodelength - ulTempLength;
	}

	User_SetDecodeLength( ulNewDecodeLength );
	SACM_Resume();
	return TRUE;
}

//========================================================
// Function Name:	SACM_A4800FAT_SeekTime
// Syntax:		SACM_A4800FAT_SeekTime(long milisecond, int orientation)
// Purpose: 	seek forward or backward while playing only for A4800 			 
// Note:		NO
// Parameter : 	milisecond:	 the seek time backward or forward
//				orientation: 0: forward; 1: backward
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int SACM_A4800FAT_SeekTime( long milisecond, int orientation )
{
	unsigned long ulTempLength, ulDecodelength, ulNewDecodeLength;
	unsigned int uiTempBitRate, uiPerFrame;

	uiTempBitRate = SACM_A4800_Mode / 1000;			//Get the Main BR
	ulTempLength = uiTempBitRate * milisecond;		//Get the Seek length when backward or forward
	ulTempLength = ulTempLength >> 4;				//Get the word Number
	uiPerFrame = SACM_A4800_Mode / 1000;			//Words number per frame before decode
	if( ulTempLength % uiPerFrame )
	{
		ulTempLength = ulTempLength / uiPerFrame;
		ulTempLength = ulTempLength * uiPerFrame;
	}
	lseek(SACMFileHandle, ulTempLength << 1, SEEK_CUR);	//seek after the current position
	ulDecodelength = ( unsigned long )User_GetCurDecodeLength();
	if( 0 == orientation )
	{
		ulNewDecodeLength = ulDecodelength + ulTempLength;
	}
	else if( 1 == orientation )
	{
		ulNewDecodeLength = ulDecodelength - ulTempLength;
	}

	User_SetDecodeLength( ulNewDecodeLength );
}

//========================================================
// Function Name:	Snd_SACM_PlayMemory
// Syntax:		Snd_SACM_PlayMemory(int _CodecType, unsigned long start_addr)
// Purpose: 	play resource from special memory 			 
// Note:		NO
// Parameter : 	_CodecType: select which algorithm to play
//				start_addr: the address of encoded data 
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_SACM_PlayMemory(int _CodecType, unsigned long start_addr)
{
	USER_SetMemoryAddr(start_addr);                 		//in user_sacm.asm
	SACM_Codec(_CodecType);
	SACM_Play(Manual_Mode_Index, DAC1+DAC2, 0);				//Manual_Mode_Index=-1
	return TRUE;
}

//========================================================
// Function Name:	Snd_SACM_PlayFAT
// Syntax:		Snd_SACM_PlayFAT(int FileHandle, int _CodecType)
// Purpose: 	Record to FAT 			 
// Note:		NO
// Parameter : 	FileHandle: the file to play
//				_CodecType: select which algorithm to play
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_SACM_PlayFAT(int FileHandle, int _CodecType)
{
	struct stat_t filestatus;

	if(SACMFileHandle >= 0)
	{
		close(SACMFileHandle);
		SACMFileHandle = -1;
	}
	SACMFileHandle = FileHandle;
	SACM_Codec(_CodecType);
	Fat_SetStartAddr(LB2LP(NFAT_DS_GetData));
	
	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);
	return TRUE;
}

//========================================================
// Function Name:	Snd_SACM_RecMemory
// Syntax:		Snd_SACM_RecFAT(unsigned long RECStartAddr, int _CodecType, int BitRate)
// Purpose: 	Record to the special memory			 
// Note:		NO
// Parameter : 	FileHandle: the file to play
//				_CodecType: select which algorithm to play
//				BitRate:	BitRate
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_SACM_RecMemory(int _CodecType, unsigned long RECStartAddr, int BitRate)
{	
	SACM_Codec(_CodecType);
	USER_SetRECStartAddr(RECStartAddr + 2);
	
	if(_CodecType == C_CODEC_AUDIO1800)
	{
		SACM_DVR1800_BITRATE(DefaultBitRate);
	}	
	
	SACM_Rec(RecMonitorOff,BitRate); 	// mic, bitrate
	return TRUE;
}

//========================================================
// Function Name:	Snd_SACM_RecFAT
// Syntax:		Snd_SACM_RecFAT(int FileHandle, int _CodecType, int BitRate)
// Purpose: 	play resource from FAT 			 
// Note:		NO
// Parameter : 	FileHandle: the file to play
//				_CodecType: select which algorithm to play
//				BitRate:	BitRate
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_SACM_RecFAT(int FileHandle, int _CodecType, int BitRate)
{
	if(SACMFileHandle >= 0)
	{
		close(SACMFileHandle);
		SACMFileHandle = -1;
	}
	SACMFileHandle = FileHandle;
	
	SACM_Codec(_CodecType);
	Fat_SetStartAddr(LB2LP(NFAT_DS_WriteData_Rec));
	
	if(_CodecType == C_CODEC_AUDIO1800)
	{
		SACM_DVR1800_BITRATE(DefaultBitRate);
	}	
	
	SACM_Rec(RecMonitorOff,BitRate); 	// mic, bitrate
}

//========================================================
// Function Name:	Snd_Stop
// Syntax:		Snd_Stop(void)
// Purpose: 	stop the playing algorithms			 
// Note:		NO
// Parameter : 	None
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_Stop()
{
	SACM_Stop();
	if(SACMFileHandle >= 0)
	{
		close(SACMFileHandle);
		SACMFileHandle = -1;
	}
	if(MP3FileHandle >= 0)
	{
		close(MP3FileHandle);
		MP3FileHandle = -1;
	}
	return TRUE;
}

//========================================================
// Function Name:	Get_DataBase_TTS320
// Syntax:		Get_DataBase_TTS320(unsigned int DesPtr, unsigned int Length, unsigned long offset )
// Purpose: 	get TTS320 database from FAT 			 
// Note:		NO
// Parameter : 	DesPtr: destinate address
//				Length: read length
//				offset: the offset of database
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
#ifdef S320TTS_Used
extern unsigned long TTS320_Buffer;
void Read_TTS320Res_NotOffset()
{	
	int ret = 0;
	ret = read(SACMFileHandle, (unsigned long)&TTS320_Buffer<<1, 2);
	lseek(SACMFileHandle, -ret, SEEK_CUR);
}

void Read_TTS320Res()
{
	unsigned long TemBuffer;
	read(SACMFileHandle, (unsigned long)&TTS320_Buffer<<1, 2);
}

void Open_TTS320_DataBase()
{
	DataBase_fp = open((unsigned long)("B:\\SP_WORD.320"), O_RDWR);
	if(-1 == DataBase_fp)
	{
		while(1);
	}
}

void Close_TTS320_DataBase()
{
	if(DataBase_fp >= 0)
	{
		close(DataBase_fp);
	}
}

unsigned long Get_DataBase_TTS320(unsigned int DesPtr, unsigned int Length, unsigned long offset )
{
	unsigned long ret;
	unsigned long Temp_Length;
	Temp_Length = (unsigned long)Length<<1;
	lseek(DataBase_fp, offset<<1, SEEK_SET );
	ret = read( DataBase_fp, (unsigned long)DesPtr<<1, Temp_Length );
	if( ret != Temp_Length )
	{
		SACM_Stop();
	}
	else
	{
		return ret;
	}
}
#endif	//end of S320TTS_Used	

#ifdef WAV_Used
void WAV_SetFileHeader(unsigned int WriterBuffer, unsigned int HeaderLength)
{		
	lseek(SACMFileHandle, 0, SEEK_SET);
	write(FileHandle, (unsigned long)WriterBuffer<< 1, HeaderLength<< 1);
}	
#endif

void SACM_SetFileHeader(void)
{		
	unsigned long Musicdata_Length;
	char *TemDataLength = NULL;
	TemDataLength = &Musicdata_Length;
	Musicdata_Length = CurSACMEncodeCount<<1;
	lseek(SACMFileHandle, 0, SEEK_SET);
	write(SACMFileHandle, (unsigned long)TemDataLength<<1, 4);
	close(SACMFileHandle);
	SACMFileHandle = -1;
}
//////////////////////////////////////////////////////////////////////
int SACM_Encode_Finish_Process(unsigned long ulEncode_WordLength)
{
	int ret,WriterNum;
	unsigned long ulTempBufferPtr;
	//////////////////////writer--leave--buffer////////////////////////////
	ulTempBufferPtr = (unsigned long)&RecordFATBuffer;
	ulTempBufferPtr += C_RECFATBUFFERSIZE;
	
	if(C_RECFATFIRSTBUFFER == uiRecWriteFlag)
	{
		uiRecWriteFlag = 0;						//avoid re-entry
		ret = write(SACMFileHandle, (unsigned long)&RecordFATBuffer<<1, C_RECFATBUFFERSIZE<<1);
		if (ret < 0)
		{	
			return -1;
		}
	}
	else if(C_RECFATSECONDBUFFER == uiRecWriteFlag)
	{
		uiRecWriteFlag = 0;						//avoid re-entry
		ret = write(SACMFileHandle, (unsigned long)ulTempBufferPtr<<1, C_RECFATBUFFERSIZE<<1);
		if (ret < 0)
		{	
			return -1;
		}
	}
	///////////////////////////////////////////////////////////
	if (pRecWriteptr >= C_RECFATBUFFERSIZE)
	{
		WriterNum = pRecWriteptr - C_RECFATBUFFERSIZE;		
		ret = write(SACMFileHandle, (unsigned long)ulTempBufferPtr<<1 ,WriterNum<<1);
		if (ret < 0)
		{	
			return -1;
		}
	}
	else
	{	
		WriterNum = pRecWriteptr;
		ret = write(SACMFileHandle, (unsigned long)&RecordFATBuffer<<1, WriterNum<<1);		
		if(ret < 0)
		{	
			return -1;
		}
	}
}

void Snd_OnPlayEnd(void)
{
	
}