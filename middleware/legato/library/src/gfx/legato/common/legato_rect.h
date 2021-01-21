/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_rect.h

  Summary:
    Defines general purposes rectangle functions.

  Description:
    Rectangle management functions.
*******************************************************************************/

/** \file legato_rect.h
 * @brief Rectangle functions and definitions.
 *
 * @details Rectangle management functions.
 *
 */

#ifndef LE_RECT_H
#define LE_RECT_H

#include "gfx/legato/common/legato_common.h"

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/**
 * @brief Determines if a point is inside a rectangle.
 * @details Returns true if <span class="param">point</span> is
 * inside <span class="param">rect</span>.
 * @code
 * const leRect* rect;
 * const lePoint* point;
 * leBool isInside = leRectContainsPoint(rect, point);
 * @endcode
 * @param rect is the area to search.
 * @param point is the criteria
 * @returns LE_TRUE if inside, otherwise LE_FALSE;
 */
leBool leRectContainsPoint(const leRect* rect, const lePoint* point);

// *****************************************************************************
/**
 * @brief Determines if a rectangle is completely inside another rectangle.
 * @details Returns true if <span class="param">l_rect</span> is
 * inside <span class="param">r_rect</span>.
 * @code
 * leRect* l_rect;
 * leRect* r_rect;
 * leBool isInside = leRectContainsRect(l_rect, r_rect);
 * @endcode
 * @param rect is the area to search.
 * @param point is the criteria
 * @returns LE_TRUE if inside, otherwise LE_FALSE;
 */
leBool leRectContainsRect(const leRect* l_rect, const leRect* r_rect);

// *****************************************************************************
/**
 * @brief Determines if two rectangles are intersecting.
 * @details Returns true if <span class="param">l_rect</span> intersects
 * <span class="param">r_rect</span>.
 * @code
 * leRect* l_rect;
 * leRect* r_rect;
 * leBool isInside = leRectIntersects(l_rect, r_rect);
 * @endcode
 * @param l_rect a rectangle.
 * @param r_rect a rectangle.
 * @returns LE_TRUE if intersect, otherwise LE_FALSE;
 */
leBool leRectIntersects(const leRect* l_rect, const leRect* r_rect);

// *****************************************************************************
/**
 * @brief Combine rectangles.
 * @details Returns a single rectangle after combining <span class="param">l_rect</span> and
 * <span class="param">r_rect</span>.
 * @code
 * leRect* l_rect;
 * leRect* r_rect;
 * leRect res;
 * leRectCombine(l_rect, r_rect, &res);
 * @endcode
 * @param l_rect a rectangle.
 * @param r_rect a rectangle.
 * @param res the resulting rectangle
 * @return leRect.
 */
void leRectCombine(const leRect* l_rect,
                   const leRect* r_rect,
                   leRect* res);

// *****************************************************************************
/**
 * @brief Clips a rectangle to the space of another rectangle.
 * @details Produces a single rectangle after cliping <span class="param">r_rect</span> by
 * <span class="param">l_rect</span>.
 * @remark result is equalvent to l_rect if the rectangles do not intersect.
 * @code
 * leRect* l_rect;
 * leRect* r_rect;
 * leRectClip(l_rect, r_rect, result);
 * @endcode
 * @param l_rect a rectangle.
 * @param r_rect a rectangle.
 * @param result is the new rectangle.
 * @return void.
 */
void leRectClip(const leRect* l_rect,
                const leRect* r_rect,
                leRect* result);

// *****************************************************************************
/* Function:
    leRect leRectClipAdj(const leRect* l_rect,
                         const leRect* r_rect, 
                         leRect* adj)

  Summary:
    Returns the rectangle clipped using r_rect, and also adjusts the third rectangle

  Parameters:
    const leRect* l_rect - the subject rectangle
    const leRect* r_rect - the object rectangle
    leRect* adj - the adjust rectangle
    
  Returns:
    void
    
  Remarks:
    result will equals l_rect if the rectangles aren't intersecting
    
*/
/**
 * @brief Clips a rectangle to the space of another rectangle.
 * @details Produces a single rectangle after cliping <span class="param">r_rect</span> by
 * <span class="param">l_rect</span>.
 * @remark result is equalvent to l_rect if the rectangles do not intersect.
 * @code
 * leRect* l_rect;
 * leRect* r_rect;
 * leRect* rect = leRectClipAdj(l_rect, r_rect, result);
 * @endcode
 * @param l_rect a rectangle.
 * @param r_rect a rectangle.
 * @param result is the adjusted rectangle.
 * @param res the resultant rectangle
 * @return void.
 */
void leRectClipAdj(const leRect* l_rect,
                   const leRect* r_rect,
                   leRect* adj,
                   leRect* res);

// *****************************************************************************                 
/**
 * @brief Create rectangle from two points.
 * @details Creates a rectangle specified by <span class="param">p1</span> and
 * p2</span>.
 * @code
 * lePoint* p1;
 * lePoint* p2;
 * leRect* r_rect = leRectFromPoints(p1, p2);
 * @endcode
 * @param p1 the first point
 * @param p2 the second point
 * @param res the resultant rectangle
 * @return leRect structure based on 2 points.
 */
void leRectFromPoints(const lePoint* p1,
                      const lePoint* p2,
                      leRect* res);

// *****************************************************************************
/**
 * @brief Create rectangle from two points.
 * @details Returns a rectangle specified by <span class="param">p1</span> and
 * <span class="param">p2</span>.
 * @code
 * leRect* l_rect;
 * lePoint* p1;
 * lePoint* p2;
 * leRectFromPoints(p1, p2);
 * @endcode
 * @param rect the rectangle
 * @param p1 the point of upper left vertex
 * @param p2 the point of the lower right vertex
 * @return void.
 */
void leRectToPoints(const leRect* rect,
                    lePoint* p1,
                    lePoint* p2);

// *****************************************************************************
/**
 * @brief Split rectangles
 * @details Splits <span class="param">sub</span> and
 * <span class="param">obj</span> into a maximum of four
 * non-overlapping rectangles <span class="param">res</span>.
 * @code
 * leRect* sub;
 * leRect* obj;
 * leRect res[4];
 * int32_t isSimilar = leRectSplit(sub, obj, res);
 * @endcode
 * @param sub the first rectangle.
 * @param obj the second rectangle.
 * @param res
 * @return the number of non-overlapping rectangles returned.
 */
uint32_t leRectSplit(const leRect* sub,
                     const leRect* obj,
                     leRect res[4]);

// *****************************************************************************
/**
 * @brief Determines if two rectangles are similar.
 * @details Determines if <span class="param">l</span> and
 * <span class="param">r</<span class="param">> have the same position
 * and dimensions.
 * @code
 * leRect* l;
 * leRect* r;
 * int32_t isSimilar = leRectCompare(l, r);
 * @endcode
 * @param l a point.
 * @param r a point.
 * @return leRect.
 */
int32_t leRectCompare(const leRect* l,
                      const leRect* r);

// *****************************************************************************
/**
 * @brief Determine if two rectanges are similar.
 * @details Determines if <span class="param">l</span> and
 * <span class="param">r</span> are adjancent and vertically or
 * horizontally aligned.
 * @code
 * leRect* l;
 * leRect* r;
 * leBool isSimilar = leRectsAreSimilar(l, r);
 * @endcode
 * @param l a point.
 * @param r a point.
 * @returns LE_TRUE if similar, otherwise LE_FALSE;
 */
leBool leRectsAreSimilar(const leRect* l,
                         const leRect* r);

/**
  * @cond INTERNAL
  *
  */static const leRect leRect_Zero = {0, 0, 0, 0};
/**
  * @endcond
  *
  */

#endif /* LE_RECT_H */
