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


#include "gfx/legato/widget/keyboard/legato_widget_keyboard.h"

#if LE_KEYBOARD_WIDGET_ENABLED

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/string/legato_stringutils.h"
#include "gfx/legato/string/legato_string_renderer.h"
#include "gfx/legato/string/legato_fixedstring.h"

#include "gfx/legato/widget/legato_widget_skin_classic_common.h"

#define MAX_STR_LENGTH 8

enum
{
    NOT_STARTED = LE_WIDGET_DRAW_STATE_READY,
    DONE = LE_WIDGET_DRAW_STATE_DONE,
    DRAW_BACKGROUND,
    DRAW_KEYBACK,
    DRAW_STRING,
#if LE_STREAMING_ENABLED == 1
    WAIT_STRING,
#endif    
    DRAW_SPECIAL_KEYS,
    DRAW_BORDER,
};

static struct
{
    uint32_t alpha;
} paintState;

void _leKeyboardWidget_InvalidateBorderAreas(leKeyboardWidget* keyboard)
{
    keyboard->fn->invalidate(keyboard);
}

static void drawBackground(leKeyboardWidget* keyboard);
static void drawString(leKeyboardWidget* keyboard);
static void drawKeyBack(leKeyboardWidget* keyboard);
static void drawBorder(leKeyboardWidget* keyboard);
static void drawSpecialKeys(leKeyboardWidget* keyboard);

static void nextState(leKeyboardWidget* keyboard)
{
    switch(keyboard->widget.status.drawState)
    {
        case NOT_STARTED:
        {
            paintState.alpha = 255;

#if LE_ALPHA_BLENDING_ENABLED == 1
            if(keyboard->fn->getCumulativeAlphaEnabled(keyboard) == LE_TRUE)
            {
                paintState.alpha = keyboard->fn->getCumulativeAlphaAmount(keyboard);
            }
#endif
            
            if(keyboard->widget.style.backgroundEnabled == LE_TRUE)
            {
                keyboard->widget.status.drawState = DRAW_BACKGROUND;
                keyboard->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBackground;

                return;
            }
        }
        // fall through
        case DRAW_BACKGROUND:
        {
            keyboard->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawKeyBack;
            keyboard->widget.status.drawState = DRAW_KEYBACK;

            return;
        }
        // fall through
        case DRAW_KEYBACK:
        {
            if (keyboard->keyFont != NULL)
            {
                keyboard->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawString;
                keyboard->widget.status.drawState = DRAW_STRING;

                return;
            }
        }
        // fall through
        case DRAW_STRING:
        {
            keyboard->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawSpecialKeys;
            keyboard->widget.status.drawState = DRAW_SPECIAL_KEYS;
            
            return;
        }
        // fall through
        case DRAW_SPECIAL_KEYS:
        {
            if(keyboard->widget.style.borderEnabled == LE_TRUE)
            {            
                keyboard->widget.drawFunc = (leWidget_DrawFunction_FnPtr)&drawBorder;
                keyboard->widget.status.drawState = DRAW_BORDER;
                
                return;
            }
            
        }
        // fall through
        case DRAW_BORDER:
        {
            keyboard->widget.status.drawState = DONE;
            keyboard->widget.drawFunc = NULL;
        }
    }
}

static void _drawStringOnKey(leKeyboardWidget* keyboard, leKeyboardKey * key)
{
    leRect boundingRect = leRect_Zero, kerningRect = leRect_Zero;
    leRect bounds;
    const leString * keyString;  
    leColor background;
    leStringRenderRequest req;

    keyString = _leKeyboardWidget_getKeyString(keyboard, key);
    if (keyString != NULL)
    {
        keyString->fn->getRect(keyString, &boundingRect);                 
        kerningRect = boundingRect;
        bounds = key->rect;

        if (key->state == LE_KEYBOARD_KEY_RELEASED)
        {
            background = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_BACKGROUND);
        }
        else
        {
            background = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_FOREGROUND);
        }         

        leStringUtils_KerningRect((leRasterFont*)keyString->fn->getFont(keyString), &kerningRect);

        leUtils_ArrangeRectangleRelative(&kerningRect, 
                                            leRect_Zero,
                                            bounds,
                                            LE_HALIGN_CENTER,
                                            LE_VALIGN_MIDDLE,
                                            LE_RELATIVE_POSITION_BEHIND,
                                            0, 0, 0, 0, 0);
        leRectClip(&kerningRect, &bounds, &kerningRect);

        leUtils_RectToScreenSpace((leWidget*)keyboard, &kerningRect);

        req.str = keyString;
        req.x = kerningRect.x;
        req.y = kerningRect.y;
        req.align = LE_HALIGN_CENTER;
        req.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_TEXT);
        req.alpha = paintState.alpha;
        req.lookupTable = leUtils_GetSchemeLookupTable(keyboard->widget.scheme,
                                                    req.color,
                                                    background);

        leStringRenderer_DrawString(&req);

        #if LE_STREAMING_ENABLED == 1
            if(leGetActiveStream() != NULL)
            {
                leGetActiveStream()->onDone = onStringStreamFinished;
                leGetActiveStream()->userData = keyboard;

                keyboard->widget.status.drawState = WAIT_STRING;

                return;
            }
        #endif            

    }
}

static void drawString(leKeyboardWidget* keyboard)
{
    uint32_t row, keys, keyIdx;
    leKeyboardKey * key;

    /* TODO: Add check for damage area */
    for (row = 0; row < NUM_KEY_ROWS; row++)
    {
        keys = _leKeyboardWidget_getKeyCount(keyboard, row);
        for (keyIdx = 0; keyIdx < keys; keyIdx++)
        {
            key = _leKeyboardWidget_getKey(keyboard, row, keyIdx);
            if ((key->type & LE_KEYBOARD_KEY_SPECIAL_MASK) == 0)
            {
                _drawStringOnKey(keyboard, key);
            }
        }
    } 

    nextState(keyboard);
}

static void drawSpecialKeys(leKeyboardWidget* keyboard)
{
    uint32_t row, keys, keyIdx;
    leRect keyRect;
    leKeyboardKey * key;

    for (row = 0; row < NUM_KEY_ROWS; row++)
    {
        keys = _leKeyboardWidget_getKeyCount(keyboard, row);
        for (keyIdx = 0; keyIdx < keys; keyIdx++)
        {
            key = _leKeyboardWidget_getKey(keyboard, row, keyIdx);
            keyRect = key->rect;

            switch (key->type)
            {
                /* special keys */
                case LE_KEYBOARD_KEY_SHIFT:
                {
                    leRect symRect = {0};
                    uint32_t shortSide;
                    lePoint pt[7];
                    struct leVector2 ptV[7];

                    shortSide = (keyRect.width < keyRect.height) ? keyRect.width : keyRect.height;

                    symRect.width = (shortSide * 3) / 7;
                    symRect.height = symRect.width;

                    leUtils_ArrangeRectangleRelative(&symRect, 
                                                     leRect_Zero,
                                                     keyRect,
                                                     LE_HALIGN_CENTER,
                                                     LE_VALIGN_MIDDLE,
                                                     LE_RELATIVE_POSITION_BEHIND,
                                                     0, 0, 0, 0, 0); 

                    leUtils_RectToScreenSpace((leWidget*)keyboard, &symRect);

                    /* pt of the shift arrow, clockwise */
                    /* tip */
                    pt[0].x = symRect.x + (symRect.width / 2);
                    pt[0].y = symRect.y;

                    /* right edge */
                    pt[1].x = symRect.x + symRect.width; 
                    pt[1].y = symRect.y + ((symRect.height * 2) / 3); 

                    /* right in-corner */
                    pt[2].x = symRect.x + ((symRect.width * 3) / 4);
                    pt[2].y = symRect.y + ((symRect.height * 2) / 3); 

                    /* right base */
                    pt[3].x = symRect.x + ((symRect.width * 3) / 4);
                    pt[3].y = symRect.y + symRect.height; 

                    /* left base */
                    pt[4].x = symRect.x + (symRect.width / 4);
                    pt[4].y = symRect.y + symRect.height; 

                    /* left in-corner */
                    pt[5].x = symRect.x + (symRect.width / 4);
                    pt[5].y = symRect.y + ((symRect.height * 2) / 3); 

                    /* left edge */
                    pt[6].x = symRect.x; 
                    pt[6].y = symRect.y + ((symRect.height * 2) / 3); 

                    struct leVectorConvexPolygon_FillAttr attr;
            
                    attr.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_TEXT);
                    attr.alpha = 0xff;
                    attr.aaMode = LE_ANTIALIASING_4X;

                    ptV[0].x = LE_REAL_I16_FROM_FLOAT(pt[0].x);
                    ptV[0].y = LE_REAL_I16_FROM_FLOAT(pt[0].y);
                    
                    ptV[1].x = LE_REAL_I16_FROM_FLOAT(pt[1].x);
                    ptV[1].y = LE_REAL_I16_FROM_FLOAT(pt[1].y);
                    
                    ptV[2].x = LE_REAL_I16_FROM_FLOAT(pt[6].x);
                    ptV[2].y = LE_REAL_I16_FROM_FLOAT(pt[6].y);

                    leDraw_VectorConvexPolygonFill((struct leVector2*)&ptV,
                                        3,
                                        &attr);                       

                    ptV[3].x = LE_REAL_I16_FROM_FLOAT(pt[2].x);
                    ptV[3].y = LE_REAL_I16_FROM_FLOAT(pt[2].y);

                    ptV[4].x = LE_REAL_I16_FROM_FLOAT(pt[3].x);
                    ptV[4].y = LE_REAL_I16_FROM_FLOAT(pt[3].y);
                    
                    ptV[5].x = LE_REAL_I16_FROM_FLOAT(pt[4].x);
                    ptV[5].y = LE_REAL_I16_FROM_FLOAT(pt[4].y);

                    ptV[6].x = LE_REAL_I16_FROM_FLOAT(pt[5].x);
                    ptV[6].y = LE_REAL_I16_FROM_FLOAT(pt[5].y);                    

                    leDraw_VectorConvexPolygonFill((struct leVector2*)&ptV[3],
                                        4,
                                        &attr);
                    break;            
                }
                case LE_KEYBOARD_KEY_BACKSPACE:
                {
                    leRect symRect = {0}, xRect = {0};
                    uint32_t shortSide;
                    struct leVectorConvexPolygon_FillAttr attr;  
                    lePoint pt[5];
                    struct leVector2 ptV[5];                                      
                    struct leVectorLineAttr lineAttr;
    
                    shortSide = (keyRect.width < keyRect.height) ? keyRect.width : keyRect.height;

                    symRect.width = (shortSide / 2);
                    symRect.height = symRect.width / 2;

                    if (symRect.width > 0 && symRect.height > 0)
                    {
                        leUtils_ArrangeRectangleRelative(&symRect, 
                                                        leRect_Zero,
                                                        keyRect,
                                                        LE_HALIGN_CENTER,
                                                        LE_VALIGN_MIDDLE,
                                                        LE_RELATIVE_POSITION_BEHIND,
                                                        0, 0, 0, 0, 0); 

                        leUtils_RectToScreenSpace((leWidget*)keyboard, &symRect);

                        /* left tip */
                        pt[0].x = symRect.x;
                        pt[0].y = symRect.y + symRect.height/2;

                        /* top left corner */
                        pt[1].x = symRect.x + symRect.width / 4; 
                        pt[1].y = symRect.y; 

                        /* top right edge */
                        pt[2].x = symRect.x + symRect.width;
                        pt[2].y = symRect.y; 

                        /* bottom right edge */
                        pt[3].x = symRect.x + symRect.width;
                        pt[3].y = symRect.y + symRect.height; 

                        /* bottom left corner */
                        pt[4].x = symRect.x + symRect.width / 4;
                        pt[4].y = symRect.y + symRect.height; 

                        attr.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_TEXT);
                        attr.alpha = 0xff;
                        attr.aaMode = LE_ANTIALIASING_4X;

                        ptV[0].x = LE_REAL_I16_FROM_FLOAT(pt[0].x);
                        ptV[0].y = LE_REAL_I16_FROM_FLOAT(pt[0].y);
                        
                        ptV[1].x = LE_REAL_I16_FROM_FLOAT(pt[1].x);
                        ptV[1].y = LE_REAL_I16_FROM_FLOAT(pt[1].y);
                        
                        ptV[2].x = LE_REAL_I16_FROM_FLOAT(pt[4].x);
                        ptV[2].y = LE_REAL_I16_FROM_FLOAT(pt[4].y);

                        leDraw_VectorConvexPolygonFill((struct leVector2*)&ptV,
                                            3,
                                            &attr);              

                        ptV[0].x = LE_REAL_I16_FROM_FLOAT(pt[1].x);
                        ptV[0].y = LE_REAL_I16_FROM_FLOAT(pt[1].y);

                        ptV[1].x = LE_REAL_I16_FROM_FLOAT(pt[2].x);
                        ptV[1].y = LE_REAL_I16_FROM_FLOAT(pt[2].y);
                        
                        ptV[2].x = LE_REAL_I16_FROM_FLOAT(pt[3].x);
                        ptV[2].y = LE_REAL_I16_FROM_FLOAT(pt[3].y);

                        ptV[3].x = LE_REAL_I16_FROM_FLOAT(pt[4].x);
                        ptV[3].y = LE_REAL_I16_FROM_FLOAT(pt[4].y);                    

                        leDraw_VectorConvexPolygonFill((struct leVector2*)&ptV,
                                            4,
                                            &attr);

                        /* draw the x inside */
                        xRect.width = (shortSide * 1) / 6;
                        xRect.height = xRect.width;

                        if (xRect.width > 0 && xRect.height > 0)
                        {
                            leUtils_ArrangeRectangleRelative(&xRect, 
                                                            leRect_Zero,
                                                            keyRect,
                                                            LE_HALIGN_CENTER,
                                                            LE_VALIGN_MIDDLE,
                                                            LE_RELATIVE_POSITION_BEHIND,
                                                            0, 0, 0, 0, 0); 

                            leUtils_RectToScreenSpace((leWidget*)keyboard, &xRect);

                            if (key->state == LE_KEYBOARD_KEY_RELEASED)
                            {
                                lineAttr.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_BACKGROUND);
                            }
                            else
                            {
                                lineAttr.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_FOREGROUND);
                            }         

                            lineAttr.alpha = 0xff;
                            lineAttr.width = LE_REAL_I16_FROM_FLOAT(3);
                            lineAttr.hardness = LE_REAL_I16_FROM_FLOAT(0.50f);
                            lineAttr.aaMode = LE_ANTIALIASING_NONE;
                            lineAttr.capStyle = LE_CAPSTYLE_NONE;  

                            ptV[0].x = LE_REAL_I16_FROM_FLOAT(xRect.x);
                            ptV[0].y = LE_REAL_I16_FROM_FLOAT(xRect.y);
                            ptV[1].x = LE_REAL_I16_FROM_FLOAT((xRect.x + xRect.width));
                            ptV[1].y = LE_REAL_I16_FROM_FLOAT((xRect.y + xRect.height));

                            ptV[2].x = LE_REAL_I16_FROM_FLOAT((xRect.x + xRect.width));
                            ptV[2].y = LE_REAL_I16_FROM_FLOAT(xRect.y);
                            ptV[3].x = LE_REAL_I16_FROM_FLOAT(xRect.x);
                            ptV[3].y = LE_REAL_I16_FROM_FLOAT((xRect.y + xRect.height));

                            leDraw_VectorLine(&ptV[0], &ptV[1], &lineAttr);
                            leDraw_VectorLine(&ptV[2], &ptV[3], &lineAttr);                        
                        }
                    }
                    
                    break;
                }
                case LE_KEYBOARD_KEY_SYMBOL1:
                case LE_KEYBOARD_KEY_SYMBOL2:
                case LE_KEYBOARD_KEY_ABC:
                {
                    _drawStringOnKey(keyboard, key);
                    break;
                }
                case LE_KEYBOARD_KEY_RETURN:
                {
                    leRect symRect = {0};
                    uint32_t shortSide;
                    struct leVectorConvexPolygon_FillAttr attr;  
                    struct leVector2 ptV[3];                                      
                    struct leVectorLineAttr lineAttr;
    
                    shortSide = (keyRect.width < keyRect.height) ? keyRect.width : keyRect.height;

                    symRect.width = (shortSide / 2);
                    symRect.height = symRect.width / 2;

                    if (symRect.width > 0 && symRect.height > 0)
                    {
                        leUtils_ArrangeRectangleRelative(&symRect, 
                                                        leRect_Zero,
                                                        keyRect,
                                                        LE_HALIGN_CENTER,
                                                        LE_VALIGN_MIDDLE,
                                                        LE_RELATIVE_POSITION_BEHIND,
                                                        0, 0, 0, 0, 0); 

                        leUtils_RectToScreenSpace((leWidget*)keyboard, &symRect);

                        attr.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_TEXT);
                        attr.alpha = 0xff;
                        attr.aaMode = LE_ANTIALIASING_4X;

                        ptV[0].x = LE_REAL_I16_FROM_FLOAT(symRect.x);
                        ptV[0].y = LE_REAL_I16_FROM_FLOAT((symRect.y + (symRect.height/2)));
                        
                        ptV[1].x = LE_REAL_I16_FROM_FLOAT((symRect.x + (symRect.width / 4)));
                        ptV[1].y = LE_REAL_I16_FROM_FLOAT(symRect.y);
                        
                        ptV[2].x = LE_REAL_I16_FROM_FLOAT((symRect.x + (symRect.width / 4)));
                        ptV[2].y = LE_REAL_I16_FROM_FLOAT((symRect.y + symRect.height));

                        leDraw_VectorConvexPolygonFill((struct leVector2*)&ptV,
                                            3,
                                            &attr);              

                        lineAttr.alpha = 0xff;
                        lineAttr.width = LE_REAL_I16_FROM_FLOAT(3);
                        lineAttr.hardness = LE_REAL_I16_FROM_FLOAT(0.50f);
                        lineAttr.aaMode = LE_ANTIALIASING_NONE;
                        lineAttr.capStyle = LE_CAPSTYLE_NONE;  
                        lineAttr.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_TEXT);

                        ptV[0].x = LE_REAL_I16_FROM_FLOAT((symRect.x + (symRect.width / 4)));
                        ptV[0].y = LE_REAL_I16_FROM_FLOAT((symRect.y + (symRect.height / 2)));

                        ptV[1].x = LE_REAL_I16_FROM_FLOAT((symRect.x + symRect.width));
                        ptV[1].y = LE_REAL_I16_FROM_FLOAT((symRect.y + (symRect.height / 2)));

                        leDraw_VectorLine(&ptV[0], &ptV[1], &lineAttr);

                        ptV[0].x = LE_REAL_I16_FROM_FLOAT((symRect.x + symRect.width));
                        ptV[0].y = LE_REAL_I16_FROM_FLOAT((symRect.y + (symRect.height / 2)));

                        ptV[1].x = LE_REAL_I16_FROM_FLOAT((symRect.x + symRect.width));
                        ptV[1].y = LE_REAL_I16_FROM_FLOAT(symRect.y);

                        leDraw_VectorLine(&ptV[0], &ptV[1], &lineAttr);                        
                    }

                    break;
                }
                /* standard character keys */
                default:
                {
                   break;     
                }
            }
        }
    } 

    nextState(keyboard);
}

static void drawKeyBack(leKeyboardWidget* keyboard)
{
    struct leVectorRect_FillAttr fillStyle;
    uint32_t row, keys, keyIdx;
    leRect fillRect;
    leRectF rectf;
    leKeyboardKey * key;

    fillStyle.alpha = keyboard->widget.style.alphaAmount;
    fillStyle.aaMode = keyboard->keyStyle->aaMode;
    fillStyle.rotation = 0;
    fillStyle.topLeftRadius = keyboard->keyStyle->topLeftRadius;
    fillStyle.topRightRadius = keyboard->keyStyle->topRightRadius;
    fillStyle.bottomLeftRadius = keyboard->keyStyle->bottomLeftRadius;
    fillStyle.bottomRightRadius = keyboard->keyStyle->bottomRightRadius;

    for (row = 0; row < NUM_KEY_ROWS; row++)
    {
        keys = _leKeyboardWidget_getKeyCount(keyboard, row);
        for (keyIdx = 0; keyIdx < keys; keyIdx++)
        {
            key = _leKeyboardWidget_getKey(keyboard, row, keyIdx);

            fillRect.height = key->rect.height;
            fillRect.width = key->rect.width;
            fillRect.x = key->rect.x + keyboard->widget.rect.x;
            fillRect.y = key->rect.y + keyboard->widget.rect.y;

            switch (key->type)
            {
                /* special keys */
                case LE_KEYBOARD_KEY_SHIFT:
                {
                    if (keyboard->mode == LE_KEYBOARD_MODE_SHIFT)
                    {
                        fillStyle.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_FOREGROUND);
                    }
                    else
                    {
                        fillStyle.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_BACKGROUND);
                    }     
                    break;
                }
                default:
                {
                    if (key->state == LE_KEYBOARD_KEY_RELEASED)
                    {
                        fillStyle.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_BACKGROUND);
                    }
                    else
                    {
                        fillStyle.color = leScheme_GetRenderColor(keyboard->widget.scheme, LE_SCHM_FOREGROUND);
                    }     
                    break;   
                }
            }

            leRectF_FromRect(&rectf, &fillRect);
            
            leDraw_VectorRectFill(&rectf, &fillStyle);    
        }
    }

    nextState(keyboard);
}

static void drawBackground(leKeyboardWidget* keyboard)
{
    leWidget* wgt = (leWidget*)keyboard;

    leWidget_Common_DrawBackground(wgt,
                                   leScheme_GetRenderColor(wgt->scheme, LE_SCHM_BASE),
                                   wgt->style.alphaAmount);
    
    nextState(keyboard);
}

static void drawBorder(leKeyboardWidget* keyboard)
{
    leWidget* wgt = (leWidget*)keyboard;

    leWidget_Common_DrawBorder(wgt,
                               leScheme_GetRenderColor(wgt->scheme, LE_SCHM_SHADOWDARK),
                               wgt->style.alphaAmount);
    
    nextState(keyboard);
}


void _leKeyboardWidget_Paint(leKeyboardWidget* keyboard)
{
    leWidget* child;
    size_t i;
    
    // hack to sync child attributes to key keyboard style
    for(i = 0; i < keyboard->widget.children.size; i++)
    {
        child = keyboard->widget.children.values[i];
        
        child->style.borderStyle = keyboard->widget.style.borderStyle;
        child->style.backgroundStyle = keyboard->widget.style.backgroundStyle;
        child->scheme = keyboard->widget.scheme;
    }
    
    if(keyboard->widget.status.drawState == NOT_STARTED)
    {
        nextState(keyboard);
    }
    
    while(keyboard->widget.status.drawState != DONE)
    {
        keyboard->widget.drawFunc((leWidget*)keyboard);
        
#if LE_PREEMPTION_LEVEL == 2
        break;
#endif
    }
}

#endif // LE_KEYBOARD_WIDGET_ENABLED
