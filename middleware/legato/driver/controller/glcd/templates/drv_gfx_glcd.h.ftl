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
  GFX GLCD Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_glcd_static.h

  Summary:
    GFX GLCD driver interface declarations for the static single instance driver.

  Description:
    The GLCD device driver provides a simple interface to manage the GLCD
    module on Microchip microcontrollers. This file defines the interface
    Declarations for the GLCD driver.

  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.

    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

/** \file drv_gfx_glcd.h
 * @brief GLCD driver functions and definitions.
 *
 * @details The GLCD device driver provides a simple interface to manage the
 * GLCD module on Microchip PIC32MZ DA microcontrollers. This file defines the interface
 * declarations for the GLCD driver.
 *
 * This driver is configured via MHC and generated specific to the configuration
 * and hardware architecture set in MHC.
 * @see LE GLCD Display Driver component
 *
 * \image html https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/gfx/images/display_driver_display_module.png
 *
 */

#ifndef _DRV_GFX_GLCD_H
#define _DRV_GFX_GLCD_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
/* Note:  A file that maps the interface definitions above to appropriate static
          implementations (depending on build mode) is included at the bottom of
          this file.
*/

#include "gfx/driver/controller/glcd/plib_glcd.h"
#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
    extern "C" {
#endif
        
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Initialize driver.
 * @details Initializes the GLCD driver. This routine is typically called
 * by a graphics library or by the application during application initialization.
 * @code
 * gfxResult res = DRV_GLCD_Initialize();
 * @endcode
 * @return GFX_SUCCESS if driver ready to render, otherwise GFX_FAILURE.
 */
void DRV_GLCD_Initialize(void);


/**
 * @brief Execute update task.
 * @details Performs a driver task update.
 * @code
 * DRV_GLCD_Update();
 * @endcode
 * @return void.
 */
void DRV_GLCD_Update(void);

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
gfxResult DRV_GLCD_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf);



/**
 * @brief Graphics driver generic IOCTL interface.
 * @details Sends an IOCTL message to the driver.
 * @code
 * gfxIOCTLArg_Value val;
 * val.value.v_uint = 1;
 * DRV_GLCD_IOCTL(GFX_IOCTL_FRAME_START, &val);
 * @endcode
 * @return gfxDriverIOCTLResponse the IOCTL handler response
 */
 gfxDriverIOCTLResponse DRV_GLCD_IOCTL(gfxDriverIOCTLRequest req,
                                       void* arg);

/**
 * @brief Defines the GLCD interface functions.
 * @details Establishes the driver abstract interface functions between the driver
 * and client. The client is either a graphics library middleware or application which
 * interfaces with the driver through these functions.
 */
<#if CanvasModeOnly == true>
static const gfxDisplayDriver glcdDisplayDriver =
<#else>
static const gfxDisplayDriver gfxDriverInterface =
</#if>
{
    .update = DRV_GLCD_Update,
    .blitBuffer = DRV_GLCD_BlitBuffer,
	.ioctl = DRV_GLCD_IOCTL
};

#ifdef __cplusplus
    }
#endif
    
#endif // #ifndef _DRV_GFX_GLCD_H
/*******************************************************************************
 End of File
*/
