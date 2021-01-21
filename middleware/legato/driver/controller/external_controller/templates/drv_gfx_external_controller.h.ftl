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
  MPLAB Harmony Generated Driver Header File

  File Name:
    drv_gfx_custom_external.h

  Summary:
    Build-time generated header file for ${ControllerName} driver.
	
  Description:
    Build-time generated header file for top-level ${ControllerName} driver.
 * 
    Created with MPLAB Harmony Version 3.00
*******************************************************************************/

/** \file drv_gfx_external_controller_ftl.h
 * @brief External Controller driver functions and definitions.
 *
 * @details This header file contains the function prototypes and definitions
 * of the data types and constants that make up the interface to the External
 * Controller Graphics Controller.
 *
 * This driver files are configured via MHC and generated specific to the configuration
 * and hardware architecture set in MHC.
 * @see LE External Controller component
 */

#ifndef DRV_GFX_${ControllerName}_H
#define DRV_GFX_${ControllerName}_H

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
    extern "C" {
#endif
 
/**
 * @brief Initialize driver.
 * @details Initializes the External Controller driver. This routine is typically called
 * by a graphics library or by the application during application initialization.
 * @code
 * gfxResult res = DRV_<em>controller</em>_Initialize();
 * @endcode
 * @return GFX_SUCCESS if driver ready to render, otherwise GFX_FAILURE.
 */
int DRV_${ControllerName}_Initialize(void);

/**
 * @brief Execute update task.
 * @details Performs a driver task update.
 * @code
 * DRV_<em>controller</em>_Update();
 * @endcode
 * @return void.
 */
void DRV_${ControllerName}_Update(void);

/**
 * @brief Blit buffer.
 * @details Copies <span style="color: #820a32"><em>buf</em></span>
 * to the framebuffer at location <span style="color: #820a32"><em>x</em></span> and
 * <span style="color: #820a32"><em>y</em></span>.
 * @code
 * gfxDisplayDriver* drv;
 * gfxResult res = drv->blitBuffer();
 * @endcode
 * @return GFX_SUCCESS if blit was performed, otherwise GFX_FAILURE.
 */
gfxResult DRV_${ControllerName}_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf);

/**
 * @brief Graphics driver generic IOCTL interface.
 * @details Sends an IOCTL message to the driver.
 * @code
 * gfxIOCTLArg_Value val;
 * val.value.v_uint = 1;
 * DRV_${ControllerName}_IOCTL(GFX_IOCTL_FRAME_START, &val);
 * @endcode
 * @return gfxDriverIOCTLResponse the IOCTL handler response
 */
 gfxDriverIOCTLResponse DRV_${ControllerName}_IOCTL(gfxDriverIOCTLRequest req,
                                                    void* arg);

/**
 * @brief Defines the External Controller interface functions.
 * @details Establishes the driver abstract interface functions between the driver
 * and client. The client is either a graphics library middleware or application which
 * interfaces with the driver through these functions.
 */
static const gfxDisplayDriver gfxDriverInterface =
{
    .update = DRV_${ControllerName}_Update,				       
    .blitBuffer = DRV_${ControllerName}_BlitBuffer,			   
	.ioctl = DRV_${ControllerName}_IOCTL,                
};

#ifdef __cplusplus
    }
#endif
    
#endif // DRV_GFX_${ControllerName}_H
