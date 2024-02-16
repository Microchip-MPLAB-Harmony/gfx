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
    legato_common.h

  Summary:
    This file defines the common macros and definitions used by the gfx
    definition and implementation headers.

  Description:
    This file defines the common macros and definitions used by the gfx
    definition and the implementation header.

  Remarks:
    The directory in which this file resides should be added to the compiler's
    search path for header files.
 *******************************************************************************/

/** \file legato_common.h
 * @brief Common macros and definitions used by Legato.
 *
 * @details This file defines the common macros and definitions used
 * by the legato library definition and implementation files.
 *
 * @remark The directory in which this file resides should be added to the
 * compiler's search path for header files.
 */

#ifndef LEGATO_COMMON_H
#define LEGATO_COMMON_H

#include "gfx/legato/legato_config.h"

#include <stddef.h>
#include <assert.h>
#include <string.h>

#ifdef _WIN32
#ifndef int8_t
typedef signed __int8        int8_t;
typedef signed __int16       int16_t;
typedef signed __int32       int32_t;
typedef unsigned __int8      uint8_t;
typedef unsigned __int16     uint16_t;
typedef unsigned __int32     uint32_t;
typedef signed __int64       int64_t;
typedef unsigned __int64     uint64_t;
#endif
#else
#include <stdint.h>
#endif

#ifndef WIN32
//#include "osal/osal.h" FIXME for H3
#endif

/**
 * @brief Defines the default color of mode for all schemes.
 * @details In Legato there is a need to identify the default
 * color mode for schemes.
 */
#ifndef LE_DEFAULT_SCHEME_COLOR_MODE
#define LE_DEFAULT_SCHEME_COLOR_MODE LE_COLOR_MODE_RGB_565
#endif

#ifdef __cplusplus
    extern "C" {
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leResult

  Summary:
    legato results (success and failure codes).

  Description:
    Various definitions for success and failure codes.

  Remarks:
    None.
*/

/**
 * @brief This enum represents function call results.
 * @details Results are used to report success and failure conditions. These
 * are the various success and failure codes.
 */
typedef enum leResult
{
    LE_FAILURE = -1,
    LE_SUCCESS = 0
} leResult;

// *****************************************************************************
/* Enumeration:
    leBool

  Summary:
    legato bool values

  Description:
    legato bool values

  Remarks:
    None.
*/
/**
 * @brief This enum represents booleans.
 * @details Boolean are used to communicate true and false conditions.
 */
typedef enum leBool
{
    LE_FALSE = 0,   /**< Logic false =0. */
    LE_TRUE         /**< Logic true = 1. */
} leBool;

// *****************************************************************************
/* Enumeration:
    leVAlignment

  Summary:
    legato vertical alignment values

  Description:
    legato vertical alignment values

  Remarks:
    None.
*/   
/**
 * @brief This enum represents the vertical alignment mode of objects.
 * @details The vertical alignment mode controls how objects are positioned
 * vertically.
 */
typedef enum
{
    LE_VALIGN_TOP,      /**< Aligns with the top. */
    LE_VALIGN_MIDDLE,   /**< Aligns with the middle. */
    LE_VALIGN_BOTTOM    /**< Aligns with the bottom. */
} leVAlignment;

// *****************************************************************************
/* Enumeration:
    leHAlignment

  Summary:
    legato horizontal alignment values

  Description:
    legato horizontal alignment values

  Remarks:
    None.
*/
/**
 * @brief This enum represents the horizontal alignment mode of objects.
 * @details The horizontal alignment mode controls how objects are positioned
 * horizontally.
 */
typedef enum
{
    LE_HALIGN_LEFT,     /**< Aligns with the left edge. */
    LE_HALIGN_CENTER,   /**< Aligns with the center. */
    LE_HALIGN_RIGHT     /**< Aligns with the right edge. */
} leHAlignment;

// *****************************************************************************
/* Enumeration:
    leMargin

  Summary:
    legato margin values

  Description:
    legato margin values

  Remarks:
    None.
*/
/**
 * @brief This struct represents the four margin settings for objects.
 * @details The four margin: left, top, right, bottom controls
 * the size of the borders around objects.
 */
typedef struct leMargin
{
    uint8_t left;       /**< Aligns with the left margin. */
    uint8_t top;        /**< Aligns with the top margin. */
    uint8_t right;      /**< Aligns with the right margin. */
    uint8_t bottom;     /**< Aligns with the bottom margin. */
} leMargin;

/**
 * @brief This const represents zero margin settings for objects.
 * @details .
 */
static const leMargin leMargin_Zero = {0, 0, 0, 0};

// *****************************************************************************
/* Enumeration:
    leDirection

  Summary:
    legato direction values

  Description:
    legato direction values

  Remarks:
    None.
*/
/**
 * @brief This enum represents the four directions modes for objects.
 * @details The four directions: right, down, left, up controls
 * the direction of objects.
 */
typedef enum leDirection
{
  LE_DIRECTION_RIGHT,   /**< Specifies right direction. */
  LE_DIRECTION_DOWN,    /**< Specifies down direction. */
  LE_DIRECTION_LEFT,    /**< Specifies left direction. */
  LE_DIRECTION_UP       /**< Specifies up direction. */
} leDirection;

// *****************************************************************************
/* Structure:
    leRotationDirection

  Summary:
    Describes rotational direction

  Description:
    Describes rotational direction

  Remarks:
    None.
*/
/**
 * @brief This enum represents the two rotation direction modes for objects.
 * @details The two directions: counter clockwise, clockwise controls
 * the rotation direction of objects.
 */
typedef enum leRotationDirection
{
    LE_COUNTER_CLOCKWISE,   /**< Specifies counter clockwise rotation. */
    LE_CLOCKWISE,           /**< Specifies clockwise rotation. */
} leRotationDirection;

// *****************************************************************************
/* Enumeration:
    leRelativePosition

  Summary:
    legato relative position values

  Description:
    legato relative position values

  Remarks:
    None.
*/
/**
 * @brief This enum represents the relative position modes for objects.
 * @details The relative position modes controls
 * the relative position of objects to other objects.
 */
typedef enum leRelativePosition
{
    LE_RELATIVE_POSITION_LEFTOF,    /**< Position to the left of object. */
    LE_RELATIVE_POSITION_ABOVE,     /**< Positive to the right of object. */
    LE_RELATIVE_POSITION_BELOW,     /**< Positive to the top of object. */
    LE_RELATIVE_POSITION_RIGHTOF,   /**< Positive to the bottom of object. */
    LE_RELATIVE_POSITION_BEHIND     /**< Positive to the top of object. */
} leRelativePosition;

// *****************************************************************************
/* Enumeration:
    leOrientation

  Summary:
    legato orientation values

  Description:
    legato orientation values

  Remarks:
    None.
*/
/**
 * @brief This enum represents the orientation modes for objects.
 * @details The orientation modes controls the horizontal and vertical of
 * objects to other objects.
 */
typedef enum leOrientation
{
  LE_ORIENTATION_HORIZONTAL,
  LE_ORIENTATION_VERTICAL,
} leOrientation;

// *****************************************************************************
/* Structure:
    lePoint

  Summary:
    A two dimensional Cartesian point.
*/
/**
 * @brief This structure represents a integer Cartesian point.
 * @details A point is specified by an x and y coordinate.
 */
typedef struct lePoint
{
    int32_t x;
    int32_t y;
} lePoint;

/**
 * @brief This const represents zero point setting for a coordinate .
 * @details .
 */
static const lePoint lePoint_Zero = {0, 0};

// *****************************************************************************
/* Structure:
    leSize

  Summary:
    A two dimensional indication of size.  Values are signed but should never be
    negative.
*/
/**
 * @brief This structure is used represents the size of an item.
 * @details A size is specified by a width and height.
 */
typedef struct leSize
{
    int32_t width;      /**< width of an item. */
    int32_t height;     /**< height of an item. */
} leSize;

/**
 * @brief This const represents zero size setting for an object .
 * @details .
 */
static const leSize leSize_Zero = {0, 0};

// *****************************************************************************
/* Structure:
    leRect

  Summary:
    A rectangle definition.  
*/
/**
 * @brief This struct represents a rectangle.
 * @details A rectangle is specified by x and y top-left position and a width and height
 * bottom-right position.
 */
typedef struct leRect
{
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
} leRect;

/**
 * @brief This typedef represents general-purpose buffer.
 * @details The address of any variable of any data type (char, int,
 * float etc.) can be assigned to a void pointer variable.
 */
typedef void* leBuffer;

/* library configuration flags */
/**
 * @brief This typedef represents Legato character.
 * @details A Legato character is a 16-bit character.
 */
typedef uint16_t leChar;

#define LE_UNKNOWN_GLYPH  0xFFFF

#ifndef _MSC_VER
#define LE_PCALL(var, func, args...) var->fn->func(var, ## args)
#define LE_OCALL(var, func, args...) var.fn->func(&var, ## args)
#else
#define LE_PCALL(var, func, ...) var->fn->func(var, __VA_ARGS__)
#define LE_OCALL(var, func, ...) var.fn->func(&var, __VA_ARGS__)
#endif

#ifdef __cplusplus
    }
#endif

#define ASSIGN_VIRTUAL_FUNCTION(name) .name = name
#define ASSIGN_VTABLE_FUNCTION(vtable, name) vtable.name = name
#endif // LEGATO_COMMON_H

/*******************************************************************************
 End of File
*/
