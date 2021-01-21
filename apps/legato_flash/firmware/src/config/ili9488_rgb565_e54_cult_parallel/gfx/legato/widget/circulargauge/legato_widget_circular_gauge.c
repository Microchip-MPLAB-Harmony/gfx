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


#include "gfx/legato/widget/arc/legato_widget_arc.h"
#include "gfx/legato/widget/circulargauge/legato_widget_circular_gauge.h"

#if LE_CIRCULARGAUGE_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/common/legato_math.h"

#define DEFAULT_WIDTH           101
#define DEFAULT_HEIGHT          101

#define DEFAULT_ORIGIN_X        50
#define DEFAULT_ORIGIN_Y        50

#define DEFAULT_RADIUS          50
#define DEFAULT_START_ANGLE     225
#define DEFAULT_CENTER_ANGLE    -270
#define DEFAULT_START_VALUE     0
#define DEFAULT_END_VALUE       100
#define DEFAULT_VALUE           50
#define DEFAULT_TICK_VALUE      5
#define DEFAULT_TICK_LENGTH     5
#define DEFAULT_HAND_LENGTH     40

#define DEFAULT_CENTER_CIRCLE_RADIUS 5
#define DEFAULT_CENTER_CIRCLE_THICKNESS 2

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leCircularGaugeWidgetVTable circularGaugeWidgetVTable;

//Invalidates the quadrant where needle is located
void invalidateHandQuadrant(const leCircularGaugeWidget* gauge)
{
    leRect damagedRect;
    
    int32_t handAngleOffset = 0, handAbsAngle;
    int32_t unitOffset = 0;
    float degPerUnit;
    lePoint p = {0};
    
    leUtils_PointToScreenSpace((leWidget*)gauge, &p);

    unitOffset = gauge->value - gauge->startValue;
    degPerUnit = (float) gauge->centerAngle / (float) (leAbsoluteValue(gauge->endValue - gauge->startValue));
    handAngleOffset = (int32_t) ((float) unitOffset * degPerUnit);

    handAbsAngle = gauge->startAngle + handAngleOffset;
    
    while(handAbsAngle < 0)
    {
        handAbsAngle += 360;
    }

    if (handAbsAngle >= 360)
    {
        handAbsAngle %= 360;
    }
    
    if (handAbsAngle > 270)
    {
        //Invalidate Q4
        damagedRect.width = gauge->fn->getWidth(gauge) / 2;
        damagedRect.height = gauge->fn->getHeight(gauge) / 2;
        
        damagedRect.x = p.x + damagedRect.width;
        damagedRect.y = p.y + damagedRect.height;
    }
    else if (handAbsAngle == 270)
    {
        //Invalidate Q3 & Q4
        damagedRect.width = gauge->fn->getWidth(gauge);
        damagedRect.height = gauge->fn->getHeight(gauge) / 2;
        
        damagedRect.x = p.x;
        damagedRect.y = p.y + damagedRect.height;
    }
    else if (handAbsAngle > 180)
    {
        //Invalidate Q3
        damagedRect.width = gauge->fn->getWidth(gauge) / 2;
        damagedRect.height = gauge->fn->getHeight(gauge) / 2;
        
        damagedRect.x = p.x;
        damagedRect.y = p.y + damagedRect.height;
    }
    else if (handAbsAngle == 180)
    {
        //Invalidate Q2 & Q3
        damagedRect.width = gauge->fn->getWidth(gauge) / 2;
        damagedRect.height = gauge->fn->getHeight(gauge);
        
        damagedRect.x = p.x;
        damagedRect.y = p.y;
    }
    else if (handAbsAngle > 90)
    {
        //Invalidate Q2
        damagedRect.width = gauge->fn->getWidth(gauge) / 2;
        damagedRect.height = gauge->fn->getHeight(gauge) / 2;
        
        damagedRect.x = p.x;
        damagedRect.y = p.y;
    }
    else if (handAbsAngle == 90)
    {
        //Invalidate Q1 & Q2
        damagedRect.width = gauge->fn->getWidth(gauge);
        damagedRect.height = gauge->fn->getHeight(gauge) / 2;
        
        damagedRect.x = p.x;
        damagedRect.y = p.y;        
    }
    else if (handAbsAngle > 0)
    {
        //Invalidate Q1
        damagedRect.width = gauge->fn->getWidth(gauge) / 2;
        damagedRect.height = gauge->fn->getHeight(gauge) / 2;
        
        damagedRect.x = p.x + damagedRect.width;
        damagedRect.y = p.y;           
    }
    else if (handAbsAngle == 0)
    {
        //Invalidate Q4 & Q1
        damagedRect.width = gauge->fn->getWidth(gauge) / 2;
        damagedRect.height = gauge->fn->getHeight(gauge);
        
        damagedRect.x = p.x + damagedRect.width;
        damagedRect.y = p.y;  
    }
        
    gauge->fn->_damageArea(gauge, &damagedRect);
}

//Invalidates the needle rectangle
static void invalidateHandRect(const leCircularGaugeWidget* gauge)
{
    leRect drawRect;
    
    lePoint outerPoint;
    int32_t handAngleOffset = 0, handAbsAngle;
    int32_t unitOffset = 0;
    float degPerUnit;
    lePoint p;
    
    p.x = gauge->fn->getWidth(gauge) / 2;
    p.y = gauge->fn->getHeight(gauge) / 2;
    
    leUtils_PointToScreenSpace((leWidget*)gauge, &p);

    unitOffset = gauge->value - gauge->startValue;
    degPerUnit = (float) gauge->centerAngle / (float) (leAbsoluteValue(gauge->endValue - gauge->startValue));
    handAngleOffset = (int32_t) ((float) unitOffset * degPerUnit);

    handAbsAngle = gauge->startAngle + handAngleOffset;
    
    while(handAbsAngle < 0)
    {
        handAbsAngle += 360;
    }

    if (handAbsAngle >= 360)
    {
        handAbsAngle %= 360;
    }
    
    lePolarToXY(gauge->handRadius, handAbsAngle, &outerPoint);
  
    if (p.x > (outerPoint.x + p.x))
    {
        drawRect.x = (outerPoint.x + p.x);
        drawRect.width = -outerPoint.x;
    }
    else 
    {
        drawRect.x = p.x;
        drawRect.width = outerPoint.x;
    }
    
    if (p.y > p.y - outerPoint.y)
    {
        drawRect.y = p.y - outerPoint.y;
        drawRect.height = outerPoint.y;
    }
    else 
    {
        drawRect.y = p.y;
        drawRect.height = -outerPoint.y;
    }
    
    drawRect.x -= 2;
    drawRect.y -= 2;
    drawRect.width += 4;
    drawRect.height += 4;
    
    gauge->fn->_damageArea(gauge, &drawRect);
}

void leCircularGaugeWidget_Constructor(leCircularGaugeWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&circularGaugeWidgetVTable;
    _this->fn = &circularGaugeWidgetVTable;

    _this->widget.type = LE_WIDGET_CIRCULAR_GAUGE;
    
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.backgroundType = LE_WIDGET_BACKGROUND_NONE;

    // default gauge values
    _this->value = DEFAULT_VALUE;
    _this->radius = DEFAULT_RADIUS;
    _this->startValue = DEFAULT_START_VALUE;
    _this->endValue = DEFAULT_END_VALUE;
    _this->startAngle = DEFAULT_START_ANGLE;
    _this->centerAngle = DEFAULT_CENTER_ANGLE;
    _this->dir = LE_COUNTER_CLOCKWISE;

    // default tick properties
    _this->ticksVisible = LE_TRUE;
    _this->tickValue = DEFAULT_TICK_VALUE;
    _this->tickLength = DEFAULT_TICK_LENGTH;

    _this->tickLabelsVisible = LE_TRUE;
    _this->ticksLabelFont = NULL;

    // default hand properties
    _this->handVisible = LE_TRUE;
    _this->handRadius = DEFAULT_HAND_LENGTH;
    
    _this->centerCircleVisible = LE_TRUE;
    _this->centerCircleRadius = DEFAULT_CENTER_CIRCLE_RADIUS;
    _this->centerCircleThickness = DEFAULT_CENTER_CIRCLE_THICKNESS;
    
    // advanced configuration
    leArray_Create(&_this->arcsArray);
    leArray_Create(&_this->ticksArray);
    leArray_Create(&_this->labelsArray);

    _this->cb = NULL;
}

void _leWidget_Destructor(leWidget* wgt);

void _leCircularGaugeWidget_Destructor(leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
    
    _this->fn->deleteArcs(_this);
    _this->fn->deleteMinorTicks(_this);
    _this->fn->deleteMinorTickLabels(_this);
    
    _leWidget_Destructor((leWidget*)_this);
}

leCircularGaugeWidget* leCircularGaugeWidget_New()
{
    leCircularGaugeWidget* gauge = NULL;

    gauge = LE_MALLOC(sizeof(leCircularGaugeWidget));

    if(gauge == NULL)
        return NULL;
    
    leCircularGaugeWidget_Constructor(gauge);

    return gauge;
}

static uint32_t getRadius(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->radius;
}

static leResult setRadius(leCircularGaugeWidget* _this,
                          uint32_t rad)
{
    LE_ASSERT_THIS();
        
    if(_this->radius == rad)
        return LE_SUCCESS;
       
    _this->radius = rad;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static int32_t getStartAngle(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->startAngle;
    
    return LE_SUCCESS;
}

static leResult setStartAngle(leCircularGaugeWidget* _this,
                              int32_t angle)
{
    LE_ASSERT_THIS();
        
    if(_this->startAngle == angle)
        return LE_SUCCESS;
        
    _this->startAngle = angle;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static int32_t getCenterAngle(const leCircularGaugeWidget* _this)
{
    if(_this == NULL)
        return 0;
        
    return _this->centerAngle;
}

static leResult setCenterAngle(leCircularGaugeWidget* _this,
                               int32_t angle)
{
    LE_ASSERT_THIS();

    if(_this->centerAngle == angle)
        return LE_SUCCESS;
        
    if (angle == 0)
        return LE_FAILURE;
    
    _this->centerAngle = angle;

    if (_this->centerAngle < 0)
    {
        _this->dir = LE_CLOCKWISE;
    }
    else
    {
        _this->dir = LE_COUNTER_CLOCKWISE;
    }
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

#if 0
static leCircularGaugeWidgetDir getDirection(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->dir;
}

static leResult setDirection(leCircularGaugeWidget* _this, 
                             leCircularGaugeWidgetDir dir)
{
    LE_ASSERT_THIS();

    if(_this->dir == dir)
        return LE_SUCCESS;
        
    _this->dir = dir;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}
#endif

static leResult addValueArc(leCircularGaugeWidget* _this, 
                            int32_t startValue, 
                            int32_t endValue,
                            uint32_t radius,
                            uint32_t thickness,
                            leScheme* scheme)
{
    leCircularGaugeArc * arc;
    
    LE_ASSERT_THIS();

    if (thickness > radius)
        return LE_FAILURE;

    if (startValue > endValue)
        return LE_FAILURE;

    if (startValue < _this->startValue || endValue > _this->endValue)
        return LE_FAILURE;
    
    arc = LE_MALLOC(sizeof(leCircularGaugeArc));
    
    if (arc == NULL)
        return LE_FAILURE;

    arc->type = VALUE_ARC;
    arc->radius = radius;
    arc->thickness = thickness;
    arc->scheme = scheme;
    arc->startValue = startValue;
    arc->endValue = endValue;
    arc->startAngle = 0;
    arc->endAngle = 0;

    leArray_PushBack(&_this->arcsArray, arc);

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leResult addAngularArc(leCircularGaugeWidget* _this, 
                              int32_t startAngle, 
                              int32_t endAngle,
                              uint32_t radius,
                              uint32_t thickness,
                              leScheme* scheme)
{
    leCircularGaugeArc * arc;
    
    LE_ASSERT_THIS();

    if (scheme == NULL)
        return LE_FAILURE;

    if (radius > _this->radius)
        return LE_FAILURE;

    if (thickness > radius)
        return LE_FAILURE;
/*
    if (startAngle < 0)
        return LE_FAILURE;

    if (endAngle > 360)
        return LE_FAILURE;
*/
    arc = LE_MALLOC(sizeof(leCircularGaugeArc));
    
    if (arc == NULL)
        return LE_FAILURE;

    arc->type = ANGLE_ARC;
    arc->radius = radius;
    arc->thickness = thickness;
    arc->scheme = scheme;
    arc->startAngle = startAngle;
    arc->endAngle = endAngle;
    arc->startValue = 0;
    arc->endValue = 0;

    leArray_PushBack(&_this->arcsArray, arc);

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leResult deleteArcs(leCircularGaugeWidget* _this)
{
    leCircularGaugeArc* arc;
    size_t idx;
    
    LE_ASSERT_THIS();
    
    for(idx = 0; idx < _this->arcsArray.size; idx++)
    {
        arc = (leCircularGaugeArc*)_this->arcsArray.values[idx];
        
        LE_FREE(arc);
    }
    
    leArray_Clear(&_this->arcsArray);

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leResult addMinorTicks(leCircularGaugeWidget* _this, 
                              int32_t startValue, 
                              int32_t endValue,
                              uint32_t radius,
                              uint32_t length,
                              uint32_t interval,
                              leScheme* scheme)
{
    leCircularGaugeTick * tick;
    
    LE_ASSERT_THIS();

    if (radius > _this->radius)
        return LE_FAILURE;

    if (length > radius)
        return LE_FAILURE;

    if (startValue < _this->startValue || 
        startValue > _this->endValue)
        return LE_FAILURE;
    
    if (endValue < _this->startValue || 
        endValue > _this->endValue)
        return LE_FAILURE;
    
    tick = LE_MALLOC(sizeof(leCircularGaugeTick));
    
    if (tick == NULL)
        return LE_FAILURE;

    tick->radius = radius;
    tick->length = length;
    tick->startValue = startValue;
    tick->endValue = endValue;
    tick->interval = interval;
    tick->scheme = scheme;

    leArray_PushBack(&_this->ticksArray, tick);

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leResult deleteMinorTicks(leCircularGaugeWidget* _this)
{
    leCircularGaugeTick* tick;
    size_t idx;
    
    LE_ASSERT_THIS();
    
    for(idx = 0; idx < _this->ticksArray.size; idx++)
    {
        tick = (leCircularGaugeTick*)_this->ticksArray.values[idx];
        
        LE_FREE(tick);
    }
    
    leArray_Clear(&_this->ticksArray);

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leResult addMinorTickLabels(leCircularGaugeWidget* _this, 
                                   int32_t startValue,
                                   int32_t endValue,
                                   uint32_t radius,
                                   leCircularGaugeWidgetLabelPosition position,
                                   uint32_t interval,
                                   leScheme* scheme)
{
    leCircularGaugeLabel * label;
    
    LE_ASSERT_THIS();

    if (radius > _this->radius)
        return LE_FAILURE;

    if (position != CIRCULAR_GAUGE_LABEL_OUTSIDE &&
        position != CIRCULAR_GAUGE_LABEL_INSIDE)
        return LE_FAILURE;

    if (startValue < _this->startValue || 
        startValue > _this->endValue)
        return LE_FAILURE;
    
    if (endValue < _this->startValue || 
        endValue > _this->endValue)
        return LE_FAILURE;
    
    label = LE_MALLOC(sizeof(leCircularGaugeLabel));
    
    if (label == NULL)
        return LE_FAILURE;

    label->radius = radius;
    label->position = position;
    label->startValue = startValue;
    label->endValue = endValue;
    label->interval = interval;
    label->scheme = scheme;

    leArray_PushBack(&_this->labelsArray, label);

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leResult deleteMinorTickLabels(leCircularGaugeWidget* _this)
{
    leCircularGaugeLabel* label;
    size_t idx;
    
    LE_ASSERT_THIS();
    
    for(idx = 0; idx < _this->labelsArray.size; idx++)
    {
        label = (leCircularGaugeLabel*)_this->labelsArray.values[idx];
        
        LE_FREE(label);
    }
    
    leArray_Clear(&_this->labelsArray);

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static int32_t getValue(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->value;
}

static leResult setValue(leCircularGaugeWidget* _this, 
                         int32_t value)
{
    LE_ASSERT_THIS();

    if (_this->value == value)
        return LE_SUCCESS;
    
    if (value < _this->startValue || value > _this->endValue)
        return LE_FAILURE;
    
    invalidateHandRect(_this);
        
    _this->value = value;
        
    invalidateHandRect(_this);
    
    return LE_SUCCESS;
}

static int32_t getStartValue(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->startValue;
}

static leResult setStartValue(leCircularGaugeWidget* _this, 
                              int32_t value)
{
    LE_ASSERT_THIS();

    if (_this->startValue == value)
        return LE_SUCCESS;

    _this->startValue = value;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static int32_t getEndValue(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->endValue;
}

static leResult setEndValue(leCircularGaugeWidget* _this, 
                            int32_t value)
{
    LE_ASSERT_THIS();

    if (_this->endValue == value)
        return LE_SUCCESS;

    _this->endValue = value;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static leBool getTicksVisible(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->ticksVisible;
}

static leResult setTicksVisible(leCircularGaugeWidget* _this, 
                                leBool visible)
{
    LE_ASSERT_THIS();

    if (_this->ticksVisible == visible)
        return LE_SUCCESS;

    _this->ticksVisible = visible;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static int32_t getTickValue(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->tickValue;
}

static leResult setTickValue(leCircularGaugeWidget* _this, 
                             int32_t value)
{
    LE_ASSERT_THIS();

    if (_this->tickValue == value)
        return LE_SUCCESS;
    
    _this->tickValue = value;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static uint32_t getTickLength(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->tickLength;
}

static leResult setTickLength(leCircularGaugeWidget* _this, 
                              uint32_t length)
{
    LE_ASSERT_THIS();

    if (_this->tickLength == length)
        return LE_SUCCESS;

    if (length > _this->radius)
        return LE_FAILURE;
    
    _this->tickLength = length;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static leBool getTickLabelsVisible(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->tickLabelsVisible;
}

static leResult setTickLabelsVisible(leCircularGaugeWidget* _this, 
                                     leBool visible)
{
    LE_ASSERT_THIS();

    if (_this->tickLabelsVisible == visible)
        return LE_SUCCESS;

    _this->tickLabelsVisible = visible;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static leBool getHandVisible(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->handVisible;
}

static leResult setHandVisible(leCircularGaugeWidget* _this, 
                               leBool visible)
{
    LE_ASSERT_THIS();

    if (_this->handVisible == visible)
        return LE_SUCCESS;

    _this->handVisible = visible;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static uint32_t getHandRadius(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->handRadius;
}

static leResult setHandRadius(leCircularGaugeWidget* _this, 
                              uint32_t length)
{
    LE_ASSERT_THIS();
        
    if(_this->handRadius == length)
        return LE_SUCCESS;
    
    if (length > _this->radius)
        return LE_FAILURE;
        
    _this->handRadius = length;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leBool getCenterCircleVisible(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->centerCircleVisible;
}

static leResult setCenterCircleVisible(leCircularGaugeWidget* _this, 
                                       leBool visible)
{
    LE_ASSERT_THIS();

    if (_this->centerCircleVisible == visible)
        return LE_SUCCESS;

    _this->centerCircleVisible = visible;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

static uint32_t getCenterCircleRadius(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->centerCircleRadius;
}

static leResult setCenterCircleRadius(leCircularGaugeWidget* _this, 
                                      uint32_t rad)
{
    LE_ASSERT_THIS();
        
    if(_this->centerCircleRadius == rad)
        return LE_SUCCESS;
    
    if (rad > _this->radius)
        return LE_FAILURE;
    
    _this->centerCircleRadius = rad;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static uint32_t getCenterCircleThickness(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->centerCircleThickness;
}

static leResult setCenterCircleThickness(leCircularGaugeWidget* _this,
                                         uint32_t thickness)
{
    LE_ASSERT_THIS();

    if (thickness > _this->centerCircleRadius)
        return LE_FAILURE;

    if(_this->centerCircleThickness == thickness)
        return LE_SUCCESS;
        
    _this->centerCircleThickness = thickness;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leResult setTickLabelFont(leCircularGaugeWidget* _this,
                                 const leFont* font)
{
    LE_ASSERT_THIS();
        
    _this->ticksLabelFont = font;
    
    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

static leResult setValueChangedEventCallback(leCircularGaugeWidget* _this,
                                             leCircularGaugeWidget_ValueChangedEvent cb)
{
    LE_ASSERT_THIS();
    
    _this->cb = cb;
    
    return LE_SUCCESS;
}

void _leCircularGaugeWidget_Paint(leCircularGaugeWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leCircularGaugeWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&circularGaugeWidgetVTable);
    
    /* overrides from base class */
    circularGaugeWidgetVTable._destructor = _leCircularGaugeWidget_Destructor;
    circularGaugeWidgetVTable._paint = _leCircularGaugeWidget_Paint;

    
    /* member functions */
    circularGaugeWidgetVTable.getRadius = getRadius;
    circularGaugeWidgetVTable.setRadius = setRadius;
    circularGaugeWidgetVTable.getStartAngle = getStartAngle;
    circularGaugeWidgetVTable.setStartAngle = setStartAngle;
    circularGaugeWidgetVTable.getCenterAngle = getCenterAngle;
    circularGaugeWidgetVTable.setCenterAngle = setCenterAngle;
    //circularGaugeWidgetVTable.getDirection = getDirection;
    //circularGaugeWidgetVTable.setDirection = setDirection;
    circularGaugeWidgetVTable.addValueArc = addValueArc;
    circularGaugeWidgetVTable.addAngularArc = addAngularArc;
    circularGaugeWidgetVTable.deleteArcs = deleteArcs;
    circularGaugeWidgetVTable.addMinorTicks = addMinorTicks;
    circularGaugeWidgetVTable.deleteMinorTicks = deleteMinorTicks;
    circularGaugeWidgetVTable.addMinorTickLabels = addMinorTickLabels;
    circularGaugeWidgetVTable.deleteMinorTickLabels = deleteMinorTickLabels;
    circularGaugeWidgetVTable.getValue = getValue;
    circularGaugeWidgetVTable.setValue = setValue;
    circularGaugeWidgetVTable.getStartValue = getStartValue;
    circularGaugeWidgetVTable.setStartValue = setStartValue;
    circularGaugeWidgetVTable.getEndValue = getEndValue;
    circularGaugeWidgetVTable.setEndValue = setEndValue;
    circularGaugeWidgetVTable.getTicksVisible = getTicksVisible;
    circularGaugeWidgetVTable.setTicksVisible = setTicksVisible;
    circularGaugeWidgetVTable.getTickValue = getTickValue;
    circularGaugeWidgetVTable.setTickValue = setTickValue;
    circularGaugeWidgetVTable.getTickLength = getTickLength;
    circularGaugeWidgetVTable.setTickLength = setTickLength;
    circularGaugeWidgetVTable.getTickLabelsVisible = getTickLabelsVisible;
    circularGaugeWidgetVTable.setTickLabelsVisible = setTickLabelsVisible;
    circularGaugeWidgetVTable.getHandVisible = getHandVisible;
    circularGaugeWidgetVTable.setHandVisible = setHandVisible;
    circularGaugeWidgetVTable.getHandRadius = getHandRadius;
    circularGaugeWidgetVTable.setHandRadius = setHandRadius;
    circularGaugeWidgetVTable.getCenterCircleVisible = getCenterCircleVisible;
    circularGaugeWidgetVTable.setCenterCircleVisible = setCenterCircleVisible;
    circularGaugeWidgetVTable.getCenterCircleRadius = getCenterCircleRadius;
    circularGaugeWidgetVTable.setCenterCircleRadius = setCenterCircleRadius;
    circularGaugeWidgetVTable.getCenterCircleThickness = getCenterCircleThickness;
    circularGaugeWidgetVTable.setCenterCircleThickness = setCenterCircleThickness;
    circularGaugeWidgetVTable.setTickLabelFont = setTickLabelFont;
    circularGaugeWidgetVTable.setValueChangedEventCallback = setValueChangedEventCallback;
}

void _leCircularGaugeWidget_FillVTable(leCircularGaugeWidgetVTable* tbl)
{
    *tbl = circularGaugeWidgetVTable;
}
#else
static const leCircularGaugeWidgetVTable circularGaugeWidgetVTable =
{
    // base class
    .getType = (void*)_leWidget_GetType,
    .getX = (void*)_leWidget_GetX,
    .setX = (void*)_leWidget_SetX,
    .getY = (void*)_leWidget_GetY,
    .setY = (void*)_leWidget_SetY,
    .setPosition = (void*)_leWidget_SetPosition,
    .translate = (void*)_leWidget_Translate,
    .getWidth = (void*)_leWidget_GetWidth,
    .setWidth = (void*)_leWidget_SetWidth,
    .getHeight = (void*)_leWidget_GetHeight,
    .setHeight = (void*)_leWidget_SetHeight,
    .setSize = (void*)_leWidget_SetSize,
    .resize = (void*)_leWidget_Resize,
    .getAlphaEnabled = (void*)_leWidget_GetAlphaEnabled,
    .getCumulativeAlphaEnabled = (void*)_leWidget_GetCumulativeAlphaEnabled,
    .setAlphaEnabled = (void*)_leWidget_SetAlphaEnabled,
    .getAlphaAmount = (void*)_leWidget_GetAlphaAmount,
    .getCumulativeAlphaAmount = (void*)_leWidget_GetCumulativeAlphaAmount,
    .setAlphaAmount = (void*)_leWidget_SetAlphaAmount,
    .isOpaque = (void*)_leWidget_IsOpaque,
    .getEnabled = (void*)_leWidget_GetEnabled,
    .setEnabled = (void*)_leWidget_SetEnabled,
    .getVisible = (void*)_leWidget_GetVisible,
    .setVisible = (void*)_leWidget_SetVisible,
    .localRect = (void*)_leWidget_LocalRect,
    .rectToParent = (void*)_leWidget_RectToParentSpace,
    .rectToScreen = (void*)_leWidget_RectToScreenSpace,
    .addChild = (void*)_leWidget_AddChild,
    .removeChild = (void*)_leWidget_RemoveChild,
    .removeAllChildren = (void*)_leWidget_RemoveAllChildren,
    .getRootWidget = (void*)_leWidget_GetRootWidget,
    .setParent = (void*)_leWidget_SetParent,
    .getChildCount = (void*)_leWidget_GetChildCount,
    .getChildAtIndex = (void*)_leWidget_GetChildAtIndex,
    .getIndexOfChild = (void*)_leWidget_GetIndexOfChild,
    .containsDescendent = (void*)_leWidget_ContainsDescendent,
    .getScheme = (void*)_leWidget_GetScheme,
    .setScheme = (void*)_leWidget_SetScheme,
    .getBorderType = (void*)_leWidget_GetBorderType,
    .setBorderType = (void*)_leWidget_SetBorderType,
    .getBackgroundType = (void*)_leWidget_GetBackgroundType,
    .setBackgroundType = (void*)_leWidget_SetBackgroundType,
    .getHAlignment = (void*)_leWidget_GetHAlignment,
    .setHAlignment = (void*)_leWidget_SetHAlignment,
    .getVAlignment = (void*)_leWidget_GetVAlignment,
    .setVAlignment = (void*)_leWidget_SetVAlignment,
    .getMargins = (void*)_leWidget_GetMargins,
    .setMargins = (void*)_leWidget_SetMargins,
    .getCornerRadius = (void*)_leWidget_GetCornerRadius,
    .setCornerRadius = (void*)_leWidget_SetCornerRadius,
    .hasFocus = (void*)_leWidget_HasFocus,
    .setFocus = (void*)_leWidget_SetFocus,
    .invalidate = (void*)_leWidget_Invalidate,
    .invalidateContents = (void*)_leWidget_InvalidateContents,
    .installEventFilter = (void*)_leWidget_InstallEventFilter,
    .removeEventFilter = (void*)_leWidget_RemoveEventFilter,

    .update = (void*)_leWidget_Update,

    .touchDownEvent = (void*)_leWidget_TouchDownEvent,
    .touchUpEvent = (void*)_leWidget_TouchUpEvent,
    .touchMoveEvent = (void*)_leWidget_TouchMoveEvent,
    .moveEvent = (void*)_leWidget_MoveEvent,
    .resizeEvent = (void*)_leWidget_ResizeEvent,
    .focusLostEvent = (void*)_leWidget_FocusLostEvent,
    .focusGainedEvent = (void*)_leWidget_FocusGainedEvent,
    .languageChangeEvent = (void*)_leWidget_LanguageChangeEvent,

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = _leCircularGaugeWidget_Destructor,
    ._paint = _leCircularGaugeWidget_Paint,

    /* member functions */
    .getRadius = getRadius,
    .setRadius = setRadius,
    .getStartAngle = getStartAngle,
    .setStartAngle = setStartAngle,
    .getCenterAngle = getCenterAngle,
    .setCenterAngle = setCenterAngle,
    //.getDirection = getDirection,
    //.setDirection = setDirection,
    .addValueArc = addValueArc,
    .addAngularArc = addAngularArc,
    .deleteArcs = deleteArcs,
    .addMinorTicks = addMinorTicks,
    .deleteMinorTicks = deleteMinorTicks,
    .addMinorTickLabels = addMinorTickLabels,
    .deleteMinorTickLabels = deleteMinorTickLabels,
    .getValue = getValue,
    .setValue = setValue,
    .getStartValue = getStartValue,
    .setStartValue = setStartValue,
    .getEndValue = getEndValue,
    .setEndValue = setEndValue,
    .getTicksVisible = getTicksVisible,
    .setTicksVisible = setTicksVisible,
    .getTickValue = getTickValue,
    .setTickValue = setTickValue,
    .getTickLength = getTickLength,
    .setTickLength = setTickLength,
    .getTickLabelsVisible = getTickLabelsVisible,
    .setTickLabelsVisible = setTickLabelsVisible,
    .getHandVisible = getHandVisible,
    .setHandVisible = setHandVisible,
    .getHandRadius = getHandRadius,
    .setHandRadius = setHandRadius,
    .getCenterCircleVisible = getCenterCircleVisible,
    .setCenterCircleVisible = setCenterCircleVisible,
    .getCenterCircleRadius = getCenterCircleRadius,
    .setCenterCircleRadius = setCenterCircleRadius,
    .getCenterCircleThickness = getCenterCircleThickness,
    .setCenterCircleThickness = setCenterCircleThickness,
    .setTickLabelFont = setTickLabelFont,
    .setValueChangedEventCallback = setValueChangedEventCallback,
};
#endif

#endif // LE_CIRCULARGAUGE_WIDGET_ENABLED
