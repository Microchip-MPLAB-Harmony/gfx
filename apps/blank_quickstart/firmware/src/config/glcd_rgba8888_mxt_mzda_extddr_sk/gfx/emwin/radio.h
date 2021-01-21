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


#ifndef RADIO_H
#define RADIO_H

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Bitmap indices
*/
#define RADIO_BI_INACTIV 0
#define RADIO_BI_ACTIV   1
#define RADIO_BI_CHECK   2

/*********************************************************************
*
*       Skinning property indices
*/
#define RADIO_SKINFLEX_PI_PRESSED     0
#define RADIO_SKINFLEX_PI_UNPRESSED   1

/*********************************************************************
*
*       Defaults for public configuration switches
*
**********************************************************************

The following are defaults for config switches which affect the
interface specified in this module
*/

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define RADIO_TEXTPOS_RIGHT       0
#define RADIO_TEXTPOS_LEFT        WIDGET_STATE_USER0  /* Not implemented, TBD */

/*********************************************************************
*
*       Public Types
*
**********************************************************************
*/
typedef WM_HMEM RADIO_Handle;

typedef struct {
  GUI_COLOR aColorButton[4];
  int       ButtonSize;
} RADIO_SKINFLEX_PROPS;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/

RADIO_Handle RADIO_Create        (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int Id, int Flags, unsigned Para);
RADIO_Handle RADIO_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumItems, int Spacing);
RADIO_Handle RADIO_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumItems, int Spacing, int NumExtraBytes);
RADIO_Handle RADIO_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void RADIO_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Managing default values
*
**********************************************************************
*/

void             RADIO_SetDefaultFont      (const GUI_FONT * pFont);
GUI_COLOR        RADIO_SetDefaultFocusColor(GUI_COLOR Color);
void             RADIO_SetDefaultImage     (const GUI_BITMAP * pBitmap, unsigned int Index);
void             RADIO_SetDefaultTextColor (GUI_COLOR TextColor);
const GUI_FONT * RADIO_GetDefaultFont      (void);
GUI_COLOR        RADIO_GetDefaultTextColor (void);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/

void               RADIO_AddValue     (RADIO_Handle hObj, int Add);
void               RADIO_Dec          (RADIO_Handle hObj);
GUI_COLOR          RADIO_GetBkColor   (RADIO_Handle hObj);
GUI_COLOR          RADIO_GetFocusColor(RADIO_Handle hObj);
const GUI_FONT *   RADIO_GetFont      (RADIO_Handle hObj);
const GUI_BITMAP * RADIO_GetImage     (RADIO_Handle hObj, unsigned int Index);
int                RADIO_GetNumItems  (RADIO_Handle hObj);
int                RADIO_GetText      (RADIO_Handle hObj, unsigned Index, char * pBuffer, int MaxLen);
GUI_COLOR          RADIO_GetTextColor (RADIO_Handle hObj);
int                RADIO_GetUserData  (RADIO_Handle hObj, void * pDest, int NumBytes);
void               RADIO_Inc          (RADIO_Handle hObj);
void               RADIO_SetBkColor   (RADIO_Handle hObj, GUI_COLOR Color);
GUI_COLOR          RADIO_SetFocusColor(RADIO_Handle hObj, GUI_COLOR Color);
void               RADIO_SetFont      (RADIO_Handle hObj, const GUI_FONT * pFont);
void               RADIO_SetGroupId   (RADIO_Handle hObj, U8 GroupId);
void               RADIO_SetImage     (RADIO_Handle hObj, const GUI_BITMAP * pBitmap, unsigned int Index);
void               RADIO_SetText      (RADIO_Handle hObj, const char* pText, unsigned Index);
void               RADIO_SetTextColor (RADIO_Handle hObj, GUI_COLOR Color);
void               RADIO_SetValue     (RADIO_Handle hObj, int v);
int                RADIO_SetUserData  (RADIO_Handle hObj, const void * pSrc, int NumBytes);

/*********************************************************************
*
*       Member functions: Skinning
*
**********************************************************************
*/
void RADIO_GetSkinFlexProps     (RADIO_SKINFLEX_PROPS * pProps, int Index);
void RADIO_SetSkinClassic       (RADIO_Handle hObj);
void RADIO_SetSkin              (RADIO_Handle hObj, WIDGET_DRAW_ITEM_FUNC * pfDrawSkin);
int  RADIO_DrawSkinFlex         (const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
void RADIO_SetSkinFlexProps     (const RADIO_SKINFLEX_PROPS * pProps, int Index);
void RADIO_SetDefaultSkinClassic(void);
WIDGET_DRAW_ITEM_FUNC * RADIO_SetDefaultSkin(WIDGET_DRAW_ITEM_FUNC * pfDrawSkin);

#define RADIO_SKIN_FLEX    RADIO_DrawSkinFlex

/*********************************************************************
*
*       Query state
*
**********************************************************************
*/
int RADIO_GetValue(RADIO_Handle hObj);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // RADIO_H

/*************************** End of file ****************************/
