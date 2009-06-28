//***************************************************************************************** 
// Topic: ETTS.c code 
// Plateform: GPL162002/GPL162003
// Description: modify for GPL162002/GPL162003
// Note: N/A         
// Version: V1.0  
// Modify History:     V1.0 first edit special for GPL162002/GPL162003
//*****************************************************************************************

//*****************************************************************************
//*****************************************************************************
// Header File Included Area
//*****************************************************************************
//*****************************************************************************
#include ".\Component\Include\Component_head.h"
#include ".\Component\Include\Speech\ETTSv2.h"


//*****************************************************************************
//*****************************************************************************
// Constant Defintion Area
//*****************************************************************************
//*****************************************************************************
#ifdef ETTS_Used
#define C_ETTS_DEFAULT_SPEED	0
#define C_ETTS_DEFAULT_RATE			150				//The default Rate:150
#define C_ETTS_MIDDLE_RATE			152				//Very Near the default speed
#define C_ETTS_SPACE_HIGHRATE		10				//So, MaxSpeed = 152 + 12*10 = 272words/minute
#define C_ETTS_SPACE_LOWRATE		8				//So, MinSpeed = 152 - 12*8 = 56words/minute

#define CODE_IN_RAM_SIZE	768

//*****************************************************************************
//*****************************************************************************
// Variable Publication and External function & variable declaration Area
//*****************************************************************************
//*****************************************************************************
int SndSACM_curSpeedTTS = C_ETTS_DEFAULT_SPEED;
int SndSACM_CurToneTTS = PERS_MALE;
extern unsigned long ETTS_Code_InRam;
extern void ETTS_NotifyBusyFcn(short b);

//========================================================
// Function Name:	ETTS_Play_Flow
// Syntax:		ETTS_Play_Flow(unsigned long Text)
// Purpose: 	play ETTS			 
// Note:		NO
// Parameter : 	unsigned long Text: the text input
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_ETTS_Play(unsigned long Text)
{
	ETTSRESULT eResult;
	LPCSHORT CritCode_Start_Addr;
	unsigned int CritCode_Length;
	int SpeakSpeed;
	unsigned int *InRam_Ptr = &ETTS_Code_InRam;
	unsigned int i = 0;
	
	memset(0x16B0, 0, 0x11D1);								//it's important, you must sure the ORAM is clean	
	eResult = ETTS_Init( LB2LP(ETTS_NotifyBusyFcn) );
	if(ETTS_NOERROR != eResult)
	{
		return -1;
	}

//copy critical code into internal ram	
	CritCode_Start_Addr = Phn2Spch_GetDefCritCodeBlock(&CritCode_Length);
	if(CritCode_Length>CODE_IN_RAM_SIZE)
	{
		return -1;
	}
	
	while(CritCode_Length)
	{
		*InRam_Ptr++ = *( CritCode_Start_Addr + i );
		CritCode_Length--;
		i++;
	}
	
	if(!Phn2Spch_UseCritCodeBlock(&ETTS_Code_InRam))
	{
		return -1;
	}

//Set ETTS Speed and tone mode befor ETTS_Init	
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	if(0 == SndSACM_curSpeedTTS)
	{
		SpeakSpeed = C_ETTS_DEFAULT_RATE;
	}
	else if(SndSACM_curSpeedTTS > 0)
	{
		SpeakSpeed = C_ETTS_MIDDLE_RATE + SndSACM_curSpeedTTS * C_ETTS_SPACE_HIGHRATE;
	}
	else if(SndSACM_curSpeedTTS < 0)
	{
		SpeakSpeed = C_ETTS_MIDDLE_RATE + SndSACM_curSpeedTTS * C_ETTS_SPACE_LOWRATE;
	}
	Phn2Spch_SetRate(SpeakSpeed);
	Phn2Spch_SetPersonality(SndSACM_CurToneTTS);				//set tone mode
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		
	eResult = ETTS_Play( (ETTSLPSTR)Text, 0);
	if(ETTS_NOERROR != eResult)
	{
		return -1;
	}
	SACM_Codec(C_CODEC_ETTS);
	SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);
}

//========================================================
// Function Name:	Snd_SACM_SetSpeedTTS
// Syntax:		Snd_SACM_SetSpeedTTS( int SpeedIndex )
// Purpose: 	set ETTS play speed			 
// Note:		NO
// Parameter : 	int SpeedIndex: the index of speed
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void Snd_SACM_SetSpeedTTS( int SpeedIndex )
{
	int SpeakSpeed;
	SndSACM_curSpeedTTS = SpeedIndex - 12;
	if( SndSACM_curSpeedTTS>12 ) SndSACM_curSpeedTTS = 12;
	else if( SndSACM_curSpeedTTS<-12 ) SndSACM_curSpeedTTS = -12;

	if(0 == SndSACM_curSpeedTTS)
	{
		SpeakSpeed = C_ETTS_DEFAULT_RATE;
	}
	else if(SndSACM_curSpeedTTS > 0)
	{
		SpeakSpeed = C_ETTS_MIDDLE_RATE + SndSACM_curSpeedTTS * C_ETTS_SPACE_HIGHRATE;
	}
	else if(SndSACM_curSpeedTTS < 0)
	{
		SpeakSpeed = C_ETTS_MIDDLE_RATE + SndSACM_curSpeedTTS * C_ETTS_SPACE_LOWRATE;
	}
	Phn2Spch_SetRate(SpeakSpeed);
}

//========================================================
// Function Name:	Snd_SACM_GetSpeedTTS
// Syntax:		Snd_SACM_GetSpeedTTS(void)
// Purpose: 	get ETTS play speed			 
// Note:		NO
// Parameter : 	N/A
// Return:		int SpeedIndex: the index of speed
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
int Snd_SACM_GetSpeedTTS(void)
{
	int SpeedIndex;
	SpeedIndex = SndSACM_curSpeedTTS + 12;
	return SpeedIndex;
}

//========================================================
// Function Name:	Snd_SACM_SetToneTTS
// Syntax:		Snd_SACM_SetToneTTS( int personalities )
// Purpose: 	set ETTS play tone mode			 
// Note:		NO
// Parameter : 	int personalities: tone mode
// Return:		N/A
// Destory: 	N/A
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
void Snd_SACM_SetToneTTS( int personalities )
{
	if( (personalities>19)||(personalities<0) )
	{
		return -1;
	}
	SndSACM_CurToneTTS = personalities;
	Phn2Spch_SetPersonality(SndSACM_CurToneTTS);
}

#endif	//end of ETTS_Used