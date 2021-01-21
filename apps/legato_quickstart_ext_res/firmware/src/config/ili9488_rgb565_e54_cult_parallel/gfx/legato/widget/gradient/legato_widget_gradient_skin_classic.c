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
    switch(grad->widget.drawState)
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
            
            if(grad->widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                grad->widget.drawState = DRAW_BACKGROUND;
                grad->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            if(grad->widget.borderType != LE_WIDGET_BORDER_NONE)
            {
                grad->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                grad->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            grad->widget.drawState = DONE;
            grad->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leGradientWidget* grad)
{
    leRect rect;
    
    if(grad->widget.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        rect = grad->fn->rectToScreen(grad);
        
        if(grad->dir == LE_DIRECTION_LEFT)
        {
            leRenderer_HorzGradientRect(&rect,
                                        grad->widget.scheme->foreground,
                                        grad->widget.scheme->foregroundInactive,
                                        paintState.alpha);
        }
        else if(grad->dir == LE_DIRECTION_RIGHT)
        {
            leRenderer_HorzGradientRect(&rect,
                                        grad->widget.scheme->foregroundInactive,
                                        grad->widget.scheme->foreground,
                                        paintState.alpha);
        }
        else if(grad->dir == LE_DIRECTION_DOWN)
        {
            leRenderer_VertGradientRect(&rect,
                                        grad->widget.scheme->foreground,
                                        grad->widget.scheme->foregroundInactive,
                                        paintState.alpha);
        }
        else if(grad->dir == LE_DIRECTION_UP)
        {
            leRenderer_VertGradientRect(&rect,
                                        grad->widget.scheme->foregroundInactive,
                                        grad->widget.scheme->foreground,
                                        paintState.alpha);
        }
    }
    
    nextState(grad);
}

static void drawBorder(leGradientWidget* grad)
{
    if(grad->widget.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)grad,
                                                    paintState.alpha);
    }
    else if(grad->widget.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)grad,
                                                      paintState.alpha);
    }
        
    nextState(grad);
}

void _leGradientWidget_Paint(leGradientWidget* grad)
{
    if(grad->widget.scheme == NULL)
    {
        grad->widget.drawState = DONE;
        
        return;
    }
    
    if(grad->widget.drawState == NOT_STARTED)
    {
        nextState(grad);
    }
    
    while(grad->widget.drawState != DONE)
    {
        grad->widget.drawFunc((leWidget*)grad);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_GRADIENT_WIDGET_ENABLED
