// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__LISTS_h__
#define	__LISTS_h__

#define LIST_ITEM_LENGTH	40
typedef struct ListItem ListItem;
typedef enum EnumListType EnumListType;

enum EnumListType {LIST_OF_LISTS, LIST_OF_PACKAGES};

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

extern void cpyListPath(char *);
extern void cpyTopicPath(char *);
extern char *getCurrentList(ListItem *);
extern char *getNextList(ListItem *, BOOL);
extern char *getPreviousList(ListItem *);
extern void setListRotation(int *, int);
extern int getListRotation(unsigned int);
extern int getListFilename(char *, int, BOOL);
extern int insertIntoList(ListItem *, long, char *);

#endif
