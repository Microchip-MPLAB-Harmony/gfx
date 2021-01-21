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
/********************************************************************************
  GFX GLCD Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_glcd.c.ftl

  Summary:
    Source code for the GFX GLCD driver static implementation.

  Description:
    This file contains the source code for the static implementation of the
    GFX GLCD driver.
*******************************************************************************/


<#if gfx_hal_le??>

<#assign Val_Width = gfx_hal_le.DisplayWidth>
<#assign Val_Height = gfx_hal_le.DisplayHeight>
<#assign Val_UseReset = gfx_hal_le.DisplayUseReset>
<#assign Val_ResetPolarity = gfx_hal_le.DisplayResetPolarity>
<#assign Val_UseChipSelect = gfx_hal_le.DisplayUseChipSelect>
<#assign Val_ChipSelectPolarity = gfx_hal_le.DisplayChipSelectPolarity>
<#assign Val_BacklightEnable = gfx_hal_le.DisplayBacklightEnable>
<#assign Val_VSYNCNegative = !gfx_hal_le.DisplayVSYNCNegative>
<#assign Val_HSYNCNegative = !gfx_hal_le.DisplayHSYNCNegative>
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
<#assign Val_UseReset = DisplayUseReset>
<#assign Val_ResetPolarity = DisplayResetPolarity>
<#assign Val_UseChipSelect = DisplayUseChipSelect>
<#assign Val_ChipSelectPolarity = DisplayChipSelectPolarity>
<#assign Val_BacklightEnable = DisplayBacklightEnable>
<#assign Val_VSYNCNegative = !DisplayVSYNCNegative>
<#assign Val_HSYNCNegative = !DisplayHSYNCNegative>
<#assign Val_UseDataEnable = DisplayDataEnable>
<#assign Val_DataEnablePolarity = DisplayDataEnablePolarity>
<#assign Val_HorzFrontPorch = DisplayHorzFrontPorch>
<#assign Val_HorzBackPorch = DisplayHorzBackPorch>
<#assign Val_HorzPulseWidth = DisplayHorzPulseWidth>
<#assign Val_VertFrontPorch = DisplayVertFrontPorch>
<#assign Val_VertBackPorch = DisplayVertBackPorch>
<#assign Val_VertPulseWidth = DisplayVertPulseWidth>

</#if>

<#if UseGPU != true>
<#assign VblankBlit = false>
</#if>

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "peripheral/evic/plib_evic.h"
#include "gfx/driver/gfx_driver.h"
#include "gfx/driver/controller/glcd/plib_glcd.h"
#include "gfx/driver/controller/glcd/drv_gfx_glcd.h"
#include "definitions.h"
<#if UseGPU == true && le_gfx_driver_2dgpu??>
#include "gfx/driver/processor/2dgpu/libnano2d.h"
</#if>
// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

<#if DoubleBuffer == true>
#define BUFFER_PER_LAYER    2
<#else>
#define BUFFER_PER_LAYER    1
</#if>

#define DISPLAY_WIDTH  ${Val_Width}
#define DISPLAY_HEIGHT ${Val_Height}
#define GFX_GLCD_LAYERS ${TotalNumLayers}
#define GFX_GLCD_BACKGROUND_COLOR 0xFFFFFF00
#define GFX_GLCD_CONFIG_CONTROL 0x80000000
#define GFX_GLCD_CONFIG_CLK_DIVIDER ${PixelClockDivider}
#define GFX_GLCD_GLOBAL_PALETTE_COLOR_COUNT 256

#define SYNC_RECT_COUNT 200

<#if CanvasModeOnly == false>
<#if FrameBufferMemoryMode == "DDR">
<#if Layer0Enable == true>
/*** GLCD Layer 0 Configuration ***/
#define  GFX_GLCD_LAYER0_BASEADDR                      ${Layer0Buffer0}
#define  GFX_GLCD_LAYER0_DBL_BASEADDR                  ${Layer0Buffer1}
</#if>
<#if Layer1Enable == true>
/*** GLCD Layer 1 Configuration ***/
#define  GFX_GLCD_LAYER1_BASEADDR                      ${Layer1Buffer0}
#define  GFX_GLCD_LAYER1_DBL_BASEADDR                  ${Layer1Buffer1}
</#if>
<#if Layer2Enable == true>
/*** GLCD Layer 2 Configuration ***/
#define  GFX_GLCD_LAYER2_BASEADDR                      ${Layer2Buffer0}
#define  GFX_GLCD_LAYER2_DBL_BASEADDR                  ${Layer2Buffer1}
</#if>
</#if>
</#if>

<#if FrameBufferColorMode == "GS_8">
#define LCDC_DEFAULT_GFX_COLOR_MODE GLCD_LAYER_COLOR_MODE_LUT8
#define FRAMEBUFFER_PTR_TYPE    uint8_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint8_t
<#elseif FrameBufferColorMode == "RGB_565">
#define LCDC_DEFAULT_GFX_COLOR_MODE GLCD_LAYER_COLOR_MODE_RGB565
#define FRAMEBUFFER_PTR_TYPE    uint16_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint16_t
<#elseif FrameBufferColorMode == "RGBA_8888">
#define LCDC_DEFAULT_GFX_COLOR_MODE GLCD_LAYER_COLOR_MODE_RGBA8888
#define FRAMEBUFFER_PTR_TYPE    uint32_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint32_t
<#else>
//Unsupported  framebuffer type specified, default to RGBA8888
#define LCDC_DEFAULT_GFX_COLOR_MODE GLCD_LAYER_COLOR_MODE_RGBA8888
#define FRAMEBUFFER_PTR_TYPE      uint32_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint32_t
</#if>

typedef enum
{
    LAYER_UNLOCKED,
    LAYER_LOCKED,
    LAYER_LOCKED_PENDING,
} LAYER_LOCK_STATUS;

/**** Hardware Abstraction Interfaces ****/
typedef enum
{
    INIT = 0,
    DRAW,
    SWAP,
    SYNC,
} DRV_STATE;

const char* DRIVER_NAME = "GLCD";
//static uint32_t supported_color_format = (GFX_COLOR_MASK_GS_8 |
//                                          GFX_COLOR_MASK_RGB_565 |
//                                          GFX_COLOR_MASK_RGBA_8888);

<#if CanvasModeOnly == false>
<#if FrameBufferMemoryMode == "INT_SRAM">
<#list 0..(TotalNumLayers-1) as i>
FRAMEBUFFER_PIXEL_TYPE  __attribute__ ((coherent, aligned (32))) framebuffer_${i}[DISPLAY_WIDTH * DISPLAY_HEIGHT];
</#list>

<#if DoubleBuffer == true>
<#list 0..(TotalNumLayers-1) as i>
FRAMEBUFFER_PIXEL_TYPE  __attribute__ ((coherent, aligned (32))) framebuffer1_${i}[DISPLAY_WIDTH * DISPLAY_HEIGHT];
</#list>
</#if>
</#if>
</#if>

static volatile DRV_STATE state;
<#if UseGPU == true && le_gfx_driver_2dgpu??>
static gfxRect srcRect, destRect;
<#if VblankBlit == true>
volatile gfxPixelBuffer* blitBuff = NULL;
static unsigned int blitLayer = 0;
</#if>
</#if>
static unsigned int vsyncCount = 0;
static unsigned int activeLayer = 0;
<#if DoubleBuffer == true>
static volatile unsigned int syncLayer = 0;
</#if>

volatile int32_t waitForAlphaSetting[GFX_GLCD_LAYERS] = {0};

//Layer Parameters
//--------------------------------------------------------------------------
typedef struct __display_layer {
    FRAMEBUFFER_PTR_TYPE  baseaddr[BUFFER_PER_LAYER];
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
    uint32_t   dblend;
    uint32_t   sblend;
    uint32_t   colorspace;
    uint16_t   color;
    bool enabled;
    volatile LAYER_LOCK_STATUS updateLock;
    gfxPixelBuffer pixelBuffer[BUFFER_PER_LAYER];
    volatile unsigned int frontBufferIdx;
    volatile unsigned int backBufferIdx;
<#if DoubleBuffer == true> 
    gfxRect syncRect[SYNC_RECT_COUNT + 1];
    volatile unsigned int syncRectIndex;
    volatile bool swapPending;
</#if>
} DISPLAY_LAYER;
static DISPLAY_LAYER drvLayer[GFX_GLCD_LAYERS];

static gfxResult DRV_GLCD_UpdateLayer(unsigned int layer);

void DRV_GLCD_Update()
{
    switch(state)
    {
        case INIT:
        {
<#if DoubleBuffer == true>
            syncLayer = 0;
</#if>
            state = DRAW;
            break;
        }
<#if DoubleBuffer == true>
        case SYNC:
        {
<#if VblankBlit != true>
            if (drvLayer[syncLayer].syncRectIndex == 0)
            {
                syncLayer++;
            }
<#if UseGPU == true && le_gfx_driver_2dgpu??>
            else
            {
                drvLayer[syncLayer].syncRectIndex--;

                gfxGPUInterface.blitBuffer(&drvLayer[syncLayer].pixelBuffer[drvLayer[syncLayer].frontBufferIdx],
                                 &drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex],
                                 &drvLayer[syncLayer].pixelBuffer[drvLayer[syncLayer].backBufferIdx],
                                 &drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex]);
            }
<#else>
            else
            {
                void* srcPtr;
                void* destPtr;
                uint32_t row, rowSize;

                drvLayer[syncLayer].syncRectIndex--;

                rowSize = drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex].width * 
                          gfxColorInfoTable[drvLayer[syncLayer].pixelBuffer[drvLayer[syncLayer].frontBufferIdx].mode].size;

                for(row = 0; row < drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex].height; row++)
                {
                    srcPtr = gfxPixelBufferOffsetGet(&drvLayer[syncLayer].pixelBuffer[drvLayer[syncLayer].frontBufferIdx],
                                        drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex].x,
                                        drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex].y + row);
                    destPtr = gfxPixelBufferOffsetGet(&drvLayer[syncLayer].pixelBuffer[drvLayer[syncLayer].backBufferIdx],
                                        drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex].x,
                                        drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex].y + row);

                    memcpy(destPtr, srcPtr, rowSize);
                }
            }
</#if>
            
            if (syncLayer == GFX_GLCD_LAYERS)
            {
                syncLayer = 0;
                state = DRAW;
            }
</#if>

            break;
        }
</#if>
        case DRAW:
        case SWAP:
        default:
            break;
    }
}

static uint32_t getColorModeStrideSize(GLCD_LAYER_COLOR_MODE mode)
{
    switch(mode)
    {
        case GLCD_LAYER_COLOR_MODE_LUT8:
            return sizeof(uint8_t);
            break;
        case GLCD_LAYER_COLOR_MODE_RGB332:
            return sizeof(uint8_t);
            break;
        case GLCD_LAYER_COLOR_MODE_RGB565:
            return sizeof(uint16_t);
            break;
        case GLCD_LAYER_COLOR_MODE_RGB888:
            return sizeof(uint32_t);
            break;
        case GLCD_LAYER_COLOR_MODE_RGBA8888:
            return sizeof(uint32_t);
            break;
        case GLCD_LAYER_COLOR_MODE_ARGB8888:
            return sizeof(uint32_t);
            break;
        case GLCD_LAYER_COLOR_MODE_L1:
            return sizeof(uint8_t);
            break;
        case GLCD_LAYER_COLOR_MODE_L4:
            return sizeof(uint8_t);
            break;
        case GLCD_LAYER_COLOR_MODE_L8:
            return sizeof(uint8_t);
            break;
        default:
            return sizeof(uint32_t);
            break;
    }
}

static GLCD_LAYER_COLOR_MODE getGLCDColorModeFromGFXColorMode(gfxColorMode mode)
{
    switch(mode)
    {
        case GFX_COLOR_MODE_GS_8:
            return GLCD_LAYER_COLOR_MODE_LUT8;
        case GFX_COLOR_MODE_RGB_332:
            return GLCD_LAYER_COLOR_MODE_RGB332;
        case GFX_COLOR_MODE_RGB_565:
            return GLCD_LAYER_COLOR_MODE_RGB565;
        case GFX_COLOR_MODE_RGBA_5551:
            return GLCD_LAYER_COLOR_MODE_RGBA5551;
        case GFX_COLOR_MODE_RGB_888:
            return GLCD_LAYER_COLOR_MODE_RGB888;
        case GFX_COLOR_MODE_ARGB_8888:
            return GLCD_LAYER_COLOR_MODE_ARGB8888;
        case GFX_COLOR_MODE_INDEX_1:
            return GLCD_LAYER_COLOR_MODE_L1;
        case GFX_COLOR_MODE_INDEX_4:
            return GLCD_LAYER_COLOR_MODE_L4;
        case GFX_COLOR_MODE_INDEX_8:
            return GLCD_LAYER_COLOR_MODE_L8;
        case GFX_COLOR_MODE_RGBA_8888:
        default:
            return GLCD_LAYER_COLOR_MODE_RGBA8888;
    }
}

static gfxColorMode getGFXColorModeFromGLCD(GLCD_LAYER_COLOR_MODE mode)
{
    switch(mode)
    {
        case GLCD_LAYER_COLOR_MODE_LUT8:
            return GFX_COLOR_MODE_GS_8;
        case GLCD_LAYER_COLOR_MODE_RGB332:
            return GFX_COLOR_MODE_RGB_332;
        case GLCD_LAYER_COLOR_MODE_RGB565:
            return GFX_COLOR_MODE_RGB_565;
        case GLCD_LAYER_COLOR_MODE_RGBA5551:
            return GFX_COLOR_MODE_RGBA_5551;
        case GLCD_LAYER_COLOR_MODE_RGB888:
            return GFX_COLOR_MODE_RGB_888;
        case GLCD_LAYER_COLOR_MODE_ARGB8888:
            return GFX_COLOR_MODE_ARGB_8888;
        case GLCD_LAYER_COLOR_MODE_L1:
            return GFX_COLOR_MODE_INDEX_1;
        case GLCD_LAYER_COLOR_MODE_L4:
            return GFX_COLOR_MODE_INDEX_4;
        case GLCD_LAYER_COLOR_MODE_L8:
            return GFX_COLOR_MODE_INDEX_8;
        default:
            return GFX_COLOR_MODE_RGBA_8888;
    }
}

void DRV_GLCD_Initialize()
{
    uint32_t      xResolution;
    uint32_t      yResolution;
    uint32_t      rightMargin;
    uint32_t      lowerMargin;
    uint32_t      hsyncLength;
    uint32_t      vsyncLength;
    uint32_t      leftMargin;
    uint32_t      upperMargin;
    uint32_t      stride;
    uint32_t      layerCount;
<#if CanvasModeOnly == false>
    uint32_t      bufferCount;
</#if>

    // general default initialization
    //if(defInitialize(context) == LE_FAILURE)
    //        return LE_FAILURE;


    /* set temporary information */
    xResolution     = ${Val_Width};
    yResolution     = ${Val_Height};
    rightMargin     = ${Val_HorzFrontPorch};
    leftMargin      = ${Val_HorzBackPorch};
    hsyncLength     = ${Val_HorzPulseWidth};
    vsyncLength     = ${Val_VertPulseWidth};
    upperMargin     = ${Val_VertBackPorch};
    lowerMargin     = ${Val_VertPulseWidth};


    /* glcd initialization */
    PLIB_GLCD_Disable();
    PLIB_GLCD_BackgroundColorSet(GFX_GLCD_BACKGROUND_COLOR);
    PLIB_GLCD_VSyncInterruptDisable();
    PLIB_GLCD_HSyncInterruptDisable();
    PLIB_GLCD_RGBSequentialModeSet(1<<31);

    PLIB_GLCD_FrontPorchXYSet(xResolution + rightMargin, yResolution + lowerMargin);
    PLIB_GLCD_BlankingXYSet(xResolution + rightMargin + hsyncLength, yResolution + lowerMargin + vsyncLength);
    PLIB_GLCD_BackPorchXYSet(xResolution + rightMargin + hsyncLength + leftMargin, yResolution + lowerMargin + vsyncLength + upperMargin);

    PLIB_GLCD_ClockDividerSet(GFX_GLCD_CONFIG_CLK_DIVIDER);
    PLIB_GLCD_ResolutionXYSet(xResolution, yResolution);

    <#if Val_VSYNCNegative == true && Val_HSYNCNegative == false>
    PLIB_GLCD_SignalPolaritySet( GLCD_VSYNC_POLARITY_NEGATIVE );
    </#if>
    <#if Val_HSYNCNegative == true && Val_VSYNCNegative == false>
    PLIB_GLCD_SignalPolaritySet( GLCD_HSYNC_POLARITY_NEGATIVE );
    </#if>
    <#if Val_HSYNCNegative == true && Val_VSYNCNegative == true>
    PLIB_GLCD_SignalPolaritySet( GLCD_VSYNC_POLARITY_NEGATIVE | GLCD_HSYNC_POLARITY_NEGATIVE );
    </#if>

<#if PaletteMode == true>
    PLIB_GLCD_PaletteGammaRampEnable();
<#else>
    PLIB_GLCD_PaletteGammaRampDisable();
</#if>

    PLIB_GLCD_Enable();

<#if CanvasModeOnly == false>
<#if FrameBufferMemoryMode == "INT_SRAM">
    <#list 0..(TotalNumLayers-1) as i>
    drvLayer[${i}].baseaddr[0] = framebuffer_${i};
    </#list>
    <#if DoubleBuffer == true>
    <#list 0..(TotalNumLayers-1) as i>
    drvLayer[${i}].baseaddr[1] = framebuffer1_${i};
    </#list>
</#if>
<#else>
    <#list 0..(TotalNumLayers-1) as i>
    <#if DoubleBuffer == true>
    drvLayer[${i}].baseaddr[0] = (FRAMEBUFFER_PTR_TYPE)GFX_GLCD_LAYER${i}_BASEADDR;
    drvLayer[${i}].baseaddr[1] = (FRAMEBUFFER_PTR_TYPE)GFX_GLCD_LAYER${i}_DBL_BASEADDR;
    <#else>
    drvLayer[${i}].baseaddr[0] = (FRAMEBUFFER_PTR_TYPE)GFX_GLCD_LAYER${i}_BASEADDR;
    </#if>
    </#list>
</#if>
</#if>

    for (layerCount = 0; layerCount < GFX_GLCD_LAYERS; layerCount++)
    {
        drvLayer[layerCount].resx       = xResolution;
        drvLayer[layerCount].resy       = yResolution;
        drvLayer[layerCount].startx     = 0;
        drvLayer[layerCount].starty     = 0;
        drvLayer[layerCount].sizex      = drvLayer[layerCount].resx;
        drvLayer[layerCount].sizey      = drvLayer[layerCount].resy;
        drvLayer[layerCount].alpha      = 255;
        drvLayer[layerCount].dblend     = GLCD_LAYER_DEST_BLEND_INV_SRCGBL;
        drvLayer[layerCount].sblend     = GLCD_LAYER_SRC_BLEND_ALPHA_SRCGBL;
        drvLayer[layerCount].colorspace = LCDC_DEFAULT_GFX_COLOR_MODE;
        drvLayer[layerCount].enabled    = true;
        drvLayer[layerCount].updateLock = LAYER_LOCKED;
<#if CanvasModeOnly == false>
        //Clear frame buffer
        for(bufferCount = 0; bufferCount < BUFFER_PER_LAYER; ++bufferCount)
        {
            memset(drvLayer[layerCount].baseaddr[bufferCount], 0, sizeof(FRAMEBUFFER_PIXEL_TYPE) * DISPLAY_WIDTH * DISPLAY_HEIGHT);
        }
</#if>
        stride = getColorModeStrideSize(drvLayer[layerCount].colorspace);

        PLIB_GLCD_LayerBaseAddressSet(layerCount, (uint32_t)drvLayer[layerCount].baseaddr[0]);
        PLIB_GLCD_LayerStrideSet(layerCount, drvLayer[layerCount].resx * stride );
        PLIB_GLCD_LayerResXYSet(layerCount, drvLayer[layerCount].resx, drvLayer[layerCount].resy );
        PLIB_GLCD_LayerStartXYSet(layerCount, drvLayer[layerCount].startx, drvLayer[layerCount].starty );
        PLIB_GLCD_LayerSizeXYSet(layerCount, drvLayer[layerCount].sizex, drvLayer[layerCount].sizey);
        PLIB_GLCD_LayerGlobalAlphaSet(layerCount, drvLayer[layerCount].alpha);
        PLIB_GLCD_LayerDestBlendFuncSet(layerCount, drvLayer[layerCount].dblend );
        PLIB_GLCD_LayerSrcBlendFuncSet(layerCount, drvLayer[layerCount].sblend );
        PLIB_GLCD_LayerColorModeSet(layerCount, drvLayer[layerCount].colorspace );

        PLIB_GLCD_LayerEnable(layerCount);

        drvLayer[layerCount].frontBufferIdx = 0;
<#if DoubleBuffer != true>
        drvLayer[layerCount].backBufferIdx = 0;
</#if>
        drvLayer[layerCount].updateLock = LAYER_UNLOCKED;

        gfxPixelBufferCreate(xResolution,
                             yResolution,
                             getGFXColorModeFromGLCD(LCDC_DEFAULT_GFX_COLOR_MODE),
                             drvLayer[layerCount].baseaddr[0],
                             &drvLayer[layerCount].pixelBuffer[drvLayer[layerCount].frontBufferIdx]);

<#if DoubleBuffer == true>
        drvLayer[layerCount].backBufferIdx = 1;
        gfxPixelBufferCreate(xResolution,
                            yResolution,
                            getGFXColorModeFromGLCD(LCDC_DEFAULT_GFX_COLOR_MODE),
                            drvLayer[layerCount].baseaddr[1],
                            &drvLayer[layerCount].pixelBuffer[drvLayer[layerCount].backBufferIdx]);
        
        drvLayer[layerCount].syncRectIndex = 0;
        drvLayer[layerCount].swapPending = false;
</#if>

    }

    EVIC_SourceStatusClear(INT_SOURCE_GLCD);
    EVIC_SourceEnable(INT_SOURCE_GLCD);

}



void GLCD_Interrupt_Handler(void)
{
    unsigned int i = 0;
    
    EVIC_SourceStatusClear(INT_SOURCE_GLCD);

    PLIB_GLCD_VSyncInterruptDisable();

<#if UseGPU == true && le_gfx_driver_2dgpu?? && VblankBlit == true>
    if (blitBuff != NULL)
    {
<#if DoubleBuffer == true> 
    gfxGPUInterface.blitBuffer((const gfxPixelBuffer*) blitBuff,
                                   &srcRect,
                                   &drvLayer[blitLayer].pixelBuffer[drvLayer[blitLayer].backBufferIdx],
                                   &destRect);
<#else>
    gfxGPUInterface.blitBuffer((const gfxPixelBuffer*) blitBuff,
                                   &srcRect,
                                   &drvLayer[blitLayer].pixelBuffer[drvLayer[blitLayer].frontBufferIdx],
                                   &destRect);
</#if>

        gfxPixelBuffer_SetLocked((gfxPixelBuffer*) blitBuff,
                                 LE_FALSE);

        blitBuff = NULL;        
    }
</#if>
    
    //Update GLCD during blanking period
    for (i = 0; i < GFX_GLCD_LAYERS; i++)
    {
<#if DoubleBuffer == true>     
        switch (state)
        {
            case SWAP:
            {
                if (drvLayer[i].swapPending == false)
                    break;
        
                drvLayer[i].frontBufferIdx = drvLayer[i].backBufferIdx;
                drvLayer[i].backBufferIdx = (drvLayer[i].backBufferIdx == 0) ? 1 : 0;

                PLIB_GLCD_LayerBaseAddressSet(i,
                                            (uint32_t) drvLayer[i].baseaddr[drvLayer[i].frontBufferIdx]);

                drvLayer[i].swapPending = false;
                
                break;
            }
<#if UseGPU == true && le_gfx_driver_2dgpu?? && VblankBlit == true>
            case SYNC:
            {
                if (drvLayer[syncLayer].syncRectIndex == 0)
                {
                    syncLayer++;
                }
                else
                {
                    drvLayer[syncLayer].syncRectIndex--;

                    gfxGPUInterface.blitBuffer(&drvLayer[syncLayer].pixelBuffer[drvLayer[syncLayer].frontBufferIdx],
                                     &drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex],
                                     &drvLayer[syncLayer].pixelBuffer[drvLayer[syncLayer].backBufferIdx],
                                     &drvLayer[syncLayer].syncRect[drvLayer[syncLayer].syncRectIndex]);
                }

                if (syncLayer == GFX_GLCD_LAYERS)
                {
                    syncLayer = 0;
                    state = DRAW;
                }
                
                PLIB_GLCD_VSyncInterruptEnable();
                
                break;
            }
</#if>
            default:
                break;
        }
</#if>

        if (drvLayer[i].updateLock == LAYER_LOCKED_PENDING)
        {
            DRV_GLCD_UpdateLayer(i);
            drvLayer[i].updateLock = LAYER_UNLOCKED;
        }
    }

<#if DoubleBuffer == true> 
    if (state == SWAP)
    {
<#if UseGPU == true && le_gfx_driver_2dgpu?? && VblankBlit == true>
        PLIB_GLCD_VSyncInterruptEnable(); 
</#if>
        state = SYNC;
    }
</#if>
}

/**** End Hardware Abstraction Interfaces ****/

gfxResult DRV_GLCD_BlitBuffer(int32_t x,
                             int32_t y,
                             gfxPixelBuffer* buf)
{
<#if UseGPU == false || !le_gfx_driver_2dgpu??>
    void* srcPtr;
    void* destPtr;
    uint32_t row, rowSize;
</#if>

    if (state != DRAW)
        return GFX_FAILURE;

<#if DoubleBuffer == true>
    if (drvLayer[activeLayer].syncRectIndex < SYNC_RECT_COUNT)
    {
        drvLayer[activeLayer].syncRect[drvLayer[activeLayer].syncRectIndex].x = x;
        drvLayer[activeLayer].syncRect[drvLayer[activeLayer].syncRectIndex].y = y;
        drvLayer[activeLayer].syncRect[drvLayer[activeLayer].syncRectIndex].width = buf->size.width;
        drvLayer[activeLayer].syncRect[drvLayer[activeLayer].syncRectIndex].height = buf->size.height;

        drvLayer[activeLayer].syncRectIndex++;
    }
    else
    {
        drvLayer[activeLayer].syncRect[SYNC_RECT_COUNT].x = 0;
        drvLayer[activeLayer].syncRect[SYNC_RECT_COUNT].y = 0;
        drvLayer[activeLayer].syncRect[SYNC_RECT_COUNT].width = 
                drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx].size.width;
        drvLayer[activeLayer].syncRect[SYNC_RECT_COUNT].height = 
                drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx].size.height;
    }
</#if>

<#if UseGPU == true && le_gfx_driver_2dgpu??>
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.height = buf->size.height;
    srcRect.width = buf->size.width;

    destRect.x = x;
    destRect.y = y;
    destRect.height = buf->size.height;
    destRect.width = buf->size.width;

<#if VblankBlit == true>
    gfxPixelBuffer_SetLocked(buf, LE_TRUE);
    
    blitBuff = buf;
    blitLayer = activeLayer;
    
    PLIB_GLCD_VSyncInterruptEnable();
<#else>
<#if DoubleBuffer == true> 
    gfxGPUInterface.blitBuffer(buf, &srcRect, &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx], &destRect);
<#else>
    gfxGPUInterface.blitBuffer(buf, &srcRect, &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx], &destRect);
</#if>
</#if>
<#else>
    rowSize = buf->size.width * gfxColorInfoTable[buf->mode].size;

    for(row = 0; row < buf->size.height; row++)
    {
        srcPtr = gfxPixelBufferOffsetGet(buf, 0, row);
        destPtr = gfxPixelBufferOffsetGet(&drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx], x, y + row);

        memcpy(destPtr, srcPtr, rowSize);
    }
</#if>

    return GFX_SUCCESS;
}

static gfxResult DRV_GLCD_UpdateLayer(unsigned int layer)
{
    PLIB_GLCD_LayerStrideSet(layer,
                             drvLayer[layer].resx *
                             getColorModeStrideSize(drvLayer[layer].colorspace));
        
    PLIB_GLCD_LayerBaseAddressSet(layer,
                                 (uint32_t)drvLayer[layer].baseaddr[0]);
                
    PLIB_GLCD_LayerStartXYSet(layer,
                              drvLayer[layer].startx,
                              drvLayer[layer].starty );
        
        //Set layer size
    PLIB_GLCD_LayerSizeXYSet(layer,
                             drvLayer[layer].sizex,
                             drvLayer[layer].sizey);
            
    PLIB_GLCD_LayerResXYSet(layer,
                            drvLayer[layer].sizex,
                            drvLayer[layer].sizey);
            
    PLIB_GLCD_LayerGlobalAlphaSet(layer,
                            drvLayer[layer].alpha);
        
    PLIB_GLCD_LayerColorModeSet(layer,
                            drvLayer[layer].colorspace);   
    
    if (drvLayer[layer].enabled == true)
        PLIB_GLCD_LayerEnable(layer);
    else
        PLIB_GLCD_LayerDisable(layer);
    
    return GFX_SUCCESS;
}

static gfxDriverIOCTLResponse DRV_GLCD_LayerConfig(gfxDriverIOCTLRequest request,
                                                   gfxIOCTLArg_LayerArg* arg)
{
    gfxIOCTLArg_LayerValue* val;
    gfxIOCTLArg_LayerPosition* pos;
    gfxIOCTLArg_LayerSize* sz;
    
    // make sure layer is locked before accepting changes
    if(arg->id >= GFX_GLCD_LAYERS)
        return GFX_IOCTL_ERROR_UNKNOWN;
    
    // attempt to lock
    if(request == GFX_IOCTL_SET_LAYER_LOCK)
    {
        PLIB_GLCD_VSyncInterruptDisable();
        
        drvLayer[arg->id].updateLock = LAYER_LOCKED;
        
        return GFX_IOCTL_OK;
    }
    
    // layer should be locked 
    if(drvLayer[arg->id].updateLock != LAYER_LOCKED)
        return GFX_IOCTL_ERROR_UNKNOWN;
    
    if(request == GFX_IOCTL_SET_LAYER_UNLOCK)
    {
        drvLayer[arg->id].updateLock = LAYER_LOCKED_PENDING;
        
        PLIB_GLCD_VSyncInterruptEnable();
        
        return GFX_IOCTL_OK;
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
            
            drvLayer[arg->id].resx = sz->width;
            drvLayer[arg->id].resy = sz->height;
            
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
            
            drvLayer[arg->id].sizex = sz->width;
            drvLayer[arg->id].sizey = sz->height;
            
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
            
            drvLayer[arg->id].colorspace = getGLCDColorModeFromGFXColorMode(val->value.v_colormode);
            
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

gfxDriverIOCTLResponse DRV_GLCD_SetPalette(gfxIOCTLArg_Palette* pal)
{
<#if PaletteMode == true>
    uint32_t lut[GFX_GLCD_GLOBAL_PALETTE_COLOR_COUNT];
    uint32_t colorIndex = 0;
    gfxPixelBuffer buffer;
    
    if (pal->palette == NULL)
        return GFX_IOCTL_ERROR_UNKNOWN;
    
    if (pal->colorCount > GFX_GLCD_GLOBAL_PALETTE_COLOR_COUNT)
        pal->colorCount = GFX_GLCD_GLOBAL_PALETTE_COLOR_COUNT;

    gfxPixelBufferCreate(pal->colorCount, 1, pal->mode, pal->palette, &buffer);
    
    for( colorIndex = 0;
         colorIndex < pal->colorCount;
         colorIndex++ )
    {
        lut[colorIndex] = gfxColorConvert(pal->mode,
                                          GFX_COLOR_MODE_RGB_888,
                                          gfxPixelBufferGetIndex(&buffer, colorIndex));
    }

    PLIB_GLCD_GlobalColorLUTSet(lut);

    return GFX_IOCTL_OK;
<#else>
    return GFX_IOCTL_UNSUPPORTED;
</#if>
}

gfxDriverIOCTLResponse DRV_GLCD_IOCTL(gfxDriverIOCTLRequest request,
                                      void* arg)
{
    gfxIOCTLArg_Value* val;
    gfxIOCTLArg_DisplaySize* disp;
    gfxIOCTLArg_LayerRect* rect;
    
    switch(request)
    {
        case GFX_IOCTL_LAYER_SWAP:
        {
<#if DoubleBuffer == true>
            if (drvLayer[activeLayer].syncRectIndex > 0)
                drvLayer[activeLayer].swapPending = true;
</#if>
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_FRAME_END:
        {
<#if DoubleBuffer == true>
            state = SWAP;
            PLIB_GLCD_VSyncInterruptEnable();
</#if>
            
            return GFX_IOCTL_OK;
        }
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
            
            val->value.v_uint = GFX_GLCD_LAYERS;
            
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
            
            if(val->value.v_uint >= GFX_GLCD_LAYERS)
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
            
            if(rect->base.id >= GFX_GLCD_LAYERS)        
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
            
            val->value.v_pbuffer = &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx];

            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_PALETTE:
        {
            return DRV_GLCD_SetPalette((gfxIOCTLArg_Palette*)arg);
        }
        default:
        {
            if (request >= GFX_IOCTL_LAYER_REQ_START && 
                request < GFX_IOCTL_LAYER_REQ_END)
            {
                return DRV_GLCD_LayerConfig(request, (gfxIOCTLArg_LayerArg*)arg);
            }
        }
    }
    
    return GFX_IOCTL_UNSUPPORTED;
}

/*******************************************************************************
 End of File
*/
