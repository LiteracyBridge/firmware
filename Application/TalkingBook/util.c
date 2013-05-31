// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/app_exception.h"
#include "typedef.h"
#include <string.h>
#include <ctype.h>
#include ".\System\Include\System\GPL162002.h"
#include "Include/device.h"
#include "Include/files.h"
extern void write_app_flash(int *, int, int);

int strIndex (const char *str, char c) {
	char * cursor;
	int ret;
	
	cursor = strchr(str,c);
	if (cursor == NULL) 
		ret = -1;
	else
		ret = cursor - str;
	return ret;
}

int getBitShift (unsigned int miliseconds) {
    int bitShift;
    for(bitShift=0;miliseconds > 1;miliseconds>>=1)
    	bitShift++;    
	return bitShift;
}

unsigned long extractTime (unsigned int compressedTime, int iBitsOfPrecision) {
    unsigned long lExtractedTime;
    unsigned int iMidPrecision;
        
	if (compressedTime) {
	    iMidPrecision = 1<<(iBitsOfPrecision-1);  // gives midpoint of precision range
		lExtractedTime = (long)compressedTime << iBitsOfPrecision;  	    
	    lExtractedTime += iMidPrecision;    
	}
	else 
		lExtractedTime = 0;
    return lExtractedTime;
}

signed long extractSignedTime (signed int compressedTime,int iBitsOfPrecision) {
    signed long lExtractedTime;
	
	if (compressedTime < 0)
		lExtractedTime = (signed long)(-extractTime(abs(compressedTime),iBitsOfPrecision));
	else
		lExtractedTime = (signed long)extractTime(compressedTime,iBitsOfPrecision);
    return lExtractedTime;
}

unsigned int compressTime (unsigned long uncompressedTime, int iBitsOfPrecision) {
    unsigned int i;

    i = uncompressedTime >> iBitsOfPrecision;
    return i;
}

int lower (int c) {
	if (c >= 'A' && c <= 'Z')
		return c + 'a' - 'A';
	else
		return c;
}
	
int strToInt (char *str) {
	//todo: flag when NaN
	int i;
	int sign;
	
	while (*str && isspace(*str))
		str++;
	sign = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-') 
		str++;
	if (*str == '0' && *(str+1) == 'x') { 
		//hex
		str += 2;
		for (i = 0; isdigit(*str) || (lower(*str) >= 'a' && lower(*str) <= 'f'); str++) {
			if (lower(*str) >= 'a')
				i = 16 * i + (lower(*str) - 'a');
			else
				i = 16 * i + (*str - '0');
		}
	}
	else
		//decimal
		for (i = 0; isdigit(*str); str++) 
			i = 10 * i + (*str - '0');
	return i * sign;
}

void longToDecimalString(long l, char * string, int numberOfDigits) {
	int digit; 
	long divisor;
	long num;
	char * cursor = string;
	
	num = l;
	for (divisor=1;numberOfDigits>1;numberOfDigits--)
		divisor *= 10;
	if (num < 0)
		*cursor++ = '-';
	for (;divisor >= 10;divisor /= 10) {
		digit = num/divisor;
		num -= (digit * divisor);
		*cursor++ = digit + 0x30;
	}
	*cursor++ = (num % 10) + 0x30;
	*cursor = 0;	
}
void unsignedlongToDecimalString(unsigned long l, char * string, int numberOfDigits) {
	int digit; 
	long divisor;
	unsigned long num;
	char * cursor = string;
	
	num = l;
	for (divisor=1;numberOfDigits>1;numberOfDigits--)
		divisor *= 10;
	for (;divisor >= 10;divisor /= 10) {
		digit = num/divisor;
		num -= (digit * divisor);
		*cursor++ = digit + 0x30;
	}
	*cursor++ = (num % 10) + 0x30;
	*cursor = 0;	
}


void longToHexString(long l, char * string, int words) {
	int digit; 
	long divisor;
	long num;
	char * cursor = string;
	int numberOfDigits;
		
	//todo: deal with signed issue
	num = l;
	numberOfDigits = words * 4;
	for (divisor=1;numberOfDigits>1;numberOfDigits--)
		divisor *= 16;
	if (num < 0)
		*cursor++ = '-';
	for (;divisor >= 16;divisor /= 16) {
		digit = num/divisor;
		num -= (digit * divisor);
		if (digit < 10)
			*cursor++ = digit + 0x30;
		else
			*cursor++ = digit - 10 + 0x41;
		if (divisor == 0x10000)
			*cursor++ = ' ';
	}
	if (num < 10)
		*cursor++ = (num % 16) + 0x30;
	else
		*cursor++ = (num % 16) - 10 + 0x41;
	*cursor = 0;	
}
void unsignedlongToHexString(unsigned long l, char * string) {
	int digit; 
	long divisor;
	unsigned long num;
	char * cursor = string;
	int numberOfDigits;
		
	//todo: deal with signed issue
	num = l;
	numberOfDigits = 8;
	for (divisor=1;numberOfDigits>1;numberOfDigits--)
		divisor *= 16;
	for (;divisor >= 16;divisor /= 16) {
		digit = num/divisor;
		num -= (digit * divisor);
		if (digit < 10)
			*cursor++ = digit + 0x30;
		else
			*cursor++ = digit - 10 + 0x41;
	}
	if (num < 10)
		*cursor++ = (num % 16) + 0x30;
	else
		*cursor++ = (num % 16) - 10 + 0x41;
	*cursor = 0;	
}

void intToBinaryString(int i, char * string) {
	int bit; 
	long divisor;
	unsigned long num;
	char * cursor = string;
	int bitCount = 16;
	
	num = i;
	for (divisor=1;bitCount>1;bitCount--)
		divisor *= 2;
	for (;divisor >= 2;divisor /= 2) {
		bit = num/divisor;
		num -= (bit * divisor);
		*cursor++ = bit + 0x30;
	}
	*cursor++ = (num % 2) + 0x30;
	*cursor = 0;	
}



long strToLong (char *str) {
	//todo: flag when NaN
	long l;
	int sign;
	
	while (*str && isspace(*str))
		str++;
	sign = (*str == '-') ? -1 : 1;
	if (*str == '+' || *str == '-') 
		str++;
	for (l = 0; isdigit(*str); str++) 
		l = 10 * l + (*str - '0');
	return l * sign;
}

int goodChar(char c, int forFilename) {
	int ret = 1;
	
	// boolean check of character to find corrupted strings

	if ((c < 0x20 && c != 0x09 && c != 0x0a && c != 0x0d) || c > 0x7e) 
		ret = 0;
/*
	if (forFilename && 
	  (	c == '\"' || c == '*'  || c == '/'  || c == '\\' ||
		c == '<'  || c == '>'  || c == '?'  || c == '*'  || c == '|'))
			ret = 0;
*/
	return ret;
}

int goodString(char * str, int forFilename) {
	char *c;
	int ret = 1;

	for (c = str; *c; c++)
		if (!goodChar(*c, forFilename)) {
			ret = 0;
			break;
		}
	if (!ret)
		logException(27,str,0);
	return ret;
}

int LBstrncpy (char *to, const char *from, int max) {
	const char * f;
	int len;
	
	for (f = from,len = 0;len < max && (*to++ = *f++);len++);
	if (len == max)
		*to = '\0';	
	return len--; // don't count \0
} 

int LBstrncat (char *to, const char *from, int max) {
	int initLen, maxAddLen, addedLen;
	
	initLen = strlen(to);
	maxAddLen = max - initLen - 1;
	if (maxAddLen > 0)
		addedLen = LBstrncpy(to + initLen,from,maxAddLen);
	else
		addedLen = 0;
	return initLen + addedLen;
} 

void 
initRandomNG()
{
	*P_TimerF_Ctrl = 0x2002; // enable, 32768HZ
	*P_TimerE_Ctrl = 0x2006; // enable, timerF overflow, at 32768HZ timerE bumps every 2 sec
}
unsigned long rand() {
	unsigned long ret;
	unsigned long wrk;

	ret = (unsigned long) *P_TimerE_UpCount;

	wrk = ret & 0xff;

	ret <<= 16;
	ret |= *P_TimerF_UpCount;
	
	wrk += *P_TimerF_UpCount & 0xff;
	wrk += (*P_TimerF_UpCount & 0xff00) >> 8;
	wrk <<= 24;

	ret &= ~wrk;	// high order byte = sum of other 3 bytes (mod 256)
	ret |= wrk;
	
	return(ret);
}


/*
Items kept in SN flash:

Updated Once:
   - Serial Number (~16 bytes or whatever we do now)

Updated 1-10 times per year:
   - Location (could be just 2 bytes for an id or ~32 bytes if we wanted to just put the full string in there)
   - ID of Content Package (could be just 2 bytes for an id or ~12 bytes if we wanted to just put the full string in there)
   - Date of Content Update (could be 2 bytes or ~10 bytes for the whole string)
*/

// find offset to first writeable byte in SN flash 
int
FindFirstFlashOffset() {
	unsigned int *fp = (unsigned int *) TB_SERIAL_NUMBER_ADDR;
	int i;
	
	for (i=0; i<TB_FLASH_SIZE; i++) {
		if(*(fp+i) == 0xffff) {  // first unwritten byte
			break;
		}
	}
	
	return(i);	
}

void
RebuildFlash(char *newstring) {
	char curLoc[MAX_LOC_SIZE];
	char curID [MAX_ID_SIZE];
	char curDate [MAX_DATE_SIZE];
	char curSN [50];
	
	unsigned int *fp = (unsigned int *) TB_SERIAL_NUMBER_ADDR;
	int offset;
	
	strcpy(curSN, fp);  // save SN
	
	curLoc[0] = 0;
	curID[0] = 0;
	curDate[0] = 0;

// copy current values of known items, overwriting older values with newer ones	
	while (*fp != 0xffff) {
		if(!strncmp(fp, LOC_PREFIX, 2)) {  // Location
			strcpy(curLoc, fp);
		}
		if(!strncmp(fp, ID_PREFIX, 2)) { // Content ID
			strcpy(curID, fp);
		}
		if(!strncmp(fp, CONT_DATE_PREFIX, 2)) { // Content date
			strcpy(curDate, fp);
		}
		
		fp += strlen(fp) + 1;
	}

// if newstring is a known item, use it as the newest value		
	if(!strncmp(newstring, LOC_PREFIX, 2)) {
		strcpy(curLoc, newstring);
	}
	if(!strncmp(newstring, ID_PREFIX, 2)) {
		strcpy(curID, newstring);
	}
	if(!strncmp(newstring, CONT_DATE_PREFIX, 2)) {
		strcpy(curDate, newstring);
	}

// erase SN flash and put SN back,  the offset of 0 causes the erase
	write_app_flash(curSN, strlen(curSN) + 1, 0);
		
	if(curLoc[0]) {
		offset = FindFirstFlashOffset();
		write_app_flash(curLoc, (int) (strlen(curLoc) + 1), offset);
	}
	if(curID[0]) {
		offset = FindFirstFlashOffset();
		write_app_flash(curID, strlen(curID) + 1, offset);
	}
	if(curDate[0]) {
		offset = FindFirstFlashOffset();
		write_app_flash(curDate, strlen(curDate) + 1, offset);
	}
}

// append a string to SN flash, if SN flash is full erase it and rewrite SN + current location, content id, content date
int
AppendStringToFlash(char *newstring) {
	int firstavail = FindFirstFlashOffset();
	int navail = TB_FLASH_SIZE - firstavail;
	int needed = strlen(newstring) + 1;
	
	if(navail < needed) {
		// not enough room for this new value, erase SN flash and reflash current items & newstring
		RebuildFlash(newstring);
	} else {	
		write_app_flash((int *)newstring, needed, firstavail);
	}
}

// find last string in SN flash which begins with prefix
char *
FindCurFlashString(char *prefix) {
	char *ret = NULL;
	int prefix_len = strlen(prefix);
	
	char *fp = (char *) TB_SERIAL_NUMBER_ADDR;
	
	while((*((int *)fp) != 0xffff) && (fp < (TB_SERIAL_NUMBER_ADDR + TB_FLASH_SIZE))) {
		if(!strncmp(fp, prefix, prefix_len)) {
			ret = fp;
		}
		fp += strlen(fp) + 1;
	}
	
}


