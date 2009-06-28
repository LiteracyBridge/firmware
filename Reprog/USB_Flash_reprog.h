#ifndef	__USB_FLASH_REPROG_h__
#define	__USB_FLASH_REPROG_h__
//	write your header here

void EraseSector();
void EndFlashProg();
void NanoPause();
void EraseSector();
int USB_Flash_init();
void WriteWord();
void MemCopy();

#ifdef USBRP
#define N_USB_LUN 3
#else
#define N_USB_LUN 1
#endif

#define BASEADDR 0x30000   // base address of nor flash
#define ENDADDR  0xaffff   // end address of nor flash
#define REPROG_HIMEM  0xac000  // reprogramming high memory
#define FLASH_SECTOR_SIZE 0x800 // 2048 words per flash page
//#define FLASH_APP_ADDR 0x80000  // if want to reprog app only 
#define FLASH_APP_ADDR BASEADDR  // reprog all flash 

typedef struct  {
		int fileHandle;
		int bytesAvail;
/*		int slen;
		int slenidx;
		int addridx;
		int ndata;
		int nbuf;
		int dataidx;
		int ncur;
		int flash_err;
		int ckerr;
		int inbyte;
		int ret;
		int prev_tries;

		unsigned char buffer[READ_LENGTH+1];
		unsigned int curbyte;
		unsigned int buf[READ_LENGTH+1];
		unsigned int  state;
		unsigned int  cksum;
		unsigned int  ckbytes;
		unsigned int  ckfinal;
		unsigned int  datashift[4];
*/		
		union {
			unsigned int *pflash;
			unsigned long lflash;
			unsigned int addr[2];
		} au;
		unsigned int *pflash;
		void *flash_exe_buf;
//		StateFunc statefunc[S_MAX+1];
		void (*nanopause)();
		void (*erasesector)();
		void (*writeword)();
		void (*memcopy)();
//		int (*flashreprog)();
//		void (*newcurrent)();
//		void (*reprogfailed)();
//		void (*newbyte)();
//		void (*readbytes)();
} flash;




#endif
