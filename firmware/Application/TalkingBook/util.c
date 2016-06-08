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

/***********************************************************************************************************************
  F A T 3 2   utilities
/**********************************************************************************************************************/

#define TARGET_VOLUME_SERIAL 0x9285f050
#define MBR_PARTITION_TYPE_OFFSET (0x1be + 4)    // offset of partition type
#define MBR_PARTITION_SECTOR_OFFSET 454     // UINT32, count of sectors before partition
#define MBR_SIGNATURE_OFFSET 0x1fe          // offset of MBR signature in sector
#define MBR_FAT32_PARTITION_CODE 0x0b
#define MBR_SIGNATURE 0xaa55                // on disk, .. .. 55 aa


#define FAT32_VOLUME_SERIAL_OFFSET 0x43     // Starting word for Volume Serial. Note: starts in high octet of the word.
#define FAT32_SIGNATURE_OFFSET 0x52         // Starting word for "FAT32 "
#define FAT32_LABEL_OFFSET 0x47             // Word where the label starts; starts in the high octet of the word.
#define FAT32_LABEL_LENGTH 11               // 11 characters long

#define FAT32_DUPLICATE_BOOT_SECTOR_OFFSET 6    // Sectors offset to the duplicate boot sector

UINT32 curSector = 0xffffffff; // Value meaning "not any sector"
UINT32 bootSector = 0;         // zero is not a valid boot sector (that's the MBR)
UINT16 sdBufferArray[512] = "";
UINT16 *SDBuffer = sdBufferArray;

extern INT16 SD_ReadSector(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf);
extern INT16 SD_WriteSector(UINT32 blkno , UINT16 blkcnt ,  UINT32 buf);

/**
 * Read a UINT8 value from the SDBuffer. Deals with high-octet / low-octet issues.
 *
 * NOTE: The GP chip doesn't actually support UINT8, so be careful with masking.
 *
 * @param byteOffset - the byte (8-bits) offset in the SDBUffer
 * @return the value at the byte offset. Note that the type is UINT16, because that is what it
 *   really is (UINT8 is typedef'ed to unsigned int, AKA UINT16).
 */
UINT16 getSdUI8(int byteOffset) {
    int wordIndex = byteOffset >> 1;
    int isHighByte = byteOffset & 1; // odd addresses are at high byte
    UINT16 val = SDBuffer[wordIndex];
    if (isHighByte) {
        val = val >> 8;
    }
    return val & 0xff;
}
/**
 * Read a UINT16 by reading the low and high UINT8s.
 */
UINT16 getSdUI16(int byteOffset) {
    UINT16 result = getSdUI8(byteOffset) | (getSdUI8(byteOffset + 1) << 8);
    return result;
}
/**
 * Read a UINT32 by reading the low and high UINT16s.
 */
UINT32 getSdUI32(int byteOffset) {
    UINT32 result = (UINT32)(getSdUI16(byteOffset)) | (((UINT32) getSdUI16(byteOffset + 2)) << 16);
    return result;
}
/**
 * Read a string from the SD buffer. The SD buffer's strings are in ASCII, while the GP chip
 * uses 16-bit characters. Deals with the conversion. No sign extension is performed.
 */
void getSdChars(int byteOffset, int numChars, char *buffer) {
    while (numChars-- > 0) {
        *buffer++ = getSdUI8(byteOffset++);
    }
}
/**
 * Writes an UINT8 value to the SD buffer. Deals with high- and low-octet issues.
 *
 * @param byteOffset - the byte (8-bits) offset in the SD buffer.
 * @param value - the 8-bit value to be written.
 */
void putSdUI8(int byteOffset, UINT8 value) {
    int wordIndex = byteOffset >> 1;
    int isHighByte = byteOffset & 1; // odd addresses are at high byte
    UINT16 val = SDBuffer[wordIndex];
    if (isHighByte) {
        val = (val & 0x00ff) | ((value << 8) & 0xff00);
    } else {
        val = (val & 0xff00) | (value & 0x00ff);
    }
    SDBuffer[wordIndex] = val;
}
/**
 * Writes a UINT16 by writing the low and high UINT8s.
 */
void putSdUI16(int byteOffset, UINT16 value) {
    putSdUI8(byteOffset, (UINT8) (value & 0xff));
    putSdUI8(byteOffset + 1, (UINT8) ((value >> 8) & 0xff));
}
/**
 * Writes a UINT32 by writing the low and high UINT16s.
 */
void putSdUI32(int byteOffset, UINT32 value) {
    putSdUI16(byteOffset, (UINT16) (value & 0xffff));
    putSdUI16(byteOffset + 2, (UINT16) ((value >> 16) & 0xffff));
}
// If we ever need to write a string back to the SD buffer, here it is.
/**
 * Writes a string to the SD buffer. Strings in the SD buffer are ASCII, while strings
 * to the GP chip are 16-bit values.  Note that if any high bits are silently ignored.
 * /
void putSdChars(int byteOffset, int numChars, char *buffer) {
    while (numChars-- > 0) {
        putSdUI8(byteOffset++, *buffer++);
    }
}
*/

/**
 * Reads a sector from the SD card, into a global buffer, SDBuffer.
 *
 * @return 0 if OK, otherwise an undocumented error code.
 */
INT16 readSector(UINT32 sector) {
    INT16 ret = 0;
    if (sector != curSector) {
        ret = SD_ReadSector(sector, 1, (UINT32)SDBuffer);
        if (ret == 0) {
            curSector = sector;
        } else {
            curSector = -1;
        }
    }
    return ret;
}

/**
 * Writes the sector most recent sector read from the SD card, back to its same address.
 *
 * @return 0 if OK, otherwise an undocumented error code.
 */
INT16 writeSector() {
    INT16 ret = -1;
    if (curSector != -1) {
        ret = SD_WriteSector(curSector, 1, (UINT32) SDBuffer);
    }
    return ret;
}

/**
 * Read the MBR and determine the location of the first FAT Boot Sector.
 *
 * @return The boot sector address, or 0 if it can not be determined.
 */
UINT32 findBootSector() {
    int ret;
    int ok = 1;
    UINT32 val;

    if (bootSector) {
        return bootSector;
    }

    ret = readSector(0);
    if (ret != 0) {
        return bootSector; // still 0
    }

    /*
     * The Master Boot Record is sector 0 on the SD card.
     *
     * We validate the indicated fields (0b = FAT32 partition, 55AA is MBR signature)
     *
     *   MBR (8-bit addressing units):
     *   | offset |  0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
     *   |  0x00  | 00 00 .. .. .. .. .. ..  .. .. .. .. .. .. .. ..
     *   | 0x1C0  | .. .. 0b .. .. .. .. ..  .. .. .. .. .. .. .. ..
     *   | 0x1F0  | .. .. .. .. .. .. .. ..  .. .. .. .. .. .. 55 AA
     *
     */

    val = getSdUI16(0);
    ok &= (val == 0);
    val = getSdUI8(MBR_PARTITION_TYPE_OFFSET);
    ok &= (val == MBR_FAT32_PARTITION_CODE);
    val = getSdUI16(MBR_SIGNATURE_OFFSET);
    ok &= (val == MBR_SIGNATURE);

    if (ok) {
        bootSector = getSdUI32(MBR_PARTITION_SECTOR_OFFSET);
    }
    return bootSector;
}

/**
 * Determines whether the given sector is a FAT32 Boot sector.
 *
 * @param sector - The sector in question.
 * @return 1 if the sector is a FAT32 boot sector, 0 if not, or if can not be determined.
 */
int isFat32Sector(UINT32 sector) {
    int ret;
    char sigBuffer[6];
    int ok = 1;

    ret = readSector(sector);
    ok = ok && (ret == 0);

    /*
     * The Volume Serial Number is a UINT32 stored at byte offset 0x43 in the boot sector.
     *
     *   Boot sector (8-bit addressing units):
     *   | offset |  0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
     *   |  0x50  | .. .. 46 41 54 33 32 20  20 20 .. .. .. .. .. .. |  ..FAT32   ...... |
     *
     */

    getSdChars(FAT32_SIGNATURE_OFFSET, 5, sigBuffer);
    sigBuffer[5] = 0;
    ok = strcmp(sigBuffer, "FAT32") == 0;

    return ok;
}

/**
* Reads the Volume Serial Number from a FAT32 Boot Sector.
* @return The volume serial, or 0 if it can not be determined.
*/
UINT32 getVolumeSerialFromFat32(UINT32 sector) {
    int ret;
    UINT32 val;
    UINT32 volumeSerial = 0;

    ret = readSector(sector);
    if (ret != 0) {
        return volumeSerial;
    }

    /*
     * The Volume Serial Number is a UINT32 stored at byte offset 0x43 in the boot sector.
     *
     *   Boot sector (8-bit addressing units):
     *   | offset |  0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
     *   |  0x40  | .. .. .. ll mm nn oo ..  .. .. .. .. .. .. .. .. |
     *
     *   SDBuffer (16-bit addressing units):
     *   | index  | contents |
     *   |  0x20  |   ....   |
     *   |  0x21  |   ll..   |
     *   |  0x22  |   nnmm   |
     *   |  0x23  |   ..oo   |
     *
     *   Volume Serial Number
     *   |  value | oonnmmll |
     */

    volumeSerial = getSdUI32(FAT32_VOLUME_SERIAL_OFFSET);

    return volumeSerial;
}

/**
 * Stores a Volume Serial Number into the sector buffer. Performs no validation.
 * @return 0 if the volume serial was written successfully, an undocumented error code otherwise.
 */
int setVolumeSerialIntoFat32(UINT32 sector, UINT32 volumeSerial) {
    int ret;
    UINT16 val;

    if (ret = readSector(sector)) {
        return ret;
    }

    putSdUI32(FAT32_VOLUME_SERIAL_OFFSET, volumeSerial);

    return writeSector(sector);
}

// It might be nice to set the volume label, if it doesn't match the TB serial number. This is a start at
// code to read/write volume label.
#ifdef SET_VOLUME_LABEL_IMPLEMENTED
/**
 * Reads the Volume Label from a FAT32 Boot Sector.
 * @param sector The sector number of a FAT32 boot sector.
 * @param buffer The nul terminated label is written here.
 * @return 0 if a label was read, an undocumented error otherwise.
 */
int getVolumeLabelFromFat32(UINT32 sector, char *labelBuffer) {
    int ret;

    if (ret = readSector(sector)) {
        return ret;
    }

    getSdChars(FAT32_LABEL_OFFSET, FAT32_LABEL_LENGTH, labelBuffer);
    labelBuffer[FAT32_LABEL_LENGTH] = 0;

    return ret;
}

/**
 * Writes the Volume Label into a FAT32 Boot Sector.
 * @param sector The sector number of a FAT32 boot sector.
 * @param label The to be written.
 * @return 0 if a label was written successfully, an undocumented error otherwise.
 */
int setVolumeLabelIntoFat32(UINT32 sector, char *label) {
    int ret;

    if (ret = readSector(sector)) {
        return ret;
    }

    putSdChars(FAT32_LABEL_OFFSET, FAT32_LABEL_LENGTH, label);

    return writeSector(sector);
}

/**
 * Attempts to set the volume label to the given value.
 */
int setVolumeLabel(char *label) {
    UINT32 addr1, addr2 = 0;
    int ok1 = 0, ok2 = 0;
    char label1[FAT32_LABEL_LENGTH+1], label2[FAT32_LABEL_LENGTH+1], target[FAT32_LABEL_LENGTH+1];

    addr1 = findBootSector();
    if (addr1) {
        if (isFat32Sector(addr1)) {
            ok1 = 1;
            getVolumeLabelFromFat32(addr1, label1);
            addr2 = addr1 + FAT32_DUPLICATE_BOOT_SECTOR_OFFSET;
        }
    }
    if (addr2) {
        if (isFat32Sector(addr2)) {
            ok2 = 1;
            getVolumeLabelFromFat32(addr2, label2);
        }
    }

    if (ok1 && ok2) {
        strcpy(target, label);
        while (strlen(target) < FAT32_LABEL_LENGTH) {
            strcat(target, " ");
        }
        // Do 2 before 1, because we read 2 most recently, so it is cached.
        if (strcmp(label2, target)) {
            setVolumeLabelIntoFat32(addr2, target);
        }
        if (strcmp(label1, target)) {
            setVolumeLabelIntoFat32(addr1, target);
        }
    }
}
#endif

/**
 * Attempts to set the Volume Serial to a predefined value.
 */
int setSDVolumeSerial() {
    UINT32 addr1=0, addr2=0;
    UINT32 vs1=0, vs2=0;
    int ret;
    int updated = 0;

    // Cleans up anything that may have happened to the disk.
    ret = _deviceunmount(0);
    if (!ret) {
        ret = _devicemount(0);
    }

    if (!ret) {
        addr1 = findBootSector();
        if (addr1) {
            if (isFat32Sector(addr1)) {
                vs1 = getVolumeSerialFromFat32(addr1);
                addr2 = addr1 + FAT32_DUPLICATE_BOOT_SECTOR_OFFSET;
            }
        }
        if (addr2) {
            if (isFat32Sector(addr2)) {
                vs2 = getVolumeSerialFromFat32(addr2);
            }
        }

        // If we got both vs numbers, but either is not what we want, update them.
        if (vs1 && vs2 && (vs1 != TARGET_VOLUME_SERIAL || vs2 != TARGET_VOLUME_SERIAL)) {
            // Do 2 before 1, because we read 2 most recently, so it is cached.
            if (vs2 != TARGET_VOLUME_SERIAL) {
                setVolumeSerialIntoFat32(addr2, TARGET_VOLUME_SERIAL);
            }
            if (vs1 != TARGET_VOLUME_SERIAL) {
                // This is the primary boot sector, and thus the most important one.
                ret = setVolumeSerialIntoFat32(addr1, TARGET_VOLUME_SERIAL);
                if (!ret) {
                    updated = 1;
                }
            }
        }

        // Cleans up, again, anything active in the disk system.
        _deviceunmount(0);
        ret = _devicemount(0);

        if (updated) {
            logStringRTCOptional((char *)"Updated Volume Serial Number.", BUFFER, LOG_ALWAYS, 0);
        }
    }

    return ret;
}

