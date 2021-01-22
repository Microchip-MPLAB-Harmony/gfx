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


#include "gfx/legato/common/legato_pixelbuffer.h"

#include "gfx/legato/common/legato_color.h"
#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_rect.h"

static leColor indexGet1bpp(const lePixelBuffer* const buffer,
                            const uint32_t idx)
{
    uint32_t offs;
    uint8_t byte;
    leColor color;
    
    offs = (idx % 8);
            
    byte = *(((uint8_t*)buffer->pixels) + (idx >> 3));
    
    color = byte & (0x80 >> offs);
    
    color >>= 7 - offs;
    
    return color;
}

static leColor indexGet4bpp(const lePixelBuffer* const buffer,
                              const uint32_t idx)
{
    uint32_t offs;
    uint8_t byte;
    leColor color;
    
    byte = *(((uint8_t*)buffer->pixels) + (idx >> 1));
    
    if(idx % 2 > 0)
    {
        offs = 0xF;
        color = byte & offs;
    }
    else
    {
        offs = 0xF0;
        color = (byte & offs) >> 0x4;
    }
    
    return color;
}

static leColor indexGet(const lePixelBuffer* const buffer,
                          const uint32_t idx)
{
    uint8_t* buf_ptr;
    leColor color = 0;
    uint32_t i;

    buf_ptr = ((uint8_t*)buffer->pixels) + (leColorInfoTable[buffer->mode].bpp >> 3) * idx;
    
    for(i = 0; i < leColorInfoTable[buffer->mode].bpp >> 3; i++)
        ((uint8_t*)&color)[i] = buf_ptr[i];
    
    return color;
}
                               
typedef leColor (*PixelBufferIndexGet_FnPtr)(const lePixelBuffer* const,
                                               const uint32_t);
                                               
PixelBufferIndexGet_FnPtr indexGet_FnTable[] =
{
    &indexGet1bpp,
    &indexGet4bpp,
    &indexGet,
    &indexGet,
    &indexGet,
    &indexGet
};

typedef leColor (*GetPixel_FnPtr)(uint8_t*);

static leColor get1byte(uint8_t* ptr)
{
    leColor clr = ptr[0];
    
    return clr;
}

static leColor get2byte(uint8_t* ptr)
{
    leColor clr = ((uint16_t*)ptr)[0];
    
    return clr;
}

static leColor get3byte(uint8_t* ptr)
{
    leColor clr;
    
    clr = ptr[0];
    clr |= ptr[1] << 8;
    clr |= ptr[2] << 16;
    
    return clr;
}

static leColor get4byte(uint8_t* ptr)
{
    leColor clr = ((uint32_t*)ptr)[0];
    
    return clr;
}

GetPixel_FnPtr pixelGet_FnTable[] =
{
    &get1byte,
    &get2byte,
    &get3byte,
    &get4byte
};

typedef leResult (*SetPixel_FnPtr)(uint8_t*, leColor);

static leResult set1byte(uint8_t* ptr, leColor clr)
{
    ptr[0] = (uint8_t)clr;
    
    return LE_SUCCESS;
}

static leResult set2byte(uint8_t* ptr, leColor clr)
{
    *((uint16_t*)ptr) = (uint16_t)clr;
    
    return LE_SUCCESS;
}

static leResult set3byte(uint8_t* ptr, leColor clr)
{
    ptr[0] = (uint8_t)(clr & 0xFF);
    ptr[1] = (uint8_t)((clr & 0xFF00) >> 8);
    ptr[2] = (uint8_t)((clr & 0xFF0000) >> 16);
    
    return LE_SUCCESS;
}

static leResult set4byte(uint8_t* ptr, leColor clr)
{
    *((uint32_t*)ptr) = clr;
    
    return LE_SUCCESS;
}

SetPixel_FnPtr pixelSet_FnTable[] =
{
    &set1byte,
    &set2byte,
    &set3byte,
    &set4byte
};

leResult lePixelBufferCreate(const int32_t width,
                             const int32_t height,
                             const leColorMode mode,
                             const void* const address,
                             lePixelBuffer* buffer)
{
    if(buffer == NULL)
        return LE_FAILURE;

    buffer->size.width = width;
    buffer->size.height = height;
    buffer->pixel_count = width * height;

    buffer->mode = mode;

    switch(mode)
    {
        case LE_COLOR_MODE_INDEX_1:
        {
            buffer->buffer_length = (width * height) / 8;
            
            if((width * height) % 8 > 0)
                buffer->buffer_length += 1;
                        
            break;
        }
        case LE_COLOR_MODE_INDEX_4:
        {
            buffer->buffer_length = (width * height) / 2;
            
            if((width * height) % 2 > 0)
                buffer->buffer_length += 1;
        
            break;
        }
        case LE_COLOR_MODE_INDEX_8:
        {
            buffer->buffer_length = (width * height);
        
            break;
        }
        default:
        {
            buffer->buffer_length = width * height * ((leColorInfoTable[buffer->mode].bpp >> 3));
        }
    }

    buffer->pixels = (void*)address;

    return LE_SUCCESS;
}

leBuffer lePixelBufferOffsetGet(const lePixelBuffer* const buffer,
                                uint32_t x,
                                uint32_t y)
{
    uint8_t* buf_ptr;

    if(buffer == NULL)
        return NULL;

    buf_ptr = (uint8_t*)buffer->pixels;
    
    if(x * y >= buffer->pixel_count)
        return NULL;
    
    return buf_ptr +
            leColorInfoTable[buffer->mode].size *
            (x + (y * buffer->size.width));
}

leBuffer lePixelBufferOffsetGet_Unsafe(const lePixelBuffer* const buffer,
                                       uint32_t x,
                                       uint32_t y)
{
    uint8_t* buf_ptr;

    buf_ptr = (uint8_t*)buffer->pixels;
    
    return buf_ptr +
           leColorInfoTable[buffer->mode].size *
           (x + (y * buffer->size.width));
}

leColor lePixelBufferGet(const lePixelBuffer* const buffer,
                         uint32_t x,
                         uint32_t y)
{
    if(buffer == NULL || x * y >= buffer->pixel_count)
        return 0x0;
        
    return lePixelBufferGet_Unsafe(buffer, x, y);
}

leColor lePixelBufferGet_Unsafe(const lePixelBuffer* const buffer,
                                uint32_t x,
                                uint32_t y)
{
    uint8_t* offs_ptr;
    leColor color = 0;
    
    offs_ptr = (uint8_t*)lePixelBufferOffsetGet(buffer, x, y);
    
    color = pixelGet_FnTable[leColorInfoTable[buffer->mode].size - 1](offs_ptr);
            
    return color;
}

leResult lePixelBufferClipRect(const lePixelBuffer* const buffer,
                               const leRect* const rect,
                               leRect* result)
{
    leRect buffer_rect;

    if(buffer == NULL || rect == NULL || result == NULL)
        return LE_FAILURE;

    buffer_rect.x = 0;
    buffer_rect.y = 0;
    buffer_rect.width = buffer->size.width;
    buffer_rect.height = buffer->size.height;

    // can't clip if rectangles do not overlap in any way
    if(leRectIntersects(&buffer_rect, rect) == LE_FALSE)
        return LE_FAILURE;

    // clip the rectangles
    leRectClip(&buffer_rect, rect, result);

    return LE_SUCCESS;
}

leColor lePixelBufferGetIndex(const lePixelBuffer* const buffer,
                              const uint32_t idx)
{
    if(idx >= buffer->pixel_count)
        return 0;
        
    return indexGet_FnTable[leColorInfoTable[buffer->mode].bppOrdinal](buffer, idx);
}

leColor lePixelBufferGetIndex_Unsafe(const lePixelBuffer* const buffer,
                                     const uint32_t idx)
{       
    return indexGet_FnTable[leColorInfoTable[buffer->mode].bppOrdinal](buffer, idx);
}

leResult lePixelBufferSet(const lePixelBuffer* const buffer,
                          uint32_t x,
                          uint32_t y,
                          leColor color)
{
    // cull the pixel if it falls outside the draw area
    if(x * y >= buffer->pixel_count)
        return LE_FAILURE;
        
    // set the buffer pixel value
    lePixelBufferSet_Unsafe(buffer, x, y, color);

    return LE_SUCCESS;
}

leResult lePixelBufferSet_Unsafe(const lePixelBuffer* const buffer,
                                 uint32_t x,
                                 uint32_t y,
                                 leColor color)
{
    uint8_t* dest_ptr;
    
    LE_ASSERT(x * y < buffer->pixel_count);

    // get address of dest pixel
    dest_ptr = (uint8_t*)lePixelBufferOffsetGet_Unsafe(buffer, x, y);
    
    pixelSet_FnTable[(leColorInfoTable[buffer->mode].bpp >> 3) - 1](dest_ptr, color);

    return LE_SUCCESS;
}

leResult lePixelBufferAreaFill(const lePixelBuffer* const buffer,
                               uint32_t x,
                               uint32_t y,
                               uint32_t w,
                               uint32_t h,
                               leColor color)
{
    leRect rect, clip_rect;

    rect.x = x;
    rect.y = y;
    rect.width = w;
    rect.height = h;

    if(buffer == NULL)
        return LE_FAILURE;

    if(lePixelBufferClipRect(buffer,
                             &rect,
                             &clip_rect) == LE_FAILURE)
        return LE_FAILURE;

    // fill the buffer
    return lePixelBufferAreaFill_Unsafe(buffer,
                                        clip_rect.x,
                                        clip_rect.y,
                                        clip_rect.width,
                                        clip_rect.height,
                                        color);
}

leResult lePixelBufferAreaFill_Unsafe(const lePixelBuffer* const buffer,
                                      uint32_t x,
                                      uint32_t y,
                                      uint32_t w,
                                      uint32_t h,
                                      leColor color)
{
    int32_t row, row_max, col, col_max;
    uint32_t drawX;
    uint32_t rowSize;
    void *srcIdx, *destIdx;

    // calculate minimums
    row_max = h;
    col_max = w;

    rowSize = col_max * leColorInfoTable[buffer->mode].size;

    // fill the first row
    for(col = 0; col < col_max; col++)
    {
        drawX = x + col;

        // set dest pixel
        lePixelBufferSet_Unsafe(buffer, drawX, y, color);
    }

    srcIdx = lePixelBufferOffsetGet_Unsafe(buffer, x, y);

    // copy first row to subsequent rows
    for(row = 1; row < row_max; row++)
    {
        destIdx = lePixelBufferOffsetGet_Unsafe(buffer, x, y + row);
        //idx = lePixelBufferOffsetGet_Unsafe(buffer, x, y + row);

        //idx = (uint8_t*)(((uint32_t)buffer->pixels) + ((y + row) * rowSize));

        memcpy(destIdx, srcIdx, rowSize);
    }

    return LE_SUCCESS;
}

leResult lePixelBufferCopy(lePixelBuffer* dest,
                           uint32_t x,
                           uint32_t y,
                           const lePixelBuffer* src,
                           const leRect* srcRect)
{
    int32_t row, col;
    leRect bufferRect, clippedRect;
    leColor color;

    bufferRect.x = 0;
    bufferRect.y = 0;
    bufferRect.width = src->size.width;
    bufferRect.height = src->size.height;

    leRectClip(&bufferRect, srcRect, &clippedRect);

    for(row = 0; row < clippedRect.height; ++row)
    {
        for(col = 0; col < clippedRect.width; ++col)
        {
            color = lePixelBufferGet(src, srcRect->x + col, srcRect->y + row);

            color = leColorConvert(src->mode, dest->mode, color);

            lePixelBufferSet_Unsafe(dest, x + col, y + row, color);
        }
    }

    return LE_SUCCESS;
}