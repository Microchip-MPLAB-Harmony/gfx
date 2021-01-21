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
    drv_gfx_lcc_generic.h

  Summary:
    Build-time generated header file
	Interface for the graphics library where the primitives are rendered and 
	sent to the graphics controller either external or internal

  Description:
    This header file contains the function prototypes and definitions of
    the data types and constants that make up the interface to the Low-Cost
    Controllerless (LCC) Graphics Controller for PIC32C MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

/** \file drv_gfx_lcc.h.ftl
 * @brief LCC driver functions and definitions.
 *
 * @details This header file contains the function prototypes and definitions
 * of the data types and constants that make up the interface to the Low-Cost
 * Controllerless (LCC) Graphics Controller.
 *
 * This driver is configured via MHC and generated specific to the configuration
 * and hardware architecture set in MHC.
 * @see LE LCC Display Driver component
 */

#ifndef _DRV_GFX_LCC_H
#define _DRV_GFX_LCC_H

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
extern "C"
{
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

/**
 * @brief Initialize driver.
 * @details Initializes the LCC driver. This routine is typically called
 * by a graphics library or by the application during application initialization.
 * @code
 * gfxResult res = DRV_LCC_Initialize();
 * @endcode
 * @return GFX_SUCCESS if driver ready to render, otherwise GFX_FAILURE.
 */
gfxResult DRV_LCC_Initialize(void);

/**
 * @brief Get color mode.
 * @details Gets the current color mode.
 * @code
 * gfxDisplayDriver*  drv;
 * gfxColorMode mode = drv->getColorMode();
 * @endcode
 * @return RGB color mode.
 */
gfxColorMode DRV_LCC_GetColorMode();

/**
 * @brief Get buffer count.
 * @details Gets the buffer count. This count is the number of configured buffers. In
 * a single buffer system count will be 1. In a double buffered system, the count
 * would be at least 2.
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t cnt = drv->getBufferCount();
 * @endcode
 * @return number of configured buffers.
 */
uint32_t DRV_LCC_GetBufferCount();

/**
 * @brief Get display width.
 * @details Gets the display width. Display with is the max number of horizontal pixels. This is the horizonal portion
 * of the screen resolution.
 * @see DRV_LCC_GetDisplayHeight()
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t width = drv->getDisplayWidth();
 * @endcode
 * @return horizontal display width.
 */
uint32_t DRV_LCC_GetDisplayWidth();

/**
 * @brief Get display height.
 * @details Gets the display height. Display height is the max number vertical
 * pixels. This is the vertical portion of the screen resolution.
 * @see DRV_LCC_GetDisplayWidth()
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t width = drv->getDisplayHeight();
 * @endcode
 * @return vertical display height.
 */
uint32_t DRV_LCC_GetDisplayHeight();

/**
 * @brief Execute update task.
 * @details Performs a driver task update.
 * @code
 * DRV_LCC_Update();
 * @endcode
 * @return void.
 */
void DRV_LCC_Update();

/**
 * @brief Get layer count.
 * @details Gets the layer count. Layer count is the number of display layers
 * configured.
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t cnt = drv->getLayerCount();
 * @endcode
 * @return number of display layers.
 */
uint32_t DRV_LCC_GetLayerCount();

/**
 * @brief Get active layer.
 * @details Gets the active layer. The active layer is the index of the active layer.
 * @code
 * gfxDisplayDriver*  drv;
 * uint32_t layer = drv->getActiveLayer();
 * @endcode
 * @return layer index.
 */
uint32_t DRV_LCC_GetActiveLayer();

/**
 * @brief Set active layer.
 * @details Sets the active the layer at <span class="param">idx</span> position.
 * @code
 * gfxDisplayDriver* drv;
 * uint32_t idx;
 * gfxResult res = drv->setActiveLayer(idx);
 * @endcode
 * @return GFX_SUCCESS if layer at idx is active, otherwise GFX_FAILURE.
 */
gfxResult DRV_LCC_SetActiveLayer(uint32_t idx);

/**
 * @brief Gets the state for a hardware layer.
 * @details Gets the state of layer <span style="color: #820a32"><em>idx</em></span> .
 * @code
 * gfxDisplayDriver* drv;
 * uint32_t idx;
 * gfxLayerState state = drv->getLayerState(idx);
 * @endcode
 * @return The rectangle of the layer.
 */
gfxLayerState DRV_LCC_GetLayerState(uint32_t idx);

/**
 * @brief Blit buffer.
 * @details Copies <span class="param">buf</span>
 * to the framebuffer at location <span class="param">x</span> and
 * <span class="param">y</span>.
 * @code
 * gfxDisplayDriver* drv;
 * gfxResult res = drv->blitBuffer();
 * @endcode
 * @return GFX_SUCCESS if blit was performed, otherwise GFX_FAILURE.
 */
gfxResult DRV_LCC_BlitBuffer(int32_t x,
                            int32_t y,
                            gfxPixelBuffer* buf);

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
void DRV_LCC_Swap(void);

/**
 * @brief Get VSYNC count.
 * @details Returns the vertical pulse count. This can be used
 * as an interrupt to indicate the end of a frame or start of a new frame.
 * @code
 * gfxDisplayDriver* drv;
 * drv->getVSYNCCount();
 * @endcode
 * @return VSYNC count.
 */
uint32_t DRV_LCC_GetVSYNCCount(void);

/**
 * @brief Get framebuffer.
 * @details Returns the framebuffer (display buffer) located at
 * <span class="param">idx</span> position.
 * @code
 * gfxDisplayDriver* drv;
 * int32_t idx;
 * gfxPixelBuffer * buf = drv->getFrameBuffer(idx);
 * @param idx is the buffer index
 * @endcode
 * @return void.
 */
gfxPixelBuffer * DRV_LCC_GetFrameBuffer(int32_t idx);

/**
 * @brief Set global palette.
 * @details Sets the global palette for the driver.  Used for blitting color map buffers.
 * @code
 * gfxDisplayDriver* drv;
 * gfxResult res = drv->setPalette(addr, colorMode, colorCount);
 * @endcode
 * @return GFX_SUCCESS if the palette was successfully set, otherwise GFX_FAILURE.
 */
gfxResult DRV_LCC_SetPalette(gfxBuffer* palette,
                             gfxColorMode mode,
                             uint32_t colorCount);

/**
 * @brief Defines the LCC interface functions.
 * @details Establishes the driver abstract interface functions between the driver
 * and client. The client is either a graphics library middleware or application which
 * interfaces with the driver through these functions.
 */
static const gfxDisplayDriver gfxDriverInterface =
{
    DRV_LCC_GetColorMode,		/**< implements getColorMode */
    DRV_LCC_GetBufferCount,		/**< implements getBufferCount */
    DRV_LCC_GetDisplayWidth,	/**< implements getDisplayWidth */
    DRV_LCC_GetDisplayHeight,	/**< implements getDisplayHeight */
    DRV_LCC_Update,				/**< implements update */
    DRV_LCC_GetLayerCount,		/**< implements layerCount */
    DRV_LCC_GetActiveLayer,		/**< implements activeLayer */
    DRV_LCC_SetActiveLayer,		/**< implements setActive */
    DRV_LCC_GetLayerState,       /**< implements getLayerState */
    DRV_LCC_BlitBuffer,			/**< implements blitbuffer */
    DRV_LCC_Swap,				/**< implements swap */
    DRV_LCC_GetVSYNCCount,		/**< implements getSwap */
    DRV_LCC_GetFrameBuffer,		/**< implements getFrameBuffer */
    DRV_LCC_SetPalette,         /**< implements setPalette */ 
	NULL                        /**< nop ctrlConfig */
};
        
#ifdef _DOXYGEN_



#endif


#ifdef __cplusplus
}
#endif
    
#endif
