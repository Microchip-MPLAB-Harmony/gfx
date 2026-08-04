/** 
 * @file drv_gfx_EPD_MIKROE_2n13_lut.h
 * @brief LUT provider for Mikore 2.13" EPD displays
 *
 * @details The look-up table provides configuration and timing
 * details for operation of an EPD display.
 *  This file contains the source code for loading EPD look-up tables
 *  (LUT); specifically for the UC8253 controller.
 *  This is achieved through the r20-24h commands.
 *  This file is derived from material supplied by the display manufacturer
 *  by email 29 July 2025 and updated 01 August 2025.
 *
 * From: https://github.com/MikroElektronika/mikrosdk_click_v2
 * Extracted from .\clicks\eink213inch\example\main.c
 */

/*******************************************************************************
  LUT controller for BW EInk display.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_eink_MIKRO_2n13lut.h

  Summary:
    Source code for initializing EInk look-up tables (LUT).

  Description:
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

#ifndef DRV_GFX_EINK_MIKRO_2n13_LUT_H
#define DRV_GFX_EINK_MIKRO_2n13_LUT_H

extern const uint8_t MIKROE_2n13_lut_table[70];

#endif // DRV_GFX_EINK_MIKRO_2n13_LUT_H
