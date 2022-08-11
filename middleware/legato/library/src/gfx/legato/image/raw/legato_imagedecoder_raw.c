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


#include "gfx/legato/image/legato_image.h"

#if LE_ENABLE_RAW_DECODER == 1

#include "gfx/legato/image/raw/legato_imagedecoder_raw.h"

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/core/legato_stream.h"
#include "gfx/legato/image/legato_palette.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/renderer/legato_gpu.h"

#if LE_STREAMING_ENABLED == 1

uint8_t LE_COHERENT_ATTR leRawImageDecoderScratchBuffer[LE_ASSET_DECODER_PIXEL_CACHE_SIZE];
uint8_t LE_COHERENT_ATTR leRawImageDecoderPaletteScratchBuffer[LE_ASSET_DECODER_PALETTE_CACHE_SIZE];
uint8_t LE_COHERENT_ATTR leRawImageDecoderMashScratchBuffer[LE_ASSET_DECODER_MASK_CACHE_SIZE];
uint8_t LE_COHERENT_ATTR leRawImageDecoderBlendBuffer[LE_ASSET_DECODER_BLEND_CACHE_SIZE];

#endif

static LE_COHERENT_ATTR leImageDecoder _decoder;
static LE_COHERENT_ATTR leRawDecodeState _state;

#if LE_STREAMING_ENABLED == 1

static leResult _exec(leStreamManager* mgr);
static leBool _isDone(leStreamManager* mgr);
static void _abort(leStreamManager* mgr);
static void _cleanup(leStreamManager* mgr);

#endif

leResult _leRawImageDecoder_SourceIterateSetupStage(leRawDecodeState* state);
leResult _leRawImageDecoder_TargetIterateSetupStage(leRawDecodeState* state);
//leResult _leRawImageDecoder_RotatedTargetIterateSetupStage(leRawDecodeState* state);

leResult _leRawImageDecoder_PostReadStage(leRawDecodeState* state);

leResult _leRawImageDecoder_ScaleNearestNeighborPreReadStage(leRawDecodeState* state);
leResult _leRawImageDecoder_ScaleBilinearPreReadStage(leRawDecodeState* state);
leResult _leRawImageDecoder_ScaleBilinearPostReadStage(leRawDecodeState* state);

leResult _leRawImageDecoder_RotateNearestNeighborPreReadStage(leRawDecodeState* state);
leResult _leRawImageDecoder_RotateBilinearPreReadStage(leRawDecodeState* state);
leResult _leRawImageDecoder_RotateBilinearPostReadStage(leRawDecodeState* state);


leResult _leRawImageDecoder_ReadStage_Internal(leRawDecodeState* state);
#if LE_STREAMING_ENABLED == 1
leResult _leRawImageDecoder_ReadStage_StreamColor(leRawDecodeState* state);
leResult _leRawImageDecoder_ReadStage_StreamIndex(leRawDecodeState* state);
leResult _leRawImageDecoder_ReadStage_StreamRLE(leRawDecodeState* state);
leResult _leRawImageDecoder_ReadStage_StreamRLEIndex(leRawDecodeState* state);
#endif

leResult _leRawImageDecoder_MaskStage_Internal(leRawDecodeState* state);

leResult _leRawImageDecoder_LookupStage_Internal(leRawDecodeState* state);
#if LE_STREAMING_ENABLED == 1
leResult _leRawImageDecoder_LookupStage_Stream(leRawDecodeState* state);
#endif

leResult _leRawImageDecoder_ImageRenderPostLookupStage(leRawDecodeState* state);
leResult _leRawImageDecoder_ImageRenderPostLookupStage_Stream(leRawDecodeState* state);

leResult _leRawImageDecoder_ConvertStage(leRawDecodeState* state);

leResult _leRawImageDecoder_BlendStage_Internal(leRawDecodeState* state);

leResult _leRawImageDecoder_FrameBufferWriteStage(leRawDecodeState* state);
leResult _leRawImageDecoder_ImageWriteStage(leRawDecodeState* state);

static leBool _supportsImage(const leImage* img)
{
    return img->format == LE_IMAGE_FORMAT_RAW ||
           img->format == LE_IMAGE_FORMAT_RLE;
}

static leResult _initReadStage(leRawDecodeState* st)
{
    // initialize read stage
#if LE_STREAMING_ENABLED == 1
    if(st->source->header.location == LE_STREAM_LOCATION_ID_INTERNAL)
    {
#endif
        if(_leRawImageDecoder_ReadStage_Internal(st) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
#if LE_STREAMING_ENABLED == 1
    }
    else
    {
        if(st->source->format == LE_IMAGE_FORMAT_RAW)
        {
            if(st->source->palette != NULL)
            {
                _leRawImageDecoder_ReadStage_StreamIndex(st);
            }
            else
            {
                _leRawImageDecoder_ReadStage_StreamColor(st);
            }
        }
        else
        {
            if(st->source->palette != NULL)
            {
                _leRawImageDecoder_ReadStage_StreamRLEIndex(st);
            }
            else
            {
                _leRawImageDecoder_ReadStage_StreamRLE(st);
            }
        }
    }
#endif

    return LE_SUCCESS;
}

static leResult _initMaskStage(leRawDecodeState* state)
{
    if((state->source->flags & LE_IMAGE_USE_MASK_COLOR) > 0)
    {
        return _leRawImageDecoder_MaskStage_Internal(state);
    }

    return LE_SUCCESS;
}

static leResult _initLookupStage(leRawDecodeState* state)
{
    if(LE_COLOR_MODE_IS_INDEX(state->source->buffer.mode) == LE_TRUE &&
       state->source->palette != NULL)
    {
#if LE_STREAMING_ENABLED == 1
        if(state->source->palette->header.location == LE_STREAM_LOCATION_ID_INTERNAL)
        {
#endif
            if(_leRawImageDecoder_LookupStage_Internal(state) == LE_FAILURE)
            {
                return LE_FAILURE;
            }
#if LE_STREAMING_ENABLED == 1
        }
        else
        {
            return _leRawImageDecoder_LookupStage_Stream(state);
        }
#endif
    }

    return LE_SUCCESS;
}

static leResult _initConvertStage(leRawDecodeState* state)
{
    if(state->source->palette != NULL && state->source->palette->buffer.mode == state->targetMode)
        return LE_SUCCESS;

    if(state->source->buffer.mode  == state->targetMode)
        return LE_SUCCESS;

    return _leRawImageDecoder_ConvertStage(state);
}

static leResult _initBlendStage(leRawDecodeState* state)
{
    if(((state->source->flags & LE_IMAGE_USE_ALPHA_MAP) > 0 && state->source->alphaMap != NULL) ||
        state->source->buffer.mode == LE_COLOR_MODE_RGBA_5551 ||
        state->source->buffer.mode == LE_COLOR_MODE_RGBA_8888 ||
        state->source->buffer.mode == LE_COLOR_MODE_ARGB_8888)
    {
        return _leRawImageDecoder_BlendStage_Internal(state);
    }

    return LE_SUCCESS;
}

static void _directBlit(const lePixelBuffer* src,
                        const leRect* srcRect,
                        const leRect* destRect)
{
    void* srcPtr;
    void* destPtr;
    lePixelBuffer dest;
    int32_t row, rowSize;
    leRect frameRect;

    leRenderer_GetFrameRect(&frameRect);

    dest.pixel_count = leGetRenderBuffer()->pixel_count;
    dest.size.width = leGetRenderBuffer()->size.width;
    dest.size.height = leGetRenderBuffer()->size.height;
    dest.mode = leGetRenderBuffer()->mode;
    dest.buffer_length = leGetRenderBuffer()->buffer_length;
    dest.pixels = (gfxBuffer)leGetRenderBuffer()->pixels;

    rowSize = srcRect->width * gfxColorInfoTable[src->mode].size;

    for(row = 0; row < srcRect->height; row++)
    {
        srcPtr = lePixelBufferOffsetGet(src, srcRect->x, srcRect->y + row);
        destPtr = lePixelBufferOffsetGet(&dest, destRect->x - frameRect.x, destRect->y - frameRect.y + row);

        memcpy(destPtr, srcPtr, rowSize);
    }
}

static leResult _draw(const leImage* img,
                      const leRect* srcRect,
                      int32_t x,
                      int32_t y,
                      uint32_t a)
{
    leRect imgRect, sourceClipRect, drawRect, clipRect;
    leRect dmgRect;

    leRenderer_GetClipRect(&dmgRect);

    // only allow a new setup if there isn't a current one
    if(_state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    memset(&_state, 0, sizeof(leRawDecodeState));

    imgRect.x = 0;
    imgRect.y = 0;
    imgRect.width = img->buffer.size.width;
    imgRect.height = img->buffer.size.height;

    /* make sure the source rect is within the source bounds */
    if(leRectIntersects(&imgRect, srcRect) == LE_FALSE)
        return LE_FAILURE;

    leRectClip(&imgRect, srcRect, &sourceClipRect);

    drawRect.x = x;
    drawRect.y = y;
    drawRect.width = sourceClipRect.width;
    drawRect.height = sourceClipRect.height;

    /* make sure the dest rect is within the damaged rect area */
    leRectClipAdj(&drawRect, &dmgRect, &sourceClipRect, &clipRect);

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    _state.manager.exec = _exec;
    _state.manager.isDone = _isDone;
    _state.manager.abort = _abort;
    _state.manager.cleanup = _cleanup;
#endif

    _state.source = img;
    _state.sourceRect = sourceClipRect;

    //_state.renderX = x;
    //_state.renderY = y;

    _state.destRect = clipRect;

    _state.globalAlpha = a;

    _state.targetMode = leRenderer_CurrentColorMode();

    if(img->header.location == LE_STREAM_LOCATION_ID_INTERNAL &&
       img->format == LE_IMAGE_FORMAT_RAW)
    {
        if((img->flags & LE_IMAGE_DIRECT_BLIT) > 0 &&
           img->buffer.mode == _state.targetMode)
        {
            _directBlit(&_state.source->buffer,
                        &_state.sourceRect,
                        &_state.destRect);

            // the op has already completed
            // failure indicates to the exe loop that there are no stages to run
            return LE_FAILURE;
        }

        if(leGPU_BlitBuffer(&_state.source->buffer,
                            &_state.sourceRect,
                            &_state.destRect,
                            a) == LE_SUCCESS)
        {
            // the op has already completed
            // failure indicates to the exe loop that there are no stages to run
            return LE_FAILURE;
        }
    }

    if(_leRawImageDecoder_SourceIterateSetupStage(&_state) == LE_FAILURE ||
       _initReadStage(&_state) == LE_FAILURE ||
       _leRawImageDecoder_PostReadStage(&_state) == LE_FAILURE ||
       _initMaskStage(&_state) == LE_FAILURE ||
       _initLookupStage(&_state) == LE_FAILURE ||
       _initBlendStage(&_state) == LE_FAILURE ||
       _initConvertStage(&_state) == LE_FAILURE ||
       _leRawImageDecoder_FrameBufferWriteStage(&_state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    _state.mode = LE_RAW_MODE_DRAW;

    return LE_SUCCESS;
}

static leResult _resize(const leImage* src,
                        const leRect* srcRect,
                        leImageFilterMode mode,
                        uint32_t sizeX,
                        uint32_t sizeY,
                        leImage* target)
{
    leRect imgRect, sourceClipRect, drawRect, targetRect, clipRect;

    // only allow a new setup if there isn't a current one
    if(_state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    if(mode == LE_IMAGEFILTER_NONE)
        return LE_FAILURE;

    // can't blend
    if(mode == LE_IMAGEFILTER_BILINEAR &&
       (LE_COLOR_MODE_IS_PIXEL(src->buffer.mode) == LE_FALSE ||
        LE_COLOR_MODE_IS_PIXEL(target->buffer.mode) == LE_FALSE))
    {
        return LE_FAILURE;
    }

    memset(&_state, 0, sizeof(leRawDecodeState));

    /* make sure the source rect is within the source bounds */
    imgRect.x = 0;
    imgRect.y = 0;
    imgRect.width = src->buffer.size.width;
    imgRect.height = src->buffer.size.height;

    if(leRectIntersects(&imgRect, srcRect) == LE_FALSE)
        return LE_FAILURE;

    // trim the source rectangle to fit
    leRectClip(&imgRect, srcRect, &sourceClipRect);

    drawRect.x = 0;
    drawRect.y = 0;
    drawRect.width = sizeX;
    drawRect.height = sizeY;

    targetRect.x = 0;
    targetRect.y = 0;
    targetRect.width = target->buffer.size.width;
    targetRect.height = target->buffer.size.height;

    /* make sure the dest rect is within the damaged rect area */
    leRectClipAdj(&drawRect, &targetRect, &sourceClipRect, &clipRect);

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    _state.manager.exec = _exec;
    _state.manager.isDone = _isDone;
    _state.manager.abort = _abort;
    _state.manager.cleanup = _cleanup;
#endif

    _state.mode = LE_RAW_MODE_RESIZE;
    _state.randomRLE = LE_TRUE;

    _state.source = src;
    _state.sourceRect = sourceClipRect;

    //_state.renderX = 0;
    //_state.renderY = 0;

    _state.filterMode = mode;
    _state.target = target;

    _state.destRect = clipRect;

    _state.targetMode = _state.target->buffer.mode;

    // iterator setup
    if(_leRawImageDecoder_TargetIterateSetupStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // filter pre read stage
    if(mode == LE_IMAGEFILTER_NEAREST_NEIGHBOR)
    {
        if(_leRawImageDecoder_ScaleNearestNeighborPreReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_ScaleBilinearPreReadStage(&_state) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    // read stage
    if(_initReadStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // post read stage
    if(mode == LE_IMAGEFILTER_BILINEAR)
    {
        if(_leRawImageDecoder_ScaleBilinearPostReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_PostReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // convert and write
    if(_initConvertStage(&_state) == LE_FAILURE ||
       _leRawImageDecoder_ImageWriteStage(&_state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    return LE_SUCCESS;
}

static leResult _resizeDraw(const leImage* src,
                            const leRect* srcRect,
                            leImageFilterMode mode,
                            uint32_t sizeX,
                            uint32_t sizeY,
                            int32_t x,
                            int32_t y,
                            uint32_t a)
{
    leRect imgRect, sourceClipRect, drawRect, drawClipRect, clipRect;
    leRect dmgRect;

    leRenderer_GetClipRect(&dmgRect);

    // only allow a new setup if there isn't a current one
    if(_state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    if(mode == LE_IMAGEFILTER_NONE)
        return LE_FAILURE;

    // can't blend
    if(mode == LE_IMAGEFILTER_BILINEAR &&
       LE_COLOR_MODE_IS_PIXEL(src->buffer.mode) == LE_FALSE)
    {
        return LE_FAILURE;
    }

    memset(&_state, 0, sizeof(leRawDecodeState));

    imgRect.x = 0;
    imgRect.y = 0;
    imgRect.width = src->buffer.size.width;
    imgRect.height = src->buffer.size.height;

    /* make sure the source rect is within the source bounds */
    if(leRectIntersects(&imgRect, srcRect) == LE_FALSE)
        return LE_FAILURE;

    leRectClip(&imgRect, srcRect, &sourceClipRect);

    drawRect.x = x;
    drawRect.y = y;
    drawRect.width = sizeX;
    drawRect.height = sizeY;

    drawClipRect.x = 0;
    drawClipRect.y = 0;
    drawClipRect.width = sizeX;
    drawClipRect.height = sizeY;

    /* make sure the dest rect is within the damaged rect area */
    leRectClipAdj(&drawRect, &dmgRect, &drawClipRect, &clipRect);

    if(drawClipRect.width <= 0 || drawClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    _state.manager.exec = _exec;
    _state.manager.isDone = _isDone;
    _state.manager.abort = _abort;
    _state.manager.cleanup = _cleanup;
#endif

    _state.mode = LE_RAW_MODE_RESIZEDRAW;
    _state.randomRLE = LE_TRUE;

    _state.source = src;
    _state.sourceRect = drawClipRect;

    _state.filterMode = mode;

    _state.destRect = clipRect;

    _state.sizeX = sizeX;
    _state.sizeY = sizeY;

    _state.targetMode = leRenderer_CurrentColorMode();

    _state.globalAlpha = a;


    if(src->header.location == LE_STREAM_LOCATION_ID_INTERNAL &&
       src->format == LE_IMAGE_FORMAT_RAW)
    {
        if(leGPU_BlitStretchBuffer(&_state.source->buffer,
                                   &_state.sourceRect,
                                   &_state.destRect,
                                   a) == LE_SUCCESS)
        {
            // the op has already completed
            // failure indicates to the exe loop that there are no stages to run
            return LE_FAILURE;
        }
    }

    // iterator setup
    if(_leRawImageDecoder_TargetIterateSetupStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // filter pre read stage
    if(mode == LE_IMAGEFILTER_NEAREST_NEIGHBOR)
    {
        if(_leRawImageDecoder_ScaleNearestNeighborPreReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_ScaleBilinearPreReadStage(&_state) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    // read stage
    if(_initReadStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // post read stage
    if(mode == LE_IMAGEFILTER_BILINEAR)
    {
        if(_leRawImageDecoder_ScaleBilinearPostReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_PostReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // convert and write
    if(_initBlendStage(&_state) == LE_FAILURE ||
	   _initConvertStage(&_state) == LE_FAILURE ||
       _leRawImageDecoder_FrameBufferWriteStage(&_state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    return LE_SUCCESS;
}

static leResult _copy(const leImage* src,
                      const leRect* srcRect,
                      int32_t x,
                      int32_t y,
                      leImage* dst)
{
    leRect imgRect, sourceClipRect, drawRect, targetRect, clipRect;

    // only allow a new setup if there isn't a current one
    if(_state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    // modes must be the same
    if(src->buffer.mode != dst->buffer.mode)
        return LE_FAILURE;

    // TODO add pixel buffer index write APIs to support this
    if(LE_COLOR_MODE_IS_INDEX(src->buffer.mode) == LE_TRUE)
        return LE_FAILURE;

    memset(&_state, 0, sizeof(leRawDecodeState));

    /* make sure the source rect is within the source bounds */
    imgRect.x = 0;
    imgRect.y = 0;
    imgRect.width = src->buffer.size.width;
    imgRect.height = src->buffer.size.height;

    if(leRectIntersects(&imgRect, srcRect) == LE_FALSE)
        return LE_FAILURE;

    // trim the source rectangle to fit
    leRectClip(&imgRect, srcRect, &sourceClipRect);

    drawRect.x = x;
    drawRect.y = y;
    drawRect.width = sourceClipRect.width;
    drawRect.height = sourceClipRect.height;

    targetRect.x = 0;
    targetRect.y = 0;
    targetRect.width = dst->buffer.size.width;
    targetRect.height = dst->buffer.size.height;

    /* make sure the dest rect is within the damaged rect area */
    leRectClipAdj(&drawRect, &targetRect, &sourceClipRect, &clipRect);

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    _state.manager.exec = _exec;
    _state.manager.isDone = _isDone;
    _state.manager.abort = _abort;
    _state.manager.cleanup = _cleanup;
#endif

    _state.mode = LE_RAW_MODE_COPY;

    _state.source = src;
    _state.sourceRect = sourceClipRect;

    _state.target = dst;

    _state.destRect = clipRect;

    _state.targetMode = _state.target->buffer.mode;

    // iterator setup
    if(_leRawImageDecoder_SourceIterateSetupStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // read stage
    if(_initReadStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    if(_leRawImageDecoder_PostReadStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // convert and write
    if(_leRawImageDecoder_ImageWriteStage(&_state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    return LE_SUCCESS;
}

static leResult _render(const leImage* src,
                        const leRect* srcRect,
                        int32_t x,
                        int32_t y,
                        leBool ignoreMask,
                        leBool ignoreAlpha,
                        leImage* dst)
{
    leRect imgRect, sourceClipRect, drawRect, targetRect, clipRect;

    // only allow a new setup if there isn't a current one
    if(_state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    if(LE_COLOR_MODE_IS_INDEX(dst->buffer.mode) == LE_TRUE)
        return LE_FAILURE;

    memset(&_state, 0, sizeof(leRawDecodeState));

    /* make sure the source rect is within the source bounds */
    imgRect.x = 0;
    imgRect.y = 0;
    imgRect.width = src->buffer.size.width;
    imgRect.height = src->buffer.size.height;

    if(leRectIntersects(&imgRect, srcRect) == LE_FALSE)
        return LE_FAILURE;

    // trim the source rectangle to fit
    leRectClip(&imgRect, srcRect, &sourceClipRect);

    drawRect.x = x;
    drawRect.y = y;
    drawRect.width = sourceClipRect.width;
    drawRect.height = sourceClipRect.height;

    targetRect.x = 0;
    targetRect.y = 0;
    targetRect.width = dst->buffer.size.width;
    targetRect.height = dst->buffer.size.height;

    /* make sure the dest rect is within the damaged rect area */
    leRectClipAdj(&drawRect, &targetRect, &sourceClipRect, &clipRect);

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    _state.manager.exec = _exec;
    _state.manager.isDone = _isDone;
    _state.manager.abort = _abort;
    _state.manager.cleanup = _cleanup;
#endif

    _state.mode = LE_RAW_MODE_RENDER;

    _state.source = src;
    _state.sourceRect = sourceClipRect;

    _state.target = dst;

    _state.destRect = clipRect;

    _state.targetMode = _state.target->buffer.mode;

    // iterator setup
    if(_leRawImageDecoder_SourceIterateSetupStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // read stage
    if(_initReadStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    if(_leRawImageDecoder_PostReadStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    if(ignoreMask == LE_TRUE)
    {
        if((src->flags & LE_IMAGE_USE_MASK_COLOR) > 0)
        {
            dst->flags |= LE_IMAGE_USE_MASK_COLOR;

            dst->mask.color = src->mask.color;
        }
        else
        {
            dst->flags &= ~(LE_IMAGE_USE_MASK_COLOR);
        }
    }
    else
    {
        if(_initMaskStage(&_state) == LE_FAILURE)
            return LE_FAILURE;

        dst->flags &= ~(LE_IMAGE_USE_MASK_COLOR);
    }

    if(_initLookupStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // source mask color is currently represented as an index into a palette that must be streamed
    // add a stage to store the resolved mask color once it has been looked up
    if(ignoreMask == LE_TRUE &&
       (dst->flags & LE_IMAGE_USE_MASK_COLOR) > 0 &&
       src->palette != NULL)
    {
#if LE_STREAMING_ENABLED == 1
        if(src->palette->header.location != LE_STREAM_LOCATION_ID_INTERNAL)
        {
            _leRawImageDecoder_ImageRenderPostLookupStage_Stream(&_state);
        }
        else
        {
#endif
            _leRawImageDecoder_ImageRenderPostLookupStage(&_state);
#if LE_STREAMING_ENABLED == 1
        }
#endif
    }

    if(ignoreAlpha == LE_FALSE && _initBlendStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // convert and write
    if(_initConvertStage(&_state) == LE_FAILURE ||
       _leRawImageDecoder_ImageWriteStage(&_state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    return LE_SUCCESS;
}

static leResult _rotate(const leImage* src,
                        const leRect* srcRect,
                        leImageFilterMode mode,
                        int32_t angle,
                        leImage** target,
                        leBool alloc)
{
    leRect imgRect, sourceClipRect, targetRect;

    // only allow a new setup if there isn't a current one
    if(_state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    if(mode == LE_IMAGEFILTER_NONE)
        return LE_FAILURE;

    // can't blend
    if(mode == LE_IMAGEFILTER_BILINEAR &&
      (LE_COLOR_MODE_IS_PIXEL(src->buffer.mode) == LE_FALSE))
    {
        return LE_FAILURE;
    }

    memset(&_state, 0, sizeof(leRawDecodeState));

    /* make sure the source rect is within the source bounds */
    imgRect.x = 0;
    imgRect.y = 0;
    imgRect.width = src->buffer.size.width;
    imgRect.height = src->buffer.size.height;

    if(leRectIntersects(&imgRect, srcRect) == LE_FALSE)
        return LE_FAILURE;

    // trim the source rectangle to fit
    leRectClip(&imgRect, srcRect, &sourceClipRect);

    if(target == NULL)
        return LE_FAILURE;

    targetRect.x = 0;
    targetRect.y = 0;
    targetRect.width = sourceClipRect.width;
    targetRect.height = sourceClipRect.height;

    leRotatedRectBounds(targetRect, angle, &targetRect);

    if(alloc == LE_TRUE)
    {
        *target = leImage_Allocate(targetRect.width, targetRect.height, src->buffer.mode);

        lePixelBufferAreaFill_Unsafe(&(*target)->buffer,
                                     0,
                                     0,
                                     (*target)->buffer.size.width,
                                     (*target)->buffer.size.height,
                                     0);
    }
    else
    {
        if(*target == NULL)
            return LE_FAILURE;

        /*if((*target)->buffer.size.width < sourceClipRect.width ||
           (*target)->buffer.size.height < sourceClipRect.height)
            return LE_FAILURE;*/
    }

    _state.target = *target;

    _state.destRect.x = 0;
    _state.destRect.y = 0;
    _state.destRect.width = _state.target->buffer.size.width;
    _state.destRect.height = _state.target->buffer.size.height;

    if(_state.target->buffer.pixels == NULL)
        return LE_FAILURE;

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    _state.manager.exec = _exec;
    _state.manager.isDone = _isDone;
    _state.manager.abort = _abort;
    _state.manager.cleanup = _cleanup;
#endif

    _state.mode = LE_RAW_MODE_ROTATE;
    _state.randomRLE = LE_TRUE;

    _state.source = src;
    _state.sourceRect = sourceClipRect;

    _state.angle = angle;
    _state.sourceOrigin.x = _state.sourceRect.x + _state.sourceRect.width / 2;
    _state.sourceOrigin.y = _state.sourceRect.y + _state.sourceRect.height / 2;
    _state.targetOrigin.x = _state.target->buffer.size.width / 2;
    _state.targetOrigin.y = _state.target->buffer.size.height / 2;

    _state.filterMode = mode;

    _state.targetMode = _state.target->buffer.mode;

    // iterator setup
    if(_leRawImageDecoder_TargetIterateSetupStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // filter pre read stage
    if(mode == LE_IMAGEFILTER_NEAREST_NEIGHBOR)
    {
        if(_leRawImageDecoder_RotateNearestNeighborPreReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_RotateBilinearPreReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // read stage
    if(_initReadStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // post read stage
    if(mode == LE_IMAGEFILTER_BILINEAR)
    {
        if(_leRawImageDecoder_RotateBilinearPostReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_PostReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // convert and write
    if(_initBlendStage(&_state) == LE_FAILURE ||
       _initConvertStage(&_state) == LE_FAILURE ||
       _leRawImageDecoder_ImageWriteStage(&_state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    return LE_SUCCESS;
}

static leResult _rotateDraw(const leImage* src,
                            const leRect* srcRect,
                            leImageFilterMode mode,
                            int32_t angle,
                            int32_t x,
                            int32_t y,
                            uint32_t a)
{
    leRect imgRect, sourceClipRect, drawRect, clipRect;
    leRect dmgRect;

    leRenderer_GetClipRect(&dmgRect);

    // only allow a new setup if there isn't a current one
    if(_state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    if(mode == LE_IMAGEFILTER_NONE)
        return LE_FAILURE;

    // can't blend
    if(mode == LE_IMAGEFILTER_BILINEAR &&
       LE_COLOR_MODE_IS_PIXEL(src->buffer.mode) == LE_FALSE)
    {
        return LE_FAILURE;
    }

    memset(&_state, 0, sizeof(leRawDecodeState));

    imgRect.x = 0;
    imgRect.y = 0;
    imgRect.width = src->buffer.size.width;
    imgRect.height = src->buffer.size.height;

    /* make sure the source rect is within the source bounds */
    if(leRectIntersects(&imgRect, srcRect) == LE_FALSE)
        return LE_FAILURE;

    leRectClip(&imgRect, srcRect, &sourceClipRect);

    // calculate source rect in screen space
    imgRect = sourceClipRect;
    imgRect.x += x;
    imgRect.y += y;

    // calculate bounds of rotated rectangle
    leRotatedRectBounds(imgRect,
                        angle,
                        &drawRect);

    // center rotated rectangle over original rectangle
    drawRect.x = imgRect.x + imgRect.width / 2;
    drawRect.y = imgRect.y + imgRect.height / 2;

    drawRect.x -= drawRect.width / 2;
    drawRect.y -= drawRect.height / 2;

    /* make sure the dest rect is within the damaged rect area */
    leRectClip(&drawRect, &dmgRect, &clipRect);

    if(clipRect.width <= 0 || clipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    _state.manager.exec = _exec;
    _state.manager.isDone = _isDone;
    _state.manager.abort = _abort;
    _state.manager.cleanup = _cleanup;
#endif

    _state.mode = LE_RAW_MODE_ROTATEDRAW;

    _state.source = src;
    _state.filterMode = mode;
    _state.sourceRect = imgRect;

    // store the target rect dimensions
    _state.destRect = clipRect;

    _state.targetY = 0;
    _state.targetX = 0;

    _state.angle = angle;
    //_state.sourceOrigin = srcRect->width

    _state.targetMode = leRenderer_CurrentColorMode();

    _state.globalAlpha = a;

    _state.randomRLE = LE_TRUE;

    // iterator setup
    if(_leRawImageDecoder_TargetIterateSetupStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // filter pre read stage
    if(mode == LE_IMAGEFILTER_NEAREST_NEIGHBOR)
    {
        if(_leRawImageDecoder_RotateNearestNeighborPreReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_RotateBilinearPreReadStage(&_state) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    // read stage
    if(_initReadStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // post read stage
    if(mode == LE_IMAGEFILTER_BILINEAR)
    {
        if(_leRawImageDecoder_RotateBilinearPostReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_PostReadStage(&_state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // convert and write
    if(_initMaskStage(&_state) == LE_FAILURE ||
		_initBlendStage(&_state) == LE_FAILURE ||
		_initConvertStage(&_state) == LE_FAILURE ||
       _leRawImageDecoder_FrameBufferWriteStage(&_state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    return LE_SUCCESS;
}

#if LE_STREAMING_ENABLED == 1
static void _cleanup(leStreamManager* mgr)
#else
static void _decoderCleanup(void)
#endif
{
#if LE_STREAMING_ENABLED == 1
    (void)mgr; // unused
#endif
    int32_t idx;

    if(_state.mode != LE_RAW_MODE_NONE)
    {
        _state.mode = LE_RAW_MODE_NONE;

        for(idx = 0; idx < LE_IMAGEDECODER_RAW_MAX_STAGES; idx++)
        {
            if(_state.stages[idx] != NULL)
            {
                if(_state.stages[idx]->cleanup != NULL)
                {
                    _state.stages[idx]->cleanup(_state.stages[idx]);
                }
            }
            else
            {
                break;
            }
        }

#if LE_STREAMING_ENABLED == 1
        if(leGetState()->activeStream == (leStreamManager*)&_state)
        {
            leGetState()->activeStream = NULL;
        }
#endif
    }
}

#if LE_STREAMING_ENABLED == 1
static leResult _exec(leStreamManager* mgr)
#else
static leResult _decoderExec(void)
#endif
{
    if(_state.mode == LE_RAW_MODE_NONE)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL &&
       leGetActiveStream() != (leStreamManager*)&_state)
        return LE_FAILURE;

    leGetState()->activeStream = (leStreamManager*)&_state;
#endif

    // any stage can set done and exit the loop
    while(_state.done == LE_FALSE)
    {
        // stages can return false if they stall
        if(_state.stages[_state.currentStage]->exec(_state.stages[_state.currentStage]) == LE_FAILURE)
            return LE_SUCCESS;

        // increment stage
        _state.currentStage++;

        if(_state.stages[_state.currentStage] == NULL)
        {
            // restart the pipeline
            _state.currentStage = 0;
        }
    }

#if LE_STREAMING_ENABLED == 1
    // free the stages
    mgr->cleanup(mgr);

    if(mgr->onDone != NULL)
    {
        mgr->onDone(mgr);
    }
#else
    _decoderCleanup();
#endif

    return LE_SUCCESS;
}

#if LE_STREAMING_ENABLED == 1
static leBool _isDone(leStreamManager* mgr)
#else
static leBool _decoderIsDone(void)
#endif
{
#if LE_STREAMING_ENABLED == 1
    (void)mgr; // unused
#endif

    return _state.done;
}

#if LE_STREAMING_ENABLED == 1
static void _abort(leStreamManager* mgr)
{
    _state.done = LE_TRUE;

    // free the stages
    mgr->cleanup(mgr);

    if(mgr->onDone != NULL)
    {
        mgr->onDone(mgr);
    }
}
#endif


#if LE_STREAMING_ENABLED == 1

static leResult _decoderExec(void)
{
    return _state.manager.exec(&_state.manager);
}

static leBool _decoderIsDone(void)
{
    return _state.done;
}

static void _decoderCleanup(void)
{
    _state.manager.cleanup(&_state.manager);
}

#endif

leImageDecoder* _leRawImageDecoder_Init(void)
{
    memset(&_state, 0, sizeof(leImageDecoder));

    _decoder.supportsImage = _supportsImage;
    _decoder.draw = _draw;
    _decoder.copy = _copy;
    _decoder.render = _render;
    _decoder.resize = _resize;
    _decoder.resizeDraw = _resizeDraw;
    _decoder.rotate = _rotate;
    _decoder.rotateDraw = _rotateDraw;
    _decoder.exec = _decoderExec;
    _decoder.isDone = _decoderIsDone;
    _decoder.free = _decoderCleanup;

    return &_decoder;
}

void _leRawImageDecoder_InjectStage(leRawDecodeState* state,
                                    leRawDecodeStage* stage)
{
    uint32_t idx = 0;

    while(state->stages[idx] != NULL)
    {
        LE_ASSERT(idx < LE_IMAGEDECODER_RAW_MAX_STAGES);

        idx++;
    }

    state->stages[idx] = stage;
}

#endif /* LE_ENABLE_RAW_DECODER */
