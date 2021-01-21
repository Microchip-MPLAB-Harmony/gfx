// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_driver.h

  Summary:
    Abstract display driver definition for the Legato graphics library.

  Description:
    Abstract display driver definition for the Legato graphics library.
*******************************************************************************/


#ifndef LEGATO_DRIVER_H
#define LEGATO_DRIVER_H

#include "gfx/legato/common/legato_pixelbuffer.h"

// *****************************************************************************
/* Structure:
    struct leDisplayDriver

  Summary:
    Defines the interface for a Legato display driver.  All drivers must,
    at a minimum, implement these interfaces

    getColorMode - returns the supported color mode for the driver

    getBufferCount - returns the number of buffers the driver is configured to use

    getDisplayWidth - returns the width of the driver frame buffer

    getDisplayHeight - returns the height of the driver frame buffer

    update - the driver tasks/update function

    getLayerCount - the number of hardware layers the driver supports

    getActiveLayer - the current active hardware layer

    setActiveLayer - sets the current active hardware layer
                     all buffer writes should go to this layer

    blitBuffer - instructs the driver to blit a buffer (buf) at
                 location (x, y)

    swap - instructs the driver to swap its buffer chain

    getVSYNCCount - queries the driver for its VSYNC counter
                    if a driver implements this counter this value
                    can be used to do frame rate calculations

*/
typedef struct leDisplayDriver
{
    leColorMode (*getColorMode)(void);
    
    uint32_t (*getBufferCount)(void);
    
    uint32_t (*getDisplayWidth)(void);
    
    uint32_t (*getDisplayHeight)(void);
    
    void (*update)(void);
    
    uint32_t (*getLayerCount)(void);
    
    uint32_t (*getActiveLayer)(void);
    
    leResult (*setActiveLayer)(uint32_t idx);
    
    leResult (*blitBuffer)(int32_t x,
                           int32_t y,
                           lePixelBuffer* buf);
                           
    void (*swap)(void);
    
    uint32_t (*getVSYNCCount)(void);
} leDisplayDriver;

#endif // LEGATO_H