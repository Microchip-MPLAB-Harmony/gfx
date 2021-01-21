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

leWidget* PanelWidget0;
leImageWidget* LogoWidget;
leLabelWidget* TitleLabel;
leButtonWidget* SloganButton;

// string list for this screen
static leTableString tableString_TitleString;
static leTableString tableString_Slogan;

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
    leTableString_Constructor(&tableString_Slogan, string_Slogan);

    // layer 0
    root0 = leWidget_New();
    root0->fn->setPosition(root0, 0, 0);
    root0->fn->setSize(root0, 320, 480);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);

    PanelWidget0 = leWidget_New();
    PanelWidget0->fn->setPosition(PanelWidget0, 0, 0);
    PanelWidget0->fn->setSize(PanelWidget0, 320, 480);
    PanelWidget0->fn->setScheme(PanelWidget0, &NewScheme);
    root0->fn->addChild(root0, PanelWidget0);

    LogoWidget = leImageWidget_New();
    LogoWidget->fn->setPosition(LogoWidget, 48, 96);
    LogoWidget->fn->setSize(LogoWidget, 214, 204);
    LogoWidget->fn->setBackgroundType(LogoWidget, LE_WIDGET_BACKGROUND_NONE);
    LogoWidget->fn->setImage(LogoWidget, &MHGC_170x170);
    root0->fn->addChild(root0, (leWidget*)LogoWidget);

    TitleLabel = leLabelWidget_New();
    TitleLabel->fn->setPosition(TitleLabel, 0, 46);
    TitleLabel->fn->setSize(TitleLabel, 321, 26);
    TitleLabel->fn->setBackgroundType(TitleLabel, LE_WIDGET_BACKGROUND_NONE);
    TitleLabel->fn->setHAlignment(TitleLabel, LE_HALIGN_CENTER);
    TitleLabel->fn->setString(TitleLabel, (leString*)&tableString_TitleString);
    root0->fn->addChild(root0, (leWidget*)TitleLabel);

    SloganButton = leButtonWidget_New();
    SloganButton->fn->setPosition(SloganButton, 46, 354);
    SloganButton->fn->setSize(SloganButton, 238, 37);
    SloganButton->fn->setScheme(SloganButton, &NewScheme_1);
    SloganButton->fn->setString(SloganButton, (leString*)&tableString_Slogan);
    root0->fn->addChild(root0, (leWidget*)SloganButton);

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
    PanelWidget0 = NULL;
    LogoWidget = NULL;
    TitleLabel = NULL;
    SloganButton = NULL;

    tableString_TitleString.fn->destructor(&tableString_TitleString);
    tableString_Slogan.fn->destructor(&tableString_Slogan);

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

