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
    legato_rect.h

  Summary:
    Defines general purposes rectangle functions.

  Description:
    Rectangle management functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LE_RECT_H
#define LE_RECT_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leBool leRectContainsPoint(const leRect* rect, const lePoint* point)

  Summary:
    Determines if a point is inside a rectangle.

  Parameters:
    const leRect* rect - the rectangle to test
    const lePoint* point - the point to use for the test
    
  Returns:
    leBool - LE_TRUE if the point is inside the rectangle
*/
LIB_EXPORT leBool leRectContainsPoint(const leRect* rect, const lePoint* point);

// *****************************************************************************
/* Function:
    leBool leRectContainsRect(const leRect* l_rect, const leRect* r_rect)

  Summary:
    Determines if a rectangle is completely inside another rectangle.  Still
    returns true if the edges are touching.

  Parameters:
    const leRect* l_rect - the subject rectangle
    const leRect* r_rect - the object rectangle
    
  Returns:
    leBool - returns LE_TRUE if r_rect is completly inside l_rect
*/
LIB_EXPORT leBool leRectContainsRect(const leRect* l_rect, const leRect* r_rect);

// *****************************************************************************
/* Function:
    leBool leRectIntersects(const leRect* l_rect, const leRect* r_rect)

  Summary:
    Determines if two rectangles are intersecting

  Parameters:
    const leRect* l_rect - rectangle argument
    const leRect* r_rect - rectangle argument
    
  Returns:
    leBool - returns LE_TRUE if l_rect and r_rect are intersecting
*/
LIB_EXPORT leBool leRectIntersects(const leRect* l_rect, const leRect* r_rect);

// *****************************************************************************
/* Function:
    leRect leRectCombine(const leRect* l_rect, 
                         const leRect* r_rect)

  Summary:
    Combines the area of two rectangles into a single rectangle.

  Parameters:
    const leRect* l_rect - the first rectangle
    const leRect* r_rect - the second rectangle
    
  Returns:
    void
    
  Remarks:
*/
LIB_EXPORT leRect leRectCombine(const leRect* l_rect, 
                                const leRect* r_rect);

// *****************************************************************************
/* Function:
    void leRectClip(const leRect* l_rect, 
                    const leRect* r_rect, 
                    leRect* result)

  Summary:
    Clips a rectangle to the space of another rectangle.  The result rectangle
    is a rectangle that will fit inside both of the given rectangles.

  Parameters:
    const leRect* l_rect - the subject rectangle
    const leRect* r_rect - the object rectangle
    leRect* result - the result rectangle
    
  Returns:
    void
    
  Remarks:
    result will equals l_rect if the rectangles aren't intersecting
    
*/
LIB_EXPORT void leRectClip(const leRect* l_rect, 
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
LIB_EXPORT leRect leRectClipAdj(const leRect* l_rect,
                                const leRect* r_rect, 
                                leRect* adj);

// *****************************************************************************
/* Function:
    leRect leRectFromPoints(const lePoint* p1,
                            const lePoint* p2)

  Summary:
    Returns a leRect structure based on 2 points

  Parameters:
    const lePoint* p1 - the first point
    const lePoint* p2 - the second point
    
  Returns:
    leRect
    
  Remarks:
    
*/                             
LIB_EXPORT leRect leRectFromPoints(const lePoint* p1,
                                   const lePoint* p2);

// *****************************************************************************
/* Function:
    void leRectToPoints(const leRect* rect,
                        lePoint* p1,
                        lePoint* p2

  Summary:
    Returns the points for the upper left and lower right vertices 
    of a rectangle

  Parameters:
    const leRect* rect - the rectangle
    lePoint* p1 - the point of upper left vertex
    lePoint* p2 - the point of the lower right vertex
    
  Returns:
    leRect
    
  Remarks:
    
*/
LIB_EXPORT void leRectToPoints(const leRect* rect,
                               lePoint* p1,
                               lePoint* p2);    

// *****************************************************************************
/* Function:
    uint32_t leRectSplit(const leRect* sub,
                         const leRect* obj,
                         leRect res[4])

  Summary:
    Splits two overlapping rectangles into several (up to 4) 
    non-overlapping rectangles

  Parameters:
    const leRect* sub - the first rectangle
    const leRect* obj - the second rectangle
    leRect res[4] - the output rectangles
    
  Returns:
    uint32_t - the number of non-overlapping rectangles returned
    
  Remarks:
    
*/
LIB_EXPORT uint32_t leRectSplit(const leRect* sub,
                                const leRect* obj,
                                leRect res[4]); 

// *****************************************************************************
/* Function:
    int32_t leRectCompare(const leRect* l,
                          const leRect* r)

  Summary:
    Returns 1 if the two rectangles have the same position and dimensions

  Parameters:
    const leRect* l,
    const leRect* r
    
  Returns:
    int32_t
    
  Remarks:
    
*/
LIB_EXPORT int32_t leRectCompare(const leRect* l,
                                 const leRect* r);   

// *****************************************************************************
/* Function:
    leBool leRectsAreSimilar(const leRect* l,
                             const leRect* r)

  Summary:
    Returns LE_TRUE if the two rectangles are adjacent and vertically 
    or horizontally aligned

  Parameters:
    const leRect* l - the first rectangle
    const leRect* r - the second rectangle
    
  Returns:
    leBool
    
  Remarks:
    
*/
LIB_EXPORT leBool leRectsAreSimilar(const leRect* l,
                                      const leRect* r);                                                            

//DOM-IGNORE-BEGIN
static const leRect leRect_Zero = {0, 0, 0, 0};
//DOM-IGNORE-END
#endif /* LE_RECT_H */
