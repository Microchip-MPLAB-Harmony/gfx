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

#ifndef LEGATO_STRINGTABLE_H
#define LEGATO_STRINGTABLE_H

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/asset/legato_font.h"

// defines meta data sizes for the string table, don't change!
#define LE_STRING_ARRAY_SIZE      4
#define LE_STRING_ENTRY_SIZE      2
#define LE_STRING_MAX_CHAR_WIDTH  6

// *****************************************************************************
/* Enumeration:
    leStringEncodingMode

  Summary:
    Indicates the string encoding mode type.  Any characters above 255 must use
    UTF8 or UTF16
*/
typedef enum leStringEncodingMode
{
    LE_STRING_ENCODING_ASCII,
    LE_STRING_ENCODING_UTF8,
    LE_STRING_ENCODING_UTF16
} leStringEncodingMode;

// *****************************************************************************
/* Structure:
    leStringTable

  Summary:
    Describes a string table asset.  There is typically only ever one of these
    defined at any one time.
    
    header - standard asset header
    languageCount - the number of languages in the string table
    stringCount - the number of strings in the string table
    stringIndexTable - the pointer to the string index table.  the string index
                       table is a table that contains all of the unique strings
                       defined in the string table.
    fontTable - the font table contains an array of pointers to all defined
                font assets that the string table references
    fontIndexTable - the font index table is a table that maps strings to font
                     indices which can then be used to get an actual font pointer
                     from the font table
    encodingMode - indicates how strings are encoded in the stringIndexTable
*/
typedef struct leStringTable
{
    leAssetHeader header;
    uint32_t languageCount;
    uint32_t stringCount;
    uint8_t* stringIndexTable;
    leFont** fontTable;
    uint8_t* fontIndexTable;
    leStringEncodingMode encodingMode;
} leStringTable;

typedef struct leStringInfo
{
    uint32_t stringIndex;
    uint32_t languageID;
    uint16_t tableIndex;
    uint8_t* address;
    uint32_t size;
} leStringInfo;

LIB_EXPORT leFont* leStringTable_FontIndexLookup(const leStringTable* table,
                                                 uint32_t stringID,
                                                 uint32_t languageID);
                                      
LIB_EXPORT leResult leStringTable_StringLookup(const leStringTable* table,
                                               leStringInfo* info);                                                                             



#endif /* LEGATO_STRINGTABLE_H */