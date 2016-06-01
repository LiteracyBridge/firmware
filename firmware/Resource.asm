
// Resource Table
// Created by IDE, Do not modify this table

.CODE
.public _RES_Table;
.external __RES_BIP_A18_sa
.public _RES_BIP_A18_SA;
.external __RES_BIP_A18_ea;
.public _RES_BIP_A18_EA;
.external __RES_DING_A18_sa
.public _RES_DING_A18_SA;
.external __RES_DING_A18_ea;
.public _RES_DING_A18_EA;


_RES_Table:

_RES_BIP_A18_SA:
	.DW offset __RES_BIP_A18_sa,seg16 __RES_BIP_A18_sa;
_RES_BIP_A18_EA:
	.DW offset __RES_BIP_A18_ea,seg16 __RES_BIP_A18_ea;

_RES_DING_A18_SA:
	.DW offset __RES_DING_A18_sa,seg16 __RES_DING_A18_sa;
_RES_DING_A18_EA:
	.DW offset __RES_DING_A18_ea,seg16 __RES_DING_A18_ea;


// End Table
//.ifdef DVR1800_Used
.public  T_SACM_DVR1800_SpeechTable
T_SACM_DVR1800_SpeechTable:
//	.DW offset __RES_DUNK2_16_A18_sa,seg16 __RES_DUNK2_16_A18_sa;
//.endif
