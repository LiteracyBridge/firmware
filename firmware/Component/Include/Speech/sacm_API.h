#ifndef	__SACM_API_h__
#define	__SACM_API_h__
//	write your header here

#define Disable         	0
#define Enable          	1

#define Max_Volume			15	// Max. volume
#define Min_Volume			0	// Min. volume

#define Manual_Mode_Index	-1
#define SpeedDemo			2

#define	DAC1				1	// Channel A
#define DAC2				2	// Channel B

#define	PWM					1	// PWM
#define	DAC					2	// DAC

#define	ORDER_8				1	// Low Pass Filter with 8 Order
#define	ORDER_32			0	// Low Pass Filter with 32 Order

#define	Ramp_UpDn_Off		0
#define Ramp_Up_On			1
#define	Ramp_Dn_On			2
#define	Ramp_UpDn_On		3
#define	Ramp_Disable		0
#define Ramp_Up				1
#define	Ramp_Dn				2

#define	ForegroundPlay		0
#define	BackgroundPlay		1


//-----------------------------------
// For DVR Use Only
//-----------------------------------
#define	Microphone			0
#define	Mic	            0

#define	RecMonitorOff		0
#define	RecMonitorOn		1

// DVR 1600 bit rate
#define	BIT_RATE_10K        0
#define	BIT_RATE_12K        1
#define	BIT_RATE_14K        2
#define	BIT_RATE_16K        3
#define	BIT_RATE_20K 		4
#define BIT_RATE_24K        5

// DVR 4800 bit rate
#define	BIT_RATE_32K           		0
#define	BIT_RATE_36K        		1
#define	BIT_RATE_40K        		2
#define	BIT_RATE_44K        		3
#define	BIT_RATE_48K 		    	4
#define BIT_RATE_52K        		5
#define BIT_RATE_56K        		6

//-----------------------------------
// For MS02 Use Only
//-----------------------------------
#define MS02_DAC_8K  		0
#define MS02_DAC_10K		1
#define MS02_DAC_12K		2
#define MS02_DAC_16K		3
#define MS02_DAC_20K		4
#define MS02_DAC_24K		5
#define MS02_DAC_32K		6
#define MS02_DAC_44K		7
//ETTS Tone Voice Select Table
#define PERS_MALE			0					// default male voice
#define PERS_FEMALE			1					// default female voice
#define PERS_LARGEMALE		2					// default large male voice
#define PERS_CHILD			3					// default child voice
#define PERS_GIANTMALE		4					// low pitched male
#define PERS_MELLOWFEM		5					// sexy female
#define PERS_MELLOWMALE		6					// mellow male
#define PERS_CRISPMALE		7					// crisp, clear male
#define PERS_THEFLY			8					// high pitched, quavering voice
#define PERS_ROBOTOID		9					// robotoid
#define PERS_MARTIAN		10					// alien
#define PERS_COLOSSUS		11					// the voice of the Colossus computer
#define PERS_FASTFRED		12					// fast talking voice
#define PERS_OLDWOMAN		13					// elderly woman
#define PERS_MUNCHKIN		14					// dwarf
#define PERS_TROLL			15					// a little larger than a dwarf
#define PERS_NERD			16					// nerdish male
#define PERS_MILKTOAST		17					// whiney male
#define PERS_TIPSY			18					// drunken male
#define PERS_CHOIRBOY		19					// high pitched, sung voice

//The definition of return Value for Error:
#define 			RTN_NOERROR					0x0000	//No Error
#define				RTN_FILEERROR				0xffff	//ÎÄ¼þ³ö´í
//
//#define _Snd_Debug_ 			 				//for Debug used
/////////////SACM Flag///////////////////////////////////////////////////////////
#define C_SACM_PLAY         		0x0001
#define C_SACM_RECORD  	      	 	0x0002
#define C_SACM_PAUSE          	 	0x0004
#define C_SACM_CODEC_MODE			0x0800
#define C_SACM_STOP			        0x8000
//
////////////////Snd Status///////////////////////////////////////////////////////
//IS return by Snd_GetStatus()---->	define by user
#define	C_Snd_Stop				0		//Snd_GetStatus() return  C_Snd_Stop   while Sound is Stop
#define	C_Snd_Playing			1		//Snd_GetStatus() return  C_Snd_Playing  while Sound is Playing
#define	C_Snd_Pause				2		//Snd_GetStatus() return  C_Snd_Pause  while Sound is Pause
#define	C_Snd_DVRing			3		//Snd_GetStatus() return  C_Snd_DVRing while Sound is Recording
//
//*****************************************************************************
//*****************************************************************************
// Function Call (API) Declaration for C language
//*****************************************************************************
//*****************************************************************************
//=============================================================================
extern void SACM_Initial(void);
extern void Snd_VarInit(void);
extern int Snd_SACM_PlayMemory(int _CodecType, unsigned long start_addr);
extern int Snd_SACM_RecMemory(int _CodecType, unsigned long RECStartAddr, int BitRate);
extern int Snd_SACM_PlayFAT(int FileHandle, int _CodecType);		//Play SACM from FAT, exception MP3
extern int Snd_SACM_RecFAT(int FileHandle, int _CodecType, int BitRate);
extern int Snd_Stop(void);						// 
extern void SACM_Pause(void);					// 
extern void SACM_Resume(void);					// 
extern void SACM_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern void SACM_Speed(int SpeedIndex);			//0~24
extern unsigned int SACM_Status(void);			// 
//Add for Get the infomation of the current Play songs:
extern int SACM_Get_Codec( void );
extern int SACM_Get_Speed( void );

//For A18000 & A4800 Skip Play Function Use:
extern unsigned int SACMGet_A1800FAT_Mode( int handle, int offset );
extern unsigned int SACMGet_A4800FAT_Mode( int handle );
extern void SACM_DVR1800_BITRATE(int BitRate);			//added by chengye 2007/7/24
extern int SACM_A1800FAT_SeekTime( long milisecond, int orientation );
extern unsigned long Snd_A1800_GetCurrentTime(void);
//added by chengye 2007/4/25
extern unsigned int CheckStatus_Snd(void);
extern int NFAT_DS_GetData(LPTR dest_addr,unsigned int length);
extern void NFAT_DS_WriteData_Rec(LPTR dest_addr,unsigned int length);
extern void USER_SetFSLength(unsigned long);

//For ETTSV2 added by chengye 2007/4/20
extern int Snd_ETTS_Play(unsigned long Text);
extern void Snd_SACM_SetSpeedTTS( int SpeedIndex );	 //you can set ETTS speed from 0~24
extern int Snd_SACM_GetSpeedTTS( void );
extern void Snd_SACM_SetToneTTS( int personalities );//you can abey ETTS Tone Voice Select Table	

//for A128 play API
extern int Snd_A128_PlayMemory(unsigned long start_addr, unsigned long end_addr);
extern int Snd_A128_PlayFAT(const char *filename);
#endif
