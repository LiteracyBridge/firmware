// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include ".\Component\Include\FS\vfs.h"
#include "Include/talkingbook.h"
#include "Include/util.h"
#include "Include/device.h"
#include "Include/files.h"
#include "Include/audio.h"
#include "Include/containers.h"
#include "Include/d2d_copy.h"

extern int setUSBHost(BOOL enter);
static int testCopy(char *, char *, int);

int d2dCopy(const char * filenameList, const char * packageName) {
	int ret, retCopy;
	char to[PATH_LENGTH], strLog[PATH_LENGTH],filename[PATH_LENGTH], path[PATH_LENGTH];
	char *cursor, *prefixCursor;
	int maxTrials = 5;
	struct f_info file_info;
	long timeNow;

	retCopy = 0;
	setLED(LED_GREEN,FALSE);
	timeNow = getRTCinSeconds();
	markEndPlay(timeNow);
	setUSBHost(TRUE);

	// code below is borrowed from the delete fct
	strcpy(strLog,"\x0d\x0a");
	longToDecimalString(timeNow,strLog+2,5);
	strcat((char *)strLog,(const char *)": COPY ");
	
	strcpy(to,INBOX_PATH);
	to[0] = 'b'; //change a:\\ drive to b:\\ drive
	prefixCursor = NULL;
	if (0 == strncmp(packageName,CUSTOM_PKG_PREFIX,strlen(CUSTOM_PKG_PREFIX)))
		prefixCursor = CUSTOM_PKG_PREFIX;
	if (prefixCursor) {
		cursor = (char *)packageName + strlen(prefixCursor);
		strcpy(path,to);
		strcat(path,cursor);
		ret = mkdir((LPSTR)path);
		strcpy(path,USER_PATH);
		strcat(path,cursor);
		strcat(path,"\\");
		strcat(strLog,cursor);
		logString(strLog,ASAP);
		strcpy(filename,path);
		strcat(filename,"*.*");
		ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
		while (ret >= 0) {
			strcpy(filename,path);
			strcat(filename,file_info.f_name);
			strcpy(to,INBOX_PATH);
			to[0] = 'b'; //change a:\\ drive to b:\\ drive
			strcat(to,cursor); // to get directory name
			strcat(to,"\\");
			strcat(to,file_info.f_name);
			retCopy = testCopy(filename,to,maxTrials); //was unlink((LPSTR)filename);
			if (retCopy == -1) {
				strcpy(strLog,(const char *)"Copy Failed! ");
				strcat(strLog,filename);
			} else
				strcpy(strLog,filename);
			logString(strLog,BUFFER);
			ret = _findnext(&file_info);
		}
	}
	else {
		cursor = (char *)packageName;
		if (LBstrncat((char *)strLog,cursor,PATH_LENGTH) == PATH_LENGTH-1)
			strLog[PATH_LENGTH-2] = '~';
		logString(strLog,BUFFER);
		strcpy(filename,USER_PATH);
		strcat(filename,cursor);
		strcat(filename,AUDIO_FILE_EXT);
		strcat(to,cursor);
		strcat(to,AUDIO_FILE_EXT);
		retCopy = testCopy(filename,to,maxTrials); //was unlink((LPSTR)filename);
		if (retCopy == -1) {
			strcpy(strLog,(const char *)"Copy Failed! ");
			strcat(strLog,filename);
		} else
			strcpy(strLog,filename);
		logString(strLog,BUFFER);
	}
	strcpy(filename,LIST_PATH); 
	strcat(filename,filenameList);
	strcat(filename,"*");
	strcat(filename,AUDIO_FILE_EXT);
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	while (ret >= 0 && (retCopy == 0)) {
		strcpy(filename,LIST_PATH);
		strcat(filename,file_info.f_name);
		strcpy(to,INBOX_PATH);
		to[0] = 'b'; //change a:\\ drive to b:\\ drive
		strcat(to,LIST_PATH+3); // +3 removes the "a:\" but keeps "\lists\", for example
		strcat(to,file_info.f_name);
		retCopy = testCopy(filename,to,maxTrials); //was unlink((LPSTR)filename);
		if (retCopy == -1) {
			strcpy(strLog,(const char *)"Copy Failed! ");
			strcat(strLog,file_info.f_name);
		} else
			strcpy(strLog,file_info.f_name);
		logString(strLog,BUFFER);
		ret = _findnext(&file_info);
	}
	
	setUSBHost(FALSE);
	return retCopy;
}

static int testCopy(char * from, char * to, int maxTrials) {
	int ret, trials;
	
	setLED(LED_ALL,FALSE);
	for (trials = 1; trials <= maxTrials; trials++) {
		setLED(LED_RED,TRUE);		
		
		ret = unlink((LPSTR)to);
		ret = _copy((LPSTR)from,(LPSTR)to);
		wait (500);
		setLED(LED_RED,FALSE);
		if (ret != -1)
			break;
		wait (1000);
	}
	if (trials <= maxTrials)
		setLED(LED_GREEN,TRUE);
	return ret;
}