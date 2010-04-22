// Copyright 2009,2010 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./Reprog1/USB_Flash_reprog.h"
//#include "./Application\TalkingBook\Include\util.h"

#define FIRMWARE_PATH	"a:\\System\\Firmware\\"
//TODO: change this to use any *.bin and keep filename for easy version identification
#define UPDATE_FN    	"a:\\System.bin"
#define ATTEMPTED_FN	FIRMWARE_PATH"Attempted.bin"
#define PREV_FN			FIRMWARE_PATH"Previous.bin"
#define CURRENT_FN 		FIRMWARE_PATH"Current.bin"


void
FlashReprogLomem(flash *fp, unsigned int *buf)
{
	int i, nbytes, nwords, nbytes1;
	unsigned int *addr, *saveaddr, *flash_buf, ret;
	unsigned long pos;
	extern void RESET();
	unsigned int offset;
	unsigned int rhmtestbuf[2048];
	
	flash  FL = {0}, *newfp; 
	int fl_size = USB_Flash_init((flash *)0, 0);
	int flash_execution_buf[fl_size];
	FL.flash_exe_buf = (void *) &flash_execution_buf[0];
	USB_Flash_init(&FL, 1);
	(flash *)newfp = &FL;
	
	SD_Initial();
	fs_init();
	ret = _devicemount(0);
	if (ret)
		ret = _getfserrcode();

	newfp->fileHandle = open(CURRENT_FN, O_RDONLY);
	if(newfp->fileHandle < 0)
		newfp->fileHandle = fp->fileHandle;
	
	flash_buf = fp->pflash;

	// rhm read low bytes, for some reason it reads zeros below
	pos = lseek(newfp->fileHandle, 0x1f000 /*0xf800 * 2*/, SEEK_SET);  // do the low page
	nbytes1 = read(newfp->fileHandle, (unsigned long)rhmtestbuf<<1, FLASH_LOW_SECTOR_SIZE<<1);

//	pos = lseek(newfp->fileHandle, FLASH_APP_ADDR * 2, SEEK_SET);
	pos = lseek(newfp->fileHandle, 0x40000 * 2, SEEK_SET);
			
//	for (addr = FLASH_APP_ADDR, offset = 0, nwords = 0; addr < 0xa0000; addr += nwords)
	for (addr = 0x40000, offset = 0, nwords = 0; addr < 0xa0000; addr += nwords)
	{

		nbytes = read(newfp->fileHandle, (unsigned long)buf<<1, FLASH_SD_READ_SIZE<<1);
		if(nbytes <= 1)
			break;
		
		nwords = nbytes >> 1;
		newfp->pflash = addr;
		
		if(((unsigned long) addr & FLASH_SD_ERASE_MASK) == 0) { // erase at each NOR page boundary 
			if((addr != 0x40000) && (addr != flash_buf)) { // not first time , flash buuffer
				copy_Flash_to_Flash(newfp);
			}
			newfp->pflash = flash_buf;
			newfp->erasesector(newfp);
			offset = 0;
		}
	
		if((addr >= flash_buf) && (addr <= (flash_buf + 0x7fff))) {
			offset += nwords;
			continue;
		}
			
		for(i=0; i<nwords; i++) {
			if(buf[i] == 0xffff)
				continue;
			(*newfp->writeword)(newfp, flash_buf + i + offset, buf[i]);
		}
		offset += nwords;
	}
	if(offset > 0) {
		newfp->pflash += FLASH_SD_ERASE_MASK;
		(unsigned long)newfp->pflash &= 0xffff8000;
		copy_Flash_to_Flash(newfp);
	}
	
//	newfp->pflash = addr;
//	(unsigned long)newfp->pflash &= ~FLASH_SD_ERASE_MASK;
//	copy_Flash_to_Flash(newfp);
		
// in a 2k flash defive this maps to 0x3f800 to 3ffff
// in the 32k device the maping is the same, but the erasesector below erases 0x38000 thru 0x3ffff
//   in USBD_drv.asm  this memory is reserved and unused see:: REPROG_DUM:	.section .code, .addr=0x38000

	addr = 0xf800;
	if(nbytes1 > 1) {
		nwords = nbytes1 >> 1;
		newfp->pflash = (unsigned long) 0x00038000;
		newfp->erasesector(newfp);
		addr = 0x3f800;
		for(i=0; i<nwords; i++) {
			if(rhmtestbuf[i] == 0xffff)
				continue;		
			(*newfp->writeword)(fp, addr + i, rhmtestbuf[i]);
		}
	}
	_deviceunmount(0);			
	RESET();
}

static void
copy_Flash_to_Flash(flash *fp) {
	unsigned long i;
	unsigned int *addr;
	unsigned int *buf = (unsigned int *) FLASH_TO_FLASH_BUF;
	extern void *reprog_reserve;
		
	fp->pflash = fp->pflash - (unsigned)FLASH_ERASE_SIZE;
	if(fp->pflash == (unsigned long)0x38000)
		return;
	
	fp->erasesector(fp);
	
	for(i=0, addr = fp->pflash; i < (unsigned)FLASH_ERASE_SIZE; i++) {
		if(buf[i] == 0xffff)
			continue;		
		(*fp->writeword)(fp, addr + i, buf[i]);
    }

}
