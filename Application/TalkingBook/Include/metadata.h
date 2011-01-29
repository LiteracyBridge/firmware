#ifndef __METADATA_H__
#define __METADATA_H__

// added for meta data 
#define CURRENT_POS 0xffffffffL
//from acm 
#define DC_CATEGORY			0
#define DC_TITLE			1
#define DC_CREATOR			2
#define DC_SUBJECT			3
#define DC_DESCRIPTION		4
#define DC_PUBLISHER		5
#define DC_CONTRIBUTOR		6
#define DC_DATE				7
#define DC_TYPE				8
#define DC_FORMAT			9
#define DC_IDENTIFIER		10
#define DC_SOURCE			11
#define DC_LANGUAGE			12
#define DC_RELATION			13
#define DC_COVERAGE			14
#define DC_RIGHTS			15
#define DTB_REVISION		16
#define DTB_REVISION_DATE	17
#define DTB_REVISION_DESCRIPTION	18
#define LB_COPY_COUNT		19
#define LB_PLAY_COUNT		20
#define LB_RATING			21

#define DC_AUDIO_ITEM_ID	0x40   // change to match ACM

#define METADATA_VERSION 1
#define META_CURRENT_VERSION 1

int writeLE32(int filehandle,long value, long offset);
int writeLE16(int handle, unsigned int value, long offset);
int addField(int handle, unsigned int field_id, char *field_value, int numfiledvalues);
int metacat(char *filename, char *category);

#define CAT_OTHER       "0" 
#define CAT_AGRICULTURE "1"
#define CAT_HEALTH      "2"  
#define CAT_EDUCATION   "3"
#define CAT_STORIES     "4"
#define CAT_BUSINESS    "5"
#define CAT_GOVERNANCE  "6"
#define CAT_MUSIC       "7"
#define CAT_DIARY       "8" 

#endif    
