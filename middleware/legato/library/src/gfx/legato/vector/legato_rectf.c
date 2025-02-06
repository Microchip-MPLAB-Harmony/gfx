#include "gfx/legato/vector/legato_rectf.h"

#include "gfx/legato/common/legato_rect.h"

leResult leRectF_FromRect(struct leRectF* rctf,
                          const struct leRect* rct)
{
    leReal_i16 start;
    leReal_i16 end;

    if(rctf == NULL || rct == NULL)
        return LE_FAILURE;

    start = leReal_i16_FromInt(rct->x) + LE_REAL_I16_ONE_OVER_TWO;
    end = (leReal_i16_FromInt(rct->x + (rct->width - 1)) + LE_REAL_I16_ONE_OVER_TWO);

    rctf->extents.x = LE_REAL_I16_MULTIPLY(end - start, LE_REAL_I16_ONE_OVER_TWO);
    rctf->origin.x = start + rctf->extents.x;

    start = leReal_i16_FromInt(rct->y) + LE_REAL_I16_ONE_OVER_TWO;
    end = ((leReal_i16_FromInt(rct->y + (rct->height - 1))) + LE_REAL_I16_ONE_OVER_TWO);

    rctf->extents.y = LE_REAL_I16_MULTIPLY(end - start, LE_REAL_I16_ONE_OVER_TWO);
    rctf->origin.y = start + rctf->extents.y;

    return LE_SUCCESS;
}

leResult leRectF_ToRect(const struct leRectF* rctf,
						struct leRect* rct)
{
    leReal_i16 t;

    if(rctf == NULL || rct == NULL)
        return LE_FAILURE;

    rct->x = leReal_i16_ToInt(rctf->origin.x - rctf->extents.x);
    rct->y = leReal_i16_ToInt(rctf->origin.y - rctf->extents.y);

    //t = LE_REAL_I16_MULTIPLY(rctf->extents.x, LE_REAL_I16_TWO);
    t = leReal_i16_Ceiling(rctf->origin.x + rctf->extents.x);
    rct->width = (LE_REAL_I16_TO_INT(t) - rct->x);

    //t = LE_REAL_I16_MULTIPLY(rctf->extents.y, LE_REAL_I16_TWO);
    t = leReal_i16_Ceiling(rctf->origin.y + + rctf->extents.y);
    rct->height = (LE_REAL_I16_TO_INT(t) - rct->y);

    return LE_SUCCESS;
}

leBool leRectF_IsValid(const struct leRectF* rct)
{
    if(rct == NULL || rct->extents.x == 0.0f || rct->extents.y == 0.0f)
        return LE_FALSE;

    return LE_TRUE;
}

leResult leRectF_Set(struct leRectF* rct,
                     const struct leVector2* pnt,
                     leReal_i16 width,
                     leReal_i16 height)
{
    if(rct == NULL || pnt == NULL || width == 0 || height == 0)
        return LE_FAILURE;

    rct->extents.x = LE_REAL_I16_MULTIPLY(width, LE_REAL_I16_ONE_OVER_TWO);
    rct->extents.y = LE_REAL_I16_MULTIPLY(height, LE_REAL_I16_ONE_OVER_TWO);
    rct->origin.x = pnt->x + rct->extents.x;
    rct->origin.y = pnt->y + rct->extents.y;

    return LE_SUCCESS;
}

leResult leRectF_SetFromPoints(struct leRectF* rct,
                               const struct leVector2* tl,
                               const struct leVector2* br)
{
    leReal_i16 minX;
    leReal_i16 maxX;
    leReal_i16 minY;
    leReal_i16 maxY;

    if(rct == NULL || tl == NULL || br == NULL)
        return LE_FAILURE;

    minX = leReal_i16_Min(tl->x, br->x);
    minY = leReal_i16_Min(tl->y, br->y);
    maxX = leReal_i16_Max(tl->x, br->x);
    maxY = leReal_i16_Max(tl->y, br->y);

    rct->extents.x = LE_REAL_I16_MULTIPLY(maxX - minX, LE_REAL_I16_ONE_OVER_TWO);
    rct->extents.y = LE_REAL_I16_MULTIPLY(maxY - minY, LE_REAL_I16_ONE_OVER_TWO);
    rct->origin.x = minX + rct->extents.x;
    rct->origin.y = minY + rct->extents.y;

    return LE_SUCCESS;
}

leResult leRectF_GetPoint(const struct leRectF* rct,
                          enum leRectF_AnchorPoint anchor,
                          struct leVector2* pnt)
{
    struct leVector2 res;

    if(rct == NULL || pnt == NULL)
        return LE_FAILURE;

    res = rct->origin;

    switch(anchor)
    {
        case LE_RECTF_TOPLEFT:
        {
            res.x += rct->extents.x;
            res.y -= rct->extents.y;

            break;
        }
        case LE_RECTF_TOP:
        {
            res.y -= rct->extents.y;

            break;
        }
        case LE_RECTF_TOPRIGHT:
        {
            res.x += rct->extents.x;
            res.y -= rct->extents.y;

            break;
        }
        case LE_RECTF_LEFT:
        {
            res.x -= rct->extents.x;

            break;
        }
        case LE_RECTF_RIGHT:
        {
            res.x += rct->extents.x;

            break;
        }
        case LE_RECTF_BOTTOMLEFT:
        {
            res.x -= rct->extents.x;
            res.y += rct->extents.y;

            break;
        }
        case LE_RECTF_BOTTOM:
        {
            res.y = rct->extents.y;

            break;
        }
        case LE_RECTF_BOTTOMRIGHT:
        {
            res.x += rct->extents.x;
            res.y += rct->extents.y;

            break;
        }
        default:
        {
            break;
        }
    }

    *pnt = res;

    return LE_SUCCESS;
}

leResult leRectF_SetPoint(struct leRectF* rct,
                          enum leRectF_AnchorPoint anchor,
                          const struct leVector2* pnt)
{
    struct leVector2 p0;
    struct leVector2 p1;

    if(rct == NULL || pnt == NULL)
        return LE_FAILURE;

    p0.x = 0;
    p0.y = 0;
    p1.x = 0;
    p1.y = 0;

    switch(anchor)
    {
        case LE_RECTF_TOPLEFT:
        {
            p0 = *pnt;
            p1 = rct->origin;
            p1.x += rct->extents.x;
            p1.y += rct->extents.y;

            break;
        }
        case LE_RECTF_TOP:
        {
            p0.x = rct->origin.x - rct->extents.x;
            p0.y = pnt->y;
            p1 = rct->origin;
            p1.x += rct->extents.x;
            p1.y += rct->extents.y;

            break;
        }
        case LE_RECTF_TOPRIGHT:
        {
            p0.x = rct->origin.x - rct->extents.x;
            p0.y = pnt->y;
            p1.x += pnt->x;
            p1.y = rct->origin.y + rct->extents.y;

            break;
        }
        case LE_RECTF_LEFT:
        {
            p0.x = pnt->x;
            p0.y = rct->origin.y - rct->extents.y;
            p1 = rct->origin;
            p1.x += rct->extents.x;
            p1.y += rct->extents.y;

            break;
        }
        case LE_RECTF_CENTER:
        {
            rct->origin = *pnt;

            return LE_SUCCESS;
        }
        case LE_RECTF_RIGHT:
        {
            p0.x = rct->origin.x - rct->extents.x;
            p0.y = pnt->y;
            p1.x = pnt->x;
            p1.y = rct->origin.y + rct->extents.y;

            break;
        }
        case LE_RECTF_BOTTOMLEFT:
        {
            p0.x = pnt->x;
            p0.y = rct->origin.y - rct->extents.y;
            p1.x = rct->origin.x + rct->extents.x;
            p1.y = pnt->y;

            break;
        }
        case LE_RECTF_BOTTOM:
        {
            p0 = rct->origin;
            p0.x -= rct->extents.x;
            p0.y -= rct->extents.y;
            p1.x = rct->origin.x + rct->extents.x;
            p1.y = pnt->y;

            break;
        }
        case LE_RECTF_BOTTOMRIGHT:
        {
            p0.x = pnt->x;
            p0.y = rct->origin.y - rct->extents.y;
            p1.x = rct->origin.x + rct->extents.x;
            p1.y = pnt->y;

            break;
        }
    }

    return leRectF_SetFromPoints(rct, &p0, &p1);
}

leResult leRectF_AdjustPoint(struct leRectF* rct,
                             enum leRectF_AnchorPoint anchor,
                             const struct leVector2* mag)
{
    struct leVector2 p0;
    struct leVector2 p1;

    if(rct == NULL || mag == NULL)
        return LE_FAILURE;

    p0.x = rct->origin.x - rct->extents.x;
    p0.y = rct->origin.y - rct->extents.y;
    p1.x = rct->origin.x + rct->extents.x;
    p1.y = rct->origin.y + rct->extents.y;

    switch(anchor)
    {
        case LE_RECTF_TOPLEFT:
        {
            p0.x += mag->x;
            p0.y += mag->y;

            break;
        }
        case LE_RECTF_TOP:
        {
            p0.y += mag->y;

            break;
        }
        case LE_RECTF_TOPRIGHT:
        {
            p1.x += mag->x;
            p0.y += mag->y;

            break;
        }
        case LE_RECTF_LEFT:
        {
            p0.x += mag->x;

            break;
        }
        case LE_RECTF_CENTER:
        {
            rct->origin.x += mag->x;
            rct->origin.y += mag->y;

            return LE_SUCCESS;
        }
        case LE_RECTF_RIGHT:
        {
            p1.x += mag->x;

            break;
        }
        case LE_RECTF_BOTTOMLEFT:
        {
            p0.x += mag->x;
            p1.y += mag->y;

            break;
        }
        case LE_RECTF_BOTTOM:
        {
            p1.y += mag->y;

            break;
        }
        case LE_RECTF_BOTTOMRIGHT:
        {
            p1.x += mag->x;
            p1.y += mag->y;

            break;
        }
    }

    return leRectF_SetFromPoints(rct, &p0, &p1);
}

leResult leRectF_SetPosition(struct leRectF* rct,
                             enum leRectF_AnchorPoint anchor,
                             const struct leVector2* pnt)
{
    if(rct == NULL || pnt == NULL)
        return LE_FAILURE;

    switch(anchor)
    {
        case LE_RECTF_TOPLEFT:
        {
            rct->origin.x = pnt->x + rct->extents.x;
            rct->origin.y = pnt->y + rct->extents.y;

            break;
        }
        case LE_RECTF_TOP:
        {
            rct->origin.x = pnt->x;
            rct->origin.y = pnt->y + rct->extents.y;

            break;
        }
        case LE_RECTF_TOPRIGHT:
        {
            rct->origin.x = pnt->x - rct->extents.x;
            rct->origin.y = pnt->y + rct->extents.y;

            break;
        }
        case LE_RECTF_LEFT:
        {
            rct->origin.x = pnt->x + rct->extents.x;
            rct->origin.y = pnt->y;

            break;
        }
        case LE_RECTF_CENTER:
        {
            rct->origin = *pnt;

            break;
        }
        case LE_RECTF_RIGHT:
        {
            rct->origin.x = pnt->x - rct->extents.x;
            rct->origin.y = pnt->y;

            break;
        }
        case LE_RECTF_BOTTOMLEFT:
        {
            rct->origin.x = pnt->x + rct->extents.x;
            rct->origin.y = pnt->y - rct->extents.y;

            break;
        }
        case LE_RECTF_BOTTOM:
        {
            rct->origin.x = pnt->x;
            rct->origin.y = pnt->y - rct->extents.y;

            break;
        }
        case LE_RECTF_BOTTOMRIGHT:
        {
            rct->origin.x = pnt->x - rct->extents.x;
            rct->origin.y = pnt->y - rct->extents.y;

            break;
        }
    }

    return LE_SUCCESS;
}

leResult leRectF_Translate(struct leRectF* rct,
                           const struct leVector2* mag)
{
    if(rct == NULL || mag == NULL)
        return LE_FAILURE;

    rct->origin.x += mag->x;
    rct->origin.y += mag->y;

    return LE_SUCCESS;
}

leReal_i16 leRectF_GetWidth(const struct leRectF* rct)
{
    if(rct == NULL)
        return 0;

    return LE_REAL_I16_MULTIPLY(rct->extents.x, LE_REAL_I16_TWO);
}

leResult leRectF_SetWidth(struct leRectF* rct,
                          leReal_i16 w)
{
    if(rct == NULL || w < 0)
        return LE_FAILURE;

    rct->extents.x = LE_REAL_I16_MULTIPLY(w, LE_REAL_I16_ONE_OVER_TWO);

    return LE_SUCCESS;
}

leReal_i16 leRectF_GetHeight(const struct leRectF* rct)
{
    if(rct == NULL)
        return 0;

    return LE_REAL_I16_MULTIPLY(rct->extents.y, LE_REAL_I16_TWO);
}

leResult leRectF_SetHeight(struct leRectF* rct,
                           leReal_i16 h)
{
    if(rct == NULL || h < 0)
        return LE_FAILURE;

    rct->extents.y = LE_REAL_I16_MULTIPLY(h, LE_REAL_I16_ONE_OVER_TWO);

    return LE_SUCCESS;
}

leBool leRectF_ContainsPoint(const struct leRectF* rct,
                             const struct leVector2* pnt)
{
    if(rct == NULL || pnt == NULL)
        return LE_FALSE;

    if(pnt->x < rct->origin.x - rct->extents.x ||
       pnt->x > rct->origin.x + rct->extents.x ||
       pnt->y < rct->origin.y - rct->extents.y ||
       pnt->y > rct->origin.y + rct->extents.y)
        return LE_FALSE;
        
    return LE_TRUE;
}

leBool leRectF_Intersects(const struct leRectF* l,
                          const struct leRectF* r)
{
    struct leVector2 pnt;

    if(l == NULL || r == NULL)
        return LE_FALSE;

    pnt.x = l->origin.x - l->extents.x;
    pnt.y = l->origin.y - l->extents.y;

    if(pnt.x < r->origin.x - r->extents.x)
        return LE_FALSE;

    pnt.x = l->origin.x + l->extents.x;

    if(pnt.x > r->origin.x + r->extents.x)
        return LE_FALSE;

    pnt.x = l->origin.x - l->extents.x;
    pnt.y = l->origin.y + l->extents.y;

    if(pnt.y < r->origin.y - r->extents.y)
        return LE_FALSE;

    pnt.y = l->origin.y + l->extents.y;

    if(pnt.y > r->origin.y + r->extents.y)
        return LE_FALSE;

    return LE_TRUE;
}