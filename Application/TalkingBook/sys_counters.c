#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"
#include "Include/talkingbook.h"
#include "Include/files.h"
#include "Include/sys_counters.h"

APP_IRAM SystemCounts systemCounts;

void saveSystemCounts() {
	int handle, ret;
	
	ret = unlink((LPSTR)(SYSTEM_VARIABLE_FILE));
	handle = tbOpen((LPSTR)(SYSTEM_VARIABLE_FILE),O_CREAT|O_RDWR);
	if (handle != -1)
		ret = write(handle, (unsigned long)&systemCounts<<1, sizeof(SystemCounts)<<1);
	else {
		close(handle);
		logException(17,SYSTEM_VARIABLE_FILE,RESET); //can't save SYSTEM_VARIABLE_FILE;
	}
	close(handle);
}

int loadSystemCounts() {
	int handle, ret;

	handle = tbOpen((LPSTR)(SYSTEM_VARIABLE_FILE),O_RDONLY);
	if (handle != -1)
		ret = read(handle,(unsigned long)&systemCounts<<1,sizeof(SystemCounts)<<1);
	else
		ret = -1;
	close(handle);
	return ret;
}

void getPkgNumber(char *name, BOOL shouldUpdate) {
	char digits[8];
	
	strcpy(name,PKG_NUM_PREFIX);
	if (shouldUpdate)
		systemCounts.packageNumber++;
	longToDecimalString(systemCounts.packageNumber,digits,5);
	strcat(name,digits);
	saveSystemCounts();
}	

/*
void nextListNumber(char *name) {
	char digits[8];
	
	strcpy(name,LIST_NUM_PREFIX);
	longToDecimalString(++systemCounts.listNumber,digits,4);
	strcat(name,digits);
	saveSystemCounts();
}	
*/

