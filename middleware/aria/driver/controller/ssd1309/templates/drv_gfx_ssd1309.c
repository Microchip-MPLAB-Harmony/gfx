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
  SSD1309 Display Top-Level Driver Source File

  File Name:
    drv_gfx_ssd1309.c

  Summary:
    Top level driver for SSD1309.

  Description:
    Build-time generated implementation for the SSD1309 Driver.

    Created with MPLAB Harmony Version 3.4
*******************************************************************************/

<#if HALConnected == true>
<#assign Val_Width = gfx_hal.DisplayWidth>
<#assign Val_Height = gfx_hal.DisplayHeight>
<#else>
<#assign Val_Width = DisplayWidth>
<#assign Val_Height = DisplayHeight>
</#if>

#include "definitions.h"

#include "gfx/hal/inc/gfx_common.h"

#include "drv_gfx_ssd1309_cmd_defs.h"
#include "drv_gfx_ssd1309_common.h"
#include "drv_gfx_ssd1309.h"

#include "gfx/interface/drv_gfx_disp_intf.h"

// Number of layers
#define LAYER_COUNT     1

// Default max width/height of SSD1309 frame
#define DISPLAY_DEFAULT_WIDTH   128
#define DISPLAY_DEFAULT_HEIGHT  64

#define DISPLAY_WIDTH   ${Val_Width}
#define DISPLAY_HEIGHT  ${Val_Height}

// Driver name
const char* DRIVER_NAME = "SSD1309";

#define SSD1309_SPI_SS_Assert(intf)     GFX_Disp_Intf_PinControl(intf, \
                                            GFX_DISP_INTF_PIN_CS, \
                                            GFX_DISP_INTF_PIN_CLEAR)
#define SSD1309_SPI_SS_Deassert(intf)   GFX_Disp_Intf_PinControl(intf, \
                                            GFX_DISP_INTF_PIN_CS, \
                                            GFX_DISP_INTF_PIN_SET)
#define SSD1309_SPI_DCX_Command(intf)   GFX_Disp_Intf_PinControl(intf, \
                                            GFX_DISP_INTF_PIN_RSDC, \
                                            GFX_DISP_INTF_PIN_CLEAR)
#define SSD1309_SPI_DCX_Data(intf)      GFX_Disp_Intf_PinControl(intf, \
                                            GFX_DISP_INTF_PIN_RSDC, \
                                            GFX_DISP_INTF_PIN_SET)

#define SSD1309_Reset_Assert()      GFX_DISP_INTF_PIN_RESET_Clear()
#define SSD1309_Reset_Deassert()    GFX_DISP_INTF_PIN_RESET_Set()

#define SSD1309_PIXELS_PER_BYTE 8
#define LCD_FRAMEBUFFER_PAGES (DISPLAY_HEIGHT / SSD1309_PIXELS_PER_BYTE)
#define LCD_FRAMEBUFFER_SIZE ((DISPLAY_WIDTH * DISPLAY_HEIGHT) / SSD1309_PIXELS_PER_BYTE)

static uint8_t framebuffer[LCD_FRAMEBUFFER_SIZE] = {0};

/* ************************************************************************** */

/**
  Function:
    static void SSD1309_DelayMS(int ms)

  Summary:
    Delay helper function.

  Description:
    This is a helper function for delay using the system tick timer.

  Parameters:
    ms      - Delay in milliseconds

  Returns:
    None.

*/

static inline void SSD1309_DelayMS(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

static GFX_Result SSD1309_WriteCommandParm(SSD1309_DRV *drv, 
                                    uint8_t cmd, 
                                    uint8_t parm)
{
    GFX_Result returnValue = GFX_FAILURE;
    GFX_Disp_Intf intf;
    uint8_t parms[2] = {cmd, parm};

    if (!drv)
        return GFX_FAILURE;

    intf = (GFX_Disp_Intf) drv->port_priv;

    SSD1309_SPI_SS_Assert(intf);
    
    returnValue = GFX_Disp_Intf_Write(intf, parms, 2);
    
    SSD1309_SPI_SS_Deassert(intf);

    return returnValue;
}

static GFX_Result SSD1309_WriteCommand(SSD1309_DRV *drv, 
                                uint8_t cmd)
{
    GFX_Result returnValue = GFX_FAILURE;
    GFX_Disp_Intf intf;

    if (!drv)
        return GFX_FAILURE;

    intf = (GFX_Disp_Intf) drv->port_priv;

    SSD1309_SPI_SS_Assert(intf);
    
    returnValue = GFX_Disp_Intf_WriteCommand(intf, cmd);
    
    SSD1309_SPI_SS_Deassert(intf);

    return returnValue;
}

GFX_Result SSD1309_WriteCommandParms(SSD1309_DRV *drv, 
                                    uint8_t cmd, 
                                    uint8_t * parms,
                                    uint32_t num_parms)
{
    GFX_Result returnValue = GFX_FAILURE;
    GFX_Disp_Intf intf;

    if (!drv)
        return GFX_FAILURE;

    intf = (GFX_Disp_Intf) drv->port_priv;

    SSD1309_SPI_SS_Assert(intf);
    
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_Write(intf, parms, num_parms);
    
    SSD1309_SPI_SS_Deassert(intf);

    return returnValue;
}

static inline uint8_t SSD1309_LCD_Framebuffer_Get_Byte(uint8_t * fb,
                                         uint8_t page,
                                         uint8_t column)
{
	return *(fb + (page * DISPLAY_WIDTH) + column);
}

static inline void SSD1309_LCD_Framebuffer_Set_Byte(uint8_t * fb,
                                      uint8_t page,
                                      uint8_t column,
                                      uint8_t data)
{
	*(fb + (page * DISPLAY_WIDTH) + column) = data;
}


static inline void SSD1309_SetPage(GFX_Disp_Intf intf, uint8_t page)
{
    GFX_Disp_Intf_WriteCommand(intf, SSD1309_CMD_SET_PAGE_START_ADDRESS(page & 0x0F));
}

static inline void SSD1309_SetColumn(GFX_Disp_Intf intf, uint8_t column)
{
    column &= 0x7f;
    
    GFX_Disp_Intf_WriteCommand(intf, SSD1309_CMD_COL_ADD_SET_MSB(column >> 4));
    GFX_Disp_Intf_WriteCommand(intf, SSD1309_CMD_COL_ADD_SET_LSB(column & 0x0f));
}

GFX_Result SSD1309_HorizontalScrollLeft(void)
{
    GFX_Context *context = GFX_ActiveContext();
    SSD1309_DRV *drv;
    uint8_t parms[7] = {0, //Dummy byte
                        2, //start page address
                        4, //time interval
                        5, //end page
                        0, //dummy byte
                        32, //start column
                        0x7f}; //end column

    if(context == NULL)
        return GFX_FAILURE;

    drv = (SSD1309_DRV *) context->driver_data;
    
    SSD1309_WriteCommandParms(drv, SSD1309_CMD_SCROLL_H_LEFT, parms, 7);
    SSD1309_WriteCommand(drv, SSD1309_CMD_ACTIVATE_SCROLL);
    
    return GFX_SUCCESS;
}

GFX_Result SSD1309_HorizontalScrollRight(void)
{
    GFX_Context *context = GFX_ActiveContext();
    SSD1309_DRV *drv;
    uint8_t parms[7] = {0, //Dummy byte
                        2, //start page address
                        4, //time interval
                        5, //end page
                        0, //dummy byte
                        32, //start column
                        0x7f}; //end column

    if(context == NULL)
        return GFX_FAILURE;

    drv = (SSD1309_DRV *) context->driver_data;
    
    SSD1309_WriteCommandParms(drv, SSD1309_CMD_SCROLL_H_RIGHT, parms, 7);
    SSD1309_WriteCommand(drv, SSD1309_CMD_ACTIVATE_SCROLL);
    
    return GFX_SUCCESS;
}

GFX_Result SSD1309_HorizontalScrollStop(void)
{
    GFX_Context *context = GFX_ActiveContext();
    SSD1309_DRV *drv;

    if(context == NULL)
        return GFX_FAILURE;

    drv = (SSD1309_DRV *) context->driver_data;
    
    SSD1309_WriteCommand(drv, SSD1309_CMD_DEACTIVATE_SCROLL);
    
    return GFX_SUCCESS;
}

uint8_t SSD1309_GetPixel(SSD1309_DRV *drv, 
                            uint8_t x, 
                            uint8_t y)
{
    GFX_Disp_Intf intf;
    uint8_t page, pixel_mask, pixel_value;

    if (!drv ||
        x > DISPLAY_WIDTH - 1 ||
        y > DISPLAY_HEIGHT - 1)
        return GFX_FAILURE;

    intf = (GFX_Disp_Intf) drv->port_priv;
    
    //determine the page and column based on pixel coordinates
    page = y / SSD1309_PIXELS_PER_BYTE;

    pixel_mask = (1 << (y - (page * 8)));
    SSD1309_SPI_SS_Assert(intf);
    
    pixel_value = SSD1309_LCD_Framebuffer_Get_Byte(framebuffer, page, x);

    return (pixel_value & pixel_mask);
}

GFX_Result SSD1309_SetPixel(SSD1309_DRV *drv, 
                            uint8_t x, 
                            uint8_t y,
                            uint32_t color)
{
    GFX_Result returnValue = GFX_FAILURE;
    GFX_Disp_Intf intf;
    uint8_t page, pixel_mask, pixel_value;

    if (!drv ||
        x > DISPLAY_WIDTH - 1 ||
        y > DISPLAY_HEIGHT - 1)
        return GFX_FAILURE;

    intf = (GFX_Disp_Intf) drv->port_priv;
    
    //determine the page and column based on pixel coordinates
    page = y / SSD1309_PIXELS_PER_BYTE;

    pixel_mask = (1 << (y - (page * 8)));
    SSD1309_SPI_SS_Assert(intf);
    
    //Read the pixel data from frame buffer memory
    pixel_value = SSD1309_LCD_Framebuffer_Get_Byte(framebuffer, page, x);

    if (color == 0)
    {
        pixel_value &= ~pixel_mask;
    }
    else
    {
        pixel_value |= pixel_mask;
    }
    
    SSD1309_LCD_Framebuffer_Set_Byte(framebuffer, page, x, pixel_value);
    SSD1309_SetPage(intf, page);
    SSD1309_SetColumn(intf, x);
    GFX_Disp_Intf_WriteData(intf, &pixel_value, 1);
    
    SSD1309_SPI_SS_Deassert(intf);

    return returnValue;
}

/**
  Function:
    static GFX_Result SSD1309_Reset(void)

  Summary:
    Toggles the hardware reset to the SSD1309.

  Description:
    This function toggles the GPIO pin for asserting reset to the SSD1309.

  Parameters:
    None

  Returns:
    None

*/
static GFX_Result SSD1309_Reset(void)
{
    SSD1309_Reset_Deassert();
    SSD1309_DelayMS(10);
    SSD1309_Reset_Assert();
    SSD1309_DelayMS(10);
    SSD1309_Reset_Deassert();
    SSD1309_DelayMS(30);

    return GFX_SUCCESS;
}

/**
  Function:
    static GFX_Color SSD1309_PixelGet(const GFX_PixelBuffer *buf,
                                      const GFX_Point *pnt)

  Summary:
    HAL interface function for reading pixel value from the framebuffer or
    SSD1309 GRAM (if supported)

  Description:

  Parameters:
    buf     - GFX_PixelBuffer pointer where pixel value will be stored
    pnt     - GFX_Point pointer describing pixel position

  Returns:

    GFX_UNSUPPORTED   Operation is not supported
    GFX_FAILURE       Operation failed
    GFX_SUCCESS       Operation successful


*/
static GFX_Color SSD1309_PixelGet(const GFX_PixelBuffer *buf,
                                  const GFX_Point *pnt)
{
    GFX_Context *context = GFX_ActiveContext();
    SSD1309_DRV *drv;
    GFX_Color pixel;

    if (!context)
        return GFX_FAILURE;

    drv = (SSD1309_DRV *) context->driver_data;

    pixel = SSD1309_GetPixel(drv, pnt->x, pnt->y);
    
    pixel = (pixel > 0) ? 0xffffffff : 0;

    return pixel;
}

/**
  Function:
    static GFX_Result SSD1309_SetPixel(const GFX_PixelBuffer *buf,
                                       const GFX_Point *pnt,
                                       GFX_Color color)

  Summary:
    HAL interface function for writing pixel value to the SSD1309 GRAM.

  Description:
    This function uses the interface-specific call to write pixel value to the
    SSD1309 GRAM.


  Parameters:
    buf     - GFX_PixelBuffer pointer
    pnt     - GFX_Point pointer describing pixel position
    color   - pixel color value (RGB565)

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
static GFX_Result SSD1309_PixelSet(const GFX_PixelBuffer *buf,
                            const GFX_Point *pnt,
                            GFX_Color color)
{
    GFX_Context *context = GFX_ActiveContext();
    GFX_Result returnValue = GFX_SUCCESS;
    SSD1309_DRV *drv;

    if (!context)
        return GFX_FAILURE;

    drv = (SSD1309_DRV *) context->driver_data;

    SSD1309_SetPixel(drv, pnt->x, pnt->y, color);

    return returnValue;
}


/**
  Function:
    GFX_Result driverSSD1309InfoGet(GFX_DriverInfo *info)

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
GFX_Result driverSSD1309InfoGet(GFX_DriverInfo *info)
{
    if(info == NULL)
        return GFX_FAILURE;

    // populate info struct
    strcpy(info->name, DRIVER_NAME);
    info->color_formats = GFX_COLOR_MASK_GS_8;
    info->layer_count = LAYER_COUNT;

    return GFX_SUCCESS;
}

/**
  Function:
    static GFX_Result SSD1309_Init(SSD1309_DRV *drv,
                                   SSD1309_CMD_PARAM *initVals,
                                   int numVals)

  Summary:
    Initializes the SSD1309 registers.

  Description:
    This function uses the register/parameter table initVals to program the
    SSD1309 registers to their initial values. It also uses the display size
    information from gfx context to set the area of the frame memory that the
    MCU can access.


  Parameters:
    drv         - SSD1309 driver handle
    initVals    - Table of command/parameters for initialization
    numVals     - Number of entries in initVals

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
static GFX_Result SSD1309_Init(SSD1309_DRV *drv)
{
    SSD1309_WriteCommandParm(drv, SSD1309_CMD_SET_COMMAND_LOCK, 0x12);
    SSD1309_WriteCommand(drv, SSD1309_CMD_SET_DISPLAY_OFF);
    
    SSD1309_WriteCommandParm(drv, SSD1309_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO, 0x${OscillatorFreq});
    SSD1309_WriteCommandParm(drv, SSD1309_CMD_SET_MULTIPLEX_RATIO, ${MultiplexRatio});
    SSD1309_WriteCommandParm(drv, SSD1309_CMD_SET_DISPLAY_OFFSET, ${DisplayOffset});
    SSD1309_WriteCommand(drv, SSD1309_CMD_SET_DISPLAY_START_LINE(${DisplayStartLine}));
<#if SegmentRemap == "Col0">
    SSD1309_WriteCommand(drv, SSD1309_CMD_SET_SEGMENT_RE_MAP_COL0_SEG0);
<#else>
    SSD1309_WriteCommand(drv, SSD1309_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0);
</#if>   
<#if OutputScanDirection == "Down">
    SSD1309_WriteCommand(drv, SSD1309_CMD_SET_COM_OUTPUT_SCAN_DOWN);
<#else>
    SSD1309_WriteCommand(drv, SSD1309_CMD_SET_COM_OUTPUT_SCAN_UP);
</#if>

    SSD1309_WriteCommandParm(drv, SSD1309_CMD_SET_COM_PINS, 0x${ComPinConfiguration});
    SSD1309_WriteCommandParm(drv, SSD1309_CMD_SET_CONTRAST_CONTROL_FOR_BANK0, 0x${ContrastControlBank0});
    SSD1309_WriteCommandParm(drv, SSD1309_CMD_SET_PRE_CHARGE_PERIOD, 0x${PreChargePeriod});
    SSD1309_WriteCommandParm(drv, SSD1309_CMD_SET_VCOMH_DESELECT_LEVEL, 0x${VcomDeselectLevel});
    SSD1309_WriteCommand(drv, SSD1309_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON); //0xA4
<#if DisplayMode == "Normal">
    SSD1309_WriteCommand(drv, SSD1309_CMD_SET_NORMAL_DISPLAY);
<#else>
    SSD1309_WriteCommand(drv, SSD1309_CMD_SET_INVERSE_DISPLAY);
</#if> 
    //Display ON
    SSD1309_WriteCommand(drv, SSD1309_CMD_SET_DISPLAY_ON); //0xAF

    SSD1309_DelayMS(100);

    return GFX_SUCCESS;
}

/**
  Function:
    static GFX_Result SSD1309_Update(void)

  Summary:
    Driver-specific implementation of GFX HAL update function.

  Description:
    On GFX update, this function flushes any pending pixels to the SSD1309.

  Parameters:
    None.

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
static GFX_Result SSD1309_Update(void)
{
    GFX_Context *context = GFX_ActiveContext();
    GFX_Result returnValue = GFX_SUCCESS;
    SSD1309_DRV *drv;

    if(context == NULL)
        return GFX_FAILURE;

    drv = (SSD1309_DRV *) context->driver_data;

    if(drv->state == INIT)
    {
        // perform driver initialization here
        SSD1309_Reset();

        returnValue = SSD1309_Init(drv);

        drv->state = RUN;
    }

    return returnValue;
}

/**
  Function:
    static void SSD1309_Destroy(GFX_Context *context)

  Summary:
    Driver-specific implementation of GFX HAL destroy function.

  Description:
    This function closes the SSD1309 interface and frees up the data structures
    allocated by the driver.

  Parameters:
    context     - GFX context

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
static void SSD1309_Destroy(GFX_Context *context)
{
    // driver specific shutdown tasks
    SSD1309_DRV *drv = (SSD1309_DRV *) context->driver_data;
    if (!drv)
        return;
    
    //Close the interface
    GFX_Disp_Intf_Close((GFX_Disp_Intf) drv->port_priv);
    
    drv->port_priv = NULL;

    // general default shutdown
    defDestroy(context);
}

/**
  Function:
    static void SSD1309_Initialize(GFX_Context *context)

  Summary:
    Driver-specific implementation of GFX HAL initialize function.

  Description:
    This function creates driver-specific data structures, initializes data
    needed by HAL, and opens the port-specific interface to the SSD1309 device.

  Parameters:
    context     - GFX context

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
static GFX_Result SSD1309_Initialize(GFX_Context *context)
{
    SSD1309_DRV *drv;

    drv = (void *) context->memory.calloc(1, sizeof(SSD1309_DRV));
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
                          GFX_COLOR_MASK_GS_8,
                          NULL,
                          &context->layer.layers[0].buffers[0].pb);

    context->layer.layers[0].buffers[0].state = GFX_BS_MANAGED;

    //Open interface to SSD1309 controller
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
    GFX_Result driverSSD1309ContextInitialize(GFX_Context *context)

  Summary:
    Initializes the driver context.

  Description:
    Initializes the driver context.

  Returns:
    * GFX_SUCCESS       - Operation successful

*/
GFX_Result driverSSD1309ContextInitialize(GFX_Context *context)
{
    // set driver-specific data initialization function address
    context->hal.initialize = &SSD1309_Initialize;

    // override essential hal functions
    context->hal.destroy = &SSD1309_Destroy;
    context->hal.update = &SSD1309_Update;

    // set driver-specific function implementations
    context->hal.drawPipeline[GFX_PIPELINE_GCU].pixelSet = &SSD1309_PixelSet;
    context->hal.drawPipeline[GFX_PIPELINE_GCU].pixelGet = &SSD1309_PixelGet;

    context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].pixelSet = &SSD1309_PixelSet;
    context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].pixelGet = &SSD1309_PixelGet;

    return GFX_SUCCESS;
}

/**** End Hardware Abstraction Interfaces ****/
