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

#define FIRMWARE_PATH	"a:\\System\\Firmware\\"
//TODO: change this to use any *.bin and keep filename for easy version identification
#define UPDATE_FN    	"a:\\System.bin"
#define ATTEMPTED_FN	FIRMWARE_PATH"Attempted.bin"
#define PREV_FN			FIRMWARE_PATH"Previous.bin"
#define CURRENT_FN 		FIRMWARE_PATH"Current.bin"
extern void check_new_sd_flash(void);

#endif
