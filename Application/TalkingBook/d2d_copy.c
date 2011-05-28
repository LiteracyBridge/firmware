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
#include "Include/files.h"
#include "Include/sys_counters.h"
#include "Include/SD_reprog.h"

extern APP_IRAM SystemCounts systemCounts;
static char * longToDecimalStringZ(long l, char * string, int numberOfDigits);

extern int setUSBHost(BOOL enter);
static int copyApplicationOrMessage(char * packageName, char *newPkgPath);
static int copyApplication(char * packageName, char *newPkgPath);
static int copyMessage(char * packageName, char *newPkgPath);
static void copyListAudio(const char * listName);
static void exchangeStatsCSV(void);
static int cloneDir(char *fromdir, char *todir);
static int copyfiles(char *fromdir, char *todir);

int d2dCopy(const char * packageName,const char * filenameList) {
	int retCopy;
	char newPkgPath[PATH_LENGTH], strLog[PATH_LENGTH];
//	int maxTrials = 5;
	long timeNow;
	ListItem *list;
	char *localPackageName;
		
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
	if (packageName) 
		retCopy = copyApplicationOrMessage((char *)packageName,newPkgPath);
	else { // copy entire category
		list = &pkgSystem.lists[1];
		list->currentFilePosition = -1;
		localPackageName = getCurrentList(list);
		do {
			logString((char *)"catcopy",BUFFER);
			logString(localPackageName,BUFFER);
			retCopy = copyApplicationOrMessage(localPackageName,newPkgPath);				
			localPackageName = getPreviousList(list);
		} while (list->currentFilePosition);
	}
	copyListAudio(filenameList);
	setUSBHost(FALSE);
	return retCopy;
}

static int copyApplicationOrMessage(char * packageName, char *newPkgPath) {
	int retCopy;
	char *prefixCursor = NULL;

	setLED(LED_GREEN,TRUE);
	if (0 == strncmp(packageName,CUSTOM_PKG_PREFIX,strlen(CUSTOM_PKG_PREFIX)))
		prefixCursor = CUSTOM_PKG_PREFIX;
	if (prefixCursor) {
		retCopy = copyApplication((char *)packageName + strlen(prefixCursor),newPkgPath);
	} else {
		retCopy = copyMessage((char *)packageName,newPkgPath);		
	}		
	setLED(LED_GREEN,FALSE);
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
	filename[0] = '*';  // to copy the short topic name and the instruction that goes with it
	strcpy(filename+1,path);
	strcat(filename,listName);
	strcat(filename,"*"); // to copy the short topic name and the instruction that goes with it
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
//	exchangeStats();
	exchangeStatsCSV();
	setUSBHost(FALSE);
	return;
}

#if 0
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
#endif

void buildMyStatsCSV() {
	void buildCSVline(char *to, char *filename, struct ondisk_filestats *tmpstats);
	char strLog[PATH_LENGTH], to[PATH_LENGTH], filename[PATH_LENGTH], lineout[180];
	int ret, bytesToWrite, mystats, rHandle, retCopy;
	struct f_info file_info;
	char *cp;

	strcpy(to, STAT_DIR);
	strcat(to, (const char *)TB_SERIAL_NUMBER_ADDR);
	strcat(to, ".csv");
	cp = strrchr(to, '/') + 1;

	mystats = open((LPSTR)to, O_CREAT|O_RDWR|O_TRUNC);
	if(mystats < 0) {
		return;
	}

//  first line is serial_number, cycle number
	strcpy(lineout, (LPSTR)TB_SERIAL_NUMBER_ADDR);
	strcat(lineout, ",");
	longToDecimalString(systemCounts.powerUpNumber,(char *)strLog,4);
	strcat(lineout, strLog);
	bytesToWrite = convertDoubleToSingleChar(filename,lineout,TRUE);
	retCopy = write(mystats, (UINT32)filename << 1, bytesToWrite);

	strcpy(filename,STAT_DIR); 
	strcat(filename,"*.*");

	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);

	for (;ret >= 0; ret = _findnext(&file_info)) {
		struct ondisk_filestats tmpstats = {0};
		
		if (!strcmp(file_info.f_name, SNCSV)) {
			continue;
		}
		if (!strcmp(file_info.f_name, cp)) {
			continue;
		}
		strcpy(filename,STAT_DIR); 
		strcat(filename,file_info.f_name);
				
		rHandle = open((LPSTR)filename,O_RDONLY);	
		if(rHandle >= 0) {
			retCopy = read(rHandle, (UINT32)&tmpstats << 1, sizeof(tmpstats) << 1);
			close(rHandle);

			buildCSVline(lineout, file_info.f_name, &tmpstats);

			bytesToWrite = convertDoubleToSingleChar(filename,lineout,TRUE);
			
			retCopy = write(mystats, (UINT32)filename << 1, bytesToWrite);
		}
	}

	close(mystats);
}
//static
void exchangeStatsCSV() {
//
// running as usb host, A: is usb host sd card, B: is usb client sd card
// assumes a device keeps its stats in a STAT_DIR directory 
//         a device keeps stats from other devics in a OSTAT_DIR directory
//         both devices maintain a .csv summary of their stats in STAT_DIR  (buildMyStatsCSV)
// copy a:/STAT_DIR/"my serial number".csv to b:/OSTAT_DIR/"my serial number".csv 
// copy b:/STAT_DIR/"other serial number".csv to a:/OSTAT_DIR/"other serial number".csv
//   no third device stats yet

	char linein[PATH_LENGTH], from[PATH_LENGTH], to[PATH_LENGTH], *cp1, cp2;
	int ret, i, j, f;

	mkdir((LPSTR)CLI_OSTAT_DIR);	
	
	strcpy(to, CLI_OSTAT_DIR);
	
//	mkdir("a:/b/system/ostats");  // remove after testing
//	strcpy(to, "a:/b/system/ostats/");  // remove after testing with a b folder on a:

	strcat(to, (const char *)TB_SERIAL_NUMBER_ADDR);
	strcat(to, ".csv");
	
	
	strcpy(from, STAT_DIR);
	strcat(from, (const char *)TB_SERIAL_NUMBER_ADDR);
	strcat(from, ".csv");
	
	ret = fileCopy((LPSTR)from, (LPSTR)to);
			
	strcpy(from, CLI_STAT_DIR);
//	strcpy(from, "a:/b/system/stats/");  // remove after testing with a b folder on a:

	cp1 = from + strlen(from);  // save this position	
	strcat(from, SNCSV);	
	
	f = open((LPSTR)from, O_RDONLY); 
	if(f <= 0) {
		return;
	}
	ret = read(f, (unsigned long)&linein << 1, 128);
	close(f);
	if(ret <= 0) {
		return;
	}
	
	strcpy(to, OSTAT_DIR);
	i = strlen(to);
	cp2 = to + i;
	
	for(j=0; j<ret; j++) {
		to[i] = linein[j] & 0xff;
		if(to[i] == ',') 
			to[i] = 0;
		if(to[i++] == 0)
			break;
		to[i] = linein[j] >> 8;
		if(to[i] == ',') 
			to[i] = 0;
		if(to[i++] == 0)
			break;
	}
	
	strcat(to, ".csv");
	strcpy(cp1, cp2);
	
	ret = fileCopy((LPSTR)from, (LPSTR)to);

//	
// Logic for which device has the most current stats for devices they both have connected to goes here
//
}

// supress leading zeros from longToDecimalString
//
// WARNING:  changes string
//
static 
char *longToDecimalStringZ(long l, char * string, int numberOfDigits) {
	int i;
	char *cp;
	
	longToDecimalString(l, string, numberOfDigits);
	cp = string;
	for(i=0; i<(numberOfDigits-1); i++) {
		if(cp[i] == '0')
			string++;
	} 
	return(string);
}
void buildCSVline(char *to, char *filename, struct ondisk_filestats *tmpstats)
{
	char num[12], *strout, *zstrout;

	strcpy(to, filename);
	strcat(to, ",");
	strout = &num[0];
	zstrout = longToDecimalStringZ((long) tmpstats->stat_num_opens, strout, 6);
	strcat(to, zstrout);
	strcat(to, ",");
	strout = &num[0];
	zstrout = longToDecimalStringZ((long) tmpstats->stat_num_completions, strout, 6);
	strcat(to, zstrout);
	strcat(to, ",");
	strout = &num[0];
	zstrout = longToDecimalStringZ((long) tmpstats->stat_num_copies, strout, 6);
	strcat(to, zstrout);
	strcat(to, ",");
	strout = &num[0];
	zstrout = longToDecimalStringZ((long) tmpstats->stat_num_survey1, strout, 6);
	strcat(to, zstrout);
	strcat(to, ",");
	strout = &num[0];
	zstrout = longToDecimalStringZ((long) tmpstats->stat_num_apply, strout, 6);
	strcat(to, zstrout);
	strcat(to, ",");
	strout = &num[0];
	zstrout = longToDecimalStringZ((long) tmpstats->stat_num_useless, strout, 6);
	strcat(to, zstrout);			
}
int
copyLanguage(char *language) {
	char from[PATH_LENGTH], to[PATH_LENGTH];
	int ret;
	
	cpyTopicPath(from);
	strcpy(to,from);
	to[0] = 'b';
	ret = cloneDir(from,to);
	
	//don't add language to languages.txt if there was an error in cloneDir (or if it's already listed)	
	if (ret >= 0) { 
		strcpy(to,LANGUAGES_PATH);
		to[0] = 'b';
		strcat(to,SYSTEM_ORDER_FILE);
		strcat(to,".txt");	
		strcpy(from,language);
		ret = appendStringToFile(to,from);
	
		//TODO: move this to openList() if the directory isn't there the first time
		strcpy(from,LISTS_PATH);
		from[0] = 'b';
		strcat(from,TEMPLATE_LISTS_DIR);
		strcpy(to,LISTS_PATH);
		to[0] = 'b';
		strcat(to,language);
		ret += cloneDir(from,to);
	}
	return ret;
}


// while in usb host mode clone the host to the client
//   format client sd card (B:)
//   copy most files from a: to b: (no stats, LOG_FILE, or SYSTEM_VARIABLE_FILE)
//   special cases
//       (a:/system/*.img to B:\*.img to force client to reprogram)
int
cloneDevice() {
	int ret, len_path, newlog;
	char path[PATH_LENGTH], to[PATH_LENGTH], from[PATH_LENGTH];
	struct f_info fi;
	
// format b:        [ _format(0, FAT16_Type);  works on a:]

	ret = _format(1, FAT16_Type);   // 0 == a, 1 == b?

// recursively copy files

	ret = cloneDir("a:/", "b:/");
//	mkdir((LPSTR)"a:/clonetest");   // Test - remove	
//	ret = cloneDir("a:/", "a:/clonetest"); // Test - remove	
	
// special cases
	
	strcpy(path, LOG_FILE);
	path[0] = 'b';
//	ret = unlink((LPSTR)path);
	newlog = tbOpen((LPSTR)path, O_CREAT|O_RDWR);
	if(newlog >= 0) {
		close(newlog);
		strcpy(to, "CLONED by ");
		strcat(to, (char *) (TB_SERIAL_NUMBER_ADDR + CONST_TB_SERIAL_PREFIX_LEN));
		appendStringToFile(path, to);
	}
	
	strcpy(path, SYSTEM_VARIABLE_FILE);
	path[0] = 'b';
	ret = unlink((LPSTR)path);
	ret = tbOpen((LPSTR)path, O_CREAT|O_RDWR);
	if(ret >= 0) {
		close(ret);
	}

	strcpy(path, SYSTEM_PATH);
	path[0] = 'b';	
//	strcpy(path, "a:/clonetest/System/");   // test remove
	
	len_path = strlen(path);
	if(path[len_path-1] != '/') {
		strcat(path, "/");
		len_path++;
	}
	strcat(path, "*.img");

	// move any b:/system/*.img to b:/Inbox/sys-updates so it will be processed by processSystemFiles
	for(ret =_findfirst((LPSTR)path, &fi, D_FILE); ret >= 0; ret = _findnext(&fi)) {	
		strcpy(to,INBOX_PATH);
		to[0] = 'b';
		strcat(to,SYS_UPDATE_SUBDIR);
		strcat(to, fi.f_name);
		strcpy(from, path);
		from[len_path] = 0;
		strcat(from, fi.f_name);
		ret = rename((LPSTR)from, (LPSTR)to);
//		ret = _copy((LPSTR)from, (LPSTR)to);
	}
}

static int 
cloneDir(char *fromdir, char *todir) {
// 	copy directory tree below fromdir (all subdirectories and files at all levels)
	int ret, r1, len_from, len_to, fret;
	char from[PATH_LENGTH], fromfind[PATH_LENGTH], to[PATH_LENGTH]; //, lastdir[FILE_LENGTH];
	char dirname[80];
//	char dbgbuf[96], dbghex[16];

	struct f_info fi;
	
	fret = 0;
	
	strcpy(from, fromdir);
	len_from = strlen(from);
	
	if(from[len_from-1] != '/') {
		strcat(from, "/");
		len_from++;
	}
	strcat(from, "*");
	
	strcpy(to, todir);
	len_to = strlen(to);
	ret = mkdir((LPSTR)to);	// just to be safe
	if(to[len_to-1] != '/') {
		strcat(to, "/");
		len_to++;
	}
	strcpy(fromfind,from);
	
	ret =_findfirst((LPSTR)fromfind, &fi, D_DIR);
	from[len_from] = 0;
//	lastdir[0] = 0;
	
	for (; ret >= 0; ret = _findnext(&fi)) {
		if(! (fi.f_attrib & D_DIR)) {
//			ret = _findnext(&fi);
			continue;
		}
	
		if(fi.f_name[0]=='.') {
//			ret = _findnext(&fi);
			continue;
		}
//		if(!strcmp("clonetest", fi.f_name)) {
//			ret = _findnext(&fi);
//			continue;
//		}
		from[len_from] = 0;
		to[len_to]= 0;
				
		strcat(from, fi.f_name);	
		strcat(to, fi.f_name);
		
		r1 = mkdir((LPSTR)to);
		
//		longToHexString((long)r1, (char *)dbghex, 1);
//		strcpy(dbgbuf, dbghex);
//		strcat(dbgbuf, " mkdir ");
//		strcat(dbgbuf, to);
 //		logString(dbgbuf,ASAP);
	
		if(!strncmp(from, STAT_DIR, strlen(from))) {
			continue;
		}
		if(!strncmp(from, OSTAT_DIR, strlen(from))) {
			continue;
		}
		
		strcpy(dirname, fi.f_name);
		fret += cloneDir (from, to);
				
		fret++;
		
		ret =_findfirst((LPSTR)fromfind, &fi, D_DIR);  //necessary to reset after rmdir?
		while(ret >= 0 && strcmp(dirname, fi.f_name)) {
			ret = _findnext(&fi);
		}
	}
	
	from[len_from] = 0;
	to[len_to]= 0;
	fret += copyfiles(from, to);
	
	return(fret);
}
//
// copy all files in fromdir to todir
//
static int copyfiles(char *fromdir, char *todir)
{
	int ret, r1, len_from, len_to, fret;
	char from[80], to[80];
	struct f_info fi;
//	char dbgbuf[96], dbghex[16];
	
	fret = 0;
	
	strcpy(from, fromdir);
	len_from = strlen(from);
	
	if(from[len_from-1] != '/') {
		strcat(from, "/");
		len_from++;
	}
	strcat(from, "*.*");
	
	strcpy(to, todir);
	len_to = strlen(to);
//	mkdir(to);	// just to be safe
	if(to[len_to-1] != '/') {
		strcat(to, "/");
		len_to++;
	}
			
	ret =_findfirst((LPSTR)from, &fi, D_FILE);
	for(; ret >= 0; ret = _findnext(&fi)) {
		if(fi.f_name[0] != '.') {
			from[len_from] = 0;
			to[len_to]= 0;
			strcat(from, fi.f_name);
			strcat(to, fi.f_name);
			
			if(!strcmp(from, LOG_FILE)) {
				continue;
			}
			if(!strcmp(from, SYSTEM_VARIABLE_FILE)) {
				continue;
			}		

			if(!strcmp(from+strlen(from)-strlen(SERIAL_EXT), SERIAL_EXT)) {
				continue;
			}		
			
				setLED(LED_GREEN,TRUE);
				
//				longToHexString((long)r1, (char *)dbghex, 1);
//				strcpy(dbgbuf, dbghex);
//				strcat(dbgbuf, " copy ");
//				strcat(dbgbuf, to);
//		 		logString(dbgbuf,ASAP);

				setLED(LED_RED,FALSE);
				if (DEBUG_MODE)
					logString(from,ASAP);
				r1 = _copy((LPSTR)from, (LPSTR)to);
				playBip();
				setLED(LED_RED,TRUE);
				setLED(LED_GREEN,FALSE);
		}
		if (r1 == -1) 
			fret = -(1<<14); // fret = very negative number so that we know at least one copy didn't work 
		else
			fret++;
	}
	return(fret);
}

	
