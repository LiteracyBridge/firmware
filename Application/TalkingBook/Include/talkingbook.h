#ifndef	__TALKINGBOOK_h__
#define	__TALKINGBOOK_h__

#define VERSION			"v1.35"

asm("APP_IRAM: .SECTION .IRAM");  // , .ADDR = 0x5000
#define APP_IRAM 		__attribute__((section(".APP_IRAM")))

#include "./Component/Include/FS/typedef.h"
#include "./Application/TalkingBook/Include/app_exception.h"
#include "util.h"

// DONT FORGET TO CHANGE #define DEVBOARD and /Driver/IOKey/IOKeyScan.asm file

// TODO LIST
//   * have ProcessInbox() handle copying new files (or new versions of existing files) to \system dir
//     This could be done by using a variant of copyCWD to copy in all new files for /system/* or /config.txt
//     This would also substitute for the current one-off check of System.bin by adding a folder /system/Firmware/Update/System.bin
//   * registry/categories
//     -- allow categories to be numbers, which means rethinking category/subcategory case distinction
//     -- handle universal content SUB-categories
//     -- handle user voice tag categories
//   * user recordings
//     -- more advanced: multiple choice creation without computer (voice prompts to create) 
//   * limit recording from filling memory card (use config setting for min free space)
//   * tag as private
//   * usb host mode
//     -- secondary scenario is Talking Book updating from Outbox
//   * power saving
//     -- low power sleep mode when no activity
//     -- consider trying lower proc speed (and check if affects catching block start/end events)
//     -- limiting volume when voltage is dropping
//   * recorded user feedback
//   * detecting held buttons vs. tapped buttons (could be used with SHIFT-like modifier)
//   * write a working malloc/free
//   * rewrite containers so that DEFAULT package doesn't alloc same # of files/blocks/actions as others
//   * allow pause before block begins (e.g. A$:3,[newblock] for a 3-sec delay)
#define PKG_CONTROL_FILENAME	"control.txt"
#define APP_DATA_FILENAME		"data.bin"
#define SYS_PKG_CHAR		'%'
#define APP_PKG_CHAR		'^'
#define CATEGORY_DELIM		'#'
#define CATEGORY_DELIM_STR	"#"
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
// moved to config.txt: #define USB_CLIENT_POLL_INTERVAL       5

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
extern char *BOOT_PACKAGE, *SYSTEM_PATH, *USER_PATH, *LIST_PATH, *INBOX_PATH, *UPDATE_PATH, *FIRMWARE_PATH, *OUTBOX_PATH;
extern int MAX_PWR_CYCLES_IN_LOG;
extern char *SYSTEM_VARIABLE_FILE, *LOG_FILE;
extern char *LIST_MASTER;
extern char *PKG_NUM_PREFIX, *LIST_NUM_PREFIX, *CUSTOM_PKG_PREFIX;
extern char *AUDIO_FILE_EXT;
extern int DEFAULT_TIME_PRECISION;
extern int DEFAULT_REWIND;
extern int INSERT_SOUND_REWIND_MS;
extern int HYPERLINK_SOUND_FILE_IDX, DELETED_FILE_IDX, PRE_COPY_FILE_IDX, POST_COPY_FILE_IDX, BEEP_SOUND_FILE_IDX, BIP_SOUND_FILE_IDX, SPEAK_SOUND_FILE_IDX, 
    INACTIVITY_SOUND_FILE_IDX, ERROR_SOUND_FILE_IDX, EMPTY_LIST_FILE_IDX;
extern int BLOCK_START_LEADER, BLOCK_END_LEADER;
extern long BIT_RATE;
extern int GREEN_LED_WHEN_PLAYING;
extern int INACTIVITY_SECONDS;
extern int MIC_GAIN_NORMAL, MIC_GAIN_HEADPHONE;
extern char *CONTROL_TEMPLATE;
extern char *MACRO_FILE;
extern int VOLTAGE_SAMPLE_FREQ_SEC, USB_CLIENT_POLL_INTERVAL;
extern int LOG_WARNINGS, LOG_KEYS;
#endif
