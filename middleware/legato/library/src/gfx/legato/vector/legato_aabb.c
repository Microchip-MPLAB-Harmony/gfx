#include "gfx/legato/vector/legato_aabb.h"

#include <math.h>

#include "gfx/legato/common/legato_rect.h"
#include "gfx/legato/vector/legato_obb.h"

leBool leAABB_ContainsPoint(const struct leAABB* aabb,
                            const struct leVector2* pnt)
{
    if(pnt->x < aabb->origin.x - aabb->extents.x ||
       pnt->x > aabb->origin.x + aabb->extents.x ||
       pnt->y < aabb->origin.y - aabb->extents.y ||
       pnt->y > aabb->origin.y + aabb->extents.y)
    {
        return LE_FALSE;
    }

    return LE_TRUE;
}

void leAABB_ToRect(const struct leAABB* aabb,
                   struct leRect* rct)
{
    leReal_i16 x0;
    leReal_i16 y0;
    leReal_i16 x1;
    leReal_i16 y1;

    x0 = aabb->origin.x - aabb->extents.x;
    y0 = aabb->origin.y - aabb->extents.y;
    x1 = aabb->origin.x + aabb->extents.x;
    y1 = aabb->origin.y + aabb->extents.y;

    rct->x = (int16_t)leReal_i16_ToInt(leReal_i16_Floor(x0));
    rct->y = (int16_t)leReal_i16_ToInt(leReal_i16_Floor(y0));
    rct->width = (int16_t)((leReal_i16_ToInt(leReal_i16_Ceiling(x1))) - rct->x);
    rct->height = (int16_t)((leReal_i16_ToInt(leReal_i16_Ceiling(y1))) - rct->y);
}

void leAABB_Combine(const struct leAABB* l,
                    const struct leAABB* r,
                    struct leAABB* res)
{
    leReal_i16 minX = LE_REAL_I16_MAX;
    leReal_i16 maxX = LE_REAL_I16_MIN;
    leReal_i16 minY = LE_REAL_I16_MAX;
    leReal_i16 maxY = LE_REAL_I16_MIN;

    minX = leReal_i16_Min(l->origin.x - l->extents.x, r->origin.x - r->extents.x);
    maxX = leReal_i16_Max(l->origin.x + l->extents.x, r->origin.x + r->extents.x);
    minY = leReal_i16_Min(l->origin.y - l->extents.y, r->origin.y - r->extents.y);
    maxY = leReal_i16_Max(l->origin.y + l->extents.y, r->origin.y + r->extents.y);

    res->extents.x = LE_REAL_I16_DIVIDE(maxX - minX, LE_REAL_I16_TWO);
    res->extents.y = LE_REAL_I16_DIVIDE(maxY - minY, LE_REAL_I16_TWO);
    res->origin.x = maxX - res->extents.x;
    res->origin.y = maxY - res->extents.y;
}