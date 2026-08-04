/** 
 * @file drv_gfx_EPD_UC8253_2n13_local.h
 * @brief Utility code for use within the domain of the UC8253 driver(s).
 *
 * @details This header file contains function prototypes and
 *    declarations of utility services for operation of the E-ink driver,
 *    including wrappers for the greater hardware interfaces.
 *
 * @note This file is not part of the public interface and should not be used
 *    by an application.
 */

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

#ifndef DRV_GFX_EPD_AMP_UC8253_LOCAL_H
#define DRV_GFX_EPD_AMP_UC8253_LOCAL_H

#include "gfx/driver/gfx_driver.h"
#include "gfx/interface/drv_gfx_disp_intf.h"
#include "../../drv_gfx_EPD_ioctl.h"
#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

/* Rename for encapsulation */
#define AMP_UC8253_t   GFX_Disp_Intf

/** Transfer-data function delegates
 *  @note This mechanism saves repetitive code blocks by provide
 *        functions for block-buffer, repetitive byte or
 *        unique test function data transfers.  */
typedef int DRV_transferFunction_t(GFX_Disp_Intf, uint8_t *, int);
int DRV_writeData(GFX_Disp_Intf intf, uint8_t * bytes, int count);
int DRV_writeRepeatedData(GFX_Disp_Intf intf, uint8_t * byte, int count);
int DRV_test_writeRotate(GFX_Disp_Intf intf, uint8_t * byte, int count);

/* Generic EPD transfer controller 
 * @param intf SPI interface selector
 * @param transfer Data-transfer method delegate (DRV_transferFunction_t)
 * @param fullPartial Invoke a full-panel or partial data transfer 
 * @param target Specify target memory within the controller
 * @param x X coordinate of upper-left corner of partial image
 * @param y Y coordinate of upper-left corner of partial image
 * @param buffer Image geometry and storage specifier */
extern gfxResult AMP_UC8253_Blit(const GFX_Disp_Intf intf, 
             DRV_transferFunction_t * const transfer, 
                     const avdisplay_load_t fullPartial, 
					 const avdisplay_buffer_t target,
					 const int32_t x,
					 const int32_t y, 
					 const gfxPixelBuffer * const buffer);

#ifdef __cplusplus
    }
#endif
    
#endif // DRV_GFX_EPD_AMP_UC8253_LOCAL_H
