/*******************************************************************************
  Controller interface for BW E-paper (EPD) display.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_EPD_AMP_UC8253.h

  Summary:
    Definitions for AMP EPD displays using the UC8253 controller.

  Description:
    Definitions for AMP EPD displays using the UC8253 controller.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2026 Microchip Technology Inc. and its subsidiaries.
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
//DOM-IGNORE-END

#ifndef DRV_GFX_EPD_AMP_UC8253_H
#define DRV_GFX_EPD_AMP_UC8253_H

#include <stdint.h>
#include "gfx/interface/drv_gfx_disp_intf.h"
#include "drv_gfx_EPD_AMP_UC8253_local.h"

#define ENFORCE_x8_TRANSFERS

#define PIXEL_BUFFER_COLOR_MODE GFX_COLOR_MODE_MONOCHROME
#define AMP_UC8253_setPartial_TEST_DISPLAY   0
#define DRV_EPD_LOAD_CONSTANT_FULL 0
#define LOG_LEGATO 1

#if 0 // no LUT for the AMP display
#define EPD_START_CONFIG(p)    AMP_UC8253_start_config(p)
#define EPD_SET_LUT(p,table)   AMP_UC8253_set_lut \
            (p, AMP_UC8253_lut_table[0], sizeof(table))
#else
#define EPD_START_CONFIG(p)    AMP_UC8253_start_config(p)
#define EPD_SET_LUT(p,table)   /* as nothing */
#endif

/**
 * @brief Start configuration display
 * 
 * @param ctx          Click object.
 */
extern void AMP_UC8253_start_config(AMP_UC8253_t *ctx);

/**
 * @brief Set LUT table
 *
 * @param ctx          Click object.
 * @param lut          Lut table
 * @param n_bytes      Number of bytes in Lut table
 *
 * @Note Changing the "lut table" can change the display's performance.
 */
extern void AMP_UC8253_set_lut(const AMP_UC8253_t * const ctx, 
                               const uint8_t * lut, uint8_t n_bytes );

extern gfxResult AMP_UC8253_blit(const GFX_Disp_Intf intf, 
             DRV_transferFunction_t * const transfer, // delegate
                     const avdisplay_load_t fullPartial, 
                   const avdisplay_buffer_t target,
    const int32_t x, const int32_t y, const gfxPixelBuffer * const buffer);

#define DRV_EPD_Blit(a,b,c,d,e,f,g) AMP_UC8253_blit(a,b,c,d,e,f,g)

extern const gfxDisplayDriver gfxDriverInterface_AMP_UC8253;

#endif // DRV_GFX_EPD_AMP_UC8253_H
