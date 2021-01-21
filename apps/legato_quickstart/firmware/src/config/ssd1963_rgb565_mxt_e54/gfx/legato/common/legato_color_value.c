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


#include "gfx/legato/common/legato_color.h"

static leColor values[LE_COLOR_LAST] =
{
    0x000000,  // LE_COLOR_BLACK
    0xFFFFFF,  // LE_COLOR_WHITE
    0xFF0000,  // LE_COLOR_RED
    0x00FF00,  // LE_COLOR_LIME
    0x0000FF,  // LE_COLOR_BLUE
    0xFFFF00,  // LE_COLOR_YELLOW
    0x00FFFF,  // LE_COLOR_CYAN
    0xFF00FF,  // LE_COLOR_MAGENTA
    0xC0C0C0,  // LE_COLOR_SILVER
    0x404040,  // LE_COLOR_DARKGRAY
    0x808080,  // LE_COLOR_GRAY
    0xD4D0C8,  // LE_COLOR_LIGHTGRAY
    0x800000,  // LE_COLOR_MAROON
    0x808000,  // LE_COLOR_OLIVE
    0x008000,  // LE_COLOR_GREEN
    0x800080,  // LE_COLOR_PURPLE
    0x008080,  // LE_COLOR_TEAL
    0x000080   // LE_COLOR_NAVY
};

leColor leColorValue(leColorMode mode, leColorName name)
{
    if(name < 0 || name >= LE_COLOR_LAST)
        return 0;

    return leColorConvert(LE_COLOR_MODE_RGB_888, mode, values[name]);
}
