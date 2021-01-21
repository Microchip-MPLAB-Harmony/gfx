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


#ifndef CHOOSEFILE_H
#define CHOOSEFILE_H

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
#define CHOOSEFILE_FINDFIRST 0
#define CHOOSEFILE_FINDNEXT  1

#define CHOOSEFILE_FLAG_DIRECTORY (1 << 0)

#ifndef   CHOOSEFILE_MAXLEN
  #define CHOOSEFILE_MAXLEN 256
#endif

#define CHOOSEFILE_BI_CANCEL 0
#define CHOOSEFILE_BI_OK     1
#define CHOOSEFILE_BI_UP     2

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       CHOOSEFILE_INFO
*/
typedef struct CHOOSEFILE_INFO CHOOSEFILE_INFO;

struct CHOOSEFILE_INFO {
  int               Cmd;                                 // Command for GetData() function
  int               Id;                                  // Id of pressed button (for internal use only)
  const char      * pMask;                               // Mask to be used for searching files
  char            * pName;                               // (for internal use only)
  char            * pExt;                                // (for internal use only)
  char            * pAttrib;                             // (for internal use only)
  WM_TOOLTIP_HANDLE hToolTip;                            // (for internal use only)
  U32               SizeL;                               // FileSize low word
  U32               SizeH;                               // FileSize high word
  U32               Flags;                               // File flags
  char              pRoot[CHOOSEFILE_MAXLEN];            // Buffer used internally and for passing result
  int            (* pfGetData)(CHOOSEFILE_INFO * pInfo); // Pointer to GetData() function
};

/*********************************************************************
*
*       Functions
*
**********************************************************************
*/
WM_HWIN CHOOSEFILE_Create(WM_HWIN           hParent,  // Parent window
                          int               xPos,     // xPosition in window coordinates
                          int               yPos,     // yPosition in window coordinates
                          int               xSize,    // xSize in pixels
                          int               ySize,    // ySize in pixels
                          const char      * apRoot[], // Pointers to root strings
                          int               NumRoot,  // Number of roots
                          int               SelRoot,  // Root to be selected at first
                          const char      * sCaption, // Shown in title bar
                          int               Flags,    // Flags for FRAMEWINDOW
                          CHOOSEFILE_INFO * pInfo     // Pointer to CHOOSEFILE_INFO structure
                          );

void    CHOOSEFILE_Callback            (WM_MESSAGE * pMsg);
void    CHOOSEFILE_EnableToolTips      (void);
void    CHOOSEFILE_SetButtonText       (WM_HWIN hWin, unsigned ButtonIndex, const char * pText);
void    CHOOSEFILE_SetDefaultButtonText(unsigned ButtonIndex, const char * pText);
void    CHOOSEFILE_SetDelim            (char Delim);
void    CHOOSEFILE_SetToolTips         (const TOOLTIP_INFO * pInfo, int NumItems);
void    CHOOSEFILE_SetTopMode          (unsigned OnOff);

#if defined(__cplusplus)
  }
#endif

#endif /* GUI_WINSUPPORT */

#endif /* CHOOSEFILE_H */

/*************************** End of file ****************************/
