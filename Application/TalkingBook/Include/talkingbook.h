#ifndef	__TALKINGBOOK_h__
#define	__TALKINGBOOK_h__

#define VERSION			"v1.33"

asm("APP_IRAM: .SECTION .IRAM");  // , .ADDR = 0x5000
#define APP_IRAM 		__attribute__((section(".APP_IRAM")))

#include "./Component/Include/FS/typedef.h"
#include "./Application/TalkingBook/Include/app_exception.h"
#include "util.h"

// DONT FORGET TO CHANGE #define DEVBOARD and /Driver/IOKey/IOKeyScan.asm file

// TODO LIST
//   * registry
//     -- handle universal content categories
//     -- handle user voice tag categories
//   * user recordings
//     -- more advanced: multiple choice creation without computer (voice prompts to create) 
//   * limit recording from filling memory card (use config setting for min free space)
//   * tag as private
//   * usb host mode
//     -- tie to 'share' option
//     -- primary scenario is another talkingbook device; 
//     -- secondary scenario is usb flash drive
//   * power saving
//     -- low power sleep mode when no activity
//     -- consider trying lower proc speed (and check if affects catching block start/end events)
//     -- limiting volume when voltage is dropping
//   * recorded user feedback
//   * detecting held buttons vs. tapped buttons (could be used with SHIFT-like modifier)
//   * write a working malloc/free

#define PKG_CONTROL_FILENAME	"control.txt"
#define QUIZ_DATA_FILENAME		"quiz.dat"
#define FORWARD_SKIP	0x0000
#define BACKWARD_SKIP	0x0001
#define DELIMITER		':'
#define MAX_HYPERLINK_STACK	5
#define AVG_SYMBOL_LENGTH	12
// TODO: malloc this -- it's huge waste of space to do it with an 2-dimensional array
#define READ_LENGTH			1024
#define FILE_LENGTH			40
#define PATH_LENGTH			(FILE_LENGTH * 2)
#define LOG_CARRYOVER_BYTES 512
#define TEXT_EVENT_LEFT		'<'
#define TEXT_EVENT_RIGHT	'>'
#define TEXT_EVENT_UP		'^'
#define TEXT_EVENT_DOWN		'v'
#define TEXT_EVENT_SELECT	'o'
#define TEXT_EVENT_HOME		'h'
#define TEXT_EVENT_PLAY		'p'
#define TEXT_EVENT_STAR		'*'
#define TEXT_EVENT_PLUS		'+'
#define TEXT_EVENT_MINUS	'-'
#define TEXT_EVENT_START	'!'
#define TEXT_EVENT_END		'$'
#define TEXT_EVENT_ORDER	"<>^vohp*+-_!$"

#define TEXT_ACTION_NOP				'~'
#define TEXT_ACTION_STOP			'.'
#define TEXT_ACTION_PAUSE			','
#define TEXT_ACTION_JUMP_BLOCK		'['
#define TEXT_ACTION_RETURN			')'
#define TEXT_ACTION_INSERT_SOUND	'I'
#define TEXT_ACTION_PLAY_PAUSE		'P'
#define TEXT_ACTION_RECORD			'E'
#define TEXT_ACTION_FWD				'F'
#define TEXT_ACTION_BACK			'B'
#define TEXT_ACTION_JUMP_TIME		'T'
#define TEXT_ACTION_CALL_BLOCK		'('
#define TEXT_ACTION_JUMP_PACKAGE	'<'
#define TEXT_ACTION_JUMP_LIST		'L'
#define TEXT_ACTION_VOLUME			'V'
#define TEXT_ACTION_SPEED			'S'
#define TEXT_ACTION_UP				'+'
#define TEXT_ACTION_DOWN			'-'
#define TEXT_ACTION_NORMAL			'N'
#define TEXT_ACTION_LED_RED			'R'
#define TEXT_ACTION_LED_GREEN		'G'
#define TEXT_ACTION_LED_ALL			'A'

// try to become a usb client after USB_CLIENT_POLL_INTERVAL seconds of inactivity
#define USB_CLIENT_POLL_INTERVAL       5

// initialize usb hardware & software to become a client but return without running usb serviceloop
#define USB_CLIENT_SETUP_ONLY          2
// run usb serviceloop continuously until usb is inactiove
#define USB_CLIENT_SVC_LOOP_CONTINUOUS 1
// run usb serviceloop[ once and return
#define USB_CLIENT_SVC_LOOP_ONCE       0

typedef enum EnumStartOrEnd EnumStartOrEnd;
enum EnumStartOrEnd {NONE, STARTING, ENDING, BOTH};


// These capitalized variables are set in the config file.
extern int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;	
extern int LED_GREEN, LED_RED, LED_ALL;
extern int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
extern int NORMAL_VOLUME, MAX_VOLUME, VOLUME_INCREMENT;
extern char *BOOT_PACKAGE, *SYSTEM_PATH, *USER_PATH, *LIST_PATH, *INBOX_PATH, *OUTBOX_PATH;
extern int MAX_PWR_CYCLES_IN_LOG;
extern char *SYSTEM_VARIABLE_FILE, *LOG_FILE;
extern char *LIST_MASTER;
extern char *PKG_NUM_PREFIX, *LIST_NUM_PREFIX, *CUSTOM_PKG_PREFIX,*QUIZ_PKG_PREFIX;
extern char *AUDIO_FILE_EXT;
extern char *CONTROL_EXT;
extern int DEFAULT_TIME_PRECISION;
extern int DEFAULT_REWIND;
extern int INSERT_SOUND_REWIND_MS;
extern int HYPERLINK_SOUND_FILE_IDX, DELETED_FILE_IDX, BEEP_SOUND_FILE_IDX, BIP_SOUND_FILE_IDX, SPEAK_SOUND_FILE_IDX, 
    INACTIVITY_SOUND_FILE_IDX, ERROR_SOUND_FILE_IDX, EMPTY_LIST_FILE_IDX;
extern int BLOCK_START_LEADER, BLOCK_END_LEADER;
extern long BIT_RATE;
extern int GREEN_LED_WHEN_PLAYING;
extern int INACTIVITY_SECONDS;
extern int MIC_GAIN_NORMAL, MIC_GAIN_HEADPHONE;
extern char *CONTROL_TEMPLATE;
extern char *MACRO_FILE;
extern int VOLTAGE_SAMPLE_FREQ_SEC;
extern int LOG_WARNINGS, LOG_KEYS;
#endif
