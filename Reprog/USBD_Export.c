// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org

#include ".\System\Include\System\GPL162002.h"
#ifdef USBRP
#include ".\Reprog\USBD_Define.h"
#include ".\Reprog\USBD.h"
#else
#include ".\Driver\Include\USB\USBD_Define.h"
#include ".\Driver\Include\USB\USBD.h"
#endif

#include ".\Reprog\USB_Flash_reprog.h"

//#define OS_vension   
#define SD_Used
//#define Nand_Used
extern void Send_To_USB_DMA(unsigned long);
extern int Send_To_USB_DMA_USB(unsigned long ,unsigned int, unsigned int);
extern int Receive_From_USB_DMA_USB(unsigned long ,unsigned int);
#ifdef SD_Used
extern unsigned int DrvSDCMultiWriteSector_USB(unsigned long SectorNum, unsigned long  DataAddr,unsigned int channel,unsigned int flag);
extern unsigned int DrvSDCReadMultiSector_USB(unsigned long SectorNum, unsigned long  DataAddr,unsigned int channel,unsigned int flag);
extern int DrvSDCInitial(void);
extern unsigned long SDCTotalSector;
#endif
#ifdef Nand_Used
extern int _NAND_ReadSector(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr);
extern int _NAND_WriteSector(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr);
extern int _NAND_ReadSector_USB(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr, unsigned int DMA_Channel);
extern int _NAND_WriteSector_USB(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr, unsigned int DMA_Channel,unsigned int DMA_RB_Flag);
#endif
extern int TXToUSB_USB(unsigned long USB_RW_Buffer,unsigned int sectorNum,unsigned int DMA_Channel,unsigned int);
void NetLanReadVendorInfo();
#ifdef OS_vension
extern unsigned int MallocChannel();
extern void FreeChannel(unsigned int ch);
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////
//Return Value:
//	1: Fail, 0: Success
///////////////////////////////////////////////////////////////////////////////////////////////////////
int Read_10(unsigned long  lUSB_LUN_Read)
{
	int i;
	unsigned long SCSI_LBA;
	unsigned int SCSI_Transfer_Length;
	unsigned long USB_RW_Buffer_PTR, USB_RW_Buffer_PTR_A,USB_RW_Buffer_PTR_B,USB_RW_Buffer_PTR_Temp;
	int ret,stage;
	unsigned long num;
	unsigned int USBDMA_CHANNEL0, USBDMA_CHANNEL1, USBDMA_CHANNEL2, USBDMA_CHANNEL3;
	str_USB_Lun_Info*  ptr_USB_LUN_Read;

	ptr_USB_LUN_Read = (str_USB_Lun_Info*)lUSB_LUN_Read;
	ulTimeout = 0x4FFFF;
	ret=0x00;
	stage = 0x00;
	Sense_Code = 0;
	R_USB_RW_Busy = 1;

	SCSI_LBA=GetLBA();

	SCSI_Transfer_Length=GetTransLeng();

	USB_RW_Buffer_PTR=GetBufferPointer();
	USB_RW_Buffer_PTR_A=USB_RW_Buffer_PTR;   //add by zhangxh 2006/01/19
	USB_RW_Buffer_PTR_B=USB_RW_Buffer_PTR_A + 256;	

#ifdef OS_vension
	USBDMA_CHANNEL0 = MallocChannel();	 //by zhangxh 2006/01/19
	USBDMA_CHANNEL1 = MallocChannel();	 //by zhangxh 2006/01/19
	USBDMA_CHANNEL2 = MallocChannel();	 //by zhangxh 2006/01/19
	USBDMA_CHANNEL3 = MallocChannel();	 //by zhangxh 2006/01/19
#else
	USBDMA_CHANNEL0 = 0;
	USBDMA_CHANNEL1 = 1;
	USBDMA_CHANNEL2 = 2;
	USBDMA_CHANNEL3 = 3;
#endif

	if(ptr_USB_LUN_Read->unLunType == LunType_SDCardReader)  //SD Card
	{
#ifdef SD_Used	
		if ((SCSI_LBA + SCSI_Transfer_Length)>SDCTotalSector)	//huck
		{
			Sense_Code = 0x1B;  //LOGICAL BLOCK ADDRESS OUT OF RANGE
			ret = 1;
			goto End;
		}

		for(i=0 ; i < SCSI_Transfer_Length ; i++)
		{			

			if (i == 0)

				ret = DrvSDCReadMultiSector_USB((unsigned long)SCSI_LBA+i, (unsigned long)USB_RW_Buffer_PTR_A,USBDMA_CHANNEL0,0);
			else 
				ret = DrvSDCReadMultiSector_USB((unsigned long)SCSI_LBA+i, (unsigned long)USB_RW_Buffer_PTR_A,USBDMA_CHANNEL0,1);

			if (i == SCSI_Transfer_Length-1)
				ret = DrvSDCReadMultiSector_USB((unsigned long)SCSI_LBA+i, (unsigned long)USB_RW_Buffer_PTR_A,USBDMA_CHANNEL0,2);


			if(ret != 0x00)
			{
				DrvSDCReadMultiSector_USB((unsigned long)SCSI_LBA+i, (unsigned long)USB_RW_Buffer_PTR_A,USBDMA_CHANNEL0,4); //4,stop the SDC Only					
				Sense_Code=0x08;
				goto End;
			}			

			if (stage)
			{
				ret = Send_To_USB_DMA_USB(USB_RW_Buffer_PTR_A,USBDMA_CHANNEL1,1);
				for(num=0;num<ulTimeout;num++)
				{
					if(R_USB_Suspend == 1)
						break;
					else
					{
						if ((*P_USBD_EPEvent & 0x0100) != 0)
							ret = 0xffff;
						else
						{
							ret = 0;
							*P_USBD_EPEvent=0x0600;
							break;
						}
					}
				}

				if(num == ulTimeout)
				{
					Sense_Code=0x12;
					ret = 0xffff;
					return ret;					
				}
			}		

			ret = Send_To_USB_DMA_USB(USB_RW_Buffer_PTR_A,USBDMA_CHANNEL1,0);									
			if (ret != 0x00)
			{
				ret = 0xffff;	
				Sense_Code = 0x12; 			
				goto End;
			}

			stage=1;		

			//for last data 
			if (i == SCSI_Transfer_Length-1)
			{
				ret = Send_To_USB_DMA_USB(USB_RW_Buffer_PTR_A,USBDMA_CHANNEL1,1);

				if (ret != 0x00)
				{
					ret = 0xffff;	
					Sense_Code = 0x12; 			
					goto End;
				}

				for(num=0;num<ulTimeout;num++)
				{
					if(R_USB_Suspend == 1)
						break;
					else
					{
						if ((*P_USBD_EPEvent & 0x0100) != 0)
							ret = 0xffff;
						else
						{
							ret = 0;
							*P_USBD_EPEvent=0x0600;
							break;
						}
					}
				}
				if(num == ulTimeout)
				{
					ret = 0xffff;	
					Sense_Code = 0x12; 			
					goto End;
				}

			}
			USB_RW_Buffer_PTR_Temp = USB_RW_Buffer_PTR_B;			
			USB_RW_Buffer_PTR_B = USB_RW_Buffer_PTR_A;
			USB_RW_Buffer_PTR_A = USB_RW_Buffer_PTR_Temp;			

		}//for		

		if(ret != 0x00)
		{			
			CSW_Residue=SCSI_Transfer_Length-i;			
			CSW_Residue=CSW_Residue << 9;	
		}
		else
			CSW_Residue=0;	
#endif
	}	
	else //Nand Drive
	{
#ifdef Nand_Used
		if ((SCSI_LBA + SCSI_Transfer_Length)>ptr_USB_LUN_Read->ulNandSizeSec)	//check read address overflow
		{
			Sense_Code = 0x1B;
			ret = 1;
			goto End;
		}

		if(ptr_USB_LUN_Read->unLunType == LunType_CDROM)	//CDROM, do not modify below CDROM code
		{
			SCSI_LBA ++;						
			SCSI_LBA = SCSI_LBA << 2;						//1 sector = 4 *512
			SCSI_LBA += ptr_USB_LUN_Read->ulNandStartAddr;		
			SCSI_Transfer_Length = SCSI_Transfer_Length << 2; //length *4
		}
		else
			SCSI_LBA += ptr_USB_LUN_Read->ulNandStartAddr;		

		for(i=0 ; i < SCSI_Transfer_Length ; i++)
		{
			ret = _NAND_ReadSector_USB((unsigned long)(SCSI_LBA+i), 1, (unsigned long)USB_RW_Buffer_PTR_A, USBDMA_CHANNEL2);

			if (stage)
			{
				TXToUSB_USB(USB_RW_Buffer_PTR_A, 0x01, USBDMA_CHANNEL1, 0x01);

				ulTimeout = 0x4FFFF;
				while((*P_USBD_EPEvent & 0x0100) != 0)
				{						
					if (!(ulTimeout--))	
					{
						*P_USBD_EPEvent=0x0600;	
						Sense_Code = 0x12; 
						ret = 1;
						goto End;
					}
				}
				*P_USBD_EPEvent=0x0600;
			}

			TXToUSB_USB(USB_RW_Buffer_PTR_A, 0x01, USBDMA_CHANNEL1, 0x00);
			stage=1;

			if (i == SCSI_Transfer_Length-1)
			{
				TXToUSB_USB(USB_RW_Buffer_PTR_A, 0x01, USBDMA_CHANNEL1, 0x01);
				ulTimeout = 0x4FFFF;
				while((*P_USBD_EPEvent & 0x0100) != 0)
				{						
					if (!(ulTimeout--))	
					{
						*P_USBD_EPEvent=0x0600;	
						Sense_Code = 0x12; 
						ret = 1;
						goto End;
					}
				}
				*P_USBD_EPEvent=0x0600;
			}
			USB_RW_Buffer_PTR_Temp = USB_RW_Buffer_PTR_B;			
			USB_RW_Buffer_PTR_B = USB_RW_Buffer_PTR_A;
			USB_RW_Buffer_PTR_A = USB_RW_Buffer_PTR_Temp;
		}		

		CSW_Residue=0x00;
#endif
	}
End:	
#ifdef OS_vension
	FreeChannel(USBDMA_CHANNEL0);
	FreeChannel(USBDMA_CHANNEL1);
	FreeChannel(USBDMA_CHANNEL2);
	FreeChannel(USBDMA_CHANNEL3);		
#endif

	return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Return Value:
//	1: Fail, 0: Success
///////////////////////////////////////////////////////////////////////////////////////////////////////
int Write_10(unsigned long	lUSB_LUN_Write)
{
	int j;
	unsigned long SCSI_LBA;
	unsigned int SCSI_Transfer_Length;
	unsigned long USB_RW_Buffer_PTR;
	unsigned long USB_RW_Buffer_PTR_A,USB_RW_Buffer_PTR_B,USB_RW_Buffer_PTR_Temp;
	unsigned int USBDMA_CHANNEL0, USBDMA_CHANNEL1, USBDMA_CHANNEL2, USBDMA_CHANNEL3;
	int ret= 0xffff, stage = 0, *p;
	int ret1=0xffff;
	unsigned int Erro_Flag = 0;
	extern unsigned int *RHM_FlashBuf, RHM_USBreprogBuf_Full;
	unsigned int idx;
	extern unsigned int USBreprogBuf[];
	str_USB_Lun_Info*  ptr_USB_LUN_Write;

	ptr_USB_LUN_Write = (str_USB_Lun_Info*)lUSB_LUN_Write;
	R_USB_RW_Busy = 1;
	Sense_Code = 0x00;
	SCSI_LBA=GetLBA();
	SCSI_Transfer_Length=GetTransLeng();
	USB_RW_Buffer_PTR=GetBufferPointer();

	USB_RW_Buffer_PTR_A=GetBufferPointer();   //add by zhangxh 2006/01/19
	USB_RW_Buffer_PTR_B=USB_RW_Buffer_PTR_A + 256;

#ifdef OS_vension
	USBDMA_CHANNEL0 = MallocChannel();	 //by zhangxh 2006/01/19
	USBDMA_CHANNEL1 = MallocChannel();	 //by zhangxh 2006/01/19
	USBDMA_CHANNEL2 = MallocChannel();	 //by zhangxh 2006/01/19
	USBDMA_CHANNEL3 = MallocChannel();	 //by zhangxh 2006/01/19
#else
	USBDMA_CHANNEL0 = 0;
	USBDMA_CHANNEL1 = 1;
	USBDMA_CHANNEL2 = 2;
	USBDMA_CHANNEL3 = 3;
#endif	

	if(ptr_USB_LUN_Write->unLunType == LunType_SDCardReader)
	{
#ifdef SD_Used	
		if ((SCSI_LBA + SCSI_Transfer_Length)>SDCTotalSector)	//huck	
		{
			Sense_Code = 0x1B; //LOGICAL BLOCK ADDRESS OUT OF RANGE
			ret = 1;
			goto End;			
		}

		for(j=0; j<SCSI_Transfer_Length; j++)
		{							
			//=================================================================================================
			// Enable DMA mode. And using USB 128x8 bytes dual FIFO			
			ret=Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B,USBDMA_CHANNEL3);			
			if(ret != 0x00)
			{
				*P_USBD_EPEvntClear=0x0010;
				*P_USBD_EPEvent=0x2000;		// Bulk OUT NACK Reset
				break;
			}

			*P_USBD_EPEvntClear=0x0010;
			*P_USBD_EPEvent=0x2000;		// Bulk OUT NACK Reset

			USB_RW_Buffer_PTR_Temp = USB_RW_Buffer_PTR_B;
			if (j>0 )
			{
				stage = 1;				
			}
			if (!Erro_Flag)
				ret1 = DrvSDCMultiWriteSector_USB((unsigned long)SCSI_LBA+j, (unsigned long)USB_RW_Buffer_PTR_B,(unsigned int)USBDMA_CHANNEL2,stage);			

			if(ret1 != 0x00)
			{
				ret = ret1;
				break;
			}			
			USB_RW_Buffer_PTR_B = USB_RW_Buffer_PTR_A;
			USB_RW_Buffer_PTR_A = USB_RW_Buffer_PTR_Temp;

		}//j

		if((ret1 == 0x00)&&(!Erro_Flag))
			ret1=DrvSDCMultiWriteSector_USB((unsigned long)SCSI_LBA+j, (unsigned long)USB_RW_Buffer_PTR_B,(unsigned int)USBDMA_CHANNEL2,2);		

		if ((ret1 != 0x00)&&(!Erro_Flag))
		{
			DrvSDCMultiWriteSector_USB((unsigned long)SCSI_LBA+j, (unsigned long)USB_RW_Buffer_PTR_B,(unsigned int)USBDMA_CHANNEL2,4);		
			ret = ret1;
		}

		if( (ret != 0x00) || (ret1 != 0x00) )
		{
			CSW_Residue=SCSI_Transfer_Length-j-1;
			CSW_Residue=CSW_Residue << 9;
			Sense_Code=0x10;	   
		}
		else
			CSW_Residue=0;
#endif
	}
	else if(ptr_USB_LUN_Write->unLunType == LunType_NOR) {  //rhm
	
		flash *pflash = (flash *)ptr_USB_LUN_Write->rhmLunData;

		ptr_USB_LUN_Write->ulNandSizeSec = 0xaabbccdd;
		ptr_USB_LUN_Write->ulNandSizeSec = 2048;
	
//		long xx = ptr_USB_LUN_Write->ulNandSizeSec;
//		if ((SCSI_LBA + (SCSI_Transfer_Length*512)) > (ptr_USB_LUN_Write->ulNandSizeSec))	//check address overflow	
//		{
//			Sense_Code = 0x1B;//LOGICAL BLOCK ADDRESS OUT OF RANGE
//			ret = 1;			
//			goto End;
//		}

//		SCSI_LBA += ptr_USB_LUN_Write->ulNandStartAddr;	
		
		//rhm nor erase sector here	
		
		*P_WatchDog_Ctrl = 0;		// disable watchdog

		pflash->pflash = SCSI_LBA;
		pflash->bytesAvail = 0;
//rhm1		pflash->erasesector(pflash);

		for(j=0; j<SCSI_Transfer_Length; j++)
		{				
			//=================================================================================================
			// Enable DMA mode. And using USB 128x8 bytes dual FIFO
			//ret=Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B,DMA_CHANNEL2);
//RHM			ret=Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B, USBDMA_CHANNEL1);

		    ret=Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B, USBDMA_CHANNEL3);

			if (ret != 0x00)
			{
				*P_USBD_EPEvntClear=0x0010;			
				*P_USBD_EPEvent=0x2000;		// Bulk OUT NACK Reset
				break;
			}
			*P_USBD_EPEvntClear=0x0010;			
			*P_USBD_EPEvent=0x2000;		// Bulk OUT NACK Reset

			if (j==0)
				stage = 0;
			else 
				stage = 1;
				
			{
			unsigned short *sp = (unsigned short *)USB_RW_Buffer_PTR_B;
			if(RHM_FlashBuf) {
				for(idx=0; idx<256; idx++) {
					RHM_FlashBuf[(j*256)+idx] = *sp++;
				}
				pflash->bytesAvail += 256;
			}
			}

			USB_RW_Buffer_PTR_Temp = USB_RW_Buffer_PTR_B;
			USB_RW_Buffer_PTR_B = USB_RW_Buffer_PTR_A;
			USB_RW_Buffer_PTR_A = USB_RW_Buffer_PTR_Temp;
		}//j

		if(ret != 0x00)
		{
			CSW_Residue=SCSI_Transfer_Length-j-1;
			CSW_Residue=CSW_Residue << 9;
			Sense_Code=0x10;	//WRITE FAULT
		}
		else {
			CSW_Residue=0;			
			USB_Status =  BULK_IN_NACK;  //TEST			
			RHM_USBreprogBuf_Full = j * 256;
		}
	} // rhm end LunType_NOR
	else { //  rhm LunType_RAM
		
//RHM #ifdef Nand_Used	
		j=0;

		if ((SCSI_LBA + (SCSI_Transfer_Length*512))>ptr_USB_LUN_Write->ulNandSizeSec)	//check address overflow	
		{
			Sense_Code = 0x1B;//LOGICAL BLOCK ADDRESS OUT OF RANGE
			ret = 1;			
			goto End;
		}

		SCSI_LBA += ptr_USB_LUN_Write->ulNandStartAddr;		

		for(j=0; j<SCSI_Transfer_Length; j++)
		{				
			//=================================================================================================
			// Enable DMA mode. And using USB 128x8 bytes dual FIFO
			//ret=Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B,DMA_CHANNEL2);
//RHM			ret=Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B, USBDMA_CHANNEL1);
		    ret=Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B, USBDMA_CHANNEL3);

			if (ret != 0x00)
			{
				*P_USBD_EPEvntClear=0x0010;			
				*P_USBD_EPEvent=0x2000;		// Bulk OUT NACK Reset
				break;
			}
			*P_USBD_EPEvntClear=0x0010;			
			*P_USBD_EPEvent=0x2000;		// Bulk OUT NACK Reset

			if (j==0)
				stage = 0;
			else 
				stage = 1;

//			if (!Erro_Flag)				
//RHM				ret1 = _NAND_WriteSector_USB((unsigned long)SCSI_LBA+j, 1, (unsigned long)USB_RW_Buffer_PTR_B,(unsigned int)USBDMA_CHANNEL0,stage);
					memcpy(SCSI_LBA+(j*256), (unsigned long)USB_RW_Buffer_PTR_B, 256);
//			if (ret1 != 0x00)
//				break;

			USB_RW_Buffer_PTR_Temp = USB_RW_Buffer_PTR_B;
			USB_RW_Buffer_PTR_B = USB_RW_Buffer_PTR_A;
			USB_RW_Buffer_PTR_A = USB_RW_Buffer_PTR_Temp;
		}//j

		if(ret != 0x00)
		{
			CSW_Residue=SCSI_Transfer_Length-j-1;
			CSW_Residue=CSW_Residue << 9;
			Sense_Code=0x10;	//WRITE FAULT
		}
		else
			CSW_Residue=0;
//RHM #endif
	}

End:	
#ifdef OS_vension
	FreeChannel(USBDMA_CHANNEL0);
	FreeChannel(USBDMA_CHANNEL1);
	FreeChannel(USBDMA_CHANNEL2);
	FreeChannel(USBDMA_CHANNEL3);		
#endif

	return ret;	
}


////////////////////////////////////////////////////////////////////////////////////////////
//Purpose:
//			Call back function, if return 0, it will run default process. If return 1, means
//			user modify reply value and default process will be skip.
////////////////////////////////////////////////////////////////////////////////////////////
unsigned int TestUnitReady_CallBack()
{
	
	return 0;
}

unsigned int ModeSense6_CallBack()
{
	
	return 0;	
}

void VendorCmd_CallBack()
{
	
	CommandFail(0x1A);	//unsupport command
}
//////////////////////////////////////////////////////////////////////////////////////////////
//This function will be called if PC run to be suspend or device be plug out.
//////////////////////////////////////////////////////////////////////////////////////////////
void USBDSuspend_CallBack()
{
 	R_USB_Suspend=1;	//set R_USB_Suspend=1 will break out usb service loop.
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Return 1, if you want to redirect R_Descriptor_High and R_Descriptor_Low address.
//Return 0, use default reply info.
///////////////////////////////////////////////////////////////////////////////////////////////
int EP0_DataPointer_CallBack(int nTableType)
{
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
//Purpose:
//			Setup CDRom timer, user can assigned TimeA~TimeD as timer register.
//			Be careful in IRQ4, will change CDDelay parameter value. If user do not use CDROM
//			function, this function will not be used.
////////////////////////////////////////////////////////////////////////////////////////////
void CDROMTimerStartup()
{
	*P_TimerD_Preload=0x10000-4096;	//enable timerD and set timer
    *P_TimerD_Ctrl=0xe064;
}

void CDROMTimerOccur()
{
	int nTrigger;
	
	nTrigger = (*P_INT_Status2) & C_INT_TimerD;
	if (nTrigger)	
	{		
	    CDROMDelay = 0;	
		*P_TimerD_Ctrl = 0x8064;	//Disable timer
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Purpose: USB service loop will polling this function to check SD card present.
// If SDC is inserted, please initialize the SDC and set R_SDC_Present to 1 and call fUSB_SDCardPlugIn().
// If SDC is removed, please clear R_SDC_Present to 0 and call fUSB_SDCardPullOut().
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Check_SDC_Present(void)
{
	int IOStatus, ret;
		
  	if(0)
	{   
		IOStatus = *((volatile unsigned int*)P_IOA_Data);//Use IOA0 detect card in for example.
		ret = IOStatus & 0x0001;
		if (R_SDC_Present == 0 && ret ==0)//insert
		{	
			ret = DrvSDCInitial();
			if(ret == 0)
			{
				R_SDC_Present = 1;				
				fUSB_SDCardPlugIn();	//call this function to notify USB library SD card plug-in		
			}		
		}
		else if (R_SDC_Present == 1 && ret ==1)//pull out
		{
			R_SDC_Present = 0;
			fUSB_SDCardPullOut();		//call this function to notify USB library SD card pull-out
		}
		
		//Check SD Card write protect
		if (IOStatus & 0x0002)	//Use IOA1 detect write protect for example.
			R_Write_protect = 1;	//SD Card write protect
		else
			R_Write_protect = 0;
	}	 


}
