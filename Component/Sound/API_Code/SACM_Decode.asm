//==========================================================================
//SACM_XXX_Decode.asm:  This file includes Kernel . 
//                   It is to be prepared by design deptartment.  
//     XXX: name of Algorism   
// Author:  Arthur, SamSung, 
// Date: 2003/01/28   V1.0 : Template release        
//
// Note:C_DECODE_OUT_LENGTH
// 1. This template is made to facilitate the collaboration between RD5 and SA.
// 2. Consistency in programming style can benefit both parties and shorten 
//    the developement lead time.
// 3. Modulization can bring better integration and promote better collaboration.
// 4. Robustness design is convenient for users to adopt this algorithm.
// 5. Efficiency and Performance in coding style will enhance the core competence 
//    of this algorithm.
//==========================================================================
.include .\Component\Include\speech\algorithm.inc
.include .\Component\Include\speech\SACM_Constant.inc  							// for flag / buffer constants
.include .\Component\Include\speech\SACM_GV.inc
//.include Kernal_lib.inc	
.include .\Component\Include\speech\SACM_Speed.inc	

.public F_SACM_System_Put_BS_Manual

.public F_SACM_Decode_Initial_DAC
.public F_SACM_Decode_Initial_BS
//.public F_SACM_XXXX_Decode_Get_BS_Auto
.public F_SACM_System_Get_BS_Manual
.public F_SACM_Decode_Initial_Process
.public F_SACM_Decode_Process

//////////////////////////////////////////////////          			
.ifdef MS01_Used
	.external MS01_Dec_Init
	.external MS01_DEC_PROC
.endif

.external A128_DEC_init_Psuedo	   //add by benwang 20060221	
.external A128_DEC_PROC_Psuedo     //add by benwang 20060221	
////////////////////////////////////////////////////

.external F_USER_GetData     							// in sacm_DVR1600_user.asm
.external F_USER_WriteData
.external F_SACM_XXXX_EndPlay
.external F_SACM_DVR1800_BITRATE //add by benwang 20060621

.external SubFrame_NUM  //for S600 by zgq on 20050728


.external R_SACM_Resouce_BS, R_SACM_Resouce_DS //added by zgq on 20051215
.external R_DataMode

.ifdef A6400S_Used	 //added by benwang 20060104
.external R_SACM_A6400S_Resource_END_BS, R_SACM_A6400S_Resource_END_DS	//only for A6400S added by zgq on 20051215
.external R_SACM_A6400S_Resource_BS, R_SACM_A6400S_Resource_DS //only for A6400S added by benwang 20060104
.external F_SP_SACM_SetSampleRate			//only for A6400S added by zgq on 20051215
.endif  //added by benwang 20060104

.ifdef MP3_Used	 //added by benwang 20060111
//.external R_SACM_MP3_Resource_END_BS, R_SACM_MP3_Resource_END_DS	//added by benwang 20060111
//.external R_SACM_MP3_Resource_BS, R_SACM_MP3_Resource_DS //added by benwang 20060111
.external	R_SACM_MP3_Length_L, R_SACM_MP3_Length_H		//add by jacky 2006.02.14
.external	R_SACM_A128_Length_L, R_SACM_A128_Length_H		//add by jacky 2006.02.12

.external F_SP_SACM_MP3_SetSampleRate			//added by benwang 20060111
.external	r_layer3_granule,r_layer3_gr	//<< update stanwang,2005.09.22 >> //add by benwang 20060209
//.external	CFrame_layer					//<< update stanwang,2005.11.17 >> //add by benwang 20060209
.external _r_DEC_PROC_pcm_ready			//<< update stanwang,2006.01.23 >>     //add by benwang 20060209
.ifdef MP3_FATREADBUFFER
	.external _MP3DecodeErrorFlag;		//Add 2007/01/12 for MP3 data is wrong during MP3 Decoding
.endif	//End of MP3_FATREADBUFFER definition

.external F_GetSyncWord_Init,F_GetSyncWord		//added by chengye 2007/7/16

.endif	//End of MP3_Used definition

.ifndef C_2002_CODE_ICA
.else
	.external Mp3_FrameCount_L 	//20060525
	.external Mp3_FrameCount_H	//20060525
	.external MP3_TimeInfo      //20060525
.endif  //added by benwang 20060111

.public F_SACM_MP3_System_Get_BS_Manual //add by benwang 20060111 20060113

.ifdef A128_Used	 
.external R_SACM_A128_Resource_END_BS, R_SACM_A128_Resource_END_DS	
.external R_SACM_A128_Resource_BS, R_SACM_A128_Resource_DS 
.external F_SP_SACM_A128_SetSampleRate
.external	r_layer3_granule,r_layer3_gr	
.external	CFrame_layer					
.external _r_DEC_PROC_pcm_ready			
.endif  

.ifdef DVR520_Used
	.external F_DVR520_Dec_Init
	.external F_DVR520_Enc_Init
	.external F_DVR520_Encode
	.external F_DVR520_Decode
.endif

.ifdef AUDIO3200_Used
	.external DVR3200_Play_Init
	.external DVR3200_Play_Proc
	
	.external DVR3200_Rec_Init
	.external DVR3200_Rec_Endp
	.external DVR3200_Rec_Proc
.endif

.ifdef ETTS_Used
	.external _ETTS_DecProc
.endif

.ifdef S320_Used						//added by chengye 2007/4/25
//	.external R_S320_SubFrameCounter
//	.external S320_DEC_INIT
//	.external S320_DEC_PROC
.endif

.external _R_DataEmptyFlag			//记录得到数据的外部Buffer是否为空的标志位, 根据GetData的返回值判断
.external _R_SACM_DecodeData_In_PTR	//记录在Decode_In_Buffer里面可用数据到达的位置
.external _R_MP3_Error_Count		//记录解码过程中解码错误的次数
//.external SubFrameNO	



//.text
//.CODE // use "CODE" instead of "TEXT"
SACM_DECODE_ROM_BLOCK:  .SECTION        .CODE
//..............................................................................


F_SACM_Decode_Initial_DAC: .proc
	    //
		// move to system.asm and replaced with function call  by arthur      
	   	retf;

.endp	   

//========================================================
// Function Name:	F_SACM_Encode_Put_BS_Auto
// Syntax:		call F_SACM_Encode_Put_BS_Auto; (in assembly domain)
// Purpose: 	this function is empty now                 
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//======================================================== 
F_SACM_Encode_Put_BS_Auto: .proc
	
 		retf;
.endp

//****************************************************************
// Function: F_SACM_Decode_Initial_BS   
// Decription: Initialize bit stream for decoding     
// Input: N/A
// Output: N/A
// Registers used : N/A
// Syntax:
// 	call F_SACM_Decode_Initial_BSs; (in assembly domain)
//**************************************************************** 
F_SACM_Decode_Initial_BS: .proc
		r1=sr&0x03f
		ds = r1
		R1 = [R_CODEC_TYPE];
		r1 += T_SACM_Res_Length;
		pc = ds:[r1];
L_MIDI_Length:
	    R2 = 0xFFFF;
		[R_SACM_Decode_Length_L] = R2;
		R2 = 0x7FFF;
		[R_SACM_Decode_Length_H] = R2;
	    //jmp _Continue_0; //del by benwang 20060221 
		goto _Continue_0;  //add by benwang 20060221
		
.ifdef	S200_Used
L_S200_Length:
		R1 = [R_SACM_Decode_In_PTR];
		R2 = [R1++];
		R3 = [R1++];
		R3 = R3 LSR 1;
		R4 = R3 LSR 3;
		R2 = R2 ROR 1; 
		[R_SACM_Decode_Length_L] = R2;
		[R_SACM_Decode_Length_H] = R3;
		R2 = [R1++];
		R2 -= 1;
		///////////		//jacky 0727
		[0xb73]=r2
		//[MODE] = R2;
	///
		R2 = [R1++];
	////			//jacky 0512
//		[0x74]=r2
		[0xb84]=r2	//modify by chengye 2007/6/21
//		[decvar_bit_pack_buffer] = R2;
    ////	
    	[R_SACM_Decode_In_PTR] = R1;    
 		R1 = 0x0000;
 		[R_SACM_Decode_Count_L] = R1;
 		R1 = 0x0000;
 		[R_SACM_Decode_Count_H] = R1; 
		retf;
.endif  

.ifdef A6400S_Used							//added by zgq on 20051215
L_A6400S_Length:							//added by zgq on 20051215
		r2 = [R_SACM_A6400S_Resource_END_BS];//added by zgq on 20051215
		r1 = [R_SACM_A6400S_Resource_END_DS];//added by zgq on 20051215
		//r4 = [R_SACM_Resouce_BS];			//added by zgq on 20051215 //del by benwang 20060104
		r4 = [R_SACM_A6400S_Resource_BS];   //add by benwang 20060104
		//r3 = [R_SACM_Resouce_DS];			//added by zgq on 20051215
		r3 = [R_SACM_A6400S_Resource_DS];   //add by benwang 20060104
		r2 -= r4;							//added by zgq on 20051215
		r1 -= r3,carry;						//added by zgq on 20051215
		[R_SACM_Decode_Length_L] = R2;		//added by zgq on 20051215
		[R_SACM_Decode_Length_H] = R1;		//added by zgq on 20051215
	 	R1 = 0x0000;						//added by zgq on 20051215
	 	[R_SACM_Decode_Count_L] = R1;		//added by zgq on 20051215
	 	R1 = 0x000;							//added by zgq on 20051215
	 	[R_SACM_Decode_Count_H] = R1; 		//added by zgq on 20051215	
	retf;									//added by zgq on 20051215
.endif										//added by zgq on 20051215


.ifdef MP3_Used								//added by benwang 20060111
L_MP3_Length:								//added by benwang 20060111
		r1=[R_SACM_MP3_Length_L]
		[R_SACM_Decode_Length_L] = R1;		//added by benwang 20060111
		r1=[R_SACM_MP3_Length_H]
		[R_SACM_Decode_Length_H] = R1;		//added by benwang 20060111

	 	R1 = 0x0000;						//added by benwang 20060111
	 	[R_SACM_Decode_Count_L] = R1;		//added by benwang 20060111
	 	R1 = 0x000;							//added by benwang 20060111
	 	[R_SACM_Decode_Count_H] = R1; 		//added by benwang 20060111
	 		
	retf;								//added by benwang 20060111
.endif										//added by benwang 20060111

.ifdef A128_Used							//added by benwang 20060221
L_A128_Length:								//added by benwang 20060221
		r1=[R_SACM_A128_Length_L]
		[R_SACM_Decode_Length_L] = R1;		//added by benwang 20060221
		r1=[R_SACM_A128_Length_H]
		[R_SACM_Decode_Length_H] = R1;		//added by benwang 20060221
	 	R1 = 0x0000;						//added by benwang 20060221
	 	[R_SACM_Decode_Count_L] = R1;		//added by benwang 20060221
	 	R1 = 0x000;							//added by benwang 20060221
	 	[R_SACM_Decode_Count_H] = R1; 		//added by benwang 20060221
	retf;									//added by benwang 20060221
.endif										//added by benwang 20060221
//-----------------------------------------------------------------
.ifdef WAV_Used							//added by chengye 2007/7/9
L_WAV_Length:
	 	R1 = 0x0000;
	 	[R_SACM_Decode_Count_L] = R1;
	 	R1 = 0x000;
	 	[R_SACM_Decode_Count_H] = R1;
	 	retf;
.endif
//-----------------------------------------------------------------	

.comment @	//20060629
.ifdef DVR1800_Used
L_DVR1800_Length:                          //add by benwang 20060621
		R1 = [R_SACM_Decode_In_PTR];
		R3 = [R1++];
		R4 = [R1++];
		R4 = R4 LSR 1;
		R2 = R4 LSR 3;
		R3 = R3 ROR 1;	
		[R_SACM_Decode_Length_L] = R3;
		[R_SACM_Decode_Length_H] = R4;
		R3 = [R1++];		
		R4 = [R1++];
		call F_SACM_DVR1800_BITRATE  //for dvr1800
	  	[R_SACM_Decode_In_PTR] = R1; 
	    	jmp _Continue_0;
.endif 
@ 
  
L_SACM_Length:
		R1 = [R_SACM_Decode_In_PTR];
		R3 = [R1++];
		R4 = [R1++];
		R4 = R4 LSR 1;
		R2 = R4 LSR 3;
		R3 = R3 ROR 1;	
		[R_SACM_Decode_Length_L] = R3;
		[R_SACM_Decode_Length_H] = R4;
	    [R_SACM_Decode_In_PTR] = R1;
        
_Continue_0:	
		R1 = 0x0000;
	 	[R_SACM_Decode_Count_L] = R1;
	 	R1 = 0x0000;
	 	[R_SACM_Decode_Count_H] = R1; 
	 	r1=0
	 	[R_SACM_DECODE_IN_LENGTH]=r1
		retf;

.endp


//****************************************************************
// Function: F_SACM_Decode_Initial_Process    
// Decription: Initialize decode process       
// Input: N/A
// Output: N/A
// Registers used : N/A
// Syntax:
// 	call F_SACM_Decode_Initial_Process; (in assembly domain)
//**************************************************************** 
F_SACM_Decode_Initial_Process: .proc
		r1=sr&0x03f
		ds = r1
 		R1 = [R_CODEC_TYPE];					//benwang
		r1 += T_SACM_Dec_Init;
		pc = ds:[r1];

.ifdef	S200_Used
L_S200_Dec_Init:
		//////////////////	//jacky 0512
		push r3,r4 to [sp]
		r3=0x00
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//CALL S200_DEC_INIT_Psuedo; //add by benwang 20051013
		/////////////////////
		RETF;
.else
	.ifdef	S200TTS_Used
L_S200_Dec_Init:
		//CALL LRC_DEC_INIT; //add by benwang 20051013
	
		CALL S200_DEC_INIT_Psuedo; //add by benwang 20051013
		RETF;
	.endif
.endif

.ifdef	S4872_Used
L_S4872_Dec_Init:
	.ifdef C_SpeedControl
		R1 = 160
	.else						
		r1 = 60		//if use S480	
		//r1 = 40;	//if use S720			
	.endif		
		[R_DAC_OUT_LENGTH] = R1
	
		call	S4872_DEC_INIT
		retf 
.endif

.ifdef	S530_Used
L_S530_Dec_Init:
	.ifdef C_SpeedControl		//added by zgq on 20050715	
		R1 = 160
	.else						//added by zgq on 20050715	
		r1 = 60						//added by zgq on 20050715	
	.endif						//added by zgq on 20050715	
		[R_DAC_OUT_LENGTH] = R1	
		 R1 = 128; 							 	// for Speed Control  //20060629
	    [R_SPEED_MAX_PITCH_LENGTH] = R1;		// for Speed Control  //20060629
	    R1 = 6 ;								// for Speed Control  //20060629 
	    [R_SPEED_PITCH_STEP] = R1;				// for Speed Control  //20060629 	
		///////////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x0e
		r4=0x01
		call mr
		pop r3,r4 from [sp]
		//call	S530_DEC_INIT_Psuedo	
		////////////////////////		
		retf
.endif

.ifdef	S600TTS_Used
L_S600TTS_Dec_Init:
		R1=20;
		[R_S600_Delay_Count]=R1
	.ifdef C_SpeedControl		//added by zgq on 20050728	
		R1 = 160; 
	.else
		r1 = 60;	
	.endif
		[R_DAC_OUT_LENGTH] = R1; 
	
		call	S600_DEC_INIT
		retf
.endif

.ifdef	S320_Used
L_S320_Dec_Init:
		///////////////////////
		push r3,r4 to [sp]
		r3=0x68
		r4=0x01
		call mr
		pop r3,r4 from [sp]	
		////////////////////////
//		call S320_DEC_INIT; //add by benwang 20051020
		
		retf
.endif

.ifdef A6400S_Used				//added by zgq on 20051215
L_A6400S_Dec_Init:				//added by zgq on 20051215

		call A6400S_DEC_init;	//added by zgq on 20051215
		r1 = 1152;				//added by zgq on 20051215
		[R_DAC_OUT_LENGTH] = r1;//added by zgq on 20051215
		retf;					//added by zgq on 20051215
.endif							//added by zgq on 20051215


.ifdef	MS01_Used
L_MS01_Dec_Init:
		r1 = 160; 								
	    [R_DAC_OUT_LENGTH] = r1; 	
	    			
		////////////////////	//jacky 0512
//		push r3,r4 to [sp]
//		r3=0x032
//		r4=0x01
//		call mr
//		pop	r3,r4 from [sp]
		CALL MS01_Dec_Init;      //add by benwang 20060220
		///////////////////////
		RETF;	
.endif

.ifdef	MS02_Used
L_MS02_Dec_Init:
		R1 = 160; 						//added by zgq on 20050715	 
		[R_DAC_OUT_LENGTH] = R1; 		//added by zgq on 20050715
		
		R1 = MS02_DAC_16K; 				//added by zgq on 20050715
		R2 = 8 							//added by zgq on 20050715 //change to 8ch by benwang 20060111
		///////////////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x14
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call F_SACM_MS02_MIDI_Initial_Psuedo; 	//added by zgq on 20050715	
		
		push r3,r4 to [sp]
		r3=0x12
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call	F_SACM_MS02_MIDI_Play_Psuedo
		//////////////////
		retf
.endif	

.ifdef	DVR520_Used							//added by zgq 20051220
L_DVR520_Dec_Init:							//added by zgq 20051220
	.ifdef C_SpeedControl					//added by zgq 20051220
		R1 = 320                			//added by zgq 20051220
	.else									//added by zgq 20051220
		r1 = 160							//added by zgq 20051220
	.endif									//added by zgq 20051220
		[R_DAC_OUT_LENGTH] = R1; 			//added by zgq 20051220
		 R1 = 128; 							 	// for Speed Control  //20060629
	    [R_SPEED_MAX_PITCH_LENGTH] = R1;		// for Speed Control  //20060629
	    R1 = 6 ;								// for Speed Control  //20060629 
	    [R_SPEED_PITCH_STEP] = R1;				// for Speed Control  //20060629 
		CALL F_DVR520_Dec_Init;				//added by zgq 20051220
		RETF;								//added by zgq 20051220
.endif	
.ifdef	SPEECH1600_Used
L_A1600_Dec_Init:
		//R1 = 256  			//move from kernel by benwang 01.08 //del by benwang 2005/07/26
	.ifdef C_SpeedControl		//added by benwang 2005/07/26	
		R1 = 256                //added by benwang 2005/07/26	
	.else						//added by benwang 2005/07/26
		r1 = 128					//added by benwang 2005/07/26
	.endif						//added by benwang 2005/07/26
		
		[R_DAC_OUT_LENGTH] = R1; 				//move from kernel by benwang 01.08 
		
		R1 = 128; 							 	// for Speed Control  //20060629
	    [R_SPEED_MAX_PITCH_LENGTH] = R1;		// for Speed Control  //20060629
	    R1 = 6 ;								// for Speed Control  //20060629 
	    [R_SPEED_PITCH_STEP] = R1;
	    
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$
		push r3,r4 to [sp]
		r3=0x04
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//		CALL DVR1600_Play_Init;
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$
		////////////////////
		RETF;
.else	
	.ifdef	AUDIO1600_Used	
L_A1600_Dec_Init:
		R1 = 256  								//move from kernel by benwang 01.08
		[R_DAC_OUT_LENGTH] = R1; 				//move from kernel by benwang 01.08 

		CALL DVR1600_DEC_INIT;
		RETF;	
	.endif
.endif


.ifdef	SPEECH4800_Used					//added by chengye 2006/9/12
L_A4800_Dec_Init:
		.ifdef C_SpeedControl			//20060629
		R1 = 512                	//20060629	
	.else							//20060629	
		r1 = 256					//20060629	
	.endif							//20060629	
		[R_DAC_OUT_LENGTH] = R1;	//20060629 
		R1 = 256; 					//20060629	
		[R_DECODE_OUT_LENGTH] = R1; //20060629	
	
	    R1 = 256; 							 	//20060629	
	    [R_SPEED_MAX_PITCH_LENGTH] = R1;		//20060629	
	    R1 = 12 ;								//20060629	
	    [R_SPEED_PITCH_STEP] = R1;				//20060629			 

		R1 = __buf_r083_ac8_codec_save;
		R2 = [R_SACM_DVR4800_Decode_In_PTR];
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//modify by chengye 2006/9/12		
		push r3,r4 to [sp]
		r3=0x01c
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	//	call __f_r083_ac8_decode_init_Psuedo;
		R2 = [R_SACM_DVR4800_Decode_In_PTR];
		R2 += R1;
		[R_SACM_DVR4800_Decode_In_PTR] = R2;
//		[R_SACM_DVR4800_DECODE_OUT_LENGTH]=R1;
		[R_SACM_DVR4800_DECODE_IN_LENGTH]=R1;
		retf
.else	
	.ifdef	AUDIO4800_Used		
L_A4800_Dec_Init:
		.ifdef C_SpeedControl			//20060629
		R1 = 512                	//20060629	
	.else							//20060629	
		r1 = 256					//20060629	
	.endif							//20060629	
		[R_DAC_OUT_LENGTH] = R1;	//20060629 
		R1 = 256; 					//20060629	
		[R_DECODE_OUT_LENGTH] = R1; //20060629	
	
	    R1 = 256; 							 	//20060629	
	    [R_SPEED_MAX_PITCH_LENGTH] = R1;		//20060629	
	    R1 = 12 ;								//20060629	
	    [R_SPEED_PITCH_STEP] = R1;				//20060629		
		
		R1 = __buf_r083_ac8_codec_save;
		R2 = [R_SACM_DVR4800_Decode_In_PTR];
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//modify by chengye 2006/9/12		
		push r3,r4 to [sp]
		r3=0x01c
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		
	//	call __f_r083_ac8_decode_init_Psuedo;
		R2 = [R_SACM_DVR4800_Decode_In_PTR];
		R2 += R1;
		[R_SACM_DVR4800_Decode_In_PTR] = R2;
//		[R_SACM_DVR4800_DECODE_OUT_LENGTH]=R1;
		[R_SACM_DVR4800_DECODE_IN_LENGTH]=R1;
		retf		
	.endif	
.endif

//added by benwang 20051220
.ifdef	AUDIO3200_Used
L_A3200_Dec_Init:
	.ifdef C_SpeedControl		
		R1 = 256
	.else						
		r1 = 128						
	.endif						
		[R_DAC_OUT_LENGTH] = R1	
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//added by chengye for	Speed Control 2006/9/12	
		R1 = 128; 							 	
	    [R_SPEED_MAX_PITCH_LENGTH] = R1;	
	    R1 = 6 ;							
	    [R_SPEED_PITCH_STEP] = R1;			
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	    
		call	DVR3200_Play_Init			
		retf
.endif
//added by benwang 20051220


.ifdef MP3_Used						//added by benwang 20060111
L_MP3_Dec_Init:						//added by benwang 20060111
		//////////////////	//jacky 0512
		push r3,r4 to [sp]
		r3=0x020
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call MP3_DEC_init_Psuedo;	//added by benwang 20060111
		//////////////////
//		r1 = 1152;					//added by benwang 20060111		//xiaolei delete according to Jacky
//		[R_DAC_OUT_LENGTH] = r1;	//added by benwang 20060111		//xiaolei delete according to Jacky

		R1 = 0;                     //add by benwang 20060221
		[F_BS] = R1;				//add by benwang 20060221

		R1 = 0;                     //add by benwang 20060221 
		[R_Fs_Ready] = R1;          //add by benwang 20060221
		
		[Mp3_FrameCount_L] = r1; //20060525
		[Mp3_FrameCount_H] = r1; //20060525			
		[MP3_TimeInfo] = r1;       //20060525 
		
//		call F_SACM_System_Get_BS_Manual; 	//must call get_BS before _MP3_Get_BR_FS_Psuedo 20060118
//		call _MP3_Get_BR_FS_Psuedo; 		//add by benwang to test this function //20060118
											//	Output:	r1--0(success) or 1(fail)
											//			r2--sampling rate(in Hz)
											//			r3--bitrate(in Kbps)
											//	Reg used:
											//	Note:If a valid header can't be found in one dec_in_buf,then return error
		retf;					//added by benwang 20060111
								//added by benwang 20060111
.endif                          //added by benwang 20060113 

//add by benwang 20060119
.ifdef	AUDIO1601_Used
L_A1601_Dec_Init:
	.ifdef C_SpeedControl
		R1 = 256
	.else
		r1 = 128
	.endif
		[R_DAC_OUT_LENGTH] = R1
		/////////////////////	//jacky 0512
		push r3,r4 to [sp]
		r3=0x02a
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call	A1601_DEC_INIT_Psuedo //add by benwang 20060119
		///////////////////////
		retf
.endif

.ifdef A128_Used						//added by benwang 20060221
L_A128_Dec_Init:						//added by benwang 20060221
		///////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x36
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call A128_DEC_init_Psuedo;		//added by benwang 20060221
		//////////////////////

//		r1 = 1152;						//added by benwang 20060221		//xiaolei delete according to Jacky
//		[R_DAC_OUT_LENGTH] = r1;		//added by benwang 20060221		//xiaolei delete according to Jacky

		R1 = 1;                     	//added by benwang 20060221
		[F_BS] = R1;					//added by benwang 20060221

		R1 = 0;                     //add by benwang 20060221 
		[R_Fs_Ready] = R1;          //add by benwang 20060221
		
		retf;							//added by benwang 20060221
										//added by benwang 20060221
.endif                          		//added by benwang 20060221 

.ifdef	DVR1800_Used					//add by benwang 20060621	
L_DVR1800_Dec_Init:
	.ifdef C_SpeedControl	 //20060629	
		R1 = 640             //20060629	
	.else					 //20060629		
		r1 = 320			 //20060629					
	.endif				 	 //20060629					
		[R_DAC_OUT_LENGTH] = R1
		R1 = 256; 							 	//20060629	
	    [R_SPEED_MAX_PITCH_LENGTH] = R1;		//20060629	
	    R1 = 12 ;								//20060629	
	    [R_SPEED_PITCH_STEP] = R1;				//20060629
	    
		push r3,r4 to [sp]
		r3=0x4e
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		
		//CALL F_DVR1800_Dec_Init_Psuedo;
		RETF;		
.endif	

.ifdef WAV_Used
L_WAV_Dec_Init:
		r1 = 0;
		[R_SACM_DECODE_IN_LENGTH] = r1;
		
		//r1 = C_DECODE_IN_LENGTH;
		R1 = [R_DECODE_IN_LENGTH]; 							 //add by benwang 20060209

		call F_Wav_Init;				// input:  r1=C_DECODE_IN_LENGTH
										// return:
										//	r1 = Samples Per Second
										//	r2 = decoded samples number
										//	r3 = data length, low
										//	r4 = data length, high
		cmp r1,0
		jz ?read_error
			
		[R_DAC_OUT_LENGTH] = r2;
		r2 = 1;
		r3 = r3 lsr r2;
		mr |= r4 lsr r2;
		[R_SACM_Decode_Length_L] = r3;
		[R_SACM_Decode_Length_H] = r4;
		call F_Wav_SetSampleRate;		//input r1
		
		retf;
			
	?read_error:
	//########################################################
	// Input file is cannot read as wav.  Some error occour.
	// Add your code here.
		nop;
		nop;
		
		retf;
	//########################################################
.endif

.ifdef	ETTS_Used	
L_ETTS_Dec_Init:
	R1 = 160
	[R_DAC_OUT_LENGTH] = R1;
	RETF;
.endif
	
.endp

//========================================================
// Function Name:	F_SACM_Encode_Initial_Process
// Syntax:		call F_SACM_Encode_Initial_Process; (in assembly domain)
// Purpose: 	Initialize encode process                 
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public F_SACM_Encode_Initial_Process
F_SACM_Encode_Initial_Process: .proc
		r1=sr&0x03f
		ds=r1
		r1=[R_CODEC_TYPE]
		r1+=T_SACM_Pro_Init
		pc=ds:[r1]

L_SACM_Not_Pro_Init:
		jmp	$

.ifdef	SPEECH1600_Used
L_SACM_DVR1600_Pro_Init:	
		R1 = 0;
		[decvar_Hbufx] = R1;
		[decvar_Hbufy + 0] = R1;
		[decvar_Hbufy + 1] = R1;
		//R1 = C_ADC_IN_LENGTH; //del by benwang 2005/07/27
		R1 = 128; //add by benwang 2005/07/27
		[R_ADC_IN_LENGTH] = r1;   
		//////////////////////		//jacky 0512
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$
		push r3,r4 to [sp]
		r3=0x08
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//		call DVR1600_Rec_Init;				//notice:SPL162002 ICB must added 1600库
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$
		////////////////////////////////
		retf;
.endif

.ifdef	SPEECH4800_Used
L_SACM_DVR4800_Pro_Init:
		R1 = __buf_r083_ac8_codec_save;
		R2 = [BITRATE];			// data rate idx
	//	R3 = [R_SACM_Encode_Out_PTR];
		R5 = [R_SACM_Encode_Out_PTR];		//modify by chengye 2006/9/12
		////////////////	//jacky 0512
		push r4 to [sp]
		R4 = OFFSET _f_r083_ac8_encode_init_nextaddr1
		push R4	to [SP];		// PUSH PC
		R4 = SR & 0x003F
		R4 = R4 | SEG _f_r083_ac8_encode_init_nextaddr1
		push R4	to [SP];		// PUSH CS		
		r4=0x018
		push r4 to [sp]
		r4=sr& (~0x03f)
		r4|=0x01
		push r4 to [sp]
		retf		
_f_r083_ac8_encode_init_nextaddr1:
		pop	r4 from [sp]		
		
		//call __f_r083_ac8_encode_init_Psuedo;
		/////////////////////////
		R2 = [R_SACM_Encode_Out_PTR];
		R2 += R1;
		[R_SACM_Encode_Out_PTR] = R2;
		[R_ENCODE_OUT_LENGTH] = R1;
		R1 = 256
		[R_ADC_IN_LENGTH] = r1;
		retf
.else
.ifdef AUDIO4800_Used
L_SACM_DVR4800_Pro_Init:

//		R1 = 0;
//		[decvar_Hbufx] = R1;
//		[decvar_Hbufy + 0] = R1;
//		[decvar_Hbufy + 1] = R1;

		R1 = __buf_r083_ac8_codec_save;
		R2 = [BITRATE];			// data rate idx
		R5 = [R_SACM_Encode_Out_PTR];   //add by benwang 20060624
		///////////////////		//jacky 0727
		push r3,r4 to [sp]
		r3=0x018
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call __f_r083_ac8_encode_init_Psuedo;
		//////////////////////////
		R2 = [R_SACM_Encode_Out_PTR];
		R2 += R1;
		[R_SACM_Encode_Out_PTR] = R2;
		[R_ENCODE_OUT_LENGTH] = R1;
		R1 = 256
		[R_ADC_IN_LENGTH] = r1;
		retf	
.endif
.endif	

//add by benwang 20051220
.ifdef	AUDIO3200_Used
L_SACM_DVR3200_Pro_Init:	
		R1 = 0;
		[decvar_Hbufx] = R1;
		[decvar_Hbufy + 0] = R1;
		[decvar_Hbufy + 1] = R1;
		R1 = 128; 
		[R_ADC_IN_LENGTH] = r1;   
		call DVR3200_Rec_Init;
		retf;
.endif

.ifdef	DVR520_Used
L_SACM_DVR520_Pro_Init:	
		R1 = 0;
		[decvar_Hbufx] = R1;
		[decvar_Hbufy + 0] = R1;
		[decvar_Hbufy + 1] = R1;
		
		R1 = 160
		[R_ADC_IN_LENGTH] = r1;
	
		R1 = 0;
		[R_ENCODE_OUT_LENGTH]=R1;
		call F_DVR520_Enc_Init;
	retf;
.endif
.ifdef WAV_Used
L_SACM_WAV_Pro_Init:
		r1 = [BITRATE];
		r2 = 1;//1，单声道 2，双声道
		r3 = C_WAV_SampleRate;
		r4 = 160;
		call F_Wav_Enc_Init;	
		[R_ADC_IN_LENGTH] = r1;	 
		[_R_LengthWAV_Header] = r2;
		/////////////////////////////seek////////
//		r1 = R_SACM_Decode_In_Buffer
//		[sp--] = r2
//		[Sp--] = r1
		r1 = [R_SACM_Encode_Out_PTR];
		r1 += r2
		[R_SACM_Encode_Out_PTR] = r1
//		call _WAV_SetFileHeader
//		sp=sp+2;
		retf;
.endif
.ifdef	DVR1800_Used
L_SACM_DVR1800_Pro_Init:	
		R1 = 0;
		[decvar_Hbufx] = R1;
		[decvar_Hbufy + 0] = R1;
		[decvar_Hbufy + 1] = R1;

		R1 = 320; //add by wgp 20060526
		[R_ADC_IN_LENGTH] = r1;  
		 
		push r3,r4 to [sp]
		r3=0x4a
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		
		//call F_DVR1800_Enc_Init_Psuedo;
		
		retf;
.endif
//////////////////////////////////////

.endp

//========================================================
// Function Name:	F_SACM_Encode_Finish_Process
// Syntax:		call F_SACM_Encode_Finish_Process; (in assembly domain)
// Purpose: 	encode finish process                 
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public F_SACM_Encode_Finish_Process
F_SACM_Encode_Finish_Process: .proc
		r1=sr&0x03f
		ds=r1
		r1=[R_CODEC_TYPE]
		r1+=T_SACM_Encode_Finish_Proc
		pc=ds:[r1]

L_SACM_Not_Finish_Pro:
		jmp	$	
		
.ifdef	SPEECH1600_Used		
L_DVR1600_Finish_Pro:	
		///////////////////	//jacky 0512
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$
		push r3,r4 to [sp]
		r3=0x0c
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//		call DVR1600_Rec_Endp;
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$
		////////////////////////
		retf;	
.endif

.ifdef	SPEECH4800_Used
L_DVR4800_Finish_Pro:
//		call DVR4800_Rec_Endp
		retf
.else
.ifdef AUDIO4800_Used
L_DVR4800_Finish_Pro:
		retf
.endif
.endif		
//add by benwang 20051220
.ifdef	AUDIO3200_Used		
L_DVR3200_Finish_Pro:	
		call DVR3200_Rec_Endp;
		retf;	
.endif
//add by benwang 20051220
.ifdef	DVR520_Used		//added by zgq on 20051222
L_DVR520_Finish_Pro:	//added by zgq on 20051222

		retf;			//added by zgq on 20051222
.endif

//add by benwang 20060621	
.ifdef	DVR1800_Used   	
L_DVR1800_Finish_Pro:		
		retf;	
.endif

.endp

//========================================================
// Function Name:	F_SACM_Decode_Process
// Syntax:		call F_SACM_Decode_Process; (in assembly domain)
// Purpose: 	execute decode process                
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
F_SACM_Decode_Process: .proc

		//fiq off; 								//temp added 2004/04/06
	 	r1=sr&0x03f
	 	ds=r1
	 	r1 = [R_CODEC_TYPE];
	 	r1+=T_SACM_Dec_Proc_2
	 	pc=ds:[r1]

.ifdef	MS01_Used			    
L_MS01_Dec_Proc:
		///////////////////////		//jacky 0512
//		push r3,r4 to [sp]
//		r3=0x34
//		r4=0x01
//		call mr
//		pop	r3,r4 from [sp]
		CALL MS01_DEC_PROC;
		////////////////////////
		retf;
.endif

.ifdef	MS02_Used
L_MS02_Dec_Proc:
		////////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x16
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call	F_SACM_MS02_MIDI_ServiceLoop_Psuedo
		///////////////////
		retf
.endif

.ifdef ETTS_Used	
L_ETTS_Dec_Proc:					//add by chengye 2007/4/18
		push R1, R3 to [sp]
		R1 = [R_SACM_DAC_Out_PTR_Decode]
		R2 = 0
		R3 = [R_DAC_OUT_LENGTH]
		push R1, R3 to [SP]
		call _ETTS_DecProc
		SP += 3
		cmp R1, 0
		jz ?SkipSetEnd
		R1 = [R_SACM_Play_Flag]
		R1 |= C_SACM_DECODE_END
		[R_SACM_Play_Flag] = R1		
?SkipSetEnd:
		pop R1, R3 from [sp]
		retf
.endif

L_Not_MIDI_Dec_Proc:
	 	R1 = [R_SACM_Decode_Count_L];
	 	R2 = [R_SACM_Decode_Count_H];
		R1 += [R_SACM_DECODE_IN_LENGTH];
		R2 += 0, carry;
	 	[R_SACM_Decode_Count_L] = R1;
		[R_SACM_Decode_Count_H] = R2;           
	 	R3 = [R_SACM_Decode_Length_L];
	 	R4 = [R_SACM_Decode_Length_H];
		CMP R2,R4;
	    JB _Branch_0;
	    CMP R1,R3;
	    JB _Branch_0;
		
		r1=sr&0x03f	    
		ds = r1
	    R1 = [R_CODEC_TYPE];
	    r1 += T_SACM_Dec_Proc_1
	    pc = ds:[r1];
 
Chk_Dec_Length_End:
		R1 = [R_SACM_Play_Flag];
		R1 |= C_SACM_DECODE_END;
		[R_SACM_Play_Flag] = R1;
//	    call F_SACM_XXXX_EndPlay;									// Decode End
		retf;

.ifdef	S530_Used
Chk_S530_Length:
		////////////////		//jacky 0727
		r3=[0xb78]
		//R3 = [SubFrameNO];		// added by Ray 2004.04.23
   		//////////////////////
   		CMP R3,0;				// added by Ray 2004.04.23
		JNE _Branch_0;	
		jmp	Chk_Dec_Length_End
.endif

.ifdef	S600TTS_Used
Chk_S600TTS_Length:
		R3=[SubFrame_NUM]; 
   		CMP R3,0;
		JNE _Branch_0;
		//Delay Decoder	
		R1=[R_S600_Delay_Count];
		R1-=1;
		[R_S600_Delay_Count]=R1;
		JE Chk_Dec_Length_End;
		call S600_ZERO_Decoder;
		RETF;	
.endif
	
.ifdef	S320_Used		
Chk_S320_Length:						//added by chengye 2007/4/28
//		R1 = [R_S320_SubFrameCounter]
//		JNE _Branch_0;	
		jmp	Chk_Dec_Length_End
.endif	

.ifdef	MS01_Used		
Chk_MIDI_Length:
		jmp	$		
.else
	.ifdef	MS02_Used
Chk_MIDI_Length:
		jmp	$		
	.endif	
.endif

//add by benwang start 20060209
.ifdef MP3_Used
Chk_MP3_Length:
	//////////////			//jacky 0727
	r1=[0x1ec1]
	//r1 = [CFrame_layer];		//<< update stanwang,2005.11.17 >>
	//////////////
	cmp		r1 , 2
	je	Chk_Dec_Length_End;
	////////////////		//jacky 0727
	r1=[0x2389]
	//r1 = [r_layer3_granule];	//<< update stanwang,2005.09.22 >>
	//////////////
	cmp		r1 , 2;
	jne	Chk_Dec_Length_End;
	////////////////		//jacky 0727
	r1=[0x23f3]
	//r1 = [r_layer3_gr];
	///////////////
	cmp		r1 , 1;
	jne	Chk_Dec_Length_End;
	jmp	_Branch_0;		
.endif
//add by benwang end 20060209
		
	//add by benwang start 20060221
.ifdef A128_Used
Chk_A128_Length:
	//////////////			//jacky 0727
	r1=[0x1ec1]
	//r1 = [CFrame_layer];		//<< update stanwang,2005.11.17 >>
	//////////////
	cmp		r1 , 2
	je	Chk_Dec_Length_End;
	////////////////		//jacky 0727
	r1=[0x2389]
	//r1 = [r_layer3_granule];	//<< update stanwang,2005.09.22 >>
	//////////////
	cmp		r1 , 2;
	jne	Chk_Dec_Length_End;
	////////////////		//jacky 0727
	r1=[0x23f3]
	//r1 = [r_layer3_gr];
	///////////////
	cmp		r1 , 1;
	jne	Chk_Dec_Length_End;
	jmp	_Branch_0;		
.endif
//add by benwang end 20060221		
		
_Branch_0:
//		R1 = [R_SACM_Decode_In_PTR];
//	    [R_SACM_DECODE_IN_LENGTH] = R1;
	  
	    r1=sr&0x03f
	    ds = r1
	    R1 = [R_CODEC_TYPE];
	    r1 += T_SACM_Dec_Proc
	    pc = ds:[r1];	    

.ifdef	S200_Used
L_S200_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;
		/////////////////////	//jacky 0512
		push r3,r4 to [sp]
		r3=0x02
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//CALL S200_DEC_PROC_Psuedo; //add by benwang 20051013 
		/////////////////////
		goto L_Continue_0;
.else
	.ifdef	S200TTS_Used		
L_S200_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;
		//CALL LRC_DEC_PROC; //del by benwang 20051013
		CALL S200_DEC_PROC_Psuedo; //add by benwang 20051013 
		goto L_Continue_0;
	.endif			    
.endif

.ifdef	S4872_Used
L_S4872_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;
.ifdef	C_SpeedControl					//added by zgq on 20051222
		r1 = R_SACM_Decode_Out_Buffer;	//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.else									//added by zgq on 20051222
		r1 = [R_SACM_DAC_Out_PTR_Decode];//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.endif									//added by zgq on 20051222
		CALL S4872_DEC_PROC
		goto L_Continue_0	
.endif

.ifdef	S530_Used
L_S530_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;
.ifdef	C_SpeedControl					//added by zgq on 20051222
		r1 = R_SACM_Decode_Out_Buffer;	//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.else									//added by zgq on 20051222
		r1 = [R_SACM_DAC_Out_PTR_Decode];//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.endif									//added by zgq on 20051222
		/////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x010
		r4=0x01
		call mr
		pop r3,r4 from [sp]
		//call	S530_DEC_PROC_Psuedo
		///////////////
		goto	L_Continue_0
.endif

.ifdef	S600TTS_Used
L_S600TTS_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];   //added by zgq on 20050801
	    [R_SACM_DECODE_IN_LENGTH] = R1;//added by zgq on 20050801
.ifdef	C_SpeedControl					//added by zgq on 20051222
		r1 = R_SACM_Decode_Out_Buffer;	//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.else									//added by zgq on 20051222
		r1 = [R_SACM_DAC_Out_PTR_Decode];//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.endif									//added by zgq on 20051222
		call	S600_DEC_PROC
		goto	L_Continue_0
.endif

.ifdef	S320_Used
L_S320_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];   //added by benwang 20051020
	    [R_SACM_DECODE_IN_LENGTH] = R1;//added by zgq on 20051020
	    /////////////////
		push r3,r4 to [sp]
		r3=0x6a
		r4=0x01
		call mr
		pop r3,r4 from [sp]
		///////////////
//		call	S320_DEC_PROC //added by zgq on 20051020
		goto	L_Continue_0 //added by zgq on 20051020
.endif

.ifdef A6400S_Used				//added by zgq on 20051215
L_A6400S_Dec_Proc:				//added by zgq on 20051215
		R1 = [R_SACM_A6400S_Decode_In_PTR];//added by zgq on 20051215
		[R_SACM_A6400S_DECODE_IN_LENGTH] = R1;//added by zgq on 20051215
		CALL A6400S_DEC_PROC				//added by zgq on 20051215

		call F_SP_SACM_SetSampleRate;		//added by zgq on 20051215
		goto	L_Continue_0				//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef	DVR520_Used						//added by zgq on 20051220
L_DVR520_Dec_Proc:						//added by zgq on 20051220
		R1 = [R_SACM_Decode_In_PTR];	//added by zgq on 20051220
	    [R_SACM_DECODE_IN_LENGTH] = R1;	//added by zgq on 20051220
	
.ifdef	C_SpeedControl					//added by zgq on 20051222
		r1 = R_SACM_Decode_Out_Buffer;	//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.else									//added by zgq on 20051222
		r1 = [R_SACM_DAC_Out_PTR_Decode];//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.endif									//added by zgq on 20051222

		call	F_DVR520_Decode			//added by zgq on 20051220
		goto	L_Continue_0			//added by zgq on 20051220
.endif

L_MIDI_Dec_Proc:
		jmp	$

.ifdef	SPEECH1600_Used
L_A1600_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;
		//CALL DVR1600_DEC_PROC; //add by benwang 2005/07/26
.ifdef	C_SpeedControl					//added by zgq on 20051222
		r1 = R_SACM_Decode_Out_Buffer;	//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.else									//added by zgq on 20051222
		r1 = [R_SACM_DAC_Out_PTR_Decode];//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.endif									//added by zgq on 20051222
		///////////////////	//jacky 0512
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$
		push r3,r4 to [sp]
		r3=0x06
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//		CALL DVR1600_Play_Proc; //add by benwang 2005/07/26
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$
		////////////////////////
		goto L_Continue_0;
.else
	.ifdef	AUDIO1600_Used
L_A1600_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;
		//CALL DVR1600_DEC_PROC; //add by benwang 2005/07/26
.ifdef	C_SpeedControl					//added by zgq on 20051222
		r1 = R_SACM_Decode_Out_Buffer;	//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.else									//added by zgq on 20051222
		r1 = [R_SACM_DAC_Out_PTR_Decode];//added by zgq on 20051222
		[R_SACM_Decode_Out_PTR] = r1;	//added by zgq on 20051222
.endif									//added by zgq on 20051222
		CALL DVR1600_Play_Proc; //add by benwang 2005/07/26
		goto L_Continue_0;
	.endif					
.endif

.ifdef	SPEECH4800_Used								//added by chengye 2006/9/12
L_A4800_Dec_Proc:
.ifdef	C_SpeedControl								//20060629
		r1 = R_SACM_Decode_Out_Buffer;				//20060629
		[R_SACM_Decode_Out_PTR] = r1;				//20060629
.else												//20060629
		r1 = [R_SACM_DAC_Out_PTR_Decode];			//20060629
		[R_SACM_Decode_Out_PTR] = r1;				//20060629
.endif												//20060629
		//R1 = [R_SACM_DVR4800_DAC_Out_PTR_Decode]; //20060629
		R2 = [R_SACM_DVR4800_Decode_In_PTR];
		R5 = __buf_r083_ac8_codec_save;
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//modify by chengye 2006/9/12	
		push r3,r4 to [sp]
		r3=0x1e
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	//	call __f_r083_ac8_decode_Psuedo;
		.ifndef C_SpeedControl			  			//20060629
		R2 = [R_SACM_DVR4800_DAC_Out_PTR_Decode];
		R2 += 256; 
		[R_SACM_DVR4800_DAC_Out_PTR_Decode] = R2;
		.endif							 			//20060629
		R2 = [R_SACM_DVR4800_Decode_In_PTR];
		R2 += R1;
		[R_SACM_DVR4800_Decode_In_PTR] = R2;
//		[R_SACM_DVR4800_DECODE_OUT_LENGTH]=R1;
		[R_SACM_DVR4800_DECODE_IN_LENGTH]=R1;
		retf	

.else
	.ifdef	AUDIO4800_Used		
L_A4800_Dec_Proc:

	.ifdef	C_SpeedControl								//20060629
		r1 = R_SACM_Decode_Out_Buffer;				//20060629
		[R_SACM_Decode_Out_PTR] = r1;				//20060629
	.else												//20060629
		r1 = [R_SACM_DAC_Out_PTR_Decode];			//20060629
		[R_SACM_Decode_Out_PTR] = r1;				//20060629
	.endif												//20060629
		//R1 = [R_SACM_DVR4800_DAC_Out_PTR_Decode]; //20060629
		R2 = [R_SACM_DVR4800_Decode_In_PTR];
		R5 = __buf_r083_ac8_codec_save;
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//modify by chengye
		push r3,r4 to [sp]
		r3=0x01e
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		
	//	call __f_r083_ac8_decode_Psuedo;
	.ifndef C_SpeedControl			  			//20060629
		R2 = [R_SACM_DVR4800_DAC_Out_PTR_Decode];
		R2 += 256; 
		[R_SACM_DVR4800_DAC_Out_PTR_Decode] = R2;
	.endif							 			//20060629
		R2 = [R_SACM_DVR4800_Decode_In_PTR];
		R2 += R1;
		[R_SACM_DVR4800_Decode_In_PTR] = R2;
//		[R_SACM_DVR4800_DECODE_OUT_LENGTH]=R1;
		[R_SACM_DVR4800_DECODE_IN_LENGTH]=R1;
		retf
	.endif	
.endif

//add by benwang 20051220
.ifdef	AUDIO3200_Used
L_A3200_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   
		.ifdef C_SpeedControl
			r1 = R_SACM_Decode_Out_Buffer			
			[R_SACM_Decode_Out_PTR] = r1			
		.else								
			r1 = [R_SACM_DAC_Out_PTR_Decode]	
			[R_SACM_Decode_Out_PTR] = r1	
		.endif	
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		
	
		CALL DVR3200_Play_Proc;
		goto L_Continue_0	
.endif

.ifdef MP3_Used									//added by benwang 20060111
L_MP3_Dec_Proc:									//added by benwang 20060111
		R1 = [R_SACM_MP3_Decode_In_PTR];		//added by benwang 20060111
		[R_SACM_MP3_DECODE_IN_LENGTH] = R1;		//added by benwang 20060111
		
		r1 = 0;			//<< update stanwang,2006.01.23 >> //add by benwang 20060209
		/////////////		//jacky 0727
		[0x22f7]=r1
		//[_r_DEC_PROC_pcm_ready] = r1;                      //add by benwang 20060209 
		////////////////		//jacky 0727
		push r3,r4 to [sp]
		r3=0x22
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//CALL MP3_DEC_PROC_Psuedo						//added by benwang 20060111
		//////////////////
//add by benwang start 20060209
		cmp		r1 , 1;		//<< update stanwang,2005.10.05--if fail,refill dec_in_buf and decode again >>
		jne		?_L1;
		goto	?_L2;
?_L1:		
//Add 2007/01/12	对于个别在MP3解码中读到无效数据的文件处理:
	.ifdef MP3_FATREADBUFFER
		r1 = 1				//Mode == 1表示此时解码错误需要在读取数据解码
		[_MP3DecodeErrorFlag] = r1
	.endif
//Add end 2007/01/12
		R1 = [R_SACM_Decode_In_PTR];
		R1 -= [R_SACM_DECODE_IN_LENGTH];
		[R_SACM_DECODE_IN_LENGTH] = R1;	

//-------------------------------------------------------------------------------
//added by chengye 2007/7/16		
		R1 = 1024 								//xiaolei add according chengye's
		call F_GetSyncWord_Init;
		R1 = 0									//added by chengye 2007/8/29
		[_R_MP3_Error_Count] = R1				//$
?L_Continue_Search:								//$
		R1 = [_R_MP3_Error_Count]				//$
		R1 = R1 + 1								//$
		[_R_MP3_Error_Count] = R1				//$
		cmp R1, 1000							//$
		jae L_Error_End?						//added by chengye 2007/8/29
		
		R1 = [R_SACM_Decode_Count_L];
	 	R2 = [R_SACM_Decode_Count_H];
		R1 += [R_SACM_DECODE_IN_LENGTH];
		R2 += 0, carry;
	 	[R_SACM_Decode_Count_L] = R1;
		[R_SACM_Decode_Count_H] = R2;
	 	R3 = [R_SACM_Decode_Length_L];
	 	R4 = [R_SACM_Decode_Length_H];
		CMP R2,R4;
	    JB _Branch_0?
	    CMP R1,R3;
	    JB _Branch_0?
		goto Chk_Dec_Length_End

L_Error_End?:										//added by chengye 2007/8/29
		goto Chk_Dec_Length_End						//added by chengye 2007/8/29
		
_Branch_0?:
		call	F_SACM_MP3_System_Get_BS_Manual;			

		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;
	    call F_GetSyncWord
	    R4 = [R_SACM_Decode_In_PTR];
		R4 -= [R_SACM_DECODE_IN_LENGTH];
		[R_SACM_DECODE_IN_LENGTH] = R4;
		cmp R1, 1
		jne ?L_Continue_Search
		
		call	F_SACM_MP3_System_Get_BS_Manual;
		push r3,r4 to [sp]
		r3=0x020
		r4=0x01
		call mr										//找到有效桢后，重新init一次
		pop	r3,r4 from [sp]
		goto	L_Not_MIDI_Dec_Proc;
//------------------------------------------------------------------------------
		
?_L2:
//add by benwang end 20060209
//Add 2007/01/12	对于个别在MP3解码中读到无效数据的文件处理:
	.ifdef MP3_FATREADBUFFER
		r1 = 0		//得到有效数据后重新置位.
		[_MP3DecodeErrorFlag] = r1
	.endif
//Add end 2007/01/12
		r1 = 1;			//<< update stanwang,2006.01.23 >> //add by benwang 20060209
		/////////		//jacky 0727
		[0x22f7]=r1
		//[_r_DEC_PROC_pcm_ready] = r1;                      //add by benwang 20060209 
		///////
		R1 = [R_Fs_Ready];                          //add by benwang 20060221     
		cmp R1, 0;                                  //add by benwang 20060221
		jne ?_Fs_Ready; 							//add by benwang 20060221

		call F_SP_SACM_MP3_SetSampleRate;		//added by benwang 20060111
		R1 = 1;                                     //add by benwang 20060221 
		[R_Fs_Ready] = R1;                          //add by benwang 20060221
?_Fs_Ready:               							//add by benwang 20060221 
		goto	L_Continue_0					//added by benwang 20060111
.endif											//added by benwang 20060111
//add by benwang 20060119
.ifdef AUDIO1601_Used
L_A1601_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;
		//CALL DVR1600_DEC_PROC; 
.ifdef	C_SpeedControl					
		r1 = R_SACM_Decode_Out_Buffer;	
		[R_SACM_Decode_Out_PTR] = r1;	
.else									
		r1 = [R_SACM_DAC_Out_PTR_Decode];
		[R_SACM_Decode_Out_PTR] = r1;	
.endif		
		/////////////////		//jacky 0512
		push r3,r4 to [sp]
		r3=0x02c							
		r4=0x01
		call mr
		pop r3,r4 from [sp]
		//CALL A1601_DEC_PROC_Psuedo;  //add by benwang 20060120
		///////////////////////
		goto L_Continue_0;
.endif

//add by benwang start 20060221
.ifdef A128_Used									
L_A128_Dec_Proc:									
		R1 = [R_SACM_Decode_In_PTR];		
		[R_SACM_DECODE_IN_LENGTH] = R1;		
		
		r1 = 0;		
		///////////////		//jacky 0727
		[0x22f7]=r1	
		//[_r_DEC_PROC_pcm_ready] = r1;                      
		//////////////
		/////////////////	//jacky 0727
		push r3,r4 to [sp]
		r3=0x038
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//CALL A128_DEC_PROC_Psuedo						
		//////////////////////////

		cmp		r1 , 1;		
		je		?_L2;
		R1 = [R_SACM_Decode_In_PTR];
		R1 -= [R_SACM_DECODE_IN_LENGTH];
		[R_SACM_DECODE_IN_LENGTH] = R1;	
		
		call	F_SACM_System_Get_BS_Manual;
		
		goto	L_Not_MIDI_Dec_Proc;
?_L2:

		r1 = 1;			
		/////////////		//jacky 0727
		[0x22f7]=r1
		//[_r_DEC_PROC_pcm_ready] = r1;
		///////////////

		R1 = [R_Fs_Ready];                          //add by benwang 20060221     
		cmp R1, 0;                                  //add by benwang 20060221
		jne ?_Fs_Ready; 							//add by benwang 20060221
		call F_SP_SACM_A128_SetSampleRate; //add by benwang 20060221		
		R1 = 1;                                     //add by benwang 20060221 
		[R_Fs_Ready] = R1;                          //add by benwang 20060221
?_Fs_Ready:               							//add by benwang 20060221
		
		goto	L_Continue_0			
.endif									
//add by benwang end 20060221


////////////////////////////////////add by benwang 20060621
.ifdef	DVR1800_Used   		
L_DVR1800_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
	    [R_SACM_DECODE_IN_LENGTH] = R1;

.ifdef	C_SpeedControl                   //20060629					
		r1 = R_SACM_Decode_Out_Buffer;   //20060629	
		[R_SACM_Decode_Out_PTR] = r1;	 //20060629
.else									 //20060629
		r1 = [R_SACM_DAC_Out_PTR_Decode];//20060629
		[R_SACM_Decode_Out_PTR] = r1;	 //20060629
.endif 							   		 //20060629									
		push r3, r4 to [sp]
		r3 = 0x50
		r4 = 0x01
		call mr
		pop r3, r4 from [sp]
		//CALL F_DVR1800_Decode_Psuedo; 
		goto L_Continue_0;			
.endif	
////////////////////////////////////add by wgp 20060530	

.ifdef WAV_Used
L_WAV_Dec_Proc:
		R1 = [R_SACM_Decode_In_PTR];
		[R_SACM_DECODE_IN_LENGTH] = R1;
	
		call F_Wav_Dec;	
		jmp L_Continue_0	
.endif
		
L_Continue_0:  
		R1 = [R_SACM_Decode_In_PTR];
		R1 -= [R_SACM_DECODE_IN_LENGTH];
		[R_SACM_DECODE_IN_LENGTH] = R1;			
		retf;
.endp

//****************************************************************
// Function: F_SACM_Encode_Process    
// Decription: execute encode process     
// Input: N/A
// Output: N/A
// Registers used : 
// Syntax: 	call F_SACM_Encode_Process; (in assembly domain)
//**************************************************************** 
.public F_SACM_Encode_Process
F_SACM_Encode_Process: .proc         
		R2 = [R_SACM_Play_Flag];
		R1 = R_SACM_ADC_In_Buffer;
		R2 = R2 &C_SACM_DECODE_ODD;
		JNE	Read_EVEN;
		R1 += [R_ADC_IN_LENGTH] 								
Read_EVEN:
		r3=sr&0x03f
		ds=r3
		r2=[R_CODEC_TYPE]
		r2+=T_SACM_Encode_Pro
		pc=ds:[r2]

L_Not_Encode_Proc:
		jmp	$

.ifdef	SPEECH4800_Used
L_A4800_Encode_Process:
		R2 = [R_SACM_DVR4800_Encode_Out_PTR];
	//	R3 = __buf_r083_ac8_codec_save;
		R5 = __buf_r083_ac8_codec_save;		//modify by chengye 2006/9/12
/////////////////////////////////			//jacky 0512
		push r4 to [sp]
		R4 = OFFSET _f_r083_ac8_encode_nextaddr1
		push R4	to [SP];		// PUSH PC
		R4 = SR & 0x003F
		R4 = R4 | SEG _f_r083_ac8_encode_nextaddr1
		push R4	to [SP];		// PUSH CS		
		r4=0x01a
		push r4 to [sp]
		r4=sr& (~0x03f)
		r4|=0x01
		push r4 to [sp]
		retf		
_f_r083_ac8_encode_nextaddr1:
		pop	r4 from [sp]	
		//call __f_r083_ac8_encode_Psuedo;
///////////////////////////////		
		R2 = [R_SACM_Encode_Out_PTR];
		R2 += R1;
		[R_SACM_Encode_Out_PTR] = R2;
		[R_ENCODE_OUT_LENGTH] = R1;
		
		R1 = [R_SACM_Encode_Count_L];
 		R2 = [R_SACM_Encode_Count_H];
 		R1 += [R_ENCODE_OUT_LENGTH];
 		R2 += 0, carry;
 		[R_SACM_Encode_Count_L] = R1;
		[R_SACM_Encode_Count_H] = R2;      
		retf;
.else
.ifdef	AUDIO4800_Used		
L_A4800_Encode_Process:
//		fir_mov off;			//added by chengye 2006/12/13
//		fraction off			//added by chengye 2006/12/13
		R2 = [R_SACM_DVR4800_Encode_Out_PTR];
		R5 = __buf_r083_ac8_codec_save;
		////////////////		//jacky 0727
		push r3,r4 to [sp]
		r3=0x01a
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
		//call __f_r083_ac8_encode_Psuedo;
		/////////////////////////
		R2 = [R_SACM_Encode_Out_PTR];
		R2 += R1;
		[R_SACM_Encode_Out_PTR] = R2;
		[R_ENCODE_OUT_LENGTH] = R1;
		
		R1 = [R_SACM_Encode_Count_L];
 		R2 = [R_SACM_Encode_Count_H];
 		R1 += [R_ENCODE_OUT_LENGTH];
 		R2 += 0, carry;
 		[R_SACM_Encode_Count_L] = R1;
		[R_SACM_Encode_Count_H] = R2;      
		retf;			
.endif			
.endif//SPEECH4800_Used

.ifdef	SPEECH1600_Used
L_A1600_Encode_Process:
		R2 = [R_SACM_Encode_Out_PTR];
		[R_ENCODE_OUT_LENGTH] = R2;
		////////////////////////		//jacky 0512
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$
		push r3,r4 to [sp]
		r3=0x0a
		r4=0x01
		call mr
		pop	r3,r4 from [sp]
//		call DVR1600_Rec_Proc;										//INPUT:R1;ADC_In_PTR
//$$$$$$$$$$$$$$$$ modify by chye 2006/8/28 $$$$$$$$$$$$$$$$$$$$$		
		//////////////////////////////

		R2 = [R_SACM_Encode_Out_PTR];
		R2 -= [R_ENCODE_OUT_LENGTH];
		[R_ENCODE_OUT_LENGTH] = R2;
        
	    R1 = [R_SACM_Encode_Count_L];
	 	R2 = [R_SACM_Encode_Count_H];
	 	R1 += [R_ENCODE_OUT_LENGTH];
	 	R2 += 0, carry;
	 	[R_SACM_Encode_Count_L] = R1;
		[R_SACM_Encode_Count_H] = R2;      
		retf;
.endif//SPEECH1600_Used

//add by benwang 20051220
.ifdef	AUDIO3200_Used
L_A3200_Encode_Process:
		R2 = [R_SACM_Encode_Out_PTR];
		[R_ENCODE_OUT_LENGTH] = R2;
		call DVR3200_Rec_Proc;										//INPUT:R1;ADC_In_PTR

		R2 = [R_SACM_Encode_Out_PTR];
		R2 -= [R_ENCODE_OUT_LENGTH];
		[R_ENCODE_OUT_LENGTH] = R2;
        
	    R1 = [R_SACM_Encode_Count_L];
	 	R2 = [R_SACM_Encode_Count_H];
	 	R1 += [R_ENCODE_OUT_LENGTH];
	 	R2 += 0, carry;
	 	[R_SACM_Encode_Count_L] = R1;
		[R_SACM_Encode_Count_H] = R2;      
		retf;
.endif//AUDIO3200_Used
//add by benwang 20051220
//begin added by zgq on 20051220
.ifdef	DVR520_Used
L_DVR520_Encode_Process:	
		[R_SACM_DVR520_ADC_In_PTR_Encode]=R1;
		R2 = [R_SACM_Encode_Out_PTR];
		[R_ENCODE_OUT_LENGTH] = R2;
		call F_DVR520_Encode; //*R5: EncOut Buf Address*//

		R2 = [R_SACM_Encode_Out_PTR];
		R2 -= [R_ENCODE_OUT_LENGTH];
		[R_ENCODE_OUT_LENGTH] = R2;
        
	    R1 = [R_SACM_Encode_Count_L];
	 	R2 = [R_SACM_Encode_Count_H];
	 	R1 += [R_ENCODE_OUT_LENGTH];
	 	R2 += 0, carry;
	 	[R_SACM_Encode_Count_L] = R1;
		[R_SACM_Encode_Count_H] = R2;      
	retf;
.endif
///////////////////////////////////////////
.ifdef WAV_Used
L_WAV_Encode_Process:
		[R_SACM_DVR1800_ADC_In_PTR_Encode]=R1;
		R2 = [R_SACM_Encode_Out_PTR];
		[R_ENCODE_OUT_LENGTH] = R2;
		call F_Wav_Enc;
		[R_SACM_Encode_Out_PTR] = r1;
		R1 -= [R_ENCODE_OUT_LENGTH];
		
		[R_ENCODE_OUT_LENGTH] = R1;
        
	    R1 = [R_SACM_Encode_Count_L];
	 	R2 = [R_SACM_Encode_Count_H];
	 	R1 += [R_ENCODE_OUT_LENGTH];
	 	R2 += 0, carry;
	 	[R_SACM_Encode_Count_L] = R1;
		[R_SACM_Encode_Count_H] = R2;      
		retf;
.endif

////////////////////////////////////add by benwang 20060621
.ifdef	DVR1800_Used
L_DVR1800_Encode_Process:
		[R_SACM_DVR1800_ADC_In_PTR_Encode]=R1; //20060629
		R2 = [R_SACM_Encode_Out_PTR];
		[R_ENCODE_OUT_LENGTH] = R2;

		push r3,r4 to [sp]
		r3=0x4c
		r4=0x01
		call mr
		pop	r3,r4 from [sp]

		//call F_DVR1800_Encode;							//INPUT:R1;ADC_In_PTR
		R2 = [R_SACM_Encode_Out_PTR];
		R2 -= [R_ENCODE_OUT_LENGTH];
		[R_ENCODE_OUT_LENGTH] = R2;
        
	    R1 = [R_SACM_Encode_Count_L];
	 	R2 = [R_SACM_Encode_Count_H];
	 	R1 += [R_ENCODE_OUT_LENGTH];
	 	R2 += 0, carry;
	 	[R_SACM_Encode_Count_L] = R1;
		[R_SACM_Encode_Count_H] = R2;      
		retf;
.endif//DVR1800_Used
////////////////////////////////////added by benwang 20060621
.endp


//========================================================
// Function Name:	F_SACM_System_Get_BS_Manual
// Syntax:		call F_SACM_System_Get_BS_Manual; (in assembly domain)
// Purpose: 	manual mode kernal APIs                
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//======================================================== 
F_SACM_System_Get_BS_Manual: .proc
F_SACM_MP3_System_Get_BS_Manual:          //add by benwang 20060111
		
		R1 = [_R_DataEmptyFlag]
		cmp R1, -1
		je ?_Branch_2
		
		R1 = [R_SACM_Decode_In_PTR];
		cmp R1, R_SACM_Decode_In_Buffer;
		je ?_Branch_0;
		//cmp R1, R_SACM_Decode_In_Buffer + C_DECODE_IN_LENGTH; //del by benwang 20060209
		R2 = [R_DECODE_IN_LENGTH];								//add by benwang 20060209
		R2 += R_SACM_Decode_In_Buffer							//add by benwang 20060209		
		cmp R1, R2;												//add by benwang 20060209	
		jl ?_Branch_1;

		R1 = R_SACM_Decode_In_Buffer;
		[R_SACM_Decode_In_PTR] = R1;
		jmp ?_Branch_2;
		
?_Branch_1:
		R2 = R_SACM_Decode_In_Buffer;
		//R3 = R_SACM_Decode_In_Buffer + C_DECODE_IN_LENGTH;	//del by benwang 20060209	

//		R3 = [R_DECODE_IN_LENGTH];								//add by benwang 20060209
//		R3 += R_SACM_Decode_In_Buffer							//add by benwang 20060209				
		R3 = [_R_SACM_DecodeData_In_PTR]	//modify 2007/01/24 记录的是上次Decode_In_Buffer内得到数据的位置
		cmp R1, R3						//Add 2007/03/22当上次解码的长度已经大于本Buffer中的剩余长度时就不再搬移
		jae ?_Branch_Enough				//Add 2007/03/22
?_Loop_01:				//完成搬移的动作，因为前两个word是长度，而每次是从头开始取数据的
		R4 = [R1++];	//R1 = [R_SACM_Decode_In_PTR]
		[R2++] = R4;	//R2 = R_SACM_Decode_In_Buffer
		cmp R1, R3;
	    jne ?_Loop_01;
		[R_SACM_Decode_In_PTR] = R2;
?_Branch_2:
		R1 = [R_SACM_Decode_In_PTR];
		//R2 = R_SACM_Decode_In_Buffer + C_DECODE_IN_LENGTH;    //del by benwang 20060209
		R2 = [R_DECODE_IN_LENGTH];								//add by benwang 20060209
		R2 += R_SACM_Decode_In_Buffer							//add by benwang 20060209			
		
		R2 -= R1;
		call F_USER_GetData;
		// parameter: R1 = R_SACM_Decode_In_Buffer,
        //            R2 = C_DECODE_IN_LENGTH  
        // return with R_SACM_Decode_In_Buffer filled
//add for Speech Data read empty:
		[_R_DataEmptyFlag] = R1					//The value of R1 is the return length
        cmp R1, -1
        je ?_Branch_0							//If r1 = -1, it means get data failure.

        R2 = [R_SACM_Decode_In_PTR];
        R1 += R2
?_Branch_Enough:
        [_R_SACM_DecodeData_In_PTR] = R1	//store the decode data ptr, It equals end buffer normal

 		R1 = R_SACM_Decode_In_Buffer;
		[R_SACM_Decode_In_PTR] = R1;
?_Branch_0:		
 		retf;
.endp

//========================================================
// Function Name:	F_SACM_System_Put_BS_Manual
// Syntax:		call F_SACM_System_Put_BS_Manual; (in assembly domain)
// Purpose: 	manual mode kernal APIs                
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
F_SACM_System_Put_BS_Manual: .proc
		R1 = [R_SACM_Encode_Out_PTR];
		cmp R1, R_SACM_Encode_Out_Buffer;
		je ?_Branch_0;

		//R2 = [R_SACM_Resouce_BS];
	    //R3 = [R_SACM_Resouce_DS];
	 	R1 = R_SACM_Encode_Out_Buffer;
	 	R2 = [R_SACM_Encode_Out_PTR];
		R2 -= R1; 	
		call F_USER_WriteData							//Start Adr:R1; Length:R2  // write
		R1 = R_SACM_Encode_Out_Buffer;
		[R_SACM_Encode_Out_PTR] = R1;
?_Branch_0:		
 		retf;
.endp

//========================================================
// Function Name:	F_SACM_Encode_Initial_BS
// Syntax:		call F_SACM_Encode_Initial_BS; (in assembly domain)
// Purpose: 	manual mode kernal APIs                
// Note:		NO
// Parameter:	N/A
// Return: 		N/A
// Destory:		R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public F_SACM_Encode_Initial_BS
F_SACM_Encode_Initial_BS: .proc  
		R1 = 0x0000;
	 	[R_SACM_Encode_Count_L] = R1;
	 	[R_SACM_Encode_Count_H] = R1;
      
		R1 = [R_DataMode]
	 	jz ?SkipReserve
	 	R1 = [R_CODEC_TYPE]
		cmp R1, C_CODEC_WAV
		jz ?SkipReserve
		R1 = [R_SACM_Encode_Out_PTR];		//reserve 2 words for recording encode data length when stop
		R2 = 0;								//$
		[R1++] = R2;						//$
		[R1++] = R2;						//$
		[R_SACM_Encode_Out_PTR] = R1;		//reserve 2 words for recording encode data length when stop 		
?SkipReserve:	 
		retf;
.endp

//$$$$$$$$$$$$Must in SACM_XXXX_Decode_8kbps.asm$$$$$$$$$$$$$$$$$$$$$$$
//added by zgq on 2005/04/21
T_SACM_Res_Length:
.ifdef	S200_Used
	.dw offset L_S200_Length			//S200			0
.endif

.ifdef	S4872_Used
	.dw	offset L_SACM_Length			//S4872 			1
.endif

.ifdef	S530_Used
	.dw offset L_SACM_Length			//S530			2
.endif

.ifdef	S600TTS_Used
	.dw	offset L_MIDI_Length			//S600TTS		3	
.endif

.ifdef	S200TTS_Used
	.dw	offset L_SACM_Length			//S200TTS		4	
.endif	

.ifdef	S320TTS_Used
	.dw	offset L_SACM_Length			//S320TTS		5	
.endif	

.ifdef	MS01_Used
	.dw	offset L_MIDI_Length			//MS01			6	
.endif

.ifdef	MS02_Used
	.dw	offset L_MIDI_Length			//MS02			7	
.endif

.ifdef	SPEECH1600_Used
	.dw	offset L_SACM_Length			//SPEECH1600	8	
.endif

.ifdef	AUDIO1600_Used
	.dw	offset L_SACM_Length			//AUDIO1600		9	
.endif

.ifdef	SPEECH4800_Used
	.dw	offset L_SACM_Length			//SPEECH4800	10	
.endif

.ifdef	AUDIO4800_Used
	.dw	offset L_SACM_Length			//AUDIO4800		11	
.endif	

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw offset L_A6400S_Length	//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used			//add by benwang 20051220
	.dw	offset L_SACM_Length	//add by benwang 20051220	
.endif							//add by benwang 20051220
.ifdef DVR520_Used				//added by zgq on 20051220
	.dw offset L_SACM_Length	//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used					//added by benwang 20060111
	.dw offset L_MP3_Length		//added by benwang 20060111
.endif							//added by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used
	.dw	offset L_SACM_Length		
.endif

.ifdef A128_Used					//added by benwang 20060221
	.dw offset L_A128_Length		//added by benwang 20060221
.endif								//added by benwang 20060221

.ifdef	DVR1800_Used
	.dw	offset L_SACM_Length//L_DVR1800_Length			//20060629
.endif

.ifdef	WAV_Used
	.dw	offset L_WAV_Length			//added by chengye 2007/7/9
.endif

.ifdef	ETTS_Used
	.dw	0							//added by chengye 2007/4/29
.endif

.ifdef	S320_Used
	.dw	offset L_SACM_Length		//added by chengye 2007/4/29
.endif
/////////////////////////////////
///////////////////////////////////	
T_SACM_Dec_Init:
.ifdef	S200_Used
	.dw offset L_S200_Dec_Init			//S200			0
.endif

.ifdef	S4872_Used
	.dw	offset L_S4872_Dec_Init			//S4872	 		1
.endif

.ifdef	S530_Used
	.dw offset L_S530_Dec_Init			//S530			2
.endif

.ifdef	S600TTS_Used
	.dw offset L_S600TTS_Dec_Init		//S600TTS		3
.endif

.ifdef	S200TTS_Used
	.dw offset L_S200_Dec_Init			//S200TTS		4
.endif

.ifdef	S320TTS_Used
	.dw offset L_S320_Dec_Init			//S320TTS		5
.endif

.ifdef	MS01_Used
	.dw offset L_MS01_Dec_Init			//MS01			6
.endif

.ifdef	MS02_Used	
	.dw offset L_MS02_Dec_Init			//MS02			7
.endif

.ifdef	SPEECH1600_Used
	.dw offset L_A1600_Dec_Init			//SPEECH1600	8
.endif

.ifdef	AUDIO1600_Used
	.dw offset L_A1600_Dec_Init			//AUDIO1600		9
.endif

.ifdef	SPEECH4800_Used
	.dw offset L_A4800_Dec_Init			//SPEECH4800	10
.endif

.ifdef	AUDIO4800_Used
	.dw offset L_A4800_Dec_Init			//AUDIO4800		11
.endif	
	
.ifdef A6400S_Used				//added by zgq on 20051215
	.dw offset L_A6400S_Dec_Init//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used			//add by benwang 20051220
	.dw offset L_A3200_Dec_Init	//add by benwang 20051220
.endif							//add by benwang 20051220
.ifdef DVR520_Used				//added by zgq on 20051220
	.dw offset L_DVR520_Dec_Init	//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used					//add by benwang 20060111
	.dw offset L_MP3_Dec_Init	//add by benwang 20060111
.endif							//add by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used
	.dw offset L_A1601_Dec_Init		
.endif

.ifdef A128_Used				//add by benwang 20060221
	.dw offset L_A128_Dec_Init	//add by benwang 20060221
.endif							//add by benwang 20060221

.ifdef	DVR1800_Used
	.dw offset L_DVR1800_Dec_Init//added by benwang 20060621
.endif
.ifdef WAV_Used
	.dw offset L_WAV_Dec_Init	//added by chengye 2007/7/9
.endif

.ifdef	ETTS_Used
	.dw offset L_ETTS_Dec_Init	//added by chengye 2007/4/18
.endif

.ifdef	S320_Used
	.dw offset L_S320_Dec_Init	//added by chengye 2007/4/18
.endif
/////////////////////////////////////
////////////////////////////////////	
T_SACM_Dec_Proc:
.ifdef	S200_Used
	.dw offset L_S200_Dec_Proc			//S200			0
.endif

.ifdef	S4872_Used
	.dw	offset L_S4872_Dec_Proc			//S4872 			1
.endif

.ifdef	S530_Used	
	.dw	offset L_S530_Dec_Proc			//S530 			2
.endif

.ifdef	S600TTS_Used
	.dw	offset L_S600TTS_Dec_Proc		//S600TTS		3
.endif

.ifdef	S200TTS_Used	
	.dw	offset L_S200_Dec_Proc			//S200TTS		4
.endif	

.ifdef	S320TTS_Used
	.dw	offset L_S320_Dec_Proc			//S320TTS		5
.endif

.ifdef	MS01_Used
	.dw	offset L_MIDI_Dec_Proc			//MS01 			6
.endif

.ifdef	MS02_Used
	.dw	offset L_MIDI_Dec_Proc			//MS02			7
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset L_A1600_Dec_Proc			//SPEECH1600	8
.endif

.ifdef	AUDIO1600_Used
	.dw	offset L_A1600_Dec_Proc			//AUDIO1600		9
.endif

.ifdef	SPEECH4800_Used	
	.dw	offset L_A4800_Dec_Proc			//SPEECH4800	10
.endif

.ifdef	AUDIO4800_Used
	.dw	offset L_A4800_Dec_Proc			//AUDIO4800		11
.endif	

.ifdef A6400S_Used					//added by zgq on 20051215
	.dw offset L_A6400S_Dec_Proc	//added by zgq on 20051215
.endif								//added by zgq on 20051215

.ifdef AUDIO3200_Used				//add by benwang 20051220
	.dw	offset L_A3200_Dec_Proc		//add by benwang 20051220
.endif								//add by benwang 20051220

.ifdef DVR520_Used					//added by zgq on 20051220
	.dw offset L_DVR520_Dec_Proc		//added by zgq on 20051220
.endif								//added by zgq on 20051220

.ifdef MP3_Used						//add by benwang 20060111
	.dw	offset L_MP3_Dec_Proc		//add by benwang 20060111
.endif								//add by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used
	.dw	offset L_A1601_Dec_Proc			
.endif

.ifdef A128_Used					//add by benwang 20060221
	.dw	offset L_A128_Dec_Proc		//add by benwang 20060221
.endif								//add by benwang 20060221

.ifdef	DVR1800_Used
	.dw	offset L_DVR1800_Dec_Proc   //added by benwang 20060621
.endif
.ifdef	WAV_Used
	.dw	offset L_WAV_Dec_Proc		//added by chengye 2007/7/9  
.endif

.ifdef	ETTS_Used
	.dw	0   						//added by chengye 2007/4/29
.endif

.ifdef	S320_Used
	.dw	offset L_S320_Dec_Proc   	//added by chengye 2007/4/29
.endif
/////////////////////////////////////////
////////////////////////////////////////
T_SACM_Dec_Proc_1:
.ifdef	S200_Used
	.dw offset 	Chk_Dec_Length_End		//S200  		0
.endif

.ifdef	S4872_Used
	.dw	offset 	Chk_Dec_Length_End		//S4872 			1
.endif

.ifdef	S530_Used
	.dw offset	Chk_S530_Length			//S530			2
.endif

.ifdef	S600TTS_Used	
	.dw	offset	Chk_S600TTS_Length		//S600TTS		3
.endif

.ifdef	S200TTS_Used	
	.dw	offset 	Chk_Dec_Length_End		//S200TTS		4
.endif

.ifdef	S320TTS_Used	
	.dw	offset	Chk_S320_Length			//S320TTS		5
.endif

.ifdef	MS01_Used
	.dw	offset  Chk_MIDI_Length			//MS01			6
.endif

.ifdef	MS02_Used	
	.dw	offset  Chk_MIDI_Length			//MS02			7
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset 	Chk_Dec_Length_End		//SPEECH1600	8
.endif

.ifdef	AUDIO1600_Used	
	.dw	offset 	Chk_Dec_Length_End		//AUDIO1600		9
.endif

.ifdef	SPEECH4800_Used	
	.dw	offset 	Chk_Dec_Length_End		//SPEECH4800	10
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset 	Chk_Dec_Length_End		//AUDIO4800		11
.endif		

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw offset Chk_Dec_Length_End//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used			//add by benwang 20051220
	.dw	offset 	Chk_Dec_Length_End	//add by benwang 20051220	
.endif							//add by benwang 20051220
.ifdef	DVR520_Used					//added by zgq on 20051220
	.dw	offset 	Chk_Dec_Length_End	//added by zgq on 20051220		
.endif								//added by zgq on 20051220

.ifdef MP3_Used						//add by benwang 20060111
	//.dw	offset 	Chk_Dec_Length_End	//add by benwang 20060111 //del by benwang 20060209	
	.dw offset Chk_MP3_Length           //add by benwang 20060209  
.endif								//add by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset 	Chk_Dec_Length_End		
.endif

.ifdef A128_Used						//add by benwang 20060221
	.dw offset Chk_A128_Length          //add by benwang 20060221  
.endif									//add by benwang 20060221

.ifdef	DVR1800_Used	
	.dw	offset 	Chk_Dec_Length_End	
.endif
.ifdef	WAV_Used	
	.dw	offset 	Chk_Dec_Length_End	//added by chengye 2007/7/9	
.endif

.ifdef	ETTS_Used	
	.dw	0							//added by chengye 2007/4/28
.endif

.ifdef	S320_Used	
	.dw	offset 	Chk_S320_Length		//added by chengye 2007/4/28
.endif

/////////////////////////////////
///////////////////////////////
T_SACM_Dec_Proc_2:
.ifdef	S200_Used
	.dw offset 	L_Not_MIDI_Dec_Proc		//S200  		0
.endif

.ifdef	S4872_Used
	.dw	offset 	L_Not_MIDI_Dec_Proc		//S4872 			1
.endif

.ifdef	S530_Used
	.dw offset	L_Not_MIDI_Dec_Proc		//S530			2
.endif

.ifdef	S600TTS_Used	
	.dw	offset	L_Not_MIDI_Dec_Proc		//S600TTS		3
.endif

.ifdef	S200TTS_Used	
	.dw	offset 	L_Not_MIDI_Dec_Proc		//S200TTS		4
.endif

.ifdef	S320TTS_Used	
	.dw	offset	L_Not_MIDI_Dec_Proc		//S320TTS		5
.endif

.ifdef	MS01_Used
	.dw	offset  L_MS01_Dec_Proc			//MS01			6
.endif

.ifdef	MS02_Used	
	.dw	offset  L_MS02_Dec_Proc			//MS02			7
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset 	L_Not_MIDI_Dec_Proc		//SPEECH1600	8
.endif

.ifdef	AUDIO1600_Used	
	.dw	offset 	L_Not_MIDI_Dec_Proc		//AUDIO1600		9
.endif

.ifdef	SPEECH4800_Used	
	.dw	offset 	L_Not_MIDI_Dec_Proc		//SPEECH4800	10
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset 	L_Not_MIDI_Dec_Proc		//AUDIO4800		11
.endif		

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw offset L_Not_MIDI_Dec_Proc//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used			//add by benwang 20051220
	.dw	offset 	L_Not_MIDI_Dec_Proc	//add by benwang 20051220	
.endif							//add by benwang 20051220

.ifdef DVR520_Used				//added by zgq on 20051220
	.dw offset L_Not_MIDI_Dec_Proc//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used						//add by benwang 20060111
	.dw	offset 	L_Not_MIDI_Dec_Proc	//add by benwang 20060111	
.endif								//add by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset 	L_Not_MIDI_Dec_Proc		
.endif

.ifdef A128_Used					//add by benwang 20060221
	.dw	offset 	L_Not_MIDI_Dec_Proc	//add by benwang 20060221	
.endif								//add by benwang 20060221

.ifdef DVR1800_Used	
	.dw	offset 	L_Not_MIDI_Dec_Proc
.endif
.ifdef WAV_Used	
	.dw	offset 	L_Not_MIDI_Dec_Proc	//added by chengye 2007/7/9	
.endif

.ifdef ETTS_Used	
	.dw	offset 	L_ETTS_Dec_Proc		//add by chengye 2007/4/18
.endif

.ifdef	S320_Used	
	.dw	offset	L_Not_MIDI_Dec_Proc		//add by chengye 2007/4/28
.endif
////////////////////////////////////////
///////////////////////////////////////
T_SACM_Encode_Pro:
.ifdef	S200_Used
	.dw offset 	L_Not_Encode_Proc		//S200  		0
.endif

.ifdef	S4872_Used
	.dw	offset 	L_Not_Encode_Proc		//S4872 			1
.endif

.ifdef	S530_Used
	.dw offset	L_Not_Encode_Proc		//S530			2
.endif

.ifdef	S600TTS_Used	
	.dw	offset	L_Not_Encode_Proc		//S600TTS		3
.endif

.ifdef	S200TTS_Used	
	.dw	offset 	L_Not_Encode_Proc		//S200TTS		4
.endif

.ifdef	S320TTS_Used	
	.dw	offset	L_Not_Encode_Proc		//S320TTS		5
.endif

.ifdef	MS01_Used
	.dw	offset  L_Not_Encode_Proc		//MS01			6
.endif

.ifdef	MS02_Used	
	.dw	offset  L_Not_Encode_Proc		//MS02			7
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset 	L_A1600_Encode_Process		//SPEECH1600	8
.endif

.ifdef	AUDIO1600_Used	
	.dw	offset 	L_Not_Encode_Proc		//AUDIO1600		9
.endif

.ifdef	SPEECH4800_Used	
	.dw	offset 	L_A4800_Encode_Process		//SPEECH4800	10
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset 	L_A4800_Encode_Process		//AUDIO4800		11
.endif

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw	offset 	L_Not_Encode_Proc//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef	AUDIO3200_Used			//add by benwang 20051220
	.dw	offset 	L_A3200_Encode_Process	//add by benwang 20051220
.endif							//add by benwang 20051220
.ifdef	DVR520_Used						//added by zgq on 20051220	
	.dw offset L_DVR520_Encode_Process	//added by zgq on 20051220
.endif									//added by zgq on 20051220

//add by benwang 20060119
.ifdef	MP3_Used	
	.dw	offset 	L_Not_Encode_Proc		
.endif

.ifdef	AUDIO1601_Used	
	.dw	offset 	L_Not_Encode_Proc		
.endif

.ifdef A128_Used						//added by benwang 20060221
	.dw	offset 	L_Not_Encode_Proc	//added by benwang 20060221
.endif

.ifdef	DVR1800_Used	
	.dw	offset 	L_DVR1800_Encode_Process//added by benwang 20060621	
.endif		

.ifdef	WAV_Used	
	.dw	offset 	L_WAV_Encode_Process	//added by chengye 2007/7/9		
.endif

///////////////////////////////////
//////////////////////////////////
T_SACM_Pro_Init:
.ifdef	S200_Used
	.dw offset 	L_SACM_Not_Pro_Init		//S200  		0
.endif

.ifdef	S4872_Used
	.dw	offset 	L_SACM_Not_Pro_Init		//S4872 			1
.endif

.ifdef	S530_Used
	.dw offset	L_SACM_Not_Pro_Init		//S530			2
.endif

.ifdef	S600TTS_Used	
	.dw	offset	L_SACM_Not_Pro_Init		//S600TTS		3
.endif

.ifdef	S200TTS_Used	
	.dw	offset 	L_SACM_Not_Pro_Init		//S200TTS		4
.endif

.ifdef	S320TTS_Used	
	.dw	offset	L_SACM_Not_Pro_Init		//S320TTS		5
.endif

.ifdef	MS01_Used
	.dw	offset  L_SACM_Not_Pro_Init		//MS01			6
.endif

.ifdef	MS02_Used	
	.dw	offset  L_SACM_Not_Pro_Init		//MS02			7
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset 	L_SACM_DVR1600_Pro_Init		//SPEECH1600	8
.endif

.ifdef	AUDIO1600_Used	
	.dw	offset 	L_SACM_Not_Pro_Init		//AUDIO1600		9
.endif

.ifdef	SPEECH4800_Used	
	.dw	offset 	L_SACM_DVR4800_Pro_Init		//SPEECH4800	10
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset 	L_SACM_DVR4800_Pro_Init		//AUDIO4800		11
.endif	

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw	offset 	L_SACM_Not_Pro_Init		//added by zgq on 20051215
.endif							//added by zgq on 20051215
	
.ifdef	AUDIO3200_Used			//add by benwang 20051220
	.dw	offset 	L_SACM_DVR3200_Pro_Init	//add by benwang 20051220
.endif							//add by benwang 20051220

.ifdef	DVR520_Used						//added by zgq on 20051220
	.dw offset L_SACM_DVR520_Pro_Init	//added by zgq on 20051220
.endif									//added by zgq on 20051220

//add by benwang 20060119

.ifdef	MP3_Used	
	.dw	offset 	L_SACM_Not_Pro_Init		
.endif	

.ifdef	AUDIO1601_Used	
	.dw	offset 	L_SACM_Not_Pro_Init		
.endif	
		
.ifdef	A128_Used	
	.dw	offset 	L_SACM_Not_Pro_Init		
.endif	

.ifdef	DVR1800_Used	
	.dw	offset 	L_SACM_DVR1800_Pro_Init		//added by benwang 20060621
.endif
.ifdef	WAV_Used	
	.dw	offset 	L_SACM_DVRwave_Pro_Init			//added by chengye 2007/7/9	
.endif	
		
////////////////////////////
T_SACM_Encode_Finish_Proc:
.ifdef	S200_Used
	.dw offset 	L_SACM_Not_Finish_Pro		//S200  		0
.endif

.ifdef	S4872_Used
	.dw	offset 	L_SACM_Not_Finish_Pro		//S4872 			1
.endif

.ifdef	S530_Used
	.dw offset	L_SACM_Not_Finish_Pro		//S530			2
.endif

.ifdef	S600TTS_Used	
	.dw	offset	L_SACM_Not_Finish_Pro		//S600TTS		3
.endif

.ifdef	S200TTS_Used	
	.dw	offset 	L_SACM_Not_Finish_Pro		//S200TTS		4
.endif

.ifdef	S320TTS_Used	
	.dw	offset	L_SACM_Not_Finish_Pro		//S320TTS		5
.endif

.ifdef	MS01_Used
	.dw	offset  L_SACM_Not_Finish_Pro		//MS01			6
.endif

.ifdef	MS02_Used	
	.dw	offset  L_SACM_Not_Finish_Pro		//MS02			7
.endif

.ifdef	SPEECH1600_Used	
	.dw	offset 	L_DVR1600_Finish_Pro		//SPEECH1600	8
.endif

.ifdef	AUDIO1600_Used	
	.dw	offset 	L_SACM_Not_Finish_Pro		//AUDIO1600		9
.endif

.ifdef	SPEECH4800_Used	
	.dw	offset 	L_DVR4800_Finish_Pro		//SPEECH4800	10
.endif

.ifdef	AUDIO4800_Used	
	.dw	offset 	L_DVR4800_Finish_Pro		//AUDIO4800		11
.endif	

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw	offset 	L_SACM_Not_Finish_Pro	//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef	AUDIO3200_Used			//add by benwang 20051220
	.dw	offset 	L_DVR3200_Finish_Pro //add by benwang 20051220
.endif							//add by benwang 20051220

.ifdef DVR520_Used						//added by zgq on 20051220
	.dw	offset 	L_DVR520_Finish_Pro		//added by zgq on 20051220
.endif									//added by zgq on 20051220

.ifdef MP3_Used							//added by benwang 20060111
	.dw	offset 	L_SACM_Not_Finish_Pro	//added by benwang 20060111
.endif									//added by benwang 20060111
//add by benwang 20060119
.ifdef	AUDIO1601_Used	
	.dw	offset 	L_SACM_Not_Finish_Pro		
.endif

.ifdef A128_Used						//added by benwang 20060221
	.dw	offset 	L_SACM_Not_Finish_Pro	//added by benwang 20060221
.endif									//added by benwang 20060221

.ifdef	DVR1800_Used	
	.dw	offset 	L_DVR1800_Finish_Pro
.endif

.ifdef	WAV_Used	
	.dw	offset 	L_WAV_Finish_Pro	//added by chengye 2007/7/9	
.endif

/////////////////////////////////////////////	
//////////////////////////////////////////////	
