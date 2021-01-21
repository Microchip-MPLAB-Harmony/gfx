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
    legato_rectarray.h

  Summary:
    An array implementation for storing rectangles for the
    Legato user interface library

  Description:
    This is an array implementation that is used internally by the 
    Legato user interface library.  
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_RECTARRAY_H
#define LEGATO_RECTARRAY_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

// *****************************************************************************
/* Structure:
    leRectArray

  Summary:
    Rectangle array definition

  Remarks:
    None.
*/
typedef struct leRectArray
{
    leRect* rects;
    uint32_t size;
    uint32_t capacity;
} leRectArray;

// *****************************************************************************
/* Function:
    leResult leRectArray_Create(leRectArray* arr)

   Summary:
    Initializes a new rectangle array.

   Parameters:
    leRectArray* arr - pointer to the array to initilaize

  Returns:
    leResult - the result of the operation

  Remarks:        
*/
LIB_EXPORT leResult leRectArray_Create(leRectArray* arr);


// *****************************************************************************
/* Function:
    leResult leRectArray_Resize(leRectArray* arr)

   Summary:
    Resizes the capacity of the array.  If the array shrinks, any nodes beyond
    the new capacity will be discarded.

   Parameters:
    leRectArray* arr - pointer to the array to resize
    uint32_t sz - the desired capacity

  Returns:
    leResult - the result of the operation

  Remarks:
*/
LIB_EXPORT leResult leRectArray_Resize(leRectArray* arr, uint32_t sz);

// *****************************************************************************
/* Function:
    leResult leRectArray_PushFront(leRectArray* arr, const leRect* rect))

   Summary:
    Pushes a new rectangle onto the front of the array.  Shuffles all other
    nodes backward one index.

   Parameters:
    leRectArray* arr - pointer to the array to modify
    const leRect* rect - the rectangle value
    
  Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_PushFront(leRectArray* arr,
                                          const leRect* rect);

// *****************************************************************************
/* Function:
    void leRectArray_PopFront(leRectArray* arr)

   Summary:
    Removes the first value from the array.  Shuffles all other nodes forward
    one index.

   Parameters:
    leRectArray* arr - pointer to the array to modify

   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_PopFront(leRectArray* arr);

// *****************************************************************************
/* Function:
    leResult leRectArray_PushBack(leRectArray* arr, const leRect* rect)

   Summary:
    Pushes a new rectangle onto the back of the array

   Parameters:
    leRectArray* arr - pointer to the array to modify
    const leRect* rect - the rectangle value
    
  Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_PushBack(leRectArray* arr, const leRect* rect);

// *****************************************************************************
/* Function:
    void leRectArray_PopBack(leRectArray* arr)

   Summary:
    Removes the last rectangle from the array

   Parameters:
    leRectArray* arr - pointer to the array to modify
*/
LIB_EXPORT leResult leRectArray_PopBack(leRectArray* arr);

// *****************************************************************************
/* Function:
    leResult leRectArray_InsertAt(leRectArray* arr,
                                  uint32_t idx,
                                  const leRect* rect);

   Summary:
    Inserts a rectangle into an array at a given index.  All existing nodes
    from index are shifted right one place.

   Parameters:
    leRectArray* arr - pointer to the array to modify
    uint32_t idx - the position to insert the value
    const leRect* rect - the rectangle value
   
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_InsertAt(leRectArray* arr,
                                         uint32_t idx,
                                         const leRect* rect);

// *****************************************************************************
/* Function:
    leResult leRectArray_RemoveAt(leRectArray* arr, uint32_t idx)

   Summary:
    Removes a rectangle from the array at an index

   Parameters:
    leRectArray* arr - pointer to the array to modify
    uint32_t idx - the index of the value to remove
   
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_RemoveAt(leRectArray* arr, uint32_t idx);

// *****************************************************************************
/* Function:
    leResult leRectArray_Copy(leRectArray* l, leRectArray* r)

   Summary:
    Creates a duplicate of an existing array

   Parameters:
    leRectArray* src - the source array
    leRectArray* dest - the result array
   
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_Copy(leRectArray* src, leRectArray* dest);

// *****************************************************************************
/* Function:
    leResult leRectArray_Clear(leRectArray* arr)

   Summary:
    Removes all values from a given array.  Array capacity remains the same.

   Parameters:
    leRectArray* arr - the array to modify
       
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_Clear(leRectArray* arr);

// *****************************************************************************
/* Function:
    leResult leRectArray_Destroy(leRectArray* arr)

   Summary:
    Removes all nodes from a given array and frees the memory owned by the array.
    Resets array capacity to zero.

   Parameters:
    leRectArray* arr - the array to modify
       
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_Destroy(leRectArray* arr);


// *****************************************************************************
/* Function:
    leResult leRectArray_RemoveDuplicates(leRectArray* arr)

   Summary:
    Removes any duplicate rectangles from an array.

   Parameters:
    leRectArray* arr - the array to analyze
       
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_RemoveDuplicates(leRectArray* arr);

// *****************************************************************************
/* Function:
    leResult leRectArray_SortBySize(leRectArray* arr)

   Summary:
    Sorts a given array largest to smallest.

   Parameters:
    leRectArray* arr - the array to analyze
       
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_SortBySize(leRectArray* arr);

// *****************************************************************************
/* Function:
    leResult leRectArray_SortByX(leRectArray* arr)

   Summary:
    Sorts a given array in order of the X coordinate.

   Parameters:
    leRectArray* arr - the array to analyze

   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_SortByX(leRectArray* arr);

// *****************************************************************************
/* Function:
    leResult leRectArray_SortByY(leRectArray* arr)

   Summary:
    Sorts a given array in order of the Y coordinate.

   Parameters:
    leRectArray* arr - the array to analyze
       
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_SortByY(leRectArray* arr);

// *****************************************************************************
/* Function:
    leResult leRectArray_MergeSimilar(leRectArray* arr)

   Summary:
    Analyzes an array and merges any rectangles similar in size.

   Parameters:
    leRectArray* arr - the array to analyze
       
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_MergeSimilar(leRectArray* arr);

// *****************************************************************************
/* Function:
    leResult leRectArray_RemoveOverlapping(leRectArray* arr)

   Summary:
    Sorts the array by size and then removes any rectangles that are completely
    overlapped by another larger rectangle.

   Parameters:
    leRectArray* arr - the array to modify
       
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_RemoveOverlapping(leRectArray* arr);

// *****************************************************************************
/* Function:
    leResult leRectArray_CropToArea(leRectArray* arr,
                                    uint32_t width,
                                    uint32_t height)

   Summary:
    Crops all rectangles to a given width and height.  Any remainder will be
    moved into a new rectangle and will be cropped again if needed.

   Parameters:
    leRectArray* arr - the array to modify
    uint32_t width - the width to crop to
    uint32_t height - the height to crop to
       
   Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leRectArray_CropToArea(leRectArray* arr,
                                           uint32_t width,
                                           uint32_t height);
                                           
                                           
leResult leRectArray_CropToSize(leRectArray* arr, uint32_t size);

#endif /* LEGATO_RECTARRAY_H */
