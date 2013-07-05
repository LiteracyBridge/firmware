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

// find offset to first writeable byte in SN flash 
int
FindFirstFlashOffset() {
	unsigned int *fp = (unsigned int *) TB_SERIAL_NUMBER_ADDR;
	int i;
	
	for (i=0; i<TB_FLASH_SIZE; i++) {
		if(*(fp+i) == 0xffff) {  // first unwritten byte
			break;
		}
	}	
	return(i);	
}

static int 
sizeOfNORStruct(char idStructType) {
	int ret;	

	switch (idStructType) {
		case NOR_STRUCT_ID_SYSTEM:
			ret = sizeof(struct SystemData);
			break;
		case NOR_STRUCT_ID_MSG_MAP:
			ret = sizeof(struct NORmsgMap);
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
		case NOR_STRUCT_ID_WEEK:
			ret = sizeof(struct NORweek);
			break;
		default:
			logException(98,(const char *)"unknown NOR struct type",SHUT_DOWN);
			break;		
	}	
	return ret;
}

static int 
ptrSizeOfNORStruct(void *structType) {
	int ret;
	char idStructType;
	
	idStructType = *(char *)structType;	
	ret = sizeOfNORStruct(idStructType); 
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
	struct NORweek *week;
	int ret, m, w, size, totalSize;	
	
	for (m=0;m < MAX_TRACKED_MESSAGES;m++) {
		for (w=0;w < MAX_WEEKS; w++) {
			stats.stats[m][w].structType = -1;
		}
	}

	memcpy(&sd,(struct SystemData *)FindFirstFlashStruct(NOR_STRUCT_ID_SYSTEM),sizeof(struct SystemData));

	sc.structType = NOR_STRUCT_ID_COUNTS;
	sc.period = getPeriod();
	sc.cumulativeDays = getCumulativeDays();
	sc.corruptionDay = getCorruptionDay();
	sc.powerups = getPowerups();
	memcpy((void *)sc.weeks,(void *)getLatestWeekStruct(),sizeof(struct NORweek));
	week = (struct NORweek *)FindFirstFlashStruct(NOR_STRUCT_ID_WEEK);
	for (w=0;w < MAX_WEEKS && (week != NULL); w++) {
		memcpy((void *)&sc.weeks[w],(void *)week,sizeof(struct NORweek));
		week = (struct NORweek *)FindNextFlashSameStruct(week);
	}
	if (w < MAX_WEEKS) {
		sc.weeks[w].structType = -1;
	}
	rebuildNORmsgMap(&msgMap);
	rebuildNORmsgStats(&stats);

	ret = saveFlashStats(&sd, &sc, &msgMap, &stats);
	if (rewriteFlash) {
		if(ret != -1) {
			// erase flash
			totalSize = 0;
			size = (sizeof(struct SystemData) + 1) >> 1;	// round up # of words		
			write_app_flash((int *)&sd, size, totalSize);
			totalSize += size;
			
			size = (sizeof(struct SystemCounts2) + 1) >> 1;
			write_app_flash((int *)&sc, size, totalSize);
			totalSize += size;

			size = (sizeof(struct NORmsgMap) + 1) >> 1;
			size -= (MAX_TRACKED_MESSAGES - msgMap.totalMessages) * MAX_MESSAGE_ID_LENGTH;
			write_app_flash((int *)&msgMap, size, totalSize);
			totalSize += size;

			size = (sizeof(struct NORmsgStats) + 1) >> 1;		
			for (w=0; w < stats.totalWeeks;w++) {
				for (m=0; m < stats.totalMessages; m++) {
					if (stats.stats[m][w].structType != -1) {
						write_app_flash((int *)&stats.stats[m][w], size,totalSize);
						totalSize += size;
					}
				}
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
// extern struct NORallMsgStats {
//	char totalMessages;
//	char totalWeeks;
//	struct NORmsgStats stats[MAX_TRACKED_MESSAGES][MAX_WEEKS];
//};
//struct NORmsgStats {
//	char structType;	// used to identify data structure
//	char indexMsg;
//	char numberWeek;//...
//};
	fp = FindFirstFlashStruct(NOR_STRUCT_ID_MESSAGE_STATS);
	while (fp != NULL) {
		//TODO: This code would be much faster if looked for NORstatEvent in parallel with scan for NORmsgStats
		struct NORmsgStats *msgStats = (struct NORmsgStats *)fp;
		int indexMsg = msgStats->indexMsg;
		int week = msgStats->numberWeek;
		memcpy(&allStats->stats[indexMsg][week],fp,sizeof(struct NORmsgStats));
		if ((allStats->totalMessages-1) < indexMsg)
			allStats->totalMessages = indexMsg + 1;
		if ((allStats->totalWeeks-1) < week)
			allStats->totalWeeks = week + 1;
		fp = FindNextFlashSameStruct(fp);
	}
//#define NOR_STRUCT_ID_STAT_EVENT	3
//struct NORstatEvent {
//	char structType;
//	char indexMsg;	// array index for message
//	char statType; // 0:10sec,1:25%,2:50%,3:75%,4:100%,5:survey:applied,6:survey useless
//	char week;
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
		int week = event->week;
		struct NORmsgStats *msgStats = &allStats->stats[indexMsg][week];
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
		AppendStructToFlash(&msg);
	}	
	return i;
}

extern
void writeMsgEventToFlash (char *strMessageID, int msgEvent) {
	char msgIndex = getMsgIndex(strMessageID);
	struct NORstatEvent statEvent;

	if (msgIndex == -1) {
		msgIndex = addNewMsgToMap(strMessageID);
	}
	statEvent.structType = NOR_STRUCT_ID_STAT_EVENT;
	statEvent.indexMsg = msgIndex;
	statEvent.statType = msgEvent;
	statEvent.week = getWeek();
	AppendStructToFlash(&statEvent);
}	

void guessCurrentWeek() {
	// needs to allow updating week when agent says so or when we believe from data (like multiple DELETES) that this has happened
	// this requires storing a struct with dayOfWeekStart along with current week
	//int days = getCumulativeDays() 
	//week = (days+5)/7
	//if yes then call updateWeek()
}

void updateWeek() {
}

void midnightUpdate() {
	// guess if should update NORweek
	// update NORcumulativeDays
}

void createMsgNameOffsets() {
	void *fp = NULL;
	int indexMsg = 0;
	int i;
	struct NORmsgMap *map = (struct NORmsgMap*)FindFirstFlashStruct(NOR_STRUCT_ID_MSG_MAP);

	offsetMsgNames[0] = 0; // necessary to initialize the end flag if no messages are in flash yet
	for (i=0; i < map->totalMessages;i++) {
		offsetMsgNames[i] = (long)&map->msgIdMap[i] - TB_SERIAL_NUMBER_ADDR;
	}
	fp = FindFirstFlashStruct(NOR_STRUCT_ID_NEW_MSG);
	while (fp != NULL) {
		struct NORnewMsg *newMsg = (struct NORnewMsg *)fp;
		indexMsg = newMsg->indexMsg;
		offsetMsgNames[indexMsg] = (long)&newMsg->idMsg - TB_SERIAL_NUMBER_ADDR;
		fp = FindNextFlashSameStruct(fp);
	}
	if (indexMsg < MAX_TRACKED_MESSAGES - 1)
		offsetMsgNames[indexMsg + 1] = 0; // end flag
}

extern
void warmStartNORStats(void) {
	unsigned int powerUps;
	
	initSystemData();
	powerUps = getPowerups();
	powerUps++;
	setPowerups(powerUps);
	createMsgNameOffsets();
}

extern
void coldStartNORStats(void) {
	char period;
	
	warmStartNORStats();
	period = getPeriod();
	period++;
	setPeriod(period);
}


extern void 
exportFlashStats(void) {
	rebuildFlash(0);	
}

static int saveFlashStats(struct SystemData *sd, struct SystemCounts2 *sc, struct NORmsgMap *msgMap, struct NORallMsgStats *stats) {
	int handle, ret;
	
	handle = tbOpen((LPSTR)(SYS_DATA_STATS_PATH),O_CREAT|O_TRUNC|O_RDWR);
	if (handle != -1) {
		ret = write(handle, (unsigned long)sd<<1, sizeof(sd)<<1);
		ret = write(handle, (unsigned long)sc<<1, sizeof(sc)<<1);
		ret = write(handle, (unsigned long)msgMap<<1, sizeof(msgMap)<<1);
		ret = write(handle, (unsigned long)stats<<1, sizeof(stats)<<1);
		close(handle);
	} else
		logString((char *)"failed to write system data and stats to " SYS_DATA_STATS_PATH,BUFFER,LOG_ALWAYS);
	return ret;
}
