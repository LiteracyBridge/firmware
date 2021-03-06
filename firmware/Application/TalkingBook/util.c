// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/app_exception.h"
#include "typedef.h"
#include <string.h>
#include <ctype.h>
#include <Application/TalkingBook/Include/util.h>
#include "./System/Include/System/GPL162002.h"
#include "Include/device.h"
#include "Include/files.h"

static int isWhiteSpace(char);

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

void ulongToStr(unsigned long number, char *dest) {
    int digit;
    char *pDigit = dest, tmp;
    // Convert the number, lsb first; fixed below.
    do {
        digit = number % 10;
        number = number / 10;
        *pDigit++ = '0' + digit;
    } while (number > 0);
    // Add the terminator, then back up the the last actual character.
    *pDigit-- = 0;
    // Reverse it in place. pDigit walks back, dest forward, until they meet in the middle.
    while (pDigit > dest) {
        tmp = *pDigit;
        *pDigit-- = *dest;
        *dest++ = tmp;
    }
}

void longToStr(long number, char *dest) {
    if (number<0) {
        number = -number;
        *dest++ = '-';
    }
    ulongToStr(number, dest);
}

void longToDecimalString(long l, char * string, int numberOfDigits) {
	int digit, i, numdig; 
	long divisor;
	long num;
	char * cursor = string;
	char buffer[72], wrk[12];
	
	num = l;
	numdig = numberOfDigits;
	for (divisor=1;numberOfDigits>1;numberOfDigits--) {
		divisor *= 10;
	}
	
	if(num >= (divisor * 10)) {  //more digits required to show l 
		for (i=numdig; i>0; i--) {
			*cursor++ = '*';
		}
		*cursor = 0;
		
		if((num & 0xffff0000) != 0) {
			i = 1;
		}
		
		strcpy(buffer,"longToDecimalString arg=0x");
		unsignedlongToHexString(l, (char *)wrk);
		strcat(buffer, wrk);
		strcat(buffer, " num digits=0x");
		unsignedlongToHexString((unsigned long)numdig, (char *)wrk);
		strcat(buffer, wrk);
		logString(buffer,ASAP,LOG_ALWAYS);
		return;
	}
	
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
	int digit, i, numdig; 
	long divisor;
	unsigned long num;
	char * cursor = string;
	char buffer[72], wrk[12];
	
	num = l;
	numdig = numberOfDigits;
	for (divisor=1;numberOfDigits>1;numberOfDigits--)
		divisor *= 10;
	
	if(num >= (divisor * 10)) {  //more digits required to show l 
		for (i=numdig; i>0; i--) {
			*cursor++ = '*';
		}
		*cursor = 0;
		
		if((num & 0xffff0000) != 0) {
			i = 1;
		}
		
		strcpy(buffer,"unsignedlongToDecimalString arg=0x");
		unsignedlongToHexString(l, (char *)wrk);
		strcat(buffer, wrk);
		strcat(buffer, " num digits=0x");
		unsignedlongToHexString((unsigned long)numdig, (char *)wrk);
		strcat(buffer, wrk);
		logString(buffer,ASAP,LOG_ALWAYS);
		return;
	}

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
void unsignedCharsToHex(unsigned char *chars, char *string, int len) {
    unsigned char byte;
    unsigned char nibble;
    while (len > 0) {
        byte = *chars++;
        nibble = (byte >> 4) & 0x0f;
        *string++ = nibble > 9 ? nibble-10+'a' : nibble+'0';
        nibble = byte & 0x0f;
        *string++ = nibble > 9 ? nibble-10+'a' : nibble+'0';
        *string++ = ' ';
        len--;
    }
    *string = 0;
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

static
int isWhiteSpace(char c) {
	return ((c==0x09 || c==0x0a || c==0x0d || c==0x20)?1:0);
}

char *trim(char *string) {
	char *ptr;
	
	ptr = string + strlen(string); // start at the null
	while (isWhiteSpace(*(ptr-1)) && ptr > string) {
		ptr--;	
	}
	*ptr = 0;  // now that previous character is first non-white-space, add null to terminate
	
	for (ptr=string;isWhiteSpace(*ptr);ptr++);
	return ptr;
}
