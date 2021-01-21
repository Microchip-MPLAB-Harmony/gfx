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


#include "gfx/legato/widget/radialmenu/legato_widget_radial_menu.h"

#if LE_RADIALMENU_WIDGET_ENABLED

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
	ARRANGE_ITEMS,
	DRAW_ELLIPSE,
};

static struct
{
    uint32_t alpha;
} paintState;

static void drawBackground(leRadialMenuWidget* mn);
static void drawEllipse(leRadialMenuWidget* mn);
static void arrangeItems(leRadialMenuWidget* mn);

static void nextState(leRadialMenuWidget* mn)
{
    switch(mn->widget.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(mn->fn->getCumulativeAlphaEnabled(mn) == LE_TRUE)
            {
                paintState.alpha = mn->fn->getCumulativeAlphaAmount(mn);
            }
#endif

            if(mn->widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                mn->widget.drawState = DRAW_BACKGROUND;
                mn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            if(mn->drawEllipse == LE_TRUE)
            {
                mn->widget.drawState = DRAW_ELLIPSE;
                mn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawEllipse;

                return;
            }
        }
        case DRAW_ELLIPSE:
        {            
            /*if(mn->widgetList.size > 0)
            {
                mn->widget.drawState = ARRANGE_ITEMS;
                mn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&arrangeItems;

                return;
            }*/
        }
        case ARRANGE_ITEMS:
        {
            mn->widget.drawState = DONE;
            mn->widget.drawFunc = NULL;

            return;
        }
    }
}

static void drawBackground(leRadialMenuWidget* mn)
{
    leWidget_SkinClassic_DrawBackground(&mn->widget, mn->widget.scheme->background);        

    nextState(mn);
}

static void drawEllipse(leRadialMenuWidget* mn)
{
    lePoint centerPoint = lePoint_Zero;
    
    leUtils_PointToScreenSpace((leWidget*)mn, &centerPoint);

    centerPoint.x += mn->widget.rect.width / 2;
    centerPoint.y += mn->widget.rect.height / 2;
    
    leRenderer_EllipseLine(centerPoint.x,
                           centerPoint.y,
                           mn->a,
                           mn->b,
                           mn->theta,
                           0,
                           360,
                           mn->widget.scheme->foreground,
                           paintState.alpha);
    
    nextState(mn);
}





void _leRadialMenuWidget_Paint(leRadialMenuWidget* mn)
{
    if(mn->widget.scheme == NULL)
    {
        mn->widget.drawState = DONE;
        
        return;
    }
    
    if(mn->widget.drawState == NOT_STARTED)
    {
        nextState(mn);
    }
    
    while(mn->widget.drawState != DONE)
    {
        mn->widget.drawFunc((leWidget*)mn);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_RADIALMENU_WIDGET_ENABLED