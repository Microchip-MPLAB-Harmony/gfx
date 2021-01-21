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

#include "gfx/legato/common/legato_color.h"
#include "gfx/legato/common/legato_rect.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"

#define MAX_IMAGE_DECODER_COUNT 10

static leImageDecoder* decoders[MAX_IMAGE_DECODER_COUNT];

leImageDecoder* _leRawImageDecoder_Init(void);

void leImage_InitDecoders()
{
    uint32_t decIdx = 0;
    
    decoders[decIdx++] = _leRawImageDecoder_Init();
}

leResult leImage_Create(leImage* img,
                        uint32_t width,
                        uint32_t height,
                        leColorMode mode,
                        void* data,
                        uint32_t locationID)
{
    if(img == NULL || width == 0 || height == 0)
        return LE_FAILURE;

    img->header.address = data;
    img->header.location = locationID;
    img->header.size = width * height * leColorInfoTable[mode].size;
    img->format = LE_IMAGE_FORMAT_RAW;
    img->buffer.size.width = width;
    img->buffer.size.height = height;
    img->buffer.buffer_length = img->header.size;
    img->buffer.mode = mode;
    img->buffer.pixels = data;
    img->buffer.pixel_count = width * height;
    img->flags = 0;
    img->mask.color = 0;
    img->alphaMap = NULL;
    img->palette = NULL;

    return LE_SUCCESS;
}

leImage* leImage_Allocate(uint32_t width,
                          uint32_t height,
                          leColorMode mode)
{
    leImage* img;

    if(width == 0 || height == 0)
        return NULL;

    img = LE_MALLOC(sizeof(leImage));

    if(img == NULL)
        return NULL;

    img->header.size = width * height * leColorInfoTable[mode].size;

    img->header.address = LE_MALLOC(img->header.size);

    if(img->header.address == NULL)
    {
        leFree(img);

        return NULL;
    }

    img->header.location = LE_STREAM_LOCATION_ID_INTERNAL;
    img->format = LE_IMAGE_FORMAT_RAW;
    img->buffer.size.width = width;
    img->buffer.size.height = height;
    img->buffer.buffer_length = img->header.size;
    img->buffer.mode = mode;
    img->buffer.pixels = img->header.address;
    img->buffer.pixel_count = width * height;
    img->flags = LE_IMAGE_INTERNAL_ALLOC;
    img->mask.color = 0;
    img->alphaMap = NULL;
    img->palette = NULL;

    return img;
}

leResult leImage_Free(leImage* img)
{
    if(img == NULL || (img->flags & LE_IMAGE_INTERNAL_ALLOC) == 0)
        return LE_FAILURE;

    leFree(img->header.address);
    leFree(img);

    return LE_SUCCESS;
}

leResult leImage_Draw(const leImage* img,
                      leRect* sourceRect,
                      int32_t x,
                      int32_t y,
                      uint32_t a)
{

    uint32_t decIdx;

    if(img == NULL || sourceRect == NULL)
        return LE_FAILURE;

    for(decIdx = 0; decIdx < MAX_IMAGE_DECODER_COUNT; decIdx++)
    {
        if(decoders[decIdx] != NULL && decoders[decIdx]->supportsImage(img) == LE_TRUE)
        {
            if(decoders[decIdx]->draw(img,
                                      sourceRect,
                                      x,
                                      y,
                                      a) == LE_SUCCESS)
            {
                decoders[decIdx]->exec();

                break;
            }
        }
    }

    return LE_SUCCESS;
}



/*leResult leImage_Render(const leImage* img,
                        leImage* dst,
                        leRect* sourceRect,
                        leBool applyAlpha)
{
    (void)img;
    (void)dst;
    (void)sourceRect;
    (void)applyAlpha;

    return LE_FAILURE;
}*/

leResult leImage_Resize(const leImage* src,
                        const leRect* sourceRect,
                        leImageFilterMode mode,
                        uint32_t sizeX,
                        uint32_t sizeY,
                        leImage* target)
{
    uint32_t decIdx;

    if(src == NULL || sourceRect == NULL || target == NULL)
        return LE_FAILURE;

    if(target->buffer.size.width == 0 ||
       target->buffer.size.height == 0 ||
       target->buffer.pixels == NULL ||
       sizeX == 0 ||
       sizeY == 0)
    {
        return LE_FAILURE;
    }

    for(decIdx = 0; decIdx < MAX_IMAGE_DECODER_COUNT; decIdx++)
    {
        if(decoders[decIdx] != NULL && decoders[decIdx]->supportsImage(src) == LE_TRUE)
        {
            if(decoders[decIdx]->resize(src,
                                        sourceRect,
                                        mode,
                                        sizeX,
                                        sizeY,
                                        target) == LE_SUCCESS)
            {
                decoders[decIdx]->exec();

                break;
            }
        }
    }

    return LE_FAILURE;
}

leResult leImage_ResizeDraw(const leImage* src,
                            const leRect* sourceRect,
                            leImageFilterMode mode,
                            uint32_t sizeX,
                            uint32_t sizeY,
                            int32_t x,
                            int32_t y,
                            uint32_t a)
{
    uint32_t decIdx;

    for(decIdx = 0; decIdx < MAX_IMAGE_DECODER_COUNT; decIdx++)
    {
        if(decoders[decIdx] != NULL && decoders[decIdx]->supportsImage(src) == LE_TRUE)
        {
            if(decoders[decIdx]->resizeDraw(src,
                                            sourceRect,
                                            mode,
                                            sizeX,
                                            sizeY,
                                            x,
                                            y,
                                            a) == LE_SUCCESS)
            {
                decoders[decIdx]->exec();

                break;
            }
        }
    }

    return LE_FAILURE;
}

leResult leImage_Copy(const leImage* src,
                      const leRect* sourceRect,
                      int32_t x,
                      int32_t y,
                      leImage* dst)
{
    uint32_t decIdx;

    for(decIdx = 0; decIdx < MAX_IMAGE_DECODER_COUNT; decIdx++)
    {
        if(decoders[decIdx] != NULL && decoders[decIdx]->supportsImage(src) == LE_TRUE)
        {
            if(decoders[decIdx]->copy(src,
                                      sourceRect,
                                      x,
                                      y,
                                      dst) == LE_SUCCESS)
            {
                decoders[decIdx]->exec();

                break;
            }
        }
    }

    return LE_FAILURE;
}

leResult leImage_Render(const leImage* src,
                        const leRect* sourceRect,
                        int32_t x,
                        int32_t y,
                        leBool ignoreMask,
                        leBool ignoreAlpha,
                        leImage* dst)
{
    uint32_t decIdx;

    for(decIdx = 0; decIdx < MAX_IMAGE_DECODER_COUNT; decIdx++)
    {
        if(decoders[decIdx] != NULL && decoders[decIdx]->supportsImage(src) == LE_TRUE)
        {
            if(decoders[decIdx]->render(src,
                                        sourceRect,
                                        x,
                                        y,
                                        ignoreMask,
                                        ignoreAlpha,
                                        dst) == LE_SUCCESS)
            {
                decoders[decIdx]->exec();

                break;
            }
        }
    }

    return LE_FAILURE;
}

leResult leImage_Rotate(const leImage* src,
                        const leRect* sourceRect,
                        leImageFilterMode mode,
                        const lePoint* origin,
                        int32_t angle,
                        leImage* dst)
{
    uint32_t decIdx;

    for(decIdx = 0; decIdx < MAX_IMAGE_DECODER_COUNT; decIdx++)
    {
        if(decoders[decIdx] != NULL && decoders[decIdx]->supportsImage(src) == LE_TRUE)
        {
            if(decoders[decIdx]->rotate(src,
                                        sourceRect,
                                        mode,
                                        origin,
                                        angle,
                                        dst) == LE_SUCCESS)
            {
                decoders[decIdx]->exec();

                break;
            }
        }
    }

    return LE_FAILURE;
}

leResult leImage_RotateDraw(const leImage* src,
                            const leRect* sourceRect,
                            leImageFilterMode mode,
                            const lePoint* origin,
                            int32_t angle,
                            int32_t x,
                            int32_t y,
                            uint32_t a)
{
    uint32_t decIdx;

    for(decIdx = 0; decIdx < MAX_IMAGE_DECODER_COUNT; decIdx++)
    {
        if(decoders[decIdx] != NULL && decoders[decIdx]->supportsImage(src) == LE_TRUE)
        {
            if(decoders[decIdx]->rotateDraw(src,
                                            sourceRect,
                                            mode,
                                            origin,
                                            angle,
                                            x,
                                            y,
                                            a) == LE_SUCCESS)
            {
                decoders[decIdx]->exec();

                break;
            }
        }
    }

    return LE_FAILURE;
}