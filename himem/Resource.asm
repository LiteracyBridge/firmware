
// Resource Table
// Created by IDE, Do not modify this table

.CODE
.public _RES_Table;
.public _FS_UniTab_Table;


_RES_Table:


_FS_UniTab_Table:


// End Table
.include .\Component\Include\speech\algorithm.inc
.comment @
.ifdef	MS01_Used	//add by benwang 20060112
.public	T_SACM_MS01_SongTable
.public T_SACM_MS01_DrumTable;        
T_SACM_MS01_DrumTable:
	.dw _RES_BD1_ADP_SA;
	.dw _RES_SD1_ADP_SA;		//1
	.dw _RES_HT1_ADP_SA;		//2
	.dw _RES_CYM1_ADP_SA;		//3
	.dw _RES_CBL_ADP_SA;		//4
	.dw _RES_CBH_ADP_SA; 		//5
	.dw _RES_HAP1_ADP_SA;		//6	
	.dw _RES_JB1_ADP_SA;		//7
	.dw _RES_CC_ADP_SA; 		//8
	//.dw _RES_JB1_ADP_SA; 		//8  //remove CC_ADP
	.dw _RES_CHT1_ADP_SA;		//9
	.dw _RES_OHT1_ADP_SA;		//10
	.dw _RES_PHT1_ADP_SA;		//11
	.dw _RES_HTM_ADP_SA;		//12
	.dw _RES_HFT_ADP_SA;		//13
	.dw _RES_LT_ADP_SA;			//14
	.dw _RES_LFT_ADP_SA;		//15	
	.dw _RES_HB_ADP_SA;			//16
	.dw _RES_LB_ADP_SA;			//17
	.dw _RES_HBQ_ADP_SA			//18
	.dw _RES_LBQ_ADP_SA;		//19
T_SACM_MS01_SongTable:
	.DW offset __RES_DECK1_BIN_sa,seg16 __RES_DECK1_BIN_sa;
.endif   //add by benwang 20060112
//SACM_RES_Table:	.section	.text
.ifdef	S200_Used
.public	T_SACM_S200_SpeechTable
T_SACM_S200_SpeechTable:
//	.dw	offset __RES_LRC_800_S20_sa, seg16 __RES_LRC_800_S20_sa //add by benwang 20051013
.endif
.ifdef	S320TTS_Used          
.public T_SACM_TTS320_DIC_Table
.@
T_SACM_TTS320_DIC_Table: 
.comment @
	.dW offset __RES_YESTERDAY_TTS32_sa,seg16 __RES_YESTERDAY_TTS32_sa;
	.dW offset __RES_ALREADY_TTS32_sa,seg16 __RES_ALREADY_TTS32_sa;
	.dW offset __RES_BASKETBALL_TTS32_sa,seg16 __RES_BASKETBALL_TTS32_sa;
	.dW offset __RES_BEAUTIFUL_TTS32_sa,seg16 __RES_BEAUTIFUL_TTS32_sa;
	.dW offset __RES_BECAUSE_TTS32_sa,seg16 __RES_BECAUSE_TTS32_sa;
	.dW offset __RES_BUY_TTS32_sa,seg16 __RES_BUY_TTS32_sa;
	.dW offset __RES_CAR_TTS32_sa,seg16 __RES_CAR_TTS32_sa;
	.dW offset __RES_CAREFUL_TTS32_sa,seg16 __RES_CAREFUL_TTS32_sa;
	.dW offset __RES_COLOR_TTS32_sa,seg16 __RES_COLOR_TTS32_sa;
	.dW offset __RES_CRAZY_TTS32_sa,seg16 __RES_CRAZY_TTS32_sa;
	.dW offset __RES_ENJOY_TTS32_sa,seg16 __RES_ENJOY_TTS32_sa;
	.dW offset __RES_EXPERIENCE_TTS32_sa,seg16 __RES_EXPERIENCE_TTS32_sa;
	.dW offset __RES_FRIENDLY_TTS32_sa,seg16 __RES_FRIENDLY_TTS32_sa;
	.dW offset __RES_IMPORTANT_TTS32_sa,seg16 __RES_IMPORTANT_TTS32_sa;
	.dW offset __RES_MAGIC_TTS32_sa,seg16 __RES_MAGIC_TTS32_sa;
	.dW offset __RES_MOON_TTS32_sa,seg16 __RES_MOON_TTS32_sa;
	.dW offset __RES_NIGHT_TTS32_sa,seg16 __RES_NIGHT_TTS32_sa;
	.dW offset __RES_PERHAPS_TTS32_sa,seg16 __RES_PERHAPS_TTS32_sa;
	.dW offset __RES_PERSON_TTS32_sa,seg16 __RES_PERSON_TTS32_sa;
	.dW offset __RES_PLAYER_TTS32_sa,seg16 __RES_PLAYER_TTS32_sa;
	.dW offset __RES_POPULAR_TTS32_sa,seg16 __RES_POPULAR_TTS32_sa;
	.dW offset __RES_POSSIBLE_TTS32_sa,seg16 __RES_POSSIBLE_TTS32_sa;
	.dW offset __RES_SORRY_TTS32_sa,seg16 __RES_SORRY_TTS32_sa;
	.dW offset __RES_SPECIAL_TTS32_sa,seg16 __RES_SPECIAL_TTS32_sa;
	.dW offset __RES_SPORTS_TTS32_sa,seg16 __RES_SPORTS_TTS32_sa;
	.dW offset __RES_STAR_TTS32_sa,seg16 __RES_STAR_TTS32_sa;
	.dW offset __RES_STUDY_TTS32_sa,seg16 __RES_STUDY_TTS32_sa;
	.dW offset __RES_WHETHER_TTS32_sa,seg16 __RES_WHETHER_TTS32_sa;
	.dW offset __RES_WHITE_TTS32_sa,seg16 __RES_WHITE_TTS32_sa;
	.dW offset __RES_WINDOW_TTS32_sa,seg16 __RES_WINDOW_TTS32_sa;
	.dW offset __RES_WORLD_TTS32_sa,seg16 __RES_WORLD_TTS32_sa;
	.dW offset __RES_BOTTOM_TTS32_sa,seg16 __RES_BOTTOM_TTS32_sa;
	.dW offset __RES_BOTTOM_TTS32_sa,seg16 __RES_BOTTOM_TTS32_sa;
@	
.comment @
.endif
.ifdef	S530_Used
.public	T_SACM_S530_SpeechTable
T_SACM_S530_SpeechTable:
//	.dw offset __RES_S530_HELLO_BIN_sa, seg16 __RES_S530_HELLO_BIN_sa
.endif
.ifdef	MS02_Used
.public	T_SACM_MS02_SpeechTable
T_SACM_MS02_SpeechTable:
//	.dw	offset __RES_MB_BIN_sa, seg16 __RES_MB_BIN_sa
.endif
.ifdef	SPEECH1600_Used
.public	T_SACM_SPEECH1600_SpeechTable
T_SACM_SPEECH1600_SpeechTable:
//	.dw	offset __RES_ENG_A16_sa, seg16 __RES_ENG_A16_sa //add by benwang 2005/07/26
.endif
.ifdef	AUDIO1600_Used
.public	T_SACM_AUDIO1600_SpeechTable
T_SACM_AUDIO1600_SpeechTable:
//	.dw	offset __RES_OUT_HASC_2_ENC_sa, seg16 __RES_OUT_HASC_2_ENC_sa //add by benwang 2005/07/26
.endif
.ifdef	SPEECH4800_Used
.public	T_SACM_SPEECH4800_SpeechTable
T_SACM_SPEECH4800_SpeechTable:
//	.dw	offset __RES_BS_PIANO_ENC_sa, seg16 __RES_BS_PIANO_ENC_sa
.endif
.ifdef	AUDIO4800_Used
.public	T_SACM_AUDIO4800_SpeechTable
T_SACM_AUDIO4800_SpeechTable:
//	.dw	offset __RES_MUSICOLD_A48_sa, seg16 __RES_MUSICOLD_A48_sa;
.endif
.ifdef	MP3_Used
.public	T_SACM_MP3_MusicTable
T_SACM_MP3_MusicTable:
//	.dw	offset __RES_CLASSIC122JS_MP3_sa,seg16 __RES_CLASSIC122JS_MP3_sa;
//	.dw	offset __RES_CLASSIC122JS_MP3_ea,seg16 __RES_CLASSIC122JS_MP3_ea;	
.endif
.ifdef	AUDIO1601_Used
.public	T_SACM_A1601_SpeechTable
T_SACM_A1601_SpeechTable:
//	.dw offset __RES_MUSIC12K_A1601_sa, seg16 __RES_MUSIC12K_A1601_sa 
.endif
.ifdef	A128_Used
.public	T_SACM_A128_MusicTable
T_SACM_A128_MusicTable:  
	.DW offset __RES_STORY01_A64_sa,seg16 __RES_STORY01_A64_sa;
	.DW offset __RES_STORY01_A64_ea,seg16 __RES_STORY01_A64_ea;	
.endif
.ifdef DVR1800_Used
.public  T_SACM_DVR1800_SpeechTable
T_SACM_DVR1800_SpeechTable:
//	.DW offset __RES_DUNK2_16_A18_sa,seg16 __RES_DUNK2_16_A18_sa;
	.DW offset __RES_ENTERTHEGAMECODE_A18_sa,seg16 __RES_ENTERTHEGAMECODE_A18_sa;
.endif
.ifdef AUDIO3200_Used
.public T_SACM_AUDIO3200_SpeechTable
T_SACM_AUDIO3200_SpeechTable:
//	.DW offset __RES_HHSZ_A32_sa,seg16 __RES_HHSZ_A32_sa;
.endif
 
.ifdef DVR520_Used
.public T_SACM_DVR520_SpeechTable
T_SACM_DVR520_SpeechTable:
	.DW offset __RES_0503B_S52_sa,seg16 __RES_0503B_S52_sa;
.endif
.ifdef S320_Used
.public T_SACM_S320_SpeechTable
T_SACM_S320_SpeechTable:
	.DW offset __RES_AFTERNOON_S32_sa,seg16 __RES_AFTERNOON_S32_sa;
	.DW offset __RES_AIRPORT_S32_sa,seg16 __RES_AIRPORT_S32_sa;
.endif
.ifdef	S880_Used
.public	T_SACM_AMRWB_SpeechTable
T_SACM_AMRWB_SpeechTable:
	.DW offset __RES_FEMALE_8_COD_sa,seg16 __RES_FEMALE_8_COD_sa;
.endif
.@
.END
