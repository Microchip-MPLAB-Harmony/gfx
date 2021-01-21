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

// DOM-IGNORE-BEGIN

#ifndef LE_COLOR_H
#define LE_COLOR_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

// DOM-IGNORE-BEGIN
typedef uint32_t               leColor;

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

//DOM-IGNORE-END

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
typedef enum leColorMask
{
    LE_COLOR_MASK_GS_8      = 0x1,
    LE_COLOR_MASK_PALETTE   = 0x1,
    LE_COLOR_MASK_RGB_332   = 0x4,
    LE_COLOR_MASK_RGB_565   = 0x8,
    LE_COLOR_MASK_RGBA_5551 = 0x10,
    LE_COLOR_MASK_RGB_888   = 0x20,
    LE_COLOR_MASK_RGBA_8888 = 0x40,
    LE_COLOR_MASK_ARGB_8888 = 0x80,
    LE_COLOR_MASK_ALL = LE_COLOR_MASK_GS_8      |
                        LE_COLOR_MASK_RGB_332   |
                        LE_COLOR_MASK_RGB_565   |
                        LE_COLOR_MASK_RGBA_5551 |
                        LE_COLOR_MASK_RGB_888   | 
                        LE_COLOR_MASK_RGBA_8888 |
                        LE_COLOR_MASK_ARGB_8888
} leColorMask;

// *****************************************************************************
/* Enumeration:
    leColorMode

  Summary:
    List of available color modes.
*/
typedef enum leColorMode
{
    LE_COLOR_MODE_GS_8       = 0x0,
    LE_COLOR_MODE_PALETTE    = LE_COLOR_MODE_GS_8,
    LE_COLOR_MODE_RGB_332    = 0x1,
    LE_COLOR_MODE_RGB_565    = 0x2,
    LE_COLOR_MODE_RGBA_5551  = 0x3,
    LE_COLOR_MODE_RGB_888    = 0x4,
    LE_COLOR_MODE_RGBA_8888  = 0x5,
    LE_COLOR_MODE_ARGB_8888  = 0x6,
    LE_COLOR_MODE_INDEX_1    = 0x7,
    LE_COLOR_MODE_INDEX_4    = 0x8,
    LE_COLOR_MODE_INDEX_8    = 0x9,
    LE_COLOR_MODE_LAST = LE_COLOR_MODE_INDEX_8
} leColorMode;

#define LE_COLOR_MODE_LAST_COLOR     (LE_COLOR_MODE_ARGB_8888)
#define LE_COLOR_MODE_COUNT          (LE_COLOR_MODE_LAST + 1)

#define LE_COLOR_MODE_IS_PIXEL(mode) ((mode >= LE_COLOR_MODE_GS_8) && (mode <= LE_COLOR_MODE_ARGB_8888))
#define LE_COLOR_MODE_IS_INDEX(mode) ((mode >= LE_COLOR_MODE_INDEX_1) && (mode <= LE_COLOR_MODE_INDEX_8))

#define LE_COLOR_MODE_IS_ALPHA(mode) ((mode == LE_COLOR_MODE_RGBA_5551) || (mode == LE_COLOR_MODE_RGBA_8888) || (mode == LE_COLOR_MODE_ARGB_8888))

// *****************************************************************************
/* Enumeration:
    leBitsPerPixel

  Summary:
    List of available bits-per-pixel sizes.    
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
typedef struct leColorModeInfo
{
    uint32_t size;
    uint32_t bpp;
    leBitsPerPixel bppOrdinal;

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
    
} leColorModeInfo;

// *****************************************************************************
/* Data Table:
    leColorInfoTable

  Summary:
    Color information reference table
*/
extern const leColorModeInfo leColorInfoTable[];

// *****************************************************************************
/* Structure:
    leColorName

  Summary:
    Color name reference table
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
LIB_EXPORT leColor leColorValue(leColorMode mode, leColorName name);

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
LIB_EXPORT uint32_t leColorChannelRed(leColor clr, leColorMode mode);

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
LIB_EXPORT uint32_t leColorChannelGreen(leColor clr, leColorMode mode);

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
LIB_EXPORT uint32_t leColorChannelBlue(leColor clr, leColorMode mode);

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
LIB_EXPORT uint32_t leColorChannelAlpha(leColor clr, leColorMode mode); 

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
LIB_EXPORT leColor leColorConvert(leColorMode mode_in,
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
LIB_EXPORT leColor leColorBlend_RGBA_8888(leColor fore, leColor back);

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
LIB_EXPORT leColor leColorBlend_ARGB_8888(leColor fore, leColor back);

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
LIB_EXPORT leColor leColorLerp(leColor l,
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
LIB_EXPORT leColor leColorBilerp(leColor c00,
                                 leColor c01,
                                 leColor c10,
                                 leColor c11,
                                 uint32_t xper,
                                 uint32_t yper,
                                 leColorMode mode);                                   

#endif /* LE_COLOR_H */
