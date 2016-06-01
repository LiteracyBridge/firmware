#ifndef	__USB_HOST_RAM_h__
#define	__USB_HOST_RAM_h__
//	write your header here

#ifdef CREATUSBHOSTVAR
	#define EXTVAR	
#else
	#define EXTVAR	extern
#endif

	EXTVAR unsigned long	USBHostBuffer;
	EXTVAR unsigned int 	*ptr_USBHostBuffer;
	EXTVAR unsigned long	USBHostCSWBuffer;
	EXTVAR unsigned int 	*ptr_USBHostCSWBuffer;
	EXTVAR unsigned int	BulkINEP;
	EXTVAR unsigned int	BulkOUTEP;
	EXTVAR unsigned int	INTINEP;
	EXTVAR int				USBH_MaxLUN;
	EXTVAR unsigned int	R_Device_Address;
	EXTVAR unsigned int	InterfaceNumber;
	EXTVAR unsigned int	DeviceClass;
	EXTVAR unsigned int	InterfaceSubClass;
	EXTVAR unsigned int	InterfaceProtocol;
	EXTVAR unsigned int	BulkINDataToggle;
	EXTVAR unsigned int	BulkOUTDataToggle;
	EXTVAR unsigned int	INTPipeDataToggle;
	EXTVAR unsigned int	R_SOFCount;
	EXTVAR unsigned long	DeviceLength0;
	EXTVAR unsigned long	DeviceLength1;
	EXTVAR unsigned long	DeviceLength2;
	EXTVAR unsigned long	DeviceLength3;
	EXTVAR unsigned int	R_USBHDMAEN;
	EXTVAR unsigned int	R_USBHLUNStatus;	// Bit0 : LUN 0		0:Success	1:Fail
											// Bit1 : LUN 1		0:Success	1:Fail
											// Bit2 : LUN 2		0:Success	1:Fail
											// Bit3 : LUN 3		0:Success	1:Fail
	EXTVAR unsigned int ResetUSBHost;
	EXTVAR unsigned int USBSectorSize; // 1: 512 byte, 2: 1k byte, 4: 2k byte
	
/*	
	EXTVAR struct CBW_Struct {
			unsigned long DataTransferLength;
			unsigned int CBWFlag;
			unsigned int LUN;
			unsigned int CBLength;
	}*CBWStruct;
	
	EXTVAR struct SCSI_Struct {
			unsigned int OPCode;
			unsigned long LBA;
			unsigned int TXParameter;
	}*SCSIStruct;
	
	extern void CBWStruct_ASM;
	extern void SCSIStruct_ASM;
*/	
#endif

