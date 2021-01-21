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
#include "gfx/legato/image/raw/legato_imagedecoder_raw.h"

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/core/legato_stream.h"
#include "gfx/legato/image/legato_palette.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/renderer/legato_gpu.h"

#if LE_STREAMING_ENABLED == 1

#if LE_ASSET_DECODER_USE_PIXEL_CACHE == 1
uint8_t leRawImageDecoderScratchBuffer[LE_ASSET_DECODER_CACHE_SIZE];
#endif

#if LE_ASSET_DECODER_USE_MASK_CACHE == 1
uint8_t leRawImageDecoderPaletteScratchBuffer[LE_ASSET_DECODER_CACHE_SIZE];
#endif

#if LE_ASSET_DECODER_USE_PALETTE_CACHE == 1
uint8_t leRawImageDecoderPaletteScratchBuffer[LE_ASSET_DECODER_CACHE_SIZE];
#endif

#if LE_ASSET_DECODER_USE_BLEND_CACHE == 1
uint8_t leRawImageDecoderBlendBuffer[LE_ASSET_DECODER_CACHE_SIZE];
#endif

#endif

static leImageDecoder decoder;
static leRawDecodeState state;

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

static leResult _initReadStage(leRawDecodeState* state)
{
    // initialize read stage
#if LE_STREAMING_ENABLED == 1
    if(state->source->header.location == LE_STREAM_LOCATION_ID_INTERNAL)
    {
#endif
        if(_leRawImageDecoder_ReadStage_Internal(state) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
#if LE_STREAMING_ENABLED == 1
    }
    else
    {
        if(state->source->format == LE_IMAGE_FORMAT_RAW)
        {
            if(state->source->palette != NULL)
            {
                _leRawImageDecoder_ReadStage_StreamIndex(state);
            }
            else
            {
                _leRawImageDecoder_ReadStage_StreamColor(state);
            }
        }
        else
        {
            if(state->source->palette != NULL)
            {
                _leRawImageDecoder_ReadStage_StreamRLEIndex(state);
            }
            else
            {
                _leRawImageDecoder_ReadStage_StreamRLE(state);
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

static leResult _draw(const leImage* img,
                      const leRect* srcRect,
                      int32_t x,
                      int32_t y,
                      uint32_t a)
{
    leRect imgRect, sourceClipRect, drawRect, clipRect;
    leRect dmgRect = leRenderer_GetDrawRect();

    // only allow a new setup if there isn't a current one
    if(state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    memset(&state, 0, sizeof(leRawDecodeState));

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
    clipRect = leRectClipAdj(&drawRect, &dmgRect, &sourceClipRect);

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    state.manager.exec = _exec;
    state.manager.isDone = _isDone;
    state.manager.abort = _abort;
    state.manager.cleanup = _cleanup;
#endif

    state.mode = LE_RAW_MODE_DRAW;

    state.source = img;
    state.sourceRect = sourceClipRect;

    //state.renderX = x;
    //state.renderY = y;

    state.destRect = clipRect;

    state.globalAlpha = a;

    state.targetMode = leRenderer_CurrentColorMode();

    if(img->header.location == LE_STREAM_LOCATION_ID_INTERNAL &&
       img->format == LE_IMAGE_FORMAT_RAW)
    {
        if(leGPU_BlitBuffer(&state.source->buffer,
                            &state.sourceRect,
                            &state.destRect,
                            a) == LE_SUCCESS)
        {
            return LE_SUCCESS;
        }
    }

    if(_leRawImageDecoder_SourceIterateSetupStage(&state) == LE_FAILURE ||
       _initReadStage(&state) == LE_FAILURE ||
       _leRawImageDecoder_PostReadStage(&state) == LE_FAILURE ||
       _initMaskStage(&state) == LE_FAILURE ||
       _initLookupStage(&state) == LE_FAILURE ||
       _initBlendStage(&state) == LE_FAILURE ||
       _initConvertStage(&state) == LE_FAILURE ||
       _leRawImageDecoder_FrameBufferWriteStage(&state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

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
    if(state.mode != LE_RAW_MODE_NONE)
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

    memset(&state, 0, sizeof(leRawDecodeState));

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
    clipRect = leRectClipAdj(&drawRect, &targetRect, &sourceClipRect);

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    state.manager.exec = _exec;
    state.manager.isDone = _isDone;
    state.manager.abort = _abort;
    state.manager.cleanup = _cleanup;
#endif

    state.mode = LE_RAW_MODE_RESIZE;

    state.source = src;
    state.sourceRect = sourceClipRect;

    //state.renderX = 0;
    //state.renderY = 0;

    state.filterMode = mode;
    state.target = target;

    state.destRect = clipRect;

    state.targetMode = state.target->buffer.mode;

    // iterator setup
    if(_leRawImageDecoder_TargetIterateSetupStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // filter pre read stage
    if(mode == LE_IMAGEFILTER_NEAREST_NEIGHBOR)
    {
        if(_leRawImageDecoder_ScaleNearestNeighborPreReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_ScaleBilinearPreReadStage(&state) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    // read stage
    if(_initReadStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // post read stage
    if(mode == LE_IMAGEFILTER_BILINEAR)
    {
        if(_leRawImageDecoder_ScaleBilinearPostReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_PostReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // convert and write
    if(_initConvertStage(&state) == LE_FAILURE ||
       _leRawImageDecoder_ImageWriteStage(&state) == LE_FAILURE)
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
    leRect dmgRect = leRenderer_GetDrawRect();

    // only allow a new setup if there isn't a current one
    if(state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    if(mode == LE_IMAGEFILTER_NONE)
        return LE_FAILURE;

    // can't blend
    if(mode == LE_IMAGEFILTER_BILINEAR &&
       LE_COLOR_MODE_IS_PIXEL(src->buffer.mode) == LE_FALSE)
    {
        return LE_FAILURE;
    }

    memset(&state, 0, sizeof(leRawDecodeState));

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
    clipRect = leRectClipAdj(&drawRect, &dmgRect, &drawClipRect);

    if(drawClipRect.width <= 0 || drawClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    state.manager.exec = _exec;
    state.manager.isDone = _isDone;
    state.manager.abort = _abort;
    state.manager.cleanup = _cleanup;
#endif

    state.mode = LE_RAW_MODE_RESIZEDRAW;

    state.source = src;
    state.sourceRect = drawClipRect;

    state.filterMode = mode;

    state.destRect = clipRect;

    state.sizeX = sizeX;
    state.sizeY = sizeY;

    state.targetMode = leRenderer_CurrentColorMode();

    state.globalAlpha = a;


    if(src->header.location == LE_STREAM_LOCATION_ID_INTERNAL &&
       src->format == LE_IMAGE_FORMAT_RAW)
    {
        if(leGPU_BlitStretchBuffer(&state.source->buffer,
                                   &state.sourceRect,
                                   &state.destRect,
                                   a) == LE_SUCCESS)
        {
            return LE_SUCCESS;
        }
    }

    // iterator setup
    if(_leRawImageDecoder_TargetIterateSetupStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // filter pre read stage
    if(mode == LE_IMAGEFILTER_NEAREST_NEIGHBOR)
    {
        if(_leRawImageDecoder_ScaleNearestNeighborPreReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_ScaleBilinearPreReadStage(&state) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    // read stage
    if(_initReadStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // post read stage
    if(mode == LE_IMAGEFILTER_BILINEAR)
    {
        if(_leRawImageDecoder_ScaleBilinearPostReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_PostReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // convert and write
    if(_initBlendStage(&state) == LE_FAILURE ||
	   _initConvertStage(&state) == LE_FAILURE ||
       _leRawImageDecoder_FrameBufferWriteStage(&state) == LE_FAILURE)
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
    if(state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    // modes must be the same
    if(src->buffer.mode != dst->buffer.mode)
        return LE_FAILURE;

    // TODO add pixel buffer index write APIs to support this
    if(LE_COLOR_MODE_IS_INDEX(src->buffer.mode) == LE_TRUE)
        return LE_FAILURE;

    memset(&state, 0, sizeof(leRawDecodeState));

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
    clipRect = leRectClipAdj(&drawRect, &targetRect, &sourceClipRect);

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    state.manager.exec = _exec;
    state.manager.isDone = _isDone;
    state.manager.abort = _abort;
    state.manager.cleanup = _cleanup;
#endif

    state.mode = LE_RAW_MODE_COPY;

    state.source = src;
    state.sourceRect = sourceClipRect;

    state.target = dst;

    state.destRect = clipRect;

    state.targetMode = state.target->buffer.mode;

    // iterator setup
    if(_leRawImageDecoder_SourceIterateSetupStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // read stage
    if(_initReadStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    if(_leRawImageDecoder_PostReadStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // convert and write
    if(_leRawImageDecoder_ImageWriteStage(&state) == LE_FAILURE)
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
    if(state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    if(LE_COLOR_MODE_IS_INDEX(dst->buffer.mode) == LE_TRUE)
        return LE_FAILURE;

    memset(&state, 0, sizeof(leRawDecodeState));

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
    clipRect = leRectClipAdj(&drawRect, &targetRect, &sourceClipRect);

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    state.manager.exec = _exec;
    state.manager.isDone = _isDone;
    state.manager.abort = _abort;
    state.manager.cleanup = _cleanup;
#endif

    state.mode = LE_RAW_MODE_RENDER;

    state.source = src;
    state.sourceRect = sourceClipRect;

    state.target = dst;

    state.destRect = clipRect;

    state.targetMode = state.target->buffer.mode;

    // iterator setup
    if(_leRawImageDecoder_SourceIterateSetupStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // read stage
    if(_initReadStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    if(_leRawImageDecoder_PostReadStage(&state) == LE_FAILURE)
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
        if(_initMaskStage(&state) == LE_FAILURE)
            return LE_FAILURE;

        dst->flags &= ~(LE_IMAGE_USE_MASK_COLOR);
    }

    if(_initLookupStage(&state) == LE_FAILURE)
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
            _leRawImageDecoder_ImageRenderPostLookupStage_Stream(&state);
        }
        else
        {
#endif
            _leRawImageDecoder_ImageRenderPostLookupStage(&state);
#if LE_STREAMING_ENABLED == 1
        }
#endif
    }

    if(ignoreAlpha == LE_TRUE && _initBlendStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // convert and write
    if(_initConvertStage(&state) == LE_FAILURE ||
       _leRawImageDecoder_ImageWriteStage(&state) == LE_FAILURE)
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
    if(state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    if(mode == LE_IMAGEFILTER_NONE)
        return LE_FAILURE;

    // can't blend
    if(mode == LE_IMAGEFILTER_BILINEAR &&
      (LE_COLOR_MODE_IS_PIXEL(src->buffer.mode) == LE_FALSE))
    {
        return LE_FAILURE;
    }

    memset(&state, 0, sizeof(leRawDecodeState));

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

    targetRect = leRotatedRectBounds(targetRect, angle);

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

    state.target = *target;

    state.destRect.x = 0;
    state.destRect.y = 0;
    state.destRect.width = state.target->buffer.size.width;
    state.destRect.height = state.target->buffer.size.height;

    if(state.target->buffer.pixels == NULL)
        return LE_FAILURE;

    if(sourceClipRect.width <= 0 || sourceClipRect.height <= 0)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    state.manager.exec = _exec;
    state.manager.isDone = _isDone;
    state.manager.abort = _abort;
    state.manager.cleanup = _cleanup;
#endif

    state.mode = LE_RAW_MODE_ROTATE;

    state.source = src;
    state.sourceRect = sourceClipRect;

    state.angle = angle;
    state.sourceOrigin.x = state.sourceRect.x + state.sourceRect.width / 2;
    state.sourceOrigin.y = state.sourceRect.y + state.sourceRect.height / 2;
    state.targetOrigin.x = state.target->buffer.size.width / 2;
    state.targetOrigin.y = state.target->buffer.size.height / 2;

    state.filterMode = mode;

    state.targetMode = state.target->buffer.mode;

    // iterator setup
    if(_leRawImageDecoder_TargetIterateSetupStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // filter pre read stage
    if(mode == LE_IMAGEFILTER_NEAREST_NEIGHBOR)
    {
        if(_leRawImageDecoder_RotateNearestNeighborPreReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_RotateBilinearPreReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // read stage
    if(_initReadStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // post read stage
    if(mode == LE_IMAGEFILTER_BILINEAR)
    {
        if(_leRawImageDecoder_RotateBilinearPostReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_PostReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // convert and write
    if(_initBlendStage(&state) == LE_FAILURE ||
       _initConvertStage(&state) == LE_FAILURE ||
       _leRawImageDecoder_ImageWriteStage(&state) == LE_FAILURE)
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
    leRect dmgRect = leRenderer_GetDrawRect();

    // only allow a new setup if there isn't a current one
    if(state.mode != LE_RAW_MODE_NONE)
        return LE_FAILURE;

    if(mode == LE_IMAGEFILTER_NONE)
        return LE_FAILURE;

    // can't blend
    if(mode == LE_IMAGEFILTER_BILINEAR &&
       LE_COLOR_MODE_IS_PIXEL(src->buffer.mode) == LE_FALSE)
    {
        return LE_FAILURE;
    }

    memset(&state, 0, sizeof(leRawDecodeState));

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
    drawRect = leRotatedRectBounds(imgRect,
                                   angle);

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
    state.manager.exec = _exec;
    state.manager.isDone = _isDone;
    state.manager.abort = _abort;
    state.manager.cleanup = _cleanup;
#endif

    state.mode = LE_RAW_MODE_ROTATEDRAW;

    state.source = src;
    state.filterMode = mode;
    state.sourceRect = imgRect;

    // store the target rect dimensions
    state.destRect = clipRect;

    state.targetY = 0;
    state.targetX = 0;

    state.angle = angle;
    //state.sourceOrigin = srcRect->width

    state.targetMode = leRenderer_CurrentColorMode();

    state.globalAlpha = a;

    state.randomRLE = LE_TRUE;

    // iterator setup
    if(_leRawImageDecoder_TargetIterateSetupStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // filter pre read stage
    if(mode == LE_IMAGEFILTER_NEAREST_NEIGHBOR)
    {
        if(_leRawImageDecoder_RotateNearestNeighborPreReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_RotateBilinearPreReadStage(&state) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    // read stage
    if(_initReadStage(&state) == LE_FAILURE)
        return LE_FAILURE;

    // post read stage
    if(mode == LE_IMAGEFILTER_BILINEAR)
    {
        if(_leRawImageDecoder_RotateBilinearPostReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }
    else
    {
        if(_leRawImageDecoder_PostReadStage(&state) == LE_FAILURE)
            return LE_FAILURE;
    }

    // convert and write
    if(_initMaskStage(&state) == LE_FAILURE ||
		_initBlendStage(&state) == LE_FAILURE ||
		_initConvertStage(&state) == LE_FAILURE ||
       _leRawImageDecoder_FrameBufferWriteStage(&state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    return LE_SUCCESS;
}

#if LE_STREAMING_ENABLED == 1
static void _cleanup(leStreamManager* mgr)
#else
static void _decoderCleanup()
#endif
{
#if LE_STREAMING_ENABLED == 1
    (void)mgr; // unused
#endif
    int32_t idx;

    if(state.mode != LE_RAW_MODE_NONE)
    {
        state.mode = LE_RAW_MODE_NONE;

        for(idx = 0; idx < LE_IMAGEDECODER_RAW_MAX_STAGES; idx++)
        {
            if(state.stages[idx] != NULL)
            {
                if(state.stages[idx]->cleanup != NULL)
                {
                    state.stages[idx]->cleanup(state.stages[idx]);
                }
            }
            else
            {
                break;
            }
        }

#if LE_STREAMING_ENABLED == 1
        if(leGetState()->activeStream == (leStreamManager*)&state)
        {
            leGetState()->activeStream = NULL;
        }
#endif
    }
}

#if LE_STREAMING_ENABLED == 1
static leResult _exec(leStreamManager* mgr)
#else
static leResult _decoderExec()
#endif
{
    if(state.mode == LE_RAW_MODE_NONE)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL &&
       leGetActiveStream() != (leStreamManager*)&state)
        return LE_FAILURE;

    leGetState()->activeStream = (leStreamManager*)&state;
#endif

    // any stage can set done and exit the loop
    while(state.done == LE_FALSE)
    {
        // stages can return false if they stall
        if(state.stages[state.currentStage]->exec(state.stages[state.currentStage]) == LE_FAILURE)
            return LE_SUCCESS;

        // increment stage
        state.currentStage++;

        if(state.stages[state.currentStage] == NULL)
        {
            // restart the pipeline
            state.currentStage = 0;
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
static leBool _decoderIsDone()
#endif
{
#if LE_STREAMING_ENABLED == 1
    (void)mgr; // unused
#endif

    return state.done;
}

#if LE_STREAMING_ENABLED == 1
static void _abort(leStreamManager* mgr)
{
    state.done = LE_TRUE;

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
    return state.manager.exec(&state.manager);
}

static leBool _decoderIsDone(void)
{
    return state.done;
}

static void _decoderCleanup(void)
{
    state.manager.cleanup(&state.manager);
}

#endif

leImageDecoder* _leRawImageDecoder_Init(void)
{
    memset(&state, 0, sizeof(leImageDecoder));

    decoder.supportsImage = _supportsImage;
    decoder.draw = _draw;
    decoder.copy = _copy;
    decoder.render = _render;
    decoder.resize = _resize;
    decoder.resizeDraw = _resizeDraw;
    decoder.rotate = _rotate;
    decoder.rotateDraw = _rotateDraw;
    decoder.exec = _decoderExec;
    decoder.isDone = _decoderIsDone;
    decoder.free = _decoderCleanup;

    return &decoder;
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
