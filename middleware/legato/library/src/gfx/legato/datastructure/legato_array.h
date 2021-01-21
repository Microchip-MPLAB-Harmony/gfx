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
    legato_array.h

  Summary:
    An array implementation for storing pointers for the
    Legato user interface library

  Description:
    This is an array implementation that is used internally by the 
    Legato user interface library.  
*******************************************************************************/


/** \file legato_array.h
 * @brief An array implementation for storing pointers.
 *
 * @details This is an array implementation that is used internally by the Legato user
 * interface library.
 */

#ifndef LEGATO_ARRAY_H
#define LEGATO_ARRAY_H

#include "gfx/legato/common/legato_common.h"

// *****************************************************************************
/**
 * @brief This struct represents a array.
 * @details An array is used to hold values. It has a size and capacity.
 */
typedef struct leArray
{
    void** values;  /**< Number of values. */
    uint32_t size; /**< Size of array. */
    uint32_t capacity; /**< Number of values allowed. */
} leArray;

// *****************************************************************************
/**
 * @brief Create a new array.
 * @details Creates a new array pointed to by <span class="param">arr</span>.
 * @code
 * leArray* arr;
 * leResult res = leArray_Create(arr)
 * @endcode
 * @param arr is array to create.
 * @return LE_SUCCESS if created, otherwise LE_FAILURE.
 */
leResult leArray_Create(leArray* arr);


// *****************************************************************************
/**
 * @brief Resize array
 * @details Expands (or shrinks)
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
 * @param arr is the array to resize.
 * @param sz is the new size.
 * @return LE_SUCCESS if resized, otherwise LE_FAILURE.
 */
leResult leArray_Resize(leArray* arr, uint32_t sz);

// *****************************************************************************
/**
 * @brief Push value on front
 * @details Pushes <span class="param">val</span> onto the
 * front of <span class="param">arr</span>.
 * This function shuffles all other values backward one index.
 * @code
 * leArray* arr;
 * void* val;
 * leResult res = leArray_PushFront(arr, val)
 * @endcode
 * @param arr is the array to modify.
 * @param val is the value to push.
 * @return LE_SUCCESS if pushed, otherwise LE_FAILURE.
 */
leResult leArray_PushFront(leArray* arr, void* val);

// *****************************************************************************
/**
 * @brief Pop value from front
 * @details Removes the first value from the front
 * of <span class="param">arr</span>.
 * This function shuffles all other values forward one index.
 * @code
 * leArray* arr;
 * leResult res = leArray_PopFront(arr)
 * @endcode
 * @param arr is the array to modify.
 * @return LE_SUCCESS if removed, otherwise LE_FAILURE.
 */
leResult leArray_PopFront(leArray* arr);

// *****************************************************************************
/**
 * @brief Push value on back.
 * @details Pushes <span class="param">val</span> onto the
 * back of <span class="param">arr</span>.
 * @code
 * leArray* arr;
 * void* val;
 * leResult res = leArray_PushBack(arr, val)
 * @endcode
 * @param arr is the array to modify.
 * @param val is the value to push.
 * @return LE_SUCCESS if pushed, otherwise LE_FAILURE.
 */
leResult leArray_PushBack(leArray* arr, void* val);

// *****************************************************************************
/**
 * @brief Pop value from back
 * @details Removes the last value from the back
 * of <span class="param">arr</span>.
 * @code
 * leArray* arr;
 * leResult res = leArray_PopBack(arr)
 * @endcode
 * @param arr is the array to modify.
 * @return LE_SUCCESS if popped, otherwise LE_FAILURE.
 */
leResult leArray_PopBack(leArray* arr);

// *****************************************************************************
/**
 * @brief Insert value at index
 * @details Inserts <span class="param">val</span> into
 * <span class="param">arr</span> at
 * the specified <span class="param">idx</span>.
 * All existing values from index are shifted right one place.
 * @code
 * leArray* arr;
 * uint32_t idx;
 * void* val;
 * leResult res = leArray_InsertAt(arr idx, val)
 * @endcode
 * @param arr is the array to modify.
 * @param idx is the location at which to insert value.
 * @param val is value to insert.
 * @return LE_SUCCESS if inserted, otherwise LE_FAILURE.
 */
leResult leArray_InsertAt(leArray* arr,
                          uint32_t idx,
                          void* val);

// *****************************************************************************
/**
 * @brief Remove value at index
 * @details Removes <span class="param">val</span> from
 * <span class="param">arr</span> at
 * the specified <span class="param">idx</span>.
 * @code
 * leArray* arr;
 * uint32_t idx;
 * leResult res = leArray_RemoveAt(arr idx)
 * @endcode
 * @param arr is the array to modify.
 * @param idx is the location at which to insert value.
 * @return LE_SUCCESS if removed, otherwise LE_FAILURE.
 */
leResult leArray_RemoveAt(leArray* arr, uint32_t idx);

// *****************************************************************************
/**
 * @brief Remove value
 * @details Removes the first instance of <span class="param">val</span>
 * from <span class="param">arr</span>.
 * The function shuffles all values left to fill the gap.
 * @code
 * leArray* arr;
 * void* val;
 * leResult res = leArray_Remove(leArray* arr, void* val)
 * @endcode
 * @param arr is the array to modify.
 * @param val is the value removed.
 * @return LE_SUCCESS if removed, otherwise LE_FAILURE.
 */
leResult leArray_Remove(leArray* arr, void* val);

// *****************************************************************************
/**
 * @brief Get entry at index
 * @details Returns the pointer to the entry from
 * <span class="param">arr</span> at
 * the specified <span class="param">idx</span>.
 * @code
 * leArray* arr;
 * uint32_t idx;
 * void* entry = leArray_Get(arr, idx)
 * @endcode
 * @param arr is the array of entries.
 * @param idx is the index at which entry is located.
 * @return void*  pointer to the entry at idx.
 */
void* leArray_Get(const leArray* arr, uint32_t idx);

// *****************************************************************************
/**
 * @brief Set value at index
 * @details Sets <span class="param">val</span> onto
 * <span class="param">arr</span> at
 * the specified <span class="param">idx</span>.
 * @code
 * leList* arr;
 * uint32_t idx;
 * void* val;
 * leResult res = leArray_Set(arr, idx, val)
 * @endcode
 * @param arr is the array to modify.
 * @param idx is the index where the value is set.
 * @param val is the value to set at index.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leArray_Set(leArray* arr, uint32_t idx, void* val);

// *****************************************************************************
/**
 * @brief Find a first index of value
 * @details Returns the first index of <span class="param">val</span>
 * in <span class="param">arr</span>.
 * @code
 * leArray* arr;
 * void* val;
 * int32_t index = leArray_Find(arr, val)
 * @endcode
 * @param arr is the array to search.
 * @param val is the value to set at index.
 * @return the first index of the value or -1 if not found.
 */
int32_t leArray_Find(const leArray* arr, void* val);

// *****************************************************************************
/**
 * @brief Copy array
 * @details Copies the contents of <span class="param">src</span>
 * to the array pointed to by <span class="param">dest</span>.
 * @code
 * leArray* src;
 * leArray* dest;
 * leResult res = leArray_Copy(src, dest)
 * @endcode
 * @param src is the source array.
 * @param dest is the destination array.
 * @return LE_SUCCESS if copied, otherwise LE_FAILURE.
 */
leResult leArray_Copy(leArray* src, leArray* dest);

// *****************************************************************************
/**
 * @brief Clear array.
 * @details Removes all values from <span class="param">arr</span>.
 * Array capacity remains the same.
 * @code
 * leArray* arr;
 * leResult res = leArray_Clear(arr)
 * @endcode
 * @param arr is the array to modify.
 * @return LE_SUCCESS if cleared, otherwise LE_FAILURE.
 */
leResult leArray_Clear(leArray* arr);

#endif /* LEGATO_ARRAY_H */
