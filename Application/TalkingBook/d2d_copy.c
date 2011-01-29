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
			retCopy = read(rHandle, (UINT32)&tmpstats << 1, sizeof(tmpstats));
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
			retCopy = read(rHandle, (UINT32)&tmpstats << 1, sizeof(tmpstats));
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


