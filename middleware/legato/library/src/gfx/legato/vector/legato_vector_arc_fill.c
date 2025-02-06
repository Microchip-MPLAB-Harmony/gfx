#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/vector/legato_vector_kernel.h"

extern leReal_i16* _AA_SampleData[5];
extern leReal_i16 __AA_SampleWeights[5];
extern uint32_t _AA_SampleCount[5];

#define PI_ANG16 ((int32_t)(180 * 16))
#define TWOPI_ANG16 ((int32_t)(360 * 16))

#define REAL_16 LE_REAL_I16_FROM_FLOAT(16.0f)

struct ArcRange
{
    int32_t start;
    int32_t end;
};

static struct Context
{
    struct leVector_KernelContext base;
    struct leAABB aabb;
    leReal_i16 radius;
    leReal_u8 radiusSquared;
    int32_t start;
    int32_t span;
    leReal_i16 minHardnessDistance;
    uint32_t rangeCount;
    struct ArcRange ranges[2];
    uint32_t hitCount;
    leReal_u8 hitDistance;
    leReal_i16 shadePrecision;
} _context;

static struct leVector_Kernel _kernel;

struct ArcHitTestResult
{
    leReal_i16 radians;
    int32_t degrees;
    leReal_u8 squaredDistance;
};

static int32_t _radianToDegree(leReal_i16 rad)
{
    int32_t res;
    int32_t mod;

    if(rad == 0)
        return 0;

    res = LE_REAL_I16_MULTIPLY(rad, LE_REAL_I16_DEGREES);
    res = LE_REAL_I16_MULTIPLY(res, REAL_16);
    res = leReal_i16_ToInt(res);

    if(res < 0)
    {
        res += 360 * 16;
    }

    mod = res % 16;

    if(mod < 7)
    {
        res -= mod;
    }
    else
    {
        res += (16 - mod);
    }

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

static leBool _arcTest(struct ArcHitTestResult* res)
{
    res->radians = -leVector2_AngleOf(&_kernel.testPoint);
    res->degrees = _radianToDegree(res->radians);

    if(res->degrees < 0)
    {
        res->degrees = (PI_ANG16 + res->degrees) + PI_ANG16;
    }

    if(_context.rangeCount == 1)
    {
        if(!(res->degrees >= _context.ranges[0].start &&
             res->degrees <= _context.ranges[0].end))
        {
            return LE_FALSE;
        }
    }
    for(uint32_t idx = 0; idx < _context.rangeCount; ++idx)
    {
        if(!((res->degrees >= _context.ranges[0].start && res->degrees <= _context.ranges[0].end) ||
             (res->degrees >= _context.ranges[1].start && res->degrees <= _context.ranges[1].end)))
        {
            return LE_FALSE;
        }
    }

    res->squaredDistance = leVector2_SquaredLength(&_kernel.testPoint);

    if(res->squaredDistance >= _context.radiusSquared)
    {
        return LE_FALSE;
    }

    return LE_TRUE;
}

static void _evaluatePoint(struct leVector_Kernel* krn)
{
    (void)krn;

    _context.hitCount = 0;
    struct ArcHitTestResult res;

    if(_arcTest(&res) == LE_TRUE)
    {
        _context.hitDistance = res.squaredDistance;
        _context.hitCount += 1;

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

    if(_context.start + _context.span > TWOPI_ANG16)
    {
        _context.rangeCount = 2;
        _context.ranges[0].start = _context.start;
        _context.ranges[0].end = TWOPI_ANG16;
        _context.ranges[1].start = 0;
        _context.ranges[1].end = (_context.start + _context.span) - TWOPI_ANG16;
    }
    else
    {
        _context.rangeCount = 1;
        _context.ranges[0].start = _context.start;
        _context.ranges[0].end = _context.start + _context.span;
    }
}

static void _calculateScanArea(void)
{
    _context.aabb.origin.x = _kernel.origin.x;
    _context.aabb.origin.y = _kernel.origin.y;
    _context.aabb.extents.x = _context.radius;
    _context.aabb.extents.y = _context.aabb.extents.x;

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
        dist = LE_REAL_U8_TO_I16(leReal_u8_SquareRoot(_context.hitDistance));

        if(dist > _context.minHardnessDistance)
        {
            hardness = _context.radius - _context.minHardnessDistance;
            dist -= _context.radius - hardness;
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

void leDraw_VectorArcFill(const struct leVector2* org,
                          leReal_i16 rad,
                          int32_t start,
                          int32_t span,
                          const struct leVectorArc_FillAttr* attr)
{
    struct leRect rect;
    //leReal_i16 width;
    leReal_i16 hardness;

    memset(&_context, 0, sizeof(struct Context));

    _kernel.origin = *org;
    _context.radius = rad;
    _context.radiusSquared = leReal_i16_Square(rad);
    _context.start = start;
    _context.span = span;

    if(span == 0)
        return;

    /*width = attr->width;

    if(width <= 0)
    {
        width = LE_REAL_I16_FROM_FLOAT(0.5f);
    }*/

    _context.base.color = attr->color;
    _context.base.alpha = LE_REAL_I16_DIVIDE(LE_REAL_I16_FROM_INT(attr->alpha), LE_REAL_I16_255);
    hardness = attr->hardness;

    if(attr->hardness < 0)
    {
        hardness = 0;
    }
    else if(attr->hardness > LE_REAL_I16_ONE)
    {
        hardness = LE_REAL_I16_ONE;
    }

    _context.minHardnessDistance = LE_REAL_I16_MULTIPLY(hardness, _context.radius);

    _adjustAngles();

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
        _kernel.evaluatePoint = _evaluatePoint;

        _kernel.evaluatePoint = _evaluatePoint;
    }
    else
    {
        _context.base.aaMode = LE_ANTIALIASING_NONE;
        _kernel.shadeFragment = _shade_Gradient;

        _kernel.evaluatePoint = _evaluatePoint;
    }

    //_drawBoundingArea();

    //_drawHitBoxes();

    _kernel.context = (struct leVector_KernelContext*)&_context;

    leVector_Kernel_Exec(&_kernel);
}