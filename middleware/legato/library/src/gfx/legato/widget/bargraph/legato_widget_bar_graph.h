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
    legato_widget_bar_graph.h

  Summary:


  Description:
    This module implements bar graph drawing widget functions.
*******************************************************************************/
/** \file legato_widget_bar_graph.h
 * @brief Bar graph widget functions and definitions.
 *
 * @details This module implements bar graph drawing widget functions.
 */

#ifndef LEGATO_WIDGET_BAR_GRAPH_H
#define LEGATO_WIDGET_BAR_GRAPH_H

#include "gfx/legato/common/legato_common.h"

#if LE_BARGRAPH_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/string/legato_dynamicstring.h"
#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Enumeration:
    leBarGraphValueAxis

  Summary:
    The value axis index value
*/
/**
  * @brief This enum represents a bar graph value axis.
  * @details A bar graph value axis is used to define supported axis values.
  */
typedef enum leBarGraphValueAxis
{
    BAR_GRAPH_AXIS_0 = 0,
//  BAR_GRAPH_AXIS_1, //unsupported
} leBarGraphValueAxis;

// *****************************************************************************
/* Enumeration:
    leBarGraphTickPosition

  Summary:
    The tick position relative to axis
*/
/**
  * @brief This enum represents a bar graph tick position.
  * @details A bar graph tick position is used to define
  * supported tick positions.
  */
typedef enum leBarGraphTickPosition
{
    BAR_GRAPH_TICK_IN,
    BAR_GRAPH_TICK_OUT,
    BAR_GRAPH_TICK_CENTER,
} leBarGraphTickPosition;

// *****************************************************************************
/* Structure:
    leBarGraphDataSeries

  Summary:
    The data series object that contains the series properties and data

  Description:


  Remarks:
    None.
*/
/**
  * @brief This enum represents a bar graph data series.
  * @details A bar graph data series is used to define
  * series properties.
  */
typedef struct leBarGraphDataSeries
{
    const leScheme* scheme;
    leArray data;
    leBarGraphValueAxis axis;
} leBarGraphDataSeries;

typedef struct leBarGraphCategory
{
    const leString* str;
    uint32_t drawX;
} leBarGraphCategory;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leBarGraphWidget leBarGraphWidget;

#define LE_BARGRAPHWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t                (*getTickLength)(const THIS_TYPE* _this); \
    leResult                (*setTickLength)(THIS_TYPE* _this, uint32_t len); \
    uint32_t                (*getMinValue)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setMinValue)(THIS_TYPE* _this, leBarGraphValueAxis axis, int32_t min); \
    uint32_t                (*getMaxValue)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setMaxValue)(THIS_TYPE* _this, leBarGraphValueAxis axis, int32_t max); \
    leBool                  (*getValueAxisLabelsVisible)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setValueAxisLabelsVisible)(THIS_TYPE* _this, leBarGraphValueAxis axis, leBool vis); \
    leBool                  (*getValueAxisTicksVisible)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setValueAxisTicksVisible)(THIS_TYPE* _this, leBarGraphValueAxis axis, leBool vis); \
    uint32_t                (*getValueAxisTicksInterval)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setValueAxisTicksInterval)(THIS_TYPE* _this, leBarGraphValueAxis axis, uint32_t itv); \
    leBarGraphTickPosition  (*getValueAxisTicksPosition)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setValueAxisTicksPosition)(THIS_TYPE* _this, leBarGraphValueAxis axis, leBarGraphTickPosition pos); \
    leBool                  (*getValueAxisSubticksVisible)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setValueAxisSubticksVisible)(THIS_TYPE* _this, leBarGraphValueAxis axis, leBool vis); \
    uint32_t                (*getValueAxisSubticksInterval)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setValueAxisSubticksInterval)(THIS_TYPE* _this, leBarGraphValueAxis axis, uint32_t itv); \
    leBarGraphTickPosition  (*getValueAxisSubticksPosition)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setValueAxisSubticksPosition)(THIS_TYPE* _this, leBarGraphValueAxis axis, leBarGraphTickPosition pos); \
    leBool                  (*getCategoryAxisTicksVisible)(const THIS_TYPE* _this); \
    leResult                (*setCategoryAxisTicksVisible)(THIS_TYPE* _this, leBool vis); \
    leBarGraphTickPosition  (*getCategoryAxisTicksPosition)(const THIS_TYPE* _this); \
    leResult                (*setCategoryAxisTicksPosition)(THIS_TYPE* _this, leBarGraphTickPosition pos); \
    leBool                  (*getCategoryAxisLabelsVisible)(const THIS_TYPE* _this); \
    leResult                (*setCategoryAxisLabelsVisible)(THIS_TYPE* _this, leBool vis); \
    leResult                (*addSeries)(THIS_TYPE* _this, uint32_t* seriesID); \
    leScheme*               (*getSeriesScheme)(const THIS_TYPE* _this, uint32_t seriesID); \
    leResult                (*setSeriesScheme)(THIS_TYPE* _this, uint32_t seriesID, const leScheme* schm); \
    leResult                (*addDataToSeries)(THIS_TYPE* _this, uint32_t seriesID, int32_t val, uint32_t* idx); \
    leResult                (*setDataInSeries)(THIS_TYPE* _this, uint32_t seriesID, uint32_t index, int32_t value); \
    leResult                (*clearData)(THIS_TYPE* _this); \
    leFont*                 (*getTicksLabelFont)(THIS_TYPE* _this); \
    leResult                (*setTicksLabelFont)(THIS_TYPE* _this, const leFont* font); \
    leBool                  (*getGridLinesVisible)(const THIS_TYPE* _this, leBarGraphValueAxis axis); \
    leResult                (*setGridLinesVisible)(THIS_TYPE* _this, leBarGraphValueAxis axis, leBool vis); \
    leResult                (*addCategory)(THIS_TYPE* _this, uint32_t* id); \
    leString*               (*getCategoryString)(const THIS_TYPE* _this, uint32_t id); \
    leResult                (*setCategoryString)(THIS_TYPE* _this, uint32_t id, const leString* str); \
    leBool                  (*getStacked)(const THIS_TYPE* _this); \
    leResult                (*setStacked)(THIS_TYPE* _this, leBool stk); \
    leBool                  (*getFillGraphArea)(const THIS_TYPE* _this); \
    leResult                (*setFillGraphArea)(THIS_TYPE* _this, leBool fill); \


typedef struct leBarGraphWidgetVTable
{
    LE_BARGRAPHWIDGET_VTABLE(leBarGraphWidget)
} leBarGraphWidgetVTable;

/**
  * @endcond
  *
  */

// *****************************************************************************
/* Structure:
    leBarGraphWidget

  Summary:
    Implementation of a bar graph widget.

  Description:
    A bar graph widget draws a bar graph. All coordinates are expressed in local widget space.

    The color of the graph is determined by the widget scheme's 'foreground'
    color.

  Remarks:
    None.
*/
/**
 * @brief This struct represents a bar graph.
 * @details A bar graph widget draws a bar graph. All coordinates
 * are expressed in local widget space. The color of the graph is determined
 * by the widget scheme's 'foreground' color.
 */
typedef struct leBarGraphWidget
{
    leWidget widget; // base widget header

    const leBarGraphWidgetVTable* fn;

    uint32_t tickLength;
    leBool fillGraphArea;

    //Value axis properties
    int32_t maxValue;
    int32_t minValue;
    uint32_t tickInterval;
    uint32_t subtickInterval;
    leBool valueAxisLabelsVisible;
    leBool valueAxisTicksVisible;
    leBool valueAxisSubticksVisible;
    leBool valueGridLinesVisible;
    leBool stacked;
    leArray dataSeriesArray;
    const leFont* ticksLabelFont;
    leBarGraphTickPosition valueAxisTicksPosition;
    leBarGraphTickPosition valueAxisSubticksPosition;

    //Category axis properties
    leBool categAxisLabelsVisible;
    leBool categAxisTicksVisible;
    leBarGraphTickPosition categAxisTicksPosition;
    leArray categories;
} leBarGraphWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leBarGraphWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leBarGraphWidget* wgt = leBarGraphWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leBarGraphWidget* leBarGraphWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leBarGraphWidget <span class="param">wgt</span>.
 * @code
 * leBarGraphWidget* wgt;
 * leBarGraphWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leBarGraphWidget_Constructor(leBarGraphWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

/**
 * @brief Get tick length.
 * @details Gets tick length for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * uint32_t min = _this->fn->getTickLength(_this);
 * @endcode
 * @param _this is the widget to query.
 * @returns the tick length
 */
uint32_t getTickLength(const leBarGraphWidget* _this);


/**
 * @brief Set tick length.
 * @details Sets the tick length
 * to <span class="param">len</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * int32_t len;
 * leResult res = _this->fn->setTickLength(_this, len);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param len the tick length.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTickLength(leBarGraphWidget* _this,
                               uint32_t len);


/**
 * @brief Get minimum value.
 * @details Gets minimum value for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * uint32_t min = _this->fn->getMinValue(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis;
 * @returns the minimum value
 */
virtual uint32_t getMinValue(const leBarGraphWidget* _this,
                             leBarGraphValueAxis axis);

/**
 * @brief Set minimum value.
 * @details Sets the minimum value for <span class="param">axis</span>
 * to <span class="param">min</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * int32_t min;
 * leResult res = _this->fn->setMinValue(_this, axis, min);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the axis to modify.
 * @param min the minimum value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setMinValue(leBarGraphWidget* _this,
                             leBarGraphValueAxis axis,
                             int32_t min);


/**
 * @brief Get maximum value.
 * @details Gets maximum value for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * uint32_t max = _this->fn->getMaxValue(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis;
 * @returns the maximum value
 */
virtual uint32_t getMaxValue(const leBarGraphWidget* _this,
                             leBarGraphValueAxis axis);

/**
 * @brief Set maximum value.
 * @details Sets the maximum value for <span class="param">axis</span>
 * to <span class="param">max</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * int32_t max;
 * leResult res = _this->fn->setValueAxisLabelsVisible(_this, axis, max);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the axis to modify.
 * @param max the maximum value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setMaxValue(leBarGraphWidget* _this,
                             leBarGraphValueAxis axis,
                             int32_t max);

/**
 * @brief Get value axis label visiblity
 * @details Gets value axis label visiblity for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBool visible = _this->fn->getValueAxisLabelsVisible(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis;
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getValueAxisLabelsVisible(const leBarGraphWidget* _this,
                                         leBarGraphValueAxis axis);


/**
 * @brief Set value axis labels visibility.
 * @details Sets the value axis labels visibility for <span class="param">axis</span>
 * to <span class="param">vis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBool vis;
 * leResult res = _this->fn->setValueAxisLabelsVisible(_this, axis, vis);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the axis to modify.
 * @param vis the visbility setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueAxisLabelsVisible(leBarGraphWidget* _this,
                                           leBarGraphValueAxis axis,
                                           leBool vis);


/**
 * @brief Get value ticks visiblity
 * @details Gets value ticks visiblity for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBool visible = _this->fn->getValueAxisTicksVisible(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis;
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getValueAxisTicksVisible(const leBarGraphWidget* _this,
                                leBarGraphValueAxis axis);


/**
 * @brief Set value axis ticks visibility.
 * @details Sets the value axis ticks visibility for <span class="param">axis</span>
 * to <span class="param">vis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBool vis;
 * leResult res = _this->fn->setValueAxisTicksVisible(_this, axis, vis);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the axis to modify.
 * @param vis the visbility setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueAxisTicksVisible(leBarGraphWidget* _this,
                                          leBarGraphValueAxis axis,
                                          leBool vis);


/**
 * @brief Get value axis ticks interval.
 * @details Gets value axis ticks interval for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * uint32_t interval = _this->fn->getValueAxisTicksInterval(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis;
 * @returns value axis ticks position.
 */
virtual uint32_t getValueAxisTicksInterval(const leBarGraphWidget* _this,
                                           leBarGraphValueAxis axis);


/**
 * @brief Set inverval of the value axis ticks.
 * @details Sets the inverval of the value axis ticks for <span class="param">axis</span>
 * to <span class="param">itv</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBarGraphTickPosition pos;
 * leResult res = _this->fn->setValueAxisTicksInterval(_this, axis, itv);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the axis to modify.
 * @param itv the interval value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueAxisTicksInterval(leBarGraphWidget* _this,
                                           leBarGraphValueAxis axis,
                                           uint32_t itv);



/**
 * @brief Get value axis ticks position.
 * @details Gets value axis ticks position for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBarGraphTickPosition pos = _this->fn->getValueAxisTicksPosition(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis;
 * @returns value axis ticks position.
 */
virtual leBarGraphTickPosition getValueAxisTicksPosition(const leBarGraphWidget* _this,
                                                         leBarGraphValueAxis axis);

/**
 * @brief Set value axis ticks position.
 * @details Sets value axis ticks position for <span class="param">axis</span>
 * to <span class="param">itv</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBarGraphTickPosition pos;
 * leResult res = _this->fn->setValueAxisTicksPosition(_this, axis, pos);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the axis to modify.
 * @param pos the position value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueAxisTicksPosition(leBarGraphWidget* _this,
                                           leBarGraphValueAxis axis,
                                           leBarGraphTickPosition pos);



/**
 * @brief Get value axis subticks visibility
 * @details Gets value axis subticks visibility for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBool visible = _this->fn->getValueAxisSubticksVisible(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis;
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getValueAxisSubticksVisible(const leBarGraphWidget* _this,
                                           leBarGraphValueAxis axis);


/**
 * @brief Set value axis subticks visibility.
 * @details Sets value axis subticks visibility for <span class="param">axis</span>
 * to <span class="param">itv</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBool vis;
 * leResult res = _this->fn->setValueAxisSubticksVisible(_this, axis, vis);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the axis to modify.
 * @param vis the visbility setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueAxisSubticksVisible(leBarGraphWidget* _this,
                                             leBarGraphValueAxis axis,
                                             leBool vis);



/**
 * @brief Get value axis subticks interval
 * @details Gets value axis subticks interval for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * uint32_t interval = _this->fn->getValueAxisSubticksInterval(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis;
 * @returns value axis subticks interval.
 */
virtual uint32_t getValueAxisSubticksInterval(const leBarGraphWidget* _this,
                                              leBarGraphValueAxis axis);


/**
 * @brief Set value axis subticks interval.
 * @details Sets the value axis subticks interval for <span class="param">axis</span>
 * to <span class="param">itv</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * uint32_t itv;
 * leResult res = _this->fn->setValueAxisSubticksInterval(_this, axis, itv);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the visbility setting.
 * @param itv the interval value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueAxisSubticksInterval(leBarGraphWidget* _this,
                                              leBarGraphValueAxis axis,
                                              uint32_t itv);


/**
 * @brief Get value axis subticks position.
 * @details Gets value axis subticks position for <span class="param">axis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBarGraphTickPosition pos = _this->fn->getValueAxisSubticksPosition(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis;
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBarGraphTickPosition getValueAxisSubticksPosition(const leBarGraphWidget* _this,
                                                            leBarGraphValueAxis axis);


/**
 * @brief Set value axis subticks position
 * @details Sets the value axis subticks position for <span class="param">axis</span>
 * to <span class="param">pos</span> using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBarGraphTickPosition pos;
 * leResult res = _this->fn->setValueAxisSubticksPosition(_this, axis, pos);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the axis;
 * @param pos the position
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueAxisSubticksPosition(leBarGraphWidget* _this,
                                      leBarGraphValueAxis axis,
                                      leBarGraphTickPosition pos);


/**
 * @brief Get category axis ticks visibility.
 * @details Gets the category axis ticks visibility
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBool* visiblity = _this->fn->getCategoryAxisTicksVisible(_this);
 * @endcode
 * @param _this is the widget to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getCategoryAxisTicksVisible(const leBarGraphWidget* _this);

/**
 * @brief Set category axis ticks visibility
 * @details Sets the category axis ticks visibility to <span class="param">vis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphTickPosition pos;
 * leResult res = _this->fn->setCategoryAxisTicksVisible(_this, vis);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param vis the visbility setting
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCategoryAxisTicksVisible(leBarGraphWidget* _this,
                                     leBool vis);

/**
 * @brief Get category axis ticks position.
 * @details Gets the category axis ticks position
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphTickPosition* pos = _this->fn->getSeriesScheme(_this);
 * @endcode
 * @param _this is the widget to query.
 * @returns the axis ticks position.
 */
virtual leBarGraphTickPosition getCategoryAxisTicksPosition(const leBarGraphWidget* _this);

/**
 * @brief Set category axis label visibility
 * @details Sets the category axis label visibility to <span class="param">vis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphTickPosition pos;
 * leResult res = _this->fn->setCategoryAxisTicksPosition(_this, pos);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param pos the position value
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCategoryAxisTicksPosition(leBarGraphWidget* _this,
                                      leBarGraphTickPosition pos)

/**
 * @brief Set category axis label visibility
 * @details Sets the category axis label visibility to  <span class="param">vis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * uint32_t seriesID;
 * const leScheme* schm;
 * leResult res = _this->fn->setSeriesScheme(_this, seriesID, schm);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param seriesID the series ID to modify
 * @param schm  the scheme to use
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCategoryAxisLabelsVisible(leBarGraphWidget* _this,
                                      leBool vis)

/**
 * @brief Add series to graph
 * @details Adds a series by <span class="param">seriesID</span> using
 * <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * uint32_t seriesID;
 * leResult res = _this->fn->addSeries(_this, seriesID);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param seriesID the series ID to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult addSeries(leBarGraphWidget* _this,
                           uint32_t* seriesID);

/**
 * @brief Get series scheme.
 * @details Gets the series scheme by
 * <span class="param">seriesID</span> using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * uint32_t seriesID;
 * leScheme* schm = _this->fn->getSeriesScheme(_this, seriesID);
 * @endcode
 * @param _this is the widget to query.
 * @returns the series scheme.
 */
virtual leScheme* getSeriesScheme(const leBarGraphWidget* _this,
                                  uint32_t seriesID);

/**
 * @brief Set seriesID scheme
 * @details Sets the scheme by
 * <span class="param">seriesID</span> to
 * <span class="param">schm</span> using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * uint32_t seriesID;
 * const leScheme* schm;
 * leResult res = _this->fn->setSeriesScheme(_this, seriesID, schm);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param seriesID the series ID to modify
 * @param schm  the scheme to use
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult setSeriesScheme(leBarGraphWidget* _this,
                         uint32_t seriesID,
                         const leScheme* schm);

/**
 * @brief Set data in a series.
 * @details Sets the data in the series by
 * <span class="param">seriesID</span> and <span class="param">index</span> to
 * <span class="param">value</span> using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * uint32_t seriesID;
 * uint32_t index;
 * int32_t value;
 * leResult res = _this->fn->setDataInSeries(_this, seriesID, index, value);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param seriesID the series index to modify
 * @param index the index value to modify
 * @param value - the value to set
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
 leResult setDataInSeries(leBarGraphWidget* _this,
                         uint32_t seriesID,
                         uint32_t index,
                         int32_t value)

/**
 * @brief Clear series data.
 * @details Clears series data using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leResult res = _this->fn->clearData(_this);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult clearData(leBarGraphWidget* _this);


/**
 * @brief Get ticks label font.
 * @details Gets the ticks label font using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leFont* font = _this->fn->getTicksLabelFont(_this);
 * @endcode
 * @param _this is the widget to query.
 * @returns the font in use
 */
virtual leFont* getTicksLabelFont(leBarGraphWidget* _this);


/**
 * @brief Set tick label font
 * @details Sets the tick label font to  <span class="param">font</span> using
 * <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * const leFont* font;
 * leResult res = _this->fn->setTicksLabelFont(_this, font);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param font the font to use
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTicksLabelFont(const leBarGraphWidget* _this,
                                   const leFont* font);

/**
 * @brief Get grid lines visibility
 * @details Gets grid lines visilibilty for <span class="param">axis</span> using
 * <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis;
 * leBool visible = _this->fn->getGridLinesVisible(_this, axis);
 * @endcode
 * @param _this is the widget to query.
 * @param axis the axis to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getGridLinesVisible(const leBarGraphWidget* _this,
                                   leBarGraphValueAxis axis);

/**
 * @brief Set grid lines visibility.
 * @details Sets grid lines visibility for
 * <span class="param">axis</span> to <span class="param">vis</span>
 * using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBarGraphValueAxis axis
 * leBool vis;
 * leResult res = _this->fn->setGridLinesVisible(_this, axis, vis);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param axis the axis to modify
 * @param vis the visbility setting
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setGridLinesVisible(leBarGraphWidget* _this,
                                     leBarGraphValueAxis axis,
                                     leBool vis);

/**
 * @brief Adds data category
 * @details Adds a data category by <span class="param">id</span> using
 * <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * uint32_t id;
 * leResult res = _this->fn->setCategoryString(_this, id);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param id the ID of the category to add
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult addCategory(leBarGraphWidget* _this,
                     uint32_t* id);

/**
 * @brief Get category string
 * @details Gets a category string by <span class="param">id</span> using
 * <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * uint32_t id;
 * leString* str = _this->fn->getCategoryString(_this, id);
 * @endcode
 * @param _this is the widget to query.
 * @param id the ID of the category.
 * @returns the string in use
 */
virtual leString* getCategoryString(const leBarGraphWidget* _this,
                                    uint32_t id);

/**
 * @brief Set category string
 * @details Sets the category string to <span class="param">str</span> using
 * <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * uint32_t id;
 * const leString* str;
 * leResult res = _this->fn->setCategoryString(_this, id, str);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param id the ID of the category to modify
 * @param str the string to use
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCategoryString(leBarGraphWidget* _this,
                                   uint32_t id,
                                   const leString* str);

/**
 * @brief Get stacked setting
 * @details Gets stacked setting using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBool fill = _this->fn->getRadius(_this);
 * @endcode
 * @param _this is the widget to operate on.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getStacked(const leBarGraphWidget* _this);

/**
 * @brief Set stacked setting
 * @details Sets the stacked setting to <span class="param">stk</span> using
 * <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBool stk;
 * leResult res = _this->fn->setStacked(_this, stk);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param stk the stacked setting
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setStacked(leBarGraphWidget* _this,
                            leBool stk);

/**
 * @brief Get fill for graph area.
 * @details Gets the fill for graph area using <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leBool fill = _this->fn->getRadius(_this);
 * @endcode
 * @param _this is the widget to operate on.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getFillGraphArea(const leBarGraphWidget* _this);

/**
 * @brief Set fill for graph area.
 * @details Sets the fill for graph area to <span class="param">fill</span> using
 * <span class="param">_this</span>.
 * @code
 * leBarGraphWidget* _this;
 * leResult res = _this->fn->setRoundEdge(_this, fill);
 * @endcode
 * @param _this is the widget pointer to modify.
 * @param fill the fill setting
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFillGraphArea(THIS_TYPE* _this,
                             leBool fill);
#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_BARGRAPH_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_BAR_GRAPH_H */
