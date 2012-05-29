#ifndef _FIMD_H_
#define _FIMD_H_

#define PROGRESSIVE         (0<<29)
#define INTERLACE           (1<<29)

#define VIDOUT_RGBIF        (0<<26)
#define CLKVALUP_ALWAYS     (0<<16)

#define MIPI_DSI_EN         (1<<30) // RGBINFO.MIPIDSIen
#define MIPI_DSI_DIS        (0<<30) // RGBINFO.MIPIDSIen

#define RGB_ORDER_E_RGB        (0<<19) // RGBINFO.RGBEvenOrder
#define RGB_ORDER_E_GBR        (1<<19) // RGBINFO.RGBEvenOrder
#define RGB_ORDER_E_BRG        (2<<19) // RGBINFO.RGBEvenOrder
#define RGB_ORDER_E_BGR        (4<<19) // RGBINFO.RGBEvenOrder
#define RGB_ORDER_E_RBG        (5<<19) // RGBINFO.RGBEvenOrder
#define RGB_ORDER_E_GRB        (6<<19) // RGBINFO.RGBEvenOrder
#define RGB_ORDER_E_MASK       (7<<19) // RGBINFO.RGBEvenOrder

#define RGB_ORDER_O_RGB        (0<<16) // RGBINFO.RGBOddOrder
#define RGB_ORDER_O_GBR        (1<<16) // RGBINFO.RGBOddOrder
#define RGB_ORDER_O_BRG        (2<<16) // RGBINFO.RGBOddOrder
#define RGB_ORDER_O_BGR        (4<<16) // RGBINFO.RGBOddOrder
#define RGB_ORDER_O_RBG        (5<<16) // RGBINFO.RGBOddOrder
#define RGB_ORDER_O_GRB        (6<<16) // RGBINFO.RGBOddOrder
#define RGB_ORDER_O_MASK       (7<<16) // RGBINFO.RGBOddOrder

#define CLKSEL_F_HCLK       (0<<2)  // RGBINFO.VCLK_Source
#define CLKSEL_F_LCDCLK     (1<<2)  // RGBINFO.VCLK_Source
#define CLKSEL_F_EXT27M     (3<<2)
#define CLKSEL_F_MASK       (3<<2)

// VIDCON1
#define VCLK_HOLD   (0<<9)
#define VCLK_RUNNING   (1<<9)
#define VCLK_RUNNING_DISABLE_VDEN   (3<<9)
#define VCLK_HOLD_MASK   (3<<9)
#define IVCLK_FALL_EDGE     (0<<7)  // RGBINFO.VCLK_Polarity
#define IVCLK_RISE_EDGE     (1<<7)  // RGBINFO.VCLK_Polarity
#define IHSYNC_HIGH_ACTIVE  (0<<6)  // RGBINFO.HSYNC_Polarity
#define IHSYNC_LOW_ACTIVE   (1<<6)  // RGBINFO.HSYNC_Polarity
#define IVSYNC_HIGH_ACTIVE  (0<<5)  // RGBINFO.VSYNC_Polarity
#define IVSYNC_LOW_ACTIVE   (1<<5)  // RGBINFO.VSYNC_Polarity
#define IVDEN_HIGH_ACTIVE   (0<<4)  // RGBINFO.VDEN_Polarity
#define IVDEN_LOW_ACTIVE    (1<<4)  //RGBINFO.VDEN_Polarity

// VIDCON2
#define RGB_SKIP_DISABLE    (0<<27)     // RGBINFO.RGBSkipEn
#define RGB_SKIP_ENABLE     (1<<27) // RGBINFO.RGBSkipEn
#define RGB_SKIP_MASK       (1<<27) // RGBINFO.RGBSkipEn
#define RGB_DUMMY_LAST      (1<<24)     // RGBINFO.RGBDummy
#define RGB_DUMMY_FIRST     (3<<24) // RGBINFO.RGBDummy
#define RGB_DUMMY_DISABLE   (0<<24)     // RGBINFO.RGBDummy
#define RGB_DUMMY_MASK      (3<<24) // RGBINFO.RGBDummy

#define VCLK_NORMAL         (0<<5)  // RGBINFO.VCLK_Gate
#define VCLK_FREERUN        (1<<5)  // RGBINFO.VCLK_Gate

#define CLKDIR_DIRECT       (0<<4)  // RGBINFO.VCLK_Direction
#define CLKDIR_DIVIDED      (1<<4)  // RGBINFO.VCLK_Direction

#define PIXEL_COMP_NEGATIVE     (3<<0)  // RGBINFO.PixelCompensation
#define PIXEL_COMP_POSITIVE     (1<<0)  // RGBINFO.PixelCompensation
#define PIXEL_COMP_DISABLE      (0<<0)  // RGBINFO.PixelCompensation

#define ENVID_DISABLE       (0<<1)
#define ENVID_ENABLE        (1<<1)
#define ENVID_F_DISABLE     (0<<0)
#define ENVID_F_ENABLE      (1<<0)

#define SET_RGBSPSEL(x)     (((x)>2) ? (1<<18):(0<<18)) // 3 = DISP_18BIT_RGB666_S
#define CLKVAL_F(n)         (((n-1)&0xff)<<6)
// VIDTCON0
#define VBPDE(n)            (((n-1)&0xff)<<24)
#define VBPD(n)             (((n-1)&0xff)<<16)
#define VFPD(n)             (((n-1)&0xff)<<8)
#define VSPW(n)             (((n-1)&0xff)<<0)

// VIDTCON1
#define VFPDE(n)            (((n-1)&0xff)<<24)
#define HBPD(n)             (((n-1)&0xff)<<16)
#define HFPD(n)             (((n-1)&0xff)<<8)
#define HSPW(n)             (((n-1)&0xff)<<0)

// VIDTCON2
#define LINEVAL(n)          (((n-1)&0x7ff)<<11)
#define HOZVAL(n)           (((n-1)&0x7ff)<<0)

#define BLEND_PER_PLANE     (0<<6)
#define BLEND_PER_PIXEL     (1<<6)
#define BLEND_MULTIPLIED    (3<<6)

#define ALPHASEL_ALPHA0     (0<<1)    // Per Plane
#define ALPHASEL_ALPHA1     (1<<1)
#define ALPHASEL_AEN        (0<<1)    // Per Pixel
#define ALPHASEL_DATA       (1<<1)

#define LOCALSEL_CAMIF1     (0<<23)
#define LOCALSEL_VP         (1<<23)
#define LOCAL_PATH_DISABLE  (0<<22)
#define LOCAL_PATH_ENABLE   (1<<22)
#define LOCAL_IN_RGB888     (0<<13)
#define LOCAL_IN_YUV444     (1<<13)

#define BITSWP_DISABLE      (0<<18)
#define BITSWP_ENABLE       (1<<18)
#define BYTSWP_DISABLE      (0<<17)
#define BYTSWP_ENABLE       (1<<17)
#define HAWSWP_DISABLE      (0<<16)
#define HAWSWP_ENABLE       (1<<16)
#define WSWP_DISABLE        (0<<15)
#define WSWP_ENABLE         (1<<15)

// SHADOWCON
#define W4_SHADOW_PROTECT   (1<<14)
#define W3_SHADOW_PROTECT   (1<<13)
#define W2_SHADOW_PROTECT   (1<<12)
#define W1_SHADOW_PROTECT   (1<<11)
#define W0_SHADOW_PROTECT   (1<<10)
#define W4_SHADOW_UNPROTECT(R)  ((R)&(~(1<<14)))
#define W3_SHADOW_UNPROTECT(R)  ((R)&(~(1<<13)))
#define W2_SHADOW_UNPROTECT(R)  ((R)&(~(1<<12)))
#define W1_SHADOW_UNPROTECT(R)  ((R)&(~(1<<11)))
#define W0_SHADOW_UNPROTECT(R)  ((R)&(~(1<<10)))

#define BUFSEL_BUF0         ((0<<30)|(0<<20))   // (BUFSEL_H | BUFSEL_L)
#define BUFSEL_BUF1         ((0<<30)|(1<<20))
#define BUFSEL_BUF2         ((1<<30)|(0<<20))

#define BUFAUTO_DISABLE     (0<<19)
#define BUFAUTO_ENABLE      (1<<19)

#define BURSTLEN_16WORD     (0<<9)
#define BURSTLEN_8WORD      (1<<9)
#define BURSTLEN_4WORD      (2<<9)
#define BURSTLEN_MASK       (3<<9)

#define CSC_WIDE_RANGE      (0<<26)
#define CSC_NARROW_RANGE    (3<<26)

#define BPPMODE_F(n)        (((n)&0xf)<<2)
#define OSD_SIZE(n)         ((n)&0xffffff)
#define VBASEL_F(n)         ((n)&0xffffff)


// VIDOSDxA
#define OSD_LEFTTOPX_F(n)   (((n)&0x7ff)<<11)
#define OSD_LEFTTOPY_F(n)   ((n)&0x7ff)

// VIDOSDxB
#define OSD_RIGHTBOTX_F(n)  (((n)&0x7ff)<<11)
#define OSD_RIGHTBOTY_F(n)  ((n)&0x7ff)

#define VBANK_F(n)          (((n)&0xff)<<24)
#define VBASEU_F(n)         ((n)&0xffffff)

#define OFFSIZE_F(n)        (((n)&0x1fff)<<13)
#define PAGEWIDTH_F(n)      ((n)&0x1fff)

#define ENWIN_F_DISABLE     (0<<0)
#define ENWIN_F_ENABLE      (1<<0)

typedef enum
{
    DISP_VIDOUT_RGBIF,
    DISP_VIDOUT_ITU6xx,
    DISP_VIDOUT_I80IF_LDI0,
    DISP_VIDOUT_I80IF_LDI1,
    DISP_VIDOUT_MODE_NUM
} DISP_VIDOUT_MODE;

typedef enum
{
    DISP_16BIT_RGB565_P = 0,
    DISP_18BIT_RGB666_P,
    DISP_24BIT_RGB888_P,
    DISP_18BIT_RGB666_S,
    DISP_24BIT_RGB888_S
} DISP_RGBIFOUT_MODE;

typedef struct _tRGBIFInfo
{
    DISP_RGBIFOUT_MODE RGBOutMode;
    unsigned int MIPIDSIen;
    unsigned int RGBEvenOrder;
    unsigned int RGBOddOrder;
    unsigned int uiWidth;
    unsigned int uiHeight;
    unsigned int VBPD_Value;
    unsigned int VFPD_Value;
    unsigned int VSPW_Value;
    unsigned int HBPD_Value;
    unsigned int HFPD_Value;
    unsigned int HSPW_Value;
    unsigned int VCLK_Polarity;
    unsigned int HSYNC_Polarity;
    unsigned int VSYNC_Polarity;
    unsigned int VDEN_Polarity;
    unsigned int VCLK_Source;
    unsigned int Frame_Rate;
    unsigned int VCLK_Direction;
    unsigned int VCLK_Gate;
    unsigned int RGBSkipEn;
    unsigned int RGBDummy;
    unsigned int PixelCompensation;
}tRGBIFInfo;

typedef struct _tDevInfo
{
    DISP_VIDOUT_MODE VideoOutMode;
    union
    {
        tRGBIFInfo RGBINFO;
        //tI80IFInfo I80INFO;
        //tITUIFInfo ITUINFO;
    };
} tDevInfo;

typedef enum
{
    DISP_WIN0 = 0,
    DISP_WIN1,
    DISP_WIN2,
    DISP_WIN3,
    DISP_WIN4,
    DISP_WIN_MAX
} DISP_WINDOW;

typedef enum _DISP_WINDOW_MODE
{
    DISP_WIN0_DMA = 0,
    DISP_WIN0_CAMIF0_RGB,
    DISP_WIN0_CAMIF0_YUV,
    DISP_WIN1_DMA,
    DISP_WIN1_CAMIF1_RGB,
    DISP_WIN1_CAMIF1_YUV,
    DISP_WIN1_VP_YUV,
    DISP_WIN2_DMA,
    DISP_WIN2_CAMIF2_RGB,
    DISP_WIN2_CAMIF2_YUV,
    DISP_WIN3_DMA,
    DISP_WIN4_DMA
} DISP_WINDOW_MODE;

typedef enum
{
    DISP_1BPP = 0,
    DISP_2BPP,
    DISP_4BPP,
    DISP_8BPP_PAL,
    DISP_8BPP_NOPAL,
    DISP_16BPP_565,
    DISP_16BPP_A555,
    DISP_16BPP_I555,
    DISP_18BPP_666,
    DISP_18BPP_A665,
    DISP_19BPP_A666,
    DISP_24BPP_888,
    DISP_24BPP_A887,
    DISP_32BPP_A888,
    DISP_25BPP_A888
} DISP_BPP_MODE;

typedef struct _tDispAlphaBlendingConfig
{
    unsigned int BlendingMethod;
    union
    {
        struct
        {
            unsigned int AlphaSelect;
            unsigned int Alpha0_Red;
            unsigned int Alpha0_Green;
            unsigned int Alpha0_Blue;
            unsigned int Alpha1_Red;
            unsigned int Alpha1_Green;
            unsigned int Alpha1_Blue;
        };
        struct
        {
            unsigned int ColorKeyValue;
            unsigned int CompareKeyValue;
            unsigned int Direction;
        };
    };
} tDispAlphaBlendingConfig;

typedef struct _tDispWindow0Config
{
    DISP_BPP_MODE BPP_Mode;
    unsigned int LocalPathEnable;
    unsigned int LocaPathSourceFormat;
    unsigned int BufferSelect;
    unsigned int BufferAutoControl;
    unsigned int BitSwapping;
    unsigned int ByteSwapping;
    unsigned int HalfWordSwapping;
    unsigned int WordSwapping;
    unsigned int BurstLength;
    unsigned int uiWidth;
    unsigned int uiHeight;
    unsigned int uiOffsetX;
    unsigned int uiOffsetY;
    unsigned int uiPageWidth;
} tDispWindow0Config;

typedef struct _tDispWindow12Config
{
    DISP_BPP_MODE BPP_Mode;
    unsigned int LocalPathSelect;
    unsigned int LocalPathEnable;
    unsigned int LocaPathSourceFormat;
    unsigned int BufferSelect;
    unsigned int BufferAutoControl;
    unsigned int BitSwapping;
    unsigned int ByteSwapping;
    unsigned int HalfWordSwapping;
    unsigned int WordSwapping;
    unsigned int BurstLength;
    unsigned int uiWidth;
    unsigned int uiHeight;
    unsigned int uiOffsetX;
    unsigned int uiOffsetY;
    unsigned int uiPageWidth;
    tDispAlphaBlendingConfig BlendConfig;
} tDispWindow12Config;

typedef struct _tDispWindow34Config
{
    DISP_BPP_MODE BPP_Mode;
    unsigned int BufferSelect;
    unsigned int BufferAutoControl;
    unsigned int BitSwapping;
    unsigned int ByteSwapping;
    unsigned int HalfWordSwapping;
    unsigned int WordSwapping;
    unsigned int BurstLength;
    unsigned int uiWidth;
    unsigned int uiHeight;
    unsigned int uiOffsetX;
    unsigned int uiOffsetY;
    unsigned int uiPageWidth;
    tDispAlphaBlendingConfig BlendConfig;
} tDispWindow34Config;

typedef enum
{
    DISP_WINDOW_OFF,
    DISP_WINDOW_ON
} DISP_WINDOW_ONOFF;

typedef enum
{
    DISP_ENVID_OFF,
    DISP_ENVID_ON,
    DISP_ENVID_DIRECT_OFF
} DISP_ENVID_ONOFF;

#endif /* _FIMD_H_ */