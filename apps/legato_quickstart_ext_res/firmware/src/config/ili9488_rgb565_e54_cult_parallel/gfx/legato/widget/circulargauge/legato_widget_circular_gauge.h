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
    legato_widget_circular_gauge.h

  Summary:


  Description:
    This module implements circular gauge drawing widget functions.
*******************************************************************************/


#ifndef LEGATO_WIDGET_CIRCULAR_GAUGE_H
#define LEGATO_WIDGET_CIRCULAR_GAUGE_H

#include "gfx/legato/common/legato_common.h"

#if LE_CIRCULARGAUGE_WIDGET_ENABLED == 1

#include "gfx/legato/core/legato_scheme.h"
#include "gfx/legato/datastructure/legato_array.h"
#include "gfx/legato/string/legato_dynamicstring.h"
#include "gfx/legato/widget/legato_widget.h"


// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

typedef struct leCircularGaugeWidget leCircularGaugeWidget;


// *****************************************************************************
/* Structure:
    leCircularGaugeWidgetDir

  Summary:
    Direction of the gauge

  Description:
    Direction of the gauge

  Remarks:
    None.
*/
typedef enum leCircularGaugeWidgetLabelPosition
{
    CIRCULAR_GAUGE_LABEL_OUTSIDE,
    CIRCULAR_GAUGE_LABEL_INSIDE,
} leCircularGaugeWidgetLabelPosition;

// *****************************************************************************
/* Structure:
    leCircularGaugeWidgetArcType

  Summary:
    Type of arc

  Description:
    Type of arc

  Remarks:
    None.
*/
typedef enum leCircularGaugeWidgetArcType
{
    ANGLE_ARC,
    VALUE_ARC,
} leCircularGaugeWidgetArcType;

// *****************************************************************************
/* Function Pointer:
    leCircularGaugeWidget_ValueChangedEvent

  Summary:
    Value changed event function callback type
*/
typedef void (*leCircularGaugeWidget_ValueChangedEvent)(leCircularGaugeWidget*,
                                                        int32_t value);


// DOM-IGNORE-BEGIN

#define LE_CIRCULARGAUGEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t                 (*getRadius)(const THIS_TYPE* _this); \
    leResult                 (*setRadius)(THIS_TYPE* _this, uint32_t rad); \
    int32_t                  (*getStartAngle)(const THIS_TYPE* _this); \
    leResult                 (*setStartAngle)(THIS_TYPE* _this, int32_t ang); \
    int32_t                  (*getCenterAngle)(const THIS_TYPE* _this); \
    leResult                 (*setCenterAngle)(THIS_TYPE* _this, int32_t ang); \
    leResult                 (*addValueArc)(THIS_TYPE* _this, int32_t startVal, int32_t endVal, uint32_t rad, uint32_t thk, leScheme* schm); \
    leResult                 (*addAngularArc)(THIS_TYPE* _this, int32_t startAng, int32_t endAng, uint32_t rad, uint32_t thk, leScheme* schm); \
    leResult                 (*deleteArcs)(THIS_TYPE* _this); \
    leResult                 (*addMinorTicks)(THIS_TYPE* _this, int32_t startVal, int32_t endVal, uint32_t rad, uint32_t len, uint32_t interval, leScheme* schm); \
    leResult                 (*deleteMinorTicks)(THIS_TYPE* _this); \
    leResult                 (*addMinorTickLabels)(THIS_TYPE* _this, int32_t startVal, int32_t endVal, uint32_t rad, leCircularGaugeWidgetLabelPosition pos, uint32_t interval, leScheme* schm); \
    leResult                 (*deleteMinorTickLabels)(THIS_TYPE* _this); \
    int32_t                  (*getValue)(const THIS_TYPE* _this); \
    leResult                 (*setValue)(THIS_TYPE* _this, int32_t val); \
    int32_t                  (*getStartValue)(const THIS_TYPE* _this); \
    leResult                 (*setStartValue)(THIS_TYPE* _this, int32_t val); \
    int32_t                  (*getEndValue)(const THIS_TYPE* _this); \
    leResult                 (*setEndValue)(THIS_TYPE* _this, int32_t val); \
    leBool                   (*getTicksVisible)(const THIS_TYPE* _this); \
    leResult                 (*setTicksVisible)(THIS_TYPE* _this, leBool vis); \
    int32_t                  (*getTickValue)(const THIS_TYPE* _this); \
    leResult                 (*setTickValue)(THIS_TYPE* _this, int32_t val); \
    uint32_t                 (*getTickLength)(const THIS_TYPE* _this); \
    leResult                 (*setTickLength)(THIS_TYPE* _this, uint32_t len); \
    leBool                   (*getTickLabelsVisible)(const THIS_TYPE* _this); \
    leResult                 (*setTickLabelsVisible)(THIS_TYPE* _this, leBool vis); \
    leBool                   (*getHandVisible)(const THIS_TYPE* _this); \
    leResult                 (*setHandVisible)(THIS_TYPE* _this, leBool vis); \
    uint32_t                 (*getHandRadius)(const THIS_TYPE* _this); \
    leResult                 (*setHandRadius)(THIS_TYPE* _this, uint32_t rad); \
    leBool                   (*getCenterCircleVisible)(const THIS_TYPE* _this); \
    leResult                 (*setCenterCircleVisible)(THIS_TYPE* _this, leBool vis); \
    uint32_t                 (*getCenterCircleRadius)(const THIS_TYPE* _this); \
    leResult                 (*setCenterCircleRadius)(THIS_TYPE* _this, uint32_t rad); \
    uint32_t                 (*getCenterCircleThickness)(const THIS_TYPE* _this); \
    leResult                 (*setCenterCircleThickness)(THIS_TYPE* _this, uint32_t thk); \
    leResult                 (*setTickLabelFont)(THIS_TYPE* _this, const leFont* font); \
    leResult                 (*setValueChangedEventCallback)(THIS_TYPE* _this, leCircularGaugeWidget_ValueChangedEvent cb); \

typedef struct leCircularGaugeWidgetVTable
{
	LE_CIRCULARGAUGEWIDGET_VTABLE(leCircularGaugeWidget)
} leCircularGaugeWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leCircularGaugeWidget

  Summary:
    Implementation of a circular gauge widget.

  Description:
    A circular gauge widget draws a circular gauge of the specified properties
    inside the widget bounds.  All coordinates are expressed in local widget space.

  Remarks:
    None.
*/
typedef struct leCircularGaugeWidget
{
    leWidget widget; // base widget header

    const leCircularGaugeWidgetVTable* fn;

    //Widget properties
    int32_t value;
    int32_t startValue;
    int32_t endValue;
    uint32_t radius; // the radius of the circular gauge
    uint32_t startAngle; //the start angle of the outer arc
    int32_t centerAngle; //the center angle of the outer arc
    leRotationDirection dir; //the turn direction of the gauge

    //Tick properties
    leBool ticksVisible; // are ticks visible
    uint32_t tickLength; //length of ticks (towards center)
    int32_t tickValue; // tick value (delta)

    //Tick label properties
    leBool tickLabelsVisible; // are tick labels visible
    const leFont* ticksLabelFont; // ticks label font

    //hand properties
    leBool handVisible;
    uint32_t handRadius;
    leBool centerCircleVisible;
    uint32_t centerCircleRadius;
    uint32_t centerCircleThickness;

    //Advanced config arrays
    leArray arcsArray; //ArcsArray list
    leArray ticksArray; //ArcsArray list
    leArray labelsArray; //ArcsArray list


    leCircularGaugeWidget_ValueChangedEvent cb; //value changed event callback

} leCircularGaugeWidget;

// *****************************************************************************
/* Structure:
    leCircularGaugeArc

  Summary:
    Internal structure for the arcs in the circular gauge widget

  Description:
    Describs the arc instances in the circular gauge widget

  Remarks:
    None.
*/
typedef struct leCircularGaugeArc
{
    leCircularGaugeWidgetArcType type;
    int32_t startAngle;
    int32_t endAngle;
    int32_t startValue;
    int32_t endValue;
    uint32_t radius;
    uint32_t thickness;
    leScheme* scheme;
} leCircularGaugeArc;

// *****************************************************************************
/* Structure:
    leCircularGaugeTick

  Summary:
    Tick object for the circular gauge

  Description:
    Contains properties of the ticks in the gauge

  Remarks:
    None.
*/
typedef struct leCircularGaugeTick
{
    int32_t startValue;
    int32_t endValue;
    uint32_t interval;
    uint32_t radius;
    uint32_t length;
    leScheme* scheme;
} leCircularGaugeTick;

// *****************************************************************************
/* Structure:
    typedef struct leCircularGaugeLabel


  Summary:
    Label object for the circular gauge

  Description:
    Contains properties of the labels in the gauge

  Remarks:
    None.
*/
typedef struct leCircularGaugeLabel
{
    int32_t startValue;
    int32_t endValue;
    uint32_t interval;
    uint32_t radius;
    leCircularGaugeWidgetLabelPosition position;
    leScheme* scheme;
} leCircularGaugeLabel;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leCircularGaugeWidget* leCircularGaugeWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the management of this memory until the
    widget is added to a widget tree.

  Description:


  Parameters:
    void

  Returns:
    leCircularGaugeWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leCircularGaugeWidget* leCircularGaugeWidget_New();


/* Function:
    void leCircularGaugeWidget_Constructor(leCircularGaugeWidget* wgt)

  Summary:
    Initializes an leCircularGaugeWidget widget pointer.

  Description:
    Initializes an leCircularGaugeWidget widget pointer.

  Parameters:
    leCircularGaugeWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leCircularGaugeWidget_Constructor(leCircularGaugeWidget* gauge);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getRadius(const leCircularGaugeWidget* _this)

  Summary:
     Gets the radius

  Description:
     Gets the radius

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getRadius(_this);

  Returns:
    uint32_t - the radius
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setRadius(leCircularGaugeWidget* _this,
                       uint32_t rad)

  Summary:
     Sets the radius

  Description:
     Sets the radius

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    uint32_t rad - the radius value

  Remarks:
    Usage - _this->fn->setRadius(_this, rad);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getStartAngle(const leCircularGaugeWidget* _this)

  Summary:
     Gets the start angle

  Description:
     Gets the start angle

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getStartAngle(_this);

  Returns:
    int32_t - the start angle
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStartAngle(leCircularGaugeWidget* _this,
                           int32_t ang)

  Summary:
     Sets the start angle

  Description:
     Sets the start angle

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t ang - the angle value

  Remarks:
    Usage - _this->fn->setStartAngle(_this, ang);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getCenterAngle(const leCircularGaugeWidget* _this)

  Summary:
     Gets the center angle

  Description:
     Gets the center angle

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getCenterAngle(_this);

  Returns:
    int32_t - the center angle
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCenterAngle(leCircularGaugeWidget* _this,
                            int32_t ang)

  Summary:
     Sets the center angle

  Description:
     Sets the center angle

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t ang - the angle value

  Remarks:
    Usage - _this->fn->setCenterAngle(_this, ang);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult addValueArc(leCircularGaugeWidget* _this,
                         int32_t startVal,
                         int32_t endVal,
                         uint32_t rad,
                         uint32_t thk,
                         leScheme* schm)

  Summary:
     Adds a value arc to the gauge

  Description:
     Adds a value arc to the gauge

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t startVal - the start value
    int32_t endVal - the end value
    uint32_t rad - the radius value
    uint32_t thk - the thickness value
    leScheme* schm - the scheme pointer

  Remarks:
    Usage - _this->fn->addValueArc(_this, startVal, endVal, rad, thk, schm);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult addAngularArc(leCircularGaugeWidget* _this,
                           int32_t startAng,
                           int32_t endAng,
                           uint32_t rad,
                           uint32_t thk,
                           leScheme* schm)

  Summary:
     Adds an angular arc to the gauge

  Description:
     Adds an angular arc to the gauge

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t startAng - the start angle value
    int32_t endAng - the end angle value
    uint32_t rad - the radius value
    uint32_t thk - the thickness value
    leScheme* schm - the scheme pointer

  Remarks:
    Usage - _this->fn->addAngularArc(_this, startAng, endAng, rad, thk, schm);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult deleteArcs(leCircularGaugeWidget* _this)

  Summary:
     Deletes all arcs from the gauge

  Description:
     Deletes all arcs from the gauge

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->deleteArcs(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult addMinorTicks(leCircularGaugeWidget* _this,
                           int32_t startVal,
                           int32_t endVal,
                           uint32_t rad,
                           uint32_t len,
                           uint32_t interval,
                           leScheme* schm)

  Summary:
     Adds a series of minor ticks

  Description:
     Adds a series of minor ticks

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t startVal - the start value
    int32_t endVal - the end value
    uint32_t rad - the radius value
    uint32_t len - the length value
    uint32_t interval - the value
    leScheme* schm - the scheme pointer

  Remarks:
    Usage - _this->fn->addMinorTicks(_this, startVal, endVal, rad, len, interval, schm);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult deleteMinorTicks(leCircularGaugeWidget* _this)

  Summary:
     Deletes all minor ticks

  Description:
     Deletes all minor ticks

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->deleteMinorTicks(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult addMinorTickLabels(leCircularGaugeWidget* _this,
                                int32_t startVal,
                                int32_t endVal,
                                uint32_t rad,
                                leCircularGaugeWidgetLabelPosition pos,
                                uint32_t interval,
                                leScheme* schm)

  Summary:
     Adds minor tick labels

  Description:
     Adds minor tick labels

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t startVal - the start value
    int32_t endVal - the end value
    uint32_t rad - the radius value
    leCircularGaugeWidgetLabelPosition pos -
    uint32_t interval - the value
    leScheme* schm - the scheme pointer

  Remarks:
    Usage - _this->fn->addMinorTickLabels(_this, startVal, endVal, rad, pos, interval, schm);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult deleteMinorTickLabels(leCircularGaugeWidget* _this)

  Summary:
     Deletes all minor tick labels

  Description:
     Deletes all minor tick labels

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->deleteMinorTickLabels(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getValue(const leCircularGaugeWidget* _this)

  Summary:
     Gets the gauge value

  Description:
     Gets the gauge value

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getValue(_this);

  Returns:
    int32_t - the gauge value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValue(leCircularGaugeWidget* _this,
                      int32_t val)

  Summary:
     Sets the gauge value

  Description:
     Sets the gauge value

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t val - the value

  Remarks:
    Usage - _this->fn->setValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getStartValue(const leCircularGaugeWidget* _this)

  Summary:
     Gets the start value

  Description:
     Gets the start value

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getStartValue(_this);

  Returns:
    int32_t - the start value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStartValue(leCircularGaugeWidget* _this,
                           int32_t val)

  Summary:
     Sets the start value

  Description:
     Sets the start value

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t val - the value

  Remarks:
    Usage - _this->fn->setStartValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getEndValue(const leCircularGaugeWidget* _this)

  Summary:
     Gets the end value

  Description:
     Gets the end value

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getEndValue(_this);

  Returns:
    int32_t - the end value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setEndValue(leCircularGaugeWidget* _this,
                         int32_t val)

  Summary:
     Sets the end value

  Description:
     Sets the end value

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t val - the value

  Remarks:
    Usage - _this->fn->setEndValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getTicksVisible(const leCircularGaugeWidget* _this)

  Summary:
     Gets the ticks visible setting

  Description:
     Gets the ticks visible setting

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getTicksVisible(_this);

  Returns:
    leBool - the visible value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTicksVisible(leCircularGaugeWidget* _this,
                             leBool vis)

  Summary:
     Sets the ticks visible setting

  Description:
     Sets the ticks visible setting

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setTicksVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getTickValue(const leCircularGaugeWidget* _this)

  Summary:
     Gets the current tick value

  Description:
     Gets the current tick value

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getTickValue(_this);

  Returns:
    int32_t - the tick value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTickValue(leCircularGaugeWidget* _this,
                          int32_t val)

  Summary:
     Sets the tick value

  Description:
     Sets the tick value

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    int32_t val - the value

  Remarks:
    Usage - _this->fn->setTickValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getTickLength(const leCircularGaugeWidget* _this)

  Summary:
     Gets the current tick length

  Description:
     Gets the current tick length

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getTickLength(_this);

  Returns:
    uint32_t - the tick length
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTickLength(leCircularGaugeWidget* _this,
                           uint32_t len)

  Summary:
     Sets the tick length

  Description:
     Sets the tick length

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    uint32_t len - the length value

  Remarks:
    Usage - _this->fn->setTickLength(_this, len);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getTickLabelsVisible(const leCircularGaugeWidget* _this)

  Summary:
     Gets the visibility setting for the tick labels

  Description:
     Gets the visibility setting for the tick labels

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getTickLabelsVisible(_this);

  Returns:
    leBool - the visible setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTickLabelsVisible(leCircularGaugeWidget* _this,
                                  leBool vis)

  Summary:
     Sets the visibility setting for the tick labels

  Description:
     Sets the visibility setting for the tick labels

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setTickLabelsVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getHandVisible(const leCircularGaugeWidget* _this)

  Summary:
     Gets hand visibility setting

  Description:
     Gets hand visibility setting

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getHandVisible(_this);

  Returns:
    leBool - the visible setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setHandVisible(leCircularGaugeWidget* _this,
                            leBool vis)

  Summary:
     Sets the hand visibility setting

  Description:
     Sets the hand visibility setting

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setHandVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getHandRadius(const leCircularGaugeWidget* _this)

  Summary:
     Gets the hand radius

  Description:
     Gets the hand radius

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getHandRadius(_this);

  Returns:
    uint32_t - the radius
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setHandRadius(leCircularGaugeWidget* _this,
                           uint32_t rad)

  Summary:
     Sets the hand radius

  Description:
     Sets the hand radius

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    uint32_t rad - the radius value

  Remarks:
    Usage - _this->fn->setHandRadius(_this, rad);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getCenterCircleVisible(const leCircularGaugeWidget* _this)

  Summary:
     Gets the center circle visibility setting

  Description:
     Gets the center circle visibility setting

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getCenterCircleVisible(_this);

  Returns:
    leBool - the visible setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCenterCircleVisible(leCircularGaugeWidget* _this,
                                    leBool vis)

  Summary:
     Sets the center circle visibility setting

  Description:
     Sets the center circle visibility setting

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setCenterCircleVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getCenterCircleRadius(const leCircularGaugeWidget* _this)

  Summary:
     Gets the center circle radius

  Description:
     Gets the center circle radius

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getCenterCircleRadius(_this);

  Returns:
    uint32_t - the radius
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCenterCircleRadius(leCircularGaugeWidget* _this,
                                   uint32_t rad)

  Summary:
     Sets the center circle radius

  Description:
     Sets the center circle radius

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    uint32_t rad - the radius value

  Remarks:
    Usage - _this->fn->setCenterCircleRadius(_this, rad);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getCenterCircleThickness(const leCircularGaugeWidget* _this)

  Summary:
     Gets the center circle thickness

  Description:
     Gets the center circle thickness

  Parameters:
    const leCircularGaugeWidget* _this - The gauge to operate on

  Remarks:
    Usage - _this->fn->getCenterCircleThickness(_this);

  Returns:
    uint32_t - the thickness value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCenterCircleThickness(leCircularGaugeWidget* _this,
                                      uint32_t thk)

  Summary:
     Sets the center circle thickness

  Description:
     Sets the center circle thickness

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    uint32_t thk - the thickness value

  Remarks:
    Usage - _this->fn->setCenterCircleThickness(_this, thk);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTickLabelFont(leCircularGaugeWidget* _this,
                              const leFont* font)

  Summary:
     Sets the tick label font

  Description:
     Sets the tick label font

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    const leFont* font - the font pointer

  Remarks:
    Usage - _this->fn->setTickLabelFont(_this, font);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueChangedEventCallback(leCircularGaugeWidget* _this,
                                          leCircularGaugeWidget_ValueChangedEvent cb)

  Summary:
     Sets the value changed callback

  Description:
     Sets the value changed callback

  Parameters:
    leCircularGaugeWidget* _this - The gauge to operate on
    leCircularGaugeWidget_ValueChangedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setValueChangedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/


#endif // LE_CIRCULARGAUGE_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_CIRCULAR_GAUGE_H */