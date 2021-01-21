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

/********************************************************************************
  GLCD Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_glcd.c

  Summary:
    GLCD function implementations for the GLCD PLIB.

  Description:
    The GLCD PLIB provides a simple interface to manage the GLCD controller.
*******************************************************************************/

/** \file plib_glcd.h
* @brief  GLCD function implementations for the GLCD PLIB.
*
* @details The GLCD PLIB provides a simple interface to manage the GLCD controller.
*/

#ifndef PLIB_GLCD_H
#define PLIB_GLCD_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

typedef enum {

    GLCD_ID_0 = 0,
    GLCD_NUMBER_OF_MODULES = 1

} GLCD_MODULE_ID;

typedef enum {

    GLCD_RGB_MODE_PARALLEL_RGB565 = 0x00000000,
    GLCD_RGB_MODE_PARALLEL_RGB888 = 0x00000001,
    GLCD_RGB_MODE_SERIAL_RGB_3 = 0x00000002,
    GLCD_RGB_MODE_SERIAL_RGBA_4 = 0x00000003,
    GLCD_RGB_MODE_SERIAL_12BIT = 0x00000004,
    GLCD_RGB_MODE_YUYV_16BIT = 0x00000005,
    GLCD_RGB_MODE_BT_656 = 0x00000006

} GLCD_RGB_MODE;

typedef enum {

    GLCD_POLARITY_POSITIVE = 0x00000000,
    GLCD_PIXEL_CLOCK_POLARITY_NEGATIVE = 0x00400000,
    GLCD_DE_POLARITY_NEGATIVE = 0x04000000,
    GLCD_HSYNC_POLARITY_NEGATIVE = 0x08000000,
    GLCD_VSYNC_POLARITY_NEGATIVE = 0x10000000

} GLCD_SIGNAL_POLARITY;

typedef enum {

    GLCD_LAYER_ID_0 = 0x0,
    GLCD_LAYER_ID_1 = 0x1,
    GLCD_LAYER_ID_2 = 0x2,
    GLCD_LAYER_ID_MAX = 0x3

} GLCD_LAYER_ID;

typedef enum {

    GLCD_LAYER_COLOR_MODE_LUT8 = 0x0,
    GLCD_LAYER_COLOR_MODE_RGBA5551 = 0x1,
    GLCD_LAYER_COLOR_MODE_RGBA8888 = 0x2,
    GLCD_LAYER_COLOR_MODE_RGB332 = 0x4,
    GLCD_LAYER_COLOR_MODE_RGB565 = 0x5,
    GLCD_LAYER_COLOR_MODE_ARGB8888 = 0x6,
    GLCD_LAYER_COLOR_MODE_L8 = 0x7,
    GLCD_LAYER_COLOR_MODE_L1 = 0x8,
    GLCD_LAYER_COLOR_MODE_L4 = 0x9,
    GLCD_LAYER_COLOR_MODE_YUYV = 0xA,
    GLCD_LAYER_COLOR_MODE_RGB888 = 0xB

} GLCD_LAYER_COLOR_MODE;

typedef enum {

    GLCD_LAYER_SRC_BLEND_BLACK = 0x0000,
    GLCD_LAYER_SRC_BLEND_WHITE = 0x0100,
    GLCD_LAYER_SRC_BLEND_ALPHA_SRC = 0x0200,
    GLCD_LAYER_SRC_BLEND_ALPHA_GBL = 0x0300,
    GLCD_LAYER_SRC_BLEND_ALPHA_SRCGBL = 0x0400,
    GLCD_LAYER_SRC_BLEND_INV_SRC = 0x0500,
    GLCD_LAYER_SRC_BLEND_INV_GBL = 0x0600,
    GLCD_LAYER_SRC_BLEND_INV_SRCGBL = 0x0700,
    GLCD_LAYER_SRC_BLEND_ALPHA_DST = 0x0A00,
    GLCD_LAYER_SRC_BLEND_INV_DST = 0x0D00

} GLCD_LAYER_SRC_BLEND_FUNC;

typedef enum {

    GLCD_LAYER_DEST_BLEND_BLACK = 0x0000,
    GLCD_LAYER_DEST_BLEND_WHITE = 0x1000,
    GLCD_LAYER_DEST_BLEND_ALPHA_SRC = 0x2000,
    GLCD_LAYER_DEST_BLEND_ALPHA_GBL = 0x3000,
    GLCD_LAYER_DEST_BLEND_ALPHA_SRCGBL = 0x4000,
    GLCD_LAYER_DEST_BLEND_INV_SRC = 0x5000,
    GLCD_LAYER_DEST_BLEND_INV_GBL = 0x6000,
    GLCD_LAYER_DEST_BLEND_INV_SRCGBL = 0x7000,
    GLCD_LAYER_DEST_BLEND_ALPHA_DST = 0xA000,
    GLCD_LAYER_DEST_BLEND_INV_DST = 0xD000

} GLCD_LAYER_DEST_BLEND_FUNC;

typedef enum {

    GLCD_IRQ_TRIGGER_LEVEL = 0x00000000,
    GLCD_IRQ_TRIGGER_EDGE = 0x80000000

} GLCD_IRQ_TRIGGER_CONTROL;

//******************************************************************************
/* Function :  PLIB_GLCD_LayerColorModeSet

  Summary:
    Sets the layer color mode

  Description:
    
*/
void PLIB_GLCD_LayerColorModeSet(GLCD_LAYER_ID layerId, GLCD_LAYER_COLOR_MODE colorMode);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerStrideSet

  Summary:
    Sets the layer stride

  Description:
    
*/
void PLIB_GLCD_LayerStrideSet(GLCD_LAYER_ID layerId, uint32_t stride);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerBaseAddressSet

  Summary:
    Sets the layer base address

  Description:
    
*/
void PLIB_GLCD_LayerBaseAddressSet(GLCD_LAYER_ID layerId, uint32_t baseAddress);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerStartXYSet

  Summary:
    Sets the layer V and H offset

  Description:
    
*/
void PLIB_GLCD_LayerStartXYSet(GLCD_LAYER_ID layerId, uint32_t startX, uint32_t startY);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerSizeXYSet

  Summary:
    Sets the layer X, Y size

  Description:
    
*/
void PLIB_GLCD_LayerSizeXYSet(GLCD_LAYER_ID layerId, uint32_t sizeX, uint32_t sizeY);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerGlobalAlphaSet

  Summary:
    Sets the layer alpha

  Description:
    
*/
void PLIB_GLCD_LayerGlobalAlphaSet(GLCD_LAYER_ID layerId, uint8_t value);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerGlobalAlphaGet

  Summary:
    Returns the layer's global alpha 

  Description:
    
*/
uint8_t PLIB_GLCD_LayerGlobalAlphaGet(GLCD_LAYER_ID layerId);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerEnable

  Summary:
    Enables the specified layer

  Description:
    
*/
void PLIB_GLCD_LayerEnable(GLCD_LAYER_ID layerId);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerDisable

  Summary:
    Disables the specified layer

  Description:
    
*/
void PLIB_GLCD_LayerDisable(GLCD_LAYER_ID layerId);

//******************************************************************************
/* Function :  PLIB_GLCD_Enable

  Summary:
    Enables the GLCD peripheral

  Description:
    
*/
void PLIB_GLCD_Enable(void);

//******************************************************************************
/* Function :  PLIB_GLCD_Disable

  Summary:
    Disables the GLCD peripheral

  Description:
    
*/
void PLIB_GLCD_Disable(void);

//******************************************************************************
/* Function :  PLIB_GLCD_BackgroundColorSet

  Summary:
    Sets the background layer color

  Description:
    
*/
void PLIB_GLCD_BackgroundColorSet(uint32_t bgColor);

//******************************************************************************
/* Function :  PLIB_GLCD_HSyncInterruptDisable

  Summary:
    Enables the GLCD HSYNC Interrupt

  Description:
    
*/
void PLIB_GLCD_HSyncInterruptDisable(void);

//******************************************************************************
/* Function :  PLIB_GLCD_RGBSequentialModeSet

  Summary:
    Sets the GLCD sequential mode

  Description:
    
*/
void PLIB_GLCD_RGBSequentialModeSet(GLCD_RGB_MODE mode);

//******************************************************************************
/* Function :  PLIB_GLCD_FrontPorchXYSet

  Summary:
    Sets the V and H Front porch

  Description:
    
*/
void PLIB_GLCD_FrontPorchXYSet(uint32_t frontPorchX, uint32_t frontPorchY);

//******************************************************************************
/* Function :  PLIB_GLCD_BlankingXYSet

  Summary:
    Sets the V and H blanking period

  Description:
    
*/
void PLIB_GLCD_BlankingXYSet(uint32_t blankingX, uint32_t blankingY);

//******************************************************************************
/* Function :  PLIB_GLCD_BackPorchXYSet

  Summary:
    Sets the V and H back porch

  Description:
    
*/
void PLIB_GLCD_BackPorchXYSet(uint32_t backPorchX, uint32_t backPorchY);

//******************************************************************************
/* Function :  PLIB_GLCD_ClockDividerSet

  Summary:
    Sets the GLCD clock divider for pixel clock

  Description:
    
*/
void PLIB_GLCD_ClockDividerSet(uint32_t clockDivider);

//******************************************************************************
/* Function :  PLIB_GLCD_ResolutionXYSet

  Summary:
    Sets the V and H resolution

  Description:
    
*/
void PLIB_GLCD_ResolutionXYSet(uint32_t resolutionX, uint32_t resolutionY);

//******************************************************************************
/* Function :  PLIB_GLCD_SignalPolaritySet

  Summary:
    Sets the polarity of the GLCD control signals

  Description:
    
*/
void PLIB_GLCD_SignalPolaritySet(GLCD_SIGNAL_POLARITY polarity);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerResXYSet

  Summary:
    Sets the layer V and H resolution

  Description:
    
*/
void PLIB_GLCD_LayerResXYSet(GLCD_LAYER_ID layerId, uint32_t resolutionX, uint32_t resolutionY);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerDestBlendFuncSet

  Summary:
    Sets the layer destination blend function

  Description:
    
*/
void PLIB_GLCD_LayerDestBlendFuncSet(GLCD_LAYER_ID layerId, GLCD_LAYER_DEST_BLEND_FUNC blendFunc);

//******************************************************************************
/* Function :  PLIB_GLCD_LayerSrcBlendFuncSet

  Summary:
    Sets the layer source blend function

  Description:
    
*/
void PLIB_GLCD_LayerSrcBlendFuncSet(GLCD_LAYER_ID layerId, GLCD_LAYER_SRC_BLEND_FUNC blendFunc);

//******************************************************************************
/* Function :  PLIB_GLCD_VSyncInterruptEnable

  Summary:
    Enables the GLCD VSYNC interrupt

  Description:
    
*/
void PLIB_GLCD_VSyncInterruptEnable(void);

//******************************************************************************
/* Function :  PLIB_GLCD_VSyncInterruptDisable

  Summary:
    Disables the GLCD VSYNC Interrupt

  Description:
    
*/
void PLIB_GLCD_VSyncInterruptDisable(void);

//******************************************************************************
/* Function :  PLIB_GLCD_IsVerticalBlankingActive

  Summary:
    Returns 1 if GLCD is in VBLANK period

  Description:
    
*/
bool PLIB_GLCD_IsVerticalBlankingActive(void);

//******************************************************************************
/* Function :  PLIB_GLCD_GlobalColorLUTSet

  Summary:
    Sets the global color LUT table

  Description:
    
*/
void PLIB_GLCD_GlobalColorLUTSet(uint32_t* globalLUT);

//******************************************************************************
/* Function :  PLIB_GLCD_PaletteGammaRampEnable

  Summary:
    Enables the Palette Gamma Ramp

  Description:
    
*/
void PLIB_GLCD_PaletteGammaRampEnable(void);

//******************************************************************************
/* Function :  PLIB_GLCD_PaletteGammaRampDisable

  Summary:
    Disables the Palette Gamma Ramp

  Description:
    
*/
void PLIB_GLCD_PaletteGammaRampDisable(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GLCD_H


