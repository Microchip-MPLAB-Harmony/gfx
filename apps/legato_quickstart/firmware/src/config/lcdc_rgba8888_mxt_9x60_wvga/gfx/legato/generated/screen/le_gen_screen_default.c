#include "gfx/legato/generated/screen/le_gen_screen_default.h"

// widget list for layer 0
static leWidget* root0;

leWidget* default_Layer0_FillPanel;
leImageWidget* ImageWidget1;
leButtonWidget* ButtonWidget1;
leLabelWidget* LabelWidget0;
leLabelWidget* LabelWidget1;
leLabelWidget* LabelWidget2;

// string list for this screen
static leTableString tableString_Fast;
static leTableString tableString_Easy;
static leTableString tableString_Smart;

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
    leTableString_Constructor(&tableString_Fast, string_Fast);
    leTableString_Constructor(&tableString_Easy, string_Easy);
    leTableString_Constructor(&tableString_Smart, string_Smart);

    // layer 0
    root0 = leWidget_New();
    root0->fn->setPosition(root0, 0, 0);
    root0->fn->setSize(root0, 800, 480);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);

    default_Layer0_FillPanel = leWidget_New();
    default_Layer0_FillPanel->fn->setPosition(default_Layer0_FillPanel, 0, 0);
    default_Layer0_FillPanel->fn->setSize(default_Layer0_FillPanel, 800, 480);
    default_Layer0_FillPanel->fn->setScheme(default_Layer0_FillPanel, &LayerScheme);
    root0->fn->addChild(root0, default_Layer0_FillPanel);

    ImageWidget1 = leImageWidget_New();
    ImageWidget1->fn->setPosition(ImageWidget1, 50, 81);
    ImageWidget1->fn->setSize(ImageWidget1, 320, 300);
    ImageWidget1->fn->setBackgroundType(ImageWidget1, LE_WIDGET_BACKGROUND_NONE);
    ImageWidget1->fn->setImage(ImageWidget1, &MHGS_logo_WVGA);
    root0->fn->addChild(root0, (leWidget*)ImageWidget1);

    ButtonWidget1 = leButtonWidget_New();
    ButtonWidget1->fn->setPosition(ButtonWidget1, 435, 220);
    ButtonWidget1->fn->setSize(ButtonWidget1, 260, 120);
    ButtonWidget1->fn->setBackgroundType(ButtonWidget1, LE_WIDGET_BACKGROUND_NONE);
    ButtonWidget1->fn->setBorderType(ButtonWidget1, LE_WIDGET_BORDER_NONE);
    ButtonWidget1->fn->setPressedImage(ButtonWidget1, &QuickstartDown_WVGA);
    ButtonWidget1->fn->setReleasedImage(ButtonWidget1, &QuickstartUp_WVGA);
    root0->fn->addChild(root0, (leWidget*)ButtonWidget1);

    LabelWidget0 = leLabelWidget_New();
    LabelWidget0->fn->setPosition(LabelWidget0, 390, 145);
    LabelWidget0->fn->setSize(LabelWidget0, 100, 50);
    LabelWidget0->fn->setScheme(LabelWidget0, &RedScheme);
    LabelWidget0->fn->setBackgroundType(LabelWidget0, LE_WIDGET_BACKGROUND_NONE);
    LabelWidget0->fn->setString(LabelWidget0, (leString*)&tableString_Fast);
    root0->fn->addChild(root0, (leWidget*)LabelWidget0);

    LabelWidget1 = leLabelWidget_New();
    LabelWidget1->fn->setPosition(LabelWidget1, 500, 145);
    LabelWidget1->fn->setSize(LabelWidget1, 120, 50);
    LabelWidget1->fn->setScheme(LabelWidget1, &GreenScheme);
    LabelWidget1->fn->setBackgroundType(LabelWidget1, LE_WIDGET_BACKGROUND_NONE);
    LabelWidget1->fn->setString(LabelWidget1, (leString*)&tableString_Easy);
    root0->fn->addChild(root0, (leWidget*)LabelWidget1);

    LabelWidget2 = leLabelWidget_New();
    LabelWidget2->fn->setPosition(LabelWidget2, 615, 145);
    LabelWidget2->fn->setSize(LabelWidget2, 140, 50);
    LabelWidget2->fn->setScheme(LabelWidget2, &BlueScheme);
    LabelWidget2->fn->setBackgroundType(LabelWidget2, LE_WIDGET_BACKGROUND_NONE);
    LabelWidget2->fn->setString(LabelWidget2, (leString*)&tableString_Smart);
    root0->fn->addChild(root0, (leWidget*)LabelWidget2);

    leAddRootWidget(root0, 0);

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

    default_Layer0_FillPanel = NULL;
    ImageWidget1 = NULL;
    ButtonWidget1 = NULL;
    LabelWidget0 = NULL;
    LabelWidget1 = NULL;
    LabelWidget2 = NULL;

    tableString_Fast.fn->destructor(&tableString_Fast);
    tableString_Easy.fn->destructor(&tableString_Easy);
    tableString_Smart.fn->destructor(&tableString_Smart);
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

