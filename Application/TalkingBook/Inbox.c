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
#include "Include/SD_reprog.h"
#include "Include/Inbox.h"

struct newContent {
	char newAudioFileCat  [FILE_LENGTH];
	char newAudioFileName [FILE_LENGTH];
	char newAudioDirCat   [FILE_LENGTH];
	char newAudioDirName  [FILE_LENGTH];
};

extern APP_IRAM char logBuffer[LOG_BUFFER_SIZE];
extern APP_IRAM int idxLogBuffer;
static int processA18(struct f_info *, struct newContent *);
static int processDir(char *, struct newContent *);
//static int copyCWD(char *);
static int updateCategory(char *, char *, char);
static void processSystemFiles(void);
static struct newContent processNewPackages(void);
static void queueNewPackage(struct newContent);

#define D_NOTDIR (D_FILE | D_RDONLY | D_HIDDEN | D_SYSTEM | D_ARCHIVE)

//  called when leaving USB mode to check for files copied from other device into a:\Inbox
//
extern void
processInbox(void) {
	// TODO: handle bad received files that might look like 0-byte files (This can lock the device up if played.)
	// TODO: handle duplicate packages and unique name issue
	// TODO: if copying 1+ files and all of them have the same name as existing files (rename returns -1), then
	//       return a message saying nothing new was copied.  If just one thing was new, that is the one that should be played.
	
	struct newContent nc;	
	char strLog[PATH_LENGTH];
	
	playBip();
	setLED(LED_RED,TRUE);

	strcpy(strLog, "processInbox");	
	logString(strLog, ASAP);
	nc = processNewPackages();
	processSystemFiles(); //copy system files, including firmware
	queueNewPackage(nc);
	setLED(LED_RED,FALSE);
	checkInactivity(TRUE);
}

static void 
queueNewPackage(struct newContent nc) {
	char fbuf[PATH_LENGTH];
	char *strList;

	// Set up current list to position at one of the newly copied messages and queue it up to play
	// This point is only reached if there was not a successful copy made of new firmware
	if (nc.newAudioFileCat[0])
		strcpy(fbuf,nc.newAudioFileCat);
	else if (nc.newAudioDirCat[0])
		strcpy(fbuf,nc.newAudioDirCat);
	else
		fbuf[0] = 0;
	//logString(nc.newAudioFileCat,BUFFER);
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
		insertSound(getListFile(fbuf),NULL,TRUE);	
		context.queuedPackageNameIndex = replaceStack(strList,&pkgSystem);
	} 
}

static struct newContent
processNewPackages(void) {
	struct f_info file_info;
	char strLog[PATH_LENGTH], savecwd[PATH_LENGTH];
	char fbuf[PATH_LENGTH], strNewPkgPath[PATH_LENGTH], fname[FILE_LENGTH];
	char *strList;
	int ret, r1, len, fret;
	struct newContent nc;	

	fret = 0;
	nc.newAudioFileCat[0] = nc.newAudioFileName[0] = 0;
	nc.newAudioDirCat [0] = nc.newAudioDirName [0] = 0;
		
//  check for *.a18 files in Inbox
	strcpy(strNewPkgPath, INBOX_PATH);
	strcat(strNewPkgPath, NEW_PKG_SUBDIR);
	len = strlen(strNewPkgPath);
	if(len > 1) {
		strNewPkgPath[len-1] = 0;
		ret = fileExists(strNewPkgPath);
		if(ret == 0) {
			mkdir(strNewPkgPath);
			strcpy(strLog, "no new packages");	
			logString(strLog, ASAP);
			return nc;
		}	
		ret = getcwd(savecwd , sizeof(savecwd) - 1 );
		ret = chdir(strNewPkgPath);
		strcat(strNewPkgPath,"\\");  // add back what was removed above
	//  process *.a18 files in inbox - category in file name after # OR IN *.txt
	//  this case may never happen
	//
		strcpy(fbuf, strNewPkgPath);
		strcat(fbuf, "*");
		strcat(fbuf,AUDIO_FILE_EXT);
		ret =_findfirst(fbuf, &file_info, D_ALL);
		while (ret >= 0) {
			strcpy(strLog, file_info.f_name);
			//logString(strLog,BUFFER);		
			fret += processA18(&file_info, &nc);
			ret = unlink(file_info.f_name);					
			ret = _findnext(&file_info);
		}
	//
	//  now process directories
		strcpy(fbuf, strNewPkgPath);
		strcat(fbuf, "*.*");
		
		ret =_findfirst(fbuf, &file_info, D_DIR);
		
		for (; ret >= 0; ret = _findnext(&file_info)) {
			if(file_info.f_attrib & D_DIR) {
				if(file_info.f_name[0] == '.')
					continue;
				
				strcpy(fname, file_info.f_name);	
				//logString(fname,BUFFER);		
				fret += processDir(fname, &nc);
				  // RHM: something I do below makes this necessary
				  //      upon return after _findnext returns -1 even if there are more dirs
				ret = _findfirst(fbuf, &file_info, D_ALL);
			}	
		}
		
		ret = chdir((LPSTR)savecwd);
	}	// end of if: length of strNewPkgPath must be > 1
	return nc;
}

static void 
processSystemFiles(void) {
	// copy any system updates from SYS_UPDATE_SUBDIR to a:\ 
	long l;
	char strSysUpdatePath[PATH_LENGTH];
	
	strcpy(strSysUpdatePath,INBOX_PATH);
	strcat(strSysUpdatePath,SYS_UPDATE_SUBDIR);
	l = (long)copydir((char *)strSysUpdatePath, (char *)"a:\\");  // returns # of items copied
	if (l > 1)  // 1 is for the SYS_UPDATE_SUBDIR
		resetSystem(); // reset to begin new firmware reprogramming or to reload new config/system control
}


static int 
processA18(struct f_info *fip, struct newContent *pNC) {
	char strLog[80], savecwd[80];
	char buffer[READ_LENGTH+1], *line, tmpbuf[READ_LENGTH+1];
	char fnbase[80], category[8], subcategory[8];
	int ret, len, len_fnbase, i, catidx, subidx, cat_base, fret;

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
			if(isupper(fip->f_name[i])) {
				category[catidx++] = fip->f_name[i];
				continue;
			}
			if(islower(fip->f_name[i]))
				subcategory[subidx++] = fip->f_name[i]; 
		}
		category[catidx] = 0;
		subcategory[subidx] = 0;
	} else {
		len_fnbase = strIndex(fip->f_name, '.');
		strcpy(fnbase, fip->f_name);
		fnbase[len_fnbase] = 0;
		strcat(fnbase, ".txt");
		ret = fileExists(fnbase);
		if(ret) {	//open txt file, read category
			getLine(-1,0);
			ret = tbOpen(fnbase,O_RDONLY);
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
			unlink(fnbase);		
		} else { // .a18 file without category info
			strcpy(category, "OTHER");
			//or
			// strcpy(category, "O");
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
			ret = rename(tmpbuf, buffer);
//			logString((char *)"Rename from/to",BUFFER);
//			logString(tmpbuf,BUFFER);
//			logString(buffer,ASAP);
		} while (ret);
	} else {
		strcat(buffer, fip->f_name);
		ret = rename(tmpbuf, buffer);
//			logString((char *)"Rename from/to",BUFFER);
//			logString(tmpbuf,BUFFER);
//			logString(buffer,ASAP);
		if(ret) {
			logString((char *)"rename failed",ASAP);
			unlink(tmpbuf);	// rename failed, remove from inbox anyway
		} else {
			fret++;
		}
	}
			
	if(pNC->newAudioFileCat[0] == 0) {
		strcpy(pNC->newAudioFileCat, category);
		strcpy(pNC->newAudioFileName, fnbase);
	}
			
// TODO - currently doing nothing with subcategory
	
	ret = updateCategory(category, fnbase, (char *)NULL);
	
	return(fret);

}

static int 
processDir(char *dirname, struct newContent *pNC) {
	struct f_info file_info;
	char strLog[80], savecwd[80];
	char buffer[READ_LENGTH+1], tempbuf[80];
	char fnbase[80], category[8], subcategory[8];
	int ret, fret, catidx, subidx, len_fnbase, i, cat_base;

	ret = getcwd(savecwd , sizeof(savecwd) - 1 );
	ret = chdir(dirname);
	
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
			ret = rename(buffer,tempbuf);
		}
	} else {
		strcpy(tempbuf,USER_PATH);
		strcat(tempbuf,dirname);
		strcpy(fnbase, dirname);
		ret = rename(buffer,tempbuf);
	}
		
	if(catidx == 0)
		strcpy(category, "OTHER");  // "O"
	
// TODO: - currently doing nothing with subcategory
	
	ret = updateCategory(category, fnbase, APP_PKG_CHAR);
	
	if(pNC->newAudioDirCat[0] == 0) {
		strcpy(pNC->newAudioDirCat, category);
		strcat(pNC->newAudioDirName, fnbase);
	}

		
	ret = chdir(savecwd);	
	ret = rmdir(dirname);
	
	return (fret);
}

static int 
updateCategory(char *category, char *fnbase, char prefix) {
	char buffer[80], tmpbuf[80];
	int ret;
	
// be sure category is in master-list.txt
	strcpy(buffer, LIST_MASTER);
	strcpy(tmpbuf,category);
	// Only add category entry if doesn't already exist.
	// Checking for existence withuot deleting and appending preserves category order.
	ret = findDeleteStringFromFile((char *)NULL, buffer, tmpbuf, 0);
	if (ret == -1) // not found in file
		ret = appendStringToFile(buffer, tmpbuf); 

// delete new file name or dir name from {category}.txt, 
// then add it (only want it to appear once)	
//   int findDeleteStringFromFile(char *path, char *filename, char* string, BOOL shouldDelete)
	strcpy(buffer, LIST_PATH);
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
	int ret;
	long time;
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
	char from[PATH_LENGTH], to[PATH_LENGTH], lastdir[FILE_LENGTH];

	struct f_info fi;
	
	fret = 0;
	
	strcpy(from, fromdir);
	len_from = strlen(from);
	
	if(from[len_from-1] != '\\') {
		strcat(from, "\\");
		len_from++;
	}
	strcat(from, "*");
	
	strcpy(to, todir);
	len_to = strlen(to);
	ret = mkdir(to);	// just to be safe
	if(to[len_to-1] != '\\') {
		strcat(to, "\\");
		len_to++;
	}
	ret =_findfirst(from, &fi, D_DIR);
	from[len_from] = 0;
	lastdir[0] = 0;
	
	for( ; ret >= 0 ; ret = _findnext(&fi)) {
		if(! (fi.f_attrib & D_DIR))
			continue;
		
		if(fi.f_name[0]=='.')
			continue;
		
		if(lastdir[0]) {
			if(!strcmp(fi.f_name, lastdir)) {
				lastdir[0] = 0;      //should not be necessary
			}
			continue;
		}
		
		strcpy(lastdir, fi.f_name);  // should not be necessary
		
		from[len_from] = 0;
		to[len_to]= 0;
				
		strcat(from, fi.f_name);
		strcat(to, fi.f_name);
		
		r1 = mkdir(to);
		fret += copydir (from, to);
		ret = rmdir(from);
		
		from[len_from] = 0;
		strcat(from, "*");
		ret =_findfirst(from, &fi, D_ALL);  // should not be necessary
		
		fret++;
	}
	
	from[len_from] = 0;
	to[len_to]= 0;
	fret += copyfiles(from, to);
	strcpy(lastdir,LIST_PATH+2);
	if (len = strlen(lastdir))
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
	
	if(from[len_from-1] != '\\') {
		strcat(from, "\\");
		len_from++;
	}
	strcat(from, "*.*");
	
	strcpy(to, todir);
	len_to = strlen(to);
//	mkdir(to);	// just to be safe
	if(to[len_to-1] != '\\') {
		strcat(to, "\\");
		len_to++;
	}
			
	ret =_findfirst(from, &fi, D_FILE);
	while(ret >= 0) {
		if(fi.f_name[0] != '.') {
			from[len_from] = 0;
			to[len_to]= 0;
			strcat(from, fi.f_name);
			strcat(to, fi.f_name);
			if((lower(from[0]) == 'a') && (lower(to[0]) == 'a')) {
				unlink(to);
				r1 = rename(from, to);
			} else {
				setLED(LED_GREEN,FALSE);
				setLED(LED_RED,TRUE);
				r1 = _copy(from, to);
				wait (500);
				setLED(LED_RED,FALSE);
				if (r1 != -1) {
					setLED(LED_GREEN,TRUE);
					wait(500);
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
