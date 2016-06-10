
//*****************************************************************************
//*****************************************************************************
// Function Call (API) Declaration for Assembly language
//*****************************************************************************
//*****************************************************************************
//=============================================================================
.external F_SACM_CHA_AudioOutput_Enable
.external F_SACM_CHB_AudioOutput_Enable
.external F_SACM_CHA_AudioOutput_Disable
.external F_SACM_CHB_AudioOutput_Disable
.external F_SACM_CHA_ControlLogic_Enable
.external F_SACM_CHB_ControlLogic_Enable
.external F_SACM_CHA_ControlLogic_Enable_FIFO
.external F_SACM_CHB_ControlLogic_Enable_FIFO
.external F_SACM_CHA_ControlLogic_Disable
.external F_SACM_CHB_ControlLogic_Disable
.external F_SACM_CHA_PlayInterrupt_Enable
.external F_SACM_CHB_PlayInterrupt_Enable
.external F_SACM_CHA_PlayInterrupt_Disable
.external F_SACM_CHB_PlayInterrupt_Disable
.external F_SACM_CHA_WaitFIFOEmpty
.external F_SACM_CHB_WaitFIFOEmpty
.external F_SACM_RampUpDAC1
.external F_SACM_RampDnDAC1
.external F_SACM_RampUpDAC2
.external F_SACM_RampDnDAC2

.external F_SACM_ADC_ControlLogic_Enable
.external F_SACM_ADC_ControlLogic_Disable
.external F_SACM_ADC_RecordInterrupt_Enable
.external F_SACM_ADC_RecordInterrupt_Disable

.external F_SACM_SendDAC1
.external F_SACM_SendDAC2
.external F_SACM_Init

.external F_SACM_GetADC

.external	F_SetIRQ_16000
.external	F_SetIRQ_8000
.external	F_SetIRQ_9200
.external	F_SetIRQ_11025
.external	F_SetIRQ_32000
.external	F_SetIRQ_22050

.external	F_SACM_ADC_MoniterON
.external	F_SACM_ADC_SetSamplerate
.external	F_ADC_SetIRQ_16000
.external	F_ADC_SetIRQ_8000