//=========================================================================
//  The information contained herein is the exclusive property of Sunplus
//  Technology Co. And shall not be distributed, reproduced, or disclosed
//  in whole in part without prior written permission.
//  (C) Copyright 2003 Sunplus Technology Co. All rights reserved.
//  The entire notice above must be reproduced on all authorized copies
//=========================================================================

//=========================================================================
//  project  name:  	 Buddy System
//  Project Description: Buddy SystemGlobal Variable declare
//  Environment:
//
//  Library Dependency:
//
//  Author: 	xiaoping
//  Date:		2005/04/15
//  E-mail:
//  Revision history:
//  ------------------------------------------------------------------------
//  YYYY-MM-DD        Modified By               Description
//  ------------------------------------------------------------------------
//==========================================================================

#ifndef	_UCBS_GV_H_
#define	_UCBS_GV_H_

#include 	"ucBS_CFG.h"
#include  "ucBS_CPU.h"
extern int _sn_ram_end;
extern int _sn_sp_val;

typedef	struct stFreeArea
{
	struct stFreeArea *m_Next;
	struct stFreeArea *m_Prev;
	INT16U	m_ListNum;
}STFreeArea;

typedef  struct stListHead
{
	STFreeArea* m_First;

} STListHead;



#ifdef CREATE_BS_VAR
    #define BS_EXT
#else
    #define BS_EXT  extern
#endif

BS_EXT STListHead      BS_ListHead [BS_NUM ];



BS_EXT BSINTU     		BS_RamEnd;			//全局变量后的空间

BS_EXT BSINTU      		BS_Start;			//Buddy 开始的空间(S偷偷占用的空间开始)

BS_EXT BSINTU      	BS_MemStart;		//Buddy 开始管理的空间

BS_EXT BSINTU      	BS_End; 		    // Buddy 管理结束的空间


#if BS_STAT_MEM_EN > 0

BS_EXT BSINTU		BS_BlockMaxUsed[BS_NUM];//Block 峰值记录
BS_EXT BSINTU		BS_BlockRealUsed[BS_NUM];//Block 峰值记录
BS_EXT BSINTU 		gMaxMemUsed;			//Buddy 峰值使用记录
BS_EXT BSINTU 		gRealMemUsed; 			//当前使用的内存大小记录

#endif

#ifdef CREATE_BS_VAR
const BSINTU ucBS_RecordMask[] =
{
	0xfff0,0xff0f,0xf0ff,0x0fff
};
const BSINTU ucBS_unMask[] =
{
	0x000f,0x00f0,0x0f00,0xf000
};
const BSINTU ucBS_NUM		 		= BS_NUM;
const BSINTU ucBS_MIN_BLOCK 	 		= BS_MIN_BLOCK;
const BSINTU	ucBS_MAX_BLOCK		 	= BS_MAX_BLOCK;
const BSINTU	ucBS_STACK_RESERVE_SIZE	= BS_STACK_RESERVE_SIZE;
const BSINTU ucBS_LIST_MERGE_SIZE		= BS_LIST_MERGE_SIZE;
const BSINTU	ucBS_MEM_RECORD_SIZE	= BS_MEM_RECORD_SIZE;
const BSINTU ucBS_RECORD_BYTE_SIZE	= BS_RECORD_BYTE_SIZE;

#ifdef	BS_CONFIG_RAM_END				//zhouli add for config ram end 2006/04/13
const BSINTU ucBS_SN_RAM_END = BS_CONFIG_RAM_END;
#else
//const BSINTU ucBS_SN_RAM_END = _sn_ram_end 和const BSINTU ucBS_SN_RAM_END = &_sn_ram_end
//编译都不能通过，所以用const BSINTU *ucBS_SN_RAM_END = &_sn_ram_end。
const BSINTU *ucBS_SN_RAM_END = &_sn_ram_end;
#endif

#ifdef	BS_CONFIG_BS_END			//zhouli modify for config buddy system end. 2007/03/07
const BSINTU ucBS_SN_BS_END = BS_CONFIG_BS_END;
#else
const BSINTU *ucBS_SN_BS_END = &_sn_sp_val;
#endif

#else
extern const BSINTU ucBS_unMask[];
extern const BSINTU ucBS_RecordMask[];
extern const BSINTU ucBS_NUM;
extern const BSINTU ucBS_MIN_BLOCK;
extern const BSINTU	ucBS_MAX_BLOCK;
extern const BSINTU	ucBS_STACK_RESERVE_SIZE;
extern const BSINTU ucBS_LIST_MERGE_SIZE;
extern const BSINTU	ucBS_MEM_RECORD_SIZE;
extern const BSINTU ucBS_RECORD_BYTE_SIZE;
extern const BSINTU ucBS_SN_RAM_END;	//zhouli add for config ram end 2006/04/13
extern const BSINTU ucBS_SN_BS_END;		//zhouli modify for config buddy system end. 2007/03/07
#endif



typedef STListHead ucSTListHead;
typedef STListHead* ucSTListHeadPtr;

typedef STFreeArea ucSTFreeArea;
typedef STFreeArea* ucSTFreeAreaPtr;


//#define BS_SET_ERROR_INFO(param)	param = param;
//#define BS_CLEAR_ERROR_INFO()

#endif