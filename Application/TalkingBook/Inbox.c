#include "Include/talkingbook.h"
#include "Include/device.h"
#include "Include/files.h"
#include "Include/lists.h"
#include "Include/containers.h"
#include "Include/audio.h"
#include "Include/pkg_ops.h"

extern APP_IRAM char logBuffer[LOG_BUFFER_SIZE];
extern APP_IRAM int idxLogBuffer;

#define D_NOTDIR (D_FILE | D_RDONLY | D_HIDDEN | D_SYSTEM | D_ARCHIVE)

//  called when leaving USB mode to check for files copied from other device into a:\Inbox
//
unsigned int
ProcessInbox()
{
	struct f_info file_info;
	char strLog[60], savecwd[60];
//	char buffer[READ_LENGTH+1], *line;
//	char fnbase[100], category[8], subcategory[8];
	int ret, len; //, len_fnbase, i, catidx, subidx;
		
//  check for *.a18 files in Inbox
	ret = fileExists("a:\\\\Inbox");
	if(ret == 0)
		return(0);
	
	ret = getcwd(savecwd , 59 );
	ret = chdir("a:\\\\Inbox");
	
	strcpy(strLog, "a:\\\\Inbox\\");
	len = strlen(strLog);
	ret =_findfirst("a:\\\\Inbox\\*.a18", &file_info, D_ALL);
	while (ret >= 0) {
		strcpy(&strLog[len], file_info.f_name);
		logString(strLog,BUFFER);

		ret = ProcessA18(&file_info);

//      REMOVE comment - commented for debugging - REMOVE		
//		ret = unlink(file_info.f_name);
			
		ret = _findnext(&file_info);
	}
	
	ret =_findfirst("a:\\\\Inbox\\*.*", &file_info, D_DIR);
	while (ret >= 0) {
		strcpy(&strLog[len], file_info.f_name);
		logString(strLog,BUFFER);

		ret = ProcessDir(&file_info);

//      REMOVE comment - commented for debugging - REMOVE		
//		ret = unlink(file_info.f_name);
			
		ret = _findnext(&file_info);
	}

	
	ret = chdir(savecwd);

}
int 
ProcessA18(struct f_info *fip)
{
	char strLog[60], savecwd[60];
	char buffer[READ_LENGTH+1], *line;
	char fnbase[100], category[8], subcategory[8];
	int ret, len, len_fnbase, i, catidx, subidx;

	category[0] = subcategory[0] = 0;
	ret = strIndex(fip->f_name, '#');
	catidx = subidx = 0;
	if(ret >= 1) {	// category info in filename
		strcpy(fnbase, fip->f_name);
		fnbase[ret] = 0;
		len_fnbase = ret;
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

	strcpy(buffer, "a:\\user\\");
	fnbase[len_fnbase] = 0;
	strcat(buffer, fnbase);
	strcat(buffer, ".a18");
// rename user recordings to R* (received)
// TODO: change number to global received number here
	if(buffer[8] == 'P') {
		buffer[8] = 'R';
		fnbase[0] = 'R';
	}
	
	ret = unlink(buffer);
	ret = _copy(fip->f_name , buffer);
			
// TODO - currently doing nothing with subcategory

// delete string from file if it exists
//	
//		int findDeleteStringFromFile(char *path, char *filename, char* string, BOOL shouldDelete)
	strcpy(buffer, "a:\\lists\\");
	strcat(buffer, category);
	strcat(buffer,".txt");
	ret = findDeleteStringFromFile((char *)NULL, buffer, fnbase, 1);
		
//  append the basename to the proper .txt file in lists
//
//		int appendStringToFile(const char * filename, char * strText)
	strcpy(buffer, "a:\\lists\\");
	strcat(buffer, category);
	strcat(buffer, ".txt");

	ret = appendStringToFile(buffer, fnbase); 
}

int 
ProcessDir(struct f_info *fip)
{
	// place holder for processing inbox directories
	
	return 0;
}
