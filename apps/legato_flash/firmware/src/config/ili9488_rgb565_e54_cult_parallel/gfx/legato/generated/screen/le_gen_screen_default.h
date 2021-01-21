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

#ifndef LE_GEN_SCREEN_DEFAULT_H
#define LE_GEN_SCREEN_DEFAULT_H

#include "gfx/legato/generated/le_gen_init.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// widget list for this screen
// layer 0
extern leWidget* BackgroundPanel;
extern leImageWidget* WatermarkImage;
extern leImageWidget* LogoImage;
extern leLabelWidget* TitleLabel;
extern leWidget* SelectMediumPanel;
extern leLabelWidget* SelectMediumLabel;
extern leButtonWidget* SDCardButton;
extern leButtonWidget* USBButton;
extern leWidget* ErrorMsgPanel;
extern leLabelWidget* NoMediumLabel2;
extern leLabelWidget* NoMediumLabel1;
extern leWidget* InfoPanel;
extern leLabelWidget* InfoLabel1;
extern leLabelWidget* InfoLabel2;
extern leButtonWidget* InfoOKButton;
extern leWidget* FlashingPanel;
extern leProgressBarWidget* FlashingProgressBar;
extern leLabelWidget* FlashingLabel;
extern leLabelWidget* RecordsTotalLabel;
extern leLabelWidget* OfLabel;
extern leLabelWidget* CurrentRecordLabel;


// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_default(); // called when Legato is initialized
leResult screenShow_default(); // called when screen is shown
void screenHide_default(); // called when screen is hidden
void screenDestroy_default(); // called when Legato is destroyed
void screenUpdate_default(); // called when Legato is updating

leWidget* screenGetRoot_default(uint32_t lyrIdx); // gets a root widget for this screen

// event handlers
void default_OnShow(void);

void default_OnUpdate(void);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_DEFAULT_H
