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

/** \file legato_string.h
 * @brief Fixed string functions and definitions.
 *
 * @details This is a string library implementation that is used internally by
 * the Legato user interface library.
 *
 * This implementation relies on the leChar definition for characters. This chararcter
 * definition is 16 bits in size and allows the library to support international character
 * code points and Unicode encoding standards
 */

#ifndef LEGATO_STRING_H
#define LEGATO_STRING_H

#include "gfx/legato/common/legato_common.h"

#include "gfx/legato/font/legato_font.h"
#include "gfx/legato/common/legato_error.h"

/* internal use only */
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
    leResult    (*setInvalidateCallback)(THIS_TYPE* _this, leString_InvalidateCallback, void* userData); \

typedef struct leStringVTable
{
	LE_STRING_VTABLE(struct leString)
} leStringVTable;

// *****************************************************************************
/**
 * @brief This struct represents a string.
 * @details Generic string definition
 */
typedef struct leString
{
    const leStringVTable* fn;                   /**< function table */

    leString_InvalidateCallback preInvCallback; /**< pre invalidate callback */
    void* preCBUserData;                        /**< pre invalidate callback data */

    leString_InvalidateCallback invCallback;    /**< invalidate callback */
    void* invCBUserData;                        /**< invalidate callback data */
} leString;

// *****************************************************************************
/**
 * @brief Delete string.
 * @details Deletes <span class="param">str</span>. Frees the memory that was
 * allocated using the la{TYPE}String_New type allocator functions.  This
 * function is used to delete all string-type objects that were allocated
 * with their class-specific new functions.
 * @see leString_New().
 * @code
 * leString * str;
 * leString_Delete(str);
 * @endcode
 * @param str is the string to delete.
 * @return void.
 */
void leString_Delete(leString* str);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************

/**
 * @brief Destruct string.
 * @details Destruct the string <span class="param">_this</span>.
 * @code
 * leString*  _this;
 * str->fn->destructor(str);
 * @endcode
 * @param _this is the string to destruct.
 * @return void.
 */
virtual void destructor(leString* _this);

// *****************************************************************************
/**
 * @brief Get font.
 * @details Gets font using <span class="param">_this</span>.
 * @code
 * leString*  _this;
 * str->fn->getFont(_this);
 * @endcode
 * @param _this is the string to query.
 * @return the font assigned to the string.
 */
virtual leFont* getFont(const leString* _this);


// *****************************************************************************
/**
 * @brief Set font.
 * @details Sets the font using <span class="param">_this</span> to <span class="param">font</span>.
 * @code
 * leString*  _this;
 * leResult res = _this->fn->setFont(font);
 * @endcode
 * @param font is the font to set.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFont(leString* _this,
                         const leFont* font);


// *****************************************************************************
/**
 * @brief Set string value from another string.
 * @details Sets the string value from <span class="param">src</span>.
 * @code
 * leString* _this;
 * struct leString* src;
 * leResult res =_this->fn->setFromString(_this, src);
 * @endcode
 * @param src is the string to set.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFromString(leString* _this,
                               const struct leString* src);

// *****************************************************************************
/**
 * @brief Set string value from a leChar buffer.
 * @details Sets the string value from <span class="param">buf</span> using the
 * first <span class="param">size</span> bytes.
 * @code
 * leString* _this;
 * leChar* buf;
 * uint32_t size;
 * leResult res = str->fn->setFromString(str, buf, size);
 * @endcode
 * @param buf is the string to set.
 * @param size is the number of chars to set.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFromChar(leString* _this,
                             const leChar* buf,
                             uint32_t size);


// *****************************************************************************
/**
 * @brief Set string value from a C string buffer.
 * @details Sets the string value from <span class="param">cstr</span>.
 * @code
 * leString*  _this;
 * const char* cstr;
 * leResult res = str->fn->setFromString(_this, cstr);
 * @endcode
 * @param cstr is the string to set.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFromCStr(leString* _this,
                             const char* cstr);


// *****************************************************************************
/**
 * @brief Get codepoint at the given index.
 * @details Gets the codepoint at <span class="param">index</span>
 * using <span class="param">_this</span>.
 * @code
 * leString*  str;
 * uint32_t idx;
 * leChar chr = str->fn->charAt(str, idx);
 * @endcode
 * @param str is the string to query.
 * @param idx is the location within the str.
 * @return the leChar contained at the index or -1.
 */
virtual leChar charAt(const leString* _this,
                      uint32_t idx);

// *****************************************************************************
/**
 * @brief Get length of the string.
 * @details Gets length of string at
 * <span class="param">idx</span> using <span class="param">_this</span>.
 * @code
 * leString*  _this;
 * uint32_t idx;
 * uint32_t length = str->fn->charAt(_this, idx);
 * @endcode
 * @param _this is the string to query.
 * @param idx is the location within the str.
 * @return length of the string.
 */
virtual uint32_t length(const leString* _this);

// *****************************************************************************
/**
 * @brief Determine if string is empty.
 * @details Checks if string is empty using <span class="param">_this</span>.
 * @code
 * leString*  _this;
 * leBool empty = str->fn->isEmpty(_this);
 * @endcode
 * @param _this is the string to query.
 * @return returns LE_TRUE if string is empty, otherwise LE_FALSE.
 */
virtual leBool isEmpty(const leString* _this);


// *****************************************************************************
/**
 * @brief Compare string to another string.
 * @details Compares the string to <span class="param">tgt</span>.
 * @code
 * leString*  _this;
 * const struct leString* tgt;
 * int32_t res = str->fn->compare(str, tgt);
 * @endcode
 * @param str is the string to compare against.
 * @param tgt is the string to compare with.
 * @return returns an integer indicating the result of the comparison. It returns 0, if
 * both strings are the same.
 */
virtual int32_t compare(const leString* _this,
                        const struct leString* tgt);


// *****************************************************************************
/**
 * @brief Append string.
 * @details Appends a string <span class="param">val</span> using <span class="param">_this</span>.
 * @code
 * leString* _this;
 * leString* val;
 * leResult res = str->fn->append(_this, val);
 * @endcode
 * @param _this is the string to append to.
 * @param val is the string to append with.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult append(leString* _this,
                        const struct leString* val);

// *****************************************************************************
/**
 * @brief Insert string at a given index.
 * @details Inserts a string <span class="param">val</span> at
 *  <span class="param">idx</span> using
 * <span class="param">_this</span>.
 * @code
 * leString* _this;
 * leString* val;
 * uint32_t idx;
 * leResult res = str->fn->append(_this, val, idx);
 * @endcode
 * @param str is the string to append to.
 * @param val is the string to append with.
 * @param idx is the string to append with.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult insert(leString* _this,
                        const struct leString* val,
                        uint32_t idx);

// *****************************************************************************
/**
 * @brief Remove substring.
 * @details Removes a substring at <span class="param">idx</span> for up to
 * <span class="param">count</span> characters using <span class="param">_this</span>.
 * @code
 * leString* _this;
 * leString* idx;
 * uint32_t count;
 * leResult res = str->fn->remove(_this, idx, count);
 * @endcode
 * @param _this is the string to append to.
 * @param idx is the string to append with.
 * @param count is the number of chars
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 *
 */
 virtual leResult remove(leString* _this,
                    uint32_t idx,
                    uint32_t count);

// *****************************************************************************
/**
 * @brief Clear string.
 * @details Clears the contents of string using <span class="param">_this</span>.
 * @code
 * leString* _this;
 * leString* val;
 * str->fn->clear(_this);
 * @endcode
 * @param _this is the string to append to.
 * @return void.
 */
virtual void clear(leString* _this);

// *****************************************************************************
/**
 * @brief Convert string to a C-style string.
 * @details Converts the string to <span class="param">buf</span> for up to
 * <span class="param">size</span> characters using <span class="param">_this</span>.
 * @code
 * const leString* _this;
 * leChar*  buf;
 * uint32_t size;
 * uint32_t nbr = str->fn->toChar(_this, buf, size);
 * @endcode
 * @param _this is the string to convert.
 * @param buf is the new string.
 * @param size is the number of chars to convert.
 * @return the number of leChar written to the buffer.
 */
virtual uint32_t toChar(const leString* _this,
                   leChar* buf,
                   uint32_t size);
virtual
// *****************************************************************************
/**
 * @brief Get rectangle of the rendered string in pixels.
 * @details Gets the rectangle of the rendered string to
 * pixels <span class="param">rect</span> using <span class="param">_this</span>.
 * @code
 * leString* str;
 * leRect* rect;
 * leResult res = str->fn->getRect(str, rect);
 * @endcode
 * @param str is the string to examine.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult getRect(const leString* _this,
                         leRect* rect);


// *****************************************************************************
/**
 * @brief Get number of lines in the string.
 * @details Gets the number of lines using <span class="param">_this</span>.
 * @code
 * leString* _this;
 * uint32_t count = str->fn->getLineCount(_this);
 * @endcode
 * @param _this is the string to examine.
 * @return the number of lines in string.
 */
uint32_t getLineCount(const leString* _this);


// *****************************************************************************
/**
 * @brief Get rectangle of a string line.
 * @details Gets the <span class="param">rect</span> of
 * <span class="param">_this</span> at <span class="param">line</span>.
 * @code
 * uint32_t line;
 * leRect* rect;
 * leResult res = str->fn->getLineCount(_this, line, rect);
 * @endcode
 * @param _this is the string to examine.
 * @param line
 * @param rect
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult getLineRect(const leString* _this,
                             uint32_t line,
                             leRect* rect);


// *****************************************************************************
/**
 * @brief Get indices of a string line.
 * @details Gets the indices of <span class="param">line</span> from
 * <span class="param">start</span> to <span class="param">end</span>.
 * @code
 * leString* _this;
 * uint32_t* start;
 * uint32_t* end;
 * leResult res = str->fn->getLineIndices(_this, line, start, end);
 * @endcode
 * @param _this is the string to examine.
 * @param line the line to query
 * @param start the starting offset of the line in the string
 * @param end the ending offset of the line in the string
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult getLineIndices(const leString* _this,
                                uint32_t line,
                                uint32_t* start,
                                uint32_t* end);


// *****************************************************************************
/**
 * @brief Get rendered rectangle of a string codepoint.
 * @details Gets the rendered <span class="param">rect</span> of a char
 * at <span class="param">index</span>.
 * @code
 * leString* _this;
 * uint32_t idx;
 * leRect* rect;
 * leResult res = str->fn->getCharRect(_this, idx, rect);
 * @endcode
 * @param str is the string to examine.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult getCharRect(const leString* _this,
                     uint32_t idx,
                     leRect* rect);

// *****************************************************************************
/**
 * @brief Get character index at point.
 * @details Gets a character <span class="param">idx</span> at
 * <span class="param">pt</span>.
 * @code
 * leString* _this;
 * uint32_t idx;
 * lePoint* pt;
 * leResult res = str->fn->getCharIndexAtPoint(_this, pt, idx);
 * @endcode
 * @param str is the string to examine.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult getCharIndexAtPoint(const leString* _this,
                                     const lePoint* pt,
                                     uint32_t* idx);


// *****************************************************************************
/**
 * @brief Draw.
 * @details Draws string at location <span class="param">x</span> and
 * <span class="param">y</span> with the specified
 * <span class="param">align</span>
 * <span class="param">clr</span>
 * <span class="param">a</span>.
 * @code
 * leString* _this;
 * uint32_t idx;
 * lePoint* pt;
 * leResult res = str->fn->_draw(_this, x, y, align, clr, a);
 * @endcode
 * @param str is the string to examine.
 * @return leResult.
 */
virtual leResult _draw(const leString* _this,
                       int32_t x,
                       int32_t y,
                       leHAlignment align,
                       leColor clr,
                       uint32_t a);


// *****************************************************************************
/**
 * @brief Raise pre-invalidate event.
 * @details Raises a pre-invalidate event for <span class="param">_this</span>.
 * @code
 * leString* _this;
 * str->fn->preinvalidate(_this);
 * @endcode
 * @param str is the string to examine.
 * @return leResult.
 */
virtual void preinvalidate(leString* _this);

// *****************************************************************************
/**
 * @brief Invalidate.
 * @details Sets the pre invalidate callback to <span class="param">func</span> with
 * <span class="param">userData</span> arguments.
 * @code
 * leString* _this;
 * str->fn->setPreInvalidateCallback(_this, func, userData);
 * @endcode
 * @param str is the string to examine.
 * @param func is of type leString_InvalidateCallback.
 * @param userData is arguments to receive.
 * @return leResult.
 */
virtual void invalidate(leString* _this);

// *****************************************************************************
/**
 * @brief Set string invalidate event.
 * @details Sets the string invalidate event to <span class="param">cb</span>
 * using <span class="param">_this</span> with <span class="param">userData</span>.
 * @code
 * leString* _this;
 * leString_InvalidateCallback cb;
 * leResult res = _this->fn->setPreInvalidateCallback(_this, cb, userData);
 * @endcode
 * @param _this is the string to modify
 * @param cb is the callback func
 * @param is the  user data pointer to pass to the callback
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPreInvalidateCallback(leString* _this,
                                          leString_InvalidateCallback,
                                          void* userData);

// *****************************************************************************
/**
 * @brief Set string invalidate event.
 * @details Sets the string invalidate event to <span class="param">cb</span>
 * using <span class="param">_this</span> with <span class="param">userData</span>.
 * @code
 * leString* _this;
 * leString_InvalidateCallback cb;
 * leResult res = _this->fn->setInvalidateCallback(_this, cb, userData);
 * @endcode
 * @param _this is the string to modify
 * @param cb is the callback func
 * @param is the  user data pointer to pass to the callback
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setInvalidateCallback(leString* _this,
                                       leString_InvalidateCallback cb,
                                       void* userData);

#undef THIS_TYPE
#endif

#endif /* LE_STRING_H */
