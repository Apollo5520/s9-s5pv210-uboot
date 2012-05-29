/**************************************************************************************
* 
*	Project Name : S5PV210 Validation
*
*	Copyright 2006 by Samsung Electronics, Inc.
*	All rights reserved.
*
*	Project Description :
*		This software is only for validating functions of the S5PC100.
*		Anybody can use this software without our permission.
*  
*--------------------------------------------------------------------------------------
* 
*	File Name : option.h
*  
*	File Description : This file defines basic setting and configuration.
*
*	Author : Haksoo,Kim
*	Dept. : AP Development Team
*	Created Date : 2006/11/08
*	Version : 0.1 
* 
*	History
*	- Created(Haksoo,Kim 2006/11/08)
*  
**************************************************************************************/

#ifndef __OPTION_H__
#define __OPTION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"

//#define	FPGA

#define 	_DRAM_BaseAddress			_DRAM1_BaseAddress
#define _DRAM_LimitAddress			_DRAM1_LimitAddress


#define	_DRAM0_BaseAddress		0x20000000
#define	_DRAM0_LimitAddress		0x28000000		// general 
#define	_DRAM1_BaseAddress		0x40200020		//yin
#define	_DRAM1_LimitAddress		0x48000000		// general 

#define ISRAM_ADDRESS 			(0xD0020000)
#define SDM_Size					(0x00010000)
#define SECOND_BL_ADDRESS 		(0xD0030000) // 0xD003_0000
#define CHECKSUM_ADDRESS		(0xD0031FFC)
#define SECURE_KEY_ADDRESS	(0xD0039400)
#define GLOBAL_VAL_BASE		(0xD003A500)


#define BL3_START_ADDR			(0x20010000)
#define BL3_START_ADDR_NOR	(0x80002000)



#define NOR_BL_ADDRESS 		(0x80000000) // SROMC0

#define SEC_BOOT_CONTEXT_SIZE 1024 // 1KB

#define CURRENT_BOOTDEVICE			*((volatile unsigned int*)(GLOBAL_VAL_BASE+0xC))
#define V210_SDMMC_BASE			*((volatile unsigned int*)(GLOBAL_VAL_BASE+0x8))
#define globalSDHCInfoBit				*((volatile unsigned int*)(GLOBAL_VAL_BASE+0x4))
#define globalBlockSize				*((volatile unsigned int*)(GLOBAL_VAL_BASE+0x0))


#define IROM_NAND_BOOT 		0x0
#define IROM_ONENAND_BOOT 	0x1
#define IROM_DENALI_MUX_BOOT 	0x2
#define IROM_AUDI_MUX_BOOT 	0x3
#define IROM_AUDI_DEMUX_BOOT 	0x4
#define IROM_MOVINAND_BOOT 	0x6
#define IROM_eMMC_4bit_BOOT 	0x7
#define IROM_eMMC_8bit_BOOT 	0xb
#define IROM_NOR_BOOT 			0xa
#define IROM_UART_USB_BOOT 	0x10

#define PCLK_DIV_REG 0xE0100300
#define PWR_CFG 0xE0108000
#define STOP_CFG 0xE0108014
#define STOP_MEM_CFG 0xE010801C

#define E_FUSE_ADDR 0xE0E00018 // V210 e-Fuse : Secure Boot Key 0 ~ 4
#define E_FUSE_BYTE_SIZE 16 // 4word = 16byte = 128 bit


#define PAGE_SIZE_512B 0
#define PAGE_SIZE_2KB 1
#define PAGE_SIZE_4KB 2
#define PAGE_SIZE_4KB_16ECC 3

#define DIVck100		1
#define DIVck200		1
#define DIVapll		1


enum UART_REG
{
	ULCON 			=	0x00,
	UCON  			=	0x04,
	UFCON 			=	0x08,
	UMCON 	 		=	0x0C,
	UTRSTAT		=	0x10,
	UERSTAT		=	0x14,
	UFSTAT			=	0x18,
	UMSTAT			=	0x1C,
	UBRDIV			=	0x28,
	DIVSL			=	0x2C,
	INTP			= 	0x30,
	INTSP			=	0x34,
	INTM			=	0x38,
	UTXH			=	0x20,
	URXH			=	0x24,
	UART_CH0		=	0x0,
	UART_CH1		=	0x400,
	UART_CH2		=	0x800,
	UART_CH3		=	0xc00
};

#define UrtDebug(offset,value)	Outp32(sCh->m_uBaseAddr+offset, value)
#define UrtOutp32(offset,value)	Outp32(sCh->m_uBaseAddr+offset, value)
#define UrtInp32(offset)			Inp32(sCh->m_uBaseAddr+offset)


#define UrtOutp8(offset,value)		Outp8(sCh->m_uBaseAddr+offset, value)
#define UrtInp8(offset)			Inp8(sCh->m_uBaseAddr+offset)


#define UART_BOOT_CHANNEL	UART1
#define UART_DEBUG_CHANNEL UART1

#ifdef FPGA
#define PCLK_FREQ			(12000000)
#else
#define PCLK_FREQ			(66000000)
#endif

#define UART_BOOT_BAUD		(115200.)


typedef enum
{
	UART0, UART1, UART2, UART3
} UART_CH;

typedef struct
{
	u32 m_uBaseAddr;
} UART;

#define DEEPIDLE_WAKEUP (1<<19)
#define DEEPSTOP_WAKEUP (1<<18)
#define SLEEP_WAKEUP (1<<16)
#define SWRESET_WAKEUP (1<<3)
#define WDTRESET_WAKEUP (1<<2)
#define WARMRESET_WAKEUP (1<<1)
#define NRESET_WAKEUP (1<<0)


#ifdef __cplusplus
}
#endif

#endif /*__OPTION_H__*/

