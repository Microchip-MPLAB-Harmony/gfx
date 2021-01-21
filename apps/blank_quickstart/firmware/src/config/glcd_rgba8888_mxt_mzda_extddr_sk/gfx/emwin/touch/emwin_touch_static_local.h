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
  emWin Touch Wrapper Local Header File

  Company:
    Microchip Technology Inc.

  File Name:
    emwin_touch_static_local.h

  Summary:
    This header file provides prototypes and definitions local to emWin 
    touch wrapper

  Description:
    
*******************************************************************************/


#ifndef _EMWIN_TOUCH_STATIC_LOCAL_H_
#define _EMWIN_TOUCH_STATIC_LOCAL_H_

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "gfx/emwin/touch/emwin_touch_static.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************


typedef enum
{
    /* Init state */
    EMWIN_TOUCH_STATE_INIT = 0,

    /* Touch process state */
    EMWIN_TOUCH_STATE_PROCESS,
            


}EMWIN_TOUCH_STATES;


typedef struct
{
    /* Pointer input device state */
    GUI_PID_STATE           pidState;
    
    /* layer index */
    uint32_t                layerIndex;

    /* Touch orientation */
    EMWIN_TOUCH_ORIENTATION orientation;

    /* Display width */
    uint32_t                displayWidth;

    /* Display Height */
    uint32_t                displayHeight;    

	EMWIN_TOUCH_STATES      touchState;
	
} EMWIN_TOUCH_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _EMWIN_TOUCH_STATIC_LOCAL_H_

/*******************************************************************************
 End of File
 */

