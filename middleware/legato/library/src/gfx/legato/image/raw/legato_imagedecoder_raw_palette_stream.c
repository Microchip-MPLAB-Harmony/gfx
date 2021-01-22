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

#include "gfx/legato/common/legato_color.h"
#include "gfx/legato/common/legato_pixelbuffer.h"
#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/image/legato_image_utils.h"
#include "gfx/legato/image/legato_palette.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"


#if LE_STREAMING_ENABLED == 1

void _leRawImageDecoder_InjectStage(leRawDecodeState* state,
                                    leRawDecodeStage* stage);

enum StageState
{
    SS_READY,
    SS_STALLED,
    SS_DONE
};

#define cache leRawImageDecoderPaletteScratchBuffer

struct StreamPaletteStage
{
    leRawDecodeStage base;

    uint32_t paletteSize;

    leColor lookupIndex;

    enum StageState state;

    leStream stream;

    lePixelBuffer cacheBuffer;
};

static LE_COHERENT_ATTR struct StreamPaletteStage streamPaletteStage;

static void advanceStage(void)
{
    streamPaletteStage.state = SS_DONE;
}

static void indexDataReady(leStream* strm)
{
    (void)strm; // unused

    streamPaletteStage.state = SS_READY;

    advanceStage();
}

static leResult exec_nonblocking(struct StreamPaletteStage* stage)
{
    uint32_t addr;

    // no need to do anything if a data read is still pending
    if(streamPaletteStage.state == SS_STALLED)
        return LE_FAILURE;

    if(streamPaletteStage.state == SS_DONE)
    {
        streamPaletteStage.state = SS_READY;

        return LE_SUCCESS;
    }

    streamPaletteStage.lookupIndex = streamPaletteStage.base.state->writeColor;

    addr = (uint32_t)streamPaletteStage.base.state->source->palette->header.address +
            streamPaletteStage.lookupIndex *
            stage->paletteSize;

    if(leStream_Read(&streamPaletteStage.stream,
                     addr,
                     stage->paletteSize,
                     (uint8_t*)&streamPaletteStage.base.state->writeColor,
                     indexDataReady) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    // only stall out of the read is still pending
    if(leStream_IsDataReady(&streamPaletteStage.stream) == LE_FALSE)
    {
        streamPaletteStage.state = SS_STALLED;

        return LE_FAILURE;
    }

    advanceStage();

    if(streamPaletteStage.state == SS_DONE)
    {
        streamPaletteStage.state = SS_READY;

        return LE_SUCCESS;
    }

    return LE_FAILURE;
}

static leResult exec_blocking(struct StreamPaletteStage* stage)
{
    uint32_t addr;

    addr = (uint32_t)streamPaletteStage.base.state->source->palette->header.address +
            streamPaletteStage.base.state->writeColor *
            stage->paletteSize;

    while(leStream_Read(&streamPaletteStage.stream,
                        addr,
                        stage->paletteSize,
                        (uint8_t*)&streamPaletteStage.base.state->writeColor,
                        NULL) != LE_SUCCESS)
    { }

    return LE_SUCCESS;
}

static void cleanup(struct StreamPaletteStage* stage)
{
    (void)stage;

    leStream_Close(&streamPaletteStage.stream);
}

leResult _leRawImageDecoder_LookupStage_Stream(leRawDecodeState* state)
{
    memset(&streamPaletteStage, 0, sizeof(streamPaletteStage));

    lePixelBufferCreate(LE_ASSET_DECODER_PALETTE_CACHE_SIZE / leColorInfoTable[state->source->palette->buffer.mode].size,
                        1,
                        state->source->palette->buffer.mode,
                        cache,
                        &streamPaletteStage.cacheBuffer);

    leStream_Init(&streamPaletteStage.stream,
                  (leStreamDescriptor*)state->source->palette,
                  LE_ASSET_DECODER_PALETTE_CACHE_SIZE,
                  leRawImageDecoderPaletteScratchBuffer,
                  NULL);

    if(leStream_Open(&streamPaletteStage.stream) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    streamPaletteStage.paletteSize = leColorInfoTable[state->source->palette->buffer.mode].size;

    streamPaletteStage.base.state = state;
    streamPaletteStage.base.cleanup = (void*)cleanup;

    if(leStream_IsBlocking(&streamPaletteStage.stream) == LE_TRUE)
    {
        streamPaletteStage.base.exec = (void*)exec_blocking;
    }
    else
    {
        streamPaletteStage.base.exec = (void*)exec_nonblocking;
    }

    _leRawImageDecoder_InjectStage(state, (void*)&streamPaletteStage);

    return LE_SUCCESS;
}

static struct ImageRenderPostLookupStage
{
    leRawDecodeStage base;
} imageRenderPostLookupStage;

static leResult stage_imageRenderPostLookup(leRawDecodeStage* stage)
{
    if(stage->state->needToLookupMaskColor == LE_TRUE &&
       streamPaletteStage.lookupIndex == stage->state->target->mask.color)
    {
        stage->state->target->mask.color = stage->state->writeColor;

        stage->state->needToLookupMaskColor = LE_FALSE;
    }

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_ImageRenderPostLookupStage_Stream(leRawDecodeState* state)
{
    memset(&imageRenderPostLookupStage, 0, sizeof(imageRenderPostLookupStage));

    state->needToLookupMaskColor = LE_TRUE;

    imageRenderPostLookupStage.base.state = state;
    imageRenderPostLookupStage.base.exec = stage_imageRenderPostLookup;

    _leRawImageDecoder_InjectStage(state, (void*)&imageRenderPostLookupStage);

    return LE_SUCCESS;
}

#endif

#endif /* LE_ENABLE_RAW_DECODER */