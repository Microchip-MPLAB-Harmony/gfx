#include "gfx/legato/renderer/legato_gpu.h"

extern leRenderState _rendererState;

static gfxColorMode _convertColorMode(leColorMode mode)
{
    switch(mode)
    {
        case LE_COLOR_MODE_INDEX_1:   return GFX_COLOR_MODE_INDEX_1;
        case LE_COLOR_MODE_INDEX_4:   return GFX_COLOR_MODE_INDEX_4;
        case LE_COLOR_MODE_INDEX_8:   return GFX_COLOR_MODE_INDEX_8;
        case LE_COLOR_MODE_GS_8:      return GFX_COLOR_MODE_GS_8;
        case LE_COLOR_MODE_RGB_332:   return GFX_COLOR_MODE_RGB_332;
        case LE_COLOR_MODE_RGB_565:   return GFX_COLOR_MODE_RGB_565;
        case LE_COLOR_MODE_RGBA_5551: return GFX_COLOR_MODE_RGBA_5551;
        case LE_COLOR_MODE_RGB_888:   return GFX_COLOR_MODE_RGB_888;
        case LE_COLOR_MODE_RGBA_8888: return GFX_COLOR_MODE_RGBA_8888;
        case LE_COLOR_MODE_ARGB_8888: return GFX_COLOR_MODE_ARGB_8888;
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
    gfxRect clipRect;
    gfxColor drawClr;

    if(_rendererState.gpuDriver == NULL ||
       _rendererState.gpuDriver->drawLine == NULL)
        return LE_FAILURE;

    buf.pixel_count = _rendererState.renderBuffer->pixel_count;
    buf.size.width = _rendererState.renderBuffer->size.width;
    buf.size.height = _rendererState.renderBuffer->size.height;
    buf.mode = _convertColorMode(_rendererState.renderBuffer->mode);
    buf.buffer_length = _rendererState.renderBuffer->buffer_length;
    buf.flags = 0;
    buf.pixels = (gfxBuffer)_rendererState.renderBuffer->pixels;

    p0.x = x0;
    p0.y = y0;
    p1.x = x1;
    p1.y = y1;

    clipRect.x = clip->x;
    clipRect.y = clip->y;
    clipRect.width = clip->width;
    clipRect.height = clip->height;

    drawClr = clr;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a <= 255)
    {
        _rendererState.gpuDriver->setGlobalAlpha(GFX_GLOBAL_ALPHA_ON,
                                                 GFX_GLOBAL_ALPHA_OFF,
                                                 a,
                                                 255);
    }
#endif

    _rendererState.gpuDriver->drawLine(&buf,
                                       &p0,
                                       &p1,
                                       &clipRect,
                                       drawClr);

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a <= 255)
    {
        _rendererState.gpuDriver->setGlobalAlpha(GFX_GLOBAL_ALPHA_OFF,
                                                 GFX_GLOBAL_ALPHA_OFF,
                                                 255,
                                                 255);
    }
#endif

    return LE_SUCCESS;
}

leResult leGPU_FillRect(const leRect* rect,
                        leColor clr,
                        uint32_t a)
{
    gfxPixelBuffer buf;
    gfxRect fillRect;
    gfxColor drawClr;

    if(_rendererState.gpuDriver == NULL ||
       _rendererState.gpuDriver->fillRect == NULL)
        return LE_FAILURE;

    buf.pixel_count = _rendererState.renderBuffer->pixel_count;
    buf.size.width = _rendererState.renderBuffer->size.width;
    buf.size.height = _rendererState.renderBuffer->size.height;
    buf.mode = _convertColorMode(_rendererState.renderBuffer->mode);
    buf.buffer_length = _rendererState.renderBuffer->buffer_length;
    buf.flags = 0;
    buf.pixels = (gfxBuffer)_rendererState.renderBuffer->pixels;

    fillRect.x = rect->x;
    fillRect.y = rect->y;
    fillRect.width = rect->width;
    fillRect.height = rect->height;

    drawClr = clr;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a <= 255)
    {
        _rendererState.gpuDriver->setGlobalAlpha(GFX_GLOBAL_ALPHA_ON,
                                                 GFX_GLOBAL_ALPHA_OFF,
                                                 a,
                                                 255);
    }
#endif

    _rendererState.gpuDriver->fillRect(&buf,
                                       &fillRect,
                                       drawClr);

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a <= 255)
    {
        _rendererState.gpuDriver->setGlobalAlpha(GFX_GLOBAL_ALPHA_OFF,
                                                 GFX_GLOBAL_ALPHA_OFF,
                                                 255,
                                                 255);
    }
#endif

    return LE_SUCCESS;
}

leResult leGPU_BlitBuffer(const lePixelBuffer* sourceBuffer,
                          const leRect* sourceRect,
                          const leRect* destRect,
                          uint32_t a)
{
    gfxPixelBuffer sourceBuf, destBuf;
    gfxRect gfxSourceRect, gfxDestRect;

    if(_rendererState.gpuDriver == NULL ||
       _rendererState.gpuDriver->blitBuffer == NULL)
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

    destBuf.pixel_count = _rendererState.renderBuffer->pixel_count;
    destBuf.size.width = _rendererState.renderBuffer->size.width;
    destBuf.size.height = _rendererState.renderBuffer->size.height;
    destBuf.mode = _convertColorMode(_rendererState.renderBuffer->mode);
    destBuf.buffer_length = _rendererState.renderBuffer->buffer_length;
    destBuf.flags = 0;
    destBuf.pixels = (gfxBuffer)_rendererState.renderBuffer->pixels;

    gfxDestRect.x = destRect->x;
    gfxDestRect.y = destRect->y;
    gfxDestRect.width = destRect->width;
    gfxDestRect.height = destRect->height;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a <= 255)
    {
        _rendererState.gpuDriver->setGlobalAlpha(GFX_GLOBAL_ALPHA_ON,
                                                 GFX_GLOBAL_ALPHA_OFF,
                                                 a,
                                                 255);
    }
#endif

    _rendererState.gpuDriver->blitBuffer(&sourceBuf,
                                         &gfxSourceRect,
                                         &destBuf,
                                         &gfxDestRect);

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a <= 255)
    {
        _rendererState.gpuDriver->setGlobalAlpha(GFX_GLOBAL_ALPHA_OFF,
                                                 GFX_GLOBAL_ALPHA_OFF,
                                                 255,
                                                 255);
    }
#endif

    return LE_SUCCESS;
}

leResult leGPU_BlitStretchBuffer(const lePixelBuffer* sourceBuffer,
                                 const leRect* sourceRect,
                                 const leRect* destRect,
                                 uint32_t a)
{
    gfxPixelBuffer sourceBuf, destBuf;
    gfxRect gfxSourceRect, gfxDestRect;

    if(_rendererState.gpuDriver == NULL ||
       _rendererState.gpuDriver->blitBuffer == NULL)
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

    destBuf.pixel_count = _rendererState.renderBuffer->pixel_count;
    destBuf.size.width = _rendererState.renderBuffer->size.width;
    destBuf.size.height = _rendererState.renderBuffer->size.height;
    destBuf.mode = _convertColorMode(_rendererState.renderBuffer->mode);
    destBuf.buffer_length = _rendererState.renderBuffer->buffer_length;
    destBuf.flags = 0;
    destBuf.pixels = (gfxBuffer)_rendererState.renderBuffer->pixels;

    gfxDestRect.x = destRect->x;
    gfxDestRect.y = destRect->y;
    gfxDestRect.width = destRect->width;
    gfxDestRect.height = destRect->height;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a <= 255)
    {
        _rendererState.gpuDriver->setGlobalAlpha(GFX_GLOBAL_ALPHA_ON,
                                                 GFX_GLOBAL_ALPHA_OFF,
                                                 a,
                                                 255);
    }
#endif

    _rendererState.gpuDriver->blitBuffer(&sourceBuf,
                                         &gfxSourceRect,
                                         &destBuf,
                                         &gfxDestRect);

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a <= 255)
    {
        _rendererState.gpuDriver->setGlobalAlpha(GFX_GLOBAL_ALPHA_OFF,
                                                 GFX_GLOBAL_ALPHA_OFF,
                                                 255,
                                                 255);
    }
#endif

    return LE_SUCCESS;
}