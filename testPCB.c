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

#define SELF_TEST_KEYPAD_TIMEOUT 20 // seconds to wait for a broken keypad


void saveSelfTestStatus(SelfTestStep step, SelfTestResult result);
SelfTestResult selfTestNORFlash();
SelfTestResult selfTestLEDs();
SelfTestResult selfTestKeypad();
SelfTestResult selfTestAudio();
SelfTestResult selfTestSD();
SelfTestResult selfTestEnd();

void deliverSelfTestResults(SelfTestStep status, SelfTestResult param);



int sendCopy(void);
int pullCopy(void); 
int receiveCopy(void);
int audioTestRecord(int recordTimeMs, int keyBreakAllowed);
int audioTestPlayback(int playTimeMs, int numSteps, int keyBreakAllowed);
int audioTests(void);
int keyTests(int);
void logPeriod();
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

void logStep(char *msg, SelfTestStep step, SelfTestResult result) {
    char buffer[100];
    strcpy(buffer, msg);
    longToStr(step, buffer+strlen(buffer));
    if (result) {
        strcat(buffer, ": ");
        longToStr(result, buffer+strlen(buffer));
    } else {
        strcat(buffer, ": success");
    }
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);
}
void logHistory() {
    char buffer [10];
    int ix=0;
    struct NORselfTestStatus *status = (struct NORselfTestStatus *)FindLastFlashStruct(NOR_STRUCT_ID_SELF_STATE_STATUS);
    buffer[0] = '@';
    while (status) {
        longToStr(ix++, buffer+1);
        strcat(buffer, ": ");
        logStep(buffer, status->step, status->result);
        status = (struct NORselfTestStatus *)FindNextFlashSameStruct(status);
    }
}

/**
 * Self test for Talking Book PCB. Implemented as a simple state machine, with
 * the current state kept in NOR flash.
 */
void selfTest() {
    void IOKey_Initial();
    struct NORselfTestStatus *status = (struct NORselfTestStatus *)FindLastFlashStruct(NOR_STRUCT_ID_SELF_STATE_STATUS);
    SelfTestStep step = status ? status->step : SELF_TEST_STEP_FIRST; // First test happens to be FLASH
    SelfTestResult result = status ? status->result : SELF_TEST_RESULT_SUCCESS;

    keyCheck(1);    // to get rid of wake-up button press
    logHistory();
    // Already passed. Nothing to do.
    if (step == SELF_TEST_PASSED) {
        return;
    }
    logStringRTCOptional("Starting self tests...", ASAP, LOG_ALWAYS, 0);

    setLED(LED_RED, FALSE);
    setLED(LED_GREEN, FALSE);

    // Run through any uncompleted steps, then deliver the results.
    while (step != SELF_TEST_PASSED && result == SELF_TEST_RESULT_SUCCESS) {
        logStep("Starting self test step ", step, 0);
        switch (step) {
            case SELF_TEST_STEP_FLASH:
                result = selfTestNORFlash();
                break;
            case SELF_TEST_STEP_SD_WRITE_READ:
                result = selfTestSD();
                break;
            case SELF_TEST_STEP_USB_HOST_MODE:
                // We can't currently do this, so just pass it.
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
            logStep("Self test failed in step ", step, result);
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
 * Verifies that we can write to and then read from NOR flash.
 */
SelfTestResult selfTestNORFlash() {
    char buffer[30];
    struct NORselfTestStatus *st;
    SelfTestResult ret;
    // This test simply tries to write that it passed. If the write works, it did pass, and flash is correct. If the write
    // fails, flash is still correct because it won't say that the write worked.
    saveSelfTestStatus(SELF_TEST_STEP_FLASH, SELF_TEST_RESULT_SUCCESS);
    st = (struct NORselfTestStatus *)FindLastFlashStruct(NOR_STRUCT_ID_SELF_STATE_STATUS);
    ret = (st != NULL && st->step == SELF_TEST_STEP_FLASH && st->result == SELF_TEST_RESULT_SUCCESS) ? SELF_TEST_RESULT_SUCCESS
                                                                                                     : SELF_TEST_RESULT_FAILURE;

    strcpy(buffer, "Self test NOR flash: ");
    strcat(buffer, (ret == SELF_TEST_RESULT_SUCCESS) ? "success" : "failure");
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);

    return ret;
}

/**
 * Lets the operator visually see that the LEDs are working.
 */
SelfTestResult selfTestLEDs() {
    int i;
    logStringRTCOptional("Self test LEDs", ASAP, LOG_ALWAYS, 0);
    setLED(LED_GREEN, FALSE);
    for (i=0; i<2; i++) {
        setLED(LED_RED, TRUE);
        wait(500);
        setLED(LED_RED, FALSE);
        setLED(LED_GREEN, TRUE);
        wait(500);
        setLED(LED_GREEN, FALSE);
    }

    return SELF_TEST_RESULT_SUCCESS;
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
    SelfTestResult ret;

    playBips(3);
    // test keypad input and RED LED
    // each button has to be pushed once to conclude test
    while (keyIx != endIx) {
        key = 0;
        keyWaitStartTime = getRTCinSeconds();
        while (!key) {
            checkVoltage();
            key = keyCheck(0);
            if ((getRTCinSeconds() - keyWaitStartTime) > SELF_TEST_KEYPAD_TIMEOUT) {
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

    strcpy(buffer, "Self test keypad: ");
    strcat(buffer, (ret == SELF_TEST_RESULT_SUCCESS) ? "success" : "failure");
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);

    return ret;
}

/**
 * Records then plays some sound, at multiple levels. The operator then presses a key to signify
 * success or failure.
 */
SelfTestResult selfTestAudio() {
    int ret;
    int key;

    logStringRTCOptional("Self test audio record", ASAP, LOG_ALWAYS, 0);
    // Turn on red light, record for a 3-4 seconds.
    setLED(LED_RED, TRUE);
    playBip();
    ret = (audioTestRecord(SELF_TEST_AUDIO_MS, 0 /*no key break*/) == 0) ? SELF_TEST_RESULT_SUCCESS : SELF_TEST_RESULT_AUDIO_RECORD_FAILURE;
    logStep("Self test audio record finished ", SELF_TEST_STEP_AUDIO, ret);
    setLED(LED_RED, FALSE);
    // If we think we recorded, play it back.
    if (ret == SELF_TEST_RESULT_SUCCESS) {
        setLED(LED_GREEN, TRUE);
        playBips(2);
        ret = (audioTestPlayback(SELF_TEST_AUDIO_MS, 2 /*volume steps*/, 1 /*key break*/) == 0) ? SELF_TEST_RESULT_SUCCESS : SELF_TEST_RESULT_AUDIO_PLAYBACK_FAILURE;
        logStep("Self test audio playback finished ", SELF_TEST_STEP_AUDIO, ret);
        setLED(LED_GREEN, FALSE);
    }
    // If we think it worked, beep.
    if (ret == SELF_TEST_RESULT_SUCCESS) {
        unlink((LPSTR)TEST_AUDIO_FILENAME);
        playBips(3);
    }

    return ret;
}

/**
 * Verifies writing and reading the SD card.
 */
SelfTestResult selfTestSD() {
    char buffer[35];
    SelfTestResult ret = SELF_TEST_RESULT_SUCCESS;
    int fileResult;

    strcpy(buffer, "Self test SD: ");
    fileResult = createTestFile(256);
    if (fileResult < 0) {
        ret = SELF_TEST_RESULT_SD_WRITE_FAILURE;
        strcat(buffer, "write failure");
    } else {
        fileResult = readTestFile();
        if (fileResult < 0) {
            ret = SELF_TEST_RESULT_SD_READ_FAILURE;
            strcat(buffer, "read failure");
        } else {
            strcat(buffer, "success");
        }
    }
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);

    return ret;
}

SelfTestResult selfTestEnd() {
    saveSelfTestStatus(SELF_TEST_PASSED, SELF_TEST_RESULT_SUCCESS);
    logStep("Self test passed ", SELF_TEST_PASSED, SELF_TEST_RESULT_SUCCESS);
    return SELF_TEST_RESULT_SUCCESS;
}

void deliverSelfTestResults(SelfTestStep step, SelfTestResult result) {
    int key;
    if (result == SELF_TEST_RESULT_SUCCESS) {
        // Success, so turn on the green light and wait for the table.
        setLED(LED_RED, FALSE);
        setLED(LED_GREEN, TRUE);
        waitForKey(KEY_DOWN);
    } else {
        // If failure, turn on the red light and wait for the table or the right hand. If table, announce error.
        setLED(LED_GREEN, FALSE);
        setLED(LED_RED, TRUE);
        do {
            key = waitForKey(KEY_DOWN | KEY_RIGHT | KEY_UP);
            if (key == KEY_DOWN) {
                // TODO: Speak error, not steps.
                playDings(step);
            } else if (key == KEY_UP) {
                // Never returns.
                logStringRTCOptional("Invoking classic testPCB.", ASAP, LOG_ALWAYS, 0);
                testPCB();
            }
        } while (key != KEY_RIGHT);
    }
    setLED(LED_ALL, FALSE);
    setOperationalMode((int)P_SLEEP);  // sleep; wake from center, home, or black button
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
	
	logPeriod();
	
	//rhm
//	while (1) {
//		SystemIntoUDisk(1);	
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
    char buffer[30];
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

    // If this works, it's the actual recording length.
    strcpy(buffer, "audio len: ");
    longToStr(playTimeMs, buffer + strlen(buffer));
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);

    keyCheck(1); 	// to get rid of wake-up button press
    SACMGet_A1800FAT_Mode(handle, 0);
    Snd_SACM_PlayFAT(handle, C_CODEC_AUDIO1800);
    for (i=0; i < numSteps; i++) {
        volume = MIN_VOLUME + (MAX_VOLUME - MIN_VOLUME) * i / numSteps;
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

/**
 * Waits for a key, with a mask of acceptable keys.
 * Returns the key actually pressed.
 * If the voltage drops too low, this function will never return.
 */
int waitForKey(int mask) {
    long now = getRTCinSeconds();
    int key = 0;

    do {
        key = 0;
        while (!key) {
            checkVoltage();
            if (now != getRTCinSeconds()) {
                // If the second has changed, play a bit. 
                playBip();
                now = getRTCinSeconds();
            }
            key = keyCheck(0);
        }
        if ((key & mask) == 0) {
            playBip();
            flashRed();
        }
    } while ((key & mask) == 0);
    return key;
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

void logPeriod() {
	char buffer[30];
	void *ptr;
	struct  NORperiod *period = (struct  NORperiod *)FindLastFlashStruct(NOR_STRUCT_ID_PERIOD);	
    strcpy(buffer, "period: ");
    unsignedlongToHexString((long)period, buffer+strlen(buffer));
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);
    
    ptr = (void *)(TB_SERIAL_NUMBER_ADDR + FindFirstFlashOffset());
    strcpy(buffer, "available: ");
    unsignedlongToHexString((unsigned long)ptr, buffer+strlen(buffer));
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);
    
    ptr = (void *)logPeriod;
    strcpy(buffer, "fn: ");
    unsignedlongToHexString((unsigned long)ptr, buffer+strlen(buffer));
    logStringRTCOptional(buffer, ASAP, LOG_ALWAYS, 0);
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

