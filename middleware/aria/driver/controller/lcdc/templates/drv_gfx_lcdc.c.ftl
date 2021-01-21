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
  MPLAB Harmony LCDC Generated Driver Implementation File

  File Name:
    drv_gfx_LCDC_generic.c

  Summary:
    Build-time generated implementation for the LCDC Driver for PIC32C MPU/MCUs.

  Description:
    Build-time generated implementation for the LCDC Driver for PIC32C MPU/MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

<#if HALConnected == true>
<#assign Val_Width = gfx_hal.DisplayWidth>
<#assign Val_Height = gfx_hal.DisplayHeight>
<#assign Val_BacklightEnable = gfx_hal.DisplayBacklightEnable>
<#assign Val_VSYNCNegative = gfx_hal.DisplayVSYNCNegative>
<#assign Val_HSYNCNegative = gfx_hal.DisplayHSYNCNegative>
<#assign Val_UseDataEnable = gfx_hal.DisplayDataEnable>
<#assign Val_DataEnablePolarity = gfx_hal.DisplayDataEnablePolarity>
<#assign Val_DoubleBuffer = gfx_hal.DoubleBufferHint>
<#assign Val_PaletteMode = gfx_hal.GlobalPaletteModeHint>
<#assign Val_FrameBufferColorMode = gfx_hal.ColorModeHint>
<#else>
<#assign Val_Width = DisplayWidth>
<#assign Val_Height = DisplayHeight>
<#assign Val_BacklightEnable = DisplayBacklightEnable>
<#assign Val_VSYNCNegative = DisplayVSYNCNegative>
<#assign Val_HSYNCNegative = DisplayHSYNCNegative>
<#assign Val_UseDataEnable = DisplayDataEnable>
<#assign Val_DataEnablePolarity = DisplayDataEnablePolarity>
<#assign Val_DoubleBuffer = DoubleBuffer>
<#assign Val_PaletteMode = PaletteMode>
<#assign Val_FrameBufferColorMode = FrameBufferColorMode>
</#if>

#include "gfx/driver/controller/lcdc/drv_gfx_lcdc.h"
#include "definitions.h"

<#if Val_DoubleBuffer == true>
#define BUFFER_PER_LAYER    2
<#else>
#define BUFFER_PER_LAYER    1
</#if>

#define DISPLAY_WIDTH  ${Val_Width}
#define DISPLAY_HEIGHT ${Val_Height}

#define MCLK_FREQ_HZ ${MasterClockSourceValue}
#define PIXEL_CLOCK_FREQ_HZ ${PixelClock}
#define LCDC_OUTPUT_COLOR_MODE ${OutputColorMode}
#define LCDC_DISPLAY_GUARD_NUM_FRAMES ${DisplayGuardTime}
#define LCDC_SYNC_EDGE LCDC_SYNC_EDGE_FIRST
#define LCDC_PWM_POLARITY LCDC_POLARITY_POSITIVE
#define GFX_LCDC_BACKGROUND_COLOR 0xffffffff
#define LCDC_NUM_LAYERS ${TotalNumLayers}
#define LCDC_DEFAULT_BRIGHTNESS_PCT ${BacklightBrightnessDefault}

<#if HEOLayerEnable == true>
#define XPHIDEF 0
#define YPHIDEF 0
</#if>

<#if Val_FrameBufferColorMode == "GFX_COLOR_MODE_GS_8">
#define LCDC_DEFAULT_GFX_COLOR_MODE GFX_COLOR_MODE_GS_8
#define FRAMEBUFFER_PIXEL_TYPE    uint8_t
<#elseif Val_FrameBufferColorMode == "GFX_COLOR_MODE_RGB_565">
#define LCDC_DEFAULT_GFX_COLOR_MODE GFX_COLOR_MODE_RGB_565
#define FRAMEBUFFER_PIXEL_TYPE    uint16_t
<#elseif FrameBufferColorMode == "GFX_COLOR_MODE_RGB_888" ||
         FrameBufferColorMode == "GFX_COLOR_MODE_RGBA_8888" ||
         FrameBufferColorMode == "GFX_COLOR_MODE_ARGB_8888">
#define LCDC_DEFAULT_GFX_COLOR_MODE ${Val_FrameBufferColorMode}
#define FRAMEBUFFER_PIXEL_TYPE    uint32_t
<#else>
//Unsupported  framebuffer type specified, default to RGBA8888
#define LCDC_DEFAULT_GFX_COLOR_MODE GFX_COLOR_MODE_RGBA_8888
#define FRAMEBUFFER_PIXEL_TYPE    uint32_t
</#if>

<#if DisplayVSYNCNegative == true>
#define LCDC_VSYNC_POLARITY LCDC_POLARITY_NEGATIVE
<#else>
#define LCDC_VSYNC_POLARITY LCDC_POLARITY_POSITIVE
</#if>

<#if DisplayHSYNCNegative == true>
#define LCDC_HSYNC_POLARITY LCDC_POLARITY_NEGATIVE
<#else>
#define LCDC_HSYNC_POLARITY LCDC_POLARITY_POSITIVE
</#if>

<#if GlobalAlphaEnable == true>
#define LCDC_ENABLE_GLOBAL_HW_ALPHA 1
</#if>

<#if BacklightPWMClockSource == "CLK_MCK">
#define LCDC_PWM_CLOCK_SOURCE LCDC_PWM_SOURCE_SYSTEM
<#else>
#define LCDC_PWM_CLOCK_SOURCE LCDC_PWM_SOURCE_SLOW
</#if>
#define LCDC_PWM_PRESCALER ${BacklightPWMClockPrescaler}



typedef struct
{
  uint32_t addr;
  uint32_t ctrl;
  uint32_t next;
} LCDC_DMA_DESC;

//Layer Parameters
//--------------------------------------------------------------------------
typedef struct __display_layer {
    LCDC_LAYER_ID hwLayerID;
    GFX_Layer * layer;
    FRAMEBUFFER_PIXEL_TYPE  *baseaddr[BUFFER_PER_LAYER];
    int        draw;
    int        frame;
    uint32_t   resx;
    uint32_t   resy;
    uint32_t   buscfg;
    uint32_t   format;
    uint32_t   stride;
    uint32_t   startx;
    uint32_t   starty;
    uint32_t   sizex;
    uint32_t   sizey;
    uint32_t   alpha;
    uint32_t   colorspace;
    uint16_t   color;
    LCDC_DMA_DESC * desc;
    uint32_t frameOffset;
} DISPLAY_LAYER;

//This array defines the z-order of the hw layers in the GLCD
//This must be dynamically configured for the GLCD periperal on the part
static LCDC_LAYER_ID lcdcLayerZOrder[LCDC_NUM_LAYERS] =
{
    LCDC_LAYER_BASE,
<#if Overlay1LayerEnable == true>
    LCDC_LAYER_OVR1,
</#if>
<#if HEOLayerEnable == true>
    LCDC_LAYER_HEO,
</#if>
<#if Overlay2LayerEnable == true>
    LCDC_LAYER_OVR2,
</#if>
};

<#list 0..(TotalNumLayers-1) as i>
FRAMEBUFFER_PIXEL_TYPE  __attribute__ ((section(".region_nocache"), aligned (32))) framebuffer_${i}[DISPLAY_WIDTH * DISPLAY_HEIGHT];
</#list>

<#if Val_DoubleBuffer == true>
<#list 0..(TotalNumLayers-1) as i>
FRAMEBUFFER_PIXEL_TYPE  __attribute__ ((section(".region_nocache"), aligned (32))) framebuffer1_${i}[DISPLAY_WIDTH * DISPLAY_HEIGHT];
</#list>
</#if>

<#list 0..(TotalNumLayers-1) as i>
LCDC_DMA_DESC __attribute__ ((section(".region_nocache"), aligned (64))) channelDesc${i};
</#list>

const char* DRIVER_NAME = "LCDC";
static uint32_t supported_color_format = GFX_COLOR_MASK_RGBA_8888;
uint32_t state;

static DISPLAY_LAYER drvLayer[LCDC_NUM_LAYERS];
static volatile int32_t waitForAlphaSetting[LCDC_NUM_LAYERS] = {0};

/**** Hardware Abstraction Interfaces ****/
enum
{
    INIT = 0,
    RUN
};

static int DRV_GFX_LCDC_Start();

void _IntHandlerVSync(uintptr_t context);

GFX_Context* cntxt;

volatile GFX_Bool pendingDMATransfer[LCDC_NUM_LAYERS];

static void LCDCUpdateDMADescriptor(LCDC_DMA_DESC * desc, uint32_t addr, uint32_t ctrl, uint32_t next)
{
    desc->addr = addr;
    desc->ctrl = ctrl;
    desc->next = next;
}

// function that returns the information for this driver
GFX_Result driverLCDCInfoGet(GFX_DriverInfo* info)
{
    if(info == NULL)
    return GFX_FAILURE;

    // populate info struct
    strcpy(info->name, DRIVER_NAME);
    info->color_formats = supported_color_format;
    info->layer_count = LCDC_NUM_LAYERS;

    return GFX_SUCCESS;
}

static GFX_Result LCDCUpdate()
{
    GFX_Context* context = GFX_ActiveContext();

    if(context == NULL)
        return GFX_FAILURE;

    if(state == INIT)
    {
        if(DRV_GFX_LCDC_Start() != 0)
            return GFX_FAILURE;

        state = RUN;
    }

    return GFX_SUCCESS;
}

static void LCDCDestroy(GFX_Context* context)
{
    // driver specific shutdown tasks
    if(context->driver_data != GFX_NULL)
    {
        context->memory.free(context->driver_data);
        context->driver_data = GFX_NULL;
    }

    // general default shutdown
    defDestroy(context);
}

static LCDC_INPUT_COLOR_MODE convertColorModeGfxToLCDC(GFX_ColorMode mode)
{
    switch(mode)
    {
        case GFX_COLOR_MODE_GS_8:
            return LCDC_INPUT_COLOR_MODE_LUT8;
            break;
        case GFX_COLOR_MODE_RGB_565:
            return LCDC_INPUT_COLOR_MODE_RGB_565;
            break;
        case GFX_COLOR_MODE_RGB_888:
            return LCDC_INPUT_COLOR_MODE_RGB_888;
            break;
        case GFX_COLOR_MODE_RGBA_8888:
            return LCDC_INPUT_COLOR_MODE_RGBA_8888;
            break;
        case GFX_COLOR_MODE_ARGB_8888:
            return LCDC_INPUT_COLOR_MODE_ARGB_8888;
            break;
        case GFX_COLOR_MODE_INDEX_1:
        case GFX_COLOR_MODE_INDEX_4:
        case GFX_COLOR_MODE_INDEX_8:
        case GFX_COLOR_MODE_RGB_332:
            return LCDC_INPUT_COLOR_MODE_UNSUPPORTED;
            break;
        default:
            return LCDC_INPUT_COLOR_MODE_RGBA_8888;
            break;
    }
}

static GFX_Result colorModeSet(GFX_ColorMode mode)
{
    GFX_Layer* layer;
    GFX_Context* context = GFX_ActiveContext();
    LCDC_INPUT_COLOR_MODE LCDCMode;

    layer = context->layer.active;

    LCDCMode = convertColorModeGfxToLCDC(mode);
    if (LCDCMode == LCDC_INPUT_COLOR_MODE_UNSUPPORTED)
        return GFX_FAILURE;

    // use default implementation to initialize buffer struct
    defColorModeSet(mode);

    //Update the active layer's color mode and stride
    drvLayer[layer->id].colorspace = LCDCMode;
    LCDC_SetRGBModeInput(drvLayer[layer->id].hwLayerID, drvLayer[layer->id].colorspace );

    return GFX_SUCCESS;
}

static GFX_Result layerBufferCountSet(uint32_t count)
{
    GFX_Layer* layer;
    GFX_Context* context = GFX_ActiveContext();
    uint32_t i;
    
    layer = context->layer.active;

    if(count > BUFFER_PER_LAYER)
    {
        count = BUFFER_PER_LAYER;
    }
    
    // use default implementation to initialize buffer struct
    defLayerBufferCountSet(count);
    
    // ensure all buffers are marked as managed by the driver so application
    // can't delete or modify them
    for(i = 0; i < layer->buffer_count; i++)
    {
        GFX_PixelBufferCreate(layer->rect.display.width,
                  layer->rect.display.height,
                  context->colorMode,
                  drvLayer[layer->id].baseaddr[i],
                  &layer->buffers[i].pb);

        layer->buffers[i].state = GFX_BS_MANAGED;
    }

    return GFX_SUCCESS;
}

static GFX_Result layerBufferAddressSet(uint32_t idx, GFX_Buffer address)
{
    GFX_Layer* layer;
    GFX_Context* context = GFX_ActiveContext();
    
    if (address == NULL || idx >= BUFFER_PER_LAYER)
    {
        return GFX_FAILURE;
    }
    
    layer = context->layer.active;

    //No need to call default address set as this is driver managed
    //defLayerBufferAddressSet(idx, address);
    
    drvLayer[layer->id].baseaddr[idx] = address;
    drvLayer[layer->id].desc->addr = (uint32_t) drvLayer[layer->id].baseaddr[idx];

    LCDC_SetDMAHeadPointer(drvLayer[layer->id].hwLayerID, (uint32_t) drvLayer[layer->id].desc);
    LCDC_UpdateAddToQueue(drvLayer[layer->id].hwLayerID);

    return GFX_SUCCESS;
}

void layerSetFrameBufferOffset(GFX_Layer* layer, uint32_t offset)
{
    FRAMEBUFFER_PIXEL_TYPE * baseAddr = (FRAMEBUFFER_PIXEL_TYPE *) drvLayer[layer->id].baseaddr[layer->buffer_read_idx];
    
    drvLayer[layer->id].frameOffset = offset;
    
    LCDCUpdateDMADescriptor(drvLayer[layer->id].desc,
                            (uint32_t) &baseAddr[drvLayer[layer->id].frameOffset] ,
                            0x1,
                            (uint32_t) drvLayer[layer->id].desc);
}

void * layerGetReadFrameBufferBase(GFX_Layer* layer)
{
    return (void *)drvLayer[layer->id].baseaddr[layer->buffer_read_idx];
}

static GFX_Result layerBufferAllocate(uint32_t idx)
{
    GFX_Layer* layer;
    GFX_Context* context = GFX_ActiveContext();
    uint32_t  i,j;
    uint32_t  color = 0;
    
    layer = context->layer.active;

    if (layer->id == 0)
    {
        color = GFX_LCDC_BACKGROUND_COLOR;
    }
    
    for(i = 0; i < layer->rect.display.height; i++)
    {
        for(j = 0; j < layer->rect.display.width; j++)
        {
            *(uint32_t*)(drvLayer[layer->id].baseaddr[idx] + i*layer->rect.display.width + j) = color;
        }
    }

    return GFX_SUCCESS;
}

static GFX_Result layerBufferFree(uint32_t idx)
{
    return GFX_UNSUPPORTED;
}

static GFX_Result LCDC_BrightnessRangeGet(uint32_t *low, uint32_t *high)
{
    *low = 0;
    *high = 100;

    return GFX_SUCCESS;
}

static GFX_Result LCDC_BrightnessSet(uint32_t brightness)
{

    LCDC_SetPWMCompareValue(brightness * 0xff / 100);

    return GFX_SUCCESS;
}

static GFX_Result layerPositionSet(int32_t x, int32_t y)
{
    uint32_t idx;
    
    idx = GFX_ActiveContext()->layer.active->id;

    defLayerPositionSet(x, y);    

    LCDC_SetWindowPosition(drvLayer[idx].hwLayerID, 
                            GFX_ActiveContext()->layer.active->rect.display.x,
                            GFX_ActiveContext()->layer.active->rect.display.y);

    return GFX_SUCCESS;
}

<#if HEOLayerEnable == true>
static void layerHEOGetScalingFactors(uint16_t xmemsize,
                                     uint16_t ymemsize,
                                     uint16_t xsize,
                                     uint16_t ysize,
                                     uint16_t* xfactor,
                                     uint16_t* yfactor)
{
    uint16_t xfactor1st, yfactor1st;

    xmemsize--;
    ymemsize--;
    xsize--;
    ysize--;
            
    xfactor1st = ((2048 * xmemsize - 256 * XPHIDEF)/ xsize) + 1;
    yfactor1st = ((2048 * ymemsize - 256 * XPHIDEF)/ ysize) + 1;

    if ((xfactor1st * xsize / 2048) > xmemsize)
        *xfactor = xfactor1st - 1;
    else
        *xfactor = xfactor1st;

    if ((yfactor1st * ysize / 2048) > ymemsize)
        *yfactor = yfactor1st - 1;
    else
        *yfactor = yfactor1st;
}

static void layerHEOSetSize(uint16_t s_width, uint16_t s_height, uint16_t w_width, uint16_t w_height)
{
    LCDC_SetWindowSize(LCDC_LAYER_HEO, w_width, w_height);
    LCDC_SetHEOImageMemSize(s_width, s_height);    
    
    //Source and window size are not the same, use scaler
    if (s_width != w_width || s_height != w_height)
    {
        uint16_t scale_w, scale_h;
        
        layerHEOGetScalingFactors(s_width,
                                s_height,
                                w_width,
                                w_height,
                                &scale_w,
                                &scale_h);
          
        LCDC_SetHEOScaler(scale_h, scale_w, true);
    }
    else
    {
        LCDC_SetHEOScaler(0, 0, false);
    }
}
</#if>

static GFX_Result layerSizeSet(int32_t width, int32_t height)
{
    uint32_t idx;

    idx = GFX_ActiveContext()->layer.active->id;

    defLayerSizeSet(width, height);
<#if HEOLayerEnable == true>
    if (drvLayer[idx].hwLayerID == LCDC_LAYER_HEO)
        layerHEOSetSize(GFX_ActiveContext()->layer.active->rect.display.width,
                        GFX_ActiveContext()->layer.active->rect.display.height,
                        GFX_ActiveContext()->layer.active->rect.display.width,
                        GFX_ActiveContext()->layer.active->rect.display.height);
    else
        LCDC_SetWindowSize(drvLayer[idx].hwLayerID,
                           GFX_ActiveContext()->layer.active->rect.display.width,
                           GFX_ActiveContext()->layer.active->rect.display.height);
<#else>
    LCDC_SetWindowSize(drvLayer[idx].hwLayerID,
                       GFX_ActiveContext()->layer.active->rect.display.width,
                       GFX_ActiveContext()->layer.active->rect.display.height);
</#if>

    return GFX_SUCCESS;
}

<#if GlobalAlphaEnable == true>
static GFX_Result layerAlphaAmountSet(uint32_t alpha, GFX_Bool wait)
{
    uint32_t idx;
    
    idx = GFX_ActiveContext()->layer.active->id;

    if(wait == GFX_TRUE)
    {
        waitForAlphaSetting[idx] = alpha;
    }
    else
    {
        waitForAlphaSetting[idx] = -1;
        
        defLayerAlphaAmountSet(alpha, wait);
        
        //Local alpha does not work with global alpha, disable it
        LCDC_SetBlenderLocalAlphaEnable(drvLayer[idx].hwLayerID, false);
        
        LCDC_SetBlenderGlobalAlpha(drvLayer[idx].hwLayerID, alpha);
        LCDC_SetBlenderGlobalAlphaEnable(drvLayer[idx].hwLayerID, true);
        LCDC_UpdateOverlayAttributesEnable(drvLayer[idx].hwLayerID);
        
    }

    
    return GFX_SUCCESS;
}
    
static uint32_t layerAlphaAmountGet(void)
{
    uint32_t idx;
    
    idx = GFX_ActiveContext()->layer.active->id;

    //TBD: Implement Alpha setting and return GFX_SUCCESS
    //return GFX_SUCCESS;
    return GFX_FAILURE;
}
</#if>

void layerSwapped(GFX_Layer* layer)
{
    FRAMEBUFFER_PIXEL_TYPE * baseAddr;
    if (layer->buffer_count > BUFFER_PER_LAYER)
        return;
    
    baseAddr = (FRAMEBUFFER_PIXEL_TYPE *) drvLayer[layer->id].baseaddr[layer->buffer_read_idx];
    switch(drvLayer[layer->id].hwLayerID)
    {
        case LCDC_LAYER_BASE:
        case LCDC_LAYER_OVR1:
        case LCDC_LAYER_OVR2:
        case LCDC_LAYER_HEO:
        case LCDC_LAYER_PP:
            LCDCUpdateDMADescriptor(drvLayer[layer->id].desc,
                                    (uint32_t) &baseAddr[drvLayer[layer->id].frameOffset] ,
                                    0x1,
                                    (uint32_t) drvLayer[layer->id].desc);
            break;
        default:
            break;
    }
}

void layerEnable(GFX_Layer* layer, GFX_Bool enable)
{
    uint32_t layerIdx = layer->id;
    
    if(enable == GFX_TRUE)
    {
        LCDC_SetChannelEnable(drvLayer[layerIdx].hwLayerID, true);
        LCDC_IRQ_Enable(LCDC_INTERRUPT_BASE + drvLayer[layerIdx].hwLayerID);
    }
    else
    {
        LCDC_SetChannelEnable(drvLayer[layerIdx].hwLayerID, false);
        LCDC_IRQ_Disable(LCDC_INTERRUPT_BASE + drvLayer[layerIdx].hwLayerID);
    }

    layer->enabled = enable;    
}

static GFX_Result layerEnabledSet(GFX_Bool val)
{
    GFX_ActiveContext()->layer.active->enabled = val;

    layerEnable(GFX_ActiveContext()->layer.active, val);
    
    return GFX_SUCCESS;
}

static GFX_Result LCDCInitialize(GFX_Context* context)
{
    uint32_t      xResolution;
    uint32_t      yResolution;
    uint32_t      rightMargin;
    uint32_t      lowerMargin;
    uint32_t      hsyncLength;
    uint32_t      vsyncLength;
    uint32_t      leftMargin;
    uint32_t      upperMargin;
    uint32_t      layerCount;
    uint32_t      i,j;

    cntxt = context;

    // general default initialization
    if(defInitialize(context) == GFX_FAILURE)
            return GFX_FAILURE;
    
    //Clear the descriptor and structures
    memset(drvLayer, 0, sizeof(drvLayer));

    // override default HAL functions with LCDC specific implementations
    context->hal.update = &LCDCUpdate;
    context->hal.destroy = &LCDCDestroy;
    context->hal.layerBufferCountSet = &layerBufferCountSet;
    context->hal.layerBufferAddressSet = &layerBufferAddressSet;
    context->hal.layerBufferAllocate = &layerBufferAllocate;
    context->hal.layerBufferFree = &layerBufferFree;
    context->hal.layerSwapped = &layerSwapped;
    context->hal.layerPositionSet = &layerPositionSet;
    context->hal.layerSizeSet = &layerSizeSet;
    context->hal.layerEnabledSet = &layerEnabledSet;

<#if GlobalAlphaEnable == true>
    context->hal.layerAlphaAmountSet = &layerAlphaAmountSet;
    context->hal.layerAlphaAmountGet = &layerAlphaAmountGet;
</#if>

    context->hal.colorModeSet = &colorModeSet;

    /* set temporary information */
    xResolution = context->display_info->rect.width;
    yResolution = context->display_info->rect.height;
    rightMargin = context->display_info->attributes.horz.front_porch;
    leftMargin = context->display_info->attributes.horz.back_porch;
    hsyncLength = context->display_info->attributes.horz.pulse_width;
    vsyncLength = context->display_info->attributes.vert.pulse_width;
    upperMargin = context->display_info->attributes.vert.back_porch;
    lowerMargin = context->display_info->attributes.vert.front_porch;

    /* LCDC initialization */
    //1. Configure the LCD timing parameters
    LCDC_WaitForSyncInProgress();
    LCDC_SetPWMClockSourceSelection(LCDC_PWM_CLOCK_SOURCE);
    LCDC_SetClockDivider(MCLK_FREQ_HZ / PIXEL_CLOCK_FREQ_HZ - 2);

    //Disable all layers for now
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_BASE, false); 
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_OVR1, false); 
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_OVR2, false);
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_HEO, false);
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_PP, false);

    LCDC_WaitForSyncInProgress();
    LCDC_SetHSYNCPulseWidth(hsyncLength); //Set the pulse widths
    LCDC_SetVSYNCPulseWidth(vsyncLength);

    LCDC_WaitForSyncInProgress();
    LCDC_SetVerticalFrontPorchWidth(lowerMargin); //Set the vertical porches
    LCDC_SetVerticalBackPorchWidth(upperMargin);
    
    LCDC_WaitForSyncInProgress();
    LCDC_SetHorizontalFrontPorchWidth(rightMargin); //Set the horizontal porches
    LCDC_SetHorizontalBackPorchWidth(leftMargin);
    
    LCDC_WaitForSyncInProgress();
    LCDC_SetNumActiveRows(yResolution);
    LCDC_SetNumPixelsPerLine(xResolution);
    
    LCDC_WaitForSyncInProgress();
    LCDC_SetDisplayGuardTime(LCDC_DISPLAY_GUARD_NUM_FRAMES);
    LCDC_SetOutputMode(LCDC_OUTPUT_COLOR_MODE);
    LCDC_SetDisplaySignalSynchronization(true);
    LCDC_SetVSYNCPulseStart(LCDC_SYNC_EDGE);
    LCDC_SetVSYNCPolarity(LCDC_VSYNC_POLARITY);
    LCDC_SetHSYNCPolarity(LCDC_HSYNC_POLARITY);

<#if BacklightPWMEnable == true>
    LCDC_WaitForSyncInProgress();
    LCDC_SetPWMCompareValue(LCDC_DEFAULT_BRIGHTNESS_PCT * 0xFF / 100);
    LCDC_SetPWMSignalPolarity(LCDC_PWM_POLARITY);
    LCDC_SetPWMPrescaler(LCDC_PWM_PRESCALER);
</#if>

    //2. Enable the Pixel Clock
    LCDC_WaitForSyncInProgress();
    LCDC_SetPixelClockEnable(true);
    
    //3. Poll CLKSTS field of the LCDC_LCDSR register to check that the clock is running.
    LCDC_WaitForClockRunning();
    
    // 4. Enable Horizontal and Vertical Synchronization by writing one to the
    // SYNCEN field of the LCDC_LCDEN register.
    LCDC_WaitForSyncInProgress();
    LCDC_SetSYNCEnable(true);
    
    // 5. Check that synchronization is up. */    
    LCDC_WaitForSynchronization();
    
    //6. Enable the display power signal 
    LCDC_WaitForSyncInProgress();
    LCDC_SetDISPSignalEnable(true);
    
    //7. Wait for power signal to be activated
    LCDC_WaitForDISPSignal();
    
    //8. Enable the backlight
    LCDC_WaitForSyncInProgress();
<#if BacklightPWMEnable == true>
    LCDC_SetPWMEnable(true);
</#if>

<#list 0..(TotalNumLayers-1) as i>
    drvLayer[${i}].baseaddr[0] = framebuffer_${i};
</#list>
<#if Val_DoubleBuffer == true>
<#list 0..(TotalNumLayers-1) as i>
    drvLayer[${i}].baseaddr[1] = framebuffer1_${i};
</#list>
</#if>
<#list 0..(TotalNumLayers-1) as i>
    drvLayer[${i}].desc = &channelDesc${i};
</#list>
    
    for (layerCount = 0; layerCount < context->layer.count; layerCount++)
    {
        drvLayer[layerCount].layer = &context->layer.layers[layerCount];
        drvLayer[layerCount].hwLayerID = lcdcLayerZOrder[layerCount];
        drvLayer[layerCount].resx       = context->layer.layers[layerCount].rect.display.width;
        drvLayer[layerCount].resy       = context->layer.layers[layerCount].rect.display.height;
        drvLayer[layerCount].startx     = context->layer.layers[layerCount].rect.display.x;
        drvLayer[layerCount].starty     = context->layer.layers[layerCount].rect.display.y;
        drvLayer[layerCount].sizex      = drvLayer[layerCount].resx;
        drvLayer[layerCount].sizey      = drvLayer[layerCount].resy;
        drvLayer[layerCount].alpha      = context->layer.layers[layerCount].alphaAmount;
        drvLayer[layerCount].colorspace = convertColorModeGfxToLCDC(LCDC_DEFAULT_GFX_COLOR_MODE);
        drvLayer[layerCount].frameOffset = 0;
        LCDCUpdateDMADescriptor(drvLayer[layerCount].desc, 
                                (uint32_t) drvLayer[layerCount].baseaddr[0],
                                0x01,
                                (uint32_t) drvLayer[layerCount].desc);
        
        //Clear frame buffer
        for(i = 0; i < context->layer.layers[layerCount].rect.display.height; i++)
        {
            for(j = 0; j < context->layer.layers[layerCount].rect.display.width; j++)
            {
                *(uint32_t*)(drvLayer[layerCount].baseaddr[0] + 
                    i*context->layer.layers[layerCount].rect.display.width + j) = 0;
            }
        }
 
        LCDC_SetLayerClockGatingDisable(drvLayer[layerCount].hwLayerID, false);
        LCDC_SetWindowPosition(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].startx, drvLayer[layerCount].starty);
<#if HEOLayerEnable == true>
        if (drvLayer[layerCount].hwLayerID == LCDC_LAYER_HEO)
            layerHEOSetSize(drvLayer[layerCount].resx, drvLayer[layerCount].resy, drvLayer[layerCount].resx, drvLayer[layerCount].resy);
        else
            LCDC_SetWindowSize(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].resx, drvLayer[layerCount].resy);
<#else>
        LCDC_SetWindowSize(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].resx, drvLayer[layerCount].resy);
</#if>
        LCDC_SetUseDMAPathEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_SetRGBModeInput(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].colorspace);
        LCDC_SetDMAAddressRegister(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].desc->addr);
        LCDC_SetDMADescriptorNextAddress(drvLayer[layerCount].hwLayerID, (uint32_t) drvLayer[layerCount].desc);
        LCDC_SetDMAHeadPointer(drvLayer[layerCount].hwLayerID, (uint32_t) drvLayer[layerCount].desc);
        LCDC_SetTransferDescriptorFetchEnable(drvLayer[layerCount].hwLayerID, true);

        //Note: Blender APIs don't do anything to the base layer
        LCDC_SetBlenderOverlayLayerEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_SetBlenderDMALayerEnable(drvLayer[layerCount].hwLayerID, true); //Enable blender DMA
        LCDC_SetBlenderLocalAlphaEnable(drvLayer[layerCount].hwLayerID, true); //Use local alpha
        LCDC_SetBlenderIteratedColorEnable(drvLayer[layerCount].hwLayerID, true); //Enable iterated color
        LCDC_SetBlenderUseIteratedColor(drvLayer[layerCount].hwLayerID, true); //Use iterated color        
        LCDC_UpdateOverlayAttributesEnable(drvLayer[layerCount].hwLayerID);
        LCDC_UpdateAttribute(drvLayer[layerCount].hwLayerID); //Apply the attributes

        // all layers off by default
        context->layer.layers[layerCount].enabled = GFX_FALSE;
    }

<#if HEOLayerEnable == true>
    //Set HEO layer on top of OVL1
    LCDC_SetHEOVideoPriority(true);
</#if>

    //Register the interrupt handler
    LCDC_IRQ_CallbackRegister(_IntHandlerVSync, (uintptr_t) NULL);
    
    return GFX_SUCCESS;
}

static void layerSwapPending(GFX_Layer* layer)
{
    uint32_t l;
    GFX_Context* context = GFX_ActiveContext();
    GFX_Layer* lyr;
    LCDC_LAYER_ID hwLayerID;

    if(context->layerSwapSync)
    {
    for(l = 0; l < context->layer.count; l++)
    {
            lyr = &context->layer.layers[l];
            hwLayerID = drvLayer[lyr->id].hwLayerID;

            if(lyr->enabled == GFX_TRUE)
            {
                if(lyr->invalid == GFX_TRUE && lyr->swap == GFX_FALSE)
                    return;
                
                pendingDMATransfer[hwLayerID] = GFX_TRUE;
                LCDC_LAYER_IRQ_Enable(hwLayerID, LCDC_LAYER_INTERRUPT_DMA);
            }
            else
                pendingDMATransfer[hwLayerID] = GFX_FALSE;
        }
        
        //Wait until DMA is complete on all layers
        for(l = 0; l < context->layer.count; l++)
        {
            lyr = &context->layer.layers[l];
            hwLayerID = drvLayer[lyr->id].hwLayerID;
            
            if (pendingDMATransfer[hwLayerID] == GFX_TRUE)
                l = 0;
        }
    }
    
    //swap the descriptors
    for(l = 0; l < context->layer.count; l++)
    {    
        lyr = &context->layer.layers[l];

        if(lyr->swap == GFX_TRUE)
            GFX_LayerSwap(lyr);    
            
        if(waitForAlphaSetting[l] >= 0)
            lyr->alphaAmount = waitForAlphaSetting[l];
    }
}

// function that initialized the driver context
GFX_Result driverLCDCContextInitialize(GFX_Context* context)
{
    // set driver-specific data initialization function address
    context->hal.initialize = &LCDCInitialize;

    // set driver-specific destroy function address
    context->hal.destroy = &LCDCDestroy;
    
    // vsync support
    context->hal.layerSwapPending = &layerSwapPending; 

    // Backlight PWM brightness support
    context->hal.brightnessRangeGet = &LCDC_BrightnessRangeGet;
    context->hal.brightnessSet = &LCDC_BrightnessSet;


    return GFX_SUCCESS;
}

void _IntHandlerVSync(uintptr_t context)
{
    uint32_t i, status;
    
    for (i = 0; i < LCDC_NUM_LAYERS; i++)
    {
        LCDC_LAYER_ID layerID = lcdcLayerZOrder[i];
        status = LCDC_LAYER_IRQ_Status(layerID);
        if (status && pendingDMATransfer[layerID] == GFX_TRUE)
        {
            LCDC_LAYER_IRQ_Disable(layerID, LCDC_LAYER_INTERRUPT_DMA);
            
            pendingDMATransfer[layerID] = GFX_FALSE;
        }
    }
}

/**** End Hardware Abstraction Interfaces ****/


static int DRV_GFX_LCDC_Start()
{
    return 0;
}
