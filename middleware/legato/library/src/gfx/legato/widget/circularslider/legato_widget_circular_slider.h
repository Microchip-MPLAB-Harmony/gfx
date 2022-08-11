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
    legato_widget_circular_slider.h

  Summary:


  Description:
    This module implements slider drawing widget functions.
*******************************************************************************/

/** \file legato_widget_circular_slider.h
 * @brief Slider widget functions and definitions.
 *
 * @details This module implements slider drawing widget functions.
 */

#ifndef LEGATO_WIDGET_CIRCULAR_SLIDER_H
#define LEGATO_WIDGET_CIRCULAR_SLIDER_H

#include "gfx/legato/common/legato_common.h"

#if LE_CIRCULARSLIDER_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

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
/**
  * @brief This enum represents circular slider button state.
  * @details A circular slider button state is used to define possible
  * button states.
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
/**
  * @brief This enum represents circular slider widget arc type.
  * @details A circular slider widget arc type is used to define
  * arcs that compose the circular slider.
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
/**
  * @brief This enum represents circular gauge widget.
  * @details A circular slider widget arc type is used to define
  * describs the arc instances in the circular gauge widget.
  */
typedef struct leCircularSliderArc
{
    leBool visible;
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
/**
 * @brief This function represents a circular slider widget press callback.
 * @details Press callback is used indicate a press event.
 * @details .
 */
typedef void (*leCircularSliderWidget_PressedEvent)(leCircularSliderWidget *, int32_t);

// *****************************************************************************
/* Function Pointer:
    leCircularSliderWidget_ValueChangedEvent

  Summary:
    Value changed event function callback type
*/
/**
 * @brief This function represents a circular slider widget value changed callback.
 * @details Value changed callback is used indicate a value changed event.
 * @details .
 */
typedef void (*leCircularSliderWidget_ValueChangedEvent)(leCircularSliderWidget *, int32_t);

// *****************************************************************************
/* Function Pointer:
    leCircularSliderWidget_ReleasedEvent

  Summary:
    Button released event function callback type
*/
/**
 * @brief This function represents a release callback.
 * @details Release callback is used indicate a release event.
 * @details .
 */
typedef void (*leCircularSliderWidget_ReleasedEvent)(leCircularSliderWidget *, int32_t);

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
#define LE_CIRCULARSLIDERWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t                 (*getRadius)(const THIS_TYPE* _this); \
    leResult                 (*setRadius)(THIS_TYPE* _this, uint32_t rad); \
    int32_t                  (*getStartAngle)(const THIS_TYPE* _this); \
    leResult                 (*setStartAngle)(THIS_TYPE* _this, int32_t ang); \
    int32_t                  (*getSpanAngle)(const THIS_TYPE* _this); \
    leResult                 (*setSpanAngle)(THIS_TYPE* _this, int32_t ang); \
    uint32_t                 (*getArcThickness)(const THIS_TYPE* _this, leCircularSliderWidgetArcType type); \
    leResult                 (*setArcThickness)(THIS_TYPE* _this, leCircularSliderWidgetArcType type, uint32_t thck); \
    uint32_t                 (*getArcRadius)(const THIS_TYPE* _this, leCircularSliderWidgetArcType type); \
    leResult                 (*setArcRadius)(THIS_TYPE* _this, leCircularSliderWidgetArcType type, uint32_t rad); \
    leScheme*                (*getArcScheme)(const THIS_TYPE* _this, leCircularSliderWidgetArcType type); \
    leResult                 (*setArcScheme)(THIS_TYPE* _this, leCircularSliderWidgetArcType type, const leScheme* schm); \
    leBool                   (*getArcVisible)(const THIS_TYPE* _this, leCircularSliderWidgetArcType type); \
    leResult                 (*setArcVisible)(THIS_TYPE* _this, leCircularSliderWidgetArcType type, leBool vis); \
    uint32_t                 (*getValue)(const THIS_TYPE* _this); \
    leResult                 (*setValue)(THIS_TYPE* _this, uint32_t val); \
    leBool                   (*getRoundEdges)(const THIS_TYPE* _this); \
    leResult                 (*setRoundEdges)(THIS_TYPE* _this, leBool rnd); \
    leBool                   (*getStickyButton)(const THIS_TYPE* _this); \
    leResult                 (*setStickyButton)(THIS_TYPE* _this, leBool stk); \
    uint32_t                 (*getSnapDivisions)(const THIS_TYPE* _this); \
    leResult                 (*setSnapDivisions)(THIS_TYPE* _this, uint32_t div); \
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

/**
  * @endcond
  *
  */

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

    int32_t startAngle; //the start angle of the slider
    int32_t spanAngle;  //the span angle of the slider

    uint32_t value;      //the value of the slider

    leBool roundEdges; //round edges
    leBool sticky; //snaps to start value before wrapping around
    uint32_t snapDivisions;
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

/**
 * @brief Create widget.
 * @details Creates a new leCircularSliderWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leCircularSliderWidget* wgt = leCircularSliderWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leCircularSliderWidget* leCircularSliderWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leCircularSliderWidget <span class="param">wgt</span>.
 * @code
 * leCircularSliderWidget* wgt;
 * leCircularSliderWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leCircularSliderWidget_Constructor(leCircularSliderWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Get radius.
 * @details Gets the radius using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * uint32_t radius = _this->fn->getRadius(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the radius.
 */
virtual uint32_t getRadius(const leCircularSliderWidget* _this);


// *****************************************************************************
/**
 * @brief Set radius.
 * @details Sets the radius to
 * <span class="param">rad</span> using <span class="param">_this</span>.
 * @code
 * leCircularSliderWidget* _this;
 * uint32_t rad;
 * leResult res = _this->fn->setRadius(_this, rad);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param thk the circle radius.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setRadius(leCircularSliderWidget* _this,
                           uint32_t rad);


// *****************************************************************************
/**
 * @brief Get start angle.
 * @details Gets the start angle using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * int32_t angle = _this->fn->getStartAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the start angle.
 */
virtual uint32_t getStartAngle(const leCircularSliderWidget* _this);


// *****************************************************************************
/**
 * @brief Set start angle.
 * @details Sets the start angle to
 * <span class="param">ang</span> using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * int32_t ang;
 * leResult res = _this->fn->setStartAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param ang is the start angle.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setStartAngle(leCircularSliderWidget* _this,
                               int32_t ang);


/**
 * @brief Get arc thickness value.
 * @details Gets the arc thickness value for <span class="param">type</span>
 * using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leCircularSliderWidgetArcType type;
 * uint32_t thickness = _this->fn->getArcThickness(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the thickness value.
 */
virtual uint32_t getArcThickness(const leCircularSliderWidget* _this,
                                 leCircularSliderWidgetArcType type);




/**
 * @brief Set arc thickness value.
 * @details Sets the arc thickness value for <span class="param">type</span> to
 * <span class="param">thck</span> using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leCircularSliderWidgetArcType type;
 * uint32_t thck;
 * leResult res = _this->fn->setArcThickness(_this, type, thck);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param type the type.
 * @param thck the thickness value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setArcThickness(leCircularSliderWidget* _this,
                                 leCircularSliderWidgetArcType type,
                                 uint32_t thck);

/ *****************************************************************************
/**
 * @brief Get arc radius.
 * @details Gets the arc radius for <span class="param">type</span>
 * using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * uint32_t radius = _this->fn->getArcRadius(_this, type);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param type the type.
 * @returns the radius.
 */
virtual uint32_t getArcRadius(const leCircularSliderWidget* _this,
                              leCircularSliderWidgetArcType type);


// *****************************************************************************
/**
 * @brief Set arc radius.
 * @details Sets the arc radius to
 * <span class="param">rad</span> using <span class="param">_this</span>.
 * @code
 * leCircularSliderWidget* _this;
 * leCircularSliderWidgetArcType type;
 * uint32_t rad;
 * leResult res = _this->fn->setArcRadius(_this, type, rad);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param thk the circle radius.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setArcRadius(leCircularSliderWidget* _this,
                              leCircularSliderWidgetArcType type,
                              uint32_t rad);


/**
 * @brief Get arc scheme.
 * @details Gets the arc scheme by
 * <span class="param">type</span> using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leCircularSliderWidgetArcType type;
 * leScheme* schm = _this->fn->getArcScheme(_this, type);
 * @endcode
 * @param _this is the widget to query.
 * @returns the arc scheme.
 */
virtual leScheme* getArcScheme(const leCircularSliderWidget* _this,
                               leCircularSliderWidgetArcType type);

/**
 * @brief Set arc scheme
 * @details Sets the arc by
 * <span class="param">type</span> to
 * <span class="param">schm</span> using <span class="param">_this</span>.
 * @code
 * leCircularSliderWidget* _this;
 * leCircularSliderWidgetArcType type;
 * const leScheme* schm;
 * leResult res = _this->fn->setSeriesScheme(_this, type, schm);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param type the type to modify
 * @param schm  the arc to use
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult setArcScheme(leCircularSliderWidget* _this,
                      leCircularSliderWidgetArcType type,
                      const leScheme* schm;

* @brief Get arc visibility
* @details Gets arc visilibilty for <span class="param">type</span> using
* <span class="param">_this</span>.
* @code
* leCircularSliderWidget* _this;
* leCircularSliderWidgetArcType type;
* leBool visible = _this->fn->getArcVisible(_this, axis);
* @endcode
* @param _this is the widget to query.
* @param type the type to query.
* @returns LE_TRUE if set, otherwise LE_FALSE;
*/
virtual leBool getArcVisible(const leCircularSliderWidget* _this,
                             leCircularSliderWidgetArcType type);

/**
* @brief Set arc visibility.
* @details Sets arc visibility for
* <span class="param">axis</span> to <span class="param">type</span>
* using <span class="param">_this</span>.
* @code
* leCircularSliderWidget* _this;
* leCircularSliderWidgetArcType type;
* leResult res = _this->fn->setArcVisible(_this, type);
* @endcode
* @param _this is the widget pointer to modify.
* @param type the type to modify
* @returns LE_SUCCESS if set, otherwise LE_FAILURE.
*/
virtual leBool setArcVisible(const leCircularSliderWidget* _this,
                             leCircularSliderWidgetArcType type);

// *****************************************************************************
/**
 * @brief Get start value.
 * @details Gets the start value using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * int32_t start = _this->fn->getStartValue(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the start value.
 */
virtual uint32_t getStartValue(const leCircularSliderWidget* _this);


// *****************************************************************************
/**
 * @brief Set start value.
 * @details Sets the start value to
 * <span class="param">val</span> using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * int32_t val;
 * leResult res = _this->fn->setStartValue(_this, val);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param val is the gauge value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setStartValue(leCircularSliderWidget* _this,
                               uint32_t val);

/ *****************************************************************************
/**
 * @brief Get end value.
 * @details Gets the end value using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * int32_t end = _this->fn->getEndValue(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the end value.
 */
virtual uint32_t getEndValue(const leCircularSliderWidget* _this);


// *****************************************************************************
/**
 * @brief Set end value.
 * @details Sets the end value to
 * <span class="param">val</span> using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * int32_t val;
 * leResult res = _this->fn->setEndValue(_this, val);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param val is the end value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setEndValue(leCircularSliderWidget* _this,
                             uint32_t val);

// *****************************************************************************
/**
 * @brief Get rounded edge setting.
 * @details Gets rounded edge setting using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leBool round = _this->fn->getRoundEdges(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if rounded, otherwise LE_FALSE.
 */
virtual leBool getRoundEdges(const leCircularSliderWidget* _this);


// *****************************************************************************
/**
 * @brief Set rounded edge setting.
 * @details Sets the rounded edge setting to
 * <span class="param">val</span> using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leBool rnd;
 * leResult res = _this->fn->setRoundEdges(_this, rnd);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param rnd is the setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setRoundEdges(leCircularSliderWidget* _this,
                               leBool rnd);


// *****************************************************************************
/**
 * @brief Get sticky button setting.
 * @details Gets sticky button setting using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leBool sticky = _this->fn->getStickyButton(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if sticky, otherwise LE_FALSE.
 */
virtual leBool getStickyButton(const leCircularSliderWidget* _this);


// *****************************************************************************
/**
 * @brief Set sticky button setting.
 * @details Sets the sticky button setting to
 * <span class="param">stk</span> using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leBool stk;
 * leResult res = _this->fn->setStickyButton(_this, stk);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param stk is the setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setStickyButton(leCircularSliderWidget* _this,
                                 leBool stk);

// *****************************************************************************
/**
 * @brief Get touch on button setting.
 * @details Gets touch on button setting using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leBool tch = _this->fn->getTouchOnButtonOnly(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if tch, otherwise LE_FALSE.
 */
virtual leBool getTouchOnButtonOnly(const leCircularSliderWidget* _this);


// *****************************************************************************
/**
 * @brief Set touch on button setting.
 * @details Sets the touch on button setting to
 * <span class="param">stk</span> using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leBool stk;
 * leResult tch = _this->fn->setTouchOnButtonOnly(_this, v);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param stk is the setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTouchOnButtonOnly(leCircularSliderWidget* _this,
                                      leBool tch);


// *****************************************************************************
/**
 * @brief Get current direction.
 * @details Gets current direction using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leRotationDirection dir = _this->fn->getDirection(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns current direction.
 */
virtual leRotationDirection getDirection(const leCircularSliderWidget* _this);


// *****************************************************************************
/**
 * @brief Set current direction.
 * @details Sets the current direction to
 * <span class="param">stk</span> using <span class="param">_this</span>.
 * @code
 * const leCircularSliderWidget* _this;
 * leRotationDirection dir;
 * leResult tch = _this->fn->setDirection(_this, dir);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param dir is the setting.
 * @returns LE_SUCCESS if dir, otherwise LE_FAILURE.
 */
virtual leResult setDirection(leCircularSliderWidget* _this,
                              leRotationDirection dir);


/**
 * @brief Set pressed event callback.
 * @details Sets the pressed event callback to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leCircularSliderWidget* _this;
 * leCircularSliderWidget_PressedEvent cb;
 * leResult res = _this->fn->setPressedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPressedEventCallback(leCircularSliderWidget* _this,
                                         leCircularSliderWidget_PressedEvent cb);


/**
 * @brief Set value changed event callback.
 * @details Sets the value changed event callback to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leCircularSliderWidget* _this;
 * leCircularSliderWidget_ValueChangedEvent cb;
 * leResult res = _this->fn->setValueChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult setValueChangedEventCallback(leCircularSliderWidget* _this,
                                      leCircularSliderWidget_ValueChangedEvent cb)


/**
 * @brief Set released event callback.
 * @details Sets the released event callback to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leCircularSliderWidget* _this;
 * leCircularSliderWidget_ReleasedEvent cb;
 * leResult res = _this->fn->setReleasedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setReleasedEventCallback(leCircularSliderWidget* _this,
                                          leCircularSliderWidget_ReleasedEvent cb);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_CIRCULARSLIDER_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_CIRCULAR_SLIDER_H */
