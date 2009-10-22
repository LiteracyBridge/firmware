// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "./Application\TalkingBook\Include\device.h"
#include "./Application\TalkingBook\Include\SD_reprog.h"

void Check_flash_reprog();
void NewCurrent();
void FlashReprogHimem(flash *fp);
void FlashReprogLomem(flash *fp, unsigned int *);

int ReprogFlash(flash *);

void Try_SD_reprog(flash *fp)
{
	unsigned int ret = 0;
	
	ret = open((LPSTR)(ATTEMPTED_FN),O_RDONLY);
		
	if(ret >= 0) {
		close(ret);
		NewCurrent ();  // make attempted firmware current firmware file
	}

	FlashReprogHimem(fp);

}

void
FlashReprogHimem(flash *fp)
{
	int i, nbytes, nwords;
	unsigned int buf[FLASH_SECTOR_SIZE], *addr;
	long pos;
	extern void FlashReprogLomem();
	
	pos = lseek(fp->fileHandle, REPROG_HIMEM * 2, SEEK_SET);

	for (addr = REPROG_HIMEM; addr < ENDADDR; addr += FLASH_SECTOR_SIZE) {
		nbytes = read(fp->fileHandle, (unsigned long)buf<<1, FLASH_SECTOR_SIZE<<1);
		if(nbytes <= 0)
			break;
		
		nwords = nbytes >> 1;
		fp->pflash = addr;
		fp->erasesector(fp);
		for(i=0; i<nwords; i++) {
			(*fp->writeword)(fp, addr + i, buf[i]);
//			nbytes -= 2;  // remove, dbg
		}
	}
	
	FlashReprogLomem(fp, buf);	
}
void
NewCurrent()
{
	char prev[128];
	char cur [128];
	char att [128];

	strcpy(prev,FIRMWARE_PATH);
	strcpy(cur,FIRMWARE_PATH);
	strcpy(att,FIRMWARE_PATH);
	
	strcat(prev,PREV_FN);
	strcat(cur,CURRENT_FN);
	strcat(att,ATTEMPTED_FN);
	
	unlink((LPSTR)prev);
	rename((LPSTR)cur, (LPSTR)prev);
			// move Attempt to Current
	unlink((LPSTR)cur);
	rename((LPSTR)att, (LPSTR)cur);
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

void check_new_sd_flash()
{
	int ret;
	flash  FL = {0};
	char Flash_fileName[64];
	char Attempt_fileName[64];
	char Archive_path[64];
	int fl_size = USB_Flash_init((flash *)0, 0);
	int flash_execution_buf[fl_size];
	char strLog[40];
	
	// if following file exists reprogram the flash with it
	strcpy(Flash_fileName,FIRMWARE_PATH);
	// rename the above to Attempted during reprogramming (the file has neither succeeded or failed)
	// if Attempted exists at powerup reprogramming was interrupted so reprogram Attempted again
	//   (until it succeeds or fails)
	strcpy(Attempt_fileName,FIRMWARE_PATH);
	// upon successful reprogramming move Current to Prev, Attempted to Current
	

	 strcat(Flash_fileName, UPDATE_FN);
	 strcat(Attempt_fileName, ATTEMPTED_FN);

// see if \system\firmware\update has a .bin file to place in flash
	
	FL.fileHandle = open((LPSTR)(Flash_fileName),O_RDONLY);
	if(FL.fileHandle < 0) {
		FL.fileHandle = open((LPSTR)(Attempt_fileName),O_RDONLY);
		if(FL.fileHandle < 0) {
			return;	// no bin file in update, return
		}
	} else {
		ret = rename((LPSTR)(Flash_fileName), (LPSTR)(Attempt_fileName));
	}
	setLED(LED_RED,TRUE);
	strcpy(strLog, "Reprogramming with new firmware update");	
	logString(strLog, ASAP);

	FL.flash_exe_buf = (void *) &flash_execution_buf[0];
	USB_Flash_init(&FL, 1);

    // be sure System\Firmware\Archive exists
	strcpy(Archive_path,FIRMWARE_PATH);
	strcat(Archive_path,"Archive");
	mkdir((LPSTR)Archive_path);
	
	Try_SD_reprog(&FL);

	setLED(LED_RED,FALSE);

	__asm__("irq on");
	__asm__("fiq on");

}

