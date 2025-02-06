#include "gfx/legato/vector/legato_vector_kernel.h"
#include "gfx/legato/renderer/legato_renderer.h"

struct ComplexScanArea
{
    struct leVector2 origin;
    leReal_i16 radius;
    leReal_u8 radiusSquared;
    leReal_u8 maxRadiusSquared;
    leReal_u8 minRadiusSquared;
};

static struct Context
{
    struct leVector_KernelContext base;
    leReal_i16 width;
    leReal_i16 halfWidth;
    enum leStrokeMask mask;
    struct leRectF rect;
    struct leRectF minRect;
    struct leRectF maxRect;
    leReal_i16 rotation;
    struct leVector2 testPoint;
    struct ComplexScanArea topLeftArea;
    struct ComplexScanArea topRightArea;
    struct ComplexScanArea bottomRightArea;
    struct ComplexScanArea bottomLeftArea;
    leReal_i16 maxEdgeDistance;
    leReal_i16 minHardnessDistance;
} _context;

static struct leVector_Kernel _kernel;

static void _clampCorners(void)
{
    leReal_i16 halfWidth;
    leReal_i16 halfHeight;
    leReal_i16 t;

    halfWidth = LE_REAL_I16_MULTIPLY(_context.rect.extents.x, LE_REAL_I16_ONE_OVER_TWO);
    halfHeight = LE_REAL_I16_MULTIPLY(_context.rect.extents.y, LE_REAL_I16_ONE_OVER_TWO);

    t = leReal_i16_Min(halfWidth, halfHeight);

    if(_context.topLeftArea.radius > t)
    {
        _context.topLeftArea.radius = t;
    }

    if(_context.topRightArea.radius > t)
    {
        _context.topRightArea.radius = t;
    }

    if(_context.bottomRightArea.radius > t)
    {
        _context.bottomRightArea.radius = t;
    }

    if(_context.bottomLeftArea.radius > t)
    {
        _context.bottomLeftArea.radius = t;
    }
}

static void _setCornerOrigins(void)
{
    if(_context.topLeftArea.radius > 0)
    {
        _context.topLeftArea.origin.x = -_context.rect.extents.x + _context.topLeftArea.radius;
        _context.topLeftArea.origin.y = -_context.rect.extents.y + _context.topLeftArea.radius;

        _context.topLeftArea.radiusSquared = leReal_i16_Square(_context.topLeftArea.radius);
        _context.topLeftArea.maxRadiusSquared = leReal_i16_Square(_context.topLeftArea.radius + _context.halfWidth);
        _context.topLeftArea.minRadiusSquared = leReal_i16_Square(_context.topLeftArea.radius - _context.halfWidth);
    }

    if(_context.topRightArea.radius > 0)
    {
        _context.topRightArea.origin.x = _context.rect.extents.x - _context.topRightArea.radius;
        _context.topRightArea.origin.y = -_context.rect.extents.y + _context.topRightArea.radius;

        _context.topRightArea.radiusSquared = leReal_i16_Square(_context.topRightArea.radius);
        _context.topRightArea.maxRadiusSquared = leReal_i16_Square(_context.topRightArea.radius + _context.halfWidth);
        _context.topRightArea.minRadiusSquared = leReal_i16_Square(_context.topRightArea.radius - _context.halfWidth);
    }

    if(_context.bottomRightArea.radius > 0)
    {
        _context.bottomRightArea.origin.x = _context.rect.extents.x - _context.bottomRightArea.radius;
        _context.bottomRightArea.origin.y = _context.rect.extents.y - _context.bottomRightArea.radius;

        _context.bottomRightArea.radiusSquared = leReal_i16_Square(_context.bottomRightArea.radius);
        _context.bottomRightArea.maxRadiusSquared = leReal_i16_Square(_context.bottomRightArea.radius + _context.halfWidth);
        _context.bottomRightArea.minRadiusSquared = leReal_i16_Square(_context.bottomRightArea.radius - _context.halfWidth);
    }

    if(_context.bottomLeftArea.radius > 0)
    {
        _context.bottomLeftArea.origin.x = -_context.rect.extents.x + _context.bottomLeftArea.radius;
        _context.bottomLeftArea.origin.y = _context.rect.extents.y - _context.bottomLeftArea.radius;

        _context.bottomLeftArea.radiusSquared = leReal_i16_Square(_context.bottomLeftArea.radius);
        _context.bottomLeftArea.maxRadiusSquared = leReal_i16_Square(_context.bottomLeftArea.radius + _context.halfWidth);
        _context.bottomLeftArea.minRadiusSquared = leReal_i16_Square(_context.bottomLeftArea.radius - _context.halfWidth);
    }
}

static void _testPoint(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_u8 len;
    leReal_i16 distX;
    leReal_i16 distY;

    if(_context.rotation != 0)
    {
        leVector2_Rotate(&krn->testPoint,
                         _context.rotation,
                         &_context.testPoint);
    }
    else
    {
        _context.testPoint.x = krn->testPoint.x;
        _context.testPoint.y = krn->testPoint.y;
    }

    distX = leReal_i16_AbsoluteValue(_context.testPoint.x);
    distY = leReal_i16_AbsoluteValue(_context.testPoint.y);

    if(distX > _context.maxRect.extents.x || distY > _context.maxRect.extents.y)
        return;

    if(_context.topLeftArea.radius > 0 &&
       (_context.testPoint.x < _context.topLeftArea.origin.x &&
       _context.testPoint.y < _context.topLeftArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.topLeftArea.origin.x;
        vec.y = _context.testPoint.y - _context.topLeftArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(_context.mask == LE_STROKEMASK_INSIDEONLY && len < _context.topLeftArea.radiusSquared)
            return;

        if(_context.mask == LE_STROKEMASK_OUTSIDEONLY && len > _context.topLeftArea.radiusSquared)
            return;

        if(len < _context.topLeftArea.maxRadiusSquared && len > _context.topLeftArea.minRadiusSquared)
        {
            krn->hitCount += 1;

            return;
        }
    }
    else if(_context.topRightArea.radius > 0 &&
            (_context.testPoint.x > _context.topRightArea.origin.x &&
             _context.testPoint.y < _context.topRightArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.topRightArea.origin.x;
        vec.y = _context.testPoint.y - _context.topRightArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(_context.mask == LE_STROKEMASK_INSIDEONLY && len < _context.topRightArea.radiusSquared)
            return;

        if(_context.mask == LE_STROKEMASK_OUTSIDEONLY && len > _context.topRightArea.radiusSquared)
            return;

        if(len < _context.topRightArea.maxRadiusSquared && len > _context.topRightArea.minRadiusSquared)
        {
            krn->hitCount += 1;

            return;
        }
    }
    else if(_context.bottomRightArea.radius > 0 &&
            (_context.testPoint.x > _context.bottomRightArea.origin.x &&
             _context.testPoint.y > _context.bottomRightArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.bottomRightArea.origin.x;
        vec.y = _context.testPoint.y - _context.bottomRightArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(_context.mask == LE_STROKEMASK_INSIDEONLY && len < _context.bottomRightArea.radiusSquared)
            return;

        if(_context.mask == LE_STROKEMASK_OUTSIDEONLY && len > _context.bottomRightArea.radiusSquared)
            return;

        if(len < _context.bottomRightArea.maxRadiusSquared && len > _context.bottomRightArea.minRadiusSquared)
        {
            krn->hitCount += 1;
        }
    }
    else if(_context.bottomLeftArea.radius > 0 &&
            (_context.testPoint.x < _context.bottomLeftArea.origin.x &&
             _context.testPoint.y > _context.bottomLeftArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.bottomLeftArea.origin.x;
        vec.y = _context.testPoint.y - _context.bottomLeftArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(_context.mask == LE_STROKEMASK_INSIDEONLY && len < _context.bottomLeftArea.radiusSquared)
            return;

        if(_context.mask == LE_STROKEMASK_OUTSIDEONLY && len > _context.bottomLeftArea.radiusSquared)
            return;

        if(len < _context.bottomLeftArea.maxRadiusSquared && len > _context.bottomLeftArea.minRadiusSquared)
        {
            krn->hitCount += 1;
        }
    }
    else
    {
        if(distX < _context.minRect.extents.x && distY < _context.minRect.extents.y)
            return;

        if(_context.mask == LE_STROKEMASK_INSIDEONLY &&
           distX < _context.rect.extents.x &&
           distY < _context.rect.extents.y)
        {
            return;
        }
        else if(_context.mask == LE_STROKEMASK_OUTSIDEONLY &&
                (distX > _context.rect.extents.x ||
                 distY > _context.rect.extents.y))
        {
            return;
        }

        krn->hitCount += 1;
    }
}

static uint8_t _shade_Gradient(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_u8 len;
    leReal_i16 distance;
    leReal_i16 delta;
    leReal_i16 hardness;
    leReal_i16 distX;
    leReal_i16 distY;

    (void)krn;

    delta = LE_REAL_I16_ONE;

    if(_context.topLeftArea.radius > 0 &&
       (_context.testPoint.x < _context.topLeftArea.origin.x &&
       _context.testPoint.y < _context.topLeftArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.topLeftArea.origin.x;
        vec.y = _context.testPoint.y - _context.topLeftArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len > _context.topLeftArea.maxRadiusSquared && len < _context.topLeftArea.minRadiusSquared)
            return 0;

        len = leReal_u8_SquareRoot(len);

        distance = LE_REAL_U8_TO_I16(len);

        distance = _context.topLeftArea.radius - distance;
        distance = leReal_i16_AbsoluteValue(distance);
    }
    else if(_context.topRightArea.radius > 0 &&
            (_context.testPoint.x > _context.topRightArea.origin.x &&
             _context.testPoint.y < _context.topRightArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.topRightArea.origin.x;
        vec.y = _context.testPoint.y - _context.topRightArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len > _context.topRightArea.maxRadiusSquared && len < _context.topRightArea.minRadiusSquared)
            return 0;

        len = leReal_u8_SquareRoot(len);

        distance = LE_REAL_U8_TO_I16(len);

        distance = _context.topRightArea.radius - distance;
        distance = leReal_i16_AbsoluteValue(distance);
    }
    else if(_context.bottomRightArea.radius > 0 &&
            (_context.testPoint.x > _context.bottomRightArea.origin.x &&
             _context.testPoint.y > _context.bottomRightArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.bottomRightArea.origin.x;
        vec.y = _context.testPoint.y - _context.bottomRightArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len > _context.bottomRightArea.maxRadiusSquared && len < _context.bottomRightArea.minRadiusSquared)
            return 0;

        len = leReal_u8_SquareRoot(len);

        distance = LE_REAL_U8_TO_I16(len);

        distance = _context.bottomRightArea.radius - distance;
        distance = leReal_i16_AbsoluteValue(distance);
    }
    else if(_context.bottomLeftArea.radius > 0 &&
            (_context.testPoint.x < _context.bottomLeftArea.origin.x &&
             _context.testPoint.y > _context.bottomLeftArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.bottomLeftArea.origin.x;
        vec.y = _context.testPoint.y - _context.bottomLeftArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len > _context.bottomLeftArea.maxRadiusSquared && len < _context.bottomLeftArea.minRadiusSquared)
            return 0;

        len = leReal_u8_SquareRoot(len);

        distance = LE_REAL_U8_TO_I16(len);

        distance = _context.bottomLeftArea.radius - distance;
        distance = leReal_i16_AbsoluteValue(distance);
    }
    else
    {
        distX = leReal_i16_AbsoluteValue(_context.rect.extents.x - leReal_i16_AbsoluteValue(_context.testPoint.x));
        distY = leReal_i16_AbsoluteValue(_context.rect.extents.y - leReal_i16_AbsoluteValue(_context.testPoint.y));

        distance = leReal_i16_Min(distX, distY);
    }

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

void leDraw_VectorRectStroke(const struct leRectF* rct,
                             const struct leVectorRect_StrokeAttr* attr)
{
    struct leOBB obb;
    struct leAABB aabb;
    struct leRect rect;
    leReal_i16 width;
    leReal_i16 hardness;

    memset(&_context, 0, sizeof(struct Context));

    _context.rect = *rct;
    _context.topLeftArea.radius = attr->topLeftRadius;
    _context.topRightArea.radius = attr->topRightRadius;
    _context.bottomRightArea.radius = attr->bottomRightRadius;
    _context.bottomLeftArea.radius = attr->bottomLeftRadius;

    width = attr->width;

    if(width <= 0)
    {
        width = LE_REAL_I16_FROM_FLOAT(0.5f);
    }

    _context.halfWidth = LE_REAL_I16_DIVIDE(width, LE_REAL_I16_TWO);

    _context.maxRect.extents.x = _context.rect.extents.x + _context.halfWidth;
    _context.maxRect.extents.y = _context.rect.extents.y + _context.halfWidth;
    _context.maxRect.origin.x = _context.rect.origin.x;
    _context.maxRect.origin.y = _context.rect.origin.y;

    _context.minRect.extents.x = _context.rect.extents.x - _context.halfWidth;
    _context.minRect.extents.y = _context.rect.extents.y - _context.halfWidth;
    _context.minRect.origin.x = _context.rect.origin.x;
    _context.minRect.origin.y = _context.rect.origin.y;

    _clampCorners();
    _setCornerOrigins();

    _context.base.color = attr->color;
    _context.base.alpha = LE_REAL_I16_DIVIDE(LE_REAL_I16_FROM_INT(attr->alpha), LE_REAL_I16_255);
    _context.base.aaMode = attr->aaMode;
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

    _context.maxEdgeDistance = _context.halfWidth;
    _context.minHardnessDistance = LE_REAL_I16_MULTIPLY(hardness, _context.maxEdgeDistance);

    if(attr->rotation != 0)
    {
        obb.angle = attr->rotation;
        obb.origin = _context.maxRect.origin;
        obb.extents = _context.maxRect.extents;

        leOBB_AABB(&obb, &aabb);
        leAABB_ToRect(&aabb, &_kernel.scanArea);

        _kernel.origin.x = aabb.origin.x;
        _kernel.origin.y = aabb.origin.y;

        _context.rotation = attr->rotation;
    }
    else
    {
        leRectF_ToRect(&_context.maxRect, &_kernel.scanArea);

        _kernel.origin.x = _context.maxRect.origin.x;
        _kernel.origin.y = _context.maxRect.origin.y;

        _context.rotation = 0;
    }

    leRenderer_GetFrameRect(&rect);

    if(leRectIntersects(&_kernel.scanArea, &rect) == LE_FALSE)
        return;

    leRectClip(&_kernel.scanArea, &rect, &_kernel.scanArea);

    leRenderer_GetClipRect(&rect);

    if(leRectIntersects(&_kernel.scanArea, &rect) == LE_FALSE)
        return;

    leRectClip(&_kernel.scanArea, &rect, &_kernel.scanArea);

    _kernel.evaluatePoint = _testPoint;

    if(hardness < LE_REAL_I16_ONE)
    {
        _kernel.shadeFragment = _shade_Gradient;
    }

    _kernel.context = (struct leVector_KernelContext*)&_context;

    leVector_Kernel_Exec(&_kernel);
}