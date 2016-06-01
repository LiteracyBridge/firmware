#include <string.h>
#include <errno.h>
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./Application/TalkingBook/Include/talkingbook.h"
#include "./Application/TalkingBook/Include/files.h"
#include "./Application/TalkingBook/Include/device.h"
#include "./Application/TalkingBook/Include/audio.h"
#include "./Application/TalkingBook/Include/SD_reprog.h"
#include "./Application/TalkingBook/Include/startup.h"
#include "./Application/TalkingBook/Include/filestats.h"

#define NUM_STARTUP_DINGS	1

#define RECORD_TIME_MS		4000
#define SELF_TEST_AUDIO_MS  3000
#define LONG_SIZE			1024
#define STARTING_FILE_SIZE	2
#define TEST_FILENAME		"test.bin"	
#define LOCAL_TEST_FILENAME	 "a:/"TEST_FILENAME
#define REMOTE_TEST_FILENAME "b:/"TEST_FILENAME
#define TEST_AUDIO_FILENAME	 "a:/test.a18"

#define SELF_TEST_KEYPAD_TIMEOUT 15 // seconds to wait for a broken keypad

char *SelfTestNames[] = {
    "LEDs",
    "Nor Flash",
    "SD w/r",
    "USB Device",
    "Keys",
    "Audio",
    "All Tests"
};

void saveSelfTestStatus(SelfTestStep step, SelfTestResult result);
SelfTestResult selfTestLEDs();
SelfTestResult selfTestNORFlash();
SelfTestResult selfTestSD();
SelfTestResult selfTestUsbDevice();
SelfTestResult selfTestKeypad();
SelfTestResult selfTestAudio();
SelfTestResult selfTestEnd();

void deliverSelfTestResults(SelfTestStep failureStepIfAny, SelfTestResult param);


int testPCB(void);

int sendCopy(void);
int pullCopy(void); 
int receiveCopy(void);
int audioTestRecord(int recordTimeMs, int keyBreakAllowed);
int audioTestPlayback(int playTimeMs, int numSteps, int keyBreakAllowed);
int audioTests(void);
int keyTests(int);
int reprogram(void);
int createTestFile(unsigned int);
int readTestFile(void);
void exception(void);
int waitForKey(int mask, int ledColor);
static void flashRed(void);
static void flashGreen(void);
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

/**
 *  Logs one self test result.
 */
void logStep(char *msg, SelfTestStep step, SelfTestResult result) {
    char buffer[100];
    strcpy(buffer, msg);
    strcat(buffer, " '");
    strcat(buffer, (step <= SELF_TEST_STEP_END) ? SelfTestNames[step] : "All Tests");

    if (result == SELF_TEST_RESULT_SUCCESS) {
        strcat(buffer, "': Success");
    } else if (result == SELF_TEST_RESULT_NOTHING) {
        strcat(buffer, "'");
    } else if (result) {
        strcat(buffer, "': error ");
        longToStr(result, buffer+strlen(buffer));
    }
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);
}

/**
 * Prints the self-test history to the log.
 */
void logHistory() {
    char buffer [10];
    int ix=0;
    struct NORselfTestStatus *status = (struct NORselfTestStatus *)FindLastFlashStruct(NOR_STRUCT_ID_SELF_STATE_STATUS);
    logStringRTCOptional("Self test history:", ASAP, LOG_ALWAYS, 0);
    buffer[0] = '@';
    while (status) {
        longToStr(ix++, buffer+1);
        strcat(buffer, ":");
        logStep(buffer, status->step, status->result);
        status = (struct NORselfTestStatus *)FindNextFlashSameStruct(status);
    }
}

/**
 * Self test for Talking Book PCB. Implemented as a simple state machine, with
 * the current state kept in NOR flash.
 */
void selfTest() {
    SelfTestStep step = SELF_TEST_STEP_FIRST; // First test happens to be LED
    SelfTestResult result = SELF_TEST_RESULT_SUCCESS;

    keyCheck(1);    // to get rid of wake-up button press
    logHistory();

    adjustVolume(NORMAL_VOLUME,FALSE,FALSE);
    writeVersionToDisk(DEFAULT_SYSTEM_PATH);  // being in this function usually means that the PCB was just reprogrammed (with a new version)

    logStringRTCOptional("Starting self tests...", ASAP, LOG_ALWAYS, 0);

    setLED(LED_RED, FALSE);
    setLED(LED_GREEN, FALSE);

    // Run through any uncompleted steps, then deliver the results.
    while (step != SELF_TEST_PASSED && result == SELF_TEST_RESULT_SUCCESS) {
        logStep("Starting", step, SELF_TEST_RESULT_NOTHING);
        switch (step) {
            case SELF_TEST_STEP_NOR_FLASH:
                result = selfTestNORFlash();
                break;
            case SELF_TEST_STEP_SD_WRITE_READ:
                result = selfTestSD();
                break;
            case SELF_TEST_STEP_USB_DEVICE_MODE:
                result = selfTestUsbDevice();
                break;


            case SELF_TEST_STEP_LEDS:
                result = selfTestLEDs();
                break;
            case SELF_TEST_STEP_KEYPAD:
                result = selfTestKeypad();
                break;
            case SELF_TEST_STEP_AUDIO:
                result = selfTestAudio();
                break;

            default:
                // This shouldn't happen, but if it does, consider it a passing test, so we don't hang in an endless loop.
                logStringRTCOptional("Unexpected case in self test loop.", ASAP, LOG_ALWAYS, 0);
                result = SELF_TEST_RESULT_SUCCESS;
                break;
        }
        if (result == SELF_TEST_RESULT_SUCCESS) {
            step++;
            if (step >= SELF_TEST_STEP_END) {
                step = SELF_TEST_PASSED;
                selfTestEnd();
            }
        } else {
            // Some tests also need to record good results. They're responsible for those. Any failures recorded here.
            saveSelfTestStatus(step, result);
            logStep("Self test failed", step, result);
        }
    }
    deliverSelfTestResults(step, result);
}

void resetSelfTestStatus() {
    logStringRTCOptional("Rerunning self tests",ASAP,LOG_NORMAL,0);
    saveSelfTestStatus(SELF_TEST_STEP_FIRST, SELF_TEST_RESULT_SUCCESS);
}

void setSelfTestPassed() {
    saveSelfTestStatus(SELF_TEST_PASSED, SELF_TEST_RESULT_SUCCESS);
}

void saveSelfTestStatus(SelfTestStep step, SelfTestResult result) {
    struct NORselfTestStatus status = {NOR_STRUCT_ID_SELF_STATE_STATUS, step, result};
    AppendStructToFlash(&status);
}

/**
 * Lets the operator visually see that the LEDs are working.
 */
SelfTestResult selfTestLEDs() {
    int i;
    setLED(LED_GREEN, FALSE);
    for (i=0; i<4; i++) {
        setLED(LED_RED, TRUE);
        wait(800);
        setLED(LED_RED, FALSE);
        setLED(LED_GREEN, TRUE);
        wait(800);
        setLED(LED_GREEN, FALSE);
    }

    logStep("Completed", SELF_TEST_STEP_LEDS, SELF_TEST_RESULT_SUCCESS);

    return SELF_TEST_RESULT_SUCCESS;
}

/**
 * Verifies that we can write to and then read from NOR flash.
 */
SelfTestResult selfTestNORFlash() {
#ifdef SCAN_NOR_FLASH
    long addrNOR;      // An address in the NOR flash
    unsigned char fromNOR[20];
    unsigned char *pFromNOR;
    char buffer[60];
#endif
    struct NORselfTestStatus *st;
    SelfTestResult ret;
    // This test simply tries to write that it passed. If the write works, it did pass, and flash is correct. If the write
    // fails, flash is still correct because it won't say that the write worked.
    saveSelfTestStatus(SELF_TEST_STEP_NOR_FLASH, SELF_TEST_RESULT_SUCCESS);
    st = (struct NORselfTestStatus *)FindLastFlashStruct(NOR_STRUCT_ID_SELF_STATE_STATUS);
    ret = (st != NULL && st->step == SELF_TEST_STEP_NOR_FLASH && st->result == SELF_TEST_RESULT_SUCCESS) ? SELF_TEST_RESULT_SUCCESS
                                                                                                     : SELF_TEST_RESULT_FAILURE;
#ifdef SCAN_NOR_FLASH
    // Now we've read some bytes from NOR flash, how we determine if NOR is working? It is pretty tricky, but the fact that
    // this code is running does mean that NOR works pretty well. So we'll just trust that running code + write/read status
    // means working NOR flash.

    // Start at the base address of the NOR flash. fetch one byte.  turn on the least-significant 0-bit. repeat.
    addrNOR = BASEADDR;
    pFromNOR = fromNOR;
    do {
        *pFromNOR++ = *(unsigned char*) addrNOR;
        addrNOR = (addrNOR + 1) | addrNOR; // 0x30000 -> 0x30001 -> 0x30003 -> 0x30007 -> 0x3000f -> 0x3001f ... 0x7ffff
    } while (addrNOR < ENDADDR);
    *pFromNOR++ = *(unsigned char*)ENDADDR;
    unsignedCharsToHex(fromNOR, buffer, pFromNOR-fromNOR);  // 30000 - 3ffff is 16, 7ffff, affff
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);
#endif

    logStep("Completed", SELF_TEST_STEP_NOR_FLASH, ret);

    return ret;
}

/**
 * Verifies writing and reading the SD card.
 */
SelfTestResult selfTestSD() {
    SelfTestResult ret = SELF_TEST_RESULT_SUCCESS;
    int fileResult;

    fileResult = createTestFile(256);
    if (fileResult < 0) {
        ret = SELF_TEST_RESULT_SD_WRITE_FAILURE;
    } else {
        fileResult = readTestFile();
        if (fileResult < 0) {
            ret = SELF_TEST_RESULT_SD_READ_FAILURE;
        } else {
            logStep("Completed", SELF_TEST_STEP_SD_WRITE_READ, ret);
        }
    }

    return ret;
}

/**
 * Verifies that we can enter into USB Device mode.
 */
SelfTestResult selfTestUsbDevice() {
    int usbret;
    int ret = SELF_TEST_RESULT_FAILURE;

    SystemIntoUDisk(SYSTEM_UDISK_INITIALIZE); // SIUD(SETUP_ONLY) always returns 1
    usbret = SystemIntoUDisk(SYSTEM_UDISK_TRY_CLIENT_MODE);
    if (!usbret) { //USB connection was made
        SD_Initial();  // recordings are bad after USB device connection without this line (todo: figure out why)
        ret = SELF_TEST_RESULT_SUCCESS;
    }

    logStep("Completed", SELF_TEST_STEP_USB_DEVICE_MODE, ret);

    return ret;
}

/**
 * Verifies the keys in order PLUS, HOUSE, MINUS, TREE, CIRCLE, LEFT, POT, RIGHT, STAR, TABLE.
 */
SelfTestResult selfTestKeypad() {
    char buffer[40];
    int key;
    int plusCount=0;
    int keys = 0;
    int list[10] = {KEY_PLUS, KEY_HOME, KEY_MINUS, KEY_UP, KEY_SELECT, KEY_LEFT, KEY_PLAY, KEY_RIGHT, KEY_STAR, KEY_DOWN};
    int keyIx = 0;
    int endIx = sizeof(list) / sizeof(list[0]);
    long keyWaitStartTime;
    SelfTestResult ret = SELF_TEST_RESULT_SUCCESS;

    playBips(3);
    // test keypad input and RED LED
    // each button has to be pushed once to conclude test
    while (keyIx != endIx && ret == SELF_TEST_RESULT_SUCCESS) {
        key = 0;
        keyWaitStartTime = getRTCinSeconds();
        while (!key) {
            checkVoltage();
            key = keyCheck(0);
            if ((getRTCinSeconds() - keyWaitStartTime) > SELF_TEST_KEYPAD_TIMEOUT) {
                ret = SELF_TEST_RESULT_FAILURE;
                break;
            }
        }
        if (key == KEY_PLUS) {
            if (++plusCount == 5) {
                playBips(4);
                break;
            }
        } else {
            plusCount = 0;
        }
        // Log the key we got / expected.
        strcpy(buffer, "keyIx ");
        longToStr(keyIx, buffer+strlen(buffer));
        strcat(buffer, " :");
        longToHexString((long)key, buffer+strlen(buffer), 1);
        strcat(buffer, ", exp:");
        longToHexString((long)list[keyIx], buffer+strlen(buffer), 1);
        logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);

        if (key == list[keyIx]) {
            // The expected key; just blink the green light.
            flashGreen();
            keys |= key;
            keyIx++;
        } else  if (key & keys) {
            // A repeated key.
            playBip();
            flashRed();
        } else {
            // A missed key.
            playBips(2);
            flashRed();
        }

    }
    ret = (keyIx < endIx) ? SELF_TEST_RESULT_FAILURE : SELF_TEST_RESULT_SUCCESS;

    logStep("Completed", SELF_TEST_STEP_KEYPAD, ret);

    return ret;
}

/**
 * Records then plays some sound, at multiple levels. Does not actually listen to the playback, so it is up
 * to the operator to note that recording / playback failed.
 */
SelfTestResult selfTestAudio() {
    int ret;

    // Turn on red light, record for a 3-4 seconds.
    setLED(LED_RED, TRUE);
    playBip();
    ret = (audioTestRecord(SELF_TEST_AUDIO_MS, 0 /*no key break*/) == 0) ? SELF_TEST_RESULT_SUCCESS : SELF_TEST_RESULT_AUDIO_RECORD_FAILURE;
    logStep("Recording", SELF_TEST_STEP_AUDIO, ret);
    setLED(LED_RED, FALSE);
    // If we think we recorded, play it back.
    if (ret == SELF_TEST_RESULT_SUCCESS) {
        setLED(LED_GREEN, TRUE);
        playBips(2);
        ret = (audioTestPlayback(SELF_TEST_AUDIO_MS, 2 /*volume steps*/, 1 /*key break*/) == 0) ? SELF_TEST_RESULT_SUCCESS : SELF_TEST_RESULT_AUDIO_PLAYBACK_FAILURE;
        logStep("Playback", SELF_TEST_STEP_AUDIO, ret);
        setLED(LED_GREEN, FALSE);
    }
    // If we think it worked, beep.
    if (ret == SELF_TEST_RESULT_SUCCESS) {
        unlink((LPSTR)TEST_AUDIO_FILENAME);
    }

    return ret;
}

/**
 * Marks the self tests as having all passed.
 */
SelfTestResult selfTestEnd() {
    saveSelfTestStatus(SELF_TEST_PASSED, SELF_TEST_RESULT_SUCCESS);
    logStep("Completed", SELF_TEST_PASSED, SELF_TEST_RESULT_SUCCESS);
    return SELF_TEST_RESULT_SUCCESS;
}

/**
 * Delivers the results to the operator. Will blink flash the LED 1 second on, 1 second off.
 * If reporting success, use the green LED, if failure, use the RED.
 *
 * For failures, press the RIGHT key, to play a sequence of N beeps, where N represents
 * the failing test:
 * 1: NOR Flash
 * 2: SD write/read
 * 3: USB device mode
 * 4: Keyboard
 * 5: Audio
 *
 * Press the DOWN key to sleep, the HOME key to attempt to enter normal operation. Of course,
 * if the self test has failed, normal operation may not be possible.
 *
 * Press the UP key to create a notest.pcb file, to suppress future tests.
 *
 * failureStepIfAny - if the result is not success, what was the failure?
 * result - the success or failure being reported
 */
void deliverSelfTestResults(SelfTestStep failureStepIfAny, SelfTestResult result) {
    int led = (result == SELF_TEST_RESULT_SUCCESS) ? LED_GREEN : LED_RED;
    int key;
    int acceptedKeys = KEY_DOWN | KEY_HOME | KEY_UP;
    int announceKey = KEY_RIGHT;
    int ret;

    playBips(3);

    setLED(LED_ALL, FALSE);
    setLED(led, TRUE);
    if (result != SELF_TEST_RESULT_SUCCESS) {
        acceptedKeys |= announceKey;
    }
    // Wait for one of our "accepted" keys.
    do {
        key = waitForKey(acceptedKeys, led);
        // If the key was the right hand, announce the error, and wait again.
        if (key == announceKey) {
            playDings(failureStepIfAny);
        }
    } while ((key == announceKey));
    if (key == KEY_HOME) {
        // Home key -- attempt normal operation.
        return;
    } else if (key == KEY_UP) {
        // No more self tests.
        logStringRTCOptional("Creating notest.pcb to suppress future tests.", ASAP, LOG_ALWAYS, 0);
        ret = tbOpen((LPSTR)SUPPRESS_SELF_TEST_MARKER_FILE, O_CREAT|O_RDWR|O_TRUNC);
        close(ret);
    }
    setOperationalMode((int) P_SLEEP);  // sleep; wake from center, home, or black button
}

/**
 * Waits for a key, with a mask of acceptable keys. Flash the LED 1 second on, 1 second off.
 * Returns the key actually pressed.
 * If the voltage drops too low, this function will never return.
 *
 * mask - the keys to wait for. If any other key is pressed, plays a bip, but otherwise ignores the key.
 * ledColor - the LED color to flash.
 */
int waitForKey(int mask, int ledColor) {
    int ledIsOn = FALSE;
    long now = getRTCinSeconds();
    int key = 0;

    do {
        key = 0;
        while (!key) {
            checkVoltage();
            // If the second has changed...
            if (now != getRTCinSeconds()) {
                now = getRTCinSeconds();
                // ... toggle the LED...
                ledIsOn = !ledIsOn;
                setLED(ledColor, ledIsOn);
            }
            key = keyCheck(0);
        }
        if ((key & mask) == 0) {
            playBip();
        }
    } while ((key & mask) == 0);
    setLED(ledColor, TRUE);
    return key;
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
	writeVersionToDisk(DEFAULT_SYSTEM_PATH);  // being in this function usually means that the PCB was just reprogrammed (with a new version)
	key = keyCheck(1); 	// to get rid of wake-up button press

	//rhm
//	while (1) {
//		SystemIntoUDisk(USB_CLIENT_SVC_LOOP_CONTINUOUS);
//		playDing();
//		reprogram(); // this will not return
//	}
	//rhm
	keys = 0;	
	while (1) {
		checkVoltage();
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
		if (key == KEY_LEFT) {
			ret = receiveCopy();
		} else if (key == KEY_RIGHT) {
			ret = sendCopy();
			ret = pullCopy();
		} else if (key == KEY_STAR) {
			// this will not return
			ret = reprogram(); 
		} else if (key == KEY_UP) { // Tree	
			ret = keyTests(keys);   // 'keys' contains those keys already received in the test
		} else if (key == KEY_PLAY) { // Pot
			ret = audioTests();  
		} else if (key == KEY_DOWN) { // Table
			setOperationalMode((int)P_SLEEP);  // sleep; wake from center, home, or black button
		} else if (key == KEY_PLUS) {	
			adjustVolume(1,TRUE,FALSE);
			playBips(2);
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
	usbret = SystemIntoUDisk(USB_CLIENT_SETUP_ONLY);    // Always returns 1
	while(usbret == 1) {
		usbret = SystemIntoUDisk(USB_CLIENT_SVC_LOOP_WITH_TIMEOUT);
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

void flashGreen() {
    setLED(LED_GREEN, TRUE);
    wait(80);
    setLED(LED_GREEN, FALSE);
}

/**
 * Records test audio. Audio is saved to TEST_AUDIO_FILENAME (a:/test.a18).
 * If there is an error creating the file, returns the error (< 0).
 * If recording is interrupted by a key, returns the key (> 0).
 * If recording finishes normally, returns 0.
 */
int audioTestRecord(int recordTimeMs, int allowKeyBreak) {
    int handle;
    long stopTime;
    int key = 0;

    unlink((LPSTR)TEST_AUDIO_FILENAME);
    if (MIC_GAIN_NORMAL >= 0) {
        *P_HQADC_MIC_PGA &= 0xFFE0; // only first 5 bits for mic pre-gain; others reserved
        *P_HQADC_MIC_PGA |= MIC_GAIN_NORMAL;
    }
    handle = tbOpen((LPSTR)TEST_AUDIO_FILENAME, O_CREAT|O_RDWR);
    if (handle < 0) {
        return handle;
    }

    //turn off speaker driver SPK_CE
    *P_IOA_Dir  |= 0x0800;
    *P_IOA_Attrib |= 0x0800;
    *P_IOA_Data  &= ~0x0800;

    key = keyCheck(1); 	// to get rid of wake-up button press
    stopTime = getRTCinSeconds() + (recordTimeMs / 1000);
    Snd_SACM_RecFAT(handle, C_CODEC_AUDIO1800, BIT_RATE);
    do {
        checkVoltage();
        if (allowKeyBreak && (key = keyCheck(0)))
            break;
    } while (getRTCinSeconds() < stopTime);

    SACM_Stop();
    close(handle);

    //turn on speaker driver SPK_CE
    *P_IOA_Data  |= 0x0800;

    return key;
}

/**
 * Plays back the audio test file.
 * If there is an error opening the file, returns the error (< 0).
 * If playback is interrupted by a key, returns the key (> 0).
 * If playback finishes normally, returns 0.
 */
int audioTestPlayback(int playTimeMs, int numSteps, int allowKeyBreak) {
    int handle;
    int key = 0;
    int i;
    int volume;
    long currentTime;
    extern long Snd_A1800FAT_GetTotalTime(int);

    handle = tbOpen((LPSTR)TEST_AUDIO_FILENAME, O_RDONLY);
    if (handle < 0) {
        return handle;
    }

    keyCheck(1); 	// to get rid of wake-up button press
    SACMGet_A1800FAT_Mode(handle, 0);
    Snd_SACM_PlayFAT(handle, C_CODEC_AUDIO1800);
    for (i=0; i < numSteps; i++) {
        volume = MIN_VOLUME + (MAX_VOLUME - MIN_VOLUME - VOLUME_INCREMENT) * i / numSteps;
        adjustVolume(volume, FALSE, FALSE);
        do {
            checkVoltage();
            if (allowKeyBreak && (key=keyCheck(0)))
                break;
            currentTime = Snd_A1800_GetCurrentTime();
        } while (currentTime < (playTimeMs - 1250)); // not quite to the very end
        if (key)
            break;
        SACM_A1800FAT_SeekTime(currentTime, BACKWARD_SKIP);
    }
    Snd_Stop();
    close(handle);

    adjustVolume(NORMAL_VOLUME, FALSE, FALSE);

    return key;
}

int audioTests(void) {
    int ret = 0;

    while (ret == 0) {
        setLED(LED_RED, TRUE);
        playBip();
        ret = audioTestRecord(RECORD_TIME_MS, 1);
        setLED(LED_RED, FALSE);
        if (ret == 0) {
            playDing();
            ret = audioTestPlayback(RECORD_TIME_MS, 4, 1);
        }
    }
    return 0;
}

int keyTests(int keys) {
	char buffer[30];
	int key;
	int prevKey=-1;
	int numRepeat=0;
	keys = 0;
	playBips(3);		
	// test keypad input and RED LED
	// each button has to be pushed once to conclude test
	while (keys != 0x3ff) {
		key = 0;
		while (!key) {
			checkVoltage();
			key = keyCheck(0);
		}
		if (key == prevKey) {
			if (++numRepeat == 10) {
				playBips(4);
				return -1;
			}
		} else {
			prevKey = key;
			numRepeat = 0;
		}
		if (key & keys) {
			playBips(2);
		} else {
			playBip();
			flashRed();
		}
		keys |= key;
		strcpy(buffer, "key: ");
		longToHexString((long)key, buffer+5, 1);
		strcat(buffer, "/");
		longToHexString((long)keys, buffer+10, 1);
	    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);
	}
	playBips(3);
	return 0;
}

int receiveCopy(void) {
	int ret;
	
	setLED(LED_RED,TRUE);
	SystemIntoUDisk(USB_CLIENT_SVC_LOOP_CONTINUOUS);
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

    logStringRTCOptional("Attempting copy to b:", ASAP, LOG_ALWAYS, 0);
	setLED(LED_RED,TRUE);
	ret = createTestFile(getFileSize());
	if (ret == -1)
		return -1;
	setUSBHost(TRUE);
	ret = unlink((LPSTR)REMOTE_TEST_FILENAME);
	setLED(LED_RED,TRUE);
	ret = fileCopy((char *)LOCAL_TEST_FILENAME,(char *)REMOTE_TEST_FILENAME);
	if (ret == 0) {
        logStringRTCOptional("Successfully copied file to b:", ASAP, LOG_ALWAYS, 0);
	} else {
		logStringRTCOptional("Error copying file to b:", ASAP, LOG_ALWAYS, 0);
	}
	setLED(LED_RED,FALSE);
//	unlink((LPSTR)LOCAL_TEST_FILENAME);
    setUSBHost(FALSE);
	return ret;
}

int pullCopy(void) {
	int ret;
    struct f_info ffinfo;
    char buffer[20];
    char spec[5];
    char dr;
    
    strcpy(spec, "a:/*");
    for (dr='a'; dr<='b'; dr++) {
    	spec[0] = dr;
	    ret = _findfirst((LPSTR)spec, &ffinfo, D_ALL);
	    if (ret < 0) {
	    	strcpy(buffer, spec);
	    	strcat(buffer, ": ");
	    	longToStr((long)ret, buffer+strlen(buffer));
	    	logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);
	    } else {
	    	logStringRTCOptional(spec, ASAP, LOG_ALWAYS, 0);
	    	logStringRTCOptional(ffinfo.f_name, ASAP, LOG_ALWAYS, 0);
	    }
    }
    
    logStringRTCOptional("Attempting copy from b:", ASAP, LOG_ALWAYS, 0);
	setLED(LED_RED,FALSE);
	setLED(LED_GREEN,TRUE);
	setUSBHost(TRUE);
	ret = unlink((LPSTR)"a:/test.dat");
	setLED(LED_RED,TRUE);
	setLED(LED_GREEN,FALSE);
	ret = fileCopy("b:/test.dat","a:/test.dat");
	setLED(LED_RED,FALSE);
	if (ret == 0) {
        logStringRTCOptional("Successfully copied file from b:", ASAP, LOG_ALWAYS, 0);
	} else {
		logStringRTCOptional("Error copying file from b:", ASAP, LOG_ALWAYS, 0);
	}
	setUSBHost(FALSE);
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
	char buffer[30];
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
	handle = tbOpen((LPSTR)LOCAL_TEST_FILENAME,O_CREAT|O_TRUNC|O_WRONLY);
	for (lCount=0; lCount < cycles; lCount++) {
		wCount = write(handle,(unsigned long)&testLong<<1,LONG_SIZE<<2);	
		if (wCount == -1) {
			ret = -1;
			break;
		}
	}
	close(handle);
    strcpy(buffer, "Created file: ");
    strcat(buffer, LOCAL_TEST_FILENAME);
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);
	return ret;
}

int readTestFile(void) {
	long testLong[LONG_SIZE];
	unsigned long lCount;
	int handle, ret;
	long bytesRead, bytesToRead, longsToCount;
	
	bytesToRead = LONG_SIZE<<2;
	longsToCount = LONG_SIZE;
	handle = tbOpen((LPSTR)LOCAL_TEST_FILENAME,O_RDONLY);
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

