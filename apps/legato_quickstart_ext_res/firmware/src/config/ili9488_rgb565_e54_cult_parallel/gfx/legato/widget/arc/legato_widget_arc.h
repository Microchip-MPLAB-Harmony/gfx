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
    legato_widget_arc.h

  Summary:


  Description:
    This module implements arc drawing widget functions.
*******************************************************************************/


#ifndef LEGATO_WIDGET_ARC_H
#define LEGATO_WIDGET_ARC_H

#include "gfx/legato/common/legato_common.h"

#if LE_ARC_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
typedef struct leArcWidget leArcWidget;

#define LE_ARCWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t    (*getRadius)(const THIS_TYPE* _this); \
    leResult    (*setRadius)(THIS_TYPE* _this, uint32_t rad); \
    int32_t     (*getStartAngle)(const THIS_TYPE* _this); \
    leResult    (*setStartAngle)(THIS_TYPE* _this, int32_t ang); \
    uint32_t    (*getThickness)(const THIS_TYPE* _this); \
    leResult    (*setThickness)(THIS_TYPE* _this, uint32_t val); \
    int32_t     (*getCenterAngle)(const THIS_TYPE* _this); \
    leResult    (*setCenterAngle)(THIS_TYPE* _this, int32_t ang); \
    leBool      (*getRoundEdge)(const THIS_TYPE* _this); \
    leResult    (*setRoundEdge)(THIS_TYPE* _this, leBool rnd); \
    
typedef struct leArcWidgetVTable
{
	LE_ARCWIDGET_VTABLE(leArcWidget)
} leArcWidgetVTable; 

// *****************************************************************************
/* Structure:
    leArcWidget

  Summary:
    Implementation of a arc widget.

  Description:
    A arc widget draws a arc of the specified origin and radius inside
    the widget bounds.  All coordinates are expressed in local widget space.

    The color of the arc is determined by the widget scheme's 'foreground'
    color.

  Remarks:
    None.
*/
typedef struct leArcWidget
{
    leWidget widget; // base widget header
    
    const leArcWidgetVTable* fn;

    uint32_t radius; // the radius of the arc

    int32_t startAngle; //the start angle of the arc
    int32_t centerAngle; //the center angle of the arc
    uint32_t endAngle; //the end angle of the arc
    uint32_t thickness; //the thickness of the arc

    leBool roundEdge; //draws round edge if true.
} leArcWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leArcWidget* leArcWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the management of this memory until the
    widget is added to a widget tree.

  Description:


  Parameters:
    void

  Returns:
    leArcWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.

*/
LIB_EXPORT leArcWidget* leArcWidget_New();

/* Function:
    void leArcWidget_Constructor(leArcWidget* wgt)

  Summary:
    Initializes an leArcWidget widget pointer.

  Description:
    Initializes an leArcWidget widget pointer.

  Parameters:
    leArcWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leArcWidget_Constructor(leArcWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getRadius(const leArcWidget* _this)

  Summary:
     Gets the radius of the arc

  Description:
     Gets the radius of the arc

  Parameters:
    const leArcWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getRadius(_this);

  Returns:
    uint32_t - the arc radius
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setRadius(leArcWidget* _this,
                       uint32_t rad)

  Summary:
     Sets the radius of the arc

  Description:
     Sets the radius of the arc

  Parameters:
    leArcWidget* _this - The widget to operate on
    uint32_t rad - the arc radius

  Remarks:
    Usage - _this->fn->setRadius(_this, rad);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getStartAngle(const leArcWidget* _this)

  Summary:
     Gets the start angle of the arc

  Description:
     Gets the start angle of the arc

  Parameters:
    const leArcWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getStartAngle(_this);

  Returns:
    int32_t - the starting angle value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStartAngle(leArcWidget* _this,
                           int32_t ang)

  Summary:
     Sets the start angle of the arc

  Description:
     Sets the start angle of the arc

  Parameters:
    leArcWidget* _this - The widget to operate on
    int32_t ang - the starting angle value

  Remarks:
    Usage - _this->fn->setStartAngle(_this, ang);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getThickness(const leArcWidget* _this)

  Summary:
     Gets the thickness of the arc

  Description:
     Gets the thickness of the arc

  Parameters:
    const leArcWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getThickness(_this);

  Returns:
    uint32_t - the arc thickness
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setThickness(leArcWidget* _this,
                          uint32_t val)

  Summary:
     Sets the thickness of the arc

  Description:
     Sets the thickness of the arc

  Parameters:
    leArcWidget* _this - The widget to operate on
    uint32_t val - the arc thickness

  Remarks:
    Usage - _this->fn->setThickness(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getCenterAngle(const leArcWidget* _this)

  Summary:
     Gets the center angle of the arc

  Description:
     Gets the center angle of the arc

  Parameters:
    const leArcWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getCenterAngle(_this);

  Returns:
    int32_t - the center angle value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCenterAngle(leArcWidget* _this,
                            int32_t ang)

  Summary:
     Sets the center angle of the arc

  Description:
     Sets the center angle of the arc

  Parameters:
    leArcWidget* _this - The widget to operate on
    int32_t ang - the center angle value

  Remarks:
    Usage - _this->fn->setCenterAngle(_this, ang);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getRoundEdge(const leArcWidget* _this)

  Summary:
     Gets the rounded edge setting value

  Description:
     Gets the rounded edge setting value

  Parameters:
    const leArcWidget* _this - The widget to operate on

  Remarks:
    Usage - _this->fn->getRoundEdge(_this);

  Returns:
    leBool - the rounded edge value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setRoundEdge(leArcWidget* _this,
                          leBool rnd)

  Summary:
     Sets the arc to draw rounded edges

  Description:
     Sets the arc to draw rounded edges

  Parameters:
    leArcWidget* _this - The widget to operate on
    leBool rnd - the rounded edge value

  Remarks:
    Usage - _this->fn->setRoundEdge(_this, rnd);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_ARC_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_ARC_H */