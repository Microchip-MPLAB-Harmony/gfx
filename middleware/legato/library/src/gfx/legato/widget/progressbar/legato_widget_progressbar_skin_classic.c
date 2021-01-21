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


#include "gfx/legato/widget/progressbar/legato_widget_progressbar.h"

#if LE_PROGRESSBAR_WIDGET_ENABLED == 1

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
    DRAW_FILL,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

void _leProgressBarWidget_GetBarRect(const leProgressBarWidget* bar,
                                     int32_t val1,
                                     int32_t val2,
                                     leRect* barRect)
{
    leRect rect;
    leRect fill = leRect_Zero;
    int32_t i;
    uint32_t p1, p2;
    
    bar->fn->rectToScreen(bar, &rect);
    
    if(val2 < val1)
    {
        i = val2;
        val2 = val1;
        val1 = i;
    }
    
    // fill bar
    if(bar->direction == LE_PROGRESSBAR_DIRECTION_RIGHT)
    {
        p1 = lePercentOf(rect.width, val1);
        p2 = lePercentOf(rect.width, val2);
        
        fill.x = rect.x + p1;
        fill.y = rect.y;
        fill.width = p2 - p1;
        fill.height = rect.height;
    }
    else if(bar->direction == LE_PROGRESSBAR_DIRECTION_UP)
    {
        p1 = lePercentOf(rect.height, val1);
        p2 = lePercentOf(rect.height, val2);
        
        fill.height = p2 - p1;
        fill.x = rect.x;
        fill.y = rect.y + rect.height - fill.height - p1;
        fill.width = rect.width;
    }
    else if(bar->direction == LE_PROGRESSBAR_DIRECTION_LEFT)
    {
        p1 = lePercentOf(rect.width, val1);
        p2 = lePercentOf(rect.width, val2);
        
        fill.width = p2 - p1;
        fill.x = rect.x + rect.width - fill.width - p1;
        fill.y = rect.y;
        fill.height = rect.height;
    }
    else if(bar->direction == LE_PROGRESSBAR_DIRECTION_DOWN)
    {
        p1 = lePercentOf(rect.height, val1);
        p2 = lePercentOf(rect.height, val2);
        
        fill.x = rect.x;
        fill.y = rect.y + p1;
        fill.width = rect.width;
        fill.height = p2 - p1;
    }
    
    *barRect = fill;
}

static void drawBackground(leProgressBarWidget* bar);
static void drawFill(leProgressBarWidget* bar);
static void drawBorder(leProgressBarWidget* bar);

static void nextState(leProgressBarWidget* bar)
{
    switch(bar->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(bar->fn->getCumulativeAlphaEnabled(bar) == LE_TRUE)
            {
                paintState.alpha = bar->fn->getCumulativeAlphaAmount(bar);
            }
#endif
            
            if(bar->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                bar->widget.status.drawState = DRAW_BACKGROUND;
                bar->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            if(bar->value > 0)
            {
                bar->widget.status.drawState = DRAW_FILL;
                bar->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawFill;
                
                return;
            }
        }
        // fall through
        case DRAW_FILL:
        {
            if(bar->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                bar->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                bar->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            bar->widget.status.drawState = DONE;
            bar->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leProgressBarWidget* bar)
{
    if(bar->widget.style.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        leWidget_SkinClassic_DrawBackground((leWidget*)bar,
                                            leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_BASE),
                                            paintState.alpha);
    }
    
    nextState(bar);
}

static void drawFill(leProgressBarWidget* bar)
{
    leRect barRect;

    _leProgressBarWidget_GetBarRect(bar, 0, bar->value, &barRect);
    
    leRenderer_RectFill(&barRect,
                        leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_BACKGROUND),
                        paintState.alpha);
             
    nextState(bar);
}

static void drawBorder(leProgressBarWidget* bar)
{
    if(bar->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)bar,
                                                    paintState.alpha);
    }
    else if(bar->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardLoweredBorder((leWidget*)bar,
                                                       paintState.alpha);
    }
    
    nextState(bar);
}

void _leProgressBarWidget_Paint(leProgressBarWidget* bar)
{
    if(bar->widget.status.drawState == NOT_STARTED)
    {
        nextState(bar);
    }
    
    while(bar->widget.status.drawState != DONE)
    {
        bar->widget.drawFunc((leWidget*)bar);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_PROGRESSBAR_WIDGET_ENABLED