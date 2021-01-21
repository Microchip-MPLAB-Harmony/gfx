#include "gfx/legato/generated/screen/le_gen_screen_Screen0.h"

// widget list for layer 0
static leWidget* root0;

leImageWidget* ImageWidget0;
leLabelWidget* LabelWidget0;
leImageWidget* ImageWidget1;
leButtonWidget* ButtonWidget0;

// string list for this screen
static leTableString tableString_MPLABHarmonyComposer;
static leTableString tableString_ModifyRegenerateRun;

static leBool showing = LE_FALSE;

leResult screenInit_Screen0()
{
    return LE_SUCCESS;
}

leResult screenShow_Screen0()
{
    if(showing == LE_TRUE)
        return LE_FAILURE;

    // initialize static strings
    leTableString_Constructor(&tableString_MPLABHarmonyComposer, string_MPLABHarmonyComposer);
    leTableString_Constructor(&tableString_ModifyRegenerateRun, string_ModifyRegenerateRun);

    // layer 0
    root0 = leWidget_New();
    root0->fn->setPosition(root0, 0, 0);
    root0->fn->setSize(root0, 320, 480);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);

    ImageWidget0 = leImageWidget_New();
    ImageWidget0->fn->setPosition(ImageWidget0, 0, 0);
    ImageWidget0->fn->setSize(ImageWidget0, 320, 480);
    ImageWidget0->fn->setScheme(ImageWidget0, &NewScheme);
    root0->fn->addChild(root0, (leWidget*)ImageWidget0);

    LabelWidget0 = leLabelWidget_New();
    LabelWidget0->fn->setPosition(LabelWidget0, 6, 28);
    LabelWidget0->fn->setSize(LabelWidget0, 315, 45);
    LabelWidget0->fn->setBackgroundType(LabelWidget0, LE_WIDGET_BACKGROUND_NONE);
    LabelWidget0->fn->setHAlignment(LabelWidget0, LE_HALIGN_CENTER);
    LabelWidget0->fn->setString(LabelWidget0, (leString*)&tableString_MPLABHarmonyComposer);
    root0->fn->addChild(root0, (leWidget*)LabelWidget0);

    ImageWidget1 = leImageWidget_New();
    ImageWidget1->fn->setPosition(ImageWidget1, 76, 123);
    ImageWidget1->fn->setSize(ImageWidget1, 170, 170);
    ImageWidget1->fn->setBackgroundType(ImageWidget1, LE_WIDGET_BACKGROUND_NONE);
    ImageWidget1->fn->setImage(ImageWidget1, &MHGC_170x170);
    root0->fn->addChild(root0, (leWidget*)ImageWidget1);

    ButtonWidget0 = leButtonWidget_New();
    ButtonWidget0->fn->setPosition(ButtonWidget0, 60, 370);
    ButtonWidget0->fn->setSize(ButtonWidget0, 209, 51);
    ButtonWidget0->fn->setString(ButtonWidget0, (leString*)&tableString_ModifyRegenerateRun);
    root0->fn->addChild(root0, (leWidget*)ButtonWidget0);

    leAddRootWidget(root0, 0);

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

    ImageWidget0 = NULL;
    LabelWidget0 = NULL;
    ImageWidget1 = NULL;
    ButtonWidget0 = NULL;

    tableString_MPLABHarmonyComposer.fn->destructor(&tableString_MPLABHarmonyComposer);
    tableString_ModifyRegenerateRun.fn->destructor(&tableString_ModifyRegenerateRun);
    showing = LE_FALSE;
}

void screenDestroy_Screen0()
{

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

