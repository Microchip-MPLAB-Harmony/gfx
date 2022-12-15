#include "gfx/legato/renderer/legato_gpu.h"

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"

#if LE_RENDER_ORIENTATION == 0
#define GPU_ORIENTATION GFX_ORIENT_0
#elif LE_RENDER_ORIENTATION == 90
#define GPU_ORIENTATION GFX_ORIENT_90
#elif LE_RENDER_ORIENTATION == 180
#define GPU_ORIENTATION GFX_ORIENT_180
#else
#define GPU_ORIENTATION GFX_ORIENT_270
#endif

static gfxColorMode _convertColorMode(leColorMode mode)
{
    switch(mode)
    {
        case LE_COLOR_MODE_INDEX_1:    return GFX_COLOR_MODE_INDEX_1;
        case LE_COLOR_MODE_INDEX_4:    return GFX_COLOR_MODE_INDEX_4;
        case LE_COLOR_MODE_INDEX_8:    return GFX_COLOR_MODE_INDEX_8;
        case LE_COLOR_MODE_GS_8:       return GFX_COLOR_MODE_GS_8;
        case LE_COLOR_MODE_RGB_332:    return GFX_COLOR_MODE_RGB_332;
        case LE_COLOR_MODE_RGB_565:    return GFX_COLOR_MODE_RGB_565;
        case LE_COLOR_MODE_RGBA_5551:  return GFX_COLOR_MODE_RGBA_5551;
        case LE_COLOR_MODE_RGB_888:    return GFX_COLOR_MODE_RGB_888;
        case LE_COLOR_MODE_RGBA_8888:  return GFX_COLOR_MODE_RGBA_8888;
        case LE_COLOR_MODE_ARGB_8888:  return GFX_COLOR_MODE_ARGB_8888;
        case LE_COLOR_MODE_MONOCHROME: return GFX_COLOR_MODE_MONOCHROME;
    }

    return 0;
}

leResult leGPU_DrawLine(int32_t x0,
                        int32_t y0,
                        int32_t x1,
                        int32_t y1,
                        const leRect* clip,
                        leColor clr,
                        uint32_t a)
{
    gfxPixelBuffer buf;
    gfxPoint p0, p1;
    leRect frameRect;
    gfxRect clipRect;
    gfxColor drawClr;
    gfxResult res;
    const gfxGraphicsProcessor* intf = leRenderer_GPUInterface();
    lePixelBuffer* leBuf;

    if(intf == NULL || intf->drawLine == NULL)
        return LE_FAILURE;

    leBuf = leGetRenderBuffer();

    buf.pixel_count = leBuf->pixel_count;
    buf.size.width = leBuf->size.width;
    buf.size.height = leBuf->size.height;
    buf.mode = _convertColorMode(leBuf->mode);
    buf.buffer_length = leBuf->buffer_length;
    buf.flags = 0;
    buf.pixels = (gfxBuffer)leBuf->pixels;
    buf.orientation = GFX_ORIENT_0;

    leRenderer_GetClipRect(&frameRect);

    p0.x = frameRect.x - x0;
    p0.y = frameRect.y - y0;
    p1.x = frameRect.x - x1;
    p1.y = frameRect.y - y1;

    clipRect.x = frameRect.x - clip->x;
    clipRect.y = frameRect.y - clip->y;
    clipRect.width = clip->width;
    clipRect.height = clip->height;

#if LE_RENDER_ORIENTATION != 0
    leUtils_PointLogicalToScratch((int16_t*)&p0.x,
                                  (int16_t*)&p0.y);

    leUtils_PointLogicalToScratch((int16_t*)&p1.x,
                                  (int16_t*)&p1.y);

    leRect rotRect;
    rotRect.x = clipRect.x;
    rotRect.y = clipRect.y;
    rotRect.width = clipRect.width;
    rotRect.height = clipRect.height;

    leUtils_RectLogicalToScratch(&rotRect);

    clipRect.x = rotRect.x;
    clipRect.y = rotRect.y;
    clipRect.width = rotRect.width;
    clipRect.height = rotRect.height;
#endif

    drawClr = clr;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255 || LE_COLOR_MODE_IS_ALPHA((leColorMode)buf.mode) == 1)
    {
        intf->setGlobalAlpha(GFX_GLOBAL_ALPHA_ON,
                             GFX_GLOBAL_ALPHA_OFF,
                             a,
                             255);
    }
#else
    (void)a; // unused
#endif

    res = intf->drawLine(&buf,
                         &p0,
                         &p1,
                         &clipRect,
                         drawClr);

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255 || LE_COLOR_MODE_IS_ALPHA((leColorMode)buf.mode) == 1)
    {
        intf->setGlobalAlpha(GFX_GLOBAL_ALPHA_OFF,
                             GFX_GLOBAL_ALPHA_OFF,
                             255,
                             255);
    }
#endif

    return res == GFX_SUCCESS ? LE_SUCCESS : LE_FAILURE;
}

leResult leGPU_ClearBuffer(lePixelBuffer* leBuf)
{
    gfxPixelBuffer buf;
    gfxRect fillRect;
    gfxResult res;

    const gfxGraphicsProcessor* intf = leRenderer_GPUInterface();

    if(intf == NULL || intf->fillRect == NULL)
        return LE_FAILURE;

    buf.pixel_count = leBuf->pixel_count;
    buf.size.width = leBuf->size.width;
    buf.size.height = leBuf->size.height;
    buf.mode = _convertColorMode(leBuf->mode);
    buf.buffer_length = leBuf->buffer_length;
    buf.flags = 0;
    buf.pixels = (gfxBuffer)leBuf->pixels;
    buf.orientation = GFX_ORIENT_0;

    fillRect.x = 0;
    fillRect.y = 0;
    fillRect.width = leBuf->size.width;
    fillRect.height = leBuf->size.height;

    res = intf->fillRect(&buf,
                         &fillRect,
                         0);

    return res == GFX_SUCCESS ? LE_SUCCESS : LE_FAILURE;
}

leResult leGPU_FillRect(const leRect* rect,
                        leColor clr,
                        uint32_t a)
{
    gfxPixelBuffer buf;
    gfxRect fillRect;
    gfxColor drawClr;
    gfxResult res;
    lePixelBuffer* leBuf;

    const gfxGraphicsProcessor* intf = leRenderer_GPUInterface();

    if(intf == NULL || intf->fillRect == NULL)
        return LE_FAILURE;

    leBuf = leGetRenderBuffer();

    buf.pixel_count = leBuf->pixel_count;
    buf.size.width = leBuf->size.width;
    buf.size.height = leBuf->size.height;
    buf.mode = _convertColorMode(leBuf->mode);
    buf.buffer_length = leBuf->buffer_length;
    buf.flags = 0;
    buf.pixels = (gfxBuffer)leBuf->pixels;
    buf.orientation = GFX_ORIENT_0;

    fillRect.x = rect->x;
    fillRect.y = rect->y;
    fillRect.width = rect->width;
    fillRect.height = rect->height;

#if LE_RENDER_ORIENTATION != 0
    leRect rotRect;
    rotRect.x = fillRect.x;
    rotRect.y = fillRect.y;
    rotRect.width = fillRect.width;
    rotRect.height = fillRect.height;

    leUtils_RectLogicalToScratch(&rotRect);

    fillRect.x = rotRect.x;
    fillRect.y = rotRect.y;
    fillRect.width = rotRect.width;
    fillRect.height = rotRect.height;
#endif

    drawClr = clr;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255 || LE_COLOR_MODE_IS_ALPHA((leColorMode)buf.mode) == 1)
    {
        intf->setGlobalAlpha(GFX_GLOBAL_ALPHA_ON,
                             GFX_GLOBAL_ALPHA_OFF,
                             a,
                             255);
    }
#else
    (void)a; // unused
#endif

    res = intf->fillRect(&buf,
                         &fillRect,
                         drawClr);

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255 || LE_COLOR_MODE_IS_ALPHA((leColorMode)buf.mode) == 1)
    {
        intf->setGlobalAlpha(GFX_GLOBAL_ALPHA_OFF,
                             GFX_GLOBAL_ALPHA_OFF,
                             255,
                             255);
    }
#endif

    return res == GFX_SUCCESS ? LE_SUCCESS : LE_FAILURE;
}

leResult leGPU_BlitBuffer(const lePixelBuffer* sourceBuffer,
                          const leRect* sourceRect,
                          const leRect* destRect,
                          uint32_t a)
{
    gfxPixelBuffer sourceBuf, destBuf;
    gfxRect gfxSourceRect, gfxDestRect;
    leRect frameRect;
    gfxResult res;
    lePixelBuffer* leBuf;

    const gfxGraphicsProcessor* intf = leRenderer_GPUInterface();

    if(intf == NULL || intf->blitBuffer == NULL)
        return LE_FAILURE;

    gfxSourceRect.x = sourceRect->x;
    gfxSourceRect.y = sourceRect->y;
    gfxSourceRect.width = sourceRect->width;
    gfxSourceRect.height = sourceRect->height;

    sourceBuf.pixel_count = sourceBuffer->pixel_count;
    sourceBuf.size.width = sourceBuffer->size.width;
    sourceBuf.size.height = sourceBuffer->size.height;
    sourceBuf.mode = _convertColorMode(sourceBuffer->mode);
    sourceBuf.buffer_length = sourceBuffer->buffer_length;
    sourceBuf.flags = 0;
    sourceBuf.pixels = (gfxBuffer)sourceBuffer->pixels;
    sourceBuf.orientation = GFX_ORIENT_0;

    leBuf = leGetRenderBuffer();

    destBuf.pixel_count = leBuf->pixel_count;
    destBuf.size.width = leBuf->size.width;
    destBuf.size.height = leBuf->size.height;
    destBuf.mode = _convertColorMode(leBuf->mode);
    destBuf.buffer_length = leBuf->buffer_length;
    destBuf.flags = 0;
    destBuf.pixels = (gfxBuffer)leBuf->pixels;
    destBuf.orientation = GPU_ORIENTATION;

    leRenderer_GetFrameRect(&frameRect);

    gfxDestRect.x = destRect->x - frameRect.x;
    gfxDestRect.y = destRect->y - frameRect.y;
    gfxDestRect.width = destRect->width;
    gfxDestRect.height = destRect->height;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255 || LE_COLOR_MODE_IS_ALPHA(sourceBuffer->mode) == 1)
    {
        intf->setGlobalAlpha(GFX_GLOBAL_ALPHA_SCALE,
                             GFX_GLOBAL_ALPHA_OFF,
                             a,
                             255);

        intf->setBlend(GFX_BLEND_SRC_OVER);
    }
#else
    (void)a; // unused
#endif

    res = intf->blitBuffer(&sourceBuf,
                           &gfxSourceRect,
                           &destBuf,
                           &gfxDestRect);

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255 || LE_COLOR_MODE_IS_ALPHA(sourceBuffer->mode) == 1)
    {
        intf->setGlobalAlpha(GFX_GLOBAL_ALPHA_OFF,
                             GFX_GLOBAL_ALPHA_OFF,
                             255,
                             255);

        intf->setBlend(GFX_BLEND_NONE);
    }
#endif

    return res == GFX_SUCCESS ? LE_SUCCESS : LE_FAILURE;
}

leResult leGPU_BlitStretchBuffer(const lePixelBuffer* sourceBuffer,
                                 const leRect* sourceRect,
                                 const leRect* destRect,
                                 uint32_t a)
{
    gfxPixelBuffer sourceBuf, destBuf;
    gfxRect gfxSourceRect, gfxDestRect;
    leRect frameRect;
    gfxResult res;
    lePixelBuffer* leBuf;

    const gfxGraphicsProcessor* intf = leRenderer_GPUInterface();

    if(intf == NULL || intf->blitBuffer == NULL)
        return LE_FAILURE;

    gfxSourceRect.x = sourceRect->x;
    gfxSourceRect.y = sourceRect->y;
    gfxSourceRect.width = sourceRect->width;
    gfxSourceRect.height = sourceRect->height;

    sourceBuf.pixel_count = sourceBuffer->pixel_count;
    sourceBuf.size.width = sourceBuffer->size.width;
    sourceBuf.size.height = sourceBuffer->size.height;
    sourceBuf.mode = _convertColorMode(sourceBuffer->mode);
    sourceBuf.buffer_length = sourceBuffer->buffer_length;
    sourceBuf.flags = 0;
    sourceBuf.pixels = (gfxBuffer)sourceBuffer->pixels;
    sourceBuf.orientation = GFX_ORIENT_0;

    leBuf = leGetRenderBuffer();

    destBuf.pixel_count = leBuf->pixel_count;
    destBuf.size.width = leBuf->size.width;
    destBuf.size.height = leBuf->size.height;
    destBuf.mode = _convertColorMode(leBuf->mode);
    destBuf.buffer_length = leBuf->buffer_length;
    destBuf.flags = 0;
    destBuf.pixels = (gfxBuffer)leBuf->pixels;
    destBuf.orientation = GPU_ORIENTATION;

    leRenderer_GetFrameRect(&frameRect);

    gfxDestRect.x = destRect->x - frameRect.x;
    gfxDestRect.y = destRect->y - frameRect.y;
    gfxDestRect.width = destRect->width;
    gfxDestRect.height = destRect->height;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255 || LE_COLOR_MODE_IS_ALPHA(sourceBuffer->mode) == 1)
    {
        intf->setGlobalAlpha(GFX_GLOBAL_ALPHA_ON,
                             GFX_GLOBAL_ALPHA_OFF,
                             a,
                             255);
    }
#else
    (void)a; // unused
#endif

    res = intf->blitBuffer(&sourceBuf,
                           &gfxSourceRect,
                           &destBuf,
                           &gfxDestRect);

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255 || LE_COLOR_MODE_IS_ALPHA(sourceBuffer->mode) == 1)
    {
        intf->setGlobalAlpha(GFX_GLOBAL_ALPHA_OFF,
                             GFX_GLOBAL_ALPHA_OFF,
                             255,
                             255);
    }
#endif

    return res == GFX_SUCCESS ? LE_SUCCESS : LE_FAILURE;
}
