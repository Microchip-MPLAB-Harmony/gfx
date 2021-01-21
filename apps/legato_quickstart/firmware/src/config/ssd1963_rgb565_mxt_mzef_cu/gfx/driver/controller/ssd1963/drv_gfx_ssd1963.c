// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

/*******************************************************************************
  SSD1963 Display Top-Level Driver Source File

  File Name:
    drv_gfx_ssd1963.c

  Summary:
    Top level driver for SSD1963.

  Description:
    Build-time generated implementation for the SSD1963 Driver.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/




#include "definitions.h"

#include "gfx/interface/drv_gfx_disp_intf.h"
#include "drv_gfx_ssd1963_cmd_defs.h"
#include "drv_gfx_ssd1963_common.h"
#include "drv_gfx_ssd1963.h"

#include "system/time/sys_time.h"

// Number of layers
#define LAYER_COUNT     1

// Default max width/height of SSD1963 frame
#define DISPLAY_DEFAULT_WIDTH   480
#define DISPLAY_DEFAULT_HEIGHT  800

#define DISPLAY_WIDTH   480
#define DISPLAY_HEIGHT  272

#define DISP_HOR_RESOLUTION DISPLAY_WIDTH
#define DISP_VER_RESOLUTION DISPLAY_HEIGHT
#define DISP_HOR_PULSE_WIDTH 41
#define DISP_HOR_FRONT_PORCH 2
#define DISP_HOR_BACK_PORCH 2
#define DISP_VER_PULSE_WIDTH 10
#define DISP_VER_FRONT_PORCH 2
#define DISP_VER_BACK_PORCH 2

#define PIXEL_BUFFER_COLOR_MODE GFX_COLOR_MODE_RGB_565
#define SCREEN_WIDTH DISPLAY_WIDTH
#define SCREEN_HEIGHT DISPLAY_HEIGHT

#define MASTER_CLK_HZ 100000000
#define PIXEL_CLOCK_DIVIDER  6.6667
#define BACKLIGHT_PWM_FREQ_HZ 200
#define BACKLIGHT_PWMF_PARM (MASTER_CLK_HZ / (BACKLIGHT_PWM_FREQ_HZ * 256 * 256) - 1)
#define BACKLIGHT_PWM_BRIGHTNESS_PCT 100

#ifdef GFX_DISP_INTF_PIN_RESET_Clear
#define DRV_SSD1963_Reset_Assert()      GFX_DISP_INTF_PIN_RESET_Clear()
#define DRV_SSD1963_Reset_Deassert()    GFX_DISP_INTF_PIN_RESET_Set()
#else
#error "ERROR: GFX_DISP_INTF_PIN_RESET not defined. Please define in Pin Manager."
#define DRV_SSD1963_Reset_Assert()
#define DRV_SSD1963_Reset_Deassert()
#endif

#define DRV_SSD1963_NCSAssert(intf)   GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_CLEAR)

#define DRV_SSD1963_NCSDeassert(intf) GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_SET)


SSD1963_DRV drv;

static uint32_t swapCount = 0;


/* ************************************************************************** */

/**
  Function:
    static void DRV_SSD1963_DelayMS(int ms)

  Summary:
    Delay helper function.

  Description:
    This is a helper function for delay using the system tick timer.

  Parameters:
    ms      - Delay in milliseconds

  Returns:
    None.

*/

static inline void DRV_SSD1963_DelayMS(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

/**
  Function:
    static gfxResult DRV_SSD1963_Reset(void)

  Summary:
    Toggles the hardware reset to the SSD1963.

  Description:
    This function toggles the GPIO pin for asserting reset to the SSD1963.

  Parameters:
    None

  Returns:
    None

*/
static gfxResult DRV_SSD1963_Reset(void)
{
    DRV_SSD1963_Reset_Assert();
    DRV_SSD1963_DelayMS(10);
    DRV_SSD1963_Reset_Deassert();
    DRV_SSD1963_DelayMS(30);

    return GFX_SUCCESS;
}

gfxResult DRV_SSD1963_Initialize(void)
{
    drv.state = INIT;

    drv.port_priv = (void *) GFX_Disp_Intf_Open();
    if (drv.port_priv == 0)
        return LE_FAILURE;
    
    return GFX_SUCCESS;
}

static gfxResult DRV_SSD1963_Configure(SSD1963_DRV *drv)
{
    #define HT (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_RESOLUTION+DISP_HOR_FRONT_PORCH)
    #define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)

    #define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_RESOLUTION+DISP_VER_FRONT_PORCH)    
    #define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)

    uint8_t  iByte;
    uint32_t lcdc_fpr;  // Mimics LCDC_FPR in the data sheet
    uint8_t parm[8];
    
    GFX_Disp_Intf intf = (GFX_Disp_Intf) drv->port_priv;

    DRV_SSD1963_NCSDeassert(intf);
    DRV_SSD1963_DelayMS(10);

    DRV_SSD1963_NCSAssert(intf);
    DRV_SSD1963_DelayMS(10);

    //Set MN(multipliers) of PLL, VCO = crystal freq * (N+1)
    //PLL freq = VCO/M with 250MHz < VCO < 800MHz
    //The max PLL freq is 110MHz. To obtain 100MHz as the PLL freq do the following:
    parm[0] = 0x1D; // Muliplier M = 29, VCO = 12*(N+1)  = 300 MHz
    parm[1] = 0x2; // Divider N = 2,   PLL = 360/(N+1) = 100MHz
    parm[2] = 0x54; // Validate M and N values ("Effectuate" values)
    GFX_Disp_Intf_WriteCommand(intf, CMD_SET_PLL_MN);
    GFX_Disp_Intf_WriteData(intf, parm, 3); // Set PLL with OSC = 10MHz (hardware)
    DRV_SSD1963_DelayMS(10);

    parm[0] = 0x01; // enable PLL
    GFX_Disp_Intf_WriteCommand(intf, CMD_PLL_START); // Start PLL command
    GFX_Disp_Intf_WriteData(intf, parm, 1); 
    
    DRV_SSD1963_DelayMS(10);

    parm[0] = 0x03; // now, use PLL output as system clock
    GFX_Disp_Intf_WriteCommand(intf, CMD_PLL_START); // Start PLL command again
    GFX_Disp_Intf_WriteData(intf, parm, 1); 

    DRV_SSD1963_DelayMS(10); // Wait for PLL to lock

    //once PLL locked (at 100MHz), the data hold time set shortest
    GFX_Disp_Intf_WriteCommand(intf,
                            CMD_SOFT_RESET);
    DRV_SSD1963_DelayMS(10);

    lcdc_fpr = ( (uint32_t)((1UL<<20)/PIXEL_CLOCK_DIVIDER) ) - 1 ;
    parm[0] = 0xFF & (lcdc_fpr>>16);
    parm[1] = 0xFF & (lcdc_fpr>>8);
    parm[2] = iByte = 0xFF &  lcdc_fpr;
    GFX_Disp_Intf_WriteCommand(intf, CMD_SET_PCLK); // Set Pixel clock to 15 MHz
    GFX_Disp_Intf_WriteData(intf, parm, 3); 
    
    parm[0] = 0x20; // set 24-bit for TY430TF480272 4.3" panel data latch in rising edge for LSHIFT
    parm[1] = 0x00; // set Hsync+Vsync mode
    parm[2] = (DISP_HOR_RESOLUTION-1)>>8; //Set panel size
    parm[3] = (uint8_t)(DISP_HOR_RESOLUTION-1);
    parm[4] = (DISP_VER_RESOLUTION-1)>>8;
    parm[5] = (uint8_t)(DISP_VER_RESOLUTION-1);
    parm[6] = 0x00; //RGB sequence
    GFX_Disp_Intf_WriteCommand(intf, CMD_SET_PANEL_MODE);
    GFX_Disp_Intf_WriteData(intf, parm, 7); 

    //Set horizontal period
    parm[0] = (HT-1)>>8;
    parm[1] = (uint8_t)(HT-1);
    parm[2] = (HPS-1)>>8;
    parm[3] = HPS-1;
    parm[4] = DISP_HOR_PULSE_WIDTH-1;
    parm[5] = 0x00;
    parm[6] = 0x00;
    parm[7] = 0x00;
    GFX_Disp_Intf_WriteCommand(intf, CMD_SET_HOR_PERIOD);
    GFX_Disp_Intf_WriteData(intf, parm, 8); 

    //Set vertical period
    parm[0] = (VT-1)>>8;
    parm[1] = (uint8_t)(VT-1);
    parm[2] = (VSP-1)>>8;
    parm[3] = VSP-1;
    parm[4] = DISP_VER_PULSE_WIDTH-1;
    parm[5] = 0x00;
    parm[6] = 0x00;
    GFX_Disp_Intf_WriteCommand(intf, CMD_SET_VER_PERIOD);
    GFX_Disp_Intf_WriteData(intf, parm, 7); 

    //Set pixel format, i.e. the bpp
    parm[0] = 0x55; // set 16bpp, (565 format) 
    GFX_Disp_Intf_WriteCommand(intf, CMD_SET_PIXEL_FORMAT);
    GFX_Disp_Intf_WriteData(intf, parm, 1); 
    
    //Set pixel data interface
    parm[0] = 0x03; //16-bit pixel data
    GFX_Disp_Intf_WriteCommand(intf, CMD_SET_DATA_INTERFACE);
    GFX_Disp_Intf_WriteData(intf, parm, 1); 

    // Turn on display; show the image on display
    GFX_Disp_Intf_WriteCommand(intf,
                            CMD_ON_DISPLAY); 

    DRV_SSD1963_DelayMS(1);
    
    DRV_SSD1963_NCSDeassert(intf);
    
    return GFX_SUCCESS;
}

static void DRV_SSD1963_SetArea(SSD1963_DRV *drv,
                    int16_t start_x,
                    int16_t start_y,
                    int16_t end_x,
                    int16_t end_y)
{
    GFX_Disp_Intf intf = (GFX_Disp_Intf) drv->port_priv;
    uint8_t parm[4];
    
    parm[0] = start_x>>8;
    parm[1] = start_x;
    parm[2] = end_x>>8;
    parm[3] = end_x;
    GFX_Disp_Intf_WriteCommand(intf, CMD_SET_COLUMN);
    GFX_Disp_Intf_WriteData(intf, parm, 4); 
    
    parm[0] = start_y>>8;
    parm[1] = start_y;
    parm[2] = end_y>>8;
    parm[3] = end_y;
    GFX_Disp_Intf_WriteCommand(intf, CMD_SET_PAGE);
    GFX_Disp_Intf_WriteData(intf, parm, 4);
}

/**
  Function:
    static gfxResult DRV_SSD1963_BrightnessSet(uint32_t val)

  Summary:
    Driver-specific implementation of GFX HAL brightnessSet function

  Description:
    Sets the SSD1963 PWM Freq and Duty cycle

  Parameters:
    val    - The backlight brightness in %

  Returns:
    * GFX_SUCCESS       - Operation successful

*/
static gfxResult DRV_SSD1963_BrightnessSet(uint32_t brightness)
{
    uint8_t parm[5] = {
                    BACKLIGHT_PWMF_PARM,   // PWMF[7:0] = 2, PWM base freq = PLL/(256*(PWMF + 1))/256
                    (brightness * 0xff / 100), // Set duty cycle, from 0x00 (total pull-down) to 0xFF
                                // (99% pull-up , 255/256)
                    0x01,   // PWM enabled and controlled by host (mcu)
                    0x00,
                    0x00
               };

    if (brightness > 100)
        brightness = 100;

    DRV_SSD1963_NCSAssert((GFX_Disp_Intf) drv.port_priv);

    GFX_Disp_Intf_WriteCommand((GFX_Disp_Intf) drv.port_priv, CMD_SET_PWM_CONF);
    GFX_Disp_Intf_WriteData((GFX_Disp_Intf) drv.port_priv, parm, 5);

    DRV_SSD1963_NCSDeassert((GFX_Disp_Intf) drv.port_priv);
    
    return GFX_SUCCESS;
}

/**
  Function:
    static gfxResult DRV_SSD1963_Update(void)

  Summary:
    Driver-specific implementation of GFX HAL update function.

  Description:
    On GFX update, this function flushes any pending pixels to the SSD1963.

  Parameters:
    None.

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
void DRV_SSD1963_Update(void)
{
    if(drv.state == INIT)
    {
        // perform driver initialization here
        DRV_SSD1963_Reset();

        DRV_SSD1963_Configure(&drv);

        DRV_SSD1963_BrightnessSet(BACKLIGHT_PWM_BRIGHTNESS_PCT);

        drv.state = RUN;
    }
}

gfxColorMode DRV_SSD1963_GetColorMode(void)
{
    return PIXEL_BUFFER_COLOR_MODE;
}

uint32_t DRV_SSD1963_GetBufferCount(void)
{
    return 1;
}

uint32_t DRV_SSD1963_GetDisplayWidth(void)
{
    return SCREEN_WIDTH;
}

uint32_t DRV_SSD1963_GetDisplayHeight(void)
{
    return SCREEN_HEIGHT;
}

uint32_t DRV_SSD1963_GetLayerCount()
{
    return 1;
}

uint32_t DRV_SSD1963_GetActiveLayer()
{
    return 0;
}

gfxResult DRV_SSD1963_SetActiveLayer(uint32_t idx)
{
    return LE_SUCCESS;
}

gfxResult DRV_SSD1963_BlitBuffer(int32_t x,
                                int32_t y,
                                 gfxPixelBuffer* buf,
                                 gfxBlend gfx)
{
    uint16_t* ptr;

    GFX_Disp_Intf intf;
    
    if (drv.state != RUN)
        return LE_FAILURE;
    
    intf = (GFX_Disp_Intf) drv.port_priv;

    DRV_SSD1963_NCSAssert(intf);

    DRV_SSD1963_SetArea(&drv, x, y, x + buf->size.width - 1, y + buf->size.height - 1);

    GFX_Disp_Intf_WriteCommand(intf, CMD_WR_MEMSTART);
    ptr = gfxPixelBufferOffsetGet_Unsafe(buf, 0, 0);
    GFX_Disp_Intf_WriteData16(intf, (uint16_t *) ptr, buf->size.width * buf->size.height);
    
    DRV_SSD1963_NCSDeassert(intf);

    return GFX_SUCCESS;
}

void DRV_SSD1963_Swap(void)
{
    swapCount++;
}

uint32_t DRV_SSD1963_GetSwapCount(void)
{
    return swapCount;
}

