//=======================================================================
//  The information contained herein is the exclusive property of
//  Sunnnorth Technology Co。 And shall not be distributed, reproduced,
//  or disclosed in whole in part without prior written permission。
//  (C) COPYRIGHT 2003  SUNNORTH TECHNOLOGY CO。
//  ALL RIGHTS RESERVED
//  The entire notice above must be reproduced on all authorized copies。
//=======================================================================
//=======================================================================
//  Filename:      sysintohighspeed.asm
//  Programmer:	   XT2
//  Head file: 	   syswaitmode.h
//  Version:       1.0.0
//  Date:          2006-06-19
//  Applied body:  GPL162001
//  Description:   system get into high speed
//  Revision history:
// ----------------------------------------------------------------------
//  Version, YYYY-MM-DD-Index, Modified By, Description
// ----------------------------------------------------------------------
//  None
//=======================================================================
.INCLUDE ./system/include/system_head.inc
.PUBLIC _SysIntoHighSpeed

.CODE

_SysIntoHighSpeed: .PROC
	r1=[P_Clock_Ctrl]
	cmp r1,SYS_96MCLOCK
	je ?ret

?WaitPll0:
	r1 = [P_Power_State]
	r1 &= 0x07
	jz ?WaitPll0

	r1 = [P_Clock_Ctrl]
	r1 &= 0x7fff
	[P_Clock_Ctrl] = r1

?WaitPll1:
	r1 = [P_Power_State]
	r1 &= 0x07
	jz ?WaitPll1

	r1=96/3					// PLL clock =48MHz
	[P_PLLChange]=r1

	r1=SYS_96MCLOCK
	[P_Clock_Ctrl]=r1		// pll enable, system clock=PLL clock

?chk_pws:
	r1 = [P_Power_State]
	r1 &= 0x0007
	cmp r1, 0x0002
	jnz ?chk_pws

	
	R1 = LCD_96MCLOCK
	__outp P_LCD_Clock,R1

?ret:
	RETF
	.ENDP
	
.END	
