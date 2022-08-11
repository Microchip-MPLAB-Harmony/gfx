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
    legato_utils.h

  Summary:
    General internal utilities for the library
    
*******************************************************************************/

/** \file legato_utils.h
 * @brief General internal utilities for the library.
 *
 * @details .
 */

#ifndef LEGATO_UTILS_H
#define LEGATO_UTILS_H

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/common/legato_color.h"
#include "gfx/legato/core/legato_scheme.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct leWidget leWidget;
typedef struct leList leList;

#define LE_TEST_FLAG(val, flag) ((val & flag) > 0)

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Finds the top-most visible widget in a layer at the given coordinates.
 * @details Finds the top-most visible widget in <span class="param">wgt</span>
 * at the given coordinates <span class="param">x</span> and
 * <span class="param">y</span>.
 * @code
 * leWidget* wdt = leUtils_PickFromWidget(wgt, x, y);
 * @endcode
 * @param wgt the widget to pick from
 * @param x position.
 * @param y position.
 * @return the result widget.
 */
leWidget* leUtils_PickFromWidget(const leWidget* wgt,
                                 int32_t x,
                                 int32_t y);

/**
 * @brief Converts a point from widget space into screen space.
 * @details Converts <span class="param">point</span> corresponding to
 * <span class="param">widget</span> space to
 * screen space.
 * @code
 * const leWidget* widget;
 * lePoint* pnt;
 * leUtils_PointToScreenSpace(widget, pnt);
 * @endcode
 * @param widget the subject widget.
 * @param pnt the point to convert.
 * @return void.
 */
void leUtils_PointToScreenSpace(const leWidget* widget,
                                lePoint* pnt);

/**
 * @brief Convert point from layer space into the local space of a widget.
 * @details Converts <span class="param">pnt</span> corresponding to
 * layer space to <span class="param">widget</span> space.
 * @code
 * const leWidget* widget;
 * lePoint* pnt;
 * leUtils_PointScreenToLocalSpace(widget, pnt);
 * @endcode
 * @param widget position.
 * @param pnt position.
 * @return void.
 */
void leUtils_PointScreenToLocalSpace(const leWidget* widget,
                                     lePoint* pnt);

/**
 * @brief Clips a rectangle to the parent of a widge.
 * @details Clips a rectangle <span class="param">rect</span> to
 * parent of <span class="param">widget</span>.
 * @code
 * const leWidget* widget;
 * leRect* rect;
 * leUtils_ClipRectToParent(widget, rect);
 * @endcode
 * @param widget position.
 * @param rect position.
 * @return void.
 */
void leUtils_ClipRectToParent(const leWidget* widget,
                              leRect* rect);

/*  Function:
        void leUtils_RectToParentSpace(leWidget* widget, leRect* rect)
 
    Summary:
        Converts a rectangle from widget local space to widget parent
        space.  Widget must be a child of a layer for this to function.
        
    Parameters:
        leWidget* widget - the subject widget
        leRect* rect - the rectangle to convert
        
    Returns:
        void
*/
/**
 * @brief Convert rectangle from widget local space to widget parent space.
 * @details Converts <span class="param">rect</span> from
 * <span class="param">widget</span> space to parent space.
 * @remark Widget must be a child of a layer for this to function.
 * @code
 * const leWidget* widget;
 * leRect* rect;
 * leUtils_RectToParentSpace(widget, rect);
 * @endcode
 * @param widget position.
 * @param rect position.
 * @return void.
 */
void leUtils_RectToParentSpace(const leWidget* widget,
                               leRect* rect);

/**
 * @brief Convert rectangle from widget parent space to widget local space.
 * @details Converts <span class="param">rect</span> to
 * <span class="param">widget</span> space.
 * @code
 * const leWidget* widget;
 * leRect* rect;
 * leUtils_RectToParentSpace(widget, rect);
 * @endcode
 * @param widget position.
 * @param rect position.
 * @return void.
 */
void leUtils_RectFromParentSpace(const leWidget* widget,
                                 leRect* rect);

/**
 * @brief Convert  rectangle from widget local space to screen space.
 * @details Converts <span class="param">rect</span> from
 * <span class="param">widget</span> space to screen space.
 * @code
 * const leWidget* widget;
 * leRect* rect;
 * leUtils_RectToParentSpace(widget, rect);
 * @endcode
 * @param widget position.
 * @param rect position.
 * @return void.
 */
void leUtils_RectToScreenSpace(const leWidget* widget,
                               leRect* rect);

/**
 * @brief Determines if parent and child intersect.
 * @details Determines if <span class="param">parent</span> and
 * <span class="param">child</span> intersects.
 * @code
 * const leWidget* parent;
 * const leWidget* child;
 * leUtils_ChildIntersectsParent(widget, rect);
 * @endcode
 * @param widget the parent widget.
 * @param child the child widget.
 * @return void.
 */
leBool leUtils_ChildIntersectsParent(const leWidget* parent,
                                     const leWidget* child);

/**
 * @brief Clips a widget's rectangle to the widget tree hierarchy
 * @details Clips a widget's rectangle to the widget tree hierarchy
 * @code
 * const leWidget* parent;
 * struct leRect* result
 * leUtils_ClipRectToAncestors(widget, &result);
 * @endcode
 * @param widget the parent widget.
 * @param result the clipped rectangle
 * @return void.
 */
void leUtils_ClipRectToAncestors(const leWidget* wgt,
                                 struct leRect* rct);

/**
 * @brief Get next highest Z order widget in the tree from 'wgt'.
 * @details Gets the next the highest Z order widget in the tree
 * <span class="param">widget</span>.
 * @code
 * leWidget * higest = leUtils_GetNextHighestWidget(wgt);
 * @endcode
 * @param widget position.
 * @param rect position.
 * @return the next highest widget or NULL if 'wgt' is already the highest.
 */
leWidget* leUtils_GetNextHighestWidget(const leWidget* wgt);

/*  Function:
        void leUtils_ArrangeRectangle(leRect* sub,
                                      leRect obj,
                                      leRect bounds,
                                      leHAlignment hAlignment,
                                      leVAlignment vAlignment,
                                      leRelativePosition position,
                                      uint8_t leftMargin,
                                      uint8_t topMargin,
                                      uint8_t rightMargin,
                                      uint8_t bottomMargin,
                                      uint16_t rectMargin)
 
    Summary:
        Calculates the position of a rectangle within the given bounds and
        in accordance with the given parameters.  A use case for this is
        when an image and a text rectangle must be arranged in a button box.  
        This version of the algorithm will calculate the location of the
        image rectangle.
        
    Parameters:
        leRect* sub - the bounds of the subject rectangle (image)
        leRect obj - the bounds of the object rectangle (text)
        leRect bounds - the bounds of the bounding rectangle (widget)
        leHAlignment hAlignment - the horizontal alignment of the rects
        leVAlignment vAlignment - the vertical alignment of the rects
        leRelativePosition position - the relative position of the rectangles
        uint8_t leftMargin - the left margin of the bounding rectangle
        uint8_t topMargin - the top margin of the bounding rectangle
        uint8_t rightMargin - the right margin of the bounding rectangle
        uint8_t bottomMargin - the bottom margin of the bounding rectangle
        uint16_t rectMargin - the distance between the image and the text rects
        
    Returns:
        void
        
    Remarks:
        The x and y position of sub will be manipulated by this function.  The
        dimensions of the rectangle should be set before calling and should
        remain unchanged after execution.
*/
/**
 * @brief Calculates the position of a rectangle within the given bound.
 * @details Calculates the position of a rectangle within the given bounds and
 * in accordance with the given parameters.  A use case for this is
 * when an image and a text rectangle must be arranged in a button box.  This version
 * of the algorithm will calculate the location of the image rectangle.
 * @code
 * leWidget * w = leUtils_ArrangeRectangle(wgt);
 * @endcode
 * @param widget position.
 * @param rect position.
 * @return void.
 */
void leUtils_ArrangeRectangle(leRect* sub,
                              leRect obj,
                              leRect bounds,
                              leHAlignment hAlignment,
                              leVAlignment vAlignment,
                              leRelativePosition position,
                              uint8_t leftMargin,
                              uint8_t topMargin,
                              uint8_t rightMargin,
                              uint8_t bottomMargin,
                              uint16_t rectMargin);

/*  Function:
        void leUtils_ArrangeRectangleRelative(leRect* sub,
                                              leRect obj,
                                              leRect bounds,
                                              leHAlignment hAlignment,
                                              leVAlignment vAlignment,
                                              leRelativePosition position,
                                              uint8_t leftMargin,
                                              uint8_t topMargin,
                                              uint8_t rightMargin,
                                              uint8_t bottomMargin,
                                              uint16_t rectMargin)
 
    Summary:
        Calculates the position of a rectangle within the given bounds and
        in accordance with the given parameters.  A use case for this is
        when an image and a text rectangle must be arranged in a button box.  
        This version of the algorithm will calculate the location of the
        text rectangle.
        
    Parameters:
        leRect* sub - the bounds of the subject rectangle (text)
        leRect obj - the bounds of the object rectangle (image)
        leRect bounds - the bounds of the bounding rectangle (widget)
        leHAlignment hAlignment - the horizontal alignment of the rects
        leVAlignment vAlignment - the vertical alignment of the rects
        leRelativePosition position - the relative position of the rectangles
        uint8_t leftMargin - the left margin of the bounding rectangle
        uint8_t topMargin - the top margin of the bounding rectangle
        uint8_t rightMargin - the right margin of the bounding rectangle
        uint8_t bottomMargin - the bottom margin of the bounding rectangle
        uint16_t rectMargin - the distance between the image and the text rects
        
    Returns:
        void
        
    Remarks:
        The x and y position of sub will be manipulated by this function.  The
        dimensions of the rectangle should be set before calling and should
        remain unchanged after execution.
*/
/**
 * @brief Calculates the position of a rectangle within the given bounds.
 * @details Calculates the position of a rectangle within the given bounds and
 * in accordance with the given parameters.  A use case for this is
 * when an image and a text rectangle must be arranged in a button box.  This version
 * of the algorithm will calculate the location of the image rectangle.
 * @remark The x and y position of sub will be manipulated by this function.  The
 * dimensions of the rectangle should be set before calling and should remain unchanged
 * after execution.
 * @code
 * leUtils_ArrangeRectangleRelative(wgt);
 * @endcode
 * @param widget position.
 * @param rect position.
 * @return void.
 */
void leUtils_ArrangeRectangleRelative(leRect* sub,
                                      leRect obj,
                                      leRect bounds,
                                      leHAlignment hAlignment,
                                      leVAlignment vAlignment,
                                      leRelativePosition position,
                                      uint8_t leftMargin,
                                      uint8_t topMargin,
                                      uint8_t rightMargin,
                                      uint8_t bottomMargin,
                                      uint16_t rectMargin);

void leUtils_PointLogicalToScratch(int16_t* x,
                                   int16_t* y);

void leUtils_RectLogicalToScratch(leRect* rect);


const leBlendLookupTable* leUtils_GetSchemeLookupTable(const leScheme* schm,
                                                       leColor foreground,
                                                       leColor background);

#ifdef __cplusplus
}
#endif

#endif // LEGATO_UTILS_H
