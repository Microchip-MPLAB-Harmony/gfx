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


#include "gfx/legato/widget/radiobutton/legato_widget_radiobutton.h"

#if LE_RADIOBUTTON_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define MIN_CIRCLE_SIZE       15
#define MIN_CIRCLE_SIZE_HALF  6
#define THICKNESS_DIV         9

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

void _leRadioButtonWidget_GetImageRect(const leRadioButtonWidget* btn,
									   leRect* imgRect,
									   leRect* imgSrcRect)
{
    leRect textRect;
    leRect bounds = btn->fn->localRect(btn);
    
    imgRect->x = 0;
    imgRect->y = 0;
    
    if(btn->string != NULL)
    {
        btn->string->fn->getRect(btn->string, &textRect);
    }
    else
    {
        textRect = leRect_Zero;
    }
    
    // calculate image dimensions
    if(btn->selected == LE_TRUE)
    {
        if(btn->selectedImage != NULL)
        {
            imgRect->width = btn->selectedImage->buffer.size.width;
            imgRect->height = btn->selectedImage->buffer.size.height;
        }
        else
        {
            imgRect->width = btn->circleButtonSize;
            imgRect->height = btn->circleButtonSize;
        }
    }
    else
    {
        if(btn->unselectedImage != NULL)
        {
            imgRect->width = btn->unselectedImage->buffer.size.width;
            imgRect->height = btn->unselectedImage->buffer.size.height;
        }
        else
        {
            imgRect->width = btn->circleButtonSize;
            imgRect->height = btn->circleButtonSize;
        }
    }

	*imgSrcRect = *imgRect;
    
    leUtils_ArrangeRectangle(imgRect,
                             textRect,
                             bounds,
                             btn->widget.halign,
                             btn->widget.valign,
                             btn->imagePosition,
                             btn->widget.margin.left,
                             btn->widget.margin.top,
                             btn->widget.margin.right,
                             btn->widget.margin.bottom,
                             btn->imageMargin);

	*imgRect = leRectClipAdj(imgRect, &bounds, imgSrcRect);

	// move the rect to layer space
	leUtils_RectToScreenSpace((leWidget*)btn, imgRect);
}

void _leRadioButtonWidget_GetTextRect(leRadioButtonWidget* btn,
								      leRect* textRect,
								      leRect* drawRect)
{
    leRect bounds;
    
    leRect imgRect = {0};
    
    bounds = btn->fn->localRect(btn);
    
    if(btn->string != NULL)
    {
        btn->string->fn->getRect(btn->string, textRect);
    }
    else
    {
        *textRect = leRect_Zero;
    }
    
    // calculate image dimensions
    if(btn->selected == LE_TRUE)
    {
        if(btn->selectedImage != NULL)
        {
            imgRect.width = btn->selectedImage->buffer.size.width;
            imgRect.height = btn->selectedImage->buffer.size.height;
        }
        else
        {
            imgRect.width = btn->circleButtonSize;
            imgRect.height = btn->circleButtonSize;
        }
    }
    else
    {
        if(btn->unselectedImage != NULL)
        {
            imgRect.width = btn->unselectedImage->buffer.size.width;
            imgRect.height = btn->unselectedImage->buffer.size.height;
        }
        else
        {
            imgRect.width = btn->circleButtonSize;
            imgRect.height = btn->circleButtonSize;
        }
    }
    
    // arrange relative to image rect
    leUtils_ArrangeRectangleRelative(textRect,
                                     imgRect,
                                     bounds,
                                     btn->widget.halign,
                                     btn->widget.valign,
                                     btn->imagePosition,
                                     btn->widget.margin.left,
                                     btn->widget.margin.top,
                                     btn->widget.margin.right,
                                     btn->widget.margin.bottom,
                                     btn->imageMargin);

	leRectClip(textRect, &bounds, drawRect);

	// move the rects to layer space
	leUtils_RectToScreenSpace((leWidget*)btn, textRect);
    leUtils_RectToScreenSpace((leWidget*)btn, drawRect);
}

static void drawBackground(leRadioButtonWidget* btn);
static void drawImage(leRadioButtonWidget* btn);
static void drawString(leRadioButtonWidget* btn);
static void drawBorder(leRadioButtonWidget* btn);

static void nextState(leRadioButtonWidget* btn)
{
    switch(btn->widget.drawState)
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

            if(btn->widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                btn->widget.drawState = DRAW_BACKGROUND;
                btn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            btn->widget.drawState = DRAW_IMAGE;
            btn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawImage;

            return;
        }
        case DRAW_IMAGE:
        {            
            if(btn->string != NULL)
            {
                btn->widget.drawState = DRAW_STRING;
                btn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;

                return;
            }
        }
        case DRAW_STRING:
        {
            if(btn->widget.borderType != LE_WIDGET_BORDER_NONE)
            {
                btn->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                btn->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            btn->widget.drawState = DONE;
            btn->widget.drawFunc = NULL;
        }
    }
}

static void drawCircle(leRadioButtonWidget* btn, leRect* rect, leBool filled)
{
    int x = rect->x;
    int y = rect->y;
    leColor clr;

    // fill
    clr = btn->widget.scheme->background;
    
    leRenderer_HorzLine(x + 4, y + 2, 7, clr, paintState.alpha);
    leRenderer_HorzLine(x + 3, y + 3, 8, clr, paintState.alpha);
    leRenderer_HorzLine(x + 2, y + 4, 10, clr, paintState.alpha);
    leRenderer_HorzLine(x + 2, y + 5, 10, clr, paintState.alpha);
    leRenderer_HorzLine(x + 2, y + 6, 10, clr, paintState.alpha);
    leRenderer_HorzLine(x + 2, y + 7, 10, clr, paintState.alpha);
    leRenderer_HorzLine(x + 3, y + 8, 8, clr, paintState.alpha);
    leRenderer_HorzLine(x + 4, y + 9, 7, clr, paintState.alpha);
    leRenderer_HorzLine(x + 2, y + 10, 8, clr, paintState.alpha);
    leRenderer_HorzLine(x + 4, y + 11, 4, clr, paintState.alpha);
    
    // upper outer ring
    clr = btn->widget.scheme->shadow;
    
    leRenderer_HorzLine(x + 4, y + 0, 4, clr, paintState.alpha);
    leRenderer_HorzLine(x + 2, y + 1, 2, clr, paintState.alpha);
    leRenderer_HorzLine(x + 8, y + 1, 2, clr, paintState.alpha);
    leRenderer_HorzLine(x + 8, y + 1, 2, clr, paintState.alpha);
    leRenderer_VertLine(x + 1, y + 2, 2, clr, paintState.alpha);
    leRenderer_VertLine(x + 1, y + 8, 2, clr, paintState.alpha);
    leRenderer_VertLine(x + 1, y + 2, 2, clr, paintState.alpha);
    leRenderer_VertLine(x + 0, y + 4, 4, clr, paintState.alpha);
    
    // upper inner ring
    clr = btn->widget.scheme->shadowDark;
    
    leRenderer_HorzLine(x + 4, y + 1, 4, clr, paintState.alpha);
    leRenderer_HorzLine(x + 2, y + 2, 2, clr, paintState.alpha);
    leRenderer_HorzLine(x + 8, y + 2, 2, clr, paintState.alpha);
    leRenderer_BlendPixel_Safe(x + 2, y + 4, clr, paintState.alpha);
    leRenderer_VertLine(x + 1, y + 4, 4, clr, paintState.alpha);
    leRenderer_BlendPixel_Safe(x + 2, y + 9, clr, paintState.alpha);
    
    // lower inner ring
    clr = btn->widget.scheme->highlight;
    
    leRenderer_BlendPixel_Safe(x + 9, y + 4, clr, paintState.alpha);
    leRenderer_VertLine(x + 10, y + 4, 4, clr, paintState.alpha);
    leRenderer_BlendPixel_Safe(x + 9, y + 9, clr, paintState.alpha);
    leRenderer_HorzLine(x + 2, y + 9, 2, clr, paintState.alpha);
    leRenderer_HorzLine(x + 8, y + 9, 2, clr, paintState.alpha);
    leRenderer_HorzLine(x + 4, y + 10, 4, clr, paintState.alpha);
    
    if(filled == LE_TRUE)
    {
        clr = btn->widget.scheme->foreground;
        
        leRenderer_HorzLine(x + 5, y + 4, 2, clr, paintState.alpha);
        leRenderer_HorzLine(x + 4, y + 5, 4, clr, paintState.alpha);
        leRenderer_HorzLine(x + 4, y + 6, 4, clr, paintState.alpha);
        leRenderer_HorzLine(x + 5, y + 7, 2, clr, paintState.alpha);
    }
}

static void drawCircleArcs(leRadioButtonWidget* btn,
                           leRect* rect,
                           leBool filled)
{
    lePoint center;
    uint32_t outRadius = (rect->width < rect->height) ? rect->width / 2 : rect->height / 2;
    uint32_t thickness = (outRadius / THICKNESS_DIV > 1) ? outRadius / THICKNESS_DIV : 1;
    
    center.x = rect->x + rect->width / 2;
    center.y = rect->y + rect->height / 2;
    
    // upper outer ring
    /*GFX_Set(GFXF_DRAW_THICKNESS, thickness);
    GFX_Set(GFXF_DRAW_COLOR, btn->widget.scheme->shadow);
    GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
            btn->widget.scheme->shadow & 0xffffff00, 
            btn->widget.scheme->shadow, 
            NULL, 
            NULL);  */  
            
    leRenderer_ArcFill(rect,
                       center.x,
                       center.y,
                       outRadius,
                       45,
                       180,
                       thickness,
                       btn->widget.scheme->shadow,
                       LE_FALSE,
                       paintState.alpha);
            
    //GFX_DrawArc(center.x, center.y, outRadius, 45, 180);
    
    // lower outer ring
    /*GFX_Set(GFXF_DRAW_THICKNESS, thickness*2);
    GFX_Set(GFXF_DRAW_COLOR, btn->widget.scheme->highlightLight);
    GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
            btn->widget.scheme->highlightLight & 0xffffff00, 
            btn->widget.scheme->highlightLight, 
            NULL, 
            NULL);
    GFX_DrawArc(center.x, center.y, outRadius, 225, 180);*/
    
    leRenderer_ArcFill(rect,
                       center.x,
                       center.y,
                       outRadius,
                       225,
                       180,
                       thickness * 2,
                       btn->widget.scheme->highlightLight,
                       LE_FALSE,
                       paintState.alpha);
    
    
    // upper inner ring
    outRadius -= thickness;
    /*GFX_Set(GFXF_DRAW_THICKNESS, thickness*2);
    GFX_Set(GFXF_DRAW_COLOR, btn->widget.scheme->shadowDark);
    GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
            btn->widget.scheme->shadowDark & 0xffffff00, 
            btn->widget.scheme->shadowDark, 
            NULL, 
            NULL);
    GFX_DrawArc(center.x, center.y, outRadius, 45, 180);*/    
    
    leRenderer_ArcFill(rect,
                       center.x,
                       center.y,
                       outRadius,
                       45,
                       180,
                       thickness * 2,
                       btn->widget.scheme->shadowDark,
                       LE_FALSE,
                       paintState.alpha);        
    
    
    // lower inner ring
    outRadius -= thickness;
    /*GFX_Set(GFXF_DRAW_THICKNESS, thickness*2);
    GFX_Set(GFXF_DRAW_COLOR, btn->widget.scheme->highlight);
    GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
            btn->widget.scheme->highlight & 0xffffff00, 
            btn->widget.scheme->highlight, 
            NULL, 
            NULL);
    GFX_DrawArc(center.x, center.y, outRadius, 225, 180); */
    
    leRenderer_ArcFill(rect,
                       center.x,
                       center.y,
                       outRadius,
                       225,
                       180,
                       thickness * 2,
                       btn->widget.scheme->highlight,
                       LE_FALSE,
                       paintState.alpha);      
    
    //Center base circle
    outRadius -= thickness;
    /*GFX_Set(GFXF_DRAW_THICKNESS, outRadius);
    GFX_Set(GFXF_DRAW_COLOR, btn->widget.scheme->background);
    GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
            btn->widget.scheme->background & 0xffffff00, 
            btn->widget.scheme->background, 
            NULL, 
            NULL);
    GFX_DrawArc(center.x, center.y, outRadius, 0, 360); */
    
    leRenderer_ArcFill(rect,
                       center.x,
                       center.y,
                       outRadius,
                       0,
                       360,
                       outRadius,
                       btn->widget.scheme->background,
                       LE_FALSE,
                       paintState.alpha);
    
    if(filled == LE_TRUE)
    {
        outRadius = ((outRadius * 2)/3 < 3) ? 3 : (outRadius * 2)/3;
        /*GFX_Set(GFXF_DRAW_THICKNESS, outRadius);
        GFX_Set(GFXF_DRAW_COLOR, btn->widget.scheme->foreground);
        GFX_Set(GFXF_DRAW_GRADIENT_COLOR, 
                btn->widget.scheme->foreground & 0xffffff00, 
                btn->widget.scheme->foreground, 
                NULL, 
                NULL);
        GFX_DrawArc(center.x, center.y, outRadius, 0, 360); */ 
        
        leRenderer_ArcFill(rect,
                           center.x,
                           center.y,
                           outRadius,
                           0,
                           360,
                           outRadius,
                           btn->widget.scheme->foreground,
                           LE_FALSE,
                           paintState.alpha);       
    }
    
}

static void drawBackground(leRadioButtonWidget* btn)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)btn,
                                                paintState.alpha);

    nextState(btn);
}

#if LE_STREAMING_ENABLED == 1
static void onImageStreamFinished(leStreamManager* dec)
{
    leRadioButtonWidget* btn = (leRadioButtonWidget*)dec->userData;

    btn->widget.drawState = DRAW_IMAGE;

    nextState(btn);
}
#endif

static void drawImage(leRadioButtonWidget* btn)
{
    leRect imgRect, imgSrcRect;
    const leImage* img = NULL;
    
    _leRadioButtonWidget_GetImageRect(btn, &imgRect, &imgSrcRect);
    
    if(btn->selected == LE_FALSE)
    {
        img = btn->unselectedImage;
    }
    else
    {
        img = btn->selectedImage;
    }
    
    if(img == NULL)
    {
        if (btn->circleButtonSize < MIN_CIRCLE_SIZE)
        {
            drawCircle(btn, &imgRect, btn->selected);
        }
        else
        {
            drawCircleArcs(btn, &imgRect, btn->selected);
        }
    }
    else
    {
        leImage_Draw(img,
                     &imgSrcRect,
                     imgRect.x,
                     imgRect.y,
                     paintState.alpha);

#if LE_STREAMING_ENABLED == 1
        if(leGetActiveStream() != NULL)
        {
            leGetActiveStream()->onDone = onImageStreamFinished;
            leGetActiveStream()->userData = btn;

            btn->widget.drawState = WAIT_IMAGE;

            return;
        }
#endif
    }
    
    nextState(btn);
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leRadioButtonWidget* btn = (leRadioButtonWidget*)strm->userData;

    btn->widget.drawState = DRAW_STRING;

    nextState(btn);
}
#endif

static void drawString(leRadioButtonWidget* btn)
{
    leRect textRect, drawRect;
    
    _leRadioButtonWidget_GetTextRect(btn, &textRect, &drawRect);
        
    btn->string->fn->_draw(btn->string,
                         textRect.x,
                         textRect.y,
                         LE_HALIGN_CENTER,
                         btn->widget.scheme->text,
                         paintState.alpha);

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onStringStreamFinished;
        leGetActiveStream()->userData = btn;

        btn->widget.drawState = WAIT_STRING;

        return;
    }
#endif
    
    nextState(btn);
}

static void drawBorder(leRadioButtonWidget* btn)
{
    if(btn->widget.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)btn,
                                                    paintState.alpha);
    }
    else if(btn->widget.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)btn,
                                                      paintState.alpha);
    }
    
    nextState(btn);
}

void _leRadioButtonWidget_Paint(leRadioButtonWidget* btn)
{
    if(btn->widget.scheme == NULL)
    {
        btn->widget.drawState = DONE;
        
        return;
    }
    
    if(btn->widget.drawState == NOT_STARTED)
    {
        nextState(btn);
    }

#if LE_STREAMING_ENABLED == 1
    if(btn->widget.drawState == WAIT_STRING ||
       btn->widget.drawState == WAIT_IMAGE)
    {
        return;
    }
#endif
    
    while(btn->widget.drawState != DONE)
    {
        btn->widget.drawFunc((leWidget*)btn);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
        
#if LE_STREAMING_ENABLED == 1
        if(btn->widget.drawState == WAIT_STRING ||
           btn->widget.drawState == WAIT_IMAGE)
            break;
#endif
    }
}

#endif // LE_RADIOBUTTON_WIDGET_ENABLED