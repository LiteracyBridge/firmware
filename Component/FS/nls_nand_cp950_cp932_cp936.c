#include	"vfs.h"
#include	"DriverLayer.h"
//#include	"Nand2_Oper.h"
#include "./driver/include/driver_head.h"

extern INT16	bNandInit;

static	UINT32	pUniToCharTab;			//UniToCharTab在整个unitab中的偏移量
static	UINT32	pCharToUniTab;			//CharToUniTab在整个unitab中的偏移量
//static	UINT32	gUnitabBuffer[512];		//为加速访问UNITAB定义的buffer
static	UINT32	gUnitabBuffer[1024];		//为加速访问UNITAB定义的buffer

UINT32 GetUni2OEMDataIndex(UINT16 index)
{
	if(pUniToCharTab ==0)
		return 0;
	
	return gUnitabBuffer[256 + index];//fs_read_dword(gUnitabBuffer + 512 + index * 2);
}

UINT32 GetOEM2UniDataIndex(UINT16 index)
{
	if(pCharToUniTab == 0)
		return 0;
	
	return gUnitabBuffer[index];//fs_read_dword(gUnitabBuffer + index * 2); 
}

UINT16 GetUniTabData(UINT32 index)
{
	UINT16	value;
	INT32	ret;
	
	ret = (UNITAB_SECTOR_OFFSET * 256 + index) << 1;
//	_Nand_Direct_Read(ret, (UINT32)&value, 2);
	
	return value;
}

static INT16  cpxxx_init_nls(UINT16 wCodePage)
{
	UINT32	Addr;
	UINT32	Flag1 , Flag2;
	extern unsigned long _RES_UNITAB_GBK_BIG5_BIN_sa;	
//	if(bNandInit != 0)
//		return FALSE;
	
	Addr = &_RES_UNITAB_GBK_BIG5_BIN_sa;//UNITAB_SECTOR_OFFSET * 512;
	
//	pCharToUniTab = fs_read_dword(Addr + 4*wCodePage);
//	pUniToCharTab = fs_read_dword(Addr + 2 + 4*wCodePage);
//	Flag1 = fs_read_dword(Addr + 15*4);
//	Flag2 = fs_read_dword(Addr + 15*4 + 2);

//houyin modify add a function "Nor_Direct_Read",read unitab chenge to read from nor flash,by 2007.12.11	
	Nor_Direct_Read(Addr + (8 * wCodePage)/2, &pCharToUniTab, 4/2);
	Nor_Direct_Read(Addr + (8 * wCodePage + 4)/2, &pUniToCharTab, 4/2);
	Nor_Direct_Read(Addr + (15 * 8)/2, (UINT32)&Flag1, 4/2);
	Nor_Direct_Read(Addr + (15 * 8 + 4)/2, (UINT32)&Flag2, 4/2);
	if((Flag1 != 0x1001) || (Flag2 != 0x55aa) || (pCharToUniTab == 0) || (pUniToCharTab == 0))
	{
		pCharToUniTab = 0;
		pUniToCharTab = 0;
		return -1;
	}
	
//houyin modify add a function "Nor_Direct_Read",read unitab chenge to read from nor flash,by 2007.12.11
	Nor_Direct_Read(Addr + pCharToUniTab, (UINT32)gUnitabBuffer, 512/2);
	Nor_Direct_Read(Addr + pCharToUniTab + 512/2, (UINT32)gUnitabBuffer + 256, 512/2);
	Nor_Direct_Read(Addr + pUniToCharTab , (UINT32)gUnitabBuffer + 512, 512/2);
	Nor_Direct_Read(Addr + pUniToCharTab + 512/2 , (UINT32)gUnitabBuffer + 768, 512/2);

	return 0;
}

static INT16 cpxxx_exit_nls(void)
{
//	ReleaseUniTabData();
	pUniToCharTab = 0;		//UniToCharTab在整个unitab中的偏移量
	pCharToUniTab = 0;		//CharToUniTab在整个unitab中的偏移量
	return 0;
}

// returns the character from unicode
// return 0 is error
static UINT16 cpxxx_uni2char(UINT16 uni)
{
	UINT8	cl = uni & 0x00FF;
	UINT8	ch = (uni >> 8) & 0xFF;
	UINT32	idx;
	
	if((UINT16)uni < (UINT16)0x80 )	// 遇到ASCII码,不用查表直接返回!!
	{	
		return uni;
	}
	
	idx = GetUni2OEMDataIndex(ch);
	if(idx)
		return GetUniTabData(cl + idx);
	else if((ch == 0) && cl)
		return cl;
	else
		return 0;
}

// returns the unicode from character
// return 0 is error
static UINT16 cpxxx_char2uni(UINT8 **rawstring)
{
	UINT8	ch , cl;
	UINT16	uni;
	UINT32	idx;
	UINT8	*temp = *rawstring;
	
	ch = temp[0];
	cl = temp[1];
	
#ifdef unSP
	if( (ch > 0xff) || (cl > 0xff) )        //2007.1.25 to avoid invalid data 
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

static INT16  cp936_init_nls(void)
{
	return  cpxxx_init_nls(UNI_GBK);
}

static INT16  cp950_init_nls(void)
{
	return  cpxxx_init_nls(UNI_BIG5);
}

static INT16  cp932_init_nls(void)
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

//houyin add this function for read unitab for ifly,by 2007.12.11
void Nor_Direct_Read(unsigned long LogicAddr, unsigned long TARAddr, unsigned int size)
{
	memcpy(TARAddr,LogicAddr,size);
}
