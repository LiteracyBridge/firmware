#ifndef	__LISTS_h__
#define	__LISTS_h__

typedef struct ListItem ListItem;
typedef enum EnumListType EnumListType;

enum EnumListType {LIST_OF_LISTS, LIST_OF_PACKAGES};

struct ListItem {
	unsigned int actionStartEnd; // see bit map for ctnrBlock below
	unsigned int idxFirstAction;
	unsigned long currentFilePosition;
    char currentString[60];
    char filename[60];
    unsigned int idxListWithFilename;
    unsigned long posListWithFilename;
    EnumListType listType;
};

extern char *getCurrentList(ListItem *);
extern char *getNextList(ListItem *, BOOL);
extern char *getPreviousList(ListItem *);
extern void setListRotation(int *, int);
extern int getListRotation(unsigned int);
extern int getListFilename(char *, int, BOOL);
extern int insertIntoList(ListItem *, long, char *);

#endif
