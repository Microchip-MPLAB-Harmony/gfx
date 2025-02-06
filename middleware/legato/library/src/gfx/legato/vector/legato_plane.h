#ifndef LE_PLANE_H
#define LE_PLANE_H

#include "gfx/legato/vector/legato_vector2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lePlane
{
    struct leVector2 normal;
    leReal_i16 d;
} lePlane;

void lePlane_Create(const struct leVector2* origin,
                    const struct leVector2* normal,
                    struct lePlane* o);

leReal_i16 lePlane_Distance(const struct lePlane* p,
                            const struct leVector2* pt);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_PLANE_H */
