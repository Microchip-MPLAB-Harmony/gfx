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

#include "gfx/legato/asset/legato_stringtable.h"

leFont* leStringTable_FontIndexLookup(const leStringTable* table,
                                      uint32_t stringID,
                                      uint32_t languageID)
{
    uint8_t* indexTable;
    uint8_t indexWidth;
    uint32_t fontID;
    
    if(table == NULL || 
       stringID >= table->stringCount || 
       languageID >= table->languageCount)
    {
        return NULL;
    }
    
    indexTable = table->fontIndexTable;
    
    // skip header
    indexTable += 2;
    
    indexWidth = indexTable[0];
       
    indexTable += 1;
    
    // skip to string index
    fontID = indexTable[indexWidth * (languageID + stringID)];

    if(fontID == -1)
        return NULL;
    
    return table->fontTable[fontID];
}

static uint16_t stringIndexLookup(const leStringTable* table,
                                  uint32_t stringID,
                                  uint32_t languageID)
{
    uint8_t* indexTable = table->stringIndexTable;
    uint16_t* ptr;
    //uint32_t languageCount;
    uint32_t indexWidth;
    uint32_t i;
    
    if(stringID >= table->stringCount || languageID >= table->languageCount)
        return 0;
        
    // skip header
    indexTable += 3;
    
    //languageCount = indexTable[0];
    //indexTable += 1;
    
    indexWidth = indexTable[0];
    indexTable += 1;
    
    // skip to string index
    indexTable += (table->languageCount * indexWidth) * stringID;
    
    if(indexWidth == 1)
    {
        i = indexTable[languageID];
        
        if(i == 0xFF)
        {
            return 0xFFFF;
        }
        else
        {
            return i;
        }
    }
    else
    {
        ptr = (uint16_t*)indexTable;
        
        return ptr[languageID];
    }
}

leResult leStringTable_StringLookup(const leStringTable* table,
                                    leStringInfo* info)
{
    //uint32_t i;
    //uint32_t stringCount, offset;
    uint8_t* data;
    
    if(table == NULL || info == NULL)
        return LE_FAILURE;
        
    info->tableIndex = stringIndexLookup(table,
                                         info->stringIndex,
                                         info->languageID);
        
    if(info->tableIndex == 0xFFFF)
        return LE_FAILURE;
        
    data = table->header.dataAddress;

    // get string count
    //stringCount = *((uint16_t*)data);

    //data += 2;
    
    // find the string data in the string table
//    for(i = 0; i < stringCount; i++)
//    {
//        if(i == info->tableIndex)
//            break;
//        
//        offset = data[0] + (data[1] << 8);
//        
//        data += LE_STRING_ENTRY_SIZE + offset;
//        
//        /*word = (uint16_t*)address;
//        
//        offset = *word;
//        offset += leSTRING_ENTRY_SIZE;
//    
//        address += offset;*/
//    }
        
    data += /*2 +*/ info->tableIndex;
    
    //word = (uint16_t*)address;
    info->size = *((uint16_t*)data);
    
    data += LE_STRING_ENTRY_SIZE;
    //address += leSTRING_ENTRY_SIZE;
    
    info->address = data;
    
    return LE_SUCCESS;
}