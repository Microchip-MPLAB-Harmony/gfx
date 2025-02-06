#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/vector/legato_vector.h"
#include "gfx/legato/vector/legato_vector_kernel.h"

struct ComplexScanArea
{
    struct leVector2 origin;
    leReal_i16 radius;
    leReal_u8 radiusSquared;
};

static struct Context
{
    struct leVector_KernelContext base;
    struct leRectF rect;
    leReal_i16 rotation;
    struct leVector2 testPoint;
    struct ComplexScanArea topLeftArea;
    struct ComplexScanArea topRightArea;
    struct ComplexScanArea bottomRightArea;
    struct ComplexScanArea bottomLeftArea;
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
    }

    if(_context.topRightArea.radius > 0)
    {
        _context.topRightArea.origin.x = _context.rect.extents.x - _context.topRightArea.radius;
        _context.topRightArea.origin.y = -_context.rect.extents.y + _context.topRightArea.radius;

        _context.topRightArea.radiusSquared = leReal_i16_Square(_context.topRightArea.radius);
    }

    if(_context.bottomRightArea.radius > 0)
    {
        _context.bottomRightArea.origin.x = _context.rect.extents.x - _context.bottomRightArea.radius;
        _context.bottomRightArea.origin.y = _context.rect.extents.y - _context.bottomRightArea.radius;

        _context.bottomRightArea.radiusSquared = leReal_i16_Square(_context.bottomRightArea.radius);
    }

    if(_context.bottomLeftArea.radius > 0)
    {
        _context.bottomLeftArea.origin.x = -_context.rect.extents.x + _context.bottomLeftArea.radius;
        _context.bottomLeftArea.origin.y = _context.rect.extents.y - _context.bottomLeftArea.radius;

        _context.bottomLeftArea.radiusSquared = leReal_i16_Square(_context.bottomLeftArea.radius);
    }
}

static void _testPoint(struct leVector_Kernel* krn)
{
    struct leVector2 vec;
    leReal_u8 len;

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

    // point in the rectangle?
    if(_context.testPoint.x < -_context.rect.extents.x ||
       _context.testPoint.x > _context.rect.extents.x ||
       _context.testPoint.y < -_context.rect.extents.y ||
       _context.testPoint.y > _context.rect.extents.y)
    {
        return;
    }

    if(_context.topLeftArea.radius > 0 &&
       (_context.testPoint.x < _context.topLeftArea.origin.x &&
        _context.testPoint.y < _context.topLeftArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.topLeftArea.origin.x;
        vec.y = _context.testPoint.y - _context.topLeftArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len < _context.topLeftArea.radiusSquared)
        {
            krn->hitCount += 1;
        }
    }
    else if(_context.topRightArea.radius > 0 &&
            (_context.testPoint.x > _context.topRightArea.origin.x &&
             _context.testPoint.y < _context.topRightArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.topRightArea.origin.x;
        vec.y = _context.testPoint.y - _context.topRightArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len < _context.topRightArea.radiusSquared)
        {
            krn->hitCount += 1;
        }
    }
    else if(_context.bottomRightArea.radius > 0 &&
            (_context.testPoint.x > _context.bottomRightArea.origin.x &&
             _context.testPoint.y > _context.bottomRightArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.bottomRightArea.origin.x;
        vec.y = _context.testPoint.y - _context.bottomRightArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len < _context.bottomRightArea.radiusSquared)
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

        if(len < _context.bottomLeftArea.radiusSquared)
        {
            krn->hitCount += 1;
        }
    }
    else
    {
        krn->hitCount += 1;
    }
}

#if 0
static void _configurePoint(leReal_i16 offsX,
                            leReal_i16 offsY)
{
    _context.testPoint.x = _context.pixelCenter.x + offsX;
    _context.testPoint.y = _context.pixelCenter.y + offsY;

    if(_context.rotation != 0)
    {
        leVector2_Rotate(&_context.testPoint,
                         _context.rotation,
                         &_context.testPoint);
    }
}

static leBool _configureAndTestPoint(leReal_i16 offsX,
                                     leReal_i16 offsY)
{
    struct leVector2 vec;
    leReal_u8 len;

    _configurePoint(offsX, offsY);

    // point in the rectangle?
    if(_context.testPoint.x < -_context.rect.extents.x ||
       _context.testPoint.x > _context.rect.extents.x ||
       _context.testPoint.y < -_context.rect.extents.y ||
       _context.testPoint.y > _context.rect.extents.y)
        return LE_FALSE;

    if(_context.topLeftArea.radius > 0 &&
       (_context.testPoint.x < _context.topLeftArea.origin.x &&
       _context.testPoint.y < _context.topLeftArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.topLeftArea.origin.x;
        vec.y = _context.testPoint.y - _context.topLeftArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len < _context.topLeftArea.radiusSquared)
        {
            _context.hitCount += 1;

            return LE_TRUE;
        }
    }
    else if(_context.topRightArea.radius > 0 &&
            (_context.testPoint.x > _context.topRightArea.origin.x &&
             _context.testPoint.y < _context.topRightArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.topRightArea.origin.x;
        vec.y = _context.testPoint.y - _context.topRightArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len < _context.topRightArea.radiusSquared)
        {
            _context.hitCount += 1;

            return LE_TRUE;
        }
    }
    else if(_context.bottomRightArea.radius > 0 &&
            (_context.testPoint.x > _context.bottomRightArea.origin.x &&
             _context.testPoint.y > _context.bottomRightArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.bottomRightArea.origin.x;
        vec.y = _context.testPoint.y - _context.bottomRightArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len < _context.bottomRightArea.radiusSquared)
        {
            _context.hitCount += 1;

            return LE_TRUE;
        }
    }
    else if(_context.bottomLeftArea.radius > 0 &&
            (_context.testPoint.x < _context.bottomLeftArea.origin.x &&
             _context.testPoint.y > _context.bottomLeftArea.origin.y))
    {
        vec.x = _context.testPoint.x - _context.bottomLeftArea.origin.x;
        vec.y = _context.testPoint.y - _context.bottomLeftArea.origin.y;

        len = leVector2_SquaredLength(&vec);

        if(len < _context.bottomLeftArea.radiusSquared)
        {
            _context.hitCount += 1;

            return LE_TRUE;
        }
    }
    else
    {
        _context.hitCount += 1;

        return LE_TRUE;
    }

    return LE_FALSE;
}
#endif

void leDraw_VectorRectFill(const struct leRectF* rct,
                           const struct leVectorRect_FillAttr* attr)
{
    struct leOBB obb;
    struct leAABB aabb;
    struct leRect rect;

    memset(&_context, 0, sizeof(struct Context));

    _context.rect = *rct;
    _context.topLeftArea.radius = attr->topLeftRadius;
    _context.topRightArea.radius = attr->topRightRadius;
    _context.bottomRightArea.radius = attr->bottomRightRadius;
    _context.bottomLeftArea.radius = attr->bottomLeftRadius;

    _clampCorners();
    _setCornerOrigins();

    _context.base.color = attr->color;
    _context.base.alpha = LE_REAL_I16_DIVIDE(LE_REAL_I16_FROM_INT(attr->alpha), LE_REAL_I16_255);
    _context.base.aaMode = attr->aaMode;

    if(attr->rotation != 0)
    {
        obb.angle = attr->rotation;
        obb.origin = _context.rect.origin;
        obb.extents = _context.rect.extents;

        leOBB_AABB(&obb, &aabb);
        leAABB_ToRect(&aabb, &_kernel.scanArea);

        _kernel.origin.x = aabb.origin.x;
        _kernel.origin.y = aabb.origin.y;

        _context.rotation = attr->rotation;
    }
    else
    {
        leRectF_ToRect(&_context.rect, &_kernel.scanArea);

        _kernel.origin.x = _context.rect.origin.x;
        _kernel.origin.y = _context.rect.origin.y;

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

    _kernel.context = (struct leVector_KernelContext*)&_context;

    leVector_Kernel_Exec(&_kernel);
}