#include <string.h>
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./Application/TalkingBook/Include/talkingbook.h"
#include "./Application/TalkingBook/Include/files.h"
#include "./Application/TalkingBook/Include/device.h"
#include "./Application/TalkingBook/Include/audio.h"
#include "./Application/TalkingBook/Include/SD_reprog.h"
#include "./Application/TalkingBook/Include/startup.h"

#define NUM_STARTUP_DINGS	1

#define RECORD_TIME_MS		4000
#define LONG_SIZE			1024
#define STARTING_FILE_SIZE	2
#define TEST_FILENAME		"test.bin"	
#define LOCAL_TEST_FILENAME	 "a:/"TEST_FILENAME
#define REMOTE_TEST_FILENAME "b:/"TEST_FILENAME
#define TEST_AUDIO_FILENAME	 "a:/test.a18"	

int sendCopy(void); 
int receiveCopy(void);
int audioTests(void);
int keyTests(int);
int reprogram(void);
int createTestFile(unsigned int);
int readTestFile(void);
void exception(void);
static void flashRed(void);
static void checkUSB(void);

extern void _SystemOnOff(void);
extern int SystemIntoUDisk(unsigned int);
extern int setUSBHost(BOOL);
extern void loadConfigFile(void);
extern INT16 SD_Initial(void);

static void playStartupSound() {
	int i;
	for (i=0; i < NUM_STARTUP_DINGS;i++) {
		playDing();
		flashRed();
	}	
}

int testPCB(void) {
	int ret, key, keys;
	unsigned long count = 0;
	
/*	logLongHex((unsigned long)*P_IOB_Dir);
	logLongHex((unsigned long)*P_IOB_Attrib);
	logLongHex((unsigned long)*P_IOB_Data);
	logLongHex((unsigned long)*P_IOB_Latch);
	logLongHex((unsigned long)*P_MINT_Ctrl);
	logLongHex((unsigned long)0xFFFF);
	logLongHex((unsigned long)0xFFFF);
	logLongHex((unsigned long)0xFFFF);
*/
	//initialization	
	SetSystemClockRate(MAX_CLOCK_SPEED); // to speed up initial startup -- set CLOCK_RATE later
	setDefaults();
	SysDisableWaitMode(WAITMODE_CHANNEL_A);

	adjustVolume(NORMAL_VOLUME,FALSE,FALSE);
	// turn on GREEN LED and keep on while there's power
	setLED(LED_GREEN,TRUE);  
	setLED(LED_RED,FALSE);  // red light can be left on after reprog restart
	
	// play startup Ding sound to test speaker and demonstrate confirmation tone
	
	playStartupSound();
	writeVersionToDisk();  // being in this function usually means that the PCB was just reprogrammed (with a new version)
	key = keyCheck(1); 	// to get rid of wake-up button press
	
	//rhm
//	while (1) {
//		SystemIntoUDisk(1);	
//		playDing();
//		reprogram(); // this will not return
//	}
	//rhm	
	while (1) {
		count++;
		key = keyCheck(0);
		if (key)
			count = 1;
		else if ((count % 150000) == 0)
			checkUSB();	 
		keys |= key;
//		if (key)
//			logLongHex((unsigned long)key);
		// '<' receives a copy (USB device mode)
		// '>' sends a copy (tests microSD card, USB host mode, and copy)
		// '*' reprograms device
		// 'v' starts main tests (keypad, audio, and sleep tests)
		ret = 999; // not 0 and not -1, which should only come from fct returns
		if (key == KEY_LEFT)
			ret = receiveCopy();
		else if (key == KEY_RIGHT)
			ret = sendCopy();
		else if (key == KEY_STAR) 
			ret = reprogram(); // this will not return
		else if (key == KEY_UP)	
			ret = keyTests(keys);   // sending keys means allows them not to require testing
		else if (key == KEY_PLAY)
			ret = audioTests();  
		else if (key == KEY_DOWN)
			setOperationalMode((int)P_SLEEP);  // sleep; wake from center, home, or black button
		else if (key == KEY_PLUS) {	
			adjustVolume(1,TRUE,FALSE);
			playBip();
			flashRed();
		} else if (key == KEY_MINUS) {	
			adjustVolume(-1,TRUE,FALSE);
			playBip();
			flashRed();
		}
		setLED(LED_GREEN,TRUE);
		if (ret == -1) {
			exception();
		}
		else if (ret != 999) {
			playDing();
			setLED(LED_RED,FALSE);
		}
	}
}

static void checkUSB() {
	int usbret;
	usbret = SystemIntoUDisk(USB_CLIENT_SETUP_ONLY);
	while(usbret == 1) {
		usbret = SystemIntoUDisk(USB_CLIENT_SVC_LOOP_ONCE);
	}
	if (!usbret) { //USB connection was made
		SD_Initial();  // recordings are bad after USB device connection without this line (todo: figure out why)
	}
}


void flashRed() {
	setLED(LED_RED,TRUE);
	wait(80);
	setLED(LED_RED,FALSE);
}

int audioTests(void) {
	int key;
	int handle, i;
	int start, end, stopTime;
	long lTime;

	key = 0;
	while (1) {
		// prompt to begin 5-second recording when any button is pushed
		i = unlink((LPSTR)TEST_AUDIO_FILENAME);
		if (MIC_GAIN_NORMAL >= 0) {
			*P_HQADC_MIC_PGA &= 0xFFE0; // only first 5 bits for mic pre-gain; others reserved
			*P_HQADC_MIC_PGA |= MIC_GAIN_NORMAL;
		}
		handle = open((LPSTR)TEST_AUDIO_FILENAME,O_CREAT|O_RDWR);
		if (handle != -1) {
			setLED(LED_RED,TRUE);
			playBip();
			//turn off speaker driver SPK_CE
			*P_IOA_Dir  |= 0x0800;
		 	*P_IOA_Attrib |= 0x0800; 	
		    *P_IOA_Data  &= ~0x0800;    	
			start = getRTCinSeconds();
			stopTime = start + (int)(RECORD_TIME_MS / 1000);
			Snd_SACM_RecFAT(handle, C_CODEC_AUDIO1800, BIT_RATE);
			do {
				if ((key = keyCheck(0)))
					break;		
				end = getRTCinSeconds();	
			} while (end < stopTime); 
			SACM_Stop();		//Snd_Stop(); // no need to call stop() and flush the log
			close(handle);
			//turn on speaker driver SPK_CE
		    *P_IOA_Data  |= 0x0800;    	
			setLED(LED_RED,FALSE);
			playDing();
		}
		if (key || (key=keyCheck(1)))
			break;		
		handle = open((LPSTR)TEST_AUDIO_FILENAME,O_RDONLY);
		if (handle >= 0) { 
			SACMGet_A1800FAT_Mode(handle,0);
			Snd_SACM_PlayFAT(handle, C_CODEC_AUDIO1800);
			for (i=0; i < 4; i++) {
				adjustVolume(i*4+1,FALSE,FALSE);
				do {
					if ((key=keyCheck(0)))
						break;		
					lTime = Snd_A1800_GetCurrentTime();
				} while (lTime < (RECORD_TIME_MS-1250)); // not quite to the very end
				if (key || (key=keyCheck(1)))
					break;
				SACM_A1800FAT_SeekTime(lTime,BACKWARD_SKIP);
			}
			Snd_Stop(); 
			close(handle);		
		}	
	}
	close(handle);		// in case file was open at break
	//adjustSpeed(NORMAL_SPEED,FALSE);	
	adjustVolume(NORMAL_VOLUME,FALSE,FALSE);	
	return 0;
}

int keyTests(int keys) {
	int key;
			
	// test keypad input and RED LED
	// each button has to be pushed once to conclude test
	while (keys != 0x3ff) {
		key = 0;
		while (!key)
			key = keyCheck(0);
		playBip();
		flashRed();
		keys |= key;
	}
	return 0;
}

int receiveCopy(void) {
	int ret;
	
	setLED(LED_RED,TRUE);
	SystemIntoUDisk(1);	
	SD_Initial();  // recordings are bad after USB device connection without this line (todo: figure out why)
	// test microSD read and writes -- assumes microSD card is FAT formatted
	setLED(LED_RED,TRUE); // gets turned off within SystemIntoUDisk
	ret = readTestFile(); 
	return ret;
}

unsigned int getFileSize(void) {
	unsigned int ret = STARTING_FILE_SIZE;
	unsigned int key;
	
	if (!ret)
		ret = 1;
	playBips(ret);
	do {
		key = keyCheck(0);
		if (key == KEY_MINUS) {
			if (ret > 1)
				ret--;
			playBips(ret);
		} else if (key == KEY_PLUS) {
			if (ret < 7) 
				ret++;
			playBips(ret);
		}
	} while (key != KEY_RIGHT);
	ret = 128 << ret; // range = 256KB to 16MB  -- note that 128 << 9 would exceed unsigned int!
	return ret;
}

int sendCopy(void) {
	int ret;

	setLED(LED_RED,TRUE);
	ret = createTestFile(getFileSize());
	if (ret == -1)
		return -1;
	setUSBHost(TRUE);
	ret = unlink((LPSTR)REMOTE_TEST_FILENAME);
	setLED(LED_RED,TRUE);
	ret = fileCopy((char *)LOCAL_TEST_FILENAME,(char *)REMOTE_TEST_FILENAME);
	//ret = _copy((LPSTR)LOCAL_TEST_FILENAME,(LPSTR)REMOTE_TEST_FILENAME);
	setLED(LED_RED,FALSE);
	unlink((LPSTR)LOCAL_TEST_FILENAME);
	return ret;
}

int reprogram(void) {
	struct f_info fInfo;
	int ret;
	char filename[20];
	
//rhm	startUpdate("System.img");
// get name of current firmware image if possible
	ret =_findfirst((LPSTR)DEFAULT_SYSTEM_PATH UPDATE_FN, &fInfo, D_FILE);
	if (ret < 0)
		filename[0] = 0;
	else
		strcpy(filename,fInfo.f_name);
// see if there is an update firmware file		
	ret =_findfirst((LPSTR)UPDATE_FP UPDATE_FN, &fInfo, D_FILE);
	if (ret >= 0) {
		// if it's the same one that is current, try to find another one
		if (!strcmp(filename,fInfo.f_name))
			ret = _findnext(&fInfo);
		if (ret >= 0) {
			setLED(LED_GREEN,FALSE);
			startUpdate(fInfo.f_name);
		}
	}
	return -1; // shouldn't return; should reprogram and restart
}

int createTestFile(unsigned int kbSize) {
	long testLong[LONG_SIZE];
	long lCount, wCount;
	int handle, ret;
	unsigned int cycles;
	
	if (LONG_SIZE < 256) // to avoid div by zero or other problems with >> 8 
		cycles = kbSize / LONG_SIZE / 256;
	cycles = kbSize / (((unsigned int)LONG_SIZE) >> 8);

	for (lCount=0; lCount < LONG_SIZE; lCount++)
		testLong[lCount] = lCount; 
	//create new file
	handle = open((LPSTR)LOCAL_TEST_FILENAME,O_CREAT|O_TRUNC|O_WRONLY);
	for (lCount=0; lCount < cycles; lCount++) {
		wCount = write(handle,(unsigned long)&testLong<<1,LONG_SIZE<<2);	
		if (wCount == -1) {
			ret = -1;
			break;
		}
	}
	close(handle);
	return ret;
}

int readTestFile(void) {
	long testLong[LONG_SIZE];
	unsigned long lCount;
	int handle, ret;
	long bytesRead, bytesToRead, longsToCount;
	
	bytesToRead = LONG_SIZE<<2;
	longsToCount = LONG_SIZE;
	handle = open((LPSTR)LOCAL_TEST_FILENAME,O_RDONLY);
	if (handle < 0)
		ret = -1;
	else {
		ret = -1;
		do {
			bytesRead = read(handle,(unsigned long)&testLong<<1,bytesToRead);
			if (bytesRead)
				ret = 0; // requires at least some bytes in first read
			if (bytesRead != bytesToRead)
				longsToCount = bytesRead>>2;
			for (lCount=0; lCount < longsToCount; lCount++) {
				if (testLong[lCount] != lCount) {
					close(handle);
					return -1;
				}
			}
		} while (bytesRead == bytesToRead);
		close(handle);
	}
	// if test is a success then delete file
	if (!ret)
		unlink((LPSTR)LOCAL_TEST_FILENAME);
	return ret;
}

void exception(void) {	
	setLED(LED_RED,TRUE);
	playBips(6);	
	setLED(LED_RED,FALSE);
}

