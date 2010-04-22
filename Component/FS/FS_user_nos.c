#define	C_SEEK_SPEEDUP_SUPPORT

#ifdef C_SEEK_SPEEDUP_SUPPORT
int				gSeekSpeedupBufferFlag;
unsigned int	gSeekSpeedupBuffer[512];
#endif


int  FS_OS_Init (void) 
{
    return (0);
}

int  FS_OS_Exit (void) 
{
    return (0);
}

void FS_OS_LOCK(void)
{
	return 0;
}

void FS_OS_UNLOCK(void)
{
	return 0;
}

/****************************************************************/
/*																*/
/*			       seek speedup									*/
/*																*/
/*		  seek speedup memery management function               */
/*																*/
/****************************************************************/
unsigned long FS_SeekSpeedup_Malloc(unsigned int len)
{
#ifdef C_SEEK_SPEEDUP_SUPPORT
	if(gSeekSpeedupBufferFlag == 0x5566)
		return 0;
	
	gSeekSpeedupBufferFlag = 0x5566;	
	return (unsigned long)(unsigned int)gSeekSpeedupBuffer;
#else
	return 0;
#endif
}

void FS_SeekSpeedup_Free(unsigned long addr)
{
#ifdef C_SEEK_SPEEDUP_SUPPORT
	gSeekSpeedupBufferFlag = 0;
#endif
}

/****************************************************************/
/*																*/
/*			       getdate.c									*/
/*																*/
/*		  user write this file to assign the file system date   */
/*																*/
/****************************************************************/
struct s_dosdate
{
   unsigned short year;
   unsigned char monthday , month;
};

void FS_OS_GetDate(struct s_dosdate *dd)
{
	 dd->year = 2004;
	 dd->month = 8;
	 dd->monthday = 23;
}

/****************************************************************/
/*																*/
/*			       gettime.c									*/
/*																*/
/*		  user write this file to assign the file system time   */
/*																*/
/****************************************************************/
struct s_dostime 	 {
   unsigned char minute , hour , hundredth , second;
};

void FS_OS_GetTime(struct s_dostime *dt)
{
	 dt->hour = 16;
	 dt->minute = 54;
	 dt->second = 37;
	 dt->hundredth = 0;
}

