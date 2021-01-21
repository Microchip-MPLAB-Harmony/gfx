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


#ifndef DIALOG_H
#define DIALOG_H

#include "WM.h"

#include "BUTTON.h"
#include "CALENDAR.h"
#include "CHECKBOX.h"
#include "CHOOSECOLOR.h"
#include "CHOOSEFILE.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "GRAPH.h"
#include "HEADER.h"
#include "ICONVIEW.h"
#include "IMAGE.h"
#include "KNOB.h"
#include "LISTBOX.h"
#include "LISTVIEW.h"
#include "LISTWHEEL.h"
#include "MENU.h"
#include "MULTIEDIT.h"
#include "MULTIPAGE.h"
#include "PROGBAR.h"
#include "RADIO.h"
#include "SCROLLBAR.h"
#include "SLIDER.h"
#include "SPINBOX.h"
#include "SWIPELIST.h"
#include "TEXT.h"
#include "TREEVIEW.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       WINDOW API
*/
WM_HWIN   WINDOW_CreateEx         (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, WM_CALLBACK * cb);
WM_HWIN   WINDOW_CreateUser       (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, WM_CALLBACK * cb, int NumExtraBytes);
WM_HWIN   WINDOW_CreateIndirect   (const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);
GUI_COLOR WINDOW_GetDefaultBkColor(void);
int       WINDOW_GetUserData      (WM_HWIN hObj, void * pDest, int NumBytes);
void      WINDOW_SetBkColor       (WM_HWIN hObj, GUI_COLOR Color);
void      WINDOW_SetDefaultBkColor(GUI_COLOR Color);
int       WINDOW_SetUserData      (WM_HWIN hObj, const void * pSrc, int NumBytes);

void WINDOW_Callback(WM_MESSAGE * pMsg);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // DIALOG_H

/*************************** End of file ****************************/
