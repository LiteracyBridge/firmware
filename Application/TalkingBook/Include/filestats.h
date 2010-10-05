#define STAT_OPEN       0
#define STAT_CLOSE      1
#define STAT_TIMEPLAYED 2
#define STAT_COPIED     3

#define STAT_DIR  "a:\\stats\\" 
#define OSTAT_DIR "a:\\ostat\\"

void recordStats(char *filename, unsigned long handle, unsigned int why, unsigned long data);

struct ondisk_filestats {
	unsigned long stat_num_opens;
	unsigned long stat_num_completions;
	unsigned long stat_num_copies;
};

#define STATSIZE (sizeof(struct ondisk_filestats) << 1) 
