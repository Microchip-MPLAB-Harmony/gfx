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
    legato_stringutils.h

  Summary:


  Description:
    This module implements various string utility functions.
*******************************************************************************/


#ifndef LE_STRINGUTILS_H
#define LE_STRINGUTILS_H

#include "gfx/legato/string/legato_string.h"

// *****************************************************************************
/* Function:
    uint32_t leStringUtils_FromCStr(const char* str,
                                    leChar* buf,
                                    uint32_t size)

  Summary:
     Converts a C-style string into a leChar buffer

  Description:
     Converts a C-style string into a leChar buffer

  Parameters:
    const char* str - the source C-style string
    leChar* buf - the destination leChar string
    uint32_t size - the size of the destination string in leChar

  Remarks:

  Returns:
    uint32_t - the number of codepoints copied
*/
LIB_EXPORT uint32_t leStringUtils_FromCStr(const char* str,
                                           leChar* buf,
                                           uint32_t size);

// *****************************************************************************
/* Function:
    uint32_t leStringUtils_ToCStr(const leChar* str,
                                  uint32_t strSize,
                                  char* buf,
                                  uint32_t bufSize)

  Summary:
     Converts an leChar style buffer to a C-style string

  Description:
     Converts an leChar style buffer to a C-style string

  Parameters:
    const leChar* str - the source leChar stringthe leChar string to copy
    uint32_t strSize - the size of the source leChar string
    char* buf - the destination C string
    uint32_t bufSize - the size of the destination buffer

  Remarks:

  Returns:
    uint32_t - the number of chars copied
*/
LIB_EXPORT uint32_t leStringUtils_ToCStr(const leChar* str,
                                         uint32_t strSize,
                                         char* buf,
                                         uint32_t bufSize);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetRect(const leChar* str,
                                   uint32_t size,
                                   const leFont* font,
                                   leRect* rect)

  Summary:
     Gets the bounding rectangle for a leChar style string

  Description:
     Gets the bounding rectangle for a leChar style string

  Parameters:
    const leChar* str - the source leChar string
    uint32_t size - the size of the leChar string in code points
    const leFont* font - the font to reference
    leRect* rect - will contain the bounding information

  Remarks:

  Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leStringUtils_GetRect(const leChar* str,
                                          uint32_t size,
                                          const leFont* font,
                                          leRect* rect);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetRectCStr(const char* str,
                                       const leFont* font,
                                       leRect* rect)

  Summary:
     Gets the bounding rectangle for a C-style string

  Description:
     Gets the bounding rectangle for a C-style string

  Parameters:
    const char* str - the c string to analyze
    const leFont* font - the font to reference
    leRect* rect - will contain the bounding information

  Remarks:

  Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leStringUtils_GetRectCStr(const char* str,
                                              const leFont* font,
                                              leRect* rect);

// *****************************************************************************
/* Function:
    uint32_t leStringUtils_GetLineCount(const leChar* str,
                                        uint32_t size)

  Summary:
     Gets the line count for a leChar style string

  Description:
     Gets the line count for a leChar style string

  Parameters:
    const leChar* str - the source leChar string
    uint32_t size - the size of the input buffer in code points

  Remarks:

  Returns:
    uint32_t - the number of lines in the string
*/
LIB_EXPORT uint32_t leStringUtils_GetLineCount(const leChar* str,
                                               uint32_t size);

// *****************************************************************************
/* Function:
    uint32_t leStringUtils_GetLineCountCStr(const char* str);

  Summary:
     Gets the line count for a C-style string

  Description:
     Gets the line count for a C-style string

  Parameters:
    const char* str - the source C string to analyze

  Remarks:

  Returns:
    uint32_t - the number of lines in the string
*/
LIB_EXPORT uint32_t leStringUtils_GetLineCountCStr(const char* str);

// *****************************************************************************
/* Function:
    leResult leStringUtils_GetLineIndices(const leChar* str,
                                          uint32_t size,
                                          uint32_t line,
                                          uint32_t* start,
                                          uint32_t* end)

  Summary:
     Gets the line indices for a leChar style string

  Description:
     Gets the line indices for a leChar style string

  Parameters:
    const leChar* str - the source leChar string
    uint32_t size - the size of the input buffer in code points
    uint32_t line - the line index to query
    uint32_t* start - will contain the start index of the line
    uint32_t* end - will contain the end index of the line

  Remarks:

  Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leStringUtils_GetLineIndices(const leChar* str,
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
     Gets the line indices for a C-style string

  Description:
     Gets the line indices for a C-style string

  Parameters:
    const char* str - the source C-string to analyze
    uint32_t line - the line index to query
    uint32_t* start - will contain the start index of the line
    uint32_t* end - will contain the end index of the line

  Remarks:

  Returns:
    leResult - the result of the operation
*/
LIB_EXPORT leResult leStringUtils_GetLineIndicesCStr(const char* str,
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
LIB_EXPORT leResult leStringUtils_GetLineRect(const leChar* str,
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
LIB_EXPORT leResult leStringUtils_GetLineRectCStr(const char* str,
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
LIB_EXPORT leResult leStringUtils_GetCharRect(const leChar* str,
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
LIB_EXPORT leResult leStringUtils_GetCharRectCStr(const char* str,
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
LIB_EXPORT leResult leStringUtils_GetCharIndexAtPoint(const leChar* str,
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
LIB_EXPORT leResult leStringUtils_GetCharIndexAtPointCStr(const char* str,
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
LIB_EXPORT leResult leDecodeCodePoint(uint32_t encoding,
                                      uint8_t* data,
                                      uint32_t max,
                                      uint32_t* codePoint,
                                      uint32_t* offset);

#endif /* LE_STRINGUTILS_H */
