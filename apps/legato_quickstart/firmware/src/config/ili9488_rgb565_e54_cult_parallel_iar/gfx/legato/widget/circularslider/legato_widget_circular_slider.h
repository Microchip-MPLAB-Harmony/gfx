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
    legato_widget_circular_slider.h

  Summary:


  Description:
    This module implements slider drawing widget functions.
*******************************************************************************/


#ifndef LEGATO_WIDGET_CIRCULAR_SLIDER_H
#define LEGATO_WIDGET_CIRCULAR_SLIDER_H

#include "gfx/legato/common/legato_common.h"

#if LE_CIRCULARSLIDER_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Structure:
    leCircularSliderButtonState

  Summary:
    State of the slider button

  Description:
    State of the slider button

  Remarks:
    None.
*/
typedef enum leCircularSliderButtonState
{
    LE_CIRCULAR_SLIDER_STATE_UP,
    LE_CIRCULAR_SLIDER_STATE_DOWN,
} leCircularSliderButtonState;

// *****************************************************************************
/* Enumeration:
    leCircularSliderWidgetArcType

  Summary:
    The arcs that compose the circular slider

  Description:
    The arcs that compose the circular slider

  Remarks:
    None.
*/
typedef enum leCircularSliderWidgetArcType
{
    OUTSIDE_CIRCLE_BORDER,
    INSIDE_CIRCLE_BORDER,
    ACTIVE_AREA,
    INACTIVE_AREA,
    CIRCLE_BUTTON,
} leCircularSliderWidgetArcType;

// *****************************************************************************
/* Structure:
    leCircularSliderArc

  Summary:
    Internal structure for the arcs in the circular slider widget

  Description:
    Describs the arc instances in the circular gauge widget

  Remarks:
    None.
*/
typedef struct leCircularSliderArc
{
    leBool visible;
    int32_t startAngle;
    int32_t centerAngle;
    uint32_t radius;
    uint32_t thickness;
    const leScheme* scheme;
} leCircularSliderArc;

typedef struct leCircularSliderWidget leCircularSliderWidget;

// *****************************************************************************
/* Function Pointer:
    leCircularSliderWidget_PressedEvent

  Summary:
    Button pressed event function callback type
*/
typedef void (*leCircularSliderWidget_PressedEvent)(leCircularSliderWidget *, int32_t);

// *****************************************************************************
/* Function Pointer:
    leCircularSliderWidget_ValueChangedEvent

  Summary:
    Value changed event function callback type
*/
typedef void (*leCircularSliderWidget_ValueChangedEvent)(leCircularSliderWidget *, int32_t);

// *****************************************************************************
/* Function Pointer:
    leCircularSliderWidget_ReleasedEvent

  Summary:
    Button released event function callback type
*/
typedef void (*leCircularSliderWidget_ReleasedEvent)(leCircularSliderWidget *, int32_t);

// DOM-IGNORE-BEGIN

#define LE_CIRCULARSLIDERWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t                 (*getRadius)(const THIS_TYPE* _this); \
    leResult                 (*setRadius)(THIS_TYPE* _this, uint32_t rad); \
    uint32_t                 (*getStartAngle)(const THIS_TYPE* _this); \
    leResult                 (*setStartAngle)(THIS_TYPE* _this, uint32_t ang); \
    uint32_t                 (*getArcThickness)(const THIS_TYPE* _this, leCircularSliderWidgetArcType type); \
    leResult                 (*setArcThickness)(THIS_TYPE* _this, leCircularSliderWidgetArcType type, uint32_t thck); \
    uint32_t                 (*getArcRadius)(const THIS_TYPE* _this, leCircularSliderWidgetArcType type); \
    leResult                 (*setArcRadius)(THIS_TYPE* _this, leCircularSliderWidgetArcType type, uint32_t rad); \
    leScheme*                (*getArcScheme)(const THIS_TYPE* _this, leCircularSliderWidgetArcType type); \
    leResult                 (*setArcScheme)(THIS_TYPE* _this, leCircularSliderWidgetArcType type, const leScheme* schm); \
    leBool                   (*getArcVisible)(const THIS_TYPE* _this, leCircularSliderWidgetArcType type); \
    leResult                 (*setArcVisible)(THIS_TYPE* _this, leCircularSliderWidgetArcType type, leBool vis); \
    uint32_t                 (*getStartValue)(const THIS_TYPE* _this); \
    leResult                 (*setStartValue)(THIS_TYPE* _this, uint32_t val); \
    uint32_t                 (*getEndValue)(const THIS_TYPE* _this); \
    leResult                 (*setEndValue)(THIS_TYPE* _this, uint32_t val); \
    uint32_t                 (*getValue)(const THIS_TYPE* _this); \
    leResult                 (*setValue)(THIS_TYPE* _this, uint32_t val); \
    leBool                   (*getRoundEdges)(const THIS_TYPE* _this); \
    leResult                 (*setRoundEdges)(THIS_TYPE* _this, leBool rnd); \
    leBool                   (*getStickyButton)(const THIS_TYPE* _this); \
    leResult                 (*setStickyButton)(THIS_TYPE* _this, leBool stk); \
    leBool                   (*getTouchOnButtonOnly)(const THIS_TYPE* _this); \
    leResult                 (*setTouchOnButtonOnly)(THIS_TYPE* _this, leBool tch); \
    leRotationDirection      (*getDirection)(const THIS_TYPE* _this); \
    leResult                 (*setDirection)(THIS_TYPE* _this, leRotationDirection dir); \
    leResult                 (*setPressedEventCallback)(THIS_TYPE* _this, leCircularSliderWidget_PressedEvent cb); \
    leResult                 (*setValueChangedEventCallback)(THIS_TYPE* _this, leCircularSliderWidget_ValueChangedEvent cb); \
    leResult                 (*setReleasedEventCallback)(THIS_TYPE* _this, leCircularSliderWidget_ReleasedEvent cb); \

typedef struct leCircularSliderWidgetVTable
{
	LE_CIRCULARSLIDERWIDGET_VTABLE(leCircularSliderWidget)
} leCircularSliderWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leCircularSliderWidget

  Summary:
    Implementation of a slider widget.

  Description:
    A slider widget draws a slider of the specified origin and radius inside
    the widget bounds.  All coordinates are expressed in local widget space.

    The color of the slider is determined by the widget scheme's 'foreground'
    color.

  Remarks:
    None.
*/
typedef struct leCircularSliderWidget
{
    leWidget widget; // base widget header

    const leCircularSliderWidgetVTable* fn;

    uint32_t radius; // the radius of the slider

    uint32_t startAngle; //the start angle of the slider

    uint32_t value;      //the value of the slider
    uint32_t startValue; //the start value of the slider
    uint32_t endValue;   //the end value of the slider
    float degPerUnit;   //degrees per unit in the slider

    leBool roundEdges; //round edges
    leBool sticky; //snaps to start value before wrapping around
    leBool buttonTouch; //only button is active to touch

    leRotationDirection direction;    //the direction of the slider

    leCircularSliderArc activeArc;            //the arc for the slider value
    leCircularSliderArc inActiveArc;          //the arc for the slider remainder
    leCircularSliderArc outsideBorderArc;     //the arc for the outside border
    leCircularSliderArc insideBorderArc;      //the arc for the inside border
    leCircularSliderArc circleButtonArc;      //the arc for the circle button

    leCircularSliderButtonState btnState;   //the state of the circular slider button

    leCircularSliderWidget_PressedEvent pressedCallback;
    leCircularSliderWidget_ValueChangedEvent valueChangedCallback;
    leCircularSliderWidget_ReleasedEvent releasedCallback;
} leCircularSliderWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leCircularSliderWidget* leCircularSliderWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the management of this memory until the
    widget is added to a widget tree.

  Description:


  Parameters:
    void

  Returns:
    leCircularSliderWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leCircularSliderWidget* leCircularSliderWidget_New();

/* Function:
    void leCircularSliderWidget_Constructor(leCircularSliderWidget* wgt)

  Summary:
    Initializes an leCircularSliderWidget widget pointer.

  Description:
    Initializes an leCircularSliderWidget widget pointer.

  Parameters:
    leCircularSliderWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leCircularSliderWidget_Constructor(leCircularSliderWidget* slider);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getRadius(const leCircularSliderWidget* _this)

  Summary:
     Gets the radius value

  Description:
     Gets the radius value

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on

  Remarks:
    Usage - _this->fn->getRadius(_this);

  Returns:
    uint32_t - the radius value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setRadius(leCircularSliderWidget* _this,
                       uint32_t rad)

  Summary:
     Sets the radius value

  Description:
     Sets the radius value

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    uint32_t rad - the radius value

  Remarks:
    Usage - _this->fn->setRadius(_this, rad);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getStartAngle(const leCircularSliderWidget* _this)

  Summary:
     Gets the start angle

  Description:
     Gets the start angle

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on

  Remarks:
    Usage - _this->fn->getStartAngle(_this);

  Returns:
    uint32_t - the angle value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStartAngle(leCircularSliderWidget* _this,
                           uint32_t ang)

  Summary:
     Sets the start angle

  Description:
     Sets the start angle

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    uint32_t ang - the angle value

  Remarks:
    Usage - _this->fn->setStartAngle(_this, ang);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getArcThickness(const leCircularSliderWidget* _this,
                             leCircularSliderWidgetArcType type)

  Summary:
     Gets the arc thickness value

  Description:
     Gets the arc thickness value

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidgetArcType type - the type

  Remarks:
    Usage - _this->fn->getArcThickness(_this, type);

  Returns:
    uint32_t - the thickness value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setArcThickness(leCircularSliderWidget* _this,
                             leCircularSliderWidgetArcType type,
                             uint32_t thck)

  Summary:
     Sets the arc thickness value

  Description:
     Sets the arc thickness value

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidgetArcType type - the type
    uint32_t thck - the thickness value

  Remarks:
    Usage - _this->fn->setArcThickness(_this, type, thck);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getArcRadius(const leCircularSliderWidget* _this,
                          leCircularSliderWidgetArcType type)

  Summary:
     Gets the arc radius

  Description:
     Gets the arc radius

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidgetArcType type - the type

  Remarks:
    Usage - _this->fn->getArcRadius(_this, type);

  Returns:
    uint32_t - the radius value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setArcRadius(leCircularSliderWidget* _this,
                          leCircularSliderWidgetArcType type,
                          uint32_t rad)

  Summary:
     Sets the arc radius

  Description:
     Sets the arc radius

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidgetArcType type - the type
    uint32_t rad - the radius value

  Remarks:
    Usage - _this->fn->setArcRadius(_this, type, rad);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leScheme* getArcScheme(const leCircularSliderWidget* _this,
                           leCircularSliderWidgetArcType type)

  Summary:
     Gets the arc scheme

  Description:
     Gets the arc scheme

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidgetArcType type - the type

  Remarks:
    Usage - _this->fn->getArcScheme(_this, type);

  Returns:
    leScheme* - the scheme pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setArcScheme(leCircularSliderWidget* _this,
                          leCircularSliderWidgetArcType type,
                          const leScheme* schm)

  Summary:
     Sets the arc scheme

  Description:
     Sets the arc scheme

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidgetArcType type - the type
    const leScheme* schm - the scheme pointer

  Remarks:
    Usage - _this->fn->setArcScheme(_this, type, schm);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getArcVisible(const leCircularSliderWidget* _this,
                         leCircularSliderWidgetArcType type)

  Summary:
     Gets the arc visibility setting

  Description:
     Gets the arc visibility setting

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidgetArcType type - the type

  Remarks:
    Usage - _this->fn->getArcVisible(_this, type);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setArcVisible(leCircularSliderWidget* _this,
                           leCircularSliderWidgetArcType type,
                           leBool vis)

  Summary:
     Sets the arc visibility setting

  Description:
     Sets the arc visibility setting

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidgetArcType type - the type
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setArcVisible(_this, type, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getStartValue(const leCircularSliderWidget* _this)

  Summary:
     Gets the start value

  Description:
     Gets the start value

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on

  Remarks:
    Usage - _this->fn->getStartValue(_this);

  Returns:
    uint32_t - the start value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStartValue(leCircularSliderWidget* _this,
                           uint32_t val)

  Summary:
     Sets the start value

  Description:
     Sets the start value

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setStartValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getEndValue(const leCircularSliderWidget* _this)

  Summary:
     Gets the end value

  Description:
     Gets the end value

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on

  Remarks:
    Usage - _this->fn->getEndValue(_this);

  Returns:
    uint32_t - the end value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setEndValue(leCircularSliderWidget* _this,
                         uint32_t val)

  Summary:
     Sets the end value

  Description:
     Sets the end value

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setEndValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getValue(const leCircularSliderWidget* _this)

  Summary:
     Gets the current value

  Description:
     Gets the current value

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on

  Remarks:
    Usage - _this->fn->getValue(_this);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValue(leCircularSliderWidget* _this,
                      uint32_t val)

  Summary:
     Sets the current value

  Description:
     Sets the current value

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getRoundEdges(const leCircularSliderWidget* _this)

  Summary:
     Gets the rounded edge setting

  Description:
     Gets the rounded edge setting

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on

  Remarks:
    Usage - _this->fn->getRoundEdges(_this);

  Returns:
    leBool -
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setRoundEdges(leCircularSliderWidget* _this,
                           leBool rnd)

  Summary:
     Sets the rounded edge setting

  Description:
     Sets the rounded edge setting

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leBool rnd -

  Remarks:
    Usage - _this->fn->setRoundEdges(_this, rnd);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getStickyButton(const leCircularSliderWidget* _this)

  Summary:
     Gets the sticky button setting

  Description:
     Gets the sticky button setting

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on

  Remarks:
    Usage - _this->fn->getStickyButton(_this);

  Returns:
    leBool -
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStickyButton(leCircularSliderWidget* _this,
                             leBool stk)

  Summary:
     Sets the sticky button setting

  Description:
     Sets the sticky button setting

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leBool stk -

  Remarks:
    Usage - _this->fn->setStickyButton(_this, stk);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getTouchOnButtonOnly(const leCircularSliderWidget* _this)

  Summary:
     Gets the touch on button setting

  Description:
     Gets the touch on button setting

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on

  Remarks:
    Usage - _this->fn->getTouchOnButtonOnly(_this);

  Returns:
    leBool -
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTouchOnButtonOnly(leCircularSliderWidget* _this,
                                  leBool tch)

  Summary:
     Sets the touch on button setting

  Description:
     Sets the touch on button setting

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leBool tch -

  Remarks:
    Usage - _this->fn->setTouchOnButtonOnly(_this, tch);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leRotationDirection getDirection(const leCircularSliderWidget* _this)

  Summary:
     Gets the current direction

  Description:
     Gets the current direction

  Parameters:
    const leCircularSliderWidget* _this - The slider to operate on

  Remarks:
    Usage - _this->fn->getDirection(_this);

  Returns:
    leRotationDirection - the direction
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setDirection(leCircularSliderWidget* _this,
                          leRotationDirection dir)

  Summary:
     Sets the current direction

  Description:
     Sets the current direction

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leRotationDirection dir - the direction

  Remarks:
    Usage - _this->fn->setDirection(_this, dir);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setPressedEventCallback(leCircularSliderWidget* _this,
                                     leCircularSliderWidget_PressedEvent cb)

  Summary:
     Sets the pressed event callback

  Description:
     Sets the pressed event callback

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidget_PressedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setPressedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueChangedEventCallback(leCircularSliderWidget* _this,
                                          leCircularSliderWidget_ValueChangedEvent cb)

  Summary:
     Sets the value changed event callback

  Description:
     Sets the value changed event callback

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidget_ValueChangedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setValueChangedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setReleasedEventCallback(leCircularSliderWidget* _this,
                                      leCircularSliderWidget_ReleasedEvent cb)

  Summary:
     Sets the released event callback

  Description:
     Sets the released event callback

  Parameters:
    leCircularSliderWidget* _this - The slider to operate on
    leCircularSliderWidget_ReleasedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setReleasedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_CIRCULARSLIDER_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_CIRCULAR_SLIDER_H */
