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
    legato_widget_bar_graph.h

  Summary:


  Description:
    This module implements bar graph drawing widget functions.
*******************************************************************************/


#ifndef LEGATO_WIDGET_BAR_GRAPH_H
#define LEGATO_WIDGET_BAR_GRAPH_H

#include "gfx/legato/common/legato_common.h"

#if LE_BARGRAPH_WIDGET_ENABLED

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
typedef struct leBarGraphDataSeries
{
    const leScheme* scheme;
    leArray data;
    leBarGraphValueAxis axis;
} leBarGraphDataSeries;

// DOM-IGNORE-BEGIN
typedef struct leBarGraphWidget leBarGraphWidget;

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

// DOM-IGNORE-END

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

// *****************************************************************************
/* Function:
    leBarGraphWidget* leBarGraphWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the management of this memory until the
    widget is added to a widget tree.

  Description:

  Parameters:
    void

  Returns:
    leBarGraphWidget* - the allocated widget pointer

  Remarks:
    Use leWidget_Delete() to free this pointer.

*/
LIB_EXPORT leBarGraphWidget* leBarGraphWidget_New();

/* Function:
    void leBarGraphWidget_Constructor(leBarGraphWidget* wgt)

  Summary:
    Initializes a leBarGraphWidget widget pointer.

  Description:
    Initializes a leBarGraphWidget widget pointer.

  Parameters:
    leBarGraphWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leBarGraphWidget_Constructor(leBarGraphWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getTickLength(const leBarGraphWidget* _this)

  Summary:
     Gets the tick length

  Description:
     Gets the tick length

  Parameters:
    const leBarGraphWidget* _this - The string to operate on

  Remarks:
    Usage - _this->fn->getTickLength(_this);

  Returns:
    uint32_t - the tick length
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTickLength(leBarGraphWidget* _this,
                           uint32_t len)

  Summary:
     Sets the tick length

  Description:
     Sets the tick length

  Parameters:
    leBarGraphWidget* _this - The string to operate on
    uint32_t len - the tick length

  Remarks:
    Usage - _this->fn->setTickLength(_this, len);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMinValue(const leBarGraphWidget* _this,
                         leBarGraphValueAxis axis)

  Summary:
     Gets the minimum value

  Description:
     Gets the minimum value

  Parameters:
    const leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getMinValue(_this, axis);

  Returns:
    uint32_t - the minimum value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setMinValue(leBarGraphWidget* _this,
                         leBarGraphValueAxis axis,
                         int32_t min)

  Summary:
     Sets the minimum value

  Description:
     Sets the minimum value

  Parameters:
    leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify
    int32_t min - the minimum value

  Remarks:
    Usage - _this->fn->setMinValue(_this, axis, min);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMaxValue(const leBarGraphWidget* _this,
                         leBarGraphValueAxis axis)

  Summary:
     Gets the maximum value

  Description:
     Gets the maximum value

  Parameters:
    const leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getMaxValue(_this, axis);

  Returns:
    uint32_t - the maximum value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setMaxValue(leBarGraphWidget* _this,
                         leBarGraphValueAxis axis,
                         int32_t max)

  Summary:
     Sets the maximum value

  Description:
     Sets the maximum value

  Parameters:
    leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify
    int32_t max - the maximum value

  Remarks:
    Usage - _this->fn->setMaxValue(_this, axis, max);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getValueAxisLabelsVisible(const leBarGraphWidget* _this,
                                     leBarGraphValueAxis axis)

  Summary:
     Gets the visible value axis label setting

  Description:
     Gets the visible value axis label setting

  Parameters:
    const leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisLabelsVisible(_this, axis);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisLabelsVisible(leBarGraphWidget* _this,
                                       leBarGraphValueAxis axis,
                                       leBool vis)

  Summary:
     Sets the visibility of the value axis labels

  Description:
     Sets the visibility of the value axis labels

  Parameters:
    leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify
    leBool vis - the visbility setting

  Remarks:
    Usage - _this->fn->setValueAxisLabelsVisible(_this, axis, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getValueAxisTicksVisible(const leBarGraphWidget* _this,
                                    leBarGraphValueAxis axis)

  Summary:
     Gets the visible value ticks setting

  Description:
     Gets the visible value ticks setting

  Parameters:
    const leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisTicksVisible(_this, axis);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisTicksVisible(leBarGraphWidget* _this,
                                      leBarGraphValueAxis axis,
                                      leBool vis)

  Summary:
     Sets the visibility of the value axis ticks

  Description:
     Sets the visibility of the value axis ticks

  Parameters:
    leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify
    leBool vis - the visbility setting

  Remarks:
    Usage - _this->fn->setValueAxisTicksVisible(_this, axis, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getValueAxisTicksInterval(const leBarGraphWidget* _this,
                                       leBarGraphValueAxis axis)

  Summary:
     Gets the value axis ticks interval setting

  Description:
     Gets the value axis ticks interval setting

  Parameters:
    const leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisTicksInterval(_this, axis);

  Returns:
    uint32_t - the interval value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisTicksInterval(leBarGraphWidget* _this,
                                       leBarGraphValueAxis axis,
                                       uint32_t itv)

  Summary:
     Sets the inverval of the value axis ticks

  Description:
     Sets the inverval of the value axis ticks

  Parameters:
    leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify
    uint32_t itv - the interval value

  Remarks:
    Usage - _this->fn->setValueAxisTicksInterval(_this, axis, itv);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBarGraphTickPosition getValueAxisTicksPosition(const leBarGraphWidget* _this,
                                                     leBarGraphValueAxis axis)

  Summary:
     Gets the value axis ticks position

  Description:
     Gets the value axis ticks position

  Parameters:
    const leBarGraphWidget* _this - The string to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisTicksPosition(_this, axis);

  Returns:
    leBarGraphTickPosition - the position value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisTicksPosition(leBarGraphWidget* _this,
                                       leBarGraphValueAxis axis,
                                       leBarGraphTickPosition pos)

  Summary:
     Sets the value axis ticks position

  Description:
     Sets the value axis ticks position

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBarGraphValueAxis axis - the axis to modify
    leBarGraphTickPosition pos - the position value

  Remarks:
    Usage - _this->fn->setValueAxisTicksPosition(_this, axis, pos);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getValueAxisSubticksVisible(const leBarGraphWidget* _this,
                                       leBarGraphValueAxis axis)

  Summary:
     Gets the value axis subticks position

  Description:
     Gets the value axis subticks position

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisSubticksVisible(_this, axis);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisSubticksVisible(leBarGraphWidget* _this,
                                         leBarGraphValueAxis axis,
                                         leBool vis)

  Summary:
     Sets the value axis subticks position

  Description:
     Sets the value axis subticks position

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBarGraphValueAxis axis - the axis to modify
    leBool vis - the visbility setting

  Remarks:
    Usage - _this->fn->setValueAxisSubticksVisible(_this, axis, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getValueAxisSubticksInterval(const leBarGraphWidget* _this,
                                          leBarGraphValueAxis axis)

  Summary:
     Gets the value axis subticks interval

  Description:
     Gets the value axis subticks interval

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisSubticksInterval(_this, axis);

  Returns:
    uint32_t - the interval value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisSubticksInterval(leBarGraphWidget* _this,
                                          leBarGraphValueAxis axis,
                                          uint32_t itv)

  Summary:
     Sets the value axis subticks interval

  Description:
     Sets the value axis subticks interval

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBarGraphValueAxis axis - the axis to modify
    uint32_t itv - the interval value

  Remarks:
    Usage - _this->fn->setValueAxisSubticksInterval(_this, axis, itv);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBarGraphTickPosition getValueAxisSubticksPosition(const leBarGraphWidget* _this,
                                                        leBarGraphValueAxis axis)

  Summary:
     Gets the value axis subticks position

  Description:
     Gets the value axis subticks position

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getValueAxisSubticksPosition(_this, axis);

  Returns:
    leBarGraphTickPosition - the position value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueAxisSubticksPosition(leBarGraphWidget* _this,
                                          leBarGraphValueAxis axis,
                                          leBarGraphTickPosition pos)

  Summary:
     Sets the value axis subticks position

  Description:
     Sets the value axis subticks position

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBarGraphValueAxis axis - the axis to modify
    leBarGraphTickPosition pos - the position value

  Remarks:
    Usage - _this->fn->setValueAxisSubticksPosition(_this, axis, pos);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getCategoryAxisTicksVisible(const leBarGraphWidget* _this)

  Summary:
     Gets the category axis ticks visible setting

  Description:
     Gets the category axis ticks visible setting

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on

  Remarks:
    Usage - _this->fn->getCategoryAxisTicksVisible(_this);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCategoryAxisTicksVisible(leBarGraphWidget* _this,
                                         leBool vis)

  Summary:
     Sets the category axis ticks visible setting

  Description:
     Sets the category axis ticks visible setting

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBool vis - the visbility setting

  Remarks:
    Usage - _this->fn->setCategoryAxisTicksVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBarGraphTickPosition getCategoryAxisTicksPosition(const leBarGraphWidget* _this)

  Summary:
     Gets the category axis ticks position

  Description:
     Gets the category axis ticks position

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on

  Remarks:
    Usage - _this->fn->getCategoryAxisTicksPosition(_this);

  Returns:
    leBarGraphTickPosition - the position value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCategoryAxisTicksPosition(leBarGraphWidget* _this,
                                          leBarGraphTickPosition pos)

  Summary:
     Sets the category axis ticks position

  Description:
     Sets the category axis ticks position

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBarGraphTickPosition pos - the position value

  Remarks:
    Usage - _this->fn->setCategoryAxisTicksPosition(_this, pos);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getCategoryAxisLabelsVisible(const leBarGraphWidget* _this)

  Summary:
     Gets the category axis labels visible setting

  Description:
     Gets the category axis labels visible setting

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on

  Remarks:
    Usage - _this->fn->getCategoryAxisLabelsVisible(_this);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCategoryAxisLabelsVisible(leBarGraphWidget* _this,
                                          leBool vis)

  Summary:
     Sets the category axis labels visible setting

  Description:
     Sets the category axis labels visible setting

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBool vis - the visbility setting

  Remarks:
    Usage - _this->fn->setCategoryAxisLabelsVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult addSeries(leBarGraphWidget* _this,
                       uint32_t* seriesID)

  Summary:
     Adds a data series to the graph

  Description:
     Adds a data series to the graph

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    uint32_t* seriesID - the ID of the new data series

  Remarks:
    Usage - _this->fn->addSeries(_this, seriesID);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leScheme* getSeriesScheme(const leBarGraphWidget* _this,
                              uint32_t seriesID)

  Summary:
     Gets the scheme for a data series

  Description:
     Gets the scheme for a data series

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on
    uint32_t seriesID - the series ID to modify

  Remarks:
    Usage - _this->fn->getSeriesScheme(_this, seriesID);

  Returns:
    leScheme* - the scheme in use
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSeriesScheme(leBarGraphWidget* _this,
                             uint32_t seriesID,
                             const leScheme* schm)

  Summary:
     Sets a data series' scheme pointer

  Description:
     Sets a data series' scheme pointer

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    uint32_t seriesID - the series ID to modify
    const leScheme* schm - the scheme to use

  Remarks:
    Usage - _this->fn->setSeriesScheme(_this, seriesID, schm);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult addDataToSeries(leBarGraphWidget* _this,
                             uint32_t seriesID,
                             int32_t val,
                             uint32_t* idx)

  Summary:
     Adds data to an existing series

  Description:
     Adds data to an existing series

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    uint32_t seriesID - the series index to modify
    uint32_t val - value to add
    uint32_t* idx - the index of the new data value

  Remarks:
    Usage - _this->fn->addDataToSeries(_this, seriesID, val, idx);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setDataInSeries(leBarGraphWidget* _this,
                             uint32_t seriesID,
                             uint32_t index,
                             int32_t value)

  Summary:
     Sets data in an existing series

  Description:
     Sets data in an existing series

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    uint32_t seriesID - the series index to modify
    uint32_t index - the index value to modify
    int32_t value - the value to set

  Remarks:
    Usage - _this->fn->setDataInSeries(_this, seriesID, index, value);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult clearData(leBarGraphWidget* _this)

  Summary:
     Clears all series data from the table

  Description:
     Clears all series data from the table

  Parameters:
    leBarGraphWidget* _this - the widget to operate on

  Remarks:
    Usage - _this->fn->clearData(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leFont* getTicksLabelFont(leBarGraphWidget* _this)

  Summary:
     Gets the font to use for rendering tick labels

  Description:
     Gets the font to use for rendering tick labels

  Parameters:
    leBarGraphWidget* _this - the widget to operate on

  Remarks:
    Usage - _this->fn->getTicksLabelFont(_this);

  Returns:
    leFont* - the font in use
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTicksLabelFont(leBarGraphWidget* _this,
                               const leFont* font)

  Summary:
     Sets the font to use for rendering tick labels

  Description:
     Sets the font to use for rendering tick labels

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    const leFont* font - the font to use

  Remarks:
    Usage - _this->fn->setTicksLabelFont(_this, font);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getGridLinesVisible(const leBarGraphWidget* _this,
                               leBarGraphValueAxis axis)

  Summary:
     Gets the visible grid lines setting

  Description:
     Gets the visible grid lines setting

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on
    leBarGraphValueAxis axis - the axis to modify

  Remarks:
    Usage - _this->fn->getGridLinesVisible(_this, axis);

  Returns:
    leBool - the visibility setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setGridLinesVisible(leBarGraphWidget* _this,
                                 leBarGraphValueAxis axis,
                                 leBool vis)

  Summary:
     Sets the visibility of the grid lines

  Description:
     Sets the visibility of the grid lines

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBarGraphValueAxis axis - the axis to modify
    leBool vis - the visbility setting

  Remarks:
    Usage - _this->fn->setGridLinesVisible(_this, axis, vis);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult addCategory(leBarGraphWidget* _this,
                         uint32_t* id)

  Summary:
     Adds a data category

  Description:
     Adds a data category

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    uint32_t* id - will contain the ID of the newly added category

  Remarks:
    Usage - _this->fn->addCategory(_this, id);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leString* getCategoryString(const leBarGraphWidget* _this,
                                uint32_t id)

  Summary:
     Gets the string currently assigned to a category

  Description:
     Gets the string currently assigned to a category

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on
    uint32_t id - the ID of the category to query

  Remarks:
    Usage - _this->fn->getCategoryString(_this, id);

  Returns:
    leString* - the string in use
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCategoryString(leBarGraphWidget* _this,
                               uint32_t id,
                               const leString* str)

  Summary:
     Assigns a string to a category

  Description:
     Assigns a string to a category

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    uint32_t id - the ID of the category to modify
    const leString* str - the string to use

  Remarks:
    Usage - _this->fn->setCategoryString(_this, id, str);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getStacked(const leBarGraphWidget* _this)

  Summary:
     Gets the stacked setting

  Description:
     Gets the stacked setting

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on

  Remarks:
    Usage - _this->fn->getStacked(_this);

  Returns:
    leBool - the stacked setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStacked(leBarGraphWidget* _this,
                        leBool stk)

  Summary:
     Sets the stacked setting

  Description:
     Sets the stacked setting

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBool stk - the stacked setting

  Remarks:
    Usage - _this->fn->setStacked(_this, stk);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getFillGraphArea(const leBarGraphWidget* _this)

  Summary:
     Gets the graph fill setting

  Description:
     Gets the graph fill setting

  Parameters:
    const leBarGraphWidget* _this - the widget to operate on

  Remarks:
    Usage - _this->fn->getFillGraphArea(_this);

  Returns:
    leBool - the fill setting
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFillGraphArea(leBarGraphWidget* _this,
                              leBool fill)

  Summary:
     Sets the graph fill setting

  Description:
     Sets the graph fill setting

  Parameters:
    leBarGraphWidget* _this - the widget to operate on
    leBool fill - the fill setting

  Remarks:
    Usage - _this->fn->setFillGraphArea(_this, fill);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_BARGRAPH_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_BAR_GRAPH_H */
