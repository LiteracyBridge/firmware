
/////////////////////////////////
////////////////////////////////	//S200
.ifdef	S200_Used
.external S200_DEC_INIT
.external S200_DEC_PROC
.external MODE
.external decvar_bit_pack_buffer
.endif

/////////////////////////////////

//add by benwang 20051020
/////////////////////////////////
////////////////////////////////	//S320
.ifdef	S320TTS_Used
.external S320_DEC_INIT
.external S320_DEC_PROC
.external R_S320_SubFrameCounter
.endif

/////////////////////////////////



////////////////////////////////	//S4872
.ifdef	S4872_Used
.external S4872_DEC_INIT
.external S4872_DEC_PROC
.endif

/////////////////////////////		//S530
////////////////////////////
.ifdef	S530_Used
.external S530_DEC_INIT
.external S530_DEC_PROC
.external SubFrameNO
.endif

.ifdef MS01_Used
.external MS01_Dec_Init
.external MS01_DEC_PROC
.endif

///////////////////////////			//ms02
////////////////////////
.ifdef	MS02_Used
.external F_SACM_MS02_MIDI_Initial
.external F_SACM_MS02_MIDI_Play   		// in ms02_player.asm
.external F_SACM_MS02_MIDI_ServiceLoop	// in ms02_seqer.asm
.endif

//////////////////////////
/////////////////////////		//DVR1600
.ifdef	SPEECH1600_Used
.external DVR1600_Play_Init
.external DVR1600_Rec_Init
.external DVR1600_Rec_Endp
.external DVR1600_Rec_Proc
.external DVR1600_Play_Proc
.endif

//////////////////////////
/////////////////////////		//DVR4800
.ifdef	SPEECH4800_Used
// KCY
.external __f_r083_ac8_encode_init
.external __f_r083_ac8_encode
.external __f_r083_ac8_decode_init
.external __f_r083_ac8_decode

.endif
/////////////////////////////////
/////////////////////////		//DVR4800 //add by benwang 20060113
.ifdef	AUDIO4800_Used
// KCY
.external __f_r083_ac8_encode_init
.external __f_r083_ac8_encode
.external __f_r083_ac8_decode_init
.external __f_r083_ac8_decode

.endif
/////////////////////////////////


////////////////////////////////	//S600TTS
.ifdef	S600TTS_Used
.external S600_DEC_INIT
.external S600_DEC_PROC
.external S600_ZERO_Decoder
.endif
///////////////////////////////////
//////////////////////////////
.ifdef A6400S_Used				//added by zgq on 20051215
.external A6400S_DEC_init		//added by zgq on 20051215
.external A6400S_DEC_PROC		//added by zgq on 20051215
.endif							//added by zgq on 20051215

////////////////////////////////
.ifdef	AUDIO3200_Used          //add by benwang 20051220
.external DVR3200_Rec_Init      //add by benwang 20051220
.external DVR3200_Rec_Proc		//add by benwang 20051220
.external DVR3200_Rec_Endp		//add by benwang 20051220
.external DVR3200_Play_Init		//add by benwang 20051220
.external DVR3200_Play_Proc		//add by benwang 20051220
.endif							//add by benwang 20051220

///////////////////////////////
.ifdef DVR520_Used				//added by zgq on 20051220
.external F_DVR520_Dec_Init		//added by zgq on 20051220
.external F_DVR520_Decode		//added by zgq on 20051220
.external F_DVR520_Enc_Init		//added by zgq on 20051220
.external F_DVR520_Encode		//added by zgq on 20051220
.endif							//added by zgq on 20051220

/////////////////////////////// //add by benwang 20060111
.ifdef MP3_Used                 
.external MP3_DEC_init
.external MP3_DEC_PROC
.external _MP3_Get_BR_FS //add by benwang 20060618
.endif

////////////////////////////////


/////////////////////////////// //add by benwang 20060119
.ifdef AUDIO1601_Used                 
.external A1601_DEC_INIT 
.external A1601_DEC_PROC 
.endif



/////////////////////////////////add by benwang 20060221
.ifdef A128_Used                 
.external MP3_DEC_init
.external MP3_DEC_PROC
.external _MP3_Get_BR_FS 
.endif
////////////////////////////////

////////////////////////////////

//add by benwang 20060621
/////////////////////////		//DVR1800
.ifdef	DVR1800_Used
.external F_DVR1800_Enc_Init	
.external F_DVR1800_Encode		
.external F_DVR1800_Dec_Init	
.external F_DVR1800_Decode		

.endif