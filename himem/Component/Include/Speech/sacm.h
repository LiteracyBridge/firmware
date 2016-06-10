#ifndef	__SACM_H__
#define	__SACM_H__
//	write your header here
//*****************************************************************************
//*****************************************************************************
// Constant Definition for C language
//*****************************************************************************
//*****************************************************************************
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

//*****************************************************************************
//*****************************************************************************
// Variable Declaration for C language
//*****************************************************************************
//*****************************************************************************
extern void USER_SetRECStartAddr(long);			//added by chengye 2006/8/28

//*****************************************************************************
//*****************************************************************************
// Variable Declaration for C language
//*****************************************************************************
//*****************************************************************************


extern void Set_Audio_Output_Mode(int);			// Option: PWM/DAC



//*****************************************************************************
//*****************************************************************************
// Function Call (API) Declaration for C language
//*****************************************************************************
//*****************************************************************************
//=============================================================================
// Progarm: SACM-S200 C API external definition
extern void SACM_S200_Initial();					// 
extern void SACM_S200_ServiceLoop(void);			// For Auto Mode only
extern void SACM_S200_Play(int,int,int);			// For Auto Mode only
extern void SACM_S200_Stop(void);					// 
extern void SACM_S200_Pause(void);					// 
extern void SACM_S200_Resume(void);					// 
extern void SACM_S200_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern unsigned int SACM_S200_Status(void);			// 
extern void SACM_S200_Speed(int);



//=============================================================================
// Progarm: SACM-S480/S720 C API external definition
extern void SACM_S4872_Initial();					// 
extern void SACM_S4872_ServiceLoop(void);			// For Auto Mode only
extern void SACM_S4872_Play(int,int,int);			// For Auto Mode only
extern void SACM_S4872_Stop(void);					// 
extern void SACM_S4872_Pause(void);					// 
extern void SACM_S4872_Resume(void);					// 
extern void SACM_S4872_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern unsigned int SACM_S4872_Status(void);			// 
extern void SACM_S4872_Speed(int);





//=============================================================================
// Progarm: SACM-S530 C API external definition
extern void SACM_S530_Initial();					// 
extern void SACM_S530_ServiceLoop(void);			// For Auto Mode only
extern void SACM_S530_Play(int,int,int);			// For Auto Mode only
extern void SACM_S530_Stop(void);					// 
extern void SACM_S530_Pause(void);					// 
extern void SACM_S530_Resume(void);					// 
extern void SACM_S530_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern unsigned int SACM_S530_Status(void);			// 
extern void SACM_S530_Speed(int);



//=============================================================================
// Progarm: SACM-A1600 C API external definition
extern void SACM_A1600_Initial();					// 
extern void SACM_A1600_ServiceLoop(void);			// For Auto Mode only
extern void SACM_A1600_Play(int,int,int);			// For Auto Mode only
extern void SACM_A1600_Stop(void);					// 
extern void SACM_A1600_Pause(void);					// 
extern void SACM_A1600_Resume(void);				// 
extern void SACM_A1600_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern unsigned int SACM_A1600_Status(void);		// 
extern void SACM_A1600_Speed(int);




//=============================================================================
// Progarm: SACM-DVR1600 C API external definition
extern void SACM_DVR1600_Initial();					// 
extern void SACM_DVR1600_ServiceLoop(void);			// For Auto/Manual Mode only
extern void SACM_DVR1600_Rec(int RceMonitor,int ADC_Channel,int bit_rate); // For Manual Mode only
extern void SACM_DVR1600_Play(int,int,int);			// For Auto/Manual Mode 
extern void SACM_DVR1600_Stop(void);					// 
extern void SACM_DVR1600_Pause(void);					// 
extern void SACM_DVR1600_Resume(void);					// 
extern void SACM_DVR1600_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern unsigned int SACM_DVR1600_Status(void);
extern void SACM_DVR1600_Speed(int);



//////////////////////////////////////////////////////////////////////////////////////////
// Progarm: SACM-DVR4800 C API external definition for sacmVxx.lib
//////////////////////////////////////////////////////////////////////////////////////////

extern void SACM_DVR4800_Initial();					// 
extern void SACM_DVR4800_ServiceLoop(void);			// For Auto/Manual Mode only
//extern void SACM_DVR4800_Rec(int RceMonitor,int ADC_Channel,int bit_rate); // For Manual Mode only
extern void SACM_DVR4800_Play(int,int,int);			// For Auto/Manual Mode 
extern void SACM_DVR4800_Stop(void);					// 
extern void SACM_DVR4800_Pause(void);					// 
extern void SACM_DVR4800_Resume(void);					// 
extern void SACM_DVR4800_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern void SACM_DVR4800_Speed(int);					// -12(Slow) ~ 0(Normal) ~ 12(Fast) 
extern unsigned int SACM_DVR4800_Status(void);
 
extern void SACM_DVR4800_Frame_Hop_Init(void);          // For forward backward function





//////////////////////////////////////////////////////////////////////////////////////////
// Progarm: Multi-SACM C API external definition for sacmVxx.lib
//////////////////////////////////////////////////////////////////////////////////////////

extern void SACM_XXXX_Initial();					 
extern void SACM_XXXX_ServiceLoop(void);						// For Auto/Manual Mode only
extern void SACM_XXXX_Play(int,int,int);						// For Auto/Manual Mode 
extern void SACM_XXXX_Stop(void);					  
extern void SACM_XXXX_Pause(void);					   
extern void SACM_XXXX_Resume(void);					  
extern void SACM_XXXX_Volume(int);								// 0(Normal) ~ 15(Fast) 
extern unsigned int SACM_XXXX_Status(void);
extern void SACM_XXXX_Codec(int);





//=============================================================================
// Progarm: SACM-MS01 C API external definition
extern void SACM_MS01_Initial();					// 
extern void SACM_MS01_ServiceLoop(void);			// For Auto Mode only
extern void SACM_MS01_Play(int,int,int);			// For Auto Mode only
extern void SACM_MS01_Stop(void);					// 
extern void SACM_MS01_Pause(void);					// 
extern void SACM_MS01_Resume(void);					// 
extern void SACM_MS01_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern unsigned int SACM_MS01_Status(void);			// 





//=============================================================================
// Progarm: SACM-MS02 C API external definition
extern int SACM_MS02_Initial();
extern void SACM_MS02_ServiceLoop(void);			// For Auto Mode only
extern void SACM_MS02_Play(int,int,int);			// For Auto Mode only
extern void SACM_MS02_Stop(void);					// 
extern void SACM_MS02_Pause(void);					// 
extern void SACM_MS02_Resume(void);					// 
extern void SACM_MS02_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern unsigned int SACM_MS02_Status(void);			// 




//=============================================================================
// Progarm: SACM-MS02 C API external definition
extern int SACM_MS02_Initial();
extern void SACM_MS02_ServiceLoop(void);			// For Auto Mode only
extern void SACM_MS02_Play(int,int,int);			// For Auto Mode only
extern void SACM_MS02_Stop(void);					// 
extern void SACM_MS02_Pause(void);					// 
extern void SACM_MS02_Resume(void);					// 
extern void SACM_MS02_Volume(int);					// 0(Normal) ~ 15(Fast) 
extern unsigned int SACM_MS02_Status(void);			// 



//=============================================================================
extern void SACM_RampUpDAC1(void);
extern void SACM_RampUpDAC2(void);
extern void SACM_RampDnDAC1(void);
extern void SACM_RampDnDAC2(void);







//=============================================================================
extern void	SACM_DisableUpSampleFilter(void);
extern void	SACM_EnableUpSampleFilter(int);







#endif
//=============================================================================
// End of SACM.h
//=============================================================================
