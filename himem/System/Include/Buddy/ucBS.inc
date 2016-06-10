//========================================================================================
//Header Name:		ucBS.inc
//Applied body:		
//Purpose:		
//Programmer:		xiaoping
//Written Date:		2005/04/15
//Version history:
//			
//========================================================================================
.IFNDEF	_UCBS_INC_
.DEFINE	_UCBS_INC_


.EXTERNAL  _ucBSInit

.EXTERNAL _ucBSMalloc

.EXTERNAL  _ucBSFree


ucBSInit:	.macro
	call _ucBSInit
	.endm
	

ucBSMalloc:	.macro isize
        r1 = isize
        push r1 to [sp]
	call _ucBSMalloc
	sp += 1
	.endm	
	
ucBSFree:	.macro ptr
	r1 = ptr
        push r1 to [sp]
	call _ucBSFree
	sp += 1
	.endm		
	

.ENDIF