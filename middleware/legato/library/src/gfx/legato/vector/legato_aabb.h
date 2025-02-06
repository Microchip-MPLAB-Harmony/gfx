#ifndef LE_AABB_H
#define LE_AABB_H

#include "gfx/legato/vector/legato_linef.h"

#ifdef __cplusplus
extern "C" {
#endif

struct leOBB;
struct leRect;

typedef struct leAABB
{
    struct leVector2 origin;
    struct leVector2 extents;
} leAABB;

leBool leAABB_ContainsPoint(const struct leAABB* aabb,
                            const struct leVector2* pnt);

void leAABB_EncloseOBB(const struct leOBB* obb,
                       struct leAABB* abb);

void leAABB_ToRect(const struct leAABB* aabb,
                   struct leRect* rct);

void leAABB_Combine(const struct leAABB* l,
                    const struct leAABB* r,
                    struct leAABB* res);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_AABB_H */
