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

#if LE_MEMORY_MANAGER_ENABLE == 1

#include "gfx/legato/common/legato_error.h"

#include <string.h>
#include <stdio.h>

#define BLOCK_HEADER_SIZE       (sizeof(Block))

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
#define BLOCK_FOOTER_SIZE       (sizeof(uint32_t))
#else
#define BLOCK_FOOTER_SIZE       0
#endif

#define BLOCK_OVERHEAD_SIZE       (BLOCK_HEADER_SIZE + BLOCK_FOOTER_SIZE)
#define BLOCK_DATA_PTR(blk)       ((uint8_t*)((uint32_t)blk + BLOCK_HEADER_SIZE))
#define BLOCK_FOOTER_PTR(blk)     ((uint8_t*)BLOCK_DATA_PTR(blk) + blk->size)

#define BLOCK_MINIMUM_SIZE        (BLOCK_OVERHEAD_SIZE + LE_VARIABLEHEAP_MINALLOCSIZE)

#define BLOCK_CHECKSUM            (-1)
#define BLOCK_NEXT(blk)           (Block*)(((uint32_t)blk + BLOCK_PHYSICAL_SIZE(blk)))
#define BLOCK_NEXT_PTR(blk)       (void*)(((uint32_t)blk + BLOCK_PHYSICAL_SIZE(blk)))

#define BLOCK_PHYSICAL_SIZE(blk)  (blk->size + BLOCK_OVERHEAD_SIZE)
#define BLOCK_LOGICAL_SIZE(blk)   (blk->size)
#define BLOCK_COMBINED_SIZE(l, r) (BLOCK_PHYSICAL_SIZE(l) + BLOCK_PHYSICAL_SIZE(r) - BLOCK_OVERHEAD_SIZE)

#define BLOCK_TO_PHYSICAL(ptr)    (Block*)((uint32_t)ptr - BLOCK_HEADER_SIZE)
#define BLOCK_TO_LOGICAL(blk)     (void*)((uint32_t)blk + BLOCK_HEADER_SIZE)

#define BLOCK_ALIGNEDSIZE(size)   (size + LE_VARIABLEHEAP_ALIGNMENT - (size % LE_VARIABLEHEAP_ALIGNMENT))

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
#define HEAP_VALIDATE(heap) leVariableHeap_Validate(heap);
#else
#define HEAP_VALIDATE(heap)
#endif

/* memory state */
enum
{
    MS_ALLOC   = 0xAC,
    MS_FREE    = 0xFE
};

struct Block;

typedef struct Block
{
    uint32_t size;

    struct Block* prev;
    struct Block* next;

    leBool free;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
#if LE_USE_ALLOCATION_TRACKING == 1
    uint32_t lineNumber;
    const char* funcName;
    const char* fileName;
#endif

    uint32_t checksum;
#endif
} Block;

static void _subtractHeapSize(leVariableHeap* heap, uint32_t size)
{
    heap->used -= size;

    LE_ASSERT(heap->used >= 0);
}

static void _addHeapSize(leVariableHeap* heap, uint32_t size)
{
    heap->used += size;

    if(heap->used > heap->maxUsage)
    {
        heap->maxUsage = heap->used;
    }

    LE_ASSERT(heap->used < heap->size);
}

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
static void _fillBlock(Block* blk, uint8_t val, uint32_t size)
{
    uint8_t* ptr = BLOCK_DATA_PTR(blk);

    memset(ptr, val, size);
}

static void _writeFooter(Block* blk)
{
    uint8_t* ptr;

    /* write the tail checksum */
    ptr = BLOCK_FOOTER_PTR(blk);

    *((uint32_t*)ptr) = blk->checksum;
}
#endif

static void _insertIntoList(Block** list, Block* blk)
{
    Block* listBlock = *list;
    Block* last = NULL;

    LE_ASSERT(blk != NULL);

    if(listBlock == NULL)
    {
        *list = blk;
        blk->next = NULL;
        blk->prev = NULL;

        return;
    }

    if((uint32_t)blk < (uint32_t)listBlock)
    {
        blk->next = listBlock;
        blk->prev = NULL;
        listBlock->prev = blk;

        *list = blk;

        return;
    }

    while(listBlock != NULL)
    {
        if((uint32_t)blk < (uint32_t)listBlock)
            break;

        last = listBlock;
        listBlock = listBlock->next;
    }

    last->next = blk;
    blk->prev = last;

    if(blk->prev != NULL)
    {
        blk->prev->next = blk;
    }

    blk->next = listBlock;

    if(blk->next != NULL)
    {
        blk->next->prev = blk;
    }
}

static void _removeFromList(Block** list, Block* blk)
{
    // adjust list pointers
    if(blk->prev != NULL)
    {
        blk->prev->next = blk->next;
    }
    else
    {
        *list = blk->next;
    }

    if(blk->next != NULL)
    {
        blk->next->prev = blk->prev;
    }

    blk->next = NULL;
    blk->prev = NULL;
}

static leBool _areAdjacent(Block* l, Block* r)
{
    return (uint8_t*)l + BLOCK_PHYSICAL_SIZE(l) == (uint8_t*)r;
}

static Block* _createBlock(void* addr,
                           uint32_t size,
                           leBool isFree)
{
    Block* blk = (Block*)addr;

    memset(blk, 0, sizeof(struct Block));

    /* write the header */
    blk->size = size;
#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    blk->checksum = (uint32_t)blk ^ BLOCK_CHECKSUM;
#endif
    blk->free = isFree;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    /* write the footer */
    _writeFooter(blk);
#endif

    return blk;
}

static Block* _findPreviousBlock(leVariableHeap* heap, void* addr, Block* ignore)
{
    Block* blk = heap->allocList;
    Block* last = 0x0;

    while(blk != NULL)
    {
        if((void*)blk > addr)
            break;

        if(blk > last && blk != ignore)
        {
            last = blk;
        }

        blk = blk->next;
    }

    blk = heap->freeList;

    while(blk != NULL)
    {
        if((void*)blk > addr && blk != ignore)
        {
            if(blk < last)
            {
                last = blk;
            }

            break;
        }

        blk = blk->next;
    }

    return last;
}

static Block* _findNextBlock(leVariableHeap* heap, void* addr/*, Block* ignore*/)
{
    Block* blk = heap->allocList;
    Block* last = 0x0;

    while(blk != NULL)
    {
        if((void*)blk > addr)
        {
            last = blk;

            break;
        }

        blk = blk->next;
    }

    blk = heap->freeList;

    while((void*)blk != NULL)
    {
        if((void*)blk > addr)
        {
            if(last == NULL || blk < last)
            {
                last = blk;
            }

            break;
        }

        blk = blk->next;
    }

    return last;
}

static uint32_t _freeSpaceLeft(leVariableHeap* heap, Block* blk)
{
    Block* leftBlock = _findPreviousBlock(heap, blk, NULL);

    if(leftBlock == NULL || leftBlock->free == LE_FALSE)
        return 0;

    return BLOCK_PHYSICAL_SIZE(leftBlock);
}

static uint32_t _freeSpaceRight(leVariableHeap* heap, Block* blk)
{
    Block* rightBlock = _findNextBlock(heap, blk/*, NULL*/);

    if(rightBlock == NULL || rightBlock->free == LE_FALSE)
        return 0;

    return BLOCK_PHYSICAL_SIZE(rightBlock);
}

/*static leBool _canFit(leVariableHeap* heap, Block* blk, void* addr)
{
    Block* prevBlock = _findPreviousBlock(heap, addr, blk);
    Block* nextBlock = _findNextBlock(heap, addr, blk);
    uint32_t startAddress = 0;
    uint32_t endAddress = heap->size;
    uint32_t availableSize;

    if(prevBlock != NULL)
    {
        startAddress = (uint8_t*)prevBlock + BLOCK_PHYSICAL_SIZE(prevBlock);
    }

    if(nextBlock != NULL)
    {
        endAddress = (uint8_t*)nextBlock;
    }

    availableSize = endAddress - startAddress;

    return BLOCK_PHYSICAL_SIZE(blk) <= availableSize;
}*/

#if 0
static void _logicallyShrinkBlock(Block* blk, uint32_t size)
{
    blk->size = size;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    /* write the footer */
    _writeFooter(blk);
#endif
}
#endif

#if 0
static void _physicallyShrinkBlock(Block* blk, uint32_t size)
{
    uint32_t newCapacity = BLOCK_CAPACITY(size);

    blk->capacity = newCapacity;
    blk->size = size;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    /* write the footer */
    //_writeFooter(blk);
#endif
}
#endif

#if 0
static void _resizeFreeBlock(leVariableHeap* heap, Block* blk, int32_t delta, leBool head)
{
    Block* newBlk;
    uint8_t* ptr;
    uint32_t capacity;

    if(blk->free == LE_FALSE)
        return;

    if(head == LE_TRUE)
    {
        ptr = (uint8_t*)blk - delta;
    }
    else
    {
        ptr = (uint8_t*)blk;
    }

    capacity - blk->capacity - delta;

    _removeFromList(&heap->freeList, blk);

    newBlk = _createBlock(ptr, capacity, capacity, LE_TRUE);

    _insertIntoList(&heap->freeList, newBlk);
}

static void _resizeAllocBlock(leVariableHeap* heap, Block* blk, uint32_t size)
{
    Block* newBlk;
    uint8_t* ptr;
    uint32_t capacity;

    if(blk->free == LE_TRUE)
        return;

    capacity = BLOCK_CAPACITY(size);

    capacity - blk->capacity - delta;

    _removeFromList(&heap->freeList, blk);

    newBlk = _createBlock(ptr, capacity, capacity, LE_TRUE);

    _insertIntoList(&heap->freeList, newBlk);
}
#endif

static void _resizeBlock(Block* blk, uint32_t size)
{
    /* write the header */
    blk->size = size;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    /* write the footer */
    _writeFooter(blk);
#endif
}

#if 0
static leResult _growAllocBlock(leVariableHeap* heap, Block* blk, uint32_t size)
{
    uint32_t newCapacity = BLOCK_CAPACITY(size);
    Block* next = _findNextBlock(heap, (void*)blk/*, blk*/);
    int32_t delta;

    if(next != NULL && next->free == LE_FALSE)
        return LE_FAILURE;

    if(_canFit(heap, blk, size) == LE_FALSE)
        return LE_FAILURE;

    delta = newCapacity - blk->capacity;

    _resizeFreeBlock(heap, next, delta, LE_TRUE);

    _resizeBlock(blk, size);

    return LE_SUCCESS;
}
#endif

static leBool _canSplit(Block* blk, uint32_t size)
{
    uint32_t leftBlockSize = BLOCK_OVERHEAD_SIZE + size;
    uint32_t rightBlockSize = BLOCK_OVERHEAD_SIZE + blk->size - size;

    if(leftBlockSize + rightBlockSize < BLOCK_PHYSICAL_SIZE(blk) ||
       leftBlockSize < BLOCK_MINIMUM_SIZE ||
       rightBlockSize < BLOCK_MINIMUM_SIZE)
        return LE_FALSE;

    return LE_TRUE;
}

static void _splitFreeBlock(leVariableHeap* heap, Block* blk, uint32_t size)
{
    uint32_t rem;
    Block *newBlock;

    rem = blk->size - size;

    if(rem == 0)
        return;

    HEAP_VALIDATE(heap);

    _resizeBlock(blk, size);

    newBlock = _createBlock(BLOCK_NEXT_PTR(blk), rem - BLOCK_OVERHEAD_SIZE, LE_TRUE);

    _insertIntoList((Block**)&heap->freeList, newBlock);

    HEAP_VALIDATE(heap);
}

static leResult _validatePointer(Block* blk,
                                 Block* list)
{
    if(blk == NULL)
        return LE_FAILURE;

    while(list != NULL)
    {
        if(blk == list)
            return LE_SUCCESS;

        list = list->next;
    }

    return LE_FAILURE;
}

static Block* _combineFreeBlockPrev(leVariableHeap* heap,
                                    Block* blk)
{
    Block* prev;
    uint32_t size;

    if(blk->prev == NULL || _areAdjacent(blk->prev, blk) == LE_FALSE)
        return blk;

    HEAP_VALIDATE(heap);

    prev = blk->prev;

    size = BLOCK_COMBINED_SIZE(prev, blk);

    _removeFromList((Block**)&heap->freeList, prev);
    _removeFromList((Block**)&heap->freeList, blk);

    _createBlock((void*)prev, size, LE_TRUE);

    _insertIntoList((Block**)&heap->freeList, prev);

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _fillBlock(prev, MS_FREE, prev->size);
#endif

    HEAP_VALIDATE(heap);

    return prev;
}

static Block* _combineFreeBlockNext(leVariableHeap* heap,
                                    Block* blk)
{
    Block* next;
    uint32_t size;

    if(blk->next == NULL || _areAdjacent(blk, blk->next) == LE_FALSE)
        return blk;

    HEAP_VALIDATE(heap);

    next = blk->next;

    size = BLOCK_COMBINED_SIZE(blk, next);

    _removeFromList((Block**)&heap->freeList, blk);
    _removeFromList((Block**)&heap->freeList, next);

    _createBlock((void*)blk, size, LE_TRUE);

    _insertIntoList((Block**)&heap->freeList, blk);

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _fillBlock(blk, MS_FREE, blk->size);
#endif

    HEAP_VALIDATE(heap);

    return blk;
}

static leResult _shrinkAllocBlock(leVariableHeap* heap, Block* blk, uint32_t size)
{
    uint32_t rem;

    if(_canSplit(blk, size) == LE_FALSE)
        return LE_FAILURE;

    HEAP_VALIDATE(heap);

    _subtractHeapSize(heap, BLOCK_PHYSICAL_SIZE(blk));

    rem = BLOCK_PHYSICAL_SIZE(blk);

    _resizeBlock(blk, size);

    rem -= BLOCK_PHYSICAL_SIZE(blk);

    _createBlock(BLOCK_NEXT_PTR(blk), rem, LE_TRUE);

    _addHeapSize(heap, BLOCK_PHYSICAL_SIZE(blk));

    _combineFreeBlockNext(heap, blk);

    HEAP_VALIDATE(heap);

    return LE_SUCCESS;
}

static leResult _growAllocBlock(leVariableHeap* heap, Block* blk, uint32_t size)
{
    int32_t needed, current, remaining;
    int32_t freeRight;

    Block* next;

    HEAP_VALIDATE(heap);

    freeRight = _freeSpaceRight(heap, blk);

    if(freeRight + BLOCK_PHYSICAL_SIZE(blk) - (BLOCK_OVERHEAD_SIZE * 2) < size)
        return LE_FAILURE;

    current = BLOCK_PHYSICAL_SIZE(blk);
    needed = (BLOCK_OVERHEAD_SIZE + size);
    remaining = needed - current;

    // this should never be null
    next = _findNextBlock(heap, blk/*, blk*/);

    // can't shrink the right block
    if(BLOCK_PHYSICAL_SIZE(next) - remaining < BLOCK_MINIMUM_SIZE)
        return LE_FAILURE;

    // remove free block
    _removeFromList((Block**)&heap->freeList, next);

    next = _createBlock((uint8_t*)next + remaining, next->size - remaining, LE_TRUE);

    _insertIntoList((Block**)&heap->freeList, next);

    _subtractHeapSize(heap, BLOCK_PHYSICAL_SIZE(blk));

    blk->size = size;

    _addHeapSize(heap, BLOCK_PHYSICAL_SIZE(blk));

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _writeFooter(blk);
#endif

    HEAP_VALIDATE(heap);

    return LE_SUCCESS;
}

static Block* _swapAllocBlockLeft(leVariableHeap* heap,
                                  Block* allocBlock,
                                  Block* freeBlock)
{
    Block tmp = *freeBlock;

    HEAP_VALIDATE(heap);

    _removeFromList((Block**)&heap->freeList, freeBlock);
    _removeFromList((Block**)&heap->allocList, allocBlock);

    *freeBlock = *allocBlock;

    memcpy(BLOCK_DATA_PTR(freeBlock), BLOCK_DATA_PTR(allocBlock), allocBlock->size);

    *allocBlock = tmp;

    allocBlock->free = LE_TRUE;

    _insertIntoList((Block**)&heap->freeList, allocBlock);
    _insertIntoList((Block**)&heap->allocList, freeBlock);

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _fillBlock(allocBlock, MS_FREE, allocBlock->size);
#endif

    _combineFreeBlockNext(heap, allocBlock);

    HEAP_VALIDATE(heap);

    return freeBlock;
}

static leResult _shiftLeftAndGrow(leVariableHeap* heap, Block** blk, uint32_t size)
{
    int32_t freeLeft, freeRight;
    Block* currentBlock = *blk;

    Block *prev, *next;

    HEAP_VALIDATE(heap);

    freeLeft = _freeSpaceLeft(heap, currentBlock);
    freeRight = _freeSpaceRight(heap, currentBlock);

    if(freeLeft == 0 || freeRight == 0)
        return LE_FAILURE;

    if(freeLeft + freeRight + BLOCK_PHYSICAL_SIZE(currentBlock) - (BLOCK_OVERHEAD_SIZE * 2) < size)
        return LE_FAILURE;

    prev = _findPreviousBlock(heap, currentBlock, currentBlock);
    next = _findNextBlock(heap, currentBlock/*, currentBlock*/);

    if(prev == NULL || next == NULL)
        return LE_FAILURE;

    currentBlock = _swapAllocBlockLeft(heap, currentBlock, prev);

    HEAP_VALIDATE(heap);

    return _growAllocBlock(heap, currentBlock, size);
}

#if LE_USE_ALLOCATION_TRACKING == 1
static leResult _allocateBlock(leVariableHeap* heap,
                               Block* blk,
                               uint32_t line,
                               const char* func,
                               const char* file)
#else
static leResult _allocateBlock(leVariableHeap* heap,
                               Block* blk)
#endif
{
    if(blk->free == LE_FALSE)
        return LE_FAILURE;

    // adjust free list pointers
    _removeFromList((Block**)&heap->freeList, blk);

    // adjust alloc list pointers
    _insertIntoList((Block**)&heap->allocList, blk);

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
#if LE_USE_ALLOCATION_TRACKING == 1
    blk->lineNumber = line;
    blk->funcName = func;
    blk->fileName = file;
#endif
#endif

    blk->free = LE_FALSE;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _fillBlock(blk, MS_ALLOC, blk->size);

    HEAP_VALIDATE(heap);
#endif

    _addHeapSize(heap, BLOCK_PHYSICAL_SIZE(blk));

    return LE_SUCCESS;
}

static leResult _freeBlock(leVariableHeap* heap,
                           Block* blk)
{
    _subtractHeapSize(heap, BLOCK_PHYSICAL_SIZE(blk));

    HEAP_VALIDATE(heap);

    // adjust alloc list pointers
    _removeFromList((Block**)&heap->allocList, blk);

    // add to free list
    _insertIntoList((Block**)&heap->freeList, blk);

    HEAP_VALIDATE(heap);

    blk->free = LE_TRUE;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    _fillBlock(blk, MS_FREE, blk->size);
#endif

    HEAP_VALIDATE(heap);

    blk = _combineFreeBlockPrev(heap, blk);
    _combineFreeBlockNext(heap, blk);

    HEAP_VALIDATE(heap);

    LE_ASSERT(heap->freeList != NULL);

    return LE_SUCCESS;
}


/*static Block* _findFreePrev(Block* blk, Block* freeList)
{
    while(freeList != NULL)
    {
        if(_areAdjacent(freeList, blk) == LE_TRUE)
            return freeList;

        freeList = freeList->next;
    }

    return NULL;
}*/

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
static leResult _validateBlock(Block* blk)
{
    uint8_t* ptr;
    uint32_t i;

    LE_ASSERT((uint32_t)blk != 0xFEFEFEFE);
    LE_ASSERT((uint32_t)blk->prev != 0xFEFEFEFE);
    LE_ASSERT((uint32_t)blk->next != 0xFEFEFEFE);

    if(blk->prev != NULL)
    {
        LE_ASSERT(blk->prev->next == blk);
    }

    if(blk->next != NULL)
    {
        LE_ASSERT(blk->next->prev == blk);
    }

    // validate header checksum

    LE_ASSERT(blk->checksum == ((uint32_t)blk ^ BLOCK_CHECKSUM));

    if(blk->checksum != ((uint32_t)blk ^ BLOCK_CHECKSUM))
        return LE_FAILURE;

    // validate footer checksum
    ptr = BLOCK_FOOTER_PTR(blk);

    LE_ASSERT(*(uint32_t*)ptr == blk->checksum);

    if(*(uint32_t*)ptr != blk->checksum)
        return LE_FAILURE;

    // validate any padding values
    /*if(blk->free == LE_FALSE && blk->size < blk->capacity)
    {
        ptr -= BLOCK_PADDING_SIZE(blk);

        for(i = 0; i < BLOCK_PADDING_SIZE(blk); i++)
        {
            LE_ASSERT(ptr[i] == MS_PADDING);

            if(ptr[i] != MS_PADDING)
                return LE_FAILURE;
        }
    }*/

    // if free, test fill values
    if(blk->free == LE_TRUE)
    {
        ptr = BLOCK_DATA_PTR(blk);

        for(i = 0; i < blk->size; i++)
        {
            //LE_ASSERT(ptr[i] == MS_FREE);

            if(ptr[i] != MS_FREE)
                return LE_FAILURE;
        }
    }

    return LE_SUCCESS;
}

static leResult _validateList(Block* blk)
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

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
static leResult _validateSpace(leVariableHeap* heap)
{
    Block* blk = NULL;
    uint32_t size = 0;

    if(heap->freeList == heap->data)
    {
        blk = heap->freeList;
    }
    else if(heap->allocList == heap->data)
    {
        blk = heap->allocList;
    }

    LE_ASSERT(blk != NULL);

    if(blk == NULL)
        return LE_FAILURE;

    while((uint32_t)blk < (uint32_t)heap->data + heap->size)
    {
        if(_validatePointer(blk, heap->freeList) == LE_FAILURE &&
           _validatePointer(blk, heap->allocList) == LE_FAILURE)
        {
            LE_ASSERT(1 == 0);

            return LE_FAILURE;
        }

        size += BLOCK_PHYSICAL_SIZE(blk);

        blk = BLOCK_NEXT(blk);
    }

    if((uint32_t)blk != ((uint32_t)heap->data) + heap->size)
    {
        LE_ASSERT(1 == 0);

        return LE_FAILURE;
    }

    if(size != heap->size)
    {
        LE_ASSERT(1 == 0);

        return LE_FAILURE;
    }

    return LE_SUCCESS;
}
#endif
#endif

/*#if LE_USE_DEBUG_ALLOCATOR == 1
#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
static uint32_t _countList(Block* blk)
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
#endif*/



#if LE_VARIABLEHEAP_BESTFIT == 0
static Block* _findFirstFit(leVariableHeap* heap, uint32_t size)
{
    Block* blk = heap->freeList;

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
static Block* _findBestFit(leVariableHeap* heap, uint32_t size)
{
    Block *blk, *best;

    blk = heap->freeList;
    best = NULL;

    /* search the free list for a best fit */
    while(blk != NULL)
    {
        if(blk->size >= size)
        {
            if(best == NULL || blk->size < best->size)
                best = blk;
        }

        blk = blk->next;
    }

    return best;
}
#endif

leResult leVariableHeap_Init(leVariableHeap* heap,
                             void* data,
                             uint32_t size)
{
    if(size % LE_VARIABLEHEAP_ALIGNMENT != 0)
        return LE_FAILURE;

    heap->size = size;
    heap->data = data;

    heap->freeList = (Block*)heap->data;

    _createBlock(heap->freeList,
                 heap->size - BLOCK_OVERHEAD_SIZE,
                 LE_TRUE);

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    // fill memory with debug values
    _fillBlock(heap->freeList, MS_FREE, ((Block*)heap->freeList)->size);
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
    Block *block;

    LE_ASSERT(heap->initialized);

    // adjust size based on the specified alignment
    size = BLOCK_ALIGNEDSIZE(size);

#if LE_VARIABLEHEAP_BESTFIT == 1
    block = _findBestFit(heap, size);
#else
    block = _findFirstFit(heap, size);
#endif

    HEAP_VALIDATE(heap);

    // out of memory?
    LE_ASSERT(block != NULL);

    if(block == NULL)
        return NULL;

    if(size < LE_VARIABLEHEAP_MINALLOCSIZE)
    {
        size = LE_VARIABLEHEAP_MINALLOCSIZE;
    }

    // split the block if necessary
    if(_canSplit(block, size) == LE_TRUE)
    {
        _splitFreeBlock(heap, block, size);
    }

#if LE_USE_ALLOCATION_TRACKING == 1
    _allocateBlock(heap, block, lineNum, funcName, fileName);
#else
    _allocateBlock(heap, block);
#endif

    HEAP_VALIDATE(heap);

    //printf("alloc %p(%p)\n", (void*)block, BLOCK_TO_LOGICAL(block));

    return BLOCK_TO_LOGICAL(block);
}


#if LE_USE_DEBUG_ALLOCATOR == 1
void* leVariableHeap_Realloc(leVariableHeap* heap,
                             void* ptr,
                             uint32_t size,
                             uint32_t lineNum,
                             const char* funcName,
                             const char* fileName)
#else
void* leVariableHeap_Realloc(leVariableHeap* heap,
                             void* ptr,
                             uint32_t size)
#endif
{
    Block *blk;
    void *newPtr;

    LE_ASSERT(heap != NULL &&
              ptr != NULL &&
              leVariableHeap_Contains(heap, ptr) == LE_TRUE);

    if(size == 0)
    {
        leVariableHeap_Free(heap, ptr);

        return NULL;
    }

    blk = BLOCK_TO_PHYSICAL(ptr);

    HEAP_VALIDATE(heap);

    if(_validatePointer(blk, heap->allocList) == LE_FAILURE)
        return LE_VHEAP_ALLOC(heap, size);

    if(blk->size == size)
        return ptr;

    // adjust size based on the specified alignment
    size = BLOCK_ALIGNEDSIZE(size);

    if(size < LE_VARIABLEHEAP_MINALLOCSIZE)
    {
        size = LE_VARIABLEHEAP_MINALLOCSIZE;
    }

    // can shrink in place
    if(size < blk->size)
    {
        HEAP_VALIDATE(heap);

        //heap->used = heap->used - blk->capacity + newCapacity;

        if(_shrinkAllocBlock(heap, blk, size) == LE_FAILURE)
        {
            //printf("failed to shrink, return %p(%p)\n", (void*)blk, BLOCK_TO_LOGICAL(blk));
        }
        else
        {
            //printf("shrunk, return %p(%p)\n", (void*)blk, BLOCK_TO_LOGICAL(blk));
        }

        HEAP_VALIDATE(heap);

        return BLOCK_TO_LOGICAL(blk);
    }

    if(_growAllocBlock(heap, blk, size) == LE_SUCCESS)
    {
        //printf("able to grow in place, return %p(%p)\n", (void*)blk, BLOCK_TO_LOGICAL(blk));

        return BLOCK_TO_LOGICAL(blk);
    }

    if(_shiftLeftAndGrow(heap, &blk, size) == LE_SUCCESS)
    {
        //printf("able to shift left and grow, return %p(%p)\n", BLOCK_TO_PHYSICAL(blk), (void*)blk);

        return BLOCK_TO_LOGICAL(blk);
    }

    newPtr = LE_VHEAP_ALLOC(heap, size);

    LE_ASSERT(newPtr != NULL);

    if(newPtr != NULL)
    {
        memcpy(newPtr, ptr, blk->size);

        leVariableHeap_Free(heap, ptr);
    }

    HEAP_VALIDATE(heap);

    //printf("full realloc return %p(%p)\n", BLOCK_TO_PHYSICAL(newPtr), (void*)newPtr);

    return newPtr;
}

void leVariableHeap_Free(leVariableHeap* heap,
                         void* ptr)
{
    Block *blk;

    LE_ASSERT(heap != NULL &&
              ptr != NULL &&
              leVariableHeap_Contains(heap, ptr) == LE_TRUE);

    blk = BLOCK_TO_PHYSICAL(ptr);

    LE_ASSERT(_validatePointer(blk, heap->allocList) == LE_SUCCESS);

    LE_ASSERT(heap->used >= BLOCK_PHYSICAL_SIZE(blk));

    //_subtractHeapSize(heap, BLOCK_PHYSICAL_SIZE(blk));

    LE_ASSERT(blk->free == LE_FALSE);

    if(blk->free == LE_TRUE)
        return;

    _freeBlock(heap, blk);

    //printf("free %p(%p)\n", BLOCK_TO_PHYSICAL(ptr), ptr);
};

size_t leVariableHeap_Used(leVariableHeap* heap)
{
    Block* blk = heap->allocList;

    size_t used = 0;

    while(blk != NULL)
    {
        used += BLOCK_PHYSICAL_SIZE(blk);

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
    (void)heap; // unused

    Block* blk = (Block*)((uint8_t*)ptr - BLOCK_HEADER_SIZE);

    return blk->size;
}

leResult leVariableHeap_Validate(leVariableHeap* heap)
{
#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    //printf("expected allocations: %d, actual %d\n", heap->allocBlockCount, _countList((Block*)heap->allocList));
    //printf("expected frees: %d, actual %d\n", heap->freeBlockCount, _countList((Block*)heap->freeList));

    /*LE_ASSERT(_countList((Block*)heap->allocList) == heap->allocBlockCount);

    if(_countList((Block*)heap->allocList) != heap->allocBlockCount)
        return LE_FAILURE;*/

    //LE_ASSERT(_countList((Block*)heap->freeList) == heap->freeBlockCount);

    //if(_countList((Block*)heap->freeList) != heap->freeBlockCount)
    //    return LE_FAILURE;
#else
    (void)heap; // unused
#endif

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    if(_validateList((Block*)heap->allocList) == LE_FAILURE)
        return LE_FAILURE;

    if(_validateList((Block*)heap->freeList) == LE_FAILURE)
        return LE_FAILURE;
#endif

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 2
    if(_validateSpace(heap) == LE_FAILURE)
        return LE_FAILURE;
#endif

    return LE_SUCCESS;
}

void leVariableHeap_Dump(leVariableHeap* heap,
                         leBool dumpRecords)
{
    Block* blk;

    printf("Variable Heap Data:\n");
	printf("  Size           %u\n", (unsigned int)heap->size);
	printf("  Max Usage:     %u\n", (unsigned int)heap->maxUsage);
	printf("  Current Usage: %u\n", (unsigned int)heap->used);
	printf("  Available:     %u\n", (unsigned int)(heap->size - heap->used));
    printf("\n");

    if(dumpRecords == LE_TRUE)
    {
        printf("  Map:\n");

        blk = heap->data;

        while((uint32_t)blk < (uint32_t)heap->data + heap->size)
        {
            if(blk->free == LE_TRUE)
            {
                printf("    Block (free): %p, %u\n", blk, (unsigned int)BLOCK_PHYSICAL_SIZE(blk));
            }
            else
            {
                printf("    Block (used): %p, %u\n", blk, (unsigned int)BLOCK_PHYSICAL_SIZE(blk));
            }

            //printf("      Address: %p\n", blk);
            //printf("      Size: %u\n", BLOCK_PHYSICAL_SIZE(blk));

#if LE_USE_ALLOCATION_TRACKING == 1
            //printf("      File:     %s\n", blk->fileName);
            //printf("      Line:     %u\n", blk->lineNumber);
            //printf("      Function: %s\n", blk->funcName);
#endif
           // printf("\n\n");

            //blk = blk->next;

            blk = BLOCK_NEXT(blk);
        }

#if 0
        printf("  Free Blocks:\n");

        blk = heap->freeList;

        while(blk != NULL)
        {
			printf("    Block: %p, %u\n", blk, (unsigned int)BLOCK_PHYSICAL_SIZE(blk));
            //printf("      Address: %p\n", blk);
            //printf("      Size: %u\n", BLOCK_PHYSICAL_SIZE(blk));

            //printf("\n\n");

            blk = blk->next;
        }

        printf("\n");
#endif
    }
}

#endif // LE_MEMORY_MANAGER_ENABLE