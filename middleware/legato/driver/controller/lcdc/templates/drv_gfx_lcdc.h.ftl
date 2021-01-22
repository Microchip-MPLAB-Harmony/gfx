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
  MPLAB Harmony LCDC Generated Driver Header File

  File Name:
    drv_gfx_lcdc.h

  Summary:
    Build-time generated header file
	Interface for the graphics library where the primitives are rendered and 
	sent to the graphics controller either external or internal

  Description:
    This header file contains the function prototypes and definitions of
    the data types and constants that make up the interface to the LCDC
    Graphics Controller for PIC32C MPU/MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

/** \file drv_gfx_lcdc.h
* @brief  LCDC Generated Driver Header File
*
* @details This header file contains the function prototypes and definitions of the data types and constants that make up the interface to the LCDC Graphics Controller for PIC32C MPU/MCUs.
*/

#ifndef _DRV_GFX_LCDC_H
#define _DRV_GFX_LCDC_H

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
    extern "C" {
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
 * @details Initializes the LCDC driver. This routine is typically called
 * by a graphics library or by the application during application initialization.
 * @code
 * gfxResult res = DRV_LCDC_Initialize();
 * @endcode
 * @return GFX_SUCCESS if driver ready to render, otherwise GFX_FAILURE.
 */
gfxResult DRV_LCDC_Initialize(void);


/**
 * @brief Execute update task.
 * @details Performs a driver task update.
 * @code
 * DRV_LCDC_Update();
 * @endcode
 * @return void.
 */
void DRV_LCDC_Update(void);

/**
 * @brief Blit buffer.
 * @details Copies <span class="param">buf</span>
 * to the framebuffer at location <span class="param">x</span> and
 * <span class="param">y</span> with
 * <span class="param">blend</span> composition.
 * @code
 * gfxDisplayDriver* drv;
 * gfxResult res = drv->blitBuffer();
 * @endcode
 * @return GFX_SUCCESS if blit was performed, otherwise GFX_FAILURE.
 */
gfxResult DRV_LCDC_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf);


/**
 * @brief Graphics driver generic IOCTL interface.
 * @details Sends an IOCTL message to the driver.
 * @code
 * gfxIOCTLArg_Value val;
 * val.value.v_uint = 1;
 * DRV_LCDC_IOCTL(gfxDriverIOCTLRequest req, void* arg);
 * @endcode
 * @return gfxDriverIOCTLResponse the IOCTL handler response
 */
gfxDriverIOCTLResponse DRV_LCDC_IOCTL(gfxDriverIOCTLRequest req, void* arg);


<#if CanvasModeOnly == true>
static const gfxDisplayDriver lcdcDisplayDriver =
<#else>
static const gfxDisplayDriver gfxDriverInterface =
</#if>
{
	.update = DRV_LCDC_Update,
	.blitBuffer = DRV_LCDC_BlitBuffer,
	.ioctl = DRV_LCDC_IOCTL
};

#ifdef __cplusplus
    }
#endif
    
#endif
