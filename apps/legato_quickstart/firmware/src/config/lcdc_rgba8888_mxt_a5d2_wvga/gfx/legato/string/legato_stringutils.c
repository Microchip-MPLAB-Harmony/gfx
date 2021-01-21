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

#include "gfx/legato/string/legato_stringutils.h"

#include "gfx/legato/common/legato_rect.h"

#define ASCII_SPACE 0x20
#define ASCII_MIN   0x20
#define ASCII_MAX   0x7E

#define LINE_BREAK  0xA

uint32_t leStringUtils_FromCStr(const char* str,
                                leChar* buf,
                                uint32_t size)
{
    uint32_t len, itr;

    if(str == NULL || buf == NULL)
        return LE_FAILURE;

    len = strlen(str);

    for(itr = 0; itr < len; itr++)
    {
        if(itr >= size)
            break;

        buf[itr] = str[itr];
    }

    return itr;
}

uint32_t leStringUtils_ToCStr(const leChar* str,
                              uint32_t strSize,
                              char* buf,
                              uint32_t bufSize)
{
    uint32_t itr;

    if(str == NULL || strSize == 0 || buf == NULL || bufSize == 0)
        return LE_FAILURE;

    for(itr = 0; itr < strSize; itr++)
    {
        if(itr >= bufSize)
            break;

        if(str[itr] >= ASCII_MIN && str[itr] <= ASCII_MAX)
        {
            buf[itr] = (char)str[itr];
        }
        else
        {
            buf[itr] = ASCII_SPACE;
        }
    }

    return itr;
}

leResult leStringUtils_GetRect(const leChar* str,
                               uint32_t size,
                               const leFont* font,
                               leRect* rect)
{
    uint32_t idx;
    uint32_t lines;
    leRect lineRect = leRect_Zero;
    leRasterFont* rasFnt = (leRasterFont*)font;

    *rect = leRect_Zero;

    if(str == NULL)
        return LE_FAILURE;

    if(size == 0)
        return LE_SUCCESS;

    lines = leStringUtils_GetLineCount(str, size);

    for(idx = 0; idx < lines; idx++)
    {
        leStringUtils_GetLineRect(str, size, font, idx, &lineRect);

        if(idx < lines - 1)
        {
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

leResult leStringUtils_GetRectCStr(const char* str,
                                   const leFont* font,
                                   leRect* rect)
{
    uint32_t idx;
    uint32_t lines;
    leRect lineRect = leRect_Zero;
    uint32_t size;
    leRasterFont* rasFnt = (leRasterFont*)font;

    *rect = leRect_Zero;

    if(str == NULL)
        return LE_FAILURE;

    size = strlen(str);

    if(size == 0)
        return LE_SUCCESS;

    lines = leStringUtils_GetLineCountCStr(str);

    for(idx = 0; idx < lines; idx++)
    {
        leStringUtils_GetLineRectCStr(str, font, idx, &lineRect);

        if(idx < lines - 1)
        {
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

uint32_t leStringUtils_GetLineCount(const leChar* str,
                                    uint32_t size)
{
    uint32_t idx;
    uint32_t count;

    if(str == NULL || size == 0)
        return 0;

    count = 1;

    for(idx = 0; idx < size; idx++)
    {
        if(str[idx] == LINE_BREAK)
        {
            count += 1;
        }
    }

    return count;
}

uint32_t leStringUtils_GetLineCountCStr(const char* str)
{
    uint32_t idx;
    uint32_t count;
    uint32_t size;

    if(str == NULL)
        return 0;

    size = strlen(str);

    if(size == 0)
        return LE_FAILURE;

    count = 1;

    for(idx = 0; idx < size; idx++)
    {
        if(str[idx] == LINE_BREAK)
        {
            count += 1;
        }
    }

    return count;
}

leResult leStringUtils_GetLineIndices(const leChar* str,
                                      uint32_t size,
                                      uint32_t line,
                                      uint32_t* start,
                                      uint32_t* end)
{
    uint32_t idx;
    uint32_t count;

    if(str == NULL)
        return LE_FAILURE;

    if(size == 0 || line >= leStringUtils_GetLineCount(str, size))
        return LE_FAILURE;

    count = 0;

    *start = 0;
    *end = 0;

    for(idx = 0; idx < size; idx++)
    {
        if(str[idx] == LINE_BREAK)
        {
            *end = idx;

            if(count == line)
                return LE_SUCCESS;

            count += 1;

            /* TODO test this */
            *start = idx + 1;
        }
    }

    *end = size;

    return LE_SUCCESS;
}

leResult leStringUtils_GetLineIndicesCStr(const char* str,
                                          uint32_t line,
                                          uint32_t* start,
                                          uint32_t* end)
{
    uint32_t idx;
    uint32_t count;
    uint32_t size;

    if(str == NULL)
        return LE_FAILURE;

    size = strlen(str);

    if(size == 0 || line >= leStringUtils_GetLineCountCStr(str))
        return LE_FAILURE;

    count = 0;

    *start = 0;
    *end = 0;

    for(idx = 0; idx < size; idx++)
    {
        if(str[idx] == LINE_BREAK)
        {
            *end = idx;

            if(count == line)
                return LE_SUCCESS;

            count += 1;

            /* TODO test this */
            *start = idx + 1;
        }
    }

    *end = size;

    return LE_SUCCESS;
}

leResult leStringUtils_GetLineRect(const leChar* str,
                                   uint32_t size,
                                   const leFont* font,
                                   uint32_t line,
                                   leRect* rect)
{
    uint32_t idx;
    leFontGlyph glyph;
    uint32_t startIdx = 0;
    uint32_t endIdx = 0;
    leRasterFont* rasFnt = (leRasterFont*)font;

    if(str == NULL ||
        size == 0 ||
        font == NULL ||
        rasFnt->glyphTable == NULL ||
        rect == NULL)
    {
        return LE_FAILURE;
    }

    *rect = leRect_Zero;

    if(line >= leStringUtils_GetLineCount(str, size))
    {
        return LE_FAILURE;
    }

    leStringUtils_GetLineIndices(str, size, line, &startIdx, &endIdx);

    for(idx = startIdx; idx < endIdx; idx++)
    {
        leFont_GetGlyphInfo(font, str[idx], &glyph);

        rect->width += glyph.advance;
    }

    rect->height = rasFnt->height;

    return LE_SUCCESS;
}

leResult leStringUtils_GetLineRectCStr(const char* str,
                                       const leFont* font,
                                       uint32_t line,
                                       leRect* rect)
{
    uint32_t idx;
    leFontGlyph glyph;
    uint32_t startIdx = 0;
    uint32_t endIdx = 0;
    uint32_t size;
    leRasterFont* rasFnt = (leRasterFont*)font;

    if(str == NULL ||
        font == NULL ||
        rasFnt->glyphTable == NULL ||
        rect == NULL)
    {
        return LE_FAILURE;
    }

    size = strlen(str);

    if(size == 0)
        return LE_FAILURE;

    *rect = leRect_Zero;

    if(line >= leStringUtils_GetLineCountCStr(str))
    {
        return LE_FAILURE;
    }

    leStringUtils_GetLineIndicesCStr(str, line, &startIdx, &endIdx);

    for(idx = startIdx; idx < endIdx; idx++)
    {
        leFont_GetGlyphInfo(font, str[idx], &glyph);

        rect->width += glyph.advance;
    }

    rect->height = rasFnt->height;

    return LE_SUCCESS;
}

leResult leStringUtils_GetCharRect(const leChar* str,
                                   uint32_t size,
                                   const leFont* font,
                                   uint32_t charIdx,
                                   leRect* rect)
{
    uint32_t idx;
    leFontGlyph glyph;
    leRasterFont* rasFnt = (leRasterFont*)font;

    if(str == NULL ||
       size == 0 ||
       charIdx >= size ||
       font == NULL ||
       rasFnt->glyphTable == NULL ||
       rect == NULL)
    {
        return LE_FAILURE;
    }

    *rect = leRect_Zero;

    for(idx = 0; idx < size; idx++)
    {
        if(str[idx] == LINE_BREAK)
        { }
        else if(idx == charIdx)
        {
            leFont_GetGlyphInfo(font, str[idx], &glyph);

            rect->width = glyph.advance;
            rect->height = rasFnt->height;

            return LE_SUCCESS;
        }
        else
        {
            leFont_GetGlyphInfo(font, str[idx], &glyph);

            rect->x += glyph.advance;
        }
    }

    return LE_FAILURE;
}

leResult leStringUtils_GetCharRectCStr(const char* str,
                                       const leFont* font,
                                       uint32_t charIdx,
                                       leRect* rect)
{
    uint32_t idx;
    leFontGlyph glyph;
    uint32_t size;
    leRasterFont* rasFnt = (leRasterFont*)font;

    if(str == NULL ||
        font == NULL ||
        rasFnt->glyphTable == NULL ||
        rect == NULL)
    {
        return LE_FAILURE;
    }

    size = strlen(str);

    if(size == 0 || charIdx >= size)
        return LE_FAILURE;

    *rect = leRect_Zero;

    for(idx = 0; idx < size; idx++)
    {
        if(str[idx] == LINE_BREAK)
        { }
        else if(idx == charIdx)
        {
            leFont_GetGlyphInfo(font, str[idx], &glyph);

            rect->width = glyph.advance;
            rect->height = rasFnt->height;

            return LE_SUCCESS;
        }
        else
        {
            leFont_GetGlyphInfo(font, str[idx], &glyph);

            rect->x += glyph.advance;
        }
    }

    return LE_FAILURE;
}

leResult leStringUtils_GetCharIndexAtPoint(const leChar* str,
                                           uint32_t size,
                                           const leFont* font,
                                           const lePoint pt,
                                           uint32_t* charIdx)
{
    uint32_t idx;
    leFontGlyph glyph;
    leRect rect;
    leRasterFont* rasFnt = (leRasterFont*)font;

    rect.x = 0;
    rect.y = 0;

    if(str == NULL ||
       size == 0 ||
       font == NULL ||
       rasFnt->glyphTable == NULL ||
       charIdx == NULL)
    {
        return LE_FAILURE;
    }

    for(idx = 0; idx < size; idx++)
    {
        if(str[idx] == LINE_BREAK)
        {
            rect.x = 0;
            rect.y += rasFnt->height;
        }
        else
        {
            leFont_GetGlyphInfo(font, str[idx], &glyph);

            rect.width = glyph.advance;
            rect.height = glyph.height;

            if(leRectContainsPoint(&rect, &pt) == LE_TRUE)
            {
                *charIdx = idx;

                return LE_SUCCESS;
            }

            rect.x += glyph.advance;
        }
    }

    return LE_FAILURE;
}

leResult leStringUtils_GetCharIndexAtPointCStr(const char* str,
                                               const leFont* font,
                                               const lePoint pt,
                                               uint32_t* charIdx)
{
    uint32_t idx;
    leFontGlyph glyph;
    leRect rect;
    uint32_t size;
    leRasterFont* rasFnt = (leRasterFont*)font;

    rect.x = 0;
    rect.y = 0;

    if(str == NULL ||
        font == NULL ||
        rasFnt->glyphTable == NULL ||
        charIdx == NULL)
    {
        return LE_FAILURE;
    }

    size = strlen(str);

    if(size == 0)
        return LE_FAILURE;

    for(idx = 0; idx < size; idx++)
    {
        if(str[idx] == LINE_BREAK)
        {
            rect.x = 0;
            rect.y += rasFnt->height;
        }
        else
        {
            leFont_GetGlyphInfo(font, str[idx], &glyph);

            rect.width = glyph.advance;
            rect.height = glyph.height;

            if(leRectContainsPoint(&rect, &pt) == LE_TRUE)
            {
                *charIdx = idx;

                return LE_SUCCESS;
            }

            rect.x += glyph.advance;
        }
    }

    return LE_FAILURE;
}

static leResult decodeASCII(uint8_t* val,
                            uint32_t max,
                            uint32_t* codePoint,
                            uint32_t* size)
{
    (void)max;

    *codePoint = val[0];
    *size = 1;

    return LE_SUCCESS;
}

static leResult decodeUTF8(uint8_t* val,
                           uint32_t max,
                           uint32_t* codePoint,
                           uint32_t* size)
{
    uint32_t point = 0;
    uint32_t length = 0;

    if((val[0] & 0x80) == 0x0)
    {
        length = 1;
        point = val[0] & 0x7F;
    }
    else if((val[0] & 0xE0) == 0xC0)
    {
        length = 2;

        if(max < 2)
            return LE_FAILURE;

        point |= (val[0] & 0x1F) << 6;
        point |= (val[1] & 0x3F);
    }
    else if((val[0] & 0xF0) == 0xE0)
    {
        length = 3;

        if(max < 3)
            return LE_FAILURE;

        point |= (val[0] & 0xF) << 12;
        point |= (val[1] & 0x3F) << 6;
        point |= (val[2] & 0x3F);
    }
    else if((val[0] & 0xF8) == 0xF0)
    {
        length = 4;

        if(max < 4)
            return LE_FAILURE;

        point |= (val[0] & 0x7) << 18;
        point |= (val[1] & 0x3F) << 12;
        point |= (val[2] & 0x3F) << 6;
        point |= (val[3] & 0x3F);
    }
    else if((val[0] & 0xFC) == 0xF8)
    {
        length = 5;

        if(max < 5)
            return LE_FAILURE;

        point |= (val[0] & 0x3) << 24;
        point |= (val[1] & 0x3F) << 18;
        point |= (val[2] & 0x3F) << 12;
        point |= (val[3] & 0x3F) << 6;
        point |= (val[4] & 0x3F);
    }
    else if((val[0] & 0xFE) == 0xFC)
    {
        length = 6;

        if(max < 6)
            return LE_FAILURE;

        point |= (val[0] & 0x1) << 30;
        point |= (val[1] & 0x3F) << 24;
        point |= (val[2] & 0x3F) << 18;
        point |= (val[3] & 0x3F) << 12;
        point |= (val[4] & 0x3F) << 6;
        point |= (val[5] & 0x3F);
    }
    else
        return LE_FAILURE;

    *size = length;
    *codePoint = point;

    return LE_SUCCESS;
}

static leResult decodeUTF16(uint8_t* val,
                            uint32_t max,
                            uint32_t* codePoint,
                            uint32_t* size)
{
    uint32_t point = 0;
    uint32_t high, low;

    if((val[1] & 0xFC) == 0xDC)
    {
        if(max < 4)
            return LE_FAILURE;

        *size = 4;

        high = val[0] << 8;
        high |= val[1];
        high -= 0xD800;

        low = val[2] << 8;
        low |= val[3];
        low -= 0xDC00;

        point = ((high << 10) + low) + 0x10000;
    }
    else
    {
        if(max < 2)
            return LE_FAILURE;

        *size = 2;

        point += val[0];
        point += (val[1] << 8);
    }

    *codePoint = point;

    return LE_SUCCESS;
}

typedef leResult (*decodeGlyph_FnPtr)(uint8_t*,
                                      uint32_t,
                                      uint32_t*,
                                      uint32_t*);

decodeGlyph_FnPtr decodeGlyph[] =
    {
        &decodeASCII,
        &decodeUTF8,
        &decodeUTF16
    };

leResult leDecodeCodePoint(uint32_t encoding,
                           uint8_t* data,
                           uint32_t max,
                           uint32_t* codePoint,
                           uint32_t* offset)
{
    return decodeGlyph[encoding](data, max, codePoint, offset);
}


