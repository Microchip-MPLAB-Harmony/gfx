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

#include "gfx/legato/string/legato_string_renderer.h"

#include "gfx/legato/string/legato_stringutils.h"

#define ASCII_SPACE 0x20

#if LE_STREAMING_ENABLED == 1
leResult _leStringStreamRenderer_Draw(leStringRenderRequest* req);
leResult _leUStringStreamRenderer_Draw(leUStringRenderRequest* req);
leResult _leCStringStreamRenderer_Draw(leCStringRenderRequest* req);
#endif

static leResult drawString(leStringRenderRequest* req)
{
    uint32_t charItr, len, lines, lineItr;
    leFontGlyph glyphInfo;
    leChar codePoint;
    int32_t stringY, lineX;
    leRect stringRect, lineRect;
    uint32_t startIdx, endIdx;
    leRasterFont* font;
    //leRect drawRect;

    len = req->str->fn->length(req->str);
    font = (leRasterFont*)req->str->fn->getFont(req->str);

    if(len == 0 || font == NULL || font->glyphTable == NULL)
        return LE_FAILURE;

    stringY = req->y;

    lines = req->str->fn->getLineCount(req->str);

    req->str->fn->getRect(req->str, &stringRect);

    //drawRect = stringRect;
    //drawRect.x += x;
    //drawRect.y += y;

    //leRenderer_RectLine(&drawRect, clr);

    for(lineItr = 0; lineItr < lines; lineItr++)
    {
        req->str->fn->getLineIndices(req->str, lineItr, &startIdx, &endIdx);
        req->str->fn->getLineRect(req->str, lineItr, &lineRect);

        if(req->align == LE_HALIGN_CENTER)
        {
            lineX = stringRect.x + (stringRect.width / 2) - (lineRect.width / 2);
        }
        else if(req->align == LE_HALIGN_RIGHT)
        {
            lineX = stringRect.x + stringRect.width - lineRect.width;
        }
        else
        {
            lineX = stringRect.x;
        }

        for(charItr = startIdx; charItr < endIdx; charItr++)
        {
            codePoint = req->str->fn->charAt(req->str, charItr);

            leFont_GetGlyphInfo((leFont*)font, codePoint, &glyphInfo);

            if(codePoint == ASCII_SPACE)
            {

            }
            else
            {
                leFont_DrawGlyph((leFont*)font,
                                 &glyphInfo,
                                 req->x + lineX + glyphInfo.bearingX,
                                 stringY + (font->baseline - glyphInfo.bearingY),
                                 req->color,
                                 req->alpha);
            }

            lineX += glyphInfo.advance;
        }

        stringY += font->height;
    }

    return LE_SUCCESS;
}

leResult leStringRenderer_DrawString(leStringRenderRequest* req)
{
#if LE_STREAMING_ENABLED == 1
    leFont* font;
#endif

    if(req == NULL || req->str == NULL)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    font = req->str->fn->getFont(req->str);

    if(font == NULL)
        return LE_FAILURE;

    if(font->header.location != LE_STREAM_LOCATION_ID_INTERNAL)
    {
        return _leStringStreamRenderer_Draw(req);
    }
    else
    {
#endif
        return drawString(req);
#if LE_STREAMING_ENABLED == 1
    }
#endif
}

static leResult drawUString(leUStringRenderRequest* req)
{
    uint32_t charItr, lines, lineItr;
    leFontGlyph glyphInfo;
    leChar codePoint = 0;
    int32_t stringY, lineX;
    leRect stringRect, lineRect;
    uint32_t startIdx = 0;
    uint32_t endIdx = 0;
    leRasterFont* rasFnt;

    rasFnt = (leRasterFont*)req->font;

    stringY = req->y;

    lines = leStringUtils_GetLineCount(req->str, req->length);

    leStringUtils_GetRect(req->str, req->length, req->font, &stringRect);

    for(lineItr = 0; lineItr < lines; lineItr++)
    {
        leStringUtils_GetLineIndices(req->str, req->length, lineItr, &startIdx, &endIdx);
        leStringUtils_GetLineRect(req->str, req->length, req->font, lineItr, &lineRect);

        if(req->align == LE_HALIGN_CENTER)
        {
            lineX = stringRect.x + (stringRect.width / 2) - (lineRect.width / 2);
        }
        else if(req->align == LE_HALIGN_RIGHT)
        {
            lineX = stringRect.x + stringRect.width - lineRect.width;
        }
        else
        {
            lineX = stringRect.x;
        }

        for(charItr = startIdx; charItr < endIdx; charItr++)
        {
            leFont_GetGlyphInfo(req->font, req->str[charItr], &glyphInfo);

            if(codePoint == ASCII_SPACE)
            {

            }
            else
            {
                leFont_DrawGlyph(req->font,
                                 &glyphInfo,
                                 req->x + lineX + glyphInfo.bearingX,
                                 stringY + (rasFnt->baseline - glyphInfo.bearingY),
                                 req->color,
                                 req->alpha);
            }

            lineX += glyphInfo.advance;
        }

        stringY += rasFnt->height;
    }

    return LE_SUCCESS;
}

leResult leStringRenderer_DrawUString(leUStringRenderRequest* req)
{
    if(req == NULL ||
       req->str == NULL ||
       req->font == NULL ||
       req->length == 0 ||
        ((leRasterFont*)req->font)->glyphTable == NULL)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    if(req->font->header.location != LE_STREAM_LOCATION_ID_INTERNAL)
    {
        return LE_FAILURE;//_leUStringStreamRenderer_Draw(req);
    }
    else
    {
#endif
        return drawUString(req);
#if LE_STREAMING_ENABLED == 1
    }
#endif
}

static leResult drawCString(leCStringRenderRequest* req)
{
    uint32_t charItr, lines, lineItr;
    uint32_t size;
    leFontGlyph glyphInfo;
    leChar codePoint = 0;
    int32_t stringY, lineX;
    leRect stringRect, lineRect;
    uint32_t startIdx = 0;
    uint32_t endIdx = 0;
    leRasterFont* rasFnt = (leRasterFont*)req->font;

    size = strlen(req->str);

    if(size == 0)
        return LE_FAILURE;

    stringY = req->y;

    lines = leStringUtils_GetLineCountCStr(req->str);

    leStringUtils_GetRectCStr(req->str, req->font, &stringRect);

    for(lineItr = 0; lineItr < lines; lineItr++)
    {
        leStringUtils_GetLineIndicesCStr(req->str, lineItr, &startIdx, &endIdx);
        leStringUtils_GetLineRectCStr(req->str, req->font, lineItr, &lineRect);

        if(req->align == LE_HALIGN_CENTER)
        {
            lineX = stringRect.x + (stringRect.width / 2) - (lineRect.width / 2);
        }
        else if(req->align == LE_HALIGN_RIGHT)
        {
            lineX = stringRect.x + stringRect.width - lineRect.width;
        }
        else
        {
            lineX = stringRect.x;
        }

        for(charItr = startIdx; charItr < endIdx; charItr++)
        {
            leFont_GetGlyphInfo(req->font, req->str[charItr], &glyphInfo);

            if(codePoint == ASCII_SPACE)
            {

            }
            else
            {
                leFont_DrawGlyph(req->font,
                                 &glyphInfo,
                                 req->x + lineX + glyphInfo.bearingX,
                                 stringY + (rasFnt->baseline - glyphInfo.bearingY),
                                 req->color,
                                 req->alpha);
            }

            lineX += glyphInfo.advance;
        }

        stringY += rasFnt->height;
    }

    return LE_SUCCESS;
}

leResult leStringRenderer_DrawCString(leCStringRenderRequest* req)
{
    if(req == NULL ||
        req->str == NULL ||
        req->font == NULL ||
        ((leRasterFont*)req->font)->glyphTable == NULL)
        return LE_FAILURE;

#if LE_STREAMING_ENABLED == 1
    if(req->font->header.location != LE_STREAM_LOCATION_ID_INTERNAL)
    {
        return LE_FAILURE;//_leCStringStreamRenderer_Draw(req);
    }
    else
    {
#endif
        return drawCString(req);
#if LE_STREAMING_ENABLED == 1
    }
#endif
}