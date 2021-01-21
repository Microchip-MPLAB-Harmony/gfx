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


#include "gfx/legato/widget/slider/legato_widget_slider.h"

#if LE_SLIDER_WIDGET_ENABLED

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
    DRAW_BAR,
    DRAW_HANDLE,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

void _leSliderWidget_GetSlideAreaRect(leSliderWidget* sld, leRect* rect)
{
    leRect scrollRect;
    leRect widgetRect;

    sld->fn->localRect(sld, &widgetRect);
    
    if(sld->alignment == LE_ORIENTATION_VERTICAL)
    {
        scrollRect.width = widgetRect.width / 4;
        scrollRect.height = widgetRect.height - sld->grip;
        scrollRect.x = widgetRect.x + (widgetRect.width / 2) - (scrollRect.width / 2);
        scrollRect.y = widgetRect.y + sld->grip / 2;
    }
    else
    {
        scrollRect.width = widgetRect.width - sld->grip;
        scrollRect.height = widgetRect.height / 4;
        scrollRect.x = widgetRect.x + sld->grip / 2;
        scrollRect.y = widgetRect.y + (widgetRect.height / 2) - (scrollRect.height / 2);
    }

    *rect = scrollRect;
}

void _leSliderWidget_GetHandleRect(leSliderWidget* sld, leRect* rect)
{
    leRect scrollRect;
    uint32_t val, max, percent;
    leRect widgetRect;

    sld->fn->localRect(sld, &widgetRect);
    
    val = sld->value - sld->min;
    max = sld->max - sld->min;
    
    percent = lePercentWholeRounded(val, max);
    
    _leSliderWidget_GetSlideAreaRect(sld, &scrollRect);
    
    if(sld->alignment == LE_ORIENTATION_VERTICAL)
    {
        percent = lePercentOf(scrollRect.height, percent);
        
        rect->x = widgetRect.x;
        rect->y = ((scrollRect.y + scrollRect.height) - percent) - (sld->grip / 2);
        rect->width = widgetRect.width;
        rect->height = sld->grip;
    }
    else
    {
        percent = lePercentOf(scrollRect.width, percent);
        
        rect->x = (scrollRect.x + percent) - (sld->grip / 2);
        rect->y = widgetRect.y;
        rect->width = sld->grip;
        rect->height = widgetRect.height;
    }
}

void _leSliderWidget_InvalidateBorderAreas(leSliderWidget* sld)
{
    sld->fn->invalidate(sld);
}

static void drawBackground(leSliderWidget* sld);
static void drawBar(leSliderWidget* sld);
static void drawHandle(leSliderWidget* sld);
//static void drawBorder(leSliderWidget* sld);

static void nextState(leSliderWidget* sld)
{
    switch(sld->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(sld->fn->getCumulativeAlphaEnabled(sld) == LE_TRUE)
            {
                paintState.alpha = sld->fn->getCumulativeAlphaAmount(sld);
            }
#endif
           
            if(sld->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                sld->widget.status.drawState = DRAW_BACKGROUND;
                sld->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            sld->widget.status.drawState = DRAW_BAR;
            sld->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBar;
            
            return;
        }
        case DRAW_BAR:
        {
            sld->widget.status.drawState = DRAW_HANDLE;
            sld->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawHandle;
            
            return;
        }
        case DRAW_HANDLE:
        {
            /*if(sld->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                sld->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                sld->widget.status.drawState = DRAW_BORDER;
                
                return;
            }*/
            
            sld->widget.status.drawState = DONE;
            sld->widget.drawFunc = NULL;
        }
        // fall through
        /*case DRAW_BORDER:
        {
            sld->widget.status.drawState = DONE;
            sld->widget.drawFunc = NULL;
        }*/
    }
}

static void drawBackground(leSliderWidget* sld)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)sld,
                                                paintState.alpha);

    nextState(sld);
}

static void drawBar(leSliderWidget* sld)
{
    leRect barRect;
    
    _leSliderWidget_GetSlideAreaRect(sld, &barRect);
    leUtils_RectToScreenSpace((leWidget*)sld, &barRect);

    // fill bar area
    leRenderer_RectFill(&barRect,
                        leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_BACKGROUND),
                        paintState.alpha);
                 
    // draw border
    if(sld->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawLineBorder(&barRect,
                                            leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_SHADOWDARK),
                                            paintState.alpha);
    }
    else if(sld->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_Draw2x2BeveledBorder(&barRect,
                                                  leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_SHADOW),
                                                  leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_SHADOWDARK),
                                                  leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                                  leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                                  paintState.alpha);
    }
    
    nextState(sld);
}

static void drawHandle(leSliderWidget* sld)
{
    leRect handleRect;
    
    _leSliderWidget_GetHandleRect(sld, &handleRect);
    leUtils_RectToScreenSpace((leWidget*)sld, &handleRect);

    // fill handle area
    leRenderer_RectFill(&handleRect,
                        leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_BASE),
                        paintState.alpha);
    
    // draw handle border
    if(sld->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawLineBorder(&handleRect,
                                            leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_SHADOWDARK),
                                            paintState.alpha);
    }
    else if(sld->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_Draw1x2BeveledBorder(&handleRect,
                                                  leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                                  leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_SHADOWDARK),
                                                  leScheme_GetRenderColor(sld->widget.scheme, LE_SCHM_SHADOW),
                                                  paintState.alpha);
    }
    
    nextState(sld);
}

/*static void drawBorder(leSliderWidget* sld)
{
    leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)sld);
    
    nextState(sld);
}*/

void _leSliderWidget_Paint(leSliderWidget* sld)
{
    if(sld->widget.status.drawState == NOT_STARTED)
    {
        nextState(sld);
    }
    
    while(sld->widget.status.drawState != DONE)
    {
        sld->widget.drawFunc((leWidget*)sld);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_SLIDER_WIDGET_ENABLED