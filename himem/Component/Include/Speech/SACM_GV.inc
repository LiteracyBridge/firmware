//==========================================================================
//SACM_XXX_Decode_RAM.inc:  This file includes all variables for SACM_XXX_decode.asm  
//                          It is to be prepared by design deptartment.  
//     XXX: name of Algorism   
//           
//  Proposed by: Arthur
//==========================================================================

// Memory public declaration

.define	C_DAC_OUT_LENGTH_1	1152

.define	DAC_Out_Buffer_RAM_Addr	0x4B0
.ifdef MP3_Used					//modify by chengye 2008/1/11	
.define DAC_Out_Buffer_RAM1_Addr DAC_Out_Buffer_RAM_Addr+C_DAC_OUT_LENGTH_1
.define	DAC_Out_Buffer1_RAM_Addr DAC_Out_Buffer_RAM1_Addr+C_DAC_OUT_LENGTH_1
.else
.define DAC_Out_Buffer_RAM1_Addr DAC_Out_Buffer_RAM_Addr+C_DAC_OUT_LENGTH
.endif	 

.define OVERLAP_SACM_RAM_BLOCK_Addr 0x60
.define A4800_Code_Save_Addr 0x4B0+1280
.define Decode_Out_Buffer_Addr	A4800_Code_Save_Addr+128

//===========================================================
//added for MP3 SpeedControl
.ifdef	C_MP3SpeedControl
.define C_MP3_DECODE_OUT_LENGTH 1152
MP3Speed_Ram:	.section	.Oram, .addr=0x3081			// modify by lchen
.public R_SACM_MP3_Decode_Out_Buffer
.public R_SPEED_MIN_PITCH_LENGTH
R_SACM_MP3_Decode_Out_Buffer:		.dw C_MP3_DECODE_OUT_LENGTH dup(?) 
.var R_SACM_DAC_Out_PTR_Decode_temp
.var R_SPEED_MIN_PITCH_LENGTH
.endif
//===========================================================

.public	R_SACM_DAC_Out_Buffer
.ifdef MP3_Used					//special for MP3, modify by chengye 2008/1/11
DAC_Out_Buffer_RAM:	.section	.Oram, .addr=DAC_Out_Buffer_RAM_Addr		//0x4B0
R_SACM_DAC_Out_Buffer:		.dw 1 * C_DAC_OUT_LENGTH_1 dup(?) 

DAC_Out_Buffer_RAM_1:	.section	.Oram, .addr=DAC_Out_Buffer_RAM1_Addr	//0x930
R_SACM_DAC_Out_Buffer_1:	.dw 1 * C_DAC_OUT_LENGTH_1 dup(?) 

DAC_Out_Buffer1_RAM:	.section	.Oram, .addr=DAC_Out_Buffer1_RAM_Addr	//0xDB0
R_SACM_DAC_Out_Buffer1:			.dw 1 * C_DAC_OUT_LENGTH dup(?) 			//To 0x16B0	
.else
DAC_Out_Buffer_RAM:	.section	.Oram, .addr=DAC_Out_Buffer_RAM_Addr		//0x4B0~0x72F
R_SACM_DAC_Out_Buffer:		.dw 1 * C_DAC_OUT_LENGTH dup(?) 

DAC_Out_Buffer_RAM_1:	.section	.Oram, .addr=DAC_Out_Buffer_RAM1_Addr	//0x730~0x9AF
R_SACM_DAC_Out_Buffer_1:	.dw 1 * C_DAC_OUT_LENGTH dup(?) 
.endif


.ifdef	AUDIO4800_Used
// KCY ========================//		//0713
A4800_Code_Save:	.section	.Oram, .addr=A4800_Code_Save_Addr			//0x9B0
__buf_r083_ac8_codec_save:	.dw 128 dup(?)
.endif
// KCY ========================//

.ifdef	C_SpeedControl
.public R_SACM_Decode_Out_Buffer		
Decode_Out_Buffer:	.section .Oram, .addr=Decode_Out_Buffer_Addr			//0xA30
R_SACM_Decode_Out_Buffer:		.dw C_DECODE_OUT_LENGTH dup(?) //add by benwang 20051019
.endif

.public R_CODEC_TYPE 						

.public R_SACM_Decode_In_Buffer
.public R_SACM_Decode_In_PTR
.public R_SACM_DECODE_IN_LENGTH
.public R_SACM_DAC_Out_PTR_Decode
.public	R_SACM_DAC_Out_PTR_Play
.public R_DECODE_OUT_LENGTH					
.public R_DAC_OUT_LENGTH
.public	R_SACM_Decode_Length_L
.public	R_SACM_Decode_Length_H
.public	R_SACM_Decode_Count_L
.public	R_SACM_Decode_Count_H
.public	R_SACM_Encode_Count_L		//added by chengye for WAV
.public	R_SACM_Encode_Count_H

.public R_SACM_Play_Flag
.public R_SACM_Play_Gain
.public R_SACM_ISR_Flag	
.public BITRATE
.public R_SACM_FR_Flag
.public R_UpSample
.public R_Decode_Finish				//add by jacky 2006.02.15
.public R_Decode_Finish_A			//added by jacky 2006.05.16
.public R_Right_ChannelData			//added by jacky 2006.05.16
.public R_Left_ChannelData			//added by jacky 2006.05.16


.public R_SACM_Decode_Out_PTR 		//added by zgq on 20050715
.public R_DECODE_IN_LENGTH          //add by benwang 20060209

//Speed_Control
.public R_SACM_SpeedIndex			
.public R_SPEED_MAX_PITCH_LENGTH		
.public R_SPEED_PITCH_STEP	

////=============
///==============		//encode
.public	R_SACM_Encode_Length_H
.public	R_SACM_Encode_Length_L
.public _CurSACMEncodeCount			//added by chengye 2008/1/15

//s200
.public R_SACM_Play_Pitch0				
.public R_SACM_Play_Pitch1				
.public R_SACM_Play_Voice	
//S320											//2005/03/31
.public R_SACM_Play_Pitch 						//2005/03/31
.public R_SACM_Play_Spectrum 					//2005/03/31


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// VAR declaration
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
OVERLAP_SACM_RAM_BLOCK:  .SECTION        .ORAM, .addr=OVERLAP_SACM_RAM_BLOCK_Addr	//0x60
.var	R_SACM_Play_Flag
.var	R_SACM_Decode_In_PTR		
.var	R_SACM_DAC_Out_PTR_Play	
.var 	R_CODEC_TYPE 
.var 	R_SACM_Play_Gain
.var	R_SACM_Decode_Length_L, 	R_SACM_Decode_Length_H
.var 	R_SACM_Decode_Count_L,  	R_SACM_Decode_Count_H
.var 	R_SACM_DECODE_IN_LENGTH 
.var	R_DECODE_IN_LENGTH          //add by benwang 20060209
.var	R_Decode_Finish		//add by jacky 2006.02.15
.var	R_Decode_Finish_A
.var	R_Right_ChannelData			//added by jacky 2006.05.16
.var 	R_Left_ChannelData			//added by jacky 2006.05.16


.var R_SACM_Decode_Out_PTR 			//added by zgq on 20050715

.var R_SACM_DAC_Out_PTR_Decode	
.var R_DECODE_OUT_LENGTH
.var R_DAC_OUT_LENGTH			
.var BITRATE	
.var R_SACM_ISR_Flag	

.var	R_SACM_FR_Flag
.var	R_UpSample

//===================
//=================--		//DVR
.var	R_SACM_Encode_Length_H
.var	R_SACM_Encode_Length_L

//Speed_Control
.var R_SACM_SpeedIndex			
.var R_SPEED_MAX_PITCH_LENGTH		
.var R_SPEED_PITCH_STEP	

//////////////////////////////////
/////////////////////////////////	//TTS
	.comment	@
.public R_Syllable_Num			// add by zgq 20050401
.public R_DIC_PTR				// add by zgq 20050401
.public R_ADDR_PTR				// add by zgq 20050401
.public R_SAC_RdPtr				// add by zgq 20050401
.public R_Syllable_Count		// add by zgq 20050401
.public SubFrame_Count			// add by zgq 20050401
.public SubFrame_SO				// add by zgq 20050401
.public SubFrame_EO				// add by zgq 20050401
.public R_DIC_Buffer			// add by zgq 20050401
.public R_TEMP_PTR				// add by zgq 20050401
.public R_Syllable_Counter		//add by zgq 20050401

.var R_Syllable_Num				// add by zgq 20050401
.var R_DIC_PTR					// add by zgq 20050401
.var R_ADDR_PTR					// add by zgq 20050401
.var R_SAC_RdPtr				// add by zgq 20050401
.var R_Syllable_Count			// add by zgq 20050401
.var SubFrame_Count				// add by zgq 20050401
.var SubFrame_SO				// add by zgq 20050401
.var SubFrame_EO				// add by zgq 20050401
.var R_TEMP_PTR					// add by zgq 20050401
.var R_Syllable_Counter			//add by zgq 20050401
	@
///////////////////////////////////////////
//////////////////////////////////////


//////////////////////////////////////////////////////////
//R_DIC_Buffer:		.dw	48	dup (?)				// add by zgq 20050401	
//S200		
.var R_SACM_Play_Pitch0				
.var R_SACM_Play_Pitch1				
.var R_SACM_Play_Voice
//S320										//2005/03/31
.var R_SACM_Play_Pitch						//2005/03/31
.var R_SACM_Play_Spectrum					//2005/03/31
////////////////////////////////////

//////////////////////////////////	filter
.public	OFILTER_BUF
.public	OFILTER_BUF_PTR
.public DAC_16K_FLAG

OFILTER_BUF:	.dw	16 dup (0) //add dup (0) by benwang 20051021
.var	OFILTER_BUF_PTR
.var	DAC_16K_FLAG
//////////////////////////////
//////////////////////////////		//mp3	
.public MP3_FS //add by benwang 20060214
.public MP3_BR //add by benwang 20060214
.var MP3_FS //add by benwang 20060214
.var MP3_BR //add by benwang 20060214
.public F_BS //add by benwang 20060221
.var F_BS    //add by benwang 20060221
.var R_Fs_Ready //add by benwang 20060221

.public Mp3_FrameCount_L
.public Mp3_FrameCount_H
.public MP3_TimeInfo
.public R_MP3_Decode_Count_L
.public R_MP3_Decode_Count_H
.var	Mp3_FrameCount_L
.var	Mp3_FrameCount_H
.var 	MP3_TimeInfo
.var	R_MP3_Decode_Count_L
.var	R_MP3_Decode_Count_H

/////////////////////////////	//definition 
//////////////////////////
.define R_SACM_Encode_Out_PTR		R_SACM_Decode_In_PTR
.define R_SACM_Encode_Count_L		R_SACM_Decode_Count_L
.define R_SACM_Encode_Count_H		R_SACM_Decode_Count_H
.define R_SACM_ADC_In_Buffer 		R_SACM_DAC_Out_Buffer
.define R_ENCODE_OUT_LENGTH 		R_SACM_DECODE_IN_LENGTH
.define R_SACM_ADC_In_PTR_Rec 		R_SACM_DAC_Out_PTR_Play
.define _CurSACMEncodeCount			R_SACM_Encode_Count_L	//added by chengye 2008/1/15

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	//DVR1600
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
.var	decvar_Hbufx
.var	n_xfct3
.var	temp_l_xfct3
.var	temp_h_xfct3
decvar_Hbufy: .dw 2 DUP(?)
/////////////////////////////////////		//A6400
//////////////////////////////////////
.public R_A6400S_Sf_ind, R_A6400S_Decoder_InEnd_Ptr			//added by zgq on 20051215
.VAR 	R_A6400S_Sf_ind										//added by zgq on 20051215
R_A6400S_Decoder_InEnd_Ptr:	.dw	2	dup(0)					//added by zgq on 20051215
////////////////////////////////////
//////////////////////////////////////		//mp3
.public R_MP3_Sf_ind, R_MP3_Decoder_InEnd_Ptr			
.VAR 	R_MP3_Sf_ind										
R_MP3_Decoder_InEnd_Ptr:	.dw	2	dup(0)	

.public R_MP3_FrameCount_L
.public R_MP3_FrameCount_H
.var	R_MP3_FrameCount_L				//add by jacky 0309
.var	R_MP3_FrameCount_H				//add by jacky 0309
///////////////////////////////////////
//////////////////////////////////		//A128
.public R_A128_Sf_ind			
.VAR 	R_A128_Sf_ind
//////////////////////////////////////		//DVR 1800
.public R_BITRATE
.var R_BITRATE
//////////////////////////////

R_SACM_Decode_In_Buffer:		.dw C_DECODE_IN_LENGTH dup(?)


.define R_SACM_Encode_Out_Buffer	R_SACM_Decode_In_Buffer

/////////////////////////////////////////////////
.public R_SACM_DVR1600_Play_Flag
.public R_SACM_DVR1600_Play_Gain
//.public R_SACM_DVR1600_Resouce_BS
//.public R_SACM_DVR1600_Resouce_DS
.public R_SACM_DVR1600_Decode_In_Buffer
.public R_SACM_DVR1600_Decode_Length_L
.public R_SACM_DVR1600_Decode_Length_H
.public R_SACM_DVR1600_Decode_Count_L
.public R_SACM_DVR1600_Decode_Count_H
.public	R_SACM_DVR1600_DAC_Out_Buffer
.public R_SACM_DVR1600_Decode_In_PTR
.public	R_SACM_DVR1600_DAC_Out_PTR_Play
.public R_SACM_DVR1600_DAC_Out_PTR_Decode
//.public R_SACM_DVR1600_DECODE_OUT_LENGTH
.public R_SACM_DVR1600_DECODE_IN_LENGTH
.public R_SACM_DVR1600_ISR_Flag		// by Ray 2004.09.07
.public R_SACM_DVR1600_Decode_Out_PTR //2005/07/26
.public R_SACM_DVR1600_DECODE_OUT_LENGTH //add by benwang 2005/06/27

.define R_SACM_DVR1600_Play_Flag			R_SACM_Play_Flag
.define R_SACM_DVR1600_Play_Gain			R_SACM_Play_Gain
.define R_SACM_DVR1600_Decode_In_Buffer		R_SACM_Decode_In_Buffer
.define	R_SACM_DVR1600_Decode_Out_PTR		R_SACM_Decode_Out_PTR //add by benwang 2005/07/26

.define	R_SACM_DVR1600_DAC_Out_Buffer			R_SACM_DAC_Out_Buffer	
.define R_SACM_DVR1600_Decode_In_PTR			R_SACM_Decode_In_PTR
.define R_SACM_DVR1600_DAC_Out_PTR_Play			R_SACM_DAC_Out_PTR_Play	
.define R_SACM_DVR1600_DAC_Out_PTR_Decode		R_SACM_DAC_Out_PTR_Decode
.define R_SACM_DVR1600_Decode_Length_L			R_SACM_Decode_Length_L
.define R_SACM_DVR1600_Decode_Length_H			R_SACM_Decode_Length_H
.define R_SACM_DVR1600_Decode_Count_L			R_SACM_Decode_Count_L
.define R_SACM_DVR1600_Decode_Count_H			R_SACM_Decode_Count_H
.define R_SACM_DVR1600_DECODE_IN_LENGTH			R_SACM_DECODE_IN_LENGTH
.define R_SACM_DVR1600_DECODE_OUT_LENGTH		R_DECODE_OUT_LENGTH	 //add by benwang 2005/06/27
.define R_SACM_DVR1600_ISR_Flag					R_SACM_ISR_Flag
.define	R_ADC_IN_LENGTH							R_DAC_OUT_LENGTH


.define R_SACM_DVR1600_ADC_In_PTR_Rec R_SACM_DVR1600_DAC_Out_PTR_Play
.define R_SACM_DVR1600_ADC_In_Buffer 		R_SACM_DVR1600_DAC_Out_Buffer
.define R_SACM_DVR1600_Encode_Out_Buffer	R_SACM_DVR1600_Decode_In_Buffer
.define R_SACM_DVR1600_Encode_Out_PTR		R_SACM_DVR1600_Decode_In_PTR
.define R_SACM_DVR1600_Encode_Count_L		R_SACM_DVR1600_Decode_Count_L
.define R_SACM_DVR1600_Encode_Count_H		R_SACM_DVR1600_Decode_Count_H
.define R_SACM_DVR1600_Encode_Length_L		R_SACM_DVR1600_Decode_Length_L
.define R_SACM_DVR1600_Encode_Length_H		R_SACM_DVR1600_Decode_Length_H



//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		//s200
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
.public R_SACM_S200_Play_Flag						
.public R_SACM_S200_Play_Gain					
.public R_SACM_S200_Decode_In_Buffer
.public R_SACM_S200_Decode_Length_L
.public R_SACM_S200_Decode_Length_H
.public R_SACM_S200_Decode_Count_L
.public R_SACM_S200_Decode_Count_H
.public	R_SACM_S200_DAC_Out_Buffer
.public R_SACM_S200_Decode_In_PTR
.public	R_SACM_S200_DAC_Out_PTR_Play
.public R_SACM_S200_DAC_Out_PTR_Decode
.public R_SACM_S200_DECODE_IN_LENGTH
.public R_SACM_S200_DAC_OUT_LENGTH
.public R_SACM_S200_ISR_Flag//benwang modified



//.public R_SACM_Play_Pitch0 //??
//.public R_SACM_Play_Pitch1 //??
//.public R_SACM_Play_Voice //??
.public R_SACM_S200_SpeedIndex				


.define R_SACM_S200_Play_Flag				R_SACM_Play_Flag		
.define R_SACM_S200_Play_Gain				R_SACM_Play_Gain	
.define R_SACM_S200_Decode_In_Buffer		R_SACM_Decode_In_Buffer

//R_SACM_S200_Decode_Out_Buffer:
//.dw C_DECODE_OUT_LENGTH dup(?)

.define	R_SACM_S200_DAC_Out_Buffer			R_SACM_DAC_Out_Buffer
.define R_SACM_S200_Decode_In_PTR			R_SACM_Decode_In_PTR
.define R_SACM_S200_DAC_Out_PTR_Play		R_SACM_DAC_Out_PTR_Play
.define R_SACM_S200_DAC_Out_PTR_Decode		R_SACM_DAC_Out_PTR_Decode
.define R_SACM_S200_Decode_Length_L			R_SACM_Decode_Length_L
.define R_SACM_S200_Decode_Length_H			R_SACM_Decode_Length_H
.define R_SACM_S200_Decode_Count_L			R_SACM_Decode_Count_L	
.define R_SACM_S200_Decode_Count_H			R_SACM_Decode_Count_H
.define R_SACM_S200_DECODE_IN_LENGTH		R_SACM_DECODE_IN_LENGTH
.define R_SACM_S200_DAC_OUT_LENGTH			R_DAC_OUT_LENGTH//R_DECODE_OUT_LENGTH //modify by benwang 20051013	
//.define R_SACM_Play_Pitch0//??
//.define R_SACM_Play_Pitch1//??
//.define R_SACM_Play_Voice//??
.define R_SACM_S200_SpeedIndex				R_SACM_SpeedIndex
.define R_SACM_S200_ISR_Flag				R_SACM_ISR_Flag

//add by benwang 20051020
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		//s320
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
.public R_SACM_S320_Play_Flag						
.public R_SACM_S320_Play_Gain					
.public R_SACM_S320_Decode_In_Buffer
.public R_SACM_S320_Decode_Length_L
.public R_SACM_S320_Decode_Length_H
.public R_SACM_S320_Decode_Count_L
.public R_SACM_S320_Decode_Count_H
.public	R_SACM_S320_DAC_Out_Buffer
.public R_SACM_S320_Decode_In_PTR
.public	R_SACM_S320_DAC_Out_PTR_Play
.public R_SACM_S320_DAC_Out_PTR_Decode
.public R_SACM_S320_DECODE_IN_LENGTH
.public R_SACM_S320_DAC_OUT_LENGTH
.public R_SACM_S320_ISR_Flag//benwang modified



//.public R_SACM_Play_Pitch0 //??
//.public R_SACM_Play_Pitch1 //??
//.public R_SACM_Play_Voice //??
.public R_SACM_S320_SpeedIndex				


.define R_SACM_S320_Play_Flag				R_SACM_Play_Flag		
.define R_SACM_S320_Play_Gain				R_SACM_Play_Gain	
.define R_SACM_S320_Decode_In_Buffer		R_SACM_Decode_In_Buffer

//R_SACM_S320_Decode_Out_Buffer:
//.dw C_DECODE_OUT_LENGTH dup(?)

.define	R_SACM_S320_DAC_Out_Buffer			R_SACM_DAC_Out_Buffer
.define R_SACM_S320_Decode_In_PTR			R_SACM_Decode_In_PTR
.define R_SACM_S320_DAC_Out_PTR_Play		R_SACM_DAC_Out_PTR_Play
.define R_SACM_S320_DAC_Out_PTR_Decode		R_SACM_DAC_Out_PTR_Decode
.define R_SACM_S320_Decode_Length_L			R_SACM_Decode_Length_L
.define R_SACM_S320_Decode_Length_H			R_SACM_Decode_Length_H
.define R_SACM_S320_Decode_Count_L			R_SACM_Decode_Count_L	
.define R_SACM_S320_Decode_Count_H			R_SACM_Decode_Count_H
.define R_SACM_S320_DECODE_IN_LENGTH		R_SACM_DECODE_IN_LENGTH
.define R_SACM_S320_DAC_OUT_LENGTH			R_DAC_OUT_LENGTH//R_DECODE_OUT_LENGTH //modify by benwang 20051013	
//.define R_SACM_Play_Pitch0//??
//.define R_SACM_Play_Pitch1//??
//.define R_SACM_Play_Voice//??
.define R_SACM_S320_SpeedIndex				R_SACM_SpeedIndex
.define R_SACM_S320_ISR_Flag				R_SACM_ISR_Flag


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$			//s480
.public R_SACM_S4872_Decode_In_PTR
.public R_SACM_S4872_DAC_Out_PTR_Decode
.public R_SACM_S4872_DECODE_IN_LENGTH
.public R_SACM_S4872_DECODE_OUT_LENGTH
.public R_SACM_S4872_Decode_Out_PTR
.ifdef	C_SpeedControl
.public R_SACM_S4872_Decode_Out_Buffer
.define	R_SACM_S4872_Decode_Out_Buffer			R_SACM_Decode_Out_Buffer
.endif
.define R_SACM_S4872_Decode_In_PTR			R_SACM_Decode_In_PTR			
.define R_SACM_S4872_DAC_Out_PTR_Decode		R_SACM_DAC_Out_PTR_Decode
.define R_SACM_S4872_DECODE_IN_LENGTH		R_SACM_DECODE_IN_LENGTH
.define R_SACM_S4872_DECODE_OUT_LENGTH		R_DECODE_OUT_LENGTH
.define	R_SACM_S4872_Decode_Out_PTR		R_SACM_Decode_Out_PTR		

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$		//s530
.public R_SACM_S530_Play_Flag
.public R_SACM_S530_Play_Gain
.public R_SACM_S530_Decode_In_Buffer
.public R_SACM_S530_Decode_Length_L
.public R_SACM_S530_Decode_Length_H
.public R_SACM_S530_Decode_Count_L
.public R_SACM_S530_Decode_Count_H
.public	R_SACM_S530_DAC_Out_Buffer
.public R_SACM_S530_Decode_In_PTR
.public	R_SACM_S530_DAC_Out_PTR_Play
.public R_SACM_S530_DAC_Out_PTR_Decode
.public R_SACM_S530_DECODE_IN_LENGTH
.public R_SACM_S530_ISR_Flag	
.public	R_SACM_S530_Decode_Out_PTR
.public	R_SACM_S530_DECODE_OUT_LENGTH


.define R_SACM_S530_Play_Flag					R_SACM_Play_Flag
.define R_SACM_S530_Play_Gain					R_SACM_Play_Gain
.define	R_SACM_S530_Decode_In_Buffer			R_SACM_Decode_In_Buffer
.define	R_SACM_S530_Decode_Out_PTR				R_SACM_Decode_Out_PTR

.ifdef	C_SpeedControl
.public R_SACM_S530_Decode_Out_Buffer
.define	R_SACM_S530_Decode_Out_Buffer			R_SACM_Decode_Out_Buffer
.endif

.define	R_SACM_S530_DAC_Out_Buffer				R_SACM_DAC_Out_Buffer
.define R_SACM_S530_Decode_In_PTR				R_SACM_Decode_In_PTR	
.define R_SACM_S530_DAC_Out_PTR_Play			R_SACM_DAC_Out_PTR_Play
.define R_SACM_S530_DAC_Out_PTR_Decode			R_SACM_DAC_Out_PTR_Decode
.define R_SACM_S530_Decode_Length_L				R_SACM_Decode_Length_L
.define R_SACM_S530_Decode_Length_H				R_SACM_Decode_Length_H
.define R_SACM_S530_Decode_Count_L				R_SACM_Decode_Count_L
.define R_SACM_S530_Decode_Count_H				R_SACM_Decode_Count_H
.define R_SACM_S530_DECODE_IN_LENGTH			R_SACM_DECODE_IN_LENGTH
.define R_SACM_S530_ISR_Flag					R_SACM_ISR_Flag
.define R_SACM_S530_DECODE_OUT_LENGTH			R_DECODE_OUT_LENGTH  	//added by zgq on 20050715



//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$		//ms01
.public R_SACM_MS01_Play_Flag
.public R_SACM_MS01_Decode_Count_L
.public R_SACM_MS01_Decode_Count_H
.public	R_SACM_MS01_DAC_Out_Buffer
.public R_SACM_MS01_Decode_In_PTR
.public	R_SACM_MS01_DAC_Out_PTR_Play
.public R_SACM_MS01_DAC_Out_PTR_Decode
.public R_SACM_MS01_DAC_OUT_LENGTH 	


.define R_SACM_MS01_Play_Flag				R_SACM_Play_Flag


.define	R_SACM_MS01_DAC_Out_Buffer			R_SACM_DAC_Out_Buffer
.define R_SACM_MS01_Decode_In_PTR			R_SACM_Decode_In_PTR
.define R_SACM_MS01_DAC_Out_PTR_Play		R_SACM_DAC_Out_PTR_Play
.define R_SACM_MS01_DAC_Out_PTR_Decode		R_SACM_DAC_Out_PTR_Decode
.define R_SACM_MS01_Decode_Count_L			R_SACM_Decode_Count_L
.define R_SACM_MS01_Decode_Count_H			R_SACM_Decode_Count_H
.define R_SACM_MS01_DAC_OUT_LENGTH 			R_DAC_OUT_LENGTH



//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		//ms02
.public R_SACM_MS02_Play_Flag
.public R_SACM_MS02_Decode_In_Buffer
.public R_SACM_MS02_Decode_Length_L
.public R_SACM_MS02_Decode_Length_H
.public R_SACM_MS02_Decode_Count_L
.public R_SACM_MS02_Decode_Count_H
.public	R_SACM_MS02_DAC_Out_Buffer
.public R_SACM_MS02_Decode_In_PTR
.public	R_SACM_MS02_DAC_Out_PTR_Play
.public R_SACM_MS02_DAC_Out_PTR_Decode
.public	R_SACM_MS02_ISR_Flag
.public R_SACM_MS02_DAC_OUT_LENGTH  			//added by zgq on 20050715


.define R_SACM_MS02_Play_Flag				R_SACM_Play_Flag	

.define R_SACM_MS02_Decode_In_Buffer		R_SACM_Decode_In_Buffer

.define	R_SACM_MS02_DAC_Out_Buffer			R_SACM_DAC_Out_Buffer
.define R_SACM_MS02_Decode_In_PTR			R_SACM_Decode_In_PTR
.define R_SACM_MS02_DAC_Out_PTR_Play		R_SACM_DAC_Out_PTR_Play
.define R_SACM_MS02_DAC_Out_PTR_Decode		R_SACM_DAC_Out_PTR_Decode
.define R_SACM_MS02_Decode_Length_L			R_SACM_Decode_Length_L
.define R_SACM_MS02_Decode_Length_H			R_SACM_Decode_Length_H
.define R_SACM_MS02_Decode_Count_L			R_SACM_Decode_Count_L
.define R_SACM_MS02_Decode_Count_H			R_SACM_Decode_Count_H	
.define R_SACM_MS02_ISR_Flag				R_SACM_ISR_Flag
.define R_SACM_MS02_DAC_OUT_LENGTH  		R_DAC_OUT_LENGTH


//.ifdef	MS02_Used
//.public R_SACM_MS02_DAC_PTR
//.var R_SACM_MS02_DAC_PTR
//.endif

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		//DVR4800
.public R_SACM_DVR4800_Play_Flag
.public R_SACM_DVR4800_Play_Gain
.public R_SACM_DVR4800_Decode_In_Buffer
.public R_SACM_DVR4800_Decode_Length_L
.public R_SACM_DVR4800_Decode_Length_H
.public R_SACM_DVR4800_Decode_Count_L
.public R_SACM_DVR4800_Decode_Count_H
.public	R_SACM_DVR4800_DAC_Out_Buffer
.public R_SACM_DVR4800_Decode_In_PTR
.public	R_SACM_DVR4800_DAC_Out_PTR_Play
.public R_SACM_DVR4800_DAC_Out_PTR_Decode
.public R_SACM_DVR4800_DECODE_IN_LENGTH
.public R_SACM_DVR4800_ISR_Flag	


.define R_SACM_DVR4800_Play_Flag			R_SACM_Play_Flag	
.define R_SACM_DVR4800_Play_Gain			R_SACM_Play_Gain

.define R_SACM_DVR4800_Decode_In_Buffer		R_SACM_Decode_In_Buffer

.ifdef	C_SpeedControl
.public R_SACM_DVR4800_Decode_Out_Buffer
.define	R_SACM_DVR4800_Decode_Out_Buffer			R_SACM_Decode_Out_Buffer
.endif

.define	R_SACM_DVR4800_DAC_Out_Buffer		R_SACM_DAC_Out_Buffer
.define R_SACM_DVR4800_Decode_In_PTR		R_SACM_Decode_In_PTR	
.define R_SACM_DVR4800_DAC_Out_PTR_Play		R_SACM_DAC_Out_PTR_Play
.define R_SACM_DVR4800_DAC_Out_PTR_Decode	R_SACM_DAC_Out_PTR_Decode
.define R_SACM_DVR4800_Decode_Length_L		R_SACM_Decode_Length_L
.define R_SACM_DVR4800_Decode_Length_H		R_SACM_Decode_Length_H
.define R_SACM_DVR4800_Decode_Count_L		R_SACM_Decode_Count_L
.define R_SACM_DVR4800_Decode_Count_H		R_SACM_Decode_Count_H
.define R_SACM_DVR4800_DECODE_IN_LENGTH		R_SACM_DECODE_IN_LENGTH
.define R_SACM_DVR4800_ISR_Flag				R_SACM_ISR_Flag

//.external R_SACM_DVR4800_DAC_Out_PTR_Play
.define R_SACM_DVR4800_ADC_In_PTR_Rec R_SACM_DVR4800_DAC_Out_PTR_Play

//external R_SACM_DVR4800_DAC_Out_Buffer
.define R_SACM_DVR4800_ADC_In_Buffer R_SACM_DVR4800_DAC_Out_Buffer

//.external R_SACM_Decode_Out_Buffer
//.define R_SACM_Encode_In_Buffer	R_SACM_Decode_Out_Buffer

//external R_SACM_DVR4800_Decode_In_Buffer
.define R_SACM_DVR4800_Encode_Out_Buffer	R_SACM_DVR4800_Decode_In_Buffer

//.external R_SACM_DVR4800_Decode_In_PTR
.define R_SACM_DVR4800_Encode_Out_PTR	R_SACM_DVR4800_Decode_In_PTR

//.external R_SACM_DVR4800_Decode_Count_L
.define R_SACM_DVR4800_Encode_Count_L	R_SACM_DVR4800_Decode_Count_L

//external R_SACM_DVR4800_Decode_Count_H
.define R_SACM_DVR4800_Encode_Count_H	R_SACM_DVR4800_Decode_Count_H

//.external R_SACM_DVR4800_Decode_Length_L
.define R_SACM_DVR4800_Encode_Length_L	R_SACM_DVR4800_Decode_Length_L

//.external R_SACM_DVR4800_Decode_Length_H
.define R_SACM_DVR4800_Encode_Length_H	R_SACM_DVR4800_Decode_Length_H



//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	//s320

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		//S600TTS


.ifdef S600TTS_Used
.public R_S600_Delay_Count
.var R_S600_Delay_Count
.endif

.public R_SACM_S600_Decode_In_PTR
.public R_SACM_S600_DAC_Out_PTR_Decode
.public R_SACM_S600_DECODE_IN_LENGTH
.public R_SACM_S600_DECODE_OUT_LENGTH
.public R_SACM_S600_Decode_Out_PTR
.public R_SACM_S600_Decode_Count_L,  R_SACM_S600_Decode_Count_H
.public R_SACM_S600_Decode_Length_L,  R_SACM_S600_Decode_Length_H
.public R_SACM_S600_Decode_In_Buffer
.ifdef	C_SpeedControl
.public R_SACM_S600_Decode_Out_Buffer
.define	R_SACM_S600_Decode_Out_Buffer			R_SACM_Decode_Out_Buffer
.endif
.define R_SACM_S600_Decode_In_PTR			R_SACM_Decode_In_PTR			
.define R_SACM_S600_DAC_Out_PTR_Decode		R_SACM_DAC_Out_PTR_Decode
.define R_SACM_S600_DECODE_IN_LENGTH		R_SACM_DECODE_IN_LENGTH
.define R_SACM_S600_DECODE_OUT_LENGTH		R_DECODE_OUT_LENGTH
.define	R_SACM_S600_Decode_Out_PTR		R_SACM_Decode_Out_PTR		
.define R_SACM_S600_Decode_Count_L    	R_SACM_Decode_Count_L
.define R_SACM_S600_Decode_Count_H		R_SACM_Decode_Count_H
.define R_SACM_S600_Decode_Length_L    	R_SACM_Decode_Length_L
.define R_SACM_S600_Decode_Length_H		R_SACM_Decode_Length_H
.define R_SACM_S600_Decode_In_Buffer 	R_SACM_Decode_In_Buffer

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		//added by zgq on 20051215

.public R_SACM_A6400S_DAC_Out_PTR_Decode								//added by zgq on 20051215
.public R_SACM_A6400S_Decode_In_PTR										//added by zgq on 20051215
.public R_SACM_A6400S_Decode_In_Buffer									//added by zgq on 20051215
.public R_SACM_A6400S_DECODE_IN_LENGTH									//added by zgq on 20051215
.define R_SACM_A6400S_DAC_Out_PTR_Decode 	R_SACM_DAC_Out_PTR_Decode	//added by zgq on 20051215
.define R_SACM_A6400S_Decode_In_PTR			R_SACM_Decode_In_PTR		//added by zgq on 20051215
.define R_SACM_A6400S_Decode_In_Buffer		R_SACM_Decode_In_Buffer		//added by zgq on 20051215
.define R_SACM_A6400S_DECODE_IN_LENGTH		R_SACM_DECODE_IN_LENGTH		//added by zgq on 20051215

//add by benwang 20051220
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	//DVR3200
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//.var	decvar_Hbufx
//.var	n_xfct3
//.var	temp_l_xfct3
//.var	temp_h_xfct3
//decvar_Hbufy: .dw 2 DUP(?)

.public R_SACM_DVR3200_Play_Flag
.public R_SACM_DVR3200_Play_Gain
//.public R_SACM_DVR3200_Resouce_BS
//.public R_SACM_DVR3200_Resouce_DS
.public R_SACM_DVR3200_Decode_In_Buffer
.public R_SACM_DVR3200_Decode_Length_L
.public R_SACM_DVR3200_Decode_Length_H
.public R_SACM_DVR3200_Decode_Count_L
.public R_SACM_DVR3200_Decode_Count_H
.public	R_SACM_DVR3200_DAC_Out_Buffer
.public R_SACM_DVR3200_Decode_In_PTR
.public	R_SACM_DVR3200_DAC_Out_PTR_Play
.public R_SACM_DVR3200_DAC_Out_PTR_Decode
//.public R_SACM_DVR3200_DECODE_OUT_LENGTH
.public R_SACM_DVR3200_DECODE_IN_LENGTH
.public R_SACM_DVR3200_ISR_Flag		// by Ray 2004.09.07
.public R_SACM_DVR3200_Decode_Out_PTR //2005/07/26
.public R_SACM_DVR3200_DECODE_OUT_LENGTH //add by benwang 2005/06/27

.define R_SACM_DVR3200_Play_Flag			R_SACM_Play_Flag
.define R_SACM_DVR3200_Play_Gain			R_SACM_Play_Gain
.define R_SACM_DVR3200_Decode_In_Buffer		R_SACM_Decode_In_Buffer
.define	R_SACM_DVR3200_Decode_Out_PTR		R_SACM_Decode_Out_PTR //add by benwang 2005/07/26

.define	R_SACM_DVR3200_DAC_Out_Buffer			R_SACM_DAC_Out_Buffer	
.define R_SACM_DVR3200_Decode_In_PTR			R_SACM_Decode_In_PTR
.define R_SACM_DVR3200_DAC_Out_PTR_Play			R_SACM_DAC_Out_PTR_Play	
.define R_SACM_DVR3200_DAC_Out_PTR_Decode		R_SACM_DAC_Out_PTR_Decode
.define R_SACM_DVR3200_Decode_Length_L			R_SACM_Decode_Length_L
.define R_SACM_DVR3200_Decode_Length_H			R_SACM_Decode_Length_H
.define R_SACM_DVR3200_Decode_Count_L			R_SACM_Decode_Count_L
.define R_SACM_DVR3200_Decode_Count_H			R_SACM_Decode_Count_H
.define R_SACM_DVR3200_DECODE_IN_LENGTH			R_SACM_DECODE_IN_LENGTH
.define R_SACM_DVR3200_DECODE_OUT_LENGTH		R_DECODE_OUT_LENGTH	 //add by benwang 2005/06/27
.define R_SACM_DVR3200_ISR_Flag					R_SACM_ISR_Flag
//.define	R_ADC_IN_LENGTH							R_DAC_OUT_LENGTH //del by benwang 20051220


.define R_SACM_DVR3200_ADC_In_PTR_Rec R_SACM_DVR3200_DAC_Out_PTR_Play
.define R_SACM_DVR3200_ADC_In_Buffer 		R_SACM_DVR3200_DAC_Out_Buffer
.define R_SACM_DVR3200_Encode_Out_Buffer	R_SACM_DVR3200_Decode_In_Buffer
.define R_SACM_DVR3200_Encode_Out_PTR		R_SACM_DVR3200_Decode_In_PTR
.define R_SACM_DVR3200_Encode_Count_L		R_SACM_DVR3200_Decode_Count_L
.define R_SACM_DVR3200_Encode_Count_H		R_SACM_DVR3200_Decode_Count_H
.define R_SACM_DVR3200_Encode_Length_L		R_SACM_DVR3200_Decode_Length_L
.define R_SACM_DVR3200_Encode_Length_H		R_SACM_DVR3200_Decode_Length_H
//add by benwang 20051220

//added by zgq on 20051220
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//.public R_VAD_THR //del by benwang 20051221
//.var R_VAD_THR 	//del by benwang 20051221

.public R_SACM_DVR520_Decode_In_PTR			//added by zgq on 20051220
.public R_SACM_DVR520_DECODE_IN_LENGTH		//added by zgq on 20051220
.public R_SACM_DVR520_DAC_Out_PTR_Decode	//added by zgq on 20051220
.public R_SACM_DVR520_ADC_In_PTR_Encode		//added by zgq on 20051220
.public R_SACM_DVR520_Decode_Out_PTR		//added by zgq on 20051220
.public R_SACM_DVR520_DECODE_OUT_LENGTH		//added by zgq on 20051220

.define R_SACM_DVR520_Decode_In_PTR 		R_SACM_Decode_In_PTR		//added by zgq on 20051220
.define R_SACM_DVR520_DECODE_IN_LENGTH 		R_SACM_DECODE_IN_LENGTH  	//added by zgq on 20051220
.define R_SACM_DVR520_DAC_Out_PTR_Decode 	R_SACM_DAC_Out_PTR_Decode	//added by zgq on 20051220
.define R_SACM_DVR520_ADC_In_PTR_Encode	 	R_SACM_DAC_Out_PTR_Decode	//added by zgq on 20051220
.define	R_SACM_DVR520_Decode_Out_PTR		R_SACM_Decode_Out_PTR 		//add by benwang 2005/07/26
.define R_SACM_DVR520_DECODE_OUT_LENGTH		R_DECODE_OUT_LENGTH			//added by zgq on 20051220

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		//added by benwang 20060111
.public R_SACM_MP3_DAC_Out_Buffer //add by benwang 20060111
.public R_SACM_MP3_DAC_Out_PTR_Decode								
.public R_SACM_MP3_Decode_In_PTR										
.public R_SACM_MP3_Decode_In_Buffer									
.public R_SACM_MP3_DECODE_IN_LENGTH	
.define	R_SACM_MP3_DAC_Out_Buffer			R_SACM_DAC_Out_Buffer	//add by benwang 20060111								
.define R_SACM_MP3_DAC_Out_PTR_Decode 	R_SACM_DAC_Out_PTR_Decode	
.define R_SACM_MP3_Decode_In_PTR			R_SACM_Decode_In_PTR		
.define R_SACM_MP3_Decode_In_Buffer		R_SACM_Decode_In_Buffer		
.define R_SACM_MP3_DECODE_IN_LENGTH		R_SACM_DECODE_IN_LENGTH						
/////////////////////////////////////////////


////////////////////////////////////////////// //add by benwang 20060119
.public R_SACM_A1601_DECODE_IN_LENGTH		
.public R_SACM_A1601_Decode_In_PTR			
.public	R_SACM_A1601_Decode_Out_PTR			
.public R_SACM_A1601_DECODE_OUT_LENGTH  //add by benwang 20060126

.define R_SACM_A1601_DECODE_IN_LENGTH		R_SACM_DECODE_IN_LENGTH	
.define R_SACM_A1601_Decode_In_PTR			R_SACM_Decode_In_PTR
.define	R_SACM_A1601_Decode_Out_PTR			R_SACM_Decode_Out_PTR 
.define R_SACM_A1601_DECODE_OUT_LENGTH		R_DECODE_OUT_LENGTH  //add by benwang 20060126

//////////////////////////////////////////////

/////////////////////////////////////////////////// add by benwang 20060621
.public R_SACM_DVR1800_Play_Flag
.public R_SACM_DVR1800_Play_Gain
//.public R_SACM_DVR1800_Resouce_BS
//.public R_SACM_DVR1800_Resouce_DS
.public R_SACM_DVR1800_Decode_In_Buffer
.public R_SACM_DVR1800_Decode_Length_L
.public R_SACM_DVR1800_Decode_Length_H
.public R_SACM_DVR1800_Decode_Count_L
.public R_SACM_DVR1800_Decode_Count_H
.public	R_SACM_DVR1800_DAC_Out_Buffer
.public R_SACM_DVR1800_Decode_In_PTR
.public	R_SACM_DVR1800_DAC_Out_PTR_Play
.public R_SACM_DVR1800_DAC_Out_PTR_Decode
//.public R_SACM_DVR1800_DECODE_OUT_LENGTH
.public R_SACM_DVR1800_DECODE_IN_LENGTH
.public R_SACM_DVR1800_ISR_Flag		//added by wgp on 20060525
.public R_SACM_DVR1800_Decode_Out_PTR //added by wgp on 20060525
.public R_SACM_DVR1800_DECODE_OUT_LENGTH //added by wgp on 20060525
.public R_SACM_DVR1800_ADC_In_PTR_Encode	//added by wgp on 20060525

.define R_SACM_DVR1800_Play_Flag			R_SACM_Play_Flag
.define R_SACM_DVR1800_Play_Gain			R_SACM_Play_Gain
.define R_SACM_DVR1800_Decode_In_Buffer		R_SACM_Decode_In_Buffer
.define	R_SACM_DVR1800_Decode_Out_PTR		R_SACM_Decode_Out_PTR //added by wgp on 20060525

.define	R_SACM_DVR1800_DAC_Out_Buffer			R_SACM_DAC_Out_Buffer	
.define R_SACM_DVR1800_Decode_In_PTR			R_SACM_Decode_In_PTR
.define R_SACM_DVR1800_DAC_Out_PTR_Play			R_SACM_DAC_Out_PTR_Play	
.define R_SACM_DVR1800_DAC_Out_PTR_Decode		R_SACM_DAC_Out_PTR_Decode
.define R_SACM_DVR1800_Decode_Length_L			R_SACM_Decode_Length_L
.define R_SACM_DVR1800_Decode_Length_H			R_SACM_Decode_Length_H
.define R_SACM_DVR1800_Decode_Count_L			R_SACM_Decode_Count_L
.define R_SACM_DVR1800_Decode_Count_H			R_SACM_Decode_Count_H
.define R_SACM_DVR1800_DECODE_IN_LENGTH			R_SACM_DECODE_IN_LENGTH
.define R_SACM_DVR1800_DECODE_OUT_LENGTH		R_DECODE_OUT_LENGTH	 //added by wgp on 20060525
.define R_SACM_DVR1800_ISR_Flag					R_SACM_ISR_Flag
//.define	R_ADC_IN_LENGTH							R_DAC_OUT_LENGTH

.define R_SACM_DVR1800_ADC_In_PTR_Rec 		R_SACM_DVR1800_DAC_Out_PTR_Play
.define R_SACM_DVR1800_ADC_In_PTR_Encode    R_SACM_DAC_Out_PTR_Decode	//added by wgp on 20060525
.define R_SACM_DVR1800_ADC_In_Buffer 		R_SACM_DVR1800_DAC_Out_Buffer
.define R_SACM_DVR1800_Encode_Out_Buffer	R_SACM_DVR1800_Decode_In_Buffer
.define R_SACM_DVR1800_Encode_Out_PTR		R_SACM_DVR1800_Decode_In_PTR
.define R_SACM_DVR1800_Encode_Count_L		R_SACM_DVR1800_Decode_Count_L
.define R_SACM_DVR1800_Encode_Count_H		R_SACM_DVR1800_Decode_Count_H
.define R_SACM_DVR1800_Encode_Length_L		R_SACM_DVR1800_Decode_Length_L
.define R_SACM_DVR1800_Encode_Length_H		R_SACM_DVR1800_Decode_Length_H

//========================================================================
//========================================================================