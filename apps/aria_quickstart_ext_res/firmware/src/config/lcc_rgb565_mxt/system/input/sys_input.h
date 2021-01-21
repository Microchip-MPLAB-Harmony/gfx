// DOM-IGNORE-BEGIN
/*******************************************************************************
 Module for Microchip Harmony Input System Service

  Company:
    Microchip Technology Inc.

  File Name:
    sys_input.h

  Summary:
    MPLAB Harmony Input System Service main header file

  Description:
    MPLAB Harmony Input System Service main header file
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


#ifndef SYS_INPUT_H
#define SYS_INPUT_H
//DOM-IGNORE-END

#include <stdint.h>
#include <stddef.h>

#include "sys_input_keyboard.h"
#include "sys_input_listener.h"
#include "sys_input_mouse.h"
#include "sys_input_touch.h"

// *****************************************************************************
/* Structure:
    SYS_INPUT_Init

  Summary:
    Input system service initialization struct
    
  Remarks:
    Only contains a placeholder for now since there are no configuration options
    as of yet
*/
typedef struct SYS_INPUT_Init
{
    uint32_t placeholder;
} SYS_INPUT_Init;

// *****************************************************************************
/* Function:
    int32_t SYS_INP_Init(void);

  Summary:
    Initializes the hardware abstraction layer global state.  Should be called 
    before any other HAL function. 
    
  Returns:
    int32_t - Non-zero value indicates an error during initialization
*/
int32_t SYS_INP_Init();

// *****************************************************************************
/* Function:
    void SYS_INP_Tasks(void);

  Summary:
    Maintains the state of the Input System Service.  During this phase input
    events may be broadcast to registered listeners
*/
void SYS_INP_Tasks();

// *****************************************************************************
/* Function:
    int32_t SYS_INP_AddListener(SYS_INP_InputListener* lst);

  Summary:
    Registers an input event listener with the service
    
  Returns:
    int32_t - The array index of the listener in the listener array.  Used for
              removing the listener at a future point.
*/
int32_t SYS_INP_AddListener(SYS_INP_InputListener* lst);

// *****************************************************************************
/* Function:
    int32_t SYS_INP_RemoveListener(uint16_t idx);

  Summary:
    Unregisters an input event listener from the service
    
  Parameters:
    idx - the listener index to remove
    
  Returns:
    int32_t - Non-zero value indicates an error during registeration
*/
int32_t SYS_INP_RemoveListener(uint16_t idx);

// *****************************************************************************
/* Function:
    int32_t SYS_INP_InjectKeyDown(SYS_INP_Key key);

  Summary:
    Injects a key down event into the system.  This event will then be broadcast
    to any registered listeners during a later task service event    
    
  Parameters:
    key - the key value that was pressed
    
  Returns:
    int32_t - Non-zero value indicates an error
*/
int32_t SYS_INP_InjectKeyDown(SYS_INP_Key key);

// *****************************************************************************
/* Function:
    int32_t SYS_INP_InjectKeyUp(SYS_INP_Key key);

  Summary:
    Injects a key up event into the system.  This event will then be broadcast
    to any registered listeners during a later task service event    
    
  Parameters:
    key - the key value that was released
    
  Returns:
    int32_t - Non-zero value indicates an error
*/
int32_t SYS_INP_InjectKeyUp(SYS_INP_Key key);

// *****************************************************************************
/* Function:
    SYS_INP_InjectMouseButtonDown(SYS_INP_MouseButton btn);

  Summary:
    Injects a mouse button down event into the system.  This event will then be broadcast
    to any registered listeners during a later task service event    
    
  Parameters:
    btn - the button value that was pressed
    
  Returns:
    int32_t - Non-zero value indicates an error
*/
int32_t SYS_INP_InjectMouseButtonDown(SYS_INP_MouseButton btn);

// *****************************************************************************
/* Function:
    SYS_INP_InjectMouseButtonUp(SYS_INP_MouseButton btn);

  Summary:
    Injects a mouse button up event into the system.  This event will then be broadcast
    to any registered listeners during a later task service event    
    
  Parameters:
    btn - the button value that was released
    
  Returns:
    int32_t - Non-zero value indicates an error
*/
int32_t SYS_INP_InjectMouseButtonUp(SYS_INP_MouseButton btn);

// *****************************************************************************
/* Function:
    SYS_INP_InjectMouseMove(uint16_t x, uint16_t y);

  Summary:
    Injects a mouse move event into the system.  This event will then be broadcast
    to any registered listeners during a later task service event    
    
  Parameters:
    x - the number of pixels moved in x
    y - the number of pixels moved in y
    
  Returns:
    int32_t - Non-zero value indicates an error
*/
int32_t SYS_INP_InjectMouseMove(uint16_t x, uint16_t y);

// *****************************************************************************
/* Function:
    SYS_INP_InjectTouchDown(uint16_t idx, uint16_t x, uint16_t y);

  Summary:
    Injects a touch down event into the system.  This event will then be broadcast
    to any registered listeners during a later task service event    
    
  Parameters:
    idx - the touch index of the event
    x - the x coordinate component of the event
    y - the y coordinate component of the event
    
  Returns:
    int32_t - Non-zero value indicates an error
*/
int32_t SYS_INP_InjectTouchDown(uint16_t idx, uint16_t x, uint16_t y);

// *****************************************************************************
/* Function:
    SYS_INP_InjectTouchUp(uint16_t idx, uint16_t x, uint16_t y);

  Summary:
    Injects a touch up event into the system.  This event will then be broadcast
    to any registered listeners during a later task service event    
    
  Parameters:
    idx - the touch index of the event
    x - the x coordinate component of the event
    y - the y coordinate component of the event
    
  Returns:
    int32_t - Non-zero value indicates an error
*/
int32_t SYS_INP_InjectTouchUp(uint16_t idx, uint16_t x, uint16_t y);

// *****************************************************************************
/* Function:
    SYS_INP_InjectTouchMove(uint16_t idx, uint16_t x, uint16_t y);

  Summary:
    Injects a touch moved event into the system.  This event will then be broadcast
    to any registered listeners during a later task service event    
    
  Parameters:
    idx - the touch index of the event
    x - the number of pixels moved in x
    y - the number of pixels moved in y
    
  Returns:
    int32_t - Non-zero value indicates an error
*/
int32_t SYS_INP_InjectTouchMove(uint16_t idx, uint16_t x, uint16_t y);

// *****************************************************************************
/* Function:
    int32_t SYS_INP_InjectFlickGesture(uint16_t x,
                                       uint16_t y,
                                       uint16_t dir,
                                       uint16_t dist);

  Summary:
    Injects a flick gesture into the system.  This event will then be broadcast
    to any registered listeners during a later task service event
    
  Parameters:
    x - the starting x coordinate
    y - the starting y coordinate
    dir - the direction of the flick
    dist - the distance/size of the flick
    
  Returns:
    int32_t - Non-zero value indicates an error during registeration
    
  Remarks:
    These parameters are based on the MaxTouch MXT336T hardware spec.  This API
    may change in the future to accomodate more information.
*/
int32_t SYS_INP_InjectFlickGesture(uint16_t x,
                                   uint16_t y,
                                   uint16_t dir,
                                   uint16_t dist);

// *****************************************************************************
/* Function:
    int32_t SYS_INP_InjectPinchGesture(uint16_t x,
                                       uint16_t y,
                                       uint16_t angle,
                                       uint16_t sep);

  Summary:
    Injects a pinch gesture into the system.  This event will then be broadcast
    to any registered listeners during a later task service event
    
  Parameters:
    x - the pinch x coordinate
    y - the pinch y coordinate
    angle - the angle of the pinch
    sep - the distance between the two pinch points
    
  Returns:
    int32_t - Non-zero value indicates an error during registeration
    
  Remarks:
    These parameters are based on the MaxTouch MXT336T hardware spec.  This API
    may change in the future to accomodate more information.
*/
int32_t SYS_INP_InjectPinchGesture(uint16_t x,
                                   uint16_t y,
                                   uint16_t angle,
                                   uint16_t sep);

                                   
// *****************************************************************************
/* Function:
    int32_t SYS_INP_InjectStretchGesture(uint16_t x,
                                         uint16_t y,
                                         uint16_t angle,
                                         uint16_t sep);

  Summary:
    Injects a stretch gesture into the system.  This event will then be broadcast
    to any registered listeners during a later task service event
    
  Parameters:
    x - the starting x coordinate
    y - the starting y coordinate
    angle - the angle of the pinch
    sep - the distance between the two points
    
  Returns:
    int32_t - Non-zero value indicates an error during registeration
    
  Remarks:
    These parameters are based on the MaxTouch MXT336T hardware spec.  This API
    may change in the future to accomodate more information.
*/
int32_t SYS_INP_InjectStretchGesture(uint16_t x,
                                     uint16_t y,
                                     uint16_t angle,
                                     uint16_t sep);

// *****************************************************************************
/* Function:
    int32_t SYS_INP_InjectRotateGesture(uint16_t x,
                                        uint16_t y,
                                        uint16_t angle,
                                        uint16_t sep,
                                        uint16_t dir);

  Summary:
    Injects a rotate gesture into the system.  This event will then be broadcast
    to any registered listeners during a later task service event
    
  Parameters:
    x - the starting x coordinate
    y - the starting y coordinate
    angle - the angle of the pinch
    sep - the distance between the two points
    dir - the direction of rotation
    
  Returns:
    int32_t - Non-zero value indicates an error during registeration
    
  Remarks:
    These parameters are based on the MaxTouch MXT336T hardware spec.  This API
    may change in the future to accomodate more information.
*/
int32_t SYS_INP_InjectRotateGesture(uint16_t x,
                                    uint16_t y,
                                    uint16_t angle,
                                    uint16_t sep,
                                    uint16_t dir);

#endif /* SYS_INPUT_H */