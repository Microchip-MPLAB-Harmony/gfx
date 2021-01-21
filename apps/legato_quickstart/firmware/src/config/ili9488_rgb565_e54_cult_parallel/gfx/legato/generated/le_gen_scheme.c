// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#include "gfx/legato/generated/le_gen_scheme.h"

leScheme NewScheme;

void legato_initialize_schemes(void)
{
    leScheme_Initialize(&NewScheme, LE_COLOR_MODE_RGB_565);
    NewScheme.base = 0xFFFF;
    NewScheme.highlight = 0xC67A;
    NewScheme.highlightLight = 0xFFFF;
    NewScheme.shadow = 0x8410;
    NewScheme.shadowDark = 0x4208;
    NewScheme.foreground = 0x0000;
    NewScheme.foregroundInactive = 0xD71C;
    NewScheme.foregroundDisabled = 0x8410;
    NewScheme.background = 0xFFFF;
    NewScheme.backgroundInactive = 0xD71C;
    NewScheme.backgroundDisabled = 0xC67A;
    NewScheme.text = 0x0000;
    NewScheme.textHighlight = 0x001F;
    NewScheme.textHighlightText = 0xFFFF;
    NewScheme.textInactive = 0xD71C;
    NewScheme.textDisabled = 0x8C92;

}
