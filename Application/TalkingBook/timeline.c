// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/talkingbook.h"
#include "Include/device.h"
#include "Include/containers.h"
#include "Include/timeline.h"

APP_IRAM BlockTimeline blockTimeline[MAX_STATES];

static int getBlockIdx(CtnrBlock *);
static int copyActiveBlocks (int *, int *, int);

static int getBlockIdx(CtnrBlock *block) {
	return (block - context.package->blocks);
}

unsigned int getCurrentTimeframeStart (void) {
	return blockTimeline[context.idxTimeframe].time;
}

EnumStartOrEnd getTimelineDiff (int idxTimeline, int *idxStartBlock, int *idxEndBlock) {
	EnumStartOrEnd ret;
	int idxRelStart, idxRelEnd;
	
	ret = NONE;
	idxRelStart = blockTimeline[idxTimeline].idxStartingEndingBlock & 0x00FF;
	idxRelEnd = ((unsigned int)blockTimeline[idxTimeline].idxStartingEndingBlock) >> 8;
	
	if (idxRelStart != 0xFF) { //different than -1 since we're looking at one byte and ints are two bytes
		*idxStartBlock = blockTimeline[idxTimeline].activeBlocks[idxRelStart];
		ret = STARTING;
	}
	if (idxRelEnd != 0xFF && idxTimeline > 0) {
		*idxEndBlock = blockTimeline[idxTimeline-1].activeBlocks[idxRelEnd];
		if (ret == NONE)
			ret = ENDING;
		else
			ret = BOTH;
	}
	return ret;
}

int getIdxTimeline(unsigned int time) {
	int arrayIndex;
	
	//search through all currently active block containers to get actions
	if (time >= blockTimeline[0].time) {   // no active blocks if time < blockTimeline[0].time 
		arrayIndex=1; // we now know time >= blockTimeline[0].time
		while ((arrayIndex < MAX_STATES) && (blockTimeline[arrayIndex].time != -1)) {
			if (blockTimeline[arrayIndex].time > time)
				break;
			else
				arrayIndex++;
		}
		arrayIndex--; // back to the array that begins before current time 
	}
	else
		// time occurs before first block is active
		arrayIndex = -1;
	return arrayIndex;		
}

int getStartingBlockIdxFromTimeline(int idxTimeline) {
	EnumStartOrEnd startOrEnd;
	int start, end, ret;
	
	startOrEnd = getTimelineDiff(idxTimeline, &start, &end);
	if (startOrEnd != ENDING)  // STARTING or BOTH
		ret = start;
	else
		ret = -1;
	return ret;
}

static int copyActiveBlocks (int *copyTo, int *copyFrom, int remove) {
	int temp;
	int idxCopyTo = 0;
	int idxCopyFrom = 0;
	do {
		temp = copyFrom[idxCopyFrom];
		if (temp != remove && temp != -1)
			copyTo[idxCopyTo++] = temp;
		idxCopyFrom++;
	} while ((temp != -1) && idxCopyFrom < MAX_BLOCK_OVERLAP);
	return idxCopyTo;
}

//could maybe do this recursively instead of iterative thing below
void buildBlockTimelines(CtnrFile *newFile) {
	CtnrBlock *startingNext;
	CtnrBlock *endingNext;
	int idxBlockTimeline;
	unsigned int nextStartTime;
	unsigned int nextEndTime;
	int index;

	idxBlockTimeline=0;
	startingNext = getStartBlockFromFile(newFile);
	endingNext = getEndBlockFromFile(newFile);
	
	while ((idxBlockTimeline < MAX_STATES)  && (startingNext || endingNext)) {
		if (startingNext)
			nextStartTime = startingNext->startTime;
		else 
			nextStartTime = 0xFFFF;
		if (endingNext)
			nextEndTime = endingNext->endTime;
		else 
			nextEndTime = 0xFFFF; 
		// if the starting/endingNext pointers were NULL, we don't care about the nextStartTime/EndTime comparison - see && operator below
		if ((nextStartTime < nextEndTime) && startingNext) {
			blockTimeline[idxBlockTimeline].time = nextStartTime;
			if (idxBlockTimeline) {
				//copy from previous active array and just add newest thing
				index = copyActiveBlocks(blockTimeline[idxBlockTimeline].activeBlocks, blockTimeline[idxBlockTimeline-1].activeBlocks, -1);
				if (index < MAX_BLOCK_OVERLAP) { //todo:error if beyond overlap
					blockTimeline[idxBlockTimeline].idxStartingEndingBlock = index;
					blockTimeline[idxBlockTimeline].activeBlocks[index] = getBlockIdx(startingNext);
					index++;
				} else
					logException(3,0,(context.package == &pkgSystem)?USB_MODE:RESET); //todo: max block overlap exceeded
			} else {
				blockTimeline[0].idxStartingEndingBlock = 0xFF00;  // indicates first element of array is starting; nothing ending
				blockTimeline[0].activeBlocks[0] = getBlockIdx(startingNext);  // blockTimeline[0].activeBlocks[0]
				index = 1;
			}
			startingNext = getNextStartBlock(startingNext);
		} else if ((nextEndTime < nextStartTime) || (endingNext && !startingNext)) {
			int lastIdxOfEnding = 0;
			do {  // get index of ending block as found in previous timeline position
				if (blockTimeline[idxBlockTimeline-1].activeBlocks[lastIdxOfEnding] == getBlockIdx(endingNext))
					break;
				lastIdxOfEnding++;
			} while (lastIdxOfEnding < MAX_BLOCK_OVERLAP);
			if (lastIdxOfEnding == MAX_BLOCK_OVERLAP)
				logException(3,0,(context.package == &pkgSystem)?USB_MODE:RESET); //todo: max block overlap exceeded
			blockTimeline[idxBlockTimeline].idxStartingEndingBlock = (lastIdxOfEnding<<8) + 0xFF;
			blockTimeline[idxBlockTimeline].time = nextEndTime;
			index = copyActiveBlocks(blockTimeline[idxBlockTimeline].activeBlocks,blockTimeline[idxBlockTimeline-1].activeBlocks, getBlockIdx(endingNext));
			endingNext = getNextEndBlock(endingNext);
		}
		//new container starts at same moment another ends 
		else if ((nextStartTime == nextEndTime) && (startingNext && endingNext)) {
			int lastIdxOfEnding = 0;
			blockTimeline[idxBlockTimeline].time = nextEndTime;
			index = copyActiveBlocks(blockTimeline[idxBlockTimeline].activeBlocks,blockTimeline[idxBlockTimeline-1].activeBlocks, getBlockIdx(endingNext));
			//fill next spot with next container
			do {  // get index of ending block as found in previous timeline position
				if (blockTimeline[idxBlockTimeline-1].activeBlocks[lastIdxOfEnding] == getBlockIdx(endingNext))
					break;
				lastIdxOfEnding++;
			} while (lastIdxOfEnding < MAX_BLOCK_OVERLAP);
			if (lastIdxOfEnding == MAX_BLOCK_OVERLAP) 
				logException(3,0,(context.package == &pkgSystem)?USB_MODE:RESET); //todo: max block overlap exceeded
			blockTimeline[idxBlockTimeline].idxStartingEndingBlock = lastIdxOfEnding<<8;
			if (index < MAX_BLOCK_OVERLAP) {
				blockTimeline[idxBlockTimeline].idxStartingEndingBlock += index;
				blockTimeline[idxBlockTimeline].activeBlocks[index++] = getBlockIdx(startingNext);
			}
			startingNext = getNextStartBlock(startingNext);
			endingNext = getNextEndBlock(endingNext);
		} 
		// set remainder of array to -1 for enterOrExitAllBlocks() 
		for (;index < MAX_BLOCK_OVERLAP;index++)
			blockTimeline[idxBlockTimeline].activeBlocks[index] = -1;
		idxBlockTimeline++;	
	}  // while loop on starting/ending Next pointers
	if (idxBlockTimeline < MAX_STATES)
		blockTimeline[idxBlockTimeline].time = -1;  // indicate end of array
}

	