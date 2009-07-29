#include "Include/talkingbook.h"
#include "Include/device.h"
#include "Include/files.h"
#include "Include/lists.h"
#include "Include/containers.h"
#include "Include/audio.h"
#include "Include/pkg_ops.h"
#include "Include/Inbox.h"

extern APP_IRAM char logBuffer[LOG_BUFFER_SIZE];
extern APP_IRAM int idxLogBuffer;
static int ProcessA18(struct f_info *);
static int ProcessDir(char *);
static int copyCWD(char *);
static int updateCategory(char *, char *, char *);

#define D_NOTDIR (D_FILE | D_RDONLY | D_HIDDEN | D_SYSTEM | D_ARCHIVE)

//  called when leaving USB mode to check for files copied from other device into a:\Inbox
//
unsigned int
ProcessInbox()
{
	struct f_info file_info;
	char strLog[80], savecwd[80];
	char fbuf[80], fname[32];
	int ret, r1, len;
	
	strcpy(strLog, "ProcessInbox");	
	logString(strLog, BUFFER);
	
//  check for *.a18 files in Inbox
	strcpy(fbuf, INBOX_PATH);
	len = strlen(fbuf);
	if(len > 1) {
		fbuf[len-1] = 0;
		ret = fileExists(fbuf);
		if(ret == 0) {
			mkdir(fbuf);
			strcpy(strLog, "ProcessInbox no inbox, created");	
			logString(strLog, ASAP);
			return(0);
		}	
		ret = getcwd(savecwd , sizeof(savecwd) - 1 );
		ret = chdir(INBOX_PATH);
		
	//  process *.a18 files in inbox - category in file name after # OR IN *.txt
	//  this case may never happen
	//
		strcpy(strLog, INBOX_PATH);
		len = strlen(strLog);
		strcpy(fbuf, INBOX_PATH);
		strcat(fbuf, "*");
		strcat(fbuf,AUDIO_FILE_EXT);
		ret =_findfirst(fbuf, &file_info, D_ALL);
		while (ret >= 0) {
			strcpy(&strLog[len], file_info.f_name);
			logString(strLog,BUFFER);
	
			ret = ProcessA18(&file_info);
	
			ret = unlink(file_info.f_name);
				
			ret = _findnext(&file_info);
		}
	//
	//  now process directories in a:\inbox
	//
		strcpy(fbuf, INBOX_PATH);
		strcat(fbuf, "*.*");
		
		ret =_findfirst(fbuf, &file_info, D_DIR);
		
		for (; ret >= 0; ret = _findnext(&file_info)) {
			if(file_info.f_attrib & D_DIR) {
				if(!strcmp(file_info.f_name, "."))
					continue;
				if(!strcmp(file_info.f_name, ".."))
					continue;
				if(!strcmp(file_info.f_name, ".svn"))
					continue;
				if(!strcmp(file_info.f_name, "lists")) // lists is special, never deleted	
					continue;
				
				strcpy(fname, file_info.f_name);	
				r1 = ProcessDir(fname);
				  // RHM: something I do below makes this necessary
				  //      upon return after _findnext returns -1 even if there are more dirs
				ret = _findfirst(fbuf, &file_info, D_ALL);
			}	
		}
			
		r1 = ProcessDir("lists");
	
		ret = chdir(savecwd);

		//TODO: queue up first recording found to be auto-played
		//context.queuedPackageType = PKG_???
		//context.queuedPackageNameIndex = replaceStack(filename,&pkgSystem);
		//will also need to set list counters properly
				
		strcpy(strLog, "ProcessInbox exits");	
		logString(strLog, ASAP);
	}	// length of INBOX_PATH must be > 1
}

int 
ProcessA18(struct f_info *fip)
{
	char strLog[80], savecwd[80];
	char buffer[READ_LENGTH+1], *line, tmpbuf[READ_LENGTH+1];
	char fnbase[80], category[8], subcategory[8];
	int ret, len, len_fnbase, i, catidx, subidx;

	category[0] = subcategory[0] = 0;
	ret = strIndex(fip->f_name, '#');
	catidx = subidx = 0;
	if(ret >= 1) {	// category info in filename
		strcpy(fnbase, fip->f_name);
//	Commenting line below since we want to carry through category info
//		fnbase[ret] = 0; 
		len_fnbase = strIndex(fip->f_name, '.');
		for(i=ret; ; i++ ) {
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
		} else { // .a18 file without category info
			strcpy(category, "OTHER");
			//or
			// strcpy(category, "O");
		}
	}

	strcpy(buffer, USER_PATH);
	fnbase[len_fnbase] = 0;

// TODO: should some other test be applied here??
	if(0 == strncmp(fnbase,PKG_NUM_PREFIX,strlen(PKG_NUM_PREFIX))) {
		ret = 1;	
		do {
			strcpy(buffer,USER_PATH);
			getrevdPkgNumber(fnbase,TRUE);
			strcat(buffer, fnbase);
			strcat(buffer,AUDIO_FILE_EXT);
			ret = fileExists((LPSTR)buffer); // this causes approx 1 sec delay!
		} while (ret);
	}
	
	strcpy(tmpbuf,INBOX_PATH);
	strcat(tmpbuf,fip->f_name);
	ret = rename(tmpbuf, buffer);
			
// TODO - currently doing nothing with subcategory
	
	ret = updateCategory(category, fnbase, (char *)NULL);

}

int 
ProcessDir(char *dirname)
{
	struct f_info file_info;
	char strLog[80], savecwd[80];
	char buffer[READ_LENGTH+1], tempbuf[80];
	char fnbase[80], category[8], subcategory[8];
	int ret, catidx, subidx, len_fnbase, i;

	ret = getcwd(savecwd , sizeof(savecwd) - 1 );
	ret = chdir(dirname);

	if(!strcasecmp(dirname, "lists")) {
		// move from \\Inbox\lists to \\lists
		copyCWD("a:\\");
		ret = chdir(savecwd);
		return(0);
	} 
	
	// processing a directory that is not "lists"
	
	category[0] = subcategory[0] = 0;
	catidx = subidx = 0;
	
	ret = strIndex(dirname, '#');

	if(ret >= 1) {	// category info in filename
		strcpy(fnbase, dirname);
		//fnbase[ret] = 0;
		len_fnbase = ret;
		for(i=ret+1; ; i++ ) {
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
	strcat(buffer,dirname);
	strcpy(tempbuf,USER_PATH);
	strcat(tempbuf,dirname);
	rename(buffer,tempbuf);
	
	if(catidx == 0)
		strcpy(category, "OTHER");  // "O"
	
// TODO: - currently doing nothing with subcategory
// TODO: - other prefixs to process besides ^	
	ret = updateCategory(category, fnbase, "^");
		
	ret = chdir(savecwd);	
	ret = rmdir(dirname);
	
	return 0;
}

int copyCWD(char *todir)
{
	char to[80], cwd[80], strLog[80];
	int ret, r1, tobase;
	struct f_info fi;
	
	strcpy(to, todir);
	
	ret = getcwd(cwd, sizeof(cwd)-1);
//  NOTE: getcwd returns a:\inbox\  not a:\\inbox\ as in INBOX_PATH 
//        hence the -1 below
	strcat(to, &cwd[strlen(INBOX_PATH)-1]);  // chop off a:\inbox\ - 
//	ret = strIndex(to, '#');
//	if(ret > 1)
//		to[ret] = 0;
	ret = mkdir(to);  // this will fail for a:\\lists (used with lists), but that's ok
	strcat(to, "\\");
	tobase = strlen(to);

	strcat(cwd,"\\*.*");
	
	ret =_findfirst(cwd, &fi, D_ALL);
	for (; ret >= 0; ret = _findnext(&fi)) {
		to[tobase] = 0;
		if(fi.f_attrib & D_DIR) {
			if(!strcmp(fi.f_name, "."))
				continue;
			if(!strcmp(fi.f_name, ".."))
				continue;
			if(!strcmp(fi.f_name, ".svn"))
				continue;
			strcpy(&to[tobase], fi.f_name);
//			r1 = strIndex(to, '#');
//			if(r1 > 1)		// remove category info from dir name
//				to[r1] = 0;
			r1 = mkdir(to);
			r1 = chdir(fi.f_name);
			r1 = copyCWD(to);
		} else {
			strcat(to,fi.f_name);
// TODO: if unconditional copy need to unlink, then _copy
//			unlink(to);
			strcpy(strLog, to);
			logString(strLog, BUFFER);
			if(!fileExists(to)) {
				r1 = _copy(fi.f_name, to);
			}
			r1 = unlink(fi.f_name);
		}
	}
	
	return(0);	
}

int updateCategory(char *category, char *fnbase, char *prefix)
{
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
	if(prefix) {
		strcpy(tmpbuf, prefix);
		strcat(tmpbuf, fnbase);
	} else {
		strcpy(tmpbuf,fnbase);
	}
// This checks if entry already exists and adds a new line only if it does not.
	ret = findDeleteStringFromFile((char *)NULL, buffer, tmpbuf, 0);
		
//  append the basename to the proper .txt file in lists
//
//		int appendStringToFile(const char * filename, char * strText)
	if(prefix) {
		strcpy(tmpbuf, prefix);
		strcat(tmpbuf, fnbase);
	} else {
		strcpy(tmpbuf,fnbase);
	}
	if (ret == -1)
		ret = appendStringToFile(buffer, tmpbuf); 
}
