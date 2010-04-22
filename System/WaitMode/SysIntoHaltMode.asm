//=======================================================================
//  The information contained herein is the exclusive property of
//  Sunnnorth Technology Co。 And shall not be distributed, reproduced,
//  or disclosed in whole in part without prior written permission。
//  (C) COPYRIGHT 2003  SUNNORTH TECHNOLOGY CO。
//  ALL RIGHTS RESERVED
//  The entire notice above must be reproduced on all authorized copies。
//=======================================================================
//=======================================================================
//  Filename:      sysintohaltmode.asm
//  Programmer:	   XT2
//  Head file: 	   syswaitmode.h
//  Version:       1.0.0
//  Date:          2006-06-19
//  Applied body:  GPL162001
//  Description:   system get into halt mode
//  Revision history:
// ----------------------------------------------------------------------
//  Version, YYYY-MM-DD-Index, Modified By, Description
// ----------------------------------------------------------------------
//  None
//=======================================================================
.INCLUDE ./system/include/system_head.inc
.PUBLIC _SysIntoHaltMode

.EXTERNAL _SysOn2OffHook
//.EXTERNAL _EnableKeyChangInt
.EXTERNAL _SysWaitLCDFP

.CODE

_SysIntoHaltMode: .PROC
	call _SysOn2OffHook

	SysSetState	SYS_OFF

//	call	_EnableKeyChangInt

?L_enter_halt_mode:
	call	_SysWaitLCDFP
	r1 = HALTMODE_CONST
	[P_HALT]=r1
	jmp ?L_enter_halt_mode
	nop
	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop
	nop
	
	jmp $
	
	retf
	.ENDP
	
.END


