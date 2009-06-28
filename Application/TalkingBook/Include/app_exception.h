#ifndef	__APP_EXCEPTION_h__
#define	__APP_EXCEPTION_h__

#define RESET					1
#define USB_MODE				2
#define ERROR_LOG_FILE			"a:\\\\error\\error.txt"

void logException(unsigned int, const char *, int);

#endif
