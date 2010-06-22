.INCLUDE ./system/include/system_head.inc
.PUBLIC _SysIntoHaltMode

.EXTERNAL _SysOn2OffHook
.EXTERNAL _SysWaitLCDFP

.CODE

_SysIntoHaltMode: .PROC
	call _SysOn2OffHook

	SysSetState	SYS_OFF


?L_enter_halt_mode:
	call	_SysWaitLCDFP
	r1 = HALTMODE_CONST
	[P_HALT]=r1
	jmp ?L_enter_halt_mode
	nop
	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop
	nop
	
	jmp $
	
	retf
	.ENDP
	
.END


