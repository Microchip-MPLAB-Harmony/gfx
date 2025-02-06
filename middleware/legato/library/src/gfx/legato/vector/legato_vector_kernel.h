#ifndef LE_VECTOR_KERNEL_H
#define LE_VECTOR_KERNEL_H

#include "gfx/legato/vector/legato_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

struct leVector_KernelContext
{
    leColor color;
    leReal_i16 alpha;
    enum leAntialiasingMode aaMode;
};

struct leVector_Kernel
{
    struct leVector_KernelContext* context;
    struct leVector2 origin;
    struct leRect scanArea;
    struct lePoint scanPoint;
    struct leVector2 pixelCenter;
    struct leVector2 testPoint;
    uint32_t hitCount;
    void (*evaluatePoint)(struct leVector_Kernel*);
    uint8_t (*shadeFragment)(struct leVector_Kernel*);
};

void leVector_Kernel_Exec(struct leVector_Kernel* krn);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LE_VECTOR_KERNEL_H