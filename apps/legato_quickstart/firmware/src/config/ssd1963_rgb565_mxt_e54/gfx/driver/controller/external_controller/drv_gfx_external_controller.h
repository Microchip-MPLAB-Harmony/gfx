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
  MPLAB Harmony Generated Driver Header File

  File Name:
    drv_gfx_custom_external.h

  Summary:
    Build-time generated header file for ssd1963 driver.
	
  Description:
    Build-time generated header file for top-level ssd1963 driver.
 * 
    Created with MPLAB Harmony Version 3.00
*******************************************************************************/

/** \file drv_gfx_external_controller_ftl.h
 * @brief External Controller driver functions and definitions.
 *
 * @details This header file contains the function prototypes and definitions
 * of the data types and constants that make up the interface to the External
 * Controller Graphics Controller.
 *
 * This driver files are configured via MHC and generated specific to the configuration
 * and hardware architecture set in MHC.
 * @see LE External Controller component
 */

#ifndef DRV_GFX_ssd1963_H
#define DRV_GFX_ssd1963_H

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
    extern "C" {
#endif
 
/**
 * @brief Initialize driver.
 * @details Initializes the External Controller driver. This routine is typically called
 * by a graphics library or by the application during application initialization.
 * @code
 * gfxResult res = DRV_<em>controller</em>_Initialize();
 * @endcode
 * @return GFX_SUCCESS if driver ready to render, otherwise GFX_FAILURE.
 */
int DRV_ssd1963_Initialize(void);

/**
 * @brief Execute update task.
 * @details Performs a driver task update.
 * @code
 * DRV_<em>controller</em>_Update();
 * @endcode
 * @return void.
 */
void DRV_ssd1963_Update(void);

/**
 * @brief Get color mode.
 * @details Returns the current color mode.
 * @code
 * gfxDisplayDriver*  drv;
 * gfxColorMode mode = drv->getColorMode();
 * @endcode
 * @return RGB color mode.
 */
gfxColorMode DRV_ssd1963_GetColorMode(void);

/**
 * @brief Get buffer count.
 * @details Returns the number of configured buffers. In a single buffer system
 * count will be 1. In a double buffered system, the count would be at least 2.
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t cnt = drv->getBufferCount();
 * @endcode
 * @return number of configured buffers.
 */
uint32_t DRV_ssd1963_GetBufferCount(void);

/**
 * @brief Get display width.
 * @details Returns the max number of horizontal pixels. This is the horizonal portion
 * of the screen resolution.
 * @see DRV_LCC_GetDisplayHeight()
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t width = drv->getDisplayWidth();
 * @endcode
 * @return horizontal display width.
 */
uint32_t DRV_ssd1963_GetDisplayWidth(void);

/**
 * @brief Get display height.
 * @details Returns the max number vertical pixels. This is the vertical portion
 * of the screen resolution.
 * @see DRV_LCC_GetDisplayWidth()
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t width = drv->getDisplayHeight();
 * @endcode
 * @return vertical display height.
 */
uint32_t DRV_ssd1963_GetDisplayHeight(void);

/**
 * @brief Get layer count.
 * @details Returns the number of display layers configured.
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t cnt = drv->getLayerCount();
 * @endcode
 * @return number of display layers.
 */
uint32_t DRV_ssd1963_GetLayerCount();

/**
 * @brief Get active layer.
 * @details Returns the index of the active layer.
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t layer = drv->getActiveLayer();
 * @endcode
 * @return layer index.
 */
uint32_t DRV_ssd1963_GetActiveLayer();

/**
 * @brief Set active layer.
 * @details Sets active the layer at <span style="color: #820a32"><em>idx</em></span> position.
 * @code
 * gfxDisplayDriver* drv;
 * uint32_t idx;
 * gfxResult res = drv->setActiveLayer(idx);
 * @endcode
 * @return GFX_SUCCESS if layer at idx is active, otherwise GFX_FAILURE.
 */
gfxResult DRV_ssd1963_SetActiveLayer(uint32_t idx);

/**
 * @brief Gets the state for a hardware layer.
 * @details Gets the state of layer <span style="color: #820a32"><em>idx</em></span> .
 * @code
 * gfxDisplayDriver* drv;
 * uint32_t idx;
 * gfxLayerState state = drv->getLayerState(idx);
 * @endcode
 * @return The state of the layer.
 */
gfxLayerState DRV_ssd1963_GetLayerState(uint32_t idx);

/**
 * @brief Blit buffer.
 * @details Copies <span style="color: #820a32"><em>buf</em></span>
 * to the framebuffer at location <span style="color: #820a32"><em>x</em></span> and
 * <span style="color: #820a32"><em>y</em></span>.
 * @code
 * gfxDisplayDriver* drv;
 * gfxResult res = drv->blitBuffer();
 * @endcode
 * @return GFX_SUCCESS if blit was performed, otherwise GFX_FAILURE.
 */
gfxResult DRV_ssd1963_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf);

/**
 * @brief Swap buffer.
 * @details Swaps the rendering buffer with the display buffer. The display buffer now
 * becomes the rendering buffer. Swapping support double buffering technology.
 * @code
 * gfxDisplayDriver* drv;
 * drv->swap();
 * @endcode
 * @return void.
 */
void DRV_ssd1963_Swap(void);

/**
 * @brief Get VSYNC count.
 * @details Returns the vertical pulse count. This can be used
 * as an interrupt to indicate the end of a frame or start of a new frame.
 * This value can also be used to do frame rate calculations.
 * @code
 * gfxDisplayDriver* drv;
 * drv->getVSYNCCount();
 * @endcode
 * @return VSYNC count.
 */
uint32_t DRV_ssd1963_GetSwapCount(void);

/**
 * @brief Set global palette.
 * @details Sets the global palette for the driver.  Used for blitting color map buffers.
 * @code
 * gfxDisplayDriver* drv;
 * gfxResult res = drv->setPalette(addr, colorMode, colorCount);
 * @endcode
 * @return GFX_SUCCESS if the palette was successfully set, otherwise GFX_FAILURE.
 */
gfxResult DRV_ssd1963_SetPalette(gfxBuffer* palette,
                                           gfxColorMode mode,
                                           uint32_t colorCount);

/**
 * @brief Defines the External Controller interface functions.
 * @details Establishes the driver abstract interface functions between the driver
 * and client. The client is either a graphics library middleware or application which
 * interfaces with the driver through these functions.
 */
static const gfxDisplayDriver gfxDriverInterface =
{
    DRV_ssd1963_GetColorMode,         /**< implements getColorMode */
    DRV_ssd1963_GetBufferCount,       /**< implements getBufferCount */
    DRV_ssd1963_GetDisplayWidth,      /**< implements getDisplayWidth */
    DRV_ssd1963_GetDisplayHeight,     /**< implements getDisplayHeight */
    DRV_ssd1963_Update,               /**< implements update */
    DRV_ssd1963_GetLayerCount,        /**< implements layerCount */
    DRV_ssd1963_GetActiveLayer,       /**< implements activeLayer */
    DRV_ssd1963_SetActiveLayer,       /**< implements setActive */
    DRV_ssd1963_GetLayerState,        /**< implements getLayerState */
    DRV_ssd1963_BlitBuffer,           /**< implements blitbuffer */
    DRV_ssd1963_Swap,                 /**< implements swap. */
    DRV_ssd1963_GetSwapCount,         /**< implements getVSYNCCount */
    NULL,                                       /**< nop getframebuffer */
    DRV_ssd1963_SetPalette,           /**< implements setPalette */
    NULL                                        /**< nop ctrlConfig */
};

#ifdef __cplusplus
    }
#endif
    
#endif // DRV_GFX_ssd1963_H
