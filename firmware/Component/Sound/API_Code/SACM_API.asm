//////////////////////////////////////////////////////////////////////////////////////////
// Progarm: DVR1600 decoding algorithm V1.0
// Writen by: Panda Hsu, 
// Modified by: Ben Wang, Arthur Shieh
// Last modified date: 
//              2003/07/30: Alpha release
//              2003/09/04: (Panda)32KHz ADC,16Hz DAC. DC removal
//              2003/09/09: (Panda)DC level detection in first 8 frames
//
//Note:
// 1. Naming rule for assembly
//      R_: RAM
//      L_: Label
//      F_: Function
//      C_: Constant
//      T_: Table
// 2. This file includes APIs for user to develope project. 
//    It is to be prepared by design deptartment.  
//////////////////////////////////////////////////////////////////////////////////////////

// Written by Sam Sung, 2003/01/28

// Function declarations

.include .\System\Include\System\GPL162002.inc 
.include .\Component\Include\speech\algorithm.inc
.include .\Component\Include\speech\sacmdrv.inc                			// Virtual port, virtual constants
.include .\Component\Include\speech\sacm_decode.inc     				// Function declarations
.include .\Component\Include\speech\SACM_Constant.inc   				// PlayFlag Definitions 

//.external	TimeBaseA_Enable
//.external	TimeBaseA_Disable

.ifdef	C_SpeedControl
.include .\Component\Include\speech\SACM_Speed.inc                    // added by zgq on 2005/1/25
.endif

//.define dump_encode_data
//.define DumpEncodeData
//==========
//==========	//encode
/////////////////////////////	//definition 
//////////////////////////

.define R_SACM_Encode_Out_PTR		R_SACM_Decode_In_PTR
.define R_SACM_Encode_Out_Buffer	R_SACM_Decode_In_Buffer
.define R_SACM_Encode_Count_L		R_SACM_Decode_Count_L
.define R_SACM_Encode_Count_H		R_SACM_Decode_Count_H
.define R_SACM_ADC_In_Buffer 		R_SACM_DAC_Out_Buffer
.define R_ENCODE_OUT_LENGTH 		R_SACM_DECODE_IN_LENGTH
.define R_SACM_ADC_In_PTR_Rec 		R_SACM_DAC_Out_PTR_Play
.define	R_ADC_IN_LENGTH				R_DAC_OUT_LENGTH
//==============================


// Kernel
//.external SpeedIndex
//	
//----------------------------------------
// Playback Functions(Assembly)/ APIs (C)
//------------------------------------------

// C APIs
//----------------------------------------------
.public _SACM_Initial
.public _SACM_Play
.public _SACM_Stop
.public _SACM_Volume
.public _SACM_Status
.public _CheckStatus_Snd
.public _SACM_Pause
.public _SACM_Resume
.public _SACM_ServiceLoop
.public _SACM_Pitch //2005/03/31  for S320TTS
.public _SACM_Spectrum //2005/03/31 for S320TTS
.public _SACM_Pitch0 //add by benwang 20051124
.public _SACM_Pitch1 //add by benwang 20051124
.public _SACM_Voice //add by benwang 20051124
.public _SACM_UpSample_Enable//added by chengye 2007/5/8
.public _SACM_UpSample_Disable//added by chengye 2007/5/8

// Assembly APIs
//----------------------------------------------
.public F_SACM_Initial
.public F_SACM_Play
.public F_SACM_Stop
.public F_SACM_Volume
.public F_SACM_Status
.public F_SACM_Pause
.public F_SACM_Resume
.public F_SACM_ServiceLoop
.public F_SACM_UpSample_Enable//added by chengye 2007/5/8
.public F_SACM_UpSample_Disable//added by chengye 2007/5/8

//.public AD_BUF_PTR
//.public AD_CBUF
.public F_ISR_Service_SACM_DAC 

.public T_SACM_Volume_Level

.external F_USER_EndRecord	
.external _USER_SetStartAddr

.external _SACM_Encode_Finish_Process
.external R_SACM_Decode_Out_PTR		//added by zgq on 20050715

.ifdef	C_SpeedControl				//added by zgq on 20050715
.external R_SACM_Decode_Out_Buffer  //added by zgq on 20050715
.endif								//added by zgq on 20050715

//===========================================================
//here for isr code
.external	_R_ShiftStore				
.external	_uiRecWriteFlag	
.external   _uiMP3WriteFlag

.external 	_Nand_GetFAT_MP3
.external 	_Nand_WriteFAT_Rec
//===========================================================

.external _Snd_onAmpOn				//added by chengye 2006/8/29
.external _Snd_onAmpOff				//added by chengye 2006/8/29
//.external _PlayNextFile
.external _ulFrame_Count_MP3
.external _R_DataEmptyFlag			//added 2007/01/22 for No MP3 Data use
.external _pRecWriteptr				//added by chengye for two buffer record 2007/3/27
.external R_SACM_ISR_Flag			//added by chengye 2007/4/24
.external _R_ISR_DataProc_Flag		//added by chengye 2007/4/24
.external R_DataMode				//added by chengye 2007/9/14

.ifdef S320TTS_Used	
.external _Open_TTS320_DataBase
.external _Close_TTS320_DataBase
.endif

.ifdef DVR520_Used
	.define DVR520CODEINRAM_Reserved_Addr	0x16B0 
	.external DVR520_TEXT_BEGIN_LOAD_Adr 
	.external DVR520_TEXT_END_LOAD_Adr 
.endif

.ifdef MS01_Used
	.define MS01CODEINRAM_Reserved_Addr		0x16B0
	.external MS01TEXT_INRAM_BEGIN_LABLE
	.external MS01TEXT_INRAM_END_LABLE	
.endif

//.ifdef S320_Used
//	.define S320CODEINRAM_Reserved_Addr	0x16B0
//	.external S320TextSection_BEGIN_LABLE
//	.external S320TextSection_END_LABLE	
//.endif

.ifdef MP3_Used
.external F_GetSyncWord_Init
.external F_GetSyncWord
.endif

.ifdef WAV_Used
.external _WAV_SetFileHeader
.external F_Wav_WriteHeader
.endif
.external _SACM_SetFileHeader

//*****************************************************************************
.external OFILTER_BUF
.external OFILTER_BUF_PTR	

////////////////////////////////////////////// //add by benwang 20060117
.external R_F_SACM_Stop_Var
.external  _Snd_OnPlayEnd						//added by chengye 2007/6/19

.external	R_MP3_Decode_Count_L, R_MP3_Decode_Count_H
.external	Mp3_FrameCount_L, Mp3_FrameCount_H
.external	MP3_TimeInfo
.external	MP3_FS
.external	MP3_BR

//--------------------------------------------------------
//for system message added by chengye 2007/4/26
.define MSG_SND_BEGIN               0x0040
.define MSG_SND_END                 0x0041
.define MSG_SND_STOP                0x0042
.define MSG_SND_RECBEGIN            0x0043
.define MSG_SND_RECEND              0x0044
.define MSG_SND_RECFULL             0x0045
.public MSG_SndType
MSG_Var_RAM:	.section	.ram
.var MSG_SndType
//--------------------------------------------------------

MP3_Var_RAM:	.section	.ram
	.var Temp1 //20060605
	.var Temp2 //20060605
	.var Temp3 //20060605
	.var Temp4 //20060605
	.var ErrorFrame_Num						//added by chengye for debug 2007/7/6

.ifdef dump_decode_data						//added by chengye for debug
	.ram
	.public decode_addrh,decode_addrl
	.var decode_addrh,decode_addrl
.endif

.ifdef dump_encode_data
	.ram
	.public encode_addrh,encode_addrl
	.var encode_addrh,encode_addrl
.endif

.ifdef DumpEncodeData
.external EncodeDataAddrH
.external EncodeDataAddrL
.endif

.ifdef A6400S_Used
.define INRAM_Reserved_Addr 0x4800

A6400S_CODE_INRAM_Reserved_Section:	.section	.ram	,.addr = INRAM_Reserved_Addr
CODE_INRAM_Reserved_F:	.dw		0x2650		dup(?)
.endif
//----------------------------------------------
SACM_API_ROM_BLOCK:  .SECTION        .CODE
//========================================================
// Function Name:	_SACM_Initial
// Syntax:		void SACM_Initial(void)
// Purpose: 	Kernel initialization and calls HW initial
// Note:		NO	
// Parameter : 	void
// Return:		N/A
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SACM_Initial: .proc
F_SACM_Initial:
	    push R1,R1 to [sp];
	
		fir_mov off;
		fraction off
		
		R1 = 0x0000;
		[R_SACM_Play_Flag] = R1; 
		[R_Decode_Finish]=r1				//add by jacky 2006.02.15
		  		
		R1 = 0x4000;
		//R1 = 0xF500;
		[R_SACM_Play_Gain] = R1;
		
		R1 = 5; 								// default 24 KHz
	    [BITRATE] = R1;	    

		r1 = 12;
		[R_SACM_SpeedIndex] = R1;      
		R1 = 12; //add by benwang 20060120	
		[R_SACM_Play_Pitch] = R1; //add by benwang 20051020
		[R_SACM_Play_Spectrum] = R1; //add by benwang 20051020		
		
		R1 = 0;									
	    [R_SACM_ISR_Flag] = R1;	
	    [_R_ISR_DataProc_Flag] = R1	//added by chengye 2007/4/24
	    [R_UpSample]=r1

		R1 = 0x0002;				
		[R_SACM_Play_Pitch0] = R1;	
		[R_SACM_Play_Pitch1] = R1;	
		[R_SACM_Play_Voice] = R1;	
	
		r1=0x8000
		[R_Right_ChannelData]=r1
		[R_Left_ChannelData]=r1

	    R1 = 128 ; 								// for Speed Control
	    [R_SPEED_MAX_PITCH_LENGTH] = R1;		// for Speed Control
	    R1 = 6 ;								// for Speed Control
	    [R_SPEED_PITCH_STEP] = R1;				// for Speed Control
   
   		r1 = 0
		[_ulFrame_Count_MP3] = r1
		[_ulFrame_Count_MP3 + 1] = r1
   		
	    call F_SACM_Init;  				// call HW initial
	    pop R1,R1 from [sp]; 	
		retf;
.endp

//========================================================
// Function Name:	_SACM_Play
// Syntax:		void SACM_Play(int,int,int);
// Purpose: 	Play speech from T_SACM_XXXX_SpeechTable in resource.asm or from FAT		  
// Note:		NO	
// Parameter:	Speech index: -1			: Mamual mode
//              			   0 - max index: Auto mode
//	
//				Channel: 	1: DAC1 on
//							2: DAC2 on
//							3: DAC1,2 on
// Return:		N/A
// Destory: 	R1,R2,R3
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SACM_Play: .proc 
		
	   	push BP to [SP];
	   	
	.ifdef dump_decode_data	
		r1 = 0x28
		[decode_addrh] = r1;
		r1 = 0;
		[decode_addrl] = r1;
	.endif	
	   	
	   	R1 = 0; 								//added by zgq on 2005/1/25
		[R_SACM_ISR_Flag] = R1;			//added by zgq on 2005/1/25 		
		[_R_ISR_DataProc_Flag] = R1		//added by chengye 2007/4/24
		[MSG_SndType] = R1				//added by chengye 2007/4/26
		
		BP = SP + 1;
	   	R1 = [BP + 3];
	   	R2 = [BP + 4];
	   	R3 = [BP + 5];   
	   
		push R1, R3 to [SP];
		R1 = [R_CODEC_TYPE]

//.ifdef S320TTS_Used		
//		cmp R1, C_CODEC_S320_TTS
//		jz ?S320_Copy_TextSection
//.endif
		
.ifdef MS01_Used		
		cmp R1, C_CODEC_MS01
		jz ?MS01_Copy_TextSection
.endif	

.ifdef DVR520_Used	
		cmp R1, C_CODEC_DVR520
		jz ?DVR520_Copy_Code
.endif

//.ifdef S320_Used		
//		cmp R1, C_CODEC_S320
//		jz ?S320_Copy_TextSection
//.endif
		
		goto ?Continue_Play

?MS01_Copy_TextSection:
.ifdef MS01_Used
		r1 = SEG16 MS01TEXT_INRAM_BEGIN_LABLE
	   	ds = r1;							
	   	R1 = OFFSET MS01TEXT_INRAM_BEGIN_LABLE	
	   	r2 = OFFSET MS01TEXT_INRAM_END_LABLE
	   	r2 -= R1
	   	r2 += 2							
	   	r3 = MS01CODEINRAM_Reserved_Addr		
MS01CopyCode?: 									
	   	r4 = D:[r1++] 						
	   	[r3++] = r4 						
	   	r2 -= 1 							
	   	jnz   MS01CopyCode?
	   	jmp ?Continue_Play
.endif		//End of MS01_Used

?DVR520_Copy_Code:   
.ifdef DVR520_Used		
		r1 = SEG16 DVR520_TEXT_BEGIN_LOAD_Adr
	   	ds = r1;							
	   	R1 = OFFSET DVR520_TEXT_BEGIN_LOAD_Adr	
	   	r2 = OFFSET DVR520_TEXT_END_LOAD_Adr
	   	r2 -= R1							
	   	r3 = DVR520CODEINRAM_Reserved_Addr		
DVR520CopyCode?: 									
	   	r4 = D:[r1++] 						
	   	[r3++] = r4 						
	   	r2 -= 1 							
	   	jnz   DVR520CopyCode?
		jmp ?Continue_Play
.endif   

//?S320_Copy_TextSection:
//.ifdef S320_Used
//		r1 = SEG16 S320TextSection_BEGIN_LABLE
//	   	ds = r1;							
//	   	R1 = OFFSET S320TextSection_BEGIN_LABLE	
//	   	r2 = OFFSET S320TextSection_END_LABLE
//	   	r2 -= R1							
//	   	r3 = S320CODEINRAM_Reserved_Addr		
//S320CopyCode?: 									
//	   	r4 = D:[r1++] 						
//	   	[r3++] = r4 						
//	   	r2 -= 1 							
//	   	jnz   S320CopyCode?
//	   	jmp ?Continue_Play
//.endif		//End of S320TTS_Used
    	
?Continue_Play:
	   	call F_SACM_Stop;
	   	pop R1, R3 from [SP];   
	    call F_SACM_Play
	    
	    pop BP from [SP];
	    retf

//========================================================
// Function Name:	F_SACM_Play 
// Syntax:		call F_SACM_Play; (in assembly domain)                  
// Purpose: 	Play speech from T_SACM_XXXX_SpeechTable in resource.asm or from FAT
// Note:		NO	
// Parameter:
// 				R1 = SpeechIndex
//        		R2 = DAC
//        		R3 = Ramp 
// Return:		N/A
// Destory: 	R1~R3
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================     
F_SACM_Play:  
        push R4 to [sp]; 					
	 	cmp	r1,Manual_Mode_Index
	 	je	skip?
	 	call	_USER_SetStartAddr			//r1 is index
skip?:	        
		R4 = C_SACM_PLAY;				 
	   	test R2, 0x0001; 					// DAC1 = 0x0001
	   	jz ?L_Branch_0;	
	   	R4 |= C_SACM_ENABLE_DAC1;
.ifdef 	S200_Used
		r5=[R_CODEC_TYPE]
		cmp	r5,	C_CODEC_S200
		jne	skip1?
		call	F_SACM_CHA_ControlLogic_Enable
		jmp	L_conti?	 
.endif
skip1?:
		call	F_SACM_CHA_ControlLogic_Enable_FIFO //del by benwang 20060113
L_conti?:
		call	F_SACM_CHA_AudioOutput_Enable //del by benwang 20060113

?L_Branch_0:	
	   	test R2, 0x0002; 							// DAC2 = 0x0002
	   	jz ?L_Branch_1;	
	   	R4 |= C_SACM_ENABLE_DAC2;
.ifdef 	S200_Used
		r5=[R_CODEC_TYPE]
		cmp	r5,	C_CODEC_S200
		jne	skip2?
		call	F_SACM_CHB_ControlLogic_Enable
		jmp	L_conti1?	 
.endif
skip2?:	
		call	F_SACM_CHB_ControlLogic_Enable_FIFO //del by benwang 20060113
L_conti1?:		
		call	F_SACM_CHB_AudioOutput_Enable  //del by benwang 20060113

//=============================================================================
// Do Ramp-Up if necessary
//=============================================================================
?L_Branch_1:
   	test R3, 0x0001; // Ramp_Up = 0x0001
   	jz ?L_Branch_2;	
   	R4 |= C_SACM_RAMP_UP_ENABLE;
   	test R4, C_SACM_ENABLE_DAC1;
   	jz ?L_Branch_3;

  	call F_SACM_RampUpDAC1;

?L_Branch_3:
   	test R4, C_SACM_ENABLE_DAC2;
   	jz ?L_Branch_2;

  	call F_SACM_RampUpDAC2;

?L_Branch_2:
   	test R3, 0x0002; // Ramp_Dn = 0x0002
   	jz ?L_Branch_4;

   	R4 |= C_SACM_RAMP_DN_ENABLE;

?L_Branch_4:
//==================================	    
//add by benwang start 20060209		
		r2=sr&0x03f
		ds=r2
		r2 = [R_CODEC_TYPE];
		r2 += offset T_SACM_Decode_In_Len;
		pc = ds:[r2];
		nop;
L_Set_Decode_In_Length:
		R1 = 256//160; //20060629
		[R_DECODE_IN_LENGTH] = R1;
		jmp L_Set_Decode_In_Length_End;
L_Set_MP3_Decode_In_Length:
		R1 = 1024;
		[R_DECODE_IN_LENGTH] = R1;
		jmp L_Set_Decode_In_Length_End; //add by benwang 20060221
L_Set_A128_Decode_In_Length:             //add by benwang 20060221
		R1 = 1024;				     	//add by benwang 20060221							
		[R_DECODE_IN_LENGTH] = R1;		//add by benwang 20060221	
L_Set_Decode_In_Length_End:		
//add by benwang end 20060209
//============================================

	    // Get bit stream  	
	    // MANUAL_MODE 
L_Manual_Mode:
		[R_SACM_Play_Flag] = R4;
		//R1 = R_SACM_Decode_In_Buffer + C_DECODE_IN_LENGTH; //del by benwang 20060209
		R1 = [R_DECODE_IN_LENGTH]; 							 //add by benwang 20060209
		R1 += R_SACM_Decode_In_Buffer; 						 //add by benwang 20060209
		[R_SACM_Decode_In_PTR] = R1;

//------------------------------------
		r1=sr&0x03f
		ds=r1
		R1 = [R_CODEC_TYPE];
		r1+=T_TTS_GetDS
		pc=ds:[r1]
		
.ifdef	S600TTS_Used
L_SACM_S600TTS_DS:
		call F_SACM_TTS600_System_Get_SAC_Manual;  
		call F_SACM_TTS600_System_Get_BS_Initial;
		call F_SACM_TTS600_System_Get_BS_Manual;
		JMP _Get_BS_END;
.endif

.ifdef	S200TTS_Used
L_SACM_S200TTS_DS:
		call F_S200TTS_System_Get_SAC_Manual;  
		call F_S200TTS_System_Get_BS_Initial;
		call F_S200TTS_System_Get_BS_Manual;
		JMP _Get_BS_END;
.endif

.ifdef	S320TTS_Used
L_SACM_S320TTS_DS:
		push R1,R5 to [sp]
		call _Open_TTS320_DataBase			 //added by chengye 2007/5/15
		pop R1,R5 from [sp]
		call F_TTS320_System_Get_SAC_Manual; //add by benwang 20051020
		JMP _Get_BS_END;
.endif		
		
L_SACM_NotTTS_DS: 
		call F_SACM_System_Get_BS_Manual;
_Get_BS_END:		
//----------------------------------------
		
// Start Decode 
L_DecodeInit:
//////////////////////	//add by jacky	2005.05.09
		r2=sr&0x03f
		ds=r2
		r2 = [R_CODEC_TYPE];
		r1=[R_UpSample]
		jnz	upsample?
		r2 += offset T_SACM_IRQ
		jmp	conti?
upsample?:
		r2+=offset T_SACM_IRQ_UpSample
conti?:		
		pc = ds:[r2];
		nop	
L_SACM_16K:
		call	F_SetIRQ_16000
		jmp		L_SACM_setIRQ_end
L_SACM_8K:
		call	F_SetIRQ_8000
		jmp		L_SACM_setIRQ_end
L_SACM_9_2K:
		call	F_SetIRQ_9200
		jmp		L_SACM_setIRQ_end
L_SACM_11K:
		call	F_SetIRQ_11025	
		jmp		L_SACM_setIRQ_end //add by benwang 20060111
L_SACM_32K:
		call	F_SetIRQ_32000
		jmp		L_SACM_setIRQ_end
L_SACM_22K:
		call	F_SetIRQ_22050
		jmp		L_SACM_setIRQ_end

L_SACM_setIRQ_end:		

/////////////////////
		r1=sr&0x03f	
		ds=r1
		R1 = [R_CODEC_TYPE];
		r1 += T_SACM_Speech_Table_Init_BS;
		pc = ds:[r1];							   
		nop
.ifdef S200_Used		
L_S200_Init_BS:
		//call F_SACM_LRC_Decode_Initial_BS; //del by benwang 20051013	
		call F_SACM_Decode_Initial_BS; //add by benwang 20051013
		jmp L_SACM_Continue_0;
.endif

L_SACM_Init_BS:
	   	call F_SACM_Decode_Initial_BS;
		jmp L_SACM_Continue_0;

.ifdef	S600TTS_Used
L_S600TTS_Init_BS:
		call F_SACM_TTS600_Decode_Initial_BS	
		jmp	L_SACM_Continue_0
.endif

.ifdef	S200TTS_Used
L_S200TTS_Init_BS:
		call F_SACM_S200TTS_Decode_Init_BS	
		jmp	L_SACM_Continue_0
.endif

.ifdef	S320TTS_Used
L_S320TTS_Init_BS:
		call F_SACM_TTS320_Decode_Initial_BS ; //add by benwang 20051020
		jmp	L_SACM_Continue_0				//add by benwang 20060111	
.endif		


	
L_SACM_Continue_0:
		r1=sr&0x03f
		ds=r1
		R1 = [R_CODEC_TYPE];	
		r1+=T_TTS_GetDS1
		pc=ds:[r1]

.ifdef	S600TTS_Used
L_SACM_S600TTS_DS1:
		call F_SACM_TTS600_System_Get_BS_Manual;		
		JMP L_DecodeInProcess;
.endif

.ifdef	S200TTS_Used
L_SACM_S200TTS_DS1:
		call F_S200TTS_System_Get_BS_Manual;
		JMP L_DecodeInProcess;
.endif

.ifdef	S320TTS_Used
L_SACM_S320TTS_DS1:
		call F_SACM_TTS320_System_Get_BS_Manual; //add by benwang 20051020
		JMP L_DecodeInProcess;
.endif
		
L_SACM_NotTTS_DS1: 
		call F_SACM_System_Get_BS_Manual;
      
    
L_DecodeInProcess:
		call F_SACM_Decode_Initial_Process;

.ifdef C_SpeedControl
		r1 = R_SACM_Decode_Out_Buffer;
		[R_SACM_Decode_Out_PTR] = r1;
		
		r1 = sr&0x03f
		ds=r1
		R1 = [R_CODEC_TYPE];
		r1 += T_TTS_GetDS3
		pc = ds:[r1]
L_SACM_UseSpeedControlDM_DS3:
		//////////////////
		push r3,r4 to [sp]
		r3=0x2e
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//		call F_SACM_XXXX_Speed_Initial_Psuedo;
		///////////////////// 
.endif		

///////////////////////////////////////
	
L_Out_Buffer_Init:   	
		R1 = 0x0000;		
		R2 = R_SACM_DAC_Out_Buffer;
		R3 = R2 + [R_DAC_OUT_LENGTH];	
		R3 += [R_DAC_OUT_LENGTH];//add by benwang 20060113	
?_Loop_0:	
		[R2++] = R1;
		cmp R2, R3;
		jne ?_Loop_0;  

//------initialize 16K DAC--------//		
//		R1 = 0;
//		[DAC_16K_FLAG] = R1;
		[OFILTER_BUF_PTR] = R1;
		[OFILTER_BUF + 0] = R1;
		[OFILTER_BUF + 1] = R1;
		[OFILTER_BUF + 2] = R1;
		[OFILTER_BUF + 3] = R1;
		[OFILTER_BUF + 4] = R1;
		[OFILTER_BUF + 5] = R1;
		[OFILTER_BUF + 6] = R1;
		[OFILTER_BUF + 7] = R1;
		[OFILTER_BUF + 8] = R1;
		[OFILTER_BUF + 9] = R1;
		[OFILTER_BUF + 10] = R1;
		[OFILTER_BUF + 11] = R1;
		[OFILTER_BUF + 12] = R1;
		[OFILTER_BUF + 13] = R1;
		[OFILTER_BUF + 14] = R1;
		[OFILTER_BUF + 15] = R1;
//-------------------------------//
		
		R1 = R_SACM_DAC_Out_Buffer;
		[R_SACM_DAC_Out_PTR_Play] = R1;
		
		r1 = [R_DAC_OUT_LENGTH];
		R1 += R_SACM_DAC_Out_Buffer;//zgq
		[R_SACM_DAC_Out_PTR_Decode] = R1;

		R1 = [R_SACM_Play_Flag];
		R1 |= (C_SACM_DECODE_WORK + C_SACM_DECODE_ODD + C_SACM_FIQ_SOUND + C_SACM_FIQ_EVEN);
		[R_SACM_Play_Flag] = R1;

//
		r1 = 1;								//add by zgq on 2005/2/19
		[R_SACM_ISR_Flag] = r1;			//add by zgq on 2005/2/19
		[_R_ISR_DataProc_Flag] = R1		//added by chengye 2007/4/24
		r1=0
		[R_Decode_Finish]=r1				//add by jacky 2006.02.15
		r1=1
		[R_Decode_Finish_A]=r1
		fraction off
		r1=fr
		[R_SACM_FR_Flag]=r1

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		r1 = [R_SACM_Play_Flag]					//add by Anson  2004/1/29 	//%
        test r1,C_SACM_ENABLE_DAC1					//enable cha,b interrupt  	//%
        jz L_EndDACA?											  		  					//%
		call F_SACM_CHA_PlayInterrupt_Enable						  					//%
L_EndDACA?:															  	  					//%
        r1 = [R_SACM_Play_Flag]									  					//%
		test r1,C_SACM_ENABLE_DAC2								  						//%
        jz L_EndDACB?											  	      					//%
		call F_SACM_CHB_PlayInterrupt_Enable						  					//%
		
L_EndDACB?:	
	    pop R4 from [sp];
   	retf;

.endp

//========================================================
// Function Name:	_SACM_Stop
// Syntax:		void SACM_A2000_Stop() 
// Purpose: 	Stop Playback 
// Note:		NO	  
// Parameter:	N/A
// Return:		N/A
// Destory:		R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SACM_Stop: .proc
F_SACM_Stop: 
		r1 = 0;								//added by zgq 2005/03/04
		[R_SACM_ISR_Flag] = r1; 			//added by zgq 2005/03/04
		[_R_ISR_DataProc_Flag] = R1			//added by chengye 2007/4/24
		[R_Decode_Finish]=r1				//add by jacky 2006.02.15

		R1 = [R_SACM_Play_Flag];
		test R1, C_SACM_CODEC_MODE;	
		JNE _SACM_ENCODE_STOP 
		goto _SACM_DECODE_STOP;
_SACM_ENCODE_STOP:	
		R1 = [R_SACM_Play_Flag];
	   	test R1, C_SACM_PLAY;
	   	jnz  ?Continue_;
	   	goto ?_Branch_End
?Continue_:
		R1 |= C_SACM_STOP;   
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//		call	F_SACM_ADC_RecordInterrupt_Disable
		call	F_SACM_ADC_ControlLogic_Disable
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		test	r1,C_SACM_REC_Mon_ON
		jz	?_Branch_0
		
		call	F_SACM_CHA_ControlLogic_Disable
		
		test R1, C_SACM_RAMP_DN_ENABLE;
   		jz ?_Branch_0;

   		test R1, C_SACM_ENABLE_DAC1;
   		jz ?_Branch_1;

		call F_SACM_RampDnDAC1;

?_Branch_1:
		test R1, C_SACM_ENABLE_DAC2;
		jz ?_Branch_0;

		call F_SACM_RampDnDAC2;
			
?_Branch_0:
		[R_SACM_Play_Flag] = R1;
		
		R1 = [R_DataMode]			//added by chengye according to xiaolei
		cmp R1, 0					//$
		jz ?EndRecord_Memory			//added by chengye according to xiaolei
		R1 = [R_SACM_Decode_Count_H];
		[sp--] = r1
		R1 = [R_SACM_Decode_Count_L];
		[sp--] = r1
		call _SACM_Encode_Finish_Process
		sp = sp+2;
///////////////////Record FAT end writer header/////////////////////	
		R1 = [R_CODEC_TYPE];
		cmp r1, C_CODEC_WAV
		jne Not_WriteWAV?	
.ifdef WAV_Used 	
		push r1 ,r5 to [sp]
		r1 = OFFSET R_SACM_Decode_In_Buffer 
		r2 = SEG R_SACM_Decode_In_Buffer	
		call F_Wav_WriteHeader;
		pop r1,r5 from [sp]
		r1 = [_R_LengthWAV_Header]
		[sp--] = r1
		r1 = R_SACM_Decode_In_Buffer
		[sp--] = r1
		call _WAV_SetFileHeader
		sp = sp+2;
		jmp ?_Branch_End;
.endif		
Not_WriteWAV?:
		call _SACM_SetFileHeader
		jmp ?_Branch_End;
		
EndRecord_Memory?:
.ifdef WAV_Used 	
		push r1 ,r5 to [sp]
		r1 = [EXT_SRAM_Start_BS] 
		r2 = [EXT_SRAM_Start_DS]	
		call F_Wav_WriteHeader;
		pop r1,r5 from [sp]
		jmp ?_Branch_End;
.endif
//此处将录音的word数转换为byte数		
		R1 = [R_SACM_Decode_Count_L];
		R2 = [R_SACM_Decode_Count_H];
		R1 = R1 LSL 4;
		R2 = R2 ROL 1;
		R1 = [R_SACM_Decode_Count_L];
		R1 = R1 LSL 1;
////////////////////////////////////////////////////
		call F_USER_EndRecord							//return file length R1:Low,R2:High	
?_Branch_End:	
		R1 = 0x0000;				
	   	[R_SACM_Play_Flag] = R1;
	   	retf;	
	
//****************************	
//*******************************
_SACM_DECODE_STOP:
	   	R1 = [R_SACM_Play_Flag];
	   	test R1, C_SACM_PLAY;
	   	jz ?_Branch_d;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%% Added by Anson 2004/2/2										%
//%%%       Clear Disable Channel FIFO Empty Interupt				%
//%%%       Latch FIFO data to DAC/PWM after writing to control reg %
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      	int	off
        r1 = [R_SACM_Play_Flag]			                        //%
        test r1,C_SACM_ENABLE_DAC1								 	//%
        jz L_EndDACA?											  		//%
		call	F_SACM_CHA_PlayInterrupt_Disable
L_EndDACA?:															  	//%
        r1 = [R_SACM_Play_Flag]								    //%
		test r1,C_SACM_ENABLE_DAC2							  	 	//%
        jz L_EndDACB?											  	    //%
		call	F_SACM_CHB_PlayInterrupt_Disable
L_EndDACB?:			
		int irq, fiq															//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   	
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%% Added by Anson 2004/2/2										%
//%%%       Wait FIFO empty at this time    			      		%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		r1 = [R_SACM_Play_Flag]
        test r1,C_SACM_ENABLE_DAC1
        jz L_EndCheckDACA?
		call	F_SACM_CHA_WaitFIFOEmpty
L_EndCheckDACA?:
        r1 = [R_SACM_Play_Flag]
		test r1,C_SACM_ENABLE_DAC2
        jz L_EndCheckDACB?
		call	F_SACM_CHB_WaitFIFOEmpty
L_EndCheckDACB?:
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


L_TurnOffAudioOutput?:
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        r1 = [R_SACM_Play_Flag]			                      //%
        test r1,C_SACM_ENABLE_DAC1								  //%
        jz L_EndDAC_A?											  	  //%
			call	F_SACM_CHA_AudioOutput_Disable
			call	F_SACM_CHA_ControlLogic_Disable				      //%
L_EndDAC_A?:															  //%
        r1 = [R_SACM_Play_Flag]								  //%
		test r1,C_SACM_ENABLE_DAC2							  	  //%
        jz L_EndDAC_B?											  	  //%
			call	F_SACM_CHB_AudioOutput_Disable
			call	F_SACM_CHB_ControlLogic_Disable					  //%
L_EndDAC_B?:															  //%
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

?_Branch_d:  
.ifdef S320TTS_Used			
		R1 = [R_CODEC_TYPE]
		cmp R1, C_CODEC_S320_TTS 
		jnz ?Continue_End	
		push R1,R5 to [sp]
		call _Close_TTS320_DataBase
		pop R1,R5 from [sp]  		
?Continue_End:	 
.endif  	
	   	R1 = 0x0000;				
	   	[R_SACM_Play_Flag] = R1;	
	   	r1=1
	   	[R_Decode_Finish]=r1		
	   	
//	   	call	TimeBaseA_Disable	//20060526
	   		
	   	retf;
.endp


////////////////////////////////////////////////////////
// Function Name:	_SACM_Volume
// Syntax:		void SACM_Volume(int);
// Purpose: 	Set volume
// Note:		NO	
// Parameter:	volume index: {0..15(max)}
//
// Return:		N/A
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
////////////////////////////////////////////////////////
_SACM_Volume: .proc
	   	R1 = SP + 3;
	   	R1 = [R1];	
F_SACM_Volume:	   
		r2=offset T_SACM_Volume_Level
		r3=seg16 T_SACM_Volume_Level 
		R1 += r2
		r3+=0,carry
		ds=r3
///////////////////////////////// add by benwang 20060114
		r1=ds:[r1]  //del by benwang 20050116			   	
	   	//R1 = [R1];    //del by benwang 20050116
/////////////////////////////////
		[R_SACM_Play_Gain] = R1;	
	   	retf;
.endp


//========================================================
// Function Name:	_SACM_UpSample_Enable
// Syntax:		void SACM_UpSample_Enable(void);
// Purpose:		Set sacm up-sample  
// Note:		NO
// Return:		N/A
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SACM_UpSample_Enable: .proc	
F_SACM_UpSample_Enable:	   
		r1=0x01
		[R_UpSample] = R1;	
	   	retf;
.endp

//========================================================
// Function Name:	_SACM_UpSample_Disable
// Syntax:  	void SACM_UpSample_Disable(void);
// Purpose: 	Disable sacm up-sample
// Note:		NO
// Return:N/A
// Destory:		R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SACM_UpSample_Disable: .proc
F_SACM_UpSample_Disable:	   
		r1=0x00
		[R_UpSample] = R1;	
	   	retf;
.endp

//========================================================
// Function Name:	_SACM_Status
// Syntax:		void SACM_A2000_Status(void);
// Purpose: 	Get playback status
// Note:		NO
// Parameter:	N/A
// Return: 		R1 = [R_SACM_XXXX_Play_Flag]  , pls refer to 
//           	SACM_DVR1600_Constant for detailed definition
//         		[b0] = C_SACM_PLAY  	0x0001 
//         		[b1] = C_SACM_RECORD	0x0002
//         		[b2] = C_SACM_PAUSE  0x0004	
// Destory:		R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SACM_Status: .proc
F_SACM_Status:
	    R1 = [R_SACM_Play_Flag]	   
	   	retf;
.endp


_CheckStatus_Snd: .proc					//added by chengye 2007/4/26
F_CheckStatus_Snd:
	    R1 = [MSG_SndType]
	    R2 = 0
	    [MSG_SndType] = R2	   
	   	retf;
.endp

////////////////////////////////////////////////////////
// Function: _SACM_Codec
// Description: Get codec type
// Syntax  :   
// void SACM_Codec(int type);
//	
// Parameter:
//  	N/A
// Return: 
// Destory: R1
////////////////////////////////////////////////////////
.public	_SACM_Codec
_SACM_Codec:
	r1=sp+3
	r1=[r1]
	[R_CODEC_TYPE]=r1
	retf

//========================================================
// Function Name:	_SACM_Get_Codec
// Syntax:		void SACM_Get_Codec(int type);
// Purpose:		Get codec type
// Note:		NO
// Parameter:	N/A
// Return: 		R1, the current Codec
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public	_SACM_Get_Codec
_SACM_Get_Codec:
	r1=[R_CODEC_TYPE]
	retf

//========================================================
// Function Name:	_SACM_Speed
// Syntax:		void SACM_Speed(int type);
// Purpose: 	Set speed value
// Note:		NO
// Parameter:	N/A
// Return:		N/A 
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public	_SACM_Speed
_SACM_Speed:
	r1=sp+3
	r1=[r1]
	[R_SACM_SpeedIndex]=r1
	retf

//========================================================
// Function Name:	_SACM_Get_Speed
// Syntax:		void SACM_Get_Speed(int type);
// Purpose: 	Set speed value   
// Note:		NO	
// Parameter:	N/A
// Return: 		N/A 
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public	_SACM_Get_Speed
_SACM_Get_Speed:
	R1 = [R_SACM_SpeedIndex]
	retf

//========================================================
// Function Name:	_SACM_ServiceLoop
// Syntax:		void SACM_ServiceLoop(void);
// Purpose: 	decode bit stream
// Note:		NO	
// Parameter: 	N/A
// Return: 		N/A
// Destory: 	R1,R2,R3;
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SACM_ServiceLoop: .proc
F_SACM_ServiceLoop:
.ifndef Background_mode		//2007/01/11 add
		push R1,R5 to [sp];	//should delete this sentence when background play
.endif	//End of Background_mode definition		//2007/01/11 add
		R1 = [R_SACM_Play_Flag];

		test R1, C_SACM_CODEC_MODE;	
		JNE _SACM_ENCODE_ServiceLoop;
		goto _SACM_DECODE_ServiceLoop;
_SACM_ENCODE_ServiceLoop:
	    test R1, C_SACM_PLAY
	    jnz ?_Continue_0;
.ifndef Background_mode		//2007/01/11 add
	    pop R1,R5 from [sp];			//should delete this sentence when background play
.endif	//End of background definition 2007/01/11
		retf;

?_Continue_0:
		test	r1,C_SACM_DECODE_WORK
		jnz	?_Continue_1
.ifndef Background_mode		//2007/01/11 add
		pop	r1,r5 from [sp]				//should delete this sentence when background play
.endif	//End of Background_mode		//2007/01/11 add
		retf

?_Continue_1:
		R1 &= ~C_SACM_DECODE_WORK;
		[R_SACM_Play_Flag] = R1;

		call F_SACM_Encode_Process;
	
	    R1 = [R_SACM_Play_Flag];  
	    R1 ^= C_SACM_DECODE_ODD;
		[R_SACM_Play_Flag] = R1;

//*********Check File Size ***********//
		R4 = [R_SACM_Encode_Count_H];
		R3 = [R_SACM_Encode_Count_L];	
		CMP R4,[R_SACM_Encode_Length_H];
		JA ?_Length_not_Enough;
		CMP R3,[R_SACM_Encode_Length_L];
		JA ?_Length_not_Enough;
		JMP ?_Length_Enough; 									// Length enough and Continue!
?_Length_not_Enough:	
		R3 -= C_ENCODE_OUT_LENGTH;
		R4 -= 0,carry;

		[R_SACM_Encode_Count_L] = R3;
		[R_SACM_Encode_Count_H] = R4;
	
		R1 = [R_SACM_Encode_Out_PTR];
		R1 -= C_ENCODE_OUT_LENGTH;
		[R_SACM_Encode_Out_PTR] = R1;

		R1 = [R_SACM_Play_Flag];
		R1 &= ~C_SACM_FIQ_SOUND;								// Disable FIQ_SOUND Flag
		R1 |= C_SACM_DECODE_END; 							// Set CODEC_END Flag	
		JMP ?_Finish_Process;	
//****** End Of Check File Size *******//
?_Length_Enough:
		//.if C_SYSTEM_MODE == C_SYSTEM_MANUAL_MODE
		call F_SACM_System_Put_BS_Manual;
		//.endif
		//.if C_SYSTEM_MODE == C_SYSTEM_AUTO_MODE///????
		//call F_SACM_XXXX_Encode_Put_BS_Auto;
		//.endif

		R1 = [R_SACM_Play_Flag];
		test R1, C_SACM_STOP;
		jz ?_Branch_2;
//**************************************************************//
		call F_SACM_Encode_Finish_Process;
		//.if C_SYSTEM_MODE == C_SYSTEM_MANUAL_MODE
		call F_SACM_System_Put_BS_Manual;
		//.endif
		//.if C_SYSTEM_MODE == C_SYSTEM_AUTO_MODE
		//call F_SACM_XXXX_Encode_Put_BS_Auto;
		//.endif								
		R1 = 0x0000;
?_Finish_Process:
		[R_SACM_Play_Flag] = R1;
//********start move to F_SACM_XXXX_Stop by zgq on 2005/03/03
//		R1 = [R_SACM_XXXX_Decode_Count_L]; 		
//		R2 = [R_SACM_XXXX_Decode_Count_H];
//		R1 = R1 LSL 4;
//		R2 = R2 ROL 1;
//		R1 = [R_SACM_XXXX_Decode_Count_L];
//		R1 = R1 LSL 1;
		  	
//		call F_USER_XXXX_EndRecord							//return file length R1:Low,R2:High

//********end move to F_SACM_XXXX_Stop by zgq on 2005/03/03	
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		call	F_SACM_ADC_RecordInterrupt_Disable
		call	F_SACM_ADC_ControlLogic_Disable
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		R1=[R_SACM_Decode_Count_L];
		R2=[R_SACM_Decode_Count_H];
		R1=R1 LSL 4;
		R2=R2 ROL 1;
		R1=[R_SACM_Decode_Count_L];
		R1=R1 LSL 1;
		call F_USER_EndRecord//F_USER_DVR4800_EndRecord; //return file length R1:Low,R2:High
//**************************************************************//
?_Branch_2:
.ifndef Background_mode		//2007/01/11 add
		pop R1, R5 from [SP]; 				//should delete this sentence when background play
.endif //End of background_mode definition
		retf;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
_SACM_DECODE_ServiceLoop:
	    test R1, C_SACM_PLAY   							// Check if playing
	    jnz ?L_Check_Pause;
.ifndef Background_mode		//2007/01/11 add
	    pop R1, R5 from [SP];				//should delete this sentence when background play
.endif						//End of back definition
		retf;
	
?L_Check_Pause:
	    test R1, C_SACM_PAUSE;   							// Check if pause
	    jz ?_Continue_0;
.ifndef Background_mode		//2007/01/11 add
	    pop R1, R5 from [SP];				//should delete this sentence when background play
.endif
		retf;

?_Continue_0:
		test R1, C_SACM_DECODE_WORK;   					// Check if decoding
		jnz ?_Continue_1;
.ifndef Background_mode		//2007/01/11 add
	    pop R1, R5 from [SP];				//should delete this sentence when background play
.endif	//End of background definition 2007/01/11
		retf;

?_Continue_1: 											// Decoding
//===================ADD 2007/01/22 For MP3 Get Datas wrong=============
//When MP3 data is empty, The user must tell the kernal there is no data
//used, So, if the user set the glable variable equals -1, it means the 
//data is empty:
		push R1 to [sp]
		R1 = [R_CODEC_TYPE]						//added by chengye 2007/4/18
		
	.ifdef ETTS_Used	
		cmp R1, C_CODEC_ETTS	
		jz ?ETTS_Skip_Get_BS_Manual				//ETTS don't need get BS manual
	.endif
		call F_SACM_System_Get_BS_Manual		//add 2007/01/22 for MP3 databuffer is empty
?ETTS_Skip_Get_BS_Manual:		
		pop R1 from [sp]

		R2 = [_R_DataEmptyFlag]
		cmp R2, -1
		jne ?L_DecodeNormal
		r2 = 0
		[R_Decode_Finish] = r2
.ifndef Background_mode		//2007/01/11 add
	    pop R1, R5 from [SP];				//should delete this sentence when background play
.endif
		retf;
?L_DecodeNormal:
//===================ADD End 2007/01/23==================================
		R1 &= ~C_SACM_DECODE_WORK;
		[R_SACM_Play_Flag] = R1;

		r1=0
		[R_Decode_Finish]=r1				//add by jacky 2006.02.15
/////////////////////////////////////
		r1=sr&0x03f
		ds=r1
		R1 = [R_CODEC_TYPE];
		r1 += T_SACM_Decode_Proc;
		pc = ds:[r1];
		
.ifdef	S200_Used		
L_S200_Decode_Proc:
		call F_SACM_Decode_Process;
		jmp _Continue_2;
.endif

.ifdef	S4872_Used
L_S4872_Decode_Proc:
	.ifdef C_SpeedControl					//added by zgq on 20050715
	/////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x30
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
	//	call F_SACM_XXXX_Speed_Process_Psuedo;	//add by benwang 20060120
	//////////////////////////
	.else									//added by zgq on 20050715
		call F_SACM_Decode_Process;			//added by zgq on 20050715
	.endif									//added by zgq on 20050715
		jmp _Continue_2;
.endif

.ifdef	S530_Used
L_S530_Decode_Proc:
	.ifdef C_SpeedControl					//added by zgq on 20050715
	/////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x30
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
	//	call F_SACM_XXXX_Speed_Process_Psuedo;	//add by benwang 20060120
	//////////////////////////
	.else									//added by zgq on 20050715
//		r1 = [R_SACM_DAC_Out_PTR_Decode];	//mov to *.asm//added by zgq on 20050715
//		[R_SACM_Decode_Out_PTR] = r1;		//mov to *.asm//added by zgq on 20050715
		call F_SACM_Decode_Process;			//added by zgq on 20050715
	.endif									//added by zgq on 20050715
		jmp _Continue_2;
.endif

.ifdef	S600TTS_Used
L_S600TTS_Decode_Proc:

	.ifdef C_SpeedControl					//added by zgq on 20050715
	//////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x30
		r4=0x01
		call mr
		pop	r3,r4 from [sp]	
	//	call F_SACM_XXXX_Speed_Process_Psuedo;	//add by benwang 20060120
	////////////////////////
	.else									//added by zgq on 20050715
//		r1 = [R_SACM_DAC_Out_PTR_Decode];	//mov to *.asm//added by zgq on 20050715
//		[R_SACM_Decode_Out_PTR] = r1;		//mov to *.asm//added by zgq on 20050715
		call F_SACM_Decode_Process;			//added by zgq on 20050715
	.endif									//added by zgq on 20050715

		jmp _Continue_2;
.endif

.ifdef	S320TTS_Used
L_S320TTS_Decode_Proc:
		call F_SACM_Decode_Process;
		jmp _Continue_2;
.endif

.ifdef A6400S_Used				//added by zgq on 20051215
L_A6400S_Decode_Proc:			//added by zgq on 20051215
		call F_SACM_Decode_Process;//added by zgq on 20051215
		jmp _Continue_2;		//added by zgq on 20051215
.endif							//added by zgq on 20051215


.ifdef DVR520_Used
L_DVR520_Decode_Proc:							//added by zgq on 20051220
	.ifdef C_SpeedControl					//added by zgq on 20051220
		//////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x30
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call F_SACM_XXXX_Speed_Process_Psuedo;	//add by benwang 20060120
		/////////////////////
	.else									//added by zgq on 20051220
		call F_SACM_Decode_Process;			//added by zgq on 20051220
	.endif									//added by zgq on 20051220
		jmp _Continue_2;
.endif

.ifdef	MS01_Used
L_MS01_Decode_Proc:
		call F_SACM_Decode_Process;
		jmp _Continue_2;
.endif

.ifdef	MS02_Used
L_MS02_Decode_Proc:
		call F_SACM_Decode_Process;
		jmp _Continue_2;
.endif

.ifdef	SPEECH1600_Used
L_A1600_Decode_Proc:
	.ifdef C_SpeedControl					//added by benwang 2005/07/26
		/////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x030
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call F_SACM_XXXX_Speed_Process_Psuedo;	//add by benwang 20060120
		/////////////////
	.else									//added by benwang 2005/07/26
//		r1 = [R_SACM_DAC_Out_PTR_Decode];	//mov to *.asm//added by zgq on 20050715
//		[R_SACM_Decode_Out_PTR] = r1;		//mov to *.asm//added by zgq on 20050715
		call F_SACM_Decode_Process;			//added by benwang 2005/07/26
	.endif									//added by benwang 2005/07/26
		jmp _Continue_2;

.else
.ifdef	AUDIO1600_Used
L_A1600_Decode_Proc:
	.ifdef C_SpeedControl					//added by benwang 2005/07/26
		//////////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x030
		r4=0x01
		call mr
		pop	r3,r4 from [sp]		
		//call F_SACM_XXXX_Speed_Process_Psuedo;	//add by benwang 20060120
		////////////////////////////
	.else									//added by benwang 2005/07/26
//		r1 = [R_SACM_DAC_Out_PTR_Decode];	//mov to *.asm//added by zgq on 20050715
//		[R_SACM_Decode_Out_PTR] = r1;		//mov to *.asm//added by zgq on 20050715
		call F_SACM_Decode_Process;			//added by benwang 2005/07/26
	.endif									//added by benwang 2005/07/26
		jmp _Continue_2;
	.endif
.endif

.ifdef	SPEECH4800_Used						//modify by chengye 2006/11/15
L_A4800_Decode_Proc:
		.ifdef C_SpeedControl            			 	
		/////////////		//jacky 0727
		push r3,r4 to [sp]
		r3=0x052
		r4=0x01
		call mr
		pop	r3,r4 from [sp]	
		//call F_SACM_XXXX_Speed_Process_16K_Psuedo;	 	
		/////////////	 		
		.else                            					 
		call F_SACM_Decode_Process;      					 
		.endif	                         				 
		jmp _Continue_2; 								
.else
	.ifdef	AUDIO4800_Used
L_A4800_Decode_Proc:
	.ifdef C_SpeedControl            			 
		/////////////		//jacky 0727
		push r3,r4 to [sp]
		r3=0x052
		r4=0x01
		call mr
		pop	r3,r4 from [sp]	
		//call F_SACM_XXXX_Speed_Process_16K_Psuedo;	 	
		/////////////
		.else                            				 
		call F_SACM_Decode_Process;      				 
		.endif	                         					 
		jmp _Continue_2; 									 
	 .endif											   
.endif

.ifdef	AUDIO3200_Used						//added by benwang 20051220
L_A3200_Decode_Proc:					//added by benwang 20051220
	.ifdef C_SpeedControl					//added by benwang 2005/07/26
		//////////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x030
		r4=0x01
		call mr
		pop	r3,r4 from [sp]		
//		call F_SACM_XXXX_Speed_Process_Psuedo;	//add by benwang 20060120
		////////////////////////////	
	.else									//added by benwang 20051220
//		r1 = [R_SACM_DAC_Out_PTR_Decode];	//mov to *.asm//added by zgq on 20050715
//		[R_SACM_Decode_Out_PTR] = r1;		//mov to *.asm//added by zgq on 20050715
		call F_SACM_Decode_Process;			//added by benwang 20051220
	.endif									//added by benwang 20051220
		jmp _Continue_2;  					//added by benwang 20051220
.endif										//added by benwang 20051220

.ifdef	MP3_Used  							//add by benwang 20060111
L_MP3_Decode_Proc: 							//add by benwang 20060111
		call F_SACM_Decode_Process; 		//add by benwang 20060111
		jmp _Continue_2;					//add by benwang 20060111
.endif										//add by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used
L_A1601_Decode_Proc:
	.ifdef C_SpeedControl					
		///////////////////////			//jacky 0512
		push r3,r4 to [sp]
		r3=0x030
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call F_SACM_XXXX_Speed_Process_Psuedo;	//add by benwang 20060120
		/////////////////////////
	.else									
//		r1 = [R_SACM_DAC_Out_PTR_Decode];	
//		[R_SACM_Decode_Out_PTR] = r1;		
		call F_SACM_Decode_Process;			
	.endif									
		jmp _Continue_2;
.endif
//add by benwang 20060119

.ifdef	A128_Used  							//add by benwang 20060221
L_A128_Decode_Proc: 						//add by benwang 20060221
		call F_SACM_Decode_Process; 		//add by benwang 20060221
		jmp _Continue_2;					//add by benwang 20060221
.endif										//add by benwang 20060221

///////////////////////////     //add by benwang 20060621
.ifdef	DVR1800_Used
L_DVR1800_Decode_Proc:
	.ifdef C_SpeedControl								//20060629
		push r3,r4 to [sp]
		r3=0x052
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call F_SACM_XXXX_Speed_Process_16K_Psuedo;	//20060629
	.else												//20060629
		call F_SACM_Decode_Process;						//20060629
	.endif												//20060629
		jmp _Continue_2;
.endif
////////////////////////////////
.ifdef	WAV_Used
L_WAV_Decode_Proc:								
		call F_SACM_Decode_Process;															
		jmp _Continue_2;
.endif	
//------------------------------------------------------

//========================================================
.ifdef	ETTS_Used					//added by chengye 2007/4/18
L_ETTS_Decode_Proc:
	call F_SACM_Decode_Process;
	jmp _Continue_2;		
.endif
//========================================================
.ifdef	S320_Used
L_S320_Decode_Proc:					//added by chengye 2007/4/28
	call F_SACM_Decode_Process;
	jmp _Continue_2;
.endif
//////////////////////////////////////////
_Continue_2:
		irq off						//added by chengye 2006/11/14
	    R1 = [R_SACM_Play_Flag];
	    test R1, C_SACM_DECODE_ODD;
	    jnz ?L_Branch_0;
	    
	    r2 = [R_DAC_OUT_LENGTH];
	    r2 += R_SACM_DAC_Out_Buffer;
	    jmp ?L_Branch_1; 
     
?L_Branch_0:    
	    R2 = R_SACM_DAC_Out_Buffer + 0;	        
?L_Branch_1: 
	    [R_SACM_DAC_Out_PTR_Decode] = R2;   
	    R1 ^= C_SACM_DECODE_ODD;
		[R_SACM_Play_Flag] = R1;

		irq on						//added by chengye 2006/11/14

		test R1, C_SACM_DECODE_END;
		jnz L_Branch_2_A;

//add by benwang 20051020
		r1=sr&0x03f
		ds=r1
		R1 = [R_CODEC_TYPE];
		r1+=T_TTS_GetDS2
		pc=ds:[r1]

.ifdef	S600TTS_Used
L_SACM_S600TTS_DS2:
		call F_SACM_TTS600_System_Get_BS_Manual;
		JMP L_Branch_2_A;
.endif

.ifdef	S200TTS_Used
L_SACM_S200TTS_DS2:
		call F_S200TTS_System_Get_BS_Manual;
		JMP L_Branch_2_A;
.endif

.ifdef	S320TTS_Used
L_SACM_S320TTS_DS2:
		call F_SACM_TTS320_System_Get_BS_Manual; //add by benwang 20051020
		JMP L_Branch_2_A;
.endif

L_SACM_NotTTS_DS2:
		call F_SACM_System_Get_BS_Manual;
		jmp L_DecodeEnd
//add by benwang 20051020
L_Branch_2_A:
//	R1 = [R_CODEC_TYPE]
//	cmp R1, C_CODEC_MP3
//	jnz L_DecodeEnd
//	call _PlayNextFile
L_DecodeEnd:
//L_Branch_2_A:
	r1=1
	[R_Decode_Finish]=r1				//add by jacky 2006.02.15
.ifndef Background_mode		//2007/01/11 add
	pop R1, R5 from [SP];				//should delete this sentence when background play
.endif
	retf;
.endp


//========================================================
// Function Name:	_SACM_Pause
// Syntax: 		void SACM_Pause(void);
// Purpose:	 	Pause speech 
// Note:		NO
// Parameter:	N/A
// Return:		N/A
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SACM_Pause: .proc
F_SACM_Pause:
	    push r1 to [sp];
		R1 = [R_SACM_Play_Flag];
		test R1, C_SACM_PLAY;	
		jz ?L_Branch_1;
	
		R1 |=  C_SACM_PAUSE;	
		R1 &= ~C_SACM_FIQ_SOUND;
		[R_SACM_Play_Flag] = R1;
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//%%%       Clear Disable Channel FIFO Empty Interupt				%
	//%%%       Latch FIFO data to DAC/PWM after writing to control reg %
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       	int	off					//add by chengye 2006/12/16
        r1 = [R_SACM_Play_Flag]			                      //%
        test r1,C_SACM_ENABLE_DAC1								  //%
        jz L_EndDACA?											  	  //%
		call	F_SACM_CHA_PlayInterrupt_Disable
L_EndDACA?:															  //%
        r1 = [R_SACM_Play_Flag]								  //%
		test r1,C_SACM_ENABLE_DAC2							  	  //%
        jz L_EndDACB?											  	  //%
		call	F_SACM_CHB_PlayInterrupt_Disable
L_EndDACB?:															  //%
				
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%		
			
?_Branch_0:
//-----------------------------------------------------------------------
//added by chengye 2007/7/23 for BBK
		test R1, C_SACM_CODEC_MODE					//录音暂停
		jz ?L_Branch_1
	
		call F_SACM_ADC_RecordInterrupt_Disable 	//暂停时关闭AD的中断		
		int irq, fiq								//add by chengye 2006/12/16
?L_Branch_1:
//-----------------------------------------------------------------------	    	    
	    pop r1 from [sp];
	   	retf;
.endp	   
	   
//========================================================
// Function Name:	_SACM_Resume
// Syntax:		void SACM_Resume(void);
// Purpose: 	Resume speech 
// Note:		NO
// Parameter: 	N/A
// Return:		N/A
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SACM_Resume: .proc
F_SACM_Resume:
	    push r1 to [sp];
		R1 = [R_SACM_Play_Flag];
		test R1, C_SACM_PAUSE;
		jz ?_Branch_0;
	
		R1 &= ~C_SACM_PAUSE;
		R1 |=  C_SACM_FIQ_SOUND;
		[R_SACM_Play_Flag] = R1;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	 	r1 = [R_SACM_Play_Flag]			                          //%
        test r1,C_SACM_ENABLE_DAC1								  //%
        jz L_EndDACA?											  	  //%
		call	F_SACM_CHA_PlayInterrupt_Enable						  //%
L_EndDACA?:															  //%
        r1 = [R_SACM_Play_Flag]								  //%
		test r1,C_SACM_ENABLE_DAC2								  //%
        jz L_EndDACB?											  	  //%
		call	F_SACM_CHB_PlayInterrupt_Enable						  //%
L_EndDACB?:															  //%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//-----------------------------------------------------------------------
//added by chengye 2007/7/23
	    test R1, C_SACM_CODEC_MODE
		jz ?_Branch_0
		call F_SACM_ADC_RecordInterrupt_Enable
//-----------------------------------------------------------------------	    
?_Branch_0:	    
	    pop r1 from [sp];
	   	retf;
.endp	   

///////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////
F_Reset_File_Access:
		R1 = 0x7fff;
		[R_SACM_Encode_Length_H] = R1;				// Set File Size
		R1 = 0xFFFF;
		[R_SACM_Encode_Length_L] = R1;				// Set File Size
		RETF;	
//..............................................................................

//========================================================
// Function Name:	_SACM_Rec
// Syntax: 		void SACM_DVR1600_Rec(int RecMonitorOff, int Bitrate)
// Purpose: 	Record from Mic or line in
// Note:		NO
// Parameter:
//  			RecMonitor: 1				: RecMonitorOn  (ADC-->DAC)
//                     		0 - max index	: RecMonitorOff (ADC)
//	
//				DVR1600
//				Bitrate:	0: 10 Kbps
//							1: 12 Kbps
//							2: 14 Kbps
//							3: 16 Kbps
//							4: 20 Kbps
//							5: 24 Kbps
//				DVR4800
//				Bitrate:	0: 32 Kbps
//							1: 36 Kbps
//							2: 40 Kbps
//							3: 44 Kbps
//							4: 48 Kbps
//							5: 52 Kbps
//							6: 56 Kbps
//		Bitrate:
//		DVR_WAVE use to set the type
//-----------------------------five types are supported-----------
//			WAVE_FORMAT_PCM			(0x1)
//			WAVE_FORMAT_ADPCM		(0x2)
//			WAVE_FORMAT_ALAW		(0x6)
//			WAVE_FORMAT_MULAW		(0x7)
//			WAVE_FORMAT_IMA_ADPCM	(0x11)
// Return:		N/A
// Destory: 	R1,R2,R3
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public _SACM_Rec
.public SACM_Rec
.public	F_SACM_Rec
SACM_Rec:
_SACM_Rec: .proc //SpeechIndex,DAC1+DAC2,0
	.ifdef dump_encode_data	
		r1 = 0x26
		[encode_addrh] = r1;
		r1 = 0;
		[encode_addrl] = r1;
	.endif
	
	.ifdef DumpEncodeData	
		r1 = 0x23
		[EncodeDataAddrH] = r1;
		r1 = 2;
		[EncodeDataAddrL] = r1;
	.endif
	
		call	F_SACM_ADC_ControlLogic_Disable
//		call	F_SACM_ADC_RecordInterrupt_Disable	
	
   		push BP to [SP];
   		BP = SP + 1;
	   	R1 = [BP + 3]; 							// RceMonitorOff
	   	R2 = [BP + 4]; 							// Bitrate
	   	
.ifdef DVR520_Used
		push R1, R3 to [sp]
		R1 = [R_CODEC_TYPE]
		cmp R1, C_CODEC_DVR520
		jnz ?NotDVR520CopyText
		
		r1 = SEG16 DVR520_TEXT_BEGIN_LOAD_Adr
	   	ds = r1;							
	   	R1 = OFFSET DVR520_TEXT_BEGIN_LOAD_Adr	
	   	r2 = OFFSET DVR520_TEXT_END_LOAD_Adr
	   	r2 -= R1							
	   	r3 = DVR520CODEINRAM_Reserved_Addr		
DVR520CopyCode?: 									
	   	r4 = D:[r1++] 						
	   	[r3++] = r4 						
	   	r2 -= 1 							
	   	jnz   DVR520CopyCode?
?NotDVR520CopyText:	
		pop R1, R3 from [sp]
.endif
	   	
	    call F_SACM_Rec
   	
		pop BP from [SP];
   		retf;
	   
F_SACM_Rec:    
    	[BITRATE] = R2;	   
		push R1, R2 to [SP]; 
	   	call F_SACM_Stop;
	   	pop R1, R2 from [SP];  
	   
		R4 = C_SACM_PLAY;	
		cmp R1,1; 							// Record Monitor on
		jne ?L_NoRecMon;
		call	F_SACM_ADC_MoniterON
		
		R4 |= C_SACM_REC_Mon_ON;
			
?L_NoRecMon:	
		R4|= C_SACM_CODEC_MODE;  		// Encode mode --> Recording
	   	[R_SACM_Play_Flag] = R4; 		//?_Branch_4 result
// Resouce Proc

		call F_Reset_File_Access;
		 
		R1 = R_SACM_Encode_Out_Buffer;
		[R_SACM_Encode_Out_PTR] = R1;
			  
	   	call F_SACM_Encode_Initial_BS;
		call F_SACM_Encode_Initial_Process;
	
		R1 = 0x0000;
		R2 = R_SACM_ADC_In_Buffer;
		r3=[R_ADC_IN_LENGTH]
		R3 += R2 
?_Loop_0:	
		[R2++] = R1;
		cmp R2, R3;
		jne ?_Loop_0;  

//Init 32K ADC
//		R1=0;
//		[AD_BUF_PTR] = R1;
//		R2 = AD_CBUF;
//?_32K_Init:		
//		[R2++] = R1;
//		CMP R2 , AD_CBUF + 32;
//		JNE ?_32K_Init;
//End init 32k ADC
	    r1=fr							//added by chengye 2007/5/30
		[R_SACM_FR_Flag]=r1 
	    
	    R1 = 0
	    [_pRecWriteptr] = R1			//added by chengye for two buffer record 2007/3/27
	      				
		R1 = R_SACM_ADC_In_Buffer;
		[R_SACM_ADC_In_PTR_Rec] = R1;
		
		R1 = [R_SACM_Play_Flag];
		R1 |= ( C_SACM_DECODE_ODD + C_SACM_FIQ_SOUND + C_SACM_FIQ_EVEN);
		[R_SACM_Play_Flag] = R1;  

///////////////////////////////////////////////////////////////////
	
		r2=sr&0x03f
		ds=r2
		r2 = [R_CODEC_TYPE];
		r2 += offset T_SACM_ADC_IRQ
		pc = ds:[r2];
		nop	
L_SACM_ADC_8K:
		call	F_ADC_SetIRQ_8000
		jmp		L_SACM_ADC_IRQNotSet
L_SACM_ADC_16K:
		call	F_ADC_SetIRQ_16000
L_SACM_ADC_IRQNotSet:


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   		call	F_SACM_ADC_ControlLogic_Enable
//	   	call	F_SACM_ADC_RecordInterrupt_Enable
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
		
		r1 = 1;								//add by zgq on 2005/3/3
		[R_SACM_ISR_Flag] = r1;			//add by zgq on 2005/3/3
		[_R_ISR_DataProc_Flag] = R1		//added by chengye 2007/4/24
	
   	retf;
.endp

//2005/03/31
_SACM_Pitch: .proc
   	R1 = SP + 3;
   	R1 = [R1];
	[R_SACM_Play_Pitch] = R1;	   
   	retf;
.endp

_SACM_Spectrum: .proc
   	R1 = SP + 3;
   	R1 = [R1];
	[R_SACM_Play_Spectrum] = R1;	   
   	retf;
.endp

//add by benwang 20051124
_SACM_Pitch0: .proc
   	R1 = SP + 3;
   	R1 = [R1];
	[R_SACM_Play_Pitch0] = R1;	   
   	retf;
.endp



_SACM_Pitch1: .proc
   	R1 = SP + 3;
   	R1 = [R1];
	[R_SACM_Play_Pitch1] = R1;	   
   	retf;
.endp



_SACM_Voice: .proc
   	R1 = SP + 3;
   	R1 = [R1];
	[R_SACM_Play_Voice] = R1;	   
   	retf;
.endp

//begin added by zgq on 20051220
.ifdef DVR520_Used
.external F_SetVADTHR //add by benwang 20051221

_SACM_DVR520_VADTHR: .proc
   	R1 = SP + 3;
   	R1 = [R1];	   
    call F_SACM_DVR520_VADTHR;
   	retf;
F_SACM_DVR520_VADTHR:
	//[R_VAD_THR] = R1; //del by benwang 20051221
	call F_SetVADTHR; //add by benwang 20051221
	retf;
.endp
.endif
//end added by zgq on 20051220

//.ifdef DVR1800_Used					//delete by chengye 2007/7/24
_SACM_DVR1800_BITRATE: .proc
	push BP to [SP];
   	BP = SP + 1;
   	R3 = [BP + 3]; // BitRate
// 	R4 = [BP + 4]; // Number of Bit per Frame 
   	   
    call F_SACM_DVR1800_BITRATE;
    
    pop BP from [sp]
   	retf;
   	
F_SACM_DVR1800_BITRATE:
	[R_BITRATE] = R3;
//	[R_BITSOFEACHFRAME] = R4;
	retf;
.endp
//.endif

//$$$$$$$$$$$$$$$$Must in SACM_XXXX_API.asm$$$$$$$$$$$$$
	
T_SACM_Speech_Table_Init_BS:
.ifdef	S200_Used	
	.dw	offset L_S200_Init_BS 		//S200			0
.endif

.ifdef	S4872_Used
	.dw	offset L_SACM_Init_BS 		//S4872 			1
.endif

.ifdef	S530_Used	
	.dw	offset L_SACM_Init_BS 		//S530 			2 
.endif	

.ifdef	S600TTS_Used	
	.dw	offset L_S600TTS_Init_BS 	//S600TTS		3	
.endif

.ifdef	S200TTS_Used	
	.dw	offset L_S200TTS_Init_BS	//S200TTS		4	
.endif	

.ifdef	S320TTS_Used	
	.dw	offset L_S320TTS_Init_BS	//S320TTS		5
.endif

.ifdef	MS01_Used
	.dw	offset L_SACM_Init_BS		//ms01			6
.endif

.ifdef	MS02_Used
	.dw	offset L_SACM_Init_BS		//ms02			7
.endif	

.ifdef	SPEECH1600_Used	
	.dw	offset L_SACM_Init_BS 		//SPEECH1600	8
.endif	

.ifdef	AUDIO1600_Used	
	.dw	offset L_SACM_Init_BS		//AUDIO1600		9
.endif

.ifdef	SPEECH4800_Used
	.dw	offset L_SACM_Init_BS 		//SPEECH4800	10
.endif

.ifdef	AUDIO4800_Used
	.dw offset L_SACM_Init_BS 		//AUDIO4800		11 		
.endif	

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw offset L_SACM_Init_BS 	//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used			//added by benwang 20051220
	.dw offset L_SACM_Init_BS 	//added by benwang 20051220
.endif							//added by benwang 20051220

.ifdef DVR520_Used				//added by zgq on 20051220
	.dw offset L_SACM_Init_BS 	//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used					//added by benwang 20060111
	.dw offset L_SACM_Init_BS 	//added by benwang 20060111
.endif							//added by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset L_SACM_Init_BS
.endif

.ifdef A128_Used				//added by benwang 20060221
	.dw offset L_SACM_Init_BS 	//added by benwang 20060221
.endif

.ifdef	DVR1800_Used	
	.dw	offset L_SACM_Init_BS		//add by benwang 20060621
.endif
.ifdef	WAV_Used	
	.dw	offset L_SACM_Init_BS	//added by chengye 2007/7/9
.endif
.ifdef	ETTS_Used	
	.dw	offset L_SACM_Continue_0	//added chengye 2007/10/30
.endif

.ifdef	S320_Used	
	.dw	offset L_SACM_Init_BS		//add by benwang 20060621
.endif

//////////////////////////////////////////	
T_SACM_Decode_Proc:
.ifdef	S200_Used
	.dw	offset L_S200_Decode_Proc 			//S200			0
.endif

.ifdef	S4872_Used
	.dw	offset L_S4872_Decode_Proc	 		//S4872 			1
.endif	
	
.ifdef	S530_Used	
	.dw	offset L_S530_Decode_Proc			//S530 			2 
.endif
	
.ifdef	S600TTS_Used	
	.dw	offset L_S600TTS_Decode_Proc		//S600TTS		3	
.endif

.ifdef	S200TTS_Used	
	.dw	offset L_S200_Decode_Proc			//S200TTS		4
.endif	
	
.ifdef	S320TTS_Used	
	.dw	offset L_S320TTS_Decode_Proc		//S320TTS		5
.endif	
	
.ifdef	MS01_Used	
	.dw	offset L_MS01_Decode_Proc			//ms01			6
.endif

.ifdef	MS02_Used
	.dw	offset L_MS02_Decode_Proc			//ms02			7
.endif

.ifdef	SPEECH1600_Used
	.dw	offset L_A1600_Decode_Proc			//SPEECH1600	8 
.endif

.ifdef	AUDIO1600_Used	
	.dw	offset L_A1600_Decode_Proc			//AUDIO1600		9 
.endif

.ifdef	SPEECH4800_Used	
	.dw	offset L_A4800_Decode_Proc			//SPEECH4800	10 
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset L_A4800_Decode_Proc			//AUDIO4800		11 
.endif			

.ifdef A6400S_Used					//added by zgq on 20051215
	.dw offset L_A6400S_Decode_Proc	//added by zgq on 20051215
.endif								//added by zgq on 20051215

.ifdef AUDIO3200_Used				//added by benwang 20051220
	.dw offset L_A3200_Decode_Proc	//added by benwang 20051220
.endif								//added by benwang 20051220

.ifdef	DVR520_Used					//added by zgq on 20051220	
	.dw	offset L_DVR520_Decode_Proc	//added by zgq on 20051220			
.endif								//added by zgq on 20051220	

.ifdef MP3_Used						//added by benwang 20060111
	.dw offset L_MP3_Decode_Proc	//added by benwang 20060111
.endif								//added by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset L_A1601_Decode_Proc			
.endif

.ifdef A128_Used						//added by benwang 20060221
	.dw offset L_A128_Decode_Proc	//added by benwang 20060221
.endif

.ifdef	DVR1800_Used					//added by benwang 20060621
	.dw	offset L_DVR1800_Decode_Proc	//added by benwang 20060621		
.endif

.ifdef	WAV_Used					
	.dw	offset L_WAV_Decode_Proc	
.endif

.ifdef	ETTS_Used						//added by chengye 2007/4/18
	.dw	offset L_ETTS_Decode_Proc		//added by chengye 2007/4/18		
.endif

.ifdef	S320_Used						//added by chengye 2007/4/28
	.dw	offset L_S320_Decode_Proc		//added by chengye 2007/4/28		
.endif

/////////////////////////////////////////////////////////	
T_SACM_IRQ:					
.ifdef	S200_Used
	.dw	offset	L_SACM_8K	//S200			0
.endif

.ifdef	S4872_Used
	.dw	offset	L_SACM_8K	//S4872 			1
.endif

.ifdef	S530_Used
	.dw	offset	L_SACM_8K	//S530 			2 
.endif

.ifdef	S600TTS_Used
	.dw	offset	L_SACM_9_2K	//S600TTS		3 
.endif	
	
.ifdef	S200TTS_Used	
	.dw	offset	L_SACM_11K	//S200TTS		4 
.endif	

.ifdef	S320TTS_Used	
	.dw	offset	L_SACM_11K	//S320TTS		5 
.endif	
	
.ifdef	MS01_Used	
	.dw	offset	L_SACM_16K	//MS01 			6 
.endif	

.ifdef	MS02_Used	
	.dw	offset	L_SACM_16K	//MS02 			7 
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset	L_SACM_8K	//SPEECH1600	8 
.endif	
	
.ifdef	AUDIO1600_Used	
	.dw	offset	L_SACM_16K	//AUDIO1600		9 
.endif

.ifdef	SPEECH4800_Used
	.dw	offset	L_SACM_8K	//SPEECH4800	10 
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset	L_SACM_16K	//AUDIO4800		11 
.endif	

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw offset L_SACM_setIRQ_end//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used			//added by benwang 20051220
	.dw offset L_SACM_8K		//added by benwang 20051220
.endif							//added by benwang 20051220

.ifdef DVR520_Used				//added by zgq on 20051220
	.dw offset L_SACM_8K		//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used					//added by benwang 20051220
	.dw offset L_SACM_setIRQ_end//added by benwang 20051220
.endif							//added by benwang 20051220

//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset	L_SACM_8K	
.endif

.ifdef A128_Used					//added by benwang 20060221
	.dw offset L_SACM_setIRQ_end//added by benwang 20060221
.endif							//added by benwang 20060221

.ifdef	DVR1800_Used	
	.dw	offset	L_SACM_16K	    //added by benwang 20060621
.endif
.ifdef	WAV_Used	  			//not do upsample & downsample procession
	.dw	offset	L_SACM_8K		//added by chengye 2007/9/7
.endif	
.ifdef	ETTS_Used	
	.dw	offset	L_SACM_11K	    //added by chengye 2007/4/18
.endif
.ifdef	S320_Used	
//	.dw	offset	L_SACM_8K	    //added by chengye 2007/4/18
	.dw	offset	L_SACM_11K		//modify by chengye 2007/6/11 
.endif

////////////////////////////////////////////////////
T_SACM_IRQ_UpSample:
.ifdef	S200_Used
	.dw	offset	L_SACM_16K	//S200			0
.endif

.ifdef	S4872_Used
	.dw	offset	L_SACM_16K	//S4872 		1
.endif

.ifdef	S530_Used
	.dw	offset	L_SACM_16K	//S530 			2 
.endif

.ifdef	S600TTS_Used
	.dw	offset	L_SACM_9_2K	//S600TTS		3 
.endif	
	
.ifdef	S200TTS_Used	
	.dw	offset	L_SACM_22K	//S200TTS		4 
.endif	

.ifdef	S320TTS_Used	
	.dw	offset	L_SACM_22K	//S320TTS		5 
.endif	
	
.ifdef	MS01_Used	
	.dw	offset	L_SACM_32K	//MS01 			6 
.endif	

.ifdef	MS02_Used	
	.dw	offset	L_SACM_32K	//MS02 			7 
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset	L_SACM_16K	//SPEECH1600	8 
.endif	
	
.ifdef	AUDIO1600_Used	
	.dw	offset	L_SACM_32K	//AUDIO1600		9 
.endif

.ifdef	SPEECH4800_Used
	.dw	offset	L_SACM_16K	//SPEECH4800	10 
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset	L_SACM_32K	//AUDIO4800		11 
.endif	

.ifdef A6400S_Used					//added by zgq on 20051215
	.dw offset L_SACM_setIRQ_end	//added by zgq on 20051215
.endif								//added by zgq on 20051215

.ifdef AUDIO3200_Used			//added by benwang 20051220
	.dw offset L_SACM_16K		//added by benwang 20051220
.endif							//added by benwang 20051220

.ifdef DVR520_Used				//added by zgq on 20051220
	.dw offset L_SACM_16K		//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used					//added by benwang 20051220
	.dw offset L_SACM_setIRQ_end//added by benwang 20051220
.endif							//added by benwang 20051220

//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset	L_SACM_16K	
.endif

.ifdef A128_Used					//added by benwang 20060221
	.dw offset L_SACM_setIRQ_end//added by benwang 20060221
.endif							//added by benwang 20060221
	
.ifdef	DVR1800_Used	
	.dw	offset	L_SACM_32K	   
.endif
	
.ifdef	WAV_Used	  			//not do upsample & downsample procession
	.dw	offset	L_SACM_8K		//added by chengye 2007/9/7
.endif	

.ifdef	ETTS_Used	
	.dw	offset	L_SACM_11K	    //added by chengye 2007/4/18
.endif

.ifdef	S320_Used	
	.dw	offset	L_SACM_16K	    //added by chengye 2007/4/18
.endif

/////////////////////////////////////////////////////////	
T_SACM_ADC_IRQ:					
.ifdef	S200_Used
	.dw	offset	L_SACM_ADC_IRQNotSet	//S200			0
.endif

.ifdef	S4872_Used
	.dw	offset	L_SACM_ADC_IRQNotSet	//S4872 		1
.endif

.ifdef	S530_Used
	.dw	offset	L_SACM_ADC_IRQNotSet	//S530 			2 
.endif

.ifdef	S600TTS_Used
	.dw	offset	L_SACM_ADC_IRQNotSet	//S600TTS		3 
.endif	
	
.ifdef	S200TTS_Used	
	.dw	offset	L_SACM_ADC_IRQNotSet	//S200TTS		4 
.endif	

.ifdef	S320TTS_Used	
	.dw	offset	L_SACM_ADC_IRQNotSet	//S320TTS		5 
.endif	
	
.ifdef	MS01_Used	
	.dw	offset	L_SACM_ADC_IRQNotSet	//MS01 			6 
.endif	

.ifdef	MS02_Used	
	.dw	offset	L_SACM_ADC_IRQNotSet	//MS02 			7 
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset	L_SACM_ADC_8K		//SPEECH1600	8 
.endif	
	
.ifdef	AUDIO1600_Used	
	.dw	offset	L_SACM_ADC_IRQNotSet	//AUDIO1600		9 
.endif

.ifdef	SPEECH4800_Used
	.dw	offset	L_SACM_ADC_IRQNotSet	//SPEECH4800	10 
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset	L_SACM_ADC_16K		//AUDIO4800		11 
.endif	

.ifdef A6400S_Used					//added by zgq on 20051215
	.dw offset L_SACM_ADC_IRQNotSet	//added by zgq on 20051215
.endif								//added by zgq on 20051215

.ifdef AUDIO3200_Used				//added by benwang 20051220
//	.dw offset L_SACM_ADC_16K		//added by benwang 20051220
	.dw	offset	L_SACM_ADC_8K		//modify by chengye 2006/9/12
.endif								//added by benwang 20051220

.ifdef DVR520_Used					//added by zgq on 20051220
	.dw offset L_SACM_ADC_8K		//added by zgq on 20051220
.endif								//added by zgq on 20051220

.ifdef MP3_Used						//added by benwang 20051220
	.dw offset L_SACM_ADC_IRQNotSet	//added by benwang 20051220
.endif								//added by benwang 20051220

//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset	L_SACM_ADC_IRQNotSet	
.endif

.ifdef A128_Used						//added by benwang 20051220
	.dw offset L_SACM_ADC_IRQNotSet	//added by benwang 20051220
.endif								//added by benwang 20051220

.ifdef DVR1800_Used				//added by benwang 20051220
	.dw offset L_SACM_ADC_16K		//added by benwang 20051220
.endif	
.ifdef	WAV_Used	  			
	.dw offset L_SACM_ADC_16K//	
.endif

/////////////////////////////////////	
////////////////////////////////////
T_TTS_GetDS:
.ifdef	S200_Used
	.dw	offset	L_SACM_NotTTS_DS	
.endif

.ifdef	S4872_Used	
	.dw	offset	L_SACM_NotTTS_DS
.endif

.ifdef	S530_Used	
	.dw	offset	L_SACM_NotTTS_DS
.endif	
	
.ifdef	S600TTS_Used
	.dw	offset	L_SACM_S600TTS_DS
.endif

.ifdef	S200TTS_Used
	.dw	offset	L_SACM_S200TTS_DS
.endif	

.ifdef	S320TTS_Used	
	.dw	offset	L_SACM_S320TTS_DS
.endif

.ifdef	MS01_Used	
	.dw	offset	L_SACM_NotTTS_DS
.endif

.ifdef	MS02_Used	
	.dw	offset	L_SACM_NotTTS_DS
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset	L_SACM_NotTTS_DS
.endif

.ifdef	AUDIO1600_Used	
	.dw	offset	L_SACM_NotTTS_DS
.endif

.ifdef	SPEECH4800_Used
	.dw	offset	L_SACM_NotTTS_DS
.endif
	
.ifdef	AUDIO4800_Used
	.dw	offset	L_SACM_NotTTS_DS
.endif

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw	offset	L_SACM_NotTTS_DS//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used			//added by benwang 20051220
	.dw	offset	L_SACM_NotTTS_DS//added by benwang 20051220
.endif	

.ifdef DVR520_Used				//added by zgq on 20051220
	.dw	offset	L_SACM_NotTTS_DS//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used					//added by benwang 20060111
	.dw	offset	L_SACM_NotTTS_DS//added by benwang 20060111
.endif	

//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset	L_SACM_NotTTS_DS
.endif

.ifdef A128_Used					//added by benwang 20060221
	.dw	offset	L_SACM_NotTTS_DS	//added by benwang 20060221
.endif	

.ifdef	DVR1800_Used	
	.dw	offset	L_SACM_NotTTS_DS   //add by benwang 20060621
.endif

.ifdef	WAV_Used	
	.dw	offset	L_SACM_NotTTS_DS	//added by chengye 2007/7/9
.endif
.ifdef	ETTS_Used	
	.dw	L_DecodeInit   				//added chengye 2007/4/28
.endif

.ifdef	S320_Used	
	.dw	offset	L_SACM_NotTTS_DS   //added chengye 2007/4/28
.endif

/////////////////////////////////////////////
T_TTS_GetDS1:
.ifdef	S200_Used
	.dw	offset	L_SACM_NotTTS_DS1 
.endif	

.ifdef	S4872_Used	
	.dw	offset	L_SACM_NotTTS_DS1
.endif	

.ifdef	S530_Used	
	.dw	offset	L_SACM_NotTTS_DS1
.endif	

.ifdef	S600TTS_Used	
	.dw	offset	L_SACM_S600TTS_DS1
.endif

.ifdef	S200TTS_Used
	.dw	offset	L_SACM_S200TTS_DS1
.endif

.ifdef	S320TTS_Used
	.dw	offset	L_SACM_S320TTS_DS1
.endif	
	
.ifdef	MS01_Used	
	.dw	offset	L_SACM_NotTTS_DS1
.endif	
	
.ifdef	MS02_Used
	.dw	offset	L_SACM_NotTTS_DS1
.endif
	
.ifdef	SPEECH1600_Used
	.dw	offset	L_SACM_NotTTS_DS1
.endif

.ifdef	AUDIO1600_Used
	.dw	offset	L_SACM_NotTTS_DS1
.endif	
	
.ifdef	SPEECH4800_Used	
	.dw	offset	L_SACM_NotTTS_DS1
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset	L_SACM_NotTTS_DS1
.endif

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw	offset	L_SACM_NotTTS_DS1//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used			//added by benwang 20051220
	.dw	offset	L_SACM_NotTTS_DS1//added by benwang 20051220
.endif							//added by benwang 20051220

.ifdef DVR520_Used					//added by zgq on 20051220
	.dw	offset	L_SACM_NotTTS_DS1	//added by zgq on 20051220
.endif								//added by zgq on 20051220

.ifdef MP3_Used						//added by benwang 20060111
	.dw	offset	L_SACM_NotTTS_DS1	//added by benwang 20060111
.endif								//added by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used
	.dw	offset	L_SACM_NotTTS_DS1
.endif	

.ifdef A128_Used						//added by benwang 20060221
	.dw	offset	L_SACM_NotTTS_DS1	//added by benwang 20060221
.endif

.ifdef	DVR1800_Used
	.dw	offset	L_SACM_NotTTS_DS1 
.endif
.ifdef	WAV_Used	
	.dw	offset	L_SACM_NotTTS_DS1	//added by chengye 2007/7/9
.endif

.ifdef	ETTS_Used	
	.dw	L_DecodeInProcess   		//added chengye 2007/4/28
.endif

.ifdef	S320_Used	
	.dw	offset	L_SACM_NotTTS_DS1   //added chengye 2007/4/28
.endif
////////////////////////////////////////////////

/////////////////////////////////////////////
T_TTS_GetDS2:
.ifdef	S200_Used
	.dw	offset	L_SACM_NotTTS_DS2 
.endif	

.ifdef	S4872_Used	
	.dw	offset	L_SACM_NotTTS_DS2
.endif	

.ifdef	S530_Used	
	.dw	offset	L_SACM_NotTTS_DS2
.endif	

.ifdef	S600TTS_Used	
	.dw	offset	L_SACM_S600TTS_DS2
.endif

.ifdef	S200TTS_Used
	.dw	offset	L_SACM_S200TTS_DS2
.endif

.ifdef	S320TTS_Used
	.dw	offset	L_SACM_S320TTS_DS2
.endif	
	
.ifdef	MS01_Used	
	.dw	offset	L_SACM_NotTTS_DS2
.endif	
	
.ifdef	MS02_Used
	.dw	offset	L_SACM_NotTTS_DS2
.endif
	
.ifdef	SPEECH1600_Used
	.dw	offset	L_SACM_NotTTS_DS2
.endif

.ifdef	AUDIO1600_Used
	.dw	offset	L_SACM_NotTTS_DS2
.endif	
	
.ifdef	SPEECH4800_Used	
	.dw	offset	L_SACM_NotTTS_DS2
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset	L_SACM_NotTTS_DS2
.endif

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw	offset	L_SACM_NotTTS_DS2//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used				//added by benwang 20051220
	.dw	offset	L_SACM_NotTTS_DS2//added by benwang 20051220
.endif							//added by benwang 20051220

.ifdef DVR520_Used				//added by zgq on 20051220
	.dw	offset	L_SACM_NotTTS_DS2//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used						//added by benwang 20060111
	.dw	offset	L_SACM_NotTTS_DS2	//added by benwang 20060111
.endif								//added by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used
	.dw	offset	L_SACM_NotTTS_DS2
.endif	

.ifdef A128_Used					//added by benwang 20060221
	.dw	offset	L_SACM_NotTTS_DS2	//added by benwang 20060221
.endif								//added by benwang 20060221

.ifdef	DVR1800_Used	
	.dw	offset	L_SACM_NotTTS_DS2
.endif
.ifdef	WAV_Used	
	.dw	offset	L_SACM_NotTTS_DS2	//added by chengye 2007/7/9
.endif

.ifdef	ETTS_Used	
	.dw	L_DecodeEnd   					//added chengye 2007/4/28
.endif

.ifdef	S320_Used	
	.dw	offset	L_SACM_NotTTS_DS2   //added chengye 2007/4/28
.endif

///////////////////////////////////////////////////
.ifdef C_SpeedControl
T_TTS_GetDS3:
.ifdef	S200_Used
	.dw	offset	L_Out_Buffer_Init 
.endif	

.ifdef	S4872_Used	
	.dw	offset	L_SACM_UseSpeedControlDM_DS3
.endif	

.ifdef	S530_Used	
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif	

.ifdef	S600TTS_Used	
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif

.ifdef	S200TTS_Used
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif

.ifdef	S320TTS_Used
	.dw	offset	L_Out_Buffer_Init
.endif	
	
.ifdef	MS01_Used	
	.dw	offset	L_Out_Buffer_Init 
.endif	
	
.ifdef	MS02_Used
	.dw	offset	L_Out_Buffer_Init
.endif
	
.ifdef	SPEECH1600_Used
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif

.ifdef	AUDIO1600_Used
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif	
	
.ifdef	AUDIO4800_Used	
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif

.ifdef	DVR4800_Used	
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif

.ifdef A6400S_Used				
	.dw	offset	L_SACM_NoUseSpeedControlDM_DS3
.endif							

.ifdef AUDIO3200_Used			
	.dw	offset	L_SACM_UseSpeedControlDM_DS3
.endif							

.ifdef DVR520_Used					
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif								

.ifdef MP3_Used						
	.dw	offset	L_Out_Buffer_Init
.endif								

.ifdef	AUDIO1601_Used
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif	

.ifdef A128_Used						
	.dw	offset	L_Out_Buffer_Init
.endif

.ifdef	DVR1800_Used
	.dw	offset	L_SACM_UseSpeedControlDM_DS3 
.endif
	
.ifdef WAV_Used						
	.dw	offset	L_Out_Buffer_Init
.endif
.ifdef	ETTS_Used
	.dw	offset	L_Out_Buffer_Init
.endif	

.ifdef	S320_Used
	.dw	offset	L_Out_Buffer_Init
.endif

.endif	//end of define C_SpeedControl
///////////////////////////////////////////////////

//add by benwang start 20060209
T_SACM_Decode_In_Len:
.ifdef	S200_Used
	.dw	offset	L_Set_Decode_In_Length 
.endif	

.ifdef	S4872_Used	
	.dw	offset	L_Set_Decode_In_Length
.endif	

.ifdef	S530_Used	
	.dw	offset	L_Set_Decode_In_Length
.endif	

.ifdef	S600TTS_Used	
	.dw	offset	L_Set_Decode_In_Length
.endif

.ifdef	S200TTS_Used
	.dw	offset	L_Set_Decode_In_Length
.endif

.ifdef	S320TTS_Used
	.dw	offset	L_Set_Decode_In_Length
.endif	
	
.ifdef	MS01_Used	
	.dw	offset	L_Set_Decode_In_Length
.endif	
	
.ifdef	MS02_Used
	.dw	offset	L_Set_Decode_In_Length
.endif
	
.ifdef	SPEECH1600_Used
	.dw	offset	L_Set_Decode_In_Length
.endif

.ifdef	AUDIO1600_Used
	.dw	offset	L_Set_Decode_In_Length
.endif	
	
.ifdef	SPEECH4800_Used	
	.dw	offset	L_Set_Decode_In_Length
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset	L_Set_Decode_In_Length
.endif

.ifdef A6400S_Used				
	.dw	offset	L_Set_MP3_Decode_In_Length
.endif							

.ifdef AUDIO3200_Used				
	.dw	offset	L_Set_Decode_In_Length
.endif							

.ifdef DVR520_Used				
	.dw	offset	L_Set_Decode_In_Length
.endif							

.ifdef MP3_Used						
	.dw	offset	L_Set_MP3_Decode_In_Length	
.endif								

.ifdef	AUDIO1601_Used
	.dw	offset	L_Set_Decode_In_Length
.endif	
//add by benwang end 20060209

.ifdef A128_Used			                 //add by benwang 20060221			
	.dw	offset	L_Set_A128_Decode_In_Length	 //add by benwang 20060221	
.endif										 //add by benwang 20060221

.ifdef	DVR1800_Used
	.dw	offset	L_Set_Decode_In_Length    //add by benwang 20060621
.endif	

.ifdef	WAV_Used
	.dw	offset	L_Set_Decode_In_Length	
.endif	
.ifdef	ETTS_Used
	.dw	offset	L_Set_Decode_In_Length    //add by chengye 2007/4/18
.endif

.ifdef	S320_Used
	.dw	offset	L_Set_Decode_In_Length    //add by chengye 2007/4/28
.endif
//$$$$$$$$$$$$$$$$Must in SACM_XXXX_API.asm$$$$$$$$$$$$$
//SACM_TextSection: .section .text //add by benwang 20060114
//.text //del by benwang 20060114
// Volume Table
T_SACM_Volume_Level: 
	.dw 0x0000, 0x0250, 0x0500, 0x1000
	.dw	0x1500, 0x2000, 0x2500, 0x3000
	.dw 0x3500, 0x4000, 0x5000, 0x6500
	.dw	0x7d00, 0x9c00, 0xc400, 0xf500 
//change 0xf50 to 0xf500 by benwang 20060120


//////////////////////////////////////////////////////////////////

SACM_ISR_Service_Section: .section .code //add by benwang to be internal rom 20060117

//begin added by zgq on 20051220
T_SACM_DAC:
.ifdef	S200_Used
	.dw	offset	L_S200_DAC	
.endif

.ifdef	S4872_Used	
	.dw	offset	L_SACM_DAC
.endif

.ifdef	S530_Used	
	.dw	offset	L_S530_DAC
.endif	
	
.ifdef	S600TTS_Used
	.dw	offset	L_SACM_DAC
.endif

.ifdef	S200TTS_Used
	.dw	offset	L_SACM_DAC
.endif	

.ifdef	S320TTS_Used	
//	.dw	offset	L_S320TTS_DAC
	.dw	offset	L_SACM_DAC				//modify by chengye 2007/4/25
.endif

.ifdef	MS01_Used	
	.dw	offset	L_SACM_DAC
.endif

.ifdef	MS02_Used	
	.dw	offset	L_SACM_DAC
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset	L_SACM_DAC
.endif

.ifdef	AUDIO1600_Used	
	.dw	offset	L_SACM_DAC
.endif

.ifdef	SPEECH4800_Used
	.dw	offset	L_SACM_DAC
.endif
	
.ifdef	AUDIO4800_Used
	.dw	offset	L_SACM_DAC
.endif

.ifdef A6400S_Used				
	.dw	offset	L_A6400S_DAC	
.endif	

.ifdef AUDIO3200_Used			//added by benwang 20051220
	.dw	offset	L_SACM_DAC		//added by benwang 20051220
.endif							//added by benwang 20051220

.ifdef	DVR520_Used				//added by zgq on 20051220
	.dw	offset	L_DVR520_DAC	//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used					//add by benwang 20060111	
	.dw	offset	L_MP3_DAC		//add by benwang 20060111		
.endif							//add by benwang 20060111	

//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset	L_SACM_DAC
.endif
//end added by zgq on 20051220

.ifdef A128_Used				//add by benwang 20060221	
	.dw	offset	L_MP3_DAC		//add by benwang 20060221		
.endif							//add by benwang 20060221	

.ifdef	DVR1800_Used	
	.dw	offset	L_SACM_DAC      //added by benwang 20060621
.endif

.ifdef	WAV_Used	
	.dw	offset	L_WAV_DAC		//added by chengye 2007/7/9
.endif

.ifdef	ETTS_Used	
	.dw	offset	L_SACM_DAC
.endif

.ifdef	S320_Used	
//	.dw	offset	L_S320_DAC      //added by chengye 2007/4/18 for upsample
	.dw	offset	L_SACM_DAC
.endif
////////////////////////////////////////////////


//========================================================
// Function Name:	F_ISR_Service_SACM_DAC                  
// Syntax:		call F_ISR_Service_SACM_XXXX_DAC_8K; (in assembly domain)
// Purpose: 	send decoded data to DAC		
// Note:		NO
// Parameter:	N/A
// return: 		N/A
// Destory: 	R1~R5
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
F_ISR_Service_SACM_DAC:	.proc

		push R1, R5 to [SP];
	 	
		R1 = [R_SACM_Play_Flag];
	    test R1, C_SACM_FIQ_SOUND; 
	 	jne ?_Branch_0;
	  	pop R1, R5 from [SP];
	    retf;
     
?_Branch_0:
		r2 = sr&0x03f						
		ds = r2							
		R2 = [R_CODEC_TYPE];			
		r2 += T_SACM_DAC; 			
		pc=ds:[r2]						
.ifdef WAV_Used
L_WAV_DAC:
    	push r1,r1 to [sp];			
		r1 = FR;					
		tstb r1, 12;				
		jz ?Fraction_OFF_a				
		fraction off					
?Fraction_OFF_a:			
			
   		R2 = [R_SACM_DAC_Out_PTR_Play]; 
    	R3 = [R2++]; 					
    	R4 = [R_SACM_Play_Gain]; 		
    	
	    MR = R4 * R3, us;				
 		R4 = R4 ^ 0x8000;						
 		[0x78F1] = R4
 	
 		r3 = [r2++];					
 		r4 = [R_SACM_Play_Gain];	
	    MR = R4 * R3, us;				
 		R4 = R4 ^ 0x8000;					
 		[0x78F1] = R4
 			
		tstb r1, 12;					
		jz ?Fraction_ON_b				
		fraction on						
?Fraction_ON_b:							
		pop r1,r1 from [sp];
					
		goto L_NoDAC_8K					

.endif
//==============================================================
.ifdef A6400S_Used						
L_A6400S_DAC:	
   		R2 = [R_SACM_DAC_Out_PTR_Play];    	
    	R3 = [R2++]; 						
    	R4 = [R_SACM_Play_Gain]; 		
    	
    	push r1,r1 to [sp];				
		r1 = FR;						
		tstb r1, 12;					
		jz ?Fraction_OFF_a				
		fraction off					
?Fraction_OFF_a:						
	    MR = R4 * R3, us;			
		tstb r1, 12;						
		jz ?Fraction_ON_a				
		fraction on							
?Fraction_ON_a:							
		pop r1,r1 from [sp];		
 		R4 = R4 ^ 0x8000;				
 		//call	F_SACM_SendDAC1;
 		[0x78F1] = R4						
 		r3 = [r2++];						
 		r4 = [R_SACM_Play_Gain];
        push r1,r1 to [sp];	
		r1 = FR;							
		tstb r1, 12;						
		jz ?Fraction_OFF_b			
		fraction off						
?Fraction_OFF_b:						
	    MR = R4 * R3, us;			
		tstb r1, 12;						
		jz ?Fraction_ON_b				
		fraction on							
?Fraction_ON_b:							
		pop r1,r1 from [sp];		
 		R4 = R4 ^ 0x8000;				
 		//call	F_SACM_SendDAC2;
 		[0x78F1] = R4						
		goto L_NoDAC_8K					
.endif									

.ifdef MP3_Used							
L_MP3_DAC:		
   		r3 = [R_Decode_Finish_A]
   		jnz	MP3_DAC_conti?		

		r2 = [R_SACM_DAC_Out_PTR_Play]
		r3 = 8
L_MP3_Send_GoOn?:
		r4 = [R_Right_ChannelData]	
   		[0x78f1] = r4							
   		r4 = [R_Left_ChannelData]	
   		[0x78f1] = r4
   		r3 -= 1
   		jnz L_MP3_Send_GoOn?
   		r2 += 8

		goto L_NoDAC_8K					
   	
MP3_DAC_conti?:   	
   		r5 = FR;							
		push r5 to [sp]
		fraction off						
		r5 = 8	      		
   		R2 = [R_SACM_DAC_Out_PTR_Play];   
 		r1 = [R_SACM_Play_Gain]
loop?:  
    	R3 = [R2++]; 						
    	MR = R1 * R3, us;				

 		R4 = R4 ^ 0x8000;				
 		//call	F_SACM_SendDAC1;
 		[0x78F1] = R4						
 		[R_Right_ChannelData]=r4
 		r3 = [r2++];						
 	    MR = R1 * R3, us;			
 		R4 = R4 ^ 0x8000;					
 		//call	F_SACM_SendDAC2;	
 		[0x78F1] = R4		
 		[R_Left_ChannelData]=r4				
		r5-=1
		jnz	loop?
		pop	r5 from [sp]
		FR=r5
		r1=[R_SACM_Play_Flag]
		goto L_NoDAC_8K				
.endif										


//.ifdef S320TTS_Used
//L_S320TTS_DAC:
//	    R2 = [R_SACM_DAC_Out_PTR_Play];    
//	    R3 = [R2++];
//	    R4 = [R_SACM_Play_Gain];
//	    push r1,r1 to [sp];
//		r1 = FR;
//		tstb r1, 12;
//		jz ?Fraction_OFF
//		fraction off				
//?Fraction_OFF:					
//	    MR = R4 * R3, us;	
//		tstb r1, 12;
//		jz ?Fraction_ON
//		fraction on					
//?Fraction_ON:						
//		pop r1,r1 from [sp];
//		jmp SendDAC_8K
//.endif

.ifdef DVR520_Used
L_DVR520_DAC:
		r3 = [R_Decode_Finish_A]				
   		jnz	DVR520_DAC_conti?					

//send mute data to DAC   		   		
   		r2 = [R_SACM_DAC_Out_PTR_Play]
		r3 = 8
		
L_DVR520_Send_Mute?:
		test R1,C_SACM_ENABLE_DAC1;
	    jz ?L_DAC2Check_DVR520_1;
		r4 = [R_Right_ChannelData]	
   		[0x78f1] = r4	
   		
?L_DAC2Check_DVR520_1: 
		test R1,C_SACM_ENABLE_DAC2; 
		jz 	?L_NoDAC_DVR520							
   		r4 = [R_Left_ChannelData]	
   		[0x78f1] = r4
   							
?L_NoDAC_DVR520:
		r3 -= 1
   		jnz L_DVR520_Send_Mute?
		r2 += 8	
		goto L_NoDAC_8K		
   		 		
DVR520_DAC_conti?:   										
	    R2 = [R_SACM_DAC_Out_PTR_Play];    
	    R5 = 8
Loop_DVR520?:	    
	    R3 = [R2++];
	    R4 = [R_SACM_Play_Gain];
	    push r1,r1 to [sp];
		r1 = FR;
		tstb r1, 12;
		jz ?Fraction_OFF
		fraction off				
?Fraction_OFF:					
	    MR = R4 * R3, us;	

		tstb r1, 12;
		jz ?Fraction_ON
		fraction on					
?Fraction_ON:						
		pop r1,r1 from [sp];	
	    
	    r4 ^= 0x8000;								
		test R1,C_SACM_ENABLE_DAC1;
	    jz ?L_DAC2Check_DVR520;
 		[0x78F1] = R4						
		[R_Right_ChannelData]=r4
		
?L_DAC2Check_DVR520:
		test R1,C_SACM_ENABLE_DAC2;
		jz Skip_DVR520?;
 		//call	F_SACM_SendDAC2;
 		[0x78F1] = R4
 		[R_Left_ChannelData]=r4	
Skip_DVR520?: 		
	    R5 -= 1
	    jnz Loop_DVR520?					
		goto L_NoDAC_8K
.endif	 

.ifdef	S200_Used
L_S200_DAC:
		r3 = [R_Decode_Finish_A]				
   		jnz	S200_DAC_conti?					

//send mute data to DAC   		   		
   		r2 = [R_SACM_DAC_Out_PTR_Play]
		r3 = 8
		
L_S200_Send_Mute?:
		test R1,C_SACM_ENABLE_DAC1;
	    jz ?L_DAC2Check_S200_1;
		r4 = [R_Right_ChannelData]	
   		[0x78f1] = r4	
   		
?L_DAC2Check_S200_1: 
		test R1,C_SACM_ENABLE_DAC2; 
		jz 	?L_NoDAC_S200							
   		r4 = [R_Left_ChannelData]	
   		[0x78f1] = r4
   							
?L_NoDAC_S200:
		r3 -= 1
   		jnz L_S200_Send_Mute?
		r2 += 8	
		goto L_NoDAC_8K				

S200_DAC_conti?:
		R2 = [R_SACM_DAC_Out_PTR_Play];    
	    R3 = [R2++];
	    R4 = [R_SACM_Play_Gain];
	    r5=FR					
		push r5	to [sp]	
		fraction off		
?Fraction_OFF_b:		
	    MR = R4 * R3, us;	
		pop r5 from [sp];
	    FR=r5   	    		
////////////////////////////////////////////////////////////////////////////////

S200_PutData?:
		r4 ^= 0x8000;					
		test R1,C_SACM_ENABLE_DAC1;
	    jz ?L_DAC2Check_S200;
 		//call	F_SACM_SendDAC1;
 		[0x78F1] = R4						
		[R_Right_ChannelData] = r4
		
?L_DAC2Check_S200:
		test R1,C_SACM_ENABLE_DAC2;
		jz Skip_S200?;
 		//call	F_SACM_SendDAC2;	
 		[0x78F1] = R4
 		[R_Left_ChannelData] = r4	
Skip_S200?: 		
 		goto	L_NoDAC_8K					
.endif

.ifdef S530_Used
L_S530_DAC:
		r3 = [R_Decode_Finish_A]				
   		jnz	S530_DAC_conti?					

//send mute data to DAC   		   		
   		r2 = [R_SACM_DAC_Out_PTR_Play]
		r3 = 8
		
L_S530_Send_Mute?:
		test R1,C_SACM_ENABLE_DAC1;
	    jz ?L_DAC2Check_S530_1;
		r4 = [R_Right_ChannelData]	
   		[0x78f1] = r4	
   		
?L_DAC2Check_S530_1: 
		test R1,C_SACM_ENABLE_DAC2; 
		jz 	?L_NoDAC_S530							
   		r4 = [R_Left_ChannelData]	
   		[0x78f1] = r4
   							
?L_NoDAC_S530:
		r3 -= 1
   		jnz L_S530_Send_Mute?
		r2 += 8	
		goto L_NoDAC_8K		

S530_DAC_conti?: 
	    R2 = [R_SACM_DAC_Out_PTR_Play];    
	    R5 = FR
	    push R5 to [sp]
	    fraction off
	    R5 = 4
Loop_S530?:	      
	    R3 = [R2++];
	    R4 = [R_SACM_Play_Gain];
	    
	.ifdef dump_decode_data		
 		push R1, R2 to [SP];
		R1 = [decode_addrh];
		R2 = [decode_addrl];
	
		ds = R1;
		D:[R2++] = R3;
		[decode_addrl] = R2;
		R1 = ds;
		[decode_addrh] = R1; 	
		pop R1, R2 from [SP];
	.endif
	    
	    MR = R4 * R3, us;			
		R4 ^= 0x8000;								
		test R1,C_SACM_ENABLE_DAC1;
	    jz ?L_DAC2Check_S530;
 		//call	F_SACM_SendDAC1;
 		[0x78F1] = R4						
		[R_Right_ChannelData]=r4
		
?L_DAC2Check_S530:
		test R1,C_SACM_ENABLE_DAC2;
		jz ?Skip_S530;
		[0x78F1] = R4
 		[R_Left_ChannelData] = R4
?Skip_S530:
 		//call	F_SACM_SendDAC2;								
		R5 -= 1
		jnz	Loop_S530?
		pop	R5 from [sp]
		FR = R5
		goto L_NoDAC_8K
.endif

L_SACM_DAC: 								//it's means read chase up decode while R_Decode_Finish_A is 0
	 	r3 = [R_Decode_Finish_A]				
   		jnz	SACM_DAC_conti?					

//send mute data to DAC     		   		
   		r2 = [R_SACM_DAC_Out_PTR_Play]
		r3 = 8
		
L_SACM_Send_Mute?:
		test R1,C_SACM_ENABLE_DAC1;
	    jz ?L_DAC2Check_SACM_1;
		r4 = [R_Right_ChannelData]	
   		[0x78f1] = r4	
   		
?L_DAC2Check_SACM_1: 
		test R1,C_SACM_ENABLE_DAC2; 
		jz 	?L_NoDAC_SACM							
   		r4 = [R_Left_ChannelData]	
   		[0x78f1] = r4
   							
?L_NoDAC_SACM:
		r3 -= 1
   		jnz L_SACM_Send_Mute?
		r2 += 8	
		goto L_NoDAC_8K			

SACM_DAC_conti?: 
	    R2 = [R_SACM_DAC_Out_PTR_Play];    
	    r5 = FR
	    push r5 to [sp]		
	    fraction off
	    r5 = 8				
Loop_SACM?:	      					
	    R3 = [R2++];
	   	R4 = [R_SACM_Play_Gain];
	.ifdef dump_decode_data		
 		push r1, r2 to [SP];
		R1 = [decode_addrh];
		r2 = [decode_addrl];
	
		ds = r1;
		D:[R2++] = R3;
		[decode_addrl] = r2;
		r1 = ds;
		[decode_addrh] = r1; 	
		POP R1, R2 from [SP];
	.endif
	    
	    MR = R4 * R3, us;		 									
		r4 ^= 0x8000;								
		test R1,C_SACM_ENABLE_DAC1;
	    jz ?L_DAC2Check_SACM;
 		[0x78F1] = R4						
		[R_Right_ChannelData] = r4
		
?L_DAC2Check_SACM:
		test R1,C_SACM_ENABLE_DAC2;
		jz Skip_SACM?;
 		//call	F_SACM_SendDAC2;
 		[0x78F1] = R4
 		[R_Left_ChannelData]=r4						
		
Skip_SACM?:		
		r5 -= 1
		jnz	Loop_SACM?	
		pop	r5 from [sp]
		FR=r5	
////////////////////////////////////		
			
L_NoDAC_8K:
		test R1, C_SACM_FIQ_EVEN;
		jz ?_Branch_1;
		
		R3 = [R_DAC_OUT_LENGTH];
		R3 += R_SACM_DAC_Out_Buffer;
		cmp R2,R3;
		//jne ?_Branch_2;
		jb ?_Branch_2;
		
		test R1, C_SACM_DECODE_END;
		jz ?_Branch_3;

		//call F_SACM_Stop; 		//del by benwang 20060117
		call R_F_SACM_Stop_Var;     //add by benwang 20060117
		R1 = MSG_SND_END			//added by chengye 2007/4/26
		[MSG_SndType] = R1
		call _Snd_OnPlayEnd			//added by chengye 2007/6/19
		pop R1, R5 from [SP];
		retf;

?_Branch_3:
		r3 = [R_Decode_Finish]
		jnz	switch?	
						
		r2 -= 8					//for MP3 Decode add to 8 words
		
		r3=0
		[R_Decode_Finish_A]=r3
		jmp	?_Branch_2

switch?:
		R1 |= C_SACM_DECODE_WORK;
		R1 ^= C_SACM_FIQ_EVEN;
		[R_SACM_Play_Flag] = R1;
		
		R1 = [Mp3_FrameCount_L]; 
	 	R3 = [Mp3_FrameCount_H]; 
		R1 += 1;				 
		R3 += 0, carry;		
	 	[Mp3_FrameCount_L] = R1; 
		[Mp3_FrameCount_H] = R3;       
		
		r3=1
		[R_Decode_Finish_A]=r3
		
?_Branch_2:
		[R_SACM_DAC_Out_PTR_Play] = R2;	
		
		pop R1, R5 from [SP];	
		retf;
	  
?_Branch_1: 
		R3 = [R_DAC_OUT_LENGTH];			
		R3 = R3 lsl 1;
		R3 += R_SACM_DAC_Out_Buffer;
	 	cmp R2,R3;
		//jne ?_Branch_4;
		jb ?_Branch_4;
	 
		test R1, C_SACM_DECODE_END;
		jz ?_Branch_5;
 	
	    //call F_SACM_Stop; 	//del by benwang 20060117	
	    call R_F_SACM_Stop_Var; //add by benwang 20060117
	    R1 = MSG_SND_END		//added by chengye 2007/4/26
		[MSG_SndType] = R1
		call _Snd_OnPlayEnd		//added by chengye 2007/6/19
		pop R1, R5 from [SP];
	    retf;
	    
?_Branch_5:
		r3=[R_Decode_Finish]
		jnz	switch1?				
		r2 -= 8					
		r3=0
		[R_Decode_Finish_A]=r3
		jmp	?_Branch_4				

switch1?:
		R1 |= C_SACM_DECODE_WORK;
		R1 ^= C_SACM_FIQ_EVEN;	
		[R_SACM_Play_Flag] = R1;
		
		R1 = [Mp3_FrameCount_L];
	 	R3 = [Mp3_FrameCount_H];
		R1 += 1;				 
		R3 += 0, carry;		
	 	[Mp3_FrameCount_L] = R1;
		[Mp3_FrameCount_H] = R3;    
		
		R2 = R_SACM_DAC_Out_Buffer; 
 		r3=1
		[R_Decode_Finish_A]=r3
?_Branch_4:
		[R_SACM_DAC_Out_PTR_Play] = R2;
		
		pop	r1,r5 from [sp]
		retf
.endp 	

//========================================================
// Function Name:	F_ISR_Service_SACM_ADC
// Syntax:		call F_ISR_Service_SACM_ADC; (in assembly domain)
// Purpose: 	get AD data                 
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R5
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public F_ISR_Service_SACM_ADC
F_ISR_Service_SACM_ADC:		.proc
	 	push R1, R5 to [SP];
	 
	 	R1 = [R_SACM_Play_Flag];
	    test R1, C_SACM_FIQ_SOUND; 
	 	jne ?_Branch_0;
		 
	  	pop R1, R5 from [SP];
	  	retf;

?_Branch_0:				
		r4 = 8											
		R2 = [R_SACM_ADC_In_PTR_Rec];       
?L_REC_NoMon:
		//call F_SACM_GetADC;		//return r3  //del by benwang 20060117
	    R3 = [0x7964]; //add by benwang 20060117
	//	R3 ^= 0x8000;
	
	.ifdef dump_encode_data
 		push r1, r2 to [SP];
		R1 = [encode_addrh];
		r2 = [encode_addrl];
	
		ds = r1;
		R3 = D:[R2++];
//		D:[R2++] = R3;
		[encode_addrl] = r2;
		r1 = ds;
		[encode_addrh] = r1; 	
		POP R1, R2 from [SP];
	.endif	 
	    
	    [R2++] = R3;
		r4-=1
		jnz	?L_REC_NoMon

		test R1, C_SACM_FIQ_EVEN;
		jz ?_Branch_1;
	
?_Continue_0:	
		r3=[R_ADC_IN_LENGTH]
		R3 += R_SACM_ADC_In_Buffer
		cmp R2, r3
		jne ?_Branch_2;
	
		test R1, C_SACM_DECODE_END;
		jz ?_Branch_3;
	
		//call F_SACM_Stop;     //del by benwang 20060117
		call R_F_SACM_Stop_Var; //add by benwang 20060117
		R1 = MSG_SND_END		//added by chengye 2007/4/26
		[MSG_SndType] = R1
		pop R1, R5 from [SP];
		retf; 
		
?_Branch_3:
		R1 |= C_SACM_DECODE_WORK;
		R1 ^= C_SACM_FIQ_EVEN;
		[R_SACM_Play_Flag] = R1;

?_Branch_2:
		[R_SACM_ADC_In_PTR_Rec] = R2;	
		pop R1, R5 from [SP];
		retf; 
	  
?_Branch_1: 
		R3 = [R_ADC_IN_LENGTH];
		R3 = R3 LSL 1;
		R3 += R_SACM_ADC_In_Buffer;
		CMP R2, R3; 
	 	jne ?_Branch_4;
	 
		test R1, C_SACM_DECODE_END;
		jz ?_Branch_5;
	
		//call F_SACM_Stop;		//del by benwang 20060117	
		call R_F_SACM_Stop_Var; //add by benwang 20060117
		R1 = MSG_SND_END		//added by chengye 2007/4/26
		[MSG_SndType] = R1
		pop R1, R5 from [SP];
		retf; 

?_Branch_5:
		R1 |= C_SACM_DECODE_WORK;
		R1 ^= C_SACM_FIQ_EVEN;	
		[R_SACM_Play_Flag] = R1;
		R2 = R_SACM_ADC_In_Buffer; 
 
?_Branch_4:
		[R_SACM_ADC_In_PTR_Rec] = R2;
		pop R1, R5 from [SP];
		retf; 
.endp

////////////////////////////////////////////////////////////////////////

.external _MP3_Get_BR_FS_Psuedo
.external _R_SampleRate
.external _R_MPEGBitRate
.external _gCurSecondTime;

//========================================================
// Function Name:	SACM_MP3_SetFS
// Syntax:		void SACM_MP3_SetFS(void);
// Purpose: 	search MP3 first valid frame                 
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R5
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
//Get the SampleRate and the BitRate
.ifdef MP3_Used
.public _SACM_MP3_SetFS, F_SACM_MP3_SetFS
_SACM_MP3_SetFS: .proc
F_SACM_MP3_SetFS:
		push r1,r5 to [sp]
		call F_SACM_Decode_Initial_BS;
		R1 = 1024;
		[R_DECODE_IN_LENGTH] = R1;		
		//R1 = [R_DECODE_IN_LENGTH]; 						 //add by benwang 20060209
		R1 += R_SACM_Decode_In_Buffer; 						 //add by benwang 20060209
		[R_SACM_Decode_In_PTR] = R1;
	    call  F_SACM_Decode_Initial_Process;
//-----------------------------------------------------------
	    R1 = 1024
		call F_GetSyncWord_Init;
		R1 = 0
		[R_SACM_DECODE_IN_LENGTH] = R1
		R1 = 0
Dec_Loop?:
		push R1 to [sp]
		
	 	R1 = [R_SACM_Decode_Count_L];
	 	R2 = [R_SACM_Decode_Count_H];
		R1 += [R_SACM_DECODE_IN_LENGTH];
		R2 += 0, carry;
	 	[R_SACM_Decode_Count_L] = R1;
		[R_SACM_Decode_Count_H] = R2;
	 	R3 = [R_SACM_Decode_Length_L];
	 	R4 = [R_SACM_Decode_Length_H];
		CMP R2,R4;
	    JB _Branch_0?;
	    CMP R1,R3;
	    JB _Branch_0?;

		pop r1 from [sp]
Dec_Error?:
		pop	r1,r5 from [sp]
		R1 = 1; //Get_BR_FS fail
		goto _End?;

_Branch_0?:
		call F_SACM_System_Get_BS_Manual; //must call get_BS before _MP3_Get_BR_FS_Psuedo 20060118
		cmp r1, -1						  //r1==-1 means read error
		jnz ?L_Continue
		pop r1 from [sp]
		jmp Dec_Error?
?L_Continue:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;		
.if 0
		/////////////////////////////
		push r3,r4 to [sp]
		r3=0x24
		r4=0x01
		call mr
		pop r4,r5 from [sp]
		//call _MP3_Get_BR_FS_Psuedo; //add by benwang to test this function //20060118
									//	Output:	r1--0(success) or 1(fail)
									//			r2--sampling rate(in Hz)
									//			r3--bitrate(in Kbps)
									//	Reg used:
									//	Note:If a valid header can't be found in one dec_in_buf,then return error
		////////////////////////////////////////
//		r4 = [CFrame_layer]
		r4 = [0x1ec1];
		cmp r4, 2
		jne ?_layer_3;
		r4 = 2304;
		jmp ?_end;
?_layer_3:
		r4 = 1152;
?_end:
		[R_DAC_OUT_LENGTH] = r4;

		R4 = [R_SACM_Decode_In_PTR];
		R4 -= [R_SACM_DECODE_IN_LENGTH];
		[R_SACM_DECODE_IN_LENGTH] = R4;

		cmp R1, 0
		jne Dec_Loop?;
.else
		call F_GetSyncWord

		push R4 to [sp]
		R4 = [R_SACM_Decode_In_PTR];
		R4 -= [R_SACM_DECODE_IN_LENGTH];
		[R_SACM_DECODE_IN_LENGTH] = R4;
		pop R4 from [sp]

		cmp R1, 1//0
		je Dec_NotLoop?
		pop R1 from [sp]
		R1 += 1
		[ErrorFrame_Num] = R1
		cmp R1, 1000
		jae Dec_Error?
		jmp Dec_Loop?
//		jne Dec_Loop?;
Dec_NotLoop?:
		pop r1 from [sp]
		[0x1ec1] = r4;
//		r4 = [0x1ec1];
		cmp r4, 2							//r4 return the value of layer
		jne ?_layer_3;
		r4 = 2304;
		jmp ?_end;
?_layer_3:
		r4 = 1152;
?_end:
		[R_DAC_OUT_LENGTH] = r4;
.endif
		r4=[R_SACM_Decode_Count_L]
		[_ulFrame_Count_MP3] = r4
		[R_MP3_Decode_Count_L]=r4
		r4=[R_SACM_Decode_Count_H]
		[_ulFrame_Count_MP3 + 1] = r4
		[R_MP3_Decode_Count_H]=r4

		[MP3_FS] = R2;
		[MP3_BR] = R3;
		[_R_SampleRate] = R2			//xiaolei add for MP3 SampleRate & BitRate
		[_R_MPEGBitRate] = R3

		pop	r1,r5 from [sp]
		R1 = 0; //Get_BR_FS success
_End?:
		retf;
.endp
.endif //MP3_Used
//========================================================
// Function Name:	F_SACM_MP3_Get_Time
// Syntax:		void SACM_MP3_SetFS(void);
// Purpose: 	get MP3 play Time, The unit is second                 
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public F_SACM_MP3_Get_Time
.public _SACM_MP3_Get_Time
F_SACM_MP3_Get_Time:.proc
_SACM_MP3_Get_Time:
	push R1, R4 to [sp]
	R1 = [Mp3_FrameCount_L];
	R2 = [R_DAC_OUT_LENGTH];
	R2 = R2 asr 1;				//MP3播放时为双声道, 所以要除以2.
	MR = R1 * R2,uu;
	[Temp1] = R4;
	[Temp2] = R3; //Sample_L

	R1 = [Mp3_FrameCount_H];
	MR = R1 * R2,uu;
	R3 += [Temp1];
	R4 += 0, carry;
	[Temp1] = R3; //Sample_H

	r1 = [MP3_FS];           //sampling rate(Hz)	
	cmp R1, 44100;
	jne ?_Endof44100;
	R1 = 0;
	jmp ?_End;
?_Endof44100:
	cmp R1, 48000;
	jne ?_Endof48000;
	R1 = 1;
	jmp ?_End;
?_Endof48000:
	cmp R1, 32000;
	jne ?_Endof32000;
	R1 = 2;
	jmp ?_End;
?_Endof32000:
	cmp R1, 22050;
	jne ?_Endof22050;
	R1 = 3;
	jmp ?_End;
?_Endof22050:
	cmp R1, 24000;
	jne ?_Endof24000;
	R1 = 4;
	jmp ?_End;
?_Endof24000:
	cmp R1, 16000;
	jne ?_Endof16000;
	R1 = 5;
	jmp ?_End;
?_Endof16000:
	cmp R1, 11025;
	jne ?_Endof11025;
	R1 = 6;
	jmp ?_End;
?_Endof11025:
	cmp R1, 12000;
	jne ?_Endof12000;
	R1 = 7;
	jmp ?_End;
?_Endof12000:
	cmp R1, 8000;
	jne ?_End;
	R1 = 8;
?_End:
	R2 = seg T_MP3Fs_Table
	DS = R2
	R1 += offset T_MP3Fs_Table;
	R1 = DS:[R1];

	R2 = [Temp2]; //Sample_L
	MR = R2 * R1, uu;
	[Temp3] = R4;
	//[Temp4] = R3;
	R2 = [Temp1]; //Sample_H
	MR = R2 * R1, uu;
	R3 += [Temp3];
	R4 += 0, carry;
	
	R1 = 11;
	R3 = R3 LSR R1;
	MR |= R4 ASR R1;
	[MP3_TimeInfo] = r3;				//The Play Time, The unit is second
	[_gCurSecondTime] = r3
	pop R1, R4 from [sp]
	retf;
//20060605
.endp		//20060525
//20060525

//=============================For A128 Play Used==========================
.ifdef A128_Used
.external _R_BitRate_A128

.public _SACM_A128_SetFS
_SACM_A128_SetFS: .proc
		push r1,r5 to [sp]
		call F_SACM_Decode_Initial_BS;
		R1 = 1024;
		[R_DECODE_IN_LENGTH] = R1;
		//R1 = [R_DECODE_IN_LENGTH]; 							 //add by benwang 20060209
		R1 += R_SACM_Decode_In_Buffer; 						 //add by benwang 20060209
		[R_SACM_Decode_In_PTR] = R1;		
	    call  F_SACM_Decode_Initial_Process;
Dec_Loop?:
	 	R1 = [R_SACM_Decode_Count_L];
	 	R2 = [R_SACM_Decode_Count_H];
		R1 += [R_SACM_DECODE_IN_LENGTH];
		R2 += 0, carry;
	 	[R_SACM_Decode_Count_L] = R1;
		[R_SACM_Decode_Count_H] = R2;
	 	R3 = [R_SACM_Decode_Length_L];
	 	R4 = [R_SACM_Decode_Length_H];
		CMP R2,R4;
	    JB _Branch_0?;
	    CMP R1,R3;
	    JB _Branch_0?;

		R1 = 1; //Get_BR_FS fail
		jmp _End?;

_Branch_0?:
		call F_SACM_System_Get_BS_Manual; //must call get_BS before _MP3_Get_BR_FS_Psuedo 20060118
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;

		/////////////////////////////
		push r3,r4 to [sp]
		r3=0x24
		r4=0x01
		call mr
		pop r4,r5 from [sp]
		//call _MP3_Get_BR_FS_Psuedo; //add by benwang to test this function //20060118
									//	Output:	r1--0(success) or 1(fail)
									//			r2--sampling rate(in Hz)
									//			r3--bitrate(in Kbps)
									//	Reg used:
									//	Note:If a valid header can't be found in one dec_in_buf,then return error
		////////////////////////////////////////
//		r4 = [CFrame_layer]
		r4 = [0x1ec1];
		cmp r4,2
		jne ?_layer_3;
		r4 = 2304;
		jmp ?_end;
?_layer_3:
		r4 = 1152;
?_end:
		[R_DAC_OUT_LENGTH] = r4;

		R4 = [R_SACM_Decode_In_PTR];
		R4 -= [R_SACM_DECODE_IN_LENGTH];
		[R_SACM_DECODE_IN_LENGTH] = R4;

		cmp R1, 0
		jne Dec_Loop?;

		r4=[R_SACM_Decode_Count_L]
		[R_MP3_Decode_Count_L]=r4
		r4=[R_SACM_Decode_Count_H]
		[R_MP3_Decode_Count_H]=r4
				
//		[MP3_FS] = R2;
		[_R_BitRate_A128] = R3;
		
		R1 = 0; //Get_BR_FS success
_End?:
		pop	r1,r5 from [sp]
		retf;
.endp

.external _gCurA128SecondTime
.external R_A128_Sf_ind

.public _SACM_A128_Get_Time
_SACM_A128_Get_Time:
	push R1, R4 to [sp]
	R1 = [Mp3_FrameCount_L];
	R2 = 1152;
	R2 = R2 asr 1;				//MP3播放时为双声道, 所以要除以2.
	MR = R1 * R2,uu;
	[Temp1] = R4;
	[Temp2] = R3; //Sample_L

	R1 = [Mp3_FrameCount_H];
	MR = R1 * R2,uu;
	R3 += [Temp1];
	R4 += 0, carry;
	[Temp1] = R3; //Sample_H

	r1 = [R_A128_Sf_ind];           //sampling rate(Hz)	

	R2 = seg T_MP3Fs_Table
	DS = R2
	R1 += offset T_MP3Fs_Table;
	R1 = DS:[R1];

	R2 = [Temp2]; //Sample_L
	MR = R2 * R1, uu;
	[Temp3] = R4;
	//[Temp4] = R3;
	R2 = [Temp1]; //Sample_H
	MR = R2 * R1, uu;
	R3 += [Temp3];
	R4 += 0, carry;
	
	R1 = 11;
	R3 = R3 LSR R1;
	MR |= R4 ASR R1;
	[_gCurA128SecondTime] = r3
	pop R1, R4 from [sp]
	retf;
.endif	//end of A128_Used	
//=============================For A128 Play Used==========================

.public _GetMP3FrameCount
_GetMP3FrameCount:
	r1 = [Mp3_FrameCount_L]
	r2 = [Mp3_FrameCount_H]
	retf

// 2007.1.3 Jacky Lu
// void ETTS_NotifyBusy(short b)
.public _ETTS_NotifyBusyFcn
_ETTS_NotifyBusyFcn: .PROC
	push r1,r2 to [sp]
	R1 = SP + 5
	R1 = [R1]			//r1=1, decoding. r1=0 free 
	jz ?NotBusy
		
	R1 = [R_SACM_Play_Flag]
	R1 |=  C_SACM_PAUSE;	
	R1 &= ~C_SACM_FIQ_SOUND
	[R_SACM_Play_Flag] = R1
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//%%%       Clear Disable Channel FIFO Empty Interupt				  %
	//%%%       Latch FIFO data to DAC/PWM after writing to control reg   %
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	r1 = [R_SACM_Play_Flag]			                      				//%
	test r1,C_SACM_ENABLE_DAC1								  			//%
	jz L_EndDACA?											  	  		//%
	call	F_SACM_CHA_PlayInterrupt_Disable						//%
	L_EndDACA?:															//%
	r1 = [R_SACM_Play_Flag]								  				//%
	test r1,C_SACM_ENABLE_DAC2							  	  			//%
	jz L_EndDACB?											  	  		//%
	call	F_SACM_CHB_PlayInterrupt_Disable						//%
	L_EndDACB?:															//%
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	jmp ?EndOf_CheckBusy

?NotBusy:
	R1 = [R_SACM_Play_Flag]
	R1 &= ~C_SACM_PAUSE;
	R1 |=  C_SACM_FIQ_SOUND;
	[R_SACM_Play_Flag] = R1;
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	r1 = [R_SACM_Play_Flag]			                          			//%
	test r1,C_SACM_ENABLE_DAC1								  			//%
	jz L_EndDACA1?											  	  		//%
	call	F_SACM_CHA_PlayInterrupt_Enable						  		//%
	L_EndDACA1?:														//%
	r1 = [R_SACM_Play_Flag]								  				//%
	test r1,C_SACM_ENABLE_DAC2								  			//%
	jz L_EndDACB1?											  	  		//%
	call	F_SACM_CHB_PlayInterrupt_Enable						  		//%
	L_EndDACB1?:														//%
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%		

?EndOf_CheckBusy:
	pop	r1,r2 from [sp]
	retf
.ENDP
// 2007.1.3 Jacky Lu


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//here code for isr

//========================================================
// Function Name:	ExecuteIF_AUDIOA
// Syntax:		void ExecuteIF_AUDIOA(void);
// Purpose: 	send decoded data to DAC                
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public ExecuteIF_AUDIOA
ExecuteIF_AUDIOA:
	r1 = [P_CHA_Ctrl];
	[P_CHA_Ctrl] = r1;			//Clear Audio CHA FIFO Empty Flag
	call	F_ISR_Service_SACM_DAC;
	
	R1 = [R_SACM_Play_Flag];
	test	r1, C_SACM_DECODE_WORK
	jz ?L_GetWriteData
				
	r1 = [R_SACM_ISR_Flag]
//	jz ?L_GetWriteData
	jz L_NoDACINT?
	
	r1 = 0
	[R_SACM_ISR_Flag] = r1		//remove here for avoiding sp overflow by chengye 2007/10/29
	sp += 2						//破坏CALL ExecuteIF_AUDIOA的pc和sr
	R1 = OFFSET L_ForBackgroundUsage
	push R1	to [SP];			//PUSH PC
	R1 = SR & (~0x003F)
	R1 = R1 | SEG L_ForBackgroundUsage
	push R1	to [SP];			//PUSH CS

	RETI						//JUMP to L_ForBackgroundUsage

?L_GetWriteData:				//When Decode or Encode is not busy, then run the data processing
//----------------------------------------------------------	
//added by chengye 2007/8/20	
	R1 = [R_CODEC_TYPE]
	cmp R1, C_CODEC_MP3
	jnz L_NoDACINT?
//----------------------------------------------------------
	r1 = [R_DataMode]
	jz L_NoDACINT?				//added by chengye 2007/7/2.when get data from memory,exit
	
	r1 =[_uiMP3WriteFlag]
	test r1 ,0x0003
	jz L_NoDACINT?
	r1 = [_R_ISR_DataProc_Flag]
	jz L_NoDACINT?
	
	r1 = 0
	[_R_ISR_DataProc_Flag] = r1	//R_SACM_XXXX_ISR_Flag = 0	
	
	sp += 2						//破坏CALL ExecuteIF_AUDIOA的pc和sr
	r1 = 0
	[_R_ISR_DataProc_Flag] = r1	//_R_ISR_DataProc_Flag = 0

	R1 = OFFSET L_BackgroundGetDataProcess
	push R1	to [SP];			// PUSH PC
	R1 = SR & (~0x003F)
	R1 = R1 | SEG L_BackgroundGetDataProcess
	push R1	to [SP];			// PUSH CS

	RETI						// JUMP to L_BackgroundDataProcess
L_NoDACINT?:
	RETF


//========================================================
// Function Name:	ExecuteIF_AUDIOB
// Syntax:		void ExecuteIF_AUDIOB(void);
// Purpose: 	send decoded data to DAC                
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================	
.public ExecuteIF_AUDIOB
ExecuteIF_AUDIOB:
	RETF


//========================================================
// Function Name:	ExecuteIF_ADC_AUTO
// Syntax:		void ExecuteIF_ADC_AUTO(void);
// Purpose: 	get data from AD                
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================	
.public ExecuteIF_ADC_AUTO
ExecuteIF_ADC_AUTO:
	r1 = [P_INT_Status1]
	test	r1,C_INT_ADCAutoSample
	jz	L_EndAutoSample?
	
	call	F_ISR_Service_SACM_ADC
	r1=[P_ASADC_Ctrl]			//clear flag
	[P_ASADC_Ctrl]=r1
	
	R1 = [R_SACM_Play_Flag];
	test	r1,C_SACM_DECODE_WORK
	jz ?L_GetWriteData
			
	r1 = [R_SACM_ISR_Flag]
	jz L_NoADCINT?
	
	r1 = 0
	[R_SACM_ISR_Flag] = r1		//remove here for avoiding sp overflow by chengye 2007/10/29
	
	sp += 2						//破坏CALL ExecuteIF_ADC_AUTO的pc和sr
	R1 = OFFSET L_ForBackgroundUsage
	push R1	to [SP];			// PUSH PC
	R1 = SR & (~0x003F)
	R1 = R1 | SEG L_ForBackgroundUsage
	push R1	to [SP];			// PUSH CS
	
	RETI						// JUMP to L_ForBackgroundUsage
	
?L_GetWriteData:				//When Decode or Encode is not busy, then run the data processing
	r1 = [R_DataMode]
	jz L_NoADCINT?				//added by chengye 2007/7/2.when get data from memory,exit
	
	r1 =[_uiRecWriteFlag]
	test r1 ,0x0003				//check record bufferA or bufferB full
	jz L_NoADCINT?
	
	r1 = [_R_ISR_DataProc_Flag]
	jz L_NoADCINT?
	
	r1 = 0
	[_R_ISR_DataProc_Flag] = r1	// R_SACM_XXXX_ISR_Flag = 0	
	
	sp += 2				//破坏CALL ExecuteIF_ADC_AUTO的pc和sr
	R1 = OFFSET L_BackgroundWriteDataProcess
	push R1	to [SP];			// PUSH PC
	R1 = SR & (~0x003F)
	R1 = R1 | SEG L_BackgroundWriteDataProcess
	push R1	to [SP];			// PUSH CS

	RETI	
	
L_EndAutoSample?:
L_NoADCINT?:	
	RETF
	
//*********************************************************************
//added by chengye for Sound module 2007/4/19
L_ForBackgroundUsage:
	r1=r1 rol 4
	[_R_ShiftStore]=r1			//Save 4-bit Shift Buffer
	
	////////////////////////////////////////////////////////////
	r1=fr 
	push r1 to [sp]
	r1=[R_SACM_FR_Flag]
	fr=r1

	call F_SACM_ServiceLoop		//SACM Service Loop

	r1=fr
	[R_SACM_FR_Flag]=r1
	pop	r1 from [sp]
	fr=r1
	////////////////////////////////////////////////////////////
	
	r1 = 1
	[R_SACM_ISR_Flag] = r1		//R_SACM_XXXX_ISR_Flag = 1
	r1=[_R_ShiftStore]
	r1=r1 lsr 4					// Retore 4-bit Shift Buffer
L_InService?:
	pop r8, r15 from [sp]				
	pop r1, r5 from [sp]
	RETF
		
L_BackgroundGetDataProcess:	
	call _Nand_GetFAT_MP3		// SACM Data Process Service Loop
	r1 = 1
	[_R_ISR_DataProc_Flag] = r1	// R_SACM_XXXX_ISR_Flag = 1	
	pop r8, r15 from [sp]				
	pop r1, r5 from [sp]
	RETF						//zhouli add 20070427
		
L_BackgroundWriteDataProcess:	
	call _Nand_WriteFAT_Rec	
	r1 = 1
	[_R_ISR_DataProc_Flag] = r1	// R_SACM_XXXX_ISR_Flag = 1	
	pop r8, r15 from [sp]				
	pop r1, r5 from [sp]
	RETF						//modify by chengye 2007/7/3		
//*********************************************************************	

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



T_MP3Fs_Table:
	.dw	0x0be3 // 1/44100 * 2^(12) * 2^(15)
	.dw	0x0aec // 1/48000 * 2^(12) * 2^(15)
	.dw	0x1062 // 1/32000 * 2^(12) * 2^(15)
	.dw	0x17c6 // 1/22050 * 2^(12) * 2^(15)
	.dw	0x15D8 // 1/24000 * 2^(12) * 2^(15)
	.dw	0x20c4 // 1/16000 * 2^(12) * 2^(15)
	.dw	0x2f80 // 1/11025 * 2^(12) * 2^(15)
	.dw	0x2BB0 // 1/12000 * 2^(12) * 2^(15)
	.dw	0x4189 // 1/8000  * 2^(12) * 2^(15)
