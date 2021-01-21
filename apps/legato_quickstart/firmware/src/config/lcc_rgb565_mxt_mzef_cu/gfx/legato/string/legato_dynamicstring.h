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

#ifndef LEGATO_DYNAMICSTRING_H
#define LEGATO_DYNAMICSTRING_H

#include "gfx/legato/string/legato_string.h"

// DOM-IGNORE-BEGIN
struct leDynamicString;

#define LE_DYNAMICSTRING_VTABLE(THIS_TYPE) \
    LE_STRING_VTABLE(THIS_TYPE) \
    \
	uint32_t (*getCapacity)(THIS_TYPE* str); \
	leResult (*setCapacity)(THIS_TYPE* str, uint32_t cap); \

typedef struct leDynamicStringVTable
{
	LE_DYNAMICSTRING_VTABLE(struct leDynamicString)
} leDynamicStringVTable;
// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leDynamicString

  Summary:
    String type that dynamically allocates internal memory to accommodate dynamic
    string operations.

  Remarks:
    None.
*/
typedef struct leDynamicString
{
    leString base; // string base data

	const leDynamicStringVTable* fn; // function table
    
    leChar*     data;     // local string data storage
    
    uint16_t    capacity; // actual memory capacity of the string
    uint16_t    length;   // actual length of the string
                          
    const leFont*  font; // the font used for this string 
} leDynamicString;

// *****************************************************************************
/* Function:
    leDynamicString* leDynamicString_New()

   Summary:
    Allocates a memory for a new dynamic string and automatically calls its
    constructor function

   Parameters:
    

  Returns:
    leDynamicString* - pointer to the newly allocated string

  Remarks:
    Caller is responsible for freeing the memory allocated by this function
    using leDynamicString_Delete()
*/
LIB_EXPORT leDynamicString* leDynamicString_New();

// *****************************************************************************
/* Function:
    void leDynamicString_Constructor(leDynamicString* str)

   Summary:
    Constructs a new dynamic string at the given pointer

   Parameters:
    leDynamicString* str - the string to construct

  Returns:
    
  Remarks:
    It is assumed that the pointer provided is being managed by the caller.  Use
    the destructor member function to clean properly clean this object up.
*/
LIB_EXPORT void leDynamicString_Constructor(leDynamicString* str);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getCapacity(leDynamicString* str)

  Summary:
    Gets the capacity of the dynamic string

  Description:
    Gets the capacity of the dynamic string

  Parameters:
    leDynamicString* str - The string to operate on

  Remarks:
    Usage - _this->fn->getCapacity(_this);

  Returns:
    uint32_t - the current string capacity
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCapacity(leDynamicString* str,
                         uint32_t cap)

  Summary:
    Sets the capacity of the dynamic string

  Description:
    Sets the capacity of the dynamic string

  Parameters:
    leDynamicString* str - The string to operate on
    uint32_t cap - the desired capacity

  Remarks:
    Usage - _this->fn->setCapacity(_this, cap);

  Returns:
    leResult - the result of the operation
*/


#endif /* LEGATO_DYNAMICSTRING_H */