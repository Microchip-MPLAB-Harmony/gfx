#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/vector/legato_vector_kernel.h"

static struct Context
{
    struct leVector_KernelContext base;
    struct leAABB aabb;
    struct leOBB obb;
    struct leVector2 rotatedPoint;
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
} _context;

static struct leVector_Kernel _kernel;

#define NORMAL_ANGLE 0xB504

static void _drawHardLine(const struct leVector2* pnt0,
                          const struct leVector2* pnt1)
{
    int32_t dx, sx, dy, sy, e2, err;
    struct lePoint p0;
    struct lePoint p1;
    struct lePoint drawPnt;

    leVector2_ToPoint(pnt0, &p0);
    leVector2_ToPoint(pnt1, &p1);

    dx = (int32_t)p1.x - (int32_t)p0.x;

    if(dx < 0)
        dx *= -1;

    sx = p0.x < p1.x ? 1 : -1;

    dy = (int32_t)p1.y - (int32_t)p0.y;

    if(dy < 0)
        dy *= -1;

    dy *= -1;

    sy = p0.y < p1.y ? 1 : -1;
    err = dx + dy;

    for(;;)
    {
        drawPnt.x = p0.x;
        drawPnt.y = p0.y;

        leRenderer_BlendPixel_Safe(drawPnt.x,
                                   drawPnt.y,
                                   _context.base.color,
                                   _context.base.alpha);

        if(p0.x == p1.x && p0.y == p1.y)
            break;

        e2 = 2 * err;

        if(e2 >= dy)
        {
            err += dy;
            p0.x += sx;
        }

        if(e2 <= dx)
        {
            err += dx;
            p0.y += sy;
        }
    }
}

static void _testPointSq(struct leVector_Kernel* krn)
{
    leVector2_Rotate(&krn->testPoint,
                     -_context.obb.angle,
                     &_context.rotatedPoint);

    if(_context.rotatedPoint.x >= -_context.obb.extents.x &&
       _context.rotatedPoint.x <= _context.obb.extents.x &&
       _context.rotatedPoint.y >= -_context.obb.extents.y &&
       _context.rotatedPoint.y <= _context.obb.extents.y)
    {
        krn->hitCount += 1;
    }
}

static void _testPointRd(struct leVector_Kernel* krn)
{
    struct leVector2 vec;

    leVector2_Rotate(&krn->testPoint,
                     -_context.obb.angle,
                     &_context.rotatedPoint);

    if(leReal_i16_AbsoluteValue(_context.rotatedPoint.x) > _context.obb.extents.x)
        return;

    if(_context.rotatedPoint.x < _context.line.p0.x)
    {
        leVector2_Subtract(&_context.line.p0, &_context.rotatedPoint, &vec);

        if(leVector2_SquaredLength(&vec) < _context.maxEdgeDistanceSquared)
        {
            _kernel.hitCount += 1;
        }
    }
    else if(_context.rotatedPoint.x > _context.line.p1.x)
    {
        leVector2_Subtract(&_context.line.p1, &_context.rotatedPoint, &vec);

        if(leVector2_SquaredLength(&vec) < _context.maxEdgeDistanceSquared)
        {
            _kernel.hitCount += 1;
        }
    }
    else
    {
        if(leReal_i16_AbsoluteValue(_context.rotatedPoint.y) <= _context.maxEdgeDistance)
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

    (void)krn;

    distance = leReal_i16_AbsoluteValue(_context.rotatedPoint.y);
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

    (void)krn;

    if(_context.rotatedPoint.x < _context.line.p0.x)
    {
        if(lePlane_Distance(&_context.leftTopPlane, &_context.rotatedPoint) > 0 ||
           lePlane_Distance(&_context.leftBottomPlane, &_context.rotatedPoint) > 0)
        {
            distance = leReal_i16_AbsoluteValue(_context.rotatedPoint.y);
        }
        else
        {
            distance = leReal_i16_AbsoluteValue(_context.line.p0.x - _context.rotatedPoint.x);
        }
    }
    else if(_context.rotatedPoint.x > _context.line.p1.x)
    {
        if(lePlane_Distance(&_context.rightTopPlane, &_context.rotatedPoint) > 0 ||
           lePlane_Distance(&_context.rightBottomPlane, &_context.rotatedPoint) > 0)
        {
            distance = leReal_i16_AbsoluteValue(_context.rotatedPoint.y);
        }
        else
        {
            distance = leReal_i16_AbsoluteValue(_context.rotatedPoint.x - _context.line.p1.x);
        }
    }
    else
    {
        distance = leReal_i16_AbsoluteValue(_context.rotatedPoint.y);
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

    (void)krn;

    if(_context.rotatedPoint.x < _context.line.p0.x)
    {
        leVector2_Subtract(&_context.line.p0, &_context.rotatedPoint, &vec);

        sqDistance = leVector2_SquaredLength(&vec);

        if(sqDistance > _context.maxEdgeDistanceSquared)
            return 0;

        distance = LE_REAL_U8_TO_I16(leReal_u8_SquareRoot(sqDistance));
    }
    else if(_context.rotatedPoint.x > _context.line.p1.x)
    {
        leVector2_Subtract(&_context.line.p1, &_context.rotatedPoint, &vec);

        sqDistance = leVector2_SquaredLength(&vec);

        if(sqDistance > _context.maxEdgeDistanceSquared)
            return 0;

        distance = LE_REAL_U8_TO_I16(leReal_u8_SquareRoot(sqDistance));
    }
    else
    {
        distance = leReal_i16_AbsoluteValue(_context.rotatedPoint.y);
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

static void _drawLine(const struct leVector2* p0,
                      const struct leVector2* p1,
                      const struct leVectorLineAttr* attr)
{
    struct leVector2 normal;
    struct leVector2 lineVector;
    struct leRect rect;
    leReal_i16 hardness;

    memset(&_context, 0, sizeof(struct Context));

    if(attr->width == 0)
    {
        _drawHardLine(p0, p1);

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

    _context.maxEdgeDistance = LE_REAL_I16_MULTIPLY(attr->width, LE_REAL_I16_ONE_OVER_TWO);
    _context.minHardnessDistance = LE_REAL_I16_MULTIPLY(hardness, _context.maxEdgeDistance);

    _context.line.p0 = *p0;
    _context.line.p1 = *p1;

    _context.actualLine = _context.line;

    if(attr->capStyle != LE_CAPSTYLE_NONE)
    {
        leVector2_Subtract(&_context.line.p1, &_context.line.p0, &lineVector);

        leVector2_Normalize(&lineVector, &lineVector);

        leVector2_TranslateAlongVector(&_context.actualLine.p0,
                                       &lineVector,
                                       -_context.maxEdgeDistance,
                                       &_context.actualLine.p0);

        leVector2_Negate(&lineVector, &lineVector);

        leVector2_TranslateAlongVector(&_context.actualLine.p1,
                                       &lineVector,
                                       -_context.maxEdgeDistance,
                                       &_context.actualLine.p1);
    }

    leOBB_FromLine(&_context.actualLine,
                   attr->width,
                   &_context.obb);

    leOBB_AABB(&_context.obb, &_context.aabb);

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

    leVector2_Subtract(&_context.line.p1, &_context.line.p0, &_context.line.p1);
    _context.line.p0 = LE_VECTOR2_ZERO;
    leVector2_Rotate(&_context.line.p1, -_context.obb.angle, &_context.line.p1);
    _context.line.p0.x -= LE_REAL_I16_MULTIPLY(_context.line.p1.x, LE_REAL_I16_ONE_OVER_TWO);
    _context.line.p1.x += _context.line.p0.x;
    _context.line.p1.y = 0;

    _context.actualLine.p0.x = -_context.obb.extents.x;
    _context.actualLine.p0.y = 0;
    _context.actualLine.p1.x = _context.obb.extents.x;
    _context.actualLine.p1.y = 0;

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

void leDraw_VectorLine(const struct leVector2* p0,
                        const struct leVector2* p1,
                        const struct leVectorLineAttr* attr)
{
    // use horizontal line draw if possible
    if(LE_REAL_I16_EQUALS(p0->y, p1->y))
    {
        if(p0->x < p1->x)
        {
            leDraw_VectorHLine(p0,
                               (p1->x - p0->x) + LE_REAL_I16_ONE,
                               attr);
        }
        else
        {
            leDraw_VectorHLine(p1,
                               (p0->x - p1->x) + LE_REAL_I16_ONE,
                               attr);
        }

        return;
    }

    // use vertial line draw if possible
    if(LE_REAL_I16_EQUALS(p0->x, p1->x))
    {
        if(p0->y < p1->y)
        {
            leDraw_VectorVLine(p0,
                               (p1->y - p0->y) + LE_REAL_I16_ONE,
                               attr);
        }
        else
        {
            leDraw_VectorVLine(p1,
                               (p0->y - p1->y) + LE_REAL_I16_ONE,
                                attr);
        }

        return;
    }

    _drawLine(p0, p1, attr);
}