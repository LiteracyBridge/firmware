//------------------------------------------------------------------------------
.include .\system\include\system_head.inc

.public _RESET, __sn_loop, __sn_loop2, _RESETPCB
.external __sn_init_table, _main, __sn_sp_val

//------------------------------------------------------------------------------
unSP_StartUp: .section .text, .addr = 0xF800
StartUp: .proc
_RESETPCB:
_RESET:
	int off
	
//	r1 = 0x8000
//?delay:
//	r1 -= 1
//	jnz ?delay
		
	sp = __sn_sp_val

	//r15=[P_Clock_Ctrl]			//Check if warm reset,ÔÝÊ±ÆÁ±Î.
	r15=0

?WaitPll0:
	r1 = [P_Power_State]
	r1 &= 0x07
	jz ?WaitPll0

	r1 = [P_Clock_Ctrl]
	r1 &= 0x7fff
	[P_Clock_Ctrl] = r1

?WaitPll1:
	r1 = [P_Power_State]
	r1 &= 0x07
	jz ?WaitPll1

	r1=96/3					// PLL clock =48MHz
	[P_PLLChange]=r1

	r1=0x8418
	[P_Clock_Ctrl]=r1		// pll enable, system clock=PLL clock

?chk_pws:
	r1 = [P_Power_State]
	r1 &= 0x0007
	cmp r1, 0x0002
	jnz ?chk_pws
	
	
	r1 = 0x1f87				//0x0786
	[P_MCS0_Ctrl] = r1		//1M byte norflash

//	r1 = 0x0746				//0x0786
//	[P_MCS1_Ctrl] = r1
//	r1 = 0x00c6
//	[P_MCS2_Ctrl] = r1		//nand flash
	r1 = 0x0				//2k text section
	[P_MAPSEL] = r1
	
	r1 = 0x00c7
	[P_Mem_Ctrl] = r1			// CS0 CS1 CS2 ENABLE
	r1 = 0x007f//0x0003					
	[P_Addr_Ctrl] = r1			// disalbe MA23, MA22, MA21, MA20, MA19(1MB NOR)---ÔÝÊ±¶¼enable

	r1 = 0x0000
	[P_WatchDog_Ctrl] = r1		// disable watchdog
	r1 = 0x0000					
	[P_LVR_Ctrl] = r1			// LVR enable vol:2.47 -- 2.55V

	cmp	r15,0					//Check if warm reset
	jne	L_Wakeup
	r1=[P_INT_Status1]
	jnz	L_Wakeup
	r1=[P_INT_Status2]
	jnz	L_Wakeup
			
	r1 = 0						// clear internal ram
	r2 = 0
?clr_inram_loop:
	[r2++] = r1
	[r2++] = r1
	[r2++] = r1
	[r2++] = r1
	cmp r2, __sn_sp_val
	jb	?clr_inram_loop


	ds = seg __sn_init_table
	r1 = offset __sn_init_table
	r2 = ds:[r1++]		// item count
	jmp __judge_itcount

__next_item:
	push r2 ,r2 to [sp]
	r3 = d:[r1++]        // dest
	r4 = d:[r1++]        // ds
	r2 = d:[r1++]        // src
	r5 = D:[r1++]        // block size
	push sr, sr to [sp]
	ds = r4
	jmp __judge_bksize

__move_data:
	r4 = DS:[r2++]
	[r3++] = r4
	r5 = r5 - 1
__judge_bksize:
	cmp r5, 0
	jg __move_data

	pop sr, sr from [sp]
	pop r2,r2 from [sp]
	r2 = r2 - 1
__judge_itcount:
	cmp r2,0
	jg __next_item

__no_item:
	call _main
	
__sn_loop2:
	jmp __sn_loop2

__sn_loop:
	reti
	.endp

L_Wakeup:
	call _main
//------------------------------------------------------------------------------
.DEBUG
    .DW '.stabs "startup_code.",0x3c,0,0,0',0x0d,0x0a
.DEBUG
    .DW '.stabs "__sn_main:F15",36,0,0,', 0, 0
    .DW offset _main,seg _main
    .DW 0x0d,0x0a
.DEBUG
    .DW '.stabs "__sn_end:F15",36,0,0,', 0, 0
    .DW offset __sn_loop2,seg __sn_loop2
    .DW 0x0d,0x0a
.DEBUG
    .DW '.stabs "__sn_exception:F15",36,0,0,', 0, 0
    .DW offset __sn_loop,seg __sn_loop
    .DW 0x0d,0x0a
.END

//--------------------------------------end of startup_Data.asm----------------------------------------


