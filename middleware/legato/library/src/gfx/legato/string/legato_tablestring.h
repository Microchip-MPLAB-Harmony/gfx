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

/** \file legato_tablestring.h
* @brief Definition for the table string feature.
*
* @details .
*/

#ifndef LEGATO_TABLESTRING_H
#define LEGATO_TABLESTRING_H

#include "gfx/legato/string/legato_string.h"

#define LE_STRING_NULLIDX -1

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
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

/**
  * @endcond
  *
  */
// *****************************************************************************
/* Structure:
    leTableString

  Summary:
    String type that references an entry in a string table.  Read only.

  Remarks:
    None.
*/
/**
 * @brief This struct represents a table string.
 * @details Color mode information keeps track of size, bpp,
 * color channel mask and color.
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
/**
 * @brief Constructs a new table string.
 * @details Creates a new table in table  <span class="param">idx</span>.
 * @see leString_New().
 * @code
 * uint32_t idx;
 * leTableString* tstr = leTableString_New(idx);
 * @endcode
 * @param  idx is the table from which to create
 * @return returns a new table string.
 */
leTableString* leTableString_New(uint32_t idx);

// *****************************************************************************
/* Function:
    void leTableString_New(leTableString* str, uint32_t idx)

   Summary:
    Constructs a new table string at the given pointer

   Parameters:
    idx - the table index to reference

  Returns:
    
  Remarks:
    It is assumed that the pointer provided is being managed by the caller.  Use
    leString_PDelete() to properly destruct this pointer.
*/
/**
 * @brief Constructs a new table string.
 * @details Creates a new table in table  <span class="param">idx</span>.
 * @see leString_New().
 * @code
 * uint32_t idx;
 * leTableString* tstr = leTableString_New(idx);
 * @endcode
 * @param  idx is the table from which to create
 * @return returns a new table string.
 */
/**
 * @brief Initialize table string.
 * @details Initializes the leTableString <span class="param">str</span>
 * at <span class="param">idx</span>.
 * @code
 * leTableString* str;
 * leTableString_Constructor(str, idx);
 * @endcode
 * @param str is the string to initialize
 * @param idx is the index location
 * @return void.
 */
void leTableString_Constructor(leTableString* str,
                               uint32_t idx);


#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Get id.
 * @details Gets the ID from <span class="param">_this</span>.
 * @code
 * leTableString* _this;
 * uint32_t id = _this->fn->getID(str);
 * @endcode
 * @param  _this is the string to examine.
 * @return returns the id of the string.
 */
virtual uint32_t getID(const leTableString* _this);


// *****************************************************************************
/**
 * @brief Set string table ID.
 * @details Sets the ID to <span class="param">id</span>.
 * @code
 * leTableString* str;
 * leResult res = _this->fn->setID(_this, id);
 * @endcode
 * @param  _this is the string to examine.
 * @param id is the string table ID to use.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setID(leTableString* this,
                       uint32_t id);


// *****************************************************************************
/**
 * @brief Get size of this string in bytes.
 * @details Gets the size of this string in bytes.
 * @code
 * leTableString* str;
 * uint32_t size = str->fn->sizeInBytes(str);
 * @endcode
 * @param  str is the string to examine.
 * @return returns the size of the string table.
 */
virtual uint32_t sizeInBytes(const leTableString* this);


#undef THIS_TYPE
#endif


#endif /* LEGATO_TABLESTRING_H */
