#include "gfx/legato/generated/screen/le_gen_screen_screen1.h"

// screen member widget declarations
leWidget* root0;

leWidget* screen1_Layer_0_FillPanel;
leLabelWidget* screen1_TitleLabel;
leButtonWidget* screen1_SloganButton;
leButtonWidget* screen1_LogoButton;
leLabelWidget* screen1_ImageTypeLabelWidget;

// string list for this screen
static leTableString string_TitleString;
static leTableString string_DrawRLE;
static leTableString string_ImageIsDirectBlit;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_screen1()
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_screen1()
{
    if(showing == LE_TRUE)
        return LE_FAILURE;

    // initialize static strings
    leTableString_Constructor(&string_TitleString, stringID_TitleString);
    leTableString_Constructor(&string_DrawRLE, stringID_DrawRLE);
    leTableString_Constructor(&string_ImageIsDirectBlit, stringID_ImageIsDirectBlit);

    // layer 0
    root0 = leWidget_New();
    root0->fn->setPosition(root0, 0, 0);
    root0->fn->setSize(root0, 480, 272);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);

    screen1_Layer_0_FillPanel = leWidget_New();
    screen1_Layer_0_FillPanel->fn->setPosition(screen1_Layer_0_FillPanel, 0, 0);
    screen1_Layer_0_FillPanel->fn->setSize(screen1_Layer_0_FillPanel, 480, 272);
    screen1_Layer_0_FillPanel->fn->setScheme(screen1_Layer_0_FillPanel, &AriaImport_LayerFillScheme);
    root0->fn->addChild(root0, (leWidget*)screen1_Layer_0_FillPanel);

    screen1_TitleLabel = leLabelWidget_New();
    screen1_TitleLabel->fn->setPosition(screen1_TitleLabel, 13, 4);
    screen1_TitleLabel->fn->setSize(screen1_TitleLabel, 455, 55);
    screen1_TitleLabel->fn->setBackgroundType(screen1_TitleLabel, LE_WIDGET_BACKGROUND_NONE);
    screen1_TitleLabel->fn->setHAlignment(screen1_TitleLabel, LE_HALIGN_CENTER);
    screen1_TitleLabel->fn->setString(screen1_TitleLabel, (leString*)&string_TitleString);
    root0->fn->addChild(root0, (leWidget*)screen1_TitleLabel);

    screen1_SloganButton = leButtonWidget_New();
    screen1_SloganButton->fn->setPosition(screen1_SloganButton, 9, 204);
    screen1_SloganButton->fn->setSize(screen1_SloganButton, 464, 47);
    screen1_SloganButton->fn->setString(screen1_SloganButton, (leString*)&string_DrawRLE);
    screen1_SloganButton->fn->setReleasedEventCallback(screen1_SloganButton, event_screen1_SloganButton_OnReleased);
    root0->fn->addChild(root0, (leWidget*)screen1_SloganButton);

    screen1_LogoButton = leButtonWidget_New();
    screen1_LogoButton->fn->setPosition(screen1_LogoButton, 82, 62);
    screen1_LogoButton->fn->setSize(screen1_LogoButton, 150, 130);
    screen1_LogoButton->fn->setBackgroundType(screen1_LogoButton, LE_WIDGET_BACKGROUND_NONE);
    screen1_LogoButton->fn->setBorderType(screen1_LogoButton, LE_WIDGET_BORDER_NONE);
    screen1_LogoButton->fn->setPressedImage(screen1_LogoButton, (leImage*)&MHGS_logo_small_raw_direct_blit);
    screen1_LogoButton->fn->setReleasedImage(screen1_LogoButton, (leImage*)&MHGS_logo_small_raw_direct_blit);
    screen1_LogoButton->fn->setPressedOffset(screen1_LogoButton, 0);
    screen1_LogoButton->fn->setReleasedEventCallback(screen1_LogoButton, event_screen1_LogoButton_OnReleased);
    root0->fn->addChild(root0, (leWidget*)screen1_LogoButton);

    screen1_ImageTypeLabelWidget = leLabelWidget_New();
    screen1_ImageTypeLabelWidget->fn->setPosition(screen1_ImageTypeLabelWidget, 251, 95);
    screen1_ImageTypeLabelWidget->fn->setSize(screen1_ImageTypeLabelWidget, 215, 51);
    screen1_ImageTypeLabelWidget->fn->setBackgroundType(screen1_ImageTypeLabelWidget, LE_WIDGET_BACKGROUND_NONE);
    screen1_ImageTypeLabelWidget->fn->setHAlignment(screen1_ImageTypeLabelWidget, LE_HALIGN_CENTER);
    screen1_ImageTypeLabelWidget->fn->setString(screen1_ImageTypeLabelWidget, (leString*)&string_ImageIsDirectBlit);
    root0->fn->addChild(root0, (leWidget*)screen1_ImageTypeLabelWidget);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

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

    screen1_Layer_0_FillPanel = NULL;
    screen1_TitleLabel = NULL;
    screen1_SloganButton = NULL;
    screen1_LogoButton = NULL;
    screen1_ImageTypeLabelWidget = NULL;

    string_TitleString.fn->destructor(&string_TitleString);
    string_DrawRLE.fn->destructor(&string_DrawRLE);
    string_ImageIsDirectBlit.fn->destructor(&string_ImageIsDirectBlit);


    showing = LE_FALSE;
}

void screenDestroy_screen1()
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
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

