//===========================================================================
//      The information contained herein is the exclusive property of
//      Sunplus Technology Co¡£ And shall not be distributed, reproduced,
//      or disclosed in whole in part without prior written permission
//      (C) COPYRIGHT 2001   SUNPLUS TECHNOLOGY CO
//      ALL RIGHTS RESERVED
//      The entire notice above must be reproduced on all authorized copies
//===========================================================================

//===========================================================================
//  Filename:	message.h
//  Author: 	XT2
//  Tel: 		62981668-2752
//  Date:
//  Description:header file for message functions
//  Reference:
//  Version history:
//---------------------------------------------------------------------------
//  Version   YYYY-MM-DD-INDEX   Modified By         Description
//---------------------------------------------------------------------------
//===========================================================================
#ifndef _MESSAGE_H_
#define _MESSAGE_H_




/****************************************************************************
*			define msg type
******************************************************************************/
#define MSG_INT_LVD1				0x0001
#define MSG_INT_LVD2				0x0002
#define MSG_INT_LVD3				0x0003
#define MSG_INT_LVD4				0x0004
#define MSG_INT_LVD5				0x0005
#define MSG_INT_LVD6				0x0006
#define MSG_INT_LVD7				0x0007
#define MSG_INT_LVD8				0x0008

#define MSG_INT_YEAR				0x000A
#define MSG_INT_MONTH				0x000B
#define MSG_INT_DAY					0x000C
#define MSG_INT_HOUR				0x000D
#define MSG_INT_MINUTE				0x000E
#define MSG_INT_SECOND				0x000F
#define MSG_INT_HALFSEC				0x0010

#define MSG_INT_REMINDER1	        0x0018
#define MSG_INT_REMINDER2	        0x0019
#define MSG_INT_REMINDER3	        0x001A
#define MSG_INT_REMINDER4	        0x001B

#define MSG_INT_EXTPIN1  	        0x0020
#define MSG_INT_EXTPIN2  	        0x0021
#define MSG_INT_EXTPIN3  	        0x0022
#define MSG_INT_EXTPIN4  	        0x0023

#define MSG_INT_INTKEY	            0x0031
#define MSG_INT_EXTKEY	            0x0032
#define MSG_COUNTDOWNTIME	        0x0033
#define MSG_CHANGESCHEDULE	        0x0034
#define MSG_ANNI_CH					0x0035
#define MSG_CHANGEDAILY		        0x0036
#define MSG_CHANGECLOCK		        0x0037
#define MSG_CHANGESYSTIME			0x0038

#define MSG_SND_BEGIN               0x0040
#define MSG_SND_END                 0x0041
#define MSG_SND_STOP                0x0042
#define MSG_SND_RECBEGIN            0x0043
#define MSG_SND_RECEND              0x0044
#define MSG_SND_RECFULL             0x0045

#define MSG_INT_TP			    	0x0046

#define MSG_AP_RESTART		        0x0050
#define MSG_AP_SWAPIN 		        0x0051
#define MSG_AP_SWAPOUT		        0x0052
#define MSG_AP_RQEXIT 		        0x0053
#define MSG_AP_ACKEXIT		        0x0054
#define MSG_AP_SUICIDE		        0x0055
#define MSG_AP_REDRAW 		        0x0056
#define MSG_KEYHELP					0x0057
#define	MSG_OFF2ON					0x0058
#define	MSG_ON2OFF					0x0059
#define	MSG_ON2OFF_ACK				0x005A
#define MSG_INT_EXTKEY_UP			0x005B

#endif