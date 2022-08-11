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
    legato_rectarray.h

  Summary:
    An array implementation for storing rectangles for the
    Legato user interface library

  Description:
    This is an array implementation that is used internally by the 
    Legato user interface library.  
*******************************************************************************/

/** \file legato_rectarray.h
 * @brief An array implementation for storing rectangles for the
 * Legato user interface library.
 *
 * @details This is an array implementation that is used internally
 * by the Legato user interface library.
 */

#ifndef LEGATO_RECTARRAY_H
#define LEGATO_RECTARRAY_H

#include "gfx/legato/common/legato_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
/* Structure:
    leRectArray

  Summary:
    Rectangle array definition

  Remarks:
    None.
*/

/**
 * @brief This struct represents a rectange array.
 * @details
 */
typedef struct leRectArray
{
    leRect* rects;  /**< Pointer to rectangles. */
    uint32_t size;  /**< Current number of rectangles. */
    uint32_t capacity;  /**< Total possible rectangles. */
} leRectArray;

// *****************************************************************************

/**
 * @brief Creates a new rectangle.
 * @details Creates a new rectangle <span class="param">arr</span>.
 * @code
 * leArray* arr;
 * leResult res = leRectArray_Create(arr)
 * @endcode
 * @param arr is array to initialize.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_Create(leRectArray* arr);


// *****************************************************************************

/**
 * @brief Resizes the capacity of the array.
 * @details Expands (or shrinks) an array
 * <span class="param">arr</span> to the specified
 * <span class="param">sz</span>.
 *
 * If the array shrinks, any nodes beyond the new capacity will be
 * discarded.
 * @code
 * leArray* arr;
 * uint32_t sz;
 * leResult res = leArray_Resize(arr, sz)
 * @endcode
 * @param arr The pointer to the array to resize.
 * @param sz The desired capacity.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_Resize(leRectArray* arr, uint32_t sz);

// *****************************************************************************
/**
 * @brief Pushes a new rectangle onto the front of the array.
 * @details Pushes <span class="param">val</span> onto the
 * front of <span class="param">arr</span>.
 * This function shuffles all other values backward one index.
 * @code
 * leRectArray* arr;
 * leRect* val;
 * leResult res = leRectArray_PushFront(arr, val)
 * @endcode
 * @param arr is the array to modify.
 * @param val is the value to push.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_PushFront(leRectArray* arr,
                               const leRect* rect);

// *****************************************************************************
/**
 * @brief Removes the first value from the array.
 * @details Removes the first value from the front
 * of <span class="param">arr</span>.
 * This function shuffles all other values forward one index.
 * @code
 * leRectArray* arr;
 *  leRectArray_PopFront(arr)
 * @endcode
 * @param arr The pointer to the array to modify.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_PopFront(leRectArray* arr);

// *****************************************************************************
/**
 * @brief Push value on back.
 * @details Pushes <span class="param">val</span> onto the
 * back of <span class="param">arr</span>.
 * @code
 * leArray* arr;
 * leRect* val;
 * leResult res = leRectArray_PushBack(arr, val)
 * @endcode
 * @param arr is the array to modify.
 * @param val is the value to push.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_PushBack(leRectArray* arr, const leRect* rect);

// *****************************************************************************
/**
 * @brief Removes the last rectangle from the array.
 * @details Removes the last value from the back
 * of <span class="param">arr</span>.
 * @code
 * leRectArray* arr;
 * leResult res = leRectArray_PopBack(arr)
 * @endcode
 * @param arr is the array to modify.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_PopBack(leRectArray* arr);

// *****************************************************************************
/**
 * @brief Inserts a rectangle into an array at a given index.
 * @details Inserts <span class="param">rect</span> into the
 * <span class="param">arr</span> at
 * the specified <span class="param">idx</span>.
 * All existing values from index are shifted right one place.
 * @code
 * leRectArray* arr;
 * uint32_t idx;
 * leRect* val;
 * leResult res = leRectArray_InsertAt(arr idx, val)
 * @endcode
 * @param arr is the array to modify.
 * @param idx is the location at which to insert value.
 * @param val is value to insert.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_InsertAt(leRectArray* arr,
                              uint32_t idx,
                              const leRect* rect);

// *****************************************************************************
/**
 * @brief Removes a rectangle from the array at an index.
 * @details Removes the rectangle at <span class="param">idx</span> from
 * <span class="param">arr</span>.
 * @code
 * leRectArray* arr;
 * uint32_t idx;
 * leResult res = leRectArray_RemoveAt(arr idx)
 * @endcode
 * @param arr is the array to modify.
 * @param idx is the location at which to insert value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_RemoveAt(leRectArray* arr, uint32_t idx);

// *****************************************************************************
/**
 * @brief Copy array.
 * @details Copys the contents of <span class="param">src</span>
 * to the array pointed to by <span class="param">dest</span>.
 * @code
 * leRectArray* src;
 * leRectArray* dest;
 * leResult res = leRectArray_Copy(arr, dest)
 * @endcode
 * @param src is the source array.
 * @param dest is the destination array.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_Copy(leRectArray* src, leRectArray* dest);

// *****************************************************************************
/**
 * @brief Removes all values from a given array.
 * @details Removes all values from <span class="param">arr</span>.
 * Array capacity remains the same.
 * @code
 * leRectArray* arr;
 * leResult res = leRectArray_Clear(arr)
 * @endcode
 * @param arr is the array to modify.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_Clear(leRectArray* arr);

// *****************************************************************************
/**
 * @brief Destroy array.
 * @details Removes all values from <span class="param">arr</span>.
 * Array capacity is set to 0.
 * leRectArray* arr;
 * leResult res = leRectArray_Destroy(arr)
 * @endcode
 * @param arr the array to modify.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_Destroy(leRectArray* arr);


// *****************************************************************************
/**
 * @brief Removes any duplicate rectangles from an array.
 * @details Removes all duplicates from  <span class="param">arr</span>.
 * @code
 * leRectArray* arr;
 * leResult res = leRectArray_RemoveDuplicates(arr)
 * @endcode
 * @param arr the array to modify.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_RemoveDuplicates(leRectArray* arr);

// *****************************************************************************
/**
 * @brief Sorts a given array
 * @details Sorts values in <span class="param">arr</span> from largest to smallest.
 * @code
 * leRectArray* arr;
 * leResult res = leRectArray_SortBySize(arr)
 * @endcode
 * @param arr is the array to sort.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_SortBySize(leRectArray* arr);

// *****************************************************************************
/**
 * @brief Sort array by X coordinate
 * @details Sorts the values in <span class="param">arr</span> in order
 * of the X coordinate.
 * @code
 * leRectArray* arr;
 * leResult res = leRectArray_SortByX(arr)
 * @endcode
 * @param arr the array to sort.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_SortByX(leRectArray* arr);

// *****************************************************************************
/**
 * @brief Sort array by Y coordinate
 * @details Sorts the values in <span class="param">arr</span> in
 * order of the Y coordinate.
 * @code
 * leRectArray* arr;
 * leResult res = leRectArray_SortByY(arr)
 * @endcode
 * @param arr the array to analyze.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_SortByY(leRectArray* arr);

// *****************************************************************************
/**
 * @brief Merge rectangles
 * @details Merges similar rectangles in <span class="param">arr</span>.
 * @code
 * leRectArray* arr;
 * leResult res = leRectArray_MergeSimilar(arr)
 * @endcode
 * @param arr the array to modify.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_MergeSimilar(leRectArray* arr);

// *****************************************************************************
/**
 * @brief Remove overlappping rectangles
 * @details Removes overlapping rectangles in <span class="param">arr</span>.
 * The function first sorts the array by size then removes any rectangles that
 * are completely overlapped by another larger rectangle.
 * @code
 * leRectArray* arr;
 * leResult res = leRectArray_RemoveOverlapping(arr)
 * @endcode
 * @param arr the array to modify.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_RemoveOverlapping(leRectArray* arr);

#if LE_SCRATCH_BUFFER_PADDING == 1
/**
 * @brief Pad existing rectangles
 * @details Analyzes and splits rectangles in order to conform them to a defined
 * padding strategy.
 * @code
 * leRectArray* arr;
 * leRectArray_PadRectangles(arr)
 * @endcode
 * @param arr the array to modify.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_PadRectangles(leRectArray* arr);

/**
 * @brief Pad existing rectangles
 * @details Analyzes and splits rectangles by width in order to conform them to a defined
 * padding strategy.
 * @code
 * leRectArray* arr;
 * leRectArray_PadRectangles(arr)
 * @endcode
 * @param arr the array to modify.
 * @param size the padding size
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_PadRectangleWidth(leRectArray* arr,
                                       uint32_t size);

/**
 * @brief Pad existing rectangles
 * @details Analyzes and splits rectangles by height in order to conform them to a defined
 * padding strategy.
 * @code
 * leRectArray* arr;
 * leRectArray_PadRectangles(arr)
 * @endcode
 * @param arr the array to modify.
 * @param size the padding size
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_PadRectangleHeight(leRectArray* arr,
                                        uint32_t size);
#endif

// *****************************************************************************
/**
 * @brief Crops rectange by width and height
 * @details Crops <span class="param">arr</span> to
 * given <span class="param">width</span>
 * and <span class="param">height</span>.
 * Any remainder will be moved into a new rectangle and will be cropped
 * again if needed.
 * @code
 * leRectArray* arr;
 * uint32_t width;
 * uint32_t height;
 * leResult res = leRectArray_CropToArea(arr, width, height);
 * @endcode
 * @param arr the array to modify.
 * @param width the crop width.
 * @param height arr the crop height.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leRectArray_CropToArea(leRectArray* arr,
                                uint32_t width,
                                uint32_t height);


// *****************************************************************************
/**
 * @brief Slices an array of rectangles to a given pixel count
 * @details Crops <span class="param">arr</span> to
 * given <span class="param">width</span>
 * and <span class="param">size</span>.
 * This method attempts to keep the rectangle slices as wide as possible.
 * @code
 * leRectArray* arr;
 * uint32_t size;
 * leResult res = leRectArray_CropToSizeY(arr, size);
 * @endcode
 * @param arr the array to modify.
 * @param size the number of pixels to crop to.
 * @returns LE_SUCCESS , otherwise LE_FAILURE.
 */
leResult leRectArray_CropToSizeY(leRectArray* arr, uint32_t size);

// *****************************************************************************
/**
 * @brief Slices an array of rectangles to a given pixel count
 * @details Crops <span class="param">arr</span> to
 * given <span class="param">width</span>
 * and <span class="param">size</span>.
 * This method attempts to keep the rectangle slices as tall as possible.
 * @code
 * leRectArray* arr;
 * uint32_t size;
 * leResult res = leRectArray_CropToSizeX(arr, size);
 * @endcode
 * @param arr the array to modify.
 * @param size the number of pixels to crop to.
 * @returns LE_SUCCESS , otherwise LE_FAILURE.
 */
leResult leRectArray_CropToSizeX(leRectArray* arr, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* LEGATO_RECTARRAY_H */
