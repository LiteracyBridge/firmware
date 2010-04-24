// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__SD_REPROG_h__
#define	__SD_REPROG_h__

#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "./Reprog/USB_Flash_reprog.h"
#include "./Application\TalkingBook\Include\talkingbook.h"
#include "./Application\TalkingBook\Include\files.h"

#define FIRMWARE_PATH	"a:/Firmware/"
#define UPDATE_FP    	"a:/"
#define SERIAL_FN  		"*.tsn"
#define ERASE_SN_CODE	"-erase-"
#define UPDATE_FN    	"*.img"
#define ATTEMPTED_EXT	".att"
#define PREV_EXT		".old"
extern void check_new_sd_flash(void);
extern void startUpdate(char *);

#endif
