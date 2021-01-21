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

/*******************************************************************************
  MPLAB Harmony Nano2D Generated HAL Wrapper

  File Name:
    libnano2D_hal.c

  Summary:
    Build-time generated implementation for the Nano2D Generated HAL Wrapper.

  Description:
    Build-time generated implementation for the Nano2D Generated HAL Wrapper.

    Created with MPLAB Harmony Version 2.03
*******************************************************************************/

#include "gfx/driver/processor/2dgpu/libnano2d.h"

#include <xc.h>
#include <sys/attribs.h>
#include <sys/kmem.h>

#define CMD_BUFFER_SIZE  8192
uint32_t __attribute__((coherent, aligned(32))) commandBuffer[CMD_BUFFER_SIZE];

#define SCRATCH_SIZE ${SCRATCHBUFFER_SIZE}

static volatile uint8_t __attribute__ ((coherent, aligned (32))) scratchBuffer[8192];

// GPU command buffer seems to work better when placed in DDR
#define CMD_BUFFER_DDR_ADDRESS 0xA9E00000

n2d_module_parameters_t gpu_params;
n2d_buffer_format_t gpu_format = N2D_RGBA8888;
n2d_buffer_format_t gpu_blend = N2D_BLEND_SRC_OVER;
n2d_orientation_t gpu_orientation = N2D_0;

gfxResult DRV_2DGPU_Line(gfxPixelBuffer * dest,
                         const gfxPoint* p1,
                           const gfxPoint* p2,
                           const gfxRect* clipRect,
                           const gfxColor color,
                         const gfxBlend blend)
{
    n2d_buffer_t dest_buffer;
    n2d_point_t np1, np2;

        // trivial cases that are better handled by the CPU
    if(p1->x == p2->x || p1->y == p2->y)
        return GFX_FAILURE;

    dest_buffer.width = dest->size.width;
    dest_buffer.height = dest->size.height;
    dest_buffer.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    dest_buffer.format = gpu_format;
    dest_buffer.orientation = gpu_orientation;
    dest_buffer.handle = NULL;
    dest_buffer.memory = dest->pixels;
    dest_buffer.gpu = KVA_TO_PA(dest->pixels);

    np1.x = p1->x;
    np1.y = p1->y;

    np2.x = p2->x;
    np2.y = p2->y;

    // compensate for missing last pixel
    if(np1.x == np2.x) // vertical line
    {
        if(np1.y < np2.y)
            np2.y++;
        else
            np1.y--;
    }

    if(np1.y == np2.y) // horizontal line
    {
        if(np1.x < np2.x)
            np2.x++;
        else
            np1.x--;
    }

    n2d_line(&dest_buffer,
             np1,
             np2,
             (n2d_rectangle_t*)clipRect,
             gfxColorConvert(dest->mode, GFX_COLOR_MODE_ARGB_8888, color),
             blend);

    return GFX_SUCCESS;
}

gfxResult DRV_2DGPU_Fill(gfxPixelBuffer * dest,
                           const gfxRect* clipRect,
                           const gfxColor color,
                         const gfxBlend blend)
{
    n2d_buffer_t dest_buffer;

    dest_buffer.width = dest->size.width;
    dest_buffer.height = dest->size.height;
    dest_buffer.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    dest_buffer.format = gpu_format;
    dest_buffer.orientation = gpu_orientation;
    dest_buffer.handle = NULL;
    dest_buffer.memory = dest->pixels;
    dest_buffer.gpu = KVA_TO_PA(dest->pixels);

    n2d_fill(&dest_buffer,
             (n2d_rectangle_t*)clipRect,
             gfxColorConvert(dest->mode, GFX_COLOR_MODE_ARGB_8888, color),
             blend);

    return GFX_SUCCESS;
}



static gfxBool createPaddedBuffer(const gfxPixelBuffer* src)
{
    int pixelSize = gfxColorInfoTable[src->mode].size;
    int rowSize = src->size.width;
    int newRowSize = rowSize + (4 - (src->size.width % 4));
    int totalSize = newRowSize * src->size.height * pixelSize;
    int row;
    
    if(totalSize > SCRATCH_SIZE)
        return GFX_FALSE;
    
    memset((uint8_t*)scratchBuffer, 0, SCRATCH_SIZE);

    for(row = 0; row < src->size.height; ++row)
    {
        memcpy(((uint8_t*)scratchBuffer) + (newRowSize * row * pixelSize),
               ((uint8_t*)src->pixels) + (rowSize * row * pixelSize),
               rowSize * pixelSize);
    }
    
    return GFX_TRUE;
}

gfxResult DRV_2DGPU_Blit(const gfxPixelBuffer* source,
                         const gfxRect* srcRect,
                         const gfxPixelBuffer* dest,
                         const gfxRect* destRect,
                         const gfxBlend blend)
{
    n2d_buffer_t src_buffer, dest_buffer;

    // craft source buffer descriptor
    src_buffer.width = source->size.width;
    src_buffer.height = source->size.height;
    src_buffer.stride = source->size.width * gfxColorInfoTable[source->mode].size;
    src_buffer.format = gpu_format;
    src_buffer.orientation = gpu_orientation;
    src_buffer.handle = NULL;
    src_buffer.memory = source->pixels;
    src_buffer.gpu = KVA_TO_PA(source->pixels);

    // the source address must reside in KSEG1 (cache coherent) memory
    // and the source buffer must be padded properly
    if(IS_KVA1(source->pixels) == GFX_FALSE || 
       (source->size.width) % 4 > 0)
    {
        if(createPaddedBuffer(source) == GFX_FALSE)
            return GFX_FAILURE;
        
        src_buffer.width = source->size.width + (4 - (source->size.width % 4));
        src_buffer.stride = (src_buffer.width * gfxColorInfoTable[source->mode].size);
        src_buffer.memory = (uint8_t*)scratchBuffer;
        src_buffer.gpu = KVA_TO_PA((uint8_t*)scratchBuffer);
    }
    
    // craft dest buffer descriptor
    dest_buffer.width = dest->size.width;
    dest_buffer.height = dest->size.height;
    dest_buffer.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    dest_buffer.format = gpu_format;
    dest_buffer.orientation = gpu_orientation;
    dest_buffer.handle = NULL;
    dest_buffer.memory = dest->pixels;
    dest_buffer.gpu = KVA_TO_PA(dest->pixels);

    n2d_blit(&dest_buffer,
             (n2d_rectangle_t*)destRect,
             &src_buffer,
             (n2d_rectangle_t*)srcRect,
             blend);

    return GFX_SUCCESS;
}

gfxResult DRV_2DGPU_BlitStretch(const gfxPixelBuffer* source,
                                const gfxRect* srcRect,
                                const gfxPixelBuffer* dest,
                                const gfxRect* destRect,
                                const gfxBlend blend)
{
    n2d_buffer_t src_buffer, dest_buffer;

    // craft source buffer descriptor
    src_buffer.width = source->size.width;
    src_buffer.height = source->size.height;
    src_buffer.stride = source->size.width * gfxColorInfoTable[dest->mode].size;
    src_buffer.format = gpu_format;
    src_buffer.orientation = gpu_orientation;
    src_buffer.handle = NULL;
    src_buffer.memory = source->pixels;
    src_buffer.gpu = KVA_TO_PA(source->pixels);

    // the source address must reside in KSEG1 (cache coherent) memory
    // and the source buffer must be padded properly
    if(IS_KVA1(source->pixels) == GFX_FALSE || 
       (source->size.width) % 4 > 0)
    {
        if(createPaddedBuffer(source) == GFX_FALSE)
            return GFX_FAILURE;
        
        src_buffer.width = source->size.width + (4 - (source->size.width % 4));
        src_buffer.stride = (src_buffer.width * gfxColorInfoTable[source->mode].size);
        src_buffer.memory = (uint8_t*)scratchBuffer;
        src_buffer.gpu = KVA_TO_PA((uint8_t*)scratchBuffer);
    }
    
    // craft dest buffer descriptor
    dest_buffer.width = dest->size.width;
    dest_buffer.height = dest->size.height;
    dest_buffer.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    dest_buffer.format = gpu_format;
    dest_buffer.orientation = gpu_orientation;
    dest_buffer.handle = NULL;
    dest_buffer.memory = dest->pixels;
    dest_buffer.gpu = KVA_TO_PA(dest->pixels);

    n2d_blit(&dest_buffer,
             (n2d_rectangle_t*)destRect,
             &src_buffer,
             (n2d_rectangle_t*)srcRect,
             blend);

    return GFX_SUCCESS;
}

// function that initialized the driver context
void DRV_2DGPU_Initialize(void)
{
    /* initialize gpu hardware and peripheral software */
    memset((void*)commandBuffer, 0, CMD_BUFFER_SIZE);

    gpu_params.contiguousBase = (n2d_uint32_t)commandBuffer;
    gpu_params.registerMemBase2D = 0xBF8EB000;
    gpu_params.baseAddress = 0;

    n2d_init_hardware(&gpu_params);

    /* open and initialize gpu software  */
    if (N2D_IS_ERROR(n2d_open()))
        return ;
}
