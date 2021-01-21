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


#include "gfx/legato/widget/gradient/legato_widget_gradient.h"

#if LE_GRADIENT_WIDGET_ENABLED

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
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

static void drawBackground(leGradientWidget* grad);
static void drawBorder(leGradientWidget* grad);

static void nextState(leGradientWidget* grad)
{
    switch(grad->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(grad->fn->getCumulativeAlphaEnabled(grad) == LE_TRUE)
            {
                paintState.alpha = grad->fn->getCumulativeAlphaAmount(grad);
            }
#endif
            
            if(grad->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                grad->widget.status.drawState = DRAW_BACKGROUND;
                grad->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            if(grad->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                grad->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                grad->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            grad->widget.status.drawState = DONE;
            grad->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leGradientWidget* grad)
{
    leRect rect;
    
    if(grad->widget.style.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        grad->fn->rectToScreen(grad, &rect);
        
        if(grad->dir == LE_DIRECTION_LEFT)
        {
            leRenderer_HorzGradientRect(&rect,
                                        leScheme_GetRenderColor(grad->widget.scheme, LE_SCHM_FOREGROUND),
                                        leScheme_GetRenderColor(grad->widget.scheme, LE_SCHM_FOREGROUND_INACTIVE),
                                        paintState.alpha);
        }
        else if(grad->dir == LE_DIRECTION_RIGHT)
        {
            leRenderer_HorzGradientRect(&rect,
                                        leScheme_GetRenderColor(grad->widget.scheme, LE_SCHM_FOREGROUND_INACTIVE),
                                        leScheme_GetRenderColor(grad->widget.scheme, LE_SCHM_FOREGROUND),
                                        paintState.alpha);
        }
        else if(grad->dir == LE_DIRECTION_DOWN)
        {
            leRenderer_VertGradientRect(&rect,
                                        leScheme_GetRenderColor(grad->widget.scheme, LE_SCHM_FOREGROUND),
                                        leScheme_GetRenderColor(grad->widget.scheme, LE_SCHM_FOREGROUND_INACTIVE),
                                        paintState.alpha);
        }
        else if(grad->dir == LE_DIRECTION_UP)
        {
            leRenderer_VertGradientRect(&rect,
                                        leScheme_GetRenderColor(grad->widget.scheme, LE_SCHM_FOREGROUND_INACTIVE),
                                        leScheme_GetRenderColor(grad->widget.scheme, LE_SCHM_FOREGROUND),
                                        paintState.alpha);
        }
    }
    
    nextState(grad);
}

static void drawBorder(leGradientWidget* grad)
{
    if(grad->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)grad,
                                                    paintState.alpha);
    }
    else if(grad->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)grad,
                                                      paintState.alpha);
    }
        
    nextState(grad);
}

void _leGradientWidget_Paint(leGradientWidget* grad)
{
    if(grad->widget.status.drawState == NOT_STARTED)
    {
        nextState(grad);
    }
    
    while(grad->widget.status.drawState != DONE)
    {
        grad->widget.drawFunc((leWidget*)grad);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_GRADIENT_WIDGET_ENABLED
