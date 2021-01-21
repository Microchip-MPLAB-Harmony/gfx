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

/*******************************************************************************
  MPLAB Harmony LCDC Generated Driver Implementation File

  File Name:
    drv_gfx_lcdc.c

  Summary:
    Build-time generated implementation for the LCDC Driver for PIC32C MPU/MCUs.

  Description:
    Build-time generated implementation for the LCDC Driver for PIC32C MPU/MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

<#if gfx_hal_le??>

<#assign Val_Width = gfx_hal_le.DisplayWidth>
<#assign Val_Height = gfx_hal_le.DisplayHeight>
<#assign Val_BacklightEnable = gfx_hal_le.DisplayBacklightEnable>
<#assign Val_VSYNCNegative = gfx_hal_le.DisplayVSYNCNegative>
<#assign Val_HSYNCNegative = gfx_hal_le.DisplayHSYNCNegative>
<#assign Val_UseDataEnable = gfx_hal_le.DisplayDataEnable>
<#assign Val_DataEnablePolarity = gfx_hal_le.DisplayDataEnablePolarity>
<#assign Val_HorzFrontPorch = gfx_hal_le.DisplayHorzFrontPorch>
<#assign Val_HorzBackPorch = gfx_hal_le.DisplayHorzBackPorch>
<#assign Val_HorzPulseWidth = gfx_hal_le.DisplayHorzPulseWidth>
<#assign Val_VertFrontPorch = gfx_hal_le.DisplayVertFrontPorch>
<#assign Val_VertBackPorch = gfx_hal_le.DisplayVertBackPorch>
<#assign Val_VertPulseWidth = gfx_hal_le.DisplayVertPulseWidth>

<#else>

<#assign Val_Width = DisplayWidth>
<#assign Val_Height = DisplayHeight>
<#assign Val_BacklightEnable = DisplayBacklightEnable>
<#assign Val_VSYNCNegative = DisplayVSYNCNegative>
<#assign Val_HSYNCNegative = DisplayHSYNCNegative>
<#assign Val_UseDataEnable = DisplayDataEnable>
<#assign Val_DataEnablePolarity = DisplayDataEnablePolarity>
<#assign Val_HorzFrontPorch = DisplayHorzFrontPorch>
<#assign Val_HorzBackPorch = DisplayHorzBackPorch>
<#assign Val_HorzPulseWidth = DisplayHorzPulseWidth>
<#assign Val_VertFrontPorch = DisplayVertFrontPorch>
<#assign Val_VertBackPorch = DisplayVertBackPorch>
<#assign Val_VertPulseWidth = DisplayVertPulseWidth>

</#if>

<#assign Val_DoubleBuffer = DoubleBuffer>
<#assign Val_FrameBufferColorMode = FrameBufferColorMode>

#include "gfx/driver/controller/lcdc/drv_gfx_lcdc.h"
#include "definitions.h"

<#if Val_DoubleBuffer == true>
#define BUFFER_PER_LAYER    2
<#else>
#define BUFFER_PER_LAYER    1
</#if>

#define DISPLAY_WIDTH  ${Val_Width}
#define DISPLAY_HEIGHT ${Val_Height}

#define PIXEL_CLOCK_DIV ${PixelClockDiv}
#define LCDC_OUTPUT_COLOR_MODE ${OutputColorMode}
#define LCDC_DISPLAY_GUARD_NUM_FRAMES ${DisplayGuardTime}
#define LCDC_SYNC_EDGE LCDC_SYNC_EDGE_FIRST
#define LCDC_PWM_POLARITY LCDC_POLARITY_POSITIVE
#define GFX_LCDC_BACKGROUND_COLOR 0xffffffff
#define GFX_LCDC_LAYERS ${TotalNumLayers}
#define LCDC_DEFAULT_BRIGHTNESS_PCT ${BacklightBrightnessDefault}

<#if HEOLayerEnable == true>
#define XPHIDEF 0
#define YPHIDEF 0
</#if>

<#if FrameBufferColorMode == "RGB_565">
#define LCDC_DEFAULT_GFX_COLOR_MODE LCDC_INPUT_COLOR_MODE_RGB_565
#define FRAMEBUFFER_PTR_TYPE    uint16_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint16_t
<#elseif FrameBufferColorMode == "RGBA_8888">
#define LCDC_DEFAULT_GFX_COLOR_MODE LCDC_INPUT_COLOR_MODE_RGBA_8888
#define FRAMEBUFFER_PTR_TYPE    uint32_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint32_t
<#else>
//Unsupported  framebuffer type specified, default to RGBA8888
#define LCDC_DEFAULT_GFX_COLOR_MODE LCDC_INPUT_COLOR_MODE_UNSUPPORTED
#define FRAMEBUFFER_PTR_TYPE      uint32_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint32_t
</#if>

typedef enum
{
    LAYER_UNLOCKED,
    LAYER_LOCKED,
    LAYER_LOCKED_PENDING,
} LAYER_LOCK_STATUS;

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
    bool enabled;
    volatile LAYER_LOCK_STATUS updateLock;
} DISPLAY_LAYER;

//This array defines the z-order of the hw layers in the LCDC
//This must be dynamically configured for the LCDC periperal on the part
static LCDC_LAYER_ID lcdcLayerZOrder[GFX_LCDC_LAYERS] =
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
uint32_t state;

<#if UseGPU == true>
static gfxBool useGPU = true;
<#else>
static gfxBool useGPU = false;
</#if>
static unsigned int vsyncCount = 0;
static gfxPixelBuffer pixelBuffer[GFX_LCDC_LAYERS];
<#if le_gfx_gfx2d??>
static gfxRect srcRect, destRect;
</#if>
static unsigned int activeLayer = 0;

static DISPLAY_LAYER drvLayer[GFX_LCDC_LAYERS];
static volatile int32_t waitForAlphaSetting[GFX_LCDC_LAYERS] = {0};

/**** Hardware Abstraction Interfaces ****/
enum
{
    INIT = 0,
    RUN
};

static gfxResult DRV_LCDC_UpdateLayer(unsigned int layer);

void _IntHandlerVSync(uintptr_t context);

static void LCDCUpdateDMADescriptor(LCDC_DMA_DESC * desc, uint32_t addr, uint32_t ctrl, uint32_t next)
{
    desc->addr = addr;
    desc->ctrl = ctrl;
    desc->next = next;
}

void DRV_LCDC_Update()
{
    if(state == INIT)
    {
        state = RUN;
    }
}

static LCDC_INPUT_COLOR_MODE getLCDCColorModeFromGFXColorMode(gfxColorMode mode)
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

static gfxColorMode getGFXColorModeFromLCDC(LCDC_INPUT_COLOR_MODE mode)
{
    switch(mode)
    {
        case LCDC_INPUT_COLOR_MODE_LUT8:
            return GFX_COLOR_MODE_GS_8;
        case LCDC_INPUT_COLOR_MODE_RGB_565:
            return GFX_COLOR_MODE_RGB_565;
        case LCDC_INPUT_COLOR_MODE_RGB_888:
            return GFX_COLOR_MODE_RGB_888;
        case LCDC_INPUT_COLOR_MODE_ARGB_8888:
            return GFX_COLOR_MODE_ARGB_8888;
        case LCDC_INPUT_COLOR_MODE_RGBA_8888:
            return GFX_COLOR_MODE_RGBA_8888;
        default:
            return GFX_COLOR_MODE_RGBA_8888;
    }
}

gfxResult LCDC_BrightnessRangeGet(uint32_t *low, uint32_t *high)
{
    *low = 0;
    *high = 100;

    return GFX_SUCCESS;
}

gfxResult LCDC_BrightnessSet(uint32_t brightness)
{

    LCDC_SetPWMCompareValue(brightness * 0xff / 100);

    return GFX_SUCCESS;
}

gfxResult DRV_LCDC_Initialize()
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
    uint32_t      bufferCount;
    
    //Clear the descriptor and structures
    memset(drvLayer, 0, sizeof(drvLayer));

    /* set temporary information */
    xResolution     = ${Val_Width};
    yResolution     = ${Val_Height};
    rightMargin     = ${Val_HorzFrontPorch};
    leftMargin      = ${Val_HorzBackPorch};
    hsyncLength     = ${Val_HorzPulseWidth};
    vsyncLength     = ${Val_VertPulseWidth};
    upperMargin     = ${Val_VertBackPorch};
    lowerMargin     = ${Val_VertPulseWidth};


    /* LCDC initialization */
    //1. Configure the LCD timing parameters
    LCDC_WaitForSyncInProgress();
    LCDC_SetPWMClockSourceSelection(LCDC_PWM_CLOCK_SOURCE);
    LCDC_SetClockDivider(PIXEL_CLOCK_DIV);

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
    
    for (layerCount = 0; layerCount < GFX_LCDC_LAYERS; layerCount++)
    {
        drvLayer[layerCount].hwLayerID = lcdcLayerZOrder[layerCount];
        drvLayer[layerCount].resx       = xResolution;
        drvLayer[layerCount].resy       = yResolution;
        drvLayer[layerCount].startx     = 0;
        drvLayer[layerCount].starty     = 0;
        drvLayer[layerCount].sizex      = drvLayer[layerCount].resx;
        drvLayer[layerCount].sizey      = drvLayer[layerCount].resy;
        drvLayer[layerCount].alpha      = 255;
        drvLayer[layerCount].colorspace = LCDC_DEFAULT_GFX_COLOR_MODE;
        drvLayer[layerCount].frameOffset = 0;
        LCDCUpdateDMADescriptor(drvLayer[layerCount].desc, 
                                (uint32_t) drvLayer[layerCount].baseaddr[0],
                                0x01,
                                (uint32_t) drvLayer[layerCount].desc);
        
        //Clear frame buffer
        for(bufferCount = 0; bufferCount < BUFFER_PER_LAYER; ++bufferCount)
        {
            memset(drvLayer[layerCount].baseaddr[bufferCount], 0, sizeof(FRAMEBUFFER_PIXEL_TYPE) * DISPLAY_WIDTH * DISPLAY_HEIGHT);
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

        LCDC_SetChannelEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_IRQ_Enable(LCDC_INTERRUPT_BASE + drvLayer[layerCount].hwLayerID);

        gfxPixelBufferCreate(xResolution,
                    yResolution,
                    getGFXColorModeFromLCDC(LCDC_DEFAULT_GFX_COLOR_MODE),
                    drvLayer[layerCount].baseaddr[0],
                    &pixelBuffer[layerCount]);
    }

<#if HEOLayerEnable == true>
    //Set HEO layer on top of OVL1
    LCDC_SetHEOVideoPriority(true);
</#if>

    //Register the interrupt handler
    LCDC_IRQ_CallbackRegister(_IntHandlerVSync, (uintptr_t) NULL);
    
    return GFX_SUCCESS;
}

void _IntHandlerVSync(uintptr_t context)
{
    uint32_t i, status;
    
    for (i = 0; i < GFX_LCDC_LAYERS; i++)
    {
        LCDC_LAYER_ID layerID = lcdcLayerZOrder[i];
        status = LCDC_LAYER_IRQ_Status(layerID);

        if (status && drvLayer[i].updateLock == LAYER_LOCKED_PENDING)
        {
            LCDC_LAYER_IRQ_Disable(layerID, LCDC_LAYER_INTERRUPT_DMA);

            DRV_LCDC_UpdateLayer(i);
            drvLayer[i].updateLock = LAYER_UNLOCKED;
        }
    }
}

/**** End Hardware Abstraction Interfaces ****/

gfxColorMode DRV_LCDC_GetColorMode()
{
<#if FrameBufferColorMode == "GS_8">
    return GFX_COLOR_MODE_GS_8;
<#elseif FrameBufferColorMode == "RGB_332">
    return GFX_COLOR_MODE_RGB_332;
<#elseif FrameBufferColorMode == "RGB_565">
    return GFX_COLOR_MODE_RGB_565;
<#elseif FrameBufferColorMode == "RGB_888">
    return GFX_COLOR_MODE_RGB_888;
<#elseif FrameBufferColorMode == "RGBA_8888">
    return GFX_COLOR_MODE_RGBA_8888;
<#elseif FrameBufferColorMode == "ARGB_8888">
    return GFX_COLOR_MODE_ARGB_8888;
</#if>
}

gfxResult DRV_LCDC_BlitBuffer(int32_t x,
                             int32_t y,
                             gfxPixelBuffer* buf)
{
    if (state != RUN)
        return GFX_FAILURE;

<#if le_gfx_gfx2d??>
    if(useGPU)
    {
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.height = buf->size.height;
        srcRect.width = buf->size.width;

        destRect.x = x;
        destRect.y = y;
        destRect.height = buf->size.height;
        destRect.width = buf->size.width;

        gfxGPUInterface.blitBuffer(buf, &srcRect, &pixelBuffer[activeLayer], &destRect );
    }
    else
</#if>
    {
        void* srcPtr;
        void* destPtr;
        uint32_t row, rowSize;

        rowSize = buf->size.width * gfxColorInfoTable[buf->mode].size;

        for(row = 0; row < buf->size.height; row++)
        {
            srcPtr = gfxPixelBufferOffsetGet(buf, 0, row);
            destPtr = gfxPixelBufferOffsetGet(&pixelBuffer[activeLayer], x, y + row);

            memcpy(destPtr, srcPtr, rowSize);
        }
    }

    return GFX_SUCCESS;
}

static gfxResult DRV_LCDC_UpdateLayer(unsigned int layer)
{
    if (drvLayer[layer].enabled == true)
        LCDC_SetUseDMAPathEnable(drvLayer[layer].hwLayerID, true);
    else
        LCDC_SetUseDMAPathEnable(drvLayer[layer].hwLayerID, false);

    //Update the active layer's color mode and stride
    LCDC_SetRGBModeInput(drvLayer[layer].hwLayerID, drvLayer[layer].colorspace);
    LCDC_SetDMAAddressRegister(drvLayer[layer].hwLayerID, drvLayer[layer].desc->addr);
    LCDC_SetDMADescriptorNextAddress(drvLayer[layer].hwLayerID, (uint32_t) drvLayer[layer].desc);
    LCDC_SetDMAHeadPointer(drvLayer[layer].hwLayerID, (uint32_t) drvLayer[layer].desc);
    LCDC_SetTransferDescriptorFetchEnable(drvLayer[layer].hwLayerID, true);
        
    LCDC_UpdateAddToQueue(drvLayer[layer].hwLayerID);

    return GFX_SUCCESS;
}

static gfxDriverIOCTLResponse _layerConfig(gfxDriverIOCTLRequest request,
                                           gfxIOCTLArg_LayerArg* arg)
{
    gfxIOCTLArg_LayerValue* val;
    gfxIOCTLArg_LayerPosition* pos;
    gfxIOCTLArg_LayerSize* sz;
    
    //Make sure layer is locked before accepting changes
    if (arg->id >= GFX_LCDC_LAYERS)
        return GFX_FAILURE;
    
    if (request == GFX_IOCTL_SET_LAYER_LOCK &&
        drvLayer[arg->id].updateLock == LAYER_UNLOCKED)
    {
        drvLayer[arg->id].updateLock = LAYER_LOCKED;
        
        return GFX_SUCCESS;
    }
    
    //Layer should be locked 
    if (drvLayer[arg->id].updateLock != LAYER_LOCKED)
        return GFX_FAILURE;
    
    if (request == GFX_IOCTL_SET_LAYER_UNLOCK)
    {
        drvLayer[arg->id].updateLock = LAYER_LOCKED_PENDING;
        
        LCDC_WaitForSyncInProgress();
        LCDC_SetSYNCEnable(true);
        
        //Wait for the changes to be applied
        while (drvLayer[arg->id].updateLock == LAYER_LOCKED_PENDING);
        
        return GFX_SUCCESS;
    }
    
    switch(request)
    {
        case GFX_IOCTL_SET_LAYER_ALPHA:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            drvLayer[arg->id].alpha = val->value.v_uint;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_SIZE:
        {
            sz = (gfxIOCTLArg_LayerSize*)arg;
            
            drvLayer[arg->id].sizex = sz->width;
            drvLayer[arg->id].sizey = sz->height;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_POSITION:
        {
            pos = (gfxIOCTLArg_LayerPosition*)arg;
            
            drvLayer[arg->id].startx = pos->x;
            drvLayer[arg->id].starty = pos->y;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_WINDOW_SIZE:
        {
            sz = (gfxIOCTLArg_LayerSize*)arg;
            
            drvLayer[arg->id].resx = sz->width;
            drvLayer[arg->id].resy = sz->height;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_BASE_ADDRESS:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            drvLayer[arg->id].baseaddr[0] = val->value.v_pointer;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_COLOR_MODE:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            drvLayer[arg->id].colorspace = getLCDCColorModeFromGFXColorMode(val->value.v_colormode);
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_LAYER_ENABLED:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            val->value.v_bool = drvLayer[arg->id].enabled;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_LAYER_ENABLED:
        {
            val = (gfxIOCTLArg_LayerValue*)arg;
            
            drvLayer[arg->id].enabled = val->value.v_bool;
            
            return GFX_IOCTL_OK;
        }
        default:
            break;
    }
    
    return GFX_IOCTL_UNSUPPORTED;
}

gfxDriverIOCTLResponse DRV_LCDC_IOCTL(gfxDriverIOCTLRequest request,
                                      void* arg)
{
    gfxIOCTLArg_Value* val;
    gfxIOCTLArg_DisplaySize* disp;
    gfxIOCTLArg_LayerRect* rect;
    
    switch(request)
    {
        case GFX_IOCTL_GET_BUFFER_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = BUFFER_PER_LAYER;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_DISPLAY_SIZE:
        {
            disp = (gfxIOCTLArg_DisplaySize*)arg;            
            
            disp->width = DISPLAY_WIDTH;
            disp->height = DISPLAY_HEIGHT;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_LAYER_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = GFX_LCDC_LAYERS;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = activeLayer;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            if(val->value.v_uint >= GFX_LCDC_LAYERS)
            {
                return GFX_IOCTL_ERROR_UNKNOWN;
            }
            else
            {
                activeLayer = val->value.v_uint;
                
                return GFX_IOCTL_OK;
            }
        }
        case GFX_IOCTL_GET_LAYER_RECT:
        {
            rect = (gfxIOCTLArg_LayerRect*)arg;
            
            if(rect->base.id >= GFX_LCDC_LAYERS)        
                return GFX_IOCTL_ERROR_UNKNOWN;
            
            rect->x = drvLayer[rect->base.id].startx;
            rect->y = drvLayer[rect->base.id].starty;
            rect->width = drvLayer[rect->base.id].sizex;
            rect->height = drvLayer[rect->base.id].sizey;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_VSYNC_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = vsyncCount;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_FRAMEBUFFER:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_pbuffer = &pixelBuffer[activeLayer];
                        
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_ENABLE_GPU:
        {
            val = (gfxIOCTLArg_Value*)arg;

            useGPU = val->value.v_bool;

            return GFX_IOCTL_OK;
        }
        default:
        {
            if (request >= GFX_IOCTL_LAYER_REQ_START && 
                request < GFX_IOCTL_LAYER_REQ_END)
            {
                return _layerConfig(request, (gfxIOCTLArg_LayerArg*)arg);
            }
        }
    }
    
    return GFX_IOCTL_UNSUPPORTED;
}

/*******************************************************************************
 End of File
*/
