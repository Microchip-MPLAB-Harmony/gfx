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

#ifndef LEGATO_TABLESTRING_H
#define LEGATO_TABLESTRING_H

#include "gfx/legato/string/legato_string.h"

// DOM-IGNORE-BEGIN
#define LE_STRING_NULLIDX -1

struct leTableString;

#define LE_TABLESTRING_VTABLE(THIS_TYPE) \
    LE_STRING_VTABLE(THIS_TYPE) \
    \
    uint32_t (*getID)(const THIS_TYPE* this); \
	leResult (*setID)(THIS_TYPE* this, uint32_t id); \
	uint32_t (*sizeInBytes)(const THIS_TYPE* this); \

typedef struct leTableStringVTable
{
	LE_TABLESTRING_VTABLE(struct leTableString)
} leTableStringVTable;
// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leTableString

  Summary:
    String type that references an entry in a string table.  Read only.

  Remarks:
    None.
*/
typedef struct leTableString
{
    leString base;

    const leTableStringVTable* fn;
    
    int32_t index; // an index into a string table or LE_STRING_NULLIDX
} leTableString;

// *****************************************************************************
/* Function:
    leTableString* leTableString_New(uint32_t idx)

   Summary:
    Allocates memory and constructs a new table string

   Parameters:
    idx - the table index to reference

  Returns:
    leTableString* - pointer to the newly allocated string

  Remarks:
    Caller is responsible for freeing the memory allocated by this function
    using leString_Delete()
*/
LIB_EXPORT leTableString* leTableString_New(uint32_t idx);

// *****************************************************************************
/* Function:
    void leTableString_PNew(leTableString* str, uint32_t idx)

   Summary:
    Constructs a new table string at the given pointer

   Parameters:
    idx - the table index to reference

  Returns:
    
  Remarks:
    It is assumed that the pointer provided is being managed by the caller.  Use
    leString_PDelete() to properly destruct this pointer.
*/
LIB_EXPORT void leTableString_Constructor(leTableString* str,
                                          uint32_t idx);


// *****************************************************************************
/* Virtual Member Function:
    uint32_t getID(const leFixedString* this)

  Summary:
     Gets the current string table ID of this string

  Description:
     Gets the current string table ID of this string

  Parameters:
    const leFixedString* this - The string to operate on

  Remarks:
    Usage - _this->fn->getID(_this);

  Returns:
    uint32_t - the string table ID
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setID(leFixedString* this,
                   uint32_t id)

  Summary:
     Sets the string table ID of this string

  Description:
     Sets the string table ID of this string

  Parameters:
    leFixedString* this - The string to operate on
    uint32_t id - the string table ID to use

  Remarks:
    Usage - _this->fn->setID(_this, id);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t sizeInBytes(const leFixedString* this)

  Summary:
     Gets the size of this string in bytes

  Description:
     Gets the size of this string in bytes

  Parameters:
    const leFixedString* this - The string to operate on

  Remarks:
    Usage - _this->fn->sizeInBytes(_this);

  Returns:
    uint32_t - the string size in bytes
*/



#endif /* LEGATO_TABLESTRING_H */