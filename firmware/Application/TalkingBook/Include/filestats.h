#ifndef	__FILESTATS_h__
#define	__FILESTATS_h__

#include "containers.h"

#define STAT_OPEN       0
#define STAT_CLOSE      1
#define STAT_TIMEPLAYED 2
#define STAT_COPIED     3
#define STAT_SURVEY1	4
#define STAT_APPLY		5
#define STAT_USELESS	6

#define STAT_DIR  "a:/statistics/" 
#define SYS_DATA_STATS_FILE		"flashData.bin"
#define SYS_DATA_STATS_PATH		STAT_DIR SYS_DATA_STATS_FILE
#define SYS_DATA_STATS_PATH_DEBUG_PRE		STAT_DIR "dump-preflash.bin"
#define SYS_DATA_STATS_PATH_DEBUG_POST		STAT_DIR "dump-postflash.bin"
#define CLI_STAT_DIR "b:/statistics/" 
#define CLI_OSTAT_DIR "b:/statistics/ostats/"
#define REFLASH_STATS_FILE_NAME				"sysdata.txt"
#define REFLASH_STATS_FILE					((LPSTR)"a:/" REFLASH_STATS_FILE_NAME)
#define REFLASH_STATS_FILE_ARCHIVE 	((LPSTR) DEFAULT_SYSTEM_PATH REFLASH_STATS_FILE_NAME)
#define SNCSV "SN.csv"
#define SRN_MAX_LENGTH	12  // this should match the define in sys_counters.h

void recordStats(char *filename, unsigned long handle, unsigned int why, unsigned long data);

#define MAX_MESSAGE_ID_LENGTH	20
#define MAX_TRACKED_MESSAGES		40
#define MAX_ROTATIONS					5

extern void write_app_flash(int *, int, int);

struct ondisk_filestats {
	unsigned long version;
	char tbSRN[SRN_MAX_LENGTH];
	char msgId[MAX_MESSAGE_ID_LENGTH];
	unsigned long stat_num_opens;
	unsigned long stat_num_completions;
	unsigned long stat_num_copies;
	unsigned long stat_num_survey1;
	unsigned long stat_num_apply;
	unsigned long stat_num_useless;
};

// Rebuild:UPDATE
#define NOR_STRUCT_ID_MSG_MAP	1
// 40 msgs x 20 chars + 2 = 802 words or 0x322 words  
struct NORmsgMap {
	char structType;
	char totalMessages;
	char msgIdMap[MAX_TRACKED_MESSAGES][MAX_MESSAGE_ID_LENGTH]; 	// map of id number for flash storage to full ID of audio messages 
};

// Not currently used; do not use.
// #define NOR_STRUCT_ID_UNUSED		2

// Not currently used; do not use.
// #define NOR_STRUCT_ID_UNUSED		3

// Rebuild:MAP
#define NOR_STRUCT_ID_NEW_MSG	4
// each of these events is 11 words
struct NORnewMsg {
	char structType;
	char indexMsg;
	char idMsg[MAX_MESSAGE_ID_LENGTH];
};

// Rebuild:KEEP-LAST
#define NOR_STRUCT_ID_PERIOD	5
struct NORperiod {
	char structType;
	char period;
};

// Rebuild:KEEP-LAST
#define NOR_STRUCT_ID_CUMULATIVE_DAYS	6
struct NORcumulativeDays {
	char structType;
	char cumulativeDaysSinceUpdate;
};

// Rebuild:KEEP-LAST
// 2 words 
#define NOR_STRUCT_ID_POWERUPS	7
struct NORpowerups {
	char structType;
	unsigned int powerups;
	int initVoltage;
};

// Rebuild:GROUP (for now - could save 25 words)
// 1 word x max of 50 days = 50 words
#define NOR_STRUCT_ID_CORRUPTION	8
struct NORcorruption {
	char structType;
	char daysAfterLastUpdate;
};

// Rebuild:GROUP
// 2 words x 5 = 10 words  // keep these -- don't 
#define NOR_STRUCT_ID_ROTATION		9
struct NORrotation {
	char structType;
	char rotationNumber;
	char periodNumber;
	unsigned int hoursAfterLastUpdate;
	int initVoltage;
};

// In file sys_counters.h:
// #define NOR_STRUCT_ID_COUNTS	10

// Not currently used; do not use.
// #define NOR_STRUCT_ID_UNUSED		11

// Rebuild:MAP
#define NOR_STRUCT_ID_STAT_EVENT	12
// 2 word
struct NORstatEvent {
	char structType;
	char indexMsg;	// array index for message
	char statType; // 0:10sec,1:25%,2:50%,3:75%,4:100%,5:survey:applied,6:survey useless
	char rotation;
	char profile;
	unsigned int secondsOfPlay;
};

// Rebuild:UPDATE
#define NOR_STRUCT_ID_MESSAGE_STATS	13
// 10 words x 20 msgs x 5 rotations = 1000 words
struct NORmsgStats {
	char structType;	// used to identify data structure
	char indexMsg;
	char numberProfile;
	char numberRotation;
	char countStarted;
	char countQuarter;
	char countHalf;
	char countThreequarters;
	char countCompleted;
	char countApplied;
	char countUseless;
	unsigned int totalSecondsPlayed;
};

// If we have 40 messages at 5 words each, that's 200 words out of 4096 words.
// But we also need a map for the 40 messages.  At 4 words per id, that's a 160-word array.
// 200+160=360 words, + 36 words for serial number and other system data, would be 396 words.
// leaving 3700 words for updates in a day
// With 40 messages, that would be 90 words per msg, at 2 words per update, that would be 45 updates per msg

// with current text ids for msgs having a max of 20 characters, 10 words, means a 400-word array for the map
// 200+400=600 words + 36 = 696, leaving 3400 words, or 85 words per msg, or 42 updates per msg

#define NOR_STRUCT_ID_ALL_MSGS	14
struct NORallMsgStats {
	char structType;
	char profileOrder;
	char profileName[MAX_PROFILE_NAME_LENGTH];
	char totalMessages;
	char totalRotations;
	struct NORmsgStats stats[MAX_TRACKED_MESSAGES][MAX_ROTATIONS];
};

struct NORallMsgStatsAllProfiles {
	struct NORallMsgStats profileStats[MAX_PROFILES];
};

// Rebuild:KEEP-LAST
// 1 word
// Used to keep track of self-test progress and pass/fail status. Keeps track of the farthest step completed,
// and the result of that step (0=pass, non-0=fail).
#define NOR_STRUCT_ID_SELF_STATE_STATUS 15
struct NORselfTestStatus {
    char strucType;
    char step;      // SelfTestStep
    char result;    // SelfTestResult
};
typedef enum {
    // this is so we can refer to "the first test, whatever that is".
    SELF_TEST_STEP_FIRST = 0,

    // Non-interactive tests
    SELF_TEST_STEP_LEDS = SELF_TEST_STEP_FIRST,
    SELF_TEST_STEP_NOR_FLASH,
    SELF_TEST_STEP_SD_WRITE_READ,
    SELF_TEST_STEP_USB_DEVICE_MODE,

    // Interactive tests
    SELF_TEST_STEP_KEYPAD,
    SELF_TEST_STEP_AUDIO,

    // keep this one last
    SELF_TEST_STEP_END,
    // this means that all of the then-active tests passed at some time.
    SELF_TEST_PASSED = 126 // largest signed char such that it+1 is also a signed char
} SelfTestStep;
typedef enum {
    SELF_TEST_RESULT_SUCCESS = 0,

    SELF_TEST_RESULT_FAILURE = 1,

    SELF_TEST_RESULT_SD_WRITE_FAILURE,
    SELF_TEST_RESULT_SD_READ_FAILURE,
    SELF_TEST_RESULT_AUDIO_RECORD_FAILURE,
    SELF_TEST_RESULT_AUDIO_PLAYBACK_FAILURE,

    SELF_TEST_RESULT_NOTHING = 126
} SelfTestResult;
extern char *SelfTestNames[];

#define STATSIZE (sizeof(struct ondisk_filestats) << 1) 

extern void *FindLastFlashStruct(char);
extern void *FindFlashStruct(char, int);
extern void exportFlashStats(void);
extern void *FindFirstFlashStruct(char);
extern void *FindNextFlashSameStruct(void *fp);
extern void *AppendStructToFlash(void *);
extern void writeMsgEventToFlash (char *, int, unsigned int);
extern void warmStartNORStats(void);
extern void coldStartNORStats(void);
extern void createMsgNameOffsets(void);
extern int FindFirstFlashOffset(void);

#endif

