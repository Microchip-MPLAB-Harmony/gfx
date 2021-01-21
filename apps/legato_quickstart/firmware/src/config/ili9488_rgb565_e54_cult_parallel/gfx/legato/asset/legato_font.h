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
    legato_font.h

  Summary:
    Describes font assets

  Description:
    Type definitions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LE_FONT_H
#define LE_FONT_H
//DOM-IGNORE-END

#include "gfx/legato/asset/legato_asset.h"

#include "gfx/legato/common/legato_color.h"

// *****************************************************************************
/* Enumeration:
    leFontBPP

  Summary:
    Indicates the bits per pixel mode of a font
*/
typedef enum leFontBPP
{
    LE_FONT_BPP_1, // standard
    LE_FONT_BPP_8  // antialiased
} leFontBPP;

typedef struct leFontGlyph
{
    leChar codePoint;
    int16_t width;
    int16_t height;
    int16_t advance;
    int16_t bearingX;
    int16_t bearingY;
    uint16_t flags;
    uint16_t dataRowWidth;
    uint32_t dataOffset;
} leFontGlyph;

// *****************************************************************************
/* Structure:
    leFontAsset

  Summary:
    Describes a font asset.  A font asset is a series of raster images that
    represet linguistic characters.  These characters are referenced by an index
    called a 'code point'.  This code point is 1-4 bytes in length.  Code points
    may be encoded to save space.  Fonts also contain kerning data that describes
    character positioning data.
    
    header - standard asset header
    height - font height in pixels
    ascent - font ascent in pixels
    descent - font descent in pixels
    baseline - font baseline in pixels
    bpp - font pixel size, either 1 or 8.  8 is for anti-aliased font data
    indexTable - pointer to the corresponding glyph index table.  this table
                 is used to reference code points to glyph data.
*/
typedef struct leFont
{
    leAssetHeader header;
    uint16_t height;
    uint16_t baseline;
    leFontBPP bpp;
    const uint8_t* glyphTable;
    const uint8_t* fontData;
} leFont;

#if LE_INCLUDE_DEFAULT_1BPP_FONT == 1
LIB_EXPORT extern leFont LiberationMono1;
#endif

#if LE_INCLUDE_DEFAULT_8BPP_FONT == 1
LIB_EXPORT extern leFont LiberationMono8;
#endif

// internal use only
leResult leFont_GetGlyphInfo(const leFont* fnt,
                             uint32_t codepoint,
                             leFontGlyph* glyph);

leResult leFont_GetGlyphRect(const leFontGlyph* glyph,
                             leRect* rect);

void leFont_DrawGlyph(const leFont* fnt,
                      const leFontGlyph* glyph,
                      int32_t x,
                      int32_t y,
                      leColor clr,
                      uint32_t a);

void leFont_DrawGlyphRow(leFontBPP bpp,
                         const uint8_t* data,
                         int32_t x,
                         int32_t y,
                         int32_t colStart,
                         int32_t colEnd,
                         leColor clr,
                         uint32_t a);

void leFont_DrawCString(const leFont* fnt,
                        const char* str,
                        int32_t x,
                        int32_t y,
                        leColor clr,
                        uint32_t a);

void leFont_DrawUString(const leFont* fnt,
                        const leChar* str,
                        uint32_t size,
                        int32_t x,
                        int32_t y,
                        leColor clr,
                        uint32_t a);

#endif /* LE_FONT_H */