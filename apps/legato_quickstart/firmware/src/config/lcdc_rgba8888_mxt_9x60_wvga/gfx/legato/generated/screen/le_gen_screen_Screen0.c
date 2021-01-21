#include "gfx/legato/generated/screen/le_gen_screen_Screen0.h"

// screen member widget declarations
leWidget* root0;
leWidget* root1;
leWidget* root2;

leWidget* Screen0_default_Layer0_FillPanel;
leImageWidget* Screen0_ImageWidget0;
leLabelWidget* Screen0_LabelWidget2;
leLabelWidget* Screen0_LabelWidget1;
leLabelWidget* Screen0_LabelWidget0;
leButtonWidget* Screen0_ButtonWidget1;
leImageWidget* Screen0_ImageWidget1;

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

    // layer 0
    root0 = leWidget_New();
    root0->fn->setSize(root0, 800, 480);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);
    root0->flags |= LE_WIDGET_IGNOREEVENTS;
    root0->flags |= LE_WIDGET_IGNOREPICK;

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGBA_8888);

    // layer 1
    root1 = leWidget_New();
    root1->fn->setSize(root1, 800, 480);
    root1->fn->setBackgroundType(root1, LE_WIDGET_BACKGROUND_NONE);
    root1->fn->setMargins(root1, 0, 0, 0, 0);
    root1->flags |= LE_WIDGET_IGNOREEVENTS;
    root1->flags |= LE_WIDGET_IGNOREPICK;

    leAddRootWidget(root1, 1);
    leSetLayerColorMode(1, LE_COLOR_MODE_RGBA_8888);

    // layer 2
    root2 = leWidget_New();
    root2->fn->setSize(root2, 800, 480);
    root2->fn->setBackgroundType(root2, LE_WIDGET_BACKGROUND_NONE);
    root2->fn->setMargins(root2, 0, 0, 0, 0);
    root2->flags |= LE_WIDGET_IGNOREEVENTS;
    root2->flags |= LE_WIDGET_IGNOREPICK;

    Screen0_default_Layer0_FillPanel = leWidget_New();
    Screen0_default_Layer0_FillPanel->fn->setPosition(Screen0_default_Layer0_FillPanel, 0, 0);
    Screen0_default_Layer0_FillPanel->fn->setSize(Screen0_default_Layer0_FillPanel, 800, 480);
    Screen0_default_Layer0_FillPanel->fn->setScheme(Screen0_default_Layer0_FillPanel, &LayerScheme);
    root2->fn->addChild(root2, (leWidget*)Screen0_default_Layer0_FillPanel);

    Screen0_ImageWidget0 = leImageWidget_New();
    Screen0_ImageWidget0->fn->setPosition(Screen0_ImageWidget0, 11, 11);
    Screen0_ImageWidget0->fn->setSize(Screen0_ImageWidget0, 152, 40);
    Screen0_ImageWidget0->fn->setBackgroundType(Screen0_ImageWidget0, LE_WIDGET_BACKGROUND_NONE);
    Screen0_ImageWidget0->fn->setBorderType(Screen0_ImageWidget0, LE_WIDGET_BORDER_NONE);
    Screen0_ImageWidget0->fn->setImage(Screen0_ImageWidget0, (leImage*)&mchpLogo_light);
    root2->fn->addChild(root2, (leWidget*)Screen0_ImageWidget0);

    Screen0_LabelWidget2 = leLabelWidget_New();
    Screen0_LabelWidget2->fn->setPosition(Screen0_LabelWidget2, 630, 145);
    Screen0_LabelWidget2->fn->setSize(Screen0_LabelWidget2, 160, 50);
    Screen0_LabelWidget2->fn->setScheme(Screen0_LabelWidget2, &BlueScheme);
    Screen0_LabelWidget2->fn->setBackgroundType(Screen0_LabelWidget2, LE_WIDGET_BACKGROUND_NONE);
    Screen0_LabelWidget2->fn->setString(Screen0_LabelWidget2, (leString*)&string_Smart);
    root2->fn->addChild(root2, (leWidget*)Screen0_LabelWidget2);

    Screen0_LabelWidget1 = leLabelWidget_New();
    Screen0_LabelWidget1->fn->setPosition(Screen0_LabelWidget1, 505, 145);
    Screen0_LabelWidget1->fn->setSize(Screen0_LabelWidget1, 120, 50);
    Screen0_LabelWidget1->fn->setScheme(Screen0_LabelWidget1, &GreenScheme);
    Screen0_LabelWidget1->fn->setBackgroundType(Screen0_LabelWidget1, LE_WIDGET_BACKGROUND_NONE);
    Screen0_LabelWidget1->fn->setString(Screen0_LabelWidget1, (leString*)&string_Easy);
    root2->fn->addChild(root2, (leWidget*)Screen0_LabelWidget1);

    Screen0_LabelWidget0 = leLabelWidget_New();
    Screen0_LabelWidget0->fn->setPosition(Screen0_LabelWidget0, 390, 145);
    Screen0_LabelWidget0->fn->setSize(Screen0_LabelWidget0, 110, 50);
    Screen0_LabelWidget0->fn->setScheme(Screen0_LabelWidget0, &RedScheme);
    Screen0_LabelWidget0->fn->setBackgroundType(Screen0_LabelWidget0, LE_WIDGET_BACKGROUND_NONE);
    Screen0_LabelWidget0->fn->setString(Screen0_LabelWidget0, (leString*)&string_Fast);
    root2->fn->addChild(root2, (leWidget*)Screen0_LabelWidget0);

    Screen0_ButtonWidget1 = leButtonWidget_New();
    Screen0_ButtonWidget1->fn->setPosition(Screen0_ButtonWidget1, 435, 230);
    Screen0_ButtonWidget1->fn->setSize(Screen0_ButtonWidget1, 260, 120);
    Screen0_ButtonWidget1->fn->setBackgroundType(Screen0_ButtonWidget1, LE_WIDGET_BACKGROUND_NONE);
    Screen0_ButtonWidget1->fn->setBorderType(Screen0_ButtonWidget1, LE_WIDGET_BORDER_NONE);
    Screen0_ButtonWidget1->fn->setPressedImage(Screen0_ButtonWidget1, (leImage*)&QuickstartDown_WVGA);
    Screen0_ButtonWidget1->fn->setReleasedImage(Screen0_ButtonWidget1, (leImage*)&QuickstartUp_WVGA);
    root2->fn->addChild(root2, (leWidget*)Screen0_ButtonWidget1);

    Screen0_ImageWidget1 = leImageWidget_New();
    Screen0_ImageWidget1->fn->setPosition(Screen0_ImageWidget1, 45, 85);
    Screen0_ImageWidget1->fn->setSize(Screen0_ImageWidget1, 320, 300);
    Screen0_ImageWidget1->fn->setBackgroundType(Screen0_ImageWidget1, LE_WIDGET_BACKGROUND_NONE);
    Screen0_ImageWidget1->fn->setBorderType(Screen0_ImageWidget1, LE_WIDGET_BORDER_NONE);
    Screen0_ImageWidget1->fn->setImage(Screen0_ImageWidget1, (leImage*)&MHGS_logo_WVGA);
    root2->fn->addChild(root2, (leWidget*)Screen0_ImageWidget1);

    leAddRootWidget(root2, 2);
    leSetLayerColorMode(2, LE_COLOR_MODE_RGBA_8888);

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

    leRemoveRootWidget(root1, 1);
    leWidget_Delete(root1);
    root1 = NULL;

    leRemoveRootWidget(root2, 2);
    leWidget_Delete(root2);
    root2 = NULL;

    Screen0_default_Layer0_FillPanel = NULL;
    Screen0_ImageWidget0 = NULL;
    Screen0_LabelWidget2 = NULL;
    Screen0_LabelWidget1 = NULL;
    Screen0_LabelWidget0 = NULL;
    Screen0_ButtonWidget1 = NULL;
    Screen0_ImageWidget1 = NULL;


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
        case 1:
        {
            return root1;
        }
        case 2:
        {
            return root2;
        }
        default:
        {
            return NULL;
        }
    }
}

