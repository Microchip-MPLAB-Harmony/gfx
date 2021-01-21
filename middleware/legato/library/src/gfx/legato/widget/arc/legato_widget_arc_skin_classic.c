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


#include "gfx/legato/widget/arc/legato_widget_arc.h"

#if LE_ARC_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_math.h"

#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_ARC,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

static void drawBackground(leArcWidget* arc);
static void drawArc(leArcWidget* arc);
static void drawBorder(leArcWidget* arc);

static void nextState(leArcWidget* arc)
{
    switch(arc->widget.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(arc->fn->getCumulativeAlphaEnabled(arc) == LE_TRUE)
            {
                paintState.alpha = arc->fn->getCumulativeAlphaAmount(arc);
            }
#endif
            
            if(arc->widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                arc->widget.drawState = DRAW_BACKGROUND;
                arc->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            arc->widget.drawState = DRAW_ARC;
            arc->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawArc;
    
            return;
        }
        case DRAW_ARC:
        {            
            if(arc->widget.borderType != LE_WIDGET_BORDER_NONE)
            {
                arc->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                arc->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            arc->widget.drawState = DONE;
            arc->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leArcWidget* arc)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)arc, paintState.alpha);
    
    nextState(arc);
}

//Returns the relative position of the midpoint in the arc
static lePoint _leArcWidget_GetMidPointAtAngle(leArcWidget* arc, int32_t angle)
{
    lePoint point, arcCenterPoint;
    
    while (angle < 0)
    {
        angle += 360;
    }

    while (angle > 360)
    {
        angle -= 360;
    }
    
    arcCenterPoint.x = arc->widget.rect.width/2;
    arcCenterPoint.y = arc->widget.rect.height/2;
    
    leUtils_PointToScreenSpace((leWidget*)arc, &arcCenterPoint);
    
    lePolarToXY(arc->radius - (arc->thickness/2), angle, &point);

    point.x = arcCenterPoint.x + point.x;
    point.y = arcCenterPoint.y - point.y;
    
    return point;
}

static void drawArc(leArcWidget* arc)
{
    lePoint p;
    leRect arcRect;

    p.x = 0;
    p.y = 0;

    leUtils_PointToScreenSpace((leWidget*)arc, &p);

    arcRect.x = p.x;
    arcRect.y = p.y;
    arcRect.width = arc->radius * 2;
    arcRect.height = arc->radius * 2;

    p.x = arc->widget.rect.width / 2;
    p.y = arc->widget.rect.height / 2;

    leRenderer_ArcFill(&arcRect,
                       p.x,
                       p.y,
                       arc->radius,
                       arc->startAngle,
                       arc->centerAngle,
                       arc->thickness,
                       arc->widget.scheme->foreground,
                       LE_FALSE,
                       paintState.alpha);
    
    //Draw the edges
    if (arc->roundEdge == LE_TRUE)
    {
        lePoint point = _leArcWidget_GetMidPointAtAngle(arc, arc->startAngle);
        
        leRenderer_ArcFill(&arcRect,
                           point.x,
                           point.y,
                           arc->thickness / 2,
                           0,
                           360,
                           arc->thickness / 2,
                           arc->widget.scheme->foreground,
                           LE_FALSE,
                           paintState.alpha);
        
        point = _leArcWidget_GetMidPointAtAngle(arc,
                                                arc->startAngle + arc->centerAngle);
        
        leRenderer_ArcFill(&arcRect,
                           point.x,
                           point.y,
                           arc->thickness / 2,
                           0,
                           360,
                           arc->thickness / 2,
                           arc->widget.scheme->foreground,
                           LE_FALSE,
                           paintState.alpha);
    }

    nextState(arc);
}

static void drawBorder(leArcWidget* arc)
{    
    if(arc->widget.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget *) arc,
                                                    paintState.alpha);
    }
    else if(arc->widget.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget *) arc,
                                                      paintState.alpha);
    }

    nextState(arc);
}

void _leArcWidget_Paint(leArcWidget* arc)
{
    if(arc->widget.scheme == NULL)
    {
        arc->widget.drawState = DONE;
        
        return;
    }
    
    if(arc->widget.drawState == NOT_STARTED)
    {
        nextState(arc);
    }

    while(arc->widget.drawState != DONE)
    {
        arc->widget.drawFunc((leWidget*)arc);
        
#if LE_PREEMPTION_LEVEL >= 2
        break;
#endif
    }
}

#endif // LE_ARC_WIDGET_ENABLED
