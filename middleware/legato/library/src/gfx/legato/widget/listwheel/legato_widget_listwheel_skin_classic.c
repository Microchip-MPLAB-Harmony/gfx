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


#include "gfx/legato/widget/listwheel/legato_widget_listwheel.h"
#include "gfx/legato/common/legato_math.h"

#if LE_LISTWHEEL_WIDGET_ENABLED

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/string/legato_stringutils.h"
#include "gfx/legato/string/legato_string_renderer.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_STRING,
#if LE_STREAMING_ENABLED == 1
    WAIT_STRING,
#endif
    DRAW_ICON,
#if LE_STREAMING_ENABLED == 1
    WAIT_ICON,
#endif
    DRAW_INDICATORS,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

static int32_t getItemY(const leListWheelWidget* whl,
                        int32_t pos,
                        int32_t itemHeight)
{
    int32_t y = pos * whl->paintState.per;
    
    return (y - (itemHeight / 2)) + whl->rotation;
}

void _leListWheelWidget_GetItemTextRects(const leListWheelWidget* whl,
                                         uint32_t idx,
                                         uint32_t pos,
                                         leRect* boundingRect,
                                         leRect* kerningRect)
{
    leRect bounds, imageRect;
    leListWheelItem* item;
    int32_t y;

    // get rectangles
    item = whl->items.values[idx];

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

    bounds.x = 0;
    bounds.y = 0;
    bounds.width = whl->widget.rect.width;
    bounds.height = boundingRect->height;

    imageRect = leRect_Zero;

    if(item->icon != NULL)
    {
        if(bounds.height < (int32_t)item->icon->buffer.size.height)
        {
            bounds.height = (int32_t)item->icon->buffer.size.height;
        }

        imageRect.width = item->icon->buffer.size.width;
        imageRect.height = item->icon->buffer.size.height;
    }

    // arrange relative to image rect
    leUtils_ArrangeRectangleRelative(kerningRect,
                                     imageRect,
                                     bounds,
                                     whl->widget.style.halign,
                                     LE_VALIGN_MIDDLE,
                                     whl->iconPos,
                                     whl->widget.margin.left,
                                     whl->widget.margin.top,
                                     whl->widget.margin.right,
                                     whl->widget.margin.bottom,
                                     whl->iconMargin);

    boundingRect->x = kerningRect->x;
    boundingRect->y = kerningRect->y;

    leRectClip(kerningRect, &bounds, kerningRect);

    // move the draw rect to layer space
    leUtils_RectToScreenSpace((leWidget*)whl, kerningRect);

    y = getItemY(whl, pos, bounds.height);

    boundingRect->y += y;
    kerningRect->y += y;
}

void _leListWheelWidget_GetIndicatorRect(leListWheelWidget* whl,
                                         leRect* res)
{
    leRect widgetRect;
    
    whl->fn->rectToScreen(whl, &widgetRect);
    
    res->x = widgetRect.x;
    res->y = widgetRect.y + widgetRect.height / 2 - whl->indicatorArea;
    res->width = widgetRect.width;
    res->height = whl->indicatorArea * 2;
}

void _leListWheelWidget_ItemRectApplyEffects(leListWheelWidget* whl,
                                             leRect itemRect,
                                             leRect* newItemRect)
{
    leRect midLineRect,widgetRect;
    int32_t pct = 100;
    
    whl->fn->rectToScreen(whl, &widgetRect);
    
    midLineRect.y = widgetRect.y + widgetRect.height/2;
    midLineRect.x = widgetRect.x;
    midLineRect.height = 1;
    midLineRect.width = widgetRect.width;
    
    newItemRect->height = itemRect.height;
    newItemRect->width = itemRect.width;
    
    if (whl->zoomEffects == LE_LISTWHEEL_ZOOM_EFFECT_FIXED_SCALE)
    {
            newItemRect->height = itemRect.height*2/3;
            newItemRect->width = itemRect.width*2/3;
    }
    else
    {
        pct = 100;

        if (itemRect.y > midLineRect.y)
        {
            pct = lePercentWholeRounded(widgetRect.y + widgetRect.height - itemRect.y, widgetRect.height/2);
        }
        else if (itemRect.y + itemRect.height < midLineRect.y)
        {
            pct = lePercentWholeRounded(itemRect.y + itemRect.height - widgetRect.y, widgetRect.height/2);
        }
        
        switch (whl->zoomEffects)
        {
            case LE_LISTWHEEL_ZOOM_EFFECT_VSCALE:
            {
                newItemRect->height = (itemRect.height * pct)/100;
                
                break;
            }
            case LE_LISTWHEEL_ZOOM_EFFECT_FULL_SCALE:
            {
                newItemRect->height = (itemRect.height * pct) / 100;
                newItemRect->width = (itemRect.width * pct) / 100;
                
                break;
            }
            default:
                break;
        }
    }
}

void _leListWheelWidget_GetItemIconRect(leListWheelWidget* whl,
                                        uint32_t idx,
                                        uint32_t pos,
                                        leRect* imgRect,
                                        leRect* imgSrcRect)
{
    leRect bounds = {0};
    leRect textRect = {0};
    leListWheelItem* item = whl->items.values[idx];
        
    const leImage* img = item->icon;
    
    imgRect->x = 0;
    imgRect->y = 0;
    imgRect->width = img->buffer.size.width;
    imgRect->height = img->buffer.size.height;
    
    if(item->string != NULL)
    {
        item->string->fn->getRect(item->string, &textRect);
    }
    else
    {
        textRect = leRect_Zero;
    }
    
    bounds.x = 0;
    bounds.y = 0;
    bounds.width = whl->widget.rect.width;
    bounds.height = textRect.height;
    
    if(item->icon != NULL)
    {
        if(bounds.height < (int32_t)item->icon->buffer.size.height)
        {
            bounds.height = (int32_t)item->icon->buffer.size.height;
        }
            
        imgRect->width = item->icon->buffer.size.width;
        imgRect->height = item->icon->buffer.size.height;
    }
    
    *imgSrcRect = *imgRect;
    
    // arrange image rect
    leUtils_ArrangeRectangle(imgRect,
                             textRect,
                             bounds,
                             whl->widget.style.halign,
                             LE_VALIGN_MIDDLE,
                             whl->iconPos,
                             whl->widget.margin.left,
                             whl->widget.margin.top,
                             whl->widget.margin.right,
                             whl->widget.margin.bottom,
                             whl->iconMargin);   
                             
    leRectClipAdj(imgRect, &bounds, imgSrcRect, imgRect);
    
    leUtils_RectToScreenSpace((leWidget*)whl, imgRect); 
    
    imgRect->y += getItemY(whl, pos, bounds.height);
    
    //if(pos == 1)
    //    printf("y=%i\n", getItemY(whl, pos, bounds.height));
}

void _leListWheelWidget_GetItemRect(leListWheelWidget* whl,
                                    uint32_t idx,
                                    uint32_t pos,
                                    leRect* itemRect)
{
    leRect textRect;
    leListWheelItem* item = whl->items.values[idx];

    *itemRect = leRect_Zero;
    
    if(item->string != NULL)
    {
        item->string->fn->getRect(item->string, &textRect);
    }
    else
    {
        textRect = leRect_Zero;
    }

    if(item->icon != NULL)
    {
        itemRect->height = item->icon->buffer.size.height;
    }

    itemRect->width = whl->widget.rect.width;
    
    if(textRect.height > itemRect->height)
    {
        itemRect->height = textRect.height;
    }
    
    leUtils_RectToScreenSpace((leWidget*)whl, itemRect); 
    
    itemRect->y += getItemY(whl, pos, itemRect->height);    
}

static void drawBackground(leListWheelWidget* whl);
static void drawString(leListWheelWidget* whl);
static void drawIcon(leListWheelWidget* whl);
static void drawIndicators(leListWheelWidget* whl);
static void drawBorder(leListWheelWidget* whl);

static void nextState(leListWheelWidget* whl)
{
    switch(whl->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(whl->fn->getCumulativeAlphaEnabled(whl) == LE_TRUE)
            {
                paintState.alpha = whl->fn->getCumulativeAlphaAmount(whl);
            }
#endif
            
            whl->paintState.per = leDivideRounding(whl->widget.rect.height, whl->visibleItems - 1);
            
            whl->widget.status.drawState = DRAW_BACKGROUND;
            whl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

            return;
        }
        case DRAW_BACKGROUND:
        {
            if(whl->items.size > 0)
            {
                whl->paintState.nextItem = whl->topItem;
                whl->paintState.y = -1;
                
                whl->widget.status.drawState = DRAW_STRING;
                whl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;
            
                return;
            }
        }
        // fall through
        case DRAW_STRING:
        {            
            //printf("\n");
            
            if(whl->items.size > 0)
            {
                whl->paintState.nextItem = whl->topItem;
                whl->paintState.y = -1;
                
                whl->widget.status.drawState = DRAW_ICON;
                whl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawIcon;
            
                return;
            }
        }
        // fall through
        case DRAW_ICON:
        {
            if(whl->showIndicators == LE_TRUE)
            {                
                whl->widget.status.drawState = DRAW_INDICATORS;
                whl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawIndicators;
            
                return;
            }
        }
        // fall through
        case DRAW_INDICATORS:
        {
            if(whl->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                whl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                whl->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            whl->widget.status.drawState = DONE;
            whl->widget.drawFunc = NULL;
        }
    }
}

static void nextItem(leListWheelWidget* whl)
{
    whl->paintState.nextItem++;
    whl->paintState.y++;
    
    if(whl->paintState.nextItem >= whl->items.size)
    {
        whl->paintState.nextItem = 0;
    }
}

static void drawBackground(leListWheelWidget* whl)
{
    leRect widgetRect, drawRect;
    
    //printf("drawbackground\n");
    if(whl->widget.style.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        if(whl->shaded == LE_TRUE)
        {
            whl->fn->rectToScreen(whl, &widgetRect);
            
            // upper rectangle
            drawRect.x = widgetRect.x;
            drawRect.y = widgetRect.y;
            drawRect.width = widgetRect.width;
            drawRect.height = (widgetRect.height / 2) - whl->indicatorArea;
            
            leRenderer_VertGradientRect(&drawRect,
                                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_BACKGROUND_DISABLED),
                                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_BACKGROUND_INACTIVE),
                                        paintState.alpha);
            
            // lower rectangle
            drawRect.y = (widgetRect.y + widgetRect.height / 2) + whl->indicatorArea;
            drawRect.height = (widgetRect.height / 2) - whl->indicatorArea;
            
            leRenderer_VertGradientRect(&drawRect,
                                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_BACKGROUND_INACTIVE),
                                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_BACKGROUND_DISABLED),
                                        paintState.alpha);
        }
        else
        {
            leWidget_SkinClassic_DrawBackground((leWidget*)whl,
                                                leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_BACKGROUND),
                                                paintState.alpha);
        }
    }
    
    if (whl->indicatorFill != LE_LISTWHEEL_INDICATOR_FILL_NONE)
    {
        whl->fn->rectToScreen(whl, &widgetRect);
            
        _leListWheelWidget_GetIndicatorRect(whl, &drawRect);
        
        if (whl->indicatorFill == LE_LISTWHEEL_INDICATOR_FILL_GRADIENT)
        {
            leRenderer_VertGradientRect(&drawRect,
                                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND_DISABLED),
                                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND_INACTIVE),
                                        paintState.alpha);
        }
        else
        {
            leRenderer_RectFill(&drawRect,
                                leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND_INACTIVE),
                                paintState.alpha);
        }
    }
   
    nextState(whl);    
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leListWheelWidget* whl = (leListWheelWidget*)strm->userData;

    nextItem(whl);

    whl->widget.status.drawState = DRAW_STRING;
    whl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;
}
#endif

static void drawString(leListWheelWidget* whl)
{
    leRect boundingRect, kerningRect, widgetRect;
    leListWheelItem* item;
    leRect midLineRect;
    leRect itemRect;
    leStringRenderRequest req;
    leColor background;
 
    //if(whl->paintState.nextItem < whl->items.size)
    //    nextItem(whl);
    
    if(!(whl->paintState.y < whl->visibleItems + 1))
    {
        nextState(whl);

        return;
    }
    
    item = whl->items.values[whl->paintState.nextItem];
    
    //printf("drawing item: %i, rot=%i\n", whl->paintState.nextItem, whl->rotation);
    
    if(item->string == NULL || item->string->fn->length(item->string) == 0)
    {
        nextItem(whl);
        
        return;
    }
    
    _leListWheelWidget_GetItemTextRects(whl,
                                        whl->paintState.nextItem,
                                        whl->paintState.y,
                                        &boundingRect,
                                        &kerningRect);

    whl->fn->rectToScreen(whl, &widgetRect);
    
    if(leRectIntersects(&kerningRect, &widgetRect) == LE_FALSE)
    {
        nextItem(whl);
        
        return;
    }
    
    item = whl->items.values[whl->paintState.nextItem];

    midLineRect.y = widgetRect.y + widgetRect.height/2;
    midLineRect.x = widgetRect.x;
    midLineRect.height = 1;
    midLineRect.width = widgetRect.width;
    
    _leListWheelWidget_GetItemRect(whl,
                                   whl->paintState.nextItem,
                                   whl->paintState.y,
                                   &itemRect);
    
    if (leRectIntersects(&itemRect, &midLineRect) == LE_TRUE)
    {
        if (whl->indicatorFill == LE_LISTWHEEL_INDICATOR_FILL_NONE && 
            whl->shaded == LE_FALSE)
        {
            background = leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_BACKGROUND);
            
            req.str = item->string;
            req.x = kerningRect.x;
            req.y = kerningRect.y;
            req.align = whl->widget.style.halign;
            req.color = leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_TEXT_HIGHLIGHTTEXT);
            req.alpha = paintState.alpha;
            req.lookupTable = leUtils_GetSchemeLookupTable(whl->widget.scheme,
                                                           req.color,
                                                           background);

            leStringRenderer_DrawString(&req);
        }
        else
        {
            item->string->fn->_draw(item->string,
                                    kerningRect.x,
                                    kerningRect.y,
                                    whl->widget.style.halign,
                                    leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_TEXT_HIGHLIGHTTEXT),
                                    paintState.alpha);
        }

#if LE_STREAMING_ENABLED == 1
        if(leGetActiveStream() != NULL)
        {
            leGetActiveStream()->onDone = onStringStreamFinished;
            leGetActiveStream()->userData = whl;

            whl->widget.status.drawState = WAIT_STRING;

            return;
        }
#endif
    }
    else if (whl->hideWheel == LE_FALSE)
    {
        if (whl->zoomEffects == LE_LISTWHEEL_ZOOM_EFFECT_NONE)
        {
            if (whl->indicatorFill == LE_LISTWHEEL_INDICATOR_FILL_NONE && 
                whl->shaded == LE_FALSE)
            {
                background = leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_BACKGROUND);
                req.str = item->string;
                req.x = kerningRect.x;
                req.y = kerningRect.y;
                req.align = whl->widget.style.halign;
                req.color = leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_TEXT);
                req.alpha = paintState.alpha;
                req.lookupTable = leUtils_GetSchemeLookupTable(whl->widget.scheme,
                                                               req.color,
                                                               background);

                leStringRenderer_DrawString(&req);                
            }
            else
            {
                item->string->fn->_draw(item->string,
                                        kerningRect.x,
                                        kerningRect.y,
                                        whl->widget.style.halign,
                                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_TEXT),
                                        paintState.alpha);
            }

#if LE_STREAMING_ENABLED == 1
            if(leGetActiveStream() != NULL)
            {
                leGetActiveStream()->onDone = onStringStreamFinished;
                leGetActiveStream()->userData = whl;

                whl->widget.status.drawState = WAIT_STRING;

                return;
            }
#endif
        }
        else
        {
#if 0
            lePixelBuffer pixelBuff;
            void * buf;
            GFX_BlendMode blendMode;
            leRect newTextRect = textRect;

            //Pre-render to a buffer
            buf = LE_MALLOC(
                                            textRect.width * 
                                            textRect.height * 
                                            4);
            if (buf != NULL)
            {
                lePixelBufferCreate(textRect.width,
                                      textRect.height,
                                      LE_COLOR_MODE_RGBA_8888,
                                      buf,
                                      &pixelBuff);
                memset(pixelBuff.pixels, 0x0, pixelBuff.buffer_length);

                GFX_Set(GFXF_DRAW_COLOR, leColorConvert(
                                            leContext_GetActive()->colorMode,
                                            LE_COLOR_MODE_RGBA_8888,
                                            leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_TEXT);

                GFX_Set(GFXF_DRAW_TARGET, &pixelBuff);

                leString_DrawClipped(&item->string,
                                     0,
                                     0,
                                     textRect.width,
                                     textRect.height,
                                     0,
                                     0,
                                     &whl->reader);

                if(whl->reader != NULL)
                {
                    whl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&waitString;
                    whl->widget.status.drawState = WAIT_STRING;

                    LE_FREE(buf);

                    return;
                }

                GFX_Set(GFXF_DRAW_TARGET, NULL);

                //Process alpha channel
                GFX_Set(GFXF_DRAW_RESIZE_MODE, GFX_RESIZE_BILINEAR);
                GFX_Get(GFXF_DRAW_BLEND_MODE, &blendMode);
                GFX_Set(GFXF_DRAW_BLEND_MODE, blendMode | GFX_BLEND_CHANNEL);

                //Apply scale effects on text rectangle
                _leListWheelWidget_ItemRectApplyEffects(whl,
                                                textRect,
                                                &newTextRect);

                if (leRectIntersects(&newTextRect,
                                       &layer->drawRect) == LE_TRUE)
                {
                    leRect imgRect = {0}, imgSrcRect, bounds;

                    if (item->icon != NULL)
                    {
                        _leListWheelWidget_GetItemIconRect(whl,
                                                           whl->paintState.nextItem,
                                                           whl->paintState.y,
                                                           &imgRect,
                                                           &imgSrcRect);

                        _leListWheelWidget_ItemRectApplyEffects(whl, 
                                                                imgRect,
                                                                &imgRect);
                    }

                    bounds.x = 0;
                    bounds.y = 0;
                    bounds.width = whl->widget.rect.width;
                    bounds.height = textRect.height;

                    // arrange image rect
                    leUtils_ArrangeRectangleRelative(&newTextRect,
                                                    imgRect,
                                                    bounds,
                                                    whl->halign,
                                                    LE_VALIGN_MIDDLE,
                                                    whl->iconPos,
                                                    whl->widget.margin.left,
                                                    whl->widget.margin.top,
                                                    whl->widget.margin.right,
                                                    whl->widget.margin.bottom,
                                                    whl->iconMargin);
                    
                    leUtils_RectToScreenSpace((leWidget*)whl, &newTextRect); 

                    newTextRect.y += getItemY(whl, whl->paintState.y, bounds.height);        

                    leRectClip(&newTextRect, 
                                 &layer->drawRect,
                                 &newTextRect);


                    GFX_DrawStretchBlit(&pixelBuff,
                                        0,
                                        0,
                                        pixelBuff.size.width,
                                        pixelBuff.size.height,
                                        newTextRect.x,
                                        newTextRect.y,
                                        newTextRect.width,
                                        newTextRect.height);
                }

                GFX_Set(GFXF_DRAW_BLEND_MODE, blendMode);

                LE_FREE(buf);
            }
#endif
        }
    }
    
    nextItem(whl);
}

#if LE_STREAMING_ENABLED == 1
static void onImageStreamFinished(leStreamManager* dec)
{
    leListWheelWidget* whl = (leListWheelWidget*)dec->userData;

    whl->widget.status.drawState = DRAW_ICON;

    nextState(whl);
}
#endif

static void drawIcon(leListWheelWidget* whl)
{
    leRect widgetRect, imgRect, imgSrcRect, clipRect;
    leRect midLineRect;
    leRect itemRect;
    //int32_t y;
    leListWheelItem* item = whl->items.values[whl->paintState.nextItem];
    
    if(!(whl->paintState.y < whl->visibleItems + 1))
    {
        nextState(whl);

        return;
    }
    
    // skip if no icon
    if(item->icon == NULL)
    {
        nextItem(whl);
        
        return;
    }
    
    _leListWheelWidget_GetItemIconRect(whl,
                                       whl->paintState.nextItem,
                                       whl->paintState.y,
                                       &imgRect,
                                       &imgSrcRect);

    whl->fn->rectToScreen(whl, &widgetRect);
    
    if(leRectIntersects(&imgRect, &widgetRect) == LE_TRUE)
    {
        // clip imgrect to widget rect
        leRectClipAdj(&imgRect, &widgetRect, &imgSrcRect, &imgRect);
        
        midLineRect.y = widgetRect.y + widgetRect.height/2;
        midLineRect.x = widgetRect.x;
        midLineRect.height = 1;
        midLineRect.width = widgetRect.width;

        leRectClipAdj(&imgRect, &widgetRect, &imgSrcRect, &clipRect);

        _leListWheelWidget_GetItemRect(whl,
                                   whl->paintState.nextItem,
                                   whl->paintState.y,
                                   &itemRect);

        if (leRectIntersects(&itemRect, &midLineRect) == LE_TRUE)
        {
            leImage_Draw(item->icon,
                         &imgSrcRect,
                         clipRect.x, clipRect.y,
                         paintState.alpha);

#if LE_STREAMING_ENABLED == 1
            if(leGetActiveStream() != NULL)
            {
                leGetActiveStream()->onDone = onImageStreamFinished;
                leGetActiveStream()->userData = whl;

                whl->widget.status.drawState = WAIT_ICON;

                return;
            }
#endif
        }
        else if (whl->hideWheel == LE_FALSE)
        {
            if (whl->zoomEffects == LE_LISTWHEEL_ZOOM_EFFECT_NONE)
            {
                leImage_Draw(item->icon,
                             &imgSrcRect,
                             clipRect.x, clipRect.y,
                             paintState.alpha);

#if LE_STREAMING_ENABLED == 1
                if(leGetActiveStream() != NULL)
                {
                    leGetActiveStream()->onDone = onImageStreamFinished;
                    leGetActiveStream()->userData = whl;

                    whl->widget.status.drawState = WAIT_ICON;

                    return;
                }
#endif
            }
            else
            {
#if 0
                lePixelBuffer pixelBuff;
                void * buf;
                GFX_BlendMode blendMode;
                leRect newImgRect = imgRect;

                //Pre-render to a buffer
                buf = LE_MALLOC(
                                                imgRect.width *
                                                imgRect.height *
                                                4);
                if (buf != NULL)
                {
                    lePixelBufferCreate(imgRect.width,
                                          imgRect.height,
                                          LE_COLOR_MODE_RGBA_8888,
                                          buf,
                                          &pixelBuff);
                    memset(pixelBuff.pixels, 0x0, pixelBuff.buffer_length);

                    GFX_Set(GFXF_DRAW_COLOR, leColorConvert(
                                                leContext_GetActive()->colorMode,
                                                LE_COLOR_MODE_RGBA_8888,
                                                leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND_TEXT));

                    GFX_Set(GFXF_DRAW_TARGET, &pixelBuff);

                    leDrawImage(item->icon,
                                   0,
                                   0,
                                   imgSrcRect.width,
                                   imgSrcRect.height,
                                   0,
                                   0,
                                   &leContext_GetActive()->memIntf,
                                   &whl->reader);

                    if(whl->reader != NULL)
                    {
                        whl->widget.status.drawState = WAIT_ICON;
                        whl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&waitIcon;

                        LE_FREE(buf);

                        return;
                    }

                    GFX_Set(GFXF_DRAW_TARGET, NULL);

                    //Process alpha channel
                    GFX_Set(GFXF_DRAW_RESIZE_MODE, GFX_RESIZE_BILINEAR);
                    GFX_Get(GFXF_DRAW_BLEND_MODE, &blendMode);
                    GFX_Set(GFXF_DRAW_BLEND_MODE, blendMode | GFX_BLEND_CHANNEL);

                    //Use draw blit to scale it up!
                    _leListWheelWidget_ItemRectApplyEffects(whl,
                                                    imgRect,
                                                    &newImgRect);

                    if (newImgRect.height != 0 &&
                        newImgRect.width != 0 &&
                        leRectIntersects(&newImgRect,
                                           &layer->drawRect) == LE_TRUE)
                    {
                        leRect textRect, textSrcRect, bounds;

                        _leListWheelWidget_GetItemTextRect(whl,
                                       whl->paintState.nextItem,
                                       whl->paintState.y,
                                       &textRect,
                                       &textSrcRect);

                        _leListWheelWidget_ItemRectApplyEffects(whl,
                                                                textRect,
                                                                &textRect);

                        bounds.x = 0;
                        bounds.y = 0;
                        bounds.width = whl->widget.rect.width;
                        bounds.height = textSrcRect.height;

                        // arrange image rect
                        leUtils_ArrangeRectangle(&newImgRect,
                                                 textRect,
                                                 bounds,
                                                 whl->halign,
                                                 LE_VALIGN_MIDDLE,
                                                 whl->iconPos,
                                                 whl->widget.margin.left,
                                                 whl->widget.margin.top,
                                                 whl->widget.margin.right,
                                                 whl->widget.margin.bottom,
                                                 whl->iconMargin);

                        leUtils_RectToScreenSpace((leWidget*)whl, &newImgRect);

                        newImgRect.y += getItemY(whl, whl->paintState.y, bounds.height);

                        leRectClip(&newImgRect,
                                     &layer->drawRect,
                                     &newImgRect);

                        GFX_DrawStretchBlit(&pixelBuff,
                                        0,
                                        0,
                                        pixelBuff.size.width,
                                        pixelBuff.size.height,
                                        newImgRect.x,
                                        newImgRect.y,
                                        newImgRect.width,
                                        newImgRect.height);
                    }

                    GFX_Set(GFXF_DRAW_BLEND_MODE, blendMode);

                    LE_FREE(buf);
                }
#endif
            }
        }
    }
    
    nextItem(whl);
}

static void drawIndicators(leListWheelWidget* whl)
{
    leRect rect, drawRect;
    uint32_t halfHeight;
    uint32_t topLine;
    uint32_t bottomLine;
    
    whl->fn->rectToScreen(whl, &rect);
    halfHeight = rect.height / 2;
    
    topLine = (rect.y + halfHeight) - whl->indicatorArea;
    bottomLine = (rect.y + halfHeight) + whl->indicatorArea;

    // upper lines
    drawRect.x = rect.x;
    drawRect.y = topLine;
    drawRect.width = rect.width;
    drawRect.height = 1;
    
    // top outer line
    leRenderer_HorzLine(rect.x,
                        topLine,
                        rect.width,
                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
    
    // bottom inner line
    leRenderer_HorzLine(rect.x,
                        bottomLine,
                        rect.width,
                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
    
    // top inner line
    leRenderer_HorzLine(rect.x,
                        topLine + 1,
                        rect.width,
                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
    
    // bottom outer line
    leRenderer_HorzLine(rect.x,
                        bottomLine + 1,
                        rect.width,
                        leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND),
                        paintState.alpha);
    
    //if the widget has no border, draw the vertical indicator borders
    if (whl->widget.style.borderType == LE_WIDGET_BORDER_NONE)
    {
        //left line
        drawRect.x = rect.x;
        drawRect.y = topLine + 2;
        drawRect.width = 2;
        drawRect.height = bottomLine - topLine;
        
        leRenderer_RectFill(&drawRect,
                            leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
     
        //right line
        drawRect.x = rect.x + rect.width - 2;
        drawRect.y = topLine + 2;
        drawRect.width = 2;
        drawRect.height = bottomLine - drawRect.y;
        
        leRenderer_RectFill(&drawRect,
                            leScheme_GetRenderColor(whl->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
    }
    
    nextState(whl);
}

static void drawBorder(leListWheelWidget* whl)
{
    if(whl->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)whl,
                                                    paintState.alpha);
    }
    else if(whl->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardLoweredBorder((leWidget*)whl,
                                                       paintState.alpha);
    }
    
    nextState(whl);
}

void _leListWheelWidget_Paint(leListWheelWidget* whl)
{
    if(whl->widget.status.drawState == NOT_STARTED)
        nextState(whl);

#if LE_STREAMING_ENABLED == 1
    if(whl->widget.status.drawState == WAIT_STRING ||
       whl->widget.status.drawState == WAIT_ICON)
    {
        return;
    }
#endif

    while(whl->widget.status.drawState != DONE)
    {
        whl->widget.drawFunc((leWidget*)whl);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif

#if LE_STREAMING_ENABLED == 1
        if(whl->widget.status.drawState == WAIT_STRING ||
           whl->widget.status.drawState == WAIT_ICON)
            break;
#endif
    }
}

#endif // LE_LISTWHEEL_WIDGET_ENABLED