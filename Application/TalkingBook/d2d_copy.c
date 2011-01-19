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
static int copyApplicationOrMessage(char * packageName, char *newPkgPath);
static int copyApplication(char * packageName, char *newPkgPath);
static int copyMessage(char * packageName, char *newPkgPath);
static void copyListAudio(const char * listName);
static void exchangeStats(void);

int d2dCopy(const char * packageName,const char * filenameList) {
	int retCopy;
	char newPkgPath[PATH_LENGTH], strLog[PATH_LENGTH];
//	int maxTrials = 5;
	long timeNow;

	setLED(LED_GREEN,FALSE);
	timeNow = getRTCinSeconds();
	markEndPlay(timeNow);
	
	//TODO: break (gracefully) after some time delay if never connected to another device
	setUSBHost(TRUE);

	// code below is borrowed from the delete fct
	strcpy(strLog,"\x0d\x0a");
	longToDecimalString(timeNow,strLog+2,5);
	strcat((char *)strLog,(const char *)": COPY ");
	
	strcpy(newPkgPath,INBOX_PATH);
	newPkgPath[0] = 'b'; //change a:/ drive to b:/ drive
	strcat(newPkgPath,NEW_PKG_SUBDIR);
	retCopy = copyApplicationOrMessage((char *)packageName,newPkgPath);
	copyListAudio(filenameList);
	setUSBHost(FALSE);
	return retCopy;
}

static int copyApplicationOrMessage(char * packageName, char *newPkgPath) {
	int retCopy;
	char *prefixCursor = NULL;

	if (0 == strncmp(packageName,CUSTOM_PKG_PREFIX,strlen(CUSTOM_PKG_PREFIX)))
		prefixCursor = CUSTOM_PKG_PREFIX;
	if (prefixCursor) {
		retCopy = copyApplication((char *)packageName + strlen(prefixCursor),newPkgPath);
	} else {
		retCopy = copyMessage((char *)packageName,newPkgPath);		
	}		
}

static int copyApplication(char * packageName, char *newPkgPath) {
	char path[PATH_LENGTH], strLog[PATH_LENGTH], filename[PATH_LENGTH], to[PATH_LENGTH];
	int ret, retCopy;
	struct f_info file_info;
	
	strcpy(path,newPkgPath);
	strcat(path,packageName);
	ret = mkdir((LPSTR)path);
	strcpy(path,USER_PATH);
	strcat(path,packageName);
	strcat(path,"/");
	strcpy(strLog,packageName);
	logString(strLog,ASAP);
	strcpy(filename,path);
	strcat(filename,"*.*");
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	while (ret >= 0) {
		strcpy(filename,path);
		strcat(filename,file_info.f_name);
		strcpy(to,INBOX_PATH);
		to[0] = 'b'; //change a:/ drive to b:/ drive
		strcat(to,NEW_PKG_SUBDIR);
		strcat(to,packageName); // to get directory name
		strcat(to,"/");
		strcat(to,file_info.f_name);
		retCopy = fileCopy(filename,to); //was unlink((LPSTR)filename);
		if (retCopy == -1) {
			strcpy(strLog,(const char *)"Copy Failed! ");
			strcat(strLog,filename);
		} else {
			recordStats(filename, 0L, STAT_COPIED, PKG_SYS+1);
			strcpy(strLog,filename);
			ret = _findnext(&file_info);
		}
		logString(strLog,BUFFER);
	}
	return retCopy;
}

static int copyMessage(char * packageName, char *newPkgPath) {
	char strLog[PATH_LENGTH], filename[PATH_LENGTH], to[PATH_LENGTH];
	char * cursor;
	int retCopy;

	cursor = (char *)packageName;
	if (LBstrncat((char *)strLog,cursor,PATH_LENGTH) == PATH_LENGTH-1)
		strLog[PATH_LENGTH-2] = '~';
	logString(strLog,BUFFER);
	strcpy(filename,USER_PATH);
	strcat(filename,cursor);
	strcat(filename,AUDIO_FILE_EXT);
	strcpy(to,newPkgPath);
	strcat(to,cursor);
	strcat(to,AUDIO_FILE_EXT);
	retCopy = fileCopy(filename,to); //was unlink((LPSTR)filename);
	if (retCopy == -1) {
		strcpy(strLog,(const char *)"Copy Failed! ");
		strcat(strLog,filename);
	} else {
		recordStats(filename, 0L, STAT_COPIED, PKG_SYS+1);
		strcpy(strLog,filename);
	}
	logString(strLog,BUFFER);
	return retCopy;
}

static void copyListAudio(const char * listName) {
	char path[PATH_LENGTH], strLog[PATH_LENGTH], filename[PATH_LENGTH], to[PATH_LENGTH];
	int ret, retCopy;
	struct f_info file_info;

	cpyTopicPath(path);
	strcpy(filename,path);
	strcat(filename,listName);
	strcat(filename,"*"); // to copy "*-LONG.a18" as well as short topic name
	strcat(filename,AUDIO_FILE_EXT);
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	while (ret >= 0 && (retCopy == 0)) {
		strcpy(filename,path);
		strcat(filename,file_info.f_name);
		strcpy(to,INBOX_PATH);
		strcat(to,SYS_UPDATE_SUBDIR);
		to[0] = 'b'; //change a:/ drive to b:/ drive
		strcat(to,path+3); // +3 removes the "a:/" but keeps "lists/", for example
		mkdir((LPSTR)to);
		strcat(to,file_info.f_name);
		retCopy = fileCopy(filename,to); //was unlink((LPSTR)filename);
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
	exchangeStats();
	setUSBHost(FALSE);
	return;
}

static void exchangeStats() {
//
// exchange stats 
// assumes a device keeps its stats in a STAT_DIR directory 
//         a device keeps stats from other devics in a OSTAT_DIR directory
// the code a to b and stats to ostats & vice versa
//
//  copy a:/system/stats/*.* to b:/system/ostats/ 

	char strLog[PATH_LENGTH], filename[PATH_LENGTH], to[PATH_LENGTH];
	int ret, retCopy;
	struct f_info file_info;

	mkdir((LPSTR)OSTAT_DIR);
	strcpy(filename,STAT_DIR); 
	strcat(filename,"*.*");
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	retCopy = 0;
	while (ret >= 0 && (retCopy == 0)) {
		strcpy(filename,STAT_DIR); 
		strcat(filename,file_info.f_name);
		strcpy(to,OSTAT_DIR);
		to[0] = 'b'; //change a:/ drive to b:/ drive
		strcat(to,file_info.f_name);
		retCopy = fileCopy(filename,to); //was unlink((LPSTR)filename);
		if (retCopy == -1) {
			strcpy(strLog,(const char *)"Stats a to b Copy Failed! ");
			strcat(strLog,filename);
		} else
			strcpy(strLog,filename);
			
		logString(strLog,BUFFER);
		ret = _findnext(&file_info);
	}

	
//  copy b:/system/stats/*.* to a:/system/ostats
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
		retCopy = fileCopy(filename,to); //was unlink((LPSTR)filename);
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

}