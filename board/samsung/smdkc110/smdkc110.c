/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <regs.h>
#include <asm/io.h>


/* ------------------------------------------------------------------------- */
#define SMC9115_Tacs  (0x0)  // 0clk    address set-up
#define SMC9115_Tcos  (0x4)  // 4clk    chip selection set-up
#define SMC9115_Tacc  (0xe)  // 14clk  access cycle
#define SMC9115_Tcoh  (0x1)  // 1clk    chip selection hold
#define SMC9115_Tah  (0x4)  // 4clk    address holding time
#define SMC9115_Tacp  (0x6)  // 6clk    page mode access cycle
#define SMC9115_PMC  (0x0)  // normal(1data)page mode configuration

#define SROM_DATA16_WIDTH(x)  (1<<((x*4)+0))
#define SROM_WAIT_ENABLE(x)  (1<<((x*4)+1))
#define SROM_BYTE_ENABLE(x)  (1<<((x*4)+2))

/* ------------------------------------------------------------------------- */
#define DM9000_Tacs  (0x0)  // 0clk    address set-up
#define DM9000_Tcos  (0x4)  // 4clk    chip selection set-up
#define DM9000_Tacc  (0xE)  // 14clk  access cycle
#define DM9000_Tcoh  (0x1)  // 1clk    chip selection hold
#define DM9000_Tah  (0x4)  // 4clk    address holding time
#define DM9000_Tacp  (0x6)  // 6clk    page mode access cycle
#define DM9000_PMC  (0x0)  // normal(1data)page mode configuration

int resume_system;
int sdfuse_system;
int force_erase_flag;
extern volatile int is_lcdon;
static inline void delay(unsigned long loops)
{
  __asm__ volatile ("1:\n" "subs %0, %1, #1\n" "bne 1b":"=r" (loops):"0"(loops));
}

#ifdef FORCE_EARSE_XINWANJIA  /*新万佳--*/
void force_erase()
{
  char run_cmd[128];
  printf("force_erase_flag = %d\n", force_erase_flag);
  if(force_erase_flag)
  {
    show_sdfuse_pic();
    run_command("sdfuse erase userdata", 0);
    run_command("sdfuse erase cache", 0);
    sprintf(run_cmd, "fatformat mmc %d:%d", BOOT_MMC, EXTSD_PART_NUM);
    run_command(run_cmd, 0);
    mdelay10(100);
    show_pic(PIC_BOOT, 0, 0, 0, 0, 0);
  }
}
#endif /* FORCE_EARSE_XINWANJIA */
/*
 * Miscellaneous platform dependent initialisations
 */

int check_lcd_mode()
{
  //PIXELQI 模式切换 GPH3(2)
  unsigned int val;
  val = readl(GPH3CON) & ~(0xF<<8);
  writel(val, GPH3CON);
  if(readl(GPH3DAT) & (0x1<<2))
    val = 0;
  else
    val = 1;
  
//  printf("check_lcd_mode %d\n", val);
  return val;
}

#if defined(CONFIG_S5PV210_DEVBOARD) || defined(CONFIG_A8388_V1) || defined(CONFIG_A8388_V2)
void lcd_on()
{
  unsigned int tmp;
  //lcd power ctl GPH2(5) DS90C365
  tmp = readl(GPH2CON) & ~(0xF<<20) | (0x1<<20);
  writel(tmp, GPH2CON);
  tmp = readl(GPH2DAT) | (0x1<<5);
  writel(tmp, GPH2DAT);

  //lvds chip power ctl GPJ2(3) 
#ifdef CONFIG_LCD_PIXELQI
  tmp = readl(GPJ2CON) & ~(0xF<<12) | (0x1<<12);
  writel(tmp, GPJ2CON);
  tmp = readl(GPJ2DAT) | (0x1<<3);
  writel(tmp, GPJ2DAT);

  //led onoff  GPH0(6)
  tmp = readl(GPH0CON) & ~(0xF<<24) | (0x1<<24);
  writel(tmp, GPH0CON);
  if(check_lcd_mode())
    tmp = readl(GPH0DAT) | (0x1<<6);
  else
    tmp = readl(GPH0DAT) & ~(0x1<<6);   

	//tmp = readl(GPH0DAT) | (0x1<<6);
  writel(tmp, GPH0DAT);
#endif

#ifdef CONFIG_LCD_LG
  //lcd power ctl GPG3(0) DS90C385
  tmp = readl(GPG3CON) & ~(0xF<<0) | (0x1<<0);
  writel(tmp, GPG3CON);
  tmp = readl(GPG3DAT) | (0x1<<0);
  writel(tmp, GPG3DAT);

  //lvds chip power ctl GPG3(1) 
  tmp = readl(GPG3CON) & ~(0xF<<4) | (0x1<<4);
  writel(tmp, GPG3CON);
  tmp = readl(GPG3DAT) | (0x1<<1);
  writel(tmp, GPG3DAT);

  //lcd LDO_EB onoff  GPG3(2)
  tmp = readl(GPG3CON) & ~(0xF<<8) | (0x1<<8);
  writel(tmp, GPG3CON);
  tmp = readl(GPG3DAT) | (0x1<<2);
  writel(tmp, GPG3DAT);

  //led onoff  GPG3(3)
  tmp = readl(GPG3CON) & ~(0xF<<12) | (0x1<<12);
  writel(tmp, GPG3CON);
  tmp = readl(GPH0DAT) | (0x1<<3);
  writel(tmp, GPH0DAT);
#endif

  //pwm on GPD0(0)
  tmp = readl(GPD0CON) & ~(0xF<<0) | (0x1<<0);
  writel(tmp, GPD0CON);
  tmp = readl(GPD0DAT) | (0x1<<0);
  writel(tmp, GPD0DAT);

  //lcd led power on   GPJ2(2)
  tmp = readl(GPJ2CON) & ~(0xF<<8) | (0x1<<8);
  writel(tmp, GPJ2CON);
#ifdef CONFIG_LCD_PIXELQI
  if(check_lcd_mode())
    tmp = readl(GPJ2DAT) | (0x1<<2);
  else
    tmp = readl(GPJ2DAT) & ~(0x1<<2);    
#else
  tmp = readl(GPJ2DAT) | (0x1<<2);
#endif
  writel(tmp, GPJ2DAT);

#if defined(CONFIG_S5PV210_DEVBOARD)
//核心板lcd接口
  //lcd led power on   GPH2(3)
  tmp = readl(GPH2CON) & ~(0xF<<12) | (0x1<<12);
  writel(tmp, GPH2CON);
  tmp = readl(GPH2DAT) | (0x1<<3);
  writel(tmp, GPH2DAT);

//hdmi GPJ3(1)
  tmp = readl(GPJ3CON) & ~(0xF<<4) | (0x1<<4);
  writel(tmp, GPJ3CON);
  tmp = readl(GPJ3DAT) | (0x1<<1);
  writel(tmp, GPJ3DAT);

#endif
//  RTC 32.768 output
  writel(0x200, 0xE2800040);
  printf("RTCCON = 0x%08x\n", readl(0xE2800040));
  printf("TICNT  = 0x%08x\n", readl(0xE2800044));
}
#endif /* CONFIG_S5PV210_DEVBOARD CONFIG_A8388_V1 CONFIG_A8388_V2 */

#if defined(CONFIG_S9) || defined(CONFIG_A9)
void lcd_on()
{
  unsigned int tmp;

printf("CLK_SRC6 = 0x%08x\n", readl(0xE0100218));
  //lcd power ctl GPH2(5) DS90C385
  tmp = readl(GPH2CON) & ~(0xF<<20) | (0x1<<20);
  writel(tmp, GPH2CON);
  tmp = readl(GPH2DAT) | (0x1<<5);
  writel(tmp, GPH2DAT);

////reset cpld  GPH3(0)
//  tmp = readl(GPH3CON) & ~(0xF<<0) | (0x1<<0);
//  writel(tmp, GPH3CON);
//
//  tmp = readl(GPH3DAT) & ~(0x1<<0);
//  writel(tmp, GPH3DAT);
//  
//  mdelay10(1);
//  tmp = readl(GPH3DAT) | (0x1<<0);
//  writel(tmp, GPH3DAT);
//
//  mdelay10(1);
//  tmp = readl(GPH3DAT) & ~(0x1<<0);
//  writel(tmp, GPH3DAT);

  //lvds chip power ctl GPJ2(3) 
  tmp = readl(GPJ2CON) & ~(0xF<<12) | (0x1<<12);
  writel(tmp, GPJ2CON);
  tmp = readl(GPJ2DAT) | (0x1<<3);
  writel(tmp, GPJ2DAT);

  //pwm on GPD0(0)
  tmp = readl(GPD0CON) & ~(0xF<<0) | (0x1<<0);
  writel(tmp, GPD0CON);
  tmp = readl(GPD0DAT) | (0x1<<0);
  writel(tmp, GPD0DAT);

  writel(0x200, 0xE2800040);
  printf("RTCCON = 0x%08x\n", readl(0xE2800040));
  printf("TICNT  = 0x%08x\n", readl(0xE2800044));
}

void reset_cpld()
{
  unsigned int tmp;

#ifdef CONFIG_LCD_LG
//reset cpld  GPH3(0)
  tmp = readl(GPH3CON) & ~(0xF<<0) | (0x1<<0);
  writel(tmp, GPH3CON);

//  tmp = readl(GPH3DAT) & ~(0x1<<0);
//  writel(tmp, GPH3DAT);
//  
//  mdelay10(1);
  tmp = readl(GPH3DAT) | (0x1<<0);
  writel(tmp, GPH3DAT);

  mdelay10(60);
//  mdelay10(1);
  tmp = readl(GPH3DAT) & ~(0x1<<0);
  writel(tmp, GPH3DAT);
#endif

  //lcd led power on   GPJ2(2)
  tmp = readl(GPJ2CON) & ~(0xF<<8) | (0x1<<8);
  writel(tmp, GPJ2CON);
  tmp = readl(GPJ2DAT) | (0x1<<2);
  writel(tmp, GPJ2DAT);

}
#endif /* CONFIG_S9 */

#if defined(CONFIG_A8388_V1)||defined(CONFIG_A8388_V2)
void power_init_off()
{
  unsigned int val;
  //sys_pwr_ctl  GPH2(4)
  val = readl(GPH2CON) & ~(0xF<<16) | (0x1<<16);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) & ~(0x1<<4);
  writel(val, GPH2DAT);

  //lcd_pwr_ctl  GPH2(5)
  val = readl(GPH2CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) & ~(0x1<<5);
  writel(val, GPH2DAT);

  //gps_pwr_ctl  GPH0(5)
  val = readl(GPH0CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPH0CON);
  val = readl(GPH0DAT) & ~(0x1<<5);
  writel(val, GPH0DAT);

  //3g_pwr_ctl   GPJ2(0)
  val = readl(GPJ2CON) & ~(0xF<<0) | (0x1<<0);
  writel(val, GPJ2CON);
  val = readl(GPJ2DAT) & ~(0x1<<0);
  writel(val, GPJ2DAT);

  //bled_pwr_ctl GPJ2(2)
  val = readl(GPJ2CON) & ~(0xF<<8) | (0x1<<80);
  writel(val, GPJ2CON);
  val = readl(GPJ2DAT) & ~(0x1<<2);
  writel(val, GPJ2DAT);

  //tp_pwr_ctl   GPJ3(2)
  val = readl(GPJ3CON) & ~(0xF<<8) | (0x1<<8);
  writel(val, GPJ3CON);
  val = readl(GPJ3DAT) & ~(0x1<<2);
  writel(val, GPJ3DAT);

  //wifi_pwr_ctl GPJ3(5)
  val = readl(GPJ3CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPJ3CON);
  val = readl(GPJ3DAT) & ~(0x1<<5);
  writel(val, GPJ3DAT);

  //cam_pwr_ctl  GPJ4(0)
  val = readl(GPJ4CON) & ~(0xF<<0) | (0x1<<0);
  writel(val, GPJ4CON);
  val = readl(GPJ4DAT) & ~(0x1<<0);
  writel(val, GPJ4DAT);
}
#endif /* CONFIG_A8388_V1 CONFIG_A8388_V2 */

#if defined(CONFIG_S5PV210_DEVBOARD)
void power_init_off()
{
  unsigned int val;
  //sys_pwr_ctl  GPH2(4)
  val = readl(GPH2CON) & ~(0xF<<16) | (0x1<<16);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) & ~(0x1<<4);
  writel(val, GPH2DAT);

  //lcd_pwr_ctl  GPH2(5)
  val = readl(GPH2CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) & ~(0x1<<5);
  writel(val, GPH2DAT);

  //gps_pwr_ctl  GPH0(5)
  val = readl(GPH0CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPH0CON);
  val = readl(GPH0DAT) & ~(0x1<<5);
  writel(val, GPH0DAT);

  //3g_pwr_ctl   GPJ2(0)
  val = readl(GPJ2CON) & ~(0xF<<0) | (0x1<<0);
  writel(val, GPJ2CON);
  val = readl(GPJ2DAT) & ~(0x1<<0);
  writel(val, GPJ2DAT);

  //bled_pwr_ctl GPJ2(2)
  val = readl(GPJ2CON) & ~(0xF<<8) | (0x1<<80);
  writel(val, GPJ2CON);
  val = readl(GPJ2DAT) & ~(0x1<<2);
  writel(val, GPJ2DAT);

  //tp_pwr_ctl   GPJ3(2)
  val = readl(GPJ3CON) & ~(0xF<<8) | (0x1<<8);
  writel(val, GPJ3CON);
  val = readl(GPJ3DAT) & ~(0x1<<2);
  writel(val, GPJ3DAT);

  //wifi_pwr_ctl GPJ3(5)
  val = readl(GPJ3CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPJ3CON);
  val = readl(GPJ3DAT) & ~(0x1<<5);
  writel(val, GPJ3DAT);

  //cam_pwr_ctl  GPJ4(0)
  val = readl(GPJ4CON) & ~(0xF<<0) | (0x1<<0);
  writel(val, GPJ4CON);
  val = readl(GPJ4DAT) & ~(0x1<<0);
  writel(val, GPJ4DAT);
}
#endif /* CONFIG_S5PV210_DEVBOARD */

#if defined(CONFIG_S9) 
void power_init_off()
{
  unsigned int val;
  //sys_pwr_ctl  GPH2(4)
  val = readl(GPH2CON) & ~(0xF<<16) | (0x1<<16);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) & ~(0x1<<4);
  writel(val, GPH2DAT);

  //lcd_pwr_ctl  GPH2(5)
  val = readl(GPH2CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) & ~(0x1<<5);
  writel(val, GPH2DAT);

  //lvds chip power ctl GPJ2(3) 
  val = readl(GPJ2CON) & ~(0xF<<12) | (0x1<<12);
  writel(val, GPJ2CON);
  val = readl(GPJ2DAT) & ~(0x1<<3);
  writel(val, GPJ2DAT);

  //gps_pwr_ctl  GPH0(5)
  val = readl(GPH0CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPH0CON);
  val = readl(GPH0DAT) & ~(0x1<<5);
  writel(val, GPH0DAT);

  //bled_pwr_ctl GPJ2(2)
  val = readl(GPJ2CON) & ~(0xF<<8) | (0x1<<80);
  writel(val, GPJ2CON);
  val = readl(GPJ2DAT) & ~(0x1<<2);
  writel(val, GPJ2DAT);

  //tp_pwr_ctl   GPJ3(2)
  val = readl(GPJ3CON) & ~(0xF<<8) | (0x1<<8);
  writel(val, GPJ3CON);
  val = readl(GPJ3DAT) & ~(0x1<<2);
  writel(val, GPJ3DAT);

  //wifi_pwr_ctl GPJ3(5)
  val = readl(GPJ3CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPJ3CON);
  val = readl(GPJ3DAT) & ~(0x1<<5);
  writel(val, GPJ3DAT);

  //cam_pwr_ctl  GPJ4(0)
  val = readl(GPJ4CON) & ~(0xF<<0) | (0x1<<0);
  writel(val, GPJ4CON);
  val = readl(GPJ4DAT) & ~(0x1<<0);
  writel(val, GPJ4DAT);
}
#endif /* CONFIG_S9 */
#if defined(CONFIG_A9)
void power_init_off()
{
  unsigned int val;
  //sys_pwr_ctl  GPH2(4)
  val = readl(GPH2CON) & ~(0xF<<16) | (0x1<<16);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) & ~(0x1<<4);
  writel(val, GPH2DAT);

  //gps_pwr_ctl  GPJ3(0)
  val = readl(GPJ3CON) & ~(0xF<<0) | (0x1<<0);
  writel(val, GPJ3CON);
  val = readl(GPJ3DAT) & ~(0x1<<0);
  writel(val, GPJ3DAT);

  //lcd_pwr_ctl  GPH2(5)
  val = readl(GPH2CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) & ~(0x1<<5);
  writel(val, GPH2DAT);

  //bled_pwr_ctl GPJ2(2)
  val = readl(GPJ2CON) & ~(0xF<<8) | (0x1<<8);
  writel(val, GPJ2CON);
  val = readl(GPJ2DAT) & ~(0x1<<2);
  writel(val, GPJ2DAT);

  //lvds chip power ctl GPJ2(3) 
  val = readl(GPJ2CON) & ~(0xF<<12) | (0x1<<12);
  writel(val, GPJ2CON);
  val = readl(GPJ2DAT) & ~(0x1<<3);
  writel(val, GPJ2DAT);

  //bled_brt GPD0(0)
  val = readl(GPD0CON) & ~(0xF<<0) | (0x1<<0);
  writel(val, GPD0CON);
  val = readl(GPD0DAT) & ~(0x1<<0);
  writel(val, GPD0DAT);

  //tp_pwr_ctl   GPJ3(2)
  val = readl(GPJ3CON) & ~(0xF<<8) | (0x1<<8);
  writel(val, GPJ3CON);
  val = readl(GPJ3DAT) & ~(0x1<<2);
  writel(val, GPJ3DAT);

  //wifi_pwr_ctl GPJ3(5)
  val = readl(GPJ3CON) & ~(0xF<<20) | (0x1<<20);
  writel(val, GPJ3CON);
  val = readl(GPJ3DAT) & ~(0x1<<5);
  writel(val, GPJ3DAT);

  //cam_pwr_ctl  GPJ4(0)
  val = readl(GPJ4CON) & ~(0xF<<0) | (0x1<<0);
  writel(val, GPJ4CON);
  val = readl(GPJ4DAT) & ~(0x1<<0);
  writel(val, GPJ4DAT);
}
#endif

void mdelay10(int count)
{
  int i;
  for(i=0; i<count; i++)
    udelay(10000);    
}

#if defined(CONFIG_S5PV210_DEVBOARD)
void check_camera()
{
  unsigned char buf[2];
  unsigned int val;

  //cam_pwr_ctl  GPJ4(0)
  val = readl(GPJ4CON) & ~(0xF<<0) | (0x1<<0);
  writel(val, GPJ4CON);
  val = readl(GPJ4DAT) & ~(0x1<<0);
  writel(val, GPJ4DAT);

  //cam_reset  GPJ4(4)
  val = readl(GPJ4CON) & ~(0xF<<16) | (0x1<<16);
  writel(val, GPJ4CON);
  val = readl(GPJ4DAT) & ~(0x1<<4);
  writel(val, GPJ4DAT);

  //cam_pwrden  GPJ4(2)
  val = readl(GPJ4CON) & ~(0xF<<8) | (0x1<<8);
  writel(val, GPJ4CON);
  val = readl(GPJ4DAT) & ~(0x1<<2);
  writel(val, GPJ4DAT);
  mdelay10(1);

  writel(val|(0x1<<0), GPJ4DAT);
  mdelay10(1);
  writel(val|(0x1<<4), GPJ4DAT);
  mdelay10(1);
  writel(val|(0x1<<2), GPJ4DAT);
  mdelay10(1);

  i2c_set_bus_num(2);
  for(val=0; val<20; val++)
  {
    buf[0] = 0;
    buf[1] = 0;
    i2c_read (0x3C, 0x0, 2, buf, 2);
    printf("camera: 0x%02x%02x\n", buf[0], buf[1]);
    mdelay10(100);
  }
}

void check_power_key()
{
  unsigned int val;
  int check_time = 0;
  //power_key  GPH1(2)
  
  val = readl(GPH1CON) & ~(0xF<<8);
  writel(val, GPH1CON);
  
  while(!(readl(GPH1DAT) & (0x1<<2)))
  {
    gotowork();	
    is_lcdon = 1;
    mdelay10(10); //100ms
    check_time++;
    if(check_time >= CHECK_KEY_TIME)
    goto POWER_KEY_ON;
    //  break;
  }
 /* 
  if(check_time < CHECK_KEY_TIME)
  {
  	disable_interrupts ();
  	printf("boot key press too short\n");
  	reset_cpu (0);
    while(1);
  }
 */ 
POWER_KEY_OFF:  
  	return 0;
POWER_KEY_ON:
  //sys_pwr_ctl  GPH2(4)
  val = readl(GPH2CON) & ~(0xF<<16) | (0x1<<16);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) | (0x1<<4);
  writel(val, GPH2DAT);
//  check_camera();
}

#endif /* CONFIG_S5PV210_DEVBOARD */

#if defined(CONFIG_A8388_V1) || defined(CONFIG_S9) || defined(CONFIG_A8388_V2) || defined(CONFIG_A9)
int check_power_key()
{
  unsigned int val;
  int check_time = 0;
  //power_key  GPH0(1)
  val = readl(GPH0CON) & ~(0xF<<0);
  writel(val, GPH0CON);
  
  //other key init
#if defined(CONFIG_A8388_V1) || defined(CONFIG_S9) || defined(CONFIG_A8388_V2)  
//valume +  GPH3(4)
  val = readl(GPH3PUD) & ~(0x3<<8) | (0x2<<8);
  writel(val, GPH3PUD);
  val = readl(GPH3CON) & ~(0xF<<16);
  writel(val, GPH3CON);
//valume -  GPH3(3)
  val = readl(GPH3PUD) & ~(0x3<<6) | (0x2<<6);
  writel(val, GPH3PUD);
  val = readl(GPH3CON) & ~(0xF<<12);
  writel(val, GPH3CON);
//screen_switch - GPH3(2)
  val = readl(GPH3PUD) & ~(0x3<<4) | (0x2<<4);
  writel(val, GPH3PUD);
  val = readl(GPH3CON) & ~(0xF<<8);
  writel(val, GPH3CON);
#elif defined(CONFIG_A9)
//valume +  GPH3(1)
  val = readl(GPH3PUD) & ~(0x3<<2) | (0x2<<2);
  writel(val, GPH3PUD);
  val = readl(GPH3CON) & ~(0xF<<4);
  writel(val, GPH3CON);
//valume -  GPH3(2)
  val = readl(GPH3PUD) & ~(0x3<<4) | (0x2<<4);
  writel(val, GPH3PUD);
  val = readl(GPH3CON) & ~(0xF<<8);
  writel(val, GPH3CON);
#endif
//home_key - GPH1(2)
  val = readl(GPH1PUD) & ~(0x3<<4) | (0x2<<4);
  writel(val, GPH1PUD);
  val = readl(GPH1CON) & ~(0xF<<8);
  writel(val, GPH1CON);

  while(!(readl(GPH0DAT) & (0x1<<1)))
  {	
    gotowork();	
    is_lcdon = 1;
    mdelay10(10); //100ms
    check_time++;
    if(check_time >= CHECK_KEY_TIME)
      goto POWER_KEY_ON;
      //break;
  }
  /*
  if(check_time < CHECK_KEY_TIME)
  {
  	disable_interrupts ();
    val = readl(GPH2CON) & ~(0xF<<16) | (0x1<<16);
    writel(val, GPH2CON);
    val = readl(GPH2DAT) & (~(0x1<<4));
    writel(val, GPH2DAT);
    mdelay10(10);
  	reset_cpu (0);
    while(1);
  }
*/
POWER_KEY_OFF:  
  return 0;
POWER_KEY_ON:
  //sys_pwr_ctl  GPH2(4)
  val = readl(GPH2CON) & ~(0xF<<16) | (0x1<<16);
  writel(val, GPH2CON);
  val = readl(GPH2DAT) | (0x1<<4);
  writel(val, GPH2DAT);
  force_erase_flag = check_force_erase_key();
  return 1;
}
#endif /* CONFIG_A8388_V1 CONFIG_S9 CONFIG_A8388_V2*/

int check_resume_key()
{
#if defined(CONFIG_A8388_V1) || defined(CONFIG_S9) || defined(CONFIG_A8388_V2)
  if(readl(GPH3DAT) & (0x1<<4))
    return 0;
  else
    return 1;
#elif defined(CONFIG_A9)
  if(readl(GPH3DAT) & (0x1<<1))
    return 0;
  else
    return 1;   
#elif defined(CONFIG_S5PV210_DEVBOARD)
  unsigned int val;
//GPH1(2)
  val = readl(GPH1CON) & ~(0xF<<8);
  writel(val, GPH1CON);
  if(readl(GPH1DAT) & (0x1<<2))
    return 0;
  else
    return 1;
#else
  return 0;
#endif
}

int check_sdfuse_key()
{
#if defined(CONFIG_A8388_V1) || defined(CONFIG_S9) || defined(CONFIG_A8388_V2) 
  if((readl(GPH3DAT) & (0x1<<3)) || (readl(GPH1DAT) & (0x1<<2)))
    return 0;
  else
    return 1;
#elif defined(CONFIG_A9)
  if((readl(GPH3DAT) & (0x1<<2)) || (readl(GPH1DAT) & (0x1<<2)))
    return 0;
  else
    return 1;
#else
  return 0;
#endif
}

int check_force_erase_key()
{//home and power
  if((readl(GPH0DAT) & (0x1<<1)) || (readl(GPH1DAT) & (0x1<<2)))
    return 0;
  else
    return 1;
}

//#ifdef CONFIG_AUTO_SDFUSE
#include "auto_sdfuse.h"
void show_sdfuse_pic(void)
{
	#if defined(CONFIG_S9) || defined(CONFIG_A9)
	show_sdfluse_pic(pic_auto_sdfuse, 0, 0, 1, 1, 1);
	#else
  show_pic(pic_auto_sdfuse, 0, 0, 1, 1, 1);
	#endif
}

void auto_sdfuse()
{
  char cmd[16];

  sprintf(cmd, "fdisk -c %d", BOOT_MMC);
  run_command(cmd, 0);
  run_command("sdfuse flashall", 0);
	reset_cpu (0);
  while(1);
}
//#endif /* CONFIG_AUTO_SDFUSE */

static void dm9000_pre_init(void)
{
  unsigned int tmp;
//Apollo + power on GPJ4[3]
  tmp = readl(GPJ4CON);
  tmp = (tmp & (~(0xF<<12))) | (0x1<<12);
  writel(tmp, GPJ4CON);
  
  tmp = readl(GPJ4DAT);
  tmp = tmp | (0x1<<3);
  writel(tmp, GPJ4DAT);
//Apollo -

#if defined(DM9000_16BIT_DATA)
  SROM_BW_REG &= ~(0xf << 4);
  SROM_BW_REG |= (0<<7) | (0<<6) | (0<<5) | (1<<4);

#else  
  SROM_BW_REG &= ~(0xf << 4);
  SROM_BW_REG |= (0<<7) | (0<<6) | (0<<5) | (0<<4);
#endif
  SROM_BC1_REG = ((0<<28)|(1<<24)|(5<<16)|(1<<12)|(4<<8)|(6<<4)|(0<<0));

  tmp = MP01CON_REG;
  tmp &=~(0xf<<4);
  tmp |=(2<<4);
  MP01CON_REG = tmp;
}

int board_init(void)
{
  unsigned int tmp;
  DECLARE_GLOBAL_DATA_PTR;
#if defined(CONFIG_A8388_V1) || defined(CONFIG_S9) || defined(CONFIG_A8388_V2) || defined(CONFIG_A9)
  
	//内部上拉串口
  tmp = readl(GPA1PUD);
  tmp &= ~(0xf);
  tmp |= 0xa;
  writel(tmp,GPA1PUD);  
#ifndef CONFIG_AUTO_SDFUSE
  power_init_off();
#endif
#endif /* CONFIG_A8388_V1 CONFIG_S9 CONFIG_A8388_V2*/

  resume_system = check_resume_key();
  sdfuse_system = check_sdfuse_key();

#ifdef CONFIG_DRIVER_SMC911X
  smc9115_pre_init();
#endif

#ifdef CONFIG_DRIVER_DM9000
  dm9000_pre_init();
#endif

  gd->bd->bi_arch_number = MACH_TYPE;
  gd->bd->bi_boot_params = (PHYS_SDRAM_1+0x100);

  return 0;
}

int dram_init(void)
{
  DECLARE_GLOBAL_DATA_PTR;

  if(CONFIG_NR_DRAM_BANKS == 2)
  {
    gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
    gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
  
    gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
    gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
  }
  

  if(CONFIG_NR_DRAM_BANKS == 4)
  {
    gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
    gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
  
    gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
    gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
  
    gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
    gd->bd->bi_dram[2].size = PHYS_SDRAM_3_SIZE;
  
    gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
    gd->bd->bi_dram[3].size = PHYS_SDRAM_4_SIZE;
  }
  
  if(CONFIG_NR_DRAM_BANKS >= 6)
  {
    int i;
    for(i=0; i<CONFIG_NR_DRAM_BANKS; i++)
    {
      gd->bd->bi_dram[i].start = PHYS_SDRAM_1 + SDRAM_BANK_SIZE*i;
      gd->bd->bi_dram[i].size = SDRAM_BANK_SIZE;
    }
  }
  
  return 0;
}

#ifdef BOARD_LATE_INIT
#if defined(CONFIG_BOOT_NAND)
int board_late_init (void)
{
  uint *magic = (uint*)(PHYS_SDRAM_1);
  char boot_cmd[100];

  if ((0x24564236 == magic[0]) && (0x20764316 == magic[1])) {
    sprintf(boot_cmd, "nand erase 0 40000;nand write %08x 0 40000", PHYS_SDRAM_1 + 0x8000);
    magic[0] = 0;
    magic[1] = 0;
    printf("\nready for self-burning U-Boot image\n\n");
    setenv("bootdelay", "0");
    setenv("bootcmd", boot_cmd);
  }

  return 0;
}
#elif defined(CONFIG_BOOT_MOVINAND)
int board_late_init (void)
{
  uint *magic = (uint*)(PHYS_SDRAM_1);
  char boot_cmd[100];
  int hc;

  hc = (magic[2] & 0x1) ? 1 : 0;

  if ((0x24564236 == magic[0]) && (0x20764316 == magic[1])) {
    sprintf(boot_cmd, "movi init %d %d;movi write u-boot %08x", magic[3], hc, PHYS_SDRAM_1 + 0x8000);
    magic[0] = 0;
    magic[1] = 0;
    printf("\nready for self-burning U-Boot image\n\n");
    setenv("bootdelay", "0");
    setenv("bootcmd", boot_cmd);
  }

  return 0;
}
#else
int board_late_init (void)
{
  return 0;
}
#endif
#endif

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
#ifdef CONFIG_MCP_SINGLE
#if defined(CONFIG_VOGUES)
  printf("\nBoard:   VOGUESV210\n");
#else
  printf("\nBoard:   SKDV210\n");
#endif //CONFIG_VOGUES
#else
  printf("\nBoard:   SMDKC110\n");
#endif
  return (0);
}
#endif

#ifdef CONFIG_ENABLE_MMU

#ifdef CONFIG_MCP_SINGLE
ulong virt_to_phy_smdkc110(ulong addr)
{
  if ((0xc0000000 <= addr) && (addr < 0xd0000000))
    return (addr - 0xc0000000 + 0x20000000);
  else
    printf("The input address don't need "\
      "a virtual-to-physical translation : %08lx\n", addr);

  return addr;
}
#else
ulong virt_to_phy_smdkc110(ulong addr)
{
  if ((0xc0000000 <= addr) && (addr < 0xd0000000))
    return (addr - 0xc0000000 + 0x30000000);
  else if ((0x30000000 <= addr) && (addr < 0x50000000))
    return addr;
  else
    printf("The input address don't need "\
      "a virtual-to-physical translation : %08lx\n", addr);

  return addr;
}
#endif

#endif

#if defined(CONFIG_CMD_NAND) && defined(CFG_NAND_LEGACY)
#include <linux/mtd/nand.h>
extern struct nand_chip nand_dev_desc[CFG_MAX_NAND_DEVICE];
void nand_init(void)
{
  nand_probe(CFG_NAND_BASE);
        if (nand_dev_desc[0].ChipID != NAND_ChipID_UNKNOWN) {
                print_size(nand_dev_desc[0].totlen, "\n");
        }
}
#endif
