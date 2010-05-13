#define STAT_OPEN       0
#define STAT_CLOSE      1
#define STAT_TIMEPLAYED 2

#define STAT_DIR "a:\\stats\\" 

void recordStats(char *filename, unsigned long handle, unsigned int why, unsigned long data);

struct ondisk_filestats {
	unsigned long stat_num_opens;
	unsigned long stat_num_completions;
};

#define STATSIZE (sizeof(struct ondisk_filestats) << 1) 
