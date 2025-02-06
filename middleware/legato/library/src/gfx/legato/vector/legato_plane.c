#include "gfx/legato/vector/legato_plane.h"

#include <math.h>

void lePlane_Create(const struct leVector2* origin,
                    const struct leVector2* normal,
                    struct lePlane* p)
{
    p->normal = *normal;
    p->d = leVector2_Dot(origin, normal);
}

leReal_i16 lePlane_Distance(const struct lePlane* p,
                            const struct leVector2* pt)
{
    struct leVector2 pos;
    struct leVector2 distVec;

    leVector2_Scale(&p->normal, p->d, &pos);

    leVector2_Subtract(pt, &pos, &distVec);

    return leVector2_Dot(&distVec, &p->normal);
}
