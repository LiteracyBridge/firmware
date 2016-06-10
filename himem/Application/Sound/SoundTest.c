
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//test for Sound added by chengye 2007/4/24
//speech define
#define MaxSpeechNum	5 								// Max. of speech in resource
#define MaxBIT_RATE 	5 								// Max. of bitrate
#define MaxVolumeNum   16 								// Max. of volume settings
#define MaxCodecNum     9                               // Max. of codec type  //add by benwang 20051025
#define	DAC1			1 								// DAC1 output
#define DAC2			2 								// DAC2 output
#define RECStartAddr	0x230000

#define MaxSpeed        24
#define MinSpeed        0
#define MaxPitch0Num    5
#define MaxPitch1Num    5
#define MaxVoiceNum     5
#define MaxPitchNum    	24
#define MaxSpectrumNum	24

int FileHandle = -1;
//char FileNameBuffer1[15];
//char FileNameBuffer2[15];
//char FileNameBuffer3[15];
List pTemp1;
List pTemp2;
List pTemp3;
List *TempMP3List;
//char MP3ListBuffer[50];

//speech varaable
int SpeechIndex;
int SpeedIndex;
int VolumeIndex;
int BIT_RATE; 			//BIT_RATE_32K; 	// BIT_RATE_(10K, 12K, 14K, 16K, 20K, 24K) //add by benwang 2005/07/26
int BitRate;
int	CodecType;
int	MP3PlayListFlag;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
extern unsigned long _RES_AIRPORT_S32_sa;
void KeyProc_Sound(void);
void SoundTest(void)
{
	unsigned int i,Key;
	int Buffer[2];
	int EQ_Mode;
	int PlayTimeSecond, k, j, m;
	int MP3SpeechFlag;
	int MP3PauseFlag;
	int MP3PlayNumber;
	int MP3EQMode, ret;
	int CurrentTime;
	unsigned long Play_Offset, Play_length;	//for test Play MP3 FAT Time Offset
	unsigned int flag;										//for test Play MP3 FAT Time Offset
	struct f_info File;
	struct stat_t filestatus;
	st_MP3FILE_INFO mp3_info;

	char StrInput[200];
	
	SysDisableWaitMode(WAITMODE_CHANNEL_A);
	System_Initial();
	
	Key = 0;
	i = 0;
	j = 0;
	k = 0;
	m = 0;
	SpeechIndex = 0;
	SpeedIndex = 12; 								// MaxSpeedHalf;
	VolumeIndex = 11;
	MP3SpeechFlag = 0;
	MP3PauseFlag = 0;
	MP3EQMode = 0;
	BIT_RATE=0;
	MP3PlayNumber = 0;

//	TempMP3List = MP3ListBuffer;
//	memcpy(FileNameBuffer1, "a:\\djrm.mp3", sizeof("a:\\djrm.mp3"));
//	memcpy(FileNameBuffer2, "a:\\zzzwx.mp3", sizeof("a:\\zzzwx.mp3"));
//	memcpy(FileNameBuffer3, "a:\\xkfy.mp3", sizeof("a:\\xkfy.mp3"));
//
//	CodecType = C_CODEC_AUDIO4800;
//	CodecType = C_CODEC_MP3;
//	CodecType = C_CODEC_MS01;
//	CodecType = C_CODEC_S530;
	CodecType = C_CODEC_AUDIO1800;
//	CodecType = C_CODEC_DVR520;
//	CodecType = C_CODEC_S320;
//	CodecType = C_CODEC_A128;

//for MP3 Play from Fat Init
//	MP3ListInit( &TempMP3List );									//initial playlist
//	MediaPlayer_SetPlayMode(C_PLAY_NORMAL  | C_PLAY_REPEAT );		//choose play mode

//	SACM_Codec(CodecType);
	Snd_SetVolume(VolumeIndex);

	if (CodecType == C_CODEC_MP3)
	{
		USER_SetMP3StartAddr(SpeechIndex);
		SACM_MP3_SetFS();
	}
//	SACM_Play(SpeechIndex, DAC1+DAC2 ,0);

//Get MP3 Information function test:
//	ret =  GetMp3Info(FileNameBuffer1, &mp3_info);
//	ret =  GetMp3Info(FileNameBuffer2, &mp3_info);

//MP3 Play function test:
//	Snd_MP3SpeedEnable();
//	Snd_PlayMP3FAT(FileNameBuffer1);
//	MediaPlayer_PlayList(TempMP3List, 0);
//	Snd_ETTS_Play("This is the TTS engine speaking. I sound just like the PC version.");
	
//for test Play MP3 FAT Time Offset
//	Snd_PlayMP3FATTimeOffset(FileNameBuffer1, Play_Offset, Play_length, flag);	
//test end

//****************************************************************************
//test for A128 play
	while (1)
	{
		if(Snd_GetStatus()== C_Snd_Stop)
		{
		
			Snd_Stop();
			
			ret = _findnext(&File);				
			if (ret != 0)
			{
				ret = _findfirst((unsigned long)("*.a64"), &File, D_ALL);
			}
			
			if(ret == 0)
			{
				CodecType = C_CODEC_A128;				
				Snd_A128_PlayFAT(File.f_name);
			}
		}
	}

//****************************************************************************

	while(1)                                        //key function
	{
		Key = SP_GetCh();
		switch(Key)
		{
			case  0x00:
				break;

			case  0x01:								//record in memory
				Snd_Stop();
				if(0 == i)
				{
					CodecType = C_CODEC_AUDIO1800;
			    	i++;
				}
				else if(1 == i)
				{
					CodecType = C_CODEC_SPEECH1600;
					i++;
				}
				else if(2 == i)
				{
					CodecType = C_CODEC_AUDIO4800;
					i++;
				}
				else if(3 == i)
				{
					CodecType = C_CODEC_DVR520;
					i++;
				}
				else if(4 == i)
				{
					CodecType = C_CODEC_WAV;
					BIT_RATE = WAVE_FORMAT_ADPCM;
					i = 0;
				}
				Snd_SACM_RecMemory(CodecType, RECStartAddr, BIT_RATE);
				break;

			case  0x02:								//play record from memory
				Snd_Stop();
				Snd_SACM_PlayMemory(CodecType, RECStartAddr);
				break;

			case  0x04:								//record to FAT
				Snd_Stop();
				if(0 == k)
				{
					CodecType = C_CODEC_AUDIO1800;
					unlink("B:\\REC.a18");
					FileHandle = open((LPSTR)("B:\\REC.a18"),O_CREAT|O_RDWR);
					k++;
					
				}
				else if(1 == k)
				{
					CodecType = C_CODEC_SPEECH1600;
					unlink("B:\\REC.a16");
					BIT_RATE = 3;	
					FileHandle = open((LPSTR)("B:\\REC.a16"),O_CREAT|O_RDWR);
					k++;
				}
				else if(2 == k)
				{
					CodecType = C_CODEC_AUDIO4800;
					unlink("B:\\REC.a48");
					FileHandle = open((LPSTR)("B:\\REC.a48"),O_CREAT|O_RDWR);
					k++;
				}
				else if(3 == k)
				{
					CodecType = C_CODEC_DVR520;
					unlink("B:\\REC.s52");
					FileHandle = open((LPSTR)("B:\\REC.s52"),O_CREAT|O_RDWR);
					k++;
				} 
				else if(4 == k)
				{
					CodecType = C_CODEC_WAV;
					BIT_RATE = WAVE_FORMAT_ADPCM;
					unlink("A:\\REC.wav");
					FileHandle = open((LPSTR)("A:\\REC.wav"),O_CREAT|O_RDWR);
					k = 0;
				} 
				Snd_SACM_RecFAT(FileHandle, CodecType, BIT_RATE); 	// mic, bitrate
				break;

			case  0x08:								//play record from FAT
				Snd_Stop();
				if(CodecType == C_CODEC_AUDIO1800) 
				{
					FileHandle = open((LPSTR)("B:\\REC.a18"),O_RDWR);
					stat((LPSTR)("B:\\REC.a18"),&filestatus);
				}
				else if(CodecType == C_CODEC_AUDIO4800) 
				{
					FileHandle = open((LPSTR)("B:\\REC.a48"),O_RDWR);
					stat((LPSTR)("B:\\REC.a48"),&filestatus);
				}
				else if(CodecType == C_CODEC_SPEECH1600) 
				{
					FileHandle = open((LPSTR)("B:\\REC.a16"),O_CREAT|O_RDWR);
					stat((LPSTR)("B:\\REC.a16"),&filestatus);
				}
				else if(CodecType == C_CODEC_DVR520) 
				{
					FileHandle = open((LPSTR)("B:\\REC.s52"),O_CREAT|O_RDWR);
					stat((LPSTR)("B:\\REC.s52"),&filestatus);
				}
				else if(CodecType == C_CODEC_WAV) 
				{
					FileHandle = open((LPSTR)("A:\\REC.wav"),O_CREAT|O_RDWR);
					stat((LPSTR)("A:\\REC.wav"),&filestatus);
				}
				Snd_SACM_PlayFAT(CodecType,FileHandle);
				break;

			case 0x10:						//play MP3 List
				Snd_Stop();
				MP3PlayListFlag = 1;
				MediaPlayer_PlayList(TempMP3List, 0);
				break;

			case 0x20:						//play from FAT
				Snd_Stop();
				if(0 == j)
				{
					CodecType = C_CODEC_AUDIO4800;
					FileHandle = open((unsigned long)("A:\\Musicold.a48"),O_RDWR);
					j++;
				}
				else if(1 == j)
				{
					CodecType = C_CODEC_AUDIO1800;
					FileHandle = open((unsigned long)("A:\\dunk2_16.a18"),O_RDWR);
					j++;
				}
				else if(2 == j)
				{
					CodecType = C_CODEC_SPEECH1600;
					FileHandle = open((unsigned long)("A:\\Eng.a16"),O_RDWR);
					j++;
				}
				else if(3 == j)
				{
					CodecType = C_CODEC_MS01;
					FileHandle = open((unsigned long)("A:\\DECK1.BIN"),O_RDWR);
					j++;
				}
				else if(4 == j)
				{
					CodecType = C_CODEC_MS02;
					FileHandle = open((unsigned long)("A:\\mb.bin"),O_RDWR);
					j++;
				}
				else if(5 == j)
				{
					CodecType = C_CODEC_S200;
					FileHandle = open((unsigned long)("A:\\lrc_800.s20"),O_RDWR);
					j++;
				}
				else if(6 == j)
				{
					CodecType = C_CODEC_S530;
					FileHandle = open((unsigned long)("A:\\otto.s53"),O_RDWR);
					j++;
				}
				else if(7 == j)
				{
					CodecType = C_CODEC_DVR520;
					FileHandle = open((unsigned long)("A:\\0503b.s52"),O_RDWR);
					j++;
				}
				else if(8 == j)
				{
					CodecType = C_CODEC_S320;
					FileHandle = open((unsigned long)("A:\\afternoon.s32"), O_RDWR);
					j++;
				}
				else if(9 == j)
				{
					Snd_A128_PlayFAT("A:\\story01.a64");
					j++;
					break;
				} 
				else if(10 == j)
				{
					CodecType = C_CODEC_S880;
					FileHandle = open((unsigned long)("A:\\FEMALE_8.cod"), O_RDWR);
					j = 0;
				}
				Snd_SACM_PlayFAT(CodecType, FileHandle);
				break;

			case 0x40:						//MP3 pause and resume
				if(0 == k)
				{
					Snd_MP3Speech_Pause();				
					Snd_SACM_PlayMemory(C_CODEC_S320_TTS,&_RES_AIRPORT_S32_sa);
					SpeechIndex++;
					if( SpeechIndex>=32 )
					{
						SpeechIndex = 0;
					}
					k++;
				}
				else if(1 == k)
				{
					Snd_MP3Speech_Resume();
					k = 0;
				}
				break;
				
			case 0x80:						// Speech up	
				SpeedIndex++;
			   	if(SpeedIndex > MaxSpeed)
			    {
			  		SpeedIndex = MinSpeed;
			    }
			 	Snd_SetSpeed(SpeedIndex);
				break;
				
			default:
				break;
		}			
	
		System_ServiceLoop();
		if(C_Snd_Stop == Snd_GetStatus())	//MP3播放下一首歌曲的测试程序
		{
			if(MP3PlayListFlag)
			{
				PlayNextFile();
			}
		}
	}
	return 0;
}

/* void MP3ListInit( List ** pTempList)
{
	pTemp1. MusicFilePath = FileNameBuffer1;
	pTemp1. FileID = 0;
	pTemp1. Next = &pTemp2;
	pTemp1. Back = 0;
	pTemp1. Param = 0;

	pTemp2. MusicFilePath = FileNameBuffer2;
	pTemp2. FileID = 1;
	pTemp2. Next = &pTemp3;
	pTemp2. Back = &pTemp1;
	pTemp2. Param = 0;

	pTemp3. MusicFilePath = FileNameBuffer3;
	pTemp3. FileID = 2;
	pTemp3. Next = 0;
	pTemp3. Back = &pTemp2;
	pTemp3. Param = 0;

	(*pTempList) = &pTemp1;
	return 0;
} */