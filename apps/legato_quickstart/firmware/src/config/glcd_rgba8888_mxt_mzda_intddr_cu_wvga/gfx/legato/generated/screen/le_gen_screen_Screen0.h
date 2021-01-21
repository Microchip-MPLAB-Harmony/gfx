#ifndef LE_GEN_SCREEN_SCREEN0_H
#define LE_GEN_SCREEN_SCREEN0_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* Screen0_default_Layer0_FillPanel;
extern leImageWidget* Screen0_ImageWidget1;
extern leButtonWidget* Screen0_ButtonWidget1;
extern leLabelWidget* Screen0_LabelWidget0;
extern leLabelWidget* Screen0_LabelWidget1;
extern leLabelWidget* Screen0_LabelWidget2;
extern leImageWidget* Screen0_ImageWidget0;

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_Screen0(); // called when Legato is initialized
leResult screenShow_Screen0(); // called when screen is shown
void screenHide_Screen0(); // called when screen is hidden
void screenDestroy_Screen0(); // called when Legato is destroyed
void screenUpdate_Screen0(); // called when Legato is updating

leWidget* screenGetRoot_Screen0(uint32_t lyrIdx); // gets a root widget for this screen

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_SCREEN0_H
