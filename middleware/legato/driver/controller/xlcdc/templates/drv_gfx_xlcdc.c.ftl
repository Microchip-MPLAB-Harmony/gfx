/*******************************************************************************
  MPLAB Harmony XLCDC Generated Driver Implementation File

  File Name:
    drv_gfx_xlcdc.c

  Summary:
    Build-time generated implementation for the XLCDC Driver for SAM9X7/SAMA7D MPUs.

  Description:
    Contains function definitions and the data types that make up the interface to the XLCDC
    Graphics Controller for SAM9X7/SAMA7D MPUs.

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

<#if SupportNEON>
#include "arm_neon.h"
</#if>
#include "toolchain_specifics.h"
#include "gfx/driver/gfx_driver.h"
<#if DoubleBuffering && gfx_legato??>
#include "gfx/legato/legato_config.h"
</#if>
<#if !XDMCPUBilt && le_gfx_gfx2d??>
#include "gfx/driver/processor/gfx2d/drv_gfx2d.h"
</#if>
#include "gfx/driver/controller/xlcdc/drv_gfx_xlcdc.h"
#include "gfx/driver/controller/xlcdc/plib/plib_xlcdc.h"

/* Utility Macros */
/* Math */
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) >= 0 ? (x) : -(x))
/* Alignment Check */
#define IS_ALIGNED(ptr, align) (((uintptr_t)(ptr) & ((align) - 1)) == 0)
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
#define XLCDC_HOR_RES           ${XTResPPL}
#define XLCDC_VER_RES           ${XTResRPF}
#define XLCDC_TOT_LAYERS        ${TotalNumLayers}
#define XLCDC_BUF_PER_LAYER     ${DoubleBuffering?then('2', '1')}
<#if DoubleBuffering>
#define XLCDC_SYNC_RECT_COUNT   ${DBROMaxRects}
<#if DBRectOpt == "Simple">
#define XLCDC_MERGE_THR_PX      ${DBROMergeThrBase}
<#elseif DBRectOpt == "Smart">
#define XLCDC_INI_MERGE_THR_PX  ${DBROMergeThrBase}
#define XLCDC_MIN_MERGE_THR_PX  ${DBROMergeThrMin}
#define XLCDC_MAX_MERGE_THR_PX  ${DBROMergeThrMax}
#define XLCDC_MAX_OVRHD_RATIO   ${(DBROMergeOHR / 100.0)?string("0.00")}f
</#if>
</#if>

/* Local Data */
/* Driver */
typedef enum
{
    INIT = 0,
    DRAW
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
    volatile uint32_t frontBufferIdx;
<#if DoubleBuffering>
    volatile uint32_t backBufferIdx;
    volatile bool swapPending;
    volatile bool syncEntireLayer;
    volatile uint32_t syncRectIndex;
    gfxRect syncRect[XLCDC_SYNC_RECT_COUNT];
</#if>
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

<#if DoubleBuffering>
/* XLCDC SOF Interrupt callback */
void DRV_XLCDC_SOF_Interrupt(void)
{
#if LE_DRIVER_LAYER_MODE
    bool swapAllLayers = true;

    for (int i = 0; i < LE_LAYER_COUNT; i++)
    {
        if (!drvLayer[i].swapPending)
        {
            swapAllLayers = false;
            break;
        }
    }

    if (swapAllLayers)
    {
        for (int i = 0; i < LE_LAYER_COUNT; i++)
        {
            XLCDC_UpdateLayerAttributes(layerOrder[i]);
            drvLayer[i].swapPending = false;
        }
    }
#else
    for (int i = 0; i < XLCDC_TOT_LAYERS; i++)
    {
        if (drvLayer[i].swapPending)
        {
            XLCDC_UpdateLayerAttributes(layerOrder[i]);
            drvLayer[i].swapPending = false;
        }
    }
#endif
}

<#if DBRectOpt == "Simple">
static void DRV_XLCDC_OptimizeDirtyRects(uint32_t layerIdx)
{
    if (drvLayer[layerIdx].syncEntireLayer || drvLayer[layerIdx].syncRectIndex <= 1)
        return;

    for (uint32_t i = 0; i < drvLayer[layerIdx].syncRectIndex; i++)
    {
        for (uint32_t j = i + 1; j < drvLayer[layerIdx].syncRectIndex; j++)
        {
            gfxRect* rect1 = &drvLayer[layerIdx].syncRect[i];
            gfxRect* rect2 = &drvLayer[layerIdx].syncRect[j];

            bool horizontalAdjacent =
                (rect1->y == rect2->y && rect1->height == rect2->height &&
                 (rect1->x + rect1->width == rect2->x || rect2->x + rect2->width == rect1->x));

            bool verticalAdjacent =
                (rect1->x == rect2->x && rect1->width == rect2->width &&
                 (rect1->y + rect1->height == rect2->y || rect2->y + rect2->height == rect1->y));

            bool horizontalMergeable =
                (ABS((rect1->x + rect1->width) - rect2->x) <= XLCDC_MERGE_THR_PX) ||
                (ABS(rect1->x - (rect2->x + rect2->width)) <= XLCDC_MERGE_THR_PX);

            bool verticalMergeable =
                (ABS((rect1->y + rect1->height) - rect2->y) <= XLCDC_MERGE_THR_PX) ||
                (ABS(rect1->y - (rect2->y + rect2->height)) <= XLCDC_MERGE_THR_PX);

            if (horizontalAdjacent || verticalAdjacent || (horizontalMergeable && verticalMergeable))
            {
                int32_t minX = MIN(rect1->x, rect2->x);
                int32_t minY = MIN(rect1->y, rect2->y);
                int32_t maxX = MAX(rect1->x + rect1->width, rect2->x + rect2->width);
                int32_t maxY = MAX(rect1->y + rect1->height, rect2->y + rect2->height);

                rect1->x = minX;
                rect1->y = minY;
                rect1->width = maxX - minX;
                rect1->height = maxY - minY;

                for (uint32_t k = j; k < drvLayer[layerIdx].syncRectIndex - 1; k++)
                {
                    drvLayer[layerIdx].syncRect[k] = drvLayer[layerIdx].syncRect[k + 1];
                }

                drvLayer[layerIdx].syncRectIndex--;
                j--;
            }
        }
    }
}

<#elseif DBRectOpt == "Smart">
static void DRV_XLCDC_OptimizeDirtyRects(uint32_t layerIdx)
{
    if (drvLayer[layerIdx].syncEntireLayer || drvLayer[layerIdx].syncRectIndex <= 1)
        return;

    static uint32_t layerMergeThr[XLCDC_TOT_LAYERS] = {0};
    if (layerMergeThr[layerIdx] == 0) {
        layerMergeThr[layerIdx] = XLCDC_INI_MERGE_THR_PX;
    }

    uint32_t mergedRectCount = 0;
    for (uint32_t i = 0; i < drvLayer[layerIdx].syncRectIndex; i++)
    {
        for (uint32_t j = i + 1; j < drvLayer[layerIdx].syncRectIndex; j++)
        {
            gfxRect* rect1 = &drvLayer[layerIdx].syncRect[i];
            gfxRect* rect2 = &drvLayer[layerIdx].syncRect[j];

            int32_t minX = MIN(rect1->x, rect2->x);
            int32_t minY = MIN(rect1->y, rect2->y);
            int32_t maxX = MAX(rect1->x + rect1->width, rect2->x + rect2->width);
            int32_t maxY = MAX(rect1->y + rect1->height, rect2->y + rect2->height);

            uint32_t originalArea1 = rect1->width * rect1->height;
            uint32_t originalArea2 = rect2->width * rect2->height;
            uint32_t mergedArea = (maxX - minX) * (maxY - minY);
            uint32_t overheadArea = mergedArea - (originalArea1 + originalArea2);
            float overheadRatio = (float)overheadArea / (originalArea1 + originalArea2);

            bool horizontalAdjacent =
                (rect1->y == rect2->y && rect1->height == rect2->height &&
                 (rect1->x + rect1->width == rect2->x || rect2->x + rect2->width == rect1->x));
            bool verticalAdjacent =
                (rect1->x == rect2->x && rect1->width == rect2->width &&
                 (rect1->y + rect1->height == rect2->y || rect2->y + rect2->height == rect1->y));
            bool horizontalMergeable =
                (ABS((rect1->x + rect1->width) - rect2->x) <= layerMergeThr[layerIdx]) ||
                (ABS(rect1->x - (rect2->x + rect2->width)) <= layerMergeThr[layerIdx]);
            bool verticalMergeable =
                (ABS((rect1->y + rect1->height) - rect2->y) <= layerMergeThr[layerIdx]) ||
                (ABS(rect1->y - (rect2->y + rect2->height)) <= layerMergeThr[layerIdx]);

            if ((horizontalAdjacent || verticalAdjacent ||
                 (horizontalMergeable && verticalMergeable)) &&
                overheadRatio <= XLCDC_MAX_OVRHD_RATIO)
            {
                rect1->x = minX;
                rect1->y = minY;
                rect1->width = maxX - minX;
                rect1->height = maxY - minY;

                for (uint32_t k = j; k < drvLayer[layerIdx].syncRectIndex - 1; k++)
                {
                    drvLayer[layerIdx].syncRect[k] = drvLayer[layerIdx].syncRect[k + 1];
                }
                drvLayer[layerIdx].syncRectIndex--;
                mergedRectCount++;
                j--;
            }
        }
    }

    if (mergedRectCount > 0) {
        layerMergeThr[layerIdx] = MAX(XLCDC_MIN_MERGE_THR_PX, layerMergeThr[layerIdx] - 1);
    } else {
        layerMergeThr[layerIdx] = MIN(XLCDC_MAX_MERGE_THR_PX, layerMergeThr[layerIdx] + 1);
    }
}

</#if>
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
<#if SupportNEON>
/* Perform a CPU based Blit w/ NEON */
static gfxResult DRV_XLCDC_CPU_Blit(const gfxPixelBuffer* restrict source,
                                   const gfxRect* restrict rectSrc,
                                   const gfxPixelBuffer* restrict dest,
                                   const gfxRect* restrict rectDest)
{
    if (!source || !rectSrc || !dest || !rectDest)
        return GFX_FAILURE;

    // Calculate dimensions
    const uint32_t width = MIN(rectSrc->width, rectDest->width);
    const uint32_t height = MIN(rectSrc->height, rectDest->height);

    if (width == 0 || height == 0)
        return GFX_FAILURE;

    // Calculate row size in bytes
    const uint32_t pixelSize = gfxColorInfoTable[dest->mode].size;
    const uint32_t rowSize = width * pixelSize;

    // Calculate source and destination strides based on buffer widths
    const uint32_t srcStride = source->size.width * pixelSize;
    const uint32_t destStride = dest->size.width * pixelSize;

    uint8_t* restrict srcBase = (uint8_t*)gfxPixelBufferOffsetGet(source, rectSrc->x, rectSrc->y);
    uint8_t* restrict destBase = (uint8_t*)gfxPixelBufferOffsetGet(dest, rectDest->x, rectDest->y);

    // Check if we can do a single large transfer i.e. we have contiguous data
    if (width == source->size.width && width == dest->size.width)
    {
        const uint32_t totalSize = rowSize * height;

        if (IS_ALIGNED(srcBase, 4) && IS_ALIGNED(destBase, 4) && totalSize >= 16)
        {
            uint8_t* src = srcBase;
            uint8_t* dst = destBase;
            uint32_t vectors = totalSize / 16;
            uint32_t remain = totalSize & 15;

            // Aggressive pre-fetch
            __builtin_prefetch(src);
            __builtin_prefetch(src + 32);
            __builtin_prefetch(src + 64);
            __builtin_prefetch(src + 96);

            while (vectors--)
            {
                // Keep pre-fetching ahead
                __builtin_prefetch(src + 128);
                __builtin_prefetch(src + 160);

                uint8x16_t data = vld1q_u8(src);
                vst1q_u8(dst, data);

                src += 16;
                dst += 16;
            }

            if (remain)
            {
                memcpy(dst, src, remain);
            }
        }
        else
        {
            memcpy(destBase, srcBase, totalSize);
        }

        return GFX_SUCCESS;
    }

    // Row by row processing for non-contiguous data
    for (uint32_t row = 0; row < height; row++)
    {
        uint8_t* restrict src = srcBase + row * srcStride;
        uint8_t* restrict dst = destBase + row * destStride;

        if (IS_ALIGNED(src, 4) && IS_ALIGNED(dst, 4) && rowSize >= 16)
        {
            uint32_t vectors = rowSize / 16;
            uint32_t remain = rowSize & 15;

            // Aggressive pre-fetch
            if (row < height - 1)
            {
                __builtin_prefetch(src + srcStride);
                __builtin_prefetch(src + srcStride + 32);
                __builtin_prefetch(src + srcStride + 64);
                __builtin_prefetch(src + srcStride + 96);
            }

            uint8_t* vectorSrc = src;
            uint8_t* vectorDst = dst;

            while (vectors--)
            {
                // Keep pre-fetching ahead
                __builtin_prefetch(vectorSrc + 128);
                __builtin_prefetch(vectorSrc + 160);

                uint8x16_t data = vld1q_u8(vectorSrc);
                vst1q_u8(vectorDst, data);

                vectorSrc += 16;
                vectorDst += 16;
            }

            if (remain)
            {
                memcpy(vectorDst, vectorSrc, remain);
            }
        }
        else
        {
            memcpy(dst, src, rowSize);
        }
    }

    return GFX_SUCCESS;
}

<#else>
/* Perform a CPU based Blit */
static gfxResult DRV_XLCDC_CPU_Blit(const gfxPixelBuffer* restrict source,
                                   const gfxRect* restrict rectSrc,
                                   const gfxPixelBuffer* restrict dest,
                                   const gfxRect* restrict rectDest)
{
    if (!source || !rectSrc || !dest || !rectDest)
        return GFX_FAILURE;

    // Calculate dimensions
    const uint32_t width = MIN(rectSrc->width, rectDest->width);
    const uint32_t height = MIN(rectSrc->height, rectDest->height);

    if (width == 0 || height == 0)
        return GFX_FAILURE;

    // Calculate row size in bytes
    const uint32_t pixelSize = gfxColorInfoTable[dest->mode].size;
    const uint32_t rowSize = width * pixelSize;

    // Calculate source and destination strides based on buffer widths
    const uint32_t srcStride = source->size.width * pixelSize;
    const uint32_t destStride = dest->size.width * pixelSize;

    uint8_t* restrict srcBase = (uint8_t*)gfxPixelBufferOffsetGet(source, rectSrc->x, rectSrc->y);
    uint8_t* restrict destBase = (uint8_t*)gfxPixelBufferOffsetGet(dest, rectDest->x, rectDest->y);

    // Check if we can do a single large transfer i.e. we have contiguous data
    if (width == source->size.width && width == dest->size.width)
    {
        const uint32_t totalSize = rowSize * height;
        memcpy(destBase, srcBase, totalSize);

        return GFX_SUCCESS;
    }

    // Row by row processing for non-contiguous data
    for (uint32_t row = 0; row < height; row++)
    {
        uint8_t* restrict src = srcBase + row * srcStride;
        uint8_t* restrict dst = destBase + row * destStride;
        memcpy(dst, src, rowSize);
    }

    return GFX_SUCCESS;
}

</#if>
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
<#if DBRectOpt != "Basic" && DoubleBuffering>
        {
            for (uint32_t layerIdx = 0; layerIdx < XLCDC_TOT_LAYERS; layerIdx++)
            {
                if (!drvLayer[layerIdx].syncEntireLayer)
                {
                    DRV_XLCDC_OptimizeDirtyRects(layerIdx);
                }
            }

            break;
        }
</#if>
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
<#if DoubleBuffering>
        drvLayer[layerCount].frontBufferIdx = 0;
        drvLayer[layerCount].backBufferIdx = (drvLayer[layerCount].frontBufferIdx + 1) % XLCDC_BUF_PER_LAYER;
        drvLayer[layerCount].swapPending = false;
        drvLayer[layerCount].syncEntireLayer = false;
        drvLayer[layerCount].syncRectIndex = 0;
<#else>
        drvLayer[layerCount].frontBufferIdx = 0;
</#if>

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
        XLCDC_SetLayerAddress(layerOrder[layerCount], (uint32_t) drvLayer[layerCount].baseaddr[drvLayer[layerCount].frontBufferIdx], false);
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

<#if DoubleBuffering>
    if (drvLayer[activeLayer].syncRectIndex < XLCDC_SYNC_RECT_COUNT)
    {
        drvLayer[activeLayer].syncRect[drvLayer[activeLayer].syncRectIndex] =
        (gfxRect)
        {
            .x = x,
            .y = y,
            .width = buf->size.width,
            .height = buf->size.height
        };

        drvLayer[activeLayer].syncRectIndex++;
    }
    else
    {
        drvLayer[activeLayer].syncEntireLayer = true;
    }
</#if>
<#if !XDMCPUBilt && le_gfx_gfx2d??>
<#if DoubleBuffering>
    result = gfxGPUInterface.blitBuffer(buf, &srcRect, &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx], &destRect);
<#else>
    result = gfxGPUInterface.blitBuffer(buf, &srcRect, &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx], &destRect);
</#if>

<#else>
<#if DoubleBuffering>
    result = DRV_XLCDC_CPU_Blit(buf,
                                &srcRect,
                                &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx],
                                &destRect);
<#else>
    result = DRV_XLCDC_CPU_Blit(buf,
                                &srcRect,
                                &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx],
                                &destRect);
</#if>

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
<#if DoubleBuffering>
            drvLayer[activeLayer].frontBufferIdx = drvLayer[activeLayer].backBufferIdx;

            drvLayer[activeLayer].backBufferIdx = (drvLayer[activeLayer].frontBufferIdx + 1) % XLCDC_BUF_PER_LAYER;

            XLCDC_SetLayerAddress(layerOrder[activeLayer], (uint32_t) drvLayer[activeLayer].baseaddr[drvLayer[activeLayer].frontBufferIdx], false);

            drvLayer[activeLayer].swapPending = true;

</#if>
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
            gfxDriverIOCTLResponse response = GFX_IOCTL_OK;

            val = (gfxIOCTLArg_Value *)arg;

            if (val->value.v_uint >= XLCDC_TOT_LAYERS)
            {
                response =  GFX_IOCTL_ERROR_UNKNOWN;
            }
            else
            {
                activeLayer = val->value.v_uint;
<#if DoubleBuffering>

                if (drvLayer[activeLayer].syncEntireLayer)
                {
                    gfxResult result = DRV_XLCDC_BlitBuffer(0, 0, &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx]);

                    drvLayer[activeLayer].syncEntireLayer = false;
                    drvLayer[activeLayer].syncRectIndex = 0;

                    if (result != GFX_SUCCESS)
                    {
                        response = GFX_IOCTL_ERROR_UNKNOWN;
                    }
                }
                else if (drvLayer[activeLayer].syncRectIndex > 0)
                {
                    while(drvLayer[activeLayer].syncRectIndex > 0)
                    {
                        drvLayer[activeLayer].syncRectIndex--;

<#if !XDMCPUBilt && le_gfx_gfx2d??>
                        gfxResult result = gfxGPUInterface.blitBuffer(&drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx],
                                                &drvLayer[activeLayer].syncRect[drvLayer[activeLayer].syncRectIndex],
                                                &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx],
                                                &drvLayer[activeLayer].syncRect[drvLayer[activeLayer].syncRectIndex]);
<#else>
                        gfxResult result = DRV_XLCDC_CPU_Blit(&drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx],
                                        &drvLayer[activeLayer].syncRect[drvLayer[activeLayer].syncRectIndex],
                                        &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx],
                                        &drvLayer[activeLayer].syncRect[drvLayer[activeLayer].syncRectIndex]);
</#if>

                        if (result != GFX_SUCCESS)
                        {
                            response = GFX_IOCTL_ERROR_UNKNOWN;
                        }
                    }
                }
</#if>
            }

            return response;
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

<#if DoubleBuffering>
            val->value.v_pbuffer = &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].backBufferIdx];
<#else>
            val->value.v_pbuffer = &drvLayer[activeLayer].pixelBuffer[drvLayer[activeLayer].frontBufferIdx];
</#if>

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
