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
/*****************************************************************************
* MPLAB Harmony Graphics Image Assets
*****************************************************************************/
/*********************************
 * Legato Image Asset
 * Name:   mchpLogo
 * Size:   32x32 pixels
 * Format: RLE
 * Mode: RGB_565
 ***********************************/
extern leImage mchpLogo;

/*********************************
 * Legato Image Asset
 * Name:   sdcard_sm
 * Size:   32x32 pixels
 * Format: RLE
 * Mode: RGB_565
 ***********************************/
extern leImage sdcard_sm;

/*********************************
 * Legato Image Asset
 * Name:   usb_icon_sm
 * Size:   32x32 pixels
 * Format: RLE
 * Mode: RGB_565
 ***********************************/
extern leImage usb_icon_sm;

/*********************************
 * Legato Image Asset
 * Name:   MHGS_logo_small
 * Size:   141x132 pixels
 * Format: RLE
 * Mode: RGB_565
 ***********************************/
extern leImage MHGS_logo_small;

/*****************************************************************************
* MPLAB Harmony Graphics Font Assets
*****************************************************************************/
/*********************************
 * Legato Font Asset
 * Name:         Arial
 * Height:       21
 * Baseline:     15
 * Style:        Antialias
 * Glyph Count:  96
 * Range Count:  1
 * Glyph Ranges: 0x20-0x7F
***********************************/
extern leRasterFont Arial;

/*********************************
 * Legato Font Asset
 * Name:         Arial_sm
 * Height:       17
 * Baseline:     12
 * Style:        Antialias
 * Glyph Count:  48
 * Range Count:  14
 * Glyph Ranges: 0x20-0x21
                 0x27
                 0x2E
                 0x30-0x39
                 0x41-0x44
                 0x46
                 0x49
                 0x4E
                 0x50-0x51
                 0x53-0x55
                 0x61
                 0x63-0x69
                 0x6B-0x70
                 0x72-0x79
***********************************/
extern leRasterFont Arial_sm;

/*****************************************************************************
 * Legato String Table
 * Encoding        ASCII
 * Language Count: 1
 * String Count:   19
*****************************************************************************/

// language IDs
#define language_default    0

// string IDs
#define string_Done    0
#define string_FileNotFound1    1
#define string_FileNotFound2    2
#define string_Flashing    3
#define string_FlashingComplete    4
#define string_InvalidFile    5
#define string_NoMedium    6
#define string_NoValidMedium    7
#define string_NoValidMedium2    8
#define string_Numbers    9
#define string_Of    10
#define string_Ok    11
#define string_RecordCount    12
#define string_SDCard    13
#define string_SelectSource    14
#define string_Title    15
#define string_USB    16
#define string_UnknownError    17
#define string_stringNumberFiller    18

extern const leStringTable stringTable;
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* LE_GEN_ASSETS_H */
