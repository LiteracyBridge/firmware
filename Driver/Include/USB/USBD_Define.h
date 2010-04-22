//**********************************************************
//	SPL162001 MSDC Driver user define table
//	Edit: Anson Chuang
//**********************************************************

//#define print

/////////////////////////str_USB_Lun_Info->unLunType//////////////////////////////////
#define LunType_NandDrive			0
#define LunType_CDROM				1
#define LunType_SDCardReader		2
////////////////////////str_USB_Lun_Info->unStatus//////////////////////////////////
#define LunStatus_Normal			0x0000
#define LunStatus_NoMedia			0x0001
#define LunStatus_WriteProtect		0x0002
#define LunStatus_MediaChange		0x0004


//----------------------------------------------------------
// Constant Definitions
//----------------------------------------------------------

#define		BIT0		0x0001
#define		BIT1		0x0002
#define		BIT2		0x0004
#define		BIT3		0x0008
#define		BIT4		0x0010
#define		BIT5		0x0020
#define		BIT6		0x0040
#define		BIT7		0x0008
#define		BIT8		0x0100
#define		BIT9		0x0200
#define		BIT10		0x0400
#define		BIT11		0x0800
#define		BIT12		0x1000
#define		BIT13		0x2000
#define		BIT14		0x4000
#define		BIT15		0x8000




#define		EP0_Setup_PKT		BIT0
#define		EP0_OUT_PKT			BIT1
#define		EP0_OUT_NACK		BIT2
#define		EP0_IN_PKT_Clear	BIT3
#define		EP0_IN_NACK			BIT4
#define		EP0_Status_Clear	BIT5
#define		EP0_Status_NACK		BIT6
#define		BULK_IN_PKT_Clear	BIT7
#define		BULK_IN_NACK		BIT8
#define		BULK_OUT_PKT_Set	BIT9
#define		BULK_OUT_NACK		BIT10
#define		INT_IN_PKT_Clear	BIT11
#define		INT_IN_NACK			BIT12
#define		Suspend				BIT13
#define		Resume				BIT14
#define		USB_RESET			BIT15

#define		EP0_SETUP_Stage		0x0001
#define		EP0_Data_Stage		0x0002
#define		EP0_Status_Stage	0x0003
#define		SCSI_CBW_Stage		0x0004
#define		SCSI_Data_Stage		0x0005
#define		SCSI_CSW_Stage		0x0006


#define		Device_Descriptor_Table		0x0001
#define		Config_Descriptor_Table		0x0002
#define		String0_Descriptor_Table	0x0003
#define		String1_Descriptor_Table	0x0004
#define		String2_Descriptor_Table	0x0005
#define		String3_Descriptor_Table	0x0006
#define		String4_Descriptor_Table	0x0007
#define		Interface_Descriptor_Table	0x0008
#define		Endpoint_Descriptor_Table	0x0009
#define		MaxLUN_Table				0x000A

#define		Bulk_IN_Stall		0x0001
#define		Bulk_OUT_Stall		0x0002

//********************************************
// used for USB Read10/Write10 A/B Buffer
#define		C_Buffer_A_Empty				0x0001
#define		C_Buffer_A_Full					0x0002
#define		C_Buffer_B_Empty				0x0004
#define		C_Buffer_B_Full					0x0008
#define		C_SDC_Access_A					0x0010
#define		C_SDC_Access_B					0x0020
#define		C_USB_Access_A					0x0040
#define		C_USB_Access_B					0x0080
#define		C_AB_Buffer_1					0x0100
#define		C_AB_Buffer_2					0x0200
#define		C_AB_Buffer_3					0x0400
#define		C_AB_Buffer_4					0x0800
//========	USB Main State Define	========//


