//========================================================================================
//Header Name:		unSPMACRO.h
//Applied body:		SPL16256A
//Purpose:		Header File for the C language that try to involve general-purposed MACRO
//Note:			to gain CPU performance, Most MACRO are written as in-line assembly
//Programmer:		Jacky Lin
//Written Date:		2003-4-18
//Version history:		
//========================================================================================

//========================================================================================
//Function:	Write a value to designated control register (I/O Port)
//Example:	outp(P_IOA_Data,0x8000);
//========================================================================================
#define         outp(Addr,Value)		*(volatile unsigned int*) (Addr) = Value;


//========================================================================================
//Function:	Read a value from designated control register (I/O Port)
//Example:	int	i;
//		inp(P_IOA_Data,i);
//========================================================================================
#define 	inp(Addr) *(volatile unsigned int*) (Addr);



//========================================================================================
//Function:	Get segment:offset of designated label
//Example:	int i,j;
//		long	label;
//		GetSegmentOffset(label,i,j);
//========================================================================================
#define         GetSegmentOffset(label,segment,offset)	    \
                asm(                                        \
                        "%1=seg %2\n"                       \
                        "\t%0=offset %2\n"                  \
                        :"=r"(segment),"=r"(offset)         \
                        :"p"(&label)                        \
		);


//========================================================================================
//Function: Disable Movement Flag of FIR DSP Operation
//Example:	Disable_FIRMOV();
//========================================================================================
#define		Disable_FIRMOV()	asm("fir_mov off\n");


//========================================================================================
//Function: Enable Movement Flag of FIR DSP Operation
//Example:	Enable_FIRMOV();
//========================================================================================
#define		Enable_FIRMOV()		asm("fir_mov on\n");


//========================================================================================
//Function: Make CPU software interrupt (break)
//Example:	SoftwareInterrupt();
//========================================================================================
#define         SoftwareInterrupt()	asm("break\n");

//========================================================================================
//Function: Make CPU no operation
//Example:	NOP();
//========================================================================================
#define         NOP()			asm("nop\n");

//========================================================================================
//Function: Set Stack Pointer
//		int	i;
//		SetDataSegment(0x17FF);
//		SetDataSegment(i);
//========================================================================================
#define		SetStackPoint(address)		\
                asm(                        	\
                        "sp=%0;\n"          	\
                        :                   	\
                        :"r"(address)       	\
                        :"0"                	\
                );
