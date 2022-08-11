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


#include "gfx/legato/datastructure/legato_array.h"

#include <string.h>

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/memory/legato_memory.h"

#define STD_RESIZE_AMT   5

static void _shuffleRight(leArray* arr, uint32_t idx)
{
    uint32_t i;

    if(arr->size == 0)
        return;

    if(arr->size < arr->capacity)
    {
        arr->size++;
    }
    
    for(i = arr->size - 1; i > idx; --i)
    {
        arr->values[i] = arr->values[i-1];
    }
    
    arr->values[i] = NULL;
}

static void _shuffleLeft(leArray* arr, uint32_t idx)
{
    uint32_t i;

    if(arr->size == 0)
        return;

    for(i = idx; i < arr->size; i++)
    {
        arr->values[i] = arr->values[i+1];
    }
    
    arr->values[i-1] = NULL;

    arr->size--;
}

leResult leArray_Create(leArray* arr)
{
    if(arr == NULL)
        return LE_FAILURE;

    arr->values = NULL;
    arr->size = 0;
    arr->capacity = 0;

    return LE_SUCCESS;
}

leResult leArray_Resize(leArray* arr, uint32_t sz)
{
    void* ptr;
    
    if(arr == NULL || arr->capacity == sz)
        return LE_FAILURE;

    ptr = LE_REALLOC(arr->values, sizeof(void*) * sz);

    LE_ASSERT(ptr != NULL);
    
    if(ptr == NULL)
        return LE_FAILURE;
        
    arr->values = ptr;

    if(arr->values == NULL)
    {
        arr->size = 0;
        arr->capacity = 0;

        return LE_FAILURE;
    }

    arr->capacity = sz; 

    if(arr->size >= arr->capacity)
    {
        arr->size = arr->capacity;
    }
    
    return LE_SUCCESS;
}

leResult leArray_PushFront(leArray* arr, void* val)
{
    if(arr == NULL || arr->size == 0 || val == NULL)
        return LE_FAILURE;

    if(arr->size == arr->capacity)
    {
        if(leArray_Resize(arr, arr->capacity + STD_RESIZE_AMT) == LE_FAILURE)
            return LE_FAILURE;
    }

    _shuffleRight(arr, 0);

    arr->values[0] = val;

    return LE_SUCCESS;
}

leResult leArray_PopFront(leArray* arr)
{
    if(arr == NULL || arr->size == 0)
        return LE_FAILURE;

    _shuffleLeft(arr, 0);

    return LE_SUCCESS;
}

leResult leArray_PushBack(leArray* arr, void* val)
{
    if(arr == NULL)
        return LE_FAILURE;

    if(arr->size == arr->capacity)
    {
        if(leArray_Resize(arr, arr->capacity + STD_RESIZE_AMT) == LE_FAILURE)
            return LE_FAILURE;
    }

    arr->values[arr->size] = val;
    arr->size++;

    return LE_SUCCESS;
}

leResult leArray_PopBack(leArray* arr)
{
    if(arr == NULL || arr->size == 0)
        return LE_FAILURE;

    arr->size--;

    return LE_SUCCESS;
}

leResult leArray_InsertAt(leArray* arr,
                                uint32_t idx,
                                void* val)
{
    if(arr == NULL || idx > arr->size || val == NULL)
        return LE_FAILURE;

    if(idx == arr->size)
        return leArray_PushBack(arr, val);

    if(arr->size == arr->capacity)
    {
        if(leArray_Resize(arr, arr->capacity + STD_RESIZE_AMT) == LE_FAILURE)
            return LE_FAILURE;
    }

    _shuffleRight(arr, idx);

    arr->values[idx] = val;

    return LE_SUCCESS;
}

leResult leArray_RemoveAt(leArray* arr, uint32_t idx)
{
    if(arr == NULL || arr->size == 0 || idx >= arr->size)
        return LE_FAILURE;

    _shuffleLeft(arr, idx);

    return LE_SUCCESS;
}

leResult leArray_Remove(leArray* arr, void* val)
{
    int32_t idx;

    if(arr == NULL || arr->size == 0 || val == NULL)
        return LE_FAILURE;

    idx = leArray_Find(arr, val);

    if(idx == -1)
        return LE_FAILURE;

    _shuffleLeft(arr, idx);

    return LE_SUCCESS;
}

void* leArray_Get(const leArray* arr, uint32_t idx)
{
    if (idx >= arr->size)
        return NULL;

    return arr->values[idx];
}

leResult leArray_Set(leArray* arr, uint32_t idx, void* val)
{
    if (idx >= arr->size)
        return LE_FAILURE;

    arr->values[idx] = val;
    
    return LE_SUCCESS;
}

int32_t leArray_Find(const leArray* arr, void* val)
{
    uint32_t i;

    if(arr == NULL || arr->size == 0 || val == NULL)
        return -1;

    for(i = 0; i < arr->size; i++)
    {
        if(arr->values[i] == val)
            return i;
    }

    return -1;
}

leResult leArray_Copy(leArray* src, leArray* dest)
{
    if(src == NULL || dest == NULL)
        return LE_FAILURE;

    if(dest->capacity < src->size)
    {
        if(leArray_Resize(dest, src->size) == LE_FAILURE)
            return LE_FAILURE;
    }

    dest->size = src->size;

    if(dest->size == 0)
        return LE_SUCCESS;

    memcpy(dest->values, src->values, src->size * sizeof(void*));
    
    return LE_SUCCESS;
}

leResult leArray_Clear(leArray* arr)
{
    if(arr == NULL)
        return LE_FAILURE;

    if(arr->values != NULL)
    {
        LE_FREE(arr->values);
    }

    arr->values = NULL;
    arr->capacity = 0;
    arr->size = 0;

    return LE_SUCCESS;
}


