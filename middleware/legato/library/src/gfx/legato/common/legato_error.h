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

/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_error.h

  Summary:
    Defines library assert macros.

  Description:
    Defines library assert macros.
*******************************************************************************/

/** \file legato_error.h
 * @brief Error functions, macros and definitions
 *
 * @details Defines library assert macros.
 */

#ifndef LE_ERROR_H
#define LE_ERROR_H

#include "gfx/legato/common/legato_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

#if LE_ASSERT_ENABLE == 1
//#include <assert.h>
#endif

/* controls whether library-wide asserts are enabled */
#if LE_ASSERT_ENABLE == 1
//#define LE_ASSERT(val)          assert(val)
//#define LE_ASSERT_MSG(val, msg) for ( ; !(val) ; assert(val) ) { leSetErrorMessage(msg); }
//#define LE_ASSERT_VAR(val, fmt, ...) for ( ; !(val) ; assert(val) ) { leSprintfErrorMessage(fmt, __VA_ARGS__); }
#define LE_ASSERT(val)          if((val) == 0) { leAssert(); }
#define LE_ASSERT_MSG(val, msg) if((val) == 0) { leSetErrorMessage(msg); }
#define LE_ASSERT_VAR(val, fmt, ...) if((val) == 0) { leSprintfErrorMessage(fmt, __VA_ARGS__); }
#else
#define LE_ASSERT(val)
#define LE_ASSERT_MSG(val, msg)
#define LE_ASSERT_VAR(val, fmt, ...)
#endif

#define LE_ASSERT_THIS() LE_ASSERT_MSG(_this != NULL, "'this' pointer is null")

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

#if LE_ASSERT_ENABLE == 1

/**
 * @brief Exits the application.
 * @details Exits the application by calling exit().
 * @code
 * LE_ASSERT(val);
 * @endcode
 * @param param1 val is not used.
 * @return void.
 */
#ifndef _MSC_VER
    void leAssert(void) __attribute__((noreturn));;
#else
    __declspec(noreturn) void leAssert(void);
#endif

// *****************************************************************************
/* Function:
    const char* leGetErrorMessage()

  Summary:
    Gets the library global error message.

  Description:
    Gets the library global error message.

  Parameters:

  Returns:
    const char* msg - the error message

  Remarks:

*/
/**
 * @brief Get global error message.
 * @details Gets the global error message set.
 * @see leSetErrorMessage()
 * @code
 * char* msg = leGetErrorMessage();
 * @endcode
 * @return pointer the global error message.
 */
const char* leGetErrorMessage(void);

// *****************************************************************************
/* Function:
    void leSetErrorMessage(const char* msg)

  Summary:
    Sets the library global error message.

  Description:
    Sets the library global error message.

  Parameters:
    const char* msg - the error message

  Returns:

  Remarks:

*/
/**
 * @brief Set global error message.
 * @details Sets the global error message to be used for debugging.
 * @see leGetErrorMessage()
 * @code
 * const char* msg =  "a message";
 * leSetErrorMessage(msg);
 * @endcode
 * @param param1 msg is the message to print.
 * @return void.
 */
void leSetErrorMessage(const char* msg);

// *****************************************************************************
/* Function:
    void leSetErrorMessage(const char* fmt, ...)

  Summary:
    Sets the library global error message using a formatted string.

  Description:
    Sets the library global error message using a formatted string.

  Parameters:
    const char* fmt - the formatted error message string
    varargs - the arguments to the formatted string

  Returns:

  Remarks:

*/
/**
 * @brief Set formatted global error message.
 * @details Sets a sprintf style formatted global error message to be used for debugging.
 * @see leGetErrorMessage()
 * @see sprintf()
 * @code
 * const char* msg =  "Error: %d message";
 * leSprintfErrorMessage(fmt);
 * @endcode
 * @param param1 msg is the message to print.
 * @return void.
 */
#ifndef _MSC_VER
void leSprintfErrorMessage(const char* fmt, ...) __attribute__((noreturn));;
#else
__declspec(noreturn) void leSprintfErrorMessage(const char* fmt, ...);
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif /* LE_ERROR_H */
