//========================================================================================
//Header Name:		unSPMACRO.h
//Applied body:		SPL16256A
//Purpose:		Header File for the Assembly language that try to involve general-purposed MACRO
//Note:			to gain CPU performance, Most MACRO are written as in-line assembly
//Programmer:		CP2
//Written Date:		2003-4-18
//Version history:
//MACRO list:
//			M_SetupDS
//			M_ClearWatchdog
//			M_ByteSwap
//			M_NibbleSwap
//			M_TwoBitSwap
//			M_BitReverse

//			M_Mov
//			M_Switch
//			M_Case
//			M_Inc32
//			M_Inc16
//			M_Dec16
//			M_Add16
//			M_Sub16
//			M_Add32
//			M_Sub32
//			M_Sub64
//			M_32Add16
//			M_Div32_2
//			M_Div32_4
//			M_Div32_8
//			M_Div32_16
//			M_Double32
//			M_Mul32_3
//			M_32Mul16
//			M_Square
//			M_Je32
//			M_Jg32
//			M_Jl32
//			M_Ja32
//			M_Jb32
//			M_FarJe32
//			M_FarJg32
//			M_FarJl32
//			M_FarJa32
//			M_FarJb32
//			M_LSL1
//			M_LSR1
//			M_ROR1
//			M_ZeroFarJmp
//			M_NZeroFarJmp
//			M_EquFarJmp
//			M_NEquFarJmp
//			M_FarJb
//			M_FarJl
//			M_FarJge
//			M_FarJa
//			M_FarJbe
//			M_FarJg
//			M_FarJe
//========================================================================================
//the below two macros:copied by Mona from Jianchao unSPMACRO.inc  on Mar.5,2004.
//========================================================================================
//M_Set60Page:	.MACRO	register
//	[0x702b] = register
//	.ENDM

M_SetupDSValue:	.MACRO	register
//	cmp	register,0x003C
//	je	?L_NeedExten#
//	cmp register,0x0040
//	jb	?L_NoNeedExtern#
//?L_NeedExten#:
//	M_Set60Page register
//	[0x702b] = register
//	register = 0x003C
//?L_NoNeedExtern#:
	M_SetupDS	register
	.ENDM

//========================================================================================
//Macro Name:	M_SetupDS
//Parameters:	val
//Return Value:	none
//Description:	setup data segment(DS) of CPU
//Destroy:	r1
//Stack Depth:	0
//Usage:	M_SetupDS	r1,[r1]
//Example:      M_SetupDS	r1
//========================================================================================
//M_SetupDS:     .MACRO    register,val
//	.IFMA 2
//	register = val
//	[P_DS_Setup] = register
//	.ELSE
//	[P_DS_Setup] = register
//	.ENDIF
//	.ENDM
M_SetupDS:     .MACRO    val
	ds = val
	.ENDM

//========================================================================================
//Macro Name:	M_ClearWatchdog
//Parameters:	none
//Return Value:	none
//Description:	setup data segment(DS) of CPU
//Destroy:	r1
//Stack Depth:	0
//Usage:	M_ClearWatchdog
//Example:	M_ClearWatchdog
//========================================================================================
M_ClearWatchdog:	.MACRO
			r1 = 0xA005
			[P_Watchdog_Clear] = r1
			.ENDM

//========================================================================================
//Macro Name:	M_ByteSwap
//Parameters:	r1
//Return Value:	r1=r1 after byte swap [B15:B8,B7,B0]-> [B7:B0,B15,B8]
//Description:
//Destroy:	r1
//Stack Depth:	0
//Usage:	M_ByteSwap
//Example:
//		M_ByteSwap
//========================================================================================
M_ByteSwap:	.MACRO
		push r2 to [sp]
		r2 = r1
		r2 = r2 lsl 4
		r2 = r2 lsl 4
		r1 = r1 lsr 4
		r1 = r1 lsr 4
		r1| = r2
		pop r2 from [sp]
		.ENDM

//========================================================================================
//Macro Name:	M_NibbleSwap
//Parameters:	r1
//Return Value:	r1=r1 after nibble swap [B11:B8,B15:B12,B3:B0,B7:B4]
//Description:
//Destroy:	r1
//Stack Depth:	0
//Usage:	M_NibbleSwap
//Example:
//		M_NibbleSwap
//========================================================================================
M_NibbleSwap:	.MACRO
		[P_Nibble_Swap] = r1
		r1 = [P_Nibble_Swap]
		.ENDM

//========================================================================================
//Macro Name:	M_TwoBitSwap
//Parameters:	r1
//Return Value:	r1=r1 after Two bit swap[B13:B12,B15:B14,B9:B8,B11:B10,B5:B4,B7,B6,B1:B0,B3:B2]
//Description:
//Destroy:	r1
//Stack Depth:	0
//Usage:	M_TwoBitSwap
//Example:
//		M_TwoBitSwap
//========================================================================================
M_TwoBitSwap:	.MACRO
		[P_TwoBit_Swap] = r1
		r1 = [P_TwoBit_Swap]
		.ENDM

//========================================================================================
//Macro Name:	M_BitReverse
//Parameters:	r1
//Return Value:	r1=r1 after Two bit swap[B0:B15]
//Description:
//Destroy:	r1
//Stack Depth:	0
//Usage:	M_BitReverse
//Example:
//		M_BitReverse
//========================================================================================
M_BitReverse:	.MACRO
		[P_Bit_Reverse] = r1
		r1 = [P_Bit_Reverse]
		.ENDM

//========================================================================================
//Macro Name:		M_Mov
//Parameters:		dest,src
//Return Value:		none
//Description:		[dest]=scr
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Mov dest,sr
//Example:
//			M_Mov RW_L_X,0x0001
//========================================================================================
M_Mov:	.MACRO	dest,src
	r1 = src
	[dest] = r1
	.ENDM

//========================================================================================
//Macro Name:		M_Switch
//Parameters:		Varible
//Return Value:		r1=[Varible]
//Description:		r1=[Varible]
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Switch RW_L_X
//Example:
//			M_Switch RW_L_X
//========================================================================================
M_Switch: .MACRO	Varible
	r1 = [Varible]
	.ENDM

//========================================================================================
//Macro Name:		M_Case
//Parameters:		r1,Number1,Number2,L_Goto
//Return Value:		none
//Description:		if arg1<=[r1]<=arg2,then jump to L_Goto,else exeute in order
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Case 1,10,L_Key
//Example:
//			M_Case 1,10,L_Key
//			M_Case 1,L_Key
//========================================================================================
M_Case:	.MACRO arg1,arg2,arg3
	.IFMA	3
	cmp r1,arg1
	jb ?L_CaseGoon#		//<
	cmp r1,arg2
	ja ?L_CaseGoon#		//>
	goto arg3
	.ELSE
	cmp r1,arg1
	jne ?L_CaseGoon#
	goto arg2
	.ENDIF
?L_CaseGoon#:
	.ENDM

//========================================================================================
//Macro Name:		M_Inc32
//Parameters:		Var1(32bits)
//Return Value:		[Var1]=[Var1]+1
//Description:
//Destroy:		r1,r2
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Inc32 RD_L_X
//Example:
//			M_Inc32 RD_L_X
//========================================================================================
M_Inc32:	.MACRO	Var1
		r1 = 1
		r1 += [Var1]
		[Var1] = r1
		r1 = 0
		r1 += [Var1+1],Carry
		[Var1+1] = r1
		.ENDM

//========================================================================================
//Macro Name:		M_Inc16
//Parameters:		Var1(16bits)
//Return Value:		[Var1]=[Var1]+1
//Description:
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Inc16 RW_L_X
//Example:
//			M_Inc16 RW_L_X
//========================================================================================
M_Inc16:	.MACRO	Var1
		r1 = [Var1]
		r1 += 1
		[Var1] = r1
		.ENDM

//========================================================================================
//Macro Name:		M_Add32
//Parameters:		add1(32bits),add2(32bits)
//Return Value:		sum(32bits)
//Description:		[sum]=[add1]+[add2],two 32 bits number add,sum is 32 bits too
//Destroy:		r1,r2
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Add32 RD_L_Sum,RD_L_Add1,RD_L_Add2
//Example:
//			M_Add32 RD_L_Sum,RD_L_Add1,RD_L_Add2
//========================================================================================
M_Add32:	.MACRO	Sum,RAdd1,RAdd2
		r1 = [RAdd1]
		r2 = [RAdd1+1]
		r1 += [RAdd2]
		r2 += [RAdd2+1],Carry
		[Sum] = r1
		[Sum+1] = r2
		.ENDM

//========================================================================================
//Macro Name:		M_32Add16
//Parameters:		add1(32bits),add2(16bits)
//Return Value:		sum(32bits)
//Description:		[sum]=[add1]+[add2],one 32 bits number add one 16 bits signed
//			number ,sum is 32 bits
//Destroy:		r1,r2
//Memory Modified:
//Stack Depth:		0
//Usage:		M_32Add16 RD_L_Sum,RD_L_Add1,RW_L_Add2
//Example:
//			M_32Add16 RD_L_Sum,RD_L_Add1,RW_L_Add2
//========================================================================================
M_32Add16:	.MACRO	Sum,RAdd1,RAdd2
		r1 = [RAdd1]			//low
		r2 = [RAdd1+1]			//high
		r1 += [RAdd2]
		r2 += 0,Carry
		[Sum] = r1
		[Sum+1] = r2
		.ENDM

//========================================================================================
//Macro Name:		M_Dec16
//Parameters:		Var1(16bits)
//Return Value:		[Var1]=[Var1]-1
//Description:
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Dec16 RW_L_X
//Example:
//			M_Dec16 RW_L_X
//========================================================================================
M_Dec16:	.MACRO	Var1
		r1 = [Var1]
		r1 -= 1
		[Var1] = r1
		.ENDM

//========================================================================================
//Macro Name:		M_Dec32
//Parameters:		Var1(32bits)
//Return Value:		[Var1]=[Var1]-1
//Description:
//Destroy:		r1,r2
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Dec32 RD_L_X
//Example:
//			M_Dec32 RD_L_X
//========================================================================================
M_Dec32:	.MACRO	Var1
		r2 = [Var1+1]
		r1 = [Var1]
		r1 -= 1
		r2 -= 0,Carry				//Between this two line,there must have no line
		[Var1] = r1
		[Var1+1] = r2
		.ENDM

//========================================================================================
//Macro Name:		M_Add16
//Parameters:		Var1(16bits)
//Return Value:		[Var1]=[Var2]+[Var3]
//Description:		var1 must <=0xffff
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Add16 RW_L_Z,RW_L_X,RW_L_Y
//Example:
//			M_Sub16 RW_L_X,RW_L_X,RW_L_Y
//========================================================================================
M_Add16:	.MACRO	Var1,Var2,Var3
	r1 = [Var2]
	r1 += [Var3]
	[Var1] = r1
	.ENDM

//========================================================================================
//Macro Name:		M_Sub16
//Parameters:		Var1(16bits)
//Return Value:		[Var1]=[Var2]-[Var3]
//Description:
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Sub16 RW_L_Z,RW_L_X,RW_L_Y
//Example:
//			M_Sub16 RW_L_X,RW_L_X,RW_L_Y
//========================================================================================
M_Sub16:	.MACRO	Var1,Var2,Var3
	r1 = [Var2]
	r1 -= [Var3]
	[Var1] = r1
	.ENDM

//========================================================================================
//Macro Name:		M_Sub32
//Parameters:		Var1(32bits),Var2(32bits),Var3(32bits)
//Return Value:		[Var1]=[Var2]-[Var3]
//Description:
//Destroy:		r1,r2
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Sub32 RD_L_X,RD_L_Y,RD_L_Z
//Example:
//			M_Sub32 RD_L_X,RD_L_Y,RD_L_Z
//========================================================================================
M_Sub32: 	.MACRO	Var1,Var2,Var3
		r1 = [Var2]
		r2 = [Var2+1]
		r1 -= [Var3]
		r2 -= [Var3+1],Carry
		[Var1] = r1
		[Var1+1] = r2
		.ENDM

//========================================================================================
//Macro Name:		M_Sub64
//Parameters:		Var2(64bits),Var3(64bits)
//Return Value:		[Var1]=r4r3r2r1=[Var2]-[Var3]
//Description:
//Destroy:		r1,r2,r3,r4
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Sub32 RBuf_L_X,RBuf_L_Y
//Example:
//			M_Sub32 RBuf_L_X,RBuf_L_Y
//========================================================================================
M_Sub64: 	.MACRO	Var1,Var2,Var3
		r4 = [Var2+3]
		r3 = [Var2+2]
		r2 = [Var2+1]
		r1 = [Var2]
		r1 -= [Var3]
		r2 -= [Var3+1],Carry
		r3 -= [Var3+2],Carry
		r4 -= [Var3+3],Carry
		[Var1] = r1
		[Var1+1] = r2
		[Var1+2] = r3
		[Var1+3] = r4
		.ENDM


//========================================================================================
//Macro Name:		M_Div32_2
//Parameters:		var1(32bits),var2(32bits)
//Return Value:		[var1]= [var2]/2 unsigned
//Description:
//Destroy:		r1
//Memory Modified:
//Stack Depth:		2
//Usage:		M_Div32_2 RD_L_X,RD_L_Y
//Example:
//			M_Div32_2 RD_L_X,RD_L_X
//========================================================================================
M_Div32_2: 	.MACRO	Var1,Var2
		r1 = [Var2+1]
		r1 = r1 lsr 1
		[Var1+1] = r1
		r1 = r1 lsr 3
		r1 = [Var2]
		r1 = r1 ror 1
		[Var1] = r1
		.ENDM

//========================================================================================
//Macro Name:		M_Div32_4
//Parameters:		var1(32bits),var2(32bits)
//Return Value:		[var1]= [var2]/4 unsigned
//Description:
//Destroy:		r1
//Memory Modified:
//Stack Depth:		2
//Usage:		M_Div32_2 RD_L_X,RD_L_Y
//Example:
//			M_Div32_2 RD_L_X,RD_L_X
//========================================================================================
M_Div32_4: 	.MACRO	Var1,Var2
		r1 = [Var2+1]
		r1 = r1 lsr 2
		[Var1+1] = r1
		r1 = r1 lsr 2
		r1 = [Var2]
		r1 = r1 ror 2
		[Var1] = r1
		.ENDM

//========================================================================================
//Macro Name:		M_Div32_8
//Parameters:		Var1(32bits,unsigned),Var2(32bits,unsigned)
//Return Value:		[Var1]=[Var2]/8
//Description:
//Destroy:		r1
//Memory Modified:
//Stack Depth:		2
//Usage:		M_Div32_8 RD_L_X,RD_L_Y
//Example:
//			M_Div32_8 RD_L_X,RD_L_X
//========================================================================================
M_Div32_8: 	.MACRO	Var1,Var2
		r1 = [Var2+1]
		r1 = r1 lsr 3
		[Var1+1] = r1
		r1 = r1 lsr 1
		r1 = [Var2]
		r1 = r1 ror 3
		[Var1] = r1
		.ENDM

//========================================================================================
//Macro Name:		M_Div32_16
//Parameters:		Var1(32bits,unsigned),Var2(32bits,unsigned)
//Return Value:		[Var1]=[Var2]/16
//Description:
//Destroy:		r1
//Memory Modified:
//Stack Depth:		2
//Usage:		M_Div32_16 RD_L_X,RD_L_Y
//Example:
//			M_Div32_16 RD_L_X,RD_L_Y
//			M_Div32_16 RD_L_X,RD_L_X
//========================================================================================
M_Div32_16: 	.MACRO	Var1,Var2
		r1 = [Var2+1]
		r1 = r1 lsr 4
		[Var1+1] = r1
		r1 = [Var2]
		r1 = r1 ror 4
		[Var1] = r1
		.ENDM

//========================================================================================
//Macro Name:		M_Double32
//Parameters:		Var1(32bits,unsigned),Var2(32bits,unsigned)
//Return Value:		[Var1]=[Var2]*2
//Description:		[Var1] is 32 bits,so if result is 64 bits,don't use this macro
//Destroy:		r1
//Memory Modified:
//Stack Depth:		2
//Usage:		M_Double32 RD_L_X,RD_L_Y
//Example:
//			M_Double32 RD_L_X,RD_L_X
//========================================================================================
M_Double32: 	.MACRO	Var1,Var2
		r1 = [Var2]
		r1 = r1 lsl 1
		[Var1] = r1
		r1 = r1 lsl 3
		r1 = [Var2+1]
		r1 = r1 rol 1
		[Var1+1] = r1
		.ENDM

//========================================================================================
//Macro Name:		M_Mul32_3
//Parameters:		Var1(32bits unsigned),Var2(32bits)
//Return Value:		[Var1]=[Var1]*3
//Description:		[result] is 32 bits,so if result is 64 bits,don't use this macro
//Destroy:		r1,r2
//Memory Modified:
//Stack Depth:		2
//Usage:		M_Mul32_3 RD_L_X
//Example:
//			M_Mul32_3 RD_L_X
//========================================================================================
M_Mul32_3: 	.MACRO	Var1
		r1 = [Var1]
		r2 = [Var1+1]
		r1 += [Var1]
		r2 += [Var1+1],carry
		r1 += [Var1]
		r2 += [Var1+1],carry
		[Var1] = r1
		[Var1+1] = r2
		.ENDM

//========================================================================================
//Macro Name:		M_32Mul16
//Parameters:		Var1(32bits,signed),Var2(32bits,signed),Var3(16bits,signed)
//Return Value:		[Var1]=[Var2]*[Var3]
//Description:		[Var1] is 32 bits,so if result is 64 bits,don't use this macro
//Destroy:		r3,r4
//Memory Modified:
//Stack Depth:		3
//Usage:		M_32Mul16 RD_L_X,RD_L_Y,RW_L_Z
//Example:
//			M_32Mul16 RD_L_X,RD_L_X,RW_L_Z
//========================================================================================
M_32Mul16: 	.MACRO	Var1,Var2,Var3
		r3 = [Var2]
		r4 = [Var3]
		MR = r3*r4			//Low*RW=r4r3
		[Var1] = r3
		[Var1+1] = r4

		r3 = [Var2+1]
		r4 = [Var3]
		MR = r3*r4			//High*RW=0r3,ignore r4
		r3 += [Var1+1]
		[Var1+1] = r3
		.ENDM

//========================================================================================
//Macro Name:		M_Unsigned32Mul16
//Parameters:		Var1(32bits,unsigned),Var2(32bits,unsigned),Var3(16bits,signed,but<=0x7fff)
//Return Value:		[Var1]=[Var2]*[Var3]
//Description:		[Var1] is 32 bits,so if result is 64 bits,don't use this macro
//Destroy:		r3,r4
//Memory Modified:
//Stack Depth:		3
//Usage:		M_Unsigned32Mul16 RD_L_X,RD_L_Y,RW_L_Z
//Example:
//			M_Unsigned32Mul16 RD_L_X,RD_L_X,RW_L_Z
//========================================================================================
M_Unsigned32Mul16: 	.MACRO	Var1,Var2,Var3
		r3 = [Var2]
		r4 = [Var3]
		MR = r3*r4,us			//Low*RW=r4r3
		[Var1] = r3
		[Var1+1] = r4

		r3 = [Var2+1]
		r4 = [Var3]
		MR = r3*r4,us			//High*RW=0r3,ignore r4
		r3 += [Var1+1]
		[Var1+1] = r3
		.ENDM

//========================================================================================
//Macro Name:		M_Square
//Parameters:		Var1(32bits),Var2(16bits)
//Return Value:		[Var1]=[Var2]*[Var2]  (all unsigned [Var2]must <0x7fff])
//Description:		[Var1] is 32 bits,so if result is 64 bits,don't use this macro
//Destroy:		r3,r4
//Memory Modified:
//Stack Depth:		2
//Usage:		M_Square RD_L_X,RW_L_Y
//Example:
//			M_Square RD_L_X,RW_L_Y
//========================================================================================
M_Square:	.MACRO	Var1,Var2
		r3 = [Var2]
		r4 = r3
		MR = r3*r4,us
		[Var1] = r3
		[Var1+1] = r4
		.ENDM

//guili add 040305
//========================================================================================
//Macro Name:		M_FarJe32
//Parameters:		Var1(32bits,signed),Var2(32bits,signed),L_Goto
//Return Value:
//Description:		goto Var3
//Destroy:		r1
//Memory Modified:
//Stack Depth:		1
//Usage:		M_FarJe32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_FarJe32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
M_FarJe32: .MACRO	Var1,Var2,Var3
	r1 = [Var1+1]
	cmp r1,[Var2+1]
//	jne ?L_Je32End#		//2004-5-11 12:12
	jne ?L_FarJe32End#
	r1 = [Var1]
	cmp r1,[Var2]
//	jne ?L_Je32End#		//2004-5-11 12:12,lable error
	jne ?L_FarJe32End#
	goto Var3
?L_FarJe32End#:
	.ENDM

//========================================================================================
//Macro Name:		M_Je32
//Parameters:		Var1(32bits,signed),Var2(32bits,signed),L_Goto
//Return Value:		if[Var1]=[Var2],jump to Var3
//Description:		goto Var3
//Destroy:		r1
//Memory Modified:
//Stack Depth:		1
//Usage:		M_Je32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_Je32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
M_Je32: 	.MACRO	Var1,Var2,Var3
		r1 = [Var1+1]
		cmp r1,[Var2+1]
//		jne L_Je32End#		//2004-5-11 12:13,lable is not local
		jne ?L_Je32End#
		r1 = [Var1]
		cmp r1,[Var2]
//		jne L_Je32End#		//2004-5-11 12:13,lable is not local	
		jne ?L_Je32End#
		goto Var3
?L_Je32End#:
		.ENDM

//========================================================================================
//Macro Name:		M_FarJg32
//Parameters:		Var1(32bits,signed),Var2(32bits,signed),L_Goto
//Return Value:		if[Var1]>[Var2],jump to Var3
//Description:		goto Var3
//Destroy:		r1,r2
//Memory Modified:
//Stack Depth:		1
//Usage:		M_FarJg32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_FarJg32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
//M_FarJg32: 	.MACRO	Var1,Var2,Var3
//		r1 = [Var1+1]
//		cmp r1,[Var2+1]
//		jl ?L_Jg32End#
//		je ?L_Jg32Low
//		goto Var3
//?L_Jg32Low:
//		r1 = [Var1]
//		cmp r1,[Var2]
//		jbe ?L_Jg32End#
//		goto Var3
//?L_Jg32End#:
//		.ENDM

//changeed by guili 040305
M_FarJg32: .MACRO	Var1,Var2,Var3
	r1 = [Var1+1]
	cmp r1,[Var2+1]
	jl ?L_FarJg32End#
	je ?L_FarJg32Low#
	goto Var3		
?L_FarJg32Low#:
	r1 = [Var1]
	cmp r1,[Var2]
	jbe ?L_FarJg32End#
	goto Var3
?L_FarJg32End#:
	.ENDM

//add by guili 040305
//========================================================================================
//Macro Name:		M_Jg32
//Parameters:		Var1(32bits,signed),Var2(32bits,signed),L_Goto
//Return Value:
//Description:		if[Var1]>[Var2],jmp to Var3
//Destroy:		r1
//Memory Modified:
//Stack Depth:		1
//Usage:		M_Jg32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_Jg32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
M_Jg32: .MACRO	Var1,Var2,Var3
	r1 = [Var1+1]
	cmp r1,[Var2+1]
	jl ?L_Jg32End#
	je ?L_Jg32Low#
	jmp Var3		
?L_Jg32Low#:
	r1 = [Var1]
	cmp r1,[Var2]
	jbe ?L_Jg32End#
	jmp Var3
?L_Jg32End#:
	.ENDM

//========================================================================================
//Macro Name:		M_FarJl32
//Parameters:		Var1(32bits,signed),Var2(32bits,signed),L_Goto
//Return Value:
//Description:		if[Var1]<[Var2],goto to Var3
//Destroy:		r1
//Memory Modified:
//Stack Depth:		1
//Usage:		M_FarJl32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_FarJl32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
M_FarJl32: .MACRO	Var1,Var2,Var3
	r1 = [Var1+1]
	cmp r1,[Var2+1]
	jg ?L_FarJl32End#
	je ?L_FarJl32Low#
	goto Var3
?L_FarJl32Low#:
	r1 = [Var1]
	cmp r1,[Var2]
	jae ?L_FarJl32End#
	goto Var3
?L_FarJl32End#:
	.ENDM

//add by guili 040305
//========================================================================================
//Macro Name:		M_Jl32
//Parameters:		Var1(32bits,signed),Var2(32bits,signed),L_Goto
//Return Value:
//Description:		if[Var1]<[Var2],jmp to Var3
//Destroy:		r1
//Memory Modified:
//Stack Depth:		1
//Usage:		M_Jl32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_Jl32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
M_Jl32: .MACRO	Var1,Var2,Var3
	r1 = [Var1+1]
	cmp r1,[Var2+1]
	jg ?L_Jl32End#
	je ?L_Jl32Low#
	jmp Var3
?L_Jl32Low#:
	r1 = [Var1]
	cmp r1,[Var2]
	jae ?L_Jl32End#
	jmp Var3
?L_Jl32End#:
	.ENDM

//========================================================================================
//Macro Name:		M_FarJa32
//Parameters:		Var1(32bits,unsgined),Var2(32bits,unsigned ),L_Goto
//Return Value:		if[Var1]>[Var2],jump to Var3
//Description:		goto Var3
//Destroy:		r1,r2
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Ja32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_Ja32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
//M_FarJa32: 	.MACRO	Var1,Var2,Var3
//		r1 = [Var1+1]
//		cmp r1,[Var2+1]
//		jb ?L_Ja32End#
//		je ?L_Ja32Low
//		goto Var3
//?L_Ja32Low:
//		r1 = [Var1]
//		cmp r1,[Var2]
//		jbe ?L_Ja32End#
//		goto Var3
//?L_Ja32End#:
//		.ENDM

//change by guili 040305
M_FarJa32: 	.MACRO	Var1,Var2,Var3
	r1 = [Var1+1]
	cmp r1,[Var2+1]
	jb ?L_FarJa32End#
	je ?L_FarJa32Low#
	goto Var3		
?L_FarJa32Low#:
	r1 = [Var1]
	cmp r1,[Var2]
	jbe ?L_FarJa32End#
	goto Var3
?L_FarJa32End#:
	.ENDM

//add by guili 040305
//========================================================================================
//Macro Name:		M_Ja32
//Parameters:		Var1(32bits,unsgined),Var2(32bits,unsigned ),L_Goto
//Return Value:
//Description:		if[Var1]>[Var2],jmp to Var3
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Ja32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_Ja32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
M_Ja32: .MACRO	Var1,Var2,Var3
	r1 = [Var1+1]
	cmp r1,[Var2+1]
	jb ?L_Ja32End#
	je ?L_Ja32Low#
	jmp Var3		
?L_Ja32Low#:
	r1 = [Var1]
	cmp r1,[Var2]
	jbe ?L_Ja32End#
	jmp Var3
?L_Ja32End#:
	.ENDM

//========================================================================================
//Macro Name:		M_FarJb32
//Parameters:		Var1(32bits,unsigned ),Var2(32bits,unsigned),L_Goto
//Return Value:		if[Var1]<[Var2],jump to Var3
//Description:
//Destroy:		r1,r2,r3
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Jb32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_Jb32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
//M_FarJb32: 	.MACRO	Var1,Var2,Var3
//		r1 = [Var1+1]
//		cmp r1,[Var2+1]
//		ja ?L_Jb32End#
//		je ?L_Jb32Low
//		goto Var3
//?L_Jb32Low:
//		r1 = [Var1]
//		cmp r1,[Var2]
//		jae ?L_Jb32End#
//		goto Var3
//?L_Jb32End#:
//		.ENDM

//change by guili 040305
M_FarJb32: .MACRO	Var1,Var2,Var3
	r1 = [Var1+1]
	cmp r1,[Var2+1]
	ja ?L_FarJb32End#
	je ?L_FarJb32Low#
	goto Var3
?L_FarJb32Low#:
	r1 = [Var1]
	cmp r1,[Var2]
	jae ?L_FarJb32End#
	goto Var3
?L_FarJb32End#:
	.ENDM

//========================================================================================
//Macro Name:		M_Jb32
//Parameters:		Var1(32bits,unsigned ),Var2(32bits,unsigned),L_Goto
//Return Value:
//Description:		if[Var1]<[Var2],jmp to Var3
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Jb32 RD_L_X,RD_L_Y,L_Goto
//Example:
//			M_Jb32 RD_L_X,RD_L_Y,L_Goto
//========================================================================================
M_Jb32: .MACRO	Var1,Var2,Var3
	r1 = [Var1+1]
	cmp r1,[Var2+1]
	ja ?L_Jb32End#
	je ?L_Jb32Low#
	jmp Var3
?L_Jb32Low#:
	r1 = [Var1]
	cmp r1,[Var2]
	jae ?L_Jb32End#
	jmp Var3
?L_Jb32End#:
	.ENDM

//========================================================================================
//Macro Name:		M_Not
//Parameters:		r1
//Return Value:		r1=~r1
//Description:
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Not
//Example:
//			M_Not
//========================================================================================
M_Not:	.MACRO
	r1^=0xffff
	.ENDM

//========================================================================================
//Macro Name:		M_Negative
//Parameters:		r1
//Return Value:		r1=~r1+1=-r1
//Description:
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_Negative
//Example:
//			M_Negative
//========================================================================================
M_Negative:	.MACRO
	r1 ^= 0xffff
	r1 += 1
	.ENDM


//========================================================================================
//Macro Name:		M_LSR1
//Parameters:		Var1,shift number
//Return Value:		[Var1] lsr 1 bit
//Description:		lsr shift [Var1]1 bit
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_LSR1 RW_L_X
//Example:
//			M_LSR1 RW_L_X
//========================================================================================
M_LSR1:	.MACRO	Var1
	r1 = [Var1]
	r1 = r1 lsr 1
	[Var1] = r1
	.ENDM

//========================================================================================
//Macro Name:		M_LSL1
//Parameters:		Var1,shift number
//Return Value:		[Var1] lsl 1 bit
//Description:		lsr shift [Var1]1 bit
//Destroy:		r1
//Memory Modified:
//Stack Depth:		0
//Usage:		M_LSL1 RW_L_X
//Example:
//			M_LSL1 RW_L_X
//========================================================================================
M_LSL1:	.MACRO	Var1
	r1 = [Var1]
	r1 = r1 lsl 1
	[Var1] = r1
	.ENDM

//========================================================================================
//Macro Name:		M_ROR1
//Parameters:		Var1,shift number
//Return Value:		[Var1] ror 1 bit
//Description:		ror shift [Var1]1 bit
//Destroy:		r1,r2
//Memory Modified:
//Stack Depth:		0
//Usage:		M_LSR1 RW_L_X,6
//Example:
//			M_LSR1 RW_L_X,6
//========================================================================================
M_ROR1:	.MACRO	Var1
	r1 = [Var1]
	r1 = r1 lsr 1
	r2 = r2 lsr 3
	r1 = r1 ror 1
	[Var1] = r1
	.ENDM

//========================================================================================
//Macro Name:		M_ZeroFarJmp
//Parameters:		flag
//Return Value:		none
//Description:		if Z=0,goto L_
//Destroy:		r1,r2,r3
//Memory Modified:
//Stack Depth:		0
//Usage:		M_ZeroFarJmp
//Example:
//			M_ZeroFarJmp
//========================================================================================
M_ZeroFarJmp:	.MACRO	 Lable
	jnz ?L_Goon#
	goto Lable
?L_Goon#:
	.ENDM


//========================================================================================
//Macro Name:		M_NZeroFARJMP
//Parameters:		destination address
//Return Value:
//Description:		z=0 then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_NZFARJMP L_End
//Example:
//			M_NZFARJMP L_End
//========================================================================================
M_NZeroFarJmp: .MACRO    Lable
	jz ?Skip#
	goto Lable
?Skip#:
	.ENDM

//========================================================================================
//Macro Name:		M_EquFarJmp
//Parameters:		destination address
//Return Value:
//Description:		if equal then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_EquFarJmp L_End
//Example:
//			M_EquFarJmp L_End
//========================================================================================
M_EquFarJmp:   .MACRO    Lable
	jne ?Skip#
	goto Lable
?Skip#:
	.ENDM

//========================================================================================
//Macro Name:		M_NEquFarJmp
//Parameters:		destination address
//Return Value:
//Description:		if not equal then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_NEquFarJmp L_End
//Example:
//			M_NEquFarJmp L_End
//========================================================================================
M_NEquFarJmp:  .MACRO    Lable
	je ?Skip#
	goto Lable
?Skip#:
	.ENDM

//========================================================================================
//Macro Name:		M_FarJb
//Parameters:		destination address
//Return Value:
//Description:		c=0 then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_FarJb L_End
//Example:
//			M_FarJb L_End
//========================================================================================
M_FarJb: .MACRO    Lable
	jnb  ?Skip#
	goto Lable
?Skip#:
	.ENDM

//guili add 040305
//========================================================================================
//Macro Name:		M_FarJbe
//Parameters:		destination address
//Return Value:
//Description:		c=0 then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_FarJbe L_End
//Example:
//			M_FarJbe L_End
//========================================================================================
M_FarJbe: .MACRO    Lable
	ja  ?Skip#
	goto Lable
?Skip#:
	.ENDM

//========================================================================================
//Macro Name:		M_FarJl
//Parameters:		destination address
//Return Value:
//Description:		s=0 then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_FarJl L_End
//========================================================================================
M_FarJl: .MACRO    Lable
	jge  ?Skip#
	goto Lable
?Skip#:
	.ENDM

//========================================================================================
//Macro Name:		M_FarJge
//Parameters:		destination address
//Return Value:
//Description:		s=1 then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_FarJge L_End
//========================================================================================
M_FarJge: .MACRO    Lable
	jl  ?Skip#
	goto Lable
?Skip#:
	.ENDM

//========================================================================================
//Macro Name:		M_FarJg
//Parameters:		destination address
//Return Value:
//Description:		s=1 then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_FarJg L_End
//========================================================================================
M_FarJg: .MACRO    Lable
	jle  ?Skip#
	goto Lable
?Skip#:
	.ENDM

//add by guili 040305
//========================================================================================
//Macro Name:		M_FarJa
//Parameters:		destination address
//Return Value:
//Description:		c=1 then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_FarJa L_End
//Example:
//			M_FarJa L_End
//========================================================================================
M_FarJa: .MACRO    Lable
	jna  ?Skip#
	goto Lable
?Skip#:
	.ENDM

//========================================================================================
//Macro Name:		M_FarJe
//Parameters:		destination address
//Return Value:
//Description:		s=0 then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:		M_FarJe L_End
//========================================================================================
M_FarJe: .MACRO    Lable
	jne  ?Skip#
	goto Lable
?Skip#:
	.ENDM
	
//========================================================================================
//Macro Name:		M_FarJne
//Parameters:		destination address
//Return Value:
//Description:		s=1 then goto
//Destroy:
//Memory Modified:
//Stack Depth:		0
//Usage:			M_FarJne L_End
//========================================================================================	

M_FarJne: .MACRO    Lable
	je  ?Skip#
	goto Lable
?Skip#:
	.ENDM