#ifndef __SPL162001_H__
#define __SPL162001_H__
//========================================================================================
// Progarm: The C header file to be included for SPL161001 Series V1.0
// Arranged by: Jay
// Platform:  unSP SPL162001 
//
// Date:        2005/Aug  V1.0 ( first version )
//
// Note: This H file defines the control register available for user to use SPL series.
//       The control regsiter setting constants is also included for users' convenience.
//========================================================================================




//////////////////////////////////////////////////////////////////
//  Definitions for Peripheral Control Register
//////////////////////////////////////////////////////////////////
#define		P_System_Ctrl_Base             	0x7800
#define		P_Memory_Ctrl_Base             	0x7820
#define		P_NANDFlash_Base				0x7850	
#define		P_IOPort_Ctrl_Base             	0x7860
#define		P_Interrupt_Ctrl_Base          	0x78A0
#define		P_TimeBase_Ctrl_Base           	0x78B0
#define		P_Timer_Ctrl_Base              	0x78C0
#define		P_AudioOuput_Ctrl_Base         	0x78F0
#define		P_UARTIrDA_Ctrl_Base           	0x7900
#define		P_RTC_Ctrl_Base                	0x7920
#define		P_SPI_Ctrl_Base                	0x7940
#define		P_Analog_Ctrl_Base				0x7960
#define		P_LCD_Ctrol_Base               	0x7980 
#define		P_SD_Base						0x79D0      			
#define		P_USBHost_Base					0x7b00
#define		P_USBDevice_Base				0x7b30	
#define		P_I2C_Base						0x7b60							
#define		P_DMA_Base						0x7b80
#define		P_KeyScan_Base					0x7bC0
#define		P_MISC_Base						0x7bd0
#define		P_EFuse_Base					0x7c30	
#define		P_TFT_Base						0x7d00

////////////////////////////////////////
//  System Control Registers
//      Base Address Start from 0x7800
////////////////////////////////////////
#define		P_BodyID					(volatile unsigned int*)(P_System_Ctrl_Base+0x00)
#define		P_CLK_Ctrl0					(volatile unsigned int*)(P_System_Ctrl_Base+0x04)
#define		P_CLK_Ctrl1					(volatile unsigned int*)(P_System_Ctrl_Base+0x05)
#define		P_Reset_Flag                (volatile unsigned int*)(P_System_Ctrl_Base+0x06)
#define		P_Clock_Ctrl               	(volatile unsigned int*)(P_System_Ctrl_Base+0x07)
#define		P_LVR_Ctrl                 	(volatile unsigned int*)(P_System_Ctrl_Base+0x08)
#define		P_WatchDog_Ctrl             (volatile unsigned int*)(P_System_Ctrl_Base+0x0A)
#define		P_WatchDog_Clear            (volatile unsigned int*)(P_System_Ctrl_Base+0x0B)
#define		P_WAIT                      (volatile unsigned int*)(P_System_Ctrl_Base+0x0C)
#define		P_HALT                      (volatile unsigned int*)(P_System_Ctrl_Base+0x0D)
#define		P_SLEEP                     (volatile unsigned int*)(P_System_Ctrl_Base+0x0E)
#define		P_Power_State				(volatile unsigned int*)(P_System_Ctrl_Base+0x0F)
#define		P_Segment_Num				(volatile unsigned int*)(P_System_Ctrl_Base+0x10)
#define		P_MAPSEL					(volatile unsigned int*)(P_System_Ctrl_Base+0x16)
#define		P_PLLN						(volatile unsigned int*)(P_System_Ctrl_Base+0x17)
#define		P_AD_Driving				(volatile unsigned int*)(P_System_Ctrl_Base+0x1F)

///////////////////////////////////////
//  Memory Control Registers
//      Base Address Start from 0x7820
////////////////////////////////////////
#define		P_MCS0_Ctrl                    	(volatile unsigned int*)(P_Memory_Ctrl_Base+0x00)
#define		P_MCS1_Ctrl                    	(volatile unsigned int*)(P_Memory_Ctrl_Base+0x01)
#define		P_MCS2_Ctrl                    	(volatile unsigned int*)(P_Memory_Ctrl_Base+0x02)
#define		P_MCS3_Ctrl                    	(volatile unsigned int*)(P_Memory_Ctrl_Base+0x03)
#define		P_MCS4_Ctrl                    	(volatile unsigned int*)(P_Memory_Ctrl_Base+0x04)
#define		P_EMUCS_Ctrl   					(volatile unsigned int*)(P_Memory_Ctrl_Base+0x05)
#define		P_MCS_BYTE_SEL                 	(volatile unsigned int*)(P_Memory_Ctrl_Base+0x06)
#define		P_MCS3_WETimingCtrl				(volatile unsigned int*)(P_Memory_Ctrl_Base+0x07)
#define		P_MCS4_WETimingCtrl				(volatile unsigned int*)(P_Memory_Ctrl_Base+0x08)
#define		P_MCS3_RDTimingCtrl				(volatile unsigned int*)(P_Memory_Ctrl_Base+0x09)
#define		P_MCS4_RDTimingCtrl				(volatile unsigned int*)(P_Memory_Ctrl_Base+0x0A)
#define		P_MCS3_TimingCtrl				(volatile unsigned int*)(P_Memory_Ctrl_Base+0x0B)
#define		P_MCS4_TimingCtrl				(volatile unsigned int*)(P_Memory_Ctrl_Base+0x0C)

#define		P_Mem_Ctrl         	            (volatile unsigned int*)(P_Memory_Ctrl_Base+0x20)
#define		P_Addr_Ctrl         	        (volatile unsigned int*)(P_Memory_Ctrl_Base+0x21)

////////////////////////////////////////
//  I/O Port Control Registers
//      Base Address Start from 0x7860
////////////////////////////////////////
#define		P_IOA_Data                     (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x00)
#define		P_IOA_Buffer                   (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x01)
#define		P_IOA_Dir                      (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x02)
#define		P_IOA_Attrib                   (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x03)
#define		P_IOB_Data                     (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x08)
#define		P_IOB_Buffer                   (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x09)
#define		P_IOB_Dir                      (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x0A)
#define		P_IOB_Attrib                   (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x0B)
#define		P_IOB_Latch                    (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x0C)
#define		P_IOC_Data                     (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x10)
#define		P_IOC_Buffer                   (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x11)
#define		P_IOC_Dir                      (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x12)
#define		P_IOC_Attrib                   (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x13)
#define		P_IOD_Data                     (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x18)
#define		P_IOD_Buffer                   (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x19)
#define		P_IOD_Dir                      (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x1A)
#define		P_IOD_Attrib                   (volatile unsigned int*)(P_IOPort_Ctrl_Base+0x1B)

////////////////////////////////////////
//  NAND Flash Control Registers
//      Base Address Start from 0x7850
////////////////////////////////////////
#define		P_NF_Ctrl					(volatile unsigned int*)(P_NANDFlash_Base+0x00)
#define		P_NF_CMD					(volatile unsigned int*)(P_NANDFlash_Base+0x01)	
#define		P_NF_AddrL					(volatile unsigned int*)(P_NANDFlash_Base+0x02)
#define		P_NF_AddrH					(volatile unsigned int*)(P_NANDFlash_Base+0x03)
#define		P_NF_Data					(volatile unsigned int*)(P_NANDFlash_Base+0x04)
#define		P_NF_INT_Ctrl				(volatile unsigned int*)(P_NANDFlash_Base+0x05)
#define		P_ECC_Ctrl					(volatile unsigned int*)(P_NANDFlash_Base+0x07)
#define		P_ECC_LPRL_LB				(volatile unsigned int*)(P_NANDFlash_Base+0x08)
#define		P_ECC_LPRH_LB				(volatile unsigned int*)(P_NANDFlash_Base+0x09)
#define		P_ECC_CPR_LB				(volatile unsigned int*)(P_NANDFlash_Base+0x0A)
#define		P_ECC_LPR_CKL_LB			(volatile unsigned int*)(P_NANDFlash_Base+0x0B)
#define		P_ECC_LPR_CKH_LB			(volatile unsigned int*)(P_NANDFlash_Base+0x0C)
#define		P_ECC_CPCKR_LB				(volatile unsigned int*)(P_NANDFlash_Base+0x0D)
#define		P_ECC_ERR0_LB				(volatile unsigned int*)(P_NANDFlash_Base+0x0E)
#define		P_ECC_ERR1_LB				(volatile unsigned int*)(P_NANDFlash_Base+0x0F)
#define		P_ECC_LPRL_HB				(volatile unsigned int*)(0x7848)
#define		P_ECC_LPRH_HB				(volatile unsigned int*)(0x7849)
#define		P_ECC_CPR_HB				(volatile unsigned int*)(0x784A)
#define		P_ECC_LPR_CKL_HB			(volatile unsigned int*)(0x784B)
#define		P_ECC_LPR_CKH_HB			(volatile unsigned int*)(0x784C)
#define		P_ECC_CPCKR_HB				(volatile unsigned int*)(0x784D)
#define		P_ECCERR0_HB				(volatile unsigned int*)(0x784E)
#define		P_ECCERR1_HB				(volatile unsigned int*)(0x784F)
#define		P_CheckSUM0_LB				(volatile unsigned int*)(0x7830)	
#define		P_CheckSUM1_LB				(volatile unsigned int*)(0x7831)	
#define		P_CheckSUM0_HB				(volatile unsigned int*)(0x7832)	
#define		P_CheckSUM1_HB				(volatile unsigned int*)(0x7833)	

////////////////////////////////////////
//  Interrupt Control Registers
//      Base Address Start from 0x78A0
////////////////////////////////////////
#define		P_INT_Status1                  (volatile unsigned int*)(P_Interrupt_Ctrl_Base+0x00)
#define 	 C_INT_EXTA              0x0001;
#define 	 C_INT_EXTB              0x0002;
#define		 C_INT_DMA				 0x0004;
#define      C_INT_USB				 0x0008;
#define		 C_INT_DACAFIFOEmpty     0x0010;
#define 	 C_INT_DACBFIFOEmpty     0x0020;
#define		 C_INT_ADCAutoSample	 0x0080;
#define 	 C_INT_TouchPanel        0x0100;
#define		 C_INT_FramePulse        0x0200;
#define		 C_INT_SPI		 		 0x0400;
#define 	 C_INT_UART          	 0x0800;
#define		 C_INT_TFTUderFlow		 0x1000;
#define	     C_INT_TFTFrameEnd		 0x2000;
#define		 C_INT_ADCReady          0x4000;	
#define		 C_INT_Key               0x8000;

#define		P_INT_Status2                  (volatile unsigned int*)(P_Interrupt_Ctrl_Base+0x01)
#define		 C_INT_HMS               0x0001;
#define		 C_INT_Alarm             0x0002;
#define		 C_INT_Schedule          0x0004;
#define		 C_INT_NF                0x0010;
#define 	 C_INT_I2C    			 0x0020;
#define		 C_INT_SD	        	 0x0040;
#define		 C_INT_TimeBaseA         0x0100;
#define		 C_INT_TimeBaseB         0x0200;
#define		 C_INT_TimeBaseC         0x0400;
#define		 C_INT_KeyScan           0x0800;
#define		 C_INT_TimerA            0x1000;
#define		 C_INT_TimerB            0x2000;
#define		 C_INT_TimerC            0x4000;
#define		 C_INT_TimerD            0x8000;

#define		P_INT_Priority1                (volatile unsigned int*)(P_Interrupt_Ctrl_Base+0x04)
#define		P_INT_Priority2                (volatile unsigned int*)(P_Interrupt_Ctrl_Base+0x05)
#define		P_MINT_Ctrl                    (volatile unsigned int*)(P_Interrupt_Ctrl_Base+0x08)
#define		C_Key2_Flag				0x8000;
#define		C_Key1_Flag				0x2000;
#define		C_Key0_Flag				0x0800;

////////////////////////////////////////
//  TimeBase Control Registers
//      Base Address Start from 0x78B0
////////////////////////////////////////
#define		P_TimeBaseA_Ctrl               (volatile unsigned int*)(P_TimeBase_Ctrl_Base+0x00)
#define		P_TimeBaseB_Ctrl               (volatile unsigned int*)(P_TimeBase_Ctrl_Base+0x01)
#define		P_TimeBaseC_Ctrl               (volatile unsigned int*)(P_TimeBase_Ctrl_Base+0x02)
#define		P_TimeBase_Reset               (volatile unsigned int*)(P_TimeBase_Ctrl_Base+0x08)


////////////////////////////////////////
//  Timer Control Registers
//      Base Address Start from 0x78C0
////////////////////////////////////////
#define		P_TimerA_Ctrl                  (volatile unsigned int*)(P_Timer_Ctrl_Base+0x00)
#define		P_TimerA_CCP_Ctrl              (volatile unsigned int*)(P_Timer_Ctrl_Base+0x01)
#define		P_TimerA_Preload               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x02)
#define		P_TimerA_CCP_Reg               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x03)
#define		P_TimerA_CCP_UpCount           (volatile unsigned int*)(P_Timer_Ctrl_Base+0x04)
#define		P_TimerB_Ctrl                  (volatile unsigned int*)(P_Timer_Ctrl_Base+0x08)
#define		P_TimerB_CCP_Ctrl              (volatile unsigned int*)(P_Timer_Ctrl_Base+0x09)
#define		P_TimerB_Preload               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x0A)
#define		P_TimerB_CCP_Reg               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x0B)
#define		P_TimerB_CCP_UpCount           (volatile unsigned int*)(P_Timer_Ctrl_Base+0x0C)
#define		P_TimerC_Ctrl                  (volatile unsigned int*)(P_Timer_Ctrl_Base+0x10)
#define		P_TimerC_CCP_Ctrl              (volatile unsigned int*)(P_Timer_Ctrl_Base+0x11)
#define		P_TimerC_Preload               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x12)
#define		P_TimerC_CCP_Reg               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x13)
#define		P_TimerC_UpCount               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x14)
#define		P_TimerD_Ctrl                  (volatile unsigned int*)(P_Timer_Ctrl_Base+0x18)
#define		P_TimerD_Preload               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x1A)
#define		P_TimerD_UpCount               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x1C)
#define		P_TimerE_Ctrl                  (volatile unsigned int*)(P_Timer_Ctrl_Base+0x0100)
#define		P_TimerE_Preload               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x0102)
#define		P_TimerE_UpCount               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x0104)
#define		P_TimerF_Ctrl                  (volatile unsigned int*)(P_Timer_Ctrl_Base+0x0108)
#define		P_TimerF_Preload               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x010A)
#define		P_TimerF_UpCount               (volatile unsigned int*)(P_Timer_Ctrl_Base+0x010C)

////////////////////////////////////////
//  Audio Output Control Registers
//      Base Address Start from 0x78F0
////////////////////////////////////////
#define		P_CHA_Ctrl                     (volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x00)
#define		P_CHA_Data                     (volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x01)
#define		P_CHA_FIFO                     (volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x02)
#define		P_CHB_Ctrl                     (volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x08)
#define		P_CHB_Data                     (volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x09)
#define		P_CHB_FIFO                     (volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x0A)
#define		P_DAC_Ctrl						(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x0D)
#define		P_HPAMP_Ctrl					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x0E)
#define		P_DAC_IIS_Ctrl					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x0F)

#define		P_DAC_ACCREQ					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x300)
#define		P_DAC_ACCDINL					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x301)
#define		P_DAC_ACCDINH					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x302)
#define		P_DAC_Effect_Ctrl				(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x303)
#define		P_DAC_ACThresL					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x304)
#define		P_DAC_ACThresH					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x305)
#define		P_DAC_EQBand_Sele				(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x306)
#define		P_DAC_EQSpecrum					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x307)
#define		P_DAC_Volume3D					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x308)
#define		P_DAC_Volume3D_Cent				(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x309)
#define		P_DAC_Volume3D_Surround			(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x30A)
#define		P_DAC_Volume3D_R				(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x30B)
#define		P_DAC_Volume3D_L				(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x30C)
#define		P_DAC_ACCDOUTL					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x30E)
#define		P_DAC_ACCDOUTH					(volatile unsigned int*)(P_AudioOuput_Ctrl_Base+0x30F)

////////////////////////////////////////
//  UART / IrDA Control Registers
//      Base Address Start from 0x7900
////////////////////////////////////////
#define		P_UART_Data                	(volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x00)
#define		P_UART_RXStatus            	(volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x01)
#define		P_UART_Ctrl                	(volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x02)
#define		P_UART_BaudRate         	(volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x03)
#define		P_UART_Status    	        (volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x04)
#define		P_UART_FIFO                	(volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x05)
#define		P_UART_TXDelay				(volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x06)
#define		P_IrDA_BuadRate				(volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x07)
#define		P_IrDA_Ctrl					(volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x08)
#define		P_IrDA_LowPower				(volatile unsigned int*)(P_UARTIrDA_Ctrl_Base+0x09)

////////////////////////////////////////
//  RTC Control Regsiters
//      Base Address Start from 0x7920
////////////////////////////////////////
#define		P_Second                       (volatile unsigned int*)(P_RTC_Ctrl_Base+0x00)
#define		P_Minute                       (volatile unsigned int*)(P_RTC_Ctrl_Base+0x01)
#define		P_Hour                         (volatile unsigned int*)(P_RTC_Ctrl_Base+0x02)
#define		P_Alarm_Second                 (volatile unsigned int*)(P_RTC_Ctrl_Base+0x04)
#define		P_Alarm_Minute                 (volatile unsigned int*)(P_RTC_Ctrl_Base+0x05)
#define		P_Alarm_Hour                   (volatile unsigned int*)(P_RTC_Ctrl_Base+0x06)
#define		P_RTC_Ctrl                     (volatile unsigned int*)(P_RTC_Ctrl_Base+0x14)
#define		P_RTC_INT_Status               (volatile unsigned int*)(P_RTC_Ctrl_Base+0x15)
#define		P_RTC_INT_Ctrl                 (volatile unsigned int*)(P_RTC_Ctrl_Base+0x16)

////////////////////////////////////////
//  SPI Control Registers
//      Base Address Start from 0x7940
////////////////////////////////////////
#define		P_SPI_Ctrl                  	(volatile unsigned int*)(P_SPI_Ctrl_Base+0x00)
#define		P_SPI_TXStatus                  (volatile unsigned int*)(P_SPI_Ctrl_Base+0x01)
#define		P_SPI_TXData	                (volatile unsigned int*)(P_SPI_Ctrl_Base+0x02)
#define		P_SPI_RXStatus                  (volatile unsigned int*)(P_SPI_Ctrl_Base+0x03)
#define		P_SPI_RXData   	        		(volatile unsigned int*)(P_SPI_Ctrl_Base+0x04)
#define		P_SPI_Misc	   	     	   		(volatile unsigned int*)(P_SPI_Ctrl_Base+0x05)


////////////////////////////////////////
//  ADC Control Registers
//      Base Address Start from 0x7960
////////////////////////////////////////
#define		P_ADC_Setup				(volatile unsigned int*)(P_Analog_Ctrl_Base+0x00)
#define		P_MADC_Ctrl				(volatile unsigned int*)(P_Analog_Ctrl_Base+0x01)
#define		P_MADC_Data				(volatile unsigned int*)(P_Analog_Ctrl_Base+0x02)
#define		P_ASADC_Ctrl			(volatile unsigned int*)(P_Analog_Ctrl_Base+0x03)
#define		P_ASADC_Data			(volatile unsigned int*)(P_Analog_Ctrl_Base+0x04)
#define		P_TP_Ctrl				(volatile unsigned int*)(P_Analog_Ctrl_Base+0x05)

#define		P_HQADC_Ctrl			(volatile unsigned int*)(P_Analog_Ctrl_Base+0x10)
#define		P_HQADC_MIC_PGA			(volatile unsigned int*)(P_Analog_Ctrl_Base+0x11)
#define		P_HQADC_R_Gain			(volatile unsigned int*)(P_Analog_Ctrl_Base+0x12)
#define		P_HQADC_L_Gain			(volatile unsigned int*)(P_Analog_Ctrl_Base+0x13)

////////////////////////////////////////
//  LCD Control Registers
//      Base Address Start from 0x7980
////////////////////////////////////////
#define		P_LCD_Setup                    (volatile unsigned int*)(P_LCD_Ctrol_Base+0x00)
#define		P_LCD_Clock                    (volatile unsigned int*)(P_LCD_Ctrol_Base+0x01)
#define		P_LCD_Segment                  (volatile unsigned int*)(P_LCD_Ctrol_Base+0x02)
#define		P_LCD_Common                   (volatile unsigned int*)(P_LCD_Ctrol_Base+0x03)
#define		P_LCD_Buffer_LowAdr            (volatile unsigned int*)(P_LCD_Ctrol_Base+0x04)
#define		P_LCD_Buffer_HighAdr           (volatile unsigned int*)(P_LCD_Ctrol_Base+0x05)
#define		P_LCD_Buffer_Offset            (volatile unsigned int*)(P_LCD_Ctrol_Base+0x06)
#define		P_LCD_Timing_Ctrl              (volatile unsigned int*)(P_LCD_Ctrol_Base+0x07)
#define		P_LCD_Frame_Ctrl               (volatile unsigned int*)(P_LCD_Ctrol_Base+0x08)
#define		P_LCD_Palette_Ctrl             (volatile unsigned int*)(P_LCD_Ctrol_Base+0x09)
#define		P_LCD_Attri_Ctrl	      	   (volatile unsigned int*)(P_LCD_Ctrol_Base+0x0A)

#define		P_LCD_Palette    	      	   (volatile unsigned int*)(0x7A00)

////////////////////////////////////////
//  TFT Control Registers
//      Base Address Start from 0x7D00
////////////////////////////////////////
#define		P_TFT_Ctrl						(volatile unsigned int*)(P_TFT_Base+0x00)
#define		P_TFT_DCLK_Ctrl					(volatile unsigned int*)(P_TFT_Base+0x01)
#define		P_TFT_INT_Ctrl					(volatile unsigned int*)(P_TFT_Base+0x02)
#define		P_TFT_H_Width					(volatile unsigned int*)(P_TFT_Base+0x03)
#define		P_TFT_H_Start					(volatile unsigned int*)(P_TFT_Base+0x04)
#define		P_TFT_H_End						(volatile unsigned int*)(P_TFT_Base+0x05)
#define		P_TFT_HSync_Setup				(volatile unsigned int*)(P_TFT_Base+0x06)
#define		P_TFT_V_Width					(volatile unsigned int*)(P_TFT_Base+0x07)
#define		P_TFT_V_Start					(volatile unsigned int*)(P_TFT_Base+0x08)
#define		P_TFT_V_End						(volatile unsigned int*)(P_TFT_Base+0x09)
#define		P_TFT_VSync_Setup				(volatile unsigned int*)(P_TFT_Base+0x0A)
#define		P_TFT_RGB_Ctrl					(volatile unsigned int*)(P_TFT_Base+0x0B)
#define		P_TFT_YUV_Ctrl					(volatile unsigned int*)(P_TFT_Base+0x0C)
#define		P_TFT_DMAStart_AH				(volatile unsigned int*)(P_TFT_Base+0x0D)
#define		P_TFT_DMAStart_AL				(volatile unsigned int*)(P_TFT_Base+0x0E)
#define		P_TFT_DMA_Offset				(volatile unsigned int*)(P_TFT_Base+0x0F)
#define		P_TFT_Pixel_Num					(volatile unsigned int*)(P_TFT_Base+0x10)
#define		P_TFT_Line_Num					(volatile unsigned int*)(P_TFT_Base+0x11)		
#define		P_TFT_PIP0_Ctrl					(volatile unsigned int*)(P_TFT_Base+0x12)
#define		P_TFT_PIP0_VirFrame_SAH			(volatile unsigned int*)(P_TFT_Base+0x13)
#define		P_TFT_PIP0_VirFrame_SAL			(volatile unsigned int*)(P_TFT_Base+0x14)
#define		P_TFT_PIP0_VirFrame_EAH			(volatile unsigned int*)(P_TFT_Base+0x15)
#define		P_TFT_PIP0_VirFrame_EAL			(volatile unsigned int*)(P_TFT_Base+0x16)
#define		P_TFT_PIP0_STR_AH				(volatile unsigned int*)(P_TFT_Base+0x17)
#define		P_TFT_PIP0_STR_AL				(volatile unsigned int*)(P_TFT_Base+0x18)
#define		P_TFT_PIP0_H_Start				(volatile unsigned int*)(P_TFT_Base+0x19)
#define		P_TFT_PIP0_H_End				(volatile unsigned int*)(P_TFT_Base+0x1A)
#define		P_TFT_PIP0_V_Start				(volatile unsigned int*)(P_TFT_Base+0x1B)
#define		P_TFT_PIP0_V_End				(volatile unsigned int*)(P_TFT_Base+0x1C)

#define		P_TFT_PIP1_Ctrl					(volatile unsigned int*)(P_TFT_Base+0x1D)
#define		P_TFT_PIP1_VirFrame_SAH			(volatile unsigned int*)(P_TFT_Base+0x1E)
#define		P_TFT_PIP1_VirFrame_SAL			(volatile unsigned int*)(P_TFT_Base+0x1F)
#define		P_TFT_PIP1_VirFrame_EAH			(volatile unsigned int*)(P_TFT_Base+0x20)
#define		P_TFT_PIP1_VirFrame_EAL			(volatile unsigned int*)(P_TFT_Base+0x21)
#define		P_TFT_PIP1_STR_AH				(volatile unsigned int*)(P_TFT_Base+0x22)
#define		P_TFT_PIP1_STR_AL				(volatile unsigned int*)(P_TFT_Base+0x23)
#define		P_TFT_PIP1_H_Start				(volatile unsigned int*)(P_TFT_Base+0x24)
#define		P_TFT_PIP1_H_End				(volatile unsigned int*)(P_TFT_Base+0x25)
#define		P_TFT_PIP1_V_Start				(volatile unsigned int*)(P_TFT_Base+0x26)
#define		P_TFT_PIP1_V_End				(volatile unsigned int*)(P_TFT_Base+0x27)

#define		P_TFT_PIP2_Ctrl					(volatile unsigned int*)(P_TFT_Base+0x28)
#define		P_TFT_PIP2_VirFrame_SAH			(volatile unsigned int*)(P_TFT_Base+0x29)
#define		P_TFT_PIP2_VirFrame_SAL			(volatile unsigned int*)(P_TFT_Base+0x2A)
#define		P_TFT_PIP2_VirFrame_EAH			(volatile unsigned int*)(P_TFT_Base+0x2B)
#define		P_TFT_PIP2_VirFrame_EAL			(volatile unsigned int*)(P_TFT_Base+0x2C)
#define		P_TFT_PIP2_STR_AH				(volatile unsigned int*)(P_TFT_Base+0x2D)
#define		P_TFT_PIP2_STR_AL				(volatile unsigned int*)(P_TFT_Base+0x2E)
#define		P_TFT_PIP2_H_Start				(volatile unsigned int*)(P_TFT_Base+0x2F)
#define		P_TFT_PIP2_H_End				(volatile unsigned int*)(P_TFT_Base+0x30)
#define		P_TFT_PIP2_V_Start				(volatile unsigned int*)(P_TFT_Base+0x31)
#define		P_TFT_PIP2_V_End				(volatile unsigned int*)(P_TFT_Base+0x32)

#define		P_TFT_PIP3_Ctrl					(volatile unsigned int*)(P_TFT_Base+0x33)
#define		P_TFT_PIP3_VirFrame_SAH			(volatile unsigned int*)(P_TFT_Base+0x34)
#define		P_TFT_PIP3_VirFrame_SAL			(volatile unsigned int*)(P_TFT_Base+0x35)
#define		P_TFT_PIP3_VirFrame_EAH			(volatile unsigned int*)(P_TFT_Base+0x36)
#define		P_TFT_PIP3_VirFrame_EAL			(volatile unsigned int*)(P_TFT_Base+0x37)
#define		P_TFT_PIP3_STR_AH				(volatile unsigned int*)(P_TFT_Base+0x38)
#define		P_TFT_PIP3_STR_AL				(volatile unsigned int*)(P_TFT_Base+0x39)
#define		P_TFT_PIP3_H_Start				(volatile unsigned int*)(P_TFT_Base+0x3A)
#define		P_TFT_PIP3_H_End				(volatile unsigned int*)(P_TFT_Base+0x3B)
#define		P_TFT_PIP3_V_Start				(volatile unsigned int*)(P_TFT_Base+0x3C)
#define		P_TFT_PIP3_V_End				(volatile unsigned int*)(P_TFT_Base+0x3D)

////////////////////////////////////////
//  SD Control Registers
//      Base Address Start from 0x79D0
////////////////////////////////////////
#define		P_SD_DataTX				(volatile unsigned int*)(P_SD_Base+0x00)
#define		P_SD_DataRX				(volatile unsigned int*)(P_SD_Base+0x01)
#define		P_SD_CMD				(volatile unsigned int*)(P_SD_Base+0x02)
#define		P_SD_ArgL				(volatile unsigned int*)(P_SD_Base+0x03)
#define		P_SD_ArgH				(volatile unsigned int*)(P_SD_Base+0x04)
#define		P_SD_RespL				(volatile unsigned int*)(P_SD_Base+0x05)
#define		P_SD_RespH				(volatile unsigned int*)(P_SD_Base+0x06)
#define		P_SD_Status				(volatile unsigned int*)(P_SD_Base+0x07)
#define		P_SD_Ctrl				(volatile unsigned int*)(P_SD_Base+0x08)
#define		P_SD_BLKLEN				(volatile unsigned int*)(P_SD_Base+0x09)
#define		P_SD_INT				(volatile unsigned int*)(P_SD_Base+0x0A)
#define	SD_CommandComplete_INT		0x0001;
#define	SD_DataComplete_INT			0x0002;
#define	SD_CommandBufferFull_INT	0x0004;
#define	SD_DataBufferFull_INT		0x0008;
#define	SD_DataBufferEmpty_INT		0x0010;
#define	SD_CardInsert_INT			0x0020;
#define	SD_SDIOCard_INT				0x0040;



////////////////////////////////////////
//  DMA Control Registers
//      Base Address Start from 0x7b80
////////////////////////////////////////
#define		P_DMA_Ctrl0				(volatile unsigned int*)(P_DMA_Base+0x00)
#define		P_DMA_SRC_AddrL0		(volatile unsigned int*)(P_DMA_Base+0x01)
#define		P_DMA_TAR_AddrL0		(volatile unsigned int*)(P_DMA_Base+0x02)
#define		P_DMA_TCountL0			(volatile unsigned int*)(P_DMA_Base+0x03)
#define		P_DMA_SRC_AddrH0		(volatile unsigned int*)(P_DMA_Base+0x04)
#define		P_DMA_TAR_AddrH0		(volatile unsigned int*)(P_DMA_Base+0x05)
#define		P_DMA_TCountH0			(volatile unsigned int*)(P_DMA_Base+0x06)
#define		P_DMA_MISC0				(volatile unsigned int*)(P_DMA_Base+0x07)

#define		P_DMA_Ctrl1				(volatile unsigned int*)(P_DMA_Base+0x08)
#define		P_DMA_SRC_AddrL1		(volatile unsigned int*)(P_DMA_Base+0x09)
#define		P_DMA_TAR_AddrL1		(volatile unsigned int*)(P_DMA_Base+0x0A)
#define		P_DMA_TCountL1			(volatile unsigned int*)(P_DMA_Base+0x0B)
#define		P_DMA_SRC_AddrH1		(volatile unsigned int*)(P_DMA_Base+0x0C)
#define		P_DMA_TAR_AddrH1		(volatile unsigned int*)(P_DMA_Base+0x0D)
#define		P_DMA_TCountH1			(volatile unsigned int*)(P_DMA_Base+0x0E)
#define		P_DMA_MISC1				(volatile unsigned int*)(P_DMA_Base+0x0F)

#define		P_DMA_Ctrl2				(volatile unsigned int*)(P_DMA_Base+0x10)
#define		P_DMA_SRC_AddrL2		(volatile unsigned int*)(P_DMA_Base+0x11)
#define		P_DMA_TAR_AddrL2		(volatile unsigned int*)(P_DMA_Base+0x12)
#define		P_DMA_TCountL2			(volatile unsigned int*)(P_DMA_Base+0x13)
#define		P_DMA_SRC_AddrH2		(volatile unsigned int*)(P_DMA_Base+0x14)
#define		P_DMA_TAR_AddrH2		(volatile unsigned int*)(P_DMA_Base+0x15)
#define		P_DMA_TCountH2			(volatile unsigned int*)(P_DMA_Base+0x16)
#define		P_DMA_MISC2	    		(volatile unsigned int*)(P_DMA_Base+0x17)

#define		P_DMA_Ctrl3				(volatile unsigned int*)(P_DMA_Base+0x18)
#define		P_DMA_SRC_AddrL3		(volatile unsigned int*)(P_DMA_Base+0x19)
#define		P_DMA_TAR_AddrL3		(volatile unsigned int*)(P_DMA_Base+0x1A)
#define		P_DMA_TCountL3			(volatile unsigned int*)(P_DMA_Base+0x1B)
#define		P_DMA_SRC_AddrH3		(volatile unsigned int*)(P_DMA_Base+0x1C)
#define		P_DMA_TAR_AddrH3		(volatile unsigned int*)(P_DMA_Base+0x1D)
#define		P_DMA_TCountH3			(volatile unsigned int*)(P_DMA_Base+0x1E)
#define		P_DMA_MISC3	    		(volatile unsigned int*)(P_DMA_Base+0x1F)

#define		P_DMA_SPRISize0			(volatile unsigned int*)(P_DMA_Base+0x30)
#define		P_DMA_SPRISize1			(volatile unsigned int*)(P_DMA_Base+0x31)
#define		P_DMA_SPRISize2			(volatile unsigned int*)(P_DMA_Base+0x32)
#define		P_DMA_SPRISize3			(volatile unsigned int*)(P_DMA_Base+0x33)

#define		P_DMA_Transpar0			(volatile unsigned int*)(P_DMA_Base+0x38)
#define		P_DMA_Transpar1			(volatile unsigned int*)(P_DMA_Base+0x39)
#define		P_DMA_Transpar2			(volatile unsigned int*)(P_DMA_Base+0x3A)
#define		P_DMA_Transpar3			(volatile unsigned int*)(P_DMA_Base+0x3B)
#define		P_DMA_LineLenght		(volatile unsigned int*)(P_DMA_Base+0x3D)

#define		P_DMA_SS			(volatile unsigned int*)(P_DMA_Base+0x3E)
#define		P_DMA_INT			(volatile unsigned int*)(P_DMA_Base+0x3F)
#define		C_DMA0_ISRFlag		0x0001;
#define		C_DMA1_ISRFlag		0x0002;
#define		C_DMA2_ISRFlag		0x0004;
#define		C_DMA3_ISRFlag		0x0008;
#define		C_DMA0_BusyFlag		0x0100;
#define		C_DMA1_BusyFlag		0x0200;
#define		C_DMA2_BusyFlag		0x0400;
#define		C_DMA3_BusyFlag		0x0800;

////////////////////////////////////////
//  KeyScan Control Registers
//      Base Address Start from 0x7bC0
////////////////////////////////////////
#define		P_KS_Ctrl			(volatile unsigned int*)(P_KeyScan_Base+0x00)
#define		P_KS_Data0			(volatile unsigned int*)(P_KeyScan_Base+0x08)
#define		P_KS_Data1			(volatile unsigned int*)(P_KeyScan_Base+0x09)
#define		P_KS_Data2			(volatile unsigned int*)(P_KeyScan_Base+0x0A)	
#define		P_KS_Data3			(volatile unsigned int*)(P_KeyScan_Base+0x0B)
#define		P_KS_Data4			(volatile unsigned int*)(P_KeyScan_Base+0x0C)
#define		P_KS_Data5			(volatile unsigned int*)(P_KeyScan_Base+0x0D)
#define		P_KS_Data6			(volatile unsigned int*)(P_KeyScan_Base+0x0E)
#define		P_KS_Data7			(volatile unsigned int*)(P_KeyScan_Base+0x0F)




////////////////////////////////////////
//  USB Device Control Registers
//      Base Address Start from 0x7b30
////////////////////////////////////////
#define		P_USBD_Config			(volatile unsigned int*)(P_USBDevice_Base+0x00)
#define		P_USBD_Device			(volatile unsigned int*)(P_USBDevice_Base+0x27)
#define		P_USBD_Function			(volatile unsigned int*)(P_USBDevice_Base+0x01)
#define		P_USBD_DMAINT			(volatile unsigned int*)(P_USBDevice_Base+0x29)

#define		P_USBD_PMR				(volatile unsigned int*)(P_USBDevice_Base+0x02)
#define		P_USBD_EP0Data			(volatile unsigned int*)(P_USBDevice_Base+0x03)
#define		P_USBD_BIData			(volatile unsigned int*)(P_USBDevice_Base+0x04)
#define		P_USBD_BOData			(volatile unsigned int*)(P_USBDevice_Base+0x05)
#define		P_USBD_INTINData		(volatile unsigned int*)(P_USBDevice_Base+0x06)
#define		P_USBD_NullPkt			(volatile unsigned int*)(P_USBDevice_Base+0x28)

#define		P_USBD_EPEvent			(volatile unsigned int*)(P_USBDevice_Base+0x07)
#define		P_USBD_GLOINT			(volatile unsigned int*)(P_USBDevice_Base+0x08)
#define		P_USBD_INTEN			(volatile unsigned int*)(P_USBDevice_Base+0x09)
#define		P_USBD_INT				(volatile unsigned int*)(P_USBDevice_Base+0x0A)
#define		P_USBD_SCINTEM			(volatile unsigned int*)(P_USBDevice_Base+0x0B)
#define		P_USBD_SCINT			(volatile unsigned int*)(P_USBDevice_Base+0x0C)
#define		P_USBD_EPAutoSet		(volatile unsigned int*)(P_USBDevice_Base+0x0D)
#define		P_USBD_EPSetStall		(volatile unsigned int*)(P_USBDevice_Base+0x0E)
#define		P_USBD_EPBufClear		(volatile unsigned int*)(P_USBDevice_Base+0x0F)
#define		P_USBD_EPEvntClear		(volatile unsigned int*)(P_USBDevice_Base+0x10)
#define		P_USBD_EP0WrtCount		(volatile unsigned int*)(P_USBDevice_Base+0x11)
#define		P_USBD_BOWrtCount		(volatile unsigned int*)(P_USBDevice_Base+0x12)
#define		P_USBD_EP0BufPointer	(volatile unsigned int*)(P_USBDevice_Base+0x13)
#define		P_USBD_BBufRdPointer	(volatile unsigned int*)(P_USBDevice_Base+0x14)
#define		P_USBD_BBufWrtPointer	(volatile unsigned int*)(P_USBDevice_Base+0x15)
#define		P_USBD_EP0RTR			(volatile unsigned int*)(P_USBDevice_Base+0x16)
#define		P_USBD_EP0RR			(volatile unsigned int*)(P_USBDevice_Base+0x17)
#define		P_USBD_EP0VR			(volatile unsigned int*)(P_USBDevice_Base+0x18)
#define		P_USBD_EP0IR			(volatile unsigned int*)(P_USBDevice_Base+0x19)
#define		P_USBD_EP0LR			(volatile unsigned int*)(P_USBDevice_Base+0x1A)

#define		P_USBD_DMAWrtCountL		(volatile unsigned int*)(P_USBDevice_Base+0x20)
#define		P_USBD_DMAWrtCountH		(volatile unsigned int*)(P_USBDevice_Base+0x21)
#define		P_USBD_DMAAckL			(volatile unsigned int*)(P_USBDevice_Base+0x22)
#define		P_USBD_DMAAckH			(volatile unsigned int*)(P_USBDevice_Base+0x23)
#define		P_USBD_EPStall			(volatile unsigned int*)(P_USBDevice_Base+0x24)

////////////////////////////////////////
//  USB Host Control Registers
//      Base Address Start from 0x7b00
////////////////////////////////////////
#define		P_USBH_Config			(volatile unsigned int*)(P_USBHost_Base+0x00)
#define		P_USBH_TimeConfig		(volatile unsigned int*)(P_USBHost_Base+0x01)
#define		P_USBH_Data				(volatile unsigned int*)(P_USBHost_Base+0x02)
#define		P_USBH_Transfer			(volatile unsigned int*)(P_USBHost_Base+0x03)
#define		P_USBH_DveAddr			(volatile unsigned int*)(P_USBHost_Base+0x04)
#define		P_USBH_DveEp			(volatile unsigned int*)(P_USBHost_Base+0x05)
#define		P_USBH_TXCount			(volatile unsigned int*)(P_USBHost_Base+0x06)
#define		P_USBH_RXCount			(volatile unsigned int*)(P_USBHost_Base+0x07)
#define		P_USBH_FIFOInPointer	(volatile unsigned int*)(P_USBHost_Base+0x08)
#define		P_USBH_FIFOOutPointer	(volatile unsigned int*)(P_USBHost_Base+0x09)
#define		P_USBH_AutoInByteCount	(volatile unsigned int*)(P_USBHost_Base+0x0A)
#define		P_USBH_AutoOutByteCount	(volatile unsigned int*)(P_USBHost_Base+0x0B)
#define		P_USBH_AutoTrans		(volatile unsigned int*)(P_USBHost_Base+0x0C)
#define		P_USBH_Status			(volatile unsigned int*)(P_USBHost_Base+0x0D)
#define		P_USBH_INT				(volatile unsigned int*)(P_USBHost_Base+0x0E)
#define		P_USBH_INTEN			(volatile unsigned int*)(P_USBHost_Base+0x0F)
#define		P_USBH_StorageRST		(volatile unsigned int*)(P_USBHost_Base+0x10)
#define		P_USBH_SoftRST	 		(volatile unsigned int*)(P_USBHost_Base+0x11)
#define		P_USBH_SOFTimer			(volatile unsigned int*)(P_USBHost_Base+0x12)
#define		P_USBH_FrameNum			(volatile unsigned int*)(P_USBHost_Base+0x13)
#define		P_USBH_OTGConfig		(volatile unsigned int*)(P_USBHost_Base+0x14)
#define		P_USBH_VBusSet			(volatile unsigned int*)(P_USBHost_Base+0x15)
#define		P_USBH_VBusStatus		(volatile unsigned int*)(P_USBHost_Base+0x16)
#define		P_USBH_INAckCount		(volatile unsigned int*)(P_USBHost_Base+0x17)
#define		P_USBH_OutAckCount		(volatile unsigned int*)(P_USBHost_Base+0x18)
#define		P_USBH_RSTAckCount		(volatile unsigned int*)(P_USBHost_Base+0x19)
#define		P_USBH_Storage			(volatile unsigned int*)(P_USBHost_Base+0x1A)
#define		P_USBH_DReadBack		(volatile unsigned int*)(P_USBHost_Base+0x1B)



////////////////////////////////////////
//  I2C Control Registers
//      Base Address Start from 0x7b60
////////////////////////////////////////
#define		P_I2C_Ctrl				(volatile unsigned int*)(P_I2C_Base+0x00)
#define		P_I2C_Status			(volatile unsigned int*)(P_I2C_Base+0x01)
#define		P_I2C_Addr				(volatile unsigned int*)(P_I2C_Base+0x02)
#define		P_I2C_Data				(volatile unsigned int*)(P_I2C_Base+0x03)
#define		P_I2C_DeCLK				(volatile unsigned int*)(P_I2C_Base+0x04)
#define		P_I2C_En				(volatile unsigned int*)(P_I2C_Base+0x05)


/////////////////////////////////
//	miscellaneous control register
//	Base address is from 0x7BD0
/////////////////////////////////
#define		P_Byte_Swap				(volatile unsigned int*)(P_MISC_Base+0x00)
#define		P_Nibble_Swap			(volatile unsigned int*)(P_MISC_Base+0x01)
#define		P_TowBit_Swap			(volatile unsigned int*)(P_MISC_Base+0x02)
#define		P_Bit_Reverse			(volatile unsigned int*)(P_MISC_Base+0x03)


/////////////////////////////////
//	EFuse control register
//	Base address is from 0x7c30
/////////////////////////////////
#define		P_EFuse_D0				(volatile unsigned int*)(P_EFuse_Base+0x00)
#define		P_EFuse_D1				(volatile unsigned int*)(P_EFuse_Base+0x01)
#define		P_EFuse_D2				(volatile unsigned int*)(P_EFuse_Base+0x02)
#define		P_EFuse_D3				(volatile unsigned int*)(P_EFuse_Base+0x03)


//========================================================================================
// End of include file
//========================================================================================


#endif
