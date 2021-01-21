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

/** \file legato_dynamicstring.h
 * @brief Dynamic string functions and definitions.
 * @details
 */
#ifndef LEGATO_DYNAMICSTRING_H
#define LEGATO_DYNAMICSTRING_H

#include "gfx/legato/string/legato_string.h"

/* internal use only */
/**
  * @cond INTERNAL
  */
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
/**
  * @endcond
  */

// *****************************************************************************
/**
 * @brief This struct represents a dynamic string.
 * @details String type that dynamically allocates internal memory to accommodate
 * dynamic string operations
 */
typedef struct leDynamicString
{
    leString base;                      /**< base data */
    const leDynamicStringVTable* fn;    /**< function table */
    leChar*     data;                   /**< data storage */
    uint16_t    capacity;               /**< string capacity */
    uint16_t    length;                 /**< string length */
    const leFont*  font;                /**< string font */
} leDynamicString;

// *****************************************************************************
/**
 * @brief Create a new dynamic string.
 * @details Creates a new leDynamicString and automatically calls
 * its constructor function.
 * @remark Caller is responsible for freeing the memory allocated by
 * this function.
 * @see leDynamicString_Delete()
 * @code
 * leDynamicString * str = leDynamicString_New(void);
 * @endcode
 * @return pointer to the newly allocated string
 */
leDynamicString* leDynamicString_New(void);

// *****************************************************************************
/**
 * @brief Consructs a dynamic string.
 * @details Allocates a memory for an existing <span style="color: #820a32"><em>str</em></span>.
 * @remark It is assumed that the pointer provided is being managed by the caller.
 * Use the destructor member function to destroy the string.
 * @code
 * leDynamicString* str;
 * leDynamicString_Constructor(str);
 * @endcode
 * @param str the string to construct.
 * @return void
 */
void leDynamicString_Constructor(leDynamicString* str);


#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Get capacity of the dynamic string.
 * @details Gets the capacity of <span class="param">_this</span>.
 * @code
 * leDynamicString* _this;
 * uint32_t cap = _this->fn->getCapacity(_this);
 * @endcode
 * @param  str is the string to operate on
 * @return the current string capacity.
 */
virtual uint32_t getCapacity(leDynamicString* _this);

// *****************************************************************************
/**
 * @brief Set capacity of the dynamic string.
 * @details Sets capacity of <span class="param">_this</span> to <span class="param">cap</span>.
 * @code
 * leDynamicString* _this;
 * uint32_t cap;
 * leResult res = _this->fn->setCapacity(_this, cap);
 * @endcode
 * @param  _this is the string to operate on;
 * @param cap is the desired capacity;
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCapacity(leDynamicString* _this,
                             uint32_t cap);

#undef THIS_TYPE
#endif

#endif /* LEGATO_DYNAMICSTRING_H */
