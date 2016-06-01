//*****************************************************************************
//*****************************************************************************
// Header File Included Area
//*****************************************************************************
//*****************************************************************************
//.include SPL162001.inc //del by benwang 20060111
.include .\System\Include\System\GPL162002.inc 
.include .\Component\Include\speech\algorithm.inc



//*****************************************************************************
//*****************************************************************************
// Contant Defintion Area
//*****************************************************************************
//*****************************************************************************
.define C_SACM_RAMP_DELAY   80





//*****************************************************************************
//*****************************************************************************
// Variable Publication Area
//*****************************************************************************
//*****************************************************************************





//*****************************************************************************
//*****************************************************************************
// Function Call Publication Area
//*****************************************************************************
//*****************************************************************************
//=============================================================================
.PUBLIC	F_SACM_CHA_AudioOutput_Enable
.PUBLIC	F_SACM_CHB_AudioOutput_Enable
.PUBLIC	F_SACM_CHA_AudioOutput_Disable
.PUBLIC	F_SACM_CHB_AudioOutput_Disable
.PUBLIC	F_SACM_CHA_ControlLogic_Enable
.PUBLIC	F_SACM_CHB_ControlLogic_Enable
.PUBLIC	F_SACM_CHA_ControlLogic_Enable_FIFO
.PUBLIC	F_SACM_CHB_ControlLogic_Enable_FIFO
.PUBLIC	F_SACM_CHA_ControlLogic_Disable
.PUBLIC	F_SACM_CHB_ControlLogic_Disable
.PUBLIC	F_SACM_CHA_PlayInterrupt_Enable
.PUBLIC	F_SACM_CHB_PlayInterrupt_Enable
.PUBLIC	F_SACM_CHA_PlayInterrupt_Disable
.PUBLIC	F_SACM_CHB_PlayInterrupt_Disable
.PUBLIC	F_SACM_CHA_WaitFIFOEmpty
.PUBLIC	F_SACM_CHB_WaitFIFOEmpty

.public	F_SACM_ADC_ControlLogic_Enable
.public	F_SACM_ADC_ControlLogic_Disable
.public	F_SACM_ADC_RecordInterrupt_Enable
.public	F_SACM_ADC_RecordInterrupt_Disable

.public _SACM_RampUpDAC1
.public _SACM_RampDnDAC1
.public _SACM_RampUpDAC2
.public _SACM_RampDnDAC2
.public F_SACM_RampUpDAC1
.public F_SACM_RampDnDAC1
.public F_SACM_RampUpDAC2
.public F_SACM_RampDnDAC2
.public	_ThreeD_Mode_On
.public	_ThreeD_Mode_Off


//+++++++++++++++++++++++++++++++++++++++++++
.public	F_SACM_Init
.public	F_SACM_SendDAC1
.public	F_SACM_SendDAC2


.public	F_SetIRQ_16000
.public	F_SetIRQ_8000
.public	F_SetIRQ_9200
.public	F_SetIRQ_11025
.public	F_SetIRQ_32000
.public	F_SetIRQ_22050
.public	F_SetIRQ_44100
//++++++++++++++++++++++++++++++++++++


//=============================================================================







//*****************************************************************************
//*****************************************************************************
// RAM Definition Area
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************




//*****************************************************************************
//*****************************************************************************
// Definition Area
//*****************************************************************************
//*****************************************************************************
.define C_Timer_Setting_8K_For_48MHz   0x10000-(48000000/2/8000)	
.define C_Timer_Setting_9K_For_48MHz   0x10000-(48000000/2/9200)
.define C_Timer_Setting_11K_For_48MHz  0x10000-(48000000/2/11025)
//.define C_Timer_Setting_16K_For_48MHz  0x10000-(48000000/2/16000)

.define C_Timer_Setting_16K_For_48MHz   0x10000-(48000000/2/16000)	
.define C_Timer_Setting_18K_For_48MHz   0x10000-(48000000/2/18400)
.define C_Timer_Setting_22K_For_48MHz  0x10000-(48000000/2/22050)
.define C_Timer_Setting_32K_For_48MHz  0x10000-(48000000/2/32000)



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// added by Benwang 20060111
.define C_SampleRate_44_1K   	0x00
.define C_SampleRate_48K 	 	0x01
.define C_SampleRate_32K  	 	0x02
.define C_SampleRate_22_05K  	0x03
.define C_SampleRate_24K  	 	0x04
.define C_SampleRate_16K 	 	0x05
.define C_SampleRate_11_025K 	0x06
.define C_SampleRate_12K 		0x07
.define C_SampleRate_8K 		0x08
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

.define	C_Fosc_48M 		0xC007			// added by Anson 2004/2/2  
.define C_SystemClock 	C_Fosc_48M	
.IF 1 //del by zgq on 20051215
	.define C_A6400S_sf_8K   		(0x10000-C_SystemClock*1000/2/8000) 	//0xf400
	.define C_A6400S_sf_12K  		(0x10000-C_SystemClock*1000/2/12000)	//0xf800
	.define C_A6400S_sf_11K025  	(0x10000-C_SystemClock*1000/2/11025)	//0xf74b
	.define C_A6400S_sf_16K  		(0x10000-C_SystemClock*1000/2/16000)	//0xfa00
	.define C_A6400S_sf_24K  		(0x10000-C_SystemClock*1000/2/24000)	//0xfc00
	.define C_A6400S_sf_22K05  		(0x10000-C_SystemClock*1000/2/22050)	//0xfba6
	.define C_A6400S_sf_32K  		(0x10000-C_SystemClock*1000/2/32000)	//0xfd00
	.define C_A6400S_sf_48K 		(0x10000-C_SystemClock*1000/2/48000)	//0xfe00
	.define C_A6400S_sf_44K1 		(0x10000-C_SystemClock*1000/2/44100)	//0xfdd3
	.define C_A6400S_sf_18K	  		(0x10000-C_SystemClock*1000/2/18000)	//
	.define C_A6400S_sf_20K	  		(0x10000-C_SystemClock*1000/2/20000)	//
.ELSE //add by zgq on 20051215
	.define C_A6400S_sf_8K   		(0x10000-48000000/2/16000) 	//0xf400
	.define C_A6400S_sf_12K  		(0x10000-48000000/2/24000)	//0xf800
	.define C_A6400S_sf_11K025  	(0x10000-48000000/2/22025)	//0xf74b
	.define C_A6400S_sf_16K  		(0x10000-48000000/2/32000)	//0xfa00
	.define C_A6400S_sf_24K  		(0x10000-48000000/2/48000)	//0xfc00
	.define C_A6400S_sf_22K05  		(0x10000-48000000/2/44050)	//0xfba6
	.define C_A6400S_sf_32K  		(0x10000-48000000/2/64000)	//0xfd00
	.define C_A6400S_sf_48K 		(0x10000-48000000/2/96000)	//0xfe00
	.define C_A6400S_sf_44K1 		(0x10000-48000000/2/88200)	//0xfdd3
	.define C_A6400S_sf_18K	  		(0x10000-48000000/2/36000)	//
	.define C_A6400S_sf_20K	  		(0x10000-48000000/2/40000)	//
.ENDIF

//add by benwang 20060111
.IF 1
	.define C_MP3_sf_8K   		8
	.define C_MP3_sf_12K  		7
	.define C_MP3_sf_11K025  	6
	.define C_MP3_sf_16K  		5
	.define C_MP3_sf_24K  		4
	.define C_MP3_sf_22K05  	3	
	.define C_MP3_sf_32K  		2
	.define C_MP3_sf_48K 		1
	.define C_MP3_sf_44K1 		0
	//.define C_MP3_sf_18K  //del by benwang 20060111	  		
	//.define C_MP3_sf_20K	//del by benwang 20060111  		
.ELSE //add by zgq on 20051215
	.define C_MP3_sf_8K   		(0x10000-48000000/2/16000) 	//0xf400
	.define C_MP3_sf_12K  		(0x10000-48000000/2/24000)	//0xf800
	.define C_MP3_sf_11K025  	(0x10000-48000000/2/22025)	//0xf74b
	.define C_MP3_sf_16K  		(0x10000-48000000/2/32000)	//0xfa00
	.define C_MP3_sf_24K  		(0x10000-48000000/2/48000)	//0xfc00
	.define C_MP3_sf_22K05  		(0x10000-48000000/2/44050)	//0xfba6
	.define C_MP3_sf_32K  		(0x10000-48000000/2/64000)	//0xfd00
	.define C_MP3_sf_48K 		(0x10000-48000000/2/96000)	//0xfe00
	.define C_MP3_sf_44K1 		(0x10000-48000000/2/88200)	//0xfdd3
	.define C_MP3_sf_18K	  		(0x10000-48000000/2/36000)	//
	.define C_MP3_sf_20K	  		(0x10000-48000000/2/40000)	//
.ENDIF
//add by benwang 20060111

//add by benwang start 20060221
.define C_A128_sf_8K   		8
.define C_A128_sf_12K  		7
.define C_A128_sf_11K025  	6
.define C_A128_sf_16K  		5
.define C_A128_sf_24K  		4
.define C_A128_sf_22K05  	3	
.define C_A128_sf_32K  		2
.define C_A128_sf_48K 		1
.define C_A128_sf_44K1 		0
//add by benwang end 20060221

//add by chengye start 2006/12/6 
.define C_SACM_ENABLE_DAC1	    0x0020
.define C_SACM_ENABLE_DAC2	    0x0040
.external R_SACM_Play_Flag
//add by chengye end 2006/12/6 

//*****************************************************************************
//*****************************************************************************
// TEXT Table Definition Area
//*****************************************************************************
//*****************************************************************************
.CODE
T_DAC_Timer:  //{ 44100, 48000, 32000, 22050, 24000, 16000 , 11025 , 12000 , 8000 };
.dw  C_A6400S_sf_44K1, 	C_A6400S_sf_48K, 	C_A6400S_sf_32K
.dw  C_A6400S_sf_22K05, 	C_A6400S_sf_24K,	C_A6400S_sf_16K
.dw  C_A6400S_sf_11K025,	C_A6400S_sf_12K,	C_A6400S_sf_8K    

//add by benwang 20060111
T_MP3_DAC_Timer:  //{ 44100, 48000, 32000, 22050, 24000, 16000 , 11025 , 12000 , 8000 };
.dw  C_MP3_sf_44K1, 	C_MP3_sf_48K, 	C_MP3_sf_32K
.dw  C_MP3_sf_22K05, 	C_MP3_sf_24K,	C_MP3_sf_16K
.dw  C_MP3_sf_11K025,	C_MP3_sf_12K,	C_MP3_sf_8K    


//add by benwang 20060221
T_A128_DAC_Timer:  //{ 44100, 48000, 32000, 22050, 24000, 16000 , 11025 , 12000 , 8000 };
.dw  C_A128_sf_44K1, 	C_A128_sf_48K, 	C_A128_sf_32K
.dw  C_A128_sf_22K05, 	C_A128_sf_24K,	C_A128_sf_16K
.dw  C_A128_sf_11K025,	C_A128_sf_12K,	C_A128_sf_8K    

T_A128_SampleRate:
.dw 44100, 48000, 32000, 22050, 24000, 16000, 11025, 12000, 8000
//*****************************************************************************
//*****************************************************************************
// CODE Definition Area
//*****************************************************************************
//*****************************************************************************

SACM_ROM_Block:	.section	.CODE
//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// Function: F_SACM_XXXX_SendDAC1
// Description: send data to DAC1 from library
//
// Syntax  : F_SACM_XXXX_SendDAC1
// Destory : R4
// Parameter : R4: 16-bit unsign PCM
// Return    :None
////////////////////////////////////////////////////////
F_SACM_SendDAC1:
	//--------------------------------------------------------------
	// Rounding to 10-bit DAC if necessary
	// Change Signed Data to Unsign Data
	//--------------------------------------------------------------
//	R3 = R4;
//	R3 = R3 + 0x0008;//rounding LSB 6 bits
//	R3 = R3 & 0xFFF0;//mask LSB 6 bits for 10Bit DAC
//	R3 = R3 ^ 0x8000;//unsigned DAC
	//--------------------------------------------------------------
	[P_CHA_Data] = R4;
	retf;

F_SACM_MS02_SendDAC1: .PROC
    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHA_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHA_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHA_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHA_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHA_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHA_Data] = R4;

    retf;
.ENDP



////////////////////////////////////////////////////////
// Function: F_SACM_XXXX_SendDAC2
// Description: send data to DAC from library
//
// Syntax  : F_SACM_XXXX_SendDAC2
// Destory : R4
// Parameter : R4: 16-bit unsign PCM
// Return    :None
////////////////////////////////////////////////////////
F_SACM_SendDAC2:
	//--------------------------------------------------------------
	// Rounding to 10-bit DAC if necessary
	// Change Signed Data to Unsign Data
	//--------------------------------------------------------------
//	R3 = R4;
//	R3 = R3 + 0x0008;//rounding LSB 6 bits
//	R3 = R3 & 0xFFF0;//mask LSB 6 bits for 10Bit DAC
//	R3 = R3 ^ 0x8000;//unsigned DAC
	//--------------------------------------------------------------
	[P_CHA_Data] = R4; //add by benwang 20060111
	retf;
////////////////////////////////////////////////////////
// Function: F_SACM_MS02_SendDAC2
// Description: send data to DAC from library
//
// Syntax  : F_SACM_MS02_SendDAC2
// Destory: R4   (Suggest to be R1 by Arthur)
// Parameter : R4: 16-bit unsign PCM
// Return    :None
////////////////////////////////////////////////////////
F_SACM_MS02_SendDAC2: .PROC
    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHB_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHB_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHB_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHB_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHB_Data] = R4;

    R4 = [R2++];
	R4 = R4 ^ 0x8000;//unsigned DAC
    [P_CHB_Data] = R4;


    retf;
.ENDP


////////////////////////////////////////////////////////
//-- Function: F_SACM_GetADC
//-- Parameter: N/A
//-- Return: R1 = ADC data
//-- Description: This function called by library to get
//                ADC data for recording. It can be from either Microphone
//                or line-in.
//                User would have to modified the function body
//                based on the need to fulfill this request from
//                library.
//                User should store the value in register(R2-R5)if use them.
////////////////////////////////////////////////////////
.public	F_SACM_GetADC
F_SACM_GetADC:
	r3=[P_ASADC_Data]
	retf







////////////////////////////////////////////////////////
// Function: F_SACM_Init
// Description: Hardware initilazation DAC
// Destory   :
// Parameter : None
// Return    : None
// Please note that users can not destroy R5 while
//        Define this function call
////////////////////////////////////////////////////////
F_SACM_Init: //add by benwang 20060111
	push	r1,r5 to [sp]
	FIR_MOV OFF;

		r1=0x8600
		r1|=C_SampleRate_22_05K + 0x2000
		[P_CHA_Ctrl]=r1
	
		r1 = 0x1000 + 0x2800
		[P_CHB_Ctrl] = r1		//Set Sample Rate CHB be the same with CHA, monochrome


      	r1 = 0x0100
      	[P_CHA_FIFO]=r1			// Setup CHA FIFO Empty Interrupt Level

      	r1 = 0x0100
      	[P_CHB_FIFO]=r1			// Setup CHB FIFO Empty Interrupt Level

		r1=0x03
		[P_HPAMP_Ctrl]=r1
///
		r1=0x0001
		[P_DAC_IIS_Ctrl]=r1
		nop
		nop
		r1=0x0d
		[P_DAC_Ctrl]=r1	

		r1=0xa000
wait0?:
		r1-=1
		jnz	wait0?
//		r1=0x01
//		[P_DAC_Ctrl]=r1
		
		r1=0x0100
		[P_DAC_Effect_Ctrl]=r1
		
		r1=0x1000
wait1?:
		r1-=1
		jnz	wait1?
//////////////////////////////////		
L_SD_Parameter:
		r1=0x0100
		[P_DAC_Effect_Ctrl]=r1

wait2?:		
		r1=[P_DAC_ACCREQ]
		jpl	wait2?		
		r1=256					//write 3D parameter
		r3=offset T_HRTF_COEF
		r4=seg16 T_HRTF_COEF
		ds=r4
		r5=0x4200
ThreeDLow_loop?:		
		r4=ds:[r3++]
		[P_DAC_ACCDINL]=r4
		[P_DAC_ACCREQ]=r5
wait3?:	
		r4=[P_DAC_ACCREQ]
		jpl	wait3?
		r5+=1
		r1-=1
		jnz	ThreeDLow_loop?
	
		r1=256					//write 3D parameter
		r3=offset T_CCS_COEF
		r4=seg16 T_CCS_COEF
		ds=r4
		r5=0x4300
ThreeDHigh_loop?:		
		r4=ds:[r3++]
		[P_DAC_ACCDINL]=r4
		[P_DAC_ACCREQ]=r5
wait4?:	
		r4=[P_DAC_ACCREQ]
		jpl	wait4?
		r5+=1
		r1-=1
		jnz	ThreeDHigh_loop?

///////////////////////////////////////////
//******************
//	3D parameter verify
//********************
//.comment	@
L_SD_Parameter_Verify:
		r1=256					//write 3D parameter
		r2=offset T_HRTF_COEF
		r3=seg16 T_HRTF_COEF
		r5=0x0200
		ds=r3
loop?:
		[P_DAC_ACCREQ]=r5
wait5?:		
		r4=[P_DAC_ACCREQ]
		jpl	wait5?		
ThreeDLow_loop?:		
		r4=ds:[r2++]
		cmp	r4,[P_DAC_ACCDOUTL]
		je	skip?	
		goto	verifyerror
skip?:		
		r5+=1
		r1-=1
		jnz	loop?	

		r1=256					//write 3D parameter
		r2=offset T_CCS_COEF
		r3=seg16 T_CCS_COEF
		r5=0x0300
		ds=r3
loop1?:
		[P_DAC_ACCREQ]=r5
wait1?:		
		r4=[P_DAC_ACCREQ]
		jpl	wait1?		
ThreeDHigh_loop?:		
		r4=ds:[r2++]
		cmp	r4,[P_DAC_ACCDOUTL]
		je	skip1?	
		goto	verifyerror
skip1?:		
		r5+=1
		r1-=1
		jnz	loop1?	
/////////////////////////
//	.comment	@
L_EQ_Parameter:
wait6?:		
		r1=[P_DAC_ACCREQ]
		jpl	wait6?		
		r1=32					//write EQ/AC parameter
		r5=0x4000
EQ_loop?:
		r4=0		
		[P_DAC_ACCDINL]=r4
		[P_DAC_ACCDINH]=r4
		[P_DAC_ACCREQ]=r5
wait7?:	
		r4=[P_DAC_ACCREQ]
		jpl	wait7?
		r5+=1
		r1-=1
		jnz	EQ_loop?


		r1=13					//write EQ/AC parameter
		r3=offset T_EQ_COEF
		r4=seg16 T_EQ_COEF
		ds=r4
		r5=0x4020
EQ_loop1?:		
		r4=ds:[r3++]
		[P_DAC_ACCDINL]=r4
		r4=ds:[r3++]
		[P_DAC_ACCDINH]=r4
		[P_DAC_ACCREQ]=r5
wait8?:	
		r4=[P_DAC_ACCREQ]
		jpl	wait8?
		r5+=1
		r1-=1
		jnz	EQ_loop1?	
		
//.comment	@	
		r8=7					//write EQ/AC parameter
		r2=0
EQ_loop2?:	
		r3=offset T_EQ_Mode		//live EQ mode
		r4=seg16 T_EQ_Mode
		r3+=r2
		r4+=0,carry
		ds=r4
		r1=ds:[r3++]
		r3=offset T_EQ_GAIN
		r4=seg16 T_EQ_GAIN	
		r1=r1 lsl 1
		r3+=r1
		r4+=0,carry
		ds=r4
		r4=ds:[r3++]
		[P_DAC_ACCDINL]=r4
		r4=ds:[r3++]
		[P_DAC_ACCDINH]=r4
		r5=r2+0x4040
		[P_DAC_ACCREQ]=r5
wait9?:	
		r4=[P_DAC_ACCREQ]
		jpl	wait9?
		r2+=1
		r8-=1
		jnz	EQ_loop2?	
		r1=0x1100
		[P_DAC_Effect_Ctrl]=r1
//		@
/////////////////////////////
//******************
//	EQ parameter verify
//********************		
.comment	@
L_EQ_Parameter_Verify:
		r1=0x20					//write 3D parameter
		r5=0x0000
loop?:
		[P_DAC_ACCREQ]=r5
wait?:		
		r4=[P_DAC_ACCREQ]
		jpl	wait?		
EQ_loop?:		
		r4=0
		cmp	r4,[P_DAC_ACCDOUTL]
		je	skip?	
		goto	verifyerror
skip?:
		r4=0
		cmp	r4,[P_DAC_ACCDOUTH]
		je	skip1?
		goto	verifyerror
skip1?:		
		r5+=1
		r1-=1
		jnz	loop?
		
		
		r1=13					//write EQ parameter
		r2=offset T_EQ_COEF
		r3=seg16 T_EQ_COEF
		r5=0x020
		ds=r3
loop1?:
		[P_DAC_ACCREQ]=r5
wait1?:		
		r4=[P_DAC_ACCREQ]
		jpl	wait1?		
EQ_loop1?:		
		r4=ds:[r2++]
		cmp	r4,[P_DAC_ACCDOUTL]
		je	skip2?	
		goto	verifyerror	
skip2?:	
		r4=ds:[r2++]
		cmp	r4,[P_DAC_ACCDOUTH]
		je	skip3?
		goto	verifyerror
skip3?:			
		r5+=1
		r1-=1
		jnz	loop1?	
		
		
		r8=7					//write EQ parameter
		r9=0x0040
		r1=offset T_EQ_Mode
		r2=seg16 T_EQ_Mode
loop2?:
		[P_DAC_ACCREQ]=r9
wait2?:		
		r3=[P_DAC_ACCREQ]
		jpl	wait2?

		ds=r2
		r3=ds:[r1++]
		r2=ds		
		r3=r3 lsl 1
		r4=offset T_EQ_GAIN
		r5=seg16 T_EQ_GAIN
		r4+=r3
		r5+=0, carry
		ds=r5
		r3=ds:[r4++]
		cmp	r3,[P_DAC_ACCDOUTL]
		je	skip4?
		goto	verifyerror
skip4?:		
		r3=ds:[r4]
		cmp	r3,[P_DAC_ACCDOUTH]
		je	skip5?
		goto	verifyerror
skip5?:
		r9+=1
		r8-=1
		jnz	loop2?		
@		
//------------------------------		
		r1=0x7fff
		[P_DAC_Volume3D]=r1
		r1=0x2000
		[P_DAC_Volume3D_Cent]=r1
		r1=0x7fff
		[P_DAC_Volume3D_Surround]=r1
		[P_DAC_Volume3D_R]=r1
		[P_DAC_Volume3D_L]=r1
		
		r1=0x0100//0x9100						//modify by chengye for disable 3D and EQ module
		[P_DAC_Effect_Ctrl]=r1
//--------------------------------	
//		@

		r1=[P_CHA_Ctrl]
		r1|=0x2000
		[P_CHA_Ctrl]=r1

	FIQ on;
	IRQ on;
	pop	r1,r5 from [sp]
	retf

verifyerror:
	nop
	jmp	$


////////////////////////////////////////////////////////
// Function: F_3D_Mode_On
// Description: Set 3D Mode On
//              
// Syntax  : 3D_Mode_On
// Destory: 
// Parameter : 
// Return    : None
////////////////////////////////////////////////////////
_ThreeD_Mode_On:  .proc
F_ThreeD_Mode_On:
	push	bp to [sp]
	r1 = [P_DAC_Effect_Ctrl]
	r1 |= 0xC000;
	[P_DAC_Effect_Ctrl] = r1;
	pop	bp from [sp]
	retf
.endp

////////////////////////////////////////////////////////
// Function: F_3D_Mode_Off
// Description: Set 3D Mode Off
//              
// Syntax  : 3D_Mode_Off
// Destory: 
// Parameter : 
// Return    : None
////////////////////////////////////////////////////////
_ThreeD_Mode_Off:  .proc
F_ThreeD_Mode_Off:
	push	bp to [sp]
	r1 = [P_DAC_Effect_Ctrl]
	r1 &= (~0xC000);
	[P_DAC_Effect_Ctrl] = r1;
	pop	bp from [sp]
	retf
.endp

////////////////////////////////////////////////////////
// Function: F_SACM_RampDnDAC1
// Description: Ramp down after using DAC to avoid "bow" sound
//              from speaker
// Syntax  : F_SACM_RampDnDAC1
// Destory: R1
////////////////////////////////////////////////////////
_SACM_RampDnDAC1: .PROC
F_SACM_RampDnDAC1:
	push R1 to [SP];
  	r1=0x100
?_Loop_0:
    call F_SACM_Delay;
    R1 -= 1
    jnz ?_Loop_0;
	
	pop R1 from [SP];
    retf;
.ENDP

////////////////////////////////////////////////////////
// Function: F_SACM_RampDnDAC2
// Description: Ramp down after using DAC to avoid "bow" sound
//              from speaker
// Syntax  : F_SACM_RampDnDAC2
// Destory: R1
////////////////////////////////////////////////////////
_SACM_RampDnDAC2: .PROC
F_SACM_RampDnDAC2:
	push R1 to [SP];
   	r1=0x100
?_Loop_0:
    call F_SACM_Delay;
    R1 -= 1
    jnz ?_Loop_0;
    pop R1 from [SP];
    retf
.ENDP

////////////////////////////////////////////////////////
// Function: F_SACM_RampUpDAC1
// Description: Ramp Up before using DAC to avoid "bow" sound
//              from speaker
// Syntax  : F_SACM_RampUpDAC1()
// Destory: R1
////////////////////////////////////////////////////////
_SACM_RampUpDAC1: .PROC		//joey
F_SACM_RampUpDAC1:			//joey
	push R1,r2 to [SP];
	r1=[P_CHA_Data]
    r1^=0x8000
    r1 &= ~0x0003
    cmp     r1,0x8000
    jb     	L_RU_NormalUp
    je      L_RU_End

L_RU_DownLoop:
    r2=[P_CHA_Ctrl]
    jpl	L_RU_DownLoop 
    r2 = 0xA005//0x0001
    [P_WatchDog_Clear] = r2
    r1 -= 0x04
    [P_CHA_Data] = r1
    [P_CHA_Data]=r1
  	call F_SACM_Delay
    cmp     r1,0x8000
    jne     L_RU_DownLoop
L_RD_DownEnd:
    jmp     L_RU_End

L_RU_NormalUp:
L_RU_Loop:
    r2=[P_CHA_Ctrl]
    jpl	L_RU_Loop 
    r2 = 0xA005//0x0001
   [P_WatchDog_Clear] = r2
     
    r1 += 0x04
    [P_CHA_Data] = r1
    [P_CHA_Data] = r1
   	call F_SACM_Delay
    cmp     r1, 0x8000
    jne     L_RU_Loop
L_RU_End:	
	
//	call F_SACM_CHA_WaitFIFOEmpty
	
	pop R1,r2 from [SP];
  	retf;
.ENDP

////////////////////////////////////////////////////////
// Function: F_SACM_RampUpDAC2
// Description: Ramp Up before using DAC to avoid "bow" sound
//              from speaker
// Syntax  : F_SACM_RampUpDAC2()
// Destory: R1
////////////////////////////////////////////////////////
_SACM_RampUpDAC2: .PROC
F_SACM_RampUpDAC2:
	push R1 to [SP];

	r1=100
loop?:	
	call F_SACM_Delay;
    r1-=1
    jnz	loop?	
    pop R1 from [SP];
    retf;
.ENDP

//-------------------------------------------------------
// Function: F_SACM_Delay
// Description: Provide delay for Ramp up/down
//              The delay time is adjustable by adjusting C_SACM_RAMP_DELAY
// Destory: R1
//-------------------------------------------------------
F_SACM_Delay: .proc
    push R1 to [SP];
    R1 = C_SACM_RAMP_DELAY; // Ramp Up/Dn delay per step

?_Loop_0:
    R1 -= 0x0001;
    jnz ?_Loop_0;

    pop R1 from [SP];
    retf;
.endp












//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_ADC_ControlLogic_Enable:
		push r1 to [sp]
///////////////	
//      cbs: COMMENTING LINES THAT OVERWROTE MIC GAIN
//		r1=0x006
//		[P_HQADC_MIC_PGA]=r1	
//////////////	
		r1=[P_HQADC_Ctrl]
		r1&=~0x03a			//enable left channel, mic ADC input select, mic bias enable 
		r1|=0x08c5			//Hp enable mono, boost amplifier, left channel, mic bias on, HQADC clock on
		[P_HQADC_Ctrl]=r1
.comment #
		r1 = 0x2000					// Wait ADC Stable
L_DelayLoop?:
		r1 = r1 - 1
		jnz	L_DelayLoop?
//#
		r2=0x5
L_DelayLoop1?:
		r1 = 0xf000                                     // Wait ADC Stable 
L_DelayLoop?:
		r1 = r1 - 1
		jnz  L_DelayLoop? 
		r2-=1
		jnz  L_DelayLoop1?

		r1=0xc100				//enable auto-sample int, FIFO 8 level
		[P_ASADC_Ctrl]=r1
		
		r1=[P_ADC_Setup]
		r1|=0x0080				//Enable auto sample mode
		[P_ADC_Setup]=r1		
		
		r1=[P_ASADC_Data]		// Clear ADC FIFO Data
		r1=[P_ASADC_Data]
		r1=[P_ASADC_Data]
		r1=[P_ASADC_Data]
		r1=[P_ASADC_Data]
		r1=[P_ASADC_Data]
		r1=[P_ASADC_Data]
		r1=[P_ASADC_Data]

		r1 = [P_ADC_Setup]
		r1 = r1 | (0x0004)		// Start Auto Sample
		[P_ADC_Setup] = r1

		pop r1 from [sp]
		retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_ADC_ControlLogic_Disable:
		push r1 to [sp]
		r1=0x8000				// Disable ADC
		[P_ASADC_Ctrl]=r1
	
		r1 = [P_ADC_Setup]
		r1 = r1 & (~0x0004)		// Stop Auto Sample
		[P_ADC_Setup] = r1
	
		r1=0x3e
		[P_HQADC_Ctrl]=r1
		
		pop r1 from [sp]
		retf
//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_ADC_RecordInterrupt_Enable:
		int off
		r4 = [P_ASADC_Ctrl]
		r4 = r4 | 0x4000		// Enable Interrupt
		[P_ASADC_Ctrl] = r4
		int fiq,irq
		retf
//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_ADC_RecordInterrupt_Disable:
		int off
		r4 = [P_ASADC_Ctrl]
		r4 = r4 & (~0x4000)		// Disable Interrupt
		[P_ASADC_Ctrl] = r4
		int fiq,irq
		retf

.public	F_SACM_ADC_MoniterON
F_SACM_ADC_MoniterON:
		push	r1 to [sp]
		r1=[P_HPAMP_Ctrl]		//mic output direct
		r1|=0x08
		r1&=~0x02				//left channel on
		[P_HPAMP_Ctrl]=r1
		r1=[P_CHA_Ctrl]
		r1|=0x2000
		[P_CHA_Ctrl]=r1
		pop	r1 from [sp]
		retf


////////////////////////////////////////////////////////
//Please Note that only R5 could be used
//It seems that SACM_XXX_Play will invoke following function
//SACM_XXX_Play must push/pop R5(BP)
//  Otherwise, there might be some software bug
////////////////////////////////////////////////////////
//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHA_AudioOutput_Enable:

	retf
//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHB_AudioOutput_Enable:

	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHA_ControlLogic_Enable:
	push r5 to [sp]
	r5 = [P_CHA_FIFO]
	r5&=~0x0f0				//clear fifo level set
	r5 = r5 | 0x0120		// Reset FIFO
	[P_CHA_FIFO] = r5

	r5=[P_DAC_Ctrl]
	r5&=~0x0c
	r5|=0x01
	[P_DAC_Ctrl]=r5	

	r5=0x01
wait?:
	r5-=1
	jnz	wait?

	r5=[P_HPAMP_Ctrl]
	r5&=~0x0c
	[P_HPAMP_Ctrl]=r5

	r5=0x3000
wait1?:
	r5-=1
	jnz	wait1?
	
	r5=[P_HPAMP_Ctrl]
	r5&=~0x02
	[P_HPAMP_Ctrl]=r5
	pop	r5 from [sp]
	retf


F_SACM_CHA_ControlLogic_Enable_FIFO:
	push r5	to [sp]
	r5 = [P_CHA_FIFO]
	r5&=~0x0f0
	r5 = r5 | 0x0120	//0170	// Reset FIFO
	[P_CHA_FIFO] = r5

	r5=[P_DAC_Ctrl]
	r5&=~0x0c
	r5|=0x01
	[P_DAC_Ctrl]=r5	

	r5=0x1
wait?:
	r5-=1
	jnz	wait?

	r5=[P_HPAMP_Ctrl]
	r5&=~0x0c
	[P_HPAMP_Ctrl]=r5

	r5=0x3000
loop?:
	r5-=1
	jnz	loop?

	r5=[P_HPAMP_Ctrl]
	r5&=~0x02
	[P_HPAMP_Ctrl]=r5

	pop	r5 from [sp]
	retf




//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHB_ControlLogic_Enable:
	push r5 to [sp]
	r5 = [P_CHB_Ctrl]
	r5&=~0x0400
	r5 = r5 | 0x9800		//add by zgq on 20051215
	[P_CHB_Ctrl] = r5
	r5 = [P_CHB_FIFO]
	r5&=~0x0f0
	r5 = r5 | 0x0120		// Reset FIFO
	[P_CHB_FIFO] = r5

	r5=[P_DAC_Ctrl]
	r5&=~0x04
	[P_DAC_Ctrl]=r5
	r5=[P_HPAMP_Ctrl]
	r5&=~0x01
	[P_HPAMP_Ctrl]=r5
	pop	r5 from [sp]
	retf


F_SACM_CHB_ControlLogic_Enable_FIFO:
	push	r5 to [sp]
	r5 = [P_CHB_Ctrl]
	r5&=~0x0400
	r5 = r5 | 0x9800		//add by zgq on 20051215
	[P_CHB_Ctrl] = r5
	r5 = [P_CHB_FIFO]
	r5&=~0x0f0
	r5 = r5 | 0x0120	//0170	// Reset FIFO
	[P_CHB_FIFO] = r5

	r5=[P_DAC_Ctrl]
	r5&=~0x04
	[P_DAC_Ctrl]=r5

	r5=[P_HPAMP_Ctrl]
	r5&=~0x01
	[P_HPAMP_Ctrl]=r5
	pop	r5 from [sp]
	retf


////////////////////////////////////////////////////////
//Please Note that only R4 could be used
////////////////////////////////////////////////////////
//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHA_AudioOutput_Disable:
	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHB_AudioOutput_Disable:
	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHA_ControlLogic_Disable:
	push	r4 to [sp]
	r4=[P_HPAMP_Ctrl]
	r4|=0x01F			//disable left channel
	r4&=~0x010
	[P_HPAMP_Ctrl]=r4


	R4=0x1000
loop?:	
	r4-=1
	jnz	loop?

	r4=[P_DAC_Ctrl]
	r4|=0x0c
	//	r1&=~0x01
	r4&=~0x01			//modify by chengye for disable DAC clock
	[P_DAC_Ctrl]=r4


	R4=0x1000
loop2?:	
	r4-=1
	jnz	loop2?


	r4=0x8000
	[P_CHA_Data]=r4
	[P_CHA_Data]=r4
	
	r4 = [P_CHA_Ctrl]
	r4 = r4 & (~0x2000)
	r4|=0x0200				//reset SRC module	
	[P_CHA_Ctrl] = r4
	pop	r4 from [sp]
	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHB_ControlLogic_Disable:
push r4 to [sp]
	r4 = [P_CHB_Ctrl]
	r4 = r4 & (~0x2000)
	[P_CHB_Ctrl] = r4
	pop	r4 from [sp]
	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHA_PlayInterrupt_Enable:
    INT off
	r4 = [P_CHA_Ctrl]
	r4 = r4 | (0x6000)
//	r4 = r4 | (0x2000)				//modify by chengye 2006/12/5
	[P_CHA_Ctrl] = r4
	int fiq, irq
	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHB_PlayInterrupt_Enable:
    INT off
	r4 = [P_CHB_Ctrl]
//	r4 = r4 | (0x6000)
	r4 = r4 | (0x2000)				//仅仅使能CHB, 但不产生CHB的中断
	[P_CHB_Ctrl] = r4
	int fiq, irq
	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHA_PlayInterrupt_Disable:
    INT off
	r4 = [P_CHA_Ctrl]
	r4 = r4 & (~0x4000)
	[P_CHA_Ctrl] = r4
	int fiq, irq
	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHB_PlayInterrupt_Disable:
    INT off
	r4 = [P_CHB_Ctrl]
	r4 = r4 & (~0x4000)
	[P_CHB_Ctrl] = r4
	int fiq, irq
	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHA_WaitFIFOEmpty:
	r1 = 0xA005
	[P_WatchDog_Clear] = r1
	r1 = [P_CHA_FIFO]
	r1 = r1 & 0x000F
	cmp r1,1//1   //modified by benwang 20060116
	ja	F_SACM_CHA_WaitFIFOEmpty
	call	F_SACM_RampUpDAC1
	retf

//---------------------------------------------------------------
//---------------------------------------------------------------
F_SACM_CHB_WaitFIFOEmpty:
	r1 = 0xA005
	[P_WatchDog_Clear] = r1
	r1 = [P_CHB_FIFO]
	r1 = r1 & 0x000F
	cmp r1,1//1   //modified by benwang 20060116
	ja	F_SACM_CHB_WaitFIFOEmpty
	retf





/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// Set Audio Output Mode
// Option : PWM / DAC
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
_Set_Audio_Output_Mode: .proc
   	push BP to [SP];
	BP = SP + 1;
   	R1 = [BP + 3];

   	pop BP from [SP];
    retf
	.endp



///////////////////////////////////////
//////////////////////////////////////
F_SetIRQ_16000:
	//r1=C_Timer_Setting_32K_For_48MHz //del by benwang 20060112
	//[P_TimerE_Preload]=r1 //del by benwang 20060112
	push r1 to [sp];   	//add by benwang 20060112
	r1=[P_CHA_Ctrl]  	//add by benwang 20060112
	r1&=~0x0f        	//add by benwang 20060112 
	r1|=C_SampleRate_16K//add by benwang 20060112
	[P_CHA_Ctrl]=r1		//add by benwang 20060112
	pop r1 from [sp]  	//add by benwang 20060112
	retf

F_SetIRQ_8000:
	//r1=C_Timer_Setting_16K_For_48MHz //del by benwang 20060112
	//[P_TimerE_Preload]=r1 //del by benwang 20060112
	push r1 to [sp];   	//add by benwang 20060112
	r1=[P_CHA_Ctrl]  	//add by benwang 20060112
	r1&=~0x0f        	//add by benwang 20060112 
	r1|=C_SampleRate_8K //add by benwang 20060112
	[P_CHA_Ctrl]=r1		//add by benwang 20060112
	pop r1 from [sp]  	//add by benwang 20060112
	retf

F_SetIRQ_9200:
	r1=C_Timer_Setting_18K_For_48MHz
	[P_TimerE_Preload]=r1
	retf


F_SetIRQ_11025:
	//r1=C_Timer_Setting_22K_For_48MHz //del by benwang 20060112
	//[P_TimerE_Preload]=r1 //del by benwang 20060112
	push r1 to [sp];   	//add by benwang 20060112
	r1=[P_CHA_Ctrl]  	//add by benwang 20060112
	r1&=~0x0f        	//add by benwang 20060112 
	r1|=C_SampleRate_11_025K//add by benwang 20060112
	[P_CHA_Ctrl]=r1		//add by benwang 20060112
	pop r1 from [sp]  	//add by benwang 20060112
	retf

F_SetIRQ_32000:
	//r1=C_Timer_Setting_22K_For_48MHz //del by benwang 20060112
	//[P_TimerE_Preload]=r1 //del by benwang 20060112
	push r1 to [sp];   	//add by benwang 20060112
	r1=[P_CHA_Ctrl]  	//add by benwang 20060112
	r1&=~0x0f        	//add by benwang 20060112 
	r1|=C_SampleRate_32K//add by benwang 20060112
	[P_CHA_Ctrl]=r1		//add by benwang 20060112
	pop r1 from [sp]  	//add by benwang 20060112
	retf

F_SetIRQ_22050:
	//r1=C_Timer_Setting_22K_For_48MHz //del by benwang 20060112
	//[P_TimerE_Preload]=r1 //del by benwang 20060112
	push r1 to [sp];   	//add by benwang 20060112
	r1=[P_CHA_Ctrl]  	//add by benwang 20060112
	r1&=~0x0f        	//add by benwang 20060112 
	r1|=C_SampleRate_22_05K//add by benwang 20060112
	[P_CHA_Ctrl]=r1		//add by benwang 20060112
	pop r1 from [sp]  	//add by benwang 20060112
	retf
	
F_SetIRQ_44100:			//added by chengye 2007/6/17
	push r1 to [sp];
	r1=[P_CHA_Ctrl]
	r1&=~0x0f
	r1|=C_SampleRate_44_1K
	[P_CHA_Ctrl]=r1
	pop r1 from [sp]
	retf	

.external R_A6400S_Sf_ind		//added by zgq on 20051215
.public F_SP_SACM_SetSampleRate	//added by zgq on 20051215
F_SP_SACM_SetSampleRate:		//added by zgq on 20051215
		push r3,r4 to [sp];		//added by zgq on 20051215
		r3 = [R_A6400S_Sf_ind];	//added by zgq on 20051215

		r4 = seg T_DAC_Timer
		ds = r4
		r3 += offset T_DAC_Timer
		r3 = ds:[r3]

		r4=[P_CHA_Ctrl]
		r4&=~0x0f
		r4|=r3
		[P_CHA_Ctrl]=r4
		
//		r3 = [r3];				//added by zgq on 20051215
//		r3 = r3 asr 1;			//added by zgq on 20051215  //del by zgq on 20051215
//		[P_TimerE_Preload] = r3	//added by zgq on 20051215
		pop r3,r4 from [sp];		//added by zgq on 20051215
		retf;					//added by zgq on 20051215
		
.external R_MP3_Sf_ind				//added by benwang 20060111
.public F_SP_SACM_MP3_SetSampleRate	//added by benwang 20060111
F_SP_SACM_MP3_SetSampleRate:		//added by benwang 20060111
		push r3,r4 to [sp];			//added by benwang 20060111
		r3 = [R_MP3_Sf_ind];		//added by benwang 20060111

		r4 = seg T_MP3_DAC_Timer
		ds = r4
		r3 += offset T_MP3_DAC_Timer
		r3 = ds:[r3]

		r4=[P_CHA_Ctrl]
		r4&=~0x0f
		r4|=r3
		[P_CHA_Ctrl]=r4		
		
		//r3 = [r3];					//added by benwang 20060111
		//r3 = r3 asr 1;				//added by benwang 20060111
		//[P_TimerE_Preload] = r3		//added by benwang 20060111
		pop r3,r4 from [sp];			//added by benwang 20060111
		retf;						//added by benwang 20060111
		

//add by benwang start 20060221
.external R_A128_Sf_ind				
.public F_SP_SACM_A128_SetSampleRate	
F_SP_SACM_A128_SetSampleRate:
		push r3,r4 to [sp];
		r3 = [R_A128_Sf_ind];

		r4 = seg T_A128_DAC_Timer
		ds = r4
		r3 += offset T_A128_DAC_Timer
		r3 = ds:[r3]

		r4=[P_CHA_Ctrl]
		r4&=~0x0f
		r4|=r3
		[P_CHA_Ctrl]=r4		
		

		pop r3,r4 from [sp];			
		retf;						
//add by benwang end 20060221

//=====================For A128 Play use, set the samplerate=============
.public _A128_GetSampleRate
_A128_GetSampleRate:
		push R2, R3 to [sp]
		R1 = [R_A128_Sf_ind]

		r3 = seg T_A128_SampleRate
		ds = r3
		r1 += offset T_A128_SampleRate
		r1 = ds:[r1]

		pop R2, R3 from [sp]
		retf
//===========================set samplerate end==========================

.public	F_ADC_SetIRQ_8000
F_ADC_SetIRQ_8000:	
		push r1 to [sp]
		r1=[P_HQADC_Ctrl]
		r1&=~0x0700
		r1|=0x0500
		[P_HQADC_Ctrl]=r1
		pop	r1 from [sp]
		retf
		
.public	F_ADC_SetIRQ_16000
F_ADC_SetIRQ_16000:	
		push r1 to [sp]
		r1=[P_HQADC_Ctrl]
		r1&=~0x0700
		r1|=0x0200
		[P_HQADC_Ctrl]=r1
		pop	r1 from [sp]
		retf

//.comment @
T_EQ_COEF:
	.dd	0x3F2AEC,0x80DB59,0x3DCDD8,0x825E4A,0x3AA52B, 0x86676F
	.dd	0x311BE2,0x978040,0x2150F8,0xC4C1F1,0x086CFE,0x1BB754,0xC0D513		

T_EQ_GAIN:
	.dd	0x0809BC,0x0904D1,0x0A1E89,0x0B5AA1,0x0CBD4B,0x0E4B3B,0x1009B9,0x11FEB3
	.dd	0x1430CD,0x16A77D,0x196B23,0x1C8520,0x200000,0x23E793,0x28491E,0x2D3382
	.dd	0x32B772,0x38E7AA,0x3FD930,0x47A39A,0x50615F,0x5A3031,0x653161,0x718A50
	.dd	0x7F64F0

T_EQ_Mode:
	.dw	12,12,12,12,12,12,12	// 0. reset
	.dw	15,14,11,9 ,14,16,16	// 1. Rock
	.dw	12,12,12,13,11,10,10	// 2. Classical
	.dw	10,13,15,14,10, 9, 9	// 3. Pop
	.dw	12,12,13,14,13,14,15	// 4. Jazz
	.dw	11,12,13,12,13,14,14	// 5. Folk
	.dw	15,13,11,11,13,15,15	// 6. Disco	
	.dw	12,12,11,12,14,14,12	// 7. Reggae	
	.dw	15,15,13, 9, 8,10,16	// 8. Solf Rock		
	.dw	14,13,12,12,12,13,13	// 9. New Age	
////////////////////////////////////		
//@

T_HRTF_COEF:
	.dw	0x0167,0xFD07,0x00A8,0xFF57,0x0153,0x030E,0xFD68,0xFBE6,0x01EA,0xFFF3,0x0217,0x041E,0xFE62,0xF7B4,0xFC4E,0x05E4
	.dw	0x2601,0x0532,0xFC17,0xF7C6,0xFEA1,0x03C1,0x0172,0xFFBE,0x0236,0xFBE9,0xFCDF,0x024D,0x0096,0xFF01,0x0006,0xFB21
	.dw	0xF88B,0xFE1A,0xFF5E,0xFFA9,0xFF43,0xFF3F,0xFF76,0x0003,0x004C,0xFFCB,0xFF5B,0xFFA2,0x0040,0x0012,0xFFC9,0xFF4E
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.dw	0x0167,0xFD07,0x00A8,0xFF57,0x0153,0x030E,0xFD68,0xFBE6,0x01EA,0xFFF3,0x0217,0x041E,0xFE62,0xF7B4,0xFC4E,0x05E4
	.dw	0x2710,0x0695,0xFC85,0xF7A2,0xFE22,0x047C,0x02BC,0x0028,0x019D,0xFBE4,0xFDF2,0x03CE,0x0210,0xFFAE,0x014A,0xFEED
	.dw	0x0775,0x01E6,0x00A2,0x0057,0x00BD,0x00C1,0x008A,0xFFFD,0xFFB4,0x0035,0x00A5,0x005E,0xFFC0,0xFFEE,0x0037,0x00B2
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.dw	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

T_CCS_COEF:
	.dw	0xFEC6,0xFF53,0xFF71,0x001F,0x0007,0x004D,0xFFDE,0x0015,0xFFEA,0x008A,0x00AB,0x0142,0x0109,0x0118,0x007D,0x0077
	.dw	0x0024,0x0078,0x0069,0x0099,0x0031,0xFFDF,0xFF4B,0xFF11,0xFF0F,0xFF4C,0xFFB3,0xFFBE,0xFFD9,0xFF54,0xFF68,0xFEFE
	.dw	0xFFC4,0xFFB4,0x00D2,0x004A,0x0105,0xFFCC,0x00AC,0xFFB7,0x019C,0x0125,0x037F,0x0279,0x046B,0x02AE,0x04E7,0x0390
	.dw	0x06D8,0x05BD,0x08D9,0x0645,0x07EA,0x03CD,0x052C,0x0159,0x03B5,0x0004,0x0192,0xFBA1,0xFAE2,0xF39F,0xF435,0xF111
	.dw	0xF7F3,0x39C4,0x065A,0x0A13,0x1191,0x0EC4,0x0EF0,0x0688,0x03FF,0xFC4E,0xFC65,0xF815,0xFA5B,0xF703,0xF8BA,0xF4E5
	.dw	0xF652,0xF391,0xF659,0xF560,0xF8D4,0xF807,0xFA81,0xF910,0xFAE3,0xF9E8,0xFC30,0xFC1A,0xFE62,0xFE26,0xFF7C,0xFEC6
	.dw	0xFFA8,0xFF62,0x009E,0x00F0,0x0202,0x0203,0x0237,0x01BD,0x0195,0x0166,0x0182,0x01A9,0x0192,0x0158,0x00A1,0x001D
	.dw	0xFF63,0xFF54,0xFF19,0xFF6E,0xFF2F,0xFF41,0xFEA3,0xFE95,0xFE28,0xFE91,0xFE9B,0xFF40,0xFF27,0xFF69,0x0000,0x0000
	.dw	0xFFB8,0xFFCC,0xFF6F,0xFFD8,0xFFC9,0x0049,0x000A,0x0031,0xFFA8,0xFFBF,0xFF6F,0xFFDF,0xFFD6,0x004A,0x0007,0x0019
	.dw	0xFF98,0xFFA4,0xFF73,0xFFD5,0xFFEE,0x0038,0x0012,0xFFE6,0xFF99,0xFF6B,0xFF90,0xFFAF,0x0027,0x0006,0x003D,0xFF8A
	.dw	0xFFB6,0xFF05,0xFFCF,0xFF62,0x0087,0xFFAD,0x0086,0xFEFD,0xFFE8,0xFE72,0x002B,0xFEFF,0x0111,0xFF48,0x00E6,0xFE51
	.dw	0x0011,0xFDC3,0x008E,0xFEB3,0x01BF,0xFF07,0x0134,0xFD7B,0xFFC6,0xFCD0,0x00B4,0xFECC,0x02E1,0xFF84,0x013A,0xFB46
	.dw	0xFB9C,0x34BE,0xF8F1,0xF704,0xFD11,0xFCF6,0x02A3,0x008A,0x0351,0xFF06,0x00BA,0xFD18,0x0017,0xFE12,0x0198,0xFF76
	.dw	0x01CE,0xFED7,0x007A,0xFDFC,0x002E,0xFEB7,0x010C,0xFF90,0x0102,0xFF1C,0x0011,0xFEA5,0xFFF6,0xFF4C,0x0098,0xFFDE
	.dw	0x0072,0xFF74,0xFFB9,0xFF2D,0xFFC3,0xFFC1,0x0046,0x0021,0x0013,0xFFB3,0xFF82,0xFF85,0xFFA8,0x000A,0x001B,0x0041
	.dw	0xFFDE,0xFFD0,0xFF6E,0xFFB5,0xFFAB,0x0030,0x000F,0x0048,0xFFC8,0xFFD4,0xFF6D,0xFFCC,0xFFBA,0x0040,0x0000,0x0000	
