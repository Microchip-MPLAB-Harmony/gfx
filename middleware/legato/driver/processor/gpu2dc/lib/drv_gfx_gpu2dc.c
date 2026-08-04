// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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

#include "gfx/driver/gpu2dc/drv_gfx_gpu2dc.h"
#include "nano2D.h"

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/* GPU2DC Stride Alignment Check */
#define GFX_STRIDE_ALIGN_FAILS(w, m, p) ( \
    ((uintptr_t)(p) & 0xF) != 0 || ( \
    ((m)==0 || (m)==9)  ? ((w) & 0x0F) != 0 : /* 1bpp:  w%16,  stride=16B */ \
    ((m)==2 || (m)==3)  ? ((w) & 0x07) != 0 : /* 2bpp:  w%8,   stride=16B */ \
    ((m)==4)            ? ((w) & 0x0F) != 0 : /* 3bpp:  w%16,  stride=48B */ \
    ((m)==5 || (m)==6)  ? ((w) & 0x03) != 0 : /* 4bpp:  w%4,   stride=16B */ \
    ((m)==7)            ? ((w) & 0x7F) != 0 : /* 1bpp indexed: w%128, stride=16B */ \
    1)) /* unsupported formats (m=1,8,10) always fail */

/* GPU2DC Driver Heap Size */
#define GPU_HEAP_SIZE 0x100000U // 1MB
__attribute__ ((section(".region_nocache"), aligned (32))) uint8_t gpu_heap[GPU_HEAP_SIZE];

static gfxBlend blendState = GFX_BLEND_NONE;

static n2d_buffer_format_t n2dFormats[GFX_COLOR_MODE_LAST + 1] =
{
    N2D_A8,             // GFX_COLOR_MODE_GS_8
    -1,                 // GFX_COLOR_MODE_RGB_332
    N2D_RGB565,         // GFX_COLOR_MODE_RGB_565
    N2D_R5G5B5A1,       // GFX_COLOR_MODE_RGBA_5551
    N2D_RGB888,         // GFX_COLOR_MODE_RGB_888
    N2D_RGBA8888,       // GFX_COLOR_MODE_RGBA_8888
    N2D_ARGB8888,       // GFX_COLOR_MODE_ARGB_8888
    N2D_INDEX1,         // GFX_COLOR_MODE_INDEX_1
    -1,                 // GFX_COLOR_MODE_INDEX_4
    N2D_INDEX8,         // GFX_COLOR_MODE_INDEX_8
    -1,                 // GFX_COLOR_MODE_MONOCHROME
};

#ifdef __ARM_NEON
/* CPU NEON blit used as fallback when the GPU blit fails.
   Only handles GFX_BLEND_NONE (straight copy). Blend modes requiring
   alpha compositing are not supported and must go through the GPU. */
static gfxResult DRV_GPU2DC_NEON_Blit(const gfxPixelBuffer* source,
                                       const gfxRect* srcRect,
                                       const gfxPixelBuffer* dest,
                                       const gfxRect* destRect)
{
    const uint32_t width  = MIN(srcRect->width,  destRect->width);
    const uint32_t height = MIN(srcRect->height, destRect->height);

    if (width == 0 || height == 0)
        return GFX_FAILURE;

    const uint32_t srcPixelSize  = gfxColorInfoTable[source->mode].size;
    const uint32_t destPixelSize = gfxColorInfoTable[dest->mode].size;
    const uint32_t rowSize       = width * destPixelSize;
    const uint32_t srcStride     = source->size.width * srcPixelSize;
    const uint32_t destStride    = dest->size.width   * destPixelSize;

    uint8_t* srcBase  = (uint8_t*)gfxPixelBufferOffsetGet(source, srcRect->x,  srcRect->y);
    uint8_t* destBase = (uint8_t*)gfxPixelBufferOffsetGet(dest,   destRect->x, destRect->y);

    /* Contiguous path: both buffers full-width and same format */
    if (width == source->size.width && width == dest->size.width
        && srcPixelSize == destPixelSize)
    {
        const uint32_t totalSize = rowSize * height;

        if (totalSize >= 16)
        {
            uint8_t* src = srcBase;
            uint8_t* dst = destBase;
            uint32_t vectors = totalSize / 16;
            uint32_t remain  = totalSize & 15;

            __builtin_prefetch(src);
            __builtin_prefetch(src + 32);
            __builtin_prefetch(src + 64);
            __builtin_prefetch(src + 96);

            while (vectors--)
            {
                __builtin_prefetch(src + 128);
                __builtin_prefetch(src + 160);
                vst1q_u8(dst, vld1q_u8(src));
                src += 16;
                dst += 16;
            }

            if (remain)
                memcpy(dst, src, remain);
        }
        else
        {
            memcpy(destBase, srcBase, totalSize);
        }

        return GFX_SUCCESS;
    }

    /* Row-by-row path for partial or format-differing blits */
    for (uint32_t row = 0; row < height; row++)
    {
        uint8_t* src = srcBase  + row * srcStride;
        uint8_t* dst = destBase + row * destStride;

        if (rowSize >= 16)
        {
            uint32_t vectors = rowSize / 16;
            uint32_t remain  = rowSize & 15;

            __builtin_prefetch(src);
            __builtin_prefetch(src + 32);
            __builtin_prefetch(src + 64);
            __builtin_prefetch(src + 96);

            if (row < height - 1)
            {
                __builtin_prefetch(src + srcStride);
                __builtin_prefetch(src + srcStride + 32);
                __builtin_prefetch(src + srcStride + 64);
                __builtin_prefetch(src + srcStride + 96);
            }

            uint8_t* vs = src;
            uint8_t* vd = dst;

            while (vectors--)
            {
                __builtin_prefetch(vs + 128);
                __builtin_prefetch(vs + 160);
                vst1q_u8(vd, vld1q_u8(vs));
                vs += 16;
                vd += 16;
            }

            if (remain)
                memcpy(vd, vs, remain);
        }
        else
        {
            memcpy(dst, src, rowSize);
        }
    }

    return GFX_SUCCESS;
}
#endif /* __ARM_NEON */

gfxResult DRV_GPU2DC_Line(gfxPixelBuffer* dest, const gfxPoint* p1, const gfxPoint* p2, const gfxRect* clipRect, const gfxColor color)
{
    if (!dest || !p1 || !p2 || !dest->pixels)
        return GFX_FAILURE;

    n2d_buffer_t buffer = {0};
    n2d_point_t np1 = {p1->x, p1->y};
    n2d_point_t np2 = {p2->x, p2->y};

    /* Extend vertical/horizontal lines by 1 pixel so the GPU rasterizer
       sees non-zero extent.  Always grow the larger coordinate to avoid
       producing negative values that fail the hardware bounds check. */
    if (np1.x == np2.x) {
        if (np1.y <= np2.y) np2.y += 1;
        else                np1.y += 1;
    } else if (np1.y == np2.y) {
        if (np1.x <= np2.x) np2.x += 1;
        else                np1.x += 1;
    }

    /* Clamp endpoints to buffer bounds.  The clip rect will constrain
       the visible output so this does not change the rendered result
       for horizontal/vertical lines. */
    if (np1.x < 0) np1.x = 0;
    if (np1.y < 0) np1.y = 0;
    if (np2.x < 0) np2.x = 0;
    if (np2.y < 0) np2.y = 0;
    if (np1.x > dest->size.width)  np1.x = dest->size.width;
    if (np1.y > dest->size.height) np1.y = dest->size.height;
    if (np2.x > dest->size.width)  np2.x = dest->size.width;
    if (np2.y > dest->size.height) np2.y = dest->size.height;

    buffer.width = dest->size.width;
    buffer.height = dest->size.height;
    buffer.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    buffer.format = n2dFormats[dest->mode];
    buffer.orientation = dest->orientation;
    buffer.tiling = N2D_LINEAR;
    buffer.memory = dest->pixels;
    buffer.gpu = (n2d_uintptr_t)dest->pixels;

    n2d_error_t err = n2d_line(&buffer, np1, np2, (n2d_rectangle_t*)clipRect, gfxColorConvert(dest->mode, GFX_COLOR_MODE_ARGB_8888, color), blendState);
    if (N2D_IS_ERROR(err)) return GFX_FAILURE;

    err = n2d_commit();
    if (N2D_IS_ERROR(err)) return GFX_FAILURE;

    return GFX_SUCCESS;
}

gfxResult DRV_GPU2DC_Fill(gfxPixelBuffer* dest, const gfxRect* clipRect, const gfxColor color)
{
    if (!dest || !dest->pixels)
        return GFX_FAILURE;

    n2d_buffer_t buffer = {0};
    buffer.width = dest->size.width;
    buffer.height = dest->size.height;
    buffer.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    buffer.format = n2dFormats[dest->mode];
    buffer.orientation = dest->orientation;
    buffer.tiling = N2D_LINEAR;
    buffer.memory = dest->pixels;
    buffer.gpu = (n2d_uintptr_t)dest->pixels;

    n2d_error_t err = n2d_fill(&buffer, (n2d_rectangle_t*)clipRect, gfxColorConvert(dest->mode, GFX_COLOR_MODE_ARGB_8888, color), blendState);
    if (N2D_IS_ERROR(err)) return GFX_FAILURE;

    err = n2d_commit();
    if (N2D_IS_ERROR(err)) return GFX_FAILURE;

    return GFX_SUCCESS;
}

gfxResult DRV_GPU2DC_Blit(const gfxPixelBuffer* source, const gfxRect* srcRect, const gfxPixelBuffer* dest, const gfxRect* destRect)
{
    if (!source || !source->pixels || !dest || !dest->pixels)
        return GFX_FAILURE;

    if (GFX_STRIDE_ALIGN_FAILS(source->size.width, source->mode, source->pixels))
        goto gpu_fail;

    n2d_buffer_t srcBuf = {0};
    srcBuf.width = source->size.width;
    srcBuf.height = source->size.height;
    srcBuf.stride = source->size.width * gfxColorInfoTable[source->mode].size;
    srcBuf.format = n2dFormats[source->mode];
    srcBuf.orientation = source->orientation;
    srcBuf.tiling = N2D_LINEAR;
    srcBuf.memory = source->pixels;
    srcBuf.gpu = (n2d_uintptr_t)source->pixels;

    n2d_buffer_t dstBuf = {0};
    dstBuf.width = dest->size.width;
    dstBuf.height = dest->size.height;
    dstBuf.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    dstBuf.format = n2dFormats[dest->mode];
    dstBuf.orientation = dest->orientation;
    dstBuf.tiling = N2D_LINEAR;
    dstBuf.memory = dest->pixels;
    dstBuf.gpu = (n2d_uintptr_t)dest->pixels;

    n2d_error_t err = n2d_blit(&dstBuf, (n2d_rectangle_t*)destRect, &srcBuf, (n2d_rectangle_t*)srcRect, blendState);
    if (N2D_IS_ERROR(err)) goto gpu_fail;

    err = n2d_commit();
    if (N2D_IS_ERROR(err)) goto gpu_fail;

    return GFX_SUCCESS;

gpu_fail:
#ifdef __ARM_NEON
    if (blendState == GFX_BLEND_NONE)
        return DRV_GPU2DC_NEON_Blit(source, srcRect, dest, destRect);
#endif
    return GFX_FAILURE;
}

gfxResult DRV_GPU2DC_SetBlend(const gfxBlend blend)
{
    blendState = blend;

    return GFX_SUCCESS;
}

gfxResult DRV_GPU2DC_SetGlobalAlpha(const gfxAlpha srcAlpha, const gfxAlpha dstAlpha, uint32_t srcVal, uint32_t dstVal)
{
    n2d_state_config_t sc = {N2D_SET_GLOBAL_ALPHA, {{0}}};

    sc.config.globalAlpha.srcMode = srcAlpha;
    sc.config.globalAlpha.dstMode = dstAlpha;
    sc.config.globalAlpha.srcValue = srcVal;
    sc.config.globalAlpha.dstValue = dstVal;

    return (N2D_IS_ERROR(n2d_set(&sc))) ? GFX_FAILURE : GFX_SUCCESS;
}

gfxResult DRV_GPU2DC_SetTransparency(gfxTransparency transparency, gfxColor color, uint32_t fg_rop, uint32_t bg_rop)
{
    n2d_error_t error;

    n2d_state_config_t rop = {N2D_SET_ROP, {{0}}};
    n2d_state_config_t sc = {N2D_SET_TRANSPARENCY, {{0}}};
    n2d_state_config_t key = {N2D_SET_DST_COLORKEY, {{0}}};

    rop.config.rop.fg_rop = fg_rop;
    rop.config.rop.bg_rop = bg_rop;

    N2D_ON_ERROR(n2d_set(&rop));

    switch (transparency)
    {
        case GFX_TRANSPARENCY_NONE:
        {
            sc.config.transparency.src = N2D_OPAQUE;
            sc.config.transparency.dst = N2D_OPAQUE;
            sc.config.transparency.pat = N2D_OPAQUE;

            N2D_ON_ERROR(n2d_set(&sc));

            break;
        }
        case GFX_TRANSPARENCY_SOURCE:
        {
            sc.config.transparency.src = N2D_KEYED;
            sc.config.transparency.dst = N2D_OPAQUE;
            sc.config.transparency.pat = N2D_OPAQUE;

            key.state = N2D_SET_SRC_COLORKEY;
            key.config.srcColorKey.colorkeyLow  = color;
            key.config.srcColorKey.colorkeyHigh = color;

            N2D_ON_ERROR(n2d_set(&key));
            N2D_ON_ERROR(n2d_set(&sc));

            break;
        }

        case GFX_TRANSPARENCY_DESTINATION:
        {
            sc.config.transparency.src = N2D_OPAQUE;
            sc.config.transparency.dst = N2D_KEYED;
            sc.config.transparency.pat = N2D_OPAQUE;

            key.state = N2D_SET_DST_COLORKEY;
            key.config.dstColorKey.colorkeyLow  = color;
            key.config.dstColorKey.colorkeyHigh = color;

            N2D_ON_ERROR(n2d_set(&key));
            N2D_ON_ERROR(n2d_set(&sc));

            break;
        }

        default:
            goto on_error;
    }

    return GFX_SUCCESS;

on_error:
    return GFX_FAILURE;
}

gfxResult DRV_GPU2DC_SetPalette(uint32_t indexCount, gfxBuffer colorTable, gfxBool convert)
{
    return (N2D_IS_ERROR(n2d_load_palette(0, indexCount, colorTable, convert))) ? GFX_FAILURE : GFX_SUCCESS;
}

void DRV_GPU2DC_Initialize(void)
{
    n2d_init(gpu_heap, GPU_HEAP_SIZE);
    n2d_open();
}

void DRV_GPU2DC_Handle_Interrupt(void)
{
    n2d_handle_events();
}

const gfxGraphicsProcessor gfxGPUInterface =
{
    DRV_GPU2DC_Line,
    DRV_GPU2DC_Fill,
    DRV_GPU2DC_Blit,
    DRV_GPU2DC_Blit,
    DRV_GPU2DC_SetBlend,
    DRV_GPU2DC_SetGlobalAlpha,
    DRV_GPU2DC_SetPalette,
    DRV_GPU2DC_SetTransparency
};
