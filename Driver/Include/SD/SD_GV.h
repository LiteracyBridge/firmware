

#ifndef __SD_GV_H__

#define __SD_GV_H__


#ifdef SDCREATEVAR
unsigned long SDCTotalSector __attribute__((section("SD_GlobalValue: .section .RAM_Bank0")));//
unsigned int R_Write_protect __attribute__((section("SD_GlobalValue: .section .RAM_Bank0"))); 
#else

extern unsigned long SDCTotalSector;
extern unsigned int R_Write_protect;
#endif
























#endif