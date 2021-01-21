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

<#macro GAC_LOCATION_ID NAME>GFXU_ASSET_LOCATION_ID_${NAME}</#macro>
<#macro GAC_IMAGE_ASSET_HEADER
	NAME
	FORMAT
	MODE
	WIDTH
	HEIGHT
	SIZE>
/*********************************
 * GFX Image Asset
 * Name:   ${NAME}
 * Size:   ${WIDTH}x${HEIGHT} pixels
 * Mode:   ${MODE}
<#if FORMAT?has_content>
 * Format: ${FORMAT}
</#if>
 ***********************************/
extern GFXU_ImageAsset ${NAME};
</#macro>
<#macro GAC_PALETTE_ASSET_HEADER
	NAME
	COUNT
	FORMAT>
/*********************************
 * GFX Palette Asset
 * Name:   ${NAME}
 * Colors: ${COUNT}
 * Format: ${FORMAT}
 ***********************************/
extern GFXU_PaletteAsset ${NAME};
</#macro>
<#macro GAC_FONT_ASSET_HEADER
	INDEX>
/*********************************
 * GFX Font Asset
 * Name:         ${"GAC_FONT_NAME_${INDEX}"?eval}
 * Height:       ${"GAC_FONT_HEIGHT_${INDEX}"?eval}
 * Style:        ${"GAC_FONT_STYLE_${INDEX}"?eval}
 * Glyph Count:  ${"GAC_FONT_GLYPH_COUNT_${INDEX}"?eval}
 * Range Count:  ${"GAC_FONT_RANGE_COUNT_${INDEX}"?eval}
 <#assign x="GAC_FONT_RANGE_COUNT_${INDEX}"?eval>
<#if x gt 0>
 * Glyph Ranges: ${"GAC_FONT_RANGE_STRING_${INDEX}_0"?eval}
<#assign x="GAC_FONT_RANGE_COUNT_${INDEX}"?eval>
<#if x &gt; 1>
<#list 1..x-1 as i>
			     ${"GAC_FONT_RANGE_STRING_${INDEX}_${i}"?eval}
</#list>
</#if>
</#if>
 ***********************************/
extern GFXU_FontAsset ${"GAC_FONT_NAME_${INDEX}"?eval};
</#macro>
<#macro GAC_BINARY_ASSET_HEADER
	NAME
	DESC
	SIZE>
/*********************************
 * GFX Binary Asset
 * Name:         ${NAME}
 * Description:  ${DESC}
 ***********************************/
extern GFXU_BinaryAsset ${NAME};
</#macro>
<#macro GAC_STRINGTABLE_ASSET_HEADER
	INDEX
	NAME
	ENCODING
	LANGUAGES
	LANGUAGECOUNT
	STRINGCOUNT>
/*********************************
 * GFX String Table
 * Name:         ${NAME}
 * Encoding:     ${ENCODING}
 * Languages:    ${LANGUAGES}
 * String Count: ${STRINGCOUNT}
 ***********************************/
// language IDs
<#list 0..LANGUAGECOUNT-1 as i>
<@GAC_STRINGTABLE_ID
	PREFIX = "language"
	NAME = "GAC_STRINGTABLE_LANGUAGE_NAME_${INDEX}_${i}"?eval
	INDEX = i/>
</#list>

// string IDs
<#list 0..STRINGCOUNT-1 as i>
<@GAC_STRINGTABLE_ID
	PREFIX = "string"
	NAME = "GAC_STRINGTABLE_STRING_NAME_${INDEX}_${i}"?eval
	INDEX = i/>
</#list>
 
extern GFXU_StringTableAsset ${NAME};
</#macro>
<#macro GAC_STRINGTABLE_ID
	PREFIX
	NAME
	INDEX>
#define ${PREFIX}_${NAME}    ${INDEX}
</#macro>
/*******************************************************************************
  MPLAB Harmony Graphics Asset Header File

  File Name:
    ${GAC_INTERNAL_FILE_NAME}.h

  Summary:
    Header file containing a list of asset specifications for use with the
	MPLAB Harmony Graphics Stack.

  Description:
    Header file containing a list of asset specifications for use with the
	MPLAB Harmony Graphics Stack.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/


#ifndef ${GAC_INTERNAL_FILE_NAME_UPPERCASE}_H
#define ${GAC_INTERNAL_FILE_NAME_UPPERCASE}_H

#include "gfx/utils/gfx_utils.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END 

/*** Generated Asset Descriptors ***/
<#if GAC_HAS_ASSETS = true>
/*****************************************************************************
 * MPLAB Harmony Graphics Asset Location IDs
 *****************************************************************************/
<#list 0..GAC_NUM_LOCATIONS-1 as i>
#define <@GAC_LOCATION_ID NAME="GAC_LOCATION_ID_${i}"?eval/>    ${i}
</#list>
 
<#if GAC_NUM_IMAGES != 0>
/*****************************************************************************
 * MPLAB Harmony Graphics Image Assets
 *****************************************************************************/
<#assign x=GAC_NUM_IMAGES>
<#list 0..x-1 as i>
    <@GAC_IMAGE_ASSET_HEADER
		NAME = "GAC_IMAGE_NAME_${i}"?eval
		FORMAT = "GAC_IMAGE_FORMAT_${i}"?eval
		MODE = "GAC_IMAGE_MODE_${i}"?eval
		WIDTH = "GAC_IMAGE_WIDTH_${i}"?eval
		HEIGHT = "GAC_IMAGE_HEIGHT_${i}"?eval
		SIZE = "GAC_IMAGE_SIZE_${i}"?eval/>
	
</#list>
</#if>
<#if GAC_NUM_PALETTES != 0>
/*****************************************************************************
 * MPLAB Harmony Graphics Palette Assets
 *****************************************************************************/
<#assign x=GAC_NUM_PALETTES>
<#list 0..x-1 as i>
    <@GAC_PALETTE_ASSET_HEADER
		NAME = "GAC_PALETTE_NAME_${i}"?eval
		COUNT = "GAC_PALETTE_COUNT_${i}"?eval
		FORMAT = "GAC_PALETTE_FORMAT_${i}"?eval/>
	
</#list>
</#if>
<#if GAC_NUM_FONTS != 0>
<#assign x=GAC_NUM_FONTS>
/*****************************************************************************
 * MPLAB Harmony Graphics Font Assets
 *****************************************************************************/
<#assign x=GAC_NUM_FONTS>
<#list 0..x-1 as i>
    <@GAC_FONT_ASSET_HEADER
		INDEX = "${i}"?eval/>
	
</#list>
</#if>
<#if GAC_NUM_BINARIES != 0>
/*****************************************************************************
 * MPLAB Harmony Graphics Binary Assets
 *****************************************************************************/
<#assign x=GAC_NUM_BINARIES>
<#list 0..x-1 as i>
    <@GAC_BINARY_ASSET_HEADER
		NAME = "GAC_BINARY_NAME_${i}"?eval
		DESC = "GAC_BINARY_DESC_${i}"?eval
		SIZE = "GAC_BINARY_SIZE_${i}"?eval/>
	
</#list>
</#if>
</#if>
<#if GAC_STRING_TABLE_EXISTS??>
/*****************************************************************************
 * MPLAB Harmony Graphics String Table
 *****************************************************************************/
/*********************************
 * GFX String Table
 * Name:         ${GAC_STRINGTABLE_NAME}
 * Encoding:     ${GAC_STRINGTABLE_ENCODING}
 * Languages:    ${GAC_STRINGTABLE_LANGUAGES}
 * String Count: ${GAC_STRINGTABLE_COUNT}
 ***********************************/
// language IDs
<#list 0..GAC_STRINGTABLE_LANGUAGECOUNT-1 as i>
<@GAC_STRINGTABLE_ID
	PREFIX = "language"
	NAME = "GAC_STRINGTABLE_LANGUAGE_NAME_${i}"?eval
	INDEX = i/>
</#list>

// string IDs
<#list 0..GAC_STRINGTABLE_COUNT-1 as i>
<@GAC_STRINGTABLE_ID
	PREFIX = "string"
	NAME = "GAC_STRINGTABLE_STRING_NAME_${i}"?eval
	INDEX = i/>
</#list>
 
extern GFXU_StringTableAsset ${GAC_STRINGTABLE_NAME};

</#if>
<#if GAC_GLOBAL_PALETTE_EXISTS??>
/*****************************************************************************
 * MPLAB Harmony Graphics Global Palette
 *****************************************************************************/

<#if GAC_GLOBAL_PALETTE_SIZE = "1">
extern uint8_t globalColorPalette[256];
<#elseif GAC_GLOBAL_PALETTE_SIZE = "2">
extern uint16_t globalColorPalette[256];
<#elseif GAC_GLOBAL_PALETTE_SIZE = "4">
extern uint32_t globalColorPalette[256];
</#if>
</#if>
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* ${GAC_INTERNAL_FILE_NAME_UPPERCASE}_H */

