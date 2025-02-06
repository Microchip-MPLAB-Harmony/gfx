#include "gfx/legato/vector/legato_point.h"

void lePoint_Negate(const struct lePoint* v,
                    struct lePoint* o)
{
    struct lePoint res;

    res.x = -v->x;
    res.y = -v->y;

    *o = res;
}

leBool lePoint_Equals(const struct lePoint* l,
                      const struct lePoint* r)
{
    return(l->x == r->x && l->y == r->y);
}

void lePoint_Add(const struct lePoint* l,
                 const struct lePoint* r,
                 struct lePoint* o)
{
    struct lePoint res;

    res.x = l->x + r->x;
    res.y = l->y + r->y;

    *o = res;
}

void lePoint_Subtract(const struct lePoint* l,
                      const struct lePoint* r,
                      struct lePoint* o)
{
    struct lePoint res;

    res.x = l->x - r->x;
    res.y = l->y - r->y;

    *o = res;
}

void lePoint_Scale(const struct lePoint* v,
                   float f,
                   struct lePoint* o)
{
    struct lePoint res;

    res.x = (int32_t)((float)v->x * f);
    res.y = (int32_t)((float)v->y * f);

    *o = res;
}

const struct lePoint LE_POINT_ZERO =
{
    .x = 0,
    .y = 0
};