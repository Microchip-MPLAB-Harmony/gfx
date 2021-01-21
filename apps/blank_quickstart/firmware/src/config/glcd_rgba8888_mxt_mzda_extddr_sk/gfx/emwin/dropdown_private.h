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


#ifndef DROPDOWN_PRIVATE_H
#define DROPDOWN_PRIVATE_H

#include "DROPDOWN.h"
#include "WIDGET.h"
#include "GUI_ARRAY.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define DROPDOWN_SF_AUTOSCROLLBAR DROPDOWN_CF_AUTOSCROLLBAR

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  WIDGET_DRAW_ITEM_FUNC * pfDrawSkin;
} DROPDOWN_SKIN_PRIVATE;

typedef struct {
  const GUI_FONT * pFont;
  GUI_COLOR aBackColor[3];
  GUI_COLOR aTextColor[3];
  GUI_COLOR aColor[2];
  GUI_COLOR aScrollbarColor[3];
  DROPDOWN_SKIN_PRIVATE SkinPrivate;
  I16       TextBorderSize;
  I16       Align;
} DROPDOWN_PROPS;

typedef struct {
  WIDGET  Widget;
  I16     Sel;        // Current selection
  I16     ySizeLB;    // ySize of assigned LISTBOX in expanded state
  I16     TextHeight;
  GUI_ARRAY Handles;
  WM_SCROLL_STATE ScrollState;
  DROPDOWN_PROPS Props;
  WIDGET_SKIN const * pWidgetSkin;
  WM_HWIN hListWin;
  U8      Flags;
  U16     ItemSpacing;
  U8      ScrollbarWidth;
  char  IsPressed;
  WM_HMEM hDisabled;
} DROPDOWN_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define DROPDOWN_INIT_ID(p) (p->Widget.DebugId = DROPDOWN_ID)
#else
  #define DROPDOWN_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  DROPDOWN_Obj * DROPDOWN_LockH(DROPDOWN_Handle h);
  #define DROPDOWN_LOCK_H(h)   DROPDOWN_LockH(h)
#else
  #define DROPDOWN_LOCK_H(h)   (DROPDOWN_Obj *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Private (module internal) data
*
**********************************************************************
*/

extern DROPDOWN_PROPS DROPDOWN__DefaultProps;

extern const WIDGET_SKIN DROPDOWN__SkinClassic;
extern       WIDGET_SKIN DROPDOWN__Skin;

extern WIDGET_SKIN const * DROPDOWN__pSkinDefault;

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/

void DROPDOWN__AdjustHeight(DROPDOWN_Handle hObj);
int  DROPDOWN__GetNumItems (DROPDOWN_Obj * pObj);
const char * DROPDOWN__GetpItemLocked(DROPDOWN_Handle hObj, int Index);

#endif // GUI_WINSUPPORT
#endif // DROPDOWN_PRIVATE_H

/*************************** End of file ****************************/
