#ifndef __adc_H__
#define __adc_H__
#include <common.h>

/*
#define BATT_MAX_ADC      2560
#define BATT_MIN_ADC      2060  //3.3
*/

#define BATTERY_MAX_ADC      3103 // 8.4v
#define BATTERY_MIN_ADC      2517 //6.4 //2217  //6 v
#define BATTERY_MAX_VOLTAGE  8400
#define BATTERY_MIN_VOLTAGE  6000
#define VOLTAGE_OFF       (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)
#define BATTERY_ADC_OFF      (BATTERY_MAX_ADC - BATTERY_MIN_ADC)

#define BATT_VOL_L	2617    //
#define BATT_VOL_M	3000     //8V   >3个小时
#define BATT_VOL_H	3130    //>=8.4V  40分钟左右
#define BG_TIME_OUT 39817567
typedef vu_long		S5PC1XX_REG32;
typedef struct {
	S5PC1XX_REG32	ADCCON;
	S5PC1XX_REG32	ADCTSC;
	S5PC1XX_REG32	ADCDLY;
	S5PC1XX_REG32	ADCDAT0;
	S5PC1XX_REG32	ADCDAT1;
	S5PC1XX_REG32	ADCUPDN;
	S5PC1XX_REG32	ADCCLRINT;
	S5PC1XX_REG32	ADCMUX;
	S5PC1XX_REG32	ADCCLRINTPNDNUP;
} /*__attribute__((__packed__))*/ S5PC210_ADC;

#define S5PC210_BASE_REG_PA_ADC         0xE1700000
int s5pc210_read_adc_data(int ch);

#endif