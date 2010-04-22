// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#include "Include/app_exception.h"
#include "typedef.h"
#include <string.h>
#include <ctype.h>

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
