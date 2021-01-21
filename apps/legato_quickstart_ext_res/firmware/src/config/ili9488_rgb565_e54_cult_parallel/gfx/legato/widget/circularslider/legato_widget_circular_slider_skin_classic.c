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

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/common/legato_utils.h"
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
    switch(slider->widget.drawState)
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
            
            if(slider->widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                slider->widget.drawState = DRAW_BACKGROUND;
                slider->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            slider->widget.drawState = DRAW_CIRCULAR_SLIDER;
            slider->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawCircularSlider;
    
            return;
        }
        case DRAW_CIRCULAR_SLIDER:
        {            
            if(slider->widget.borderType != LE_WIDGET_BORDER_NONE)
            {
                slider->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                slider->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            slider->widget.drawState = DONE;
            slider->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leCircularSliderWidget* slider)
{
    if(slider->widget.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        leWidget_SkinClassic_DrawBackground((leWidget*) slider, 
                                             slider->widget.scheme->background,
                                             paintState.alpha);
    }
    
    nextState(slider);
}

static void drawCircularSlider(leCircularSliderWidget* slider)
{
    lePoint p;
    leRect sliderRect;
    int32_t valueCenterAngle;
    
    p.x = slider->widget.rect.width/2;
    p.y = slider->widget.rect.height/2;
    
    sliderRect.x = 0;
    sliderRect.y = 0;
    sliderRect.width = slider->widget.rect.width;
    sliderRect.height = slider->widget.rect.height;

    valueCenterAngle = (int32_t) ((slider->value - slider->startValue) * slider->degPerUnit);
    
    leUtils_RectToScreenSpace((leWidget*)slider, &sliderRect);
    
    /*GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_LINE);
    GFX_Set(GFXF_DRAW_COLOR, slider->widget.scheme->foreground);

    GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
            slider->widget.scheme->foreground & 0xffffff00, 
            slider->widget.scheme->foreground, 
            NULL, 
            NULL);*/

    //Draw inactive arc
    if (slider->inActiveArc.visible == LE_TRUE)
    {
        /*GFX_Set(GFXF_DRAW_COLOR, slider->widget.scheme->foregroundInactive);
        GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
                slider->widget.scheme->foregroundInactive & 0xffffff00, 
                slider->widget.scheme->foregroundInactive, 
                NULL, 
                NULL);*/

        if (slider->direction == LE_COUNTER_CLOCKWISE)
        {
            //GFX_Set(GFXF_DRAW_THICKNESS, slider->inActiveArc.thickness);
            
            leRenderer_ArcFill(&sliderRect,
                               p.x, 
                               p.y, 
                               slider->radius - slider->outsideBorderArc.thickness, 
                               (slider->startAngle + valueCenterAngle), 
                               360 - valueCenterAngle,
                               slider->inActiveArc.thickness,
                               slider->widget.scheme->foregroundInactive,
                               LE_FALSE,
                               paintState.alpha);
        }
        else
        {
            int32_t start, center;
            start = slider->startAngle;
            center = 360 - valueCenterAngle;

            //GFX_Set(GFXF_DRAW_THICKNESS, slider->inActiveArc.thickness);
            
            leRenderer_ArcFill(&sliderRect,
                               p.x, 
                               p.y, 
                               slider->radius - slider->outsideBorderArc.thickness,
                               start, 
                               center,
                               slider->inActiveArc.thickness,
                               slider->widget.scheme->foregroundInactive,
                               LE_FALSE,
                               paintState.alpha);
        }
    }
    
    //Draw active arc
    if (slider->activeArc.visible == LE_TRUE)
    {
        /*GFX_Set(GFXF_DRAW_COLOR, slider->widget.scheme->foreground);
        GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
                slider->widget.scheme->foreground & 0xffffff00, 
                slider->widget.scheme->foreground, 
                NULL, 
                NULL);*/

        if (slider->direction == LE_COUNTER_CLOCKWISE)
        {
            //GFX_Set(GFXF_DRAW_THICKNESS, slider->activeArc.thickness);
                        
            leRenderer_ArcFill(&sliderRect,
                               p.x, 
                               p.y, 
                               slider->radius - slider->outsideBorderArc.thickness, 
                               slider->startAngle, 
                               valueCenterAngle,
                               slider->activeArc.thickness,
                               slider->widget.scheme->foreground,
                               LE_FALSE,
                               paintState.alpha);
        }
        else
        {
            //GFX_Set(GFXF_DRAW_THICKNESS, slider->activeArc.thickness);
                        
            leRenderer_ArcFill(&sliderRect,
                               p.x, 
                               p.y, 
                               slider->radius - slider->outsideBorderArc.thickness, 
                               slider->startAngle, 
                               -valueCenterAngle,
                               slider->activeArc.thickness,
                               slider->widget.scheme->foreground,
                               LE_FALSE,
                               paintState.alpha);
        }

        //Draw round edges
        if (slider->roundEdges == LE_TRUE)
        {
            //Draw start round edge
            lePoint center = _leCircularSliderWidget_GetCircleCenterPointAtValue(slider, slider->startValue);

            //Draw a full circle for now
            /*GFX_Set(GFXF_DRAW_COLOR, slider->widget.scheme->foreground);
            GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
                    slider->widget.scheme->foreground & 0xffffff00, 
                    slider->widget.scheme->foreground, 
                    NULL, NULL);
            GFX_Set(GFXF_DRAW_THICKNESS, slider->activeArc.thickness/2);
            GFX_DrawArc(center.x,
                        center.y,
                        slider->activeArc.thickness/2,
                        0,
                        360);*/
                        
            leRenderer_ArcFill(&sliderRect,
                               center.x - sliderRect.x,
                               center.y - sliderRect.y,
                               slider->activeArc.thickness / 2,
                               0,
                               360,
                               slider->activeArc.thickness / 2,
                               slider->widget.scheme->foreground,
                               LE_FALSE,
                               paintState.alpha);
            
            //Draw end round edge if circle button is not visible
            if (slider->circleButtonArc.visible != LE_TRUE)
            {
                //Draw end round edge
                lePoint center = _leCircularSliderWidget_GetCircleCenterPointAtValue(slider, slider->value);

                //Draw a full circle for now
                /*GFX_Set(GFXF_DRAW_COLOR, slider->widget.scheme->foreground);
                GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
                        slider->widget.scheme->foreground & 0xffffff00, 
                        slider->widget.scheme->foreground, 
                        NULL, NULL);
                GFX_Set(GFXF_DRAW_THICKNESS, slider->activeArc.thickness/2);
                GFX_DrawArc(center.x,
                            center.y,
                            slider->activeArc.thickness/2,
                            0,
                            360);*/  
                            
                leRenderer_ArcFill(&sliderRect,
                                   center.x - sliderRect.x,
                                   center.y - sliderRect.y,
                                   slider->activeArc.thickness/2,
                                   0,
                                   360,
                                   slider->activeArc.thickness / 2,
                                   slider->widget.scheme->foreground,
                                   LE_FALSE,
                                   paintState.alpha);
            }
        }
    }

    //Draw outer arc
    if (slider->outsideBorderArc.visible == LE_TRUE)
    {
        /*GFX_Set(GFXF_DRAW_COLOR, slider->widget.scheme->base);
        GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
                slider->widget.scheme->base& 0xffffff00, 
                slider->widget.scheme->base, 
                NULL, 
                NULL);
        GFX_Set(GFXF_DRAW_THICKNESS, slider->outsideBorderArc.thickness);
        GFX_DrawArc(p.x, p.y, 
                    slider->radius,
                    0,
                    360);*/
                    
        leRenderer_ArcFill(&sliderRect,
                           p.x - sliderRect.x,
                           p.y - sliderRect.y,
                           slider->radius,
                           0,
                           360,
                           slider->outsideBorderArc.thickness,
                           slider->widget.scheme->base,
                           LE_FALSE,
                           paintState.alpha);   
    }
    
    //Draw inner arc
    if (slider->insideBorderArc.visible == LE_TRUE)
    {
        /*GFX_Set(GFXF_DRAW_COLOR, slider->widget.scheme->base);
        GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
                slider->widget.scheme->base& 0xffffff00, 
                slider->widget.scheme->base, 
                NULL, NULL);
        GFX_Set(GFXF_DRAW_THICKNESS, slider->insideBorderArc.thickness);
        GFX_DrawArc(p.x, 
                    p.y, 
                    slider->radius - slider->outsideBorderArc.thickness - slider->activeArc.thickness, 
                    0, 
                    360);*/
                    
        leRenderer_ArcFill(&sliderRect,
                           p.x - sliderRect.x,
                           p.y - sliderRect.y,
                           slider->radius - slider->outsideBorderArc.thickness - slider->activeArc.thickness, 
                           0, 
                           360,
                           slider->insideBorderArc.thickness,
                           slider->widget.scheme->base,
                           LE_FALSE,
                           paintState.alpha); 
    }

    //Draw circular button
    if (slider->circleButtonArc.visible == LE_TRUE)
    {
        lePoint center = _leCircularSliderWidget_GetCircleCenterPointAtValue(slider, slider->value);

        //Draw outside border
        /*GFX_Set(GFXF_DRAW_COLOR, slider->circleButtonArc.scheme->base);
        GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
                slider->circleButtonArc.scheme->base & 0xffffff00, 
                slider->circleButtonArc.scheme->base, 
                NULL, NULL);
        GFX_Set(GFXF_DRAW_THICKNESS, slider->circleButtonArc.thickness);
        GFX_DrawArc(center.x,
                    center.y,
                    slider->circleButtonArc.radius,
                    0,
                    360);*/
                    
        leRenderer_ArcFill(&sliderRect,
                           center.x - sliderRect.x,
                           center.y - sliderRect.y,
                           slider->circleButtonArc.radius,
                           0,
                           360,
                           slider->circleButtonArc.thickness,
                           slider->circleButtonArc.scheme->base,
                           LE_FALSE,
                           paintState.alpha);

        if (slider->circleButtonArc.radius != slider->circleButtonArc.thickness)
        {
            //Draw circle fill
            /*GFX_Set(GFXF_DRAW_COLOR, slider->circleButtonArc.scheme->foreground);
            GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
                    slider->circleButtonArc.scheme->foreground & 0xffffff00, 
                    slider->circleButtonArc.scheme->foreground,
                    NULL, NULL);
            GFX_Set(GFXF_DRAW_THICKNESS, slider->circleButtonArc.radius - slider->circleButtonArc.thickness);
            GFX_DrawArc(center.x, 
                        center.y, 
                        slider->circleButtonArc.radius - slider->circleButtonArc.thickness, 
                        0,
                        360);*/
                        
            leRenderer_ArcFill(&sliderRect,
                               center.x - sliderRect.x,
                               center.y - sliderRect.y,
                               slider->circleButtonArc.radius - slider->circleButtonArc.thickness, 
                               0,
                               360,
                               slider->circleButtonArc.radius - slider->circleButtonArc.thickness,
                               slider->circleButtonArc.scheme->foreground,
                               LE_FALSE,
                               paintState.alpha);
        }
    }

    nextState(slider);
}

static void drawBorder(leCircularSliderWidget* slider)
{    
    if(slider->widget.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)slider,
                                                    paintState.alpha);
    }
    else if(slider->widget.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)slider,
                                                      paintState.alpha);
    }
    
    nextState(slider);
}

void _leCircularSliderWidget_Paint(leCircularSliderWidget* slider)
{
    if(slider->widget.scheme == NULL)
    {
        slider->widget.drawState = DONE;
        
        return;
    }
    
    if(slider->widget.drawState == NOT_STARTED)
        nextState(slider);
    
    while(slider->widget.drawState != DONE)
    {
        slider->widget.drawFunc((leWidget*)slider);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_CIRCULARSLIDER_WIDGET_ENABLED
