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

#ifndef LEGATO_FIXEDSTRING_H
#define LEGATO_FIXEDSTRING_H

#include "gfx/legato/string/legato_string.h"

// DOM-IGNORE-BEGIN
struct leFixedString;

#define LE_FIXEDSTRING_VTABLE(THIS_TYPE) \
    LE_STRING_VTABLE(THIS_TYPE) \
    \
	leResult (*setBuffer)(THIS_TYPE* str, leChar* buf, uint32_t size); \

typedef struct leFixedStringVTable
{
	LE_FIXEDSTRING_VTABLE(struct leFixedString)
} leFixedStringVTable;

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leFixedString

  Summary:
    String type that dynamically allocates internal memory to accommodate dynamic
    string operations.

  Remarks:
    None.
*/
typedef struct leFixedString
{
    leString base; // string base data

    const leFixedStringVTable* fn; // function table
    
    leChar*  data;     // local string data storage
    uint16_t capacity; // actual memory capacity of the string
    
    uint16_t length;   // actual length of the string
    
    const leFont*  font; // the font used for this string    
} leFixedString;

// *****************************************************************************
/* Function:
    leFixedString* leFixedString_New(leChar* buffer,
                                     uint32_t size)

   Summary:
    Allocates a memory for a new fixed string and automatically calls its
    constructor function

   Parameters:
    leChar* buf - the buffer to assign to this string
    uint32_t size - the size of the buffer

  Returns:
    leFixedString* - pointer to the newly allocated string

  Remarks:
    Caller is responsible for freeing the memory allocated by this function
    using leString_Delete()
*/
LIB_EXPORT leFixedString* leFixedString_New(leChar* buf,
                                            uint32_t size);

// *****************************************************************************
/* Function:
    void leFixedString_Constructor(leFixedString* str, leChar* buffer, uint32_t size)

   Summary:
    Constructs a new fixed string at the given pointer

   Parameters:
    leFixedString* str - the string to construct
    leChar* buf - the buffer to assign to this string
    uint32_t size - the size of the buffer

  Returns:
    
  Remarks:
    It is assumed that the pointer provided is being managed by the caller.  Use
    leString_PDelete() to properly destruct this pointer.
*/
LIB_EXPORT void leFixedString_Constructor(leFixedString* str,
                                          leChar* buf,
                                          uint32_t size);

// *****************************************************************************
/* Virtual Member Function:
    leResult setBuffer(leFixedString* str,
                       leChar* buf,
                       uint32_t size)

  Summary:
    Sets a fixed buffer to this string

  Description:
    Sets a fixed buffer to this string

  Parameters:
    leFixedString* str - The string to operate on
    leChar* buf - the buffer to set
    uint32_t size - the size of the buffer

  Remarks:
    Usage - _this->fn->setBuffer(_this, buf, size);

  Returns:
    leResult - the result of the operation
*/



#endif /* LEGATO_FIXEDSTRING_H */