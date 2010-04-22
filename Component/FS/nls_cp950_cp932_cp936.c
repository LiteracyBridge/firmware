#include "vfs.h"
extern unsigned long _RES_UNITAB_GBK_BIG5_BIN_sa;

#define WORD_PACK_VALUE		1

static	unsigned long	pUniTabAdd;
static	unsigned long	pUniToCharTab;		//UniToCharTab在整个unitab中的偏移量
static	unsigned long	pCharToUniTab;		//CharToUniTab在整个unitab中的偏移量

unsigned long fs_read_dword(unsigned long p);
unsigned long fs_read_word(unsigned long p);

unsigned long get_unitab_add(void)
{
	return (unsigned long)&_RES_UNITAB_GBK_BIG5_BIN_sa;
}

static unsigned long GetUni2OEMDataIndex(unsigned short index)
{
	if(pUniToCharTab == 0)
		return 0;
	
	return fs_read_dword(pUniToCharTab + index * (4 >> WORD_PACK_VALUE)); 
}

static unsigned long GetOEM2UniDataIndex(unsigned short index)
{
	if(pCharToUniTab == 0)
		return 0;
	
	return fs_read_dword(pCharToUniTab + index * (4 >> WORD_PACK_VALUE)); 
}

static unsigned short GetUniTabData(unsigned long index)
{
	return fs_read_dword(pUniTabAdd + index * (2 >> WORD_PACK_VALUE));
}

static short  cpxxx_init_nls(unsigned int wCodePage)
{
	unsigned long Flag1 , Flag2;
	
	pUniTabAdd = get_unitab_add();
	if(pUniTabAdd == 0)
		return -1;
	
	pCharToUniTab = fs_read_dword(pUniTabAdd + (8 >> WORD_PACK_VALUE) * wCodePage);
	pUniToCharTab = fs_read_dword(pUniTabAdd + (4 >> WORD_PACK_VALUE) + (8 >> WORD_PACK_VALUE) * wCodePage);
	Flag1 = fs_read_dword(pUniTabAdd + 15 * (8 >> WORD_PACK_VALUE));
	Flag2 = fs_read_dword(pUniTabAdd + 15 * (8 >> WORD_PACK_VALUE) + (4 >> WORD_PACK_VALUE));
	
	if((Flag1 != 0x1001) || (Flag2 != 0x55aa) || (pCharToUniTab <= 0) || (pUniToCharTab <= 0))
	{
		pCharToUniTab = 0;
		pUniToCharTab = 0;
		return -1;
	}
	pCharToUniTab = pUniTabAdd + pCharToUniTab * (2 >> WORD_PACK_VALUE);
	pUniToCharTab = pUniTabAdd + pUniToCharTab * (2 >> WORD_PACK_VALUE);
	return 0;
}

static short cpxxx_exit_nls(void)
{
//	ReleaseUniTabData();
	pUniToCharTab = 0;		//UniToCharTab在整个unitab中的偏移量
	pCharToUniTab = 0;		//CharToUniTab在整个unitab中的偏移量
	return 0;
}

// returns the character from unicode
// return 0 is error
static unsigned short cpxxx_uni2char(unsigned short uni)
{
	unsigned char cl = uni&0xFF;
	unsigned char ch = (uni>>8)&0xFF;
	unsigned long idx;	
	
	if((unsigned short)uni < (unsigned short)0x80 )	// 遇到ASCII码,不用查表直接返回!!
	{	
		return uni;
	}
	
	idx = GetUni2OEMDataIndex(ch);
	if(idx)
		return GetUniTabData(cl+idx);
	else if((ch == 0) && cl)
		return cl;
	else
		return 0;
}

// returns the unicode from character
// return 0 is error
static unsigned short cpxxx_char2uni(unsigned char **rawstring)
{
	unsigned char	ch , cl;
	unsigned short	uni;
	unsigned long	idx;
	unsigned char	*temp = *rawstring;
	
	ch = temp[0];
	cl = temp[1];
	
#ifdef unSP
	if( (ch>0xff) || (cl>0xff) )        //2007.1.25 to avoid invalid data 
    { 
		return 0;					//error 
    } 
#endif
	
	idx = GetOEM2UniDataIndex(ch);
	
	if (idx && cl)
	{
		uni = GetUniTabData(cl+idx); 		
		*rawstring += 2;				//the char point increase 2
	} 
	else
	{
		uni = ch;
		*rawstring += 1;				//the char point increase 1
	}
	
	return uni; 		 //success
}

static short  cp936_init_nls(void)
{
	return  cpxxx_init_nls(UNI_GBK);
}

static short  cp950_init_nls(void)
{
	return  cpxxx_init_nls(UNI_BIG5);
}

static short  cp932_init_nls(void)
{
	return  cpxxx_init_nls(UNI_SJIS);
}

//GBK/cp936
const struct nls_table nls_cp936_table = {
	"GBK",
	0, 
	cp936_init_nls,
	cpxxx_exit_nls,
	cpxxx_uni2char,
	cpxxx_char2uni,
};

//BIG5/cp950
const struct nls_table nls_cp950_table = {
	"BIG5",
	0, 
	cp950_init_nls,
	cpxxx_exit_nls,
	cpxxx_uni2char,
	cpxxx_char2uni,
};

//SJIS/cp932
const struct nls_table nls_cp932_table = {
	"SJIS",
	0, 
	cp932_init_nls,
	cpxxx_exit_nls,
	cpxxx_uni2char,
	cpxxx_char2uni,
};

