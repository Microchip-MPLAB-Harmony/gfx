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
    legato_array.h

  Summary:
    An array implementation for storing pointers for the
    Legato user interface library

  Description:
    This is an array implementation that is used internally by the 
    Legato user interface library.  
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_ARRAY_H
#define LEGATO_ARRAY_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

// *****************************************************************************
/* Structure:
    leArray

  Summary:
    Array data structure definition

  Remarks:
    None.
*/
typedef struct leArray
{
    void** values;
    uint32_t size;
    uint32_t capacity;
} leArray;

// *****************************************************************************
/* Function:
    leResult leArray_Create(leArray* arr)

   Summary:
    Initializes a new array.

   Parameters:
    leArray* arr - pointer to the array to initilaize

  Returns:
    leResult - the result of the operation

  Remarks:        
*/
leResult leArray_Create(leArray* arr);


// *****************************************************************************
/* Function:
    leResult leArray_Resize(leArray* arr)

   Summary:
    Resizes the capacity of the array.  If the array shrinks, any nodes beyond
    the new capacity will be discarded.

   Parameters:
    leArray* arr - pointer to the array to resize
    uint32_t sz - the desired capacity

  Returns:
    leResult - the result of the operation

  Remarks:
*/
leResult leArray_Resize(leArray* arr, uint32_t sz);

// *****************************************************************************
/* Function:
    leResult leArray_PushFront(leArray* arr, void* val))

   Summary:
    Pushes a new value onto the front of the array.  Shuffles all other
    nodes backward one index.

   Parameters:
    leArray* arr - pointer to the array to modify
    void* val - the value to store
    
  Returns:
    leResult - the result of the operation
*/
leResult leArray_PushFront(leArray* arr, void* val);

// *****************************************************************************
/* Function:
    void leArray_PopFront(leArray* arr)

   Summary:
    Removes the first value from the array.  Shuffles all other nodes forward
    one index.

   Parameters:
    leArray* arr - pointer to the array to modify

   Returns:
    leResult - the result of the operation
*/
leResult leArray_PopFront(leArray* arr);

// *****************************************************************************
/* Function:
    leResult leArray_PushBack(leArray* arr, void* val)

   Summary:
    Pushes a new value onto the back of the array

   Parameters:
    leArray* arr - pointer to the array to modify
    void* val - the value to store
    
  Returns:
    leResult - the result of the operation
*/
leResult leArray_PushBack(leArray* arr, void* val);

// *****************************************************************************
/* Function:
    void leArray_PopBack(leArray* arr)

   Summary:
    Removes the last value from the array

   Parameters:
    leArray* arr - pointer to the array to modify
*/
leResult leArray_PopBack(leArray* arr);

// *****************************************************************************
/* Function:
    leResult leArray_InsertAt(leArray* arr,
                              uint32_t idx,
                              void* val);

   Summary:
    Inserts a value into an array at a given index.  All existing nodes
    from index are shifted right one place.

   Parameters:
    leArray* arr - pointer to the array to modify
    uint32_t idx - the position to insert the value
    void* val - the value to store
   
   Returns:
    leResult - the result of the operation
*/
leResult leArray_InsertAt(leArray* arr,
                          uint32_t idx,
                          void* val);

// *****************************************************************************
/* Function:
    leResult leArray_RemoveAt(leArray* arr, uint32_t idx)

   Summary:
    Removes a value from the array at an index

   Parameters:
    leArray* arr - pointer to the array to modify
    uint32_t idx - the index of the value to remove
   
   Returns:
    leResult - the result of the operation
*/
leResult leArray_RemoveAt(leArray* arr, uint32_t idx);

// *****************************************************************************
/* Function:
    leResult leArray_Remove(leArray* arr, void* val)

   Summary:
    Removes the first instance of a value from the array and shuffles
    all values left to fill the gap

   Parameters:
    leArray* arr - pointer to the array to modify
    void* val - the value to remove
   
   Returns:
    leResult - the result of the operation
*/
leResult leArray_Remove(leArray* arr, void* val);

// *****************************************************************************
/* Function:
    void* leArray_Get(const leArray* arr, uint32_t idx);

   Summary:
    Returns the pointer to the entry at index idx

   Parameters:
    const leList* list - const pointer to the list to reference
    uint32_t idx - the index of the entry to get
   
   Returns:
    void* - pointer to the entry at idx
*/
void* leArray_Get(const leArray* arr, uint32_t idx);

// *****************************************************************************
/* Function:
    leResult leArray_Set(leArray* arr, uint32_t idx, void* val);

   Summary:
    Sets the pointer at idx

   Parameters:
    leList* list - pointer to the list to reference
    uint32_t idx - the index of the entry to set
    void* val - the value to set the index to
   
   Returns:
    leResult - the result of the operation
*/
leResult leArray_Set(leArray* arr, uint32_t idx, void* val);

// *****************************************************************************
/* Function:
    leResult leArray_Find(leArray* arr, void* val)

   Summary:
    Finds the first index of a value in the array

   Parameters:
    leArray* arr - pointer to the array to search
    void* val - the value to remove
   
   Returns:
    int32_t - the first index of the value or -1 if not found
*/
int32_t leArray_Find(const leArray* arr, void* val);

// *****************************************************************************
/* Function:
    leResult leArray_Copy(leArray* l, leArray* r)

   Summary:
    Creates a duplicate of an existing array

   Parameters:
    leArray* src - the source array
    leArray* dest - the result array
   
   Returns:
    leResult - the result of the operation
*/
leResult leArray_Copy(leArray* src, leArray* dest);

// *****************************************************************************
/* Function:
    void leArray_Clear(leArray* arr)

   Summary:
    Removes all values from a given array.  Array capacity remains the same.

   Parameters:
    leArray* arr - the array to modify
       
   Returns:
    leResult - the result of the operation
*/
leResult leArray_Clear(leArray* arr);

#endif /* LEGATO_ARRAY_H */
