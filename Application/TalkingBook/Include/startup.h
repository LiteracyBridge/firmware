// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__STARTUP_h__
#define	__STARTUP_h__

#define MAX_SYSTEMS			20
#define SYSTEM_HEAP_SIZE 300	//config file values
#define CONFIG_FILE		"a://system/config.txt"
#define CONFIG_BIN_FILE     "a://system/config.bin"
#define ALT_CONFIG_FILE		"a://config.txt"
#define FLAGGED_CONFIG_FILE	"a://system/config-VERSION-CHANGED.txt"

#define BOOT_TYPE_COLD_RESET 0
#define BOOT_TYPE_KEY_PRESS  1
#define BOOT_TYPE_RTC_ALARM  2

// set BAD_MEMBRANE_SW for the old membrane switch that A-Tech has
//#define OLD_MEMBRANE_SW 

extern void startUp(unsigned int);
extern void setDefaults(void);
extern char *nextSystem(void);
extern char *prevSystem(void); 
extern char *currentSystem(void);
extern unsigned int GetMemManufacturer(void);
extern void cleanUpOldRevs(void);
extern int loadSystemNames(void);

#ifndef OLD_MEMBRANE_SW 

//#ifdef TB_CAN_WAKE
// BELOW ARE THE GOOD KEY CODES FOR SCH 3.1+
#define V1_DEFAULT_KEY_DOWN 	0x0001
#define V1_DEFAULT_KEY_STAR 	0x0002
#define V1_DEFAULT_KEY_RIGHT 	0x0004
#define V1_DEFAULT_KEY_LEFT 	0x0008
#define V1_DEFAULT_KEY_UP 		0x0010
#define V1_DEFAULT_KEY_MINUS 	0x0020
#define V1_DEFAULT_KEY_PLUS 	0x0040
#define V1_DEFAULT_KEY_PLAY	0x0080
#define V1_DEFAULT_KEY_SELECT 	0x0100
#define V1_DEFAULT_KEY_HOME 	0x0200

//#else // FOR OLD HARDWARE (SCH 2.2)

#define V0_DEFAULT_KEY_DOWN 	0x0001
#define V0_DEFAULT_KEY_STAR 	0x0002
#define V0_DEFAULT_KEY_PLAY	0x0004
#define V0_DEFAULT_KEY_RIGHT 	0x0008
#define V0_DEFAULT_KEY_LEFT 	0x0010
#define V0_DEFAULT_KEY_UP 		0x0020
#define V0_DEFAULT_KEY_MINUS 	0x0040
#define V0_DEFAULT_KEY_PLUS 	0x0080
#define V0_DEFAULT_KEY_SELECT 	0x0100
#define V0_DEFAULT_KEY_HOME 	0x0200

//#endif

#else
//BELOW ARE THE KEYCODES FOR A-TECH's OLD MEMBRANE SWITCHES
#define DEFAULT_KEY_DOWN 	0x0001
#define DEFAULT_KEY_SELECT 	0x0002
#define DEFAULT_KEY_RIGHT 	0x0004
#define DEFAULT_KEY_LEFT 	0x0008
#define DEFAULT_KEY_UP 		0x0010
#define DEFAULT_KEY_HOME 	0x0020
#define DEFAULT_KEY_STAR 	0x0040
#define DEFAULT_KEY_PLAY	0x0080
#define DEFAULT_KEY_MINUS 	0x0100
#define DEFAULT_KEY_PLUS 	0x0200

#endif

#define DEFAULT_LED_GREEN	0x0040
#define DEFAULT_LED_RED		0x0200

#define MAX_BIT_RATE		32000

#define DEFAULT_CLOCK_RATE	48
#define DEFAULT_MIC_GAIN_NORMAL		0
#define DEFAULT_MIC_GAIN_HEADPHONE 	4
#define DEFAULT_BIT_RATE	MAX_BIT_RATE
#define DEFAULT_NORMAL_VOLUME	2
#define DEFAULT_NORMAL_SPEED	12
#define DEFAULT_MAX_VOLUME 		15
#define DEFAULT_MAX_SPEED		24
#define DEFAULT_VOLUME_INCREMENT	1
#define DEFAULT_SPEED_INCREMENT 	3
#define DEFAULT_INACTIVITY_SECONDS	60
#define DEFAULT_USB_CLIENT_POLL_INTERVAL	3
#define DEFAULT_V_FAST_VOLTAGE_DROP_TIME_SEC	60
#define DEFAULT_V_VOLTAGE_DROP_CHECK_INTERVAL 	3  // check every .03 volts
#define DEFAULT_DEFAULT_REWIND	1000

#define	DEFAULT_MAX_PWR_CYCLES_IN_LOG  100
	
#define DEFAULT_DEFAULT_TIME_PRECISION  128
#define DEFAULT_INSERT_SOUND_REWIND_MS  100
#define DEFAULT_SPEAK_SOUND_FILE_IDX  8	
#define DEFAULT_REC_PAUSED_FILE_IDX  9
#define DEFAULT_POST_REC_FILE_IDX  10
#define DEFAULT_PLEASE_WAIT_IDX  24
#define DEFAULT_NO_TRANSLATION_FILE_IDX  19
#define DEFAULT_POST_TRANSLATE_FILE_IDX  18
#define DEFAULT_OVERWRITE_WARNING_FILE_IDX  25				
#define DEFAULT_SUBJECT_NOW_LOCKED_IDX 26
#define DEFAULT_SUBJECT_NOW_UNLOCKED_IDX 27
#define DEFAULT_SORRY_LOCKED_SUBJECT_IDX	28
#define DEFAULT_NOT_YET_TRANSLATED_FILE_IDX  20
#define DEFAULT_PLS_RECORD_TRANSLATION_FILE_IDX  21
#define DEFAULT_PLS_WAIT_FILE_IDX  17
#define DEFAULT_NEW_RECORDING_FILE_IDX  22
#define DEFAULT_ORIG_RECORDING_FILE_IDX  23
#define DEFAULT_INACTIVITY_SOUND_FILE_IDX  11
	
#define DEFAULT_ERROR_SOUND_FILE_IDX  3	
#define DEFAULT_EMPTY_LIST_FILE_IDX  12
#define DEFAULT_DELETED_FILE_IDX  13
#define DEFAULT_PRE_COPY_FILE_IDX  14
#define DEFAULT_POST_COPY_FILE_IDX  15
#define DEFAULT_POST_PLAY_FILE_IDX  16				
#define DEFAULT_HYPERLINK_SOUND_FILE_IDX  0
#define DEFAULT_BLOCK_START_LEADER  1000
#define DEFAULT_BLOCK_END_LEADER  5000
#define DEFAULT_GREEN_LED_WHEN_PLAYING  1
#define DEFAULT_VOLTAGE_SAMPLE_FREQ_SEC 120
#define DEFAULT_DEBUG_MODE 1
#define DEFAULT_LOG_KEYS 0
#define DEFAULT_LONG_LIST_NAMES 1


#define CONFIG_BUFLEN strlen (SYSTEM_ORDER_FILE) + 1 + \
				strlen (SYSTEM_PATH) + 1 + \
				strlen (LANGUAGES_PATH) + 1 + \
				strlen (UI_SUBDIR) + 1 + \
				strlen (TOPICS_SUBDIR) + 1 + \
				strlen (USER_PATH) + 1 + \
				strlen (LISTS_PATH) + 1 + \
				strlen (INBOX_PATH) + 1 + \
				strlen (OUTBOX_PATH) + 1 + \
				strlen (NEW_PKG_SUBDIR) + 1 + \
				strlen (SYS_UPDATE_SUBDIR) + 1 + \
				strlen (LOG_FILE) + 1 + \
				strlen (LIST_MASTER) + 1 + \
				strlen (SYSTEM_VARIABLE_FILE) + 1 + \
				strlen (PKG_NUM_PREFIX) + 1 + \
				strlen (LIST_NUM_PREFIX) + 1 + \
				strlen (CUSTOM_PKG_PREFIX) + 1 + \
				strlen (AUDIO_FILE_EXT) + 1 + \
				strlen (USER_CONTROL_TEMPLATE) + 1 + \
				strlen (MACRO_FILE)

typedef struct config_bin {
				int KEY_PLAY;
				int KEY_LEFT;
				int KEY_RIGHT;
				int KEY_UP;
				int KEY_DOWN;
				int KEY_SELECT;
				int KEY_STAR;
				int KEY_HOME;
				int KEY_PLUS;
				int KEY_MINUS;			
				int LED_GREEN;
				int LED_RED;
				int MAX_SPEED;
				int NORMAL_SPEED;
				int MAX_VOLUME;
				int NORMAL_VOLUME;
				int SPEED_INCREMENT;
				int VOLUME_INCREMENT;
				int offset_SYSTEM_ORDER_FILE;
				int offset_SYSTEM_PATH;
				int offset_LANGUAGES_PATH;
				int offset_UI_SUBDIR;
				int offset_TOPICS_SUBDIR;
				int offset_USER_PATH;
				int offset_LISTS_PATH;
				int offset_INBOX_PATH;
				int offset_OUTBOX_PATH;
				int offset_NEW_PKG_SUBDIR;
				int offset_SYS_UPDATE_SUBDIR;
				int offset_LOG_FILE;
				int offset_LIST_MASTER;
				int MAX_PWR_CYCLES_IN_LOG;
				int offset_SYSTEM_VARIABLE_FILE;
				int offset_PKG_NUM_PREFIX;
				int offset_LIST_NUM_PREFIX;
				int offset_CUSTOM_PKG_PREFIX;				
				int offset_AUDIO_FILE_EXT;
				int DEFAULT_TIME_PRECISION;
				int DEFAULT_REWIND;
				int INSERT_SOUND_REWIND_MS;
				int SPEAK_SOUND_FILE_IDX;
				int REC_PAUSED_FILE_IDX;
				int POST_REC_FILE_IDX;
				int PLEASE_WAIT_IDX;
				int NO_TRANSLATION_FILE_IDX;
				int POST_TRANSLATE_FILE_IDX;
				int OVERWRITE_WARNING_FILE_IDX;
				int SUBJECT_NOW_LOCKED_IDX;
				int SUBJECT_NOW_UNLOCKED_IDX;
				int SORRY_LOCKED_SUBJECT_IDX;				
				int NOT_YET_TRANSLATED_FILE_IDX;
				int PLS_RECORD_TRANSLATION_FILE_IDX;
				int PLS_WAIT_FILE_IDX;
				int NEW_RECORDING_FILE_IDX;
				int ORIG_RECORDING_FILE_IDX;
				int INACTIVITY_SOUND_FILE_IDX;
				int ERROR_SOUND_FILE_IDX;
				int EMPTY_LIST_FILE_IDX;
				int DELETED_FILE_IDX;
				int PRE_COPY_FILE_IDX;
				int POST_COPY_FILE_IDX;
				int POST_PLAY_FILE_IDX;				
				int HYPERLINK_SOUND_FILE_IDX;
				int BLOCK_START_LEADER;
				int BLOCK_END_LEADER;
				int BIT_RATE;
				int GREEN_LED_WHEN_PLAYING;
				int INACTIVITY_SECONDS;
				int MIC_GAIN_NORMAL;
				int MIC_GAIN_HEADPHONE;
				int offset_USER_CONTROL_TEMPLATE;
				int offset_MACRO_FILE;
				int VOLTAGE_SAMPLE_FREQ_SEC;
				int USB_CLIENT_POLL_INTERVAL;
				int DEBUG_MODE;
				int LOG_KEYS;
				int CLOCK_RATE;
				int LONG_LIST_NAMES;
				int V_FAST_VOLTAGE_DROP_TIME_SEC;
				int V_VOLTAGE_DROP_CHECK_INTERVAL;
				int LONG_KEYPRESS_COUNTER;
				int len_string_buf;
} CONFIG_BIN;

#define DEFAULT_SYSTEM_ORDER_FILE "languages"
#define DEFAULT_SYSTEM_PATH       "a:/system/"
#define DEFAULT_LANGUAGES_PATH    "a:/languages/"
#define DEFAULT_UI_SUBDIR         ""
#define DEFAULT_TOPICS_SUBDIR     "cat/"
#define DEFAULT_USER_PATH         "a:/messages/audio/"
#define DEFAULT_LISTS_PATH        "a:/messages/lists/"
#define DEFAULT_INBOX_PATH        "a:/inbox/"
#define DEFAULT_OUTBOX_PATH       "a:/outbox/"
#define DEFAULT_NEW_PKG_SUBDIR    "messages/"
#define DEFAULT_SYS_UPDATE_SUBDIR "sys-updates/"
#define DEFAULT_LOG_FILE          "a:/system/log.txt"
#define DEFAULT_LIST_MASTER       "activeLists"
#define DEFAULT_SYSTEM_VARIABLE_FILE "a:/system/system-vars"
#define DEFAULT_PKG_NUM_PREFIX    "P"
#define DEFAULT_LIST_NUM_PREFIX   "L"
#define DEFAULT_CUSTOM_PKG_PREFIX "^"
#define DEFAULT_AUDIO_FILE_EXT    ".a18"
#define DEFAULT_USER_CONTROL_TEMPLATE ""  // "package-template"
#define DEFAULT_MACRO_FILE        ""  // "a:/system/macro.txt"

#endif

