#ifndef LE_REAL_U8_H
#define LE_REAL_U8_H

#include "gfx/legato/common/legato_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t leReal_u8;


#define LE_REAL_U8_FRACTION_BITS  6
#define LE_REAL_U8_FRACTION_SCL   (1 << LE_REAL_U8_FRACTION_BITS)
#define LE_REAL_U8_255            (255 << LE_REAL_U8_FRACTION_BITS)
#define LE_REAL_U8_MAX            (0xFFFFFFFF)
#define LE_REAL_U8_MIN            (0x00000000)

#define LE_REAL_U8_ONE            LE_REAL_U8_FRACTION_SCL
#define LE_REAL_U8_ONE_OVER_TWO   (0x20)
#define LE_REAL_U8_EPSILON        (0x4)

#define LE_REAL_U8_FROM_UINT(i)   ((i) << LE_REAL_U8_FRACTION_BITS)
#define LE_REAL_U8_TO_UINT(r)     ((r) >> LE_REAL_U8_FRACTION_BITS)
#define LE_REAL_U8_FROM_FLOAT(f)  ((int32_t)((f) * LE_REAL_U8_FRACTION_SCL))
#define LE_REAL_U8_TO_FLOAT(r)    ((float)(r) / LE_REAL_U8_FRACTION_SCL)
#define LE_REAL_U8_FROM_I16(u)    ((u) >> (LE_REAL_I16_FRACTION_BITS - LE_REAL_U8_FRACTION_BITS))
#define LE_REAL_U8_TO_I16(u)      ((u) << (LE_REAL_I16_FRACTION_BITS - LE_REAL_U8_FRACTION_BITS))
#define LE_REAL_U8_CMP(l, r)      (((l) >= (r) - LE_REAL_U8_EPSILON) && ((l) <= (r) + LE_REAL_U8_EPSILON))
#define LE_REAL_U8_SQUARE(r)      LE_REAL_U8_MULTIPLY(r, r)
#define LE_REAL_U8_MULTIPLY(l, r) ((leReal_u8)((((uint64_t)(l) * (r)) / LE_REAL_U8_FRACTION_SCL)))
#define LE_REAL_U8_DIVIDE(l, r)   ((leReal_u8)(((uint64_t)(l) * LE_REAL_U8_FRACTION_SCL) / (r)))

leReal_u8 leReal_u8_Min(leReal_u8 l, leReal_u8 r);

leReal_u8 leReal_u8_Max(leReal_u8 l, leReal_u8 r);

leReal_u8 leReal_u8_Round(leReal_u8 rl);

leReal_u8 leReal_u8_Ceiling(leReal_u8 rl);

leReal_u8 leReal_u8_Middle(leReal_u8 rl);

leReal_u8 leReal_u8_SquareRoot(leReal_u8 rl);

leReal_u8 leReal_u8_InverseSquareRoot(leReal_u8 rl);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_REAL_U8_H */
