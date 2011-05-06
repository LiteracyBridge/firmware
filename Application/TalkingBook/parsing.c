// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/talkingbook.h"
#include "Include/files.h"
#include "Include/device.h"
#include "Include/containers.h"
#include "Include/parsing.h"
#include <ctype.h>

typedef struct OrderBlock OrderBlock;

struct OrderBlock{
	int idxBlock;  //index into blocks[] 
	OrderBlock *earlier; //back pointer
};

static int getIndexFromLine(char *, char *);
static int getEventEnumFromChar (char *);
static int getActionEnumFromChar (char *);
static int buildEnterExitCode (int, EnumAction);
static void setEnterCode(CtnrBlock *, EnumAction);
static void setExitCode(CtnrBlock *, EnumAction);
static BOOL parseCreateAction (char *, Action *, int *, char *, CtnrBlock *, ListItem *, TranslationList *);
static BOOL parseCreateBlock (char *, CtnrBlock *, char **, int);
static int getBlockIndexFromSymbol (char *, const char *);
static unsigned int getLinkedTime(OrderBlock *, EnumStartOrEnd);
static void insertOrderBlock (OrderBlock *, OrderBlock **, unsigned int, EnumStartOrEnd);
static void setBlockOrder(CtnrPackage *, OrderBlock *, OrderBlock *);
static int getEnterExitEnumFromChar (char *);

static int getIndexFromLine(char *line, char *symbolMapStart) {
	// This fct finds the [label] for a block and gets the block index number stored in symbolMap
	int index;
	char *cursor;
	char overwritten;
	
	cursor = strchr(line,']');
	//set null marker on ']' or at beginning of any whitespace
	if (cursor) {
		while (*(cursor-1) && ((*(cursor-1) == ' ') || (*(cursor - 1) == '\t')))
			cursor--;
		overwritten = *cursor; // to fix it up again
		*cursor= '\0';
		cursor = strchr(line,'[');
		//todo: check that '[' comes before ']'
		do
			cursor++;
		while (isspace(*cursor));
		index = getBlockIndexFromSymbol (symbolMapStart, cursor);
		if (index == -1) {
			logException(6,cursor,(context.package == &pkgSystem)?USB_MODE:RESET);
		}  // todo:invalid internal reference in control track file
		for (;*cursor;cursor++); // get back to \0
		*cursor = overwritten;  // fix it up
	} else
		index = -1;
	return index;
}

static int getEventEnumFromChar (char *c) {
	int ret = -1;
	const char *EVENT_CODES = "!$`<>^vohp*+-";   // '`' is placeholder for BUTTON_MARKER
	
	ret = strIndex(EVENT_CODES,*c);
	return ret;		
}

static int getActionEnumFromChar (char *c) {
	int ret = -1;
	const char *ACTION_CODES = "~.,[)I`PCCEEEEMOYYYFBT(<LLLLLLDDDW*`VVVSSSS`UUUU`RGARGAHZX";  // '`' is placeholder for marker codes
	// note that E is for rEcord since R should represent Red
	// only first instance of action code is found, others are placeholders as dealt with below
	
	ret = strIndex(ACTION_CODES,*c);
	if (ret >= EOL_MARKER)
		ret = -1;
	else if (ret > LED_MARKER && *(c+1) == '-')  // LEDs
		ret += 3;
	else if (ret > USB_MARKER) {
		// =='d' keeps current ret
		if (*(c+1) == 'h')
			ret++;
		if (*(c+2) == '-')
			ret += 2;
	} 
	else if (ret > ENTER_EXIT_MARKER) { //volume or speed
		if (*(c+1) == '-')
			ret += 1;
		else if (*(c+1) == 'N') // N=normal (volume or speed)
			ret += 2;
		else if (*(c+1) == 'M') // M=max speed
			ret += 3;
	}
	else if (ret == RECORD_TITLE) {
		// == 't' keeps current ret
		if (*(c+1) == 'm')
			ret += 1;
		else if (*(c+1) == 'p')
			ret += 2;
		else if (*(c+1) == 'l')
			ret += 3;
	}
	else if (ret == JUMP_LIST) {
		if (*(c+1) == 'n')
			ret += 1;
		else if (*(c+1) == 'o')
			ret += 2;
		else if (*(c+1) == '?')
			ret += 3;
		else if (*(c+1) == 'c')
			ret += 4;
		else if (*(c+1) == '!')
			ret += 5;	
	}
	else if (ret == DELETE) {
		if (*(c+1) == 'm')
			ret += 1;
		if (*(c+1) == 't')
			ret += 2;
	}
	else if (ret == COPY) {
		if (*(c+1) == 'l')  //clone
			ret += 1;
	}
	else if (ret == SURVEY_TAKEN) {
		if (*(c+1) == 'a')
			ret += 1;
		else if (*(c+1) == 'u')
			ret += 2;
	}
	return ret;	
}

static int buildEnterExitCode (int currentLEDcode, EnumAction actionCode) {
	// this is only called by setEnterCode() and setExitCode() to do work common to both
	int LEDcode, green, red, onoff, fliponoff, ret;
	
	ret = -1;	
	if (actionCode >= ENTER_EXIT_MARKER && actionCode < EOL_MARKER) {
		if (actionCode >= VOLUME_UP && actionCode <= VOLUME_NORMAL) {
			actionCode = actionCode - VOLUME_UP + 1; // 0 = do nothing
			actionCode <<= 6; // push to bits 6-7
			currentLEDcode &= ~0xC0; // clear bits to set (6&7)
			currentLEDcode |= actionCode;
		} else if (actionCode >= SPEED_UP && actionCode <= SPEED_NORMAL) {
			actionCode = actionCode - SPEED_UP + 1; // 0 = do nothing
			actionCode <<= 4; // push to bits 4-5
			currentLEDcode &= ~0x30; // clear bits to set (4&5)
			currentLEDcode |= actionCode;				
		} else if (actionCode == USB_DEVICE_ON) {
//			if (actionCode == USB_DEVICE_ON) 
				actionCode = 0x08; 
//			else
//				actionCode == 0x03; // no LEDs and no USB and no hyperlink
			currentLEDcode &= ~0x0F; // clear bits 0-3
			currentLEDcode |= actionCode;
			}			
		else {
			LEDcode = currentLEDcode & 0x0F; // get current LED code;
			if (LEDcode != 0x08 && LEDcode != 0x04) {
				// do not override existing USB or Hyperlink setting
				// LEDs on and off
				green = LEDcode & 0x01;
				red = LEDcode & 0x02;
				onoff = LEDcode & 0x04;
				fliponoff = LEDcode & 0x08;
				switch (actionCode) {
					case LED_GREEN_ON:
						if (red && onoff)  // red ON
							LEDcode = 0x07; // first three bits on
						else if (red && !onoff) // red off
							LEDcode = 0x09; // sets fliponoff and green (forcing red off)
						else if (!red) // red doesn't change
							LEDcode = 0x05; // set green & onoff
						break;
					case LED_GREEN_OFF:
						if (red && onoff)  // red ON
							LEDcode = 0x0A; // sets fliponoff and red (forcing green off)
						else if (red && !onoff) // red off
							LEDcode = 0x03; // sets red, green and clear onoff
						else if (!red) // red doesn't change
							LEDcode = 0x01; // set green & clear onoff, forcing green off
						break;
					case LED_RED_ON:
						if (green && onoff)  // green ON
							LEDcode = 0x07; // first three bits on
						else if (green && !onoff) // green off
							LEDcode = 0x0A; // sets fliponoff and red (forcing green off)
						else if (!green) // green doesn't change
							LEDcode = 0x06; // set red & onoff
						break;
					case LED_RED_OFF:
						if (green && onoff)  // green ON
							LEDcode = 0x09; // sets fliponoff and green (forcing red off)
						else if (green && !onoff) // green off
							LEDcode = 0x03; // sets red, green and clear onoff
						else if (!green) // green doesn't change
							LEDcode = 0x02; // set red & clear onoff, forcing red off
						break;
					case LED_ALL_ON:
						LEDcode = 0x07; // first three bits on
						break;
					case LED_ALL_OFF:
						LEDcode = 0x03; // sets red, green and clear onoff
						break;	
					default:
						logException(1,0,(context.package == &pkgSystem)?USB_MODE:RESET);
						//nothing should enter here
						break;
				} // switch
				currentLEDcode &= ~0x0F; // clear bits 0-3
				currentLEDcode |= LEDcode; // set bits 
			}	// if (LEDcode != 0x08 && LEDcode != 0x04) 
		}  // last else of main if
		ret = currentLEDcode;
	} // if > ENTER_EXIT_MARKER
	return ret;
}

static void setEnterCode(CtnrBlock *block, EnumAction actionCode) {
	int enterExitCode, enterCode;
	
	enterExitCode = block->actionEnterExit;
	enterCode = enterExitCode & 0x00FF;
	enterCode = buildEnterExitCode(enterCode,actionCode);
	enterExitCode &= ~0x00FF;  //clear bits 0-15
	enterExitCode |= enterCode; //set bits
	block->actionEnterExit = enterExitCode;
}

static void setExitCode(CtnrBlock *block, EnumAction actionCode) {
	int enterExitCode, exitCode;
	
	enterExitCode = block->actionEnterExit;
	exitCode = (enterExitCode & 0xFF00) >> 8;
	exitCode = buildEnterExitCode(exitCode,actionCode);
	enterExitCode &= ~0xFF00;  //clear bits 16-31
	enterExitCode |= (exitCode<<8); //set bits
	block->actionEnterExit = enterExitCode;
}

static BOOL parseCreateAction (char *line, Action *action, int *actionCount, char *symbolMapStart, CtnrBlock *block, ListItem *list, TranslationList *transList) {	
	//TODO:
	//    Add code to parse speed changes in actions (already incorporated into Action, but nowhere else yet)
	//
	//block is NULL when parsing package actions; otherwise it is used to set block startEndAction
	int ret = TRUE; //todo: add exception handling
	char *strEvents, *delimeter, *strAction, *cursor;
	long l;
	int eventCode, actionCode, index;
	BOOL inLabel, whenPaused, whenHeld;
		
	(*actionCount)++;
	strEvents = line + 1; // move past the 'A'
	delimeter = strchr(strEvents,DELIMITER);
	strAction = delimeter + 1;
	while (*strAction && isspace(*strAction))
		strAction++;

	eventCode = getEventEnumFromChar(strEvents);

	// skip whitespace
	for (strEvents++;*strEvents && isspace(*strEvents);strEvents++);

	// check for onlyWhenPaused marker
	if (strEvents < delimeter) {
		whenPaused = (*strEvents == '|') ||  (*(strEvents+1) == '|') ;
		whenHeld = (*strEvents == '_') ||  (*(strEvents+1) == '_');
	} else 
		whenPaused = whenHeld = FALSE;
	setEventCodes(action + *actionCount, eventCode, whenPaused, whenHeld);

	// skip whitespace
	for (strEvents++;*strEvents && isspace(*strEvents);strEvents++);
	
	//work on action
	//but first check if there's a sound insert
	actionCode = getActionEnumFromChar(strAction);
	if (actionCode == INSERT_SOUND) {
		int *pStartEndCode;
		//process block to be sound-inserted before action is processed
		index = getIndexFromLine(++strAction,symbolMapStart);
		action[*actionCount].aux = index;  // as long as the index < 2^11-1, a CALL action's rewind bits won't overwrite (see below)
		setSoundInsert(&action[*actionCount], TRUE);
		if (list)
			pStartEndCode = &list->actionStartEnd;
		else if (transList)
			pStartEndCode = &transList->actionStartEnd;
		else
			pStartEndCode = &block->actionStartEnd;
		if (eventCode == START)
			setStartCode(pStartEndCode,INSERT_SOUND);
		if (eventCode == END)
			setEndCode(pStartEndCode,INSERT_SOUND);
		actionCode = -1; // reset so that we can check if we have a new action
 		//move strAction to be ready for action Code
		for (;*strAction && *strAction != ']';strAction++);
		if (*strAction) //move just past the ']' and skip whitespace
			for (strAction++;*strAction && isspace(*strAction);strAction++);
		if (*strAction)
			actionCode = getActionEnumFromChar(strAction);
/* 		CODE BELOW IS BAD BECAUSE IT OVERWRITES BLOCK START/END ACTIONS SET ON BLOCK LINE AND NO LONGER NECESSARY.
		if ((!*strAction || actionCode == -1) 
				&& (eventCode1 == START || eventCode1 == END) && (eventCode2 == START || eventCode2 == END)) {
			actionCode = INSERT_SOUND;
			setActionCode(action + *actionCount, actionCode);
		}  
		else
	  		ret = FALSE; */
	} 
	if (actionCode != -1) {
		setActionCode(action + *actionCount,actionCode);
		// actions like STOP, PAUSE, RETURN, FWD, BACK don't require parameters
		if (actionCode == JUMP_TIME || actionCode == CALL_BLOCK) {
			// get to numbers, but skip numbers inside any label
			inLabel = FALSE;
			for (cursor=strAction;*cursor;cursor++) {
				if (*cursor == '[')
					inLabel = TRUE;
				else if (*cursor == ']')
					inLabel = FALSE;
				if (!inLabel && (isdigit(*cursor) || *cursor == '-' || *cursor == '+'))
					break;
			}					
			if (*cursor) {
				l = strToLong(cursor);
				switch (actionCode) {
					case JUMP_TIME:
						action[*actionCount].destination = compressTime(l, context.package->timePrecision);
						break;
					case CALL_BLOCK:
						setRewind(&action[*actionCount].aux,(int)l);
						break;
					default:  /// not used now; was originally used for VOLUME and SPEED
						action[*actionCount].destination = (int)l;
						break;
				}
			} else if (actionCode == CALL_BLOCK)
				// no specified rewind time, so use default
				setRewind(&action[*actionCount].aux,-DEFAULT_REWIND);
		}					
		if (actionCode == DELETE || actionCode == DELETE_MESSAGES || actionCode == COPY || actionCode == TRIM || 
				actionCode == SURVEY_TAKEN || actionCode == SURVEY_APPLY || actionCode == SURVEY_USELESS || 
				actionCode == POSITION_TO_TOP || actionCode == MAKE_FAVORITE) {
			strAction++;
			if (actionCode == SURVEY_TAKEN || actionCode == SURVEY_APPLY || 
				actionCode == SURVEY_USELESS || actionCode == DELETE_MESSAGES)
				strAction++; // skip the 't', 'a', or 'u' or 'm'
			while (isspace(*strAction)) strAction++;
			if (*strAction == '{') { // variable-based package
				cursor = strchr(strAction,'}');
				if (cursor)
					*cursor = 0;			
				else {
						logException(8,strAction,0);  // syntax error in control track
						ret = FALSE;
				}
				strAction++;
				index = getIndexFromLine(strAction,symbolMapStart);
				if (index != -1) {
					action[*actionCount].destination = index; 
				}
				else {
					ret = FALSE;
					logException(6,strAction,0);  // todo:invalid internal reference in control track file
				}
			}
			else if (actionCode == POSITION_TO_TOP) // otherwise no parameter is legit for pushing language to top
				action[*actionCount].destination = -1;
			else  { 
				ret = FALSE;
				logException(8,strAction,0);  // syntax error in control track
			}
			if (actionCode != TRIM) {
				// TRIM doesn't use aux
				cursor++;
				index = getIndexFromLine(cursor,symbolMapStart);
				if (index != -1)
					action[*actionCount].aux = index; 		
			}	
		}
		if (actionCode == JUMP_BLOCK || actionCode == CALL_BLOCK || actionCode == CLONE ||
			actionCode == RECORD_TITLE || actionCode == RECORD_MSG || actionCode == PACKAGE_RECORDING || 
			actionCode == USB_DEVICE_ON || actionCode == USB_HOST_ON) {
			if (actionCode == RECORD_TITLE || actionCode == RECORD_MSG) {
				// check if recording from headphone port instead of a microphone
				if (*(strAction+2) == '-') {  // moves past 'E' and 't'/'m'
					action[*actionCount].aux = 1;
					strAction += 3;
				} else
					action[*actionCount].aux = 0;	
			}
			index = getIndexFromLine(strAction,symbolMapStart);
			if (index != -1)
				action[*actionCount].destination = index; 
			//else {
			//	ret = FALSE;
			//	logException(6,strAction,0);  // todo:invalid internal reference in control track file
			//}
// TODO:If (index==-1), there was no destination ("[]"), which is ok for now -- and ret=FALSE was ignored anyway   
//			else
//				ret = FALSE;
		}
		if (actionCode == JUMP_PACKAGE) {
			strAction++;
			if (*strAction == '{') { // variable-based package
				cursor = strchr(strAction,'}');
				if (cursor)
					*cursor = 0;			
				strAction++;
				index = getIndexFromLine(strAction,symbolMapStart);
				if (index != -1) {
					action[*actionCount].destination = index; 
					action[*actionCount].aux = PKG_VARIABLE;
				}
				else {
					ret = FALSE;
					logException(6,strAction,0);  // todo:invalid internal reference in control track file
				}
			} else { // explicit name
				if (*strAction == SYS_PKG_CHAR) {
					action[*actionCount].aux = PKG_SYS;
					strAction++;
				} else if (*strAction == APP_PKG_CHAR) {
					action[*actionCount].aux = PKG_APP;
					strAction++;
				} else
					action[*actionCount].aux = PKG_MSG;
				cursor = strchr(strAction,'>');
				if (cursor == strAction) // "<%>" indicates current system package
					action[*actionCount].destination = SAME_SYSTEM;
				else {
					if (*strAction == '+')
						action[*actionCount].destination = NEXT_SYSTEM;
					else if (*strAction == '-')
						action[*actionCount].destination = PREV_SYSTEM;
					else
						logException(8,strAction-2,USB_MODE);
				}
			}
		}
		if (actionCode == JUMP_LIST) {
			strAction++;
			switch (*strAction) {
				case '+':
					setListRotation(&action[*actionCount].aux, 1);
					strAction++;
					break;
				case '-':
					setListRotation(&action[*actionCount].aux, -1);
					strAction++;
					break;
				default:
					action[*actionCount].aux = 0;	
			}
			index = getIndexFromLine(strAction,symbolMapStart);
			if (index != -1)	
				action[*actionCount].destination = index; 
			else {
				ret = FALSE;
				logException(6,strAction,0);  // todo:invalid internal reference in control track file 
			}
		}
		if (actionCode == DELETE_TRANSLATION || actionCode == WRAP_TRANSLATION) {
			//strAction = strAction+3;
			index = getIndexFromLine(strAction,symbolMapStart);
			if (index != -1)	
				action[*actionCount].destination = index; 
			else {
				ret = FALSE;
				logException(6,strAction,0);  // todo:invalid internal reference in control track file 
			}
		}
		if (actionCode == TRANSLATE_NEW){
			//Get first block and store in destination
			//Move past 'c' (c for create)
			strAction=strAction+2;
			while (*strAction && isspace(*strAction))
				strAction++;
			
			if (*strAction == '[') { 
				cursor = strchr(strAction,']');
				index = getIndexFromLine(strAction,symbolMapStart);
				if (index != -1) {
					action[*actionCount].destination = index; 
					strAction = cursor+1;
				}
				else {
					ret = FALSE;
					logException(6,strAction,0);  // todo:invalid internal reference in control track file
				}
			}
			else {
				ret = FALSE;
				logException(8,strAction,0);  // syntax error in control track
			}
		}
		if (actionCode == TRANSLATE_OVERWRITE) {
			//Get first block and store in destination
			//Move past '!'
			strAction=strAction+2;
			while (*strAction && isspace(*strAction))
				strAction++;
			
			if (*strAction == '[') { 
				cursor = strchr(strAction,']');
				index = getIndexFromLine(strAction,symbolMapStart);
				if (index != -1) {
					action[*actionCount].destination = index; 
					strAction = cursor+1;
				}
				else {
					ret = FALSE;
					logException(6,strAction,0);  // todo:invalid internal reference in control track file
				}
			}
			else {
				ret = FALSE;
				logException(8,strAction,0);  // syntax error in control track
			}
		}
		if (actionCode == TRANSLATE_DELETE_FINISH) {
			//Get first block and store in destination
			//Move past ?
			strAction=strAction+2;
			while (*strAction && isspace(*strAction))
				strAction++;
			
			if (*strAction == '[') { 
				cursor = strchr(strAction,']');
				//if (cursor)
				//	*cursor = 0;			
				//else {
				//	logException(8,strAction,0);  // syntax error in control track
				//	ret = FALSE;
				//}
				//strAction++;
				index = getIndexFromLine(strAction,symbolMapStart);
				if (index != -1) {
					action[*actionCount].destination = index; 
					strAction = cursor+1;
				}
				else {
					ret = FALSE;
					logException(6,(char *)"here1",0);  // todo:invalid internal reference in control track file
				}
			}
			else {
				ret = FALSE;
				logException(8,strAction,0);  // syntax error in control track
			}
			// Store second block name in aux
			while (*strAction && isspace(*strAction))
				strAction++;
			
			if (*strAction == '[') { 
				//cursor = strchr(strAction,']');
				//if (cursor)
				//	*cursor = 0;			
				//else {
				//	logException(8,strAction,0);  // syntax error in control track
				//	ret = FALSE;
				//}
				//strAction++;
				index = getIndexFromLine(strAction,symbolMapStart);
				if (index != -1) {
					action[*actionCount].aux = index; 
				}
				else {
					ret = FALSE;
					logException(6,(char *)"here1",0);  // todo:invalid internal reference in control track file
				}
			}
			else {
				ret = FALSE;
				logException(8,strAction,0);  // syntax error in control track
			}	
		
			/////////
		}
	}		
	if ((block || list || transList) && actionCode != -1) {
		int *pStartEndCode;
		if (list)
			pStartEndCode = &list->actionStartEnd;
		else if (transList)
			pStartEndCode = &transList->actionStartEnd;
		else
			pStartEndCode = &block->actionStartEnd;	
		if (eventCode == START)
			setStartCode(pStartEndCode,actionCode);
		if (eventCode == END)
			setEndCode(pStartEndCode,actionCode);
	}
	return ret;
}

static BOOL parseCreateBlock (char *line, CtnrBlock *block, char **freeSymbolMap, int idxBlock) {

	int ret;
	char *cursor;
 	unsigned long lStartTime, lEndTime;
	EnumAction blockAction;
	BOOL inLabel;
	
	ret = TRUE;  // TODO: set ret = 0 if there's not even a single time or no '-'  
	
	block->idxFirstAction = -1;
	block->idxNextStart = MASK_IDX_NEXT_START;
	block->idxNextEnd = -1;

	cursor = strchr(line,DELIMITER);
	cursor++; //either number or whitespace immediately following DELIMITER
	
	while (*cursor && isspace(*cursor))
		cursor++;
	
	//check for block-start action (of type EnumAction)
	//todo: don't get tricked into seeing digits in [labels]
	while (*cursor && !isdigit(*cursor) && *cursor != '-') {
		if (!isspace(*cursor)) {
			blockAction = getActionEnumFromChar(cursor);
			if (blockAction >= 0)
				setStartCode(&block->actionStartEnd,blockAction);
			//todo:else exception and log
		}
		cursor++;
	}

	if (*cursor == '-')	
		lStartTime = 0; //nothing before '-' means block begins at beginning of file
	else
		lStartTime = strToLong(cursor);
	//reduce the precision to convert long to int ans space space
	block->startTime = compressTime(lStartTime, context.package->timePrecision);
		
	cursor = strchr(cursor,'-');
	do //either number or whitespace immediately following '-'
		cursor++; 
	while (*cursor && isspace(*cursor));
	
	if (isdigit(*cursor))
		lEndTime = strToLong(cursor);
	else 
		lEndTime = 0xFFFFFFFF; // nothing after '-' means block runs to end of file
	//reduce the precision to convert long to int ans space space
	block->endTime = compressTime(lEndTime, context.package->timePrecision);

	if (block->startTime > block->endTime)
		ret = FALSE; //todo:log this problem
	
	while (*cursor && (isspace(*cursor) || isdigit(*cursor)))
		cursor++;
	
	//check for block-end action (of type EnumAction)
	while (*cursor && *cursor != '[' && *cursor != '/') {
		if (!isspace(*cursor)) {
			blockAction = getActionEnumFromChar(cursor);
			if (blockAction != -1)
				setEndCode(&block->actionStartEnd,blockAction);
		}
		cursor++;
	}
	
	// check if block is hyperlinked
	inLabel = FALSE;
	for (cursor=line;*cursor;cursor++) {
		if (*cursor == '[')
			inLabel = TRUE;
		else if (*cursor == ']')
			inLabel = FALSE;
		if (!inLabel && *cursor == '?') {
			setBlockHyperlinked(block);
			break;
		}
	}					

	// check if block has enter/exit events
	cursor = strchr(line,'/');
	if (cursor != NULL) {
		// go backwards to beginning of legit enter/exit codes
		do {
			cursor--;
			if (*cursor != '+' && *cursor != '-')
				blockAction = getEnterExitEnumFromChar(cursor); 
		} while (blockAction != -1 || *cursor == '+' || *cursor == '-' || *cursor == 'N');
		//read in enter codes
		for (cursor++;*cursor != '/';cursor++) {
			if (*cursor != '-' && *cursor != '+' && *cursor != 'N') {
				blockAction = getEnterExitEnumFromChar(cursor);
				setEnterCode(block,blockAction);
			}
		}
		// must be back to '/'; now read in exit codes
		do {
			cursor++;
			if (*cursor && *cursor != '-' && *cursor != '+' && *cursor != 'N') {
				blockAction = getEnterExitEnumFromChar(cursor);
				if (blockAction != -1)
					setExitCode(block,blockAction);
			}
		} while (*cursor && (blockAction != -1 || *cursor == '+' || *cursor == '-' || *cursor == 'N'));
	}
			
	cursor = strchr(line,']');
	// a block does not need a symbol if nothing refers to it
	if (cursor != NULL) {
		//set null marker before ] but also before any whitespace
		while (isspace(*(cursor-1)))
			cursor--;
		*cursor = '\0';
		cursor = strchr(line,'[');
		// todo: error if there is a ] but no [
		do
			cursor++;
		while (isspace(*cursor));
		cursor = strcpy(*freeSymbolMap,cursor);
		cursor = strchr(cursor,'\0');
		*(cursor + 1) = idxBlock;
		*freeSymbolMap = cursor + 2;
	}
	return ret;
}

static int getBlockIndexFromSymbol (char *symbolMapBegin, const char *symbol) {

	char *cursor;
	char *symbolMapEnd;
	BOOL foundSymbol;
	int ret;
	
	ret = -1; //error code
	symbolMapEnd = symbolMapBegin + (MAX_BLOCKS * AVG_SYMBOL_LENGTH) - 1; // to match array size declared in loadPackage
	foundSymbol = FALSE;
	
	while (!foundSymbol && symbolMapBegin < symbolMapEnd) {
		foundSymbol = (0 == strcmp(symbolMapBegin,symbol));
		cursor = (strchr(symbolMapBegin,'\0') + 1); // go to char afer null char either way
		if (foundSymbol)
			ret = *cursor; // gets block index for symbol	
		else
			symbolMapBegin = ++cursor; //advaces to next symbol
	}		
	return ret;
}

static unsigned int getLinkedTime(OrderBlock *orderBlock, EnumStartOrEnd whichTime) {
	unsigned int time;
	int idxBlock;
	
	idxBlock = orderBlock->idxBlock;
	if (whichTime == STARTING)
		time = context.package->blocks[idxBlock].startTime;
	else //whichTime == ENDING
		time = context.package->blocks[idxBlock].endTime;
	return time;
}

static void insertOrderBlock (OrderBlock *orderBlock, OrderBlock **latest, unsigned int blockTime, EnumStartOrEnd whichTime) {
	
	OrderBlock *orderCursor, **earlierLink;
	
	if (*latest == NULL) {
		*latest = orderBlock;
		(*latest)->earlier = NULL;
	}
	else {
		orderCursor = *latest;
		earlierLink = latest;
		while (1) {
			if (getLinkedTime(orderCursor,whichTime) <= blockTime) {
				orderBlock->earlier = orderCursor;
				*earlierLink = orderBlock;
				break;
			}
			if (orderCursor->earlier == NULL) {
				orderCursor->earlier = orderBlock;
				orderBlock->earlier = NULL;
				break;	
			}
			earlierLink = &orderCursor->earlier;
			orderCursor = orderCursor->earlier;										
		} 
	}
}

static void setBlockOrder(CtnrPackage *pkg, OrderBlock *latestStart, OrderBlock *latestEnd) {
	OrderBlock *cursor;
	int currentBlock, nextBlock;
	
	// connect blocks with idxNextStart/idxNextEnd from orderBlocks[]
	// start with connecting the nextStart links
	cursor = latestStart;
	nextBlock = -1;
	do {
		currentBlock = cursor->idxBlock;
		setNextStartBlock(currentBlock, nextBlock);
		nextBlock = currentBlock;
		cursor = cursor->earlier;
	} while (cursor != NULL);
	pkg->files[pkg->countFiles - 1].idxFirstBlockStart = currentBlock;
	
	// now connect the nextEnd links
	cursor = latestEnd;
	nextBlock = -1;
	do {
		currentBlock = cursor->idxBlock;
		pkg->blocks[currentBlock].idxNextEnd = nextBlock;
		nextBlock = currentBlock;
		cursor = cursor->earlier;
	} while (cursor != NULL);		
	pkg->files[pkg->countFiles - 1].idxFirstBlockEnd = currentBlock;
}

int addTextToPkgHeap (const char *line, CtnrPackage *pkg) {
	int startingHeap, length;
	char *heapCursor, *lineCursor;

	while (isspace(*line) && *line != 0)
		line++;
	lineCursor = strchr(line,0); 
	while (isspace(*(lineCursor-1)) && lineCursor >= line)
		lineCursor--;
	*lineCursor = 0;
	length = (lineCursor - line + 1);
	startingHeap = pkg->idxStrHeap;
	pkg->idxStrHeap = startingHeap + length;
	if (pkg->idxStrHeap > PKG_HEAP_SIZE)
		logException(11,0,(context.package == &pkgSystem)?USB_MODE:RESET); //todo:heap is full
	heapCursor = &(pkg->strHeapStack[startingHeap]);
	strcpy(heapCursor,line);
	return startingHeap;
}

static int getEnterExitEnumFromChar (char *c) {
	int ret;
	
	ret = getActionEnumFromChar(c);
	if (!(ret > ENTER_EXIT_MARKER))
		ret = -1;	
	return ret;
}

void parseControlFile (char * filePath, CtnrPackage *pkg) {
	//symbolMap array size allows for avg symbol length of 10 plus room for an int index to block index
	char *pSymbolMap;
	int currentBlock, currentList, actionCount, currentOrderBlock; 
	int ret; // reused return value check
	int fileHandle;
	BOOL firstBuffer;
	BOOL needPkgActions;
	char *line;
	char *charCursor;
	char *charCursor2;
	OrderBlock *latestStart, *latestEnd;
	char currentContainerType; // P = package; F = file; B = block; L = List (of lists or packages); T = Translation List
	int lineCount;
	ListItem *list;
	TranslationList *transList;
	OrderBlock orderBlocks[MAX_BLOCKS*2];
	char buffer[READ_LENGTH+1];
	char tempBuffer[50];
	char symbolMap[MAX_BLOCKS * AVG_SYMBOL_LENGTH]; 
	int attempt, goodPass;
	//int tempListNum;
	const int MAX_ATTEMPTS = 3;
	
	buffer[READ_LENGTH] = '\0'; //prevents readLine from searching for \n past buffer
	
	for (attempt = 0, goodPass = 0; attempt < MAX_ATTEMPTS && !goodPass; attempt++) {
		// loop to deal with error #8 when no pkg->countFiles == 0
		// it appears that this might be due to a bad read, so we'll retry

		if (attempt) // more than one attempt
			logException(26,0,0);

		goodPass = 1;
		lineCount = -1;  // to make it 0-based
		currentBlock = -1; // haven't started with 0 yet
		actionCount = -1;
		pSymbolMap = symbolMap; //tracks top of heap
		firstBuffer = TRUE;
		latestStart = NULL;
		latestEnd = NULL;
		currentContainerType = 0;	
		pkg->idxLanguageCode = -1;
	
		fileHandle = tbOpen((LPSTR)(filePath),O_RDONLY);
		if (fileHandle == -1) {
			goodPass = 0;
			break; // tbOpen already retries before returning, so give up and handle as if retried
		} else {
			getLine(-1,0); // resets fct to beginning of file
			while ((line = getLine(fileHandle,buffer))) {
				if (!goodString(line,0)) {
					goodPass = 0;
					continue;
				}
				lineCount++;
				if (!currentContainerType) {
					// haven't entered into package container yet
					// check for preface settings
					charCursor = strstr(line,"TIME-PRECISION"); //TODO: move to case-insensitive fct
					if (!charCursor)
					    charCursor = strstr(line,"time-precision"); 
					if (charCursor) {
						charCursor = strchr(line,DELIMITER);
						if (charCursor) {
							pkg->timePrecision = getBitShift((int)strToLong(++charCursor));
							continue;
						}
					}
					charCursor = strstr(line,"APP"); //TODO: handle all this in a const array of application strings
					if (!charCursor)
					    charCursor = strstr(line,"app"); 
					if (charCursor) {
						if ((charCursor = strchr(line,DELIMITER))) {
							charCursor++;
							if (strstr(line,APP_QUIZ_PLAY_STR)) 
								pkg->app_type = APP_QUIZ_PLAY;
							else if (strstr(line,APP_QUIZ_REC_STR)) 
								pkg->app_type = APP_QUIZ_REC;
							else
								pkg->app_type = APP_CUSTOM;
						}
					}
				}
				// parse line if ended with \n or if last line of last buffer	
				switch (*line) {
					case 'P':
						currentContainerType = 'P';
						// if time precision was not specified (see above), use default
						if (!pkg->timePrecision)
							pkg->timePrecision = getBitShift(DEFAULT_TIME_PRECISION);
						// Setting package idxName with list's package name, not the package name in the control track
/*						charCursor = strchr(line,DELIMITER) + 1;
						if (*charCursor != 0) {
							ret = addTextToPkgHeap (charCursor,pkg);
							if (ret > -1) {
								pkg->idxName = ret;											
							}
						}
*/						break;
					case 'L':
						charCursor = strchr(line,DELIMITER) + 1;
						if (*charCursor == 'T') {
							currentContainerType = 'T';
							transList = &context.transList;
							transList->idxFirstAction = -1;
							transList->currFileIdx = -1;
							transList->mode = '0';
							
						}
						else {
							currentContainerType = 'L';
							pkg->countLists++;
							if (pkg->countLists > MAX_LISTS) {
								close(fileHandle);
								strcpy(tempBuffer,"ListItem#");
								longToDecimalString((long)pkg->countLists,tempBuffer+strlen(tempBuffer),2);
								logException(2,tempBuffer,(context.package == &pkgSystem)?USB_MODE:RESET); // todo: too many files, blocks, or lists
							}

							// now prep new ListItem container
							list = &pkg->lists[pkg->countLists-1];
							list->idxFirstAction = -1;
							list->currentFilePosition =  -1;
						}
						// first wrap up last File container, if any
						if (pkg->countFiles && latestStart) 
							setBlockOrder(pkg,latestStart,latestEnd);
						//charCursor = strchr(line,DELIMITER) + 1;
						if (*charCursor == 'L') {
							list->listType = LIST_OF_LISTS;
							context.package->idxMasterList = pkg->countLists-1;	
						}
						else if (*charCursor == 'P')
							list->listType = LIST_OF_PACKAGES;
						else if (*charCursor != 'T') {
							close(fileHandle);
							logException(8,line,(context.package == &pkgSystem)?USB_MODE:RESET);//syntax error
						}
						charCursor++;
						if (*charCursor != '[') 
							charCursor++; // skip any single char delimiter before list name, unless already at label
						while (*charCursor && isspace(*charCursor))
							charCursor++;
						if (*charCursor != 0) {
							if (currentContainerType == 'L' && *charCursor == '{') {
								charCursor++;
								ret = getIndexFromLine(charCursor,symbolMap);
	 							if (ret != -1) {	
									list->idxListWithFilename = ret;
									list->filename[0] = 0;
	 							}
								else {
									logException(6,charCursor,(context.package == &pkgSystem)?USB_MODE:RESET);  // todo:invalid internal reference in control track file 
								}
	 							charCursor = strchr(charCursor,'}');
	 							charCursor = strchr(charCursor,'[') + 1;
							}
							else {
								charCursor2 = strchr(charCursor,'[');
								if (charCursor2 == NULL) {
									close(fileHandle);
									strcpy(tempBuffer,"ListItem#");
									longToDecimalString((long)pkg->countLists,tempBuffer+strlen(tempBuffer),2);
								    logException(8,tempBuffer,(context.package == &pkgSystem)?USB_MODE:RESET); //todo: syntax error in control track
								} else if (charCursor == charCursor2){
									// '[' immediate follows list type -- no list filename
									//if (list->listType == LIST_OF_LISTS) { // no list filename needed for Lists of Lists (they use config master list)
									if (currentContainerType == 'L' && list->listType == LIST_OF_LISTS) { // no list filename needed for Lists of Lists (they use config master list)
										strcpy(list->filename,LIST_MASTER);
										charCursor++; //advance cursor from '[' to first character of list label
									}
									else if(currentContainerType == 'T') {
										charCursor++; //advance cursor from '[' to first character of list label
									}
									else {
										// Lists of Packages must have list filename
										strcpy(tempBuffer,"No list name");
										longToDecimalString((long)pkg->countLists,tempBuffer+strlen(tempBuffer),2);
									    logException(8,tempBuffer,(context.package == &pkgSystem)?USB_MODE:RESET); //todo: syntax error in control track
									}
								} else if (currentContainerType == 'L') {
									while (isspace(*(charCursor2-1)) && charCursor2 >= charCursor)
										charCursor2--;							
									*charCursor2++ = 0; // set null mark and move back to space or [
									strcpy(list->filename,charCursor);
									charCursor = charCursor2;  
									for (;isspace(*charCursor);charCursor++); // move cursor back to '['
									charCursor++; // move to first character of label
								}
							}
							charCursor2 = strchr(charCursor,']');
							if (charCursor2 != NULL)
								*charCursor2 = 0;
							else  {
								close(fileHandle);
								logException(8,charCursor,(context.package == &pkgSystem)?USB_MODE:RESET); //todo: syntax error in control track
							}
							//todo: separate list and block namespaces?
							charCursor = strcpy(pSymbolMap,charCursor);
							charCursor = strchr(charCursor,'\0');
							if(currentContainerType == 'T') {
								*(charCursor + 1) = MAX_LISTS;
								//*(charCursor + 1) = 2;
								//tempListNum = *(charCursor + 1);
								//strcpy(tempBuffer,"TranslationList#");
								//longToDecimalString((long)tempListNum,tempBuffer+strlen(tempBuffer),2);
								//logException(99,tempBuffer,(context.package == &pkgSystem)?USB_MODE:RESET); // todo: too many files, blocks, or lists
							}
							else 
								*(charCursor + 1) = pkg->countLists-1;
							pSymbolMap = charCursor + 2;
							if ((pSymbolMap - symbolMap) > (MAX_BLOCKS * AVG_SYMBOL_LENGTH)) {
								close(fileHandle);
								logException(7,0,(context.package == &pkgSystem)?USB_MODE:RESET); // todo: internal symbol heap is full							
							}
						}
						break;
					case 'F':
						currentContainerType = 'F';
						// first wrap up last File container, if any
						if (pkg->countFiles && latestStart) 
							setBlockOrder(pkg,latestStart,latestEnd);
									
						// now prep new File container
						charCursor = strchr(line,DELIMITER) + 1;
						if (*charCursor != 0) {
							if (*charCursor == '{') {
								if (*++charCursor == '$' && *++charCursor == 'L')
									pkg->files[pkg->countFiles++].idxFilename = -1; // signal of lastFileRecorded;
							} else { 
								latestStart = NULL;
								latestEnd = NULL;
								ret = addTextToPkgHeap (charCursor,pkg);
								if (ret != -1) {
									pkg->files[pkg->countFiles++].idxFilename = ret;
									currentOrderBlock = -1;
									if (pkg->countFiles > MAX_FILES) {
										close(fileHandle);
										strcpy(tempBuffer,"File#");
										longToDecimalString((long)pkg->countFiles,tempBuffer+strlen(tempBuffer),3);
										logException(2,tempBuffer,(context.package == &pkgSystem)?USB_MODE:RESET); // todo: too many files, blocks, or lists
									}
								}
								else
									logException(4,0,(context.package == &pkgSystem)?USB_MODE:RESET); //todo: text heap is full
							}
						}
						break;
					case 'B':
						currentBlock++;
						if (currentBlock > MAX_BLOCKS) {
							strcpy(tempBuffer,"Block#");
							longToDecimalString((long)currentBlock,tempBuffer+strlen(tempBuffer),3);							
							logException(2,tempBuffer,(context.package == &pkgSystem)?USB_MODE:RESET); // todo: too many actions, files, blocks, or lists
						}
						// check if first block in file
						if (currentContainerType == 'F') 
							pkg->files[pkg->countFiles-1].idxFirstBlockInFile = currentBlock;
						currentContainerType = 'B';
						if (parseCreateBlock(line,&pkg->blocks[currentBlock],&pSymbolMap,currentBlock)) {
							if ((pSymbolMap - symbolMap) > (MAX_BLOCKS * AVG_SYMBOL_LENGTH))
								logException(7,0,(context.package == &pkgSystem)?USB_MODE:RESET); // todo: internal symbol heap is full
							// prepare new orderBlock, but don't know where to link it yet
							orderBlocks[++currentOrderBlock].idxBlock = currentBlock;
							insertOrderBlock (&orderBlocks[currentOrderBlock], &latestStart, pkg->blocks[currentBlock].startTime, STARTING);
							orderBlocks[++currentOrderBlock].idxBlock = currentBlock;
							insertOrderBlock (&orderBlocks[currentOrderBlock], &latestEnd, pkg->blocks[currentBlock].endTime, ENDING);								
						}
						else
							currentBlock--; //back parse, so ignore line and back up counter
						break;
					case 'A':
							// the line below does not handle package actions
							// file actions might not be necessary right now
							if (currentContainerType == 'B' && pkg->blocks[currentBlock].idxFirstAction == -1) {
								//temporarily store file line number to find first action during second pass (below)
								pkg->blocks[currentBlock].idxFirstAction = lineCount; 
							} 
							else if (currentContainerType == 'P' && !pkg->countPackageActions)
								pkg->countPackageActions = lineCount;
							else if (currentContainerType == 'L' && list->idxFirstAction == -1) {
								//temporarily store file line number to find first action during second pass (below)
								list->idxFirstAction = lineCount; 
							} 
							else if (currentContainerType == 'T' && transList->idxFirstAction == -1) {
								//temporarily store file line number to find first action during second pass (below)
								transList->idxFirstAction = lineCount; 
							} 
							break;
				} // switch
			} // while (line);
			// block order for prev file is set when a new file is declared, 
			// but this catches the last file
	
			setBlockOrder(pkg,latestStart,latestEnd);
			
			pkg->countBlocks = currentBlock + 1;  //0-based
			getLine(fileHandle,0); // resets fct to beginning of file
			lineCount = -1;
			actionCount = -1;
			currentBlock = 0;			
			// Get Package Actions
			if (pkg->countPackageActions > 0) {
				do {
					if ((line = getLine(fileHandle,buffer)))
						lineCount++;
					if (!goodString(line,0)) 
						goodPass = 0;
				} while (line && lineCount < pkg->countPackageActions);
				if (!goodPass)
					continue;
				while (line && *line == 'A') {
					ret = parseCreateAction(line, (Action *) &pkg->actions, &actionCount, (char *)&symbolMap, NULL, NULL, NULL);
					if (!ret) {
						goodPass = 0;
						logException(8,0,0); 
					}
					if (actionCount > MAX_ACTIONS) {
						close(fileHandle);
						strcpy(tempBuffer,"Action#");
						longToDecimalString((long)actionCount,tempBuffer+strlen(tempBuffer),3);
						logException(2,tempBuffer,(context.package == &pkgSystem)?USB_MODE:RESET); // todo: too many actions, files, blocks, or lists
					}
					if ((line = getLine(fileHandle,buffer)))
						lineCount++;
					if (!goodString(line,0)) {
						goodPass = 0;
						continue;
					}
				} 
				setEndOfActions(&pkg->actions[actionCount],TRUE);
				pkg->countPackageActions = actionCount + 1;
				needPkgActions = FALSE;
			}  // if (pkg->countPackageActions > 0) 
			do {
				while (pkg->blocks[currentBlock].idxFirstAction == -1 && currentBlock < pkg->countBlocks)
					currentBlock++;
				do {
					if ((line = getLine(fileHandle,buffer)))
						lineCount++;
				} while (line && lineCount < pkg->blocks[currentBlock].idxFirstAction);
				// parse line if ended with \n or if last line of last buffer	
				if (line && *line == 'A') {
					//actionCount is updated within parseCreateAction
					pkg->blocks[currentBlock].idxFirstAction = actionCount + 1;
					do {
						ret = parseCreateAction(line, (Action *)&pkg->actions, &actionCount, (char *)&symbolMap, &pkg->blocks[currentBlock], NULL, NULL);
						if (ret == -1) {
							goodPass = 0;
							break;
						}
						if ((line = getLine(fileHandle,buffer)))
							lineCount++;
						if (!goodString(line,0)) {
							goodPass = 0;
							continue;
						}			
					} while (line && *line == 'A');
					setEndOfActions(&pkg->actions[actionCount],TRUE);
				}	
				currentBlock++;
			} while (line && currentBlock <= pkg->countBlocks);
			//Begin parsing for lists
			currentList = 0;		
			getLine(fileHandle,0); // resets fct to beginning of file
			line = getLine(fileHandle,buffer); // read first line
			if (!goodString(line,0)) {
				goodPass = 0;
				continue;
			}
			lineCount = 0;
			do {
				while (pkg->lists[currentList].idxFirstAction == -1 && currentList < pkg->countLists)
					currentList++;
				while (line && lineCount < pkg->lists[currentList].idxFirstAction) {
					if ((line = getLine(fileHandle,buffer)))
						lineCount++;
					if (!goodString(line,0)) {
						goodPass = 0;
						continue;
					}
				}
				// parse line if ended with \n or if last line of last buffer	
				if (line && *line == 'A') {
					//actionCount is updated within parseCreateAction
					pkg->lists[currentList].idxFirstAction = actionCount + 1;
					do {
						ret = parseCreateAction(line, (Action *)&pkg->actions, &actionCount, (char *)&symbolMap, NULL, &pkg->lists[currentList], NULL);
						//todo: handle ret = -1 or FALSE or 0 or whatever
						if ((line = getLine(fileHandle,buffer)))
							lineCount++;
						if (!goodString(line,0))
							goodPass = 0;
					} while (line && *line == 'A');	
					setEndOfActions(&pkg->actions[actionCount],TRUE);
				}	
				currentList++;
			} while (line && currentList <= pkg->countLists);
			
			//Begin parsing for Translation Lists
			getLine(fileHandle,0); // resets fct to beginning of file
			line = getLine(fileHandle,buffer); // read first line
			if (!goodString(line,0)) {
				goodPass = 0;
				continue;
			}
			lineCount = 0;
			while (line && lineCount < context.transList.idxFirstAction) {
				if ((line = getLine(fileHandle,buffer)))
					lineCount++;
				if (!goodString(line,0)) {
					goodPass = 0;
					continue;
				}
			}
			// parse line if ended with \n or if last line of last buffer	
			if (line && *line == 'A') {
				//actionCount is updated within parseCreateAction
				context.transList.idxFirstAction = actionCount + 1;
				do {
					ret = parseCreateAction(line, (Action *)&pkg->actions, &actionCount, (char *)&symbolMap, NULL, NULL, &context.transList);
					//todo: handle ret = -1 or FALSE or 0 or whatever
					if ((line = getLine(fileHandle,buffer)))
						lineCount++;
					if (!goodString(line,0))
						goodPass = 0;
				} while (line && *line == 'A');	
				setEndOfActions(&pkg->actions[actionCount],TRUE);
			}	
			
			close(fileHandle);
		} // if fileHandle >= 0 
		if (pkg->countFiles == 0)
			goodPass = 0;
	}
	if (!goodPass) 
		logException(8,filePath,(context.package == &pkgSystem)?USB_MODE:RESET); // not able to get a good read after MAX_ATTEMPTS trials
//	else {
//		logString((char *)"parsed ",BUFFER);
//		logString(filePath,ASAP);
//	}
}
