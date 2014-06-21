// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__LISTS_h__
#define	__LISTS_h__

#include "talkingbook.h"
#define LIST_ITEM_LENGTH	FILE_LENGTH
#define MAX_TRANSLATE_FILE  120
#define TRANSLATE_TEMP_DIR  "translate_temp"
#define OTHER_CATEGORY		"0-0"
#define FAVORITES_CATEGORY	"0-2"
#define FEEDBACK_CATEGORY	"9-0"
#define TB_CATEGORY			"$0-1"

typedef struct ListItem ListItem;
typedef struct TranslationList TranslationList;
typedef enum EnumListType EnumListType;

typedef unsigned long MLENTRY;
#define MAX_ML_ENTRIES 50

enum EnumListType {LIST_OF_LISTS, LIST_OF_PACKAGES};

struct TranslationList {
	unsigned int actionStartEnd;
	unsigned int idxFirstAction;
	char translatedFileMarker[MAX_TRANSLATE_FILE];
	//char translatedFileMarker[MAX_FILES];
	int currFileIdx;
	char mode;
	char updateOnly;
};

struct ListItem {
	unsigned int actionStartEnd; // see bit map for ctnrBlock below
	unsigned int idxFirstAction;
	unsigned long currentFilePosition;
    char currentString[LIST_ITEM_LENGTH]; // stores item in list
    char filename[FILE_LENGTH];  // stores name of list file without path
    unsigned int idxListWithFilename;
    unsigned long posListWithFilename;
    EnumListType listType;
    BOOL isLocked;
};

extern void cpyListPath(char *, char *);
extern void cpyTopicPath(char *);
extern char *getCurrentList(ListItem *);
//for some reason the definition below causes a parsing error, but it doesn't when included at the top of mainLoop.c
//extern BOOL getNextTransList(TranslationList *, int, CtnrPackage *);
extern char *getNextList(ListItem *, BOOL);
extern char *getPreviousList(ListItem *);
extern void setListRotation(int *, int);
extern int getListRotation(unsigned int);
extern int getListFilename(char *, int, BOOL);
extern void catLangDir(char *);
extern int addCategoryToActiveLists(char *, char *);
extern void setLockCat(char *, int);
#endif
