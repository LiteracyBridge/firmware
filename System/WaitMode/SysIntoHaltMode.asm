.INCLUDE ./system/include/system_head.inc
.PUBLIC _SysIntoHaltMode

.CODE

.EXTERNAL _sav_P_MINT_Ctrl
.EXTERNAL _sav_P_Clock_Ctrl
	
_SysIntoHaltMode: .PROC

	SysSetState	 SYS_OFF
	r1 = [P_MINT_Ctrl];
	
	[_sav_P_MINT_Ctrl] = r1;
	
	r1 &= 0xFC00;
	r1 |= 0x5400;  // enable IOB[0..2] key change wakeup and clear all flags
	[P_MINT_Ctrl] = r1;
	r1 = 0x00FF;
delayLoop1?:
	r1-= 1;
	jnz  delayLoop1?;
		
	r1 = [P_IOB_Data];
    [P_IOB_Latch] = r1;

	r1 = [P_Clock_Ctrl];
	[_sav_P_Clock_Ctrl] = r1;
	r1 = 0x0600  // disables clocks and sets IOB0-2 interupts
	[P_Clock_Ctrl] = r1

 	r1 = HALTMODE_CONST; //500a
	[P_HALT] = r1;
  
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
	nop
	nop
	nop
	nop
	nop
	nop
	pop  BP from [SP];
	retf;
	.ENDP
	
.END
