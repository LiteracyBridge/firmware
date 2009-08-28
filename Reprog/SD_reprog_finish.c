#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./Reprog/USB_Flash_reprog.h"
#include "./Application\TalkingBook\Include\util.h"

void
FlashReprogLomem(flash *fp, unsigned int *buf)
{
	int i, nbytes, nwords;
	unsigned int *addr;
	long pos;
	extern void RESET();

	pos = lseek(fp->fileHandle, FLASH_APP_ADDR * 2, SEEK_SET);
	
	for (addr = FLASH_APP_ADDR; addr < REPROG_HIMEM; addr += FLASH_SECTOR_SIZE) {
		nbytes = read(fp->fileHandle, (unsigned long)buf<<1, FLASH_SECTOR_SIZE<<1);
		if(nbytes <= 1)
			break;
		
		nwords = nbytes >> 1;
		fp->pflash = addr;
		fp->erasesector(fp);
		for(i=0; i<nwords; i++) {
			(*fp->writeword)(fp, addr + i, buf[i]);
//			nbytes -= 2;  // remove, dbg
		}
	}
	
/*	{ //rhm dbg remove
		fp->pflash = 0x78000;
		fp->erasesector(fp);
		(*fp->writeword)(fp, 0x78000, 0xaa55);
	} */
	
	addr = 0xf800;
	pos = lseek(fp->fileHandle, 0x1f000 /*0xf800 * 2*/, SEEK_SET);  // do the low page
	nbytes = read(fp->fileHandle, (unsigned long)buf<<1, FLASH_SECTOR_SIZE<<1);
	if(nbytes > 1) {
		nwords = nbytes >> 1;
		fp->pflash = 0xf800;
		fp->erasesector(fp);
		for(i=0; i<nwords; i++) {
			(*fp->writeword)(fp, addr + i, buf[i]);
//			nbytes -= 2;  // remove, dbg
		}
	}
	
/*	{ //rhm dbg remove
		(*fp->writeword)(fp, 0x78001, 0xbb66);
	} */
		
	RESET();
}