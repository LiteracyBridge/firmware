.ifndef	USB_Inc
.define USB_Inc

.public	USB_RW_Buffer
.public	_USB_Status
.public _Bulk_Stall
.public _R_SDC_Present
.public _R_USB_Suspend
.public _R_USB_State_Machine
.public _R_NAND_Present
.public _NandTotalSector
.public _SDC_AB_Buffer_Ctrl
//.public _R_USB_RW_Flag
.public _R_EP0DataCount
.public R_Descriptor_High
.public R_Descriptor_Low
.public _CSW_Residue
.public _CSW_Status
.public _Sense_Code
.public _NandIDBuffer
.public	_R_CBW_PKT
.public _Nandtype
.public _Command_Value
.public _CBWLUN
.public _Command_SecValue
.public _SCSI_Direct_Write
.public _ulTimeout
.public _unGotoRamStart
.public _MaxLUN
.public _Max_LUN
.public _USBLunStatus
.public _ptr_USB_LUN_Info
.public _USBWriteProtect
.public _CDROMEnable
.public _CDROMDelay
.public _R_USB_RW_Busy
.public _USBD_PlugIn
.public _tra_end


///////////////////////////////////ASM Export Table///////////////////////////////////////////
.external 	_SCSISENSE
.external	_Device_Descriptor
.external	_Config_Descriptor
.external	_String0_Descriptor
.external	_String1_Descriptor
.external	_String2_Descriptor
.external	_String3_Descriptor
.external	_String4_Descriptor
.external	_DeviceVersionInfo
.external   _SCSIInquiryData
.external   _SCSIInquiryData_End
.external   _SCSIInquiryData_CDROM
.external   _SCSIInquiryData_CDROM_End
.external   _SCSIInquiryData_CardReader
.external   _SCSIInquiryData_CardReader_End
.external   _ScsiModeSense10CDROM
.external   _ScsiModeSense10CDROM_End
.external  _Nand_Part0_Size
.external  _Nand_Part1_Size
.external  _Nand_Part2_Size
.external  _ScsiFlexPage
.external  _ScsiFlexPage_End
///////////////////////////////////ASM Export Function///////////////////////////////////////////
//.external _CheckCDRomValid

////////////////////////////////Global Valuable/////////////////////////////////////////////////////
USB_RAM_Section:	.section	.ORAM//,	.addr=0x40000//.addr=0x6C00
USB_RW_Buffer:
	.dw		512		dup(?)

_CSW_Residue:		.dw	2 dup(?)	
_NandIDBuffer:		.dw 3 dup(?)
_R_CBW_PKT:			.dw 31 dup(?)
_NandTotalSector:	.dw	2 dup(?)	
_unGotoRamStart:	.dw 1 dup(?)
_ulTimeout: 		.dw 2 dup(?)
_USBLunStatus: 		.dw 5 dup(?)	//lun status, Okay or MediaChange
_ptr_USB_LUN_Info:	.dw 2 dup(?)

////////////////////////
	.var    _tra_end
	.var	_USB_Status
	.var	_Bulk_Stall
	.var	_R_SDC_Present
	.var	_R_USB_Suspend
	.var	_R_USB_State_Machine
	.var	_R_NAND_Present
	.var	_SDC_AB_Buffer_Ctrl
	.var	_R_USB_RW_Busy			//If device is read/write, this flag will be set 1. Otherwise set 0.
	.var	_R_EP0DataCount
	.var	_CSW_Status
	.var	_Sense_Code	
	.var	_Nandtype
	.var	_Command_Value
	.var	_CBWLUN					
	.var	_Command_SecValue
	.var	_SCSI_Direct_Write
	.var	R_Descriptor_High
	.var	R_Descriptor_Low
	.var	_USBWriteProtect
	.var	_CDROMEnable
	.var 	_CDROMDelay
	.var	_USBD_PlugIn			//0:Device not connect to PC, 1:Device connect to PC
_Max_LUN:
	.var	_MaxLUN					//Lun number, 0=1 lun, 1=2 luns.



.endif	
/////////////////////////