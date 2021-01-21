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
/** \file legato_variableheap.h
 * @brief Variable heap functions and definitions.
 *
 * @details .
 */

#ifndef LEGATO_VARIABLEHEAP_H
#define LEGATO_VARIABLEHEAP_H

#include "gfx/legato/common/legato_common.h"

#if LE_MEMORY_MANAGER_ENABLE == 1

#if LE_USE_DEBUG_ALLOCATOR == 1
#define LE_VHEAP_ALLOC(heap, size) leVariableHeap_Alloc(heap, size, __LINE__, __FUNCTION__, __FILE__)
#define LE_VHEAP_REALLOC(heap, ptr, size) leVariableHeap_Realloc(heap, ptr, size, __LINE__, __FUNCTION__, __FILE__)
#else
#define LE_VHEAP_ALLOC(heap, size) leVariableHeap_Alloc(heap, size)
#define LE_VHEAP_REALLOC(heap, ptr, size) leVariableHeap_Realloc(ptr, size)
#endif

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
/**
 * @brief This struct represents a variable heap.
 * @details A variable heap is an allocator is capable of allocating blocks
 * of a variable size.  This heap is flexible in capability but suffers from
 * fragmentation and possibly slower allocation times than a fixed heap.  Good
 * for larger allocation sizes.
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

    uint32_t allocBlockCount;
    uint32_t freeBlockCount;
} leVariableHeap;

// *****************************************************************************
/**
 * @brief Initialize Heap.
 * @details .
 * @code
 * leVariableHeap* heap;
 * void* data;
 * uint32_t size;
 * leResult res = leVariableHeap_Init(heap, data, size);
 * @endcode
 * @param heap the pointer to the heap object
 * @param size the size of the heap in bytes
 * @param data pointer to the data buffer to use for the heap.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leVariableHeap_Init(leVariableHeap* heap,
                             void* data,
                             uint32_t size);

// *****************************************************************************
/**
 * @brief Destory Heap.
 * @details .
 * @code
 * leVariableHeap* heap;
 * leVariableHeap_Destroy(heap);
 * @param heap pointer to the heap object
 * @endcode
 * @return void.
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
/**
 * @brief Allocate a block.
 * @details Allocates a block from a variable heap.
 * @code
 * leVariableHeap* heap;
 * void* block = leVariableHeap_Alloc(heap, size);
 * @param heap pointer to the heap object
 * @param size the size of block to allocate
 * @endcode
 * @return void pointer.
 */
void* leVariableHeap_Alloc(leVariableHeap* heap,
                           uint32_t size);
#endif

#if LE_USE_DEBUG_ALLOCATOR == 1
/**
 * @brief Reallocate a block.
 * @details Reallocates a block from a variable heap.
 * @code
 * leVariableHeap* heap;
 * void* ptr = leVariableHeap_Realloc(heap, ptr, size, lineNum, funcName, fileName);
 * @param heap pointer to the heap object
 * @param ptr the address to reallocate
 * @param size the size of block to allocate
 * @param lineNum the file line number of the caller
 * @param funcName the function name of the caller
 * @param fileName the file name of the caller
 * @endcode
 * @return void pointer.
 */
void* leVariableHeap_Realloc(leVariableHeap* heap,
                             void* ptr,
                             uint32_t size,
                             uint32_t lineNum,
                             const char* funcName,
                             const char* fileName);

#else
/**
 * @brief Reallocate a block.
 * @details Reallocates a block from a variable heap.
 * @code
 * leVariableHeap* heap;
 * void* ptr = leVariableHeap_Realloc(heap, ptr, size);
 * @param heap pointer to the heap object
 * @param ptr the address to reallocate
 * @param size the size of block to allocate
 * @endcode
 * @return void pointer.
 */
void* leVariableHeap_Realloc(leVariableHeap* heap,
                             void* ptr,
                             uint32_t size);
#endif
// *****************************************************************************
/**
 * @brief Free a block.
 * @details Frees a block from a variable heap.
 * @code
 * leVariableHeap* heap;
 * void* ptr;
 * leResult res = leVariableHeap_Free(heap, ptr);
 * @param heap pointer to the heap object
 * @param ptr the pointer to look for
 * @endcode
 * @return void.
 */
void leVariableHeap_Free(leVariableHeap* heap,
                         void* ptr);

// *****************************************************************************
/**
 * @brief Query heap by pointer.
 * @details Queries a heap by pointer. Determines if <span class="param">heap</span> contains
 * <span class="param">ptr</span>.
 * @code
 * leVariableHeap* heap;
 * void* ptr;
 * leResult res = leVariableHeap_Contains(heap, ptr);
 * @param heap pointer to the heap object
 * @param ptr the pointer to look for
 * @endcode
 * @return LE_TRUE if contained, otherwise LE_FALSE.
 */
leBool leVariableHeap_Contains(leVariableHeap* heap,
                               void* ptr);

// *****************************************************************************
/**
 * @brief Validate heap.
 * @details Validates a <span class="param">heap</span>.  Validation involves
 * scanning the heap blocks to look for errors.  The amount of checking is
 * dependent on the heap debug level.  When heap debugging is enabled the heap
 * will use extra memory to store block checksums.  These checksums are verified
 * to look for buffer overruns or other errors.
 * @code
 * leVariableHeap* heap;
 * leResult res = leVariableHeap_Validate(heap);
 * @param heap is the pointer to the heap object
 * @endcode
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leVariableHeap_Validate(leVariableHeap* heap);

// *****************************************************************************
/**
 * @brief Get size of allocated pointer.
 * @details Get size of allocated <span class="param">ptr</span> in
 * <span class="param">heap</span>.
 * @code
 * leVariableHeap* heap;
 * void* ptr;
 * uint32_t size = leVariableHeap_SizeOf(heap, ptr);

 * @endcode
 * @return the size of the pointer, otherwise zero if it wasn't found.
 */
uint32_t leVariableHeap_SizeOf(leVariableHeap* heap,
                               void* ptr);

// *****************************************************************************

/**
 * @brief Dumps the contents of heap
 * @details Dumps the contents of a heap to STDOUT.
 * @code
 * leVariableHeap* heap;
 * leBool dumpRecords;
 * leVariableHeap_Dump(heap, dumpRecords);
 * @param heap pointer to the heap object
 * @param dumpRecords set to true to dump the actual block data arrays
 * @endcode
 * @return void.
 */
void leVariableHeap_Dump(leVariableHeap* heap,
                         leBool dumpRecords);

#endif // LE_MEMORY_MANAGER_ENABLE

#endif /* LEGATO_VARIABLEHEAP_H */

