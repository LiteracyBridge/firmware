// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#define const

#include "GPL162002.h"
#include ".\Reprog\USBD_Define.h"
#include ".\Reprog\USBD.h"

#include "USB_Flash_reprog.h"
extern int SetVenderID();
extern unsigned int R_CBW_PKT[31];
extern int REPROG_IN_Progress;

				
#define	USB_TIME_OUT	3			// timeout  3s
//RHM extern USBHostType  USBHost_Flag;

extern void (*ReprogJump)();
extern flash *RHM_FlashPtr;
void FP_USB_Insert_TimeOut(void);
long RHMdbg_pflash;
extern unsigned int RHM_USBreprogBuf_Full;
extern unsigned int *RHM_FlashBuf;

//FP_Flashing_EP must be first function 
void FP_Flashing_EP()
{
	extern long USB_ISR_PTR;
	extern long USB_INSERT_PTR;
	
	void FP_USB_ISR(), FP_USB_Insert_TimeOut();
	
	USB_ISR_PTR = (long) FP_USB_ISR;
	USB_INSERT_PTR = (long) FP_USB_Insert_TimeOut;
	RHM_USBreprogBuf_Full = 0;
	ReprogJump = 0;

	CSW_Residue=0;
	CSW_Status=0;	

	R_USB_State_Machine=SCSI_CSW_Stage;

	FP_USB_ServiceLoop(1);
}

void FP_memcpy(int *to, int *from, int count)
{
	int i;
	for (i=0; i<count; i++)
		*to++ = *from++;
}

void FP_USB_Insert_TimeOut(void)
{
	int temp;
	extern unsigned long usb_time_out;

	usb_time_out += 1;
	temp=*P_TimeBaseA_Ctrl;
	*P_TimeBaseA_Ctrl=temp;	//clear timerA int flag	
	
	if( usb_time_out > USB_TIME_OUT ) {
#ifdef USBHOST
		if(USBHost_Flag == C_USBDiskPlugIn){			
		   USBHost_Flag = C_USBPlugInTimeOut;					
		}
		else
#endif
		if (!USBD_PlugIn){		//Check USB plug in		
			R_USB_Suspend = 1;
		}
	*P_TimeBaseA_Ctrl = 0x0000;
	}
}

//RHM flash boot, start all externs with FP_ so linker will
//    not find the code in the normally running code

void FP_Cmd_TestUnitReady();
void FP_Cmd_RequestSense();
void FP_Cmd_Inquiry();
void FP_Cmd_ModeSense6();
void FP_Cmd_StartStop();
void FP_Cmd_MediumRemoval();
void FP_Cmd_FormatCapacity();
void FP_Cmd_ReadCapacity();
void FP_Cmd_Read();
void FP_Cmd_Write();
void FP_Cmd_Verify();
void FP_Cmd_ModeSense10();
void FP_Cmd_GPVendorCmd();
void FP_Cmd_GPVendorCmdMP();
void FP_Cmd_ReadTOC();
void FP_ReadDriveInfoTable();
void FP_GetDriveStatus();
void FP_GetICVersion();
void FP_CheckCDRomValid(void);
int FP_SetVenderID();

extern tra_end;
extern str_USB_Lun_Info 	*FP_USB_Lun_Define[N_USB_LUN];

/* RHM 
SCSI_CMD_MAP FP_ScsiCmdMapList[] =
	{
		{0x00, (void*)FP_Cmd_TestUnitReady},	
		{0x03, (void*)FP_Cmd_RequestSense},	
		{0x12, (void*)FP_Cmd_Inquiry},	
		{0x1A, (void*)FP_Cmd_ModeSense6},	
		{0x1B, (void*)FP_Cmd_StartStop},	
		{0x1E, (void*)FP_Cmd_MediumRemoval},	
		{0x23, (void*)FP_Cmd_FormatCapacity},	
		{0x25, (void*)FP_Cmd_ReadCapacity},	
		{0x28, (void*)FP_Cmd_Read},	
		{0x2A, (void*)FP_Cmd_Write},	
		{0x2F, (void*)FP_Cmd_Verify},	
		{0x43, (void*)FP_Cmd_ReadTOC},	
		{0x5A, (void*)FP_Cmd_ModeSense10},	
		{0xF0, (void*)FP_Cmd_GPVendorCmd},	
		{0xFF, (void*)FP_Cmd_GPVendorCmdMP}	

	};

	
SCSI_CMD_MAP FP_GPVScsiCmdMapList[] = {
		{0x00, (void*)FP_ReadDriveInfoTable},	
		{0x01, (void*)FP_GetDriveStatus},	
		{0x10, (void*)FP_GetICVersion}	
	};				



SCSI_CMD_MAP FP_MPScsiCmdMapList[] = {
		{0xF0, (void*)FP_SetVenderID},
		{0x2a, (void*)FP_RHMWrite},
		{0xff, (void*)FP_RHMJump}
	};				
*/

void FP_RHMWrite();
void FP_RHMJump();

void FP_SCSI_Command(void)
{
	unsigned int i;//, unRet;
	
//rhm	SCSI_CMD_MAP	*ScsiCmdPtr;
/*RHM	
SCSI_CMD_MAP FP_ScsiCmdMapList[] =
	{
		{0x00, (void*)FP_Cmd_TestUnitReady},	
		{0x03, (void*)FP_Cmd_RequestSense},	
		{0x12, (void*)FP_Cmd_Inquiry},	
		{0x1A, (void*)FP_Cmd_ModeSense6},	
		{0x1B, (void*)FP_Cmd_StartStop},	
		{0x1E, (void*)FP_Cmd_MediumRemoval},	
		{0x23, (void*)FP_Cmd_FormatCapacity},	
		{0x25, (void*)FP_Cmd_ReadCapacity},	
		{0x28, (void*)FP_Cmd_Read},	
		{0x2A, (void*)FP_Cmd_Write},	
		{0x2F, (void*)FP_Cmd_Verify},	
		{0x43, (void*)FP_Cmd_ReadTOC},	
		{0x5A, (void*)FP_Cmd_ModeSense10},	
		{0xF0, (void*)FP_Cmd_GPVendorCmd},	
		{0xFF, (void*)FP_Cmd_GPVendorCmdMP}	

	};
rhm */
	CSW_Status=0;
	
	Command_Value = FP_GetCMDValue();
	CBWLUN = FP_GetLUN();
	CSW_Residue = FP_GetCSWLength();
	if (FP_GetCMDValueEx(12))
		SCSI_Direct_Write = 0;
	else
		SCSI_Direct_Write = 1;
	
	if (!SCSI_Direct_Write || !CSW_Residue)//Bulk-in check ready
	{
		*P_USBD_EPEvent=0x0400;		// Bulk IN NACK Reset
		FP_RD_Until_Bulk_IN_NACK();	// Read until Bulk IN NACK
	}	
	
	if ((Command_Value > 0xF0)&&(Command_Value != 0xFF))	//customer vendor command
	{
		FP_VendorCmd_CallBack();	
		return;
	}	
		
/*rhm causes call to multiply in low memory
	for(i=0; i<(sizeof(FP_ScsiCmdMapList)/sizeof(SCSI_CMD_MAP)); i++)
	{
		ScsiCmdPtr = &FP_ScsiCmdMapList[i];
		if(ScsiCmdPtr->ScsiCmdCode == Command_Value)//+15: vendor command item code
		{
			(*ScsiCmdPtr->function)();
			return;
		}		
	}
*/
	if(Command_Value == 0) {
		FP_Cmd_TestUnitReady();
		return;
	}
	if(Command_Value == 3) {
		FP_Cmd_RequestSense();
		return;
	}	
	if(Command_Value == 0x12) {
		FP_Cmd_Inquiry();
		return;
	}
	if(Command_Value == 0x1a) {
		FP_Cmd_ModeSense6();
		return;
	}	
	if(Command_Value == 0x1b) {
		FP_Cmd_StartStop();
		return;
	}
	if(Command_Value == 0x1e) {
		FP_Cmd_MediumRemoval();
		return;
	}	
	if(Command_Value == 0x23) {
		FP_Cmd_FormatCapacity();
		return;
	}	
	if(Command_Value == 0x25) {
		FP_Cmd_ReadCapacity();
		return;
	}	
	if(Command_Value == 0x28) {
		FP_Cmd_Read();
		return;
	}	
	if(Command_Value == 0x2a) {
		FP_Cmd_Write();
		return;
	}	
	if(Command_Value == 0x2f) {
		FP_Cmd_Verify();
		return;
	}	
	if(Command_Value == 0x43) {
		FP_Cmd_ReadTOC();
		return;
	}	
	if(Command_Value == 0x5a) {
		FP_Cmd_ModeSense10();
		return;
	}
	if(Command_Value == 0xf0) {
		FP_Cmd_GPVendorCmd();
		return;
	}	
	if(Command_Value == 0xff) {
		FP_Cmd_GPVendorCmdMP();
		return;
	}	
			
	FP_CommandFail(0x1A);			
}


void FP_CommandFail(int nSenseIndex)
{
	unsigned int unCnt;//, i;
	unsigned int ret;
	CSW_Status=1;
	Sense_Code = nSenseIndex;//*3;

	if (!CSW_Residue)
		return;
	
	if (SCSI_Direct_Write)	
	{
		unCnt = CSW_Residue/512;
		while(unCnt)
		{			
			FP_RxUSB_RW_Buffer(512);
			unCnt--;
			if(tra_end=0xffff)
				break;
		}	
		//Remain
		unCnt = CSW_Residue%512;
		if (unCnt)
			FP_RxUSB_RW_Buffer(unCnt);
//		if(ret==0xffff)
//			break;
	}			
	else 
		Bulk_Stall = Bulk_IN_Stall;		
	
	CSW_Residue = 0;	

}
////////////////////////////////////////////////////////////////////////////////
//Return 1: Media present
//		 0: Media not present
////////////////////////////////////////////////////////////////////////////////
unsigned int FP_CheckLunStatus(unsigned long  lUSB_LUN_Info_CMP)
{
	str_USB_Lun_Info*  ptr_USB_LUN_Info_CMP = (str_USB_Lun_Info*)lUSB_LUN_Info_CMP;
	
	if (ptr_USB_LUN_Info_CMP->unLunType <= 1)//nand flash
	{
		if ((!R_NAND_Present)||(ptr_USB_LUN_Info_CMP->unStatus==LunStatus_NoMedia)||
			((ptr_USB_LUN_Info_CMP->unLunType == LunType_CDROM) && CDROMDelay))
		{
			FP_CommandFail(0x0B);	//NoMedia
			return 0;	
		}
		if (ptr_USB_LUN_Info_CMP->unStatus&LunStatus_MediaChange)
		{
			ptr_USB_LUN_Info_CMP->unStatus ^= LunStatus_MediaChange;
			FP_CommandFail(0x22);	//Media change
			return 0;	
		}
	}
	
	if (ptr_USB_LUN_Info_CMP->unLunType == LunType_SDCardReader)//SD card
	{
		if ((!R_SDC_Present)||(ptr_USB_LUN_Info_CMP->unStatus==LunStatus_NoMedia))
		{
			FP_CommandFail(0x0B);	//NoMedia
			return 0;	
		}
		if (ptr_USB_LUN_Info_CMP->unStatus&LunStatus_MediaChange)
		{
			ptr_USB_LUN_Info_CMP->unStatus ^= LunStatus_MediaChange;
			FP_CommandFail(0x22);	//Media change
			return 0;	
		}
	}				
	return 1;	
}

void FP_Cmd_TestUnitReady()
{
	int i;
	
	CSW_Residue=0;
	
//rhm	if (TestUnitReady_CallBack())
//rhm		return;
	
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = FP_USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			FP_CheckLunStatus((unsigned long)ptr_USB_LUN_Info);
			break;
			
/*			if (ptr_USB_LUN_Info->unLunType <= 1)//nand flash
			{
				if (!R_NAND_Present || ((ptr_USB_LUN_Info->unLunType == LunType_CDROM) && CDROMDelay))
				{	
					CSW_Status=1;
					Sense_Code=0x0B;			//0x0B - MEDIUM NOT PRESENT
				}
			}
			if (ptr_USB_LUN_Info->unLunType == LunType_SDCardReader)//SD card
			{
				if (!R_SDC_Present)
				{	
					CSW_Status=1;
					Sense_Code=0x0B;			//0x0B - MEDIUM NOT PRESENT
				}
			}		*/		
			break;
		}
	}				
}

void FP_Cmd_RequestSense()
{
	FP_Request_Sense(Sense_Code*3);
	CSW_Residue=0;
	CSW_Status=0;
}

void FP_Cmd_Inquiry()
{
	int i;
		
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = FP_USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
//			if (CDROMEnable && CDROMDelay)	//enable CDROM delay timer
//				CDROMTimerStartup();

			FP_Inquiry(ptr_USB_LUN_Info->unLunType);
			break;
		}
	}
	CSW_Residue=0;
	CSW_Status=0;	
}


void FP_Cmd_ModeSense6()
{
	int i;
		
//rhm	if (FP_ModeSense6_CallBack())
//rhm		return;
	
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = FP_USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			if (FP_CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{
				if (ptr_USB_LUN_Info->unLunType == LunType_SDCardReader)
					USBWriteProtect = R_Write_protect ? 0x80 : 0; //R_Write_protect is card reader write protect
				else if (ptr_USB_LUN_Info->unStatus & LunStatus_WriteProtect)
					USBWriteProtect = 0x80;
				else
					USBWriteProtect = 0x00;
				
				FP_Mode_Sense_06();
			    Bulk_Stall = Bulk_IN_Stall;
			    CSW_Residue-=4;
				CSW_Status=0;							
			}
			break;
		}
	}	
}

void FP_Cmd_StartStop()
{
	CSW_Residue=0;
	CSW_Status=0;
}

void FP_Cmd_MediumRemoval()
{
	FP_Prevent_Allow_Medium_removal();
}

void FP_Cmd_FormatCapacity()
{
	int i;
		
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = FP_USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			if (FP_CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{	
				if (ptr_USB_LUN_Info->unLunType == LunType_SDCardReader)
					FP_Read_Format_Capacities(SDCTotalSector);
				else
			    	FP_Read_Format_Capacities(ptr_USB_LUN_Info->ulNandSizeSec);
	
				if(CSW_Residue > 20)
				{
					// reply stall
					Bulk_Stall = Bulk_IN_Stall;
					Sense_Code=0x1C;	//05 24 00	//INVALID FIELD IN COMMAND PACKET
					CSW_Residue-=20;
					CSW_Status=0x00;
				}
				else
				{
					CSW_Residue=0;
					CSW_Status=0;
				}
			}
			break;
		}
	}	
}

void FP_Cmd_ReadCapacity()
{
	int i;	
	
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = FP_USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			if (FP_CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{	
				if (ptr_USB_LUN_Info->unLunType == LunType_SDCardReader)
					FP_Read_Capacity(SDCTotalSector);
				else
				{
					if (ptr_USB_LUN_Info->unLunType == LunType_CDROM)	//CDROM not ready
					{						
						if (CDROMDelay)
						{
							Bulk_Stall=Bulk_IN_Stall;
							Sense_Code=0x0B;	//media not present
							CSW_Status=0x01;
							return;
						}
						else
						{
							FP_Read_CDROM_Capacity(ptr_USB_LUN_Info->ulNandSizeSec);
							return;	
						}	
					}						
			    	FP_Read_Capacity(ptr_USB_LUN_Info->ulNandSizeSec);
				}
			}
			break;
		}
	}	
}

void FP_Cmd_Read()
{
	int i;	
	
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = FP_USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			if (FP_CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{	
				CSW_Status=0;

				if (FP_Read_10((unsigned long)ptr_USB_LUN_Info))
				{
					Bulk_Stall=Bulk_IN_Stall;
					CSW_Status=1;
				}

			}
			break;
		}
	}	
}

void FP_Cmd_Write()
{
	int i;
		
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = FP_USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			CSW_Status=0;
			//check write protect
			if (((ptr_USB_LUN_Info->unLunType == LunType_SDCardReader) && R_Write_protect)||
				(ptr_USB_LUN_Info->unStatus & LunStatus_WriteProtect))
			{
				FP_CommandFail(0x25);
				return;
			}
			//Check CDROM
			if (ptr_USB_LUN_Info->unLunType == LunType_CDROM)	//CDROM write command, check CDROM first
			{
				if ((FP_GetCMDValueEx(27)!='G')||(FP_GetCMDValueEx(28)!='P')||(FP_GetCMDValueEx(29)!='C')||(FP_GetCMDValueEx(30)!='D'))		
				{
					FP_CommandFail(0x25);
					return;
				}
				if (FP_Write_10((unsigned long)ptr_USB_LUN_Info))
					FP_CommandFail(Sense_Code);	
			}
			else if (FP_CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{	
				if (FP_Write_10((unsigned long)ptr_USB_LUN_Info))
					FP_CommandFail(Sense_Code);	
					
			}
			break;
		}
	}	
}

void FP_Cmd_Verify()
{
	CSW_Residue=0;
	CSW_Status=0;
}

void FP_Cmd_ReadTOC()
{
	unsigned long USB_RW_Buffer_PTR;
	
	
	if (FP_CheckLunStatus((unsigned long)ptr_USB_LUN_Info))	
	{
/*	if ((!CDROMEnable) || CDROMDelay)
	{
		Bulk_Stall=Bulk_IN_Stall;
		CSW_Status=1;
		Sense_Code=0x0B;			//0x0B - MEDIUM NOT PRESENT
		return;			
	}
	*/
/*	for(i=0; i<(sizeof(USB_Lun_Define)/sizeof(str_USB_Lun_Info)); i++)
	{
		ptr_USB_LUN_Info = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunType == LunType_CDROM)
		{*/
			USB_RW_Buffer_PTR=FP_GetBufferPointer();
//			_NAND_ReadSector(ptr_USB_LUN_Info->ulNandStartAddr, 1, (unsigned long)USB_RW_Buffer_PTR);
/*			break;
		}
	}	*/		
	}
	switch(FP_GetCMDValueEx(24) & 0xC0)
	{
	case 0x00:
		FP_TxUSB_RW_Buffer_Word(CSW_Residue, 128/2);
		break;
	case 0x40:
		FP_TxUSB_RW_Buffer_Word(CSW_Residue, 192/2);
		break;				
	case 0x80:
		FP_TxUSB_RW_Buffer_Word(CSW_Residue, 256/2);
		break;
	default:
		FP_CommandFail(0x1C);
	}				
}

void FP_Cmd_ModeSense10()
{
	int nVendorCmd;
	
	nVendorCmd = FP_GetCMDValueEx(17);
	
	if((nVendorCmd==5) || (nVendorCmd==63))
	{
		FP_ReplyFlexPage();	
		return;
	}
	
	if (FP_CheckLunStatus(ptr_USB_LUN_Info))
	{	
		if(nVendorCmd == 0x2A)
		{
			FP_ModeSenseData();
			return;	
		}
	}
	Bulk_Stall=Bulk_IN_Stall;
	CSW_Status=1;
	Sense_Code=0x0B;			
	
}

void FP_Cmd_GPVendorCmd()
{
	int i, rhm = 0;
/*RHM	SCSI_CMD_MAP	*ScsiCmdPtr;
	SCSI_CMD_MAP FP_GPVScsiCmdMapList[] = {
		{0x00, (void*)FP_ReadDriveInfoTable},	
		{0x01, (void*)FP_GetDriveStatus},	
		{0x10, (void*)FP_GetICVersion}	
	};				
RHM*/	
	if (FP_GetCMDValueEx(29)!='G' || FP_GetCMDValueEx(30)!='P')
	{
		FP_CommandFail(0x27);					
		return;		
	}
	
	Command_SecValue = FP_GetCMDValueEx(16);
	
	if(Command_SecValue == 0) {
		FP_ReadDriveInfoTable();
		return;
	}
	if(Command_SecValue == 1) {
		FP_GetDriveStatus();
		return;
	}
	if(Command_SecValue == 0x10) {
		FP_GetDriveStatus();
		return;
	}
	
/*RHM	for(i=0; i<(sizeof(FP_GPVScsiCmdMapList)/sizeof(SCSI_CMD_MAP)); i++)
	{
		ScsiCmdPtr = &FP_GPVScsiCmdMapList[i];
		if(ScsiCmdPtr->ScsiCmdCode == Command_SecValue){					//+15: vendor command item code
			ScsiCmdPtr->function();
			return;
		}
	}
	rhm*/
	
	rhm = Command_SecValue;		
			
	FP_CommandFail(0x1A);			
	
}

void
FP_RHMJump()
{
	void (*P)(); 
	short *memptr = 0;
	short *p = &memptr;
	
	*(p+1) = (short) (FP_GetCMDValueEx(17) << 24);
	*(p+1) |= (short) (FP_GetCMDValueEx(18) << 16);
	*p |= FP_GetCMDValueEx(19) << 8;
	*p |= FP_GetCMDValueEx(20);
	
	Sense_Code = 0;
	CSW_Residue=0;
	CSW_Status=0;
	USB_Status =  BULK_IN_NACK;  //TEST	
	if(R_USB_State_Machine != SCSI_CSW_Stage) //test
		R_USB_State_Machine = SCSI_CSW_Stage; //test
		
	P = memptr;
	ReprogJump = P;
	
//	if((unsigned long) P >= 0xac000)
//		return;
}

void FP_RHMWrite()
{
	short *memptr = 0;
	short length, memtype, s;
	short *p = &memptr;
	short lun;
	
	*(p+1) = (short) (FP_GetCMDValueEx(17) << 24);
	*(p+1) |= (short) (FP_GetCMDValueEx(18) << 16);
	*p |= FP_GetCMDValueEx(19) << 8;
	*p |= FP_GetCMDValueEx(20);
	
	memtype = FP_GetCMDValueEx(21);
	
	length = FP_GetCMDValueEx(22) << 8;
	length |= FP_GetCMDValueEx(23);
	
	s = length;
	
	if(memtype == LunType_RAM)
		lun = 1;
	else
		lun = 2;  //nor flash
	
	if (FP_Write_10((unsigned long)FP_USB_Lun_Define[lun]))
		FP_CommandFail(Sense_Code);	

}

void FP_Cmd_GPVendorCmdMP()
{
	int i, rhm = 0;
/*RHM	SCSI_CMD_MAP	*ScsiCmdPtr;
	SCSI_CMD_MAP FP_MPScsiCmdMapList[] = {
		{0xF0, (void*)FP_SetVenderID},
		{0x2a, (void*)FP_RHMWrite},
		{0xff, (void*)FP_RHMJump}
	};				
RHM*/		
	if (FP_GetCMDValueEx(29)!='G' || FP_GetCMDValueEx(30)!='P')
	{
		FP_CommandFail(0x27);					
		return;		
	}
	
	Command_SecValue = FP_GetCMDValueEx(16);

	if(Command_SecValue == 0xf0) {
		FP_SetVenderID();
		return;
	}
	if(Command_SecValue == 0x2a) {
		FP_RHMWrite();
		return;
	}
	if(Command_SecValue == 0xff) {
		FP_RHMJump();
		return;
	}
/*RHM	for(i=0; i<(sizeof(FP_MPScsiCmdMapList)/sizeof(SCSI_CMD_MAP)); i++)
	{
		ScsiCmdPtr = &FP_MPScsiCmdMapList[i];
		if(ScsiCmdPtr->ScsiCmdCode == Command_SecValue){					//+15: vendor command item code
			ScsiCmdPtr->function();
			return;
		}
	}	
RHM*/		
	rhm = Command_SecValue;
		
	FP_CommandFail(0x1A);			
	
}

void FP_GetDriveStatus()
{
	Command_Value = FP_GetCMDValueEx(17);

	switch(Command_Value)
	{
		case 0x00:	//the lun number that operating now
			*P_USBD_BIData = CBWLUN;
			*P_USBD_EPEvent = 0x0100;
		break;

		case 0x01:
			break;

		default:
			FP_CommandFail(0x1A);			
			break;
	}		
}

void FP_USB_ISR()
{ 
	int a;
	
	if (*P_USBD_INT){						//USB device interrupt event happened.
				
		if (*P_USBD_INT & BIT0){		// USB Device Receive Setup Packet
			*P_USBD_INT = BIT0; 			// Set EP0_State to EP0_Setup_Stage

			R_USB_State_Machine=EP0_SETUP_Stage;
		}
		
		else if	(*P_USBD_INT & BIT3){		// EP0 IN Packet Clear INT
			*P_USBD_INT = BIT3; 			// This bit is set when IN PKT is read from the host
			*P_USBD_EPEvent = 0x0040;		// Then Enable EP0 Status Stage
		}
		
		else if	(*P_USBD_INT & BIT4){		// USB Host Send IN Token
			USB_Status =  EP0_IN_NACK;		// Then set USB_Status=EP0_IN_NACK
			a=*P_USBD_INTEN;				// Disable EP0_IN_NACK INT
			*P_USBD_INTEN=a & 0xffef;
			*P_USBD_INT = BIT4; 			// But Device reply NACK
		}
		
		else if	(*P_USBD_INT & BIT5){		// USB Host Start EP0 Status Stage, and
			*P_USBD_INT = BIT5; 			// Device has finished this stage.
			R_USB_State_Machine=0;
//			R_EP0DataCount = 0;				//huck: Modify to fix cable too long or connect with a long cable hub
		}
		
		else if	(*P_USBD_INT & BIT8){		// USB Host Send Bulk IN Token
								 			// But Device reply NACK
			USB_Status =  BULK_IN_NACK;
			
			a=*P_USBD_INTEN;				// Disable Bulk_IN_NACK INT
			*P_USBD_INTEN=a & 0xfeff;
			*P_USBD_INT = BIT8;				// Clear Bulk IN NACK Interrupt Flag
		}
		
		else if	(*P_USBD_INT & BIT10){		// USB Host Send Bulk OUT PKT
							 				// But Device reply NACK
			USB_Status =  BULK_OUT_NACK;	// Then set USB_Status=Bulk_OUT_NACK
			R_USB_State_Machine=SCSI_CBW_Stage;
			a=*P_USBD_INTEN;				// Disable Bulk_OUT_NACK INT
			*P_USBD_INTEN=a & 0xfbff;
			*P_USBD_INT = BIT10;			// Clear Bulk OUT Interrupt Flag
		}
		
		else if(*P_USBD_INT & BIT13	){		// USB Suspend
			a=*P_USBD_INTEN;
			a=a | 0x4000;					// Enable Resume Interrupt
			a=a & 0xdfff;					// Disable Suspend Interrupt		
			*P_USBD_INTEN=a;
			USBD_PlugIn = 0;
			*P_USBD_INT = BIT13;
//R_USB_Suspend=1;			
			FP_USBDSuspend_CallBack();			//user could do something
		}
		
		else if(*P_USBD_INT & BIT14){			// USB Resume
			*P_USBD_INT = BIT14;
			a=*P_USBD_INTEN;
			a=a | 0x2000;					// Enable Suspend Interrupt
			a=a & 0xbfff;					// Disable Resume Interrupt
			*P_USBD_INTEN=a;
			R_USB_Suspend=0;
		}
		
		else if(*P_USBD_INT & BIT15){			// USB Reset
			*P_USBD_INT = BIT15; 	
			R_USB_Suspend=0;
			USBD_PlugIn = 1;
			FP_USB_Reset();
		}
	
	}	
}

void FP_USB_Reset(void)
{
	if (R_EP0DataCount)
		R_EP0DataCount = 0;

	R_USB_State_Machine=0;
	USB_Status=0;
	Bulk_Stall=0;
	R_USB_Suspend=0;
	*P_USBD_EPBufClear = 0x000F;	//huck add
	*P_USBD_INTEN=0x8429;

}

void  FP_USB_ServiceLoop(unsigned int unUseLoop)
{
	unsigned a;
//	unsigned int FP_RHM_USBreprogBuf[2048];
	int i, reprog_delay;

	#ifdef print	
	init_uart();
	print1("\n\n\n\n\n/// SPL162001 USB DEVICE MSDC Program Start ///\n");
	#endif
		
	reprog_delay = 0;
	
	while(1)
	{

		
//b 		switch (R_USB_State_Machine)
//b		{
			
//b			case EP0_SETUP_Stage:
			if(R_USB_State_Machine == EP0_SETUP_Stage)
			{
				R_USB_State_Machine=EP0_Data_Stage;
				FP_Setup_Token();
			
				USB_Status=0x00;
				a=*P_USBD_INTEN;				// Enable EP0_IN_NACK INT
				*P_USBD_INTEN=a|0x0010;
				*P_USBD_INT=0x0010;				// Clear EP0_IN_NACK	// added by Anson 2005/06/30 for verify test				
//b				break;
			} else if (R_USB_State_Machine == EP0_Data_Stage) {
//b			case EP0_Data_Stage:
				if(USB_Status==EP0_IN_NACK)		// When USB_Status = EP0 IN NACK
				{								// Start EP0 Data Stage
					FP_Write_EP0_FIFO();					
					USB_Status=0x00;
				}			
//b				break;
			
			} else if (R_USB_State_Machine == SCSI_CBW_Stage) {
//b			case SCSI_CBW_Stage:
				if(USB_Status==BULK_OUT_NACK)
				{
					*P_USBD_EPEvent=0x0800;		// Enable BULK OUT Packet
												// After Enabling this bit,
												// the data sent from USB Host
												// will load into Bulk OUT
												// Data Register.
					ulTimeout = 0x4FFFF;
					while((*P_USBD_EPEvent & 0x1000) == 0)	// Wait until packet load 
					{										// into Bulk OUT FIFO
						if (!(ulTimeout--))
							break;																
					}
					a=*P_USBD_EPEvent & 0x0800;	// 
					#ifdef print	
					if(a != 0)	print1("Error! Bulk OUT EN should be 0 !!\n");
					#endif
					
					FP_Receive_CBW_PKT();
					*P_USBD_EPEvntClear=0x0010;
					
					USB_Status=0x00;			// Clear USB_Status to zero
					
//					SCSI_State=SCSI_Data_Stage;	
					R_USB_State_Machine=SCSI_Data_Stage;
				}	
			
//b				break;
			} else if (R_USB_State_Machine == SCSI_Data_Stage) {
		
//b			case SCSI_Data_Stage:
				*P_USBD_EPEvent=0x2400;		// Clear Bulk IN / OUT NACK
				FP_SCSI_Command();
				
				/////////////////////////////////////////////////////
				if(Bulk_Stall==Bulk_IN_Stall)
				{
					*P_USBD_EPEvent=0x0400;		// Bulk IN NACK Reset
			
					FP_RD_Until_Bulk_IN_NACK();	// Read until Bulk IN NACK
					#ifdef print
					print1("Bulk IN Stall");
					#endif
					a=*P_USBD_EPStall;
					#ifdef print	
					print2("Stall bit : ",1,a);
					print1("Enable Bulk IN Stall\n");
					#endif
					*P_USBD_EPSetStall=0x0002;	// Enable Bulk IN Stall
				
					a=*P_USBD_EPStall & 0x0200;		// wait until device reply stall 
					
					ulTimeout = 0x4FFFF;
					while(a==0)
					{
						if (!(ulTimeout--))
							break;																
						a=*P_USBD_EPStall & 0x0200;
					}

					*P_USBD_EPStall=0x0200;			// clear Bulk IN Stall Flag
					
					*P_USBD_EPSetStall=0x0000;	// Disable Bulk IN Stall
		
					Bulk_Stall=0;
				}
				else if(Bulk_Stall==Bulk_OUT_Stall)
				{
					*P_USBD_EPSetStall=0x0004;	// Enable Bulk OUT Stall
				
					a=*P_USBD_EPStall & 0x0400;		// wait until device reply stall 
					
					ulTimeout = 0x4FFFF;
					while(a==0)
					{
						if (!(ulTimeout--))
							break;																
						a=*P_USBD_EPStall & 0x0400;
					}
					
					*P_USBD_EPStall=0x0400;			// clear Bulk OUT Stall Flag
					
					*P_USBD_EPSetStall=0x0000;	// Disable Bulk OUT Stall
				
					Bulk_Stall=0;
				}
				/////////////////////////////////////////////////////
				
				a=*P_USBD_INTEN;				// Enable Bulk_IN_NACK INT
				*P_USBD_INTEN=a|0x0100;
				
//				SCSI_State=SCSI_CSW_Stage;
				R_USB_State_Machine=SCSI_CSW_Stage;
			
//b				break;
				
			} else if (R_USB_State_Machine == SCSI_CSW_Stage) {
//b			case SCSI_CSW_Stage:
				if(USB_Status==BULK_IN_NACK)
				{
//					print1("SCSI CSW Stage\n");
					
					FP_SCSI_CSW();
					
					USB_Status=0x00;
					
					*P_USBD_EPEvent=0x2000;			// Clear Bulk OUT NACK
		
					a=*P_USBD_INTEN;				// Enable Bulk OUT NACK Interrupt
					a |= 0x0400;					// Used fo SCSI CBW Stage
					*P_USBD_INTEN=a;
		
//					SCSI_State=SCSI_CBW_Stage;
					R_USB_State_Machine=0;
					
					if (R_USB_RW_Busy)				//off read/write busy, user use this flag to know device is read/write busy
						R_USB_RW_Busy = 0;

//					if (unGotoRamStart)
//					{
//						unsigned long ulAddress;
//						unGotoRamStart = 0;	
//						ulAddress = GetLBA();
//						GotoRam(ulAddress);
//					}
				}
			
//b				break;
			} else {
				
				
//b			default:
//b				break;
		}
		
		if(ReprogJump != 0) {
			if(++reprog_delay > 1000) {
				REPROG_IN_Progress = 0;
				(*ReprogJump)();
				// below not reached
				ReprogJump = 0;	
				reprog_delay = 0;
			}
		}	
	
		if(R_USB_Suspend == 1)
		{
//			EnterHaltMode();
			break;
		}
		if(RHM_USBreprogBuf_Full != 0) {
			flash *pflash = RHM_FlashPtr;
			
			__asm__("irq off");
			__asm__("fiq off");
//rhm1			
			(*RHM_FlashPtr->erasesector)(pflash);
//rhm1			
			for(i=0; i<RHM_USBreprogBuf_Full; i++) {
				{
					extern long RHM_DEBUG_LAST_WRITE;
					RHM_DEBUG_LAST_WRITE = RHM_FlashPtr->pflash + i;
				}
				(*RHM_FlashPtr->writeword)(RHM_FlashPtr, RHM_FlashPtr->pflash + i, RHM_FlashBuf[i]);
	   			//Nor_WordWrite(RHM_FlashAddr, RHM_USBreprogBuf_Full, (unsigned short)RHM_USBreprogBuf);
	   			//WriteWord(RHM_FlashPtr, RHM_FlashPtr->pflash + i, RHM_USBreprogBuf[i]);
			}
	   		
	   		__asm__("irq on");
			__asm__("fiq on");

			RHM_USBreprogBuf_Full = 0;
		}
		
		FP_Check_SDC_Present();
		if (!unUseLoop)
			break;
	}	
//	RHM_FlashBuf = 0;
}		

#if 0
void FP_CheckCDRomValid(void)
{
	int i, *USB_RW_Buffer_PTR;
	
	if (R_NAND_Present)
	{
		USB_RW_Buffer_PTR = FP_GetBufferPointer();
		
		for(i=0; i<N_USB_LUN; i++)
		{
			ptr_USB_LUN_Info = FP_USB_Lun_Define[i];
			if(ptr_USB_LUN_Info->unLunType == LunType_CDROM)
			{
				
//				_NAND_ReadSector(ptr_USB_LUN_Info->ulNandStartAddr, 1, (unsigned long)USB_RW_Buffer_PTR);
				
				if ((*USB_RW_Buffer_PTR) == 0x4547  && (*(USB_RW_Buffer_PTR+1)) == 0x504E && (*(USB_RW_Buffer_PTR+2)) == 0x554C)//"GENPLU"
					CDROMEnable = 1;
			}
		}			
	}			
}
#endif

void FP_Setup_Token(void)
{
	unsigned int i;
	
	*P_USBD_EPEvent=0x0001;			// Clear Setup Packet Ready
	USB_Status=0x00;				// added by Anson 2005/06/28 for verify test
	if(*P_USBD_EP0RTR==0x80){
		switch((0xff00 & *P_USBD_EP0VR)){
			
			//////////////////	Get Device Descriptor	///////////////////
			case 0x0100:			// Device Descriptor
				i=FP_Get_Descriptor_Length(Device_Descriptor_Table);
				if(*P_USBD_EP0LR > i)
					R_EP0DataCount=i;			// Assign Data Count
				else
					R_EP0DataCount=*P_USBD_EP0LR;
					if (!FP_EP0_DataPointer_CallBack(Device_Descriptor_Table))
						FP_DataPointer(Device_Descriptor_Table);				// Assign Data Pointer
					
					*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
//					EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
					#ifdef print
//					print1("Get Device Descriptor\n");
					#endif
				break;
				
			
			//////////////////	Get Config Descriptor	///////////////////
			case 0x0200:			// Configuration Descriptor
				
				i=*P_USBD_INTEN;				// Enable Suspend Interrupt
				i |= 0x2000;					//
				*P_USBD_INTEN=i;
		
				i=FP_Get_Descriptor_Length(Config_Descriptor_Table);
				if(*P_USBD_EP0LR > i)
					R_EP0DataCount=i;
				else
					R_EP0DataCount=*P_USBD_EP0LR;
					
					if (!FP_EP0_DataPointer_CallBack(Config_Descriptor_Table))
						FP_DataPointer(Config_Descriptor_Table);				// Assign Data Pointer
					
					*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
					
//					EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
					#ifdef print	
//					print1("Get Config Descriptor\n");
					#endif
				break;
			
			
			//////////////////	Get String Descriptor	///////////////////
			case 0x0300:			// String Descriptor
//b				switch(*P_USBD_EP0VR & 0x00ff) {
//b					case 0x0000:
					if((*P_USBD_EP0VR & 0x00ff) == 0) 
					{
						i=FP_Get_Descriptor_Length(String0_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;
	
						if (!FP_EP0_DataPointer_CallBack(String0_Descriptor_Table))
							FP_DataPointer(String0_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 0 Descriptor\n");
						#endif
						
//b						break;
					} else if ((*P_USBD_EP0VR & 0x00ff) == 1) {
						
//b					case 0x0001:
						i=FP_Get_Descriptor_Length(String1_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;
							
						if (!FP_EP0_DataPointer_CallBack(String1_Descriptor_Table))					
							FP_DataPointer(String1_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 1 Descriptor\n");
						#endif
//b						break;
					} else if ((*P_USBD_EP0VR & 0x00ff) == 2) {				
//b					case 0x0002:
						i=FP_Get_Descriptor_Length(String2_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;
					
						if (!FP_EP0_DataPointer_CallBack(String2_Descriptor_Table))						
							FP_DataPointer(String2_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 2 Descriptor\n");
						#endif
//b						break;
					} else if ((*P_USBD_EP0VR & 0x00ff) == 3) {						
//b					case 0x0003:
						i=FP_Get_Descriptor_Length(String3_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;
					
						if (!FP_EP0_DataPointer_CallBack(String3_Descriptor_Table))
							FP_DataPointer(String3_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 3 Descriptor\n");
						#endif
//b						break;
					} else if ((*P_USBD_EP0VR & 0x00ff) == 4) {							
//b					case 0x0004:
						i=FP_Get_Descriptor_Length(String4_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;

						if (!FP_EP0_DataPointer_CallBack(String4_Descriptor_Table))					
							FP_DataPointer(String4_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 4 Descriptor\n");
						#endif
//b						break;
						
//b					default:
					} else {
						#ifdef print	
						print1("Not Support    Check (String)\n");
						#endif
						FP_NotSupport();
//b						break;
					}	
//b				}
				
				break;

/*
			//////////////////	Get Interface Descriptor	///////////////////
			case 0x0400:			// Interface Descriptor
				#ifdef print	
				print1("Get Interface Descriptor\n");
				#endif
			
				break;
			
			//////////////////	Get Endpoint Descriptor	///////////////////
			case 0x0500:			// Endpoint Descriptor
				#ifdef print	
				print1("Get Endpoint Descriptor\n");
				#endif
			
				break;
*/			
			default:
				FP_NotSupport();
				#ifdef print	
				print1("Not Support    Check\n");
				#endif
				break;
		
		
		
		}
	}
	else if(*P_USBD_EP0RTR==0xA1 && *P_USBD_EP0RR==0xFE && *P_USBD_EP0VR==0x00){
		R_EP0DataCount=*P_USBD_EP0LR;
		FP_DataPointer(MaxLUN_Table);
		#ifdef print	
		print1("Get Max LUN\n");
		#endif
		*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK
		while((*P_USBD_EPEvent & 0x0020) == 0);
		FP_Write_EP0_FIFO();
		*P_USBD_EPEvent = 0x0040;				// Then Enable EP0 Status Stage
		
		//////////////////////////////////////////////////////////////
		// Change to USB MSDC SCSI Stage
//		SCSI_State=SCSI_CBW_Stage;
		R_USB_State_Machine=SCSI_CBW_Stage;
		
		*P_USBD_EPEvent=0x2000;			// Clear Bulk OUT NACK
		
		i=*P_USBD_INTEN;				// Enable Bulk OUT NACK Interrupt
		i |= 0x0400;					// Used fo SCSI CBW Stage
		*P_USBD_INTEN=i;
	}
	else{
		FP_NotSupport();
		#ifdef print	
		print1("Not Support    Other Setup Command\n");
		#endif
		
	}	
	
	
}

void FP_Write_EP0_FIFO()
{
	int a;
	
	if(R_EP0DataCount==0){
		// reply Null Packet
		FP_Send_Null();
		#ifdef print	
		print1("Send Null Packet\n");
		#endif
		return;
	}
	
	while (R_EP0DataCount)		//huck: Modify to fix cable too long issue or connect with a long cable hub
	{
		if(R_EP0DataCount >= 8)
		{
			FP_Send_8_Bytes();
	
			a=*P_USBD_INTEN;			// Enable EP0_IN_NACK INT
			*P_USBD_INTEN=a|0x0010;
			*P_USBD_INT=0x0010;			// Clear EP0_IN_NACK	// added by Anson 2005/06/30 for verify test

//*P_IOA_Data=0xF000;				
	//		print1("Send 8 Bytes\n");
		}
		else if(R_EP0DataCount <8)
		{
			FP_Send_Remain_Bytes();
//*P_IOA_Data=0x0000;							
	//		print1("Send Remain Bytes\n");
		}
	}	
	return;
}



void FP_NotSupport()
{
	int a;
	
	// reply stall	
	*P_USBD_EPSetStall=0x0001;	// Enable EP0 Stall
				
	a=*P_USBD_EPStall & 0x0100;		// wait until device reply stall 

	ulTimeout = 0x2FFFF;
	while(a==0)
	{
		if (!(ulTimeout--))
			break;																
		a=*P_USBD_EPStall & 0x0100;
	}
	*P_USBD_EPStall=0x0100;			// clear EP0 Stall Flag
					
	*P_USBD_EPSetStall=0x0000;	// Disable EP0 Stall
	R_USB_State_Machine=0;
}


void FP_fUSB_SDCardPlugIn()
{
	int	i;
	str_USB_Lun_Info*  ptr_USB_LUN_Info_SD;

	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info_SD = FP_USB_Lun_Define[i];
		if(ptr_USB_LUN_Info_SD->unLunType == LunType_SDCardReader)
		{
			ptr_USB_LUN_Info_SD->unStatus = LunStatus_Normal | LunStatus_MediaChange;						
			break;
		}
	}
}

void FP_fUSB_SDCardPullOut()
{
	int	i;
	str_USB_Lun_Info*  ptr_USB_LUN_Info_SD;

	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info_SD = FP_USB_Lun_Define[i];
		if(ptr_USB_LUN_Info_SD->unLunType == LunType_SDCardReader)
		{
			ptr_USB_LUN_Info_SD->unStatus = LunStatus_NoMedia ^ LunStatus_WriteProtect;						
			break;
		}
	}
}
const unsigned int FP_szKeyTable4[16]={0xE7,0x53,0xED,0x50,0x82,0x68,0x93,0x29,0x88,0x6F,0x34,0x89,0xB7,0x1E,0x19,0xDC};

const unsigned int FP_szKeyTable1[16]={0xBB,0x8E,0xD9,0xAE,0x92,0x99,0x1D,0xB3,0xC7,0x58,0x33,0x98,0x3A,0xD4,0xC1,0x69};

const unsigned int FP_szKeyTable3[16]={0xE1,0x69,0x7F,0x6E,0x08,0x22,0xAA,0xC0,0x53,0x4F,0xF0,0x65,0x7A,0x4B,0xD9,0x5D};

const unsigned int FP_szKeyTable2[16]={0xB1,0xA9,0x66,0xE9,0x81,0x39,0x59,0xC9,0xE5,0xAC,0x02,0x9E,0x06,0x43,0xC7,0x58};

					
					
int FP_SetVenderID()
{
	unsigned int unTmp, unTmp1, unBase, unKeyID, unShiftByte, unShiftBit, unKeyBuffer[81], unGetData, unReplyData[11];
	int			 i, nRlt=0;
	
	unTmp = R_CBW_PKT[18]^0x5E;
	
	unTmp1 = R_CBW_PKT[18+4];

	if (unTmp1 != unTmp)
	{
		for (i=0; i<10; i++)
			*P_USBD_BIData = 0;
		
		goto TXBUFFER;
	}
	
	unTmp = R_CBW_PKT[18+1] + R_CBW_PKT[18+3];
	unTmp1 = R_CBW_PKT[18+5] + R_CBW_PKT[18+7];
	unBase = (unTmp ^ unTmp1)&0xFF;
	
//	unTmp = R_CBW_PKT[18+9]^unBase;
//	unTmp1 = (R_CBW_PKT[18+8]^unBase)<<8;

	unKeyID = R_CBW_PKT[18+9]^unBase + ((R_CBW_PKT[18+8]^unBase)<<8);
	unKeyID %= 512;
	unShiftByte = unKeyID/8;
	unShiftBit = unKeyID%8;	
	
	FP_memcpy(unKeyBuffer, FP_szKeyTable4, 16);
	FP_memcpy(unKeyBuffer+16, FP_szKeyTable1, 16);
	FP_memcpy(unKeyBuffer+32, FP_szKeyTable3, 16);
	FP_memcpy(unKeyBuffer+48, FP_szKeyTable2, 16);			
	FP_memcpy(unKeyBuffer+64, FP_szKeyTable4, 16);				
	
	unBase = ((R_CBW_PKT[18] + R_CBW_PKT[18+2])^(R_CBW_PKT[18+4] + R_CBW_PKT[18+6]))&0xFF;

	//Get key table value	
	for (i=unShiftByte; i<(unShiftByte+8); i++)
	{
		unGetData = unKeyBuffer[i]*0x100 + unKeyBuffer[i+1];
		unReplyData[i-unShiftByte] = (unGetData<<unShiftBit)>>8;
	}	

	unTmp=2;	
	for (i=7; i>=0; i--)//insert dummy word in 3 and 4 byte
	{
		if (i<3)
			unTmp=0;		
			
		unReplyData[i+unTmp] = unReplyData[i] ^ unBase;
		if (i==3)
			unReplyData[i] = R_CBW_PKT[4]^R_CBW_PKT[18+7];
		if (i==4)
			unReplyData[i] = R_CBW_PKT[6]^R_CBW_PKT[18+6];
	}

	for (i=0; i<10; i++)
		*P_USBD_BIData = unReplyData[i];
	
	nRlt = 1;
	
TXBUFFER:	
	
	*P_USBD_EPEvent=0x0100;
	return nRlt;
	
}
void FP_VendorCmd_CallBack()
{
	
	FP_CommandFail(0x1A);	//unsupport command
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Return Value:
//	1: Fail, 0: Success
///////////////////////////////////////////////////////////////////////////////////////////////////////
int FP_Read_10(unsigned long  lUSB_LUN_Read)
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

	SCSI_LBA=FP_GetLBA();

	SCSI_Transfer_Length=FP_GetTransLeng();

	USB_RW_Buffer_PTR=FP_GetBufferPointer();
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

				ret = FP_DrvSDCReadMultiSector_USB((unsigned long)SCSI_LBA+i, (unsigned long)USB_RW_Buffer_PTR_A,USBDMA_CHANNEL0,0);
			else 
				ret = FP_DrvSDCReadMultiSector_USB((unsigned long)SCSI_LBA+i, (unsigned long)USB_RW_Buffer_PTR_A,USBDMA_CHANNEL0,1);

			if (i == SCSI_Transfer_Length-1)
				ret = FP_DrvSDCReadMultiSector_USB((unsigned long)SCSI_LBA+i, (unsigned long)USB_RW_Buffer_PTR_A,USBDMA_CHANNEL0,2);


			if(ret != 0x00)
			{
				FP_DrvSDCReadMultiSector_USB((unsigned long)SCSI_LBA+i, (unsigned long)USB_RW_Buffer_PTR_A,USBDMA_CHANNEL0,4); //4,stop the SDC Only					
				Sense_Code=0x08;
				goto End;
			}			

			if (stage)
			{
				ret = FP_Send_To_USB_DMA_USB(USB_RW_Buffer_PTR_A,USBDMA_CHANNEL1,1);
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

			ret = FP_Send_To_USB_DMA_USB(USB_RW_Buffer_PTR_A,USBDMA_CHANNEL1,0);									
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
				ret = FP_Send_To_USB_DMA_USB(USB_RW_Buffer_PTR_A,USBDMA_CHANNEL1,1);

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

int FP_Write_10(unsigned long	lUSB_LUN_Write)
{
	int j;
	unsigned long SCSI_LBA, FP_GetLBA();
	unsigned int SCSI_Transfer_Length;
	unsigned long USB_RW_Buffer_PTR;
	unsigned long USB_RW_Buffer_PTR_A,USB_RW_Buffer_PTR_B,USB_RW_Buffer_PTR_Temp;
	unsigned int USBDMA_CHANNEL0, USBDMA_CHANNEL1, USBDMA_CHANNEL2, USBDMA_CHANNEL3;
	int ret= 0xffff, stage = 0, *p;
	int ret1=0xffff;
	unsigned int Erro_Flag = 0;
	extern unsigned int *RHM_FlashBuf, RHM_USBreprogBuf_Full;
	unsigned int idx;
	extern unsigned int FP_USBreprogBuf[];
	str_USB_Lun_Info*  ptr_USB_LUN_Write;

	ptr_USB_LUN_Write = (str_USB_Lun_Info*)lUSB_LUN_Write;
	R_USB_RW_Busy = 1;
	Sense_Code = 0x00;
	SCSI_LBA = FP_GetLBA();
//	SCSI_LBA=GetLBA();
	SCSI_Transfer_Length=FP_GetTransLeng();
	USB_RW_Buffer_PTR=FP_GetBufferPointer();

	USB_RW_Buffer_PTR_A=FP_GetBufferPointer();   //add by zhangxh 2006/01/19
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
			ret=FP_Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B,USBDMA_CHANNEL3);			
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
		pflash->pflash = SCSI_LBA;
		pflash->bytesAvail = 0;
		{
			extern long RHM_DEBUG_LAST_LBA;
			RHM_DEBUG_LAST_LBA = SCSI_LBA;
			if(RHM_DEBUG_LAST_LBA > 0xbffff) {
			Sense_Code = 0x1B;//LOGICAL BLOCK ADDRESS OUT OF RANGE
			ret = 1;			
			goto End;
			}
		}
		
		//RHM test
//rhm1		if(((unsigned long)pflash->pflash) < 0xac000) {
//rhm1			pflash->erasesector(pflash);
//rhm1		}
		//RHM test

		for(j=0; j<SCSI_Transfer_Length; j++)
		{				
			//=================================================================================================
			// Enable DMA mode. And using USB 128x8 bytes dual FIFO
			//ret=FP_Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B,DMA_CHANNEL2);
//RHM			ret=FP_Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B, USBDMA_CHANNEL1);
		    ret=FP_Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B, USBDMA_CHANNEL3);

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
//			memcpy((unsigned long) &rhmbuf[j*256], (unsigned long)USB_RW_Buffer_PTR_B, 512);
			{
			unsigned short *sp = (unsigned short *)USB_RW_Buffer_PTR_B;
			if(RHM_FlashBuf) {
				for(idx=0; idx<256; idx++) {
					RHM_FlashBuf[(j*256)+idx] = *sp++;
				}
				pflash->bytesAvail += 256;
			}
			}
/*
			p = SCSI_LBA+(j*256);
			
			__asm__("irq off");
			__asm__("fiq off");

           	Nor_WordWrite(p, 256, (unsigned short)USB_RW_Buffer_PTR_B);
           	
			__asm__("irq on");
			__asm__("fiq on");
*/	
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
			RHM_USBreprogBuf_Full = j * 256;
			if(R_USB_State_Machine != SCSI_CSW_Stage) //test
				R_USB_State_Machine = SCSI_CSW_Stage; //test
			USB_Status =  BULK_IN_NACK;  //TEST			
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
			//ret=FP_Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B,DMA_CHANNEL2);
//RHM			ret=FP_Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B, USBDMA_CHANNEL1);
		    ret=FP_Receive_From_USB_DMA_USB(USB_RW_Buffer_PTR_B, USBDMA_CHANNEL3);

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
					FP_memcpy(SCSI_LBA+(j*256), (unsigned long)USB_RW_Buffer_PTR_B, 256);
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


					
