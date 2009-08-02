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
	int ret;
	char to[80], strLog[60],filename[80], path[80];
	char *cursor, *prefixCursor;
	int maxTrials = 5;
	struct f_info file_info;
	long timeNow;

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
	else if (0 == strncmp(packageName,QUIZ_PKG_PREFIX,strlen(QUIZ_PKG_PREFIX)))
		prefixCursor = QUIZ_PKG_PREFIX;
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
			testCopy(filename,to,maxTrials); //was unlink((LPSTR)filename);
			if (ret != -1) 
				logString(file_info.f_name,BUFFER);
			ret = _findnext(&file_info);
		}
	}
	else {
		cursor = (char *)packageName;
		if (LBstrncat((char *)strLog,cursor,60) == 59)
			strLog[58] = '~';
		logString(strLog,BUFFER);
		strcpy(filename,USER_PATH);
		strcat(filename,cursor);
		strcat(filename,AUDIO_FILE_EXT);
		strcat(to,cursor);
		strcat(to,AUDIO_FILE_EXT);
		ret = testCopy(filename,to,maxTrials); //was unlink((LPSTR)filename);
	}
	// this needs to be part of inbox processing
/*	if(filenameList[0]) {
		strcpy(to,LIST_PATH);
		to[0] = 'b'; //change a:\\ drive to b:\\ drive
		strcat(to,filenameList);
		strcpy(temp,package); // since appendStringToFile destroys string
		appendStringToFile(to,temp); //destroys temp
	}
*/
	strcpy(filename,LIST_PATH); 
	strcat(filename,filenameList);
	strcat(filename,"*");
	strcat(filename,AUDIO_FILE_EXT);
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	while (ret >= 0) {
		strcpy(filename,LIST_PATH);
		strcat(filename,file_info.f_name);
		strcpy(to,INBOX_PATH);
		to[0] = 'b'; //change a:\\ drive to b:\\ drive
		strcat(to,LIST_PATH+3); // +3 removes the "a:\" but keeps "\lists\", for example
		strcat(to,file_info.f_name);
		ret = testCopy(filename,to,maxTrials); //was unlink((LPSTR)filename);
		if (ret != -1) 
			logString(file_info.f_name,BUFFER);
		ret = _findnext(&file_info);
	}

	if (!ret) 
		insertSound(&pkgSystem.files[DELETED_FILE_IDX],NULL,TRUE);  // todo: give it a unique sound
	else
		ret = -1;
	
	setUSBHost(FALSE);
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