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
/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_memory.h

  Summary:
    File containing various memory-related constructs.
*******************************************************************************/
/** \file legato_memory.h
 * @brief Memory functions and definitions.
 *
 * @details File containing various memory-related constructs.
 */

#ifndef LEGATO_MEMORY_H
#define LEGATO_MEMORY_H

#include "gfx/legato/common/legato_common.h"

#if LE_MEMORY_MANAGER_ENABLE == 1

#define LE_FIXED_HEAP_COUNT 7

/**
 * @brief This struct represents a variable heap usage report.
 */
typedef struct leVariableHeapUsageReport
{
    size_t size;        /**< size of report. */
    size_t maxUsage;    /**< max usage. */
    size_t used;        /**< used. */
} leVariableHeapUsageReport;

/**
 * @brief This enum represents a fixed heap sizes support.
 */
enum leFixedHeapSize
{
    FH_16,
    FH_32,
    FH_64,
    FH_128,
    FH_196,
    FH_256
};

/**
 * @brief This struct represents a fixed heap usage report.
 */
typedef struct leFixedHeapUsageReport
{
    size_t logicalSize;
    size_t numElements;
    size_t currentCapacity;
    size_t maxUsage;
} leFixedHeapUsageReport;

/**
 * @brief This struct represents fixed and variable heap reports.
 */
typedef struct leMemoryStatusReport
{
    leVariableHeapUsageReport variableHeapReport;
    leFixedHeapUsageReport fixedHeapReport[LE_FIXED_HEAP_COUNT - 1];
} leMemoryStatusReport;

/**
 * @brief Memory Init.
 * @details .
 * @code
 * leResult res = leMemory_Init()
 * @endcode
 * @param void.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leMemory_Init(void);

#if LE_USE_DEBUG_ALLOCATOR == 1
#define LE_MALLOC(size) leMalloc(size, __LINE__, __FUNCTION__, __FILE__)
#define LE_REALLOC(ptr, size) leRealloc(ptr, size, __LINE__, __FUNCTION__, __FILE__)
#else
#define LE_MALLOC(size) leMalloc(size)
#define LE_REALLOC(ptr, size) leRealloc(ptr, size)
#endif

#define LE_FREE(ptr) leFree(ptr)

#if LE_USE_DEBUG_ALLOCATOR == 1
/**
 * @brief Malloc Memory.
 * @details .
 * @code
 * leResult res = leMemory_Init()
 * @endcode
 * @param void.
 * @return void.
 */
void* leMalloc(size_t size,
               size_t line,
               const char* func,
               const char* file);

/**
 * @brief Malloc Memory.
 * @details .
 * @code
 * leResult res = leMemory_Init()
 * @endcode
 * @param ptr.
 * @param size.
 * @param line.
 * @param func.
 * @param file.
 * @return void.
 */
void* leRealloc(void* ptr,
                size_t size,
                size_t line,
                const char* func,
                const char* file);

/**
 * @brief Malloc Memory.
 * @details .
 * @code
 * leResult res = leMemory_Init()
 * @endcode
 * @param void.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
void leFree(void* ptr);
#else
/**
 * @brief Malloc Memory.
 * @details .
 * @code
 * void* mem = leMalloc();
 * @endcode
 * @param size.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
void* leMalloc(size_t size);

/**
 * @brief Realloc Memory.
 * @details .
 * @code
 * void* mem = leRealloc(ptr, size);
 * @endcode
 * @param ptr.
 * @param size.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
void* leRealloc(void* ptr,
                size_t size);

/**
 * @brief Free Memory.
 * @details .
 * @code
 * void* ptr;
 * leFree(ptr);
 * @endcode
 * @param ptr.
 * @return void.
 */
void leFree(void* ptr);
#endif

/**
 * @brief Get Usage Report.
 * @details .
 * @code
 * leMemoryGetUsageReport(rpt)
 * @endcode
 * @param rpt.
 * @return void.
 */
void leMemoryGetUsageReport(leMemoryStatusReport* rpt);

#ifdef _WIN32
void leMemoryDumpAllocations(void);
#endif

/**
 * @brief Validate Memory Heaps
 * @details .
 * @code
 * leResult res = leMemoryValidateHeaps()
 * @endcode
 * @param void.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leMemoryValidateHeaps(void);

#else

#include <stdlib.h>

#define LE_MALLOC(size) malloc(size)
#define LE_REALLOC(ptr, size) realloc(ptr, size)
#define LE_FREE(ptr) free(ptr)
#endif // LE_MEMORY_MANAGER_ENABLE

#endif /* LEGATO_MEMORY_H */
