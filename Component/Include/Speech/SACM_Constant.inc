// Constants Definition for R_PlayFlag
.define C_SACM_PLAY         		0x0001 
.define C_SACM_RECORD  	      	 	0x0002
.define C_SACM_REC_Mon_ON     		0x0002
.define C_SACM_PAUSE          	 	0x0004	
.define	C_SACM_RAMP_DN_ENABLE		0x0008
.define C_SACM_RAMP_UP_ENABLE		0x0010	
.define C_SACM_ENABLE_DAC1	    	0x0020   
.define C_SACM_ENABLE_DAC2			0x0040
.define	C_SACM_AUTO		    		0x0080
//.define	C_SACM_RESERVE_2	
		
.define C_SACM_DECODE_WORK	     	0x0100 
.define	C_SACM_DECODE_ODD	 		0x0200
.define C_SACM_DECODE_END	      	0x0400  
//.define	C_SACM_DECODE_RESERVE_1		0x0800	
.define C_SACM_CODEC_MODE			0x0800
.define C_SACM_FIQ_SOUND	    	0x1000  
.define	C_SACM_FIQ_EVEN				0x2000
.define	C_SACM_ISR_ServiceLoop    	0x4000
//.define C_SACM_XXXX_FIQ_RESERVE_2		0x8000	
.define C_SACM_STOP			        0x8000

//**************************************
//*************************************	//multi-algorithm definition
.define C_SACM_DVR1600_PLAY         		C_SACM_PLAY 
//.define C_SACM_DVR1600_RECORD         	C_SACM_RECORD
.define C_SACM_DVR1600_REC_Mon_ON     		C_SACM_REC_Mon_ON
.define C_SACM_DVR1600_PAUSE          	 	C_SACM_PAUSE	
.define	C_SACM_DVR1600_RAMP_DN_ENABLE		C_SACM_RAMP_DN_ENABLE
.define C_SACM_DVR1600_RAMP_UP_ENABLE		C_SACM_RAMP_UP_ENABLE	
.define C_SACM_DVR1600_ENABLE_DAC1	    	C_SACM_ENABLE_DAC1   
.define C_SACM_DVR1600_ENABLE_DAC2			C_SACM_ENABLE_DAC2
.define	C_SACM_DVR1600_AUTO	    			C_SACM_AUTO
//.define	C_SACM_RESERVE_2			
.define C_SACM_DVR1600_DECODE_WORK     		C_SACM_DECODE_WORK 
.define	C_SACM_DVR1600_DECODE_ODD	 		C_SACM_DECODE_ODD
.define C_SACM_DVR1600_DECODE_END      		C_SACM_DECODE_END  
//.define	C_SACM_DVR1600_DECODE_RESERVE_1	0x0800	
.define C_SACM_DVR1600_CODEC_MODE			C_SACM_CODEC_MODE
.define C_SACM_DVR1600_FIQ_SOUND	    	C_SACM_FIQ_SOUND  
.define	C_SACM_DVR1600_FIQ_EVEN				C_SACM_FIQ_EVEN
.define	C_SACM_DVR1600_ISR_ServiceLoop    	C_SACM_ISR_ServiceLoop
//.define C_SACM_DVR1600_FIQ_RESERVE_2		0x8000	
.define C_SACM_DVR1600_STOP		       		C_SACM_STOP

//*************************************
//*************************************
.define C_SACM_S200_PLAY         		C_SACM_PLAY 
.define C_SACM_S200_RECORD         		C_SACM_RECORD
.define C_SACM_S200_PAUSE          	 	C_SACM_PAUSE	
.define	C_SACM_S200_RAMP_DN_ENABLE		C_SACM_RAMP_DN_ENABLE
.define C_SACM_S200_RAMP_UP_ENABLE		C_SACM_RAMP_UP_ENABLE	
.define C_SACM_S200_ENABLE_DAC1	    	C_SACM_ENABLE_DAC1   
.define C_SACM_S200_ENABLE_DAC2			C_SACM_ENABLE_DAC2
.define	C_SACM_S200_AUTO	    		C_SACM_AUTO
//.define	C_SACM_RESERVE_2			
.define C_SACM_S200_DECODE_WORK     	C_SACM_DECODE_WORK 
.define	C_SACM_S200_DECODE_ODD	 		C_SACM_DECODE_ODD
.define C_SACM_S200_DECODE_END      	C_SACM_DECODE_END  
.define	C_SACM_S200_DECODE_RESERVE_1	0x0800	
.define C_SACM_S200_FIQ_SOUND	    	C_SACM_FIQ_SOUND  
.define	C_SACM_S200_FIQ_EVEN			C_SACM_FIQ_EVEN
.define	C_SACM_S200_ISR_ServiceLoop    	C_SACM_ISR_ServiceLoop
.define C_SACM_S200_FIQ_RESERVE_2		0x8000	

//
//.define C_DECODE_IN_LENGTH 60
//.define C_DECODE_OUT_LENGTH 400
//.define C_DECODE_OUT_LENGTH 320
//.define C_DECODE_OUT_LENGTH 60

.DEFINE   AZ_ORDER    10   // (16.0), unsigned
.DEFINE   CST2         5   // (16.0), unsigned

//add by benwang 20051020
//*************************************
.define C_SACM_S320_PLAY         		C_SACM_PLAY 
.define C_SACM_S320_RECORD         		C_SACM_RECORD
.define C_SACM_S320_PAUSE          	 	C_SACM_PAUSE	
.define	C_SACM_S320_RAMP_DN_ENABLE		C_SACM_RAMP_DN_ENABLE
.define C_SACM_S320_RAMP_UP_ENABLE		C_SACM_RAMP_UP_ENABLE	
.define C_SACM_S320_ENABLE_DAC1	    	C_SACM_ENABLE_DAC1   
.define C_SACM_S3200_ENABLE_DAC2			C_SACM_ENABLE_DAC2
.define	C_SACM_S320_AUTO	    		C_SACM_AUTO
//.define	C_SACM_RESERVE_2			
.define C_SACM_S320_DECODE_WORK     	C_SACM_DECODE_WORK 
.define	C_SACM_S320_DECODE_ODD	 		C_SACM_DECODE_ODD
.define C_SACM_S320_DECODE_END      	C_SACM_DECODE_END  
.define	C_SACM_S320_DECODE_RESERVE_1	0x0800	
.define C_SACM_S320_FIQ_SOUND	    	C_SACM_FIQ_SOUND  
.define	C_SACM_S320_FIQ_EVEN			C_SACM_FIQ_EVEN
.define	C_SACM_S320_ISR_ServiceLoop    	C_SACM_ISR_ServiceLoop
.define C_SACM_S320_FIQ_RESERVE_2		0x8000	

//***********************************
//*********************************
.define C_SACM_S4872_PLAY         		C_SACM_PLAY 
.define C_SACM_S4872_RECORD         		C_SACM_RECORD
.define C_SACM_S4872_PAUSE          	 	C_SACM_PAUSE	
.define	C_SACM_S4872_RAMP_DN_ENABLE		C_SACM_RAMP_DN_ENABLE
.define C_SACM_S4872_RAMP_UP_ENABLE		C_SACM_RAMP_UP_ENABLE	
.define C_SACM_S4872_ENABLE_DAC1	    	C_SACM_ENABLE_DAC1   
.define C_SACM_S4872_ENABLE_DAC2			C_SACM_ENABLE_DAC2
.define	C_SACM_S4872_AUTO	    		C_SACM_AUTO
//.define	C_SACM_RESERVE_2			
.define C_SACM_S4872_DECODE_WORK     	C_SACM_DECODE_WORK 
.define	C_SACM_S4872_DECODE_ODD	 		C_SACM_DECODE_ODD
.define C_SACM_S4872_DECODE_END      	C_SACM_DECODE_END  
.define	C_SACM_S4872_DECODE_RESERVE_1	0x0800	
.define C_SACM_S4872_FIQ_SOUND	    	C_SACM_FIQ_SOUND  
.define	C_SACM_S4872_FIQ_EVEN			C_SACM_FIQ_EVEN
.define	C_SACM_S4872_ISR_ServiceLoop    	C_SACM_ISR_ServiceLoop
.define C_SACM_S4872_FIQ_RESERVE_2		0x8000	

//S4872 related//
//.define _ORDER         10 ;
//.define _LTPLength    148 ;
//.define _SubFrame60    60 ;
//.define _SubFrame40    40 ;
//.define  C_CmdMask    0x3;

//************************************
//**************************************
.define C_SACM_S530_PLAY         		C_SACM_PLAY 
.define C_SACM_S530_RECORD         		C_SACM_RECORD
.define C_SACM_S530_PAUSE          	 	C_SACM_PAUSE	
.define	C_SACM_S530_RAMP_DN_ENABLE		C_SACM_RAMP_DN_ENABLE
.define C_SACM_S530_RAMP_UP_ENABLE		C_SACM_RAMP_UP_ENABLE	
.define C_SACM_S530_ENABLE_DAC1	    	C_SACM_ENABLE_DAC1   
.define C_SACM_S530_ENABLE_DAC2			C_SACM_ENABLE_DAC2
.define	C_SACM_S530_AUTO	    		C_SACM_AUTO
//.define	C_SACM_RESERVE_2			
.define C_SACM_S530_DECODE_WORK     	C_SACM_DECODE_WORK 
.define	C_SACM_S530_DECODE_ODD	 		C_SACM_DECODE_ODD
.define C_SACM_S530_DECODE_END      	C_SACM_DECODE_END  
.define	C_SACM_S530_DECODE_RESERVE_1	0x0800	
.define C_SACM_S530_FIQ_SOUND	    	C_SACM_FIQ_SOUND  
.define	C_SACM_S530_FIQ_EVEN			C_SACM_FIQ_EVEN
.define	C_SACM_S530_ISR_ServiceLoop    	C_SACM_ISR_ServiceLoop
.define C_SACM_S530_FIQ_RESERVE_2		0x8000	

//S530 related//
//.define _ORDER         10 ;
//.define _LTPLength    145 ;
//.define _SubFrame60    60 ;
//.define _SubFrames   	4 ;

// LTP constants
//.define  PitchMin      18
//.define  PitchMax      (PitchMin+127)
//.define  ClPitchOrd    5
//.define  NbFilt170     170
//.define  LspPrd0       0x3000
//.define  Scon0         0x0100

// Gain constant
//.define  NumOfGainLev  24
//.define  Factor24      0xaab  //(1/24 = 2731/65536)
//.define LspStabTest //added by benwang 2004/04/14

//*********************************
//*********************************
.define C_SACM_MS01_PLAY         		C_SACM_PLAY 
.define C_SACM_MS01_RECORD         		C_SACM_RECORD
.define C_SACM_MS01_PAUSE          	 	C_SACM_PAUSE	
.define	C_SACM_MS01_RAMP_DN_ENABLE		C_SACM_RAMP_DN_ENABLE
.define C_SACM_MS01_RAMP_UP_ENABLE		C_SACM_RAMP_UP_ENABLE	
.define C_SACM_MS01_ENABLE_DAC1	    	C_SACM_ENABLE_DAC1   
.define C_SACM_MS01_ENABLE_DAC2			C_SACM_ENABLE_DAC2
.define	C_SACM_MS01_AUTO	    		C_SACM_AUTO
//.define	C_SACM_RESERVE_2			
.define C_SACM_MS01_DECODE_WORK     	C_SACM_DECODE_WORK 
.define	C_SACM_MS01_DECODE_ODD	 		C_SACM_DECODE_ODD
.define C_SACM_MS01_DECODE_END      	C_SACM_DECODE_END  
.define	C_SACM_MS01_DECODE_RESERVE_1	0x0800	
.define C_SACM_MS01_FIQ_SOUND	    	C_SACM_FIQ_SOUND  
.define	C_SACM_MS01_FIQ_EVEN			C_SACM_FIQ_EVEN
.define	C_SACM_MS01_ISR_ServiceLoop    	C_SACM_ISR_ServiceLoop
.define C_SACM_MS01_FIQ_RESERVE_2		0x8000	

//**************************************
//**************************************
.define C_SACM_MS02_PLAY         		C_SACM_PLAY 
.define C_SACM_MS02_RECORD         		C_SACM_RECORD
.define C_SACM_MS02_PAUSE          	 	C_SACM_PAUSE	
.define	C_SACM_MS02_RAMP_DN_ENABLE		C_SACM_RAMP_DN_ENABLE
.define C_SACM_MS02_RAMP_UP_ENABLE		C_SACM_RAMP_UP_ENABLE	
.define C_SACM_MS02_ENABLE_DAC1	    	C_SACM_ENABLE_DAC1   
.define C_SACM_MS02_ENABLE_DAC2			C_SACM_ENABLE_DAC2
.define	C_SACM_MS02_AUTO	    		C_SACM_AUTO
//.define	C_SACM_RESERVE_2			
.define C_SACM_MS02_DECODE_WORK     	C_SACM_DECODE_WORK 
.define	C_SACM_MS02_DECODE_ODD	 		C_SACM_DECODE_ODD
.define C_SACM_MS02_DECODE_END      	C_SACM_DECODE_END  
.define	C_SACM_MS02_DECODE_RESERVE_1	0x0800	
.define C_SACM_MS02_FIQ_SOUND	    	C_SACM_FIQ_SOUND  
.define	C_SACM_MS02_FIQ_EVEN			C_SACM_FIQ_EVEN
.define	C_SACM_MS02_ISR_ServiceLoop    	C_SACM_ISR_ServiceLoop
//.define C_SACM_MS02_FIQ_RESERVE_2		0x8000	

.define MS02_DAC_8K				0					// MS02 use only
.define MS02_DAC_10K			1					// MS02 use only
.define MS02_DAC_12K			2					// MS02 use only
.define MS02_DAC_16K			3					// MS02 use only
.define MS02_DAC_20K			4					// MS02 use only
.define MS02_DAC_24K			5					// MS02 use only
.define MS02_DAC_28K			6					// MS02 use only
.define MS02_DAC_32K			7					// MS02 use only
.define MS02_DAC_36K			8					// MS02 use only
.define MS02_DAC_40K			9					// MS02 use only
//**********************************
//***********************************
.define C_SACM_DVR4800_PLAY         		C_SACM_PLAY 
//.define C_SACM_DVR4800_RECORD         	C_SACM_RECORD
.define C_SACM_DVR4800_REC_Mon_ON     		C_SACM_REC_Mon_ON
.define C_SACM_DVR4800_PAUSE          	 	C_SACM_PAUSE	
.define	C_SACM_DVR4800_RAMP_DN_ENABLE		C_SACM_RAMP_DN_ENABLE
.define C_SACM_DVR4800_RAMP_UP_ENABLE		C_SACM_RAMP_UP_ENABLE	
.define C_SACM_DVR4800_ENABLE_DAC1	    	C_SACM_ENABLE_DAC1   
.define C_SACM_DVR4800_ENABLE_DAC2			C_SACM_ENABLE_DAC2
.define	C_SACM_DVR4800_AUTO	    			C_SACM_AUTO
//.define	C_SACM_RESERVE_2			
.define C_SACM_DVR4800_DECODE_WORK    	 	C_SACM_DECODE_WORK 
.define	C_SACM_DVR4800_DECODE_ODD	 		C_SACM_DECODE_ODD
.define C_SACM_DVR4800_DECODE_END    	  	C_SACM_DECODE_END  
//.define	C_SACM_DVR4800_DECODE_RESERVE_1	0x0800	
.define C_SACM_DVR4800_CODEC_MODE			C_SACM_CODEC_MODE
.define C_SACM_DVR4800_FIQ_SOUND	    	C_SACM_FIQ_SOUND  
.define	C_SACM_DVR4800_FIQ_EVEN				C_SACM_FIQ_EVEN
.define	C_SACM_DVR4800_ISR_ServiceLoop    	C_SACM_ISR_ServiceLoop
//.define C_SACM_DVR4800_FIQ_RESERVE_2		0x8000	
.define C_SACM_DVR4800_STOP					C_SACM_STOP


//////////////////////////////////////////


//constant encoder

.define C_ADC_IN_LENGTH 128//?????	
.define C_ENCODE_IN_LENGTH 128		
.define C_ENCODE_OUT_LENGTH 126	
.ifdef A6400S_Used					//added by zgq on 20051215
	.define C_DECODE_IN_LENGTH 1024	//added by zgq on 20051215
	.define C_DECODE_OUT_LENGTH 1152//added by zgq on 20051215
	.define C_DAC_OUT_LENGTH 1152	//added by zgq on 20051215
.else								//added by zgq on 20051215
.ifdef MP3_Used						//added by benwang 20060111
	.define C_DECODE_IN_LENGTH 1024	//added by benwang 20060111
	.define C_DECODE_OUT_LENGTH 2304//added by benwang 20060111
	.define C_DAC_OUT_LENGTH 2304	//added by benwang 20060111
.else								//added by benwang 20060111
//		.define C_DECODE_IN_LENGTH 126  //added by zgq on 20051215
		.define C_DECODE_IN_LENGTH 256  //modify by chengye 2008/1/11
		.define C_DECODE_OUT_LENGTH 160	//added by zgq on 20051220
	
//		.define C_DAC_OUT_LENGTH 400	//added by zgq on 20051215
		.define C_DAC_OUT_LENGTH 640	//modify by chengye 2008/1/11		
.endif								//added by zgq on 20051215


.comment @		//del by zgq on 20051215
.ifdef	S200_Used
	.ifdef	C_SpeedControl	
	.define C_DAC_OUT_LENGTH 400 //480 //modify by benwang 20051019
	.else
	.define C_DAC_OUT_LENGTH 400 //480 //modify by benwang 20051019
	.endif
.else
.define C_DAC_OUT_LENGTH 400 //480 //modify by benwang 20051019
.endif	
.@

//.define C_SPEED_MIN_PITCH_LENGTH  13
.define C_SPEED_MIN_PITCH_LENGTH  32	//modified by zgq on 20050906
//.define C_SPEED_MAX_PITCH_LENGTH  141 //modified by benwang 01.08
.define C_SPEED_MAX_PITCH_LENGTH  128 	//modified by zgq on 20050906

.define C_SPEED_BE_WEIGHT         0x0001
.define C_SPEED_MAX_SPEED_HALF    12


.comment @ //del by benwang 2005/07/27
// LTP constants
.define  PitchMin      		18
.define  PitchMax      		(PitchMin+127)
.define  ClPitchOrd    		5
.define  NbFilt170     		170
.define  LspPrd0       		0x3000
.define  Scon0         		0x0100
// Gain constant
.define  NumOfGainLev  		24
.define  Factor24      		0xaab  		//(1/24 = 2731/65536)
.define  LspStabTest 					//added by benwang 2004/04/14


//S4872,S530 related//
.define _ORDER         		10 ; 		//del by benwang 0113
.define S530_LTPLength    	145 ;
.define _SubFrame60    		60 ; 		//del by benwang 0113
.define _SubFrames   		4 ;

//S4872 related
.define _LTPLength    148 ;
.define _SubFrame40    40 ;
.define  C_CmdMask    0x3;
@

.define	Manual_Mode_Index	-1


