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
    legato_widget_circle.h

  Summary:


  Description:
    This module implements circle drawing widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_CIRCLE_H
#define LEGATO_CIRCLE_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_CIRCLE_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
typedef struct leCircleWidget leCircleWidget;

#define LE_CIRCLEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    lePoint          (*getOrigin)(const THIS_TYPE* _this); \
    leResult         (*setOrigin)(THIS_TYPE* _this, const lePoint org); \
    int32_t          (*getOriginX)(const THIS_TYPE* _this); \
    leResult         (*setOriginX)(THIS_TYPE* _this, const int32_t x); \
    int32_t          (*getOriginY)(const THIS_TYPE* _this); \
    leResult         (*setOriginY)(THIS_TYPE* _this, const int32_t y); \
    uint32_t         (*getRadius)(const THIS_TYPE* _this); \
    leResult         (*setRadius)(THIS_TYPE* _this, uint32_t rad); \
    uint32_t         (*getThickness)(const THIS_TYPE* _this); \
    leResult         (*setThickness)(THIS_TYPE* _this, uint32_t thk); \
    leBool           (*getFilled)(const THIS_TYPE* _this); \
    leResult         (*setFilled)(THIS_TYPE* _this, leBool fill); \
    
typedef struct leCircleWidgetVTable
{
	LE_CIRCLEWIDGET_VTABLE(leCircleWidget)
} leCircleWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leCircleWidget

  Summary:
    Implementation of a circle widget.

  Description:
    A circle widget draws a circle of the specified origin and radius inside
    the widget bounds.  All coordinates are expressed in local widget space.

    The color of the circle is determined by the widget scheme's 'foreground'
    color.

  Remarks:
    None.
*/
typedef struct leCircleWidget
{
    leWidget widget; // base widget header

    const leCircleWidgetVTable* fn;

    int32_t x; // the origin x cooridnate
    int32_t y; // the origin y coordinate
    int32_t radius; // the radius of the circle
    int32_t thickness; // the thickness of the circle outline
    leBool filled; // fills the circle area
} leCircleWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leCircleWidget* leCircleWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Description:


  Parameters:
    void

  Returns:
    leCircleWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leCircleWidget* leCircleWidget_New();

/* Function:
    void leCircleWidget_Constructor(leCircleWidget* wgt)

  Summary:
    Initializes an leCircleWidget widget pointer.

  Description:
    Initializes an leCircleWidget widget pointer.

  Parameters:
    leCircleWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leCircleWidget_Constructor(leCircleWidget* cir);

// *****************************************************************************
/* Virtual Member Function:
    lePoint getOrigin(const leCircleWidget* _this)

  Summary:
     Gets the circle origin

  Description:
     Gets the circle origin

  Parameters:
    const leCircleWidget* _this - The circle to operate on

  Remarks:
    Usage - _this->fn->getOrigin(_this);

  Returns:
    lePoint - the origin
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setOrigin(leCircleWidget* _this,
                       const lePoint org)

  Summary:
     Sets the circle origin

  Description:
     Sets the circle origin

  Parameters:
    leCircleWidget* _this - The circle to operate on
    const lePoint org - the origin value

  Remarks:
    Usage - _this->fn->setOrigin(_this, org);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getOriginX(const leCircleWidget* _this)

  Summary:
     Gets the X value of the circle origin

  Description:
     Gets the X value of the circle origin

  Parameters:
    const leCircleWidget* _this - The circle to operate on

  Remarks:
    Usage - _this->fn->getOriginX(_this);

  Returns:
    int32_t - the origin X value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setOriginX(leCircleWidget* _this,
                        const int32_t x)

  Summary:
     Sets the X value of the circle origin

  Description:
     Sets the X value of the circle origin

  Parameters:
    leCircleWidget* _this - The circle to operate on
    const int32_t x - the origin X value

  Remarks:
    Usage - _this->fn->setOriginX(_this, x);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getOriginY(const leCircleWidget* _this)

  Summary:
     Gets the Y value of the circle origin

  Description:
     Gets the Y value of the circle origin

  Parameters:
    const leCircleWidget* _this - The circle to operate on

  Remarks:
    Usage - _this->fn->getOriginY(_this);

  Returns:
    int32_t - the origin Y value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setOriginY(leCircleWidget* _this,
                        const int32_t y)

  Summary:
     Sets the Y value of the circle origin

  Description:
     Sets the Y value of the circle origin

  Parameters:
    leCircleWidget* _this - The circle to operate on
    const int32_t y - the origin Y value

  Remarks:
    Usage - _this->fn->setOriginY(_this, y);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getRadius(const leCircleWidget* _this)

  Summary:
     Gets the circle radius

  Description:
     Gets the circle radius

  Parameters:
    const leCircleWidget* _this - The circle to operate on

  Remarks:
    Usage - _this->fn->getRadius(_this);

  Returns:
    uint32_t - the radius value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setRadius(leCircleWidget* _this,
                       uint32_t rad)

  Summary:
     Sets the circle radius

  Description:
     Sets the circle radius

  Parameters:
    leCircleWidget* _this - The circle to operate on
    uint32_t rad - the radius value

  Remarks:
    Usage - _this->fn->setRadius(_this, rad);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getThickness(const leCircleWidget* _this)

  Summary:
     Gets the circle thickness

  Description:
     Gets the circle thickness

  Parameters:
    const leCircleWidget* _this - The circle to operate on

  Remarks:
    Usage - _this->fn->getThickness(_this);

  Returns:
    uint32_t - the thickness value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setThickness(leCircleWidget* _this,
                          uint32_t thk)

  Summary:
     Sets the circle thickness

  Description:
     Sets the circle thickness

  Parameters:
    leCircleWidget* _this - The circle to operate on
    uint32_t thk - the thickness value

  Remarks:
    Usage - _this->fn->setThickness(_this, thk);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getFilled(const leCircleWidget* _this)

  Summary:
     Gets the filled setting

  Description:
     Gets the filled setting

  Parameters:
    const leCircleWidget* _this - The circle to operate on

  Remarks:
    Usage - _this->fn->getFilled(_this);

  Returns:
    leBool - the filled setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFilled(leCircleWidget* _this,
                       leBool fill)

  Summary:
     Sets the filled setting

  Description:
     Sets the filled setting

  Parameters:
    leCircleWidget* _this - The circle to operate on
    leBool fill - the filled setting

  Remarks:
    Usage - _this->fn->setFilled(_this, fill);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_CIRCLE_WIDGET_ENABLED
#endif /* LEGATO_CIRCLE_H */