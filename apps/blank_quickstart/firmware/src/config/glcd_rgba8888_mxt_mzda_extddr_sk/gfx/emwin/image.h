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


#ifndef IMAGE_H
#define IMAGE_H

#include "WM.h"
#include "DIALOG_Intern.h"
#include "WIDGET.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" { // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define IMAGE_CF_MEMDEV   (1 << 0) // Widget uses an internal memory device which speeds up use of compressed images (GIF, JPEG, PNG)
#define IMAGE_CF_TILE     (1 << 1) // Uses tiling to fill up the whole area of the widget
#define IMAGE_CF_ALPHA    (1 << 2) // Needs to be set if alpha blending is required (PNG)
#define IMAGE_CF_ATTACHED (1 << 3) // Widget size is fixed to the parent border
#define IMAGE_CF_AUTOSIZE (1 << 4) // Widget size is taken from the attached image

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef WM_HMEM IMAGE_Handle;

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
IMAGE_Handle IMAGE_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);
IMAGE_Handle IMAGE_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumExtraBytes);
IMAGE_Handle IMAGE_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

void IMAGE_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/
int  IMAGE_GetUserData(IMAGE_Handle hObj, void * pDest, int NumBytes);
void IMAGE_SetBitmap  (IMAGE_Handle hWin, const GUI_BITMAP * pBitmap);
void IMAGE_SetBMP     (IMAGE_Handle hObj, const void * pData, U32 FileSize);
void IMAGE_SetBMPEx   (IMAGE_Handle hObj, GUI_GET_DATA_FUNC * pfGetData, void * pVoid);
void IMAGE_SetDTA     (IMAGE_Handle hObj, const void * pData, U32 FileSize);
void IMAGE_SetDTAEx   (IMAGE_Handle hObj, GUI_GET_DATA_FUNC * pfGetData, void * pVoid);
void IMAGE_SetGIF     (IMAGE_Handle hObj, const void * pData, U32 FileSize);
void IMAGE_SetGIFEx   (IMAGE_Handle hObj, GUI_GET_DATA_FUNC * pfGetData, void * pVoid);
void IMAGE_SetJPEG    (IMAGE_Handle hObj, const void * pData, U32 FileSize);
void IMAGE_SetJPEGEx  (IMAGE_Handle hObj, GUI_GET_DATA_FUNC * pfGetData, void * pVoid);
void IMAGE_SetPNG     (IMAGE_Handle hObj, const void * pData, U32 FileSize);
void IMAGE_SetPNGEx   (IMAGE_Handle hObj, GUI_GET_DATA_FUNC * pfGetData, void * pVoid);
int  IMAGE_SetUserData(IMAGE_Handle hObj, const void * pSrc, int NumBytes);


#if defined(__cplusplus)
  }
#endif

#endif // GUI_WINSUPPORT
#endif // IMAGE_H

/*************************** End of file ****************************/
