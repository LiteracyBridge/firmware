#include "vfs.h"

extern const struct nls_table	nls_ascii_table;
//extern const struct nls_table	nls_arabic_table;
//extern const struct nls_table	nls_cp936_table;
//extern const struct nls_table	nls_cp950_table;
//extern const struct nls_table	nls_cp932_table;

extern UINT16			gUnicodePage;

UINT16 ChangeCodePage(UINT16 wCodePage)
{
	INT16	ret = -1;
	
	gUnicodePage = wCodePage;
	
	switch(wCodePage)
	{
		case UNI_ENGLISH:
			ret = ChangeUnitab((struct nls_table *)&nls_ascii_table);
			break;
//		case UNI_ARABIC:
//			ret = ChangeUnitab((struct nls_table *)&nls_arabic_table);
//			break;
//		case UNI_GBK:
//			ret = ChangeUnitab((struct nls_table *)&nls_cp936_table);
//			break;
//		case UNI_BIG5:
//			ret = ChangeUnitab((struct nls_table *)&nls_cp950_table);
//			break;
//		case UNI_SJIS:
//			ret = ChangeUnitab((struct nls_table *)&nls_cp932_table);
//			break;
		default:
			break;
	}
	
	if(ret != 0)
	{	
		gUnicodePage = UNI_ENGLISH;
		ChangeUnitab((struct nls_table *)&nls_ascii_table);
	}
	
	return gUnicodePage;
}

UINT16 GetCodePage(void)
{
	return gUnicodePage;
}
