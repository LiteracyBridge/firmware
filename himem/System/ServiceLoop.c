
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"

unsigned int CheckSystemStatus(void);

unsigned int ServiceLoop(void)
{
	unsigned int Status;

	//check system status
	while( 0==(Status=CheckSystemStatus()) )
	{
		//check halt mode
		if(SYS_ON2OFF==SysGetOnOffKey())
		{
			SysSetOnOffKey(SYS_NULL);
			if(0==SysGetHaltModeFlag())
				SysIntoHaltMode();
		}

		//check wait mode
		if(0==SysGetWaitModeFlag())
			SysIntoWaitMode();
	}

	return(Status);
}

//========================================================================================
//========================================================================================
//========================================================================================
// 	Please add your status checking here.
//========================================================================================
//========================================================================================
//========================================================================================
#define CheckStatus_RTC GetRTC_MSGType
unsigned int CheckSystemStatus(void)
{
	unsigned int Status=0;

	//如果有状态需要返回给用户处理,则状态检查函数返回非0的消息类型,否则返回0.
//	if(Status = CheckStatus_Snd())		//播放声音的时候需要有用户来调用SysDisableWaitMode,保证一直在高频.
	{
		return(Status);
	}

	return(Status);
}
