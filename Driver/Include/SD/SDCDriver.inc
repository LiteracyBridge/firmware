
.define	C_SDC_BlkLen_512Bytes	0x0200
.define C_SDC_ClkSpeed			0x0020//test
//.define C_SDC_ClkSpeed_0		0x0000
.define C_SDC_ClkSpeed_0		0x0000
.define C_SDC_ClkSpeed_1		0x0002   //sunxw added for MMC
.define	C_SDC_DataBus_1bit		0x0000
.define	C_SDC_DataBus_4bit		0x0100
.define	C_DMA_Mode_Disable		0x0000
.define	C_SDC_INT_Disable		0x0000
.define	C_SDC_Enable			0x0800
.define	C_SDC_CMD_Run			0x0080
.define C_SDC_CMD_With_Data		0x0100
.define C_SDC_TranData			0x0200
.define C_SDC_MulBlk_Enable		0x0400

//Command Set
.define C_SDC_CMD0				0x0000
.define C_MMC_CMD1				0x0001
.define C_SDC_CMD2				0x0002
.define C_SDC_CMD3				0x0003
.define C_SDC_CMD7				0x0007
.define C_SDC_CMD8              0x0008
.define C_SDC_CMD9				0x0009
.define C_SDC_CMD12				0x000c
.define C_SDC_CMD13				0x000d
.define C_SDC_CMD17				0x0011
.define C_SDC_CMD18				0x0012
.define C_SDC_CMD24				0x0018
.define C_SDC_CMD25				0x0019
.define C_SDC_CMD55				0x0037

.define C_SDC_ACMD6				0x0006
.define C_SDC_ACMD13			0x000d
.define C_SDC_ACMD41			0x0029


// Status
.define	C_Status_Busy		0x0001
.define	C_Status_CardBusy	0x0002
.define	C_Status_CMDCOM		0x0004
.define	C_Status_DataCOM	0x0008
.define	C_Status_RSPINDXERR	0x0010
.define	C_Status_RSPCRCERR	0x0020
.define	C_Status_RSPRegFull	0x0040
.define	C_Status_DataFull	0x0080
.define	C_Ststus_DataEmpty	0x0100

// Response Type
.define C_Response_0		0x0000
.define C_Response_R1		0x1000
.define C_Response_R2		0x2000
.define C_Response_R3		0x3000
.define C_Response_R6		0x6000
.define C_Response_R1b		0x7000