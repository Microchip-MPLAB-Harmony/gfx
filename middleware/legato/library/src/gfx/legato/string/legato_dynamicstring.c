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

#include "gfx/legato/string/legato_dynamicstring.h"

#include <string.h>

#include "gfx/legato/memory/legato_memory.h"

#if LE_DYNAMIC_VTABLES == 1
static leDynamicStringVTable dynamicStringVTable;
#else
static const leDynamicStringVTable dynamicStringVTable;
#endif

void _leString_Constructor(leString* _this);
void _leString_Destructor(leString* _this);

void leDynamicString_Constructor(leDynamicString* _this)
{
    LE_ASSERT_THIS();

    _this->base.fn = (void*)&dynamicStringVTable;
    _this->fn = (void*)&dynamicStringVTable;
    
    _leString_Constructor((leString*)_this);
    
    _this->data = NULL;
    _this->capacity = 0;
    _this->length = 0;
    _this->font = NULL;
}

leDynamicString* leDynamicString_New()
{
    leDynamicString* str;
    
    str = LE_MALLOC(sizeof(leDynamicString));
    
    leDynamicString_Constructor(str);
    
    return str;
}

void _leDynamicString_Destructor(leDynamicString* _this)
{
    if(_this->data != NULL)
    {
        LE_FREE(_this->data);
    }
    
    _this->data = NULL;
    _this->capacity = 0;
    _this->length = 0;
    _this->font = NULL;
    
    _leString_Destructor((leString*)_this);
}

leFont* _leDynamicString_GetFont(const leDynamicString* _this)
{
    LE_ASSERT_THIS();
        
    return (leFont*)_this->font;
}

leResult _leDynamicString_SetFont(leDynamicString* _this,
                                  const leFont* font)
{
    LE_ASSERT_THIS();

    _this->fn->preinvalidate(_this);

    _this->font = font;

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

uint32_t _leDynamicString_GetCapacity(leDynamicString* _this)
{
    LE_ASSERT_THIS();
        
    return _this->capacity;
}

leResult _leDynamicString_SetCapacity(leDynamicString* _this, uint32_t cap)
{
    LE_ASSERT_THIS();
        
    if(_this->capacity == cap)
        return LE_SUCCESS;

    _this->fn->preinvalidate(_this);
    
    if(_this->data != NULL && cap == 0)
    {
        LE_FREE(_this->data);
        
        _this->data = NULL;
        _this->capacity = 0;
        _this->length = 0;
        
        return LE_SUCCESS;
    }
        
    _this->data = LE_REALLOC(_this->data, cap * sizeof(leChar));
    
    if(_this->data == NULL)
        return LE_FAILURE;
    
    _this->capacity = cap;
    
    if(_this->length > _this->capacity)
    {
        _this->length = _this->capacity;
    }

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

leResult _leDynamicString_SetFromString(leDynamicString* _this,
                                        const leString* src)
{ 
    uint32_t len, itr;

    LE_ASSERT_THIS();

    if(src == NULL)
        return LE_FAILURE;
    
    len = src->fn->length(src);
    
    if(_this->capacity < len)
    {
        if(_this->fn->setCapacity(_this, len) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    _this->fn->preinvalidate(_this);
    
    for(itr = 0; itr < len; itr++)
    {
        _this->data[itr] = src->fn->charAt(src, itr);
    }
    
    _this->length = len;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

leResult _leDynamicString_SetFromChar(leDynamicString* _this,
                                      const leChar* buf,
                                      uint32_t size)
{
    uint32_t itr;

    LE_ASSERT_THIS();

    if(buf == NULL)
        return LE_FAILURE;
       
    if(_this->capacity < size)
    {
        if(_this->fn->setCapacity(_this, size) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    _this->fn->preinvalidate(_this);
    
    for(itr = 0; itr < size; itr++)
    {
        _this->data[itr] = buf[itr];
    }
    
    _this->length = size;

    _this->fn->invalidate(_this);

    return LE_SUCCESS;
}

leResult _leDynamicString_SetFromCStr(leDynamicString* _this,
                                      const char* cstr)
{
    uint32_t len, itr;

    LE_ASSERT_THIS();

    if(cstr == NULL)
    {
        return LE_FAILURE;
    }
    
    len = strlen(cstr);
    
    if(len == 0)
        return LE_SUCCESS;
        
    if(_this->capacity < len)
    {
        if(_this->fn->setCapacity(_this, len) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    _this->fn->preinvalidate(_this);

    _this->length = len;
    
    for(itr = 0; itr < len; itr++)
    {
        _this->data[itr] = (leChar)((unsigned char)cstr[itr]);
    }

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

uint32_t _leDynamicString_Length(const leDynamicString* _this)
{
    LE_ASSERT_THIS();
        
    return _this->length;
}

leBool _leDynamicString_IsEmpty(const leDynamicString* _this)
{
    LE_ASSERT_THIS();
        
    return _this->length == 0;
}

leChar _leDynamicString_CharAt(const leDynamicString* _this,
                               uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->length)
        return 0;
        
    return _this->data[idx];
}

int32_t _leDynamicString_Compare(const leDynamicString* _this,
                                 const leString* tgt)
{
    uint32_t len, itr, chr;

    LE_ASSERT_THIS();

    if(tgt == NULL)
        return -1;
        
    len = tgt->fn->length(tgt);
    
    if(_this->length < len)
    {
        len = _this->length;
    }
    
    if(len == 0)
        return 0;
        
    for(itr = 0; itr < len; itr++)
    {
        chr = tgt->fn->charAt(tgt, itr);
        
        if(_this->data[itr] != chr)
        {
            return _this->data[itr] - chr;
        }
    }
    
    return 0;
}

leResult _leDynamicString_Append(leDynamicString* _this,
                                 const leString* val)
{
    uint32_t cpyLen, itr;

    LE_ASSERT_THIS();
        
    if(val == NULL || val->fn->length(val) == 0)
        return LE_SUCCESS;
        
    cpyLen = val->fn->length(val);
    
    /* resize to fit */
    if(_this->length + cpyLen > _this->capacity)
    {
        if(_this->fn->setCapacity(_this, _this->length + cpyLen) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }

    _this->fn->preinvalidate(_this);
    
    for(itr = 0; itr < cpyLen; itr++)
    {
        _this->data[_this->length + itr] = val->fn->charAt(val, itr);
    }

    _this->length += itr;

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

leResult _leDynamicString_Insert(leDynamicString* _this,
                                 const leString* val,
                                 uint32_t idx)
{
    uint32_t cpyLen;
    uint32_t itr;

    LE_ASSERT_THIS();

    if(val == NULL || val->fn->length(val) == 0)
        return LE_SUCCESS;
        
    if(idx >= _this->length)
    {
        return _leDynamicString_Append(_this, val);
    }
        
    cpyLen = val->fn->length(val);
    
    if(cpyLen == 0)
        return LE_SUCCESS;

    _this->fn->preinvalidate(_this);

    /* resize to fit */  
    if(_this->length + cpyLen > _this->capacity)
    {
        if(_this->fn->setCapacity(_this, _this->length + cpyLen) == LE_FAILURE)
        {
            return LE_FAILURE;
        }
    }
    
    /* shift data right as much as possible to make room for the new values */
    for(itr = 0; itr < _this->length - idx; itr++)
    {
        _this->data[_this->length - 1 + cpyLen - itr] = _this->data[_this->length - itr - 1];
    }

    /* insert new data */
    for(itr = 0; itr < cpyLen; itr++)
    {
        if(idx + itr >= _this->capacity)
            break;
            
        _this->data[idx + itr] = val->fn->charAt(val, itr);
    }
    
    _this->length += cpyLen;
    
    if(_this->length > _this->capacity)
    {
        _this->length = _this->capacity;
    }

    _this->fn->invalidate(_this);
   
    return LE_SUCCESS;
}

leResult _leDynamicString_Remove(leDynamicString* _this,
                                 uint32_t idx,
                                 uint32_t count)
{
    uint32_t itr;

    LE_ASSERT_THIS();

    if(_this->data == NULL || idx >= _this->length)
        return LE_FAILURE;
    
    if(count == 0)
        return LE_SUCCESS;

    _this->fn->preinvalidate(_this);
    
    /* simple case, just move length index */
    if(idx + count >= _this->length)
    {
        _this->length = idx;
        
        return LE_SUCCESS;
    }

    /* shift data left */
    for(itr = idx; itr < (uint32_t)(_this->length - 1); itr++)
    {
        _this->data[itr] = _this->data[itr + count];
    }

    _this->length -= count;

    _this->fn->invalidate(_this);
    
    return LE_SUCCESS;
}

void _leDynamicString_Clear(leDynamicString* _this)
{
    LE_ASSERT_THIS();

    _this->fn->preinvalidate(_this);

    _this->length = 0;

    _this->fn->invalidate(_this);
}

uint32_t _leDynamicString_ToChar(const leDynamicString* _this,
                                 leChar* buf,
                                 uint32_t size)
{
    LE_ASSERT_THIS();

    if(_this->data == NULL || buf == NULL || size == 0)
        return LE_FAILURE;
        
    if(size >= _this->length)
    {
        size = _this->length;
    }
    
    memcpy(buf, _this->data, size * sizeof(leChar));
    
    return size;
}

#if LE_DYNAMIC_VTABLES == 1
void _leString_FillVTable(leStringVTable* vt);

void _leDynamicString_GenerateVTable(void)
{
    _leString_FillVTable((void*)&dynamicStringVTable);
    
    dynamicStringVTable.destructor = _leDynamicString_Destructor;
    
    dynamicStringVTable.getFont = _leDynamicString_GetFont;
    dynamicStringVTable.setFont = _leDynamicString_SetFont;
    dynamicStringVTable.setFromString = _leDynamicString_SetFromString;
	dynamicStringVTable.setFromChar = _leDynamicString_SetFromChar;
	dynamicStringVTable.setFromCStr = _leDynamicString_SetFromCStr;
    dynamicStringVTable.charAt = _leDynamicString_CharAt;
    dynamicStringVTable.length = _leDynamicString_Length;
    dynamicStringVTable.isEmpty = _leDynamicString_IsEmpty;
    dynamicStringVTable.compare = _leDynamicString_Compare;
    dynamicStringVTable.append = _leDynamicString_Append;
    dynamicStringVTable.insert = _leDynamicString_Insert;
    dynamicStringVTable.remove = _leDynamicString_Remove;
    dynamicStringVTable.clear = _leDynamicString_Clear;
    dynamicStringVTable.toChar = _leDynamicString_ToChar;
    dynamicStringVTable.getCapacity = _leDynamicString_GetCapacity;
    dynamicStringVTable.setCapacity = _leDynamicString_SetCapacity;
}
#else
leResult _leString_GetRect(const leString*, leRect*);
uint32_t _leString_GetLineCount(const leString*);
leResult _leString_GetLineRect(const leString*, uint32_t, leRect*);
leResult _leString_GetLineIndices(const leString*, uint32_t, uint32_t*, uint32_t*);
leResult _leString_GetCharRect(const leString*, uint32_t, leRect*);
leResult _leString_GetCharIndexAtPoint(const leString*, const lePoint*, uint32_t*);
leResult _leString_Draw(const leString*, int32_t, int32_t, leHAlignment, leColor, uint32_t);
void _leString_PreInvalidate(leString*);
void _leString_Invalidate(leString*);
leResult _leString_SetPreInvalidateCallback(leString*, leString_InvalidateCallback, void*);
leResult _leString_SetInvalidateCallback(leString*, leString_InvalidateCallback, void*);

static const leDynamicStringVTable dynamicStringVTable =
{
    // base class funcs
    .getRect = (void*)_leString_GetRect,
    .getLineCount = (void*)_leString_GetLineCount,
    .getLineRect = (void*)_leString_GetLineRect,
    .getLineIndices = (void*)_leString_GetLineIndices,
    .getCharRect = (void*)_leString_GetCharRect,
    .getCharIndexAtPoint = (void*)_leString_GetCharIndexAtPoint,
    ._draw = (void*)_leString_Draw,
    .preinvalidate = (void*)_leString_PreInvalidate,
    .invalidate = (void*)_leString_Invalidate,
    .setPreInvalidateCallback = (void*)_leString_SetPreInvalidateCallback,
    .setInvalidateCallback = (void*)_leString_SetInvalidateCallback,

    // member funcs
    .destructor = _leDynamicString_Destructor,

    .getFont = _leDynamicString_GetFont,
    .setFont = _leDynamicString_SetFont,
    .setFromString = _leDynamicString_SetFromString,
	.setFromChar = _leDynamicString_SetFromChar,
	.setFromCStr = _leDynamicString_SetFromCStr,
    .charAt = _leDynamicString_CharAt,
    .length = _leDynamicString_Length,
    .isEmpty = _leDynamicString_IsEmpty,
    .compare = _leDynamicString_Compare,
    .append = _leDynamicString_Append,
    .insert = _leDynamicString_Insert,
    .remove = _leDynamicString_Remove,
    .clear = _leDynamicString_Clear,
    .toChar = _leDynamicString_ToChar,
    .getCapacity = _leDynamicString_GetCapacity,
    .setCapacity = _leDynamicString_SetCapacity,
};
#endif