
/////////////////////////
//	nor flash type: SST 39VF3201 70ns
//////////////////////////

//L8100T	64K*7+48K+4K+4K+8K word
//L8100B	8k+4k+4k+48k+64k*7 word
//LV160T	32k*31+16k+4k+4k+8k word
//LV160B	8k+4k+4k+16k+32k*31 word
//LV320T	32k*63+4k*8 word
//LV320B	4k*8+32k*63 word
// .include ..\GPF32001A.inc
.INCLUDE .\System\include\system\GPL162002.inc

.define	Nor_StartAddressHigh	0x03
.define	Nor_StartAddressLow		0x00
.define L8100T	0x0085	//device ID
.define L8100B	0x0084	//device ID
.define LV160T	0x22c4	//device ID
.define LV160B	0x2249	//device ID
.define LV320T	0x22a7	//device ID
.define LV320B	0x22a8	//device ID
.define	MX_BOTTOM	0x2200	//6th ID value
//.define NORCODEINSRAM		0x1000
.public _NorFlashDriver_Start
.public _NorFlashDriver_End

	.ram
	.var	R_NorTemp
	.var	R_toggle
NORFLASHDRIVER_CODE:	.section .iram //,.addr=0x30000
_NorFlashDriver_Start:
.public	_Nor_FlashID
_Nor_FlashID:
	ds = Nor_StartAddressHigh				//reset norflash
	r1 = 0xf0
	r2 = 0
	d:[r2] = r1
	
	ds = Nor_StartAddressHigh
	r1 = 0x5555
	r2 = 0xAA
	D:[r1] = r2
	r1 = 0x2AAA
	r2 = 0x55
	D:[r1] = r2
	r1 = 0x5555
	r2 = 0x90
	D:[r1] = r2
	r1 = 0xA
?L_loop:
	r1- = 1
	cmp r1,0
	jne	?L_loop
	r1 = 0
	r3 = D:[r1]
	r1 = 0xf0		//reset nor flas
	r2 = 0
	d:[r2] = r1
	r1 = r3;
	retf
//
.public _Nor_BlockErase
_Nor_BlockErase:
	push bp to [sp]
//	fiq off
//	irq off
	r1=0
	[R_toggle]=r1
	
	bp=sp+1
	r1=[bp+3]		//address low
	r2=[bp+4]		//address high
	r3=[bp+5]		//count
	call	F_Nor_BlockErase
	pop	bp from [sp]
//	fiq on
//	irq on
	retf
//////////////////////////////////////////////
.public _Nor_WordWrite
_Nor_WordWrite:
	push bp to [sp]
//	fiq off
//	int off
	bp=sp+1
	r1=[bp+3]		//destination address low
	r2=[bp+4]		//destination address high
	//r2+ = Nor_StartAddressHigh
	r3=[bp+5]		//count
	r4=[bp+6]		//source address
	call	F_Nor_WordWrite
	pop	bp from [sp]
//	fiq on
//	irq on
	retf
.comment @
//////////////////////////////////////////////

////////////////////////////////////////////
.public _Nor_WordRead
_Nor_WordRead:
	push bp to [sp]
	int off
	bp=sp+1
	r1=[bp+3]		//source address low
	r2=[bp+4]		//source address high
	r3=[bp+5]		//count
	r4=[bp+6]		//destination address

Nor_WordReadLoop?:	
	ds = r2
	r5 = ds:[r1++]
	r2 = ds			//keep 
	[r4++] = r5
	r3 -= 1
	jnz Nor_WordReadLoop?
	
	pop	bp from [sp]
	int  FIQ,IRQ
	retf
@

//=========================================================================
//void DrvNorFlashRead(unsigned long bufferAddress,unsigned long norflashAddr,unsigned int wordSize);
.public _DrvNorFlashRead
.public F_DrvNorFlashRead
_DrvNorFlashRead: .proc
F_DrvNorFlashRead:
	push bp to [sp]
	r1 = [P_IOB_Buffer]
	r1 |= 0x2000
	[P_IOB_Buffer] = r1;
?F_NORDrv_NoLED:
	bp = sp + 1	
	r1 = [bp + 3]		//low 16bit address of buffer
	r2 = [bp + 4]		//high 16bit address of buffer
	r3 = [bp + 5]		//low 16bit address of norflash(from 0x0000)
	r4 = [bp + 6]		//high 16bit address of norflash
	//r4 += Nor_StartAddressHigh
		
	r8 = 0
?L_ReadWordLoop:
	ds = r4
	r9 = bp
	bp = d:[r3++]			//read a word from norflash
	r4 = ds
	ds = r2
	d:[r1++] = bp			//write such word to buffer
	r2 = ds
	bp = r9
	r8 = r8+1
	cmp r8,[bp + 7]
	jne	?L_ReadWordLoop

	r1 = [P_IOB_Buffer]
	r1 &= ~0x2000
	[P_IOB_Buffer] = r1;

	pop bp from [sp]
	retf
	.endp

////////////////////////////////////////////

//////////////////////////////////////////////
.public F_Nor_BlockErase
F_Nor_BlockErase:		//input parameter r2:r1 start erase address, r3: rease block number(32KW/block) 	
	push	r1,r5 to [sp]
//check norflash manufacture ID
	ds = Nor_StartAddressHigh				//reset norflash
	r1 = 0xf0
	r2 = 0
	d:[r2] = r1
	
	ds = 0x3
	r1 = 0x5555
	r2 = 0xAA
	D:[r1] = r2
	r1 = 0x2AAA
	r2 = 0x55
	D:[r1] = r2
	r1 = 0x5555
	r2 = 0x90
	D:[r1] = r2
	r1 = 0xA
?L_loop:
	r1- = 1
	cmp r1,0
	jne	?L_loop
	r1 = 0
	r2 = D:[r1]
	cmp r2,0xBF
	jne	?L_MX
	goto ?L_SST
?L_MX:
	r1 = Nor_StartAddressHigh				//reset norflash
	ds = r1
	r1 = 0xf0
	r2 = 0
	d:[r2] = r1
	pop r1,r5 from [sp]
	push r1,r5 to [sp]
	call F_CheckID
	cmp r4,0x227e   // huanghe for SPL71 20081101
	jne ?L_MX1
	goto ?L_SPL71
?L_MX1:	
	call F_CheckID
	cmp r4,L8100T
	jne	?L_checkL8100B
	r2 = r1 + offset T_L8100T_SEG
	r1 = r1 + offset T_L8100T_OFF
	jmp	BlockErase_Loop?
?L_checkL8100B:
	cmp r4,L8100B
	jne	?L_checkLV160T
	r2 = r1 + offset T_L8100B_SEG
	r1 = r1 + offset T_L8100B_OFF
	jmp	BlockErase_Loop?
?L_checkLV160T:
	cmp r4,LV160T
	jne	?L_LV160B
	r2 = r1 + offset T_LV160T_SEG
	r1 = r1 + offset T_LV160T_OFF
	jmp	BlockErase_Loop?
?L_LV160B:
	cmp r4,LV160B
	jne	?L_LV320T
	r2 = r1 + offset T_LV160B_SEG
	r1 = r1 + offset T_LV160B_OFF
	jmp	BlockErase_Loop?		
?L_LV320T:
	cmp r4,LV320T
	jne	?L_LV320B
	r2 = r1 + offset T_LV320T_SEG
	r1 = r1 + offset T_LV320T_OFF
	jmp	BlockErase_Loop?	
?L_LV320B:
	cmp r4,LV320B
	je	?L_320B
	goto	?L_End
?L_320B:
	r2 = r1 + offset T_LV320B_SEG
	r1 = r1 + offset T_LV320B_OFF
BlockErase_Loop?:
	r2 = [r2]
	r1 = [r1]
	r4=Nor_StartAddressHigh	//first command
	ds=r4
	r4=0x555
	r5=0x0aa
	ds:[r4]=r5		//first command
	
	r4=0x2aa		//second command
	r5=0x055
	ds:[r4]=r5		//second command
	
	r4=0x555		//third command
	r5=0x080
	ds:[r4]=r5		//third command
	
	r4=0x555		//fourth command
	r5=0x0aa
	ds:[r4]=r5		//fourth command
	
	r4=0x2aa		//fifth command
	r5=0x055
	ds:[r4]=r5		//fifth command
	
	ds=r2			//sixth command
	r5=0x030
	ds:[r1]=r5		//sixth command
	nop
	call F_Toggle
	goto ?L_End
?L_SST:
	r3=Nor_StartAddressHigh	
	r1 = r3				//reset norflash
	ds = r1
	r1 = 0xf0
	r2 = 0
	d:[r2] = r1
	pop r1,r5 from [sp]
	push r1,r5 to [sp]
	r2 = 0x8000
	mr = r1*r2,uu
	r1 = r3
	r2 = r4
	r2+ = Nor_StartAddressHigh

	r4=Nor_StartAddressHigh	//first command
	ds=r4
	r4=0x5555
	r5=0x0aa
	ds:[r4]=r5		//first command
	
	r4=0x2aaa		//second command
	r5=0x055
	ds:[r4]=r5		//second command
	
	r4=0x5555		//third command
	r5=0x080
	ds:[r4]=r5		//third command
	
	r4=0x5555		//fourth command
	r5=0x0aa
	ds:[r4]=r5		//fourth command
	
	r4=0x2aaa		//fifth command
	r5=0x055
	ds:[r4]=r5		//fifth command
	
	ds=r2			//sixth command
	r5=0x050
	ds:[r1]=r5		//sixth command
	nop
	call F_Toggle
	goto ?L_End
?L_SPL71:					// huanghe for SPL71 20081101
// read 5th & 6th word to determine the TOP / BOTTOM
	r4 = Nor_StartAddressHigh
	ds = r4
	r5 = 0x0E
	r4 = d:[r5++]
	r5 = d:[r5]
//
	cmp r5,MX_BOTTOM			//00 = BOTTOM; 01 = TOP
	jne	?L_MX_TOP
	r2 = r1 + offset T_LV320B_SEG
	r4 = r1 + offset T_LV320B_OFF
//	goto	BlockErase_Loop?	
	jmp		?L_ResetFlash
?L_MX_TOP:
	r2 = r1 + offset T_LV320T_SEG
	r4 = r1 + offset T_LV320T_OFF
?L_ResetFlash:
	r1=Nor_StartAddressHigh	
	ds = r1
	r1 = 0xf0
	r5 = 0
	d:[r5] = r1
	r1 = r4
	goto	BlockErase_Loop?	
.comment @
	r1 = Nor_StartAddressHigh				//reset norflash
	ds = r1
	r1 = 0xf0
	r2 = 0
	d:[r2] = r1
	pop r1,r5 from [sp]
	push r1,r5 to [sp]
	
	cmp r1 ,4
	jb ?L_Sector_Below4  //below sector 4 or not refer to spec
	
	r1 = r1 -4
	r2 = 0x02
	mr = r1*r2, uu
	r2 = r3
	r2 = r2 + 0x02
	jmp ?L_Erase_Command
		
?L_Sector_Below4:	
	r2 = 0x8000
	mr = r1*r2, uu
	r1 = r3
	r2 = r4
	
?L_Erase_Command:
	
	r2+ = Nor_StartAddressHigh

	r4=Nor_StartAddressHigh	//first command
	ds=r4
	r4=0x555
	r5=0x0aa
	ds:[r4]=r5		//first command
	
	r4=0x2aa		//second command
	r5=0x055
	ds:[r4]=r5		//second command
	
	r4=0x555		//third command
	r5=0x080
	ds:[r4]=r5		//third command
	
	r4=0x555		//fourth command
	r5=0x0aa
	ds:[r4]=r5		//fourth command
	
	r4=0x2aa		//fifth command
	r5=0x055
	ds:[r4]=r5		//fifth command
	
	ds=r2			//sixth command
	r5=0x030
	ds:[r1]=r5		//sixth command
	nop
	call F_Toggle
.@
?L_End:
	pop	r1,r5 from [sp]
	retf

.public F_CheckID
F_CheckID:
	r4=Nor_StartAddressHigh	//first command
	ds=r4
	r4=0x555
	r5=0x0aa
	ds:[r4]=r5		//first command
	
	r4=0x2aa		//second command
	r5=0x055
	ds:[r4]=r5		//second command
	
	r4=0x555		//third command
	r5=0x090
	ds:[r4]=r5		//third command
	
	r5=0x0001		//fourth command
	r4 = ds:[r5]	//fourth command
	retf

////////////////////////////////////////////

//////////////////////////////////////////////
	
////////////////////////////////////////////////////	
.public F_Nor_WordWrite
F_Nor_WordWrite:					
					//input parameter r2:r1 destination address, r3: count(64Kw max)
	push	r1,r5 to [sp]		//				  r4: source address 
//	r1 = [P_IOB_Buffer]
//	r1 |= 0x2000
//	[P_IOB_Buffer] = r1;
?F_NORDrv_NoLED:
//check norflash manufacture ID
	ds = Nor_StartAddressHigh				//reset norflash
	r1 = 0xf0
	r2 = 0
	d:[r2] = r1
	
	ds = Nor_StartAddressHigh
	r1 = 0x5555
	r2 = 0xAA
	D:[r1] = r2
	r1 = 0x2AAA
	r2 = 0x55
	D:[r1] = r2
	r1 = 0x5555
	r2 = 0x90
	D:[r1] = r2
	r1 = 0xA
?L_loop:
	r1- = 1
	cmp r1,0
	jne	?L_loop
	r1 = 0
	r2 = D:[r1]
	cmp r2,0xBF
	jne	?L_MX
	goto ?L_SST
?L_MX:
	r1 = Nor_StartAddressHigh				//reset norflash
	ds = r1
	r1 = 0xf0
	r2 = 0
	d:[r2] = r1
	pop r1,r5 from [sp]
	push r1,r5 to [sp]			 
WordWrite_Loop?:	
	[R_NorTemp]=r3
	ds=Nor_StartAddressHigh	
	r3=0x555					//first command
	r5=0x0aa
	ds:[r3]=r5					//first command
	
	r3=0x2aa					//second command
	r5=0x55
	ds:[r3]=r5					//second command
	
	r3=0x555					//third command
	r5=0x0a0
	ds:[r3]=r5					//third command

	ds=r2						//fourth command
	r5=[r4++]
	ds:[r1]=r5				//fourth command	
	nop
	call	F_Toggle
	call F_Nor_WordWrite_Check
	r2=ds
	r1 += 1
	r3=[R_NorTemp]
	r3-=1
	jnz	WordWrite_Loop?
	goto ?L_End
?L_SST:
	r1 = 0x3				//reset norflash
	ds = r1
	r1 = 0xf0
	r2 = 0
	d:[r2] = r1
	pop r1,r5 from [sp]
	push r1,r5 to [sp]	
WordWriteSST_Loop?:	
	[R_NorTemp]=r3
	ds=Nor_StartAddressHigh	
	r3=0x5555					//first command
	r5=0x0aa
	ds:[r3]=r5					//first command
	
	r3=0x2aaa					//second command
	r5=0x55
	ds:[r3]=r5					//second command
	
	r3=0x5555					//third command
	r5=0x0a0
	ds:[r3]=r5					//third command

	ds=r2						//fourth command
	r5=[r4++]
	ds:[r1]=r5				//fourth command	
	nop
	call	F_Toggle	
	call F_Nor_WordWrite_Check
	r2=ds
	r1 += 1
	r3=[R_NorTemp]
	r3-=1
	jnz	WordWriteSST_Loop?
?L_End:
	r1 = [P_IOB_Buffer]
	r1 &= ~0x2000
	[P_IOB_Buffer] = r1;
	pop	r1,r5 from [sp]
	retf
	
	
/////////////////////////////////////////////////////////////////	
F_Toggle:
	push r1,r2 to [sp]
	ds=Nor_StartAddressHigh
	r1=0

	r2=[R_toggle]
	r2+=1
	[R_toggle]=r2
	
	r2=ds:[r1]
	nop
	nop
	nop
	nop
	test	r2,0x0040	//D6 is toggle?
	jz	zero?	
one?:	
	
	r2=[R_toggle]
	r2+=1
	[R_toggle]=r2

	r2=ds:[r1]
	nop
	nop
	nop
	nop
	test	r2,0x0040
	jnz	toggle_end?
zero?:
	r2=[R_toggle]
	r2+=1
	[R_toggle]=r2
	r2=ds:[r1]	
	nop
	nop
	nop
	nop
	test	r2,0x0040
	jnz	one?
toggle_end?:
	pop	r1,r2 from [sp]
	//int  FIQ,IRQ
	retf	
////////////////////////////////////	
F_Nor_WordWrite_Check:
	push r1,r3 to [sp]
	r3 = 0xff;
CheckContinue?:
	r2 = ds:[r1]
	r3 = r3-1
	jz TimeOut?
	cmp r2, r5
	jne CheckContinue?
TimeOut?:
	pop	r1,r3 from [sp]
	retf
_NorFlashDriver_End:	

T_L8100T_SEG:		//64K*7+48K+4K+4K+8K word
	.dw	3,4,5,6,7,8,9,0xA,0xA,0xA,0xA

T_L8100T_OFF:
	.dw 0,0,0,0,0,0,0,0,0xc000,0xd000,0xe000
T_L8100B_SEG:		//8k+4k+4k+48k+64k*7 word
	.dw 3,3,3,3,4,5,6,7,8,9,0xA
T_L8100B_OFF:
	.dw 0,0x2000,0x3000,0x4000,0,0,0,0,0,0,0
T_LV160T_SEG:		//32k*31+16k+4k+4k+8k word
	.dw	3,3,4,4,5,5,6,6,7,7,8,8,9,9,0xa,0xa,0xb,0xb,0xc,0xc,0xd,0xd,0xe,0xe,0xf,0xf,0x10,0x10,0x11,0x11,0x12,0x12,0x12,0x12,0x12
T_LV160T_OFF:
	.dw 0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000
	.dw 0,0x8000,0,0x8000,0,0x8000,0xc000,0xd000,0xe000
T_LV160B_SEG:		//8k+4k+4k+16k+32k*31 word
	.dw 3,3,3,3,3,4,4,5,5,6,6,7,7,8,8,9,9,0xa,0xa,0xb,0xb,0xc,0xc,0xd,0xd,0xe,0xe,0xf,0xf,0x10,0x10,0x11,0x11,0x12,0x12
T_LV160B_OFF:
	.dw	0,0x2000,0x3000,0x4000,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000
	.dw	0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000
T_LV320T_SEG:		//32k*63+4k*8 word
	.dw 3,3,4,4,5,5,6,6,7,7,8,8,9,9,0xa,0xa,0xb,0xb,0xc,0xc,0xd,0xd,0xe,0xe,0xf,0xf,0x10,0x10,0x11,0x11,0x12,0x12
	.dw	0x13,0x13,0x14,0x14,0x15,0x15,0x16,0x16,0x17,0x17,0x18,0x18,0x19,0x19,0x1a,0x1a,0x1b,0x1b,0x1c,0x1c,0x1d,0x1d
	.dw 0x1e,0x1e,0x1f,0x1f,0x20,0x20,0x21,0x21,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22
T_LV320T_OFF:
	.dw 0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000
	.dw	0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000
	.dw	0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0x9000,0xa000,0xb000,0xc000,0xd000
	.dw	0xe000,0xf000
T_LV320B_SEG:		//4k*8+32k*63 word
	.dw 3,3,3,3,3,3,3,3,3,4,4,5,5,6,6,7,7,8,8,9,9,0xa,0xa,0xb,0xb,0xc,0xc,0xd,0xd,0xe,0xe,0xf,0xf,0x10,0x10,0x11,0x11
	.dw	0x12,0x12,0x13,0x13,0x14,0x14,0x15,0x15,0x16,0x16,0x17,0x17,0x18,0x18,0x19,0x19,0x1a,0x1a,0x1b,0x1b,0x1c,0x1c
	.dw	0x1d,0x1d,0x1e,0x1e,0x1f,0x1f,0x20,0x20,0x21,0x21,0x22,0x22
T_LV320B_OFF:
	.dw	0,0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000
	.dw	0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000
	.dw	0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000,0,0x8000
	.dw	0,0x8000,0,0x8000,0,0x8000,0,0x8000
	
	
	
	
	