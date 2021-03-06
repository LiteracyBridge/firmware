// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__CONTAINERS_h__
#define	__CONTAINERS_h__

#include "lists.h"

#define MAX_PROFILES	2
#define MAX_LANGUAGES 	3
#define MAX_MESSAGE_LISTS	5
#define MAX_CONTROL_TRACKS 3
#define MAX_LANGUAGE_CODE_LENGTH 12	// long enough to support current new language naming convention in wrap_translation()
#define MAX_MESSAGE_LIST_CODE_LENGTH 8
#define MAX_CONTROL_TRACK_CODE_LENGTH 8
#define MAX_PROFILE_NAME_LENGTH	20

#define MAX_FILES		100
#define MAX_BLOCKS		100
#define MAX_STATES		(2 * MAX_BLOCKS)   // should always be 2 x MAX_BLOCKS
#define MAX_ACTIONS		120   // a little more than 2 actions per block (in addition to start/end actions)
#define MAX_LISTS		3  		// includes 2 regular lists (list of lists and list of packages) and translation list
#define MAX_BLOCK_OVERLAP	2   // allows for a file-wide block, a page, and a hyperlink
#define PKG_HEAP_SIZE	500  // enough for ~5 chars per struct file/filename and per ListItem filename
#define PKG_STACK_SIZE	80   // only stores one item and is erased by next item stored (used for list item filenames)
// PKG_STACK_SIZE is used so that list filenames can be relative to the heap without being accumulated and hogging memory

#define PKG_NONE		0
#define PKG_SYS			1
#define PKG_APP			2
#define PKG_MSG			3
#define PKG_VARIABLE	4
#define SYS_MSG			5

#define APP_CUSTOM		0
#define APP_QUIZ_PLAY   1
#define APP_QUIZ_REC 	2

typedef enum EnumEvent EnumEvent;
typedef enum EnumAction EnumAction;
typedef enum EnumBorderCrossing EnumBorderCrossing;
typedef struct CtnrFile CtnrFile;
typedef struct CtnrBlock CtnrBlock;
typedef struct CtnrPackage CtnrPackage;
typedef struct Action Action;
typedef struct Context Context;
typedef struct ProfileData ProfileData;

//WARNING: if the order of these enumerations change, there must also be a change to getEventEnumFromChar and getActionEnumFromChar (char *c)
#define HELD_KEY		0x10
enum EnumEvent {
	START, END, BUTTON_MARKER, 
	LEFT, RIGHT, UP, DOWN, SELECT, HOME, PLAY, STAR, PLUS, MINUS, 
	LEFT_HOLD = HELD_KEY, RIGHT_HOLD, UP_HOLD, DOWN_HOLD, SELECT_HOLD, HOME_HOLD, PLAY_HOLD, STAR_HOLD, PLUS_HOLD, MINUS_HOLD
};
enum EnumAction {NOP = 0, STOP, PAUSE, JUMP_BLOCK, RETURN, INSERT_SOUND, START_END_MARKER,			
				PLAY_PAUSE, COPY, CLONE, COPY_PROFILE, RECORD_TITLE, RECORD_MSG, PACKAGE_RECORDING, RECORD_FEEDBACK, RECORD_TRANSLATION, TRIM,
				TOGGLE_LOCK, POSITION_TO_TOP, SURVEY_TAKEN, SURVEY_APPLY, SURVEY_USELESS, ROTATE, FWD, BACK, JUMP_TIME, CALL_BLOCK, JUMP_PACKAGE, 
				JUMP_LIST, TRANSLATED_LIST, NOT_TRANSLATED_LIST, TRANSLATE_DELETE_FINISH, 
				TRANSLATE_NEW, TRANSLATE_OVERWRITE,DELETE, DELETE_MESSAGES, DELETE_TRANSLATION, WRAP_TRANSLATION,
				MAKE_FAVORITE,
                ENTER_EXIT_MARKER, VOLUME_UP, VOLUME_DOWN, VOLUME_NORMAL, SPEED_UP, SPEED_DOWN, SPEED_NORMAL, SPEED_MAX,  
				USB_MARKER, USB_DEVICE_ON, USB_HOST_ON, USB_DEVICE_OFF, USB_HOST_OFF,  
                LED_MARKER, LED_RED_ON, LED_GREEN_ON, LED_ALL_ON, LED_RED_OFF, LED_GREEN_OFF, LED_ALL_OFF,
                HALT, SLEEP, TEST_PCB, EOL_MARKER}; //end-of-list marker
///WARNING: I think EnumAction has to be limited to 64 items (last count with TOGGLE_LOCK:61), based on available bits for it in eventAction.
enum EnumBorderCrossing {PLAYING, FORWARD_JUMPING, BACKWARD_JUMPING};

struct CtnrFile {
	//TODO: char *altFilename; //for alternative language or brief/verbose option in Audio UI
	//other metadata about file?
	int idxFilename;
	int idxFirstBlockStart; // This allows run loop to flip through ordered list of blocks as time count progresses
	int idxFirstBlockEnd; // This allows run loop to flip through ordered list of blocks about to end as time count progresses
	int idxFirstBlockInFile; // -1 indicates this belongs to a list  
	//Othrwise, idxFirstBlockInFile allows easy file-block correlation, 
	//there's no idxNextFile because the array order is the play order (same as text file order); package.countFiles indicates end.
	//to save space, we could get rid of idxFirstBlockInFile and make sure idxFirstBlockStart always points to the first block for the file
	//by swapping the first block with the earliest start block during parsing.  
};

struct CtnrBlock {
	unsigned int startTime; // relative to physical file, not ctnrFile 
	unsigned int endTime; // 0xFFFF means continue to end of ctnrFile
	int idxFirstAction; // index into action[]
	int idxNextStart; // index of the next block to start after this one
	int idxNextEnd; // index of the next block to end after this one
	int actionStartEnd; // see bit map below
	int actionEnterExit;  // see bit map below
};

// BIT MAP of idxNextStart
// bits 0-9: index of next block to start (0-1023), where value 0x03FF means last block     see const maskIdxNextStart 
// bits 10-15: id of block type (0-63) used for forward/backward relative jumps

// BIT MAP of actionStartEnd
// bits 0-3: start action bits
//   bits 0-1: start actionCode
//   bit 2: start return action
//   bit 3: start insert sound

// bits 8-11: end action bits:
//   bits 8-9: end actionCode
//   bit 10: end return action
//   bit 11: end insert sound
//
// BIT MAP of actionEnterExit: LSB is enter; MSB is exit.  Below is map for either byte -- see DEFINEs above
// bits 0-3: LEDs and hyperlink status
// 0000: no change to LEDs; no hyperlink
// 0001: turn Green LED off; no hyperlink
// 0010: turn Red LED off; no hyperlink
// 0011: turn both LEDs off; no hyperlink
// 0100: hyperlink (no LED control)
// 0101: turn Green LED on; no hyperlink
// 0110: turn Red LED on; no hyperlink
// 0111: turn both LEDs on; no hyperlink
// 1000: USB device
// 1001: turn Green LED on; turn Red LED off
// 1010: turn Red LED on; turn Greeen LED off
// 1011: undefined
// 1100: undefined
// 1101: undefined
// 1110: undefined
// 1111: undefined
//
// bits 4-5: Volume change
//  00: no change
//  01: normal volume (last user set volume)
//  10: lower volume
//  11: higher volume
//
// bits 6-7: Speed change
//  00: no change
//  01: normal speed
//  10: slower speed
//  11: faster speed

struct Action {
// TODO: consider jumping to both a new context and a relative jump within it.  
// For now - only look at relativeJump if *newContext is null 
	unsigned int eventAction; // LSB for event; MSB for action
	int destination; // index into blocks[] or relative time jump value or "6-bit block-type id" for relative fwd/back jumps (see ctnrBlock)
	unsigned int aux;  //see BIT MAP below
	//TODO: make sure it's okay for multiple blocks to share actions that might be found in the list of only one of the blocks
	//TODO int goBack; //number of steps TODO:need a list of history to implement this
	//TODO int pauseTime;
	//TODO:boolean doPause;
};

// BIT MAP of eventAction
// bits 0-3: event id #1
// bit 4: long-hold keypresses
// bit 5: pause state (1== applies only when paused; 0 == any time, unless superceded by event specifed for when paused)
// bits 6-7: unused
// bits 8-13: action code
// bit 14: indicates need to insert sound by playing block number in bits 0-10 of aux before executing action code
// bit 15: indicates last event/action for container (e.g. block/pkg)

//BIT MAP of aux 
// bits 0-10: block number of sound insert before executing action code
// bits 11-12: speed change...00: no change, 01: normal speed, 10: slower speed, 11: faster speed
// bits 13-15: FOR CALL_BLOCK: indicate how far to rewind after hyperlink return relative to activation point - see REWIND[]
//             FOR JUMP_LIST: bit 15 set to movee to previous list item; bit 14 to move to next list item; otherwise use current item.


struct CtnrPackage {
	int idxName;  //reference to Heap index; same as directory name for "OTHER content" (anything non-system audio received from elsewhere)
	int pkg_type; // see defined values (PKG_SYS, PKG_APP, PKG_MSG)
	int app_type; // e.g. APP_QUIZ_PLAY, APP_QUIZ_REC, and APP_CUSTOM
    CtnrFile tempFile; // used for playing list names and package titles
	CtnrFile files[MAX_FILES]; 
	CtnrBlock blocks[MAX_BLOCKS];
	Action actions[MAX_ACTIONS];
	ListItem lists[MAX_LISTS];
	int countPackageActions;
	int countFiles;
	int countBlocks;
	int countLists;
	int idxMasterList;	
	int blockBookmark;  //TODO:presist this to SDcard and consider also storing ctnrBlock, if any 
	char strHeapStack[PKG_HEAP_SIZE+PKG_STACK_SIZE];
	int idxStrHeap;
	int timePrecision;  
	//BOOL recInProgress;
	int idxLanguageCode;
    TranslationList transList;
	//TODO:unsigned int length;
	//TODO:unsigned int playCount;
	//TODO:boolean allowReplies;
	//add in other metadata here like author info, OWNER ratings
};

struct Context {
	//TODO: folder containers
	CtnrPackage *package;
	CtnrFile *file;
	int idxTimeframe; // current context of active blocks; index into blockTimeline[]
	int idxPausedOnBorder; // block index where end event executed but context has not yet changed
	unsigned int timeNextTimeframe; // time associated with next timeline point (could be start and/or end event)
	int keystroke; // to store keystroke during insertSound and later process in mainLoop
	BOOL isPaused;
	BOOL isStopped;
	BOOL isScanning;
	BOOL isHyperlinked; //1=hyperlink is currently active
	BOOL USB;
	int queuedPackageType;
	int queuedPackageNameIndex;
	int idxActiveList; // -1 == no active list
	CtnrPackage *returnPackage;
    CtnrFile *lastFile;
    long packageStartTime;
    unsigned long msgLengthMsec;
    BOOL msgAtEnd;
};

struct ProfileData {
	//TODO the 'heap...' members below are not really used like a heap -- just as an array, which is fine, but it's not a heap.  Why are ptrs below needed?
	char heapProfileNames[MAX_PROFILES][MAX_PROFILE_NAME_LENGTH];
	char heapLanguages[MAX_LANGUAGES][MAX_LANGUAGE_CODE_LENGTH + 1]; // max language code +1 for /0
	char heapMessageLists[MAX_MESSAGE_LISTS][MAX_MESSAGE_LIST_CODE_LENGTH + 1]; // max message list code length +1 for /0
	char heapControlTracks[MAX_CONTROL_TRACKS][MAX_CONTROL_TRACK_CODE_LENGTH + 1]; // max control track list code length + 1 for /0
	char *ptrProfileNames[MAX_PROFILES];
	char *ptrProfileLanguages[MAX_PROFILES];
	char *ptrProfileMessageLists[MAX_PROFILES];
	char *ptrProfileControlTracks[MAX_PROFILES];
	int intTotalProfiles;
	int intTotalLanguages;
	int intTotalMessageLists;
	int intTotalControlTracks;
	int intCurrentProfile;
};

extern Context context;
extern CtnrPackage pkgSystem, pkgUser; 

#define MASK_IDX_NEXT_START	0x03FF   // masks bits 0-9; see ctnrBlock.idxNextStart and action.destination

extern BOOL stackPush (CtnrPackage *, CtnrFile *, int);
extern BOOL stackPop(CtnrPackage **, CtnrFile **, int *);
extern void setRewind(int *, int);
extern int getRewind(unsigned int);
extern int getSoundInsertIdxFromAux(int);
extern void setStartCode(int *, EnumAction);
extern void setEndCode(int *, EnumAction);
extern CtnrBlock *getStartInsert(int, int);
extern CtnrBlock *getEndInsert(int, int);
extern CtnrBlock* getNextStartBlock(CtnrBlock *);
extern void setNextStartBlock(int, int);
extern CtnrBlock* getNextEndBlock(CtnrBlock *);
extern CtnrBlock* getNextStartBlockFromTime(unsigned int);
extern CtnrBlock* getNextEndBlockFromTime(unsigned int);
extern CtnrBlock* getStartBlockFromFile(CtnrFile *);
extern CtnrBlock* getEndBlockFromFile(CtnrFile *);
extern CtnrPackage* getPackageFromFile (CtnrFile *);
extern CtnrFile* getFileFromBlock (CtnrBlock *);
extern void setBlockHyperlinked(CtnrBlock *);
extern BOOL isBlockHyperlinked(CtnrBlock *);
extern Action* getNextAction(Action *);
extern EnumAction getStartCode(CtnrBlock *, int *, EnumBorderCrossing);
extern EnumAction getEndCode(CtnrBlock *, int *, EnumBorderCrossing);
extern void setSoundInsert(Action *, BOOL);
extern BOOL hasSoundInsert(Action *);
extern BOOL isEventInAction(Action *, EnumEvent, BOOL);
extern EnumAction getActionCode(Action *);
extern void setActionCode(Action *, EnumAction);
extern void setEventCodes(Action *, EnumEvent, BOOL, BOOL);
extern void setEndOfActions(Action *, BOOL);
extern Action *getBlockActions(CtnrBlock *);
extern Action *getListActions(ListItem *);
extern Action *getTransListActions(TranslationList *);
extern int replaceStack (char *, CtnrPackage *);
extern CtnrFile *getTempFileFromName(char *, int); 
extern void resetPackage(CtnrPackage *);
extern void loadDefaultUserPackage(const char *);
extern void logProfile(void);
extern int initializeProfiles(void);
extern int initializeProfilesBeforeConfig(void);
extern int loadProfileNames(char *, ProfileData *);
extern char *currentProfileMessageList(void);
extern char *currentProfileLanguage(void);
extern char *currentProfileControlTrack(void);
extern int currentProfile(void);
extern int nextProfile(void);
extern int prevProfile(void);	
extern ProfileData *getProfiles(void);
extern char *getProfileName(int);
extern int totalProfiles(void);
extern char *currentProfileName(void);
#endif
