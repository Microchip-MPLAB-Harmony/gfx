#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/vector/legato_vector_kernel.h"

static struct Context
{
    struct leVector_KernelContext base;
    struct leAABB aabb;
    struct lePoint iPoint;
    leReal_i16 radius;
    leReal_u8 radiusSquared;
    leReal_u8 hitDistance;
    leReal_i16 minHardnessDistance;
} _context;

static struct leVector_Kernel _kernel;

static void _testPoint(struct leVector_Kernel* krn)
{
    (void)krn;

    _context.hitDistance = leVector2_SquaredLength(&_kernel.testPoint);

    if(_context.hitDistance <= _context.radiusSquared)
    {
        _kernel.hitCount += 1;
    }
}

static uint8_t _shade_Gradient(struct leVector_Kernel* krn)
{
    leReal_i16 distance;
    leReal_i16 delta;
    leReal_i16 hardness;

    (void)krn;

    delta = LE_REAL_I16_ONE;

    distance = leReal_i16_FromU8(leReal_u8_SquareRoot(_context.hitDistance));

    if(distance >= _context.minHardnessDistance)
    {
        hardness = (_context.radius - _context.minHardnessDistance);
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

void leDraw_VectorPoint(const struct leVector2* pnt,
                        const struct leVectorPointAttr* attr)
{
    leSize sz;
    struct leRect rect;
    leReal_i16 width;
    leReal_i16 hardness;

    leRenderer_DisplaySize(&sz);

    if(attr == NULL)
        return;

    if(attr->alpha == 0 ||
       pnt->x < 0 ||
       pnt->x >= LE_REAL_I16_FROM_INT(sz.width) ||
       pnt->y < 0 ||
       pnt->y >= LE_REAL_I16_FROM_INT(sz.height))
    {
        return;
    }

    memset(&_context, 0, sizeof(struct Context));

    leVector2_ToPoint(pnt, &_context.iPoint);

    if(attr->width == 0)
    {
        if(attr->alpha == 255)
        {
            leRenderer_PutPixel_Safe(_context.iPoint.x,
                                     _context.iPoint.y,
                                     attr->color);
        }
        else
        {
            leRenderer_BlendPixel_Safe(_context.iPoint.x,
                                       _context.iPoint.y,
                                       attr->color,
                                       attr->alpha);
        }

        return;
    }

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

    _context.radius = LE_REAL_I16_MULTIPLY(width, LE_REAL_I16_ONE_OVER_TWO);
    _context.radiusSquared = leReal_i16_Square(_context.radius);
    _context.minHardnessDistance = LE_REAL_I16_MULTIPLY(hardness, _context.radius);

    _context.aabb.origin = *pnt;
    _context.aabb.extents.x = _context.radius;
    _context.aabb.extents.y = _context.radius;

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

    _context.base.color = attr->color;
    _context.base.alpha = LE_REAL_I16_DIVIDE(LE_REAL_I16_FROM_INT(attr->alpha), LE_REAL_I16_255);
    _context.base.aaMode = attr->aaMode;

    _kernel.evaluatePoint = _testPoint;

    if(attr->hardness < LE_REAL_I16_ONE)
    {
        _kernel.shadeFragment = _shade_Gradient;
    }

    _kernel.origin.x = pnt->x;
    _kernel.origin.y = pnt->y;
    _kernel.context = (struct leVector_KernelContext*)&_context;

    leVector_Kernel_Exec(&_kernel);
}