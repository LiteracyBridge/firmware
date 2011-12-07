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
extern void loadSystemNames(void);

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
#endif

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
				
#define SAVE_CONFIG_STRINGS (buf) \
	{ \
		SAVE_CONFIG_STRING(SYSTEM_ORDER_FILE); \
		SAVE_CONFIG_STRING(SYSTEM_PATH); \
		SAVE_CONFIG_STRING(LANGUAGES_PATH); \
		SAVE_CONFIG_STRING(UI_SUBDIR); \
		SAVE_CONFIG_STRING(TOPICS_SUBDIR); \
		SAVE_CONFIG_STRING(TOPICS_SUBDIR); \
		SAVE_CONFIG_STRING(USER_PATH); \
		SAVE_CONFIG_STRING(LISTS_PATH); \
		SAVE_CONFIG_STRING(INBOX_PATH); \
		SAVE_CONFIG_STRING(OUTBOX_PATH); \
		SAVE_CONFIG_STRING(NEW_PKG_SUBDIR); \
		SAVE_CONFIG_STRING(SYS_UPDATE_SUBDIR); \
		SAVE_CONFIG_STRING(LOG_FILE); \
		SAVE_CONFIG_STRINGstrlen(LIST_MASTER); \
		SAVE_CONFIG_STRINGstrlen(SYSTEM_VARIABLE_FILE); \
		SAVE_CONFIG_STRING(PKG_NUM_PREFIX); \
		SAVE_CONFIG_STRING(LIST_NUM_PREFIX); \
		SAVE_CONFIG_STRING(CUSTOM_PKG_PREFIX); \
		SAVE_CONFIG_STRING(AUDIO_FILE_EXT); \
		SAVE_CONFIG_STRING(USER_CONTROL_TEMPLATE); \
		SAVE_CONFIG_STRING(MACRO_FILE); \
	}

#define RESTORE_CONFIG_STRING (string) \
	SYSTEM_ORDER_FILE = addTextToSystemHeap(buf); \
	   offset += strlen(SYSTEM_ORDER_FILE) + 1
	
#define RESTORE_CONFIG_STRINGS (buf + offset) \
	{ \
 	int offset = 0; \
	RESTORE_CONFIG_STRINGS(SYSTEM_ORDER_FILE); \
	RESTORE_CONFIG_STRINGS(SYSTEM_PATH); \
	RESTORE_CONFIG_STRINGS(LANGUAGES_PATH; \
	RESTORE_CONFIG_STRINGS(UI_SUBDIR); \
	RESTORE_CONFIG_STRINGS(TOPICS_SUBDIR); \
	RESTORE_CONFIG_STRINGS(USER_PATH); \
	RESTORE_CONFIG_STRINGS(LISTS_PATH); \
	RESTORE_CONFIG_STRINGS(INBOX_PATH); \
	RESTORE_CONFIG_STRINGS(OUNEW_PKG_SUBDIR); \
	RESTORE_CONFIG_STRINGS(SYS_UPDATE_SUBDIR); \
	RESTORE_CONFIG_STRINGS(LOG_FILE); \
	RESTORE_CONFIG_STRINGS(LIST_MASTER); \
	RESTORE_CONFIG_STRINGS(SYSTEM_VARIABLE_FILE); \
	RESTORE_CONFIG_STRINGS(PKG_NUM_PREFIX); \
	RESTORE_CONFIG_STRINGS(LIST_NUM_PREFIX); \
	RESTORE_CONFIG_STRINGS(CUSTOM_PKG_PREFIX); \
	RESTORE_CONFIG_STRINGS(AUDIO_FILE_EXT); \
	RESTORE_CONFIG_STRINGS(USER_CONTROL_TEMPLATE); \
	RESTORE_CONFIG_STRINGS(MACRO_FILE); \
	}

