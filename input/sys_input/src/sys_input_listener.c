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


#include "system/input/sys_input_listener.h"

static void defaultKeyDownHandler(const SYS_INP_KeyEvent* const evt) { }
static void defaultKeyUpHandler(const SYS_INP_KeyEvent* const evt) { }

static void defaultMouseMoveHandler(const SYS_INP_MouseMoveEvent* const evt) { }
static void defaultMouseButtonHandler(const SYS_INP_MouseButtonEvent* const evt) { }

static void defaultTouchStateHandler(const SYS_INP_TouchStateEvent* const evt) { }
static void defaultTouchMoveHandler(const SYS_INP_TouchMoveEvent* const evt) { }

static void defaultFlickGestureHandler(const SYS_INP_FlickGestureEvent* const evt) { }
static void defaultPinchEventHandler(const SYS_INP_PinchGestureEvent* const evt) { }
static void defaultStretchEventHandler(const SYS_INP_StretchGestureEvent* const evt) { }
static void defaultRotateEventHandler(const SYS_INP_RotateGestureEvent* const evt) { }

// initialize an input listener structure
void SYS_INP_ListenerInit(SYS_INP_InputListener* l)
{
    if(l == NULL)
        return;
    
    l->handleKeyDown = &defaultKeyDownHandler;
    l->handleKeyUp = &defaultKeyUpHandler;
    
    l->handleMouseMove = &defaultMouseMoveHandler;
    l->handleMouseButtonDown = &defaultMouseButtonHandler;
    l->handleMouseButtonUp = &defaultMouseButtonHandler;
    
    l->handleTouchDown = &defaultTouchStateHandler;
    l->handleTouchUp = &defaultTouchStateHandler;
    l->handleTouchMove = &defaultTouchMoveHandler;
    
    l->handleFlickGesture = &defaultFlickGestureHandler;
    l->handlePinchGesture = &defaultPinchEventHandler;
    l->handleStretchGesture = &defaultStretchEventHandler;
    l->handleRotateGesture = &defaultRotateEventHandler;
}