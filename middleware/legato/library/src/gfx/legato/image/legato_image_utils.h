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
    gfx_image_utils.h

  Summary:
    Image return utilities

  Description:
    Internal library use only
*******************************************************************************/

/** \file legato_image_utils.h
* @brief Image return utilities
*
* @details Internal library use only
*
*/

// DOM-IGNORE-BEGIN
#ifndef LE_IMAGE_UTILS_H
#define LE_IMAGE_UTILS_H
//DOM-IGNORE-END

#include "gfx/legato/image/legato_image.h"

// DOM-IGNORE-BEGIN

// *****************************************************************************
/* Function:
    uint32_t leGetRLEDataAtIndex(uint8_t* data,
                                 uint32_t max,
                                 uint32_t idx,
                                 uint32_t* startBlock,
                                 uint32_t* startOffset)

  Summary:
    Gets an RLE data value at the given index.

  Parameters:
    uint8_t* data - the data buffer to query
    uint32_t max - the maximum size of the buffer
    uint32_t idx - the index to query for
    uint32_t* startBlock - the block to start at, initialize to zero
    uint32_t* startOffset - the offset to start at, initialize to zero

  Returns:
    leResult
*/
uint32_t leGetRLEDataAtIndex(uint8_t* data,
                             uint32_t max,
                             uint32_t idx,
                             uint32_t* startBlock,
                             uint32_t* startOffset);

// *****************************************************************************
/* Function:
    uint32_t lleGetDiscreteValueAtIndex(uint32_t index,
                                        uint32_t value,
                                        leColorMode mode)

  Summary:
    Gets a discrete color value from an index.  Capable of doing sub-byte
    value retrieval.

  Parameters:
    uint32_t index - the index value
    uint32_t value - the data value to query
    leColorMode mode - the color mode of the data

  Returns:
    leResult
*/
uint32_t leGetDiscreteValueAtIndex(uint32_t index,
                                   uint32_t value,
                                   leColorMode mode);

// *****************************************************************************
/* Function:
    uint32_t leGetOffsetFromIndexAndBPP(uint32_t index,
                                       leBitsPerPixel bpp)

  Summary:
    Gets an offset in bytes of a pixel index and a BPP mode.

  Parameters:
    uint32_t index - the index value
    leBitsPerPixel bpp - the bpp value of the data

  Returns:
    leResult
*/
uint32_t leGetOffsetFromIndexAndBPP(uint32_t index,
                                    leBitsPerPixel bpp);


// DOM-IGNORE-END

#endif /* LE_IMAGE_UTILS_H */