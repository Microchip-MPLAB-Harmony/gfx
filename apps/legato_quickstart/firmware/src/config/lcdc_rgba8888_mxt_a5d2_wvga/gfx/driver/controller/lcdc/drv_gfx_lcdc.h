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

/*******************************************************************************
  MPLAB Harmony LCC Generated Driver Header File

  File Name:
    drv_gfx_lcdc.h

  Summary:
    Build-time generated header file
	Interface for the graphics library where the primitives are rendered and 
	sent to the graphics controller either external or internal

  Description:
    This header file contains the function prototypes and definitions of
    the data types and constants that make up the interface to the LCDC
    Graphics Controller for PIC32C MPU/MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

#ifndef _DRV_GFX_LCDC_H
#define _DRV_GFX_LCDC_H

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
    extern "C" {
#endif

//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
gfxResult DRV_LCDC_Initialize(void);

gfxColorMode DRV_LCDC_GetColorMode(void);
uint32_t DRV_LCDC_GetBufferCount(void);
uint32_t DRV_LCDC_GetDisplayWidth(void);
uint32_t DRV_LCDC_GetDisplayHeight(void);
void DRV_LCDC_Update(void);
uint32_t DRV_LCDC_GetLayerCount();
uint32_t DRV_LCDC_GetActiveLayer();
gfxResult DRV_LCDC_SetActiveLayer(uint32_t idx);
gfxLayerState DRV_LCDC_GetLayerState(uint32_t idx);
gfxResult DRV_LCDC_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf);
void DRV_LCDC_Swap(void);
uint32_t DRV_LCDC_GetVSYNCCount(void);
gfxPixelBuffer * DRV_LCDC_GetFrameBuffer(int32_t idx);
gfxResult DRV_LCDC_SetPalette(gfxBuffer* palette,
                              gfxColorMode mode,
                              uint32_t colorCount);
void DRV_LCDC_SetUseGPU(gfxBool onOff);
gfxResult DRV_LCDC_CtrlrConfig(ctlrCfg request, void * arg);

static const gfxDisplayDriver gfxDriverInterface =
{
	DRV_LCDC_GetColorMode,
	DRV_LCDC_GetBufferCount,
	DRV_LCDC_GetDisplayWidth,
	DRV_LCDC_GetDisplayHeight,
	DRV_LCDC_Update,
	DRV_LCDC_GetLayerCount,
	DRV_LCDC_GetActiveLayer,
	DRV_LCDC_SetActiveLayer,
	DRV_LCDC_GetLayerState,
	DRV_LCDC_BlitBuffer,
	DRV_LCDC_Swap,
	DRV_LCDC_GetVSYNCCount,
	DRV_LCDC_GetFrameBuffer,
	DRV_LCDC_SetPalette,
	DRV_LCDC_CtrlrConfig
};

#ifdef __cplusplus
    }
#endif
    
#endif
