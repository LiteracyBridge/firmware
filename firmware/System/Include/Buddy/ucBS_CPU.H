//===========================================================================
//      The information contained herein is the exclusive property of
//      Sunplus Technology Co¡£ And shall not be distributed, reproduced,
//      or disclosed in whole in part without prior written permission
//       (C) COPYRIGHT 2005   SUNPLUS TECHNOLOGY CO
//       ALL RIGHTS RESERVED
//       The entire notice above must be reproduced on all authorized copies
//===========================================================================
//  Filename   : xxxx.h
//  Author     : xiaoping (Email:xiaoping@sunnorth.com.cn)
//  Tel        : 010-62981668-2733
//  Date       : 2005-03-05
//  Description: unOS Type define 
//
//  Reference  :
//
//  Version history:
//---------------------------------------------------------------------------
//  Version   YYYY-MM-DD-INDEX   Modified By         Description
//---------------------------------------------------------------------------
#ifndef _UCBS_CPU_H_
#define _UCBS_CPU_H_

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/
#ifndef _BOOLEAN_TYPE_
#define _BOOLEAN_TYPE_
typedef unsigned int  BOOLEAN;
#endif

#ifndef _INT8U_TYPE_
#define _INT8U_TYPE_
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
#endif

#ifndef _INT8S_TYPE_
#define _INT8S_TYPE_
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
#endif

#ifndef _INT16U_TYPE_
#define _INT16U_TYPE_
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
#endif

#ifndef _INT16S_TYPE_
#define _INT16S_TYPE_
typedef signed   int   INT16S;                   /* Signed   16 bit quantity                           */
#endif

#ifndef _INT32U_TYPE_
#define _INT32U_TYPE_
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
#endif

#ifndef _INT32S_TYPE_
#define _INT32S_TYPE_
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
#endif

#ifndef _FP32_TYPE_
#define _FP32_TYPE_
typedef float            FP32;                     /* Single precision floating point                    */
#endif

#ifndef _FP64_TYPE_
#define _FP64_TYPE_
typedef double           FP64;                     /* Double precision floating point                    */
#endif

#ifndef _BSPTR_TYPE_
#define _BSPTR_TYPE_
typedef unsigned int*  BSPTR;
#endif

#ifndef _BSINTU_TYPE_
#define _BSINTU_TYPE_
typedef unsigned int  BSINTU;
#endif

#ifndef _BSINTUPTR_TYPE_
#define _BSINTUPTR_TYPE_
typedef unsigned int*   BSINTUPTR;
#endif

#ifndef _BSINT32U_TYPE_
#define _BSINT32U_TYPE_
typedef unsigned long BSINT32U;
#endif

#ifndef _BSINT32UPTR_TYPE_
#define _BSINT32UPTR_TYPE_
typedef unsigned long BSINT32UPTR;
#endif

#ifndef _BSINTS_TYPE_
#define _BSINTS_TYPE_
typedef int     BSINTS;
#endif

#endif//#define _UNOS_CPU_H_