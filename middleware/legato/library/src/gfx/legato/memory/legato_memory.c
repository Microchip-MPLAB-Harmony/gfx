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


#include "gfx/legato/memory/legato_memory.h"

#if LE_MEMORY_MANAGER_ENABLE == 1

#include <stdlib.h>
#include <string.h>

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/memory/legato_variableheap.h"

#if LE_FIXEDHEAP_ENABLE == 1

#define FIXED_HEAP_COUNT 6

#include "gfx/legato/memory/legato_fixedheap.h"

#if LE_DEBUG == 0
static uint8_t LE_COHERENT_MEMORY fixedHeap16[LE_FIXEDHEAP_BLOCK_SIZE(16) * LE_FIXEDHEAP_SIZE_16];
static uint8_t LE_COHERENT_MEMORY fixedHeap32[LE_FIXEDHEAP_BLOCK_SIZE(32) * LE_FIXEDHEAP_SIZE_32];
static uint8_t LE_COHERENT_MEMORY fixedHeap64[LE_FIXEDHEAP_BLOCK_SIZE(64) * LE_FIXEDHEAP_SIZE_64];
static uint8_t LE_COHERENT_MEMORY fixedHeap128[LE_FIXEDHEAP_BLOCK_SIZE(128) * LE_FIXEDHEAP_SIZE_128];
static uint8_t LE_COHERENT_MEMORY fixedHeap196[LE_FIXEDHEAP_BLOCK_SIZE(196) * LE_FIXEDHEAP_SIZE_196];
static uint8_t LE_COHERENT_MEMORY fixedHeap256[LE_FIXEDHEAP_BLOCK_SIZE(256) * LE_FIXEDHEAP_SIZE_256];

static uint8_t* fixedHeapData[FIXED_HEAP_COUNT] =
{
    (void*)&fixedHeap16,
    (void*)&fixedHeap32,
    (void*)&fixedHeap64,
    (void*)&fixedHeap128,
    (void*)&fixedHeap196,
    (void*)&fixedHeap256
};
#endif // LE_DEBUG

typedef struct leFixedMemoryPoolDef
{
    uint32_t size;
    uint32_t count;
    void* data;
} leFixedMemoryPoolDef;

static leFixedMemoryPoolDef fixedPoolDefs[] =
{
    { 16, LE_FIXEDHEAP_SIZE_16, NULL },
    { 32, LE_FIXEDHEAP_SIZE_32, NULL },
    { 64, LE_FIXEDHEAP_SIZE_64, NULL },
    { 128, LE_FIXEDHEAP_SIZE_128, NULL },
    { 196, LE_FIXEDHEAP_SIZE_196, NULL },
    { 256, LE_FIXEDHEAP_SIZE_256, NULL },
    { 0, 0, NULL }
};

static leFixedHeap fixedHeaps[LE_FIXED_HEAP_COUNT];

#endif // LE_FIXEDHEAP_ENABLE

#if LE_DEBUG == 0
static uint8_t LE_COHERENT_MEMORY variableHeapData[LE_VARIABLEHEAP_SIZE];
#else
static uint8_t* variableHeapData;
#endif

static leVariableHeap variableHeap;

leResult leMemory_Init()
{
#if LE_FIXEDHEAP_ENABLE == 1
    uint32_t i = 0;
    leFixedHeap* heap;

    memset(&fixedHeaps, 0, sizeof(fixedHeaps));


    for(i = 0; i < FIXED_HEAP_COUNT; ++i)
    {
#if LE_DEBUG == 0
        fixedPoolDefs[i].data = fixedHeapData[i];
#else
        fixedPoolDefs[i].data = malloc(fixedPoolDefs[i].size * fixedPoolDefs[i].count);
#endif
    }

    i = 0;

    while(fixedPoolDefs[i].data != NULL)
    {
        heap = &fixedHeaps[i];

        leFixedHeap_Init(heap,
                         fixedPoolDefs[i].size,
                         fixedPoolDefs[i].count,
                         fixedPoolDefs[i].data);

        i++;
     }
#endif // LE_FIXEDHEAP_ENABLE

#if LE_DEBUG == 1
    variableHeapData = malloc(LE_VARIABLEHEAP_SIZE);
#endif

    leVariableHeap_Init(&variableHeap,
                        variableHeapData,
                        LE_VARIABLEHEAP_SIZE);

    return LE_SUCCESS;
}

#if LE_FIXEDHEAP_ENABLE == 1
#if LE_USE_DEBUG_ALLOCATOR == 1
static void* _fixedHeapAlloc(uint32_t size, uint32_t line, const char* func, const char* file)
#else
static void* _fixedHeapAlloc(uint32_t size)
#endif
{
    uint32_t i = 0;
    void* ptr;

    /* try to find a fixed heap that this will fit in */
    while(fixedHeaps[i].initialized == LE_TRUE)
    {
        if(size < fixedHeaps[i].logicalBlockSize)
        {
            ptr = LE_FHEAP_ALLOC(&fixedHeaps[i]);

            if(ptr != NULL)
            {
#if LE_MALLOC_ZEROIZE == 1
                memset(ptr, 0, size);
#endif

                return ptr;
            }
        }

        i++;
    }

    return NULL;
}



static int32_t _findFixedHeapForPointer(void* ptr)
{
    int32_t i = 0;

    /* try to find the fixed heap that contains this pointer */
    while(fixedHeaps[i].initialized == LE_TRUE)
    {
        /* make sure the heap actually contains this pointer */
        if(leFixedHeap_Contains(&fixedHeaps[i], ptr) == LE_TRUE)
        {
            return i;
        }

        i++;
    }

    return -1;
}

static int32_t _findFixedHeapSizeForPointer(void* ptr)
{
    int32_t i = 0;

    /* try to find the fixed heap that contains this pointer */
    while(fixedHeaps[i].initialized == LE_TRUE)
    {
        /* make sure the heap actually contains this pointer */
        if(leFixedHeap_Contains(&fixedHeaps[i], ptr) == LE_TRUE)
        {
            return fixedHeaps[i].logicalBlockSize;
        }

        i++;
    }

    return -1;
}

static leResult _fixedHeapFree(void* ptr)
{
    int32_t heapIdx = _findFixedHeapForPointer(ptr);

    if(heapIdx == -1)
        return LE_FAILURE;

    leFixedHeap_Free(&fixedHeaps[heapIdx], ptr);

    return LE_SUCCESS;
}

static int32_t _getFixedHeapForSize(uint32_t size)
{
    int32_t i = 0;

    /* try to find a fixed heap that this will fit in */
    while(fixedHeaps[i].initialized == LE_TRUE)
    {
        if(size < fixedHeaps[i].logicalBlockSize)
            return i;

        i++;
    }

    return -1;
}

#if LE_USE_DEBUG_ALLOCATOR == 1
static void* _fixedHeapRealloc(void* ptr, uint32_t size, uint32_t line, const char* func, const char* file)
#else
static void* _fixedHeapRealloc(void* ptr, uint32_t size)
#endif
{
    int32_t current = _findFixedHeapForPointer(ptr);
    int32_t target = _getFixedHeapForSize(size);
    void* newPtr;

    if(target == -1)
        return NULL;

    if(current == target)
        return ptr;

    /* try to find a fixed heap that this will fit in */
    for(; target < LE_FIXED_HEAP_COUNT; ++target)
    {
        if(fixedHeaps[target].capacity > 0)
            break;
    }

    // no room
    if(target == LE_FIXED_HEAP_COUNT)
        return NULL;

    if(target == current)
        return ptr;

    newPtr = LE_FHEAP_ALLOC(&fixedHeaps[target]);

    LE_ASSERT(newPtr != NULL);

    if(newPtr != NULL)
    {
#if LE_MALLOC_ZEROIZE == 1
        memset(newPtr, 0, size);
#endif

        // copy the data
        if(target > current)
        {
            /* new block can completely contain old data */
            memcpy(newPtr, ptr, size);
        }
        else
        {
            /* need to truncate old data for new block */
            memcpy(newPtr, ptr, size);
        }

        leFixedHeap_Free(&fixedHeaps[current], ptr);

#if LE_FIXEDHEAP_DEBUG == 1
        LE_ASSERT(leFixedHeap_Validate(&fixedHeaps[current]) == LE_SUCCESS);
#endif

        return newPtr;
    }

    return NULL;
}
#endif

#if LE_USE_DEBUG_ALLOCATOR == 1
void* leMalloc(size_t size,
               size_t line,
               const char* func,
               const char* file)
#else
void* leMalloc(size_t size)
#endif
{
    void* ptr;

#if LE_FIXEDHEAP_ENABLE == 1
#if LE_USE_DEBUG_ALLOCATOR == 1
    ptr = _fixedHeapAlloc(size, line, func, file);
#else
    ptr = _fixedHeapAlloc(size);
#endif


    if(ptr != NULL)
        return ptr;

#endif

    /* use the dynamic allocator to handle anything the fixed heaps can't fit */
    ptr = LE_VHEAP_ALLOC(&variableHeap, size);

    LE_ASSERT(ptr != NULL);

    if(ptr != NULL)
    {
#if LE_MALLOC_ZEROIZE == 1
        memset(ptr, 0, size);
#endif

        //leMemoryPrintReport();

        return ptr;
    }

    return NULL;
}

#if LE_USE_DEBUG_ALLOCATOR == 1
void* leRealloc(void* ptr,
                size_t size,
                size_t line,
                const char* func,
                const char* file)
#else
void* leRealloc(void* ptr, size_t size)
#endif
{
    void* newPtr = NULL;

#if LE_FIXEDHEAP_ENABLE == 1
    uint32_t oldSize = 0;
    uint32_t fixedHeapSize;
    int32_t fixedHeapIndex;
#endif

    if(ptr == NULL)
    {
        return LE_MALLOC(size);
    }

    if(size == 0)
    {
        LE_FREE(ptr);

        return NULL;
    }

#if LE_FIXEDHEAP_ENABLE == 1
    fixedHeapIndex = _findFixedHeapForPointer(ptr);

    if(fixedHeapIndex >= 0)
    {
#if LE_USE_DEBUG_ALLOCATOR == 1
        newPtr = _fixedHeapRealloc(ptr, size, line, func, file);
#else
        newPtr = _fixedHeapRealloc(ptr, size);
#endif

        if(newPtr != NULL)
            return newPtr;

        // allocate from variable heap
#if LE_USE_DEBUG_ALLOCATOR == 1
        newPtr = leVariableHeap_Alloc(&variableHeap, size, line, func, file);
#else
        newPtr = leVariableHeap_Alloc(&variableHeap, size);
#endif

        fixedHeapSize = _findFixedHeapSizeForPointer(ptr);

        // copy the data
        if(fixedHeapSize < size)
        {
            /* new block can completely contain old data */
            memcpy(newPtr, ptr, size);
        }
        else
        {
            /* need to truncate old data for new block */
            memcpy(newPtr, ptr, size);
        }

        LE_FREE(ptr);

        return newPtr;
    }
#endif

    // not an allocated pointer, just do normal malloc
    if(leVariableHeap_Contains(&variableHeap, ptr) == LE_FALSE)
        return LE_MALLOC(size);

#if LE_FIXEDHEAP_ENABLE == 1
    oldSize = leVariableHeap_SizeOf(&variableHeap, ptr);

    // see if this will fit in a fixed pool

#if LE_USE_DEBUG_ALLOCATOR == 1
    newPtr = _fixedHeapAlloc(size, line, func, file);
#else
    newPtr = _fixedHeapAlloc(size);
#endif

    // found a fixed pool to hold the data
    if(newPtr != NULL)
    {
        // copy the data
        if(oldSize < size)
        {
            /* new block can completely contain old data */
            memcpy(newPtr, ptr, size);
        }
        else
        {
            /* need to truncate old data for new block */
            memcpy(newPtr, ptr, size);
        }

        LE_FREE(ptr);

        //leMemoryPrintReport();

        return newPtr;
    }
#endif

#if LE_USE_DEBUG_ALLOCATOR == 1
    newPtr = leVariableHeap_Realloc(&variableHeap, ptr, size, line, func, file);
#else
    newPtr = leVariableHeap_Realloc(&variableHeap, ptr, size);
#endif

    LE_ASSERT(newPtr != NULL);

    //leMemoryPrintReport();

    return newPtr;
}

void leFree(void* ptr)
{
    if(ptr == NULL)
        return;

    leVariableHeap_Validate(&variableHeap);

#if LE_FIXEDHEAP_ENABLE == 1
    if(_fixedHeapFree(ptr) == LE_SUCCESS)
    {
        leVariableHeap_Validate(&variableHeap);

        return;
    }
#endif

    leVariableHeap_Validate(&variableHeap);

    /* attempt to free from the dynamic heap */
    leVariableHeap_Free(&variableHeap, ptr);

    leVariableHeap_Validate(&variableHeap);

    //leMemoryPrintReport();
}

void leMemoryGetUsageReport(leMemoryStatusReport* rpt)
{
#if LE_FIXEDHEAP_ENABLE == 1
    size_t idx;
#endif

    if(rpt == NULL)
        return;

    rpt->variableHeapReport.size = variableHeap.size;
    rpt->variableHeapReport.maxUsage = variableHeap.maxUsage;
    rpt->variableHeapReport.used = variableHeap.used;

#if LE_FIXEDHEAP_ENABLE == 1
    for(idx = 0; idx < LE_FIXED_HEAP_COUNT - 1; idx++)
    {
        rpt->fixedHeapReport[idx].logicalSize = fixedHeaps[idx].logicalBlockSize;
        rpt->fixedHeapReport[idx].numElements = fixedHeaps[idx].numElements;
        rpt->fixedHeapReport[idx].maxUsage = fixedHeaps[idx].maxUsage;
        rpt->fixedHeapReport[idx].currentCapacity = fixedHeaps[idx].capacity;
    }
#endif
}

#include <stdio.h>

void leMemoryPrintReport(void)
{
    leVariableHeap_Dump(&variableHeap, LE_TRUE);
#if 0
#if LE_FIXEDHEAP_ENABLE == 1
    size_t idx;
#endif

    printf("Variable Heap Status:\n");
    printf("    Total Size:    %u\n", variableHeap.size);
    printf("    Max Usage:     %u\n", variableHeap.maxUsage);
    printf("    Current Usage: %u\n\n", variableHeap.used);

#if LE_FIXEDHEAP_ENABLE == 1
    for(idx = 0; idx < LE_FIXED_HEAP_COUNT - 1; idx++)
    {
        printf("Fixed Heap (%u) Status:\n", fixedHeaps[idx].logicalBlockSize);
        printf("    Number of Elements:  %u\n", fixedHeaps[idx].numElements);
        printf("    Max Usage:           %u\n", fixedHeaps[idx].maxUsage);
        printf("    Current Usage:       %u\n\n", fixedHeaps[idx].capacity);
    }
#endif
#endif
}

#ifdef _WIN32
void leMemoryDumpAllocations()
{
    size_t idx;

    leVariableHeap_Dump(&variableHeap, LE_TRUE);

    for(idx = 0; idx < LE_FIXED_HEAP_COUNT - 1; idx++)
    {
        leFixedHeap_Dump(&fixedHeaps[idx], LE_TRUE);
    }
}
#endif

leResult leMemoryValidateHeaps()
{
#if LE_FIXEDHEAP_ENABLE == 1
    size_t idx;
#endif

    if(leVariableHeap_Validate(&variableHeap) == LE_FAILURE)
        return LE_FAILURE;

#if LE_FIXEDHEAP_ENABLE == 1
    for(idx = 0; idx < LE_FIXED_HEAP_COUNT - 1; idx++)
    {
        if(leFixedHeap_Validate(&fixedHeaps[idx]) == LE_FAILURE)
            return LE_FAILURE;
    }
#endif

    return LE_SUCCESS;
}

#endif // LE_MEMORY_MANAGER_ENABLE
