/** 
 * @file drv_gfx_EPD_MIKROE_2n13.h
 * @brief Custom controller for Mikore 2.13" EPD displays
 */

/*******************************************************************************
  Customization package for using the MIKROE 2.13" E-ink display

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_EPD_MIKRO_2n13.h

  Summary:
    Source code for configuring the Legato interface.

*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#include <stdint.h>
#include "drv_gfx_EPD_MIKROE_2n13_local.h"
#include "drv_gfx_EPD_MIKROE_2n13_lut.h"
#include "gfx/interface/drv_gfx_disp_intf.h"

#ifndef DRV_GFX_EPD_MIKRO_2n13_H
#define DRV_GFX_EPD_MIKRO_2n13_H

#define DISPLAY_CONTROLLER_WAVESHARE   1
//#define ENFORCE_x8_TRANSFERS
#define PIXEL_BUFFER_COLOR_MODE GFX_COLOR_MODE_MONOCHROME

#define ENABLE_HALF_DUPLEX 0

#define EPD_START_CONFIG(p)    MIKROE_2n13_start_config(p)
#define EPD_SET_LUT(p,table)   MIKROE_2n13_set_lut \
            (p, &MIKROE_2n13_lut_table[0], sizeof(table))
#define DRV_EPD_Blit(a,b,c,d,e,f,g) MIKROE_2n13_blit(a,b,c,d,e,f,g)            

/**
 * @brief Start configuration display
 * 
 * @param ctx          Click object.
 */
extern void MIKROE_2n13_start_config ( MIKROE_2n13_t *ctx );

/**
 * @brief Set LUT table
 *
 * @param ctx          Click object.
 * @param lut          Lut table
 * @param n_bytes      Number of bytes in Lut table
 *
 * @Note Changing the "lut table" can change the display's performance.
 */
extern void MIKROE_2n13_set_lut(const MIKROE_2n13_t * const ctx, 
                               const uint8_t * lut, uint8_t n_bytes );

extern gfxResult MIKROE_2n13_blit(const GFX_Disp_Intf intf, 
             DRV_transferFunction_t * const transfer, // delegate
                     const avdisplay_load_t fullPartial, 
                   const avdisplay_buffer_t target,
    const int32_t x, const int32_t y, const gfxPixelBuffer * const buffer);

extern void MIKROE_2n13_display_constant(const GFX_Disp_Intf eink213inch, uint8_t datum);

#endif // DRV_GFX_EPD_MIKRO_2n13_H
