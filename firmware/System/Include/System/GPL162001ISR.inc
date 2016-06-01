.ifndef	__SPL162001ISR_INC__
.define	__SPL162001ISR_INC__

.define		INT_EXTA				0
.define		INT_EXTB				1
.define     INT_DMA                 2
.define     INT_USB                 3
.define     INT_AUDIOA              4
.define     INT_AUDIOB              5
.define     INT_SPBUS               6
.define     INT_ADC_AUTO            7
.define     INT_TOUCHPANEL          8
.define     INT_LCD_FP              9
.define     INT_SPI                 10
.define     INT_UART_IRDA           11
.define		INT_CMOS_HALFFULL   	12
.define		INT_CMOS_FULL   	    13
.define		INT_ADC_READY			14
.define		INT_KEY					15
.define     INT_HMS                 16
.define     INT_ALARM               17
.define     INT_SCHED_INT           18
.define     INT_SHUTDOWNNB          19
.define     INT_NANDFLASH           20
.define     INT_I2C                 21
.define     INT_SD                  22
.define     INT_LVD                 23
.define     INT_TIMER_BASEA         24
.define     INT_TIMER_BASEB         25
.define     INT_TIMER_BASEC         26
.define     INT_KEYSCAN             27
.define		INT_TIMERA				28
.define		INT_TIMERB				29
.define		INT_TIMERC				30
.define		INT_TIMERD				31


.define		INT_EXTA_MASK			    1<<0
.define		INT_EXTB_MASK				1<<1
.define     INT_DMA_MASK                1<<2
.define     INT_USB_MASK                1<<3
.define     INT_AUDIOA_MASK             1<<4
.define     INT_AUDIOB_MASK             1<<5
.define     INT_SPBUS_MASK              1<<6
.define     INT_ADC_AUTO_MASK           1<<7
.define     INT_TOUCHPANEL_MASK         1<<8
.define     INT_LCD_FP_MASK             1<<9
.define     INT_SPI_MASK                1<<10
.define     INT_UART_IRDA_MASK          1<<11
.define		INT_CMOS_HALFFULL_MASK   	1<<12
.define		INT_CMOS_FULL_MASK   	    1<<13
.define		INT_ADC_READY_MASK			1<<14
.define		INT_KEY_MASK				1<<15
.define     INT_HMS_MASK                1<<(16-16)
.define     INT_ALARM_MASK              1<<(17-16)
.define     INT_SCHED_INT_MASK          1<<(18-16)
.define     INT_SHUTDOWNNB_MASK         1<<(19-16)
.define     INT_NANDFLASH_MASK          1<<(20-16)
.define     INT_I2C_MASK                1<<(21-16)
.define     INT_SD_MASK                 1<<(22-16)
.define     INT_LVD_MASK                1<<(23-16)
.define     INT_TIMER_BASEA_MASK        1<<(24-16)
.define     INT_TIMER_BASEB_MASK        1<<(25-16)
.define     INT_TIMER_BASEC_MASK        1<<(26-16)
.define     INT_KEYSCAN_MASK            1<<(27-16)
.define		INT_TIMERA_MASK				1<<(28-16)
.define		INT_TIMERB_MASK				1<<(29-16)
.define		INT_TIMERC_MASK				1<<(30-16)
.define		INT_TIMERD_MASK				1<<(31-16)


.endif
