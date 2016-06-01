//=========================================================================
//      The information contained herein is the exclusive property of
//      Sunplus Technology Co¡£ And shall not be distributed, reproduced,
//      or disclosed in whole in part without prior written permission
//      (C) COPYRIGHT 2003   SUNPLUS TECHNOLOGY CO
//      ALL RIGHTS RESERVED
//      The entire notice above must be reproduced on all authorized copies
//===========================================================================

//===========================================================================
//  Filename: OPHW.H
//  Author: Taylor Wang
//  Tel: +86-10-62981668-2814
//  Date:  2003,11,20
//  Description: Basic input/output of port
//               
//  Reference:
//
//  Version history:
//------------------------------------------------------------------------------
//  Version   YYYY-MM-DD-INDEX   Modified By         Description
//------------------------------------------------------------------------------
// none
//===========================================================================

.ifndef	__OPHW_inc__
.define	__OPHW_inc__

//================================================================
//Function: Read a value from port
//example:	__inp P_IOA_Data,r1
//================================================================
__inp:	.macro port,reg
		reg = [port]
.endm

//================================================================
//Function: Write a value to port
//example:	__outp P_IOA_Data,r1
//================================================================
__outp:	.macro	port,reg
		[port] = reg
.endm	

//================================================================
//Function: Clear port bit
//example:	__clrb P_IOA_Data, 0, r1		
//================================================================
__clrb:		.macro	port,bit,clobber
		clobber = ~(1 << bit)
		clobber &= [port]
		[port] = clobber
.endm

//================================================================
//Function: Set port bit
//example:	__setb P_IOA_Data, 0, r1
//================================================================
__setb: 	.macro 	port,bit,clobber
		clobber = (1 << bit)
		clobber |= [port]
		[port] = clobber
.endm

//================================================================
//Function: Inverse  port bit
//example:	__invb P_IOA_Data, 0, r1
//================================================================
__invb: 	.macro	port,bit,clobber
		clobber = (1 << bit)
		clobber ^= [port]
		[port] = clobber
.endm

//================================================================
//Function: Tst port bits
//example: __tstb P_IOA_Data,0,rq
//================================================================
__tstb:		.macro port,bit,clobber
		clobber = (1 << bit)
		test clobber,[port]
.endm

//================================================================
//Function: Clear port bits
//================================================================
__clrb_member:	 .macro	port,mask,reg
		reg = mask
		reg &= [port]
		[port] = reg
.endm

//================================================================
//Function: Set port bits
//================================================================
__setb_member:	.macro	 port,Value_reg,mask,clobber
		clobber = mask
		clobber &= [port]
		[port] = clobber | Value_reg
.endm
//===============================================================
//Function:get port bits
//===============================================================		
__getb_member:	.macro	port,mask,reg
		reg = mask
		reg &= [port]
.endm

.endif
