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


#include "gfx/legato/widget/groupbox/legato_widget_groupbox.h"

#if LE_GROUPBOX_WIDGET_ENABLED

#include "gfx/legato/string/legato_stringutils.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define OUTLINE_SPACE 2

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_OUTLINE,
    DRAW_STRING,
    WAIT_STRING,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

void _leGroupBoxWidget_GetTextRect(const leGroupBoxWidget* box,
								   leRect* textRect,
								   leRect* drawRect)
{
    leRect bounds;
    
    *textRect = leRect_Zero;
    
    if(box->string == NULL)
        return;
    
    box->fn->localRect(box, &bounds);
    
    box->string->fn->getRect(box->string, textRect);
    
    //boundRect = box->widget.rect;
    bounds.x += (OUTLINE_SPACE * 3) + 2;
    bounds.y += OUTLINE_SPACE;
    bounds.width -= ((OUTLINE_SPACE * 3) + 2) * 2;
    bounds.height -= OUTLINE_SPACE * 2;
    
    // arrange relative to image rect
    leUtils_ArrangeRectangleRelative(textRect,
                                     leRect_Zero,
                                     bounds,
                                     box->widget.style.halign,
                                     LE_VALIGN_TOP,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0);
    
    leRectClip(textRect, &bounds, drawRect);
    
    // move the rect to layer space   
    leUtils_RectToScreenSpace((leWidget*)box, textRect);
    leUtils_RectToScreenSpace((leWidget*)box, drawRect);
}

static void drawBackground(leGroupBoxWidget* box);
static void drawOutline(leGroupBoxWidget* box);
static void drawString(leGroupBoxWidget* box);
static void drawBorder(leGroupBoxWidget* box);

static void nextState(leGroupBoxWidget* box)
{
    switch(box->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(box->fn->getCumulativeAlphaEnabled(box) == LE_TRUE)
            {
                paintState.alpha = box->fn->getCumulativeAlphaAmount(box);
            }
#endif
            
            if(box->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                box->widget.status.drawState = DRAW_BACKGROUND;
                box->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            box->widget.status.drawState = DRAW_OUTLINE;
            box->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawOutline;

            return;
        }
        case DRAW_OUTLINE:
        {            
            if(box->string != NULL)
            {
                box->widget.status.drawState = DRAW_STRING;
                box->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;

                return;
            }
        }
        // fall through
        case DRAW_STRING:
        {
            if(box->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                box->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                box->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            box->widget.status.drawState = DONE;
            box->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leGroupBoxWidget* box)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)box,
                                                paintState.alpha);

    nextState(box);
}

static void drawLine(leRect* rect,
                     int32_t x1,
                     int32_t y1,
                     int32_t x2,
                     int32_t y2,
                     leColor clr)
{
    leRect lineRect, clipRect;
    
    lineRect.x = x1;
    lineRect.y = y1;
    lineRect.width = x2 - x1;
    lineRect.height = y2 - y1;
    
    if(leRectIntersects(&lineRect, rect) == LE_FALSE)
        return;

    leRectClip(&lineRect, rect, &clipRect);
    
    leRenderer_DrawLine(clipRect.x,
                        clipRect.y,
                        clipRect.x + clipRect.width,
                        clipRect.y + clipRect.height,
                        clr,
                        paintState.alpha);
}

static void drawOutline(leGroupBoxWidget* box)
{
    leRect widgetRect, textRect; 
    int32_t top, bottom, left, right;
    
    box->fn->rectToScreen(box, &widgetRect);
    
    if(box->string != NULL)
    {
        box->string->fn->getRect(box->string, &textRect);
    }
    else
    {
        textRect = leRect_Zero;
    }
    
    // draw vertical lines
    top = widgetRect.y + textRect.height / 2;   
    bottom = widgetRect.y + widgetRect.height - OUTLINE_SPACE;

    // left outer line
    drawLine(&widgetRect,
             widgetRect.x + OUTLINE_SPACE,
             top,
             widgetRect.x + OUTLINE_SPACE,
             bottom,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_SHADOW));
    
    // right inner line
    drawLine(&widgetRect,
             widgetRect.x + box->widget.rect.width - OUTLINE_SPACE - 2,
             top,
             widgetRect.x + box->widget.rect.width - OUTLINE_SPACE - 2,
             bottom - 1,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_SHADOW));
             
    // left inner line
    drawLine(&widgetRect,
             widgetRect.x + OUTLINE_SPACE + 1,
             top + 1,
             widgetRect.x + OUTLINE_SPACE + 1,
             bottom - 1,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT));
             
    // right outer line
    drawLine(&widgetRect,
             widgetRect.x + box->widget.rect.width - OUTLINE_SPACE - 1,
             top,
             widgetRect.x + box->widget.rect.width - OUTLINE_SPACE - 1,
             bottom,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT));
             
    if(box->widget.style.halign == LE_HALIGN_LEFT)
    {   
        left = OUTLINE_SPACE * 3;
        right = left + textRect.width + 1;
        
        if(right > box->widget.rect.width - OUTLINE_SPACE * 3)
        {
            right = box->widget.rect.width - OUTLINE_SPACE * 3;
        }
    }
    else if(box->widget.style.halign == LE_HALIGN_CENTER)
    {
        left = box->widget.rect.width / 2 - textRect.width / 2 - 2;
        right = left + textRect.width + 3;
    }
    else
    {
        right = box->widget.rect.width - OUTLINE_SPACE * 3;
        left = right - textRect.width - 1;
        
        if(left < OUTLINE_SPACE * 3)
        {
            left = OUTLINE_SPACE * 3;
        }
    }
             
    // draw horizontal lines
    left += widgetRect.x;
    right += widgetRect.x;
    
    // top outer line left segment
    drawLine(&widgetRect,
             widgetRect.x + OUTLINE_SPACE + 1,
             top,
             left,
             top,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_SHADOW));
             
    // top outer line right segment
    drawLine(&widgetRect,
             right,
             top,
             widgetRect.x + widgetRect.width - OUTLINE_SPACE * 2,
             top,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_SHADOW));
                 
    // bottom inner line
    drawLine(&widgetRect,
             widgetRect.x + OUTLINE_SPACE + 1,
             bottom - 1,
             widgetRect.x + widgetRect.width - (OUTLINE_SPACE * 2),
             bottom - 1,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_SHADOW));
             
    // top inner line left segment
    drawLine(&widgetRect,
             widgetRect.x + OUTLINE_SPACE * 2,
             top + 1,
             left,
             top + 1,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT));
             
    // top inner line right segment
    drawLine(&widgetRect,
             right,
             top + 1,
             widgetRect.x + widgetRect.width - (OUTLINE_SPACE * 2) - 1,
             top + 1,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT));
             
    // bottom outer line
    drawLine(&widgetRect,
             widgetRect.x + OUTLINE_SPACE,
             bottom,
             widgetRect.x + widgetRect.width - (OUTLINE_SPACE * 2),
             bottom,
             leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT));
           
    nextState(box);
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leGroupBoxWidget* box = (leGroupBoxWidget*)strm->userData;

    box->widget.status.drawState = DRAW_STRING;

    nextState(box);
}
#endif

static void drawString(leGroupBoxWidget* box)
{
    leRect textRect, drawRect;
    
    _leGroupBoxWidget_GetTextRect(box, &textRect, &drawRect);

    leStringUtils_KerningRect((leRasterFont*)box->string->fn->getFont(box->string),
                              &textRect);
        
    box->string->fn->_draw(box->string,
                           textRect.x,
                           textRect.y,
                           box->widget.style.halign,
                           leScheme_GetRenderColor(box->widget.scheme, LE_SCHM_TEXT),
                           paintState.alpha);

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onStringStreamFinished;
        leGetActiveStream()->userData = box;

        box->widget.status.drawState = WAIT_STRING;

        return;
    }
#endif
    
    nextState(box);
}

static void drawBorder(leGroupBoxWidget* box)
{
    if(box->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)box,
                                                    paintState.alpha);
    }
    else if(box->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)box,
                                                      paintState.alpha);
    }
        
    nextState(box);
}

void _leGroupBoxWidget_Paint(leGroupBoxWidget* box)
{
    if(box->widget.status.drawState == NOT_STARTED)
    {
        nextState(box);
    }
    
    while(box->widget.status.drawState != DONE)
    {
        box->widget.drawFunc((leWidget*)box);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
        
#if LE_STREAMING_ENABLED == 1
        if(box->widget.status.drawState == WAIT_STRING)
            break;
#endif
    }
}

#endif // LE_GROUPBOX_WIDGET_ENABLED
