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

#include <stdint.h>

/*******************************************************************************
  GFX GLCD Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    gfx_canvas_init.c

  Summary:
    GFX Canvas Virtual Display Component private header file

  Description:
    The GFX Canvas provides a virtual display driver interface. 

  Remarks:
    None
*******************************************************************************/

#include "gfx_canvas_api.h"
#include "gfx_canvas.h"

/* *****************************************************************************
 End of File
 */
<#list 0.. (NumCanvasObjects - 1) as i>
<#assign FB = "Canvas" + i + "FrameBufferAlloc">
<#if .vars[FB] == "Auto">
<#assign HEIGHT = "Canvas" + i + "Height">
<#assign WIDTH = "Canvas" + i + "Width">
<#assign MODE = "Canvas" + i + "Mode">
<#if .vars[MODE] == "GS_8">
uint8_t canvasfb${i}[${.vars[WIDTH]} * ${.vars[HEIGHT]}] = {0};
<#elseif .vars[MODE] == "RGB_565">
uint16_t canvasfb${i}[${.vars[WIDTH]} * ${.vars[HEIGHT]}] = {0};
<#else>
uint32_t canvasfb${i}[${.vars[WIDTH]} * ${.vars[HEIGHT]}] = {0};
</#if>
</#if>
</#list>

void GFX_CANVAS_Initialize(void)
{
    unsigned int id;

<#list 0.. (NumCanvasObjects - 1) as i>
    id = gfxcCreate();
    <#assign HEIGHT_A = "Canvas" + i + "Height">
    <#assign WIDTH_A = "Canvas" + i + "Width">
    <#assign MODE_A = "Canvas" + i + "Mode">
    <#assign FB_A = "Canvas" + i + "FrameBufferAlloc">
    <#assign ADDR_A = "Canvas" + i + "Address">
    gfxcSetPixelBuffer(id, ${.vars[WIDTH_A]}, ${.vars[HEIGHT_A]}, GFX_COLOR_MODE_${.vars[MODE_A]},
    <#if .vars[FB_A] == "None">
                       NULL);
    <#elseif .vars[FB_A] == "Manual">
                       0x${.vars[ADDR_A]});
    <#else>
                       canvasfb${i});
    </#if>
</#list>
}