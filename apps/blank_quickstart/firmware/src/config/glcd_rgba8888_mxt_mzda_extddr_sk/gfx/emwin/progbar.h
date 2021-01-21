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


#ifndef PROGBAR_H        /* Avoid multiple inclusion  */
#define PROGBAR_H

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       Create flags
*/
#define PROGBAR_CF_HORIZONTAL (0 << 0)
#define PROGBAR_CF_VERTICAL   (1 << 0)
#define PROGBAR_CF_USER       (1 << 1)

/*********************************************************************
*
*       Skinning constants
*/
#define PROGBAR_SKINFLEX_L 0
#define PROGBAR_SKINFLEX_R 1

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef WM_HMEM PROGBAR_Handle;

typedef struct {
  GUI_COLOR aColorUpperL[2];
  GUI_COLOR aColorLowerL[2];
  GUI_COLOR aColorUpperR[2];
  GUI_COLOR aColorLowerR[2];
  GUI_COLOR ColorFrame;
  GUI_COLOR ColorText;
} PROGBAR_SKINFLEX_PROPS;

typedef struct {
  int IsVertical;
  int Index;
  const char * pText;
} PROGBAR_SKINFLEX_INFO;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/

PROGBAR_Handle PROGBAR_Create        (int x0, int y0, int xSize, int ySize, int Flags);
PROGBAR_Handle PROGBAR_CreateAsChild (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int Id, int Flags);
PROGBAR_Handle PROGBAR_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);
PROGBAR_Handle PROGBAR_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumExtraBytes);
PROGBAR_Handle PROGBAR_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void PROGBAR_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Individual member functions
*
**********************************************************************
*/
GUI_COLOR        PROGBAR_GetBarColor(PROGBAR_Handle hObj, unsigned int Index);
const GUI_FONT * PROGBAR_GetFont     (PROGBAR_Handle hObj);
void             PROGBAR_GetMinMax   (PROGBAR_Handle hObj, int * pMin, int * pMax);
int              PROGBAR_GetUserData (PROGBAR_Handle hObj, void * pDest, int NumBytes);
GUI_COLOR        PROGBAR_GetTextColor(PROGBAR_Handle hObj, unsigned int Index);
int              PROGBAR_GetValue    (PROGBAR_Handle hObj);
void             PROGBAR_SetBarColor (PROGBAR_Handle hObj, unsigned int index, GUI_COLOR color);
void             PROGBAR_SetFont     (PROGBAR_Handle hObj, const GUI_FONT * pfont);
void             PROGBAR_SetMinMax   (PROGBAR_Handle hObj, int Min, int Max);
void             PROGBAR_SetText     (PROGBAR_Handle hObj, const char* s);
void             PROGBAR_SetTextAlign(PROGBAR_Handle hObj, int Align);
void             PROGBAR_SetTextColor(PROGBAR_Handle hObj, unsigned int index, GUI_COLOR color);
void             PROGBAR_SetTextPos  (PROGBAR_Handle hObj, int XOff, int YOff);
void             PROGBAR_SetValue    (PROGBAR_Handle hObj, int v);
int              PROGBAR_SetUserData (PROGBAR_Handle hObj, const void * pSrc, int NumBytes);

/*********************************************************************
*
*       Member functions: Skinning
*
**********************************************************************
*/
void PROGBAR_GetSkinFlexProps     (PROGBAR_SKINFLEX_PROPS * pProps, int Index);
void PROGBAR_SetSkinClassic       (PROGBAR_Handle hObj);
void PROGBAR_SetSkin              (PROGBAR_Handle hObj, WIDGET_DRAW_ITEM_FUNC * pfDrawSkin);
int  PROGBAR_DrawSkinFlex         (const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
void PROGBAR_SetSkinFlexProps     (const PROGBAR_SKINFLEX_PROPS * pProps, int Index);
void PROGBAR_SetDefaultSkinClassic(void);
WIDGET_DRAW_ITEM_FUNC * PROGBAR_SetDefaultSkin(WIDGET_DRAW_ITEM_FUNC * pfDrawSkin);

#define PROGBAR_SKIN_FLEX    PROGBAR_DrawSkinFlex

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // PROGBAR_H

/*************************** End of file ****************************/
