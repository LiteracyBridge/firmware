#ifndef	__USB_RAM_DEFIN_h__
#define	__USB_RAM_DEFIN_h__
//	write your header here



////////////////////////////Initial Info///////////////////////////

typedef struct {
	unsigned int	unLunNum;			//Lun number.
	unsigned int	unLunType;			//Lun type.
	unsigned long 	ulNandStartAddr;	//Indicate any operation on this lun should be offset ulNandStartAddr sector, only for LunType_NandDrive or LunType_CDROM.
	unsigned long 	ulNandSizeSec;		//Nand flash size sector
	unsigned int 	unStatus;			//Status, write protect, no media, media change.
#ifdef USBRP
	unsigned long   rhmLunData;         //for Nor flash, point to struct flash
#endif
} str_USB_Lun_Info;

typedef struct {
	unsigned int ScsiCmdCode;
	void	(*function)(void);
} SCSI_CMD_MAP;

extern str_USB_Lun_Info USB_Lun_Define[];

extern str_USB_Lun_Info*  ptr_USB_LUN_Info; //str_USB_Lun_Info*

////////////////////////////Global Valuable/////////////////////////
//SD driver value
extern unsigned long SDCTotalSector;

//unsigned int	SCSI_State;
extern int R_EP0DataCount;
extern unsigned int	 USB_Status;
extern unsigned int	 Bulk_Stall;
extern unsigned int	 R_SDC_Present;
extern unsigned int	 R_NAND_Present;
extern unsigned int	 R_USB_Suspend;
extern unsigned long NandTotalSector;
extern unsigned int	 R_USB_State_Machine;

extern unsigned int  CBWLUN;
extern unsigned int  Command_Value;
extern unsigned int  CBWLUN;
extern unsigned int  Command_SecValue;
extern unsigned int  SCSI_Direct_Write;
extern unsigned long ulTimeout;
extern unsigned int  unGotoRamStart;
extern unsigned long CSW_Residue;
extern unsigned int	 CSW_Status;
extern unsigned int	 Sense_Code;
extern int 	SDC_AB_Buffer_Ctrl;
extern unsigned int MaxLUN;
extern unsigned int USBWriteProtect;
extern unsigned int R_Write_protect;	//SD card write protect detect, if not use SD driver, please initial this value to 0 by yourself.
extern unsigned int CDROMEnable;
extern unsigned int CDROMDelay;
extern unsigned int CDROMTimer;
extern unsigned int CDROMTimerBK;
extern unsigned int R_USB_RW_Busy;		//If device is read/write, this flag will be set 1. Otherwise set 0.
extern unsigned int USBD_PlugIn;		//0:Device not connect to PC, 1:Device connect to PC
////////////////////////////Public function/////////////////////////
void Send_Null(void);
void Send_8_Bytes(void);
void Send_Reamin_Bytes(void);
void Send_Remain_Bytes(void);
void ModeSenseData(void);

void Setup_Token(void);
void Write_EP0_FIFO(void);
void NotSupport(void);
void SCSI_Command(void);
//void GPVendorCommand();
void CommandFail(int nSenseIndex);

void Read_NandPhsicalAdr(unsigned int LUN);
int  Write_NandPhsicalAdr(unsigned int LUN);
int  Erase_NandPhsicalAdr(int BlockNum);
void GotoRAM(unsigned long address);

void RxUSB_RW_Buffer(unsigned int unLength);
void Inquiry(unsigned int unIndex);	//unIndex: 0=Removable disk, 1=CDROM disk.
void Request_Sense(unsigned int unSense_Code);
int  Read_10(unsigned long  lUSB_LUN_Read);
int  Write_10(unsigned long  lUSB_LUN_Write);
void CallRam(unsigned long address);

void DataPointer(int nTableType);
int  EP0_DataPointer_CallBack(int nTableType);

int  Get_Descriptor_Length(int nType);

unsigned int GetCMDValue(void);
unsigned int GetSecCMDValue(void);
unsigned int GetCMDID(void);

unsigned int  GetLUN(void);
unsigned long GetCSWLength(void);
unsigned int  GetCMDValueEx(int nIndex);
unsigned int  RD_Until_Bulk_IN_NACK(void);
void Mode_Sense_06(void);
unsigned long GetBufferPointer(void);
unsigned int GetTransLeng(void);
void Nand_PhysicalPage_Read(unsigned long Read_Address, int nFlashType, unsigned long ReadBuffer_Address);
int Nand_PhysicalPage_Write(unsigned long Write_Address, int nFlashType, unsigned long WriteBuffer_Address);
int Nand_ErasePhysicalBlock(unsigned int unBlockAddr, int nFlashType);
int TXToUSB(unsigned long Buffer_Address, unsigned int Sector_Number);
int RXFromUSB(unsigned long Buffer_Address, unsigned int Sector_Number);
int TxUSB_RW_Buffer_Word(unsigned int unLength, unsigned int unStartAddr);
void TxUSB_RW_Buffer(unsigned int unLength, unsigned int unStartAddr);
unsigned long GetLBA(void);
void USB_Initial(void);
void Receive_CBW_PKT(void);
void SCSI_CSW(void);
void USB_ISR(void);
void USB_Reset(void);
void Send_To_USB_DMA(unsigned long);
void Receive_From_USB_DMA(unsigned long);
int  NandFlashInitial(void);
void Read_NandLogicalAdr(unsigned int LUN);
int  Write_NandLogicalAdr(unsigned int LUN);
void USBDSuspend_CallBack(void);
void USB_ServiceLoop(unsigned int unUseLoop);

void GotoRAM(unsigned long address);
void Check_SDC_Present(void);
void NetLanReadVendorInfo(void);
void Read_Capacity(unsigned long ulCapacity);
void Read_CDROM_Capacity(unsigned long ulCapacity);
void Read_Format_Capacities(unsigned long ulCapacity);
void Prevent_Allow_Medium_removal(void);
void fUSB_SDCardPlugIn();
void fUSB_SDCardPullOut();
void CheckCDRomValid(void);
////////////////////////////////USB Commands call back///////////////////////////////////////////
unsigned int TestUnitReady_CallBack(void);
unsigned int ModeSense6_CallBack(void);
void CDROMTimerStartup(void);
void CDROMTimerOccur(void);
void VendorCmd_CallBack(void);	
void USBDSuspend_CallBack(void);


extern int _NAND_ReadSector(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr);
#endif
