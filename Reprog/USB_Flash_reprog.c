#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"

#include "USB_Flash_reprog.h"

int
USB_Flash_init(flash *fp, int function)
{
	int i, *ip;	
	void *(*pCheck)(flash *) = fp->flash_exe_buf;

// get space on stack for Check_flash_reprog routine	
	int *plow = (int *)&EraseSector;
	int *phigh = (int *)&EndFlashProg;
	int plength = phigh - plow;
//	int flash_execution_buf[plength+1];	
	
	if(function == 0) {
		return (plength+1);
	}
	
	ip = (int *) fp->flash_exe_buf;
	
	// put code for Check_flash_reprog on stack
	for(i=0; i<=plength; i++) {
		*ip++ = *plow++;
	}
	
	// build pointers for our state S37 state functions	

	plow = (int *)&EraseSector;  // lowest routine on the stack
	
	phigh = (int *)&NanoPause;
	fp->nanopause = pCheck + (phigh - plow);
	
	phigh = (int *) &EraseSector;
	fp->erasesector = pCheck + (phigh - plow);
	
	phigh = (int *) &WriteWord;
	fp->writeword = pCheck + (phigh - plow);
	
	phigh = (int *) &MemCopy;
	fp->memcopy = pCheck + (phigh - plow);


	
//test	FL.pflash = 0x30800;
	
//test	(*FL.erasesector) (&FL);

}
void MemCopy(unsigned int *to, unsigned int *from, unsigned int count)
{
again:
	*to++ = *from++;
	if(--count != 0)
		goto again;
}

void EraseSector(flash *fp)
{
	unsigned int i;

	*(WORD *) (BASEADDR + 0x5555) = 0x00AA;     // write data 0x00AA to device addr 5555H
	*(WORD *) (BASEADDR + 0x2AAA) = 0x0055;     // write data 0x0055 to device addr 2AAAH
	*(WORD *) (BASEADDR + 0x5555) = 0x0080;     // write data 0x0080 to device addr 5555H
	*(WORD *) (BASEADDR + 0x5555) = 0x00AA;     // write data 0x00AA to device addr 5555H
	*(WORD *) (BASEADDR + 0x2AAA) = 0x0055;     // write data 0x0055 to device addr 2AAAH
	
	*(WORD *) (fp->pflash) = 0x0030;     // write data 0x0030 to device sector addr 5555H

	for(i = *(WORD *) (fp->pflash);  (i & 0x0080) == 0; ) {
		i = *(WORD *) (BASEADDR);
	}

	(*fp->nanopause)(1000);

}

//RHM modified pause from TalkingBook.c
void NanoPause(int t) { //t=time in nano secs
	int i;
//	float a = 16564542,b=34534;
//	int factor = 96000; // cycles per milisecond
//	int factor = 96; // cycles per microsecond
//    int factor = 1; // about 1 cycle per 10 nano secs
//    t /= 10;	//units of 10 nano sec
//	for (i = 0; i++; i < (factor * t))
//	for (i = 0; i++; i < t)
//	{
//		a = a / b;	
//	}
	for (i = 0; i < 1000; i++) {
		__asm__("push	r1,r5 to [sp]");
		__asm__("pop	r1,r5 from [sp]");
	}

	
}

void
WriteWord(flash *fp, WORD *addr, unsigned int dataword)
{
    unsigned int i, j = 10000;
    
    *(WORD *) (BASEADDR + 0x5555 ) = 0x00AA;  // 1st write data 0x00AA to device addr 5555H
    *(WORD *) (BASEADDR + 0x2AAA) = 0x0055;  // 2nd write data 0x0055 to device addr 2AAAH
    *(WORD *) (BASEADDR + 0x5555) = 0x00A0;  // 3rd write data 0x00A0 to device addr 5555H
    
    *addr = dataword; // 4th write data word into destination address Dst

    dataword = dataword & 0x0080;  //true data bit
   
    for(i = (*addr & 0x0080);  (i != dataword) && (j-- > 0); ) {
		i = *addr;
		i &= 0x0080;
	}
	
//	(*fp->nanopause)(1000);

}


void EndFlashProg()
{
}


