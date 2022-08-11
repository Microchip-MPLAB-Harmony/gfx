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
    legato_widget_circular_gauge.h

  Summary:


  Description:
    This module implements circular gauge drawing widget functions.
*******************************************************************************/
/** \file legato_widget_circular_gauge.h
 * @brief Circular guage widget functions and definitions.
 *
 * @details This module implements circular gauge drawing widget functions.
 */

#ifndef LEGATO_WIDGET_CIRCULAR_GAUGE_H
#define LEGATO_WIDGET_CIRCULAR_GAUGE_H

#include "gfx/legato/common/legato_common.h"

#if LE_CIRCULARGAUGE_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

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
/* Function Pointer:
    leCircularGaugeWidget_ValueChangedEvent

  Summary:
    Value changed event function callback type
*/
/**
 * @brief This function represents a value changed event callback.
 * @details Value change event callback is used indicate a value change.
 */
typedef void (* leCircularGaugeWidget_ValueChangedEvent)(leCircularGaugeWidget*,
                                                         int32_t value);


/* internal use only */
/**
  * @cond INTERNAL
  *
  */
#define LE_CIRCULARGAUGEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t                 (*getRadius)(const THIS_TYPE* _this); \
    leResult                 (*setRadius)(THIS_TYPE* _this, uint32_t rad); \
    int32_t                  (*getStartAngle)(const THIS_TYPE* _this); \
    leResult                 (*setStartAngle)(THIS_TYPE* _this, int32_t ang); \
    int32_t                  (*getCenterAngle)(const THIS_TYPE* _this); \
    leResult                 (*setCenterAngle)(THIS_TYPE* _this, int32_t ang); \
    uint32_t                 (*getValue)(const THIS_TYPE* _this); \
    leResult                 (*setValue)(THIS_TYPE* _this, uint32_t val); \
    \
    int32_t                  (*addArc)(THIS_TYPE* _this, uint32_t startAngle, uint32_t endAngle, int32_t offset, const leScheme* schm, uint32_t thk); \
    leResult                 (*deleteArc)(THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*deleteAllArcs)(THIS_TYPE* _this); \
    uint32_t                 (*arcCount)(const THIS_TYPE* _this); \
    leBool                   (*getArcEnabled)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setArcEnabled)(THIS_TYPE* _this, uint32_t idx, leBool b); \
    uint32_t                 (*getArcStartAngle)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setArcStartAngle)(THIS_TYPE* _this, uint32_t idx, uint32_t val); \
    uint32_t                 (*getArcEndAngle)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setArcEndAngle)(THIS_TYPE* _this, uint32_t idx, uint32_t val); \
    int32_t                  (*getArcOffset)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setArcOffset)(THIS_TYPE* _this, uint32_t idx, int32_t offs); \
    const leScheme*          (*getArcScheme)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setArcScheme)(THIS_TYPE* _this, uint32_t idx, const leScheme* schm); \
    uint32_t                 (*getArcThickness)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setArcThickness)(THIS_TYPE* _this, uint32_t idx, uint32_t thk); \
    \
    int32_t                  (*addTickRange)(THIS_TYPE* _this, uint32_t startAngle, uint32_t endAngle, int32_t offset, const leScheme* schm, uint32_t thk, uint32_t div); \
    leResult                 (*deleteTickRange)(THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*deleteAllTickRanges)(THIS_TYPE* _this); \
    uint32_t                 (*tickRangeCount)(const THIS_TYPE* _this); \
    leBool                   (*getTickRangeEnabled)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setTickRangeEnabled)(THIS_TYPE* _this, uint32_t idx, leBool b); \
    uint32_t                 (*getTickRangeStartAngle)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setTickRangeStartAngle)(THIS_TYPE* _this, uint32_t idx, uint32_t val); \
    uint32_t                 (*getTickRangeEndAngle)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setTickRangeEndAngle)(THIS_TYPE* _this, uint32_t idx, uint32_t val); \
    int32_t                  (*getTickRangeOffset)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setTickRangeOffset)(THIS_TYPE* _this, uint32_t idx, int32_t offs); \
    const leScheme*          (*getTickRangeScheme)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setTickRangeScheme)(THIS_TYPE* _this, uint32_t idx, const leScheme* schm); \
    uint32_t                 (*getTickRangeThickness)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setTickRangeThickness)(THIS_TYPE* _this, uint32_t idx, uint32_t thk); \
    uint32_t                 (*getTickRangeDivisions)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setTickRangeDivisions)(THIS_TYPE* _this, uint32_t idx, uint32_t div); \
    \
    int32_t                  (*addLabelRange)(THIS_TYPE* _this, uint32_t startVal, uint32_t endVal, int32_t offset, const leScheme* schm, const leFont* fnt, uint32_t div); \
    leResult                 (*deleteLabelRange)(THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*deleteAllLabelRanges)(THIS_TYPE* _this); \
    uint32_t                 (*labelRangeCount)(const THIS_TYPE* _this); \
    leBool                   (*getLabelRangeEnabled)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setLabelRangeEnabled)(THIS_TYPE* _this, uint32_t idx, leBool b); \
    uint32_t                 (*getLabelRangeStartAngle)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setLabelRangeStartAngle)(THIS_TYPE* _this, uint32_t idx, uint32_t val); \
    uint32_t                 (*getLabelRangeEndAngle)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setLabelRangeEndAngle)(THIS_TYPE* _this, uint32_t idx, uint32_t val); \
    int32_t                  (*getLabelRangeOffset)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setLabelRangeOffset)(THIS_TYPE* _this, uint32_t idx, int32_t offs); \
    const leScheme*          (*getLabelRangeScheme)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setLabelRangeScheme)(THIS_TYPE* _this, uint32_t idx, const leScheme* schm); \
    const leFont*            (*getLabelRangeFont)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setLabelRangeFont)(THIS_TYPE* _this, uint32_t idx, const leFont* font); \
    uint32_t                 (*getLabelRangeDivisions)(const THIS_TYPE* _this, uint32_t idx); \
    leResult                 (*setLabelRangeDivisions)(THIS_TYPE* _this, uint32_t idx, uint32_t div); \
    \
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
    leResult                 (*setValueChangedEventCallback)(THIS_TYPE* _this, leCircularGaugeWidget_ValueChangedEvent cb); \

typedef struct leCircularGaugeWidgetVTable
{
    LE_CIRCULARGAUGEWIDGET_VTABLE(leCircularGaugeWidget)
} leCircularGaugeWidgetVTable;

/**
  * @endcond
  *
  */

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
/**
 * @brief This struct represents a circular gauge.
 * @details A circular gauge widget draws a circular gauge of the specified
 * properties inside the widget bounds.  All coordinates are expressed in local
 * widget space.
 */
typedef struct leCircularGaugeWidget
{
    leWidget widget; // base widget header

    const leCircularGaugeWidgetVTable* fn;

    //Widget properties
    uint32_t value;
    uint32_t radius; // the radius of the circular gauge
    int32_t startAngle; //the start angle of the arc
    int32_t centerAngle; //the center/span angle of the outer arc

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
    Contains state values for a circular gauge arc

  Remarks:
    None.
*/
/**
  * @brief This struct represents circular gauge widget.
  * @details A circular gauge widget is used to describe the
  * arc instances in the circular gauge widget.
  */
typedef struct leCircularGaugeArc
{
    leBool enabled;
    uint32_t startAngle;
    uint32_t endAngle;
    int32_t offset;
    const leScheme* scheme;
    uint32_t thickness;
} leCircularGaugeArc;

// *****************************************************************************
/* Structure:
    leCircularGaugeTickRange

  Summary:
    Tick range for a circular gauge

  Description:
    Contains properties of a circular gauge tick range

  Remarks:
    None.
*/
/**
  * @brief This struct represents circular gauge tick.
  * @details A circular gauge tick is used to
  * contain properties of the ticks in the gauge.
  */
typedef struct leCircularGaugeTickRange
{
    leBool enabled;
    uint32_t startAngle;
    uint32_t endAngle;
    int32_t offset;
    const leScheme* scheme;
    uint32_t thickness;
    uint32_t divisions;
} leCircularGaugeTickRange;

// *****************************************************************************
/* Structure:
    typedef struct leCircularGaugeLabel

  Summary:
    Contains state values for a circular gauge label range

  Description:
    Contains properties of the labels in the gauge

  Remarks:
    None.
*/
/**
  * @brief This struct represents circular gauge label.
  * @details A circular gauge tick is used to
  * contain properties of the labels in the gauge.
  */
typedef struct leCircularGaugeLabelRange
{
    leBool enabled;
    uint32_t startAngle;
    uint32_t endAngle;
    int32_t offset;
    const leScheme* scheme;
    const leFont* font;
    uint32_t divisions;
} leCircularGaugeLabelRange;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leCircularGaugeWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leCircularGaugeWidget* wgt = leCircularGaugeWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leCircularGaugeWidget* leCircularGaugeWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leCircularGaugeWidget <span class="param">wgt</span>.
 * @code
 * leCircularGaugeWidget* wgt;
 * leCircularGaugeWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leCircularGaugeWidget_Constructor(leCircularGaugeWidget* gauge);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Get radius.
 * @details Gets the radius using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * int32_t angle = _this->fn->getRadius(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the radius.
 */
virtual uint32_t getRadius(const leCircularGaugeWidget* _this);


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
/**
 * @brief Set radius.
 * @details Sets the radius to <span class="param">ang</span>
 * using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * uint32_t rad;
 * leResult res = _this->fn->setStartAngle(_this, rad);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param rad is the start angle.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setRadius(leCircularGaugeWidget* _this,
                           uint32_t rad);

// *****************************************************************************
/**
 * @brief Get start angle.
 * @details Gets the start angle using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * int32_t angle = _this->fn->getStartAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the start angle.
 */
virtual int32_t getStartAngle(const leCircularGaugeWidget* _this);


// *****************************************************************************
/**
 * @brief Set start angle.
 * @details Sets the start angle to <span class="param">ang</span>
 * using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * int32_t ang;
 * leResult res = _this->fn->setStartAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param ang is the start angle.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setStartAngle(leCircularGaugeWidget* _this,
                               int32_t ang);

// *****************************************************************************
/**
 * @brief Get center angle.
 * @details Gets the center angle using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * int32_t angle = _this->fn->getCenterAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the center angle.
 */
virtual int32_t getCenterAngle(const leCircularGaugeWidget* _this);


// *****************************************************************************
/**
 * @brief Set center angle.
 * @details Sets the center angle to
 * <span class="param">ang</span> using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * int32_t ang;
 * leResult res = _this->fn->setStartAngle(_this, ang);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param ang is the center angle.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCenterAngle(leCircularGaugeWidget* _this,
                                int32_t ang);



// *****************************************************************************
/**
 * @brief Get tick labels visibility setting.
 * @details Gets the tick labels visibility setting using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * leBool visible = _this->fn->getHandVisible(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the tick labels visibility setting.
 */
virtual leBool getHandVisible(const leCircularGaugeWidget* _this);

// *****************************************************************************
/**
 * @brief Set visibility setting for the tick labels.
 * @details Sets the visibility setting for the tick labels to
 * <span class="param">vis</span> using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * leBool vis;
 * leResult res = _this->fn->setTicksVisible(_this, vis);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param vis is the visibility setting for the tick labels.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTickLabelsVisible(leCircularGaugeWidget* _this,
                                      leBool vis);

// *****************************************************************************
/**
 * @brief Get hand visibility setting.
 * @details Gets the visibility setting using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * leBool visible = _this->fn->getHandRadius(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the visibility setting.
 */
virtual leBool getHandVisible(const leCircularGaugeWidget* _this);

// *****************************************************************************
/**
 * @brief Set hand visibility setting for the tick labels.
 * @details Sets the hand visibility setting to
 * <span class="param">vis</span> using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * leBool vis;
 * leResult res = _this->fn->setHandVisible(_this, vis);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param vis is the visibility setting for the tick labels.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setHandVisible(leCircularGaugeWidget* _this,
                                      leBool vis);

// *****************************************************************************
/**
 * @brief Get hand radius setting.
 * @details Gets the hand radius using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * leBool visible = _this->fn->getHandRadius(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the hand radius.
 */
virtual uint32_t getHandRadius(const leCircularGaugeWidget* _this);

// *****************************************************************************
/**
 * @brief Set hand radius for the tick labels.
 * @details Sets the hand radius to
 * <span class="param">rad</span> using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * uint32_t rad;
 * leResult res = _this->fn->setHandRadius(_this, rad);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param rad is the hand radius.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setHandRadius(leCircularGaugeWidget* _this,
                              uint32_t rad);


// *****************************************************************************
/**
 * @brief Get center circle visibility setting.
 * @details Gets the center circle visibility setting using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * leBool visible = _this->fn->getCenterCircleVisible(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the center circle visibility setting.
 */
virtual leBool getCenterCircleVisible(const leCircularGaugeWidget* _this);


// *****************************************************************************
/**
 * @brief Set center circle visibility setting.
 * @details Sets the center circle visibility setting to
 * <span class="param">vis</span> using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * leBool vis;
 * leResult res = _this->fn->setCenterCircleVisible(_this, vis);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param vis is the center circle visibility setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCenterCircleVisible(leCircularGaugeWidget* _this,
                                        leBool vis);

// *****************************************************************************
/**
 * @brief Get center circle radius.
 * @details Gets the center circle radius using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * uint32_t radius = _this->fn->getCenterCircleRadius(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the radius value.
 */
virtual uint32_t getCenterCircleRadius(const leCircularGaugeWidget* _this);


// *****************************************************************************
/**
 * @brief Set center circle radius.
 * @details Sets the center circle radius to
 * <span class="param">rad</span> using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * uint32_t rad;
 * leResult res = _this->fn->setCenterCircleRadius(_this, rad);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param rad center circle radius.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCenterCircleRadius(leCircularGaugeWidget* _this,
                                       uint32_t rad);


// *****************************************************************************
/**
 * @brief Get center circle thickness.
 * @details Gets the center circle thickness using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * uint32_t thickness = _this->fn->getCenterCircleThickness(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the thickness value.
 */
virtual uint32_t getCenterCircleThickness(const leCircularGaugeWidget* _this);


// *****************************************************************************
/**
 * @brief Set center circle thickness.
 * @details Sets the center circle thickness to
 * <span class="param">rad</span> using <span class="param">_this</span>.
 * @code
 * const leCircularGaugeWidget* _this;
 * uint32_t rad;
 * leResult res = _this->fn->setCenterCircleThickness(_this, rad);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param rad center circle thickness.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCenterCircleThickness(leCircularGaugeWidget* _this,
                                       uint32_t rad);

/**
 * @brief Set value changed callback.
 * @details Sets the value changed callback to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leCircularGaugeWidget* _this;
 * leCircularGaugeWidget_ValueChangedEvent cb;
 * leResult res = _this->fn->setValueChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueChangedEventCallback(leCircularGaugeWidget* _this,
                                              leCircularGaugeWidget_ValueChangedEvent cb);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_CIRCULARGAUGE_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_CIRCULAR_GAUGE_H */
