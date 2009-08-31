#ifndef	__SD_REPROG_h__
#define	__SD_REPROG_h__

#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./Reprog/USB_Flash_reprog.h"
#include "./Application\TalkingBook\Include\talkingbook.h"
#include "./Application\TalkingBook\Include\files.h"

#define SYSTEM_FN	"System.bin"
#define UPDATE_FN    "Update\\"SYSTEM_FN 
#define ATTEMPTED_FN "Update\\Attempted.bin"
#define PREV_FN      "Archive\\Previous.bin"
#define CURRENT_FN 	 "Archive\\Current.bin"

#endif
