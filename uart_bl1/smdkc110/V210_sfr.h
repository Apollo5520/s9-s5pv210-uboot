/**************************************************************************************
* 
*	Project Name : S5PV210 Validation
*
*	Copyright 2009 by Samsung Electronics, Inc.
*	All rights reserved.
*
*	Project Description :
*		This software is only for validating functions of the S5PV210.
*		Anybody can use this software without our permission.
*  
*--------------------------------------------------------------------------------------
* 
*	File Name : V210_sfr.h
*  
*	File Description : This file defines SFR base addresses.
*
*	Author : GOM
*	Dept. : AP Development Team
*	Created Date : 2009/02/05
*	Version : 0.0 
* 
*	History
*	- Created(GOM 2009/02/05)
*  
**************************************************************************************/


#ifndef __V210_SFR_H__
#define __V210_SFR_H__

#ifdef __cplusplus
extern "C" {
#endif

//#define	SYSCON_BASE		
#define GPIO_BASE			0xE0200000
//#define OM_REG_ADDR 		0xE0000004
#define OM_STAT				0xE010E100
#define PCLK_DIV_REG 		0xE0100300
#define PWR_CFG 			0xE010C000
#define STOP_CFG 			0xE010C030
#define STOP_MEM_CFG 		0xE010C034

//#define rE_FUSE				(0xF5000018)
//#define rE_FUSEDEVID		(*(volatile unsigned short *)(0x7E00E004))

#define PWMTIMER_BASE			(0xE2500000)
#define SYSCON_BASE				(0x7E00F000)
#define ONDXL_BASE				(0xB0000000)	//OneNAND
#define UART0_BASE				(0xE2900000)
#define UART1_BASE				(0xE2900400)
#define UART2_BASE				(0xE2900800) 
#define UART3_BASE				(0xE2900C00)
#define NANDF_BASE				(0xB0E00000)

#define OTG_LINK_BASE			(0xEC000000)
#define OTG_PHY_BASE			(0xEC100000)

//// Interrupt
#define VIC0_BASE				(0xF2000000)
#define VIC1_BASE				(0xF2100000)
#define VIC2_BASE				(0xF2200000)
#define VIC3_BASE				(0xF2300000)

//SysCon PMU
//#define rOSC_CON					(*(volatile unsigned int*)(0xE0108000))

#define rPWR_CFG					(*(volatile unsigned int*)(0xE010C000))
#define rEINT_WAKEUP_MASK			(*(volatile unsigned int*)(0xE010C004))
#define rNORMAL_CFG					(*(volatile unsigned int*)(0xE010C010))
#define rIDLE_CFG					(*(volatile unsigned int*)(0xE010C020))
#define rSTOP_CFG					(*(volatile unsigned int*)(0xE010C030))
#define rSTOP_MEM_CFG				(*(volatile unsigned int*)(0xE010C034))
#define rSLEEP_CFG					(*(volatile unsigned int*)(0xE010C040))
#define rWAKEUP_STAT				(*(volatile unsigned int*)(0xE010C200))
#define rOTHERS						(*(volatile unsigned int*)(0xE010E000))
#define rOM_STAT					(*(volatile unsigned int*)(0xE010E100))
#define rRST_STAT					(*(volatile unsigned int*)(0xE010A000))


//SysCon CMU
#define rAPLL_LOCK 					(*(volatile unsigned int*)(0xE0100000))
#define rMPLL_LOCK 					(*(volatile unsigned int*)(0xE0100008))
#define rEPLL_LOCK 					(*(volatile unsigned int*)(0xE0100010))
#define rVPLL_LOCK 					(*(volatile unsigned int*)(0xE0100020))

#define rAPLL_CON 					(*(volatile unsigned int*)(0xE0100100))
#define rMPLL_CON 					(*(volatile unsigned int*)(0xE0100108))
#define rEPLL_CON 					(*(volatile unsigned int*)(0xE0100110))
#define rVPLL_CON 					(*(volatile unsigned int*)(0xE0100120))

#define rCLK_SRC0 					(*(volatile unsigned int*)(0xE0100200))
#define rCLK_SRC1 					(*(volatile unsigned int*)(0xE0100204))
#define rCLK_SRC2 					(*(volatile unsigned int*)(0xE0100208))
#define rCLK_SRC3 					(*(volatile unsigned int*)(0xE010020C))
#define rCLK_SRC4 					(*(volatile unsigned int*)(0xE0100210))
#define rCLK_SRC5					(*(volatile unsigned int*)(0xE0100214))
#define rCLK_SRC6 					(*(volatile unsigned int*)(0xE0100218))

#define rCLK_SRC_MASK0				(*(volatile unsigned int*)(0xE0100280))
#define rCLK_SRC_MASK1				(*(volatile unsigned int*)(0xE0100284))

#define rCLK_DIV0					(*(volatile unsigned int*)(0xE0100300))
#define rCLK_DIV1					(*(volatile unsigned int*)(0xE0100304))
#define rCLK_DIV2					(*(volatile unsigned int*)(0xE0100308))
#define rCLK_DIV3					(*(volatile unsigned int*)(0xE010030C))
#define rCLK_DIV4					(*(volatile unsigned int*)(0xE0100310))
#define rCLK_DIV5					(*(volatile unsigned int*)(0xE0100314))
#define rCLK_DIV6					(*(volatile unsigned int*)(0xE0100318))
#define rCLK_DIV7					(*(volatile unsigned int*)(0xE010031C))

#define rCLK_GATE_MAIN0			(*(volatile unsigned int*)(0xE0100400))
#define rCLK_GATE_MAIN1			(*(volatile unsigned int*)(0xE0100404))
#define rCLK_GATE_MAIN0			(*(volatile unsigned int*)(0xE0100408))

#define rCLK_GATE_PERI0				(*(volatile unsigned int*)(0xE0100420))
#define rCLK_GATE_PERI1				(*(volatile unsigned int*)(0xE0100424))

#define rCLK_GATE_SCLK0				(*(volatile unsigned int*)(0xE0100440))
#define rCLK_GATE_SCLK1				(*(volatile unsigned int*)(0xE0100444))

#define rCLK_GATE_IP0				(*(volatile unsigned int*)(0xE0100460))
#define rCLK_GATE_IP1				(*(volatile unsigned int*)(0xE0100464))
#define rCLK_GATE_IP2				(*(volatile unsigned int*)(0xE0100468))
#define rCLK_GATE_IP3				(*(volatile unsigned int*)(0xE010046C))
#define rCLK_GATE_IP4				(*(volatile unsigned int*)(0xE0100470))

#define rCLK_GATE_BLOCK				(*(volatile unsigned int*)(0xE0100480))
#define rCLK_GATE_BUS0				(*(volatile unsigned int*)(0xE0100484))
#define rCLK_GATE_BUS1				(*(volatile unsigned int*)(0xE0100488))

#define rCLK_OUT 					(*(volatile unsigned int*)(0xE0100500))
#define rCLK_DIV_STAT0				(*(volatile unsigned int*)(0xE0101000))
#define rCLK_DIV_STAT1				(*(volatile unsigned int*)(0xE0101004))
#define rCLK_MUX_STAT0				(*(volatile unsigned int*)(0xE0101100))
#define rCLK_MUX_STAT1				(*(volatile unsigned int*)(0xE0101104))

#define rSWRESET 					(*(volatile unsigned int*)(0xE0102000))

//ROM Controller - gom
#define rSROM_BW 					(*(volatile unsigned int*)(0xE8000000))

// chapter28 MoviNAND Interface - gom
#define rHM_SYSAD        (*(volatile unsigned      *)(V210_SDMMC_BASE + 0x0000))	//SDI control register
#define rHM_BLKSIZE      (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x0004))	//Host buffer boundary and transfer block size register
#define rHM_BLKCNT	     (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x0006))	//Block count for current transfer
#define rHM_ARGUMENT     (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0008))	//Command Argument
#define rHM_TRNMOD	     (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x000C))	//Transfer Mode setting register
#define rHM_CMDREG	     (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x000E))	//Command register
#define rHM_RSPREG0      (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0010))	//Response 0
#define rHM_RSPREG1      (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0014))	//Response 1
#define rHM_RSPREG2      (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0018))	//Response 2
#define rHM_RSPREG3      (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x001C))	//Response 3
#define rHM_BDATA        (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0020))	//Buffer Data register
#define rHM_PRNSTS       (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0024))	//Present state
#define rHM_HOSTCTL      (*(volatile unsigned char *)(V210_SDMMC_BASE + 0x0028))	//Host control
#define rHM_PWRCON       (*(volatile unsigned char *)(V210_SDMMC_BASE + 0x0029))	//Power control
#define rHM_BLKGAP       (*(volatile unsigned char *)(V210_SDMMC_BASE + 0x002A))	//Block Gap control
#define rHM_WAKCON       (*(volatile unsigned char *)(V210_SDMMC_BASE + 0x002B))	//Wakeup control
#define rHM_CLKCON       (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x002C))	//Clock control
#define rHM_TIMEOUTCON   (*(volatile unsigned char *)(V210_SDMMC_BASE + 0x002E))	//Time out control
#define rHM_SWRST        (*(volatile unsigned char *)(V210_SDMMC_BASE + 0x002F))	//Software reset
#define rHM_NORINTSTS    (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x0030))	//Normal interrupt status
#define rHM_ERRINTSTS    (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x0032))	//Error interrupt status
#define rHM_NORINTSTSEN  (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x0034))	//Normal interrupt status enable
#define rHM_ERRINTSTSEN  (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x0036))	//Error interrupt status enable
#define rHM_NORINTSIGEN  (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x0038))	//Normal interrupt signal enable
#define rHM_ERRINTSIGEN  (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x003A))	//Error interrupt signal enable
#define rHM_ACMD12ERRSTS (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x003C))	//Auto CMD12 Error Status
#define rHM_CAPAREG  	 (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0040))	//Capability
#define rHM_MAXCURR  	 (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0048))	//Maximum current Capability
#define rHM_CONTROL2  	 (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0080))	//Control 2
#define rHM_CONTROL3  	 (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0084))	//Control 3
#define rHM_DEBUG  		(*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x0088))	//DEBUG
#define rHM_CONTROL4  	 (*(volatile unsigned int  *)(V210_SDMMC_BASE + 0x008C))	//Control 4

#define rHM_HCVER  	     (*(volatile unsigned short*)(V210_SDMMC_BASE + 0x00FE))	//Host controller version

#define	rGPA0CON		(*(volatile u32*) (GPIO_BASE + 0x000)) // UART
#define	rGPA0DAT		(*(volatile u32*) (GPIO_BASE + 0x004))
#define	rGPA0PUD		(*(volatile u32*) (GPIO_BASE + 0x008))

#define	rGPA1CON		(*(volatile u32*) (GPIO_BASE + 0x020)) // UART
#define	rGPA1DAT		(*(volatile u32*) (GPIO_BASE + 0x024))
#define	rGPA1PUD		(*(volatile u32*) (GPIO_BASE + 0x028))

#define	rGPD0CON		(*(volatile u32*) (GPIO_BASE + 0x0A0)) // TOUT
#define	rGPD0DAT		(*(volatile u32*) (GPIO_BASE + 0x0A4))
#define	rGPD0PUD		(*(volatile u32*) (GPIO_BASE + 0x0A8))

#define	rGPG0CON		(*(volatile u32*) (GPIO_BASE + 0x1A0)) // SD0
#define	rGPG0DAT		(*(volatile u32*) (GPIO_BASE + 0x1A4))
#define	rGPG0PUD		(*(volatile u32*) (GPIO_BASE + 0x1A8))

#define	rGPG1CON		(*(volatile u32*) (GPIO_BASE + 0x1C0)) // SD1
#define	rGPG1DAT		(*(volatile u32*) (GPIO_BASE + 0x1C4))
#define	rGPG1PUD		(*(volatile u32*) (GPIO_BASE + 0x1C8))

#define	rGPG2CON		(*(volatile u32*) (GPIO_BASE + 0x1E0)) // SD2
#define	rGPG2DAT		(*(volatile u32*) (GPIO_BASE + 0x1E4))
#define	rGPG2PUD		(*(volatile u32*) (GPIO_BASE + 0x1E8))

#define	rGPG3CON		(*(volatile u32*) (GPIO_BASE + 0x2A0)) // SD3
#define	rGPG3DAT		(*(volatile u32*) (GPIO_BASE + 0x2A4))
#define	rGPG3PUD		(*(volatile u32*) (GPIO_BASE + 0x2A8))

#define	rGPJ1CON		(*(volatile u32*) (GPIO_BASE + 0x260)) // SROM_ADDR_16to22
#define	rGPJ1DAT		(*(volatile u32*) (GPIO_BASE + 0x264))
#define	rGPJ1PUD		(*(volatile u32*) (GPIO_BASE + 0x268))

#define	rGPJ4CON		(*(volatile u32*) (GPIO_BASE + 0x2C0)) // SROM_ADDR_16to22
#define	rGPJ4DAT		(*(volatile u32*) (GPIO_BASE + 0x2C4))
#define	rGPJ4PUD		(*(volatile u32*) (GPIO_BASE + 0x2C8))

#define	rMP0_1CON		(*(volatile u32*) (GPIO_BASE + 0x2E0))
#define	rMP0_1DAT		(*(volatile u32*) (GPIO_BASE + 0x2E4))
#define	rMP0_1PUD		(*(volatile u32*) (GPIO_BASE + 0x2E8))

#define	rMP0_3CON		(*(volatile u32*) (GPIO_BASE + 0x320))
#define	rMP0_3DAT		(*(volatile u32*) (GPIO_BASE + 0x324))
#define	rMP0_3PUD		(*(volatile u32*) (GPIO_BASE + 0x328))

#define	rGPH0CON		(*(volatile u32*) (GPIO_BASE + 0xC00)) 
#define	rGPH0DAT		(*(volatile u32*) (GPIO_BASE + 0xC04))
#define	rGPH0PUD		(*(volatile u32*) (GPIO_BASE + 0xC08))



#ifdef __cplusplus
}
#endif

#endif /*__V210_SFR_H__*/

