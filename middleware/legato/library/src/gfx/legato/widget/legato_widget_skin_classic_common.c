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


#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/renderer/legato_renderer.h"

extern int32_t rectCounter;

void leDraw_2x1BevelBorder(leRect* rect,
                           leColor topOuterColor,
                           leColor topInnerColor,
                           leColor bottomOuterColor,
                           uint32_t a)
{

    // top outer lines
    leRenderer_HorzLine(rect->x,
                        rect->y,
                        rect->width - 1,
                        topOuterColor,
                        a);

    leRenderer_VertLine(rect->x,
                        rect->y + 1,
                        rect->height - 1,
                        topOuterColor,
                        a);

    // top inner lines
    leRenderer_HorzLine(rect->x + 1,
                        rect->y + 1,
                        rect->width - 2,
                        topInnerColor,
                        a);

    leRenderer_VertLine(rect->x + 1,
                        rect->y + 1,
                        rect->height - 2,
                        topInnerColor,
                        a);

    // bottom outer lines
    leRenderer_HorzLine(rect->x,
                        rect->y + rect->height - 1,
                        rect->width - 1,
                        bottomOuterColor,
                        a);
    
    leRenderer_VertLine(rect->x + rect->width - 1,
                        rect->y + 1,
                        rect->height - 1,
                        bottomOuterColor,
                        a);               
}

void leDraw_1x2BevelBorder(leRect* rect,
                           leColor topColor,
                           leColor bottomInnerColor,
                           leColor bottomOuterColor,
                           uint32_t a)
{
    // top outer lines
    leRenderer_HorzLine(rect->x,
                        rect->y,
                        rect->width - 2,
                        topColor,
                        a);

    leRenderer_VertLine(rect->x,
                        rect->y + 1,
                        rect->height - 2,
                        topColor,
                        a);

    // bottom inner lines
    leRenderer_HorzLine(rect->x + 1,
                        rect->y + rect->height - 2,
                        rect->width - 2,
                        bottomInnerColor,
                        a);
    
    leRenderer_VertLine(rect->x + rect->width - 2,
                        rect->y + 1,
                        rect->height - 3,
                        bottomInnerColor,
                        a);

    // bottom outer lines
    leRenderer_HorzLine(rect->x,
                        rect->y + rect->height - 1,
                        rect->width - 1,
                        bottomOuterColor,
                        a);
    
    leRenderer_VertLine(rect->x + rect->width - 1,
                        rect->y,
                        rect->height - 2,
                        bottomOuterColor,
                        a);               
}

void leDraw_2x2BevelBorder(leRect* rect,
                           leColor topOuterColor,
                           leColor topInnerColor,
                           leColor bottomInnerColor,
                           leColor bottomOuterColor,
                           uint32_t a)
{
    // top outer lines
    leRenderer_HorzLine(rect->x,
                        rect->y,
                        rect->width - 2,
                        topOuterColor,
                        a);

    leRenderer_VertLine(rect->x,
                        rect->y + 1,
                        rect->height - 2,
                        topOuterColor,
                        a);

    // top inner lines
    leRenderer_HorzLine(rect->x + 1,
                        rect->y + 1,
                        rect->width - 3,
                        topInnerColor,
                        a);

    leRenderer_VertLine(rect->x + 1,
                        rect->y + 1,
                        rect->height - 3,
                        topInnerColor,
                        a);

    // bottom inner lines
    leRenderer_HorzLine(rect->x + 1,
                        rect->y + rect->height - 2,
                        rect->width - 2,
                        bottomInnerColor,
                        a);
    
    leRenderer_VertLine(rect->x + rect->width - 2,
                        rect->y + 1,
                        rect->y + rect->height - 3,
                        bottomInnerColor,
                        a);

    // bottom outer lines
    leRenderer_HorzLine(rect->x,
                        rect->y + rect->height - 1,
                        rect->width - 1,
                        bottomOuterColor,
                        a);
    
    leRenderer_VertLine(rect->x + rect->width - 1,
                        rect->y,
                        rect->height - 2,
                        bottomOuterColor,
                        a);
}

void leWidget_SkinClassic_FillRoundCornerRect(const leRect* rect,
                                              leColor clr,
                                              uint32_t radius,
                                              uint32_t a)
{
    leRect drawRect;
    
    //Draw the center rectangle
    drawRect.x = rect->x + radius;
    drawRect.y = rect->y + radius;
    drawRect.width = rect->width - (radius * 2);
    drawRect.height = rect->height - (radius * 2);
    
    leRenderer_RectFill(&drawRect, clr, a);
    
    //Draw the top rectangle
    drawRect.x = rect->x + radius;
    drawRect.y = rect->y;
    drawRect.width = rect->width - (radius * 2);
    drawRect.height = radius;
    
    leRenderer_RectFill(&drawRect, clr, a);
    
    //Draw the bottom rectangle
    drawRect.x = rect->x + radius;
    drawRect.y = rect->y + rect->height - radius;
    drawRect.width = rect->width - (radius * 2);
    drawRect.height = radius;
    
    leRenderer_RectFill(&drawRect, clr, a);
    
    //Draw the left rectangle
    drawRect.x = rect->x;
    drawRect.y = rect->y + radius;
    drawRect.width = radius;
    drawRect.height = rect->height - (radius * 2);
    
    leRenderer_RectFill(&drawRect, clr, a);
    
    //Draw the right rectangle
    drawRect.x = rect->x + rect->width - radius;
    drawRect.y = rect->y + radius;
    drawRect.width = radius;
    drawRect.height = rect->height - (radius * 2);
    
    leRenderer_RectFill(&drawRect, clr, a);   
    
    //Draw the corners
    //Upper left
    drawRect.x = rect->x;
    drawRect.y = rect->y;
    drawRect.width = radius;
    drawRect.height = radius;

    leRenderer_ArcFill(&drawRect,
                       90,
                       90,
                       radius,
                       LE_FALSE,
                       clr,
                       LE_ARC_SMOOTH_EDGE,
                       a);
    
    //Upper right
    drawRect.x = rect->x + rect->width - radius;
    drawRect.y = rect->y;
    drawRect.width = radius;
    drawRect.height = radius;
    
    leRenderer_ArcFill(&drawRect,
                       0,
                       90,
                       radius,
                       LE_FALSE,
                       clr,
                       LE_ARC_SMOOTH_EDGE,
                       a);
    
    //Lower left
    drawRect.x = rect->x;
    drawRect.y = rect->y + rect->height - (radius * 2);
    drawRect.width = radius;
    drawRect.height = radius;
    
    //pnt.x = drawRect.x + radius;
    //pnt.y = drawRect.y + radius;
    
    leRenderer_ArcFill(&drawRect,
                       180,
                       90,
                       radius,
                       LE_FALSE,
                       clr,
                       LE_ARC_SMOOTH_EDGE,
                       a);
    
    //Lower right
    drawRect.x = rect->x + rect->width - radius;
    drawRect.y = rect->y + rect->height - radius;
    drawRect.width = radius;
    drawRect.height = radius;
    
    //pnt.x = drawRect.x + radius;
    //pnt.y = drawRect.y + radius;
    
    leRenderer_ArcFill(&drawRect,
                       270,
                       90,
                       radius,
                       LE_FALSE,
                       clr,
                       LE_ARC_SMOOTH_EDGE,
                       a);
}

void leWidget_SkinClassic_DrawBackground(leWidget* wgt,
                                         leColor clr,
                                         uint32_t alpha)
{
    leRect rect;

    wgt->fn->rectToScreen(wgt, &rect);

    //Only support round corners for no or line borders
    if (wgt->style.cornerRadius > 0 &&
        (wgt->style.borderType == LE_WIDGET_BORDER_NONE ||
         wgt->style.borderType == LE_WIDGET_BORDER_LINE))
    {
        leWidget_SkinClassic_FillRoundCornerRect(&rect, 
                                                 clr,
                                                 wgt->style.cornerRadius,
                                                 alpha);
    }    
    else
    {
        leRenderer_RectFill(&rect, clr, alpha);
    }
}

void leWidget_SkinClassic_DrawRoundCornerBackground(leWidget* wgt, 
                                                    leColor clr,
                                                    uint32_t alpha)
{
    leRect rect;

    wgt->fn->rectToScreen(wgt, &rect);
    
    leWidget_SkinClassic_FillRoundCornerRect(&rect, 
                                             clr,
                                             wgt->style.cornerRadius,
                                             alpha);
}

void leWidget_SkinClassic_DrawStandardBackground(leWidget* wgt,
                                                 uint32_t alpha)
{    
    if(wgt->style.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {    
        leWidget_SkinClassic_DrawBackground(wgt,
                                            leScheme_GetRenderColor(wgt->scheme, LE_SCHM_BASE),
                                            alpha);
    }
}

void leWidget_SkinClassic_DrawRoundCornerLineBorder(const leRect* rect,
                                                    leColor clr,
                                                    uint32_t radius,
                                                    uint32_t a)
{
    leRect drawRect;
    
    //Draw the top line
    leRenderer_HorzLine(rect->x + radius,
                        rect->y,
                        rect->width - (radius * 2),
                        clr,
                        a);
    
    //Draw the bottom line
    leRenderer_HorzLine(rect->x + radius,
                        rect->y + rect->height - 1,
                        rect->width - (radius * 2),
                        clr,
                        a);
    
    //Draw the left line
    leRenderer_VertLine(rect->x,
                        rect->y + radius,
                        rect->height - (radius * 2),
                        clr,
                        a);
    
    //Draw the right line
    leRenderer_VertLine(rect->x + rect->width - 1,
                        rect->y + radius,
                        rect->height - (radius * 2),
                        clr,
                        a);
    
    //Upper left
    drawRect.x = rect->x;
    drawRect.y = rect->y;
    drawRect.width = radius;
    drawRect.height = radius;
    
    leRenderer_ArcFill(&drawRect,
                       90,
                       90,
                       1,
                       LE_FALSE,
                       clr,
                       LE_FALSE,
                       a);
    
    //Upper right
    drawRect.x = rect->x + rect->width - radius;
    drawRect.y = rect->y;
    drawRect.width = radius;
    drawRect.height = radius;
    
    leRenderer_ArcFill(&drawRect,
                       0,
                       90,
                       1,
                       LE_FALSE,
                       clr,
                       LE_FALSE,
                       a);
    
    //Lower left
    drawRect.x = rect->x;
    drawRect.y = rect->y + rect->height - (radius * 2);
    drawRect.width = radius;
    drawRect.height = radius;
    
    leRenderer_ArcFill(&drawRect,
                       180,
                       90,
                       1,
                       LE_FALSE,
                       clr,
                       LE_FALSE,
                       a);
    
    //Lower right
    drawRect.x = rect->x + rect->width - radius;
    drawRect.y = rect->y + rect->height - radius;
    drawRect.width = radius;
    drawRect.height = radius;
    
    leRenderer_ArcFill(&drawRect,
                       270,
                       90,
                       1,
                       LE_FALSE,
                       clr,
                       LE_FALSE,
                       a);
}

void leWidget_SkinClassic_DrawLineBorder(const leRect* rect,
                                         leColor clr,
                                         uint32_t a)
{
    // left line
    leRenderer_VertLine(rect->x,
                        rect->y + 1,
                        rect->height - 1,
                        clr,
                        a);
                        
    // right line
    leRenderer_VertLine(rect->x + rect->width - 1,
                        rect->y + 1,
                        rect->height - 1,
                        clr,
                        a);

    // top line                        
    leRenderer_HorzLine(rect->x,
                        rect->y,
                        rect->width,
                        clr,
                        a);  
                        
    // bottom line                        
    leRenderer_HorzLine(rect->x,
                        rect->y + rect->height - 1,
                        rect->width,
                        clr,
                        a);                
}

void leWidget_SkinClassic_Draw2x2BeveledBorder(const leRect* rect,
                                               leColor leftUpOuter,
                                               leColor leftUpInner,
                                               leColor bottomRightOuter,
                                               leColor bottomRightInner,
                                               uint32_t a)
{

    
    // left outer
    leRenderer_VertLine(rect->x,
                        rect->y + 1,
                        rect->height - 2,
                        leftUpOuter,
                        a);
                        
    // left inner
    leRenderer_VertLine(rect->x + 1,
                        rect->y + 1,
                        rect->height - 2,
                        leftUpInner,
                        a);
    
    // top outer
    leRenderer_HorzLine(rect->x,
                        rect->y,
                        rect->width - 1,
                        leftUpOuter,
                        a);
                        
    // top inner
    leRenderer_HorzLine(rect->x + 1,
                        rect->y + 1,
                        rect->width - 2,
                        leftUpInner,
                        a);
                        
    // right outer
    leRenderer_VertLine(rect->x + rect->width - 1,
                        rect->y + 1,
                        rect->height - 2,
                        bottomRightOuter,
                        a);
                        
    // right inner
    leRenderer_VertLine(rect->x + rect->width - 2,
                        rect->y + 1,
                        rect->height - 2,
                        bottomRightInner,
                        a);
    
    // bottom outer
    leRenderer_HorzLine(rect->x,
                        rect->y + rect->height - 1,
                        rect->width,
                        bottomRightOuter,
                        a);
    // bottom inner
    leRenderer_HorzLine(rect->x + 1,
                        rect->y + rect->height - 2,
                        rect->width - 2,
                        bottomRightInner,
                        a);
}                                           
                                            
void leWidget_SkinClassic_Draw1x2BeveledBorder(const leRect* rect,
                                               leColor leftUp,
                                               leColor bottomRightOuter,
                                               leColor bototmRightInner,
                                               uint32_t a)
{
    // left line
    leRenderer_VertLine(rect->x,
                        rect->y + 1,
                        rect->height - 1,
                        leftUp,
                        a);
                        
    // top line                        
    leRenderer_HorzLine(rect->x,
                        rect->y,
                        rect->width - 1,
                        leftUp,
                        a);  

    // right outer line
    leRenderer_VertLine(rect->x + rect->width - 1,
                        rect->y,
                        rect->height - 1,
                        bottomRightOuter,
                        a);
                            
    // right inner line
    leRenderer_VertLine(rect->x + rect->width - 2,
                        rect->y + 1,
                        rect->height - 2,
                        bototmRightInner,
                        a);
    
    // bottom outer
    leRenderer_HorzLine(rect->x,
                        rect->y + rect->height - 1,
                        rect->width,
                        bottomRightOuter,
                        a);
    
    // bottom inner
    leRenderer_HorzLine(rect->x + 1,
                        rect->y + rect->height - 2,
                        rect->width - 2,
                        bototmRightInner,
                        a);
}

void leWidget_SkinClassic_DrawStandardLineBorder(leWidget* wgt,
                                                 uint32_t alpha)
{
    leRect rect;

    wgt->fn->rectToScreen(wgt, &rect);

    if (wgt->style.cornerRadius == 0)
    {
        leWidget_SkinClassic_DrawLineBorder(&rect,
                                            leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOWDARK),
                                            alpha);
    }
    else
    {
        leWidget_SkinClassic_DrawRoundCornerLineBorder(&rect,
                                                       leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOWDARK),
                                                       wgt->style.cornerRadius,
                                                       alpha);
    }
}

void leWidget_SkinClassic_DrawStandardRoundCornerLineBorder(leWidget* wgt,
                                                            uint32_t alpha)
{
    leRect rect;

    wgt->fn->rectToScreen(wgt, &rect);

    leWidget_SkinClassic_DrawRoundCornerLineBorder(&rect,
                                                   leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOWDARK),
                                                   wgt->style.cornerRadius,
                                                   alpha);
}

void leWidget_SkinClassic_DrawStandardRaisedBorder(leWidget* wgt,
                                                   uint32_t alpha)
{
    leRect rect;

    wgt->fn->rectToScreen(wgt, &rect);

    leWidget_SkinClassic_Draw2x2BeveledBorder(&rect,
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOWDARK),
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOW),
                                              alpha);
}

void leWidget_SkinClassic_DrawStandardLoweredBorder(leWidget* wgt,
                                                    uint32_t alpha)
{
    leRect rect;

    wgt->fn->rectToScreen(wgt, &rect);

    leWidget_SkinClassic_Draw2x2BeveledBorder(&rect,
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOWDARK),
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOW),
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                              alpha);
}

void leWidget_SkinClassic_DrawStandardHybridBorder(leWidget* wgt,
                                                   uint32_t alpha)
{
    leRect rect;

    wgt->fn->rectToScreen(wgt, &rect);

    leWidget_SkinClassic_Draw1x2BeveledBorder(&rect,
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOWDARK),
                                              leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOW),
                                              alpha);
}

void leWidget_SkinClassic_InvalidateBorderAreas(leWidget* wgt)
{
	leRect rect, dmgRect;
	int32_t width, height;
	
	if(wgt->style.borderType == LE_WIDGET_BORDER_NONE)
	    return;
	
	wgt->fn->rectToScreen(wgt, &rect);
	
	if(wgt->style.borderType == LE_WIDGET_BORDER_LINE)
	{
	    if(rect.width == 0 || rect.height == 0)
			return;

		if(rect.width <= 1 || rect.height <= 1)
		{
			wgt->fn->_damageArea(wgt, &rect);

			return;
		}
	
	    width = 1;
	    height = 1;
	}
	else
	{
	    if(rect.width == 0 || rect.height == 0)
			return;

		if(rect.width <= 3 || rect.height <= 3)
		{
			wgt->fn->_damageArea(wgt, &rect);
			
			return;
        }
        
        width = 2;
	    height = 2;
	}
	
	// left line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y;
	dmgRect.width = width;
	dmgRect.height = rect.height;

	wgt->fn->_damageArea(wgt, &dmgRect);

	// top line
	dmgRect.width = rect.width;
	dmgRect.height = height;

	wgt->fn->_damageArea(wgt, &dmgRect);

	// right line
	dmgRect.x = rect.x + rect.width - width;
	dmgRect.y = rect.y;
	dmgRect.width = width;
	dmgRect.height = rect.height;

	wgt->fn->_damageArea(wgt, &dmgRect);

	// bottom line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y + rect.height - height;
	dmgRect.width = rect.width;
	dmgRect.height = height;

	wgt->fn->_damageArea(wgt, &dmgRect);
}