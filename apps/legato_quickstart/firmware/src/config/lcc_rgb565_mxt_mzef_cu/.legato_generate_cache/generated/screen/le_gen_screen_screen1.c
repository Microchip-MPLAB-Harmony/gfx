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
    root0->fn->setSize(root0, 480, 272);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);

    PanelWidget0 = leWidget_New();
    PanelWidget0->fn->setPosition(PanelWidget0, 0, 0);
    PanelWidget0->fn->setSize(PanelWidget0, 480, 272);
    PanelWidget0->fn->setScheme(PanelWidget0, &DefaultScheme);
    root0->fn->addChild(root0, PanelWidget0);

    LogoWidget = leImageWidget_New();
    LogoWidget->fn->setPosition(LogoWidget, 161, 51);
    LogoWidget->fn->setSize(LogoWidget, 150, 150);
    LogoWidget->fn->setBackgroundType(LogoWidget, LE_WIDGET_BACKGROUND_NONE);
    LogoWidget->fn->setImage(LogoWidget, &MHGC_150x150);
    root0->fn->addChild(root0, (leWidget*)LogoWidget);

    TitleLabel = leLabelWidget_New();
    TitleLabel->fn->setPosition(TitleLabel, 71, 18);
    TitleLabel->fn->setSize(TitleLabel, 343, 25);
    TitleLabel->fn->setBackgroundType(TitleLabel, LE_WIDGET_BACKGROUND_NONE);
    TitleLabel->fn->setHAlignment(TitleLabel, LE_HALIGN_CENTER);
    TitleLabel->fn->setString(TitleLabel, (leString*)&tableString_TitleString);
    root0->fn->addChild(root0, (leWidget*)TitleLabel);

    SloganButton = leButtonWidget_New();
    SloganButton->fn->setPosition(SloganButton, 119, 211);
    SloganButton->fn->setSize(SloganButton, 238, 37);
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

