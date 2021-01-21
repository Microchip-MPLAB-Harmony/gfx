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

//DOM-IGNORE-BEGIN
#ifndef LEGATO_COMMON_H
#define LEGATO_COMMON_H
//DOM-IGNORE-END

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

/* export macros for shared libraries */
#ifdef _WIN32
#ifdef LIBRARY
#define LIB_EXPORT __declspec( dllexport )
#else
#define LIB_EXPORT __declspec( dllimport )
#endif
#else
#define LIB_EXPORT 
#endif

#ifndef WIN32
//#include "osal/osal.h" FIXME for H3
#endif

// DOM-IGNORE-BEGIN
#ifndef LE_DEFAULT_SCHEME_COLOR_MODE
#define LE_DEFAULT_SCHEME_COLOR_MODE LE_COLOR_MODE_RGB_565
#endif
// DOM-IGNORE-END

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
    extern "C" {
#endif
// DOM-IGNORE-END
        
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
typedef enum leBool
{
    LE_FALSE = 0,
    LE_TRUE
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
typedef enum
{
    LE_VALIGN_TOP,
    LE_VALIGN_MIDDLE,
    LE_VALIGN_BOTTOM
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
typedef enum
{
    LE_HALIGN_LEFT,
    LE_HALIGN_CENTER,
    LE_HALIGN_RIGHT
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
typedef struct leMargin
{
    uint8_t left;
    uint8_t top;
    uint8_t right;
    uint8_t bottom;
} leMargin;

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
typedef enum leDirection
{
  LE_DIRECTION_RIGHT,
  LE_DIRECTION_DOWN,
  LE_DIRECTION_LEFT,
  LE_DIRECTION_UP
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
typedef enum leRotationDirection
{
    LE_COUNTER_CLOCKWISE,
    LE_CLOCKWISE,
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
typedef enum leRelativePosition
{
    LE_RELATIVE_POSITION_LEFTOF,
    LE_RELATIVE_POSITION_ABOVE,
    LE_RELATIVE_POSITION_RIGHTOF,
    LE_RELATIVE_POSITION_BELOW,
    LE_RELATIVE_POSITION_BEHIND
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
typedef struct lePoint
{
    int32_t x;
    int32_t y;
} lePoint;

static const lePoint lePoint_Zero = {0, 0};

// *****************************************************************************
/* Structure:
    leSize

  Summary:
    A two dimensional indication of size.  Values are signed but should never be
    negative.
*/
typedef struct leSize
{
    int32_t width;
    int32_t height;
} leSize;

static const leSize leSize_Zero = {0, 0};

// *****************************************************************************
/* Structure:
    leRect

  Summary:
    A rectangle definition.  
*/
typedef struct leRect
{
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
} leRect;

typedef void* leBuffer;

/* library configuration flags */
typedef uint16_t leChar;
#define LE_UNKNOWN_GLYPH  0xFFFF

// DOM-IGNORE-END

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
    }
#endif
// DOM-IGNORE-END
     
#endif // LEGATO_COMMON_H

/*******************************************************************************
 End of File
*/
