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
    int32_t startAngle;
    int32_t centerAngle;
    //lePoint p;
    lePoint pieCenter;
    uint32_t totalValue;
    uint32_t alpha;
} paintState;

static void drawBackground(lePieChartWidget* chart);
static void drawPieChart(lePieChartWidget* chart);
static void drawBorder(lePieChartWidget* chart);

static void nextState(lePieChartWidget* chart)
{
    switch(chart->widget.drawState)
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
            
            if(chart->widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                chart->widget.drawState = DRAW_BACKGROUND;
                chart->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            chart->widget.drawState = DRAW_PIE_CHART;
            chart->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawPieChart;
    
            return;
        }
        case DRAW_PIE_CHART:
        {            
            if(chart->widget.borderType != LE_WIDGET_BORDER_NONE)
            {
                chart->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                chart->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            chart->widget.drawState = DONE;
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
    uint32_t value;
    
    value = pie->value;

    //Protect from overflow
    if (value < MAX_TICK_LABEL_VALUE)
    {
        sprintf(strbuff, "%d", (int)value);
    }
    else
    {
        sprintf(strbuff, "---");
    }

    leFixedString_Constructor(&str, charbuff, MAX_TICK_LABEL_DIGITS);
    str.fn->setFromCStr(&str, strbuff);
    str.fn->setFont(&str, chart->labelFont);

    //Get the string rectangle
    bounds = chart->fn->localRect(chart);
    str.fn->getRect(&str, &textRect);

    leRectClip(&textRect, &bounds, &drawRect);

    //Find center angle of pie
    midAngle = paintState.startAngle + paintState.centerAngle / 2;

    if (midAngle > 360)
        midAngle %= 360;

    lePolarToXY(pie->offset + chart->labelsOffset, midAngle , &centerPoint);

    //Orient the text rectangle based on the label quadrant
    if (midAngle < 10)
    {
        drawRect.x = paintState.pieCenter.x + centerPoint.x;
        drawRect.y = paintState.pieCenter.y - centerPoint.y - (textRect.height/2);
    }
    else if (midAngle <= 80)
    {
        drawRect.x = paintState.pieCenter.x + centerPoint.x;
        drawRect.y = paintState.pieCenter.y - centerPoint.y - textRect.height;
    }
    else if (midAngle <= 100)
    {
        drawRect.x = paintState.pieCenter.x + centerPoint.x - (textRect.width/2);
        drawRect.y = paintState.pieCenter.y - centerPoint.y - textRect.height;
    }
    else if (midAngle <= 170)
    {
        drawRect.x = paintState.pieCenter.x + centerPoint.x - textRect.width;
        drawRect.y = paintState.pieCenter.y - centerPoint.y - textRect.height;
    }
    else if (midAngle <= 190)
    {
        drawRect.x = paintState.pieCenter.x + centerPoint.x - textRect.width;
        drawRect.y = paintState.pieCenter.y - centerPoint.y - (textRect.height/2);
    }
    else if (midAngle <= 260)
    {
        drawRect.x = paintState.pieCenter.x + centerPoint.x - textRect.width;
        drawRect.y = paintState.pieCenter.y - centerPoint.y;
    }
    else if (midAngle <= 280)
    {
        drawRect.x = paintState.pieCenter.x + centerPoint.x - (textRect.width/2);
        drawRect.y = paintState.pieCenter.y - centerPoint.y;
    }
    else if (midAngle <= 350)
    {
        drawRect.x = paintState.pieCenter.x + centerPoint.x;
        drawRect.y = paintState.pieCenter.y - centerPoint.y;
    }
    else
    {
        drawRect.x = paintState.pieCenter.x + centerPoint.x;
        drawRect.y = paintState.pieCenter.y - centerPoint.y - (textRect.height/2);
    }

//                drawRect.x = p.x + centerPoint.x;
//                drawRect.y = p.y - centerPoint.y;

    str.fn->_draw(&str,
                  drawRect.x,
                  drawRect.y,
                  LE_HALIGN_CENTER,
                  chart->widget.scheme->text,
                  paintState.alpha);

    str.fn->destructor(&str);
}

static void drawSliceArc(lePieChartWidget* chart,
                         lePieChartPie* pie,
                         leColor clr)
{
    lePoint offsetPoint;
    int32_t midAngle;
    lePoint center;

    center.x = paintState.pieCenter.x;
    center.y = paintState.pieCenter.y;

    if (pie->offset != 0)
    {
        //Find center angle of pie
        midAngle = paintState.startAngle + paintState.centerAngle / 2;

        if (midAngle > 360)
        {
            midAngle %= 360;
        }
        
        //Find points of 
        lePolarToXY(pie->offset, midAngle , &offsetPoint);

        center.x += offsetPoint.x;
        center.y -= offsetPoint.y;
    }

    leRenderer_ArcFill(&paintState.pieRect,
                       center.x,
                       center.y,
                       pie->radius,
                       paintState.startAngle,
                       paintState.centerAngle,
                       pie->radius,
                       clr,
                       LE_FALSE,
                       paintState.alpha);  
}
                         

static void drawPieSlice(lePieChartWidget* chart,
                         int32_t idx)
{
    lePieChartPie* pie = leArray_Get(&chart->pieArray, idx);
    leColor clr;
    
    //paintState.pieCenter = paintState.p;

    if (pie->scheme != NULL)
    {
        clr = pie->scheme->foreground;
        //GFX_Set(GFXF_DRAW_COLOR, pie->scheme->foreground);
        //GFX_Set(GFXF_DRAW_GRADIENT_COLOR, pie->scheme->foreground & 0xffffff00, pie->scheme->foreground, NULL, NULL);
    }
    else
    {
        clr = chart->widget.scheme->foreground;
        //GFX_Set(GFXF_DRAW_COLOR, chart->widget.scheme->foreground);
        //GFX_Set(GFXF_DRAW_GRADIENT_COLOR, chart->widget.scheme->foreground & 0xffffff00, chart->widget.scheme->foreground, NULL, NULL);
    }

    //GFX_Set(GFXF_DRAW_THICKNESS, pie->radius);
    
    if (chart->centerAngle < 0)
    {
        paintState.centerAngle = - ((int32_t) (((float) (-chart->centerAngle) * (float) pie->value)/(float) paintState.totalValue + 0.5));
    }
    else
    {
        paintState.centerAngle = (int32_t) (((float) (chart->centerAngle) * (float) pie->value)/(float) paintState.totalValue + 0.5);
    }
    
                
    if (pie->radius > 0)
    {                
        drawSliceArc(chart, pie, clr);

        if (chart->labelsVisible == LE_TRUE && chart->labelFont != NULL)
        {
            drawSliceLabel(chart, pie);
        }
    }
    
    paintState.startAngle += paintState.centerAngle;
    
    while (paintState.startAngle < 0)
    {
        paintState.startAngle += 360;
    }
}

static void drawPieChart(lePieChartWidget* chart)
{
    unsigned int i;
    
    paintState.totalValue = 0;
    
    //paintState.p.x = chart->widget.rect.width / 2;
    //paintState.p.y = chart->widget.rect.height / 2;
    
    paintState.startAngle = chart->startAngle;
    
    while(paintState.startAngle < 0)
    {
        paintState.startAngle += 360;
    }
    
    if(paintState.startAngle > 360)
    {
        paintState.startAngle %= 360;
    }
    
    
    //leUtils_PointToScreenSpace((leWidget*)chart, &paintState.p);

    paintState.pieRect.x = 0;
    paintState.pieRect.y = 0;
    paintState.pieRect.width = chart->widget.rect.width;
    paintState.pieRect.height = chart->widget.rect.height;

    leUtils_RectToScreenSpace((leWidget*)chart, &paintState.pieRect);

    paintState.pieCenter.x = paintState.pieRect.width / 2;
    paintState.pieCenter.y = paintState.pieRect.height / 2;

    //GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_LINE);
    //GFX_Set(GFXF_DRAW_COLOR, chart->widget.scheme->foreground);
        
    //Get the total
    for (i = 0; i < chart->pieArray.size; i++) 
    {
        lePieChartPie * pie = leArray_Get(&chart->pieArray, i);
        
        paintState.totalValue += pie->value;
    }

    //Draw the pies
    for (i = 0; i < chart->pieArray.size; i++) 
    {
        drawPieSlice(chart, i);
    }        

    nextState(chart);
}

static void drawBorder(lePieChartWidget* chart)
{    
    if(chart->widget.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)chart,
                                                    paintState.alpha);
    }
    else if(chart->widget.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)chart,
                                                      paintState.alpha);
    }
    
    nextState(chart);
}

void _lePieChartWidget_Paint(lePieChartWidget* chart)
{
    if(chart->widget.scheme == NULL)
    {
        chart->widget.drawState = DONE;
        
        return;
    }
    
    if(chart->widget.drawState == NOT_STARTED)
    {
        nextState(chart);
    }
    
    while(chart->widget.drawState != DONE)
    {
        chart->widget.drawFunc((leWidget*)chart);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_PIECHART_WIDGET_ENABLED
