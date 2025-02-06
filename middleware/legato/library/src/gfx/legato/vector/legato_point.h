#ifndef LE_POINT_H
#define LE_POINT_H

#include "gfx/legato/common/legato_math.h"

#ifdef __cplusplus
extern "C" {
#endif

void lePoint_Negate(const struct lePoint* v,
                    struct lePoint* o);

leBool lePoint_Equals(const struct lePoint* l,
                      const struct lePoint* r);

void lePoint_Add(const struct lePoint* l,
                 const struct lePoint* r,
                 struct lePoint* o);

void lePoint_Subtract(const struct lePoint* l,
                      const struct lePoint* r,
                      struct lePoint* o);

void lePoint_Scale(const struct lePoint* v,
                   float f,
                   struct lePoint* o);

extern const struct lePoint LE_POINT_ZERO;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_POINT_H */
