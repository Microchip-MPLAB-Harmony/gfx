#ifndef LE_GEN_SCREEN_DEFAULT_H
#define LE_GEN_SCREEN_DEFAULT_H

#include "gfx/legato/legato.h"

#include "gfx/legato/generated/le_gen_scheme.h"
#include "gfx/legato/generated/le_gen_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// screen member widget declarations
extern leWidget* default_PanelWidget0;
extern leImageWidget* default_ImageWidget0;
extern leImageWidget* default_ImageWidget1;
extern leButtonWidget* default_ButtonWidget0;
extern leLabelWidget* default_LabelWidget0;
extern leLabelWidget* default_LabelWidget1;
extern leLabelWidget* default_LabelWidget2;

// screen lifecycle functions
// DO NOT CALL THESE DIRECTLY
leResult screenInit_default(); // called when Legato is initialized
leResult screenShow_default(); // called when screen is shown
void screenHide_default(); // called when screen is hidden
void screenDestroy_default(); // called when Legato is destroyed
void screenUpdate_default(); // called when Legato is updating

leWidget* screenGetRoot_default(uint32_t lyrIdx); // gets a root widget for this screen

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_GEN_SCREEN_DEFAULT_H
