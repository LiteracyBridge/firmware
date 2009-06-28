#include ".\Component\FS\usb_host\USB_Host_Constant.h"
#include ".\Component\FS\usb_host\USBHostMSDC.h"
#include ".\System\Include\System\GPL162002.h"
#include "./component/include/component_head.h"

void USBHLongDelay(unsigned int delay);
void TurnON_IOD15();
void TurnOFF_IOD15();
void TurnON_IOD15(void);
void TurnOFF_IOD15(void);


//int	g_music_fd;
//unsigned long g_music_buf_ptr;
//int g_music_filename_idx;
//int g_play_finish;
//unsigned int g_ring_ready_cnt;
//unsigned int g_ring_play_cnt;
//unsigned int g_file_op_semaphore;

char DiskVolume[6][5];
int Key_Flag;
int  CurrentDiskVolume;
unsigned long g_music_buf_ptr;
int	g_music_fd;
int g_music_filename_idx;
struct f_info g_music_finfo; // to keep slide show file inof
int g_play_finish;
int g_music_buf_ready_pos;
int g_music_buf_play_pos;
int g_mp3_timeout_flag;
unsigned int g_ring_ready_cnt;
unsigned int g_ring_play_cnt;
unsigned int g_file_op_semaphore;


#define MusicBufferReadSize 0x200
#define MusicBufferSize 0x2000   

extern  USBHost_Flag;

void USBHostEnter()
{
	unsigned long size;
	int ret,fd,i;
	char filename[128];
	int pre_state = -1; // -1: initial, 0: play sd, 1: play usb host
//	struct	sfn_info file_info; 
	struct f_info file_info;
	 f_pos	*fppos; 
	do{
		if(USBHost_Flag == 0x01){
			size = DrvUSBHGetSize(0);
			//CurrentDiskVolume = 2;
			//fppos = getfirstfile(CurrentDiskVolume, "mp3", &file_info, D_ALL);        //find the first file 
			chdir("b:\\");
			ret =_findfirst((unsigned long)("*.mp3"), &file_info, D_ALL);
			fd = open(file_info.f_name,D_RDONLY);
			ret = read(fd, (unsigned long)filename<<1, 200);
			close(fd);
			if(ret < 0 )
				while(1);
			strcpy(filename,"b:\\wenjian");
			ret = mkdir((LPSTR)filename);
			if(ret < 0 )	
				chdir((LPSTR)filename);		
		}
	}while(i++ < 10);
	_deviceunmount(1);
	*P_USBH_Config = 0;
	/*
	while(1)
	{
			//Key_Flag = SP_GetCh();
		switch(Key_Flag)
		  {	
			case 0x00:
				break;
				
			case 0x01: //play mp3 from u-disk
 				Key_Flag = 0;
				if (pre_state != 1)
					g_music_filename_idx = 1;
				SACM_Stop();
				g_play_finish = 0;
				ret = chdir((unsigned long)(unsigned int)DiskVolume[CurrentDiskVolume]);
				if (ret >= 0)
					OpenMP3File();
				else
				{
					Key_Flag = 0x1;
					g_music_filename_idx = 1;
					CurrentDiskVolume++;
					CurrentDiskVolume = CurrentDiskVolume > 5 ? 0 : CurrentDiskVolume;
				}
				pre_state = 1;
				break;		
			
			case 0x02:// play mp3 from sd
				Key_Flag = 0;
				if (pre_state != 0)
					g_music_filename_idx = 1;
				SACM_Stop();
				g_play_finish = 0;
				CurrentDiskVolume = 0;
				ret = chdir((unsigned long)(unsigned int)DiskVolume[CurrentDiskVolume]);
				if (ret >= 0)
					OpenMP3File();
				pre_state = 0;
				break;
			default:
				break;
			}
			ReadMusicFile_ServiceLoop();
		//	System_ServiceLoop();									// Keyboard Scan			
			USB_Host_ServiceLoop();
	}
	*/		
}
/*
void USB_Host_ServiceLoop()
{
	int i;
	int j;
	int mount_ok=0;

	if (GetUSBHPlugStatus() == USBHPlugStatus_PlugIn)
	{
		TurnON_IOD15();
		for (j=0; j<3; j++)
		{
			for (i=0; i<4; i++)
				if (_devicemount(i+2) == 0)
					mount_ok = 1;
			if (mount_ok)
				break;
			else
				USBHLongDelay(0x10);
		}
		SetUSBHPlugStatus(USBHPlugStatus_None);
	}
	if (GetUSBHPlugStatus() == USBHPlugStatus_PlugOut)
	{
		TurnOFF_IOD15();
		for (i=0; i<4; i++)
		{
			disk_safe_exit(i+2);
   			_deviceunmount(i+2);
		}
		SetUSBHPlugStatus(USBHPlugStatus_None);
	}
}

void TurnON_IOD15(void)
{
	int temp;
	temp = *P_IOD_Buffer;
	temp |= 0x8000;
	*P_IOD_Buffer = temp;
}

void TurnOFF_IOD15(void)
{
	int temp;
	temp = *P_IOD_Buffer;
	temp &= 0x7ffff;
	*P_IOD_Buffer = temp;
}
/*
int OpenMP3File()
{
	int ret,err;
	unsigned long buf_ptr;
	struct	sfn_info file_info; 
    f_pos	*fppos; 
	int	g_music_fd;
//	g_music_buf_ready_pos = 0;
//	g_music_buf_play_pos = 0;
	
	if (g_music_fd >= 0)
		close(g_music_fd);

	if (g_music_filename_idx == 1) // first one
	{
    	fppos = getfirstfile(CurrentDiskVolume, "mp3", &file_info, D_ALL);        //find the first file 
    	if(fppos == NULL) 
    	{
			Key_Flag = 0x1;
			g_music_filename_idx = 1;
			CurrentDiskVolume++;
			CurrentDiskVolume = CurrentDiskVolume > 5 ? 0 : CurrentDiskVolume;
    		return -1; 
    	}
        g_music_fd = sfn_open(fppos);
	}
	else
	{
		fppos = getnextfile(CurrentDiskVolume, "mp3", &file_info, D_ALL);
		if(fppos == NULL)
		{
    		fppos = getfirstfile(CurrentDiskVolume, "mp3", &file_info, D_ALL);        //find the first file 
    		err = _getfserrcode();
        	g_music_fd = sfn_open(fppos);
			Key_Flag = 0x1;
			g_music_filename_idx = 1;
			CurrentDiskVolume++;
			CurrentDiskVolume = CurrentDiskVolume > 5 ? 0 : CurrentDiskVolume;
    		return -1; 
		}
		else
		{
        	g_music_fd = sfn_open(fppos);
		}
	}

	if (g_music_fd >= 0)
	{
		g_ring_ready_cnt=0;
		g_ring_play_cnt=0;
		id3_parse(g_music_fd);
		g_music_filename_idx++;
		buf_ptr = (unsigned long)g_music_buf_ptr;
		ret = read(g_music_fd, buf_ptr << 1, MusicBufferSize*2);
		USER_SetStartAddr2((unsigned long)buf_ptr);
		USER_SetFileLength((unsigned long)(file_info.f_size>>1));
		if (SACM_MP3_SetFS() == 1) //fail
		{
			close (g_music_fd);
			g_music_fd = -1;
			Key_Flag = 0x1;
			return -1;
		}
		SACM_Play(Manual_Mode_Index,DAC1+DAC2,0);	 //Manual_Mode_Index=-1	
		g_play_finish = 0;
		if (ret < MusicBufferSize*2)
		{
			close (g_music_fd);
			g_music_fd = -1;
		}
	}
	return -1;
}
*/
/*
void ReadMusicFile_ServiceLoop()
{
	int ret;
	unsigned long buf_ptr;
	int i;
	int sacm_status;
	
	if (g_music_fd < 0) return;
	
	if (g_file_op_semaphore) return;
	g_file_op_semaphore = 1;
	
	if (g_ring_play_cnt <= g_ring_ready_cnt)
	{	
		if ((g_music_buf_play_pos > g_music_buf_ready_pos) && 
	    	(g_music_buf_play_pos - g_music_buf_ready_pos < MusicBufferReadSize))
	    {
	    	g_file_op_semaphore = 0;
			return;
	    }
	}

	if (g_ring_play_cnt > g_ring_ready_cnt+1)
	{
		for (;g_ring_play_cnt <= g_ring_ready_cnt + 1;)
		{
			TurnON_IOD15();
			lseek(g_music_fd, MusicBufferSize*2, SEEK_CUR);
			g_ring_ready_cnt++;
		}
	}
		
	
	for (i=0;;i++)
	{
		if (g_ring_play_cnt <= g_ring_ready_cnt)
			if ((g_music_buf_play_pos >= g_music_buf_ready_pos) && 
		    	(g_music_buf_play_pos - g_music_buf_ready_pos < MusicBufferReadSize))
				break;
		
		
		buf_ptr = (unsigned long)(g_music_buf_ptr + g_music_buf_ready_pos);
			
		ret = read(g_music_fd, buf_ptr << 1, MusicBufferReadSize*2);
	
		if (ret == -1)
		{
			SACM_Stop();
			g_play_finish = 0;
			close(g_music_fd);
			g_music_fd = -1;
			break;
		}
		
		g_music_buf_ready_pos += MusicBufferReadSize;
		
		if (g_music_buf_ready_pos == MusicBufferSize)
		{
			g_music_buf_ready_pos = 0;
			g_ring_ready_cnt++;
		}
		
		if (ret < MusicBufferReadSize*2)
		{
			close(g_music_fd);
			g_music_fd = -1;
			break;
		}
	}
	
	sacm_status = SACM_Status();
	if (sacm_status & 0x4) //pause
	{
		SACM_Resume();
	}
	g_file_op_semaphore=0;
}
*/