// Copyright 2009 Literacy Bridge
// CONFIDENTIAL -- Do not share without Literacy Bridge Non-Disclosure Agreement
// Contact: info@literacybridge.org
#ifndef	__MACRO_h__
#define	__MACRO_h__

typedef struct MacroInstr MacroInstr;
typedef struct MacroLoop MacroLoop;

struct MacroInstr{
	unsigned int wait;
	unsigned int key;
	char log;
};

struct MacroLoop {
	unsigned int times;
	unsigned int begin;
	unsigned int end;
};

extern void loadMacro(void);
extern int nextMacroKey (int);

#endif
