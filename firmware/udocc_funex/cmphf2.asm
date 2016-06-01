m_cmp_r4r3_imm32: .macro imm_h, imm_l
	cmp r4, imm_h
	jne cmp_long_imm32_exit#
	cmp r3, imm_l
cmp_long_imm32_exit#:
	.endm

m_cmp_r2r1_imm32: .macro imm_h, imm_l
	cmp r2, imm_h
	jne cmp_long_imm32_exit#
	cmp r1, imm_l
cmp_long_imm32_exit#:
	.endm

//========================================================================================
//
//  Designer:    anping@sunnorth.com.cn
//  Date:        2002/12/06
//  Description: int __eqhf2(float first, float second);
//               int __nehf2(float first, float second);
//  Return:      equal: r1=0,  not equal: r1=1
//  Revision:
//    2005/05/19
//    tim_ouyang@sunplus.com.tw
//    Modify to fit GCC behavior           
//
//============================================================================

	.code
	.public ___eqhf2
	.public ___nehf2
___nehf2:
___eqhf2: .proc
	push bp to [sp]
	bp = sp + 1

	r4 = [bp+4]
	r4 &= 0x7fff
	r3 = [bp+3]
	m_cmp_r4r3_imm32 0x7f80, 0
	ja __sn_eqf2_nan

	r2 = [bp+6]
	r2 &= 0x7fff
	r1 = [bp+5]
	m_cmp_r2r1_imm32 0x7f80, 0
	ja __sn_eqf2_nan

__sn_eqf2_not_nan:
	r4 = [bp+4]
	r2 = [bp+6]
	cmp r4, r2
	jne __sn_not_equal
	cmp r3, r1
	jne __sn_not_equal
	r1 = 0
        jmp __sn_eqf2_exit

__sn_eqf2_nan:
__sn_not_equal:
	r1 = 1

__sn_eqf2_exit:
	pop bp from [sp]
	retf
	.endp

//========================================================================================
//
//  Designer:    anping@sunnorth.com.cn
//  Date:        2002/12/06
//  Description: int __gthf2(float first, float second);
//               int __gehf2(float first, float second);
//  Return:      great: r1=1,  equal: r1=0,  less: r1=-1
//  Revision:
//    2005/05/19
//    tim_ouyang@sunplus.com.tw
//    Modify to fit GCC behavior           
//
//============================================================================
	.code
	.public ___gthf2
	.public ___gehf2
___gehf2:
___gthf2: .proc
	push bp to [sp]
	bp = sp + 1

	r4 = [bp+4]
	r4 &= 0x7fff
	r3 = [bp+3]
	m_cmp_r4r3_imm32 0x7f80, 0
	ja __sn_gtf2_nan

	r2 = [bp+6]
	r2 &= 0x7fff
	r1 = [bp+5]
	m_cmp_r2r1_imm32 0x7f80, 0
	ja __sn_gtf2_nan

__sn_gtf2_not_nan:
	r4 = [bp+4]
	r2 = [bp+6]
	cmp r4, r2
	jg __sn_gtf2_great
	jl __sn_gft2_less
	// high 16bit equal
	r4 &= 0x8000
	jnz __sn_gtf2_all_negative
	cmp r3, r1
	ja __sn_gtf2_great
	je __sn_gtf2_equal

__sn_gtf2_nan:
__sn_gft2_less:
	r1 = -1
	jmp __sn_gtf2_exit

__sn_gtf2_all_negative:
	cmp r3, r1
	ja __sn_gft2_less

__sn_gtf2_equal:
	r1 = 0
	jmp __sn_gtf2_exit

__sn_gtf2_great:
	r1 = 1

__sn_gtf2_exit:
	pop bp from [sp]
	retf
	.endp


//========================================================================================
//
//  Filename:   ltf2.asm
//  Designer:    anping@sunnorth.com.cn
//  Date:        2002,12,6
//  Description: int __lthf2(float first, float second);
//               int __lehf2(float first, float second);
//  Return:      great: r1=1,  equal: r1=0,  less: r1=-1
//  Reference:
//  Revision:
//    2005/05/19
//    tim_ouyang@sunplus.com.tw
//    Modify to fit GCC behavior           
//
//============================================================================

	.code
	.public ___lthf2
	.public ___lehf2
___lthf2:
___lehf2: .proc
	push bp to [sp]
	bp = sp + 1

	r4 = [bp+4]
	r4 &= 0x7fff
	r3 = [bp+3]
	m_cmp_r4r3_imm32 0x7f80, 0
	ja __sn_ltf2_nan

	r2 = [bp+6]
	r2 &= 0x7fff
	r1 = [bp+5]
	m_cmp_r2r1_imm32 0x7f80, 0
	ja __sn_ltf2_nan

__sn_lf2_not_nan:
	r4 = [bp+4]
	r2 = [bp+6]
	cmp r4, r2
	jg __sn_ltf2_great
	jl __sn_ltf2_less
	// high 16bit equal
	r4 &= 0x8000                         
	jnz __sn_ltf2_all_negative
	cmp r3, r1
	jb __sn_ltf2_less

__sn_ltf2_equal:
	r1 = 0
	jmp __sn_ltf2_exit

__sn_ltf2_nan:
__sn_ltf2_great:
	r1 = 1
	jmp __sn_ltf2_exit

__sn_ltf2_all_negative:
	cmp r3, r1
	jb __sn_ltf2_great
	je __sn_ltf2_equal

__sn_ltf2_less:
	r1 = -1

__sn_ltf2_exit:
	pop bp from [sp]
	retf
	.endp
