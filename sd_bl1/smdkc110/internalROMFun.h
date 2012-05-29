#ifndef __INTERNAL_ROM_FN__
#define __INTERNAL_ROM_FN__


/* 
 * This Function copies a block of page to destination( 1-bit ECC only )
 * @param blcok : Source block address number to copy.
 * @param page : Source page address number to copy.
 * @param buffer : Target Buffer pointer.
 * @return int - Success or failure.
 */ 
#define NF8_ReadPage(a,b,c) (((int(*)(unsigned int, unsigned int, unsigned char*))(*((unsigned int *)0xD003E000)))(a,b,c))


/* 
 * This Function copies a block of page to destination( 4-bit ECC only )
 * @param blcok : Source block address number to copy.
 * @param page : Source page address number to copy.
 * @param buffer : Target Buffer pointer.
 * @return int - Success or failure.
 */ 
#define NF8_ReadPage_Adv(a,b,c) (((int(*)(unsigned int, unsigned int, unsigned char*))(*((unsigned int *)0xD003E004)))(a,b,c))


/**
  * This Function copies movinand(MMC) Card Data to memory.
  * Always use EPLL source clock.
  * This function works at 25Mhz.
  * @param channel : HSMMC Controller channel number.
  * @param StartBlkAddress : Source card(movinand(MMC)) Address.(It must block address.)
  * @param blockSize : Number of blocks to copy.
  * @param memoryPtr : Buffer to copy from.
//  * @param extClockSpeed : External clock speed(per HZ)
  * @param with_init : reinitialize or not
  * @return bool(unsigend char) - Success or failure.
  */
//#define CopySDMMCtoMem(z,a,b,c,e)	(((bool(*)(int, unsigned int, unsigned short, unsigned int*, bool))(*((unsigned int *)0xD003E008)))(z,a,b,c,e))
#define CopySDMMCtoMem(z,a,b,c,e)	(((bool(*)(int, unsigned int, unsigned short, unsigned int*, bool))(*((unsigned int *)0xD0037F98)))(z,a,b,c,e))


#define CopyMMC4_3toMem(a,b,c,d)	(((bool(*)(bool, unsigned int, unsigned int*, int))(*((unsigned int *)0xD003E00C)))(a,b,c,d))

/**
 * Single Word Transfer
 * bool ONENAND_ReadPage(unsigned int Controller, unsigned int uBlkAddr, unsigned char uPageAddr, unsigned int* aData) 
 * @param Controller - OneNand Controller Number ('0' fixed) 
 * @param uBlkAddr - Block Number to read 
 * @param uPageAddr - Page Number to read 
 * @param aData - Destination Address 
 * @return bool(unsigend char) - Success or failure.
 */
#define CopyONDtoMem(a,b,c,d,e) (((bool(*)(unsigned int, unsigned int, unsigned char, unsigned int, unsigned int))(*((unsigned int *)0xD003E010)))(a,b,c,d,e))
//bool ONENAND_ReadPage(unsigned int Controller, unsigned int uBlkAddr, unsigned char uPageAddr, unsigned int* aData)


/**
 * 4 burst word transfer (for enhanced Read performance)
 * bool ONENAND_ReadPage_4burst(u32 Controller, u32 uBlkAddr, u8 uPageAddr, u32* aData) 
 * @param Controller - OneNand Controller Number ('0' fixed) 
 * @param uBlkAddr - Block Number to read
 * @param uPageAddr - Page Number to read
 * @param aData - Destination Address
 * @return bool(unsigend char) - Success or failure.
 */
// #define ONENAND_ReadPage_4burst(a,b,c,d) (((bool(*)(unsigned int, unsigned int, unsigned char, unsigned int*))(*((unsigned int *)0x0C004010)))(a,b,c,d))
 // bool ONENAND_ReadPage_4burst(u32 Controller, u32 uBlkAddr, u8 uPageAddr, u32* aData)


/**
 * 8 burst word transfer (for enhanced Read performance)
 * bool ONENAND_ReadPage_8burst(u32 Controller, u32 uBlkAddr, u8 uPageAddr, u32* aData) 
 * @param Controller - OneNand Controller Number ('0' fixed) 
 * @param uBlkAddr - Block Number to read
 * @param uPageAddr - Page Number to read
 * @param aData - Destination Address
 * @return bool(unsigend char) - Success or failure.
 */
 //#define ONENAND_ReadPage_8burst(a,b,c,d) (((bool(*)(unsigned int, unsigned int, unsigned char, unsigned int*))(*((unsigned int *)0x0C004014)))(a,b,c,d))
 // bool ONENAND_ReadPage_8burst(u32 Controller, u32 uBlkAddr, u8 uPageAddr, u32* aData)


// MMC Card Block Size.
//#define globalBlockSizeHide			*((volatile unsigned int*)(0xD003A400))

#endif

