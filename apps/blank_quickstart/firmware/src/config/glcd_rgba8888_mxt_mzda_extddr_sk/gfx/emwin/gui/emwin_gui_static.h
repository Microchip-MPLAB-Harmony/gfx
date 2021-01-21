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
  emWin GUI wrapper Header File

  Company:
    Microchip Technology Inc.

  File Name:
    emwin_gui_static.h

  Summary:
    This header file provides prototypes and definitions for emwin GUI wrapper.

  Description:
    
*******************************************************************************/


#ifndef _EMWIN_GUI_STATIC_H_
#define _EMWIN_GUI_STATIC_H_

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"
#include "gfx/emwin/emwin_config.h"
#include "gfx/emwin/gui.h"
#include "gfx/emwin/dialog.h"

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
/* 

  Summary:

  Description:

  Remarks:

 */

typedef WM_HWIN (* EMWIN_GUI_SCREEN_CREATE)(void);

// *****************************************************************************
/* 

  Summary:

  Description:

  Remarks:

 */

typedef void    (* EMWIN_GUI_SCREEN_INITIALIZE )( void );

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */
void emWin_GuiInitialize(void);

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */

void emWin_GuiScreenInitializeRegister( EMWIN_GUI_SCREEN_INITIALIZE screenInit );

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */

void emWin_GuiScreenRegister( int32_t screenId, EMWIN_GUI_SCREEN_CREATE screen );

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */

void emWin_GuiStartScreenSet( int32_t screenId );

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */

void emWin_GuiScreenChange( int32_t screenId );

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */

WM_HWIN emWin_GuiScreenGet( int32_t screenId );

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */

void emWin_GuiScreenEnd( int32_t screenId );

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */
void emWin_GuiTasks(void);

/*******************************************************************************
  Function:

  Summary:

  Description:

  Precondition:

  Parameters:

  Returns:

  Example:
    <code>
    </code>

  Remarks:

 */
void emWin_Tasks(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _EMWIN_GUI_STATIC_H_

/*******************************************************************************
 End of File
 */

