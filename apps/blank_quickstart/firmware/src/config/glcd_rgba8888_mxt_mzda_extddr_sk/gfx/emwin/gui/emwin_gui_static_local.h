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
  emWin GUI Wrapper local header

  Company:
    Microchip Technology Inc.

  File Name:
    emwin_gui_static_local.h

  Summary:
    This header file provides prototypes and definitions for the emWin GUI
    wrapper.

  Description:
    
*******************************************************************************/


#ifndef _EMWIN_GUI_STATIC_LOCAL_H_
#define _EMWIN_GUI_STATIC_LOCAL_H_

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "gfx/emwin/gui/emwin_gui_static.h"
#include "gfx/hal/inc/gfx_driver_interface.h"

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

// *****************************************************************************
/* emWin GUI Wrapper States

  Summary:

  Description:

  Remarks:

 */

typedef enum
{
    /* Init display state */
    EMWIN_GUI_STATE_DISPLAY_INIT = 0,

    /* GUI Init state */
    EMWIN_GUI_STATE_INIT,
            
    /* Screen Init state */
    EMWIN_GUI_STATE_SCREEN_INIT,
            
    /* Tasks state */
    EMWIN_GUI_STATE_TASKS,

}EMWIN_GUI_STATES;

// *****************************************************************************
/* emWin GUI wrapper data object 

  Summary:

  Description:

  Remarks:

 */

typedef struct
{
    /* GUI wrapper states */
    EMWIN_GUI_STATES            state;
    
    /* GUI wrapper status */
    int32_t                     status;
    
    /* screen change flag */
    bool                        screenChanged;
    
    /* screen Id */
    int32_t                     screenId;
    
    /* screen create function array */
    EMWIN_GUI_SCREEN_CREATE     screenCreate[ EMWIN_GUI_NUM_SCREENS ];

    /* screen window handle */
    WM_HWIN                     hScreen[ EMWIN_GUI_NUM_SCREENS ];
        
    /* screen initialize function pointer */
    EMWIN_GUI_SCREEN_INITIALIZE screenInitialize;

} EMWIN_GUI_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _EMWIN_GUI_STATIC_LOCAL_H_

/*******************************************************************************
 End of File
 */


