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

/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_memory.h

  Summary:
    File containing various memory-related constructs.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_MEMORY_H
#define LEGATO_MEMORY_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#define LE_FIXED_HEAP_COUNT 7

typedef struct leVariableHeapUsageReport
{
    size_t size;
    size_t maxUsage;
    size_t used;
} leVariableHeapUsageReport;

enum leFixedHeapSize
{
    FH_16,
    FH_32,
    FH_64,
    FH_128,
    FH_196,
    FH_256
};

typedef struct leFixedHeapUsageReport
{
    size_t logicalSize;
    size_t numElements;
    size_t currentCapacity;
    size_t maxUsage;
} leFixedHeapUsageReport;

typedef struct leMemoryStatusReport
{
    leVariableHeapUsageReport variableHeapReport;
    leFixedHeapUsageReport fixedHeapReport[LE_FIXED_HEAP_COUNT - 1];
} leMemoryStatusReport;

leResult leMemory_Init();

#if LE_USE_DEBUG_ALLOCATOR == 1
#define LE_MALLOC(size) leMalloc(size, __LINE__, __FUNCTION__, __FILE__)
#define LE_REALLOC(ptr, size) leRealloc(ptr, size, __LINE__, __FUNCTION__, __FILE__)
#else
#define LE_MALLOC(size) leMalloc(size)
#define LE_REALLOC(ptr, size) leRealloc(ptr, size)
#endif

#define LE_FREE(ptr) leFree(ptr)

#if LE_USE_DEBUG_ALLOCATOR == 1
LIB_EXPORT void* leMalloc(size_t size,
                          size_t line,
                          const char* func,
                          const char* file);
                
LIB_EXPORT void* leRealloc(void* ptr,
                           size_t size,
                           size_t line,
                           const char* func,
                           const char* file);
                 
LIB_EXPORT void leFree(void* ptr);
#else
LIB_EXPORT void* leMalloc(size_t size);
                
LIB_EXPORT void* leRealloc(void* ptr,
                           size_t size);
                 
LIB_EXPORT void leFree(void* ptr);
#endif

LIB_EXPORT void leMemoryGetUsageReport(leMemoryStatusReport* rpt);

#ifdef _WIN32
LIB_EXPORT void leMemoryDumpAllocations();
#endif

LIB_EXPORT leResult leMemoryValidateHeaps();

#endif /* LEGATO_MEMORY_H */
