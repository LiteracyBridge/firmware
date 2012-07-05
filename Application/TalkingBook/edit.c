// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include ".\Component\Include\component_head.h"			
#include "Include/talkingbook.h"
//#include "Include/device.h"
#include "Include/files.h"
#include "Include/audio.h"
#include "./System/Include/WaitMode/SysWaitMode.h"
#include "Include/edit.h"

extern unsigned long Snd_A1800FAT_GetTotalTime(int);
extern void KeyScan_ServiceLoop(void);
extern int SP_GetCh(void);
extern void SACM_Resume();
extern int Snd_Stop(void);
extern unsigned int CLOCK_RATE;

APP_IRAM static unsigned long clipStartFrame, clipEndFrame, clipOriginalLengthFrames;
APP_IRAM char filePath[80];
APP_IRAM int handle;

static int loopClip(void);

int edit(char *lFilePath) {
		
	SetSystemClockRate(MAX_CLOCK_SPEED); // might help to catch ending frames without going over; generally better precision
	Snd_Stop();
	strcpy(filePath,lFilePath);
	handle = tbOpen((LPSTR)(filePath),O_RDWR);
	if (handle < 0) 
		return -1; 
	clipStartFrame = 0;
	clipOriginalLengthFrames = framesFromMSec(Snd_A1800FAT_GetTotalTime(handle));
	clipEndFrame = clipOriginalLengthFrames;
	if (loopClip() != -1) {
		Snd_Stop();
   		trimFile((char *)filePath, clipStartFrame, clipEndFrame);
   		playDing();
	} else
		playBip();
	SetSystemClockRate(CLOCK_RATE);
}

int loopClip(void) {
	const unsigned int adjustmentFrames = 10;
	unsigned int loopTimeFrames = 150;
	unsigned long currentFrame, tempFrame;
	int key;
	int loopEnding = 0;
	unsigned int wasStopped;
	int justStarted;
		
	do { // start with beginning of clip then shift to end after select key
		do {  // keep looping with new adjustments or play/pause
			if (loopTimeFrames > clipEndFrame-clipStartFrame)
					loopTimeFrames = clipEndFrame-clipStartFrame;
			justStarted = 0;
			wasStopped = (SACM_Status() == 0);
			if (wasStopped) {
				close(handle);
				handle = tbOpen((LPSTR)(filePath),O_RDONLY);
				if (handle >= 0) {
					SACMGet_A1800FAT_Mode(handle,0);
					Snd_SACM_PlayFAT(handle, C_CODEC_AUDIO1800);
					justStarted = 1;
				}
			}
			if (loopEnding) {
				tempFrame = clipEndFrame - loopTimeFrames;
				gotoFrame(tempFrame);
			} else {
				tempFrame = clipStartFrame + loopTimeFrames;
				if (clipStartFrame || !justStarted)
					gotoFrame(clipStartFrame);
			}
			do {
				currentFrame = getCurrentFrame();
				KeyScan_ServiceLoop();
				key = (int)SP_GetCh();		
			} while (SACM_Status() && !key && ((loopEnding && currentFrame < clipEndFrame) || (!loopEnding && currentFrame < tempFrame)));	
			if (currentFrame > clipOriginalLengthFrames)
				Snd_Stop(); 
			if (SACM_Status()) 
				pause();
			while (!key) {
				KeyScan_ServiceLoop();
				key = (int)SP_GetCh();
			}					
			if (key == KEY_RIGHT) {
				if (loopEnding) {
					clipEndFrame += adjustmentFrames;
					if (clipEndFrame > clipOriginalLengthFrames)
						clipEndFrame = clipOriginalLengthFrames;
				} else {
					clipStartFrame += adjustmentFrames;
					if (clipStartFrame > clipEndFrame)
							clipStartFrame = clipEndFrame;
				}				
			} else if (key == KEY_LEFT) {
				if (loopEnding) {
					clipEndFrame -= adjustmentFrames;
					if (clipEndFrame < clipStartFrame)
						clipEndFrame = clipStartFrame;
				} else {
					if (clipStartFrame < adjustmentFrames)
						clipStartFrame = 0;
					else
						clipStartFrame -= adjustmentFrames;
				}				
			} 
		} while (key != KEY_SELECT && key != KEY_HOME);
		loopEnding++;
	} while (loopEnding == 1 && key != KEY_HOME);
	close(handle);
	if (key == KEY_HOME)
		return -1;
	else 
		return 0;
}

//delete part of file,from offset,delete length byte
//but the file system will change the offset and length to cluster number 
//DeleteAudioSection {
//	INT16 DeletePartFile(INT16 fd, UINT32 offset, UINT32 length);


//insert the src file to tag file
//INT16 InserPartFile(INT16 tagfd, INT16 srcfd, UINT32 tagoff, UINT32 srclen);
//split tag file into two file, one is remain in tag file, one is in src file
//INT16 SplitFile(INT16 tagfd, INT16 srcfd, UINT32 splitpoint);
