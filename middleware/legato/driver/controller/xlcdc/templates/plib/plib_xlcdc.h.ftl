/*******************************************************************************
  XLCDC Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_xlcdc.h

  Summary:
    Contains XLCDC peripheral library function declarations.

  Description:
    This library implements a SUBSET of the register configurations
    for the XLCDC peripheral.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _PLIB_XLCDC_H
#define _PLIB_XLCDC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
    extern "C" {
#endif
//DOM-IGNORE-END

/*******************************************************************************
  XLCDC Layer Order

  Summary:
    Enumerates the layers in order.

  Description:
    The order depends on the HEO video priority selection.

  Remarks:
    None.
*/
typedef enum
{
    XLCDC_LAYER_BASE,
<#if XLMHEOVIDPRI>
    XLCDC_LAYER_OVR1,
    XLCDC_LAYER_HEO,
<#else>
    XLCDC_LAYER_HEO,
    XLCDC_LAYER_OVR1,
</#if>
<#if SupportOVR2>
    XLCDC_LAYER_OVR2,
</#if>
} XLCDC_LAYER;

/*******************************************************************************
  XLCDC CLUT Color Modes

  Summary:
    Enumerates the supported color modes.

  Description:
    This enumeration lists the available CLUT color modes on XLCDC Layers.

  Remarks:
    Applies to all layers
*/
typedef enum
{
    XLCDC_CLUT_COLOR_MODE_1BPP,
    XLCDC_CLUT_COLOR_MODE_2BPP,
    XLCDC_CLUT_COLOR_MODE_3BPP,
    XLCDC_CLUT_COLOR_MODE_4BPP,
} XLCDC_CLUT_COLOR_MODE;

/*******************************************************************************
  XLCDC RGB Color Modes

  Summary:
    Enumerates the supported color modes.

  Description:
    This enumeration lists the available RGB color modes on XLCDC Layers.

  Remarks:
    Applies to all layers
*/
typedef enum
{
    XLCDC_RGB_COLOR_MODE_CLUT = -1,
    XLCDC_RGB_COLOR_MODE_RGB_444,
    XLCDC_RGB_COLOR_MODE_ARGB_4444,
    XLCDC_RGB_COLOR_MODE_RGBA_4444,
    XLCDC_RGB_COLOR_MODE_RGB_565,
    XLCDC_RGB_COLOR_MODE_ARGB_1555,
    XLCDC_RGB_COLOR_MODE_RGB_666,
    XLCDC_RGB_COLOR_MODE_RGB_666_PACKED,
    XLCDC_RGB_COLOR_MODE_ARGB_1666,
    XLCDC_RGB_COLOR_MODE_ARGB_1666_PACKED,
    XLCDC_RGB_COLOR_MODE_RGB_888,
    XLCDC_RGB_COLOR_MODE_RGB_888_PACKED,
    XLCDC_RGB_COLOR_MODE_ARGB_1888,
    XLCDC_RGB_COLOR_MODE_ARGB_8888,
    XLCDC_RGB_COLOR_MODE_RGBA_8888,
} XLCDC_RGB_COLOR_MODE;

<#if XLMHEOYCbCrEN>
/*******************************************************************************
  XLCDC YCBCR Color Modes

  Summary:
    Enumerates the supported YCbCr color modes.

  Description:
    This enumeration lists the available YCbCr modes on the XLCDC HEO Layer.

  Remarks:
    Applies to HEO layer. FFmpeg format equivalents provided in brackets
    where applicable, convert to lowercase before passing to FFmpeg.
*/
typedef enum
{
    XLCDC_YCBCR_COLOR_MODE_AYCBCR_444 = 0, // AYCBCR 4:4:4 Packed (VUYA)
    XLCDC_YCBCR_COLOR_MODE_YCBCR_422_M0,   // YCBCR_MODE0 4:2:2 Packed (YUYV422)
    XLCDC_YCBCR_COLOR_MODE_YCBCR_422_M1,   // YCBCR_MODE1 4:2:2 Packed (UYVY422)
    XLCDC_YCBCR_COLOR_MODE_YCBCR_422_M2,   // YCBCR_MODE2 4:2:2 Packed (YVYU422)
    XLCDC_YCBCR_COLOR_MODE_YCBCR_422_M3,   // YCBCR_MODE3 4:2:2 Packed
    XLCDC_YCBCR_COLOR_MODE_YCBCR_422_SP,   // YCBCR 4:2:2 Semi-Planar
    XLCDC_YCBCR_COLOR_MODE_YCBCR_422_PL,   // YCBCR 4:2:2 Planar (YUV422P)
    XLCDC_YCBCR_COLOR_MODE_YCBCR_420_SP,   // YCBCR 4:2:0 Semi-Planar
    XLCDC_YCBCR_COLOR_MODE_YCBCR_420_PL,   // YCBCR 4:2:0 Planar (YUV420P)
} XLCDC_YCBCR_COLOR_MODE;

/*******************************************************************************
  XLCDC YCbCr Surface Descriptor

  Summary:
    Parameters required to descibe a YCbCr surface.
*/
typedef struct
{
    XLCDC_YCBCR_COLOR_MODE colorMode;
    uint16_t windowStartX;
    uint16_t windowStartY;
    uint16_t windowSizeX;
    uint16_t windowSizeY;
    uint16_t imageSizeX;
    uint16_t imageSizeY;
    void *imageAddress[3];
    bool scaleToWindow;
} XLCDC_HEO_YCBCR_SURFACE;

<#else>
/*******************************************************************************
  XLCDC HEO RGB Surface Descriptor

  Summary:
    Parameters required to descibe an HEO RGB surface.
*/
typedef struct
{
    XLCDC_RGB_COLOR_MODE colorMode;
    uint16_t windowStartX;
    uint16_t windowStartY;
    uint16_t windowSizeX;
    uint16_t windowSizeY;
    uint16_t imageSizeX;
    uint16_t imageSizeY;
    void *imageAddress;
    bool scaleToWindow;
} XLCDC_HEO_RGB_SURFACE;

</#if>
/*******************************************************************************
  Function:
    void XLCDC_EnableClocks(void)

   Summary:
    Enables XLCDC clocks.

   Description:
    Enables and configures Peripheral, Pixel and PWM clocks.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_EnableClocks(void);

/*******************************************************************************
  Function:
    void XLCDC_SetupTimingEngine(void)

   Summary:
    Configures the XLCDC Timing Engine.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_SetupTimingEngine(void);

/*******************************************************************************
  Function:
    void XLCDC_SetupBaseLayer(void)

   Summary:
    Enables the XLCDC Base Layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_SetupBaseLayer(void);

/*******************************************************************************
  Function:
    void XLCDC_SetupOVR1Layer(void)

   Summary:
    Enables the XLCDC OVR1 Layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_SetupOVR1Layer(void);
<#if SupportOVR2>

/*******************************************************************************
  Function:
    void XLCDC_SetupOVR2Layer(void)

   Summary:
    Enables the XLCDC OVR2 Layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_SetupOVR2Layer(void);
</#if>

/*******************************************************************************
  Function:
    void XLCDC_SetupHEOLayer(void)

   Summary:
    Enables the XLCDC HEO Layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_SetupHEOLayer(void);

/*******************************************************************************
  Function:
    void XLCDC_EnableBacklight(void)

   Summary:
    Enables the XLCDC Backlight.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_EnableBacklight(void);

/*******************************************************************************
  Function:
    void XLCDC_DisableBacklight(void)

   Summary:
    Disables the XLCDC Backlight.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_DisableBacklight(void);

/*******************************************************************************
  Function:
    void XLCDC_SetBacklightBrightness(uint8_t brightness)

   Summary:
    Sets the XLCDC Backlight Brightness.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    brightness - Brightness in the range of 0 to 255.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_SetBacklightBrightness(uint8_t brightness);

/*******************************************************************************
  Function:
    bool XLCDC_SetLayerEnable(XLCDC_LAYER layer, bool enable, bool update)

   Summary:
    Enable an XLCDC layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    layer - One the four XLCDC layers of type XLCDC_LAYER.
    enable - If True, enables layer. If False, disables layer.
    update - If True, updates attributes immediately.
             If False, update manually using XLCDC_UpdateLayerAttributes(layer).

   Returns:
    0, on success. 1, on failure.

   Remarks:
    None.
*/
bool XLCDC_SetLayerEnable(XLCDC_LAYER layer, bool enable, bool update);

/*******************************************************************************
  Function:
    bool XLCDC_SetLayerOpts(XLCDC_LAYER layer, uint8_t alpha, bool enable_dma, bool update)

   Summary:
    Sets the alpha value and enables/disables the DMA of a selected layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    layer - One the four XLCDC layers of type XLCDC_LAYER.
    alpha - Alpha in the range of 0 to 255.
    enable_dma - If True, enables DMA.
                 If False, disables DMA.
    update - If True, updates attributes immediately.
             If False, update manually using XLCDC_UpdateLayerAttributes(layer).

   Returns:
    0, on success. 1, on failure.

   Remarks:
    Base layer does not support alpha, alpha only affects the layer window.
*/
bool XLCDC_SetLayerOpts(XLCDC_LAYER layer, uint8_t alpha, bool enable_dma, bool update);

/*******************************************************************************
  Function:
    bool XLCDC_SetLayerRGBColorMode(XLCDC_LAYER layer, XLCDC_RGB_COLOR_MODE mode, bool update)

   Summary:
    Sets the RGB color mode of the selected layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    layer - One the four XLCDC layers of type XLCDC_LAYER.
    mode - Any XLCDC_RGB_COLOR_MODE type.
    update - If True, updates attributes immediately.
             If False, update manually using XLCDC_UpdateLayerAttributes(layer).

   Returns:
    0, on success. 1, on failure.

   Remarks:
    Does not effect YUV or CLUT modes.
*/
bool XLCDC_SetLayerRGBColorMode(XLCDC_LAYER layer, XLCDC_RGB_COLOR_MODE mode, bool update);

/*******************************************************************************
  Function:
    bool XLCDC_SetLayerWindowXYPos(XLCDC_LAYER layer, uint32_t xpos, uint32_t ypos, bool update)

   Summary:
    Sets the Window X, Y Position of the selected layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    layer - One the four XLCDC layers of type XLCDC_LAYER.
    xpos - Integer specifying Window X Position.
    ypos - Integer specifying Window Y Position.
    update - If True, updates attributes immediately.
             If False, update manually using XLCDC_UpdateLayerAttributes(layer).

   Returns:
    0, on success. 1, on failure.

   Remarks:
    None.
*/
bool XLCDC_SetLayerWindowXYPos(XLCDC_LAYER layer, uint32_t xpos, uint32_t ypos, bool update);

/*******************************************************************************
  Function:
    bool XLCDC_SetLayerWindowXYSize(XLCDC_LAYER layer, uint32_t xsize, uint32_t ysize, bool update)

   Summary:
    Sets the Window X, Y Size of the selected layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    layer - One the four XLCDC layers of type XLCDC_LAYER.
    xsize - Integer specifying Window X Size.
    ysize - Integer specifying Window Y Size.
    update - If True, updates attributes immediately.
             If False, update manually using XLCDC_UpdateLayerAttributes(layer).

   Returns:
    0, on success. 1, on failure.

   Remarks:
    None.
*/
bool XLCDC_SetLayerWindowXYSize(XLCDC_LAYER layer, uint32_t xsize, uint32_t ysize, bool update);

/*******************************************************************************
  Function:
    bool XLCDC_SetLayerXStride(XLCDC_LAYER layer, uint32_t xstride, bool update)

   Summary:
    Sets the row stride of the selected layer.

   Description:
    Specifies row striding offset in bytes.

   Precondition:
    None.

   Parameters:
    layer - One the four XLCDC layers of type XLCDC_LAYER.
    xstride - Integer specifying row stride in bytes.
    update - If True, updates attributes immediately.
             If False, update manually using XLCDC_UpdateLayerAttributes(layer).

   Returns:
    0, on success. 1, on failure.

   Remarks:
    None.
*/
bool XLCDC_SetLayerXStride(XLCDC_LAYER layer, uint32_t xstride, bool update);

/*******************************************************************************
  Function:
    bool XLCDC_SetLayerPStride(XLCDC_LAYER layer, uint32_t pstride, bool update)

   Summary:
    Sets the pixel stride of the selected layer.

   Description:
    Specifies pixel striding offset in bytes(between consecutive pixels).

   Precondition:
    None.

   Parameters:
    layer - One the four XLCDC layers of type XLCDC_LAYER.
    pstride - Integer specifying pixel stride in bytes.
    update - If True, updates attributes immediately.
             If False, update manually using XLCDC_UpdateLayerAttributes(layer).

   Returns:
    0, on success. 1, on failure.

   Remarks:
    None.
*/
bool XLCDC_SetLayerPStride(XLCDC_LAYER layer, uint32_t pstride, bool update);

/*******************************************************************************
  Function:
    bool XLCDC_SetLayerAddress(XLCDC_LAYER layer, uint32_t address, bool update)

   Summary:
    Sets the frame buffer address of the selected layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    layer - One the four XLCDC layers of type XLCDC_LAYER.
    address - Integer specifying the frame buffer address in memory.
    update - If True, updates attributes immediately.
             If False, update manually using XLCDC_UpdateLayerAttributes(layer).

   Returns:
    0, on success. 1, on failure.

   Remarks:
    Ensure address is 32-byte aligned.
*/
bool XLCDC_SetLayerAddress(XLCDC_LAYER layer, uint32_t address, bool update);

/*******************************************************************************
  Function:
   bool XLCDC_UpdateLayerAttributes(XLCDC_LAYER layer, bool update);

   Summary:
    Updates the attributes of the selected layer.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    layer - One the four XLCDC layers of type XLCDC_LAYER.
    update - If True, updates attributes immediately.
             If False, update manually using XLCDC_UpdateLayerAttributes(layer).

   Returns:
    0, on success. 1, on failure.

   Remarks:
    None.
*/
bool XLCDC_UpdateLayerAttributes(XLCDC_LAYER layer);

<#if XLMHEOYCbCrEN>
/*******************************************************************************
  Function:
   bool XLCDC_DisplayHEOYCbCrSurface(XLCDC_HEO_YCBCR_SURFACE *surface);

   Summary:
    Displays a YCbCr Surface on the High-End Overlay.

   Description:
    Displays a surface on HEO and performs scaling if required.

   Precondition:
    None.

   Parameters:
    surface - Pointer to XLCDC YCbCr Surface Descriptor.

   Returns:
    0, on success. 1, on failure.

   Remarks:
    None.
*/
bool XLCDC_DisplayHEOYCbCrSurface(XLCDC_HEO_YCBCR_SURFACE *surface);

<#else>
/*******************************************************************************
  Function:
   bool XLCDC_DisplayHEORGBSurface(XLCDC_HEO_RGB_SURFACE *surface);

   Summary:
    Displays a HEO RGB Surface on the High-End Overlay.

   Description:
    Displays a surface on HEO and performs scaling if required.

   Precondition:
    None.

   Parameters:
    surface - Pointer to XLCDC YCbCr Surface Descriptor.

   Returns:
    0, on success. 1, on failure.

   Remarks:
    None.
*/
bool XLCDC_DisplayHEORGBSurface(XLCDC_HEO_RGB_SURFACE *surface);

</#if>
/*******************************************************************************
  Function:
    void XLCDC_MIPIColorModeSignalEnable(bool enable)

   Summary:
    Enable or Disable the MIPI DSI Host's Color Mode On signal.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    enable - If True, enables Color Mode On signal.
             If False, disables Color Mode On signal.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_MIPIColorModeSignalEnable(bool enable);

/*******************************************************************************
  Function:
    void XLCDC_Stop(void)

   Summary:
    Stops the XLCDC peripheral.

   Description:
    Disables the XLCDC digital blocks.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_Stop(void);

/*******************************************************************************
  Function:
    void XLCDC_Start(void)

   Summary:
    Starts the XLCDC peripheral.

   Description:
    Enables the XLCDC digital blocks.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_Start(void);

/*******************************************************************************
  Function:
    void XLCDC_Initialize(void)

   Summary:
    Initializes the XLCDC peripheral.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    None.
*/
void XLCDC_Initialize(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
    }
#endif
// DOM-IGNORE-END

#endif
