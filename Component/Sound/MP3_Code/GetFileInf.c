
#include ".\Component\Include\Component_head.h"
#include ".\System\Include\System_head.h"
//private
#include ".\Component\Include\speech\SACM_GV.h"	
//
#define ID3HEAD			"ID3"
#define ID3HEADLEN 		10
#define READLEN			1024
#define FRAMESIZE1		48000
#define FRAMESIZE2		144000
#define FRAMESIZE3		24000
#define FRAMESIZE4		72000
#define VBRHEAD			"Xing"	//抛弃了头为 info的情况( 非常少见 )
#define VBRIHEAD		"VBRI"
/**************************************************************/
extern unsigned long gMP3InfoBuffer;
//The Main API:
int GetMp3Info( const char * FilePath, st_MP3FILE_INFO *mp3file_info );

/**************************************************************/
//The Internal API:
static unsigned long GetId3v2Offset( unsigned int *pStr );
static int CmpHead( int fp, unsigned int *pStr, unsigned long CurOffset, st_MP3FILE_INFO *mp3file_info );
static int ObtainMP3Info(unsigned int *pHead,st_MP3FILE_INFO *mp3file_info);
static unsigned long GetVBRMp3TotalFrame( st_MP3FILE_INFO *pTmp, int fp, unsigned long offset );
static unsigned long ChCharToLong( char *ptr );
/***************************************************************/

//========================================================
// Function Name:	GetMp3Info
// Syntax:		GetMp3Info( const char * FilePath, st_MP3FILE_INFO *mp3file_info )
// Purpose: 	get the MP3 FAT information			 
// Note:		NO
// Parameter : 	char * FilePath: path name
//				t_MP3FILE_INFO *mp3file_info: MP3 FAT information struction
// Return:		-1: open FAT fail; 
//				-2: space is not enough;
//				-3: can't find the head of frame;
//				 0: successfull
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int GetMp3Info( const char * FilePath, st_MP3FILE_INFO *mp3file_info )
{
	int retval;
	int i=0;
	unsigned long j=0;
	int fp;
	unsigned long StrBuf;
	unsigned int Head[4];
	unsigned long Offset;
	unsigned long TotalFrame;
	struct stat_t filestatus;
	char FileNameBuffer[20];
	unsigned long length1, length2;
	
	memcpy(FileNameBuffer, FilePath, 20);
	fp = open((LPSTR)FileNameBuffer, O_RDONLY);
	if( fp== -1){
		return fp;
	}
	lseek( fp, 0, SEEK_SET );
	readB( fp, Head, 20 );
	if( memcmp( Head, ID3HEAD, 3)==0 ){			//ID3
		 Offset = GetId3v2Offset( (unsigned int*)(Head+6) );
		 lseek( fp, Offset+ID3HEADLEN, SEEK_SET );
		 readB( fp, Head, 4 );
 		 retval = CmpHead( fp, Head, Offset+ID3HEADLEN, mp3file_info );
		 if( retval==0 ){
			TotalFrame = GetVBRMp3TotalFrame( mp3file_info, fp, mp3file_info->firstframe );	//判断数据为VBR or CBR
			stat(FilePath,&filestatus);								//文件长度
			if( TotalFrame==-1 )											//CBR?
			{
				length1 = filestatus.st_size - (j*READLEN+i-j*3);
				length1 = length1 << 3;
				length2 = (long)mp3file_info->bitrate;
				length2 = length2 * (long)1000;
				mp3file_info->total_time = length1 / length2;
				//mp3file_info->total_time=(((filestatus.st_size-(j*READLEN+i-j*3))<<3)/(mp3file_info->bitrate*1000));//add
			}
			else
			{
				mp3file_info->total_time = (mp3file_info->frametime*TotalFrame+500000)/1000000;	//VBR
			}
			mp3file_info->filesize = filestatus.st_size;
			close(fp);
			return retval;
		 }
	}
	StrBuf = &gMP3InfoBuffer;
	if(!StrBuf)
	{
		return -2;
	}
	lseek( fp, 0, SEEK_SET );
	for( j=0; ; j++, lseek( fp, j*READLEN-j*3, SEEK_SET ) ){
		if(readB( fp, StrBuf, READLEN )!=READLEN ){
			close(fp);
			return -3;
		}
		for( i=0; i<READLEN; i++ ){
			retval = CmpHead( fp, (unsigned int*)(StrBuf+i), j*READLEN+i-j*3, mp3file_info );
			if(0 == retval)
			{
				TotalFrame = GetVBRMp3TotalFrame( mp3file_info, fp, mp3file_info->firstframe );
				stat(FilePath,&filestatus);
				if( TotalFrame==-1 )
				{
					length1 = filestatus.st_size - (j*READLEN+i-j*3);
					length1 = length1 << 3;
					length2 = (long)mp3file_info->bitrate;
					length2 = length2 * (long)1000;
					mp3file_info->total_time = length1 / length2;
					//mp3file_info->total_time = ((long)((filestatus.st_size-(j*READLEN+i-j*3))<<3))/((long)(mp3file_info->bitrate*1000));//add
				}
				else
				{
					mp3file_info->total_time = (mp3file_info->frametime*TotalFrame+500000)/1000000;
				}
				mp3file_info->filesize = filestatus.st_size;
				close(fp);
				return retval;
			}
		}
	}
	lseek( fp, -3, SEEK_CUR );
}

//========================================================
// Function Name:	GetId3v2Offset
// Syntax:		GetId3v2Offset( unsigned int *pStr )
// Purpose: 	get the offset from MP3 frame head			 
// Note:		NO
// Parameter : 	unsigned int *pStr: point to file position
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
static unsigned long GetId3v2Offset( unsigned int *pStr )	
{
	unsigned long ptr1, ptr2;
	ptr1 = *(pStr+3);
	ptr2 = *(pStr+2);
	ptr1 += ( ptr2&0x7f )<<7;
	ptr2 = *(pStr+1) ;
	ptr1 += ( ptr2&0x7f )<<14;
	ptr2 = *pStr;
	ptr1 += ( ptr2&0x7f )<<21;
	return ptr1;
}

//========================================================
// Function Name:	CmpHead
// Syntax:		CmpHead(int fp, unsigned int *pStr, unsigned long CurOffset, st_MP3FILE_INFO *mp3file_info)
// Purpose: 	check whether the MP3 frame head valid? 		 
// Note:		NO
// Parameter : 	int fp: file handle
//				unsigned int *pStr: point to the file position
//				unsigned long CurOffset: the current offset
//				st_MP3FILE_INFO *mp3file_info: the struction about MP3 file information
// Return:		 0: success
//				-1: false
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
static int CmpHead( int fp, unsigned int *pStr, unsigned long CurOffset, st_MP3FILE_INFO *mp3file_info )
{
	int StrTmp[5];

	if( ((*pStr&0xff)==0xff)&&((*(pStr+1)&0xe0)==0xe0)&&((*(pStr+2)&0xf0)!=0xf0)&&(*(pStr+2)!=0)){
		ObtainMP3Info( pStr,mp3file_info );
		lseek( fp, CurOffset+mp3file_info->framesize, SEEK_SET );
		readB( fp,StrTmp, 5 ); 
		if( ((((*(int *)StrTmp&0xff)==0xff)									//简单判断是否为有效帧
					&&((*(int*)(StrTmp+1)&0xe0)==0xe0)
					&&( (*(int*)(StrTmp+2)&0xf0)!=0xf0)&&((*(int *)(StrTmp+2))!=0)))
					||(((*(int *)(StrTmp+1)&0xff)==0xff) &&(((*(int*)(StrTmp+2)&0xe0)==0xe0)
					&&( (*(int*)(StrTmp+3)&0xf0)!=0xf0)&&((*(int *)(StrTmp+3))!=0)))){
			mp3file_info->firstframe = CurOffset;						//data 偏移
			return 0;
		}else
			return -1;
	}
	else
	{
		return -1;
	}
}

//========================================================
// Function Name:	ObtainMP3Info
// Syntax:		ObtainMP3Info(unsigned int *pHead,st_MP3FILE_INFO *mp3file_info)
// Purpose: 	get some information about MP3 file			 
// Note:		NO
// Parameter : 	unsigned int *pHead: point to some position of MP3 file
//				st_MP3FILE_INFO *mp3file_info: the struction about MP3 file information
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
static int ObtainMP3Info(unsigned int *pHead,st_MP3FILE_INFO *mp3file_info)
{
	unsigned int bitrateindex;	//kbps
	unsigned int frequence; 		//hz
	unsigned int row;
	unsigned int paddingbit;
	unsigned long frametype;
	unsigned long timetype;

	//{mpeg1,mpeg2,mpeg2.5}
	unsigned int sampleratetable[3][3]={{44100,48000,32000},{22050,24000,16000},{11025,12000,8000}};

	unsigned int BitRatTable[6][14]={
		{32,64,96,128,160,192,224,256,288,320,352,384,416,448}, 	//MPEG-1,layer I
		{32,48,56,64,80,96,112,128,160,192,224,256,320,384},		//MPEG-1,layer II
		{32,40,48,56,64,80,96,112,128,160,192,224,256,320},			//MPEG-1, layer III
		{32,48,56,64,80,96,112,128,144,160,176,192,224,256},		//MPEG-2,layer I
		{8,16,24,32,40,48,56,64,80,96,112,128,144,160},   			//M2 L2
		{8,16,24,32,40,48,56,64,80,96,112,128,144,160}};          	//M2 L3
	/*mpeglyer3---03,mpeglyer2---02,mpeglyer1---01*/	

	if(0x01 == ((*(pHead+1)>>3)&0x03))			//01
	{
		mp3file_info->mpegtype = 0xffff;
	}
	else if(0x02 == ((*(pHead+1)>>3)&0x03))		//10
	{
		mp3file_info->mpegtype = 0x02;
	}
	else if(0x03 == ((*(pHead+1)>>3)&0x03))		//11
	{
		mp3file_info->mpegtype = 0x01;
	}
	else if(0x00 == ((*(pHead+1)>>3)&0x03))		//00
	{
		mp3file_info->mpegtype = 0x25;				//表示为Mepg2.5
	}
	if(0x00 == ((*(pHead+1)>>1)&0x03))
	{
		mp3file_info->layer = 0xffff;
	}
	else
	{
		mp3file_info->layer = 0x04 - (*(pHead+1)>>1)&0x03;
	}
	bitrateindex=(*(pHead+2)>>4)&0x0f;
	frequence=(*(pHead+2)>>2)&0x03;
	paddingbit=(*(pHead+2)>>1)&0x01;

	if( mp3file_info->mpegtype==0x01||mp3file_info->mpegtype==0x03 )     	//mpeg1?  
		mp3file_info->samplerate=sampleratetable[0][frequence];
	else if(mp3file_info->mpegtype==0x02) 									//mpeg2?
		mp3file_info->samplerate=sampleratetable[1][frequence];
	else if(mp3file_info->mpegtype==0x25) //mpeg2.5?
		mp3file_info->samplerate=sampleratetable[2][frequence];

	if(mp3file_info->mpegtype == 0x01)     	//mpeg1?  
	{
		row=mp3file_info->layer-1;
	}
	else  //mpeg2?
	{
		row=mp3file_info->layer+2;
	}
	mp3file_info->bitrate=BitRatTable[row][bitrateindex-1];

	R_MPEGLayer=mp3file_info->layer;
	R_SampleRate=mp3file_info->samplerate;
	R_MPEGBitRate=mp3file_info->bitrate;
	R_MPEGType = mp3file_info->mpegtype;

	if( (R_MPEGType==0x01)||(R_MPEGType==0x03) ){  //mpeg 1
		if( R_MPEGLayer == 0x01 ){  								//layer 1
			frametype = FRAMESIZE1;
			timetype = 384;						//是解码后每一桢的Sample数
		}
		else if( (R_MPEGLayer==0x03)||(R_MPEGLayer==0x02) ){  		//layer2 layer3
			frametype = FRAMESIZE2;
			timetype = 1152;
		}
	}
	else if( (R_MPEGType==0x25)||(R_MPEGType==0x02) ){  //mpeg 2
		if( R_MPEGLayer == 0x01 ){  								//layer 1
			frametype = FRAMESIZE3;
			timetype = 384;
		}
		else if( (R_MPEGLayer==0x02)||(R_MPEGLayer==0x03) ){  		//layer2 layer3
			frametype = FRAMESIZE4;
			if( R_MPEGLayer==0x03 )									//layer 3
				timetype = 576;
			else													//layer 2
				timetype = 1152;
		}
	}
	//Get the length of the encode data before decode:
	mp3file_info->framesize = ( frametype * mp3file_info->bitrate ) / mp3file_info->samplerate+paddingbit;
	//The FrameTime may not be quite right:
	mp3file_info->frametime = ( timetype*1000000 )/mp3file_info->samplerate;			//frame time,精确到us级 (*1000000增加精确度)
	return 0;
}

//========================================================
// Function Name:	GetVBRMp3TotalFrame
// Syntax:		GetVBRMp3TotalFrame( st_MP3FILE_INFO *pTmp, int fp, unsigned long offset )
// Purpose: 	get the frame length of VBR NP3			 
// Note:		NO
// Parameter : 	st_MP3FILE_INFO *pTmp: the struction about MP3 file information
//				int fp: file handle
//				unsigned long offset: offset
// Return:		totalframe
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
static unsigned long GetVBRMp3TotalFrame( st_MP3FILE_INFO *pTmp, int fp, unsigned long offset )
{
	int tmpbuf[50];
	unsigned long totalframe;				//VBR VBRI   只有四钟情况所以未判断 直接读取

	lseek( fp, offset+4, SEEK_SET );
	readB( fp, (unsigned long)tmpbuf, 50 );
	if( memcmp((int*)(tmpbuf+32), VBRHEAD, 4 ) == 0 )
		totalframe = ChCharToLong( (char *)(tmpbuf+40) );
	else if( memcmp((int*)(tmpbuf+32), VBRIHEAD, 4 ) == 0 )
		totalframe = ChCharToLong( (char *)(tmpbuf+46) );
	else if( memcmp((int*)(tmpbuf+17), VBRHEAD, 4 ) == 0 )
		totalframe = ChCharToLong( (char *)(tmpbuf+25) );
	else if( memcmp((int*)(tmpbuf+9), VBRHEAD, 4 ) == 0 )
		totalframe = ChCharToLong( (char *)(tmpbuf+17) );
	else 
		totalframe = -1;
	return totalframe;
}

//========================================================
// Function Name:	ChCharToLong
// Syntax:		ChCharToLong( char *ptr )
// Purpose: 	change char to long			 
// Note:		NO
// Parameter : 	char *ptr
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
static unsigned long ChCharToLong( char *ptr )
{
	unsigned long tmp;
	unsigned long Val;

	tmp = *ptr;
	Val = tmp<<24;
	tmp = *(ptr+1);
	Val += tmp<<16;
	tmp = *(ptr+2);
	Val += tmp<<8;
	Val += *(ptr+3);
	
	return Val;
}
