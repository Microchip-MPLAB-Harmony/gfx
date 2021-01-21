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

// GPU command buffer seems to work better when placed in DDR
#define CMD_BUFFER_DDR_ADDRESS 0xA9E00000

n2d_module_parameters_t gpu_params;
n2d_orientation_t gpu_orientation = N2D_0;

static n2d_buffer_format_t n2dFormats[GFX_COLOR_MODE_LAST + 1] =
{
    -1, // GFX_COLOR_MODE_GS_8
    -1, // GFX_COLOR_MODE_RGB_332,
    N2D_RGB565, // GFX_COLOR_MODE_RGB_565
    -1, // GFX_COLOR_MODE_RGBA_5551
    -1, // GFX_COLOR_MODE_RGB_888
    N2D_RGBA8888, // GFX_COLOR_MODE_RGBA_8888
    -1, // GFX_COLOR_MODE_ARGB_8888
    -1, // GFX_COLOR_MODE_INDEX_1
    -1, // GFX_COLOR_MODE_INDEX_4
    -1, //GFX_COLOR_MODE_INDEX_8
};

static  gfxBlend blendState = GFX_BLEND_NONE;

// *****************************************************************************
// *****************************************************************************
// Section: GFX Driver Nano2d Client Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    n2d_error_t n2d_blit()

   Summary:
    Copy a source buffer to the the destination buffer

   Description:
    The specified region of the source buffer is copied to the specified region
    of the destination buffer. If the regions are different in size, simple low-quality
    scaling will automatically be performed.

    An optional blend mode can be specified that defines the blending of the
    source onto the destination.

   Precondition:

   Parameters:
    destination           - Pointer to a n2d_buffer_t structure that describes the destination of the
                            blit
    destination_rectangle - Optional pointer to the rectangle that defines the region inside the
                            destination buffer. If this rectangle is not specified, the entire destination
                            buffer is used as the destination region
    source                - Pointer to a n2d_buffer_t structure that describes the source of the blit
    source_rectangle      - Optional pointer to the rectangle that defines the region inside the source buffer.
                            If this rectangle is not specified, the entire source buffer is used as the source
                            region
    blend                 - Optional blending mode to be applied to each pixel. If no blending is required, set this
                            value to N2D_BLEND_NONE (0)

  Returns:
    Returns the status as defined by n2d_error_t

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous.
  */
n2d_error_t n2d_blit(
    n2d_buffer_t *destination,
    n2d_rectangle_t *destination_rectangle,
    n2d_buffer_t *source,
    n2d_rectangle_t *source_rectangle,
    n2d_blend_t blend);

// *****************************************************************************
/* Function:
    n2d_error_t n2d_blit_async()

   Summary:
    Copy a source buffer to the the destination buffer

   Description:
    The specified region of the source buffer is copied to the specified region
    of the destination buffer. If the regions are different in size, simple low-quality
    scaling will automatically be performed.

    An optional blend mode can be specified that defines the blending of the
    source onto the destination.

   Precondition:

   Parameters:
    destination           - Pointer to a n2d_buffer_t structure that describes the destination of the
                            blit
    destination_rectangle - Optional pointer to the rectangle that defines the region inside the
                            destination buffer. If this rectangle is not specified, the entire destination
                            buffer is used as the destination region
    source                - Pointer to a n2d_buffer_t structure that describes the source of the blit
    source_rectangle      - Optional pointer to the rectangle that defines the region inside the source buffer.
                            If this rectangle is not specified, the entire source buffer is used as the source
                            region
    blend                 - Optional blending mode to be applied to each pixel. If no blending is required, set this
                            value to N2D_BLEND_NONE (0)

  Returns:
    Returns GENERIC_IO as defined by n2d_error_t if the GPU does not immediately return acknowledgement status as defined by n2d_error_t

  Remarks:
    This function will not wait until the hardware is complete, i.e. it is asynchronous.
  */
n2d_error_t n2d_blit_async(
    n2d_buffer_t *destination,
    n2d_rectangle_t *destination_rectangle,
    n2d_buffer_t *source,
    n2d_rectangle_t *source_rectangle,
    n2d_blend_t blend);
// *****************************************************************************
/* Function:
    n2d_error_t n2d_dither()

   Summary:
    Enable or disable dithering

   Description:
    Sets the capability to scatter or approximate colors when using less than 32bpp or
        16bpp color depth. Dither attempts to improve the overall appearance of low resolution
        images. Dithering is on when enable is true, otherwise, dithering is off.

   Precondition:

   Parameters:
    enable                - defines whether dither is set on or off.

  Returns:
    Returns the status as defined by n2d_error_t

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous.
  */
n2d_error_t n2d_dither(
    n2d_bool_t enable);

// *****************************************************************************
/* Function:
    n2d_error_t n2d_draw_state()

   Summary:
    Set the drawing state for any following Nano2D API draw call

   Description:
    In order to setup transparency for the n2d_blit function, this function needs to
    be called. Note that this function is static, so set once, all draw commands that
    follow this function will take this transparency into effect. Call this function
    again with different parameters to set another transparency mode or turn transparency off.

    It will return N2D_INVALID_ARGUMENT if the source defines the transparency but the
    rop has nothing to do with the source buffer.

    The default transparency mode for any newly opened context is N2D_TRANSPARENCY_NONE,
    using a foreground_rop of 0xC (copy source).

    Binary ROPs supported in both foreground and background operations:
        ROP 	Formula 	Description
        0x0 	0 	Set all destination bits to 0.
        0x1 	~(D|S) 	Inverse of merge source and destination.
        0x2 	D&~S 	Mask inverse of source and destination.
        0x3 	~S 	Copy inverse of source.
        0x4 	S&~D 	Mask source and inverse of destination.
        0x5 	~D 	Invert destination.
        0x6 	D^S 	Exclusive or of source and destination.
        0x7 	~(D&S) 	Inverse of mask source and destination.
        0x8 	D&S 	Mask source and destination.
        0x9 	~(D^S) 	Inverse of exclusive or of source and destination.
        0xA 	D 	Copy destination.
        0xB 	D|~S 	Merge inverse of source and destination.
        0xC 	S 	Copy source.
        0xD 	S|~D 	Merge source and inverse of destination.
        0xE 	D|S 	Merge source and destination.
        0xF 	1 	Set all destination bits to 1.

   Precondition:

   Parameters:
    transparency   - The transparency mode applied to each pixel. See n2d_transparency_t for a list of
                     all supported transparency modes
    color          - If transparency is not N2D_TRANSPARENCY_NONE, this color value specifies if a
                     pixel is a foreground or a background pixel. If the color matches, it is a background pixel,
                     otherwise it is a foreground pixel
    foreground_rop - A Binary ROP (ROP2) code that gets executed by the hardware for each foreground pixel
    background_rop - A Binary ROP (ROP2) code that gets executed by the hardware for each background pixel

  Returns:
    Returns the status as defined by n2d_error_t

  Remarks:
    When using a source buffer with the A8 pixel format, transparency must be enabled to N2D_TRANSPARENCY_SOURCE and the alpha channel of color will be used to check for transparency. If the pixel is not transparent, the RGB channels of color value will be used as the color for the pixel.

*/
n2d_error_t n2d_draw_state(
    n2d_transparency_t transparency,
    n2d_color_t color,
    n2d_uint8_t foreground_rop,
    n2d_uint8_t background_rop);

// *****************************************************************************
/* Function:
    n2d_error_t n2d_set_global_alpha()

   Summary:
    Set the global alpha value for source and destination surfaces.

   Description:
    Sets the alpha blend mode and alpha value for subsequent source and
    destination buffers.

   Precondition:

   Parameters:
    srcGlobalAlpha      - The mode applied to the source buffer. See:
                          n2d_global_alpha for a list of all supported
                          alpha modes
    dstGlobalAlpha      - The mode applied to the destination buffer. See:
                          n2d_global_alpha for a list of all supported
                          alpha modes
    srcGlobalAlphaValue - A value from 0 - 255 to set as the global alpha channel
                          of the source buffer.
    dstGlobalAlphaValue - A value from 0 - 255 to set as the global alpha channel of
                          the destination buffer.

  Returns:
    Returns the status as defined by n2d_error_t

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous
*/
n2d_error_t n2d_set_global_alpha(
    enum n2d_global_alpha    srcGlobalAlpha,
    enum n2d_global_alpha    dstGlobalAlpha,
    n2d_uint32_t             srcGlobalAlphaValue,
    n2d_uint32_t             dstGlobalAlphaValue
    );


// *****************************************************************************
/* Function:
    n2d_error_t n2d_load_palette()

   Summary:
    Load 256-entry color table for INDEX8 source surfaces.

   Description:
    Loads color_table into the GPU peripheral at the given starting address
    first_index. The number of indices to load is given by index_count. A
    conversion is performed if convert_color is set to true, other no conversion
    is performed.

   Precondition:

   Parameters:
        first_index  -  The index to start loading from (0..255).

    index_count  -  The number of indices to load (first_index + index_count <= 256).

    color_table  -  Pointer to the color table to load. The value of the pointer
                    should be set to the first value to load no matter what the
                    value of FirstIndex is. The table must consist of 32-bit
                    entries that contain color values in either ARGB8 or the
                    destination color format (see color_convert).

    color_convert - If set to N2D_TRUE, the 32-bit values in the table are
                    assumed to be in ARGB8 format and will be converted by the
                    hardware to the destination format as needed. If set to
                    N2D_FALSE, the 32-bit values in the table are assumed to be
                    preconverted to the destination format.

  Returns:
    Returns the status as defined by n2d_error_t

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous
*/
n2d_error_t n2d_load_palette(
        n2d_uint32_t first_index,
        n2d_uint32_t index_count,
        n2d_pointer color_table,
        n2d_bool_t color_convert);

// *****************************************************************************
/* Function:
    n2d_error_t n2d_fill()

   Summary:
    Fill a (partial) buffer with a specified color

   Description:
    Draws and fills a rectangle with a specific color onto destination buffer.

    An optional blend mode can be specified that defines the blending of the
    color onto the destination.

   Precondition:

   Parameters:
    destination   - Pointer to a n2d_buffer_t structure that describes the buffer to be filled
    rectangle     - Pointer to a rectangle that specifies the area to be filled. If rectangle is NULL,
                    the entire buffer will be filled with the specified color
    color         - The color value to use for filling the buffer
    blend         - The blending mode to be applied to each pixel. If no blending is required, set this
                    value to N2D_BLEND_NONE (0)

  Returns:
    Returns the status as defined by n2d_error_t

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous
*/
n2d_error_t n2d_fill(
    n2d_buffer_t *destination,
    n2d_rectangle_t *rectangle,
    n2d_color_t color,
    n2d_blend_t blend);

// *****************************************************************************
/* Function:
    n2d_error_t n2d_line()

   Summary:
    Draw a line

   Description:
    Draw a line with a specific color. The last pixel of the line will not be drawn.

    An optional blend mode can be specified that defines the blending of the
    color onto the destination.

   Precondition:

   Parameters:
    destination  - Pointer to a n2d_buffer_t structure that describes the buffer to be
                    used to draw the line into.
    start        - The starting point of the line, given in destination coordinates.
    end          - The ending point of the line, given in destination coordinates. The last
                   pixel will not be drawn.
    clip         - Optional pointer to a rectangle that specifies the clipping region of
                   the destination. If clip is NULL, the clipping region will be the entire
                   destination buffer.
    color        - The color value to use for drawing the line.
    blend        - The blending mode to be applied to each pixel on the line. If no
                   blending is required, set this value to N2D_BLEND_NONE (0).

  Returns:
    Returns the status as defined by n2d_error_t

  Remarks:
    This function will wait until the hardware is complete, i.e. it is synchronous
*/
n2d_error_t n2d_line(
    n2d_buffer_t *destination,
    n2d_point_t start,
    n2d_point_t end,
    n2d_rectangle_t *clip,
    n2d_color_t color,
    n2d_blend_t blend);

// *****************************************************************************
/* Function:
    n2d_error_t n2d_open()

   Summary:
    Open Nano2d context

   Description:
    The n2d_line, n2d_fill, n2d_blit, and n2d_draw_state functions require
    a Nano2D context to be opened. This function is the first interface that
    accesses the hardware. The hardware will be turned on and initialized.

   Precondition:

   Parameters:

   Returns:
    Returns the status as defined by n2d_error_t.

   Remarks:
    There is only one Nano2d context per application, so this function must
    be called once in your application.
*/
n2d_error_t n2d_open(
    void);

n2d_error_t n2d_init_hardware(
    n2d_module_parameters_t *params
    );


gfxResult DRV_2DGPU_Line(gfxPixelBuffer * dest,
                         const gfxPoint* p1,
                           const gfxPoint* p2,
                           const gfxRect* clipRect,
                           const gfxColor color)
{
    n2d_buffer_t dest_buffer;
    n2d_point_t np1, np2;

        // trivial cases that are better handled by the CPU
    if(p1->x == p2->x || p1->y == p2->y)
        return GFX_FAILURE;

    dest_buffer.width = dest->size.width;
    dest_buffer.height = dest->size.height;
    dest_buffer.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    dest_buffer.format = n2dFormats[dest->mode];
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
             blendState);

    return GFX_SUCCESS;
}

gfxResult DRV_2DGPU_Fill(gfxPixelBuffer * dest,
                           const gfxRect* clipRect,
                           const gfxColor color)
{
    n2d_buffer_t dest_buffer;

    dest_buffer.width = dest->size.width;
    dest_buffer.height = dest->size.height;
    dest_buffer.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    dest_buffer.format = n2dFormats[dest->mode];
    dest_buffer.orientation = gpu_orientation;
    dest_buffer.handle = NULL;
    dest_buffer.memory = dest->pixels;
    dest_buffer.gpu = KVA_TO_PA(dest->pixels);

    n2d_fill(&dest_buffer,
             (n2d_rectangle_t*)clipRect,
             gfxColorConvert(dest->mode, GFX_COLOR_MODE_ARGB_8888, color),
             blendState);

    return GFX_SUCCESS;
}

gfxResult DRV_2DGPU_Blit(const gfxPixelBuffer* source,
                           const gfxRect* srcRect,
                           const gfxPixelBuffer* dest,
                         const gfxRect* destRect)
{
    n2d_error_t err;
    n2d_buffer_t src_buffer, dest_buffer;

    // craft source buffer descriptor
    src_buffer.width = source->size.width;
    src_buffer.height = source->size.height;
    src_buffer.stride = source->size.width * gfxColorInfoTable[source->mode].size;
    src_buffer.format = n2dFormats[source->mode];
    src_buffer.orientation = gpu_orientation;
    src_buffer.handle = NULL;
    src_buffer.memory = source->pixels;
    src_buffer.gpu = KVA_TO_PA(source->pixels);

    if(IS_KVA1(source->pixels) == GFX_FALSE ||    //not coherent
       (source->size.width) % 4 > 0 ||            //not width aligned
       (((uint32_t) src_buffer.memory) & 0x3))    //not 32-bit address aligned
    {
        return GFX_FAILURE;
    }
    
    // craft dest buffer descriptor
    dest_buffer.width = dest->size.width;
    dest_buffer.height = dest->size.height;
    dest_buffer.stride = dest->size.width * gfxColorInfoTable[dest->mode].size;
    dest_buffer.format = n2dFormats[dest->mode];
    dest_buffer.orientation = gpu_orientation;
    dest_buffer.handle = NULL;
    dest_buffer.memory = dest->pixels;
    dest_buffer.gpu = KVA_TO_PA(dest->pixels);

    <#if DRV_NANO2D_MODE == "Synchronous">

    err = n2d_blit(&dest_buffer,
                  (n2d_rectangle_t*)destRect,
                  &src_buffer,
                  (n2d_rectangle_t*)srcRect,
                  blendState);
    <#else>

    err = n2d_blit_async(&dest_buffer,
                  (n2d_rectangle_t*)destRect,
                  &src_buffer,
                  (n2d_rectangle_t*)srcRect,
                  blendState);

    </#if>
        
    return ((err == N2D_SUCCESS) ? GFX_SUCCESS : GFX_FAILURE);
}

gfxResult DRV_2DGPU_SetBlend(
                const gfxBlend blend)
{
    blendState = blend;

    return GFX_SUCCESS;
}

gfxResult DRV_2DGPU_SetGlobalAlpha(
                        const gfxAlpha srcGlobalAlpha,
                        const gfxAlpha dstGlobalAlpha,
                        uint32_t srcGlobalAlphaValue,
                        uint32_t dstGlobalAlphaValue)
{

    if (N2D_IS_ERROR( n2d_set_global_alpha(srcGlobalAlpha, dstGlobalAlpha,
                              srcGlobalAlphaValue, dstGlobalAlphaValue) ) )
    {
        return GFX_FAILURE;
    }

    return GFX_SUCCESS;
}

gfxResult DRV_2DGPU_SetPalette(
                        uint32_t index_count,
                        gfxBuffer color_table,
                        gfxBool color_convert)
{
    if (N2D_IS_ERROR( n2d_load_palette(0, index_count,
                                       color_table, color_convert) ) )
    {
         return GFX_FAILURE;
    }

    return GFX_SUCCESS;
}

gfxResult DRV_2DGPU_SetTransparency(
                        gfxTransparency transparency,
                        gfxColor color,
                        uint32_t foreground_rop,
                        uint32_t background_rop)
{
    if (N2D_IS_ERROR(n2d_draw_state(transparency, color,
                                    foreground_rop, background_rop) ) )
    {
         return GFX_FAILURE;
    }

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
