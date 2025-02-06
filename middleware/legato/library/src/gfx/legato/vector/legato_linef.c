#include "gfx/legato/vector/legato_linef.h"

leReal_i16 leLineF_Length(const struct leLineF* ln)
{
    struct leVector2 delta;

    leVector2_Subtract(&ln->p1, &ln->p0, &delta);

    return leVector2_Length(&delta);
}

leReal_i16 leLineF_Angle(const struct leLineF* ln)
{
    struct leVector2 vec;

    vec.x = ln->p1.x - ln->p0.x;
    vec.y = ln->p1.y - ln->p0.y;

    return leVector2_AngleOf(&vec);
}

void leLineF_Center(const struct leLineF* ln,
                    struct leVector2* pnt)
{
    pnt->x = LE_REAL_I16_DIVIDE(ln->p0.x + ln->p1.x, LE_REAL_I16_TWO);
    pnt->y = LE_REAL_I16_DIVIDE(ln->p0.y + ln->p1.y, LE_REAL_I16_TWO);
}

leReal_i16 leLineF_PointDistance(const struct leLineF* ln,
                                 const struct leVector2* pnt)
{
    leReal_i16 t0;
    leReal_i16 t1;

    t0 = LE_REAL_I16_MULTIPLY(ln->p1.x - ln->p0.x, ln->p0.y - pnt->y);
    t1 = LE_REAL_I16_MULTIPLY(ln->p0.x - pnt->x, ln->p1.y - ln->p0.y);

    t0 = leReal_i16_AbsoluteValue(t0 - t1);

    return LE_REAL_I16_DIVIDE(t0, leLineF_Length(ln));
}

void leLineF_ClosestPoint(const struct leLineF* ln,
                          const struct leVector2* pnt,
                          struct leVector2* res)
{
    struct leVector2 vl, proj, l0p, l1p;
    leReal_i16 dotstart;
    leReal_i16 dotend;

    leVector2_Subtract(&ln->p1, &ln->p0, &vl);
    leVector2_Subtract(pnt, &ln->p0, &l0p);
    leVector2_Subtract(pnt, &ln->p1, &l1p);

    dotstart = leVector2_Dot(&vl, &l0p);
    dotend = leVector2_Dot(&vl, &l1p);

    if(dotstart < 0)
    {
        *res = ln->p0;

        return;
    }

    if(dotend > 0)
    {
        *res = ln->p1;

        return;
    }

    leVector2_ProjectOnto(&l0p, &vl, &proj);

    res->x = proj.x;
    res->y = proj.y;
}

leResult leLineF_Normal(const struct leLineF* ln,
                        struct leVector2* nrm)
{
    leReal_i16 dx;
    leReal_i16 dy;
    struct leLineF seg;

    dx = ln->p1.x - ln->p0.x;
    dy = ln->p1.y - ln->p0.y;

    seg.p0.x = -dy;
    seg.p0.y = dx;
    seg.p1.x = dy;
    seg.p1.y = -dx;

    leVector2_Subtract(&seg.p1,
                       &seg.p0,
                       nrm);

    leVector2_Normalize(nrm, nrm);

    return LE_SUCCESS;
}

#if 0
leBool leLine_PointSideQuery(const struct leLine* ln,
                             const struct lePoint* pnt,
                             const struct lePoint* sign)
{
    leInt32 ptsin = ((sign->x)) * (pnt->y) - ((sign->y) * (pnt->x));
    leInt32 d = ((pt->x) * (linePt->y)) - ((pt->y * linePt->x));

    if(ptsin < 0 && d < 0)
        return LE_TRUE;

    if(ptsin > 0 && d > 0)
        return LE_TRUE;

    if(ptsin == d)
        return LE_TRUE;

    return LE_FALSE;
}
#endif