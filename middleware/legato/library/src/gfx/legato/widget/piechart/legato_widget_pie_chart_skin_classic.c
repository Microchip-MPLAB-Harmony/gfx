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


#include "gfx/legato/widget/piechart/legato_widget_pie_chart.h"

#if LE_PIECHART_WIDGET_ENABLED == 1

#include <stdio.h>

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_fixedstring.h"
#include "gfx/legato/string/legato_stringutils.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define MAX_TICK_LABEL_DIGITS 10
#define MAX_TICK_LABEL_VALUE 9999999999

static char strbuff[MAX_TICK_LABEL_DIGITS];
static leChar charbuff[MAX_TICK_LABEL_DIGITS];
static leFixedString str;

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_PIE_CHART,
    DRAW_BORDER,
};

static struct
{
    leRect pieRect;
    //lePoint p;
    lePoint pieCenter;
    uint32_t alpha;
} paintState;

static void drawBackground(lePieChartWidget* chart);
static void drawPieChart(lePieChartWidget* chart);
static void drawBorder(lePieChartWidget* chart);

static void nextState(lePieChartWidget* chart)
{
    switch(chart->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(chart->fn->getCumulativeAlphaEnabled(chart) == LE_TRUE)
            {
                paintState.alpha = chart->fn->getCumulativeAlphaAmount(chart);
            }
#endif
            
            if(chart->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                chart->widget.status.drawState = DRAW_BACKGROUND;
                chart->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            chart->widget.status.drawState = DRAW_PIE_CHART;
            chart->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawPieChart;
    
            return;
        }
        case DRAW_PIE_CHART:
        {            
            if(chart->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                chart->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                chart->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            chart->widget.status.drawState = DONE;
            chart->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(lePieChartWidget* chart)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)chart,
                                                paintState.alpha);
    
    nextState(chart);
}

static void drawSliceLabel(lePieChartWidget* chart,
                           lePieChartPie* pie)
{
    lePoint centerPoint;
    int32_t midAngle;
    
    leRect textRect, bounds, drawRect;

    sprintf(strbuff, "%ld", (int32_t)pie->value);

    leFixedString_Constructor(&str, charbuff, MAX_TICK_LABEL_DIGITS);
    str.fn->setFromCStr(&str, strbuff);
    str.fn->setFont(&str, chart->labelFont);

    //Get the string rectangle
    chart->fn->localRect(chart, &bounds);
    str.fn->getRect(&str, &textRect);

    leRectClip(&textRect, &bounds, &drawRect);

    //Find center angle of pie
    midAngle = pie->startAngle + (pie->value / 2);

    lePolarToXY(pie->radius + pie->offset + chart->labelsOffset, midAngle , &centerPoint);
    centerPoint.y *= -1;

    centerPoint.x += chart->widget.rect.x + (chart->widget.rect.width / 2);
    centerPoint.y += chart->widget.rect.y + (chart->widget.rect.height / 2);

    drawRect.x = centerPoint.x - (textRect.width / 2);
    drawRect.y = centerPoint.y - (textRect.height / 2);
    drawRect.width = textRect.width;
    drawRect.height = textRect.height;

    leStringUtils_KerningRect((leRasterFont*)str.fn->getFont(&str),
                              &drawRect);

    str.fn->_draw(&str,
                  drawRect.x,
                  drawRect.y,
                  LE_HALIGN_CENTER,
                  leScheme_GetRenderColor(chart->widget.scheme, LE_SCHM_TEXT),
                  paintState.alpha);

    str.fn->destructor(&str);
}

static void drawSliceArc(lePieChartWidget* chart,
                         lePieChartPie* pie,
                         leColor clr)
{
    uint32_t midAngle;
    lePoint center, offset;
    leRect arcRect;

    center.x = chart->widget.rect.x + paintState.pieCenter.x;
    center.y = chart->widget.rect.y + paintState.pieCenter.y;

    arcRect.x = center.x - (pie->radius / 2);
    arcRect.y = center.y - (pie->radius / 2);
    arcRect.width = pie->radius;
    arcRect.height = pie->radius;

    if (pie->offset != 0)
    {
        // find center angle of pie
        midAngle = pie->startAngle + (pie->spanAngle / 2);

        // find points of
        lePolarToXY(pie->offset, midAngle , &offset);
        offset.y *= -1;

        arcRect.x += offset.x;
        arcRect.y += offset.y;
    }

    leRenderer_ArcFill(&arcRect,
                       pie->startAngle,
                       pie->spanAngle,
                       pie->radius,
                       LE_FALSE,
                       clr,
                       LE_FALSE,
                       paintState.alpha);  
}
                         

static void drawPieSlice(lePieChartWidget* chart,
                         int32_t idx)
{
    lePieChartPie* pie = leArray_Get(&chart->pieArray, idx);
    leColor clr;
    
    if (pie->scheme != NULL)
    {
        clr = leScheme_GetRenderColor(pie->scheme, LE_SCHM_FOREGROUND);
    }
    else
    {
        clr = leScheme_GetRenderColor( chart->widget.scheme, LE_SCHM_FOREGROUND);
    }
                
    if (pie->radius > 0)
    {                
        drawSliceArc(chart, pie, clr);

        if (chart->labelsVisible == LE_TRUE && chart->labelFont != NULL)
        {
            drawSliceLabel(chart, pie);
        }
    }
}

static void drawPieChart(lePieChartWidget* chart)
{
    unsigned int i;
    
    paintState.pieRect.x = 0;
    paintState.pieRect.y = 0;
    paintState.pieRect.width = chart->widget.rect.width;
    paintState.pieRect.height = chart->widget.rect.height;

    leUtils_RectToScreenSpace((leWidget*)chart, &paintState.pieRect);

    paintState.pieCenter.x = paintState.pieRect.width / 2;
    paintState.pieCenter.y = paintState.pieRect.height / 2;

    // draw the pies
    for (i = 0; i < chart->pieArray.size; i++) 
    {
        drawPieSlice(chart, i);
    }        

    nextState(chart);
}

static void drawBorder(lePieChartWidget* chart)
{    
    if(chart->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)chart,
                                                    paintState.alpha);
    }
    else if(chart->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)chart,
                                                      paintState.alpha);
    }
    
    nextState(chart);
}

void _lePieChartWidget_Paint(lePieChartWidget* chart)
{
    if(chart->widget.status.drawState == NOT_STARTED)
    {
        nextState(chart);
    }
    
    while(chart->widget.status.drawState != DONE)
    {
        chart->widget.drawFunc((leWidget*)chart);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_PIECHART_WIDGET_ENABLED
