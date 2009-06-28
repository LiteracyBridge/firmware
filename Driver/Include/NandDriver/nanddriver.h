#ifndef	__NANDDRIVER_H__
#define	__NANDDRIVER_H__
/**************************************************************************************/
extern		void 			_Nandflash_Initial(void);
extern		void 			_UpdataNandflash_Initial(void);
extern		void			_Nand_Reset(void);
extern		void			_UpdataNand_Reset(void);
extern      void    		_Dummytologic(unsigned long LABAddr, unsigned long PageAddr);//
extern      void            _NAND_ReadID(unsigned long  DataAddr);
extern      unsigned int    ReadNANDStatus_Byte(void);
extern      void            _UpdataNAND_ReadID(unsigned long  DataAddr);
extern		unsigned long 	_Nand_ReadPhysicalPage_ECC(unsigned long PageAddr, unsigned long DataAddr);
extern		unsigned int 	_Nand_WritePhysicalPage(unsigned long PageAddr, unsigned long DataAddr);
extern      unsigned int	_Nand_ErasePhysicalBlock(unsigned int BlockAddr);

extern      unsigned int    _Nand_PhyRead_Word_Soft( unsigned long PageAddr, unsigned long  StoreAddr, int size );
extern      unsigned int    _Nand_PhyRead_Word_Odd_Soft( unsigned long PageAddr, unsigned long  StoreAddr, int size );
extern      unsigned int    _Nand_PhyRead_Byte_Soft( unsigned long PageAddr, unsigned long  StoreAddr, int size );

//extern      unsigned int 	_Nand_ReadPhysical_C_Page(unsigned long PageAddr, unsigned long  StoreAddr);
//extern      unsigned int 	_Nand_WritePhysical_C_Page(unsigned long PageAddr, unsigned long  StoreAddr);
extern      unsigned int 	_Nand_ReadPhysical_C_PageFlag(unsigned long PageAddr, unsigned int WordIndex, unsigned long DataAddr, unsigned int Length);
extern      unsigned int 	_Nand_WritePhysical_C_PageFlag(unsigned long PageAddr, unsigned int WordIndex, unsigned long DataAddr, unsigned int Length);

extern      unsigned int	_Nand_Continue_Read_Aarea(unsigned long PageAddr, unsigned long  DataAddr, int size);//size是BYTE的方式计算
extern      unsigned int	_UpdataNand_Continue_Read_Aarea(unsigned long PageAddr, unsigned long  DataAddr, int size);//size是BYTE的方式计算
extern      unsigned int 	_Nand_Continue_Write_Aarea(unsigned long PhysicalAddr, unsigned long  DataAddr, int size);//size是BYTE的方式计算
extern      int	_Nand_CopyPhysicalPage(unsigned long SrcAddr, unsigned long DestAddr);


extern		unsigned int 	_Nand_WritePhysicalPage_USB(unsigned long PageAddr, unsigned long DataAddr, unsigned int DMA_Channel );
extern		unsigned long 	_Nand_ReadPhysicalPageUSB_ECC(unsigned long PageAddr, unsigned long DataAddr,unsigned int DMA_Channel );
//==============================================================================
#endif

