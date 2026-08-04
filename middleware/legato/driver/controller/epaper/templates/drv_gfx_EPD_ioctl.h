/** 
 * @file drv_gfx_EPD_ioctl.h
 * @brief EPD Controller IOCTL option enumerations.
 *
 * @details This header file contains the enumerations used by the IOCTL function.
 */

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

#ifndef DRV_GFX_EPD_IOCTL_H
#define DRV_GFX_EPD_IOCTL_H

#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
    extern "C" {
#endif

    typedef enum
    {
        GFX_IOCTL_EPD_FAST_REFRESH = GFX_IOCTL_VENDOR_START,
        GFX_IOCTL_EPD_LOAD_FULL_IMAGE,
        GFX_IOCTL_EPD_OVERDRAW,
    } gfxDriverIOCTLRequestExtended;
 
    /** IOCTL command constants that allow the client to 
     *  control how the EPD display will be updated. 
     *  @details The FULL variant requires all pixels to be defined
     *  by the data transfer (according to the device dimensions). 
     *  The PARTIAL variant requires the gfxPixelBuffer to 
     *  specify the X-by-Y display region. */
    typedef enum avdisplay_load_e
    {
        AVDISPLAY_LOAD_NONE = 0,
        AVDISPLAY_LOAD_FULL_DISPLAY,
        AVDISPLAY_LOAD_PARTIAL,
    } avdisplay_load_t;
        
    /** IOCTL command constants that allow the client to 
     *  select the EPD display color buffer(s). */
    typedef enum avdisplay_buffer_e
    {
        AVDISPLAY_BUFFER_NONE = 0,
        AVDISPLAY_BUFFER_OLDorBW  = 0x10,
        AVDISPLAY_BUFFER_NEWorRED = 0x13,
    } avdisplay_buffer_t;

#ifdef __cplusplus
    }
#endif
    
#endif // DRV_GFX_EPD_IOCTL_H
