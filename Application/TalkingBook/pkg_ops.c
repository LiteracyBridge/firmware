// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/talkingbook.h"
#include "Include/device.h"
#include "Include/files.h"
#include "Include/lists.h"
#include "Include/containers.h"
#include "Include/audio.h"
#include "Include/pkg_ops.h"

void packageRecording(char * pkgName, char *listName) {
	// adds name to current subject list
	ListItem *tempList;
	char *cursor;	
	char filePath[PATH_LENGTH];
//	long pos;
	cursor = getCurrentList(&pkgSystem.lists[context.package->idxMasterList]);
	if (strcmp(cursor,listName)) {
		// list assigned to the recording is not the current list (e.g. user feedback forced into that category)
		cpyListPath(filePath,listName);
		strcat(filePath,listName);
		strcat(filePath,".txt");
		insertStringInFile(filePath,pkgName,0);
	} else {
		tempList = &context.package->lists[1]; // todo: this is a hack -- I shouldn't know it is list[1] 
		
	//	if (tempList->currentFilePosition == -1 || strlen(tempList->currentString) == 0)
	//		pos = 0;
	//	else 
	//		pos = tempList->currentFilePosition + strlen(tempList->currentString) + 2;
		insertIntoList(tempList,0,pkgName); //insert new recording at top of list
		tempList->currentFilePosition = 0;
		strcpy(tempList->currentString,pkgName); 
	}
}

int deletePackage(char * packageName) {
	// check master list; iterate through each list; if packageName not found in any list, delete package files
	// return 0 if not found in any list and package deleted; return -1 otherwise (usually that it was found in another list)
	const int LOG_LENGTH = PATH_LENGTH + 20;
	char filename[PATH_LENGTH], path[PATH_LENGTH];
	char strLog[LOG_LENGTH];
	int ret, attempt;
	char *cursor, *prefixCursor;
	struct f_info file_info;
	BOOL shouldDelete = TRUE;
	const int MAX_RETRIES = 3;
	long timeNow;
/*
	// This code checks if the last list reference to the file has been removed
	
	charCursor = getCurrentList(&context.package->lists[context.package->idxMasterList],context.package);			
	idxList = context.package->lists[context.package->idxMasterList].currentFilePosition;
	idxStartingList = idxList;
	do {
		strcpy(filename,charCursor);
		strcat(filename,".txt"); //todo: move to config file
		if (!findDeleteStringFromFile(LIST_PATH,filename,packageName,FALSE)) {
			shouldDelete = FALSE;
			break;
		}
		charCursor = getPreviousList(&context.package->lists[context.package->idxMasterList],context.package);
		idxList = context.package->lists[context.package->idxMasterList].currentFilePosition;
	} while (charCursor && idxList != idxStartingList);
*/
	if (shouldDelete) {
		if (context.packageStartTime) {
			timeNow = getRTCinSeconds();
			markEndPlay(timeNow);
		}
		stop();
		strcpy(strLog,"\x0d\x0a");
		longToDecimalString(timeNow,strLog+2,5);
		strcat((char *)strLog,(const char *)": DELETE ");
		
		prefixCursor = NULL;
		if (0 == strncmp(packageName,CUSTOM_PKG_PREFIX,strlen(CUSTOM_PKG_PREFIX)))
			prefixCursor = CUSTOM_PKG_PREFIX;
		if (prefixCursor) {
			cursor = packageName + strlen(prefixCursor);
			strcpy(path,USER_PATH);
			strcat(path,cursor);
			strcat(path,"/");
			strcat(strLog,cursor);
			logString(strLog,ASAP);
			strcpy(filename,path);
			strcat(filename,"*.*");
			ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
			while (ret >= 0) {
				strcpy(filename,path);
				strcat(filename,file_info.f_name);
				ret = unlink((LPSTR)filename);
				if (ret != -1) 
					logString(file_info.f_name,BUFFER);
				ret = _findnext(&file_info);
			}
			ret = rmdir((LPSTR)path);
		}
		else {
			cursor = packageName;
			if (LBstrncat((char *)strLog,cursor,LOG_LENGTH) == LOG_LENGTH-1)
				strLog[LOG_LENGTH-2] = '~';
			logString(strLog,BUFFER);
			strcpy(filename,USER_PATH);
			strcat(filename,cursor);
			strcat(filename,AUDIO_FILE_EXT);
			for (attempt=0;attempt < MAX_RETRIES;attempt++) {			
				ret = unlink((LPSTR)filename);
				if (ret != -1)
					break;
				else
					logException(29,filename,0);			
			}
		}
		if (!(shouldDelete && ret == 0))
			ret = -1;
	}
	return ret;
}	
