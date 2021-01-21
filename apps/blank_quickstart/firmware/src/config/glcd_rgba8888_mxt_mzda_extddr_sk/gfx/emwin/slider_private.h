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


#ifndef SLIDER_PRIVATE_H
#define SLIDER_PRIVATE_H

#include "WM.h"
#include "WIDGET.h"
#include "SLIDER.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Flags
//
#define SLIDER_FLAG_DRAW_FOCUS_RECT (1 << 0)

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  WIDGET_DRAW_ITEM_FUNC * pfDrawSkin;
} SLIDER_SKIN_PRIVATE;

typedef struct {
  U8                  Flags;
  GUI_COLOR           BkColor;
  GUI_COLOR           BarColor;
  GUI_COLOR           FocusColor;
  GUI_COLOR           TickColor;
  SLIDER_SKIN_PRIVATE SkinPrivate;
} SLIDER_PROPS;

typedef struct {
  WIDGET              Widget;
  SLIDER_PROPS        Props;
  WIDGET_SKIN const * pWidgetSkin;
  int                 NumTicks;
  int                 Max;
  int                 Min;
  int                 v;
  I16                 Width;
} SLIDER_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define SLIDER_INIT_ID(p) (p->Widget.DebugId = SLIDER_ID)
#else
  #define SLIDER_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  SLIDER_Obj * SLIDER_LockH(SLIDER_Handle h);
  #define SLIDER_LOCK_H(h)  SLIDER_LockH(h)
#else
  #define SLIDER_LOCK_H(h)  (SLIDER_Obj *)GUI_LOCK_H(h)
#endif

#ifndef   SLIDER_SUPPORT_TRANSPARENCY
  #define SLIDER_SUPPORT_TRANSPARENCY WM_SUPPORT_TRANSPARENCY
#endif

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/
extern       SLIDER_PROPS   SLIDER__DefaultProps;
extern const WIDGET_SKIN    SLIDER__SkinClassic;
extern       WIDGET_SKIN    SLIDER__Skin;
extern const WIDGET_SKIN  * SLIDER__pSkinDefault;

#endif  // GUI_WINSUPPORT
#endif  // SLIDER_PRIVATE_H

/*************************** End of file ****************************/
