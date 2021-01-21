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


#include "gfx/legato/widget/imagescale/legato_widget_imagescale.h"

#if LE_IMAGESCALE_WIDGET_ENABLED

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#include "gfx/legato/common/legato_math.h"

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_IMAGE,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

void _leImageScaleWidget_GetImageRect(const leImageScaleWidget* img,
                                     leRect* imgRect,
                                     leRect* imgSrcRect)
{
    leRect clipRect;
    int32_t x = 0;
    int32_t y = 0;
    int32_t w = 0;
    int32_t h = 0;
    
    int32_t aspect;
    
    leRect bounds;

    img->fn->localRect(img, &bounds);
    
    imgRect->x = 0;
    imgRect->y = 0;
    
    if(img->image != NULL)
    {
        if(img->resizeToFit == LE_FALSE)
        {
            imgRect->width = img->image->buffer.size.width + img->transformWidth;
            imgRect->height = img->image->buffer.size.height + img->transformHeight;
            imgRect->x = bounds.width / 2 - imgRect->width / 2;
            imgRect->y = bounds.height / 2 - imgRect->height / 2;
        }
        else
        {
            imgRect->width = img->widget.rect.width;
            imgRect->height = img->widget.rect.height;
            
            imgRect->x += img->widget.margin.left;
            imgRect->width -= img->widget.margin.left;

            imgRect->y += img->widget.margin.top;
            imgRect->height -= img->widget.margin.top;

            imgRect->width -= img->widget.margin.right;
            imgRect->height -= img->widget.margin.bottom;
                
            if(img->preserveAspect == LE_TRUE)
            {
                if(img->image->buffer.size.width < img->image->buffer.size.height)
                {
                    aspect = lePercentWholeRounded(img->image->buffer.size.height, img->image->buffer.size.width);
                    imgRect->height = lePercentOf(imgRect->width, aspect);
                }
                else
                {                    
                    aspect = lePercentWholeRounded(img->image->buffer.size.width, img->image->buffer.size.height);
                
                    imgRect->width = lePercentOf(imgRect->height, aspect);
                    
                    if(imgRect->width > bounds.width - img->widget.margin.left - img->widget.margin.right)
                    {
                        imgRect->width = bounds.width - img->widget.margin.left - img->widget.margin.right;
                        
                        aspect = lePercentWholeRounded(img->image->buffer.size.height, img->image->buffer.size.width);
                        
                        imgRect->height = lePercentOf(imgRect->width, aspect);
                    }
                }
                
                imgRect->x = bounds.width / 2 - imgRect->width / 2;
                imgRect->y = bounds.height / 2 - imgRect->height / 2;
            }
        }
        
        imgSrcRect->x = 0;
        imgSrcRect->y = 0;
        imgSrcRect->width = img->image->buffer.size.width;
        imgSrcRect->height = img->image->buffer.size.height;
    }
    else
    {
        imgRect->width = 0;
        imgRect->height = 0;
        
        *imgSrcRect = *imgRect;
    }
    
    imgRect->x += img->transformX;
    imgRect->y += img->transformY;
    
    leRectClip(imgRect, &bounds, &clipRect);
    
    w = img->image->buffer.size.width;
    h = img->image->buffer.size.height;
    
    // scaled rectangle has been clipped, adjust source rectangle
    if(clipRect.width != imgRect->width)
    {
        w = lePercentWholeRounded(clipRect.width, imgRect->width);
        w = lePercentOf(imgSrcRect->width, w);
        
        // calculate the delta in x
        x = clipRect.x - imgRect->x;
        
        if(x > 0)
        {
            x = lePercentWholeRounded(x, imgRect->width);
            x = lePercentOf(imgSrcRect->width, x);
        }
    }
    
    if(clipRect.height != imgRect->height)
    {
        h = lePercentWholeRounded(clipRect.height, imgRect->height);
        h = lePercentOf(imgSrcRect->height, h);
        
        // calculate the delta in x
        y = clipRect.y - imgRect->y;
        
        if(y > 0)
        {
            y = lePercentWholeRounded(y, imgRect->height);
            y = lePercentOf(imgSrcRect->height, y);
        }
    }
    
    imgSrcRect->x = x;
    imgSrcRect->y = y;
    imgSrcRect->width = w;
    imgSrcRect->height = h;
    
    *imgRect = clipRect;
    
    leUtils_RectToScreenSpace((leWidget*)img, imgRect); 
}

static void drawBackground(leImageScaleWidget* img);
static void drawImage(leImageScaleWidget* img);
static void drawBorder(leImageScaleWidget* img);

static void nextState(leImageScaleWidget* img)
{
    switch(img->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(img->fn->getCumulativeAlphaEnabled(img) == LE_TRUE)
            {
                paintState.alpha = img->fn->getCumulativeAlphaAmount(img);
            }
#endif
            
            if(img->widget.style.backgroundType != LE_WIDGET_BACKGROUND_NONE)
            {
                img->widget.status.drawState = DRAW_BACKGROUND;
                img->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            if(img->image != NULL)
            {
                img->widget.status.drawState = DRAW_IMAGE;
                img->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawImage;

                return;
            }
        }
        // fall through
        case DRAW_IMAGE:
        {            
            if(img->widget.style.borderType != LE_WIDGET_BORDER_NONE)
            {
                img->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                img->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
        }
        // fall through
        case DRAW_BORDER:
        {           
            
            img->widget.status.drawState = DONE;
            img->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leImageScaleWidget* img)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)img,
                                                paintState.alpha);
    
    nextState(img);
}

static void drawImage(leImageScaleWidget* img)
{
    leRect imgRect, imgSrcRect;
       
    _leImageScaleWidget_GetImageRect(img,
                                     &imgRect,
                                     &imgSrcRect);

    leImage_ResizeDraw(img->image,
                       &imgSrcRect,
                       img->filter,
                       imgRect.width,
                       imgRect.height,
                       imgRect.x,
                       imgRect.y,
                       paintState.alpha);

    nextState(img);
}

static void drawBorder(leImageScaleWidget* img)
{
    if(img->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)img,
                                                    paintState.alpha);
    }
    else if(img->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)img,
                                                      paintState.alpha);
    }
    
    nextState(img);
}

void _leImageScaleWidget_Paint(leImageScaleWidget* img)
{
    if(img->widget.status.drawState == NOT_STARTED)
    {
        nextState(img);
    }
    
    while(img->widget.status.drawState != DONE)
    {
        img->widget.drawFunc((leWidget*)img);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_IMAGESCALE_WIDGET_ENABLED