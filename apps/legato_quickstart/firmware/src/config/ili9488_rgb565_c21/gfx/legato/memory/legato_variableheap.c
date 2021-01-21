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

#include "gfx/legato/memory/legato_variableheap.h"

#include "gfx/legato/common/legato_error.h"

#include <string.h>
#include <stdio.h>

#define LE_VARIABLEHEAP_BLOCK_HEADER_SIZE   (sizeof(leVariableHeapBlockHeader))

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
#define LE_VARIABLEHEAP_BLOCK_FOOTER_SIZE   (sizeof(uint32_t))
#else
#define LE_VARIABLEHEAP_BLOCK_FOOTER_SIZE   0
#endif

#define LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE (LE_VARIABLEHEAP_BLOCK_HEADER_SIZE + LE_VARIABLEHEAP_BLOCK_FOOTER_SIZE)

#define LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(blk)   (blk->capacity + LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE)
#define LE_VARIABLEHEAP_BLOCK_PADDING_SIZE(blk) (blk->capacity - blk->size)
#define LE_VARIABLEHEAP_BLOCK_DATA_PTR(blk)     ((uint8_t*)blk + LE_VARIABLEHEAP_BLOCK_HEADER_SIZE)
#define LE_VARIABLEHEAP_BLOCK_FOOTER_PTR(blk)   ((uint8_t*)blk + LE_VARIABLEHEAP_BLOCK_HEADER_SIZE + blk->capacity)
#define LE_VARIABLEHEAP_BLOCK_PADDING_PTR(blk)  (LE_VARIABLEHEAP_BLOCK_FOOTER_PTR(blk) - LE_VARIABLEHEAP_BLOCK_PADDING_SIZE(blk))

#define LE_VARIABLEHEAP_BLOCK_MINIMUM_SIZE      (LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE + LE_VARIABLEHEAP_MINALLOCSIZE)

#define LE_VARIABLEHEAP_BLOCK_CHECKSUM          (-1)


/* memory state */
enum
{
    MS_ALLOC   = 0xAC,
    MS_FREE    = 0xFE,
    MS_PADDING = 0xAB
};

struct leVariableHeapBlockHeader;

typedef struct leVariableHeapBlockHeader
{
    uint32_t capacity;
    uint32_t size;

    struct leVariableHeapBlockHeader* prev;
    struct leVariableHeapBlockHeader* next;

    leBool free;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
#if LE_USE_ALLOCATION_TRACKING == 1
    uint32_t type;
    uint32_t lineNumber;
    const char* funcName;
    const char* fileName;
#endif

    uint32_t checksum;
#endif
} leVariableHeapBlockHeader;

static leBool _areAdjacent(leVariableHeapBlockHeader* l, leVariableHeapBlockHeader* r)
{
    return (uint8_t*)l + LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(l) == (uint8_t*)r;
}

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
static leResult _validateBlock(leVariableHeapBlockHeader* blk)
{
    uint8_t* ptr;
    uint32_t i;

    LE_ASSERT((uint32_t)blk != 0xFEFEFEFE);
    LE_ASSERT((uint32_t)blk->prev != 0xFEFEFEFE);
    LE_ASSERT((uint32_t)blk->next != 0xFEFEFEFE);

    if(blk->prev != NULL)
        LE_ASSERT(blk->prev->next == blk);

    if(blk->next != NULL)
        LE_ASSERT(blk->next->prev == blk);

    // validate header checksum

    LE_ASSERT(blk->checksum == ((uint32_t)blk ^ LE_VARIABLEHEAP_BLOCK_CHECKSUM));

    if(blk->checksum != ((uint32_t)blk ^ LE_VARIABLEHEAP_BLOCK_CHECKSUM))
        return LE_FAILURE;

    // validate footer checksum
    ptr = LE_VARIABLEHEAP_BLOCK_FOOTER_PTR(blk);

    LE_ASSERT(*(uint32_t*)ptr == blk->checksum);

    if(*(uint32_t*)ptr != blk->checksum)
        return LE_FAILURE;

    // validate any padding values
    if(blk->free == LE_FALSE && blk->size < blk->capacity)
    {
        ptr -= LE_VARIABLEHEAP_BLOCK_PADDING_SIZE(blk);

        for(i = 0; i < LE_VARIABLEHEAP_BLOCK_PADDING_SIZE(blk); i++)
        {
            LE_ASSERT(ptr[i] == MS_PADDING);

            if(ptr[i] != MS_PADDING)
                return LE_FAILURE;
        }
    }

    // if free, test fill values
    if(blk->free == LE_TRUE)
    {
        ptr = LE_VARIABLEHEAP_BLOCK_DATA_PTR(blk);

        for(i = 0; i < blk->capacity; i++)
        {
            LE_ASSERT(ptr[i] == MS_FREE);

            if(ptr[i] != MS_FREE)
                return LE_FAILURE;
        }
    }

    return LE_SUCCESS;
}

static leResult _validateList(leVariableHeapBlockHeader* blk)
{
    leResult res;

    while(blk != NULL)
    {
        res = _validateBlock(blk);

        LE_ASSERT(res == LE_SUCCESS);

        if(res == LE_FAILURE)
            return LE_FAILURE;

        blk = blk->next;
    }

    return LE_SUCCESS;
}
#endif

#if LE_USE_DEBUG_ALLOCATOR == 1
#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
static uint32_t _countList(leVariableHeapBlockHeader* blk)
{
    uint32_t count = 0;

    while(blk != NULL)
    {
        count++;

        blk = blk->next;
    }

    return count;
}
#endif
#endif

static leVariableHeapBlockHeader* _findSlot(leVariableHeapBlockHeader* list, leVariableHeapBlockHeader* blk)
{
    leVariableHeapBlockHeader* hdr = list;
    leVariableHeapBlockHeader* last = NULL;

    LE_ASSERT(blk != NULL);

    if(list == NULL)
        return NULL;

    while(hdr != NULL)
    {
        if((uint32_t)blk < (uint32_t)hdr)
            return hdr->prev;

        last = hdr;
        hdr = hdr->next;
    }

    return last;
}

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
static void _fillBlock(leVariableHeapBlockHeader* blk, uint8_t val, uint32_t size)
{
    uint8_t* ptr = LE_VARIABLEHEAP_BLOCK_DATA_PTR(blk);

    memset(ptr, val, size);
}

static void _writeFooter(leVariableHeapBlockHeader* blk)
{
    uint8_t* ptr;
    uint32_t pad;

    /* write the tail checksum */
    ptr = LE_VARIABLEHEAP_BLOCK_FOOTER_PTR(blk);

    *((uint32_t*)ptr) = blk->checksum;

    pad = LE_VARIABLEHEAP_BLOCK_PADDING_SIZE(blk);

    /* write any padding bytes */
    ptr = LE_VARIABLEHEAP_BLOCK_PADDING_PTR(blk);

    memset(ptr, MS_PADDING, pad);
}
#endif

static leVariableHeapBlockHeader* _createBlock(void* addr, uint32_t capacity, uint32_t size)
{
    leVariableHeapBlockHeader* blk = (leVariableHeapBlockHeader*)addr;

    /* write the header */
    blk->capacity = capacity;
    blk->size = size;
    blk->next = NULL;
    blk->prev = NULL;
#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    blk->checksum = (uint32_t)blk ^ LE_VARIABLEHEAP_BLOCK_CHECKSUM;
#endif
    blk->free = LE_TRUE;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    /* write the footer */
    _writeFooter(blk);
#endif

    return blk;
}

#if LE_VARIABLEHEAP_BESTFIT == 0
static leVariableHeapBlockHeader* _findFirstFit(leVariableHeap* heap, uint32_t size)
{
    leVariableHeapBlockHeader* blk = heap->freeList;

    LE_ASSERT(blk->free == 1);

    /* search the free list for a fit */
    while(blk != NULL)
    {
        if(blk->capacity >= size)
            break;

        blk = blk->next;

        LE_ASSERT(blk->prev != NULL && blk->prev->next != NULL && blk->free == 1);
    }

    return blk;
}
#else
static leVariableHeapBlockHeader* _findBestFit(leVariableHeap* heap, uint32_t size)
{
    leVariableHeapBlockHeader *blk, *best;

    blk = heap->freeList;
    best = NULL;

    /* search the free list for a best fit */
    while(blk != NULL)
    {
        if(blk->capacity >= size)
        {
            if(best == NULL || blk->capacity < best->capacity)
                best = blk;
        }

        blk = blk->next;
    }

    return best;
}
#endif

static leResult _splitBlock(leVariableHeapBlockHeader* blk, uint32_t capacity, uint32_t size)
{
    uint32_t rem;
    leVariableHeapBlockHeader *newBlock, *prev, *next;

    rem = blk->capacity - capacity;

    if(rem < LE_VARIABLEHEAP_BLOCK_MINIMUM_SIZE)
    {
        // remainder isn't large enough for a new block.  just adjust the logical size
        blk->size = size;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
        _writeFooter(blk);
#endif

        return LE_FAILURE;
    }
    else
    {
        prev = blk->prev;
        next = blk->next;

        _createBlock(blk, capacity, size);

        newBlock = _createBlock((uint8_t*)blk + LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE + capacity,
                                rem - LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE,
                                rem - LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE);

        blk->prev = prev;
        blk->next = newBlock;

        newBlock->prev = blk;
        newBlock->next = next;

        if(prev != NULL)
            prev->next = blk;

        if(next != NULL)
            next->prev = newBlock;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
        _fillBlock(blk, MS_FREE, blk->capacity);
        _fillBlock(newBlock, MS_FREE, newBlock->capacity);
#endif

        return LE_SUCCESS;
    }
}

static leVariableHeapBlockHeader* _combineBlockPrev(leVariableHeap* heap,
                                                    leVariableHeapBlockHeader* blk)
{
    leVariableHeapBlockHeader *prevprev, *prev, *next;

#if LE_VARIABLEHEAP_DEBUG == 0
    (void)heap;
#endif

    if(blk->prev == NULL || _areAdjacent(blk->prev, blk) == LE_FALSE)
        return blk;

    prevprev = blk->prev->prev;
    prev = blk->prev;
    next = blk->next;

    _createBlock((void*)prev,
                 prev->capacity + LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(blk),
                 prev->capacity + LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(blk));

    prev->prev = prevprev;
    prev->next = next;

    if(prev->next != NULL)
        prev->next->prev = prev;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _fillBlock(prev, MS_FREE, prev->capacity);
#endif

#if LE_VARIABLEHEAP_DEBUG >= 1
    heap->freeBlockCount--;
#endif

    return prev;
}

static leVariableHeapBlockHeader* _combineBlockNext(leVariableHeap* heap,
                                                    leVariableHeapBlockHeader* blk)
{
    leVariableHeapBlockHeader *prev, *next, *nextnext;

    if(blk->next == NULL || _areAdjacent(blk, blk->next) == LE_FALSE)
        return blk;

    prev = blk->prev;
    next = blk->next;
    nextnext = blk->next->next;

    _createBlock((void*)blk,
                 blk->capacity + LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(next),
                 blk->capacity + LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(next));

    blk->prev = prev;
    blk->next = nextnext;

    if(blk->next != NULL)
        blk->next->prev = blk;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _fillBlock(blk, MS_FREE, blk->capacity);
#endif

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    heap->freeBlockCount--;
#endif

    return blk;
}

leResult leVariableHeap_Init(leVariableHeap* heap,
                             void* data,
                             uint32_t size)
{
#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    leVariableHeapBlockHeader* hdr;
#endif

    if(size % LE_VARIABLEHEAP_ALIGNMENT != 0)
        return LE_FAILURE;

    heap->size = size;
    heap->data = data;

    heap->freeList = (leVariableHeapBlockHeader*)heap->data;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    hdr = _createBlock(heap->freeList,
                       heap->size - LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE,
                       heap->size - LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE);


    heap->freeBlockCount = 1;

    // fill memory with debug values
    _fillBlock(heap->freeList, MS_FREE, hdr->capacity);
#else
    _createBlock(heap->freeList,
                 heap->size - LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE,
                 heap->size - LE_VARIABLEHEAP_BLOCK_OVERHEAD_SIZE);
#endif

    heap->initialized = LE_TRUE;

    return LE_SUCCESS;
}

void leVariableHeap_Destroy(leVariableHeap* heap)
{
    LE_ASSERT(heap != NULL && heap->initialized == LE_TRUE);

    if(heap->initialized == LE_FALSE)
        return;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    memset(heap->data, 0, heap->size);
#endif

    heap->initialized = LE_FALSE;
}

#if LE_USE_DEBUG_ALLOCATOR == 1
void* leVariableHeap_Alloc(leVariableHeap* heap,
                           uint32_t size,
                           uint32_t lineNum,
                           const char* funcName,
                           const char* fileName)
#else
void* leVariableHeap_Alloc(leVariableHeap* heap,
                           uint32_t size)
#endif
{
    uint32_t capacity = size;

    leVariableHeapBlockHeader *block, *slot;

    LE_ASSERT(heap->initialized);

    // adjust size based on the specified alignment
    if((capacity % LE_VARIABLEHEAP_ALIGNMENT) != 0)
    {
        capacity += LE_VARIABLEHEAP_ALIGNMENT - (capacity % LE_VARIABLEHEAP_ALIGNMENT);
    }

#if LE_VARIABLEHEAP_BESTFIT == 1
    block = _findBestFit(heap, capacity);
#else
    block = _findFirstFit(heap, capacity);
#endif

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
    leVariableHeap_Validate(heap);
#endif

    LE_ASSERT(block != NULL);

    if(block == NULL)
        return NULL;

    if(block->capacity == capacity)
    {
        block->size = size;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
        _writeFooter(block);
#endif
    }
        // break up the block if need be
    else if (block->capacity > capacity)
    {
#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
        if(_splitBlock(block, capacity, size) == LE_SUCCESS)
            heap->freeBlockCount++;
#else
        _splitBlock(block, capacity, size);
#endif
    }

    // adjust free list pointers
    if(block->prev != NULL)
    {
        block->prev->next = block->next;
    }
    else
    {
        heap->freeList = block->next;
    }

    if(block->next != NULL)
    {
        block->next->prev = block->prev;
    }

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    heap->freeBlockCount--;
#endif

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
    leVariableHeap_Validate(heap);
#endif

    block->prev = NULL;
    block->next = NULL;

    // adjust alloc list pointers
    slot = _findSlot(heap->allocList, block);

    if(slot == NULL)
    {
        // block is the new list head
        block->next = heap->allocList;

        if(block->next != NULL)
        {
            block->next->prev = block;
        }

        heap->allocList = block;
    }
    else
    {
        block->prev = slot;
        block->next = slot->next;

        if(slot->next != NULL)
        {
            slot->next->prev = block;
        }

        slot->next = block;
    }

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    heap->allocBlockCount++;

#if LE_USE_ALLOCATION_TRACKING == 1
    block->lineNumber = lineNum;
    block->funcName = funcName;
    block->fileName = fileName;
#endif
#endif

    block->free = LE_FALSE;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _fillBlock(block, MS_PADDING, block->capacity);
    _fillBlock(block, MS_ALLOC, block->size);

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
    leVariableHeap_Validate(heap);
#endif
#endif

    heap->used += LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(block);

    if(heap->used > heap->maxUsage)
    {
        heap->maxUsage = heap->used;
    }

    LE_ASSERT(heap->freeList != NULL);

    return LE_VARIABLEHEAP_BLOCK_DATA_PTR(block);
}

void leVariableHeap_Free(leVariableHeap* heap,
                         void* ptr)
{
    leVariableHeapBlockHeader *blk, *slot;

    LE_ASSERT(heap != NULL &&
              ptr != NULL &&
              leVariableHeap_Contains(heap, ptr) == LE_TRUE);

    blk = (leVariableHeapBlockHeader*)((uint8_t*)ptr - LE_VARIABLEHEAP_BLOCK_HEADER_SIZE);

    LE_ASSERT(heap->used >= LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(blk));

    heap->used -= LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(blk);

    LE_ASSERT(blk->free == LE_FALSE);

    if(blk->free == LE_TRUE)
        return;

    // adjust alloc list pointers
    if(blk->prev == NULL)
        heap->allocList = blk->next;
    else
        blk->prev->next = blk->next;

    if(blk->next != NULL)
        blk->next->prev = blk->prev;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    heap->allocBlockCount--;
#endif

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
    leVariableHeap_Validate(heap);
#endif

    // add to free list
    blk->next = NULL;
    blk->prev = NULL;

    slot = _findSlot(heap->freeList, blk);

    if(slot == NULL)
    {
        blk->next = heap->freeList;
        blk->next->prev = blk;

        heap->freeList = blk;
    }
    else
    {
        blk->next = slot->next;
        blk->prev = slot;

        slot->next = blk;

        if(blk->next != NULL)
            blk->next->prev = blk;
    }

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    heap->freeBlockCount++;
#endif

    blk->free = LE_TRUE;
    blk->size = blk->capacity;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _fillBlock(blk, MS_FREE, blk->capacity);
#endif

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
    leVariableHeap_Validate(heap);
#endif

    blk = _combineBlockPrev(heap, blk);
    blk = _combineBlockNext(heap, blk);

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
    leVariableHeap_Validate(heap);
#endif

    LE_ASSERT(heap->freeList != NULL);
};

size_t leVariableHeap_Used(leVariableHeap* heap)
{
    leVariableHeapBlockHeader* blk = heap->allocList;

    size_t used = 0;

    while(blk != NULL)
    {
        used += LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(blk);

        blk = blk->next;
    }

    return used;
}

leBool leVariableHeap_Contains(leVariableHeap* heap, void* ptr)
{
    void* heapEnd = (uint8_t*)heap->data + heap->size;

    return ptr >= heap->data && ptr < heapEnd;
}

uint32_t leVariableHeap_SizeOf(leVariableHeap* heap, void* ptr)
{
    leVariableHeapBlockHeader* blk = (leVariableHeapBlockHeader*)((uint8_t*)ptr - LE_VARIABLEHEAP_BLOCK_HEADER_SIZE);

    return blk->size;
}

leResult leVariableHeap_Validate(leVariableHeap* heap)
{
#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    //printf("expected allocations: %d, actual %d\n", heap->allocBlockCount, _countList((leVariableHeapBlockHeader*)heap->allocList));
    //printf("expected frees: %d, actual %d\n", heap->freeBlockCount, _countList((leVariableHeapBlockHeader*)heap->freeList));

    LE_ASSERT(_countList((leVariableHeapBlockHeader*)heap->allocList) == heap->allocBlockCount);

    if(_countList((leVariableHeapBlockHeader*)heap->allocList) != heap->allocBlockCount)
        return LE_FAILURE;

    //LE_ASSERT(_countList((leVariableHeapBlockHeader*)heap->freeList) == heap->freeBlockCount);

    //if(_countList((leVariableHeapBlockHeader*)heap->freeList) != heap->freeBlockCount)
    //    return LE_FAILURE;
#endif

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
    if(_validateList((leVariableHeapBlockHeader*)heap->allocList) == LE_FAILURE)
        return LE_FAILURE;

    if(_validateList((leVariableHeapBlockHeader*)heap->freeList) == LE_FAILURE)
        return LE_FAILURE;
#endif

    return LE_SUCCESS;
}

#ifdef _WIN32
void leVariableHeap_Dump(leVariableHeap* heap,
                         leBool dumpRecords)
{
    leVariableHeapBlockHeader* blk;

    printf("Variable Heap Data:\n");
    printf("  Size           %u\n", heap->size);
    printf("  Max Usage:     %u\n", heap->maxUsage);
    printf("  Current Usage: %u\n", heap->used);
    printf("\n\n");

    if(dumpRecords == LE_TRUE)
    {
        printf("  Block allocation information:\n");
        
        blk = heap->data;
        
        while(blk != NULL)
        {
            printf("    Block:\n");
            printf("      Size: %u\n", LE_VARIABLEHEAP_BLOCK_TOTAL_SIZE(blk));

#if LE_USE_ALLOCATION_TRACKING == 1            
            printf("      File:     %s\n", blk->fileName);
            printf("      Line:     %u\n", blk->lineNumber);
            printf("      Function: %s\n", blk->funcName);
#endif
            printf("\n\n");
            
            blk = blk->next;
        }
    }
}
#endif
