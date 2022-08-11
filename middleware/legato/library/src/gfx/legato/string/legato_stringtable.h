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

/** \file legato_stringtable.h
 * @brief String table functions and definitions.
 *
 * @details .
 */

#ifndef LEGATO_STRINGTABLE_H
#define LEGATO_STRINGTABLE_H

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/font/legato_font.h"

#ifdef __cplusplus
extern "C" {
#endif

// defines meta data sizes for the string table, don't change!
#define LE_STRING_ARRAY_SIZE      4
#define LE_STRING_ENTRY_SIZE      2
#define LE_STRING_MAX_CHAR_WIDTH  6

// *****************************************************************************
/**
 * @brief This enum represents string encoding modes.
 * @details String encoding modes are used to encode text.
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
/**
 * @brief This struct represents string a string table.
 * @details String table is used to describe a string table asset. There is
 * typically only ever one of these defined at any one time.
 */
typedef struct leStringTable
{
    leStreamDescriptor header;
    uint8_t* stringTableData;
    leFont** fontTable;
    leStringEncodingMode encodingMode;
} leStringTable;

// *****************************************************************************
/* Structure:
    struct leStringInfo

  Summary:
    Struct containing the details of a string table entry

  Remarks:
    None.
*/
/**
 * @brief This struct represents string a string table entry.
 * @details
 */
typedef struct leStringInfo
{
    uint32_t stringIndex;
    uint32_t languageID;
    uint32_t offset;
    uint8_t* data;
    uint32_t dataSize;
    uint32_t length;
} leStringInfo;

// *****************************************************************************
/**
 * @brief Gets the number of strings in a string table.
 * @details Gets the number of strings in <span class="param">table</span>.
 * @code
 * leStringTable* table;
 * uint32_t cnt = leStringTable_GetStringCount(table);
 * @endcode
 * @param table is the table to query.
 * @return the number of strings in the table.
 */
uint32_t leStringTable_GetStringCount(const leStringTable* table);

// *****************************************************************************
/**
 * @brief Get number of languages in a string table.
 * @details Gets the number of languages in <span class="param">table</span>.
 * @code
 * leStringTable* table;
 * uint32_t cnt = leStringTable_GetLanguageCount(table);
 * @endcode
 * @param table is the table to query.
 * @return the number of languages in the table.
 */
uint32_t leStringTable_GetLanguageCount(const leStringTable* table);

// *****************************************************************************
/**
 * @brief Get offset of a string in table by string and language ID.
 * @details Gets the offset of a string in <span class="param">table</span>
 * by <span class="param">stringID</span> and <span class="param">languageID</span>.
 * @code
 * leStringTable* table;
 * uint32_t off = leStringTable_GetStringOffset(table, stringID, languageID);
 * @endcode
 * @param table is the table to query.
 * @param stringID the search criteria.
 * @param languageID the search criteria.
 * @return the offset of the string in bytes.
 */
uint32_t leStringTable_GetStringOffset(const leStringTable* table,
                                       uint32_t stringID,
                                       uint32_t languageID);

// *****************************************************************************
/**
 * @brief Get offset of a string in table using active global language.
 * @details Gets the offset of a string in <span class="param">table</span>
 * by <span class="param">stringID</span>.
 * @code
 * leStringTable* table;
 * uint32_t off = leStringTable_GetActiveStringOffset(table, stringID);
 * @endcode
 * @param table is the table to query.
 * @param stringID is the search criteria.
 * @return the offset of the string in bytes.
 */
uint32_t leStringTable_GetActiveStringOffset(const leStringTable* table,
                                             uint32_t stringID);

// *****************************************************************************
/**
 * @brief Get font for a string table string.
 * @details Gets the font in <span class="param">table</span> by
 * <span class="param">stringID</span> and
 * <span class="param">languageID</span>.
 * @code
 * leStringTable* table;
 * uint32_t stringID;
 * uint32_t languageID;
 * leFont* font = leStringTable_GetActiveStringOffset(table, stringID, languageID);
 * @endcode
 * @param table is the table to query.
 * @param stringID the string search criteria.
 * @param languageID the language search criteria.
 * @return the font for the string.
 */
leFont* leStringTable_GetStringFont(const leStringTable* table,
                                    uint32_t stringID,
                                    uint32_t languageID);

// *****************************************************************************
/**
 * @brief Get info for a string table string.
 * @details Gets the offset of a string in <span class="param">table</span> by
 * <span class="param">info</span>.
 * @code
 * leStringTable* table;
 * leResult res = leStringTable_GetActiveStringOffset(table, info);
 * @endcode
 * @param table is the table to query.
 * @param info is the filled leStringInfo structure
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringTable_StringLookup(const leStringTable* table,
                                    leStringInfo* info);

// *****************************************************************************
/**
 * @brief Get length of a string in the string table in codepoints.
 * @details Gets the length of a string in <span class="param">table</span> by
 * <span class="param">info</span>.
 * @code
 * leStringTable* table;
 * leStringInfo* info;
 * leResult res = leStringTable_GetStringLength(table, info);
 * @endcode
 * @param table is the table to query.
 * @param info is the info search criteria.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leStringTable_GetStringLength(const leStringTable* table,
                                       leStringInfo* info);

#ifdef __cplusplus
}
#endif

#endif /* LEGATO_STRINGTABLE_H */
