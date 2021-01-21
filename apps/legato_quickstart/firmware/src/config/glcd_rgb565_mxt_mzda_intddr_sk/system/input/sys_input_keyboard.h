// DOM-IGNORE-BEGIN
/*******************************************************************************
 Module for Microchip Harmony Input System Service

  Company:
    Microchip Technology Inc.

  File Name:
    sys_input_touch.h

  Summary:
    MPLAB Harmony Input System Service touch event definition

  Description:
    MPLAB Harmony Input System Service touch event definition
*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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


#ifndef SYS_INPUT_KEYBOARD_H
#define SYS_INPUT_KEYBOARD_H
//DOM-IGNORE-END

#include <stdint.h>
#include <stddef.h>

// *****************************************************************************
/* Enumeration:
    SYS_INP_Key

  Summary:
    Enumerates key value types

  Remarks:
    None.
*/
typedef enum SYS_INP_Key_t
{
    SYS_INP_KEY_NULL = 0,
    SYS_INP_KEY_ESCAPE,
    SYS_INP_KEY_F1,
    SYS_INP_KEY_F2,
    SYS_INP_KEY_F3,
    SYS_INP_KEY_F4,
    SYS_INP_KEY_F5,
    SYS_INP_KEY_F6,
    SYS_INP_KEY_F7,
    SYS_INP_KEY_F8,
    SYS_INP_KEY_F9,
    SYS_INP_KEY_F10,
    SYS_INP_KEY_F11,
    SYS_INP_KEY_F12,
    SYS_INP_KEY_PRINTSCREEN,
    SYS_INP_KEY_SCROLLLOCK,
    SYS_INP_KEY_PAUSE,
    SYS_INP_KEY_1,
    SYS_INP_KEY_2,
    SYS_INP_KEY_3,
    SYS_INP_KEY_4,
    SYS_INP_KEY_5,
    SYS_INP_KEY_6,
    SYS_INP_KEY_7,
    SYS_INP_KEY_8,
    SYS_INP_KEY_9,
    SYS_INP_KEY_0,
    SYS_INP_KEY_BACKQUOTE,
    SYS_INP_KEY_TAB,
    SYS_INP_KEY_CAPSLOCK,
    SYS_INP_KEY_BRACKET_LEFT,
    SYS_INP_KEY_BRACKET_RIGHT,
    SYS_INP_KEY_SLASH,
    SYS_INP_KEY_SEMICOLON,
    SYS_INP_KEY_QUOTE,
    SYS_INP_KEY_BACKSLASH,
    SYS_INP_KEY_EQUALS,
    SYS_INP_KEY_BACKSPACE,
    SYS_INP_KEY_MINUS,
    SYS_INP_KEY_COMMA,
    SYS_INP_KEY_ENTER,
    SYS_INP_KEY_PERIOD,
    SYS_INP_KEY_A,
    SYS_INP_KEY_B,
    SYS_INP_KEY_C,
    SYS_INP_KEY_D,
    SYS_INP_KEY_E,
    SYS_INP_KEY_F,
    SYS_INP_KEY_G,
    SYS_INP_KEY_H,
    SYS_INP_KEY_I,
    SYS_INP_KEY_J,
    SYS_INP_KEY_K,
    SYS_INP_KEY_L,
    SYS_INP_KEY_M,
    SYS_INP_KEY_N,
    SYS_INP_KEY_O,
    SYS_INP_KEY_P,
    SYS_INP_KEY_Q,
    SYS_INP_KEY_R,
    SYS_INP_KEY_S,
    SYS_INP_KEY_T,
    SYS_INP_KEY_U,
    SYS_INP_KEY_V,
    SYS_INP_KEY_W,
    SYS_INP_KEY_X,
    SYS_INP_KEY_Y,
    SYS_INP_KEY_Z,
    SYS_INP_KEY_SPACE,
    SYS_INP_KEY_LCTRL,
    SYS_INP_KEY_RCTRL,
    SYS_INP_KEY_LSHIFT,
    SYS_INP_KEY_RSHIFT,
    SYS_INP_KEY_LALT,
    SYS_INP_KEY_RALT,
    SYS_INP_KEY_LMETA,
    SYS_INP_KEY_RMETA,
    SYS_INP_KEY_INSERT,
    SYS_INP_KEY_HOME,
    SYS_INP_KEY_PAGEUP,
    SYS_INP_KEY_END,
    SYS_INP_KEY_PAGEDOWN,
    SYS_INP_KEY_RIGHT,
    SYS_INP_KEY_LEFT,
    SYS_INP_KEY_DOWN,
    SYS_INP_KEY_UP,
    SYS_INP_KEY_NUMLOCK,
    SYS_INP_KEY_KP_DIVIDE,
    SYS_INP_KEY_KP_MULTIPLY,
    SYS_INP_KEY_KP_MINUS,
    SYS_INP_KEY_KP_PLUS,
    SYS_INP_KEY_KP_ENTER,
    SYS_INP_KEY_KP_1,
    SYS_INP_KEY_KP_2,
    SYS_INP_KEY_KP_3,
    SYS_INP_KEY_KP_4,
    SYS_INP_KEY_KP_5,
    SYS_INP_KEY_KP_6,
    SYS_INP_KEY_KP_7,
    SYS_INP_KEY_KP_8,
    SYS_INP_KEY_KP_9,
    SYS_INP_KEY_KP_0,
    SYS_INP_KEY_KP_PERIOD,
    SYS_INP_KEY_LAST = SYS_INP_KEY_KP_PERIOD
} SYS_INP_Key;

/* Structure:
    SYS_INP_KeyEvent

  Summary:
    Defines a generic key event

  Description:
    key - the key value that that experienced a change in state
    
  Remarks:
    None.
*/
typedef struct SYS_INP_KeyEvent_t
{
    SYS_INP_Key key;
} SYS_INP_KeyEvent;

/* Event handler function definitions */
typedef void (*SYS_INP_HandleKeyDownEvent)(const SYS_INP_KeyEvent* const);
typedef void (*SYS_INP_HandleKeyUpEvent)(const SYS_INP_KeyEvent* const);

#endif /* SYS_INPUT_KEYBOARD_H */