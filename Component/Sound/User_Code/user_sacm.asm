//========================================================================================
// Progarm: SACM user.asm for SACM_DVR1600
// By: Arthur Shieh
// Last modified date: 
//      2005/03/01: V1.0:First release for SACM DVR1600   
//
// Functions:
//  	F_USER_XXXX_GetData
//  
// I/O APIs:
//  _USER_XXXX_SetStartAddr
//
// Note:1. In the demo, it shows a way to fetch ROM data to demonstrate 
//         the manual mode operation. It fetchs data from T_SACM_S200_SpeechTable
//         and feeds back to library for playback. 
//========================================================================================
.include .\System\Include\System\GPL162002.inc 

.include .\Component\Include\speech\algorithm.inc

//.define DumpEncodeData			//add for debug 2006/12/19

.public F_USER_EndRecord
.public F_USER_GetData
.public F_USER_WriteData
.public USER_SetRECStartAddr
.public F_SACM_MS02_SongEvent
.public _USER_SetMemoryAddr			//added by chengye 2007/6/15
.public _USER_SetStartAddr
.ifdef MP3_Used
.public _USER_SetMP3StartAddr		//added by chengye 2007/4/18
.public _USER_SetMP3MemoryAddr		//added by chengye 2008/1/17
.endif //mp3 Used
.public _USER_SetRECStartAddr
.public _User_SetStartAddrManual					//xiaolei add for Play when set the start address
.public _Snd_onAmpOff
.public _Snd_onAmpOn

//add by zhangxh 2006/07/31
.public _Fat_SetStartAddr
.ifdef MP3_Used 
.public _USER_SetFSLength
.endif
.public _MP3_DecodeCount_Resume		//added by chengye 2007/4/23
//end 

// Speech Table
.external R_CODEC_TYPE
.external R_SACM_ISR_Flag
.external _R_ISR_DataProc_Flag		//added by chengye 2007/4/24	
.external R_BITRATE

.RAM
.public EXT_SRAM_Start_DS			
.public EXT_SRAM_Start_BS 

.var EXT_SRAM_Start_DS			
.var EXT_SRAM_Start_BS 					//0:Memory, 1:Source

.public R_SACM_Resouce_BS, R_SACM_Resouce_DS
.public R_DataMode
.var	R_SACM_Resouce_BS
.var	R_SACM_Resouce_DS
.var	R_DataMode

.ifdef A6400S_Used	 //added by benwang 20060104
.public R_SACM_A6400S_Resource_END_BS, R_SACM_A6400S_Resource_END_DS	//only for A6400S added by zgq on 20051215
.var 	R_SACM_A6400S_Resource_END_BS, R_SACM_A6400S_Resource_END_DS	//only for A6400S added by zgq on 20051215
.public R_SACM_A6400S_Resource_BS, R_SACM_A6400S_Resource_DS //only for A6400S added by benwang 20060104
.var R_SACM_A6400S_Resource_BS, R_SACM_A6400S_Resource_DS //only for A6400S added by benwang 20060104
.endif  //added by benwang 20060104

.ifdef MP3_Used	 //added by benwang 20060111
//.public R_SACM_MP3_Resource_END_BS, R_SACM_MP3_Resource_END_DS	//added by benwang 20060111
//.var 	R_SACM_MP3_Resource_END_BS, R_SACM_MP3_Resource_END_DS	//added by benwang 20060111
//.public R_SACM_MP3_Resource_BS, R_SACM_MP3_Resource_DS //added by benwang 20060111
//.var R_SACM_MP3_Resource_BS, R_SACM_MP3_Resource_DS //added by benwang 20060111
.public _dacoutlength
.public R_SACM_MP3_Length_L							//add by jacky 2006.02.14
.public R_SACM_MP3_Length_H							//add by jacky 2006.02.14

.var	_dacoutlength								//记录DAC_OUT_Length
.var	R_SACM_MP3_Length_L							//add by jacky 2006.02.14
.var	R_SACM_MP3_Length_H							//add by jacky 2006.02.14
.var	R_OLDMP3FrameCount_L, R_OLDMP3FrameCount_H
.endif  //added by benwang 20060111

.ifdef A128_Used	 //added by benwang start 20060221
//.public R_SACM_A128_Resource_END_BS, R_SACM_A128_Resource_END_DS	
//.var 	R_SACM_A128_Resource_END_BS, R_SACM_A128_Resource_END_DS	
//.public R_SACM_A128_Resource_BS, R_SACM_A128_Resource_DS 
//.var R_SACM_A128_Resource_BS, R_SACM_A128_Resource_DS 

.public R_SACM_A128_Length_L							//add by jacky 2006.02.14
.public R_SACM_A128_Length_H							//add by jacky 2006.02.14
.var	R_SACM_A128_Length_L							//add by jacky 2006.02.14
.var	R_SACM_A128_Length_H							//add by jacky 2006.02.14

.public _R_BitRate_A128
.var _R_BitRate_A128

.endif  //added by benwang end 20060221


.ifdef	S600TTS_Used
.public R_Syllable_Num 		//for S600TTS
.var R_Syllable_Num			//for S600TTS
.endif

.ifdef DumpEncodeData
.public EncodeDataAddrH,EncodeDataAddrL
.var EncodeDataAddrH,EncodeDataAddrL
.endif

User_SACM:	.section .CODE
//========================================================
// Function Name:	F_SACM_EndRecord
// Syntax:		F_SACM_EndRecord
// Purpose: 	call back from kernel when bit stream 
//              encoding is done 
// Note:		NO	
// Parameter : 	R1: File length(bytes) low
//              R2: File length(bytes) high
// Return:		N/A
// Destory: 	R1,R2
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
F_USER_EndRecord: .proc				//Get file length R1:Low,R2:High
		push R2 to [SP];					//High
		push R1 to [SP];					//Low
		R1 = [EXT_SRAM_Start_BS]				
		r2 = [EXT_SRAM_Start_DS]		
		r1-=2
		r2-=0,carry
		ds=r2	    
	    pop R2 from [SP];//Low	    
	    // INPUT:R1:address, R2:DATA  | OUTPUT : R1:address+1,R2:0
		DS:[R1++] = R2						// added by Anson 2004/2/2
		pop R2 from [SP];					// High 
		
		//INPUT:R1:address, R2:DATA  | OUTPUT : R1:address+1,R2:0
		DS:[R1] = R2						// added by Anson 2004/2/2
		retf;
.endp



//========================================================
// Procedure:	_USER_SetMemoryAddr
// Syntax:	USER_SetMemoryAddr(long)
// Parameter:	R1 = Data pointer (long) 
// Return:	NONE
// Description:	This API allows users to set the beginning address 
//                to fetch data. This address can be either a ROM address
//                or a external storage address. User would have to modify
//                the function body based on the application's need.    
//========================================================
_USER_SetMemoryAddr:
	push R1, R2 to [sp];
	R2 = 0;
	[R_DataMode] = R2;

   	R2 = SP + 5;

	R1 = [R2++];
	[R_SACM_Resouce_BS] = R1;
	R1 = [R2];
	[R_SACM_Resouce_DS] = R1;

	pop R1, R2 from [sp];
	retf

//========================================================
// Function Name:	_USER_SetMP3StartAddr
// Syntax:		USER_SetMP3StartAddr(int index)
// Purpose: 	This API allows users to set the beginning address of MP3
//              to fetch data. This address can be either a ROM address
//              or a external storage address. User would have to modify
//              the function body based on the application's need.
// Note:		NO	
// Parameter : 	R1 = PlayIndex
// Return:		N/A
// Destory: 	R1~R3
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.ifdef MP3_Used	
.external T_SACM_MP3_MusicTable	
_USER_SetMP3StartAddr:						
		r3 = sp + 3
		r1 = [r3]
		
		R2 = 0;
		[R_DataMode] = R2;				//Get the datas from Memory
		
		r1 = r1 lsl 2;		
		R1 += T_SACM_MP3_MusicTable;
		r2 = [r1++];
		[R_SACM_Resouce_BS] = r2;
		r2 = [r1++];
		[R_SACM_Resouce_DS] = r2;
		r2 = [r1++];					//end low addr	
		r1 = [r1];						//end high addr	
		r2-=[R_SACM_Resouce_BS]
		r1-=[R_SACM_Resouce_DS], carry		
		[R_SACM_MP3_Length_L]=r2
		[R_SACM_MP3_Length_H]=r1		

		retf;				 

//========================================================
// Procedure:	_USER_SetMP3MemoryAddr
// Syntax:	_USER_SetMP3MemoryAddr(long start_addr, long end_addr)
// Parameter:	 long start_addr, long dest_addr
// Return:	NONE
// Description:	This API allows users to set the beginning address 
//                to fetch data. This address can be either a ROM address
//                or a external storage address. User would have to modify
//                the function body based on the application's need.    
//========================================================
_USER_SetMP3MemoryAddr:
	push R1, R5 to [sp]
	
	R1 = 0;				
	[R_DataMode] = R1;
	
	R1 = SP + 8
	R4 = [R1++]				//The BS of sa
	R3 = [R1++]				//The DS of sa
	R2 = [R1++]				//The BS of ea
	R1 = [R1]				//The DS of ea

	[R_SACM_Resouce_BS] = r4
	[R_SACM_Resouce_DS] = r3
	r2-=[R_SACM_Resouce_BS]
	r1-=[R_SACM_Resouce_DS], carry

//	R3 = R3 LSL 4;
//	R3 = R3 LSL 4;
//	R3 = R3 LSL 2;
//	[R_SACM_Resouce_DS] = R3;
		
	[R_SACM_MP3_Length_L]=r2
	[R_SACM_MP3_Length_H]=r1

	pop R1, R5 from [sp]
	retf

.endif

.ifdef A128_Used
//--------------------------------------------------------------------
//-- Procedure: _USER_SetA128StartAddr
//-- Syntax: USER_SetA128StartAddr(int)
//-- Parameter: R1 = PlayIndex 
//-- Return: NONE
//-- Description: This API allows users to set the beginning address of A128
//                to fetch data. This address can be either a ROM address
//                or a external storage address. User would have to modify
//                the function body based on the application's need.    
//--------------------------------------------------------------------
.public _USER_SetA128StartAddr	
.external T_SACM_A128_MusicTable	
_USER_SetA128StartAddr:					//added by chengye 2007/4/18			
		r3 = sp + 3
		r1 = [r3]
		
		R2 = 0;
		[R_DataMode] = R2;				//Get the datas from Memory

		r1 = r1 lsl 2;
		r2 = SEG16 T_SACM_A128_MusicTable
		DS = r2
		R1 += OFFSET T_SACM_A128_MusicTable;
		r2 = DS:[r1++];
		[R_SACM_Resouce_BS] = r2;
		r2 = DS:[r1++];
		[R_SACM_Resouce_DS] = r2;
		r2=DS:[r1++]
		r1=DS:[r1]
		r2-=[R_SACM_Resouce_BS]
		r1-=[R_SACM_Resouce_DS], carry
		[R_SACM_A128_Length_L]=r2
		[R_SACM_A128_Length_H]=r1

		retf;
		
//========================================================
// Procedure:	_USER_SetA128MemoryAddr
// Syntax:	_USER_SetA128MemoryAddr(long start_addr, long end_addr)
// Parameter:	 long start_addr, long dest_addr
// Return:	NONE
// Description:	This API allows users to set the beginning address 
//                to fetch data. This address can be either a ROM address
//                or a external storage address. User would have to modify
//                the function body based on the application's need.    
//========================================================
.public _USER_SetA128MemoryAddr
_USER_SetA128MemoryAddr:
	push R1, R5 to [sp]
	
	R1 = 0;				
	[R_DataMode] = R1;
	
	R1 = SP + 8
	R4 = [R1++]				//The BS of sa
	R3 = [R1++]				//The DS of sa
	R2 = [R1++]				//The BS of ea
	R1 = [R1]				//The DS of ea

	[R_SACM_Resouce_BS] = r4
	[R_SACM_Resouce_DS] = r3
	r2-=[R_SACM_Resouce_BS]
	r1-=[R_SACM_Resouce_DS], carry

//	R3 = R3 LSL 4;
//	R3 = R3 LSL 4;
//	R3 = R3 LSL 2;
//	[R_SACM_Resouce_DS] = R3;
		
	[R_SACM_A128_Length_L]=r2
	[R_SACM_A128_Length_H]=r1

	pop R1, R5 from [sp]
	retf

.endif		
		
.endif

//--------------------------------------------------------------------
//-- Procedure: _USER_SetStartAddr
//-- Syntax: USER_SetStartAddr(int)
//-- Parameter: R1 = PlayIndex 
//-- Return: NONE
//-- Description: This API allows users to set the beginning address 
//                to fetch data. This address can be either a ROM address
//                or a external storage address. User would have to modify
//                the function body based on the application's need.    
//--------------------------------------------------------------------
_USER_SetStartAddr:
	    //-----------------------
	    // defined by user
	    //-----------------------
//.comment @
		push	r2,r3 to [sp]
		
		R2 = 0;							//Get the datas from Memory
		[R_DataMode] = R2;				//xiaolei add 2006/11/15

		r2=sr&0x03f
		ds=r2
		R2 = [R_CODEC_TYPE];
		r2 =r2+ T_SACM_Speech_Table_M;
		pc=ds:[r2]
		
.ifdef	S200_Used
.external	T_SACM_S200_SpeechTable
L_S200_Res_M:
		r1 = r1 lsl 1
		r2 = SEG16 T_SACM_S200_SpeechTable
		DS = r2
		R1 += OFFSET T_SACM_S200_SpeechTable;
		goto L_Continue_0;	
.endif

.ifdef	S4872_Used
.external	T_SACM_S4872_SpeechTable
L_S4872_Res_M:
		r1 = r1 lsl 1
		r2 = SEG16 T_SACM_S4872_SpeechTable
		DS = r2
		R1 += OFFSET T_SACM_S4872_SpeechTable;
		jmp L_Continue_0;	
.endif

.ifdef	S530_Used
.external	T_SACM_S530_SpeechTable
L_S530_Res_M:
		r1 = r1 lsl 1;
		r2 = SEG16 T_SACM_S530_SpeechTable
		DS = r2
		R1 += OFFSET T_SACM_S530_SpeechTable;
		goto L_Continue_0;	
.endif

.ifdef	S600TTS_Used
.external T_DIC_Table 		//for S600TTS
.external T_Syllable_Table 	//for S600TTS
L_S600TTS_Res_M:
//		R2 = R1;                    
//		R2 += T_Syllable_Table;     
//		R2 = [R2];                  
//		[R_Syllable_Num] = R2;      
//		
//		R1 += T_DIC_Table;			
		jmp L_Continue_0;	
.endif

.ifdef	S200TTS_Used
L_S200TTS_Res_M:
		nop
		jmp L_Continue_0;	
.endif

.ifdef	S320TTS_Used
.external T_SACM_TTS320_DIC_Table 
L_S320TTS_Res_M:
		r1 = r1 lsl 1;
		r2 = SEG16 T_SACM_TTS320_DIC_Table
		DS = r2
		R1 += OFFSET T_SACM_TTS320_DIC_Table;
		goto L_Continue_0;	
.endif

.ifdef	MS01_Used
.external	T_SACM_MS01_SongTable
L_MS01_Res_M:
		r1 = r1 lsl 1;
		r2 = SEG16 T_SACM_MS01_SongTable
		DS = r2
		R1 += OFFSET T_SACM_MS01_SongTable;
		goto L_Continue_0;	
.endif

.ifdef	MS02_Used
.external	T_SACM_MS02_SpeechTable
L_MS02_Res_M:
		r1 = r1 lsl 1;	
		r2 = SEG16 T_SACM_MS02_SpeechTable
		DS = r2
		R1 += OFFSET T_SACM_MS02_SpeechTable;
		goto L_Continue_0;	
.endif

.ifdef	SPEECH1600_Used
.external	T_SACM_SPEECH1600_SpeechTable
L_SPEECH1600_Res_M:
		r1 = r1 lsl 1;	
		r2 = SEG16 T_SACM_SPEECH1600_SpeechTable
		DS = r2
		R1 += OFFSET T_SACM_SPEECH1600_SpeechTable;
		goto L_Continue_0;	
.endif

.ifdef AUDIO1600_Used
.external	T_SACM_AUDIO1600_SpeechTable
L_AUDIO1600_Res_M:
		r1 = r1 lsl 1;	
		r2 = SEG16 T_SACM_AUDIO1600_SpeechTable	
		DS = r2		
		R1 += OFFSET T_SACM_AUDIO1600_SpeechTable;
		jmp L_Continue_0;	
.endif

.ifdef	SPEECH4800_Used
.external	T_SACM_SPEECH4800_SpeechTable
L_SPEECH4800_Res_M:
		r1 = r1 lsl 1;	
		r2 = SEG16 	T_SACM_SPEECH4800_SpeechTable
		DS = r2			
		R1 += OFFSET T_SACM_SPEECH4800_SpeechTable;
		jmp L_Continue_0;	
.endif

.ifdef	AUDIO4800_Used
.external	T_SACM_AUDIO4800_SpeechTable	
L_AUDIO4800_Res_M:
		r1 = r1 lsl 1;		
		r2 = SEG16 T_SACM_AUDIO4800_SpeechTable
		DS = r2
		R1 += OFFSET T_SACM_AUDIO4800_SpeechTable;
		goto L_Continue_0;	
.endif

.ifdef A6400S_Used	
.external T_Music_Table		
L_A6400S_Res_M:
//		r1 = r1 lsl 1;						//added by zgq on 20051215
//		R1 += T_Music_Table;				//added by zgq on 20051215
//		r3 = [r1++];							//added by zgq on 20051215
//		r2 = [r3++];						//added by zgq on 20051215
//		[R_SACM_Resouce_BS] = r2;			//added by zgq on 20051215
//		[R_SACM_A6400S_Resource_BS] = r2;   //add by benwang 20060104
//		r2 = [r3++];						//added by zgq on 20051215
//		[R_SACM_Resouce_DS] = r2;			//added by zgq on 20051215
//		[R_SACM_A6400S_Resource_DS] = r2;   //add by benwang 20060104
//		r1=[r1++]
//		r2 = [r1++];						//added by zgq on 20051215
//		[R_SACM_A6400S_Resource_END_BS] = r2;//added by zgq on 20051215
//		r2 = [r1++];						//added by zgq on 20051215
//		[R_SACM_A6400S_Resource_END_DS] = r2;//added by zgq on 20051215
//			
//		pop R2,r3 from [sp];					//added by zgq on 20051215
		retf;								//added by zgq on 20051215
.endif										//added by zgq on 20051215

.ifdef	AUDIO3200_Used  
.external	T_SACM_AUDIO3200_SpeechTable    
L_AUDIO3200_Res_M:						
		r1 = r1 lsl 1;
		r2 = SEG16 T_SACM_AUDIO3200_SpeechTable	
		DS = r2			
		R1 += OFFSET T_SACM_AUDIO3200_SpeechTable;
		jmp L_Continue_0;				  	
.endif											
	
.ifdef DVR520_Used						
.external T_SACM_DVR520_SpeechTable		
L_DVR520_Res_M:								
		r1 = r1 lsl 1;
		r2 = SEG16 T_SACM_DVR520_SpeechTable
		DS = r2					
		R1 += OFFSET T_SACM_DVR520_SpeechTable;	
		jmp L_Continue_0;				
.endif									

.ifdef MP3_Used						
.external T_SACM_MP3_MusicTable			
L_MP3_Res_M:						
		r1 = r1 lsl 2;	
		r2 = SEG16 T_SACM_MP3_MusicTable
		DS = r2			
		R1 += OFFSET T_SACM_MP3_MusicTable;	
		r2 = DS:[r1++];					
		[R_SACM_Resouce_BS] = r2;		
		r2 = DS:[r1++];						
		[R_SACM_Resouce_DS] = r2;		
		r2 = DS:[r1++];						//end low addr	
		r1 = DS:[r1];						//end high addr	
		r2-=[R_SACM_Resouce_BS]
		r1-=[R_SACM_Resouce_DS], carry		
		[R_SACM_MP3_Length_L]=r2
		[R_SACM_MP3_Length_H]=r1		
		pop R2,r3 from [sp];				
		retf;							
.endif									

.ifdef	AUDIO1601_Used
.external	T_SACM_A1601_SpeechTable
L_AUDIO1601_Res_M:
		r1 = r1 lsl 1;	
		r2 = SEG16 T_SACM_A1601_SpeechTable
		DS = r2
		R1 += OFFSET T_SACM_A1601_SpeechTable;
		jmp L_Continue_0;	
.endif

.ifdef A128_Used								
.external T_SACM_A128_MusicTable				
L_A128_Res_M:								
		r1 = r1 lsl 2;	
		r2 = SEG16 T_SACM_A128_MusicTable
		DS = r2				
		R1 += OFFSET T_SACM_A128_MusicTable;		
		r2 = DS:[r1++];						
		[R_SACM_Resouce_BS] = r2;			
		r2 = DS:[r1++];						
		[R_SACM_Resouce_DS] = r2;			
		r2=DS:[r1++]
		r1=DS:[r1]
		r2-=[R_SACM_Resouce_BS]
		r1-=[R_SACM_Resouce_DS], carry
		[R_SACM_A128_Length_L]=r2
		[R_SACM_A128_Length_H]=r1	
		pop R2,r3 from [sp];					
		retf;								
.endif										

.ifdef	DVR1800_Used
.external	T_SACM_DVR1800_SpeechTable
L_DVR1800_Res_M:
		r1 = r1 lsl 1;	
		r2 = SEG16 T_SACM_DVR1800_SpeechTable
		DS = r2
		r1 += OFFSET T_SACM_DVR1800_SpeechTable;
		jmp L_Continue_0;	
.endif

.ifdef	S320_Used
.external	T_SACM_S320_SpeechTable
L_S320_Res_M:
		r1 = r1 lsl 1;			
		r2 = SEG16 T_SACM_S320_SpeechTable
		DS = r2
		r1 += OFFSET T_SACM_S320_SpeechTable;
		jmp L_Continue_0;	
.endif
	
L_Continue_0:
		R2 = DS:[R1 ++];
		[R_SACM_Resouce_BS] = R2; 
		R1 = DS:[R1];
		[R_SACM_Resouce_DS] = R1;
		pop R2,r3 from [sp];
	    //-----------------------
		// end of user define area
	    //-----------------------
		retf;	
//========================================================
// Function Name:	User_SetStartAddrManual
// Syntax:		User_SetStartAddrManual;(in assembly domain)
// Purpose: 	This function called by library to fetch data blocks 
//              for playback. The Library needs a data block specified
//              by R1,the start address, and R2, the length of the block.
//              Uaser would have to compose the function body 
//              based on the storage type to fulfill this demand from library. 
// Note:		NO	
// Parameter : 	R1 = R_SACM_Decode_In_Buffer,
//            	R2 = C_DECODE_IN_LENGTH 
// return with R_SACM_Decode_In_Buffer filled
// Return:		Block data (can be a word, a byte or an array)
// Destory: 	R1~R5
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================	
_User_SetStartAddrManual:
		push R1, R5 to [sp]

		R1 = 0;							//Get the datas from Memory
		[R_DataMode] = R1;

		R5 = [R_CODEC_TYPE];
		cmp R5, C_CODEC_MP3
		jne ?L_NotMP3
		R1 = SP + 8
		R4 = [R1++]				//The BS of sa
		R3 = [R1++]				//The DS of sa
		R2 = [R1++]				//The BS of ea
		R1 = [R1]				//The DS of ea
//MP3
//			DS = R3
//			R3 = DS:[R4++]
//		R1 += T_SACM_MP3_MusicTable;		//added by benwang 20060111
//		r2 = [r1++];						//added by benwang 20060111
//		[R_SACM_Resouce_BS] = r2;			//added by benwang 20060111
		[R_SACM_Resouce_BS] = r4;
//		r2 = [r1++];						//added by benwang 20060111
//		[R_SACM_Resouce_DS] = r2;			//added by benwang 20060111
//			R4 = DS:[R4]
		[R_SACM_Resouce_DS] = r3;
//		r2 = [r1++];						//end low addr
//		r1 = [r1];						//end high addr
//		r2-=[R_SACM_Resouce_BS]
//		r1-=[R_SACM_Resouce_DS], carry
//			DS = R1
//			R3 = DS:[R2++]
		r2-=[R_SACM_Resouce_BS]
//			R4 = DS:[R2]
		r1-=[R_SACM_Resouce_DS], carry
.ifdef MP3_Used
		[R_SACM_MP3_Length_L]=r2
		[R_SACM_MP3_Length_H]=r1
.endif
		jmp ?L_AddrGetEnd
//MP3 end
?L_NotMP3:
//		cmp R5, C_CODEC_SPEECH1600
//		jne ?L_AddrGetEnd
		R1 = SP + 8
		R2 = [R1++]
		[R_SACM_Resouce_BS] = R2;                    // modified by zgq on 2005/2/22
		R1 = [R1];
		[R_SACM_Resouce_DS] = R1;
?L_AddrGetEnd:
		pop R1, R5 from [sp]
		retf
	
//========================================================
// Function Name:	F_USER_GetData
// Syntax:		F_USER_GetData;(in assembly domain)
// Purpose: 	This function called by library to fetch data blocks 
//              for playback. The Library needs a data block specified
//              by R1,the start address, and R2, the length of the block.
//              Uaser would have to compose the function body 
//              based on the storage type to fulfill this demand from library. 
// Note:		NO	
// Parameter : 	R1 = R_SACM_Decode_In_Buffer,
//            	R2 = C_DECODE_IN_LENGTH 
// return with R_SACM_Decode_In_Buffer filled
// Return:		Block data (can be a word, a byte or an array)
// Destory: 	R1~R5
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================		
// Read from memory and write to buffer

F_USER_GetData:		.proc	//get data from nor flash FS
	push R2, R5 to [SP];
	R5 = [R_DataMode]
	cmp R5, 0
	je ?GetDataFromMemory;
//.if	1						//Only for MP3 or FAT Play
	// R1 = store dest pointer, R2 = store length 
	[SP--] = R2;			// param int length
	R2 = 0;
	[SP--] = R2;
	[SP--] = R1;

	R3 = [R_SACM_Resouce_BS];	// pointer to read data
	R4 = [R_SACM_Resouce_DS];

	call MR;			//===far indirect call ISA 1.2

	SP = SP + 3;			// clean param
//	jmp ?L_GetEnd
	pop R2, R5 from [SP];
//.endif
	jmp ?L_GetEndData		//r1 is the return value
?GetDataFromMemory:
//.else
	r5 = r2
	r3 = [R_SACM_Resouce_DS];
	ds=r3
	R3 = [R_SACM_Resouce_BS];
?L_Get_Loop:
	R4 = D:[R3++];
	[R1++] = R4;
	R2 -= 1;
	jnz ?L_Get_Loop;
?L_End:
	r4 = ds
	[R_SACM_Resouce_DS] = r4
	[R_SACM_Resouce_BS] = R3;
?L_GetEnd:
	r1 = r5;				//Normal return value: the length of get data length
	pop R2, R5 from [SP];
//.endif
?L_GetEndData:
	retf;
.endp

//========================================================
// Function Name:	_Fat_SetStartAddr
// Syntax:		FAT_SetStartAddr(LB2LP(function_name))
// Purpose: 	This API allows users to set the beginning address
//              to fetch data. This address can be either a ROM address
//              or a external storage address. User would have to modify
//              the function body based on the application s need.
// Note:		NO	
// Parameter : 	R1 = PlayIndex 
// Return:		N/A
// Destory: 	R1~R5
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_Fat_SetStartAddr: .proc
FAT_SetStartAddr:
		push R1 to [sp]
		R1 = 1;
		[R_DataMode] = R1
		pop r1 from [sp]

	   	push BP to [SP];
		BP = SP + 1;
	   	R1 = [BP + 3];
	   	R2 = [BP + 4];
        [R_SACM_Resouce_BS] = r1
        [R_SACM_Resouce_DS] = r2
		pop BP from [SP];		
        retf
        .endp

//========================================================
// Function Name:	_USER_SetFSLength
// Syntax:		USER_SetFSLength(unsigned long)
// Purpose: 	set total decode length
// Note:		NO	
// Parameter : 	R1:	length low address
//				R2: length high address 
// Return:		N/A
// Destory: 	R1,R2
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================        
.ifdef MP3_Used
_USER_SetFSLength:	.proc
F_USER_SetFSLength:
		push BP to [sp]
		BP = SP + 1;
	   	R1 = [BP + 3];
	   	R2 = [BP + 4];
        [R_SACM_MP3_Length_L] = r1
        [R_SACM_MP3_Length_H] = r2
		pop BP from [SP];		
        retf
        .endp
.endif
//========================================================
// Function Name:	_MP3_DecodeCount_Resume
// Syntax:		MP3_DecodeCount_Resume(MP3Decode_Length)
// Purpose: 	resume MP3 decode length
// Note:		NO	
// Parameter : 	R1:	length low address
//				R2: length high address 
// Return:		N/A
// Destory: 	R1,R2
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================          
_MP3_DecodeCount_Resume:	.proc
F_MP3_DecodeCount_Resume:
	   	push BP to [sp]
		BP = SP + 1;
	   	R1 = [BP + 3];
	   	R2 = [BP + 4];
	   	R1 += 0x400;							//加上一桢
	   	R2 += 0, carry							//防止此时就发生溢出
	   	R1 += [R_SACM_Decode_Count_L]
	   	R2 += [R_SACM_Decode_Count_H], carry
        [R_SACM_Decode_Count_L] = r1
        [R_SACM_Decode_Count_H] = r2
		pop BP from [SP];		
        retf
        .endp    

//========================================================
// Function Name:	F_USER_WriteData
// Syntax:		F_USER_WriteData;(in assembly domain)
// Purpose: 	write encoded data to memory or FAT 
// Note:		NO	
// Parameter : 	R1:	data mode, determine write to momery(0) or FAT(1)
// Return:		Block data (can be a word, a byte or an array)
// Destory: 	R1~R5
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================	
F_USER_WriteData:		.proc	//get data from nor flash FS
	push R1 to [sp]
	R1 = [R_DataMode]
	cmp R1, 0
	je ?PutToMemory;
?PutToResource:	
	pop R1 from [sp]
	push R1, R5 to [SP];	// add by adamcar
	// R1 = store dest pointer, R2 = store length 
	[SP--] = R2;			// param int length
	R2 = 0;
	[SP--] = R2;
	[SP--] = R1;

	R3 = [R_SACM_Resouce_BS];	// pointer to read data
	R4 = [R_SACM_Resouce_DS];

	call MR;			//===far indirect call ISA 1.2

	SP=SP+3;			// clean param
	pop R1, R5 from [SP]; 

.ifdef DumpEncodeData
	push r1, r4 to [SP];
	r3=[EncodeDataAddrH];			//delete for debug 2006/12/19       
	ds=r3
	R3 = [EncodeDataAddrL];   		
?L_Get_Loop0:		
	r4 = [r1++];
	D:[R3++] = r4;
	R2 -= 1;
	jnz ?L_Get_Loop0;
?L_End0:
	r4=ds
	[EncodeDataAddrH]=r4
	[EncodeDataAddrL] = R3;   
	pop r1, r4 from [SP];
.endif
	retf;
		
?PutToMemory:
	pop r1 from [sp];
	push r1, r4 to [SP];
  	r3=[R_SACM_Resouce_DS];   
	ds=r3
	R3 = [R_SACM_Resouce_BS];    	//delete for debug 2006/12/19  		
?L_Get_Loop:		
	r4 = [r1++];
	D:[R3++] = r4;
	R2 -= 1;
	jnz ?L_Get_Loop;
?L_End:
	r4=ds
	[R_SACM_Resouce_DS]=r4			
	[R_SACM_Resouce_BS] = R3;   	
	pop r1, r4 from [SP];	
.endif

	retf;
.endp
//add end 
.ifdef MP3_Used
.public _SACM_MP3_GetDacOut

_SACM_MP3_GetDacOut:
	push R1 to [sp]
	R1 = [_dacoutlength]
	[R_DAC_OUT_LENGTH] = R1
	pop R1 from [sp]
	retf
.endif // MP3_Used
//F_USER_WriteData:
F_USER_ReadWordBlock:						//Start Adr:R1; Length:R2
		push r1, r4 to [SP];
	    r3=[R_SACM_Resouce_DS];         
		ds=r3
		R3 = [R_SACM_Resouce_BS];       
?L_Get_Loop:		
		r4 = [r1++];
		D:[R3++] = r4;
		R2 -= 1;
		jnz ?L_Get_Loop;
?L_End:
		r4=ds
		[R_SACM_Resouce_DS]=r4
		[R_SACM_Resouce_BS] = R3;     
		pop r1, r4 from [SP];           
		RETF;


///////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------
//-- Procedure: _USER_SetRECStartAddr
//-- Syntax: USER_SetRECStartAddr(int)
//-- Parameter: R1 = PlayIndex 
//-- Return: NONE
//-- Description: This API allows users to set the beginning address 
//                to fetch data. This address can be either a ROM address
//                or a external storage address. User would have to modify
//                the function body based on the application's need.    
//--------------------------------------------------------------------
_USER_SetRECStartAddr:
USER_SetRECStartAddr:

		R1 = 0;							//added by zgq on 2005/03/04				
	    [R_SACM_ISR_Flag] = R1;			//added by zgq on 2005/03/04		
		[_R_ISR_DataProc_Flag] = R1		//added by chengye 2007/4/24
		R1 = 0;
		[R_DataMode] = R1;			//0:Memory, 1:Source	added by chengye 2006/11/15			
		
		push R5 to [SP];
		bp = SP + 1;
		R1 = [bp + 3];//
		R2 = [bp + 4]					// added by Anson 2004/2/2
		call F_USER_SetRECStartAddr;
		pop R5 from [sp];
		retf;	

F_USER_SetRECStartAddr: 
ManuCase:// external RAM/ROM Address
		[EXT_SRAM_Start_BS] = R1;			//del by zgq on 2005/03/01
		[R_SACM_Resouce_BS] = R1;	//add by zgq on 2005/03/01
		
		[EXT_SRAM_Start_DS] = R2;		

		[R_SACM_Resouce_DS] = R2;	//add by zgq on 2005/03/01
		
CaseEnd:		
	RETF;

T_SACM_Speech_Table_M:
.ifdef	S200_Used
	.dw offset L_S200_Res_M			//S200	   0
.endif

.ifdef	S4872_Used
	.dw	offset L_S4872_Res_M			//S4872 		1
.endif	
	
.ifdef	S530_Used	
	.dw	offset L_S530_Res_M 		//S530 		2 
.endif

.ifdef	S600TTS_Used			
	.dw	offset L_S600TTS_Res_M		//S600TTS	3
.endif		

.ifdef	S200TTS_Used
	.dw	offset L_S200TTS_Res_M		//S200TTS	4
.endif		

.ifdef	S320TTS_Used
	.dw	offset L_S320TTS_Res_M		//S320TTS	5
.endif		

.ifdef	MS01_Used
	.dw	offset L_MS01_Res_M			//MS01		6
.endif		

.ifdef	MS02_Used
	.dw	offset L_MS02_Res_M			//MS02		7
.endif		

.ifdef	SPEECH1600_Used	
	.dw	offset L_SPEECH1600_Res_M		//SPEECH1600	8
.endif		

.ifdef	AUDIO1600_Used
	.dw	offset L_AUDIO1600_Res_M		//AUDIO1600		9			
.endif		

.ifdef	SPEECH4800_Used
	.dw	offset L_SPEECH4800_Res_M		//SPEECH4800	10
.endif		

.ifdef	AUDIO4800_Used
	.dw	offset L_AUDIO4800_Res_M		//AUDIO4800		11
.endif		

.ifdef A6400S_Used				//added by zgq on 20051215
	.dw offset L_A6400S_Res_M	//added by zgq on 20051215
.endif							//added by zgq on 20051215

.ifdef AUDIO3200_Used				//add by benwang 20051220
	.dw offset L_AUDIO3200_Res_M	//add by benwang 20051220
.endif	                            //add by benwang 20051220

.ifdef DVR520_Used				//added by zgq on 20051220
	.dw offset L_DVR520_Res_M		//added by zgq on 20051220
.endif							//added by zgq on 20051220

.ifdef MP3_Used				//add by benwang 20060111
	.dw offset L_MP3_Res_M	//add by benwang 20060111
.endif	                            //add by benwang 20060111

//add by benwang 20060119
.ifdef	AUDIO1601_Used
	.dw	offset L_AUDIO1601_Res_M			
.endif		
//add by benwang 20060119

.ifdef A128_Used				//add by benwang 20060221
	.dw offset L_A128_Res_M		//add by benwang 20060221
.endif	                        //add by benwang 20060221


.ifdef DVR1800_Used				//add by benwang 20060621
	.dw offset L_DVR1800_Res_M	//add by benwang 20060621
.endif

.ifdef ETTS_Used				//added by chengye 2007/4/28
	.dw 0
.endif	

.ifdef S320_Used				//added by chengye 2007/4/28
	.dw offset L_S320_Res_M
.endif	
/////////////////////////////////
////////////////////////////////
///////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  Function: F_SACM_MS02_SongEvent	:Call back function for Song Notes events
//  Input: R1
//		bit[15:12] = 0x0 : End Event 
//		bit[15:12] = 0x1 : Note Event 
//		bit[15:12] = 0x2 : Instrument Event 
//		bit[15:12] = 0x3 : Volume Event
//		bit[15:12] = 0x4 : Pan Event
//		bit[15:12] = 0x6 : Tempo Event
//		bit[15:12] = 0x7 : Pitch Bend Event
//		bit[15:12] = 0x8 : Other Controller Event
// 		bit[11:8] Channel 0-7
//
//	End Event			bit[7:0]  Unuse	
//	Note Event:			bit[7:0]  Pitch
//	Instrument Event: 	bit[7:0] GM Instrument
//  Volume Event:		bit[7:0] Volume
//	Pan Event:			bit[7:0] Pan
//	Tempo Event: 		bit[7:0] Unuse
//
//		External Argument R2 bit[15:0] beat per minute
//
//	Pitch Bend Event
//		bit[7:0] Unuse
//		External Argument R2 bit[15:14] zero
//		External Argument R2 bit[13:7]  pitchMsb
//		External Argument R2 bit[6:0]	pitchLsb
//
//	Other Controller Event
//		bit[15:0] Unuse
//
/////////////////////////////////////////////////////////////////////////////////

//F_SACM_MS02_SongEvent_SECTION: .SECTION .code, .ADDR = 0x46000
//added by benwang2004/04/17
MS02_SongEvent:	.section	.code, .addr=0xfc54
F_SACM_MS02_SongEvent: .proc
		push	R1,R5 to [SP]
        //-----------------------
        // Event processed by user
        //-----------------------		
    	pop		R1,R5 from [SP]
        //-----------------------
        // End of Event processing
        //-----------------------
		RETF;
.endp

_Snd_onAmpOn:
//		push r1 to [sp]
//		r1=0xffff
//		[P_IOD_Dir]=r1		//Set the IOD is Output
//		[P_IOD_Dir]=r1
//		r1=0x0001
//		[P_IOD_Data]=r1		//P_IOA_Data --> P_IOE_Data
//		pop r1 from [sp]
		retf

//off gongfang,rewrite accoding to your hardware
_Snd_onAmpOff:
//		push r1 to [sp]
//		r1=0xffff
//		[P_IOD_Dir]=r1		//Set the IOD is Output
//		[P_IOD_Dir]=r1
//		r1=0x0000
//		[P_IOD_Data]=r1		//P_IOA_Data --> P_IOE_Data
//		pop r1 from [sp]
		retf
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=================================================================
.external R_SACM_Decode_Count_L
.external R_SACM_Decode_Count_H
.external F_SACM_System_Get_BS_Manual
.external F_SACM_Decode_Initial_BS
.external Mp3_FrameCount_L, Mp3_FrameCount_H
.external R_SACM_Decode_Length_L, R_SACM_Decode_Length_H
.public _User_NewDataMP3
.public _User_OldDataMP3
.public _User_ClearMP3AB
.public _SetMP3FrameCount

//========================================================
// Function Name:	_User_NewDataMP3
// Syntax:		User_NewDataMP3(unsigned long DecodeLen, unsigned long MP3FrameLen);
// Purpose: 	Set Mp3_FrameCount_L/H
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_User_NewDataMP3:
	push R1, R4 to [sp]
	
	R1 = sp + 7
	R3 = [R1++]						//Get the low addr
	R4 = [R1]

	R1 = [R_SACM_Decode_Count_L]
 	R2 = [R_SACM_Decode_Count_H]
	R1 += R3
	R2 += R4, carry
 	[R_SACM_Decode_Count_L] = R1
	[R_SACM_Decode_Count_H] = R2

	R1 = sp + 9
	R3 = [R1++]						//The offset of Frame Count
	R4 = [R1]
	R1 = [Mp3_FrameCount_L]
 	R2 = [Mp3_FrameCount_H]
 	R1 += R3
	R2 += R4, carry
 	[Mp3_FrameCount_L] = R1
	[Mp3_FrameCount_H] = R2

	call F_SACM_System_Get_BS_Manual

	pop r1, r4 from [sp]
	retf

_User_OldDataMP3:
	push R1, R4 to [sp]

	R1 = sp + 7
	R3 = [R1++]						//Get the low addr
	R4 = [R1]

	R1 = [R_SACM_Decode_Count_L]
 	R2 = [R_SACM_Decode_Count_H]
	R1 -= R3
	R2 -= R4, carry
 	[R_SACM_Decode_Count_L] = R1
	[R_SACM_Decode_Count_H] = R2

	R1 = sp + 9
	R3 = [R1++]						//The offset of Frame Count
	R4 = [R1]
	R1 = [Mp3_FrameCount_L]
 	R2 = [Mp3_FrameCount_H]
 	R1 -= R3
	R2 -= R4, carry
 	[Mp3_FrameCount_L] = R1
	[Mp3_FrameCount_H] = R2

	call F_SACM_System_Get_BS_Manual

	pop r1, r4 from [sp]
	retf

//========================================================
// Function Name:	_User_ClearMP3AB
// Syntax:		User_ClearMP3AB(unsigned long Playlength);
// Purpose: 	For AB Play only, resume the decode_length of the old song:
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	R1~R4
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_User_ClearMP3AB:
	push R1, R4 to [sp]

	R1 = sp + 7
	R3 = [R1++]						//Get the low addr
	R4 = [R1]

 	[R_SACM_Decode_Length_L] = R3
	[R_SACM_Decode_Length_H] = R4

	pop r1, r4 from [sp]
	retf

//========================================================
// Function Name:	_SetMP3FrameCount
// Syntax:		SetMP3FrameCount(unsigned long MP3Frame)
// Purpose: 	reset MP3 played FrameNumber
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	R1,R2
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
_SetMP3FrameCount:
	push r1, r2 to [sp]
	r1 = sp + 5
	r2 = [r1++]				//Get the low value of the input parament
	r1 = [r1]				//Get the High value of the input parament
	[Mp3_FrameCount_L] = r2
	[Mp3_FrameCount_H] = r1
	pop r1, r2 from [sp]
	retf

//For MP3 Use function:
.ifdef MP3_Used
.public _USER_PauseMP3
.public _USER_ResumeMP3
.endif
.public _UserGetDecodeLength

.external _MP3Decode_Length
.external _MP3Former_Length
.external R_SACM_DAC_Out_Buffer
.external R_DAC_OUT_LENGTH
.external R_SACM_Decode_In_Buffer
.external R_DECODE_IN_LENGTH
.external R_SACM_Decode_In_PTR
.external _ulFrame_CurrentCount

//========================================================
// Function Name:	_USER_PauseMP3
// Syntax:		USER_PauseMP3(void);
// Purpose: 	pause MP3 play and store some variale
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.ifdef MP3_Used
_USER_PauseMP3:
	push r1 to [sp]
	r1 = [R_SACM_MP3_Length_L]
	[_MP3Former_Length] = r1
	r1 = [R_SACM_MP3_Length_H]
	[_MP3Former_Length + 1] = r1
	
	R1 = [R_SACM_Decode_Count_L]
	[_MP3Decode_Length] = R1
 	R1 = [R_SACM_Decode_Count_H]
 	[_MP3Decode_Length + 1] = R1

 	//保存此次的MP3的Mp3_FrameCount_L&Mp3_FrameCount_H的值:
 	R1 = [Mp3_FrameCount_L]
 	[R_OLDMP3FrameCount_L] = R1
 	R1 = [Mp3_FrameCount_H]
 	[R_OLDMP3FrameCount_H] = R1
 	R1 = [R_DAC_OUT_LENGTH]
 	[_dacoutlength] = R1

	pop r1 from [sp]
	retf
.endif  // MP3_Used
_UserGetDecodeLength:
	push r1, r2 to [sp]
	r1 = [R_SACM_Decode_Count_L]
	[_ulFrame_CurrentCount] = r1
	r2 = [R_SACM_Decode_Count_H]
	[_ulFrame_CurrentCount + 1] = r2
	pop r1, r2 from [sp]
	retf

//========================================================
// Function Name:	_MP3_FrameResume
// Syntax:		MP3_FrameResume(void);
// Purpose: 	resume MP3 frame count
// Note:		NO	
// Parameter : 	N/A
// Return:		N/A
// Destory: 	R1
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.ifdef MP3_Used
.public _MP3_FrameResume
_MP3_FrameResume:
	push R1 to [sp]
	R1 = [R_OLDMP3FrameCount_L]
 	[Mp3_FrameCount_L] = R1
 	R1 = [R_OLDMP3FrameCount_H]
 	[Mp3_FrameCount_H] = R1
	pop R1 from [sp]
	retf

_USER_ResumeMP3:
	push R1, R5 to [sp]

	R1 = sp + 8
	R1 = [R1]
	[R_SACM_MP3_Length_L]=r1
	R1 = sp + 9
	R1 = [R1]
	[R_SACM_MP3_Length_H]=r1
	R1 = sp + 10
	R4 = [R1]					//Low Addr
	R1 = sp + 11
	R5 = [R1]					//High Addr

	R1 = 0x0000;
	R2 = R_SACM_DAC_Out_Buffer;
	R3 = R2 + [R_DAC_OUT_LENGTH];
	R3 += [R_DAC_OUT_LENGTH];
?_Loop_0:
	[R2++] = R1;
	cmp R2, R3;
	jne ?_Loop_0;

	call F_SACM_Decode_Initial_BS

	[R_SACM_Decode_Count_L] = R4
	[R_SACM_Decode_Count_H] = R5
	
	R1 = R_SACM_Decode_In_Buffer
	R1 += [R_DECODE_IN_LENGTH]		//Get The New Data
	
	[R_SACM_Decode_In_PTR] = R1
	call F_SACM_System_Get_BS_Manual

	pop R1, R5 from [sp]
	retf
.endif //MP3_used
.public _User_Getdaclength
.external R_DAC_OUT_LENGTH

_User_Getdaclength:
	R1 = [R_DAC_OUT_LENGTH]
	retf
//=======================================================================
//For Seek Play:
//========================================================
// Function Name:	_User_GetCurDecodeLength
// Syntax:		_User_GetCurDecodeLength(void);
// Purpose: 	get current decode length
// Note:		NO	
// Parameter : 	N/A
// Return:		R1,R2
// Destory: 	R1,R2
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public _User_GetCurDecodeLength
_User_GetCurDecodeLength:
	r1 = [R_SACM_Decode_Count_L]
	r2 = [R_SACM_Decode_Count_H]
	retf

//========================================================
// Function Name:	_User_SetDecodeLength
// Syntax:		_User_GetCurDecodeLength(void);
// Purpose: 	set decode length
// Note:		NO	
// Parameter : 	N/A
// Return:		R1,R2
// Destory: 	R1,R2
// Stack Depth:  
// CPU Cycle:
// Transfer method:	stack
//========================================================
.public _User_SetDecodeLength
_User_SetDecodeLength:
	push R1, R2 to [sp]
	R1 = SP + 5
	R2 = [R1 ++]
	R3 = [R1 ++]
	[R_SACM_Decode_Count_L] = R2
	[R_SACM_Decode_Count_H] = R3
	pop R1, R2 from [sp]
	retf

//====================For A128 Use to set ==================
.ifdef A128_Used
.public _USER_PauseA128
_USER_PauseA128:
	push r1 to [sp]
	r1 = [R_SACM_A128_Length_L]
	[_MP3Former_Length] = r1
	r1 = [R_SACM_A128_Length_H]
	[_MP3Former_Length + 1] = r1

	R1 = [R_SACM_Decode_Count_L]
	[_MP3Decode_Length] = R1
 	R1 = [R_SACM_Decode_Count_H]
 	[_MP3Decode_Length + 1] = R1

 	//保存此次的MP3的Mp3_FrameCount_L&Mp3_FrameCount_H的值:
 	R1 = [Mp3_FrameCount_L]
 	[R_OLDMP3FrameCount_L] = R1
 	R1 = [Mp3_FrameCount_H]
 	[R_OLDMP3FrameCount_H] = R1

	pop r1 from [sp]
	retf

.public _User_A128Setlength
_User_A128Setlength:
	push R1, R2 to [sp]
	R1 = SP + 5
	R2 = [R1++]
	R1 = [R1]
	[R_SACM_A128_Length_L] = R2
	[R_SACM_A128_Length_H] = R1
	pop R1, R2 from [sp]
	retf
.endif	//end of A128_Used	
//============================================================

//========================================================================