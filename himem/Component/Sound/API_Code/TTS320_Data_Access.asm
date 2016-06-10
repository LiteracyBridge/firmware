.include .\Component\Include\speech\algorithm.inc

.ifdef S320TTS_Used

.PUBLIC F_TTS320_System_Get_SAC_Manual
.PUBLIC F_SACM_TTS320_Decode_Initial_BS
.PUBLIC F_SACM_TTS320_System_Get_BS_Manual
//.PUBLIC _USER_SetStartAddr //2005/03/29

//.define TTS320_GetData_From_Memory

//20051020
.EXTERNAL R_SACM_Resouce_BS, R_SACM_Resouce_DS
.EXTERNAL R_SACM_S320_Decode_Length_L, R_SACM_S320_Decode_Length_H
.EXTERNAL R_SACM_S320_Decode_Count_L, R_SACM_S320_Decode_Count_H
.EXTERNAL R_SACM_S320_Decode_In_PTR
.EXTERNAL R_SACM_S320_Decode_In_Buffer
.EXTERNAL R_DECODE_IN_LENGTH

.ifdef 	TTS320_GetData_From_Memory
.EXTERNAL __RES_SP_WORD_320_sa
.endif

//added by chengye 2007/5/15
.external _Get_DataBase_TTS320
.external _Read_TTS320Res_NotOffset
.external _Read_TTS320Res

.define MAX_SEG			20

OVERLAP_TTS320_DM_BLOCK:	.SECTION	.ORAM, .addr = 0x930
R_DIC_Buffer:			.DW	MAX_SEG DUP(?)
R_START_ADDR_BUFFER:	.DW	MAX_SEG*2 DUP(?)
R_Syllable_Num:			.DW ?
R_DIC_PTR:				.DW	?
R_ADDR_PTR:				.DW	?
.public TTS320_Buffer,_TTS320_Buffer
TTS320_Buffer:
_TTS320_Buffer:			.DW ?
.CODE
F_TTS320_System_Get_SAC_Manual: .PROC
	push BP to [SP]
.ifdef TTS320_GetData_From_Memory
	r4 = [R_SACM_Resouce_DS]; //20051020         
	ds=r4 //20051020
	R4 = [R_SACM_Resouce_BS]; //20051020  
	R1 = D:[R4]     
.else
	//SR &= ~0xfc00
   	//SR |= [R_SACM_Resouce_DS] //20051020
	//R4 = [R_SACM_Resouce_BS] //20051020

	
  	// R_Syllable_Num = R5 = D:[R4] & 0xFF
  	push R1,R5 to [sp]
  	R1 = 0
  	[TTS320_Buffer] = R1
  	call _Read_TTS320Res_NotOffset
  	pop R1,R5 from [sp]
  	R1 = [TTS320_Buffer]
.endif  	
  	R5 = R1 & 0xFF
  	// if(R5>MAX_SEG) R5 = MAX_SEG;
  	cmp R5, MAX_SEG
  	jle ?_EndOfLimiter
  		R5 = MAX_SEG
  	?_EndOfLimiter:
  	[R_Syllable_Num] = R5

	//Get Segment information
	R1 = R_DIC_Buffer
	[R_DIC_PTR] = R1
	R1 = R_START_ADDR_BUFFER
	[R_ADDR_PTR] = R1
	?_odd:
		// odd //////////////////////////
		// if(SybCnt==0) break;
		cmp R5, 0
//		jz ?_EndOfGetSeg
		jnz ?continue
		goto ?_EndOfGetSeg
?continue:		
		// SybCnt--;
		R5 -= 1
		// Address_Low
		R3 = [R_ADDR_PTR]
		
.ifdef TTS320_GetData_From_Memory
		R1 = D:[R4++]
.else				
		push R1, R5 to [sp]
		R1 = 0
  		[TTS320_Buffer] = R1
		call _Read_TTS320Res
		pop R1, R5 from [sp]
		R1 = [TTS320_Buffer]	
.endif

.ifdef TTS320_GetData_From_Memory
		R2 = D:[R4]
.else		
		push R1,R5 to [sp]
		R1 = 0
  		[TTS320_Buffer] = R1
		call _Read_TTS320Res_NotOffset
		pop R1,R5 from [sp]
		R2 = [TTS320_Buffer]
.endif

		R1 = R1 LSL 4
		R2 = R2 ROL 4
		R1 = R1 LSL 4
		R2 = R2 ROL 4
		R2 = R2 LSL 2
		[R3++] = R2
		// Address_High
.ifdef TTS320_GetData_From_Memory
		R1 = D:[R4++]
.else				
		push R1,R5 to [sp]
		R1 = 0
  		[TTS320_Buffer] = R1
		call _Read_TTS320Res
		pop R1,R5 from [sp]
		R1 = [TTS320_Buffer]
.endif
		R1 = R1 LSR 4
		R1 = R1 LSR 2
		R2 = R1 & 0x0F
		[R3++] = R2
		[R_ADDR_PTR] = R3
		// DataLength (= FrameLength * 4)
		R3 = [R_DIC_PTR]
		R1 = R1 LSR 2
		R1 &= 0xFC
		[R3++] = R1
		[R_DIC_PTR] = R3
		// even //////////////////////////
		// if(SybCnt==0) break;
		cmp R5, 0
		jz ?_EndOfGetSeg
		// SybCnt--;
		R5 -= 1
		// Address_Low
		R3 = [R_ADDR_PTR]
		
.ifdef TTS320_GetData_From_Memory		
		R1 = D:[R4++]
.else		
		push R1,R5 to [sp]
		R1 = 0
  		[TTS320_Buffer] = R1
		call _Read_TTS320Res
		pop R1,R5 from [sp]
		R1 = [TTS320_Buffer]
.endif
		R2 = R1 LSL 2
		[R3++] = R2
		// Address_High
.ifdef TTS320_GetData_From_Memory
		R2 = D:[R4]
.else				
		push R1,R5 to [sp]
		R1 = 0
  		[TTS320_Buffer] = R1
		call _Read_TTS320Res_NotOffset
		pop R1,R5 from [sp]
		R2 = [TTS320_Buffer]
.endif
		R2 &= 0x3
		R1 = R1 LSL 4
		R2 = R2 ROL 2
		[R3++] = R2
		[R_ADDR_PTR] = R3
		// DataLength (= FrameLength * 4)
		R3 = [R_DIC_PTR]
		
.ifdef TTS320_GetData_From_Memory
		R1 = D:[R4]
.else				
		push R1,R5 to [sp]
		R1 = 0
  		[TTS320_Buffer] = R1
		call _Read_TTS320Res_NotOffset
		pop R1,R5 from [sp]
		R1 = [TTS320_Buffer]
.endif
		R1 &= 0xFC
		[R3++] = R1
		[R_DIC_PTR] = R3
//		jmp ?_odd
		goto ?_odd
?_EndOfGetSeg:
	R1 = R_DIC_Buffer
	[R_DIC_PTR] = R1
	R1 = R_START_ADDR_BUFFER
	[R_ADDR_PTR] = R1
	
	pop BP from [SP]
	retf;
.endp

F_SACM_TTS320_Decode_Initial_BS: .PROC
	R1 = R_DIC_Buffer
	R2 = R1 + [R_Syllable_Num]
	R3 = 0
	cmp R1, R2
	je ?_EndOfSum
	?_SumLoop:
		R3 += [R1++]
		cmp R1, R2
		jne ?_SumLoop
	?_EndOfSum:
	
	R4 = 0
	
 	[R_SACM_S320_Decode_Length_L] = R3 //20051020
 	//[R_SACM_XXXX_Decode_Length_L] = R3 //2005/03/29
 	[R_SACM_S320_Decode_Length_H] = R4 //20051020
 	//[R_SACM_XXXX_Decode_Length_H] = R4 //2005/03/29
	[R_SACM_S320_Decode_Count_L] = R4 //20051020
	//[R_SACM_XXXX_Decode_Count_L] = R4 //2005/03/29
	[R_SACM_S320_Decode_Count_H] = R4 //20051020
	//[R_SACM_XXXX_Decode_Count_H] = R4 //2005/03/29
	retf
.ENDP

F_SACM_TTS320_System_Get_BS_Manual: .proc
	R1 = [R_SACM_S320_Decode_In_PTR] //20051020
	//R1 = [R_SACM_XXXX_Decode_In_PTR] //2005/03/29
	cmp R1, R_SACM_S320_Decode_In_Buffer //20051020
	//cmp R1, R_SACM_XXXX_Decode_In_Buffer //2005/03/29
	je ?_EndOfGetBS
		// Get BS
		r2 = R_SACM_S320_Decode_In_Buffer
		r2 += [R_DECODE_IN_LENGTH]
		//cmp R1, R_SACM_S320_Decode_In_Buffer + C_DECODE_IN_LENGTH //20051020
		cmp R1, r2
		//cmp R1, R_SACM_XXXX_Decode_In_Buffer + C_DECODE_IN_LENGTH //2005/03/29
		jl ?_DataMoveUp
			R1 = R_SACM_S320_Decode_In_Buffer //20051020
			//R1 = R_SACM_XXXX_Decode_In_Buffer //2005/03/29
			[R_SACM_S320_Decode_In_PTR] = R1 //20051020
			//[R_SACM_XXXX_Decode_In_PTR] = R1 //2005/03/29
			jmp ?_EndOfDataMoveUp
		?_DataMoveUp:
			R2 = R_SACM_S320_Decode_In_Buffer //20051020
			//R2 = R_SACM_XXXX_Decode_In_Buffer //2005/03/29
			r3 = r2 + [R_DECODE_IN_LENGTH]
			//R3 = R_SACM_S320_Decode_In_Buffer + C_DECODE_IN_LENGTH //20051020
			//R3 = R_SACM_XXXX_Decode_In_Buffer + C_DECODE_IN_LENGTH //2005/03/29
			?_DataMoveUpLoop:
				R4 = [R1++]
				[R2++] = R4
				cmp R1, R3
			    jne ?_DataMoveUpLoop
			[R_SACM_S320_Decode_In_PTR] = R2 //20051020
			//[R_SACM_XXXX_Decode_In_PTR] = R2 //2005/03/29
		?_EndOfDataMoveUp:
		
		R1 = [R_SACM_S320_Decode_In_PTR] //20051020
		//R1 = [R_SACM_XXXX_Decode_In_PTR] //2005/03/29
		r2 = R_SACM_S320_Decode_In_Buffer
		r2 += [R_DECODE_IN_LENGTH]
		//R2 = R_SACM_S320_Decode_In_Buffer + C_DECODE_IN_LENGTH; //20051020
		//R2 = R_SACM_XXXX_Decode_In_Buffer + C_DECODE_IN_LENGTH; //2005/03/29
		R2 -= R1
		push R1, R2 to [SP]
		call F_TTS320_GetData;	// parameter: R1 = Destination buffer pointer,
								//            R2 = FrameLength
		SP += 2
		R1 = R_SACM_S320_Decode_In_Buffer //20051020
		//R1 = R_SACM_XXXX_Decode_In_Buffer //2005/03/29
		[R_SACM_S320_Decode_In_PTR] = R1 //20051020
		//[R_SACM_XXXX_Decode_In_PTR] = R1 //2005/03/29
	
	?_EndOfGetBS:
 	retf
.endp

// short *F_TTS320_GetData(short *DesPtr, short CopyLen)
F_TTS320_GetData: .PROC
.define DesPtr	0
.define CopyLen	1
	push BP to [SP]
	BP = SP + 4
	
	// if(CopyLen<=0) goto EndOfLoop
	R1 = [BP+CopyLen]
	cmp R1, 0
	jg ?_LoopStart
		goto ?_EndOfLoop
	
	// while(1)
	?_LoopStart:
		// if(R_Syllable_Num<=0)
		R4 = [R_Syllable_Num]
		cmp R4, 0
		jg ?_EndOfSyllableNumLessEquZero
			// while(CopyLen)
			// {
			// 	*DesPtr++ = 0;
			// 	CopyLen--;
			// }
			R1 = [BP+DesPtr]
			R2 = [BP+CopyLen]
			R3 = 0
			?_CopySilence:
				[R1++] = R3
				R2 -= 1
				jnz ?_CopySilence
			[BP+DesPtr] = R1
			[BP+CopyLen] = R2
			// break;
			goto ?_EndOfLoop
		?_EndOfSyllableNumLessEquZero:
		
		// if(*dic<=0) goto NextSegment
		R1 = [R_DIC_PTR]
		R2 = [R1]
		cmp R2, 0
			jle ?_NextSegment
		
		// if(*dic>=CopyLen) // Complete Segment
		cmp R2, [BP+CopyLen]
		jl ?_EndOfCompleteSegment
			// DesPtr = CopyFarData(DesPtr,sa,CopyLen)
			R1 = [R_ADDR_PTR]
			R2 = [R1++]
			R3 = [R1--]
			R1 = [BP+DesPtr]
			R4 = [BP+CopyLen]
			push R1, R4 to [SP]
			call _CopyTTS320DB
			SP += 4
			[BP+DesPtr] = R1
			// *dic -= CopyLen;
			R1 = [R_DIC_PTR]
			R2 = [R1]
			R2 -= [BP+CopyLen]
			[R1] = R2
			// *sa += CopyLen;
			R1 = [R_ADDR_PTR]
			R2 = [R1++]
			R3 = [R1--]
			R2 += [BP+CopyLen]
			R3 += 0, carry
			[R1++] = R2
			[R1--] = R3
			// break
			jmp ?_EndOfLoop
		?_EndOfCompleteSegment:
		
		// else // Uncomplete Segment:
			// CopyLen -= *dic;
			R1 = [BP+CopyLen]
			R2 = [R_DIC_PTR]
			R1 -= [R2]
			[BP+CopyLen] = R1
			// DesPtr = CopyFarData(DesPtr,*sa++,*dic++);
			R1 = [R_ADDR_PTR]
			R2 = [R1++]
			R3 = [R1--]
			R1 = [R_DIC_PTR]
			R4 = [R1]
			R1 = [BP+DesPtr]
			push R1, R4 to [SP]
			call _CopyTTS320DB
			SP += 4
			[BP+DesPtr] = R1

		// NextSegment:
		?_NextSegment:
			// dic++;
			R1 = [R_DIC_PTR]
			R1 += 1
			[R_DIC_PTR] = R1
			// sa++;
			R1 = [R_ADDR_PTR]
			R1 += 2
			[R_ADDR_PTR] = R1
			// R_Syllable_Num--;
			R1 = [R_Syllable_Num]
			R1 -= 1
			[R_Syllable_Num] = R1
			goto ?_LoopStart
	?_EndOfLoop:

	// return DesPtr
	R1 = [BP+DesPtr]
	pop BP from [SP]
	retf
.ENDP

// short *CopyTTS320DB(short *DesPtr,FARPTR sa,short CopyLen)
// .PUBLIC _CopyTTS320DB
_CopyTTS320DB: .PROC
.define CopyTTS320DB_DesPtr		0
.define CopyTTS320DB_sa_L		1
.define CopyTTS320DB_sa_H		2
.define CopyTTS320DB_CopyLen	3
	push BP to [SP]
	BP = SP + 4
	
	// R1 = DesPtr
	// R2 = CopyLen
	R1 = [BP+CopyTTS320DB_DesPtr]
	R2 = [BP+CopyTTS320DB_CopyLen]
	
	// if(R2<=0) return DesPtr;
	cmp R2, 0
	jle ?_Return
	
	// D:R3 = sa
	R3 = [BP+CopyTTS320DB_sa_L]
	R4 = [BP+CopyTTS320DB_sa_H]
	
.ifdef 	TTS320_GetData_From_Memory
//get data from memory
	R3 += offset __RES_SP_WORD_320_sa
	R4 += seg16 __RES_SP_WORD_320_sa, carry
	SR &= ~0xfc00
	R4 = R4 LSL 4
	R4 = R4 LSL 4
	SR |= R4 LSL 2
	
	// do
	?_LoopStart:
		// *D:R3++ = *R1++;
		R4 = D:[R3++]
		[R1++] = R4
		// R2--;
		R2 -= 1
	// while(R2)
	jnz ?_LoopStart
.else
	push R1, R4 to [sp]
	call _Get_DataBase_TTS320
	pop R1, R4 from [sp];
	R1 += R2;					//return DesPtr
.endif		//end of define TTS320_GetData_From_Memory

?_Return:
	// return R1;
	pop BP from [SP]
	retf
.ENDP

.endif	//#ifdef S320TTS_Used
