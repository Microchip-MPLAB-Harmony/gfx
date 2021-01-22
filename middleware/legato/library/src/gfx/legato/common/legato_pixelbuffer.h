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
    legato_pixelbuffer.h

  Summary:
    Defines a general purpose pixel buffer construct.

  Description:
    Pixel buffer generation and management functions.
*******************************************************************************/

/** \file legato_pixelbuffer.h
 * @brief Pixel Buffer functions and definitions.
 *
 * @details Pixel buffer generation and management functions.
 */

#ifndef LE_PIXELBUFFER_H
#define LE_PIXELBUFFER_H

#include "gfx/legato/common/legato_color.h"

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
/**
 * @brief leArray data structure definition.
 */
enum BufferFlags
{
    BF_NONE = 0,
    //BF_LOCKED = 1 << 0,
};

// *****************************************************************************
/* Structure:
    lePixelBuffer

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
typedef struct lePixelBuffer
{
    leColorMode mode;
    
    leSize size;
    uint32_t pixel_count;
    
    uint32_t buffer_length;
    leBuffer pixels;

    uint32_t reserved;
} lePixelBuffer;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leResult lePixelBufferCreate(const int32_t width,
                                 const int32_t height,
                                 const leColorMode mode,
                                 const void* const address,
                                 lePixelBuffer* buffer)

  Summary:
    Initializes a pixel buffer struct.  Does not actually allocate any memory.

  Parameters:
    const int32_t - the width of the buffer
    const int32_t - the height of the buffer
    const leColorMode - the color mode of the buffer
    const void* - the data addres of the buffer (may be NULL)
    lePixelBuffer* - pointer of the pixel buffer buffer to initialize
    
  Returns:
    leResult
    
  Remarks:
    
*/
/**
 * @brief Create a pixelbuffer
 * @details .
 * @code
 * lePixelBuffer* buffer;
 * leResult res = lePixelBufferCreate(buffer, x, y);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leResult.
 */
leResult lePixelBufferCreate(const int32_t width,
                             const int32_t height,
                             const leColorMode mode,
                             const void* const address,
                             lePixelBuffer* buffer);

// *****************************************************************************
/* Function:
    leBuffer lePixelBufferOffsetGet(const lePixelBuffer* const buffer,
                                    const lePoint* const pnt)

  Summary:
    Gets the offset address of the pixel that resides at the provided
    point in the given buffer.

  Parameters:
    const lePixelBuffer* - the source buffer
    const lePoint* - the point for which the offset should be calculated
    
  Returns:
    leBuffer - the pointer to the offset point in the source buffer
*/                                          
/**
 * @brief Get buffer at point
 * @details Gets the offset address of the pixel that resides at the
 * provided point in the given buffer.
 * @code
 * lePixelBuffer* buffer;
 * leColor color = lePixelBufferOffsetGet(buffer, x, y);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leBuffer.
 */
leBuffer lePixelBufferOffsetGet(const lePixelBuffer* const buffer,
                                uint32_t x,
                                uint32_t y);

// *****************************************************************************
/* Function:
    leBuffer lePixelBufferOffsetGet_Unsafe(const lePixelBuffer* const buffer,
                                               const lePoint* const pnt)

  Summary:
    Gets the offset address of the pixel that resides at the provided
    point in the given buffer.  Similar to lePixelBufferOffsetGet but performs
    no bounds checking.

  Parameters:
    const lePixelBuffer* - the source buffer
    const lePoint* - the point for which the offset should be calculated
    
  Returns:
    leBuffer - the pointer to the offset point in the source buffer
*/                                               
/**
 * @brief Get buffer at point
 * @details Gets the offset address of the pixel that resides at the provided
 * point in the given buffer.  Similar to lePixelBufferOffsetGet but performs
 * no bounds checking.
 * @code
 * lePixelBuffer* buffer;
 * leColor color = lePixelBufferOffsetGet_Unsafe(buffer, x, y);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leBuffer.
 */
leBuffer lePixelBufferOffsetGet_Unsafe(const lePixelBuffer* const buffer,
                                       uint32_t x,
                                       uint32_t y);

// *****************************************************************************
/* Function:
    leColor lePixelBufferGet(const lePixelBuffer* const buffer,
                                 const lePoint* const pnt)

  Summary:
    Gets the value of the pixel that resides at the provided point in
    the given buffer.

  Parameters:
    const lePixelBuffer* - the source buffer
    const lePoint* - the point for which the offset should be calculated
    
  Returns:
    leColor - the value of the pixel at the point in the source buffer
*/
/**
 * @brief Get pixel at point
 * @details Gets the value of the pixel that resides at the provided point
 * in the given buffer
 * @code
 * lePixelBuffer* buffer;
 * leColor color = lePixelBufferGet(buffer, x, y);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leColor.
 */
leColor lePixelBufferGet(const lePixelBuffer* const buffer,
                         uint32_t x,
                         uint32_t y);

// *****************************************************************************
/* Function:
    leColor lePixelBufferGet_Unsafe(const lePixelBuffer* const buffer,
                                    const lePoint* const pnt)

  Summary:
    Gets the value of the pixel that resides at the provided point in
    the given buffer.  Like lePixelBufferGet but performs no bounds checking.

  Parameters:
    const lePixelBuffer* - the source buffer
    const lePoint* - the point for which the offset should be calculated
    
  Returns:
    leColor - the value of the pixel at the point in the source buffer
*/
/**
 * @brief Get pixel at point
 * @details Gets the value of the pixel that resides at the provided point in
 * the given buffer.  Like lePixelBufferGet but performs no bounds checking.
 * @code
 * lePixelBuffer* buffer;
 * leColor color = lePixelBufferGetIndex_Unsafe(buffer, idx);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leResult.
 */
leColor lePixelBufferGet_Unsafe(const lePixelBuffer* const buffer,
                                uint32_t x,
                                uint32_t y);

/**
 * @brief Get color at index
 * @details Interprets the pixel buffer as a table of indices and looks up
 * a specific index at position 'idx'.  Indices may be 1bpp, 4bpp, or 8bpp
 * in size and are indicated by the color mode of the pixel buffer.
 * @code
 * lePixelBuffer* buffer;
 * leColor color = lePixelBufferGetIndex_Unsafe(buffer, idx);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leResult.
 */
leColor lePixelBufferGetIndex(const lePixelBuffer* const buffer,
                              const uint32_t idx);


/**
 * @brief Pixel get
 * @details A faster less-safe version of lePixelBufferGetIndex.
 * @code
 * lePixelBuffer* buffer;
 * leColor color = lePixelBufferGetIndex_Unsafe(buffer, idx);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leResult.
 */
leColor lePixelBufferGetIndex_Unsafe(const lePixelBuffer* const buffer,
                                     const uint32_t idx);

// *****************************************************************************
/* Function:
    leResult lePixelBufferClipRect(const lePixelBuffer* const buffer,
                                   const leRect* const rect,
                                   leRect* result)

  Summary:
    Clips a rectangle against a pixel buffer.  The result is guaranteed to fit
    inside the buffer's area.

  Parameters:
    const lePixelBuffer* const buffer - the source buffer
    const leRect* const - the rectangle to analyze
    leRect* result - the clipped rectangle
    
  Returns:
    leResult
*/                                             
/**
 * @brief Clip rectangle
 * @details Clips a rectangle against a pixel buffer.  The result is guaranteed to
 * fit inside the buffer's area.
 * @code
 * lePixelBuffer* buffer;
 * leResult res = lePixelBufferClipRect(buffer, x, y, color);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leResult.
 */
leResult lePixelBufferClipRect(const lePixelBuffer* const buffer,
                               const leRect* const rect,
                               leRect* result);

// *****************************************************************************
/* Function:
    leResult lePixelBufferSet(const lePixelBuffer* const buffer,
                              const lePoint* const pnt,
                              leColor color)

  Summary:
    Sets a pixel in a pixel buffer at a point to a specified color.  Caller is
    responsible for ensuring that the input color is in the same color format
    as the pixel buffer.

  Parameters:
    const lePixelBuffer* const buffer - the buffer to operate on
    const lePoint* const - the location of the pixel to set
    leColor - the color to set the pixel to.  must be the same format as the
              buffer
    
  Returns:
    leResult
*/                                                
/**
 * @brief Pixel set
 * @details Sets a pixel in a pixel buffer at a point to a specified color.  Caller
 * is responsible for ensuring that the input color is in the same color
 * format as the pixel buffer
 * @code
 * lePixelBuffer* buffer;
 * leResult res = lePixelBufferSet(buffer, x, y, color);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leResult.
 */
leResult lePixelBufferSet(const lePixelBuffer* const buffer,
                          uint32_t x,
                          uint32_t y,
                          leColor color);

// *****************************************************************************
/* Function:
    leResult lePixelBufferSet_Unsafe(const lePixelBuffer* const buffer,
                                     const lePoint* const pnt,
                                     leColor color)

  Summary:
    Sets a pixel in a pixel buffer at a point to a specified color.  Caller is
    responsible for ensuring that the input color is in the same color format
    as the pixel buffer.  Like lePixelBufferSet but performs no bounds
    checking.

  Parameters:
    const lePixelBuffer* const buffer - the buffer to operate on
    const lePoint* const - the location of the pixel to set
    leColor - the color to set the pixel to.  must be the same format as the
              buffer
    
  Returns:
    leResult
*/                                         
/**
 * @brief Pixel set
 * @details Sets a pixel in a pixel buffer at a point to a specified color.
 * Caller is responsible for ensuring that the input color is in the same color
 * format as the pixel buffer.  Like lePixelBufferSet but performs no
 * bounds checking.
 * @code
 * lePixelBuffer* buffer;
 * leResult res = lePixelBufferSet_Unsafe(buffer, x, y, color);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return leResult.
 */
leResult lePixelBufferSet_Unsafe(const lePixelBuffer* const buffer,
                                 uint32_t x,
                                 uint32_t y,
                                 leColor color);

// *****************************************************************************
/* Function:
    leResult lePixelBufferAreaFill(const lePixelBuffer* const buffer,
                                   uint32_t x,
                                   uint32_t y,
                                   uint32_t w,
                                   uint32_t h,
                                   const leColor color)

  Summary:
    Fills a rectangular area of a pixel buffer with a solid color.

  Parameters:
    const lePixelBuffer* const buffer - the buffer to operate on
    uint32_t x - the x coordinate of the rectangle to fill
    uint32_t y - the y coordinate of the rectangle to fill
    uint32_t w - the width of the rectangle area
    uint32_t h - the height of the rectangle area
    const leColor color

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Area fill with checking
 * @details Fill <span style="color: #820a32"><em>buffer</em></span> with
 * <span style="color: #820a32"><em>color</em></span> defined by the x, y w, h.
 * @code
 * lePixelBuffer* buffer;
 * leResult res = lePixelBufferAreaFill_Unsafe(buffer, locked);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return true if the buffer locked, otherwse false.
 */
leResult lePixelBufferAreaFill(const lePixelBuffer* const buffer,
                               uint32_t x,
                               uint32_t y,
                               uint32_t w,
                               uint32_t h,
                               const leColor color);

// *****************************************************************************
/* Function:
    leResult lePixelBufferAreaFill_Unsafe(const lePixelBuffer* const buffer,
                                          const leRect* const rect,
                                          const leColor color)

  Summary:
    Fills an area of a pixel buffer with a solid color.  Caller is responsible
    for ensuring that the color is the same color format as the destination
    buffer.  Like lePixelBufferAreaFill but performs no bounds checking.

  Parameters:
    const lePixelBuffer* const buffer - the buffer to manipulate
    const leRect* const rect - the rectangle of the buffer to fill
    const leColor color - the color to use for the fill operation
    
  Returns:
    leResult
*/
/**
 * @brief Area fill no checking
 * @details Fill <span style="color: #820a32"><em>buffer</em></span> with
 * <span style="color: #820a32"><em>color</em></span> defined by the x, y w, h.
 * @code
 * lePixelBuffer* buffer;
 * leResult res = lePixelBufferAreaFill_Unsafe(buffer, locked);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return true if the buffer locked, otherwse false.
 */
leResult lePixelBufferAreaFill_Unsafe(const lePixelBuffer* const buffer,
                                      uint32_t x,
                                      uint32_t y,
                                      uint32_t w,
                                      uint32_t h,
                                      leColor color);

// *****************************************************************************
/* Function:
    leResult lePixelBufferCopy(lePixelBuffer* dest,
                               int16_t x,
                               int16_t y,
                               const lePixelBuffer* src,
                               leRect* srcRect)

  Summary:
    Copies a rectangle of data from one buffer into another buffer.  This is
    a safe copy in that any reads or writes outside of the buffers are detected
    and discarded.  The color data is automatically converted from the source
    mode to the destination mode.

  Parameters:
    lePixelBuffer* dest - the buffer to write to
    int16_t - the x coordinate to write to
    int16_t - the y cooridnate to write to
    const lePixelBuffer* src - the pixel buffer to read from
    leRect* srcRect - the rectangle of the source data

  Returns:
    leResult
*/
leResult lePixelBufferCopy(lePixelBuffer* dest,
                           uint32_t x,
                           uint32_t y,
                           const lePixelBuffer* src,
                           const leRect* srcRect);

// *****************************************************************************
/* Function:
    leResult lePixelBuffer_IsLocked(const lePixelBuffer* const buffer)

  Summary:
    Returns true if the buffer's lock flag is on.

  Parameters:
    const lePixelBuffer* const buffer - the buffer to query

  Returns:
    leBool - true if the buffer is locked
*/
/**
 * @brief Determines the lock state.
 * @details Returns the lock state of <span style="color: #820a32"><em>buffer</em></span>.
 * @code
 * lePixelBuffer* buffer;
 * leResult res = lePixelBuffer_SetLocked(buffer, locked);
 * @endcode
 * @param param1 buffer is the source color value.
 * @return true if the buffer locked, otherwse false.
 */
leBool lePixelBuffer_IsLocked(const lePixelBuffer* const buffer);

// *****************************************************************************
/* Function:
    leResult lePixelBuffer_SetLocked(lePixelBuffer* buffer,
                                     leBool locked)

  Summary:
    Manipulates a buffer lock flag

  Parameters:
    const lePixelBuffer* const buffer - the buffer to query
    leBool locked - the desired lock state

  Returns:
    leResult
*/
/**
 * @brief Sets the lock.
 * @details Set the lock flag on <span style="color: #820a32"><em>buffer</em></span>
 * to the desired lock state specified by <span style="color: #820a32"><em>locked</em></span>.
 * @code
 * lePixelBuffer* buffer;
 * leBool locked;
 * leResult res = lePixelBuffer_SetLocked(buffer, locked);
 * @endcode
 * @param param1 buffer is the source color value.
 * @param param2 locked is the source color mode
 * @return leResult.
 */
leResult lePixelBuffer_SetLocked(lePixelBuffer* buffer,
                                 leBool locked);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
    }
#endif
// DOM-IGNORE-END

#endif /* LE_PIXELBUFFER_H */
