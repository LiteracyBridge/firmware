//===========================================================================
//      The information contained herein is the exclusive property of
//      Sunplus Technology Co。 And shall not be distributed, reproduced,
//      or disclosed in whole in part without prior written permission
//      (C) COPYRIGHT 2001   SUNPLUS TECHNOLOGY CO
//      ALL RIGHTS RESERVED
//      The entire notice above must be reproduced on all authorized copies
//===========================================================================

//===========================================================================
//  Filename:	sys_gv.h
//  Author: 	XT2
//  Tel: 		62981668-2752
//  Date:
//  Description:system global variables definition
//  Reference:
//  Version history:
//---------------------------------------------------------------------------
//  Version   YYYY-MM-DD-INDEX   Modified By         Description
//---------------------------------------------------------------------------
//===========================================================================
#ifndef _SYS_GV_H
#define _SYS_GV_H

#ifdef    SYS_CREATEVAR
	#define SYS_EXT
#else
	#define SYS_EXT extern
#endif

//-----wait mode start-----
SYS_EXT unsigned int gWaitModeFlag;			//可否允许进入WaitMode 的标志位
SYS_EXT unsigned int gHaltModeFlag;			//可否关机的标志位

SYS_EXT unsigned int gCurrentAutoOffTime;	//自动关机当前时间
SYS_EXT unsigned int gAutoOffTime;			//自动关机时间

SYS_EXT unsigned int gOnOffKey;				//用户按下OnOff Key的标志位，关机到开机(OFF2ON)，开机到关机(ON2OFF)。
SYS_EXT unsigned int gState;				//全局状态,就只有两种状态，分别是SYS_ON 和 SYS_OFF，表示开关机状态.
//-----wait mode end-----

//===========================================================================
//===========================================================================
//===========================================================================
//internal ram分配情况
//start from 0
//(1)---_sn_ram_end:全局变量
//(2)---BS_CONFIG_BS_END(配置BS_STACK_RESERVE_SIZE=0)=0x5FFF:buddy
//(3)---0x6B40:LCD Buffer(160*96 mono使用0x3C0 words,共预留3个buffer(lcd buffer+common backup buffer for GUI+pop message backup buffer forGUI))
//(4)---0x77ff:user stack(3.2KW)
//===========================================================================
//===========================================================================
//===========================================================================
#endif