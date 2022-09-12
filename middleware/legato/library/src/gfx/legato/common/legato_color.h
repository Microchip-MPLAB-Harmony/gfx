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
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_color.h

  Summary:
    Contains functions for color information and manipulation operations

  Description:
    Color conversion and color channel management
*******************************************************************************/

/** \file legato_color.h
 * @brief Color definitions and functions.
 *
 * @details Contains functions for color information and manipulation
 * operations. Color conversion and color channel management.
 */

#ifndef LE_COLOR_H
#define LE_COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/common/legato_common.h"

/**
  * @cond INTERNAL
  */
typedef uint32_t leColor;

#define LE_COLOR_MAX_SIZE      4

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
/**
  * @endcond
  */

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leColorMask

  Summary:
    Maskable list of color valies.

*/
/**
 * @brief This enum represents the color masks for color modes.
 * @details .
 */
typedef enum leColorMask
{
    LE_COLOR_MASK_GS_8 = 0x1,
    LE_COLOR_MASK_PALETTE = 0x1,
    LE_COLOR_MASK_RGB_332 = 0x4,
    LE_COLOR_MASK_RGB_565 = 0x8,
    LE_COLOR_MASK_RGBA_5551 = 0x10,
    LE_COLOR_MASK_RGB_888 = 0x20,
    LE_COLOR_MASK_RGBA_8888 = 0x40,
    LE_COLOR_MASK_ARGB_8888 = 0x80,
    LE_COLOR_MASK_ALL = LE_COLOR_MASK_GS_8 |
    LE_COLOR_MASK_RGB_332 |
    LE_COLOR_MASK_RGB_565 |
    LE_COLOR_MASK_RGBA_5551 |
    LE_COLOR_MASK_RGB_888 |
    LE_COLOR_MASK_RGBA_8888 |
    LE_COLOR_MASK_ARGB_8888
} leColorMask;

// *****************************************************************************
/* Enumeration:
    leColorMode

  Summary:
    List of available color modes.
*/
/**
 * @brief This enum represents the supported RGB color formats.
 * @details The pixels in a graphics buffer or supported display is represented
 * by the pixel format. For embedded graphics this is limited by memory and hardware.
 */
typedef enum leColorMode
{
    LE_COLOR_MODE_GS_8       = 0,
    LE_COLOR_MODE_RGB_332    = 1,
    LE_COLOR_MODE_RGB_565    = 2,
    LE_COLOR_MODE_RGBA_5551  = 3,
    LE_COLOR_MODE_RGB_888    = 4,
    LE_COLOR_MODE_RGBA_8888  = 5,
    LE_COLOR_MODE_ARGB_8888  = 6,
    LE_COLOR_MODE_INDEX_1    = 7,
    LE_COLOR_MODE_INDEX_4    = 8,
    LE_COLOR_MODE_INDEX_8    = 9,
    LE_COLOR_MODE_PALETTE    = LE_COLOR_MODE_INDEX_8,
    LE_COLOR_MODE_MONOCHROME = 10
} leColorMode;

#define LE_COLOR_MODE_LAST_COLOR     (LE_COLOR_MODE_MONOCHROME)
#define LE_COLOR_MODE_COUNT          (LE_COLOR_MODE_LAST_COLOR + 1)

#define LE_COLOR_MODE_IS_PIXEL(mode) ((mode >= LE_COLOR_MODE_GS_8) && (mode <= LE_COLOR_MODE_ARGB_8888))
#define LE_COLOR_MODE_IS_INDEX(mode) ((mode >= LE_COLOR_MODE_INDEX_1) && (mode <= LE_COLOR_MODE_INDEX_8))

#define LE_COLOR_MODE_IS_ALPHA(mode) ((mode == LE_COLOR_MODE_RGBA_5551) || (mode == LE_COLOR_MODE_RGBA_8888) || (mode == LE_COLOR_MODE_ARGB_8888))

// *****************************************************************************
/* Enumeration:
    leBitsPerPixel

  Summary:
    List of available bits-per-pixel sizes.    
*/
/**
 * @brief This enum represents the bits per pixel (bpp) options.
 * @details The BPP controls the color depth and bit needed to hold
 * a color. Higher bpp allows for more colours to be represented,
 * however, more resources are required to store or display.
 */
typedef enum leBitsPerPixel
{
    LE_BPP1,
    LE_BPP4,
    LE_BPP8,
    LE_BPP16,
    LE_BPP24,
    LE_BPP32
} leBitsPerPixel;

// *****************************************************************************
/* Structure:
    leColorModeInfo

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
 * @details Color mode information keeps track of size, bpp,
 * color channel mask and color.
 */
typedef struct leColorModeInfo
{
    uint32_t size;              /**< size in bytes. */
    uint32_t bpp;               /**< bpp value. */
    leBitsPerPixel bppOrdinal;  /**< bpp enum value. */

    struct
    {
        uint32_t red;
        uint32_t green;
        uint32_t blue;
        uint32_t alpha;
    } mask;                     /**< individual color mask. */

    struct
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
    } shift;

} leColorModeInfo;

// *****************************************************************************
/* Data Table:
    leColorInfoTable

  Summary:
    Color information reference table
*/
/**
 * @brief This array represents information reference table.
 * @details The reference table is used to hold color information.
 */
extern const leColorModeInfo leColorInfoTable[];

// *****************************************************************************
/* Structure:
    leColorName

  Summary:
    Color name reference table
*/
/**
 * @brief This enum represents predefined color options.
 * @details
 */
typedef enum leColorName
{
    LE_COLOR_BLACK,
    LE_COLOR_WHITE,
    LE_COLOR_RED,
    LE_COLOR_LIME,
    LE_COLOR_BLUE,
    LE_COLOR_YELLOW,
    LE_COLOR_CYAN,
    LE_COLOR_MAGENTA,
    LE_COLOR_SILVER,
    LE_COLOR_DARKGRAY,
    LE_COLOR_GRAY,
    LE_COLOR_LIGHTGRAY,
    LE_COLOR_MAROON,
    LE_COLOR_OLIVE,
    LE_COLOR_GREEN,
    LE_COLOR_PURPLE,
    LE_COLOR_TEAL,
    LE_COLOR_NAVY,
    LE_COLOR_LAST
} leColorName;

/**
 * @brief This struct represents a blend color lookup table.
 * @details A blend color lookup table is allows for a fast blending
 * result based on a lookup table versus having to perform the calculation
 * mathematically.
 */
typedef struct leBlendLookupTable
{
    leColor foreground;   /**< The table foreground color */
    leColor background;   /**< The table background color */
    leColorMode mode;     /**< The table color mode. */

    const void* data;     /**< Pointer to a buffer which holds the lookup values. */
} leBlendLookupTable;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leColor leColorValue(leColorMode mode, leColorName name)

  Summary:
    Used for getting a color value by name.

  Parameters:
    leColorMode - the color mode for the return type
    leColorName - the name of the color to retrieve
    
  Returns:
    leColor - the color value of the given name in the specified format
    
  Remarks:
    
*/
/**
 * @brief Get color by name and mode.
 * @details Gets a color by <span class="param">name</span>
 * in the format <span class="param">mode</span>.
 * @code
 * leColorMode mode;
 * leColorName name;
 * leColor color = leColorValue(mode, name);
 * @endcode
 * @parammode is the colormode required.
 * @paramname is the name to query
 * @return the color value of the given name in the specified format.
 */
leColor leColorValue(leColorMode mode, leColorName name);

// *****************************************************************************
/* Function:
    uint32_t leColorChannelRed(leColor clr, leColorMode mode)

  Summary:
    Used for getting the red color channel of a given color value.

  Description:
    

  Parameters:
    leColor - the source color value
    leColorMode - the source color mode
    
  Returns:
    uint32_t - the red color channel
    
  Remarks:
    
*/
/**
 * @brief Get red color channel.
 * @details Gets the red color channel from <span class="param">color</span>
 * in the format <span class="param">mode</span>.
 * @code
 * leColor clr;
 * leColorMode mode;
 * uint32_t red = leColorChannelRed(clr, mode);
 * @endcode
 * @param mode is the colormode required.
 * @param name is the name to query
 * @return the red color channel
 */
uint32_t leColorChannelRed(leColor clr, leColorMode mode);

// *****************************************************************************
/* Function:
    uint32_t leColorChannelGreen(leColor clr, leColorMode mode)

  Summary:
    Used for getting the green color channel of a given color value.

  Description:
    

  Parameters:
    leColor - the source color value
    leColorMode - the source color mode
    
  Returns:
    uint32_t - the green color channel
    
  Remarks:
    
*/
/**
 * @brief Get green color channel.
 * @details Gets the green color channel from <span class="param">color</span>
 * in the format <span class="param">mode</span>.
 * @code
 * leColor clr;
 * leColorMode mode;
 * uint32_t green = leColorChannelGreen(clr, mode);
 * @endcode
 * @paramclr is the color to query.
 * @parammode is the format.
 * @return the green color channel.
 */
uint32_t leColorChannelGreen(leColor clr, leColorMode mode);

// *****************************************************************************
/* Function:
    uint32_t leColorChannelBlue(leColor clr, leColorMode mode)

  Summary:
    Used for getting the blue color channel of a given color value.

  Description:
    

  Parameters:
    leColor - the source color value
    leColorMode - the source color mode
    
  Returns:
    uint32_t - the blue color channel
    
  Remarks:
    
*/
/**
 * @brief Get blue color channel.
 * @details Gets the blue color channel of type <span class="param">mode</span>
 * for the given <span class="param">color</span>.
 * @coded
 * leColor clr;
 * leColorMode mode;
 * uint32_t blue = leColorChannelBlue(clr, mode);
 * @endcode
 * @param mode is the colormode format.
 * @param name is the name to query.
 * @return the blue color channel.
 */
uint32_t leColorChannelBlue(leColor clr, leColorMode mode);

// *****************************************************************************
/* Function:
    uint32_t leColorChannelAlpha(leColor clr, leColorMode mode)

  Summary:
    Used for getting the alpha color channel of a given color value.

  Description:
    

  Parameters:
    leColor - the source color value
    leColorMode - the source color mode
    
  Returns:
    uint32_t - the alpha color channel
    
  Remarks:
    
*/
/**
 * @brief Get alpha color channel.
 * @details Gets the alpha color channel of type <span class="param">mode</span>
 * for the given <span class="param">color</span>.
 * @code
 * leColorMode mode;
 * leColorName name;
 * uint32_t alpha = leColorChannelAlpha(clr, mode);
 * @endcode
 * @param clr is the source color value.
 * @param mode is the source color mode
 * @return the alpha color channel.
 */
uint32_t leColorChannelAlpha(leColor clr, leColorMode mode);

// *****************************************************************************
/* Function:
    leColor leColorConvert(leColorMode mode_in,
                           leColorMode mode_out,
                           leColor color)

  Summary:
    Converts a color value from one mode to another


  Parameters:
    leColorMode - the input color mode
    leColorMode - the output color mode
    leColor - the source color
    
  Returns:
    leColor - the result color
    
  Remarks:
    
*/
/**
 * @brief Convert to color value.
 * @details Converts the color value of <span class="param">color</span>
 * from <span class="param">mode_in</span> to
 * <span class="param">mode_out</span>.
 * @code
 * leColorMode mode_in;
 * leColorName mode_out;
 * leColor color
 * leColor clr = leColorConvert(mode_in, mode_out, color);
 * @endcode
 * @param mode_in is the the source color.
 * @param mode_out is the source color mode.
 * @param color is the color to convert.
 * @return the result color.
 */
leColor leColorConvert(leColorMode mode_in,
                       leColorMode mode_out,
                       leColor color);

// *****************************************************************************
/* Function:
    leColor leColorBlend_RGBA_8888(leColor fore, leColor back)

  Summary:
    Blends two RGBA8888 colors together using their alpha channel values.

  Description:
    

  Parameters:
    leColor - the foreground color
    leColor - the background color
    
  Returns:
    leColor - the blended result color
    
  Remarks:
    
*/
/**
 * @brief Get color from RGBA blend
 * @details Gets a color from the blend of RGBA <span class="param">fore</span>
 * and RGBA <span class="param">back</span>.
 * @code
 * leColor fore;
 * leColor back;
 * leColor blend = leColorBlend_RGBA_8888(fore, back);
 * @endcode
 * @param fore is the foreground color.
 * @param back is the background color
 * @return the blended result color.
 */
leColor leColorBlend_RGBA_8888(leColor fore, leColor back);

// *****************************************************************************
/* Function:
    leColor leColorBlend_ARGB_8888(leColor fore, leColor back)

  Summary:
    Blends two ARGB8888 colors together using their alpha channel values.

  Description:


  Parameters:
    leColor - the foreground color
    leColor - the background color

  Returns:
    leColor - the blended result color

  Remarks:

*/
/**
 * @brief Get color from ARGB blend
 * @details Gets a color from the blend of ARGB <span class="param">fore</span>
 * and RGBA <span class="param">back</span>.
 * @code
 * leColor fore;
 * leColor back;
 * leColor blend = leColorBlend_ARGB_8888(fore, back);
 * @endcode
 * @param fore is the foreground color.
 * @param back is the background color
 * @return the blended result color.
 */
leColor leColorBlend_ARGB_8888(leColor fore, leColor back);

// *****************************************************************************
/* Function:
    leColor leColorLerp(leColor l,
                        leColor r,
                        uint32_t percent,
                        leColorMode mode)

  Summary:
    Linear interpolation between two colors

  Parameters:
    leColor - first color input
    leColor - second color input
    uint32_t - percentage of interpolation [0-100]
    leColorMode - input color mode
    
  Returns:
    leColor - the result color
    
  Remarks:
    
*/
/**
 * @brief Get color from linear interpolate of two colors
 * @details Gets a color from the linear interpolation of
 * <span class="param">l</span>
 * and <span class="param">r</span> with
 * percentage of interpolation specified by
 * <span class="param">percent</span> and color format
 * <span class="param">mode</span>
 * @code
 * leColor l;
 * leColor r;
 * uint32_t percent;
 * leColorMode mode;
 * leColor blend = leColorLerp(l, r, percent, mode);
 * @endcode
 * @param fore is the foreground color.
 * @param back is the background color
 * @return the result color.
 */
leColor leColorLerp(leColor l,
                    leColor r,
                    uint32_t percent,
                    leColorMode mode);

// *****************************************************************************
/* Function:
    leColor leColorBilerp(leColor c00,
                          leColor c01,
                          leColor c10,
                          leColor c11,
                          uint32_t xper,
                          uint32_t yper,
                          leColorMode mode)

  Summary:
    Calculates bilinear interpolation between four colors

  Parameters:
    leColor c00 - top left color input
    leColor c01 - top right color input
    leColor c10 - bottom left color input
    leColor c11 - bottom right color input
    uint32_t xper - percentage of interpolation in x [0-100]
    uint32_t yper - percentage of interpolation in y [0-100]
    leColorMode - input color mode
    
  Returns:
    leColor - the result color
    
  Remarks:
    
*/
/**
 * @brief Get color from bi-linear interpolation of four colors.
 * @details Gets a color from the bilinear interpolation of
 * <span class="param">c00</span>,
 * <span class="param">c01</span>,
 * <span class="param">c10</span>, and
 * <span class="param">c11</span>.  The percentage of interpolation
 * of x and y is specified by <span class="param">xper</span>
 * <span class="param">yper</span>. The color format
 * is <span class="param">mode</span>.
 * @code
 * @endcode
 * @param c00 is the top left color input.
 * @param c01 is the top right color input.
 * @param c10 is the bottom left color input.
 * @param c11 is the bottom right color input.
 * @param xper is the percentage of interpolation in x [0-100].
 * @param yper is the percentage of interpolation in y [0-100].
 * @param mode is the input color mode.
 * @return the result color.
 */
leColor leColorBilerp(leColor c00,
                      leColor c01,
                      leColor c10,
                      leColor c11,
                      uint32_t xper,
                      uint32_t yper,
                      leColorMode mode);

/**
 * @brief Swaps the red and blue channels for a given color value.
 * @details Swaps the red and blue channels for a given color value.
 *  This can change an RGB color to BGR and vice versa.
 * @code
 * @endcode
 * @param clr is the color value.
 * @param mode is the color value mode.
 * @return the result color.
 */
leColor leColorSwap(leColor clr,
                    leColorMode mode);

#ifdef __cplusplus
}
#endif

#endif /* LE_COLOR_H */
