#include "gfx/legato/generated/screen/le_gen_screen_default.h"

// screen member widget declarations
leWidget* root0;

leWidget* default_PanelWidget0;
leImageWidget* default_ImageWidget0;
leImageWidget* default_ImageWidget1;
leButtonWidget* default_ButtonWidget0;
leLabelWidget* default_LabelWidget0;
leLabelWidget* default_LabelWidget1;
leLabelWidget* default_LabelWidget2;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_default()
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_default()
{
    if(showing == LE_TRUE)
        return LE_FAILURE;

    // layer 0
    root0 = leWidget_New();
    root0->fn->setSize(root0, 480, 272);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);
    root0->flags |= LE_WIDGET_IGNOREEVENTS;
    root0->flags |= LE_WIDGET_IGNOREPICK;

    default_PanelWidget0 = leWidget_New();
    default_PanelWidget0->fn->setPosition(default_PanelWidget0, 0, 0);
    default_PanelWidget0->fn->setSize(default_PanelWidget0, 480, 272);
    default_PanelWidget0->fn->setScheme(default_PanelWidget0, &WhiteScheme);
    root0->fn->addChild(root0, (leWidget*)default_PanelWidget0);

    default_ImageWidget0 = leImageWidget_New();
    default_ImageWidget0->fn->setPosition(default_ImageWidget0, 8, 6);
    default_ImageWidget0->fn->setSize(default_ImageWidget0, 120, 28);
    default_ImageWidget0->fn->setBackgroundType(default_ImageWidget0, LE_WIDGET_BACKGROUND_NONE);
    default_ImageWidget0->fn->setBorderType(default_ImageWidget0, LE_WIDGET_BORDER_NONE);
    default_ImageWidget0->fn->setImage(default_ImageWidget0, (leImage*)&mchpLogo_small);
    root0->fn->addChild(root0, (leWidget*)default_ImageWidget0);

    default_ImageWidget1 = leImageWidget_New();
    default_ImageWidget1->fn->setPosition(default_ImageWidget1, 30, 60);
    default_ImageWidget1->fn->setSize(default_ImageWidget1, 180, 169);
    default_ImageWidget1->fn->setBackgroundType(default_ImageWidget1, LE_WIDGET_BACKGROUND_NONE);
    default_ImageWidget1->fn->setBorderType(default_ImageWidget1, LE_WIDGET_BORDER_NONE);
    default_ImageWidget1->fn->setImage(default_ImageWidget1, (leImage*)&MHGS_logo_smaller);
    root0->fn->addChild(root0, (leWidget*)default_ImageWidget1);

    default_ButtonWidget0 = leButtonWidget_New();
    default_ButtonWidget0->fn->setPosition(default_ButtonWidget0, 256, 142);
    default_ButtonWidget0->fn->setSize(default_ButtonWidget0, 160, 74);
    default_ButtonWidget0->fn->setBackgroundType(default_ButtonWidget0, LE_WIDGET_BACKGROUND_NONE);
    default_ButtonWidget0->fn->setBorderType(default_ButtonWidget0, LE_WIDGET_BORDER_NONE);
    default_ButtonWidget0->fn->setPressedImage(default_ButtonWidget0, (leImage*)&QuickstartDown_WQVGA);
    default_ButtonWidget0->fn->setReleasedImage(default_ButtonWidget0, (leImage*)&QuickstartUp_WQVGA);
    root0->fn->addChild(root0, (leWidget*)default_ButtonWidget0);

    default_LabelWidget0 = leLabelWidget_New();
    default_LabelWidget0->fn->setPosition(default_LabelWidget0, 225, 87);
    default_LabelWidget0->fn->setSize(default_LabelWidget0, 65, 40);
    default_LabelWidget0->fn->setScheme(default_LabelWidget0, &RedScheme);
    default_LabelWidget0->fn->setBackgroundType(default_LabelWidget0, LE_WIDGET_BACKGROUND_NONE);
    default_LabelWidget0->fn->setString(default_LabelWidget0, (leString*)&string_Fast);
    root0->fn->addChild(root0, (leWidget*)default_LabelWidget0);

    default_LabelWidget1 = leLabelWidget_New();
    default_LabelWidget1->fn->setPosition(default_LabelWidget1, 300, 87);
    default_LabelWidget1->fn->setSize(default_LabelWidget1, 70, 40);
    default_LabelWidget1->fn->setScheme(default_LabelWidget1, &GreenScheme);
    default_LabelWidget1->fn->setBackgroundType(default_LabelWidget1, LE_WIDGET_BACKGROUND_NONE);
    default_LabelWidget1->fn->setString(default_LabelWidget1, (leString*)&string_Easy);
    root0->fn->addChild(root0, (leWidget*)default_LabelWidget1);

    default_LabelWidget2 = leLabelWidget_New();
    default_LabelWidget2->fn->setPosition(default_LabelWidget2, 375, 87);
    default_LabelWidget2->fn->setSize(default_LabelWidget2, 100, 40);
    default_LabelWidget2->fn->setScheme(default_LabelWidget2, &BlueScheme);
    default_LabelWidget2->fn->setBackgroundType(default_LabelWidget2, LE_WIDGET_BACKGROUND_NONE);
    default_LabelWidget2->fn->setString(default_LabelWidget2, (leString*)&string_Smart);
    root0->fn->addChild(root0, (leWidget*)default_LabelWidget2);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGB_565);

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_default()
{
}

void screenHide_default()
{

    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    default_PanelWidget0 = NULL;
    default_ImageWidget0 = NULL;
    default_ImageWidget1 = NULL;
    default_ButtonWidget0 = NULL;
    default_LabelWidget0 = NULL;
    default_LabelWidget1 = NULL;
    default_LabelWidget2 = NULL;


    showing = LE_FALSE;
}

void screenDestroy_default()
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
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

