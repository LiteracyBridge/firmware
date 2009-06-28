#include ".\Component\Include\Component_head.h"
#include ".\System\Include\System_head.h"

#ifdef MP3_FATREADBUFFER
extern unsigned long MP3FATReadBuffer;		//从文件系统中读取数据暂时到Buffer中
#endif
extern int SACMFileHandle;	
extern int MP3FileHandle;
extern int NFAT_DS_GetData( LPTR dest_addr,unsigned int length);
extern void NFAT_DS_GetData_MP3( LPTR dest_addr,unsigned int length);
extern void MP3_FrameResume(void);
extern void User_NewDataMP3(unsigned long DecodeLen, unsigned long MP3FrameLen);
extern void SetMP3FrameCount(unsigned long MP3Frame);
extern unsigned int User_Getdaclength(void);
extern unsigned long GetMP3FrameCount(void);
int Snd_MP3Player_SeekTime(long second,int orientation);
int Snd_MP3Player_SeekLength(long mp3length,int orientation);

/**********************************************************************/
//The following functions are used for mp3 play demo only
//the users can write their own function according to these functions.
void MediaPlayer_PlayBack(void);
void MediaPlayer_PlayNext(void);
void MediaPlayer_SetPlayMode(unsigned int mode);
int MediaPlayer_PlayList(List *Head, unsigned int FileID);
void Mp3_onPlayErr( int n );
void PlayListTraversal(List *Head);
void PlayNextFile( );
int GetNextFilePath( );
int find_list_head(List *Cur_List);
int create_rand_file(List *Head);
void FreeDLList( void );
int Snd_A128_SeekTime(long milisecond, int orientation);
/***********************************************************************/

//========================================================
// Function Name:	Snd_MP3_PlayMemory
// Syntax:		Snd_SACM_PlayMemory(unsigned long start_addr, unsigned long end_addr)
// Purpose: 	play resource from special memory 			 
// Note:		NO
// Parameter : 	_CodecType: select which algorith to play
//				_DataPtr:   the address of encoded data 
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_MP3_PlayMemory(unsigned long start_addr, unsigned long end_addr)
{
	SACM_Codec(C_CODEC_MP3);
	USER_SetMP3MemoryAddr(start_addr, end_addr);             //in sacm_user.asm
	SACM_MP3_SetFS();

	USER_SetMP3MemoryAddr(start_addr, end_addr);
	
	SACM_Play(Manual_Mode_Index, DAC1+DAC2, 0);				//Manual_Mode_Index=-1
	return TRUE;
}

//========================================================
// Function Name:	Snd_PlayMP3FAT
// Syntax:		Snd_PlayMP3FAT(const char *filename)
// Purpose: 	play resource from special memory 			 
// Note:		NO
// Parameter: 	filename: the path of MP3 file
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_PlayMP3FAT(const char *filename)
{	
	unsigned long ulTempMForlength;
	unsigned long Musicdata_Length;
	struct stat_t filestatus;
	
	SACM_Stop();
	MP3Filelength = 0;
	SACM_Codec(C_CODEC_MP3);					//Set the MP3 Codec Type
	Fat_SetStartAddr(LB2LP(NFAT_DS_GetData_MP3));	//Set the addr
	if(MP3FileHandle >= 0)
	{
		close(MP3FileHandle);
		MP3FileHandle = -1;
	}
	MP3FileHandle = open((LPSTR)filename,O_RDONLY );
	if(MP3FileHandle < 0)
	{
		return -1;
	}
	stat((LPSTR)filename,&filestatus);
	Musicdata_Length=filestatus.st_size;
	gMP3FATLength = Musicdata_Length;
	Musicdata_Length = Musicdata_Length >> 1;
	USER_SetFSLength(Musicdata_Length);
	ulFrame_Count_MP3 = 0;
	ulTempMForlength = MP3Filelength;
#ifdef MP3_FATREADBUFFER
	{
		int ret;

		MP3ReadModeFlag = 0;
		if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
		{
			return -1;					//added by chengye 2007/6/20
		}
		MP3ReadModeFlag = 1;
		MP3Filelength = ulTempMForlength;
		MP3Filelength += ulFrame_Count_MP3 << 1;
		gMP3_FileOffset = ulFrame_Count_MP3;			//记录文件的起始桢偏移
		lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
		USER_SetFSLength( Musicdata_Length - ulFrame_Count_MP3 );
		uiMP3WriteFlag = 0;
		pMP3Readptr = 0;
		ret = read( MP3FileHandle, (unsigned long)&MP3FATReadBuffer<<1, C_MP3FATBUFFERSIZE<<2);
		MP3Filelength += C_MP3FATBUFFERSIZE<<2;
		if(ret == 0xffff)
		{
			return -1;
		}
	}
#else
	if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
	{
		return -1;					//added by chengye 2007/6/20
	}
	MP3Filelength = ulTempMForlength;
	USER_SetFSLength( Musicdata_Length - ulFrame_Count_MP3 );
//	MP3Filelength += ulFrame_Count_MP3 << 1;
//	lseek(MP3FileHandle, MP3Filelength, SEEK_SET);		//重新Seek到MP3Frame的真正开始处
#endif
	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);
}

//========================================================
// Function Name:	Snd_PlayMP3FAT_Part
// Syntax:		Snd_PlayMP3FAT_Part(char *filename, unsigned long offset, unsigned long datalength)
// Purpose: 	play MP3 from the special point of FAT
// Note:		NO	
// Parameter : 	char * filename:full path name
//				unsigned long offset:the unit is byte
//				unsigned long datalength:the unit is byte
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_PlayMP3FAT_Part(char *filename, unsigned long offset, unsigned long datalength)
{
	unsigned long ulTempMForlength;
	struct stat_t filestatus;
	
	SACM_Stop();
	MP3Filelength = offset;							//record the begin location
	SACM_Codec(C_CODEC_MP3);						//Set the MP3 Codec Type
	Fat_SetStartAddr(LB2LP(NFAT_DS_GetData_MP3));	//Set the function address of get data
	if(MP3FileHandle >= 0)
	{
		close(MP3FileHandle);
		MP3FileHandle = -1;
	}
	MP3FileHandle = open((LPSTR)filename,O_RDONLY );
	if(MP3FileHandle < 0)
	{
		return -1;
	}
	lseek(MP3FileHandle, offset, SEEK_SET);			//seek to the special point
	stat((LPSTR)filename,&filestatus);
	if( (offset+datalength-1)>filestatus.st_size )
	{
		return -1;									//means the input dataleng is overflow
	}
	gMP3FATLength = datalength;
	USER_SetFSLength(gMP3FATLength>>1);
	ulFrame_Count_MP3 = 0;
	ulTempMForlength = MP3Filelength;
#ifdef MP3_FATREADBUFFER
	{
		int ret;

		MP3ReadModeFlag = 0;
		if(SACM_MP3_SetFS())						//while return 1,it's means this MP3 file can't decode 
		{
			return -1;	
		}
		MP3ReadModeFlag = 1;
		MP3Filelength = ulTempMForlength;
		MP3Filelength += ulFrame_Count_MP3 << 1;
		gMP3_FileOffset = ulFrame_Count_MP3;			//记录文件的起始桢偏移
		lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
		USER_SetFSLength((gMP3FATLength>>1) - ulFrame_Count_MP3);
		uiMP3WriteFlag = 0;
		pMP3Readptr = 0;
		ret = read( MP3FileHandle, (unsigned long)&MP3FATReadBuffer<<1, C_MP3FATBUFFERSIZE<<2);
		MP3Filelength += C_MP3FATBUFFERSIZE<<2;
		if(ret < 0)
		{
			return -1;
		}
	}
#else
	if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
	{
		return -1;					//added by chengye 2007/6/20
	}
	MP3Filelength = ulTempMForlength;
	USER_SetFSLength((gMP3FATLength>>1) - ulFrame_Count_MP3);
//	MP3Filelength += ulFrame_Count_MP3 << 1;
//	lseek(MP3FileHandle, MP3Filelength, SEEK_SET);		//重新Seek到MP3Frame的真正开始处
#endif
	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);
}

//Function: Seek Time:
//使用这个函数时要注意一点: 这个函数的动作是停止播放, 然后再重新Seek长度播放, 所以
//CurrentTime会在这时被清零, User在使用时要自己记录原来的时间
int Snd_MP3Player_SeekTime(long milisecond,int orientation)
{
	int ret;
	unsigned long ulSkipData, lplaydata, lplaydata1, length, uldecode;
	unsigned long ulFramLength1, ulFramLength2, ulBufferRead;

	ulFramLength1 = ( unsigned long )GetMP3FrameCount();	//记录上次播放的FrameCountNumber
	USER_PauseMP3();				//得到MP3上次的Decode_Count值存在MP3Decode_Length变量中
	SACM_Stop();

	ulSkipData = (R_MPEGBitRate * milisecond) >> 3;	//byte unit
	MP3Filelength = (MP3Decode_Length + gMP3_FileOffset) << 1;		//Add 2007/04/04
	lplaydata = R_SampleRate * ( milisecond ) / 500;	//	/(1000/2)双声道要将播放的数据个数*2
	lplaydata = lplaydata/User_Getdaclength();

	if(orientation == C_Forward)
	{
		MP3Filelength += ulSkipData;
		if(MP3Filelength > gMP3FATLength)
		{
			//PlayNextFile();
			//return 0;
			SACM_Stop();
			return -2;
		}
		lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
	}
	else if(orientation == C_Backward)
	{
		if( ulSkipData <= 0 )
		{
			return -2;
		}
		if(ulSkipData > MP3Filelength)
		{
			MP3Filelength = 0;
			lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
		}
		else
		{
			MP3Filelength -= ulSkipData;
			lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
		}
	}
	else
	{
		return -3;			//  parament erro
	}
	
	ulFrame_Count_MP3 = 0;
#ifdef MP3_FATREADBUFFER
	MP3ReadModeFlag = 0;
	if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
	{
		return -1;					//added by chengye 2007/6/20
	}
	MP3ReadModeFlag = 1;
//	MP3Filelength += ulFrame_Count_MP3 << 1;
//	lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
	uiMP3WriteFlag = 0;
	pMP3Readptr = 0;
	ret = read( MP3FileHandle, (unsigned long)&MP3FATReadBuffer<<1, C_MP3FATBUFFERSIZE<<2);
	MP3Filelength += C_MP3FATBUFFERSIZE<<2;
	if(ret == 0xffff)
	{
		return -1;
	}							//PlayAB, 3 is just a Falg.
#else
	if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
	{
		return -1;					//added by chengye 2007/6/20
	}
//	MP3Filelength += ulFrame_Count_MP3 << 1;
//	lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
#endif
	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);

	ulFramLength2 = ( unsigned long )GetMP3FrameCount();	//记录现在播放的MP3的FrameCountNumber
	ulFramLength1 += ulFramLength2;							//加上原来的FrameCounter
#if 1
//===============Add 2007/04/03 start====================	//在MP3播放前先找到有效祯头与现在位置的偏移
	lplaydata1 = ( ulFrame_Count_MP3 << 4 ) / R_MPEGBitRate;	//得到偏移长度可以播放的时间ms
	lplaydata1 = R_SampleRate * lplaydata1 / 500;		//	/(1000/2)双声道要将播放的数据个数*2
	lplaydata1 = lplaydata1 / User_Getdaclength();
	ulFramLength1 += lplaydata1;										//加上偏移的Data长度.
//===============Add 2007/04/03 end======================
#endif
	if(orientation == C_Forward)
	{
		uldecode = MP3Decode_Length + ulFrame_Count_MP3 + (ulSkipData >> 1);
		User_NewDataMP3(uldecode, 0);
		ulFramLength1 += 0x0004;							//加上8作为长度补偿
		SetMP3FrameCount(ulFramLength1 + lplaydata);		//重新设置MP3的FrameCountNumber
	}
	else if(orientation == C_Backward)
	{
		if(ulFramLength1 < lplaydata)
		{
			lplaydata = ulFramLength1;
		}
		uldecode = MP3Decode_Length + ulFrame_Count_MP3 - (ulSkipData >> 1);
		User_NewDataMP3(uldecode, 0);
		SetMP3FrameCount(ulFramLength1 - lplaydata);		//重新设置MP3的FrameCountNumber
	}

	return 0;
}

//========================================================
// Function Name:	Snd_MP3Player_SeekLength
// Syntax:		Snd_MP3Player_SeekLength(long mp3length,int orientation)
// Purpose: 	play MP3 from a special time
// Note:		we must notice that: this function is purpose to stop play, then re-seek length to play.
//				so CurrentTime will be set to 0 at present, user should record the formerly time by himself.	
// Parameter : 	long milisecond
//				int orientation
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
//Function: Seek length, input mp3length is in byte unit
int Snd_MP3Player_SeekLength(long mp3length,int orientation)
{
	int ret;
	unsigned long ulSkipData, lplaydata, length;
	unsigned long ulFramLength1, ulFramLength2;
	
	ulFramLength1 = GetMP3FrameCount();				//记录上次播放的FrameCountNumber
	SACM_Stop();

	lplaydata = mp3length/User_Getdaclength();

	if(orientation == C_Forward)
	{
		MP3Filelength += mp3length;
		if(MP3Filelength > gMP3FATLength)
		{
			PlayNextFile();
			return 0;
		}
		lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
	}
	else if(orientation == C_Backward)
	{
		if( mp3length <= 0 )
		{
			return -2;
		}
		if(mp3length > MP3Filelength)
		{
			MP3Filelength = 0;
			lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
		}
		else
		{
			MP3Filelength -= mp3length;
			lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
		}
	}
	else
	{
		return -3;			//  parament erro
	}
	
	ulFrame_Count_MP3 = 0;
#ifdef MP3_FATREADBUFFER
	MP3ReadModeFlag = 0;
	if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
	{
		return -1;					//added by chengye 2007/6/20
	}
	MP3ReadModeFlag = 1;
	MP3Filelength += ulFrame_Count_MP3 << 1;
	lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
	uiMP3WriteFlag = 0;
	pMP3Readptr = 0;
	ret = read( MP3FileHandle, (unsigned long)&MP3FATReadBuffer<<1, C_MP3FATBUFFERSIZE<<2);
	MP3Filelength += C_MP3FATBUFFERSIZE<<2;
	if(ret == 0xffff)
	{
		while(1);
	}							//PlayAB, 3 is just a Falg.
#else
	if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
	{
		return -1;					//added by chengye 2007/6/20
	}
	MP3Filelength += ulFrame_Count_MP3 << 1;
	lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
#endif
	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);
	
	ulFramLength2 = GetMP3FrameCount();			//记录现在播放的MP3的FrameCountNumber
	ulFramLength1 += ulFramLength2;				//加上原来的FrameCounter
	if(orientation == C_Forward)
	{
		ulFramLength1 += 0x0008;							//加上8作为长度补偿
		SetMP3FrameCount(ulFramLength1 + lplaydata);		//重新设置MP3的FrameCountNumber
	}
	else if(orientation == C_Backward)
	{
		if(ulFramLength1 < lplaydata)
		{
			lplaydata = ulFramLength1;
		}
		SetMP3FrameCount(ulFramLength1 - lplaydata);		//重新设置MP3的FrameCountNumber
	}
	
	return 0;
}

//========================================================
// Function Name:	Snd_MP3Speech_Pause
// Syntax:		Snd_MP3Speech_Pause(void)
// Purpose: 	special for MP3 pause,then play others' sacm 
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void Snd_MP3Speech_Pause(void)
{
	SndMP3_Pause();
	SACM_Stop();
}

//========================================================
// Function Name:	Snd_MP3Speech_Resume
// Syntax:		Snd_MP3Speech_Resume(void)
// Purpose: 	special for MP3 resume after others' sacm stop 
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void Snd_MP3Speech_Resume( void )
{
	SACM_Stop();
	if(SACMFileHandle >= 0)								
	{
		close(SACMFileHandle);
		SACMFileHandle = -1;
	}
	Snd_SACM_PlayMP3Middle();
}

//========================================================
// Function Name:	SndMP3_Pause
// Syntax:		SndMP3_Pause(void)
// Purpose: 	restore the total length decoded and the length has been decoded
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void SndMP3_Pause(void)
{
	USER_PauseMP3();
}

//恢复原来的解码总长度和已经解码的长度
void SndMP3_Resume(long length1, long length2)
{
	USER_ResumeMP3(length1, length2);
}

void Snd_SACM_PlayMP3Middle( void )
{
	extern unsigned long MP3Filelength;
	
	extern void SACM_MP3_GetDacOut(void);
	
//	Fat_SetStartAddr(LB2LP(NFAT_DS_GetData));
	Fat_SetStartAddr(LB2LP(NFAT_DS_GetData_MP3));	//modify by chengye 2007/4/23 
	USER_SetFSLength(MP3Former_Length);		
	SACM_MP3_GetDacOut();
	
	SACM_Codec(C_CODEC_MP3);
	SACM_Play(Manual_Mode_Index, DAC1+DAC2, 0);
	MP3_DecodeCount_Resume(MP3Decode_Length);		//added by chengye 2007/4/23
	MP3_FrameResume();
}

//========================================================
// Function Name:	GetCurPlayTime
// Syntax:		GetCurPlayTime(void)
// Purpose: 	Get the current play time, the unit is second 
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int GetCurPlayTime(void)
{
	SACM_MP3_Get_Time();

	return gCurSecondTime;
}

//========================================================
// Function Name:	GetCurPlayms
// Syntax:		GetCurPlayms(void)
// Purpose: 	Get the current play time, the unit is milisecond 
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
unsigned long GetCurPlayms(void)
{
	unsigned long gCurMiliSecond;		//当前播放时间(milisecond)的全局变量
	unsigned int iTempSample;

	iTempSample = R_SampleRate / 100;			//分子分母同除以100
	gCurMiliSecond = GetMP3FrameCount() * 5;	//means *1000/2: *1000表示转换为ms, /2表示是双声道
	gCurMiliSecond = gCurMiliSecond * User_Getdaclength() / iTempSample;
	return gCurMiliSecond;
}

//========================================================
// Function Name:	MediaPlayer_SetA
// Syntax:		MediaPlayer_SetA(void)
// Purpose: 	The Following 4 functions are used for AB Play
//				For Play AB functions 
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void MediaPlayer_SetA( void )
{
	SndMP3_Pause();
	//ulFrame_Count_A = MP3Filelength;
	UserGetDecodeLength();
	ulFrame_Count_A = ulFrame_CurrentCount;
	ulFrame_Count_A += ulFrame_Count_MP3;
	gMP3_FramSizeA = GetMP3FrameCount();
}


//========================================================
// Function Name:	MediaPlayer_SetB
// Syntax:		MediaPlayer_SetB(void)
// Purpose: 	For Play AB functions			 
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void MediaPlayer_SetB( void )
{
	//ulFrame_Count_B = MP3Filelength;
	UserGetDecodeLength();
	ulFrame_Count_B = ulFrame_CurrentCount;
	ulFrame_Count_B += ulFrame_Count_MP3;
}

//========================================================
// Function Name:	MediaPlayer_PlayAB
// Syntax:		MediaPlayer_PlayAB(void)
// Purpose: 	For Play AB functions			 
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void MediaPlayer_PlayAB( void )
{
	int ret;
	unsigned long Playlength;
	unsigned long ulTempMForlength;

	SACM_Stop();

	Playlength = ulFrame_Count_B - ulFrame_Count_A;	//The Unit is word
	MP3Filelength = ulFrame_Count_A << 1;
	lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
	ulFrame_Count_MP3 = 0;
#ifdef MP3_FATREADBUFFER
	MP3ReadModeFlag = 0;
	if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
	{
		return -1;					//added by chengye 2007/6/20
	}
	MP3ReadModeFlag = 1;
	MP3Filelength += ulFrame_Count_MP3 << 1;
	lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
	uiMP3WriteFlag = 0;
	pMP3Readptr = 0;
	ret = read( MP3FileHandle, (unsigned long)&MP3FATReadBuffer<<1, C_MP3FATBUFFERSIZE<<2);
	MP3Filelength += C_MP3FATBUFFERSIZE<<2;
	if(ret == 0xffff)
	{
		while(1);
	}							//PlayAB, 3 is just a Falg.
#else
	if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
	{
		return -1;					//added by chengye 2007/6/20
	}
	MP3Filelength += ulFrame_Count_MP3 << 1;
	lseek(MP3FileHandle, MP3Filelength, SEEK_SET);
#endif
	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);
	User_ClearMP3AB(Playlength);
}

//========================================================
// Function Name:	MediaPlayer_ClearAB
// Syntax:		MediaPlayer_ClearAB(void)
// Purpose: 	For Play AB functions			 
// Note:		here you must notice that: it isn't just at B point playing while resume,
//				so shoule consider the problem of decoded length.	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void MediaPlayer_ClearAB( void )
{
	User_NewDataMP3(MP3Decode_Length, gMP3_FramSizeA);	//将原来的已解码长度加到当前解码长度之上
	User_ClearMP3AB(MP3Former_Length);					//恢复原来的解码长度
}


//============================================================================
//============================================================================
//========The Following function is used for MP3 Demo Play, The user can select
//====================them for use if necessory===============================
//=============================================================================
//=============================================================================

//========================================================
// Function Name:	MediaPlayer_PlayList
// Syntax:		MediaPlayer_PlayList(List *Head, unsigned int FileID)
// Purpose: 	Play the songs according to the PlayList			 
// Note:		NO
// Parameter : 	List *Head:	the head of song list
//				unsigned int FileID: present the number of song
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int MediaPlayer_PlayList(List *Head, unsigned int FileID)
{
	int retval;

	if(0 == Head)
	{
		return -1;						//歌曲列表空，返回错误
	}
	
	MusicPlayMode |= C_PLAY_LISTPLAY;
	MusicPlayList = Head;

	while((MusicPlayList->FileID) != FileID)
	{
		MusicPlayList = MusicPlayList->Next;
		if(MusicPlayList == 0)		//It's the end of the PLayList
		{
			return -1;
		}
	}

 	PlayListTraversal( Head );			//Clear the Info in the List
	if( MusicPlayMode & C_PLAY_SHUFFLE ){
        MusicPlayList->Param = 1;
	}

	retval = Snd_PlayMP3FAT( MusicPlayList->MusicFilePath );

	return retval;
}

//========================================================
// Function Name:	MediaPlayer_PlayNext
// Syntax:		MediaPlayer_PlayNext(void)
// Purpose: 	Play the next songs according to the PlayList			 
// Note:		NO
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void MediaPlayer_PlayNext()
{
	int retval;

	SACM_Stop();
	if( ( MusicPlayMode & C_PLAY_SHUFFLE ) != 0 )		//随机播放列表模式
	{
		find_list_head( MusicPlayList );
		create_rand_file( MusicPlayList_Head );	//产生随机播放文件
	}else{
		if( MusicPlayList->Next == 0 )
		{
			MusicPlayList = MusicPlayList_Head;
		}
		else
			MusicPlayList = MusicPlayList->Next;
	}
	retval = Snd_PlayMP3FAT( MusicPlayList->MusicFilePath );
	if( retval < 0 ){
		SACM_Stop();
	}
}

//========================================================
// Function Name:	MediaPlayer_PlayBack
// Syntax:		MediaPlayer_PlayBack(void)
// Purpose: 	Play the former songs according to the PlayList			 
// Note:		NO
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void MediaPlayer_PlayBack()
{
	int retval;
	
	SACM_Stop();
	
	/*******************************************/	
	/******结束当前播放************************/
	/*******************************************/	

	if( ( MusicPlayMode & C_PLAY_SHUFFLE ) != 0 )		//随机播放列表模式
	{
		find_list_head( MusicPlayList );
		create_rand_file( MusicPlayList_Head );	//产生随机播放文件
	}else{
		if(MusicPlayList == MusicPlayList_Head)
		{
			do{
				MusicPlayList = MusicPlayList->Next;
			} while( MusicPlayList->Next != 0 );
		}
		else
			MusicPlayList = MusicPlayList->Back;
	}
	retval = Snd_PlayMP3FAT( (LPTR)MusicPlayList->MusicFilePath );

	if( retval < 0){
		SACM_Stop();
	}
}

//========================================================
// Function Name:	MediaPlayer_SetPlayMode
// Syntax:		MediaPlayer_SetPlayMode(unsigned int mode)
// Purpose: 	set play mode		 
// Note:		NO
// Parameter : 	unsigned int mode:
// 									C_PLAY_SHUFFLE 	0x01			
// 									C_PLAY_NORMAL 	0x02			
// 									C_PLAY_ONE 		0x04
// 									C_PLAY_REPEAT 	0x08
// 									C_PLAY_LISTPLAY	0x10
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void MediaPlayer_SetPlayMode(unsigned int mode)
{
	MusicPlayMode |= mode;
}

//================================================================================
//The following functions can be considered to be the MP3 driver layer, they are 
//no need to be transfered by user directly
//================================================================================

//========================================================
// Function Name:	PlayNextFile
// Syntax:		PlayNextFile(void)
// Purpose: 	Play the next songs according to the PlayList			 
// Note:		NO
// Parameter : 	N/A
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void PlayNextFile( )
{
	int retval;

	if(!(MusicPlayMode & C_PLAY_LISTPLAY))
	{
		return;
	}

	retval = GetNextFilePath( );       //取下个播放文件路径

	if( retval ){
		SACM_Stop();
		if(Mp3_ContinueFlag == 1 ){
			Mp3_ContinueFlag = 0;
			FreeDLList();
		}
	}else{
		retval = Snd_PlayMP3FAT( MusicPlayList->MusicFilePath );
		if( retval < 0 ){
			SACM_Stop();
		}
	}
}

//========================================================
// Function Name:	PlayListTraversal
// Syntax:		PlayListTraversal(List *Head)
// Purpose: 	clear the the played flag 			 
// Note:		NO
// Parameter : 	List *Head: the head of song list
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void PlayListTraversal(List *Head)
{
	List *pListTemp;

	pListTemp = Head;
	while(pListTemp != 0){					//Clear all falgs until the end
		pListTemp->Param = 0;
		pListTemp = pListTemp->Next;
	}
}

//========================================================
// Function Name:	GetNextFilePath
// Syntax:		GetNextFilePath(void)
// Purpose: 	get next file path			 
// Note:		NO
// Parameter : 	N/A
// Return:		0: success
//				-1: false
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int GetNextFilePath( void )
{
	unsigned int ret;

	if( MusicPlayMode&C_PLAY_SHUFFLE )				//随机播放列表模式
	{
		find_list_head( MusicPlayList );
		ret = create_rand_file( MusicPlayList_Head );//产生随机播放文件
		if( ret == 0xffff ){						//一遍已经播完
			if( MusicPlayMode&C_PLAY_REPEAT ){		//循环随机播放模式
				PlayListTraversal(MusicPlayList_Head);//清除已播放标志Param
				create_rand_file(MusicPlayList_Head);
			}else
				return -1;
		}
	}else if( MusicPlayMode&C_PLAY_NORMAL ){		//顺序播放模式
		if( (MusicPlayList->Next) != 0 ){
			MusicPlayList = MusicPlayList->Next;
		}else{
			if( MusicPlayMode & C_PLAY_REPEAT ){
				find_list_head( MusicPlayList );
				MusicPlayList = MusicPlayList_Head;
			}else
				return -1;
		}
	}else{											//单曲
			if( !(MusicPlayMode & C_PLAY_REPEAT))
				return -1;
	}
			return 0;
}

//========================================================
// Function Name:	find_list_head
// Syntax:		find_list_head(List *Cur_List)
// Purpose: 	find out the head node in the list		 
// Note:		NO
// Parameter : 	List *Cur_List: current position in the list
// Return:		0: success
//				-1: false
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int find_list_head(List *Cur_List)
{
	List *pListTemp;

	pListTemp = Cur_List;
	if(Cur_List == 0)
	{
		return -1;
	}
	while( pListTemp->Back != 0 ){ 
		pListTemp = pListTemp->Back;
	}
	MusicPlayList_Head = pListTemp;

	return 1;
}

//========================================================
// Function Name:	create_rand_file
// Syntax:		create_rand_file(List *Head)
// Purpose: 				 
// Note:		NO
// Parameter : 	List *Head
// Return:		0: success
//				-1: false
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int create_rand_file(List *Head)
{
	unsigned int ID;
	unsigned int ListNum_MAX;
	int flag;

	List *pListTemp;

	flag = 0;
	pListTemp = Head;
	while( pListTemp != 0 ){
		ListNum_MAX += 1;
		pListTemp=pListTemp->Next;
		if( pListTemp->Param == 1 )
		{
			flag++;
		}
	}
	if( !flag )
	{
		return -1;							//无序播放结束
	}
	do{
		ID=abs(rand())%ListNum_MAX+1;		//产生1~ListNum_MAX之间的FileID
		pListTemp=Head;
		while(pListTemp->FileID!=ID)
		{
			pListTemp=pListTemp->Next;		//
		}
	}while(pListTemp->Param==1);

	MusicPlayList=pListTemp;
	MusicPlayList->Param=1;
	return 0;
}

void FreeDLList( void )
{
	return;
}

//The unit of the Offset and lenth is Byte:
//Parament: flag is reserved for other mp3 function
int Snd_PlayMP3FATTimeOffset(const char *filename, unsigned long Offset, unsigned long length, unsigned int flag)
{
	unsigned long ulTempMForlength;
	unsigned long Musicdata_Length;
	struct stat_t filestatus;
	int ret;
	
	SACM_Stop();
	MP3Filelength = 0;
	SACM_Codec(C_CODEC_MP3);					//Set the MP3 Codec Type
	Fat_SetStartAddr(LB2LP(NFAT_DS_GetData_MP3));	//Set the addr
	if(MP3FileHandle >= 0)
	{
		close(MP3FileHandle);
	}
	MP3FileHandle = open((LPSTR)filename,O_RDONLY );
	if(-1 == MP3FileHandle)
	{
		return -1;
	}
	stat((LPSTR)filename,&filestatus);
		
	if( Offset>filestatus.st_size )
	{
		return -1;
	}
	if( Offset+length>filestatus.st_size )
	{
		length = filestatus.st_size - Offset;
	}
	
	Musicdata_Length = length;
	gMP3FATLength = Musicdata_Length;
	Musicdata_Length = Musicdata_Length >> 1;
	USER_SetFSLength(Musicdata_Length);
	ulFrame_Count_MP3 = 0;
	ulTempMForlength = MP3Filelength;
	lseek(MP3FileHandle, Offset, SEEK_SET);
	
//-----------------------------------------------------------------------------------------
	MP3ReadModeFlag = 0;
	if( SACM_MP3_SetFS() )			//while return 1,it's means this MP3 file can't decode 
	{
		return -1;					//added by chengye 2007/6/20
	}
	MP3ReadModeFlag = 1;
	MP3Filelength = ulTempMForlength;
	MP3Filelength += ulFrame_Count_MP3 << 1;
	gMP3_FileOffset = ulFrame_Count_MP3;			//记录文件的起始桢偏移
	lseek(MP3FileHandle, MP3Filelength+Offset, SEEK_SET);
	USER_SetFSLength( Musicdata_Length - ulFrame_Count_MP3 );
	uiMP3WriteFlag = 0;
	pMP3Readptr = 0;
	ret = read( MP3FileHandle, (unsigned long)&MP3FATReadBuffer<<1, C_MP3FATBUFFERSIZE<<2);
	MP3Filelength += C_MP3FATBUFFERSIZE<<2;
	if(ret == 0xffff)
	{
		while(1);
	}
//-----------------------------------------------------------------------------------------
	
	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);
}

#ifdef A128_Used
//======================For A128 Play seek time use 2007/09/05========================
//Function:		A128_SetLength
//Paraments:	unsigned long Reslength: the input length of the A128 Resource
//return:		void
void A128_SetLength( unsigned long Reslength )
{
	extern void User_A128Setlength(unsigned long length);

	User_A128Setlength( Reslength );
}

//Function:		GetCurA128PlayTime
//Parament:		void
//return:		int: the current time of A128 Play
int GetCurA128PlayTime(void)
{
	SACM_A128_Get_Time();

	return gCurA128SecondTime;
}

//Input Paraments: 	long milisecond：每次快进快退的时间，以ms为单位，由User输入，如果是4s，就输入4000(ms)
//					int orientation：快进还是快退的标志Flag，0为快进，1为快退
//OutPur Parament:	None
//Return Value:	0：Normal Sucess; -1: 快进长度过长; -2: 快退长度过长，超过了文件的头; -3: Input Paraments error.
int Snd_A128_SeekTime(long milisecond, int orientation)
{
	int ret;
	unsigned int iTempSmpleRate;
	unsigned long ulSkipData, lplaydata, uldecode;
	unsigned long ulFramLength1, ulBufferRead;
	
	extern int SACMFileHandle;

	ulFramLength1 = ( unsigned long )GetMP3FrameCount();//Get the Former Frame Counter
	USER_PauseA128();				//Store the Decode_Count value in MP3Decode_Length varible
	SACM_Stop();

	ulSkipData = (R_BitRate_A128 * milisecond) >> 3;	//in byte unit
	MP3Filelength = MP3Decode_Length << 1;				//MP3 current decode length in byte unit
	iTempSmpleRate = (unsigned int)A128_GetSampleRate();
	lplaydata = iTempSmpleRate * ( milisecond ) / 500;	//	/(1000/2)双声道要将播放的数据个数*2
	lplaydata = lplaydata / 1152;		//纯粹的根据时间偏移来计算

	if(orientation == C_Forward)
	{
		MP3Filelength += ulSkipData;
		if(MP3Filelength > MP3Former_Length) //MP3Former_Length is stored the length of the resource
		{
			SACM_Stop();
			return -1;
		}
		lseek(SACMFileHandle, MP3Filelength, SEEK_SET);
	}
	else if(orientation == C_Backward)
	{
		if( ulSkipData <= 0 )
		{
			return -2;
		}
		if(ulSkipData > MP3Filelength)
		{
			MP3Filelength = 0;
			ulSkipData = MP3Decode_Length << 1;
			lseek(SACMFileHandle, MP3Filelength, SEEK_SET);
		}
		else
		{
			MP3Filelength -= ulSkipData;
			lseek(SACMFileHandle, MP3Filelength, SEEK_SET);
		}
	}
	else
	{
		return -3;			//  parament erro
	}

	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);

	ulFramLength1 = ( unsigned long )GetMP3FrameCount();	//记录现在播放的MP3的FrameCountNumber

	if(orientation == C_Forward)
	{
		uldecode = MP3Decode_Length + (ulSkipData >> 1);
		User_NewDataMP3(uldecode, 0);
		ulFramLength1 += 0x0004;							//加上8作为长度补偿
		SetMP3FrameCount(ulFramLength1 + lplaydata);		//重新设置MP3的FrameCountNumber
	}
	else if(orientation == C_Backward)
	{
		if(ulFramLength1 < lplaydata)
		{
			lplaydata = ulFramLength1;
		}
		uldecode = MP3Decode_Length - (ulSkipData >> 1);
		User_NewDataMP3(uldecode, 0);
		SetMP3FrameCount(ulFramLength1 - lplaydata);		//重新设置MP3的FrameCountNumber
	}

	return 0;
}

//========================================================
// Function Name:	Snd_A128_PlayMemory
// Syntax:		Snd_A128_PlayMemory(unsigned long start_addr, unsigned long end_addr)
// Purpose: 	play resource from special memory 			 
// Note:		NO
// Parameter : 	_CodecType: select which algorith to play
//				_DataPtr:   the address of encoded data 
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_A128_PlayMemory(unsigned long start_addr, unsigned long end_addr)
{
	SACM_Codec(C_CODEC_A128);
	USER_SetA128MemoryAddr(start_addr, end_addr);             //in sacm_user.asm
	SACM_A128_SetFS();

	USER_SetA128MemoryAddr(start_addr, end_addr);
	
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
int Snd_A128_PlayFAT(const char *filename)
{
	struct stat_t filestatus;

	if(SACMFileHandle >= 0)
	{
		close(SACMFileHandle);
		SACMFileHandle = -1;
	}
	SACMFileHandle = open((LPSTR)filename,O_RDONLY );
	SACM_Codec(C_CODEC_A128);
	Fat_SetStartAddr(LB2LP(NFAT_DS_GetData));	

	stat(SACMFileHandle, &filestatus);
	A128_SetLength(filestatus.st_size>>1);	//The length is decided by the input size
	SACM_A128_SetFS();
	lseek(SACMFileHandle, 0, SEEK_SET);
	
	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);
	return TRUE;
}

#endif	//end of A128_Used
//======================For A128 Play seek time use 2007/09/05========================