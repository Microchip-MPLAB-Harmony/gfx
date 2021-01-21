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


#include "gfx/legato/widget/circulargauge/legato_widget_circular_gauge.h"

#if LE_CIRCULARGAUGE_WIDGET_ENABLED == 1

#include <stdlib.h>
#include <stdio.h>
#include <gfx/legato/legato.h>

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string_renderer.h"
#include "gfx/legato/string/legato_stringutils.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"


#include "gfx/legato/common/legato_math.h"

#define MAX_TICK_LABEL_DIGITS 10
#define MAX_TICK_LABEL_VALUE 9999999999
#define LABEL_OFFSET_MIN_PIX 2

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_CIRCULAR_GAUGE,
    DRAW_BORDER,
};

static struct
{
    leFixedString labelStr;
    lePoint p;
    leRect gaugeRect;
    float degPerUnit;
    uint32_t alpha;
    char strbuff[MAX_TICK_LABEL_DIGITS];
} paintState;

static void drawBackground(leCircularGaugeWidget* gauge);
static void drawCircularGauge(leCircularGaugeWidget* gauge);
static void drawBorder(leCircularGaugeWidget* gauge);

static void nextState(leCircularGaugeWidget* gauge)
{
    switch(gauge->widget.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(gauge->fn->getCumulativeAlphaEnabled(gauge) == LE_TRUE)
            {
                paintState.alpha = gauge->fn->getCumulativeAlphaAmount(gauge);
            }
#endif
            
            if(gauge->widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                gauge->widget.drawState = DRAW_BACKGROUND;
                gauge->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            gauge->widget.drawState = DRAW_CIRCULAR_GAUGE;
            gauge->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawCircularGauge;
    
            return;
        }
        case DRAW_CIRCULAR_GAUGE:
        {            
            if(gauge->widget.borderType != LE_WIDGET_BORDER_NONE)
            {
                gauge->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                gauge->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            gauge->widget.drawState = DONE;
            gauge->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leCircularGaugeWidget* gauge)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)gauge,
                                                paintState.alpha);
    
    nextState(gauge);
}

void drawTickLabelsAtAngleWithValue(const leCircularGaugeWidget* gauge, 
                                    int tickAbsAngle,
                                    int radius,
                                    leCircularGaugeWidgetLabelPosition position,
                                    int value,
                                    leColor clr)
{
    lePoint labelPoint;
    lePoint p;
    leRect textRect;
    leCStringRenderRequest req;

    if(gauge->ticksLabelFont == NULL)
        return;

    p.x = gauge->fn->getWidth(gauge) / 2;
    p.y = gauge->fn->getHeight(gauge) / 2;
    
    leUtils_PointToScreenSpace((leWidget*)gauge, &p);

    while (tickAbsAngle < 0)
    {
        tickAbsAngle += 360;
    }
    
    if (tickAbsAngle >= 360)
    {
        tickAbsAngle %= 360;
    }
    
    lePolarToXY(radius, tickAbsAngle, &labelPoint);
    
    //Draw the tick labels
    //Protect from overflow
    if (value < MAX_TICK_LABEL_VALUE)
    {
        sprintf(paintState.strbuff, "%d", value);
    }
    else
    {
        sprintf(paintState.strbuff, "---");
    }

    //Get the string rectangle
    //bounds = gauge->fn->localRect(gauge);
    leStringUtils_GetRectCStr(paintState.strbuff, gauge->ticksLabelFont, &textRect);

#if 0
    //Orient the text rectangle based on the tick quadrant
    if (position == CIRCULAR_GAUGE_LABEL_OUTSIDE)
    {
        if (tickAbsAngle < 10)
        {
            drawRect.x = p.x + labelPoint.x;
            drawRect.y = p.y - labelPoint.y - (textRect.height/2);
        }
        else if (tickAbsAngle <= 80)
        {
            drawRect.x = p.x + labelPoint.x;
            drawRect.y = p.y - labelPoint.y - textRect.height;
        }
        else if (tickAbsAngle <= 100)
        {
            drawRect.x = p.x + labelPoint.x - (textRect.width/2);
            drawRect.y = p.y - labelPoint.y - textRect.height;
        }
        else if (tickAbsAngle <= 170)
        {
            drawRect.x = p.x + labelPoint.x - textRect.width;
            drawRect.y = p.y - labelPoint.y - textRect.height;
        }
        else if (tickAbsAngle <= 190)
        {
            drawRect.x = p.x + labelPoint.x - textRect.width;
            drawRect.y = p.y - labelPoint.y - (textRect.height/2);
        }
        else if (tickAbsAngle <= 260)
        {
            drawRect.x = p.x + labelPoint.x - textRect.width;
            drawRect.y = p.y - labelPoint.y;
        }
        else if (tickAbsAngle <= 280)
        {
            drawRect.x = p.x + labelPoint.x - (textRect.width/2);
            drawRect.y = p.y - labelPoint.y;
        }
        else if (tickAbsAngle <= 350)
        {
            drawRect.x = p.x + labelPoint.x;
            drawRect.y = p.y - labelPoint.y;
        }
        else
        {
            drawRect.x = p.x + labelPoint.x;
            drawRect.y = p.y - labelPoint.y - (textRect.height/2);
        }
    }
    else
    {
        if (tickAbsAngle < 10)
        {
            drawRect.x = p.x + labelPoint.x - textRect.width;
            drawRect.y = p.y - labelPoint.y - textRect.height/2;
        }
        else if (tickAbsAngle <= 80)
        {
            drawRect.x = p.x + labelPoint.x - textRect.width;
            drawRect.y = p.y - labelPoint.y;
        }
        else if (tickAbsAngle <= 100)
        {
            drawRect.x = p.x + labelPoint.x - textRect.width/2;
            drawRect.y = p.y - labelPoint.y;
        }
        else if (tickAbsAngle <= 170)
        {
            drawRect.x = p.x + labelPoint.x;
            drawRect.y = p.y - labelPoint.y;
        }
        else if (tickAbsAngle <= 190)
        {
            drawRect.x = p.x + labelPoint.x;
            drawRect.y = p.y - labelPoint.y - textRect.height/2;
        }
        else if (tickAbsAngle <= 260)
        {
            drawRect.x = p.x + labelPoint.x;
            drawRect.y = p.y - labelPoint.y - textRect.height;
        }
        else if (tickAbsAngle <= 280)
        {
            drawRect.x = p.x + labelPoint.x - textRect.height/2;
            drawRect.y = p.y - labelPoint.y - textRect.height;
        }
        else if (tickAbsAngle <= 350)
        {
            drawRect.x = p.x + labelPoint.x - textRect.width;
            drawRect.y = p.y - labelPoint.y - textRect.height;
        }
        else
        {
            drawRect.x = p.x + labelPoint.x - textRect.width;
            drawRect.y = p.y - labelPoint.y - textRect.height/2;
        }
    }
#endif

    req.str = paintState.strbuff;
    req.font = gauge->ticksLabelFont;
    req.x = textRect.x;
    req.y = textRect.y;
    req.align = LE_HALIGN_CENTER;
    req.color = clr;
    req.alpha = paintState.alpha;

    leStringRenderer_DrawCString(&req);
}

static void drawTicksAtAngle(leCircularGaugeWidget* gauge, 
                             uint32_t radius,
                             uint32_t length,
                             int32_t tickAbsAngle,
                             leColor clr)
{
    lePoint outerPoint, innerPoint;
    lePoint p;

    p.x = gauge->fn->getWidth(gauge) / 2;
    p.y = gauge->fn->getHeight(gauge) / 2;
    
    leUtils_PointToScreenSpace((leWidget*)gauge, &p);

    while (tickAbsAngle < 0)
    {
        tickAbsAngle += 360;
    }
    
    if (tickAbsAngle >= 360)
    {
        tickAbsAngle %= 360;
    }
    
    lePolarToXY(radius, tickAbsAngle , &outerPoint);
    lePolarToXY(radius - length, tickAbsAngle , &innerPoint);
    
    leRenderer_DrawLine(p.x + outerPoint.x,
                        p.y - outerPoint.y,
                        p.x + innerPoint.x,
                        p.y - innerPoint.y,
                        clr,
                        paintState.alpha);
}

static void drawArcArray(leCircularGaugeWidget* gauge)
{
    leCircularGaugeArc* arc;
    leColor clr/*, gradient1, gradient2*/;
    uint32_t i;
    
    for (i = 0; i < gauge->arcsArray.size; i++)
    {
        arc = leArray_Get(&gauge->arcsArray, i);

        if (arc->scheme != NULL)
        {
            clr = arc->scheme->foreground;
            //gradient1 = arc->scheme->foreground & 0xffffff00;
            //gradient2 = arc->scheme->foreground;
        }
        else
        {
            clr = gauge->widget.scheme->foreground;
            //gradient1 = gauge->widget.scheme->foreground & 0xffffff00;
            //gradient2 = gauge->widget.scheme->foreground;
        }

        if (arc->type == VALUE_ARC)
        {
            arc->startAngle = gauge->startAngle + (int32_t) (paintState.degPerUnit * (float) (arc->startValue - gauge->startValue));
            arc->endAngle = gauge->startAngle  + (int32_t) (paintState.degPerUnit * (float)(arc->endValue - gauge->startValue));
        }

        leRenderer_ArcFill(&paintState.gaugeRect,
                           paintState.p.x - paintState.gaugeRect.x,
                           paintState.p.y - paintState.gaugeRect.y,
                           arc->radius,
                           arc->startAngle,
                           arc->endAngle - arc->startAngle,
                           arc->thickness,
                           clr,
                           LE_FALSE,
                           paintState.alpha);
    }
}

static void drawMinorTicks(leCircularGaugeWidget* gauge)
{
    int32_t tickAngleOffset;
    int32_t unitOffset;
    leCircularGaugeTick* ticks;
    leColor clr;
    uint32_t i;
    
    for (i = 0; i < gauge->ticksArray.size; i++)
    {
        ticks = leArray_Get(&gauge->ticksArray, i);
        unitOffset = 0;

        if (ticks->startValue >= gauge->startValue &&
            ticks->endValue <= gauge->endValue &&
            ticks->interval > 0 &&
            ticks->radius > 0 &&
            ticks->length > 0)
        {
            if (ticks->scheme != NULL)
            {
                clr = ticks->scheme->foreground;
            }
            else
            {
                clr = gauge->widget.scheme->foregroundInactive;
            }
            
            //draw start and middle ticks
            while((ticks->startValue + unitOffset) < ticks->endValue)
            {
                tickAngleOffset = (int) ((float) (ticks->startValue - gauge->startValue + unitOffset) * paintState.degPerUnit);

                drawTicksAtAngle(gauge,
                                 ticks->radius,
                                 ticks->length,
                                 gauge->startAngle + tickAngleOffset,
                                 clr);

                unitOffset += ticks->interval;
            }

            //draw tick at end
            tickAngleOffset = (int) ((float) (ticks->endValue - gauge->startValue) * paintState.degPerUnit);
            
            drawTicksAtAngle(gauge,
                             ticks->radius,
                             ticks->length,
                             gauge->startAngle + tickAngleOffset,
                             clr);
        }
    }
}

static void drawMinorTickLabels(leCircularGaugeWidget* gauge)
{
    int32_t tickAngleOffset;
    int32_t unitOffset;
    leCircularGaugeLabel* labels;
    leColor clr;
    uint32_t i;
        
    for(i = 0; i < gauge->labelsArray.size; i++)
    {
        labels = leArray_Get(&gauge->labelsArray, i);
        unitOffset = 0;

        if (labels->startValue >= gauge->startValue &&
            labels->endValue <= gauge->endValue &&
            labels->interval > 0 &&
            labels->radius > 0)
        {
            if (labels->scheme != NULL)
            {
                clr = labels->scheme->text;
            }
            else
            {
                clr = gauge->widget.scheme->text;
            }
            
            //draw start and middle ticks
            while((labels->startValue + unitOffset) < labels->endValue)
            {
                tickAngleOffset = (int) ((float) (labels->startValue - gauge->startValue + unitOffset) * paintState.degPerUnit);

                drawTickLabelsAtAngleWithValue(gauge, 
                                               gauge->startAngle + tickAngleOffset,
                                               labels->radius,
                                               labels->position,
                                               labels->startValue + unitOffset,
                                               clr);

                unitOffset += labels->interval;
            }

            //draw tick at end
            tickAngleOffset = (int) ((float) (labels->endValue - gauge->startValue) * paintState.degPerUnit);
            
            if (tickAngleOffset != 360)
            {
                drawTickLabelsAtAngleWithValue(gauge, 
                                               gauge->startAngle + tickAngleOffset,
                                               labels->radius,
                                               labels->position,
                                               labels->endValue,
                                               clr);
            }
        }
    }
}

static void drawMajorTicks(leCircularGaugeWidget* gauge)
{  
    int32_t tickAngleOffset = 0;
    int32_t unitOffset;
        
    if (gauge->ticksVisible == LE_FALSE)
        return;
        
    unitOffset = gauge->tickValue;
        
    // draw the starting tick
    drawTicksAtAngle(gauge,
                     gauge->radius,
                     gauge->tickLength,
                     gauge->startAngle,
                     gauge->widget.scheme->foregroundInactive);
    
    while((gauge->startValue + unitOffset) < gauge->endValue)
    {
        tickAngleOffset = (int32_t) ((float) unitOffset * paintState.degPerUnit);

        drawTicksAtAngle(gauge,
                         gauge->radius,
                         gauge->tickLength,
                         gauge->startAngle + tickAngleOffset,
                         gauge->widget.scheme->foregroundInactive);

        unitOffset += gauge->tickValue;
    } 

    //Draw end tick, if not a full circle
    if(gauge->centerAngle != 360 && gauge->centerAngle != -360)
    {
        drawTicksAtAngle(gauge,
                         gauge->radius,
                         gauge->tickLength,
                         gauge->startAngle + gauge->centerAngle,
                         gauge->widget.scheme->foregroundInactive);
    }
}

static void drawMajorTickLabels(leCircularGaugeWidget* gauge)
{
    int32_t tickAngleOffset = 0;
    int32_t unitOffset;
        
    if(gauge->tickLabelsVisible == LE_FALSE)
        return;
        
    unitOffset = gauge->tickValue;

    // draw the starting tick
    drawTickLabelsAtAngleWithValue(gauge, 
                                   gauge->startAngle,
                                   gauge->radius + LABEL_OFFSET_MIN_PIX,
                                   CIRCULAR_GAUGE_LABEL_OUTSIDE,
                                   gauge->startValue,
                                   gauge->widget.scheme->text);
    
    while((gauge->startValue + unitOffset) < gauge->endValue)
    {
        tickAngleOffset = (int32_t) ((float) unitOffset * paintState.degPerUnit);

        drawTickLabelsAtAngleWithValue(gauge, 
                                       gauge->startAngle + tickAngleOffset,
                                       gauge->radius + LABEL_OFFSET_MIN_PIX,
                                       CIRCULAR_GAUGE_LABEL_OUTSIDE,
                                       gauge->startValue + unitOffset,
                                       gauge->widget.scheme->text);
  
        unitOffset += gauge->tickValue;
    } 

    //Draw end tick, if not a full circle
    if (gauge->centerAngle != 360 && gauge->centerAngle != -360)
    {
        drawTickLabelsAtAngleWithValue(gauge, 
                                       gauge->startAngle + gauge->centerAngle,
                                       gauge->radius + LABEL_OFFSET_MIN_PIX,
                                       CIRCULAR_GAUGE_LABEL_OUTSIDE,
                                       gauge->endValue,
                                       gauge->widget.scheme->text);
    }
}

static void drawHand(leCircularGaugeWidget* gauge,
                     leRect* rect)
{
    lePoint outerPoint, innerPoint;
    int32_t handAngleOffset = 0, handAbsAngle;
    int32_t unitOffset = 0;
        
    if (gauge->handVisible == LE_FALSE)
        return;

    unitOffset = gauge->value - gauge->startValue;
    handAngleOffset = (int32_t) ((float) unitOffset * paintState.degPerUnit);

    handAbsAngle = gauge->startAngle + handAngleOffset;
    
    while(handAbsAngle < 0)
    {
        handAbsAngle += 360;
    }
    
    if (handAbsAngle >= 360)
    {
        handAbsAngle %= 360;
    }

    innerPoint.x = 0;
    innerPoint.y = 0;
    
    lePolarToXY(gauge->handRadius, handAbsAngle, &outerPoint);
    
    //Draw the center circle
    if(gauge->centerCircleVisible == LE_TRUE)
    {
        //GFX_Set(GFXF_DRAW_COLOR, gauge->widget.scheme->foreground);
        //GFX_Set(GFXF_DRAW_GRADIENT_COLOR, gauge->widget.scheme->foreground & 0xffffff00, gauge->widget.scheme->foreground, NULL, NULL);
        //GFX_Set(GFXF_DRAW_THICKNESS, gauge->centerCircleThickness);



        leRenderer_ArcFill(rect,
                           rect->width >> 1,
                           rect->height >> 1,
                           gauge->centerCircleRadius,
                           0,
                           360,
                           gauge->centerCircleThickness,
                           gauge->widget.scheme->foreground,
                           LE_FALSE,
                           paintState.alpha);
        
        lePolarToXY(gauge->centerCircleRadius, handAbsAngle, &innerPoint);
    }
    
    leRenderer_DrawLine(paintState.p.x + outerPoint.x,
                        paintState.p.y - outerPoint.y,
                        paintState.p.x + innerPoint.x,
                        paintState.p.y - innerPoint.y,
                        gauge->widget.scheme->foreground,
                        paintState.alpha);
}

static void drawCircularGauge(leCircularGaugeWidget* gauge)
{
    paintState.p.x = gauge->fn->getWidth(gauge)/2;
    paintState.p.y = gauge->fn->getHeight(gauge)/2;
    
    leUtils_PointToScreenSpace((leWidget*)gauge, &paintState.p);

    paintState.gaugeRect.x = 0;
    paintState.gaugeRect.y = 0;
    paintState.gaugeRect.width = gauge->fn->getWidth(gauge);
    paintState.gaugeRect.height = gauge->fn->getHeight(gauge);

    paintState.degPerUnit = (float) gauge->centerAngle / (float) (leAbsoluteValue(gauge->endValue - gauge->startValue));
    
    leUtils_RectToScreenSpace((leWidget*)gauge, &paintState.gaugeRect);
    
    // draw the arcs
    drawArcArray(gauge);
    
    // draw the minor ticks
    drawMinorTicks(gauge);
    
    //draw the minor tick labels
    drawMinorTickLabels(gauge);

    //Draw the major ticks.
    drawMajorTicks(gauge);
    
    //Draw the major tick labels
    drawMajorTickLabels(gauge);

    //Draw the hand
    drawHand(gauge, &paintState.gaugeRect);

    nextState(gauge);
}

static void drawBorder(leCircularGaugeWidget* gauge)
{    
    if(gauge->widget.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)gauge,
                                                    paintState.alpha);
    }
    else if(gauge->widget.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)gauge,
                                                      paintState.alpha);
    }
    
    nextState(gauge);
}

void _leCircularGaugeWidget_Paint(leCircularGaugeWidget* gauge)
{    
    if(gauge->widget.scheme == NULL)
    {
        gauge->widget.drawState = DONE;
        
        return;
    }
    
    if(gauge->widget.drawState == NOT_STARTED)
    {
        nextState(gauge);
    }
    
    while(gauge->widget.drawState != DONE)
    {
        gauge->widget.drawFunc((leWidget*)gauge);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_CIRCULARGAUGE_WIDGET_ENABLED
