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


#ifndef IMAGE_PRIVATE_H
#define IMAGE_PRIVATE_H

#include "IMAGE.h"
#include "GUI_Private.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/
#ifndef IMAGE_SUPPORT_TRANSPARENCY
  #define IMAGE_SUPPORT_TRANSPARENCY WM_SUPPORT_TRANSPARENCY
#endif

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  U32 Flags;
} IMAGE_PROPS;

typedef struct {
  WIDGET              Widget;
  IMAGE_PROPS         Props;
  const void        * pData;                                                          // Data pointer of the object to be drawn (Bitmap, BMP, GIF, JPEG, PNG)
  void             (* pfDrawImageAt) (IMAGE_Handle hObj, int xPos, int yPos);         // Object specific (Bitmap, BMP, GIF, JPEG, PNG) code
  void             (* pfPaint)       (IMAGE_Handle hObj);                             // Drawing method specific (Default, Tiled, Magnified) code
  void             (* pfGetImageSize)(IMAGE_Handle hObj, int * pxSize, int * pySize); // Returns the image size of the attached item
  void             (* pfOnTimer)     (IMAGE_Handle hObj);                             // Timer function for animated images (currently only animated GIFs are supported)
  U32                 FileSize;
  //
  // Data items used by IAMGE_GIF.c
  //
  int                 NumImages;    // Number of (sub)images
  int                 CurrentImage; // Image index used for animated images
  GUI_TIMER_HANDLE    hTimer;       // Timer used for animated images
  //
  // Data items used by IAMGE_DTA.c
  //
  GUI_BITMAP          Bitmap;       // Bitmap structure
  GUI_LOGPALETTE      Palette;      // Palette structure
  //
  // Data items used by Image_...Ex() - functions
  //
  void              * pVoid;        // Void pointer passed to GetData() function
  GUI_GET_DATA_FUNC * pfGetData;    // Pointer to GetData() function
  //
  // Data items used if memory devices are available and IMAGE_CF_MEMDEV has been set
  //
  #if GUI_SUPPORT_MEMDEV
    GUI_MEMDEV_Handle hMem;
  #endif
} IMAGE_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define IMAGE_INIT_ID(p) (p->Widget.DebugId = IMAGE_ID)
#else
  #define IMAGE_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  IMAGE_OBJ * IMAGE__LockH(IMAGE_Handle h);
  #define IMAGE_LOCK_H(h)   IMAGE__LockH(h)
#else
  #define IMAGE_LOCK_H(h)   (IMAGE_OBJ *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Module internal data
*
**********************************************************************
*/
extern IMAGE_PROPS IMAGE__DefaultProps;

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
void IMAGE__SetWindowSize(IMAGE_Handle hObj);
void IMAGE__FreeAttached (IMAGE_Handle hObj, int LeaveTimer);

#endif // GUI_WINSUPPORT
#endif // IMAGE_PRIVATE_H

/*************************** End of file ****************************/
