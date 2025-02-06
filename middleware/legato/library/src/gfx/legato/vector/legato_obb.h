#ifndef LE_OBB_H
#define LE_OBB_H

#include "gfx/legato/vector/legato_linef.h"

#ifdef __cplusplus
extern "C" {
#endif

struct leAABB;

typedef struct leOBB
{
    struct leVector2 origin;
    struct leVector2 extents;
    leReal_i16 angle;
} leOBB;

leResult leOBB_FromLine(const struct leLineF* ln,
                        leReal_i16 height,
                        struct leOBB* obb);

leBool leOBB_ContainsPoint(const struct leOBB* obb,
                           const struct leVector2* pnt);

leResult leOBB_AABB(const struct leOBB* obb,
                    struct leAABB* aabb);

leResult leOBB_Points(const struct leOBB* obb,
                      struct leVector2* pnts);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_OBB_H */
