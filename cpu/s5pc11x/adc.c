#include "adc.h"

static volatile S5PC210_ADC *g_pADC = S5PC210_BASE_REG_PA_ADC;
int s5pc210_read_adc_data(int ch)
{
	unsigned int preScaler;
	unsigned int rADCCON_save;
  	int data = 0;
    
	/*����AD�Ĵ���*/
	rADCCON_save = g_pADC->ADCCON;
	/* ��ʼ��AD�Ĵ��� */
	preScaler = 65; //FRQ:1M
	g_pADC->ADCCON |= (0x1<<16) | (0x1<<14) | (preScaler<<6);
	/* ��ͨģʽ */
	g_pADC->ADCCON &=  ~(0x01<<2);
	/* ��ʼADת�� */
	g_pADC->ADCCON |=  0x01;
	
	/* ���ADת���Ƿ�ʹ�� */
	while ((g_pADC->ADCCON & (0x1<<0)));
	
	/* �ȴ�ת������ */
	while (!(g_pADC->ADCCON & (0x1<<15)));
    
	/* ���ö�ȡADͨ��*/
	g_pADC->ADCMUX = (g_pADC->ADCMUX & ~(0xf<<0)) | (0<<0);
	/* ��ADת��ֵ */
	data = g_pADC->ADCDAT0 & 0xFFF;
	/* ��ԭAD�Ĵ���ֵ */
	g_pADC->ADCCON = rADCCON_save;

    return data;
}
