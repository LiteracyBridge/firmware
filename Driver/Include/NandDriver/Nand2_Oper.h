#ifndef _NAND_OPER_H_
#define _NAND_OPER_H_

#define Nand1G_Select					0
#define Nand2G_Select					1

int _Nand_Initial(void);
int UpdataNandInit(void);
int InitMapTable();
int BuildMapTable();
unsigned long _NAND_GetSize();
int LowFormat(void);
void Nand_CreateSem(void);
int _Nand_Flush();
unsigned int IsReserveBadBlock( unsigned long blockaddr );

int _NAND_ReadSector(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr);
int _NAND_ReadSector_MP3(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr);

int _NAND_WriteSector(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr);

int _NAND_ReadSector_USB(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr, unsigned int DMA_Channel);
int _NAND_WriteSector_USB(unsigned long  LABAddr, unsigned int blkcnt, unsigned long StoreAddr, unsigned int DMA_Channel,unsigned int DMA_RB_Flag);

int _Nand_Direct_Read(unsigned long LogicAddr, unsigned long TARAddr, unsigned int size);

int	_UpDataNand_Continue_Read(unsigned long PageAddr, unsigned long  DataAddr, int size);

int NAND_WriteDirty_Area(unsigned long LABAddr);

int _Nand_LogicRead_Byte( unsigned long Addr, unsigned long buf_ptr, unsigned int word_len );
int _Nand_LogicRead_Word( unsigned long Addr, unsigned long buf_ptr, unsigned int word_len );

int NAND_PowerOff_init( void );
extern const unsigned long Nand_Reserve_Offset;

int _NAND_EndOfWrite(void);
#endif
