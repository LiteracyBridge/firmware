#ifndef __METADATA_H__
#define __METADATA_H__

// added for meta data 
#define CURRENT_POS 0xffffffffL
//from acm 
#define DC_CATEGORY			0
#define DC_TITLE			1
#define DC_PUBLISHER		5
#define DC_IDENTIFIER		10
#define DC_SOURCE			11
#define DC_LANGUAGE			12
#define DC_RELATION			13
#define DTB_REVISION		16
#define LB_DURATION			22
#define LB_DATE_RECORDED	25
#define LB_TIMING			27
#define	LB_PRIMARY_SPEAKER	28
#define DC_AUDIO_ITEM_ID	0x40   // change to match ACM

#define METADATA_VERSION 1
#define META_CURRENT_VERSION 1

int writeLE32(int filehandle,long value, long offset);
int writeLE16(int handle, unsigned int value, long offset);
int addField(int handle, unsigned int field_id, char *field_value, int numfiledvalues);
int metacat(char *filename, char *category);

#endif    
