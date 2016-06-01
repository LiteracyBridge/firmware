/*

uDOC32 compiler problem report - Sun May 24 00:24:09 2009
Please report this problem to <fae@generalplus.com>.
If possible, please attach a copy of this file with your report.

PLEASE NOTE: This file contains a preprocessed copy of the source file
that may have led to this problem occurring.

Compiler command line (u'nSP ABI):
  "C:\\PROGRA~1\\GENERA~1\\UNSPID~1.1\\toolchain\\udocc" -S -gstabs -Wall -DUSBRP -mglobal-var-iram -mISA=2.0 "-BC:\\PROGRA~1\\GENERA~1\\UNSPID~1.1\\toolchain\\" "-IE:/Documents and Settings/Cliff/My Documents/code/Cliff_code/current-20090516" "-IC:/Program Files/Generalplus/unSPIDE 3.0.1/library/include" "-IC:/Program Files/Generalplus/unSPIDE 3.0.1/library/include/sys" -I./Component/Include/FS

Version 1.1.0 build information:
  ChangeSet unknown
  Built by jerrychen@jerrychen in unknown
  Build date 2007-12-28 19:09:27 +0800

The remainder of this file contains a preprocessed copy of the
source file that appears to have led to this problem.

*/
# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c"
# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\talkingbook.h" 1





asm("APP_IRAM: .SECTION .IRAM");   


# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Component\\Include\\FS\\typedef.h" 1









































# 9 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\talkingbook.h" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\app_exception.h" 1







void logException(unsigned int, const char *, int);


# 10 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\talkingbook.h" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\util.h" 1



# 1 "C:\\Program Files\\Generalplus\\unSPIDE 3.0.1\\library\\include\\string.h" 1




# 1 "C:\\Program Files\\Generalplus\\unSPIDE 3.0.1\\library\\include\\stddef.h" 1















typedef unsigned long size_t;
typedef long ssize_t;








# 44 "C:\\Program Files\\Generalplus\\unSPIDE 3.0.1\\library\\include\\stddef.h"





# 5 "C:\\Program Files\\Generalplus\\unSPIDE 3.0.1\\library\\include\\string.h" 2



int      bcmp(const void *m1, const void *m2, size_t n);

void * bsearch(const void * key,
               const void * base,
               size_t nmemb,
               size_t size,
               int (*compar) (const void *, const void *));

void  bzero(void *b, size_t length);

void   *memchr(const void *, int, size_t);
int     memcmp(const void *, const void *, size_t);
void   *memcpy(void *, const void *, size_t);
void   *memmove(void *, const void *, size_t);
void   *memset(void *, int, size_t);
char   *index(const char *s, int c);

char   *rindex(const char *s,	int c);
char   *strdup(const char   *str);
char   *strlwr(char*);
char   * strupr (char *a);

int    strcasecmp(const char *, const char *);



char   *strcat(char *, const char *);



char   *strchr(const char *, int);



int     strncmp(const char *, const char *, size_t);



int     strncasecmp(const char *, const char *, size_t);



int     strcmp(const char *, const char *);

char    *strcpy(char *, const char *);
size_t  strcspn(const char *, const char *);
char    *strerro(int);
size_t  strlen(const char *);
char    *strncat(char *, const char *, size_t);
char    *strncpy(char *, const char *, size_t);


char     *strpbrk(const char *, const char *);

char     *strrchr(const char *, int);

size_t  strspn(const char *, const char *);

char     *strstr(const char *, const char *);
char     *strtok(char *, const char *);

size_t   strxfrm(char *, const char *, size_t);

void     swab (const void *b1,	void *b2, ssize_t length);
int      strcoll(const char *a, 	const char *b);
void     *memccpy(void *,const void *,int,size_t);











# 4 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\util.h" 2

 
 
extern int isspace(char);
extern int isdigit(char);
 
 
 
 
 
extern void *memset(void *, int, unsigned long);
extern void *memcpy(void *, const void *, unsigned long);

extern int strIndex (const char *, char);
extern int getBitShift (unsigned int);
extern unsigned long extractTime (unsigned int, int);
extern signed long extractSignedTime (signed int, int);
extern unsigned int compressTime (unsigned long, int);
extern int lower (int);
extern int strToInt (char *);
extern void longToDecimalString(long, char *, int);
extern void longToHexString(long, char *, int);
extern long strToLong (char *);
extern int goodChar(char,int);
extern int goodString(char *,int);
extern int LBstrncpy (char *, const char *, int);

# 11 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\talkingbook.h" 2


 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 






 







































typedef enum EnumStartOrEnd EnumStartOrEnd;
enum EnumStartOrEnd {NONE, STARTING, ENDING, BOTH};


 
extern int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;	
extern int LED_GREEN, LED_RED, LED_ALL;
extern int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
extern int NORMAL_VOLUME, MAX_VOLUME, VOLUME_INCREMENT;
extern char *BOOT_PACKAGE, *SYSTEM_PATH, *USER_PATH, *LIST_PATH;
extern int MAX_PWR_CYCLES_IN_LOG;
extern char *SYSTEM_VARIABLE_FILE, *LOG_FILE;
extern char *LIST_MASTER;
extern char *PKG_NUM_PREFIX, *LIST_NUM_PREFIX, *CUSTOM_PKG_PREFIX;
extern char *AUDIO_FILE_EXT;
extern char *CONTROL_EXT;
extern int DEFAULT_TIME_PRECISION;
extern int DEFAULT_REWIND;
extern int INSERT_SOUND_REWIND_MS;
extern int HYPERLINK_SOUND_FILE_IDX, DELETED_FILE_IDX, BEEP_SOUND_FILE_IDX, BIP_SOUND_FILE_IDX, SPEAK_SOUND_FILE_IDX, 
    INACTIVITY_SOUND_FILE_IDX, ERROR_SOUND_FILE_IDX, EMPTY_LIST_FILE_IDX;
extern int BLOCK_START_LEADER, BLOCK_END_LEADER;
extern long BIT_RATE;
extern int GREEN_LED_WHEN_PLAYING;
extern int INACTIVITY_SECONDS;
extern int MIC_GAIN_NORMAL, MIC_GAIN_HEADPHONE;
extern char *CONTROL_TEMPLATE;
extern char *MACRO_FILE;
extern int VOLTAGE_SAMPLE_FREQ_SEC;
extern int LOG_WARNINGS, LOG_KEYS;

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\containers.h" 1



# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\lists.h" 1



typedef struct ListItem ListItem;
typedef enum EnumListType EnumListType;

enum EnumListType {LIST_OF_LISTS, LIST_OF_PACKAGES};

struct ListItem {
	unsigned int actionStartEnd;  
	unsigned int idxFirstAction;
	unsigned long currentFilePosition;
    char currentString[40];
    char filename[40];
    unsigned int idxListWithFilename;
    unsigned long posListWithFilename;
    EnumListType listType;
};

extern char *getCurrentList(ListItem *);
extern char *getNextList(ListItem *, int  );
extern char *getPreviousList(ListItem *);
extern void setListRotation(int *, int);
extern int getListRotation(unsigned int);
extern int getListFilename(char *, int);
extern int insertIntoList(ListItem *, long, char *);


# 4 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\containers.h" 2
















 

typedef enum EnumEvent EnumEvent;
typedef enum EnumAction EnumAction;
typedef enum EnumBorderCrossing EnumBorderCrossing;
typedef struct CtnrFile CtnrFile;
typedef struct CtnrBlock CtnrBlock;
typedef struct CtnrPackage CtnrPackage;
typedef struct Action Action;
typedef struct Context Context;


 
enum EnumEvent {
	LEFT, RIGHT, UP, DOWN, SELECT, HOME, PLAY, STAR, PLUS, MINUS, BUTTON_MARKER, START, END
};
enum EnumAction {NOP = 0, STOP, PAUSE, JUMP_BLOCK, RETURN, INSERT_SOUND, START_END_MARKER,			
				PLAY_PAUSE, COPY, RECORD_TITLE, RECORD_MSG, PACKAGE_RECORDING, 
				FWD, BACK, JUMP_TIME, CALL_BLOCK, JUMP_PACKAGE, JUMP_LIST, DELETE, 
                ENTER_EXIT_MARKER, VOLUME_UP, VOLUME_DOWN, VOLUME_NORMAL, SPEED_UP, SPEED_DOWN, SPEED_NORMAL,  
				USB_MARKER, USB_DEVICE_ON, USB_HOST_ON, USB_DEVICE_OFF, USB_HOST_OFF,  
                LED_MARKER, LED_RED_ON, LED_GREEN_ON, LED_ALL_ON, LED_RED_OFF, LED_GREEN_OFF, LED_ALL_OFF,
				EOL_MARKER};  
enum EnumBorderCrossing {PLAYING, FORWARD_JUMPING, BACKWARD_JUMPING};

struct CtnrFile {
	 
	 
	int idxFilename;
	int idxFirstBlockStart;  
	int idxFirstBlockEnd;  
	int idxFirstBlockInFile;  
	 
	 
	 
	 
};

struct CtnrBlock {
	unsigned int startTime;  
	unsigned int endTime;  
	int idxFirstAction;  
	int idxNextStart;  
	int idxNextEnd;  
	int actionStartEnd;  
	int actionEnterExit;   
};

 
 
 

 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

struct Action {
 
 
	unsigned int eventAction;  
	int destination;  
	unsigned int aux;   
	 
	 
	 
	 
};

 
 
 
 
 
 
 

 
 
 
 
 


struct CtnrPackage {
	int idxName;   
	int pkg_type;  
    CtnrFile tempFile;  
	CtnrFile files[40 ]; 
	CtnrBlock blocks[70 ];
	Action actions[140 ];
	ListItem lists[5 ];
	int countPackageActions;
	int countFiles;
	int countBlocks;
	int countLists;
	int idxMasterList;	
	int blockBookmark;   
	char strHeapStack[512 + 100 ];
	int idxStrHeap;
	int timePrecision;  
	int   recInProgress;
	int idxLanguageCode;
	 
	 
	 
	 
};

struct Context {
	 
	CtnrPackage *package;
	CtnrFile *file;
	int idxTimeframe;  
	int idxPausedOnBorder;  
	unsigned int timeNextTimeframe;  
	int keystroke;  
	int   isPaused;
	int   isStopped;
	int   isHyperlinked;  
	int   USB;
	int queuedPackageType;
	int queuedPackageNameIndex;
	int idxActiveList;  
	CtnrPackage *returnPackage;
    CtnrFile *lastFile;
    long packageStartTime;
};

extern Context context;
extern CtnrPackage pkgSystem, pkgUser, pkgDefault; 



extern int   stackPush (CtnrPackage *, CtnrFile *, int);
extern int   stackPop(CtnrPackage **, CtnrFile **, int *);
extern void setRewind(int *, int);
extern int getRewind(unsigned int);
extern int getSoundInsertIdxFromAux(int);
extern void setStartCode(int *, EnumAction);
extern void setEndCode(int *, EnumAction);
extern CtnrBlock *getStartInsert(int, int);
extern CtnrBlock *getEndInsert(int, int);
extern CtnrBlock* getNextStartBlock(CtnrBlock *);
extern void setNextStartBlock(int, int);
extern CtnrBlock* getNextEndBlock(CtnrBlock *);
extern CtnrBlock* getNextStartBlockFromTime(unsigned int);
extern CtnrBlock* getNextEndBlockFromTime(unsigned int);
extern CtnrBlock* getStartBlockFromFile(CtnrFile *);
extern CtnrBlock* getEndBlockFromFile(CtnrFile *);
extern CtnrPackage* getPackageFromFile (CtnrFile *);
extern CtnrFile* getFileFromBlock (CtnrBlock *);
extern void setBlockHyperlinked(CtnrBlock *);
extern int   isBlockHyperlinked(CtnrBlock *);
extern Action* getNextAction(Action *);
extern EnumAction getStartCode(CtnrBlock *, int *, EnumBorderCrossing);
extern EnumAction getEndCode(CtnrBlock *, int *, EnumBorderCrossing);
extern void setSoundInsert(Action *, int  );
extern int   hasSoundInsert(Action *);
extern int   isEventInAction(Action *, EnumEvent, int  );
extern EnumAction getActionCode(Action *);
extern void setActionCode(Action *, EnumAction);
extern void setEventCodes(Action *, EnumEvent, int  );
extern void setEndOfActions(Action *, int  );
extern Action *getBlockActions(CtnrBlock *);
extern Action *getListActions(ListItem *);
extern int replaceStack (char *, CtnrPackage *);
extern CtnrFile *getListFile(char *); 


# 2 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\timeline.h" 1



typedef struct BlockTimeline BlockTimeline;

struct BlockTimeline {
	unsigned int time;
	int idxStartingEndingBlock;
	int activeBlocks[4 ];
};

extern BlockTimeline blockTimeline[];

extern unsigned int getCurrentTimeframeStart (void);
extern EnumStartOrEnd getTimelineDiff (int, int *, int *);
extern int getIdxTimeline(unsigned int);
extern int getStartingBlockIdxFromTimeline(int);
extern void buildBlockTimelines(CtnrFile *);


# 3 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\device.h" 1



# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\component_head.h" 1



# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\FS\\typedef.h" 1
# 41 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\FS\\typedef.h"

# 4 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\component_head.h" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\FS\\vfs.h" 1






# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\FS\\Typedef.h" 1
# 41 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\FS\\Typedef.h"

# 7 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\FS\\vfs.h" 2


 
 
 






 




     









 
 








 

 







    							 













 

 
 
 
 
struct stat_t 
{	
	unsigned int 	st_mode;
	long 	st_size;
	unsigned long 	st_mtime;
};

 
struct _diskfree_t {
	unsigned long  total_clusters;
	unsigned long  avail_clusters;
	unsigned long  sectors_per_cluster;
	unsigned long  bytes_per_sector;
};

struct deviceinfo 	 {
	char  device_name[16];		 
	char  device_enable;			 
	char  device_typeFAT;		 
	unsigned long  device_availspace;	 
	unsigned long  device_capacity;		 
};

 
struct timesbuf 	 {
	unsigned int  modtime;
	unsigned int  moddate;
	unsigned int  accdate;
};

 
struct statbuf 	 {
	unsigned long  st_size;
	unsigned char  st_mode;
	unsigned int  crtime;
	unsigned int  crdate;
	unsigned int  modtime;
	unsigned int  moddate;
	unsigned int  accdate;
};

struct f_info         
{
	unsigned char  f_attrib;
	unsigned int   f_time;
	unsigned int   f_date;
	unsigned long  f_size;
	unsigned int   entry;			 

	 char  f_name[256]; 	  



};

typedef struct        
{
	char     f_dsk;
	unsigned long   f_entry;
	unsigned int   f_offset;
}f_pos, *f_ppos;

struct sfn_info
{
    unsigned char    f_attrib;
	unsigned int   f_time;
	unsigned int   f_date;
	unsigned long   f_size;
    char     f_name[9];
    char     f_extname[4];
    f_pos	f_pos;
};

struct nls_table {
	char 			*charset;						 
	unsigned int 			Status;							 
	int 			(*init_nls)(void);				 
	int 			(*exit_nls)(void);				 
	unsigned int 			(*uni2char)(unsigned int  uni);		 
	unsigned int 			(*char2uni)(unsigned char  **rawstring);	 
};

 
 
 
 
 
 
 
 
struct STFileNodeInfo
{
	char 		flag;					 
	char 		disk;					 
	char 		extname[4];				 
	unsigned int *		FileBuffer;				 
	unsigned int 		FileBufferSize;			 
	unsigned int *		FolderBuffer;			 
	unsigned int 		FolderBufferSize;		 
};

 
 
 
 
f_ppos getfirstfile(int  dsk, char * extname, struct sfn_info* f_info, int  attr);
 
f_ppos getnextfile(int  dsk, char * extname, struct sfn_info* f_info, int  attr);
 
int  sfn_open(f_ppos ppos);
 
int  sfn_stat(int  fd, struct sfn_info *sfn_info);
 
int  GetFileInfo(f_ppos ppos, struct f_info *f_info);
 
int  GetFolderInfo(f_ppos ppos, struct f_info *f_info);
 
int  sfn_unlink(f_ppos ppos);

 
int  StatFileNumByExtName(int  dsk, unsigned long   extname, unsigned long  *filenum);
 
int  GetFileNumEx(struct STFileNodeInfo *stFNodeInfo, unsigned long  *nFolderNum, unsigned long  *nFileNum);
 
 
 
f_ppos GetFileNodeInfo(struct STFileNodeInfo *stFNodeInfo, unsigned long  nIndex, struct sfn_info* f_info);
 
 
 
int  GetFileNumOfFolder(struct STFileNodeInfo *stFNodeInfo, unsigned long  nFolderIndex, unsigned int * nFile);
 
 
 
int  FolderIndexToFileIndex(struct STFileNodeInfo *stFNodeInfo, unsigned long  nFolderIndex, unsigned long * nFileIndex);
 
 
 
int  FileIndexToFolderIndex(struct STFileNodeInfo *stFNodeInfo, unsigned long  nFileIndex, unsigned long * nFolderIndex);
 
 
f_ppos GetFolderNodeInfo(struct STFileNodeInfo *stFNodeInfo, unsigned long  nFolderIndex, struct sfn_info* f_info);

 
void dosdate_decode(unsigned int  dos_date, unsigned int  *pyear, unsigned char  *pmonth, unsigned char  *pday);
 
void dostime_decode(unsigned int   dos_time, unsigned char  *phour, unsigned char  *pminute, unsigned char  *psecond);

 
char  *time_decode(unsigned int  *tp, char  *timec);
 
char  *date_decode(unsigned int  *dp, char  *datec);

 
void fs_safexit(void);
 
void fs_registerfd(int  fd);

 
void disk_safe_exit(int  dsk);

int  close(int  fd);

int  open(unsigned long   path , int  open_flag);

long  read(int  fd , unsigned long   buf , unsigned long  size);
long  write(int  fd , unsigned long   buf ,  unsigned long  size);

long  lseek(int  fd ,long  offset ,int  fromwhere);
long  tell(int  fd); 

int  unlink(unsigned long   filename);
int  rename(unsigned long   oldname , unsigned long   newname);

int  readB(int  fd , unsigned long   buf , unsigned int  size);
int  writeB(int  fd , unsigned long   buf ,  unsigned int  size);

int  mkdir(unsigned long   pathname);

int  rmdir(unsigned long   pathname);
int  chdir(unsigned long   path);
unsigned long   getcwd(unsigned long   buffer , int  maxlen );

int  fstat(int  handle , struct stat_t *statbuf);
int  stat(unsigned long   path, struct stat_t *statbuf);

int  _findfirst(unsigned long   name , struct f_info *f_info ,  unsigned int  attr);
int  _findnext(struct f_info *f_info);

int  _getdiskfree(int  dsk, struct _diskfree_t *st_free);


int  fs_changedisk ( unsigned char  disk);  
 
long  vfsFreeSpace(int  driver);
int  _copy(unsigned long   srcfile , unsigned long   destfile);

void fs_init(void);
int  fs_uninit();

unsigned int  tellcurrentfiledir(void);
unsigned int  telldir(void);
void seekdir( unsigned int  pos);      
void rewinddir(void);

 
int  _setfattr (unsigned long   filename ,  unsigned int  attr);
 
int  _setdirattr (unsigned long   dirname ,  unsigned int  attr); 
 
int  _getdirattr (unsigned long   dirname ,  unsigned int  *attr); 

int  _devicemount(int  disked);
int  _deviceunmount(int  disked);

int  _getfserrcode ();
void _clsfserrcode ();

int  _format ( unsigned char  drv ,  unsigned char  fstype);
int  _deleteall (unsigned long   filename);

 
long  _GetCluster(int  fd);
 
long  Clus2Phy( unsigned int  dsk ,  unsigned long  cl_no);

 
 
 
int  DeletePartFile(int  fd, unsigned long  offset, unsigned long  length);
 
int  InserPartFile(int  tagfd, int  srcfd, unsigned long  tagoff, unsigned long  srclen);
 
int  SplitFile(int  tagfd, int  srcfd, unsigned long  splitpoint);

 
unsigned int  GetSectorsPerCluster(unsigned int  dsk);

 
unsigned int  ChangeCodePage(unsigned int  wCodePage);
unsigned int  GetCodePage(void);
int  ChangeUnitab(struct nls_table *st_nls_table);

 
int  checkfattype (unsigned char  disk);

 
int  UpdataDir(int  fd);

 
 
int  FileRepair(int  fd);

 
 
 
int  sformat(unsigned char  drv, unsigned long  totalsectors, unsigned long  realsectors);

 
int  GetDiskOfFile(int  fd);

 
 
 
int  CreatFileBySize(unsigned long   path ,unsigned long  size);


# 5 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\component_head.h" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\speech\\algorithm.h" 1


 



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 
 
 


	



 































































































































 
 






# 6 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\component_head.h" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\speech\\sacm_API.h" 1


 































 
 
 






 







 








 
 
 













 





















 


 
 
 
 
 
 
extern void SACM_Initial(void);
extern void Snd_VarInit(void);
extern int Snd_SACM_PlayMemory(int _CodecType, unsigned long start_addr);
extern int Snd_SACM_RecMemory(int _CodecType, unsigned long RECStartAddr, int BitRate);
extern int Snd_SACM_PlayFAT(int FileHandle, int _CodecType);		 
extern int Snd_SACM_RecFAT(int FileHandle, int _CodecType, int BitRate);
extern int Snd_Stop(void);						 
extern void SACM_Pause(void);					 
extern void SACM_Resume(void);					 
extern void SACM_Volume(int);					 
extern void SACM_Speed(int SpeedIndex);			 
extern unsigned int SACM_Status(void);			 
 
extern int SACM_Get_Codec( void );
extern int SACM_Get_Speed( void );

 
extern unsigned int SACMGet_A1800FAT_Mode( int handle );
extern unsigned int SACMGet_A4800FAT_Mode( int handle );
extern void SACM_DVR1800_BITRATE(int BitRate);			 
extern int SACM_A1800FAT_SeekTime( long milisecond, int orientation );
extern unsigned long Snd_A1800_GetCurrentTime(void);
 
extern unsigned int CheckStatus_Snd(void);
extern int NFAT_DS_GetData( unsigned long   dest_addr,unsigned int length);
extern void NFAT_DS_WriteData_Rec( unsigned long   dest_addr,unsigned int length);
extern void USER_SetFSLength(unsigned long);

 
extern int Snd_ETTS_Play(unsigned long Text);
extern void Snd_SACM_SetSpeedTTS( int SpeedIndex );	  
extern int Snd_SACM_GetSpeedTTS( void );
extern void Snd_SACM_SetToneTTS( int personalities ); 

 
extern int Snd_A128_PlayMemory(unsigned long start_addr, unsigned long end_addr);
extern int Snd_A128_PlayFAT(const char *filename);

# 7 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\component_head.h" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\speech\\MP3.h" 1



typedef struct{
	unsigned long   firstframe;			 
	unsigned long   filesize;			 
	unsigned long   frametime;			 
	unsigned int 	bitrate;			 
	unsigned int 	mpegtype;			 
	unsigned int 	layer;				 
	unsigned int 	samplerate; 		 
	unsigned int 	framesize;			 
 
	unsigned long 	total_time;			 
}st_MP3FILE_INFO;

typedef struct Node
{
	unsigned long 	MusicFilePath;	 
	unsigned int 	FileID;			 
	struct 			Node *Next;
	struct 			Node *Back;
	unsigned int 	Param;			 
}List;

extern unsigned int Snd_MP3_Status;

extern int Snd_MP3_PlayMemory(unsigned long start_addr, unsigned long end_addr);
extern int Snd_PlayMP3FAT( const char * filename );		 
extern int Snd_PlayMP3FAT_Part(char *filename, unsigned long offset, unsigned long datalength);
extern void PlayNextFile( void );
extern int GetMp3Info( const char * FilePath, st_MP3FILE_INFO *mp3file_info );
extern void MediaPlayer_SetA( void );
extern void MediaPlayer_SetB( void );
extern void MediaPlayer_ClearAB( void );
extern void MediaPlayer_PlayAB( void );
 
extern int Snd_PlayMP3FATTimeOffset(const char *filename, unsigned long Offset, unsigned long length, unsigned int flag); 

extern void MediaPlayer_PlayBack(void);
extern void MediaPlayer_PlayNext(void);
extern void MediaPlayer_SetPlayMode(unsigned int mode);
extern int Snd_MP3Player_SeekTime(long second,int orientation);
extern int Snd_MP3Player_SeekLength(long mp3length,int orientation);
extern int MediaPlayer_PlayList(List *Head,unsigned int FileID);
extern int GetCurPlayTime(void);
extern unsigned long GetCurPlayms(void);

extern void Snd_MP3Speech_Pause(void);
extern void Snd_MP3Speech_Resume(void);
 






 






 




# 79 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\speech\\MP3.h"



	




	
# 100 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\speech\\MP3.h"



# 8 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\component_head.h" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\speech\\SACM_GV.h" 1








	


 

extern  unsigned int pMP3Readptr, uiMP3WriteFlag;	 
extern  unsigned int MP3DecodeErrorFlag, MP3ReadModeFlag; 
extern  unsigned int R_MPEGType;				 


 
 
extern  List *MusicPlayList;				 
extern  List *MusicPlayList_Head;			 

 
extern  unsigned int MusicPlayMode;			 
extern  unsigned int Snd_MP3_Status;		 
extern  unsigned int gCurSecondTime;		 
extern  unsigned int Mp3_ContinueFlag;		 

extern  unsigned int R_MPEGLayer;			 
extern  unsigned int R_SampleRate;			 
extern  unsigned int R_MPEGBitRate;			 
extern  unsigned int R_MPEGType;			 

extern  unsigned long MP3Filelength;		 
extern  unsigned long gMP3FATLength;		 
extern  unsigned long ulFrame_Count_MP3;	 
extern  unsigned long gMP3_FileOffset;		 
extern  unsigned long MP3Former_Length;		 
extern  unsigned long MP3Decode_Length;		 

extern  unsigned long ulFrame_CurrentCount;	 
extern  unsigned  long ulFrame_Count_A;		 
extern  unsigned long ulFrame_Count_B;
extern  unsigned long gMP3_FramSizeA;		 
 

extern  unsigned int SACM_A1800_Mode;		 
extern  unsigned int SACM_A4800_Mode;		 

 
extern  unsigned int gCurA128SecondTime;	 
extern  unsigned int R_BitRate_A128;	 
 

 
extern  unsigned int pRecWriteptr;
extern  unsigned int uiRecWriteFlag;	

extern  unsigned int R_ShiftStore;
extern  unsigned int R_DataEmptyFlag;
extern  unsigned int R_SACM_DecodeData_In_PTR;
extern  unsigned int R_ISR_DataProc_Flag;			 
extern  unsigned int R_MP3_Error_Count;				 

# 9 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\.\\Component\\Include\\component_head.h" 2



# 4 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\device.h" 2






extern int SystemIntoUSB(unsigned int);

extern int KEY_PLAY, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_SELECT, KEY_STAR, KEY_HOME, KEY_PLUS, KEY_MINUS;	
extern int LED_GREEN, LED_RED, LED_ALL;
extern int MAX_SPEED, NORMAL_SPEED, SPEED_INCREMENT;
extern int NORMAL_VOLUME, MAX_VOLUME, VOLUME_INCREMENT;

extern void resetRTC(void);
extern long getRTCinSeconds(void);
extern void setLED(unsigned int, int  );
extern int restoreVolume(int  );
extern int adjustVolume (int, int  , int  );
extern int adjustSpeed (int, int  );
extern int getVolume(void);
extern int getSpeed(void);
extern void setUSBDevice (int  );
 
extern void logVoltage(long);
extern int keyCheck(int);
extern int waitForButton(int);
extern void wait(int);
extern void resetSystem(void);


# 4 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\files.h" 1









extern void logString(char *, int);
extern char *getLine (int, char *);
extern int   readBuffer(int, char *, int);
extern int   nextNameValuePair (int, char *, char, char **, char **);
extern void clearStaleLog(void);
extern void flushLog(void);
extern int appendStringToFile(const char *, char *);
 
extern int findDeleteStringFromFile(char *, char *, char *, int  );
extern long getFilePosition(void);
extern int  tbOpen(unsigned long  , int );
extern int  tbChdir(unsigned long  );
extern int fileExists(unsigned long  );
extern int dirExists(unsigned long  );
extern int convertDoubleToSingleChar(char *, const char *, int  );

# 5 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\startup.h" 1



extern void startUp(void);


# 6 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\sys_counters.h" 1



typedef struct SystemCounts SystemCounts;

struct SystemCounts {
	unsigned long powerUpNumber;
	unsigned long packageNumber;
	unsigned long listNumber;
	unsigned long lastLogErase;
};

extern SystemCounts systemCounts;

extern void saveSystemCounts();
extern int loadSystemCounts(void);
extern void getPkgNumber(char *, int  );


# 7 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\audio.h" 1





extern void stop(void);
extern void play(CtnrFile *, unsigned int);
extern void insertSound(CtnrFile *, CtnrBlock *, int  );
extern void insertSoundFile(int);
extern int createRecording(char *, int, char *);
extern void markEndPlay(long);
extern void markStartPlay(long, const char *);


# 8 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\pkg_ops.h" 1



extern void packageRecording(char *, char *);
extern int deletePackage (char *);


# 9 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\parsing.h" 1






void parseControlFile (char *, CtnrPackage *);


# 10 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\macro.h" 1



typedef struct MacroInstr MacroInstr;
typedef struct MacroLoop MacroLoop;

struct MacroInstr{
	unsigned int wait;
	unsigned int key;
	char log;
};

struct MacroLoop {
	unsigned int times;
	unsigned int begin;
	unsigned int end;
};

extern void loadMacro(void);
extern int nextMacroKey (void);


# 11 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\d2d_copy.h" 1



extern int d2dCopy(const char *, const char *);


# 12 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2

# 1 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\Include\\mainLoop.h" 1



extern void mainLoop (void);

# 13 "E:\\Documents and Settings\\Cliff\\My Documents\\code\\Cliff_code\\current-20090516\\Application\\TalkingBook\\mainLoop.c" 2


typedef enum EnumEnterOrExit EnumEnterOrExit;
enum EnumEnterOrExit {ENTERING, EXITING};

static void processBlockEnterExit (CtnrBlock *, EnumEnterOrExit);
static void processTimelineJump (int, int);
static void enterOrExitAllBlocks (int, EnumEnterOrExit);
static Action *getMatchingAction (EnumEvent);
static void processButtonEvent(int);
static void processStartBlock(int);
static void processIntoBlock(int);
static EnumAction processEndBlock(int);
static void endOfTimeframe(int, int  );
static void keyResponse(void);
static int checkInactivity(int  );
static void takeAction (Action *, EnumAction);
static void loadPackage(int, const char *);


static EnumAction getStartEndCodeFromTimeframe(int idxTimeframe, EnumBorderCrossing approach, int *actionTime, int *idxAction) {
	 
	 
	 
	 
	EnumStartOrEnd startOrEnd;
	EnumAction actionCode;
	int idxStart, idxEnd;
	int   skipHyperlinkedBlock;
	CtnrBlock *blocks = context.package->blocks;

	if (approach == FORWARD_JUMPING) {
		do {
			skipHyperlinkedBlock = (1==0) ;
			if (blockTimeline[++idxTimeframe].time != -1) {
				startOrEnd = getTimelineDiff(idxTimeframe,&idxStart,&idxEnd);
				if (startOrEnd == STARTING && isBlockHyperlinked(&blocks[idxStart]))
					skipHyperlinkedBlock = (1==1) ;
				else if (startOrEnd == ENDING && isBlockHyperlinked(&blocks[idxEnd]))
					skipHyperlinkedBlock = (1==1) ;
				else if (startOrEnd == BOTH && isBlockHyperlinked(&blocks[idxStart]) && isBlockHyperlinked(&blocks[idxEnd]))
					skipHyperlinkedBlock = (1==1) ;
			}   
		} while(skipHyperlinkedBlock);
	}
	else if (approach == BACKWARD_JUMPING) {
		do {
			skipHyperlinkedBlock = (1==0) ;
			if (idxTimeframe >= 0) {
				startOrEnd = getTimelineDiff(idxTimeframe--,&idxStart,&idxEnd);
				if (startOrEnd == STARTING && isBlockHyperlinked(&blocks[idxStart]))
					skipHyperlinkedBlock = (1==1) ;
				else if (startOrEnd == ENDING && isBlockHyperlinked(&blocks[idxEnd]))
					skipHyperlinkedBlock = (1==1) ;
				else if (startOrEnd == BOTH && isBlockHyperlinked(&blocks[idxStart]) && isBlockHyperlinked(&blocks[idxEnd]))
					skipHyperlinkedBlock = (1==1) ;
			}   
		} while(skipHyperlinkedBlock);
	}
	if (idxTimeframe == -1) {
		 
		*actionTime = 0;
		*idxAction = -1;
		actionCode = STOP;
	} else if (blockTimeline[idxTimeframe].time == -1) {
		 
		getTimelineDiff(idxTimeframe+1,&idxStart,&idxEnd);
		*actionTime = blocks[idxEnd].endTime;
		*idxAction = -1;
		actionCode = STOP;
	} else {
		switch (startOrEnd) {
			case STARTING:
				actionCode = getStartCode(&blocks[idxStart],idxAction,approach);
				break;
			case ENDING: 
				actionCode = getEndCode(&blocks[idxEnd],idxAction,approach);
				break;
			case BOTH:
				if (approach == FORWARD_JUMPING)
					actionCode = getEndCode(&blocks[idxEnd],idxAction,approach);
				else  
					actionCode = getStartCode(&blocks[idxStart],idxAction,approach);
				break;
			case NONE:
				 
				break;					
		}
	}
	return actionCode;
} 

static void processBlockEnterExit (CtnrBlock *block, EnumEnterOrExit enteringOrExiting) {
	unsigned int code, LEDhyperlink, speedChange, volumeChange;
	int   red, green, onoff, fliponoff;
	__attribute__((section(".APP_IRAM")))  static unsigned int enteringVolume;

	code = block->actionEnterExit;
	if (enteringOrExiting == ENTERING) {
		code &= 0x00FF;
		enteringVolume = getVolume();
	}
	else {
		if (context.isHyperlinked && ((code & 0x00FF) == 0x04)) {  
			context.isHyperlinked = (1==0) ;
			setLED(LED_RED,(1==0) );
		}
		code >>= 8;
	}

	if (code) {
		LEDhyperlink = code & 0x0F;  
		speedChange  = (code & 0x30) >> 4;   
		volumeChange = (code & 0xC0) >> 6;  
	
		switch (speedChange) {
			case 0x01:
				adjustSpeed(3,(1==1) );
				break;
			case 0x02:
				adjustSpeed(-3,(1==1) );
				break;
			case 0x03:
				adjustSpeed(NORMAL_SPEED,(1==0) );
				break;
		}
		switch (volumeChange) {
			case 0x01:
				adjustVolume(3,(1==1) ,(1==1) );
				break;
			case 0x02:
				adjustVolume(-3,(1==1) ,(1==1) );
				break;
			case 0x03:
				 
				restoreVolume((1==0) );
				break;
		}   
		if (LEDhyperlink == 0x04) {
			if (enteringOrExiting == ENTERING) {
				context.isHyperlinked = (1==1) ;
				context.USB = (1==0) ;
				setLED(LED_RED,(1==1) );
				insertSound(&pkgSystem.files[HYPERLINK_SOUND_FILE_IDX],((void*)0) ,(1==1) );
			}
			else {
				context.isHyperlinked = (1==0) ;
				setLED(LED_RED,(1==0) );
			}
		}
		else {
			if (LEDhyperlink == 0x08) { 
				context.USB = (1==1) ;
				stop();
				setUSBDevice((1==1) );
			}
			else {
				green = LEDhyperlink & 0x01;
				red = LEDhyperlink & 0x02;
				onoff = LEDhyperlink & 0x04;
				fliponoff = LEDhyperlink & 0x08;
				
				if ((green && onoff) || (green && fliponoff))	
					setLED(LED_GREEN,(1==1) );
				else if ((green && !onoff) || (!green && fliponoff))	
					setLED(LED_GREEN,(1==0) );
				if ((red && onoff) || (red && fliponoff))
					setLED(LED_RED,(1==1) );
				else if ((red && !onoff) || (!red && fliponoff))	
					setLED(LED_RED,(1==0) );
			}
		}			
	}   
}	

static void processTimelineJump (int idxFromTimeline, int idxToTimeline) {
	 
	 
	int idxActiveBlock, idxBlock;
	
	if (idxFromTimeline != idxToTimeline) {
		if (idxFromTimeline != -1)
			for (idxActiveBlock= 4 -1;idxActiveBlock >= 0; idxActiveBlock--) {
				idxBlock = blockTimeline[idxFromTimeline].activeBlocks[idxActiveBlock];
				if (idxBlock != -1)
					processBlockEnterExit (&context.package->blocks[idxBlock], EXITING);
			}
		context.idxTimeframe = idxToTimeline;
		context.timeNextTimeframe = blockTimeline[idxToTimeline + 1].time;
		if (idxToTimeline != -1)
			for (idxActiveBlock=0;idxActiveBlock < 4 ; idxActiveBlock++) {
				idxBlock = blockTimeline[idxToTimeline].activeBlocks[idxActiveBlock];
				if (idxBlock == -1)
					break;
				else 
					processBlockEnterExit (&context.package->blocks[idxBlock], ENTERING);
		}
	} 
}

static void enterOrExitAllBlocks (int idxTimeline, EnumEnterOrExit enteringOrExiting) {
	 
	 
	int idxActiveBlockList, idxBlock;
	
	if (idxTimeline != -1) {	
		idxActiveBlockList = 0;
		do {
			idxBlock = blockTimeline[idxTimeline].activeBlocks[idxActiveBlockList++];
			if (idxBlock != -1)
				processBlockEnterExit(&context.package->blocks[idxBlock], enteringOrExiting);
			else
				break;
		}
		while (idxActiveBlockList < 4 );
	}	
}

static Action *getMatchingAction (EnumEvent eventType) {
	Action *iAction;	
	int idxBlock;
	int idxActiveBlock;
	int arrayIndex;
	int *activeBlocks;
	iAction = ((void*)0) ;
	
     
	if (eventType < BUTTON_MARKER) {   
		arrayIndex = context.idxTimeframe;   
		 
		if (arrayIndex != -1) {  
			activeBlocks = blockTimeline[arrayIndex].activeBlocks;
			idxActiveBlock = 4 ;
			do {
				idxBlock = activeBlocks[--idxActiveBlock];
				if (idxBlock != -1) {
					iAction = getBlockActions(&context.package->blocks[idxBlock]);
					while (iAction && !isEventInAction(iAction,eventType,context.isPaused))
						iAction = getNextAction(iAction);
				}
			} while (!iAction && (idxActiveBlock > 0));
		}
		 

		 
		if (iAction == ((void*)0) ) {
			if (context.package->countPackageActions) 
				iAction = context.package->actions;		 
 			while (iAction && !isEventInAction(iAction,eventType,context.isPaused)) 
				iAction = getNextAction(iAction);
		}					
		 
		if ((iAction == ((void*)0) ) && (context.package != &pkgSystem) && pkgSystem.countPackageActions) {
			iAction = pkgSystem.actions; 
			while (iAction && !isEventInAction(iAction,eventType,context.isPaused))
				iAction = getNextAction(iAction);
			if (iAction) {
				context.returnPackage = context.package;
			    context.package = &pkgSystem;
			}
		}	
	}  
	else {
 
			 
 
			 
	}
	return iAction;
}

static void processButtonEvent(int eventType) {
	Action *action = ((void*)0) ;
	EnumAction actionCode;
	
	if (context.idxActiveList != -1) {
		action = getListActions(&pkgSystem.lists[context.idxActiveList]);
		while (action && !isEventInAction(action,eventType,context.isPaused))
			action = getNextAction(action);
	}
	if (!action)
		action = getMatchingAction(eventType);

	if (action)
		takeAction(action, -1);
	if (context.returnPackage) {
		if (action) {
			actionCode = getActionCode(action);
			if (actionCode == VOLUME_UP || actionCode == VOLUME_DOWN || actionCode == VOLUME_NORMAL
				|| actionCode == SPEED_UP || actionCode == SPEED_DOWN || actionCode == SPEED_NORMAL
				|| actionCode == JUMP_TIME || actionCode == PLAY_PAUSE || actionCode == PAUSE)
					context.package = context.returnPackage;  
		}
		context.returnPackage = ((void*)0) ;
	}
}

static void processStartBlock(int idxBlock) {
	EnumAction actionCode;
	int idxAction;
	CtnrBlock *block, *insertBlock;

	block = &context.package->blocks[idxBlock];
	insertBlock = getStartInsert(block->actionStartEnd, block->idxFirstAction);
	if (insertBlock)
		insertSound(getFileFromBlock(insertBlock),insertBlock,(1==0) );
	actionCode = getStartCode(block,&idxAction,STARTING);
	if (idxAction >= 0)
		takeAction(&context.package->actions[idxAction],actionCode);
	else
		takeAction(((void*)0) ,actionCode);
}


static void processIntoBlock(int idxBlock) {
	 
	 
	CtnrBlock *block;
	block = &context.package->blocks[idxBlock];
	
	processBlockEnterExit(block,ENTERING);
	processStartBlock(idxBlock);
}

static EnumAction processEndBlock(int idxBlock) {
	 
	 

	EnumAction actionCode;
	int idxAction;
	CtnrBlock *block, *insertBlock;
	block = &context.package->blocks[idxBlock];
	
	insertBlock = getEndInsert(block->actionStartEnd, block->idxFirstAction);
	if (insertBlock)
		insertSound(getFileFromBlock(insertBlock),insertBlock,(1==0) );
	actionCode = getEndCode(block,&idxAction,PLAYING);
	if (actionCode != NOP) {
		if (actionCode == PAUSE)
			context.idxPausedOnBorder = idxBlock;
		if (idxAction >= 0)
			takeAction(&context.package->actions[idxAction],actionCode);
		else
			takeAction(((void*)0) ,actionCode);	
	}
	return actionCode;
}

static void endOfTimeframe(int idxTimeframe, int   isPlayerStopped) {
	 
	 
	 
	 
	 
	 
	EnumStartOrEnd startOrEnd;
	unsigned int nextTimeframe;
	int idxStart = -1, idxEnd = -1;
	EnumAction endAction = EOL_MARKER;   
	
	startOrEnd = getTimelineDiff(idxTimeframe+1,&idxStart,&idxEnd);
	if (context.idxPausedOnBorder != idxEnd && (startOrEnd == ENDING || startOrEnd == BOTH)) 
		endAction = processEndBlock(idxEnd);
	if (endAction == NOP || (context.idxPausedOnBorder == idxEnd && endAction == EOL_MARKER)) {
		 
		 
		context.idxPausedOnBorder = -1;
		processBlockEnterExit(&context.package->blocks[idxEnd],EXITING);
		if (!isPlayerStopped) {   
			context.idxTimeframe++;   
			if (idxTimeframe < 140 )
				nextTimeframe = blockTimeline[context.idxTimeframe+1].time;
			else
				nextTimeframe = -1;
			context.timeNextTimeframe = nextTimeframe;
		}
	}
	if (startOrEnd == STARTING || startOrEnd == BOTH)
		processIntoBlock(idxStart);
 


}

static void keyResponse(void) {
	 
	int keystroke;
	
	if (context.keystroke) {
		keystroke = context.keystroke;
		context.keystroke = 0;
	} else
		keystroke = keyCheck(0);
	
	if (keystroke)
		checkInactivity((1==1) );
	
	if (keystroke == KEY_PLAY) {	
		processButtonEvent(PLAY);
	} else if (keystroke == KEY_LEFT) {
		 
		processButtonEvent(LEFT);
	} else if(keystroke == KEY_RIGHT) {						
		processButtonEvent(RIGHT);
	} else if (keystroke == KEY_UP) {
		processButtonEvent(UP);
	} else if (keystroke == KEY_DOWN) {
		processButtonEvent(DOWN);
	} else if (keystroke == KEY_SELECT) {
		processButtonEvent(SELECT);
	} else if (keystroke == KEY_HOME) {
		processButtonEvent(HOME);
	} else if (keystroke == KEY_STAR) {
		processButtonEvent(STAR);	
	} else if (keystroke == KEY_PLUS) {
		processButtonEvent(PLUS);	
	} else if (keystroke == KEY_MINUS) {
		processButtonEvent(MINUS);	
	}
}

static int checkInactivity(int   resetTimer) {
	long currentTime;
	__attribute__((section(".APP_IRAM")))  static int   warnedUser;
	__attribute__((section(".APP_IRAM")))  static long lastActivity;

	currentTime = getRTCinSeconds();	
	logVoltage(currentTime);

	if (resetTimer) {
		lastActivity = currentTime;
		warnedUser = (1==0) ;
	} else if (!warnedUser && currentTime - lastActivity > INACTIVITY_SECONDS) {
		adjustVolume(MAX_VOLUME,(1==0) ,(1==1) );   
		insertSound(&pkgSystem.files[INACTIVITY_SOUND_FILE_IDX],((void*)0) ,(1==0) );
		restoreVolume((1==0) );
		lastActivity = getRTCinSeconds();
 




		 
		 
	} 
	 







}

void mainLoop (void) {
	CtnrBlock *insertBlock;
	ListItem *list;
	int inactivityCheckCounter = 0;
	int ret;
	extern int SACMFileHandle;

	startUp();

	ret = tbOpen((unsigned long  )"a:\\\\user\\africa-my-africa.a18",0x0002 );		
	SACMGet_A1800FAT_Mode(ret);
	if(SACMFileHandle >= 0)
	{
		close(SACMFileHandle);
		SACMFileHandle = -1;
	}
	SACMFileHandle = ret;
	SACM_Codec(0 +0 +0 +0 +0 +0 +0 +0 +0 +0 +0 +0  + 0  + 0  + 0  + 0  + 0  + 0  + 0 );
	Fat_SetStartAddr(({	register union {	unsigned long oval;	unsigned int ival[2];	}__t;	__asm__ ("%0 = offset %2\n"	"\t%1 = seg %2"	:	:"r"(__t.ival[0]),"r"(__t.ival[1]),	"p"(& NFAT_DS_GetData )	);	__t.oval;	}) );
	
	lseek(ret,100,0 );
	SACM_Play(-1 ,1 + 2 ,0);
 
	

 
 

	while(1) {
		 
		if (context.queuedPackageType > 0 ) {
			if (context.queuedPackageNameIndex != -1)
				loadPackage(context.queuedPackageType, pkgSystem.strHeapStack + context.queuedPackageNameIndex);
			else
				loadPackage(context.queuedPackageType, ((void*)0) );
		}
		 
		 
		if (SACM_Status() && !context.isPaused && 
			compressTime(Snd_A1800_GetCurrentTime(),context.package->timePrecision) >= context.timeNextTimeframe)
				endOfTimeframe(context.idxTimeframe, (1==0) );
		else if (!context.isStopped && !SACM_Status()) {  
			 
			 
			context.isStopped = (1==1) ;
			flushLog();			
			if (GREEN_LED_WHEN_PLAYING) {
				setLED(LED_GREEN,(1==0) );
				setLED(LED_RED,(1==0) );				
			}
			if (context.idxActiveList == -1)		
				endOfTimeframe(context.idxTimeframe, (1==1) );
			else {
				 
				 
				list = &context.package->lists[context.idxActiveList];
				insertBlock = getEndInsert(list->actionStartEnd, list->idxFirstAction);
				if (insertBlock)
					insertSound(getFileFromBlock(insertBlock),insertBlock,(1==0) );
			}
		}
		keyResponse();
		if (++inactivityCheckCounter > 100) {
			checkInactivity(!context.isStopped && !context.isPaused);
			inactivityCheckCounter = 0;
		}
	}  
}

static void takeAction (Action *action, EnumAction actionCode) {
	unsigned int newTime, oldTime; 
	unsigned long longNewTime, longOldTime;
	int newIdxTimeframe, tempTime;
	CtnrFile *newFile;
	CtnrBlock *newBlock, *soundInsertBlock;
	int newIdxAction;
	EnumAction newActionCode;
	EnumBorderCrossing direction;
	signed long l;	
	int destination, aux, i, ret;
	int status;
	int   reposition = (1==0) ;
	int   isTooFar = (1==0) ;
	ListItem *list, *tempList;
	char filename[60];
	char *cursor;
	CtnrFile *replayFile;
		
	replayFile = ((void*)0) ;
	list = ((void*)0) ;
	oldTime = compressTime(Snd_A1800_GetCurrentTime(),context.package->timePrecision);
	newFile = 0;
	newTime = -1;
	newBlock = 0;
		
	if (actionCode == -1)
		actionCode = getActionCode(action);
	if (action) {
		aux = action->aux;
		destination = action->destination;
	}
		
	switch (actionCode) {
		case JUMP_LIST:
			stop();
			list = &context.package->lists[destination];
			context.idxActiveList = destination;
			switch (getListRotation(aux)) {
				case 0:
				    cursor = getCurrentList(list);
				    strcpy(filename,cursor);
					break;
				case 1:
				    strcpy(filename,getNextList(list,(1==1) ));
					break;
				case -1:
				    strcpy(filename,getPreviousList(list));
					break;
			}
			if (!filename[0]) { 
				 
				insertSound(&pkgSystem.files[EMPTY_LIST_FILE_IDX],((void*)0) ,(1==0) );
			} else {
				if (list->listType == LIST_OF_PACKAGES) {
					 
					if (0 == strncmp(filename,CUSTOM_PKG_PREFIX,strlen(CUSTOM_PKG_PREFIX))) {
						destination = replaceStack(filename+strlen(CUSTOM_PKG_PREFIX),&pkgSystem);
						context.queuedPackageType = 2 ;
					} else {
						destination = replaceStack(filename,&pkgSystem);
						context.queuedPackageType = 3 ;
					}
					context.queuedPackageNameIndex = destination;
				} else {
					 
					newFile = getListFile(filename);
					newTime = 0;
					reposition = (1==1) ;
				}
			}
			break;
		
		case JUMP_PACKAGE:
			if (aux == 4 ) {
				aux = 2 ;
				destination = replaceStack(getCurrentList(&pkgSystem.lists[destination]),&pkgSystem);
			}
			context.queuedPackageType = aux;
			context.queuedPackageNameIndex = destination;
			break;   

		case COPY:
			stop();
			tempList = &context.package->lists[destination];
			getListFilename((char *)filename,destination);
			cursor = getCurrentList(tempList);			
			d2dCopy((const char *)filename,(const char *)cursor);
			loadPackage(1 ,BOOT_PACKAGE);
			break;		
		case DELETE:
			stop();
			tempList = &context.package->lists[destination];
			getListFilename(filename,destination);
			cursor = getCurrentList(tempList);			
			ret = findDeleteStringFromFile(LIST_PATH,filename,cursor,(1==1) );
			tempList->currentFilePosition = -1;  
			if (ret != -1)
				ret = deletePackage(cursor);
			else
				logException(29,cursor,0);
			newBlock = &context.package->blocks[aux];
			newTime = newBlock->startTime;
			reposition = (1==1) ;
			break;
			
		case SPEED_UP:
			adjustSpeed(SPEED_INCREMENT,(1==1) );
			break;

		case SPEED_DOWN:
			adjustSpeed(-SPEED_INCREMENT,(1==1) );
			break;

		case SPEED_NORMAL:
			adjustSpeed(NORMAL_SPEED,(1==0) );
			break;

		case VOLUME_UP:
			adjustVolume(VOLUME_INCREMENT,(1==1) ,(1==0) );
			break;

		case VOLUME_DOWN:
			adjustVolume(-VOLUME_INCREMENT,(1==1) ,(1==0) );
			break;

		case VOLUME_NORMAL:
			adjustVolume(NORMAL_VOLUME,(1==0) ,(1==0) );
			break;

		case LED_RED_ON:
			setLED(LED_RED,(1==1) );
			break;	
			
		case LED_RED_OFF:
			setLED(LED_RED,(1==0) );
			break;	

		case LED_GREEN_ON:
			setLED(LED_GREEN,(1==1) );
			break;
			
		case LED_GREEN_OFF:	
			setLED(LED_GREEN,(1==0) );
			break;

		case LED_ALL_ON:
			setLED(LED_ALL,(1==1) );	
			break;	

		case LED_ALL_OFF:
			setLED(LED_ALL,(1==1) );
			break;
			
		case USB_DEVICE_ON:
			stop();
			setUSBDevice((1==1) );
			newBlock = &context.package->blocks[destination];
			newTime = newBlock->startTime;
			reposition = (1==1) ;
			break;	
					
		case USB_HOST_ON:
			stop();
			setUSBDevice((1==1) );
			newBlock = &context.package->blocks[destination];
			newTime = newBlock->startTime;
			reposition = (1==1) ;
			break;	
					
		case STOP:
			stop();
			enterOrExitAllBlocks(context.idxTimeframe,EXITING);
			break;

		case PLAY_PAUSE:
			status = SACM_Status();
			switch (status) {
				case 0:
					if (context.idxActiveList == -1) {
						enterOrExitAllBlocks(context.idxTimeframe,ENTERING);
						i = getStartingBlockIdxFromTimeline(context.idxTimeframe);
						if (i != -1) 
							processStartBlock(i);
						play(context.file,getCurrentTimeframeStart());
					} else
						play(context.file,0);
					break;
				case 0x0002 :
				case 0x0001 :
					context.isPaused = (1==1) ;
					SACM_Pause();
					break;
				case 0x0004 :
					context.isPaused = (1==0) ;
					SACM_Resume();	
					break;
				default:
					if (context.isPaused) {
						context.isPaused = (1==0) ;
						SACM_Resume();	
					} else {
						context.isPaused = (1==1) ;
						SACM_Pause();
					}		
					break;
			}
			break;
			
		case PAUSE:
			SACM_Pause();
			context.isPaused = (1==1) ;
			break;
			
		case RECORD_TITLE:  
		case RECORD_MSG:
			stop();
 







			do {
				strcpy(filename,USER_PATH);
				getPkgNumber(filename+strlen(USER_PATH),(1==1) );
				cursor = filename + strlen(filename);
				strcat(filename,AUDIO_FILE_EXT);
				ret = fileExists((unsigned long  )filename);  
			} while (ret);
			*cursor = 0;  
			strcpy(filename,filename+strlen(USER_PATH));  
			
			cursor = getCurrentList(&pkgSystem.lists[context.package->idxMasterList]);
			ret = createRecording(filename,aux,cursor);
			if (ret != -1) {
				destination = replaceStack(filename,context.package);
				context.queuedPackageNameIndex = destination;
				context.queuedPackageType = 3 ;
			} else
				logException(28,"recording failed",1 );  
			break;	

		case CALL_BLOCK:
			 
			stackPush(context.package,context.file,oldTime - getRewind(aux));  
			 
			
		case JUMP_BLOCK:
			newBlock = &context.package->blocks[destination];
			newTime = newBlock->startTime;
			reposition = (1==1) ;
			break;
	
		case RETURN:
			if (stackPop(&context.package,&newFile,&newTime))   
				reposition = (1==1) ;
			break;

		case FWD:
			newActionCode = getStartEndCodeFromTimeframe(context.idxTimeframe,FORWARD_JUMPING, &newTime, &newIdxAction);
			switch (newActionCode) {
				case NOP:
				case PAUSE:
					break;
				case STOP:
					 
					newTime -= compressTime(BLOCK_END_LEADER,context.package->timePrecision);
					break;
				case JUMP_BLOCK:
					 
					newBlock = &context.package->blocks[context.package->actions[newIdxAction].destination];
					newTime = newBlock->startTime;
					break;
				case RETURN:
					stackPop(&context.package,&newFile,&newTime);  
					break;
				default:
					 
					break;
			}
			reposition = (1==1) ;
			break;
			
		case BACK:
			 
			if ((oldTime - getCurrentTimeframeStart()) > compressTime(BLOCK_START_LEADER, context.package->timePrecision)) {
				 
				newTime = getCurrentTimeframeStart();  
			} else {
				newActionCode = getStartEndCodeFromTimeframe(context.idxTimeframe,BACKWARD_JUMPING, &newTime, &newIdxAction);
				switch (newActionCode) {
					case NOP:
					case PAUSE:
					case STOP:
						break;
					case JUMP_BLOCK:
						 
						newBlock = &context.package->blocks[context.package->actions[newIdxAction].destination];
						newTime = newBlock->startTime;
						break;  
					case RETURN:
						stackPop(&context.package,&newFile,&newTime);  
						break;
					default:
						 
						break;						
				}
			}
			reposition = (1==1) ;
			break;

		case JUMP_TIME:
			 
			 
			 
			 
			if (context.returnPackage)
				context.package = context.returnPackage;
			if (context.package != &pkgSystem) {
				longOldTime = Snd_A1800_GetCurrentTime();
				l = (signed long)extractSignedTime(destination,context.package->timePrecision);  
				longNewTime = longOldTime + l;
				if (l >= 0) 
					direction = FORWARD_JUMPING;
				else {
					direction = BACKWARD_JUMPING;
					if (abs(l) > longOldTime)
						longNewTime = 0;
				}
				newTime = compressTime(longNewTime,context.package->timePrecision);
	
				 
				newIdxTimeframe = context.idxTimeframe;
				do {
					newActionCode = getStartEndCodeFromTimeframe(newIdxTimeframe,direction, &tempTime, &newIdxAction);
					if (direction == FORWARD_JUMPING) 
						isTooFar = blockTimeline[++newIdxTimeframe].time > newTime;
					else
						isTooFar = blockTimeline[--newIdxTimeframe].time < newTime;
				} while (!isTooFar && newActionCode == NOP);
				switch (newActionCode) {
					case JUMP_BLOCK:
						newBlock = &context.package->blocks[context.package->actions[newIdxAction].destination];
						newTime = newBlock->startTime;
						newFile = getFileFromBlock(newBlock);
						break;  
					case RETURN:
						stackPop(&context.package,&newFile,&newTime);  
						break;
					default:
						 
						break;
				}
	
				reposition = (1==1) ;
			}  
			break;
		case NOP:
			 
			break;
		default:
			 
			logException(1,0,2 );
			break;
	}
		
	if (reposition) {
		 
		if (newBlock && !newFile) 
			newFile = getFileFromBlock(newBlock);
		if (newFile && newFile != context.file) {
			enterOrExitAllBlocks(context.idxTimeframe,EXITING);
			context.file = newFile;
			if (!list)
				buildBlockTimelines(newFile);
			context.idxTimeframe = -1;  
			context.timeNextTimeframe = -1;  
		}
		if (!list) {
			context.idxActiveList = -1;
			newIdxTimeframe = getIdxTimeline(newTime);
			if (newIdxTimeframe != context.idxTimeframe) 
				processTimelineJump(context.idxTimeframe, newIdxTimeframe);   
		}
	}
	
	 
	if (action && hasSoundInsert(action)) {
		if (reposition) 
			stop();   
		if (actionCode == JUMP_TIME) {
			if (context.returnPackage)
				context.package = &pkgSystem;
			soundInsertBlock = &context.package->blocks[getSoundInsertIdxFromAux(aux)];
			insertSound(getFileFromBlock(soundInsertBlock),soundInsertBlock,(1==1) );
		}
		else {
			soundInsertBlock = &context.package->blocks[getSoundInsertIdxFromAux(aux)];
			insertSound(getFileFromBlock(soundInsertBlock),soundInsertBlock,(1==0) );
		}
	}
	 
	if (reposition && newBlock && newTime == newBlock->startTime)
		processStartBlock(newBlock - context.package->blocks);
	if (list) {
		soundInsertBlock = getStartInsert(list->actionStartEnd, list->idxFirstAction);
		if (soundInsertBlock)
			insertSound(getFileFromBlock(soundInsertBlock),soundInsertBlock,(1==0) );
	}		
	if (actionCode == JUMP_TIME && context.returnPackage) {   
		context.package = context.returnPackage;
		context.returnPackage = ((void*)0) ;
	}
	if (reposition)
		play(context.file,newTime);  
	 
}

static void loadPackage(int pkgType, const char * pkgName) {
	CtnrPackage *pkg;
	int i;
	CtnrBlock *block;
	Action *action;
	char filePath[60];
	long timeNow;
		
	stop();   

	 
	timeNow = getRTCinSeconds();
	if (context.packageStartTime)
		markEndPlay(timeNow);
	if (pkgType != 1 )
		markStartPlay(timeNow,pkgName);

	context.lastFile = ((void*)0) ;
	context.queuedPackageType = 0 ;  
	context.returnPackage = ((void*)0) ;		
	if ((pkgType == 1 ) && (pkgName == ((void*)0) )) {
		context.package = &pkgSystem;
		pkg = context.package;
	} else if (pkgType == 3 ) {
		context.package = &pkgDefault;
		pkg = context.package;
		 
		strcpy(pkg->strHeapStack+pkg->files[0].idxFilename,pkgName);
	}
	else {
		 
		switch (pkgType) {
			case 1 :
				context.package = &pkgSystem;
				strcpy(filePath,SYSTEM_PATH);
				break;
			case 2 :
				context.package = &pkgUser;
				strcpy(filePath,USER_PATH);
				strcat(filePath,pkgName);
				strcat(filePath,"\\");
				break;
			default:
				logException(5,0,2 );
				break;
		}
		strcat(filePath,pkgName);
		strcat(filePath,".txt");
		pkg = context.package;
		memset(pkg,0,sizeof(CtnrPackage));
		pkg->pkg_type = pkgType;
		parseControlFile(filePath, pkg);
	}
	 
	 
	for (i=0; i < 5 ; i++) 
		pkg->lists[i].currentFilePosition = -1;
	pkg->recInProgress = (1==0) ;
	if (pkg->countPackageActions) 
		action = pkg->actions;
	while (action && !isEventInAction(action,START,context.isPaused))
		action = getNextAction(action);
	if (action) {
		 
		block = pkg->blocks + action->destination;
    	context.file = getFileFromBlock(block);
	} else {
		context.file = &pkg->files[0];
		block = 0;
	}
    buildBlockTimelines(context.file);
	context.idxTimeframe = getIdxTimeline(block?block->startTime:0);
	context.timeNextTimeframe = blockTimeline[context.idxTimeframe+1].time;
	context.idxPausedOnBorder = -1;
	context.idxActiveList = -1;
	play(context.file,blockTimeline[context.idxTimeframe].time);
}

/* End of uDOC32 problem report. */
