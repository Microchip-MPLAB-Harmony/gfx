/*******************************************************************************
  MPLAB Harmony XLCDC Generated Driver Implementation File

  File Name:
    drv_gfx_xlcdc.c

  Summary:
    Build-time generated implementation for the XLCDC Driver for SAM9X72/75 MPUs.

  Description:
    Contains function definitions and the data types that make up the interface to the XLCDC
    Graphics Controller for SAM9X72/75 MPUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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

#include "gfx/driver/gfx_driver.h"
<#if !XDMCPUBilt && le_gfx_gfx2d??>
#include "gfx/driver/processor/gfx2d/drv_gfx2d.h"
</#if>
#include "gfx/driver/controller/xlcdc/drv_gfx_xlcdc.h"
#include "gfx/driver/controller/xlcdc/plib/plib_xlcdc.h"

/* Utility Macros */
/* Frame Buffer Macros */
/* Cached, Cache Aligned */
#define FB_CACHE_CA             CACHE_ALIGN
/* Not Cached */
#define FB_CACHE_NC             __attribute__ ((section(".region_nocache"), aligned (32)))
/* Frame Buffer Pointer Type */
<#if XLMColMode == "13">
#define FB_COL_MODE             XLCDC_RGB_COLOR_MODE_RGBA_8888
#define FB_BPP_TYPE             uint32_t
<#else>
#define FB_COL_MODE             XLCDC_RGB_COLOR_MODE_RGB_565
#define FB_BPP_TYPE             uint16_t
</#if>
#define FB_PTR_TYPE             FB_BPP_TYPE *
#define FB_TYPE_SZ              sizeof(FB_BPP_TYPE)

/* Driver Settings */
#define XLCDC_TOT_LAYERS        ${TotalNumLayers}
#define XLCDC_BUF_PER_LAYER     1
#define XLCDC_HOR_RES           ${XTResPPL}
#define XLCDC_VER_RES           ${XTResRPF}

/* Local Data */
/* Driver */
typedef enum
{
    INIT = 0,
    DRAW,
    SWAP,
    SYNC,
    FREEZE
} DRV_STATE;

/* Generated Layer Order */
static const char layerOrder[XLCDC_TOT_LAYERS] = {
    XLCDC_LAYER_BASE,
<#if XLMEnableOVR1 && XLMHEOVIDPRI>
    XLCDC_LAYER_OVR1,
</#if>
<#if XLMEnableHEO && !XLMHEOYCbCrEN>
    XLCDC_LAYER_HEO,
</#if>
<#if XLMEnableOVR1 && !XLMHEOVIDPRI>
    XLCDC_LAYER_OVR1,
</#if>
<#if XLMEnableOVR2>
    XLCDC_LAYER_OVR2,
</#if>
};

const char *DRIVER_NAME = "XLCDC";
static volatile DRV_STATE state;
static volatile uint32_t vsyncCount = 0;
static volatile bool vblankSync = false;

/* Layer */
typedef enum
{
    LAYER_LOCK_UNLOCKED,
    LAYER_LOCK_LOCKED,
    LAYER_LOCK_PENDING,
} LAYER_LOCK_STATUS;

typedef struct
{
    bool enabled;
    int pixelformat;
    uint32_t resx;
    uint32_t resy;
    uint32_t startx;
    uint32_t starty;
    uint32_t sizex;
    uint32_t sizey;
    uint32_t alpha;
    FB_PTR_TYPE baseaddr[XLCDC_BUF_PER_LAYER];
    gfxPixelBuffer pixelBuffer[XLCDC_BUF_PER_LAYER];
    volatile uint32_t bufferIdx;
    volatile LAYER_LOCK_STATUS updateLock;
} LAYER_ATTRIBUTES;

static uint32_t activeLayer = 0;
static gfxRect srcRect, destRect;
static LAYER_ATTRIBUTES drvLayer[XLCDC_TOT_LAYERS];

<#if !CanvasModeOnly>
/* Layer Frame Buffers */
<#if XDMCacheFB>
FB_BPP_TYPE FB_CACHE_CA frame_buffer [XLCDC_TOT_LAYERS * XLCDC_BUF_PER_LAYER][XLCDC_HOR_RES * XLCDC_VER_RES];
<#else>
FB_BPP_TYPE FB_CACHE_NC frame_buffer [XLCDC_TOT_LAYERS * XLCDC_BUF_PER_LAYER][XLCDC_HOR_RES * XLCDC_VER_RES];
</#if>

/* Local Functions */
/* Convert XLCDC Color Mode to GFX Color Mode */
static gfxColorMode DRV_XLCDC_ColorModeGFXFromXLCDC(XLCDC_RGB_COLOR_MODE mode)
{
    switch(mode)
    {
        case XLCDC_RGB_COLOR_MODE_CLUT:
            return GFX_COLOR_MODE_GS_8;
        case XLCDC_RGB_COLOR_MODE_RGB_565:
            return GFX_COLOR_MODE_RGB_565;
        case XLCDC_RGB_COLOR_MODE_RGB_888:
            return GFX_COLOR_MODE_RGB_888;
        case XLCDC_RGB_COLOR_MODE_ARGB_8888:
            return GFX_COLOR_MODE_ARGB_8888;
        case XLCDC_RGB_COLOR_MODE_RGBA_8888:
            return GFX_COLOR_MODE_RGBA_8888;
        default:
            return GFX_COLOR_MODE_RGBA_8888;
    }
}

/* Frame Buffer specific fill */
static void DRV_XLCDC_ColorSet (void * fb)
{
    register FB_BPP_TYPE *ptr = (FB_PTR_TYPE)fb;
    register uint32_t size = XLCDC_HOR_RES * XLCDC_VER_RES;
    while(size-- > 0) *ptr++ = 0x${XDMColorFB?upper_case};
}

<#else>
/* Convert GFX Color Mode to XLCDC Color Mode */
static XLCDC_RGB_COLOR_MODE DRV_XLCDC_ColorModeXLCDCFromGFX(gfxColorMode mode)
{
    switch(mode)
    {
        case GFX_COLOR_MODE_GS_8:
            return XLCDC_RGB_COLOR_MODE_CLUT;
        case GFX_COLOR_MODE_RGB_565:
            return XLCDC_RGB_COLOR_MODE_RGB_565;
        case GFX_COLOR_MODE_RGB_888:
            return XLCDC_RGB_COLOR_MODE_RGB_888;
        case GFX_COLOR_MODE_ARGB_8888:
            return XLCDC_RGB_COLOR_MODE_ARGB_8888;
        case GFX_COLOR_MODE_RGBA_8888:
            return XLCDC_RGB_COLOR_MODE_RGBA_8888;
        default:
            return XLCDC_RGB_COLOR_MODE_RGBA_8888;
    }
}

</#if>
<#if XDMCPUBilt || !le_gfx_gfx2d??>
/* Perform a CPU based Blit */
static gfxResult DRV_XLCDC_CPU_Blit(const gfxPixelBuffer *source,
                                    const gfxRect *rectSrc,
                                    const gfxPixelBuffer *dest,
                                    const gfxRect *rectDest)
{
    void *srcPtr;
    void *destPtr;
    uint32_t width, height;
    uint32_t row, rowSize;

    width = (rectSrc->width < rectDest->width) ? rectSrc->width : rectDest->width;
    height = (rectSrc->height < rectDest->height) ? rectSrc->height : rectDest->height;
    rowSize = width * gfxColorInfoTable[dest->mode].size;

    for (row = 0; row < height; row++)
    {
        srcPtr = gfxPixelBufferOffsetGet(source, rectSrc->x, rectSrc->y + row);
        destPtr = gfxPixelBufferOffsetGet(dest, rectDest->x, rectDest->y + row);

        memcpy(destPtr, srcPtr, rowSize);
    }

    return GFX_SUCCESS;
}

</#if>
<#if CanvasModeOnly>
/* Process the Layer IOCTL Subset */
static gfxDriverIOCTLResponse DRV_XLCDC_LayerConfig(gfxDriverIOCTLRequest request,
                                                  gfxIOCTLArg_LayerArg *arg)
{
    gfxIOCTLArg_LayerValue *layerVal;
    gfxIOCTLArg_LayerPosition *layerPos;
    gfxIOCTLArg_LayerSize *layerSize;

    if (arg->id >= XLCDC_TOT_LAYERS)
    {
        return GFX_IOCTL_ERROR_UNKNOWN;

    }

    if (request == GFX_IOCTL_SET_LAYER_LOCK)
    {
        drvLayer[arg->id].updateLock = LAYER_LOCK_LOCKED;

        return GFX_IOCTL_OK;
    }

    if (drvLayer[arg->id].updateLock != LAYER_LOCK_LOCKED)
    {
        return GFX_IOCTL_ERROR_UNKNOWN;
    }

    if (request == GFX_IOCTL_SET_LAYER_UNLOCK)
    {
        XLCDC_SetLayerAddress(layerOrder[arg->id], (uint32_t)drvLayer[arg->id].baseaddr[0], false);
        XLCDC_SetLayerOpts(layerOrder[arg->id], drvLayer[arg->id].alpha, true, false);
        XLCDC_SetLayerWindowXYPos(layerOrder[arg->id], drvLayer[arg->id].startx, drvLayer[arg->id].starty, false);
        XLCDC_SetLayerWindowXYSize(layerOrder[arg->id], drvLayer[arg->id].sizex, drvLayer[arg->id].sizey, false);
        XLCDC_SetLayerXStride(layerOrder[arg->id], FB_TYPE_SZ * (drvLayer[arg->id].resx - drvLayer[arg->id].sizex), false);
        XLCDC_SetLayerEnable(layerOrder[arg->id], drvLayer[arg->id].enabled, true);

        drvLayer[arg->id].updateLock = LAYER_LOCK_UNLOCKED;

        return GFX_IOCTL_OK;
    }

    switch (request)
    {
        case GFX_IOCTL_SET_LAYER_ALPHA:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            drvLayer[arg->id].alpha = layerVal->value.v_uint;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_SIZE:
        {
            layerSize = (gfxIOCTLArg_LayerSize *)arg;

            drvLayer[arg->id].resx = layerSize->width;
            drvLayer[arg->id].resy = layerSize->height;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_POSITION:
        {
            layerPos = (gfxIOCTLArg_LayerPosition *)arg;

            drvLayer[arg->id].startx = layerPos->x;
            drvLayer[arg->id].starty = layerPos->y;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_WINDOW_SIZE:
        {
            layerSize = (gfxIOCTLArg_LayerSize *)arg;

            drvLayer[arg->id].sizex = layerSize->width;
            drvLayer[arg->id].sizey = layerSize->height;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_BASE_ADDRESS:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            drvLayer[arg->id].baseaddr[0] = layerVal->value.v_pointer;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_COLOR_MODE:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            drvLayer[arg->id].pixelformat = DRV_XLCDC_ColorModeXLCDCFromGFX(layerVal->value.v_colormode);

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_LAYER_ENABLED:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            layerVal->value.v_bool = drvLayer[arg->id].enabled;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_LAYER_ENABLED:
        {
            layerVal = (gfxIOCTLArg_LayerValue *)arg;

            drvLayer[arg->id].enabled = layerVal->value.v_bool;

            return GFX_IOCTL_OK;
        }

        default:
            break;
    }

    return GFX_IOCTL_UNSUPPORTED;
}

</#if>
void DRV_XLCDC_Update(void)
{
    switch(state)
    {
        case INIT:
        {
            state = DRAW;
            break;
        }
        case DRAW:
        case SWAP:
        default:
            break;
    }
}

gfxResult DRV_XLCDC_Initialize(void)
{
    /* Clear the Layer Attributes */
    memset(drvLayer, 0, sizeof(drvLayer));

    /* Initialize Layer Attributes */
    for (uint32_t layerCount = 0; layerCount < XLCDC_TOT_LAYERS; layerCount++)
    {
        drvLayer[layerCount].pixelformat = FB_COL_MODE;
        drvLayer[layerCount].resx = XLCDC_HOR_RES;
        drvLayer[layerCount].resy = XLCDC_VER_RES;
        drvLayer[layerCount].startx = 0;
        drvLayer[layerCount].starty = 0;
        drvLayer[layerCount].sizex = drvLayer[layerCount].resx;
        drvLayer[layerCount].sizey = drvLayer[layerCount].resy;
        drvLayer[layerCount].alpha = 255;
<#if !CanvasModeOnly>
        drvLayer[layerCount].enabled = true;
<#else>
        drvLayer[layerCount].enabled = false;
</#if>
        drvLayer[layerCount].updateLock = LAYER_LOCK_UNLOCKED;
        drvLayer[layerCount].bufferIdx = XLCDC_BUF_PER_LAYER - 1;

<#if !CanvasModeOnly>
        for (uint32_t bufferCount = 0; bufferCount < XLCDC_BUF_PER_LAYER; ++bufferCount)
        {
            drvLayer[layerCount].baseaddr[bufferCount] = (FB_PTR_TYPE)frame_buffer[layerCount + bufferCount * XLCDC_TOT_LAYERS];

            DRV_XLCDC_ColorSet(drvLayer[layerCount].baseaddr[bufferCount]);

            gfxPixelBufferCreate(XLCDC_HOR_RES,
                                 XLCDC_VER_RES,
                                 DRV_XLCDC_ColorModeGFXFromXLCDC(drvLayer[layerCount].pixelformat),
                                 drvLayer[layerCount].baseaddr[bufferCount],
                                 &drvLayer[layerCount].pixelBuffer[bufferCount]);
        }

</#if>
        XLCDC_SetLayerEnable(layerOrder[layerCount], false, true);
        XLCDC_SetLayerAddress(layerOrder[layerCount], (uint32_t) drvLayer[layerCount].baseaddr[drvLayer[layerCount].bufferIdx], false);
        XLCDC_SetLayerOpts(layerOrder[layerCount], 255, true, false);
        XLCDC_SetLayerWindowXYPos(layerOrder[layerCount], 0, 0, false);
        XLCDC_SetLayerWindowXYSize(layerOrder[layerCount], XLCDC_HOR_RES, XLCDC_VER_RES, false);
        XLCDC_SetLayerEnable(layerOrder[layerCount], drvLayer[layerCount].enabled, true);
    }

    return GFX_SUCCESS;
}

gfxResult DRV_XLCDC_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf)
{
    gfxResult result = GFX_FAILURE;

    if (state != DRAW)
        return result;

    gfxPixelBuffer_SetLocked(buf, GFX_TRUE);

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.height = buf->size.height;
    srcRect.width = buf->size.width;

    destRect.x = x;
    destRect.y = y;
    destRect.height = buf->size.height;
    destRect.width = buf->size.width;

<#if !XDMCPUBilt && le_gfx_gfx2d??>
    result = gfxGPUInterface.blitBuffer(buf, &srcRect, &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].bufferIdx], &destRect);

<#else>
    result = DRV_XLCDC_CPU_Blit(buf,
                                &srcRect,
                                &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].bufferIdx],
                                &destRect);

</#if>
    gfxPixelBuffer_SetLocked(buf, GFX_FALSE);

    return result;
}

gfxDriverIOCTLResponse DRV_XLCDC_IOCTL(gfxDriverIOCTLRequest request, void* arg)
{
    gfxIOCTLArg_Value *val;
    gfxIOCTLArg_DisplaySize *disp;
    gfxIOCTLArg_LayerRect *rect;

    switch (request)
    {
        case GFX_IOCTL_LAYER_SWAP:
        {
            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_FRAME_END:
        {
            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_BUFFER_COUNT:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_uint = XLCDC_BUF_PER_LAYER;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_DISPLAY_SIZE:
        {
            disp = (gfxIOCTLArg_DisplaySize *)arg;

            disp->width = XLCDC_HOR_RES;
            disp->height = XLCDC_VER_RES;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_LAYER_COUNT:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_uint = XLCDC_TOT_LAYERS;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_uint = activeLayer;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value *)arg;

            if (val->value.v_uint >= XLCDC_TOT_LAYERS)
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
            rect = (gfxIOCTLArg_LayerRect *)arg;

            if (rect->layer.id >= XLCDC_TOT_LAYERS)
            {
                return GFX_IOCTL_ERROR_UNKNOWN;
            }

            rect->x = drvLayer[rect->layer.id].startx;
            rect->y = drvLayer[rect->layer.id].starty;
            rect->width = drvLayer[rect->layer.id].sizex;
            rect->height = drvLayer[rect->layer.id].sizey;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_VSYNC_COUNT:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_uint = vsyncCount;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_FRAMEBUFFER:
        {
            val = (gfxIOCTLArg_Value *)arg;

            val->value.v_pbuffer = &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].bufferIdx];

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_SET_PALETTE:
        {
            return GFX_IOCTL_UNSUPPORTED;
        }

        case GFX_IOCTL_SET_VBLANK_SYNC:
        {
            vblankSync = ((gfxIOCTLArg_Value *)arg)->value.v_bool;

            return GFX_IOCTL_OK;
        }

        case GFX_IOCTL_GET_STATUS:
        {
            val = (gfxIOCTLArg_Value *)arg;
            val->value.v_uint = 0;
<#if CanvasModeOnly>
            unsigned int i;

            for (i = 0; i < XLCDC_TOT_LAYERS; i++)
            {
                if (drvLayer[i].updateLock != LAYER_LOCK_UNLOCKED)
                {
                    val->value.v_uint = 1;

                    break;
                }
            }
</#if>

            return GFX_IOCTL_OK;
        }

        default:
        {
<#if CanvasModeOnly>
            if (request >= GFX_IOCTL_LAYER_REQ_START &&
                request <= GFX_IOCTL_LAYER_REQ_END)
            {
                return DRV_XLCDC_LayerConfig(request, (gfxIOCTLArg_LayerArg *)arg);
            }
</#if>
                break;
        }
    }

    return GFX_IOCTL_UNSUPPORTED;
}
