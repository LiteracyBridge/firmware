#ifndef	__USBHOSTMSDC_h__
#define	__USBHOSTMSDC_h__
//	write your header here

//#define print

#define USBHostMSDCFail		-1
#define USBHostMSDCSuccess	0
//========================================================
//Function Name:	DrvUSBHostMSDCInitial
//Syntax:			void DrvUSBHostMSDCInitial(void)
//Purpose:			Initial USB Host Module
//Note:
//Parameters:		void
//Return:			0x0000 : Success
//					0xFFFF : Initial Fail
//					0xFFFE : USB Device is NOT Plugged IN
//=======================================================
extern int DrvUSBHMSDCInitial(void);

//========================================================
//Function Name:	DrvUSBHostMSDCLUNIniti
//Syntax:			int DrvUSBHostMSDCLUNIniti(unsigned int)
//Purpose:			Initial USB LUN
//Note:
//Parameters:		unsigned int LUN, Logical Unit Number
//Return:			-1 : Fail		0 : Success
//=======================================================
extern int DrvUSBHLUNInit(unsigned int LUN);

//========================================================
//Function Name:	DrvUSBHReadMultiSector
//Syntax:			unsigned int DrvUSBHReadMultiSector(unsigned long LBA, 
//														unsigned long StoreAddr, 
//														unsigned int SectorNumber, 
//														unsigned int LUN);
//Purpose:			Read Multi Sector
//Note:
//Parameters:		unsigned long LBA, unsigned long StoreAddr, unsigned int SectorNumber, unsigned int LUN
//Return:			-1 : Fail		0 : Success
//=======================================================
extern unsigned int DrvUSBHReadMultiSector(unsigned long LBA, unsigned long StoreAddr, unsigned int SectorCnt, unsigned int LUN);
extern unsigned int MSDC_Read28(unsigned long StartLBA, unsigned long StoreAddr, unsigned int SectorCnt, unsigned int LUN);
extern unsigned int MSDC_Write2A(unsigned long StartLBA, unsigned long StoreAddr, unsigned int SectorCnt, unsigned int LUN);
//========================================================
//Function Name:	DrvUSBHWriteMultiSector
//Syntax:			unsigned int DrvUSBHWriteMultiSector(unsigned long LBA, 
//														unsigned long StoreAddr, 
//														unsigned int SectorNumber, 
//														unsigned int LUN);
//Purpose:			Write Multi Sector
//Note:
//Parameters:		unsigned long LBA, unsigned long StoreAddr, unsigned int SectorNumber, unsigned int LUN
//Return:			-1 : Fail		0 : Success
//=======================================================
extern unsigned int DrvUSBHWriteMultiSector(unsigned long LBA, unsigned long StoreAddr, unsigned int SectorCnt, unsigned int LUN);

//========================================================
//Function Name:	DrvUSBHMouseEnable
//Syntax:			void DrvUSBHMouseEnable(void);
//Purpose:			Enable Mouse Function
//Note:
//Parameters:		none
//Return:			none
//=======================================================
//extern void DrvUSBHMouseEnable(void);

//========================================================
//Function Name:	DrvUSBHMouseDisable
//Syntax:			void DrvUSBHMouseDisable(void);
//Purpose:			Disable Mouse Function
//Note:
//Parameters:		none
//Return:			none
//=======================================================
//extern void DrvUSBHMouseDisable(void);

//========================================================
//Function Name:	DrvUSBHGetMouseStatus
//Syntax:			int DrvUSBHGetMouseStatus(struct MouseStatus MouseStatus);
//Purpose:			Get Mouse Current Status
//Note:
//Parameters:		struct MouseStatus
//Return:			0 : Success		-1 : Mouse Reply NACK
//=======================================================
//extern int DrvUSBHGetMouseStatus(struct MouseStatus *MouseStatus);


extern void USBHostISR(void);
extern unsigned int DrvUSBHGetSectorSize(); // call after LUN is initialized
											// 1: 512k, 2: 1k, 4: 2k
extern unsigned long DrvUSBHGetSize(unsigned int LUN); //size by sector											
extern unsigned int GetUSBHPlugStatus(); 										
extern void SetUSBHPlugStatus(unsigned int status); 			
extern void USBH_Delay(unsigned int);							

extern void	init_uart(void);
extern void	print1(char[]);
extern void print2(char x [],int type,unsigned int y);
#endif
