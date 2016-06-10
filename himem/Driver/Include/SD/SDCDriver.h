#ifndef	__SDCDRIVER_h__
#define	__SDCDRIVER_h__
//	write your header here
typedef struct m_sd_info
{
	unsigned int SDC_CID[8];
	unsigned int SDC_CSD[8];
	unsigned int SDC_RCA;	
	unsigned int SDC_OCR[2];	//first word is Low  bit start
	unsigned int SDC_SCR[4];	//first word is high bit start
}sd_info;
//*************************************************************************
//========================================================
//Function Name:	DrvGetSDInfo
//Syntax:			void DrvGetSDInfo(struct sd_info* f_info)
//Purpose:			Get SD Information
//Note:
//Parameters:		f_info
//Return:			void
//=======================================================
extern int DrvGetSDInfo(sd_info* f_info);

//*************************************************************************
//========================================================
//Function Name:	DrvSDCCreatSem
//Syntax:			void DrvSDCCreatSem(void)
//Purpose:			Creat Sem
//Note:
//Parameters:		void
//Return:			void
//=======================================================
extern void DrvSDCCreatSem(void);

//========================================================
//Function Name:	DrvSDCInitial
//Syntax:			int DrvSDCInitial()
//Purpose:			Initialize SD Card
//Note:
//Parameters:		void
//Return:			0:Success	-1:Fail
//=======================================================
extern int DrvSDCInitial(void);

//========================================================
//Function Name:	DrvSDCReadSector
//Syntax:			int DrvSDCReadSector((unsigned long) LBA, (unsigned long) Store Address)
//Purpose:			Read one Sector
//Note:
//Parameters:		
//Return:			0:Success	-1:Fail
//=======================================================
extern int DrvSDCReadSector(unsigned long LBA, unsigned long Store_Address);

//========================================================
//Function Name:	DrvSDCWriteSector
//Syntax:			int DrvSDCWriteSector((unsigned long) LBA, (unsigned long) Data Address)
//Purpose:			Write one Sector
//Note:
//Parameters:		
//Return:			0:Success	-1:Fail
//=======================================================
extern int DrvSDCWriteSector(unsigned long LBA, unsigned long Data_Address);

//========================================================
//Function Name:	DrvSDCReadMultiSector
//Syntax:			int DrvSDCReadMultiSector((unsigned long) LBA, (unsigned long) Store Address, (unsigned int) Sector Count)
//Purpose:			Read Multi-Sector
//Note:
//Parameters:		
//Return:			0:Success	-1:Fail
//=======================================================
extern int DrvSDCReadMultiSector(unsigned long LBA, unsigned long Store_Address, unsigned int Sector_Count);

//========================================================
//Function Name:	DrvSDCWriteMultiSector
//Syntax:			int DrvSDCWriteMultiSector((unsigned long) LBA, (unsigned long) Data Address, (unsigned int) Sector Count)
//Purpose:			Write Multi-Sector
//Note:
//Parameters:		
//Return:			0:Success	-1:Fail
//=======================================================
extern int DrvSDCWriteMultiSector(unsigned long LBA, unsigned long Data_Address, unsigned int Sector_Count);

//========================================================
//Function Name:	DrvSDCGetSize
//Syntax:			int DrvSDCGetSize()
//Purpose:			Return Number of SDC Sectors
//Note:
//Parameters:		
//Return:			Total Sector Number
//=======================================================
extern unsigned long DrvSDCGetSize(void);

//for USB use only
extern  int DrvSDCMultiWriteSector_USB(unsigned long SectorNum, unsigned long  DataAddr,unsigned int channel,unsigned int flag);
extern  int DrvSDCReadMultiSector_USB(unsigned long SectorNum, unsigned long  DataAddr,unsigned int channel,unsigned int flag);

extern int DrvSDCRead(unsigned long LBA, unsigned long Store_Address, unsigned int Sector_Count);
extern int DrvSDCWrite(unsigned long LBA, unsigned long Data_Address, unsigned int Sector_Count);

//*************************************************************************
#endif
