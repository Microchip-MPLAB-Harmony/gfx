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

/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_string.h

  Summary:
    A string library implementation for the Legato user interface library.  

  Description:
    This is a string library implementation that is used internally by the 
    Legato user interface library.  
    
    This implementation relies on the leChar definition for characters.  
    This chararcter definition is 16 bits in size and allows the library to
    support international character code points and Unicode encoding standards.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_STRING_H
#define LEGATO_STRING_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#include "gfx/legato/font/legato_font.h"
#include "gfx/legato/common/legato_error.h"

//DOM-IGNORE-BEGIN
struct leString;

typedef void (*leString_InvalidateCallback)(const struct leString* str, void* userData);

#define LE_STRING_VTABLE(THIS_TYPE) \
    void        (*destructor)(THIS_TYPE* _this); \
	leFont*     (*getFont)(const THIS_TYPE* _this); \
	leResult    (*setFont)(THIS_TYPE* _this, const leFont* font); \
	leResult    (*setFromString)(THIS_TYPE* _this, const struct leString* src); \
	leResult    (*setFromChar)(THIS_TYPE* _this, const leChar* buf, uint32_t size); \
	leResult    (*setFromCStr)(THIS_TYPE* _this, const char* cstr); \
    leChar      (*charAt)(const THIS_TYPE* _this, uint32_t idx); \
    uint32_t    (*length)(const THIS_TYPE* _this); \
    leBool      (*isEmpty)(const THIS_TYPE* _this); \
    int32_t     (*compare)(const THIS_TYPE* _this, const struct leString* tgt); \
    leResult    (*append)(THIS_TYPE* _this, const struct leString* val); \
    leResult    (*insert)(THIS_TYPE* _this, const struct leString* val, uint32_t idx); \
    leResult    (*remove)(THIS_TYPE* _this, uint32_t idx, uint32_t count); \
    void        (*clear)(THIS_TYPE* _this); \
    uint32_t    (*toChar)(const THIS_TYPE* _this, leChar* buf, uint32_t size); \
    leResult    (*getRect)(const THIS_TYPE* _this, leRect* rect); \
    uint32_t    (*getLineCount)(const THIS_TYPE* _this); \
    leResult    (*getLineRect)(const THIS_TYPE* _this, uint32_t line, leRect* rect); \
    leResult    (*getLineIndices)(const THIS_TYPE* _this, uint32_t line, uint32_t* start, uint32_t* end); \
    leResult    (*getCharRect)(const THIS_TYPE* _this, uint32_t idx, leRect* rect); \
    leResult    (*getCharIndexAtPoint)(const THIS_TYPE* _this, const lePoint* pt, uint32_t* idx); \
    leResult    (*_draw)(const THIS_TYPE* _this, int32_t x, int32_t y, leHAlignment align, leColor clr, uint32_t a); \
    void        (*preinvalidate)(THIS_TYPE* _this); \
    void        (*invalidate)(THIS_TYPE* _this); \
    leResult    (*setPreInvalidateCallback)(THIS_TYPE* _this, leString_InvalidateCallback, void* userData); \
    leResult    (*setInvalidateCallback)(THIS_TYPE* _this, leString_InvalidateCallback, void* userData);

typedef struct leStringVTable
{
	LE_STRING_VTABLE(struct leString)
} leStringVTable;
//DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leString

  Summary:
    Generic string definition

  Remarks:
    None.
*/
typedef struct leString
{
    const leStringVTable* fn; // function table

    leString_InvalidateCallback preInvCallback; // pre-invalidate callback
    void* preCBUserData; // pre-invalidate callback user data

    leString_InvalidateCallback invCallback; // nvalidate callback
    void* invCBUserData; // invalidate callback user data
} leString;

// *****************************************************************************
/* Function:
    void leString_Delete(leString* str)

   Summary:
    Frees the memory that was allocated using the la{TYPE}String_New type
    allocator functions

   Parameters:

   Returns:

   Remarks:
    Used in conjunction with the la{TYPE}String_New type allocator functions
*/
LIB_EXPORT void leString_Delete(leString* str);

// *****************************************************************************
/* Virtual Member Function:
    void destructor(leString* _this)

  Summary:
    The generic destructor

  Description:
    The generic destructor

  Parameters:
    leString* _this - The string to operate on

  Remarks:
    Usage - _this->fn->destructor(_this);

  Returns:
    void
*/

// *****************************************************************************
/* Virtual Member Function:
    leFont* getFont(const leString* _this)

  Summary:
    Gets the string font

  Description:
    Gets the string font

  Parameters:
    const leString* _this - The string to operate on

  Remarks:
    Usage - _this->fn->getFont(_this);

  Returns:
    leFont* - the font assigned to the string
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFont(leString* _this,
                     const leFont* font)

  Summary:
    Sets the string font

  Description:
    Sets the string font

  Parameters:
    leString* _this - The string to operate on
    const leFont* font - the font to assign to the string

  Remarks:
    Usage - _this->fn->setFont(_this, font);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFromString(leString* _this,
                           const struct leString* src)

  Summary:
    Sets the string value from another string

  Description:
    Sets the string value from another string

  Parameters:
    leString* _this - The string to operate on
    const struct leString* src - the string to copy

  Remarks:
    Usage - _this->fn->setFromString(_this, src);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFromChar(leString* _this,
                         const leChar* buf,
                         uint32_t size)

  Summary:
    Sets the string value from a leChar buffer

  Description:
    Sets the string value from a leChar buffer

  Parameters:
    leString* _this - The string to operate on
    const leChar* buf - the buffer to copy
    uint32_t size - the size of the buffer in leChar values

  Remarks:
    Usage - _this->fn->setFromChar(_this, buf, size);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFromCStr(leString* _this,
                         const char* cstr)

  Summary:
    Sets the string value from a C string buffer

  Description:
    Sets the string value from a C string buffer

  Parameters:
    leString* _this - The string to operate on
    const char* cstr - the size of the buffer in bytes

  Remarks:
    Usage - _this->fn->setFromCStr(_this, cstr);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leChar charAt(const leString* _this,
                  uint32_t idx)

  Summary:
    Gets a codepoint at the given index

  Description:
    Gets a codepoint at the given index

  Parameters:
    const leString* _this - The string to operate on
    uint32_t idx - the index to query

  Remarks:
    Usage - _this->fn->charAt(_this, idx);

  Returns:
    leChar - the leChar contained at the index or -1
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t length(const leString* _this)

  Summary:
    Gets the length of the string

  Description:
    Gets the length of the string

  Parameters:
    const leString* _this - The string to operate on

  Remarks:
    Usage - _this->fn->length(_this);

  Returns:
    uint32_t - the length of the string
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool isEmpty(const leString* _this)

  Summary:
    Test to see if a string is empty

  Description:
    Test to see if a string is empty

  Parameters:
    const leString* _this - The string to operate on

  Remarks:
    Usage - _this->fn->isEmpty(_this);

  Returns:
    leBool - LE_TRUE if the string is empty
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t compare(const leString* _this,
                    const struct leString* tgt)

  Summary:
    Compare a string to another string

  Description:
    Compare a string to another string

  Parameters:
    const leString* _this - The string to operate on
    const struct leString* tgt -

  Remarks:
    Usage - _this->fn->compare(_this, tgt);

  Returns:
    int32_t - (< 0) the first character that does not match has a lower value in
                    this than tgt

              0 the contents of both strings are equal

              (> 0)	the first character that does not match has a greater value
                    this than tgt
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult append(leString* _this,
                    const struct leString* val)

  Summary:
    Appends a string to this string

  Description:
    Appends a string to this string

  Parameters:
    leString* _this - The string to operate on
    const struct leString* val - the string to append

  Remarks:
    Usage - _this->fn->append(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult insert(leString* _this,
                    const struct leString* val,
                    uint32_t idx)

  Summary:
    Inserts a string into this one at a given index

  Description:
    Inserts a string into this one at a given index

  Parameters:
    leString* _this - The string to operate on
    const struct leString* val - the string to insert
    uint32_t idx - the insertion index

  Remarks:
    Usage - _this->fn->insert(_this, val, idx);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult remove(leString* _this,
                    uint32_t idx,
                    uint32_t count)

  Summary:
    Removes a part of this string

  Description:
    Removes a part of this string

  Parameters:
    leString* _this - The string to operate on
    uint32_t idx - the removal index
    uint32_t count - the number of leChars to remove

  Remarks:
    Usage - _this->fn->remove(_this, idx, count);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    void clear(leString* _this)

  Summary:
    Clears the string data

  Description:
    Clears the string data

  Parameters:
    leString* _this - The string to operate on

  Remarks:
    Usage - _this->fn->clear(_this);

  Returns:
    void
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t toChar(const leString* _this,
                    leChar* buf,
                    uint32_t size)

  Summary:
    Converts the string to a C-style string

  Description:
    Converts the string to a C-style string

  Parameters:
    const leString* _this - The string to operate on
    leChar* buf - the buffer to fill
    uint32_t size - the size of the input buffer

  Remarks:
    Usage - _this->fn->toChar(_this, buf, size);

  Returns:
    uint32_t - the number of leChar written to the buffer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult getRect(const leString* _this,
                     leRect* rect)

  Summary:
    Gets the rectangle of the rendered string in pixels

  Description:
    Gets the rectangle of the rendered string in pixels

  Parameters:
    const leString* _this - The string to operate on
    leRect* rect - will contain the result of the query

  Remarks:
    Usage - _this->fn->getRect(_this, rect);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getLineCount(const leString* _this)

  Summary:
    Gets the number of lines in the string

  Description:
    Gets the number of lines in the string

  Parameters:
    const leString* _this - The string to operate on

  Remarks:
    Usage - _this->fn->getLineCount(_this);

  Returns:
    uint32_t - the number of lines in the string
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult getLineRect(const leString* _this,
                         uint32_t line,
                         leRect* rect)

  Summary:
    Gets the rectangle of a string line

  Description:
    Gets the rectangle of a string line

  Parameters:
    const leString* _this - The string to operate on
    uint32_t line - the line to query
    leRect* rect - will contain the result of the query

  Remarks:
    Usage - _this->fn->getLineRect(_this, line, rect);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult getLineIndices(const leString* _this,
                            uint32_t line,
                            uint32_t* start,
                            uint32_t* end)

  Summary:
    Gets the indices of a string line

  Description:
    Gets the indices of a string line

  Parameters:
    const leString* _this - The string to operate on
    uint32_t line - the line to query
    uint32_t* start - the starting offset of the line in the string
    uint32_t* end - the ending offset of the line in the string

  Remarks:
    Usage - _this->fn->getLineIndices(_this, line, start, end);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult getCharRect(const leString* _this,
                         uint32_t idx,
                         leRect* rect)

  Summary:
    Gets the rendered rectangle of a string codepoint

  Description:
    Gets the rendered rectangle of a string codepoint

  Parameters:
    const leString* _this - The string to operate on
    uint32_t idx - the leChar index to query
    leRect* rect - will contain the leChar bounding rectangle

  Remarks:
    Usage - _this->fn->getCharRect(_this, idx, rect);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult getCharIndexAtPoint(const leString* _this,
                                 const lePoint* pt,
                                 uint32_t* idx)

  Summary:
    Given a point, gets a character index in the string

  Description:
    Given a point, gets a character index in the string

  Parameters:
    const leString* _this - The string to operate on
    const lePoint* pt - the point to query
    uint32_t* idx - will contain the string index

  Remarks:
    Usage - _this->fn->getCharIndexAtPoint(_this, pt, idx);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult _draw(const leString* _this,
                   int32_t x,
                   int32_t y,
                   leHAlignment align,
                   leColor clr,
                   uint32_t a)

  Summary:
    Draw the string

  Description:
    Draw the string

  Parameters:
    const leString* _this - The string to operate on
    int32_t x - the X draw position (screen space)
    int32_t y - the Y draw position (screen space)
    leHAlignment align - the alignment of the string
    leColor clr - the color to write
    uint32_t a - a global alpha value to apply

  Remarks:
    Usage - _this->fn->_draw(_this, x, y, align, clr, a);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    void preinvalidate(leString* _this)

  Summary:
    Raise a string pre-invalidate event

  Description:
    Raise a string pre-invalidate event

  Parameters:
    leString* _this - The string to operate on

  Remarks:
    Usage - _this->fn->preinvalidate(_this);

  Returns:
    void
*/

// *****************************************************************************
/* Virtual Member Function:
    void invalidate(leString* _this)

  Summary:
    Sets a string pre-invalidate event

  Description:
    Sets a string pre-invalidate event

  Parameters:
    leString* _this - The string to operate on

  Remarks:
    Usage - _this->fn->invalidate(_this);

  Returns:
    void
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setPreInvalidateCallback(leString* _this,
                                      leString_InvalidateCallback,
                                      void* userData)

  Summary:
    Raise a string invalidate event

  Description:
    Raise a string invalidate event

  Parameters:
    leString* _this - The string to operate on
    leString_InvalidateCallback - the callback pointer to set
    void* userData - a user data pointer to pass to the callback

  Remarks:
    Usage - _this->fn->setPreInvalidateCallback(_this, leString_InvalidateCallback, userData);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setInvalidateCallback(leString* _this,
                                   leString_InvalidateCallback,
                                   void* userData)

  Summary:
    Sets a string invalidate event

  Description:
    Sets a string invalidate event

  Parameters:
    leString* _this - The string to operate on
    leString_InvalidateCallback - the callback pointer to set
    vvoid* userData - a user data pointer to pass to the callback

  Remarks:
    Usage - _this->fn->setInvalidateCallback(_this, leString_InvalidateCallback, userData);

  Returns:
    leResult - the result of the operation
*/


#endif /* LE_STRING_H */