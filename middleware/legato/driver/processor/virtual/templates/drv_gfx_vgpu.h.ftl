// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

/*******************************************************************************
  MPLAB Harmony Generated Driver Header File

  File Name:
    drv_gfx_vgpu.h

  Summary:
    Build-time generated header file for virtual GPU driver.
	
  Description:
    Build-time generated header file for top-level virtual GPU driver.
 * 
    Created with MPLAB Harmony Version 3
*******************************************************************************/

#ifndef DRV_GFX_VGPU
#define DRV_GFX_VGPU

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
    extern "C" {
#endif
 

//DOM-IGNORE-END
void DRV_VGPU_Initialize(void);
void DRV_VGPU_Task(void);

<#if intfInit != "NULL">
void ${intfInit}(void);
</#if>

<#if drawLineFunc != "NULL">
gfxResult ${drawLineFunc}(gfxPixelBuffer * dest,
                      const gfxPoint* p1,
                      const gfxPoint* p2,
                      const gfxRect* clipRect,
					  const gfxColor color);
</#if>					  

<#if fillRectFunc != "NULL">
gfxResult ${fillRectFunc}(gfxPixelBuffer * dest,
					  const gfxRect* clipRect,
					  const gfxColor color);
</#if>					  

<#if blitBuffFunc != "NULL">
gfxResult ${blitBuffFunc}(
						const gfxPixelBuffer* source,
						const gfxRect* srcRect,
						const gfxPixelBuffer* dest,
						const gfxRect* destRect);
</#if>						

<#if blendFunc != "NULL">
gfxResult ${blendFunc}(const gfxBlend blend);
</#if>

<#if setGlobalAlphaFunc != "NULL">
gfxResult ${setGlobalAlphaFunc}(
					const gfxAlpha srcGlobalAlpha,
					const gfxAlpha dstGlobalAlpha,
					uint32_t srcGlobalAlphaValue,
					uint32_t dstGlobalAlphaValue);
</#if>

<#if setPaletteFunc != "NULL">
gfxResult ${setPaletteFunc}(
					uint32_t index_count,
					gfxBuffer color_table,
					gfxBool color_convert);
</#if>

<#if setTransFunc != "NULL">
gfxResult ${setTransFunc}(
					gfxTransparency transparency,
					gfxColor color,
					uint32_t foreground_rop,
					uint32_t background_rop);
</#if>

static const gfxGraphicsProcessor gfxGPUInterface =
{
    .drawLine = ${drawLineFunc},
	.fillRect = ${fillRectFunc},
	.blitBuffer = ${blitBuffFunc},
	.setBlend = ${blendFunc},
	.setGlobalAlpha = ${setGlobalAlphaFunc},
	.setPalette = ${setPaletteFunc},
	.setTransparency = ${setTransFunc},
};

#ifdef __cplusplus
    }
#endif
    
#endif // DRV_GFX_ILI9488_H
