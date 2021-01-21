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

//DOM-IGNORE-BEGIN
#ifndef GFX_DRIVER_H
#define GFX_DRIVER_H
//DOM-IGNORE-END

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


// DOM-IGNORE-BEGIN
#ifdef __cplusplus
    extern "C" {
#endif
// DOM-IGNORE-END
        
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
typedef struct gfxPoint
{
    int32_t x;
    int32_t y;
} gfxPoint;

static const gfxPoint gfxPoint_Zero = {0, 0};

// *****************************************************************************
/* Structure:
    gfxSize

  Summary:
    A two dimensional indication of size.  Values are signed but should never be
    negative.
*/
typedef struct gfxSize
{
    int32_t width;
    int32_t height;
} gfxSize;

static const gfxSize gfxSize_Zero = {0, 0};

// *****************************************************************************
/* Structure:
    gfxRect

  Summary:
    A rectangle definition.  
*/
typedef struct gfxRect
{
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
} gfxRect;

typedef void* gfxBuffer;

// *****************************************************************************
/* Enumeration:
    gfxColorMask

  Summary:
    Maskable list of color valies.

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
extern const gfxColorModeInfo gfxColorInfoTable[];


// *****************************************************************************
/* Structure:
    gfxBlend

  Summary:
    Blend name reference table
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
typedef struct gfxPixelBuffer
{
    gfxColorMode mode;
    
    gfxSize size;
    uint32_t pixel_count;
    
    uint32_t buffer_length;
    gfxBuffer pixels;

    uint32_t flags;
} gfxPixelBuffer;


//DOM-IGNORE-BEGIN
static const gfxRect gfxRect_Zero = {0, 0, 0, 0};

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
gfxColor gfxPixelBufferGetIndex_Unsafe(const gfxPixelBuffer* const buffer,
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
gfxResult gfxPixelBuffer_SetLocked(gfxPixelBuffer* buffer,
                                   gfxBool locked);




// *****************************************************************************
/* Structure:
    struct gfxDisplayDriver

  Summary:
    Defines the interface for a Legato display driver.  All drivers must,
    at a minimum, implement these interfaces

    getColorMode - returns the supported color mode for the driver

    getBufferCount - returns the number of buffers the driver is configured to use

    getDisplayWidth - returns the width of the driver frame buffer

    getDisplayHeight - returns the height of the driver frame buffer

    update - the driver tasks/update function

    getLayerCount - the number of hardware layers the driver supports

    getActiveLayer - the current active hardware layer

    setActiveLayer - sets the current active hardware layer
                     all buffer writes should go to this layer

    blitBuffer - instructs the driver to blit a buffer (buf) at
                 location (x, y)

    swap - instructs the driver to swap its buffer chain

    getVSYNCCount - queries the driver for its VSYNC counter
                    if a driver implements this counter this value
                    can be used to do frame rate calculations

*/
typedef struct gfxDisplayDriver
{
    gfxColorMode (*getColorMode)(void);

    uint32_t (*getBufferCount)(void);

    uint32_t (*getDisplayWidth)(void);

    uint32_t (*getDisplayHeight)(void);

    void (*update)(void);

    uint32_t (*getLayerCount)(void);

    uint32_t (*getActiveLayer)(void);

    gfxResult (*setActiveLayer)(uint32_t idx);

    gfxResult (*blitBuffer)(int32_t x,
                           int32_t y,
                           gfxPixelBuffer* buf,
                           gfxBlend gfx);

    void (*swap)(void);

    uint32_t (*getVSYNCCount)(void);

    gfxPixelBuffer* (*getFrameBuffer)(int32_t idx);

} gfxDisplayDriver;

// *****************************************************************************
/* Structure:
    struct gfxGraphicsProcessor

  Summary:
    Defines the interface for a Legato display driver.  All drivers must,
    at a minimum, implement these interfaces

    getColorMode - returns the supported color mode for the driver

    getBufferCount - returns the number of buffers the driver is configured to use

    getDisplayWidth - returns the width of the driver frame buffer

    getDisplayHeight - returns the height of the driver frame buffer

    update - the driver tasks/update function

    getLayerCount - the number of hardware layers the driver supports

    getActiveLayer - the current active hardware layer

    setActiveLayer - sets the current active hardware layer
                     all buffer writes should go to this layer

    blitBuffer - instructs the driver to blit a buffer (buf) at
                 location (x, y)

    swap - instructs the driver to swap its buffer chain

    getVSYNCCount - queries the driver for its VSYNC counter
                    if a driver implements this counter this value
                    can be used to do frame rate calculations

*/
typedef struct gfxGraphicsProcessor
{
    gfxResult (*drawLine)(gfxPixelBuffer * dest,
                        const gfxPoint* p1,
                        const gfxPoint* p2,
                           const gfxRect* clipRect,
                           const gfxColor color,
                        const gfxBlend blend);

    gfxResult (*fillRect)(gfxPixelBuffer * dest,
                        const gfxRect* clipRect,
                        const gfxColor color,
                        const gfxBlend blend);

    gfxResult (*blitBuffer)(const gfxPixelBuffer* source,
                        const gfxRect* srcRect,
                        const gfxPixelBuffer* dest,
                        const gfxRect* destRect,
                        const gfxBlend blend);

    gfxResult (*blitStretchBuffer)(const gfxPixelBuffer* source,
                        const gfxRect* srcRect,
                        const gfxPixelBuffer* dest,
                        const gfxRect* destRect,
                        const gfxBlend blend);

} gfxGraphicsProcessor;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
    }
#endif
// DOM-IGNORE-END
     
#endif // GFX_DRIVER_H

/*******************************************************************************
 End of File
*/
