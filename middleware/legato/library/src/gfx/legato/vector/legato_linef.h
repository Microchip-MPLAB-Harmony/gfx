#ifndef LE_LINE_H
#define LE_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/vector/legato_vector2.h"

typedef struct leLineF
{
    struct leVector2 p0;
    struct leVector2 p1;
} leLineF;

leReal_i16 leLineF_Length(const struct leLineF* ln);

leReal_i16 leLineF_Angle(const struct leLineF* ln);

void leLineF_Center(const struct leLineF* ln,
                    struct leVector2* pnt);

leReal_i16 leLineF_PointDistance(const struct leLineF* ln,
                                 const struct leVector2* pnt);

void leLineF_ClosestPoint(const struct leLineF* ln,
                          const struct leVector2* pnt,
                          struct leVector2* res);

leBool leLineF_PointSideQuery(const struct leLineF* ln,
                              const struct leVector2* pnt,
                              const struct leVector2* sign);

leResult leLineF_Normal(const struct leLineF* ln,
                        struct leVector2* nrm);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_LINE_H */
