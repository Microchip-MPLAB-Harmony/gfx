#ifndef DRV_ISI_COMMON_H
#define DRV_ISI_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <toolchain_specifics.h>

/**
 * @brief Struct defining a frame buffer descriptor
 * @details These frame buffer descriptors are used by both the preview
 *          and codec pipelines.  They define a chain of buffers
 *          that are used in sequence when decoding frame data
 */
typedef struct
{
    void* address; // the address of the buffer
    uint32_t ctrl; // control information for the peripheral (set to zero)
    void* next;    // the buffer to the next descriptor in the chain
                   // ensure that the last descriptor properly references the first 
} DRV_ISI_FrameBufferDescriptor;

/**
 * @brief Helper function that sleeps for a given time period
 * @details Helper function that sleeps for a given time period
 * @param ms the number of milliseconds to sleep
 */
void DRV_ISI_Sleep(int ms);

#endif // DRV_ISI_COMMON_H
