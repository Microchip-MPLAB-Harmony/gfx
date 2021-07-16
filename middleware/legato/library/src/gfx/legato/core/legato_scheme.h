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
    legato_scheme.h

  Summary:
    A scheme is a collection of colors that can be referenced by one or more
    widgets.  Widgets may use schemes in different ways.  While the color
    names strive to be intuitive they aren't always used in the manner in
    which they describe.
*******************************************************************************/

/** \file legato_scheme.h
 * @brief Schemes support various properties that make it possible to customize the
 * color of widgets.
 *
 * @details A scheme is a collection of colors that can be referenced by
 * one or more widgets.  Widgets may use schemes in different ways.  While
 * the color names strive to be intuitive they aren't always used in the manner
 * in which they describe.
 */

#ifndef LEGATO_SCHEME_H
#define LEGATO_SCHEME_H

#include "gfx/legato/common/legato_common.h"

#include "gfx/legato/common/legato_color.h"

#ifdef __cplusplus
    extern "C" {
#endif
      
// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************
enum leSchemeColorModes
{
    LE_SCHM_COLOR_MODE_GS_8 = LE_COLOR_MODE_GS_8,
    LE_SCHM_COLOR_MODE_RGB_332 = LE_COLOR_MODE_RGB_332,
    LE_SCHM_COLOR_MODE_RGB_565 = LE_COLOR_MODE_RGB_565,
    LE_SCHM_COLOR_MODE_RGBA_5551 = LE_COLOR_MODE_RGBA_5551,
    LE_SCHM_COLOR_MODE_RGB_888 = LE_COLOR_MODE_RGB_888,
    LE_SCHM_COLOR_MODE_RGBA_8888 = LE_COLOR_MODE_RGBA_8888,
    LE_SCHM_COLOR_MODE_ARGB_8888 = LE_COLOR_MODE_ARGB_8888,
    LE_SCHM_COLOR_MODE_GLOBALPALETTE = LE_COLOR_MODE_PALETTE,
    LE_SCHM_COLOR_MODE_MONOCHROME = LE_COLOR_MODE_MONOCHROME,
};

typedef enum leSchemeColor
{
    LE_SCHM_BASE,
    LE_SCHM_HIGHLIGHT,
    LE_SCHM_HIGHLIGHTLIGHT,
    LE_SCHM_SHADOW,
    LE_SCHM_SHADOWDARK,
    LE_SCHM_FOREGROUND,
    LE_SCHM_FOREGROUND_INACTIVE,
    LE_SCHM_FOREGROUND_DISABLED,
    LE_SCHM_BACKGROUND,
    LE_SCHM_BACKGROUND_INACTIVE,
    LE_SCHM_BACKGROUND_DISABLED,
    LE_SCHM_TEXT,
    LE_SCHM_TEXT_HIGHLIGHT,
    LE_SCHM_TEXT_HIGHLIGHTTEXT,
    LE_SCHM_TEXT_INACTIVE,
    LE_SCHM_TEXT_DISABLED
} leSchemeColor;

#define LE_SCHEME_COLOR_COUNT        16
#define LE_SCHEME_COLOR_MODE_COUNT   LE_COLOR_MODE_COUNT

/*
  Union:
    leSchemeColorTable

  Summary:
    This union specifies a list of scheme colors for a given color mode.

  Description:
    This union specifies a list of scheme colors for a given color mode.

  Remarks:
   None.
 */
typedef union leSchemeColorTable
{
    struct
    {
        leColor base;
        leColor highlight;
        leColor highlightLight;
        leColor shadow;
        leColor shadowDark;
        leColor foreground;
        leColor foregroundInactive;
        leColor foregroundDisabled;
        leColor background;
        leColor backgroundInactive;
        leColor backgroundDisabled;
        leColor text;
        leColor textHighlight;
        leColor textHighlightText;
        leColor textInactive;
        leColor textDisabled;
    } colors;
    leColor values[LE_SCHEME_COLOR_COUNT];
} leSchemeColorTable;

/*
  Enumeration:
    leScheme

  Summary:
    This structure specifies the style scheme components of an object.

  Description:
    A scheme is a collection of colors that can be referenced by widgets
    or other objects. While the color names strive to be intuitive they
    aren't always used in the manner in which they describe.

  Remarks:
   None.
 */
typedef struct leScheme
{
    leSchemeColorTable tables[LE_SCHEME_COLOR_MODE_COUNT];
} leScheme;

extern const struct leScheme leDefaultScheme;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/* Function:
     void leScheme_Initialize(leScheme* scheme)
 
   Summary:
     Initialize the scheme to the default values as per the specified color mode. 

   Description:
     Initialize the scheme to the default values as per the specified color mode.

   Parameters:
     leScheme* scheme - the scheme to modify

   Returns:
     void    

*/
/**
 * @brief Initialize scheme.
 * @details Initializes <span class="param">scheme</span>
 * @code
 * const leScheme* scheme;
 * leScheme_Initialize(scheme);
 * @endcode
 * @param scheme is the scheme to initialize.
 * @return void.
 */
void leScheme_Initialize(leScheme* scheme);

/**
 * @brief Gets a scheme color.
 * @details Gets a <span class="param">scheme</span> color
 * @code
 * leScheme* scheme;
 * clr = leScheme_GetColor(schm, LE_SCHM_BASE, CM_RGB_565);
 * @endcode
 * @param scheme is the scheme to query.
 * @param clr is the scheme color to get.
 * @param mode is the color mode to get.
 * @return leColor.
 */
leColor leScheme_GetColor(const leScheme* schm,
                          leSchemeColor clr,
                          leColorMode mode);

/**
 * @brief Gets a scheme render color for the current layer color mode.
 * @details Gets a <span class="param">scheme</span> color
 * @code
 * leScheme* scheme;
 * clr = leScheme_GetRenderColor(schm, LE_SCHM_BASE);
 * @endcode
 * @param scheme is the scheme to query.
 * @param clr is the scheme color to get.
 * @return leColor.
 */
leColor leScheme_GetRenderColor(const leScheme* schm,
                                leSchemeColor clr);

#ifdef __cplusplus
    }
#endif
    
#endif // LEGATO_SCHEME_H 
