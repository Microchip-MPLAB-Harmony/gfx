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


#ifndef DIALOG_INTERN_H
#define DIALOG_INTERN_H

#include "WM.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {        // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct  GUI_WIDGET_CREATE_INFO_struct GUI_WIDGET_CREATE_INFO;
typedef WM_HWIN GUI_WIDGET_CREATE_FUNC        (const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       Structures
*
**********************************************************************
*/
struct GUI_WIDGET_CREATE_INFO_struct {
  GUI_WIDGET_CREATE_FUNC * pfCreateIndirect;
  const char             * pName;            // Text ... Not used on all widgets
  I16                      Id;               // ID ... should be unique in a dialog
  I16                      x0;               // x position
  I16                      y0;               // y position
  I16                      xSize;            // x size
  I16                      ySize;            // y size
  U16                      Flags;            // Widget specific create flags (opt.)
  I32                      Para;             // Widget specific parameter (opt.)
  U32                      NumExtraBytes;    // Number of extra bytes usable with <WIDGET>_SetUserData & <WIDGET>_GetUserData
};

/*********************************************************************
*
*       Public API functions
*
**********************************************************************
*/
WM_HWIN            GUI_CreateDialogBox   (const GUI_WIDGET_CREATE_INFO * paWidget, int NumWidgets, WM_CALLBACK * cb, WM_HWIN hParent, int x0, int y0);
void               GUI_EndDialog         (WM_HWIN hWin, int r);
int                GUI_ExecDialogBox     (const GUI_WIDGET_CREATE_INFO * paWidget, int NumWidgets, WM_CALLBACK * cb, WM_HWIN hParent, int x0, int y0);
int                GUI_ExecCreatedDialog (WM_HWIN hDialog);
WM_DIALOG_STATUS * GUI_GetDialogStatusPtr(WM_HWIN hDialog);                                    // Not to be documented
void               GUI_SetDialogStatusPtr(WM_HWIN hDialog, WM_DIALOG_STATUS * pDialogStatus);  // Not to be documented

/*********************************************************************
*
*       Obsolete
*/
LCD_COLOR          DIALOG_GetBkColor(void);
LCD_COLOR          DIALOG_SetBkColor(LCD_COLOR BkColor);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // DIALOG_INTERN_H

/*************************** End of file ****************************/
