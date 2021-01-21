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


#include "gfx/legato/widget/rectangle/legato_widget_rectangle.h"

#if LE_RECTANGLE_WIDGET_ENABLED

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
    DRAW_EDGE,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

static void drawBackground(leRectangleWidget* rct);
static void drawEdge(leRectangleWidget* rct);
static void drawBorder(leRectangleWidget* rct);

void _leRectangleWidget_InvalidateRect(leRectangleWidget* rct)
{
	leRect rect, dmgRect;
	
	if(rct->thickness == 0)
	    return;
	    
	rct->fn->localRect(rct, &rect);
	    
	if(rect.width <= rct->thickness || rect.height <= rct->thickness)
	{
	    rct->fn->invalidate(rct);

		return;
	}

	// left line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y;
	dmgRect.width = rct->thickness;
	dmgRect.height = rect.height;

    rct->fn->_damageArea(rct, &dmgRect);

	// top line
	dmgRect.width = rect.width;
	dmgRect.height = rct->thickness;

	rct->fn->_damageArea(rct, &dmgRect);

	// right line
	dmgRect.x = rect.x + rect.width - rct->thickness;
	dmgRect.y = rect.y;
	dmgRect.width = rct->thickness;
	dmgRect.height = rect.height;

	rct->fn->_damageArea(rct, &dmgRect);

	// bottom line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y + rect.height - rct->thickness;
	dmgRect.width = rect.width;
	dmgRect.height = rct->thickness;

	rct->fn->_damageArea(rct, &dmgRect);
}

static void nextState(leRectangleWidget* rct)
{
    switch(rct->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(rct->fn->getCumulativeAlphaEnabled(rct) == LE_TRUE)
            {
                paintState.alpha = rct->fn->getCumulativeAlphaAmount(rct);
            }
#endif
            
            if(rct->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                rct->widget.status.drawState = DRAW_BACKGROUND;
                rct->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            if(rct->thickness > 0)
            {
                rct->widget.status.drawState = DRAW_EDGE;
                rct->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawEdge;
                
                return;
            }
        }
        // fall through
        case DRAW_EDGE:
        {            
            if(rct->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                rct->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                rct->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            rct->widget.status.drawState = DONE;
            rct->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leRectangleWidget* rct)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)rct,
                                                paintState.alpha);
    
    nextState(rct);
}

static void drawEdge(leRectangleWidget* rct)
{
    leRect widgetRect, edgeRect;
    
    rct->fn->rectToScreen(rct, &widgetRect);

    // left bar
    edgeRect.x = widgetRect.x;
    edgeRect.y = widgetRect.y;
    edgeRect.width = rct->thickness;
    edgeRect.height = widgetRect.height;
    
    leRenderer_RectFill(&edgeRect,
                        leScheme_GetRenderColor(rct->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
                 
    // top bar
    edgeRect.x = widgetRect.x + rct->thickness;
    edgeRect.width = widgetRect.width - (rct->thickness * 2);
    edgeRect.height = rct->thickness; 
                 
    leRenderer_RectFill(&edgeRect,
                        leScheme_GetRenderColor(rct->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
                 
    // right bar
    edgeRect.x = widgetRect.x + widgetRect.width - rct->thickness;
    edgeRect.width = rct->thickness;
    edgeRect.height = widgetRect.height; 
                 
    leRenderer_RectFill(&edgeRect,
                        leScheme_GetRenderColor(rct->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
                 
    // bottom bar
    edgeRect.x = widgetRect.x + rct->thickness;
    edgeRect.y = widgetRect.y + widgetRect.height - rct->thickness;
    edgeRect.width = widgetRect.width - (rct->thickness * 2);
    edgeRect.height = rct->thickness; 
                 
    leRenderer_RectFill(&edgeRect,
                        leScheme_GetRenderColor(rct->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
    
    nextState(rct);
}

static void drawBorder(leRectangleWidget* rct)
{
    if(rct->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)rct,
                                                    paintState.alpha);
    }
    else if(rct->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)rct,
                                                      paintState.alpha);
    }
    
    nextState(rct);
}

void _leRectangleWidget_Paint(leRectangleWidget* rct)
{
    if(rct->widget.status.drawState == NOT_STARTED)
    {
        nextState(rct);
    }
    
    while(rct->widget.status.drawState != DONE)
    {
        rct->widget.drawFunc((leWidget*)rct);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_RECTANGLE_WIDGET_ENABLED