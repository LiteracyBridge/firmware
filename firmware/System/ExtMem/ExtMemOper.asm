.include .\system\include\system_head.inc

.PUBLIC _WLPUL
.PUBLIC _RLPUL
.PUBLIC _RLPUI
.PUBLIC _WLPUI
.PUBLIC _WVUL
.PUBLIC _RVUL
.PUBLIC _RVUI
.PUBLIC _WVUI

.CODE
M_CheckDS: .MACRO dsreg
	CMP dsreg,0x3f
	JBE	?L_Exit#
?L_Error#:
	R1 = EXTMEM_INVALID_ADDR
	PUSH R1 TO [SP]
	CALL _Sys_Exception
?L_Exit#:
.ENDM

//  void WVUL(unsigned long paddr,unsigned long value);
_WVUL: .PROC //(unsigned long paddr, unsigned long  value);
	PUSH BP TO [SP]
	R4 = SP + 7
	
	R2 = [R4--]		// = value & 0xffff >> 16	
	R3 = [R4--]		// = value & 0x0000ffff
	
	R1 = [R4--]		//SEG
	M_CheckDS R1
	DS = R1			//Setup DS	
	
	R1 = [R4--]		//Offset		
	
	DS:[R1++] = R3	//Write Low addr
	DS:[R1++] = R2	//Wriet high addr
	POP  BP FROM [SP]
	RETF
.ENDP

// unsigned long RVUL(unsigned long paddr);
_RVUL: .PROC //(unsigned long paddr);
	PUSH BP TO [SP]
	R4 = SP + 5	

	R1 = [R4--]		//SEG
	M_CheckDS R1
	DS = R1			//Setup DS
	
	R2 = [R4--]
	
	R1 = DS:[R2++]
	R2 = DS:[R2++]
	
	POP  BP FROM [SP]
	RETF
.ENDP

//	void WVUI(unsigned long paddr, int value);
_WVUI: .PROC 

	PUSH BP TO [SP]
	R4 = SP + 6
	
	R2 = [R4--]		//value
	
	R1 = [R4--]		//SEG	
	M_CheckDS R1
	DS = R1			//Setup DS
	
	
	R3 = [R4--]		//Offset	DS:R3 = R2		
	DS:[R3] = R2
	POP  BP FROM [SP]
	RETF
.ENDP

//unsigned int RVUI(unsigned long paddr);
_RVUI: .PROC 
	PUSH BP TO [SP]
	R4 = SP + 5	

	R1 = [R4--]		//SEG
	DS = R1			//Setup DS
	M_CheckDS R1
	R3 = [R4--]		//Offset	DS:R3 = R2		
	R1 = DS:[R3]
	
	POP  BP FROM [SP]
	RETF
.ENDP

//============================================================================
//============================================================================

//void WLPUL(unsigned int seg,unsigned int offset,unsigned long value);
_WLPUL: .PROC 
	PUSH BP TO [SP]
	R4 = SP + 7
	
	R2 = [R4--]		// = value & 0xffff >> 16	
	R3 = [R4--]		// = value & 0x0000ffff
	
	R1 = [R4--]		//Offset
	
	R4 = [R4--]		//SEG
	M_CheckDS R4
	DS = R4			//Setup DS	
	
	DS:[R1++] = R3	//Write Low addr
	DS:[R1++] = R2	//Wriet high addr
	POP  BP FROM [SP]
	RETF
.ENDP

//unsigned long RLPUL(unsigned int seg,unsigned int offset);
_RLPUL: .PROC 
	PUSH BP TO [SP]
	R4 = SP + 5	

	R3 = [R4--]		//Offset
	
	R1 = [R4--]
	M_CheckDS R1
	DS = R1			//Setup DS

	R1 = DS:[R3++]
	R2 = DS:[R3++]
	
	POP  BP FROM [SP]
	RETF
.ENDP

//void WLPUI(unsigned int seg,unsigned int offset, int value);
_WLPUI: .PROC 

	PUSH BP TO [SP]
	R4 = SP + 6
	
	R2 = [R4--]		//value
	
	R3 = [R4--]		//Offset	
	
	R1 = [R4--]		//Seg	
	M_CheckDS R1
	DS = R1			//Setup DS
		
	DS:[R3] = R2
	POP  BP FROM [SP]
	RETF
.ENDP

//unsigned int RLPUI(unsigned int seg,unsigned int offset);
_RLPUI: .PROC 
	PUSH BP TO [SP]
	R4 = SP + 5	

	R3 = [R4--]		//Offset
	R1 = [R4--]		//Seg	
	M_CheckDS R1
	DS = R1			//Setup DS
	
	R1 = DS:[R3]
	
	POP  BP FROM [SP]
	RETF
.ENDP