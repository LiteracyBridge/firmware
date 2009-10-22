// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
.INCLUDE .\System\include\system\GPL162002.inc

.PUBLIC RW_G_KeyStrobe
///////////////////////////////////////////////////////


GLOBAL_System_SRAM:	.SECTION	.RAM
	.VAR	RW_G_DebounceReg;			//for keyboard scan
	.VAR	RW_G_DebounceCnt;			//for keyboard scan
	.VAR	RW_G_KeyBuf;				//for keyboard scan
	.VAR	RW_G_KeyStrobe;				//for keyboard scan

.DEFINE C_DebounceCnt			0x0008;
// Key APIs
.PUBLIC	F_Key_Scan_Initial;
.PUBLIC F_Key_Scan_ServiceLoop;
.PUBLIC F_Key_DebounceCnt_Down;
.PUBLIC _SP_GetCh;
.PUBLIC	F_SP_GetCh;

System_CODE:		.SECTION	.CODE

.PUBLIC	_IOKey_Initial;
.PUBLIC	F_System_Initial;
_IOKey_Initial: .PROC
F_System_Initial:			
    r1 = 0xfc00			// set IOA[15..0]
 	[P_IOA_Dir] = r1;	// IOA0-9 are inputs for buttons
 	[P_IOA_Attrib] = r1; 	// rest are set to output high
    r1 = 0xec00			// set IOA[15..0]
 	[P_IOA_Data] = r1;
//  set up input through IOB
// 	r1 = [P_IOB_Dir];
// 	r1 = r1 and 0xFFF8;	
//	[P_IOB_Dir] = r1;
//	r1 = [P_IOB_Attrib];
//	r1 = r1 and 0xFFF8; 
//	[P_IOB_Attrib] = r1;
	call	F_Key_Scan_Initial;	
	retf;
	.ENDP;
	

//========================================================================================
//Function Name:	System_ServiceLoop
//Syntax:		call F_System_ServiceLoop; (in assembly domain)
//  			System_ServiceLoop(); (in C domain)
//Purpose:		Main Loop of system
//Parameters:		
//Return:			
//Destroy:		r1
//Stack Depth:		
//Transfer method:	
//========================================================================================
.PUBLIC _KeyScan_ServiceLoop;
.PUBLIC	F_System_ServiceLoop;
_KeyScan_ServiceLoop:	.PROC
F_System_ServiceLoop:
	call	F_Key_DebounceCnt_Down;		// calling debounce subroutine for key scan subroutine
	call    F_Key_Scan_ServiceLoop;		// calling key scan subroutine
	// Add other general service functions here
//	r1=0xA005;				// Clear watch dog
//	[P_WatchDog_Clear]=r1;			//
	retf;
	.ENDP;
 
//========================================================================================
//Function Name:	Key_Scan_Initial
//Syntax:		call F_Key_Scan_Initial; (in assembly domain)
//Purpose:		Initialization for F_Key_Scan_ServiceLoop
//Parameters:		
//Return:			
//Destroy:		r1
//Stack Depth:		
//Transfer method:	
//========================================================================================
F_Key_Scan_Initial: .PROC
	r1 = 0x0000;				//
	[RW_G_DebounceReg] = r1;		//
	[RW_G_KeyBuf] = r1; 			//
	[RW_G_KeyStrobe] = r1;			//
	r1 = C_DebounceCnt;			//
	[RW_G_DebounceCnt] = r1;		//reset debounce counter
	retf;
	.ENDP;

//========================================================================================
//Function Name:	Key_Scan_ServiceLoop
//Syntax:		call F_Key_Scan_ServiceLoop; (in assembly domain)
//Purpose:		Get Key code from key pad(8 x 1 key pad)
//Note:			1)link F_DebounceCntDown is necessary
//Parameters:		Port IOA b7-b0
//Return:		RW_G_KeyStrobe b7-b0
//          | 	  |     |     |
// +Vcc --- x --- x ... x --- x ---
//          |     |     |     |
//          IOA.7             IOA.0
//Destroy:		r1,r2
//Stack Depth:		
//Transfer method:	
//========================================================================================
F_Key_Scan_ServiceLoop:	.PROC
	
	r1 = [P_IOA_Data];			// get key data from IOA
//	r2 = [P_IOB_Data];			// get key data from IOB
//	r2 = r2 and 0x07;
	r1 = r1 and 0x3ff;			// mask out only IOA0-IOA9 -  AA
//	r1 = r1 or r2;
	r2 = [RW_G_DebounceReg];		//
	[RW_G_DebounceReg] = r1;		//
	cmp r2,[RW_G_DebounceReg];		//
	je	L_KS_StableTwoSample;		//

	r1 = C_DebounceCnt;			//debounce time setting
	[RW_G_DebounceCnt] = r1;		//
	retf;					//

L_KS_StableTwoSample:
	r1 = [RW_G_DebounceCnt];		//
	jz	L_KS_StableOverDebounce;	//
	retf;

L_KS_StableOverDebounce:
	[RW_G_DebounceCnt] = r1;		//
	r2 = [RW_G_DebounceReg];		//
	r1 = [RW_G_KeyBuf];			//
	[RW_G_KeyBuf] = r2;			//
	r1 = r1 xor 0x03ff;			//
	r1 = r1 and [RW_G_KeyBuf];		//
	r1 = r1 and 0x03ff;			//
	r1 = r1 or [RW_G_KeyStrobe];		//
	[RW_G_KeyStrobe] = r1;			//save stable key code to RW_G_KeyStrobe
	retf;
	.ENDP;

//========================================================================================
//Function Name:	Key_DebounceCnt_Down
//Syntax:		call F_Key_DebounceCnt_Down; (in assembly domain)
//Purpose:		debounce counter down count
//Parameters:		
//Return:			
//Destroy:		r1
//Stack Depth:		
//Transfer method:	stack
//========================================================================================
F_Key_DebounceCnt_Down:	.PROC
		r1 = [RW_G_DebounceCnt];		// Debounce subroutine for F_IO_Key_Scan:
		jz	L_DebounceCntZero;		// stop count if zero
		r1 -= 0x0001;				//
		[RW_G_DebounceCnt] = r1;		//
L_DebounceCntZero:					//
		retf;  					//
	.ENDP;

//========================================================================================
//Function Name:	SP_GetCh
//Syntax:		call F_SP_GetCh; (in assembly domain)
//Purpose:		Get Keycode for I/O Port
//Parameters:		
//Return:			
//Destroy:		r1,r2
//Stack Depth:		
//Transfer method:	stack
//========================================================================================
_SP_GetCh:		.PROC
F_SP_GetCh:
		//push	BP,BP to [SP];
  		//BP = SP + 1;
		r1 = [RW_G_KeyStrobe];			// Get Key code
		r2 = 0x0000;				// Clear KeyStrobe for next key
		[RW_G_KeyStrobe] = r2;			//
		//pop		BP,BP from [SP];
		retf;
		.ENDP  		
        		