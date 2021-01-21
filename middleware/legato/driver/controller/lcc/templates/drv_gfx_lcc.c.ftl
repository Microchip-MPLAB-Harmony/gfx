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
  MPLAB Harmony LCC Generated Driver Implementation File

  File Name:
    drv_gfx_lcc_generic.c

  Summary:
    Build-time generated implementation for the LCC Driver for PIC32C MCUs.

  Description:
    Build-time generated implementation for the LCC Driver for PIC32C MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

<#if gfx_hal_le??>

<#assign Val_Width = gfx_hal_le.DisplayWidth>
<#assign Val_Height = gfx_hal_le.DisplayHeight>
<#assign Val_HorzFrontPorch = gfx_hal_le.DisplayHorzFrontPorch>
<#assign Val_HorzBackPorch = gfx_hal_le.DisplayHorzBackPorch>
<#assign Val_HorzPulseWidth = gfx_hal_le.DisplayHorzPulseWidth>
<#assign Val_VertFrontPorch = gfx_hal_le.DisplayVertFrontPorch>
<#assign Val_VertBackPorch = gfx_hal_le.DisplayVertBackPorch>
<#assign Val_VertPulseWidth = gfx_hal_le.DisplayVertPulseWidth>

<#else>

<#assign Val_Width = DisplayWidth>
<#assign Val_Height = DisplayHeight>
<#assign Val_HorzFrontPorch = DisplayHorzFrontPorch>
<#assign Val_HorzBackPorch = DisplayHorzBackPorch>
<#assign Val_HorzPulseWidth = DisplayHorzPulseWidth>
<#assign Val_VertFrontPorch = DisplayVertFrontPorch>
<#assign Val_VertBackPorch = DisplayVertBackPorch>
<#assign Val_VertPulseWidth = DisplayVertPulseWidth>

</#if>

#include "gfx/driver/controller/lcc/drv_gfx_lcc.h"
#include "definitions.h"

#include <string.h>

#define MAX_LAYER_COUNT 1
#define BUFFER_COUNT    1
#define DISPLAY_WIDTH   ${Val_Width}
#define DISPLAY_HEIGHT  ${Val_Height}

#define EBI_CS_INDEX  ${EBIChipSelectIndex}

<#if (EBIPeripheralType??) && (EBIPeripheralType == "EBIPMP")>
#define EBI_BASE_ADDR __KSEG2_EBI_DATA_MEM_BASE
<#else>
<#if EBIChipSelectIndex == 0>
#define EBI_BASE_ADDR  EBI_CS0_ADDR
<#elseif EBIChipSelectIndex == 1>
#define EBI_BASE_ADDR  EBI_CS1_ADDR
<#elseif EBIChipSelectIndex == 2>
#define EBI_BASE_ADDR  EBI_CS2_ADDR
<#elseif EBIChipSelectIndex == 3>
#define EBI_BASE_ADDR  EBI_CS3_ADDR
</#if>
</#if>

#define FRAMEBUFFER_TYPE uint16_t
#define FRAMEBUFFER_PIXEL_BYTES 2

<#if (UseCachedFrameBuffer??) && (UseCachedFrameBuffer == true)>
#define FRAMEBUFFER_ATTRIBUTE __attribute__((aligned(FRAMEBUFFER_PIXEL_BYTES*8)))
<#else>
#define FRAMEBUFFER_ATTRIBUTE __attribute__((coherent, aligned(FRAMEBUFFER_PIXEL_BYTES*8)))
</#if>

FRAMEBUFFER_TYPE FRAMEBUFFER_ATTRIBUTE frameBuffer[BUFFER_COUNT][DISPLAY_WIDTH * DISPLAY_HEIGHT];

<#if (DMAController??) && (DMAController == "DMAC")>
#define DRV_GFX_LCC_DMA_CHANNEL_INDEX DMAC_CHANNEL_${DMAChannel}
#define DRV_GFX_DMA_EVENT_TYPE DMAC_TRANSFER_EVENT
<#else>
#define DRV_GFX_LCC_DMA_CHANNEL_INDEX XDMAC_CHANNEL_${DMAChannel}
#define DRV_GFX_DMA_EVENT_TYPE XDMAC_TRANSFER_EVENT
</#if>

#ifndef GFX_DISP_INTF_PIN_RESET_Set
#error "GFX_DISP_INTF_PIN_RESET GPIO must be defined in the Pin Settings"
#endif

<#if PeripheralControl != "TC">
#ifndef GFX_DISP_INTF_PIN_BACKLIGHT_Set
#warning "GFX_DISP_INTF_PIN_BACKLIGHT GPIO must be defined in the Pin Settings"
#define GFX_DISP_INTF_PIN_BACKLIGHT_Set()
#endif
</#if>

#ifndef GFX_DISP_INTF_PIN_VSYNC_Set
#error "GFX_DISP_INTF_PIN_VSYNC GPIO must be defined in the Pin Settings"
#endif

#ifndef GFX_DISP_INTF_PIN_DE_Set
#error "GFX_DISP_INTF_PIN_DE GPIO must be defined in the Pin Settings"
#endif

#ifndef GFX_DISP_INTF_PIN_HSYNC_Set
#error "GFX_DISP_INTF_PIN_HSYNC GPIO must be defined in the Pin Settings"
#endif

/**** Hardware Abstraction Interfaces ****/
enum
{
    INIT = 0,
    RUN
};

gfxPixelBuffer pixelBuffer;

static int DRV_GFX_LCC_Start();
static void DRV_GFX_LCC_DisplayRefresh(void);
void dmaIntHandler (DRV_GFX_DMA_EVENT_TYPE status,
                    uintptr_t contextHandle);

uint16_t HBackPorch;
uint32_t VER_BLANK;

uint32_t DISP_HOR_FRONT_PORCH = ${Val_HorzFrontPorch};
uint32_t DISP_HOR_RESOLUTION = DISPLAY_WIDTH;
uint32_t DISP_HOR_BACK_PORCH = ${Val_HorzBackPorch};
uint32_t DISP_HOR_PULSE_WIDTH = ${Val_HorzPulseWidth};

uint32_t DISP_VER_FRONT_PORCH = ${Val_VertFrontPorch};
uint32_t DISP_VER_RESOLUTION = DISPLAY_HEIGHT;
uint32_t DISP_VER_BACK_PORCH = ${Val_VertBackPorch};
uint32_t DISP_VER_PULSE_WIDTH = ${Val_VertPulseWidth};

int16_t line = 0;
uint32_t offset = 0;
uint16_t pixels = 0;
uint32_t hSyncs = 0;
    
uint32_t vsyncPeriod = 0;
uint32_t vsyncPulseDown = 0;
uint32_t vsyncPulseUp = 0;
uint32_t vsyncEnd = 0;

unsigned int vsyncCount = 0;

static uint32_t state;

gfxColorMode DRV_LCC_GetColorMode()
{
    return GFX_COLOR_MODE_RGB_565;
}

uint32_t DRV_LCC_GetBufferCount()
{
    return 1;    
}

uint32_t DRV_LCC_GetDisplayWidth()
{
    return DISP_HOR_RESOLUTION;
}

uint32_t DRV_LCC_GetDisplayHeight()
{
    return DISP_VER_RESOLUTION;
}

void DRV_LCC_Update()
{
    if(state == INIT)
    {
        if(DRV_GFX_LCC_Start() != 0)
            return;
        
        memset(frameBuffer, 0x55, DISPLAY_WIDTH * DISPLAY_HEIGHT * 2);
        
        state = RUN;
    }
}

uint32_t DRV_LCC_GetLayerCount()
{
	return 1;
}

uint32_t DRV_LCC_GetActiveLayer()
{
	return 0;
}

gfxResult DRV_LCC_SetActiveLayer(uint32_t idx)
{
        return GFX_SUCCESS;
}

void DRV_LCC_Swap(void)
{
    
}

uint32_t DRV_LCC_GetVSYNCCount(void)
{
	return vsyncCount;
}

gfxPixelBuffer * DRV_LCC_GetFrameBuffer(int32_t idx)
{
        return &pixelBuffer;
}

gfxResult DRV_LCC_BlitBuffer(int32_t x,
                             int32_t y,
                             gfxPixelBuffer* buf,
                             gfxBlend gfx)
{
    void* srcPtr;
    void* destPtr;
    uint32_t row, rowSize;

    if (state != RUN)
        return GFX_FAILURE;
    
    rowSize = buf->size.width * gfxColorInfoTable[buf->mode].size;
    
    for(row = 0; row < buf->size.height; row++)
    {
        srcPtr = gfxPixelBufferOffsetGet(buf, 0, row);
        destPtr = gfxPixelBufferOffsetGet(&pixelBuffer, x, y + row);
        
        memcpy(destPtr, srcPtr, rowSize);
    }
    
    return GFX_SUCCESS;
}

static gfxResult lccBacklightBrightnessSet(uint32_t brightness)
{
<#if PeripheralControl == "TC">
    uint32_t value;
    brightness = (brightness <= 100) ? brightness : 100;
    
    value = TC${TCInstance}_CH${TCChannel}_ComparePeriodGet() * (100 - brightness) / 100;
    
    //Use a positive value
    if (value == 0)
        value = 1;
    
    TC${TCInstance}_CH${TCChannel}_Compare${TCChannelCompare}Set(value);
<#else>
    if (brightness == 0)
    {
<#if Val_BacklightEnable == 1>
        GFX_DISP_INTF_PIN_BACKLIGHT_Clear();
<#else>
        GFX_DISP_INTF_PIN_BACKLIGHT_Set();
</#if>
    }
    else
    {
<#if Val_BacklightEnable == 1>
        GFX_DISP_INTF_PIN_BACKLIGHT_Set();
<#else>
        GFX_DISP_INTF_PIN_BACKLIGHT_Clear();
</#if>
    }
</#if>

    return GFX_SUCCESS;

}

gfxResult DRV_LCC_Initialize(void)
{
    state = INIT;

    gfxPixelBufferCreate(DISP_HOR_RESOLUTION,
                        DISP_VER_RESOLUTION,
                        GFX_COLOR_MODE_RGB_565,
                        frameBuffer,
                        &pixelBuffer);
    
    // driver specific initialization tasks    
    VER_BLANK = DISP_VER_PULSE_WIDTH +
                DISP_VER_BACK_PORCH +
                DISP_VER_FRONT_PORCH - 1;
    
    HBackPorch = DISP_HOR_PULSE_WIDTH +
                 DISP_HOR_BACK_PORCH;
    
    vsyncPeriod = DISP_VER_FRONT_PORCH + DISP_VER_RESOLUTION + DISP_VER_BACK_PORCH;  

    GFX_DISP_INTF_PIN_RESET_Set();

    /*Turn Backlight on*/
<#if PeripheralControl == "TC">
    TC${TCInstance}_CH${TCChannel}_CompareStart();
</#if>

    lccBacklightBrightnessSet(100);

    return GFX_SUCCESS;
}

/**** End Hardware Abstraction Interfaces ****/

static void lccDMAStartTransfer(const void *srcAddr, size_t srcSize,
                                       const void *destAddr)
{
<#if DMAController?? && DMAController == "DMAC">
    DMAC_ChannelTransfer(DRV_GFX_LCC_DMA_CHANNEL_INDEX,
                         srcAddr,
                         srcSize,
                         destAddr,
                         FRAMEBUFFER_PIXEL_BYTES,
                         srcSize);
<#else>
    XDMAC_ChannelBlockLengthSet(DRV_GFX_LCC_DMA_CHANNEL_INDEX, (srcSize / FRAMEBUFFER_PIXEL_BYTES) - 1);

<#if UseCachedFrameBuffer == true>
    SCB_CleanInvalidateDCache_by_Addr(
                    (uint32_t *)((uint32_t ) srcAddr & ~0x1F),
                    srcSize + 32);
</#if>

    XDMAC_ChannelTransfer(DRV_GFX_LCC_DMA_CHANNEL_INDEX, srcAddr, destAddr, 1);
</#if>
}

static int DRV_GFX_LCC_Start()
{
<#if DMAController?? && DMAController == "DMAC">
    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_0, dmaIntHandler, 0);
    
    lccDMAStartTransfer(frameBuffer, 
                        FRAMEBUFFER_PIXEL_BYTES, 
                        (const void*) EBI_BASE_ADDR);
<#else>
    XDMAC_ChannelCallbackRegister(DRV_GFX_LCC_DMA_CHANNEL_INDEX, dmaIntHandler, 0);

    lccDMAStartTransfer(frameBuffer, 
                        FRAMEBUFFER_PIXEL_BYTES,
                        (const void *) EBI_BASE_ADDR);
</#if>

    return 0;
}

static void DRV_GFX_LCC_DisplayRefresh(void)
{
    gfxPoint drawPoint;
    gfxBuffer* buffer_to_tx = (void*) frameBuffer;

    typedef enum
    {
        HSYNC_FRONT_PORCH,
        HSYNC_PULSE,
        HSYNC_BACK_PORCH,
        HSYNC_DATA_ENABLE,
        HSYNC_DATA_ENABLE_OVERFLOW        
    } HSYNC_STATES;

    typedef enum
    {
        VSYNC_FRONT_PORCH,
        VSYNC_PULSE,
        VSYNC_BACK_PORCH,
        VSYNC_BLANK        
    } VSYNC_STATES;

    static HSYNC_STATES hsyncState = HSYNC_FRONT_PORCH;
    static VSYNC_STATES vsyncState = VSYNC_BLANK;

    switch(vsyncState)
    {
        case VSYNC_FRONT_PORCH:
        {
            if (hSyncs > vsyncPulseDown)
            {
<#if DisplayVSYNCNegative == true>
                GFX_DISP_INTF_PIN_VSYNC_Set();
<#else>
                GFX_DISP_INTF_PIN_VSYNC_Clear();
</#if>

                vsyncPulseUp = hSyncs + DISP_VER_PULSE_WIDTH;
                vsyncState = VSYNC_PULSE;

                /*if(cntxt->layer.active->vsync == GFX_TRUE
                    && cntxt->layer.active->swap == GFX_TRUE)
                {
                    GFX_LayerSwap(cntxt->layer.active);
                }*/

                line = 0;
            }
            
            break;
        }
        case VSYNC_PULSE:
        {
            if (hSyncs >= vsyncPulseUp)
            {
<#if DisplayVSYNCNegative == true>
                GFX_DISP_INTF_PIN_VSYNC_Clear();
<#else>
                GFX_DISP_INTF_PIN_VSYNC_Set();
</#if>
                vsyncEnd = hSyncs + DISP_VER_BACK_PORCH;
                vsyncState = VSYNC_BACK_PORCH;

                vsyncCount++;                
            }
            
            break;
        }
        case VSYNC_BACK_PORCH:
        {
            if (hSyncs >= vsyncEnd)
                vsyncState = VSYNC_BLANK;
            
            break;
        }
        case VSYNC_BLANK:
        {
            break;
        }
    }

    switch (hsyncState)
    {
        case HSYNC_FRONT_PORCH:
        {
<#if DisplayDataEnable == true>
<#if DisplayDataEnablePolarity == true>
            GFX_DISP_INTF_PIN_DE_Clear();
<#else>
            GFX_DISP_INTF_PIN_DE_Set();
</#if>
</#if>

            hsyncState = HSYNC_PULSE;

            if (DISP_HOR_FRONT_PORCH > 0)
            {
                pixels = DISP_HOR_FRONT_PORCH;
                break;
            }
        }
        case HSYNC_PULSE:
        {
<#if DisplayHSYNCNegative == true>
            GFX_DISP_INTF_PIN_HSYNC_Set();
<#else>
            GFX_DISP_INTF_PIN_HSYNC_Clear();
</#if>

            if (hSyncs >= vsyncPeriod)
            {
                vsyncPeriod = hSyncs + DISP_VER_PULSE_WIDTH + DISP_VER_FRONT_PORCH + DISP_VER_RESOLUTION + DISP_VER_BACK_PORCH;
                vsyncPulseDown = hSyncs + DISP_VER_FRONT_PORCH;
                vsyncState = VSYNC_FRONT_PORCH;
            }

            hSyncs++; 

            pixels = DISP_HOR_PULSE_WIDTH;
            hsyncState = HSYNC_BACK_PORCH;  

            break;
        }
        case HSYNC_BACK_PORCH:
        {
<#if DisplayHSYNCNegative == true>
            GFX_DISP_INTF_PIN_HSYNC_Clear();
<#else>
            GFX_DISP_INTF_PIN_HSYNC_Set();
</#if>

            hsyncState = HSYNC_DATA_ENABLE; 

            if (DISP_HOR_BACK_PORCH > 0)
            {
                pixels = DISP_HOR_BACK_PORCH;
                break;
            }
        }
        case HSYNC_DATA_ENABLE:
        {
            if (vsyncState == VSYNC_BLANK)
            {
<#if DisplayDataEnable == true>
<#if DisplayDataEnablePolarity == true>
                GFX_DISP_INTF_PIN_DE_Set();
<#else>
                GFX_DISP_INTF_PIN_DE_Clear();
</#if>
</#if>
                drawPoint.x = 0;
                drawPoint.y = line++;

                //buffer = .pb;

                buffer_to_tx = gfxPixelBufferOffsetGet_Unsafe(&pixelBuffer, drawPoint.x, drawPoint.y);

            }

            pixels = DISP_HOR_RESOLUTION;
            hsyncState = HSYNC_FRONT_PORCH;

            break;
        }
        case HSYNC_DATA_ENABLE_OVERFLOW:
        {
            hsyncState = HSYNC_FRONT_PORCH;
            
            break;
        }
    }

    lccDMAStartTransfer(buffer_to_tx,
                        (pixels * FRAMEBUFFER_PIXEL_BYTES), //2 bytes per pixel
                        (uint32_t*) EBI_BASE_ADDR);
}

void dmaIntHandler (DRV_GFX_DMA_EVENT_TYPE status,
                    uintptr_t contextHandle)
{
    DRV_GFX_LCC_DisplayRefresh();
}
