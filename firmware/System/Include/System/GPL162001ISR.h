//===========================================================================
//      The information contained herein is the exclusive property of
//      Sunplus Technology Co¡£ And shall not be distributed, reproduced,
//      or disclosed in whole in part without prior written permission
//       (C) COPYRIGHT 2004   SUNPLUS TECHNOLOGY CO
//       ALL RIGHTS RESERVED
//       The entire notice above must be reproduced on all authorized copies
//===========================================================================
//  Filename   : course.h
//  Author     : Mona (Email:Chengang@sunnorth.com.cn)
//  Tel        : 010-62981668-2412
//  Date       : 2004-03-05
//  Description:  unOS ISR Vector number define
//
//  Reference  :
//
//  Version history:
//---------------------------------------------------------------------------
//  Version   YYYY-MM-DD-INDEX   Modified By         Description
//---------------------------------------------------------------------------

#ifndef	__SPL162001ISR_h__
#define	__SPL162001ISR_h__

/***************************************************************************
According SPL2001's datasheet 
P_INT_Status1.0     0
P_INT_Status1.1     1
...
P_INT_Status1.15    15
P_INT_Status2.0     16
P_INT_Status2.1     17
...
P_INT_Status2.1     31
***************************************************************************/

#define		INT_EXTA				0
#define		INT_EXTB				1
#define     INT_DMA                 2
#define     INT_USB                 3
#define     INT_AUDIOA              4
#define     INT_AUDIOB              5
#define     INT_SPBUS               6
#define     INT_ADC_AUTO            7
#define     INT_TOUCHPANEL          8
#define     INT_LCD_FP              9
#define     INT_SPI                 10
#define     INT_UART_IRDA           11
#define		INT_CMOS_HALFFULL   	12
#define		INT_CMOS_FULL   	    13
#define		INT_ADC_READY			14
#define		INT_KEY					15
#define     INT_HMS                 16
#define     INT_ALARM               17
#define     INT_SCHED_INT           18
#define     INT_SHUTDOWNNB          19
#define     INT_NANDFLASH           20
#define     INT_I2C                 21
#define     INT_SD                  22
#define     INT_LVD                 23
#define     INT_TIMER_BASEA         24
#define     INT_TIMER_BASEB         25
#define     INT_TIMER_BASEC         26
#define     INT_KEYSCAN             27
#define		INT_TIMERA				28
#define		INT_TIMERB				29
#define		INT_TIMERC				30
#define		INT_TIMERD				31

#endif
