/*******************************************************************************
  MPLAB Harmony Generated Display Driver Header File

  File Name:
    touch_display.h

  Summary:
    Build-time generated header file that
    adds display driver support for the
    AC40T08A DSI LCD Touch Display.
    https://www.microchip.com/en-us/development-tool/ac40t08a

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _TOUCH_DISPLAY_H
#define _TOUCH_DISPLAY_H

#ifdef __cplusplus
    extern "C" {
#endif

/*******************************************************************************
  Display Driver State

  Summary:
    Holds application data.
*/
typedef enum
{
    /* Configure display */
    DISP_STATE_CONFIGURE,

    /* Idle */
    DISP_STATE_IDLE
} DISP_STATES;

/*******************************************************************************
  Display Driver Data

  Summary:
    Holds application data.
*/
typedef struct
{
    /* Current state */
    DISP_STATES  state;
} DISP_DATA;

/*******************************************************************************
  Function:
    void DISP_Initialize(void)

  Summary:
    Initializes the display.
*/
void DISP_Initialize(void);

/*******************************************************************************
  Function:
    void DISP_Update(void)

  Summary:
    Polls for touch events and processes it.
*/
void DISP_Update(void);

#ifdef __cplusplus
    }
#endif

#endif
