//========================================================================================
//Module Name:		isr.asm
//Applied body:		GPL2002
//Purpose:			ISR Service routine Area V1.0
//Description: 		
//Note:
//Programmer:			
//Written Date:			
//Version history:
//========================================================================================
.INCLUDE ./system/include/system_head.inc


.define C_USBDiskPlugIn   	0x01
.define C_USBDiskPlugOut 	0x02
.define C_USBHostError		0xffff
//*****************************************************************************
//*****************************************************************************
// Variable Publication and External function & variable declaration Area
//*****************************************************************************
//*****************************************************************************	
.external	ExecuteIF_AUDIOA
.external	ExecuteIF_AUDIOB
.external	ExecuteIF_ADC_AUTO
.external	_USBHostISR
.external _USBDiskPlugIn
.external _USBDiskPlugOut
.external _USBHost_Flag
.external _HandleHostISR;
//********************************************************
.PUBLIC	_BREAK
.PUBLIC	_FIQ
.PUBLIC	_IRQ0
.PUBLIC	_IRQ1
.PUBLIC	_IRQ2
.PUBLIC	_IRQ3
.PUBLIC	_IRQ4
.PUBLIC	_IRQ5
.PUBLIC	_IRQ6
.PUBLIC	_IRQ7

M_IntSaveContext:		.MACRO
	PUSH R1,R5 TO [SP]					
	PUSH R8,R15 TO [SP]					
.ENDM

M_IntRestoreContext:	.MACRO
    POP	R8,R15 FROM [SP]				
	POP R1,R5 FROM [SP]					
.ENDM

LB_OSIRQ: .SECTION .TEXT
//========================================================================================
// Interrupt Subroutine for BREAK (Software)
//========================================================================================
_BREAK:
	JMP$
	RETI

//========================================================================================
// Interrupt Subroutine for FIQ
//========================================================================================
_FIQ:
	JMP $
	RETI

//========================================================================================
// Interrupt Subroutine for IRQ0
//========================================================================================
_IRQ0:
	M_IntSaveContext
	
	R1 = [P_INT_Status1]
	TEST R1,INT_AUDIOA_MASK
	JNZ ?L_INT_AUDIOA
	
	TEST R1,INT_AUDIOB_MASK
	JNZ	?L_INT_AUDIOB

	R1 = [P_INT_Status2]
	TEST R1,INT_SHUTDOWNNB_MASK
	JNZ ?L_INT_SHUTDOWNNB

?L_INT_AUDIOA:
	CALL ExecuteIF_AUDIOA
	JMP ?L_Exit
	
?L_INT_AUDIOB:
    CALL ExecuteIF_AUDIOB
	JMP ?L_Exit
	
?L_INT_SHUTDOWNNB:
    CALL ExecuteIF_SHUTDOWNNB

?L_Exit:
	M_IntRestoreContext
	
	RETI

//========================================================================================
// Interrupt Subroutine for IRQ1
//========================================================================================
_IRQ1:
	M_IntSaveContext
	
	R1 = [P_INT_Status1]
	TEST R1,INT_ADC_AUTO_MASK
	JNZ ?L_INT_ADC_AUTO
	
	TEST R1,INT_TOUCHPANEL_MASK
	JNZ ?L_INT_TOUCHPANEL
	
	TEST R1,INT_ADC_READY_MASK
	JNZ ?L_INT_ADC_READY

?L_INT_ADC_AUTO:
	CALL ExecuteIF_ADC_AUTO
	JMP ?L_Exit
	
?L_INT_TOUCHPANEL:
    CALL ExecuteIF_TOUCHPANEL
	JMP ?L_Exit
	
?L_INT_ADC_READY:
    CALL ExecuteIF_ADC_READY

?L_Exit:
	M_IntRestoreContext
	RETI

//========================================================================================
// Interrupt Subroutine for IRQ2
//========================================================================================
_IRQ2:
	M_IntSaveContext
	
	R1 = [P_INT_Status1]
	TEST R1,INT_EXTA_MASK
	JNZ ?L_INT_EXTA

	TEST R1,INT_EXTB_MASK
	JNZ ?L_INT_EXTB

?L_INT_EXTA:
    CALL ExecuteIF_EXTA
	JMP ?L_Exit
	
?L_INT_EXTB:
    CALL ExecuteIF_EXTB
	JMP ?L_Exit

?L_Exit:
	M_IntRestoreContext
	RETI
	
//========================================================================================
// Interrupt Subroutine for IRQ3
//========================================================================================
_IRQ3:
	M_IntSaveContext
	
	R1 = [P_INT_Status1]
	TEST R1,INT_DMA_MASK
	JNZ ?L_INT_DMA

	TEST R1,INT_USB_MASK
	JNZ ?L_INT_USB

	TEST R1,INT_SPI_MASK
	JNZ ?L_INT_SPI

	TEST R1,INT_UART_IRDA_MASK
	JNZ ?L_INT_UART_IRDA

?L_INT_DMA:
    CALL ExecuteIF_DMA
	JMP ?L_Exit
	
?L_INT_USB:
    CALL ExecuteIF_USB
	JMP ?L_Exit
	
?L_INT_SPI:
    CALL ExecuteIF_SPI
	JMP ?L_Exit
	
?L_INT_UART_IRDA:
    CALL ExecuteIF_UART_IRDA

?L_Exit:
	M_IntRestoreContext
    RETI                                 // Return to interrupted task

//========================================================================================
// Interrupt Subroutine for IRQ4
//========================================================================================
_IRQ4:
	M_IntSaveContext
	
	R1 = [P_INT_Status2]
	TEST R1,INT_TIMERA_MASK
	JNZ ?L_INT_TIMERA

    TEST R1,INT_TIMERB_MASK
    JNZ ?L_INT_TIMERB

    TEST R1,INT_TIMERC_MASK
    JNZ  ?L_INT_TIMERC

    TEST R1,INT_TIMERD_MASK
    JNZ  ?L_INT_TIMERD

?L_INT_TIMERA:
    CALL ExecuteIF_TIMERA
	JMP ?L_Exit
	
?L_INT_TIMERB:
    CALL ExecuteIF_TIMERB
	JMP ?L_Exit
	
?L_INT_TIMERC:
    CALL ExecuteIF_TIMERC
	JMP ?L_Exit
	
?L_INT_TIMERD:
    CALL ExecuteIF_TIMERD

?L_Exit:
	M_IntRestoreContext
	RETI

//========================================================================================
// Interrupt Subroutine for IRQ5
//========================================================================================
_IRQ5:
	M_IntSaveContext

	R1 = [P_INT_Status1]
	TEST R1,INT_SPBUS_MASK
	JNZ ?L_INT_SPBUS

	TEST R1,INT_LCD_FP_MASK
	JNZ ?L_INT_LCD_FP

	TEST R1,INT_CMOS_HALFFULL_MASK
	JNZ ?L_INT_CMOS_HALFFULL

	TEST R1,INT_CMOS_FULL_MASK
	JNZ ?L_INT_CMOS_FULL

	TEST R1,INT_KEY_MASK
	JNZ ?L_INT_KEY

	R1 = [P_INT_Status2]
	TEST R1,INT_NANDFLASH_MASK
	JNZ ?L_INT_NANDFLASH

	TEST R1,INT_I2C_MASK
	JNZ ?L_INT_I2C

	TEST R1,INT_SD_MASK
	JNZ ?L_INT_SD

?L_INT_SPBUS:
    CALL ExecuteIF_SPBUS
   	JMP ?L_Exit
   	
?L_INT_LCD_FP:
    CALL ExecuteIF_LCD_FP
   	JMP ?L_Exit
   	
?L_INT_CMOS_HALFFULL:
    CALL ExecuteIF_CMOS_HALFFULL
   	JMP ?L_Exit
   	
?L_INT_CMOS_FULL:
    CALL ExecuteIF_CMOS_FULL
   	JMP ?L_Exit
   	
?L_INT_KEY:
    CALL ExecuteIF_KEY
   	JMP ?L_Exit
   	
?L_INT_NANDFLASH:
    CALL ExecuteIF_NANDFLASH
   	JMP ?L_Exit
   	
?L_INT_I2C:
    CALL ExecuteIF_I2C
   	JMP ?L_Exit
   	
?L_INT_SD:
	CALL ExecuteIF_SD

?L_Exit:
	M_IntRestoreContext
	RETI

//========================================================================================
// Interrupt Subroutine for IRQ6
//========================================================================================
_IRQ6:
	M_IntSaveContext
	
	R1 = [P_INT_Status2]
	TEST R1,INT_SCHED_INT_MASK
	JNZ ?L_INT_SCHED

	TEST R1,INT_KEYSCAN_MASK
	JNZ ?L_INT_KEYSCAN

	TEST R1,INT_TIMER_BASEC_MASK
	JNZ ?L_INT_TIMER_BASEC

?L_INT_SCHED:
    CALL ExecuteIF_SCHED_INT
   	JMP ?L_Exit
   	
?L_INT_KEYSCAN:
    CALL ExecuteIF_KEYSCAN
   	JMP ?L_Exit
   	
?L_INT_TIMER_BASEC:
    CALL ExecuteIF_TIMER_BASEC

?L_Exit:
	M_IntRestoreContext
	RETI                                

//========================================================================================
// Interrupt Subroutine for IRQ7
//========================================================================================
_IRQ7:
	M_IntSaveContext
	
	R1 = [P_INT_Status2]
	TEST R1,INT_HMS_MASK
	JNZ ?L_INT_HMS

	TEST R1,INT_ALARM_MASK
	JNZ ?L_INT_ALERM

	TEST R1,INT_TIMER_BASEA_MASK
	JNZ ?L_INT_TIMER_BASEA

	TEST R1,INT_TIMER_BASEB_MASK
	JNZ ?L_INT_TIMER_BASEB

?L_INT_HMS:
    CALL ExecuteIF_HMS
   	JMP ?L_Exit

?L_INT_ALERM:
    CALL ExecuteIF_ALARM
   	JMP ?L_Exit

?L_INT_TIMER_BASEA:
    CALL ExecuteIF_TIMER_BASEA
   	JMP ?L_Exit

?L_INT_TIMER_BASEB:
    CALL ExecuteIF_TIMER_BASEB

?L_Exit:
	M_IntRestoreContext
	RETI
	
ExecuteIF_SHUTDOWNNB:
	RETF


ExecuteIF_TOUCHPANEL:
	RETF

ExecuteIF_ADC_READY:
	RETF

ExecuteIF_EXTA:
	RETF

ExecuteIF_EXTB:
	RETF

ExecuteIF_DMA:
	RETF

.EXTERNAL _USB_ISR
ExecuteIF_USB:

		r1=[P_USBH_INTEN]
		cmp r1, 0
		jnz ?L_USBHost

		jmp ?USB_Dvice

	?L_USBHost:
		call _HandleHostISR		
		jmp End_USB?
		
?USB_Dvice:
		call _USB_ISR

End_USB?:
	RETF


ExecuteIF_SPI:
	RETF

ExecuteIF_UART_IRDA:
	RETF

ExecuteIF_TIMERA:
	RETF

ExecuteIF_TIMERB:
	RETF

ExecuteIF_TIMERC:
	RETF

ExecuteIF_TIMERD:
	RETF

ExecuteIF_SPBUS:
	RETF

ExecuteIF_LCD_FP:
	RETF

ExecuteIF_CMOS_HALFFULL:
	RETF

ExecuteIF_CMOS_FULL:
	RETF

ExecuteIF_KEY:
	RETF

ExecuteIF_NANDFLASH:
	RETF

ExecuteIF_I2C:
	RETF

ExecuteIF_SD:
	RETF

ExecuteIF_SCHED_INT:
	RETF

ExecuteIF_KEYSCAN:
	RETF

ExecuteIF_TIMER_BASEC:
	RETF

ExecuteIF_HMS:
	RETF

ExecuteIF_ALARM:
	RETF

.EXTERNAL _USB_Insert_TimeOut
ExecuteIF_TIMER_BASEA:
	CALL _USB_Insert_TimeOut
	RETF

ExecuteIF_TIMER_BASEB:
	RETF