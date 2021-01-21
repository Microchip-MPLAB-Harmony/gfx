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

#ifndef LEGATO_FIXEDHEAP_H
#define LEGATO_FIXEDHEAP_H

#include "gfx/legato/common/legato_common.h"

#if LE_MEMORY_MANAGER_ENABLE == 1

#if LE_FIXEDHEAP_ENABLE == 1

#if LE_USE_DEBUG_ALLOCATOR == 1
#define LE_FHEAP_ALLOC(heap) leFixedHeap_Alloc(heap, __LINE__, __FUNCTION__, __FILE__)
#define LE_FHEAP_REALLOC(heap, ptr) leFixedHeap_Realloc(heap, ptr, __LINE__, __FUNCTION__, __FILE__)
#else
#define LE_FHEAP_ALLOC(heap) leFixedHeap_Alloc(heap)
#define LE_FHEAP_REALLOC(heap, ptr) leFixedHeap_Realloc(ptr)
#endif

#if LE_FIXEDHEAP_DEBUG == 1
#define LE_FIXEDHEAP_HEADER_SIZE   sizeof(leFixedHeapDebugHeader)
#define LE_FIXEDHEAP_FOOTER_SIZE   4
#define LE_FIXEDHEAP_CHECKSUM      (-1)

#define LE_FIXEDHEAP_BLOCK_FOOTER_PTR(size, blk)   ((uint8_t*)blk + size - LE_FIXEDHEAP_FOOTER_SIZE)

#else
#define LE_FIXEDHEAP_HEADER_SIZE        0
#define LE_FIXEDHEAP_FOOTER_SIZE        0
#endif

#define LE_FIXEDHEAP_BLOCK_SIZE(size) (LE_FIXEDHEAP_HEADER_SIZE + size + LE_FIXEDHEAP_FOOTER_SIZE)

#if LE_FIXEDHEAP_DEBUG == 1
// *****************************************************************************
/* Structure:
    struct leFixedHeapDebugHeader

  Summary:
    Defines a fixed heap block debug header

    uint32_t lineNumber - the line number of the allocation
    const char* funcName - the function name of the allocation
    const char* fileName - the file name of the allocation
    leBool free - indicates if the block is free
    uint32_t checksum - the block checksum
*/
/**
 * @brief This struct represents a fixed heap debug header
 * @details Used to debug fixed heap memory allocation
 */

typedef struct leFixedHeapDebugHeader
{
#if LE_USE_ALLOCATION_TRACKING == 1
    uint32_t lineNumber;    /**< allocation line number. */
    const char* funcName;   /**< allocation function. */
    const char* fileName;   /**< allocation filename. */
#endif

    leBool   free;      /**< indicates if the block is free. */
    uint32_t checksum;  /**< the block checksum. */
} leFixedHeapDebugHeader;
#endif

// *****************************************************************************
/* Structure:
    struct leFixedHeapBlock

  Summary:
    Defines a fixed heap block

    leFixedHeapDebugHeader debug - the block debug information
    void* data - pointer to the block data, also acts as a linked list
                 pointer to free blocks
*/
/**
 * @brief This struct represents a fixed heap block
 * @details Defines a fixed heap block.
 */
typedef struct leFixedHeapBlock
{
#if LE_FIXEDHEAP_DEBUG == 1
    leFixedHeapDebugHeader debug;
#endif
    void* data;
} leFixedHeapBlock;

// *****************************************************************************
/* Structure:
    struct leFixedHeap

  Summary:
    Defines a fixed heap object

    leBool initialized - indicates whether the heap has been properly initialized
    uint32_t numElements - the number of currently allocated blocks
    uint32_t logicalBlockSize - the logical block size
    uint32_t physicalBlockSize - the physical size of the blocks
    uint32_t capacity - the total number of blocks in the heap
    uint32_t maxUsage - usage watermark value
    void* data - pointer to the actual heap data buffer
    void* freeList - pointer to the next free block
*/
/**
 * @brief This struct represents a fixed heap.
 * @details Defines a fixed heap object
 */
typedef struct leFixedHeap
{
    leBool initialized;
    uint32_t numElements;
    uint32_t logicalBlockSize;
    uint32_t physicalBlockSize;
    uint32_t capacity;
    uint32_t maxUsage;
    void* data;
    void* freeList;
} leFixedHeap;

// *****************************************************************************
/* Function:
    leResult leFixedHeap_Init(leFixedHeap* heap,
                              uint32_t size,
                              uint32_t count,
                              uint8_t* data)

  Summary:
    Initializes a fixed heap object struct.

  Description:
    A fixed heap is a memory storage manager that relies on fixed blocks of
    data to fulfull allocation requests.  Fixed blocks do not suffer from
    memory fragmentation and fulfill allocation requests faster than a
    variable heap but often use memory less efficiently.

  Parameters:
    leFixedHeap* heap - pointer to the heap object
    uint32_t size - the size a heap block in bytes
    uint32_t count - the number of blocks in the heap
    uint8_t* data - pointer to the data buffer to use for the heap

  Returns:
    leResult
*/
/**
 * @brief Initialize a fixed heap.
 * @details Initialize <span class="param">heap</span> with
 * <span class="param">size</span> bytes
 * and <span class="param">count</span> number of blocks
 * and using the data buffer specified by <span class="param">data</span>.
 * @code
 * leFixedHeap* heap;
 * leResult res = leFixedHeap_Init(heap, size, count, data);
 * @endcode
 * @param heap is the fixed heap to scan.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leFixedHeap_Init(leFixedHeap* heap,
                          uint32_t size,
                          uint32_t count,
                          uint8_t* data);

// *****************************************************************************
/* Function:
    void leFixedHeap_Destroy(leFixedHeap* heap)

  Summary:
    Destroys a fixed heap object.

  Description:
    Destroys a fixed heap object.  All pointers that are allocated from this
    heap should be considered invalid.

  Parameters:
    leFixedHeap* heap - pointer to the heap object

  Returns:
    leResult
*/
/**
 * @brief Destroy a fixed heap.
 * @details Destroys <span class="param">heap</span>.
 * All pointers that are allocated from this heap should be considered invalid.
 * @code
 * leFixedHeap* heap;
 * leFixedHeap_Destroy(heap);
 * @endcode
 * @param heap is the fixed heap to scan.
 * @return void.
 */
void leFixedHeap_Destroy(leFixedHeap* heap);

#if LE_USE_DEBUG_ALLOCATOR == 1
// *****************************************************************************
/* Function:
    void* leFixedHeap_Alloc(leFixedHeap* heap,
                            uint32_t lineNum,
                            const char* funcName,
                            const char* fileName)

  Summary:
    Allocates a block from a fixed heap.

  Description:
    Allocates a block from a fixed heap.  This is the debug version of this
    function and requires additional parameters.

    Use the LE_MALLOC macro to automatically differentiate between
    this function and its non-debug counterpart.

  Parameters:
    leFixedHeap* heap - pointer to the heap object
    uint32_t lineNum - the file line number where this allocation occured
    const char* funcName - the function name where this allocation occured
    const char* fileName - the file name where this allocation occured

  Returns:
    leResult
*/
/**
 * @brief Allocates a block.
 * @details Allocates a block from the fixed heap <span class="param">heap</span>.
 * This is the debug version of this function and requires
 * additional parameters which includes the file number
 * <span class="param">lineNum</span> the function
 * name <span class="param">funcName</span> and the
 * file name <span class="param">fileName</span> where
 * the allocation took place.
 * @code
 * leFixedHeap* heap;
 * void * block = leFixedHeap_Alloc(heap, lineNum, funcName, fileName);
 * @endcode
 * @param heap is the fixed heap to scan.
 * @return allocated block.
 */
void* leFixedHeap_Alloc(leFixedHeap* heap,
                        uint32_t lineNum,
                        const char* funcName,
                        const char* fileName);
#else
// *****************************************************************************
/* Function:
    void* leFixedHeap_Alloc(leFixedHeap* heap)

  Summary:
    Allocates a block from a fixed heap.

  Description:
    Allocates a block from a fixed heap.

    Use the LE_MALLOC macro to automatically differentiate between
    this function and its debug counterpart.

  Parameters:
    leFixedHeap* heap - pointer to the heap object

  Returns:
    leResult
*/

/**
 * @brief Allocates a block.
 * @details Allocates a block from the fixed heap <span class="param">heap</span>.
 * @code
 * leFixedHeap* heap;
 * void * block = leFixedHeap_Alloc(heap);
 * @endcode
 * @param heap is the fixed heap to scan.
 * @return allocated block.
 */
void* leFixedHeap_Alloc(leFixedHeap* heap);
#endif

// *****************************************************************************
/* Function:
    void* leFixedHeap_Free(leFixedHeap* heap, void* ptr)

  Summary:
    Frees a block from a fixed heap.

  Description:
    Frees a block from a fixed heap.

  Parameters:
    leFixedHeap* heap - pointer to the heap object
    void* ptr - the pointer to free

  Returns:
    leResult
*/
/**
 * @brief Free a block from heap.
 * @details Frees the given block <span class="param">ptr</span> from
 * the specified <span class="param">heap</span>
 * @code
 * leFixedHeap* heap;
 * leFixedHeap_Free(heap, ptr);
 * @endcode
 * @param heap is the fixed heap to scan.
 * @param ptr is the block ptr to free.
 * @return void.
 */
void leFixedHeap_Free(leFixedHeap* heap, void* ptr);

// *****************************************************************************
/* Function:
    leBool leFixedHeap_Contains(leFixedHeap* heap, void* ptr)

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
/**
 * @brief Query a heap.
 * @details Queries <span class="param">heap</span> to
 * determine if it contains a block ptr as specified by <span class="param">ptr</span>
 * @code
 * leFixedHeap* heap;
 * leBool hasBlock = leFixedHeap_Contains(heap, ptr);
 * @endcode
 * @param heap is the fixed heap to scan.
 * @param ptr is the block ptr to query.
 * @return LE_TRUE if block ptr is found, otherwise LE_FALSE.
 */
leBool leFixedHeap_Contains(leFixedHeap* heap, void* ptr);

// *****************************************************************************
/* Function:
    leResult leFixedHeap_Validate(leFixedHeap* heap)

  Summary:
    Validates a heap.

  Description:
    Validates a heap.  Validation involves scanning the heap blocks to look
    for errors.  The amount of checking is dependent on the heap debug level.
    When heap debugging is enabled the heap will use extra memory to
    store block checksums.  These checksums are verified to look for buffer
    overruns or other errors.

  Parameters:
    leFixedHeap* heap - pointer to the heap object

  Returns:
    leResult
*/
/**
 * @brief Validate a heap.
 * @details Validates the heap specified by <span class="param">heap</span>.
 * Validation involves scanning the heap blocks to look
 * for errors.  The amount of checking is dependent on the heap
 * debug level. When heap debugging is enabled the heap
 * will use extra memory to store block checksums.  These
 * checksums are verified to look for buffer overruns or other errors.
 * @code
 * leFixedHeap* heap;
 * leResult res = leFixedHeap_Validate(heap);
 * @endcode
 * @param heap is the fixed heap.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leFixedHeap_Validate(leFixedHeap* heap);

#ifdef _WIN32
// *****************************************************************************
/* Function:
    leFixedHeap_Dump(leFixedHeap* heap, leBool dumpRecords)

  Summary:
    Dumps the contents of a heap to STDOUT.

  Description:
    Dumps the contents of a heap to STDOUT.

  Parameters:
    leFixedHeap* heap - pointer to the heap object
    leBool dumpRecords - set to true to dump the actual block data arrays

  Returns:
    void
*/
/**
 * @brief Dump contents of heap
 * @details Dumps the contents of <span class="param">hea</span>.
 * If <span class="param">dumpRecords</span> is set
 * to true the actual block data array will be dumped.
 * @code
 * leFixedHeap* heap;
 * leBool dumpRecords=true;
 * leFixedHeap_Dump(heap, dumpRecords)
 * @endcode
 * @param heap is the fixed heap.
 * @param dumpRecords determines whether the actual block data is dumped.
 * @return void.
 */
void leFixedHeap_Dump(leFixedHeap* heap, leBool dumpRecords);
#endif

#endif

#endif // LE_MEMORY_MANAGER_ENABLE

#endif /* LEGATO_FIXEDHEAP_H */
