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


#include "gfx/legato/widget/scrollbar/legato_widget_scrollbar.h"

#if LE_SCROLLBAR_WIDGET_ENABLED

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
    DRAW_UP_BUTTON,
    DRAW_DOWN_BUTTON,
    DRAW_HANDLE
};

static struct
{
    uint32_t alpha;
} paintState;

uint32_t _leScrollWidget_GetExtent(const leScrollBarWidget* _this);

void _leScrollBar_GetUpLeftButtonRect(const leScrollBarWidget* bar, leRect* rect)
{
    rect->x = 0;
    rect->y = 0;
        
    if(bar->alignment == LE_ORIENTATION_VERTICAL)
    {
        rect->width = bar->widget.rect.width;
        rect->height = rect->width;
    }
    else
    {
        rect->width = bar->widget.rect.height;
        rect->height = rect->width;
    }
    
    leUtils_RectToScreenSpace((leWidget*)bar, rect);
}

void _leScrollBar_GetDownRightButtonRect(const leScrollBarWidget* bar, leRect* rect)
{
    if(bar->alignment == LE_ORIENTATION_VERTICAL)
    {
        rect->width = bar->widget.rect.width;
        rect->height = rect->width;
        rect->x = 0;
        rect->y = bar->widget.rect.height - rect->width;
    }
    else
    {
        rect->width = bar->widget.rect.height;
        rect->height = rect->width;
        rect->y = 0;
        rect->x = bar->widget.rect.width - rect->height;
    }
    
    leUtils_RectToScreenSpace((leWidget*)bar, rect);
}

void _leScrollBar_GetScrollAreaRect(const leScrollBarWidget* bar, leRect* rect)
{
    leRect scrollRect;
    
    if(bar->alignment == LE_ORIENTATION_VERTICAL)
    {
        scrollRect.x = 0;
        scrollRect.y = bar->widget.rect.width;
        scrollRect.width = bar->widget.rect.width;
        scrollRect.height = bar->widget.rect.height - (bar->widget.rect.width * 2);
    }
    else
    {
        scrollRect.x = bar->widget.rect.height;
        scrollRect.y = 0;
        scrollRect.width = bar->widget.rect.width - (bar->widget.rect.height * 2);
        scrollRect.height = bar->widget.rect.height;
    }
    
    *rect = scrollRect;
    
    leUtils_RectToScreenSpace((leWidget*)bar, rect);
}

void _leScrollBar_GetHandleRect(const leScrollBarWidget* bar, leRect* rect)
{
    leRect scrollRect;
    uint32_t extent;
    uint32_t sizePercent;
    uint32_t locPercent;
    
    extent = _leScrollWidget_GetExtent(bar);
        
    if(bar->value >= 1)
        extent = extent;
    
    sizePercent = lePercentWholeRounded(extent, bar->max);
    locPercent = lePercentWholeRounded(bar->value, bar->max);
    
    _leScrollBar_GetScrollAreaRect(bar, &scrollRect);
    
    if(bar->alignment == LE_ORIENTATION_VERTICAL)
    {
        sizePercent = lePercentOf(scrollRect.height, sizePercent);
        locPercent = lePercentOf(scrollRect.height - sizePercent, locPercent);
        
        rect->x = scrollRect.x;
        rect->y = scrollRect.y + locPercent;
        rect->width = scrollRect.width;
        rect->height = sizePercent;
    }
    else
    {
        sizePercent = lePercentOf(scrollRect.width, sizePercent);
        locPercent = lePercentOf(scrollRect.width - sizePercent, locPercent);
        
        rect->x = scrollRect.x + locPercent;
        rect->y = scrollRect.y;
        rect->width = sizePercent;
        rect->height = scrollRect.height;
    }
}

void _leScrollBarWidget_InvalidateBorderAreas(const leScrollBarWidget* bar)
{
    bar->fn->invalidate(bar);
}

static void drawBackground(leScrollBarWidget* bar);
static void drawUpButton(leScrollBarWidget* bar);
static void drawDownButton(leScrollBarWidget* bar);
static void drawHandle(leScrollBarWidget* bar);

static void nextState(leScrollBarWidget* bar)
{
    switch(bar->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(bar->fn->getCumulativeAlphaEnabled(bar) == LE_TRUE)
            {
                paintState.alpha = bar->fn->getCumulativeAlphaAmount(bar);
            }
#endif

            bar->widget.status.drawState = DRAW_BACKGROUND;
            bar->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

            return;
        }
        case DRAW_BACKGROUND:
        {
            bar->widget.status.drawState = DRAW_UP_BUTTON;
            bar->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawUpButton;
            
            return;
        }
        case DRAW_UP_BUTTON:
        {            
            bar->widget.status.drawState = DRAW_DOWN_BUTTON;
            bar->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawDownButton;
            
            return;
        }
        case DRAW_DOWN_BUTTON:
        {            
            bar->widget.status.drawState = DRAW_HANDLE;
            bar->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawHandle;
            
            return;
        }
        case DRAW_HANDLE:
        {
            bar->widget.status.drawState = DONE;
            bar->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leScrollBarWidget* bar)
{
    leRect rect;
    
    if(bar->widget.style.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        _leScrollBar_GetScrollAreaRect(bar, &rect);
     
        leRenderer_RectFill(&rect,
                            leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_BACKGROUND),
                            paintState.alpha);
    }
    
    nextState(bar);
}

static void drawUpButton(leScrollBarWidget* bar)
{
    leRect rect;
    lePoint pnt;
    uint32_t arrowOffset = 0;
    
    _leScrollBar_GetUpLeftButtonRect(bar, &rect);
    
    // fill button area
    if(bar->state == LE_SCROLLBAR_STATE_TOP_INSIDE)
    {
        leRenderer_RectFill(&rect,
                            leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_BACKGROUND),
                            paintState.alpha);
        
        arrowOffset = 1;
    }
    else
    {
        leRenderer_RectFill(&rect,
                            leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_BASE),
                            paintState.alpha);
    }
                 
    // draw button arrow
    if(bar->alignment == LE_ORIENTATION_VERTICAL)
    {
        pnt.x = rect.x + (rect.width / 2) - 1 + arrowOffset;
        pnt.y = rect.y + (rect.height / 2) - 1 + arrowOffset;
        
        leRenderer_BlendPixel_Safe(pnt.x, pnt.y - 1, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_HorzLine(pnt.x - 1, pnt.y + 0, 2, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_HorzLine(pnt.x - 2, pnt.y + 1, 4, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_HorzLine(pnt.x - 3, pnt.y + 2, 6, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
    }
    else
    {
        pnt.x = rect.x + (rect.width / 2) - 1 + arrowOffset;
        pnt.y = rect.y + (rect.height / 2) - 1 + arrowOffset;

        leRenderer_BlendPixel_Safe(pnt.x - 1, pnt.y, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_VertLine(pnt.x, pnt.y - 1, 2, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_VertLine(pnt.x + 1, pnt.y - 2, 4, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_VertLine(pnt.x + 2, pnt.y - 3, 6, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
    }
    
    // draw button border
    if(bar->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawLineBorder(&rect,
                                            leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOWDARK),
                                            paintState.alpha);
    }
    else if(bar->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        if(bar->state == LE_SCROLLBAR_STATE_TOP_INSIDE)
        {
            leWidget_SkinClassic_Draw2x2BeveledBorder(&rect,
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOWDARK),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOW),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_HIGHLIGHT),
                                                      paintState.alpha);
        }
        else
        {
            leWidget_SkinClassic_Draw1x2BeveledBorder(&rect,
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOWDARK),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOW),
                                                      paintState.alpha);
        }
    }
    
    nextState(bar);
}

static void drawDownButton(leScrollBarWidget* bar)
{
    leRect rect;
    lePoint pnt;
    uint32_t arrowOffset = 0;
    
    _leScrollBar_GetDownRightButtonRect(bar, &rect);
    
    // fill button area
    if(bar->state == LE_SCROLLBAR_STATE_BOTTOM_INSIDE)
    {
        leRenderer_RectFill(&rect,
                            leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_BACKGROUND),
                            paintState.alpha);
        
        arrowOffset = 1;
    }
    else
    {
        leRenderer_RectFill(&rect,
                            leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_BASE),
                            paintState.alpha);
    }
        
    /*GFX_Set(GFXF_DRAW_MASK_ENABLE, LE_FALSE);
    GFX_Set(GFXF_DRAW_MODE, GFX_DRAW_FILL);
    
    GFX_DrawRect(rect.x,
                 rect.y,
                 rect.width,
                 rect.height);*/
    
    // draw button arrow      
    if(bar->alignment == LE_ORIENTATION_VERTICAL)
    {
        pnt.x = rect.x + (rect.width / 2) - 1 + arrowOffset;
        pnt.y = rect.y + (rect.height / 2) - 1 + arrowOffset;
        
        leRenderer_HorzLine(pnt.x - 3, pnt.y - 1, 6, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_HorzLine(pnt.x - 2, pnt.y + 0, 4, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_HorzLine(pnt.x - 1, pnt.y + 1, 2, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_BlendPixel_Safe(pnt.x, pnt.y + 2, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
    }
    else
    {
        pnt.x = rect.x + (rect.width / 2) - 1 + arrowOffset;
        pnt.y = rect.y + (rect.height / 2) - 1 + arrowOffset;
        
        leRenderer_VertLine(pnt.x - 1, pnt.y - 3, 6, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_VertLine(pnt.x + 0, pnt.y - 2, 4, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_VertLine(pnt.x + 1, pnt.y - 1, 2, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
        leRenderer_BlendPixel_Safe(pnt.x + 2, pnt.y, leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_FOREGROUND), paintState.alpha);
    }
    
    // draw button border
    if(bar->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawLineBorder(&rect,
                                            leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOWDARK),
                                            paintState.alpha);
    }
    else if(bar->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        if(bar->state == LE_SCROLLBAR_STATE_BOTTOM_INSIDE)
        {
            leWidget_SkinClassic_Draw2x2BeveledBorder(&rect,
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOWDARK),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOW),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_HIGHLIGHT),
                                                      paintState.alpha);
        }
        else
        {
            leWidget_SkinClassic_Draw1x2BeveledBorder(&rect,
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOWDARK),
                                                      leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOW),
                                                      paintState.alpha);
        }
    }
    
    nextState(bar);
}

static void drawHandle(leScrollBarWidget* bar)
{
    leRect rect;
    
    _leScrollBar_GetHandleRect(bar, &rect);
    
    // fill handle area
    leRenderer_RectFill(&rect,
                        leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_BASE),
                        paintState.alpha);
    
    // draw handle border
    if(bar->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawLineBorder(&rect,
                                            leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOWDARK),
                                            paintState.alpha);
    }
    else if(bar->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_Draw1x2BeveledBorder(&rect,
                                                  leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                                  leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOWDARK),
                                                  leScheme_GetRenderColor(bar->widget.scheme, LE_SCHM_SHADOW),
                                                  paintState.alpha);
    }
    
    nextState(bar);
}

void _leScrollBarWidget_Paint(leScrollBarWidget* bar)
{
    if(bar->widget.status.drawState == NOT_STARTED)
        nextState(bar);
    
    while(bar->widget.status.drawState != DONE)
    {
        bar->widget.drawFunc((leWidget*)bar);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_SCROLLBAR_WIDGET_ENABLED