.INCLUDE ./system/include/system_head.inc

.CODE

.PUBLIC _Sys_Initial
_Sys_Initial: .PROC
	r1=0x8418
	[P_Clock_Ctrl]=r1	//pll enable, system clock=PLL clock

	r1=0x20				//PLL clock =96MHz
	[P_PLLChange]=r1

pllwait?:
	r1=[P_Power_State]
	r1&=0x0f
	cmp	r1,0x02
	jne	pllwait?

	RETF
	.ENDP

.END