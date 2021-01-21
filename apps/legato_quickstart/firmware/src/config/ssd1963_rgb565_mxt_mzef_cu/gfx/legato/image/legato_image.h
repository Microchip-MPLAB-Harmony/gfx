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
    legato_image.h

  Summary:
    Defines image assets

  Description:
    Image drawing at specified coordinates
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LE_IMAGE_H
#define LE_IMAGE_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#include "gfx/legato/common/legato_color.h"
#include "gfx/legato/common/legato_pixelbuffer.h"
#include "gfx/legato/core/legato_stream.h"

typedef struct lePalette lePalette;

// *****************************************************************************
/* Enumeration:
    leImageFormat

  Summary:
    Indicates the image encoding format
*/
typedef enum leImageFormat
{
    LE_IMAGE_FORMAT_RAW = 0,
    LE_IMAGE_FORMAT_RLE,
    //LE_IMAGE_FORMAT_JPEG,
    //LE_IMAGE_FORMAT_PNG
} leImageFormat;

#define LE_IMAGE_FORMAT_COUNT (LE_IMAGE_FORMAT_RLE + 1)

// *****************************************************************************
/* Enumeration:
    leImageFlags

  Summary:
    A list of flags describing an image asset
*/
typedef enum leImageFlags
{
    LE_IMAGE_USE_MASK_COLOR  = 1 << 0,
    LE_IMAGE_USE_MASK_MAP    = 1 << 1,
    LE_IMAGE_USE_ALPHA_MAP   = 1 << 2,
    LE_IMAGE_INTERNAL_ALLOC  = 1 << 3
} leImageFlags;

// *****************************************************************************
/* Enumeration:
    leImageFilterMode

  Summary:
    A list of flags defining image filtering modes
*/
typedef enum leImageFilterMode
{
    LE_IMAGEFILTER_NONE,
    LE_IMAGEFILTER_NEAREST_NEIGHBOR,
    LE_IMAGEFILTER_BILINEAR,
} leImageFilterMode;

// *****************************************************************************
/* Structure:
    struct leImageMap

  Summary:
    Structure defining a general image map buffer.
*/
typedef struct leImageMap
{
    leStreamDescriptor header; // standard stream header
    lePixelBuffer buffer; // the buffer that describes this map
} leImageMap;

// *****************************************************************************
/* Structure:
    leImage

  Summary:
    Describes an image asset.
    
    header - standard asset header
    format - the format of the image.  this directly affects which decoder
             is invoked when rendering the image
    width - the width of the image in pixels
    height - the height of the image in pixels
    colorMode - the color mode of the image
    compType - the compression mode of the image
    flags - indicates of the mask field is used
    mask - may contain a masking color for the image.  blit operations may 
           reference this value and reject image pixels that match this
           value.  This can either be a single color mode or a pointer to an
           image map data array.

    alphaMask - pointer to an array of per-pixel alpha blending values

    palette - will contain a valid pointer to a palette asset if thie image
              is an index map instead of a color image

    leStreamDescriptor header - standard stream header
    format - the format of the image.  this directly affects which decoder
             is invoked when rendering the image
    lePixelBuffer buffer - the buffer that describes this image data
    leImageFlags flags - image descriptor flags

    union
    {
        leColor color; // single masking color
        leImageMap* map; // color mask map
    } mask; // defines mask techniques

    leImageMap* alphaMap; // placeholder for blending mask

    lePalette* palette; // lookup palette for this image
*/
typedef struct leImage
{
    leStreamDescriptor header;
    leImageFormat format;
    lePixelBuffer buffer;
    leImageFlags flags;

    union
    {
        leColor color;
        leImageMap* map;
    } mask;

    leImageMap* alphaMap;

    lePalette* palette;
} leImage;

// *****************************************************************************
/* Function:
    leResult leImage_Create(leImage* img,
                            uint32_t width,
                            uint32_t height,
                            leColorMode mode,
                            void* data,
                            uint32_t locationID)

   Summary:
    Initializes a leImage pointer

   Description:
    Initializes a leImage pointer

   Parameters:
    leImage* img - the image object to initialize
    uint32_t width - the width of the image
    uint32_t height - the height of the image
    leColorMode mode - the color mode of the image
    void* data - the data address of the image
    uint32_t locationID - the location ID of the image

  Returns:

  Remarks:
*/
LIB_EXPORT leResult leImage_Create(leImage* img,
                                   uint32_t width,
                                   uint32_t height,
                                   leColorMode mode,
                                   void* data,
                                   uint32_t locationID);

// *****************************************************************************
/* Function:
    leImage* leImage_Allocate(uint32_t width,
                              uint32_t height,
                              leColorMode mode)

   Summary:
    Dynamically allocates an image buffer in local memory using the
    given parameters.

   Description:
    Dynamically allocates an image buffer in local memory using the
    given parameters.  This uses the library's internal allocator and
    memory pools.

   Parameters:
    uint32_t width - the width of the image in pixels
    uint32_t height - the height of the image in pixels
    leColorMode mode - the color mode of the image

  Returns:
    leImage* - a valid image or null if there wasn't enough memory for the
               allocation
  Remarks:
*/
LIB_EXPORT leImage* leImage_Allocate(uint32_t width,
                                     uint32_t height,
                                     leColorMode mode);

// *****************************************************************************
/* Function:
    leResult leImage_Free(leImage* img)

   Summary:
    Frees an image buffer that was allocated using leImage_Allocate.

   Description:
    Frees an image buffer that was allocated using leImage_Allocate.

   Parameters:
    leImage* img - the image to free

  Returns:

  Remarks:
*/
LIB_EXPORT leResult leImage_Free(leImage* img);

// *****************************************************************************
/* Structure:
    struct leImageDecoder

  Summary:
    Structure defining a general image decoder object.  Specific decoders
    will implement this in their own way.

    supportsImage - queries the decoder to see if it supports a given image
    draw - initializes the decoder to draw an image to the frame buffer
    copy - intiailizes the decoder to perform an image copy operation
    render - initializes the decoder to perform a direct image render operation
    resize - initialies the decoder to perform an direct image resize operation
    resize - initialies the decoder to perform an image resize operation draw to the frame buffer
    exec - run the decoder
    isDone - query the decoder for completion
    free - frees the decoder to cleanup any allocated resources
*/
typedef struct leImageDecoder
{
    leBool   (*supportsImage)(const leImage* img);
    leResult (*draw)(const leImage* img, const leRect* srcRect, int32_t x, int32_t y, uint32_t a);
    leResult (*copy)(const leImage* src, const leRect* srcRect, int32_t x, int32_t y, leImage* dst);
    leResult (*render)(const leImage* src, const leRect* srcRect, int32_t x, int32_t y, leBool ignoreMask, leBool ignoreAlpha, leImage* dst);
    leResult (*resize)(const leImage* src, const leRect* srcRect, leImageFilterMode mode, uint32_t sizeX, uint32_t sizeY, leImage* dst);
    leResult (*resizeDraw)(const leImage* src, const leRect* srcRect, leImageFilterMode mode, uint32_t sizeX, uint32_t sizeY, int32_t x, int32_t y, uint32_t a);
    leResult (*rotate)(const leImage* src, const leRect* srcRect, leImageFilterMode mode, const lePoint* origin, int32_t angle, leImage* dst);
    leResult (*rotateDraw)(const leImage* src, const leRect* srcRect, leImageFilterMode mode, const lePoint* origin, int32_t angle, int32_t x, int32_t y, uint32_t a);
    leResult (*exec)(void);
    leBool   (*isDone)(void);
    void     (*free)(void);
} leImageDecoder;

// *****************************************************************************
/* Function:
    void leImage_InitDecoders()

   Summary:
    Global function to initialize all image decoders.  INTERNAL USE ONLY

   Description:
    Global function to initialize all image decoders.  INTERNAL USE ONLY

   Parameters:

  Returns:

  Remarks:
*/
void leImage_InitDecoders();

#if LE_STREAMING_ENABLED == 1
// *****************************************************************************
/* Structure:
    struct leImageStreamDecoder

  Summary:
    Defines the base implementation for a streaming image decoder.
*/
typedef struct leImageStreamDecoder
{
    leStreamManager base;
} leImageStreamDecoder;
#endif

// *****************************************************************************
/* Function:
    leResult leDrawImage(void);

  Summary:
    Draws a portion of the given image at the specified coordinates.
     
  Parameters:
    leImage* img - pointer to the image asset to draw
    leRect* sourceRect - the source rectangle of the image to blit   
    int32_t x - the x position to draw to
    int32_t x - the y position to draw to
    uint32_t a - global alpha amount to apply

  Returns:
    leResult
*/
LIB_EXPORT leResult leImage_Draw(const leImage* img,
                                 leRect* sourceRect,
                                 int32_t x,
                                 int32_t y,
                                 uint32_t a);


// *****************************************************************************
/* Function:
    leResult leImage_Resize(const leImage* src,
                            const leRect* sourceRect,
                            leImageFilterMode mode,
                            leImage* dst);

  Summary:
    Decodes a portion of the given image at the specified coordinates and
    scales it to the given dimensions using the specified filter mode.  The
    result is stored into the provided destination image pointer.

  Parameters:
    leImage* src - pointer to source image asset to draw
    leRect* sourceRect - the source rectangle of the image to decode
    leImageFilterMode mode - the filter to use when resizing
    leImage* dst - pointer to destination image asset

  Returns:
    leResult
*/
LIB_EXPORT leResult leImage_Resize(const leImage* src,
                                   const leRect* sourceRect,
                                   leImageFilterMode mode,
                                   uint32_t sizeX,
                                   uint32_t sizeY,
                                   leImage* target);

// *****************************************************************************
/* Function:
    leResult leImage_ResizeDraw(const leImage* src,
                                const leRect* sourceRect,
                                leImageFilterMode mode,
                                const leRect* destRect);

  Summary:
    Decodes a portion of the given image at the specified coordinates and
    scales it to the given dimensions using the specified filter mode.  The
    result is written to the active scratch buffer.

  Parameters:
    leImage* src - pointer to source image asset to draw
    const leRect* sourceRect - the source rectangle of the image to decode
    leImageFilterMode mode - the filter to use when resizing
    int32_t x - the X coordinate to draw to
    int32_t y - the Y coordinate to draw to
    uint32_t a - the alpha value to use

  Returns:
    leResult
*/
LIB_EXPORT leResult leImage_ResizeDraw(const leImage* src,
                                       const leRect* sourceRect,
                                       leImageFilterMode mode,
                                       uint32_t sizeX,
                                       uint32_t sizeY,
                                       int32_t x,
                                       int32_t y,
                                       uint32_t a);

// *****************************************************************************
/* Function:
    leResult leImage_Copy(const leImage* src,
                          const leRect* sourceRect,
                          int32_t x,
                          int32_t y,
                          leImage* dst);

  Summary:
    Copies the data from one image to another.

  Parameters:
    leImage* src - pointer to source image asset to draw
    const leRect* sourceRect - the source rectangle of the image to decode
    int32_t x - the x position of the destination image to write to
    int32_t y - the y position of the desgination image to write to,
    leImage* dst - the destination image to fill

  Returns:
    leResult
*/
LIB_EXPORT leResult leImage_Copy(const leImage* src,
                                 const leRect* sourceRect,
                                 int32_t x,
                                 int32_t y,
                                 leImage* dst);

// *****************************************************************************
/* Function:
    leResult leImage_Render(const leImage* src,
                            const leRect* sourceRect,
                            int32_t x,
                            int32_t y,
                            leBool ignoreMask,
                            leBool ignoreAlpha,
                            leImage* dst);

  Summary:
    Renders an image into another image.  Source data will be decoded,
    transformed, and written into the destination image.  The source image
    mask and alpha data may be optionally ignored.

  Parameters:
    leImage* src - pointer to source image asset to draw
    const leRect* sourceRect - the source rectangle of the image to decode
    int32_t x - the x position of the destination image to write to
    int32_t y - the y position of the desgination image to write to
    leBool ignoreMask - set to true to skip the mask stage for the source image
    leBool ignoreAlpha - set to true to skip the blend stage for the source image
    leImage* dst - the destination image to fill

  Returns:
    leResult
*/
LIB_EXPORT leResult leImage_Render(const leImage* src,
                                   const leRect* sourceRect,
                                   int32_t x,
                                   int32_t y,
                                   leBool ignoreMask,
                                   leBool ignoreAlpha,
                                   leImage* dst);

// *****************************************************************************
/* Function:
    leResult leImage_Rotate(const leImage* src,
                            const leRect* sourceRect,
                            leImageFilterMode mode,
                            const lePoint* origin,
                            int32_t angle,
                            leImage* dst);

  Summary:
    Decodes a portion of the given image at the specified coordinates and
    rotates it by the given angle in degrees, around the origin point,
    using the specified filter mode.

    The result is stored into the provided destination image pointer.

  Parameters:
    leImage* src - pointer to source image asset to draw
    leRect* sourceRect - the source rectangle of the image to decode
    leImageFilterMode mode - the filter to use when rotating
    const lePoint* origin - the point to rotate around in image space
    int32_t angle - the angle (degrees) to rotate by
    leImage* dst - pointer to destination image asset

  Returns:
    leResult
*/
LIB_EXPORT leResult leImage_Rotate(const leImage* src,
                                   const leRect* sourceRect,
                                   leImageFilterMode mode,
                                   const lePoint* origin,
                                   int32_t angle,
                                   leImage* dst);

// *****************************************************************************
/* Function:
    leResult leImage_Rotate(const leImage* src,
                            const leRect* sourceRect,
                            leImageFilterMode mode,
                            const lePoint* origin,
                            int32_t angle,
                            int32_t x,
                            int32_t y,
                            uint32_t a);

  Summary:
    Decodes a portion of the given image at the specified coordinates and
    rotates it by the given angle in degrees, around the origin point,
    using the specified filter mode.

    The result is rendered directly into the frame buffer.

  Parameters:
    leImage* src - pointer to source image asset to draw
    leRect* sourceRect - the source rectangle of the image to decode
    leImageFilterMode mode - the filter to use when rotating
    const lePoint* origin - the point to rotate around in image space
    int32_t angle - the angle (degrees) to rotate by
    int32_t x - the X coordinate to draw to
    int32_t y - the Y coordinate to draw to
    uint32_t a - the alpha value to use

  Returns:
    leResult
*/
LIB_EXPORT leResult leImage_RotateDraw(const leImage* src,
                                       const leRect* sourceRect,
                                       leImageFilterMode mode,
                                       const lePoint* origin,
                                       int32_t angle,
                                       int32_t x,
                                       int32_t y,
                                       uint32_t a);

#endif /* LE_IMAGE_H */