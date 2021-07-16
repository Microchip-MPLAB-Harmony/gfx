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

#if LE_ENABLE_JPEG_DECODER == 1

#include "gfx/legato/image/jpeg/legato_imagedecoder_jpeg.h"
#include "gfx/legato/image/jpeg/legato_imagedecoder_jpeg_common.h"

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/core/legato_stream.h"
#include "gfx/legato/image/legato_palette.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"

#if LE_STREAMING_ENABLED == 1

LE_COHERENT_ATTR uint8_t leJPEGImageDecoderScratchBuffer[LE_JPEG_DECODER_CACHE_SIZE];

#endif

static LE_COHERENT_ATTR leImageDecoder decoder;

static leBool _supportsImage(const leImage* img)
{
#if LE_STREAMING_ENABLED == 0
    if(img->header.location != LE_STREAM_LOCATION_ID_INTERNAL)
        return LE_FALSE;
#endif

    return img->format == LE_IMAGE_FORMAT_JPEG;
}

static uint32_t internalMemoryRead(void* ptr, uint32_t size, uint32_t n, JPEGDECODER* dcd)
{
    uint32_t count = 0;
    uint8_t* pFile;
    uint8_t* bptr;

    pFile = (uint8_t*)dcd->pImageFile->header.address;
    bptr = (uint8_t *)ptr;

    for(count = 0; count < size * n; ++count)
    {
        bptr[count] = (uint8_t) pFile[dcd->fileIndex];
        ++dcd->fileIndex;
    }

    return count;
}

#if LE_STREAMING_ENABLED == 1
static uint32_t externalMemoryRead(void* ptr, uint32_t size, uint32_t n, JPEGDECODER* dcd)
{
    uint32_t count = size * n;
    uint32_t addr = (size_t)dcd->pImageFile->header.address + dcd->fileIndex;

    while(leStream_Read(&dcd->stream, addr, count, ptr, NULL) != LE_SUCCESS)
    { }

    dcd->fileIndex += count;

    return count;
}
#endif

static void blitToFrameBuffer(JPEGDECODER* state,
                              lePixelBuffer* buffer,
                              int32_t src_x,
                              int32_t src_y,
                              int32_t src_width,
                              int32_t src_height,
                              int32_t dest_x,
                              int32_t dest_y,
                              uint32_t a)
{
    int32_t row, col;
    uint32_t color;
    (void)state; // unused
    (void)src_width; // unused
    (void)src_height; // unused

    for(row = 0; row < buffer->size.height; ++row)
    {
        for(col = 0; col < buffer->size.width; ++col)
        {
            color = lePixelBufferGet_Unsafe(buffer,
                                            src_x + col,
                                            src_y + row);

            color = leRenderer_ConvertColor(color, buffer->mode);

#if LE_ALPHA_BLENDING_ENABLED == 1
            leRenderer_BlendPixel_Safe(dest_x + col,
                                       dest_y + row,
                                       color,
                                       a);
#else
            (void)a; // unused

            // write color
            leRenderer_PutPixel_Safe(dest_x + col,
                                     dest_y + row,
                                     color);
#endif
        }
    }
}

static void blitToImage(JPEGDECODER* state,
                        lePixelBuffer* buffer,
                        int32_t src_x,
                        int32_t src_y,
                        int32_t src_width,
                        int32_t src_height,
                        int32_t dest_x,
                        int32_t dest_y,
                        uint32_t a)
{
    int32_t row, col;
    uint32_t color;
    (void)state; // unused
    (void)src_width; // unused
    (void)src_height; // unused
    (void)a; // unused

    for(row = 0; row < buffer->size.height; ++row)
    {
        for(col = 0; col < buffer->size.width; ++col)
        {
            color = lePixelBufferGet_Unsafe(buffer,
                                            src_x + col,
                                            src_y + row);

            color = leColorConvert(color, buffer->mode, state->imageWriteBuffer->mode);

            lePixelBufferSet(state->imageWriteBuffer,
                             dest_x + col,
                             dest_y + row,
                             color);
        }
    }
}



static leResult _draw(const leImage* img,
                      const leRect* srcRect,
                      int32_t x,
                      int32_t y,
                      uint32_t a)
{
    static uint16_t whblocks, wvblocks;
    static uint16_t wi, wj;
    static JPEGDECODER JPEG_JpegDecoder;
    (void)srcRect; // unused

    memset(&JPEG_JpegDecoder, 0, sizeof(JPEG_JpegDecoder));
    JPEG_vResetDecoder(&JPEG_JpegDecoder);

#if LE_STREAMING_ENABLED == 1
    if(img->header.location != LE_STREAM_LOCATION_ID_INTERNAL)
    {
       leStream_Init(&JPEG_JpegDecoder.stream,
                     &img->header,
                     LE_JPEG_DECODER_CACHE_SIZE,
                     leJPEGImageDecoderScratchBuffer,
                     NULL);

        if(leStream_Open(&JPEG_JpegDecoder.stream) == LE_FAILURE)
            return LE_FAILURE;

        JPEG_JpegDecoder.stream.flags |= SF_BLOCKING;
    }
#endif

    JPEG_JpegDecoder.pImageFile = img;

    if(img->header.location != LE_STREAM_LOCATION_ID_INTERNAL)
    {
#if LE_STREAMING_ENABLED == 1
        JPEG_JpegDecoder.readPtr = externalMemoryRead;
#else
        return LE_FAILURE;
#endif
    }
    else
    {
        JPEG_JpegDecoder.readPtr = internalMemoryRead;
    }

    JPEG_JpegDecoder.blitPtr = blitToFrameBuffer;
    JPEG_JpegDecoder.globalAlpha = a;
    leRenderer_GetDrawRect(&JPEG_JpegDecoder.clipRect);
    JPEG_JpegDecoder.wStartY = 0;
    JPEG_JpegDecoder.wStartX = 0;
    JPEG_JpegDecoder.wDrawWidth = img->buffer.size.width;
    JPEG_JpegDecoder.wDrawHeight = img->buffer.size.height;
    JPEG_JpegDecoder.wDrawX = x;
    JPEG_JpegDecoder.wDrawY = y;

    if(JPEG_bReadHeader(&JPEG_JpegDecoder) != 0)
    {
#if LE_STREAMING_ENABLED == 1
        if(leStream_IsOpen(&JPEG_JpegDecoder.stream) == LE_TRUE)
        {
            leStream_Close(&JPEG_JpegDecoder.stream);
        }
#endif

        return LE_FAILURE;
    }

    JPEG_bGenerateHuffmanTables(&JPEG_JpegDecoder);

    whblocks = JPEG_JpegDecoder.wWidth >> 3;
    wvblocks = JPEG_JpegDecoder.wHeight >> 3;

    if((whblocks << 3) < JPEG_JpegDecoder.wWidth) /* Odd sizes */
    {
        whblocks++;
    }

    if((wvblocks << 3) < JPEG_JpegDecoder.wHeight) /* Odd sizes */
    {
        wvblocks++;
    }

    if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_1x2)
    {
        wvblocks =  (wvblocks>>1) + (wvblocks&1);
    }
    else if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_2x1)
    {
        whblocks = (whblocks>>1) + (whblocks&1);
    }
    else if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_2x2)
    {
        wvblocks =  (wvblocks>>1) + (wvblocks&1);
        whblocks = (whblocks>>1) + (whblocks&1);
    }

    JPEG_vInitDisplay(&JPEG_JpegDecoder);

    wi = 0;
    wj = 0;

    while(wi < whblocks)
    {
        JPEG_bDecodeOneBlock(&JPEG_JpegDecoder); /* Fills a block after correcting the zigzag, dequantizing, IDCR and color conversion to RGB */
        JPEG_bPaintOneBlock(&JPEG_JpegDecoder);

        wj++;

        if(wj >= wvblocks)
        {
            wj = 0;
            wi++;
        }
    }

#if LE_STREAMING_ENABLED == 1
    if(leStream_IsOpen(&JPEG_JpegDecoder.stream) == LE_TRUE)
    {
        leStream_Close(&JPEG_JpegDecoder.stream);
    }
#endif

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
    static uint16_t whblocks, wvblocks;
    static uint16_t wi, wj;
    static JPEGDECODER JPEG_JpegDecoder;
    (void)srcRect; // unused
    (void)ignoreMask; // unused
    (void)ignoreAlpha; // unused

    memset(&JPEG_JpegDecoder, 0, sizeof(JPEG_JpegDecoder));

    JPEG_vResetDecoder(&JPEG_JpegDecoder);

#if LE_STREAMING_ENABLED == 1
    if(src->header.location != LE_STREAM_LOCATION_ID_INTERNAL)
    {
        leStream_Init(&JPEG_JpegDecoder.stream,
                      &src->header,
                      LE_JPEG_DECODER_CACHE_SIZE,
                      leJPEGImageDecoderScratchBuffer,
                      NULL);

        if(leStream_Open(&JPEG_JpegDecoder.stream) == LE_FAILURE)
            return LE_FAILURE;

        JPEG_JpegDecoder.stream.flags |= SF_BLOCKING;
    }
#endif

    JPEG_JpegDecoder.pImageFile = src;
    JPEG_JpegDecoder.imageWriteBuffer = &dst->buffer;
    JPEG_JpegDecoder.readPtr = &internalMemoryRead;
    JPEG_JpegDecoder.blitPtr = blitToImage;
    leRenderer_GetDrawRect(&JPEG_JpegDecoder.clipRect);
    JPEG_JpegDecoder.wStartY = 0;
    JPEG_JpegDecoder.wStartX = 0;
    JPEG_JpegDecoder.wDrawWidth = src->buffer.size.width;
    JPEG_JpegDecoder.wDrawHeight = src->buffer.size.height;
    JPEG_JpegDecoder.wDrawX = x;
    JPEG_JpegDecoder.wDrawY = y;

    if(JPEG_bReadHeader(&JPEG_JpegDecoder) != 0)
    {
#if LE_STREAMING_ENABLED == 1
        if(leStream_IsOpen(&JPEG_JpegDecoder.stream) == LE_TRUE)
        {
            leStream_Close(&JPEG_JpegDecoder.stream);
        }
#endif

        return LE_FAILURE;
    }

    JPEG_bGenerateHuffmanTables(&JPEG_JpegDecoder);

    whblocks = JPEG_JpegDecoder.wWidth >> 3;
    wvblocks = JPEG_JpegDecoder.wHeight >> 3;

    if((whblocks << 3) < JPEG_JpegDecoder.wWidth) /* Odd sizes */
    {
        whblocks++;
    }

    if((wvblocks << 3) < JPEG_JpegDecoder.wHeight) /* Odd sizes */
    {
        wvblocks++;
    }

    if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_1x2)
    {
        wvblocks =  (wvblocks>>1) + (wvblocks&1);
    }
    else if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_2x1)
    {
        whblocks = (whblocks>>1) + (whblocks&1);
    }
    else if(JPEG_JpegDecoder.bSubSampleType == JPEG_SAMPLE_2x2)
    {
        wvblocks =  (wvblocks>>1) + (wvblocks&1);
        whblocks = (whblocks>>1) + (whblocks&1);
    }

    JPEG_vInitDisplay(&JPEG_JpegDecoder);

    wi = 0;
    wj = 0;

    while(wi < whblocks)
    {
        JPEG_bDecodeOneBlock(&JPEG_JpegDecoder); /* Fills a block after correcting the zigzag, dequantizing, IDCR and color conversion to RGB */
        JPEG_bPaintOneBlock(&JPEG_JpegDecoder);

        wj++;

        if(wj >= wvblocks)
        {
            wj = 0;
            wi++;
        }
    }

#if LE_STREAMING_ENABLED == 1
    if(leStream_IsOpen(&JPEG_JpegDecoder.stream) == LE_TRUE)
    {
        leStream_Close(&JPEG_JpegDecoder.stream);
    }
#endif

    return LE_SUCCESS;
}

static leResult exec(void)
{
    return LE_SUCCESS;
}

leImageDecoder* _leJPEGImageDecoder_Init(void)
{
    decoder.supportsImage = _supportsImage;
    decoder.draw = _draw;
    decoder.copy = NULL;
    decoder.render = _render;
    decoder.resize = NULL;
    decoder.resizeDraw = NULL;
    decoder.rotate = NULL;
    decoder.rotateDraw = NULL;
    decoder.exec = exec;
    decoder.isDone = NULL;
    decoder.free = NULL;

    return &decoder;
}

#endif /* LE_ENABLE_JPEG_DECODER */