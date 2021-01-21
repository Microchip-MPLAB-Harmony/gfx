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

#if LE_STREAMING_ENABLED == 1

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/image/legato_image_utils.h"

#if LE_ASSET_DECODER_USE_PIXEL_CACHE == 1
#define cache leRawImageDecoderScratchBuffer
#endif

#define RLE_HEADER_SIZE 2
#define RLE_BLOCK_SIZE_MAX 8

static struct StreamReadStage
{
    leRawDecodeStage base;

    void (*decode)(void);

    uint32_t mod;
    uint32_t dataSize;
    uint32_t imgBPP;
    uint32_t imgOffs;

    lePixelBuffer readBuffer;

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
} streamReadStage;

static leResult decode(struct StreamReadStage* stage);

static void colorDataReady(leStream* strm)
{
    streamReadStage.stalled = LE_FALSE;

    // advance to the next stage
    streamReadStage.base.state->currentStage = streamReadStage.base.state->maskStage;
}

static void colorDecode()
{
    uint32_t addr;

    // get the address of the desired pixel
    addr = (uint32_t)streamReadStage.base.state->source->header.address +
           streamReadStage.base.state->bufferIdx *
               leColorInfoTable[streamReadStage.base.state->source->buffer.mode].size;

    // read the pixel
    leStream_Read(&streamReadStage.stream,
                  addr,
                  leColorInfoTable[streamReadStage.base.state->source->buffer.mode].size,
                  (uint8_t*)&streamReadStage.base.state->sourceColor,
                  colorDataReady);

    // only stall out of the read is still pending
    streamReadStage.stalled = !leStream_IsDataReady(&streamReadStage.stream);
}

static void rleHeaderDataReady(leStream* strm)
{
    // get the header values
    streamReadStage.rleDataSize = (streamReadStage.rleLengthSize & 0xFF00) >> 8;
    streamReadStage.rleLengthSize = (streamReadStage.rleLengthSize & 0xFF);

    streamReadStage.base.exec = (void*)decode;

    // resume processing
    streamReadStage.stalled = LE_FALSE;
}

static void rleHeaderDecode(struct StreamReadStage* stage)
{
    // read the header data
    leStream_Read(&streamReadStage.stream,
                  (uint32_t)streamReadStage.base.state->source->header.address,
                  RLE_HEADER_SIZE,
                  (void*)&streamReadStage.rleLengthSize,
                  rleHeaderDataReady);

    // only stall out of the request wasn't handled immediately
    streamReadStage.stalled = !leStream_IsDataReady(&streamReadStage.stream);
}

static void rleDataReady(leStream* strm)
{
    uint32_t i;

    // calculate the RLE block values
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

    // get the color data
    streamReadStage.base.state->sourceColor = streamReadStage.rleData;
}

static void readRLEData()
{
    // don't increment on the first read
    if(streamReadStage.rleDecodeCount > 0)
    {
        streamReadStage.rleIndexOffset += streamReadStage.rleLength;
        streamReadStage.rleBlockOffset += 1;
    }

    // read the next RLE block
    leStream_Read(&streamReadStage.stream,
                 (uint32_t)(((uint8_t*)streamReadStage.stream.desc->address) +
                     RLE_HEADER_SIZE +
                     (streamReadStage.rleBlockOffset * (streamReadStage.rleLengthSize + streamReadStage.rleDataSize))),
                 streamReadStage.rleLengthSize + streamReadStage.rleDataSize,
                 (void*)&streamReadStage.rleBlock,
                 rleDataReady);

    streamReadStage.rleDecodeCount += 1;

    // no need to stall out if the request was handled immediately
    if(leStream_IsDataReady(&streamReadStage.stream) == LE_TRUE)
    {
        streamReadStage.stalled = LE_FALSE;
    }
}

static void rleDecode()
{
    // if the offset is contained in the current cache then just return the value
    if(streamReadStage.rleDecodeCount > 0 &&
        streamReadStage.base.state->bufferIdx >= streamReadStage.rleIndexOffset &&
        streamReadStage.base.state->bufferIdx <= streamReadStage.rleIndexOffset + streamReadStage.rleLength)
    {
        streamReadStage.stalled = LE_FALSE;
    }
    // request more data
    else
    {
        streamReadStage.stalled = LE_TRUE;

        while(streamReadStage.stalled == LE_TRUE)
        {
            readRLEData();

            // waiting for data
            if(streamReadStage.stalled == LE_TRUE)
                break;

            // found the appropriate RLE entry
            if(streamReadStage.base.state->bufferIdx >= streamReadStage.rleIndexOffset &&
               streamReadStage.base.state->bufferIdx <= streamReadStage.rleIndexOffset + streamReadStage.rleLength)
            {
                streamReadStage.stalled = LE_FALSE;
            }
            else
            {
                streamReadStage.stalled = LE_TRUE;
            }
        }
    }
}

static void indexDataReady(leStream* strm)
{
    // get a pixel from the buffer
    streamReadStage.base.state->sourceColor = lePixelBufferGetIndex_Unsafe(&streamReadStage.readBuffer,
                                                                           streamReadStage.base.state->bufferIdx % streamReadStage.mod);

    // resume processing
    streamReadStage.stalled = LE_FALSE;
}

static void indexDecode()
{
    uint32_t offs;

    // calculate the offset of the desired pixel in bytes
    offs = leGetOffsetFromIndexAndBPP(streamReadStage.base.state->bufferIdx,
                                      leColorInfoTable[streamReadStage.base.state->source->buffer.mode].bppOrdinal);

    // read an index value
    leStream_Read(&streamReadStage.stream,
                  (uint32_t)streamReadStage.base.state->source->header.address + offs,
                  leColorInfoTable[streamReadStage.base.state->source->buffer.mode].size,
                  (uint8_t*)&streamReadStage.base.state->sourceColor,
                  indexDataReady);

    // stall out if the data read is still pending
    streamReadStage.stalled = !leStream_IsDataReady(&streamReadStage.stream);
}

static void irleDataReady(leStream* strm)
{
    uint32_t i;

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

    // grab the index value
    streamReadStage.base.state->sourceColor = leGetDiscreteValueAtIndex(streamReadStage.base.state->bufferIdx,
                                                                        streamReadStage.rleData,
                                                                        streamReadStage.base.state->source->buffer.mode);
}

static void readIRLEData()
{
    // only increment after the first read
    if(streamReadStage.rleDecodeCount > 0)
    {
        streamReadStage.rleIndexOffset += streamReadStage.rleLength;
        streamReadStage.rleBlockOffset += 1;
    }

    // get some RLE data
    leStream_Read(&streamReadStage.stream,
                  (uint32_t)(((uint8_t*)streamReadStage.stream.desc->address) +
                      RLE_HEADER_SIZE +
                      (streamReadStage.rleBlockOffset * (streamReadStage.rleLengthSize + streamReadStage.rleDataSize))),
                  streamReadStage.rleLengthSize + streamReadStage.rleDataSize,
                  (void*)&streamReadStage.rleBlock,
                  irleDataReady);

    streamReadStage.rleDecodeCount += 1;

    // no need to stall out if the request was handled immediately
    if(leStream_IsDataReady(&streamReadStage.stream) == LE_TRUE)
    {
        streamReadStage.stalled = LE_FALSE;
    }
}

static void irleDecode()
{
    uint32_t offs;

    // calculate byte offset of desired index
    offs = leGetOffsetFromIndexAndBPP(streamReadStage.base.state->bufferIdx,
                                      streamReadStage.imgBPP);

    // if the offset is contained in the current cache then just return the value
    if(streamReadStage.rleDecodeCount > 0 &&
        offs >= streamReadStage.rleIndexOffset &&
        offs < streamReadStage.rleIndexOffset + streamReadStage.rleLength)
    {
        streamReadStage.base.state->sourceColor = leGetDiscreteValueAtIndex(streamReadStage.base.state->bufferIdx,
                                                                            streamReadStage.rleData,
                                                                            streamReadStage.base.state->source->buffer.mode);

        streamReadStage.stalled = LE_FALSE;
    }
    // request more data
    else
    {
        streamReadStage.stalled = LE_TRUE;

        while(streamReadStage.stalled == LE_TRUE)
        {
            readIRLEData();

            // waiting for data
            if(streamReadStage.stalled == LE_TRUE)
                break;

            // found the appropriate RLE entry
            if(offs >= streamReadStage.rleIndexOffset &&
                offs < streamReadStage.rleIndexOffset + streamReadStage.rleLength)
            {
                streamReadStage.stalled = LE_FALSE;
            }
            // get the next RLE block
            else
            {
                streamReadStage.stalled = LE_TRUE;
            }
        }
    }
}

static leResult decode(struct StreamReadStage* stage)
{
    leRawDecodeState* state = stage->base.state;

    // no need to do anything if a data read is still pending
    if(streamReadStage.stalled == LE_TRUE)
        return LE_SUCCESS;

    // determine if the image is finished
    if(state->imageRow >= (uint32_t)state->sourceRect.height)
    {
        state->currentStage = NULL;

        return LE_SUCCESS;
    }

    // calculate read metrics
    state->drawY = state->renderY + state->imageRow;
    state->sourceY = state->sourceRect.y + state->imageRow;

    state->drawX = state->renderX + state->imageCol;
    state->sourceX = state->sourceRect.x + state->imageCol;

    streamReadStage.base.state->bufferIdx = state->sourceX +
                                            (state->sourceY *
                                             state->source->buffer.size.width);

    // attempt to decode a pixel value
    streamReadStage.decode();

    // stall out if a data read is pending
    if(streamReadStage.stalled == LE_TRUE)
        return LE_SUCCESS;

    // draw the pixel and increment
    state->currentStage = state->maskStage;

    if(state->imageCol < (uint32_t)state->sourceRect.width - 1)
    {
        state->imageCol += 1;
    }
    else
    {
        state->imageCol = 0;
        state->imageRow += 1;
    }

    return LE_SUCCESS;
}

static void cleanup(struct StreamReadStage* stage)
{
    leStream_Close(&streamReadStage.stream);
}

leResult _leRawImageDecoder_ReadStreamInit(leRawDecodeState* state)
{
    memset(&streamReadStage, 0, sizeof(streamReadStage));

    state->imageRow = 0;
    state->imageCol = 0;

#if LE_ASSET_DECODER_USE_PIXEL_CACHE == 0
    if(state->source->buffer.mode == LE_COLOR_MODE_INDEX_1)
    {
        streamReadStage.mod = 8;
    }
    else if(state->source->buffer.mode == LE_COLOR_MODE_INDEX_4)
    {
        streamReadStage.mod = 2;
    }
    else
    {
        streamReadStage.mod = 1;
    }

    leStream_Init(&streamReadStage.stream,
                  (leStreamDescriptor*)state->source,
                  0,
                  NULL,
                  NULL);
#else
    leStream_Init(&streamReadStage.stream,
                  (struct leStreamDescriptor*)state->source,
                  LE_ASSET_DECODER_CACHE_SIZE,
                  leRawImageDecoderScratchBuffer,
                  NULL);
#endif

    if(leStream_Open(&streamReadStage.stream) == LE_FAILURE)
    {
        return LE_FAILURE;
    }

    streamReadStage.base.state = state;
    streamReadStage.base.cleanup = (void*)cleanup;
    streamReadStage.base.exec = (void*)decode;

    streamReadStage.dataSize = leColorInfoTable[state->source->buffer.mode].size;
    streamReadStage.imgBPP = leColorInfoTable[state->source->buffer.mode].bppOrdinal;

    if(LE_COLOR_MODE_IS_INDEX(streamReadStage.base.state->source->buffer.mode) == LE_TRUE)
    {
        if(streamReadStage.base.state->source->format == LE_IMAGE_FORMAT_RAW)
        {
            lePixelBufferCreate(leColorInfoTable[state->source->buffer.mode].size,
                                1,
                                state->source->buffer.mode,
                                &streamReadStage.base.state->sourceColor,
                                &streamReadStage.readBuffer);

            streamReadStage.decode = (void*)indexDecode;
        }
        else
        {
            streamReadStage.decode = (void*)irleDecode;
            streamReadStage.base.exec = (void*)rleHeaderDecode;
        }
    }
    else
    {
        if(streamReadStage.base.state->source->format == LE_IMAGE_FORMAT_RAW)
        {
            streamReadStage.decode = (void*)colorDecode;
        }
        else
        {
            streamReadStage.decode = (void*)rleDecode;
            streamReadStage.base.exec = (void*)rleHeaderDecode;
        }
    }

    state->readStage = (void*)&streamReadStage;

    return LE_SUCCESS;
}

#endif