.ifndef USB_Table_Inc
.define USB_Table_Inc

///////////////////////////////////////Define///////////////////////////////////////////////////////
.define LunType_NandDrive			0
.define LunType_CDROM				1
.define LunType_SDCardReader		2





.define		C_Device_Descriptor_Table		0x0001
.define		C_Config_Descriptor_Table		0x0002
.define		C_String0_Descriptor_Table		0x0003
.define		C_String1_Descriptor_Table		0x0004
.define		C_String2_Descriptor_Table		0x0005
.define		C_String3_Descriptor_Table		0x0006
.define		C_String4_Descriptor_Table		0x0007
.define		C_Interface_Descriptor_Table	0x0008
.define		C_Endpoint_Descriptor_Table		0x0009
.define		C_MaxLUN_Table					0x000A

.define		C_Buffer_A_Empty				0x0001
.define		C_Buffer_A_Full					0x0002
.define		C_Buffer_B_Empty				0x0004
.define		C_Buffer_B_Full					0x0008
.define		C_SDC_Access_A					0x0010
.define		C_SDC_Access_B					0x0020
.define		C_USB_Access_A					0x0040
.define		C_USB_Access_B					0x0080
.define		C_AB_Buffer_1					0x0100
.define		C_AB_Buffer_2					0x0200
.define		C_AB_Buffer_3					0x0400
.define		C_AB_Buffer_4					0x0800

.endif