.ifndef _USB_HOST_MSDC_INTERNAL_INC_
.define _USB_HOST_MSDC_INTERNAL_INC_

CBWStruct_asm: .struct 
DataTransferLength_L: 	.dw 0
DataTransferLength_H: 	.dw 0
CBWFlag: 				.dw 0
LUN: 					.dw 0
CBLength:				.dw 0
.ends

SCSIStruct_asm: .struct
OPCode: 				.dw 0
LBA_L:					.dw 0
LBA_H:					.dw 0
TXParameter:			.dw 0
.ends

.endif