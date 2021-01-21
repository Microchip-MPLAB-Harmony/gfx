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
  MPLAB Harmony Graphics Composer Generated Definitions Header

  File Name:
    libaria_demo_mode.h

  Summary:
    Build-time generated definitions header of the graphics demo mode.

  Description:
    Build-time generated definitions header of the graphics demo mode.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/


#ifndef _LIBARIA_DEMO_MODE_H    /* Guard against multiple inclusion */
#define _LIBARIA_DEMO_MODE_H

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    DEMO_MODE_INPUT_INVALID   = 0,            // Invalid event.
    DEMO_MODE_INPUT_MOVE,                     // Move event.
    DEMO_MODE_INPUT_PRESS,                    // Press event.
    DEMO_MODE_INPUT_STILLPRESS,               // Continuous press event.
    DEMO_MODE_INPUT_RELEASE,                  // Release event.
    DEMO_MODE_INPUT_KEYSCAN,                  // Key scan event, parameters for the object
                                                    // ID and keyboard scan code will be sent
                                                    // with this event in the GFX_GOL_MESSAGE
                                                    // as parameter.
    DEMO_MODE_INPUT_CHARCODE,                 // Character code event. The actual
                                                    // character code will be sent with this
                                                    // event in the GFX_GOL_MESSAGE as
                                                    // parameter.
    DEMO_MODE_INPUT_SET,                      // Generic set event.
    DEMO_MODE_INPUT_SET_STATE,                // Generic set state event.
    DEMO_MODE_INPUT_CLR_STATE                 // Generic clear state event.
} DEMO_MODE_INPUT_TYPE;

#define DEMO_EVENT_START        (1 << 0)
#define DEMO_EVENT_NEXT_EVENT   (1 << 1)
#define DEMO_EVENT_STOP         (1 << 2)
#define DEMO_EVENT_INPUT        (1 << 3)
#define DEMO_EVENT_RESTART      (1 << 4)
    
#define MAX_DEMO_EVENTS ${demoModeMaxEvents}
<#if demoModeRecordInput == true>
#define RECORD_TICK_PERIOD_MS ${demoModeRecordTickPeriod}
</#if>
#define DEMO_IDLE_TIMEOUT_S ${demoModeIdleTimeout}
#define DEMO_REPEAT_TIMEOUT_S ${demoModeReplayDelay}

/** 
  @Function
    void LibAria_DemoModeSendEvent(uint32_t event)

  @Summary
    This function sets an event flag to the demo mode process.

  @Parameters
    @param event        event flag bit to set
 
  @Returns
    None
*/
void LibAria_DemoModeSendEvent(uint32_t event);

/** 
  @Function
    void LibAria_DemoModeAddInputEvent(uint32_t dt,
                                       DEMO_MODE_INPUT_TYPE te,
                                   int32_t x,
                                   int32_t y);

  @Summary
    This function adds an input event to the queue.

  @Parameters
    @param dt_ms        delay in ms
    @param te           input event to add
    @param x            x value of input
    @param y            y value of input

  @Returns
    None
*/
void LibAria_DemoModeAddInputEvent(uint32_t dt_ms,
                                   DEMO_MODE_INPUT_TYPE te,
                                   int32_t index,
                                   int32_t x,
                                   int32_t y);

/** 
  @Function
    void LibAria_DemoModeRecordInputEvent(DEMO_MODE_INPUT_TYPE te,
                                          int32_t index,
                                          int32_t x,
                                          int32_t y);

  @Summary
    Records an input event. Called in the Aria event input handler.

  @Parameters
    @param te           input event to record
    @param index        the index of the event
    @param x            x value of input
    @param y            y value of input

  @Returns
    None
*/
void LibAria_DemoModeRecordInputEvent(DEMO_MODE_INPUT_TYPE te,
                                      int32_t index,
                                      int32_t x,
                                      int32_t y);

/** 
  @Function
    void LibAria_DemoModeProcessEvents(void)

  @Summary
    This function is called in the Aria task to process the demo events.

  @Parameters
    None

  @Returns
    None
*/
void LibAria_DemoModeProcessEvents(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _LIBARIA_DEMO_MODE_H */

/* *****************************************************************************
 End of File
 */
