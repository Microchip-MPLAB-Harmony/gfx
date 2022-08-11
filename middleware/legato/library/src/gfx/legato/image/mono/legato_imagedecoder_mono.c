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

#if LE_ENABLE_MONO_DECODER == 1

#include "gfx/legato/image/mono/legato_imagedecoder_mono.h"

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/core/legato_stream.h"
#include "gfx/legato/image/legato_palette.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/renderer/legato_gpu.h"

#if LE_STREAMING_ENABLED == 1

uint8_t LE_COHERENT_ATTR leMonoImageDecoderScratchBuffer[LE_ASSET_DECODER_PIXEL_CACHE_SIZE];

#endif

static LE_COHERENT_ATTR leImageDecoder _decoder;
static LE_COHERENT_ATTR leMonoDecodeState _state;

#if LE_STREAMING_ENABLED == 1

static leResult _exec(leStreamManager* mgr);
static leBool _isDone(leStreamManager* mgr);
static void _abort(leStreamManager* mgr);
static void _cleanup(leStreamManager* mgr);

#endif

leResult _leMonoImageDecoder_SourceIterateSetupStage(leMonoDecodeState* state);
leResult _leMonoImageDecoder_ReadStage_Internal(leMonoDecodeState* state);
#if LE_STREAMING_ENABLED == 1
leResult _leMonoImageDecoder_ReadStage_Stream(leMonoDecodeState* state);
#endif

leResult _leMonoImageDecoder_FrameBufferWriteStage(leMonoDecodeState* state);
leResult _leMonoImageDecoder_ImageWriteStage(leMonoDecodeState* state);

static leBool _supportsImage(const leImage* img)
{
    return img->format == LE_IMAGE_FORMAT_MONO;
}

static leResult _initReadStage(leMonoDecodeState* st)
{
    // initialize read stage
#if LE_STREAMING_ENABLED == 1
    if(st->source->header.location == LE_STREAM_LOCATION_ID_INTERNAL)
    {
#endif
        if(_leMonoImageDecoder_ReadStage_Internal(st) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
#if LE_STREAMING_ENABLED == 1
    }
    else
    {
        _leMonoImageDecoder_ReadStage_Stream(st);
    }
#endif

    return LE_SUCCESS;
}

static leResult _draw(const leImage* img,
                      const leRect* srcRect,
                      int32_t x,
                      int32_t y,
                      uint32_t a)
{
    leRect imgRect, sourceClipRect, drawRect, clipRect;
    leRect dmgRect;
    (void)a; // unused

    leRenderer_GetClipRect(&dmgRect);

    // only allow a new setup if there isn't a current one
    if(_state.mode != LE_MONO_MODE_NONE)
        return LE_FAILURE;

    memset(&_state, 0, sizeof(leMonoDecodeState));

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

    _state.destRect = clipRect;

    if(_leMonoImageDecoder_SourceIterateSetupStage(&_state) == LE_FAILURE ||
       _initReadStage(&_state) == LE_FAILURE ||
       _leMonoImageDecoder_FrameBufferWriteStage(&_state) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    _state.mode = LE_MONO_MODE_DRAW;

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
    if(_state.mode != LE_MONO_MODE_NONE)
        return LE_FAILURE;

    // modes must be the same
    if(src->buffer.mode != dst->buffer.mode)
        return LE_FAILURE;

    memset(&_state, 0, sizeof(leMonoDecodeState));

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

    _state.mode = LE_MONO_MODE_COPY;

    _state.source = src;
    _state.sourceRect = sourceClipRect;

    _state.target = dst;

    _state.destRect = clipRect;

    // iterator setup
    if(_leMonoImageDecoder_SourceIterateSetupStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // read stage
    if(_initReadStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

    // convert and write
    if(_leMonoImageDecoder_ImageWriteStage(&_state) == LE_FAILURE)
        return LE_FAILURE;

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

    if(_state.mode != LE_MONO_MODE_NONE)
    {
        _state.mode = LE_MONO_MODE_NONE;

        for(idx = 0; idx < LE_IMAGEDECODER_MONO_MAX_STAGES; idx++)
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
    if(_state.mode == LE_MONO_MODE_NONE)
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

leImageDecoder* _leMonoImageDecoder_Init(void)
{
    memset(&_state, 0, sizeof(leImageDecoder));

    _decoder.supportsImage = _supportsImage;
    _decoder.draw = _draw;
    _decoder.copy = _copy;
    _decoder.render = NULL;
    _decoder.resize = NULL;
    _decoder.resizeDraw = NULL;
    _decoder.rotate = NULL;
    _decoder.rotateDraw = NULL;
    _decoder.exec = _decoderExec;
    _decoder.isDone = _decoderIsDone;
    _decoder.free = _decoderCleanup;

    return &_decoder;
}

void _leMonoImageDecoder_InjectStage(leMonoDecodeState* state,
                                     leMonoDecodeStage* stage)
{
    uint32_t idx = 0;

    while(state->stages[idx] != NULL)
    {
        LE_ASSERT(idx < LE_IMAGEDECODER_MONO_MAX_STAGES);

        idx++;
    }

    state->stages[idx] = stage;
}

static struct SetupStage
{
    leMonoDecodeStage base;
} setupStage;

static leResult stage_sourceIterateSetup(leMonoDecodeStage* stage)
{
    leMonoDecodeState* state = stage->state;

    if(state->rowIterator >= (uint32_t)state->sourceRect.height)
    {
        state->currentStage = 0;
        state->done = LE_TRUE;

        return LE_SUCCESS;
    }

    state->referenceX = state->sourceRect.x + state->colIterator;
    state->referenceY = state->sourceRect.y + state->rowIterator;

    // calculate target coordinates
    state->targetX = state->destRect.x + state->colIterator;
    state->targetY = state->destRect.y + state->rowIterator;

    state->readCount = 1;

    state->readOperation[0].x = state->sourceRect.x + state->colIterator;
    state->readOperation[0].y = state->sourceRect.y + state->rowIterator;

    // calculate buffer index
    state->readOperation[0].bufferIndex = state->readOperation[0].x +
    (state->readOperation[0].y *
    stage->state->source->buffer.size.width);

    if(state->colIterator < (uint32_t)state->sourceRect.width - 1)
    {
        state->colIterator += 1;
    }
    else
    {
        state->colIterator = 0;
        state->rowIterator += 1;
    }

    return LE_SUCCESS;
}

leResult _leMonoImageDecoder_SourceIterateSetupStage(leMonoDecodeState* state)
{
    memset(&setupStage, 0, sizeof(setupStage));

    state->rowIterator = 0;
    state->colIterator = 0;

    setupStage.base.state = state;
    setupStage.base.exec = stage_sourceIterateSetup;

    _leMonoImageDecoder_InjectStage(state, (void*)&setupStage);

    return LE_SUCCESS;
}

static struct FrameBufferWriteStage
{
    leMonoDecodeStage base;
} frameBufferWriteStage;

//#include <stdio.h>

#if LE_ALPHA_BLENDING_ENABLED == 1
static leResult stage_FrameBufferWrite(leMonoDecodeStage* stage)
{
    // write color
    leRenderer_PutPixel(stage->state->targetX,
                        stage->state->targetY,
                        stage->state->writeColor);

    /*printf("%i, %i, %u\n", stage->state->targetX,
                           stage->state->targetY,
                           stage->state->writeColor);*/

    return LE_SUCCESS;
}
#else
static leResult stage_FrameBufferWrite(leMonoDecodeStage* stage)
{
    // write color
    leRenderer_PutPixel(stage->state->targetX,
                        stage->state->targetY,
                        stage->state->writeColor);

    return LE_SUCCESS;
}
#endif

leResult _leMonoImageDecoder_FrameBufferWriteStage(leMonoDecodeState* state)
{
    memset(&frameBufferWriteStage, 0, sizeof(frameBufferWriteStage));

    frameBufferWriteStage.base.state = state;
    frameBufferWriteStage.base.exec = stage_FrameBufferWrite;

    _leMonoImageDecoder_InjectStage(state, (void*) &frameBufferWriteStage);

    return LE_SUCCESS;
}

static struct ImageWriteStage
{
    leMonoDecodeStage base;
} imageWriteStage;

static leResult stage_ImageWrite(leMonoDecodeStage* stage)
{
    // write color
    lePixelBufferSet_Unsafe(&stage->state->target->buffer,
                            stage->state->targetX,
                            stage->state->targetY,
                            stage->state->writeColor);

    return LE_SUCCESS;
}

leResult _leMonoImageDecoder_ImageWriteStage(leMonoDecodeState* state)
{
    memset(&imageWriteStage, 0, sizeof(imageWriteStage));

    imageWriteStage.base.state = state;
    imageWriteStage.base.exec = stage_ImageWrite;

    _leMonoImageDecoder_InjectStage(state, (void*)&imageWriteStage);

    return LE_SUCCESS;
}

#endif /* LE_ENABLE_MONO_DECODER */
