.INCLUDE ./system/include/system_head.inc


.code
//*************************************************************    
//GPL162002 current test
//*************************************************************  
.PUBLIC	__SystemOnOff
__SystemOnOff:	.PROC
	push BP to [SP];
	//CALL __TurnOffLCD;
	// r1 = 0x5fff;		//Turn off SPY0030(IOD13)
 	// [P_IOD_Data] = r1;
	
//.comment @
	r1 = 0x8000;
	[P_TimerA_Ctrl] = r1;
	[P_TimerB_Ctrl] = r1;
	[P_TimerC_Ctrl] = r1;
	[P_TimerD_Ctrl] = r1;
	[P_TimerE_Ctrl] = r1;
	[P_TimerF_Ctrl] = r1;

	[P_TimeBaseA_Ctrl]=r1;
	[P_TimeBaseB_Ctrl]=r1;
	[P_TimeBaseC_Ctrl]=r1;

	r1 = 0x0000;
	[P_CHA_Data] = r1;
	[P_CHB_Data] = r1;
	r1 = 0x0000;
	[P_CHA_Ctrl] = r1;
	[P_CHB_Ctrl] = r1;
	r1 = 0x0100;
	[P_CHA_FIFO] = r1;
	[P_CHB_FIFO] = r1;

        r1 = [P_DAC_Ctrl]
        r1 |= 0x000c
        r1 &= ~0x0001
        [P_DAC_Ctrl] = r1
        r1 = 0x001f
        [P_HPAMP_Ctrl] = r1
	r1 = 0x0000;
	[P_DAC_IIS_Ctrl]  = r1;				// disalbe DAC IIS
	
	r1 = 0x8000;
	[P_NF_INT_Ctrl] = r1;
	r1 = 0x8000;		//0x8010;
	[P_LCD_Setup] = r1;
	r1 = 0x8000;
	[P_KS_Ctrl] = r1;

	r1 = 0x0000;
	[P_RTC_Ctrl] = r1;
	r1 = 0x0000;
	[P_TFT_Ctrl] = r1;
	r1 = 0x0000;
	[P_UART_Ctrl] = r1;
	r1 = 0x0000;
	[P_IrDA_Ctrl] = r1;
	r1 = 0x0000;						//SPI control Register
	[P_SPI_Ctrl] = r1;
	r1 = 0x0000;						//disable I2C
	[P_I2C_En] = r1
	R1 = 0x0000;						// disable SDC module
	[P_SD_Ctrl] = R1;

	r1 = 0x0000;
	[P_ADC_Setup] = r1;
	r1 =0x8007;		//0x8037;
	[P_MADC_Ctrl] = r1;
	R1 = 0x8000;
	[P_ASADC_Ctrl]= r1;
	r1 = 0x8000;
	[P_TP_Ctrl] = r1;					// disable Touch Panel module
        r1 = [P_HQADC_Ctrl]
        r1 &= ~0x0800
        [P_HQADC_Ctrl] = r1

	r1 = 0x0200
	[P_DMA_Ctrl0] = r1;					// reset DMA channel 0
	[P_DMA_Ctrl1] = r1;					// reset DMA channel 1
	[P_DMA_Ctrl2] = r1;					// reset DMA channel 2
	[P_DMA_Ctrl3] = r1;					// reset DMA channel 3
//@
	r1 = [P_MINT_Ctrl]
	r1&= 0xA800
	[P_MINT_Ctrl] = r1
//ReleaseKeyOnOff1?:
//	r1 = [P_IOB_Buffer];
//	r1&= 0x0FFC;
//	r1|= 0xF000;
//	[P_IOB_Data] = r1;
//	nop;
//	nop;
//	nop;
//	nop;
//	nop;
//	nop;
//	nop;
//	nop;
//	nop;
//	nop;
//	nop;
//	nop;
//	r1 = [P_IOB_Data];
//	test r1,0x0003;
//	jnz  ReleaseKeyOnOff1?;

	r1 = 0x00FF;
delayLoop1?:
	r1-= 1;
	jnz  delayLoop1?;

	r1 = 0x000F;
	[P_CLK_Ctrl0] = r1;
	r1 = 0xC000;		//0xC480;
	[P_CLK_Ctrl1] = r1;

	r1 = [P_MINT_Ctrl];
	r1|= 0x0400;
	[P_MINT_Ctrl] = r1;
//	r1 = [P_IOB_Buffer];
//	r1&= 0x0FFC;
//	r1|= 0x1002;
//	[P_IOB_Data] = r1;
//	r1 = [P_IOB_Latch];
	nop
	nop
	nop
pllwait1?:
	r1 = [P_Power_State]
	r1&= 0x0007
	jz	pllwait1?

	r1 = 0x0600
	[P_Clock_Ctrl] = r1
	r1 = 0x0000
	[P_PLLChange]  = r1
pllwait2?:
	r1 = [P_Power_State]
	r1&= 0x0007
	cmp r1,0x01	//Slow State=12MHz
	jne	pllwait2?

	//[_R_SP_BackUp]= SP;
	//[_R_BP_BackUp]= BP;
	//[_R_SR_BackUp]= SR;
	//r1 = FR;
	//[_R_FR_BackUp]= r1;
	nop
	nop
	nop
	nop
	nop
	nop
//	r1 = 0x500A;
	r1 = 0xA00A;
//	[P_HALT] = r1;
	[P_SLEEP] = r1;
	nop
	nop
	nop
	nop
	nop
	nop
GPL_WakeUp_FromHalt:
	nop
	nop
	nop
	nop
	nop
	nop
	r1 = 0x00FF;
delayLoop2?:
	r1-= 1;
	jnz  delayLoop2?;

pllwait3?:
	r1 = [P_Power_State]
	r1&= 0x0007
	jz	pllwait3?

	r1 = 0x0600
	[P_Clock_Ctrl] = r1
	r1 = 0x0010
	[P_PLLChange]  = r1
	r1 = 0x8600
	[P_Clock_Ctrl] = r1
pllwait4?:
	r1 = [P_Power_State]
	r1&= 0x0007
	cmp r1,0x02	//Fast State=48MHz
	jne	pllwait4?

ReleaseKeyOnOff2?:
	r1 = [P_IOB_Buffer];
	r1&= 0x0FFC;
	r1|= 0xF000;
	[P_IOB_Data] = r1;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
	r1 = [P_IOB_Data];
	test r1,0x0003;
	jnz  ReleaseKeyOnOff2?;

	r1 = [P_MINT_Ctrl]
	r1&= 0xA8FF
	[P_MINT_Ctrl] = r1
	r1 = 0x6D7F;
	[P_CLK_Ctrl0] = r1;
	r1 = 0xEEF1;
	[P_CLK_Ctrl1] = r1;
	//CALL __TurnOnLCD;
	pop  BP from [SP];
	retf;
	.ENDP
 
