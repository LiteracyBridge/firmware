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

	cpyListPath(filePath,listName);
	strcat(filePath,listName);
	strcat(filePath,".txt");
	insertStringInFile(filePath,pkgName,0);

	cursor = getCurrentList(&pkgSystem.lists[context.package->idxMasterList]);
	if (!strcmp(cursor,listName)) {
		// list assigned to the recording is the current list (e.g. not user feedback on another category)
		tempList = &context.package->lists[1]; // todo: this is a hack -- I shouldn't know it is list[1] 
		tempList->currentFilePosition = 0;
		strcpy(tempList->currentString,pkgName); 
	}
}

int clearDeleteQueue(void) {
	char strQueuePath[PATH_LENGTH];
	char strMsgProfilePath[PATH_LENGTH],strMsgListPath[PATH_LENGTH];
	struct f_info fileInfo;
	int handle,ret;
	int found,i,lineCount,linesCounted;
	char buffer[READ_LENGTH+1],*packageName, *cursor;
	ProfileData *pd;
	
	// For each string in DELETE_QUEUE_FILENAME, look for references in all message list files.  If none found, delete.
	// When finished, delete DELETE_QUEUE_FILENAME.
	// Look at all message files, not just the ones in current profile or even only the ones that are active lists, 
	// because a category that lists the message could later be activated.

	strcpy(strQueuePath,SYSTEM_PATH);
	strcat(strQueuePath,(char *)DELETE_QUEUE_FILENAME);
	pd = getProfiles();
	linesCounted = 0;
	handle=tbOpen((LPSTR)strQueuePath,O_RDONLY);
	if (handle < 0) {
		logString(strQueuePath,ASAP,LOG_DETAIL);
		return -1; //no queue
	}
	do {
		getLine(-1,0);  // reset from prior use
		lineCount = 0;
		while ((packageName = getLine(handle,buffer)) && ++lineCount <= linesCounted) {}
			// This lineCount and linesCounted skips previously processed lines in this loop.
			// This is necessary because findDeleteStringFromFile() also calls getLine and resets everything. (TODO:not optimal)
		if (!*packageName)
			break;  // blank line at end of file
		getLine(handle,0); //reset file cursor to beginning of file
		linesCounted++;
		found = 0; 
		if (DEBUG_MODE) {
			logString((char *)"DELETE QUEUE: Looking for references to ",BUFFER,LOG_DETAIL);
			logString(packageName,ASAP,LOG_DETAIL);
		}
		for (i=0;i<pd->intTotalMessageLists;i++) {
			strcpy(strMsgProfilePath,LISTS_PATH);
			strcat(strMsgProfilePath,pd->ptrProfileMessageLists[i]);
			strcat(strMsgProfilePath,"/*");
			ret = _findfirst((LPSTR)strMsgProfilePath, &fileInfo,D_FILE);
			strcpy(strMsgListPath,strMsgProfilePath);
			while (ret >=0) {
				if (!strstr(fileInfo.f_name,LIST_MASTER)) {  // don't search _activeLists.txt
					cursor = strrchr(strMsgListPath, '/') + 1;
					*cursor = 0;  //remove *.* or previous filename 
					strcat(strMsgListPath,fileInfo.f_name);
					logString(strMsgListPath,BUFFER,LOG_DETAIL);
					if (-1 != findDeleteStringFromFile(NULL,strMsgListPath,packageName,FALSE)) {
						found = 1;
						if (DEBUG_MODE)
							logString((char *)"Found another reference - will not delete package.",BUFFER,LOG_NORMAL);
						break;
					}
				}
				ret = _findnext(&fileInfo);
			}
			if (found) 
				break;
		}
		if (!found) 
			deletePackage(packageName);
	} while (packageName);
	close(handle);
	unlink((LPSTR)strQueuePath);
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

	if (shouldDelete) {
		stop();
		strcpy(strLog,(char *)"DELETE ");
		
		prefixCursor = NULL;
		if (0 == strncmp(packageName,CUSTOM_PKG_PREFIX,strlen(CUSTOM_PKG_PREFIX)))
			prefixCursor = CUSTOM_PKG_PREFIX;
		if (prefixCursor) {
			cursor = packageName + strlen(prefixCursor);
			strcpy(path,USER_PATH);
			strcat(path,cursor);
			strcat(path,"/");
			strcat(strLog,cursor);
			logString(strLog,ASAP,LOG_NORMAL);
			strcpy(filename,path);
			strcat(filename,"*.*");
			ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
			while (ret >= 0) {
				strcpy(filename,path);
				strcat(filename,file_info.f_name);
				ret = unlink((LPSTR)filename);
				if (ret != -1) 
					logString(file_info.f_name,BUFFER,LOG_NORMAL);
				ret = _findnext(&file_info);
			}
			ret = rmdir((LPSTR)path);
		}
		else {
			cursor = packageName;
			if (LBstrncat((char *)strLog,cursor,LOG_LENGTH) == LOG_LENGTH-1)
				strLog[LOG_LENGTH-2] = '~';
			logString(strLog,BUFFER,LOG_NORMAL);
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
