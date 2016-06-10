#ifndef  _SYSEXCEPTION_H_
#define _SYSEXCEPTION_H_

#define BS_ASSERT_EQUAL			1
#define BS_ASSERT_INAREA		2
#define BS_ASSERT_NOTEQUAL		3
#define BS_ASSERT_LESS			4

#define BS_GET_MAXINDEX			5
#define BS_FREE_INVALID			6
#define BS_FREE_ADDR_INVALID	7

#define EXTMEM_INVALID_ADDR		15

void Sys_Exception(unsigned int errorcode);

#endif

