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
	char strLog[PATH_LENGTH], savecwd[PATH_LENGTH];
	char fbuf[PATH_LENGTH], fname[FILE_LENGTH];
	int ret, r1, len, fret;
	
	fret = 0;
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
	
			fret += ProcessA18(&file_info);
	
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
				if(file_info.f_name[0] == '.')
					continue;
				
/*				if(!strcmp(file_info.f_name, "."))
					continue;
				if(!strcmp(file_info.f_name, ".."))
					continue;
				if(!strcmp(file_info.f_name, ".svn"))
					continue;
*/
				if(!strcmp(file_info.f_name, "lists")) // lists is special, never deleted	
					continue;
				
				strcpy(fname, file_info.f_name);	
				fret += ProcessDir(fname);
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
	
	return(fret);
}

int 
ProcessA18(struct f_info *fip)
{
	char strLog[80], savecwd[80];
	char buffer[READ_LENGTH+1], *line, tmpbuf[READ_LENGTH+1];
	char fnbase[80], category[8], subcategory[8];
	int ret, len, len_fnbase, i, catidx, subidx, cat_base, fret;

	category[0] = subcategory[0] = 0;
	cat_base = strIndex(fip->f_name, '#');
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
	strcat(tmpbuf,fip->f_name);

// TODO: should some other test be applied here??
	if(0 == strncmp(fnbase,PKG_NUM_PREFIX,strlen(PKG_NUM_PREFIX))) {
		ret = 1;
		fret++;
		do {
			strcpy(buffer,USER_PATH);
			getrevdPkgNumber(fnbase,TRUE);
			strcat(fnbase,"#");
			strcat(fnbase, category);
			strcat(fnbase, subcategory);
			strcat(buffer, fnbase);
			strcat(buffer,AUDIO_FILE_EXT);
			ret = rename(tmpbuf, buffer);
		} while (ret);
	} else {
		strcat(buffer, fip->f_name);
		ret = rename(tmpbuf, buffer);
		if(ret) {
			unlink(tmpbuf);	// rename failed, remove from inbox anyway
		} else {
			fret++;
		}
	}
			
// TODO - currently doing nothing with subcategory
	
	ret = updateCategory(category, fnbase, (char *)NULL);
	
	return(fret);

}

int 
ProcessDir(char *dirname)
{
	struct f_info file_info;
	char strLog[80], savecwd[80];
	char buffer[READ_LENGTH+1], tempbuf[80];
	char fnbase[80], category[8], subcategory[8];
	int ret, fret, catidx, subidx, len_fnbase, i, cat_base;

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
	
	fret = 1;
	
	cat_base = strIndex(dirname, '#');

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
// TODO: - other prefixs to process besides ^
	
	ret = updateCategory(category, fnbase, "^");
		
	ret = chdir(savecwd);	
	ret = rmdir(dirname);
	
	return (fret);
}

int copyCWD(char *todir)
{
	char to[80], cwd[80], strLog[80];
	int ret, r1, tobase, fret;
	struct f_info fi;
	
	strcpy(to, todir);
	fret = 0;
	
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
		
// skip files and directories beginning with .		
		if(fi.f_name[0] == '.')
			continue;
		
		to[tobase] = 0;

		if(fi.f_attrib & D_DIR) {
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
				fret++;
			}
			r1 = unlink(fi.f_name);
		}
	}
	
	return(fret);	
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
	
	if(!fileExists((LPSTR)buffer)) {  // if category.txt does not exist create it
		ret = open((LPSTR)buffer,O_CREAT|O_RDWR);
		close(ret);
	}
	
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
		ret = insertStringInFile(buffer,tmpbuf,0);
//		ret = appendStringToFile(buffer, tmpbuf); 
	return ret;
}

int copyOutbox()
{
	int ret;
	char inbox[PATH_LENGTH];
	
	LBstrncpy(inbox,INBOX_PATH,PATH_LENGTH);
	inbox[0] = 'b'; // changes a: to b:
	ret = copydir(OUTBOX_PATH, inbox);	
	return(ret);
}
//
// copy directory tree below fromdir (all subdirectories and files at all levels)
//
int copydir(char *fromdir, char *todir)
{
	int ret, r1, len_from, len_to, fret;
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
		
//		fret += copyfiles(from, to);
		fret += copydir (from, to);
		
		from[len_from] = 0;
		strcat(from, "*");
		ret =_findfirst(from, &fi, D_ALL);  // should not be necessary
		
		fret++;
	}
	
	from[len_from] = 0;
	to[len_to]= 0;
	fret += copyfiles(from, to);
	
	return(fret);

}
//
// copy all files in fromdir to todir
//
int copyfiles(char *fromdir, char *todir)
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
			r1 = _copy(from, to);
		}
		ret = _findnext(&fi);
		fret++;
	}
		
	return(fret);
}
