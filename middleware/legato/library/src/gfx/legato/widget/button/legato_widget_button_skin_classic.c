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


#include "gfx/legato/widget/button/legato_widget_button.h"

#if LE_BUTTON_WIDGET_ENABLED

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define DEFAULT_NUM_LINES 5

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_IMAGE,
#if LE_STREAMING_ENABLED == 1
    WAIT_IMAGE,
#endif
    DRAW_STRING,
#if LE_STREAMING_ENABLED == 1
    WAIT_STRING,
#endif
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

void _leButtonWidget_GetImageRect(const leButtonWidget* btn,
                                  leRect* imgRect,
								  leRect* imgSrcRect)
{
    leRect textRect = leRect_Zero;
    leRect bounds;

    btn->fn->localRect(btn, &bounds);
    
    imgRect->x = 0;
    imgRect->y = 0;
    
    if(btn->string != NULL)
    {
        btn->string->fn->getRect(btn->string, &textRect);
    }
    
    //leString_GetMultiLineRect(&btn->string, &textRect, -1);
    
    if(btn->state != LE_BUTTON_STATE_UP)
    {
        if(btn->pressedImage != NULL)
        {
            imgRect->width = btn->pressedImage->buffer.size.width;
            imgRect->height = btn->pressedImage->buffer.size.height;
        }
    }
    else
    {
        if(btn->releasedImage != NULL)
        {
            imgRect->width = btn->releasedImage->buffer.size.width;
            imgRect->height = btn->releasedImage->buffer.size.height;
        }
    }
    
    *imgSrcRect = *imgRect;
    
    leUtils_ArrangeRectangle(imgRect,
                             textRect,
                             bounds,
                             btn->widget.style.halign,
                             btn->widget.style.valign,
                             btn->imagePosition,
                             btn->widget.margin.left,
                             btn->widget.margin.top,
                             btn->widget.margin.right,
                             btn->widget.margin.bottom,
                             btn->imageMargin);
                             
    if(btn->state != LE_BUTTON_STATE_UP)
    {
        imgRect->x += btn->pressedOffset;
        imgRect->y += btn->pressedOffset;
    }                             
                             
    leRectClipAdj(imgRect, &bounds, imgSrcRect, imgRect);
    
    // move the rect to screen space
    leUtils_RectToScreenSpace((leWidget*)btn, imgRect);                         
}

void _leButtonWidget_GetTextRect(const leButtonWidget* btn,
                                 leRect* textRect,
								 leRect* drawRect)
{
    leRect bounds;
    
    leRect imgRect = leRect_Zero;
    
    *textRect = leRect_Zero;
    
    if(btn->string == NULL)
        return;
        
    btn->string->fn->getRect(btn->string, textRect);
    
    btn->fn->localRect(btn, &bounds);
    
    if(btn->state != LE_BUTTON_STATE_UP)
    {
        if(btn->pressedImage != NULL)
        {
            imgRect.width = btn->pressedImage->buffer.size.width;
            imgRect.height = btn->pressedImage->buffer.size.height;
        }
    }
    else
    {
        if(btn->releasedImage != NULL)
        {
            imgRect.width = btn->releasedImage->buffer.size.width;
            imgRect.height = btn->releasedImage->buffer.size.height;
        }
    }
    
    // arrange relative to image rect
    leUtils_ArrangeRectangleRelative(textRect,
                                     imgRect,
                                     bounds,
                                     btn->widget.style.halign,
                                     btn->widget.style.valign,
                                     btn->imagePosition,
                                     btn->widget.margin.left,
                                     btn->widget.margin.top,
                                     btn->widget.margin.right,
                                     btn->widget.margin.bottom,
                                     btn->imageMargin);

    if(btn->state != LE_BUTTON_STATE_UP)
    {
        textRect->x += btn->pressedOffset;
        textRect->y += btn->pressedOffset;
    }
                                     
    leRectClip(textRect, &bounds, drawRect);

	// move the rects to screen space
	leUtils_RectToScreenSpace((leWidget*)btn, textRect);
    leUtils_RectToScreenSpace((leWidget*)btn, drawRect);
}

void _leButtonWidget_InvalidateBorderAreas(const leButtonWidget* btn)
{
    leRect rect, dmgRect;
	int32_t left, top, right, bottom;
	
	if(btn->widget.style.borderType == LE_WIDGET_BORDER_NONE)
	    return;
	
	btn->fn->rectToScreen(btn, &rect);
	
	if(btn->widget.style.borderType == LE_WIDGET_BORDER_LINE)
	{
	    if(rect.width == 0 || rect.height == 0)
			return;

		if(rect.width <= 1 || rect.height <= 1)
		{
		    btn->fn->_damageArea(btn, &rect);

			return;
		}
	
	    left = 1;
	    top = 1;
	    right = 1;
	    bottom = 1;
	}
	else
	{
	    if(rect.width == 0 || rect.height == 0)
			return;

		if(rect.width <= 3 || rect.height <= 3)
		{
		    btn->fn->_damageArea(btn, &rect);
			
			return;
        }
        
        right = 2;
        bottom = 2;
        
        if(btn->state == LE_BUTTON_STATE_UP)
        {
            left = 1;
            top = 1;
        }
        else
        {
            left = 2;
	        top = 2;
	    }
	}
	
	// left line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y;
	dmgRect.width = left;
	dmgRect.height = rect.height;

	btn->fn->_damageArea(btn, &dmgRect);

	// top line
	dmgRect.width = rect.width;
	dmgRect.height = top;

	btn->fn->_damageArea(btn, &dmgRect);

	// right line
	dmgRect.x = rect.x + rect.width - right;
	dmgRect.y = rect.y;
	dmgRect.width = right;
	dmgRect.height = rect.height;

	btn->fn->_damageArea(btn, &dmgRect);

	// bottom line
	dmgRect.x = rect.x;
	dmgRect.y = rect.y + rect.height - bottom;
	dmgRect.width = rect.width;
	dmgRect.height = bottom;

	btn->fn->_damageArea(btn, &dmgRect);
}

static void drawBackground(leButtonWidget* btn);
static void drawString(leButtonWidget* btn);
static void drawImage(leButtonWidget* btn);
static void drawBorder(leButtonWidget* btn);
static void nextState(leButtonWidget* btn)
{
    switch(btn->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(btn->fn->getCumulativeAlphaEnabled(btn) == LE_TRUE)
            {
                paintState.alpha = btn->fn->getCumulativeAlphaAmount(btn);
            }
#endif
            
            if(btn->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                btn->widget.status.drawState = DRAW_BACKGROUND;
                btn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            if((btn->state != LE_BUTTON_STATE_UP && btn->pressedImage != NULL) ||
               (btn->state == LE_BUTTON_STATE_UP && btn->releasedImage != NULL))
            {
                btn->widget.status.drawState = DRAW_IMAGE;
                btn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawImage;

                return;
            }
        }
        // fall through
        case DRAW_IMAGE:
        {            
            if(btn->string != NULL && btn->string->fn->isEmpty(btn->string) == LE_FALSE)
            {
                btn->widget.status.drawState = DRAW_STRING;
                btn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;

                return;
            }
        }
        // fall through
        case DRAW_STRING:
        {
            if(btn->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                btn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                btn->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            btn->widget.status.drawState = DONE;
            btn->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leButtonWidget* btn)
{
    if(btn->widget.style.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        if(btn->state != LE_BUTTON_STATE_UP)
        {
            leWidget_SkinClassic_DrawBackground((leWidget*)btn,
                                                leScheme_GetRenderColor(btn->widget.scheme, LE_SCHM_BACKGROUND),
                                                paintState.alpha);
        }
        else
        {
            leWidget_SkinClassic_DrawBackground((leWidget*)btn,
                                                leScheme_GetRenderColor(btn->widget.scheme, LE_SCHM_BASE),
                                                paintState.alpha);
        }
    }

    nextState(btn);
}

#if LE_STREAMING_ENABLED == 1
static void onImageStreamFinished(leStreamManager* strm)
{
    leButtonWidget* btn = (leButtonWidget*)strm->userData;

    btn->widget.status.drawState = DRAW_IMAGE;

    nextState(btn);
}
#endif

static void drawImage(leButtonWidget* btn)
{
    leRect imgRect, imgSrcRect;
    const leImage* img = NULL;
    
    if(btn->state != LE_BUTTON_STATE_UP)
    {
        if(btn->pressedImage != NULL)
        {
            img = btn->pressedImage;
        }
    }
    else
    {
        if(btn->releasedImage != NULL)
        {
            img = btn->releasedImage;
        }
    }
    
    _leButtonWidget_GetImageRect(btn, &imgRect, &imgSrcRect);
    
    leImage_Draw(img, &imgSrcRect, imgRect.x, imgRect.y, paintState.alpha);
             
#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onImageStreamFinished;
        leGetActiveStream()->userData = btn;

        btn->widget.status.drawState = WAIT_IMAGE;
        
        return;
    }
#endif
    
    nextState(btn);
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leButtonWidget* btn = (leButtonWidget*)strm->userData;

    btn->widget.status.drawState = DRAW_STRING;

    nextState(btn);
}
#endif

static void drawString(leButtonWidget* btn)
{
    leRect textRect, drawRect;

    _leButtonWidget_GetTextRect(btn, &textRect, &drawRect);
    
    /*leRenderer_RectLine(&textRect,
                        leColorValue(LE_COLOR_MODE_RGB_565, LE_COLOR_RED),
                        255);*/
    
    btn->string->fn->_draw(btn->string,
                           textRect.x,
                           textRect.y,
                           btn->widget.style.halign,
                           leScheme_GetRenderColor(btn->widget.scheme, LE_SCHM_TEXT),
                           paintState.alpha);

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onStringStreamFinished;
        leGetActiveStream()->userData = btn;

        btn->widget.status.drawState = WAIT_STRING;

        return;
    }
#endif
    
    nextState(btn);
}

static void drawBorder(leButtonWidget* btn)
{
    if(btn->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)btn,
                                                    paintState.alpha);
    }
    else if(btn->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        if(btn->state != LE_BUTTON_STATE_UP)
        {
            leWidget_SkinClassic_DrawStandardLoweredBorder((leWidget*)btn,
                                                           paintState.alpha);
        }
        else
        {
            leWidget_SkinClassic_DrawStandardHybridBorder((leWidget*)btn,
                                                          paintState.alpha);
        }
    }
    
    nextState(btn);
}

void _leButtonWidget_Paint(leButtonWidget* btn)
{
    if(btn->widget.status.drawState == NOT_STARTED)
    {
        nextState(btn);
    }

#if LE_STREAMING_ENABLED == 1
    if(btn->widget.status.drawState == WAIT_IMAGE ||
        btn->widget.status.drawState == WAIT_STRING)
    {
        return;
    }
#endif
    
    while(btn->widget.status.drawState != DONE)
    {
        btn->widget.drawFunc((leWidget*)btn);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
        
#if LE_STREAMING_ENABLED == 1
        if(btn->widget.status.drawState == WAIT_IMAGE ||
           btn->widget.status.drawState == WAIT_STRING)
            break;
#endif
    }
}

#endif // LE_BUTTON_WIDGET_ENABLED
