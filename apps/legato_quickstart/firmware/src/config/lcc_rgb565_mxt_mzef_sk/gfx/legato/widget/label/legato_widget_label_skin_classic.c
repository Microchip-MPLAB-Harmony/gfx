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


#include "gfx/legato/widget/label/legato_widget_label.h"

#if LE_LABEL_WIDGET_ENABLED

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define DEFAULT_NUM_LINES 5

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
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

void _leLabelWidget_GetTextRect(leLabelWidget* lbl,
                                leRect* textRect,
								leRect* drawRect)
{
    leRect bounds;
    
    *textRect = leRect_Zero;
    
    if(lbl->string == NULL)
        return;
    
    lbl->string->fn->getRect(lbl->string,
                             textRect);
    
    bounds = lbl->fn->localRect(lbl);
    
    // arrange relative to image rect
    leUtils_ArrangeRectangleRelative(textRect,
                                     leRect_Zero,
                                     bounds,
                                     lbl->widget.halign,
                                     lbl->widget.valign,
                                     0,
                                     lbl->widget.margin.left,
                                     lbl->widget.margin.top,
                                     lbl->widget.margin.right,
                                     lbl->widget.margin.bottom,
                                     0);

    leRectClip(textRect, &bounds, drawRect);

	// move the rects to screen space
	leUtils_RectToScreenSpace((leWidget*)lbl, textRect);
    leUtils_RectToScreenSpace((leWidget*)lbl, drawRect);
}

static void drawBackground(leLabelWidget* lbl);
static void drawString(leLabelWidget* lbl);
static void drawBorder(leLabelWidget* lbl);


static void nextState(leLabelWidget* lbl)
{
    switch(lbl->widget.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(lbl->fn->getCumulativeAlphaEnabled(lbl) == LE_TRUE)
            {
                paintState.alpha = lbl->fn->getCumulativeAlphaAmount(lbl);
            }
#endif
            
            if(lbl->widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                lbl->widget.drawState = DRAW_BACKGROUND;
                lbl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            if(lbl->string != NULL && lbl->string->fn->isEmpty(lbl->string) == LE_FALSE)
            {
                lbl->widget.drawState = DRAW_STRING;
                lbl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;

                return;
            }
        }
        case DRAW_STRING:
        {
            if(lbl->widget.borderType != LE_WIDGET_BORDER_NONE)
            {
                lbl->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                lbl->widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            lbl->widget.drawState = DONE;
            lbl->widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leLabelWidget* lbl)
{
    leWidget_SkinClassic_DrawStandardBackground((leWidget*)lbl,
                                                paintState.alpha);
    
    nextState(lbl);
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leLabelWidget* lbl = (leLabelWidget*)strm->userData;

    lbl->widget.drawState = DRAW_STRING;

    nextState(lbl);
}
#endif

static void drawString(leLabelWidget* lbl)
{
    leRect textRect, drawRect;

    _leLabelWidget_GetTextRect(lbl, &textRect, &drawRect);

    //leRenderer_RectFill(&textRect, leColorConvert(LE_COLOR_MODE_RGB_888, LE_COLOR_MODE_RGB_565, 0xFF0000), 255);
    
    lbl->string->fn->_draw(lbl->string,
                          textRect.x,
                          textRect.y,
                          lbl->widget.halign,
                          lbl->widget.scheme->text,
                          paintState.alpha);

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onStringStreamFinished;
        leGetActiveStream()->userData = lbl;

        lbl->widget.drawState = WAIT_STRING;

        return;
    }
#endif
    
    nextState(lbl);
}

static void drawBorder(leLabelWidget* lbl)
{
    if(lbl->widget.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)lbl,
                                                    paintState.alpha);
    }
    else if(lbl->widget.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardRaisedBorder((leWidget*)lbl,
                                                      paintState.alpha);
    }
    
    nextState(lbl);
}

void _leLabelWidget_Paint(leLabelWidget* lbl)
{
    if(lbl->widget.scheme == NULL)
    {
        lbl->widget.drawState = DONE;
        
        return;
    }

    if(lbl->widget.drawState == NOT_STARTED)
        nextState(lbl);

#if LE_STREAMING_ENABLED == 1
    if(lbl->widget.drawState == WAIT_STRING)
        return;
#endif

    while(lbl->widget.drawState != DONE)
    {
        lbl->widget.drawFunc((leWidget*)lbl);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
        
#if LE_STREAMING_ENABLED == 1
        if(lbl->widget.drawState == WAIT_STRING)
            break;
#endif
    }
}

#endif // LE_IMAGE_WIDGET_ENABLED
