
#define SYS_CREATEVAR
#define CREATE_BS_VAR
#define CREATGUIVAR
#define CREATEKEYDECODEVAR

#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"

#if	RTC_Key_TEST
#include "./Application/Include/TestPattern.h"
int RTCTest1=0,RTCTest2=0,KeyTest=0;
void APTest(void);
#endif

void BodyInit(void);
void fs_mount_all(void);

void Nor_WordWrite(unsigned int offset,unsigned int seg, int count,unsigned int *ptr);
void Nor_BlockErase(unsigned int blocknum, int, int);

#define	MX_BLOCK_START	9
#define	MX_BLOCK_END	23
#define	SST_BLOCK_START	2
#define	SST_BLOCK_END	16
#define	SST_MID			0xBF
#define	PROG_ADDR_START	0x40000L
#define	PROG_ADDR_END	0xB0000L
#define	NUM_WRITE_COUNT	((PROG_ADDR_END-PROG_ADDR_START)/BUF_SIZE)

#define	BUF_SIZE	(1024*4)

unsigned int buffer[BUF_SIZE];

int main (void)
{
	int	fd,ret,i,block_start,block_end;
	unsigned long address, progaddress, lRet;

	*P_IOA_Attrib |= 0x1000;
	*P_IOA_Dir |= 0x1000;		//enable SD power
	*P_IOA_Buffer &= ~0x1000;
	
	*P_IOB_Attrib |= 0x0200;
	*P_IOB_Dir |= 0x0200;
	*P_IOB_Buffer &= ~0x0200;
	
	if(SYS_OFF!=SysGetState())
		BodyInit();

	__asm__("irq off");
	__asm__("fiq off");
	
	fs_init();
	
	ret = _devicemount(0);
	if (ret)
		ret = _getfserrcode();
	ChangeCodePage(UNI_ENGLISH);
	
	fd = open("A:\\System2.img" ,  O_RDONLY);
	lseek(fd,0x060000L,SEEK_SET);
	
	ret = Nor_FlashID();
	
	if (ret == SST_MID)
	{
		block_start = SST_BLOCK_START;
		block_end = SST_BLOCK_END;
	}
	else
	{
		block_start = MX_BLOCK_START;
		block_end = MX_BLOCK_END;
	}

	// erase flash
	for (i = block_start; i < block_end; i++)
	{
		*P_IOB_Buffer ^= 0x0200;
		Nor_BlockErase(i,0,1);
	}

	i = NUM_WRITE_COUNT;
	progaddress = PROG_ADDR_START;
	address = buffer;
	address *= 2;
	while (i--)
	{
		*P_IOB_Buffer ^= 0x0200;
		lRet = read (fd, address, BUF_SIZE*2);
		Nor_WordWrite((unsigned int )(progaddress & 0xFFFF), (unsigned int) (progaddress >> 16), BUF_SIZE, buffer);
		progaddress += BUF_SIZE;
		if (lRet < BUF_SIZE*2)
		{
			break;
		}
	}

	close(fd);
//	SoundTest();
//	Nand_SD_USB_Test();
// unpdate finish reset
	while (1);
}

#if 0
void fs_mount_all(void)
{
	unsigned int i,ret,result;

	for(i=0;i<3;i++)
	{
		ret = _devicemount(i);			//加载disk
		result = _getfserrcode();

		if(ret)
		{
			ret = _format(i,FORCE_FAT16_Type);

			if(ret != 0)
			{
				//can not access the disk
				while(1);
			}
		}
	}
	//ChangeCodePage(UNI_GBK); 			//切换unicode为GBK(fs_init后默认为UNI_ENGLISH)
}
#endif

#if	0
int Nand_Init()
{
	int ret,i;

	extern unsigned int gNandBlockNum;
    extern unsigned int IsBadBlock(unsigned long blockaddr);
    extern void SetBadBlock(unsigned long blockaddr);

	ret = _Nand_Initial();
	if(ret)
		return -1;

	ret = InitMapTable();
	if(ret != 0)
	{
		for(i=0;i<gNandBlockNum;i++)
		{
			if(IsBadBlock(i)==0)
			{
				ret	= _Nand_ErasePhysicalBlock(i);
				if(ret&0x0001)
					SetBadBlock(i);
			}
		}
		BuildMapTable();
		ret = InitMapTable();
		if(ret)
			return -2;
	}

	return 0;
}
#endif

void FS_Test(void)
{
	int result,fd,bufferw[10]="@abcdef",bufferr[10];
	long address;

	//test file system
 	fd = open("A:\\Hello.txt" , O_CREAT | O_RDWR);
	result = _getfserrcode();
	address = bufferw;
	address = address*2;
	write (fd, address, 6);
	close (fd);

	fd = open("A:\\Hello.txt" ,  O_RDWR);
	if(1!=lseek (fd, 1, SEEK_SET))
		while(1);
	address = bufferr;
	address = address*2;
	read (fd, address, 2);
	close (fd);
	
	fd = open("A:\\ABC.txt" , O_CREAT | O_RDWR);
	result = _getfserrcode();
	address = bufferw;
	address = address*2;
	write (fd, address, 6);
	close (fd);

	fd = open("A:\\ABC.txt" ,  O_RDWR);
	if(1!=lseek (fd, 1, SEEK_SET))
		while(1);
	address = bufferr;
	address = address*2;
	read (fd, address, 2);
	close (fd);
	
	fd = open("A:\\123.txt" , O_CREAT | O_RDWR);
	result = _getfserrcode();
	address = bufferw;
	address = address*2;
	write (fd, address, 6);
	close (fd);

	fd = open("A:\\123.txt" ,  O_RDWR);
	if(1!=lseek (fd, 1, SEEK_SET))
		while(1);
	address = bufferr;
	address = address*2;
	read (fd, address, 2);
	close (fd);
	
	fd = open("A:\\测试.txt" , O_CREAT | O_RDWR);
	result = _getfserrcode();
	address = bufferw;
	address = address*2;
	write (fd, address, 6);
	close (fd);

	fd = open("A:\\测试.txt" ,  O_RDWR);
	if(1!=lseek (fd, 1, SEEK_SET))
		while(1);
	address = bufferr;
	address = address*2;
	read (fd, address, 2);
	close (fd);
	
	fd = open("A:\\弹道弹道弹道导弹.txt" , O_CREAT | O_RDWR);
	result = _getfserrcode();
	address = bufferw;
	address = address*2;
	write (fd, address, 6);
	close (fd);

	fd = open("A:\\弹道弹道弹道导弹.txt" ,  O_RDWR);
	if(1!=lseek (fd, 1, SEEK_SET))
		while(1);
	address = bufferr;
	address = address*2;
	read (fd, address, 2);
	close (fd);
	
	return 0;
}

#if 0
int Nand_Init()
{
	int ret,i;

	extern unsigned int gNandBlockNum;
    extern unsigned int IsBadBlock(unsigned long blockaddr);
    extern void SetBadBlock(unsigned long blockaddr);

	ret = _Nand_Initial();
	if(ret)
		return -1;

	ret = InitMapTable();
	if(ret != 0)
	{
		for(i=0;i<gNandBlockNum;i++)
		{
			if(IsBadBlock(i)==0)
			{
				ret	= _Nand_ErasePhysicalBlock(i);
				if(ret&0x0001)
					SetBadBlock(i);
			}
		}
		BuildMapTable();
		ret = InitMapTable();
		if(ret)
			return -2;
	}

	return 0;
}
#endif