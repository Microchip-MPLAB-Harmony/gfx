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

#include "gfx/legato/memory/legato_fixedheap.h"

#if LE_MEMORY_MANAGER_ENABLE == 1

#include "gfx/legato/common/legato_error.h"
#include "legato_fixedheap.h"

#if LE_FIXEDHEAP_ENABLE == 1

#include <string.h>
#include <stdio.h>

/* memory state */
enum
{
    MS_ALLOC   = 0xAC,
    MS_FREE    = 0xFE, 
};

leResult leFixedHeap_Init(leFixedHeap* heap,
                          uint32_t size,
                          uint32_t count,
                          uint8_t* data)
{
    leFixedHeapBlock* block;
    uint32_t i;

#if LE_FIXEDHEAP_DEBUG == 1
    uint32_t* checksum;
#endif

    memset(heap, 0, sizeof(leFixedHeap));

    // validate args
    if(heap == NULL || count == 0)
        return LE_FAILURE;

    if(size <= sizeof(void*))
    {
        size = sizeof(void*);
    }
    
    // set up sizes
    heap->numElements = count;
    heap->logicalBlockSize = size;

    heap->physicalBlockSize = LE_FIXEDHEAP_BLOCK_SIZE(heap->logicalBlockSize);
    
    // allocate memory
    heap->data = data;

    LE_ASSERT(heap->data != NULL);

    //printf("fixed heap: 0x%08X, %u, %u\n", (uint32_t)heap->data, heap->numElements, heap->logicalBlockSize);

    if(heap->data == NULL)
        return LE_FAILURE;

#if LE_FIXEDHEAP_DEBUG == 1
    memset(heap->data, 0, heap->physicalBlockSize * heap->numElements);
#endif

    // init free list
    heap->freeList = heap->data;

    block = (leFixedHeapBlock*)heap->data;

    for (i = 0; i < heap->numElements; i++)
    {
#if LE_FIXEDHEAP_DEBUG == 1
        block->debug.checksum = (uint32_t)block ^ LE_FIXEDHEAP_CHECKSUM;
        checksum = (uint32_t*)((uint8_t*)block + heap->physicalBlockSize - LE_FIXEDHEAP_FOOTER_SIZE);
        *checksum = block->debug.checksum;
        
        block->debug.free = LE_TRUE;
#endif        

        if(i < heap->numElements - 1)
        {
            block->data = (void*)((uint8_t*)block + heap->physicalBlockSize);
            block = (leFixedHeapBlock*)(((uint8_t*)block) + heap->physicalBlockSize);
        }
        else
        {
            block->data = NULL;
        }
    }

    heap->capacity = count;
    heap->maxUsage = 0;
    heap->initialized = LE_TRUE;

    return LE_SUCCESS;
}

void leFixedHeap_Destroy(leFixedHeap* heap)
{
    if(heap == NULL || heap->initialized != LE_TRUE)
        return;

    memset(heap, 0, sizeof(leFixedHeap));

    heap->initialized = LE_FALSE;
}

#if LE_USE_DEBUG_ALLOCATOR == 1
void* leFixedHeap_Alloc(leFixedHeap* heap,
                        uint32_t lineNum,
                        const char* funcName,
                        const char* fileName)
#else
void* leFixedHeap_Alloc(leFixedHeap* heap)
#endif
{
    leFixedHeapBlock* freeBlk;
    leFixedHeapBlock* allocBlk;
    
    LE_ASSERT(heap != NULL && heap->initialized == LE_TRUE);
    
    LE_ASSERT_VAR(heap->capacity != 0 &&
                  heap->capacity <= heap->numElements &&
                  heap->freeList != NULL, "Fixed heap %u is full", heap->logicalBlockSize);

    if(heap->freeList == NULL)
        return NULL;

#if LE_FIXEDHEAP_DEBUG == 1
    LE_ASSERT(leFixedHeap_Validate(heap) == LE_SUCCESS);
#endif

    freeBlk = (leFixedHeapBlock*)heap->freeList;

    heap->freeList = freeBlk->data;

    allocBlk = (leFixedHeapBlock*)freeBlk;

#if LE_FIXEDHEAP_DEBUG == 1
#if LE_USE_ALLOCATION_TRACKING == 1
    allocBlk->debug.lineNumber = lineNum;
    allocBlk->debug.funcName = funcName;
    allocBlk->debug.fileName = fileName;
#endif

    allocBlk->debug.free = LE_FALSE;
    
    allocBlk->data = NULL;

    memset(&allocBlk->data, MS_ALLOC, heap->logicalBlockSize);
#endif

    heap->capacity--;

    if(heap->capacity == 0)
    {
        heap->freeList = NULL;
    }
    
#if LE_FIXEDHEAP_DEBUG == 1
    LE_ASSERT(leFixedHeap_Validate(heap) == LE_SUCCESS);
#endif
    
    if(heap->numElements - heap->capacity > heap->maxUsage)
    {
        heap->maxUsage = heap->numElements - heap->capacity;
    }

    return &allocBlk->data;
}

void leFixedHeap_Free(leFixedHeap* heap,
                      void* ptr)
{
    leFixedHeapBlock *oldFreeBlk, *freeBlk;

    LE_ASSERT(heap != NULL && heap->initialized == LE_TRUE);
    
    LE_ASSERT_VAR(heap->capacity != heap->numElements, "Trying to free from an empty heap (%u)", heap->logicalBlockSize);

#if LE_FIXEDHEAP_DEBUG == 1
    LE_ASSERT(leFixedHeap_Validate(heap) == LE_SUCCESS);
#endif

    oldFreeBlk = (leFixedHeapBlock*)heap->freeList;
    freeBlk = (leFixedHeapBlock*)((uint8_t*)ptr - LE_FIXEDHEAP_HEADER_SIZE);

#if LE_FIXEDHEAP_DEBUG == 1
    LE_ASSERT_MSG(freeBlk->debug.free != LE_TRUE, "Trying to free an address that hasn't been allocated");

    memset(&freeBlk->data, MS_FREE, heap->logicalBlockSize);

    freeBlk->debug.free = LE_TRUE;
#endif

    freeBlk->data = (void*)oldFreeBlk;
    heap->freeList = (void*)freeBlk;

    //_ASSERT(KT_FixedHeap_Contains(heap, heap->freeList) == KT_TRUE);

    heap->capacity++;

#if LE_FIXEDHEAP_DEBUG == 1
    LE_ASSERT(leFixedHeap_Validate(heap) == LE_SUCCESS);
#endif
}

leBool leFixedHeap_Contains(leFixedHeap* heap, void* ptr)
{
    uint8_t *data, *dataPtr;

    LE_ASSERT(heap != NULL && heap->initialized == LE_TRUE);

    if(heap->data == NULL)
        return LE_FALSE;

    data = (uint8_t*)heap->data;
    dataPtr = (uint8_t*)ptr;

	if(dataPtr < data || dataPtr >= data + (heap->physicalBlockSize * heap->numElements))
		return LE_FALSE;

	dataPtr -= (size_t)data;
	
	return ((size_t)dataPtr % heap->physicalBlockSize) == LE_FIXEDHEAP_HEADER_SIZE;
}

leResult leFixedHeap_Validate(struct leFixedHeap* heap)
{
#if LE_FIXEDHEAP_DEBUG == 1
    leFixedHeapBlock* blk;
    uint32_t* ftr;
    uint32_t i;

    blk = heap->data;

    for(i = 0; i < heap->numElements; i++)
    {
        // validate checksums
        if(blk->debug.checksum != ((uint32_t)blk ^ LE_FIXEDHEAP_CHECKSUM))
            return LE_FAILURE;

        ftr = (uint32_t*)((uint8_t*)blk + heap->physicalBlockSize - sizeof(uint32_t));

        if(*ftr != blk->debug.checksum)
            return LE_FAILURE;
    }

    i = 0;

    while(1)
    {
        if(i == heap->numElements - heap->capacity)
            break;

        blk = heap->freeList;

        blk = (leFixedHeapBlock*)blk->data;

        i++;
    }
#else
    (void)heap; // unused
#endif

    return LE_SUCCESS;
}

#ifdef _WIN32
void leFixedHeap_Dump(leFixedHeap* heap,
                      leBool dumpRecords)
{
    leFixedHeapBlock* blk = NULL;
    uint32_t i = 0;

    blk = heap->data;
    
    printf("Fixed Heap Data:\n");
    printf("  Physical Block Size %u\n", heap->physicalBlockSize);
    printf("  Logical Block Size: %u\n", heap->logicalBlockSize);
    printf("  Total Capacity:     %u\n", heap->numElements);
    printf("  Current Capacity:   %u\n", heap->capacity);
    printf("  Max Usage:          %u\n", heap->maxUsage);
    printf("\n\n");
    
#if LE_USE_ALLOCATION_TRACKING == 1
    if(dumpRecords == LE_TRUE)
    {
        printf("  Block allocation information:\n");
        
        for(i = 0; i < heap->numElements; i++)
        {
            // valid checksum == valid block
            if(blk->debug.free == LE_FALSE &&
               blk->debug.checksum == ((uint32_t)blk ^ LE_FIXEDHEAP_CHECKSUM))
            {
                printf("    Block %u\n", i);
                printf("      File:     %s\n", blk->debug.fileName);
                printf("      Line:     %u\n", blk->debug.lineNumber);
                printf("      Function: %s\n", blk->debug.funcName);
                printf("\n\n");
            }

            ((uint8_t*)blk) += heap->physicalBlockSize;
        }
    }
#endif
    
}
#endif

#endif

#endif // LE_MEMORY_MANAGER_ENABLE