#ifndef LE_RECTF_H
#define LE_RECTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/common/legato_common.h"

#include "gfx/legato/vector/legato_vector2.h"

struct leRect;

typedef struct leRectF
{
	struct leVector2 origin;
	struct leVector2 extents;
} leRectF;

typedef enum leRectF_AnchorPoint
{
	LE_RECTF_TOPLEFT,
	LE_RECTF_TOP,
	LE_RECTF_TOPRIGHT,
	LE_RECTF_LEFT,
	LE_RECTF_CENTER,
	LE_RECTF_RIGHT,
	LE_RECTF_BOTTOMLEFT,
	LE_RECTF_BOTTOM,
	LE_RECTF_BOTTOMRIGHT,
} leRectF_AnchorPoint;

leResult leRectF_FromRect(struct leRectF* rctf,
                          const struct leRect* rct);

leResult leRectF_ToRect(const struct leRectF* rctf,
						struct leRect* rct);

leBool leRectF_IsValid(const struct leRectF* rct);

leResult leRectF_Set(struct leRectF* rct,
					 const struct leVector2* pnt,
					 leReal_i16 width,
					 leReal_i16 height);

leResult leRectF_SetFromPoints(struct leRectF* rct,
                               const struct leVector2* tl,
					           const struct leVector2* br);

leResult leRectF_GetPoint(const struct leRectF* rct,
						  enum leRectF_AnchorPoint anchor,
                          struct leVector2* pnt);

leResult leRectF_SetPoint(struct leRectF* rct,
						  enum leRectF_AnchorPoint anchor,
						  const struct leVector2* pnt);

leResult leRectF_AdjustPoint(struct leRectF* rct,
			     			 enum leRectF_AnchorPoint anchor,
                             const struct leVector2* mag);

leResult leRectF_SetPosition(struct leRectF* rct,
							 enum leRectF_AnchorPoint anchor,
							 const struct leVector2* pnt);

leResult leRectF_Translate(struct leRectF* rct,
			               const struct leVector2* mag);

leReal_i16 leRectF_GetWidth(const struct leRectF* rct);

leResult leRectF_SetWidth(struct leRectF* rct,
						  leReal_i16 w);

leReal_i16 leRectF_GetHeight(const struct leRectF* rct);

leResult leRectF_SetHeight(struct leRectF* rct,
                           leReal_i16 h);

leBool leRectF_ContainsPoint(const struct leRectF* rct,
			                 const struct leVector2* pnt);

leBool leRectF_Intersects(const struct leRectF* lrct,
			              const struct leRectF* rrct);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_RECTF_H */
