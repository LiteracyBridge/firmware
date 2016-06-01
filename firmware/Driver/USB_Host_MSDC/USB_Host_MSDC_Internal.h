#ifndef _USB_HOST_MSDC_INTERNAL_H_
#define _USB_HOST_MSDC_INTERNAL_H_

#ifdef CREATE_USB_HOST_INTERNAL_VAR
	#define EXT_VAR
#else
	#define EXT_VAR	extern
#endif
//*****************************************************************************
//*****************************************************************************
//	External Function Declaration
//*****************************************************************************
//*****************************************************************************
// Starndard Command
extern unsigned int StandardCommandWithoutData(unsigned int bmRequestType,
										unsigned int bRequest,
										unsigned int wValue,
										unsigned int wIndex,
										unsigned int wLength);
extern unsigned int StandardCommandWithINData(unsigned int bmRequestType,
										  	  unsigned int bRequest,
						   			   		  unsigned int wValue, 
									   		  unsigned int wIndex, 
									   		  unsigned int wLength);
unsigned int StandardCommand_CBI(unsigned int bmRequestType,
								 unsigned int bRequest,
								 unsigned int wValue, 
								 unsigned int wIndex, 
								 unsigned int wLength);
// EP0 Command						   			   		  
extern unsigned int EP0StandardCommand(unsigned intbmRequestType,
									   unsigned intbRequest,
									   unsigned int wValue,
									   unsigned int wIndex,
									   unsigned int wLength);
extern unsigned int EP0DataStageIN(unsigned int Length);
extern unsigned int EP0StatusStage(unsigned int);
extern unsigned int EP0DataStageOUT(unsigned int Length);
unsigned int	BulkOnly_TestUnitReady(unsigned int LUN);
unsigned int	CBI_TestUnitReady(unsigned int LUN);

// SCSI Command
extern unsigned int SCSISendCMD();
extern unsigned int SCSI_Data_Stage_IN(unsigned long DataTransferLength, 
									   unsigned long StoreAddr);
extern unsigned int SCSI_Data_Stage_OUT(unsigned long DataTransferLength, 
                                        unsigned long StoreAddr);
extern unsigned int SCSICBWWithINData(unsigned long StoreAddr);
extern unsigned int SCSICBWWithNOData();
extern unsigned int SCSICBWWithDataOut(unsigned long StoreAddr);
// Bulk Command
extern void GetBulkEndpoint();
extern unsigned int BulkInData(unsigned long DataTransferLength, unsigned long StoreAddr);
extern unsigned int BulkOutData(unsigned long DataTransferLength, unsigned long StoreAddr);
extern unsigned int Data_IN(unsigned long DataTransferLength, unsigned long StoreAddr);
extern int CBI_Read28(unsigned int LUN, unsigned long LBA, unsigned long SectorNumber,unsigned long StoreAddr);
extern unsigned int	BulkOnly_ReadCapacity(unsigned int LUN);
extern unsigned int	CBI_ReadCapacity(unsigned int LUN);

// Others Command C Level
extern unsigned int CSWStatus(unsigned long);
extern unsigned int GetMaxLUNNumber();
extern unsigned int CheckLUNStatus(unsigned int LUN);
extern unsigned int GetMaxLUN(unsigned int Number);
extern void GetINTEndpoint();
extern void USB_Device_Reset(void);
extern void USBH_Delay(unsigned int);
extern void Update_USBLUN(unsigned int ret, unsigned int LUN);
extern int CBI_Status();
extern void USB_Host_Initial(void);
extern void USB_Host_Uninitial(void);
extern unsigned int USB_Device_Plug_IN(void);
extern void USB_Device_Reset(void);
extern void USBPlugOutEN(void);
extern int from_GetDesc_to_GetMaxLUN();
extern void Disable_USB_PLUG_IN();
extern void	Enable_USB_PLUG_OUT();
extern void	Disable_USB_PLUG_OUT();
extern void	Enable_USB_PLUG_IN();
extern void USBHLongDelay(unsigned int delay);

// Others Command ASM level
extern unsigned int GetConfigurationNumber();
extern unsigned int DecodeDescriptorConfiguration();
extern unsigned int DecodeDescriptorConfiguration1();
extern unsigned int DecodeDescriptorConfiguration2();
extern unsigned int CheckPortStatus();
extern unsigned long GetRequestData();
extern void GetDeviceLength(unsigned int LUN);

// Print Functin
extern void print1(char[]);
extern void print2(char x [],int type,unsigned int y);
extern void init_uart();

//*****************************************************************************
//*****************************************************************************
//	External Variable Declaration
//*****************************************************************************
//*****************************************************************************
extern unsigned int 	R_USBHLUNStatus;
extern unsigned int 	R_Device_Address;
extern unsigned int 	R_USBHDMAEN;
extern unsigned int 	USBSectorSize;
extern unsigned long	USBHostBuffer;
extern unsigned int 	*ptr_USBHostBuffer;
extern unsigned long 	USBHostCSWBuffer;
extern unsigned int 	*ptr_USBHostCSWBuffer;

extern unsigned int		InterfaceProtocol;
extern unsigned int 	InterfaceNumber;
extern unsigned int 	InterfaceSubClass;
extern unsigned int 	InterfaceNumber;
extern unsigned int		INTPipeDataToggle;

extern unsigned int 	BulkINEP;
extern unsigned int 	BulkOUTEP;
extern unsigned int 	BulkOUTDataToggle;

extern int 				MaxLUN;
extern unsigned int 	DeviceClass;
extern unsigned int 	INTINEP;

extern unsigned int 	CBI_Byte1;
extern unsigned int     USBHPlugStatus; // 0: none, 1: plug in 2: plug out

EXT_VAR struct CBW_Struct {
		unsigned long DataTransferLength;
		unsigned int CBWFlag;
		unsigned int LUN;
		unsigned int CBLength;
}*CBWStruct;
	
EXT_VAR struct SCSI_Struct {
		unsigned int OPCode;
		unsigned long LBA;
		unsigned int TXParameter;
}*SCSIStruct;
	
extern void CBWStruct_ASM;
extern void SCSIStruct_ASM;

#endif