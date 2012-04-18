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
#include "Include/d2d_copy.h"
#include "Include/startup.h"
#include "Include/metadata.h"
#include "Include/Inbox.h"

struct newContent {
	// LENGTH + 1 allows for LENGTH chars + '/0'
	char newAudioCategory  [FILE_LENGTH+1];
	char newAudioName [FILE_LENGTH+1];
	char newAudioLanguage [MAX_LANGUAGE_CODE_LENGTH+1];
	int isNew;
};

extern APP_IRAM char logBuffer[LOG_BUFFER_SIZE];
extern APP_IRAM int idxLogBuffer;

extern int setUSBHost(BOOL);

static int processA18(struct f_info *, struct newContent *);
static int processDir(char *, struct newContent *);
//static int copyCWD(char *);
static int newUpdateCategory(struct newContent *, int);
static void processSystemFiles(void);
static void processNewPackages(struct newContent *);
static void queueNewPackage(struct newContent *);
static int checkRevisions(int, int);
static int getMetaCat(char *, char *);

char Lang[MAX_LANGUAGE_CODE_LENGTH] = {0};
char *MLp = (unsigned int *) 0;
int  nMLp = 0;

#define D_NOTDIR (D_FILE | D_RDONLY | D_HIDDEN | D_SYSTEM | D_ARCHIVE)
/*
__attribute__((section(".code"))) char *categories[] = {"OTHER", "AGRIC", "HEALTH", "EDU", "STORY", "BIZ", "GOV", "MUSIC", "DIARY", "USERS" };
*/
//  called when leaving USB mode to check for files copied from other device into a:\Inbox
//
extern void
processInbox(void) {
	// TODO: handle bad received files that might look like 0-byte files (This can lock the device up if played.)
	// TODO: handle duplicate packages and unique name issue
	// TODO: if copying 1+ files and all of them have the same name as existing files (rename returns -1), then
	//       return a message saying nothing new was copied.  If just one thing was new, that is the one that should be played.
	
	struct newContent nc;	
	
	stop(); // if audio was paused, this prevents it from resuming during inbox processing, which can lead to file corruption
	setLED(LED_RED,TRUE);
	processSystemFiles(); //copy system files, including firmware
	processNewPackages(&nc);
	queueNewPackage(&nc);
	setLED(LED_RED,FALSE);
	checkInactivity(TRUE);
}

static void 
queueNewPackage(struct newContent *ncp) {
	char fbuf[PATH_LENGTH];
	char system_language[FILE_LENGTH];
	char *strList;

	// First, be sure that the queued package is of the same language as the current system.
	// Also, be sure that the queued message is new (wasn't already on the device).
	// If not, then don't queue it up.
	strcpy(system_language,&pkgSystem.strHeapStack[pkgSystem.idxLanguageCode]);
	if (!ncp->isNew || strcmp(system_language,ncp->newAudioLanguage))
		return;
	
	// Set up current list to position at one of the newly copied messages and queue it up to play
	// This point is only reached if there was not a successful copy made of new firmware
	if (ncp->newAudioCategory[0])
		strcpy(fbuf,ncp->newAudioCategory);
	else
		fbuf[0] = 0;
	logString((char *)"queued category:",BUFFER,LOG_DETAIL);
	logString(ncp->newAudioCategory,BUFFER,LOG_DETAIL);

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
	struct newContent lastAddedNewContent = {0};
	struct f_info file_info;
	char strLog[PATH_LENGTH], savecwd[PATH_LENGTH];
	char fbuf[PATH_LENGTH], strNewPkgPath[PATH_LENGTH], strBadImports[PATH_LENGTH];
	int ret, len, fret, wait;
	MLENTRY mla[MAX_ML_ENTRIES]= { 0 };
	Lang[0] = 0;
	MLp = &mla[0];
	nMLp = 0;

//	struct newContent nc;	

	fret = 0;
	ncp->newAudioCategory[0] = ncp->newAudioName[0] = 0;
		
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
			logString(strLog, ASAP,LOG_DETAIL);
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
			playBip();
			//strcpy(strLog, file_info.f_name);
			//logString(strLog,BUFFER);		
			if (file_info.f_size!= 0 && file_info.f_name[0] != ' ') { 
				// 0 KB file and filenames beginning with a space are a problem to open
				if (!wait && PLEASE_WAIT_IDX && context.package) {  // prevents trying to insert this sound before config & control files are loaded.
					insertSound(&pkgSystem.files[PLEASE_WAIT_IDX],NULL,TRUE); 
					wait = 1; // so msg isn't repeated
				}
				fret += processA18(&file_info, ncp);
				if (!lastAddedNewContent.isNew && ncp->isNew) {
					// This latest item was new and we haven't saved at least one new content yet.
					// So save this to have at least something new to queue up after finished processing.
					lastAddedNewContent.isNew = 1;
					strcpy(lastAddedNewContent.newAudioCategory,ncp->newAudioCategory);
					strcpy(lastAddedNewContent.newAudioLanguage,ncp->newAudioLanguage);
					strcpy(lastAddedNewContent.newAudioName,ncp->newAudioName);
				}
				ret = unlink((LPSTR)file_info.f_name);
			} else {
				// move files that can't be imported into a bad-import folder
				// todo: use this for other issues, not just files beginning with a space
				strcpy(strBadImports,INBOX_PATH);
				strcat(strBadImports,BAD_IMPORTS_SUBDIR);
				mkdir((LPSTR)strBadImports);
				strcat(strBadImports,file_info.f_name);
				ret = rename((LPSTR)file_info.f_name,(LPSTR)strBadImports);
				if (ret == -1) // in case the bad file has had an attempted import before
					unlink((LPSTR)file_info.f_name);
			}					
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
				playBip();
				//strcpy(fname, file_info.f_name);	
				//logString(fname,BUFFER);		
				if (!wait && PLEASE_WAIT_IDX && context.package) {  // prevents trying to insert this sound before config & control files are loaded.
					insertSound(&pkgSystem.files[PLEASE_WAIT_IDX],NULL,TRUE); 
					wait = 1; // so directory check doesn't duplicate
				}
				fret += processDir(file_info.f_name, ncp);
				if (ncp->isNew) {
					// Save the most recent new content to have something new to queue up after finished processing.
					lastAddedNewContent.isNew = 1;
					strcpy(lastAddedNewContent.newAudioCategory,ncp->newAudioCategory);
					strcpy(lastAddedNewContent.newAudioLanguage,ncp->newAudioLanguage);
					strcpy(lastAddedNewContent.newAudioName,ncp->newAudioName);
				}
				  // RHM: something I do below makes this necessary
				  //      upon return after _findnext returns -1 even if there are more dirs
				ret = _findfirst((LPSTR)fbuf, &file_info, D_ALL);
			}	
		}
		
		ret = chdir((LPSTR)savecwd);
	}	// end of if: length of strNewPkgPath must be > 1
	
	Lang[0] = 0;
	MLp = 0;
	nMLp = 0;
	
	if (!ncp->isNew && lastAddedNewContent.isNew) {
		// before returning, replace ncp with new content if last wasn't new and some earlier content was new
		// (because the queueNewContent() fct will play the content in the first position)
		ncp->isNew = 1;
		strcpy(ncp->newAudioCategory,lastAddedNewContent.newAudioCategory);
		strcpy(ncp->newAudioLanguage,lastAddedNewContent.newAudioLanguage);
		strcpy(ncp->newAudioName,lastAddedNewContent.newAudioName);
	}
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
	l = (long)copyMovedir((char *)strSysUpdatePath, (char *)"a:/");  // returns # of items copied
	ret = check_new_sd_flash(strSysUpdatePath);  //strSysUpdatePath may be changed
	if ((l > 1) || (ret != 0)) { // 1 is for the SYS_UPDATE_SUBDIR
		if (PLEASE_WAIT_IDX && context.package) {  // prevents trying to insert this sound before config & control files are loaded.
			insertSound(&pkgSystem.files[PLEASE_WAIT_IDX],NULL,TRUE); 
		}
		playBip();
		resetSystem(); // reset to begin new firmware reprogramming or to reload new config/system control
	}
}


static int 
processA18(struct f_info *fip, struct newContent *pNC) {
	int getMetaCat(char *filename, char *category);
	char toPath[PATH_LENGTH], fromPath[LONG_PATH_LENGTH];
	char newFileName[LONG_PATH_LENGTH], category[FILE_LENGTH], lang[FILE_LENGTH];
	int ret, len, /*i, catidx, subidx, cat_base, */ fret, handle;

	category[0] = lang[0] = 0;
	if(getMetaCat(fip->f_name, category))  {				
		handle = open((LPSTR)fip->f_name, O_RDONLY);
		if(handle >= 0) {
			ret = metaRead(handle, DC_LANGUAGE, (unsigned int*)&lang);
			close(handle);
		}
	} else {
		strcpy(category, "0");
		handle = tbOpen((LPSTR)fip->f_name, O_RDONLY);
		if(handle >= 0) {
			metaRead(handle, DC_LANGUAGE, (unsigned int*)&lang);
			close(handle);
		}
	}
	logString((char *)"category is",BUFFER,LOG_DETAIL);
	logString(category,BUFFER,LOG_DETAIL);
	if (!lang[0]) {
		if (pkgSystem.idxLanguageCode)
			strcpy(lang,&pkgSystem.strHeapStack[pkgSystem.idxLanguageCode]);
		else 
			strcpy(lang,currentProfileLanguage()); // TODO: use first language in profiles.txt file when processing inbox before system start
	} 		
	strcpy(pNC->newAudioCategory, category);
	strcpy(pNC->newAudioLanguage,lang);

	strcpy(fromPath,INBOX_PATH);
	strcat(fromPath,NEW_PKG_SUBDIR);
	strcat(fromPath,fip->f_name);

	strcpy(newFileName, fip->f_name);
	len =  strlen(newFileName) - strlen(AUDIO_FILE_EXT);   //todo: handle 0-length 
	if (len > FILE_LENGTH)
		len = FILE_LENGTH;
	while (newFileName[len-1] == ' ')
		len--; // don't let a filename end in a space
	newFileName[len] = 0;	
	strcpy(pNC->newAudioName, newFileName);
			
	strcpy(toPath, USER_PATH);
	strcat(toPath, newFileName);
	strcat(toPath, AUDIO_FILE_EXT);
	ret = rename((LPSTR)fromPath, (LPSTR)toPath);
	logString((char *)fromPath,BUFFER,LOG_DETAIL);
	logString((char *)toPath,BUFFER,LOG_DETAIL);
	logString((char *)pNC->newAudioLanguage,ASAP,LOG_DETAIL);
	
	if(ret) {   // rename failed, probably already exists
		int fdinbox, fdcard;
		fdcard  = tbOpen((LPSTR)toPath, O_RDONLY);
		fdinbox = tbOpen((LPSTR)fromPath, O_RDONLY);
		ret = checkRevisions(fdcard, fdinbox);
		if(ret == 0) {  // revision in inbox > revision on card
			unlink((LPSTR)toPath);
			ret = rename((LPSTR)fromPath, (LPSTR)toPath);
		}
		if(ret) {
			pNC->isNew = 0;
			logString((char *)"inbox message already exists on device",ASAP,LOG_DETAIL);
			unlink((LPSTR)fromPath);	// rename failed, remove from inbox anyway
		} else {
			pNC->isNew = 1;
			fret++;
		}
	} else {
		pNC->isNew = 1;
		fret++;
	}
		
	ret = newUpdateCategory(pNC,0);
	
	return(fret);

}

static int 
processDir(char *dirname, struct newContent *pNC) {
	char savecwd[PATH_LENGTH];
	char fromPath[LONG_PATH_LENGTH], toPath[PATH_LENGTH];
	char newDirName[LONG_PATH_LENGTH], category[FILE_LENGTH], lang[FILE_LENGTH];
	char metadataFilePath[LONG_PATH_LENGTH];
	
	int ret, fret, catidx, subidx, i, handle, len;
	char *endOfPath;

	ret = getcwd((LPSTR)savecwd , sizeof(savecwd) - 1 );
	ret = chdir((LPSTR)dirname);
	
	category[0] = 0;
	lang[0] = 0;
	catidx = subidx = 0;	
	fret = 1;
	
	//copyCWD(USER_PATH);
	strcpy(fromPath,INBOX_PATH);
	strcat(fromPath,NEW_PKG_SUBDIR);
	strcat(fromPath,dirname);
	strcpy(metadataFilePath,fromPath);
	strcat(metadataFilePath,"/");
	strcat(metadataFilePath, dirname);
	strcat(metadataFilePath,AUDIO_FILE_EXT);	
	
	if(getMetaCat(metadataFilePath, category))  {				
		logString((char *)"category is",BUFFER,LOG_DETAIL);
		logString(category,ASAP,LOG_DETAIL);
		handle = open((LPSTR)metadataFilePath, O_RDONLY);
		if(handle >= 0) {
			ret = metaRead(handle, DC_LANGUAGE, (unsigned int*)&lang);
			close(handle);
		} else {
			strcpy(lang,&pkgSystem.strHeapStack[pkgSystem.idxLanguageCode]);
		}
	} else {
		strcpy(category, "0");
		handle = tbOpen((LPSTR)metadataFilePath, O_RDONLY);
		if(handle >= 0) {
			i = metaRead(handle, DC_LANGUAGE,  (unsigned int*)&lang);
			close(handle);
		}
	}
	if (!lang[0])
		strcpy(lang,&pkgSystem.strHeapStack[pkgSystem.idxLanguageCode]);
	strcpy(pNC->newAudioCategory, category);
	strcpy(pNC->newAudioLanguage,lang);

	strcpy(newDirName,dirname);
	len = strlen(newDirName);
	if (len > FILE_LENGTH)
		len = FILE_LENGTH;
	while (newDirName[len-1] == ' ')
		len--; // don't let a directory end in a space
	newDirName[len] = 0;	
	strcpy(pNC->newAudioName, newDirName);
	strcpy(toPath,USER_PATH);
	strcat(toPath,newDirName);
	
	ret = rename((LPSTR)fromPath,(LPSTR)toPath);
	logString((char *)fromPath,BUFFER,LOG_DETAIL);
	logString((char *)toPath,BUFFER,LOG_DETAIL);
	logString((char *)pNC->newAudioLanguage,ASAP,LOG_DETAIL);
	if(ret) {   // rename failed, probably already exists
		int fdinbox, fdcard;
		endOfPath = toPath + strlen(toPath);
		strcat(toPath,"/");
		strcat(toPath, dirname);
		strcat(toPath,AUDIO_FILE_EXT);	
		fdinbox = tbOpen((LPSTR)metadataFilePath, O_RDONLY);
		fdcard  = tbOpen((LPSTR)toPath, O_RDONLY);
		ret = checkRevisions(fdcard, fdinbox);
		if(ret == 0) {  // revision in inbox > revision on card
			*endOfPath = 0; // return toPath to directory name only
			deleteAllFiles(toPath);
			rmdir((LPSTR)toPath);			
			ret = rename((LPSTR)fromPath, (LPSTR)toPath);
		}
		if(ret) {
			pNC->isNew = 0;
			logString((char *)"inbox message already exists on device",ASAP,LOG_DETAIL);
			deleteAllFiles(fromPath);
			rmdir((LPSTR)fromPath);			
		} else {
			pNC->isNew = 1;
			fret++;
		}
	} else {
		pNC->isNew = 1;
		fret++;
	}
					
	ret = newUpdateCategory(pNC,1);
			
	ret = chdir((LPSTR)savecwd);		
	return (fret);
}

MLENTRY 
matchCategory(MLENTRY *mlp, MLENTRY *filecat) {
	int i;
	MLENTRY *mp, tmpfilecat = *filecat;
	for (i=0, mp=mlp; i<nMLp; i++, mp++) {
		if(*mp == tmpfilecat) {
			return(tmpfilecat);
		}
	}
	tmpfilecat &= (long) 0xffffff00;
	for (i=0, mp=mlp; i<nMLp; i++, mp++) {
		if(*mp == tmpfilecat) {
			return(tmpfilecat);
		}
	}
	tmpfilecat &= (long) 0xffff0000;
	for (i=0, mp=mlp; i<nMLp; i++, mp++) {
		if(*mp == tmpfilecat) {
			return(tmpfilecat);
		}
	}
	tmpfilecat &= (long) 0xff000000;
	for (i=0, mp=mlp; i<nMLp; i++, mp++) {
		if(*mp == tmpfilecat) {
			return(tmpfilecat);
		}
	}
	
	return((MLENTRY)0);
}
static int 
newUpdateCategory(struct newContent *pNC, int isDir) {
	char buffer[80], tmpbuf[80], path[PATH_LENGTH], catwrk[20], *cp, *lang;
	int i, ret;
	MLENTRY filecat, mlret;

	lang = pNC->newAudioLanguage;
	logString((char *)"newUpdateCategory",BUFFER,LOG_DETAIL);	
	// rotate through all profiles and update categories with matching language
	i = currentProfile(); 
	do {
		if (strcmp(lang,currentProfileLanguage()))
				continue; // skip profile if profile language doesn't match content languge				
		strcpy(path,LISTS_PATH);
		strcat(path,currentProfileMessageList());
		strcat(path, "/");
		strcat(path,LIST_MASTER);
		strcat(path,".txt");
		logString(path,ASAP,LOG_DETAIL);
		if(strcmp(lang, &Lang)) {
			ret = loadLanglisttoMemory(path,  MLp, MAX_ML_ENTRIES);
			nMLp = ret;
		}
	
		if (!strcmp((char *)FEEDBACK_CATEGORY,(char *)pNC->newAudioCategory)) {
			// Feedback messages stay in the feedback category -- add the category if needed
			addCategoryToActiveLists((char *)FEEDBACK_CATEGORY,currentProfileMessageList());
			if (DEBUG_MODE) {
				logString((char *)"Assigned to feedback category: list "FEEDBACK_CATEGORY,ASAP,LOG_NORMAL);
			}			
		}
		else {
			categoryStringtoLong(pNC->newAudioCategory, &filecat);
			if (DEBUG_MODE) {
				logString((char *)"Metadata category is:",BUFFER,LOG_NORMAL);
				logString(pNC->newAudioCategory,ASAP,LOG_NORMAL);
			}
			mlret = matchCategory(MLp, &filecat);		
			categoryLongtoString(&catwrk[0], &mlret);
			strcpy(pNC->newAudioCategory, catwrk);
			if (DEBUG_MODE) {
				logString((char *)"Assigned list is:",BUFFER,LOG_NORMAL);
				logString(pNC->newAudioCategory,ASAP,LOG_NORMAL);
			}
		}	
	//	cpyListPath(path,&catwrk[0]);
		if (pNC->newAudioCategory[0] == SYS_MSG_CHAR)
			strcpy(path,LANGUAGES_PATH);
		else
			strcpy(path,LISTS_PATH);
		strcat(path, currentProfileMessageList());
		strcat(path, "/");
		
		strcpy(buffer, path);
		strcat(buffer, pNC->newAudioCategory);
		strcat(buffer,".txt");
	
	/*	
		if(!fileExists((LPSTR)buffer)) {  // if category does not exist, put file in OTHER - 0.txt
			strcpy(buffer, path);
			strcat(buffer,"0.txt");
		}
	*/	
	    if(isDir) {
			tmpbuf[0] = APP_PKG_CHAR;
			strcpy(tmpbuf+1, pNC->newAudioName);
		} else {
			strcpy(tmpbuf,pNC->newAudioName);
		}
		logString(buffer, BUFFER,LOG_DETAIL);
		logString(tmpbuf, BUFFER,LOG_DETAIL);
	// This checks if entry already exists and adds a new line only if it does not.
		ret = findDeleteStringFromFile((char *)NULL, buffer, tmpbuf, 0);
			
	//  append the basename to the proper .txt file in lists
		if (ret == -1)
			ret = insertStringInFile(buffer,tmpbuf,0);
	
	/*
		// FOR TESTING ONLY: To test lots of writes to a list file
		for(ret = 0;ret < MAX_VOLUME; ret++) {
			tmpbuf[0]='0'+ret%65;
			insertStringInFile(buffer,tmpbuf,0);	
			setLED(LED_RED,FALSE);
			wait(200);
			setLED(LED_RED,TRUE);			
		} 
	
	*/
		// if categorized as other, ensure other is on the activeList
		if((cp = strrchr(buffer, '/')) && (!strcmp(cp+1, OTHER_CATEGORY ".txt")))
			addCategoryToActiveLists((char *)OTHER_CATEGORY,currentProfileMessageList());
	} while (i != nextProfile());
	return(ret);

}

static int 
getMetaCat(char *filename, char *category)
{
	int convertTwoByteToSingleChar(unsigned int *, const unsigned int *, int);
	int ret = 0;
	long wrk, numfields, fieldlen;
	unsigned int nret, fd;
	int fid;
	unsigned char nfv;
	unsigned char buf[128];
	int i, j;

	fd = tbOpen((LPSTR)filename, 0);
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
//			buf[0] &= 0xff;
			buf[fl] = 0;
			//printf("    field value[%d]=",j);
/*			for(k=0; k<fl; k++) {
				printf("0x%.2x ", buf[k]);
			}
			printf("\n");
*/
			//printf("'%s'",buf);
			if(fid == 0) { // categories
				unsigned int m = (buf[0] & 0xff) - '0';
				if((m >= 0 && m <= 9)) {
					ret = convertTwoByteToSingleChar(category,(const unsigned int *)buf,fl);
					ret = 1;
					goto done;
				} else {
					strcpy(category, "0");
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

// return 0 if the metadata DTB_REVISION from the 2nd arg is greater that that on the first card
// return non zero for all other cases & errors
static int 
checkRevisions(int fdcard, int fdinbox)
{
	int i, j, iInbox, iCard;
	char revInInbox[8], revOnCard[8];
	
	revInInbox[0] = revOnCard[0] = 0;
	
	if(fdcard < 0) {
		if(fdinbox >= 0)
			close(fdinbox);
			return(1);
	}
	if(fdinbox < 0) {
		close(fdcard);
		return(1);
	}

	i = metaRead(fdcard, DTB_REVISION, revOnCard);
	j = metaRead(fdinbox, DTB_REVISION, revInInbox);
	close(fdinbox);
	close(fdcard);
	
	if(i <= 0 || j <= 0) {
		return(2);
	}
	iInbox = strToInt(revInInbox);
	iCard  = strToInt(revOnCard);
	if(iInbox > iCard) {
		return(0); 
	}
	return(3);  // no change
}
