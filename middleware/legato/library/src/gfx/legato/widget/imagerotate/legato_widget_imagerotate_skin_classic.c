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


#include "gfx/legato/widget/imagerotate/legato_widget_imagerotate.h"

#if LE_IMAGEROTATE_WIDGET_ENABLED

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

static void drawBackground(leImageRotateWidget* img);
static void drawImage(leImageRotateWidget* img);
static void drawBorder(leImageRotateWidget* img);

static void nextState(leImageRotateWidget* img)
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

static void drawBackground(leImageRotateWidget* img)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)img, paintState.alpha);
    
    nextState(img);
}

static void drawImage(leImageRotateWidget* img)
{
    leRect imgRect, wgtRect;
    int32_t x;
    int32_t y;

    imgRect.x = 0;
    imgRect.y = 0;
    imgRect.width = img->image->buffer.size.width;
    imgRect.height = img->image->buffer.size.height;

    img->fn->rectToScreen(img, &wgtRect);

    x = wgtRect.x + (wgtRect.width / 2) - (img->image->buffer.size.width / 2);
    y = wgtRect.y + (wgtRect.height / 2) - (img->image->buffer.size.height / 2);

    leImage_RotateDraw(img->image,
                       &imgRect,
                       img->filter,
                       img->angle,
                       x,
                       y,
                       paintState.alpha);

    nextState(img);
}

static void drawBorder(leImageRotateWidget* img)
{
    if(img->widget.style.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)img, paintState.alpha);
    }
    else if(img->widget.style.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)img, paintState.alpha);
    }
    
    nextState(img);
}

void _leImageRotateWidget_Paint(leImageRotateWidget* img)
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

#endif // LE_IMAGEPLUS_WIDGET_ENABLED