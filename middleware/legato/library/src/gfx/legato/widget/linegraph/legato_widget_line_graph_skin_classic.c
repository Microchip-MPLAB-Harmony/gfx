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
    leRect graphRect;

    //leRect rect;
    float pixelsPerUnit;
    //leColor clr;
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
    switch(graph->widget.status.drawState)
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
            
            if(graph->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                graph->widget.status.drawState = DRAW_BACKGROUND;
                graph->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            graph->widget.status.drawState = DRAW_LINE_GRAPH;
            graph->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawLineGraph;
    
            return;
        }
        case DRAW_LINE_GRAPH:
        {            
            graph->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;
            graph->widget.status.drawState = DRAW_STRING;
            
            return;
        }
        case DRAW_STRING:
        {
            if(graph->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                graph->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                graph->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            graph->widget.status.drawState = DONE;
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

static void _calculateCategoryPoints(leLineGraphWidget* graph)
{
    leLineGraphCategory* cat;

    if(graph->categories.size == 0)
        return;

    int count = graph->categories.size;
    int segmentWidth = paintState.graphRect.width;

    if(count > 1)
    {
        segmentWidth /= (count - 1);
    }

    for(int i = 0; i < count; ++i)
    {
        cat = (leLineGraphCategory*)graph->categories.values[i];

        if(i == 0)
        {
            cat->drawX = paintState.graphRect.x;
        }
        else if(i == count - 1)
        {
            cat->drawX = paintState.graphRect.x + paintState.graphRect.width;
        }
        else
        {
            cat->drawX = paintState.graphRect.x + (segmentWidth * i);
        }
    }
}

static void _getCategoryTextRect(const leLineGraphWidget* graph,
                                 uint32_t catIdx,
                                 leRect* rect)
{
    *rect = leRect_Zero;
    leLineGraphCategory* cat;

    if (catIdx >= graph->categories.size)
        return;

    cat = (leLineGraphCategory*)graph->categories.values[catIdx];

    if(cat == NULL)
        return;

    if(cat->text == NULL)
        return;

    cat->text->fn->getRect(cat->text, rect);

    rect->x = cat->drawX;
    rect->x -= rect->width / 2;
    rect->y = paintState.graphRect.y + paintState.graphRect.height + 1;

    // get center of category, arrange text below it
    switch(graph->categAxisTicksPosition)
    {
        case LINE_GRAPH_TICK_OUT:
        {
            rect->y += 1 + graph->tickLength;

            break;
        }
        case LINE_GRAPH_TICK_CENTER:
        {
            rect->y += 1 + (graph->tickLength / 2);

            break;
        }
        case LINE_GRAPH_TICK_IN:
        {
            rect->y += 1;

            break;
        }
    }
}

static void _getCategoryLabelMaxDrawRect(const leLineGraphWidget* graph,
                                         leRect* labelRect)
{
    leLineGraphCategory* cat;
    leRect strRect;

    for(uint32_t i = 0; i < graph->categories.size; ++i)
    {
        cat = (leLineGraphCategory*)graph->categories.values[i];

        if(cat->text == NULL)
            continue;

        cat->text->fn->getRect(cat->text, &strRect);

        //strRect.adjust(0, 0, 0, 1);

        if(strRect.height > labelRect->height)
        {
            labelRect->height = strRect.height;
        }

        if(strRect.width > labelRect->width)
        {
            labelRect->width = strRect.width;
        }
    }
}

static void _getValueLabelMaxDrawRect(const leLineGraphWidget* graph,
                                      leRect* textRect)
{
    leRect minRect, maxRect;

    if(graph->ticksLabelFont == NULL)
        return;

    sprintf(paintState.strbuff, "%i", graph->minValue);

    leStringUtils_GetRectCStr(paintState.strbuff, graph->ticksLabelFont, &minRect);

    sprintf(paintState.strbuff, "%i", graph->maxValue);

    leStringUtils_GetRectCStr(paintState.strbuff, graph->ticksLabelFont, &maxRect);

    if(maxRect.width > minRect.width)
    {
        textRect->width = maxRect.width;
        textRect->height = maxRect.height;
    }
    else
    {
        textRect->width = minRect.width;
        textRect->height = minRect.height;
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

    if (position == LE_RELATIVE_POSITION_LEFTOF)
    {
        textRect.x = tickPoint.x - textRect.width - LABEL_OFFSET_MIN_PIX;
        textRect.y = tickPoint.y - textRect.height/2;
    }
    else if (position == LE_RELATIVE_POSITION_RIGHTOF)
    {
        textRect.x = tickPoint.x + textRect.width + LABEL_OFFSET_MIN_PIX;
        textRect.y = tickPoint.y - textRect.height/2;
    }
    else
    {
        textRect.x = tickPoint.x;
        textRect.y = tickPoint.y;
    }

    req.str = paintState.strbuff;
    req.font = graph->ticksLabelFont;
    req.x = textRect.x;
    req.y = textRect.y;
    req.align = LE_HALIGN_CENTER;
    req.color = clr;
    req.alpha = paintState.alpha;

    leStringRenderer_DrawCString(&req);
}

//Gets the rectangle of the graph area (without labels or ticks)
void _leLineGraphWidget_GetGraphRect(const leLineGraphWidget* graph,
                                     leRect* graphRect)
{
    leRect categoryLabelMaxRect;
    leRect valueLabelMaxRect;

    graphRect->x = graph->widget.rect.x;
    graphRect->y = graph->widget.rect.y;
    graphRect->width = graph->widget.rect.width;
    graphRect->height = graph->widget.rect.height;

    graphRect->x += graph->widget.margin.left;
    graphRect->y += graph->widget.margin.top;
    graphRect->width -= graph->widget.margin.left + graph->widget.margin.right;
    graphRect->height -= graph->widget.margin.top + graph->widget.margin.bottom;

    // adjust for value tick position
    if(graph->valueAxisTicksVisible == LE_TRUE)
    {
        switch(graph->valueAxisTicksPosition)
        {
            case LINE_GRAPH_TICK_OUT:
            {
                graphRect->x += graph->tickLength;
                graphRect->width -= graph->tickLength;

                break;
            }
            case LINE_GRAPH_TICK_CENTER:
            {
                graphRect->x += graph->tickLength / 2;
                graphRect->width -= graph->tickLength / 2;

                break;
            }
            default: { }
        }
    }

    // adjust for value labels
    if(graph->valueAxisLabelsVisible == LE_TRUE && graph->ticksLabelFont != NULL)
    {
        _getValueLabelMaxDrawRect(graph, &valueLabelMaxRect);

        //graphRect.adjust(0, valueLabelMaxRect.height / 2 + LABEL_OFFSET_MIN_PIX, 0, 0);

        if(valueLabelMaxRect.width < LABEL_OFFSET_MIN_PIX)
        {
            valueLabelMaxRect.width = LABEL_OFFSET_MIN_PIX;
        }

        if(valueLabelMaxRect.height < LABEL_OFFSET_MIN_PIX)
        {
            valueLabelMaxRect.height = LABEL_OFFSET_MIN_PIX;
        }

        graphRect->x += valueLabelMaxRect.width;
        graphRect->width -= valueLabelMaxRect.width;

        graphRect->y += valueLabelMaxRect.height;
        graphRect->height -= valueLabelMaxRect.height;
    }

    if(graph->categories.size > 0)
    {
        // adjust for category ticks
        if(graph->categAxisTicksVisible == LE_TRUE)
        {
            switch(graph->categAxisTicksPosition)
            {
                case LINE_GRAPH_TICK_OUT:
                {
                    graphRect->height -= graph->tickLength;

                    break;
                }
                case LINE_GRAPH_TICK_CENTER:
                {
                    graphRect->height -= graph->tickLength / 2;

                    break;
                }
                default: { }
            }
        }

        if(graph->categAxisLabelsVisible == LE_TRUE)
        {
            _getCategoryLabelMaxDrawRect(graph, &categoryLabelMaxRect);

            graphRect->height -= categoryLabelMaxRect.height;
        }
    }
}

static void fillBase(leLineGraphWidget* graph)
{
    leRenderer_RectFill(&paintState.graphRect,
                        leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_BACKGROUND),
                        paintState.alpha);
}

static void drawValueAxisTickBorders(leLineGraphWidget* graph)
{
    switch (graph->valueAxisTicksPosition)
    {
        case LINE_GRAPH_TICK_OUT:
        {
            leRenderer_HorzLine(paintState.graphRect.x - graph->tickLength,
                                paintState.graphRect.y,
                                paintState.graphRect.width,
                                leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                                paintState.alpha);
                                
            leRenderer_HorzLine(paintState.graphRect.x - graph->tickLength,
                                paintState.graphRect.y + paintState.graphRect.height,
                                paintState.graphRect.width,
                                leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                                paintState.alpha);

            break;
        }
        case LINE_GRAPH_TICK_CENTER:
        {
            leRenderer_HorzLine(paintState.graphRect.x - graph->tickLength / 2,
                                paintState.graphRect.y,
                                paintState.graphRect.width,
                                leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                                paintState.alpha);
                                
            leRenderer_HorzLine(paintState.graphRect.x - graph->tickLength / 2,
                                paintState.graphRect.y + paintState.graphRect.height,
                                paintState.graphRect.width,
                                leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
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
    
    drawPoint.x = paintState.graphRect.x;
    
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
    
    drawPoint.y = paintState.graphRect.y;
    
    drawTickLabelWithValue(graph,
                           drawPoint,
                           LE_RELATIVE_POSITION_LEFTOF,
                           graph->maxValue,
                           leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_TEXT));

    drawPoint.y = paintState.graphRect.y + paintState.graphRect.height;
    
    drawTickLabelWithValue(graph,
                           drawPoint,
                           LE_RELATIVE_POSITION_LEFTOF,
                           graph->minValue,
                           leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_TEXT));
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
                drawPoint.x = paintState.graphRect.x - graph->tickLength;
                break;
            }
            case LINE_GRAPH_TICK_CENTER:
            {
                drawPoint.x = paintState.graphRect.x - graph->tickLength / 2;
                break;
            }
            case LINE_GRAPH_TICK_IN:
            {
                drawPoint.x = paintState.graphRect.x;
                break;
            }
            default:
                break;
        }
        
        drawEndPoint.x = drawPoint.x + graph->tickLength;
        
    }
    else
    {
        drawPoint.x = paintState.graphRect.x;
        drawEndPoint = drawPoint;
    }
    
    if (graph->valueGridLinesVisible)
    {
        drawEndPoint.x = paintState.graphRect.x + paintState.graphRect.width;
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
            
            if(drawPoint.y > paintState.graphRect.y)
            {
                leRenderer_HorzLine(drawPoint.x,
                                    drawPoint.y,
                                    drawEndPoint.x - drawPoint.x,
                                    leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                                    paintState.alpha);

                if (graph->valueAxisLabelsVisible)
                {
                    drawTickLabelWithValue(graph,
                                           drawPoint,
                                           LE_RELATIVE_POSITION_LEFTOF,
                                           value,
                                           leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_TEXT));
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
            
            if (drawPoint.y < (paintState.graphRect.y + paintState.graphRect.height))
            {
                leRenderer_HorzLine(drawPoint.x,
                                    drawPoint.y,
                                    drawEndPoint.x - drawPoint.x,
                                    leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                                    paintState.alpha);
            
                if (graph->valueAxisLabelsVisible)
                {
                    drawTickLabelWithValue(graph,
                                           drawPoint,
                                           LE_RELATIVE_POSITION_LEFTOF,
                                           value,
                                           leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_TEXT));
                }
            }
        }
    }
}

static void fillRegion(leLineGraphWidget* graph,
                       leLineGraphCategory* category,
                       leLineGraphDataSeries* series,
                       int32_t categoryIndex,
                       lePoint valuePoint,
                       lePoint prevPoint,
                       leColor clr)
{
    leLineGraphCategory* prevCategory;
    int32_t prevOldValue;
    //Get the four points
    lePoint nowOld, nowNew; //Old and new points of current category
    lePoint prevOld, prevNew; //old and new points of previous category
    lePoint scanPointA;
    lePoint scanPointB;
    void* arrayVal;

    prevCategory = leArray_Get(&graph->categories, categoryIndex - 1);

    nowNew = valuePoint;
    nowOld = nowNew;

    prevNew = prevPoint;
    prevOld = prevNew;

    nowOld.y = paintState.originPoint.y - (int32_t) ((float) (category->stackValue  - paintState.originValue) * paintState.pixelsPerUnit);

    arrayVal = leArray_Get(&series->data, categoryIndex - 1);
    prevOldValue = (int32_t)arrayVal;
    prevOld.y = paintState.originPoint.y - (int32_t) ((float) (prevCategory->stackValue - prevOldValue  - paintState.originValue) * paintState.pixelsPerUnit);

    //Fill the region using vertical lines for now
    for (scanPointA.x = prevNew.x; scanPointA.x < nowNew.x; scanPointA.x++)
    {
        scanPointA.y = leGetYGivenXOnLine(prevNew, nowNew, scanPointA.x);

        if (scanPointA.y < paintState.graphRect.y)
        {
            scanPointA.y = paintState.graphRect.y;
        }
        else if (scanPointA.y > paintState.graphRect.y + paintState.graphRect.height)
        {
            scanPointA.y = paintState.graphRect.y + paintState.graphRect.height;
        }

        scanPointB.x = scanPointA.x;
        scanPointB.y = leGetYGivenXOnLine(prevOld, nowOld, scanPointA.x);

        if (scanPointB.y < paintState.graphRect.y)
        {
            scanPointB.y = paintState.graphRect.y;
        }
        else if (scanPointB.y > paintState.graphRect.y + paintState.graphRect.height)
        {
            scanPointB.y = paintState.graphRect.y + paintState.graphRect.height;
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
    void* arrayVal;
    
    for (categoryIndex = 0; (categoryIndex < (int32_t)graph->categories.size); categoryIndex++)
    {
        category = leArray_Get(&graph->categories, categoryIndex);
        category->stackValue = 0;
    }

    for (seriesIndex = 0; seriesIndex < (int32_t)graph->dataSeries.size; seriesIndex++)
    {
        prevPoint = paintState.originPoint;
        valuePoint = paintState.originPoint;

        series = leArray_Get(&graph->dataSeries, seriesIndex);

        for (categoryIndex = 0; (categoryIndex < (int32_t)graph->categories.size); categoryIndex++)
        {
            if (categoryIndex < (int32_t)series->data.size)
            {
                arrayVal = leArray_Get(&series->data, categoryIndex);
                value = (int32_t)arrayVal;
            }
            
            category = leArray_Get(&graph->categories, categoryIndex);

            valuePoint.x = paintState.originPoint.x + 
                          ((categoryIndex * paintState.graphRect.width) / (graph->categories.size - 1));

            topValue = category->stackValue;
            topValue += value;

            valuePoint.y = paintState.originPoint.y - (int32_t) ((float) (topValue - paintState.originValue) * paintState.pixelsPerUnit);

            if (categoryIndex > 0)
            {
                fillRegion(graph,
                           category,
                           series,
                           categoryIndex,
                           valuePoint,
                           prevPoint,
                           leScheme_GetRenderColor(series->scheme, LE_SCHM_BASE));
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
    void* arrayVal;
            
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
                arrayVal = leArray_Get(&series->data, categoryIndex);
                value = (int32_t)arrayVal;

                if (graph->fillValueArea == LE_TRUE && (int32_t)graph->categories.size > 1)
                {
                    valuePoint.x = paintState.originPoint.x + ((categoryIndex * paintState.graphRect.width) / (graph->categories.size - 1));
                }
                else
                {
                    valuePoint.x = paintState.originPoint.x + (((categoryIndex + 1) * paintState.graphRect.width) / (graph->categories.size + 1));
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
                        
                        if (scanPoint.y <= paintState.graphRect.y)
                        {
                            scanPoint.y = paintState.graphRect.y;
                        }
                        else if (scanPoint.y >= paintState.graphRect.y + paintState.graphRect.height)
                        {
                            scanPoint.y = paintState.graphRect.y + paintState.graphRect.height;
                        }
                        
                        leRenderer_DrawLine(scanPoint.x,
                                            scanPoint.y,
                                            scanPoint.x,
                                            paintState.originPoint.y,
                                            leScheme_GetRenderColor(series->scheme, LE_SCHM_BASE),
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
                            paintState.graphRect.x + paintState.graphRect.width,
                            drawPoint.y,
                            leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
    }
    else
    {
        leRenderer_DrawLine(drawPoint.x,
                            drawPoint.y,
                            paintState.graphRect.x + paintState.graphRect.width,
                            drawPoint.y,
                            leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
    }

    if (graph->valueAxisLabelsVisible)
    {
        drawTickLabelWithValue(graph,
                               drawPoint,
                               LE_RELATIVE_POSITION_LEFTOF,
                               paintState.originValue,
                               leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_TEXT));
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
    
    if (drawPoint.y < paintState.graphRect.y)
    {
        length = paintState.originPoint.y - paintState.graphRect.y;
        
        drawPoint.y = paintState.graphRect.y;
    }
    
    for (categoryIndex = 0; 
         ((drawPoint.x < paintState.graphRect.x + paintState.graphRect.width) &&
         (categoryIndex < graph->categories.size));
         categoryIndex++)
    {
        if (graph->fillValueArea == LE_TRUE && graph->categories.size > 1)
        {
            drawPoint.x = paintState.graphRect.x + categoryIndex * paintState.graphRect.width / (graph->categories.size - 1);
        }
        else
        {
            drawPoint.x = paintState.graphRect.x + (categoryIndex + 1) * paintState.graphRect.width / (graph->categories.size + 1);
        }
        
        leRenderer_VertLine(drawPoint.x,
                            drawPoint.y,
                            length,
                            leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
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
            drawPoint.x = paintState.graphRect.x - subTickLength;
            break;
        }
        case LINE_GRAPH_TICK_CENTER:
        {
            drawPoint.x = paintState.graphRect.x - subTickLength / 2;
            break;
        }
        case LINE_GRAPH_TICK_IN:
        {
            drawPoint.x = paintState.graphRect.x;
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

        if (drawPoint.y > paintState.graphRect.y)
        {
            leRenderer_HorzLine(drawPoint.x,
                                drawPoint.y,
                                subTickLength,
                                leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
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
        
        if (drawPoint.y < (paintState.graphRect.y + paintState.graphRect.height))
        {
            leRenderer_HorzLine(drawPoint.x,
                                drawPoint.y,
                                subTickLength,
                                leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                                paintState.alpha);
        }              
    }
}

static void drawBorders(leLineGraphWidget* graph)
{
    // draw the borders
    leRenderer_VertLine(paintState.graphRect.x,
                        paintState.graphRect.y,
                        paintState.graphRect.height,
                        leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
                        
    leRenderer_VertLine(paintState.graphRect.x + paintState.graphRect.width,
                        paintState.graphRect.y,
                        paintState.graphRect.height,
                        leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
    
    leRenderer_HorzLine(paintState.graphRect.x,
                        paintState.graphRect.y,
                        paintState.graphRect.width,
                        leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
                        
    leRenderer_HorzLine(paintState.graphRect.x,
                        paintState.graphRect.y + paintState.graphRect.height,
                        paintState.graphRect.width,
                        leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
}

static void drawSeriesPoint(leLineGraphDataSeries* series,
                            const lePoint pnt)
{
    leColor clr = leScheme_GetRenderColor(series->scheme, LE_SCHM_FOREGROUND);

    leRect drawRect;
    drawRect.x = pnt.x - series->pointSize / 2;
    drawRect.y = pnt.y - series->pointSize / 2;
    drawRect.width = series->pointSize;
    drawRect.height = series->pointSize;

    if(series->pointType == LINE_GRAPH_DATA_POINT_CIRCLE)
    {
        if(series->fillPoints == LE_TRUE)
        {
            leRenderer_CircleFill(&drawRect,
                                  0,
                                  0,
                                  clr,
                                  paintState.alpha);
        }
        else
        {
            leRenderer_CircleDraw(&drawRect,
                                  1,
                                  clr,
                                  paintState.alpha);
        }
    }
    else if(series->pointType == LINE_GRAPH_DATA_POINT_SQUARE)
    {
        if(series->fillPoints == LE_TRUE)
        {
            leRenderer_RectFill(&drawRect,
                                clr,
                                paintState.alpha);
        }
        else
        {
            leRenderer_RectLine(&drawRect,
                                clr,
                                paintState.alpha);
        }
    }
}

static int LineGetXGivenY(int x1, int y1, int x2, int y2, int y3)
{
    if (y1 == y2)
        return x1;

    return (x1 - ((x1 - x2) * (y1 - y3))/(y1 - y2));
}

/*static int LineGetYGivenX(int x1, int y1, int x2, int y2, int x3)
{
    if (x1 == x2)
        return y1;

    return (y1 - ((y1 - y2) * (x1 - x3))/(x1 - x2));
}*/

static void drawUnstackedLines(leLineGraphWidget* graph)
{
    uint32_t itr, catIdx;
    lePoint prevPoint;
    lePoint valuePoint;
    lePoint start, end;
    leColor clr;
    leLineGraphDataSeries* series;

    //determine the width of each category + L & R margins
    for(itr = 0; itr < graph->dataSeries.size; ++itr)
    {
        series = (leLineGraphDataSeries*)graph->dataSeries.values[itr];

        prevPoint = paintState.originPoint;
        valuePoint = paintState.originPoint;

        clr = leScheme_GetRenderColor(series->scheme, LE_SCHM_FOREGROUND);

        for(catIdx = 0; catIdx < graph->categories.size; ++catIdx)
        {
            int32_t value = (int32_t)series->data.values[catIdx];

            if(graph->fillGraphArea == LE_TRUE && graph->categories.size > 1)
            {
                valuePoint.x = paintState.originPoint.x + ((catIdx * paintState.graphRect.width) / (graph->categories.size - 1));
            }
            else
            {
                valuePoint.x = paintState.originPoint.x + (((catIdx + 1) * paintState.graphRect.width) / (graph->categories.size + 1));
            }

            if (value >= paintState.originValue)
            {
                valuePoint.y = paintState.originPoint.y - (int) ((float)(value - paintState.originValue) * paintState.pixelsPerUnit);
            }
            // draw the points below the origin value
            else if (value < paintState.originValue)
            {
                valuePoint.y = paintState.originPoint.y + (int) ((float)(paintState.originValue - value) * paintState.pixelsPerUnit);
            }

            if(series->drawLines == LE_TRUE && catIdx > 0)
            {
                start = prevPoint;
                end = valuePoint;

                if(start.y < paintState.graphRect.y)
                {
                    start.y = paintState.graphRect.y;

                    start.x = LineGetXGivenY(prevPoint.x,
                                             prevPoint.y,
                                             valuePoint.x,
                                             valuePoint.y,
                                             start.y);
                }
                else if(start.y > paintState.graphRect.y + paintState.graphRect.height)
                {
                    start.y = paintState.graphRect.y + paintState.graphRect.height;

                    start.x = LineGetXGivenY(prevPoint.x,
                                             prevPoint.y,
                                             valuePoint.x,
                                             valuePoint.y,
                                             start.y);
                }

                if (end.y < paintState.graphRect.y)
                {
                    end.y = paintState.graphRect.y;

                    end.x = LineGetXGivenY(prevPoint.x,
                                           prevPoint.y,
                                           valuePoint.x,
                                           valuePoint.y,
                                           end.y);
                }
                else if(end.y > paintState.graphRect.y + paintState.graphRect.height)
                {
                    end.y = paintState.graphRect.y + paintState.graphRect.height;

                    end.x = LineGetXGivenY(prevPoint.x,
                                           prevPoint.y,
                                           valuePoint.x,
                                           valuePoint.y,
                                           end.y);
                }

                // draw lines
                if(series->drawLines == LE_TRUE && catIdx > 0)
                {
                    leRenderer_DrawLine(start.x,
                                        start.y,
                                        end.x,
                                        end.y,
                                        clr,
                                        paintState.alpha);
                }
            }

            //draw the point if it's within graph area
            if(series->pointType != LINE_GRAPH_DATA_POINT_NONE &&
               valuePoint.y >= paintState.graphRect.y &&
               valuePoint.y <= paintState.graphRect.y + paintState.graphRect.height)
            {
                drawSeriesPoint(series, valuePoint);
            }

            prevPoint.x = valuePoint.x;
            prevPoint.y = valuePoint.y;
        }
    }
}

static void drawStackedLines(leLineGraphWidget* graph)
{
    uint32_t itr;
    lePoint valuePoint;
    int32_t topValue;
    leLineGraphDataSeries* series;
    leLineGraphCategory *cat, *prevCat;
    leColor clr;
    uint32_t catIdx;
    lePoint start, end;

    for(catIdx = 0; catIdx < graph->categories.size; ++catIdx)
    {
        cat = graph->categories.values[catIdx];
        cat->stackValue = 0;
    }

    for(itr = 0; itr < graph->dataSeries.size; ++itr)
    {
        valuePoint = paintState.originPoint;

        series = (leLineGraphDataSeries*)graph->dataSeries.values[itr];

        clr = leScheme_GetRenderColor(series->scheme, LE_SCHM_FOREGROUND);

        for(catIdx = 0; catIdx < graph->categories.size; ++catIdx)
        {
            cat = graph->categories.values[catIdx];

            int32_t value = (int32_t)series->data.values[catIdx];

            if(graph->fillGraphArea == LE_TRUE && graph->categories.size > 1)
            {
                valuePoint.x = paintState.originPoint.x + ((catIdx * paintState.graphRect.width) / (graph->categories.size - 1));
            }
            else
            {
                valuePoint.x = paintState.originPoint.x + (((catIdx + 1) * paintState.graphRect.width) / (graph->categories.size + 1));
            }

            topValue = cat->stackValue + value;

            valuePoint.y = paintState.originPoint.y - (int) ((float) (topValue - paintState.originValue) * paintState.pixelsPerUnit);

            if(series->drawLines == LE_TRUE && catIdx > 0)
            {
                if(catIdx > 0)
                {
                    prevCat = graph->categories.values[catIdx - 1];

                    if(graph->fillGraphArea == LE_TRUE && graph->categories.size > 1)
                    {
                        start.x = paintState.originPoint.x + ((catIdx - 1) * paintState.graphRect.width) / (graph->categories.size - 1);
                    }
                    else
                    {
                        start.x = paintState.originPoint.x + (((catIdx - 1) * paintState.graphRect.width) / (graph->categories.size + 1));
                    }

                    start.y = paintState.originPoint.y - (int) ((float) (prevCat->stackValue  - paintState.originValue) * paintState.pixelsPerUnit);
                }

                end.x = valuePoint.x;
                end.y = valuePoint.y;

                if (start.y < paintState.graphRect.y)
                {
                    start.x = LineGetXGivenY(start.x,
                                             start.y,
                                             valuePoint.x,
                                             valuePoint.y,
                                             paintState.graphRect.y);

                    start.y = paintState.graphRect.y;

                }
                else if (start.y > paintState.graphRect.y + paintState.graphRect.height)
                {
                    start.x = LineGetXGivenY(start.x,
                                             start.y,
                                             valuePoint.x,
                                             valuePoint.y,
                                             paintState.graphRect.y + paintState.graphRect.height);

                    start.y = paintState.graphRect.y + paintState.graphRect.height;
                }

                if (end.y < paintState.graphRect.y)
                {
                    end.x = LineGetXGivenY(end.x,
                                              end.y,
                                              start.x,
                                              start.y,
                                              paintState.graphRect.y);

                    end.y = paintState.graphRect.y;
                }
                else if (end.y > paintState.graphRect.y + paintState.graphRect.height)
                {
                    end.x = LineGetXGivenY(end.x,
                                              end.y,
                                              start.x,
                                              start.y,
                                              paintState.graphRect.y + paintState.graphRect.height);

                    end.y = paintState.graphRect.y + paintState.graphRect.height;
                }

                leRenderer_DrawLine(start.x,
                                    start.y,
                                    end.x,
                                    end.y,
                                    clr,
                                    paintState.alpha);
            }

            //Draw the point if it's within graph area
            if(series->pointType != LINE_GRAPH_DATA_POINT_NONE &&
               (valuePoint.y >= paintState.graphRect.y) &&
               (valuePoint.y <= paintState.graphRect.y + paintState.graphRect.height))
            {
                drawSeriesPoint(series, valuePoint);
            }

            cat->stackValue = topValue;
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
    _leLineGraphWidget_GetGraphRect(graph, &paintState.graphRect);
        
    paintState.pixelsPerUnit = (float) paintState.graphRect.height / ((float) graph->maxValue - (float) graph->minValue);
        
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

    //determine the origin axis
    paintState.originPoint.x = paintState.graphRect.x;

    if(graph->minValue < 0 && graph->maxValue > 0)
    {
        paintState.originPoint.y = paintState.graphRect.y + (int) ((float) graph->maxValue * paintState.pixelsPerUnit);
        paintState.originValue = 0;
    }
    else if(graph->minValue >= 0)
    {
        paintState.originPoint.y = paintState.graphRect.y + paintState.graphRect.height;
        paintState.originValue = graph->minValue;
    }
    else if(graph->minValue <= 0)
    {
        paintState.originPoint.y = paintState.graphRect.y;
        paintState.originValue = graph->minValue;
    }

    _calculateCategoryPoints(graph);

    // fill the base
    if(graph->fillGraphArea == LE_TRUE)
    {
        fillBase(graph);
    }

    // draw the fills
    if (graph->fillValueArea == LE_TRUE &&
        graph->categories.size > 0 &&
        graph->dataSeries.size > 0)
    {
        drawFills(graph);
    }

    // draw the origin line
    if(paintState.originValue == 0)
    {
        drawOriginLine(graph);
    }

    // draw the points/lines
    if(graph->categories.size > 0 && graph->dataSeries.size > 0)
    {
        drawLines(graph);
    }

    // draw the ticks
    if (graph->valueAxisTicksVisible || graph->valueGridLinesVisible)
    {
        drawTicks(graph);
    }

    // draw the category ticks
    if((graph->categAxisTicksVisible == LE_TRUE) && (graph->categories.size > 0))
    {
        drawCategoryTicks(graph);
    }

    // draw the subticks, only if major ticks are also visible
    if (graph->valueAxisSubticksVisible && graph->valueAxisTicksVisible)
    {
        drawSubTicks(graph);
    }

    drawBorders(graph);

    nextState(graph);
}


#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leLineGraphWidget* graph = (leLineGraphWidget*)strm->userData;

    graph->widget.status.drawState = DRAW_STRING;

    nextState(graph);
}
#endif

static void drawString(leLineGraphWidget* graph)
{
    leRect textRect;
    leLineGraphCategory* category;
    uint32_t categoryIndex = 0;

    if (graph->categAxisLabelsVisible)
    {
        for (categoryIndex = 0; (categoryIndex < graph->categories.size); categoryIndex++)
        {
            category = leArray_Get(&graph->categories, categoryIndex);
            
            if (category == NULL || category->text == NULL)
                continue;

            _getCategoryTextRect(graph,
                                 categoryIndex,
                                 &textRect);

            category->text->fn->_draw(category->text,
                                      textRect.x,
                                      textRect.y,
                                      LE_HALIGN_CENTER,
                                      leScheme_GetRenderColor(graph->widget.scheme, LE_SCHM_TEXT),
                                      paintState.alpha);

#if LE_STREAMING_ENABLED == 1
            if(leGetActiveStream() != NULL)
            {
                leGetActiveStream()->onDone = onStringStreamFinished;
                leGetActiveStream()->userData = graph;

                graph->widget.status.drawState = WAIT_STRING;

                return;
            }
#endif
        }
    }

    nextState(graph);
}

static void drawBorder(leLineGraphWidget* graph)
{    
    if(graph->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)graph,
                                                    paintState.alpha);
    }
    else if(graph->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)graph,
                                                      paintState.alpha);
    }
    
    nextState(graph);
}

void _leLineGraphWidget_Paint(leLineGraphWidget* graph)
{
    if(graph->widget.status.drawState == NOT_STARTED)
        nextState(graph);
    
    while(graph->widget.status.drawState != DONE)
    {
        graph->widget.drawFunc((leWidget*)graph);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif

#if LE_STREAMING_ENABLED == 1
        if(graph->widget.status.drawState == WAIT_STRING)
            break;
#endif
    }
}

#endif // LE_LINEGRAPH_WIDGET_ENABLED
