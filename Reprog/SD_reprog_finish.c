// Copyright 2009,2010 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./component/include/fs/vfs.h"
#include "./Reprog/USB_Flash_reprog.h"
#include "./Application\TalkingBook\Include\SD_reprog.h"
//#include "./Application\TalkingBook\Include\util.h"


#define HIMEM_LED_GREEN 0x0040
#define HIMEM_LED_RED   0x0200
#define HIMEM_LED_ALL   0x0240
unsigned int cur_led = HIMEM_LED_GREEN;

void
FlashReprogLomem(flash *fp, unsigned int *buf)
{
	int i, nbytes, nwords, calledfromboot = 0;
	unsigned int *addr, *saveaddr, *startaddr, ret;
	unsigned long pos;
	extern void RESETPCB();
	struct f_info fInfo;
	unsigned char fn[40];
	unsigned int *bufp = buf;
	flash  FL = {0}, *newfp, *fparg; 
	int fl_size = USB_Flash_init((flash *)0, 0);
	int flash_execution_buf[fl_size];	
	
	HimemSetLED(cur_led, TRUE);

	FL.flash_exe_buf = (void *) &flash_execution_buf[0];
	USB_Flash_init(&FL, 1);
	(flash *)newfp = &FL;
	(flash *)fparg = fp;
	
	
	/*  this is BodyInit(), if called from boot we need to do some of this
	IOKey_Initial();	
	SysInitWaitMode();
	ucBSInit();		
	Snd_VarInit();
	SACM_Initial();	
	SD_Initial();
	*/
	
	IOKey_Initial();	
	i = SD_Initial();
	fs_init();
	ret = _devicemount(0);
	if (ret)
		ret = _getfserrcode();
	
	__asm__("irq off");
	__asm__("fiq off");

	ret =_findfirst((LPSTR)DEFAULT_SYSTEM_PATH UPDATE_FN, &fInfo, D_FILE);
	if (ret < 0)
		return;    // should not happen

	strcpy(fn, DEFAULT_SYSTEM_PATH);
	strcat(fn, fInfo.f_name);

	newfp->fileHandle = open(fn, O_RDONLY);
	if(newfp->fileHandle < 0)
		return;  // should not happen

	pos = (unsigned long) fp;
	if((pos & 0xffff0000) != 0)  // if called from FlashReprogHimem pos is on stack and seg == 0 
		calledfromboot = 1;
	if((pos & 0xffff) < 3)	// if called from boot this is boottype, otherwise a ptr to a stack address hepefully >= 3
		calledfromboot = 1;
	
	if (!calledfromboot && fparg->Flash_type == MX_MID) {	// change startup_data.asm to call FlashReprogLomem instead of main until reprogramming completes		
					
		pos = lseek(newfp->fileHandle, 0x1f000 /*0xf800 * 2*/, SEEK_SET);  // do the low page
		nbytes = read(newfp->fileHandle, (unsigned long)bufp<<1, FLASH_SD_READ_SIZE<<1);
		
		HimemSetLED(HIMEM_LED_ALL, FALSE);
		HimemSetLED(HIMEM_LED_RED, TRUE);
		cur_led = HIMEM_LED_RED;

		fparg->pflash = 0x38000;
		fparg->erasesector(fparg);	// erases 0x38000 thru 0x3ffff

		// reprogram boot code to call FlashReprogLomem until reprogramming succeeds
		addr = 0xf800;
		if(nbytes > 1) {
			//nwords = nbytes >> 1;
			addr = 0x3f800;
			for(i=0; i<0x800; i++) {
				if(bufp[i] == 0xffff)
					continue;
				// change call main to call FlashReprogLomem in startup_Data.asm
				if(bufp[i+2] == 0xaaaa & bufp[i+3] == 0x5555) {
					bufp[i] = 0xf04a;
					bufp[i+1] = 0xe000;
				}		
				(*newfp->writeword)(fparg, addr + i, bufp[i]);
			}
		}
		// note that 0x38000 thru 0x3f7ff remains erased
			
	} else {
		
		goto flashrecover;
		
	}
	
// if it is the first attempt reprogramming should proceed as before, the final step reflashed the call to main	

// MX memory ERASE 30000-37000 in 7 4k blocks - 40000-9ffff 32k blocks -- 38000 is special, boot code at 3f800
	if(fparg->Flash_type == MX_MID) {	
		for(pos=BASEADDR; pos<0x37000; pos+= 0x1000) { // 0x37000 is serial number flash, skip
			HimemSwitchLED();
			fparg->pflash = pos;
			fparg->erasesector(fparg);
		}
		for(pos = 0x40000; pos < REPROG_STAND_ALONE; pos += FLASH_ERASE_SIZE) { 
			// note that 0x38000 thru 0x3f7ff remains erased from above
			HimemSwitchLED();
			fparg->pflash = pos;
			fparg->erasesector(fparg);
		}
	} else { // SST memory
		for(pos = 0x30000; pos < REPROG_STAND_ALONE; pos += FLASH_LOW_SECTOR_SIZE) {
			if(pos == 0x37000) {
				pos = 0x38000;  // 0x37000 is serial number flash, skip
			}
			fparg->pflash = pos;
			fparg->erasesector(fparg);
		}
//		fp->pflash = 0xf800;   // to be safe, maps to 0x3f800
//		fp->erasesector(fp);		
	}
	
	HimemSetLED(HIMEM_LED_ALL,FALSE);
	wait(150);
	HimemSetLED(cur_led, TRUE);

	
	fparg->pflash = BASEADDR;

	pos = lseek(newfp->fileHandle, BASEADDR * 2, SEEK_SET);
			
	for (addr = BASEADDR, /*offset = 0,*/ nwords = 0; addr < REPROG_STAND_ALONE; addr += FLASH_SD_READ_SIZE)
	{
		if(addr == 0x37000) { //app_flash_data
			pos = lseek(newfp->fileHandle, 0x40000 * 2, SEEK_SET);
			addr = 0x40000;
		}
		HimemSwitchLED();
		nbytes = read(newfp->fileHandle, (unsigned long)bufp<<1, FLASH_SD_READ_SIZE<<1);
		if(nbytes <= 1)
			break;
		
		nwords = nbytes >> 1;
		newfp->pflash = addr;
		
		for(i=0; i<nwords; i++) {
			if(bufp[i] == 0xffff)
				continue;
			(*newfp->writeword)(newfp, addr + i, bufp[i]);
		}
		
	}
	

//  we need to change startup_Data to call main but we have to erase more than that
	fparg->pflash = 0x38000;
	fparg->erasesector(fparg);	// erases 0x38000 thru 0x3ffff

	pos = lseek(newfp->fileHandle, 0x38000 * 2, SEEK_SET);  // fix 38000 thru 38f7ff 
	
	for (addr = 0x38000, nwords = 0; addr < 0x3f800; addr += FLASH_SD_READ_SIZE) {

//      reflash what we unnecessarily erased
		HimemSwitchLED();
		nbytes = read(newfp->fileHandle, (unsigned long)bufp<<1, FLASH_SD_READ_SIZE<<1);
		if(nbytes > 1) {
			nwords = nbytes >> 1;
			for(i=0; i<nwords; i++) {
				if(bufp[i] == 0xffff)
					continue;		
				(*newfp->writeword)(fparg, addr + i, bufp[i]);
			}
		}
	}

	HimemSetLED(HIMEM_LED_ALL,FALSE);
	wait(150);
	HimemSetLED(cur_led, TRUE);

	pos = lseek(newfp->fileHandle, 0x1f000 /*0xf800 * 2*/, SEEK_SET);  // do the low page
	nbytes = read(newfp->fileHandle, (unsigned long)bufp<<1, FLASH_SD_READ_SIZE<<1);

	addr = 0xf800;
	if(nbytes > 1) {
		nwords = nbytes >> 1;
		addr = 0x3f800;
		for(i=0; i<nwords; i++) {
			if(bufp[i] == 0xffff)
				continue;		
			(*newfp->writeword)(fparg, addr + i, bufp[i]);
		}
	}

	_deviceunmount(0);			
	RESETPCB();


// we started but did not complete reprogramming
flashrecover:

	// called from boot so neither of our args are really present
	// allocate our 2048 byte buffer for file input
	bufp = 0;
	__asm__ (".define FLASH_SD_READ_SIZE 2048");
	__asm__ ("sp -= FLASH_SD_READ_SIZE");  // this should be 2048 == FLASH_SD_READ_SIZE
	bufp = retsp() + 4;

	(flash *)fparg = newfp;	// no valid args so use the flash struct on the stack
	fparg->Flash_type = MX_MID;
	
	HimemSetLED(HIMEM_LED_ALL,FALSE);
	wait(150);
	HimemSetLED(HIMEM_LED_RED, TRUE);
	wait(150);
	HimemSetLED(HIMEM_LED_GREEN, TRUE);
	cur_led = HIMEM_LED_GREEN;
	
	
	// read flash file and see where we need to start reprogramming
	
	fparg->pflash = BASEADDR;
	pos = lseek(newfp->fileHandle, BASEADDR * 2, SEEK_SET);
			
	for (addr = BASEADDR, /*offset = 0,*/ nwords = 0; addr < REPROG_STAND_ALONE; addr += FLASH_SD_READ_SIZE)
	{
		if(addr == 0x37000) { //app_flash_data
			pos = lseek(newfp->fileHandle, 0x40000 * 2, SEEK_SET);
			addr = 0x40000;	// skip app flash data and 38000 block where boot code lives at 3f800
		}
		HimemSwitchLED();
		nbytes = read(newfp->fileHandle, (unsigned long)bufp<<1, FLASH_SD_READ_SIZE<<1);
		if(nbytes <= 1)
			break;
		
		nwords = nbytes >> 1;
		newfp->pflash = addr;
		
		for(i=0, startaddr = 0; i<nwords; i++) {
			if(*(addr+i) != bufp[i]) {
				// we need to erase this block and start reprogramming
				startaddr = addr;
				break;
			}
		}
		if(startaddr != 0)
			break;
	}
	
	if(startaddr != 0) {
		// erase from startaddr to 9ffff
		if(startaddr < 0x38000) {
			(long) startaddr |= 0xfff;		// erase size 0x1000
			(long) startaddr ^= 0xfff;
			for(pos=startaddr; pos<0x37000; pos+= 0x1000) { // 0x37000 is serial number flash, skip
				HimemSwitchLED();
				fparg->pflash = pos;
				fparg->erasesector(fparg);
			}
			for(pos = 0x40000; pos < REPROG_STAND_ALONE; pos += FLASH_ERASE_SIZE) { 
				//if(pos == 0x38000) pos = 0x40000; // leave 0xf800 boot code in place
				// note that 0x38000 thru 0x3f7ff remains erased from above
				HimemSwitchLED();
				fparg->pflash = pos;
				fparg->erasesector(fparg);
			}
		} else {	// startaddr >= 0x38000
			(long) startaddr |= 0x7fff;	// erase size 0x8000
			(long) startaddr ^= 0x7fff;
			for(pos = startaddr; pos < REPROG_STAND_ALONE; pos += FLASH_ERASE_SIZE) { 
				//if(pos == 0x38000) pos = 0x40000; // leave 0xf800 boot code in place
				// note that 0x38000 thru 0x3f7ff remains erased from above
				HimemSwitchLED();
				fparg->pflash = pos;
				fparg->erasesector(fparg);
			}
		}

		// we have areased from startaddr on, now write that area
			
		fparg->pflash = startaddr;
		pos = lseek(newfp->fileHandle, ((long)startaddr) * 2, SEEK_SET);
				
		for (addr = startaddr, /*offset = 0,*/ nwords = 0; addr < REPROG_STAND_ALONE; addr += FLASH_SD_READ_SIZE)
		{
			if(addr == 0x37000) { //app_flash_data
				pos = lseek(newfp->fileHandle, 0x40000 * 2, SEEK_SET);
				addr = 0x40000;
			}
			HimemSwitchLED();
			nbytes = read(newfp->fileHandle, (unsigned long)bufp<<1, FLASH_SD_READ_SIZE<<1);
			if(nbytes <= 1)
				break;
			
			nwords = nbytes >> 1;
			newfp->pflash = addr;
			
			for(i=0; i<nwords; i++) {
				if(bufp[i] == 0xffff)
					continue;
				(*newfp->writeword)(newfp, addr + i, bufp[i]);
			}
		}
	}
//  we need to change startup_Data to call main but we have to erase more than that
	fparg->pflash = 0x38000;
	fparg->erasesector(fparg);	// erases 0x38000 thru 0x3ffff

	pos = lseek(newfp->fileHandle, 0x38000 * 2, SEEK_SET);  // fix 38000 thru 38f7ff 
	
	for (addr = 0x38000, nwords = 0; addr < 0x3f800; addr += FLASH_SD_READ_SIZE) {

//      reflash what we unnecessarily erased
		HimemSwitchLED();
		nbytes = read(newfp->fileHandle, (unsigned long)bufp<<1, FLASH_SD_READ_SIZE<<1);
		if(nbytes > 1) {
			nwords = nbytes >> 1;
			for(i=0; i<nwords; i++) {
				if(bufp[i] == 0xffff)
					continue;		
				(*newfp->writeword)(fparg, addr + i, bufp[i]);
			}
		}
	}

	HimemSwitchLED();
	pos = lseek(newfp->fileHandle, 0x1f000 /*0xf800 * 2*/, SEEK_SET);  // do the low page
	nbytes = read(newfp->fileHandle, (unsigned long)bufp<<1, FLASH_SD_READ_SIZE<<1);

	addr = 0xf800;
	if(nbytes > 1) {
		nwords = nbytes >> 1;
		addr = 0x3f800;
		for(i=0; i<nwords; i++) {
			if(bufp[i] == 0xffff)
				continue;		
			(*newfp->writeword)(fparg, addr + i, bufp[i]);
		}
	}
	
	_deviceunmount(0);			
	RESETPCB();
	
}

void HimemSwitchLED() {
	HimemSetLED(HIMEM_LED_ALL, FALSE);
	cur_led ^= HIMEM_LED_ALL;
	HimemSetLED(cur_led, TRUE);
}

void HimemSetLED(unsigned int color, BOOL on) {
	struct GPIO {
		unsigned int nData;
		unsigned int nBuffer;
		unsigned int nDir;
		unsigned int nAttrib;
		unsigned int nDrv;
	};
	struct GPIO *LEDPort = (struct GPIO *)P_IOB_Data;
//	checkStackMemory();
	// on = FALSE;
	if (on) {
 		LEDPort->nDir 	 |= color;
		LEDPort->nAttrib |= color;  // LEDPort->nData   |= color;
		LEDPort->nBuffer   |= color;
	} else
		LEDPort->nBuffer   &= ~color;  //LEDPort->nData   &= ~color;
}



int
retsp()
{
	__asm__ ("r1 = sp");	// set return value to current sp
}