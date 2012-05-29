#ifndef _animation_h_
#define _animation_h_

#if defined(CONFIG_A8388_V1)||defined(CONFIG_S5PV210_DEVBOARD)||defined(CONFIG_A8388_V2)
#define X0 	(246)
#define Y0 	(418)
#define X1 	(X0+128)
#define Y1 	(Y0+189)

#define BX0 	(385)
#define BY0	(470)
#define BX1	(BX0+45)
#define BY1 	(BY0+151) 

#define MY_LCD_HEIGHT			(600)
#define MY_LCD_WIDTH			(1024)


#define DATA_HEAD_LEN    		8
#define NAND_PAGE_SIZE  		(512)
#define SUB_LOGO_ALL_NUM 		12
#define SUB_LOGO_SIZE			(189*128*4+512)
#define NUM_PER_GROUP			4
#define BATTERY_VOL_LOGO_SIZE 		(27648)
#define BATTERY_VOL_LOGO_NUM	 	(11)
 
#define POWER_ON_LOGO_SIZE		(1024*600*4+512)
#define UPDATE_LOGO_SIZE		(1024*600*4+512)
#define BACKGROUND_LOGO_SIZE		(1024*600*4+512)

#elif defined(CONFIG_S9) || defined(CONFIG_A9)


#ifdef ANGLE_0
#define MY_LCD_HEIGHT			(768)
#define MY_LCD_WIDTH			(1024)

#define X0 	((MY_LCD_HEIGHT/2)-(128/2))
#define Y0 	((MY_LCD_WIDTH/2)-(189/2))
#define X1 	(X0+128)
#define Y1 	(Y0+189)

#define BX0 	(X1)
#define BY0	((Y0+Y1)/2 -151/2)
#define BX1	(BX0+45)
#define BY1 	(BY0+151) 
#elif defined(ANGLE_90)
#elif defined(ANGLE_180)
#define MY_LCD_HEIGHT			(768)
#define MY_LCD_WIDTH			(1024)

#define X0 	((MY_LCD_HEIGHT/2)-(128/2))
#define Y0 	((MY_LCD_WIDTH/2)-(189/2))
#define X1 	(X0+128)
#define Y1 	(Y0+189)

#define BX0 	(X0-45)
#define BY0	((Y0+Y1)/2 - 151/2)
#define BX1	(BX0 + 45)
#define BY1 	(BY0 + 151) 
#elif defined(ANGLE_270)
#endif

#define DATA_HEAD_LEN    		8
#define NAND_PAGE_SIZE  		(512)
#define SUB_LOGO_ALL_NUM 		12
#define SUB_LOGO_SIZE			(189*128*4+512)
#define NUM_PER_GROUP			4
#define BATTERY_VOL_LOGO_SIZE 		(27648)
#define BATTERY_VOL_LOGO_NUM	 	(11)
 
#define POWER_ON_LOGO_SIZE		(1024*768*4+512)
#define UPDATE_LOGO_SIZE		(1024*768*4+512)
#define BACKGROUND_LOGO_SIZE		(1024*768*4+512)
#endif
#endif