#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/vector/legato_vector.h"
#include "gfx/legato/vector/legato_vector_kernel.h"

#include "gfx/legato/memory/legato_memory.h"

static struct Context
{
    struct leVector_KernelContext base;
    struct leVector2* points;
    struct leRectF bounds;
    uint32_t count;
    struct leVector2 testPoint;
    struct lePlane* planes;
} _context;

static struct leVector_Kernel _kernel;

static void _testPoint(struct leVector_Kernel* krn)
{
    leReal_i16 d;

    _context.testPoint.x = krn->testPoint.x;
    _context.testPoint.y = krn->testPoint.y;

    // point in the polygon?
    for(uint32_t i = 0; i < _context.count; ++i)
    {
        d = lePlane_Distance(&_context.planes[i],
                             &_context.testPoint);

        if(d > 0)
        {
            return;
        }
    }

    krn->hitCount += 1;
}

static void _calculateBoundingRect(void)
{
    struct leVector2 topLeft = _context.points[0];
    struct leVector2 bottomRight = _context.points[0];
    struct leRectF rectf;

    for(uint32_t i = 1; i < _context.count; ++i)
    {
        if(_context.points[i].x < topLeft.x)
        {
            topLeft.x = _context.points[i].x;
        }

        if(_context.points[i].x > bottomRight.x)
        {
            bottomRight.x = _context.points[i].x;
        }

        if(_context.points[i].y < topLeft.y)
        {
            topLeft.y = _context.points[i].y;
        }

        if(_context.points[i].y > bottomRight.y)
        {
            bottomRight.y = _context.points[i].y;
        }
    }

    leRectF_SetFromPoints(&rectf,
                          &topLeft,
                          &bottomRight);

    leRectF_ToRect(&rectf, &_kernel.scanArea);

    _kernel.origin.x = rectf.origin.x;
    _kernel.origin.y = rectf.origin.y;
}

static void _buildPlanes(void)
{
    _context.planes = leMalloc(sizeof(struct lePlane) * _context.count);
    struct leVector2 normal;
    struct leLineF line;

    for(uint32_t i = 0; i < _context.count - 1; ++i)
    {
        line.p0 = _context.points[i];
        line.p1 = _context.points[i + 1];

        leVector2_Subtract(&line.p0, &_kernel.origin, &line.p0);
        leVector2_Subtract(&line.p1, &_kernel.origin, &line.p1);

        leLineF_Normal(&line, &normal);

        lePlane_Create(&line.p0,
                       &normal,
                       &_context.planes[i]);
    }

    line.p0 = _context.points[_context.count - 1];
    line.p1 = _context.points[0];

    leVector2_Subtract(&line.p0, &_kernel.origin, &line.p0);
    leVector2_Subtract(&line.p1, &_kernel.origin, &line.p1);

    leLineF_Normal(&line, &normal);

    lePlane_Create(&line.p0,
                   &normal,
                   &_context.planes[_context.count - 1]);
}

void leDraw_VectorConvexPolygonFill(struct leVector2 points[],
                                    uint32_t count,
                                    const struct leVectorConvexPolygon_FillAttr* attr)
{
    struct leRect rect;

    memset(&_context, 0, sizeof(struct Context));

    // must have at least three points
    if(count <= 2)
    {
        return;
    }

    _context.points = points;
    _context.count = count;
    _context.base.color = attr->color;
    _context.base.alpha = LE_REAL_I16_DIVIDE(LE_REAL_I16_FROM_INT(attr->alpha), LE_REAL_I16_255);
    _context.base.aaMode = attr->aaMode;

    _calculateBoundingRect();

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

    _buildPlanes();

    leVector_Kernel_Exec(&_kernel);

    leFree(_context.planes);
}