#include <common.h>
#include <regs.h>
#include <asm/io.h>
#include <asm/types.h>

#include "fimd.h"
#include "animation.h"
#include "adc.h"

#define LCD_BGCOLOR		0x1428A0

extern int resume_system;
extern int sdfuse_system;
extern int update_from_sd (char *part, char *file);
extern int find_file(char *path, char *file);
static unsigned int gFgColor = 0xFF;
static unsigned int gLeftColor = LCD_BGCOLOR;
static unsigned int recovery_flag = 111;
static unsigned int normal_flag = 0;
static unsigned int update_flag = 222;
#define Delay(_a)		udelay(_a*1000)

static S5PC11X_LCD *g_pDispConReg;
static tDevInfo g_DevInfoDisp;
static tDispWindow0Config g_Win0Config;
static tDispWindow12Config g_Win1Config;
static tDispWindow12Config g_Win2Config;
static tDispWindow34Config g_Win3Config;
static tDispWindow34Config g_Win4Config;
static int lcd_width;
static int lcd_height;
volatile int is_lcdon = 0;
void LDI_fill_output_device_information(tDevInfo *pDeviceInfo)
{
#ifdef CONFIG_LCD_PIXELQI
  pDeviceInfo->VideoOutMode = DISP_VIDOUT_RGBIF;
  pDeviceInfo->RGBINFO.RGBOutMode = DISP_24BIT_RGB888_P;
  pDeviceInfo->RGBINFO.MIPIDSIen = MIPI_DSI_DIS;
  pDeviceInfo->RGBINFO.RGBEvenOrder = RGB_ORDER_E_RGB;
  pDeviceInfo->RGBINFO.RGBOddOrder = RGB_ORDER_O_RGB;
  pDeviceInfo->RGBINFO.uiWidth = 1024;
  pDeviceInfo->RGBINFO.uiHeight = 600;
  pDeviceInfo->RGBINFO.VBPD_Value = 6;
  pDeviceInfo->RGBINFO.VFPD_Value = 3;
  pDeviceInfo->RGBINFO.VSPW_Value = 10;
  pDeviceInfo->RGBINFO.HBPD_Value = 80;
  pDeviceInfo->RGBINFO.HFPD_Value = 48;
  pDeviceInfo->RGBINFO.HSPW_Value = 32;
  pDeviceInfo->RGBINFO.VCLK_Polarity = IVCLK_FALL_EDGE;
  pDeviceInfo->RGBINFO.HSYNC_Polarity = IHSYNC_LOW_ACTIVE;
  pDeviceInfo->RGBINFO.VSYNC_Polarity = IVSYNC_LOW_ACTIVE;
  pDeviceInfo->RGBINFO.VDEN_Polarity = IVDEN_HIGH_ACTIVE;
  pDeviceInfo->RGBINFO.VCLK_Source = CLKSEL_F_LCDCLK;
  pDeviceInfo->RGBINFO.VCLK_Direction = CLKDIR_DIVIDED;
  pDeviceInfo->RGBINFO.Frame_Rate = 40;
  pDeviceInfo->RGBINFO.VCLK_Gate = VCLK_NORMAL;
  pDeviceInfo->RGBINFO.RGBSkipEn = RGB_SKIP_DISABLE;
  pDeviceInfo->RGBINFO.RGBDummy = RGB_DUMMY_DISABLE;
  pDeviceInfo->RGBINFO.PixelCompensation = PIXEL_COMP_DISABLE;
#endif

#ifdef CONFIG_LCD_LG
  pDeviceInfo->VideoOutMode = DISP_VIDOUT_RGBIF;
  pDeviceInfo->RGBINFO.RGBOutMode = DISP_24BIT_RGB888_P;
  pDeviceInfo->RGBINFO.MIPIDSIen = MIPI_DSI_DIS;
  pDeviceInfo->RGBINFO.RGBEvenOrder = RGB_ORDER_E_RGB;
  pDeviceInfo->RGBINFO.RGBOddOrder = RGB_ORDER_O_RGB;
  pDeviceInfo->RGBINFO.uiWidth = 1024;
  pDeviceInfo->RGBINFO.uiHeight = 768;

  pDeviceInfo->RGBINFO.VBPD_Value = 6;
  pDeviceInfo->RGBINFO.VFPD_Value = 16;
  pDeviceInfo->RGBINFO.VSPW_Value = 10;
  pDeviceInfo->RGBINFO.HBPD_Value = 256;
  pDeviceInfo->RGBINFO.HFPD_Value = 256;
  pDeviceInfo->RGBINFO.HSPW_Value = 256;

  pDeviceInfo->RGBINFO.VCLK_Polarity = IVCLK_RISE_EDGE;
  pDeviceInfo->RGBINFO.HSYNC_Polarity = IHSYNC_LOW_ACTIVE;
  pDeviceInfo->RGBINFO.VSYNC_Polarity = IVSYNC_LOW_ACTIVE;
  pDeviceInfo->RGBINFO.VDEN_Polarity = IVDEN_HIGH_ACTIVE;
  pDeviceInfo->RGBINFO.VCLK_Source = CLKSEL_F_LCDCLK;
  pDeviceInfo->RGBINFO.VCLK_Direction = CLKDIR_DIVIDED;
  pDeviceInfo->RGBINFO.Frame_Rate = 60;
  pDeviceInfo->RGBINFO.VCLK_Gate = VCLK_NORMAL;
  pDeviceInfo->RGBINFO.RGBSkipEn = RGB_SKIP_DISABLE;
  pDeviceInfo->RGBINFO.RGBDummy = RGB_DUMMY_DISABLE;
  pDeviceInfo->RGBINFO.PixelCompensation = PIXEL_COMP_DISABLE;
#endif

#ifdef CONFIG_LCD_LG_V2
  pDeviceInfo->VideoOutMode = DISP_VIDOUT_RGBIF;
  pDeviceInfo->RGBINFO.RGBOutMode = DISP_24BIT_RGB888_P;
  pDeviceInfo->RGBINFO.MIPIDSIen = MIPI_DSI_DIS;
  pDeviceInfo->RGBINFO.RGBEvenOrder = RGB_ORDER_E_RGB;
  pDeviceInfo->RGBINFO.RGBOddOrder = RGB_ORDER_O_RGB;
  pDeviceInfo->RGBINFO.uiWidth = 1024;
  pDeviceInfo->RGBINFO.uiHeight = 768;

  pDeviceInfo->RGBINFO.VBPD_Value = 15;
  pDeviceInfo->RGBINFO.VFPD_Value = 3;
  pDeviceInfo->RGBINFO.VSPW_Value = 4;
  pDeviceInfo->RGBINFO.HBPD_Value = 32;
  pDeviceInfo->RGBINFO.HFPD_Value = 48;
  pDeviceInfo->RGBINFO.HSPW_Value = 80;

  pDeviceInfo->RGBINFO.VCLK_Polarity = IVCLK_FALL_EDGE;
  pDeviceInfo->RGBINFO.HSYNC_Polarity = IHSYNC_LOW_ACTIVE;
  pDeviceInfo->RGBINFO.VSYNC_Polarity = IVSYNC_LOW_ACTIVE;
  pDeviceInfo->RGBINFO.VDEN_Polarity = IVDEN_HIGH_ACTIVE;
  pDeviceInfo->RGBINFO.VCLK_Source = CLKSEL_F_LCDCLK;
  pDeviceInfo->RGBINFO.VCLK_Direction = CLKDIR_DIVIDED;
  pDeviceInfo->RGBINFO.Frame_Rate = 60;
  pDeviceInfo->RGBINFO.VCLK_Gate = VCLK_NORMAL;
  pDeviceInfo->RGBINFO.RGBSkipEn = RGB_SKIP_DISABLE;
  pDeviceInfo->RGBINFO.RGBDummy = RGB_DUMMY_DISABLE;
  pDeviceInfo->RGBINFO.PixelCompensation = PIXEL_COMP_DISABLE;
#endif
}

void Disp_initialize_port_RGBIF(DISP_RGBIFOUT_MODE RGBOutMode)
{
	// GPF0_4~7->VD0~VD3
	// GPF1_0~7->VD4~VD11
	// GPF2_0~7->VD12~VD19
	// GPF3_0~3->VD20~VD23
	writel(0x22222222, GPF0CON);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	writel(0x0, GPF0PUD);			//set pull-up,down disable
	writel(0x22222222, GPF1CON);	//set GPF1 as VD[11:4]
	writel(0x0, GPF1PUD);			//set pull-up,down disable
	writel(0x22222222, GPF2CON);	//set GPF2 as VD[19:12]
	writel(0x0, GPF2PUD);			//set pull-up,down disable
	writel(0x00002222, GPF3CON);	//set GPF3 as VD[23:20]
	writel(0x0, GPF3PUD);			//set pull-up,down disable

	writel(0x2, DISPLAY_CONTROL);

	/* drive strength to max */
	writel(0xffffffff, GPF0DRV);
	writel(0xffffffff, GPF1DRV);
	writel(0xffffffff, GPF2DRV);
	writel(0x000000ff, GPF3DRV);
}

void Disp_set_syscon_clk_fimd(void)
{
  unsigned long dwTimeOut = 0;
  unsigned int val;

  val = readl(CLK_SRC_MASK0);
  writel((val & ~(0x1<<5)), CLK_SRC_MASK0);   //MUX OFF
  
  val = readl(CLK_SRC1);
  writel((val & ~(0xF<<20) |(0x6<<20)), CLK_SRC1);

  val = readl(CLK_DIV1);
  writel((val & ~(0xF<<20) |(0x4<<20)), CLK_DIV1);
  // Divider Status Check
  dwTimeOut = 10000;
  do
  {
    if(dwTimeOut-- == 0)
      break;
  }while(readl(CLK_DIV_STAT0)  & (0x1<<13));

  val = readl(CLK_SRC_MASK0);
  writel((val & ~(0x1<<5) | (0x1<<5)), CLK_SRC_MASK0);  //MUX ON
//  printf("[DISP:INF] CLK_SRC_MASK0 = 0x%08x\n", readl(CLK_SRC_MASK0));
//  printf("[DISP:INF] CLK_SRC1  = 0x%08x\n", readl(CLK_SRC1));
//  printf("[DISP:INF] CLK_DIV1  = 0x%08x\n", readl(CLK_DIV1));
}

int Disp_get_src_clock(unsigned int CLKSrc, unsigned int* pdwVCLKSrc)
{
  unsigned long dwVCLKSrc = 0;

  switch(CLKSrc)
  {
  case CLKSEL_F_HCLK:
    dwVCLKSrc = get_HCLK();
//    printf("[DISP:INF] VCLK Source = HCLK (%d Hz)\n", dwVCLKSrc);
    break;
  case CLKSEL_F_LCDCLK:
    Disp_set_syscon_clk_fimd();
    // if we use Dout_MPLL then we can calculate Clock from SysClk Register
    dwVCLKSrc = get_MPLL_CLK();
//    dwVCLKSrc = get_VPLL_CLK();
//    printf("[DISP:INF] MPLLout = (%d Hz)\n", dwVCLKSrc);
    dwVCLKSrc = dwVCLKSrc/(((readl(CLK_DIV1) & (0xF<<20))>>20) + 1);
//    printf("[DISP:INF] VCLK Source = LCDCLK (%d Hz)\n", dwVCLKSrc);
    break;
  case CLKSEL_F_EXT27M:
    dwVCLKSrc = 27000000;  // 27MHz
//    printf("[DISP:INF] VCLK Source = EXT27M (%d Hz)\n", dwVCLKSrc);
    break;
  default:
    printf("[DISP:ERR] --Disp_get_vclk_direction_divider() : Unknown CLKSrc = %d\n", CLKSrc);
    return 0;
  }
  *pdwVCLKSrc = dwVCLKSrc;
  return 1;
}

int Disp_get_vclk_direction_divider(unsigned int CLKSrc, unsigned int *ClkDir, unsigned int *ClkDiv)
{
  unsigned long dwVCLKSrc = 0;
  unsigned long dwHozTime = 0;
  unsigned long dwVerTime = 0;
  unsigned long dwVCLK = 0;
  unsigned long dwDiv = 0;
  unsigned long dTmpDiv;

  if(!Disp_get_src_clock(CLKSrc, &dwVCLKSrc))
  {
    return 0;
  }

  switch(g_DevInfoDisp.RGBINFO.RGBOutMode)
  {
    case DISP_16BIT_RGB565_P:
    case DISP_18BIT_RGB666_P:
    case DISP_24BIT_RGB888_P:
      dwHozTime = g_DevInfoDisp.RGBINFO.uiWidth+ g_DevInfoDisp.RGBINFO.HBPD_Value+ g_DevInfoDisp.RGBINFO.HFPD_Value+ g_DevInfoDisp.RGBINFO.HSPW_Value;
      break;
    case DISP_18BIT_RGB666_S:
    case DISP_24BIT_RGB888_S:
      dwHozTime = g_DevInfoDisp.RGBINFO.uiWidth*3+ g_DevInfoDisp.RGBINFO.HBPD_Value+ g_DevInfoDisp.RGBINFO.HFPD_Value+ g_DevInfoDisp.RGBINFO.HSPW_Value;
      break;
  }
  dwVerTime = g_DevInfoDisp.RGBINFO.uiHeight+ g_DevInfoDisp.RGBINFO.VBPD_Value+ g_DevInfoDisp.RGBINFO.VFPD_Value+ g_DevInfoDisp.RGBINFO.VSPW_Value;

  dTmpDiv = dwVCLKSrc*10/(dwHozTime*dwVerTime*g_DevInfoDisp.RGBINFO.Frame_Rate);
//  printf("[DISP:INF] dwHozTime = %d, dwVerTime = %d, Frame_Rate = %d\n", dwHozTime, dwVerTime, g_DevInfoDisp.RGBINFO.Frame_Rate);
  dTmpDiv = (dTmpDiv+5)*10;
  dwDiv = (unsigned int)(dTmpDiv/100);

//printf("[DISP:INF] VCLKSrc = %d, VCLK = %d, Div = %d\n", dwVCLKSrc, dwVCLK, dwDiv);
  if (dwDiv < 1)
  {
    printf("[DISP:ERR] --Disp_get_vclk_direction_divider() : VCLK Source is Too Slow\n");
    return 0;
  }
  else if(dwDiv == 1)
  {
    // No Divide, Direct Clock
    *ClkDir = CLKDIR_DIRECT;
    *ClkDiv = 1;
//    printf("[DISP:INF] VCLK Direction = Direct, VCLK = %d Hz\n", dwVCLKSrc);
  }
  else
  {
    // Divide by more than 1, Divided Clock
    *ClkDir = CLKDIR_DIVIDED;
    *ClkDiv = dwDiv;
//printf("[DISP:INF] VCLK Direction = Divided, Divider = %d, VCLK = %d Hz\n", *ClkDiv,  (unsigned int)(dwVCLKSrc/(*ClkDiv)));
  }
  return 1;
}

void Disp_initialize_RGBIF(void)
{
  unsigned int VCLKDivider;

  if (Disp_get_vclk_direction_divider( g_DevInfoDisp.RGBINFO.VCLK_Source, &g_DevInfoDisp.RGBINFO.VCLK_Direction, &VCLKDivider))
  {
    g_pDispConReg->VIDCON0 = \
            g_DevInfoDisp.RGBINFO.MIPIDSIen | \
            PROGRESSIVE | VIDOUT_RGBIF | \
            SET_RGBSPSEL(g_DevInfoDisp.RGBINFO.RGBOutMode) | \
            CLKVALUP_ALWAYS | CLKVAL_F(VCLKDivider) | \
            g_DevInfoDisp.RGBINFO.VCLK_Gate | \
            g_DevInfoDisp.RGBINFO.VCLK_Direction |
            g_DevInfoDisp.RGBINFO.VCLK_Source | \
            ENVID_DISABLE | ENVID_F_DISABLE;

    g_pDispConReg->VIDCON1 = g_DevInfoDisp.RGBINFO.VCLK_Polarity | \
                  g_DevInfoDisp.RGBINFO.HSYNC_Polarity | \
                  g_DevInfoDisp.RGBINFO.VSYNC_Polarity | \
                  g_DevInfoDisp.RGBINFO.VDEN_Polarity | \
                  VCLK_RUNNING_DISABLE_VDEN;

    g_pDispConReg->VIDCON2 = \
      (g_pDispConReg->VIDCON2 & ~(RGB_SKIP_MASK|RGB_DUMMY_MASK|\
                    RGB_ORDER_E_MASK|RGB_ORDER_O_MASK)) | \
      g_DevInfoDisp.RGBINFO.RGBSkipEn | \
      g_DevInfoDisp.RGBINFO.RGBDummy | \
      g_DevInfoDisp.RGBINFO.RGBEvenOrder | \
      g_DevInfoDisp.RGBINFO.RGBOddOrder;

    g_pDispConReg->VIDCON3 = g_DevInfoDisp.RGBINFO.PixelCompensation;

    g_pDispConReg->VIDTCON0 = VBPDE(1) | \
                VBPD(g_DevInfoDisp.RGBINFO.VBPD_Value) | \
                VFPD(g_DevInfoDisp.RGBINFO.VFPD_Value) | \
                VSPW(g_DevInfoDisp.RGBINFO.VSPW_Value);

    g_pDispConReg->VIDTCON1 = VFPDE(1) | \
                HBPD(g_DevInfoDisp.RGBINFO.HBPD_Value) | \
                HFPD(g_DevInfoDisp.RGBINFO.HFPD_Value) | \
                HSPW(g_DevInfoDisp.RGBINFO.HSPW_Value);

    g_pDispConReg->VIDTCON2 =
                LINEVAL(g_DevInfoDisp.RGBINFO.uiHeight) | \
                HOZVAL(g_DevInfoDisp.RGBINFO.uiWidth);
  }
  else
    printf("[DISP:ERR] Disp_initialize_RGBIF() : Clock Source Decision Failed\n");
}

void Disp_initialize_output_interface(void)
{
  if(g_DevInfoDisp.RGBINFO.MIPIDSIen == MIPI_DSI_DIS)
  {
    Disp_initialize_port_RGBIF(g_DevInfoDisp.RGBINFO.RGBOutMode);
  }
  Disp_initialize_RGBIF();
}

void Disp_window0_initialize(DISP_WINDOW_MODE Mode, DISP_BPP_MODE BPPMode, unsigned int uiWidth, unsigned int uiHeight, unsigned int uiOffsetX, unsigned int uiOffsetY)
{
    unsigned int uiTempReg = g_pDispConReg->WINCON0 & (BLEND_MULTIPLIED | ALPHASEL_ALPHA1);

//    printf("[DISP]++Disp_window0_initialize(%d, %d, %d, %d, %d, %d)\n", Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);

    switch(Mode)
    {
    case DISP_WIN0_DMA:
        g_Win0Config.LocalPathEnable = LOCAL_PATH_DISABLE;
        //g_Win0Config.LocaPathSourceFormat;    // Don't care when Local Path is disabled
        break;
    case DISP_WIN0_CAMIF0_RGB:
        g_Win0Config.LocalPathEnable = LOCAL_PATH_ENABLE;
        g_Win0Config.LocaPathSourceFormat = LOCAL_IN_RGB888;
        break;
    case DISP_WIN0_CAMIF0_YUV:
        g_Win0Config.LocalPathEnable = LOCAL_PATH_ENABLE;
        g_Win0Config.LocaPathSourceFormat = LOCAL_IN_YUV444;
        break;
    default:
        printf("[DISP:ERR] Disp_window0_initialize() : Unsupported Window Mode [%d]\n", Mode);
        return;
    }

    g_Win0Config.BitSwapping = BITSWP_DISABLE;
    g_Win0Config.ByteSwapping = BYTSWP_DISABLE;
    g_Win0Config.HalfWordSwapping = HAWSWP_DISABLE;
    g_Win0Config.WordSwapping = WSWP_DISABLE;

    switch(BPPMode)
    {
    //case DISP_1BPP:
    //case DISP_2BPP:
    //case DISP_4BPP:
    //case DISP_8BPP_PAL:
    //case DISP_8BPP_NOPAL:
    //case DISP_18BPP_A665:
    //case DISP_19BPP_A666:
    //case DISP_24BPP_A887:
    //case DISP_25BPP_A888:
    case DISP_16BPP_I555:
    case DISP_16BPP_A555:
    case DISP_16BPP_565:
        g_Win0Config.uiPageWidth = uiWidth*2;    // 2 byte per pixel
        g_Win0Config.HalfWordSwapping = HAWSWP_ENABLE;    // 16BPP need Halfword Swapping
        break;
    case DISP_18BPP_666:
    case DISP_24BPP_888:
    case DISP_32BPP_A888:
        g_Win0Config.uiPageWidth = uiWidth*4;    // 4 byte per pixel

        if(g_Win0Config.LocalPathEnable != LOCAL_PATH_ENABLE)
        {
            g_Win0Config.WordSwapping = WSWP_ENABLE;
        }
        break;
    default:
        printf("[DISP:ERR] Disp_window0_initialize() : Unsupported BPP Mode [%d]\n", BPPMode);
        return;
    }

    g_Win0Config.BPP_Mode = BPPMode;
    g_Win0Config.BufferSelect = BUFSEL_BUF0;
    g_Win0Config.BufferAutoControl = BUFAUTO_DISABLE;
    g_Win0Config.uiWidth = uiWidth;
    g_Win0Config.uiHeight = uiHeight;
    g_Win0Config.uiOffsetX = uiOffsetX;
    g_Win0Config.uiOffsetY = uiOffsetY;

    if (g_Win0Config.LocalPathEnable == LOCAL_PATH_ENABLE)
    {
        g_Win0Config.BurstLength = BURSTLEN_4WORD;    // 4 Words Burst
    }
    else if ((g_Win0Config.uiPageWidth%64) == 0)        // 16 words burst case
    {
        g_Win0Config.BurstLength = BURSTLEN_16WORD;
    }
    else if ((g_Win0Config.uiPageWidth%32) == 0)    // 8 words burst case
    {
        g_Win0Config.BurstLength = BURSTLEN_8WORD;
    }
    else if ((g_Win0Config.uiPageWidth%16) == 0)    // 4 words burst case
    {
        g_Win0Config.BurstLength = BURSTLEN_4WORD;
    }
    else
    {
        printf("[DISP:ERR] Disp_window0_initialize() : uiPageWidth[%d] is not Aligned with Minimum Burst Size (4 Words)\n", g_Win0Config.uiPageWidth);
        return;
    }

    g_pDispConReg->WINCON0 = CSC_WIDE_RANGE | //g_Win0Config.LocalPathEnable |
                            g_Win0Config.BufferSelect | g_Win0Config.BufferAutoControl | g_Win0Config.BitSwapping |
                            g_Win0Config.ByteSwapping | g_Win0Config.HalfWordSwapping |g_Win0Config.WordSwapping |
                            g_Win0Config.LocaPathSourceFormat |g_Win0Config.BurstLength |
                            BPPMODE_F(g_Win0Config.BPP_Mode) | uiTempReg;

    g_pDispConReg->VIDOSD0A = OSD_LEFTTOPX_F(g_Win0Config.uiOffsetX) | OSD_LEFTTOPY_F(g_Win0Config.uiOffsetY);

    g_pDispConReg->VIDOSD0B = OSD_RIGHTBOTX_F(g_Win0Config.uiWidth+g_Win0Config.uiOffsetX-1) |
                                OSD_RIGHTBOTY_F(g_Win0Config.uiHeight+g_Win0Config.uiOffsetY-1);

    g_pDispConReg->VIDOSD0C = OSD_SIZE(g_Win0Config.uiWidth*g_Win0Config.uiHeight);
}

void Disp_window1_initialize(DISP_WINDOW_MODE Mode, DISP_BPP_MODE BPPMode, unsigned int uiWidth, unsigned int uiHeight, unsigned int uiOffsetX, unsigned int uiOffsetY)
{
    unsigned int uiTempReg = g_pDispConReg->WINCON1 & (BLEND_MULTIPLIED | ALPHASEL_ALPHA1);

//    printf("[DISP]++Disp_window1_initialize(%d, %d, %d, %d, %d, %d)\n", Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);

    switch(Mode)
    {
    case DISP_WIN1_DMA:
        g_Win1Config.LocalPathEnable = LOCAL_PATH_DISABLE;
        //g_Win1Config.LocalPathSelect;            // Don't care when Local Path is disabled
        //g_Win1Config.LocaPathSourceFormat;    // Don't care when Local Path is disabled
        break;
    case DISP_WIN1_CAMIF1_RGB:
        g_Win1Config.LocalPathEnable = LOCAL_PATH_ENABLE;
        g_Win1Config.LocalPathSelect = LOCALSEL_CAMIF1;
        g_Win1Config.LocaPathSourceFormat = LOCAL_IN_RGB888;
        break;
    case DISP_WIN1_CAMIF1_YUV:
        g_Win1Config.LocalPathEnable = LOCAL_PATH_ENABLE;
        g_Win1Config.LocalPathSelect = LOCALSEL_CAMIF1;
        g_Win1Config.LocaPathSourceFormat = LOCAL_IN_YUV444;
        break;
    case DISP_WIN1_VP_YUV:
        g_Win1Config.LocalPathEnable = LOCAL_PATH_ENABLE;
        g_Win1Config.LocalPathSelect = LOCALSEL_VP;
        g_Win1Config.LocaPathSourceFormat = LOCAL_IN_YUV444;
        break;
    default:
        printf("[DISP:ERR] Disp_window1_initialize() : Unsupported Window Mode [%d]\n", Mode);
        return;
        break;
    }

    g_Win1Config.BitSwapping = BITSWP_DISABLE;
    g_Win1Config.ByteSwapping = BYTSWP_DISABLE;
    g_Win1Config.HalfWordSwapping = HAWSWP_DISABLE;
    g_Win1Config.WordSwapping = WSWP_DISABLE;

    switch(BPPMode)
    {
    //case DISP_1BPP:
    //case DISP_2BPP:
    //case DISP_4BPP:
    //case DISP_8BPP_PAL:
    //case DISP_8BPP_NOPAL:
    //case DISP_18BPP_A665:
    //case DISP_19BPP_A666:
    //case DISP_24BPP_A887:
    //case DISP_25BPP_A888:
    case DISP_16BPP_I555:
    case DISP_16BPP_A555:
    case DISP_16BPP_565:
        g_Win1Config.uiPageWidth = uiWidth*2;    // 2 byte per pixel
        g_Win1Config.HalfWordSwapping = HAWSWP_ENABLE;    // 16BPP need Halfword Swapping
        break;
    case DISP_18BPP_666:
    case DISP_24BPP_888:
    case DISP_32BPP_A888:
        g_Win1Config.uiPageWidth = uiWidth*4;    // 4 byte per pixel
        if(g_Win1Config.LocalPathEnable != LOCAL_PATH_ENABLE)
        {
            g_Win1Config.WordSwapping = WSWP_ENABLE;
        }
        break;
    default:
        printf("[DISP:ERR] Disp_window1_initialize() : Unsupported BPP Mode [%d]\n", BPPMode);
        return;
        break;
    }

    g_Win1Config.BPP_Mode = BPPMode;
    g_Win1Config.BufferSelect = BUFSEL_BUF0;
    g_Win1Config.BufferAutoControl = BUFAUTO_DISABLE;
    g_Win1Config.uiWidth = uiWidth;
    g_Win1Config.uiHeight = uiHeight;
    g_Win1Config.uiOffsetX = uiOffsetX;
    g_Win1Config.uiOffsetY = uiOffsetY;

    if (g_Win1Config.LocalPathEnable == LOCAL_PATH_ENABLE)
    {
        g_Win1Config.BurstLength = BURSTLEN_4WORD;    // 4 Words Burst
    }
    else if ((g_Win1Config.uiPageWidth%64) == 0)        // 16 words burst case
    {
        g_Win1Config.BurstLength = BURSTLEN_16WORD;
    }
    else if ((g_Win1Config.uiPageWidth%32) == 0)    // 8 words burst case
    {
        g_Win1Config.BurstLength = BURSTLEN_8WORD;
    }
    else if ((g_Win1Config.uiPageWidth%16) == 0)    // 4 words burst case
    {
        g_Win1Config.BurstLength = BURSTLEN_4WORD;
    }
    else
    {
        printf("[DISP:ERR] Disp_window1_initialize() : uiPageWidth is not Word Aligned [%d]\n", g_Win0Config.uiPageWidth);
        return;
    }

    g_pDispConReg->WINCON1 = CSC_WIDE_RANGE | g_Win1Config.LocalPathSelect | //g_Win1Config.LocalPathEnable |
                            g_Win1Config.BufferSelect | g_Win1Config.BufferAutoControl | g_Win1Config.BitSwapping |
                            g_Win1Config.ByteSwapping | g_Win1Config.HalfWordSwapping |g_Win1Config.WordSwapping |
                            g_Win1Config.LocaPathSourceFormat |g_Win1Config.BurstLength | BLEND_PER_PLANE | BPPMODE_F(g_Win1Config.BPP_Mode) |
                            ALPHASEL_ALPHA0 | uiTempReg;

    g_pDispConReg->VIDOSD1A = OSD_LEFTTOPX_F(g_Win1Config.uiOffsetX) | OSD_LEFTTOPY_F(g_Win1Config.uiOffsetY);

    g_pDispConReg->VIDOSD1B = OSD_RIGHTBOTX_F(g_Win1Config.uiWidth+g_Win1Config.uiOffsetX-1) |
                                OSD_RIGHTBOTY_F(g_Win1Config.uiHeight+g_Win1Config.uiOffsetY-1);

    g_pDispConReg->VIDOSD1C = 0x0;

    g_pDispConReg->VIDOSD1D = OSD_SIZE(g_Win1Config.uiWidth*g_Win1Config.uiHeight);
}

void Disp_window2_initialize(DISP_WINDOW_MODE Mode, DISP_BPP_MODE BPPMode, unsigned int uiWidth, unsigned int uiHeight, unsigned int uiOffsetX, unsigned int uiOffsetY)
{
    unsigned int uiTempReg = g_pDispConReg->WINCON2 & (BLEND_MULTIPLIED | ALPHASEL_ALPHA1);

//    printf("[DISP]++Disp_window2_initialize(%d, %d, %d, %d, %d, %d)\n", Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);

    switch(Mode)
    {
    case DISP_WIN2_DMA:
        g_Win2Config.LocalPathEnable = LOCAL_PATH_DISABLE;
        //g_Win2Config.LocalPathSelect;            // Don't care when Local Path is disabled
        //g_Win2Config.LocaPathSourceFormat;    // Don't care when Local Path is disabled
        break;
    case DISP_WIN2_CAMIF2_RGB:
        g_Win2Config.LocalPathEnable = LOCAL_PATH_ENABLE;
        g_Win2Config.LocaPathSourceFormat = LOCAL_IN_RGB888;
        break;
    case DISP_WIN2_CAMIF2_YUV:
        g_Win2Config.LocalPathEnable = LOCAL_PATH_ENABLE;
        g_Win2Config.LocaPathSourceFormat = LOCAL_IN_YUV444;
        break;
    default:
        printf("[DISP:ERR] Disp_window2_initialize() : Unsupported Window Mode [%d]\n", Mode);
        return;
    }

    g_Win2Config.BitSwapping = BITSWP_DISABLE;
    g_Win2Config.ByteSwapping = BYTSWP_DISABLE;
    g_Win2Config.HalfWordSwapping = HAWSWP_DISABLE;
    g_Win2Config.WordSwapping = WSWP_DISABLE;

    switch(BPPMode)
    {
    //case DISP_1BPP:
    //case DISP_2BPP:
    //case DISP_4BPP:
    //case DISP_8BPP_PAL:
    //case DISP_8BPP_NOPAL:
    //case DISP_18BPP_A665:
    //case DISP_19BPP_A666:
    //case DISP_24BPP_A887:
    //case DISP_25BPP_A888:
    case DISP_16BPP_I555:
    case DISP_16BPP_A555:
    case DISP_16BPP_565:
        g_Win2Config.uiPageWidth = uiWidth*2;    // 2 byte per pixel
        g_Win2Config.HalfWordSwapping = HAWSWP_ENABLE;    // 16BPP need Halfword Swapping
        break;
    case DISP_18BPP_666:
    case DISP_24BPP_888:
    case DISP_32BPP_A888:
        g_Win2Config.uiPageWidth = uiWidth*4;    // 4 byte per pixel

        if(g_Win2Config.LocalPathEnable != LOCAL_PATH_ENABLE)
        {
            g_Win2Config.WordSwapping = WSWP_ENABLE;
        }
        break;
    default:
        printf("[DISP:ERR] Disp_window2_initialize() : Unsupported BPP Mode [%d]\n", BPPMode);
        return;
    }

    g_Win2Config.BPP_Mode = BPPMode;
    g_Win2Config.BufferSelect = BUFSEL_BUF0;
    g_Win2Config.BufferAutoControl = BUFAUTO_DISABLE;
    g_Win2Config.uiWidth = uiWidth;
    g_Win2Config.uiHeight = uiHeight;
    g_Win2Config.uiOffsetX = uiOffsetX;
    g_Win2Config.uiOffsetY = uiOffsetY;

    if (g_Win2Config.LocalPathEnable == LOCAL_PATH_ENABLE)
    {
        g_Win2Config.BurstLength = BURSTLEN_4WORD;    // 4 Words Burst
    }
    else if ((g_Win2Config.uiPageWidth%64) == 0)        // 16 words burst case
    {
        g_Win2Config.BurstLength = BURSTLEN_16WORD;
    }
    else if ((g_Win2Config.uiPageWidth%32) == 0)    // 8 words burst case
    {
        g_Win2Config.BurstLength = BURSTLEN_8WORD;
    }
    else if ((g_Win2Config.uiPageWidth%16) == 0)    // 4 words burst case
    {
        g_Win2Config.BurstLength = BURSTLEN_4WORD;
    }
    else
    {
        printf("[DISP:ERR] Disp_window2_initialize() : uiPageWidth is not Word Aligned [%d]\n", g_Win0Config.uiPageWidth);
        return;
    }

    g_pDispConReg->WINCON2 = CSC_WIDE_RANGE | g_Win2Config.LocalPathSelect | //g_Win2Config.LocalPathEnable |
                            g_Win2Config.BufferSelect | g_Win2Config.BufferAutoControl | g_Win2Config.BitSwapping |
                            g_Win2Config.ByteSwapping | g_Win2Config.HalfWordSwapping |g_Win2Config.WordSwapping | g_Win2Config.LocaPathSourceFormat |
                            g_Win2Config.BurstLength | BLEND_PER_PLANE | BPPMODE_F(g_Win2Config.BPP_Mode) |
                            ALPHASEL_ALPHA0 | uiTempReg;

    g_pDispConReg->VIDOSD2A = OSD_LEFTTOPX_F(g_Win2Config.uiOffsetX) | OSD_LEFTTOPY_F(g_Win2Config.uiOffsetY);

    g_pDispConReg->VIDOSD2B = OSD_RIGHTBOTX_F(g_Win2Config.uiWidth+g_Win2Config.uiOffsetX-1) |
                                OSD_RIGHTBOTY_F(g_Win2Config.uiHeight+g_Win2Config.uiOffsetY-1);

    g_pDispConReg->VIDOSD2C = 0x0;

    g_pDispConReg->VIDOSD2D = OSD_SIZE(g_Win2Config.uiWidth*g_Win2Config.uiHeight);
}

void Disp_window3_initialize(DISP_WINDOW_MODE Mode, DISP_BPP_MODE BPPMode, unsigned int uiWidth, unsigned int uiHeight, unsigned int uiOffsetX, unsigned int uiOffsetY)
{
    unsigned int uiTempReg = g_pDispConReg->WINCON3 & (BLEND_MULTIPLIED | ALPHASEL_ALPHA1);

//    printf("[DISP]++Disp_window3_initialize(%d, %d, %d, %d, %d, %d)\n", Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);

    g_Win3Config.BitSwapping = BITSWP_DISABLE;
    g_Win3Config.ByteSwapping = BYTSWP_DISABLE;
    g_Win3Config.HalfWordSwapping = HAWSWP_DISABLE;
    g_Win3Config.WordSwapping = WSWP_DISABLE;

    switch(BPPMode)
    {
    //case DISP_1BPP:
    //case DISP_2BPP:
    //case DISP_4BPP:
    //case DISP_8BPP_PAL:
    //case DISP_8BPP_NOPAL:
    //case DISP_18BPP_A665:
    //case DISP_19BPP_A666:
    //case DISP_24BPP_A887:
    //case DISP_25BPP_A888:
    case DISP_16BPP_I555:
    case DISP_16BPP_A555:
    case DISP_16BPP_565:
        g_Win3Config.uiPageWidth = uiWidth*2;    // 2 byte per pixel
        g_Win3Config.HalfWordSwapping = HAWSWP_ENABLE;    // 16BPP need Halfword Swapping
        break;
    case DISP_18BPP_666:
    case DISP_24BPP_888:
    case DISP_32BPP_A888:
        g_Win3Config.uiPageWidth = uiWidth*4;    // 4 byte per pixel
        g_Win3Config.WordSwapping = WSWP_ENABLE;
        break;
    default:
        printf("[DISP:ERR] Disp_window3_initialize() : Unsupported BPP Mode [%d]\n", BPPMode);
        return;
    }

    g_Win3Config.BPP_Mode = BPPMode;
    g_Win3Config.BufferSelect = BUFSEL_BUF0;
    g_Win3Config.BufferAutoControl = BUFAUTO_DISABLE;
    g_Win3Config.uiWidth = uiWidth;
    g_Win3Config.uiHeight = uiHeight;
    g_Win3Config.uiOffsetX = uiOffsetX;
    g_Win3Config.uiOffsetY = uiOffsetY;

    if (g_Win3Config.uiPageWidth%4)
    {
        printf("[DISP:ERR] Disp_window3_initialize() : uiPageWidth is not Word Aligned [%d]\n", g_Win0Config.uiPageWidth);
        return;
    }
    else if (g_Win3Config.uiPageWidth >= 16*4)
    {
        g_Win3Config.BurstLength = BURSTLEN_16WORD;
    }
    else if (g_Win3Config.uiPageWidth >= 8*4)
    {
        g_Win3Config.BurstLength = BURSTLEN_8WORD;
    }
    else
    {
        g_Win3Config.BurstLength = BURSTLEN_4WORD;
    }
    g_pDispConReg->WINCON3 = g_Win3Config.BufferSelect | g_Win3Config.BufferAutoControl | g_Win3Config.BitSwapping | g_Win3Config.ByteSwapping | g_Win3Config.HalfWordSwapping |
                            g_Win3Config.WordSwapping |g_Win3Config.BurstLength | BLEND_PER_PLANE | BPPMODE_F(g_Win3Config.BPP_Mode) |
                            ALPHASEL_ALPHA0 | uiTempReg;
    g_pDispConReg->VIDOSD3A = OSD_LEFTTOPX_F(g_Win3Config.uiOffsetX) | OSD_LEFTTOPY_F(g_Win3Config.uiOffsetY);
    g_pDispConReg->VIDOSD3B = OSD_RIGHTBOTX_F(g_Win3Config.uiWidth+g_Win3Config.uiOffsetX-1) |
                                OSD_RIGHTBOTY_F(g_Win3Config.uiHeight+g_Win3Config.uiOffsetY-1);
    g_pDispConReg->VIDOSD3C = 0x0;
}

void Disp_window4_initialize(DISP_WINDOW_MODE Mode, DISP_BPP_MODE BPPMode, unsigned int uiWidth, unsigned int uiHeight, unsigned int uiOffsetX, unsigned int uiOffsetY)
{
    unsigned int uiTempReg = g_pDispConReg->WINCON4 & (BLEND_MULTIPLIED | ALPHASEL_ALPHA1);

//    printf("[DISP]++Disp_window4_initialize(%d, %d, %d, %d, %d, %d)\n", Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);

    g_Win4Config.BitSwapping = BITSWP_DISABLE;
    g_Win4Config.ByteSwapping = BYTSWP_DISABLE;
    g_Win4Config.HalfWordSwapping = HAWSWP_DISABLE;
    g_Win4Config.WordSwapping = WSWP_DISABLE;

    switch(BPPMode)
    {
    //case DISP_1BPP:
    //case DISP_2BPP:
    //case DISP_4BPP:
    //case DISP_8BPP_PAL:
    //case DISP_8BPP_NOPAL:
    //case DISP_18BPP_A665:
    //case DISP_19BPP_A666:
    //case DISP_24BPP_A887:
    //case DISP_25BPP_A888:
    case DISP_16BPP_I555:
    case DISP_16BPP_A555:
    case DISP_16BPP_565:
        g_Win4Config.uiPageWidth = uiWidth*2;    // 2 byte per pixel
        g_Win4Config.HalfWordSwapping = HAWSWP_ENABLE;    // 16BPP need Halfword Swapping
        break;
    case DISP_18BPP_666:
    case DISP_24BPP_888:
    case DISP_32BPP_A888:
        g_Win4Config.uiPageWidth = uiWidth*4;    // 4 byte per pixel
        g_Win4Config.WordSwapping = WSWP_ENABLE;
        break;
    default:
        printf("[DISP:ERR] Disp_window4_initialize() : Unsupported BPP Mode [%d]\n", BPPMode);
        return;
    }

    g_Win4Config.BPP_Mode = BPPMode;
    g_Win4Config.BufferSelect = BUFSEL_BUF0;
    g_Win4Config.BufferAutoControl = BUFAUTO_DISABLE;
    g_Win4Config.uiWidth = uiWidth;
    g_Win4Config.uiHeight = uiHeight;
    g_Win4Config.uiOffsetX = uiOffsetX;
    g_Win4Config.uiOffsetY = uiOffsetY;

    if (g_Win4Config.uiPageWidth%4)
    {
        printf("[DISP:ERR] Disp_window4_initialize() : uiPageWidth is not Word Aligned [%d]\n", g_Win0Config.uiPageWidth);
        return;
    }
    else if (g_Win4Config.uiPageWidth >= 16*4)
    {
        g_Win4Config.BurstLength = BURSTLEN_16WORD;
    }
    else if (g_Win4Config.uiPageWidth >= 8*4)
    {
        g_Win4Config.BurstLength = BURSTLEN_8WORD;
    }
    else
    {
        g_Win4Config.BurstLength = BURSTLEN_4WORD;
    }
    g_pDispConReg->WINCON4 = g_Win4Config.BufferSelect | g_Win4Config.BufferAutoControl | g_Win4Config.BitSwapping | g_Win4Config.ByteSwapping | g_Win4Config.HalfWordSwapping |
                            g_Win4Config.WordSwapping |g_Win4Config.BurstLength | BLEND_PER_PLANE | BPPMODE_F(g_Win4Config.BPP_Mode) |
                            ALPHASEL_ALPHA0 | uiTempReg;
    g_pDispConReg->VIDOSD4A = OSD_LEFTTOPX_F(g_Win4Config.uiOffsetX) | OSD_LEFTTOPY_F(g_Win4Config.uiOffsetY);
    g_pDispConReg->VIDOSD4B = OSD_RIGHTBOTX_F(g_Win4Config.uiWidth+g_Win4Config.uiOffsetX-1) |
                                OSD_RIGHTBOTY_F(g_Win4Config.uiHeight+g_Win4Config.uiOffsetY-1);
    g_pDispConReg->VIDOSD4C = 0x0;
}

void Disp_set_window_mode(DISP_WINDOW_MODE Mode, DISP_BPP_MODE BPPMode, unsigned int uiWidth, unsigned int uiHeight, unsigned int uiOffsetX, unsigned int uiOffsetY)
{
//    printf("[DISP]++Disp_set_window_mode(%d)\n", Mode);

    switch(Mode)
    {
    case DISP_WIN0_DMA:
    case DISP_WIN0_CAMIF0_RGB:
    case DISP_WIN0_CAMIF0_YUV:
        Disp_window0_initialize(Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);
        break;
    case DISP_WIN1_DMA:
    case DISP_WIN1_CAMIF1_RGB:
    case DISP_WIN1_CAMIF1_YUV:
    case DISP_WIN1_VP_YUV:
        Disp_window1_initialize(Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);
        break;
    case DISP_WIN2_DMA:
    case DISP_WIN2_CAMIF2_RGB:
    case DISP_WIN2_CAMIF2_YUV:
        Disp_window2_initialize(Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);
        break;
    case DISP_WIN3_DMA:
        Disp_window3_initialize(Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);
        break;
    case DISP_WIN4_DMA:
        Disp_window4_initialize(Mode, BPPMode, uiWidth, uiHeight, uiOffsetX, uiOffsetY);
        break;
    default:
        printf("[DISP:ERR] Disp_set_window_mode() : Unsupported Window Mode [%d]\n", Mode);
        break;
    }
}

void Disp_window_registers_lock(DISP_WINDOW Win)
{
//    printf("[DISP]++Disp_window_registers_lock(%d)\n",Win);

    switch(Win)
    {
    case DISP_WIN0:
        g_pDispConReg->SHADOWCON |= W0_SHADOW_PROTECT;
        break;
    case DISP_WIN1:
        g_pDispConReg->SHADOWCON |= W1_SHADOW_PROTECT;
        break;
    case DISP_WIN2:
        g_pDispConReg->SHADOWCON |= W2_SHADOW_PROTECT;
        break;
    case DISP_WIN3:
        g_pDispConReg->SHADOWCON |= W3_SHADOW_PROTECT;
        break;
    case DISP_WIN4:
        g_pDispConReg->SHADOWCON |= W4_SHADOW_PROTECT;
        break;
    default:
        printf("[DISP:ERR] Disp_window_registers_lock() : Unknown Window Number [%d]\n", Win);
        break;
    }
}

void Disp_window_registers_unlock(DISP_WINDOW Win)
{
//    printf("[DISP]++Disp_window_registers_unlock(%d)\n",Win);
    switch(Win)
    {
    case DISP_WIN0:
        g_pDispConReg->SHADOWCON &= ~W0_SHADOW_PROTECT;
        break;
    case DISP_WIN1:
        g_pDispConReg->SHADOWCON &= ~W1_SHADOW_PROTECT;
        break;
    case DISP_WIN2:
        g_pDispConReg->SHADOWCON &= ~W2_SHADOW_PROTECT;
        break;
    case DISP_WIN3:
        g_pDispConReg->SHADOWCON &= ~W3_SHADOW_PROTECT;
        break;
    case DISP_WIN4:
        g_pDispConReg->SHADOWCON &= ~W4_SHADOW_PROTECT;
        break;
    default:
        printf("[DISP:ERR] Disp_window_registers_unlock() : Unknown Window Number [%d]\n", Win);
        break;
    }
}

void Disp_set_framebuffer(DISP_WINDOW Win, unsigned int uiFrameBufferAddress)
{
//    printf("[DISP]++Disp_set_framebuffer(%d, 0x%08x)\n", Win, uiFrameBufferAddress);
    Disp_window_registers_lock(Win);
    switch(Win)
    {
    case DISP_WIN0:
        if (g_Win0Config.LocalPathEnable == LOCAL_PATH_ENABLE)
        {
            // Safe Frame buffer Address for Local Path
            g_pDispConReg->VIDW00ADD0B0 = VBANK_F(uiFrameBufferAddress>>24) | VBASEU_F(uiFrameBufferAddress);
            g_pDispConReg->VIDW00ADD1B0 = VBASEL_F(VBASEU_F(uiFrameBufferAddress)+0x10);    // 4 Words
            g_pDispConReg->VIDW00ADD2 = PAGEWIDTH_F(0x10);                                    // 4 Words
            g_pDispConReg->WINCON0 &= ~(BUFSEL_BUF1|BUFSEL_BUF2);    // Buffer set to Buf0
        }
        else
        {
            g_pDispConReg->VIDW00ADD0B0 = VBANK_F(uiFrameBufferAddress>>24) | VBASEU_F(uiFrameBufferAddress);
            g_pDispConReg->VIDW00ADD1B0 = VBASEL_F(VBASEU_F(uiFrameBufferAddress)+g_Win0Config.uiPageWidth*g_Win0Config.uiHeight);
            g_pDispConReg->VIDW00ADD2 = PAGEWIDTH_F(g_Win0Config.uiPageWidth);
            g_pDispConReg->WINCON0 &= ~(BUFSEL_BUF1|BUFSEL_BUF2);    // Buffer set to Buf0
        }
        break;
    case DISP_WIN1:
        if (g_Win1Config.LocalPathEnable == LOCAL_PATH_ENABLE)
        {
            // Safe Frame buffer Address for Local Path
            g_pDispConReg->VIDW01ADD0B0 = VBANK_F(uiFrameBufferAddress>>24) | VBASEU_F(uiFrameBufferAddress);
            g_pDispConReg->VIDW01ADD1B0 = VBASEL_F(VBASEU_F(uiFrameBufferAddress)+0x10);    // 4 Words
            g_pDispConReg->VIDW01ADD2 = PAGEWIDTH_F(0x10);                                    // 4 Words
            g_pDispConReg->WINCON1 &= ~(BUFSEL_BUF1|BUFSEL_BUF2);    // Buffer set to Buf0
        }
        else
        {
            g_pDispConReg->VIDW01ADD0B0 = VBANK_F(uiFrameBufferAddress>>24) | VBASEU_F(uiFrameBufferAddress);
            g_pDispConReg->VIDW01ADD1B0 = VBASEL_F(VBASEU_F(uiFrameBufferAddress)+g_Win1Config.uiPageWidth*g_Win1Config.uiHeight);
            g_pDispConReg->VIDW01ADD2 = PAGEWIDTH_F(g_Win1Config.uiPageWidth);
            g_pDispConReg->WINCON1 &= ~(BUFSEL_BUF1|BUFSEL_BUF2);    // Buffer set to Buf0
        }
        break;
    case DISP_WIN2:
        if (g_Win2Config.LocalPathEnable == LOCAL_PATH_ENABLE)
        {
            // Safe Frame buffer Address for Local Path
            g_pDispConReg->VIDW02ADD0B0 = VBANK_F(uiFrameBufferAddress>>24) | VBASEU_F(uiFrameBufferAddress);
            g_pDispConReg->VIDW02ADD1B0 = VBASEL_F(VBASEU_F(uiFrameBufferAddress)+0x10);        // 4 Words
            g_pDispConReg->VIDW02ADD2 = PAGEWIDTH_F(0x10);
            g_pDispConReg->WINCON2 &= ~(BUFSEL_BUF1|BUFSEL_BUF2);    // Buffer set to Buf0// 4 Words
        }
        else
        {
            g_pDispConReg->VIDW02ADD0B0 = VBANK_F(uiFrameBufferAddress>>24) | VBASEU_F(uiFrameBufferAddress);
            g_pDispConReg->VIDW02ADD1B0 = VBASEL_F(VBASEU_F(uiFrameBufferAddress)+g_Win2Config.uiPageWidth*g_Win2Config.uiHeight);
            g_pDispConReg->VIDW02ADD2 = PAGEWIDTH_F(g_Win2Config.uiPageWidth);
            g_pDispConReg->WINCON2 &= ~(BUFSEL_BUF1|BUFSEL_BUF2);    // Buffer set to Buf0
        }
        break;
    case DISP_WIN3:
        g_pDispConReg->VIDW03ADD0B0 = VBANK_F(uiFrameBufferAddress>>24) | VBASEU_F(uiFrameBufferAddress);
        g_pDispConReg->VIDW03ADD1B0 = VBASEL_F(VBASEU_F(uiFrameBufferAddress)+g_Win3Config.uiPageWidth*g_Win3Config.uiHeight);
        g_pDispConReg->VIDW03ADD2 = PAGEWIDTH_F(g_Win3Config.uiPageWidth);
        g_pDispConReg->WINCON3 &= ~(BUFSEL_BUF1|BUFSEL_BUF2);    // Buffer set to Buf0
        break;
    case DISP_WIN4:
        g_pDispConReg->VIDW04ADD0B0 = VBANK_F(uiFrameBufferAddress>>24) | VBASEU_F(uiFrameBufferAddress);
        g_pDispConReg->VIDW04ADD1B0 = VBASEL_F(VBASEU_F(uiFrameBufferAddress)+g_Win4Config.uiPageWidth*g_Win4Config.uiHeight);
        g_pDispConReg->VIDW04ADD2 = PAGEWIDTH_F(g_Win4Config.uiPageWidth);
        g_pDispConReg->WINCON4 &= ~(BUFSEL_BUF1|BUFSEL_BUF2);    // Buffer set to Buf0
        break;
    default:
        printf("[DISP:ERR] Disp_set_framebuffer() : Unknown Window Number [%d]\n", Win);
        break;
    }
    Disp_window_registers_unlock(Win);
}

void Disp_window_onfoff(DISP_WINDOW Win, DISP_WINDOW_ONOFF WinOnOff)
{
//    printf("[DISP]++Disp_window_onfoff(%d, %d)\n", Win, WinOnOff);
    if (WinOnOff == DISP_WINDOW_ON || WinOnOff == DISP_WINDOW_OFF)
    {
        switch(Win)
        {
        case DISP_WIN0:
            if (WinOnOff == DISP_WINDOW_ON)
            {
                g_pDispConReg->SHADOWCON |= (1<<0) | ((g_Win0Config.LocalPathEnable>>22)<<5);
                g_pDispConReg->WINCON0 |= (ENWIN_F_ENABLE | g_Win0Config.LocalPathEnable);
            }
            else
            {
                // ENLOCAL bit must be disabled after ENWIN_F has been disabled
                g_pDispConReg->WINCON0 &= ~(ENWIN_F_ENABLE | g_Win0Config.LocalPathEnable);
                //g_pDispConReg->WINCON0 &= ~ENWIN_F_ENABLE;
                g_pDispConReg->SHADOWCON &= ~((1<<0) | (1<<5));
            }
            break;
        case DISP_WIN1:
            if (WinOnOff == DISP_WINDOW_ON)
            {
                g_pDispConReg->SHADOWCON |= (1<<1) | ((g_Win1Config.LocalPathEnable>>22)<<6);
                g_pDispConReg->WINCON1 |= (ENWIN_F_ENABLE | g_Win1Config.LocalPathEnable);
            }
            else
            {
                // ENLOCAL bit must be disabled after ENWIN_F has been disabled
                g_pDispConReg->WINCON1 &= ~(ENWIN_F_ENABLE | g_Win1Config.LocalPathEnable);
                //g_pDispConReg->WINCON1 &= ~ENWIN_F_ENABLE;
                g_pDispConReg->SHADOWCON &= ~((1<<1) | (1<<6));
            }
            break;
        case DISP_WIN2:
            if (WinOnOff == DISP_WINDOW_ON)
            {
                g_pDispConReg->SHADOWCON |= (1<<2) | ((g_Win2Config.LocalPathEnable>>22)<<7);
                g_pDispConReg->WINCON2 |= (ENWIN_F_ENABLE | g_Win2Config.LocalPathEnable);
            }
            else
            {
                // ENLOCAL bit must be disabled after ENWIN_F has been disabled
                g_pDispConReg->WINCON2 &= ~(ENWIN_F_ENABLE | g_Win2Config.LocalPathEnable);
                //g_pDispConReg->WINCON2 &= ~ENWIN_F_ENABLE;
                g_pDispConReg->SHADOWCON &= ~((1<<2) | (1<<7));
            }
            break;
        case DISP_WIN3:
            if (WinOnOff == DISP_WINDOW_ON)
            {
                g_pDispConReg->SHADOWCON |= (1<<3);
                g_pDispConReg->WINCON3 |= ENWIN_F_ENABLE;
            }
            else
            {
                g_pDispConReg->WINCON3 &= ~ENWIN_F_ENABLE;
                g_pDispConReg->SHADOWCON &= ~(1<<3);
            }
            break;
        case DISP_WIN4:
            if (WinOnOff == DISP_WINDOW_ON)
            {
                g_pDispConReg->SHADOWCON |= (1<<4);
                g_pDispConReg->WINCON4 |= ENWIN_F_ENABLE;
            }
            else
            {
                g_pDispConReg->WINCON4 &= ~ENWIN_F_ENABLE;
                g_pDispConReg->SHADOWCON &= ~(1<<4);
            }
            break;
        default:
            printf("[DISP:ERR] Disp_window_onfoff() : Unknown Window Number [%d]\n", Win);
            break;
        }
    }
    else
        printf("[DISP:ERR] Disp_window_onfoff() : Unknown OnOff Parameter [%d]\n", WinOnOff);
}

void Disp_envid_onoff(DISP_ENVID_ONOFF EnvidOnOff)
{
//    printf("[DISP]++Disp_envid_onoff(%d)\n", EnvidOnOff);

    if (EnvidOnOff == DISP_ENVID_ON)
    {
        g_pDispConReg->VIDCON0 |= (ENVID_ENABLE | ENVID_F_ENABLE);
#if defined(CONFIG_S9) || defined(CONFIG_A9)
        reset_cpld();
#endif
    }
    else if (EnvidOnOff == DISP_ENVID_OFF)
    {
        g_pDispConReg->VIDCON0 &= ~(ENVID_F_ENABLE);    // Per Frame Off
    }
    else if (EnvidOnOff == DISP_ENVID_DIRECT_OFF)
    {
        g_pDispConReg->VIDCON0 &= ~(ENVID_ENABLE | ENVID_F_ENABLE);    // Direct Off
    }
    else
        printf("[DISP:ERR] Disp_envid_onoff() : Unknown Parameter [%d]\n", EnvidOnOff);

//    printf("[DISP:INF] DUALCON = 0x%08x\n", g_pDispConReg->DUALCON);
//    printf("[DISP:INF] VIDCON2    = 0x%08x\n", g_pDispConReg->VIDCON2);
//    printf("[DISP:INF] DISPLAY_CONTROL  = 0x%08x\n", readl(DISPLAY_CONTROL));
//    printf("[DISP:INF] GPOUTCON0    = 0x%08x\n", g_pDispConReg->GPOUTCON0);
//    printf("[DISP:INF] VIDCON0 = 0x%08x\n", g_pDispConReg->VIDCON0);
//    printf("[DISP:INF] CLK_SRC_MASK0 = 0x%08x\n", readl(CLK_SRC_MASK0));
//    printf("[DISP:INF] CLK_SRC1    = 0x%08x\n", readl(CLK_SRC1));
//    printf("[DISP:INF] CLK_DIV1  = 0x%08x\n", readl(CLK_DIV1));
//    printf("[mDNIe:INF] CLK_SRC_MASK1 = 0x%08x\n", readl(CLK_SRC_MASK1));
//    printf("[mDNIe:INF] CLK_SRC3    = 0x%08x\n", readl(CLK_SRC3));
//    printf("[mDNIe:INF] CLK_DIV3  = 0x%08x\n", readl(CLK_DIV3));
//
//    printf("[DISP:INF]%s() : VIDCON0 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDCON0);
//    printf("[DISP:INF]%s() : VIDCON1 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDCON1);
//    printf("[DISP:INF]%s() : VIDCON2 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDCON2);
//    printf("[DISP:INF]%s() : VIDCON3 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDCON3);
//    printf("[DISP:INF]%s() : VIDTCON0 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDTCON0);
//    printf("[DISP:INF]%s() : VIDTCON1 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDTCON1);
//    printf("[DISP:INF]%s() : VIDTCON2 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDTCON2);
//    printf("[DISP:INF]%s() : VIDOSD2A = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDOSD2A);
//    printf("[DISP:INF]%s() : VIDOSD2B = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDOSD2B);
//    printf("[DISP:INF]%s() : VIDOSD2C = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDOSD2C);
//    printf("[DISP:INF]%s() : VIDW02ADD0B0 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDW02ADD0B0);
//    printf("[DISP:INF]%s() : VIDW02ADD1B0 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDW02ADD1B0);
//    printf("[DISP:INF]%s() : VIDW02ADD2 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->VIDW02ADD2);
//    printf("[DISP:INF]%s() : WINCON2 = 0x%08x\n", (__FUNCTION__),g_pDispConReg->WINCON2);
//
//    printf("[DISP:INF] CLK_GATE_IP0 = 0x%08x\n", readl(CLK_GATE_IP0));
//    printf("[DISP:INF] CLK_GATE_IP1 = 0x%08x\n", readl(CLK_GATE_IP1));
//    printf("[DISP:INF] CLK_GATE_IP2 = 0x%08x\n", readl(CLK_GATE_IP2));
//    printf("[DISP:INF] CLK_GATE_IP3 = 0x%08x\n", readl(CLK_GATE_IP3));
//    printf("[DISP:INF] CLK_GATE_IP4 = 0x%08x\n", readl(CLK_GATE_IP4));
//    printf("[DISP:INF] CLK_GATE_BLOCK = 0x%08x\n", readl(CLK_GATE_BLOCK));
}
unsigned int get_battery_volme(unsigned int adc_value)
{
	unsigned int batt_vol;
	batt_vol = (adc_value - BATTERY_MIN_ADC)*VOLTAGE_OFF/BATTERY_ADC_OFF + BATTERY_MIN_VOLTAGE;
	return batt_vol;
}

int power_is_full()
{
#if defined(CONFIG_A8388_V1)	
	int tmp =  0;
  	tmp = readl(GPH2CON) & ~(0xF<<24);
  	writel(tmp, GPH2CON);
  	tmp = readl(GPH2DAT) & (0x1<<6);
	//printf("power is full:%s\n",tmp?"YES":"NO");
	return tmp;
#elif defined(CONFIG_A8388_V2)	
	int tmp =  0;
	tmp = readl(GPH2CON) & ~(0xF<<24);
	writel(tmp, GPH2CON);
	tmp = readl(GPH3CON) & ~(0xF<<0);
	writel(tmp, GPH3CON);
	tmp = 0x3&(((readl(GPH2DAT) & (0x1<<6))>>6)<<1 | readl(GPH3DAT) & (0x1<<0));
	if(tmp == 1)
		return tmp;
	else
		return 0;
#elif 	defined(CONFIG_S9) || defined(CONFIG_A9)

	int tmp =  0;
	tmp = readl(GPH2CON) & ~(0xF<<24);
	writel(tmp, GPH2CON);
	tmp = readl(GPH2PUD) & ~(0x3<<12);
	writel(tmp, GPH2PUD);
	
	tmp = readl(GPH1CON) & ~(0xF<<12);
	writel(tmp, GPH1CON);
	tmp = readl(GPH1PUD) & ~(0x3<<6);
	writel(tmp, GPH1PUD);
	
	tmp = 0x3&(((readl(GPH2DAT) & (0x1<<6))>>6)<<1 | (readl(GPH1DAT) & (0x1<<3))>>3);
	
	if(tmp == 1)
		return tmp;
	else
		return 0;
#endif
}

void power_down_backlight()
{
	unsigned int tmp;
#if defined(CONFIG_S5PV210_DEVBOARD) || defined(CONFIG_A8388_V1) || defined(CONFIG_S9) || defined(CONFIG_A8388_V2) || defined(CONFIG_A9)
  //lcd power ctl GPH2(5) DS90C365
  tmp = readl(GPH2CON) & ~(0xF<<20) | (0x1<<20);
  writel(tmp, GPH2CON);
  tmp = readl(GPH2DAT) & ~(0x1<<5);
  writel(tmp, GPH2DAT);

  //lvds chip power ctl GPJ2(3) 
#ifdef CONFIG_LCD_PIXELQI
  tmp = readl(GPJ2CON) & ~(0xF<<12) | (0x1<<12);
  writel(tmp, GPJ2CON);
  tmp = readl(GPJ2DAT) & ~(0x1<<3);
  writel(tmp, GPJ2DAT);

  //led onoff  GPH0(6)
  tmp = readl(GPH0CON) & ~(0xF<<24) | (0x1<<24);
  writel(tmp, GPH0CON);
  if(check_lcd_mode())
    tmp = readl(GPH0DAT) & ~(0x1<<6);  
  else
    tmp = readl(GPH0DAT) | (0x1<<6);  
  writel(tmp, GPH0DAT);
#endif
#ifdef CONFIG_LCD_LG_V2
  //BLED POWER DOWN GPJ2(2)
  tmp = readl(GPJ2CON) & ~(0xF<<8) | (0x1<<8);
  writel(tmp, GPJ2CON);
  tmp = readl(GPJ2DAT) & ~(0x1<<2);
  writel(tmp, GPJ2DAT);

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

  //pwm off GPD0(0)
  tmp = readl(GPD0CON) & ~(0xF<<0) | (0x1<<0);
  writel(tmp, GPD0CON);
  tmp = readl(GPD0DAT) & ~(0x1<<0);
  writel(tmp, GPD0DAT);

  //lcd led power on   GPJ2(2)
  tmp = readl(GPJ2CON) & ~(0xF<<8) | (0x1<<8);
  writel(tmp, GPJ2CON);
#ifdef CONFIG_LCD_PIXELQI
  if(check_lcd_mode())
   tmp = readl(GPJ2DAT) & ~(0x1<<2);
  else
   tmp = readl(GPJ2DAT) | (0x1<<2);  
#else
  tmp = readl(GPJ2DAT) & ~(0x1<<2);
#endif
  writel(tmp, GPJ2DAT);
#endif /* CONFIG_S5PV210_DEVBOARD CONFIG_A8388_V1 CONFIG_S9 CONFIG_A8388_V2*/

#if defined(CONFIG_S5PV210_DEVBOARD)
//核心板lcd接口
  //lcd led power on   GPH2(3)
  tmp = readl(GPH2CON) & ~(0xF<<12) | (0x1<<12);
  writel(tmp, GPH2CON);
  tmp = readl(GPH2DAT) | (0x1<<3);
  writel(tmp, GPH2DAT);
#endif
}
void LCD_color(unsigned int color)
{
	u32 i, j;
	u32* pBuffer = (u32*)CFG_LCD_FBUFFER;
  for(i=0;i<lcd_height;i++)
    for(j=0;j<lcd_width;j++)
      *pBuffer++ = color;
}
void DelayLoop(int count)
{
  udelay(count*3000);

}

int mmc_get_pic_info(int start_add, int *width, int *height)
{
	char run_cmd[100];
  unsigned int start_block = start_add/MMC_BLOCK_SIZE;
  unsigned int total_length, total_block;
  unsigned short *buff;
  
  buff = (unsigned short *)CFG_LCD_TMP_BUFFER;

  sprintf(run_cmd,"mmc read %d 0x%lx 0x%lx 1", BOOT_MMC, CFG_LCD_TMP_BUFFER, start_block);
	run_command(run_cmd, 0);

//printf("0x%02x 0x%02x 0x%02x\n", buff[0], buff[1], buff[2]);
  if((buff[0] != 0x2000) || (buff[1] > g_DevInfoDisp.RGBINFO.uiWidth) || (buff[2] > g_DevInfoDisp.RGBINFO.uiHeight))
    return 0;

  *width = buff[1];
  *height = buff[2];

  total_length = buff[1]*buff[2]*4+8;

  if((total_length%MMC_BLOCK_SIZE) > 0)
    total_block = total_length/MMC_BLOCK_SIZE + 1;
  else
    total_block = total_length/MMC_BLOCK_SIZE;

  sprintf(run_cmd,"mmc read %d 0x%lx 0x%lx 0x%lx", BOOT_MMC, CFG_LCD_TMP_BUFFER, start_block, total_block);
	run_command(run_cmd, 0);
  return 1;
}
/*
*函数名: mmc_get_data
*作用:从INAND中读取指定长度的数据,长度/单位字节数
*参数:
	start_add:起始地址 /字节数
	length;要读取的长度
	
*返回值:
*作者:
*/
int mmc_get_data(int start_add, int length)
{
	char run_cmd[100];
  unsigned int start_block = start_add/MMC_BLOCK_SIZE;
  unsigned int  total_block;
  unsigned short *buff;
  
 #if 0 
  buff = (unsigned short *)CFG_LCD_TMP_BUFFER;

  sprintf(run_cmd,"mmc read %d 0x%lx 0x%lx 1", BOOT_MMC, CFG_LCD_TMP_BUFFER, start_block);
	run_command(run_cmd, 0);

  printf("0x%02x 0x%02x 0x%02x+++++++++\n", buff[0], buff[1], buff[2]);
  if((buff[0] != 0x2000) || (buff[1] > g_DevInfoDisp.RGBINFO.uiWidth) || (buff[2] > g_DevInfoDisp.RGBINFO.uiHeight))
    return 0;

  *width = buff[1];
  *height = buff[2];
  total_length = buff[1]*buff[2]*4+8;
#endif
  
  if((length%MMC_BLOCK_SIZE) > 0)
    total_block = length/MMC_BLOCK_SIZE + 1;
  else
    total_block = length/MMC_BLOCK_SIZE;

  sprintf(run_cmd,"mmc read %d 0x%lx 0x%lx 0x%lx", BOOT_MMC, CFG_LCD_TMP_BUFFER, start_block, total_block);
	run_command(run_cmd, 0);
  return 1;
}

int get_pic_info(int start_add, int *width, int *height)
{
  unsigned short *buff;
  buff = (unsigned short *)start_add;

//printf("0x%02x 0x%02x 0x%02x\n", buff[0], buff[1], buff[2]);
  if((buff[0] != 0x2000) || (buff[1] > g_DevInfoDisp.RGBINFO.uiWidth) || (buff[2] > g_DevInfoDisp.RGBINFO.uiHeight))
    return 0;

  *width = buff[1];
  *height = buff[2];
  
  return 1;
}

void show_buf(unsigned char *start_buf, int width, int height, int x, int y)
{
  int i,j, w, h;
  unsigned char *dst_start, *src_start;
  int i_tmp, d_tmp, s_tmp;
	unsigned int *dst_tmp;
	unsigned int *src_tmp;
  
//printf("show_buf width=%d height=%d x=%d y=%d\n", width, height, x, y);
  w = x+width;
  h = y+height;
  
  if(w >= lcd_width)
    w = lcd_width-x;
  else
    w = width;
  
  if(h >= lcd_height)
    h = lcd_height-y;
  else
    h = height + y;
	
	#ifdef ANGLE_0
  src_start = start_buf + 8;
  dst_start = CFG_LCD_FBUFFER + (lcd_width*y+x)*4;
	
  i_tmp = w*4;
  d_tmp = lcd_width*4;
  s_tmp = width*4;
  
  for(i=y; i<h; i++)
  {
    memcpy((void *)dst_start, (void *)src_start, i_tmp);
    dst_start += d_tmp;
    src_start += s_tmp;
  }
	#elif defined(ANGLE_90)
	#elif defined(ANGLE_180)

	src_start = start_buf + 8;
  dst_start = CFG_LCD_FBUFFER + (lcd_width*lcd_height)*4;

	src_tmp = (unsigned int*)src_start;
	dst_tmp = (unsigned int*)dst_start;
	
  for(i=0; i<lcd_height; i++)
  {
		for(j = 0;j<lcd_width;j++)
		{
			*dst_tmp-- = *src_tmp++;
		}
  }
	#elif defined(ANGLE_270)
	#endif
}
/*
*函数名: LCDRectangle(unsigned int x0,unsigned int y0,unsigned int x1,unsigned y1,unsigned int* data,unsigned int* pLCD)
*功能:在LCD的指定位置上画一块矩形区域，并填充数据
*参数:(x0,y0),(x1,y1)为矩形区域坐标,并(x0,y0)<(x1,y1)坐标系如下:
         0
         -|------------------->y
          |
          |
          |
          |
       x  |
          v
*data:为要填充的数据buffer的指针
*pLCD:为显存首地址指针
*/

void LCDRectangle(unsigned int x0,unsigned int y0,unsigned int x1,unsigned y1,unsigned char* data,unsigned char* pLCD,unsigned int deep)
{
   unsigned char *pFB = NULL;
   unsigned char *p = NULL;
   unsigned char *pdata = NULL;
   unsigned i,j,n ;
	 unsigned int *dst_tmp;
	 unsigned int *src_tmp;
	 
   pFB = (unsigned char*)pLCD;
   pdata = data;
   
   if(x0>=MY_LCD_HEIGHT||y0>=MY_LCD_WIDTH||x1>=MY_LCD_HEIGHT||y1>=MY_LCD_WIDTH)
   {
   	printf("arguments invalid 1+++++\n");
 		return;
   }
   
   if(x0*x1*y0*y1<0||(pLCD == NULL)||(data==NULL))
   {
   	printf("arguments invalid 2++++\n");
 		return;
   }
   #ifdef ANGLE_0
   for(i = x0;i < x1;i ++)
   	
   	{
   	   p = pFB;
   	   p = p +( i*lcd_width+y0)*deep ;
   	   for(j = y0;j < y1;j ++)
   	   {
	   			for(n = 0;n<deep;n++)
					*p++ = *pdata++;
	
	 
   	   }
   	}
	 #elif defined(ANGLE_90)
	 #elif defined(ANGLE_180)
	
		src_tmp = (unsigned int*)pdata;
		
	 for(i = x1;i > x0;i --)
   	
   	{
   	   p = pFB;
   	   p = p + ( i*lcd_width+y1)*deep ;
			 dst_tmp = (unsigned int*)p;
			 
   	   for(j = y1;j > y0;j --)
   	   {

						if(src_tmp==NULL)
						{
								printf(" ERROR:LCDRectangle   src_tmp is null!!!!!!!!!!!!!!!\n");
								return;
						}
						*dst_tmp-- = *src_tmp++;
	 
   	   }
   	}
	 #elif defined(ANGLE_270)
	 #endif
}  

void show_pic(unsigned int position, int x, int y, int lcdonoff, int ifcenter, int ifinline)
{
  int width, height;
  int ret;
  
  if(lcdonoff)
    Disp_envid_onoff(DISP_ENVID_OFF);
 
  memset((void *)CFG_LCD_FBUFFER, 0x00, lcd_width*lcd_height*4);
  
  if(ifinline)
    ret = get_pic_info(position, &width, &height);
  else
    ret = mmc_get_pic_info(position, &width, &height);

  if(ret)
  {
    if(ifcenter)
    {
      x = (lcd_width-width)/2;
      y = (lcd_height-height)/2;
    }
    if(ifinline)
      show_buf((void *)position, width, height, x, y);
    else
      show_buf((void *)CFG_LCD_TMP_BUFFER, width, height, x, y);
  }
  else
    LCD_color(0x000000FF);

  if(lcdonoff)
   Disp_envid_onoff(DISP_ENVID_ON);
}

/*
*函数名:void Showbackground(void)
*作用:显示充电背景图片
*参数:
	position:背景图片在存储地址
	x,y;在LCD上显示的起始坐标
	
	
*返回值:空
*作者:
*/
void Showbackground(unsigned int position, int x, int y, int lcdonoff, int ifcenter, int ifinline)
{
  int width, height;
  int ret;
  
  if(lcdonoff)
    Disp_envid_onoff(DISP_ENVID_OFF);
 
  memset((void *)CFG_LCD_FBUFFER, 0x00, lcd_width*lcd_height*4);
  
  if(ifinline)
    ret = get_pic_info(position, &width, &height);
  else
    ret = mmc_get_pic_info(position, &width, &height);

  if(ret)
  {
    if(ifcenter)
    {
      x = (lcd_width-width)/2;
      y = (lcd_height-height)/2;
    }
    if(ifinline)
      show_buf((void *)position, width, height, x, y);
    else
      show_buf((void *)CFG_LCD_TMP_BUFFER, width, height, x, y);
  }
  else
    LCD_color(0x000000FF);

  if(lcdonoff)
   Disp_envid_onoff(DISP_ENVID_ON);
}
void show_sdfluse_pic(unsigned char* position, int x, int y, int lcdonoff, int ifcenter, int ifinline)
{
  int width, height;
  int ret;
  unsigned char *ptmp = position;
	
  if(lcdonoff)
    Disp_envid_onoff(DISP_ENVID_OFF);
 
  memset((void *)CFG_LCD_FBUFFER, 0x00, lcd_width*lcd_height*4);
  
  if(ifinline)
    ret = get_pic_info(position, &width, &height);
  else
    ret = mmc_get_pic_info(position, &width, &height);

  if(ret)
  {
    if(ifcenter)
    {
      x = (lcd_width-width)/2;
      y = (lcd_height-height)/2;
    }
    if(ifinline)
    {
    	LCDRectangle(x, y, x + height, y + width, ptmp + 8, CFG_LCD_FBUFFER, 4);
    }
    else
      show_buf((void *)CFG_LCD_TMP_BUFFER, width, height, x, y);
  }
  else
    LCD_color(0x000000FF);

  if(lcdonoff)
   Disp_envid_onoff(DISP_ENVID_ON);
}


#define S5P_PWR_CFG  0xE010C000
#define S5P_CFG_WFI_SLEEP (0x3<<8)
#define S5P_OTHERS 0xE010E000
#define S5P_OTHER_SYSC_INTOFF		(1 << 0)

#define VA_VIC(x)		(0x00000000 + ((x) * 0x10000))
#define VA_VIC0			VA_VIC(0)
#define VA_VIC1			VA_VIC(1)
#define VA_VIC2			VA_VIC(2)
#define VA_VIC3			VA_VIC(3)
#define VIC_INT_ENABLE_CLEAR		0x14

#define EXT_INT0_CON	0xE0200E00
#define EXT_INT0_FLTCON0	0xE0200E80
#define GPH0CON		0xE0200c00

#define EINT_WAKEUP_MASK	0xE010C004
#define EINT_INT0_PEND		0xE0200F40
#define IDLE_CFG                0xE020C020
#define NORMAL_CFG		0xe020c010
#define CLK_GATE_IP0 		0xE0100460
#define HDMI_CONTROL            0xE010E804
void gotowork()
{
	int tmp = 0;
	
	tmp = __raw_readl(CLK_GATE_IP3);
	
	tmp |= (0x1<<19);
	__raw_writel(tmp, CLK_GATE_IP3);
}
void gotosleep()
{
	int tmp = 0;
//fighter++
  writel(0, GPF0CON);
  writel(0x5555, GPF0PUD);
  
  writel(0, GPF1CON);
  writel(0x5555, GPF1PUD);
  
  writel(0, GPF2CON);
  writel(0x5555, GPF2PUD);
  
  writel(0, GPF3CON);
  writel(0x55, GPF3PUD);  
//fighter--
	tmp = __raw_readl(NORMAL_CFG);
	
	tmp &= ~(0x1<<20);
	tmp &= ~(0x1<<7);
	tmp &= ~(0x1F<<1);
	//tmp = 0;
	__raw_writel(tmp, NORMAL_CFG);

	tmp = __raw_readl(CLK_GATE_IP0);
	
	tmp &= ~(0x1<<29);
	tmp &= ~(0x1<<26);
	tmp &= ~(0x1<<25);
	tmp &= ~(0x1<<24);
	tmp &= ~(0x1<<16);
	tmp &= ~(0x1<<8);
	tmp &= ~(0x3<<3);
	tmp &= ~(0x1<<2);
	tmp &= ~(0x1<<1);
	__raw_writel(tmp, CLK_GATE_IP0);

	tmp = __raw_readl(CLK_GATE_BLOCK);
	
	tmp &= ~(0x1<<10);
	tmp &= ~(0x1<<8);
	tmp &= ~(0x1<<7);
	tmp &= ~(0x1<<5);
	tmp &= ~(0x1<<4);
	tmp &= ~(0xF<<4);
	
	__raw_writel(tmp, CLK_GATE_BLOCK);

	tmp = __raw_readl(CLK_GATE_IP1);
	
	//tmp &= ~(0xF<<8);
	//tmp &= ~(0x1<<2);
	
	__raw_writel(tmp, CLK_GATE_IP1);

	tmp = __raw_readl(CLK_GATE_IP2);
	
	tmp &= ~(0x1<<11);
	tmp &= ~(0x3<<8);
	tmp &= ~(0x3<<0);
	__raw_writel(tmp, CLK_GATE_IP2);

	tmp = __raw_readl(CLK_GATE_IP3);
	
	tmp &= ~(0x1<<20);
	tmp &= ~(0x1<<19);
	tmp &= ~(0x3<<17);
	tmp &= ~(0x1f<<9);
	tmp &= ~(0xf<<4);
	tmp &= ~(0x3<<0);
	__raw_writel(tmp, CLK_GATE_IP3);

	tmp = __raw_readl(HDMI_CONTROL);

	tmp &= ~(0x1<<0);
	__raw_writel(tmp, HDMI_CONTROL);

	tmp = __raw_readl(HDMI_CONTROL + 0x4);

	tmp &= ~(0x3<<0);
	__raw_writel(tmp, HDMI_CONTROL + 0x4);
	
	
	
}

/*
*函数名:void get_animation_data(unsigned char *read_buf)
*作用:显示动画效果，通过刷多张图片实现
*参数:data_buf  存放数据的缓冲区地址指针  
*返回值:空
*作者:fighter
*充电动画数据存储布局:
*                ------------------------------------------------      低地址
*                                背景图片                               |
*                ------------------------------------------------       | 
*                                3组电池图片                            |
*                                 每组4张                               |
*                                                                       |
*                -------------------------------------------------      |
*                                                                       |
*                          11张显示电池电量的图片                       |
*                                                                       |
*                -------------------------------------------------     高地址
*/
#if 1
void show_animation()
{

  int num = 0;
  int i = 0,j = 0;
  int begin = 0;
  int battery_vol_change = 1;
  float battery_float_tmp = 0;
  int battery_volue_new = 0;
  int battery_volue_old = 0xffff;
  int tmp = 0;
  unsigned char *sub_logo;
  unsigned char *sub_logo1;
  unsigned char *sub_logo2;
  unsigned char *sub_logo3;
  unsigned char *bat_vol;
 
  unsigned char *ptmp;
  unsigned char *ptmp1;
  unsigned char *ptmp2;
  unsigned char *ptmp3; 
  unsigned char *ptmp4;
         
  unsigned long adc_data = 0;
  unsigned long tmp_data = 0;

  sub_logo = (unsigned char *)CFG_LCD_TMP_BUFFER;               /*data_buf指向的内存区域包含背景图片和几张动画图片*/
// sub_logo = sub_logo+ MAIN_LOGO_SIZE + DATA_HEAD_LEN;          /*得到存储动画图片数据的内存首地址*/
  ptmp = sub_logo;
 
  for(i = 0;i<SUB_LOGO_ALL_NUM;i++)
  {
    //  printf("sub_logo[%d] = 0x%x+++++++++++++++\n",i,sub_logo);
  	if((sub_logo[1] != 0x20))
  	{
		printf("The sub logo[%d] is not right,please check! ++++++++++++++\n",i);
		printf("0x%02x 0x%02x 0x%02x\n", sub_logo[0], sub_logo[1], sub_logo[2]);
  	 }
  	
  	sub_logo += SUB_LOGO_SIZE;
  }	

  for(i = 0;i<BATTERY_VOL_LOGO_NUM;i++)
  {
    //  printf("BATTERY sub_logo[%d] = 0x%x+++++++++++++++\n",i,sub_logo);
  	if((sub_logo[1] != 0x20))
  	{
		printf("The BATTERY sub logo[%d] is not right,please check! ++++++++++++++\n",i);
		printf("0x%02x 0x%02x 0x%02x\n", sub_logo[0], sub_logo[1], sub_logo[2]);
  	 }
  	
  	sub_logo += BATTERY_VOL_LOGO_SIZE;
  }
  /*总共有3组图片，每组4张，这里得到每组第一张图片的首地址，
    *注意这里得到每帧图片的首地址开头8个字节为数据头，后面要
    *画图的时候要去掉
  */
                                                                      
  sub_logo1 = ptmp;
  sub_logo2 = sub_logo1 + SUB_LOGO_SIZE*NUM_PER_GROUP;
  sub_logo3 = sub_logo2 + SUB_LOGO_SIZE*NUM_PER_GROUP;
  bat_vol = sub_logo3 + SUB_LOGO_SIZE*NUM_PER_GROUP;
                                               					  
  ptmp1 = sub_logo1;                                                                             /*备份每组图片的首地址*/
  ptmp2 = sub_logo2;
  ptmp3 = sub_logo3;
  ptmp4 = bat_vol;
  
  for(j = 0;j < 5;j++)                                                                          /*进入while循环前测量一次电池电压(取10次的平均值)*/
  {
  	adc_data += s5pc210_read_adc_data(0);
		DelayLoop(10);
  }
  tmp_data = adc_data/5;
  adc_data = 0;
  j = 0;
 
   reset_timer();
   set_timer(0);
   begin = get_timer(0);

	while(1)
	{	
//	 printf("runing+++++++++++++++++\n");
	 
		while( power_is_full())
		{
			//printf("power is full +++++++++++++++++\n");
			if(BG_TIME_OUT<get_timer(begin))	
			{
				reset_timer();
   			set_timer(0);
  			begin = get_timer(0);
				power_down_backlight();	                                             /*显示充电画面15秒后若无按键关闭背光*/
				power_init_off();
				gotosleep();
				is_lcdon = 0;
			}
			LCDRectangle(X0,Y0,X1,Y1,sub_logo3+DATA_HEAD_LEN,CFG_LCD_FBUFFER,4);
			bat_vol = ptmp4 + DATA_HEAD_LEN + BATTERY_VOL_LOGO_SIZE*10;
			LCDRectangle(BX0,BY0,BX1,BY1,bat_vol,CFG_LCD_FBUFFER,4);
			DelayLoop(300);
			if(check_power_key())
				goto GET_OUT;
		
			if(is_lcdon)
			{
				lcd_on();
				is_lcdon = 0;
			}	
		}
		//printf("power is not full +++++++++++++++++\n");
		if(j<3)
		{
			adc_data += s5pc210_read_adc_data(0);
			j++;

		}else
			{
				adc_data /=3;
				tmp_data = adc_data;
				adc_data = 0;
				j = 0;
		
			}
		//printf("tmp_data = %d+++++++++++\n",tmp_data);
    if(tmp_data <= BATT_VOL_L)
    {
        /*电池电压低时充电画面*/
      for(i = 0;i<NUM_PER_GROUP;i++)
 			{
 				LCDRectangle(X0,Y0,X1,Y1,sub_logo1+DATA_HEAD_LEN,CFG_LCD_FBUFFER,4);
				DelayLoop(100);
				sub_logo1 += SUB_LOGO_SIZE;
	 			if(check_power_key())
					goto GET_OUT;
 			}
			sub_logo1 = ptmp1; 
     }else if(tmp_data <= BATT_VOL_M && tmp_data > BATT_VOL_L)
			{
				/*电池电压中等时充电画面*/
		      for(i = 0;i<NUM_PER_GROUP;i++)
					{
						LCDRectangle(X0,Y0,X1,Y1,sub_logo2+DATA_HEAD_LEN,CFG_LCD_FBUFFER,4);
						DelayLoop(100);
						sub_logo2 += SUB_LOGO_SIZE;
						if(check_power_key())
							goto GET_OUT;
					}
					sub_logo2 = ptmp2; 
			}else if( tmp_data > BATT_VOL_M)
					{
						/*电池电压高时充电画面*/
			      for(i = 0;i<NUM_PER_GROUP;i++)
						{
							LCDRectangle(X0,Y0,X1,Y1,sub_logo3+DATA_HEAD_LEN,CFG_LCD_FBUFFER,4);
							DelayLoop(100);
							sub_logo3 += SUB_LOGO_SIZE;
							if(check_power_key())
							goto GET_OUT;
						}
						sub_logo3 = ptmp3; 
					}
				
	if(tmp_data > BATTERY_MIN_ADC)				
        {
	        tmp = get_battery_volme(tmp_data);	
					if(tmp>=BATTERY_MAX_VOLTAGE&&!power_is_full())
						tmp = BATTERY_MAX_VOLTAGE - 10; //当AD读到的值大于最大电压，而充电IC并为充满时，此处减去一个数(<21),使显示为90%
	        battery_volue_new = ((tmp - BATTERY_MIN_VOLTAGE)*10/(BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE));
	 }else
	        battery_volue_new = 0;                
	 
	if(battery_volue_new != battery_volue_old)
	{
	  bat_vol = ptmp4 + DATA_HEAD_LEN + BATTERY_VOL_LOGO_SIZE*battery_volue_new;
		LCDRectangle(BX0,BY0,BX1,BY1,bat_vol,CFG_LCD_FBUFFER,4);
		battery_volue_old = battery_volue_new;
	}
	
	if(BG_TIME_OUT<get_timer(begin))	
	{
		reset_timer();
   	set_timer(0);
  	begin = get_timer(0);
		power_down_backlight();	                                             /*显示充电画面15秒后若无按键关闭背光*/
		power_init_off();
		gotosleep();
		is_lcdon = 0;
		
	}
	if(is_lcdon)
	{
		lcd_on();
		is_lcdon = 0;
	}
	
 }	
 
 GET_OUT:
  reset_cpu (0);
	//show_pic(PIC_BOOT, 0, 0, 0, 0, 0);
	return;
 	   
}
#endif
#define S5PV_RTCCON         0xE2800040
#define S5PV_RTCALM         0xE2800050
#define S5PV_RTC_BCDYEAR    0xE2800088
#define S5PV_RTC_ALMYEAR    0xE2800068
#define S3C2410_RTCCON_RTCEN  (1<<0)
#define S3C2410_RTCCON_CLKSEL (1<<1)
#define S3C2410_RTCCON_CNTSEL (1<<2)
#define S3C2410_RTCCON_CLKRST (1<<3)
static void smdkc110_rtc_enable(int en)
{
	unsigned int tmp;

	if (!en) {
		tmp = readl(S5PV_RTCCON);
		writel(tmp & ~(S3C2410_RTCCON_RTCEN), S5PV_RTCCON);
	} else {
		/* re-enable the device, and check it is ok */

		if ((readl(S5PV_RTCCON) & S3C2410_RTCCON_RTCEN) == 0) {
//			dev_info(dev, "rtc disabled, re-enabling\n");

			tmp = readl(S5PV_RTCCON);
			writel(tmp|S3C2410_RTCCON_RTCEN, S5PV_RTCCON);
		}

		if ((readl(S5PV_RTCCON) & S3C2410_RTCCON_CNTSEL)) {
//			dev_info(dev, "removing RTCCON_CNTSEL\n");

			tmp = readl(S5PV_RTCCON);
			writel(tmp & ~S3C2410_RTCCON_CNTSEL,S5PV_RTCCON);
		}

		if ((readl(S5PV_RTCCON) & S3C2410_RTCCON_CLKRST)) {
//			dev_info(dev, "removing RTCCON_CLKRST\n");

			tmp = readl(S5PV_RTCCON);
			writel(tmp & ~S3C2410_RTCCON_CLKRST,S5PV_RTCCON);
		}
	}
}

void LCD_turnon(void)
{
  unsigned int val;
  unsigned int update = 0;
  unsigned int sdfuse = 0;
  unsigned int backgroundsize = 0;	
  unsigned int show = 0;
	unsigned int temp;
	char cmd[16];
	
  g_pDispConReg = S5PC11X_GetBase_LCD();
  // Initialize Display Power Gating
  if(!(readl(BLK_PWR_STAT) & 0x1<<3))
  {
    val = readl(NORMAL_CFG);
    writel( val | (0x1<<3), NORMAL_CFG);
    while(!(readl(BLK_PWR_STAT) & 0x1<<3));
  }

  // Initialize Display Controller
  LDI_fill_output_device_information(&g_DevInfoDisp);
  Disp_initialize_output_interface();  
  Disp_set_window_mode(DISP_WIN2_DMA, DISP_24BPP_888, g_DevInfoDisp.RGBINFO.uiWidth, g_DevInfoDisp.RGBINFO.uiHeight, 0, 0);
  Disp_set_framebuffer(DISP_WIN2, CFG_LCD_FBUFFER);
  Disp_window_onfoff(DISP_WIN2, DISP_WINDOW_ON);

  lcd_width = g_DevInfoDisp.RGBINFO.uiWidth;
  lcd_height = g_DevInfoDisp.RGBINFO.uiHeight;
  
  if(check_power_key())
  {
		smdkc110_rtc_enable(1);

		val = readl(S5PV_RTC_BCDYEAR) & 0xfff;
		if(recovery_flag == val)
		{
			sprintf(cmd, "fdisk -c %d", BOOT_MMC);
  		run_command(cmd, 0);
  		run_command("sdfuse erase cache", 0);
			run_command("sdfuse erase userdata", 0);

			writel(normal_flag,S5PV_RTC_BCDYEAR);
		 }
		if(update_flag == val)
		{
			writel(normal_flag,S5PV_RTC_BCDYEAR);
			show_sdfuse_pic();
			lcd_on();
			auto_sdfuse();
	  }
		smdkc110_rtc_enable(0);
			
  	show_pic(PIC_BOOT, 0, 0, 0, 0, 0);
//    lcd_on();
  }
  else
  {	  
  	backgroundsize = POWER_ON_LOGO_SIZE + UPDATE_LOGO_SIZE;
  	Showbackground(PIC_BOOT + backgroundsize, 0, 0, 0, 0, 0);
    show = 1;
  }

#ifndef CONFIG_AUTO_SDFUSE
  if(resume_system && (check_resume_key()))
  {
    //显示升级图片
#if defined(CONFIG_S5PV210_DEVBOARD)
    //enable lcd
    lcd_on();
  
    // Video Output Enable
    Disp_envid_onoff(DISP_ENVID_ON);

    mdelay10(300);
    if(check_resume_key())
#endif
    {
      show_pic(PIC_BOOT + POWER_ON_LOGO_SIZE, 0, 0, 1, 0, 0);
      update = 1;
    }
  }
#ifdef CONFIG_SDFUSE
  //从TF卡升级
  if(sdfuse_system && (check_sdfuse_key()))
  {
    show_sdfuse_pic();
    sdfuse = 1;
  }
#endif /* CONFIG_SDFUSE */
#endif
    
  //enable lcd
  lcd_on();

  // Video Output Enable
  Disp_envid_onoff(DISP_ENVID_ON);

  if(update)
    run_command("fastboot", 0);
  
  if(sdfuse)
    auto_sdfuse();
  if(show == 1)
  {
   	mmc_get_data(PIC_BOOT + POWER_ON_LOGO_SIZE + UPDATE_LOGO_SIZE + BACKGROUND_LOGO_SIZE ,SUB_LOGO_SIZE*SUB_LOGO_ALL_NUM +BATTERY_VOL_LOGO_SIZE*BATTERY_VOL_LOGO_NUM);
    show_animation();
    show = 0;
  }
}

void LCD_setfgcolor(unsigned int color)
{
	gFgColor = color;
}

void LCD_setleftcolor(unsigned int color)
{
	gLeftColor = color;
}

void LCD_setprogress(int percentage)
{
	u32 i, j;
	u32* pBuffer = (u32*)CFG_LCD_FBUFFER;
	u32 height, width;

	width = g_DevInfoDisp.RGBINFO.uiWidth;
	height = g_DevInfoDisp.RGBINFO.uiHeight;

	for (i=0; i < (height/100)*percentage; i++)
	{
		for (j=0; j < width; j++)
		{
			*pBuffer++ = gFgColor;
		}
	}

	for (; i < height; i++)
	{
		for (j=0; j < (width >> 5); j++)
		{
			*pBuffer++ = gLeftColor;
		}
		for (; j < width; j++)
		{
			*pBuffer++ = LCD_BGCOLOR;
		}
	}
}
