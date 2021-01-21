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
    legato_stringutils.h

  Summary:


  Description:
    This module implements various string utility functions.
*******************************************************************************/
/** \file legato_stringutils.h
 * @brief String utility functions and definitions.
 *
 * @details This module implements various string utility functions..
 */

#ifndef LE_STRINGUTILS_H
#define LE_STRINGUTILS_H

#include "gfx/legato/string/legato_string.h"

// *****************************************************************************
/**
 * @brief Convert c-style string to leChar.
 * @details Converts up to <span class="param">size</span> elements of
 * c-style string <span class="param">str</span>
 * to <span class="param">buf</span>.
 * @code
 * const char* str;
 * leChar* buf;
 * uint32_t size;
 * uint32_t cp = leStringUtils_FromCStr(str, buf, size);
 * @endcode
 * @param str is the source C-style string.
 * @param buf the destination leChar string.
 * @param size is the size of the destination string in leChar
 * @return returns the number of codepoints copied.
 */
uint32_t leStringUtils_FromCStr(const char* str,
                                leChar* buf,
                                uint32_t size);

// *****************************************************************************
/**
 * @brief Convert leChar buffer to C-style string.
 * @details Converts leChar buffer at most <span class="param">strSize</span>
 * of <span class="param">str</span> to <span class="param">buf</span>
 * of size <span class="param">bufSize</span>.
 * @code
 * const leChar* str;
 * uint32_t strSize;
 * char* buf;
 * uint32_t bufSize;
 * uint32_t cp = leStringUtils_ToCStr(str, strSize, buf, size, bufSize);
 * @endcode
 * @param str is an integer.
 * @param buf is an integer.
 * @return the number of chars copied
 */
uint32_t leStringUtils_ToCStr(const leChar* str,
                              uint32_t strSize,
                              char* buf,
                              uint32_t bufSize);

// *****************************************************************************
/**
 * @brief Get  bounding rectangle for a leChar style string.
 * @details Gets the bounding <span class="param">rect</span> for
 * up to <span class="param">size</span> characters of
 * <span class="param">str</span> with <span class="param">font</span>.
 * @code
 * leResult res = leStringUtils_GetRect(str, size, font, rect);
 * @endcode
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringUtils_GetRect(const leChar* str,
                               uint32_t size,
                               const leFont* font,
                               leRect* rect);

// *****************************************************************************
/**
 * @brief Gets the bounding rectangle for a C-style string.
 * @details Gets the bounding <span class="param">rect</span>  of
 * <span class="param">str</span> with <span class="param">font</span>.
 * @code
 * const char* str;
 * const leFont* font;
 * leRect* rect;
 * leResult res = leStringUtils_GetRectCStr(str, font, rect);
 * @endcode
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringUtils_GetRectCStr(const char* str,
                                   const leFont* font,
                                   leRect* rect);

// *****************************************************************************
/**
 * @brief Get  line count for a leChar style string.
 * @details Gets the line count for <span class="param">str</span> with
 * <span class="param">size</span>.
 * @code
 * const leChar* str;
 * uint32_t size;
 * uint32_t res = leStringUtils_GetLineCount(str, size);
 * @endcode
 * @param str is an leChar string.
 * @param size is an integer.
 * @return the number of lines in the string.
 */
uint32_t leStringUtils_GetLineCount(const leChar* str,
                                    uint32_t size);

// *****************************************************************************
/**
 * @brief Get line count for a C-style string.
 * @details Gets the line count for <span class="param">str</span>.
 * @code
 * const char* str;
 * uint32_t res = leStringUtils_GetLineCountCStr(str);
 * @endcode
 * @param str is an integer.
 * @return the number of lines in the string.
 */
uint32_t leStringUtils_GetLineCountCStr(const char* str);

// *****************************************************************************
/**
 * @brief Get line indices for a leChar style string.
 * @details Gets the line indices for <span class="param">str</span>
 * with <span class="param">size</span> at <span class="param">line</span>
 * from <span class="param">start</span> to <span class="param">end</span> .
 * @code
 * const leChar* str;
 * uint32_t size;
 * uint32_t line;
 * uint32_t* start;
 * uint32_t* end;
 * uint32_t res = leStringUtils_GetLineIndices(str, size, line, start, end);
 * @endcode
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringUtils_GetLineIndices(const leChar* str,
                                      uint32_t size,
                                      uint32_t line,
                                      uint32_t* start,
                                      uint32_t* end);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetLineIndicesCStr(const char* str,
                                              uint32_t line,
                                              uint32_t* start,
                                              uint32_t* end)

  Summary:


  Description:
     Gets the line indices for a C-style string

  Parameters:
    const char* str - the source C-string to aGets the line indices for a C-style stringnalyze
    uint32_t line - the line index to query
    uint32_t* start - will contain the start index of the line
    uint32_t* end - will contain the end index of the line

  Remarks:

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Gets the line indices for a C-style string.
 * @details Gets the line indices for a C-style string.
 * @code
 * leResult res = leStringUtils_GetLineIndicesCStr(str);
 * @endcode
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringUtils_GetLineIndicesCStr(const char* str,
                                          uint32_t line,
                                          uint32_t* start,
                                          uint32_t* end);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetLineRect(const leChar* str,
                                       uint32_t size,
                                       const leFont* font,
                                       uint32_t line,
                                       leRect* rect)

  Summary:
     Gets the line bounding rectangle for a leChar style string

  Description:
     Gets the line bounding rectangle for a leChar style string

  Parameters:
    const leChar* str - the source leChar string
    uint32_t size - the size of the input buffer in code points
    const leFont* font - the font to reference
    uint32_t line - the line index to reference
    leRect* rect - will contain the line bounding rectangle

  Remarks:

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Gets the line bounding rectangle for a leChar style string.
 * @details Gets the line bounding rectangle for a leChar style string.
 * @code
 * leResult res = leStringUtils_GetLineRect(str);
 * @endcode
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringUtils_GetLineRect(const leChar* str,
                                   uint32_t size,
                                   const leFont* font,
                                   uint32_t line,
                                   leRect* rect);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetLineRectCStr(const char* str,
                                           const leFont* font,
                                           uint32_t line,
                                           leRect* rect)

  Summary:
     Gets the line bounding rectangle for a C-style string

  Description:
     Gets the line bounding rectangle for a C-style string

  Parameters:
    const char* str - the source C string
    const leFont* font - the font to reference
    uint32_t line - the line index to reference
    leRect* rect - will contain the line bounding rectangle

  Remarks:

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Gets the line bounding rectangle for a C-style string.
 * @details Gets the line bounding rectangle for a C-style string.
 * @code
 * leResult res = leStringUtils_GetLineRectCStr(str);
 * @endcode
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */

leResult leStringUtils_GetLineRectCStr(const char* str,
                                       const leFont* font,
                                       uint32_t line,
                                       leRect* rect);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetCharRect(const leChar* str,
                                       uint32_t size,
                                       const leFont* font,
                                       uint32_t charIdx,
                                       leRect* rect)

  Summary:
     Gets a glyph bounding rectangle for a leChar style string

  Description:
     Gets a glyph bounding rectangle for a leChar style string

  Parameters:
    const leChar* str - the source leChar string
    uint32_t size - the size of the input buffer in code points
    const leFont* font - the font to reference
    uint32_t charIdx - the char index to reference
    leRect* rect - will contain the glyph bounding rectangle

  Remarks:

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Gets a glyph bounding rectangle for a leChar style string.
 * @details Gets a glyph bounding rectangle for a leChar style string.
 * @code
 * leResult res = leStringUtils_GetCharRect(str);
 * @endcode
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringUtils_GetCharRect(const leChar* str,
                                   uint32_t size,
                                   const leFont* font,
                                   uint32_t charIdx,
                                   leRect* rect);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetCharRectCStr(const char* str,
                                           const leFont* font,
                                           uint32_t charIdx,
                                           leRect* rect)

  Summary:
     Gets a glyph bounding rectangle for a C-style string

  Description:
     Gets a glyph bounding rectangle for a C-style string

  Parameters:
    const char* str - the source C string
    const leFont* font - the font to reference
    uint32_t charIdx - the char index to reference
    leRect* rect - will contain the glyph bounding rectangle

  Remarks:

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Gets a glyph bounding rectangle for a C-style string.
 * @details Gets a glyph bounding rectangle for a C-style string.
 * @code
 * leResult res = leStringUtils_GetCharRectCStr(str);
 * @endcode
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringUtils_GetCharRectCStr(const char* str,
                                       const leFont* font,
                                       uint32_t charIdx,
                                       leRect* rect);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetCharIndexAtPoint(const leChar* str,
                                               uint32_t size,
                                               const leFont* font,
                                               const lePoint pt,
                                               uint32_t* charIdx)

  Summary:
     Gets the glyph at a point inside a leChar string bounding rectangle

  Description:
     Gets the glyph at a point inside a leChar string bounding rectangle

  Parameters:
    const leChar* str - the source leChar string
    uint32_t size - the size of the input buffer in code points
    const leFont* font - the font to reference
    const lePoint pt - the point to query with
    uint32_t* charIdx - will contain the index of the char at pt

  Remarks:

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Gets the glyph at a point inside a leChar string bounding rectangle.
 * @details Gets the glyph at a point inside a leChar string bounding rectangle.
 * @code
 * leResult res = leStringUtils_GetCharRectCStr(str);
 * @leStringUtils_GetCharIndexAtPoint
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringUtils_GetCharIndexAtPoint(const leChar* str,
                                           uint32_t size,
                                           const leFont* font,
                                           const lePoint pt,
                                           uint32_t* charIdx);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetCharIndexAtPointCStr(const char* str,
                                                   const leFont* font,
                                                   const lePoint pt,
                                                   uint32_t* charIdx)

  Summary:
     Gets the glyph at a point inside a C-style string bounding rectangle

  Description:
     Gets the glyph at a point inside a C-style string bounding rectangle

  Parameters:
    const char* str - the source C string
    const leFont* font - the font to reference
    const lePoint pt - the point to query with
    uint32_t* charIdx - will contain the index of the char at pt

  Remarks:

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Gets the glyph at a point inside a C-style string bounding rectangle.
 * @details Gets the glyph at a point inside a C-style string bounding rectangle.
 * @code
 * leResult res = leStringUtils_GetCharIndexAtPointCStr(str);
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringUtils_GetCharIndexAtPointCStr(const char* str,
                                               const leFont* font,
                                               const lePoint pt,
                                               uint32_t* charIdx);

// *****************************************************************************
/* Function:
    leResult leDecodeCodePoint(uint32_t encoding,
                               uint8_t* data,
                               uint32_t max,
                               uint32_t* codePoint,
                               uint32_t* offset)

  Summary:
     Given an encoded codepoint (ASCII, UTF-8, or UTF-16) returns the actual
     codepoint

  Description:
     Given an encoded codepoint (ASCII, UTF-8, or UTF-16) returns the actual
     codepoint

  Parameters:
    uint32_t encoding - the encoding of the value
    uint8_t* data - the data to decode
    uint32_t max - the size of the data buffer
    uint32_t* codePoint - will contain the decoded code point
    uint32_t* offset - will contain the offset in the buffer after the decode
                       point

  Remarks:

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Given an encoded codepoint (ASCII, UTF-8, or UTF-16) returns the
 * actual codepoint.
 * @details Given an encoded codepoint (ASCII, UTF-8, or UTF-16) returns the
 * actual codepoint.
 * @code
 * leResult res = leDecodeCodePoint(str);
 * @param str is an integer.
 * @param buf is an integer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leDecodeCodePoint(uint32_t encoding,
                           uint8_t* data,
                           uint32_t max,
                           uint32_t* codePoint,
                           uint32_t* offset);

#endif /* LE_STRINGUTILS_H */
