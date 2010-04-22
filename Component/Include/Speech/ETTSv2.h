#ifndef __ETTSV2_H__
#define __ETTSV2_H__

#if defined(WIN32)
#define __STDCALL __stdcall
#define __POINTER_32__
#elif defined(SUNPLUS) && defined(unSP)
#define __STDCALL
#else
#error This header file can only be used in Generalplus unSP C-compiler (both far and near)
#endif

#if defined(__POINTER_32__)
	typedef const char		*LPCSTR;
	typedef char			*ETTSLPSTR;
	typedef short			*LPSHORT;
	typedef const short		*LPCSHORT;
	typedef void (__STDCALL *LPFCN_CALLBACK_NOTIFY_BUSY)(short b);		
#else
	typedef unsigned long	LPCSTR;
	typedef unsigned long	ETTSLPSTR;
	typedef unsigned long	LPSHORT;
	typedef unsigned long	LPCSHORT;
	typedef unsigned long	LPFCN_CALLBACK_NOTIFY_BUSY;
#endif

#define OPT_WORDBYWORD     0x0001          // do word by word translation
#define OPT_SPELL          0x0002          // spell each word
#define OPT_IGNORE_QM      0x0004          // ignore question marks

//////////////////////////////////////////////////////////////////////////
// Phone to Speech Error ID
//////////////////////////////////////////////////////////////////////////
typedef enum
{
	ETTS_NOERROR = 0,		//		0	// no error
	ETTS_ENDOFPLAY,			//		1	// end of play
	ETTS_ERR_PARSE,			//		2	// parse error
	ETTS_ERR_NOMEM_TTP_BUF,	//		3   // can't alloc memory

	ETTS_ERR_NOMEM_PHONEME,	//		4
	ETTS_ERR_NOMEM_F0,		//		5	// can't alloc memory

	ETTS_ERR_OUTOFRANGE,	//		6	// parm out of range
	ETTS_ERR_INVALIDADDR,	//		7	// invalid address
	ETTS_ERR_SYNTH_MEM,		//		8	// synthmem not in page 0
	ETTS_ERR_TRACK_MEM,		//		9	// Tracks not in page 0

	ETTS_ERR_LIBRARY,		//		10	// Library Version Error
} ETTSRESULT;



ETTSRESULT __STDCALL ETTS_Init(LPFCN_CALLBACK_NOTIFY_BUSY NotifyBusyFcn);
ETTSRESULT __STDCALL ETTS_Play(LPCSTR Text, unsigned short Option);
ETTSRESULT __STDCALL ETTS_DecProc(LPSHORT OutWaveBuf, short WaveLen);

//////////////////////////////////////////////////////////////////////////
// Text to Phone
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Text2Phn(
	LPCSTR lpszInput,
	ETTSLPSTR lpszOutput,
	short cbOutput,
	unsigned short uFlags);

//////////////////////////////////////////////////////////////////////////
// Initial Phone to Speech Module
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_Init(LPFCN_CALLBACK_NOTIFY_BUSY NotifyBusyFcn);



//////////////////////////////////////////////////////////////////////////
// Critcal Block Function (Used by unSP only)
//////////////////////////////////////////////////////////////////////////
#if defined(SUNPLUS) && defined(unSP)
LPCSHORT Phn2Spch_GetDefCritCodeBlock(LPSHORT Len);
short Phn2Spch_UseCritCodeBlock(LPCSHORT Data);
#endif // defined(SUNPLUS) && defined(unSP)


//////////////////////////////////////////////////////////////////////////
// Set Phone string to Phone to Speech Module
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_Play(LPCSTR lpszInput);

//////////////////////////////////////////////////////////////////////////
// Next Sentence
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL NextSentence();


//////////////////////////////////////////////////////////////////////////
// Phone to Speech Decoder Process
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_DecProc(LPSHORT OutWaveBuf, short WaveLen);


//////////////////////////////////////////////////////////////////////////
// Set Pitch
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetPth(short nNewPitch);
#define MINPITCH           10        // minimum allowable pitch
#define MAXPITCH         2000        // maximum allowable pitch


//////////////////////////////////////////////////////////////////////////
// Set Rate
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetRate(short nNewRate);
#define MINRATE            20        // minimum speaking rate
#define MAXRATE           500        // maximum speaking rate


//////////////////////////////////////////////////////////////////////////
// Set Gender
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetGender(short nNewGender);
#define VTRACT_MALE       0          // male
#define VTRACT_FEMALE     1          // female
#define VTRACT_CHILD      2          // child's vocal tract
#define VTRACT_BIGMALE    3          // large male vocal tract


//////////////////////////////////////////////////////////////////////////
// Set Glottal Source
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetGlottalSrc(short nNewGlot);
#define GLOT_DEFAULT	0              // use gender to select source
#define GLOT_MALE		1              // adult male
#define GLOT_FEMALE		2              // adult female
#define GLOT_CHILD		3              // young child or high female
#define GLOT_HIGH		4              // for very high voices, > 300Hz
#define GLOT_MELLOW		5              // ellow adult male voice
#define GLOT_IMPULSE	6              // for lowish voices
#define GLOT_ODD		7              // odd harmonics only
#define GLOT_COLOSSUS	8              // Colossus computer voice

//////////////////////////////////////////////////////////////////////////
// Set F0 Style
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetF0Style(short nNewF0Style);
#define F0_NATURAL		0              // natural pitch contour
#define F0_RESERVED		1              // reserved for later use
#define F0_MONOTONE		2              // flat pitch contour


//////////////////////////////////////////////////////////////////////////
// Set F0 Perturb
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetF0Perturb(short nNewF0Perturb);
#define MIN_F0_PERTRUB	0
#define MAX_F0_PERTRUB	500


//////////////////////////////////////////////////////////////////////////
// Set F0 Range
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetF0Rng(short nNewF0Range);
#define MIN_F0_RANGE	0
#define MAX_F0_RANGE	500


//////////////////////////////////////////////////////////////////////////
// Set AV Bias
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetAVBias(short swAVBias);
// any value


//////////////////////////////////////////////////////////////////////////
// Set AH Bias
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetAHBias(short swAHBias);
// any value


//////////////////////////////////////////////////////////////////////////
// Set AF Bias
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetAFBias(short swAFBias);
// any value


//////////////////////////////////////////////////////////////////////////
// Set Vowel Factor
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetVowFac(short nNewVowelFactor);
// any value


//////////////////////////////////////////////////////////////////////////
// Set Vocal Mode
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetVocMode(short nNewVoicingMode);
#define VMODE_NORMAL		0           // normal voicing
#define VMODE_BREATHY		1           // mellower with breath noise
#define VMODE_WHISPERED		2           // no voicing, all aspiration


//////////////////////////////////////////////////////////////////////////
// Set Personality
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetPersonality(short nNewPersonality);
#define PERS_MALE			0					// default male voice
#define PERS_FEMALE			1					// default female voice
#define PERS_LARGEMALE		2					// default large male voice
#define PERS_CHILD			3					// default child voice
#define PERS_GIANTMALE		4					// low pitched male
#define PERS_MELLOWFEM		5					// sexy female
#define PERS_MELLOWMALE		6					// mellow male
#define PERS_CRISPMALE		7					// crisp, clear male
#define PERS_THEFLY			8					// high pitched, quavering voice
#define PERS_ROBOTOID		9					// robotoid
#define PERS_MARTIAN		10					// alien
#define PERS_COLOSSUS		11					// the voice of the Colossus computer
#define PERS_FASTFRED		12					// fast talking voice
#define PERS_OLDWOMAN		13					// elderly woman
#define PERS_MUNCHKIN		14					// dwarf
#define PERS_TROLL			15					// a little larger than a dwarf
#define PERS_NERD			16					// nerdish male
#define PERS_MILKTOAST		17					// whiney male
#define PERS_TIPSY			18					// drunken male
#define PERS_CHOIRBOY		19					// high pitched, sung voice


//////////////////////////////////////////////////////////////////////////
// Set Speak Mode
//////////////////////////////////////////////////////////////////////////
ETTSRESULT __STDCALL Phn2Spch_SetSpkMode(short nNewMode);
// nNewMode can be OPT_WORDBYWORD or 0


//////////////////////////////////////////////////////////////////////////
// Get Error String
//////////////////////////////////////////////////////////////////////////
LPCSTR __STDCALL ETTS_GetErrStr(ETTSRESULT ErrID);

void __STDCALL Phn2Spch_GetMouth(LPSHORT width, LPSHORT height);


#endif // __ETTSV2_H__
