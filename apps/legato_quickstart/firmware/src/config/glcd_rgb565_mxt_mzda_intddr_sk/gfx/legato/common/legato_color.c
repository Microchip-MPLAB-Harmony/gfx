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

const leColorModeInfo leColorInfoTable[] =
{
    
    {1,8,  LE_BPP8,  {0x0,0x0,0x0,0x0},{0x0,0x0,0x0,0x0}},                        // LE_COLOR_MODE_GS_8
    {1,8,  LE_BPP8,  {0xE0,0x1C,0x3,0x0},{0x0,0x0,0x0,0x0}},                      // LE_COLOR_MODE_RGB_332
    {2,16, LE_BPP16, {0xF800,0x07E0,0x001F,0x0},{11,5,0,0}},                      // LE_COLOR_MODE_RGB_565
    {2,16, LE_BPP16, {0xF800,0x07C0,0x003E,0x1},{11,6,1,0}},                      // LE_COLOR_MODE_RGBA_5551
    {3,24, LE_BPP24, {0xFF0000,0x00FF00,0x0000FF,0x0},{16,8,0,0}},                // LE_COLOR_MODE_RGB_888
    {4,32, LE_BPP32, {0xFF000000,0x00FF0000,0x0000FF00,0x000000FF},{24,16,8,0}},  // LE_COLOR_MODE_RGBA_8888
    {4,32, LE_BPP32, {0x00FF0000,0x0000FF00,0x000000FF,0xFF000000},{16,8,0,24}},  // LE_COLOR_MODE_ARGB_8888
    {1,1,  LE_BPP1,  {0x0,0x0,0x0,0x0},{0x0,0x0,0x0,0x0}},                        // LE_COLOR_MODE_INDEX_1
    {1,4,  LE_BPP4,  {0x0,0x0,0x0,0x0},{0x0,0x0,0x0,0x0}},                        // LE_COLOR_MODE_INDEX_4
    {1,8,  LE_BPP8,  {0x0,0x0,0x0,0x0},{0x0,0x0,0x0,0x0}},                        // LE_COLOR_MODE_INDEX_8
};

uint32_t leColorChannelRed(leColor clr, leColorMode mode)
{
    return (clr & leColorInfoTable[mode].mask.red) >> leColorInfoTable[mode].shift.red;
}

uint32_t leColorChannelGreen(leColor clr, leColorMode mode)
{
    return (clr & leColorInfoTable[mode].mask.green) >> leColorInfoTable[mode].shift.green;
}

uint32_t leColorChannelBlue(leColor clr, leColorMode mode)
{
    return (clr & leColorInfoTable[mode].mask.blue) >> leColorInfoTable[mode].shift.blue;
}

uint32_t leColorChannelAlpha(leColor clr, leColorMode mode)
{
    if(mode == LE_COLOR_MODE_RGBA_5551 ||
       mode == LE_COLOR_MODE_RGBA_8888 ||
       mode == LE_COLOR_MODE_ARGB_8888)
        return (clr & leColorInfoTable[mode].mask.alpha) >> leColorInfoTable[mode].shift.alpha;
    
    return 0xFF;
}