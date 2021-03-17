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

/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_widget_skin_classic_common.c

  Summary:
    Common functions for the classic widget skin.

  Description:
*******************************************************************************/

/** \file legato_widget_skin_classic_common.h
* @brief Common functions for the classic widget skin.
*
* @details Common functions for the classic widget skin.
*/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_WIDGET_SKIN_CLASSIC_COMMON_H
#define LEGATO_WIDGET_SKIN_CLASSIC_COMMON_H
//DOM-IGNORE-END

#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/common/legato_pixelbuffer.h"

// DOM-IGNORE-BEGIN
// internal use only

void leWidget_SkinClassic_FillRect(const leRect* rect,
                                   leColor clr,
                                   uint32_t a);

void leWidget_SkinClassic_DrawBackground(leWidget* wgt,
                                         leColor clr,
                                         uint32_t a);

void leWidget_SkinClassic_DrawRoundCornerBackground(leWidget* wgt,
                                                    leColor clr,
                                                    uint32_t a);

void leWidget_SkinClassic_DrawStandardBackground(leWidget* wgt,
                                                 uint32_t a);

void leWidget_SkinClassic_DrawLineBorderLeft(const leRect* rect, 
                                             leColor clr,
                                             uint32_t a);

void leWidget_SkinClassic_DrawLineBorderTop(const leRect* rect,
                                            leColor clr,
                                            uint32_t a);

void leWidget_SkinClassic_DrawLineBorderRight(const leRect* rect,
                                              leColor clr,
                                              uint32_t a);

void leWidget_SkinClassic_DrawLineBorderBottom(const leRect* rect,
                                               leColor clr,
                                               uint32_t a);

void leWidget_SkinClassic_DrawLineBorder(const leRect* rect,
                                         leColor clr,
                                         uint32_t a);

void leWidget_SkinClassic_DrawBevelBorderLeft(const leRect* rect,
                                              leColor outer,
                                              leColor inner,
                                              uint32_t a);

void leWidget_SkinClassic_DrawBevelBorderTop(const leRect* rect,
                                             leColor outer,
                                             leColor inner,
                                             uint32_t a);

void leWidget_SkinClassic_DrawBevelBorderRight(const leRect* rect,
                                               leColor outer,
                                               leColor inner,
                                               uint32_t a);

void leWidget_SkinClassic_DrawBevelBorderBottom(const leRect* rect,
                                                leColor outer,
                                                leColor inner,
                                                uint32_t a);

void leWidget_SkinClassic_Draw2x2BeveledBorder(const leRect* rect,
                                               leColor leftUpOuter,
                                               leColor leftUpInner,
                                               leColor bottomRightOuter,
                                               leColor bototmRightInner,
                                               uint32_t a);

void leWidget_SkinClassic_Draw1x2BeveledBorder(const leRect* rect,
                                              leColor leftUp,
                                              leColor bottomRightOuter,
                                              leColor bototmRightInner,
                                              uint32_t a);

void leWidget_SkinClassic_DrawStandardLineBorder(leWidget* wgt,
                                                 uint32_t a);

void leWidget_SkinClassic_DrawStandardRoundCornerLineBorder(leWidget* wgt,
                                                            uint32_t a);

void leWidget_SkinClassic_DrawStandardRaisedBorder(leWidget* wgt,
                                                   uint32_t a);

void leWidget_SkinClassic_DrawStandardLoweredBorder(leWidget* wgt,
                                                    uint32_t a);

void leWidget_SkinClassic_DrawStandardHybridBorder(leWidget* wgt,
                                                   uint32_t a);

void leWidget_SkinClassic_InvalidateBorderAreas(const leWidget*);

// DOM-IGNORE-END

#endif /* LEGATO_WIDGET_SKIN_CLASSIC_COMMON_H */