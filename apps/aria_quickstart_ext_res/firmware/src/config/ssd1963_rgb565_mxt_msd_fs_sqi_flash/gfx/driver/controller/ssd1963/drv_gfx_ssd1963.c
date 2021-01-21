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

#include "gfx/hal/inc/gfx_common.h"

#include "gfx/interface/drv_gfx_disp_intf.h"
#include "drv_gfx_ssd1963_cmd_defs.h"
#include "drv_gfx_ssd1963_common.h"
#include "drv_gfx_ssd1963.h"

#include "system/time/sys_time.h"

// Number of layers
#define LAYER_COUNT     1

// Default max width/height of SSD1963 frame
#define DISPLAY_DEFAULT_WIDTH   320
#define DISPLAY_DEFAULT_HEIGHT  480

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

#define BYTES_PER_PIXEL_BUFFER 2

#define PIXEL_BUFFER_COLOR_MODE GFX_COLOR_MODE_RGB_565

#define MASTER_CLK_HZ 100000000
#define PIXEL_CLOCK_DIVIDER  6.6667
#define BACKLIGHT_PWM_FREQ_HZ 200
#define BACKLIGHT_PWMF_PARM (MASTER_CLK_HZ / (BACKLIGHT_PWM_FREQ_HZ * 256 * 256) - 1)
#define BACKLIGHT_PWM_BRIGHTNESS_PCT 100

// Driver name
const char* DRIVER_NAME = "SSD1963";

#ifdef GFX_DISP_INTF_PIN_RESET_Clear
#define SSD1963_Reset_Assert()      GFX_DISP_INTF_PIN_RESET_Clear()
#define SSD1963_Reset_Deassert()    GFX_DISP_INTF_PIN_RESET_Set()
#else
#error "ERROR: GFX_DISP_INTF_PIN_RESET not defined. Please define in Pin Manager."
#define SSD1963_Reset_Assert()
#define SSD1963_Reset_Deassert()
#endif

#define SSD1963_NCSAssert(intf)   GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_CLEAR)

#define SSD1963_NCSDeassert(intf) GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_SET)


/* ************************************************************************** */

/**
  Function:
    static void SSD1963_DelayMS(int ms)

  Summary:
    Delay helper function.

  Description:
    This is a helper function for delay using the system tick timer.

  Parameters:
    ms      - Delay in milliseconds

  Returns:
    None.

*/

static inline void SSD1963_DelayMS(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

/**
  Function:
    static GFX_Result SSD1963_Reset(void)

  Summary:
    Toggles the hardware reset to the SSD1963.

  Description:
    This function toggles the GPIO pin for asserting reset to the SSD1963.

  Parameters:
    None

  Returns:
    None

*/
static GFX_Result SSD1963_Reset(void)
{
    SSD1963_Reset_Assert();
    SSD1963_DelayMS(10);
    SSD1963_Reset_Deassert();
    SSD1963_DelayMS(30);

    return GFX_SUCCESS;
}

static GFX_Result SSD1963_Configure(SSD1963_DRV *drv)
{
    #define HT (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_RESOLUTION+DISP_HOR_FRONT_PORCH)
    #define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)

    #define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_RESOLUTION+DISP_VER_FRONT_PORCH)    
    #define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)

    uint8_t  iByte;
    uint32_t lcdc_fpr;  // Mimics LCDC_FPR in the data sheet
    uint8_t parm[8];
    
    GFX_Disp_Intf intf = (GFX_Disp_Intf) drv->port_priv;

    SSD1963_NCSDeassert(intf);
    SSD1963_DelayMS(10);

    SSD1963_NCSAssert(intf);
    SSD1963_DelayMS(10);

    //Set MN(multipliers) of PLL, VCO = crystal freq * (N+1)
    //PLL freq = VCO/M with 250MHz < VCO < 800MHz
    //The max PLL freq is 110MHz. To obtain 100MHz as the PLL freq do the following:
    parm[0] = 0x1D; // Muliplier M = 29, VCO = 12*(N+1)  = 300 MHz
    parm[1] = 0x2; // Divider N = 2,   PLL = 360/(N+1) = 100MHz
    parm[2] = 0x54; // Validate M and N values ("Effectuate" values)
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_SET_PLL_MN, parm, 3); // Set PLL with OSC = 10MHz (hardware)
    SSD1963_DelayMS(10);

    parm[0] = 0x01; // enable PLL
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_PLL_START, parm, 1); // Start PLL command
    
    SSD1963_DelayMS(10);

    parm[0] = 0x03; // now, use PLL output as system clock
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_PLL_START, parm, 1); // Start PLL command again

    SSD1963_DelayMS(10); // Wait for PLL to lock

    //once PLL locked (at 100MHz), the data hold time set shortest
    GFX_Disp_Intf_WriteCommand(intf,
                            CMD_SOFT_RESET);
    SSD1963_DelayMS(10);

    lcdc_fpr = ( (uint32_t)((1UL<<20)/PIXEL_CLOCK_DIVIDER) ) - 1 ;
    parm[0] = 0xFF & (lcdc_fpr>>16);
    parm[1] = 0xFF & (lcdc_fpr>>8);
    parm[2] = iByte = 0xFF &  lcdc_fpr;
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_SET_PCLK, parm, 3); // Set Pixel clock to 15 MHz
    
    parm[0] = 0x20; // set 24-bit for TY430TF480272 4.3" panel data latch in rising edge for LSHIFT
    parm[1] = 0x00; // set Hsync+Vsync mode
    parm[2] = (DISP_HOR_RESOLUTION-1)>>8; //Set panel size
    parm[3] = (uint8_t)(DISP_HOR_RESOLUTION-1);
    parm[4] = (DISP_VER_RESOLUTION-1)>>8;
    parm[5] = (uint8_t)(DISP_VER_RESOLUTION-1);
    parm[6] = 0x00; //RGB sequence
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_SET_PANEL_MODE, parm, 7);

    //Set horizontal period
    parm[0] = (HT-1)>>8;
    parm[1] = (uint8_t)(HT-1);
    parm[2] = (HPS-1)>>8;
    parm[3] = HPS-1;
    parm[4] = DISP_HOR_PULSE_WIDTH-1;
    parm[5] = 0x00;
    parm[6] = 0x00;
    parm[7] = 0x00;
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_SET_HOR_PERIOD, parm, 8);
    
    //Set vertical period
    parm[0] = (VT-1)>>8;
    parm[1] = (uint8_t)(VT-1);
    parm[2] = (VSP-1)>>8;
    parm[3] = VSP-1;
    parm[4] = DISP_VER_PULSE_WIDTH-1;
    parm[5] = 0x00;
    parm[6] = 0x00;
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_SET_VER_PERIOD, parm, 7);

    //Set pixel format, i.e. the bpp
    parm[0] = 0x55; // set 16bpp, (565 format) 
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_SET_PIXEL_FORMAT, parm, 1);
    
    //Set pixel data interface
    parm[0] = 0x03; //16-bit pixel data
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_SET_DATA_INTERFACE, parm, 1);

    // Turn on display; show the image on display
    GFX_Disp_Intf_WriteCommand(intf,
                            CMD_ON_DISPLAY); 

    SSD1963_DelayMS(1);
    
    SSD1963_NCSDeassert(intf);
    
    return GFX_SUCCESS;
}

static void SetArea(SSD1963_DRV *drv,
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
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_SET_COLUMN, parm, 4);
    
    parm[0] = start_y>>8;
    parm[1] = start_y;
    parm[2] = end_y>>8;
    parm[3] = end_y;
    GFX_Disp_Intf_WriteCommandParm(intf, CMD_SET_PAGE, parm, 4);
}

/**
  Function:
    static GFX_Color SSD1963_PixelGet(const GFX_PixelBuffer *buf,
                                      const GFX_Point *pnt)

  Summary:
    HAL interface function for reading pixel value from the SSD1963 GRAM.

  Description:
    This function uses the interface-specific call to read pixel value from
    the SSD1963 GRAM.

  Parameters:
    buf     - GFX_PixelBuffer pointer where pixel value will be stored
    pnt     - GFX_Point pointer describing pixel position

  Returns:

    GFX_UNSUPPORTED   Operation is not supported
    GFX_FAILURE       Operation failed
    GFX_SUCCESS       Operation successful


*/
static GFX_Color SSD1963_PixelGet(const GFX_PixelBuffer *buf,
                                  const GFX_Point *pnt)
{
    GFX_Context *context = GFX_ActiveContext();
    SSD1963_DRV *drv;
    GFX_Disp_Intf intf;
    uint16_t pixel;

    if (!context)
        return GFX_FAILURE;

    drv = (SSD1963_DRV *) context->driver_data;
    
    intf = (GFX_Disp_Intf) drv->port_priv;
    
        
    SSD1963_NCSAssert(intf);

    SetArea(drv, pnt->x, pnt->y, DISP_HOR_RESOLUTION, DISP_VER_RESOLUTION);
    GFX_Disp_Intf_WriteCommand(intf, CMD_RD_MEMSTART);
    GFX_Disp_Intf_ReadData16(intf, (uint16_t *) &pixel, 1);
    
    SSD1963_NCSDeassert(intf);


    return pixel;
}

/**
  Function:
    static GFX_Result SSD1963_SetPixel(const GFX_PixelBuffer *buf,
                                       const GFX_Point *pnt,
                                       GFX_Color color)

  Summary:
    HAL interface function for writing pixel value to the SSD1963 GRAM.

  Description:
    This function uses the interface-specific call to write pixel value to the
    SSD1963 GRAM.


  Parameters:
    buf     - GFX_PixelBuffer pointer
    pnt     - GFX_Point pointer describing pixel position
    color   - pixel color value (RGB565)

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
static GFX_Result SSD1963_PixelSet(const GFX_PixelBuffer *buf,
                            const GFX_Point *pnt,
                            GFX_Color color)
{
    GFX_Context *context = GFX_ActiveContext();
    SSD1963_DRV *drv;
    GFX_Disp_Intf intf;
    

    if (!context)
        return GFX_FAILURE;

    drv = (SSD1963_DRV *) context->driver_data;
    
    intf = (GFX_Disp_Intf) drv->port_priv;


    SSD1963_NCSAssert(intf);

    SetArea(drv, pnt->x, pnt->y, DISP_HOR_RESOLUTION, DISP_VER_RESOLUTION);
    GFX_Disp_Intf_WriteCommand(intf, CMD_WR_MEMSTART);
    GFX_Disp_Intf_WriteData16(intf, (uint16_t *) &color, 1);

    SSD1963_NCSDeassert(intf);

    return GFX_SUCCESS;
}

static GFX_Result SSD1963_DrawDirectBlit(const GFX_PixelBuffer* source,
                             const GFX_Rect* srcRect,
                             const GFX_Point* drawPoint,
                             const GFX_DrawState* state)
{
    int16_t  left, right, top, bottom, row;
    //uint32_t size;
    GFX_Context* pContext = GFX_ActiveContext();
    GFX_Point srcPnt, destPnt;
    GFX_Buffer *src;

    SSD1963_DRV *drv;
    GFX_Disp_Intf intf;
    
    drv = (SSD1963_DRV *) pContext->driver_data;
    intf = (GFX_Disp_Intf) drv->port_priv;
    
    srcPnt.x = srcRect->x;
    srcPnt.y = srcRect->y;
        
    destPnt.x = drawPoint->x;
    destPnt.y = drawPoint->y;
    
    // size for one row at a time
    //size = srcRect->width * GFX_ColorInfo[source->mode].size;
    
    SSD1963_NCSAssert(intf);

    // can't do bulk copy, go row by row instead        
    for(row = 0; row < srcRect->height; row++)
    {
        src = GFX_PixelBufferOffsetGet(source, &srcPnt);
    
        left   = destPnt.x;
        right  = left + srcRect->width;
        top    = destPnt.y;
        bottom = top + 1;

        SetArea(drv, left,top,right,bottom);

        GFX_Disp_Intf_WriteCommand(intf, CMD_WR_MEMSTART);

        GFX_Disp_Intf_WriteData16(intf, (uint16_t *)src, srcRect->width);

        srcPnt.y++;
        destPnt.y++;
    }

    SSD1963_NCSDeassert(intf);
    
    return GFX_SUCCESS;
}

GFX_Result SSD1963_fillRect(const GFX_Rect* pRect,
                            const GFX_DrawState* pState)
{
    int16_t      x, y, left, right, top, bottom;
    GFX_Context* pContext = GFX_ActiveContext();
    GFX_Layer*   pLayer  = pContext->layer.active;
    GFX_Rect     clipRect;
    GFX_Color    color;
    
    SSD1963_DRV *drv;
    GFX_Disp_Intf intf;
    
    drv = (SSD1963_DRV *) pContext->driver_data;
    intf = (GFX_Disp_Intf) drv->port_priv;

    // a basic fill is an optimal case for this driver
    // everything else should go through software pixel pipeline
    if(pContext->orientation != GFX_ORIENTATION_0 ||
       pContext->mirrored != GFX_FALSE)
        return cpuDrawRect_Fill(pRect, pState);

    // clip against the physical pLayer bounds
    if(GFX_PixelBufferClipRect(&pLayer->buffers[pLayer->buffer_write_idx].pb,
                               pRect,
                               &clipRect) == GFX_FAILURE)
    { // Input rectangle doesn't intersect with clip rectangle, we're done!
        return GFX_FAILURE;
    }

  #if GFX_BOUNDS_CLIPPING_ENABLED
    // clip against the global clipping rectangle
    if(pState->clipEnable == GFX_TRUE)
    {
        if(GFX_RectIntersects(pRect, &pState->clipRect) == GFX_FALSE)
            return GFX_SUCCESS;

        GFX_RectClip(pRect, &pState->clipRect, &clipRect);
    }
    else
    { // Clipping not on
        clipRect = *pRect;
    }
  #else
    clipRect = *pRect; // Clipping disabled.
  #endif

    left   = clipRect.x;
    right  = left + clipRect.width;
    top    = clipRect.y;
    bottom = top + clipRect.height;
    color  = pState->color;


    SSD1963_NCSAssert(intf);

    SetArea(drv, left,top,right,bottom);

    GFX_Disp_Intf_WriteCommand(intf, CMD_WR_MEMSTART);
    for(y=top; y<bottom+1; y++)
    {
        for(x=left; x<right+1; x++){
            GFX_Disp_Intf_WriteData16(intf, (uint16_t *) &color, 1);
        }
    }
    

    SSD1963_NCSDeassert(intf);

    return(GFX_SUCCESS);
}

/**
  Function:
    static GFX_Result SSD1963_BrightnessRangeGet(uint32_t *low, uint32_t *high)

  Summary:
    Driver-specific implementation of GFX HAL brightnessRangeGet function.

  Description:
    Stub function, operation not supported in driver.

  Returns:
     * GFX_UNSUPPORTED      - Operation not supported

*/
static GFX_Result SSD1963_BrightnessRangeGet(uint32_t *low, uint32_t *high)
{
    *low = 0;
    *high = 100;
    
    return GFX_SUCCESS;
}

/**
  Function:
    static GFX_Result SSD1963_BrightnessSet(uint32_t val)

  Summary:
    Driver-specific implementation of GFX HAL brightnessSet function

  Description:
    Sets the SSD1963 PWM Freq and Duty cycle

  Parameters:
    val    - The backlight brightness in %

  Returns:
    * GFX_SUCCESS       - Operation successful

*/
static GFX_Result SSD1963_BrightnessSet(uint32_t brightness)
{
    GFX_Context *context = GFX_ActiveContext();
    SSD1963_DRV *drv;
    
    if (!context)
        return GFX_FAILURE;

    drv = (SSD1963_DRV *) context->driver_data;
    
    uint8_t parm[5] = {
                        BACKLIGHT_PWMF_PARM,   // PWMF[7:0] = 2, PWM base freq = PLL/(256*(PWMF + 1))/256
                        (brightness * 0xff / 100), // Set duty cycle, from 0x00 (total pull-down) to 0xFF
                                // (99% pull-up , 255/256)
                        0x01,   // PWM enabled and controlled by host (mcu)
                        0x00,
                        0x00
                      };
    
    SSD1963_NCSAssert((GFX_Disp_Intf) drv->port_priv);

    GFX_Disp_Intf_WriteCommandParm((GFX_Disp_Intf) drv->port_priv,
                                    CMD_SET_PWM_CONF, parm, 5);

    SSD1963_NCSDeassert((GFX_Disp_Intf) drv->port_priv);
    
    return GFX_SUCCESS;
}

/**
  Function:
    GFX_Result driverSSD1963InfoGet(GFX_DriverInfo *info)

  Summary:
    HAL interface function for providing driver information.

  Description:
    This function provides driver information to the HAL.

  Parameters:
    info    - Pointer to driver information

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
GFX_Result driverSSD1963InfoGet(GFX_DriverInfo *info)
{
    if(info == NULL)
        return GFX_FAILURE;

    // populate info struct
    strcpy(info->name, DRIVER_NAME);
    info->color_formats = GFX_COLOR_MASK_RGB_565;
    info->layer_count = LAYER_COUNT;

    return GFX_SUCCESS;
}

/**
  Function:
    static GFX_Result SSD1963_Update(void)

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
static GFX_Result SSD1963_Update(void)
{
    GFX_Context *context = GFX_ActiveContext();
    GFX_Result returnValue = GFX_SUCCESS;
    SSD1963_DRV *drv;

    if(context == NULL)
        return GFX_FAILURE;

    drv = (SSD1963_DRV *) context->driver_data;

    if(drv->state == INIT)
    {
        // perform driver initialization here
        SSD1963_Reset();

        returnValue = SSD1963_Configure(drv);

        SSD1963_BrightnessSet(BACKLIGHT_PWM_BRIGHTNESS_PCT);

        drv->state = RUN;
    }

    return returnValue;
}

/**
  Function:
    static void SSD1963_Destroy(GFX_Context *context)

  Summary:
    Driver-specific implementation of GFX HAL destroy function.

  Description:
    This function closes the SSD1963 interface and frees up the data structures
    allocated by the driver.

  Parameters:
    context     - GFX context

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
static void SSD1963_Destroy(GFX_Context *context)
{
    // driver specific shutdown tasks
    SSD1963_DRV *drv = NULL;
    GFX_Disp_Intf intf = 0;

    if (context != NULL && 
        context->driver_data != NULL)
    {
        drv = (SSD1963_DRV *) context->driver_data;
        intf = (GFX_Disp_Intf) drv->port_priv;
        
        //Close the interface    
        SSD1963_NCSDeassert(intf);
        
        GFX_Disp_Intf_Close(intf);
        drv->port_priv = NULL;

        context->memory.free(context->driver_data);
        context->driver_data = GFX_NULL;

        // general default shutdown
        defDestroy(context);
    }
}

/**
  Function:
    static void SSD1963_Initialize(GFX_Context *context)

  Summary:
    Driver-specific implementation of GFX HAL initialize function.

  Description:
    This function creates driver-specific data structures, initializes data
    needed by HAL, and opens the port-specific interface to the SSD1963 device.

  Parameters:
    context     - GFX context

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
static GFX_Result SSD1963_Initialize(GFX_Context *context)
{
    SSD1963_DRV *drv;

    drv = (void *) context->memory.calloc(1, sizeof(SSD1963_DRV));
    if (!drv)
        return GFX_FAILURE;

    drv->gfx = context;
    drv->state = INIT;

    context->layer.layers[0].enabled = GFX_TRUE;
    context->layer.layers[0].visible = GFX_TRUE;

    context->layer.layers[0].vsync = GFX_FALSE;
    context->layer.layers[0].swap = GFX_FALSE;

    context->layer.layers[0].rect.local.x = 0;
    context->layer.layers[0].rect.local.y = 0;
    context->layer.layers[0].rect.local.width = context->display_info->rect.width;
    context->layer.layers[0].rect.local.height = context->display_info->rect.height;

    context->layer.layers[0].rect.display = context->layer.layers[0].rect.local;

    context->layer.layers[0].alphaEnable = GFX_FALSE;
    context->layer.layers[0].alphaAmount = 255;

    context->layer.layers[0].maskEnable = GFX_FALSE;
    context->layer.layers[0].maskColor = 0;

    context->layer.layers[0].buffer_count = 1;
    context->layer.layers[0].buffer_read_idx = 0;
    context->layer.layers[0].buffer_write_idx = 0;

    GFX_PixelBufferCreate(DISPLAY_WIDTH,
                          DISPLAY_HEIGHT,
                          PIXEL_BUFFER_COLOR_MODE,
                          NULL,
                          &context->layer.layers[0].buffers[0].pb);

    context->layer.layers[0].buffers[0].state = GFX_BS_MANAGED;

    //Open interface to SSD1963 controller
    drv->port_priv = (void *) GFX_Disp_Intf_Open(drv->gfx);
    if (drv->port_priv == 0)
    {
        if (drv)
            context->memory.free(drv);

        return GFX_FAILURE;
    }

    context->driver_data = (void *) drv;

    // general default initialization
    if(defInitialize(context) == GFX_FAILURE)
    {
        if (drv)
            context->memory.free(drv);

        return GFX_FAILURE;
    }

    return GFX_SUCCESS;
}

/**
  Function:
    static GFX_Result SSD1963_vsyncSet(GFX_Bool enable)

  Summary:
    Driver-specific implementation of GFX HAL layerVsyncSet function.

  Description:
     Stub function, operation not supported in driver.

  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_vsyncSet(GFX_Bool enable)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_vsyncCallbackSet(GFX_SyncCallback_FnPtr cb)

  Summary:
    Driver-specific implementation of GFX HAL vsyncCallbackSet function.

  Description:
    Stub function, operation not supported in driver.


  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_vsyncCallbackSet(GFX_SyncCallback_FnPtr cb)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_hsyncCallbackSet(GFX_SyncCallback_FnPtr cb)

  Summary:
    Driver-specific implementation of GFX HAL hsyncCallbackSet function.

  Description:
    Stub function, operation not supported in driver.


  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_hsyncCallbackSet(GFX_SyncCallback_FnPtr cb)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    sstatic GFX_Result SSD1963_layerActiveSet(uint32_t idx)

  Summary:
    Driver-specific implementation of GFX HAL layerActiveSet function.

  Description:
    Stub function, operation not supported in driver.

  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_layerActiveSet(uint32_t idx)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerEnabledSet(GFX_Bool val)

  Summary:
    Driver-specific implementation of GFX HAL layerEnabledSet function.

  Description:
    Stub function, operation not supported in driver.

  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_layerEnabledSet(GFX_Bool val)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerPositionSet(int32_t x, int32_t y)

  Summary:
    Driver-specific implementation of GFX HAL layerPositionSet function.

  Description:
    Stub function, operation not supported in driver.

  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_layerPositionSet(int32_t x, int32_t y)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerSizeSet(int32_t width, int32_t height)

  Summary:
    Driver-specific implementation of GFX HAL layerSizeSet function.

  Description:
     Stub function, operation not supported in driver.

  Returns:
    * GFX_UNSUPPORTED     Operation not supported

*/
static GFX_Result SSD1963_layerSizeSet(int32_t width, int32_t height)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerBufferCountSet(uint32_t count)

  Summary:
    Driver-specific implementation of GFX HAL layerBufferCountSet function.

  Description:
    Stub function, operation not supported in driver.

  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_layerBufferCountSet(uint32_t count)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerBufferAddressSet(uint32_t idx,
                                                    GFX_Buffer address)

  Summary:
    Driver-specific implementation of GFX HAL layerBufferAddressSet function.

  Description:
    Stub function, operation not supported in driver.

  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_layerBufferAddressSet(uint32_t idx, GFX_Buffer address)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerBufferCoherentSet(uint32_t idx,
                                                     GFX_Bool coherent)

  Summary:
    Driver-specific implementation of GFX HAL layerBufferCoherentSet function

  Description:
    Stub function, operation not supported in driver.

  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_layerBufferCoherentSet(uint32_t idx, GFX_Bool coherent)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerBufferAllocate(uint32_t idx)

  Summary:
    Driver-specific implementation of GFX HAL layerBufferAllocate function.

  Description:
    Stub function, operation not supported in driver.


  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_layerBufferAllocate(uint32_t idx)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerBufferFree(uint32_t idx)

  Summary:
    Driver-specific implementation of GFX HAL layerBufferFree function.

  Description:
    Stub function, operation not supported in driver.

  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_layerBufferFree(uint32_t idx)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerVisibleSet(GFX_Bool val)

  Summary:
    Driver-specific implementation of GFX HAL layerVisibleSet function.

  Description:
    Stub function, operation not supported in driver.


  Returns:
    * GFX_UNSUPPORTED       - Operation not supported

*/
static GFX_Result SSD1963_layerVisibleSet(GFX_Bool val)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    static GFX_Result SSD1963_layerAlphaEnableSet(GFX_Bool enable)

  Summary:
    Driver-specific implementation of GFX HAL layerAlphaEnableSet function.

  Description:
     Stub function, operation not supported in driver.


  Returns:

      GFX_UNSUPPORTED     Operation not supported

*/
static GFX_Result SSD1963_layerAlphaEnableSet(GFX_Bool enable, GFX_Bool wait)
{
    return GFX_UNSUPPORTED;
}

/**
  Function:
    GFX_Result driverSSD1963ContextInitialize(GFX_Context *context)

  Summary:
    Initializes the driver context.

  Description:
    Initializes the driver context.

  Returns:
    * GFX_SUCCESS       - Operation successful

*/
GFX_Result driverSSD1963ContextInitialize(GFX_Context *context)
{
    // set driver-specific data initialization function address
    context->hal.initialize = &SSD1963_Initialize;

    // override essential hal functions
    context->hal.destroy = &SSD1963_Destroy;
    context->hal.update = &SSD1963_Update;

    // set driver-specific function implementations
    context->hal.brightnessRangeGet = &SSD1963_BrightnessRangeGet;
    context->hal.brightnessSet = &SSD1963_BrightnessSet;
    context->hal.layerVsyncSet = &SSD1963_vsyncSet;
    context->hal.vsyncCallbackSet = &SSD1963_vsyncCallbackSet;
    context->hal.hsyncCallbackSet = &SSD1963_hsyncCallbackSet;
    context->hal.layerActiveSet = &SSD1963_layerActiveSet;
    context->hal.layerEnabledSet = &SSD1963_layerEnabledSet;
    context->hal.layerPositionSet = &SSD1963_layerPositionSet;
    context->hal.layerSizeSet = &SSD1963_layerSizeSet;
    context->hal.layerBufferCountSet = &SSD1963_layerBufferCountSet;
    context->hal.layerBufferAddressSet = &SSD1963_layerBufferAddressSet;
    context->hal.layerBufferCoherentSet = &SSD1963_layerBufferCoherentSet;
    context->hal.layerBufferAllocate = &SSD1963_layerBufferAllocate;
    context->hal.layerBufferFree = &SSD1963_layerBufferFree;
    context->hal.layerVisibleSet = &SSD1963_layerVisibleSet;
    context->hal.layerAlphaEnableSet = &SSD1963_layerAlphaEnableSet;

    context->hal.drawPipeline[GFX_PIPELINE_GCU].pixelSet = &SSD1963_PixelSet;
    context->hal.drawPipeline[GFX_PIPELINE_GCU].pixelGet = &SSD1963_PixelGet;

    context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].pixelSet = &SSD1963_PixelSet;
    context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].pixelGet = &SSD1963_PixelGet;
    
    context->hal.drawPipeline[GFX_PIPELINE_GCU].drawRect[GFX_DRAW_FILL][GFX_ANTIALIAS_OFF] = &SSD1963_fillRect;
    context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].drawRect[GFX_DRAW_FILL][GFX_ANTIALIAS_OFF] = &SSD1963_fillRect;

    context->hal.drawPipeline[GFX_PIPELINE_GCU].drawDirectBlit = &SSD1963_DrawDirectBlit;
    context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].drawDirectBlit = &SSD1963_DrawDirectBlit;

    return GFX_SUCCESS;
}

/**** End Hardware Abstraction Interfaces ****/
