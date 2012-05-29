/*
 * Copyright (c) 2009 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * Main: Copy 2 U-Boot from OneNAND to DRAM 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
/* This code support ONLY 4KB OneNAND!!!!! - djpark */

/*
 * Copy U-Boot from OneNAND to DRAM (368KB)
 */

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

typedef enum
{
	UART0, UART1, UART2, UART3
} UART_CH;

typedef struct
{
	unsigned int m_uBaseAddr;
} UART;

#define UART_DEBUG_CHANNEL UART2
#define PCLK_FREQ			(66000000)
#define UART_BOOT_BAUD		(115200.)
#define SD_CHANNEL    0

#define GPIO_BASE			0xE0200000

#define UART0_BASE				(0xE2900000)
#define UART1_BASE				(0xE2900400)
#define UART2_BASE				(0xE2900800) 
#define UART3_BASE				(0xE2900C00)

#define RST_STATE         (0xE010A000)

#define	rGPA0CON		(*(volatile unsigned int*) (GPIO_BASE + 0x000)) // UART
#define	rGPA0DAT		(*(volatile unsigned int*) (GPIO_BASE + 0x004))
#define	rGPA0PUD		(*(volatile unsigned int*) (GPIO_BASE + 0x008))

#define	rGPA1CON		(*(volatile unsigned int*) (GPIO_BASE + 0x020)) // UART
#define	rGPA1DAT		(*(volatile unsigned int*) (GPIO_BASE + 0x024))
#define	rGPA1PUD		(*(volatile unsigned int*) (GPIO_BASE + 0x028))

#define Outp32(addr, data)	(*(volatile unsigned int *)(addr) = (data))
#define Outp16(addr, data)	(*(volatile unsigned short *)(addr) = (data))
#define Outp8(addr, data)	(*(volatile unsigned char *)(addr) = (data))
#define Inp32(addr)			(*(volatile unsigned int *)(addr))
#define Inp16(addr)			(*(volatile unsigned short *)(addr))
#define Inp8(addr)			(*(volatile unsigned char *)(addr))


#define UrtDebug(offset,value)	Outp32(sCh->m_uBaseAddr+offset, value)
#define UrtOutp32(offset,value)	Outp32(sCh->m_uBaseAddr+offset, value)
#define UrtInp32(offset)			Inp32(sCh->m_uBaseAddr+offset)


#define UrtOutp8(offset,value)		Outp8(sCh->m_uBaseAddr+offset, value)
#define UrtInp8(offset)			Inp8(sCh->m_uBaseAddr+offset)

#define LOAD_ADDRESS_PHYSICAL 0x3FE00000

UART oUrtDbg_Debug;

/**
  * This Function copies movinand(MMC) Card Data to memory.
  * Always use EPLL source clock.
  * This function works at 20Mhz.
  * @param StartBlkAddress : Source card(movinand/iNand (MMC)) Address.(It must block address.)
  * @param blockSize : Number of blocks to copy.
  * @param memoryPtr : Buffer to copy from.
  * @param with_init : reinitialize or not
  * @return bool(unsigend char) - Success or failure.
  */
#define CopySDMMCtoMem(z,a,b,c,e)	(((unsigned char(*)(int, unsigned int, unsigned short, unsigned int*, unsigned char))(*((unsigned int *)0xD0037F98)))(z,a,b,c,e))


void(*run_uboot)(void) = LOAD_ADDRESS_PHYSICAL;


void	UART_Debug(UART_CH channel, unsigned int baudRateDiv) {
		UART *sCh;
	
		sCh = &oUrtDbg_Debug;
		UART_InitCh_Debug(channel, sCh);
		UART_Init_Debug( sCh, baudRateDiv );
}

int UART_Init_Debug(UART *sCh, unsigned int baudRateDiv)
{
	int i;
	
	UrtOutp32(UFCON, (1<<2)|(1<<1));
	
	UrtOutp32(UMCON, 0x0);// Modem interrupt, AFC disable 
	UrtOutp32(ULCON, 0x3);// Normal, No parity, 1 stop, 8 bit
 
	// tx,rx=level, disable timeout int., enable rx error int., normal, Tx.Rx:interrupt or polling
#if 0
	UrtOutp32(UCON, (0<<10)|(1<<9)|(1<<8)|
					(1<<7)|(0<<6)|(0<<5)|(0<<4)|
					(1<<2)|(1<<0));
#else
	UrtOutp32(UCON, 0x745); // UART SCLK_UART, Only Transmit mode setting
#endif
//	UrtOutp32(UCON, 0x27B4);

	// set UART FIFO Level.
	UrtOutp32(UFCON, (7<<8)|(7<<4)|(1<<0));


	// baudrate
	UrtOutp32(UBRDIV, baudRateDiv);

#ifdef FPGA
	UrtOutp32(DIVSL, 0x5555); // PCLK:12MHz
#else
	UrtOutp32(DIVSL, 0xDDDD); // PCLK:66MHz
#endif

	if(baudRateDiv!= UrtInp32(UBRDIV)) {
		return 0;
	}
	else {
		return 1;
	}

}

// Base address setting and gpio initialize
void UART_InitCh_Debug(UART_CH eCh, UART *sCh)
{
	if ( eCh == UART0 ) {
		sCh->m_uBaseAddr = UART0_BASE;
		rGPA0CON &= ~0xFFFF;
		rGPA0CON |= 0x2222;
		rGPA0PUD &= ~0xFF;
	}
	else if ( eCh == UART1 ) {
		sCh->m_uBaseAddr = UART1_BASE;
		rGPA0CON &= ~(0xFFFF<<16);
		rGPA0CON |= (0x2222<<16);
		rGPA0PUD &= ~(0xFF<<8);
	}
	else if ( eCh == UART2 ) {
		sCh->m_uBaseAddr = UART2_BASE;
		rGPA1CON &= ~0xFF;
		rGPA1CON |= 0x22;
		rGPA1PUD &= ~(0xF);
	}
	else if ( eCh == UART3 ) {
		sCh->m_uBaseAddr = UART3_BASE;
		rGPA1CON &= ~(0xFF<<8);
		rGPA1CON |= (0x22<<8);
		rGPA1PUD &= ~(0xF<<4);
	}
	else {
		sCh->m_uBaseAddr = UART2_BASE;	//default channel is 2.
		rGPA1CON &= ~0xFF;
		rGPA1CON |= 0x22;
		rGPA1PUD &= ~(0xF);
	}

}

void Uart_SendByte(int data)
{
	UART *sCh;	
	sCh = &oUrtDbg_Debug;

    if(data=='\n')
    {
	while(!(UrtInp32(UTRSTAT)  & 0x2));
	//TimeOutDelay(milli, 1);
	UrtOutp32(UTXH,'\r');
    }
	while(!(UrtInp32(UTRSTAT) & 0x2));   //Wait until THR is empty.
	//TimeOutDelay(milli, 1);
	UrtOutp32(UTXH,data);
}  

void Uart_printf(char *pt)
{
	while(*pt)
	Uart_SendByte(*pt++);
}

//将数字转换成字符===========================================
char hex2char(unsigned int val)
{
    static char str;


    if(val<=9)
        str='0'+val;
    else 
        str=('a'+val-10);
        
    return str;
}

//发送一个16进制文本
void Uart_SendDWORD(unsigned int d)
{
    Uart_printf("0x");
    Uart_SendByte(hex2char((d & 0xf0000000) >> 28));
    Uart_SendByte(hex2char((d & 0x0f000000) >> 24));
    Uart_SendByte(hex2char((d & 0x00f00000) >> 20));    
    Uart_SendByte(hex2char((d & 0x000f0000) >> 16));
    Uart_SendByte(hex2char((d & 0x0000f000) >> 12));
    Uart_SendByte(hex2char((d & 0x00000f00) >> 8));
    Uart_SendByte(hex2char((d & 0x000000f0) >> 4));
    Uart_SendByte(hex2char((d & 0x0000000f) >> 0));
    Uart_printf("\n");
}

//void ComLoad(void)
//{
//	UART *sCh;	
//	unsigned char size[16],buf[128];
//	unsigned long i,tmp,j;
//	unsigned short checksum=0,dnCS;
//	unsigned long DownloadFileSize,filesize=10;
//	unsigned char *dwnPt, *comloadaddress, ch_tmp;
//
//	dwnPt = (unsigned char *)LOAD_ADDRESS_PHYSICAL;
//
////	*dwnPt = 0xAA;
////	*(dwnPt+1) = 0x55;
////	*(dwnPt+2) = 0xAA;
////	*(dwnPt+3) = 0x55;
////	Uart_SendDWORD(*((unsigned int *)dwnPt));
//
//	Uart_printf("Download address: ");
//	Uart_SendDWORD(LOAD_ADDRESS_PHYSICAL);
//	Uart_printf("Now Download file from UART:\n");
//	sCh = &oUrtDbg_Debug;
//
//	i=0;
//  while(i<filesize)
//  {
//    while(!(UrtInp32(UTRSTAT)&0x00000001));
//    j = UrtInp32(UFSTAT)&0x000000FF;
//  	if(j>0)
//  	{
////      Uart_SendDWORD(j);
//  	  while(j-- > 0)
//  	  {
//        if(i<=3)
//          buf[i] = UrtInp32(URXH)&0x000000FF;
//        else
//        {
//          *(dwnPt+i-4) = UrtInp32(URXH)&0x000000FF;
//        }
//        if(i==3)
//        {
//          filesize = buf[0] | buf[1]<<8 | buf[2]<<16 | buf[3]<<24;
//          Uart_printf("filesize = ");
//          Uart_SendDWORD(filesize);
//        }
//    		i++;
//  	  }
//  	}
//  }
////  memcpy((void*)LOAD_ADDRESS_PHYSICAL, buf, i);
////  sprintf(size," filesize=%d\n",i);
////  Uart_printf(size);
//	Uart_printf("receive done: ");
//	Uart_SendDWORD(i);
//}

void Main(void)
{
  UART_Debug(UART_DEBUG_CHANNEL, (PCLK_FREQ/16./UART_BOOT_BAUD)-1);
 #if 1 
  Outp32(0xE0200280, 0x00100000);
//  Outp32(0xE0200284, 0x00000020);
  Outp32(0xE0200284, 0x00000000);
 #endif
  Uart_printf("\nSKDV210 sd0 bl1 boot by ApolloYang\n\n");
	Uart_printf("RST_STAT: ");
  Uart_SendDWORD(Inp32(RST_STATE));
//  ComLoad();
//  LoadFromUSB();
	if(!(CopySDMMCtoMem(SD_CHANNEL, 17, 6143, (unsigned int*)LOAD_ADDRESS_PHYSICAL, 0)))
	  Uart_printf("\ncopy sd to ram error!\n");		
	run_uboot();
}

