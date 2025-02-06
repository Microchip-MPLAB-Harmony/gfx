#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/vector/legato_vector_kernel.h"

static struct Context
{
    struct leVector_KernelContext base;
    struct leAABB aabb;
    struct leLineF line;
    struct leLineF actualLine;
    struct lePlane leftTopPlane;
    struct lePlane rightTopPlane;
    struct lePlane leftBottomPlane;
    struct lePlane rightBottomPlane;
    leReal_i16 maxEdgeDistance;
    leReal_u8 maxEdgeDistanceSquared;
    leReal_i16 minHardnessDistance;
    leReal_i16 shadePrecision;
    leReal_u8 distance;
} _context;

static struct leVector_Kernel _kernel;

#define NORMAL_ANGLE 0xB504

static void _drawHardLine(const struct leVector2* pnt,
                          leReal_i16 w)
{
    struct lePoint drawPnt;
    int32_t x, iw;

    x = leReal_i16_ToInt(pnt->x);

    drawPnt.y = leReal_i16_ToInt(pnt->y);
    iw = leReal_i16_ToInt(w);

    if(_context.base.alpha == LE_REAL_I16_ONE)
    {
        for(drawPnt.x = x; drawPnt.x < x + iw; ++drawPnt.x)
        {
            leRenderer_PutPixel_Safe(drawPnt.x,
                                     drawPnt.y,
                                     _context.base.color);
        }
    }
    else
    {
        for(drawPnt.x = x; drawPnt.x < x + iw; ++drawPnt.x)
        {
            leRenderer_BlendPixel_Safe(drawPnt.x,
                                       drawPnt.y,
                                       _context.base.color,
                                       _context.base.alpha);
        }
    }
}

static void _testPointSq(struct leVector_Kernel* krn)
{
    if(krn->testPoint.x >= -_context.aabb.extents.x &&
       krn->testPoint.x <= _context.aabb.extents.x &&
       krn->testPoint.y >= -_context.aabb.extents.y &&
       krn->testPoint.y <= _context.aabb.extents.y)
    {
        krn->hitCount += 1;
    }
}

static void _testPointRd(struct leVector_Kernel* krn)
{
    struct leVector2 vec;

    if(leReal_i16_AbsoluteValue(krn->testPoint.x) > _context.aabb.extents.x)
        return;

    if(krn->testPoint.x < _context.line.p0.x)
    {
        leVector2_Subtract(&_context.line.p0, &krn->testPoint, &vec);

        if(leVector2_SquaredLength(&vec) < _context.maxEdgeDistanceSquared)
        {
            _kernel.hitCount += 1;
        }
    }
    else if(krn->testPoint.x > _context.line.p1.x)
    {
        leVector2_Subtract(&_context.line.p1, &krn->testPoint, &vec);

        if(leVector2_SquaredLength(&vec) < _context.maxEdgeDistanceSquared)
        {
            _kernel.hitCount += 1;
        }
    }
    else
    {
        if(leReal_i16_AbsoluteValue(krn->testPoint.y) <= _context.maxEdgeDistance)
        {
            _kernel.hitCount += 1;
        }
    }
}

static uint8_t _shade_CapNo_Gradient(struct leVector_Kernel* krn)
{
    leReal_i16 distance;
    leReal_i16 delta;
    leReal_i16 hardness;

    distance = leReal_i16_AbsoluteValue(krn->testPoint.y);
    delta = LE_REAL_I16_ONE;

    if(distance >= _context.minHardnessDistance)
    {
        hardness = (_context.maxEdgeDistance - _context.minHardnessDistance);
        hardness = LE_REAL_I16_ONE - LE_REAL_I16_DIVIDE((distance - _context.minHardnessDistance), hardness);

        if(hardness < 0)
        {
            hardness = 0;
        }

        delta = LE_REAL_I16_MULTIPLY(delta, hardness);
    }

    delta = LE_REAL_I16_MULTIPLY(delta, LE_REAL_I16_255);
    delta = leReal_i16_Round(delta);

    return (uint8_t)(leReal_i16_ToInt(delta));
}

static uint8_t _shade_CapSq_Gradient(struct leVector_Kernel* krn)
{
    leReal_i16 distance;
    leReal_i16 delta;
    leReal_i16 hardness;

    if(krn->testPoint.x < _context.line.p0.x)
    {
        if(lePlane_Distance(&_context.leftTopPlane, &krn->testPoint) > 0 ||
           lePlane_Distance(&_context.leftBottomPlane, &krn->testPoint) > 0)
        {
            distance = leReal_i16_AbsoluteValue(krn->testPoint.y);
        }
        else
        {
            distance = leReal_i16_AbsoluteValue(_context.line.p0.x - krn->testPoint.x);
        }
    }
    else if(krn->testPoint.x > _context.line.p1.x)
    {
        if(lePlane_Distance(&_context.rightTopPlane, &krn->testPoint) > 0 ||
           lePlane_Distance(&_context.rightBottomPlane, &krn->testPoint) > 0)
        {
            distance = leReal_i16_AbsoluteValue(krn->testPoint.y);
        }
        else
        {
            distance = leReal_i16_AbsoluteValue(krn->testPoint.x - _context.line.p1.x);
        }
    }
    else
    {
        distance = leReal_i16_AbsoluteValue(krn->testPoint.y);
    }

    delta = LE_REAL_I16_ONE;

    if(distance >= _context.minHardnessDistance)
    {
        hardness = (_context.maxEdgeDistance - _context.minHardnessDistance);
        hardness = LE_REAL_I16_ONE - LE_REAL_I16_DIVIDE((distance - _context.minHardnessDistance), hardness);

        if(hardness < 0)
        {
            hardness = 0;
        }

        delta = LE_REAL_I16_MULTIPLY(delta, hardness);
    }

    delta = LE_REAL_I16_MULTIPLY(delta, LE_REAL_I16_255);
    delta = leReal_i16_Round(delta);

    return (uint8_t)(leReal_i16_ToInt(delta));
}

static uint8_t _shade_CapRd_Gradient(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_u8 sqDistance;
    leReal_i16 distance;
    leReal_i16 delta;
    leReal_i16 hardness;
    leReal_i16 t;

    if(krn->testPoint.x < _context.line.p0.x)
    {
        leVector2_Subtract(&_context.line.p0, &krn->testPoint, &vec);

        sqDistance = leVector2_SquaredLength(&vec);

        if(sqDistance > _context.maxEdgeDistanceSquared)
            return 0;

        distance = LE_REAL_U8_TO_I16(leReal_u8_SquareRoot(sqDistance));
    }
    else if(krn->testPoint.x > _context.line.p1.x)
    {
        leVector2_Subtract(&_context.line.p1, &krn->testPoint, &vec);

        sqDistance = leVector2_SquaredLength(&vec);

        if(sqDistance > _context.maxEdgeDistanceSquared)
            return 0;

        distance = LE_REAL_U8_TO_I16(leReal_u8_SquareRoot(sqDistance));
    }
    else
    {
        distance = leReal_i16_AbsoluteValue(krn->testPoint.y);
    }

    delta = LE_REAL_I16_ONE;

    if(distance >= _context.minHardnessDistance)
    {
        hardness = _context.maxEdgeDistance - _context.minHardnessDistance;
        t = distance - _context.minHardnessDistance;
        hardness = LE_REAL_I16_DIVIDE(t, hardness);
        hardness = LE_REAL_I16_ONE - hardness;

        if(hardness < 0)
        {
            hardness = 0;
        }

        delta = LE_REAL_I16_MULTIPLY(delta, hardness);
    }

    delta = LE_REAL_I16_MULTIPLY(delta, LE_REAL_I16_255);
    delta = leReal_i16_Round(delta);

    return (uint8_t)(leReal_i16_ToInt(delta));
}

void leDraw_VectorHLine(const struct leVector2* pnt,
                        leReal_i16 w,
                        const struct leVectorLineAttr* attr)
{
    struct leVector2 normal;
    struct leRect rect;
    leReal_i16 width;
    leReal_i16 hardness;

    memset(&_context, 0, sizeof(struct Context));

    if(attr->width == 0)
    {
        _drawHardLine(pnt, w);

        return;
    }

    _context.base.color = attr->color;
    _context.base.alpha = LE_REAL_I16_DIVIDE(LE_REAL_I16_FROM_INT(attr->alpha), LE_REAL_I16_255);
    _context.base.aaMode = attr->aaMode;

    hardness = attr->hardness;

    if(hardness < 0)
    {
        hardness = 0;
    }
    else if(hardness > LE_REAL_I16_ONE)
    {
        hardness = LE_REAL_I16_ONE;
    }

    width = attr->width;

    if(width < 0)
    {
        width = LE_REAL_I16_FROM_FLOAT(0.5f);
    }

    _context.maxEdgeDistance = LE_REAL_I16_MULTIPLY(attr->width, LE_REAL_I16_ONE_OVER_TWO);
    _context.minHardnessDistance = LE_REAL_I16_MULTIPLY(hardness, _context.maxEdgeDistance);

    _context.line.p0 = *pnt;
    _context.line.p1.x = _context.line.p0.x + w;
    _context.line.p1.y = pnt->y;

    _context.actualLine = _context.line;

    if(attr->capStyle != LE_CAPSTYLE_NONE)
    {
        _context.actualLine.p0.x -= _context.maxEdgeDistance;
        _context.actualLine.p1.x += _context.maxEdgeDistance;
    }

    _context.aabb.extents.x = leReal_i16_AbsoluteValue(_context.actualLine.p1.x - _context.actualLine.p0.x);
    _context.aabb.extents.x = LE_REAL_I16_MULTIPLY(_context.aabb.extents.x, LE_REAL_I16_ONE_OVER_TWO);
    _context.aabb.extents.y = _context.maxEdgeDistance;
    _context.aabb.origin.x = _context.actualLine.p0.x + _context.aabb.extents.x;
    _context.aabb.origin.y = _context.actualLine.p0.y;

    leAABB_ToRect(&_context.aabb, &_kernel.scanArea);
    _kernel.scanArea.width += 1;
    _kernel.scanArea.height += 1;

    leRenderer_GetFrameRect(&rect);

    if(leRectIntersects(&_kernel.scanArea, &rect) == LE_FALSE)
        return;

    leRectClip(&_kernel.scanArea, &rect, &_kernel.scanArea);

    leRenderer_GetClipRect(&rect);

    if(leRectIntersects(&_kernel.scanArea, &rect) == LE_FALSE)
        return;

    leRectClip(&_kernel.scanArea, &rect, &_kernel.scanArea);

    leVector2_Subtract(&_context.line.p0,
                       &_context.aabb.origin,
                       &_context.line.p0);

    leVector2_Subtract(&_context.line.p1,
                       &_context.aabb.origin,
                       &_context.line.p1);

    _context.actualLine.p0.x = 0;
    _context.actualLine.p0.y = -_context.aabb.extents.x;
    _context.actualLine.p1.x = 0;
    _context.actualLine.p1.y = _context.aabb.extents.x;

    _kernel.origin = _context.aabb.origin;

    if(attr->capStyle == LE_CAPSTYLE_ROUND)
    {
        _context.maxEdgeDistanceSquared = leReal_i16_Square(_context.maxEdgeDistance);

        _kernel.evaluatePoint = _testPointRd;

        if(attr->hardness < LE_REAL_I16_ONE)
        {
            _kernel.shadeFragment = _shade_CapRd_Gradient;
        }
    }
    else if(attr->capStyle == LE_CAPSTYLE_SQUARE)
    {
        normal.x = NORMAL_ANGLE;
        normal.y = -NORMAL_ANGLE;

        lePlane_Create(&_context.line.p0, &normal, &_context.leftTopPlane);

        normal.x = NORMAL_ANGLE;
        normal.y = NORMAL_ANGLE;

        lePlane_Create(&_context.line.p0, &normal, &_context.leftBottomPlane);

        normal.x = -NORMAL_ANGLE;
        normal.y = -NORMAL_ANGLE;

        lePlane_Create(&_context.line.p1, &normal, &_context.rightTopPlane);

        normal.x = -NORMAL_ANGLE;
        normal.y = NORMAL_ANGLE;

        lePlane_Create(&_context.line.p1, &normal, &_context.rightBottomPlane);

        _kernel.evaluatePoint = _testPointSq;

        if(attr->hardness < LE_REAL_I16_ONE)
        {
            _kernel.shadeFragment = _shade_CapSq_Gradient;
        }
    }
    else
    {
        _kernel.evaluatePoint = _testPointSq;

        if(attr->hardness < LE_REAL_I16_ONE)
        {
            _kernel.shadeFragment = _shade_CapNo_Gradient;
        }
    }

    _kernel.context = (struct leVector_KernelContext*)&_context;

    leVector_Kernel_Exec(&_kernel);
}