#include "GPL162002.h"
#ifdef USBRP
#include ".\reprog\USBD_Define.h"
#include ".\reprog\USBD.h"
#else
#include ".\Driver\Include\USB\USBD_Define.h"
#include ".\Driver\Include\USB\USBD.h"
#endif

#ifdef USBRP

#include "USB_Flash_reprog.h"

extern int REPROG_IN_Progress;

extern unsigned int RHM_USBreprogBuf_Full;
extern unsigned int *RHM_DEBUG_LAST_LBA;
extern unsigned int *RHM_DEBUG_LAST_WRITE;
extern flash *RHM_FlashPtr;
extern unsigned int *RHM_FlashBuf;
extern long USB_INSERT_PTR;
extern long USB_ISR_PTR;
extern void (*ReprogJump)();

#endif

//#define print

void Cmd_TestUnitReady();	
void Cmd_RequestSense();
void Cmd_Inquiry();
void Cmd_ModeSense6();
void Cmd_StartStop();
void Cmd_MediumRemoval();
void Cmd_FormatCapacity();
void Cmd_ReadCapacity();
void Cmd_Read();
void Cmd_Write();
void Cmd_Verify();
void Cmd_ModeSense10();
void Cmd_GPVendorCmd();
void Cmd_GPVendorCmdMP();
void Cmd_ReadTOC();
void ReadDriveInfoTable();
void GetDriveStatus();
void GetICVersion();
void CheckCDRomValid(void);
int SetVenderID();
extern tra_end;

void SCSI_Command(void)
{
	unsigned int i;//, unRet;
	
	SCSI_CMD_MAP	*ScsiCmdPtr;
/*RHM	
	SCSI_CMD_MAP ScsiCmdMapList[] = 
	{
		{0x00, (void*)Cmd_TestUnitReady},	
		{0x03, (void*)Cmd_RequestSense},	
		{0x12, (void*)Cmd_Inquiry},	
		{0x1A, (void*)Cmd_ModeSense6},	
		{0x1B, (void*)Cmd_StartStop},	
		{0x1E, (void*)Cmd_MediumRemoval},	
		{0x23, (void*)Cmd_FormatCapacity},	
		{0x25, (void*)Cmd_ReadCapacity},	
		{0x28, (void*)Cmd_Read},	
		{0x2A, (void*)Cmd_Write},	
		{0x2F, (void*)Cmd_Verify},	
		{0x43, (void*)Cmd_ReadTOC},	
		{0x5A, (void*)Cmd_ModeSense10},	
		{0xF0, (void*)Cmd_GPVendorCmd},	
		{0xFF, (void*)Cmd_GPVendorCmdMP}	

	};
*/
	CSW_Status=0;
	
	Command_Value=GetCMDValue();
	CBWLUN=GetLUN();
	CSW_Residue=GetCSWLength();
	if (GetCMDValueEx(12))
		SCSI_Direct_Write = 0;
	else
		SCSI_Direct_Write = 1;
	
	if (!SCSI_Direct_Write || !CSW_Residue)//Bulk-in check ready
	{
		*P_USBD_EPEvent=0x0400;		// Bulk IN NACK Reset
		RD_Until_Bulk_IN_NACK();	// Read until Bulk IN NACK
	}	
	
	if ((Command_Value > 0xF0)&&(Command_Value != 0xFF))	//customer vendor command
	{
		VendorCmd_CallBack();	
		return;
	}
	
/* RHM
	for(i=0; i<(sizeof(ScsiCmdMapList)/sizeof(SCSI_CMD_MAP)); i++)
	{
		ScsiCmdPtr = &ScsiCmdMapList[i];
		if(ScsiCmdPtr->ScsiCmdCode == Command_Value)//+15: vendor command item code
		{
			ScsiCmdPtr->function();
			return;
		}		
	}		
*/
	
	if(Command_Value == 0) {
		Cmd_TestUnitReady();
		return;
	}
	if(Command_Value == 3) {
		Cmd_RequestSense();
		return;
	}	
	if(Command_Value == 0x12) {
		Cmd_Inquiry();
		return;
	}
	if(Command_Value == 0x1a) {
		Cmd_ModeSense6();
		return;
	}	
	if(Command_Value == 0x1b) {
		Cmd_StartStop();
		return;
	}
	if(Command_Value == 0x1e) {
		Cmd_MediumRemoval();
		return;
	}	
	if(Command_Value == 0x23) {
		Cmd_FormatCapacity();
		return;
	}	
	if(Command_Value == 0x25) {
		Cmd_ReadCapacity();
		return;
	}	
	if(Command_Value == 0x28) {
		Cmd_Read();
		return;
	}	
	if(Command_Value == 0x2a) {
		Cmd_Write();
		return;
	}	
	if(Command_Value == 0x2f) {
		Cmd_Verify();
		return;
	}	
	if(Command_Value == 0x43) {
		Cmd_ReadTOC();
		return;
	}	
	if(Command_Value == 0x5a) {
		Cmd_ModeSense10();
		return;
	}
	if(Command_Value == 0xf0) {
		Cmd_GPVendorCmd();
		return;
	}	
	if(Command_Value == 0xff) {
		Cmd_GPVendorCmdMP();
		return;
	}	
			
	CommandFail(0x1A);			
}


void CommandFail(int nSenseIndex)
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
			RxUSB_RW_Buffer(512);
			unCnt--;
			if(tra_end=0xffff)
				break;
		}	
		//Remain
		unCnt = CSW_Residue%512;
		if (unCnt)
			RxUSB_RW_Buffer(unCnt);
//		if(ret==0xffff)
//			break;
	}			
	else 
		Bulk_Stall=Bulk_IN_Stall;		
	
	CSW_Residue = 0;	

}
////////////////////////////////////////////////////////////////////////////////
//Return 1: Media present
//		 0: Media not present
////////////////////////////////////////////////////////////////////////////////
unsigned int CheckLunStatus(unsigned long  lUSB_LUN_Info_CMP)
{
	str_USB_Lun_Info*  ptr_USB_LUN_Info_CMP = (str_USB_Lun_Info*)lUSB_LUN_Info_CMP;
	
	if (ptr_USB_LUN_Info_CMP->unLunType <= 1)//nand flash
	{
		if ((!R_NAND_Present)||(ptr_USB_LUN_Info_CMP->unStatus==LunStatus_NoMedia)||
			((ptr_USB_LUN_Info_CMP->unLunType == LunType_CDROM) && CDROMDelay))
		{
			CommandFail(0x0B);	//NoMedia
			return 0;	
		}
		if (ptr_USB_LUN_Info_CMP->unStatus&LunStatus_MediaChange)
		{
			ptr_USB_LUN_Info_CMP->unStatus ^= LunStatus_MediaChange;
			CommandFail(0x22);	//Media change
			return 0;	
		}
	}
	
	if (ptr_USB_LUN_Info_CMP->unLunType > 1)//SD card
	{
		if ((!R_SDC_Present)||(ptr_USB_LUN_Info_CMP->unStatus==LunStatus_NoMedia))
		{
			CommandFail(0x0B);	//NoMedia
			return 0;	
		}
		if (ptr_USB_LUN_Info_CMP->unStatus&LunStatus_MediaChange)
		{
			ptr_USB_LUN_Info_CMP->unStatus ^= LunStatus_MediaChange;
			CommandFail(0x22);	//Media change
			return 0;	
		}
	}				
	return 1;	
}

void Cmd_TestUnitReady()
{
	int i;
	
	CSW_Residue=0;
	
	if (TestUnitReady_CallBack())
		return;
	
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			CheckLunStatus((unsigned long)ptr_USB_LUN_Info);
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

void Cmd_RequestSense()
{
	Request_Sense(Sense_Code*3);
	CSW_Residue=0;
	CSW_Status=0;
}

void Cmd_Inquiry()
{
	int i;
		
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			if (CDROMEnable && CDROMDelay)	//enable CDROM delay timer
				CDROMTimerStartup();

			Inquiry(ptr_USB_LUN_Info->unLunType);
			break;
		}
	}
	CSW_Residue=0;
	CSW_Status=0;	
}

void Cmd_ModeSense6()
{
	int i;
		
	if (ModeSense6_CallBack())
		return;
	
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			if (CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{
				if (ptr_USB_LUN_Info->unLunType == LunType_SDCardReader)
					USBWriteProtect = R_Write_protect?0x80:0; //R_Write_protect is card reader write protect
				else if (ptr_USB_LUN_Info->unStatus & LunStatus_WriteProtect)
					USBWriteProtect = 0x80;
				else
					USBWriteProtect = 0x00;
				
				Mode_Sense_06();
			    Bulk_Stall=Bulk_IN_Stall;
			    CSW_Residue-=4;
				CSW_Status=0;							
			}
			break;
		}
	}	
}

void Cmd_StartStop()
{
	CSW_Residue=0;
	CSW_Status=0;
}

void Cmd_MediumRemoval()
{
	Prevent_Allow_Medium_removal();
}

void Cmd_FormatCapacity()
{
	int i;
		
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			if (CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{	
				if (ptr_USB_LUN_Info->unLunType == LunType_SDCardReader)
					Read_Format_Capacities(SDCTotalSector);
				else
			    	Read_Format_Capacities(ptr_USB_LUN_Info->ulNandSizeSec);
	
				if(CSW_Residue > 20)
				{
					// reply stall
					Bulk_Stall=Bulk_IN_Stall;
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

void Cmd_ReadCapacity()
{
	int i;	
	
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			if (CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{	
				if (ptr_USB_LUN_Info->unLunType == LunType_SDCardReader)
					Read_Capacity(SDCTotalSector);
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
							Read_CDROM_Capacity(ptr_USB_LUN_Info->ulNandSizeSec);
							return;	
						}	
					}						
			    	Read_Capacity(ptr_USB_LUN_Info->ulNandSizeSec);
				}
			}
			break;
		}
	}	
}

void Cmd_Read()
{
	int i;	
	
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			if (CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{	
				CSW_Status=0;

				if (Read_10((unsigned long)ptr_USB_LUN_Info))
				{
					Bulk_Stall=Bulk_IN_Stall;
					CSW_Status=1;
				}

			}
			break;
		}
	}	
}

void Cmd_Write()
{
	int i;
		
	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info->unLunNum == CBWLUN)
		{
			CSW_Status=0;
			//check write protect
			if (((ptr_USB_LUN_Info->unLunType == LunType_SDCardReader) && R_Write_protect)||
				(ptr_USB_LUN_Info->unStatus & LunStatus_WriteProtect))
			{
				CommandFail(0x25);
				return;
			}
			//Check CDROM
			if (ptr_USB_LUN_Info->unLunType == LunType_CDROM)	//CDROM write command, check CDROM first
			{
				if ((GetCMDValueEx(27)!='G')||(GetCMDValueEx(28)!='P')||(GetCMDValueEx(29)!='C')||(GetCMDValueEx(30)!='D'))		
				{
					CommandFail(0x25);
					return;
				}
				if (Write_10((unsigned long)ptr_USB_LUN_Info))
					CommandFail(Sense_Code);	
			}
			else if (CheckLunStatus((unsigned long)ptr_USB_LUN_Info))
			{	
				if (Write_10((unsigned long)ptr_USB_LUN_Info))
					CommandFail(Sense_Code);	
					
			}
			break;
		}
	}	
}

void Cmd_Verify()
{
	CSW_Residue=0;
	CSW_Status=0;
}

void Cmd_ReadTOC()
{
	unsigned long USB_RW_Buffer_PTR;
	
	
	if (CheckLunStatus((unsigned long)ptr_USB_LUN_Info))	
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
			USB_RW_Buffer_PTR=GetBufferPointer();
//			_NAND_ReadSector(ptr_USB_LUN_Info->ulNandStartAddr, 1, (unsigned long)USB_RW_Buffer_PTR);
/*			break;
		}
	}	*/		
	}
	switch(GetCMDValueEx(24) & 0xC0)
	{
	case 0x00:
		TxUSB_RW_Buffer_Word(CSW_Residue, 128/2);
		break;
	case 0x40:
		TxUSB_RW_Buffer_Word(CSW_Residue, 192/2);
		break;				
	case 0x80:
		TxUSB_RW_Buffer_Word(CSW_Residue, 256/2);
		break;
	default:
		CommandFail(0x1C);
	}				
}

void Cmd_ModeSense10()
{
	int nVendorCmd;
	
	nVendorCmd = GetCMDValueEx(17);
	
	if((nVendorCmd==5) || (nVendorCmd==63))
	{
		ReplyFlexPage();	
		return;
	}
	
	if (CheckLunStatus(ptr_USB_LUN_Info))
	{	
		if(nVendorCmd == 0x2A)
		{
			ModeSenseData();
			return;	
		}
	}
	Bulk_Stall=Bulk_IN_Stall;
	CSW_Status=1;
	Sense_Code=0x0B;			
	
}

void Cmd_GPVendorCmd()
{
	int i;
#ifdef USBRP
	int rhm;
#endif 
	SCSI_CMD_MAP	*ScsiCmdPtr;
	/*RHM
	SCSI_CMD_MAP ScsiCmdMapList[] = {
		{0x00, (void*)ReadDriveInfoTable},	
		{0x01, (void*)GetDriveStatus},	
		{0x10, (void*)GetICVersion}	
	};				
*/	
	if (GetCMDValueEx(29)!='G' || GetCMDValueEx(30)!='P')
	{
		CommandFail(0x27);					
		return;		
	}
	
	Command_SecValue = GetCMDValueEx(16);
/*RHM
	for(i=0; i<(sizeof(ScsiCmdMapList)/sizeof(SCSI_CMD_MAP)); i++)
	{
		ScsiCmdPtr = &ScsiCmdMapList[i];
		if(ScsiCmdPtr->ScsiCmdCode == Command_SecValue){					//+15: vendor command item code
			ScsiCmdPtr->function();
			return;
		}
	}
*/
	if(Command_SecValue == 0) {
		ReadDriveInfoTable();
		return;
	}
	if(Command_SecValue == 1) {
		GetDriveStatus();
		return;
	}
	if(Command_SecValue == 0x10) {
		GetICVersion();
		return;
	}

#ifdef USBRP
	rhm = Command_SecValue;	 // for debugging
#endif 	
			
	CommandFail(0x1A);			
	
}

#ifdef USBRP

void *
RHMJump()
{
	void (*P)();
	short *memptr = 0;
	short *p = &memptr;
	
	*(p+1) = (short) (GetCMDValueEx(17) << 24);
	*(p+1) |= (short) (GetCMDValueEx(18) << 16);
	*p |= GetCMDValueEx(19) << 8;
	*p |= GetCMDValueEx(20);
	
	Sense_Code = 0;
	CSW_Residue=0;
	CSW_Status=0;
	USB_Status =  BULK_IN_NACK;  //TEST			
	
	P = memptr;
	
	ReprogJump = P;
	
	return;
	
//	(*P)();

}

void RHMWrite()
{
	short *memptr = 0;
	short length, memtype, s;
	short *p = &memptr;
	short lun;
	
	*(p+1) = (short) (GetCMDValueEx(17) << 24);
	*(p+1) |= (short) (GetCMDValueEx(18) << 16);
	*p |= GetCMDValueEx(19) << 8;
	*p |= GetCMDValueEx(20);
	
	memtype = GetCMDValueEx(21);
	
	length = GetCMDValueEx(22) << 8;
	length |= GetCMDValueEx(23);
	
	s = length;
	
	if(memtype == LunType_RAM)
		lun = 1;
	else
		lun = 2;  //nor flash
	
	if (Write_10((unsigned long)&USB_Lun_Define[lun]))
		CommandFail(Sense_Code);
	else 
		REPROG_IN_Progress = 1;
}
#endif 

void Cmd_GPVendorCmdMP()
{
	int i;
#ifdef USBRP
	int rhm;
#endif
	SCSI_CMD_MAP	*ScsiCmdPtr;
	/*RHM
	SCSI_CMD_MAP ScsiCmdMapList[] = {
#ifdef USBRP
		{0x2a, (void*)RHMWrite},
		{0xff, (void*)RHMJump},
#endif
		{0xF0, (void*)SetVenderID}
	};
*/		
	if (GetCMDValueEx(29)!='G' || GetCMDValueEx(30)!='P')
	{
		CommandFail(0x27);					
		return;		
	}
	
	Command_SecValue = GetCMDValueEx(16);
/*RHM
	for(i=0; i<(sizeof(ScsiCmdMapList)/sizeof(SCSI_CMD_MAP)); i++)
	{
		ScsiCmdPtr = &ScsiCmdMapList[i];
		if(ScsiCmdPtr->ScsiCmdCode == Command_SecValue){					//+15: vendor command item code
			ScsiCmdPtr->function();
			return;
		}
	}	
*/
	if(Command_SecValue == 0xf0) {
		SetVenderID();
		return;
	}
	if(Command_SecValue == 0x2a) {
		RHMWrite();
		return;
	}
	if(Command_SecValue == 0xff) {
		RHMJump();
		return;
	}
		
	rhm = Command_SecValue;
		
	CommandFail(0x1A);			
	
}

void GetDriveStatus()
{
	Command_Value=GetCMDValueEx(17);

	switch(Command_Value)
	{
		case 0x00:	//the lun number that operating now
			*P_USBD_BIData = CBWLUN;
			*P_USBD_EPEvent = 0x0100;
		break;

		case 0x01:
			break;

		default:
			CommandFail(0x1A);			
			break;
	}		
}

void USB_ISR()
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
			USBDSuspend_CallBack();			//user could do something
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
			USB_Reset();
		}
	
	}	
}

void USB_Reset(void)
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

void USB_ServiceLoop(unsigned int unUseLoop)
{
	unsigned a;
#ifdef USBRP
	unsigned int RHM_USBreprogBuf[2048];
	void (*PF)() = 0;
	int reprogwait = -1;
#endif

	int i;

	#ifdef print	
	init_uart();
	print1("\n\n\n\n\n/// SPL162001 USB DEVICE MSDC Program Start ///\n");
	#endif
		
#ifdef USBRP
	RHM_FlashBuf = &RHM_USBreprogBuf[0];
	ReprogJump = 0;
#endif	

	while(1)
	{
#ifdef USBRP
		if(ReprogJump != 0) {
			if (reprogwait++ >= 0) //go through state machine once to keep usb happy
				(*ReprogJump)();
		}
#endif 
	
//b		switch (R_USB_State_Machine)
		if(R_USB_State_Machine == EP0_SETUP_Stage) {
//b		{
			
//b			case EP0_SETUP_Stage:
				R_USB_State_Machine=EP0_Data_Stage;
				Setup_Token();
			
				USB_Status=0x00;
				a=*P_USBD_INTEN;				// Enable EP0_IN_NACK INT
				*P_USBD_INTEN=a|0x0010;
				*P_USBD_INT=0x0010;				// Clear EP0_IN_NACK	// added by Anson 2005/06/30 for verify test				
//b				break;
		} else if(R_USB_State_Machine == EP0_Data_Stage) {
//b			case EP0_Data_Stage:
				if(USB_Status==EP0_IN_NACK)		// When USB_Status = EP0 IN NACK
				{								// Start EP0 Data Stage
					Write_EP0_FIFO();					
					USB_Status=0x00;
				}			
//b				break;
		} else if(R_USB_State_Machine == SCSI_CBW_Stage) {
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
					
					Receive_CBW_PKT();
					*P_USBD_EPEvntClear=0x0010;
					
					USB_Status=0x00;			// Clear USB_Status to zero
					
//					SCSI_State=SCSI_Data_Stage;	
					R_USB_State_Machine=SCSI_Data_Stage;
				}	
			
//b				break;
			} else if(R_USB_State_Machine == SCSI_Data_Stage) {
//b			case SCSI_Data_Stage:
				*P_USBD_EPEvent=0x2400;		// Clear Bulk IN / OUT NACK
				SCSI_Command();
				
				/////////////////////////////////////////////////////
				if(Bulk_Stall==Bulk_IN_Stall)
				{
					*P_USBD_EPEvent=0x0400;		// Bulk IN NACK Reset
			
					RD_Until_Bulk_IN_NACK();	// Read until Bulk IN NACK
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
			} else if(R_USB_State_Machine == SCSI_CSW_Stage) {
//b			case SCSI_CSW_Stage:
				if(USB_Status==BULK_IN_NACK)
				{
//					print1("SCSI CSW Stage\n");
					
					SCSI_CSW();
					
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
				
				
//b			default:
//b				break;
		}
		
		if(R_USB_Suspend == 1)
		{
//			EnterHaltMode();
			break;
		}
#ifdef USBRP		
		if(RHM_USBreprogBuf_Full != 0) {
			flash *pflash = RHM_FlashPtr;
			
			__asm__("irq off");
			__asm__("fiq off");
			
			(*RHM_FlashPtr->erasesector)(RHM_FlashPtr);
			
			for(i=0; i<RHM_USBreprogBuf_Full; i++) {
				(*RHM_FlashPtr->writeword)(RHM_FlashPtr, RHM_FlashPtr->pflash + i, RHM_USBreprogBuf[i]);
	   			//Nor_WordWrite(RHM_FlashAddr, RHM_USBreprogBuf_Full, (unsigned short)RHM_USBreprogBuf);
	   			//WriteWord(RHM_FlashPtr, RHM_FlashPtr->pflash + i, RHM_USBreprogBuf[i]);
			}
	   		
	   		__asm__("irq on");
			__asm__("fiq on");

			RHM_USBreprogBuf_Full = 0;
		}
#endif	
		Check_SDC_Present();
		if (!unUseLoop)
			break;
	}
#ifdef USBRP	
	RHM_FlashBuf = 0;
#endif
}		

void CheckCDRomValid(void)
{
	int i, *USB_RW_Buffer_PTR;
	
	if (R_NAND_Present)
	{
		USB_RW_Buffer_PTR = GetBufferPointer();
		
		for(i=0; i<N_USB_LUN; i++)
		{
			ptr_USB_LUN_Info = &USB_Lun_Define[i];
			if(ptr_USB_LUN_Info->unLunType == LunType_CDROM)
			{
#ifndef USBRP				
				_NAND_ReadSector(ptr_USB_LUN_Info->ulNandStartAddr, 1, (unsigned long)USB_RW_Buffer_PTR);
#endif				
				if ((*USB_RW_Buffer_PTR) == 0x4547  && (*(USB_RW_Buffer_PTR+1)) == 0x504E && (*(USB_RW_Buffer_PTR+2)) == 0x554C)//"GENPLU"
					CDROMEnable = 1;
			}
		}			
	}			
}

void Setup_Token(void)
{
	unsigned int i;
	
	*P_USBD_EPEvent=0x0001;			// Clear Setup Packet Ready
	USB_Status=0x00;				// added by Anson 2005/06/28 for verify test
	if(*P_USBD_EP0RTR==0x80){
		switch((0xff00 & *P_USBD_EP0VR)){
			//////////////////	Get Device Descriptor	///////////////////
			case 0x0100:			// Device Descriptor
				i=Get_Descriptor_Length(Device_Descriptor_Table);
				if(*P_USBD_EP0LR > i)
					R_EP0DataCount=i;			// Assign Data Count
				else
					R_EP0DataCount=*P_USBD_EP0LR;
					if (!EP0_DataPointer_CallBack(Device_Descriptor_Table))
						DataPointer(Device_Descriptor_Table);				// Assign Data Pointer
					
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
		
				i=Get_Descriptor_Length(Config_Descriptor_Table);
				if(*P_USBD_EP0LR > i)
					R_EP0DataCount=i;
				else
					R_EP0DataCount=*P_USBD_EP0LR;
					
					if (!EP0_DataPointer_CallBack(Config_Descriptor_Table))
						DataPointer(Config_Descriptor_Table);				// Assign Data Pointer
					
					*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
					
//					EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
					#ifdef print	
//					print1("Get Config Descriptor\n");
					#endif
				break;
			
			//////////////////	Get String Descriptor	///////////////////
			case 0x0300:			// String Descriptor
				if((*P_USBD_EP0VR & 0x00ff) == 0) {
//b				switch(*P_USBD_EP0VR & 0x00ff){
//b					case 0x0000:
						i=Get_Descriptor_Length(String0_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;
	
						if (!EP0_DataPointer_CallBack(String0_Descriptor_Table))
							DataPointer(String0_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 0 Descriptor\n");
						#endif
//b						break;
				} else if ((*P_USBD_EP0VR & 0x00ff) == 1) {	
//b					case 0x0001:
						i=Get_Descriptor_Length(String1_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;
							
						if (!EP0_DataPointer_CallBack(String1_Descriptor_Table))					
							DataPointer(String1_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 1 Descriptor\n");
						#endif
//b						break;
				} else if ((*P_USBD_EP0VR & 0x00ff) == 2) {							
//b					case 0x0002:
						i=Get_Descriptor_Length(String2_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;
					
						if (!EP0_DataPointer_CallBack(String2_Descriptor_Table))						
							DataPointer(String2_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 2 Descriptor\n");
						#endif
//b						break;
				} else if ((*P_USBD_EP0VR & 0x00ff) == 3) {													
//b					case 0x0003:
						i=Get_Descriptor_Length(String3_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;
					
						if (!EP0_DataPointer_CallBack(String3_Descriptor_Table))
							DataPointer(String3_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 3 Descriptor\n");
						#endif
//b						break;
				} else if ((*P_USBD_EP0VR & 0x00ff) == 4) {													
//b					case 0x0004:
						i=Get_Descriptor_Length(String4_Descriptor_Table);
						if(*P_USBD_EP0LR > i)
							R_EP0DataCount=i;
						else
							R_EP0DataCount=*P_USBD_EP0LR;

						if (!EP0_DataPointer_CallBack(String4_Descriptor_Table))					
							DataPointer(String4_Descriptor_Table);				// Assign Data Pointer
						
						*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK	// added by Anson 2005/06/28 for verify test
						
//						EP0_State=EP0_Data_Stage;					// Change USB State to EP0 Data Stage
						#ifdef print	
//						print1("Get String 4 Descriptor\n");
						#endif
//b						break;
				} else {							
						
//b					default:
						#ifdef print	
						print1("Not Support    Check (String)\n");
						#endif
						NotSupport();
//b						break;	
				}
				
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
				NotSupport();
				#ifdef print	
				print1("Not Support    Check\n");
				#endif
				break;
		
		
		
		}
	}
	else if(*P_USBD_EP0RTR==0xA1 && *P_USBD_EP0RR==0xFE && *P_USBD_EP0VR==0x00){
		R_EP0DataCount=*P_USBD_EP0LR;
		DataPointer(MaxLUN_Table);
		#ifdef print	
		print1("Get Max LUN\n");
		#endif
		*P_USBD_EPEvent=0x0020;						// Clear EP0 IN NACK
		while((*P_USBD_EPEvent & 0x0020) == 0);
		Write_EP0_FIFO();
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
		NotSupport();
		#ifdef print	
		print1("Not Support    Other Setup Command\n");
		#endif
		
	}	
	
	
}

void Write_EP0_FIFO()
{
	int a;
	
	if(R_EP0DataCount==0){
		// reply Null Packet
		Send_Null();
		#ifdef print	
		print1("Send Null Packet\n");
		#endif
		return;
	}
	
	while (R_EP0DataCount)		//huck: Modify to fix cable too long issue or connect with a long cable hub
	{
		if(R_EP0DataCount >= 8)
		{
			Send_8_Bytes();
	
			a=*P_USBD_INTEN;			// Enable EP0_IN_NACK INT
			*P_USBD_INTEN=a|0x0010;
			*P_USBD_INT=0x0010;			// Clear EP0_IN_NACK	// added by Anson 2005/06/30 for verify test

//*P_IOA_Data=0xF000;				
	//		print1("Send 8 Bytes\n");
		}
		else if(R_EP0DataCount <8)
		{
			Send_Remain_Bytes();
//*P_IOA_Data=0x0000;							
	//		print1("Send Remain Bytes\n");
		}
	}	
	return;
}



void NotSupport()
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


void fUSB_SDCardPlugIn()
{
	int	i;
	str_USB_Lun_Info*  ptr_USB_LUN_Info_SD;

	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info_SD = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info_SD->unLunType == LunType_SDCardReader)
		{
			ptr_USB_LUN_Info_SD->unStatus = LunStatus_Normal | LunStatus_MediaChange;						
			break;
		}
	}
}

void fUSB_SDCardPullOut()
{
	int	i;
	str_USB_Lun_Info*  ptr_USB_LUN_Info_SD;

	for(i=0; i<N_USB_LUN; i++)
	{
		ptr_USB_LUN_Info_SD = &USB_Lun_Define[i];
		if(ptr_USB_LUN_Info_SD->unLunType == LunType_SDCardReader)
		{
			ptr_USB_LUN_Info_SD->unStatus = LunStatus_NoMedia ^ LunStatus_WriteProtect;						
			break;
		}
	}
}
