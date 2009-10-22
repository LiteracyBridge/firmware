// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__TIMELINE_h__
#define	__TIMELINE_h__

typedef struct BlockTimeline BlockTimeline;

struct BlockTimeline {
	unsigned int time;
	int idxStartingEndingBlock;
	int activeBlocks[MAX_BLOCK_OVERLAP];
};

extern BlockTimeline blockTimeline[];

extern unsigned int getCurrentTimeframeStart (void);
extern EnumStartOrEnd getTimelineDiff (int, int *, int *);
extern int getIdxTimeline(unsigned int);
extern int getStartingBlockIdxFromTimeline(int);
extern void buildBlockTimelines(CtnrFile *);

#endif
