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
 Module for Microchip Graphics Library - Graphics Driver Layer

  Company:
    Microchip Technology Inc.

  File Name:
    libnano2d.h

  Summary:
    Main header file for MPLAB Harmony Graphics Driver libnano2D GPU functions

  Description:
    The API functions to be used for the Nano2D graphics accelerator.

*******************************************************************************/

/****************************************************************************
*
*    Copyright (c) 2005 - 2014 by Vivante Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Vivante Corporation. This is proprietary information owned by
*    Vivante Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Vivante Corporation.
*
*****************************************************************************/

/** \file libnano2d.h
 * @brief 2DGPU definitions and functions.
 *
 * @details This module implements 2DGPU driver interface.
 */

#ifndef _nano2d_h__
#define _nano2d_h__

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

#define IN
#define OUT

typedef int                 n2d_bool_t;
typedef unsigned char       n2d_uint8_t;
typedef short               n2d_int16_t;
typedef unsigned short      n2d_uint16_t;
typedef int                 n2d_int32_t;
typedef unsigned int        n2d_uint32_t;
typedef unsigned int        n2d_size_t;
typedef float               n2d_float_t;
typedef void*               n2d_pointer;

#ifdef __cplusplus
#define N2D_NULL 0
#else
#define N2D_NULL ((void *) 0)
#endif

#define N2D_TRUE  1
#define N2D_FALSE 0

#define N2D_INFINITE ((n2d_uint32_t) ~0U)

#define __gcmSTART(reg_field) \
    (0 ? reg_field)

#define __gcmEND(reg_field) \
    (1 ? reg_field)

#define __gcmGETSIZE(reg_field) \
    (__gcmEND(reg_field) - __gcmSTART(reg_field) + 1)

#define __gcmALIGN(data, reg_field) \
    (((n2d_uint32_t) (data)) << __gcmSTART(reg_field))

#define __gcmMASK(reg_field) \
    ((n2d_uint32_t) ((__gcmGETSIZE(reg_field) == 32) \
        ?  ~0 \
        : (~(~0 << __gcmGETSIZE(reg_field)))))

#define gcmVERIFYFIELDVALUE(data, reg, field, value) \
( \
    (((n2d_uint32_t) (data)) >> __gcmSTART(reg##_##field) & \
                             __gcmMASK(reg##_##field)) \
        == \
    (reg##_##field##_##value & __gcmMASK(reg##_##field)) \
)

#define gcmSETFIELD(data, reg, field, value) \
( \
    (((n2d_uint32_t) (data)) \
        & ~__gcmALIGN(__gcmMASK(reg##_##field), reg##_##field)) \
        |  __gcmALIGN((n2d_uint32_t) (value) \
            & __gcmMASK(reg##_##field), reg##_##field) \
)

#define gcmGETFIELD(data, reg, field) \
( \
    ((((n2d_uint32_t) (data)) >> __gcmSTART(reg##_##field)) \
        & __gcmMASK(reg##_##field)) \
)

#define gcmSETFIELDVALUE(data, reg, field, value) \
( \
    (((n2d_uint32_t) (data)) \
        & ~__gcmALIGN(__gcmMASK(reg##_##field), reg##_##field)) \
        |  __gcmALIGN(reg##_##field##_##value \
            & __gcmMASK(reg##_##field), reg##_##field) \
)

#define gcmSETMASKEDFIELDVALUE(reg, field, value) \
( \
    gcmSETFIELDVALUE(~0, reg,          field, value) & \
    gcmSETFIELDVALUE(~0, reg, MASK_ ## field, ENABLED) \
)

#define gcmSETMASKEDFIELD(reg, field, value) \
( \
    gcmSETFIELD     (~0, reg,          field, value) & \
    gcmSETFIELDVALUE(~0, reg, MASK_ ## field, ENABLED) \
)

#define gcmALIGN(n, align) \
( \
    ((n) + ((align) - 1)) & ~((align) - 1) \
)

#define gcmMIN(x, y) \
( \
    ((x) <= (y)) \
        ? (x) \
        : (y) \
)

#define gcmMAX(x, y) \
( \
    ((x) >= (y)) \
        ? (x) \
        : (y) \
)

#define gcmINT2PTR(i) \
( \
    (void *)(n2d_uint32_t)(i) \
)

#define gcmPTR2INT(p) \
( \
    (n2d_uint32_t)(p) \
)

#define N2D_IS_SUCCESS(error) (error == N2D_SUCCESS)
#define N2D_IS_ERROR(error)   (error != N2D_SUCCESS)

#define N2D_ON_ERROR(func) \
    do \
    { \
        error = func; \
        if (N2D_IS_ERROR(error)) \
        { \
            goto on_error; \
        } \
    } \
    while (0)

#define gcmASSERT(exp)
#define gcmkASSERT(exp)
#define gcmkTRACE(...)

#ifndef gcmCOUNTOF
#   define gcmCOUNTOF(array) \
        (sizeof(array) / sizeof(array[0]))
#endif

/**
 * @brief This struct represents gpu parameters.
 * @details This is used describe a individual parameters used to
 * initialize the GPU module.
*/

typedef struct n2d_module_parameters
{
    n2d_int32_t     irqLine2D;         /**< command completion interrupt pin. */
    n2d_uint32_t    registerMemBase2D; /**< base address of gpu (physical address). */
    n2d_uint32_t    registerMemSize2D; /**< size of gpu address space. */
    n2d_uint32_t    contiguousSize;    /**< size of memory pool. */
    n2d_uint32_t    contiguousBase;    /**< start address of memory (virtual address). */
    n2d_uint32_t    baseAddress;       /**< base address display buffer. */
}
n2d_module_parameters_t;

/**
 * @brief This enum represents blending modes.
 * @details List of blending modes.
 * Some of the Nano2D API functions calls support blending. S and D
 * represent source and destination color channels and Sa and Da
 * represent the source and destination alpha channels.
*/
typedef enum n2d_blend
{
    N2D_BLEND_NONE,         /**< S, i.e. no blending. */
    N2D_BLEND_SRC_OVER,     /**< S + (1 - Sa) * D. */
    N2D_BLEND_DST_OVER,     /**< (1 - Da) * S + D. */
    N2D_BLEND_SRC_IN,       /**< Da * S. */
    N2D_BLEND_DST_IN,       /**< Sa * D. */
    N2D_BLEND_ADDITIVE,     /**< S + D. */
    N2D_BLEND_SUBTRACT,     /**< D * (1 - S). */
}
n2d_blend_t;

/**
 * @brief This enum represents buffer formats.
 * @details List of the following pixel types for the buffer structure.
 */
typedef enum n2d_buffer_format
{
    N2D_RGBA8888,   /**< 32-bit RGBA format with 8 bits per color channel. Red is
                    in bits 7:0, green in bits 15:8, blue in bits 23:16, and
                    the alpha channel is in bits 31:24. */
    N2D_BGRA8888,   /**< 32-bit RGBA format with 8 bits per color channel. Red is
                    in bits 23:16, green in bits 15:8, blue in bits 7:0, and
                     the alpha channel is in bits 31:24. */
    N2D_RGB565,     /**< 16-bit RGB format with 5 and 6 bits per color channel. Red
                    is in bits 4:0, green in bits 10:5, and the blue color
                     channel is in bits 15:11. */
    N2D_BGR565,     /**< 16-bit RGB format with 5 and 6 bits per color channel. Red
                    is in bits 15:11, green in bits 10:5, and the blue color
                     channel is in bits 4:0. */
    N2D_RGBA4444,   /**< 16-bit RGBA format with 4 bits per color channel. Red is
                    in bits 3:0, green in bits 7:4, blue in bits 11:8 and the
                     alpha channel is in bits 15:12. Note: currently not available in HAL. */
    N2D_BGRA4444,   /**< 16-bit RGBA format with 4 bits per color channel. Red is
                     in bits 3:0, green in bits 7:4, blue in bits 11:8 and the
                     alpha channel is in bits 15:12. Note: currently not available in HAL. */
    N2D_A8,         /**< 8-bit alpha format. There are no RGB values. */
    N2D_YUYV,
    N2D_UYVY,
    N2D_INDEX8,     /**< 8-bit index format. There are no RGB values. */
}
n2d_buffer_format_t;

/**
 * @brief This enum represents orientations.
 * @details List of of buffer orientations. Orientation is orthogonal.
 * Rotation which is not parallel to the x or y axis is not supported.
*/
typedef enum n2d_orientation
{
    N2D_0,      /**< Buffer is 0 degrees rotated. */
    N2D_90,     /**< Buffer is 90 degrees rotated. */
    N2D_180,    /**< Buffer is 180 degrees rotated. */
    N2D_270,    /**< Buffer is 270 degrees rotated. */
}
n2d_orientation_t;

/**
 * @brief This struct represents any image or render target.
 * @details Each piece of memory, whether it is an image used as a source
 * or a buffer used as a destination, requires a structure to define it. This
 * structure contains all the information the Nano2D API requires to access
 * the buffer's memory by the hardware
*/
typedef struct n2d_buffer
{
    n2d_int32_t width;              /**< Width of the buffer in pixels. */
    n2d_int32_t height;             /**< Height of the buffer in pixels. */
    n2d_int32_t stride;             /**< Stride of the buffer in bytes. */
    n2d_buffer_format_t format;     /**< Pixel format of the buffer. */
    n2d_orientation_t orientation;  /**< Buffer's orientation. */
    void *handle;                   /**< unused. */
    void *memory;                   /**< Logical pointer to the buffer's memory for the CPU. */
    n2d_uint32_t gpu;               /**< Logical pointer to the buffer's memory for the CPU. */
}
n2d_buffer_t;

/**
 * @brief This enum represents error codes.
 * @details List of possible error codes that functions return. All API
 * functions return a status code. On success, N2D_SUCCESS will be returned
 * when a function is successful. This value is set to zero, so if any
 * function returns a non-zero value, an error has occured.
*/
typedef enum n2d_error
{
    N2D_SUCCESS = 0,            /**< Success. */
    N2D_INVALID_ARGUMENT,       /**< An invalid argument was specified. */
    N2D_OUT_OF_MEMORY,          /**< Out of memorys. */
    N2D_NO_CONTEXT,             /**< No open context is present. */
    N2D_TIMEOUT,                /**< A timeout has accored during a wait. */
    N2D_OUT_OF_RESOURCES,       /**< Out of system resources. */
    N2D_GENERIC_IO,             /**< Cannot communicate with the kernel driver. */
    N2D_NOT_SUPPORTED,          /**< The request is not supported. */
}
n2d_error_t;

/**
 * @brief This struct represents a gpu point.
 * @details Defines a position on the screen.
*/
typedef struct n2d_point
{
    n2d_int32_t x;      /**< horizontal coordinate of the point. */
    n2d_int32_t y;      /**< vertical coordinate of the point. */
}
n2d_point_t;

/**
 * @brief This struct represents a gpu rectangle.
 * @details Defines a rectangular shape area of the screen.
*/
typedef struct n2d_rectangle
{
    n2d_int32_t x;      /**< Left coordinate of the rectangle. */
    n2d_int32_t y;      /**< Top coordinate of the rectangle. */
    n2d_int32_t width;  /**< Width of the rectangle. */
    n2d_int32_t height; /**< Height of the rectangle. */
}
n2d_rectangle_t;

/**
 * @brief This enum represents transparency modes.
 * @details The 2DGPU hardware can be programmed to use transparencyc color,
 * extracted from either the source or the destination
*/
typedef enum n2d_transparency
{
    N2D_TRANSPARENCY_NONE,          /**< No transparency. */
    N2D_TRANSPARENCY_SOURCE,        /**< The source defines the transparency. */
    N2D_TRANSPARENCY_DESTINATION,   /**< The destination defines the transparency. */
}
n2d_transparency_t;

/**
 * @brief This enum represents transparency modes.
 * @details The 2DGPU hardware can be programmed to use transparency color,
 * extracted from either the source or the destination
 */
typedef enum n2d_global_alpha
{
    N2D_GLOBAL_ALPHA_OFF,       /**< Disable global alpha. */
    N2D_GLOBAL_ALPHA_ON,        /**< Enable global alpha replacement. */
    N2D_GLOBAL_ALPHA_SCALE      /**< Enable global alpha scaling. */
}
n2d_global_alpha_t;

/**
 * @brief This type represents specific pixel color.
 * @details This contains a color.
*/
typedef n2d_int32_t n2d_color_t;


/**
 * @brief Initialize the 2DGPU.
 * @details Initializes the 2DGPU.
 * @code
 * DRV_2DGPU_Initialize();
 * @endcode
 * @return the color value of the given name in the specified format.
 */
void DRV_2DGPU_Initialize(void);

/**
 * @brief Initialize the 2DGPU.
 * @details Initializes the 2DGPU.
 * @code
 * gfxResult res = gfxGPUInterface.DRV_2DGPU_Line(dest, p1, p2, clipRect, color);
 * @endcode
 * @param dest is the target buffer.
 * @param p1 is the starting point
 * @param p2 is the ending point
 * @param clipRect is the region to clip
 * @param color is the line color
 * @param blend is applied blend
 * @return GFX_SUCCESS if completed, otherwise GFX_FAILURE.
 */
gfxResult DRV_2DGPU_Line(gfxPixelBuffer * dest,
                        const gfxPoint* p1,
                           const gfxPoint* p2,
                           const gfxRect* clipRect,
                           const gfxColor color);

/**
 * @brief Fill rectangle.
 * @details Fills a rectangle defined on <span class="param">dest</span>
 * bounded by <span class="param">clipRect</span> given <span class="param">color</span>.
 * @code
 * gfxResult res = gfxGPUInterface.DRV_2DGPU_Fill(dest, clipRect, color);
 * @endcode
 * @param dest is the target buffer.
 * @param clipRect is the region to clip
 * @param color is the line color
 * @param blend is applied blend
 * @return GFX_SUCCESS if completed, otherwise GFX_FAILURE.
 */
gfxResult DRV_2DGPU_Fill(gfxPixelBuffer * dest,
                           const gfxRect* clipRect,
                           const gfxColor color);

/**
 * @brief Blit a region.
 * @details Blits a region from <span class="param">source</span> bounded
 * by <span class="param">srcRect</span> to
 * <span class="param">dest</span> bounded
 * by <span class="param">destRect</span> with the given
 * <span class="param">blend</span>.
 * @code
 * gfxResult res = gfxGPUInterface.DRV_2DGPU_BlitCopy(source, srcRect, dest, destRect);
 * @endcode
 * @param dest is the target buffer.
 * @param clipRect is the region to clip
 * @param color is the line color
 * @param blend is applied blend
 * @return GFX_SUCCESS if completed, otherwise GFX_FAILURE.
 */

gfxResult DRV_2DGPU_Blit(const gfxPixelBuffer* source,
                           const gfxRect* srcRect,
                           const gfxPixelBuffer* dest,
                        const gfxRect* destRect);


/**
 * @brief Set blend type.
 * @details Sets the blend type specified by <span class="param">blend</span>.
 * @code
 * gfxBlend blend;
 * gfxResult res = gfxGPUInterface.DRV_2DGPU_SetBlend(blend);
 * @endcode
 * @return GFX_SUCCESS if completed, otherwise GFX_FAILURE.
 */
gfxResult DRV_2DGPU_SetBlend(const gfxBlend blend);

/**
 * @brief Set global alpha.
 * @details Set global alpha for source and destination buffers using
 * <span class="param">srcGlobalAlpha</span> and
 * <span class="param">dstGlobalAlpha</span> with specified
 * <span class="param">srcGlobalAlphaValue</span> and
 * <span class="param">dstGlobalAlphaValue</span>.
 * @code
 * gfxResult res = gfxGPUInterface.DRV_2DGPU_SetGlobalAlpha(srcGlobalAlpha, dstGlobalAlpha, srcGlobalAlphaValue, dstGlobalAlphaValue);
 * @endcode
 * @param srcGlobalAlpha is the source alpha operation.
 * @param dstGlobalAlpha is the destination alpha operation
 * @param srcGlobalAlphaValue is the source alpha value
 * @param dstGlobalAlphaValue is the destination alpha value
 * @return GFX_SUCCESS if completed, otherwise GFX_FAILURE.
 */
gfxResult DRV_2DGPU_SetGlobalAlpha(
                        const gfxAlpha srcGlobalAlpha,
                        const gfxAlpha dstGlobalAlpha,
                        uint32_t srcGlobalAlphaValue,
                        uint32_t dstGlobalAlphaValue);

/**
 * @brief Set a index palette.
 * @details Sets a index palette <span class="param">palette</span> at <span class="param">first_index</span>
 * with length <span class="param">index_count</span> usisng
 * color conversion flag <span class="param">color_convert</span>.
 * @code
 * gfxResult res = gfxGPUInterface.DRV_2DGPU_SetPalette(index_count, color_table, color_convert);
 * @endcode
 * @param index_count is the length of table
 * @param color_table is the color table
 * @param color_convert is a swizzle conversion flag
 * @return GFX_SUCCESS if completed, otherwise GFX_FAILURE.
 */
gfxResult DRV_2DGPU_SetPalette(
                        uint32_t index_count,
                        gfxBuffer color_table,
                        gfxBool color_convert);

/**
 * @brief Set transparency type and color.
 * @details Sets the transparency type to <span class="param">transparency</span>
 * for <span class="param">color</span> using the specified raster operations
 * <span class="param">foreground_rop</span> and
 * <span class="param">background_rop</span>.
 * @code
 * gfxResult res = gfxGPUInterface.DRV_2DGPU_SetTransparency(transparency, color, dest, foreground_rop, background_rop);
 * @endcode
 * @param transparency is the transparecy mode.
 * @param color is the color to mask
 * @param foreground_rop is the source raster operation
 * @param background_rop is background raster operation
 * @return GFX_SUCCESS if completed, otherwise GFX_FAILURE.
 */
gfxResult DRV_2DGPU_SetTransparency(
                        gfxTransparency transparency,
                        gfxColor color,
                        uint32_t foreground_rop,
                        uint32_t background_rop);
/**
 * @brief This struct represents the 2DGPU graphics processor interface.
 * @details Establishes the interface for a Legato 2DGPU GPU driver.  This
 * driver supports the following interfaces. Interfaces that return a gfxResult
 * return unsuccessful if not supported.
 */
static const gfxGraphicsProcessor gfxGPUInterface =
{
    NULL,
    DRV_2DGPU_Fill,
    DRV_2DGPU_Blit,
    DRV_2DGPU_SetBlend,
    DRV_2DGPU_SetGlobalAlpha,
    DRV_2DGPU_SetPalette,
    DRV_2DGPU_SetTransparency
};

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _nano2d_h__ */
