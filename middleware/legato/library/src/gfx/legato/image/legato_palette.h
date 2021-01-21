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
    legato_palette.h

  Summary:
    Defines palette assets

  Description:
    Get palette color
*******************************************************************************/

/** \file legato_palette.h
 * @brief Palette functions and definitions.
 *
 * @details Defines palette assets and get palette color.
 */

#ifndef LE_PALETTE_H
#define LE_PALETTE_H

#include "gfx/legato/common/legato_color.h"
#include "gfx/legato/core/legato_stream.h"

// *****************************************************************************
/* Structure:
    lePalette

  Summary:
    Describes a palette asset.  A palette is a lookup table for unique colors.
    Given in an index, a color can be retrieved.
    
    header - standard asset header
    colorCount - the number of colors contained in the palette
    colorMode - the color mode of the image
*/
/**
 * @brief This struct represents a palette asset.
 * @details A palette is a lookup table for unique colors. Given in an index, a color
 * can be retrieved.
 */
typedef struct lePalette
{
    leStreamDescriptor header;  /**< standard asset header */
    uint32_t colorCount;        /**< number of colors in the palatte */
    leColorMode colorMode;      /**< color mode of the image */
} lePalette;

// *****************************************************************************
/* Function:
    leResult lePaletteGetColor(void);

  Summary:
    Gets a color from a palette asset given an index value.
     
  Parameters:
    lePalette* pal - pointer to the palette to read
    
    uint32_t idx - the index of the color to look up

  Returns:
    leColor - the color that was retrieved
*/
/**
 * @brief Get color from a palette.
 * @details Gets a color from <span class="param">pal</span>
 * at <span class="param">idx</span>.
 * @code
 * lePalette * pal;
 * uint32_t idx;
 * leColor color = leColorValue(pal, idx);
 * @endcode
 * @param pal is a color palatte.
 * @param idx is the location of the color
 * @return leColor.
 */
leColor lePalette_GetColor(lePalette* pal, uint32_t idx);


#endif /* LE_PALETTE_H */
