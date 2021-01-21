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



#ifndef SYS_INPUT_MOUSE_H
#define SYS_INPUT_MOUSE_H
//DOM-IGNORE-END

#include <stdint.h>
#include <stddef.h>

// *****************************************************************************
/* Enumeration:
    SYS_INP_MouseButton

  Summary:
    Enumerates mouse button value types

  Remarks:
    None.
*/
typedef enum SYS_INP_MouseButton_t
{
    SYS_INP_BUTTON_NONE = 0,
    SYS_INP_BUTTON_LEFT,
    SYS_INP_BUTTON_MIDDLE,
    SYS_INP_BUTTON_RIGHT,
    SYS_INP_BUTTON_WHEEL_UP,
    SYS_INP_BUTTON_WHEEL_DOWN,
    SYS_INP_BUTTON_LAST = SYS_INP_BUTTON_WHEEL_DOWN
} SYS_INP_MouseButton;

/* Structure:
    SYS_INP_MouseButtonEvent

  Summary:
    Defines a generic mouse button event

  Description:
    btn - the mouse button value that that experienced a change in state
    
  Remarks:
    None.
*/
typedef struct SYS_INP_MouseButtonEvent_t
{
    SYS_INP_MouseButton btn;
} SYS_INP_MouseButtonEvent;

/* Structure:
    SYS_INP_MouseMoveEvent

  Summary:
    Defines a mouse move event

  Description:
    x - the amount of change in x
    y - the amount of change in y
    
  Remarks:
    None.
*/
typedef struct SYS_INP_MouseMoveEvent_t
{
    uint16_t x;
    uint16_t y;
} SYS_INP_MouseMoveEvent;

/* Mouse Handler Function Definitions */
typedef void (*SYS_INP_HandleMouseMoveEvent)(const SYS_INP_MouseMoveEvent* const);
typedef void (*SYS_INP_HandleMouseButtonDownEvent)(const SYS_INP_MouseButtonEvent* const);
typedef void (*SYS_INP_HandleMouseButtonUpEvent)(const SYS_INP_MouseButtonEvent* const);

#endif /* SYS_INPUT_MOUSE_H */