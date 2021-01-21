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


#include "gfx/legato/widget/keypad/legato_widget_keypad.h"

#if LE_KEYPAD_WIDGET_ENABLED && LE_BUTTON_WIDGET_ENABLED

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

void _leKeyPadWidget_InvalidateBorderAreas(leKeyPadWidget* pad)
{
    pad->fn->invalidate(pad);
}

static void drawBackground(leKeyPadWidget* pad);
static void drawBorder(leKeyPadWidget* pad);

static void nextState(leKeyPadWidget* pad)
{
    switch(pad->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(pad->fn->getCumulativeAlphaEnabled(pad) == LE_TRUE)
            {
                paintState.alpha = pad->fn->getCumulativeAlphaAmount(pad);
            }
#endif
            
            if(pad->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                pad->widget.status.drawState = DRAW_BACKGROUND;
                pad->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            if(pad->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                pad->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                pad->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            pad->widget.status.drawState = DONE;
            pad->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leKeyPadWidget* pad)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)pad,
                                                paintState.alpha);
    
    nextState(pad);
}

static void drawBorder(leKeyPadWidget* pad)
{
    if(pad->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)pad,
                                                    paintState.alpha);
    }
    else if(pad->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)pad,
                                                      paintState.alpha);
    }
    
    nextState(pad);
}

void _leKeyPadWidget_Paint(leKeyPadWidget* pad)
{
    leWidget* child;
    size_t i;
    
    // hack to sync child attributes to key pad style
    for(i = 0; i < pad->widget.children.size; i++)
    {
        child = pad->widget.children.values[i];
        
        child->style.borderType = pad->widget.style.borderType;
        child->scheme = pad->widget.scheme;
    }
    
    if(pad->widget.status.drawState == NOT_STARTED)
    {
        nextState(pad);
    }
    
    while(pad->widget.status.drawState != DONE)
    {
        pad->widget.drawFunc((leWidget*)pad);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_WIDGET_KEYPAD_ENABLED && LE_WIDGET_BUTTON_ENABLED