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


#include "gfx/legato/widget/window/legato_widget_window.h"

#if LE_WINDOW_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/string/legato_stringutils.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define DEFAULT_BAR_HEIGHT 20

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_TITLE_BAR,
    DRAW_ICON,
#if LE_STREAMING_ENABLED == 1
    WAIT_ICON,
#endif
    DRAW_STRING,
#if LE_STREAMING_ENABLED == 1
    WAIT_STRING,
#endif
    DRAW_BORDER,
};

struct
{
    uint32_t alpha;
} paintState;

void _leWindowWidget_GetTextRect(const leWindowWidget* win,
                                 leRect* textRect,
                                 leRect* drawRect);

void _leWindowWidget_GetTitleBarRect(const leWindowWidget* win,
                                     leRect* barRect)
{
    barRect->x = win->widget.margin.left;
    
    barRect->y = win->widget.margin.top;
    
    barRect->width = win->widget.rect.width -
                     win->widget.margin.left -
                     win->widget.margin.right;
                     
    barRect->height = win->titleHeight;
}

void _leWindowWidget_GetIconRect(const leWindowWidget* win,
                                 leRect* imgRect,
                                 leRect* imgSrcRect)
{
#if 0
    leRect barRect;
    
    imgRect->x = 0;
    imgRect->y = 0;
    
    if(win->icon == NULL)
    {
        imgRect->width = 0;
        imgRect->height = 0;
        
        imgSrcRect->x = 0;
        imgSrcRect->y = 0;
        imgSrcRect->width = 0;
        imgSrcRect->height = 0;
    
        return;
    }
    
    imgRect->width = win->icon->buffer.size.width;
    imgRect->height = win->icon->buffer.size.height;
    
    *imgSrcRect = *imgRect;
    
    _leWindowWidget_GetTitleBarRect(win, &barRect);
    
    barRect.y += 2;
    barRect.height -= 4;

    leUtils_ArrangeRectangleRelative(imgRect,
                                     leRect_Zero,
                                     barRect,
                                     LE_HALIGN_LEFT,
                                     LE_VALIGN_MIDDLE,
                                     LE_RELATIVE_POSITION_LEFTOF,
                                     win->widget.margin.left,
                                     win->widget.margin.top,
                                     win->widget.margin.right,
                                     win->widget.margin.bottom,
                                     win->iconMargin);
                                     
    leRectClipAdj(imgRect, &barRect, imgSrcRect, imgRect);
    
    leUtils_RectToScreenSpace((leWidget*)win, imgRect);
#endif
    leRect textRect = leRect_Zero;
    leRect bounds;
    int32_t x, y;

    _leWindowWidget_GetTitleBarRect(win, &bounds);

    //win->fn->localRect(win, &bounds);

    imgRect->x = 0;
    imgRect->y = 0;

    if(win->title != NULL)
    {
        win->title->fn->getRect(win->title, &textRect);
    }

    //leString_GetMultiLineRect(&win->string, &textRect, -1);

    imgRect->width = win->icon->buffer.size.width;
    imgRect->height = win->icon->buffer.size.height;

    *imgSrcRect = *imgRect;

    leUtils_ArrangeRectangle(imgRect,
                             textRect,
                             bounds,
                             win->widget.style.halign,
                             win->widget.style.valign,
                             LE_RELATIVE_POSITION_LEFTOF,
                             win->widget.margin.left,
                             win->widget.margin.top,
                             win->widget.margin.right,
                             win->widget.margin.bottom,
                             win->iconMargin);

    x = imgRect->x;
    y = imgRect->y;

    imgSrcRect->x += x;
    imgSrcRect->y += y;

    leRectClip(imgSrcRect, &bounds, imgSrcRect);

    imgSrcRect->x -= x;
    imgSrcRect->y -= y;

    leRectClip(imgRect, &bounds, imgRect);

    // move the rect to screen space
    leUtils_RectToScreenSpace((leWidget*)win, imgRect);
}

void _leWindowWidget_GetTextRect(const leWindowWidget* win,
                                 leRect* textRect,
                                 leRect* drawRect)
{
    leRect barRect, imageRect;
    
    *textRect = leRect_Zero;
    
    if(win->title == NULL)
        return;
    
    _leWindowWidget_GetTitleBarRect(win, &barRect);
    
    win->title->fn->getRect(win->title, textRect);
    
    imageRect = leRect_Zero;
    
    if(win->icon != NULL)
    {
        imageRect.width = win->icon->buffer.size.width;
        imageRect.height = win->icon->buffer.size.height;
    }
    
    // arrange relative to image rect
    leUtils_ArrangeRectangleRelative(textRect,
                                     imageRect,
                                     barRect,
                                     LE_HALIGN_LEFT,
                                     LE_VALIGN_MIDDLE,
                                     LE_RELATIVE_POSITION_LEFTOF,
                                     win->widget.margin.left,
                                     win->widget.margin.top,
                                     win->widget.margin.right,
                                     win->widget.margin.bottom,
                                     win->iconMargin);
                                      
    leRectClip(textRect, &barRect, drawRect);
    
    leUtils_RectToScreenSpace((leWidget*)win, drawRect);

    leStringUtils_KerningRect((leRasterFont*)win->title->fn->getFont(win->title),
                              drawRect);
}

static void drawBackground(leWindowWidget* win);
static void drawTitleBar(leWindowWidget* win);
static void drawIcon(leWindowWidget* win);
static void drawString(leWindowWidget* win);
static void drawBorder(leWindowWidget* win);

static void nextState(leWindowWidget* win)
{
    switch(win->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(win->fn->getCumulativeAlphaEnabled(win) == LE_TRUE)
            {
                paintState.alpha = win->fn->getCumulativeAlphaAmount(win);
            }
#endif

            if(win->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                win->widget.status.drawState = DRAW_BACKGROUND;
                win->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            win->widget.status.drawState = DRAW_TITLE_BAR;
            win->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawTitleBar;
            
            return;
        }
        case DRAW_TITLE_BAR:
        {
            if(win->icon != NULL)
            {
                win->widget.status.drawState = DRAW_ICON;
                win->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawIcon;
                
                return;
            }
        }
        // fall through
        case DRAW_ICON:
        {
            if(win->title != NULL)
            {
                win->widget.status.drawState = DRAW_STRING;
                win->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;
                
                return;
            }
        }
        // fall through
        case DRAW_STRING:
        {
            if(win->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                win->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                win->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            win->widget.status.drawState = DONE;
            win->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leWindowWidget* win)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)win,
                                                paintState.alpha);
    
    nextState(win);
}

static void drawTitleBar(leWindowWidget* win)
{
    leRect barRect;
    
    _leWindowWidget_GetTitleBarRect(win, &barRect);
    leUtils_RectToScreenSpace((leWidget*)win, &barRect);
    
    leRenderer_RectFill(&barRect,
                        leScheme_GetRenderColor(win->widget.scheme, LE_SCHM_BACKGROUND),
                        paintState.alpha);
    
    nextState(win);
}

#if LE_STREAMING_ENABLED == 1
static void onImageStreamFinished(leStreamManager* dec)
{
    leWindowWidget* win = (leWindowWidget*)dec->userData;

    win->widget.status.drawState = DRAW_ICON;

    nextState(win);
}
#endif

static void drawIcon(leWindowWidget* win)
{
    leRect iconRect, imgSrcRect;
    
    _leWindowWidget_GetIconRect(win, &iconRect, &imgSrcRect);
       
    leImage_Draw(win->icon,
                 &imgSrcRect,
                 iconRect.x,
                 iconRect.y,
                 paintState.alpha);

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onImageStreamFinished;
        leGetActiveStream()->userData = win;

        win->widget.status.drawState = WAIT_ICON;

        return;
    }
#endif
        
    nextState(win);
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leWindowWidget* win = (leWindowWidget*)strm->userData;

    win->widget.status.drawState = DRAW_STRING;

    nextState(win);
}
#endif

static void drawString(leWindowWidget* win)
{
    leRect textRect, drawRect;
    
    _leWindowWidget_GetTextRect(win, &textRect, &drawRect);
    
    win->title->fn->_draw(win->title,
                          drawRect.x,
                          drawRect.y,
                          win->widget.style.halign,
                          leScheme_GetRenderColor(win->widget.scheme, LE_SCHM_TEXT),
                          paintState.alpha);

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onStringStreamFinished;
        leGetActiveStream()->userData = win;

        win->widget.status.drawState = WAIT_STRING;

        return;
    }
#endif
    
    nextState(win);
}

static void drawBorder(leWindowWidget* win)
{
    leRect rect;
    
    if(win->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)win,
                                                    paintState.alpha);
    }
    else if(win->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        win->fn->localRect(win, &rect);
        
        leUtils_RectToScreenSpace((leWidget*)win, &rect);
        
        leWidget_SkinClassic_Draw2x2BeveledBorder(&rect,
                                                  leScheme_GetRenderColor(win->widget.scheme, LE_SCHM_BASE),
                                                  leScheme_GetRenderColor(win->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                                  leScheme_GetRenderColor(win->widget.scheme, LE_SCHM_SHADOWDARK),
                                                  leScheme_GetRenderColor(win->widget.scheme, LE_SCHM_SHADOW),
                                                  paintState.alpha);
    }
    
    nextState(win);
}

void _leWindowWidget_Paint(leWindowWidget* win)
{
    if(win->widget.status.drawState == NOT_STARTED)
        nextState(win);

#if LE_STREAMING_ENABLED == 1
    if(win->widget.status.drawState == WAIT_ICON ||
       win->widget.status.drawState == WAIT_STRING)
    {
        return;
    }
#endif
    
    while(win->widget.status.drawState != DONE)
    {
        win->widget.drawFunc((leWidget*)win);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
        
#if LE_STREAMING_ENABLED == 1
        if(win->widget.status.drawState == WAIT_ICON ||
           win->widget.status.drawState == WAIT_STRING)
            break;
#endif
    }
}

#endif // LE_WINDOW_WIDGET_ENABLED