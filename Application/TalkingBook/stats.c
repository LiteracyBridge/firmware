// Copyright 2009-2011 Literacy Bridge
// Contact: info@literacybridge.org
#include <ctype.h>
#include "./system/include/system_head.h"
#include "Include/talkingbook.h"
#include "Include/app_exception.h"
#include "Include/device.h"
#include "Include/files.h"
#include "Include/app_exception.h"
#include "Include/sys_counters.h"
#include "Include/filestats.h"

APP_IRAM int offsetMsgNames[MAX_TRACKED_MESSAGES];
static void rebuildNORmsgMap(struct NORmsgMap *);
static void rebuildNORmsgStats(struct NORallMsgStats *);
static int saveFlashStats(struct SystemData *, struct SystemCounts2 *, struct NORmsgMap *, struct NORallMsgStats *);
static void *FindNextFlashSameStruct(void *); 
static void rebuildFlash(int);
static int ptrSizeOfNORStruct(void *);
static void createMsgNameOffsetsFromMap(void);


// find offset to first writeable byte in SN flash 
int
FindFirstFlashOffset() {
	unsigned int *fp = (unsigned int *) TB_SERIAL_NUMBER_ADDR;
	
	while((*((int *)fp) != 0xffff) && ((long)fp < ((long)(TB_SERIAL_NUMBER_ADDR + TB_FLASH_SIZE)))) {
		fp += ptrSizeOfNORStruct(fp);
	}	
	return (int)((long)fp-TB_SERIAL_NUMBER_ADDR);
}

static int 
ptrSizeOfNORStruct(void *structType) {
	int ret;
	int msgCount;
	struct NORmsgMap *map;
	
	switch (*(char *)structType) {
		case NOR_STRUCT_ID_SYSTEM:
			ret = sizeof(struct SystemData);
			break;
		case NOR_STRUCT_ID_COUNTS:
			ret = sizeof(struct SystemCounts2);
			break;
		case NOR_STRUCT_ID_MSG_MAP:
			// This is the one struct that can have a variable length in memory.
			// Need to determine how many messages are included in map to get size
			//struct NORmsgMap {
			//	char structType;
			//	char totalMessages;
			//	char msgIdMap[MAX_TRACKED_MESSAGES][MAX_MESSAGE_ID_LENGTH];
			map=(struct NORmsgMap *)structType;
			msgCount=map->totalMessages;
			ret = sizeof(struct NORmsgMap) - ((MAX_TRACKED_MESSAGES - msgCount) * MAX_MESSAGE_ID_LENGTH);
			break;
		case NOR_STRUCT_ID_MESSAGE_STATS:
			ret = sizeof(struct NORmsgStats);
			break;
		case NOR_STRUCT_ID_STAT_EVENT:
			ret = sizeof(struct NORstatEvent);
			break;
		case NOR_STRUCT_ID_NEW_MSG:
			ret = sizeof(struct NORnewMsg);
			break;
		case NOR_STRUCT_ID_PERIOD:
			ret = sizeof(struct NORperiod);
			break;
		case NOR_STRUCT_ID_CUMULATIVE_DAYS:
			ret = sizeof(struct NORcumulativeDays);
			break;
		case NOR_STRUCT_ID_POWERUPS:
			ret = sizeof(struct NORpowerups);
			break;
		case NOR_STRUCT_ID_CORRUPTION:
			ret = sizeof(struct NORcorruption);
			break;
		case NOR_STRUCT_ID_ROTATION:
			ret = sizeof(struct NORrotation);
			break;
		default:
			logException(98,(const char *)"unknown NOR struct type",0);
			// this could be a trigger that we have an old rev 
			ret = 1; // inspect every byte, which will more likely lead to a FFFF
			break;		
	}	
	return ret;
}

static void
rebuildFlash(int rewriteFlash) {
	// refresh system data
	// refresh message map
	// build message stats
	// export stats just in case something goes wrong with erase and rewrite
	// append everything with AppendStructToFlash
	struct SystemData sd;
	struct SystemCounts2 sc;
	struct NORmsgMap msgMap;
	struct NORallMsgStats stats;
	struct NORrotation *rotation;
	struct SystemData *ptrSD;
	int ret, m, r, size, totalSize;	
	
	for (m=0;m < MAX_TRACKED_MESSAGES;m++) {
		for (r=0;r < MAX_ROTATIONS; r++) {
			stats.stats[m][r].structType = -2;
		}
	}
	ptrSD = (struct SystemData *)FindFirstFlashStruct(NOR_STRUCT_ID_SYSTEM);
	if (ptrSD != NULL) 
		memcpy(&sd,ptrSD,sizeof(struct SystemData));
	if (rewriteFlash) {
		sd.countReflashes++;
	}
	sc.structType = NOR_STRUCT_ID_COUNTS;
	sc.period = getPeriod();
	sc.cumulativeDays = getCumulativeDays();
	sc.corruptionDay = getCorruptionDay();
	sc.powerups = getPowerups();
	sc.lastInitVoltage = getLastInitVoltage();
	memcpy((void *)sc.rotations,(void *)ptrsCounts.systemCounts->rotations,sizeof(struct NORrotation) * MAX_ROTATIONS);
	rotation = (struct NORrotation *)FindFirstFlashStruct(NOR_STRUCT_ID_ROTATION);
	while (rotation != NULL) {
		memcpy((void *)&sc.rotations[(int)rotation->rotationNumber],(void *)rotation,sizeof(struct NORrotation));
		rotation = (struct NORrotation *)FindNextFlashSameStruct(rotation);
	}
	rebuildNORmsgMap(&msgMap);
	rebuildNORmsgStats(&stats);

	ret = saveFlashStats(&sd, &sc, &msgMap, &stats);
	if (rewriteFlash) {
		if(ret != -1) {
			//FOR DEBUGGING PURPOSES, dump entire memory block to disk before and after reflash
			int handle;
			handle = tbOpen((LPSTR)(SYS_DATA_STATS_PATH_DEBUG_PRE),O_CREAT|O_TRUNC|O_RDWR);
			if (handle != -1) {
				ret = write(handle, (unsigned long)TB_SERIAL_NUMBER_ADDR<<1, TB_FLASH_SIZE<<1);
				close(handle);
			}
			
			// erase flash
			totalSize = 0;
			size = sizeof(struct SystemData);	// round up # of words		
			write_app_flash((int *)&sd, size, totalSize);
			totalSize += size;
			
			size = sizeof(struct SystemCounts2);
			write_app_flash((int *)&sc, size, totalSize);
			ptrsCounts.systemCounts = (struct SystemCounts2 *)(unsigned long)(TB_SERIAL_NUMBER_ADDR + totalSize); 
			ptrsCounts.period = NULL; // (struct NORperiod *)&ptrsCounts.systemCounts->period;
			ptrsCounts.corruptionDay = NULL;  // (struct NORcorruption *)&ptrsCounts.systemCounts->corruptionDay;		
			ptrsCounts.cumulativeDays = NULL;  // (struct NORcumulativeDays *)&ptrsCounts.systemCounts->cumulativeDays;
			//for (r=1;r < MAX_ROTATIONS && ptrsCounts.systemCounts->rotations[r].structType == NOR_STRUCT_ID_ROTATION;r++);
			ptrsCounts.latestRotation = NULL;  //  &ptrsCounts.systemCounts->rotations[r-1];
			ptrsCounts.powerups = NULL;  // (struct NORpowerups *)&ptrsCounts.systemCounts->powerups;
			totalSize += size;

			size = sizeof(struct NORmsgMap);
			size -= (MAX_TRACKED_MESSAGES - msgMap.totalMessages) * MAX_MESSAGE_ID_LENGTH;
			write_app_flash((int *)&msgMap, size, totalSize);
			totalSize += size;
			createMsgNameOffsetsFromMap();

			size = sizeof(struct NORmsgStats);		
			for (r=0; r < stats.totalRotations;r++) {
				for (m=0; m < stats.totalMessages; m++) {
					if (stats.stats[m][r].structType == NOR_STRUCT_ID_MESSAGE_STATS) {
						write_app_flash((int *)&stats.stats[m][r], size,totalSize);
						totalSize += size;
					}
				}
			}
			//FOR DEBUGGING PURPOSES, dump entire memory block to disk before and after reflash
			handle = tbOpen((LPSTR)(SYS_DATA_STATS_PATH_DEBUG_POST),O_CREAT|O_TRUNC|O_RDWR);
			if (handle != -1) {
				ret = write(handle, (unsigned long)TB_SERIAL_NUMBER_ADDR<<1, TB_FLASH_SIZE<<1);
				close(handle);
			}
		} else
			logException(96,(const char *)"Can't save flash stats, so will not erase and rebuild flash.",SHUT_DOWN);
	}
}


// append a string to SN flash, if SN flash is full erase it and rewrite SN + current location, content id, content date
extern void *
AppendStructToFlash(void *newStruct) {
	int firstavail = FindFirstFlashOffset();
	int navail = TB_FLASH_SIZE - firstavail;
	int needed = ptrSizeOfNORStruct(newStruct);
	
	if(navail < needed) {
		// not enough room for this new value, erase SN flash and reflash current items & newstring
		rebuildFlash(1);
	} else {	
		write_app_flash((int *)newStruct, needed, firstavail);
	}
	return (void *)(TB_SERIAL_NUMBER_ADDR + firstavail);
}

extern void * FindFirstFlashStruct(char structType) {
	void *ret;
	ret = FindFlashStruct(structType,1);
	return ret;
}

void * FindLastFlashStruct(char structType) {
	void *ret;
	ret = FindFlashStruct(structType,0);
	return ret;
}

// find first or last struct in flash of the type provided
void * FindFlashStruct(char structType, int first) {
	void *ret = NULL;	
	void *fp = (void *) TB_SERIAL_NUMBER_ADDR;
	
	while((*((int *)fp) != 0xffff) && (fp < (void *)(TB_SERIAL_NUMBER_ADDR + TB_FLASH_SIZE))) {
		if((*(char *)fp)==structType) {
			ret = fp;
			if (first == 1)
				break;
		}
		fp += ptrSizeOfNORStruct(fp);
	}
	return ret;
}

// find next struct in flash
static void * 
FindNextFlashSameStruct(void *fp) {
	char structType = *(char *)fp;
	char *ret = NULL;	

	fp += ptrSizeOfNORStruct(fp);
	while(fp >= (void *)TB_SERIAL_NUMBER_ADDR  && (fp < (void *)(TB_SERIAL_NUMBER_ADDR + TB_FLASH_SIZE)) &&  (*((int *)fp) != 0xffff) )  {  
		if((*(char *)fp)==structType) {
			ret = fp;
			break;
		}
		fp += ptrSizeOfNORStruct(fp);
	}
	return ret;
}

// find next struct in flash
void * FindNextFlashStruct(void *fp, char structType) {
	char *ret = NULL;	

	while(fp >= (void *)TB_SERIAL_NUMBER_ADDR  && (fp < (void *)(TB_SERIAL_NUMBER_ADDR + TB_FLASH_SIZE)) &&  (*((int *)fp) != 0xffff) )  {  
		if((*(char *)fp)==structType) {
			ret = fp;
			break;
		}
		fp += ptrSizeOfNORStruct(fp);
	}
	return ret;
}

static
void rebuildNORmsgMap(struct NORmsgMap *msgMap) {
	int i;
	
	msgMap->structType = NOR_STRUCT_ID_MSG_MAP;
	for (i=0; i < MAX_TRACKED_MESSAGES && offsetMsgNames[i]; i++) {
		strcpy(msgMap->msgIdMap[i],(char *)(TB_SERIAL_NUMBER_ADDR + offsetMsgNames[i]));
	}
	msgMap->totalMessages = i;
}

static
void rebuildNORmsgStats(struct NORallMsgStats *allStats) {
	void *fp;
	char log[40];
	int highestMessage=-1, highestRotation=-1;
	struct NORmsgStats *msgStats;
	int m,r;
	//initialize all counters to 0
	for (m=0; m < MAX_TRACKED_MESSAGES; m++) {
		for (r=0; r < MAX_ROTATIONS; r++) {
			msgStats = &allStats->stats[m][r];
			msgStats->structType = -2;
			msgStats->countStarted = 0;
			msgStats->countQuarter = 0;
			msgStats->countHalf = 0;
			msgStats->countThreequarters = 0;
			msgStats->countCompleted = 0;
			msgStats->countApplied = 0;
			msgStats->countUseless = 0;
			msgStats->totalSecondsPlayed = 0; 
		}
	}	
		
// extern struct NORallMsgStats {
//	char totalMessages;
//	char totalRotations;
//	struct NORmsgStats stats[MAX_TRACKED_MESSAGES][MAX_ROTATIONS];
//};
//struct NORmsgStats {
//	char structType;	// used to identify data structure
//	char indexMsg;
//	char numberRotation;//...
//};
	fp = FindFirstFlashStruct(NOR_STRUCT_ID_MESSAGE_STATS);
	while (fp != NULL) {
		//TODO: This code would be much faster if looked for NORstatEvent in parallel with scan for NORmsgStats
		struct NORmsgStats *msgStats = (struct NORmsgStats *)fp;
		int indexMsg = msgStats->indexMsg;
		int rotation = msgStats->numberRotation;
		if (indexMsg > highestMessage)
			highestMessage = indexMsg;
		if (rotation > highestRotation)
			highestRotation = rotation;
		memcpy(&allStats->stats[indexMsg][rotation],fp,sizeof(struct NORmsgStats));
		if ((allStats->totalMessages-1) < indexMsg)
			allStats->totalMessages = indexMsg + 1;
		if ((allStats->totalRotations-1) < rotation)
			allStats->totalRotations = rotation + 1;
		fp = FindNextFlashSameStruct(fp);
	}
//#define NOR_STRUCT_ID_STAT_EVENT	3
//struct NORstatEvent {
//	char structType;
//	char indexMsg;	// array index for message
//	char statType; // 0:10sec,1:25%,2:50%,3:75%,4:100%,5:survey:applied,6:survey useless
//	char rotation;
//};
//struct NORmsgStats {
//...
//	char countStarted;
//	char countQuarter;
//	char countHalf;
//	char countThreequarters;
//	char countCompleted;
//	char countApplied;
//	char countUseless;
//};
//
	fp = FindFirstFlashStruct(NOR_STRUCT_ID_STAT_EVENT);
	while (fp != NULL) {
		struct NORstatEvent *event = (struct NORstatEvent *)fp;
		int indexMsg = event->indexMsg;
		int rotation = event->rotation;
		struct NORmsgStats *msgStats = &allStats->stats[indexMsg][rotation];
		
		log[0] = 0;
		if (indexMsg > MAX_TRACKED_MESSAGES) {
			strcpy(log,(char *)"NORstatEvent had bad msg #");
			longToDecimalString(indexMsg,log+strlen(log),5);
			strcat(log," rotation #");
			longToDecimalString(rotation,log+strlen(log),5);			
		}
		if (rotation > MAX_ROTATIONS) {
			strcpy(log,(char *)"NORstatEvent had bad rotation #");
			longToDecimalString(rotation,log+strlen(log),5);			
			strcat(log," message #");
			longToDecimalString(indexMsg,log+strlen(log),5);
		}
		if (log[0] != 0) {
			logException(97,log,0);
		}
		if (indexMsg > highestMessage)
			highestMessage = indexMsg;
		if (rotation > highestRotation)
			highestRotation = rotation;
		msgStats->structType = NOR_STRUCT_ID_MESSAGE_STATS;
		msgStats->indexMsg = indexMsg;
		msgStats->numberRotation = rotation;
		msgStats->totalSecondsPlayed += event->secondsOfPlay;
		switch(event->statType) {
			case 0:
				msgStats->countStarted++;
				break;	
			case 1:
				msgStats->countQuarter++;
				break;	
			case 2:
				msgStats->countHalf++;
				break;	
			case 3:
				msgStats->countThreequarters++;
				break;	
			case 4:
				msgStats->countCompleted++;
				break;	
			case 5:
				msgStats->countApplied++;
				break;	
			case 6:
				msgStats->countUseless++;
				break;	
		}
		fp = FindNextFlashSameStruct(fp);
	}
	allStats->totalMessages = highestMessage + 1;
	allStats->totalRotations  = highestRotation + 1;
}

char getMsgIndex (char * strMessageID) {
	// first search NORmsgMap in flash using pointer to it created during startup
	char ret=-1;
	int i;
	// check APP_IRAM array of int offsetMsgNames[40]
	for (i=0; offsetMsgNames[i] && i < MAX_TRACKED_MESSAGES; i++) {
		if (!strcmp((char *)(TB_SERIAL_NUMBER_ADDR+offsetMsgNames[i]),strMessageID)) {
			ret = (char)i;
			break;
		}
	}	
	// if not found there, return -1 (which is really FF)
	return ret;
}

static 
char addNewMsgToMap (char *strMessageID) {
	struct NORnewMsg msg;
	int i;
	for (i=0;i < MAX_TRACKED_MESSAGES && offsetMsgNames[i];i++);
	if (i == MAX_TRACKED_MESSAGES)
		logException(97,(char *)"attempted to add more than MAX_TRACKED_MESSAGES",SHUT_DOWN);
	else {
		msg.structType = NOR_STRUCT_ID_NEW_MSG; 
		msg.indexMsg = (char)i;
		strcpy(msg.idMsg,strMessageID);
		// set flash offset using return value of AppendStructToFlash, but adjust to place offset at string within the struct
		offsetMsgNames[i]=(int)((long)&msg.idMsg-(long)&msg) + ((long)AppendStructToFlash(&msg) - TB_SERIAL_NUMBER_ADDR);
	}	
	return i;
}

extern
void writeMsgEventToFlash (char *strMessageID, int msgEvent, unsigned int secondsPlayed) {
	char msgIndex = getMsgIndex(strMessageID);
	struct NORstatEvent statEvent;

	if (msgIndex == -1) {
		msgIndex = addNewMsgToMap(strMessageID);
	}
	statEvent.structType = NOR_STRUCT_ID_STAT_EVENT;
	statEvent.indexMsg = msgIndex;
	statEvent.statType = msgEvent;
	statEvent.rotation = getRotation();
	statEvent.secondsOfPlay = secondsPlayed;
	AppendStructToFlash(&statEvent);
}	


static
void createMsgNameOffsetsFromMap(void) {
	int i = 0;
	struct NORmsgMap *map = (struct NORmsgMap*)FindFirstFlashStruct(NOR_STRUCT_ID_MSG_MAP);

	if (map != NULL) {
		for (i=0; i < map->totalMessages;i++) {
			offsetMsgNames[i] = (long)&map->msgIdMap[i] - TB_SERIAL_NUMBER_ADDR;
		}
	}
}

void createMsgNameOffsets() {
	void *fp = NULL;
	int indexMsg;

	// initialize array with 0s indicating end of list (msgMap won't exist until first reflash after an update)
	for (indexMsg=0; indexMsg < MAX_TRACKED_MESSAGES; indexMsg++) 
		offsetMsgNames[indexMsg] = 0;
	createMsgNameOffsetsFromMap();
	
	// now add to what was created from the map with all the new message event structs
	fp = FindFirstFlashStruct(NOR_STRUCT_ID_NEW_MSG);
	while (fp != NULL) {
		struct NORnewMsg *newMsg = (struct NORnewMsg *)fp;
		indexMsg = newMsg->indexMsg;
		offsetMsgNames[indexMsg] = (long)&newMsg->idMsg - TB_SERIAL_NUMBER_ADDR;
		fp = FindNextFlashSameStruct(fp);
	}
}

extern
void warmStartNORStats(void) {
	unsigned int powerUps;
	
	powerUps = getPowerups();
	powerUps++;
	setPowerups(powerUps);
}

extern
void coldStartNORStats(void) {
	
	incrementPeriod();
}


extern void 
exportFlashStats(void) {
	//export all flash stats
	rebuildFlash(0);	
	//Temporary code until transition is complete to new flash stats:
	confirmSNonDisk();
	writeVersionToDisk(SYSTEM_PATH);  // make sure the version file is correct
	confirmPackageNameonDisk();
	confirmLocationonDisk();
}

static int saveFlashStats(struct SystemData *sd, struct SystemCounts2 *sc, struct NORmsgMap *msgMap, struct NORallMsgStats *stats) {
	int handle, ret;
	
	handle = tbOpen((LPSTR)(SYS_DATA_STATS_PATH),O_CREAT|O_TRUNC|O_RDWR);
	if (handle != -1) {
		ret = write(handle, (unsigned long)sd<<1, sizeof(struct SystemData)<<1);
		ret = write(handle, (unsigned long)sc<<1, sizeof(struct SystemCounts2)<<1);
		ret = write(handle, (unsigned long)msgMap<<1, sizeof(struct NORmsgMap)<<1);
		ret = write(handle, (unsigned long)stats<<1, sizeof(struct NORallMsgStats)<<1);
		close(handle);
	} else
		logString((char *)"failed to write system data and stats to " SYS_DATA_STATS_PATH,BUFFER,LOG_ALWAYS);
	return ret;
}
