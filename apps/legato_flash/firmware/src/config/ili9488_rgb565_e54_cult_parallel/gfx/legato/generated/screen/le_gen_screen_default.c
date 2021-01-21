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

#include "gfx/legato/generated/screen/le_gen_screen_default.h"

// widget list for layer 0
static leWidget* root0;

leWidget* BackgroundPanel;
leImageWidget* WatermarkImage;
leImageWidget* LogoImage;
leLabelWidget* TitleLabel;
leWidget* SelectMediumPanel;
leLabelWidget* SelectMediumLabel;
leButtonWidget* SDCardButton;
leButtonWidget* USBButton;
leWidget* ErrorMsgPanel;
leLabelWidget* NoMediumLabel2;
leLabelWidget* NoMediumLabel1;
leWidget* InfoPanel;
leLabelWidget* InfoLabel1;
leLabelWidget* InfoLabel2;
leButtonWidget* InfoOKButton;
leWidget* FlashingPanel;
leProgressBarWidget* FlashingProgressBar;
leLabelWidget* FlashingLabel;
leLabelWidget* RecordsTotalLabel;
leLabelWidget* OfLabel;
leLabelWidget* CurrentRecordLabel;

// string list for this screen
static leTableString tableString_Title;
static leTableString tableString_SelectSource;
static leTableString tableString_SDCard;
static leTableString tableString_USB;
static leTableString tableString_NoValidMedium2;
static leTableString tableString_NoValidMedium;
static leTableString tableString_FileNotFound1;
static leTableString tableString_FileNotFound2;
static leTableString tableString_Ok;
static leTableString tableString_Flashing;
static leTableString tableString_stringNumberFiller;
static leTableString tableString_Of;

static leBool showing = LE_FALSE;

leResult screenInit_default()
{
    return LE_SUCCESS;
}

leResult screenShow_default()
{
    if(showing == LE_TRUE)
        return LE_FAILURE;

    // initialize static strings
    leTableString_Constructor(&tableString_Title, string_Title);
    leTableString_Constructor(&tableString_SelectSource, string_SelectSource);
    leTableString_Constructor(&tableString_SDCard, string_SDCard);
    leTableString_Constructor(&tableString_USB, string_USB);
    leTableString_Constructor(&tableString_NoValidMedium2, string_NoValidMedium2);
    leTableString_Constructor(&tableString_NoValidMedium, string_NoValidMedium);
    leTableString_Constructor(&tableString_FileNotFound1, string_FileNotFound1);
    leTableString_Constructor(&tableString_FileNotFound2, string_FileNotFound2);
    leTableString_Constructor(&tableString_Ok, string_Ok);
    leTableString_Constructor(&tableString_Flashing, string_Flashing);
    leTableString_Constructor(&tableString_stringNumberFiller, string_stringNumberFiller);
    leTableString_Constructor(&tableString_Of, string_Of);

    // layer 0
    root0 = leWidget_New();
    root0->fn->setPosition(root0, 0, 0);
    root0->fn->setSize(root0, 480, 320);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);

    BackgroundPanel = leWidget_New();
    BackgroundPanel->fn->setPosition(BackgroundPanel, 0, 0);
    BackgroundPanel->fn->setSize(BackgroundPanel, 480, 320);
    BackgroundPanel->fn->setScheme(BackgroundPanel, &whiteScheme);
    root0->fn->addChild(root0, BackgroundPanel);

    WatermarkImage = leImageWidget_New();
    WatermarkImage->fn->setPosition(WatermarkImage, 170, 106);
    WatermarkImage->fn->setSize(WatermarkImage, 141, 132);
    WatermarkImage->fn->setBackgroundType(WatermarkImage, LE_WIDGET_BACKGROUND_NONE);
    WatermarkImage->fn->setImage(WatermarkImage, &MHGS_logo_small);
    root0->fn->addChild(root0, (leWidget*)WatermarkImage);

    LogoImage = leImageWidget_New();
    LogoImage->fn->setPosition(LogoImage, 11, 11);
    LogoImage->fn->setSize(LogoImage, 32, 32);
    LogoImage->fn->setBackgroundType(LogoImage, LE_WIDGET_BACKGROUND_NONE);
    LogoImage->fn->setImage(LogoImage, &mchpLogo);
    root0->fn->addChild(root0, (leWidget*)LogoImage);

    TitleLabel = leLabelWidget_New();
    TitleLabel->fn->setPosition(TitleLabel, 64, 14);
    TitleLabel->fn->setSize(TitleLabel, 377, 32);
    TitleLabel->fn->setScheme(TitleLabel, &defaultScheme);
    TitleLabel->fn->setBackgroundType(TitleLabel, LE_WIDGET_BACKGROUND_NONE);
    TitleLabel->fn->setString(TitleLabel, (leString*)&tableString_Title);
    root0->fn->addChild(root0, (leWidget*)TitleLabel);

    SelectMediumPanel = leWidget_New();
    SelectMediumPanel->fn->setPosition(SelectMediumPanel, 0, 60);
    SelectMediumPanel->fn->setSize(SelectMediumPanel, 480, 260);
    SelectMediumPanel->fn->setBackgroundType(SelectMediumPanel, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->addChild(root0, SelectMediumPanel);

    SelectMediumLabel = leLabelWidget_New();
    SelectMediumLabel->fn->setPosition(SelectMediumLabel, 78, 6);
    SelectMediumLabel->fn->setSize(SelectMediumLabel, 333, 25);
    SelectMediumLabel->fn->setScheme(SelectMediumLabel, &defaultScheme);
    SelectMediumLabel->fn->setBackgroundType(SelectMediumLabel, LE_WIDGET_BACKGROUND_NONE);
    SelectMediumLabel->fn->setString(SelectMediumLabel, (leString*)&tableString_SelectSource);
    SelectMediumPanel->fn->addChild(SelectMediumPanel, (leWidget*)SelectMediumLabel);

    SDCardButton = leButtonWidget_New();
    SDCardButton->fn->setPosition(SDCardButton, 58, 65);
    SDCardButton->fn->setSize(SDCardButton, 100, 100);
    SDCardButton->fn->setVisible(SDCardButton, LE_FALSE);
    SDCardButton->fn->setScheme(SDCardButton, &defaultScheme);
    SDCardButton->fn->setString(SDCardButton, (leString*)&tableString_SDCard);
    SDCardButton->fn->setPressedImage(SDCardButton, &sdcard_sm);
    SDCardButton->fn->setReleasedImage(SDCardButton, &sdcard_sm);
    SDCardButton->fn->setImagePosition(SDCardButton, LE_RELATIVE_POSITION_ABOVE);
    SelectMediumPanel->fn->addChild(SelectMediumPanel, (leWidget*)SDCardButton);

    USBButton = leButtonWidget_New();
    USBButton->fn->setPosition(USBButton, 316, 65);
    USBButton->fn->setSize(USBButton, 100, 100);
    USBButton->fn->setVisible(USBButton, LE_FALSE);
    USBButton->fn->setScheme(USBButton, &defaultScheme);
    USBButton->fn->setString(USBButton, (leString*)&tableString_USB);
    USBButton->fn->setPressedImage(USBButton, &usb_icon_sm);
    USBButton->fn->setReleasedImage(USBButton, &usb_icon_sm);
    USBButton->fn->setImagePosition(USBButton, LE_RELATIVE_POSITION_ABOVE);
    SelectMediumPanel->fn->addChild(SelectMediumPanel, (leWidget*)USBButton);

    ErrorMsgPanel = leWidget_New();
    ErrorMsgPanel->fn->setPosition(ErrorMsgPanel, 67, 189);
    ErrorMsgPanel->fn->setSize(ErrorMsgPanel, 327, 57);
    ErrorMsgPanel->fn->setBackgroundType(ErrorMsgPanel, LE_WIDGET_BACKGROUND_NONE);
    SelectMediumPanel->fn->addChild(SelectMediumPanel, ErrorMsgPanel);

    NoMediumLabel2 = leLabelWidget_New();
    NoMediumLabel2->fn->setPosition(NoMediumLabel2, -6, 27);
    NoMediumLabel2->fn->setSize(NoMediumLabel2, 358, 25);
    NoMediumLabel2->fn->setScheme(NoMediumLabel2, &defaultScheme);
    NoMediumLabel2->fn->setBackgroundType(NoMediumLabel2, LE_WIDGET_BACKGROUND_NONE);
    NoMediumLabel2->fn->setHAlignment(NoMediumLabel2, LE_HALIGN_CENTER);
    NoMediumLabel2->fn->setString(NoMediumLabel2, (leString*)&tableString_NoValidMedium2);
    ErrorMsgPanel->fn->addChild(ErrorMsgPanel, (leWidget*)NoMediumLabel2);

    NoMediumLabel1 = leLabelWidget_New();
    NoMediumLabel1->fn->setPosition(NoMediumLabel1, 33, 2);
    NoMediumLabel1->fn->setSize(NoMediumLabel1, 277, 25);
    NoMediumLabel1->fn->setScheme(NoMediumLabel1, &defaultScheme);
    NoMediumLabel1->fn->setBackgroundType(NoMediumLabel1, LE_WIDGET_BACKGROUND_NONE);
    NoMediumLabel1->fn->setHAlignment(NoMediumLabel1, LE_HALIGN_CENTER);
    NoMediumLabel1->fn->setString(NoMediumLabel1, (leString*)&tableString_NoValidMedium);
    ErrorMsgPanel->fn->addChild(ErrorMsgPanel, (leWidget*)NoMediumLabel1);

    InfoPanel = leWidget_New();
    InfoPanel->fn->setPosition(InfoPanel, 0, 60);
    InfoPanel->fn->setSize(InfoPanel, 480, 260);
    InfoPanel->fn->setVisible(InfoPanel, LE_FALSE);
    InfoPanel->fn->setBackgroundType(InfoPanel, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->addChild(root0, InfoPanel);

    InfoLabel1 = leLabelWidget_New();
    InfoLabel1->fn->setPosition(InfoLabel1, 14, 4);
    InfoLabel1->fn->setSize(InfoLabel1, 448, 33);
    InfoLabel1->fn->setScheme(InfoLabel1, &defaultScheme);
    InfoLabel1->fn->setBackgroundType(InfoLabel1, LE_WIDGET_BACKGROUND_NONE);
    InfoLabel1->fn->setHAlignment(InfoLabel1, LE_HALIGN_CENTER);
    InfoLabel1->fn->setString(InfoLabel1, (leString*)&tableString_FileNotFound1);
    InfoPanel->fn->addChild(InfoPanel, (leWidget*)InfoLabel1);

    InfoLabel2 = leLabelWidget_New();
    InfoLabel2->fn->setPosition(InfoLabel2, 72, 29);
    InfoLabel2->fn->setSize(InfoLabel2, 336, 29);
    InfoLabel2->fn->setScheme(InfoLabel2, &defaultScheme);
    InfoLabel2->fn->setBackgroundType(InfoLabel2, LE_WIDGET_BACKGROUND_NONE);
    InfoLabel2->fn->setHAlignment(InfoLabel2, LE_HALIGN_CENTER);
    InfoLabel2->fn->setString(InfoLabel2, (leString*)&tableString_FileNotFound2);
    InfoPanel->fn->addChild(InfoPanel, (leWidget*)InfoLabel2);

    InfoOKButton = leButtonWidget_New();
    InfoOKButton->fn->setPosition(InfoOKButton, 177, 189);
    InfoOKButton->fn->setSize(InfoOKButton, 120, 44);
    InfoOKButton->fn->setScheme(InfoOKButton, &defaultScheme);
    InfoOKButton->fn->setString(InfoOKButton, (leString*)&tableString_Ok);
    InfoPanel->fn->addChild(InfoPanel, (leWidget*)InfoOKButton);

    FlashingPanel = leWidget_New();
    FlashingPanel->fn->setPosition(FlashingPanel, 39, 243);
    FlashingPanel->fn->setSize(FlashingPanel, 398, 60);
    FlashingPanel->fn->setVisible(FlashingPanel, LE_FALSE);
    FlashingPanel->fn->setBackgroundType(FlashingPanel, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->addChild(root0, FlashingPanel);

    FlashingProgressBar = leProgressBarWidget_New();
    FlashingProgressBar->fn->setPosition(FlashingProgressBar, 5, 30);
    FlashingProgressBar->fn->setSize(FlashingProgressBar, 385, 25);
    FlashingProgressBar->fn->setScheme(FlashingProgressBar, &ProgressScheme);
    FlashingPanel->fn->addChild(FlashingPanel, (leWidget*)FlashingProgressBar);

    FlashingLabel = leLabelWidget_New();
    FlashingLabel->fn->setPosition(FlashingLabel, 0, 0);
    FlashingLabel->fn->setSize(FlashingLabel, 310, 25);
    FlashingLabel->fn->setScheme(FlashingLabel, &defaultScheme);
    FlashingLabel->fn->setBackgroundType(FlashingLabel, LE_WIDGET_BACKGROUND_NONE);
    FlashingLabel->fn->setString(FlashingLabel, (leString*)&tableString_Flashing);
    FlashingPanel->fn->addChild(FlashingPanel, (leWidget*)FlashingLabel);

    RecordsTotalLabel = leLabelWidget_New();
    RecordsTotalLabel->fn->setPosition(RecordsTotalLabel, 327, 0);
    RecordsTotalLabel->fn->setSize(RecordsTotalLabel, 66, 25);
    RecordsTotalLabel->fn->setVisible(RecordsTotalLabel, LE_FALSE);
    RecordsTotalLabel->fn->setScheme(RecordsTotalLabel, &defaultScheme);
    RecordsTotalLabel->fn->setBackgroundType(RecordsTotalLabel, LE_WIDGET_BACKGROUND_NONE);
    RecordsTotalLabel->fn->setHAlignment(RecordsTotalLabel, LE_HALIGN_RIGHT);
    RecordsTotalLabel->fn->setString(RecordsTotalLabel, (leString*)&tableString_stringNumberFiller);
    FlashingPanel->fn->addChild(FlashingPanel, (leWidget*)RecordsTotalLabel);

    OfLabel = leLabelWidget_New();
    OfLabel->fn->setPosition(OfLabel, 301, 0);
    OfLabel->fn->setSize(OfLabel, 23, 25);
    OfLabel->fn->setVisible(OfLabel, LE_FALSE);
    OfLabel->fn->setScheme(OfLabel, &defaultScheme);
    OfLabel->fn->setBackgroundType(OfLabel, LE_WIDGET_BACKGROUND_NONE);
    OfLabel->fn->setString(OfLabel, (leString*)&tableString_Of);
    FlashingPanel->fn->addChild(FlashingPanel, (leWidget*)OfLabel);

    CurrentRecordLabel = leLabelWidget_New();
    CurrentRecordLabel->fn->setPosition(CurrentRecordLabel, 221, 0);
    CurrentRecordLabel->fn->setSize(CurrentRecordLabel, 80, 25);
    CurrentRecordLabel->fn->setVisible(CurrentRecordLabel, LE_FALSE);
    CurrentRecordLabel->fn->setScheme(CurrentRecordLabel, &defaultScheme);
    CurrentRecordLabel->fn->setBackgroundType(CurrentRecordLabel, LE_WIDGET_BACKGROUND_NONE);
    CurrentRecordLabel->fn->setHAlignment(CurrentRecordLabel, LE_HALIGN_RIGHT);
    CurrentRecordLabel->fn->setString(CurrentRecordLabel, (leString*)&tableString_stringNumberFiller);
    FlashingPanel->fn->addChild(FlashingPanel, (leWidget*)CurrentRecordLabel);

    leAddRootWidget(root0, 0);

    showing = LE_TRUE;

    default_OnShow();

    return LE_SUCCESS;
}

void screenUpdate_default()
{
    default_OnUpdate();
}

void screenHide_default()
{
    leRemoveRootWidget(root0, 0);

    leWidget_Delete(root0);

    root0 = NULL;

    BackgroundPanel = NULL;
    WatermarkImage = NULL;
    LogoImage = NULL;
    TitleLabel = NULL;
    SelectMediumPanel = NULL;
    SelectMediumLabel = NULL;
    SDCardButton = NULL;
    USBButton = NULL;
    ErrorMsgPanel = NULL;
    NoMediumLabel2 = NULL;
    NoMediumLabel1 = NULL;
    InfoPanel = NULL;
    InfoLabel1 = NULL;
    InfoLabel2 = NULL;
    InfoOKButton = NULL;
    FlashingPanel = NULL;
    FlashingProgressBar = NULL;
    FlashingLabel = NULL;
    RecordsTotalLabel = NULL;
    OfLabel = NULL;
    CurrentRecordLabel = NULL;

    tableString_Title.fn->destructor(&tableString_Title);
    tableString_SelectSource.fn->destructor(&tableString_SelectSource);
    tableString_SDCard.fn->destructor(&tableString_SDCard);
    tableString_USB.fn->destructor(&tableString_USB);
    tableString_NoValidMedium2.fn->destructor(&tableString_NoValidMedium2);
    tableString_NoValidMedium.fn->destructor(&tableString_NoValidMedium);
    tableString_FileNotFound1.fn->destructor(&tableString_FileNotFound1);
    tableString_FileNotFound2.fn->destructor(&tableString_FileNotFound2);
    tableString_Ok.fn->destructor(&tableString_Ok);
    tableString_Flashing.fn->destructor(&tableString_Flashing);
    tableString_stringNumberFiller.fn->destructor(&tableString_stringNumberFiller);
    tableString_Of.fn->destructor(&tableString_Of);
    showing = LE_FALSE;
}

void screenDestroy_default()
{

}

leWidget* screenGetRoot_default(uint32_t lyrIdx)
{
    if(lyrIdx >= LE_LAYER_COUNT)
        return NULL;

    switch(lyrIdx)
    {
        case 0:
        {
            return root0;
        }
        default:
        {
            return NULL;
        }
    }
}

