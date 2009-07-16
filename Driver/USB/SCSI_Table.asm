
.public _SCSIVendorInformation
.public _SCSIProductIdentification
.public _ScsiProductRevisionLevel
.public _SCSISENSE
.public _SCSIInquiryData
.public _SCSIInquiryData_End
.public _SCSIInquiryData_CDROM
.public _SCSIInquiryData_CDROM_End
.public _SCSIInquiryData_CardReader
.public _SCSIInquiryData_CardReader_End
.public _ScsiModeSense10CDROM
.public _ScsiModeSense10CDROM_End

.public	_Device_Descriptor
.public	_Config_Descriptor
.public	_String0_Descriptor
.public	_String1_Descriptor
.public	_String2_Descriptor
.public	_String3_Descriptor
.public	_String4_Descriptor
.comment @
.ifndef USBRP
.public _DeviceVersionInfo
.endif
@
.public _ScsiFlexPage
.public _ScsiFlexPage_End
	
SCSI_Table_Setion:	.section	.data
//======================================6================================//
//							Device Descriptor							//
//======================================================================//

_Device_Descriptor:
	.dw		_Device_Descriptor_End-_Device_Descriptor		//bLength
	.dw		0x01					//bDescriptorType	: Device
	.dw		0x10, 0x01				//bcdUSB			: version 1.10
	.dw		0x00					//bDeviceClass
	.dw		0x00					//bDeviceSubClass
	.dw		0x00					//bDeviceProtocol
	.dw		0x08					//bMaxPacketSize0
	.dw		0x3F, 0x1B				//idVendor
	.dw		0x02, 0x20				//idProduct
	.dw		0x00, 0x01				//bcdDevice
	.dw		0x01					//iManufacturer
	.dw		0x02					//iProduct
	.dw		0x03					//iSerialNumber
	.dw		0x01					//bNumConfigurations
	
_Device_Descriptor_End:


//======================================================================//
//							Cinfig Descriptor							//
//======================================================================//
_Config_Descriptor:
//Configuration (0x09 byte)
    .dw    _Config_Descriptor_End-_Config_Descriptor                   //bLength: 0x09 byte
    .dw    0x02                   //bDescriptorType: CONFIGURATION
    .dw    _Config_Descriptor_Total-_Config_Descriptor             //wTotalLength:
    .dw	   0x00
    .dw    0x01                   //bNumInterfaces: 1 interfaces
    .dw    0x01                   //bConfigurationValue: configuration 1
    .dw    0x00                   //iConfiguration: index of string
    .dw    0x80                   //bmAttributes: bus powered, Not Support Remote-Wakeup
    .dw    0x32                   //MaxPower: 100 mA
_Config_Descriptor_End:
        //-------------------------------------------------------------------
        //SubLength = 0x001e byte
        //-------------------------------------------------------------------

_Interface_Descriptor:
        //Interface 1 (0x09 byte)
     .dw   0x09                   //bLength: 0x09 byte
     .dw   0x04                   //bDescriptorType: INTERFACE
     .dw   0x00                   //bInterfaceNumber: interface 0
     .dw   0x00                   //bAlternateSetting: alternate setting 0
     .dw   0x03                   //bNumEndpoints: 3 endpoints(EP1,EP2,EP3)
     .dw   0x08                   //bInterfaceClass: Mass Storage Devices Class
     .dw   0x06                   //bInterfaceSubClass:
     .dw   0x50                   //bInterfaceProtocol
     .dw   0x00                   //iInterface: index of string
_Interface_Descriptor_End:

_Endpoint1:
        //Endpoint 1 (0x07 byte)
     .dw   0x07                   //bLength: 0x07 byte
     .dw   0x05                   //bDescriptorType: ENDPOINT
     .dw   0x81                   //bEndpointAddress: IN endpoint 1 --Chamber For PA8591
     .dw   0x02                   //bmAttributes: Bulk
     .dw   0x40, 0x00             //wMaxPacketSize: 64 byte
     .dw   0x00                   //bInterval: ignored

_Endpoint2:
        //Endpoint 2 (0x07 byte)
     .dw   0x07                   //bLength: 0x07 byte
     .dw   0x05                   //bDescriptorType: ENDPOINT
     .dw   0x02                   //bEndpointAddress: OUT endpoint 2 -- Chamber For PA8591
     .dw   0x02                   //bmAttributes: Bulk
     .dw   0x40, 0x00             //wMaxPacketSize: 64 byte
     .dw   0x00                   //bInterval: ignored

_Endpoint3:
        //Endpoint 3 (0x07 byte)
     .dw   0x07                   //bLength: 0x07 byte
     .dw   0x05                   //bDescriptorType: ENDPOINT
     .dw   0x83                   //bEndpointAddress: IN endpoint 3 -- Chamber For PA8591
     .dw   0x03                   //bmAttributes: Interrupt
     .dw   0x02, 0x00             //wMaxPacketSize: 2 byte
     .dw   0x01                   //bInterval: polling interval is 1 ms
_Config_Descriptor_Total:


//======================================================================//
//							String Descriptor							//
//======================================================================//
_String0_Descriptor:
	.dw   0x04		//bLength
	.dw	  0x03		//bDescriptorType
	.dw   0x09, 0x04	//bString
_String0_Descriptor_End:


_String1_Descriptor:
	.dw   _String1_Descriptor_End-_String1_Descriptor		//bLength
	.dw   0x03		//bDescriptorType
	.dw   'G', 0x00	//bString
	.dw   'E', 0x00
	.dw   'N', 0x00
	.dw   'E', 0x00
	.dw   'R', 0x00
	.dw   'A', 0x00
	.dw   'L', 0x00
	.dw   ' ', 0x00
//	.dw   't', 0x00
//	.dw   'e', 0x00
//	.dw   'c', 0x00
//	.dw   'h', 0x00
_String1_Descriptor_End:


_String2_Descriptor:
	.dw   _String2_Descriptor_End-_String2_Descriptor		//bLength
	.dw   0x03		//bDescriptorType
	.dw   'G', 0x00	//bString
	.dw   'E', 0x00
	.dw   'N', 0x00
	.dw   'E', 0x00
	.dw   'R', 0x00
	.dw   'A', 0x00
	.dw   'L', 0x00
	.dw   'P', 0x00
	.dw   'L', 0x00
	.dw   'U', 0x00
	.dw   'S', 0x00
	.dw   '-', 0x00
	.dw   'M', 0x00
	.dw   'S', 0x00
	.dw   'D', 0x00
	.dw   'C', 0x00
_String2_Descriptor_End:

_String3_Descriptor:
	.dw   _String3_Descriptor_End-_String3_Descriptor		//bLength
	.dw   0x03		//bDescriptorType
	.dw   '1', 0x00	//bString
	.dw   '2', 0x00
	.dw   '3', 0x00
	.dw   '4', 0x00
	.dw   '5', 0x00
	.dw   '6', 0x00
	.dw   '7', 0x00
	.dw   '8', 0x00
	.dw   '9', 0x00
	.dw   'A', 0x00
	.dw   'B', 0x00
	.dw   'C', 0x00
	.dw   'D', 0x00
	.dw   'E', 0x00
	.dw   'F', 0x00
	.dw   'G', 0x00	
_String3_Descriptor_End:

_String4_Descriptor:
	.dw   _String4_Descriptor_End-_String4_Descriptor		//bLength
	.dw   0x03		//bDescriptorType
	.dw   '1', 0x00	//bString
	.dw   '.', 0x00
	.dw   '0', 0x00
	.dw   '.', 0x00
	.dw   '0', 0x00
_String4_Descriptor_End:


_SCSIInquiryData:
	.dw		0x00
	.dw		0x80
	.dw		0x00
	.dw		0x00
	.dw		0x1F
	.dw		0x00
	.dw		0x00
	.dw		0x00
	.dw		'G'
	.dw		'E'
	.dw		'N'
	.dw		'P'
	.dw		'L'
	.dw		'U'
	.dw		'S'
	.dw		0x20
	.dw		'U'
	.dw		'S'
	.dw		'B'
	.dw		'-'
	.dw		'M'
	.dw		'S'
	.dw		'D'
	.dw		'C'
	.dw		' '
	.dw		'D'
	.dw		'I'
	.dw		'S'
	.dw		'K'
	.dw		' '
	.dw		'A'
	.dw		0x20
	.dw		'1'
	.dw		'.'
	.dw		'0'
	.dw		'0'
///////Do not modify start////////
	.dw		'G'
	.dw		'P'
	.dw		'-'
	.dw		'P'
	.dw		'R'
	.dw		'O'
	.dw		'D'
	.dw		'.'
///////Do not modify end////////	
_SCSIInquiryData_End:

_CDimageSignature:
	.dw 'G', 'E', 'N', 'P', 'L', 'U', 'S'


_SCSIInquiryData_CDROM:
	.dw 	0x05												//direct access device (CDROM)
	.dw 	0x80												//b7=1 RMB, Removable Media Bit
	.dw 	0x00												//ISO, ECMA, ANSI version
 	.dw 	0x21 												//response data format
	.dw 	0x1F												//additional length
	.dw 	0x00, 0x00, 0x00									//reserved
	.dw 	'G','E','N','E','R','A','L',' '						//vendor info.
	.dw 	'P','L','U','S',' ','C','D','R'						//Product ID
	.dw 	'O','M',' ',' ',' ',' ',' ',' '
	.dw 	'1', '.', '0', '0'									//n.nn Product Revision Level
///////Do not modify start////////
	.dw		'G'
	.dw		'P'
	.dw		'-'
	.dw		'P'
	.dw		'R'
	.dw		'O'
	.dw		'D'
	.dw		'.'
///////Do not modify end////////	
_SCSIInquiryData_CDROM_End:

_ScsiModeSense10CDROM:
	.dw		0x00 											//Density Code
	.dw 	0x20, 0x70, 0x00								//Number of Blocks MSB~LSB 
	.dw  	0x00											//Reserved
	.dw 	0x00, 0x00, 0x00								//Block Length MSB~LSB
	.dw 	0x2A											//PS(0)(1bit),Reserved(1bit),Page Code(2A)
	.dw 	0x12											//Page Length
	.dw 	0x07											//Reserved(5bits),Method2(1bit),CD-RW Read(1bit),CD-R Read(1bit)
	.dw 	0x00											//Reserved(5bits),Test Write(1bit),CD-RW Write(1bit),CD-R Write(1bit)
	.dw 	0x71											//Reserved,MultiSession,Mode2Form2,Mode2Form1,DigitalPort(2),DigitalPort(1),Composite,AudioPlay
	.dw 	0x63											//Read BarCode,UPC,ISRC,C2Pointers are Supported,R-W Deinterleaved&supported,R-W Supported,CD-DA Stream is Accurate,CD-DA Commands Supported
	.dw 	0x29											//Loading Mechanism Type(3bits),Reserved,Eject,Prevent Jumper,Lock State,Lock	
	.dw 	0x03											//Reserved(4bits),S/W SlotSelection(SSS),changer SupportsDisc Present Reporting,Separate Channel Mute Supported,Separate volume levels per channel
	.dw 	0x40, 0x00										//Maximum Read Speed Supported(in kBps)MSB~LSB 
	.dw 	0x00, 0xFF										//Number of Volume Levels Supported MSB~LSB
	.dw 	0x00, 0x00										//Buffer Size supported by Drive(in Kbytes) MSB~LSB
	.dw 	0x40, 0x00										//Current Read Speed Selected(in Kbytes) MSB~LSB
	.dw 	0x00, 0x00, 0x00, 0x00
_ScsiModeSense10CDROM_End:

_SCSIInquiryData_CardReader:
	.dw		0x00
	.dw		0x80
	.dw		0x00
	.dw		0x00
	.dw		0x1F
	.dw		0x00
	.dw		0x00
	.dw		0x00
	.dw		'G'
	.dw		'E'
	.dw		'N'
	.dw		'P'
	.dw		'L'
	.dw		'U'
	.dw		'S'
	.dw		0x20
	.dw		'U'
	.dw		'S'
	.dw		'B'
	.dw		'-'
	.dw		'M'
	.dw		'S'
	.dw		'D'
	.dw		'C'
	.dw		' '
	.dw		'D'
	.dw		'I'
	.dw		'S'
	.dw		'K'
	.dw		' '
	.dw		'A'
	.dw		0x20
	.dw		'1'
	.dw		'.'
	.dw		'0'
	.dw		'0'
///////Do not modify start////////
	.dw		'G'
	.dw		'P'
	.dw		'-'
	.dw		'P'
	.dw		'R'
	.dw		'O'
	.dw		'D'
	.dw		'.'
///////Do not modify end////////	
_SCSIInquiryData_CardReader_End:
	
	
_SCSIVendorInformation:
	.dw		'S'
	.dw		'u'
	.dw		'n'
	.dw		'p'
	.dw		'l'
	.dw		'u'
	.dw		's'
	.dw		0x20
_SCSIVendorInformation_End:

_SCSIProductIdentification:
	.dw		'U'
	.dw		'S'
	.dw		'B'
	.dw		'-'
	.dw		'M'
	.dw		'S'
	.dw		'D'
	.dw		'C'
	.dw		' '
	.dw		'D'
	.dw		'I'
	.dw		'S'
	.dw		'K'
	.dw		' '
	.dw		'A'
	.dw		0x20
_SCSIProductIdentification_End:

_ScsiProductRevisionLevel:
	.dw		'1'
	.dw		'.'
	.dw		'0'
	.dw		'0'
	.dw		'G'
	.dw		'P'
	.dw		'1'
	.dw		'6'
	.dw		'2'
	.dw		'0'
	.dw		'0'
	.dw		'2'
_ScsiProductRevisionLevel_End:

_ScsiFlexPage:
	.dw		0x05													
	.dw     0x1E													
	.dw     0x13, 0x88												
 	.dw     0x00 													
	.dw     0x00													
	.dw     0x02, 0x00												
	.dw     0x00, 0x00												
	.dw     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	
	.dw     0x05													
	.dw     0x1E													
	.dw     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00				
	.dw     0x01, 0x68												
	.dw     0x00, 0x00												
_ScsiFlexPage_End:
////////////////Table//////////////////////////////////////////////////////
.comment @
.ifndef USBRP
_DeviceVersionInfo:
	.dw		0x00	//0~3: IC body
	.dw		0x16
	.dw		0x20
	.dw		0x02
	.dw		0xFF	//4~7: IC Version, 0xFFFFFF means mask rom
	.dw		0x00
	.dw		0x00
	.dw		0x00
	.dw		0x09	//Year
	.dw		0x05	//Month
	.dw		0x10	//Day
_DeviceVersionInfo_End:
.endif
@

_SCSISENSE:
        //  SK   ASC  ASCQ
	.dw     0x00, 0x00, 0x00    //0x00 - NO SENSE
    .dw     0x01, 0x17, 0x01    //0x01 - RECOVERD DATA WITH RETRIES
    .dw     0x01, 0x18, 0x00    //0x02 - RECOVERD DATA WITH ECC
    .dw     0x02, 0x04, 0x01    //0x03 - LOGICAL DRIVE NOT READY - BECOMING READY
    .dw     0x02, 0x04, 0x02    //0x04 - LOGICAL DRIVE NOT READY - INITIALIZATION REQUIRED
    .dw     0x02, 0x04, 0x04    //0x05 - LOGICAL UNIT NOT READY - FORMAT IN PROGRESS
    .dw     0x02, 0x04, 0xFF    //0x06 - LOGICAL DRIVE NOT READY - DEVICE IS BUSY
    .dw     0x02, 0x06, 0x00    //0x07 - NO REFERENCE POSITION FOUND
    .dw     0x02, 0x08, 0x00    //0x08 - LOGICAL UNIT COMMUNICATION FAILURE
    .dw     0x02, 0x08, 0x01    //0x09 - LOGICAL UNIT COMMUNICATION TIME-OUT
    .dw     0x02, 0x08, 0x80    //0x0A - LOGICAL UNIT COMMUNICATION OVERRUN
    .dw     0x02, 0x3A, 0x00    //0x0B - MEDIUM NOT PRESENT
    .dw     0x02, 0x54, 0x00    //0x0C - USB TO HOST SYSTEM INTERFACE FAILURE
    .dw     0x02, 0x80, 0x00    //0x0D - INSUFFICIENT RESOURCES
    .dw     0x02, 0xFF, 0xFF    //0x0E - UNKNOWN ERROR
    .dw     0x03, 0x02, 0x00    //0x0F - NO SEEK COMPLETE
    .dw     0x03, 0x03, 0x00    //0x10 - WRITE FAULT
    .dw     0x03, 0x10, 0x00    //0x11 - ID CRC ERROR
    .dw     0x03, 0x11, 0x00    //0x12 - UNRECOVERD READ ERROR
    .dw     0x03, 0x12, 0x00    //0x13 - ADDRESS MARK NOT FOUND FOR ID FIELD
    .dw     0x03, 0x13, 0x00    //0x14 - ADDRESS MARK NOT FOUND FOR DATA FIELD
    .dw     0x03, 0x14, 0x00    //0x15 - RECORDED ENTITY NOT FOUND
    .dw     0x03, 0x30, 0x01    //0x16 - CANNOT READ MEDIUM - UNKNOWN FORMAT
    .dw     0x03, 0x31, 0x01    //0x17 - FORMAT COMMAND FAILED
    .dw     0x04, 0x40, 0x80    //0x18 - DIAGNOSTIC FAILURE ON COMPONENT NN (80H-FFH)
    .dw     0x05, 0x1A, 0x00    //0x19 - PARAMETER LIST LENGTH ERROR
    .dw     0x05, 0x20, 0x00    //0x1A - INVALID COMMAND OPERATION CODE
    .dw     0x05, 0x21, 0x00    //0x1B - LOGICAL BLOCK ADDRESS OUT OF RANGE
    .dw     0x05, 0x24, 0x00    //0x1C - INVALID FIELD IN COMMAND PACKET   
    .dw     0x05, 0x25, 0x00    //0x1D - LOGICAL UNIT NOT SUPPORTED
    .dw     0x05, 0x26, 0x00    //0x1E - INVALID FIELD IN PARAMETER LIST
    .dw     0x05, 0x26, 0x01    //0x1F - PARAMETER NOT SUPPORTED 
    .dw     0x05, 0x26, 0x02    //0x20 - PARAMETER VALUE INVALID
    .dw     0x05, 0x39, 0x00    //0x21 - SAVING PARAMETERS NOT SUPPORT  
    .dw     0x06, 0x28, 0x00    //0x22 - NOT READY TO READY TRANSITION - MEDIA CHANGED
    .dw     0x06, 0x29, 0x00    //0x23 - POWER ON RESET OR BUS DEVICE RESET OCCURRED
    .dw     0x06, 0x2F, 0x00    //0x24 - COMMANDS CLEARED BY ANOTHER INITIATOR
    .dw     0x07, 0x27, 0x00    //0x25 - WRITE PROTECTED MEDIA
    .dw     0x0B, 0x4E, 0x00    //0x26 - OVERLAPPED COMMAND ATTEMPED		
	.dw		0xFF, 0xFF, 0xFF	//0x27 - USB vendor command not have "GP" signature
