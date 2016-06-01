#ifndef  __UNSP162001_BASE_DATA_TYPEDEF_H__
#define  __UNSP162001_BASE_DATA_TYPEDEF_H__

#define		INT16	int
#define		UINT16	unsigned int
#define 	INT8	char
#define 	UINT8   unsigned char
#define		INT32   long
#define		UINT32  unsigned long


#define		BYTE    INT8
#define		UBYTE   UINT8
#define		WORD    INT16
#define		UWORD	UINT16
#define		DWORD   INT32
#define		UDWORD  UINT32
#define		LONG	INT32
#define		ULONG	UINT32

#define		LPTR	UINT32
#define		LPSTR	UINT32
#define		LPDATA	UINT32

#ifndef		BOOL
#define		BOOL	INT16
#endif

#ifndef		FALSE
#define		FALSE	(1==0)
#endif

#ifndef		TRUE
#define		TRUE	(1==1)
#endif

#ifndef		NULL
#define		NULL	((void*)0)
#endif

#endif		//__TYPEDEF_H__
