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


#include <gfx/legato/legato.h>
#include "gfx/legato/widget/linegraph/legato_widget_line_graph.h"

#if LE_LINEGRAPH_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"


#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif


#define DEFAULT_WIDTH              101
#define DEFAULT_HEIGHT             101

#define DEFAULT_TICK_LENGTH        5
#define DEFAULT_TICK_INTERVAL      10
#define DEFAULT_SUBTICK_INTERVAL   5
#define DEFAULT_MAX_VALUE          100
#define DEFAULT_MIN_VALUE          0

#define DEFAULT_POINT_SIZE         10
 
void _leLineGraphWidget_GetGraphRect(const leLineGraphWidget* graph, leRect* graphRect);

void _leLineGraphWidget_GetOriginPoint(const leLineGraphWidget* _this, lePoint* pnt);

static void getValuePoint(const leLineGraphWidget* _this,
                          uint32_t seriesID,
                          uint32_t categoryIndex,
                          lePoint originPoint,
                          lePoint* pnt);

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leLineGraphWidgetVTable lineGraphWidgetVTable;

static void stringPreinvalidate(const leString* str,
                                leLineGraphWidget* gph)
{
    (void)str; // unused

    gph->fn->invalidate(gph);
}

static void stringInvalidate(const leString* str,
                             leLineGraphWidget* gph)
{
    (void)str; // unused

    gph->fn->invalidate(gph);
}

void leLineGraphWidget_Constructor(leLineGraphWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&lineGraphWidgetVTable;
    _this->fn = &lineGraphWidgetVTable;

    _this->widget.type = LE_WIDGET_LINE_GRAPH;
    
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_NONE;
    
    _this->tickLength = DEFAULT_TICK_LENGTH;
    _this->fillGraphArea = LE_TRUE;
    _this->fillValueArea = LE_FALSE;
    _this->stacked = LE_FALSE;
    
    _this->maxValue = DEFAULT_MAX_VALUE;
    _this->minValue = DEFAULT_MIN_VALUE;
    _this->tickInterval = DEFAULT_TICK_INTERVAL;
    _this->subtickInterval = DEFAULT_SUBTICK_INTERVAL;
    
    _this->valueGridLinesVisible = LE_TRUE;
    _this->valueAxisLabelsVisible = LE_TRUE;
    _this->valueAxisTicksVisible = LE_TRUE;
    _this->valueAxisSubticksVisible = LE_TRUE;
    _this->valueAxisTicksPosition = LINE_GRAPH_TICK_CENTER;
    _this->valueAxisSubticksPosition = LINE_GRAPH_TICK_CENTER;
    
    _this->categAxisLabelsVisible = LE_TRUE;
    _this->categAxisTicksVisible = LE_TRUE;
    _this->categAxisTicksPosition = LINE_GRAPH_TICK_CENTER;

    _this->ticksLabelFont = NULL;

    leArray_Create(&_this->dataSeries);
    leArray_Create(&_this->categories);
}

void _leWidget_Destructor(leWidget* _this);

static void destructor(leLineGraphWidget* _this)
{
    _this->fn->clear(_this);
    
    _leWidget_Destructor((leWidget*)_this);
}

leLineGraphWidget* leLineGraphWidget_New()
{
    leLineGraphWidget* graph = NULL;

    graph = LE_MALLOC(sizeof(leLineGraphWidget));

    if(graph == NULL)
        return NULL;
    
    leLineGraphWidget_Constructor(graph);

    return graph;
}

static void getValueDamagedRect(const leLineGraphWidget* graph,
                                uint32_t seriesID,
                                uint32_t categoryID,
                                leRect* res)
{
    
    leRect damagedRect;
    leRect graphRect;
    
    _leLineGraphWidget_GetGraphRect(graph, &graphRect);
    
    if(graph->stacked == LE_TRUE)
    {
        leRect widgetRect;
    
        graph->fn->rectToScreen(graph, &widgetRect);
        
        damagedRect.height = widgetRect.height;
        damagedRect.y = widgetRect.y;
    
        if(graph->fillValueArea == LE_TRUE && graph->categories.size > 1)
        {
            damagedRect.x = graphRect.x + categoryID * graphRect.width / (graph->categories.size - 1);
            damagedRect.x -= graphRect.width / (graph->categories.size - 1);
            damagedRect.width = 2 * graphRect.width / (graph->categories.size - 1);
        }
        else
        {
            damagedRect.x = graphRect.x + (categoryID + 1) * graphRect.width / (graph->categories.size + 1);
            damagedRect.x -= graphRect.width / (graph->categories.size + 1);
            damagedRect.width = 2 * graphRect.width / (graph->categories.size + 1);
        }
    }
    else
    {
        //Get the rectangle for the point and adjacent point/s and combine them
        lePoint valuePoint;
        lePoint originPoint;
        leRect valueRect;
        leLineGraphDataSeries* series;
        
        _leLineGraphWidget_GetOriginPoint(graph, &originPoint);
        
        getValuePoint(graph, seriesID, categoryID, originPoint, &valuePoint);
        
        series = leArray_Get(&graph->dataSeries, seriesID);
        
        valueRect.x = valuePoint.x - (series->pointSize/2 + 2);
        valueRect.y = valuePoint.y - (series->pointSize/2 + 2);
        valueRect.height = valueRect.width = series->pointSize + 4;
        
        damagedRect = valueRect;
        
        if(series->drawLines == LE_TRUE || graph->fillValueArea == LE_TRUE)
        {
            //Get point from previous category
            if(categoryID > 0)
            {
                getValuePoint(graph, seriesID, categoryID - 1, originPoint, &valuePoint);

                valueRect.x = valuePoint.x - (series->pointSize/2 + 2);
                valueRect.y = valuePoint.y - (series->pointSize/2 + 2);
                valueRect.height = valueRect.width = series->pointSize + 4;
                    
                leRectCombine(&damagedRect, &valueRect, &damagedRect);
            }
            
            //Get point from next category
            if(categoryID + 1 < graph->categories.size)
            {
                getValuePoint(graph, seriesID, categoryID + 1, originPoint, &valuePoint);

                valueRect.x = valuePoint.x - series->pointSize/2 + 2;
                valueRect.y = valuePoint.y - series->pointSize/2 + 2;
                valueRect.height = valueRect.width = series->pointSize + 4;

                leRectCombine(&damagedRect, &valueRect, &damagedRect);
            }
        }
    }

    *res = damagedRect;
}

static uint32_t getTickLength(const leLineGraphWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->tickLength;
}

static leResult setTickLength(leLineGraphWidget* _this,
                              uint32_t length)
{
    LE_ASSERT_THIS();
        
    if(_this->tickLength == length)
        return LE_SUCCESS;
        
    _this->tickLength = length;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getStacked(const leLineGraphWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->stacked;
}

static leResult setStacked(leLineGraphWidget* _this,
                           leBool stacked)
{
    LE_ASSERT_THIS();
        
    if(_this->stacked == stacked)
        return LE_SUCCESS;
        
    _this->stacked = stacked;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getMaxValue(const leLineGraphWidget* _this,
                           leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->maxValue;
}

static leResult setMaxValue(leLineGraphWidget* _this, 
                            leLineGraphValueAxis axis,
                            int32_t value)
{
    LE_ASSERT_THIS();

    (void)axis; // unused

    if(value <= _this->minValue)
        return LE_FAILURE;
    
    if(_this->maxValue == value)
        return LE_SUCCESS;
        
    _this->maxValue = value;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getMinValue(const leLineGraphWidget* _this,
                           leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->minValue;
}

static leResult setMinValue(leLineGraphWidget* _this,
                            leLineGraphValueAxis axis,
                            int32_t value)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
    
    if(value >= _this->maxValue)
        return LE_FAILURE;

    if(_this->minValue == value)
        return LE_SUCCESS;
        
    _this->minValue = value;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getValueAxisLabelsVisible(const leLineGraphWidget* _this,
                                        leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->valueAxisLabelsVisible;
}

static leResult setValueAxisLabelsVisible(leLineGraphWidget* _this,
                                          leLineGraphValueAxis axis,
                                          leBool visible)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    if(_this->valueAxisLabelsVisible == visible)
        return LE_SUCCESS;
        
    _this->valueAxisLabelsVisible = visible;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getFillGraphArea(const leLineGraphWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->fillGraphArea;
}

static leResult setFillGraphArea(leLineGraphWidget* _this,
                                 leBool fill)
{
    LE_ASSERT_THIS();
        
    if(_this->fillGraphArea == fill)
        return LE_SUCCESS;
        
    _this->fillGraphArea = fill;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getFillSeriesArea(const leLineGraphWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->fillValueArea;
}

static leResult setFillSeriesArea(leLineGraphWidget* _this,
                                  leBool fill)
{
    LE_ASSERT_THIS();
        
    if(_this->fillValueArea == fill)
        return LE_SUCCESS;
        
    _this->fillValueArea = fill;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getGridLinesVisible(const leLineGraphWidget* _this,
                                  leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->valueGridLinesVisible;
}

static leResult setGridLinesVisible(leLineGraphWidget* _this,
                                    leLineGraphValueAxis axis,
                                    leBool visible)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    if(_this->valueGridLinesVisible == visible)
        return LE_SUCCESS;
        
    _this->valueGridLinesVisible = visible;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getValueAxisTicksVisible(const leLineGraphWidget* _this,
                                       leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->valueAxisTicksVisible;
}

static leResult setValueAxisTicksVisible(leLineGraphWidget* _this,
                                         leLineGraphValueAxis axis,
                                         leBool visible)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    if(_this->valueAxisTicksVisible == visible)
        return LE_SUCCESS;
        
    _this->valueAxisTicksVisible = visible;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getValueAxisTickInterval(const leLineGraphWidget* _this,
                                         leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->tickInterval;
}

static leResult setValueAxisTickInterval(leLineGraphWidget* _this,
                                         leLineGraphValueAxis axis,
                                         uint32_t interval)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    if(_this->tickInterval == interval)
        return LE_SUCCESS;
        
    _this->tickInterval = interval;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getValueAxisSubtickInterval(const leLineGraphWidget* _this,
                                            leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->subtickInterval;
}

static leResult setValueAxisSubtickInterval(leLineGraphWidget* _this,
                                            leLineGraphValueAxis axis,
                                            uint32_t interval)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    if(_this->subtickInterval == interval)
        return LE_SUCCESS;
        
    _this->subtickInterval = interval;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getValueAxisSubticksVisible(const leLineGraphWidget* _this,
                                          leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->valueAxisSubticksVisible;
}

static leResult setValueAxisSubticksVisible(leLineGraphWidget* _this,
                                            leLineGraphValueAxis axis,
                                            leBool visible)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    if(_this->valueAxisSubticksVisible == visible)
        return LE_SUCCESS;
        
    _this->valueAxisSubticksVisible = visible;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getCategoryAxisTicksVisible(const leLineGraphWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->categAxisTicksVisible;
}

static leResult setCategoryAxisTicksVisible(leLineGraphWidget* _this,
                                            leBool visible)
{
    LE_ASSERT_THIS();
        
    if(_this->categAxisTicksVisible == visible)
        return LE_SUCCESS;
        
    _this->categAxisTicksVisible = visible;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t addCategory(leLineGraphWidget* _this)
{
    leLineGraphCategory* category;
    int32_t id;
    
    LE_ASSERT_THIS();
    
    category = LE_MALLOC(sizeof(leLineGraphCategory));
    
    if(category == NULL)
        return -1;
    
    category->stackValue = 0;
    category->text = NULL;

    leArray_PushBack(&_this->categories, category);

    id = _this->categories.size - 1;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return id;
}

static leString* getCategoryString(const leLineGraphWidget* _this,
                                 int32_t categoryID)
{
    leLineGraphCategory* category;
    
    LE_ASSERT_THIS();
    
    if(categoryID >= (int32_t)_this->categories.size)
        return NULL;
    
    category = leArray_Get(&_this->categories, categoryID);
    
    if(category == NULL)
        return NULL;
    
    return (leString*)category->text;
}

static leResult setCategoryString(leLineGraphWidget* _this,
                                int32_t categoryID,
                                const leString* str)
{
    leLineGraphCategory* category;
    
    LE_ASSERT_THIS();
    
    if(categoryID >= (int32_t) _this->categories.size)
        return LE_FAILURE;
    
    if(categoryID < 0)
        categoryID = _this->categories.size - 1;
    
    category = leArray_Get(&_this->categories, categoryID);
    
    if(category == NULL)
        return LE_FAILURE;

    if(category->text != NULL)
    {
        category->text->fn->setPreInvalidateCallback((leString*)category->text,
                                                    NULL,
                                                    NULL);

        category->text->fn->setInvalidateCallback((leString*)category->text,
                                                  NULL,
                                                  NULL);
    }

    category->text = str;

    category->text->fn->setPreInvalidateCallback((leString*)category->text,
                                                 (leString_InvalidateCallback)stringPreinvalidate,
                                                 _this);

    category->text->fn->setInvalidateCallback((leString*)category->text,
                                              (leString_InvalidateCallback)stringInvalidate,
                                              _this);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t addSeries(leLineGraphWidget* _this)
{
    leLineGraphDataSeries* series;
    int32_t id;
    
    LE_ASSERT_THIS();
    
    series = LE_MALLOC(sizeof(leLineGraphDataSeries));
    
    if(series == NULL)
        return -1;

    leArray_Create(&series->data);
    
    series->axis = LINE_GRAPH_AXIS_0;
    series->scheme = _this->widget.scheme;
    series->drawLines = LE_TRUE;
    series->pointType = LINE_GRAPH_DATA_POINT_CIRCLE;
    series->fillPoints = LE_TRUE;
    series->pointSize = DEFAULT_POINT_SIZE;

    leArray_PushBack(&_this->dataSeries, series);

    id = _this->dataSeries.size - 1;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return id;
}

static int32_t addDataToSeries(leLineGraphWidget* _this,
                               int32_t seriesID,
                               int32_t value)
{
    leLineGraphDataSeries* series;
    //int32_t* data;
    int32_t idx;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return -1;
    
    //data = LE_MALLOC(sizeof(data));
    
    //*data = value;
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    leArray_PushBack(&series->data, (void*)(size_t)value);

    idx = series->data.size - 1;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return idx;
}

leRect prevDamagedRect, damagedRect;

static leResult setDataInSeries(leLineGraphWidget* _this, 
                                int32_t seriesID,
                                int32_t index,
                                int32_t value)
{

    leLineGraphDataSeries* series;

    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return LE_FAILURE;
    
    series = leArray_Get(&_this->dataSeries, seriesID);

    if(index >= (int32_t) series->data.size)
        return LE_FAILURE;
    
    getValueDamagedRect(_this,
                        seriesID,
                        index,
                        &prevDamagedRect);

    series->data.values[index] = (void*)(size_t)value;

    getValueDamagedRect(_this,
                        seriesID,
                        index,
                        &damagedRect);
    
    leRectCombine(&prevDamagedRect, &damagedRect, &damagedRect);

    _this->fn->_damageArea(_this, &damagedRect);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leScheme* getSeriesScheme(const leLineGraphWidget* _this,
                                 int32_t seriesID)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return NULL;
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return NULL;
    
    return (leScheme*)series->scheme;
}

static leResult setSeriesScheme(leLineGraphWidget* _this,
                                int32_t seriesID,
                                const leScheme* scheme)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(scheme == NULL)
        return LE_FAILURE;
    
    if(seriesID >= (int32_t)_this->dataSeries.size)
        return LE_FAILURE;
    
    if(seriesID < 0)
    {
        seriesID = _this->dataSeries.size - 1;
    }
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return LE_FAILURE;
    
    series->scheme = scheme;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getSeriesFillPoints(const leLineGraphWidget* _this,
                                  int32_t seriesID)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return LE_FALSE;
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return LE_FALSE;
    
    return series->fillPoints;
}

static leResult setSeriesFillPoints(leLineGraphWidget* _this,
                                    int32_t seriesID,
                                    leBool fill)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return LE_FAILURE;
    
    if(seriesID < 0)
    {
        seriesID = _this->dataSeries.size - 1;
    }
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return LE_FAILURE;
    
    series->fillPoints = fill;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getSeriesLinesVisible(const leLineGraphWidget* _this,
                                    int32_t seriesID)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return LE_FALSE;
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return LE_FALSE;
    
    return series->drawLines;
}

static leResult setSeriesLinesVisible(leLineGraphWidget* _this,
                                      int32_t seriesID,
                                      leBool visible)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return LE_FAILURE;
    
    if(seriesID < 0)
    {
        seriesID = _this->dataSeries.size - 1;
    }
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return LE_FAILURE;
    
    series->drawLines = visible;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leLineGraphDataPointType getSeriesPointType(const leLineGraphWidget* _this,
                                                   int32_t seriesID)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return LINE_GRAPH_DATA_POINT_NONE;
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return LINE_GRAPH_DATA_POINT_NONE;
    
    return series->pointType;
}

static leResult setSeriesPointType(leLineGraphWidget* _this,
                                   int32_t seriesID,
                                   leLineGraphDataPointType type)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return LE_FAILURE;
    
    if(seriesID < 0)
    {
        seriesID = _this->dataSeries.size - 1;
    }
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return LE_FAILURE;
    
    series->pointType = type;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getSeriesPointSize(const leLineGraphWidget* _this,
                                   int32_t seriesID)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return 0;
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return 0;
    
    return series->pointSize;
}

static leResult setSeriesPointSize(leLineGraphWidget* _this,
                                   int32_t seriesID,
                                   uint32_t size)
{
    leLineGraphDataSeries* series;
    
    LE_ASSERT_THIS();
    
    if(seriesID >= (int32_t) _this->dataSeries.size)
        return LE_FAILURE;
    
    if(seriesID < 0)
    {
        seriesID = _this->dataSeries.size - 1;
    }
    
    series = leArray_Get(&_this->dataSeries, seriesID);
    
    if(series == NULL)
        return LE_FAILURE;
    
    series->pointSize = size;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult clear(leLineGraphWidget* _this)
{
    leLineGraphCategory* category;
    leLineGraphDataSeries* series;
    uint32_t i;
    
    LE_ASSERT_THIS();
    
    // free category string data
    for (i = 0; i < _this->categories.size; i++)
    {
        category = leArray_Get(&_this->categories, i);
        
        LE_FREE(category);
    }

    // free the category array and contents
    leArray_Clear(&_this->categories);
    
    // free category string data
    for (i = 0; i < _this->dataSeries.size; i++)
    {
        series = leArray_Get(&_this->dataSeries, i);
        
        if(series != NULL)
        {
            // destroy the data list and containers
            leArray_Clear(&series->data);
        
            // destroy the pointer list
            //leArray_Destroy(&series->data);
        }
        
        LE_FREE(series);
    }
    
    leArray_Clear(&_this->dataSeries);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult setTicksLabelFont(leLineGraphWidget* _this, 
                                  const leFont* font)
{
    LE_ASSERT_THIS();
        
    _this->ticksLabelFont = font;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getCategoryAxisLabelsVisible(const leLineGraphWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->categAxisLabelsVisible;
}

static leResult setCategoryAxisLabelsVisible(leLineGraphWidget* _this,
                                             leBool visible)
{
    LE_ASSERT_THIS();
    
    if(_this->categAxisLabelsVisible == visible)
        return LE_SUCCESS;
        
    _this->categAxisLabelsVisible = visible;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leLineGraphTickPosition getValueAxisTicksPosition(const leLineGraphWidget* _this,
                                                         leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->valueAxisTicksPosition;
}

static leResult setValueAxisTicksPosition(leLineGraphWidget* _this,
                                          leLineGraphValueAxis axis,
                                          leLineGraphTickPosition position)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    if(_this->valueAxisTicksPosition == position)
        return LE_SUCCESS;
        
    _this->valueAxisTicksPosition = position;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS; 
}

static leLineGraphTickPosition getValueAxisSubticksPosition(const leLineGraphWidget* _this,
                                                            leLineGraphValueAxis axis)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    return _this->valueAxisSubticksPosition;
}

static leResult setValueAxisSubticksPosition(leLineGraphWidget* _this,
                                             leLineGraphValueAxis axis,
                                             leLineGraphTickPosition position)
{
    LE_ASSERT_THIS();

    (void)axis; // unused
        
    if(_this->valueAxisSubticksPosition == position)
        return LE_SUCCESS;
        
    _this->valueAxisSubticksPosition = position;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;     
}

static leLineGraphTickPosition getCategoryAxisTicksPosition(const leLineGraphWidget* _this)

{
    LE_ASSERT_THIS();
        
    return _this->categAxisTicksPosition;
}

static leResult setCategoryAxisTicksPosition(leLineGraphWidget* _this,
                                             leLineGraphTickPosition position)
{
    LE_ASSERT_THIS();
        
    if(_this->categAxisTicksPosition == position)
        return LE_SUCCESS;
        
    _this->categAxisTicksPosition = position;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;      
}

void _leLineGraphWidget_GetOriginPoint(const leLineGraphWidget* _this,
                                       lePoint* pnt)
{
    lePoint originPoint = {0};
    leRect graphRect = {0};
    float pixelsPerUnit;
    
    LE_ASSERT_THIS();
    
    _leLineGraphWidget_GetGraphRect(_this, &graphRect);
    
    pixelsPerUnit = (float) graphRect.height / ((float) _this->maxValue - (float) _this->minValue);
    
    originPoint.x = graphRect.x;
    
    if(_this->minValue < 0 && _this->maxValue > 0)
    {
        originPoint.y = graphRect.y + (int32_t) ((float) _this->maxValue * pixelsPerUnit);
    }
    else if(_this->minValue >= 0)
    {
        originPoint.y = graphRect.y + graphRect.height;
    }
    else if(_this->maxValue <= 0)
    {
        originPoint.y = graphRect.y;
    }
    
    *pnt = originPoint;
}

static void getValuePoint(const leLineGraphWidget* _this,
                          uint32_t seriesID,
                          uint32_t categoryIndex,
                          lePoint originPoint,
                          lePoint* pnt)
{
    lePoint valuePoint = {0};
    leRect graphRect = {0};
    int32_t value;
    //int32_t* valuePtr = NULL;
    leLineGraphDataSeries* series;
    int32_t originValue = 0;
    int32_t stackedPosValue;
    int32_t stackedNegValue;
    float pixelsPerUnit;
    unsigned int i;
    void* val;
    
    LE_ASSERT_THIS();
    
    _leLineGraphWidget_GetGraphRect(_this, &graphRect);
    
    pixelsPerUnit = (float) graphRect.height / ((float) _this->maxValue - (float) _this->minValue);    
    
    if(_this->minValue < 0 && _this->maxValue > 0)
    {
        originValue = 0;
    }
    else if(_this->minValue >= 0)
    {
        originValue = _this->minValue;
    }
    else if(_this->maxValue <= 0)
    {
        originValue = _this->maxValue;
    }
    
    value = originValue;
    
    if(_this->stacked == LE_TRUE)
    {
        stackedPosValue = 0;
        stackedNegValue = 0;
        
        for (i = 0; i < seriesID; i++)
        {
            series = leArray_Get(&_this->dataSeries, seriesID);
            
            if(series == NULL)
                break;

            val = leArray_Get(&series->data, categoryIndex);
            value = (int32_t)(size_t)(val);
            
            //if(valuePtr != NULL)
            //{
                if(value > 0)
                {
                    stackedPosValue += value;
                    value = stackedPosValue;
                }
                else
                {
                    stackedNegValue += value;
                    value = stackedNegValue;
                }
            //}
        }
    }
    else
    {
        series = leArray_Get(&_this->dataSeries, seriesID);
        
        if(series == NULL)
        {
            *pnt = originPoint;

            return;
        }

        val = leArray_Get(&series->data, categoryIndex);
        value = (int32_t)(size_t)val;
        
        //if(valuePtr != NULL)
        //{
        //    value = *valuePtr;
        //}    
    }
    
    if(_this->fillValueArea == LE_TRUE && _this->categories.size > 1)
    {

        valuePoint.x = originPoint.x + ((categoryIndex * graphRect.width) / (_this->categories.size - 1));

    }
    else
    {
        valuePoint.x = originPoint.x + (((categoryIndex + 1) * graphRect.width) / (_this->categories.size + 1));
    }
    
    if(value >= originValue)
    {
        valuePoint.y = originPoint.y - (int32_t) ((float)(value - originValue) * pixelsPerUnit);
    }
    //draw the points below the origin value
    else if(value < originValue)
    {
        valuePoint.y = originPoint.y + (int32_t) ((float)(originValue - value) * pixelsPerUnit);
    }

    *pnt = valuePoint;
}

static void handleLanguageChangeEvent(leLineGraphWidget* _this)
{
    LE_ASSERT_THIS();

    _this->fn->invalidate(_this);
}

void _leLineGraphWidget_Paint(leLineGraphWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leLineGraphWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&lineGraphWidgetVTable);
    
    /* overrides from base class */
    lineGraphWidgetVTable._destructor = destructor;
    lineGraphWidgetVTable._paint = _leLineGraphWidget_Paint;
    lineGraphWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;

    /* member functions */
    lineGraphWidgetVTable.getTickLength = getTickLength;
    lineGraphWidgetVTable.setTickLength = setTickLength;
    lineGraphWidgetVTable.getStacked = getStacked;
    lineGraphWidgetVTable.setStacked = setStacked;
    lineGraphWidgetVTable.getMaxValue = getMaxValue;
    lineGraphWidgetVTable.setMaxValue = setMaxValue;
    lineGraphWidgetVTable.getMinValue = getMinValue;
    lineGraphWidgetVTable.setMinValue = setMinValue;
    lineGraphWidgetVTable.getValueAxisLabelsVisible = getValueAxisLabelsVisible;
    lineGraphWidgetVTable.setValueAxisLabelsVisible = setValueAxisLabelsVisible;
    lineGraphWidgetVTable.getFillGraphArea = getFillGraphArea;
    lineGraphWidgetVTable.setFillGraphArea = setFillGraphArea;
    lineGraphWidgetVTable.getFillSeriesArea = getFillSeriesArea;
    lineGraphWidgetVTable.setFillSeriesArea = setFillSeriesArea;
    lineGraphWidgetVTable.getGridLinesVisible = getGridLinesVisible;
    lineGraphWidgetVTable.setGridLinesVisible = setGridLinesVisible;
    lineGraphWidgetVTable.getValueAxisTicksVisible = getValueAxisTicksVisible;
    lineGraphWidgetVTable.setValueAxisTicksVisible = setValueAxisTicksVisible;
    lineGraphWidgetVTable.getValueAxisTickInterval = getValueAxisTickInterval;
    lineGraphWidgetVTable.setValueAxisTickInterval = setValueAxisTickInterval;
    lineGraphWidgetVTable.getValueAxisSubtickInterval = getValueAxisSubtickInterval;
    lineGraphWidgetVTable.setValueAxisSubtickInterval = setValueAxisSubtickInterval;
    lineGraphWidgetVTable.getValueAxisSubticksVisible = getValueAxisSubticksVisible;
    lineGraphWidgetVTable.setValueAxisSubticksVisible = setValueAxisSubticksVisible;
    lineGraphWidgetVTable.getCategoryAxisTicksVisible = getCategoryAxisTicksVisible;
    lineGraphWidgetVTable.setCategoryAxisTicksVisible = setCategoryAxisTicksVisible;
    lineGraphWidgetVTable.addCategory = addCategory;
    lineGraphWidgetVTable.getCategoryString = getCategoryString;
    lineGraphWidgetVTable.setCategoryString = setCategoryString;
    lineGraphWidgetVTable.addSeries = addSeries;
    lineGraphWidgetVTable.addDataToSeries = addDataToSeries;
    lineGraphWidgetVTable.setDataInSeries = setDataInSeries;
    lineGraphWidgetVTable.getSeriesScheme = getSeriesScheme;
    lineGraphWidgetVTable.setSeriesScheme = setSeriesScheme;
    lineGraphWidgetVTable.getSeriesFillPoints = getSeriesFillPoints;
    lineGraphWidgetVTable.setSeriesFillPoints = setSeriesFillPoints;
    lineGraphWidgetVTable.getSeriesLinesVisible = getSeriesLinesVisible;
    lineGraphWidgetVTable.setSeriesLinesVisible = setSeriesLinesVisible;
    lineGraphWidgetVTable.getSeriesPointType = getSeriesPointType;
    lineGraphWidgetVTable.setSeriesPointType = setSeriesPointType;
    lineGraphWidgetVTable.getSeriesPointSize = getSeriesPointSize;
    lineGraphWidgetVTable.setSeriesPointSize = setSeriesPointSize;
    lineGraphWidgetVTable.clear = clear;
    lineGraphWidgetVTable.setTicksLabelFont = setTicksLabelFont;
    lineGraphWidgetVTable.getCategoryAxisLabelsVisible = getCategoryAxisLabelsVisible;
    lineGraphWidgetVTable.setCategoryAxisLabelsVisible = setCategoryAxisLabelsVisible;
    lineGraphWidgetVTable.getValueAxisTicksPosition = getValueAxisTicksPosition;
    lineGraphWidgetVTable.setValueAxisTicksPosition = setValueAxisTicksPosition;
    lineGraphWidgetVTable.getValueAxisSubticksPosition = getValueAxisSubticksPosition;
    lineGraphWidgetVTable.setValueAxisSubticksPosition = setValueAxisSubticksPosition;
    lineGraphWidgetVTable.getCategoryAxisTicksPosition = getCategoryAxisTicksPosition;
    lineGraphWidgetVTable.setCategoryAxisTicksPosition = setCategoryAxisTicksPosition;
}

void _leLineGraphWidget_FillVTable(leLineGraphWidgetVTable* tbl)
{
    *tbl = lineGraphWidgetVTable;
}
#else
static const leLineGraphWidgetVTable lineGraphWidgetVTable =
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

    .touchDownEvent = (void*)_leWidget_TouchDownEvent,
    .touchUpEvent = (void*)_leWidget_TouchUpEvent,
    .touchMoveEvent = (void*)_leWidget_TouchMoveEvent,
    .moveEvent = (void*)_leWidget_MoveEvent,
    .resizeEvent = (void*)_leWidget_ResizeEvent,
    .focusLostEvent = (void*)_leWidget_FocusLostEvent,
    .focusGainedEvent = (void*)_leWidget_FocusGainedEvent,

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = destructor,
    ._paint = _leLineGraphWidget_Paint,
    .languageChangeEvent = handleLanguageChangeEvent,

    /* member functions */
    .getTickLength = getTickLength,
    .setTickLength = setTickLength,
    .getStacked = getStacked,
    .setStacked = setStacked,
    .getMaxValue = getMaxValue,
    .setMaxValue = setMaxValue,
    .getMinValue = getMinValue,
    .setMinValue = setMinValue,
    .getValueAxisLabelsVisible = getValueAxisLabelsVisible,
    .setValueAxisLabelsVisible = setValueAxisLabelsVisible,
    .getFillGraphArea = getFillGraphArea,
    .setFillGraphArea = setFillGraphArea,
    .getFillSeriesArea = getFillSeriesArea,
    .setFillSeriesArea = setFillSeriesArea,
    .getGridLinesVisible = getGridLinesVisible,
    .setGridLinesVisible = setGridLinesVisible,
    .getValueAxisTicksVisible = getValueAxisTicksVisible,
    .setValueAxisTicksVisible = setValueAxisTicksVisible,
    .getValueAxisTickInterval = getValueAxisTickInterval,
    .setValueAxisTickInterval = setValueAxisTickInterval,
    .getValueAxisSubtickInterval = getValueAxisSubtickInterval,
    .setValueAxisSubtickInterval = setValueAxisSubtickInterval,
    .getValueAxisSubticksVisible = getValueAxisSubticksVisible,
    .setValueAxisSubticksVisible = setValueAxisSubticksVisible,
    .getCategoryAxisTicksVisible = getCategoryAxisTicksVisible,
    .setCategoryAxisTicksVisible = setCategoryAxisTicksVisible,
    .addCategory = addCategory,
    .getCategoryString = getCategoryString,
    .setCategoryString = setCategoryString,
    .addSeries = addSeries,
    .addDataToSeries = addDataToSeries,
    .setDataInSeries = setDataInSeries,
    .getSeriesScheme = getSeriesScheme,
    .setSeriesScheme = setSeriesScheme,
    .getSeriesFillPoints = getSeriesFillPoints,
    .setSeriesFillPoints = setSeriesFillPoints,
    .getSeriesLinesVisible = getSeriesLinesVisible,
    .setSeriesLinesVisible = setSeriesLinesVisible,
    .getSeriesPointType = getSeriesPointType,
    .setSeriesPointType = setSeriesPointType,
    .getSeriesPointSize = getSeriesPointSize,
    .setSeriesPointSize = setSeriesPointSize,
    .clear = clear,
    .setTicksLabelFont = setTicksLabelFont,
    .getCategoryAxisLabelsVisible = getCategoryAxisLabelsVisible,
    .setCategoryAxisLabelsVisible = setCategoryAxisLabelsVisible,
    .getValueAxisTicksPosition = getValueAxisTicksPosition,
    .setValueAxisTicksPosition = setValueAxisTicksPosition,
    .getValueAxisSubticksPosition = getValueAxisSubticksPosition,
    .setValueAxisSubticksPosition = setValueAxisSubticksPosition,
    .getCategoryAxisTicksPosition = getCategoryAxisTicksPosition,
    .setCategoryAxisTicksPosition = setCategoryAxisTicksPosition,
};
#endif

#endif // LE_LINEGRAPH_WIDGET_ENABLED
