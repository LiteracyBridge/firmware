#ifndef __METADATA_H__
#define __METADATA_H__

// added for meta data 
#define CURRENT_POS 0xffffffffL
#define DC_IDENTIFIER 10 
#define DC_CATEGORY    0
#define METADATA_VERSION 1

int writeLE32(int filehandle,long value, long offset);
int writeLE16(int handle, unsigned int value, long offset);
int addField(int handle, unsigned int field_id, char *field_value, int numfiledvalues);


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
