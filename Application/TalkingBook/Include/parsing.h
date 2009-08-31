#ifndef	__PARSING_h__
#define	__PARSING_h__

#include "containers.h"

#define LANG_CODE_MAX_LENGTH	5
#define APP_QUIZ_PLAY_STR		"QUIZ-PLAY"
#define APP_QUIZ_REC_STR		"QUIZ-REC"


void parseControlFile (char *, CtnrPackage *);
int addTextToPkgHeap (const char *, CtnrPackage *);
#endif
