#ifndef	__POSIX_INTERFACE_h__
#define	__POSIX_INTERFACE_h__

#define LFN_API
#define WITHFAT32

#include "Typedef.h"

/**************************************************************************/
/*                              M A C R O S                               */
/***************************************************************************/

#define FAT16_Type	        		0x01
#define FAT32_Type	        		0x02
#define FORCE_FAT32_Type	        0x12
#define FORCE_FAT16_Type	        0x11

/*-----------------  seek flags  ------------------*/
#define SEEK_SET     0 /* offset from beginning of the file*/
#define SEEK_CUR     1 /* offset from current location     */
#define SEEK_END     2 /* offset from eof  */

/***************** open flags (the 2nd parameter)**********************/   //wanghuidi
#define O_RDONLY        0x0000
#define O_WRONLY        0x0001
#define O_RDWR          0x0002
#define O_ACCMODE       0x0003

#define O_TRUNC         0x0200 /*    both */
#define O_CREAT         0x0400
#define O_EXCL		    0x4000	/* not fcntl */

/*-----------------------------------------------------*/
/* File attribute constants for _findfirst() */

#define _A_NORMAL       0x00    /* Normal file - No read/write restrictions */
#define _A_RDONLY       0x01    /* Read only file */
#define _A_HIDDEN       0x02    /* Hidden file */
#define _A_SYSTEM       0x04    /* System file */
#define _A_SUBDIR       0x10    /* Subdirectory */
#define _A_ARCH         0x20    /* Archive file */

/*-----------------------------------------------------*/

/* FAT file system attribute bits                                   */
#define D_NORMAL        0       /* normal                       */
#define D_RDONLY        0x01    /* read-only file               */
#define D_HIDDEN        0x02    /* hidden                       */
#define D_SYSTEM        0x04    /* system                       */
#define D_VOLID         0x08    /* volume id                    */
#define D_DIR           0x10    /* subdir                       */
#define D_ARCHIVE       0x20    /* archive bit                  */
    							/* /// Added D_DEVICE bit.  - Ron Cemer */

#define D_FILE			(0x40)	
#define D_ALL (D_FILE | D_RDONLY | D_HIDDEN | D_SYSTEM | D_DIR | D_ARCHIVE)

#define UNI_GBK			0
#define UNI_BIG5		1
#define UNI_SJIS		2

#define UNI_ENGLISH		0x8003
#define UNI_ARABIC		0x8004
#define UNI_CP1252		0x8005

#define UNI_UNICODE		0x8100
//#define UNI_INVALID_TAB	0xffff

/**************************************************************************/
/*                              DATA TYPES ABOUT FILE                     */
/***************************************************************************/
//----------------------------------
struct stat_t 
{	
	UINT16	st_mode;
	INT32	st_size;
	UINT32	st_mtime;
};

//----------------------------------
struct _diskfree_t {
	UINT32 total_clusters;
	UINT32 avail_clusters;
	UINT32 sectors_per_cluster;
	UINT32 bytes_per_sector;
};

struct deviceinfo 	 {
	INT8 device_name[16];		// device name
	INT8 device_enable;			// device enable status
	INT8 device_typeFAT;		// device FAT type
	UINT32 device_availspace;	// device available space
	UINT32 device_capacity;		// device capacity
};

// data structure for _setftime()
struct timesbuf 	 {
	UINT16 modtime;
	UINT16 moddate;
	UINT16 accdate;
};

// data structure for _getfstat(), 555FS
struct statbuf 	 {
	UINT32 st_size;
	UINT8 st_mode;
	UINT16 crtime;
	UINT16 crdate;
	UINT16 modtime;
	UINT16 moddate;
	UINT16 accdate;
};

struct f_info         
{
	UINT8 f_attrib;
	UINT16  f_time;
	UINT16  f_date;
	UINT32 f_size;
	UINT16  entry;			//2006.7.12
#ifdef LFN_API
	 INT8 f_name[256]; 	 //change to 256 form 255
#else
	 INT8 f_name[13];
#endif
};

typedef struct        
{
	INT8    f_dsk;
	UINT32  f_entry;
	UINT16  f_offset;
}f_pos, *f_ppos;

struct sfn_info
{
    UINT8   f_attrib;
	UINT16  f_time;
	UINT16  f_date;
	UINT32  f_size;
    INT8    f_name[9];
    INT8    f_extname[4];
    f_pos	f_pos;
};

struct nls_table {
	INT8			*charset;						//本地编码名称
	UINT16			Status;							//unicode加载状况
	INT16			(*init_nls)(void);				//初始化unicode转换程序
	INT16			(*exit_nls)(void);				//退出unicode转换程序
	UINT16			(*uni2char)(UINT16 uni);		//unicode转换为本地编码
	UINT16			(*char2uni)(UINT8 **rawstring);	//本地编码转换为unicode
};

//struct STFileNodeInfo
//{
//	INT8		flag;
//	INT8		disk;
//	INT8*		extname;
//	UINT16*		Buffer;
//	UINT16		BufferSize;
//};
struct STFileNodeInfo
{
	INT8		flag;					//此struct是否有效
	INT8		disk;					//disk
	INT8		extname[4];				//待统计的文件的扩展名
	UINT16*		FileBuffer;				//文件节点buffer
	UINT16		FileBufferSize;			//文件节点buffer大小，每20 word为一个节点，所以需是20的倍数
	UINT16*		FolderBuffer;			//目录节点buffer
	UINT16		FolderBufferSize;		//目录节点buffer大小，如果大小为0，表示不统计目录信息
};

/***************************************************************************/
/*        F U N C T I O N    D E C L A R A T I O N S	     			   */
/***************************************************************************/
//查找disk的第一个文件
f_ppos getfirstfile(INT16 dsk, INT8* extname, struct sfn_info* f_info, INT16 attr);
//查找disk的文件
f_ppos getnextfile(INT16 dsk, INT8* extname, struct sfn_info* f_info, INT16 attr);
//open
INT16 sfn_open(f_ppos ppos);
//get file status
INT16 sfn_stat(INT16 fd, struct sfn_info *sfn_info);
//根据文件名节点返回长文件名信息f_info
INT16 GetFileInfo(f_ppos ppos, struct f_info *f_info);
//根据目录名节点返回长文件名信息f_info
INT16 GetFolderInfo(f_ppos ppos, struct f_info *f_info);
//根据文件名节点删除该文件
INT16 sfn_unlink(f_ppos ppos);

//统计某种类型的文件的数量
INT16 StatFileNumByExtName(INT16 dsk, LPSTR extname, UINT32 *filenum);
//统计磁盘中指定扩展名的文件数量和包含此种文件的目录数量
INT16 GetFileNumEx(struct STFileNodeInfo *stFNodeInfo, UINT32 *nFolderNum, UINT32 *nFileNum);
//根据index获取文件名节点信息
//注意执行此函数前请确保执行了函数GetFileNumEx
//0 <= nIndex < nMaxFileNum
f_ppos GetFileNodeInfo(struct STFileNodeInfo *stFNodeInfo, UINT32 nIndex, struct sfn_info* f_info);
//根据目录的index获取目录中的特定扩展名的文件的数目
//注意执行此函数前请确保执行了函数GetFileNumEx
//成功返回0，失败返回-1
INT16 GetFileNumOfFolder(struct STFileNodeInfo *stFNodeInfo, UINT32 nFolderIndex, UINT16* nFile);
//根据folder的index得到该folder的第一个文件的index
//注意执行此函数前请确保执行了函数GetFileNumEx
//成功返回0，失败返回-1
INT16 FolderIndexToFileIndex(struct STFileNodeInfo *stFNodeInfo, UINT32 nFolderIndex, UINT32* nFileIndex);
//根据file的index得到该file所在的folder的index
//注意执行此函数前请确保执行了函数GetFileNumEx
//成功返回0，失败返回-1
INT16 FileIndexToFolderIndex(struct STFileNodeInfo *stFNodeInfo, UINT32 nFileIndex, UINT32* nFolderIndex);
//根据目录index获取该目录节点的信息
//注意执行此函数前请确保执行了函数GetFileNumEx
f_ppos GetFolderNodeInfo(struct STFileNodeInfo *stFNodeInfo, UINT32 nFolderIndex, struct sfn_info* f_info);

//convert the dos_data to year, month, day
void dosdate_decode(UINT16 dos_date, UINT16 *pyear, UINT8 *pmonth, UINT8 *pday);
//convert the dos_time to hour, minute, second
void dostime_decode(UWORD dos_time, UINT8 *phour, UINT8 *pminute, UINT8 *psecond);

//convert *tp to a string like "hh:mm:ss"
INT8 *time_decode(UINT16 *tp, INT8 *timec);
//convert *dp to a string like "yyyy-mm-dd" 
INT8 *date_decode(UINT16 *dp, INT8 *datec);

//close all the opened files except the registed file
void fs_safexit(void);
//regist opened file so when you call function fs_safexit() this file will not close
void fs_registerfd(INT16 fd);

//close all the opened files of the disk
void disk_safe_exit(INT16 dsk);

INT16 close(INT16 fd);

INT16 open(LPSTR path , INT16 open_flag);

INT32 read(INT16 fd , LPDATA buf , UINT32 size);
INT32 write(INT16 fd , LPDATA buf ,  UINT32 size);

INT32 lseek(INT16 fd ,INT32 offset ,INT16 fromwhere);
INT32 tell(INT16 fd); 

INT16 unlink(LPSTR filename);
INT16 rename(LPSTR oldname , LPSTR newname);

INT16 readB(INT16 fd , LPDATA buf , UINT16 size);
INT16 writeB(INT16 fd , LPDATA buf ,  UINT16 size);

INT16 mkdir(LPSTR pathname);

INT16 rmdir(LPSTR pathname);
INT16 chdir(LPSTR path);
LPSTR getcwd(LPSTR buffer , INT16 maxlen );

INT16 fstat(INT16 handle , struct stat_t *statbuf);
INT16 stat(LPSTR path, struct stat_t *statbuf);

INT16 _findfirst(LPSTR name , struct f_info *f_info ,  UINT16 attr);
INT16 _findnext(struct f_info *f_info);

INT16 _getdiskfree(INT16 dsk, struct _diskfree_t *st_free);

#define _changedisk     fs_changedisk 
INT16 _changedisk( UINT8 disk);  
/****************************************************************************/
INT32 vfsFreeSpace(INT16 driver);
INT16 _copy(LPSTR srcfile , LPSTR destfile);

void fs_init(void);
INT16 fs_uninit();

UINT16 tellcurrentfiledir(void);
UINT16 telldir(void);
void seekdir( UINT16 pos);     //the parameter "pos" must be the return value of "telldir"
void rewinddir(void);

//set file attribute
INT16 _setfattr (LPSTR filename ,  UINT16 attr);
//set dir attribute
INT16 _setdirattr (LPSTR dirname ,  UINT16 attr); 
//get dir attribute
INT16 _getdirattr (LPSTR dirname ,  UINT16 *attr); 

INT16 _devicemount(INT16 disked);
INT16 _deviceunmount(INT16 disked);

INT16 _getfserrcode ();
void _clsfserrcode ();

INT16 _format ( UINT8 drv ,  UINT8 fstype);
INT16 _deleteall (LPSTR filename);

//获取当前seek的位置对应的cluster ID
INT32 _GetCluster(INT16 fd);
//cluster ID to sector address
INT32 Clus2Phy( UINT16 dsk ,  UINT32 cl_no);

/****************************************************************************/
//delete part of file,from offset,delete length byte
//but the file system will change the offset and length to cluster number 
INT16 DeletePartFile(INT16 fd, UINT32 offset, UINT32 length);
//insert the src file to tag file
INT16 InserPartFile(INT16 tagfd, INT16 srcfd, UINT32 tagoff, UINT32 srclen);
//split tag file into two file, one is remain in tag file, one is in src file
INT16 SplitFile(INT16 tagfd, INT16 srcfd, UINT32 splitpoint);

//一个cluster有多少个sector
UINT16 GetSectorsPerCluster(UINT16 dsk);

//select unicode page,the code page is defined above
UINT16 ChangeCodePage(UINT16 wCodePage);
UINT16 GetCodePage(void);
INT16 ChangeUnitab(struct nls_table *st_nls_table);

//is it fat16 or fat32?
INT16 checkfattype (UINT8 disk);

//updata dir information bu not close the dir
INT16 UpdataDir(INT16 fd);

//if the file is destroy for some reason,this function will repair the file
//note it can't deal with some complicated condition
INT16 FileRepair(INT16 fd);

//支持小容量的磁盘的格式化，固定格式化为FAT16，不带MBR，需要指定磁盘的最大sector数和实际sector数
//因为我们的文件系统不支持FAT12，而当磁盘容量小于16MB时，PC会把磁盘认做FAT12，所以做了这个函数，可以
//把小于16MB的磁盘格式化为大于16Mb（可指定大小），并可以指定磁盘实际的空间大小
INT16 sformat(UINT8 drv, UINT32 totalsectors, UINT32 realsectors);

//得到打开的文件所处的磁盘，返回值0,1,2...
INT16 GetDiskOfFile(INT16 fd);

//创建指定大小的文件，size is byte size
//返回该文件的句柄
//创建失败，返回-1
INT16 CreatFileBySize(LPSTR path ,UINT32 size);

#endif //# end __FS_INTERFACE_h__
