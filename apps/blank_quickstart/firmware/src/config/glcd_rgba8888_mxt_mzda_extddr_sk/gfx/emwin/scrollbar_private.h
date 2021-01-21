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


#ifndef SCROLLBAR_PRIVATE_H
#define SCROLLBAR_PRIVATE_H

#include "SCROLLBAR.h"
#include "WIDGET.h"
#include "GUI_Debug.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define PRESSED_STATE_NONE  0
#define PRESSED_STATE_RIGHT 1
#define PRESSED_STATE_LEFT  2
#define PRESSED_STATE_THUMB 3

/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/

/* Define colors */
#ifndef   SCROLLBAR_COLOR_SHAFT_DEFAULT
  #define SCROLLBAR_COLOR_SHAFT_DEFAULT GUI_GRAY
#endif

#ifndef   SCROLLBAR_COLOR_ARROW_DEFAULT
  #define SCROLLBAR_COLOR_ARROW_DEFAULT GUI_BLACK
#endif

#ifndef   SCROLLBAR_COLOR_THUMB_DEFAULT
  #define SCROLLBAR_COLOR_THUMB_DEFAULT GUI_GRAY_C0
#endif

#ifndef   SCROLLBAR_THUMB_SIZE_MIN_DEFAULT
  #define SCROLLBAR_THUMB_SIZE_MIN_DEFAULT 4
#endif

#ifndef SCROLLBAR_DEFAULT_WIDTH
  #if   WIDGET_USE_SCHEME_SMALL
    #define SCROLLBAR_DEFAULT_WIDTH 11
  #elif WIDGET_USE_SCHEME_MEDIUM
    #define SCROLLBAR_DEFAULT_WIDTH 16
  #elif WIDGET_USE_SCHEME_LARGE
    #define SCROLLBAR_DEFAULT_WIDTH 22
  #endif
#endif

#define SCROLLBAR_TIMER_ID 1234

/*********************************************************************
*
*       Module internal data
*
**********************************************************************
*/
extern GUI_COLOR  SCROLLBAR__aDefaultBkColor[2];
extern GUI_COLOR  SCROLLBAR__aDefaultColor[2];
extern I16        SCROLLBAR__DefaultWidth;
extern I16        SCROLLBAR__ThumbSizeMin;

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  WIDGET_DRAW_ITEM_FUNC * pfDrawSkin;
} SCROLLBAR_SKIN_PRIVATE;

typedef struct {
  GUI_COLOR aColor[3];
  SCROLLBAR_SKIN_PRIVATE SkinPrivate;
} SCROLLBAR_PROPS;

typedef struct {
  int x0_LeftArrow;
  int x1_LeftArrow;
  int x0_Thumb;
  int x1_Thumb;
  int x0_RightArrow;
  int x1_RightArrow;
  int x1;
  int xSizeMoveable;
  int ThumbSize;
} SCROLLBAR_POSITIONS;

typedef struct SCROLLBAR_OBJ SCROLLBAR_OBJ;

struct SCROLLBAR_OBJ {
  WIDGET Widget;
  SCROLLBAR_PROPS Props;
  WIDGET_SKIN const * pWidgetSkin;
  void (* pfCalcPositions)(SCROLLBAR_Handle hObj, SCROLLBAR_POSITIONS * pPos);
  int NumItems, v, PageSize;
  int State;
  int TimerStep;
  int TouchPos;
  WM_HMEM hTimer;
};

/*********************************************************************
*
*       Private macros
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define SCROLLBAR_INIT_ID(p) (p->Widget.DebugId = SCROLLBAR_ID)
#else
  #define SCROLLBAR_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  SCROLLBAR_OBJ * SCROLLBAR_LockH(SCROLLBAR_Handle h);
  #define SCROLLBAR_LOCK_H(h)   SCROLLBAR_LockH(h)
#else
  #define SCROLLBAR_LOCK_H(h)   (SCROLLBAR_OBJ *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
void SCROLLBAR__InvalidatePartner(SCROLLBAR_Handle hObj);
void SCROLLBAR__Rect2VRect       (const WIDGET * pWidget, GUI_RECT * pRect);

/*********************************************************************
*
*       Private data
*
**********************************************************************
*/
extern SCROLLBAR_PROPS SCROLLBAR__DefaultProps;

extern const WIDGET_SKIN SCROLLBAR__SkinClassic;
extern       WIDGET_SKIN SCROLLBAR__Skin;

extern WIDGET_SKIN const * SCROLLBAR__pSkinDefault;

#endif        /* GUI_WINSUPPORT */
#endif        /* Avoid multiple inclusion */

/*************************** End of file ****************************/
