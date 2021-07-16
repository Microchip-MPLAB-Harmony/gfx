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
    legato_image.h

  Summary:
    Defines image assets

  Description:
    Image drawing at specified coordinates
*******************************************************************************/

/** \file legato_image.h
 * @brief Image functions and defintions.
 *
 * @details Image drawing at specified coordinates.
 */

#ifndef LE_IMAGE_H
#define LE_IMAGE_H

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
/**
 * @brief This enum represents an image format.
 * @details Image format is used to list supported image encodings.
 */
typedef enum leImageFormat
{
    LE_IMAGE_FORMAT_RAW = 0,
    LE_IMAGE_FORMAT_RLE,
    LE_IMAGE_FORMAT_JPEG,
    LE_IMAGE_FORMAT_PNG,
    LE_IMAGE_FORMAT_MONO
} leImageFormat;

#define LE_IMAGE_FORMAT_COUNT (LE_IMAGE_FORMAT_RLE + 1)

// *****************************************************************************
/* Enumeration:
    leImageFlags

  Summary:
    A list of flags describing an image asset
*/
/**
 * @brief This enum represents image flags.
 * @details Image flags are used to describe an image asset.
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
/**
 * @brief This enum represents image filter modes.
 * @details Image filter modes are used to list supported image filters.
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
/**
 * @brief This struct represents an image map.
 * @details An image map is used to describe an image map buffer.
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
/**
 * @brief This struct represents an image.
 * @details An image is used to describe an image asset.
 */
struct leImage;

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

    struct leImage* palette;
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
/**
 * @brief Create an image.
 * @details Creates an image <span class="param">img</span>
 * given <span class="param">width</span>, <span class="param">height</span>,
 * and <span class="param">mode</span>. The image is located at
 * <span class="param">data</span> with <span class="param">locationID</span>.
 * @code
 * leResult res = leImage_Create();
 * @endcode
 * @param img the image object to initialize
 * @param width the width of the image
 * @param height the height of the image
 * @param mode the color mode of the image
 * @param data the data address of the image
 * @param locationID the location ID of the image
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leImage_Create(leImage* img,
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
/**
 * @brief Allocate an image buffer.
 * @details Dynamically allocates an image buffer in local memory
 * using <span class="param">width</span> and <span class="param">height</span>
 * color mode <span class="param">mode</span>.
 * @code
 * uint32_t width;
 * uint32_t height;
 * leColorMode mode;
 * leImage* img = leImage_Allocate();
 * @endcode
 * @param width the width of the image in pixels
 * @param height the height of the image in pixels
 * @param mode the color mode of the image
 * @return a valid image or null if there wasn't enough memory for the allocation
 */
leImage* leImage_Allocate(uint32_t width,
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
/**
 * @brief Free image buffer.
 * @details Frees an image buffer <span class="param">img</span>.
 * @remark Buffer must be allocated using leImage_Allocate.
 * @code
 * leImage* img;
 * leResult res = leImage_Free(img);
 * @endcode
 * @param img the image to free
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leImage_Free(leImage* img);

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
/**
 * @brief This struct represents an image decoder.
 * @details Structure defining a general image decoder object.  Specific decoders will
 * implement this in their own way.
 */
typedef struct leImageDecoder
{
    leBool   (*supportsImage)(const leImage* img);
    leResult (*draw)(const leImage* img, const leRect* srcRect, int32_t x, int32_t y, uint32_t a);
    leResult (*copy)(const leImage* src, const leRect* srcRect, int32_t x, int32_t y, leImage* dst);
    leResult (*render)(const leImage* src, const leRect* srcRect, int32_t x, int32_t y, leBool ignoreMask, leBool ignoreAlpha, leImage* dst);
    leResult (*resize)(const leImage* src, const leRect* srcRect, leImageFilterMode mode, uint32_t sizeX, uint32_t sizeY, leImage* dst);
    leResult (*resizeDraw)(const leImage* src, const leRect* srcRect, leImageFilterMode mode, uint32_t sizeX, uint32_t sizeY, int32_t x, int32_t y, uint32_t a);
    leResult (*rotate)(const leImage* src, const leRect* srcRect, leImageFilterMode mode, int32_t angle, leImage** dst, leBool alloc);
    leResult (*rotateDraw)(const leImage* src, const leRect* srcRect, leImageFilterMode mode, int32_t angle, int32_t x, int32_t y, uint32_t a);
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
/**
 * @brief Get Event Count.
 * @details Returns the number of events listed in the current context.
 * @code
 * uint32_t cnt = leEvent_GetCount();
 * @endcode
 * @param void.
 * @return number of events.
 */
void leImage_InitDecoders(void);

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
/**
 * @brief Draw an Image.
 * @details Draws an image <span class="param">img</span> bounded by
 * <span class="param">sourceRect</span> to location
 * <span class="param">x</span> and <span class="param">y</span>.
 * @code
 * leImage * fnptr;
 * leResult res = leImage_Draw(img);
 * @endcode
 * @param img is the image to draw.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leImage_Draw(const leImage* img,
                      const leRect* sourceRect,
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
/**
 * @brief Resize image.
 * @details Resizes the source image <span class="param">src</span> to
 * destination image <span class="param">dst</span>
 * to location <span class="param">x</span> and <span class="param">y</span>.
 * using the specified  filter <span class="param">mode</span>.
 * @code
 * leImage * src;
 * leResult res = leImage_Resize(src);
 * @endcode
 * @param src is the image to resize.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leImage_Resize(const leImage* src,
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
/**
 * @brief Resize draw image.
 * @details Resizes the source image <span class="param">src</span> to
 * destination image <span class="param">dst</span>
 * to location <span class="param">x</span> and <span class="param">y</span>.
 * using the specified  filter <span class="param">mode</span>.
 * @code
 * leImage * src;
 * leResult res = fnptr->resizeDraw(src, sourceRect, mode, sizeX, sizeY, x, y, a);
 * @endcode
 * @param src is the image to resize.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leImage_ResizeDraw(const leImage* src,
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
/**
 * @brief Copy image.
 * @details Copies <span class="param">img</span> bounded by
 * <span class="param">srcRect</span> to location
 * <span class="param">x</span> and <span class="param">y</span>.
 * @code
 * leImage* src;
 * leRect* sourceRect;
 * int32_t x;
 * int32_t y;
 * leImage* dst;
 * leResult res = leImage_Copy(src, sourceRect, x, y, dst);
 * @endcode
 * @param src pointer to source image asset to draw
 * @param sourceRect the source rectangle of the image to decode
 * @param x the x position
 * @param y the y position
 * @param dst the destination image to fill
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leImage_Copy(const leImage* src,
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
/**
 * @brief Render image.
 * @details Copies <span class="param">img</span> bounded by
 * <span class="param">srcRect</span> to location
 * <span class="param">x</span> and <span class="param">y</span>.
 * The flag <span class="param">ignoreMask</span> determines if the masking color should
 * be performed. The flag <span class="param">ignoreAlpha</span> determines
 * if the alpha blending is performed.
 * @code
 * leImage * src;
 * leResult res = leImage_Render(src);
 * @endcode
 * @param src is the image to render.
 * @param sourceRect the source rectangle
 * @param x the x position
 * @param y the y position
 * @param ignoreMask set to true to skip the mask stage for the source image
 * @param ignoreAlpha the destination image to fill
 * @param dst  set to true to skip the blend stage for the source image
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leImage_Render(const leImage* src,
                        const leRect* sourceRect,
                        int32_t x,
                        int32_t y,
                        leBool ignoreMask,
                        leBool ignoreAlpha,
                        leImage* dst);

/**
 * @brief Rotate image.
 * @details Rotate image <span class="param">src</span> bounded by
 * <span class="param">srcRect</span> by <span class="param">angle</span>
 * degress. The resulting image is <span class="param">dst</span>  The
 * result image is automatically allocated and should be freed by the
 * caller.
 * @code
 * leImage * fnptr;
 * leResult res = leImage_Rotate(src);
 * @endcode
 * @param src is the image to render.
 * @param sourceRect the source rectangle
 * @param mode the the filter mode
 * @param angle the angle to rotate by (positive is counter clockwise)
 * @param dst the destination image to fill
 * @param alloc true if the decoder should automatically allocate the
 *              destination image
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leImage_Rotate(const leImage* src,
                        const leRect* sourceRect,
                        leImageFilterMode mode,
                        int32_t angle,
                        leImage** dst,
                        leBool alloc);

// *****************************************************************************
/* Function:
    leResult leImage_Rotate(const leImage* src,
                            const leRect* sourceRect,
                            leImageFilterMode mode,
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
    int32_t angle - the angle (degrees) to rotate by
    int32_t x - the X coordinate to draw to
    int32_t y - the Y coordinate to draw to
    uint32_t a - the alpha value to use

  Returns:
    leResult
*/
/**
 * @brief Rotate draw image.
 * @details Rotate image <span class="param">src</span> bounded by
 * <span class="param">srcRect</span> around
 * <span class="param">origin</span> at <span class="param">angle</span>
 * degress. The resulting image is <span class="param">dst</span>.
 * @code
 * leImage * src;
 * leResult res = leImage_RotateDraw(src);
 * @endcode
 * @param src is the image to render.
 * @param sourceRect the source rectangle
 * @param angle set to true to skip the mask stage for the source image
 * @param x the destination image to fill
 * @param y is the image to resize.
 * @param a the alpha value to use
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leImage_RotateDraw(const leImage* src,
                            const leRect* sourceRect,
                            leImageFilterMode mode,
                            int32_t angle,
                            int32_t x,
                            int32_t y,
                            uint32_t a);


/**
 * @brief Process an image to another location.
 * @details Processes an image from one location (typically an external source)
 * to an internally accessible memory address.  The source image <span class="param">img</span>
 * will be uncompressed and rendered in a raw format to the address
 * <span class="param">addr</span> using the color mode <span class="param">mode</span>.
 * This operation is typically used to transform an image into a GPU-compatible
 * state.
 * @code
 * leImage* img;
 * uint32_t addr = 0xA0000000;
 * leColorMode mode = LE_COLOR_MODE_RGBA8888;
 * leProcessImage(img, addr, mode);
 * @endcode
 * @param img the source image to process.
 * @param addr the address of the location for the new image data
 * @param mode the new color mode of the image
 */
void leProcessImage(leImage* img,
                    size_t addr,
                    leColorMode mode);

#endif /* LE_IMAGE_H */
