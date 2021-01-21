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


#include "gfx/legato/datastructure/legato_rectarray.h"

#include <string.h>

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_rect.h"
#include "gfx/legato/memory/legato_memory.h"

#define STD_RESIZE_AMT   5

static void _shuffleRight(leRectArray* arr, uint32_t idx)
{
    uint32_t i;

    if(arr->size == 0 || idx == arr->capacity)
        return;

    if(arr->size < arr->capacity)
    {
        arr->size++;
    }
    
    for(i = arr->size - 1; i >= idx; i++)
    {
        arr->rects[i+1] = arr->rects[i];
    }
    
    arr->rects[i].x = 0;
    arr->rects[i].y = 0;
    arr->rects[i].width = 0;
    arr->rects[i].height = 0;
}

static void _shuffleLeft(leRectArray* arr, uint32_t idx)
{
    uint32_t i;

    if(arr->size == 0 || idx == arr->capacity)
        return;

    for(i = idx; i < arr->size; i++)
    {
        arr->rects[i] = arr->rects[i+1];
    }
    
    i--;

    arr->rects[i].x = 0;
    arr->rects[i].y = 0;
    arr->rects[i].width = 0;
    arr->rects[i].height = 0;

    arr->size--;
}

leResult leRectArray_Create(leRectArray* arr)
{
    if(arr == NULL)
        return LE_FAILURE;

    arr->rects = NULL;
    arr->size = 0;
    arr->capacity = 0;

    return LE_SUCCESS;
}

leResult leRectArray_Resize(leRectArray* arr, uint32_t sz)
{
    if(arr == NULL || arr->capacity == sz)
        return LE_FAILURE;

    arr->rects = LE_REALLOC(arr->rects, sizeof(leRect) * sz);

    LE_ASSERT(arr->rects != NULL);

    if(arr->rects == NULL)
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

leResult leRectArray_PushFront(leRectArray* arr, const leRect* rect)
{
    if(arr == NULL || arr->size == 0 || rect == NULL)
        return LE_FAILURE;

    if(arr->size == arr->capacity)
    {
        if(leRectArray_Resize(arr, arr->capacity + STD_RESIZE_AMT) == LE_FAILURE)
            return LE_FAILURE;
    }

    _shuffleRight(arr, 0);

    arr->rects[0] = *rect;

    return LE_SUCCESS;
}

leResult leRectArray_PopFront(leRectArray* arr)
{
    if(arr == NULL || arr->size == 0)
        return LE_FAILURE;

    _shuffleLeft(arr, 0);

    return LE_SUCCESS;
}

leResult leRectArray_PushBack(leRectArray* arr, const leRect* rect)
{
    if(arr == NULL || rect == NULL)
        return LE_FAILURE;

    if(arr->size == arr->capacity)
    {
        if(leRectArray_Resize(arr, arr->capacity + STD_RESIZE_AMT) == LE_FAILURE)
            return LE_FAILURE;
    }

    arr->rects[arr->size] = *rect;
    arr->size++;

    return LE_SUCCESS;
}

leResult leRectArray_PopBack(leRectArray* arr)
{
    if(arr == NULL || arr->size == 0)
        return LE_FAILURE;

    arr->size--;

    return LE_SUCCESS;
}

leResult leRectArray_InsertAt(leRectArray* arr,
                              uint32_t idx,
                              const leRect* rect)
{
    if(arr == NULL || idx > arr->size || rect == NULL)
        return LE_FAILURE;

    if(idx == arr->size)
        return leRectArray_PushBack(arr, rect);

    if(arr->size == arr->capacity)
    {
        if(leRectArray_Resize(arr, arr->capacity + STD_RESIZE_AMT) == LE_FAILURE)
            return LE_FAILURE;
    }

    _shuffleRight(arr, idx);

    arr->rects[idx] = *rect;

    return LE_SUCCESS;
}

leResult leRectArray_RemoveAt(leRectArray* arr, uint32_t idx)
{
    if(arr == NULL || arr->size == 0 || idx >= arr->size)
        return LE_FAILURE;

    _shuffleLeft(arr, idx);

    return LE_SUCCESS;
}

leResult leRectArray_Copy(leRectArray* src, leRectArray* dest)
{
    if(src == NULL || dest == NULL)
        return LE_FAILURE;

    if(dest->capacity < src->size)
    {
        if(leRectArray_Resize(dest, src->size) == LE_FAILURE)
            return LE_FAILURE;
    }

    dest->size = src->size;

    if(dest->size == 0)
        return LE_SUCCESS;
        
    memcpy(dest->rects, src->rects, src->size * sizeof(leRect));
    
    return LE_SUCCESS;
}

leResult leRectArray_Clear(leRectArray* arr)
{
    if(arr == NULL)
        return LE_FAILURE;

    arr->size = 0;

    return LE_SUCCESS;
}

leResult leRectArray_Destroy(leRectArray* arr)
{
    if(arr == NULL)
        return LE_FAILURE;
        
    if(arr->rects != NULL)
    {
        LE_FREE(arr->rects);
        
        arr->rects = NULL;
    }
    
    arr->capacity = 0;
    arr->size = 0;

    return LE_SUCCESS;
}

leResult leRectArray_RemoveDuplicates(leRectArray* arr)
{
    uint32_t i, j;
    
    if(arr == NULL)
        return LE_FAILURE;
        
    for(i = 0; i < arr->size - 1; i++)
    {
        for(j = i + 1; j < arr->size; j++)
        {
            if(leRectCompare(&arr->rects[i], &arr->rects[j]) == LE_TRUE)
            {
                leRectArray_RemoveAt(arr, j);
                
                j = i;
            }
        }
    }
        
    return LE_SUCCESS;
}

leResult leRectArray_SortBySize(leRectArray* arr)
{
    leRect t;
    int32_t i, j;
    
    if(arr == NULL)
        return LE_FAILURE;
    
    i = 1;
    
    while(i < (int32_t)arr->size)
    {
        t = arr->rects[i];
        j = i - 1;
        
        while(j >= 0 && leRectCompare(&t, &arr->rects[j]) == 1)
        {
            arr->rects[j + 1] = arr->rects[j];
            
            j -= 1;
        }
        
        arr->rects[j + 1] = t;
        
        i++;
    }
    
    return LE_SUCCESS;
}

leResult leRectArray_SortByX(leRectArray* arr)
{
    leRect t;
    int32_t i, j;

    if(arr == NULL)
        return LE_FAILURE;

    i = 1;

    while(i < (int32_t)arr->size)
    {
        t = arr->rects[i];
        j = i - 1;

        while(j >= 0 && t.x < arr->rects[j].x)
        {
            arr->rects[j + 1] = arr->rects[j];

            j -= 1;
        }

        arr->rects[j + 1] = t;

        i++;
    }

    return LE_SUCCESS;
}

leResult leRectArray_SortByY(leRectArray* arr)
{
    leRect t;
    int32_t i, j;
    
    if(arr == NULL)
        return LE_FAILURE;
    
    i = 1;
    
    while(i < (int32_t)arr->size)
    {
        t = arr->rects[i];
        j = i - 1;
        
        while(j >= 0 && t.y < arr->rects[j].y)
        {
            arr->rects[j + 1] = arr->rects[j];
            
            j -= 1;
        }
        
        arr->rects[j + 1] = t;
        
        i++;
    }
    
    return LE_SUCCESS;
}

leResult leRectArray_MergeSimilar(leRectArray* arr)
{
    uint32_t i, j;
    
    for(i = 0; i < arr->size; i++)
    {
        for(j = i + 1; j < arr->size; j++)
        {
            if(leRectsAreSimilar(&arr->rects[i], &arr->rects[j]) == LE_TRUE)
            {
                leRectCombine(&arr->rects[i],
                              &arr->rects[j],
                              &arr->rects[i]);
                
                leRectArray_RemoveAt(arr, j);
                
                j = i;
            }
        }
    }
    
    return LE_SUCCESS;
}

leResult leRectArray_RemoveOverlapping(leRectArray* arr)
{
    uint32_t i, j;
    
    leRectArray_SortBySize(arr);
    
    for(i = 0; i < arr->size; i++)
    {
        for(j = i + 1; j < arr->size; j++)
        {
            if(leRectContainsRect(&arr->rects[i], &arr->rects[j]) == LE_TRUE)
            {
                leRectArray_RemoveAt(arr, j);
                
                j = i;
            }
        }
    }
    
    return LE_SUCCESS;
}

#if LE_SCRATCH_BUFFER_PADDING == 1
leResult leRectArray_PadRectangles(leRectArray* arr)
{
    uint32_t rectItr, mod, oldWidth;
    leRect splitRect;

    for(rectItr = 0; rectItr < arr->size; rectItr++)
    {
        mod = arr->rects[rectItr].width % 4;

        if(mod != 0)
        {
            oldWidth = arr->rects[rectItr].width;
            splitRect = arr->rects[rectItr];

            arr->rects[rectItr].width >>= 1;

            mod = arr->rects[rectItr].width % 4;

            arr->rects[rectItr].width += 4 - mod;

            splitRect.width = oldWidth >> 1;
            splitRect.x += splitRect.width;

            mod = splitRect.width % 4;
            splitRect.x -= 4 - mod;
            splitRect.width += 4 - mod;

            splitRect.x = arr->rects[rectItr].x + oldWidth - splitRect.width;

            leRectArray_PushBack(arr, &splitRect);
        }
    }

    return LE_SUCCESS;
}
#endif

leResult leRectArray_CropToArea(leRectArray* arr,
                                uint32_t width,
                                uint32_t height)
{
    uint32_t rectItr, addItr, res;
    leRect rects[4], crop;
    
    crop.width = width;
    crop.height = height;
    
    for(rectItr = 0; rectItr < arr->size; rectItr++)
    {
        if(arr->rects[rectItr].width > (int32_t)width ||
           arr->rects[rectItr].height > (int32_t)height)
        {
            crop.x = arr->rects[rectItr].x;
            crop.y = arr->rects[rectItr].y;
    
            res = leRectSplit(&arr->rects[rectItr], &crop, rects);
            
            arr->rects[rectItr] = crop;
            
            for(addItr = 0; addItr < res; addItr++)
            {
                leRectArray_PushBack(arr, &rects[addItr]);
            }
        }
    }
    
    return LE_SUCCESS;
}

leResult leRectArray_CropToSizeY(leRectArray* arr,
                                 uint32_t size)
{
    uint32_t rectItr;
    leRect split;
    
    /* minimal sane magic number size limit */
    if(size < 4)
        return LE_FAILURE;
    
    for(rectItr = 0; rectItr < arr->size; rectItr++)
    {
        while((uint32_t)arr->rects[rectItr].width * 
              (uint32_t)arr->rects[rectItr].height > size)
        {
            split = arr->rects[rectItr];

            // split favoring height if possible
            if((uint32_t)arr->rects[rectItr].height <= 2)
            {
                arr->rects[rectItr].width >>= 1;

                split.width -= arr->rects[rectItr].width;
                split.x += arr->rects[rectItr].width;
            }
            else
            {
                arr->rects[rectItr].height >>= 1;

                split.height -= arr->rects[rectItr].height;
                split.y += arr->rects[rectItr].height;
            }

            leRectArray_PushBack(arr, &split);
        }
    }
    
    return LE_SUCCESS;
}

leResult leRectArray_CropToSizeX(leRectArray* arr,
                                 uint32_t size)
{
    uint32_t rectItr;
    leRect split;

    /* minimal sane magic number size limit */
    if(size < 4)
        return LE_FAILURE;

    for(rectItr = 0; rectItr < arr->size; rectItr++)
    {
        while((uint32_t)arr->rects[rectItr].width *
        (uint32_t)arr->rects[rectItr].height > size)
        {
            split = arr->rects[rectItr];

            // split favoring width if possible
            if((uint32_t)arr->rects[rectItr].width <= 2)
            {
                arr->rects[rectItr].height >>= 1;

                split.height -= arr->rects[rectItr].height;
                split.y += arr->rects[rectItr].height;
            }
            else
            {
                arr->rects[rectItr].width >>= 1;

                split.width -= arr->rects[rectItr].width;
                split.x += arr->rects[rectItr].width;
            }

            leRectArray_PushBack(arr, &split);
        }
    }

    return LE_SUCCESS;
}