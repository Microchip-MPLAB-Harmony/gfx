// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_utils.h

  Summary:
    General internal utilities for the library
    
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_UTILS_H
#define LEGATO_UTILS_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

typedef struct leWidget leWidget;
typedef struct leList leList;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/*  Function:
        leWidget* leUtils_Pick(int32_t x, int32_t y)
 
    Summary:
        Finds the top-most visible widget in the tree at the given coordinates.
        
    Parameters:
        int32_t x - the x coordinate of the pick point
        int32_t y - the y coordinate of the pick point
        
    Returns:
        leWidget* - the result widget    
*/
LIB_EXPORT leWidget* leUtils_Pick(int32_t x,
                                  int32_t y);

/*  Function:
        leWidget* leUtils_PickFromLayer(const leLayer* layer, int32_t x, int32_t y)
 
    Summary:
        Finds the top-most visible widget in a layer at the given coordinates.
        
    Parameters:
        int32_t x - the x coordinate of the pick point
        int32_t y - the y coordinate of the pick point
        
    Returns:
        leWidget* - the result widget    
*/
LIB_EXPORT leWidget* leUtils_PickFromWidget(const leWidget* wgt,
                                            int32_t x,
                                            int32_t y);


/*  Function:
        void leUtils_PointToScreenSpace(leWidget* widget, lePoint* pnt)
 
    Summary:
        Converts a point from widget space into screen space
        
    Parameters:
        leWidget* widget - the subject widget
        lePoint* pnt - the point to convert
        
    Returns:
        void
*/
void leUtils_PointToScreenSpace(const leWidget* widget,
                                lePoint* pnt);

/*  Function:
        void leUtils_PointLayerToLocalSpace(leWidget* widget, lePoint* pnt)
 
    Summary:
        Converts a point from layer space into the local space of a widget
        
    Parameters:
        leWidget* widget - the subject widget
        lePoint* pnt - the point to convert
        
    Returns:
        void
*/
void leUtils_PointScreenToLocalSpace(const leWidget* widget,
                                     lePoint* pnt);

/*  Function:
        void leUtils_ClipRectToParent(leWidget* widget, leRect* rect)
 
    Summary:
        Clips a rectangle to the parent of a widget
        
    Parameters:
        leWidget* widget - the subject widget
        leRect* rect - the rectangle to clip
        
    Returns:
        void
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
void leUtils_RectToParentSpace(const leWidget* widget,
                               leRect* rect);

/*  Function:
        void leUtils_RectFromParentSpace(leWidget* widget, leRect* rect)
 
    Summary:
        Converts a rectangle from widget parent space to widget local
        space
        
    Parameters:
        leWidget* widget - the subject widget
        leRect* rect - the rectangle to convert
        
    Returns:
        void
*/
void leUtils_RectFromParentSpace(const leWidget* widget,
                                 leRect* rect);


/*  Function:
        void leUtils_RectToScreenSpace(leWidget* widget, leRect* rect)
 
    Summary:
        Converts a rectangle from widget local space to screen space
        
    Parameters:
        leWidget* widget - the subject widget
        leRect* rect - the rectangle to convert
        
    Returns:
        void
*/
void leUtils_RectToScreenSpace(const leWidget* widget,
                               leRect* rect);

/*  Function:
        leBool leUtils_ChildIntersectsParent(const leWidget* parent,
                                             const leWidget* child)
 
    Summary:
        Performs an intersection test between a parent widget and a child
        widget
        
    Parameters:
        leWidget* parent - the parent widget
        leWidget* child - the child widget
        
    Returns:
        leBool - result of the intersection test
*/
leBool leUtils_ChildIntersectsParent(const leWidget* parent,
                                     const leWidget* child);

/*  Function:
        leBool leUtils_GetNextHighestWidget(leWidget* wgt)
 
    Summary:
        Gets the next highest Z order widget in the tree from 'wgt'
        
    Parameters:
        leWidget* wgt - the widget to analyze
        
    Returns:
        leWidget* - the next highest widget or NULL if 'wgt' is
        already the highest
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

#endif // LEGATO_UTILS_H
