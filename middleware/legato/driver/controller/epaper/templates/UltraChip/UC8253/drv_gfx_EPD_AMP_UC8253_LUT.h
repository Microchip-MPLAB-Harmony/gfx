/*******************************************************************************
  LUT controller for E-paper (EPD) display.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_EPD_AMP_UC8253_LUT.h

  Summary:
    Source code for initializing EPD look-up tables (LUT).

  Description:
    This file contains the source code for loading EPD look-up tables
    (LUT); specifically for the UC8253 controller.
    This is achieved through the r20-24h commands.
    This file is derived from material supplied by the display manufacturer
    by email 29 July 2025 and updated 01 August 2025.
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

#include "gfx/interface/drv_gfx_disp_intf.h"
#include <stdint.h>

#ifndef DRV_GFX_LUT_AMP_UC8253_H
#define DRV_GFX_LUT_AMP_UC8253_H

#define DRV_GFX_PROGRAMMABLE_LUT

/** The LUT information is organized as a byte array. */
typedef const uint8_t * DRV_EPD_lut_t;

/** The original code toggles the 0x22/0x23 command pair
 *  to different portions of the LUT data. This seems to 
 *  invert colors on every pass, so it may have been 
 *  simply showing the ability to invert data. */
#define DRV_GFX_DO_NOT_TOGGLE_LUT   0

/* =================================================
 *  Look-up table data 
 * -------------------------------------------------
 * GC is Full-screen refresh; updating all pixels.
 */
extern const uint8_t AMP_UC8253_LUT_GC[];

/** -------------------------------------------------
 * DU is partial and fast refresh. 
 * The screen refresh is achieved by comparing the 
 * new and old images(r13h and r10h). All you need to do 
 * is use the r13h command [to] transmits image data into the IC.
 * */
extern const uint8_t AMP_UC8253_LUT_DU[];

/** DU is partial and fast refresh. 
 * Per email: Lawrence Cham 15 Sept 2025 */
extern const uint8_t AMP_UC8253_LUT_DU2[];

/** Load EPD LUTs for UC8253 display controller 
 * @param intf SPI interface index number
 * @param lut Look-up table data array
 * @details The LUT data includes 5 LUT datasets
 * (commands 0x20, 0x21, 0x22, 0x23, 0x24).
 * Each dataset consists of 6 line items, each with 7 bytes 
 * (but not all are used). Dataset GC is used when writing the full
 * image and DU is used for partial updates. Note the alternating
 * value of lightDark_toggle.
 * @note Ported from the display vendor's example code. The name of the
 * function is significant to the vendor, as is the operation of
 * the lightDark_toggle variable.
 *  */
void AMP_UC8253_send_lut(GFX_Disp_Intf intf, const DRV_EPD_lut_t lut);

extern uint8_t AMP_lightDark_toggle;

#endif // DRV_GFX_LUT_AMP_UC8253_H
