#include "gfx/legato/generated/screen/le_gen_screen_Screen0.h"

// screen member widget declarations
leWidget* root0;

leWidget* Screen0_Layer0_FillPanel;
leLabelWidget* Screen0_TitleLabel;
leButtonWidget* Screen0_SloganButton;
leButtonWidget* Screen0_LogoButton;
leLabelWidget* Screen0_ImageTypeLabelWidget;

// string list for this screen
static leTableString string_TitleString;
static leTableString string_DrawRLE;
static leTableString string_ImageIsPaletteCompressed;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_Screen0()
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_Screen0()
{
    if(showing == LE_TRUE)
        return LE_FAILURE;

    // initialize static strings
    leTableString_Constructor(&string_TitleString, stringID_TitleString);
    leTableString_Constructor(&string_DrawRLE, stringID_DrawRLE);
    leTableString_Constructor(&string_ImageIsPaletteCompressed, stringID_ImageIsPaletteCompressed);

    // layer 0
    root0 = leWidget_New();
    root0->fn->setPosition(root0, 0, 0);
    root0->fn->setSize(root0, 480, 320);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);

    Screen0_Layer0_FillPanel = leWidget_New();
    Screen0_Layer0_FillPanel->fn->setPosition(Screen0_Layer0_FillPanel, 0, 0);
    Screen0_Layer0_FillPanel->fn->setSize(Screen0_Layer0_FillPanel, 480, 272);
    Screen0_Layer0_FillPanel->fn->setScheme(Screen0_Layer0_FillPanel, &AriaImport_LayerScheme);
    root0->fn->addChild(root0, (leWidget*)Screen0_Layer0_FillPanel);

    Screen0_TitleLabel = leLabelWidget_New();
    Screen0_TitleLabel->fn->setPosition(Screen0_TitleLabel, 16, 0);
    Screen0_TitleLabel->fn->setSize(Screen0_TitleLabel, 455, 55);
    Screen0_TitleLabel->fn->setBackgroundType(Screen0_TitleLabel, LE_WIDGET_BACKGROUND_NONE);
    Screen0_TitleLabel->fn->setHAlignment(Screen0_TitleLabel, LE_HALIGN_CENTER);
    Screen0_TitleLabel->fn->setString(Screen0_TitleLabel, (leString*)&string_TitleString);
    root0->fn->addChild(root0, (leWidget*)Screen0_TitleLabel);

    Screen0_SloganButton = leButtonWidget_New();
    Screen0_SloganButton->fn->setPosition(Screen0_SloganButton, 10, 210);
    Screen0_SloganButton->fn->setSize(Screen0_SloganButton, 464, 50);
    Screen0_SloganButton->fn->setString(Screen0_SloganButton, (leString*)&string_DrawRLE);
    root0->fn->addChild(root0, (leWidget*)Screen0_SloganButton);

    Screen0_LogoButton = leButtonWidget_New();
    Screen0_LogoButton->fn->setPosition(Screen0_LogoButton, 71, 60);
    Screen0_LogoButton->fn->setSize(Screen0_LogoButton, 150, 130);
    Screen0_LogoButton->fn->setBackgroundType(Screen0_LogoButton, LE_WIDGET_BACKGROUND_NONE);
    Screen0_LogoButton->fn->setBorderType(Screen0_LogoButton, LE_WIDGET_BORDER_NONE);
    Screen0_LogoButton->fn->setPressedImage(Screen0_LogoButton, (leImage*)&MHGS_logo_small_compressed);
    Screen0_LogoButton->fn->setReleasedImage(Screen0_LogoButton, (leImage*)&MHGS_logo_small_compressed);
    Screen0_LogoButton->fn->setPressedOffset(Screen0_LogoButton, 0);
    root0->fn->addChild(root0, (leWidget*)Screen0_LogoButton);

    Screen0_ImageTypeLabelWidget = leLabelWidget_New();
    Screen0_ImageTypeLabelWidget->fn->setPosition(Screen0_ImageTypeLabelWidget, 236, 100);
    Screen0_ImageTypeLabelWidget->fn->setSize(Screen0_ImageTypeLabelWidget, 215, 51);
    Screen0_ImageTypeLabelWidget->fn->setBackgroundType(Screen0_ImageTypeLabelWidget, LE_WIDGET_BACKGROUND_NONE);
    Screen0_ImageTypeLabelWidget->fn->setHAlignment(Screen0_ImageTypeLabelWidget, LE_HALIGN_CENTER);
    Screen0_ImageTypeLabelWidget->fn->setString(Screen0_ImageTypeLabelWidget, (leString*)&string_ImageIsPaletteCompressed);
    root0->fn->addChild(root0, (leWidget*)Screen0_ImageTypeLabelWidget);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_Screen0()
{
}

void screenHide_Screen0()
{

    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    Screen0_Layer0_FillPanel = NULL;
    Screen0_TitleLabel = NULL;
    Screen0_SloganButton = NULL;
    Screen0_LogoButton = NULL;
    Screen0_ImageTypeLabelWidget = NULL;

    string_TitleString.fn->destructor(&string_TitleString);
    string_DrawRLE.fn->destructor(&string_DrawRLE);
    string_ImageIsPaletteCompressed.fn->destructor(&string_ImageIsPaletteCompressed);


    showing = LE_FALSE;
}

void screenDestroy_Screen0()
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_Screen0(uint32_t lyrIdx)
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

