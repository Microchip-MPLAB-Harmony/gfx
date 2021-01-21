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
