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

#include "gfx/legato/generated/screen/le_gen_screen_screen1.h"

// widget list for layer 0
static leWidget* root0;

leWidget* Layer0_FillPanel;
leLabelWidget* TitleLabel;
leButtonWidget* SloganButton;
leButtonWidget* LogoButton;
leLabelWidget* ImageTypeLabelWidget;

// string list for this screen
static leTableString tableString_TitleString;
static leTableString tableString_DrawRLE;
static leTableString tableString_ImageIsPaletteCompressed;

static leBool showing = LE_FALSE;

leResult screenInit_screen1()
{
    return LE_SUCCESS;
}

leResult screenShow_screen1()
{
    if(showing == LE_TRUE)
        return LE_FAILURE;

    // initialize static strings
    leTableString_Constructor(&tableString_TitleString, string_TitleString);
    leTableString_Constructor(&tableString_DrawRLE, string_DrawRLE);
    leTableString_Constructor(&tableString_ImageIsPaletteCompressed, string_ImageIsPaletteCompressed);

    // layer 0
    root0 = leWidget_New();
    root0->fn->setPosition(root0, 0, 0);
    root0->fn->setSize(root0, 480, 320);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);

    Layer0_FillPanel = leWidget_New();
    Layer0_FillPanel->fn->setPosition(Layer0_FillPanel, 0, 0);
    Layer0_FillPanel->fn->setSize(Layer0_FillPanel, 480, 320);
    Layer0_FillPanel->fn->setScheme(Layer0_FillPanel, &AriaImport_LayerScheme);
    root0->fn->addChild(root0, Layer0_FillPanel);

    TitleLabel = leLabelWidget_New();
    TitleLabel->fn->setPosition(TitleLabel, 16, 20);
    TitleLabel->fn->setSize(TitleLabel, 455, 55);
    TitleLabel->fn->setBackgroundType(TitleLabel, LE_WIDGET_BACKGROUND_NONE);
    TitleLabel->fn->setHAlignment(TitleLabel, LE_HALIGN_CENTER);
    TitleLabel->fn->setString(TitleLabel, (leString*)&tableString_TitleString);
    root0->fn->addChild(root0, (leWidget*)TitleLabel);

    SloganButton = leButtonWidget_New();
    SloganButton->fn->setPosition(SloganButton, 10, 261);
    SloganButton->fn->setSize(SloganButton, 464, 50);
    SloganButton->fn->setString(SloganButton, (leString*)&tableString_DrawRLE);
    SloganButton->fn->setReleasedEventCallback(SloganButton, SloganButton_OnReleased);
    root0->fn->addChild(root0, (leWidget*)SloganButton);

    LogoButton = leButtonWidget_New();
    LogoButton->fn->setPosition(LogoButton, 71, 90);
    LogoButton->fn->setSize(LogoButton, 150, 130);
    LogoButton->fn->setBackgroundType(LogoButton, LE_WIDGET_BACKGROUND_NONE);
    LogoButton->fn->setBorderType(LogoButton, LE_WIDGET_BORDER_NONE);
    LogoButton->fn->setPressedImage(LogoButton, &MHGS_logo_small_compressed);
    LogoButton->fn->setReleasedImage(LogoButton, &MHGS_logo_small_compressed);
    LogoButton->fn->setPressedOffset(LogoButton, 0);
    LogoButton->fn->setReleasedEventCallback(LogoButton, LogoButton_OnReleased);
    root0->fn->addChild(root0, (leWidget*)LogoButton);

    ImageTypeLabelWidget = leLabelWidget_New();
    ImageTypeLabelWidget->fn->setPosition(ImageTypeLabelWidget, 236, 130);
    ImageTypeLabelWidget->fn->setSize(ImageTypeLabelWidget, 215, 51);
    ImageTypeLabelWidget->fn->setBackgroundType(ImageTypeLabelWidget, LE_WIDGET_BACKGROUND_NONE);
    ImageTypeLabelWidget->fn->setHAlignment(ImageTypeLabelWidget, LE_HALIGN_CENTER);
    ImageTypeLabelWidget->fn->setString(ImageTypeLabelWidget, (leString*)&tableString_ImageIsPaletteCompressed);
    root0->fn->addChild(root0, (leWidget*)ImageTypeLabelWidget);

    leAddRootWidget(root0, 0);

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_screen1()
{
}

void screenHide_screen1()
{
    leRemoveRootWidget(root0, 0);

    leWidget_Delete(root0);

    root0 = NULL;

    Layer0_FillPanel = NULL;
    TitleLabel = NULL;
    SloganButton = NULL;
    LogoButton = NULL;
    ImageTypeLabelWidget = NULL;

    tableString_TitleString.fn->destructor(&tableString_TitleString);
    tableString_DrawRLE.fn->destructor(&tableString_DrawRLE);
    tableString_ImageIsPaletteCompressed.fn->destructor(&tableString_ImageIsPaletteCompressed);
    showing = LE_FALSE;
}

void screenDestroy_screen1()
{

}

leWidget* screenGetRoot_screen1(uint32_t lyrIdx)
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

