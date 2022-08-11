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
    legato_widget_line_graph.h

  Summary:


  Description:
    This module implements line graph drawing widget functions.
*******************************************************************************/
/** \file legato_widget_line_graph.h
 * @brief Line graph functions and definitions.
 *
 * @details This module implements line graph drawing widget functions.
 */

#ifndef LEGATO_WIDGET_LINE_GRAPH_H
#define LEGATO_WIDGET_LINE_GRAPH_H

#include "gfx/legato/common/legato_common.h"

#if LE_LINEGRAPH_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leLineGraphValueAxis

  Summary:
    The value axis index value
*/
typedef enum leLineGraphValueAxis
{
    LINE_GRAPH_AXIS_0 = 0,
//  LINE_GRAPH_AXIS_1, //unsupported
} leLineGraphValueAxis;

// *****************************************************************************
/* Enumeration:
    leLineGraphTickPosition

  Summary:
    The tick position relative to axis
*/
typedef enum leLineGraphTickPosition
{
    LINE_GRAPH_TICK_IN,
    LINE_GRAPH_TICK_OUT,
    LINE_GRAPH_TICK_CENTER,
} leLineGraphTickPosition;

// *****************************************************************************
/* Enumeration:
    leLineGraphDataPointType

  Summary:
    The graph data point type
*/
typedef enum leLineGraphDataPointType
{
    LINE_GRAPH_DATA_POINT_NONE,
    LINE_GRAPH_DATA_POINT_CIRCLE,
    LINE_GRAPH_DATA_POINT_SQUARE,
} leLineGraphDataPointType;

// *****************************************************************************
/* Structure:
    leLineGraphDataSeries

  Summary:
    The data series object that contains the series properties and data

  Description:


  Remarks:
    None.
*/
typedef struct leLineGraphDataSeries
{
    const leScheme* scheme;
    leArray data;
    leLineGraphValueAxis axis;
    leLineGraphDataPointType pointType;
    uint32_t pointSize;
    leBool fillPoints;
    leBool drawLines;
} leLineGraphDataSeries;

// *****************************************************************************
/* Structure:
    leLineGraphCategory

  Summary:
    Contains the Category properties

  Description:


  Remarks:
    None.
*/
typedef struct leLineGraphCategory
{
    const leString* text;
    int32_t stackValue;
    int32_t drawX;
} leLineGraphCategory;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
typedef struct leLineGraphWidget leLineGraphWidget;

#define LE_LINEGRAPHWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t                 (*getTickLength)(const THIS_TYPE* _this); \
    leResult                 (*setTickLength)(THIS_TYPE* _this, uint32_t len); \
    leBool                   (*getStacked)(const THIS_TYPE* _this); \
    leResult                 (*setStacked)(THIS_TYPE* _this, leBool stk); \
    int32_t                  (*getMaxValue)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setMaxValue)(THIS_TYPE* _this, leLineGraphValueAxis axis, int32_t val); \
    int32_t                  (*getMinValue)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setMinValue)(THIS_TYPE* _this, leLineGraphValueAxis axis, int32_t val); \
    leBool                   (*getValueAxisLabelsVisible)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setValueAxisLabelsVisible)(THIS_TYPE* _this, leLineGraphValueAxis axis, leBool vis); \
    leBool                   (*getFillGraphArea)(const THIS_TYPE* _this); \
    leResult                 (*setFillGraphArea)(THIS_TYPE* _this, leBool fill); \
    leBool                   (*getFillSeriesArea)(const THIS_TYPE* _this); \
    leResult                 (*setFillSeriesArea)(THIS_TYPE* _this, leBool fill); \
    leBool                   (*getGridLinesVisible)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setGridLinesVisible)(THIS_TYPE* _this, leLineGraphValueAxis axis, leBool visible); \
    leBool                   (*getValueAxisTicksVisible)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setValueAxisTicksVisible)(THIS_TYPE* _this, leLineGraphValueAxis axis, leBool vis); \
    uint32_t                 (*getValueAxisTickInterval)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setValueAxisTickInterval)(THIS_TYPE* _this, leLineGraphValueAxis axis, uint32_t inv); \
    uint32_t                 (*getValueAxisSubtickInterval)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setValueAxisSubtickInterval)(THIS_TYPE* _this, leLineGraphValueAxis axis, uint32_t inv); \
    leBool                   (*getValueAxisSubticksVisible)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setValueAxisSubticksVisible)(THIS_TYPE* _this, leLineGraphValueAxis axis, leBool vis); \
    leBool                   (*getCategoryAxisTicksVisible)(const THIS_TYPE* _this); \
    leResult                 (*setCategoryAxisTicksVisible)(THIS_TYPE* _this, leBool vis); \
    int32_t                  (*addCategory)(THIS_TYPE* _this); \
    leString*                (*getCategoryString)(const THIS_TYPE* _this, int32_t categoryID); \
    leResult                 (*setCategoryString)(THIS_TYPE* _this, int32_t categoryID, const leString* str); \
    int32_t                  (*addSeries)(THIS_TYPE* _this); \
    int32_t                  (*addDataToSeries)(THIS_TYPE* _this, int32_t seriesID, int32_t val); \
    leResult                 (*setDataInSeries)(THIS_TYPE* _this, int32_t seriesID, int32_t idx, int32_t val); \
    leScheme*                (*getSeriesScheme)(const THIS_TYPE* _this, int32_t seriesID); \
    leResult                 (*setSeriesScheme)(THIS_TYPE* _this, int32_t seriesID, const leScheme* schm); \
    leBool                   (*getSeriesFillPoints)(const THIS_TYPE* _this, int32_t seriesID); \
    leResult                 (*setSeriesFillPoints)(THIS_TYPE* _this, int32_t seriesID, leBool fill); \
    leBool                   (*getSeriesLinesVisible)(const THIS_TYPE* _this, int32_t seriesID); \
    leResult                 (*setSeriesLinesVisible)(THIS_TYPE* _this, int32_t seriesID, leBool vis); \
    leLineGraphDataPointType (*getSeriesPointType)(const THIS_TYPE* _this, int32_t seriesID); \
    leResult                 (*setSeriesPointType)(THIS_TYPE* _this, int32_t seriesID, leLineGraphDataPointType type); \
    uint32_t                 (*getSeriesPointSize)(const THIS_TYPE* _this, int32_t seriesID); \
    leResult                 (*setSeriesPointSize)(THIS_TYPE* _this, int32_t seriesID, uint32_t size); \
    leResult                 (*clear)(THIS_TYPE* _this); \
    leResult                 (*setTicksLabelFont)(THIS_TYPE* _this, const leFont* font); \
    leBool                   (*getCategoryAxisLabelsVisible)(const THIS_TYPE* _this); \
    leResult                 (*setCategoryAxisLabelsVisible)(THIS_TYPE* _this, leBool vis); \
    leLineGraphTickPosition  (*getValueAxisTicksPosition)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setValueAxisTicksPosition)(THIS_TYPE* _this, leLineGraphValueAxis axis, leLineGraphTickPosition position); \
    leLineGraphTickPosition  (*getValueAxisSubticksPosition)(const THIS_TYPE* _this, leLineGraphValueAxis axis); \
    leResult                 (*setValueAxisSubticksPosition)(THIS_TYPE* _this, leLineGraphValueAxis axis, leLineGraphTickPosition position); \
    leLineGraphTickPosition  (*getCategoryAxisTicksPosition)(const THIS_TYPE* _this); \
    leResult                 (*setCategoryAxisTicksPosition)(THIS_TYPE* _this, leLineGraphTickPosition position); \

typedef struct leLineGraphWidgetVTable
{
    LE_LINEGRAPHWIDGET_VTABLE(leLineGraphWidget)
} leLineGraphWidgetVTable;

/**
  * @endcond
  *
  */

// *****************************************************************************
/**
 * @brief This struct represents a line graph widget
 * @details A line graph widget draws a line graph. All coordinates are
 * expressed in local widget space. The color of the graph is determined by
 * the widget scheme's 'foreground' color..
 */
typedef struct leLineGraphWidget
{
    leWidget widget; // base widget header

    const leLineGraphWidgetVTable* fn;

    uint32_t tickLength;
    leBool fillGraphArea;
    leBool fillValueArea;

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
    leArray dataSeries;
    const leFont* ticksLabelFont; // ticks label font
    leLineGraphTickPosition valueAxisTicksPosition;
    leLineGraphTickPosition valueAxisSubticksPosition;

    //Category axis properties
    leBool categAxisLabelsVisible;
    leBool categAxisTicksVisible;
    leLineGraphTickPosition categAxisTicksPosition;
    leArray categories;
} leLineGraphWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leLineGraphWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leLineGraphWidget* wgt = leLineGraphWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leLineGraphWidget* leLineGraphWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leLineGraphWidget <span class="param">wgt</span>.
 * @code
 * leLineGraphWidget* wgt;
 * leLineGraphWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leLineGraphWidget_Constructor(leLineGraphWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getTickLength(const leLineGraphWidget* _this)

  Summary:
     Gets the tick length

  Description:
     Gets the tick length

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on

  Remarks:
    Usage - _this->fn->getTickLength(_this);

  Returns:
    uint32_t - the length value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTickLength(leLineGraphWidget* _this,
                           uint32_t len)

  Summary:
     Sets the tick length

  Description:
     Sets the tick length

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    uint32_t len - the length value

  Remarks:
    Usage - _this->fn->setTickLength(_this, len);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getStacked(const leLineGraphWidget* _this)

  Summary:
     Gets the stacked setting

  Description:
     Gets the stacked setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on

  Remarks:
    Usage - _this->fn->getStacked(_this);

  Returns:
    leBool - the setting value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStacked(leLineGraphWidget* _this,
                        leBool stk)

  Summary:
     Sets the stacked setting

  Description:
     Sets the stacked setting

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    leBool stk - the setting value

  Remarks:
    Usage - _this->fn->setStacked(_this, stk);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMinValue(const leLineGraphWidget* _this,
                         leLineGraphValueAxis axis)

  Summary:
     Gets the minimum value

  Description:
     Gets the minimum value

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getMinValue(_this, axis);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setMinValue(leLineGraphWidget* _this,
                         leLineGraphValueAxis axis,
                         int32_t min)

  Summary:
     Sets the minimum value

  Description:
     Sets the minimum value

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    int32_t min - the minimum value

  Remarks:
    Usage - _this->fn->setMinValue(_this, axis, min);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMaxValue(const leLineGraphWidget* _this,
                         leLineGraphValueAxis axis)

  Summary:
     Gets the maximum value

  Description:
     Gets the maximum value

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getMaxValue(_this, axis);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setMaxValue(leLineGraphWidget* _this,
                         leLineGraphValueAxis axis,
                         int32_t max)

  Summary:
     Sets the maximum value

  Description:
     Sets the maximum value

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    int32_t max - the maximum value

  Remarks:
    Usage - _this->fn->setMaxValue(_this, axis, max);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getValueAxisLabelsVisible(const leLineGraphWidget* _this,
                                     leLineGraphValueAxis axis)

  Summary:
     Gets the visible value axis label setting

  Description:
     Gets the visible value axis label setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisLabelsVisible(_this, axis);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisLabelsVisible(leLineGraphWidget* _this,
                                       leLineGraphValueAxis axis,
                                       leBool vis)

  Summary:
     Sets the visibility of the value axis labels

  Description:
     Sets the visibility of the value axis labels

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setValueAxisLabelsVisible(_this, axis, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getFillGraphArea(const leLineGraphWidget* _this)

  Summary:
     Gets the graph fill setting

  Description:
     Gets the graph fill setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on

  Remarks:
    Usage - _this->fn->getFillGraphArea(_this);

  Returns:
    leBool - the setting value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFillGraphArea(leLineGraphWidget* _this,
                              leBool fill)

  Summary:
     Sets the graph fill setting

  Description:
     Sets the graph fill setting

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    leBool fill - the fill setting

  Remarks:
    Usage - _this->fn->setFillGraphArea(_this, fill);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getFillSeriesArea(const leLineGraphWidget* _this)

  Summary:
     Gets the fill series area setting

  Description:
     Gets the fill series area setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on

  Remarks:
    Usage - _this->fn->getFillSeriesArea(_this);

  Returns:
    leBool - the setting value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFillSeriesArea(leLineGraphWidget* _this,
                               leBool fill)

  Summary:
     Sets the fill series area setting

  Description:
     Sets the fill series area setting

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    leBool fill - the fill setting

  Remarks:
    Usage - _this->fn->setFillSeriesArea(_this, fill);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getGridLinesVisible(const leLineGraphWidget* _this,
                               leLineGraphValueAxis axis)

  Summary:
     Gets the visible grid lines setting

  Description:
     Gets the visible grid lines setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getGridLinesVisible(_this, axis);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setGridLinesVisible(leLineGraphWidget* _this,
                                 leLineGraphValueAxis axis,
                                 leBool vis)

  Summary:
     Sets the visibility of the grid lines

  Description:
     Sets the visibility of the grid lines

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setGridLinesVisible(_this, axis, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getValueAxisTicksVisible(const leLineGraphWidget* _this,
                                    leLineGraphValueAxis axis)

  Summary:
     Gets the visible value ticks setting

  Description:
     Gets the visible value ticks setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisTicksVisible(_this, axis);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisTicksVisible(leLineGraphWidget* _this,
                                      leLineGraphValueAxis axis,
                                      leBool vis)

  Summary:
     Sets the visibility of the value axis ticks

  Description:
     Sets the visibility of the value axis ticks

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setValueAxisTicksVisible(_this, axis, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getValueAxisTicksInterval(const leLineGraphWidget* _this,
                                       leLineGraphValueAxis axis)

  Summary:
     Gets the value axis ticks interval setting

  Description:
     Gets the value axis ticks interval setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisTicksInterval(_this, axis);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisTicksInterval(leLineGraphWidget* _this,
                                       leLineGraphValueAxis axis,
                                       uint32_t itv)

  Summary:
     Sets the inverval of the value axis ticks

  Description:
     Sets the inverval of the value axis ticks

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    uint32_t itv -

  Remarks:
    Usage - _this->fn->setValueAxisTicksInterval(_this, axis, itv);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getValueAxisSubticksVisible(const leLineGraphWidget* _this,
                                       leLineGraphValueAxis axis)

  Summary:
     Gets the value axis subticks position

  Description:
     Gets the value axis subticks position

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisSubticksVisible(_this, axis);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisSubticksVisible(leLineGraphWidget* _this,
                                         leLineGraphValueAxis axis,
                                         leBool vis)

  Summary:
     Sets the value axis subticks position

  Description:
     Sets the value axis subticks position

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setValueAxisSubticksVisible(_this, axis, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getValueAxisSubticksInterval(const leLineGraphWidget* _this,
                                          leLineGraphValueAxis axis)

  Summary:
     Gets the value axis subticks interval

  Description:
     Gets the value axis subticks interval

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisSubticksInterval(_this, axis);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisSubticksInterval(leLineGraphWidget* _this,
                                          leLineGraphValueAxis axis,
                                          uint32_t itv)

  Summary:
     Sets the value axis subticks interval

  Description:
     Sets the value axis subticks interval

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    uint32_t itv - the interval value

  Remarks:
    Usage - _this->fn->setValueAxisSubticksInterval(_this, axis, itv);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getCategoryAxisTicksVisible(const leLineGraphWidget* _this)

  Summary:
     Gets the category axis ticks visible setting

  Description:
     Gets the category axis ticks visible setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on

  Remarks:
    Usage - _this->fn->getCategoryAxisTicksVisible(_this);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCategoryAxisTicksVisible(leLineGraphWidget* _this,
                                         leBool vis)

  Summary:
     Sets the category axis ticks visible setting

  Description:
     Sets the category axis ticks visible setting

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setCategoryAxisTicksVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult addCategory(leLineGraphWidget* _this,
                         uint32_t* id)

  Summary:
     Adds a data category

  Description:
     Adds a data category

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    uint32_t* id - will contain the ID of the new category

  Remarks:
    Usage - _this->fn->addCategory(_this, id);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leString* getCategoryString(const leLineGraphWidget* _this,
                                uint32_t id)

  Summary:
     Gets the string currently assigned to a category

  Description:
     Gets the string currently assigned to a category

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    uint32_t id - the category ID to modify

  Remarks:
    Usage - _this->fn->getCategoryString(_this, id);

  Returns:
    leString* - the string pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCategoryString(leLineGraphWidget* _this,
                               uint32_t id,
                               const leString* str)

  Summary:
     Assigns a string to a category

  Description:
     Assigns a string to a category

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    uint32_t id - the ID of the category to modify
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setCategoryString(_this, id, str);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t addSeries(leLineGraphWidget* _this)

  Summary:
     Adds a data series to the graph

  Description:
     Adds a data series to the graph

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on

  Remarks:
    Usage - _this->fn->addSeries(_this);

  Returns:
    int32_t - the ID of the new series
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t addDataToSeries(leLineGraphWidget* _this,
                            int32_t seriesID,
                            int32_t val)

  Summary:
     Adds data to an existing series

  Description:
     Adds data to an existing series

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID - the series ID to modify
    int32_t val - the value

  Remarks:
    Usage - _this->fn->addDataToSeries(_this, seriesID, val);

  Returns:
    int32_t - the ID of the new value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setDataInSeries(leLineGraphWidget* _this,
                             int32_t seriesID,
                             int32_t idx,
                             int32_t val)

  Summary:
     Sets data in an existing series

  Description:
     Sets data in an existing series

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID -
    int32_t idx - the index
    int32_t val - the value

  Remarks:
    Usage - _this->fn->setDataInSeries(_this, seriesID, idx, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leScheme* getSeriesScheme(const leLineGraphWidget* _this,
                              uint32_t seriesID)

  Summary:
     Gets the scheme for a data series

  Description:
     Gets the scheme for a data series

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    uint32_t seriesID - the ID of the series to modify

  Remarks:
    Usage - _this->fn->getSeriesScheme(_this, seriesID);

  Returns:
    leScheme* - the scheme pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSeriesScheme(leLineGraphWidget* _this,
                             uint32_t seriesID,
                             const leScheme* schm)

  Summary:
     Sets a data series' scheme pointer

  Description:
     Sets a data series' scheme pointer

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    uint32_t seriesID -
    const leScheme* schm - the scheme pointer

  Remarks:
    Usage - _this->fn->setSeriesScheme(_this, seriesID, schm);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getSeriesFillPoints(const leLineGraphWidget* _this,
                               int32_t seriesID)

  Summary:
     Gets the series fill point setting

  Description:
     Gets the series fill point setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID -

  Remarks:
    Usage - _this->fn->getSeriesFillPoints(_this, seriesID);

  Returns:
    leBool - the setting value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSeriesFillPoints(leLineGraphWidget* _this,
                                 int32_t seriesID,
                                 leBool fill)

  Summary:
     Sets the series fill point setting

  Description:
     Sets the series fill point setting

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID -
    leBool fill - the fill setting

  Remarks:
    Usage - _this->fn->setSeriesFillPoints(_this, seriesID, fill);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getSeriesLinesVisible(const leLineGraphWidget* _this,
                                 int32_t seriesID)

  Summary:
     Gets the series lines visible setting

  Description:
     Gets the series lines visible setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID -

  Remarks:
    Usage - _this->fn->getSeriesLinesVisible(_this, seriesID);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSeriesLinesVisible(leLineGraphWidget* _this,
                                   int32_t seriesID,
                                   leBool vis)

  Summary:
     Sets the series lines visible setting

  Description:
     Sets the series lines visible setting

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID -
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setSeriesLinesVisible(_this, seriesID, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leLineGraphDataPointType getSeriesPointType(const leLineGraphWidget* _this,
                                                int32_t seriesID)

  Summary:
     Ges the series point type setting

  Description:
     Ges the series point type setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID -

  Remarks:
    Usage - _this->fn->getSeriesPointType(_this, seriesID);

  Returns:
    leLineGraphDataPointType - the type
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSeriesPointType(leLineGraphWidget* _this,
                                int32_t seriesID,
                                leLineGraphDataPointType type)

  Summary:
     Sets the series point type setting

  Description:
     Sets the series point type setting

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID -
    leLineGraphDataPointType type - the type

  Remarks:
    Usage - _this->fn->setSeriesPointType(_this, seriesID, type);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getSeriesPointSize(const leLineGraphWidget* _this,
                                int32_t seriesID)

  Summary:
     Gets the series point size seting

  Description:
     Gets the series point size seting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID -

  Remarks:
    Usage - _this->fn->getSeriesPointSize(_this, seriesID);

  Returns:
    uint32_t - the point size value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSeriesPointSize(leLineGraphWidget* _this,
                                int32_t seriesID,
                                uint32_t size)

  Summary:
     Sets the series point size setting

  Description:
     Sets the series point size setting

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    int32_t seriesID - the series ID to modify
    uint32_t size - the size value

  Remarks:
    Usage - _this->fn->setSeriesPointSize(_this, seriesID, size);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult clear(leLineGraphWidget* _this)

  Summary:
     Clears all series data

  Description:
     Clears all series data

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on

  Remarks:
    Usage - _this->fn->clear(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTicksLabelFont(leLineGraphWidget* _this,
                               const leFont* font)

  Summary:
     Sets the font to use for rendering tick labels

  Description:
     Sets the font to use for rendering tick labels

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    const leFont* font - the font pointer

  Remarks:
    Usage - _this->fn->setTicksLabelFont(_this, font);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getCategoryAxisLabelsVisible(const leLineGraphWidget* _this)

  Summary:
     Gets the category axis labels visible setting

  Description:
     Gets the category axis labels visible setting

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on

  Remarks:
    Usage - _this->fn->getCategoryAxisLabelsVisible(_this);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCategoryAxisLabelsVisible(leLineGraphWidget* _this,
                                          leBool vis)

  Summary:
     Sets the category axis labels visible setting

  Description:
     Sets the category axis labels visible setting

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setCategoryAxisLabelsVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leLineGraphTickPosition getValueAxisTicksPosition(const leLineGraphWidget* _this,
                                                      leLineGraphValueAxis axis)

  Summary:
     Gets the value axis ticks position

  Description:
     Gets the value axis ticks position

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisTicksPosition(_this, axis);

  Returns:
    leLineGraphTickPosition - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisTicksPosition(leLineGraphWidget* _this,
                                       leLineGraphValueAxis axis,
                                       leLineGraphTickPosition pos)

  Summary:
     Sets the value axis ticks position

  Description:
     Sets the value axis ticks position

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    leLineGraphTickPosition pos -

  Remarks:
    Usage - _this->fn->setValueAxisTicksPosition(_this, axis, pos);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leLineGraphTickPosition getValueAxisSubticksPosition(const leLineGraphWidget* _this,
                                                         leLineGraphValueAxis axis)

  Summary:
     Gets the value axis subticks position

  Description:
     Gets the value axis subticks position

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisSubticksPosition(_this, axis);

  Returns:
    leLineGraphTickPosition - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisSubticksPosition(leLineGraphWidget* _this,
                                          leLineGraphValueAxis axis,
                                          leLineGraphTickPosition pos)

  Summary:
     Sets the value axis subticks position

  Description:
     Sets the value axis subticks position

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    lleLineGraphValueAxis axis - the axis to modify
    leLineGraphTickPosition pos -

  Remarks:
    Usage - _this->fn->setValueAxisSubticksPosition(_this, axis, pos);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leLineGraphTickPosition getCategoryAxisTicksPosition(const leLineGraphWidget* _this)

  Summary:
     Gets the category axis ticks position

  Description:
     Gets the category axis ticks position

  Parameters:
    const leLineGraphWidget* _this - The line graph widget to operate on

  Remarks:
    Usage - _this->fn->getCategoryAxisTicksPosition(_this);

  Returns:
    leLineGraphTickPosition -
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCategoryAxisTicksPosition(leLineGraphWidget* _this,
                                          leLineGraphTickPosition pos)

  Summary:
     Sets the category axis ticks position

  Description:
     Sets the category axis ticks position

  Parameters:
    leLineGraphWidget* _this - The line graph widget to operate on
    leLineGraphTickPosition pos -

  Remarks:
    Usage - _this->fn->setCategoryAxisTicksPosition(_this, pos);

  Returns:
    leResult - the result of the operation
*/

#ifdef __cplusplus
}
#endif

#endif // LE_LINEGRAPH_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_LINE_GRAPH_H */
