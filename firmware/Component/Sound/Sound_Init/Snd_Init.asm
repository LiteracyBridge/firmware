
.include .\Component\Include\speech\algorithm.inc

.external F_SACM_Stop 		//add by benwang 20060117
.external __RES_TONE_LIB_sa //add by benwang 20060114
.external F_SACM_MP3_System_Get_BS_Manual //add by benwang 20060117
.external F_SACM_Decode_Process //add by benwang 20060120
.external F_SACM_MS02_SongEvent	//add by chengye 2006/12/10

Reserved_SpeechRAM:	.section	.oram, .addr = 0x0b70
ReservedSpeechRAM:	.dw	0x3cc dup(?)
.ifdef MP3_Used
Reserved_MP3RAM:	.section	.oram, .addr = 0x16b0
ReservedMP3RAM:	.dw	0x11d1 dup(?)
.endif
//The following ram definition are used for mp3
//MP3InfoBuffer:作为保存MP3信息的临时Buffer,和MP3的Decode Cache_Buffer设在同一个Buffer空间
//同时这段空间也可以做成
//.ifdef MP3_Used
.public _gMP3InfoBuffer				//MP3 Infomation的全局Buffer，和MP3PlayBuffer公用
MP3InfoORAM:		.section	.oram, .addr = 0x16b0
_gMP3InfoBuffer:	.dw 1024 dup(?)
//.endif
.ifdef MP3_Used
.ifdef MP3_FATREADBUFFER
//MP3 FAT ReadBuffera, 作为MP3对FAT读取数据的Buffer之用，这样可以保证不在Decode之后再对NandFAT操作
.public _MP3FATReadBuffer
MP3FATReadRAM:		.section	.oram, .addr = 0x2881
_MP3FATReadBuffer: .dw 2048 dup(?)
.endif		//end of MP3_FATREADBUFFER defintion
.endif
//Record FATBuffer,作为录音的后续两个Buffer，这样可以避免在Encode之后就对NandFAT进行操作，这个Buffer可以和MP3的Buffer相重
.public _RecordFATBuffer
RecordFATRAM:		.section	.oram, .addr = 0x2881
_RecordFATBuffer: .dw 2048 dup(?)

.ifdef ETTS_Used				//added by chengye 2007/4/19
.define CODE_IN_RAM_SIZE	768
.public _ETTS_Code_InRam
ETTS_Code_Cache:	.section	.Oram, .addr=0x16B0
_ETTS_Code_InRam:	.dw CODE_IN_RAM_SIZE dup(?)
.endif
//========================================================================================
// RAM Define Area (Global Variable)
//========================================================================================
.public R_F_SACM_Stop_Var	   //add by benwang 20060117 
.public R_T_SACM_MS02_InstrumentSet //2004/06/16
.public R_F_SACM_MP3_System_Get_BS_Manual //add by benwang 20060117
.public R_F_SACM_Decode_Process
.public	F_USER_JPEG_GetData

	TEMP_RAM_SECTION: .SECTION .RAM, .addr=0x40  //add by benwang 20060114	
	R_RAM_START:	.EQU	$
	
	R_F_SACM_Stop_Var:    	//0x40 
	.dw 2 dup (?)			 
	
	R_T_SACM_MS02_InstrumentSet: 	//0x42
	.dw 2 dup (?) 					

	R_F_SACM_MP3_System_Get_BS_Manual:  //0x44
	.dw 2 dup (?) 							 

	R_F_SACM_Decode_Process:			//0x46
	.dw 2 dup (?) 
	
	R_F_Reserved1:						//0x48
		.dw	2 dup(?)
	R_F_Reserved2:						//0x4a
		.dw	2 dup(?)
	R_F_Reserved3:						//0x4c
		.dw	2 dup(?)

	F_USER_JPEG_GetData:				//0x4e
		.dw	2 dup (?)

	R_xxx:
		.dw	(0x20-($-R_RAM_START)) dup (?)
///////////////////////////////////////////////////////
SACM_Mask_InitCode: .section .code
.public _Snd_VarInit
_Snd_VarInit:	.proc
	push R1, R2 to [sp]
	R1 = 0x40;

	R2 = seg F_SACM_Stop;    //add by benwang 20060117
	R2 += 0xFE80;			 //add by benwang 20060117	
	[R1++] = R2;			 //add by benwang 20060117
	R2 = offset F_SACM_Stop; //add by benwang 20060117
	[R1++] = R2;

	R2 = offset F_SACM_Stop//__RES_TONE_LIB_sa
//	r2=0
	[R1++] = R2;
	R2 = seg F_SACM_Stop//__RES_TONE_LIB_sa
//	r2=0
	[R1++] = R2;

	R2 = seg F_SACM_MP3_System_Get_BS_Manual;    //add by benwang 20060117
	R2 += 0xFE80;			 					 //add by benwang 20060117	
	[R1++] = R2;			 					 //add by benwang 20060117
	R2 = offset F_SACM_MP3_System_Get_BS_Manual; //add by benwang 20060117
	[R1++] = R2;

	R2 = seg F_SACM_Decode_Process;    //add by benwang 20060120
	R2 += 0xFE80;			 					 //add by benwang 20060120	
	[R1++] = R2;			 					 //add by benwang 20060120
	R2 = offset F_SACM_Decode_Process; //add by benwang 20060120
	[R1++] = R2;	

	R2 = seg F_SACM_Stop//F_SACM_MS02_SongEvent;    //add by benwang 20060621 
    R2 += 0xFE80;                                                                 
    [R1++] = R2;                                                                 
    R2 = offset F_SACM_Stop//F_SACM_MS02_SongEvent; 
    [R1++] = R2;
    
    pop R1, R2 from [sp]
   	retf
.endp    