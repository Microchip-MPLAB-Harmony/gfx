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
    sys_input_listener.h

  Summary:
    MPLAB Harmony Input System Service listener definition

  Description:
    MPLAB Harmony Input System Service listener definition
*******************************************************************************/



#ifndef SYS_INPUT_LISTENER_H
#define SYS_INPUT_LISTENER_H
//DOM-IGNORE-END

#include "sys_input_gesture.h"
#include "sys_input_keyboard.h"
#include "sys_input_mouse.h"
#include "sys_input_touch.h"

// *****************************************************************************
/* Structure:
    SYS_INP_InputListener

  Summary:
    Defines input event listener functions.
    
  Description:
    An entity that desires to listen for input events must provide an
    SYS_INP_InputListener implementation and fill the function pointers
    appropriately to receive the events.
    
  Remarks:
    None
*/
typedef struct SYS_INP_InputListener_t
{
    /* keyboard handlers */
    SYS_INP_HandleKeyDownEvent handleKeyDown;
    SYS_INP_HandleKeyUpEvent handleKeyUp;
    
    /* mouse handlers */
    SYS_INP_HandleMouseMoveEvent handleMouseMove;
    SYS_INP_HandleMouseButtonDownEvent handleMouseButtonDown;
    SYS_INP_HandleMouseButtonUpEvent handleMouseButtonUp;
    
    /* touch handlers */
    SYS_INP_HandleTouchDownEvent handleTouchDown;
    SYS_INP_HandleTouchUpEvent handleTouchUp;
    SYS_INP_HandleTouchMoveEvent handleTouchMove;
    
    /* gesture handlers */
    SYS_INP_HandleFlickGestureEvent handleFlickGesture;
    SYS_INP_HandlePinchGestureEvent handlePinchGesture;
    SYS_INP_HandleStretchGestureEvent handleStretchGesture;
    SYS_INP_HandleRotateGestureEvent handleRotateGesture;
	SYS_INP_HandleGenericGestureEvent handleGenericGesture;
    
} SYS_INP_InputListener;

// *****************************************************************************
/* Function:
    void SYS_INP_ListenerInit(SYS_INP_InputListener* lst);

  Summary:
    Initializes an input listener struct to all empty functions.  Implementers
    must then overwrite the default function pointers with
    implementation-specific functions to handle events.
    
  Returns:
    None
*/
void SYS_INP_ListenerInit(SYS_INP_InputListener* l);


#endif /* SYS_INPUT_LISTENER_H */