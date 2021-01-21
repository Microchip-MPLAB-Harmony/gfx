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
  emWin Touch Wrapper Source File

  Company:
    Microchip Technology Inc.

  File Name:
    emwin_touch_static.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/



// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "emwin_touch_static_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
static EMWIN_TOUCH_DATA emWinTouchData;

SYS_INP_InputListener inputListener_emwin;
GUI_PID_STATE * pidState = NULL;

static void touchDownHandler_emwin(const SYS_INP_TouchStateEvent* const evt);
static void touchUpHandler_emwin(const SYS_INP_TouchStateEvent* const evt);
static void touchMoveHandler_emwin(const SYS_INP_TouchMoveEvent* const evt);

/*******************************************************************************/
/*  Function:
    void emWin_TouchInitialize ( void )

  Remarks:
    See prototype in app.h.
 */
 void emWin_TouchInitialize( )
{

    pidState = &emWinTouchData.pidState;
    inputListener_emwin.handleTouchDown = &touchDownHandler_emwin;
    inputListener_emwin.handleTouchUp = &touchUpHandler_emwin;
    inputListener_emwin.handleTouchMove = &touchMoveHandler_emwin;

    emWinTouchData.layerIndex    = 0;
    emWinTouchData.orientation   = EMWIN_TOUCH_ORIENTATION_0_DEGREE;
    emWinTouchData.displayWidth  = 480;
    emWinTouchData.displayHeight = 272;
    emWinTouchData.touchState = EMWIN_TOUCH_STATE_INIT;
    return;
}

// *****************************************************************************
// *****************************************************************************
// Section: Local Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void emWin_TouchLayerIndexSet ( void )

  Remarks:
    See prototype in app.h.
*/
void emWin_TouchLayerIndexSet( uint32_t layerIndex )
{
    emWinTouchData.layerIndex = layerIndex;
}

/*******************************************************************************
  Function:
    void emWin_TouchOrientationSet ( void )

  Remarks:
    See prototype in app.h.
*/
void emWin_TouchOrientationSet( EMWIN_TOUCH_ORIENTATION orientation )
{
    emWinTouchData.orientation = orientation;
}

/*******************************************************************************
  Function:
    void emWin_TouchResolutionSet ( void )

  Remarks:
    See prototype in app.h.
*/
void emWin_TouchResolutionSet( uint32_t displayWidth, uint32_t displayHeight )
{
    emWinTouchData.displayWidth  = displayWidth;
    emWinTouchData.displayHeight = displayHeight;
}

/*******************************************************************************/
/*
  Function:
    void emWin_TouchTasks ( void )

  Remarks:
    See prototype in app.h.
*/
void emWin_TouchTasks(void)
{
    /* Check the application's current state. */
    switch ( emWinTouchData.touchState)
    {

        case EMWIN_TOUCH_STATE_INIT:
            SYS_INP_AddListener(&inputListener_emwin);
            emWinTouchData.touchState = EMWIN_TOUCH_STATE_PROCESS;
            break;

        case EMWIN_TOUCH_STATE_PROCESS:
            break;

        default:
            break;
    }

}

void touchDownHandler_emwin(const SYS_INP_TouchStateEvent* const evt)
{

    pidState->Pressed = 1;


    pidState->Layer = emWinTouchData.layerIndex;

    switch( emWinTouchData.orientation )
    {
        case EMWIN_TOUCH_ORIENTATION_0_DEGREE:
        {
            pidState->x = evt->x;
            pidState->y = evt->y;
            break;
        }

        case EMWIN_TOUCH_ORIENTATION_90_DEGREE:
        {
            pidState->x = emWinTouchData.displayHeight - evt->x - 1;
            pidState->y = evt->y;
            break;
        }

        case EMWIN_TOUCH_ORIENTATION_180_DEGREE:
        {
            pidState->x = emWinTouchData.displayWidth  - evt->x;
            pidState->y = emWinTouchData.displayHeight - evt->y;
            break;
        }

        case EMWIN_TOUCH_ORIENTATION_270_DEGREE:
        {
            pidState->x = evt->x;
            pidState->y = emWinTouchData.displayHeight - evt->y - 1;
            break;
        }
    }

    GUI_TOUCH_StoreStateEx( pidState );
}

void touchUpHandler_emwin(const SYS_INP_TouchStateEvent* const evt)
{
    pidState->Pressed = 0;


    pidState->Layer = emWinTouchData.layerIndex;

    switch( emWinTouchData.orientation )
    {
        case EMWIN_TOUCH_ORIENTATION_0_DEGREE:
        {
            pidState->x = evt->x;
            pidState->y = evt->y;
            break;
        }

        case EMWIN_TOUCH_ORIENTATION_90_DEGREE:
        {
            pidState->x = emWinTouchData.displayHeight - evt->x - 1;
            pidState->y = evt->y;
            break;
        }

        case EMWIN_TOUCH_ORIENTATION_180_DEGREE:
        {
            pidState->x = emWinTouchData.displayWidth  - evt->x;
            pidState->y = emWinTouchData.displayHeight - evt->y;
            break;
        }

        case EMWIN_TOUCH_ORIENTATION_270_DEGREE:
        {
            pidState->x = evt->x;
            pidState->y = emWinTouchData.displayHeight - evt->y - 1;
            break;
        }
    }

    GUI_TOUCH_StoreStateEx( pidState );

}

void touchMoveHandler_emwin(const SYS_INP_TouchMoveEvent* const evt)
{
    GUI_TOUCH_StoreStateEx( pidState );
}

/*******************************************************************************
 End of File
 */



