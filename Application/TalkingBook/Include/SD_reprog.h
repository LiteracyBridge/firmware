// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__SD_REPROG_h__
#define	__SD_REPROG_h__

#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./Reprog/USB_Flash_reprog.h"
#include "./Application/TalkingBook/Include/talkingbook.h"
#include "./Application/TalkingBook/Include/files.h"

#define UPDATE_FP    	"a:/"
#define SERIAL_EXT		".srn"
#define SERIAL_FN  		"*" SERIAL_EXT
#define ERASE_SN_CODE	"-erase-"
#define UPDATE_FN    	"*.img"
#define ATTEMPTED_EXT	".att"
#define PREV_EXT		".old"
extern int check_new_sd_flash(char *);
extern void startUpdate(char *);
extern void updateSN(void);

#endif
