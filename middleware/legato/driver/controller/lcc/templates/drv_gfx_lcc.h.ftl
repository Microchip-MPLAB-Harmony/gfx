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
 * @brief Execute update task.
 * @details Performs a driver task update.
 * @code
 * DRV_LCC_Update();
 * @endcode
 * @return void.
 */
void DRV_LCC_Update(void);

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
 * @brief Graphics driver generic IOCTL interface.
 * @details Sends an IOCTL message to the driver.
 * @code
 * gfxIOCTLArg_Value val;
 * val.value.v_uint = 1;
 * DRV_LCC_IOCTL(GFX_IOCTL_FRAME_START, &val);
 * @endcode
 * @return gfxDriverIOCTLResponse the IOCTL handler response
 */
 gfxDriverIOCTLResponse DRV_LCC_IOCTL(gfxDriverIOCTLRequest req,
                                      void* arg);

/**
 * @brief Defines the LCC interface functions.
 * @details Establishes the driver abstract interface functions between the driver
 * and client. The client is either a graphics library middleware or application which
 * interfaces with the driver through these functions.
 */
static const gfxDisplayDriver gfxDriverInterface =
{
    .update = DRV_LCC_Update,				        /**< implements update */
    .blitBuffer = DRV_LCC_BlitBuffer,			    /**< implements blitbuffer */
	.ioctl = DRV_LCC_IOCTL                          /**< implements driver ioctl interface */
};
        
#ifdef _DOXYGEN_



#endif


#ifdef __cplusplus
}
#endif
    
#endif
