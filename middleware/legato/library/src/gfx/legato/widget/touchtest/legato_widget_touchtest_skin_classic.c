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


#include <gfx/legato/legato.h>
#include "gfx/legato/widget/touchtest/legato_widget_touchtest.h"

#if LE_TOUCHTEST_WIDGET_ENABLED

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
    DRAW_LINES,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

/*
lePoint _leTouchTestWidget_GetLineRects(const leTouchTestWidget* tch,
                                        uint32_t idx)
{
    leRect widgetRect = tch->fn->localRect(tch);
    //lePoint pnt = tch->pnts[idx];

    //leUtils_PointToLayerSpace((leWidget*)tch,

    horzRect->x = widgetRect.x;
    horzRect->y = widgetRect.y + tch->pnts[idx].y;
    horzRect->width = widgetRect.width;
    horzRect->height = 1;

    vertRect->x = widgetRect.x + tch->pnts[idx].x;
    vertRect->y = widgetRect.y;
    vertRect->width = 1;
    vertRect->height = widgetRect.height;
}*/

static void drawBackground(leTouchTestWidget* tch);
static void drawLines(leTouchTestWidget* tch);
static void drawBorder(leTouchTestWidget* tch);

static void nextState(leTouchTestWidget* tch)
{
    switch(tch->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(tch->fn->getCumulativeAlphaEnabled(tch) == LE_TRUE)
            {
                paintState.alpha = tch->fn->getCumulativeAlphaAmount(tch);
            }
#endif

            if(tch->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                tch->widget.status.drawState = DRAW_BACKGROUND;
                tch->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            if(tch->size > 0)
            {
                tch->widget.status.drawState = DRAW_LINES;
                tch->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawLines;
                
                return;
            }
        }
        // fall through
        case DRAW_LINES:
        {
            if(tch->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                tch->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                tch->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            tch->widget.status.drawState = DONE;
            tch->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leTouchTestWidget* tch)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)tch, paintState.alpha);
    
    nextState(tch);
}

static void drawLines(leTouchTestWidget* tch)
{
    leRect widgetRect;
    lePoint touchPnt;
    uint32_t per;
    uint32_t i, j;
    leColor c1, c2, clr;
    
    tch->fn->rectToScreen(tch, &widgetRect);
    
    // draw lines
    j = tch->start;
    
    for(i = 0; i < tch->size; i++)
    {
        if(j >= tch->size)
        {
            j = 0;
        }

        per = lePercentWholeRounded(tch->pnts[j].y, widgetRect.height);
        
        c1 = leScheme_GetRenderColor(tch->widget.scheme, LE_SCHM_FOREGROUND);
        c2 = leScheme_GetRenderColor(tch->widget.scheme, LE_SCHM_FOREGROUND_INACTIVE);
        
        clr = leColorLerp(c1,
                          c2,
                          per,
                          leRenderer_CurrentColorMode());

        touchPnt = tch->pnts[j];

        leRenderer_HorzLine(widgetRect.x,
                            widgetRect.y + touchPnt.y,
                            widgetRect.width,
                            clr,
                            paintState.alpha);
        
        per = lePercentWholeRounded(tch->pnts[j].x, widgetRect.width);
            
        c1 = leScheme_GetRenderColor(tch->widget.scheme, LE_SCHM_BACKGROUND);
        c2 = leScheme_GetRenderColor(tch->widget.scheme, LE_SCHM_BACKGROUND_INACTIVE);
            
        clr = leColorLerp(c1,
                          c2,
                          per,
                          leRenderer_CurrentColorMode());

        leRenderer_VertLine(widgetRect.x + touchPnt.x,
                            widgetRect.y,
                            widgetRect.height,
                            clr,
                            paintState.alpha);
        
        j++;
    }

    nextState(tch);
}

static void drawBorder(leTouchTestWidget* tch)
{
    if(tch->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)tch, paintState.alpha);
    }
    else if(tch->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)tch, paintState.alpha);
    }
    
    nextState(tch);
}

void _leTouchTestWidget_Paint(leTouchTestWidget* tch)
{
    if(tch->widget.status.drawState == NOT_STARTED)
    {
        nextState(tch);
    }
    
    while(tch->widget.status.drawState != DONE)
    {
        tch->widget.drawFunc((leWidget*)tch);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_TOUCHTEST_WIDGET_ENABLED