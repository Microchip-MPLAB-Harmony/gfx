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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "peripheral/evic/plib_evic.h"
#include "gfx/driver/controller/glcd/plib_glcd.h"
#include "gfx/driver/controller/glcd/drv_gfx_glcd.h"
#include "definitions.h"
#include "gfx/driver/processor/2dgpu/libnano2d.h"

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

const char* DRIVER_NAME = "GLCD";
//static uint32_t supported_color_format = (GFX_COLOR_MASK_GS_8 |
//                                          GFX_COLOR_MASK_RGB_565 |
//                                          GFX_COLOR_MASK_RGBA_8888);

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
static uint32_t state;
<#if DoubleBuffer == true>
static gfxBool useGPU = true;
<#else>
static gfxBool useGPU = false;
</#if>
static unsigned int vsyncCount = 0;
static gfxPixelBuffer pixelBuffer;
static gfxRect srcRect, destRect;

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
} DISPLAY_LAYER;
static DISPLAY_LAYER drvLayer[GFX_GLCD_LAYERS];


/**** Hardware Abstraction Interfaces ****/
enum
{
    INIT = 0,
    RUN
};

static int DRV_GFX_GLCD_Start();

volatile gfxBool waitingForVSync;

void DRV_GLCD_Update()
{
    if(state == INIT)
    {
        if(DRV_GFX_GLCD_Start() != 0)
            return ;

        state = RUN;
    }
}

static uint32_t getColorModeStrideSize(GLCD_LAYER_COLOR_MODE mode)
{
<#if FrameBufferColorMode == "GS_8">
	return 1;
<#elseif FrameBufferColorMode == "RGB_565">
	return 2;
<#elseif FrameBufferColorMode == "RGB_888">
	return 3;
<#elseif FrameBufferColorMode == "RGBA_8888" ||
         FrameBufferColorMode == "RGBA_8888" ||
         FrameBufferColorMode == "ARGB_8888">
	return 4;
</#if>
}

<#if PaletteMode == true>
static gfxResult globalPaletteSet(GFX_GlobalPalette palette)
{
    uint32_t lut[GFX_GLOBAL_PALETTE_SIZE];
    uint32_t colorIndex = 0;
    uint32_t* pal;
	
    if (palette == NULL)
        return GFX_FAILURE;

	defGlobalPaletteSet(palette);

    for( colorIndex = 0; colorIndex < GFX_GLOBAL_PALETTE_SIZE; colorIndex++ )
    {
        pal = (uint32_t*)palette;
        lut[colorIndex] = GFX_ColorConvert(GFX_COLOR_MODE_RGBA_8888, GFX_COLOR_MODE_RGB_888, pal[colorIndex]);
    }

    PLIB_GLCD_GlobalColorLUTSet(GLCD_ID_0, lut );
    PLIB_GLCD_PaletteGammaRampEnable(GLCD_ID_0);

        return GFX_SUCCESS;
}
</#if>

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
    uint32_t      bufferCount;

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
    PLIB_GLCD_PaletteGammaRampDisable();

    PLIB_GLCD_Enable();

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

        //Clear frame buffer
        for(bufferCount = 0; bufferCount < BUFFER_PER_LAYER; ++bufferCount)
        {
            memset(drvLayer[layerCount].baseaddr[bufferCount], 0, sizeof(FRAMEBUFFER_PIXEL_TYPE) * DISPLAY_WIDTH * DISPLAY_HEIGHT);
        }
        
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

        gfxPixelBufferCreate(xResolution,
                    yResolution,
                    DRV_GLCD_GetColorMode(),
                    drvLayer[0].baseaddr[0],
                    &pixelBuffer);

        // all layers off by default
        //context->layer.layers[layerCount].enabled = GFX_FALSE;
    }

    EVIC_SourceStatusClear(INT_SOURCE_GLCD);
    EVIC_SourceEnable(INT_SOURCE_GLCD);

}

gfxPixelBuffer * DRV_GLCD_GetFrameBuffer(int32_t idx)
{
    return &pixelBuffer;
}

void GLCD_Interrupt_Handler(void)
{
	// disable vsync interrupt
    //PLIB_GLCD_VSyncInterruptDisable();

    // clear interrupt flag
    //EVIC_SourceStatusClear(INT_SOURCE_GLCD);

    //_2dgpuGraphicsProcessor.blitBuffer(&scratchBuffer, &srcRect, &pixelBuffer, &destRect, blendMode );
    
    //waitingForVSync = GFX_FALSE;
}

/**** End Hardware Abstraction Interfaces ****/


static int DRV_GFX_GLCD_Start()
{
    return 0;
}

gfxColorMode DRV_GLCD_GetColorMode()
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

uint32_t DRV_GLCD_GetBufferCount()
{
    return 1;
}

uint32_t DRV_GLCD_GetDisplayWidth()
{
    return 480;
}

uint32_t DRV_GLCD_GetDisplayHeight()
{
    return 272;
}

uint32_t DRV_GLCD_GetLayerCount()
{
	return 1;
}

uint32_t DRV_GLCD_GetActiveLayer()
{
	return 0;
}

gfxResult DRV_GLCD_SetActiveLayer(uint32_t idx)
{
	return GFX_SUCCESS;
}

void DRV_GLCD_Swap(void)
{

}

uint32_t DRV_GLCD_GetVSYNCCount(void)
{
	return vsyncCount;
}

void DRV_GLCD_SetUseGPU(gfxBool onOff)
{
	useGPU = onOff;
}

gfxResult DRV_GLCD_BlitBuffer(int32_t x,
                             int32_t y,
                             gfxPixelBuffer* buf,
                             gfxBlend blend)
{
    if (state != RUN)
        return GFX_FAILURE;

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

        _2dgpuGraphicsProcessor.blitBuffer(buf, &srcRect, &pixelBuffer, &destRect, blend );
    }
    else
    {
    	void* srcPtr;
    	void* destPtr;
    	uint32_t row, rowSize;

        rowSize = buf->size.width * gfxColorInfoTable[buf->mode].size;

    	for(row = 0; row < buf->size.height; row++)
    	{
        	srcPtr = gfxPixelBufferOffsetGet(buf, 0, row);
        	destPtr = gfxPixelBufferOffsetGet(&pixelBuffer, x, y + row);

        	memcpy(destPtr, srcPtr, rowSize);
    	}
    }

    return GFX_SUCCESS;
}

/*******************************************************************************
 End of File
*/
