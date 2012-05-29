#include "adc.h"

static volatile S5PC210_ADC *g_pADC = S5PC210_BASE_REG_PA_ADC;
int s5pc210_read_adc_data(int ch)
{
	unsigned int preScaler;
	unsigned int rADCCON_save;
  	int data = 0;
    
	/*备份AD寄存器*/
	rADCCON_save = g_pADC->ADCCON;
	/* 初始化AD寄存器 */
	preScaler = 65; //FRQ:1M
	g_pADC->ADCCON |= (0x1<<16) | (0x1<<14) | (preScaler<<6);
	/* 普通模式 */
	g_pADC->ADCCON &=  ~(0x01<<2);
	/* 开始AD转换 */
	g_pADC->ADCCON |=  0x01;
	
	/* 检测AD转换是否使能 */
	while ((g_pADC->ADCCON & (0x1<<0)));
	
	/* 等待转换结束 */
	while (!(g_pADC->ADCCON & (0x1<<15)));
    
	/* 设置读取AD通道*/
	g_pADC->ADCMUX = (g_pADC->ADCMUX & ~(0xf<<0)) | (0<<0);
	/* 读AD转换值 */
	data = g_pADC->ADCDAT0 & 0xFFF;
	/* 还原AD寄存器值 */
	g_pADC->ADCCON = rADCCON_save;

    return data;
}
