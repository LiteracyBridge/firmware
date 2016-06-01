//==========================================================================
//      The information contained herein is the exclusive property of
//      Sunplus Technology Co¡£ And shall not be distributed, reproduced,
//      or disclosed in whole in part without prior written permission
//      (C) COPYRIGHT 2001   SUNPLUS TECHNOLOGY CO
//      ALL RIGHTS RESERVED
//      The entire notice above must be reproduced on all authorized copies
//==========================================================================
//==========================================================================
//  File Name:		syswaitmode.inc
//  Programmer:     xt2
//  Version:        1.0.0
//  Date£º          2006-06-19
//  Applied body:   gpl162001
//  Description:    header file for system wait mode function
//  Revision history:
// -------------------------------------------------------------------------
//  Version, YYYY-MM-DD-Index, File-Name: Modified By, Description
// -------------------------------------------------------------------------
//  None
//==========================================================================
.ifndef	__SYSWAITMODE_INC__
.define	__SYSWAITMODE_INC__

//LCD SETUP
.define C_LCD_Segment	160
.define C_LCD_Common	96
.define C_LCD_FrameRate	160//60//:for mono;120:for gray or color
.define C_SystemClock	96000000
.define C_LowSystemClock	12000000

.define SYS_96MCLOCK		0x8418
.define SYS_12MCLOCK		0x0400
.define LCD_96MCLOCK		(C_SystemClock/C_LCD_Segment/C_LCD_Common/C_LCD_FrameRate-2)
.define LCD_12MCLOCK		(C_LowSystemClock/C_LCD_Segment/C_LCD_Common/C_LCD_FrameRate-2)

.define WAITMODE_CONST  	0x5005
.define HALTMODE_CONST  	0x500a

.define SYS_ON2OFF			1
.define SYS_OFF2ON			2
.define SYS_NULL			4
.define SYS_NOTONOFF		8

.define SYS_ON				1
.define SYS_OFF				2

//WaitMode Flag
.define WAITMODE_TIMEDLY	0x0001
.define WAITMODE_QPEND		0x0002
.define WAITMODE_TIMER		0x0004
.define WAITMODE_CHANNEL_A	0x0008
.define WAITMODE_CHANNEL_B	0x0010

.external _SysSetOnOffKey
.external _SysGetOnOffKey
.external _SysGetWaitModeFlag
.external _SysGetHaltModeFlag
.external _SysSetState
.external _SysGetState

SysGetOnOffKey:	.macro
	CALL	_SysGetOnOffKey
	.endm

SysSetOnOffKey:	.macro value
	R1 = value
	push R1 to [sp]
	CALL _SysSetOnOffKey
	SP += 1
	.endm
	
SysGetWaitModeFlag:	.macro
	CALL _SysGetWaitModeFlag
	.endm
	
SysGetHaltModeFlag:	.macro
	CALL _SysGetHaltModeFlag
	.endm

SysGetState:	.macro
	CALL _SysGetState
	.endm

SysSetState:	.macro value
	R1 = value
	push R1 to [sp]
	CALL _SysSetState
	SP += 1
	.endm

.endif