	.define VAR_N  0

m_cmp_r4r3_imm32: .macro imm_h, imm_l
	cmp r4, imm_h
	jne cmp_long_imm64_exit#
	cmp r3, imm_l
cmp_long_imm64_exit#:
	.endm

m_cmp_r2r1_imm32: .macro imm_h, imm_l
	cmp r2, imm_h
	jne cmp_long_imm64_exit#
	cmp r1, imm_l
cmp_long_imm64_exit#:
	.endm

m_cmp_r4r1_imm64: .macro imm_hh, imm_hl, imm_lh, imm_ll
	cmp r4, imm_hh
	jne cmp_long_imm64_exit#
	cmp r3, imm_hl
	jne cmp_long_imm64_exit#
	cmp r2, imm_lh
	jne cmp_long_imm64_exit#
	cmp r1, imm_ll
cmp_long_imm64_exit#:
	.endm

//========================================================================================
//
//  Filename:    eqd4.asm
//  Designer:    anping@sunnorth.com.cn
//  Date:        2002,12,6
//  Description: int _eqd4(double first, double second);
//  Return:      equal: r1=0,  not equal: r1=1
//  Reference:
//  Revision:
//  ---------------------------------------------------------------------------
//  YYYY-MM-DD    MODIFIED BY    DESCRIPTION
//  ---------------------------------------------------------------------------
//
//============================================================================

	.code
	.public ___eqsf2
	.public ___nesf2
___eqsf2:
___nesf2: .proc
	push bp to [sp]
	bp = sp + 1

	r4 = [bp+3+VAR_N+3]
	r4 &= 0x7fff
	r3 = [bp+3+VAR_N+2]
	r2 = [bp+3+VAR_N+1]
	r1 = [bp+3+VAR_N+0]
	m_cmp_r4r1_imm64 0x7ff0, 0, 0, 0
	ja __sn_eqd4_nan

	r4 = [bp+3+VAR_N+7]
	r4 &= 0x7fff
	r3 = [bp+3+VAR_N+6]
	r2 = [bp+3+VAR_N+5]
	r1 = [bp+3+VAR_N+4]
	m_cmp_r4r1_imm64 0x7ff0, 0, 0, 0
	ja __sn_eqd4_nan

	r4 = [bp+3+VAR_N+3]
	r3 = [bp+3+VAR_N+2]
	r2 = [bp+3+VAR_N+7]
	r1 = [bp+3+VAR_N+6]
	m_cmp_r4r3_imm32 r2, r1
	jne __sn_eqd4_not_equal
	r4 = [bp+3+VAR_N+1]
	r3 = [bp+3+VAR_N+0]
	r2 = [bp+3+VAR_N+5]
	r1 = [bp+3+VAR_N+4]
	m_cmp_r4r3_imm32 r2, r1
	jne __sn_eqd4_not_equal

__sn_eqd4_equal:
	r1 = 0
	jmp __sn_eqd4_exit

__sn_eqd4_nan:
__sn_eqd4_not_equal:
	r1 = 1

__sn_eqd4_exit:
	sp += VAR_N
	pop bp from [sp]
	retf
	.endp

//========================================================================================
//
//  Filename:   gtd4.asm
//  Designer:    anping@sunnorth.com.cn
//  Date:        2002,12,6
//  Description: int _gtd4(double first, double second);
//  Return:      great: r1=1,  equal: r1=0,  less: r1=-1
//  Reference:
//  Revision:
//  ---------------------------------------------------------------------------
//  YYYY-MM-DD    MODIFIED BY    DESCRIPTION
//  ---------------------------------------------------------------------------
//
//============================================================================

	.code
	.public ___gtsf2
	.public ___gesf2
___gtsf2:
___gesf2: .proc
	push bp to [sp]
	bp = sp + 1

	r4 = [bp+3+VAR_N+3]
	r4 &= 0x7fff
	r3 = [bp+3+VAR_N+2]
	r2 = [bp+3+VAR_N+1]
	r1 = [bp+3+VAR_N+0]
	m_cmp_r4r1_imm64 0x7ff0, 0, 0, 0
	ja __sn_gtd4_nan

	r4 = [bp+3+VAR_N+7]
	r4 &= 0x7fff
	r3 = [bp+3+VAR_N+6]
	r2 = [bp+3+VAR_N+5]
	r1 = [bp+3+VAR_N+4]
	m_cmp_r4r1_imm64 0x7ff0, 0, 0, 0
	ja __sn_gtd4_nan

__sn_gtd4_not_nan:
	r4 = [bp+3+VAR_N+3]
	r2 = [bp+3+VAR_N+7]
	cmp r4, r2
	jg __sn_gtd4_great
	jl __sn_gtd4_less
	r4 &= 0x8000
	jnz __sn_gtd4_all_negative
	r3 = [bp+3+VAR_N+2]
	r1 = [bp+3+VAR_N+6]
	cmp r3, r1
        ja __sn_gtd4_great
        jb __sn_gtd4_less

	r4 = [bp+3+VAR_N+1]
	r3 = [bp+3+VAR_N+0]
	r2 = [bp+3+VAR_N+5]
	r1 = [bp+3+VAR_N+4]
	m_cmp_r4r3_imm32 r2, r1
	ja __sn_gtd4_great
	je __sn_gtd4_equal

__sn_gtd4_nan:
__sn_gtd4_less:
	r1 = -1
	jmp __sn_gtd4_exit

__sn_gtd4_equal:
	r1 = 0
	jmp __sn_gtd4_exit

__sn_gtd4_all_negative:
	r3 = [bp+3+VAR_N+2]
	r1 = [bp+3+VAR_N+6]
	cmp r3, r1
        ja __sn_gtd4_less
        jb __sn_gtd4_great

	r4 = [bp+3+VAR_N+1]
	r3 = [bp+3+VAR_N+0]
	r2 = [bp+3+VAR_N+5]
	r1 = [bp+3+VAR_N+4]
	m_cmp_r4r3_imm32 r2, r1
	ja __sn_gtd4_less
	je __sn_gtd4_equal

__sn_gtd4_great:
	r1 = 1

__sn_gtd4_exit:
	sp+= VAR_N
	pop bp from [sp]
	retf
	.endp

//========================================================================================
//
//  Filename:   ltd4.asm
//  Designer:    anping@sunnorth.com.cn
//  Date:        2002,12,6
//  Description: int _ltd4(double first, double second);
//  Return:      great: r1=1,  equal: r1=0,  less: r1=-1
//  Reference:
//  Revision:
//  ---------------------------------------------------------------------------
//  YYYY-MM-DD    MODIFIED BY    DESCRIPTION
//  ---------------------------------------------------------------------------
//
//============================================================================

	.code
	.public ___ltsf2
	.public ___lesf2
___ltsf2:
___lesf2: .proc
	push bp to [sp]
	bp = sp + 1

	r4 = [bp+3+VAR_N+3]
	r4 &= 0x7fff
	r3 = [bp+3+VAR_N+2]
	r2 = [bp+3+VAR_N+1]
	r1 = [bp+3+VAR_N+0]
	m_cmp_r4r1_imm64 0x7ff0, 0, 0, 0
	ja __sn_ltd4_nan

	r4 = [bp+3+VAR_N+7]
	r4 &= 0x7fff
	r3 = [bp+3+VAR_N+6]
	r2 = [bp+3+VAR_N+5]
	r1 = [bp+3+VAR_N+4]
	m_cmp_r4r1_imm64 0x7ff0, 0, 0, 0
	ja __sn_ltd4_nan

__sn_ltd4_not_nan:
	r4 = [bp+3+VAR_N+3]
	r2 = [bp+3+VAR_N+7]
	cmp r4, r2
	jg __sn_ltd4_great
	jl __sn_ltd4_less
	r4 &= 0x8000
	jnz __sn_ltd4_all_negative
	r3 = [bp+3+VAR_N+2]
	r1 = [bp+3+VAR_N+6]
	cmp r3, r1
        ja __sn_ltd4_great
        jb __sn_ltd4_less

	r4 = [bp+3+VAR_N+1]
	r3 = [bp+3+VAR_N+0]
	r2 = [bp+3+VAR_N+5]
	r1 = [bp+3+VAR_N+4]
	m_cmp_r4r3_imm32 r2, r1
	jb __sn_ltd4_less
	je __sn_ltd4_equal

__sn_ltd4_nan:
__sn_ltd4_great:
	r1 = 1
	jmp __sn_ltd4_exit

__sn_ltd4_equal:
	r1 = 0
	jmp __sn_ltd4_exit

__sn_ltd4_all_negative:
	r3 = [bp+3+VAR_N+2]
	r1 = [bp+3+VAR_N+6]
	cmp r3, r1
        ja __sn_ltd4_less
        jb __sn_ltd4_great

	r4 = [bp+3+VAR_N+1]
	r3 = [bp+3+VAR_N+0]
	r2 = [bp+3+VAR_N+5]
	r1 = [bp+3+VAR_N+4]
	m_cmp_r4r3_imm32 r2, r1
	jb __sn_ltd4_great
	je __sn_ltd4_equal

__sn_ltd4_less:
	r1 = -1

__sn_ltd4_exit:
	sp += VAR_N
	pop bp from [sp]
	retf
	.endp
