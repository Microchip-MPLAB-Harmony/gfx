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


#include <gfx/legato/legato.h>
#include "gfx/legato/widget/textfield/legato_widget_textfield.h"

#if LE_TEXTFIELD_WIDGET_ENABLED == 1

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_STRING,
    WAIT_STRING,
    DRAW_CURSOR,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

void _leTextFieldWidget_GetTextRect(leTextFieldWidget* txt,
                                    leRect* textRect,
                                    leRect* drawRect)
{
    leRect bounds = txt->fn->localRect(txt);

    *textRect = leRect_Zero;
    *drawRect = leRect_Zero;

    if(txt->text.fn->length(&txt->text) > 0)
    {
        txt->text.fn->getRect(&txt->text, textRect);
    }
    else if(txt->hintText != NULL && leGetEditWidget() != (void*)txt)
    {
        txt->hintText->fn->getRect(txt->hintText, textRect);
    }

    leUtils_ArrangeRectangleRelative(textRect,
                                     leRect_Zero,
                                     bounds,
                                     txt->editWidget.widget.halign,
                                     LE_VALIGN_MIDDLE,
                                     0,
                                     txt->editWidget.widget.margin.left,
                                     txt->editWidget.widget.margin.top,
                                     txt->editWidget.widget.margin.right,
                                     txt->editWidget.widget.margin.bottom,
                                     0);
                                     
    leRectClip(textRect, &bounds, drawRect);
    
    leUtils_RectToScreenSpace((leWidget*)txt, textRect);
    leUtils_RectToScreenSpace((leWidget*)txt, drawRect);
}

void _leTextFieldWidget_GetCursorRect(const leTextFieldWidget* txt,
                                      leRect* cursorRect)
{
    leRect textRect;
    leRect bounds = txt->fn->localRect(txt);
    leRect charRect;
    const leString* str = NULL;

    if(txt->text.fn->length(&txt->text) > 0)
    {
        str = (leString*)&txt->text;
    }
    else if(txt->hintText != NULL && leGetEditWidget() != (void*)txt)
    {
        str = (leString*)txt->hintText;
    }
    else
    {
        textRect = leRect_Zero;
        charRect = leRect_Zero;

        cursorRect->x = bounds.x + txt->editWidget.widget.margin.left;
    }

    if(str != NULL)
    {
        str->fn->getRect(str, &textRect);

        leUtils_ArrangeRectangleRelative(&textRect,
                                         leRect_Zero,
                                         bounds,
                                         txt->editWidget.widget.halign,
                                         LE_VALIGN_MIDDLE,
                                         0,
                                         txt->editWidget.widget.margin.left,
                                         txt->editWidget.widget.margin.top,
                                         txt->editWidget.widget.margin.right,
                                         txt->editWidget.widget.margin.bottom,
                                         0);

        // start of the text
        if(txt->cursorPos == 0)
        {
            cursorRect->x = textRect.x;
        }
        // end of the text
        else if(txt->cursorPos >= str->fn->length(str))
        {
            cursorRect->x = textRect.x + textRect.width;
        }
        // middle of the text
        else
        {
            txt->hintText->fn->getCharRect(txt->hintText, txt->cursorPos, &charRect);

            cursorRect->x = textRect.x + charRect.x;
        }
    }


    cursorRect->y = bounds.y + txt->editWidget.widget.margin.top + 2;
    cursorRect->width = 1;
    cursorRect->height = bounds.height - txt->editWidget.widget.margin.top - txt->editWidget.widget.margin.bottom - 4;
    
    leUtils_RectToScreenSpace((leWidget*)txt, cursorRect);
}

static void drawBackground(leTextFieldWidget* txt);
static void drawString(leTextFieldWidget* txt);
static void drawCursor(leTextFieldWidget* txt);
static void drawBorder(leTextFieldWidget* txt);

static void nextState(leTextFieldWidget* txt)
{
    switch(txt->editWidget.widget.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(txt->fn->getCumulativeAlphaEnabled(txt) == LE_TRUE)
            {
                paintState.alpha = txt->fn->getCumulativeAlphaAmount(txt);
            }
#endif

            if(txt->editWidget.widget.backgroundType != LE_WIDGET_BACKGROUND_NONE) 
            {
                txt->editWidget.widget.drawState = DRAW_BACKGROUND;
                txt->editWidget.widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        case DRAW_BACKGROUND:
        {
            if(txt->text.fn->length(&txt->text) > 0 ||
                (txt->hintText != NULL && leGetEditWidget() != (void*)txt))
            {
                txt->editWidget.widget.drawState = DRAW_STRING;
                txt->editWidget.widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;

                return;
            }
        }
        case DRAW_STRING:
        {
            if(txt->cursorEnable == LE_TRUE && txt->cursorVisible == LE_TRUE)
            {
                txt->editWidget.widget.drawState = DRAW_CURSOR;
                txt->editWidget.widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawCursor;
                
                return; 
            }
        }
        case DRAW_CURSOR:
        {
            if(txt->editWidget.widget.borderType != LE_WIDGET_BORDER_NONE)
            {
                txt->editWidget.widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                txt->editWidget.widget.drawState = DRAW_BORDER;
                
                return;
            }
        }
        case DRAW_BORDER:
        {
            txt->editWidget.widget.drawState = DONE;
            txt->editWidget.widget.drawFunc = NULL;
        }
    }
}

static void drawBackground(leTextFieldWidget* txt)
{
    if(txt->editWidget.widget.backgroundType == LE_WIDGET_BACKGROUND_FILL)
    {
        leWidget_SkinClassic_DrawBackground((leWidget*)txt, 
                                            txt->editWidget.widget.scheme->background,
                                            paintState.alpha);
    }
    
    nextState(txt);
}

#if LE_STREAMING_ENABLED == 1
static void onStringStreamFinished(leStreamManager* strm)
{
    leTextFieldWidget* txt = (leTextFieldWidget*)strm->userData;

    txt->editWidget.widget.drawState = DRAW_STRING;

    nextState(txt);
}
#endif

static void drawString(leTextFieldWidget* txt)
{
    leRect textRect, drawRect;

    _leTextFieldWidget_GetTextRect(txt, &textRect, &drawRect);

    if(txt->text.fn->length(&txt->text) > 0)
    {
        txt->text.fn->_draw(&txt->text,
                            textRect.x,
                            textRect.y,
                            txt->editWidget.widget.halign,
                            txt->editWidget.widget.scheme->text,
                            paintState.alpha);
    }
    else if(txt->hintText != NULL && leGetEditWidget() != (void*)txt)
    {
        txt->hintText->fn->_draw(txt->hintText,
                                 textRect.x,
                                 textRect.y,
                                 txt->editWidget.widget.halign,
                                 txt->editWidget.widget.scheme->textDisabled,
                                 paintState.alpha);
    }

#if LE_STREAMING_ENABLED == 1
    if(leGetActiveStream() != NULL)
    {
        leGetActiveStream()->onDone = onStringStreamFinished;
        leGetActiveStream()->userData = txt;

        txt->editWidget.widget.drawState = WAIT_STRING;

        return;
    }
#endif
    
    nextState(txt);
}

static void drawCursor(leTextFieldWidget* txt)
{
    leRect cursorRect;
    
    _leTextFieldWidget_GetCursorRect(txt, &cursorRect);
    
    // draw cursor line
    leRenderer_RectFill(&cursorRect, 
                        txt->editWidget.widget.scheme->foreground,
                        paintState.alpha);
    
    nextState(txt);
}

static void drawBorder(leTextFieldWidget* txt)
{
    if(txt->editWidget.widget.borderType == LE_WIDGET_BORDER_LINE)
    {
        leWidget_SkinClassic_DrawStandardLineBorder((leWidget*)txt,
                                                    paintState.alpha);
    }
    else if(txt->editWidget.widget.borderType == LE_WIDGET_BORDER_BEVEL)
    {
        leWidget_SkinClassic_DrawStandardLoweredBorder((leWidget*)txt,
                                                       paintState.alpha);
    }
    
    nextState(txt);
}

void _leTextFieldWidget_Paint(leTextFieldWidget* txt)
{
    if(txt->editWidget.widget.scheme == NULL)
    {
        txt->editWidget.widget.drawState = DONE;
        
        return;
    }

    if(txt->editWidget.widget.drawState == NOT_STARTED)
    {
        nextState(txt);
    }
    
    while(txt->editWidget.widget.drawState != DONE)
    {
        txt->editWidget.widget.drawFunc((leWidget*)txt);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif

#if LE_STREAMING_ENABLED == 1
        if(txt->editWidget.widget.drawState == WAIT_STRING)
            break;
#endif
    }
}

#endif // LE_TEXTFIELD_WIDGET_ENABLED