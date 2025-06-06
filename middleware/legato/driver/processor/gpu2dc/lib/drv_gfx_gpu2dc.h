// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef DRV_GFX_GPU2DC_H
#define DRV_GFX_GPU2DC_H

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Draws a line between two points on the destination buffer.
 *
 * @param dest Pointer to the destination pixel buffer.
 * @param p1 Pointer to the starting point of the line.
 * @param p2 Pointer to the ending point of the line.
 * @param clipRect Pointer to the clipping rectangle; can be NULL.
 * @param color Color value to use for drawing the line.
 *
 * @return GFX_SUCCESS if the operation is successful; otherwise, GFX_FAILURE.
 */
gfxResult DRV_GPU2DC_Line(
    gfxPixelBuffer* dest,
    const gfxPoint* p1,
    const gfxPoint* p2,
    const gfxRect* clipRect,
    const gfxColor color);

/**
 * @brief Fills a rectangle area on the destination buffer with a specified color.
 *
 * @param dest Pointer to the destination pixel buffer.
 * @param clipRect Pointer to the rectangle area to fill; can be NULL to fill the entire buffer.
 * @param color Color value to use for filling.
 *
 * @return GFX_SUCCESS if the operation is successful; otherwise, GFX_FAILURE.
 */
gfxResult DRV_GPU2DC_Fill(
    gfxPixelBuffer* dest,
    const gfxRect* clipRect,
    const gfxColor color);

/**
 * @brief Copies a region from the source buffer to the destination buffer.
 *
 * @param source Pointer to the source pixel buffer.
 * @param srcRect Pointer to the source rectangle region; can be NULL to use the entire source buffer.
 * @param dest Pointer to the destination pixel buffer.
 * @param destRect Pointer to the destination rectangle region; can be NULL to use the entire destination buffer.
 *
 * @return GFX_SUCCESS if the operation is successful; otherwise, GFX_FAILURE.
 */
gfxResult DRV_GPU2DC_Blit(
    const gfxPixelBuffer* source,
    const gfxRect* srcRect,
    const gfxPixelBuffer* dest,
    const gfxRect* destRect);

/**
 * @brief Sets the blending mode for subsequent drawing operations.
 *
 * @param blend The blending mode to set.
 *
 * @return GFX_SUCCESS if the operation is successful; otherwise, GFX_FAILURE.
 */
gfxResult DRV_GPU2DC_SetBlend(
    const gfxBlend blend);

/**
 * @brief Sets the global alpha values for source and destination surfaces.
 *
 * @param srcGlobalAlpha Global alpha mode for the source buffer.
 * @param dstGlobalAlpha Global alpha mode for the destination buffer.
 * @param srcGlobalAlphaValue Alpha value (0-255) for the source buffer.
 * @param dstGlobalAlphaValue Alpha value (0-255) for the destination buffer.
 *
 * @return GFX_SUCCESS if the operation is successful; otherwise, GFX_FAILURE.
 */
gfxResult DRV_GPU2DC_SetGlobalAlpha(
    const gfxAlpha srcGlobalAlpha,
    const gfxAlpha dstGlobalAlpha,
    uint32_t srcGlobalAlphaValue,
    uint32_t dstGlobalAlphaValue);

/**
 * @brief Loads a color palette into the GPU for INDEX8 source surfaces.
 *
 * @param index_count Number of palette entries to load.
 * @param color_table Pointer to the color table data.
 * @param color_convert Boolean flag indicating whether to convert colors to the destination format.
 *
 * @return GFX_SUCCESS if the operation is successful; otherwise, GFX_FAILURE.
 */
gfxResult DRV_GPU2DC_SetPalette(
    uint32_t index_count,
    gfxBuffer color_table,
    gfxBool color_convert);

/**
 * @brief Sets the transparency mode and associated parameters for drawing operations.
 *
 * @param transparency Transparency mode to apply.
 * @param color Color value used for transparency comparison.
 * @param foreground_rop Raster operation code for foreground pixels.
 * @param background_rop Raster operation code for background pixels.
 *
 * @return GFX_SUCCESS if the operation is successful; otherwise, GFX_FAILURE.
 */
gfxResult DRV_GPU2DC_SetTransparency(
    gfxTransparency transparency,
    gfxColor color,
    uint32_t foreground_rop,
    uint32_t background_rop);

/**
 * @brief Initializes the 2D GPU driver and hardware context.
 *
 * This function sets up the GPU command buffer, initializes hardware parameters,
 * and opens the Nano2D context required for subsequent GPU operations.
 */
void DRV_GPU2DC_Initialize(void);

/**
 * @brief Processes the 2D GPU driver interrupt events.
 *
 * This function must be called in the GPU ISR to process driver events.
 */
void DRV_GPU2DC_Handle_Interrupt(void);

/**
 * @brief External interface instance for the GPU driver.
 *
 * This is the Legato-compatible graphics processor interface instance that
 * provides access to GPU-accelerated functions.
 */
extern const gfxGraphicsProcessor gfxGPUInterface;

#ifdef __cplusplus
}
#endif

#endif // DRV_GFX_GPU2DC_H
