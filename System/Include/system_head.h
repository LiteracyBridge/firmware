#ifndef _SYSTEM_HEAD_H_
#define _SYSTEM_HEAD_H_

#include ".\System\include\system\GPL162002.h"
#include ".\System\include\system\GPL162001ISR.h"
#include ".\System\include\system\SysException.h"
#include ".\System\include\system\Sys_GV.h"
#include ".\System\include\WaitMode\SysWaitMode.h"
#include ".\System\include\Message\Message.h"
#include ".\System\include\ExtMem\unExtMem.h"
#include ".\System\include\buddy\ucBS.h"
#include ".\system\include\system\unSPMACRO.h"

unsigned int ServiceLoop(void);
unsigned int CheckSystemStatus(void);
#endif