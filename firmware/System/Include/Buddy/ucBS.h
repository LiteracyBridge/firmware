//=========================================================================
//  The information contained herein is the exclusive property of Sunplus
//  Technology Co. And shall not be distributed, reproduced, or disclosed
//  in whole in part without prior written permission.
//  (C) Copyright 2003 Sunplus Technology Co. All rights reserved.
//  The entire notice above must be reproduced on all authorized copies
//=========================================================================

//=========================================================================
//  project  name:  	u'nSP Buddy System
//  Project Description:u'nSP Buddy System on MiniOS
//  Environment: 	SPL16256 with 47.9232MHz
//					SUNPLUS u'nSPTM  IDE 1.8.1£¨or later£©
//  Library Dependency: OS-Lib: SpOSA153.lib
//
//  Author: 	wang quan
//  Date:		2003.12.10
//  E-mail:     unsp@sunnorth.com.cn
//  Revision history:
//  ------------------------------------------------------------------------
//  YYYY-MM-DD        Modified By               Description
//  ------------------------------------------------------------------------
//==========================================================================

#ifndef	_UCBS_H_
#define	_UCBS_H_
//	write your header here
//zhouli add for exculding external buddy. 2007/03/07
//#define INCLUDE_EXTERNAL_BUDDY 1

/*************************************************/
#ifndef BS_NO_ERR
#define BS_NO_ERR 	0
#endif

/*************************************************/
#ifndef FALSE
#define FALSE 		0
#endif

/*************************************************/
#ifndef TRUE
#define TRUE 		1
#endif

/*************************************************/
#ifndef BS_NULL
#define BS_NULL	0
#endif

/*************************************************/
#include "ucBS_CPU.h"
#include "ucBS_CFG.h"
#include "ucBS_GV.h"

#ifdef	INCLUDE_EXTERNAL_BUDDY
//#ifdef CREATE_BSEXT_VAR
#include "ucBSExt_CFG.h"
#include "ucBSExt_GV.h"
//#endif
#endif	//INCLUDE_EXTERNAL_BUDDY

BSINTU  ucBSInit(void);

BSPTR ucBSMalloc(BSINTU size);
void ucBSFree(BSPTR ptr);

#if BS_STAT_MEM_EN > 0
void ucBS_GetStatus(BSINTU* pMaxUsed,BSINTU* pMinFreed);
BSINTU ucBS_GetTotalMem();
BSINTU ucBS_GetBlockMaxUsed(BSINTU index);
#endif

#ifdef	INCLUDE_EXTERNAL_BUDDY
//zhouli modify for reset ext memory size. 2006/06/02
//BSINT32U  ucBSExtInit(void);
BSINT32U  ucBSExtInit(STExtSection *ExtSectionCFG);

BSINT32U ucBSExtMalloc(BSINT32U  size);
void ucBSExtFree(BSINT32U ptr);

//zhouli 2006/08/07, add for large block malloc.
//BSINT32U ucBSExtMalloc_L(BSINT32U  size,BSPTR buffer);
//void ucBSExtFree_L(BSPTR buffer);

//zhouli modify for unite. 2006/08/23
BSINT32U ucBSExtMalloc_A(BSINT32U  size, BSPTR buffer);
void ucBSExtFree_A(BSPTR buffer);

#if BS_STAT_MEM_EN > 0
BSINT32U ucBSExt_GetTotalMem();
void ucBSExt_GetStatus(BSINT32U* pMaxUsed,BSINT32U* pMinFreed);
BSINTU ucBSExt_GetBlockMaxUsed(BSINTU index);
#endif

#endif	//INCLUDE_EXTERNAL_BUDDY

#endif