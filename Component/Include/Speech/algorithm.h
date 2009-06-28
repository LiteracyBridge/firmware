#ifndef	__ALGORITHM_h__
#define	__ALGORITHM_h__
//	write your header here



//#define S200_Used					1
//#define S4872_Used 				1		//162002_001 not support
//#define S530_Used					1
//#define	S600TTS_Used			1		//162002_001 not support
//#define	S200TTS_Used			1		//162002_001 not support
//#define	S320TTS_Used			1		//162002_001 not support
//#define	MS01_Used					1
//#define	MS02_Used					1
//#define SPEECH1600_Used				1
//#define	AUDIO1600_Used			1 		//162002_001 not support
//#define	SPEECH4800_Used			1		//162002_001 not support
//#define	AUDIO4800_Used				1
//#define A6400S_Used				1		//162002_001 not support
//#define   AUDIO3200_Used          1 		//162002_001 not support
//#define DVR520_Used				1		//162002_001 not support
//#define MP3_Used					1
//#define AUDIO1601_Used 			    1
//#define A128_Used                   1
#define DVR1800_Used				1 		//add by benwang 20060621
//#define WAV_Used 					1
//#define ETTS_Used				1 			//add by chengye 2007/4/18
//#define	S320_Used						//added by chengye 2007/10/30

#ifndef S320_Used
	#ifdef S320TTS_Used
		#define S320_Used
	#endif	//S320TTS_Used
#endif	//S320_Used	
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
#define C_CODEC_S200 		0

#ifdef	S200_Used
#define C_CODEC_S4872 		C_CODEC_S200+1		 
#else	
#define	C_CODEC_S4872	 	C_CODEC_S200+0		
#endif

#ifdef	S4872_Used		
#define C_CODEC_S530 		C_CODEC_S4872+1 
#else
#define	C_CODEC_S530 		C_CODEC_S4872+0
#endif

#ifdef	S530_Used
#define C_CODEC_S600_TTS	C_CODEC_S530+1
#else
#define	C_CODEC_S600_TTS	C_CODEC_S530+0
#endif

#ifdef	S600TTS_Used
#define C_CODEC_S200_TTS	C_CODEC_S600_TTS+1	 
#else
#define	C_CODEC_S200_TTS	C_CODEC_S600_TTS+0
#endif

#ifdef	S200TTS_Used
#define C_CODEC_S320_TTS	C_CODEC_S200_TTS+1 
#else
#define	C_CODEC_S320_TTS	C_CODEC_S200_TTS+0
#endif

#ifdef	S320TTS_Used
#define C_CODEC_MS01 		C_CODEC_S320_TTS+1
#else
#define	C_CODEC_MS01		C_CODEC_S320_TTS+0
#endif

#ifdef	MS01_Used
#define C_CODEC_MS02 		C_CODEC_MS01+1
#else
#define	C_CODEC_MS02		C_CODEC_MS01+0
#endif

#ifdef	MS02_Used	 
#define	C_CODEC_SPEECH1600	C_CODEC_MS02+1
#else
#define	C_CODEC_SPEECH1600	C_CODEC_MS02+0
#endif

#ifdef	SPEECH1600_Used
#define	C_CODEC_AUDIO1600	C_CODEC_SPEECH1600+1
#else
#define	C_CODEC_AUDIO1600	C_CODEC_SPEECH1600+0
#endif

#ifdef	AUDIO1600_Used
#define	C_CODEC_SPEECH4800	C_CODEC_AUDIO1600+1
#else
#define	C_CODEC_SPEECH4800	C_CODEC_AUDIO1600+0
#endif	

#ifdef	SPEECH4800_Used
#define	C_CODEC_AUDIO4800	C_CODEC_SPEECH4800+1
#else
#define	C_CODEC_AUDIO4800	C_CODEC_SPEECH4800+0
#endif

#ifdef AUDIO4800_Used								//added by zgq on 20051215
#define C_CODEC_A6400S	C_CODEC_AUDIO4800 + 1	//added by zgq on 20051215
#else											//added by zgq on 20051215
#define C_CODEC_A6400S	C_CODEC_AUDIO4800 + 0	//added by zgq on 20051215
#endif											//added by zgq on 20051215

#ifdef A6400S_Used								//added by benwang 20051220
#define C_CODEC_AUDIO3200	C_CODEC_A6400S + 1	//added by benwang 20051220
#else											//added by benwang 20051220
#define C_CODEC_AUDIO3200	C_CODEC_A6400S + 0	//added by benwang 20051220
#endif											//added by benwang 20051220

#ifdef AUDIO3200_Used								//added by zgq on 20051220
#define C_CODEC_DVR520 C_CODEC_AUDIO3200 + 1		//added by zgq on 20051220
#else											//added by zgq on 20051220
#define C_CODEC_DVR520 C_CODEC_AUDIO3200 + 0		//added by zgq on 20051220
#endif											//added by zgq on 20051220

#ifdef DVR520_Used								//added by benwang 20060111
#define C_CODEC_MP3	C_CODEC_DVR520 + 1	//added by benwang 20060111
#else											//added by benwang 20060111
#define C_CODEC_MP3	C_CODEC_DVR520 + 0	//added by benwang 20060111
#endif											//added by benwang 20060111

#ifdef MP3_Used								//added by benwang 20060119
#define C_CODEC_A1601	C_CODEC_MP3 + 1		//added by benwang 20060119
#else										//added by benwang 20060119
#define C_CODEC_A1601	C_CODEC_MP3 + 0		//added by benwang 20060119
#endif										//added by benwang 20060119

#ifdef AUDIO1601_Used							//added by benwang 20060221
#define C_CODEC_A128	C_CODEC_A1601 + 1		//added by benwang 20060221
#else											//added by benwang 20060221
#define C_CODEC_A128	C_CODEC_A1601 + 0		//added by benwang 20060221
#endif	

#ifdef A128_Used								//added by benwang 20060621
#define C_CODEC_AUDIO1800	C_CODEC_A128 + 1		
#else											
#define C_CODEC_AUDIO1800	C_CODEC_A128 + 0		
#endif

#ifdef DVR1800_Used								//added by chengye 2007/4/18
#define C_CODEC_WAV		C_CODEC_AUDIO1800 + 1		
#else											
#define C_CODEC_WAV		C_CODEC_AUDIO1800 + 0		
#endif

#ifdef WAV_Used								//added by chengye 2007/4/18
#define C_CODEC_ETTS		C_CODEC_WAV + 1		
#else											
#define C_CODEC_ETTS		C_CODEC_WAV + 0		
#endif

#ifdef ETTS_Used								//added by chengye 2007/4/18
#define C_CODEC_S320		C_CODEC_ETTS + 1		
#else											
#define C_CODEC_S320		C_CODEC_ETTS + 0		
#endif
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#define	C_SpeedControl	1 //add by benwang 20060120
#define FARRELEASE		1 //add by chengye 2007/4/18
#define MP3_FATREADBUFFER //要使用MP3FATBuffer的宏标志

#endif
