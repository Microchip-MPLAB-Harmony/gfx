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


#include "gfx/legato/image/raw/legato_imagedecoder_raw.h"

#if LE_ENABLE_RAW_DECODER == 1

#if LE_STREAMING_ENABLED == 1

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/image/legato_image_utils.h"

#define cache leRawImageDecoderScratchBuffer

#define RLE_HEADER_SIZE 2
#define RLE_BLOCK_SIZE_MAX 8

void _leRawImageDecoder_InjectStage(leRawDecodeState* state,
                                    leRawDecodeStage* stage);

struct StreamReadStage
{
    leRawDecodeStage base;

    void (*decode)(void);

    uint32_t imgBPP;

    uint32_t rleLengthSize;
    uint32_t rleDataSize;
    uint32_t rleLength;
    uint32_t rleData;
    uint32_t rleIndexOffset;
    uint32_t rleBlockOffset;
    uint32_t rleDecodeCount;

    uint8_t rleBlock[RLE_BLOCK_SIZE_MAX];

    leBool stalled;

    leStream stream;
};

static LE_COHERENT_ATTR struct StreamReadStage streamReadStage;

static leResult exec(struct StreamReadStage* stage);

static leResult advanceStage(void)
{
    streamReadStage.base.state->readIndex += 1;

    if(streamReadStage.base.state->readIndex < streamReadStage.base.state->readCount)
    {
        return LE_FAILURE;
    }
    else
    {
        // reset for next iteration
        streamReadStage.base.state->readIndex = 0;

        return LE_SUCCESS;
    }
}

static void rleHeaderDataReady(leStream* strm)
{
    (void)strm; // unused

    // get the header values
    streamReadStage.rleDataSize = (streamReadStage.rleLengthSize & 0xFF00) >> 8;
    streamReadStage.rleLengthSize = (streamReadStage.rleLengthSize & 0xFF);

    streamReadStage.base.exec = (void*)exec;

    streamReadStage.stalled = LE_FALSE;
}

static leResult rleHeaderDecode(struct StreamReadStage* stage)
{
    (void)stage; // unused

    if(streamReadStage.stalled == LE_TRUE)
        return LE_FAILURE;

    // read the header data
    if(leStream_Read(&streamReadStage.stream,
                     (uint32_t)streamReadStage.base.state->source->header.address,
                     RLE_HEADER_SIZE,
                     (void*)&streamReadStage.rleLengthSize,
                     rleHeaderDataReady) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    // only stall out of the request wasn't handled immediately
    streamReadStage.stalled = !leStream_IsDataReady(&streamReadStage.stream);

    return streamReadStage.stalled == LE_FALSE ? LE_SUCCESS : LE_FAILURE;
}

static void irleDataReady(leStream* strm)
{
    uint32_t i;
    (void)strm; // unused

    // calculate rle block length and get the data
    streamReadStage.rleLength = 0;
    streamReadStage.rleData = 0;

    for(i = 0; i < streamReadStage.rleLengthSize; i++)
    {
        streamReadStage.rleLength |= streamReadStage.rleBlock[i] << (i * 8);
    }

    for(i = 0; i < streamReadStage.rleDataSize; i++)
    {
        streamReadStage.rleData |= streamReadStage.rleBlock[i+streamReadStage.rleLengthSize] << (i * 8);
    }

    streamReadStage.stalled = LE_FALSE;
}

static leResult readIRLEData(void)
{
    // only increment after the first read
    if(streamReadStage.rleDecodeCount > 0)
    {
        streamReadStage.rleIndexOffset += streamReadStage.rleLength;
        streamReadStage.rleBlockOffset += 1;
    }

    // get some RLE data
    if(leStream_Read(&streamReadStage.stream,
                     (uint32_t)(((uint8_t*)streamReadStage.stream.desc->address) +
                     RLE_HEADER_SIZE +
                     (streamReadStage.rleBlockOffset * (streamReadStage.rleLengthSize + streamReadStage.rleDataSize))),
                     streamReadStage.rleLengthSize + streamReadStage.rleDataSize,
                     (void*)&streamReadStage.rleBlock,
                     irleDataReady) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    streamReadStage.rleDecodeCount += 1;

    return LE_SUCCESS;
}

static leResult exec(struct StreamReadStage* stage)
{
    (void)stage; // unused
    uint32_t offs;

    leRawSourceReadOperation* op;

    // no need to do anything if a data read is still pending
    if(streamReadStage.stalled == LE_TRUE)
        return LE_FAILURE;

    op = &streamReadStage.base.state->readOperation[streamReadStage.base.state->readIndex];

    // attempt to decode a pixel value
    // calculate byte offset of desired index
    offs = leGetOffsetFromIndexAndBPP(op->bufferIndex,
                                      streamReadStage.imgBPP);

    // if the offset is contained in the current cache then just return the value
    if(streamReadStage.rleDecodeCount > 0 &&
       offs >= streamReadStage.rleIndexOffset &&
       offs < streamReadStage.rleIndexOffset + streamReadStage.rleLength)
    {
        op->data = leGetDiscreteValueAtIndex(op->bufferIndex,
                                             streamReadStage.rleData,
                                             streamReadStage.base.state->source->buffer.mode);
    }
    // request more data
    else
    {
        streamReadStage.stalled = LE_TRUE;

        while(streamReadStage.stalled == LE_TRUE)
        {
            if(readIRLEData() == LE_FAILURE)
            {
                advanceStage();

                return LE_SUCCESS;
            }

            // waiting for data
            if(streamReadStage.stalled == LE_TRUE)
                break;

            // found the appropriate RLE entry
            if(offs >= streamReadStage.rleIndexOffset &&
               offs < streamReadStage.rleIndexOffset + streamReadStage.rleLength)
            {
                op->data = leGetDiscreteValueAtIndex(op->bufferIndex,
                                                     streamReadStage.rleData,
                                                     streamReadStage.base.state->source->buffer.mode);
            }
            // get the next RLE block
            else
            {
                streamReadStage.stalled = LE_TRUE;
            }
        }
    }

    // stall out if a data read is pending
    if(streamReadStage.stalled == LE_TRUE)
        return LE_FAILURE;

    advanceStage();

    return LE_SUCCESS;
}

static leResult exec_blocking(struct StreamReadStage* stage)
{
    uint32_t offs;
    uint32_t i;
    leRawSourceReadOperation* op;

    stage->base.state->readIndex = 0;

    for(stage->base.state->readIndex = 0;
        stage->base.state->readIndex < stage->base.state->readCount;
        stage->base.state->readIndex++)
    {
        op = &streamReadStage.base.state->readOperation[streamReadStage.base.state->readIndex];

        // attempt to decode a pixel value
        // calculate byte offset of desired index
        offs = leGetOffsetFromIndexAndBPP(op->bufferIndex,
                                          streamReadStage.imgBPP);

        // if the offset is contained in the current cache then just return the value
        if(streamReadStage.rleDecodeCount > 0 &&
           offs >= streamReadStage.rleIndexOffset &&
           offs < streamReadStage.rleIndexOffset + streamReadStage.rleLength)
        {
            op->data = leGetDiscreteValueAtIndex(op->bufferIndex,
                                                 streamReadStage.rleData,
                                                 streamReadStage.base.state->source->buffer.mode);
        }
        // request more data
        else
        {
            if(op->bufferIndex >= streamReadStage.base.state->source->buffer.pixel_count)
                return LE_SUCCESS;

            stage->rleDecodeCount = 0;
            stage->rleIndexOffset = 0;
            stage->rleBlockOffset = 0;
            stage->rleLength = 0;

            while(1)
            {
                // only increment after the first read
                if(streamReadStage.rleDecodeCount > 0)
                {
                    streamReadStage.rleIndexOffset += streamReadStage.rleLength;
                    streamReadStage.rleBlockOffset += 1;
                }

                // get some RLE data
                while(leStream_Read(&streamReadStage.stream,
                                   (uint32_t)(((uint8_t*)streamReadStage.stream.desc->address) +
                                   RLE_HEADER_SIZE +
                                   (streamReadStage.rleBlockOffset * (streamReadStage.rleLengthSize + streamReadStage.rleDataSize))),
                                   streamReadStage.rleLengthSize + streamReadStage.rleDataSize,
                                   (void*)&streamReadStage.rleBlock,
                                   irleDataReady) != LE_SUCCESS)
                { }

                streamReadStage.rleDecodeCount += 1;

                // calculate rle block length and get the data
                streamReadStage.rleLength = 0;
                streamReadStage.rleData = 0;

                for(i = 0; i < streamReadStage.rleLengthSize; i++)
                {
                    streamReadStage.rleLength |= streamReadStage.rleBlock[i] << (i * 8);
                }

                for(i = 0; i < streamReadStage.rleDataSize; i++)
                {
                    streamReadStage.rleData |= streamReadStage.rleBlock[i+streamReadStage.rleLengthSize] << (i * 8);
                }

                // found the appropriate RLE entry
                if(offs >= streamReadStage.rleIndexOffset &&
                   offs < streamReadStage.rleIndexOffset + streamReadStage.rleLength)
                {
                    op->data = leGetDiscreteValueAtIndex(op->bufferIndex,
                                                         streamReadStage.rleData,
                                                         streamReadStage.base.state->source->buffer.mode);

                    break;
                }
            }
        }

        streamReadStage.base.state->readIndex += 1;
    }

    return LE_SUCCESS;
}

static void cleanup(struct StreamReadStage* stage)
{
    (void)stage; // unused

    leStream_Close(&streamReadStage.stream);
}

leResult _leRawImageDecoder_ReadStage_StreamRLEIndex(leRawDecodeState* state)
{
    memset(&streamReadStage, 0, sizeof(streamReadStage));

    leStream_Init(&streamReadStage.stream,
                  (struct leStreamDescriptor*)state->source,
                  LE_ASSET_DECODER_PIXEL_CACHE_SIZE,
                  leRawImageDecoderScratchBuffer,
                  NULL);

    if(leStream_Open(&streamReadStage.stream) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    streamReadStage.base.state = state;
    streamReadStage.base.cleanup = (void*)cleanup;

    if(leStream_IsBlocking(&streamReadStage.stream) == LE_TRUE)
    {
        streamReadStage.base.exec = (void*)exec_blocking;

        // read the header data
        leStream_Read(&streamReadStage.stream,
                      (uint32_t)streamReadStage.base.state->source->header.address,
                      RLE_HEADER_SIZE,
                      (void*)&streamReadStage.rleLengthSize,
                      NULL);

        // get the header values
        streamReadStage.rleDataSize = (streamReadStage.rleLengthSize & 0xFF00) >> 8;
        streamReadStage.rleLengthSize = (streamReadStage.rleLengthSize & 0xFF);
    }
    else
    {
        streamReadStage.base.exec = (void*)rleHeaderDecode;
    }

    streamReadStage.imgBPP = leColorInfoTable[state->source->buffer.mode].bppOrdinal;

    _leRawImageDecoder_InjectStage(state, (void*)&streamReadStage);

    return LE_SUCCESS;
}

#endif

#endif /* LE_ENABLE_RAW_DECODER */