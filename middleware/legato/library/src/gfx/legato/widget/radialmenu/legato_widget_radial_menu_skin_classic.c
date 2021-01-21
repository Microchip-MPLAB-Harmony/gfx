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
#include "gfx/legato/widget/radialmenu/legato_widget_radial_menu.h"

#if LE_RADIALMENU_WIDGET_ENABLED

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define DEFAULT_HIGHLIGHT_MARGIN 2

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
	DRAW_ELLIPSE,
	ARRANGE_ITEMS,
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
    switch(mn->widget.status.drawState)
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

            if(mn->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                mn->widget.status.drawState = DRAW_BACKGROUND;
                mn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            if(mn->drawEllipse == LE_TRUE)
            {
                mn->widget.status.drawState = DRAW_ELLIPSE;
                mn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawEllipse;

                return;
            }
        }
        // fall through
        case DRAW_ELLIPSE:
        {
            if(mn->widgetList.size > 0)
            {
                mn->widget.status.drawState = ARRANGE_ITEMS;
                mn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&arrangeItems;

                return;
            }
        }
        // fall through
        default:
        {
            mn->widget.status.drawState = DONE;
            mn->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leRadialMenuWidget* mn)
{
    leWidget_SkinClassic_DrawBackground(&mn->widget,
                                        leScheme_GetRenderColor(mn->widget.scheme, LE_SCHM_BACKGROUND),
                                        paintState.alpha);

    nextState(mn);
}



static void drawEllipse(leRadialMenuWidget* mn)
{
    lePoint centerPoint = lePoint_Zero;
    leRadialMenuItemNode* item;
    leRect widgetRect, highlightRect;

    leUtils_PointToScreenSpace((leWidget*)mn, &centerPoint);

    centerPoint.x += mn->widget.rect.width / 2;
    centerPoint.y += mn->widget.rect.height / 2;

    leRenderer_EllipseLine(centerPoint.x,
                           centerPoint.y,
                           mn->ellipse.a,
                           mn->ellipse.b,
                           mn->ellipse.theta,
                           0,
                           360,
                           leScheme_GetRenderColor(mn->widget.scheme, LE_SCHM_FOREGROUND),
                           paintState.alpha);

    if(mn->rotationDegrees == 0 && mn->highlightProminent == LE_TRUE && mn->widgetList.size > 0)
    {
        item = leList_Get(&mn->widgetList, mn->prominentIndex);
        item->widget->fn->rectToScreen(item->widget, &widgetRect);

        // top line
        highlightRect = widgetRect;
        highlightRect.x -= 4;
        highlightRect.y -= 4;
        highlightRect.width += 8;
        highlightRect.height = 4;

        leRenderer_RectFill(&highlightRect, leScheme_GetRenderColor(mn->widget.scheme, LE_SCHM_HIGHLIGHT), paintState.alpha);

        // left line
        highlightRect = widgetRect;
        highlightRect.x -= 4;
        highlightRect.y -= 4;
        highlightRect.width = 4;
        highlightRect.height += 8;

        leRenderer_RectFill(&highlightRect, leScheme_GetRenderColor(mn->widget.scheme, LE_SCHM_HIGHLIGHT), paintState.alpha);

        // right line
        highlightRect = widgetRect;
        highlightRect.x += highlightRect.width;
        highlightRect.y -= 4;
        highlightRect.width = 4;
        highlightRect.height += 8;

        leRenderer_RectFill(&highlightRect, leScheme_GetRenderColor(mn->widget.scheme, LE_SCHM_HIGHLIGHT), paintState.alpha);

        // bottom line
        highlightRect = widgetRect;
        highlightRect.x -= 4;
        highlightRect.y += highlightRect.height;
        highlightRect.width += 8;
        highlightRect.height = 4;

        leRenderer_RectFill(&highlightRect, leScheme_GetRenderColor(mn->widget.scheme, LE_SCHM_HIGHLIGHT), paintState.alpha);
    }

    nextState(mn);
}

static void arrangeItems(leRadialMenuWidget* mn)
{
    nextState(mn);
}

void _leRadialMenuWidget_Paint(leRadialMenuWidget* mn)
{
    if(mn->widget.status.drawState == NOT_STARTED)
    {
        nextState(mn);
    }
    
    while(mn->widget.status.drawState != DONE)
    {
        mn->widget.drawFunc((leWidget*)mn);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_RADIALMENU_WIDGET_ENABLED