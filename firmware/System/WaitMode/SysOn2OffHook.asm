//=======================================================================
//  The information contained herein is the exclusive property of
//  Sunnnorth Technology Co。 And shall not be distributed, reproduced,
//  or disclosed in whole in part without prior written permission。
//  (C) COPYRIGHT 2003  SUNNORTH TECHNOLOGY CO。
//  ALL RIGHTS RESERVED
//  The entire notice above must be reproduced on all authorized copies。
//=======================================================================
//=======================================================================
//  Filename:      syson2offhook.asm
//  Programmer:	   XT2
//  Head file: 	   syswaitmode.h
//  Version:       1.0.0
//  Date:          2006-06-19
//  Applied body:  GPL162001
//  Description:   hook function for system from on to off
//  Revision history:
// ----------------------------------------------------------------------
//  Version, YYYY-MM-DD-Index, Modified By, Description
// ----------------------------------------------------------------------
//  None
//=======================================================================
.INCLUDE ./system/include/system_head.inc

.CODE

.PUBLIC _SysOn2OffHook
_SysOn2OffHook:	.PROC
	__clrb P_LCD_Setup,13,R1				//disable lcd module
	__clrb P_TimeBaseC_Ctrl ,13,R1			//clear timebasec interrupt

	__clrb P_RTC_INT_Ctrl ,0,R1				//clear half second
	__clrb P_RTC_INT_Ctrl ,1,R1				//clear  second
	__clrb P_RTC_INT_Ctrl ,2,R1				//clear  miniute
	__clrb P_RTC_INT_Ctrl ,3,R1				//clear  hour
	__clrb P_RTC_INT_Ctrl ,8,R1				//clear  schedule

	//__clrb P_TP_Ctrl ,14,R1				//clear  touch panel int

	RETF
.ENDP

.END