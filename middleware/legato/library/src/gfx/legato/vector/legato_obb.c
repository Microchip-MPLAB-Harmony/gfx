#include "gfx/legato/vector/legato_obb.h"

#include <limits.h>
#include <math.h>

#include "gfx/legato/vector/legato_aabb.h"
#include "gfx/legato/vector/legato_linef.h"
#include "gfx/legato/vector/legato_vector2.h"

leResult leOBB_FromLine(const struct leLineF* ln,
                        leReal_i16 height,
                        struct leOBB* obb)
{
    struct leVector2 vec;
    leReal_i16 rot;

    if(height == 0)
        return LE_FAILURE;

    leLineF_Center(ln, &obb->origin);

    vec.x = ln->p1.x - ln->p0.x;
    vec.y = ln->p1.y - ln->p0.y;

    rot = leVector2_AngleOf(&vec);

    leVector2_Rotate(&vec, -rot, &vec);

    obb->extents.x = LE_REAL_I16_MULTIPLY(vec.x, LE_REAL_I16_ONE_OVER_TWO);
    obb->extents.y = LE_REAL_I16_MULTIPLY(height, LE_REAL_I16_ONE_OVER_TWO);
    obb->angle = leLineF_Angle(ln);

    return LE_SUCCESS;
}

leBool leOBB_ContainsPoint(const struct leOBB* obb,
                           const struct leVector2* pnt)
{
    struct leVector2 rotPnt;

    rotPnt.x = pnt->x - obb->origin.x;
    rotPnt.y = pnt->y - obb->origin.y;

    leVector2_Rotate(&rotPnt, -obb->angle, &rotPnt);

    if(rotPnt.x >= -obb->extents.x &&
       rotPnt.x <= obb->extents.x &&
       rotPnt.y >= -obb->extents.y &&
       rotPnt.y <= obb->extents.y)
        return LE_TRUE;

    return LE_FALSE;
}

leResult leOBB_AABB(const struct leOBB* obb,
                    struct leAABB* aabb)
{
    struct leVector2 pnts[4];
    leReal_i16 minX;
    leReal_i16 maxX;
    leReal_i16 minY;
    leReal_i16 maxY;
    uint32_t itr;

    if(obb == NULL || aabb == NULL)
        return LE_FAILURE;

    leOBB_Points(obb, pnts);

    minX = LE_REAL_I16_MAX;
    maxX = -LE_REAL_I16_MAX;
    minY = LE_REAL_I16_MAX;
    maxY = -LE_REAL_I16_MAX;

    for(itr = 0; itr < 4; ++itr)
    {
        if(pnts[itr].x < minX)
        {
            minX = pnts[itr].x;
        }

        if(pnts[itr].y < minY)
        {
            minY = pnts[itr].y;
        }

        if(pnts[itr].x > maxX)
        {
            maxX = pnts[itr].x;
        }

        if(pnts[itr].y > maxY)
        {
            maxY = pnts[itr].y;
        }
    }

    aabb->origin = obb->origin;
    aabb->extents.x = LE_REAL_I16_MULTIPLY(maxX - minX, LE_REAL_I16_ONE_OVER_TWO);
    aabb->extents.y = LE_REAL_I16_MULTIPLY(maxY - minY, LE_REAL_I16_ONE_OVER_TWO);

    return LE_SUCCESS;
}

leResult leOBB_Points(const struct leOBB* obb,
                      struct leVector2* pnts)
{
    if(obb == NULL || pnts == NULL)
        return LE_FAILURE;

    pnts[0].x = -obb->extents.x;
    pnts[0].y = obb->extents.y;

    pnts[1].x = obb->extents.x;
    pnts[1].y = obb->extents.y;

    pnts[2].x = -obb->extents.x;
    pnts[2].y = -obb->extents.y;

    pnts[3].x = obb->extents.x;
    pnts[3].y = -obb->extents.y;

    leVector2_Rotate(&pnts[0], obb->angle, &pnts[0]);
    leVector2_Rotate(&pnts[1], obb->angle, &pnts[1]);
    leVector2_Rotate(&pnts[2], obb->angle, &pnts[2]);
    leVector2_Rotate(&pnts[3], obb->angle, &pnts[3]);

    pnts[0].x += obb->origin.x;
    pnts[0].y += obb->origin.y;
    pnts[1].x += obb->origin.x;
    pnts[1].y += obb->origin.y;
    pnts[2].x += obb->origin.x;
    pnts[2].y += obb->origin.y;
    pnts[3].x += obb->origin.x;
    pnts[3].y += obb->origin.y;

    return LE_SUCCESS;
}

