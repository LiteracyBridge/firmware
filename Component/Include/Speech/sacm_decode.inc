//////////////////////////////////////////////////////////////////////////////////////////
// Progarm: SACM-DVR1600 API external definition for sacm_DVR1600_decode.asm
// Writen by: Sam Sung, 
//  Date: 2003/01/28
//////////////////////////////////////////////////////////////////////////////////////////
// Constant
// Functions
//=====================
//======================	//decode
//.external F_SACM_XXXX_Encode_Put_BS_Auto
.external F_SACM_System_Put_BS_Manual
.external F_SACM_Decode_Initial_Process
.external F_SACM_Decode_Process
.external F_SACM_Decode_Initial_BS
.external F_SACM_System_Get_BS_Manual
//.external F_SACM_XXXX_Decode_Get_BS_Auto

//===================
//====================		//encode
.external F_SACM_Encode_Initial_Process
.external F_SACM_Encode_Process
.external F_SACM_Encode_Finish_Process
.external F_SACM_Encode_Initial_BS

.external	R_SACM_Encode_Length_H
.external	R_SACM_Encode_Length_L

/////////////////////////
////////////////////////		//vabiables	
.external R_SACM_Play_Flag
.external R_SACM_Play_Gain
.external R_DAC_OUT_LENGTH
.external R_SACM_Decode_In_Buffer
.external R_SACM_Decode_In_PTR
.external R_SACM_DAC_Out_Buffer
.external R_SACM_DAC_Out_PTR_Decode
.external R_SACM_DAC_Out_PTR_Play
.external R_SACM_Decode_Length_L, R_SACM_Decode_Length_H
.external R_SACM_Decode_Count_L,  R_SACM_Decode_Count_H
.external R_SACM_ISR_Flag
.external R_ISR_DataProc_Flag			//added by chengye 2007/4/24			
.external R_Decode_Finish				//add by jacky 2006.02.15
.external R_DECODE_IN_LENGTH
.external R_Decode_Finish_A
.external R_Right_ChannelData			//added by jacky 2006.05.16
.external R_Left_ChannelData			//added by jacky 2006.05.16


.external	R_SACM_FR_Flag
.external R_SACM_DECODE_IN_LENGTH
.external R_UpSample

////////		//speed control
.external	R_SACM_SpeedIndex
.external	R_SPEED_MAX_PITCH_LENGTH		
.external	R_SPEED_PITCH_STEP

//S320
.external R_SACM_Play_Pitch 						//2005/03/31
.external R_SACM_Play_Spectrum 						//2005/03/31

.external BITRATE 
.external R_CODEC_TYPE 					//added by zgq on 2005/1/25
.external R_SACM_Play_Pitch0			//added by zgq on 2005/1/25
.external R_SACM_Play_Pitch1			//added by zgq on 2005/1/25
.external R_SACM_Play_Voice				//added by zgq on 2005/1/25
.external R_SPEED_PITCH_STEP			//added by zgq on 2005/1/25
.external R_SPEED_MAX_PITCH_LENGTH		//added by zgq on 2005/1/25

.ifdef	S600TTS_Used
.external F_SACM_TTS600_Decode_Initial_BS
.external F_SACM_TTS600_System_Get_SAC_Manual;  
.external F_SACM_TTS600_System_Get_BS_Initial;
.external F_SACM_TTS600_System_Get_BS_Manual;
.endif


//add by benwang 20051020
.ifdef  S320TTS_Used
.EXTERNAL F_SACM_TTS320_Decode_Initial_BS 
.EXTERNAL F_TTS320_System_Get_SAC_Manual 
.EXTERNAL F_SACM_TTS320_System_Get_BS_Manual 
.endif

//DVR520
.ifdef DVR520_Used
.external R_VAD_THR					//added by zgq on 20051220
.public _SACM_DVR520_VADTHR	//added by zgq on 20051220
.public F_SACM_DVR520_VADTHR 		//added by zgq on 20051220
.endif

//DVR1800
.ifdef DVR1800_Used              //add by benwang 20060621
.external R_BITRATE
.external R_BITSOFEACHFRAME
.public _SACM_DVR1800_BITRATE
.public F_SACM_DVR1800_BITRATE 	
.endif

//========================================================================================        
// End of SACM_DVR1600_Decode.inc
//========================================================================================