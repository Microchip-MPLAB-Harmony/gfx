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


#include "gfx/legato/widget/circularslider/legato_widget_circular_slider.h"

#if LE_CIRCULARSLIDER_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_CIRCULAR_SLIDER,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

static void drawBackground(leCircularSliderWidget* slider);
static void drawCircularSlider(leCircularSliderWidget* slider);
static void drawBorder(leCircularSliderWidget* slider);

lePoint _leCircularSliderWidget_GetCircleCenterPointAtValue(leCircularSliderWidget* _this, int32_t value);

static void nextState(leCircularSliderWidget* slider)
{
    switch(slider->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(slider->fn->getCumulativeAlphaEnabled(slider) == LE_TRUE)
            {
                paintState.alpha = slider->fn->getCumulativeAlphaAmount(slider);
            }
#endif
            
            if(slider->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                slider->widget.status.drawState = DRAW_BACKGROUND;
                slider->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            slider->widget.status.drawState = DRAW_CIRCULAR_SLIDER;
            slider->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawCircularSlider;
    
            return;
        }
        case DRAW_CIRCULAR_SLIDER:
        {            
            if(slider->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                slider->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                slider->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            slider->widget.status.drawState = DONE;
            slider->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leCircularSliderWidget* slider)
{
    if(slider->widget.style.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        leWidget_SkinClassic_DrawBackground((leWidget*) slider, 
                                             leScheme_GetRenderColor(slider->widget.scheme, LE_SCHM_BACKGROUND),
                                             paintState.alpha);
    }
    
    nextState(slider);
}

static void drawCircularSlider(leCircularSliderWidget* slider)
{
    lePoint p;
    leRect sliderRect, arcRect;

    p.x = slider->widget.rect.width / 2;
    p.y = slider->widget.rect.height / 2;
    
    sliderRect.x = p.x - slider->radius;
    sliderRect.y = p.y - slider->radius;
    sliderRect.width = slider->radius * 2;
    sliderRect.height = sliderRect.width;

    leUtils_RectToScreenSpace((leWidget*)slider, &sliderRect);

    //Draw inactive arc
    if (slider->inActiveArc.visible == LE_TRUE)
    {
        leRenderer_CircleDraw(&sliderRect,
                              slider->inActiveArc.thickness,
                              leScheme_GetRenderColor(slider->widget.scheme, LE_SCHM_FOREGROUND_INACTIVE),
                              paintState.alpha);
    }

    // draw active arc
    if (slider->activeArc.visible == LE_TRUE)
    {
        leRenderer_ArcFill(&sliderRect,
                           slider->startAngle,
                           slider->spanAngle,
                           slider->activeArc.thickness,
                           slider->roundEdges,
                           leScheme_GetRenderColor(slider->widget.scheme, LE_SCHM_FOREGROUND),
                           LE_FALSE,
                           paintState.alpha);
    }

    //Draw outer arc
    if (slider->outsideBorderArc.visible == LE_TRUE)
    {
        arcRect = sliderRect;
        arcRect.x -= slider->activeArc.thickness / 2;
        arcRect.width += slider->activeArc.thickness;
        arcRect.y -= slider->activeArc.thickness / 2;
        arcRect.height += slider->activeArc.thickness;

        leRenderer_CircleDraw(&arcRect,
                              slider->outsideBorderArc.thickness,
                              leScheme_GetRenderColor(slider->widget.scheme, LE_SCHM_BASE),
                              paintState.alpha);
    }

    // draw inner arc
    if (slider->insideBorderArc.visible == LE_TRUE)
    {
        arcRect = sliderRect;
        arcRect.x += slider->activeArc.thickness / 2;
        arcRect.width -= slider->activeArc.thickness;
        arcRect.y += slider->activeArc.thickness / 2;
        arcRect.height -= slider->activeArc.thickness;

        leRenderer_CircleDraw(&arcRect,
                              slider->insideBorderArc.thickness,
                              leScheme_GetRenderColor(slider->widget.scheme, LE_SCHM_BASE),
                              paintState.alpha);
    }

    // draw circular button
    if (slider->circleButtonArc.visible == LE_TRUE)
    {
        lePoint btnPnt;

        uint32_t deg = leDegreesFromPercent(slider->value,
                                            slider->spanAngle,
                                            slider->startAngle);

        lePointOnCircle(slider->radius,
                        deg,
                        &btnPnt);

        btnPnt.y *= -1;

        arcRect.x = p.x + btnPnt.x - slider->circleButtonArc.radius;
        arcRect.y = p.y + btnPnt.y - slider->circleButtonArc.radius;
        arcRect.width = slider->circleButtonArc.radius * 2;
        arcRect.height = arcRect.width;

        leUtils_RectToScreenSpace((leWidget*)slider, &arcRect);

        leRenderer_CircleFill(&arcRect,
                              slider->circleButtonArc.thickness,
                              leScheme_GetRenderColor(slider->widget.scheme, LE_SCHM_BASE),
                              leScheme_GetRenderColor(slider->circleButtonArc.scheme, LE_SCHM_FOREGROUND),
                              paintState.alpha);
    }

    nextState(slider);
}

static void drawBorder(leCircularSliderWidget* slider)
{    
    if(slider->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)slider,
                                                    paintState.alpha);
    }
    else if(slider->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)slider,
                                                      paintState.alpha);
    }
    
    nextState(slider);
}

void _leCircularSliderWidget_Paint(leCircularSliderWidget* slider)
{
    if(slider->widget.status.drawState == NOT_STARTED)
        nextState(slider);
    
    while(slider->widget.status.drawState != DONE)
    {
        slider->widget.drawFunc((leWidget*)slider);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_CIRCULARSLIDER_WIDGET_ENABLED
