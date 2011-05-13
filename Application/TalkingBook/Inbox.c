// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include <string.h>
#include <ctype.h>
#include "Include/talkingbook.h"
#include "Include/device.h"
#include "Include/files.h"
#include "Include/lists.h"
#include "Include/containers.h"
#include "Include/audio.h"
#include "Include/pkg_ops.h"
#include "Include/SD_reprog.h"
#include "Include/sys_counters.h"
#include "Include/mainLoop.h"
#include "Include/Inbox.h"
#include "Include/metadata.h"

struct newContent {
	char newAudioFileCat  [FILE_LENGTH];
	char newAudioFileName [FILE_LENGTH];
	char newAudioDirCat   [FILE_LENGTH];
	char newAudioDirName  [FILE_LENGTH];
};

extern APP_IRAM char logBuffer[LOG_BUFFER_SIZE];
extern APP_IRAM int idxLogBuffer;

extern int setUSBHost(BOOL);

static int processA18(struct f_info *, struct newContent *);
static int processDir(char *, struct newContent *);
//static int copyCWD(char *);
static int updateCategory(char *, char *, char);
static void processSystemFiles(void);
static void processNewPackages(struct newContent *);
static void queueNewPackage(struct newContent *);
static int copydir(char *, char *);
static int copyfiles(char *, char *);

#define D_NOTDIR (D_FILE | D_RDONLY | D_HIDDEN | D_SYSTEM | D_ARCHIVE)

__attribute__((section(".code"))) char *categories[] = {"OTHER", "AGRIC", "HEALTH", "EDU", "STORIES", "BUSINESS", "GOV", "MUSIC", "DIARY" };

//  called when leaving USB mode to check for files copied from other device into a:\Inbox
//
extern void
processInbox(void) {
	// TODO: handle bad received files that might look like 0-byte files (This can lock the device up if played.)
	// TODO: handle duplicate packages and unique name issue
	// TODO: if copying 1+ files and all of them have the same name as existing files (rename returns -1), then
	//       return a message saying nothing new was copied.  If just one thing was new, that is the one that should be played.
	
	struct newContent nc;	
	
	stop();
	playBip();
	setLED(LED_RED,TRUE);
	writeVersionToDisk();  // make sure the version file wasn't deleted during USB device time

	processNewPackages(&nc);
	processSystemFiles(); //copy system files, including firmware
	queueNewPackage(&nc);
	setLED(LED_RED,FALSE);
	checkInactivity(TRUE);
}

static void 
queueNewPackage(struct newContent *ncp) {
	char fbuf[PATH_LENGTH];
	char *strList;

	// Set up current list to position at one of the newly copied messages and queue it up to play
	// This point is only reached if there was not a successful copy made of new firmware
	if (ncp->newAudioFileCat[0])
		strcpy(fbuf,ncp->newAudioFileCat);
	else if (ncp->newAudioDirCat[0])
		strcpy(fbuf,ncp->newAudioDirCat);
	else
		fbuf[0] = 0;
	//logString(ncp->newAudioFileCat,BUFFER);
	//flushLog();
	if (fbuf[0]) {
		context.package = &pkgSystem; // in case paused on content
		pkgSystem.lists[0].currentFilePosition = -1;
		strList = getCurrentList(&pkgSystem.lists[0]);
		while (strcmp(strList,fbuf)) {
			strList = getPreviousList(&pkgSystem.lists[0]);
		}
		pkgSystem.lists[1].currentFilePosition = -1;
		strList = getCurrentList(&pkgSystem.lists[1]);
		if (strList[0] == APP_PKG_CHAR) {
			strList++;
			context.queuedPackageType = PKG_APP;  
		} else
			context.queuedPackageType = PKG_MSG;
		insertSound(getTempFileFromName(fbuf,0),NULL,TRUE);	
		context.queuedPackageNameIndex = replaceStack(strList,&pkgSystem);
	} 
}

static void
processNewPackages(struct newContent *ncp) {
	struct f_info file_info;
	char strLog[PATH_LENGTH], savecwd[PATH_LENGTH];
	char fbuf[PATH_LENGTH], strNewPkgPath[PATH_LENGTH], fname[FILE_LENGTH];
	int ret, len, fret;
//	struct newContent nc;	

	fret = 0;
	ncp->newAudioFileCat[0] = ncp->newAudioFileName[0] = 0;
	ncp->newAudioDirCat [0] = ncp->newAudioDirName [0] = 0;
		
//  check for *.a18 files in Inbox
	strcpy(strNewPkgPath, INBOX_PATH);
	strcat(strNewPkgPath, NEW_PKG_SUBDIR);
	len = strlen(strNewPkgPath);
	if(len > 1) {
		strNewPkgPath[len-1] = 0;
		ret = fileExists((LPSTR)strNewPkgPath);
		if(ret == 0) {
			mkdir((LPSTR)strNewPkgPath);
			strcpy(strLog, "no new packages");	
			logString(strLog, ASAP);
			return;
		}	
		ret = getcwd((LPSTR)savecwd , sizeof(savecwd) - 1 );
		ret = chdir((LPSTR)strNewPkgPath);
		strcat(strNewPkgPath,"/");  // add back what was removed above
	//  process *.a18 files in inbox - category in file name after # OR IN *.txt
	//  this case may never happen
	//
		strcpy(fbuf, strNewPkgPath);
		strcat(fbuf, "*");
		strcat(fbuf,AUDIO_FILE_EXT);
		ret =_findfirst((LPSTR)fbuf, &file_info, D_ALL);
		while (ret >= 0) {
			strcpy(strLog, file_info.f_name);
			//logString(strLog,BUFFER);		
			fret += processA18(&file_info, ncp);
			ret = unlink((LPSTR)file_info.f_name);					
			ret = _findnext(&file_info);
		}
	//
	//  now process directories
		strcpy(fbuf, strNewPkgPath);
		strcat(fbuf, "*.*");
		
		ret =_findfirst((LPSTR)fbuf, &file_info, D_DIR);
		
		for (; ret >= 0; ret = _findnext(&file_info)) {
			if(file_info.f_attrib & D_DIR) {
				if(file_info.f_name[0] == '.')
					continue;
				
				strcpy(fname, file_info.f_name);	
				//logString(fname,BUFFER);		
				fret += processDir(fname, ncp);
				  // RHM: something I do below makes this necessary
				  //      upon return after _findnext returns -1 even if there are more dirs
				ret = _findfirst((LPSTR)fbuf, &file_info, D_ALL);
			}	
		}
		
		ret = chdir((LPSTR)savecwd);
	}	// end of if: length of strNewPkgPath must be > 1
	return;
}

static void 
processSystemFiles(void) {
	// copy any system updates from SYS_UPDATE_SUBDIR to a:\ 
	long l;
	int ret;
	char strSysUpdatePath[PATH_LENGTH];
	
	strcpy(strSysUpdatePath,INBOX_PATH);
	strcat(strSysUpdatePath,SYS_UPDATE_SUBDIR);
	l = (long)copydir((char *)strSysUpdatePath, (char *)"a:/");  // returns # of items copied
	ret = check_new_sd_flash(strSysUpdatePath);  //strSysUpdatePath may be changed
	if ((l > 1) || (ret != 0)) { // 1 is for the SYS_UPDATE_SUBDIR
		if (PLEASE_WAIT_IDX) 
			insertSound(&pkgSystem.files[PLEASE_WAIT_IDX],NULL,TRUE); 
		resetSystem(); // reset to begin new firmware reprogramming or to reload new config/system control
	}
}


static int 
processA18(struct f_info *fip, struct newContent *pNC) {
	int getMetaCat(char *filename, char *category);
	char buffer[READ_LENGTH+1], *line, tmpbuf[READ_LENGTH+1];
	char fnbase[80], category[40], subcategory[40];
	int ret, len_fnbase, i, catidx, subidx, cat_base, fret;

	category[0] = subcategory[0] = 0;
	cat_base = strIndex(fip->f_name, CATEGORY_DELIM);
	catidx = subidx = fret = 0;
	if(cat_base >= 1) {	// category info in filename
		strcpy(fnbase, fip->f_name);
//	Commenting line below since we want to carry through category info
//		fnbase[cat_base] = 0; 
		len_fnbase = strIndex(fip->f_name, '.');
		for(i=cat_base; ; i++ ) {
			if(fip->f_name[i] == '.')
				break;
			// categories must be caps; subcategories must be lower; each can have digits but not in first position
			if(isupper(fip->f_name[i]) || (catidx && isdigit(fip->f_name[i])) ) {
				category[catidx++] = fip->f_name[i];
				continue;
			}
			if(islower(fip->f_name[i])|| (catidx && isdigit(fip->f_name[i])))
				subcategory[subidx++] = fip->f_name[i]; 
		}
		category[catidx] = 0;
		subcategory[subidx] = 0;
	} else {
		len_fnbase = strIndex(fip->f_name, '.');
		strcpy(fnbase, fip->f_name);
		fnbase[len_fnbase] = 0;
		strcat(fnbase, ".txt");
		ret = fileExists((LPSTR)fnbase);
		if(ret) {	//open txt file, read category
			getLine(-1,0);
			ret = tbOpen((LPSTR)fnbase,O_RDONLY);
			line = getLine(ret, buffer);
			strncpy(category, line, sizeof(category)-1);
			close(ret);	
			for(i=0; i < sizeof(category)-1; i++) {
				if(category[i] <= 0x20) {
					category[i] = 0;
					break;
				}
			}
			category[sizeof(category)-1] = 0;
			unlink((LPSTR)fnbase);		
		} else { // .a18 file without category info
			if(!getMetaCat(fip->f_name, category))  {
				strcpy(category, "OTHER");
			}
		}
	}

	strcpy(buffer, USER_PATH);
	fnbase[len_fnbase] = 0;

	strcpy(tmpbuf,INBOX_PATH);
	strcat(tmpbuf,NEW_PKG_SUBDIR);
	strcat(tmpbuf,fip->f_name);

// TODO: should some other test be applied here??
	if(0 == strncmp(fnbase,PKG_NUM_PREFIX,strlen(PKG_NUM_PREFIX))) {
		ret = 1;
		fret++;
		do {
			strcpy(buffer,USER_PATH);
			getrevdPkgNumber(fnbase,TRUE);
			strcat(fnbase,CATEGORY_DELIM_STR);
			strcat(fnbase, category);
			strcat(fnbase, subcategory);
			strcat(buffer, fnbase);
			strcat(buffer,AUDIO_FILE_EXT);
			ret = rename((LPSTR)tmpbuf, (LPSTR)buffer);
//			logString((char *)"Rename from/to",BUFFER);
//			logString(tmpbuf,BUFFER);
//			logString(buffer,ASAP);
		} while (ret);
	} else {
		strcat(buffer, fip->f_name);
		ret = rename((LPSTR)tmpbuf, (LPSTR)buffer);
//			logString((char *)"Rename from/to",BUFFER);
//			logString(tmpbuf,BUFFER);
//			logString(buffer,ASAP);
		if(ret) {
			logString((char *)tmpbuf,BUFFER);
			logString((char *)buffer,BUFFER);
			logString((char *)"rename failed",ASAP);
			unlink((LPSTR)tmpbuf);	// rename failed, remove from inbox anyway
			return(fret);
		} else {
			fret++;
		}
	}
			
	if(pNC->newAudioFileCat[0] == 0) {
		strcpy(pNC->newAudioFileCat, category);
		strcpy(pNC->newAudioFileName, fnbase);
	}
			
// TODO - currently doing nothing with subcategory
	
	ret = updateCategory((char *)category, (char *)fnbase, 0);
	
	return(fret);

}

static int 
processDir(char *dirname, struct newContent *pNC) {
	char savecwd[80];
	char buffer[READ_LENGTH+1], tempbuf[80];
	char fnbase[80], category[40], subcategory[40];
	int ret, fret, catidx, subidx, len_fnbase, i, cat_base;

	ret = getcwd((LPSTR)savecwd , sizeof(savecwd) - 1 );
	ret = chdir((LPSTR)dirname);
	
	category[0] = subcategory[0] = 0;
	catidx = subidx = 0;
	
	fret = 1;
	
	cat_base = strIndex(dirname, CATEGORY_DELIM);

	if(cat_base >= 1) {	// category info in filename
		strcpy(fnbase, dirname);
		//fnbase[ret] = 0;
		len_fnbase = cat_base;
		for(i=cat_base+1; ; i++ ) {
			if(dirname[i] == '.')
				break;
			if(isupper(dirname[i])) {
				category[catidx++] = dirname[i];
				continue;
			}
			if(islower(dirname[i])) {
				subcategory[subidx++] = dirname[i];
				continue;
			}
			break; 
		}
	category[catidx] = 0;
	subcategory[subidx] = 0;
	}	
	//copyCWD(USER_PATH);
	strcpy(buffer,INBOX_PATH);
	strcat(buffer,NEW_PKG_SUBDIR);
	strcat(buffer,dirname);
	
	if(0 == strncmp(dirname,PKG_NUM_PREFIX,strlen(PKG_NUM_PREFIX))) {
		ret = 1;
		while(ret) {
			strcpy(tempbuf,USER_PATH);
//			getrevdPkgNumber(tempbuf+strlen(tempbuf),TRUE);
			getrevdPkgNumber(fnbase,TRUE);
			strcat(fnbase, dirname+cat_base);
			strcat(tempbuf, fnbase);
			ret = rename((LPSTR)buffer,(LPSTR)tempbuf);
		}
	} else {
		strcpy(tempbuf,USER_PATH);
		strcat(tempbuf,dirname);
		strcpy(fnbase, dirname);
		ret = rename((LPSTR)buffer,(LPSTR)tempbuf);
		if (ret == -1) {
			// assume the directory already exists
			deleteAllFiles(buffer);
			rmdir((LPSTR)buffer);			
		}
	}
		
	if(catidx == 0)
		strcpy(category, "OTHER");  // "O"
	
// TODO: - currently doing nothing with subcategory

	if (ret != -1) // dont bother if move didnt happen, which means it probably already exists	
		ret = updateCategory(category, fnbase, APP_PKG_CHAR);
	
	if(pNC->newAudioDirCat[0] == 0) {
		strcpy(pNC->newAudioDirCat, category);
		strcat(pNC->newAudioDirName, fnbase);
	}

		
	ret = chdir((LPSTR)savecwd);	
	ret = rmdir((LPSTR)dirname);
	
	return (fret);
}

static int 
updateCategory(char *category, char *fnbase, char prefix) {
	char buffer[80], tmpbuf[80], path[PATH_LENGTH];
	int ret;
	
// be sure category is in master-list.txt
	cpyListPath(path,LIST_MASTER);
	strcpy(buffer,path);
	strcat(buffer,(char *)LIST_MASTER);
	strcat(buffer,(char *)".txt");
	strcpy(tmpbuf,category);
	// Only add category entry if doesn't already exist.
	// Checking for existence without deleting and appending preserves category order.
	ret = findDeleteStringFromFile((char *)NULL, buffer, tmpbuf, 0);
	if (ret == -1) // not found in file
		ret = appendStringToFile(buffer, tmpbuf); 

// delete new file name or dir name from {category}.txt, 
// then add it (only want it to appear once)	
//   int findDeleteStringFromFile(char *path, char *filename, char* string, BOOL shouldDelete)
	cpyListPath(path,category);
	strcpy(buffer, path);
	strcat(buffer, category);
	strcat(buffer,".txt");
	
	if(!fileExists((LPSTR)buffer)) {  // if category.txt does not exist create it
		ret = open((LPSTR)buffer,O_CREAT|O_RDWR);
		close(ret);
	}
	
	if(prefix) {
		tmpbuf[0] = prefix;
		strcpy(tmpbuf+1, fnbase);
	} else {
		strcpy(tmpbuf,fnbase);
	}
// This checks if entry already exists and adds a new line only if it does not.
	ret = findDeleteStringFromFile((char *)NULL, buffer, tmpbuf, 0);
		
//  append the basename to the proper .txt file in lists
	if (ret == -1)
		ret = insertStringInFile(buffer,tmpbuf,0);
	return ret;
}

extern void 
copyOutbox() {
	char bInbox[PATH_LENGTH];
	
	setUSBHost(TRUE);
	strcpy(bInbox,INBOX_PATH);
	bInbox[0] = 'b'; // changes a: to b:
	
	copydir(OUTBOX_PATH, bInbox);
	setUSBHost(FALSE);
}

static int 
copydir(char *fromdir, char *todir) {
// 	copy directory tree below fromdir (all subdirectories and files at all levels)
	int ret, r1, len_from, len_to, len, fret;
	char from[PATH_LENGTH], fromfind[PATH_LENGTH], to[PATH_LENGTH], lastdir[FILE_LENGTH];

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
	lastdir[0] = 0;
	
	while (ret >= 0) {
		if(! (fi.f_attrib & D_DIR)) {
			ret = _findnext(&fi);
			continue;
		}
	
		if(fi.f_name[0]=='.') {
			ret = _findnext(&fi);
			continue;
		}
		from[len_from] = 0;
		to[len_to]= 0;
				
		strcat(from, fi.f_name);
		strcat(to, fi.f_name);
		
		r1 = mkdir((LPSTR)to);
		fret += copydir (from, to);
		ret = rmdir((LPSTR)from);
				
		fret++;
		ret =_findfirst((LPSTR)fromfind, &fi, D_DIR);  //necessary to reset after rmdir? 
	};
	
	from[len_from] = 0;
	to[len_to]= 0;
	fret += copyfiles(from, to);
	cpyTopicPath(from);
	strcpy(lastdir,from+2);
	if ((len = strlen(lastdir)))
		lastdir[len-1] = 0; // remove last '\'
	if (strstr(fromdir,lastdir))
		fret = 0; // prevents system reset if only copying list files
	
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
	while(ret >= 0) {
		if(fi.f_name[0] != '.') {
			from[len_from] = 0;
			to[len_to]= 0;
			strcat(from, fi.f_name);
			strcat(to, fi.f_name);
			if((lower(from[0]) == 'a') && (lower(to[0]) == 'a')) {
				unlink((LPSTR)to);
				r1 = rename((LPSTR)from, (LPSTR)to);
			} else {
				setLED(LED_GREEN,FALSE);
				setLED(LED_RED,TRUE);
				r1 = _copy((LPSTR)from, (LPSTR)to);
// speed up 				wait (500);
				setLED(LED_RED,FALSE);
				if (r1 != -1) {
					setLED(LED_GREEN,TRUE);
// speed up					wait(500);
				}
			}
//			logString((char *)"FROM/TO:",BUFFER);
//			logString(from,BUFFER);
//			logString(to,ASAP);
		}
		ret = _findnext(&fi);
		fret++;
	}
	return(fret);
}
int getMetaCat(char *filename, char *category)
{
	int ret = 0;
	long wrk, numfields, fieldlen;
	unsigned int nret, fd;
	int fid;
	unsigned char nfv;
	unsigned char buf[128];
	int i, j;

	fd = open((char *)filename, 0);
	if(fd < 0) {
		return(ret);
	}
	nret = read(fd, (unsigned long)&wrk <<1, 4);
	wrk = lseek(fd, wrk + 4, SEEK_SET);
	nret = read(fd, (unsigned long)&wrk << 1, 4);
	//printf("meta data version=%ld\n", wrk);
	if(wrk == 0 || wrk > META_CURRENT_VERSION) {
		return(ret);
	}
	nret = read(fd, (unsigned long)&numfields << 1, 4);
	//printf("num fields=%ld\n", numfields);

	for(i=0; i<(int)numfields; i++) {
		nret = read(fd, (unsigned long)&fid << 1, 2);
		//printf("\n  field id=%d\n",fid);
		nret = read(fd, (unsigned long)&fieldlen << 1, 4);
		//printf("    filed length=%d\n", fieldlen);
		nret = read(fd, (unsigned long)&nfv << 1, 1);
		nfv &= 0xff;
		//printf("    num field values=%d\n", nfv);
		for(j=0; j<nfv; j++) {
			short fl;
			nret = read(fd, (unsigned long)&fl << 1, 2);
			//printf("    field value length[%d]=%d\n",j,fl);
			nret = read(fd, (unsigned long)buf << 1, fl);
			buf[0] &= 0xff;
			//printf("    field value[%d]=",j);
/*			for(k=0; k<fl; k++) {
				printf("0x%.2x ", buf[k]);
			}
			printf("\n");
*/
			//printf("'%s'",buf);
			if(fid == 0) { // categories
				unsigned int m = buf[0] - '0';
				if((m >= 0 && m < 9)) {
					strcpy(category, categories[m]);
					ret = 1;
					goto done;
				} else {
					strcpy(category, categories[0]);
					ret = 1;
					goto done;
				}
			}
		}
	}
done:
	close(fd);
	return(ret);
}
