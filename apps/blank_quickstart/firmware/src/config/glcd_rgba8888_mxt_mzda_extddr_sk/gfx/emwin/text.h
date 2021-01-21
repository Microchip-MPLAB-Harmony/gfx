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


#ifndef TEXT_H
#define TEXT_H

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"
#include "GUI_Debug.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/************************************************************
*
*       #defines
*
*************************************************************
*/

/************************************************************
*
*       Create / Status flags
*/
#define TEXT_CF_LEFT    GUI_TA_LEFT
#define TEXT_CF_RIGHT   GUI_TA_RIGHT
#define TEXT_CF_HCENTER GUI_TA_HCENTER

#define TEXT_CF_VCENTER GUI_TA_VCENTER
#define TEXT_CF_TOP     GUI_TA_TOP
#define TEXT_CF_BOTTOM  GUI_TA_BOTTOM


/*********************************************************************
*
*       Public Types
*
**********************************************************************

*/
typedef WM_HMEM TEXT_Handle;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/
TEXT_Handle TEXT_Create        (int x0, int y0, int xSize, int ySize, int Id, int Flags, const char * s, int Align);
TEXT_Handle TEXT_CreateAsChild (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int Id, int Flags, const char * s, int Align);
TEXT_Handle TEXT_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, const char * pText);
TEXT_Handle TEXT_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, const char * pText, int NumExtraBytes);
TEXT_Handle TEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void TEXT_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/

/* Methods changing properties */

GUI_COLOR        TEXT_GetBkColor  (TEXT_Handle hObj); 
const GUI_FONT * TEXT_GetFont     (TEXT_Handle hObj);
int              TEXT_GetNumLines (TEXT_Handle hObj);
int              TEXT_GetText     (TEXT_Handle hObj, char * pDest, U32 BufferSize);
int              TEXT_GetTextAlign(TEXT_Handle hObj);
GUI_COLOR        TEXT_GetTextColor(TEXT_Handle hObj);
int              TEXT_GetUserData (TEXT_Handle hObj, void * pDest, int NumBytes);
GUI_WRAPMODE     TEXT_GetWrapMode (TEXT_Handle hObj);
void             TEXT_SetBkColor  (TEXT_Handle hObj, GUI_COLOR Color);
void             TEXT_SetFont     (TEXT_Handle hObj, const GUI_FONT * pFont);
int              TEXT_SetDec      (TEXT_Handle hObj, I32 v, U8 Len, U8 Shift, U8 Signed, U8 Space);
int              TEXT_SetText     (TEXT_Handle hObj, const char * s);
void             TEXT_SetTextAlign(TEXT_Handle hObj, int Align);
void             TEXT_SetTextColor(TEXT_Handle hObj, GUI_COLOR Color);
int              TEXT_SetUserData (TEXT_Handle hObj, const void * pSrc, int NumBytes);
void             TEXT_SetWrapMode (TEXT_Handle hObj, GUI_WRAPMODE WrapMode);

/*********************************************************************
*
*       Managing default values
*
**********************************************************************
*/

const GUI_FONT * TEXT_GetDefaultFont     (void);
GUI_COLOR        TEXT_GetDefaultTextColor(void);
GUI_WRAPMODE     TEXT_GetDefaultWrapMode (void);
void             TEXT_SetDefaultFont     (const GUI_FONT * pFont);
void             TEXT_SetDefaultTextColor(GUI_COLOR Color);
GUI_WRAPMODE     TEXT_SetDefaultWrapMode (GUI_WRAPMODE WrapMode);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // TEXT_H

/*************************** End of file ****************************/
