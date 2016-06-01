	//  C:\PROGRA~1\GENERA~1\UNSPID~1.1\TOOLCH~1\be::1.1.0

	//-----------------------------------------------------------
	// Compiling C:\USB_JUNK\GPL162002-RecordPen+USBH_SourceCode\Driver\USB\USB_ID.c (C:\DOCUME~1\ROGERM~1\LOCALS~1\Temp\ccB.257)
	//-----------------------------------------------------------

	//-----------------------------------------------------------
	// Options:
	//-----------------------------------------------------------
	//  Target:unSP, ISA:ISA_2.0, Pointer Size:32
	//  -O0	(Optimization level)
	//  -g2	(Debug level)
	//  -m1	(Report warnings)
	//  -mglobal-var-iram (Put global-var with no initval in .iram)
	//  -mpack-string-bigendian (Pack String with Big Endian)
	//-----------------------------------------------------------

.stabs "C:\USB_JUNK\GPL162002-RecordPen+USBH_SourceCode",100,0,3,Ltext0
.stabs "C:\USB_JUNK\GPL162002-RecordPen+USBH_SourceCode\Driver\USB\USB_ID.c",100,0,3,Ltext0

.code
Ltext0:
.stabs "int:t1=r1;-32768;32767;",128,0,0,0
.stabs "char:t2=r2;-32768;32767;",128,0,0,0
.stabs "long int:t3=r3;-2147483648;2147483647;",128,0,0,0
.stabs "unsigned int:t4=r4;0;65535;",128,0,0,0
.stabs "long unsigned int:t5=r5;0;4294967295;",128,0,0,0
.stabs "long long int:t6=r1;4;0;",128,0,0,0
.stabs "long long unsigned int:t7=r1;4;0;",128,0,0,0
.stabs "short int:t8=r8;-32768;32767;",128,0,0,0
.stabs "short unsigned int:t9=r9;0;65535;",128,0,0,0
.stabs "signed char:t10=r10;-32768;32767;",128,0,0,0
.stabs "unsigned char:t11=r11;0;65535;",128,0,0,0
.stabs "float:t12=r1;2;0;",128,0,0,0
.stabs "double:t13=r1;4;0;",128,0,0,0
.stabs "long double:t14=r1;4;0;",128,0,0,0
.stabs "complex float:t15=r1;4;0;",128,0,0,0
.stabs "complex double:t16=r1;8;0;",128,0,0,0
.stabs "complex long double:t17=r1;8;0;",128,0,0,0
.stabs "void:t18=18",128,0,0,0
.code
	     .stabs "SetVenderID:F1",36,0,0,_SetVenderID

	// Program Unit: SetVenderID
.public	_SetVenderID
_SetVenderID: .proc	
	     .stabn 0xa6,0,0,103
	// unTmp = 1
	// unTmp1 = 2
	// unBase = 4
	// unKeyID = 5
	// unShiftByte = 6
	// unShiftBit = 7
	// unKeyBuffer = 8
	// unGetData = 89
	// unReplyData = 90
	// i = 3
	// nRlt = 0
	// old_frame_pointer = 103
	// return_address = 104
	// lra_spill_temp_0 = 101
	// lra_spill_temp_1 = 102
//  47  
//  48  					
//  49  					
//  50  int SetVenderID()
//  51  {

LM1:
	     .stabn 68,0,51,LM1-_SetVenderID
BB1_PU0:	// 0x0
// BB:1 cycle count: 31
	     push BP to [SP]          	// [0:51]  
	     SP = SP - 103            	// [2:51]  
	     BP = SP + 1              	// [4:51]  
LBB2:
//  52  	unsigned int unTmp, unTmp1, unBase, unKeyID, unShiftByte, unShiftBit, unKeyBuffer[81], unGetData, unReplyData[11];
//  53  	int			 i, nRlt=0;

LM2:
	     .stabn 68,0,53,LM2-_SetVenderID
	     R4 = 0                   	// [6:53]  
	     [BP + 0] = R4            	// [7:53]  nRlt
//  54  	
//  55  	unTmp = R_CBW_PKT[18]^0x5E;

LM3:
	     .stabn 68,0,55,LM3-_SetVenderID
	     DS = seg(_R_CBW_PKT+18)  	// [8:55]  R_CBW_PKT+18
	     R4 = (_R_CBW_PKT+18)     	// [9:55]  R_CBW_PKT+18
	     R4 = DS:[R4]             	// [11:55]  
	     R4 = R4 ^ 94             	// [13:55]  
	     [BP + 1] = R4            	// [15:55]  unTmp
//  56  	
//  57  	unTmp1 = R_CBW_PKT[18+4];

LM4:
	     .stabn 68,0,57,LM4-_SetVenderID
	     DS = seg(_R_CBW_PKT+22)  	// [16:57]  R_CBW_PKT+22
	     R4 = (_R_CBW_PKT+22)     	// [17:57]  R_CBW_PKT+22
	     R4 = DS:[R4]             	// [19:57]  
	     [BP + 2] = R4            	// [21:57]  unTmp1
//  58  
//  59  	if (unTmp1 != unTmp)

LM5:
	     .stabn 68,0,59,LM5-_SetVenderID
	     R3 = [BP + 1]            	// [22:59]  unTmp
	     R4 = [BP + 2]            	// [24:59]  unTmp1
	     cmp R3, R4               	// [26:59]  
	     je L_0_6                 	// [27:59]  
BB2_PU0:	// 0x17
// BB:2 cycle count: 2
//  60  	{
//  61  		for (i=0; i<10; i++)

LM6:
	     .stabn 68,0,61,LM6-_SetVenderID
	     R4 = 0                   	// [0:61]  
	     [BP + 3] = R4            	// [1:61]  i
L_0_7:	// 0x19
// BB:3 cycle count: 7
	     R4 = [BP + 3]            	// [0:61]  i
	     cmp R4, 9                	// [2:61]  
	     jg L_0_8                 	// [3:61]  
BB4_PU0:	// 0x1c
// BB:4 cycle count: 7
//  62  			*P_USBD_BIData = 0;

LM7:
	     .stabn 68,0,62,LM7-_SetVenderID
	     R2 = 0                   	// [0:62]  
	     R3 = 31540               	// [1:62]  
	     R4 = 0                   	// [3:62]  
	     DS = R4                  	// [4:62]  
	     DS:[R3] = R2             	// [5:62]  
Lt_0_1:	// 0x22
// BB:5 cycle count: 8

LM8:
	     .stabn 68,0,61,LM8-_SetVenderID
	     R4 = [BP + 3]            	// [0:61]  i
	     R4 = R4 + 1              	// [2:61]  
	     [BP + 3] = R4            	// [3:61]  i
	     jmp L_0_7                	// [4:61]  
L_0_8:	// 0x26
// BB:6 cycle count: 3
//  63  		
//  64  		goto TXBUFFER;

LM9:
	     .stabn 68,0,64,LM9-_SetVenderID
	     goto Lt_0_2              	// [0:64]  
L_0_6:	// 0x28
// BB:7 cycle count: 92
//  65  	}
//  66  	
//  67  	unTmp = R_CBW_PKT[18+1] + R_CBW_PKT[18+3];

LM10:
	     .stabn 68,0,67,LM10-_SetVenderID
	     DS = seg(_R_CBW_PKT+19)  	// [0:67]  R_CBW_PKT+19
	     R4 = (_R_CBW_PKT+19)     	// [1:67]  R_CBW_PKT+19
	     R4 = DS:[R4]             	// [3:67]  
	     DS = seg(_R_CBW_PKT+21)  	// [5:67]  R_CBW_PKT+21
	     R3 = (_R_CBW_PKT+21)     	// [6:67]  R_CBW_PKT+21
	     R3 = DS:[R3]             	// [8:67]  
	     R4 = R4 + R3             	// [10:67]  
	     [BP + 1] = R4            	// [11:67]  unTmp
//  68  	unTmp1 = R_CBW_PKT[18+5] + R_CBW_PKT[18+7];

LM11:
	     .stabn 68,0,68,LM11-_SetVenderID
	     DS = seg(_R_CBW_PKT+23)  	// [12:68]  R_CBW_PKT+23
	     R4 = (_R_CBW_PKT+23)     	// [13:68]  R_CBW_PKT+23
	     R4 = DS:[R4]             	// [15:68]  
	     DS = seg(_R_CBW_PKT+25)  	// [17:68]  R_CBW_PKT+25
	     R3 = (_R_CBW_PKT+25)     	// [18:68]  R_CBW_PKT+25
	     R3 = DS:[R3]             	// [20:68]  
	     R4 = R4 + R3             	// [22:68]  
	     [BP + 2] = R4            	// [23:68]  unTmp1
//  69  	unBase = (unTmp ^ unTmp1)&0xFF;

LM12:
	     .stabn 68,0,69,LM12-_SetVenderID
	     R4 = [BP + 1]            	// [24:69]  unTmp
	     R4 = R4 ^ [BP + 2]       	// [26:69]  unTmp1
	     R4 = R4 & 255            	// [28:69]  
	     [BP + 4] = R4            	// [30:69]  unBase
//  70  	
//  71  //	unTmp = R_CBW_PKT[18+9]^unBase;
//  72  //	unTmp1 = (R_CBW_PKT[18+8]^unBase)<<8;
//  73  
//  74  	unKeyID = R_CBW_PKT[18+9]^unBase + ((R_CBW_PKT[18+8]^unBase)<<8);

LM13:
	     .stabn 68,0,74,LM13-_SetVenderID
	     DS = seg(_R_CBW_PKT+27)  	// [31:74]  R_CBW_PKT+27
	     R4 = (_R_CBW_PKT+27)     	// [32:74]  R_CBW_PKT+27
	     R4 = DS:[R4]             	// [34:74]  
	     DS = seg(_R_CBW_PKT+26)  	// [36:74]  R_CBW_PKT+26
	     R3 = (_R_CBW_PKT+26)     	// [37:74]  R_CBW_PKT+26
	     R3 = DS:[R3]             	// [39:74]  
	     R3 = R3 ^ [BP + 4]       	// [41:74]  unBase
	     R3 = R3 lsl 4            	// [43:74]  
	     R3 = R3 lsl 4            	// [44:74]  
	     R3 = R3 + [BP + 4]       	// [45:74]  unBase
	     R4 = R4 ^ R3             	// [47:74]  
	     [BP + 5] = R4            	// [48:74]  unKeyID
//  75  	unKeyID %= 512;

LM14:
	     .stabn 68,0,75,LM14-_SetVenderID
	     R4 = [BP + 5]            	// [49:75]  unKeyID
	     R4 = R4 & 511            	// [51:75]  
	     [BP + 5] = R4            	// [53:75]  unKeyID
//  76  	unShiftByte = unKeyID/8;

LM15:
	     .stabn 68,0,76,LM15-_SetVenderID
	     R4 = [BP + 5]            	// [54:76]  unKeyID
	     R4 = R4 lsr 3            	// [56:76]  
	     [BP + 6] = R4            	// [57:76]  unShiftByte
//  77  	unShiftBit = unKeyID%8;	

LM16:
	     .stabn 68,0,77,LM16-_SetVenderID
	     R4 = [BP + 5]            	// [58:77]  unKeyID
	     R4 = R4 & 7              	// [60:77]  
	     [BP + 7] = R4            	// [61:77]  unShiftBit
	     SP = SP - 6              	// [62:77]  
	     R2 = BP + 8              	// [63:77]  unKeyBuffer
	     R3 = 0                   	// [65:77]  
	     R4 = SP + 1              	// [66:77]  
	     [R4++] = R2              	// [68:77]  
	     [R4] = R3                	// [70:77]  
	     R2 = (_szKeyTable4)      	// [72:77]  szKeyTable4
	     R3 = seg(_szKeyTable4)   	// [74:77]  szKeyTable4
	     R4 = SP + 3              	// [75:77]  
	     [R4++] = R2              	// [77:77]  
	     [R4] = R3                	// [79:77]  
	     R2 = 16                  	// [81:77]  
	     R3 = 0                   	// [82:77]  
	     R4 = SP + 5              	// [83:77]  
	     [R4++] = R2              	// [85:77]  
	     [R4] = R3                	// [87:77]  
//  78  	
//  79  	memcpy(unKeyBuffer, szKeyTable4, 16);

LM17:
	     .stabn 68,0,79,LM17-_SetVenderID
	     call _memcpy             	// [89:79]  memcpy
BB8_PU0:	// 0x70
// BB:8 cycle count: 29
	     R2 = BP + 24             	// [0:79]  unKeyBuffer+16
	     R3 = 0                   	// [2:79]  
	     R4 = SP + 1              	// [3:79]  
	     [R4++] = R2              	// [5:79]  
	     [R4] = R3                	// [7:79]  
	     R2 = (_szKeyTable1)      	// [9:79]  szKeyTable1
	     R3 = seg(_szKeyTable1)   	// [11:79]  szKeyTable1
	     R4 = SP + 3              	// [12:79]  
	     [R4++] = R2              	// [14:79]  
	     [R4] = R3                	// [16:79]  
	     R2 = 16                  	// [18:79]  
	     R3 = 0                   	// [19:79]  
	     R4 = SP + 5              	// [20:79]  
	     [R4++] = R2              	// [22:79]  
	     [R4] = R3                	// [24:79]  
//  80  	memcpy(unKeyBuffer+16, szKeyTable1, 16);

LM18:
	     .stabn 68,0,80,LM18-_SetVenderID
	     call _memcpy             	// [26:80]  memcpy
BB9_PU0:	// 0x86
// BB:9 cycle count: 29
	     R2 = BP + 40             	// [0:80]  unKeyBuffer+32
	     R3 = 0                   	// [2:80]  
	     R4 = SP + 1              	// [3:80]  
	     [R4++] = R2              	// [5:80]  
	     [R4] = R3                	// [7:80]  
	     R2 = (_szKeyTable3)      	// [9:80]  szKeyTable3
	     R3 = seg(_szKeyTable3)   	// [11:80]  szKeyTable3
	     R4 = SP + 3              	// [12:80]  
	     [R4++] = R2              	// [14:80]  
	     [R4] = R3                	// [16:80]  
	     R2 = 16                  	// [18:80]  
	     R3 = 0                   	// [19:80]  
	     R4 = SP + 5              	// [20:80]  
	     [R4++] = R2              	// [22:80]  
	     [R4] = R3                	// [24:80]  
//  81  	memcpy(unKeyBuffer+32, szKeyTable3, 16);

LM19:
	     .stabn 68,0,81,LM19-_SetVenderID
	     call _memcpy             	// [26:81]  memcpy
BB10_PU0:	// 0x9c
// BB:10 cycle count: 29
	     R2 = BP + 56             	// [0:81]  unKeyBuffer+48
	     R3 = 0                   	// [2:81]  
	     R4 = SP + 1              	// [3:81]  
	     [R4++] = R2              	// [5:81]  
	     [R4] = R3                	// [7:81]  
	     R2 = (_szKeyTable2)      	// [9:81]  szKeyTable2
	     R3 = seg(_szKeyTable2)   	// [11:81]  szKeyTable2
	     R4 = SP + 3              	// [12:81]  
	     [R4++] = R2              	// [14:81]  
	     [R4] = R3                	// [16:81]  
	     R2 = 16                  	// [18:81]  
	     R3 = 0                   	// [19:81]  
	     R4 = SP + 5              	// [20:81]  
	     [R4++] = R2              	// [22:81]  
	     [R4] = R3                	// [24:81]  
//  82  	memcpy(unKeyBuffer+48, szKeyTable2, 16);			

LM20:
	     .stabn 68,0,82,LM20-_SetVenderID
	     call _memcpy             	// [26:82]  memcpy
BB11_PU0:	// 0xb2
// BB:11 cycle count: 29
	     R2 = BP + 72             	// [0:82]  unKeyBuffer+64
	     R3 = 0                   	// [2:82]  
	     R4 = SP + 1              	// [3:82]  
	     [R4++] = R2              	// [5:82]  
	     [R4] = R3                	// [7:82]  
	     R2 = (_szKeyTable4)      	// [9:82]  szKeyTable4
	     R3 = seg(_szKeyTable4)   	// [11:82]  szKeyTable4
	     R4 = SP + 3              	// [12:82]  
	     [R4++] = R2              	// [14:82]  
	     [R4] = R3                	// [16:82]  
	     R2 = 16                  	// [18:82]  
	     R3 = 0                   	// [19:82]  
	     R4 = SP + 5              	// [20:82]  
	     [R4++] = R2              	// [22:82]  
	     [R4] = R3                	// [24:82]  
//  83  	memcpy(unKeyBuffer+64, szKeyTable4, 16);				

LM21:
	     .stabn 68,0,83,LM21-_SetVenderID
	     call _memcpy             	// [26:83]  memcpy
BB12_PU0:	// 0xc8
// BB:12 cycle count: 30
	     SP = SP + 6              	// [0:83]  
//  84  	
//  85  	unBase = ((R_CBW_PKT[18] + R_CBW_PKT[18+2])^(R_CBW_PKT[18+4] + R_CBW_PKT[18+6]))&0xFF;

LM22:
	     .stabn 68,0,85,LM22-_SetVenderID
	     DS = seg(_R_CBW_PKT+18)  	// [1:85]  R_CBW_PKT+18
	     R4 = (_R_CBW_PKT+18)     	// [2:85]  R_CBW_PKT+18
	     R4 = DS:[R4]             	// [4:85]  
	     DS = seg(_R_CBW_PKT+20)  	// [6:85]  R_CBW_PKT+20
	     R3 = (_R_CBW_PKT+20)     	// [7:85]  R_CBW_PKT+20
	     R3 = DS:[R3]             	// [9:85]  
	     R4 = R4 + R3             	// [11:85]  
	     DS = seg(_R_CBW_PKT+22)  	// [12:85]  R_CBW_PKT+22
	     R3 = (_R_CBW_PKT+22)     	// [13:85]  R_CBW_PKT+22
	     R3 = DS:[R3]             	// [15:85]  
	     DS = seg(_R_CBW_PKT+24)  	// [17:85]  R_CBW_PKT+24
	     R2 = (_R_CBW_PKT+24)     	// [18:85]  R_CBW_PKT+24
	     R2 = DS:[R2]             	// [20:85]  
	     R3 = R3 + R2             	// [22:85]  
	     R4 = R4 ^ R3             	// [23:85]  
	     R4 = R4 & 255            	// [24:85]  
	     [BP + 4] = R4            	// [26:85]  unBase
//  86  
//  87  	//Get key table value	
//  88  	for (i=unShiftByte; i<(unShiftByte+8); i++)

LM23:
	     .stabn 68,0,88,LM23-_SetVenderID
	     R4 = [BP + 6]            	// [27:88]  unShiftByte
	     [BP + 3] = R4            	// [29:88]  i
L_0_9:	// 0xe1
// BB:13 cycle count: 10
	     R3 = [BP + 3]            	// [0:88]  i
	     R4 = [BP + 6]            	// [2:88]  unShiftByte
	     R4 = R4 + 8              	// [4:88]  
	     cmp R3, R4               	// [5:88]  
	     jb BB14_PU0              	// [6:88]  
BB33_PU0:	// 0xe6
// BB:33 cycle count: 3
	     goto L_0_10              	// [0:0]  
BB14_PU0:	// 0xe8
// BB:14 cycle count: 80
//  89  	{
//  90  		unGetData = unKeyBuffer[i]*0x100 + unKeyBuffer[i+1];

LM24:
	     .stabn 68,0,90,LM24-_SetVenderID
	     R4 = [BP + 3]            	// [0:90]  i
	     R4 = R4 + 1              	// [2:90]  
	     R3 = R4 asr 4            	// [3:90]  
	     R3 = R3 asr 4            	// [4:90]  
	     R3 = R3 asr 4            	// [5:90]  
	     R3 = R3 asr 4            	// [6:90]  
	     R1 = BP + 8              	// [7:90]  unKeyBuffer
	     R2 = 0                   	// [9:90]  
	     R4 = R4 + R1             	// [10:90]  
	     R3 = R3 + R2, Carry      	// [11:90]  
	     DS = R3                  	// [12:90]  
	     R4 = DS:[R4]             	// [13:90]  
	     BP = BP + 101            	// [15:90]  
	     [BP + 0] = R4            	// [17:90]  lra_spill_temp_0
	     BP = BP - 101            	// [18:90]  
	     R4 = [BP + 3]            	// [20:90]  i
	     R3 = R4 asr 4            	// [22:90]  
	     R3 = R3 asr 4            	// [23:90]  
	     R3 = R3 asr 4            	// [24:90]  
	     R3 = R3 asr 4            	// [25:90]  
	     R1 = BP + 8              	// [26:90]  unKeyBuffer
	     R2 = 0                   	// [28:90]  
	     R4 = R4 + R1             	// [29:90]  
	     R3 = R3 + R2, Carry      	// [30:90]  
	     DS = R3                  	// [31:90]  
	     R4 = DS:[R4]             	// [32:90]  
	     R3 = R4 lsl 4            	// [34:90]  
	     R3 = R3 lsl 4            	// [35:90]  
	     BP = BP + 89             	// [36:90]  
	     R4 = [BP + 12]           	// [38:90]  lra_spill_temp_0
	     R4 = R4 + R3             	// [40:90]  
	     [BP + 0] = R4            	// [41:90]  unGetData
	     BP = BP - 89             	// [42:90]  
//  91  		unReplyData[i-unShiftByte] = (unGetData<<unShiftBit)>>8;

LM25:
	     .stabn 68,0,91,LM25-_SetVenderID
	     BP = BP + 89             	// [44:91]  
	     R4 = [BP + 0]            	// [46:91]  unGetData
	     BP = BP - 89             	// [48:91]  
	     R3 = [BP + 7]            	// [50:91]  unShiftBit
	     R3 = R3 & 15             	// [52:91]  
	     R4 = R4 lsl R3           	// [53:91]  
	     R4 = R4 lsr 4            	// [54:91]  
	     R4 = R4 lsr 4            	// [55:91]  
	     BP = BP + 102            	// [56:91]  
	     [BP + 0] = R4            	// [58:91]  lra_spill_temp_1
	     BP = BP - 102            	// [59:91]  
	     R4 = [BP + 3]            	// [61:91]  i
	     R4 = R4 - [BP + 6]       	// [63:91]  unShiftByte
	     R3 = 0                   	// [65:91]  
	     R1 = BP + 90             	// [66:91]  unReplyData
	     R2 = 0                   	// [68:91]  
	     R4 = R4 + R1             	// [69:91]  
	     R3 = R3 + R2, Carry      	// [70:91]  
	     DS = R3                  	// [71:91]  
	     BP = BP + 102            	// [72:91]  
	     R3 = [BP + 0]            	// [74:91]  lra_spill_temp_1
	     BP = BP - 102            	// [76:91]  
	     DS:[R4] = R3             	// [78:91]  
Lt_0_3:	// 0x12d
// BB:15 cycle count: 7

LM26:
	     .stabn 68,0,88,LM26-_SetVenderID
	     R4 = [BP + 3]            	// [0:88]  i
	     R4 = R4 + 1              	// [2:88]  
	     [BP + 3] = R4            	// [3:88]  i
	     goto L_0_9               	// [4:88]  
L_0_10:	// 0x132
// BB:16 cycle count: 4
//  92  	}	
//  93  
//  94  	unTmp=2;	

LM27:
	     .stabn 68,0,94,LM27-_SetVenderID
	     R4 = 2                   	// [0:94]  
	     [BP + 1] = R4            	// [1:94]  unTmp
//  95  	for (i=7; i>=0; i--)//insert dummy word in 3 and 4 byte

LM28:
	     .stabn 68,0,95,LM28-_SetVenderID
	     R4 = 7                   	// [2:95]  
	     [BP + 3] = R4            	// [3:95]  i
L_0_11:	// 0x136
// BB:17 cycle count: 7
	     R4 = [BP + 3]            	// [0:95]  i
	     cmp R4, 0                	// [2:95]  
	     jge BB18_PU0             	// [3:95]  
BB32_PU0:	// 0x139
// BB:32 cycle count: 3
	     goto L_0_12              	// [0:0]  
BB18_PU0:	// 0x13b
// BB:18 cycle count: 7
//  96  	{
//  97  		if (i<3)

LM29:
	     .stabn 68,0,97,LM29-_SetVenderID
	     R4 = [BP + 3]            	// [0:97]  i
	     cmp R4, 2                	// [2:97]  
	     jg L_0_13                	// [3:97]  
BB19_PU0:	// 0x13e
// BB:19 cycle count: 2
//  98  			unTmp=0;		

LM30:
	     .stabn 68,0,98,LM30-_SetVenderID
	     R4 = 0                   	// [0:98]  
	     [BP + 1] = R4            	// [1:98]  unTmp
L_0_13:	// 0x140
// BB:20 cycle count: 47
//  99  			
// 100  		unReplyData[i+unTmp] = unReplyData[i] ^ unBase;

LM31:
	     .stabn 68,0,100,LM31-_SetVenderID
	     R4 = [BP + 3]            	// [0:100]  i
	     R3 = R4 asr 4            	// [2:100]  
	     R3 = R3 asr 4            	// [3:100]  
	     R3 = R3 asr 4            	// [4:100]  
	     R3 = R3 asr 4            	// [5:100]  
	     R1 = BP + 90             	// [6:100]  unReplyData
	     R2 = 0                   	// [8:100]  
	     R4 = R4 + R1             	// [9:100]  
	     R3 = R3 + R2, Carry      	// [10:100]  
	     DS = R3                  	// [11:100]  
	     R4 = DS:[R4]             	// [12:100]  
	     R4 = R4 ^ [BP + 4]       	// [14:100]  unBase
	     BP = BP + 102            	// [16:100]  
	     [BP + 0] = R4            	// [18:100]  lra_spill_temp_1
	     BP = BP - 102            	// [19:100]  
	     R4 = [BP + 3]            	// [21:100]  i
	     R4 = R4 + [BP + 1]       	// [23:100]  unTmp
	     R3 = 0                   	// [25:100]  
	     R1 = BP + 90             	// [26:100]  unReplyData
	     R2 = 0                   	// [28:100]  
	     R4 = R4 + R1             	// [29:100]  
	     R3 = R3 + R2, Carry      	// [30:100]  
	     DS = R3                  	// [31:100]  
	     BP = BP + 102            	// [32:100]  
	     R3 = [BP + 0]            	// [34:100]  lra_spill_temp_1
	     BP = BP - 102            	// [36:100]  
	     DS:[R4] = R3             	// [38:100]  
// 101  		if (i==3)

LM32:
	     .stabn 68,0,101,LM32-_SetVenderID
	     R4 = [BP + 3]            	// [40:101]  i
	     cmp R4, 3                	// [42:101]  
	     jne L_0_14               	// [43:101]  
BB21_PU0:	// 0x164
// BB:21 cycle count: 36
// 102  			unReplyData[i] = R_CBW_PKT[4]^R_CBW_PKT[18+7];

LM33:
	     .stabn 68,0,102,LM33-_SetVenderID
	     DS = seg(_R_CBW_PKT+4)   	// [0:102]  R_CBW_PKT+4
	     R4 = (_R_CBW_PKT+4)      	// [1:102]  R_CBW_PKT+4
	     R4 = DS:[R4]             	// [3:102]  
	     DS = seg(_R_CBW_PKT+25)  	// [5:102]  R_CBW_PKT+25
	     R3 = (_R_CBW_PKT+25)     	// [6:102]  R_CBW_PKT+25
	     R3 = DS:[R3]             	// [8:102]  
	     R4 = R4 ^ R3             	// [10:102]  
	     BP = BP + 101            	// [11:102]  
	     [BP + 0] = R4            	// [13:102]  lra_spill_temp_0
	     BP = BP - 101            	// [14:102]  
	     R4 = [BP + 3]            	// [16:102]  i
	     R3 = R4 asr 4            	// [18:102]  
	     R3 = R3 asr 4            	// [19:102]  
	     R3 = R3 asr 4            	// [20:102]  
	     R3 = R3 asr 4            	// [21:102]  
	     R1 = BP + 90             	// [22:102]  unReplyData
	     R2 = 0                   	// [24:102]  
	     R4 = R4 + R1             	// [25:102]  
	     R3 = R3 + R2, Carry      	// [26:102]  
	     DS = R3                  	// [27:102]  
	     BP = BP + 101            	// [28:102]  
	     R3 = [BP + 0]            	// [30:102]  lra_spill_temp_0
	     BP = BP - 101            	// [32:102]  
	     DS:[R4] = R3             	// [34:102]  
L_0_14:	// 0x183
// BB:22 cycle count: 7
// 103  		if (i==4)

LM34:
	     .stabn 68,0,103,LM34-_SetVenderID
	     R4 = [BP + 3]            	// [0:103]  i
	     cmp R4, 4                	// [2:103]  
	     jne L_0_15               	// [3:103]  
BB23_PU0:	// 0x186
// BB:23 cycle count: 36
// 104  			unReplyData[i] = R_CBW_PKT[6]^R_CBW_PKT[18+6];

LM35:
	     .stabn 68,0,104,LM35-_SetVenderID
	     DS = seg(_R_CBW_PKT+6)   	// [0:104]  R_CBW_PKT+6
	     R4 = (_R_CBW_PKT+6)      	// [1:104]  R_CBW_PKT+6
	     R4 = DS:[R4]             	// [3:104]  
	     DS = seg(_R_CBW_PKT+24)  	// [5:104]  R_CBW_PKT+24
	     R3 = (_R_CBW_PKT+24)     	// [6:104]  R_CBW_PKT+24
	     R3 = DS:[R3]             	// [8:104]  
	     R4 = R4 ^ R3             	// [10:104]  
	     BP = BP + 102            	// [11:104]  
	     [BP + 0] = R4            	// [13:104]  lra_spill_temp_1
	     BP = BP - 102            	// [14:104]  
	     R4 = [BP + 3]            	// [16:104]  i
	     R3 = R4 asr 4            	// [18:104]  
	     R3 = R3 asr 4            	// [19:104]  
	     R3 = R3 asr 4            	// [20:104]  
	     R3 = R3 asr 4            	// [21:104]  
	     R1 = BP + 90             	// [22:104]  unReplyData
	     R2 = 0                   	// [24:104]  
	     R4 = R4 + R1             	// [25:104]  
	     R3 = R3 + R2, Carry      	// [26:104]  
	     DS = R3                  	// [27:104]  
	     BP = BP + 102            	// [28:104]  
	     R3 = [BP + 0]            	// [30:104]  lra_spill_temp_1
	     BP = BP - 102            	// [32:104]  
	     DS:[R4] = R3             	// [34:104]  
L_0_15:	// 0x1a5
Lt_0_4:	// 0x1a5
// BB:24 cycle count: 7

LM36:
	     .stabn 68,0,95,LM36-_SetVenderID
	     R4 = [BP + 3]            	// [0:95]  i
	     R4 = R4 - 1              	// [2:95]  
	     [BP + 3] = R4            	// [3:95]  i
	     goto L_0_11              	// [4:95]  
L_0_12:	// 0x1aa
// BB:25 cycle count: 2
// 105  	}
// 106  
// 107  	for (i=0; i<10; i++)

LM37:
	     .stabn 68,0,107,LM37-_SetVenderID
	     R4 = 0                   	// [0:107]  
	     [BP + 3] = R4            	// [1:107]  i
L_0_16:	// 0x1ac
// BB:26 cycle count: 7
	     R4 = [BP + 3]            	// [0:107]  i
	     cmp R4, 9                	// [2:107]  
	     jg L_0_17                	// [3:107]  
BB27_PU0:	// 0x1af
// BB:27 cycle count: 20
// 108  		*P_USBD_BIData = unReplyData[i];

LM38:
	     .stabn 68,0,108,LM38-_SetVenderID
	     R4 = [BP + 3]            	// [0:108]  i
	     R3 = R4 asr 4            	// [2:108]  
	     R3 = R3 asr 4            	// [3:108]  
	     R3 = R3 asr 4            	// [4:108]  
	     R3 = R3 asr 4            	// [5:108]  
	     R1 = BP + 90             	// [6:108]  unReplyData
	     R2 = 0                   	// [8:108]  
	     R4 = R4 + R1             	// [9:108]  
	     R3 = R3 + R2, Carry      	// [10:108]  
	     DS = R3                  	// [11:108]  
	     R2 = DS:[R4]             	// [12:108]  
	     R3 = 31540               	// [14:108]  
	     R4 = 0                   	// [16:108]  
	     DS = R4                  	// [17:108]  
	     DS:[R3] = R2             	// [18:108]  
Lt_0_5:	// 0x1c0
// BB:28 cycle count: 8

LM39:
	     .stabn 68,0,107,LM39-_SetVenderID
	     R4 = [BP + 3]            	// [0:107]  i
	     R4 = R4 + 1              	// [2:107]  
	     [BP + 3] = R4            	// [3:107]  i
	     jmp L_0_16               	// [4:107]  
L_0_17:	// 0x1c4
// BB:29 cycle count: 2
// 109  	
// 110  	nRlt = 1;

LM40:
	     .stabn 68,0,110,LM40-_SetVenderID
	     R4 = 1                   	// [0:110]  
	     [BP + 0] = R4            	// [1:110]  nRlt
Lt_0_2:	// 0x1c6
// BB:30 cycle count: 17
// 111  	
// 112  TXBUFFER:	
// 113  	
// 114  	*P_USBD_EPEvent=0x0100;

LM41:
	     .stabn 68,0,114,LM41-_SetVenderID
	     R2 = 256                 	// [0:114]  
	     R3 = 31543               	// [2:114]  
	     R4 = 0                   	// [4:114]  
	     DS = R4                  	// [5:114]  
	     DS:[R3] = R2             	// [6:114]  
// 115  	return nRlt;

LM42:
	     .stabn 68,0,115,LM42-_SetVenderID
	     R1 = [BP + 0]            	// [8:115]  nRlt
	     SP = SP + 103            	// [10:115]  
	     pop BP, PC from [SP]     	// [12:115]  
LBE2:
	.endp	
	     .stabn 192,0,0,LBB2-_SetVenderID
	     .stabs "unTmp:4",128,0,0,1
	     .stabs "unTmp1:4",128,0,0,2
	     .stabs "unBase:4",128,0,0,4
	     .stabs "unKeyID:4",128,0,0,5
	     .stabs "unShiftByte:4",128,0,0,6
	     .stabs "unShiftBit:4",128,0,0,7
	     .stabs "unKeyBuffer:19=ar3;0;80;4",128,0,0,8
	     .stabs "unGetData:4",128,0,0,89
	     .stabs "unReplyData:20=ar3;0;10;4",128,0,0,90
	     .stabs "i:1",128,0,0,3
	     .stabs "nRlt:1",128,0,0,0
	     .stabn 224,0,0,LBE2-_SetVenderID
LME1:
	     .stabf LME1-_SetVenderID

.nb_data
	     .stabs "szKeyTable4:G21=ar3;0;15;4",32,0,0,_szKeyTable4
.public	_szKeyTable4
_szKeyTable4:	// 0x0
	.dw	231
	.dw	83
	.dw	237
	.dw	80
	.dw	130
	.dw	104
	.dw	147
	.dw	41
	.dw	136
	.dw	111
	.dw	52
	.dw	137
	.dw	183
	.dw	30
	.dw	25
	.dw	220
	// end of initialization for szKeyTable4
	     .stabs "szKeyTable1:G21",32,0,0,_szKeyTable1
.public	_szKeyTable1
_szKeyTable1:	// 0x10
	.dw	187
	.dw	142
	.dw	217
	.dw	174
	.dw	146
	.dw	153
	.dw	29
	.dw	179
	.dw	199
	.dw	88
	.dw	51
	.dw	152
	.dw	58
	.dw	212
	.dw	193
	.dw	105
	// end of initialization for szKeyTable1
	     .stabs "szKeyTable3:G21",32,0,0,_szKeyTable3
.public	_szKeyTable3
_szKeyTable3:	// 0x20
	.dw	225
	.dw	105
	.dw	127
	.dw	110
	.dw	8
	.dw	34
	.dw	170
	.dw	192
	.dw	83
	.dw	79
	.dw	240
	.dw	101
	.dw	122
	.dw	75
	.dw	217
	.dw	93
	// end of initialization for szKeyTable3
	     .stabs "szKeyTable2:G21",32,0,0,_szKeyTable2
.public	_szKeyTable2
_szKeyTable2:	// 0x30
	.dw	177
	.dw	169
	.dw	102
	.dw	233
	.dw	129
	.dw	57
	.dw	89
	.dw	201
	.dw	229
	.dw	172
	.dw	2
	.dw	158
	.dw	6
	.dw	67
	.dw	199
	.dw	88
	// end of initialization for szKeyTable2
.external _R_CBW_PKT
.external _memcpy
