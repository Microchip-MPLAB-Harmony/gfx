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


#ifndef GUI_SPRITE_PRIVATE_H
#define GUI_SPRITE_PRIVATE_H

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define SPRITE_LOCK_H(h) (GUI_SPRITE_OBJ *)GUI_LOCK_H(h)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
//
// The sprite object
//
typedef struct {
  GUI_DEVICE       * pDevice;
  GUI_RECT           Rect;
  GUI_HMEM           hColors;
  U16                Flags;
  const GUI_BITMAP * pBM;
  void            (* pCB)(GUI_HSPRITE hSprite, int Cmd); // Callback routine for animated sprites
  GUI_HMEM           hContext;
} GUI_SPRITE_OBJ;


#endif // GUI_SPRITE_PRIVATE_H

/*************************** End of file ****************************/
