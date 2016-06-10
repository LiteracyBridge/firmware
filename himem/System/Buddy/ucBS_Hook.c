//===========================================================================
//      The information contained herein is the exclusive property of
//      Sunplus Technology Co¡£ And shall not be distributed, reproduced,
//      or disclosed in whole in part without prior written permission
//      (C) COPYRIGHT 2001   SUNPLUS TECHNOLOGY CO
//      ALL RIGHTS RESERVED
//      The entire notice above must be reproduced on all authorized copies
//===========================================================================

//===========================================================================
//  Filename:	ucBS_hook.c
//  Author: 	XT2
//  Tel: 		62981668-2752
//  Date:  		2006.06.19
//  Description:hook functions for buddy
//  Reference:
//  Version history:
//---------------------------------------------------------------------------
//  Version   YYYY-MM-DD-INDEX   Modified By         Description
//---------------------------------------------------------------------------
//  None
//===========================================================================
#include "./system/include/system_head.h"

void ucBSMalloc_EnterHook(BSINTU  size)
{
//	EA = 0;

}
void ucBSMalloc_ExitHook(BSPTR ptr)
{
//	EA = 1;

}
void ucBSFree_EnterHook(BSPTR ptr)
{
//	EA = 0;

}
void ucBSFree_ExitHook(BSPTR ptr)
{
//	EA = 1;

}

//zhouli add for packup functions. 2006/08/15
void ucBS_Packup_EnterHook(void)
{
	
}
void ucBS_Packup_ExitHook(void)
{
	
}
