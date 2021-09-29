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


#include "gfx/legato/widget/list/legato_widget_list.h"

#if LE_LIST_WIDGET_ENABLED == 1 && LE_SCROLLBAR_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/string/legato_stringutils.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_STRING,
    WAIT_STRING,
    DRAW_ICON,
    WAIT_ICON,
    DRAW_BORDER,
};

static struct
{
    leListItem* item;
    leRect itemRect;

    int32_t y;

    int32_t nextItem;

    uint32_t alpha;
} paintState;

int32_t _leListWidget_GetRowY(const leListWidget* lst, uint32_t idx)
{
    uint32_t row;
    int32_t y = 0;
    leListItem* item;
    
    //if(idx == 0)
    //    return 0;
    
    for(row = 0; row < idx; row++)
    {
        // get list item
        item = lst->items.values[row];
        
        y += item->rowRect.height;
    }

    y -= lst->scrollbar->value;
    
    return y;
}

void _leListWidget_GetListRect(const leListWidget* lst,
                               leRect* rect)
{
    *rect = lst->widget.rect;
    
    //if(lst->widget
    
    if(lst->widget.style.borderType == LE_WIDGET_BORDER_NONE)
    {
        if(LE_TEST_FLAG(lst->scrollbar->widget.flags, LE_WIDGET_VISIBLE) == LE_TRUE)
        {
            rect->width -= lst->scrollbar->widget.rect.width;
        }
    }
    else if(lst->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        rect->x += 1;
        rect->width -= 1;
        
        if(LE_TEST_FLAG(lst->scrollbar->widget.flags, LE_WIDGET_VISIBLE) == LE_TRUE)
        {
            rect->width -= lst->scrollbar->widget.rect.width;
        }
        else
        {
            rect->width -= 1;
        }
            
        rect->y += 1;
        rect->height -= 2;
    }
    else if(lst->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        rect->x += 2;
        rect->width -= 2;
        
        if(LE_TEST_FLAG(lst->scrollbar->widget.flags, LE_WIDGET_VISIBLE) == LE_TRUE)
        {
            rect->width -= lst->scrollbar->widget.rect.width;
        }
        else
        {
            rect->width -= 2;
        }
            
        rect->y += 2;
        rect->height -= 4;
    }
}

void _leListWidget_GetRowRect(const leListWidget* lst,
                              uint32_t idx,
                              leRect* rect)
{
    leRect textRect = leRect_Zero;
    
    leListItem* item;
    
    // get list item
    item = lst->items.values[idx];

    // get text rectangle
    if(item->string != NULL)
    {
        item->string->fn->getRect(item->string, &textRect);
    }

    // craft item entry rectangle
    rect->x = 0;
    rect->width = lst->widget.rect.width;
    
    if(LE_TEST_FLAG(lst->scrollbar->widget.flags, LE_WIDGET_VISIBLE) == LE_TRUE)
    {
        rect->width -= lst->scrollbar->widget.rect.width;
    }
        
    rect->y = _leListWidget_GetRowY(lst, idx);
    rect->height = item->rowRect.height;

    leUtils_RectToScreenSpace((leWidget*)lst, rect);
}

void _leListWidget_GetTextRects(const leListWidget* lst,
                                uint32_t idx,
                                leRect* boundingRect,
                                leRect* kerningRect)
{
    leRect rowRect, imageRect;
    int32_t y;
    
    leListItem* item;
    
    // get list item
    item = lst->items.values[idx];

    // get text rectangle
    if(item->string != NULL)
    {
        item->string->fn->getRect(item->string, boundingRect);

        *kerningRect = *boundingRect;

        leStringUtils_KerningRect((leRasterFont*)item->string->fn->getFont(item->string),
                                  kerningRect);
    }
    else
    {
        *boundingRect = leRect_Zero;
        *kerningRect = *boundingRect;
    }

    // craft item entry rectangle
    rowRect.x = 0;
    rowRect.width = lst->widget.rect.width;
    
    if(LE_TEST_FLAG(lst->scrollbar->widget.flags, LE_WIDGET_VISIBLE) == LE_TRUE)
    {
        rowRect.width -= lst->scrollbar->widget.rect.width;
    }
        
    rowRect.y = 0;
    rowRect.height = item->rowRect.height;
                      
    imageRect = leRect_Zero;
    
    if(item->icon != NULL)
    {
        imageRect.width = item->icon->buffer.size.width;
        imageRect.height = item->icon->buffer.size.height;
    }
    
    // arrange relative to image rect
    leUtils_ArrangeRectangleRelative(kerningRect,
                                     imageRect,
                                     rowRect,
                                     lst->widget.style.halign,
                                     LE_VALIGN_MIDDLE,
                                     lst->iconPos,
                                     lst->widget.margin.left,
                                     lst->widget.margin.top,
                                     lst->widget.margin.right,
                                     lst->widget.margin.bottom,
                                     lst->iconMargin);

    boundingRect->x = kerningRect->x;
    boundingRect->y = kerningRect->y;

    leRectClip(kerningRect, &rowRect, kerningRect);
                                     
    leUtils_RectToScreenSpace((leWidget*)lst, kerningRect);
    
    y = _leListWidget_GetRowY(lst, idx);

    boundingRect->y += y;
    kerningRect->y += y;
}

void _leListWidget_GetIconRect(const leListWidget* lst,
                               uint32_t idx,
                               leRect* iconRect,
                               leRect* imgSrcRect)
{
    leRect rowRect, textRect;
    leListItem* item;
    
    textRect = leRect_Zero;
    
    // get list item
    item = lst->items.values[idx];

    // get text rectangle
    if(item->string != NULL)
    {
        item->string->fn->getRect(item->string, &textRect);
    }
    
    // craft item entry rectangle
    rowRect.x = 0;
    rowRect.width = lst->widget.rect.width;
    
    if(LE_TEST_FLAG(lst->scrollbar->widget.flags, LE_WIDGET_VISIBLE) == LE_TRUE)
    {
        rowRect.width -= lst->scrollbar->widget.rect.width;
    }
        
    rowRect.y = 0;
    rowRect.height = item->rowRect.height;
                      
    *iconRect = leRect_Zero;
    
    if(item->icon != NULL)
    {
        iconRect->width = item->icon->buffer.size.width;
        iconRect->height = item->icon->buffer.size.height;
    }
    
    *imgSrcRect = *iconRect;
    
    // arrange relative to image rect
    leUtils_ArrangeRectangle(iconRect,
                             textRect,
                             rowRect,
                             lst->widget.style.halign,
                             LE_VALIGN_MIDDLE,
                             lst->iconPos,
                             lst->widget.margin.left,
                             lst->widget.margin.top,
                             lst->widget.margin.right,
                             lst->widget.margin.bottom,
                             lst->iconMargin);
                             
    leRectClipAdj(iconRect, &rowRect, imgSrcRect, iconRect);
    
    leUtils_RectToScreenSpace((leWidget*)lst, iconRect);
    
    iconRect->y += _leListWidget_GetRowY(lst, idx);
}

void _leListWidget_RecalculateRowRect(leListWidget* lst,
                                      uint32_t idx)
{
    leListItem* item;
    leRect textRect = leRect_Zero;

    // get list item
    item = lst->items.values[idx];

    // get text rectangle
    if(item->string != NULL)
    {
        item->string->fn->getRect(item->string, &textRect);
    }

    // craft item entry rectangle
    item->rowRect.x = 0;
    item->rowRect.width = lst->widget.rect.width;

    if(LE_TEST_FLAG(lst->scrollbar->widget.flags, LE_WIDGET_VISIBLE) == LE_TRUE)
    {
        item->rowRect.width -= lst->scrollbar->widget.rect.width;
    }
    
    item->rowRect.y = _leListWidget_GetRowY(lst, idx);

    if (item->icon != NULL && item->icon->buffer.size.height > textRect.height)
    {
        item->rowRect.height = item->icon->buffer.size.height +
                       lst->widget.margin.top +
                       lst->widget.margin.bottom;
    }
    else
    {
        item->rowRect.height = textRect.height +
                       lst->widget.margin.top +
                       lst->widget.margin.bottom;
    }

    leUtils_RectToScreenSpace((leWidget*)lst, &item->rowRect);
}

void _leListWidget_GetLogicalRect(leListWidget* lst, leRect* rect)
{
    uint32_t idx;
    leRect row;
    
    rect->x = 0;
    rect->y = 0;
    rect->width = lst->widget.rect.width;
    rect->height = 0;
    
    for(idx = 0; idx < lst->items.size; idx++)
    {
        _leListWidget_GetRowRect(lst, idx, &row);
        
        rect->height += row.height;
    }
}

static void drawBackground(leListWidget* lst);
static void drawString(leListWidget* lst);
static void drawIcon(leListWidget* lst);
static void drawBorder(leListWidget* lst);

static void nextState(leListWidget* lst)
{
    switch(lst->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(lst->fn->getCumulativeAlphaEnabled(lst) == LE_TRUE)
            {
                paintState.alpha = lst->fn->getCumulativeAlphaAmount(lst);
            }
#endif

            lst->widget.status.drawState = DRAW_BACKGROUND;
            lst->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

            return;
        }
        case DRAW_BACKGROUND:
        {
            if(lst->items.size > 0)
            {
                paintState.nextItem = 0;
                paintState.y = 0;
                
                lst->widget.status.drawState = DRAW_STRING;
                lst->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;
            
                return;
            }
        }
        // fall through
        case DRAW_STRING:
        {            
            if(lst->items.size > 0)
            {
                paintState.nextItem = 0;
                paintState.y = 0;
                
                lst->widget.status.drawState = DRAW_ICON;
                lst->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawIcon;
            
                return;
            }
        }
        // fall through
        case DRAW_ICON:
        {
            if(lst->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                lst->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                lst->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            lst->widget.status.drawState = DONE;
            lst->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leListWidget* lst)
{
    int32_t i;
    leRect widgetRect, rowRect;
    leColor clr;
    leListItem* item;
    
    // draw widget background
    if(lst->widget.style.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        leWidget_SkinClassic_DrawBackground((leWidget*)lst,
                                            leScheme_GetRenderColor(lst->widget.scheme, LE_SCHM_BACKGROUND),
                                            paintState.alpha);
    }
    
    lst->fn->rectToScreen(lst, &widgetRect);

    // draw item highlights
    if(lst->items.size > 0)
    {
        for(i = 0; i < (int32_t)lst->items.size; i++)
        {
            item = lst->items.values[i];
            
            _leListWidget_GetRowRect(lst, i, &rowRect);
            
            if(leRectIntersects(&rowRect, &widgetRect) == LE_TRUE)
            {        
                if(i == lst->itemDown && item->enabled == LE_TRUE)
                {
                    clr = leScheme_GetRenderColor(lst->widget.scheme, LE_SCHM_BACKGROUND_INACTIVE);
                }
                else if(item->selected == LE_TRUE)
                {
                    clr = leScheme_GetRenderColor(lst->widget.scheme, LE_SCHM_TEXT_HIGHLIGHT);
                }
                else
                {
                    continue;
                }
                                    
                // clip again to the widget bounds
                leRenderer_RectFill(&rowRect,
                                    clr,
                                    paintState.alpha);
            }
        }
    }
    
    nextState(lst);
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leListWidget* lst = (leListWidget*)strm->userData;

    paintState.nextItem++;

    lst->widget.status.drawState = DRAW_STRING;
    lst->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;
}
#endif

static void drawString(leListWidget* lst)
{
    leRect boundingRect, kerningRect;
    leColor clr;
    
    if(paintState.nextItem == (int32_t)lst->items.size)
    {
        nextState(lst);

        return;
    }

    paintState.item = lst->items.values[paintState.nextItem];

    if(paintState.item->string != NULL)
    {
        // get rectangles
        paintState.item = lst->items.values[paintState.nextItem];

        _leListWidget_GetTextRects(lst,
                                   paintState.nextItem,
                                   &boundingRect,
                                   &kerningRect);

        if(paintState.nextItem == lst->itemDown || paintState.item->selected == LE_FALSE)
        {
            if(paintState.item->enabled == LE_TRUE)
            {
                clr = leScheme_GetRenderColor(lst->widget.scheme, LE_SCHM_TEXT);
            }
            else
            {
                clr = leScheme_GetRenderColor(lst->widget.scheme, LE_SCHM_TEXT_DISABLED);
            }
        }
        else
        {
            clr = leScheme_GetRenderColor(lst->widget.scheme, LE_SCHM_TEXT_HIGHLIGHTTEXT);
        }

        paintState.item->string->fn->_draw(paintState.item->string,
                                           kerningRect.x,
                                           kerningRect.y,
                                           LE_HALIGN_CENTER,
                                           clr,
                                           paintState.alpha);

#if LE_STREAMING_ENABLED == 1
        if(leGetActiveStream() != NULL)
        {
            leGetActiveStream()->onDone = onStringStreamFinished;
            leGetActiveStream()->userData = lst;

            lst->widget.status.drawState = WAIT_STRING;

            return;
        }
#endif
    }
    
    paintState.nextItem++;
}

#if LE_STREAMING_ENABLED == 1
static void onImageStreamFinished(leStreamManager* strm)
{
    leListWidget* lst = (leListWidget*)strm->userData;

    lst->widget.status.drawState = DRAW_ICON;

    nextState(lst);
}
#endif

static void drawIcon(leListWidget* lst)
{
    leRect imgRect, imgSrcRect;
    
    if(paintState.nextItem == (int32_t)lst->items.size)
    {
        nextState(lst);

        return;
    }
    
    paintState.item = lst->items.values[paintState.nextItem];
    
    // skip if no icon
    if(paintState.item->icon == NULL)
    {
        paintState.nextItem++;
        
        if(paintState.nextItem == (int32_t)lst->items.size)
        {
            nextState(lst);
        }
        
        return;
    }
    
    _leListWidget_GetIconRect(lst, paintState.nextItem, &imgRect, &imgSrcRect);
    
    leImage_Draw(paintState.item->icon,
                 &imgSrcRect,
                 imgRect.x,
                 imgRect.y,
                 paintState.alpha);
    
#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onImageStreamFinished;
        leGetActiveStream()->userData = lst;

        lst->widget.status.drawState = WAIT_ICON;

        return;
    }
#endif
    
    paintState.nextItem++;
}

static void drawBorder(leListWidget* lst)
{
    // kind of a hack to make sure the border types stay in sync
    // without having to create an event to override
    lst->scrollbar->fn->setBackgroundType(lst->scrollbar, lst->widget.style.borderType);
   
    if(lst->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)lst,
                                                    paintState.alpha);
    }
    else if(lst->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardLoweredBorder((leWidget*)lst,
                                                       paintState.alpha);
    }
    
    nextState(lst);
}

void _leListWidget_Paint(leListWidget* lst)
{
    if(lst->widget.status.drawState == NOT_STARTED)
    {
        nextState(lst);
    }
    
    while(lst->widget.status.drawState != DONE)
    {
        lst->widget.drawFunc((leWidget*)lst);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
   
#if LE_STREAMING_ENABLED == 1
        if(lst->widget.status.drawState == WAIT_STRING ||
           lst->widget.status.drawState == WAIT_ICON)
            break;
#endif
    }
}

#endif // LE_LIST_WIDGET_ENABLED && LE_SCROLLBAR_WIDGET_ENABLED
