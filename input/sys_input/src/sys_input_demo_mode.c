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
  MPLAB Harmony Graphics Composer Generated Implementation File

  File Name:
    libaria_demo_mode.c

  Summary:
    Build-time generated implementation of the graphics demo mode.

  Description:
    Build-time generated implementation of the graphics demo mode.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/
#include "definitions.h"

#include "system/time/sys_time.h"
#include "system/input/sys_input.h"
#include "system/input/sys_input_demo_mode.h"

#include "app.h"

typedef enum
{
    DEMO_TASK_INIT = 0,
    DEMO_INIT,
    DEMO_IDLE,
    DEMO_STARTING,
    DEMO_RUNNING,
    DEMO_RESTARTING,
    DEMO_STOPPING,
    DEMO_STOPPED,
} SYS_INP_DEMO_MODE_STATE;

typedef struct
{
    SYS_INP_DEMO_MODE_STATE state;
    uint32_t prevEventTick;
    volatile uint32_t demoEventFlags;
    uint32_t numEvents;
    int32_t pendingEvent;
    SYS_TIME_HANDLE demoRunTimer;
    SYS_TIME_HANDLE demoTimeoutTimer;
    const SYS_INP_DEMO_EVENT_t * demoEvents;    
    SYS_INP_Start_Demo_Callback startCB;
} SYS_INP_DEMO_MODE_t;

static SYS_INP_DEMO_MODE_t demoMode = 
{
    .state = DEMO_TASK_INIT,
    .numEvents = 0,
    .demoEvents = NULL,
    .startCB = NULL
};

static SYS_INP_InputListener inputListener;
static unsigned int demoModeTimeElapsedSecs = 0;

static void SYS_INP_RestartDemoModeTimerCallback(uintptr_t context)
{
    SYS_INP_DemoModeSendEvent(DEMO_EVENT_START);
}
static void SYS_INP_DemoModeRunTimerCallback(uintptr_t context)
{
    SYS_INP_DemoModeSendEvent(DEMO_EVENT_NEXT_EVENT);
}

static void SYS_INP_DemoModeStartTimerCallback (uintptr_t context)
{
    demoModeTimeElapsedSecs++;
    
    if (demoModeTimeElapsedSecs > DEMO_IDLE_TIMEOUT_S)
    {
        SYS_INP_DemoModeSendEvent(DEMO_EVENT_START);
        demoModeTimeElapsedSecs = 0;
    }
}

void SYS_INP_DemoModeSendEvent(uint32_t event)
{
    demoMode.demoEventFlags |= (event);
}

void SYS_INP_DemoModeSetAppCallback(SYS_INP_Start_Demo_Callback cb)
{
    if (cb == NULL)
        return;
    
    demoMode.startCB = cb;
}

void SYS_INP_DemoModeSetEvents(const SYS_INP_DEMO_EVENT_t * events, unsigned int numEvents)
{
    if (events == NULL || numEvents == 0)
        return;
    
    demoMode.numEvents = numEvents;
    demoMode.demoEvents = events;
}

void SYS_INP_DemoModeTouchDownHandler(const SYS_INP_TouchStateEvent* const evt)
{
    SYS_INP_DemoModeSendEvent(DEMO_EVENT_INPUT);
}

void SYS_INP_DemoModeTouchUpHandler(const SYS_INP_TouchStateEvent* const evt)
{
    SYS_INP_DemoModeSendEvent(DEMO_EVENT_INPUT);
}

void SYS_INP_DemoModeTouchMoveHandler(const SYS_INP_TouchMoveEvent* const evt)
{
    SYS_INP_DemoModeSendEvent(DEMO_EVENT_INPUT);
}

void SYS_INP_DemoModeProcessEvents(void)
{
    switch (demoMode.state) 
    {
        case DEMO_INIT:
        {
            demoMode.numEvents = 0; 
            demoMode.pendingEvent = 0;
            demoMode.demoEventFlags = 0;
            demoMode.demoRunTimer = SYS_TIME_HANDLE_INVALID;
            demoMode.demoTimeoutTimer = SYS_TIME_HANDLE_INVALID;

            demoMode.state = DEMO_IDLE;
            
            //Start timeout timer
            demoMode.demoTimeoutTimer = SYS_TIME_CallbackRegisterMS
                                              (SYS_INP_DemoModeStartTimerCallback,
                                              (uintptr_t) & demoMode,
                                              1000,
                                              SYS_TIME_PERIODIC);
            break;
        }
        case DEMO_IDLE:
        {
            if (demoMode.demoEventFlags & DEMO_EVENT_START)
            {
                // idle timeout timer triggered, start demo
                demoMode.demoEventFlags &= ~DEMO_EVENT_START;
                demoMode.state = DEMO_STARTING;
            }
            
            if (demoMode.demoEventFlags & DEMO_EVENT_INPUT)
            {
                demoMode.demoEventFlags &= ~DEMO_EVENT_INPUT;
                
                //Reset the idle timeout counter
                demoModeTimeElapsedSecs = 0;
                
                demoMode.state = DEMO_IDLE;
            }
            
            if (demoMode.demoEventFlags & DEMO_EVENT_STOP)
            {
                demoMode.demoEventFlags &= ~DEMO_EVENT_STOP;
                demoMode.state = DEMO_STOPPED;
            }
            break;
        }
        case DEMO_STARTING:
        {
            SYS_INP_DEMO_EVENT_t currEvent;
            
            if (demoMode.startCB != NULL)
                demoMode.startCB();

            // START Demo Mode
            if (demoMode.numEvents > 0) 
            {
                demoMode.pendingEvent = 0;
                
                currEvent = demoMode.demoEvents[0];

                if (demoMode.demoRunTimer != SYS_TIME_HANDLE_INVALID)
                    SYS_TIME_TimerDestroy(demoMode.demoRunTimer);
                
                demoMode.demoRunTimer = SYS_TIME_CallbackRegisterMS
                                                (SYS_INP_DemoModeRunTimerCallback,
                                                (uintptr_t) & demoMode,
                                                currEvent.delayMSECS,
                                                SYS_TIME_SINGLE);
                
                demoMode.state = DEMO_RUNNING;
            }
            else
            {
                demoMode.state = DEMO_IDLE;
            }

            break;
        }
        case DEMO_STOPPING:
        {
            if (demoMode.demoTimeoutTimer != SYS_TIME_HANDLE_INVALID)
                SYS_TIME_TimerDestroy(demoMode.demoTimeoutTimer);

            if (demoMode.demoRunTimer != SYS_TIME_HANDLE_INVALID)
                SYS_TIME_TimerDestroy(demoMode.demoRunTimer);

            demoMode.demoTimeoutTimer = SYS_TIME_HANDLE_INVALID;
            demoMode.demoRunTimer = SYS_TIME_HANDLE_INVALID;
            
            demoMode.state = DEMO_STOPPED;
        }
        case DEMO_STOPPED:
        {
            if (demoMode.demoEventFlags & DEMO_EVENT_RESTART)
            {
                // received start event
                demoMode.demoEventFlags &= ~DEMO_EVENT_RESTART;
                
                demoMode.state = DEMO_INIT;
            }
            break;
        }
        case DEMO_RUNNING:
        {
            if (demoMode.demoEventFlags & DEMO_EVENT_INPUT)
            {
                // Clear all events
                demoMode.demoEventFlags = 0;

                //Reset the timeout counter
                demoModeTimeElapsedSecs = 0;
                
                demoMode.state = DEMO_IDLE;
            }

            if (demoMode.demoEventFlags & DEMO_EVENT_STOP)
            {
                demoMode.demoEventFlags = 0;
                demoMode.state = DEMO_STOPPING;
                break;
            }
            
            if (demoMode.demoEventFlags & DEMO_EVENT_NEXT_EVENT)
            {
                SYS_INP_DEMO_EVENT_t currEvent, nextEvent;
                
                demoMode.demoEventFlags &= ~DEMO_EVENT_NEXT_EVENT;

                currEvent = demoMode.demoEvents[demoMode.pendingEvent];

                switch (currEvent.touchEvent) 
                {
                    case DEMO_MODE_INPUT_PRESS:
                        leInput_InjectTouchDown(currEvent.index, currEvent.x, currEvent.y);
                        break;
                    case DEMO_MODE_INPUT_RELEASE:
                        leInput_InjectTouchUp(currEvent.index, currEvent.x, currEvent.y);
                        break;
                    case DEMO_MODE_INPUT_MOVE:
                        leInput_InjectTouchMoved(currEvent.index, currEvent.x, currEvent.y);
                        break;
                    default:
                        break;
                }

                demoMode.pendingEvent++;

                // End of demo events, restart
                if (demoMode.pendingEvent >= demoMode.numEvents) 
                {
                    demoMode.state = DEMO_RESTARTING;

                    if (demoMode.demoRunTimer != SYS_TIME_HANDLE_INVALID)
                        SYS_TIME_TimerDestroy(demoMode.demoRunTimer);

                    demoMode.demoRunTimer = SYS_TIME_CallbackRegisterMS
                                                (SYS_INP_RestartDemoModeTimerCallback,
                                                (uintptr_t) & demoMode,
                                                (DEMO_REPEAT_TIMEOUT_S*1000),
                                                SYS_TIME_SINGLE);   
                }
                else 
                {
                    nextEvent = demoMode.demoEvents[demoMode.pendingEvent];

                    if (demoMode.demoRunTimer != SYS_TIME_HANDLE_INVALID)
                         SYS_TIME_TimerDestroy(demoMode.demoRunTimer);
                    
                    demoMode.demoRunTimer = SYS_TIME_CallbackRegisterMS
                                                (SYS_INP_DemoModeRunTimerCallback,
                                                (uintptr_t) & demoMode,
                                                nextEvent.delayMSECS,
                                                SYS_TIME_SINGLE); 
                    
                    demoMode.state = DEMO_RUNNING;
                }
            }
            break;
        }
        case DEMO_RESTARTING:
        {
            if (demoMode.demoEventFlags & DEMO_EVENT_START)
            {
                demoMode.demoEventFlags &= ~DEMO_EVENT_START;

                demoMode.state = DEMO_STARTING;
            }

            if (demoMode.demoEventFlags & DEMO_EVENT_INPUT)
            {
                // Clear all events
                demoMode.demoEventFlags = 0;

                //Restart the idle timeout timer
                if (demoMode.demoRunTimer != SYS_TIME_HANDLE_INVALID)
                    SYS_TIME_TimerDestroy(demoMode.demoRunTimer);

                demoMode.demoRunTimer = SYS_TIME_HANDLE_INVALID;

                //Reset the timeout counter
                demoModeTimeElapsedSecs = 0;

                demoMode.state = DEMO_IDLE;
            }

            break;
        }
        //One time demo mode task initialization
        case DEMO_TASK_INIT:
        {
            
            SYS_INP_ListenerInit(&inputListener);

            inputListener.handleTouchDown = &SYS_INP_DemoModeTouchDownHandler;
            inputListener.handleTouchUp = &SYS_INP_DemoModeTouchUpHandler;
            inputListener.handleTouchMove = &SYS_INP_DemoModeTouchMoveHandler;

            SYS_INP_AddListener(&inputListener);
            
            demoMode.state = DEMO_INIT;

            break;
        }
        default:
        {
            break;
        }
    }
}
