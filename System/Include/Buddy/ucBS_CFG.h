//=========================================================================
//  The information contained herein is the exclusive property of Sunplus 
//  Technology Co. And shall not be distributed, reproduced, or disclosed 
//  in whole in part without prior written permission.
//  (C) Copyright 2003 Sunplus Technology Co. All rights reserved.
//  The entire notice above must be reproduced on all authorized copies
//=========================================================================

//=========================================================================
//  project  name:           Buddy System 
//  Project Description: Buddy System Configure File
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

#ifndef	_UCBS_CFG_H_
#define	_UCBS_CFG_H_

#define 	BS_NUM					7                       
#define 	BS_MIN_BLOCK			16              //Min Block Size
#define 	BS_MAX_BLOCK			1024 			//Max Block Size
#define 	BS_STACK_RESERVE_SIZE	0				//Stack Reserve Size
#define 	BS_LIST_MERGE_SIZE		4               //When the list up to this size ,merge the list

#define		BS_RECORD_BYTE_SIZE		1				//16/16 = 1

#define		BS_MEM_RECORD_SIZE		4				//16/4 =4

#define		BS_STAT_MEM_EN		1					//static memstatus

#define UCBS_DEBUG

//zhouli add for config ram end 2006/04/13
//if user want to config the start address for internal buddy system, define it;
//else, mask it.
//#define BS_CONFIG_RAM_END	0x1000

//zhouli modify for config buddy system end. 2007/03/07
//if user want to config the end address for internal buddy system, define it;
//else, mask it.
#define	BS_CONFIG_BS_END	0x5FFF

#endif