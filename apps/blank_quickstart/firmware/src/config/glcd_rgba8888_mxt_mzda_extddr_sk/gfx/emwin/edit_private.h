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


#ifndef EDIT_PRIVATE_H
#define EDIT_PRIVATE_H

#include "EDIT.h"

#if GUI_WINSUPPORT

#include "WIDGET.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define EDIT_REALLOC_SIZE  16

#ifndef EDIT_XOFF
  #define EDIT_XOFF        1
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct EDIT_Obj_struct EDIT_Obj;

typedef struct {
  int              Align;
  int              Border;
  const GUI_FONT * pFont;
  GUI_COLOR        aTextColor[3];
  GUI_COLOR        aBkColor[3];
} EDIT_PROPS;

struct EDIT_Obj_struct {
  WIDGET               Widget;
  WM_HMEM              hpText;
  I16                  MaxLen;
  U16                  BufferSize;
  I32                  Min, Max;        // Min max values as normalized floats (integers)
  U8                   NumDecs;         // Number of decimals
  I32                  CurrentValue;    // Current value
  int                  CursorPos;       // Cursor position. 0 means left most
  unsigned             SelSize;         // Number of selected characters
  U8                   EditMode;        // Insert or overwrite mode
  U8                   XSizeCursor;     // Size of cursor when working in insert mode
  U8                   Flags;
  tEDIT_AddKeyEx     * pfAddKeyEx;      // Handle key input
  tEDIT_UpdateBuffer * pfUpdateBuffer;  // Update textbuffer
  EDIT_PROPS           Props;
  WM_HTIMER            hTimer;
  U8                   MinMaxMode;
};

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define EDIT_INIT_ID(p) (p->Widget.DebugId = EDIT_ID)
#else
  #define EDIT_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  EDIT_Obj * EDIT_LockH(EDIT_Handle h);
  #define EDIT_LOCK_H(h)   EDIT_LockH(h)
#else
  #define EDIT_LOCK_H(h)   (EDIT_Obj *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/
extern EDIT_PROPS EDIT__DefaultProps;

/*********************************************************************
*
*       Public functions (internal)
*
**********************************************************************
*/
U16  EDIT__GetCurrentChar  (EDIT_Obj * pObj);
void EDIT__SetCursorPos    (EDIT_Handle hObj, int CursorPos);
void EDIT__SetValueUnsigned(EDIT_Handle hObj, I32 Value);

#endif // GUI_WINSUPPORT

#endif // EDIT_PRIVATE_H

/*************************** End of file ****************************/
