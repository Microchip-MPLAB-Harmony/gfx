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
  Module for Microchip Graphics Library - Driver Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    gfx_driver.h

  Summary:
    This file defines the common macros and definitions used by MPLAB Harmony Graphics
    Suite drivers.

  Description:
    This file defines the common macros and definitions used by the
    MPLAB Harmony Graphics Suite drivers.

  Remarks:
    This header file should be included by the graphics library and by each driver.
 *******************************************************************************/

/** \file gfx_driver.h.ftl
 * @brief GFX driver functions and definitions.
 *
 * @details This file defines the common macros and definitions used by the MPLAB
 * Harmony Graphics Suite drivers
 */
#ifndef GFX_DRIVER_H
#define GFX_DRIVER_H

#include <stddef.h>
#include <assert.h>
#include <string.h>

#ifdef _WIN32
#ifndef int8_t
typedef signed __int8        int8_t;
typedef signed __int16       int16_t;
typedef signed __int32       int32_t;
typedef unsigned __int8      uint8_t;
typedef unsigned __int16     uint16_t;
typedef unsigned __int32     uint32_t;
typedef signed __int64       int64_t;
typedef unsigned __int64     uint64_t;
#endif
#else
#include <stdint.h>
#endif

typedef uint32_t               gfxColor;

#define GFX_COLOR_MAX_SIZE      4

#define RGB_8_BITS             255
#define RGB_6_BITS             63
#define RGB_5_BITS             31
#define RGB_3_BITS             7
#define RGB_2_BITS             2

#define RGB_332_RED_MASK       0xE0
#define RGB_332_GREEN_MASK     0x1C
#define RGB_332_BLUE_MASK      0x3

#define RGB_565_RED_MASK       0xF800
#define RGB_565_GREEN_MASK     0x7E0
#define RGB_565_BLUE_MASK      0x1F

#define RGBA_5551_RED_MASK     0xF800
#define RGBA_5551_GREEN_MASK   0x7C0
#define RGBA_5551_BLUE_MASK    0x3E
#define RGBA_5551_ALPHA_MASK   0x1

#define RGB_888_RED_MASK       0xFF0000
#define RGB_888_GREEN_MASK     0xFF00
#define RGB_888_BLUE_MASK      0xFF

#define RGBA_8888_RED_MASK     0xFF000000
#define RGBA_8888_GREEN_MASK   0xFF0000
#define RGBA_8888_BLUE_MASK    0xFF00
#define RGBA_8888_ALPHA_MASK   0xFF

#define ARGB_8888_RED_MASK     0xFF0000
#define ARGB_8888_GREEN_MASK   0xFF00
#define ARGB_8888_BLUE_MASK    0xFF
#define ARGB_8888_ALPHA_MASK   0xFF000000

#define GFX_CTRLR_LAYER_START 0x80000000

#ifdef __cplusplus
    extern "C" {
#endif
        
// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Enumeration:
    gfxResult

  Summary:
    gfx results (success and failure codes).

  Description:
    Various definitions for success and failure codes.

  Remarks:
    None.
*/
/**
 * @brief This enum represents function call results.
 * @details Results are used to report success and failure conditions. These
 * are the various success and failure codes.
 */
typedef enum gfxResult
{
    GFX_FAILURE = -1,
    GFX_SUCCESS = 0
} gfxResult;

// *****************************************************************************
/* Enumeration:
    gfxBool

  Summary:
    gfx bool values

  Description:
    gfx bool values

  Remarks:
    None.
*/
/**
 * @brief This enum represents booleans.
 * @details Boolean are used to communicate true and false conditions.
 */
typedef enum gfxBool
{
    GFX_FALSE = 0,
    GFX_TRUE
} gfxBool;

// *****************************************************************************
/* Structure:
    gfxPoint

  Summary:
    A two dimensional Cartesian point.
*/
/**
 * @brief This struct represents a point.
 * @details A point is used to define a two dimensional Cartesian coordinate.
 */
typedef struct gfxPoint
{
    int32_t x;
    int32_t y;
} gfxPoint;

/**
 * @brief This constant represents a zero point.
 * @details The zero is used to .
 */
static const gfxPoint gfxPoint_Zero = {0, 0};

// *****************************************************************************
/* Structure:
    gfxSize

  Summary:
    A two dimensional indication of size.  Values are signed but should never be
    negative.
*/
/**
 * @brief This struct represents a size.
 * @details A size is use to communicate the width and height of objects.
 */
typedef struct gfxSize
{
    int32_t width;
    int32_t height;
} gfxSize;

/**
 * @brief This constant represents a zero size.
 * @details
 */
static const gfxSize gfxSize_Zero = {0, 0};

// *****************************************************************************
/* Structure:
    gfxRect

  Summary:
    A rectangle definition.  
*/
/**
 * @brief This struct represents a rectangle.
 * @details A rectangle defines a rectangle boundary.
 * @details
 */
typedef struct gfxRect
{
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
} gfxRect;

/**
 * @brief This constant represents a zero rectangle.
 * @details
 */
static const gfxRect gfxRect_Zero = {0, 0, 0, 0};

// *****************************************************************************
/* Structure:
    gfxLayerState

  Summary:
    A layer state structure definition.  
*/
/**
 * @brief This struct represents a layer state.
 * @details This structure is provided by the driver to describe layer state.
 * @details
 */
typedef struct gfxLayerState
{
    gfxRect rect;
    gfxBool enabled;
} gfxLayerState;

/**
 * @brief This type represents a buffer;
 * @details
 */
typedef void* gfxBuffer;

// *****************************************************************************
/* Enumeration:
    gfxColorMask

  Summary:
    Maskable list of color valies.

*/
/**
 * @brief This enum represents a color mask.
 * @details Colormask is maskable list of color values to extract color components.
 */
typedef enum gfxColorMask
{
    GFX_COLOR_MASK_GS_8      = 0x1,
    GFX_COLOR_MASK_PALETTE   = 0x1,
    GFX_COLOR_MASK_RGB_332   = 0x4,
    GFX_COLOR_MASK_RGB_565   = 0x8,
    GFX_COLOR_MASK_RGBA_5551 = 0x10,
    GFX_COLOR_MASK_RGB_888   = 0x20,
    GFX_COLOR_MASK_RGBA_8888 = 0x40,
    GFX_COLOR_MASK_ARGB_8888 = 0x80,
    GFX_COLOR_MASK_ALL = GFX_COLOR_MASK_GS_8      |
                        GFX_COLOR_MASK_RGB_332   |
                        GFX_COLOR_MASK_RGB_565   |
                        GFX_COLOR_MASK_RGBA_5551 |
                        GFX_COLOR_MASK_RGB_888   | 
                        GFX_COLOR_MASK_RGBA_8888 |
                        GFX_COLOR_MASK_ARGB_8888
} gfxColorMask;

// *****************************************************************************
/* Enumeration:
    gfxColorMode

  Summary:
    List of available color modes.
*/
/**
 * @brief This enum represents color modes.
 * @details Lists the common RGB color representations.
 * @see GFX_COLOR_MODE_IS_PIXEL(mode) GFX_COLOR_MODE_IS_INDEX(mode) GFX_COLOR_MODE_IS_ALPHA(mode)
 */
typedef enum gfxColorMode
{
    GFX_COLOR_MODE_GS_8       = 0x0,
    GFX_COLOR_MODE_PALETTE    = GFX_COLOR_MODE_GS_8,
    GFX_COLOR_MODE_RGB_332    = 0x1,
    GFX_COLOR_MODE_RGB_565    = 0x2,
    GFX_COLOR_MODE_RGBA_5551  = 0x3,
    GFX_COLOR_MODE_RGB_888    = 0x4,
    GFX_COLOR_MODE_RGBA_8888  = 0x5,
    GFX_COLOR_MODE_ARGB_8888  = 0x6,
    GFX_COLOR_MODE_INDEX_1    = 0x7,
    GFX_COLOR_MODE_INDEX_4    = 0x8,
    GFX_COLOR_MODE_INDEX_8    = 0x9,
    GFX_COLOR_MODE_LAST = GFX_COLOR_MODE_INDEX_8
} gfxColorMode;

#define GFX_COLOR_MODE_LAST_COLOR     (GFX_COLOR_MODE_ARGB_8888)
#define GFX_COLOR_MODE_COUNT          (GFX_COLOR_MODE_LAST + 1)

#define GFX_COLOR_MODE_IS_PIXEL(mode) ((mode >= GFX_COLOR_MODE_GS_8) && (mode <= GFX_COLOR_MODE_ARGB_8888))
#define GFX_COLOR_MODE_IS_INDEX(mode) ((mode >= GFX_COLOR_MODE_INDEX_1) && (mode <= GFX_COLOR_MODE_INDEX_8))

#define GFX_COLOR_MODE_IS_ALPHA(mode) ((mode == GFX_COLOR_MODE_RGBA_5551) || (mode == GFX_COLOR_MODE_RGBA_8888) || (mode == GFX_COLOR_MODE_ARGB_8888))

// *****************************************************************************
/* Enumeration:
    gfxBitsPerPixel

  Summary:
    List of available bits-per-pixel sizes.    
*/
/**
 * @brief This enum represents bits per pixel (BPP).
 * @details List of commonly BPP values to represent color depth.
 */
typedef enum gfxBitsPerPixel
{
    GFX_BPP1,
    GFX_BPP4,
    GFX_BPP8,
    GFX_BPP16,
    GFX_BPP24,
    GFX_BPP32
} gfxBitsPerPixel;

// *****************************************************************************
/* Structure:
    gfxColorModeInfo

  Summary:
    Struct that provides information about a color mode.

  Description:
    size - size in bytes
    bpp - bpp value
    bppOrdinal - bpp enum value
    masks - the masks used for extracting individual color channel information

  Remarks:
    None.
*/
/**
 * @brief This struct represents color mode information.
 * @details Color mode information is use to keep track of
 * specific information about a color.
 */
typedef struct gfxColorModeInfo
{
    uint32_t size;
    uint32_t bpp;
    gfxBitsPerPixel bppOrdinal;

    struct
    {
        uint32_t red;
        uint32_t green;
        uint32_t blue;
        uint32_t alpha;
    } mask;
    
    struct
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
    } shift;
    
} gfxColorModeInfo;

// *****************************************************************************
/* Data Table:
    gfxColorInfoTable

  Summary:
    Color information reference table
*/
/**
 * @brief This array represents a color information reference table.
 * @details
 */
extern const gfxColorModeInfo gfxColorInfoTable[];


// *****************************************************************************
/* Structure:
    gfxBlend

  Summary:
    Blend name reference table
*/
/**
 * @brief This enum represents blend modes.
 * @details Blend modes are used to define the type of blend function.
 */
typedef enum gfxBlend
{
    GFX_BLEND_NONE,
    GFX_BLEND_SRC_OVER,
    GFX_BLEND_DST_OVER,
    GFX_BLEND_SRC_IN,
    GFX_BLEND_DST_IN,
    GFX_BLEND_ADDITIVE,
    GFX_BLEND_SUBTRACT,
} gfxBlend;

/**
 * @brief This enum represents alpha modes.
 * @details Global alpha modes are used to define the type of global
 * alpha blending operations.
 */
typedef enum gfxAlpha
{
    GFX_GLOBAL_ALPHA_OFF,
    GFX_GLOBAL_ALPHA_ON,
    GFX_GLOBAL_ALPHA_SCALE
} gfxAlpha;

/**
 * @brief This enum represents transparency modes.
 * @details Transparency modes are used to define the target of transparency
 * to perform.
 */
typedef enum gfxTransparency
{
    GFX_TRANSPARENCY_NONE,
    GFX_TRANSPARENCY_SOURCE,
    GFX_TRANSPARENCY_DESTINATION,
}
gfxTransparency;


typedef enum gfxPaletteSelect
{
    GFX_PALETTE_ONE,
    GFX_PALETTE_TWO
}
gfxPaletteSelect;

/**
 * @brief This enum represents buffer flags.
 * @details List of buffer status conditions.
 */
enum gfxBufferFlags
{
    GFX_BF_NONE = 0,
    GFX_BF_LOCKED = 1 << 0,
};

// *****************************************************************************
/* Structure:
    gfxPixelBuffer

  Summary:
    A pixel buffer is a wrapper around a basic data pointer.  A pixel buffer
    has a color mode, a pixel count, a rectangular dimension, a pixel count,
    and a lenght in bytes.

  Description:
    mode - the color mode of the pixel buffer
    size - the width and height dimension of the pixel buffer
    pixel_count - the total number of pixels in the buffer
    buffer_length - the total size of the buffer in bytes
    pixels - the pointer to the pixel data for the buffer

  Remarks:
    None.
*/
/**
 * @brief This struct represents a pixel buffer.
 * @details A pixel buffer is a wrapper around a basic data pointer.  A
 * pixel buffer has a color mode, a pixel count, a rectangular dimension, a
 * pixel count, and a length in bytes.
 */
typedef struct gfxPixelBuffer
{
    gfxColorMode mode;
    
    gfxSize size;
    uint32_t pixel_count;
    
    uint32_t buffer_length;
    gfxBuffer pixels;

    uint32_t flags;
} gfxPixelBuffer;



// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Function:
    uint32_t gfxColorChannelRed(gfxColor clr, gfxColorMode mode)

  Summary:
    Used for getting the red color channel of a given color value.

  Description:
    

  Parameters:
    gfxColor - the source color value
    gfxColorMode - the source color mode
    
  Returns:
    uint32_t - the red color channel
    
  Remarks:
    
*/
/**
 * @brief Get red channel.
 * @details Returns the red channel from <span class="param">clr</span>
 * using <span class="param">mode</span>.
 * @code
 * gfxColor clr;
 * gfxColorMode mode;
 * uint32_t red = gfxColorChannelRed(clr, mode);
 * @endcode
 * @param clr is an rgb color.
 * @param mode is an rgb colormode.
 * @return Returns the red color component
 */
uint32_t gfxColorChannelRed(gfxColor clr, gfxColorMode mode);

// *****************************************************************************
/* Function:
    uint32_t gfxColorChannelGreen(gfxColor clr, gfxColorMode mode)

  Summary:
    Used for getting the green color channel of a given color value.

  Description:
    

  Parameters:
    gfxColor - the source color value
    gfxColorMode - the source color mode
    
  Returns:
    uint32_t - the green color channel
    
  Remarks:
    
*/
/**
 * @brief Get green channel.
 * @details Returns the green channel from <span class="param">clr</span>
 * using <span class="param">mode</span>.
 * @code
 * gfxColor clr;
 * gfxColorMode mode;
 * uint32_t green = gfxColorChannelGreen(clr, mode);
 * @endcode
 * @param clr is an rgb color.
 * @param mode is an rgb colormode.
 * @return Returns the green color component
 */
uint32_t gfxColorChannelGreen(gfxColor clr, gfxColorMode mode);

// *****************************************************************************
/* Function:
    uint32_t gfxColorChannelBlue(gfxColor clr, gfxColorMode mode)

  Summary:
    Used for getting the blue color channel of a given color value.

  Description:
    

  Parameters:
    gfxColor - the source color value
    gfxColorMode - the source color mode
    
  Returns:
    uint32_t - the blue color channel
    
  Remarks:
    
*/
/**
 * @brief Get blue channel.
 * @details Returns the blue channel from <span class="param">clr</span>
 * using <span class="param">mode</span>.
 * @code
 * gfxColor clr;
 * gfxColorMode mode;
 * uint32_t blue = gfxColorChannelBlue(clr, mode);
 * @endcode
 * @param clr is an rgb color.
 * @param mode is an rgb colormode.
 * @return Returns the blue color component
 */
uint32_t gfxColorChannelBlue(gfxColor clr, gfxColorMode mode);

// *****************************************************************************
/* Function:
    uint32_t gfxColorChannelAlpha(gfxColor clr, gfxColorMode mode)

  Summary:
    Used for getting the alpha color channel of a given color value.

  Description:
    

  Parameters:
    gfxColor - the source color value
    gfxColorMode - the source color mode
    
  Returns:
    uint32_t - the alpha color channel
    
  Remarks:
    
*/
/**
 * @brief Get alpha channel.
 * @details Returns the alpha channel from <span class="param">clr</span>
 * using <span class="param">mode</span>.
 * @code
 * gfxColor clr;
 * gfxColorMode mode;
 * uint32_t alpha = gfxColorChannelAlpha(clr, mode);
 * @endcode
 * @param clr is an rgb color.
 * @param mode is an rgb colormode.
 * @return Returns the alpha color component
 */
uint32_t gfxColorChannelAlpha(gfxColor clr, gfxColorMode mode); 

// *****************************************************************************
/* Function:
    gfxColor gfxColorConvert(gfxColorMode mode_in,
                           gfxColorMode mode_out,
                           gfxColor color)

  Summary:
    Converts a color value from one mode to another


  Parameters:
    gfxColorMode - the input color mode
    gfxColorMode - the output color mode
    gfxColor - the source color
    
  Returns:
    gfxColor - the result color
    
  Remarks:
    
*/
gfxColor gfxColorConvert(gfxColorMode mode_in,
                         gfxColorMode mode_out,
                         gfxColor color);                             

// *****************************************************************************
/* Function:
    fxResult gfxPixelBufferCreate(const int32_t width,
                                 const int32_t height,
                                 const gfxColorMode mode,
                                 const void* const address,
                                 gfxPixelBuffer* buffer)

  Summary:
    Initializes a pixel buffer struct.  Does not actually allocate any memory.

  Parameters:
    const int32_t - the width of the buffer
    const int32_t - the height of the buffer
    const gfxColorMode - the color mode of the buffer
    const void* - the data addres of the buffer (may be NULL)
    gfxPixelBuffer* - pointer of the pixel buffer buffer to initialize
    
  Returns:
    gfxResult
    
  Remarks:
    
*/
/**
 * @brief Create pixel buffer.
 * @details Initializes a rectangular pixel buffer <span class="param">buffer</span>
 * with size <span class="param">x</span> by <span class="param">y</span>
 * using <span class="param">mode</span> at optional address
 * <span class="param">address</span>.
 * @code
 * gfxColor clr;
 * gfxColorMode mode;
 * uint32_t blue = gfxPixelBufferCreate(clr, mode);
 * @endcode
 * @param width is the horizonal width.
 * @param height is the vertical height.
 * @param mode is an rgb colormode.
 * @param address is an optional address location.
 * @param buffer is buffer to initialize.
 * @return GFX_SUCCESS if set, otherwise GFX_FAILURE.
 */

gfxResult gfxPixelBufferCreate(const int32_t width,
                               const int32_t height,
                               const gfxColorMode mode,
                               const void* const address,
                               gfxPixelBuffer* buffer);

// *****************************************************************************
/* Function:
    gfxBuffer gfxPixelBufferOffsetGet(const gfxPixelBuffer* const buffer,
                                    const gfxPoint* const pnt)

  Summary:
    Gets the offset address of the pixel that resides at the provided
    point in the given buffer.

  Parameters:
    const gfxPixelBuffer* - the source buffer
    const gfxPoint* - the point for which the offset should be calculated
    
  Returns:
    gfxBuffer - the pointer to the offset point in the source buffer
*/
/**
 * @brief Get buffer offset.
 * @details Gets the buffer offset of <span class="param">buffer</span> at location
 * <span class="param">x</span> and <span class="param">y</span>.
 * @code
 * gfxBuffer buffer;
 * uint32_t x, y;
 * gfxBuffer offset = gfxPixelBufferOffsetGet(buffer, x, y);
 * @endcode
 * @param buffer is pixel buffer.
 * @param x is a x coordinate.
 * @param y is a y coordinate.
 * @return Returns buffer offset.
 */
gfxBuffer gfxPixelBufferOffsetGet(const gfxPixelBuffer* const buffer,
                                  uint32_t x,
                                  uint32_t y);

// *****************************************************************************
/* Function:
    gfxBuffer gfxPixelBufferOffsetGet_Unsafe(const gfxPixelBuffer* const buffer,
                                               const gfxPoint* const pnt)

  Summary:
    Gets the offset address of the pixel that resides at the provided
    point in the given buffer.  Similar to gfxPixelBufferOffsetGet but performs
    no bounds checking.

  Parameters:
    const gfxPixelBuffer* - the source buffer
    const gfxPoint* - the point for which the offset should be calculated
    
  Returns:
    gfxBuffer - the pointer to the offset point in the source buffer
*/
/**
 * @brief Get buffer offset.
 * @details Gets the buffer offset of <span class="param">buffer</span> at location
 * <span class="param">x</span> and <span class="param">y</span>.
 * @code
 * gfxBuffer buffer;
 * uint32_t x, y;
 * gfxBuffer offset = gfxPixelBufferOffsetGet_Unsafe(buffer, x, y);
 * @endcode
 * @param buffer is pixel buffer.
 * @param x is a x coordinate.
 * @param y is a y coordinate.
 * @return Returns buffer offset.
 */
gfxBuffer gfxPixelBufferOffsetGet_Unsafe(const gfxPixelBuffer* const buffer,
                                         uint32_t x,
                                         uint32_t y);                                                  

// *****************************************************************************
/* Function:
    gfxColor gfxPixelBufferGet(const gfxPixelBuffer* const buffer,
                                 const gfxPoint* const pnt)

  Summary:
    Gets the value of the pixel that resides at the provided point in
    the given buffer.

  Parameters:
    const gfxPixelBuffer* - the source buffer
    const gfxPoint* - the point for which the offset should be calculated
    
  Returns:
    gfxColor - the value of the pixel at the point in the source buffer
*/
/**
 * @brief Get pixel.
 * @details Gets the pixel from <span class="param">buffer</span> at location
 * <span class="param">x</span> and <span class="param">y</span>.
 * @code
 * gfxBuffer buffer;
 * uint32_t x, y;
 * gfxColor clr = gfxPixelBufferGet(buffer, x, y);
 * @endcode
 * @param buffer is pixel buffer.
 * @param x is a x coordinate.
 * @param y is a y coordinate.
 * @return the value of the pixel at the point in the source buffer
 */
gfxColor gfxPixelBufferGet(const gfxPixelBuffer* const buffer,
                           uint32_t x,
                           uint32_t y);

// *****************************************************************************
/* Function:
    gfxColor gfxPixelBufferGet_Unsafe(const gfxPixelBuffer* const buffer,
                                    const gfxPoint* const pnt)

  Summary:
    Gets the value of the pixel that resides at the provided point in
    the given buffer.  Like gfxPixelBufferGet but performs no bounds checking.

  Parameters:
    const gfxPixelBuffer* - the source buffer
    const gfxPoint* - the point for which the offset should be calculated
    
  Returns:
    gfxColor - the value of the pixel at the point in the source buffer
*/
/**
 * @brief Get pixel.
 * @details Gets the pixel from <span class="param">buffer</span> at location
 * <span class="param">x</span> and <span class="param">y</span>.
 * @remark Like gfxPixelBufferGet but performs no bounds checking.
 * @code
 * gfxBuffer buffer;
 * uint32_t x, y;
 * gfxColor clr = gfxPixelBufferGet(buffer, x, y);
 * @endcode
 * @param buffer is pixel buffer.
 * @param x is a x coordinate.
 * @param y is a y coordinate.
 * @return the value of the pixel at the point in the source buffer
 */
gfxColor gfxPixelBufferGet_Unsafe(const gfxPixelBuffer* const buffer,
                                  uint32_t x,
                                  uint32_t y);
                                               
// *****************************************************************************
/* Function:
    gfxColor gfxPixelBufferGetIndex(const gfxPixelBuffer* const buffer,
                                  const int32_t idx)

  Summary:
    Interprets the pixel buffer as a tabgfx of indices and looks up a specific
    index at position 'idx'.  Indices may be 1bpp, 4bpp, or 8bpp in size and
    are indicated by the color mode of the pixel buffer.

  Parameters:
    const gfxPixelBuffer* const - the input buffer
    const int32_t - the index to retrieve
    
  Returns:
    gfxColor - the resultant value that was retrieved
*/
/**
 * @brief Get pixel at index.
 * @details Gets the pixel from <span class="param">buffer</span> at index
 * <span class="param">idx</span>.
 * Interprets the pixel buffer as a table of indices and looks up a specific index at
 * position 'idx'.  Indices may be 1bpp, 4bpp, or 8bpp in size and are indicated by the
 * color mode of the pixel buffer.
 * @code
 * gfxBuffer buffer;
 * uint32_t idx;
 * gfxColor clr = gfxPixelBufferGetIndex(buffer, idx);
 * @endcode
 * @param buffer is pixel buffer.
 * @param idx is a location.
 * @return Returns color.
 */
gfxColor gfxPixelBufferGetIndex(const gfxPixelBuffer* const buffer,
                                       const uint32_t idx);

// *****************************************************************************
/* Function:
    gfxColor gfxPixelBufferGetIndex_Unsafe(const gfxPixelBuffer* const buffer,
                                     const uint32_t idx)

  Summary:
    A faster less-safe version of lePixelBufferGetIndex;

  Parameters:
    const gfxPixelBuffer* const - the input buffer
    const int32_t - the index to retrieve

  Returns:
    gfxColor - the resultant value that was retrieved
*/
/**
 * @brief Get pixel at index.
 * @details Gets the pixel from <span class="param">buffer</span> at index
 * <span class="param">idx</span>.
 * Interprets the pixel buffer as a table of indices and looks up a specific index at
 * position 'idx'.  Indices may be 1bpp, 4bpp, or 8bpp in size and are indicated by the
 * color mode of the pixel buffer.
 * A faster less-safe version of gfxPixelBufferGetIndex;
 * @code
 * gfxBuffer buffer;
 * uint32_t idx;
 * gfxColor clr = gfxPixelBufferGetIndex_Unsafe(buffer, idx);
 * @endcode
 * @param buffer is pixel buffer.
 * @param idx is a location.
 * @return Returns color.
 */
gfxColor gfxPixelBufferGetIndex_Unsafe(const gfxPixelBuffer* const buffer,
                                       const uint32_t idx); 

// *****************************************************************************
/* Function:
    gfxResult gfxPixelBuffer_IsLocked(const gfxPixelBuffer* const buffer)

  Summary:
    Returns true if the buffer's lock flag is on.

  Parameters:
    const gfxPixelBuffer* const buffer - the buffer to query

  Returns:
    gfxBool - true if the buffer is locked
*/
/**
 * @brief Determine if buffer is locked.
 * @details Returns locked status of <span class="param">buffer</span>.
 * @code
 * gfxBuffer buffer;
 * gfxColor clr = gfxPixelBuffer_IsLocked(buffer);
 * @endcode
 * @param buffer is pixel buffer.
 * @return GFX_SUCCESS if set, otherwise GFX_FAILURE.
 */
gfxBool gfxPixelBuffer_IsLocked(const gfxPixelBuffer* const buffer);

// *****************************************************************************
/* Function:
    gfxResult gfxPixelBuffer_SetLocked(gfxPixelBuffer* buffer,
                                     gfxBool locked)

  Summary:
    Manipulates a buffer lock flag

  Parameters:
    const gfxPixelBuffer* const buffer - the buffer to query
    gfxBool locked - the desired lock state

  Returns:
    gfxResult
*/
/**
 * @brief Set locked state.
 * @details Sets the lock state of <span class="param">buffer</span>.
 * @code
 * gfxBuffer buffer;
 * gfxBool locked;
 * gfxResult res = gfxPixelBuffer_SetLocked(buffer, locked);
 * @endcode
 * @param buffer is pixel buffer to modify.
 * @param locked is new lock state.
 * @return GFX_SUCCESS if set, otherwise GFX_FAILURE.
 */
gfxResult gfxPixelBuffer_SetLocked(gfxPixelBuffer* buffer,
                                   gfxBool locked);


/**
 * @brief This struct represents the possible IOCTL requests for a gfx driver.
 * @details This struct represents the possible IOCTL requests for a gfx driver.
 */
typedef enum gfxDriverIOCTLRequest
{
    GFX_IOCTL_RESET, // resets the device, arg = NULL
    GFX_IOCTL_GET_COLOR_MODE, // returns the supported color mode for the driver, arg = gfxIOCTLArg_Value
    GFX_IOCTL_GET_BUFFER_COUNT, // returns the driver buffer count, arg = gfxIOCTLArg_Value
    GFX_IOCTL_GET_DISPLAY_SIZE, // returns the driver buffer count, arg = gfxIOCTLArg_DisplaySize
    GFX_IOCTL_ENABLE_GPU, // tells the driver to utilize a GPU if possible, arg = gfxIOCTLArg_Value
    
    GFX_IOCTL_LAYER_SWAP, // indicates that the driver should swap the current layer, arg = NULL
    GFX_IOCTL_FRAME_START, // indicates that the driver should begin a new frame, arg = gfxIOCTLArg_Value
    GFX_IOCTL_FRAME_END, // indicates that the driver should end the current frame, arg = NULL
    GFX_IOCTL_GET_VSYNC_COUNT, // gets the current driver vsync count, arg = gfxIOCTLArg_Value
    GFX_IOCTL_GET_FRAMEBUFFER, // gets a pointer to the internal driver frame buffer, arg = gfxIOCTLArg_Value
    GFX_IOCTL_SET_PALETTE, // sets the current driver palette, arg = gfxIOCTLArg_Palette
    
    GFX_IOCTL_GET_LAYER_COUNT, // gets the driver layer count, arg = gfxIOCTLArg_Value
    GFX_IOCTL_GET_ACTIVE_LAYER, // gets the active driver layer, arg = gfxIOCTLArg_Value
    GFX_IOCTL_SET_ACTIVE_LAYER, // sets the active driver layer, arg = gfxIOCTLArg_Value

    GFX_IOCTL_SET_LAYER_LOCK, // locks a layer, arg = gfxIOCTLArg_LayerValue
    GFX_IOCTL_GET_LAYER_ENABLED, // get layer enabled, arg = gfxIOCTLArg_LayerValue
    GFX_IOCTL_SET_LAYER_ENABLED, // set layer enabled, arg = gfxIOCTLArg_LayerValue
    GFX_IOCTL_SET_LAYER_UNLOCK, // unlocks a layer, arg = gfxIOCTLArg_LayerValue
    GFX_IOCTL_GET_LAYER_RECT, // get layer rect, arg = gfxIOCTLArg_LayerRect
    GFX_IOCTL_SET_LAYER_POSITION, // set layer position, arg = gfxIOCTLArg_LayerPosition
    GFX_IOCTL_SET_LAYER_SIZE, // sets a layer size, arg = gfxIOCTLArg_LayerSize
    GFX_IOCTL_SET_LAYER_WINDOW_SIZE, // set layer clipped window size, arg = gfxIOCTLArg_LayerWindowSize
    GFX_IOCTL_SET_LAYER_ALPHA, // set layer alpha value, arg = gfxIOCTLArg_LayerValue
    GFX_IOCTL_SET_LAYER_BASE_ADDRESS, // set layer base address, arg = gfxIOCTLArg_LayerValue
    GFX_IOCTL_SET_LAYER_COLOR_MODE, // set layer color mode, arg = gfxIOCTLArg_LayerValue
} gfxDriverIOCTLRequest;

#define GFX_IOCTL_LAYER_REQ_START GFX_IOCTL_SET_LAYER_LOCK
#define GFX_IOCTL_LAYER_REQ_END   GFX_IOCTL_SET_LAYER_COLOR_MODE

/**
 * @brief This enum represents the possible IOCTL responses for a gfx driver.
 * @details This enum represents the possible IOCTL responses for a gfx driver.
 */
typedef enum gfxDriverIOCTLResponse
{
    GFX_IOCTL_UNSUPPORTED   = -1,
    GFX_IOCTL_OK            =  0,
    GFX_IOCTL_ERROR_UNKNOWN =  1,
} gfxDriverIOCTLResponse;

/**
 * @brief This struct represents a standard IOCTL value argument.
 * @details This struct represents a standard IOCTL value argument.
 */
typedef struct gfxIOCTLArg_Value
{
    union
    {
        uint32_t v_uint;
        int32_t v_int;
        void* v_pointer;
        gfxBool v_bool;
        gfxColorMode v_colormode;
        gfxPixelBuffer* v_pbuffer;
    } value;
} gfxIOCTLArg_Value;

/**
 * @brief This struct represents a standard IOCTL size value argument.
 * @details This struct represents a standard IOCTL size value argument.
 */
typedef struct gfxIOCTLArg_DisplaySize
{
    uint32_t width;
    uint32_t height;
} gfxIOCTLArg_DisplaySize;

/**
 * @brief This struct represents an IOCTL palette argument.
 * @details This struct represents an IOCTL palette value argument.
 */
typedef struct gfxIOCTLArg_Palette
{
    gfxBuffer* palette;
    gfxColorMode mode;
    uint32_t colorCount;
} gfxIOCTLArg_Palette;

/**
 * @brief This struct represents a basic layer request argument.
 * @details This struct represents a basic layer request argument.
 */
typedef struct
{
    uint32_t id;
} gfxIOCTLArg_LayerArg;

/**
 * @brief This struct represents layer position.
 * @details Layer point is used to define the position of a layer.
 */
typedef struct
{
    gfxIOCTLArg_LayerArg base;
    
    int32_t x;
    int32_t y;
} gfxIOCTLArg_LayerPosition;

/**
 * @brief This struct represents layer size.
 * @details Layer size is used to define the extent of a layer.
 */
typedef struct
{
    gfxIOCTLArg_LayerArg base;
    
    uint32_t width;
    uint32_t height;
} gfxIOCTLArg_LayerSize;

/**
 * @brief This struct represents a layer rectangle.
 * @details Layer size is used to define the dimensions of a layer.
 */
typedef struct
{
    gfxIOCTLArg_LayerArg base;
    
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;
} gfxIOCTLArg_LayerRect;

/**
 * @brief This struct represents a common layer value.
 * @details .
 */
typedef struct
{
    gfxIOCTLArg_LayerArg base;
    
    union
    {
        uint32_t v_uint;
        int32_t v_int;
        void* v_pointer;
        gfxBool v_bool;
        gfxColorMode v_colormode;
    } value;
} gfxIOCTLArg_LayerValue;

/**
 * @brief This struct represents the display driver interface.
 * @details Establishes the interface for a Legato display driver.  All drivers
 * must, at a minimum, implement these interfaces. Interfaces with gfxResult
 * can return unsuccessful if not supported.
 * @see implementations:
 * - drv_gfx_lcc_ftl.h
 * - drv_gfx_glcd.h
 * - drv_gfx_external_controller_ftl.h
 */
typedef struct gfxDisplayDriver
{
    void (*update)(void);

    gfxResult (*blitBuffer)(int32_t x,
                            int32_t y,
                            gfxPixelBuffer* buf);

    gfxDriverIOCTLResponse (*ioctl)(gfxDriverIOCTLRequest request,
                                    void* arg);

} gfxDisplayDriver;

/**
 * @brief This struct represents the graphics processor interface.
 * @details Establishes the interface for a Legato GPU driver.  All drivers
 * must, at a minimum, implement these interfaces. Interfaces that return gfxResult
 * can return unsuccessful if not supported.
 * @see implementations:
 * - libnano2d_ftl.h
 */
typedef struct gfxGraphicsProcessor
{
    gfxResult (*drawLine)(gfxPixelBuffer * dest,      /**< @see drv_gfx_glcd.h */
                          const gfxPoint* p1,
                          const gfxPoint* p2,
                          const gfxRect* clipRect,
                          const gfxColor color);

    gfxResult (*fillRect)(gfxPixelBuffer * dest,
                          const gfxRect* clipRect,
                          const gfxColor color);

    gfxResult (*blitBuffer)(const gfxPixelBuffer* source,
                            const gfxRect* srcRect,
                            const gfxPixelBuffer* dest,
                            const gfxRect* destRect);

    gfxResult (*setBlend)(const gfxBlend blend);

    gfxResult (*setGlobalAlpha)(
                        const gfxAlpha srcGlobalAlpha,
                        const gfxAlpha dstGlobalAlpha,
                        uint32_t srcGlobalAlphaValue,
                        uint32_t dstGlobalAlphaValue);

    gfxResult (*setPalette)(
                        uint32_t index_count,
                        gfxBuffer color_table,
                        gfxBool color_convert);

    gfxResult (*setTransparency)(
                        gfxTransparency transparency,
                        gfxColor color,
                        uint32_t foreground_rop,
                        uint32_t background_rop);

} gfxGraphicsProcessor;

<#if gfx_legato?? && gfx_legato.LeGPUConnected?? && gfx_legato.LeGPUConnected == false>
static const gfxGraphicsProcessor gfxGPUInterface =
{
    NULL,
    NULL,
    NULL,
    NULL
};
</#if>

#ifdef __cplusplus
    }
#endif
     
#endif // GFX_DRIVER_H

/*******************************************************************************
 End of File
*/
