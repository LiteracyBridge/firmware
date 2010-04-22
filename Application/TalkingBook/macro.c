// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/talkingbook.h"
#include "Include/device.h"
#include "Include/files.h"
#include "Include/audio.h"
#include "Include/containers.h"
#include "Include/macro.h"
extern void KeyScan_ServiceLoop(void);
extern int SP_GetCh(void);

#define MAX_MACRO_ITEMS     100
#define MAX_MACRO_LOOPS     10
#define TOUR_OFFSET			0x100  // to keep tour audio file numbers away from keystroke values

APP_IRAM static MacroInstr macro[MAX_MACRO_ITEMS];
APP_IRAM static MacroLoop loop[MAX_MACRO_LOOPS];

void loadMacro(void) {
	const char START_LOOP = '(';
	const char END_LOOP = ')';
	const char LOG = 'L';
	
	int handle;
	char *event, *action, *cursor;
	char buffer[READ_LENGTH+1];
	int wait, key, loopCount, begin, end;
	int attempt, goodPass;
    int  idxMacro = 0, idxLoop = 0;		
    const int MAX_RETRIES = 3;

	for (attempt = 0,goodPass = 0;attempt < MAX_RETRIES && !goodPass;attempt++) {
		goodPass = 1;
		buffer[READ_LENGTH] = '\0';
		handle = tbOpen((unsigned long)(MACRO_FILE),O_RDONLY);
		if (handle == -1) {
			goodPass = 0;
			break;
		} else {
			getLine(-1,0);  // reset in case at end from prior use
			while (goodPass && nextNameValuePair(handle, buffer, ',', &event, &action)) {
				if (!goodString(event,0) || !goodString(action,0)) { 
					goodPass = 0;
					logException(31,MACRO_FILE,0);
					break; // from the while, not the for
				}
				if (idxMacro >= MAX_MACRO_ITEMS)
					break;
				if ((cursor = strchr(event,START_LOOP)) && (idxLoop < MAX_MACRO_LOOPS)) {
					// expects "5 (" to indicate the beginning of a loop with 5 iterations	
					*cursor = 0;
					loopCount = strToInt(event);
					begin = idxMacro;
					event = cursor + 1; //allows a macro instruction to follow opening of loop on same line e.g."5(2,^"
				}
				if (action) {
					// the ',' delimiter was present
					// pass initial spaces and any Log character - we catch that after the switch stmt
					for (;isspace(*action) || *action == LOG || *action == lower(LOG);action++); 
					switch (lower(*action)) {
						case TEXT_EVENT_LEFT:   //  <
							key = KEY_LEFT;
							break;
						case TEXT_EVENT_RIGHT:	//  >
							key = KEY_RIGHT;
							break;
						case TEXT_EVENT_UP:		//  ^
							key = KEY_UP;
							break;
						case TEXT_EVENT_DOWN:	//	v
							key = KEY_DOWN;
							break;
						case TEXT_EVENT_SELECT:	//  o
							key = KEY_SELECT;
							break;
						case TEXT_EVENT_HOME:	//	h
							key = KEY_HOME;
							break;
						case TEXT_EVENT_PLAY:	//	p
							key = KEY_PLAY;
							break;
						case TEXT_EVENT_STAR:	//	*
							key = KEY_STAR;
							break;
						case TEXT_EVENT_PLUS:	//	+
							key = KEY_PLUS;
							break;
						case TEXT_EVENT_MINUS:	//  -
							key = KEY_MINUS;
							break;
						default:
							// todo-macro if *action is number, store value in key (+0x100 or someting like that)
							// using TOUR_OFFSET
							key = 0xFFFF; // causes no-op in case macro script has some unallowed character
							break;
					}
					wait = strToInt(event);
					macro[idxMacro].wait = wait;
					macro[idxMacro].key = key;
					if ((cursor = strchr(action,LOG))) {
						cursor++;
						if (*cursor && !isspace(*cursor))
							macro[idxMacro].log = *cursor;  // give log point a reference ID if non-whitespace
						else
							macro[idxMacro].log = 1;  // otherwise just give indication of generic log point
					}
					else
						macro[idxMacro].log = 0;
					idxMacro++;
				}
				
				// allows ')' on same line as macro instr or not
				// It can even be on the same line as '(' but it will close that same loop 
				// ") 3,^ 3(" is not handled
				if (loopCount && (strchr(event,END_LOOP) || strchr(action,END_LOOP))) { 
					end = idxMacro-1;
					loop[idxLoop].times = loopCount;
					loop[idxLoop].begin = begin;
					loop[idxLoop].end = end;
					loopCount = 0;
					idxLoop++;
				}
			}
			if (idxMacro < MAX_MACRO_ITEMS)
				macro[idxMacro].wait = -1; 	//indicates end of arrays
			if (idxLoop < MAX_MACRO_LOOPS)
				loop[idxLoop].begin = -1; //ensures it never matches an idxMacro
			close(handle);
		}
	}
	if (!goodPass) {
		MACRO_FILE = 0;
		logException(31,MACRO_FILE,0);
	}
}


int nextMacroKey (int keystroke) {
	APP_IRAM static int idxMacro;
	APP_IRAM static long secLastMacro;
	APP_IRAM static int idxLoop;
	APP_IRAM static int countLoop;
	long secNow;
	long secNextEvent;
	char buffer[50];

	//todo-macro: left/right to step through macro (not easy with current setup)

	if (keystroke == KEY_PLUS || keystroke == KEY_MINUS)
		return keystroke; // pass through to allow volume change

	if (keystroke == KEY_PLAY && SACM_Status() && !context.isPaused) {
		secNow = getRTCinSeconds();
		pause();
		do {
			KeyScan_ServiceLoop();
			keystroke = (int)SP_GetCh();
		} while (keystroke != KEY_PLAY && keystroke != KEY_SELECT);					
		if (keystroke == KEY_PLAY) {
			secLastMacro += (getRTCinSeconds() - secNow); // don't count paused time in macro timeline
			resume();
		}
	}

	if (keystroke == KEY_SELECT) {  // handles this whether or not happened during pause
		idxMacro = MAX_MACRO_ITEMS; // terminate macro
//		stop();
		// TODO:using pause instad of stop: when stop() called and then USB_POLL happens, device keys don't respond?
		context.isPaused = TRUE;
		pause();
	}

	if ((idxMacro >= MAX_MACRO_ITEMS) || (macro[idxMacro].wait == -1)) {
		MACRO_FILE = 0; // end of macros
		keystroke = 0;
	} else {
		secNow = getRTCinSeconds();
		secNextEvent = secLastMacro + macro[idxMacro].wait;
 		if (secNow < secNextEvent)
 			keystroke = 0;
 		else {
 			keystroke = macro[idxMacro].key;
			// if > #DEFINE then call fct, play, loop, reset secNow			
 			secLastMacro = secNow;
 			if (macro[idxMacro].log) {
				buffer[0] = 'M';  //macro log entry
				if (macro[idxMacro].log != 1) {
					buffer[1] = macro[idxMacro].log; // log reference id char
					buffer[2] = 0;
				}
				else
					buffer[1] = 0;
 				if (countLoop) {	// if in a loop, indicate what iteration
 					strcat(buffer,"/I");
 					longToDecimalString((long)countLoop,buffer+strlen(buffer),3);
 				}
				strcat(buffer,"/T");   // log system time in seconds
				longToDecimalString(secNow,buffer+strlen(buffer),5);
				logString(buffer,ASAP);
 			}	 				
			if (!countLoop && (loop[idxLoop].begin == idxMacro))
				countLoop = loop[idxLoop].times;
			idxMacro++;
			if (countLoop && (loop[idxLoop].end < idxMacro)) {
				countLoop--;
				if (countLoop)
					idxMacro = loop[idxLoop].begin;
				else if (idxLoop < MAX_MACRO_LOOPS)
		 			idxLoop++;
			} 
 		}		
	}
	return keystroke;	
}


/* THIS CODE is here to remind Cliff to allow playing "tour-guide files" in the middle of a macro
 		do {
			key = keyCheck(0);
			if (key == KEY_PLAY) { // pause  TODO: this key press to pause shouldn't be hard coded
				pause();
				setLED(LED_RED,FALSE);
				do
					key = keyCheck(0);
				while (key != KEY_PLAY && key != KEY_STAR);					
				setLED(LED_RED,TRUE);
				resume();
			}
		} while (key != KEY_STAR); // TODO: this key press to stop shouldn't be hard coded
 */