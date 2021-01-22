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


#include "gfx/legato/string/legato_string.h"

#include <string.h>

#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/font/legato_font.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_string_renderer.h"

#if LE_DYNAMIC_VTABLES == 1
static leStringVTable stringVTable;
#endif

#define LE_STRING_SPACE     0x20 // ' '
#define LE_STRING_LINEBREAK 0xA // '\n'

void leString_Delete(leString* str)
{
    if(str == NULL)
        return;

    str->fn->destructor(str);

    LE_FREE(str);
}

void _leString_Constructor(leString* _this)
{
    _this->preCBUserData = NULL;
    _this->preInvCallback = NULL;
    _this->invCBUserData = NULL;
    _this->invCallback = NULL;
}

void _leString_Destructor(leString* _this)
{
    (void)_this; // unused
}

leResult _leString_GetRect(const leString* _this,
                           leRect* rect)
{
    uint32_t idx;
    uint32_t len, lines;
    leRect lineRect;
    leRasterFont* rasFnt;

    LE_ASSERT_THIS();
    
    *rect = leRect_Zero;
    
    len = _this->fn->length(_this);
    
    if(len == 0)
    {
        return LE_SUCCESS;
    }
        
    lines = _this->fn->getLineCount(_this);
    
    for(idx = 0; idx < lines; idx++)
    {
        _this->fn->getLineRect(_this, idx, &lineRect);
        
        if(idx < lines - 1)
        {
            rasFnt = (leRasterFont*)_this->fn->getFont(_this);
            rect->height += rasFnt->height;
        }
        else
        {
            rect->height += lineRect.height;
        }
        
        if(rect->width < lineRect.width)
        {
            rect->width = lineRect.width;
        }
    }
    
    return LE_SUCCESS;
}

uint32_t _leString_GetLineCount(const leString* _this)
{
    uint32_t len;
    uint32_t idx;
    uint32_t count;
    leChar chr;

    LE_ASSERT_THIS();

    len = _this->fn->length(_this);
    
    count = 1;
    
    if(len == 0)
        return count;
        
    for(idx = 0; idx < len; idx++)
    {
        chr = _this->fn->charAt(_this, idx);
        
        if(chr == LE_STRING_LINEBREAK)
        {
            count += 1;
        }
    }
    
    return count;
}

leResult _leString_GetLineIndices(const leString* _this,
                                  uint32_t line,
                                  uint32_t* start,
                                  uint32_t* end)
{
    uint32_t idx;
    uint32_t len;
    uint32_t count;
    leChar chr;

    LE_ASSERT_THIS();
    
    len = _this->fn->length(_this);

    if(len == 0 || line >= _this->fn->getLineCount(_this))
        return LE_FAILURE;
        
    count = 0;
    
    *start = 0;
    *end = 0;
        
    for(idx = 0; idx < len; idx++)
    {
        chr = _this->fn->charAt(_this, idx);
        
        if(chr == LE_STRING_LINEBREAK)
        {
            *end = idx;
            
            if(count == line)
                return LE_SUCCESS;
                
            count += 1;

            *start = idx + 1;
        }
    }
    
    *end = len;
    
    return LE_SUCCESS;
}

leResult _leString_GetLineRect(const leString* _this,
                               uint32_t line,
                               leRect* rect)
{
    uint32_t idx;
    uint32_t len;
    leRasterFont* fnt;
    leFontGlyph glyph;
    uint32_t startIdx, endIdx;
    leChar chr;

    LE_ASSERT_THIS();

    *rect = leRect_Zero;
    
    len = _this->fn->length(_this);
    fnt = (leRasterFont*)_this->fn->getFont(_this);
    
    if(len == 0 || fnt == NULL || line >= _this->fn->getLineCount(_this))
        return LE_FAILURE;
        
    if(fnt->glyphTable == NULL)
        return LE_FAILURE;
    
    _this->fn->getLineIndices(_this, line, &startIdx, &endIdx);
    
    for(idx = startIdx; idx < endIdx; idx++)
    {
        chr = _this->fn->charAt(_this, idx);
        
        leFont_GetGlyphInfo((leFont*)fnt, chr, &glyph);
            
        rect->width += glyph.advance;
    }
    
    rect->height = fnt->height;
    
    return LE_SUCCESS;
}

leResult _leString_GetCharRect(const leString* _this,
                               uint32_t charIdx,
                               leRect* rect)
{
    uint32_t idx;
    uint32_t len;
    leRasterFont* fnt;
    leFontGlyph glyph;
    leChar chr;

    LE_ASSERT_THIS();
    
    *rect = leRect_Zero;
    
    len = _this->fn->length(_this);
    fnt = (leRasterFont*)_this->fn->getFont(_this);
    
    if(len == 0 || fnt == NULL || charIdx >= len)
        return LE_FAILURE;
        
    if(fnt->glyphTable == NULL)
        return LE_FAILURE;
    
    for(idx = 0; idx < len; idx++)
    {
        chr = _this->fn->charAt(_this, idx);
        
        if(chr == LE_STRING_LINEBREAK)
        {
        }
        else if(idx == charIdx)
        {
            leFont_GetGlyphInfo((leFont*)fnt, chr, &glyph);
            
            rect->width = glyph.advance;
            rect->height = fnt->height;
            
            return LE_SUCCESS;
        }
        else
        {
            leFont_GetGlyphInfo((leFont*)fnt, chr, &glyph);
            
            rect->x += glyph.advance;
        }
    }
    
    return LE_FAILURE;
}

leResult _leString_GetCharIndexAtPoint(const leString* _this,
                                       const lePoint* pt,
                                       uint32_t* charIdx)
{
    uint32_t idx;
    uint32_t len;
    leRasterFont* fnt;
    leFontGlyph glyph;
    leChar chr;
    leRect rect;

    LE_ASSERT_THIS();
    
    rect.x = 0;
    rect.y = 0;

    *charIdx = 0;
    
    if(pt->x < 0 || pt->y < 0)
        return LE_FAILURE;
        
    len = _this->fn->length(_this);
    fnt = (leRasterFont*)_this->fn->getFont(_this);
    
    if(len == 0 || fnt == NULL)
        return LE_SUCCESS;

    for(idx = 0; idx < len; idx++)
    {
        chr = _this->fn->charAt(_this, idx);

        if(chr == LE_STRING_LINEBREAK)
        {
            rect.x = 0;
            rect.y += fnt->height;
        }
        else
        {
            leFont_GetGlyphInfo((leFont*)fnt, chr, &glyph);
            
            rect.width = glyph.advance;
            rect.height = glyph.height;
            
            if(leRectContainsPoint(&rect, pt) == LE_TRUE)
            {
                *charIdx = idx;
                
                return LE_SUCCESS;
            }
                
            rect.x += glyph.advance;
        }
    }

    return LE_FAILURE;
}

leResult _leString_Draw(const leString* _this,
                        int32_t x,
                        int32_t y,
                        leHAlignment align,
                        leColor clr,
                        uint32_t a)
{
    leStringRenderRequest req;

    LE_ASSERT_THIS();

    req.str = _this;
    req.x = x;
    req.y = y;
    req.align = align;
    req.color = clr;
    req.alpha = a;

    return leStringRenderer_DrawString(&req);
}

void _leString_PreInvalidate(leString* _this)
{
    LE_ASSERT_THIS();

    if(_this->preInvCallback != NULL)
    {
        _this->preInvCallback(_this, _this->preCBUserData);
    }
}

void _leString_Invalidate(leString* _this)
{
    LE_ASSERT_THIS();

    if(_this->invCallback != NULL)
    {
        _this->invCallback(_this, _this->preCBUserData);
    }
}

leResult _leString_SetPreInvalidateCallback(leString* _this,
                                            leString_InvalidateCallback cb,
                                            void* userData)
{
    LE_ASSERT_THIS();

    _this->preInvCallback = cb;

    if(cb == NULL)
    {
        _this->preCBUserData = NULL;
    }
    else
    {
        _this->preCBUserData = userData;
    }

    return LE_SUCCESS;
}

leResult _leString_SetInvalidateCallback(leString* _this,
                                         leString_InvalidateCallback cb,
                                         void* userData)
{
    LE_ASSERT_THIS();

    _this->invCallback = cb;

    if(cb == NULL)
    {
        _this->invCBUserData = NULL;
    }
    else
    {
        _this->invCBUserData = userData;
    }

    return LE_SUCCESS;
}

#if LE_DYNAMIC_VTABLES == 1
void _leString_GenerateVTable(void)
{
    stringVTable.getRect = _leString_GetRect;
    stringVTable.getLineCount = _leString_GetLineCount;
    stringVTable.getLineRect = _leString_GetLineRect;
    stringVTable.getLineIndices = _leString_GetLineIndices;
    stringVTable.getCharRect = _leString_GetCharRect;
    stringVTable.getCharIndexAtPoint = _leString_GetCharIndexAtPoint;
    stringVTable._draw = _leString_Draw;
    stringVTable.preinvalidate = _leString_PreInvalidate;
    stringVTable.invalidate = _leString_Invalidate;
    stringVTable.setPreInvalidateCallback = _leString_SetPreInvalidateCallback;
    stringVTable.setInvalidateCallback = _leString_SetInvalidateCallback;
}

void _leString_FillVTable(leStringVTable* vt)
{
    *vt = stringVTable;
}
#else
#if 0
static const leStringVTable stringVTable =
{
    // base class funcs
    .getRect = _leString_GetRect,
    .getLineCount = _leString_GetLineCount,
    .getLineRect = _leString_GetLineRect,
    .getLineIndices = _leString_GetLineIndices,
    .getCharRect = _leString_GetCharRect,
    .getCharIndexAtPoint = _leString_GetCharIndexAtPoint,
    .getCharIndexAtPoint = _leString_GetCharIndexAtPoint,
    ._draw = _leString_Draw,
    .preinvalidate = _leString_PreInvalidate,
    .invalidate = _leString_Invalidate,
    .setPreInvalidateCallback = _leString_SetPreInvalidateCallback,
    .setInvalidateCallback = _leString_SetInvalidateCallback,
};
#endif
#endif
