// Copyright 2009-2011 Literacy Bridge
// Contact: info@literacybridge.org
#ifndef	__TALKINGBOOK_h__
#define	__TALKINGBOOK_h__

#define MAJOR_VERSION	"v1.70"
#define SVN_REVISION	"r860"
#define VERSION			MAJOR_VERSION SVN_REVISION
#define TB_CAN_WAKE  // hardware that allows wake from sleep/halt
//IMPORTANT: changes to TB_CAN_WAKE must also be made to system_head.inc!!!

asm("APP_IRAM: .SECTION .IRAM");  // , .ADDR = 0x5000
#define APP_IRAM 		__attribute__((section(".APP_IRAM")))

#include "./Component/Include/FS/typedef.h"
#include "./Application/TalkingBook/Include/app_exception.h"
#include "util.h"

// TODO LIST
//	 * BUG: shutdown to HALT sometimes doesn't wake up with IOB buttons - seems to now be limited to after USB activity
//	 * BUG: after 2 non-.old firmwares are in /firmware dir, new one will not work.
//   * reset RTC with specially named file
//	 * read and write new metadata scheme into audio files
//	 * take a recording and make a translation on the device (keeping it linked) 
//	 * copy entire category
//	 * track playcount and copy count of each package
//   * auto update VERSION with each new build (or SVN revision)
//   * recognize when batteries have been changed (not waking from sleep/halt) -- reset clock and whatever else.
//   * create an easy way to set a unique device id (group/unit id combo)
//	 * store device id and all count info in NOR flash not memory card
//   * create mechanism for one Talking Book to collect stats of others it meets
//	 * write fct to read decimal and hex numbers for debugging purposes (and for speaking version #, etc)
//   * add audio feedback to fwd/back time jumps
//	 * On Startup, if no microSD card or if blank (no config.txt) then run full system test/diagnostic with reprog abiliy`
//	 * Don't crash if 0KB audio file.  Maybe return -1 for filehandle if filesize=0.
//   * ADC/DAC modem-like capability for xfer of control tracks and compressed audio over mobile phones
//   * create new personal category (becomes subcat of OTHER)
//   * configure device to include selected categories (or create a recording in an unlisted category)
//   * refactor languages across system, lists, and user
//   * registry/categories
//     -- rethink category/subcategory case distinction
//     -- handle universal content SUB-categories
//     -- handle user voice tag categories
//   * hold down button to play recording at 2x (slow scan)
//   * change louder/quieter to a count(?)
//   * start macro from a package
//	 * tour guide audio within macro (or just include that within a package that is a macro)
//   * allow pause before block begins (e.g. A$:3,[newblock] for a 3-sec delay) -- use for help instructions if user delays action
//   * user recordings
//     -- more advanced: multiple choice creation without computer (voice prompts to create) 
//   * limit recording from filling memory card (use config setting for min free space)
//   * tag as private (not to be d2d copied)
//   * madlibs app
//   * secondary scenario is Talking Book updating from Outbox
//   * power saving
//     -- low power sleep mode when no activity
//     -- limiting volume when voltage is dropping
//   * recorded user feedback
//   * detecting held buttons vs. tapped buttons (could be used with SHIFT-like modifier)
//   * write a working malloc/free
//   * rewrite containers so that DEFAULT package doesn't alloc same # of files/blocks/actions as others
#define MAX_CLOCK_SPEED			96
#define DEFAULT_SYSTEM_PATH	"a:/system/"
#define PKG_CONTROL_FILENAME_BIN	"control.bin"
#define PKG_CONTROL_FILENAME_TXT	"control.txt"
#define TRANSLATE_FILENAME_BIN      "translate.bin"
#define APP_DATA_FILENAME		"data.bin"
#define DELETE_QUEUE_FILENAME	"deleteQueue.txt"
#define SYS_PKG_CHAR		'%'
#define SYS_MSG_CHAR		'$'
#define APP_PKG_CHAR		'^'
#define CATEGORY_DELIM		'#'
#define CATEGORY_DELIM_STR	"#"
#define FORWARD_SKIP	0x0000
#define BACKWARD_SKIP	0x0001
#define DELIMITER		':'
#define MAX_HYPERLINK_STACK	5
#define AVG_SYMBOL_LENGTH	20
// TODO: malloc this -- it's huge waste of space to do it with an 2-dimensional array
#define READ_LENGTH			1024
#define FILE_LENGTH			40
#define PATH_LENGTH			(FILE_LENGTH * 2)
#define LONG_FILE_LENGTH	256
#define LONG_PATH_LENGTH	(LONG_FILE_LENGTH * 2)
#define LOG_CARRYOVER_BYTES 8096
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

#define TEXT_ACTION_NOP				'~'
#define TEXT_ACTION_STOP			'.'
#define TEXT_ACTION_PAUSE			','
#define TEXT_ACTION_JUMP_BLOCK		'['
#define TEXT_ACTION_RETURN			')'
#define TEXT_ACTION_INSERT_SOUND	'I'
#define TEXT_ACTION_PLAY_PAUSE		'P'
#define TEXT_ACTION_RECORD			'E' // need to fix these two
#define TEXT_ACTION_TRIM			'M' // need to fix these two
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

// These definitions are used with rotating system langauges. 
// They have to be < 0 since >0 is used as a pointer to a text heap for named system packages
#define SAME_SYSTEM	-1
#define	PREV_SYSTEM -2
#define NEXT_SYSTEM -3

typedef enum EnumStartOrEnd EnumStartOrEnd;
enum EnumStartOrEnd {NONE, STARTING, ENDING, BOTH};


// These capitalized variables are set in the config file.
extern int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;	
extern int ADMIN_COMBO_KEYS;
extern int LED_GREEN, LED_RED, LED_ALL;
extern int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
extern int NORMAL_VOLUME, MAX_VOLUME, VOLUME_INCREMENT;
extern char *PROFILE_ORDER_FILE, *SYSTEM_PATH, *LANGUAGES_PATH, *UI_SUBDIR, *TOPICS_SUBDIR, *USER_PATH, *LISTS_PATH;
extern char *INBOX_PATH, *OUTBOX_PATH, *NEW_PKG_SUBDIR, *SYS_UPDATE_SUBDIR;
extern int MAX_PWR_CYCLES_IN_LOG;
extern char *SYSTEM_VARIABLE_FILE, *LOG_FILE;
extern char *LIST_MASTER;
extern char *PKG_NUM_PREFIX, *LIST_NUM_PREFIX, *CUSTOM_PKG_PREFIX;
extern char *AUDIO_FILE_EXT;
extern int DEFAULT_TIME_PRECISION;
extern int DEFAULT_REWIND;
extern int INSERT_SOUND_REWIND_MS;
extern int HYPERLINK_SOUND_FILE_IDX, DELETED_FILE_IDX, PRE_COPY_FILE_IDX, POST_COPY_FILE_IDX, SPEAK_SOUND_FILE_IDX, 
    INACTIVITY_SOUND_FILE_IDX, ERROR_SOUND_FILE_IDX, EMPTY_LIST_FILE_IDX, REC_PAUSED_FILE_IDX, POST_REC_FILE_IDX, PLEASE_WAIT_IDX, POST_TRANSLATE_FILE_IDX, NO_TRANSLATION_FILE_IDX,
    NOT_YET_TRANSLATED_FILE_IDX,PLS_RECORD_TRANSLATION_FILE_IDX,NEW_RECORDING_FILE_IDX,ORIG_RECORDING_FILE_IDX,PLS_WAIT_FILE_IDX,OVERWRITE_WARNING_FILE_IDX,
    SUBJECT_NOW_LOCKED_IDX,SUBJECT_NOW_UNLOCKED_IDX,SORRY_LOCKED_SUBJECT_IDX,
    POST_PLAY_FILE_IDX;
extern int BLOCK_START_LEADER, BLOCK_END_LEADER;
extern long BIT_RATE;
extern int GREEN_LED_WHEN_PLAYING, LONG_LIST_NAMES;
extern int INACTIVITY_SECONDS;
extern int MIC_GAIN_NORMAL, MIC_GAIN_HEADPHONE;
extern char *USER_CONTROL_TEMPLATE;
extern char *MACRO_FILE;
extern int VOLTAGE_SAMPLE_FREQ_SEC, USB_CLIENT_POLL_INTERVAL;
extern int DEBUG_MODE, LOG_KEYS;
extern unsigned int V_FAST_VOLTAGE_DROP_TIME_SEC,V_VOLTAGE_DROP_CHECK_INTERVAL;
#endif
