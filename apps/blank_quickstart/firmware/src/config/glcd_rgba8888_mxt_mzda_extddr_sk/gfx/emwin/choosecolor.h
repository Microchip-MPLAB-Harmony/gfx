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


#ifndef CHOOSECOLOR_H
#define CHOOSECOLOR_H

#include "WM.h"

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
#define CHOOSECOLOR_CF_MOVEABLE FRAMEWIN_CF_MOVEABLE

#define CHOOSECOLOR_CI_FRAME 0
#define CHOOSECOLOR_CI_FOCUS 1

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       CHOOSECOLOR_PROPS
*/
typedef struct {
  unsigned  aBorder[2];
  unsigned  aSpace[2];
  unsigned  aButtonSize[2];
  GUI_COLOR aColor[2];
} CHOOSECOLOR_PROPS;

/*********************************************************************
*
*       CHOOSECOLOR_CONTEXT
*/
typedef struct {
  U32               LastColor;
  const GUI_COLOR * pColor;
  unsigned          NumColors;
  unsigned          NumColorsPerLine;
  int               SelOld;
  int               Sel;
  WM_HWIN           hParent;
  CHOOSECOLOR_PROPS Props;
} CHOOSECOLOR_CONTEXT;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
WM_HWIN CHOOSECOLOR_Create(WM_HWIN           hParent,
                           int               xPos,
                           int               yPos,
                           int               xSize,
                           int               ySize,
                           const GUI_COLOR * pColor,
                           unsigned          NumColors,
                           unsigned          NumColorsPerLine,
                           int               Sel,
                           const char      * sCaption,
                           int               Flags);

int  CHOOSECOLOR_GetSel(WM_HWIN hObj);
void CHOOSECOLOR_SetSel(WM_HWIN hObj, int Sel);

void CHOOSECOLOR_SetDefaultColor     (unsigned Index, GUI_COLOR Color);
void CHOOSECOLOR_SetDefaultSpace     (unsigned Index, unsigned Space);
void CHOOSECOLOR_SetDefaultBorder    (unsigned Index, unsigned Border);
void CHOOSECOLOR_SetDefaultButtonSize(unsigned Index, unsigned ButtonSize);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void CHOOSECOLOR_Callback(WM_MESSAGE * pMsg);

#if defined(__cplusplus)
  }
#endif

#endif /* GUI_WINSUPPORT */

#endif /* CHOOSECOLOR_H */

/*************************** End of file ****************************/
