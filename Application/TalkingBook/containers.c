// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/talkingbook.h"
#include "Include/parsing.h"
#include "Include/containers.h"
#include "Include/files.h"
#include ".\Component\Include\FS\vfs.h"
#include <ctype.h>

static const int REWIND[] = {0,500,1000,1500,2000,3000,5000,10000};
APP_IRAM Context context;
APP_IRAM CtnrPackage pkgSystem, pkgUser;
APP_IRAM static ProfileData profiles; 

typedef struct StackFrame StackFrame;
struct StackFrame {
	// This struct could be compressed more, but there should be very few stackFrames in memory at once.
	CtnrPackage *package;
	CtnrFile *file;
	int offset; 
};

APP_IRAM static StackFrame stack[MAX_HYPERLINK_STACK];
APP_IRAM static int stackCount;


static BOOL isStackEmpty(void);
static BOOL isLastAction(Action *);

static BOOL isStackEmpty(void) {
	return (BOOL)(stackCount == 0);
}

BOOL stackPush (CtnrPackage *pkg, CtnrFile *file, int offset) {
	int ret;
	if (stackCount < MAX_HYPERLINK_STACK) {
		stack[stackCount].package = pkg;
		stack[stackCount].file = file;
		stack[stackCount].offset = offset;
		stackCount++;
		ret = TRUE;
	}
	else 
		ret = FALSE;
	return ret;	
}

BOOL stackPop(CtnrPackage **pkg, CtnrFile **file, int *offset) {
	BOOL ret;
	if (stackCount > 0) {
		stackCount--;
		*offset = stack[stackCount].offset;
		*file = stack[stackCount].file;
		*pkg = stack[stackCount].package;
		ret = TRUE;
	}
	else
		ret = FALSE;
	return ret;
}

void setRewind(int *actionAux, int rewindAmount) {
	int bitPattern;
	int absRewindAmount;
	int i;
	
	//math is more intuitive using abs values
	absRewindAmount = abs(rewindAmount);

	//can't jump forward, so use zero
	if (rewindAmount >=	0 || absRewindAmount < REWIND[1]/2) 
		bitPattern = 0x0;
	else 
		for (i=1;i<7;i++) {
			// find closest preset rewind amount
			if (absRewindAmount < (REWIND[i] + REWIND[i+1]) / 2) {
				bitPattern = i;
				break;
			}
			bitPattern = 7;
		}
	*actionAux &= 0x1FFF;	// zero out highest three bits
	*actionAux |= (bitPattern << 13); // set highest three bits with bitPattern
}

int getRewind(unsigned int actionAux) {
	int bitPattern;
	int ret;
	
	bitPattern = actionAux >> 13;
	ret = compressTime(REWIND[bitPattern],context.package->timePrecision);
	return ret;
}

int getSoundInsertIdxFromAux(int aux) {
	// return bits 0-10 only
	return aux & 0x07FF;
}

void setStartCode(int *startEndCode, EnumAction codeAction) {
	// see bit map for CtnrBlock.actionStartEnd
	int startCode;
	// we're only using 4bits of each byte right now	
	if (codeAction >= 0 && codeAction < START_END_MARKER) {
		startCode = *startEndCode & 0x00FF;
		if (codeAction == INSERT_SOUND)
			startCode |= 0x08;  // set bit 3
		else if (codeAction == RETURN)
			startCode |= 0x04;  // set bit 2
		else {
			startCode &= 0x0C;  // clear bits 0&1
			startCode |= codeAction; // bits 0&1 equals code
		}
		*startEndCode &= 0xFF00; // clear bits to set
		*startEndCode |= startCode; // set bits
	}
}

void setEndCode(int *startEndCode, EnumAction codeAction) {
	// see bit map for CtnrBlock.actionStartEnd
	int endCode;
	// we're only using 4bits of each byte right now
	if (codeAction >= 0 && codeAction < START_END_MARKER) {
		endCode = *startEndCode >> 8;
		if (codeAction == INSERT_SOUND)
			endCode |= 0x08;  // set bit 3
		else if (codeAction == RETURN)
			endCode |= 0x04;  // set bit 2
		else {
			endCode &= 0x0C;  // clear bits 0&1
			endCode |= codeAction; // bits 0&1 equals code
		}
		*startEndCode &= 0x00FF; // clear bits to set
		*startEndCode |= (endCode << 8); // set bits
	}
}

struct CtnrBlock *getStartInsert(int actionStartEnd, int idxFirstAction) {
	CtnrBlock *ret;
	Action *actions, *action;
	int idxBlock;
	
	ret = 0;
	if (actionStartEnd & 0x08) { // check INSERT_SOUND bit is set
		actions = context.package->actions;
		action = &actions[idxFirstAction];
		while (!isEventInAction(action,START,context.isPaused) && !hasSoundInsert(action)) {
			if (!isLastAction(action))
				action = getNextAction(action);
			else
				action = 0;
				break;
		}
		if (action) {
			idxBlock = getSoundInsertIdxFromAux(action->aux);
			ret = &context.package->blocks[idxBlock];
		}
	}	
	return ret;	 	
}

struct CtnrBlock *getEndInsert(int actionStartEnd, int idxFirstAction) {
	CtnrBlock *ret;
	Action *actions,*action;
	int idxBlock;
	
	ret = 0;
	if (actionStartEnd & 0x0800) { // check INSERT_SOUND bit is set
		actions = context.package->actions;
		action = &actions[idxFirstAction];
		while (!isEventInAction(action,END,context.isPaused) && getActionCode(action) != INSERT_SOUND) {
			if (!isLastAction(action))
				action = getNextAction(action);
			else
				action = 0;
				break;
		}
		if (action) {
			idxBlock = getSoundInsertIdxFromAux(action->aux);
			ret = &context.package->blocks[idxBlock];
		}
	}	
	return ret;	 	
}

CtnrBlock* getNextStartBlock(CtnrBlock *block) {
	CtnrBlock *ret;
	int idx = block->idxNextStart & MASK_IDX_NEXT_START;
	if (idx == MASK_IDX_NEXT_START)  // highest possible block id (similar to -1) means last start block
		ret = 0;
	else
		ret = &context.package->blocks[idx];
	return ret;
}

void setNextStartBlock(int idxCurrentBlock, int idxNextBlock) {
	CtnrBlock *block;
	
	// todo: log this error if (idxNextBlock >= MASK_IDX_NEXT_START || idxCurrentBlock >= MASK_IDX_NEXT_START)
	if (idxNextBlock == -1)  // last start block
		idxNextBlock = MASK_IDX_NEXT_START;  // use MASK_IDX_NEXT_START since masked with block-type
	block = &context.package->blocks[idxCurrentBlock];
    block->idxNextStart =  (block->idxNextStart & ~MASK_IDX_NEXT_START) | idxNextBlock;
}

CtnrBlock* getNextEndBlock(CtnrBlock *block) {
	CtnrBlock *ret;
	int idx = block->idxNextEnd;
	if (idx == -1)
		ret = 0;
	else
		ret = &context.package->blocks[idx];
	return ret;
}

CtnrBlock* getNextStartBlockFromTime(unsigned int elapsedFileTime) {
	CtnrBlock *cursorBlock;

	cursorBlock = getStartBlockFromFile(context.file);
	while (cursorBlock && (cursorBlock->startTime <= elapsedFileTime)) 
		cursorBlock = getNextStartBlock(cursorBlock);	
	return cursorBlock;	
}
 
CtnrBlock* getNextEndBlockFromTime(unsigned int elapsedFileTime) {
	CtnrBlock *cursorBlock;
	
	cursorBlock = getEndBlockFromFile(context.file);
	while (cursorBlock && (cursorBlock->endTime <= elapsedFileTime)) 
		cursorBlock = getNextEndBlock(cursorBlock);	
	return cursorBlock;	
}

CtnrBlock* getStartBlockFromFile(CtnrFile *file) {
	CtnrBlock *ret;
	
	if (file->idxFirstBlockStart != -1)
		ret = &context.package->blocks[file->idxFirstBlockStart];
	else
		ret = 0;
	return ret;
}

CtnrBlock* getEndBlockFromFile(CtnrFile *file) {
	CtnrBlock *ret;
	
	if (file->idxFirstBlockEnd != -1)
		ret = &context.package->blocks[file->idxFirstBlockEnd];
	else
		ret = 0;
	return ret;
}

CtnrPackage* getPackageFromFile (CtnrFile *file) {
	CtnrPackage *ret;
	int i;
	char path[PATH_LENGTH];
	
	if (file >= pkgSystem.files && file < (pkgSystem.files + pkgSystem.countFiles))
		ret = &pkgSystem;
//	else if (file >= pkgDefault.files && file < (pkgDefault.files + pkgDefault.countFiles))
//		ret = &pkgDefault;
	else if (file >= pkgUser.files && file < (pkgUser.files + pkgUser.countFiles))
		ret = &pkgUser;
	else if (file == &context.package->tempFile)
		ret = context.package;
	else {
		// delete the binary control track to force a re-parse.
		strcpy(path,LANGUAGES_PATH);
		catLangDir(path);
		strcat(path,PKG_CONTROL_FILENAME_BIN);
		i = unlink((LPSTR)path);	
		if (i)
			strcat(path,(char *)": tried to delete this binary control");
		else
			strcpy(path,(char *)"deleted binary control to force re-parse");
		logException(18,path,RESET); //file address doen't match pkgSystem or pkgUser	
	}
	return ret;
}

CtnrFile* getFileFromBlock (CtnrBlock *block) {
	int idxFile;
	int idxBlock;
	int filesInPackage;
	
	filesInPackage = context.package->countFiles;	
	idxBlock = block - context.package->blocks;
	
	for (idxFile = filesInPackage-1; idxFile >= 0; idxFile--) {
		if (idxBlock >= context.package->files[idxFile].idxFirstBlockInFile) {
			break;
		}
	}
	//todo:error if not idxBlock < files[0].idxFirstBlockInFile
	return &context.package->files[idxFile];
}

void setBlockHyperlinked(CtnrBlock *block) {
	int code;
	
	code = block->actionEnterExit;
	code &= 0xFFF0; //clear bits 0-3
	code |= 0x04;   //set hyperlink code (see CtnrBlock definition)
	block->actionEnterExit = code;
} 

BOOL isBlockHyperlinked(CtnrBlock *block) {
	int code;
	BOOL ret;
	
	code = block->actionEnterExit;
	code &= 0x000F; //clear all but bits 0-3
	ret = (code == 0x04); //hyperlink code (see CtnrBlock definition)
	return ret;
} 


Action* getNextAction(Action *action) {
	Action *ret;
	int isLastOne = isLastAction(action);
	if (isLastOne)
		ret = 0;
	else		
		ret = action + 1;
	return ret;
}

EnumAction getStartCode(CtnrBlock *block, int *idxAction, EnumBorderCrossing approach) {
	// This function returns an action based on a particular block, how its start event is being
	// approached (playing, forward/backward jumping, and some rules embedded in the function.

	int startEnd, ret;
	EnumAction startCode;
	BOOL returnAction;
	Action *actions, *cursor;
	ret = 0;
	*idxAction = -1;

	if (approach == BACKWARD_JUMPING) {
		// Start event is never triggered when playing or jumping forward.
		// Inserted sound may still occur, but there is a separate fct for checking that.
		startEnd = block->actionStartEnd;
		startCode = startEnd & 0x00FF;
		returnAction = (startCode & 0x04);  // check bit 2 is set
		if (returnAction && !isStackEmpty())  // only give return if there's something to pop from the stack
			ret = RETURN;
		else {	
			ret = startCode & 0x03; // just bits 0 and 1
			if (ret == JUMP_BLOCK) {
				// find action index for JUMP_BLOCK
				actions =(Action *)&context.package->actions;
				cursor = &actions[block->idxFirstAction];
				while (!isEventInAction(cursor,START,context.isPaused) && getActionCode(cursor) != JUMP_BLOCK) {
					if (!isLastAction(cursor))
						cursor= getNextAction(cursor);
					else {
						cursor = 0;
						break;
					}
				}
				if (cursor)
					*idxAction = cursor - actions;
				else
					*idxAction = -1; 
			}
		}
	}
	return ret;
}

EnumAction getEndCode(CtnrBlock *block, int *idxAction, EnumBorderCrossing approach) {
	// This function returns an action based on a particular block, how its end event is being
	// approached (playing, forward/backward jumping, and some rules embedded in the function.

	int startEnd, ret;
	EnumAction endCode;
	BOOL returnAction;
	Action *actions, *cursor;
	ret = 0;
	*idxAction = -1;

	if (approach != BACKWARD_JUMPING) {
		// End event is never triggered when jumping backward.
		startEnd = block->actionStartEnd;
		endCode = startEnd >> 8;
		returnAction = (endCode & 0x04);  // check bit 2 is set
		if (returnAction && !isStackEmpty())  // only give return if there's something to pop from the stack
			ret = RETURN;
		else {	
			ret = endCode & 0x03; // just bits 0 and 1
			if (ret == PAUSE) {
				*idxAction = -1;
				if (approach != PLAYING)
					ret = NOP;
			}
			else {
				if (ret == JUMP_BLOCK) {
					// find action index for JUMP_BLOCK
					actions = context.package->actions;
					cursor = &actions[block->idxFirstAction];
					while (!isEventInAction(cursor,END,context.isPaused) && getActionCode(cursor) != JUMP_BLOCK) {
						if (!isLastAction(cursor))
							cursor= getNextAction(cursor);
						else {
							cursor = 0;
							break;
						}
					}
					if (cursor)
						*idxAction = cursor - actions;
					else
						*idxAction = -1; 
				}
			}
		}
	}
	return ret;
}

void setSoundInsert(Action *action, BOOL flagSoundInsert) {
	if (flagSoundInsert) 
		// set bit 14 of eventAction -- see struct definition for action
		action->eventAction = action->eventAction | 0x4000;
	else
		action->eventAction = action->eventAction & 0xBFFF;
}
		
BOOL hasSoundInsert(Action *action) {
	return (action->eventAction >> 14) & 0x01;
}

BOOL isEventInAction(Action *action, EnumEvent checkEvent, BOOL whenPaused) {
	// events are LSB; see bit map for Action
	unsigned int eventAction;
	EnumEvent eventCode;
	BOOL ret;
	
	eventAction = action->eventAction;
	eventCode = eventAction & 0x00FF; 
	ret = ((eventCode & 0x1F) == checkEvent);
	if (ret && !whenPaused) // device is playing or stopped (not paused)
		ret = !(eventCode & 0x20); // event does not apply if set only for paused state
	return ret;
}

EnumAction getActionCode(Action *action) {
	// action is MSB; see bit map for Action
	unsigned int eventAction;
	EnumAction actionCode;

	eventAction = action->eventAction;
	actionCode = eventAction >> 8;
	return (actionCode & 0x3F);  // removes highest two bits
}

void setActionCode(Action *action, EnumAction actionCode) {
	int eventActionCode = action->eventAction;
	eventActionCode = eventActionCode & 0xC0FF; //zero everything in high byte besides bits 14 and 15
	eventActionCode = eventActionCode | (actionCode << 8);
	action->eventAction = eventActionCode;	
}

void setEventCodes(Action *action, EnumEvent eventCode, BOOL whenPaused, BOOL whenHeld) {
	int eventActionCode = action->eventAction;
	eventActionCode = eventActionCode & 0xFF00;
	eventActionCode = eventActionCode | eventCode | (whenHeld << 4) | (whenPaused << 5);
	action->eventAction = eventActionCode;	
}

void setEndOfActions(Action *action, BOOL flagEndOfList) {
	if (flagEndOfList) 
		action->eventAction = action->eventAction | 0x8000;
	else
		action->eventAction = action->eventAction & 0x7FFF;
}

static BOOL isLastAction(Action *action) {
	return (action->eventAction >> 15);
}

Action* getBlockActions(CtnrBlock *block) {
	Action *ret;
	int idx = block->idxFirstAction;
	if (idx == -1)
		ret = 0;
	else
		ret = &context.package->actions[idx];
	return ret;
}	
Action* getTransListActions(TranslationList *list) {
	Action *ret;
	int idx = list->idxFirstAction;
	if (idx == -1)
		ret = 0;
	else
		ret = &context.package->actions[idx];
	return ret;
}
Action* getListActions(ListItem *list) {
	Action *ret;
	int idx = list->idxFirstAction;
	if (idx == -1)
		ret = 0;
	else
		ret = &context.package->actions[idx];
	return ret;
}	

int replaceStack (char *line, CtnrPackage *pkg) {
	// This is a one-item stack that get erased by then next item added.
	// It sits on top of the heap space so that a relative int to the heap can be used, as it is with
	// file.filename for play()/getFileHandle().  This is important for list items, which do not need to
	// be persisted, but are treated like other files in play/getFileHandle.
	int length;
	char *stack, *lineCursor;

	stack = &(pkg->strHeapStack[PKG_HEAP_SIZE]);

	while (isspace(*line) && *line != 0)
		line++;
	lineCursor = strchr(line,0); 
	while (isspace(*(lineCursor-1)) && lineCursor >= line)
		lineCursor--;
	*lineCursor = 0;
	length = (lineCursor - line + 1);
	if (length > PKG_STACK_SIZE)
		logException(12,0,RESET); //todo:attempting to store string too big for pkg stack	
	strcpy(stack,line);
	return PKG_HEAP_SIZE; //marks the beginning of the stack	
}

CtnrFile *getTempFileFromName(char *name, int getLongName) {
	// This function takes a filename and populates a CtnrFile object for it.
	// The getLongName feature is used when creating a category file and distinguishing the simple category name
	// from the instruction to choose that cateogory (e.g. "press up to listen to ____ category").
	CtnrFile *file;
	char temp[FILE_LENGTH];
	
	if (getLongName) { //Temporary Patch  
		temp[0] = 'i';  // to distinguish the instruction associated with a category from just the category name
		strcpy(temp+1,name);
	} else
		strcpy(temp,name);
	file = &pkgSystem.tempFile;
	file->idxFilename = replaceStack(temp,&pkgSystem);	
	file->idxFirstBlockStart = 0;
	file->idxFirstBlockEnd = 0;
	file->idxFirstBlockInFile = -1;
	return file;
}

void resetPackage(CtnrPackage *pkg) {
	pkg->countBlocks = 0;
	pkg->countFiles = 0;
	pkg->countLists = 0;
	pkg->countPackageActions = 0;
	pkg->idxStrHeap = 0;
	pkg->idxMasterList = 0;
	pkg->tempFile.idxFilename = 0;	
	pkg->tempFile.idxFirstBlockStart = 0;
	pkg->tempFile.idxFirstBlockEnd = 0;
	pkg->tempFile.idxFirstBlockInFile = 0;
    pkg->files[0].idxFilename = 0;
    pkg->files[0].idxFirstBlockStart = 0;
    pkg->files[0].idxFirstBlockEnd = 0;
    pkg->files[0].idxFirstBlockInFile = 0;
    pkg->lists[0].idxFirstAction = 0;
    pkg->lists[0].currentFilePosition = 0;
    pkg->lists[0].idxListWithFilename = 0;
    pkg->lists[0].posListWithFilename = 0;
    pkg->lists[0].currentString[0] = 0;
    pkg->lists[0].filename[0] = 0;
    pkg->lists[1].idxFirstAction = 0;
    pkg->lists[1].currentFilePosition = 0;
    pkg->lists[1].idxListWithFilename = 0;
    pkg->lists[1].posListWithFilename = 0;
    pkg->lists[1].currentString[0] = 0;
    pkg->lists[1].filename[0] = 0;


//	CtnrFile files[MAX_FILES]; 
//	CtnrBlock blocks[MAX_BLOCKS];
//	Action actions[MAX_ACTIONS];
//	ListItem lists[MAX_LISTS];
//	char strHeapStack[PKG_HEAP_SIZE+PKG_STACK_SIZE];
	
}


void loadDefaultUserPackage(const char *strPkgName) {
	char sTemp[PATH_LENGTH];
	
	memset(&pkgUser,0,sizeof(CtnrPackage));
	pkgUser.pkg_type = PKG_MSG;
	
	if (USER_CONTROL_TEMPLATE) {
		// load in configurable control template
		strcpy(sTemp,SYSTEM_PATH);
		strcat(sTemp,USER_CONTROL_TEMPLATE);
		// TODO: load in binary version if exists
		strcat(sTemp,".txt"); //todo: move to config file	
		parseControlFile (sTemp, &pkgUser);	
		//TODO: save binary version
		strcpy(pkgUser.strHeapStack+pkgUser.files[0].idxFilename,strPkgName);
	} else {
		// create the basic default user package dynamically
		pkgUser.timePrecision = getBitShift(DEFAULT_TIME_PRECISION);
		pkgUser.idxLanguageCode = -1;
		strcpy(pkgUser.strHeapStack,strPkgName);
		pkgUser.idxStrHeap = strlen(strPkgName) + 1;  // necessary since loadPackage() adds more to this heap for SYS_MSG packages

		pkgUser.countFiles = 1;
		pkgUser.files[0].idxFilename = 0;  // filename found at top of heap
		pkgUser.files[0].idxFirstBlockStart = 0;
		pkgUser.files[0].idxFirstBlockEnd = -1;
		pkgUser.files[0].idxFirstBlockInFile = 0;
		pkgUser.files[1].idxFilename = -1;			

		pkgUser.countBlocks = 1;
		pkgUser.blocks[0].startTime = 0;
		pkgUser.blocks[0].endTime = -1;
		pkgUser.blocks[0].idxNextStart = MASK_IDX_NEXT_START;
		pkgUser.blocks[0].idxNextEnd = -1;
		pkgUser.blocks[0].idxFirstAction = -1; 
		setNextStartBlock(0,-1);

		pkgUser.countPackageActions = 1;
		setActionCode(&pkgUser.actions[0], JUMP_BLOCK);
		setEventCodes(&pkgUser.actions[0], START, FALSE, FALSE);
		pkgUser.actions[0].destination = 0;
		setEndOfActions(&pkgUser.actions[0],TRUE);
	}
	pkgUser.idxName = pkgUser.files[0].idxFilename;
}

extern void logProfile() {
	char strLog[40];
	
	strcpy(strLog,(char *)"PROFILE: ");
	strcat(strLog,currentProfileLanguage());
	strcat(strLog,",");
	strcat(strLog,currentProfileMessageList());
	logString(strLog,BUFFER);	
}

extern int initializeProfiles() {
	char profileOrderFile[PATH_LENGTH];
	int ret;
	
	strcpy(profileOrderFile,SYSTEM_PATH);
	strcat(profileOrderFile,PROFILE_ORDER_FILE);	
	strcat(profileOrderFile,".txt"); //todo: move to config file	
	ret = loadProfileNames(profileOrderFile,&profiles);
	logProfile();
	return ret;
}

extern int loadProfileNames(char *path, ProfileData *pd) {
	// This function will load any profile file into any ProfileData struct.
	// It can be used during startup (see initalizeProfiles()) or when inspecting another device's profiles.

	int ret, i, handle;
	char *ptrLanguage, *ptrMessageList;
	char buffer[READ_LENGTH+1];
	char strLog[80];
	
	handle = tbOpen((LPSTR)path,O_RDONLY);
	if (handle == -1)
		logException(33,path,USB_MODE);
	pd->intTotalProfiles = pd->intTotalLanguages = pd->intTotalMessageLists = 0;
	getLine(-1,0);  // reset in case at end from prior use
	while (nextNameValuePair(handle,buffer,',',&ptrLanguage,&ptrMessageList))	{
		if (strlen(ptrLanguage) > MAX_LANGUAGE_CODE_LENGTH) {
			strcpy(strLog,(char *)"Language code too long:");
			strcat(strLog,ptrLanguage);
			logException(33,strLog,USB_MODE);			
		} else if (!ptrMessageList) {
			strcpy(strLog,(char *)"No comma in profile entry. Language:");
			strcat(strLog,ptrLanguage);
			logException(33,strLog,USB_MODE);			
		} else if (strlen(ptrMessageList) > MAX_MESSAGE_LIST_CODE_LENGTH) {
			strcpy(strLog,(char *)"MessageList code too long:");
			strcat(strLog,ptrMessageList);
			logException(33,strLog,USB_MODE);			
		} else if (pd->intTotalProfiles == MAX_PROFILES) {
			strcpy(strLog,(char *)"Too many profiles:  >");
			longToDecimalString((long)MAX_PROFILES,strLog+strlen(strLog),2);
			logException(33,strLog,USB_MODE);
		}	

		//check that language is new
		i = pd->intTotalLanguages;
		while (--i >=0) {
			if (!strcmp(ptrLanguage,pd->heapLanguages[i]))
				break; // language already exists
		}
		if (i < 0) 	{ // no matching language; add as new entry 
			i = pd->intTotalLanguages++; // assign to latest language and increment total
			if (pd->intTotalLanguages > MAX_LANGUAGES) {
				logException(33,ptrLanguage,USB_MODE);
			}	
			LBstrncpy(pd->heapLanguages[i],ptrLanguage,MAX_LANGUAGE_CODE_LENGTH);
		}
		pd->ptrProfileLanguages[pd->intTotalProfiles] = pd->heapLanguages[i];

		//check that messsage list is new
		i = pd->intTotalMessageLists;
		while (--i >=0)
			if (!strcmp(ptrMessageList,pd->heapMessageLists[i]))
				break; // language already exists
		if (i < 0) {	// no matching message list; add as new entry 
			i = pd->intTotalMessageLists++; // assign to latest message list and increment total
			if (pd->intTotalMessageLists > MAX_MESSAGE_LISTS) {
				logException(33,ptrMessageList,USB_MODE);
			}	
			LBstrncpy(pd->heapMessageLists[i],ptrMessageList,MAX_MESSAGE_LIST_CODE_LENGTH);
		}
		pd->ptrProfileMessageLists[pd->intTotalProfiles] = pd->heapMessageLists[i];

		pd->intTotalProfiles++;
	} 
	close(handle);
	
	ret = pd->intTotalProfiles?1:0;  // at least one language found
	pd->intCurrentProfile = 0;

	return(ret);
}

extern char *currentProfileMessageList() {
	char * ret;
	
	ret = profiles.ptrProfileMessageLists[profiles.intCurrentProfile];
	return ret;
}

extern char *currentProfileLanguage() {
	char * ret;
	
	ret = profiles.ptrProfileLanguages[profiles.intCurrentProfile];
	return ret;
}

extern int currentProfile() {
	return profiles.intCurrentProfile;
}

extern int nextProfile() {
	if (++profiles.intCurrentProfile == profiles.intTotalProfiles)
		profiles.intCurrentProfile = 0;
	return profiles.intCurrentProfile;
}

extern int prevProfile() {	
	if (--profiles.intCurrentProfile == -1)
		profiles.intCurrentProfile = profiles.intTotalProfiles - 1;
	return profiles.intCurrentProfile;
}

extern ProfileData *getProfiles(void) {
	return &profiles;	
}