#define STAT_OPEN       0
#define STAT_CLOSE      1
#define STAT_TIMEPLAYED 2
#define STAT_COPIED     3
#define STAT_SURVEY1	4
#define STAT_APPLY		5
#define STAT_USELESS	6

#define STAT_DIR  "a:/statistics/stats/" 
#define OSTAT_DIR "a:/statistics/ostats/"

#define CLI_STAT_DIR "b:/statistics/stats/" 
#define CLI_OSTAT_DIR "b:/statistics/ostats/"

#define SNCSV "SN.csv"

void recordStats(char *filename, unsigned long handle, unsigned int why, unsigned long data);

struct ondisk_filestats {
	unsigned long stat_num_opens;
	unsigned long stat_num_completions;
	unsigned long stat_num_copies;
	unsigned long stat_num_survey1;
	unsigned long stat_num_apply;
	unsigned long stat_num_useless;
};

#define STATSIZE (sizeof(struct ondisk_filestats) << 1) 
