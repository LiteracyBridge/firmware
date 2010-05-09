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

void
FlashReprogLomem(flash *fp, unsigned int *buf)
{
	int i, nbytes, nwords;
	unsigned int *addr, *saveaddr, ret;
	unsigned long pos;
	extern void RESETPCB();
	struct f_info fInfo;
	unsigned char fn[40];
	
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

	ret =_findfirst((LPSTR)FIRMWARE_PATH UPDATE_FN, &fInfo, D_FILE);
	if (ret < 0)
		return;    // should not happen

	strcpy(fn, FIRMWARE_PATH);
	strcat(fn, fInfo.f_name);

	newfp->fileHandle = open(fn, O_RDONLY);
	if(newfp->fileHandle < 0)
		return;  // should not happen
	

// MX memory erase 30000 in eight 4k blocks - all others are 32k blocks
	if(fp->Flash_type == MX_MID) {	
		for(pos=BASEADDR; pos<0x37000; pos+= 0x1000) { // 0x37000 is serial number flash, skip
			fp->pflash = pos;
			fp->erasesector(fp);
		}
		for(pos = 0x38000; pos < REPROG_STAND_ALONE; pos += FLASH_ERASE_SIZE) { 
			fp->pflash = pos;
			fp->erasesector(fp);
		}
	} else { // SST memory
		for(pos = 0x30000; pos < REPROG_STAND_ALONE; pos += FLASH_LOW_SECTOR_SIZE) {
			if(pos == 0x37000) {
				pos = 0x38000;  // 0x37000 is serial number flash, skip
			}
			fp->pflash = pos;
			fp->erasesector(fp);
		}
//		fp->pflash = 0xf800;   // to be safe, maps to 0x3f800
//		fp->erasesector(fp);		
	}
	
	fp->pflash = BASEADDR;

	pos = lseek(newfp->fileHandle, BASEADDR * 2, SEEK_SET);
			
	for (addr = BASEADDR, /*offset = 0,*/ nwords = 0; addr < REPROG_STAND_ALONE; addr += FLASH_SD_READ_SIZE)
	{
		if(addr == 0x37000) { //app_flash_data
			pos = lseek(newfp->fileHandle, 0x38000 * 2, SEEK_SET);
			addr = 0x38000;
		} else if(addr == 0x3f800) {  //monkey business -- 0xf800 maps to 38000, leave until last
			pos = lseek(newfp->fileHandle, 0x40000 * 2, SEEK_SET);
			addr = 0x40000;
		}
		nbytes = read(newfp->fileHandle, (unsigned long)buf<<1, FLASH_SD_READ_SIZE<<1);
		if(nbytes <= 1)
			break;
		
		nwords = nbytes >> 1;
		newfp->pflash = addr;
		
		for(i=0; i<nwords; i++) {
			if(buf[i] == 0xffff)
				continue;
			(*newfp->writeword)(newfp, addr + i, buf[i]);
		}
	}

	pos = lseek(newfp->fileHandle, 0x1f000 /*0xf800 * 2*/, SEEK_SET);  // do the low page
	nbytes = read(newfp->fileHandle, (unsigned long)buf<<1, FLASH_SD_READ_SIZE<<1);

	addr = 0xf800;
	if(nbytes > 1) {
		nwords = nbytes >> 1;
		addr = 0x3f800;
		for(i=0; i<nwords; i++) {
			if(buf[i] == 0xffff)
				continue;		
			(*newfp->writeword)(fp, addr + i, buf[i]);
		}
	}
	_deviceunmount(0);			
	RESETPCB();
}
