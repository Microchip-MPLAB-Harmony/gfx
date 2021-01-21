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

/********************************************************************************
  GFX GFX2D Driver Functions

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx2d.c

  Summary:
    Source code for the GFX GFX2D driver static implementation.

  Description:
    This file contains the source code for the static implementation of the
    GFX GFX2D driver.
*******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "gfx/driver/processor/gfx2d/drv_gfx2d.h"
#include "definitions.h"
#include "system/time/sys_time.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
<#if gfx_hal_le??>

<#assign Val_Width = gfx_hal_le.DisplayWidth>
<#assign Val_Height = gfx_hal_le.DisplayHeight>

<#else>

<#assign Val_Width = le_gfx_lcdc.DisplayWidth>
<#assign Val_Height = le_gfx_lcdc.DisplayHeight>

</#if>

#define DISPLAY_WIDTH  ${Val_Width}
#define DISPLAY_HEIGHT ${Val_Height}
uint32_t  __attribute__ ((aligned (64))) blitbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT] ;
uint32_t  __attribute__ ((aligned (64))) maskbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT] ;

GFX2D_PIXEL_FORMAT gfx2dFormats[GFX_COLOR_MODE_LAST + 1] =
{
    -1, // GFX_COLOR_MODE_GS_8
    -1, // GFX_COLOR_MODE_RGB_332,
    GFX2D_RGB16, // GFX_COLOR_MODE_RGB_565
    -1, // GFX_COLOR_MODE_RGBA_5551
    -1, // GFX_COLOR_MODE_RGB_888
    GFX2D_ARGB32, // GFX_COLOR_MODE_RGBA_8888
    -1, // GFX_COLOR_MODE_ARGB_8888
    -1, // GFX_COLOR_MODE_INDEX_1
    -1, // GFX_COLOR_MODE_INDEX_4
    GFX2D_IDX8, //GFX_COLOR_MODE_INDEX_8
};

static  gfxBlend blendState = GFX_BLEND_NONE;

/* Indicate end of execute instruction */
volatile uint8_t gpu_end = 0;

void _IntHandler(uintptr_t context)
{
    gpu_end = 1;
}

/**** End Hardware Abstraction Interfaces ****/


void DRV_GFX2D_Initialize()
{
    // call the plib initialization routines
    PLIB_GFX2D_Initialize();
    PLIB_GFX2D_Enable();
    PLIB_GFX2D_IRQ_CallbackRegister(_IntHandler, 0);
}

gfxResult DRV_GFX2D_Fill(gfxPixelBuffer* dest,
                           const gfxRect* clipRect,
                           const gfxColor color)
{
    GFX2D_BUFFER    dest_buffer;
    GFX2D_RECTANGLE dest_rect;

    dest_buffer.width = dest->size.width;
    dest_buffer.height = dest->size.height;
    dest_buffer.format = gfx2dFormats[dest->mode];
    dest_buffer.dir = GFX2D_XY00;
    dest_buffer.addr = (uint32_t)dest->pixels;

    dest_rect.x = (uint32_t)clipRect->x;
    dest_rect.y = (uint32_t)clipRect->y;
    dest_rect.width = (uint32_t)clipRect->width;
    dest_rect.height = (uint32_t)clipRect->height;

    PLIB_GFX2D_Fill(&dest_buffer, &dest_rect, color);

    /* Wait for instruction to complete */
    while ( PLIB_GFX2D_GetGlobalStatusBusy() == true ) ;
    //while (gpu_end == 0) {
    //};
    
    return GFX_SUCCESS;
}

gfxResult DRV_GFX2D_Blit(const gfxPixelBuffer* source,
                           const gfxRect* srcRect,
                           const gfxPixelBuffer* dest,
                        const gfxRect* destRect)
{
    GFX2D_BUFFER    dest_buffer;
    GFX2D_RECTANGLE dest_rect;
    GFX2D_BUFFER    src_buffer;
    GFX2D_RECTANGLE src_rect;
    
    dest_buffer.width = dest->size.width;
    dest_buffer.height = dest->size.height;
    dest_buffer.format = gfx2dFormats[dest->mode];
    dest_buffer.dir = GFX2D_XY00;
    dest_buffer.addr = (uint32_t)dest->pixels;

    dest_rect.x = (uint32_t)destRect->x;
    dest_rect.y = (uint32_t)destRect->y;
    dest_rect.width = (uint32_t)destRect->width;
    dest_rect.height = (uint32_t)destRect->height;

    src_buffer.width = source->size.width;
    src_buffer.height = source->size.height;
    src_buffer.format = gfx2dFormats[source->mode];
    src_buffer.dir = GFX2D_XY00;
    src_buffer.addr = (uint32_t)source->pixels;

    src_rect.x = (uint32_t)srcRect->x;
    src_rect.y = (uint32_t)srcRect->y;
    src_rect.width = (uint32_t)srcRect->width;
    src_rect.height = (uint32_t)srcRect->height;

    if ( blendState == GFX_BLEND_NONE )
    {
        PLIB_GFX2D_Copy(&dest_buffer, &dest_rect, &src_buffer, &src_rect);
    }
    else
    {
        GFX2D_BLEND blend;

        blend.spe=GFX2D_SPE_MULTIPLY;
        blend.func= GFX2D_FUNCTION_SPE;
        blend.dfact= GFX2D_FACTOR_DST_COLOR;
        blend.sfact= GFX2D_FACTOR_SRC_COLOR;

        DRV_GFX2D_Blend(&dest_buffer, (GFX2D_RECTANGLE*)&dest_rect, &dest_buffer,
            (GFX2D_RECTANGLE*)&dest_rect, &src_buffer, (GFX2D_RECTANGLE*)&src_rect,
            blend);
    }

    /* Wait for instruction to complete */
    while ( PLIB_GFX2D_GetGlobalStatusBusy() == true ) ;
    //while (gpu_end == 0) {
    //};

    return GFX_SUCCESS;
}

void  DRV_GFX2D_Blend(
    GFX2D_BUFFER *dest,
    GFX2D_RECTANGLE *dest_rect,
    GFX2D_BUFFER *src1,
    GFX2D_RECTANGLE *src1_rect,
    GFX2D_BUFFER *src2,
    GFX2D_RECTANGLE *src2_rect,
    GFX2D_BLEND blend)
{
    PLIB_GFX2D_Blend(dest, dest_rect, src1, src1_rect, src2, src2_rect, blend);

    /* Wait for instruction to complete */
    while ( PLIB_GFX2D_GetGlobalStatusBusy() == true ) ;
    //while (gpu_end == 0) {
    //};
}

void  DRV_GFX2D_Rop(
   GFX2D_BUFFER *dest, 
   GFX2D_RECTANGLE *dest_rect, 
   GFX2D_BUFFER *src1,      
   GFX2D_RECTANGLE *src1_rect, 
   GFX2D_BUFFER *src2, 
   GFX2D_RECTANGLE *src2_rect,
   GFX2D_BUFFER *pmask, 
   GFX2D_ROP rop)
{
    PLIB_GFX2D_Rop(dest, dest_rect, src1, src1_rect, src2, src2_rect, pmask, rop);

    /* Wait for instruction to complete */
    while ( PLIB_GFX2D_GetGlobalStatusBusy() == true ) ;
    //while (gpu_end == 0) {
    //};
}

gfxResult DRV_GFX2D_SetBlend(
                const gfxBlend blend)
{
    blendState = blend;

    return GFX_SUCCESS;
}

gfxResult DRV_GFX2D_SetPalette(
                        uint32_t index_count,
                        gfxBuffer color_table,
                        gfxBool color_convert)
{

    return GFX_SUCCESS;
}

gfxResult DRV_GFX2D_SetTransparency(
                        gfxTransparency transparency,
                        gfxColor color,
                        uint32_t foreground_rop,
                        uint32_t background_rop)
{

    return GFX_SUCCESS;
}
/*******************************************************************************
 End of File
*/
