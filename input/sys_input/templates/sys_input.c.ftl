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


<#if RTOSEnabled == true>
#include "osal/osal.h"
</#if>
#include "system/input/sys_input.h"
<#if enableDemoMode == true>
#include "system/input/sys_input_demo_mode.h"
</#if>

#include <string.h>

#define SYS_INP_MAX_LISTENERS        ${MaxListeners}  // max number of listeners
#define SYS_INP_MAX_GENERAL_EVENTS   ${MaxEvents} // max number of pending events

<#if TransformTouchCoords == true>

#define SYS_INP_DISP_WIDTH           ${TransformTouchCoords_DisplayW} // display width in pixels
#define SYS_INP_DISP_HEIGHT          ${TransformTouchCoords_DisplayH} // display height in pixels
#define SYS_INP_TOUCH_WIDTH          ${TransformTouchCoords_TouchW} // touch panel width in pixels
#define SYS_INP_TOUCH_HEIGHT         ${TransformTouchCoords_TouchH} // touch panel height in pixels

static const float touchInvWidth  = 1.0f/(float)(SYS_INP_TOUCH_WIDTH-1);
static const float touchInvHeight = 1.0f/(float)(SYS_INP_TOUCH_HEIGHT-1);

</#if>

// event type values
enum
{
    KEY_DOWN_EVENT = 0,
    KEY_UP_EVENT,
    MOUSE_MOVE_EVENT,
    MOUSE_DOWN_EVENT,
    MOUSE_UP_EVENT,
    TOUCH_MOVE_EVENT,
    TOUCH_DOWN_EVENT,
    TOUCH_UP_EVENT,
    GESTURE_FLICK_EVENT,
    GESTURE_PINCH_EVENT,
    GESTURE_STRETCH_EVENT,
    GESTURE_ROTATE_EVENT,
    GESTURE_GENERIC_EVENT,
};

// generic struct for storing input events
typedef struct InputEvent_t
{
    uint8_t type;
    union
    {
        SYS_INP_KeyEvent key;
        SYS_INP_MouseButtonEvent mouseBtn;
        SYS_INP_MouseMoveEvent mouseMove;
        SYS_INP_TouchStateEvent touchState;
        SYS_INP_TouchMoveEvent touchMove;
        SYS_INP_FlickGestureEvent flickGesture;
        SYS_INP_PinchGestureEvent pinchGesture;
        SYS_INP_StretchGestureEvent stretchGesture;
        SYS_INP_RotateGestureEvent rotateGesture;
        SYS_INP_GenericGestureEvent genericGesture;
    } event;
} InputEvent;

// the event queue
InputEvent generalEvents[SYS_INP_MAX_GENERAL_EVENTS];
uint32_t eventCount;

// will be used later for software-based gesture support
/*InputEvent touchEvents[SYS_INP_MULTITOUCH_COUNT][SYS_INP_MAX_TOUCH_EVENTS];
uint32_t touchEventCount;
uint32_t touchNextEvent;*/

// the listener array
SYS_INP_InputListener listeners[SYS_INP_MAX_LISTENERS];
uint8_t listenerFlags[SYS_INP_MAX_LISTENERS];

<#if RTOSEnabled == true>
OSAL_MUTEX_HANDLE_TYPE listenersLock;
OSAL_MUTEX_HANDLE_TYPE eventsLock;
</#if>

/*static int32_t _insertIntoEventQueue(InputEvent* evt)
{
    if(nextEvent == eventCount)
        return -1;
    
    generalEvents[nextEvent] = *evt;
    nextEvent++;
    
    return 0;
}*/

int32_t SYS_INP_Init(void)
{
    eventCount = 0;

<#if RTOSEnabled == true>
    OSAL_MUTEX_Create(&eventsLock);
    OSAL_MUTEX_Create(&listenersLock);
</#if>
    
    memset(generalEvents, 0, sizeof(generalEvents));
    //memset(touchEvents, 0, sizeof(touchEvents));
    memset(listeners, 0, sizeof(listeners));
    
    return 0;
}

void SYS_INP_Tasks(void)
{
    uint32_t i, j;

<#if enableDemoMode == true>
    SYS_INP_DemoModeProcessEvents();
</#if>

    // no events?  nothing to do
    if(eventCount == 0)
        return;

<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // iterate over all listeners
    for(j = 0; j < SYS_INP_MAX_LISTENERS; j++)
    {
        if(listenerFlags[j] == 0)
            continue;
        
        // iterate over all pending events and broadcast
        for(i = 0; i < eventCount; i++)
        {
            switch(generalEvents[i].type)
            {
                case KEY_DOWN_EVENT:
                {
                    if (listeners[j].handleKeyDown != NULL)
                        listeners[j].handleKeyDown((const SYS_INP_KeyEvent* const)&generalEvents[i].event);
                    break;
                }
                case KEY_UP_EVENT:
                {
                    if (listeners[j].handleKeyUp != NULL)
                        listeners[j].handleKeyUp((const SYS_INP_KeyEvent* const)&generalEvents[i].event);
                    break;
                }
                case MOUSE_MOVE_EVENT:
                {
                    if (listeners[j].handleMouseMove != NULL)
                        listeners[j].handleMouseMove((const SYS_INP_MouseMoveEvent* const)&generalEvents[i].event);
                    break;
                }
                case MOUSE_DOWN_EVENT:
                {
                    if (listeners[j].handleMouseButtonDown != NULL)
                        listeners[j].handleMouseButtonDown((const SYS_INP_MouseButtonEvent* const)&generalEvents[i].event);
                    break;
                }
                case MOUSE_UP_EVENT:
                {
                    if (listeners[j].handleMouseButtonUp != NULL)
                        listeners[j].handleMouseButtonUp((const SYS_INP_MouseButtonEvent* const)&generalEvents[i].event);
                    break;
                }
                case TOUCH_MOVE_EVENT:
                {
                    if (listeners[j].handleTouchMove != NULL)
                        listeners[j].handleTouchMove((const SYS_INP_TouchMoveEvent* const)&generalEvents[i].event);
                    break;
                }
                case TOUCH_DOWN_EVENT:
                {
                    if (listeners[j].handleTouchDown != NULL)
                        listeners[j].handleTouchDown((const SYS_INP_TouchStateEvent* const)&generalEvents[i].event);
                    break;
                }
                case TOUCH_UP_EVENT:
                {
                    if (listeners[j].handleTouchUp != NULL)
                        listeners[j].handleTouchUp((const SYS_INP_TouchStateEvent* const)&generalEvents[i].event);
                    break;
                }
                case GESTURE_FLICK_EVENT:
                {
                    if (listeners[j].handleFlickGesture != NULL)
                        listeners[j].handleFlickGesture((const SYS_INP_FlickGestureEvent* const)&generalEvents[i].event);
                    break;
                }
                case GESTURE_PINCH_EVENT:
                {
                    if (listeners[j].handlePinchGesture != NULL)
                        listeners[j].handlePinchGesture((const SYS_INP_PinchGestureEvent* const)&generalEvents[i].event);
                    break;
                }
                case GESTURE_STRETCH_EVENT:
                {
                    if (listeners[j].handleStretchGesture != NULL)
                        listeners[j].handleStretchGesture((const SYS_INP_StretchGestureEvent* const)&generalEvents[i].event);
                    break;
                }
                case GESTURE_ROTATE_EVENT:
                {
                    if (listeners[j].handleRotateGesture != NULL)
                        listeners[j].handleRotateGesture((const SYS_INP_RotateGestureEvent* const)&generalEvents[i].event);
                    break;
                }
                case GESTURE_GENERIC_EVENT:
                {
                    if (listeners[j].handleGenericGesture != NULL)
                        listeners[j].handleGenericGesture((const SYS_INP_GenericGestureEvent* const)&generalEvents[i].event);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
    
    eventCount = 0;

<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>
    
}

int32_t SYS_INP_AddListener(SYS_INP_InputListener* lst)
{
    int32_t i;

<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&listenersLock, OSAL_WAIT_FOREVER);
</#if>
    
    // find the next available listener slot
    for(i = 0; i < SYS_INP_MAX_LISTENERS; i++)
    {
        if(listenerFlags[i] == 0)
        {
            listeners[i] = *lst;
            listenerFlags[i] = 1;
            
<#if RTOSEnabled == true>
            OSAL_MUTEX_Unlock(&listenersLock);
</#if>
            return i;
        }
    }
    
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&listenersLock);
</#if>
    
    return -1;
}

int32_t SYS_INP_RemoveListener(uint16_t idx)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&listenersLock, OSAL_WAIT_FOREVER);
</#if>
    
    if(idx >= SYS_INP_MAX_LISTENERS || listenerFlags[idx] == 0)
    {
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&listenersLock);
</#if>
        return -1;
    }
    
    // clear the indicated array index
    listenerFlags[idx] = 0;
    
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&listenersLock);
</#if>
    return 0;
}

int32_t SYS_INP_InjectKeyDown(SYS_INP_Key key)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
    generalEvents[eventCount].type = KEY_DOWN_EVENT;
    generalEvents[eventCount].event.key.key = key;

    eventCount++;

<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>

    return 0;
}

int32_t SYS_INP_InjectKeyUp(SYS_INP_Key key)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&listenersLock);
</#if>
        return -1;
    }
    
    generalEvents[eventCount].type = KEY_UP_EVENT;
    generalEvents[eventCount].event.key.key = key;
    
    eventCount++;

<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>
    
    return 0;
}

int32_t SYS_INP_InjectMouseButtonDown(SYS_INP_MouseButton btn)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
    generalEvents[eventCount].type = MOUSE_DOWN_EVENT;
    generalEvents[eventCount].event.mouseBtn.btn = btn;
    
    eventCount++;

<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>
    
    return 0;
}

int32_t SYS_INP_InjectMouseButtonUp(SYS_INP_MouseButton btn)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
    generalEvents[eventCount].type = MOUSE_UP_EVENT;
    generalEvents[eventCount].event.mouseBtn.btn = btn;
    
    eventCount++;

<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>

    return 0;
}

int32_t SYS_INP_InjectMouseMove(uint16_t x, uint16_t y)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
    generalEvents[eventCount].type = MOUSE_MOVE_EVENT;
    generalEvents[eventCount].event.mouseMove.x = x;
    generalEvents[eventCount].event.mouseMove.y = y;
    
    eventCount++;
    
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>
    
    return 0;
}

<#if TransformTouchCoords == true>
// Transform touch inputs based on Input System Service configuration
static void SYS_INP_Transform2DTouchPoint(uint16_t* x, uint16_t* y)
{    
<#if TransformTouchCoords_InvertX == true>
    // X input is inverted
    *x = (SYS_INP_TOUCH_WIDTH-1) - *x;

</#if>
<#if TransformTouchCoords_InvertY == true>
    // Y input is inverted
    *y = (SYS_INP_TOUCH_HEIGHT-1) - *y;

</#if>
<#if TransformTouchCoords_SwapXY == false>
    float fracX = (float)(*x) * touchInvWidth;
    float fracY = (float)(*y) * touchInvHeight;
<#else>
    // X and Y inputs are swapped
    float fracX = (float)(*y) * touchInvHeight;
    float fracY = (float)(*x) * touchInvWidth;
</#if>
    
<#switch TransformTouchCoords_Orientation>
<#case "0">
    // touch panel origin is the same as display origin
    float newX = (SYS_INP_DISP_WIDTH-1)  * fracX;
    float newY = (SYS_INP_DISP_HEIGHT-1) * fracY;
<#break>
<#case "90">
    // touch panel origin is rotated 90 degrees clockwise from display origin
    float newX = (SYS_INP_DISP_WIDTH-1)  * (1.0f - fracY);
    float newY = (SYS_INP_DISP_HEIGHT-1) * fracX;
<#break>
<#case "180">
    // touch panel origin is rotated 180 degrees clockwise from display origin
    float newX = (SYS_INP_DISP_WIDTH-1)  * (1.0f - fracX);
    float newY = (SYS_INP_DISP_HEIGHT-1) * (1.0f - fracY);
<#break>
<#case "270">
    // touch panel origin is rotated 270 degrees clockwise from display origin
    float newX = (SYS_INP_DISP_WIDTH-1)  * fracY;
    float newY = (SYS_INP_DISP_HEIGHT-1) * (1.0f - fracX);
<#break>
<#default>
    // touch panel origin is the same as display origin
    float newX = (SYS_INP_DISP_WIDTH-1)  * fracX;
    float newY = (SYS_INP_DISP_HEIGHT-1) * fracY;
</#switch>
    
    *x = (uint16_t)(newX + 0.5f); // round X to nearest integer
    *y = (uint16_t)(newY + 0.5f); // round Y to nearest integer
}
</#if>

int32_t SYS_INP_InjectTouchDown(uint16_t idx, uint16_t x, uint16_t y)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
<#if TransformTouchCoords == true>
    SYS_INP_Transform2DTouchPoint(&x, &y);
</#if>
    generalEvents[eventCount].type = TOUCH_DOWN_EVENT;
    generalEvents[eventCount].event.touchState.index = idx;
    generalEvents[eventCount].event.touchState.x = x;
    generalEvents[eventCount].event.touchState.y = y;
    
    eventCount++;
    
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>
    
    return 0;
}

int32_t SYS_INP_InjectTouchUp(uint16_t idx, uint16_t x, uint16_t y)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
<#if TransformTouchCoords == true>
    SYS_INP_Transform2DTouchPoint(&x, &y);
</#if>
    generalEvents[eventCount].type = TOUCH_UP_EVENT;
    generalEvents[eventCount].event.touchState.index = idx;
    generalEvents[eventCount].event.touchState.x = x;
    generalEvents[eventCount].event.touchState.y = y;
    
    eventCount++;
    
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>
    
    return 0;
}

int32_t SYS_INP_InjectTouchMove(uint16_t idx, uint16_t x, uint16_t y)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
<#if TransformTouchCoords == true>
    SYS_INP_Transform2DTouchPoint(&x, &y);
</#if>
    generalEvents[eventCount].type = TOUCH_MOVE_EVENT;
    generalEvents[eventCount].event.touchMove.index = idx;
    generalEvents[eventCount].event.touchMove.x = x;
    generalEvents[eventCount].event.touchMove.y = y;
    
    eventCount++;
    
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>
    
    return 0;
}

int32_t SYS_INP_InjectFlickGesture(uint16_t x,
                                   uint16_t y,
                                   uint16_t dir,
                                   uint16_t dist)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
    generalEvents[eventCount].type = GESTURE_FLICK_EVENT;
    generalEvents[eventCount].event.flickGesture.x = x;
    generalEvents[eventCount].event.flickGesture.y = y;
    generalEvents[eventCount].event.flickGesture.dir = dir;
    generalEvents[eventCount].event.flickGesture.dist = dist;
    
    eventCount++;

<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>
    
    return 0;
}

int32_t SYS_INP_InjectPinchGesture(uint16_t x,
                                   uint16_t y,
                                   uint16_t angle,
                                   uint16_t sep)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
    
    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
    generalEvents[eventCount].type = GESTURE_PINCH_EVENT;
    generalEvents[eventCount].event.pinchGesture.x = x;
    generalEvents[eventCount].event.pinchGesture.y = y;
    generalEvents[eventCount].event.pinchGesture.angle = angle;
    generalEvents[eventCount].event.pinchGesture.sep = sep;
    
    eventCount++;
    
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>

    return 0;
}

int32_t SYS_INP_InjectStretchGesture(uint16_t x,
                                     uint16_t y,
                                     uint16_t angle,
                                     uint16_t sep)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>

    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
    generalEvents[eventCount].type = GESTURE_STRETCH_EVENT;
    generalEvents[eventCount].event.stretchGesture.x = x;
    generalEvents[eventCount].event.stretchGesture.y = y;
    generalEvents[eventCount].event.stretchGesture.angle = angle;
    generalEvents[eventCount].event.stretchGesture.sep = sep;
    
    eventCount++;
    
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>

    return 0;
}

int32_t SYS_INP_InjectRotateGesture(uint16_t x,
                                    uint16_t y,
                                    uint16_t angle,
                                    uint16_t sep,
                                    uint16_t dir)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>

    // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>
        return -1;
    }
    
    generalEvents[eventCount].type = GESTURE_STRETCH_EVENT;
    generalEvents[eventCount].event.rotateGesture.x = x;
    generalEvents[eventCount].event.rotateGesture.y = y;
    generalEvents[eventCount].event.rotateGesture.angle = angle;
    generalEvents[eventCount].event.rotateGesture.sep = sep;
    generalEvents[eventCount].event.rotateGesture.dir = dir;
    
    eventCount++;
    
<#if RTOSEnabled == true>
    OSAL_MUTEX_Unlock(&eventsLock);
</#if>
    
    return 0;
}

int32_t SYS_INP_InjectGenericGesture(uint16_t gest,
                                     uint16_t x,
                                     uint16_t y,
                                     void * parm)
{
<#if RTOSEnabled == true>
    OSAL_MUTEX_Lock(&eventsLock, OSAL_WAIT_FOREVER);
</#if>
        // add the event to the next empty slot
    if(eventCount >= SYS_INP_MAX_GENERAL_EVENTS)
    {
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>	
        return -1;
    }
    
    generalEvents[eventCount].type = GESTURE_GENERIC_EVENT;
    generalEvents[eventCount].event.genericGesture.gest = gest;
    generalEvents[eventCount].event.genericGesture.x = x;
    generalEvents[eventCount].event.genericGesture.y = y;
    generalEvents[eventCount].event.genericGesture.parm = parm;
    
    eventCount++;
	
<#if RTOSEnabled == true>
        OSAL_MUTEX_Unlock(&eventsLock);
</#if>	
    
    return 0;
}
