// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__APP_EXCEPTION_h__
#define	__APP_EXCEPTION_h__
#include "./Application/TalkingBook/Include/talkingbook.h"

#define RESET					1
#define USB_MODE				2
#define SHUT_DOWN				3
#define LOG_ONLY				0

#define ERROR_LOG_FILE			DEFAULT_SYSTEM_PATH "log.txt"

void logException(unsigned int, const char *, int);

#endif
