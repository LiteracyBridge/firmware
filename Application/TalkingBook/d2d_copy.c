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
#include "Include/filestats.h"

extern int setUSBHost(BOOL enter);
extern int testCopy2(char * from, char * to);
static int fileCopy(char *, char *, int);

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
	strcat(to,NEW_PKG_SUBDIR);
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
			strcat(to,NEW_PKG_SUBDIR);
			strcat(to,cursor); // to get directory name
			strcat(to,"\\");
			strcat(to,file_info.f_name);
			retCopy = testCopy2(filename,to); //was unlink((LPSTR)filename);
			if (retCopy == -1) {
				strcpy(strLog,(const char *)"Copy Failed! ");
				strcat(strLog,filename);
			} else {
				strcpy(strLog,filename);
				recordStats(filename, 0L, STAT_COPIED, PKG_SYS+1);
			}
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
		retCopy = testCopy2(filename,to); //was unlink((LPSTR)filename);
		if (retCopy == -1) {
			strcpy(strLog,(const char *)"Copy Failed! ");
			strcat(strLog,filename);
		} else {
			strcpy(strLog,filename);
			recordStats(filename, 0L, STAT_COPIED, PKG_SYS+1);
		}
		logString(strLog,BUFFER);
	}
	strcpy(filename,LIST_PATH); 
	strcat(filename,filenameList);
//	strcat(filename,"*");  //TODO: for now only copying the current language for the copied category
	strcat(filename,"-");
	strcat(filename,&pkgSystem.strHeapStack[pkgSystem.idxLanguageCode]);
	strcat(filename,AUDIO_FILE_EXT);
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	while (ret >= 0 && (retCopy == 0)) {
		strcpy(filename,LIST_PATH);
		strcat(filename,file_info.f_name);
		strcpy(to,INBOX_PATH);
		strcat(to,SYS_UPDATE_SUBDIR);
		to[0] = 'b'; //change a:\\ drive to b:\\ drive
		strcat(to,LIST_PATH+3); // +3 removes the "a:\" but keeps "\lists\", for example
		mkdir((LPSTR)to);
		strcat(to,file_info.f_name);
		retCopy = testCopy2(filename,to); //was unlink((LPSTR)filename);
		if (retCopy == -1) {
			strcpy(strLog,(const char *)"Copy Failed! ");
			strcat(strLog,filename);
			strcat(strLog,(const char *)" ");
			strcat(strLog,to);
		} else {
			strcpy(strLog,file_info.f_name);
			recordStats(file_info.f_name, 0L, STAT_COPIED, PKG_SYS+1);
		}
		logString(strLog,BUFFER);
		ret = _findnext(&file_info);
	}
//
// echange stats - should be a new function but I wanted to use already allocated to, filename, ...
// assumes a device keeps its stas in a directory a:\stats   (STAT_DIR)
//         a device keeps stats from other devics in a:\ostat  (OSTAT_DIR)
// the code a to b and stats to ostats & vice versa
//
//  copy a:\stats\*.* to b:\ostat 

	mkdir((LPSTR)OSTAT_DIR);
	strcpy(filename,STAT_DIR); 
	strcat(filename,"*.*");
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	retCopy = 0;
	while (ret >= 0 && (retCopy == 0)) {
		strcpy(filename,STAT_DIR); 
		strcat(filename,file_info.f_name);
		strcpy(to,OSTAT_DIR);
		to[0] = 'b'; //change a:\\ drive to b:\\ drive
		strcat(to,file_info.f_name);
		retCopy = testCopy2(filename,to); //was unlink((LPSTR)filename);
		if (retCopy == -1) {
			strcpy(strLog,(const char *)"Stats a to b Copy Failed! ");
			strcat(strLog,filename);
		} else
			strcpy(strLog,filename);
			
		logString(strLog,BUFFER);
		ret = _findnext(&file_info);
	}

	
//  copy b:\stats\*.* to a:\ostat 
	strcpy(filename,STAT_DIR);
	filename[0] = 'b'; 
	strcat(filename,"*.*");
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	while (ret >= 0 && (retCopy == 0)) {
		strcpy(filename,STAT_DIR); 
		strcat(filename,file_info.f_name);
		filename[0] = 'b'; 
		strcpy(to,OSTAT_DIR);
		strcat(to,file_info.f_name);
		retCopy = testCopy2(filename,to); //was unlink((LPSTR)filename);
		if (retCopy == -1) {
			strcpy(strLog,(const char *)"Stats b to a Copy Failed! ");
			strcat(strLog,filename);
		} else
			strcpy(strLog,filename);
			
		logString(strLog,BUFFER);
		ret = _findnext(&file_info);
	}

//	
// Logic for which device has the most current stats for devices they both have connected to goes here
//

	setUSBHost(FALSE);
	return retCopy;
}

static int fileCopy(char * from, char * to, int maxTrials) {
	int ret, trials;
/*
	long filesize;
	int secToCopy;
	struct f_info f_info;
	// get filesize to estimate expected copy time
	ret =_findfirst((LPSTR)from, &f_info, D_FILE);
	filesize = f_info.f_size;
*/	
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
