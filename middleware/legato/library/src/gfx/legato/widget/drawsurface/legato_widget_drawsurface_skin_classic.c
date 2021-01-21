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


#include "gfx/legato/widget/drawsurface/legato_widget_drawsurface.h"

#if LE_DRAWSURFACE_WIDGET_ENABLED

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
    DRAW_CALLBACK,
    DRAW_BORDER,
};

static void drawBackground(leDrawSurfaceWidget* sfc);
static void drawCallback(leDrawSurfaceWidget* sfc);
static void drawBorder(leDrawSurfaceWidget* sfc);

static struct
{
    uint32_t alpha;
} paintState;

static void nextState(leDrawSurfaceWidget* sfc)
{
    switch(sfc->widget.status.drawState)
    {
        case NOT_STARTED:
        {
paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(sfc->fn->getCumulativeAlphaEnabled(sfc) == LE_TRUE)
            {
                paintState.alpha = sfc->fn->getCumulativeAlphaAmount(sfc);
            }
#endif

            if(sfc->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                sfc->widget.status.drawState = DRAW_BACKGROUND;
                sfc->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            sfc->widget.status.drawState = DRAW_CALLBACK;
            sfc->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawCallback;
            
            return;
        }
        case DRAW_CALLBACK:
        {            
            if(sfc->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                sfc->widget.status.drawState = DRAW_BORDER;
                sfc->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            sfc->widget.status.drawState = DONE;
            sfc->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leDrawSurfaceWidget* sfc)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)sfc,
                                                paintState.alpha);

    nextState(sfc);
}

static void drawCallback(leDrawSurfaceWidget* sfc)
{
    leRect rect;
    
    // paint callback
    if(sfc->cb != NULL)
    {
        sfc->fn->localRect(sfc, &rect);
        
        if(sfc->cb(sfc, &rect) == LE_FALSE)
            return;
    }
    
    nextState(sfc);
}

static void drawBorder(leDrawSurfaceWidget* sfc)
{
    if(sfc->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)sfc,
                                                    paintState.alpha);
    }
    else if(sfc->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)sfc,
                                                      paintState.alpha);
    }
        
    nextState(sfc);
}

void _leDrawSurfaceWidget_Paint(leDrawSurfaceWidget* sfc)
{
    if(sfc->widget.status.drawState == NOT_STARTED)
    {
        nextState(sfc);
    }
    
    while(sfc->widget.status.drawState != DONE)
    {
        sfc->widget.drawFunc((leWidget*)sfc);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_DRAWSURFACE_WIDGET_ENABLED