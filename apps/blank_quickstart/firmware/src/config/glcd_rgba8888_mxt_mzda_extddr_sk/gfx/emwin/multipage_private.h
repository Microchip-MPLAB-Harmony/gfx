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


#ifndef MULTIPAGE_PRIVATE_H
#define MULTIPAGE_PRIVATE_H

#include "GUI_Debug.h"
#include "GUI_ARRAY.h"
#include "MULTIPAGE.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define MULTIPAGE_STATE_ENABLED     (1 << 0)
#define MULTIPAGE_STATE_SCROLLMODE  WIDGET_STATE_USER0

#define MULTIPAGE_NUMCOLORS         2

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
//
// MULTIPAGE_PAGE
//
typedef struct {
  WM_HWIN hWin;
  U8      Status;
  int     ItemWidth;
  WM_HMEM hDrawObj[3];
  char    acText;
} MULTIPAGE_PAGE;

//
// MULTIPAGE_SKIN_PRIVATE
//
typedef struct {
  WIDGET_DRAW_ITEM_FUNC * pfDrawSkin;
} MULTIPAGE_SKIN_PRIVATE;

//
// MULTIPAGE_PROPS
//
typedef struct {
  const GUI_FONT          * pFont;
  unsigned                  Align;
  GUI_COLOR                 aBkColor[MULTIPAGE_NUMCOLORS];
  GUI_COLOR                 aTextColor[MULTIPAGE_NUMCOLORS];
  MULTIPAGE_SKIN_PRIVATE    SkinPrivate;
  int                       BorderSize0;
  int                       BorderSize1;
  unsigned                  TextAlign;
  unsigned                  Scrollbar;
  int                    (* pfGetTouchedPage)(MULTIPAGE_Handle hObj, int x, int y);
  int                    (* pfGetTabBarWidth)(MULTIPAGE_Handle hObj);
} MULTIPAGE_PROPS;

//
// MULTIPAGE_Obj
//
typedef struct MULTIPAGE_Obj MULTIPAGE_Obj;

struct MULTIPAGE_Obj {
  WIDGET                 Widget;
  void                (* pfDrawTextItem)(MULTIPAGE_Obj * pObj, const char * pText, unsigned Index, const GUI_RECT * pRect, int x0, int xSize, int ColorIndex);
  WM_HWIN                hClient;
  GUI_ARRAY              hPageArray;
  unsigned               Selection;
  int                    ScrollState;
  MULTIPAGE_PROPS        Props;
  WIDGET_SKIN const    * pWidgetSkin;
  MULTIPAGE_SKIN_PROPS   SkinProps;
  int                    ItemHeight;
  int                    MaxHeight;
};

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define MULTIPAGE_INIT_ID(p) (p->Widget.DebugId = MULTIPAGE_ID)
#else
  #define MULTIPAGE_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  MULTIPAGE_Obj * MULTIPAGE_LockH(MULTIPAGE_Handle h);
  #define MULTIPAGE_LOCK_H(h)   MULTIPAGE_LockH(h)
#else
  #define MULTIPAGE_LOCK_H(h)   (MULTIPAGE_Obj *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/
extern GUI_COLOR           MULTIPAGE__aEffectColor[2];
extern MULTIPAGE_PROPS     MULTIPAGE__DefaultProps;

extern const WIDGET_SKIN   MULTIPAGE__SkinClassic;
extern       WIDGET_SKIN   MULTIPAGE__Skin;

extern WIDGET_SKIN const * MULTIPAGE__pSkinDefault;

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
void MULTIPAGE__CalcBorderRect (MULTIPAGE_Obj * pObj, GUI_RECT * pRect);
void MULTIPAGE__CalcClientRect (MULTIPAGE_Handle hObj, GUI_RECT * pRect);
void MULTIPAGE__DeleteScrollbar(MULTIPAGE_Handle hObj);
void MULTIPAGE__DrawTextItemH  (MULTIPAGE_Obj * pObj, const char * pText, unsigned Index, const GUI_RECT * pRect, int x0, int w, int ColorIndex);
int  MULTIPAGE__GetPagePos     (MULTIPAGE_Handle hObj, unsigned Index);
int  MULTIPAGE__GetPageWidth   (MULTIPAGE_Handle hObj, unsigned Index);
void MULTIPAGE__GetTabBarRect  (MULTIPAGE_Handle hObj, GUI_RECT * pRect);
void MULTIPAGE__UpdatePositions(MULTIPAGE_Handle hObj);

/*********************************************************************
*
*       Private Skinning functions
*
**********************************************************************
*/
int  MULTIPAGE_SKIN__GetPagePos    (MULTIPAGE_Handle hObj, unsigned Index);
int  MULTIPAGE_SKIN__GetTabBarWidth(MULTIPAGE_Handle hObj);
int  MULTIPAGE_SKIN__GetTouchedPage(MULTIPAGE_Handle hObj, int TouchX, int TouchY);

#endif  // GUI_WINSUPPORT
#endif  // MULTIPAGE_PRIVATE_H

/*************************** End of file ****************************/
