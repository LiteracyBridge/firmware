//=======================================================================
//  The information contained herein is the exclusive property of
//  Sunnnorth Technology Co。 And shall not be distributed, reproduced,
//  or disclosed in whole in part without prior written permission。
//  (C) COPYRIGHT 2003  SUNNORTH TECHNOLOGY CO。
//  ALL RIGHTS RESERVED
//  The entire notice above must be reproduced on all authorized copies。
//=======================================================================
//=======================================================================
//  Filename:      sysoff2onhook.asm
//  Programmer:	   XT2
//  Head file: 	   syswaitmode.h
//  Version:       1.0.0
//  Date:          2006-06-19
//  Applied body:  GPL162001
//  Description:   hook function for system from off to on
//  Revision history:
// ----------------------------------------------------------------------
//  Version, YYYY-MM-DD-Index, Modified By, Description
// ----------------------------------------------------------------------
//  None
//=======================================================================
.INCLUDE ./system/include/system_head.inc

.CODE

.PUBLIC _SysOff2OnHook
_SysOff2OnHook:	.PROC
//rhm TB has no LCD	__setb P_LCD_Setup,13,R1				//enable lcd module
	__setb P_TimeBaseC_Ctrl ,13,R1			//set timebasec interrupt

	__setb P_RTC_INT_Ctrl ,0,R1				//set half second
	__setb P_RTC_INT_Ctrl ,1,R1				//set  second
	__setb P_RTC_INT_Ctrl ,2,R1				//set  miniute
	__setb P_RTC_INT_Ctrl ,3,R1				//set  hour
	__setb P_RTC_INT_Ctrl ,8,R1				//set  schedule

	//__setb P_TP_Ctrl ,14,R1				//set  touch panel int
	RETF
.ENDP

.END
