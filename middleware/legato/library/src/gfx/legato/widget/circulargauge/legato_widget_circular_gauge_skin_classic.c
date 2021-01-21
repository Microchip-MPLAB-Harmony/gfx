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
#include <string.h>
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
    leRect gaugeRect;
    lePoint gaugeOrigin;
    int32_t radius;
    uint32_t alpha;

    leChar strbuff[MAX_TICK_LABEL_DIGITS];
} paintState;

static void drawBackground(leCircularGaugeWidget* gauge);
static void drawCircularGauge(leCircularGaugeWidget* gauge);
static void drawBorder(leCircularGaugeWidget* gauge);

static void nextState(leCircularGaugeWidget* gauge)
{
    switch(gauge->widget.status.drawState)
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
            
            if(gauge->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                gauge->widget.status.drawState = DRAW_BACKGROUND;
                gauge->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            gauge->widget.status.drawState = DRAW_CIRCULAR_GAUGE;
            gauge->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawCircularGauge;
    
            return;
        }
        case DRAW_CIRCULAR_GAUGE:
        {            
            if(gauge->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                gauge->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                gauge->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            gauge->widget.status.drawState = DONE;
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

static void paintArc(const leCircularGaugeWidget* gauge,
                     const leCircularGaugeArc* arc)
{
    leRect rect;

    int32_t startAngle = leDegreesFromPercent(arc->startAngle, gauge->centerAngle, gauge->startAngle);
    int32_t endAngle = leDegreesFromPercent(arc->endAngle, gauge->centerAngle, gauge->startAngle);

    leColor clr = leScheme_GetRenderColor(arc->scheme,
                                          LE_SCHM_FOREGROUND);

    rect.x = paintState.gaugeOrigin.x - (gauge->radius + arc->offset);
    rect.y = paintState.gaugeOrigin.y - (gauge->radius + arc->offset);
    rect.width = (gauge->radius + arc->offset) * 2;
    rect.height = (gauge->radius + arc->offset) * 2;

    leRenderer_ArcFill(&rect,
                       startAngle,
                       endAngle - startAngle,
                       arc->thickness,
                       LE_FALSE,
                       clr,
                       LE_FALSE,
                       paintState.alpha);
}

void _paintTicksAtAngle(int offset,
                        int length,
                        int angle,
                        leColor clr)
{
    lePoint start, end;

    lePolarToXY(paintState.radius - (length / 2) + offset, angle, &start);
    lePolarToXY(paintState.radius + (length / 2) + offset, angle, &end);

    leRenderer_DrawLine(paintState.gaugeOrigin.x + start.x,
                        paintState.gaugeOrigin.y - start.y,
                        paintState.gaugeOrigin.x + end.x,
                        paintState.gaugeOrigin.y - end.y,
                        clr,
                        paintState.alpha);
}

static void _paintTickRange(const leCircularGaugeWidget* gauge,
                            const leCircularGaugeTickRange* rng)
{
    leColor clr = leScheme_GetRenderColor(rng->scheme, LE_SCHM_FOREGROUND);

    int32_t startAngle = leDegreesFromPercent(rng->startAngle, gauge->centerAngle, gauge->startAngle);
    int32_t endAngle = leDegreesFromPercent(rng->endAngle, gauge->centerAngle, gauge->startAngle);
    int32_t t;

    if(endAngle < startAngle)
    {
        t = endAngle;
        endAngle = startAngle;
        startAngle = t;
    }

    if(rng->thickness == 0)
        return;

    if(rng->divisions == 0)
    {
        //draw tick at start
        _paintTicksAtAngle(rng->offset,
                           rng->thickness,
                           startAngle,
                           clr);

        //draw tick at end
        _paintTicksAtAngle(rng->offset,
                           rng->thickness,
                           endAngle,
                           clr);
    }
    else
    {
        int32_t increment = (rng->endAngle - rng->startAngle) / rng->divisions;
        uint32_t idx;

        //draw start and middle ticks
        for(idx = 0; idx < rng->divisions; ++idx)
        {
            _paintTicksAtAngle(rng->offset,
                               rng->thickness,
                               leDegreesFromPercent(rng->startAngle + (idx * increment), gauge->centerAngle, gauge->startAngle),
                               clr);
        }

        //draw tick at end
        _paintTicksAtAngle(rng->offset,
                           rng->thickness,
                           endAngle,
                           clr);
    }
}


static void _paintLabelValueAtAngle(int32_t angle,
                                    int offset,
                                    int value,
                                    leColor clr,
                                    const leFont* fnt)
{
    char strbuff[MAX_TICK_LABEL_DIGITS];
    lePoint lblPoint;

    lePolarToXY(paintState.radius + offset, angle, &lblPoint);

    sprintf(strbuff, "%d", value);

    leFixedString_Constructor(&paintState.labelStr, paintState.strbuff, MAX_TICK_LABEL_DIGITS);
    paintState.labelStr.fn->setFromCStr(&paintState.labelStr, strbuff);
    paintState.labelStr.fn->setFont(&paintState.labelStr, fnt);

    leRect stringRect;

    paintState.labelStr.fn->getRect(&paintState.labelStr, &stringRect);

    stringRect.x = paintState.gaugeOrigin.x + lblPoint.x - stringRect.width / 2;
    stringRect.y = paintState.gaugeOrigin.y + (-lblPoint.y) - stringRect.height / 2;

    paintState.labelStr.fn->_draw(&paintState.labelStr,
                                  stringRect.x,
                                  stringRect.y,
                                  LE_HALIGN_CENTER,
                                  clr,
                                  paintState.alpha);
}

static void _paintLabelRange(const leCircularGaugeWidget* gauge,
                             const leCircularGaugeLabelRange* rng)
{
    leColor clr;

    if(rng->font == NULL)
        return;

    clr = leScheme_GetRenderColor(rng->scheme, LE_SCHM_TEXT);

    if(rng->divisions == 0)
    {
        // draw label at start
        _paintLabelValueAtAngle(leDegreesFromPercent(rng->startAngle, gauge->centerAngle, gauge->startAngle),
                                rng->offset,
                                0,
                                clr,
                                rng->font);

        // draw label at end
        _paintLabelValueAtAngle(leDegreesFromPercent(rng->endAngle, gauge->centerAngle, gauge->startAngle),
                                rng->offset,
                                100,
                                clr,
                                rng->font);
    }
    else
    {
        if(gauge->centerAngle < 0)
        {
            int32_t startAngle = leDegreesFromPercent(rng->startAngle, gauge->centerAngle, gauge->startAngle);
            int32_t endAngle = leDegreesFromPercent(rng->endAngle, gauge->centerAngle, gauge->startAngle);
            int32_t increment = -((startAngle - endAngle) / rng->divisions);
            uint32_t idx;

            // draw start and middle ticks
            for(idx = 0; idx < rng->divisions; ++idx)
            {
                int32_t angle = startAngle + (idx * increment);

                uint32_t deltaVal = lePercentWholeRounded(idx, rng->divisions);

                _paintLabelValueAtAngle(angle,
                                        rng->offset,
                                        deltaVal,
                                        clr,
                                        rng->font);
            }

            // draw label at end
            _paintLabelValueAtAngle(leDegreesFromPercent(rng->endAngle, gauge->centerAngle, gauge->startAngle),
                                    rng->offset,
                                    100,
                                    clr,
                                    rng->font);
        }
        else
        {
            int32_t startAngle = leDegreesFromPercent(rng->startAngle, gauge->centerAngle, gauge->startAngle);
            int32_t endAngle = leDegreesFromPercent(rng->endAngle, gauge->centerAngle, gauge->startAngle);
            int32_t increment = (endAngle - startAngle) / rng->divisions;
            uint32_t idx;

            // draw start and middle ticks
            for(idx = 0; idx < rng->divisions; ++idx)
            {
                int32_t angle = startAngle + (idx * increment);

                uint32_t deltaVal = lePercentWholeRounded(idx, rng->divisions);

                _paintLabelValueAtAngle(angle,
                                        rng->offset,
                                        deltaVal,
                                        clr,
                                        rng->font);
            }

            // draw label at end
            _paintLabelValueAtAngle(leDegreesFromPercent(rng->endAngle, gauge->centerAngle, gauge->startAngle),
                                    rng->offset,
                                    100,
                                    clr,
                                    rng->font);
        }
    }
}

static void paintCenterCircle(const leCircularGaugeWidget* gauge)
{
    leRect circleRect;

    if(gauge->centerCircleVisible == LE_FALSE)
        return;

    uint32_t thickness = gauge->centerCircleThickness;

    if(thickness > gauge->centerCircleRadius)
    {
        thickness = gauge->centerCircleRadius;
    }

    uint32_t circleRadius = lePercentOf(gauge->radius, gauge->centerCircleRadius);

    circleRect.x = paintState.gaugeOrigin.x - circleRadius;
    circleRect.y = paintState.gaugeOrigin.y - circleRadius;
    circleRect.width = circleRadius * 2;
    circleRect.height = circleRadius * 2;

    leRenderer_CircleDraw(&circleRect,
                          thickness,
                          leScheme_GetRenderColor(gauge->widget.scheme, LE_SCHM_FOREGROUND),
                          paintState.alpha);
}

static void paintHand(const leCircularGaugeWidget* gauge)
{
    lePoint startPt, endPt;
    int32_t handLength;
    int32_t angle;

    if(gauge->handVisible == LE_FALSE)
        return;

    handLength = lePercentOf(gauge->radius, gauge->handRadius);
    angle = leDegreesFromPercent(gauge->value, gauge->centerAngle, gauge->startAngle);

    if(gauge->centerCircleVisible == LE_FALSE)
    {
        startPt.x = 0;
        startPt.y = 0;
    }
    else
    {
        lePolarToXY(gauge->centerCircleRadius - 2, angle, &startPt);
        lePolarToXY(handLength, angle, &endPt);
    }

    leRenderer_DrawLine(paintState.gaugeOrigin.x + startPt.x,
                        paintState.gaugeOrigin.y - startPt.y,
                        paintState.gaugeOrigin.x + endPt.x,
                        paintState.gaugeOrigin.y - endPt.y,
                        leScheme_GetRenderColor(gauge->fn->getScheme(gauge), LE_SCHM_FOREGROUND),
                        paintState.alpha);
}


static void drawCircularGauge(leCircularGaugeWidget* gauge)
{
    uint32_t itr;
    leCircularGaugeArc* arc;
    leCircularGaugeTickRange* tickRange;
    leCircularGaugeLabelRange* labelRange;

    paintState.radius = gauge->radius;

    paintState.gaugeOrigin.x = gauge->fn->getWidth(gauge)/2;
    paintState.gaugeOrigin.y = gauge->fn->getHeight(gauge)/2;
    
    leUtils_PointToScreenSpace((leWidget*)gauge, &paintState.gaugeOrigin);

    paintState.gaugeRect.x = paintState.gaugeOrigin.x - gauge->radius;
    paintState.gaugeRect.y = paintState.gaugeOrigin.y - gauge->radius;
    paintState.gaugeRect.width = gauge->radius * 2;
    paintState.gaugeRect.height = gauge->radius * 2;

    // draw the arcs
    for(itr = 0; itr < gauge->arcsArray.size; ++itr)
    {
        arc = (leCircularGaugeArc*)leArray_Get(&gauge->arcsArray, itr);

        if(arc->enabled == LE_FALSE)
            continue;

        paintArc(gauge, arc);
    }

    // draw the tick ranges
    for(itr = 0; itr < gauge->ticksArray.size; ++itr)
    {
        tickRange = (leCircularGaugeTickRange*)leArray_Get(&gauge->ticksArray, itr);

        if(tickRange->enabled == LE_FALSE)
            continue;

        _paintTickRange(gauge, tickRange);
    }

    // draw the labels
    for(itr = 0; itr < gauge->labelsArray.size; ++itr)
    {
        labelRange = (leCircularGaugeLabelRange*)leArray_Get(&gauge->labelsArray, itr);

        if(labelRange->enabled == LE_FALSE)
            continue;

        _paintLabelRange(gauge, labelRange);
    }

    // draw the hand
    paintHand(gauge);

    // draw center circle
    paintCenterCircle(gauge);

    //leRenderer_RectLine(&paintState.gaugeRect,
    //                    leColorValue(LE_COLOR_RED, leRenderer_CurrentColorMode()),
    //                    255);

    nextState(gauge);
}

static void drawBorder(leCircularGaugeWidget* gauge)
{    
    if(gauge->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)gauge,
                                                    paintState.alpha);
    }
    else if(gauge->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)gauge,
                                                      paintState.alpha);
    }
    
    nextState(gauge);
}

void _leCircularGaugeWidget_Paint(leCircularGaugeWidget* gauge)
{    
    if(gauge->widget.status.drawState == NOT_STARTED)
    {
        nextState(gauge);
    }
    
    while(gauge->widget.status.drawState != DONE)
    {
        gauge->widget.drawFunc((leWidget*)gauge);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_CIRCULARGAUGE_WIDGET_ENABLED
