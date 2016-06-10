
#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"

void BodyInit()
{
	SysInitWaitMode();
	// ucBSInit();
		
//	Snd_VarInit();
//	SACM_Initial();	
	SD_Initial();
}
