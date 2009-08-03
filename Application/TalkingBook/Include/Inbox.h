#ifndef	__INBOX_h__
#define	__INBOX_h__

struct newContent {
	char newAudioFileCat  [FILE_LENGTH];
	char newAudioFileName [FILE_LENGTH];
	char newAudioDirCat   [FILE_LENGTH];
	char newAudioDirName  [FILE_LENGTH];
};

extern unsigned int ProcessInbox(struct newContent *);

#endif
