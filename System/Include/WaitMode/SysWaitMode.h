//===========================================================================
//      The information contained herein is the exclusive property of
//      Sunplus Technology Co¡£ And shall not be distributed, reproduced,
//      or disclosed in whole in part without prior written permission
//      (C) COPYRIGHT 2001   SUNPLUS TECHNOLOGY CO
//      ALL RIGHTS RESERVED
//      The entire notice above must be reproduced on all authorized copies
//===========================================================================

//===========================================================================
//  Filename:	syswaitmode.h
//  Author: 	XT2
//  Tel: 		62981668-2752
//  Date:  		2006.06.19
//  Description:header file for system wait mode function
//  Reference:
//  Version history:
//---------------------------------------------------------------------------
//  Version   YYYY-MM-DD-INDEX   Modified By         Description
//---------------------------------------------------------------------------
//===========================================================================
#ifndef _SYSWAITMODE_H_
#define _SYSWAITMODE_H_

//LCD SETUP
#define C_LCD_Segment	160
#define C_LCD_Common	96
#define C_LCD_FrameRate	160//60//:for mono;120:for gray or color
#define C_SystemClock	96000000
#define C_LowSystemClock	12000000

#define SYS_96MCLOCK		0x8418
#define SYS_12MCLOCK		0x0400
#define LCD_96MCLOCK		(C_SystemClock/C_LCD_Segment/C_LCD_Common/C_LCD_FrameRate-2)
#define LCD_12MCLOCK		(C_LowSystemClock/C_LCD_Segment/C_LCD_Common/C_LCD_FrameRate-2)

#define WAITMODE_CONST  	0x5005
#define HALTMODE_CONST  	0x500a

//on/off key
#define SYS_ON2OFF			0x1
#define SYS_OFF2ON			0x2
#define SYS_NULL			0x4
#define SYS_NOTONOFF		0x8

//state
#define SYS_ON				0x1
#define SYS_OFF				0x2

//WaitMode Flag
#define WAITMODE_TIMEDLY		0x0001
#define WAITMODE_QPEND			0x0002
#define WAITMODE_TIMER			0x0004
#define WAITMODE_CHANNEL_A		0x0008
#define WAITMODE_CHANNEL_B		0x0010
#define WAITMODE_UART			0x0020
#define WAITMODE_DAMSEM			0x0040

void SysInitWaitMode(void);

void SysEnableWaitMode(unsigned int flag);
void SysDisableWaitMode(unsigned int flag);
unsigned int SysGetWaitModeFlag(void);

void SysDisableHaltMode(unsigned int flag);
void SysEnableHaltMode(unsigned int flag);
unsigned int SysGetHaltModeFlag(void);

void SysReloadAutoOffTime(void);
void SysSetAutoOffTime(unsigned int autoofftime);
void SysDecAutoOffTime(void);
unsigned int SysGetCurrentOffTime(void);

void SysSetOnOffKey(unsigned int value);
unsigned int SysGetOnOffKey(void);

void SysSetState(unsigned int value);
unsigned int SysGetState(void);

void SysOff2OnHook(void);
void SysOn2OffHook(void);
void SysIntoHighSpeed(void);
void SysIntoWaitMode(void);
void SysIntoHaltMode(void);
unsigned int SetSystemClockRate(unsigned int);

#endif