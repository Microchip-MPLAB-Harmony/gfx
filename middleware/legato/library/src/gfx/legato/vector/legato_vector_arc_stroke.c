#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/vector/legato_vector_kernel.h"

#define PI_ANG16 ((int32_t)(180 * 16))
#define TWOPI_ANG16 ((int32_t)(360 * 16))

#define REAL_16 LE_REAL_I16_FROM_FLOAT(16.0f)

static struct Context
{
    struct leVector_KernelContext base;
    struct leAABB aabb;
    leReal_i16 radius;
    leReal_u8 radiusSquared;
    int32_t start;
    int32_t span;
    leReal_i16 halfWidth;
    leReal_u8 halfWidthSquared;
    enum leStrokeMask mask;
    enum leCapStyle capStyle;
    leReal_i16 minHardnessDistance;
    leReal_u8 maxEdgeDistance;
    leReal_u8 minEdgeDistance;
    struct leAABB startCapScanRect;
    struct leAABB endCapScanRect;
    struct lePlane startCapPlane;
    struct lePlane endCapPlane;
    leBool convex;
    uint32_t hitCount;
    leReal_u8 hitDistance[3];
    leReal_i16 hitRange[3];
} _context;

static struct leVector_Kernel _kernel;

static leReal_i16 _degreeToRadian(int32_t deg)
{
    leReal_i16 res;

    if(deg == 0)
        return 0;

    res = leReal_i16_FromInt(deg);
    res = LE_REAL_I16_MULTIPLY(res, LE_REAL_I16_RADIANS);
    res = LE_REAL_I16_DIVIDE(res, REAL_16);

    return res;
}

#if 0
static void _drawBoundingArea(void)
{
    int32_t r;
    int32_t c;

    for(r = _kernel.scanArea.y;
        r < _kernel.scanArea.y + _kernel.scanArea.height;
        ++r)
    {
        for(c = _kernel.scanArea.x;
            c < _kernel.scanArea.x + _kernel.scanArea.width;
            ++c)
        {
            leRenderer_PutPixel(c, r, 0xFE18);
        }
    }
}
#endif

#if 0
static void _drawHitBoxes(void)
{
    int32_t r;
    int32_t c;
    struct leRect boxArea;

    if(_context.capStyle != LE_CAPSTYLE_NONE)
    {
        leAABB_ToRect(&_context.startCapScanRect, &boxArea);

        for(r = boxArea.y;
            r < boxArea.y + boxArea.height;
            ++r)
        {
            for(c = boxArea.x;
                c < boxArea.x + boxArea.width;
                ++c)
            {
                leRenderer_BlendPixel_Safe(c, r, 0x1F, 0x54);
            }
        }

        leAABB_ToRect(&_context.endCapScanRect, &boxArea);

        for(r = boxArea.y;
            r < boxArea.y + boxArea.height;
            ++r)
        {
            for(c = boxArea.x;
                c < boxArea.x + boxArea.width;
                ++c)
            {
                leRenderer_BlendPixel_Safe(c, r, 0x1F, 0x54);
            }
        }
    }
}
#endif

static void _fillConvexArc(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_i16 planeDist;
    leReal_u8 sqDist;

    (void) krn;

    leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

    planeDist = lePlane_Distance(&_context.startCapPlane,
                                 &_kernel.testPoint);

    if(planeDist >= 0)
    {
        return;
    }

    leVector2_Subtract(&_kernel.testPoint, &_context.endCapScanRect.origin, &vec);

    planeDist = lePlane_Distance(&_context.endCapPlane,
                                 &_kernel.testPoint);

    if(planeDist >= 0)
    {
        return;
    }

    sqDist = leVector2_SquaredLength(&_kernel.testPoint);

    if(_context.mask == LE_STROKEMASK_OUTSIDEONLY)
    {
        if(sqDist >= _context.radiusSquared &&
           sqDist <= _context.maxEdgeDistance)
        {
            _kernel.hitCount += 1;
        }
    }
    else if(sqDist <= _context.maxEdgeDistance &&
            sqDist >= _context.minEdgeDistance)
    {
        if(_context.mask == LE_STROKEMASK_INSIDEONLY &&
           sqDist >= _context.radiusSquared)
        {
            return;
        }

        _kernel.hitCount += 1;
    }
}

static void _fillConcaveArc(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_i16 planeDist;
    leReal_u8 sqDist;
    uint32_t count = 0;

    (void) krn;

    leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

    planeDist = lePlane_Distance(&_context.startCapPlane,
                                 &_kernel.testPoint);

    if(planeDist < 0)
    {
        count += 1;
    }

    leVector2_Subtract(&_kernel.testPoint, &_context.endCapScanRect.origin, &vec);

    planeDist = lePlane_Distance(&_context.endCapPlane,
                                 &_kernel.testPoint);

    if(planeDist < 0)
    {
        count += 1;
    }

    if(count == 0)
    {
        return;
    }

    sqDist = leVector2_SquaredLength(&_kernel.testPoint);

    if(_context.mask == LE_STROKEMASK_OUTSIDEONLY)
    {
        if(sqDist >= _context.radiusSquared &&
           sqDist <= _context.maxEdgeDistance)
        {
            _kernel.hitCount += 1;
        }
    }
    else if(sqDist <= _context.maxEdgeDistance &&
            sqDist >= _context.minEdgeDistance)
    {
        if(_context.mask == LE_STROKEMASK_INSIDEONLY &&
           sqDist >= _context.radiusSquared)
        {
            return;
        }

        _kernel.hitCount += 1;
    }
}

static void _fillConvexArcWithCaps(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_u8 sqDist;
    leBool done = LE_FALSE;
    leReal_i16 startPlaneDist;
    leReal_i16 endPlaneDist;

    (void) krn;

    leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

    startPlaneDist = lePlane_Distance(&_context.startCapPlane,
                                 &_kernel.testPoint);

    leVector2_Subtract(&_kernel.testPoint, &_context.endCapScanRect.origin, &vec);

    endPlaneDist = lePlane_Distance(&_context.endCapPlane,
                                    &_kernel.testPoint);

    if(leAABB_ContainsPoint(&_context.startCapScanRect,
                            &_kernel.testPoint) == LE_TRUE)
    {
        if(startPlaneDist >= 0)
        {
            leVector2_Subtract(&_kernel.testPoint,
                               &_context.startCapScanRect.origin,
                               &vec);

            sqDist = leVector2_SquaredLength(&vec);

            if(sqDist < _context.halfWidthSquared)
            {
                sqDist = leVector2_SquaredLength(&_kernel.testPoint);

                done = LE_TRUE;
            }
            else
            {
                return;
            }
        }
    }

    if(done == LE_FALSE && leAABB_ContainsPoint(&_context.endCapScanRect,
                                                &_kernel.testPoint) == LE_TRUE)
    {
        if(endPlaneDist >= 0)
        {
            leVector2_Subtract(&_kernel.testPoint,
                               &_context.endCapScanRect.origin,
                               &vec);

            sqDist = leVector2_SquaredLength(&vec);

            if(sqDist < _context.halfWidthSquared)
            {
                sqDist = leVector2_SquaredLength(&_kernel.testPoint);

                done = LE_TRUE;
            }
            else
            {
                return;
            }
        }
    }

    if(done == LE_FALSE)
    {
        leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

        if(startPlaneDist >= 0)
        {
            return;
        }

        if(endPlaneDist >= 0)
        {
            return;
        }

        sqDist = leVector2_SquaredLength(&_kernel.testPoint);
    }

    if(_context.mask == LE_STROKEMASK_OUTSIDEONLY)
    {
        if(sqDist >= _context.radiusSquared &&
           sqDist <= _context.maxEdgeDistance)
        {
            _kernel.hitCount += 1;
        }
    }
    else if(sqDist <= _context.maxEdgeDistance &&
            sqDist >= _context.minEdgeDistance)
    {
        if(_context.mask == LE_STROKEMASK_INSIDEONLY &&
           sqDist >= _context.radiusSquared)
        {
            return;
        }

        _kernel.hitCount += 1;
    }
}

static void _fillConcaveArcWithCaps(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_u8 sqDist;
    leBool done = LE_FALSE;
    leReal_i16 startPlaneDist;
    leReal_i16 endPlaneDist;

    (void) krn;

    leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

    startPlaneDist = lePlane_Distance(&_context.startCapPlane,
                                      &_kernel.testPoint);

    leVector2_Subtract(&_kernel.testPoint, &_context.endCapScanRect.origin, &vec);

    endPlaneDist = lePlane_Distance(&_context.endCapPlane,
                                    &_kernel.testPoint);

    if(leAABB_ContainsPoint(&_context.startCapScanRect,
                            &_kernel.testPoint) == LE_TRUE)
    {
        if(startPlaneDist >= 0)
        {
            leVector2_Subtract(&_kernel.testPoint,
                               &_context.startCapScanRect.origin,
                               &vec);

            sqDist = leVector2_SquaredLength(&vec);

            if(sqDist < _context.halfWidthSquared)
            {
                sqDist = leVector2_SquaredLength(&_kernel.testPoint);

                done = LE_TRUE;
            }
        }
    }

    if(done == LE_FALSE && leAABB_ContainsPoint(&_context.endCapScanRect,
                                                &_kernel.testPoint) == LE_TRUE)
    {
        if(endPlaneDist >= 0)
        {
            leVector2_Subtract(&_kernel.testPoint,
                               &_context.endCapScanRect.origin,
                               &vec);

            sqDist = leVector2_SquaredLength(&vec);

            if(sqDist < _context.halfWidthSquared)
            {
                sqDist = leVector2_SquaredLength(&_kernel.testPoint);

                done = LE_TRUE;
            }
        }
    }

    if(done == LE_FALSE)
    {
        leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

        if(startPlaneDist >= 0 && endPlaneDist >= 0)
        {
            return;
        }

        sqDist = leVector2_SquaredLength(&_kernel.testPoint);
    }

    if(_context.mask == LE_STROKEMASK_OUTSIDEONLY)
    {
        if(sqDist >= _context.radiusSquared &&
           sqDist <= _context.maxEdgeDistance)
        {
            _kernel.hitCount += 1;
        }
    }
    else if(sqDist <= _context.maxEdgeDistance &&
            sqDist >= _context.minEdgeDistance)
    {
        if(_context.mask == LE_STROKEMASK_INSIDEONLY &&
           sqDist >= _context.radiusSquared)
        {
            return;
        }

        _kernel.hitCount += 1;
    }
}

static void _shadeConvexArc(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_i16 planeDist;
    leReal_u8 sqDist;

    (void) krn;

    _context.hitCount = 0;

    leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

    planeDist = lePlane_Distance(&_context.startCapPlane,
                                 &_kernel.testPoint);

    if(planeDist >= 0)
    {
        return;
    }

    leVector2_Subtract(&_kernel.testPoint, &_context.endCapScanRect.origin, &vec);

    planeDist = lePlane_Distance(&_context.endCapPlane,
                                 &_kernel.testPoint);

    if(planeDist >= 0)
    {
        return;
    }

    sqDist = leVector2_SquaredLength(&_kernel.testPoint);

    if(_context.mask == LE_STROKEMASK_OUTSIDEONLY)
    {
        if(sqDist >= _context.radiusSquared &&
           sqDist <= _context.maxEdgeDistance)
        {
            _kernel.hitCount += 1;
        }
    }
    else if(sqDist <= _context.maxEdgeDistance &&
            sqDist >= _context.minEdgeDistance)
    {
        if(_context.mask == LE_STROKEMASK_INSIDEONLY &&
           sqDist >= _context.radiusSquared)
        {
            return;
        }

        _kernel.hitCount += 1;
    }

    _context.hitDistance[_context.hitCount] = sqDist;
    _context.hitRange[_context.hitCount] = _context.radius;
    _context.hitCount += 1;
}

static void _shadeConvexArcWithCaps(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_u8 sqDist;
    leReal_i16 startPlaneDist;
    leReal_i16 endPlaneDist;

    (void) krn;

    _context.hitCount = 0;

    sqDist = leVector2_SquaredLength(&_kernel.testPoint);

    if(_context.mask == LE_STROKEMASK_OUTSIDEONLY)
    {
        if(!(sqDist >= _context.radiusSquared &&
             sqDist <= _context.maxEdgeDistance))
        {
            return;
        }
    }
    else if(sqDist <= _context.maxEdgeDistance &&
            sqDist >= _context.minEdgeDistance)
    {
        if(_context.mask == LE_STROKEMASK_INSIDEONLY &&
           sqDist >= _context.radiusSquared)
        {
            return;
        }
    }
    else
    {
        return;
    }

    leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

    startPlaneDist = lePlane_Distance(&_context.startCapPlane,
                                      &_kernel.testPoint);

    leVector2_Subtract(&_kernel.testPoint, &_context.endCapScanRect.origin, &vec);

    endPlaneDist = lePlane_Distance(&_context.endCapPlane,
                                    &_kernel.testPoint);

    if(leAABB_ContainsPoint(&_context.startCapScanRect,
                            &_kernel.testPoint) == LE_TRUE)
    {
        if(startPlaneDist >= 0)
        {
            leVector2_Subtract(&_kernel.testPoint,
                               &_context.startCapScanRect.origin,
                               &vec);

            sqDist = leVector2_SquaredLength(&vec);

            if(sqDist < _context.halfWidthSquared)
            {
                _context.hitDistance[_context.hitCount] = sqDist;
                _context.hitRange[_context.hitCount] = _context.halfWidth;
                _context.hitCount += 1;

                _kernel.hitCount += 1;

                return;
            }
        }
    }

    if(leAABB_ContainsPoint(&_context.endCapScanRect,
                            &_kernel.testPoint) == LE_TRUE)
    {
        if(endPlaneDist >= 0)
        {
            leVector2_Subtract(&_kernel.testPoint,
                               &_context.endCapScanRect.origin,
                               &vec);

            sqDist = leVector2_SquaredLength(&vec);

            if(sqDist < _context.halfWidthSquared)
            {
                _context.hitDistance[_context.hitCount] = sqDist;
                _context.hitRange[_context.hitCount] = _context.halfWidth;
                _context.hitCount += 1;

                _kernel.hitCount += 1;

                return;
            }
        }
    }

    if(startPlaneDist < 0 && endPlaneDist < 0)
    {
        sqDist = leVector2_SquaredLength(&_kernel.testPoint);

        _context.hitDistance[_context.hitCount] = sqDist;
        _context.hitRange[_context.hitCount] = _context.radius;
        _context.hitCount += 1;

        _kernel.hitCount += 1;
    }
}

static void _shadeConcaveArc(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_u8 sqDist;
    leReal_i16 startPlaneDist;
    leReal_i16 endPlaneDist;

    (void) krn;

    _context.hitCount = 0;

    sqDist = leVector2_SquaredLength(&_kernel.testPoint);

    if(_context.mask == LE_STROKEMASK_OUTSIDEONLY)
    {
        if(!(sqDist >= _context.radiusSquared &&
             sqDist <= _context.maxEdgeDistance))
        {
            return;
        }
    }
    else if(sqDist <= _context.maxEdgeDistance &&
            sqDist >= _context.minEdgeDistance)
    {
        if(_context.mask == LE_STROKEMASK_INSIDEONLY &&
           sqDist >= _context.radiusSquared)
        {
            return;
        }
    }
    else
    {
        return;
    }

    leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

    startPlaneDist = lePlane_Distance(&_context.startCapPlane,
                                      &_kernel.testPoint);

    leVector2_Subtract(&_kernel.testPoint, &_context.endCapScanRect.origin, &vec);

    endPlaneDist = lePlane_Distance(&_context.endCapPlane,
                                    &_kernel.testPoint);

    if(startPlaneDist < 0 || endPlaneDist < 0)
    {
        sqDist = leVector2_SquaredLength(&_kernel.testPoint);

        _context.hitDistance[_context.hitCount] = sqDist;
        _context.hitRange[_context.hitCount] = _context.radius;
        _context.hitCount += 1;

        _kernel.hitCount += 1;
    }
}

static void _shadeConcaveArcWithCaps(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_u8 sqDist;
    leReal_i16 startPlaneDist;
    leReal_i16 endPlaneDist;

    (void) krn;

    _context.hitCount = 0;

    sqDist = leVector2_SquaredLength(&_kernel.testPoint);

    if(_context.mask == LE_STROKEMASK_OUTSIDEONLY)
    {
        if(!(sqDist >= _context.radiusSquared &&
             sqDist <= _context.maxEdgeDistance))
        {
            return;
        }
    }
    else if(sqDist <= _context.maxEdgeDistance &&
            sqDist >= _context.minEdgeDistance)
    {
        if(_context.mask == LE_STROKEMASK_INSIDEONLY &&
           sqDist >= _context.radiusSquared)
        {
            return;
        }
    }
    else
    {
        return;
    }

    leVector2_Subtract(&_kernel.testPoint, &_context.startCapScanRect.origin, &vec);

    startPlaneDist = lePlane_Distance(&_context.startCapPlane,
                                      &_kernel.testPoint);

    leVector2_Subtract(&_kernel.testPoint, &_context.endCapScanRect.origin, &vec);

    endPlaneDist = lePlane_Distance(&_context.endCapPlane,
                                    &_kernel.testPoint);

    if(leAABB_ContainsPoint(&_context.startCapScanRect,
                            &_kernel.testPoint) == LE_TRUE)
    {
        if(startPlaneDist >= 0)
        {
            leVector2_Subtract(&_kernel.testPoint,
                               &_context.startCapScanRect.origin,
                               &vec);

            sqDist = leVector2_SquaredLength(&vec);

            if(sqDist < _context.halfWidthSquared)
            {
                _context.hitDistance[_context.hitCount] = sqDist;
                _context.hitRange[_context.hitCount] = _context.halfWidth;
                _context.hitCount += 1;
            }
        }
    }

    if(leAABB_ContainsPoint(&_context.endCapScanRect,
                            &_kernel.testPoint) == LE_TRUE)
    {
        if(endPlaneDist >= 0)
        {
            leVector2_Subtract(&_kernel.testPoint,
                               &_context.endCapScanRect.origin,
                               &vec);

            sqDist = leVector2_SquaredLength(&vec);

            if(sqDist < _context.halfWidthSquared)
            {
                _context.hitDistance[_context.hitCount] = sqDist;
                _context.hitRange[_context.hitCount] = _context.halfWidth;
                _context.hitCount += 1;
            }
        }
    }

    if(startPlaneDist < 0 || endPlaneDist < 0)
    {
        sqDist = leVector2_SquaredLength(&_kernel.testPoint);

        _context.hitDistance[_context.hitCount] = sqDist;
        _context.hitRange[_context.hitCount] = _context.radius;
        _context.hitCount += 1;
    }

    if(_context.hitCount > 0)
    {
        _kernel.hitCount += 1;
    }
}

static void _adjustAngles(void)
{
    if(_context.span < -TWOPI_ANG16 ||
       _context.span > TWOPI_ANG16)
    {
        _context.span = TWOPI_ANG16;
    }

    if(_context.span < 0)
    {
        _context.start += _context.span;
        _context.span = -_context.span;
    }

    while(_context.start < 0)
    {
        _context.start += TWOPI_ANG16;
    }

    while(_context.start > TWOPI_ANG16)
    {
        _context.start -= TWOPI_ANG16;
    }

    if(_context.span > PI_ANG16)
    {
        _context.convex = LE_FALSE;
    }
}

static void _calculateScanArea(void)
{
    _context.aabb.origin.x = _kernel.origin.x;
    _context.aabb.origin.y = _kernel.origin.y;
    _context.aabb.extents.x = _context.radius + _context.halfWidth;
    _context.aabb.extents.y = _context.aabb.extents.x;

    if(_context.capStyle == LE_CAPSTYLE_ROUND)
    {
        leAABB_Combine(&_context.aabb, &_context.startCapScanRect, &_context.aabb);
    }

    leAABB_ToRect(&_context.aabb, &_kernel.scanArea);
}

static uint8_t _shade_Gradient(struct leVector_Kernel* krn)
{
    (void)krn;

    leReal_i16 delta;
    leReal_i16 hardness;
    leReal_i16 dist;

    delta = 0;

    for(uint32_t idx = 0; idx < _context.hitCount; ++idx)
    {
        dist = LE_REAL_U8_TO_I16(leReal_u8_SquareRoot(_context.hitDistance[idx]));

        if(_context.hitRange[idx] == _context.radius)
        {
            if(dist >= _context.hitRange[idx])
            {
                dist -= _context.hitRange[idx];
            }
            else
            {
                dist = _context.hitRange[idx] - dist;
            }
        }

        if(dist > _context.minHardnessDistance)
        {
            hardness = _context.halfWidth - _context.minHardnessDistance;
            dist -= _context.halfWidth - hardness;
            hardness = LE_REAL_I16_ONE - LE_REAL_I16_DIVIDE(dist, hardness);

            if(hardness < 0)
            {
                hardness = 0;
            }

            delta += hardness;
        }
        else
        {
            return 255;
        }
    }

    if(delta < 0)
    {
        return 0;
    }
    else if(delta > LE_REAL_I16_ONE)
    {
        return 255;
    }

    delta = LE_REAL_I16_MULTIPLY(delta, LE_REAL_I16_255);
    delta = leReal_i16_Round(delta);

    return (uint8_t)(leReal_i16_ToInt(delta));
}

static void _calculateStartCapRect(void)
{
    leReal_i16 rad;
    struct leVector2 normal;

    rad = _degreeToRadian(_context.start);

    _context.startCapScanRect.origin.x = _context.radius;
    _context.startCapScanRect.origin.y = 0;

    leVector2_Rotate(&_context.startCapScanRect.origin,
                     -rad,
                     &_context.startCapScanRect.origin);

    normal.x = _context.startCapScanRect.origin.x;
    normal.y = _context.startCapScanRect.origin.y;

    leVector2_Normalize(&normal, &normal);

    leVector2_Rotate(&normal, LE_REAL_I16_PI_OVER_TWO, &normal);

    lePlane_Create(&_context.startCapScanRect.origin,
                   &normal,
                   &_context.startCapPlane);

    _context.startCapScanRect.extents.x = _context.halfWidth;
    _context.startCapScanRect.extents.y = _context.halfWidth;

    _context.startCapScanRect.origin.x += _kernel.origin.x;
    _context.startCapScanRect.origin.y += _kernel.origin.y;
}

static void _calculateEndCapRect(void)
{
    leReal_i16 rad;
    struct leVector2 normal;

    rad = _degreeToRadian(_context.start + _context.span);

    _context.endCapScanRect.origin.x = _context.radius;
    _context.endCapScanRect.origin.y = 0;

    leVector2_Rotate(&_context.endCapScanRect.origin,
                     -rad,
                     &_context.endCapScanRect.origin);

    normal.x = _context.endCapScanRect.origin.x;
    normal.y = _context.endCapScanRect.origin.y;

    leVector2_Normalize(&normal, &normal);

    leVector2_Rotate(&normal, -LE_REAL_I16_PI_OVER_TWO, &normal);

    lePlane_Create(&_context.endCapScanRect.origin,
                   &normal,
                   &_context.endCapPlane);

    _context.endCapScanRect.extents.x = _context.halfWidth;
    _context.endCapScanRect.extents.y = _context.halfWidth;

    _context.endCapScanRect.origin.x += _kernel.origin.x;
    _context.endCapScanRect.origin.y += _kernel.origin.y;
}

void leDraw_VectorArcStroke(const struct leVector2* org,
                            leReal_i16 rad,
                            int32_t start,
                            int32_t span,
                            const struct leVectorArc_StrokeAttr* attr)
{
    struct leRect rect;
    leReal_i16 width;
    leReal_i16 hardness;

    memset(&_context, 0, sizeof(struct Context));

    _kernel.origin = *org;
    _context.radius = rad;
    _context.radiusSquared = leReal_i16_Square(rad);
    _context.start = start;
    _context.span = span;
    _context.convex = LE_TRUE;

    if(span == 0)
        return;

    width = attr->width;

    if(width <= 0)
    {
        width = LE_REAL_I16_FROM_FLOAT(0.5f);
    }

    _context.halfWidth = LE_REAL_I16_DIVIDE(width, LE_REAL_I16_TWO);

    if(attr->mask != LE_STROKEMASK_OUTSIDEONLY)
    {
        if(_context.halfWidth > rad)
        {
            _context.halfWidth = rad;
        }
    }

    _context.halfWidthSquared = leReal_i16_Square(_context.halfWidth);

    _context.minEdgeDistance = LE_REAL_U8_SQUARE(LE_REAL_I16_TO_U8(_context.radius - _context.halfWidth));
    _context.maxEdgeDistance = LE_REAL_U8_SQUARE(LE_REAL_I16_TO_U8(_context.radius + _context.halfWidth));

    _context.base.color = attr->color;
    _context.base.alpha = LE_REAL_I16_DIVIDE(LE_REAL_I16_FROM_INT(attr->alpha), LE_REAL_I16_255);
    //_context.aaMode = attr->aaMode;
    _context.capStyle = attr->capStyle;
    _context.mask = attr->mask;
    hardness = attr->hardness;

    if(attr->hardness < 0)
    {
        hardness = 0;
    }
    else if(attr->hardness > LE_REAL_I16_ONE)
    {
        hardness = LE_REAL_I16_ONE;
    }

    _context.minHardnessDistance = LE_REAL_I16_MULTIPLY(hardness, _context.halfWidth);

    _adjustAngles();

    _calculateStartCapRect();
    _calculateEndCapRect();

    // calculate tight bounding box
    _calculateScanArea();

    leRenderer_GetFrameRect(&rect);

    if(leRectIntersects(&_kernel.scanArea, &rect) == LE_FALSE)
        return;

    leRectClip(&_kernel.scanArea, &rect, &_kernel.scanArea);

    leRenderer_GetClipRect(&rect);

    if(leRectIntersects(&_kernel.scanArea, &rect) == LE_FALSE)
        return;

    leRectClip(&_kernel.scanArea, &rect, &_kernel.scanArea);

    if(hardness == LE_REAL_I16_ONE)
    {
        _context.base.aaMode = attr->aaMode;

        if(attr->capStyle == LE_CAPSTYLE_ROUND)
        {
            if(_context.convex == LE_TRUE)
            {
                _kernel.evaluatePoint = _fillConvexArcWithCaps;
            }
            else
            {
                _kernel.evaluatePoint = _fillConcaveArcWithCaps;
            }
        }
        else
        {
            if(_context.convex == LE_TRUE)
            {
                _kernel.evaluatePoint = _fillConvexArc;
            }
            else
            {
                _kernel.evaluatePoint = _fillConcaveArc;
            }
        }
    }
    else
    {
        _context.base.aaMode = LE_ANTIALIASING_NONE;
        _kernel.shadeFragment = _shade_Gradient;

        if(attr->capStyle == LE_CAPSTYLE_ROUND)
        {
            if(_context.convex == LE_TRUE)
            {
                _kernel.evaluatePoint = _shadeConvexArcWithCaps;
            }
            else
            {
                _kernel.evaluatePoint = _shadeConcaveArcWithCaps;
            }
        }
        else
        {
            if(_context.convex == LE_TRUE)
            {
                _kernel.evaluatePoint = _shadeConvexArc;
            }
            else
            {
                _kernel.evaluatePoint = _shadeConcaveArc;
            }
        }
    }

    if(leReal_i16_AbsoluteValue(_context.span) > 180 * 16)
    {
        _context.convex = LE_FALSE;
    }

    //_drawBoundingArea();

    //_drawHitBoxes();

    if(_context.capStyle == LE_CAPSTYLE_ROUND)
    {
        _context.startCapScanRect.origin.x -= _kernel.origin.x;
        _context.startCapScanRect.origin.y -= _kernel.origin.y;
        _context.endCapScanRect.origin.x -= _kernel.origin.x;
        _context.endCapScanRect.origin.y -= _kernel.origin.y;
    }

    _kernel.context = (struct leVector_KernelContext*)&_context;

    leVector_Kernel_Exec(&_kernel);
}