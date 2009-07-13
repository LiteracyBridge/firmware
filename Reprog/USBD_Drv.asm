.include .\System\include\system\GPL162002.inc
.include .\Driver\Include\USB\USBD.inc
.include .\Driver\Include\USB\USBD_Define.inc
//RHM.include GPL162002.inc
//RHM.include USBD.inc
//RHM.include USBD_Define.inc
//.include NandDriver.inc

.public _Receive_CBW_PKT
.public _Request_Sense
.public _Inquiry
.public _Mode_Sense_06
.public _Prevent_Allow_Medium_removal
.public _Read_Format_Capacities
.public _Read_Capacity

.public _GetCMDValue
.public _GetLBA
.public _GetTransLeng
.public _GetLUN
.public _GetCSWLength
.public _RD_Until_Bulk_IN_NACK
.public _RD_Until_Bulk_OUT_NACK
.public _GetBufferPointer
.public _GetCMDValueEx
.public _ReadDriveInfoTable
.public _TxUSB_RW_Buffer
.public _RxUSB_RW_Buffer
.public _GetICVersion
//.public _Request_ReadID		//add by zhangxh
.public _GotoRam
.public _CallRam
.public _USB_Initial;
.public	F_USB_Initial;
.public _TxUSB_RW_Buffer_Word
.public _Send_To_USB_DMA_USB
.public _Receive_From_USB_DMA_USB
.public _Read_CDROM_Capacity
.external _USB_Insert_TimeOut


//.external   _NandFlashInitial
//Interrupt_Vector_Table_Reserved_Section:	.section	.ram	,.addr=0xFFF0
//Interrupt_Vector_Table_Reserved:			.dw	 	0xF		dup(?)

USB_Code_ROM:	.section .code//, .addr=0x40000

//********************************EP0 Code******************************************************

//EP0_Data_Stage_Section:	.section	.code
//////////////////////////////////////////////////////////////////////////
//========================================================
//Function Name:	Get_Descriptor_Length
//Syntax:		void Get_Descriptor_Length(void)
//Purpose:		Get Descriptor Length
//Note:
//Parameters:	Descriptor Type
//Return:		Descriptor Length
//=======================================================
.public _Get_Descriptor_Length
_Get_Descriptor_Length:
		push bp to [sp]
		bp=sp+1
		r1=[bp+3]
		
	?L_Device_Length:
		cmp r1,C_Device_Descriptor_Table
		jne ?L_Config_Length
		
		r1=seg _Device_Descriptor
		ds=r1
		r1=offset _Device_Descriptor
		r1=ds:[r1]

		pop bp from [sp]
		retf
		
	?L_Config_Length:
		cmp r1,C_Config_Descriptor_Table
		jne ?L_String0_Length
		r1=seg _Config_Descriptor
		ds=r1
		r1=offset _Config_Descriptor
		r1+=2
		r1=ds:[r1]
		
		pop bp from [sp]
		retf
		
	?L_String0_Length:
		cmp r1,C_String0_Descriptor_Table
		jne ?L_String1_Length
		
		r1=seg _String0_Descriptor
		ds=r1
		r1=offset _String0_Descriptor
		r1=ds:[r1]
		
		pop bp from [sp]
		retf
		
	?L_String1_Length:
		cmp r1,C_String1_Descriptor_Table
		jne ?L_String2_Length
		
		r1=seg _String1_Descriptor
		ds=r1
		r1=offset _String1_Descriptor
		r1=ds:[r1]
		
		pop bp from [sp]
		retf
		
	?L_String2_Length:
		cmp r1,C_String2_Descriptor_Table
		jne ?L_String3_Length
		
		r1=seg _String2_Descriptor
		ds=r1
		r1=offset _String2_Descriptor
		r1=ds:[r1]
		
		pop bp from [sp]
		retf
		
	?L_String3_Length:
		cmp r1,C_String3_Descriptor_Table
		jne ?L_String4_Length
		
		r1=seg _String3_Descriptor
		ds=r1
		r1=offset _String3_Descriptor
		r1=ds:[r1]
		
		pop bp from [sp]
		retf
		
	?L_String4_Length:
//		cmp r1,C_String4_Descriptor_Table
//		jne ?L_String4_Length
		
		r1=seg _String4_Descriptor
		ds=r1
		r1=offset _String4_Descriptor
		r1=ds:[r1]
		
		pop bp from [sp]
		retf
/////////////////////////////////////////////////////////////////////////////////
//========================================================
//Function Name:	DataPointer
//Syntax:		 void DataPointer(void)
//Purpose:		Get Descriptor Data Pointer
//Note:
//Parameters:	Descriptor Type
//Return:		None
//=======================================================
.public _DataPointer
_DataPointer:
		push bp to [sp]
		bp=sp+1
		push r1 to [sp]
		r1=[bp+3]
		cmp r1,C_Device_Descriptor_Table
		jne ?L_Config_Table
		
		r1=seg	_Device_Descriptor
		[R_Descriptor_High]=r1
		r1=offset	_Device_Descriptor
		[R_Descriptor_Low]=r1
		
		pop r1 from [sp]
		pop bp from [sp]
		retf
		
	?L_Config_Table:
		cmp r1,C_Config_Descriptor_Table
		jne ?L_String0_Table
		
		r1=seg	_Config_Descriptor
		[R_Descriptor_High]=r1
		r1=offset	_Config_Descriptor
		[R_Descriptor_Low]=r1
		
		pop r1 from [sp]
		pop bp from [sp]
		retf
		

	?L_String0_Table:
		cmp r1,C_String0_Descriptor_Table
		jne ?L_String1_Table
		r1=seg	_String0_Descriptor
		[R_Descriptor_High]=r1
		r1=offset	_String0_Descriptor
		[R_Descriptor_Low]=r1
	
		pop r1 from [sp]
		pop bp from [sp]
		retf

	?L_String1_Table:
		cmp r1,C_String1_Descriptor_Table
		jne ?L_String2_Table
		r1=seg	_String1_Descriptor
		[R_Descriptor_High]=r1
		r1=offset	_String1_Descriptor
		[R_Descriptor_Low]=r1
	
		pop r1 from [sp]
		pop bp from [sp]
		retf
	
	?L_String2_Table:
		cmp r1,C_String2_Descriptor_Table
		jne ?L_String3_Table
		r1=seg	_String2_Descriptor
		[R_Descriptor_High]=r1
		r1=offset	_String2_Descriptor
		[R_Descriptor_Low]=r1
	
		pop r1 from [sp]
		pop bp from [sp]
		retf
		
	?L_String3_Table:
		cmp r1,C_String3_Descriptor_Table
		jne ?L_String4_Table
		r1=seg	_String3_Descriptor
		[R_Descriptor_High]=r1
		r1=offset	_String3_Descriptor
		[R_Descriptor_Low]=r1
	
		pop r1 from [sp]
		pop bp from [sp]
		retf
	
	?L_String4_Table:
		cmp r1,C_String4_Descriptor_Table
		jne ?L_MaxLUN_Table
		r1=seg	_String4_Descriptor
		[R_Descriptor_High]=r1
		r1=offset	_String4_Descriptor
		[R_Descriptor_Low]=r1
	
		pop r1 from [sp]
		pop bp from [sp]
		retf
	?L_MaxLUN_Table:
//		cmp r1,C_MaxLUN_Table
		r1=seg	_Max_LUN
		[R_Descriptor_High]=r1
		r1=offset	_Max_LUN
		[R_Descriptor_Low]=r1
	
		pop r1 from [sp]
		pop bp from [sp]
		retf
		
/////////////////////////////////////////////////////////////////////////////////
//========================================================
//Function Name:	Send_Null
//Syntax:		void Send_Null(void)
//Purpose:		Send EP0 Null Packet
//Note:
//Parameters:	None
//Return:		None
//=======================================================
.public _Send_Null
_Send_Null:
		push r1 to [sp]
		
//		r1=[P_USBD_Config]
//		r1=r1 | 0x8000
//		[P_USBD_Config]=r1		// EnableNull Packet
		
		r1=0x0001					// Enable Null Packet for Endpoint 0
		[P_USBD_NullPkt]=r1
		
		r1=0x0010
		[P_USBD_EPEvent]=r1			// EP0 IN Packet Ready
		
		///////	Wait Until EP0 IN Packet Clear	///////
	?L_EP0_IN_Clear:
		r1=[P_USBD_EPEvent]
		test r1,0x0010
		jnz ?L_EP0_IN_Clear
		
	//	r1=[P_USBD_Config]
	//	r1=r1 & 0x7fff
	//	[P_USBD_Config]=r1		// Disable Null Packet
		
		r1=0x0000
		[P_USBD_NullPkt]=r1
		
		r1=0xff
		[P_USBD_EPBufClear]=r1
		
		pop r1 from [sp]
		retf
		
////////////////////////////////////////////////////////////////////////
//========================================================
//Function Name:	Send_8_Bytes
//Syntax:		void Send_8_Bytes(void)
//Purpose:		Send EP0 8 Bytes Packet
//Note:
//Parameters:	None
//Return:		None
//=======================================================
.public _Send_8_Bytes
.public F_Send_8_Bytes
_Send_8_Bytes:
F_Send_8_Bytes:
		push r1,r4 to [sp]
		r1=[R_Descriptor_High]
		ds=r1
		r1=[R_Descriptor_Low]
		
	//	r2=0x04
		r2=0x08
	?L_Write_Finish:
		r3=ds:[r1++]	// Low Byte
	//	r4=ds:[r1++]	// High Byte
	//	r4=r4 lsl 4
	//	r4=r4 lsl 4
	//	r3=r3 | r4
		[P_USBD_EP0Data]=r3		
		r2-=1
		jnz ?L_Write_Finish
		
		[R_Descriptor_Low]=r1
		r1=ds
		[R_Descriptor_High]=r1
		
		r1=[_R_EP0DataCount]
		r1-=8
		[_R_EP0DataCount]=r1		
		
		r1=0x0010
		[P_USBD_EPEvent]=r1			// EP0 IN Packet Ready

		r2 = 0		
	?L_WaitTxFinish:				//Huck add to pass USB-IF
		r2 -= 1
		jz ?L_TimeOut
		r1 = [P_USBD_EPEvent]
		r1 = r1 & 0x0010
		jnz ?L_WaitTxFinish
	?L_TimeOut:	
		pop r1,r4 from [sp]
		retf
		
////////////////////////////////////////////////////////////////////////
//========================================================
//Function Name:	Send_Remain_Bytes
//Syntax:		void Send_Remain_Bytes(void)
//Purpose:		Send EP0 Remain Bytes
//Note:
//Parameters:	None
//Return:		None
//=======================================================
.public _Send_Remain_Bytes
_Send_Remain_Bytes:
		push r1,r5 to [sp]
		r1=[R_Descriptor_High]
		ds=r1
		r1=[R_Descriptor_Low]
		
		r2=[_R_EP0DataCount]
		r3 = 0
		[_R_EP0DataCount] = r3	
	//	r5=r2 lsr 1
	//	cmp r5,0x00
	//	je ?L_Odd_Byte
	?L_Write_Finish:
		r3=ds:[r1++]	// Low Byte
	//	r4=ds:[r1++]	// High Byte
	//	r4=r4 lsl 4
	//	r4=r4 lsl 4
	//	r3=r3 | r4
		[P_USBD_EP0Data]=r3		
	//	r5-=1
		r2-=1
		jnz ?L_Write_Finish
		
	//	test r2,0x0001
	//	jnz ?L_Odd_Byte
		
		r2=0x0010
		[P_USBD_EPEvent]=r2			// EP0 IN Packet Ready

		r2 = 0		
	?L_WaitTxFinishRmn:				//Huck add to pass USB-IF
		r2 -= 1
		jz ?L_TimeOutRmn
		r1 = [P_USBD_EPEvent]
		r1 = r1 & 0x0010
		jnz ?L_WaitTxFinishRmn
	?L_TimeOutRmn:	
		
		pop r1,r5 from [sp]
		retf	
		
//	?L_Odd_Byte:
//		r3=ds:[r1]
//		[P_USBD_EP0SData]=r3
		
//		r2=0x0010
//		[P_USBD_EPEvent]=r2			// EP0 IN Packet Ready
		
//		pop r1,r5 from [sp]
		retf
		

//*****************************SCSI Command**************************************************************************************
//SCSI_CSW_Section:	.section	.code
		
.public _SCSI_CSW
_SCSI_CSW:
		push r1,r3 to [sp]
		
		r1=0x55						// 0 CSW Signature
		[P_USBD_BIData]=r1
		
		r1=0x53						// 1 CSW Signature
		[P_USBD_BIData]=r1
		
		r1=0x42						// 2 CSW Signature
		[P_USBD_BIData]=r1
		
		r1=0x53						// 3 CSW Signature
		[P_USBD_BIData]=r1
		
		r1=seg _R_CBW_PKT
		ds=r1
		r1= offset _R_CBW_PKT
		
		r1+=4
		
		
		r2=[r1++]					// 4 CSW Tag
		[P_USBD_BIData]=r2
		
		r2=[r1++]					// 5 CSW Tag
		[P_USBD_BIData]=r2
		
		r2=[r1++]					// 6 CSW Tag
		[P_USBD_BIData]=r2
		
		r2=[r1++]					// 7 CSW Tag
		[P_USBD_BIData]=r2

		///////////////////////////////////
		// CSW Residue Length
		r1=_CSW_Residue
		r2=[r1++]
		r3=r2 & 0x00ff
		[P_USBD_BIData]=r3
		
		r3=r2 & 0xff00
		r3=r3 lsr 4
		r3=r3 lsr 4
		[P_USBD_BIData]=r3
		
		r2=[r1]
		r3=r2 & 0x00ff
		[P_USBD_BIData]=r3
		
		r3=r2 & 0xff00
		r3=r3 lsr 4
		r3=r3 lsr 4
		[P_USBD_BIData]=r3
		
		///////////////////////////////////
		// CSW Status
		r1=[_CSW_Status]		//00----COMMAND PASS
								//01----COMMAND FAIL
								//02----PHASE ERROR
								//03&04-RESERVED(OBSOLETE)
								//05-FF-RESERVED	
		[P_USBD_BIData]=r1
		
		r1=0x0100
		[P_USBD_EPEvent]=r1
	
		pop r1,r3 from [sp]
		retf
		
//SCSI_Data_Section:	.section	.code
////////////////////////////////////////////////////////////////
_Receive_CBW_PKT:
		push r1,r3 to [sp]
		
		r1=_R_CBW_PKT
		r2=31
	?L_Receive_Finish:
		r3=[P_USBD_BOData]
		[r1++]=r3
		r2-=1
		jnz ?L_Receive_Finish
		
		// Clear BULK OUT Ready
		r1=0x1000
		[P_USBD_EPEvent]=r1
		
		pop r1,r3 from [sp]
		retf
		
////////////////////////////////////////////////////////////////
_GetCMDValue:
		r1=seg _R_CBW_PKT
		ds=r1
		r1= offset _R_CBW_PKT
		
		r1+=15
		r1=ds:[r1]

		retf 
		

////////////////////////////////////////////////////////////////
//unsigned int _GetCMDValueEx(int nIndex)
//nIndex: Get the number of CDB 
////////////////////////////////////////////////////////////////
_GetCMDValueEx:
		push bp to [sp]
		bp=sp+1

		r1 = [bp+3]
GetCMDValueEx:		
		r2=seg _R_CBW_PKT
		ds=r2
		r2= offset _R_CBW_PKT
		
		r2 += r1
		r1=ds:[r2]

		pop bp from [sp]		
		retf		
		
////////////////////////////////////////////////////////////////
_Request_Sense:
		push bp to [sp]
		bp=sp+1
		push r1,r3 to [sp]
		
		r1=0xF0
		[P_USBD_BIData]=r1		//0 Valid, Error Code
		r1=0x00
		[P_USBD_BIData]=r1		//1 Reserved
		
		r1=seg _SCSISENSE
		ds=r1
		r2=offset _SCSISENSE
		
		r3=[bp+3]				// Sense Code
		r2+=r3
		
		r1=ds:[r2++]
		[P_USBD_BIData]=r1		//2 Sense Key
		
		r1=0x00
		[P_USBD_BIData]=r1		//3 Information
		[P_USBD_BIData]=r1		//4 Information
		[P_USBD_BIData]=r1		//5 Information
		[P_USBD_BIData]=r1		//6 Information
		
		r1=0x0A
		[P_USBD_BIData]=r1		//7 Additional Sense Length 0x0A
		
		r1=0x00
		[P_USBD_BIData]=r1		//8 Reserved
		[P_USBD_BIData]=r1		//9 Reserved
		[P_USBD_BIData]=r1		//10 Reserved
		[P_USBD_BIData]=r1		//11 Reserved
		
		r1=ds:[r2++]
		[P_USBD_BIData]=r1		//12 Additional Sense Code
		
		r1=ds:[r2]
		[P_USBD_BIData]=r1			//13 Additional Sense Code Qualifier

		r1 = [_CSW_Residue]
		r1 = r1 - 14
		cmp r1, 1
		jl ?L_GoTx
	?L_CheckLen:	
		r2 = 0
		[P_USBD_BIData]=r2		
		r1 -= 1
		jnz ?L_CheckLen
	
	?L_GoTx:		
		r1=0x0100				// Set Bulk IN packet ready
		[P_USBD_EPEvent]=r1
		
		pop r1,r3 from [sp]
		pop bp from [sp]
		retf
////////////////////////////////////////////////////////////////
_Inquiry:
		push bp to [sp]
		bp=sp+1
		push r1,r4 to [sp]
		r9 = 0
		r1 = [bp+3]
		r8 = r1
		cmp r1,1
		je ?L_CDROM	
		cmp r1,2
		je ?L_CardReader		
			
?L_RemovableDisk:        		
		r1=seg _SCSIInquiryData
		ds=r1
		r1=offset _SCSIInquiryData
		r4=offset _SCSIInquiryData_End
		jmp ?L_END
		
?L_CDROM:	
		r1=seg _SCSIInquiryData_CDROM
		ds=r1
		r1=offset _SCSIInquiryData_CDROM
		r4=offset _SCSIInquiryData_CDROM_End
		jmp ?L_END
		
?L_CardReader:
		r1=seg _SCSIInquiryData_CardReader
		ds=r1
		r1=offset _SCSIInquiryData_CardReader
		r4=offset _SCSIInquiryData_CardReader_End

?L_END:		
		r4=r4-r1

		r2=[_CSW_Residue]				// Inquiry data length
		cmp r2, r4
		jbe ?L_InquiryData
		
		r2 = r4							//Fix reply length, prevent reply too much
	?L_InquiryData:
		r3=ds:[r1++]
		cmp r8, 3
		je 	?L_CheckInquiry
	?L_InquirySkip:	
		[P_USBD_BIData]=r3
		r2-=1
		jnz ?L_InquiryData
	
		r1=0x0100				// Set Bulk IN packet ready
		[P_USBD_EPEvent]=r1
				
		pop r1,r4 from [sp]
		pop bp from [sp]
		retf
		
	?L_CheckInquiry:
		r9 += 1
		cmp r9, 2
		jne ?L_InquirySkip
		r3 = 0					//huck		
		jmp ?L_InquirySkip
////////////////////////////////////////////////////////////////
_Mode_Sense_06:
		push bp to [sp]
		push r1 to [sp]
//		bp = sp + 2 
		
		r1=0x03
		[P_USBD_BIData]=r1		//0 Mode Data Length
		r1=0x00
		[P_USBD_BIData]=r1		//1 Medium Type Code
		
//		r1=[bp+3]				// Determine Which LUN
//		cmp r1,0x01
//		jne ?L_LUN_1
		
		r1=[_USBWriteProtect]    
		jnz ?L_WPEnable
		[P_USBD_BIData]=r1		//2 
		
		jmp ?L_Byte3
		
	?L_WPEnable:
		r1=0x80
		[P_USBD_BIData]=r1		//2
	
	?L_Byte3:
		r1=0x00
		[P_USBD_BIData]=r1		//3 
		
		r1=0x0100				// Set Bulk IN packet ready
		[P_USBD_EPEvent]=r1
				
		pop r1 from [sp]
		pop bp from [sp]
		retf
////////////////////////////////////////////////////////////////
_Prevent_Allow_Medium_removal:
		push r1,r2 to [sp]
		
		r1=seg _R_CBW_PKT
		ds=r1
		r1= offset _R_CBW_PKT
		
		r1+=19
		
		r2=ds:[r1]
		cmp r2,0x0001
		jne ?L_No_Sense
		r1=1
		[_CSW_Status]=r1
		
		r1=0x1C
		[_Sense_Code]=r1
		
		pop r1,r2 from [sp]
		retf
	
	?L_No_Sense:
		r1=0
		[_CSW_Status]=r1
		
		[_Sense_Code]=r1
		
		pop r1,r2 from [sp]
		retf
////////////////////////////////////////////////////////////////
_Read_Format_Capacities:
		push bp to [sp]
		bp=sp+1
		push r1, r4  to [sp]
		
		r1=0x00
		[P_USBD_BIData]=r1		//0 Reserved
		[P_USBD_BIData]=r1		//1 Reserved
		[P_USBD_BIData]=r1		//2 Reserved
		
		r1=0x10
		[P_USBD_BIData]=r1		//3 Capacity List Length
		// Determine Which type index
		r2 = [bp+3]				// low word capacity
		r3 = [bp+4]				// high word capacity
//		r2=[r1++]		// Low Byte
//		r3=[r1]			// High Byte
		r4=r3 lsr 4
		r4=r4 lsr 4
		
		[P_USBD_BIData]=r4		//4 Number of Blocks
		r4=r3 & 0x00FF
		[P_USBD_BIData]=r4		//5 Number of Blocks
		
		r4=r2 lsr 4
		r4=r4 lsr 4
		[P_USBD_BIData]=r4		//6 Number of Blocks
		
		r4=r2 & 0x00FF
		[P_USBD_BIData]=r4		//7 Number of Blocks
	//////////////////////	
		r1=0x02
		[P_USBD_BIData]=r1		//8 Descriptor Code
		r1=0x00
		[P_USBD_BIData]=r1		//9 Block Length
		r1=0x02
		[P_USBD_BIData]=r1		//10 Block Length
		r1=0x00
		[P_USBD_BIData]=r1		//11 Block Length
		
		r2 = [bp+3]				// low capacity
		r3 = [bp+4]				// high capacity
//		r2=[r1++]		// Low Byte
//		r3=[r1]			// High Byte
		r4=r3 lsr 4
		r4=r4 lsr 4
		[P_USBD_BIData]=r4		//12 Number of Blocks
		r4=r3 & 0x00FF
		[P_USBD_BIData]=r4		//13 Number of Blocks
		
		r4=r2 lsr 4
		r4=r4 lsr 4
		[P_USBD_BIData]=r4		//14 Number of Blocks
		
		r4=r2 & 0x00FF
		[P_USBD_BIData]=r4		//15 Number of Blocks		
		r1=0x00
		[P_USBD_BIData]=r1		//16 Descriptor Code
		[P_USBD_BIData]=r1		//17 Block Length
		
		r2=0x02
		[P_USBD_BIData]=r2;		//18 Block Length
		[P_USBD_BIData]=r1		//19 Block Length
	
		r1=0x0100				// Set Bulk IN packet ready
		[P_USBD_EPEvent]=r1
	
		pop r1, r4  from [sp]
		pop bp from [sp]
		retf
////////////////////////////////////////////////////////////////
_Read_CDROM_Capacity:
		push bp to [sp]
		bp=sp+1
		
		r2 = [bp+3]				// low word
		r1 = [bp+4]				// high word

		r2-=1
		r1=r1-0,carry
		
		r3 = r1 ror 4
		r2 = r2 ror 2
		r1 = r1 lsr 2
				
		r3=r1 lsr 4
		r3=r3 lsr 4
	
		[P_USBD_BIData]=r3		//0 

		r3=r1 & 0x00FF
		[P_USBD_BIData]=r3		//1

		r3=r2 lsr 4
		r3=r3 lsr 4
		[P_USBD_BIData]=r3		//2

		r3=r2 & 0x00FF
		[P_USBD_BIData]=r3		//3  
	
		r1=0x00
		[P_USBD_BIData]=r1		//4
		[P_USBD_BIData]=r1		//5
		r2=0x08
		[P_USBD_BIData]=r2		//6
		[P_USBD_BIData]=r1		//7
	
		r1=0x0100				// Set Bulk IN packet ready
		[P_USBD_EPEvent]=r1
		
		pop bp from [sp]
		retf		
////////////////////////////////////////////////////////////////
_Read_Capacity:
		push bp to [sp]
		bp=sp+1
		
		r2 = [bp+3]				// low word
		r1 = [bp+4]				// high word

		r2-=1
		r1=r1-0,carry
		
		r3=r1 lsr 4
		r3=r3 lsr 4
	
		[P_USBD_BIData]=r3		//0 

		r3=r1 & 0x00FF
		[P_USBD_BIData]=r3		//1

		r3=r2 lsr 4
		r3=r3 lsr 4
		[P_USBD_BIData]=r3		//2

		r3=r2 & 0x00FF
		[P_USBD_BIData]=r3		//3  
	
		r1=0x00
		[P_USBD_BIData]=r1		//4
		[P_USBD_BIData]=r1		//5
		r2=0x02
		[P_USBD_BIData]=r2		//6
		[P_USBD_BIData]=r1		//7
	
		r1=0x0100				// Set Bulk IN packet ready
		[P_USBD_EPEvent]=r1
		
		pop bp from [sp]
		retf
////////////////////////////////////////////////////////////////
_GetLBA:
		push r3 to [sp]
		
		r3=seg _R_CBW_PKT
		ds=r3
		r3= offset _R_CBW_PKT
		
		r3+=17
		
		r2=ds:[r3++]
		r2=r2 lsl 4
		r2=r2 lsl 4
		r4=ds:[r3++]
		r2|=r4				// High Byte
		
		r1=ds:[r3++]
		r1=r1 lsl 4
		r1=r1 lsl 4
		r4=ds:[r3]
		r1|=r4				// Low Byte
				
		pop r3 from [sp]
		retf
////////////////////////////////////////////////////////////////
_GetTransLeng:
		push r2 to [sp]
		r1=seg _R_CBW_PKT
		ds=r1
		r1= offset _R_CBW_PKT
		
		r1+=22
		r2=ds:[r1++]
		r2=r2 lsl 4
		r2=r2 lsl 4
		r1=ds:[r1]
		r1|=r2		
		
		pop r2 from [sp]
		retf
/////////////////////////////////////////////////////////////
_GetLUN:
		r1=seg _R_CBW_PKT
		ds=r1
		r1= offset _R_CBW_PKT
		
		r1+=13
		r1=ds:[r1]
		
		retf
////////////////////////////////////////////////////////////////
_GetCSWLength:
		r3=seg _R_CBW_PKT
		ds=r3
		r3= offset _R_CBW_PKT
		
		r3+=8
		r1=ds:[r3++]
		r4=ds:[r3++]
		r4=r4 lsl 4
		r4=r4 lsl 4
		r1|=r4			// Low Byte
		
		r2=ds:[r3++]
		r4=ds:[r3++]
		r4=r4 lsl 4
		r4=r4 lsl 4
		r2|=r4			// High Bytw
		
		retf
		

////////////////////////////////////////////////////////////////
//Device table info below
//0~3:		USB V/P ID (4 bytes)
//4~19:		USB manufacturer string (16 bytes)
//20~51:	USB product string (32 bytes)
//52~83: 	serial number (32 bytes)
//84~91: 	SCSI VID
//92~107:	SCSI PID
//108~111:  SCSI Version
//108~255:	Reserved1
//256: 		Serial number enable flag
//257:		Max lun
//258~511:	Reserved2 
////////////////////////////////////////////////////////////////
_ReadDriveInfoTable:
		push bp to [sp]
		bp=sp+1
//		push r1,r4 to [sp]
		
		bp = USB_RW_Buffer
		r1 = 0x200
		r2 = 0x00
	//clear R/W buffer	
	?L_ClearBuffer:		
		[bp++] = r2
		r1 -= 1
		jnz ?L_ClearBuffer

		
		bp = USB_RW_Buffer
		//USB VPID (4 bytes)	
		r1=seg _Device_Descriptor
		ds=r1
		r1= offset _Device_Descriptor
		r3 = 4
		r1 += 8		
	?L_GetUSBVPID:			
		r2 = ds:[r1++]
		[bp++] = r2
		r3-=1
		jnz ?L_GetUSBVPID
	
		//USB Manufacturer string
		r1=seg _String1_Descriptor
		ds=r1
		r1= offset _String1_Descriptor
		r3 = 16
		r1 += 2		
	?L_GetUSBManuString:			
		r2 = ds:[r1++]
		[bp++]=r2		
		r3-=1
		jnz ?L_GetUSBManuString
		
		//USB Production string
		r1=seg _String2_Descriptor
		ds=r1
		r1= offset _String2_Descriptor
		r3 = 32
		r1 += 2		
	?L_GetUSBProdString:		
		r2 = ds:[r1++]
		[bp++]=r2		
		r3-=1
		jnz ?L_GetUSBProdString
		
		//Serial number string
		r1=seg _String3_Descriptor
		ds=r1
		r1= offset _String3_Descriptor
		r3 = 32
		r1 += 2		
	?L_GetSNString:		
		r2 = ds:[r1++]
		[bp++]=r2		
		r3-=1
		jnz ?L_GetSNString

		//SCSI V/PID string
		r1=seg _SCSIInquiryData
		ds=r1
		r1= offset _SCSIInquiryData
		r3 = 8+16+4
		r1 += 8		
	?L_SCSIVPIDString:		
		r2 = ds:[r1++]
		[bp++]=r2		
		r3-=1
		jnz ?L_SCSIVPIDString
	


	////////////////////////////////
		bp = USB_RW_Buffer+256
		//Serial number option		
		r1=seg _Device_Descriptor
		ds=r1
		r1= offset _Device_Descriptor
		r1 += 16		
		r2 = ds:[r1]
		[bp++]=r2		
		
		//Max lun		
		r1=seg _Max_LUN
		ds=r1
		r1= offset _Max_LUN
		r2 = ds:[r1++]
		[bp++]=r2				
		
		
		r1 = [_CSW_Residue]				//table 512 bytes
		r2 = 0							//Start address
		call TxUSB_RW_Buffer

		
//		pop r1,r4 from [sp]
		pop bp from [sp]
		retf
		
////////////////////////////////////////////////////////////////
_RD_Until_Bulk_IN_NACK:
		push r1, r2 to [sp]
		r2 = 0
	?L_BI_NACK:
		r2 -= 1
		jz ?L_BulkInTimeout
		r1=[P_USBD_EPEvent]
		test r1,0x0400
		jz ?L_BI_NACK
	?L_BulkInTimeout:	
		r1=0x0400
		[P_USBD_EPEvent]=r1
	
		pop r1, r2 from [sp]
		retf
////////////////////////////////////////////////////////////////
_RD_Until_Bulk_OUT_NACK:
		push r1, r2 to [sp]
		r2 = 0
	?L_BO_NACK:
		r2 -= 1
		jz ?L_BulkOutTimeout
		r1=[P_USBD_EPEvent]
		test r1,0x2000
		jz ?L_BO_NACK
	?L_BulkOutTimeout:	
		r1=0x2000
		[P_USBD_EPEvent]=r1
	
		pop r1, r2 from [sp]
		retf
////////////////////////////////////////////////////////////
//r1: capacity, 
///////////////////////////////////////////////////////////
Read_Format_Capacity_Nand:
		push r1,r4 to [sp]
		r2=[r1++]		// Low Byte
		r3=[r1]			// High Byte
		r4=r3 lsr 4
		r4=r4 lsr 4
		
		[P_USBD_BIData]=r4		//4 Number of Blocks
		
		r4=r3 & 0x00FF
		[P_USBD_BIData]=r4		//5 Number of Blocks
		
		r4=r2 lsr 4
		r4=r4 lsr 4

		[P_USBD_BIData]=r4		//6 Number of Blocks
		
		r4=r2 & 0x00FF
		[P_USBD_BIData]=r4		//7 Number of Blocks
		
		pop r1,r4 from [sp]
		retf
		
////////////////////////////////////////////////////////////////		
_GetBufferPointer:
		push bp to [sp]
		r1=offset USB_RW_Buffer
		r2=seg USB_RW_Buffer

		pop bp from [sp]
		retf
		
////////////////////////////////////////////////////////////////
//Tx data in USB_RW_Buffer to host.
//void TxUSB_RW_Buffer(unsigned int unLength, unsigned int unStartAddr);
//Parameter1 (WORD): 
//				unLength:The length of data want to Tx to host, r1
//				unStartAddr: Start address of RW_Buffer, shift byte unit
////////////////////////////////////////////////////////////////
_TxUSB_RW_Buffer:
		push bp to [sp]
		bp = sp+1
		r1 = [bp+3]			//Tx length
		r2 = [bp+4]			//Start address
		call TxUSB_RW_Buffer
		pop	bp from [sp]
		retf

TxUSB_RW_Buffer:		
		push r1, r5 to [sp]
		bp = USB_RW_Buffer
		bp += r2			//shift start address		
		r2 = r1				//length
		r3 = r2 & 0x3F		//remainder
		r2 = r2 lsr 4		
		r2 = r2 lsr 2		//quotient	
	
	?L_CheckLength:
		r4 = 64		
		cmp	r2, 0x0000			
		jne ?L_Descnt
		r1 = r3
		r3 = 0
		r4 = r1				//Set remainder length as Tx length
		jnz ?L_FillData
		jmp ?L_TxFinish
	?L_Descnt:
		r2 -= 1
	?L_FillData:	
		r1 = [bp++]
		[P_USBD_BIData] = r1
		r4-=1
		jnz ?L_FillData
	
		r1 = 0x0100				// Set Bulk IN packet ready
		[P_USBD_EPEvent] = r1
		
		call _CheckFIFOTxFinish

		jmp ?L_CheckLength
		
	?L_TxFinish:	
		pop r1,r5 from [sp]
		retf				


////////////////////////////////////////////////////////////////
//Tx data in USB_RW_Buffer_Word to host.
//void TxUSB_RW_Buffer_Word(unsigned int unLength, unsigned int unStartAddr);
//Parameter1 (WORD): 
//				unLength:The length of data want to Tx to host, r1
//				unStartAddr: Start address of RW_Buffer, shift word unit
////////////////////////////////////////////////////////////////
_TxUSB_RW_Buffer_Word:
		push bp to [sp]
		bp = sp+1
		r1 = [bp+3]		
		r2 = [bp+4]		
		call TxUSB_RW_Buffer_Word
		pop	bp from [sp]
		retf

TxUSB_RW_Buffer_Word:		
		push r1, r5 to [sp]
		bp = USB_RW_Buffer
		bp += r2
		r2 = r1				//length
		r3 = r2 & 0x3F		//remainder
		r2 = r2 lsr 4		
		r2 = r2 lsr 2		//quotient	
	
	?L_CheckLength:
		r4 = 64		
		cmp	r2, 0x0000			
		jne ?L_Descnt
		r1 = r3
		r3 = 0
		r4 = r1				//Set remainder length as Tx length
		jnz ?L_FillData
		jmp ?L_TxFinish
	?L_Descnt:
		r2 -= 1
	?L_FillData:	
		r1 = [bp++]
		[P_USBD_BIData] = r1
		r1 = r1 lsr 4
		r1 = r1 lsr 4
		[P_USBD_BIData] = r1 
		r4-=2
		jnz ?L_FillData
	
		r1 = 0x0100				// Set Bulk IN packet ready
		[P_USBD_EPEvent] = r1
		
		call _CheckFIFOTxFinish

		jmp ?L_CheckLength
		
	?L_TxFinish:	
		pop r1,r5 from [sp]
		retf				
		
/////////////////////////////////////////////////////////////////////////////////////////////
//Check bulk-in FIFO Tx data complete. Use this function when Bulk-in FIFO have data need Tx out.
//Notes: The bit 10 in P_USBD_EPEvent register must be clear before FIFO Tx to host.
////////////////////////////////////////////////////////////////////////////////////////////
_CheckFIFOTxFinish:
		push r1, r2 to [sp]
		r2=0
	?L_CHK_FIFO_Tx:				//check Tx package ready
		r2 -= 1
		jz ?L_CheckFIFOTimeout
		r1=[P_USBD_EPEvent]
		test r1,0x0100
		jnz ?L_CHK_FIFO_Tx
	?L_CheckFIFOTimeout:	
		r1 = 0x0400
		[P_USBD_EPEvent] = r1	// Bulk IN NACK Reset		
		call _RD_Until_Bulk_IN_NACK
		pop r1, r2 from [sp]
		retf		
		
		
////////////////////////////////////////////////////////////////
//Rx data in USB_RW_Buffer from host.
//Parameter1 (WORD): 
//				The length of data want to Rx to host, r1
//Caution: use word save in USB_RW_Buffer.
////////////////////////////////////////////////////////////////
_RxUSB_RW_Buffer:
		push bp to [sp]
		bp = sp+1
		r1 = [bp+3]		
		call RxUSB_RW_Buffer
		
?tr_end:	
		pop	bp from [sp]
		retf

RxUSB_RW_Buffer:
		push r1, r5 to [sp]
		bp = USB_RW_Buffer
		r3 = r1 lsr 4		
		r3 = r3 lsr 2		//quotient	
		jz ?L_RxRemaind		
		
		r8 = 64
	?L_RxData:
		call RxUSBData
		r1=[_tra_end]
		cmp r1,0xffff
		jz ?L_RxFinish
		r3 -= 1			
		jnz ?L_RxData
	
	?L_RxRemaind:	
		//Rx remaind data	
		r8 = r1 & 0x3F		//remainder
		jz ?L_RxFinish
		call RxUSBData	
		

//		r2 = 0x0400						// 
//		[P_USBD_EPEvent] = r2
		
//		r2 = [P_USBD_Config];			// set bit5 to 1
//		r2 |= 0x0020
//		[P_USBD_Config] = r2;
?L_RxFinish:
		pop r1,r5 from [sp]
		retf	
		
RxUSBData:
	push r1, r4 to [sp]
		
	?L_RxData:
		r2 = 0x0800						// Enable BULK OUT Packet
		[P_USBD_EPEvent] = r2
		
		r4 = 0
	?L_CheckRxUSBData:	
		r4 -= 1
		jz ?L_RxTimeout					//check timeout
		r2 = [P_USBD_EPEvent]			//Check data received
		r2 = r2 & 0x1000
		jz  ?L_CheckRxUSBData

		r2 = 0x0
	?L_GetBulkoutData:		
		r9 = [P_USBD_BOData]		//Get bulk-out data from FIFO Low
		r4 = [P_USBD_BOData]		//Get bulk-out data from FIFO High
		r4 = r4 lsl 4
		r4 = r4 lsl 4
		r4 += r9 
		[bp++] = r4					//Put data to read/write buffer
		r2 += 2				
		cmp r2,r8 
		jb ?L_GetBulkoutData	
		
		r2=0x1000					// clear Bulk OUT packet ready flag
		[P_USBD_EPEvent]=r2

		r2 = 0x0010					//
		[P_USBD_EPEvntClear] = r2;

		r2 = 0x2000
		[P_USBD_EPEvent] = r2;		// Bulk OUT NACK Reset	
		pop r1, r4 from [sp]
		retf	

	?L_RxTimeout:
		call _USB_Insert_TimeOut
		r1=0xffff
		[_tra_end]=r1
		pop r1, r4 from [sp]
		retf	
		
				
_GetICVersion:
		push bp to [sp]
		push r1,r4 to [sp]
		ds = seg _DeviceVersionInfo
		r1 = offset _DeviceVersionInfo
		r2 = [_CSW_Residue]
		jz ?L_ICVerFinish
		
	?L_ICVerLen:	
		r4 = ds:[r1++]		
		[P_USBD_BIData]=r4		
		r2 -= 1
		jnz ?L_ICVerLen

		r1=0x0100				// Set Bulk IN packet ready
		[P_USBD_EPEvent]=r1
	?L_ICVerFinish:
		pop r1, r4 from [sp]
		pop bp from [sp]
		retf

//Goto ram and will not return 
_GotoRam:
	push bp to [sp]
	bp=sp+1
	r3=[bP+3]
	r4=[bp+4]	//r4:r3 is goto ram addr		
	goto mr
	nop
	jmp	$

//Call a function in ram then return 		
_CallRam:
	push bp to [sp]
	bp=sp+1
	r3=[bP+3]
	r4=[bp+4]	//r4:r3 is call ram addr		
r1=0
r2=2	
push r1,r2 to [sp]	

	call mr

pop r1,r2 from [sp]	
	pop bp from [sp]
	retf		
				
		
//USB_Initial:	.section	.code
_USB_Initial: .PROC
F_USB_Initial:	

		r1=0x3a50				// EP1: Bulk IN		EP2: Bulk OUT
		[P_USBD_Device]=r1		// EP3: INT IN		EP4: Reserved
		
		r1=0x0000				// Disable DMA Mode
		[P_USBD_Function]=r1
		
		r1=0x8429
		[P_USBD_INTEN]=r1

		r1 = 0x001A				// 
		r1 |= 0x3FC0
		[P_USBD_Config]=r1		//


		r1=0
		[_R_USB_State_Machine]=r1
		[_USB_Status]=r1
		[_Bulk_Stall]=r1
		[_R_USB_Suspend]=r1
		[_USBWriteProtect] = r1;
		[_CDROMEnable] = r1
		[_R_USB_RW_Busy] = r1
		[_Sense_Code] = r1
		r1 = 1
		[_CDROMDelay] = r1	

//		call _CheckCDRomValid
//		call main
				
		retf
		.endp		
				

//============================================
_Send_To_USB_DMA_USB:
		push bp to [sp]
		bp=sp+1
		
		r1 = [bp + 6]
		jz ?L_Not_Finish
		goto ?L_Finish
		
?L_Not_Finish:
		r1=0x000C				// Enable Bulk Auto Set
		[P_USBD_EPAutoSet]=r1
		
		r1=256
		[P_USBD_DMAWrtCountL]=r1
		r1=0x0000
		[P_USBD_DMAWrtCountH]=r1
		
		
		r1 = [bp + 5]
		jz ?Send_DMA_USB_Channel0
		cmp r1,0x01
		je ?Send_DMA_USB_Channel1
		cmp r1,0x02
		je ?Send_DMA_USB_Channel2_Near
		cmp r1,0x03
		je ?Send_DMA_USB_Channel3_Near
		r1 = 0xffff
		pop bp from [sp]
		retf
?Send_DMA_USB_Channel2_Near:
		goto ?Send_DMA_USB_Channel2
?Send_DMA_USB_Channel3_Near:				
		goto ?Send_DMA_USB_Channel3
		
?Send_DMA_USB_Channel0:
		r1=0x0200				// DMA Channel Reset
		[P_DMA_Ctrl0]=r1
		
		r1=0x0001				// Clear USB DMA INT Flag
		[P_USBD_DMAINT]=r1	
		
		r1=[P_DMA_SS]
		r1&=0xfff0
		[P_DMA_SS]=r1
		
		r1=[bp+3]				// Sector_Buffer Address
		[P_DMA_SRC_AddrL0]=r1
		r1=P_USBD_BIData
		[P_DMA_TAR_AddrL0]=r1
		r1=256					// 256 words
		[P_DMA_TCountL0]=r1
		r1=[bp+4]
		[P_DMA_SRC_AddrH0]=r1
		r1=0x00
		[P_DMA_TAR_AddrH0]=r1
		r1=0x00
		[P_DMA_TCountH0]=r1				
		
		r1=0x0200				// Enable DMA mode
		[P_USBD_Function]=r1
		
		r1=0x4445  //0x4545
		[P_DMA_Ctrl0]=r1
		goto ?Send_DMA_USB_Channel_End
		
?Send_DMA_USB_Channel1:	
		r1=0x0200				// DMA Channel Reset
		[P_DMA_Ctrl1]=r1
		
		r1=0x0001				// Clear USB DMA INT Flag
		[P_USBD_DMAINT]=r1	
		
		r1=[P_DMA_SS]
		r1&=0xff0f
		[P_DMA_SS]=r1
		
		r1=[bp+3]				// Sector_Buffer Address
		[P_DMA_SRC_AddrL1]=r1
		r1=P_USBD_BIData
		[P_DMA_TAR_AddrL1]=r1
		r1=256					// 256 words
		[P_DMA_TCountL1]=r1
		r1=[bp+4]
		[P_DMA_SRC_AddrH1]=r1
		r1=0x00
		[P_DMA_TAR_AddrH1]=r1
		r1=0x00
		[P_DMA_TCountH1]=r1				
		
		r1=0x0200				// Enable DMA mode
		[P_USBD_Function]=r1
		
		r1=0x4445  //0x4545
		[P_DMA_Ctrl1]=r1
		goto  ?Send_DMA_USB_Channel_End
?Send_DMA_USB_Channel2:	
		r1=0x0200				// DMA Channel Reset
		[P_DMA_Ctrl2]=r1
		
		r1=0x0001				// Clear USB DMA INT Flag
		[P_USBD_DMAINT]=r1
			
		r1=[P_DMA_SS]
		r1&=0xf0ff
		[P_DMA_SS]=r1
		
		r1=[bp+3]				// Sector_Buffer Address
		[P_DMA_SRC_AddrL2]=r1
		r1=P_USBD_BIData
		[P_DMA_TAR_AddrL2]=r1
		r1=256					// 256 words
		[P_DMA_TCountL2]=r1
		r1=[bp+4]
		[P_DMA_SRC_AddrH2]=r1
		r1=0x00
		[P_DMA_TAR_AddrH2]=r1
		r1=0x00
		[P_DMA_TCountH2]=r1				
		
		r1=0x0200				// Enable DMA mode
		[P_USBD_Function]=r1
		
		r1=0x4445  //0x4545
		[P_DMA_Ctrl2]=r1
		jmp ?Send_DMA_USB_Channel_End
?Send_DMA_USB_Channel3:	
		r1=0x0200				// DMA Channel Reset
		[P_DMA_Ctrl3]=r1
		
		r1=0x0001				// Clear USB DMA INT Flag
		[P_USBD_DMAINT]=r1
			
		r1=[P_DMA_SS]
		r1&=0x0fff
		[P_DMA_SS]=r1
		
		r1=[bp+3]				// Sector_Buffer Address
		[P_DMA_SRC_AddrL3]=r1
		r1=P_USBD_BIData
		[P_DMA_TAR_AddrL3]=r1
		r1=256					// 256 words
		[P_DMA_TCountL3]=r1
		r1=[bp+4]
		[P_DMA_SRC_AddrH3]=r1
		r1=0x00
		[P_DMA_TAR_AddrH3]=r1
		r1=0x00
		[P_DMA_TCountH3]=r1				
		
		r1=0x0200				// Enable DMA mode
		[P_USBD_Function]=r1
		
		r1=0x4445  //0x4545
		[P_DMA_Ctrl3]=r1
?Send_DMA_USB_Channel_End:	
		
		r1=0					// r1 = 0x0 : Success
		pop bp from [sp]		// r1 = 0xA : TimeOut
		retf		
		
?L_Finish:
		r2 = 0xffff 	//count Num Low Bit
		r3 = 0x00ff		//count Num High Bit
		
		r1 = [bp + 5]
		jz ?Send_USB_Channel0
		cmp r1,0x01
		je ?Send_USB_Channel1
		cmp r1,0x02
		je ?Send_USB_Channel2
		cmp r1,0x03
		je ?Send_USB_Channel3_Near
		r1 = 0xffff
		pop bp from [sp]
		retf
?Send_USB_Channel3_Near:
		goto ?Send_USB_Channel3
				
?Send_USB_Channel0:	
		r2 -= 1
		jz 	?Send_USB_C0_Erro_Temp
		
		r1 = [_R_USB_Suspend]
		jnz ?Send_USB_C0_Erro
		
		r1=[P_USBD_DMAINT]
		test r1,0x0001
		jz ?Send_USB_Channel0
		r2 = 1
		jmp ?Send_USB_C0_No_Erro
					
?Send_USB_C0_Erro_Temp:	
		r3 -= 1
		jnz ?Send_USB_Channel0
		
?Send_USB_C0_Erro:
		r2 = 0
?Send_USB_C0_No_Erro:			
		r1=0x0001
		[P_DMA_INT]=r1
		[P_USBD_DMAINT] = r1
		jmp ?Send_USB_ChannelEnd
		
?Send_USB_Channel1:	
		r2 -= 1
		jz ?Send_USB_C1_Erro_Temp

		r1 = [_R_USB_Suspend]
		jnz ?Send_USB_C1_Erro

		r1=[P_USBD_DMAINT]
		test r1,0x0001
		jz ?Send_USB_Channel1
		r2 = 1
		jmp ?Send_USB_C1_No_Erro
		
?Send_USB_C1_Erro_Temp:	
		r3 -= 1
		jnz ?Send_USB_Channel1		

?Send_USB_C1_Erro:
		r2 = 0
?Send_USB_C1_No_Erro:		
		r1=0x0002
		[P_DMA_INT]=r1
		r1 = 0x0001
		[P_USBD_DMAINT] = r1
		jmp ?Send_USB_ChannelEnd
				
?Send_USB_Channel2:		
		r2 -= 1
		jz ?Send_USB_C2_Erro_Temp

		r1 = [_R_USB_Suspend]
		jnz ?Send_USB_C2_Erro

		r1=[P_USBD_DMAINT]
		test r1,0x0001
		jz ?Send_USB_Channel2
		r2 = 1
		jmp ?Send_USB_C2_No_Erro
		
?Send_USB_C2_Erro_Temp:	
		r3 -= 1
		jnz ?Send_USB_Channel2	
		
?Send_USB_C2_Erro:
		r2 = 0					
?Send_USB_C2_No_Erro:		
		r1=0x0004
		[P_DMA_INT]=r1
		r1 = 0x0001
		[P_USBD_DMAINT] = r1
		jmp ?Send_USB_ChannelEnd
		
?Send_USB_Channel3:
		r2 -= 1
		jz ?Send_USB_C3_Erro_Temp		
		
		r1 = [_R_USB_Suspend]
		jnz ?Send_USB_C3_Erro

		r1=[P_USBD_DMAINT]
		test r1,0x0001
		jz ?Send_USB_Channel3
		r2 = 1
		jmp ?Send_USB_C3_No_Erro
			
?Send_USB_C3_Erro_Temp:	
		r3 -= 1
		jnz ?Send_USB_Channel3
		
?Send_USB_C3_Erro:
		 r2 = 0							
?Send_USB_C3_No_Erro:		
		r1=0x0008
		[P_DMA_INT]=r1
		r1 = 0x0001
		[P_USBD_DMAINT] = r1
		
?Send_USB_ChannelEnd:				
		r1=0x0000				// Disable Bulk IN Auto Set
		[P_USBD_EPAutoSet]=r1
		
		r1=0x0000				// Disable DMA mode
		[P_USBD_Function]=r1
		
		r1=0x0600
		[P_USBD_EPEvent]=r1		
		
		r1 = r2 
		jz ?Send_USB_Erro_End
		r1 = 0x00
		pop bp from [sp]		// r1 = 0xA : TimeOut
		retf
		
?Send_USB_Erro_End:
		r1=0xfffe					// r1 = 0x0 : Success
		pop bp from [sp]		// r1 = 0xA : TimeOut
		retf		
///////////////////////////			
////////////////////////////////////////////////////////////////				
_Receive_From_USB_DMA_USB:
		push bp to [sp]
		bp=sp+1		
		
		r1=0x000C				// Enable Bulk Auto Set
		[P_USBD_EPAutoSet]=r1
		
		r1=256
		[P_USBD_DMAWrtCountL]=r1
		r1=0x0000
		[P_USBD_DMAWrtCountH]=r1
		
//.comment @
		r2 = 0xffff 	//count Num Low Bit //USB timeout
		r3 = 0x00ff		//count Num High Bit//USB timeout
		r1 = [BP + 5]
		jz  ?DMA_channel0
        cmp r1 , 1
        je  ?DMA_channel1
        cmp r1 , 2
        je  ?DMA_channel2
        cmp r1 , 3
        je  ?DMA_channel3
        r1 = 0xffff // no DMA channel 
        retf            
?DMA_channel0:
        goto ?USB_DMA_0
?DMA_channel1:
		goto ?USB_DMA_1
?DMA_channel2:
		goto ?USB_DMA_2
?DMA_channel3:              
		goto ?USB_DMA_3
?USB_DMA_0:
		r1=0x0200				// DMA Channel Reset		
		[P_DMA_Ctrl0]=r1
		
		r1=0x0001				// Clear USB DMA INT Flag
		[P_USBD_DMAINT]=r1	
		
		r1=[P_DMA_SS]
		r1&=0xfff0
		[P_DMA_SS]=r1
		
		r1=P_USBD_BOData
		[P_DMA_SRC_AddrL0]=r1
		
		r1=[bp+3]				// Sector_Buffer Address
		[P_DMA_TAR_AddrL0]=r1
		
		r1=256					// 256 words
		[P_DMA_TCountL0]=r1
		r1=0x00
		[P_DMA_SRC_AddrH0]=r1
		r1=[bp+4]
		[P_DMA_TAR_AddrH0]=r1
		r1=0x00
		[P_DMA_TCountH0]=r1	
	
		r1=0x0800
		[P_USBD_EPEvent]=r1		// Enable Bulk OUT Packet
								// After Enable this bit, the data from Host 
								// will load into Bulk OUT Data Register.
?L_Finish0:
		r2 -= 1
		jz ?L_Finish0_Continue
		
		r1 = [_R_USB_Suspend]
		jnz ?L_Finish0_Erro

		r1=[P_USBD_DMAINT]               //使用USB int flag 判斷會有問題
		test r1,0x0001                   //modified by peng 2006/07/31
//        r1=[P_DMA_INT]                   //使用DMA channel INT 判斷 
//        test r1,0x0008 
        		
		jz ?L_Finish0
		r2 = 1
		jmp ?L_Finish0_No_Erro
				
?L_Finish0_Continue:	
		r3 -= 1
		jnz ?L_Finish0
		
?L_Finish0_Erro:
		r2 = 0		
?L_Finish0_No_Erro:			
		r1=0x0001
		[P_DMA_INT]=r1
		[P_USBD_DMAINT] = r1
		goto ?end_dma_config
		
?USB_DMA_1:
		r1=0x0200				// DMA Channel Reset		
		[P_DMA_Ctrl1]=r1
		
		r1=0x0001				// Clear USB DMA INT Flag
		[P_USBD_DMAINT]=r1	
		
		r1=[P_DMA_SS]
		r1&=0xff0f
		[P_DMA_SS]=r1
		
		r1=P_USBD_BOData
		[P_DMA_SRC_AddrL1]=r1
		
		r1=[bp+3]				// Sector_Buffer Address
		[P_DMA_TAR_AddrL1]=r1
		
		r1=256					// 256 words
		[P_DMA_TCountL1]=r1
		r1=0x00
		[P_DMA_SRC_AddrH1]=r1
		r1=[bp+4]
		[P_DMA_TAR_AddrH1]=r1
		r1=0x00
		[P_DMA_TCountH1]=r1	
		
		r1=0x0400				// Enable DMA mode
		[P_USBD_Function]=r1
		
		r1=0x4885  //0x4985
		[P_DMA_Ctrl1]=r1

		r1=0x0800
		[P_USBD_EPEvent]=r1		// Enable Bulk OUT Packet
								// After Enable this bit, the data from Host 
								// will load into Bulk OUT Data Register.
?L_Finish1:
		r2 -= 1
		jz ?L_Finish1_Continue
		
		r1 = [_R_USB_Suspend]
		jnz ?L_Finish1_Erro

		r1=[P_USBD_DMAINT]               //使用USB int flag 判斷會有問題
		test r1,0x0001                   //modified by peng 2006/07/31
//        r1=[P_DMA_INT]                   //使用DMA channel INT 判斷 
//        test r1,0x0008 		
		jz ?L_Finish1
		r2 = 1
		jmp ?L_Finish1_No_Erro
				
?L_Finish1_Continue:	
		r3 -= 1
		jnz ?L_Finish1
		
?L_Finish1_Erro:
		r2 = 0		
?L_Finish1_No_Erro:			
		r1=0x0002
		[P_DMA_INT]=r1
		r1 = 0x0001		
		[P_USBD_DMAINT] = r1
		goto ?end_dma_config
		
?USB_DMA_2:
		r1=0x0200				// DMA Channel Reset		
		[P_DMA_Ctrl2]=r1
	
		r1=0x0001				// Clear USB DMA INT Flag
		[P_USBD_DMAINT]=r1	
		
		r1=[P_DMA_SS]
		r1&=0xf0ff
		[P_DMA_SS]=r1
		
		r1=P_USBD_BOData
		[P_DMA_SRC_AddrL2]=r1
		
		r1=[bp+3]				// Sector_Buffer Address
		[P_DMA_TAR_AddrL2]=r1
		
		r1=256					// 256 words
		[P_DMA_TCountL2]=r1
		r1=0x00
		[P_DMA_SRC_AddrH2]=r1
		r1=[bp+4]
		[P_DMA_TAR_AddrH2]=r1
		r1=0x00
		[P_DMA_TCountH2]=r1	

		r1=0x0400				// Enable DMA mode
		[P_USBD_Function]=r1
		
		r1=0x4885  //0x4985
		[P_DMA_Ctrl2]=r1
		
		r1=0x0800
		[P_USBD_EPEvent]=r1		// Enable Bulk OUT Packet
								// After Enable this bit, the data from Host 
								// will load into Bulk OUT Data Register.

?L_Finish2:

		r2 -= 1
		jz ?L_Finish2_Continue
		
		r1 = [_R_USB_Suspend]
		jnz ?L_Finish2_Erro

		r1=[P_USBD_DMAINT]               //使用USB int flag 判斷會有問題
		test r1,0x0001                   //modified by peng 2006/07/31
//        r1=[P_DMA_INT]                   //使用DMA channel INT 判斷 
//        test r1,0x0008 		
		
		jz ?L_Finish2
		r2 = 1
		jmp ?L_Finish2_No_Erro
				
?L_Finish2_Continue:	
		r3 -= 1
		jnz ?L_Finish2
		
?L_Finish2_Erro:
		r2 = 0		
?L_Finish2_No_Erro:			
		r1=0x0004
		[P_DMA_INT]=r1
		r1 = 0x0001
		[P_USBD_DMAINT]  =r1
		goto ?end_dma_config
		
?USB_DMA_3:
		r1=0x0200				// DMA Channel Reset		
		[P_DMA_Ctrl3]=r1
	
		r1=0x0001				// Clear USB DMA INT Flag
		[P_USBD_DMAINT]=r1	
		
		r1=[P_DMA_SS]
		r1&=0x0fff
		[P_DMA_SS]=r1
		
		r1=P_USBD_BOData
		[P_DMA_SRC_AddrL3]=r1
		
		r1=[bp+3]				// Sector_Buffer Address
		[P_DMA_TAR_AddrL3]=r1
		
		r1=256					// 256 words
		[P_DMA_TCountL3]=r1
		r1=0x00
		[P_DMA_SRC_AddrH3]=r1
		r1=[bp+4]
		[P_DMA_TAR_AddrH3]=r1
		r1=0x00
		[P_DMA_TCountH3]=r1	
		
		r1=0x0400				// Enable DMA mode
		[P_USBD_Function]=r1
		
		r1=0x4885  //0x4985
		[P_DMA_Ctrl3]=r1
		
		r1=0x0800
		[P_USBD_EPEvent]=r1		// Enable Bulk OUT Packet
								// After Enable this bit, the data from Host 
								// will load into Bulk OUT Data Register.
?L_Finish3:

		r2 -= 1
		jz ?L_Finish3_Continue
		
		r1 = [_R_USB_Suspend]
		jnz ?L_Finish3_Erro

		r1=[P_USBD_DMAINT]               //使用USB int flag 判斷會有問題
		test r1,0x0001                   //modified by peng 2006/07/31
//        r1=[P_DMA_INT]                   //使用DMA channel INT 判斷 
//        test r1,0x0008 
        
		jz ?L_Finish3
		r2 = 1
		jmp ?L_Finish3_No_Erro
				
?L_Finish3_Continue:	
		r3 -= 1
		jnz ?L_Finish3
		
?L_Finish3_Erro:
		r2 = 0		
?L_Finish3_No_Erro:			
		r1=0x0008
		[P_DMA_INT]=r1
		r1 = 0x0001
		[P_USBD_DMAINT] = r1

?end_dma_config:									
		
		r1=0x0000				// Disable Bulk IN Auto Set
		[P_USBD_EPAutoSet]=r1
		
		r1=0x0000				// Disable DMA mode
		[P_USBD_Function]=r1
//@

		
		r1=0x0010
		[P_USBD_EPEvntClear]=r1

		r1=0x2000;		// Bulk OUT NACK Reset
		[P_USBD_EPEvent]=r1
		
		r1 = r2 
		jz ?L_Finish_Erro
		r1 = 0x00
		pop bp from [sp]		 
		retf		
?L_Finish_Erro:	
		r1 = 0xfffe
		pop bp from [sp]		 
		retf	
//========================================================
//Function Name:	TXToUSB
//Syntax:			unsigned int TXToUSB(unsigned long Buffer_Address, unsigned int Sector_Number)
//Purpose:			Transmit data to USB
//Note:
//Parameters:		Buffer address, Sector Number
//Return:			none
//=======================================================
.public _TXToUSB_USB		
_TXToUSB_USB:
		push bp to [sp]
		bp=sp+1
		
		r1 = [bp+7]
		jnz ?C_DMA_Config
				
		r1 = 0x0001
		[P_USBD_DMAINT]	= r1
		
		r1=0x000C				// Enable Bulk Auto Set
		[P_USBD_EPAutoSet]=r1
		
		r3=[bp+5]
		r4=r3 lsr 4
		r4=r4 lsr 4				// DMA High Count
		
		r3=r3 lsl 4
		r3=r3 lsl 4				// DMA Low Count
		
		[P_USBD_DMAWrtCountL]=r3
		[P_USBD_DMAWrtCountH]=r4
						
		r1=0x0200				// DMA Channel Reset
		[P_DMA_Ctrl1]=r1
		
		r1=[P_DMA_SS]
		r1&=0xff0f
		[P_DMA_SS]=r1
		
		r1=[bp+3]				// Sector_Buffer Address
		[P_DMA_SRC_AddrL1]=r1
		r1=P_USBD_BIData
		[P_DMA_TAR_AddrL1]=r1
		

		[P_DMA_TCountL1]=r3
		[P_DMA_TCountH1]=r4	
		
		r1=[bp+4]
		[P_DMA_SRC_AddrH1]=r1
		r1=0x00
		[P_DMA_TAR_AddrH1]=r1
			
		
		r1=0x0200				// Enable DMA mode
		[P_USBD_Function]=r1
		
		r1=0x4445
		[P_DMA_Ctrl1]=r1
		
		pop bp from [sp]
		retf
		
?C_DMA_Config:		
		r2=0x0000
	?L_Finish:
		r2-=1
		jz ?L_DMA_TimeOut
		r1=[P_USBD_DMAINT]
		test r1,0x0001
		jz ?L_Finish
		
		jmp ?L_DMA_Success
	?L_DMA_TimeOut:
		r1=0x00
		[P_DMA_TCountL1]=r1
		[P_DMA_TCountH1]=r1
		r1=0x0200				// DMA Channel Reset
		[P_DMA_Ctrl1]=r1
		
		r1=0x0000				// Disable Bulk IN Auto Set
		[P_USBD_EPAutoSet]=r1
		
		r1=0x0000				// Disable DMA mode
		[P_USBD_Function]=r1
		r1=0xFFFE
		pop bp from [sp]
		retf
		
	?L_DMA_Success:
		r1=0x0002
		[P_DMA_INT]=r1
		
		
		r1 = 0x0001
		[P_USBD_DMAINT]	= r1
		
		r1=0x0000				// Disable Bulk IN Auto Set
		[P_USBD_EPAutoSet]=r1
		
		r1=0x0000				// Disable DMA mode
		[P_USBD_Function]=r1
	
		r1=0x00
		pop bp from [sp]
		retf		
		
.public _ModeSenseData						
_ModeSenseData:
		push bp to [sp]
		bp=sp+1
		push r1,r4 to [sp]

		r1=seg _ScsiModeSense10CDROM
		ds=r1
		r1=offset _ScsiModeSense10CDROM
		r4=offset _ScsiModeSense10CDROM_End

		r4=r4-r1

		r2=[_CSW_Residue]				// Inquiry data length
		cmp r2, r4
		jbe ?L_ModeSenseData
		
		r2 = r4							//Fix reply length, prevent reply too much
	?L_ModeSenseData:
		r3=ds:[r1++]
		[P_USBD_BIData]=r3
		r2-=1
		jnz ?L_ModeSenseData
	
		r1=0x0100						// Set Bulk IN packet ready
		[P_USBD_EPEvent]=r1
		
		pop r1,r4 from [sp]
		pop bp from [sp]
		retf						
		
.public _ReplyFlexPage						
_ReplyFlexPage:
		push bp to [sp]
		bp=sp+1
		push r1,r4 to [sp]

		r1=seg _ScsiFlexPage
		ds=r1
		r1=offset _ScsiFlexPage
		r4=offset _ScsiFlexPage_End

		r4=r4-r1

		r2=[_CSW_Residue]				// data length
		
	?L_ModeSenseData:
		r3=ds:[r1++]
		[P_USBD_BIData]=r3
		r2-=1
		jnz ?L_ModeSenseData
	
		r1=0x0100						// Set Bulk IN packet ready
		[P_USBD_EPEvent]=r1
		
		pop r1,r4 from [sp]
		pop bp from [sp]
		retf													