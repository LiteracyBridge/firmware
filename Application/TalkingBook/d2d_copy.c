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

static char * longToDecimalStringZ(long l, char * string, int numberOfDigits);

extern int setUSBHost(BOOL enter);
static int copyApplicationOrMessage(char * packageName, char *newPkgPath);
static int copyApplication(char * packageName, char *newPkgPath);
static int copyMessage(char * packageName, char *newPkgPath);
static void copyListAudio(const char * listName);
static void exchangeStatsCSV(void);

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

//static
void exchangeStatsCSV() {
//
// running as usb host, A: is usb host sd card, B: is usb client sd card
// assumes a device keeps its stats in a STAT_DIR directory 
//         a device keeps stats from other devics in a OSTAT_DIR directory
// create b:/OSTAT_DIR/"my serial number".csv with one line per file that has stats
// create a:/OSTAT_DIR/"other serial number".csv with one line per file that has stats
//   no third device stats yet

	char strLog[PATH_LENGTH], filename[PATH_LENGTH], to[PATH_LENGTH], num[12], *strout, *zstrout;
	int ret, retCopy, rHandle, wrk, bytesToWrite, i, j;
	struct f_info file_info;
	int hoststats, clientstats;
	
//
//  for every file in a:/system/stats add a csv line in b:/ostats/host_serial_number.csv
//     the first line will be host_serial_number,cycle number

	mkdir((LPSTR)CLI_OSTAT_DIR);	
	strcpy(to, CLI_OSTAT_DIR);
//	mkdir("a:/b/system/ostats");  // remove after testing
//	strcpy(to, "a:/b/system/ostats/");  // remove after testing with a b folder on a:
	
	strcat(to, (const char *)TB_SERIAL_NUMBER_ADDR);
	strcat(to, ".csv");
	fileCopy(STAT_DIR SNCSV, to); // will create to or truncate
	
	hoststats = open((LPSTR)to, O_RDWR);
	if(hoststats < 0) {
		strcpy(strLog, "Cannot create ");
		strcat(strLog, to);
		strcat(strLog, " - copying stats failed");
		logString(strLog ,ASAP);
		return;
	}
	lseek(hoststats, 0, SEEK_END);

	strcpy(filename,STAT_DIR); 
	strcat(filename,"*.*");
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	for (;ret >= 0; ret = _findnext(&file_info)) {
		struct ondisk_filestats tmpstats = {0};
		
		if (!strcmp(file_info.f_name, SNCSV)) {
			continue;
		}
		strcpy(filename,STAT_DIR); 
		strcat(filename,file_info.f_name);
				
		rHandle = open((LPSTR)filename,O_RDONLY);	
		if(rHandle >= 0) {
			retCopy = read(rHandle, (UINT32)&tmpstats << 1, sizeof(tmpstats) << 1);
			close(rHandle);
//			sprintf(to,"%s,%lu,%lu,%lu",
//					file_info.f_name,
//					tmpstats.stat_num_opens,
//					tmpstats.stat_num_completions,
//					tmpstats.stat_num_copies);
			strcpy(to, file_info.f_name);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_opens, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_completions, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_copies, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_survey1, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_apply, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_useless, strout, 6);
			strcat(to, zstrout);
					
			bytesToWrite = convertDoubleToSingleChar(filename,to,TRUE);
			
			retCopy = write(hoststats, (UINT32)filename << 1, bytesToWrite);
		}
			
		logString(to,BUFFER);		
	}
	
	close(hoststats);

	
//  from b:\stats\*.* create a:\ostat\usbclientname.csv
//  for every file in b:/system/stats add a csv line in a:/ostats/host_serial_number.csv
//     the first line will be client_serial_number,cycle number

// find out who client is
	ret = open((LPSTR)CLI_STAT_DIR SNCSV, O_RDONLY);
//	ret = open("a:/b/system/stats/SN.csv", O_RDONLY);  // for testing remove
	if(ret < 0) {
		strcpy(strLog, "Cannot create ");
		strcat(strLog, to);
		strcat(strLog," - copying client stats failed");
		logString(strLog ,ASAP);
		return;
	}
	wrk = read(ret, (unsigned long)&strLog << 1, sizeof(strLog));
	close(ret);
	
	if(wrk <= 0) {
		strcpy(strLog, "Cannot read ");
		strcat(strLog, CLI_STAT_DIR SNCSV);
		strcat(strLog, " - copying client stats failed");
		logString(strLog ,ASAP);
		return;
	}
			
	strcpy(to, OSTAT_DIR);	
	i = strlen(to);
	for(j=0; j<wrk; j++) {
		to[i] = strLog[j] & 0xff;
		if(to[i] == ',') 
			to[i] = 0;
		if(to[i++] == 0)
			break;
		to[i] = strLog[j] >> 8;
		if(to[i] == ',') 
			to[i] = 0;
		if(to[i++] == 0)
			break;
	}
	strcat(to, ".csv");
	
	fileCopy(CLI_STAT_DIR SNCSV, to); // will create to or truncate
//	fileCopy("a:/b/system/stats/SN.csv", to);  // for testing , remove
	
	clientstats = open((LPSTR)to, O_RDWR);
	if(clientstats < 0) {
		strcpy(strLog, "Cannot create ");
		strcat(strLog, to);
		strcat(strLog, " - copying client stats failed");
		logString(strLog ,ASAP);
		return;
	}
	lseek(clientstats, 0, SEEK_END);
	
	strcpy(filename,CLI_STAT_DIR);
//	strcpy(filename, "a:/b/system/stats/");   // for testing on folder a:/b   remove for normal use
	
	strcat(filename,"*.*");
	ret =_findfirst((LPSTR)filename, &file_info, D_FILE);
	for (;ret >= 0; ret = _findnext(&file_info)) {
		struct ondisk_filestats tmpstats = {0};
		
		strcpy(filename,CLI_STAT_DIR); 
//		strcpy(filename, "a:/b/system/stats/");   // for testing on folder a:/b   remove for normal use
		strcat(filename,file_info.f_name);
				
		if (!strcmp(file_info.f_name, SNCSV)) {
			continue;
		}
		
		rHandle = open((LPSTR)filename,O_RDONLY);	
		if(rHandle >= 0) {
			retCopy = read(rHandle, (UINT32)&tmpstats << 1, sizeof(tmpstats) << 1);
			close(rHandle);
//			sprintf(to,"%s,%lu,%lu,%lu",
//					file_info.f_name,
//					tmpstats.stat_num_opens,
//					tmpstats.stat_num_completions,
//					tmpstats.stat_num_copies);
			strcpy(to, file_info.f_name);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_opens, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_completions, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_copies, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_survey1, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_apply, strout, 6);
			strcat(to, zstrout);
			strcat(to, ",");
			strout = &num[0];
			zstrout = longToDecimalStringZ((long) tmpstats.stat_num_useless, strout, 6);
			strcat(to, zstrout);
					
			bytesToWrite = convertDoubleToSingleChar(filename,to,TRUE);

			retCopy = write(clientstats, (UINT32)filename << 1, bytesToWrite);
		}	
				
		logString(to,BUFFER);
	}
	close(clientstats);
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
		strcat(to, (LPSTR) (TB_SERIAL_NUMBER_ADDR + CONST_TB_SERIAL_PREFIX_LEN));
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
		ret = rename(from, to);
//		ret = _copy((LPSTR)from, (LPSTR)to);
	}
}

static int 
cloneDir(char *fromdir, char *todir) {
// 	copy directory tree below fromdir (all subdirectories and files at all levels)
	int ret, r1, len_from, len_to, len, fret;
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
			
				setLED(LED_GREEN,FALSE);
				setLED(LED_RED,TRUE);
				r1 = _copy((LPSTR)from, (LPSTR)to);
				
//				longToHexString((long)r1, (char *)dbghex, 1);
//				strcpy(dbgbuf, dbghex);
//				strcat(dbgbuf, " copy ");
//				strcat(dbgbuf, to);
//		 		logString(dbgbuf,ASAP);

				setLED(LED_RED,FALSE);
				if (r1 != -1) {
					setLED(LED_GREEN,TRUE);
				}
		}
		fret++;
	}
	return(fret);
}


