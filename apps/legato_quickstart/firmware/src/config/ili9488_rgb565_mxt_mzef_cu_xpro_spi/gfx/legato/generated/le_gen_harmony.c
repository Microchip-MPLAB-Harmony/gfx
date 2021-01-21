// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C)  Microchip Technology Inc. and its subsidiaries.
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
#include "gfx/legato/generated/le_gen_harmony.h"

#include "definitions.h"

#include "system/input/sys_input.h"

// Input System Service interface code
SYS_INP_InputListener inputListener;

static void touchDownHandler(const SYS_INP_TouchStateEvent* const evt);
static void touchUpHandler(const SYS_INP_TouchStateEvent* const evt);
static void touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt);


static LegatoState legatoState;

void Legato_Initialize(void)
{
    leInitialize(&gfxDriverInterface, &gfxGPUInterface);

    gfxDriverInterface.setPalette(leGlobalPalette.header.address,
                                  leGlobalPalette.colorMode,
                                  leGlobalPalette.colorCount);

    legato_initialize();


    inputListener.handleTouchDown = &touchDownHandler;
    inputListener.handleTouchUp = &touchUpHandler;
    inputListener.handleTouchMove = &touchMoveHandler;

    legatoState = LEGATO_STATE_INIT;
}

void Legato_Tasks(void)
{
    switch(legatoState)
    {
        case LEGATO_STATE_INIT:
        {
            SYS_INP_AddListener(&inputListener);

            legatoState = LEGATO_STATE_RUNNING;

            break;
        }
        case LEGATO_STATE_RUNNING:
        {
            //Legato_DemoModeProcessEvents();

            legato_updateCurrentScreen();

            leUpdate(0);

            break;
        }

        default:
        {
            break;
        }
    }
}

void touchDownHandler(const SYS_INP_TouchStateEvent* const evt)
{
    leInput_InjectTouchDown(evt->index, evt->x, evt->y);
}

void touchUpHandler(const SYS_INP_TouchStateEvent* const evt)
{
    leInput_InjectTouchUp(evt->index, evt->x, evt->y);
}

void touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt)
{
    leInput_InjectTouchMoved(evt->index, evt->x, evt->y);
}

