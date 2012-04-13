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
#include "Include/filestats.h"
#include "Include/files.h"
#include "Include/sys_counters.h"
#include "Include/SD_reprog.h"
#include "Include/d2d_copy.h"

extern APP_IRAM SystemCounts systemCounts;
static char * longToDecimalStringZ(long l, char * string, int numberOfDigits);

extern int setUSBHost(BOOL enter);
static void getAudioFeedback(void);
static int copyApplicationOrMessage(char * packageName, char *newPkgPath);
static int copyApplication(char * packageName, char *newPkgPath);
static int copyMessage(char * packageName, char *newPkgPath);
static void copyListAudio(const char * listName);
static void exchangeStatsCSV(void);
static int copyfilesFiltered(char *fromdir, char *todir);
static void getStats(void);

static void getAudioFeedback(void) {
	ProfileData profiles;
	char path[PATH_LENGTH];
	char pathAudioFile[PATH_LENGTH];	
	char buffer[READ_LENGTH+1];
	int i, j, linesRead, gotFeedback, ret, handle;
	char *cursor, *markAudioFile;

	if (DEBUG_MODE)
		logString((char *)"getting other device's audio feedback",ASAP);
	strcpy(path,SYSTEM_PATH);
	path[0] = 'b';
	strcat(path,PROFILE_ORDER_FILE);
	strcat(path,".txt");
	
	ret = loadProfileNames(path,&profiles);
	if (!ret) {
		logString((char *)"no profiles found",BUFFER);
		logString(path,ASAP);
		return;
	} else
		i = profiles.intTotalMessageLists;

	strcpy(pathAudioFile,USER_PATH);
	pathAudioFile[0] = 'b';
	markAudioFile = pathAudioFile + strlen(pathAudioFile);
	do {
		i--;		
		strcpy(path,"0");
		path[0] += i;
		logString(path,BUFFER);
		
		linesRead = 0;
		gotFeedback = 0;
		do {			
			strcpy(path,LISTS_PATH);
			path[0] = 'b';
			strcat(path,profiles.heapMessageLists[i]);
			strcat(path,"/" FEEDBACK_CATEGORY ".txt");
			logString(path,BUFFER);
			handle = open((LPSTR)path,O_RDONLY);
			if (handle == -1) {
				strcpy(buffer,(char *)"could not access ");
				strcat(buffer,path);	
				logString(buffer,ASAP);
				break;
			}
			getLine(-1,0);  // reset in case at end from prior use			
			for (j=0;j <= linesRead; j++) {
				cursor = getLine(handle,buffer);
			}
			close(handle);
			linesRead++;
			if (!cursor) 
				break;
			if (strlen(cursor) < FILE_LENGTH) {
				strcpy(markAudioFile,cursor);
				strcat(markAudioFile,AUDIO_FILE_EXT);
				logString((char *)"_copy",BUFFER);
				logString(pathAudioFile,BUFFER);
				strcpy(path,INBOX_PATH);
				strcat(path,NEW_PKG_SUBDIR);
				strcat(path,markAudioFile);
				logString(path,ASAP);
				ret = _copy((LPSTR)pathAudioFile,(LPSTR)path);
				if (!gotFeedback && !ret)
					gotFeedback = 1;
			}
		} while (1);
		if (gotFeedback) {
			if (DEBUG_MODE)
				logString((char *)"ensuring 'feedback' is added to the other device's list for this profile",ASAP);
			// If there's at least one feedback msg for this profile, 
			// ensure the feedback category is added to _activeLists.txt for this device, for this profile.
			strcpy(path,LISTS_PATH);
			strcat(path,profiles.heapMessageLists[i]);
			strcat(path, "/");
			strcat(path,(char *)LIST_MASTER);
			strcat(path,(char *)".txt");
			strcpy(buffer, FEEDBACK_CATEGORY);
			ret = findDeleteStringFromFile((char *)NULL, path, buffer, 0);
			if (ret == -1)
				ret = appendStringToFile(path,buffer);		
		}
	} while (i > 0);
}

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
	retCopy = setUSBHost(TRUE);
	if (retCopy == 0) {
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
				if (retCopy == -1)
					break;
				localPackageName = getPreviousList(list);
			} while (list->currentFilePosition);
		}
		if (retCopy == 0) {
			setLED(LED_RED,TRUE);
			copyListAudio(filenameList);
//   built in device.c setOperationalMode when device halts or sleeps
//		  	buildMyStatsCSV();
			exchangeStatsCSV();
			setLED(LED_RED,FALSE);
		}
		setUSBHost(FALSE);
		if (retCopy != 0)
			logException(20,localPackageName,RESET);
	}
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
	return retCopy;
}

static int copyApplication(char * packageName, char *newPkgPath) {
	char path[PATH_LENGTH], strLog[PATH_LENGTH], filename[PATH_LENGTH], to[PATH_LENGTH];
	int ret, retCopy, pathLength;
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
			ret = -1; // to break from loop after logging
		} else {
			recordStats(filename, 0L, STAT_COPIED, PKG_SYS+1);
			strcpy(strLog,filename);
			ret = _findnext(&file_info);
		}
		logString(strLog,BUFFER);
	}
	// remove partially copied applications
	if (retCopy == -1) {
		strcpy(filename,path);
		strcat(filename,file_info.f_name);
		strcpy(to,INBOX_PATH);
		to[0] = 'b'; //change a:/ drive to b:/ drive
		strcat(to,NEW_PKG_SUBDIR);
		strcat(to,packageName); // to get directory name
		pathLength = strlen(to) + 1;  // to include only path + '/' 
		strcat(to,"/*.*");
		ret =_findfirst((LPSTR)to, &file_info, D_FILE);
		while (ret >= 0) {
			strcpy(filename,to);
			strcpy(filename + pathLength,file_info.f_name);
			unlink((LPSTR)filename);
			ret = _findnext(&file_info);
		}		
		to[pathLength-1] = 0;  // do not include "/*.*"
		rmdir((LPSTR)to);
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

void 
buildMyStatsCSV() {
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
	strcpy(lineout, (char *)TB_SERIAL_NUMBER_ADDR);
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

	char from[PATH_LENGTH], to[PATH_LENGTH];
	int ret;

	mkdir((LPSTR)CLI_OSTAT_DIR);	
	
	strcpy(to, CLI_OSTAT_DIR);
	
//	mkdir("a:/b/system/ostats");  // remove after testing
//	strcpy(to, "a:/b/system/ostats/");  // remove after testing with a b folder on a:

	strcat(to, (const char *)TB_SERIAL_NUMBER_ADDR);
	strcat(to, ".csv");
	
	
	strcpy(from, STAT_DIR);
	strcat(from, (const char *)TB_SERIAL_NUMBER_ADDR);
	strcat(from, ".csv");
	
	ret = fileCopy((char *)from, (char *)to);

	getStats();
//	
// Logic for which device has the most current stats for devices they both have connected to goes here
//

	strcpy(to, CLI_OSTAT_DIR);
	strcat(to, (const char *)TB_SERIAL_NUMBER_ADDR);
	strcat(to, OSTATS_EXCHG_EXT);
	
	strcpy(from, OSTAT_DIR);
	strcat(from, (const char *)TB_SERIAL_NUMBER_ADDR);
	strcat(from, OSTATS_EXCHG_EXT);

//  from a: to b:
	ret = fileCopy((char *)from, (char *)to);

}

static void
getStats(void) {
	char linein[PATH_LENGTH], from[PATH_LENGTH], to[PATH_LENGTH], *cp1, *cp2;
	char cli_serial_number[PATH_LENGTH], strLog[PATH_LENGTH];
	int ret, i, j, f;
	
	if (DEBUG_MODE)
		logString((char *)"getting connected device's stats",ASAP);
	
	strcpy(from, CLI_STAT_DIR);
//	strcpy(from, "a:/b/system/stats/");  // remove after testing with a b folder on a:

	cp1 = from + strlen(from);  // save this position	
	strcat(from, SNCSV);	
	
	f = open((LPSTR)from, O_RDONLY); 
	if(f < 0) {
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
	strcpy(cli_serial_number, (char *)cp2);

	strcat(to, ".csv");
	strcpy((char *)cp1, (char *)cp2);

	
	logString(from,BUFFER);
	logString(to,ASAP);
	
	// from b: to a:
	ret = fileCopy((char *)from, (char *)to);

//	
// Logic for which device has the most current stats for devices they both have connected to goes here
//

	strcpy(from, CLI_OSTAT_DIR);
	strcat(from, cli_serial_number);
	strcat(from, OSTATS_EXCHG_EXT);
	
	strcpy(to, OSTAT_DIR);
	strcat(to, cli_serial_number);
	strcat(to, OSTATS_EXCHG_EXT);
	
	// from b: to a:
	ret = fileCopy((char *)from, (char *)to);
	
	strcpy(strLog, "getStats ostats copy from ");
	strcat(strLog, from);
	logString(strLog, BUFFER);
	
	strcpy(strLog, "getStats ostats copy  to  ");
	strcat(strLog, to);
	logString(strLog, BUFFER); 

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
copyProfile(void) {
	char from[PATH_LENGTH], to[PATH_LENGTH];
	int ret;
	
	// copy language 
	strcpy(from,LANGUAGES_PATH);
	strcat(from,currentProfileLanguage()); // just language portion
	strcat(from,"/");	
	strcpy(to,from);
	to[0] = 'b';
	ret = cloneDir(from,to);

	// create new message list folder and copy _activeLists file
	strcpy(from,LISTS_PATH);
	strcat(from,currentProfileMessageList()); // just message list portion
	strcat(from,"/");	
	strcpy(to,from);
	to[0] = 'b';
	mkdir((LPSTR)to);
	if (DEBUG_MODE) 
		logString((char *)"copy activelist",BUFFER);
	strcat(from,LIST_MASTER);
	strcat(from,(char *)".txt");
	strcpy(to,from);
	to[0] = 'b';
	if (DEBUG_MODE) {
		logString(from,BUFFER);
		logString(to,ASAP);
	}
	ret = _copy((LPSTR)from, (LPSTR)to);
	
		strcpy(to,SYSTEM_PATH);
		to[0] = 'b';
		strcat(to,PROFILE_ORDER_FILE);
		strcat(to,".txt");	
		strcpy(from,currentProfileLanguage());
		strcat(from,",");
		strcat(from,currentProfileMessageList());
		ret = findDeleteStringFromFile((char *)NULL, to, from, 0);
		if (ret == -1)
			ret = appendStringToFile(to,from);		

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

	// move any b:/system/*.img to b:/*.img so it will be processed by processSystemFiles
	for(ret =_findfirst((LPSTR)path, &fi, D_FILE); ret >= 0; ret = _findnext(&fi)) {	
		//strcpy(to,INBOX_PATH);
		//to[0] = 'b';
		//strcat(to,SYS_UPDATE_SUBDIR);
		strcpy(to, (char *)"b:/");
		strcat(to, fi.f_name);
		strcpy(from, path);
		from[len_path] = 0;
		strcat(from, fi.f_name);
		ret = rename((LPSTR)from, (LPSTR)to);
//		ret = _copy((LPSTR)from, (LPSTR)to);
	}
}

int 
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
	
		if(!strcmp(from, STAT_DIR)) {
			continue;
		}
		if(!strcmp(from, OSTAT_DIR)) {
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
	fret += copyfilesFiltered(from, to);
	
	return(fret);
}
//
// copy all files in fromdir to todir
//
static int copyfilesFiltered(char *fromdir, char *todir)
{
	int ret, r1, len_from, len_to, fret;
	char from[PATH_LENGTH], to[PATH_LENGTH];
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
	
			if ((fi.f_size == 0) && strcmp(fi.f_name+strlen(fi.f_name)-4, ".txt")) 
				continue; // don't copy 0-size files (should prevent the volume label from being copied as a file) unless they are .txt files
					
			setLED(LED_GREEN,TRUE);
				
//			longToHexString((long)r1, (char *)dbghex, 1);
//			strcpy(dbgbuf, dbghex);
//			strcat(dbgbuf, " copy ");
//			strcat(dbgbuf, to);
//	 		logString(dbgbuf,ASAP);

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

void 
pushContentGetFeedback() {
	char bInbox[PATH_LENGTH];
	int ret;
	
	ret = setUSBHost(TRUE);
	if (ret == 0) {
		//copy this device's outbox to connected device's inbox
		if (dirExists((LPSTR)OUTBOX_PATH)) {
			strcpy(bInbox,INBOX_PATH);
			bInbox[0] = 'b'; // changes a: to b:
			copyMovedir(OUTBOX_PATH, bInbox);
		} else
			logString((char *)"no outbox folder",ASAP);
		getStats();  //grab all connected device's stats
		playBip();
		getAudioFeedback();  //grab all connected device's messages in User Feedback category
		playDing();
		setUSBHost(FALSE);
	} else
		playBips(3);
}
