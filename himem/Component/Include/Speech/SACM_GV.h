#ifndef	__SACM_GV_h__
#define	__SACM_GV_h__

#include "algorithm.h"

#ifdef    SYS_CREATEVAR_SACM
	#define SYS_EXT
#else
	#define SYS_EXT extern
#endif

//MP3ReadModeFlag表示读取文件的模式，是直接读取(0)还是利用Buffer缓冲(1), 还是从后台填充缓冲Buffer(2)
#ifdef MP3_FATREADBUFFER
SYS_EXT unsigned int pMP3Readptr, uiMP3WriteFlag;	//记录MP3读取从Buffer中读取的指针和向Buffer中填数据的Falg
SYS_EXT unsigned int MP3DecodeErrorFlag, MP3ReadModeFlag;//MP3DecodeErrorFlag在解码过程中出错时会被置位
SYS_EXT unsigned int R_MPEGType;				//MP3的MPEG Type类型
#endif

//-----------------------------------------
//global variable for PlayList.c
SYS_EXT List *MusicPlayList;				//播放链表的结点
SYS_EXT List *MusicPlayList_Head;			//记录List头结点

//SYS_EXT unsigned int MP3FileHandle;			//MP3文件句柄
SYS_EXT unsigned int MusicPlayMode;			//记录MP3的播放模式：无序 顺序 单曲 循环 Playlist
SYS_EXT unsigned int Snd_MP3_Status;		//记录MP3的播放状态
SYS_EXT unsigned int gCurSecondTime;		//当前播放时间的全局变量
SYS_EXT unsigned int Mp3_ContinueFlag;		//表示后台前台播放的全局变量

SYS_EXT unsigned int R_MPEGLayer;			//记录MP3的MPEG类型
SYS_EXT unsigned int R_SampleRate;			//记录MP3的SampleRate
SYS_EXT unsigned int R_MPEGBitRate;			//记录MP3的BitRate
SYS_EXT unsigned int R_MPEGType;			//MP3的MPEG Type类型

SYS_EXT unsigned long MP3Filelength;		//记录MP3文件长度的全局变量
SYS_EXT unsigned long gMP3FATLength;		//xiaolei add为了避免有些MP3解码结束了，但还照常去读文件的操作.
SYS_EXT unsigned long ulFrame_Count_MP3;	//MP3的FrameCount
SYS_EXT unsigned long gMP3_FileOffset;		//MP3文件开始的偏移
SYS_EXT unsigned long MP3Former_Length;		//用于MP3暂停发单词是记录MP3上次信息(Decode_Count&Decode_L)的全局变量
SYS_EXT unsigned long MP3Decode_Length;		//记录Decode的长度

SYS_EXT unsigned long ulFrame_CurrentCount;	//记录当前的FrameCount值的一个long型变量
SYS_EXT unsigned  long ulFrame_Count_A;		//记录A,B两点的FrameCount, 就是Decode之前的数据长度
SYS_EXT unsigned long ulFrame_Count_B;
SYS_EXT unsigned long gMP3_FramSizeA;		//记录A点的FrameCount,是解码之后播放的Frame值
//-----------------------------------------

SYS_EXT unsigned int SACM_A1800_Mode;		//The bitrate of A1800 Play
SYS_EXT unsigned int SACM_A4800_Mode;		//The Mode infomation of A4800 Play File

//-----------------------------------------
SYS_EXT unsigned int gCurA128SecondTime;	//A128 current play time
SYS_EXT extern unsigned int R_BitRate_A128;	//Add for A128 Play
//-----------------------------------------

//pRecWriteptr为记录从Buffer中读数据的指针,uiRecWriteFlag表示向NandFAT里写数据的Flag
SYS_EXT unsigned int pRecWriteptr;
SYS_EXT unsigned int uiRecWriteFlag;	

SYS_EXT unsigned int R_ShiftStore;
SYS_EXT unsigned int R_DataEmptyFlag;
SYS_EXT unsigned int R_SACM_DecodeData_In_PTR;
SYS_EXT unsigned int R_ISR_DataProc_Flag;			//这个变量用于表示是否正在读取或写入文件数据
SYS_EXT unsigned int R_MP3_Error_Count;				//这个变量用于记录解码过程中解码错误的次数
#endif	//end of __SACM_GV_h__