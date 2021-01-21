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


#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

static void drawBackground(leWidget* wdg);
static void drawBorder(leWidget* wdg);

static void drawBackground(leWidget* wdg)
{
    //printf("button painting\n");
    
    if(wdg->style.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        //Only support round corners for no or line borders
        if (wdg->style.cornerRadius > 0 &&
            (wdg->style.borderType == LE_WIDGET_BORDER_NONE ||
             wdg->style.borderType == LE_WIDGET_BORDER_LINE))
        {
            leWidget_SkinClassic_DrawRoundCornerBackground(wdg,
                                                           leScheme_GetRenderColor(wdg->scheme, LE_SCHM_BASE),
                                                           paintState.alpha);
        }
        else
        {
            leWidget_SkinClassic_DrawBackground(wdg,
                                                leScheme_GetRenderColor(wdg->scheme, LE_SCHM_BASE),
                                                paintState.alpha);
        }
    }

    if(wdg->style.borderType != LE_WIDGET_BORDER_NONE)
    {
        wdg->status.drawState = DRAW_BORDER;
        wdg->drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
    }
    else
    {
        wdg->status.drawState = DONE;
        wdg->drawFunc = NULL;
        
        return;
    }
    
#if LE_PREEMPTION_LEVEL < 2
    wdg->drawFunc((leWidget*)wdg);
#endif
}

static void drawBorder(leWidget* wdg)
{
    if(wdg->style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder(wdg, paintState.alpha);
    }
    else if(wdg->style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder(wdg, paintState.alpha);
    }
        
    wdg->status.drawState = DONE;
    wdg->drawFunc = NULL;
}

void _leWidget_Paint(leWidget* wdg)
{
    if(wdg->status.drawState == NOT_STARTED)
    {
        paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
        if(wdg->fn->getCumulativeAlphaEnabled(wdg) == LE_TRUE)
        {
            paintState.alpha = wdg->fn->getCumulativeAlphaAmount(wdg);
        }
#endif

        wdg->drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;
    }
    
    wdg->drawFunc((leWidget*)wdg);
}