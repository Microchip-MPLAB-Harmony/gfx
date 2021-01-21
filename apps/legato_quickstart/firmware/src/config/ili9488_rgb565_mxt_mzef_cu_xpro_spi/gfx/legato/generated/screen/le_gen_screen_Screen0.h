#ifndef LE_GEN_SCREEN_SCREEN0_H
#define LE_GEN_SCREEN_SCREEN0_H

#include "gfx/legato/generated/le_gen_init.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

// widget list for this screen
// layer 0
extern leImageWidget* ImageWidget0;
extern leLabelWidget* LabelWidget0;
extern leImageWidget* ImageWidget1;
extern leButtonWidget* ButtonWidget0;


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
