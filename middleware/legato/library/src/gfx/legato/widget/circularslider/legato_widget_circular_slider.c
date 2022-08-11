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


#include "gfx/legato/widget/circularslider/legato_widget_circular_slider.h"

#if LE_CIRCULARSLIDER_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#include <math.h>

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

#define DEFAULT_RADIUS          30
#define DEFAULT_START_ANGLE     0
#define DEFAULT_SPAN_ANGLE      180
#define DEFAULT_VALUE           0

#define DEFAULT_DIRECTION       COUNTER_CLOCKWISE

#define DEFAULT_OUTSIDE_BORDER_THICKNESS    5
#define DEFAULT_INSIDE_BORDER_THICKNESS     5
#define DEFAULT_ACTIVE_ARC_THICKNESS        30
#define DEFAULT_SLIDER_BUTTON_THICKNESS     3
#define DEFAULT_SLIDER_BUTTON_RADIUS        15

//static void touchDown(leCircularSliderWidget* _this, leInput_TouchDownEvent* evt);
//static void touchUp(leCircularSliderWidget* _this, leInput_TouchUpEvent* evt);
//static void touchMoved(leCircularSliderWidget* _this, leInput_TouchMoveEvent* evt);

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leCircularSliderWidgetVTable circularSliderWidgetVTable;

static uint32_t _calculateSnapValue(uint32_t divisions,
                                    uint32_t val)
{
    uint32_t clmp;

    if(divisions > 1)
    {
        uint32_t stepAmt = 100 / divisions;
        uint32_t lastStep;

        for(lastStep = 0; lastStep < 100; lastStep += stepAmt)
        {
            clmp = lastStep + stepAmt;

            if(clmp > 100)
            {
                clmp = 100;
            }

            if(val < clmp)
            {
                if(val - lastStep < (clmp) - val)
                {
                    return lastStep;
                }
                else
                {
                    return clmp;
                }
            }
            else if(val == stepAmt)
            {
                return stepAmt;
            }
        }

        return 100;
    }
    else
    {
        if(val <= 50)
        {
            return 0;
        }
        else
        {
            return 100;
        }
    }
}

void leCircularSliderWidget_Constructor(leCircularSliderWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&circularSliderWidgetVTable;
    _this->fn = &circularSliderWidgetVTable;

    _this->widget.type = LE_WIDGET_CIRCULAR_SLIDER;
    
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_NONE;

    _this->radius = DEFAULT_RADIUS;
    _this->startAngle = DEFAULT_START_ANGLE;
    _this->spanAngle = DEFAULT_SPAN_ANGLE;
    _this->value = DEFAULT_VALUE;

    _this->direction = LE_COUNTER_CLOCKWISE;
    _this->roundEdges = LE_TRUE;
    _this->sticky = LE_FALSE;
    _this->snapDivisions = 1;
    _this->buttonTouch = LE_TRUE;
    _this->btnState = LE_CIRCULAR_SLIDER_STATE_UP;

    _this->outsideBorderArc.visible = LE_TRUE;
    _this->outsideBorderArc.thickness = DEFAULT_OUTSIDE_BORDER_THICKNESS;
    _this->outsideBorderArc.scheme = _this->widget.scheme;

    _this->activeArc.visible = LE_TRUE;
    _this->activeArc.radius = _this->radius - _this->outsideBorderArc.thickness;
    _this->activeArc.thickness = DEFAULT_ACTIVE_ARC_THICKNESS;
    _this->activeArc.scheme = _this->widget.scheme;

    _this->inActiveArc.visible = LE_TRUE;
    _this->inActiveArc.radius = _this->activeArc.radius;
    _this->inActiveArc.thickness = _this->activeArc.thickness;
    _this->inActiveArc.scheme = _this->widget.scheme;

    _this->insideBorderArc.visible = LE_TRUE;
    _this->insideBorderArc.radius = _this->activeArc.radius - _this->activeArc.thickness;
    _this->insideBorderArc.thickness = DEFAULT_INSIDE_BORDER_THICKNESS;
    _this->insideBorderArc.scheme = _this->widget.scheme;

    _this->circleButtonArc.visible = LE_TRUE;
    _this->circleButtonArc.radius = DEFAULT_SLIDER_BUTTON_RADIUS;
    _this->circleButtonArc.thickness = DEFAULT_SLIDER_BUTTON_THICKNESS;
    _this->circleButtonArc.scheme = _this->widget.scheme;

    _this->pressedCallback = NULL;
    _this->valueChangedCallback = NULL;
    _this->releasedCallback = NULL;
}

void _leWidget_Destructor(leWidget* _this);

void _leCircularSliderWidget_Destructor(leCircularSliderWidget* _this)
{
    _leWidget_Destructor((leWidget*)_this);
}

leCircularSliderWidget* leCircularSliderWidget_New()
{
    leCircularSliderWidget* slider = NULL;

    slider = LE_MALLOC(sizeof(leCircularSliderWidget));

    if(slider == NULL)
        return NULL;
    
    leCircularSliderWidget_Constructor(slider);

    return slider;
}

static uint32_t getRadius(const leCircularSliderWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->radius;
}

static leResult setRadius(leCircularSliderWidget* _this,
                          uint32_t rad)
{
    LE_ASSERT_THIS();
        
    if(_this->radius == rad)
        return LE_SUCCESS;
        
    _this->radius = rad;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getStartAngle(const leCircularSliderWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->startAngle;
}

static leResult setStartAngle(leCircularSliderWidget* _this,
                              int32_t angle)
{
    LE_ASSERT_THIS();
        
    if(_this->startAngle == angle)
        return LE_SUCCESS;

    if (angle > 360)
    {
        angle = 360;
    }
    else if(angle < -360)
    {
        angle = -360;
    }

    _this->startAngle = angle;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getSpanAngle(const leCircularSliderWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->spanAngle;
}

static leResult setSpanAngle(leCircularSliderWidget* _this,
                             int32_t angle)
{
    LE_ASSERT_THIS();

    if(_this->spanAngle == angle)
        return LE_SUCCESS;

    if (angle > 360)
    {
        angle = 360;
    }
    else if(angle < -360)
    {
        angle = -360;
    }

    _this->spanAngle = angle;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getValue(const leCircularSliderWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->value;
}

static leResult setValue(leCircularSliderWidget* _this,
                         uint32_t value)
{
    LE_ASSERT_THIS();
        
    if(_this->value == value)
        return LE_SUCCESS;

    if (value > 100)
        return LE_FAILURE;
    
    _this->fn->invalidate(_this);

    _this->value = value;
    
    if (_this->valueChangedCallback != NULL)
    {
        _this->valueChangedCallback(_this, _this->value);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getRoundEdges(const leCircularSliderWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->roundEdges;
}

static leResult setRoundEdges(leCircularSliderWidget* _this,
                              leBool round)
{
    LE_ASSERT_THIS();
        
    if(_this->roundEdges == round)
        return LE_SUCCESS;
        
    _this->roundEdges = round;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getStickyButton(const leCircularSliderWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->sticky;
}

static leResult setStickyButton(leCircularSliderWidget* _this,
                                leBool sticky)
{
    LE_ASSERT_THIS();
        
    if(_this->sticky == sticky)
        return LE_SUCCESS;
        
    _this->sticky = sticky;

    if(_this->sticky == LE_TRUE)
    {
        _this->fn->setValue(_this,
                            _calculateSnapValue(_this->snapDivisions,
                                                _this->value));
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getSnapDivisions(const leCircularSliderWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->snapDivisions;
}

static leResult setSnapDivisions(leCircularSliderWidget* _this,
                                 uint32_t div)
{
    LE_ASSERT_THIS();

    if(_this->snapDivisions == div)
        return LE_SUCCESS;

    if(_this->snapDivisions == 0)
        return LE_FAILURE;

    _this->snapDivisions = div;

    if(_this->sticky == LE_TRUE)
    {
        _this->fn->setValue(_this,
                            _calculateSnapValue(_this->snapDivisions,
                                                _this->value));
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getTouchOnButtonOnly(const leCircularSliderWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->buttonTouch;
}

static leResult setTouchOnButtonOnly(leCircularSliderWidget* _this,
                                     leBool buttonOnly)
{
    LE_ASSERT_THIS();

    if(_this->buttonTouch == buttonOnly)
        return LE_SUCCESS;
        
    _this->buttonTouch = buttonOnly;
    
    //_this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}


static uint32_t getArcThickness(const leCircularSliderWidget* _this, 
                                leCircularSliderWidgetArcType type)
{
    LE_ASSERT_THIS();
    
    switch(type)
    {
        case OUTSIDE_CIRCLE_BORDER:
            return _this->outsideBorderArc.thickness;
        case INSIDE_CIRCLE_BORDER:
            return _this->insideBorderArc.thickness;
        case ACTIVE_AREA:
            return _this->activeArc.thickness;
        case INACTIVE_AREA:
            return _this->inActiveArc.thickness;
        case CIRCLE_BUTTON:
            return _this->circleButtonArc.thickness;
        default:
            break;
    }
        
    return 0;
}

static leResult setArcThickness(leCircularSliderWidget* _this, 
                                leCircularSliderWidgetArcType type,
                                uint32_t thickness)
{
    LE_ASSERT_THIS();

    switch(type)
    {
        case OUTSIDE_CIRCLE_BORDER:
        {
            //if (thickness == _this->outsideBorderArc.thickness)
            //    return LE_SUCCESS;

            if (thickness > _this->radius)
                return LE_FAILURE;
            
            if (thickness > (_this->radius - _this->activeArc.thickness - _this->insideBorderArc.thickness))
                return LE_FAILURE;

            _this->outsideBorderArc.thickness = thickness;

            break;
        }
        case ACTIVE_AREA:
        { 
            //if (thickness == _this->activeArc.thickness)
            //    return LE_SUCCESS;

            if (thickness > _this->radius - _this->outsideBorderArc.thickness)
                return LE_FAILURE;
            
            if (thickness > (_this->radius - _this->outsideBorderArc.thickness - _this->insideBorderArc.thickness))
                return LE_FAILURE;

            _this->activeArc.thickness = thickness;
            _this->inActiveArc.thickness = thickness;

            _this->insideBorderArc.radius = _this->activeArc.radius - _this->activeArc.thickness;

            break;
        }

        case INSIDE_CIRCLE_BORDER:
        {
            //if (thickness == _this->insideBorderArc.thickness)
            //    return LE_SUCCESS;

            if (thickness > _this->radius - _this->outsideBorderArc.thickness - _this->activeArc.thickness)
                return LE_FAILURE;
            
            _this->insideBorderArc.thickness = thickness;
        
            break;
        }
        case CIRCLE_BUTTON:
        {
            //if (thickness == _this->circleButtonArc.thickness)
            //    return LE_SUCCESS;

            if (thickness > _this->circleButtonArc.radius)
                return LE_FAILURE;

            _this->circleButtonArc.thickness = thickness;

            break;
        }
        default:
            return LE_FAILURE;
    }

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getArcRadius(const leCircularSliderWidget* _this,
                             leCircularSliderWidgetArcType type)
{
    LE_ASSERT_THIS();

    switch(type)
    {
        case CIRCLE_BUTTON:
            return _this->circleButtonArc.radius;
        default:
            break;
    }
        
    return 0;
}

static leResult setArcRadius(leCircularSliderWidget* _this,
                             leCircularSliderWidgetArcType type,
                             uint32_t rad)
{
    LE_ASSERT_THIS();

    switch(type)
    {
        case CIRCLE_BUTTON:
        {
            if (_this->circleButtonArc.radius == rad)
                return LE_SUCCESS;
            
            if (rad > _this->radius)
                return LE_FAILURE;

            _this->circleButtonArc.radius = rad;

            break;
        }
        default:
            return LE_FAILURE;
    }
        
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leScheme* getArcScheme(const leCircularSliderWidget* _this,
                              leCircularSliderWidgetArcType type)
{
    LE_ASSERT_THIS();

    switch(type)
    {
        case CIRCLE_BUTTON:
            return (leScheme*)_this->circleButtonArc.scheme;
        default:
            return (leScheme*)_this->widget.scheme;
    }
        
    return NULL;
}

static leResult setArcScheme(leCircularSliderWidget* _this,
                             leCircularSliderWidgetArcType type,
                             const leScheme* scheme)
{
    LE_ASSERT_THIS();

    switch(type)
    {
        //Only the circle button has a different scheme from parent widget
        case CIRCLE_BUTTON:
            _this->circleButtonArc.scheme = scheme;
            break;
        default:
            return LE_FAILURE;
    }
        
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getArcVisible(const leCircularSliderWidget* _this,
                            leCircularSliderWidgetArcType type)
{
    LE_ASSERT_THIS();

    switch(type)
    {
        case OUTSIDE_CIRCLE_BORDER:
            return _this->outsideBorderArc.visible;
        case INSIDE_CIRCLE_BORDER:
            return _this->insideBorderArc.visible;
        case ACTIVE_AREA:
            return _this->activeArc.visible;
        case INACTIVE_AREA:
            return _this->inActiveArc.visible;
        case CIRCLE_BUTTON:
            return _this->circleButtonArc.visible;
        default:
            break;
    }
        
    return LE_FALSE;
}

static leResult setArcVisible(leCircularSliderWidget* _this,
                              leCircularSliderWidgetArcType type,
                              leBool visible)
{
    LE_ASSERT_THIS();

    switch(type)
    {
        case OUTSIDE_CIRCLE_BORDER:
        {
            if (_this->outsideBorderArc.visible == visible)
                return LE_SUCCESS;

            _this->outsideBorderArc.visible = visible;
            
            break;
        }
        case INSIDE_CIRCLE_BORDER:
        {
            if (_this->insideBorderArc.visible == visible)
                return LE_SUCCESS;

            _this->insideBorderArc.visible = visible;
            
            break;
        }
        case ACTIVE_AREA:
        {
            if (_this->activeArc.visible == visible)
                return LE_SUCCESS;

            _this->activeArc.visible = visible;
            
            break;
        }
        case INACTIVE_AREA:
        {
            if (_this->inActiveArc.visible == visible)
                return LE_SUCCESS;

            _this->inActiveArc.visible = visible;
            
            break;
        }
        case CIRCLE_BUTTON:
        {
            if (_this->circleButtonArc.visible == visible)
                return LE_SUCCESS;

            _this->circleButtonArc.visible = visible;
            
            break;
        }
        default:
            return LE_FAILURE;
    }
        
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leRotationDirection getDirection(const leCircularSliderWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->direction;
}

static leResult setDirection(leCircularSliderWidget* _this,
                             leRotationDirection dir)
{
    LE_ASSERT_THIS();
        
    if(_this->direction == dir)
        return LE_SUCCESS;
        
    _this->direction = dir;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult setPressedEventCallback(leCircularSliderWidget* _this,
                                        leCircularSliderWidget_PressedEvent cb)
{
    LE_ASSERT_THIS();

    _this->pressedCallback = cb;
        
    return LE_SUCCESS;
}

static leResult setValueChangedEventCallback(leCircularSliderWidget* _this,
                                             leCircularSliderWidget_ValueChangedEvent cb)
{
    LE_ASSERT_THIS();

    _this->valueChangedCallback = cb;
        
    return LE_SUCCESS;
}

static leResult setReleasedEventCallback(leCircularSliderWidget* _this,
                                         leCircularSliderWidget_ReleasedEvent cb)
{
    LE_ASSERT_THIS();
        
    _this->releasedCallback = cb;

    return LE_SUCCESS;
}

static leBool getPercent(leCircularSliderWidget* _this,
                         lePoint pt,
                         uint32_t* percent)
{
    int32_t startAngle;
    int32_t endAngle;
    lePoint centerPt;
    float rad, mag, dot, det;
    int32_t deg, adj;
    float xf, yf;

    // transform the vector
    centerPt.x = _this->widget.rect.width / 2;
    centerPt.y = _this->widget.rect.height / 2;

    pt.x -= centerPt.x;
    pt.y -= centerPt.y;
    pt.y *= -1;

    // normalize the vector
    mag = leSqrt((pt.x * pt.x) + (pt.y * pt.y));

    xf = (float)pt.x / mag;
    yf = (float)pt.y / mag;

    // calculate the angle
    dot = (xf * 1.0f) + (yf * 0.0f);
    det = (xf * 0.0f) - (yf * 1.0f);

    rad = -atan2f(det, dot);

    deg = (int32_t)(rad * 57.295827909f);

    // normalize the angle
    if(deg < 0)
    {
        deg += 360;
    }

    // normalize the angles the angle span
    leNormalizeAngles(_this->startAngle,
                      _this->spanAngle,
                      &startAngle,
                      &endAngle);

    adj = 360 - startAngle;

    endAngle -= startAngle;
    startAngle = 0;

    deg += adj;

    if(deg >= 360)
    {
        deg -= 360;
    }

    // calculate the percent
    startAngle = 0;

    if(deg < 0)
        return LE_FALSE;

    if(deg > endAngle)
        return LE_FALSE;

    if(_this->spanAngle < 0)
    {
        *percent = 100 - lePercentWholeRounded(deg, endAngle);
    }
    else
    {
        *percent = lePercentWholeRounded(deg, endAngle);
    }

    return LE_TRUE;
}

static leBool _pointInActiveArc(leCircularSliderWidget* _this,
                                lePoint pt)
{

    uint32_t innerRadius;
    uint32_t outerRadius;
    lePoint centerPt;
    uint32_t vecLength;
    uint32_t percent;

    // test the radius
    innerRadius = _this->radius - _this->activeArc.thickness / 2;
    outerRadius = _this->radius + _this->activeArc.thickness / 2;

    centerPt.x = _this->widget.rect.width / 2;
    centerPt.y = _this->widget.rect.height / 2;

    pt.x -= centerPt.x;
    pt.y -= centerPt.y;

    vecLength = (pt.x * pt.x) + (pt.y * pt.y);

    innerRadius *= innerRadius;
    outerRadius *= outerRadius;

    if(vecLength < innerRadius || vecLength > outerRadius)
        return LE_FALSE;

    return getPercent(_this, pt, &percent);
}

static leBool _pointInButton(leCircularSliderWidget* _this,
                             lePoint pt)
{
    lePoint centerPt;
    lePoint btnPnt;

    uint32_t deg = leDegreesFromPercent(_this->value,
                                        _this->spanAngle,
                                        _this->startAngle);

    lePointOnCircle(_this->radius,
                    deg,
                    &btnPnt);

    btnPnt.y *= -1;

    centerPt.x = _this->widget.rect.width / 2;
    centerPt.y = _this->widget.rect.height / 2;

    btnPnt.x += centerPt.x;
    btnPnt.y += centerPt.y;

    btnPnt.x -= pt.x;
    btnPnt.y -= pt.y;

    if((btnPnt.x * btnPnt.x + btnPnt.y * btnPnt.y) < (int32_t)(_this->circleButtonArc.radius * _this->circleButtonArc.radius))
    {
        return LE_TRUE;
    }

    return LE_FALSE;
}

static void handleTouchDownEvent(leCircularSliderWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    lePoint pnt;
    uint32_t percent;

    LE_ASSERT_THIS();

    pnt.x = evt->x;
    pnt.y = evt->y;

    if(_this->circleButtonArc.visible == LE_FALSE)
        return;

    leUtils_PointScreenToLocalSpace((leWidget*)_this, &pnt);

    if(_pointInButton(_this, pnt) == LE_FALSE)
    {
        if(_this->buttonTouch == LE_FALSE && _pointInActiveArc(_this, pnt) == LE_FALSE)
        {
            _this->btnState = LE_CIRCULAR_SLIDER_STATE_UP;

            return;
        }
    }

    if(getPercent(_this, pnt, &percent) == LE_FALSE)
        return;

    _this->fn->setValue(_this, percent);

    _this->btnState = LE_CIRCULAR_SLIDER_STATE_DOWN;

    if (_this->pressedCallback != NULL)
    {
        _this->pressedCallback(_this, _this->value);
    }

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

static void handleTouchUpEvent(leCircularSliderWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    LE_ASSERT_THIS();
    
    if (_this->btnState == LE_CIRCULAR_SLIDER_STATE_DOWN)
    {
        leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
        
        _this->btnState = LE_CIRCULAR_SLIDER_STATE_UP;

        if(_this->sticky == LE_TRUE)
        {
            _this->fn->setValue(_this,
                                _calculateSnapValue(_this->snapDivisions,
                                                    _this->value));
        }
    
        if (_this->releasedCallback != NULL)
        {
            _this->releasedCallback(_this, _this->value);
        }
    }
}

static void handleTouchMovedEvent(leCircularSliderWidget* _this, leWidgetEvent_TouchMove* evt)
{
    lePoint pnt;
    uint32_t percent;

    if(_this->btnState != LE_CIRCULAR_SLIDER_STATE_DOWN)
        return;

    pnt.x = evt->x;
    pnt.y = evt->y;

    if(_this->circleButtonArc.visible == LE_FALSE)
        return;

    leUtils_PointScreenToLocalSpace((leWidget*)_this, &pnt);

    if(_pointInButton(_this, pnt) == LE_FALSE)
    {
        if(_this->buttonTouch == LE_FALSE && _pointInActiveArc(_this, pnt) == LE_FALSE)
        {
            _this->btnState = LE_CIRCULAR_SLIDER_STATE_UP;

            return;
        }
    }

    if(getPercent(_this, pnt, &percent) == LE_FALSE)
        return;

    _this->fn->setValue(_this, percent);

    _this->btnState = LE_CIRCULAR_SLIDER_STATE_DOWN;

    if (_this->pressedCallback != NULL)
    {
        _this->pressedCallback(_this, _this->value);
    }

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

void _leCircularSliderWidget_Paint(leCircularSliderWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leCircularSliderWidget_GenerateVTable(void)
{
    _leWidget_FillVTable((void*)&circularSliderWidgetVTable);
    
    /* overrides from base class */
    circularSliderWidgetVTable._destructor = _leCircularSliderWidget_Destructor;
    circularSliderWidgetVTable._paint = _leCircularSliderWidget_Paint;
    circularSliderWidgetVTable.touchDownEvent = handleTouchDownEvent;
    circularSliderWidgetVTable.touchUpEvent = handleTouchUpEvent;
    circularSliderWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    
    /* member functions */
    circularSliderWidgetVTable.getRadius = getRadius;
    circularSliderWidgetVTable.setRadius = setRadius;
    circularSliderWidgetVTable.getStartAngle = getStartAngle;
    circularSliderWidgetVTable.setStartAngle = setStartAngle;
    circularSliderWidgetVTable.getSpanAngle = getSpanAngle;
    circularSliderWidgetVTable.setSpanAngle = setSpanAngle;
    circularSliderWidgetVTable.getArcThickness = getArcThickness;
    circularSliderWidgetVTable.setArcThickness = setArcThickness;
    circularSliderWidgetVTable.getArcRadius = getArcRadius;
    circularSliderWidgetVTable.setArcRadius = setArcRadius;
    circularSliderWidgetVTable.getArcScheme = getArcScheme;
    circularSliderWidgetVTable.setArcScheme = setArcScheme;
    circularSliderWidgetVTable.getArcVisible = getArcVisible;
    circularSliderWidgetVTable.setArcVisible = setArcVisible;
    circularSliderWidgetVTable.getValue = getValue;
    circularSliderWidgetVTable.setValue = setValue;
    circularSliderWidgetVTable.getRoundEdges = getRoundEdges;
    circularSliderWidgetVTable.setRoundEdges = setRoundEdges;
    circularSliderWidgetVTable.getStickyButton = getStickyButton;
    circularSliderWidgetVTable.setStickyButton = setStickyButton;
    circularSliderWidgetVTable.getSnapDivisions = getSnapDivisions;
    circularSliderWidgetVTable.setSnapDivisions = setSnapDivisions;
    circularSliderWidgetVTable.getTouchOnButtonOnly = getTouchOnButtonOnly;
    circularSliderWidgetVTable.setTouchOnButtonOnly = setTouchOnButtonOnly;
    circularSliderWidgetVTable.getDirection = getDirection;
    circularSliderWidgetVTable.setDirection = setDirection;
    circularSliderWidgetVTable.setPressedEventCallback = setPressedEventCallback;
    circularSliderWidgetVTable.setValueChangedEventCallback = setValueChangedEventCallback;
    circularSliderWidgetVTable.setReleasedEventCallback = setReleasedEventCallback;
}

void _leCircularSliderWidget_FillVTable(leCircularSliderWidgetVTable* tbl)
{
    *tbl = circularSliderWidgetVTable;
}
#else
static const leCircularSliderWidgetVTable circularSliderWidgetVTable =
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
    .containsDescendant = (void*)_leWidget_ContainsDescendant,
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
    ._destructor = _leCircularSliderWidget_Destructor,
    ._paint = _leCircularSliderWidget_Paint,
    .touchDownEvent = handleTouchDownEvent,
    .touchUpEvent = handleTouchUpEvent,
    .touchMoveEvent = handleTouchMovedEvent,

    /* member functions */
    .getRadius = getRadius,
    .setRadius = setRadius,
    .getStartAngle = getStartAngle,
    .setStartAngle = setStartAngle,
    .getSpanAngle = getSpanAngle,
    .setSpanAngle = setSpanAngle,
    .getArcThickness = getArcThickness,
    .setArcThickness = setArcThickness,
    .getArcRadius = getArcRadius,
    .setArcRadius = setArcRadius,
    .getArcScheme = getArcScheme,
    .setArcScheme = setArcScheme,
    .getArcVisible = getArcVisible,
    .setArcVisible = setArcVisible,
    .getValue = getValue,
    .setValue = setValue,
    .getRoundEdges = getRoundEdges,
    .setRoundEdges = setRoundEdges,
    .getStickyButton = getStickyButton,
    .setStickyButton = setStickyButton,
    .getSnapDivisions = getSnapDivisions,
    .setSnapDivisions = setSnapDivisions,
    .getTouchOnButtonOnly = getTouchOnButtonOnly,
    .setTouchOnButtonOnly = setTouchOnButtonOnly,
    .getDirection = getDirection,
    .setDirection = setDirection,
    .setPressedEventCallback = setPressedEventCallback,
    .setValueChangedEventCallback = setValueChangedEventCallback,
    .setReleasedEventCallback = setReleasedEventCallback,
};
#endif

#endif // LE_CIRCULARSLIDER_WIDGET_ENABLED
