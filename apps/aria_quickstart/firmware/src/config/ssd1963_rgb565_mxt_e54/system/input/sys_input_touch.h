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


#ifndef SYS_INPUT_TOUCH_H
#define SYS_INPUT_TOUCH_H
//DOM-IGNORE-END

#include <stdint.h>
#include <stddef.h>

// *****************************************************************************
/* Structure:
    SYS_INP_TouchStateEvent

  Summary:
    Defines a generic touch state event

  Description:
    index - the touch index of the event
    x - the x coordinate
    y - the y coordinate
    
  Remarks:
    None.
*/
typedef struct SYS_INP_TouchStateEvent_t
{
    uint8_t index;
    int16_t x;
    int16_t y;
} SYS_INP_TouchStateEvent;

/* Structure:
    SYS_INP_TouchMoveEvent

  Summary:
    Defines a touch move event

  Description:
    index - the touch index of the event
    x - the x coordinate
    y - the y coordinate
    
  Remarks:
    None.
*/
typedef struct SYS_INP_TouchMoveEvent_t
{
    uint8_t index;
    int16_t x;
    int16_t y;
} SYS_INP_TouchMoveEvent;

/* Touch Handler Function Definitions */
typedef void (*SYS_INP_HandleTouchDownEvent)(const SYS_INP_TouchStateEvent* const);
typedef void (*SYS_INP_HandleTouchUpEvent)(const SYS_INP_TouchStateEvent* const);
typedef void (*SYS_INP_HandleTouchMoveEvent)(const SYS_INP_TouchMoveEvent* const);

#endif /* SYS_INPUT_TOUCH_H */