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


#include "gfx/legato/widget/linegraph/legato_widget_line_graph.h"

#if LE_LINEGRAPH_WIDGET_ENABLED

#include <stdio.h>
#include <gfx/legato/legato.h>

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string_renderer.h"
#include "gfx/legato/string/legato_stringutils.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define MAX_TICK_LABEL_DIGITS 10
#define MAX_TICK_LABEL_VALUE 999999999
#define LABEL_OFFSET_MIN_PIX 5
#define TICK_LABEL_MARGIN 3

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_LINE_GRAPH,
    DRAW_STRING,
    WAIT_STRING,    
    DRAW_BORDER,
};

static struct
{
    leRect rect;
    float pixelsPerUnit;
    leColor clr;
    uint32_t alpha;
    lePoint originPoint;
    int32_t originValue;
    lePoint valuePoint;
    char strbuff[MAX_TICK_LABEL_DIGITS];
} paintState;

static void drawBackground(leLineGraphWidget* graph);
static void drawLineGraph(leLineGraphWidget* graph);
static void drawString(leLineGraphWidget* graph);
static void drawBorder(leLineGraphWidget* graph);

lePoint _leLineGraphWidget_GetOriginPoint(const leLineGraphWidget* _this);

static void nextState(leLineGraphWidget* graph)
{
    switch(graph->widget.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(graph->fn->getCumulativeAlphaEnabled(graph) == LE_TRUE)
            {
                paintState.alpha = graph->fn->getCumulativeAlphaAmount(graph);
            }
#endif
            
            if(graph->widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                graph->widget.drawState = DRAW_BACKGROUND;
                graph->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            graph->widget.drawState = DRAW_LINE_GRAPH;
            graph->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawLineGraph;
    
            return;
        }
        case DRAW_LINE_GRAPH:
        {            
            graph->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;
            graph->widget.drawState = DRAW_STRING;
            
            return;
        }
        case DRAW_STRING:
        {
            if(graph->widget.borderType != LE_WIDGET_BORDER_NONE)
            {
                graph->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                graph->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            graph->widget.drawState = DONE;
            graph->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leLineGraphWidget* graph)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)graph,
                                                paintState.alpha);
    
    nextState(graph);
}

//Gets the superset (largest) draw rectangle for the labels. 
static void getValueLabelMaxDrawRect(const leLineGraphWidget* graph,
                                     leRect * rect)
{
    leRect minLabelRect;
    leRect maxLabelRect;
    *rect = leRect_Zero;

    if(graph->valueAxisLabelsVisible == LE_FALSE ||
       graph->ticksLabelFont == NULL)
        return;
    
    //Calculate the offset for the value labels
    minLabelRect = leRect_Zero;
    maxLabelRect = leRect_Zero;

    if (graph->minValue < 0)
    {
        //Protect from overflow
        if (graph->minValue > -MAX_TICK_LABEL_VALUE)
        {
            sprintf(paintState.strbuff, "%d", (int)graph->minValue);
        }
        else
        {
            sprintf(paintState.strbuff, "---");
        }

        leStringUtils_GetRectCStr(paintState.strbuff, graph->ticksLabelFont, &minLabelRect);
    }

    //Protect from overflow
    if (graph->maxValue < MAX_TICK_LABEL_VALUE)
    {
        sprintf(paintState.strbuff, "%d", (int)graph->maxValue);
    }
    else
    {
        sprintf(paintState.strbuff, "---");
    }

    leStringUtils_GetRectCStr(paintState.strbuff, graph->ticksLabelFont, &maxLabelRect);

    rect->width = (maxLabelRect.width > minLabelRect.width) ?
                  (maxLabelRect.width) :
                  (minLabelRect.width);

    rect->height = (maxLabelRect.height > minLabelRect.height) ?
                   (maxLabelRect.height) :
                   (minLabelRect.height);
}

//Gets the maximum draw rectangle for the category labels
static void getCategoryLabelMaxDrawRect(const leLineGraphWidget* graph,
                                        leRect* rect)
{
    int32_t categoryIndex;
    leLineGraphCategory* category;
    leRect textRect;
    
    *rect = leRect_Zero;
    
    for (categoryIndex = 0; (categoryIndex < (int32_t)graph->categories.size); categoryIndex++)
    {
        category = leArray_Get(&graph->categories, categoryIndex);
        
        if (category == NULL || category->text == NULL)
            continue;
        
        category->text->fn->getRect(category->text, &textRect);
        
        if (textRect.height > rect->height)
        {
            rect->height = textRect.height;
        }
                   
        if (textRect.width > rect->width)
        {
            rect->width = textRect.width;
        }
    }
}

static void drawTickLabelWithValue(const leLineGraphWidget* graph,
                                   lePoint tickPoint,
                                   leRelativePosition position,
                                   int32_t value,
                                   leColor clr)
{
    leRect textRect;
    leCStringRenderRequest req;

    if(graph->ticksLabelFont == NULL)
        return;
        
    //Protect from overflow
    if (value < MAX_TICK_LABEL_VALUE)
    {
        sprintf(paintState.strbuff, "%d", (int)value);
    }
    else
    {
        sprintf(paintState.strbuff, "---");
    }

    // get the string rectangle
    leStringUtils_GetRectCStr(paintState.strbuff,
                              graph->ticksLabelFont,
                              &textRect);

#if 0
    if (position == LE_RELATIVE_POSITION_LEFTOF)
    {
        drawRect.x = tickPoint.x - textRect.width - LABEL_OFFSET_MIN_PIX;
        drawRect.y = tickPoint.y - textRect.height/2;
    }
    else if (position == LE_RELATIVE_POSITION_RIGHTOF)
    {
        drawRect.x = tickPoint.x + textRect.width + LABEL_OFFSET_MIN_PIX;
        drawRect.y = tickPoint.y - textRect.height/2;
    }
    else
    {
        drawRect.x = tickPoint.x;
        drawRect.y = tickPoint.y;
    }
#endif

    req.str = paintState.strbuff;
    req.font = graph->ticksLabelFont;
    req.x = textRect.x;
    req.y = textRect.y;
    req.align = LE_HALIGN_CENTER;
    req.color = clr;
    req.alpha = paintState.alpha;

    leStringRenderer_DrawCString(&req);
}

static void drawSeriesPoint(leLineGraphDataSeries* series,
                            lePoint point)
{
    leRect rect;
    uint32_t thk;
    
    switch(series->pointType)
    {
        case LINE_GRAPH_DATA_POINT_CIRCLE:
        {
            thk = series->fillPoints == LE_TRUE ? series->pointSize / 2 : 1;

            rect.width = series->pointSize * 2;
            rect.height = rect.width;
            rect.x = point.x - rect.width;
            rect.y = point.y - rect.height;

            leRenderer_ArcFill(&rect,
                               point.x - rect.x,
                               point.y - rect.y,
                               series->pointSize / 2,
                               0,
                               360,
                               thk,
                               series->scheme->foreground,
                               LE_FALSE,
                               paintState.alpha);

            break;
        }
        case LINE_GRAPH_DATA_POINT_SQUARE:
        {
            rect.x = point.x - series->pointSize / 2;
            rect.y = point.y - series->pointSize / 2;
            rect.width = series->pointSize;
            rect.height = series->pointSize;
            
            if (series->fillPoints == LE_TRUE)
            {
                leRenderer_RectFill(&rect,
                                    series->scheme->foreground,
                                    paintState.alpha);
            }
            else
            {
                leRenderer_RectLine(&rect,
                                    series->scheme->foreground,
                                    paintState.alpha);
            }

            break;
        }
        case LINE_GRAPH_DATA_POINT_NONE: //Do nothing
        default:
            break;
    }    
}

//Gets the rectangle of the graph area (without labels or ticks)
void _leLineGraphWidget_GetGraphRect(const leLineGraphWidget* graph,
                                     leRect* graphRect)
{
    lePoint p;
    leMargin margin;
    leRect widgetRect, valueLabelMaxRect, categoryLabelMaxRect;
    
    widgetRect = graph->fn->rectToScreen(graph);
    
    p.x = widgetRect.x;
    p.y = widgetRect.y;
    
    valueLabelMaxRect = leRect_Zero;
    *graphRect = leRect_Zero;
    categoryLabelMaxRect = leRect_Zero;
    
    margin = graph->widget.margin;
    graphRect->x = p.x + margin.left;
    graphRect->width = widgetRect.width - margin.left - margin.right;

    if (graph->valueAxisTicksVisible)
    {
        switch (graph->valueAxisTicksPosition)
        {
            case LINE_GRAPH_TICK_OUT:
            {
                graphRect->x += graph->tickLength;
                graphRect->width -= graph->tickLength;
                break;
            }
            case LINE_GRAPH_TICK_CENTER:
            {
                graphRect->x += graph->tickLength/2;
                graphRect->width -= graph->tickLength/2;
                break;
            }
            default:
                break;
        }
    }

    graphRect->y = p.y + margin.top;
    graphRect->height = widgetRect.height - margin.top - margin.bottom;

    if (graph->valueAxisLabelsVisible)
    {
        getValueLabelMaxDrawRect(graph, &valueLabelMaxRect);

        if(valueLabelMaxRect.y > 0)
        {
            graphRect->y += valueLabelMaxRect.height / 2 + LABEL_OFFSET_MIN_PIX;
            graphRect->height -= valueLabelMaxRect.height / 2 + LABEL_OFFSET_MIN_PIX;
        }
    }

    if (graph->categAxisTicksVisible)
    {
        switch (graph->categAxisTicksPosition)
        {
            case LINE_GRAPH_TICK_OUT:
            {
                graphRect->height -= graph->tickLength;
                break;
            }
            case LINE_GRAPH_TICK_CENTER:
            {
                graphRect->height -= graph->tickLength/2;
                break;
            }
            default:
                break;
        }
    }

    if (graph->categAxisLabelsVisible)
    {
        getCategoryLabelMaxDrawRect(graph, &categoryLabelMaxRect);
    }

    if (categoryLabelMaxRect.height > (valueLabelMaxRect.height / 2))
    {
        graphRect->height -= (categoryLabelMaxRect.height +  LABEL_OFFSET_MIN_PIX);
    }
    else if (valueLabelMaxRect.height != 0)
    {
        graphRect->height -= (valueLabelMaxRect.height / 2 +  LABEL_OFFSET_MIN_PIX);
    }

    if (graph->fillValueArea == LE_TRUE)
    {
        if (categoryLabelMaxRect.width != 0)
        {
            if((categoryLabelMaxRect.width / 2) > valueLabelMaxRect.width)
            {
                graphRect->x += (categoryLabelMaxRect.width/2 + LABEL_OFFSET_MIN_PIX * 2);
                graphRect->width -= (categoryLabelMaxRect.width/2 + LABEL_OFFSET_MIN_PIX * 2);
            }
            else
            {
                graphRect->x += (valueLabelMaxRect.width + LABEL_OFFSET_MIN_PIX * 2);
                graphRect->width -= (valueLabelMaxRect.width + LABEL_OFFSET_MIN_PIX * 2);
            }

            graphRect->width -= (categoryLabelMaxRect.width/2 + LABEL_OFFSET_MIN_PIX);
        }
    }
    else if (valueLabelMaxRect.width != 0)
    {
        graphRect->x += (valueLabelMaxRect.width + LABEL_OFFSET_MIN_PIX * 2);
        graphRect->width -= (valueLabelMaxRect.width + LABEL_OFFSET_MIN_PIX * 2);
    }
}

static void fillBase(leLineGraphWidget* graph)
{
    leRenderer_RectFill(&paintState.rect,
                        graph->widget.scheme->background,
                        paintState.alpha);
}

static void drawValueAxisTickBorders(leLineGraphWidget* graph)
{
    switch (graph->valueAxisTicksPosition)
    {
        case LINE_GRAPH_TICK_OUT:
        {
            leRenderer_HorzLine(paintState.rect.x - graph->tickLength,
                                paintState.rect.y,
                                paintState.rect.width,
                                graph->widget.scheme->foreground,
                                paintState.alpha);
                                
            leRenderer_HorzLine(paintState.rect.x - graph->tickLength,
                                paintState.rect.y + paintState.rect.height,
                                paintState.rect.width,
                                graph->widget.scheme->foreground,
                                paintState.alpha);

            break;
        }
        case LINE_GRAPH_TICK_CENTER:
        {
            leRenderer_HorzLine(paintState.rect.x - graph->tickLength / 2,
                                paintState.rect.y,
                                paintState.rect.width,
                                graph->widget.scheme->foreground,
                                paintState.alpha);
                                
            leRenderer_HorzLine(paintState.rect.x - graph->tickLength / 2,
                                paintState.rect.y + paintState.rect.height,
                                paintState.rect.width,
                                graph->widget.scheme->foreground,
                                paintState.alpha);
                                
            break;
        }
        default:
            break;
    }
}

static void drawMinMaxLabels(leLineGraphWidget* graph)
{
    lePoint drawPoint = {0};
    
    drawPoint.x = paintState.rect.x;
    
    if(graph->valueAxisTicksVisible)
    {
        switch (graph->valueAxisTicksPosition)
        {
            case LINE_GRAPH_TICK_OUT:
            {
                drawPoint.x -= graph->tickLength;
                break;
            }
            case LINE_GRAPH_TICK_CENTER:
            {
                drawPoint.x -= graph->tickLength / 2;
                break;
            }
            default:
                break;
        }
    }
    
    drawPoint.y = paintState.rect.y;
    
    drawTickLabelWithValue(graph,
                           drawPoint,
                           LE_RELATIVE_POSITION_LEFTOF,
                           graph->maxValue,
                           graph->widget.scheme->text);

    drawPoint.y = paintState.rect.y + paintState.rect.height;
    
    drawTickLabelWithValue(graph,
                           drawPoint,
                           LE_RELATIVE_POSITION_LEFTOF,
                           graph->minValue,
                           graph->widget.scheme->text);
}

static void drawTicks(leLineGraphWidget* graph)
{
    lePoint drawPoint = lePoint_Zero;
    lePoint drawEndPoint = lePoint_Zero;
    int32_t value = 0;

    if (graph->valueAxisTicksVisible)
    {
        switch (graph->valueAxisTicksPosition)
        {
            case LINE_GRAPH_TICK_OUT:
            {
                drawPoint.x = paintState.rect.x - graph->tickLength;
                break;
            }
            case LINE_GRAPH_TICK_CENTER:
            {
                drawPoint.x = paintState.rect.x - graph->tickLength / 2;
                break;
            }
            case LINE_GRAPH_TICK_IN:
            {
                drawPoint.x = paintState.rect.x;
                break;
            }
            default:
                break;
        }
        
        drawEndPoint.x = drawPoint.x + graph->tickLength;
        
    }
    else
    {
        drawPoint.x = paintState.rect.x;
        drawEndPoint = drawPoint;
    }
    
    if (graph->valueGridLinesVisible)
    {
        drawEndPoint.x = paintState.rect.x + paintState.rect.width;
    }

    //Start drawing the positive ticks or gridlines
    if (graph->maxValue > 0)
    {
        drawPoint.y = paintState.originPoint.y;
        
        for (value = paintState.originValue + graph->tickInterval; 
             (value < graph->maxValue); 
             value += graph->tickInterval)
        {
            drawPoint.y = paintState.originPoint.y - (int32_t) ((float)(value - paintState.originValue) * paintState.pixelsPerUnit);
            
            if(drawPoint.y > paintState.rect.y) 
            {
                leRenderer_HorzLine(drawPoint.x,
                                    drawPoint.y,
                                    drawEndPoint.x - drawPoint.x,
                                    graph->widget.scheme->foreground,
                                    paintState.alpha);

                if (graph->valueAxisLabelsVisible)
                {
                    drawTickLabelWithValue(graph,
                                           drawPoint,
                                           LE_RELATIVE_POSITION_LEFTOF,
                                           value,
                                           graph->widget.scheme->text);
                }
            }
        }
    }
    
    //Start drawing the negative ticks or gridlines
    if (graph->minValue < 0)
    {
        drawPoint.y = paintState.originPoint.y;

        for (value = paintState.originValue - graph->tickInterval; 
             (value > graph->minValue); 
             value -= graph->tickInterval)
        {
            drawPoint.y = paintState.originPoint.y + (int32_t) ((float) (value - paintState.originValue) * -paintState.pixelsPerUnit);
            
            if (drawPoint.y < (paintState.rect.y + paintState.rect.height))
            {
                leRenderer_HorzLine(drawPoint.x,
                                    drawPoint.y,
                                    drawEndPoint.x - drawPoint.x,
                                    graph->widget.scheme->foreground,
                                    paintState.alpha);
            
                if (graph->valueAxisLabelsVisible)
                {
                    drawTickLabelWithValue(graph,
                                           drawPoint,
                                           LE_RELATIVE_POSITION_LEFTOF,
                                           value,
                                           graph->widget.scheme->text);
                }
            }
        }
    }
}

static void fillRegion(leLineGraphWidget* graph,
                       leLineGraphCategory* category,
                       leLineGraphDataSeries* series,
                       int32_t categoryIndex,
                       lePoint prevPoint,
                       leColor clr)
{
    leLineGraphCategory* prevCategory;
    int32_t * prevOldValue;
    //Get the four points
    lePoint nowOld, nowNew; //Old and new points of current category
    lePoint prevOld, prevNew; //old and new points of previous category
    lePoint scanPointA;
    lePoint scanPointB;
    
    prevCategory = leArray_Get(&graph->categories, categoryIndex - 1);

    nowNew = paintState.valuePoint;
    nowOld = nowNew;

    prevNew = prevPoint;
    prevOld = prevNew;

    nowOld.y = paintState.originPoint.y - (int32_t) ((float) (category->stackValue  - paintState.originValue) * paintState.pixelsPerUnit);

    prevOldValue = leArray_Get(&series->data, categoryIndex - 1);
    prevOld.y = paintState.originPoint.y - (int32_t) ((float) (prevCategory->stackValue - *prevOldValue  - paintState.originValue) * paintState.pixelsPerUnit);

    //Fill the region using vertical lines for now
    for (scanPointA.x = prevNew.x; scanPointA.x < nowNew.x; scanPointA.x++)
    {
        scanPointA.y = leGetYGivenXOnLine(prevNew, nowNew, scanPointA.x);
        
        if (scanPointA.y < paintState.rect.y)
        {
            scanPointA.y = paintState.rect.y;
        }
        else if (scanPointA.y > paintState.rect.y + paintState.rect.height)
        {
            scanPointA.y = paintState.rect.y + paintState.rect.height;
        }
        
        scanPointB.x = scanPointA.x;
        scanPointB.y = leGetYGivenXOnLine(prevOld, nowOld, scanPointA.x);
        
        if (scanPointB.y < paintState.rect.y)
        {
            scanPointB.y = paintState.rect.y;
        }
        else if (scanPointB.y > paintState.rect.y + paintState.rect.height)
        {
            scanPointB.y = paintState.rect.y + paintState.rect.height;
        }
        
        leRenderer_VertLine(scanPointA.x,
                            scanPointA.y,
                            scanPointB.y - scanPointA.y,
                            clr,
                            paintState.alpha);
    }
}

static void drawStackedFills(leLineGraphWidget* graph)
{
    int32_t categoryIndex;
    int32_t seriesIndex;
    lePoint prevPoint;
    lePoint valuePoint;
    int32_t topValue;
    int32_t value = 0;
    leLineGraphDataSeries* series;
    leLineGraphCategory* category;
    
    for (categoryIndex = 0; (categoryIndex < (int32_t)graph->categories.size); categoryIndex++)
    {
        category = leArray_Get(&graph->categories, categoryIndex);
        category->stackValue = 0;
    }

    for (seriesIndex = 0; seriesIndex < (int32_t)graph->dataSeries.size; seriesIndex++)
    {
        prevPoint = paintState.originPoint;
        valuePoint = paintState.originPoint;
        topValue = 0;

        series = leArray_Get(&graph->dataSeries, seriesIndex);

        for (categoryIndex = 0; 
             (categoryIndex < (int32_t)graph->categories.size); 
             categoryIndex++)
        {
            if (categoryIndex < (int32_t)series->data.size)
            {
                value = (int32_t)leArray_Get(&series->data, categoryIndex);
            }
            
            category = leArray_Get(&graph->categories, categoryIndex);

            valuePoint.x = paintState.originPoint.x + 
                          ((categoryIndex * paintState.rect.width) / (graph->categories.size - 1));

            topValue = category->stackValue;
            topValue += value;

            valuePoint.y = paintState.originPoint.y - (int32_t) ((float) (topValue - paintState.originValue) * paintState.pixelsPerUnit);

            if (categoryIndex > 0)
            {
                fillRegion(graph,
                           category,
                           series,
                           categoryIndex,
                           prevPoint,
                           series->scheme->base);
            }

            category->stackValue = topValue;
            prevPoint = valuePoint;
        }
    }
}

static void drawUnstackedFills(leLineGraphWidget* graph)
{
    lePoint prevPoint;
    lePoint valuePoint;
    int32_t value;
    lePoint scanPoint;
    int32_t seriesIndex;
    int32_t categoryIndex;
    leLineGraphDataSeries* series;
            
    //Do the fills first, then plot the points/lines
    if (graph->fillValueArea == LE_TRUE)
    {
        for (seriesIndex = 0; seriesIndex < (int32_t)graph->dataSeries.size; seriesIndex++)
        {
            prevPoint = paintState.originPoint;
            valuePoint = paintState.originPoint;

            series = leArray_Get(&graph->dataSeries, seriesIndex);
            
            if (series == NULL)
                continue;

            for (categoryIndex = 0;
                (categoryIndex < (int32_t)graph->categories.size && categoryIndex < (int32_t)series->data.size);
                 categoryIndex++)
            {
                value = (int32_t)leArray_Get(&series->data, categoryIndex);

                if (graph->fillValueArea == LE_TRUE && (int32_t)graph->categories.size > 1)
                {
                    valuePoint.x = paintState.originPoint.x + ((categoryIndex * paintState.rect.width) / (graph->categories.size - 1));
                }
                else
                {
                    valuePoint.x = paintState.originPoint.x + (((categoryIndex + 1) * paintState.rect.width) / (graph->categories.size + 1));    
                }

                if (value >= paintState.originValue)
                {
                    valuePoint.y = paintState.originPoint.y - (int32_t) ((float)(value - paintState.originValue) * paintState.pixelsPerUnit);
                }
                //draw the points below the origin value
                else if (value < paintState.originValue)
                {
                    valuePoint.y = paintState.originPoint.y + (int32_t) ((float)(paintState.originValue - value) * paintState.pixelsPerUnit);
                }

                if (categoryIndex > 0)
                {
                    scanPoint = prevPoint;
                    
                    while(scanPoint.x <= valuePoint.x)
                    {
                        scanPoint.y = leGetYGivenXOnLine(prevPoint, valuePoint, scanPoint.x);
                        
                        if (scanPoint.y <= paintState.rect.y)
                        {
                            scanPoint.y = paintState.rect.y;
                        }
                        else if (scanPoint.y >= paintState.rect.y + paintState.rect.height)
                        {
                            scanPoint.y = paintState.rect.y + paintState.rect.height;
                        }
                        
                        leRenderer_DrawLine(scanPoint.x,
                                            scanPoint.y,
                                            scanPoint.x,
                                            paintState.originPoint.y,
                                            series->scheme->base,
                                            paintState.alpha);

                        scanPoint.x++;
                    }
                }

                prevPoint = valuePoint;
            }
        }
    }   
}

static void drawFills(leLineGraphWidget* graph)
{
    if(graph->stacked == LE_TRUE)
    {
        drawStackedFills(graph);
    }
    else 
    {
        drawUnstackedFills(graph);
    }
}

static void drawOriginLine(leLineGraphWidget* graph)
{
    lePoint drawPoint = paintState.originPoint;

    //Draw the origin line
    if(graph->valueAxisTicksVisible)
    {
        switch (graph->valueAxisTicksPosition)
        {
            case LINE_GRAPH_TICK_OUT:
            {
                drawPoint.x -= graph->tickLength;
                break;
            }
            case LINE_GRAPH_TICK_CENTER:
            {
                drawPoint.x -= graph->tickLength/2;
                break;
            }
            default:
                break;
        }

        leRenderer_DrawLine(drawPoint.x,
                            drawPoint.y,
                            paintState.rect.x + paintState.rect.width,
                            drawPoint.y,
                            graph->widget.scheme->foreground,
                            paintState.alpha);
    }
    else
    {
        leRenderer_DrawLine(drawPoint.x,
                            drawPoint.y,
                            paintState.rect.x + paintState.rect.width,
                            drawPoint.y,
                            graph->widget.scheme->foreground,
                            paintState.alpha);
    }

    if (graph->valueAxisLabelsVisible)
    {
        drawTickLabelWithValue(graph,
                               drawPoint,
                               LE_RELATIVE_POSITION_LEFTOF,
                               paintState.originValue,
                               graph->widget.scheme->text);
    }
}

static void drawCategoryTicks(leLineGraphWidget* graph)
{
    uint32_t categoryIndex = 0;
    lePoint drawPoint = paintState.originPoint;
    uint32_t length = graph->tickLength;
    
    switch (graph->categAxisTicksPosition)
    {
        case LINE_GRAPH_TICK_OUT:
        {
            drawPoint.y = paintState.originPoint.y;
            break;
        }
        case LINE_GRAPH_TICK_CENTER:
        {
            drawPoint.y = paintState.originPoint.y - graph->tickLength / 2;
            break;
        }
        case LINE_GRAPH_TICK_IN:
        {
            drawPoint.y = paintState.originPoint.y - graph->tickLength;
            break;
        }
        default:
            break;
    }
    
    if (drawPoint.y < paintState.rect.y)
    {
        length = paintState.originPoint.y - paintState.rect.y;
        
        drawPoint.y = paintState.rect.y;
    }
    
    for (categoryIndex = 0; 
         ((drawPoint.x < paintState.rect.x + paintState.rect.width) && 
         (categoryIndex < (int32_t)graph->categories.size)); 
         categoryIndex++)
    {
        if (graph->fillValueArea == LE_TRUE && graph->categories.size > 1)
        {
            drawPoint.x = paintState.rect.x + categoryIndex * paintState.rect.width / (graph->categories.size - 1);
        }
        else
        {
            drawPoint.x = paintState.rect.x + (categoryIndex + 1) * paintState.rect.width / (graph->categories.size + 1);
        }
        
        leRenderer_VertLine(drawPoint.x,
                            drawPoint.y,
                            length,
                            graph->widget.scheme->foreground,
                            paintState.alpha);
    }
}

static void drawSubTicks(leLineGraphWidget* graph)
{
    lePoint drawPoint = {0};
    uint32_t subTickLength = graph->tickLength / 2;
    int32_t value = 0;

    drawPoint.y = paintState.originPoint.y;
    
    switch(graph->valueAxisSubticksPosition)
    {
        case LINE_GRAPH_TICK_OUT:
        {
            drawPoint.x = paintState.rect.x - subTickLength;
            break;
        }
        case LINE_GRAPH_TICK_CENTER:
        {
            drawPoint.x = paintState.rect.x - subTickLength / 2;
            break;
        }
        case LINE_GRAPH_TICK_IN:
        {
            drawPoint.x = paintState.rect.x;
            break;
        }
        default:
            break;
    }

    // Draw the subticks above the origin point
    for (value = paintState.originValue + graph->subtickInterval; 
         (value < graph->maxValue); 
         value += graph->subtickInterval)
    {
        drawPoint.y = paintState.originPoint.y - (int32_t) ((float) (value - paintState.originValue) * paintState.pixelsPerUnit);

        if (drawPoint.y > paintState.rect.y)
        {
            leRenderer_HorzLine(drawPoint.x,
                                drawPoint.y,
                                subTickLength,
                                graph->widget.scheme->foreground,
                                paintState.alpha);
        }               
    }

    //Start drawing the negative ticks or gridlines
    drawPoint.y = paintState.originPoint.y;
    
    for (value = paintState.originValue - graph->subtickInterval; 
         value > graph->minValue; 
         value -= graph->subtickInterval)
    {
        drawPoint.y = paintState.originPoint.y + (int32_t) ((float) (paintState.originValue - value) * paintState.pixelsPerUnit); 
        
        if (drawPoint.y < (paintState.rect.y + paintState.rect.height))
        {
            leRenderer_HorzLine(drawPoint.x,
                                drawPoint.y,
                                subTickLength,
                                graph->widget.scheme->foreground,
                                paintState.alpha);
        }              
    }
}

static void drawBorders(leLineGraphWidget* graph)
{
    // draw the borders
    leRenderer_VertLine(paintState.rect.x,
                        paintState.rect.y,
                        paintState.rect.height,
                        graph->widget.scheme->foreground,
                        paintState.alpha);
                        
    leRenderer_VertLine(paintState.rect.x + paintState.rect.width,
                        paintState.rect.y,
                        paintState.rect.height,
                        graph->widget.scheme->foreground,
                        paintState.alpha);
    
    leRenderer_HorzLine(paintState.rect.x,
                        paintState.rect.y,
                        paintState.rect.width,
                        graph->widget.scheme->foreground,
                        paintState.alpha);
                        
    leRenderer_HorzLine(paintState.rect.x,
                        paintState.rect.y + paintState.rect.height,
                        paintState.rect.width,
                        graph->widget.scheme->foreground,
                        paintState.alpha);
}

static void drawStackedLines(leLineGraphWidget* graph)
{
    int32_t categoryIndex = 0;
    int32_t seriesIndex = 0;
    leLineGraphDataSeries* series;
    leLineGraphCategory* category;
    lePoint valuePoint;
    int32_t topValue;
    int32_t value = 0;
    lePoint start;
    lePoint end;
    leLineGraphCategory* prevCategory;
    
    for (categoryIndex = 0; 
         categoryIndex < (int32_t)graph->categories.size; 
         categoryIndex++)
    {
        category = leArray_Get(&graph->categories, categoryIndex);
        category->stackValue = 0;
    }

    for (seriesIndex = 0; seriesIndex < (int32_t)graph->dataSeries.size; seriesIndex++)
    {
        valuePoint = paintState.originPoint;
        topValue = 0;

        series = leArray_Get(&graph->dataSeries, seriesIndex);

        for (categoryIndex = 0; 
             categoryIndex < (int32_t)graph->categories.size; 
             categoryIndex++)
        {
            if (categoryIndex < (int32_t)series->data.size)
            {
                value = (int32_t)leArray_Get(&series->data, categoryIndex);
            }
            
            category = leArray_Get(&graph->categories, categoryIndex);

            if (graph->fillValueArea == LE_TRUE && graph->categories.size > 1)
            {
                valuePoint.x = paintState.originPoint.x + ((categoryIndex * paintState.rect.width) / (graph->categories.size - 1));
            }
            else
            {
                valuePoint.x = paintState.originPoint.x + (((categoryIndex + 1) * paintState.rect.width) / (graph->categories.size + 1));    
            }

            topValue = category->stackValue;

            topValue += value;

            valuePoint.y = paintState.originPoint.y - (int32_t) ((float) (topValue - paintState.originValue) * paintState.pixelsPerUnit);

            if (series->drawLines == LE_TRUE && categoryIndex > 0)
            {
                start.x = -1;
                start.y = -1;
                prevCategory = leArray_Get(&graph->categories, categoryIndex - 1);

                if (graph->fillValueArea == LE_TRUE && graph->categories.size > 1)
                {
                    start.x = paintState.originPoint.x + ((categoryIndex - 1)* paintState.rect.width) / (graph->categories.size - 1);
                }
                else
                {
                    start.x = paintState.originPoint.x + (((categoryIndex) * paintState.rect.width) / (graph->categories.size + 1));    
                }

                start.y = paintState.originPoint.y - (int32_t) ((float) (prevCategory->stackValue  - paintState.originValue) * paintState.pixelsPerUnit);
            
                end.x = paintState.valuePoint.x;
                end.y = paintState.valuePoint.y;

                if (start.y < paintState.rect.y)
                {

                    start.x = leGetXGivenYOnLine(start, paintState.valuePoint, paintState.rect.y);
                    start.y = paintState.rect.y;

                }
                else if (start.y > paintState.rect.y + paintState.rect.height)
                {
                    start.x = leGetXGivenYOnLine(start, valuePoint, paintState.rect.y + paintState.rect.height);
                    start.y = paintState.rect.y + paintState.rect.height;
                }

                if (end.y < paintState.rect.y)
                {
                    end.x = leGetXGivenYOnLine(end, start, paintState.rect.y);
                    end.y = paintState.rect.y;
                }
                else if (end.y > paintState.rect.y + paintState.rect.height)
                {
                    end.x = leGetXGivenYOnLine(end, start, paintState.rect.y + paintState.rect.height);
                    end.y = paintState.rect.y + paintState.rect.height;
                }

                if (start.x != end.x || start.y != end.y)
                {
                    leRenderer_DrawLine(start.x,
                                        start.y,
                                        end.x,
                                        end.y,
                                        series->scheme->foreground,
                                        paintState.alpha);
                }
            }

            //Draw the point if it's within graph area
            if ((series->pointType != LINE_GRAPH_DATA_POINT_NONE) &&
                (valuePoint.y >= paintState.rect.y) && 
                (valuePoint.y <= paintState.rect.y + paintState.rect.height))
            {
                drawSeriesPoint(series, valuePoint);
            }

            category->stackValue = topValue;
        }
    }
}   

static void drawUnstackedLines(leLineGraphWidget* graph)
{
    int32_t categoryIndex = 0;
    int32_t seriesIndex = 0;
    leLineGraphDataSeries * series;
    lePoint prevPoint;
    lePoint valuePoint;
    int32_t value;
    lePoint start, end;
    
    //determine the width of each category + L & R margins
    for (seriesIndex = 0; seriesIndex < (int32_t)graph->dataSeries.size; seriesIndex++)
    {
        prevPoint = paintState.originPoint;
        valuePoint = paintState.originPoint;

        series = leArray_Get(&graph->dataSeries, seriesIndex);
        
        if (series == NULL)
            continue;

        for (categoryIndex = 0; 
             (categoryIndex < (int32_t)graph->categories.size &&
             categoryIndex < (int32_t)series->data.size); 
             categoryIndex++)
        {
            value = (int32_t)leArray_Get(&series->data, categoryIndex);

            if (graph->fillValueArea == LE_TRUE && graph->categories.size > 1)
            {
                valuePoint.x = paintState.originPoint.x + ((categoryIndex * paintState.rect.width) / (graph->categories.size - 1));
            }
            else
            {
                valuePoint.x = paintState.originPoint.x + (((categoryIndex + 1) * paintState.rect.width) / (graph->categories.size + 1));    
            }
            
            if (value >= paintState.originValue)
            {
                valuePoint.y = paintState.originPoint.y - (int32_t) ((float)(value - paintState.originValue) * paintState.pixelsPerUnit);
            }
            //draw the points below the origin value
            else if (value < paintState.originValue)
            {
                valuePoint.y = paintState.originPoint.y + (int32_t) ((float)(paintState.originValue - value) * paintState.pixelsPerUnit);
            }
            
            if (series->drawLines == LE_TRUE && categoryIndex > 0)
            {
                start = prevPoint;
                end.x = valuePoint.x;
                end.y = valuePoint.y;

                if (start.y < paintState.rect.y)
                {
                    start.y = paintState.rect.y;
                    start.x = leGetXGivenYOnLine(prevPoint, valuePoint, start.y);
                }
                else if (start.y > paintState.rect.y + paintState.rect.height)
                {
                    start.y = paintState.rect.y + paintState.rect.height;
                    start.x = leGetXGivenYOnLine(prevPoint, valuePoint, start.y);
                }

                if (end.y < paintState.rect.y)
                {
                    end.y = paintState.rect.y;
                    end.x = leGetXGivenYOnLine(prevPoint, valuePoint, end.y);
                }
                else if (end.y > paintState.rect.y + paintState.rect.height)
                {
                    end.y = paintState.rect.y + paintState.rect.height;
                    end.x = leGetXGivenYOnLine(prevPoint, valuePoint, end.y);
                }

                //Draw lines
                if (start.x != end.x || start.y != end.y)
                {
                    leRenderer_DrawLine(start.x,
                                        start.y,
                                        end.x,
                                        end.y,
                                        series->scheme->foreground,
                                        paintState.alpha);
                }
            }
            
            //Draw the point if it's within graph area
            if (series->pointType != LINE_GRAPH_DATA_POINT_NONE && 
                valuePoint.y >= paintState.rect.y &&
                valuePoint.y <= paintState.rect.y + paintState.rect.height)
            {
                drawSeriesPoint(series, valuePoint);
            }

            prevPoint = valuePoint;
        }
    }
}

static void drawLines(leLineGraphWidget* graph)
{   
    if (graph->stacked == LE_TRUE)
    {
        drawStackedLines(graph);
    }
    //not stacked
    else
    {
        drawUnstackedLines(graph);
    }
}

static void drawLineGraph(leLineGraphWidget* graph)
{
    //leRect widgetRect;
    
    //widgetRect = graph->fn->rectToScreen(graph);


    _leLineGraphWidget_GetGraphRect(graph, &paintState.rect);
        
    paintState.pixelsPerUnit = (float) paintState.rect.height / ((float) graph->maxValue - (float) graph->minValue);
        
    // fill the base
    if(graph->fillGraphArea == LE_TRUE)
    {
        fillBase(graph);
    }

    // adjust the horizontal borders if ticks are shown
    if (graph->valueAxisTicksVisible == LE_TRUE)
    {
        drawValueAxisTickBorders(graph);
    }

    //Draw the labels for min/max values
    if (graph->valueAxisLabelsVisible == LE_TRUE)
    {
        drawMinMaxLabels(graph);
    }

    // determine the origin point
    paintState.originPoint = _leLineGraphWidget_GetOriginPoint(graph);
    
    if (graph->minValue < 0 && graph->maxValue > 0)
    {
        paintState.originValue = 0;
    }
    else if (graph->minValue >= 0)
    {
        paintState.originValue = graph->minValue;
    }
    else if (graph->maxValue <= 0)
    {
        paintState.originValue = graph->maxValue;
    }
    
    //Draw the ticks
    if (graph->valueAxisTicksVisible || graph->valueGridLinesVisible)
    {
        drawTicks(graph);
    }

    //Draw the fills if needed
    if (graph->fillValueArea == LE_TRUE &&
        graph->categories.size > 0 && 
        graph->dataSeries.size > 0)
    {
        drawFills(graph);
    }
    
    //Draw the origin line
    if(paintState.originValue == 0)
    {
        drawOriginLine(graph);
    }

    //Draw the category ticks
    if((graph->categAxisTicksVisible == LE_TRUE) && (graph->categories.size > 0))
    {
        drawCategoryTicks(graph);
    }
    
    //Draw the subticks, only if major ticks are also visible
    if (graph->valueAxisSubticksVisible && graph->valueAxisTicksVisible)
    {
        drawSubTicks(graph);
    }

    drawBorders(graph);
    
    //Draw the points/lines
    if(graph->categories.size > 0 && graph->dataSeries.size > 0)
    {
        drawLines(graph);
    }

    nextState(graph);
}

static void _leLineGraphWidget_GetCategoryTextRect(leLineGraphWidget* graph,
                                           uint32_t categoryIndex,
                                           const leRect* graphRect,
                                           leRect* textRect,
                                           leRect* drawRect)
{
    leLineGraphCategory * category;
    leRect bounds;
    
    *textRect = leRect_Zero;
    *drawRect = leRect_Zero;
    
    if (categoryIndex >= graph->categories.size)
        return;
    
    category = leArray_Get(&graph->categories, categoryIndex);
    
    if (category == NULL || category->text == NULL)
        return;
    
    category->text->fn->getRect(category->text, textRect);
    
    bounds.x = 0;
    bounds.y = 0;
    bounds.width = graphRect->width;
    bounds.height = textRect->height;
    
    leRectClip(textRect, &bounds, drawRect);

    //get center of category, arrange text below it
    switch (graph->categAxisTicksPosition)
    {
        case LINE_GRAPH_TICK_OUT:
        {
            textRect->y = graphRect->y + graphRect->height + graph->tickLength + LABEL_OFFSET_MIN_PIX;
            break;
        }
        case LINE_GRAPH_TICK_CENTER:
        {
            textRect->y = graphRect->y + graphRect->height + graph->tickLength / 2 + LABEL_OFFSET_MIN_PIX;
            break;
        }
        case LINE_GRAPH_TICK_IN:
        {
            textRect->y = graphRect->y + graphRect->height + LABEL_OFFSET_MIN_PIX;
            break;
        }
        default:
            break;
    }

    if (graph->fillValueArea == LE_TRUE  && graph->categories.size > 1)
    {
        textRect->x = graphRect->x + 
                     ((categoryIndex)* graphRect->width / (graph->categories.size - 1)) - textRect->width/2;
    }
    else
    {
        textRect->x = graphRect->x + 
                     ((categoryIndex + 1)* graphRect->width / (graph->categories.size + 1)) - textRect->width/2;
    }

    drawRect->x = textRect->x;
    drawRect->y = textRect->y;
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leLineGraphWidget* graph = (leLineGraphWidget*)strm->userData;

    graph->widget.drawState = DRAW_STRING;

    nextState(graph);
}
#endif

static void drawString(leLineGraphWidget* graph)
{
    leRect textRect, drawRect;
    leLineGraphCategory* category;
    uint32_t categoryIndex = 0;

    if (graph->categAxisLabelsVisible)
    {
        for (categoryIndex = 0; 
             (categoryIndex < (int32_t)graph->categories.size); 
             categoryIndex++)
        {
            category = leArray_Get(&graph->categories, categoryIndex);
            
            if (category == NULL || category->text == NULL)
                continue;
                
            _leLineGraphWidget_GetCategoryTextRect(graph,
                                                   categoryIndex,
                                                   &paintState.rect,
                                                   &textRect,
                                                   &drawRect);
            
            
            category->text->fn->_draw(category->text,
                                      textRect.x,
                                      textRect.y,
                                      LE_HALIGN_CENTER,
                                      graph->widget.scheme->text,
                                      paintState.alpha);

#if LE_STREAMING_ENABLED == 1
            if(leGetActiveStream() != NULL)
            {
                leGetActiveStream()->onDone = onStringStreamFinished;
                leGetActiveStream()->userData = graph;

                graph->widget.drawState = WAIT_STRING;

                return;
            }
#endif
        }
    }

    nextState(graph);
}

static void drawBorder(leLineGraphWidget* graph)
{    
    if(graph->widget.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)graph,
                                                    paintState.alpha);
    }
    else if(graph->widget.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)graph,
                                                      paintState.alpha);
    }
    
    nextState(graph);
}

void _leLineGraphWidget_Paint(leLineGraphWidget* graph)
{
    if(graph->widget.scheme == NULL)
    {
        graph->widget.drawState = DONE;
        
        return;
    }
    
    if(graph->widget.drawState == NOT_STARTED)
        nextState(graph);
    
    while(graph->widget.drawState != DONE)
    {
        graph->widget.drawFunc((leWidget*)graph);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif

#if LE_STREAMING_ENABLED == 1
        if(graph->widget.drawState == WAIT_STRING)
            break;
#endif
    }
}

#endif // LE_LINEGRAPH_WIDGET_ENABLED
