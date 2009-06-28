#include ".\System\include\system\GPL162002.h"
#include ".\Component\Include\FS\typedef.h"  // for BOOL
#include ".\Application\TalkingBook\Include\sys_counters.h"

#define	C_SEEK_SPEEDUP_SUPPORT

#ifdef C_SEEK_SPEEDUP_SUPPORT

#ifdef USBRP
extern int				gSeekSpeedupBufferFlag;
extern unsigned int	gSeekSpeedupBuffer[512];
#else
int				gSeekSpeedupBufferFlag;
unsigned int	gSeekSpeedupBuffer[512];
#endif //USBRP

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
	return;
}

void FS_OS_UNLOCK(void)
{
	return;
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
	 dd->year = 2000 + systemCounts.powerUpNumber;
	 dd->month = 1;
	 dd->monthday = 1;
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
	 dt->hour = (int)*P_Hour;
	 dt->minute = (int)*P_Minute;
	 dt->second = (int)*P_Second;
	 dt->hundredth = 0;
}

