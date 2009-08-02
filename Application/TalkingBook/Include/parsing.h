#ifndef	__PARSING_h__
#define	__PARSING_h__

#include "containers.h"

#define LANG_CODE_MAX_LENGTH	5
void parseControlFile (char *, CtnrPackage *);
int addTextToPkgHeap (const char *, CtnrPackage *);
#endif
