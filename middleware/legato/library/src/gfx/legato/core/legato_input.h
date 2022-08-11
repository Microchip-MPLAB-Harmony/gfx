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
    legato_input.h

  Summary:
    

*******************************************************************************/

/** \file legato_input.h
 * @brief Legato input definitions.
 *
 * @details Defines events that are used in the UI library.  Events
 * are created and stored for later processing during a library context's update loop.
 */
#ifndef LEGATO_INPUT_H
#define LEGATO_INPUT_H

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/core/legato_event.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

#include "gfx/legato/widget/legato_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

/* internal use only */
/**
  * @cond INTERNAL
  */
#define LE_INPUT_PRIMARY_ID    0
#define LE_MAX_TOUCH_STATES    2
/**
  * @endcond
  *
  */

// *****************************************************************************

/**
 * @brief This enum represents gesture type IDs.
 * @details This enum type defines the valid gesture types.
 */
typedef enum leGestureID
{
    LE_GESTURE_NONE = 0  /**< No event found. */
} leGestureID;

// *****************************************************************************
/* Enumeration:
    leKey

  Summary:
    All values possible for key entry from the legato keyboard widget

  Description:
    All values possible for key entry from the legato keyboard widget

  Remarks:
    None.
*/
/**
 * @brief This enum represents legato keyboard entry.
 * @details All values possible for key entry from
 * the legato keyboard widget
 */
typedef enum leKey
{
    KEY_NULL = 0,
    KEY_ESCAPE,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_PRINTSCREEN,
    KEY_SCROLLLOCK,
    KEY_PAUSE,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_BACKQUOTE,
    KEY_TAB,
    KEY_CAPSLOCK,
    KEY_BRACKET_LEFT,
    KEY_BRACKET_RIGHT,
    KEY_SLASH,
    KEY_SEMICOLON,
    KEY_QUOTE,
    KEY_BACKSLASH,
    KEY_EQUALS,
    KEY_BACKSPACE,
    KEY_MINUS,
    KEY_COMMA,
    KEY_ENTER,
    KEY_PERIOD,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_SPACE,
    KEY_LCTRL,
    KEY_RCTRL,
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_LALT,
    KEY_RALT,
    KEY_LMETA,
    KEY_RMETA,
    KEY_INSERT,
    KEY_HOME,
    KEY_PAGEUP,
    KEY_END,
    KEY_PAGEDOWN,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_NUMLOCK,
    KEY_KP_DIVIDE,
    KEY_KP_MULTIPLY,
    KEY_KP_MINUS,
    KEY_KP_PLUS,
    KEY_KP_ENTER,
    KEY_KP_1,
    KEY_KP_2,
    KEY_KP_3,
    KEY_KP_4,
    KEY_KP_5,
    KEY_KP_6,
    KEY_KP_7,
    KEY_KP_8,
    KEY_KP_9,
    KEY_KP_0,
    KEY_KP_PERIOD,
    KEY_LAST = KEY_KP_PERIOD
} leKey;

// *****************************************************************************
/**
 * @brief This enum represents key entry for legato mouse input.
 * @details All values possible for mouse key entry from the legato
 * mouse input.
 */
typedef enum leMouseButton
{
    BUTTON_NONE = 0,
    BUTTON_LEFT,
    BUTTON_MIDDLE,
    BUTTON_RIGHT,
    BUTTON_WHEEL_UP,
    BUTTON_WHEEL_DOWN,
    BUTTON_LAST = BUTTON_WHEEL_DOWN
} leMouseButton;

#define NUM_KEYS    KEY_LAST + 1
#define NUM_BUTTONS BUTTON_LAST + 1

// *****************************************************************************
/**
 * @brief This struct represents a touch state.
 * @details A touch state has valid touch indicator and touch coordinate location.
 * mouse input.
 */
typedef struct leTouchState
{
    uint32_t valid; /**< Valid touch. */
    int32_t x; /**< x coordinate location. */
    int32_t y; /**< y coordinate location. */
} leTouchState;

// *****************************************************************************
/**
 * @brief This struct represents input state.
 * @details Input state maintains a history of touch states; currently legato keeps
 * track of the last touch state only.
 */
typedef struct leInputState
{
    leBool enabled;

    leTouchState touch[LE_MAX_TOUCH_STATES];
    //uint8_t keyState[KEY_LAST];  // change to bitset?
    //leMouseState mouse;

    int32_t driverAdjustX;
    int32_t driverAdjustY;

} leInputState;

/* internal use only */
/**
  * @cond INTERNAL
  */
leInputState* _leGetInputState(void);

leResult leInput_Init(void);
void leInput_Shutdown(void);
/**
  * @endcond
  *
  */


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leBool leInput_GetEnabled()

  Summary:
    Returns the input enabled status of the current context

  Description:
    Returns the input enabled status of the current context

  Parameters:
    
    
  Returns:
    leBool
    
  Remarks:
    
*/
/**
 * @brief Get input enabled status.
 * @details Returns the input enabled status of the current context.
 * @code
 * leBool status = leInput_GetEnabled();
 * @endcode
 * @param void.
 * @return LE_TRUE if input is enabled, otherwise LE_FALSE.
 */
leBool leInput_GetEnabled(void);

// *****************************************************************************
/* Function:
    leResult leInput_SetEnabled(leBool enable)

  Summary:
    Sets the input status of the current context with the specified input argument 

  Description:
    Sets the input status of the current context with the specified input argument

  Parameters:
    leBool
    
  Returns:
    leResult
    
  Remarks:
    
*/
/**
 * @brief Set input enabled status.
 * @details Sets the input status of the current context to <span class="param">enable</span>.
 * @code
 * leBool enable = true;
 * leResult res = leInput_SetEnabled(enable);
 * @endcode
 * @param enable the new status.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leInput_SetEnabled(leBool enable);

// *****************************************************************************
/* Function:
    leResult leInput_InjectTouchDown(uint32_t id, int32_t x, int32_t y)

  Summary:
    Register and track the touch down event and queue it for handling by associated widgets 

  Description:
    Register and track the touch down event and queue it for handling by associated widgets

  Parameters:
    uint32_t
    int32_t x
    int32_t y
    
  Returns:
    leResult
    
  Remarks:
    
*/
/**
 * @brief Inject touch down event.
 * @details Injects a touch down event for <span class="param">id</span>
 * at location <span class="param">x</span>, <span class="param">y</span>.
 * @code
 * leBool enable = true;
 * leResult res = leInput_InjectTouchDown(id, x, y);
 * @endcode
 * @param id is the touch input id.
 * @param x is the x coordinate.
 * @param y is the y coordinate.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leInput_InjectTouchDown(uint32_t id, int32_t x, int32_t y);

// *****************************************************************************
/* Function:
    leResult leInput_InjectTouchUp(uint32_t id, int32_t x, int32_t y)

  Summary:
    Register and track the touch up event and queue it for handling by associated widgets

  Description:
    Register and track the touch up event and queue it for handling by associated widgets

  Parameters:
    uint32_t
    int32_t x
    int32_t y
    
  Returns:
    leResult
    
  Remarks:
    
*/
/**
 * @brief Inject touch up event.
 * @details Injects a touch up event for <span class="param">id</span>
 * at location <span class="param">x</span>, <span class="param">y</span>.
 * @code
 * leBool enable = true;
 * leResult res = leInput_InjectTouchDown(id, x, y);
 * @endcode
 * @param id is the touch input id.
 * @param x is the x coordinate.
 * @param y is the y coordinate.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leInput_InjectTouchUp(uint32_t id, int32_t x, int32_t y);

// *****************************************************************************
/* Function:
    leResult leInput_InjectTouchMoved(uint32_t id, int32_t x, int32_t y)

  Summary:
    Register and track the touch moved event and queue it for handling by associated widgets

  Description:
    Register and track the touch moved event and queue it for handling by associated widgets

  Parameters:
    uint32_t
    int32_t x
    int32_t y
    
  Returns:
    leResult
    
  Remarks:
    
*/
/**
 * @brief Inject touch moved event.
 * @details Injects a touch moved event for <span class="param">id</span>
 * at location <span class="param">x</span>, <span class="param">y</span>.
 * @code
 * leBool enable = true;
 * leResult res = leInput_InjectTouchDown(id, x, y);
 * @endcode
 * @param id is the touch input id.
 * @param x is the x coordinate.
 * @param y is the y coordinate.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leInput_InjectTouchMoved(uint32_t id, int32_t x, int32_t y);

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
// alternative input APIs (not yet implemented)

/*
leResult leInput_InjectGesture(leGesture id, int32_t x, int32_t y);

leResult leInput_InjectKeyDown(leKey key);
leResult leInput_InjectKeyUp(leKey key);

leResult leInput_InjectMouseButtonDown(leMouseButton button);
leResult leInput_InjectMouseButtonUp(leMouseButton button);
leResult leInput_InjectMouseMoved(int32_t x, int32_t y);*/

leEventResult _leInput_HandleInputEvent(leEvent* evt);
/**
  * @endcond
  *
  */

#ifdef __cplusplus
}
#endif

#endif /* LEGATO_INPUT_H */
