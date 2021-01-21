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

#include "gfx/legato/string/legato_tablestring.h"

#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_dynamicstring.h"
#include "gfx/legato/string/legato_stringtable.h"
#include "gfx/legato/string/legato_stringutils.h"

#if LE_DYNAMIC_VTABLES == 1
static leTableStringVTable tableStringVTable;
#else
static const leTableStringVTable tableStringVTable;
#endif

void _leString_Constructor(leString* _this);
void _leString_Destructor(leString* _this);

void leTableString_Constructor(leTableString* _this,
                               uint32_t idx)
{
    LE_ASSERT_THIS();

    _this->base.fn = (void*)&tableStringVTable;
    _this->fn = (void*)&tableStringVTable;
    
    _leString_Constructor((leString*)_this);
    
    _this->index = idx;
}

void _leTableString_Destructor(leTableString* _this)
{
    LE_ASSERT_THIS();

    _this->index = LE_STRING_NULLIDX;
    
    _leString_Destructor((leString*)_this);
}

leTableString* leTableString_New(uint32_t idx)
{
    leTableString* str;
    
    str = LE_MALLOC(sizeof(leTableString));
    
    leTableString_Constructor(str, idx);
    
    return str;
}

leFont* _leTableString_GetFont(const leTableString* _this)
{
    LE_ASSERT_THIS();
        
    return leStringTable_GetStringFont(leGetState()->stringTable,
                                       _this->index,
                                       leGetState()->languageID);
}

leResult _leTableString_SetFont(leTableString* _this,
                                const leFont* font)
{
    (void)_this; // unused
    (void)font; // unused

    return LE_FAILURE;
}

leResult _leTableString_SetFromString(leTableString* _this,
                                      const leString* src)
{
    (void)_this; // unused
    (void)src; // unused

    return LE_FAILURE;
}

leResult _leTableString_SetFromChar(leTableString* _this,
                                    const leChar* buf,
                                    uint32_t size)
{
    (void)_this; // unused
    (void)buf; // unused
    (void)size; // unused

    return LE_FAILURE;
}

leResult _leTableString_SetFromCStr(leTableString* _this,
                                    const char* cstr)
{
    (void)_this; // unused
    (void)cstr; // unused

    return LE_FAILURE;
}

uint32_t _leTableString_Length(const leTableString* _this)
{
    const leStringTable* tbl = leGetState()->stringTable;
    leStringInfo info;

    LE_ASSERT_THIS();

    if(_this->index == LE_STRING_NULLIDX)
        return 0;
    
    info.stringIndex = _this->index;
    info.languageID = leGetState()->languageID;
        
    if(leStringTable_StringLookup(tbl, &info) == LE_FAILURE)
        return 0;

    if(leStringTable_GetStringLength(tbl, &info) == LE_FAILURE)
        return 0;

    return info.length;
}

leBool _leTableString_IsEmpty(const leTableString* _this)
{
    LE_ASSERT_THIS();
        
    return _leTableString_Length(_this) == 0;
}

leChar _leTableString_CharAt(const leTableString* _this,
                             uint32_t idx)
{
    leFont* fnt = NULL;
    uint32_t i, j;
    uint32_t codePoint;
    uint32_t offset = 0;
    const leStringTable* tbl = leGetState()->stringTable;
    uint32_t lang = leGetState()->languageID;
    leStringInfo info;

    LE_ASSERT_THIS();
    
    if(tbl == NULL)
        return 0;
    
    fnt = leStringTable_GetStringFont(tbl, _this->index, lang);
    
    if(fnt == NULL)
        return 0;
        
    info.stringIndex = _this->index;
    info.languageID = lang;
    
    if(leStringTable_StringLookup(tbl, &info) == LE_FAILURE)
        return LE_FAILURE;
    
    j = 0;
    
    for(i = 0; i < info.dataSize;)
    {
        if(leDecodeCodePoint(tbl->encodingMode,
                             info.data + i,
                             info.dataSize,
                             &codePoint,
                             &offset) == LE_FAILURE)
            return LE_FAILURE;
        
        i += offset;
        
        if(j == idx)
            return codePoint;
        
        j++;
    }
    
    return 0;
}



int32_t _leTableString_Compare(const leTableString* _this,
                               const leString* tgt)
{
    uint32_t myLen, len, itr, chr;

    LE_ASSERT_THIS();

    if(tgt == NULL)
        return -1;
    
    myLen = _this->fn->length(_this);
    len = tgt->fn->length(tgt);
    
    if(myLen < len)
    {
        len = myLen;
    }
    
    if(len == 0)
        return 0;
        
    for(itr = 0; itr < len; itr++)
    {
        chr = tgt->fn->charAt(tgt, itr);
        
        if(_this->fn->charAt(_this, itr) != chr)
        {
            return _this->fn->charAt(_this, itr) - chr;
        }
    }
    
    return 0;
}

leResult _leTableString_Append(leTableString* _this,
                               const leString* val)
{
    (void)_this; // unused
    (void)val; // unused

    return LE_FAILURE;
}

leResult _leTableString_Insert(leTableString* _this,
                               const leString* val,
                               uint32_t idx)
{
    (void)_this; // unused
    (void)val; // unused
    (void)idx; // unused

    return LE_FAILURE;
}

leResult _leTableString_Remove(leTableString* _this,
                               uint32_t idx,
                               uint32_t count)
{
    (void)_this; // unused
    (void)idx; // unused
    (void)count; // unused

    return LE_FAILURE;
}

void _leTableString_Clear(leTableString* _this)
{
    (void)_this; // unused
}

uint32_t _leTableString_ToChar(const leTableString* _this, leChar* buf, uint32_t size)
{
    uint32_t myLen, itr;

    LE_ASSERT_THIS();

    if(_this->index == LE_STRING_NULLIDX ||
       buf == NULL ||
       size == 0)
    {
        return LE_FAILURE;
    }
    
    myLen = _this->fn->length(_this);
        
    if(size >= myLen)
    {
        size = myLen;
    }
    
    for(itr = 0; itr < size; itr++)
    {
        buf[itr] = _this->fn->charAt(_this, itr);
    }
    
    return size;
}

/*leResult _leTableString_Draw(const leTableString* _this,
                             int32_t x,
                             int32_t y,
                             leHAlignment align,
                             leColor clr,
                             uint32_t a)
{
    const leStringTable* table = leGetState()->stringTable;
    leFont* font;
    leStringInfo info;

    LE_ASSERT_THIS();

    font = _this->fn->getFont(_this);

    if(font == NULL)
        return LE_FAILURE;
        
    info.stringIndex = _this->index;
    info.languageID = leGetState()->languageID;
    
    if(leStringTable_StringLookup(table, &info) == LE_FAILURE)
        return LE_FAILURE;

    return _leString_Draw((void*)_this,
                          x,
                          y,
                          align,
                          clr,
                          a);
}*/

uint32_t _leTableString_GetID(const leTableString* _this)
{
    LE_ASSERT_THIS();
        
    return _this->index;
}

leResult _leTableString_SetID(leTableString* _this, uint32_t id)
{
    LE_ASSERT_THIS();

    _this->fn->preinvalidate(_this);

    _this->index = id;

    _this->fn->invalidate(_this);
        
    return LE_SUCCESS;
}

uint32_t _leTableString_SizeInBytes(leTableString* _this)
{
    const leStringTable* table;
    leStringInfo info;

    LE_ASSERT_THIS();
    
    table = leGetState()->stringTable;
    
    info.stringIndex = _this->index;
    info.languageID = leGetState()->languageID;
    
    if(leStringTable_StringLookup(table, &info))
        return 0;
        
    return info.dataSize;
}

#if LE_DYNAMIC_VTABLES == 1
void _leString_FillVTable(leStringVTable* vt);

void _leTableString_GenerateVTable(void)
{
    _leString_FillVTable((void*)&tableStringVTable);
    
    /* base functions */
    tableStringVTable.destructor = _leTableString_Destructor;
    tableStringVTable.getFont = _leTableString_GetFont;
    tableStringVTable.setFont = _leTableString_SetFont;
    tableStringVTable.setFromString = _leTableString_SetFromString;
	tableStringVTable.setFromChar = _leTableString_SetFromChar;
	tableStringVTable.setFromCStr = _leTableString_SetFromCStr;
    tableStringVTable.charAt = _leTableString_CharAt;
    tableStringVTable.length = _leTableString_Length;
    tableStringVTable.isEmpty = _leTableString_IsEmpty;
    tableStringVTable.compare = _leTableString_Compare;
    tableStringVTable.append = _leTableString_Append;
    tableStringVTable.insert = _leTableString_Insert;
    tableStringVTable.remove = _leTableString_Remove;
    tableStringVTable.clear = _leTableString_Clear;
    //tableStringVTable._draw = _leTableString_Draw;
    
    /* member functions */
    tableStringVTable.getID = _leTableString_GetID;
    tableStringVTable.setID = _leTableString_SetID;
    tableStringVTable.sizeInBytes = _leTableString_GetID;
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

static const leTableStringVTable tableStringVTable =
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
    /* base functions */
    .destructor = _leTableString_Destructor,
    .getFont = _leTableString_GetFont,
    .setFont = _leTableString_SetFont,
    .setFromString = _leTableString_SetFromString,
	.setFromChar = _leTableString_SetFromChar,
	.setFromCStr = _leTableString_SetFromCStr,
    .charAt = _leTableString_CharAt,
    .length = _leTableString_Length,
    .isEmpty = _leTableString_IsEmpty,
    .compare = _leTableString_Compare,
    .append = _leTableString_Append,
    .insert = _leTableString_Insert,
    .remove = _leTableString_Remove,
    .clear = _leTableString_Clear,
    //_draw = _leTableString_Draw,

    /* member functions */
    .getID = _leTableString_GetID,
    .setID = _leTableString_SetID,
    .sizeInBytes = _leTableString_GetID,
};
#endif