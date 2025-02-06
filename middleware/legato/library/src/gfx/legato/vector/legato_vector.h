#ifndef LE_VECTOR_H
#define LE_VECTOR_H

#include "gfx/legato/common/legato_color.h"
#include "gfx/legato/common/legato_rect.h"
#include "gfx/legato/vector/legato_aabb.h"
#include "gfx/legato/vector/legato_obb.h"
#include "gfx/legato/vector/legato_plane.h"
#include "gfx/legato/vector/legato_rectf.h"
#include "gfx/legato/core/legato_real_i16.h"
#include "gfx/legato/core/legato_real_u8.h"
#include "gfx/legato/vector/legato_vector2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum leAntialiasingMode
{
    LE_ANTIALIASING_NONE = 0,
    LE_ANTIALIASING_2X,
    LE_ANTIALIASING_4X,
    LE_ANTIALIASING_8X,
    LE_ANTIALIASING_16X
} leAntialiasingMode;

typedef enum leCapStyle
{
    LE_CAPSTYLE_NONE,
    LE_CAPSTYLE_SQUARE,
    LE_CAPSTYLE_ROUND
} leCapStyle;

typedef enum leStrokeMask
{
    LE_STROKEMASK_ALL,
    LE_STROKEMASK_INSIDEONLY,
    LE_STROKEMASK_OUTSIDEONLY
} leStrokeMask;

typedef struct leVectorPointAttr
{
    leColor color;
    uint8_t alpha;
    leReal_i16 width;
    leReal_i16 hardness;
    enum leAntialiasingMode aaMode;
} leVectorPointAttr;

typedef struct leVectorLineAttr
{
    leColor color;
    uint8_t alpha;
    leReal_i16 width;
    leReal_i16 hardness;
    enum leAntialiasingMode aaMode;
    enum leCapStyle capStyle;
} leVectorLineAttr;

typedef struct leVectorRect_FillAttr
{
    leColor color;
    uint8_t alpha;
    leReal_i16 rotation;
    enum leAntialiasingMode aaMode;
    leReal_i16 topLeftRadius;
    leReal_i16 topRightRadius;
    leReal_i16 bottomLeftRadius;
    leReal_i16 bottomRightRadius;
} leVectorRect_FillAttr;

typedef struct leVectorRect_StrokeAttr
{
    leColor color;
    uint8_t alpha;
    leReal_i16 width;
    leReal_i16 hardness;
    enum leStrokeMask mask;
    leReal_i16 rotation;
    enum leAntialiasingMode aaMode;
    leReal_i16 topLeftRadius;
    leReal_i16 topRightRadius;
    leReal_i16 bottomLeftRadius;
    leReal_i16 bottomRightRadius;
} leVectorRect_StrokeAttr;

typedef struct leVectorArc_StrokeAttr
{
    leColor color;
    uint8_t alpha;
    leReal_i16 width;
    leReal_i16 hardness;
    enum leStrokeMask mask;
    enum leAntialiasingMode aaMode;
    enum leCapStyle capStyle;
} leVectorArc_StrokeAttr;

typedef struct leVectorArc_FillAttr
{
    leColor color;
    uint8_t alpha;
    //leReal_i16 width;
    leReal_i16 hardness;
    enum leAntialiasingMode aaMode;
} leVectorArc_FillAttr;

typedef struct leVectorConvexPolygon_FillAttr
{
    leColor color;
    uint8_t alpha;
    leReal_i16 rotation;
    enum leAntialiasingMode aaMode;
} leVectorConvexPolygon_FillAttr;

void leDraw_VectorPoint(const struct leVector2* pnt,
                        const struct leVectorPointAttr* attr);

void leDraw_VectorHLine(const struct leVector2* pnt,
                        leReal_i16 width,
                        const struct leVectorLineAttr* attr);

void leDraw_VectorVLine(const struct leVector2* pnt,
                        leReal_i16 height,
                        const struct leVectorLineAttr* attr);

void leDraw_VectorLine(const struct leVector2* pnt0,
                       const struct leVector2* pnt1,
                       const struct leVectorLineAttr* attr);

void leDraw_VectorRectFill(const struct leRectF* rect,
                           const struct leVectorRect_FillAttr* attr);

void leDraw_VectorRectStroke(const struct leRectF* rect,
                             const struct leVectorRect_StrokeAttr* attr);

void leDraw_VectorArcStroke(const struct leVector2* orgin,
                            leReal_i16 radius,
                            int32_t start,
                            int32_t span,
                            const struct leVectorArc_StrokeAttr* attr);

void leDraw_VectorArcFill(const struct leVector2* orgin,
                          leReal_i16 radius,
                          int32_t start,
                          int32_t span,
                          const struct leVectorArc_FillAttr* attr);

void leDraw_VectorConvexPolygonFill(struct leVector2 points[],
                                    uint32_t count,
                                    const struct leVectorConvexPolygon_FillAttr* attr);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LE_VECTOR_H