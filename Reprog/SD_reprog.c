// Copyright 2009,2010 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org

#include "./Application\TalkingBook\Include\device.h"
#include "./Application\TalkingBook\Include\SD_reprog.h"
#include "./Application\TalkingBook\Include\audio.h"

void Check_flash_reprog();
static void NewCurrent();
void FlashReprogHimem(flash *fp);
void FlashReprogLomem(flash *fp, unsigned int *);

// flash address 0xf800 maps to 0x3f800, but with 32k block flash device the erase at this address
//   erases 0x38000 thru 0x3ffff, the code below reserves that 32k block, and it it not used
//__asm__("REPROG_DUM:	.section .code, .addr=0x38000");
//__asm__(".public _reprog_reserve");
// reserve 0x30000 thru 0x37ffff
//__asm__(".dw 16384 dup(?)");
//__asm__(".dw 16384 dup(?)");
// reserve 0x38000 until linker complains
//__asm__("_reprog_reserve:");
//	__asm__(".dw 16384 dup(?)");
//	__asm__(".dw 8192 dup(?)");
//	__asm__(".dw 4096 dup(?)");
//	__asm__(".dw 2048 dup(?)");


int ReprogFlash(flash *);

void Try_SD_reprog(flash *fp) {
	
	NewCurrent ();  // make attempted firmware current firmware file
	FlashReprogHimem(fp);
}

void
FlashReprogHimem(flash *fp) {
	int  i, nbytes, nwords;
	unsigned int buf[2048], *addr;
	long pos;
	extern void FlashReprogLomem();
	unsigned long offset;
	
	// see if high mem flash reprogramming code is in file
	pos = lseek(fp->fileHandle, REPROG_STAND_ALONE * 2, SEEK_SET);
	if(pos < 0) {
		if((*((unsigned int *)(&FlashReprogLomem))) == 0xffff) {
			return;  // should not happen
		}
		goto prog_himem;
	}

	__asm__("irq off");
	__asm__("fiq off");
		
	if(fp->Flash_type == MX_MID) {		
		for(fp->pflash = REPROG_STAND_ALONE; fp->pflash < 0xb0000; fp->pflash += FLASH_ERASE_SIZE) {
			fp->erasesector(fp);
		}
	} else {  // SST memory
		for(fp->pflash = REPROG_STAND_ALONE; fp->pflash < 0xb0000; fp->pflash += FLASH_LOW_SECTOR_SIZE) {
			fp->erasesector(fp);
		}
	}
	
	offset = 0;
	for (addr = (void *)REPROG_STAND_ALONE; (unsigned long)addr < (unsigned long)ENDADDR; addr += 2048) {
		nbytes = read(fp->fileHandle, (unsigned long)buf<<1, 2048<<1);
		if(nbytes <= 0)
			break;
		
		nwords = nbytes >> 1;
		
		// write nwords to flash
		for(i=0; i<nwords; i++) {
			if(buf[i] == 0xffff)
				continue;
			(*fp->writeword)(fp, REPROG_STAND_ALONE + i + offset, buf[i]);
	    }
	    offset += nwords;
	}
	
	
prog_himem:
	FlashReprogLomem(fp, buf);	

}

static void
NewCurrent() {
	int ret;
	struct f_info file_info;
	char filename[40];
	
	ret = chdir((LPSTR)DEFAULT_SYSTEM_PATH);
	ret =_findfirst((LPSTR)"*" PREV_EXT, &file_info, D_FILE);
	if (ret >= 0) 
		ret = unlink((LPSTR)file_info.f_name);	
	ret =_findfirst((LPSTR)UPDATE_FN, &file_info, D_FILE);
	if (ret >= 0) {
#ifdef PCBTESTS
		//CHANGE MADE BELOW JUST FOR PCB TESTS: 
		strcpy(filename,UPDATE_FP);
		strcat(filename,file_info.f_name);
#else
		strcpy(filename,file_info.f_name);
		strcat(filename,PREV_EXT);		
#endif		
		rename((LPSTR)file_info.f_name,(LPSTR)filename);
	}
	// move Attempt to Current
	ret =_findfirst((LPSTR)"*" ATTEMPTED_EXT, &file_info, D_FILE);
	if (ret >=0) {
		strcpy(filename,file_info.f_name);
		filename[strlen(filename)-strlen(ATTEMPTED_EXT)] = 0;
		rename((LPSTR)file_info.f_name,(LPSTR)filename);
	}
	
}

/*
void
ReprogFailed(flash *fp)
{
	int i;
	
	// move Attempt to Bad if Attempt exists
	fp->ret = open((LPSTR)(ATTEMPTED_FN),O_RDONLY);
	if(fp->ret >= 0) {
		close(fp->ret);
		fp->ret = unlink(BAD_FN);
		fp->ret = rename(ATTEMPTED_FN, BAD_FN);
	}
	
	// reprog with Current
	fp->fileHandle = open((LPSTR)(CURRENT_FN),O_RDONLY);
	if(fp->fileHandle < 0) {
		if(fp->prev_tries++ == 0) {	// first time ??
			_copy(PREV_FN , CURRENT_FN);
		}
	}
	fp->fileHandle = open((LPSTR)(CURRENT_FN),O_RDONLY);
	if(fp->fileHandle < 0) {
		// we are toast, recover vis usb
		return;
	}
	
	for(i = 0; i < 3; i++) {

//			fp->ret = FlashReprog(fp);
		fp->ret = (*fp->flashreprog)(fp);
		if((fp->flash_err == 0) && (fp->ckerr == 0)) {
			//
			// jump to hardware rest vector reset
			//
		}
	}
}
*/

void check_new_sd_flash() {
	extern void write_app_flash(int *, int, unsigned int);
	struct f_info file_info;
	char *newSN;
	int ret;
	
	// see if a .tsn file is present
	ret =_findfirst((LPSTR)UPDATE_FP SERIAL_FN, &file_info, D_FILE);
	if(ret >= 0) {
		char *dot;
		int lenEraseCode = strlen((char *)ERASE_SN_CODE);
		int flagErase, flagSNexists;
		flagSNexists = SNexists();
		newSN = file_info.f_name;
		dot = strrchr(newSN, '.'); //looks for last '.' to allow a '.' to exist within the pre-ext filename.
		flagErase = !strncmp(newSN,(char *)ERASE_SN_CODE,lenEraseCode);
		if (flagErase)
			newSN += lenEraseCode;
		unlink(file_info.f_name);
		if(dot != NULL && (!flagSNexists || (flagSNexists && flagErase)) ) {
			int vlen = strlen(newSN) - 3; //skip tsn
			if (newSN == dot)
				vlen = 0;
			else
				*dot = 0;
			write_app_flash((int *)newSN, vlen, (unsigned int)0xffff);
		}
	}
			
	ret =_findfirst((LPSTR)UPDATE_FP UPDATE_FN, &file_info, D_FILE);
	if (ret < 0)
		file_info.f_name[0] = 0;
	startUpdate(file_info.f_name);
}

void startUpdate(char *filenameUpdate) {
	int ret;
	flash  FL = {0};
	int fl_size = USB_Flash_init((flash *)0, 0);
	int flash_execution_buf[fl_size];
	char oldPath[60], newPath[60];
	struct f_info file_info;
	
	// if following file exists reprogram the flash with it
	// rename the above to Attempted during reprogramming (the file has neither succeeded or failed)
	// if Attempted exists at powerup reprogramming was interrupted so reprogram Attempted again
	//   (until it succeeds or fails)
	// upon successful reprogramming move Current to Prev, Attempted to Current
	
	ret = mkdir((LPSTR)DEFAULT_SYSTEM_PATH);
	// see if new image file exists or was attempted	

	if(filenameUpdate[0] != 0) {  // assumes only one version in update location
		strcpy(oldPath,UPDATE_FP);
		strcat(oldPath,filenameUpdate);
		strcpy(newPath,DEFAULT_SYSTEM_PATH);
		strcat(newPath,filenameUpdate);
		strcat(newPath,ATTEMPTED_EXT);
		
		ret = rename((LPSTR)oldPath, (LPSTR)newPath); 
		FL.fileHandle = open((LPSTR)newPath,O_RDONLY);
	} else {
		ret =_findfirst((LPSTR)DEFAULT_SYSTEM_PATH "*" ATTEMPTED_EXT, &file_info, D_FILE);		
		if (ret >= 0) {
			strcpy(oldPath,DEFAULT_SYSTEM_PATH);
			strcat(oldPath,file_info.f_name);
			FL.fileHandle = open((LPSTR)oldPath,O_RDONLY);		
		} else
			FL.fileHandle = -1;
	}

	if(FL.fileHandle < 0)
			return;	// no system update image in folder, return
	playBips(2);
	setLED(LED_RED,TRUE);
	
	//commenting these two lines out since we want reprogramming to happen before config file loading where log filename is given
	//strcpy(strLog, "Reprogramming with new firmware update");	
	//logString(strLog, ASAP);

	FL.flash_exe_buf = (void *) &flash_execution_buf[0];
	USB_Flash_init(&FL, 1);

	Try_SD_reprog(&FL);
	setLED(LED_RED,FALSE);

	__asm__("irq on");
	__asm__("fiq on");

}

