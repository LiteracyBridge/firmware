// Description: Support long pointer for near pointer compiler
// Author     :J.K.Chen and Taylor Wang
// Date       :2004/3/15
////////////////////////////////////////////////////////////////
#ifndef	__UNEXTMEM_H__
#define	__UNEXTMEM_H__

#define LPCONST __attribute__((section(".code")))

#define GETSEG(label)	({					\
	register unsigned int __t;							\
	__asm__ __volatile__ ("%0 = seg %1"		\
		:"=r"(__t)							\
		:"p"(&label)						\
		);									\
	__t;									\
	})
#define GETHI6(label)	({					\
	register unsigned int __t;							\
	__asm__ __volatile__ ("%0 = high6 %1"	\
		:"=r"(__t)							\
		:"p"(&label)						\
		);									\
	__t;									\
	})
#define LB2LP(label)					(unsigned long)label
/*
	({					\
	register union {						\
		unsigned long oval;					\
		unsigned int ival[2];				\
	}__t;									\
	__asm__ ("%0 = offset %2\n"				\
	"\t%1 = seg %2"							\
	:"=r"(__t.ival[0]),"=r"(__t.ival[1])		\
	:"p"(&label)								\
	);										\
	__t.oval;								\
})
*/
#define SP2LP(b) \
	(LPTR)(UINT) b
#define GLPSTR(content)	({					\
	static LPCONST unsigned int __farstr[] = content;	\
	LB2LP(__farstr);						\
})

#define T_LHEAD(name)						\
	__asm__ (".public _"###name##"\n"		\
		".code\n"							\
		"_"###name##":"						\
		);

#define T_LLIST(element)					\
		__asm__(".dw offset _"###element	\
		",seg _"###element);

void WVUL(unsigned long paddr,unsigned long value);
unsigned long RVUL(unsigned long paddr);
void WVUI(unsigned long paddr, int value);
unsigned int RVUI(unsigned long paddr);
void WLPUL(unsigned int seg,unsigned int offset,unsigned long value);
unsigned long RLPUL(unsigned int seg,unsigned int offset);
void WLPUI(unsigned int seg,unsigned int offset, int value);
unsigned int RLPUI(unsigned int seg,unsigned int offset);

#endif
	