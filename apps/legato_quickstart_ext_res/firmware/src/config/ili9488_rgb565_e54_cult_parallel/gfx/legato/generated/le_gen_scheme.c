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

leScheme AriaImport_LayerScheme;

void legato_initialize_schemes(void)
{
    leScheme_Initialize(&AriaImport_LayerScheme, LE_COLOR_MODE_RGB_565);
    AriaImport_LayerScheme.base = 0xFFFF;
    AriaImport_LayerScheme.highlight = 0xC67A;
    AriaImport_LayerScheme.highlightLight = 0xFFFF;
    AriaImport_LayerScheme.shadow = 0x8410;
    AriaImport_LayerScheme.shadowDark = 0x4208;
    AriaImport_LayerScheme.foreground = 0x0000;
    AriaImport_LayerScheme.foregroundInactive = 0xD71C;
    AriaImport_LayerScheme.foregroundDisabled = 0x8410;
    AriaImport_LayerScheme.background = 0xFFFF;
    AriaImport_LayerScheme.backgroundInactive = 0xD71C;
    AriaImport_LayerScheme.backgroundDisabled = 0xC67A;
    AriaImport_LayerScheme.text = 0x0000;
    AriaImport_LayerScheme.textHighlight = 0x001F;
    AriaImport_LayerScheme.textHighlightText = 0xFFFF;
    AriaImport_LayerScheme.textInactive = 0xD71C;
    AriaImport_LayerScheme.textDisabled = 0x8C92;

}
