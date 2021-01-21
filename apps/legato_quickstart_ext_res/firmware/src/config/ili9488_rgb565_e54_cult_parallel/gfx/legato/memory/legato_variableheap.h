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

#ifndef LEGATO_VARIABLEHEAP_H
#define LEGATO_VARIABLEHEAP_H

#include "gfx/legato/common/legato_common.h"

// *****************************************************************************
/* Structure:
    struct leVariableHeap

  Summary:
    Defines a variable heap object.  A variable heap is an allocator is
    capable of allocating blocks of a variable size.  This heap is flexible
    in capability but suffers from fragmentation and possibly slower allocation
    times than a fixed heap.  Good for larger allocation sizes.

    leBool initialized - indicates whether the heap has been properly initialized
    size_t size - the overall size of the heap
    size_t used - the amount of space the heap is currently using
    size_t maxUsage - the highest amount of usage the heap has seen
    void* data - pointer to the heap data
    void* allocList - a linked list of allocation data blocks
    void* freeList - a linked list of free data blocks
    uint32_t allocBlockCount - the number of blocks currently allocated
    uint32_t freeBlockCount - the number of blocks in the free list
*/
typedef struct leVariableHeap
{
    leBool initialized;
    size_t size;
    size_t used;
    size_t maxUsage;

    void* data;

    void* allocList;
    void* freeList;

#if LE_VARIABLEHEAP_DEBUGLEVEL >= 1
    uint32_t allocBlockCount;
    uint32_t freeBlockCount;
#endif
} leVariableHeap;

// *****************************************************************************
/* Function:
    leResult leVariableHeap_Init(leVariableHeap* heap,
                                 void* data,
                                 uint32_t size)

  Summary:
    Initializes a variable heap object struct.

  Description:
    Initializes a variable heap object struct.

  Parameters:
    leVariableHeap* heap - pointer to the heap object
    uint32_t size - the size of the heap in bytes
    uint8_t* data - pointer to the data buffer to use for the heap

  Returns:
    leResult
*/
leResult leVariableHeap_Init(leVariableHeap* heap,
                             void* data,
                             uint32_t size);

// *****************************************************************************
/* Function:
    void leVariableHeap_Destroy(leVariableHeap* heap)

  Summary:
    Destroys a variable heap object.

  Description:
    Destroys a variable heap object.  All pointers that are allocated from this
    heap should be considered invalid.

  Parameters:
    leVariableHeap* heap - pointer to the heap object

  Returns:
    leResult
*/
void leVariableHeap_Destroy(leVariableHeap* heap);

#if LE_USE_DEBUG_ALLOCATOR == 1
// *****************************************************************************
/* Function:
    void* leVariableHeap_Alloc(leVariableHeap* heap,
                               uint32_t size,
                               uint32_t lineNum,
                               const char* funcName,
                               const char* fileName)

  Summary:
    Allocates a block from a variable heap.

  Description:
    Allocates a block from a variable heap.  This is the debug version of this
    function and requires additional parameters.

    Use the LE_MALLOC macro to automatically differentiate between
    this function and its non-debug counterpart.

  Parameters:
    leVariableHeap* heap - pointer to the heap object
    uint32_t size - the size
    uint32_t lineNum - the file line number where this allocation occured
    const char* funcName - the function name where this allocation occured
    const char* fileName - the file name where this allocation occured

  Returns:
    leResult
*/
void* leVariableHeap_Alloc(leVariableHeap* heap,
                           uint32_t size,
                           uint32_t lineNum,
                           const char* funcName,
                           const char* fileName);
#else
// *****************************************************************************
/* Function:
    void* leVariableHeap_Alloc(leVariableHeap* heap,
                               uint32_t size)

  Summary:
    Allocates a block from a variable heap.

  Description:
    Allocates a block from a variable heap.

    Use the LE_MALLOC macro to automatically differentiate between
    this function and its debug counterpart.

  Parameters:
    leVariableHeap* heap - pointer to the heap object
    uint32_t size - the size of block to allocate

  Returns:
    leResult
*/
void* leVariableHeap_Alloc(leVariableHeap* heap,
                           uint32_t size);
#endif

// *****************************************************************************
/* Function:
    void leVariableHeap_Free(leVariableHeap* heap,
                             void* ptr)

  Summary:
    Frees a block from a variable heap.

  Description:
    Frees a block from a variable heap.

  Parameters:
    leVariableHeap* heap - pointer to the heap object
    void* ptr - the pointer to free

  Returns:
    leResult
*/
void leVariableHeap_Free(leVariableHeap* heap,
                         void* ptr);

// *****************************************************************************
/* Function:
    leBool leVariableHeap_Contains(leVariableHeap* heap,
                                   void* ptr)

  Summary:
    Queries a heap to see if it contains a block pointer.

  Description:
    Queries a heap to see if it contains a block pointer.

  Parameters:
    leFixedHeap* heap - pointer to the heap object
    void* ptr - the pointer to look for

  Returns:
    leResult
*/
leBool leVariableHeap_Contains(leVariableHeap* heap,
                               void* ptr);

// *****************************************************************************
/* Function:
    leResult leVariableHeap_Validate(leFixedHeap* heap)

  Summary:
    Validates a heap.

  Description:
    Validates a heap.  Validation involves scanning the heap blocks to look
    for errors.  The amount of checking is dependent on the heap debug level.
    When heap debugging is enabled the heap will use extra memory to
    store block checksums.  These checksums are verified to look for buffer
    overruns or other errors.

  Parameters:
    leVariableHeap* heap - pointer to the heap object

  Returns:
    leResult
*/
leResult leVariableHeap_Validate(leVariableHeap* heap);

// *****************************************************************************
/* Function:
    uint32_t leVariableHeap_SizeOf(leVariableHeap* heap,
                                   void* ptr)

  Summary:
    Queries a variable heap to determine the size of a given allocation
    pointer.

  Description:
    Queries a variable heap to determine the size of a given allocation
    pointer.

  Parameters:
    leVariableHeap* heap - pointer to the heap object
    void* ptr - the pointer to look for

  Returns:
    uint32_t - the size of the pointer or zero if it wasn't found
*/
uint32_t leVariableHeap_SizeOf(leVariableHeap* heap,
                               void* ptr);

#ifdef _WIN32
// *****************************************************************************
/* Function:
    void leVariableHeap_Dump(leVariableHeap* heap,
                             leBool dumpRecords)

  Summary:
    Dumps the contents of a heap to STDOUT.

  Description:
    Dumps the contents of a heap to STDOUT.

  Parameters:
    leVariableHeap* heap - pointer to the heap object
    leBool dumpRecords - set to true to dump the actual block data arrays

  Returns:
    void
*/
void leVariableHeap_Dump(leVariableHeap* heap,
                         leBool dumpRecords);
#endif

#endif /* LEGATO_VARIABLEHEAP_H */
