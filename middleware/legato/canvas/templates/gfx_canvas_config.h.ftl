/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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

/*******************************************************************************
  GFX Canvas Configuration Header

  Company:
    Microchip Technology Inc.

  File Name:
    gfx_canvas_config.h

  Summary:
    GFX Canvas Virtual Display Component configuration header file

  Description:
    The GFX Canvas provides a virtual display driver interface. This is the user specified
    or generated configuration header file.

  Remarks:
    None
*******************************************************************************/

/** \file gfx_canvas.h
 * @brief  GFX Canvas Virtual Display definitions and functions.
 *
 * @details The GFX Canvas provides a virtual display driver interface.
 */

#ifndef _GFX_CANVAS_CONFIG_H    /* Guard against multiple inclusion */
#define _GFX_CANVAS_CONFIG_H

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

<#if WindowClippingEnabled == true>
#define CONFIG_CANVAS_ENABLE_WINDOW_CLIPPING true
<#else>
#define CONFIG_CANVAS_ENABLE_WINDOW_CLIPPING false
</#if>

#define CONFIG_CANVAS_DEFAULT_COLOR_MODE GFX_COLOR_MODE_${ColorMode}

#define CONFIG_CANVAS_NUM_OBJ ${NumCanvasObjects}

<#if EffectsEnabled == true>
#define CONFIG_CANVAS_ENABLE_FX true
<#else>
#define CONFIG_CANVAS_ENABLE_FX false
</#if>

#define CONFIG_CANVAS_FX_INTERVAL_MS ${EffectsInterval}

<#if FadeEffectsEnabled == true>
#define CONFIG_CANVAS_ENABLE_FADE_FX true
<#else>
#define CONFIG_CANVAS_ENABLE_FADE_FX false
</#if>

<#if MoveEffectsEnabled == true>
#define CONFIG_CANVAS_ENABLE_MOVE_FX true
<#else>
#define CONFIG_CANVAS_ENABLE_MOVE_FX false
</#if>

<#if DisplayDriverName != "NULL">
#define CONFIG_CANVAS_DISPLAY_INTERFACE ${DisplayDriverName}
</#if>

<#if GraphicsProcessorDriverName != "NULL">
#define CONFIG_CANVAS_GPU_INTERFACE ${GraphicsProcessorDriverName}
</#if>

<#if GPUBlitEnabled == true>
#define CONFIG_CANVAS_GPU_BLIT_ENABLE true
<#else>
#define CONFIG_CANVAS_GPU_BLIT_ENABLE false
</#if>

void gfxcConfigObjectsInitialize(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _GFX_CANVAS_H */