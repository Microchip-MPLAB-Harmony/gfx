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


#include "gfx/legato/widget/imagesequence/legato_widget_imagesequence.h"

#if LE_IMAGESEQUENCE_WIDGET_ENABLED

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_IMAGE,
    WAIT_IMAGE,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

void _leImageSequenceWidget_GetImageRect(leImageSequenceWidget* img,
                                         leRect* imgRect,
                                         leRect* imgSrcRect)
{
    leRect bounds;

    img->fn->localRect(img, &bounds);
    
    imgRect->x = 0;
    imgRect->y = 0;
    
    if(img->images[img->activeIdx].image != NULL)
    {
        imgRect->width = img->images[img->activeIdx].image->buffer.size.width;
        imgRect->height = img->images[img->activeIdx].image->buffer.size.height;
    }
    else
    {
        imgRect->width = 0;
        imgRect->height = 0;
    }
    
    *imgSrcRect = *imgRect;
    
    // arrange image rect
    leUtils_ArrangeRectangle(imgRect,
                             leRect_Zero,
                             bounds,
                             img->images[img->activeIdx].halign,
                             img->images[img->activeIdx].valign,
                             0,
                             img->widget.margin.left,
                             img->widget.margin.top,
                             img->widget.margin.right,
                             img->widget.margin.bottom,
                             0);
        
    leRectClipAdj(imgRect, &bounds, imgSrcRect, imgRect);
        
    leUtils_RectToScreenSpace((leWidget*)img, imgRect); 
}

static void drawBackground(leImageSequenceWidget* img);
static void drawImage(leImageSequenceWidget* img);
static void drawBorder(leImageSequenceWidget* img);

static void nextState(leImageSequenceWidget* img)
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
            if(img->activeIdx >= 0 && img->activeIdx < (int32_t)img->count &&
               img->images[img->activeIdx].image != NULL)
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

static void drawBackground(leImageSequenceWidget* img)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)img,
                                                paintState.alpha);
    
    nextState(img);
}

#if LE_STREAMING_ENABLED == 1
static void onImageStreamFinished(leStreamManager* dec)
{
    leImageSequenceWidget* img = (leImageSequenceWidget*)dec->userData;

    img->widget.status.drawState = DRAW_IMAGE;

    nextState(img);
}
#endif

static void drawImage(leImageSequenceWidget* img)
{
    leRect imgRect, imgSrcRect;
    
    imgSrcRect.x = 0;
    imgSrcRect.y = 0;
    imgSrcRect.width = img->images[img->activeIdx].image->buffer.size.width;
    imgSrcRect.height = img->images[img->activeIdx].image->buffer.size.height;
    
    _leImageSequenceWidget_GetImageRect(img, &imgRect, &imgSrcRect);
    
    leImage_Draw(img->images[img->activeIdx].image,
                 &imgSrcRect,
                 imgRect.x,
                 imgRect.y, paintState.alpha);

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onImageStreamFinished;
        leGetActiveStream()->userData = img;

        img->widget.status.drawState = WAIT_IMAGE;

        return;
    }
#endif
    
    nextState(img);
}

static void drawBorder(leImageSequenceWidget* img)
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

void _leImageSequenceWidget_Paint(leImageSequenceWidget* img)
{
    if(img->widget.status.drawState == NOT_STARTED)
        nextState(img);

#if LE_STREAMING_ENABLED == 1
    if(img->widget.status.drawState == WAIT_IMAGE)
        return;
#endif

    while(img->widget.status.drawState != DONE)
    {
        img->widget.drawFunc((leWidget*)img);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
        
#if LE_STREAMING_ENABLED == 1
        if(img->widget.status.drawState == WAIT_IMAGE)
            break;
#endif
    }
}

#endif // LE_IMAGESEQUENCE_WIDGET_ENABLED