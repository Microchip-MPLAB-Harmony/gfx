#include "gfx/legato/generated/le_gen_scheme.h"

leScheme DefaultScheme;

void legato_initialize_schemes(void)
{
    leScheme_Initialize(&DefaultScheme, LE_COLOR_MODE_RGB_565);
    DefaultScheme.base = 0xFFFF;
    DefaultScheme.highlight = 0xC67A;
    DefaultScheme.highlightLight = 0xFFFF;
    DefaultScheme.shadow = 0x8410;
    DefaultScheme.shadowDark = 0x4208;
    DefaultScheme.foreground = 0x0000;
    DefaultScheme.foregroundInactive = 0xD71C;
    DefaultScheme.foregroundDisabled = 0x8410;
    DefaultScheme.background = 0xFFFF;
    DefaultScheme.backgroundInactive = 0xD71C;
    DefaultScheme.backgroundDisabled = 0xC67A;
    DefaultScheme.text = 0x0000;
    DefaultScheme.textHighlight = 0x001F;
    DefaultScheme.textHighlightText = 0xFFFF;
    DefaultScheme.textInactive = 0xD71C;
    DefaultScheme.textDisabled = 0x8C92;

}
