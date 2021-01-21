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

#include <stdlib.h>
#include <string.h>

#if 0
leChar leGetCharAt(leStringTable* tbl,
                         uint32_t id,
                         uint32_t lang,
                         uint32_t idx)
{
    leFont* fnt = NULL;
    uint16_t stringIdx;
    uint8_t* stringAddress;
    uint32_t stringSize;
    uint32_t i, j;
    uint32_t codePoint;
    uint32_t offset = 0;
    
    if(tbl == NULL)
        return 0;
    
    fnt = leStringFontIndexLookup(tbl, id, lang);
    
    if(fnt == NULL)
        return 0;
    
    stringIdx = leStringIndexLookup(tbl, id, lang);
    
    if(leStringLookup(tbl, stringIdx, &stringAddress, &stringSize))
        return LE_FAILURE;
    
    j = 0;
    
    for(i = 0; i < stringSize;)
    {
        if(leDecodeCodePoint(tbl->encodingMode,
                                stringAddress + i,
                                stringSize,
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

uint32_t leGetStringLength(leStringTable* tbl,
                              uint32_t id,
                              uint32_t lang)
{
    uint16_t idx;
    uint8_t* stringAddress;
    uint32_t stringSize;
    uint32_t stringLength = 0;
    uint32_t codePoint;
    uint32_t offset = 0;
    uint32_t i;
    
    idx = leStringIndexLookup(tbl, id, lang);
    
    if(leStringLookup(tbl, idx, &stringAddress, &stringSize))
        return LE_FAILURE;
        
    for(i = 0; i < stringSize;)
    {
        if(leDecodeCodePoint(tbl->encodingMode,
                                stringAddress + i,
                                stringSize,
                                &codePoint,
                                &offset) == LE_FAILURE)
            return LE_FAILURE;
        
        i += offset;
        
        stringLength += 1;
    }
        
    return stringLength;
}



leResult leGetStringRect(leStringTable* tbl,
                              uint32_t id,
                              uint32_t lang,
                              leRect* rect)
{
    if(tbl == NULL ||
       id >= tbl->stringCount || 
       lang >= tbl->languageCount ||
       rect == NULL)
        return LE_FAILURE;
        
    rect->x = 0;
    rect->y = 0;
    rect->width = leCalculateStringWidth(tbl, id, lang);
    rect->height = leGetStringHeight(tbl, id, lang);
    
    return LE_SUCCESS;
}

uint32_t leGetStringLineRect(leStringTable* tbl,
                                uint32_t id,
                                uint32_t lang,
                                uint32_t start,
                                leRect* rect)
{
    uint16_t idx;
    uint8_t* stringAddress;
    uint32_t stringSize;
    uint32_t codePoint;
	uint32_t i;
	uint32_t width;
    uint32_t totalWidth = 0;
	uint32_t offs = 0;
	leFont* fnt = NULL;
    
    idx = leStringIndexLookup(tbl, id, lang);
    
    if(leStringLookup(tbl, idx, &stringAddress, &stringSize))
    {
        return LE_FAILURE;
    }

	fnt = leStringFontIndexLookup(tbl, id, lang);
	if(fnt == NULL)
    {
        return 0;
    }
    
    for(i = start; i < stringSize;)
    {
        if(leDecodeCodePoint(tbl->encodingMode,
                                stringAddress + i,
                                stringSize,
                                &codePoint,
                                &offs) == LE_FAILURE)
            return LE_FAILURE;

		i += offs;

		//Test for line feed
		if (codePoint == 0x5c) // '\' character (92d)
		{
			if (i < (stringSize - 1))
			{
				if (leDecodeCodePoint(tbl->encodingMode,
                                stringAddress + i,
                                stringSize,
                                &codePoint,
                                &offs) == LE_FAILURE)
				return LE_FAILURE;

				if (codePoint == 0x6E) // 'n' character (110d)
				{
					i += offs;
					break;
				}
			}
		}
		
		leFont_GetGlyphInfo(fnt,
                              codePoint,
                              &offs,
                              &width);
        
		totalWidth += width;
    }

	rect->width = totalWidth;
	rect->height = fnt->height;

    return i;
}

uint32_t leGetCharStringLineRect(leChar* str,
                                    leFont* font,
                                    uint32_t start,
									leRect* rect)
{
    uint32_t len;
    uint32_t codePoint;
    uint32_t width;
    uint32_t glyphoff;
    uint32_t i;
    
    len = 0;
    i = 0;

    rect->height = 0;
    rect->width = 0;

    if(str == NULL || font == NULL || rect == NULL)
        return start;

    while (str[i++] != '\0')
        len++;

    if (start > len)
        return start;
    
    for(i = start;; i++)
    {
        codePoint = str[i];

        //Test for line feed or end of string
        if ((codePoint == '\n') ||
            (codePoint == '\0'))
        {
            i++;
            break;
        }

        //Test for string "\n"        
        if ((codePoint == 0x5c) && 
             (str[i+1] == 0x6E))
        {
            i += 2;
            break;
        }
        
        leFont_GetGlyphInfo(font,
                              codePoint,
                              &glyphoff,
                              &width);

        rect->width += width;
    }

    rect->height = font->height;
    
    return i;
}

static uint32_t calculateWidth(leStringTable* tbl,
                               leFont* fnt,
                               uint32_t id,
                               uint32_t lang)
{
    uint16_t idx;
    uint8_t* stringAddress;
    uint32_t stringSize;
    uint32_t i;
    uint32_t codePoint;
    uint32_t offset = 0;
    uint32_t width;
    uint32_t totalWidth = 0;
    
    if(tbl == NULL || fnt == NULL)
        return 0;
        
    idx = leStringIndexLookup(tbl, id, lang);
    
    if(leStringLookup(tbl, idx, &stringAddress, &stringSize))
        return LE_FAILURE;
    
    for(i = 0; i < stringSize;)
    {
        if(leDecodeCodePoint(tbl->encodingMode,
                                stringAddress + i,
                                stringSize,
                                &codePoint,
                                &offset) == LE_FAILURE)
            return LE_FAILURE;
        
        i += offset;
        
        leFont_GetGlyphInfo(fnt,
                              codePoint,
                              &offset,
                              &width);
            
        totalWidth += width;
    }
    
    return totalWidth;
}

static uint32_t calculatePartialWidth(leStringTable* tbl,
                                      leFont* fnt,
                                      uint32_t id,
                                      uint32_t lang,
                                      uint32_t length)
{
    uint16_t idx;
    uint8_t* stringAddress;
    uint32_t stringSize;
    uint32_t i;
    uint32_t codePoint;
    uint32_t offset = 0;
    uint32_t width;
    uint32_t totalWidth = 0;
    uint32_t maxLength = 0;
    
    if(tbl == NULL || fnt == NULL)
        return 0;
        
    maxLength = leGetStringLength(tbl, id, lang);
        
    if(length > maxLength)
        length = maxLength;
        
    idx = leStringIndexLookup(tbl, id, lang);
    
    if(leStringLookup(tbl, idx, &stringAddress, &stringSize))
        return LE_FAILURE;
    
    idx = 0;
    
    for(i = 0; i < stringSize;)
    {
        if(leDecodeCodePoint(tbl->encodingMode,
                                stringAddress + i,
                                stringSize,
                                &codePoint,
                                &offset) == LE_FAILURE)
            return LE_FAILURE;
        
        i += offset;
        
        leFont_GetGlyphInfo(fnt,
                              codePoint,
                              &offset,
                              &width);
            
        totalWidth += width;
        
        idx++;
        
        if(idx >= length)
            break;
    }
    
    return totalWidth;
}

uint32_t leGetStringHeight(leStringTable* tbl,
                              uint32_t id,
                              uint32_t lang)
{
    leFont* ast;
    
    if(tbl == NULL || id >= tbl->stringCount || lang >= tbl->languageCount)
        return 0;
        
    ast = leStringFontIndexLookup(tbl, id, lang);
    
    if(ast == NULL)
        return 0;
    
    return ast->height;
}

uint32_t leGetStringAscent(leStringTable* tbl,
                              uint32_t id,
                              uint32_t lang)
{
    leFont* ast;
    
    if(tbl == NULL || id >= tbl->stringCount || lang >= tbl->languageCount)
        return 0;
        
    ast = leStringFontIndexLookup(tbl, id, lang);
    
    if(ast == NULL)
        return 0;
    
    return ast->ascent;
}

uint32_t leGetCharWidth(leChar chr, leFont* fnt)
{
    uint32_t offset;
    uint32_t width;
    
    if(fnt == NULL)
        return 0;
    
    leFont_GetGlyphInfo(fnt, chr, &offset, &width);
    
    return width;
}

uint32_t leCalculateStringWidth(leStringTable* tbl, 
                                   uint32_t id,
                                   uint32_t lang)
{
    leFont* fnt = NULL;
    
    if(tbl == NULL)
        return 0;
    
    fnt = leStringFontIndexLookup(tbl, id, lang);
    
    if(fnt == NULL)
        return 0;
        
    return calculateWidth(tbl, fnt, id, lang);
}

uint32_t leCalculatePartialStringWidth(leStringTable* tbl, 
                                          uint32_t id,
                                          uint32_t lang,
                                          uint32_t length)
{
    leFont* fnt = NULL;
    
    if(tbl == NULL)
        return 0;
    
    fnt = leStringFontIndexLookup(tbl, id, lang);
    
    if(fnt == NULL)
        return 0;
        
    return calculatePartialWidth(tbl, fnt, id, lang, length);
}

uint32_t leCalculateCharStringWidth(leChar* str, leFont* fnt)
{
    uint32_t len;
    uint32_t codePoint;
    uint32_t offset;
    uint32_t totalWidth;
    uint32_t width;
    uint32_t i;
    
    len = 0;
    i = 0;
    
    if(str == NULL || fnt == NULL)
        return 0;
    
    while(str[len] != '\0')
        len++;
        
    if(len == 0)
        return 0;
    
    totalWidth = 0;
    
    for(i = 0; i < len; i++)
    {
        codePoint = str[i];
        
        leFont_GetGlyphInfo(fnt,
                              codePoint,
                              &offset,
                              &width);
        
        totalWidth += width;
    }
    
    return totalWidth;
}

uint32_t leCalculatePartialCharStringWidth(leChar* str,
                                              leFont* fnt,
                                              uint32_t size)
{
    uint32_t len;
    uint32_t codePoint;
    uint32_t offset;
    uint32_t totalWidth;
    uint32_t width;
    uint32_t i;
    
    len = 0;
    i = 0;
    
    if(str == NULL || fnt == NULL)
        return 0;
    
    while(str[len] != '\0')
        len++;
        
    if(len == 0)
        return 0;
        
    if(size > len)
        size = len;
    
    totalWidth = 0;
    
    for(i = 0; i < size; i++)
    {
        codePoint = str[i];
        
        leFont_GetGlyphInfo(fnt,
                              codePoint,
                              &offset,
                              &width);
        
        totalWidth += width;
    }
    
    return totalWidth;
}
#endif