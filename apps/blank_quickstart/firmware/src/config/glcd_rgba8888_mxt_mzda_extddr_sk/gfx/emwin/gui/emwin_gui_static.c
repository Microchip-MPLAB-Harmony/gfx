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
  emWin GUI wrapper Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    emwin_gui_static.c

  Summary:
    This file contains the source code for the emWin GUI wrapper.

  Description:

 *******************************************************************************/



// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "gfx/emwin/gui/emwin_gui_static_local.h"
#include "gfx/emwin/touch/emwin_touch_static.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

static EMWIN_GUI_DATA emWinGuiData;

/*******************************************************************************
  Function:
    void emWin_GuiInitialize(void)

  Remarks:
    See prototype in emwin_gui_static.h.
 */

void emWin_GuiInitialize(void)
{   
    emWinGuiData.state         = EMWIN_GUI_STATE_DISPLAY_INIT;
    emWinGuiData.screenChanged = true;
    emWinGuiData.screenId      = 0;
    
	if (GFX_Open(0, 0, 0, GFX_NULL) != NULL)
    {
        emWinGuiData.state = EMWIN_GUI_STATE_INIT;                
    }
	
	emWinGuiData.status = GUI_Init();
	if( 0 == emWinGuiData.status )
    {
                emWinGuiData.state = EMWIN_GUI_STATE_SCREEN_INIT;
    }
	
	
    return;
}

/*******************************************************************************
  Function:
    void emWin_GuiScreenInitializeRegister( 
                                       EMWIN_GUI_SCREEN_INITIALIZE screenInit )

  Remarks:
    See prototype in emwin_gui_static.h.
*/

void emWin_GuiScreenInitializeRegister( EMWIN_GUI_SCREEN_INITIALIZE screenInit )
{
    if( NULL == screenInit )
    {
        return;
    }
    
    emWinGuiData.screenInitialize = screenInit;
}

/*******************************************************************************
  Function:
    void emWin_GuiScreenRegister( int32_t screenId, 
                                  EMWIN_GUI_SCREEN_CREATE screen )

  Remarks:
    See prototype in emwin_gui_static.h.
 */

void emWin_GuiScreenRegister( int32_t screenId, EMWIN_GUI_SCREEN_CREATE screen )
{
    if( screenId >= EMWIN_GUI_NUM_SCREENS )
    {
        return;
    }
    
    if( NULL == screen )
    {
        return;
    }
    
    emWinGuiData.screenCreate[screenId] = screen;
    
    return;

}

/*******************************************************************************
  Function:
    void emWin_GuiStartScreenSet( int32_t screenId )

  Remarks:
    See prototype in emwin_gui_static.h.
*/

void emWin_GuiStartScreenSet( int32_t screenId )
{
    if( screenId >= EMWIN_GUI_NUM_SCREENS )
    {
        return;
    }
    
    emWinGuiData.screenId = screenId;
}

/*******************************************************************************
  Function:
    void emWin_GuiScreenChange( int32_t screenId )

  Remarks:
    See prototype in emwin_gui_static.h.
 */
void emWin_GuiScreenChange( int32_t screenId )
{
    if( screenId >= EMWIN_GUI_NUM_SCREENS )
    {
        return;
    }
    
    emWinGuiData.screenChanged = true;
    emWinGuiData.screenId      = screenId;
    
    return;
}

/*******************************************************************************
  Function:
    WM_HWIN emWin_GuiScreenGet( int32_t screenId )

  Remarks:
    See prototype in emwin_gui_static.h.
 */

WM_HWIN emWin_GuiScreenGet( int32_t screenId )
{
    if( screenId >= EMWIN_GUI_NUM_SCREENS )
    {
        return 0;
    }

    return ( emWinGuiData.hScreen[screenId] );
}

/*******************************************************************************
  Function:
    void emWin_GuiScreenEnd ( int32_t screenId )

  Remarks:
    See prototype in emwin_gui_static.h.
 */

void emWin_GuiScreenEnd( int32_t screenId )
{
    if( screenId >= EMWIN_GUI_NUM_SCREENS )
    {
        return;
    }

    if( 0 != emWinGuiData.hScreen[screenId] )
    {

        GUI_EndDialog( emWinGuiData.hScreen[screenId], 0 );
        emWinGuiData.hScreen[screenId] = 0;

    }

    return;
}


/*********************************************************************
*
*       emWinTasks
*/

void emWin_Tasks(void)
{
    emWin_GuiTasks();
    emWin_TouchTasks();
}

void emWin_GuiTasks(void) 
{
    switch( emWinGuiData.state )
    {
        case EMWIN_GUI_STATE_DISPLAY_INIT:
		{
            if (GFX_Open(0, 0, 0, GFX_NULL) != NULL)
            {
                emWinGuiData.state = EMWIN_GUI_STATE_INIT;                
            }
            break;
        }        
        case EMWIN_GUI_STATE_INIT:
        {
            /* Initialize SEGGER emWin GUI Library */
            emWinGuiData.status = GUI_Init();
  
            if( 0 == emWinGuiData.status )
            {
                emWinGuiData.state = EMWIN_GUI_STATE_SCREEN_INIT;
            }

            break;
        }
        
        case EMWIN_GUI_STATE_SCREEN_INIT:
        {
            /* Initialize first screen */
            if( NULL != emWinGuiData.screenInitialize )
            {
                emWinGuiData.screenInitialize();
            }
            
            emWinGuiData.state = EMWIN_GUI_STATE_TASKS;
            
            break;
        }
        
        case EMWIN_GUI_STATE_TASKS:
        {
            if(emWinGuiData.screenChanged)
	        {
		        emWinGuiData.screenChanged = 0;
                
                if( NULL != emWinGuiData.screenCreate[emWinGuiData.screenId] )
                {
                    if ( !emWinGuiData.hScreen[emWinGuiData.screenId] )
                    {
                        emWinGuiData.hScreen[emWinGuiData.screenId] = 
                           emWinGuiData.screenCreate[emWinGuiData.screenId]();
                    }
                    else
                    {
                        WM_BringToTop(emWinGuiData.hScreen[emWinGuiData.screenId]);
                    }
                }
	        }
            
            GUI_Exec();
            
            break;
        }
        
        default:
        {
            break;
        }
    }
    
    return;
}

/*******************************************************************************
 End of File
 */



