//========================================================================================
// Program: Standard function definition V1.0 for SPL Series
// Arranged by: Wilson
// Platform:  unSP SPL Series
//
// Date:        2005/Aug  V1.0 ( first version )
//
// Note: 1)This inc file defines the control register available for user to use SPL series.
//         The control regsiter setting constants is also included for users' convenience.
//       2)Naming rule:
//         C : define constant
//         P : define I/O port
//       3)Goal:
//         a) include file for SPL.asm
//         b) include file for user source code in assembly language
//=======================================================================================
.ifndef __SPL162001_INC__
.define __SPL162001_INC__

//////////////////////////////////////////////////////////////////
//  Definitions for Peripheral Control Register
//////////////////////////////////////////////////////////////////
P_System_Ctrl_Base:             .VDEF           0x7800
P_Memory_Ctrl_Base:             .VDEF           0x7820
P_NANDFlash_Base:				.VDEF			0x7850
P_IOPort_Ctrl_Base:             .VDEF           0x7860
P_Interrupt_Ctrl_Base:          .VDEF           0x78A0
P_TimeBase_Ctrl_Base:           .VDEF           0x78B0
P_Timer_Ctrl_Base:              .VDEF           0x78C0
P_AudioOuput_Ctrl_Base:         .VDEF           0x78F0
P_UART_Ctrl_Base:           	.VDEF           0x7900
P_RTC_Ctrl_Base:                .VDEF           0x7920
P_SPI_Ctrl_Base:                .VDEF           0x7940
P_Analog_Ctrl_Base:				.VDEF			0x7960
P_LCD_Ctrol_Base:               .VDEF           0x7980
P_SD_Base:						.VDEF			0x79D0
P_USBHost_Base:					.VDEF			0x7b00
P_USBDevice_Base:				.VDEF			0x7b30
P_I2C_Base:						.VDEF			0x7b60
P_DMA_Base:						.VDEF			0x7b80
P_KeyScan_Base:					.VDEF			0x7bC0
P_MISC_Base:					.VDEF			0x7bd0
P_EFuse_Base:					.VDEF			0x7c30	
P_TFT_Base:						.VDEF			0x7d00

////////////////////////////////////////
//  System Control Registers
//      Base Address Start from 0x7800
////////////////////////////////////////
P_BodyID:                       .VDEF   P_System_Ctrl_Base+0x00
P_CLK_Ctrl0:					.VDEF   P_System_Ctrl_Base+0x04
P_CLK_Ctrl1:					.VDEF   P_System_Ctrl_Base+0x05
P_Reset_Flag:                   .VDEF   P_System_Ctrl_Base+0x06
P_Clock_Ctrl:                   .VDEF   P_System_Ctrl_Base+0x07
P_LVR_Ctrl:                     .VDEF   P_System_Ctrl_Base+0x08
P_WatchDog_Ctrl:                .VDEF   P_System_Ctrl_Base+0x0A
P_WatchDog_Clear:               .VDEF   P_System_Ctrl_Base+0x0B
P_WAIT:                         .VDEF   P_System_Ctrl_Base+0x0C
P_HALT:                         .VDEF   P_System_Ctrl_Base+0x0D
P_SLEEP:                        .VDEF   P_System_Ctrl_Base+0x0E
P_Power_State:					.VDEF   P_System_Ctrl_Base+0x0F
P_Segment_Num:					.VDEF   P_System_Ctrl_Base+0x10
P_MAPSEL:						.VDEF   P_System_Ctrl_Base+0x16
P_PLLChange:					.VDEF   P_System_Ctrl_Base+0x17
P_AD_Driving:					.VDEF   P_System_Ctrl_Base+0x1F

///////////////////////////////////////
//  Memory Control Registers
//      Base Address Start from 0x7820
////////////////////////////////////////
P_MCS0_Ctrl:                    .VDEF   P_Memory_Ctrl_Base+0x00
P_MCS1_Ctrl:                    .VDEF   P_Memory_Ctrl_Base+0x01
P_MCS2_Ctrl:                    .VDEF   P_Memory_Ctrl_Base+0x02
P_MCS3_Ctrl:                    .VDEF   P_Memory_Ctrl_Base+0x03
P_MCS4_Ctrl:                    .VDEF   P_Memory_Ctrl_Base+0x04
P_EMUCS_Ctrl:                   .VDEF   P_Memory_Ctrl_Base+0x05
P_MCS_BYTE_SEL:                 .VDEF   P_Memory_Ctrl_Base+0x06
P_MCS3_WETimingCtrl:			.VDEF   P_Memory_Ctrl_Base+0x07
P_MCS4_WETimingCtrl:			.VDEF   P_Memory_Ctrl_Base+0x08
P_MCS3_RDTimingCtrl:			.VDEF   P_Memory_Ctrl_Base+0x09
P_MCS4_RDTimingCtrl:			.VDEF   P_Memory_Ctrl_Base+0x0A
P_MCS3_TimingCtrl:				.VDEF   P_Memory_Ctrl_Base+0x0B
P_MCS4_TimingCtrl:				.VDEF   P_Memory_Ctrl_Base+0x0C


P_Mem_Ctrl:                     .VDEF   P_Memory_Ctrl_Base+0x20
P_Addr_Ctrl:                    .VDEF   P_Memory_Ctrl_Base+0x21
P_Bank_Switch:					.VDEF	0x7810

////////////////////////////////////////
//  I/O Port Control Registers
//      Base Address Start from 0x7860
////////////////////////////////////////
P_IOA_Data:                     .VDEF   P_IOPort_Ctrl_Base+0x00
P_IOA_Buffer:                   .VDEF   P_IOPort_Ctrl_Base+0x01
P_IOA_Dir:                      .VDEF   P_IOPort_Ctrl_Base+0x02
P_IOA_Attrib:                   .VDEF   P_IOPort_Ctrl_Base+0x03
P_IOB_Data:                     .VDEF   P_IOPort_Ctrl_Base+0x08
P_IOB_Buffer:                   .VDEF   P_IOPort_Ctrl_Base+0x09
P_IOB_Dir:                      .VDEF   P_IOPort_Ctrl_Base+0x0A
P_IOB_Attrib:                   .VDEF   P_IOPort_Ctrl_Base+0x0B
P_IOB_Latch:                    .VDEF   P_IOPort_Ctrl_Base+0x0C
P_IOC_Data:                     .VDEF   P_IOPort_Ctrl_Base+0x10
P_IOC_Buffer:                   .VDEF   P_IOPort_Ctrl_Base+0x11
P_IOC_Dir:                      .VDEF   P_IOPort_Ctrl_Base+0x12
P_IOC_Attrib:                   .VDEF   P_IOPort_Ctrl_Base+0x13
P_IOD_Data:                     .VDEF   P_IOPort_Ctrl_Base+0x18
P_IOD_Buffer:                   .VDEF   P_IOPort_Ctrl_Base+0x19
P_IOD_Dir:                      .VDEF   P_IOPort_Ctrl_Base+0x1A
P_IOD_Attrib:                   .VDEF   P_IOPort_Ctrl_Base+0x1B

////////////////////////////////////////
//  NAND Flash Control Registers
//      Base Address Start from 0x7850
////////////////////////////////////////
P_NF_Ctrl:						.vdef	P_NANDFlash_Base+0x00
P_NF_CMD:						.vdef	P_NANDFlash_Base+0x01	
P_NF_AddrL:						.vdef	P_NANDFlash_Base+0x02
P_NF_AddrH:						.vdef	P_NANDFlash_Base+0x03
P_NF_Data:						.vdef	P_NANDFlash_Base+0x04
P_NF_INT_Ctrl:					.vdef	P_NANDFlash_Base+0x05
P_ECC_Ctrl:						.vdef	P_NANDFlash_Base+0x07
P_ECC_LPRL_LB:					.vdef	P_NANDFlash_Base+0x08
P_ECC_LPRH_LB:					.vdef	P_NANDFlash_Base+0x09
P_ECC_CPR_LB:					.vdef	P_NANDFlash_Base+0x0A
P_ECC_LPR_CKL_LB:				.vdef	P_NANDFlash_Base+0x0B
P_ECC_LPR_CKH_LB:				.vdef	P_NANDFlash_Base+0x0C
P_ECC_CPCKR_LB:					.vdef	P_NANDFlash_Base+0x0D
P_ECC_ERR0_LB:					.vdef	P_NANDFlash_Base+0x0E
P_ECC_ERR1_LB:					.vdef	P_NANDFlash_Base+0x0F

P_ECC_LPRL_HB:					.vdef	0x7848
P_ECC_LPRH_HB:					.vdef	0x7849
P_ECC_CPR_HB:					.vdef	0x784A
P_ECC_LPR_CKL_HB:				.vdef	0x784B
P_ECC_LPR_CKH_HB:				.vdef	0x784C
P_ECC_CPCKR_HB:					.vdef	0x784D
P_ECC_ERR0_HB:					.vdef	0x784E
P_ECC_ERR1_HB:					.vdef	0x784F

P_CheckSUM0_LB:					.vdef	0x7830
P_CheckSUM1_LB:					.vdef	0x7831
P_CheckSUM0_HB:					.vdef	0x7832
P_CheckSUM1_HB:					.vdef	0x7833



////////////////////////////////////////
//  Interrupt Control Registers
//      Base Address Start from 0x78A0
////////////////////////////////////////
P_INT_Status1:                  .VDEF   P_Interrupt_Ctrl_Base+0x00
        .DEFINE C_INT_EXTA              0x0001
        .DEFINE C_INT_EXTB              0x0002
        .DEFINE	C_INT_DMA				0x0004
        .DEFINE	C_INT_USB				0x0008
        .DEFINE C_INT_AUDAFIFOEmpty     0x0010
        .DEFINE C_INT_AUDBFIFOEmpty     0x0020
        .DEFINE	C_INT_ADCAutoSample		0x0080
        .DEFINE C_INT_TouchPanel        0x0100	
        .DEFINE C_INT_FramePulse        0x0200
		.DEFINE	C_INT_SPI				0x0400
        .DEFINE C_INT_UART          	0x0800
        .DEFINE C_INT_TFTUderFlow		0x1000
        .DEFINE	C_INT_TFTFrameEnd		0x2000	
        .DEFINE C_INT_ADCReady      	0x4000
        .DEFINE C_INT_Key               0x8000
P_INT_Status2:                  .VDEF   P_Interrupt_Ctrl_Base+0x01
        .DEFINE C_INT_HMS               0x0001
        .DEFINE C_INT_Alarm             0x0002
        .DEFINE C_INT_Schedule          0x0004
        .DEFINE C_INT_NF                0x0010
		.DEFINE C_INT_I2C 	       		0x0020
        .DEFINE C_INT_SD	        	0x0040
        .DEFINE C_INT_TimeBaseA         0x0100
        .DEFINE C_INT_TimeBaseB         0x0200
        .DEFINE C_INT_TimeBaseC         0x0400
        .DEFINE C_INT_KeyScan           0x0800
        .DEFINE C_INT_TimerA            0x1000
        .DEFINE C_INT_TimerB            0x2000
        .DEFINE C_INT_TimerC            0x4000
        .DEFINE C_INT_TimerD            0x8000
P_INT_Priority1:                .VDEF   P_Interrupt_Ctrl_Base+0x04
P_INT_Priority2:                .VDEF   P_Interrupt_Ctrl_Base+0x05
P_MINT_Ctrl:                    .VDEF   P_Interrupt_Ctrl_Base+0x08
		.define	C_Key2_Flag				0x8000
		.define	C_Key1_Flag				0x2000
		.define	C_Key0_Flag				0x0800	

////////////////////////////////////////
//  TimeBase Control Registers
//      Base Address Start from 0x78B0
////////////////////////////////////////
P_TimeBaseA_Ctrl:               .VDEF   P_TimeBase_Ctrl_Base+0x00
P_TimeBaseB_Ctrl:               .VDEF   P_TimeBase_Ctrl_Base+0x01
P_TimeBaseC_Ctrl:               .VDEF   P_TimeBase_Ctrl_Base+0x02
P_TimeBase_Reset:               .VDEF   P_TimeBase_Ctrl_Base+0x08


////////////////////////////////////////
//  Timer Control Registers
//      Base Address Start from 0x78C0
////////////////////////////////////////
P_TimerA_Ctrl:                  .VDEF   P_Timer_Ctrl_Base+0x00
P_TimerA_CCP_Ctrl:              .VDEF   P_Timer_Ctrl_Base+0x01
P_TimerA_Preload:               .VDEF   P_Timer_Ctrl_Base+0x02
P_TimerA_CCP_Reg:               .VDEF   P_Timer_Ctrl_Base+0x03
P_TimerA_UpCount:		        .VDEF   P_Timer_Ctrl_Base+0x04
P_TimerB_Ctrl:                  .VDEF   P_Timer_Ctrl_Base+0x08
P_TimerB_CCP_Ctrl:              .VDEF   P_Timer_Ctrl_Base+0x09
P_TimerB_Preload:               .VDEF   P_Timer_Ctrl_Base+0x0A
P_TimerB_CCP_Reg:               .VDEF   P_Timer_Ctrl_Base+0x0B
P_TimerB_UpCount:  		        .VDEF   P_Timer_Ctrl_Base+0x0C
P_TimerC_Ctrl:                  .VDEF   P_Timer_Ctrl_Base+0x10
P_TimerC_CCP_Ctrl:              .VDEF   P_Timer_Ctrl_Base+0x11
P_TimerC_Preload:               .VDEF   P_Timer_Ctrl_Base+0x12
P_TimerC_CCP_Reg:               .VDEF   P_Timer_Ctrl_Base+0x13
P_TimerC_UpCount:               .VDEF   P_Timer_Ctrl_Base+0x14
P_TimerD_Ctrl:                  .VDEF   P_Timer_Ctrl_Base+0x18
P_TimerD_Preload:               .VDEF   P_Timer_Ctrl_Base+0x1A
P_TimerD_UpCount:               .VDEF   P_Timer_Ctrl_Base+0x1C
P_TimerE_Ctrl:                  .VDEF   P_Timer_Ctrl_Base+0x0100
P_TimerE_Preload:               .VDEF   P_Timer_Ctrl_Base+0x0102
P_TimerE_UpCount:               .VDEF   P_Timer_Ctrl_Base+0x0104
P_TimerF_Ctrl:                  .VDEF   P_Timer_Ctrl_Base+0x0108
P_TimerF_Preload:               .VDEF   P_Timer_Ctrl_Base+0x010A
P_TimerF_UpCount:               .VDEF   P_Timer_Ctrl_Base+0x010C

////////////////////////////////////////
//  Audio Output Control Registers
//      Base Address Start from 0x78F0
////////////////////////////////////////
P_CHA_Ctrl:                     .VDEF   P_AudioOuput_Ctrl_Base+0x00
P_CHA_Data:                     .VDEF   P_AudioOuput_Ctrl_Base+0x01
P_CHA_FIFO:                     .VDEF   P_AudioOuput_Ctrl_Base+0x02
P_CHB_Ctrl:                     .VDEF   P_AudioOuput_Ctrl_Base+0x08
P_CHB_Data:                     .VDEF   P_AudioOuput_Ctrl_Base+0x09
P_CHB_FIFO:                     .VDEF   P_AudioOuput_Ctrl_Base+0x0A
P_DAC_Ctrl:						.VDEF   P_AudioOuput_Ctrl_Base+0x0D
P_HPAMP_Ctrl:					.VDEF   P_AudioOuput_Ctrl_Base+0x0E
P_DAC_IIS_Ctrl:					.VDEF   P_AudioOuput_Ctrl_Base+0x0F

P_DAC_ACCREQ:					.VDEF   P_AudioOuput_Ctrl_Base+0x300
P_DAC_ACCDINL:					.VDEF   P_AudioOuput_Ctrl_Base+0x301
P_DAC_ACCDINH:					.VDEF   P_AudioOuput_Ctrl_Base+0x302
P_DAC_Effect_Ctrl:				.VDEF   P_AudioOuput_Ctrl_Base+0x303
P_DAC_ACThresL:					.VDEF   P_AudioOuput_Ctrl_Base+0x304
P_DAC_ACThresH:					.VDEF   P_AudioOuput_Ctrl_Base+0x305
P_DAC_EQBand_Sele:				.VDEF   P_AudioOuput_Ctrl_Base+0x306
P_DAC_EQSpecrum:				.VDEF   P_AudioOuput_Ctrl_Base+0x307
P_DAC_Volume3D:					.VDEF   P_AudioOuput_Ctrl_Base+0x308			
P_DAC_Volume3D_Cent:			.VDEF   P_AudioOuput_Ctrl_Base+0x309
P_DAC_Volume3D_Surround:		.VDEF   P_AudioOuput_Ctrl_Base+0x30A
P_DAC_Volume3D_R:				.VDEF   P_AudioOuput_Ctrl_Base+0x30B
P_DAC_Volume3D_L:				.VDEF   P_AudioOuput_Ctrl_Base+0x30C
P_DAC_ACCDOUTL:					.VDEF   P_AudioOuput_Ctrl_Base+0x30E
P_DAC_ACCDOUTH:					.VDEF   P_AudioOuput_Ctrl_Base+0x30F

////////////////////////////////////////
//  UART / IrDA Control Registers
//      Base Address Start from 0x7900
////////////////////////////////////////
P_UART_Data:                	.VDEF   P_UART_Ctrl_Base+0x00
P_UART_RXStatus:                .VDEF   P_UART_Ctrl_Base+0x01
P_UART_Ctrl:                	.VDEF   P_UART_Ctrl_Base+0x02
P_UART_BaudRate:            	.VDEF   P_UART_Ctrl_Base+0x03
P_UART_Status:              	.VDEF   P_UART_Ctrl_Base+0x04
P_UART_FIFO:                    .VDEF   P_UART_Ctrl_Base+0x05
P_UART_TXDelay:					.VDEF	P_UART_Ctrl_Base+0x06
P_IrDA_BuadRate:				.VDEF	P_UART_Ctrl_Base+0x07
P_IrDA_Ctrl:					.VDEF	P_UART_Ctrl_Base+0x08
P_IrDA_LowPower:				.VDEF	P_UART_Ctrl_Base+0x09	

////////////////////////////////////////
//  RTC Control Regsiters
//      Base Address Start from 0x7920
////////////////////////////////////////
P_Second:                       .VDEF   P_RTC_Ctrl_Base+0x00
P_Minute:                       .VDEF   P_RTC_Ctrl_Base+0x01
P_Hour:                         .VDEF   P_RTC_Ctrl_Base+0x02
P_Alarm_Second:                 .VDEF   P_RTC_Ctrl_Base+0x04
P_Alarm_Minute:                 .VDEF   P_RTC_Ctrl_Base+0x05
P_Alarm_Hour:                   .VDEF   P_RTC_Ctrl_Base+0x06
P_RTC_Ctrl:                     .VDEF   P_RTC_Ctrl_Base+0x14
		.DEFINE C_RTCEN                 0x8000
        .DEFINE C_ALMEN                 0x0400
        .DEFINE C_HMSEN                 0x0200
        .DEFINE C_SCHEMEN               0x0100
        .DEFINE C_16HzSchd              0x0000
        .DEFINE C_32HzSchd              0x0001
        .DEFINE C_64HzSchd              0x0002
        .DEFINE C_128HzSchd             0x0003
        .DEFINE C_256HzSchd             0x0004
        .DEFINE C_512HzSchd             0x0005
        .DEFINE C_1024HzSchd            0x0006
        .DEFINE C_2048HzSchd            0x0007
P_RTC_INT_Status:               .VDEF   P_RTC_Ctrl_Base+0x15
P_RTC_INT_Ctrl:                 .VDEF   P_RTC_Ctrl_Base+0x16
		.DEFINE C_HSecondINT            0x0001
        .DEFINE C_SecondINT             0x0002
        .DEFINE C_MinuteINT             0x0004
        .DEFINE C_HourINT               0x0008
        .DEFINE C_ScheduleINT           0x0100
        .DEFINE C_AlarmINT              0x0400
P_RTC_Busy:						.VDEF	P_RTC_Ctrl_Base+0x17	


////////////////////////////////////////
//  SPI Control Registers
//      Base Address Start from 0x7940
////////////////////////////////////////
P_SPI_Ctrl:                     .VDEF   P_SPI_Ctrl_Base+0x00
P_SPI_TXStatus:                 .VDEF   P_SPI_Ctrl_Base+0x01
P_SPI_TXData:	                .VDEF   P_SPI_Ctrl_Base+0x02
P_SPI_RXStatus:                 .VDEF   P_SPI_Ctrl_Base+0x03
P_SPI_RXData:   	        	.VDEF   P_SPI_Ctrl_Base+0x04
P_SPI_Misc:	   	        		.VDEF   P_SPI_Ctrl_Base+0x05

////////////////////////////////////////
//  LCD Control Registers
//      Base Address Start from 0x7980
////////////////////////////////////////
P_LCD_Setup:                    .VDEF   P_LCD_Ctrol_Base+0x00
P_LCD_Clock:                    .VDEF   P_LCD_Ctrol_Base+0x01
P_LCD_Segment:                  .VDEF   P_LCD_Ctrol_Base+0x02
P_LCD_Common:                   .VDEF   P_LCD_Ctrol_Base+0x03
P_LCD_Buffer_LowAdr:            .VDEF   P_LCD_Ctrol_Base+0x04
P_LCD_Buffer_HighAdr:           .VDEF   P_LCD_Ctrol_Base+0x05
P_LCD_Buffer_Offset:            .VDEF   P_LCD_Ctrol_Base+0x06
P_LCD_Timing_Ctrl:              .VDEF   P_LCD_Ctrol_Base+0x07
P_LCD_Frame_Ctrl:               .VDEF   P_LCD_Ctrol_Base+0x08
P_LCD_Palette_Ctrl:             .VDEF   P_LCD_Ctrol_Base+0x09
P_LCD_Attri_Ctrl:	        	.VDEF   P_LCD_Ctrol_Base+0x0A

//Palette[0:255] 				0x7A00 ~ 0x7AFF
P_LCD_Palette:    	      	    .VDEF	0x7A00


////////////////////////////////////////
//  TFT Control Registers
//      Base Address Start from 0x7D00
////////////////////////////////////////
P_TFT_Ctrl:						.VDEF	P_TFT_Base+0x00
P_TFT_DCLK_Ctrl:				.VDEF	P_TFT_Base+0x01
P_TFT_INT_Ctrl:					.VDEF	P_TFT_Base+0x02	
P_TFT_H_Width:					.VDEF	P_TFT_Base+0x03
P_TFT_H_Start:					.VDEF	P_TFT_Base+0x04
P_TFT_H_End:					.VDEF	P_TFT_Base+0x05
P_TFT_HSync_Setup:				.VDEF	P_TFT_Base+0x06
P_TFT_V_Width:					.VDEF	P_TFT_Base+0x07
P_TFT_V_Start:					.VDEF	P_TFT_Base+0x08
P_TFT_V_End:					.VDEF	P_TFT_Base+0x09
P_TFT_VSync_Setup:				.VDEF	P_TFT_Base+0x0a
P_TFT_RGB_Ctrl:					.VDEF	P_TFT_Base+0x0b
P_TFT_YUV_Ctrl:					.VDEF	P_TFT_Base+0x0c
P_TFT_DMAStart_AH:				.VDEF	P_TFT_Base+0x0d
P_TFT_DMAStart_AL:				.VDEF	P_TFT_Base+0x0e
P_TFT_DMA_Offset:				.VDEF	P_TFT_Base+0x0f
P_TFT_Pixel_Num:				.VDEF	P_TFT_Base+0x10
P_TFT_Line_Num:					.VDEF	P_TFT_Base+0x11		
P_TFT_PIP0_Ctrl:				.VDEF	P_TFT_Base+0x12
P_TFT_PIP0_VirFrame_SAH:		.VDEF	P_TFT_Base+0x13
P_TFT_PIP0_VirFrame_SAL:		.VDEF	P_TFT_Base+0x14
P_TFT_PIP0_VirFrame_EAH:		.VDEF	P_TFT_Base+0x15
P_TFT_PIP0_VirFrame_EAL:		.VDEF	P_TFT_Base+0x16
P_TFT_PIP0_STR_AH:				.VDEF	P_TFT_Base+0x17
P_TFT_PIP0_STR_AL:				.VDEF	P_TFT_Base+0x18
P_TFT_PIP0_H_Start:				.VDEF	P_TFT_Base+0x19
P_TFT_PIP0_H_End:				.VDEF	P_TFT_Base+0x1a
P_TFT_PIP0_V_Start:				.VDEF	P_TFT_Base+0x1b
P_TFT_PIP0_V_End:				.VDEF	P_TFT_Base+0x1c

P_TFT_PIP1_Ctrl:				.VDEF	P_TFT_Base+0x1d
P_TFT_PIP1_VirFrame_SAH:		.VDEF	P_TFT_Base+0x1e
P_TFT_PIP1_VirFrame_SAL:		.VDEF	P_TFT_Base+0x1f
P_TFT_PIP1_VirFrame_EAH:		.VDEF	P_TFT_Base+0x20
P_TFT_PIP1_VirFrame_EAL:		.VDEF	P_TFT_Base+0x21
P_TFT_PIP1_STR_AH:				.VDEF	P_TFT_Base+0x22
P_TFT_PIP1_STR_AL:				.VDEF	P_TFT_Base+0x23
P_TFT_PIP1_H_Start:				.VDEF	P_TFT_Base+0x24
P_TFT_PIP1_H_End:				.VDEF	P_TFT_Base+0x25
P_TFT_PIP1_V_Start:				.VDEF	P_TFT_Base+0x26
P_TFT_PIP1_V_End:				.VDEF	P_TFT_Base+0x27

P_TFT_PIP2_Ctrl:				.VDEF	P_TFT_Base+0x28
P_TFT_PIP2_VirFrame_SAH:		.VDEF	P_TFT_Base+0x29
P_TFT_PIP2_VirFrame_SAL:		.VDEF	P_TFT_Base+0x2a
P_TFT_PIP2_VirFrame_EAH:		.VDEF	P_TFT_Base+0x2b
P_TFT_PIP2_VirFrame_EAL:		.VDEF	P_TFT_Base+0x2c
P_TFT_PIP2_STR_AH:				.VDEF	P_TFT_Base+0x2d
P_TFT_PIP2_STR_AL:				.VDEF	P_TFT_Base+0x2e
P_TFT_PIP2_H_Start:				.VDEF	P_TFT_Base+0x2f
P_TFT_PIP2_H_End:				.VDEF	P_TFT_Base+0x30
P_TFT_PIP2_V_Start:				.VDEF	P_TFT_Base+0x31
P_TFT_PIP2_V_End:				.VDEF	P_TFT_Base+0x32

P_TFT_PIP3_Ctrl:				.VDEF	P_TFT_Base+0x33
P_TFT_PIP3_VirFrame_SAH:		.VDEF	P_TFT_Base+0x34
P_TFT_PIP3_VirFrame_SAL:		.VDEF	P_TFT_Base+0x35
P_TFT_PIP3_VirFrame_EAH:		.VDEF	P_TFT_Base+0x36
P_TFT_PIP3_VirFrame_EAL:		.VDEF	P_TFT_Base+0x37
P_TFT_PIP3_STR_AH:				.VDEF	P_TFT_Base+0x38
P_TFT_PIP3_STR_AL:				.VDEF	P_TFT_Base+0x39
P_TFT_PIP3_H_Start:				.VDEF	P_TFT_Base+0x3a
P_TFT_PIP3_H_End:				.VDEF	P_TFT_Base+0x3b
P_TFT_PIP3_V_Start:				.VDEF	P_TFT_Base+0x3c
P_TFT_PIP3_V_End:				.VDEF	P_TFT_Base+0x3d



////////////////////////////////////////
//  DMA Control Registers
//      Base Address Start from 0x7B80
////////////////////////////////////////
P_DMA_Ctrl0:					.VDEF	P_DMA_Base+0x00
P_DMA_SRC_AddrL0:				.VDEF	P_DMA_Base+0x01
P_DMA_TAR_AddrL0:				.VDEF	P_DMA_Base+0x02
P_DMA_TCountL0:					.VDEF	P_DMA_Base+0x03
P_DMA_SRC_AddrH0:				.VDEF	P_DMA_Base+0x04
P_DMA_TAR_AddrH0:				.VDEF	P_DMA_Base+0x05
P_DMA_TCountH0:					.VDEF	P_DMA_Base+0x06
P_DMA_MISC0:					.VDEF	P_DMA_Base+0x07

P_DMA_Ctrl1:					.VDEF	P_DMA_Base+0x08
P_DMA_SRC_AddrL1:				.VDEF	P_DMA_Base+0x09
P_DMA_TAR_AddrL1:				.VDEF	P_DMA_Base+0x0A
P_DMA_TCountL1:					.VDEF	P_DMA_Base+0x0B
P_DMA_SRC_AddrH1:				.VDEF	P_DMA_Base+0x0C
P_DMA_TAR_AddrH1:				.VDEF	P_DMA_Base+0x0D
P_DMA_TCountH1:					.VDEF	P_DMA_Base+0x0E
P_DMA_MISC1:					.VDEF	P_DMA_Base+0x0F

P_DMA_Ctrl2:					.VDEF	P_DMA_Base+0x10
P_DMA_SRC_AddrL2:				.VDEF	P_DMA_Base+0x11
P_DMA_TAR_AddrL2:				.VDEF	P_DMA_Base+0x12
P_DMA_TCountL2:					.VDEF	P_DMA_Base+0x13
P_DMA_SRC_AddrH2:				.VDEF	P_DMA_Base+0x14
P_DMA_TAR_AddrH2:				.VDEF	P_DMA_Base+0x15
P_DMA_TCountH2:					.VDEF	P_DMA_Base+0x16
P_DMA_MISC2:					.VDEF	P_DMA_Base+0x17

P_DMA_Ctrl3:					.VDEF	P_DMA_Base+0x18
P_DMA_SRC_AddrL3:				.VDEF	P_DMA_Base+0x19
P_DMA_TAR_AddrL3:				.VDEF	P_DMA_Base+0x1A
P_DMA_TCountL3:					.VDEF	P_DMA_Base+0x1B
P_DMA_SRC_AddrH3:				.VDEF	P_DMA_Base+0x1C
P_DMA_TAR_AddrH3:				.VDEF	P_DMA_Base+0x1D
P_DMA_TCountH3:					.VDEF	P_DMA_Base+0x1E
P_DMA_MISC3:					.VDEF	P_DMA_Base+0x1F

P_DMA_SPRISize0:				.VDEF	P_DMA_Base+0x30
P_DMA_SPRISize1:				.VDEF	P_DMA_Base+0x31
P_DMA_SPRISize2:				.VDEF	P_DMA_Base+0x32
P_DMA_SPRISize3:				.VDEF	P_DMA_Base+0x33

P_DMA_Transpar0:				.VDEF	P_DMA_Base+0x38
P_DMA_Transpar1:				.VDEF	P_DMA_Base+0x39
P_DMA_Transpar2:				.VDEF	P_DMA_Base+0x3A
P_DMA_Transpar3:				.VDEF	P_DMA_Base+0x3B
P_DMA_LineLenght:				.VDEF	P_DMA_Base+0x3D

P_DMA_SS:			.VDEF	P_DMA_Base+0x3E
P_DMA_INT:			.VDEF	P_DMA_Base+0x3F


.define		C_DMA0_ISRFlag		0x0001
.define		C_DMA1_ISRFlag		0x0002
.define		C_DMA2_ISRFlag		0x0004
.define		C_DMA3_ISRFlag		0x0008
.define		C_DMA0_BusyFlag		0x0100
.define		C_DMA1_BusyFlag		0x0200
.define		C_DMA2_BusyFlag		0x0400
.define		C_DMA3_BusyFlag		0x0800

////////////////////////////////////////
//  KeyScan Control Registers
//      Base Address Start from 0x7BC0
////////////////////////////////////////
P_KS_Ctrl:			.VDEF	P_KeyScan_Base+0x00
P_KS_Data0:			.VDEF	P_KeyScan_Base+0x08
P_KS_Data1:			.VDEF	P_KeyScan_Base+0x09
P_KS_Data2:			.VDEF	P_KeyScan_Base+0x0A	
P_KS_Data3:			.VDEF	P_KeyScan_Base+0x0B
P_KS_Data4:			.VDEF	P_KeyScan_Base+0x0C
P_KS_Data5:			.VDEF	P_KeyScan_Base+0x0D
P_KS_Data6:			.VDEF	P_KeyScan_Base+0x0E
P_KS_Data7:			.VDEF	P_KeyScan_Base+0x0F


////////////////////////////////////////
//  ADC Control Registers
//      Base Address Start from 0x7960
////////////////////////////////////////
P_ADC_Setup:			.VDEF	P_Analog_Ctrl_Base+0x00
P_MADC_Ctrl:			.VDEF	P_Analog_Ctrl_Base+0x01
P_MADC_Data:			.VDEF	P_Analog_Ctrl_Base+0x02
P_ASADC_Ctrl:			.VDEF	P_Analog_Ctrl_Base+0x03
P_ASADC_Data:			.VDEF	P_Analog_Ctrl_Base+0x04
P_TP_Ctrl:				.VDEF	P_Analog_Ctrl_Base+0x05

P_HQADC_Ctrl:			.VDEF	P_Analog_Ctrl_Base+0x10
P_HQADC_MIC_PGA:		.VDEF	P_Analog_Ctrl_Base+0x11
P_HQADC_R_Gain:			.VDEF	P_Analog_Ctrl_Base+0x12
P_HQADC_L_Gain:			.VDEF	P_Analog_Ctrl_Base+0x13

////////////////////////////////////////
//  SD Control Registers
//      Base Address Start from 0x79D0
////////////////////////////////////////
P_SD_DataTX:			.VDEF	P_SD_Base+0x00
P_SD_DataRX:			.VDEF	P_SD_Base+0x01
P_SD_CMD:			.VDEF	P_SD_Base+0x02
P_SD_ArgL:			.VDEF	P_SD_Base+0x03
P_SD_ArgH:			.VDEF	P_SD_Base+0x04
P_SD_RespL:			.VDEF	P_SD_Base+0x05
P_SD_RespH:			.VDEF	P_SD_Base+0x06
P_SD_Status:			.VDEF	P_SD_Base+0x07
P_SD_Ctrl:			.VDEF	P_SD_Base+0x08
P_SD_BLKLEN:			.VDEF	P_SD_Base+0x09
P_SD_INT:			.VDEF	P_SD_Base+0x0A
	.DEFINE	SD_CommandComplete_INT		0x0001
	.DEFINE	SD_DataComplete_INT		0x0002
	.DEFINE	SD_CommandBufferFull_INT		0x0004
	.DEFINE	SD_DataBufferFull_INT		0x0008
	.DEFINE	SD_DataBufferEmpty_INT		0x0010
	.DEFINE	SD_CardInsert_INT		0x0020
	.DEFINE	SD_SDIOCard_INT			0x0040


////////////////////////////////////////
//  I2C Control Registers
//      Base Address Start from 0x7b60
////////////////////////////////////////
P_I2C_Ctrl:				.VDEF	P_I2C_Base+0x00
P_I2C_Status:			.VDEF	P_I2C_Base+0x01
P_I2C_Addr:				.VDEF	P_I2C_Base+0x02
P_I2C_Data:				.VDEF	P_I2C_Base+0x03
P_I2C_DeCLK:			.VDEF	P_I2C_Base+0x04
P_I2C_En:				.VDEF	P_I2C_Base+0x05



////////////////////////////////////////
//  USB Device Control Registers
//      Base Address Start from 0x7b30
////////////////////////////////////////
P_USBD_Config:			.VDEF	P_USBDevice_Base+0x00
P_USBD_Device:			.VDEF	P_USBDevice_Base+0x27
P_USBD_Function:		.VDEF	P_USBDevice_Base+0x01
P_USBD_DMAINT:			.VDEF	P_USBDevice_Base+0x29

P_USBD_PMR:				.VDEF	P_USBDevice_Base+0x02
P_USBD_EP0Data:			.VDEF	P_USBDevice_Base+0x03
P_USBD_BIData:			.VDEF	P_USBDevice_Base+0x04
P_USBD_BOData:			.VDEF	P_USBDevice_Base+0x05
P_USBD_INTINData:		.VDEF	P_USBDevice_Base+0x06
P_USBD_NullPkt:			.VDEF	P_USBDevice_Base+0x28

P_USBD_EPEvent:			.VDEF	P_USBDevice_Base+0x07	
P_USBD_GLOINT:			.VDEF	P_USBDevice_Base+0x08
P_USBD_INTEN:			.VDEF	P_USBDevice_Base+0x09
P_USBD_INTF:			.VDEF	P_USBDevice_Base+0x0A
P_USBD_SCINTEM:			.VDEF	P_USBDevice_Base+0x0B
P_USBD_SCINTF:			.VDEF	P_USBDevice_Base+0x0C
P_USBD_EPAutoSet:		.VDEF	P_USBDevice_Base+0x0D
P_USBD_EPSetStall:		.VDEF	P_USBDevice_Base+0x0E
P_USBD_EPBufClear:		.VDEF	P_USBDevice_Base+0x0F
P_USBD_EPEvntClear:		.VDEF	P_USBDevice_Base+0x10
P_USBD_EP0WrtCount:		.VDEF	P_USBDevice_Base+0x11
P_USBD_BOWrtCount:		.VDEF	P_USBDevice_Base+0x12
P_USBD_EP0BufPointer:	.VDEF	P_USBDevice_Base+0x13
P_USBD_BBufRdPointer:	.VDEF	P_USBDevice_Base+0x14
P_USBD_BBufWrtPointer:	.VDEF	P_USBDevice_Base+0x15
P_USBD_EP0RTR:			.VDEF	P_USBDevice_Base+0x16
P_USBD_EP0RR:			.VDEF	P_USBDevice_Base+0x17
P_USBD_EP0VR:			.VDEF	P_USBDevice_Base+0x18
P_USBD_EP0IR:			.VDEF	P_USBDevice_Base+0x19
P_USBD_EP0LR:			.VDEF	P_USBDevice_Base+0x1A


P_USBD_DMAWrtCountL:	.VDEF	P_USBDevice_Base+0x20
P_USBD_DMAWrtCountH:	.VDEF	P_USBDevice_Base+0x21
P_USBD_DMAAckL:			.VDEF	P_USBDevice_Base+0x22
P_USBD_DMAAckH:			.VDEF	P_USBDevice_Base+0x23
P_USBD_EPStall:			.VDEF	P_USBDevice_Base+0x24

////////////////////////////////////////
//  USB Host Control Registers
//      Base Address Start from 0x7b00
////////////////////////////////////////
P_USBH_Config:				.VDEF	P_USBHost_Base+0x00
P_USBH_TimeConfig:			.VDEF	P_USBHost_Base+0x01
P_USBH_Data:				.VDEF	P_USBHost_Base+0x02
P_USBH_Transfer:			.VDEF	P_USBHost_Base+0x03
P_USBH_DveAddr:				.VDEF	P_USBHost_Base+0x04
P_USBH_DveEp:				.VDEF	P_USBHost_Base+0x05
P_USBH_TXCount:				.VDEF	P_USBHost_Base+0x06
P_USBH_RXCount:				.VDEF	P_USBHost_Base+0x07
P_USBH_FIFOInPointer:		.VDEF	P_USBHost_Base+0x08
P_USBH_FIFOOutPointer:		.VDEF	P_USBHost_Base+0x09
P_USBH_AutoInByteCount:		.VDEF	P_USBHost_Base+0x0A
P_USBH_AutoOutByteCount:	.VDEF	P_USBHost_Base+0x0B
P_USBH_AutoTrans:			.VDEF	P_USBHost_Base+0x0C
P_USBH_Status:				.VDEF	P_USBHost_Base+0x0D
P_USBH_INT:					.VDEF	P_USBHost_Base+0x0E
P_USBH_INTEN:				.VDEF	P_USBHost_Base+0x0F
P_USBH_StorageRST:			.VDEF	P_USBHost_Base+0x10
P_USBH_SoftRST:				.VDEF	P_USBHost_Base+0x11
P_USBH_SOFTimer:			.VDEF	P_USBHost_Base+0x12
P_USBH_FrameNum:			.VDEF	P_USBHost_Base+0x13
P_USBH_OTGConfig:			.VDEF	P_USBHost_Base+0x14
P_USBH_VBusSet:				.VDEF	P_USBHost_Base+0x15
P_USBH_VBusStatus:			.VDEF	P_USBHost_Base+0x16
P_USBH_INAckCount:			.VDEF	P_USBHost_Base+0x17
P_USBH_OutAckCount:			.VDEF	P_USBHost_Base+0x18
P_USBH_RSTAckCount:			.VDEF	P_USBHost_Base+0x19
P_USBH_Storage:				.VDEF	P_USBHost_Base+0x1A
P_USBH_DReadBack:			.VDEF	P_USBHost_Base+0x1B



/////////////////////////////////
//	miscellaneous control register
//	Base address is from 0x7BD0
/////////////////////////////////
P_Byte_Swap:				.VDEF	P_MISC_Base+0x00
P_Nibble_Swap:				.VDEF	P_MISC_Base+0x01
P_TowBit_Swap:				.VDEF	P_MISC_Base+0x02
P_Bit_Reverse:				.VDEF	P_MISC_Base+0x03

/////////////////////////////////
//	EFuse control register
//	Base address is from 0x7c30
/////////////////////////////////
P_EFuse_D0:					.VDEF	P_EFuse_Base+0x00
P_EFuse_D1:					.VDEF	P_EFuse_Base+0x01
P_EFuse_D2:					.VDEF	P_EFuse_Base+0x02
P_EFuse_D3:					.VDEF	P_EFuse_Base+0x03


//========================================================================================
// End of include file
//========================================================================================

.endif

