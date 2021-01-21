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
 Module for Microchip Legato Graphics Library

  Company:
    Microchip Technology Inc.

  File Name:
    generated/le_gen_assets.h

  Summary:
    Header file containing a list of asset specifications for use with the
    MPLAB Harmony Graphics Stack.

  Description:
    Header file containing a list of asset specifications for use with the
    MPLAB Harmony Graphics Stack.
*******************************************************************************/


#ifndef LE_GEN_ASSETS_H
#define LE_GEN_ASSETS_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

#include "gfx/legato/legato.h"

/*****************************************************************************
* MPLAB Harmony Graphics Asset Location IDs
*****************************************************************************/
/*********************************
 * Legato Asset Memory Location
 * Name:   SQI
 * Size:   8000000
 * Format: HEX
 * Path:   SQI
 * Offset: 0
 ***********************************/
#define LA_ASSET_LOCATION_ID_SQI    2

/*****************************************************************************
* MPLAB Harmony Graphics Image Assets
*****************************************************************************/
/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_jpeg
 * Size:   141x132 pixels
 * Format: Raw
 * Mode: RGB_565
 ***********************************/
extern leImage MHGS_logo_small_jpeg;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_png
 * Size:   141x132 pixels
 * Format: Raw
 * Mode: RGB_565
 ***********************************/
extern leImage MHGS_logo_small_png;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_raw
 * Size:   141x132 pixels
 * Format: Raw
 * Mode: RGB_565
 ***********************************/
extern leImage MHGS_logo_small_raw;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_rle
 * Size:   141x132 pixels
 * Format: RLE
 * Mode: RGB_565
 ***********************************/
extern leImage MHGS_logo_small_rle;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_compressed
 * Size:   141x132 pixels
 * Format: Raw
 * Mode: INDEX_8
 ***********************************/
extern leImage MHGS_logo_small_compressed;

/*****************************************************************************
* MPLAB Harmony Graphics Font Assets
*****************************************************************************/
/*********************************
 * Legato Font Asset
 * Name:         ArialUnicodeMS
 * Height:       16
 * Baseline:     12
 * Style:        Antialias
 * Glyph Count:  97
 * Range Count:  2
 * Glyph Ranges: 0x0A
                 0x20-0x7F
***********************************/
extern leRasterFont ArialUnicodeMS;

/*********************************
 * Legato Font Asset
 * Name:         ArialUnicodeMS_Large
 * Height:       24
 * Baseline:     19
 * Style:        Antialias
 * Glyph Count:  8109
 * Range Count:  22
 * Glyph Ranges: 0x20-0x7F
                 0x4E26
                 0x50CF
                 0x5132
                 0x53EF
                 0x5716
                 0x58D3
                 0x5916
                 0x5B57-0x5B58
                 0x5F62
                 0x6309
                 0x6539
                 0x66F4
                 0x672A
                 0x6AA2
                 0x6B64
                 0x7247
                 0x7684
                 0x7D22
                 0x7E2E
                 0x7E6A
                 0x80C8-0x9FFF
***********************************/
extern leRasterFont ArialUnicodeMS_Large;

/*****************************************************************************
 * Legato String Table
 * Encoding        UTF16
 * Language Count: 2
 * String Count:   11
*****************************************************************************/

// language IDs
#define language_English    0
#define language_Chinese    1

// string IDs
#define string_DrawJpeg    0
#define string_DrawPNG    1
#define string_DrawPaletteCompressed    2
#define string_DrawRLE    3
#define string_DrawRaw    4
#define string_ImageIsJPEG    5
#define string_ImageIsPNG    6
#define string_ImageIsPaletteCompressed    7
#define string_ImageIsRLE    8
#define string_ImageIsRaw    9
#define string_TitleString    10

extern const leStringTable stringTable;
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* LE_GEN_ASSETS_H */
