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

/** \file legato_fixedstring.h
 * @brief Fixed string functions and definitions.
 *
 * @details
 */

#ifndef LEGATO_FIXEDSTRING_H
#define LEGATO_FIXEDSTRING_H

#include "gfx/legato/string/legato_string.h"

/**
  * @cond INTERNAL
  */struct leFixedString;
#define LE_FIXEDSTRING_VTABLE(THIS_TYPE) \
    LE_STRING_VTABLE(THIS_TYPE) \
    \
	leResult (*setBuffer)(THIS_TYPE* str, leChar* buf, uint32_t size); \

typedef struct leFixedStringVTable
{
	LE_FIXEDSTRING_VTABLE(struct leFixedString)
} leFixedStringVTable;
/**
  * @endcond
  */

/**
 * @brief This struct represents a fixed string.
 * @details String type that allocates internal memory to accommodate
 * fixed string operations
 */
typedef struct leFixedString
{
    leString base;                    /**< base data */
    const leFixedStringVTable* fn;    /**< function table */
    leChar*     data;                 /**< data storage */
    uint16_t    capacity;             /**< string capacity */
    uint16_t    length;               /**< string length */
    const leFont*  font;              /**< string font */
} leFixedString;

// *****************************************************************************
/**
 * @brief Create a new fixed string.
 * @details Creates a new leFixedString and automatically calls
 * its constructor function.
 * @remark Caller is responsible for freeing the memory allocated by this
 * function using leString_Delete().
 * @code
 * leChar* buf;
 * uint32_t size;
 * leFixedString * str = leFixedString_New(buf, size);
 * @endcode
 * @return pointer to the newly allocated string
 */
leFixedString* leFixedString_New(leChar* buf,
                                 uint32_t size);

// *****************************************************************************
/**
 * @brief Consructs a fixed string.
 * @details Allocates a memory for an existing <span class="param">str </span>.
 * @remark It is assumed that the pointer provided is being managed by the caller.
 * Use the destructor member function to destroy the string.
 * @code
 * leFixedString_Constructor(str, buf, size);
 * @param str is string to construct.
 * @param buf is the buffer to assign to this string.
 * @param size is the size of the buffer.
 * @endcode
 * @return void
 */
void leFixedString_Constructor(leFixedString* str,
                               leChar* buf,
                               uint32_t size);

// *****************************************************************************

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/**
 * @brief Set capacity of the dynamic string.
 * @details Copies up to <span class="param">size</span> items of
 * <span class="param">buf</span> to  <span class="param">_this</span>.
 * @code
 * leFixedString* _this;
 * leChar* buf;
 * uint32_t size;
 * leResult res = _this->fn->setBuffer(_this, buf, size);
 * @endcode
 * @param  _this is the string to operate on;
 * @param buf is the buffer to set;
 * @param size is the size of the buffer;
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setBuffer(leFixedString* _this,
                           leChar* buf,
                           uint32_t size);

#undef THIS_TYPE
#endif


#endif /* LEGATO_FIXEDSTRING_H */
