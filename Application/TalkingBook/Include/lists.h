// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__LISTS_h__
#define	__LISTS_h__

#define LIST_ITEM_LENGTH	40
#define MAX_TRANSLATE_FILE  100
#define TRANSLATE_TEMP_DIR  "translate_temp"
#define FAVORITES_CATEGORY	"FAVORITES"
#define FEEDBACK_CATEGORY	"USERS"
#define TEMPLATE_LISTS_DIR	"copy-for-new-language"
typedef struct ListItem ListItem;
typedef struct TranslationList TranslationList;
typedef enum EnumListType EnumListType;

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
extern int insertIntoList(ListItem *, long, char *);
extern void catLangDir(char *);
#endif
