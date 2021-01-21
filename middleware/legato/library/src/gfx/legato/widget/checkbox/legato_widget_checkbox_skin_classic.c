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


#include "gfx/legato/widget/checkbox/legato_widget_checkbox.h"

#if LE_CHECKBOX_WIDGET_ENABLED

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_scheme.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"

#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define CHECKBOX_SIZE       13
#define CHECKBOX_SIZE_HALF  6

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
    DRAW_BORDER
};

static struct
{
    uint32_t alpha;
} paintState;

void _leCheckBoxWidget_GetImageRect(const leCheckBoxWidget* cbox,
									leRect* imgRect,
									leRect* imgSrcRect)
{
    leRect textRect = leRect_Zero;
    leRect bounds;

    cbox->fn->localRect(cbox, &bounds);
    
    imgRect->x = 0;
    imgRect->y = 0;
    
    if(cbox->string != NULL)
    {
        cbox->string->fn->getRect(cbox->string, &textRect);
    }
    
    if(cbox->checked == LE_FALSE)
    {
        if(cbox->uncheckedImage == NULL)
        {
            imgRect->width = CHECKBOX_SIZE;
            imgRect->height = CHECKBOX_SIZE;
        }
        else
        {
            imgRect->width = cbox->uncheckedImage->buffer.size.width;
            imgRect->height = cbox->uncheckedImage->buffer.size.height;
        }
    }
    else
    {
        if(cbox->checkedImage == NULL)
        {
            imgRect->width = CHECKBOX_SIZE;
            imgRect->height = CHECKBOX_SIZE;
        }
        else
        {
            imgRect->width = cbox->checkedImage->buffer.size.width;
            imgRect->height = cbox->checkedImage->buffer.size.height;
        }
    }

	*imgSrcRect = *imgRect;
    
    leUtils_ArrangeRectangle(imgRect,
                             textRect,
                             bounds,
                             cbox->widget.style.halign,
                             cbox->widget.style.valign,
                             cbox->imagePosition,
                             cbox->widget.margin.left,
                             cbox->widget.margin.top,
                             cbox->widget.margin.right,
                             cbox->widget.margin.bottom,
                             cbox->imageMargin);

	leRectClipAdj(imgRect, &bounds, imgSrcRect, imgRect);

	// move the rect to screen space
	leUtils_RectToScreenSpace((leWidget*)cbox, imgRect);
}

void _leCheckBoxWidget_GetTextRect(const leCheckBoxWidget* cbox,
								   leRect* textRect,
								   leRect* drawRect)
{
    leRect bounds;
    
    leRect imgRect = {0};
    
    *textRect = leRect_Zero;
    
    if(cbox->string == NULL)
        return;
    
    cbox->string->fn->getRect(cbox->string, textRect);

    cbox->fn->localRect(cbox, &bounds);
    
    // calculate image dimensions
    if(cbox->checked == LE_TRUE)
    {
        if(cbox->checkedImage != NULL)
        {
            imgRect.width = cbox->checkedImage->buffer.size.width;
            imgRect.height = cbox->checkedImage->buffer.size.height;
        }
        else
        {
            imgRect.width = CHECKBOX_SIZE;
            imgRect.height = CHECKBOX_SIZE;
        }
    }
    else
    {
        if(cbox->uncheckedImage != NULL)
        {
            imgRect.width = cbox->uncheckedImage->buffer.size.width;
            imgRect.height = cbox->uncheckedImage->buffer.size.height;
        }
        else
        {
            imgRect.width = CHECKBOX_SIZE;
            imgRect.height = CHECKBOX_SIZE;
        }
    }
    
    // arrange relative to image rect
    leUtils_ArrangeRectangleRelative(textRect,
                                     imgRect,
                                     bounds,
                                     cbox->widget.style.halign,
                                     cbox->widget.style.valign,
                                     cbox->imagePosition,
                                     cbox->widget.margin.left,
                                     cbox->widget.margin.top,
                                     cbox->widget.margin.right,
                                     cbox->widget.margin.bottom,
                                     cbox->imageMargin);

	leRectClip(textRect, &bounds, drawRect);

	// move the rects to layer space
	leUtils_RectToScreenSpace((leWidget*)cbox, textRect);
    leUtils_RectToScreenSpace((leWidget*)cbox, drawRect);
}

static void drawBackground(leCheckBoxWidget* cbox);
static void drawImage(leCheckBoxWidget* cbox);
static void drawString(leCheckBoxWidget* cbox);
static void drawBorder(leCheckBoxWidget* cbox);

static void nextState(leCheckBoxWidget* cbox)
{
    switch(cbox->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(cbox->fn->getCumulativeAlphaEnabled(cbox) == LE_TRUE)
            {
                paintState.alpha = cbox->fn->getCumulativeAlphaAmount(cbox);
            }
#endif
            
            if(cbox->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                cbox->widget.status.drawState = DRAW_BACKGROUND;
                cbox->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            cbox->widget.status.drawState = DRAW_IMAGE;
            cbox->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawImage;

            return;
        }
        case DRAW_IMAGE:
        {            
            if(cbox->string != NULL &&
               cbox->string->fn->length(cbox->string) != 0)
            {
                cbox->widget.status.drawState = DRAW_STRING;
                cbox->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;

                return;
            }
        }
        // fall through
        case DRAW_STRING:
        {
            if(cbox->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                cbox->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                cbox->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {
            cbox->widget.status.drawState = DONE;
            cbox->widget.drawFunc = NULL;
        }
    }
}

static void drawCheckBox(leCheckBoxWidget* cbox, leRect* rect)
{
    leRect fillRect;
    
    // draw box fill
    fillRect.x = rect->x;
    fillRect.y = rect->y;
    fillRect.width = CHECKBOX_SIZE;
    fillRect.height = CHECKBOX_SIZE;
    
    leRenderer_RectFill(&fillRect,
                        leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_BACKGROUND),
                        paintState.alpha);
    
    // draw box border
    leWidget_SkinClassic_Draw2x2BeveledBorder(rect,
                                              leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_SHADOW),
                                              leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_SHADOWDARK),
                                              leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_HIGHLIGHT),
                                              leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_HIGHLIGHTLIGHT),
                                              paintState.alpha);
        
    if(cbox->checked == LE_TRUE)
    {
        leRenderer_DrawLine(rect->x + 3,
                            rect->y + 5,
                            rect->x + 5,
                            rect->y + 7,
                            leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
                            
        leRenderer_DrawLine(rect->x + 5,
                            rect->y + 7,
                            rect->x + 9,
                            rect->y + 3,
                            leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
        
        leRenderer_DrawLine(rect->x + 3,
                            rect->y + 6,
                            rect->x + 5,
                            rect->y + 8,
                            leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
                            
        leRenderer_DrawLine(rect->x + 5,
                            rect->y + 8,
                            rect->x + 9,
                            rect->y + 4,
                            leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
                     
        leRenderer_DrawLine(rect->x + 3,
                            rect->y + 7,
                            rect->x + 5,
                            rect->y + 9,
                            leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
                            
        leRenderer_DrawLine(rect->x + 5,
                            rect->y + 9,
                            rect->x + 9,
                            rect->y + 5,
                            leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_FOREGROUND),
                            paintState.alpha);
    }
}

static void drawBackground(leCheckBoxWidget* cbox)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)cbox,
                                                 paintState.alpha);

    nextState(cbox);
}

#if LE_STREAMING_ENABLED == 1
static void onImageStreamFinished(leStreamManager* dec)
{
    leCheckBoxWidget* cbox = (leCheckBoxWidget*)dec->userData;

    cbox->widget.status.drawState = DRAW_IMAGE;

    nextState(cbox);
}
#endif

static void drawImage(leCheckBoxWidget* cbox)
{
	const leImage* img = NULL;
	leRect imgRect, imgSrcRect;
	
	if(cbox->checked == LE_FALSE)
	{
        img = cbox->uncheckedImage;
    }
    else
    {
        img = cbox->checkedImage;
    }
    
	_leCheckBoxWidget_GetImageRect(cbox, &imgRect, &imgSrcRect);
	
        
    if(img == NULL)
    {
        drawCheckBox(cbox, &imgRect);
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
            leGetActiveStream()->userData = cbox;

            cbox->widget.status.drawState = WAIT_IMAGE;
            
            return;
        }
#endif            
    }
    
    nextState(cbox);
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leCheckBoxWidget* cbox = (leCheckBoxWidget*)strm->userData;

    cbox->widget.status.drawState = DRAW_STRING;

    nextState(cbox);
}
#endif

static void drawString(leCheckBoxWidget* cbox)
{
	leRect textRect, drawRect;

	_leCheckBoxWidget_GetTextRect(cbox, &textRect, &drawRect);

    /*leRenderer_RectLine(&textRect,
                        leColorValue(LE_COLOR_MODE_RGB_565, LE_COLOR_RED),
                        255);*/

    cbox->string->fn->_draw(cbox->string,
                            textRect.x,
                            textRect.y,
                            cbox->widget.style.halign,
                            leScheme_GetRenderColor(cbox->widget.scheme, LE_SCHM_TEXT),
                            paintState.alpha);

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onStringStreamFinished;
        leGetActiveStream()->userData = cbox;

        cbox->widget.status.drawState = WAIT_STRING;

        return;
    }
#endif
    
    nextState(cbox);
}

static void drawBorder(leCheckBoxWidget* cbox)
{
    if(cbox->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)cbox,
                                                    paintState.alpha);
    }
    else if(cbox->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)cbox,
                                                      paintState.alpha);
    }
    
    nextState(cbox);
}

void _leCheckBoxWidget_Paint(leCheckBoxWidget* cbox)
{
    if(cbox->widget.status.drawState == NOT_STARTED)
    {
        nextState(cbox);
    }

#if LE_STREAMING_ENABLED == 1
    if(cbox->widget.status.drawState == WAIT_IMAGE ||
       cbox->widget.status.drawState == WAIT_STRING)
    {
        return;
    }
#endif
    
    while(cbox->widget.status.drawState != DONE)
    {
        cbox->widget.drawFunc((leWidget*)cbox);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
        
#if LE_STREAMING_ENABLED == 1
        if(cbox->widget.status.drawState == WAIT_IMAGE ||
           cbox->widget.status.drawState == WAIT_STRING)
            break;
#endif
    }
}

#endif // LE_CHECKBOX_WIDGET_ENABLED
