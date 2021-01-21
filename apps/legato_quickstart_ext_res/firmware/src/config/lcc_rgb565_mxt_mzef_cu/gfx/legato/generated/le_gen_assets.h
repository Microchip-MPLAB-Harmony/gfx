/*******************************************************************************
 Module for Microchip Legato Graphics Library

  Company:
    Microchip Technology Inc.

  File Name:
    le_gen_assets.h

  Summary:
    Header file containing a list of asset specifications for use with the
    Legato Graphics Stack.


  Description:
    Header file containing a list of asset specifications for use with the
    Legato Graphics Stack.

*******************************************************************************/


// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C)  Microchip Technology Inc. and its subsidiaries.
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

#ifndef LE_GEN_ASSETS_H
#define LE_GEN_ASSETS_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

#include "gfx/legato/legato.h"

extern const lePalette leGlobalPalette;

/*****************************************************************************
 * Legato Graphics Asset Location IDs
 *****************************************************************************/
/*********************************
 * Legato Asset Memory Location
 * Name:          SQI
 ***********************************/
#define LE_STREAM_LOCATION_ID_SQI    1

/*****************************************************************************
 * Legato Graphics Image Assets
 *****************************************************************************/
/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_compressed_palette
 * Size:   256x1 pixels
 * Type:   RGB Color Palette
 * Format: RGB_565
 ***********************************/
extern leImage MHGS_logo_small_compressed_palette;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_jpeg
 * Size:   141x132 pixels
 * Type:   RGB Data
 * Format: RGB_888
 ***********************************/
extern leImage MHGS_logo_small_jpeg;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_raw
 * Size:   141x132 pixels
 * Type:   RGB Data
 * Format: RGB_565
 ***********************************/
extern leImage MHGS_logo_small_raw;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_rle
 * Size:   141x132 pixels
 * Type:   RGB Data
 * Format: RGB_565
 ***********************************/
extern leImage MHGS_logo_small_rle;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_compressed
 * Size:   141x132 pixels
 * Type:   Color Lookup Map
 * Format: INDEX_8
 ***********************************/
extern leImage MHGS_logo_small_compressed;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small_png
 * Size:   48x45 pixels
 * Type:   RGB Data
 * Format: RGB_888
 ***********************************/
extern leImage MHGS_logo_small_png;

/*****************************************************************************
 * Legato Graphics Font Assets
 *****************************************************************************/
/*********************************
 * Legato Font Asset
 * Name:         NotoSansCJKtc_Regular
 * Height:       23
 * Baseline:     19
 * Style:        Antialias
 * Glyph Count:  4105
 * Range Count:  41
 * Glyph Ranges: 0x20-0x7E
                 0x4E00-0x5D98
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
                 0x8A00
                 0x8A9E
                 0x90E8
                 0x91CD
                 0x9215
***********************************/
extern leRasterFont NotoSansCJKtc_Regular;

/*********************************
 * Legato Font Asset
 * Name:         NotoSans_Regular
 * Height:       21
 * Baseline:     12
 * Style:        Antialias
 * Glyph Count:  96
 * Range Count:  16
 * Glyph Ranges: 0xA
                 0x20-0x7E
***********************************/
extern leRasterFont NotoSans_Regular;

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
#define stringID_ImageIsRLE    0
#define stringID_DrawRaw    1
#define stringID_DrawPNG    2
#define stringID_DrawRLE    3
#define stringID_TitleString    4
#define stringID_ImageIsJPEG    5
#define stringID_ImageIsRaw    6
#define stringID_DrawPaletteCompressed    7
#define stringID_ImageIsPNG    8
#define stringID_ImageIsPaletteCompressed    9
#define stringID_DrawJpeg    10

extern const leStringTable stringTable;


// string list
extern leTableString string_ImageIsRLE;
extern leTableString string_DrawRaw;
extern leTableString string_DrawPNG;
extern leTableString string_DrawRLE;
extern leTableString string_TitleString;
extern leTableString string_ImageIsJPEG;
extern leTableString string_ImageIsRaw;
extern leTableString string_DrawPaletteCompressed;
extern leTableString string_ImageIsPNG;
extern leTableString string_ImageIsPaletteCompressed;
extern leTableString string_DrawJpeg;

void initializeStrings(void);
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* LE_GEN_ASSETS_H */
