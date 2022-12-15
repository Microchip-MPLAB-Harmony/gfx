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
    legato_imagedecoder_raw.h

  Summary:
    Image decoder

  Description:
    Internal library use only
*******************************************************************************/

/** \file legato_imagedeocder_raw.h
* @brief RAW Image decoder
*
* @details Internal library use only
*
*/

// DOM-IGNORE-BEGIN



#ifndef LE_IMAGEDECODER_RAW_H
#define LE_IMAGEDECODER_RAW_H

#include "gfx/legato/image/legato_image.h"

#if LE_ENABLE_RAW_DECODER == 1

#include "gfx/legato/common/legato_pixelbuffer.h"

#define LE_IMAGEDECODER_RAW_MAX_STAGES 16
#define LE_IMAGEDECODER_BLOCK_READ_SIZE 4

#if LE_STREAMING_ENABLED == 1

// the cache used for streaming image source data
extern uint8_t leRawImageDecoderScratchBuffer[LE_ASSET_DECODER_PIXEL_CACHE_SIZE];
// the cache used for streaming mask lookup data
extern uint8_t leRawImageDecoderMaskScratchBuffer[LE_ASSET_DECODER_MASK_CACHE_SIZE];
// the cache used for streaming palette lookup data
extern uint8_t leRawImageDecoderPaletteScratchBuffer[LE_ASSET_DECODER_PALETTE_CACHE_SIZE];
// the cache used for streaming blend mask lookup data
extern uint8_t leRawImageDecoderBlendBuffer[LE_ASSET_DECODER_BLEND_CACHE_SIZE];

#endif

// *****************************************************************************
/* Enumeration:
    enum leRawDecoderMode

  Summary:
    Indicates the current mode of the raw image decoder
*/
enum leRawDecoderMode
{
    LE_RAW_MODE_NONE,
    LE_RAW_MODE_DRAW,
    LE_RAW_MODE_COPY,
    LE_RAW_MODE_RESIZE,
    LE_RAW_MODE_RESIZEDRAW,
    LE_RAW_MODE_RENDER,
    LE_RAW_MODE_ROTATE,
    LE_RAW_MODE_ROTATEDRAW
};

struct leRawDecodeState;

// *****************************************************************************
/* Structure:
    struct leRawDecodeStage

  Summary:
    Structure defining an individual raw image decoding stage

    struct leRawDecodeState* state - pointer to the decoder state

    exec - function that runs this stage
    cleanup - function that cleans up this stage
*/
typedef struct leRawDecodeStage
{
    struct leRawDecodeState* state;

    leResult (*exec)(struct leRawDecodeStage* stage);
    void (*cleanup)(struct leRawDecodeStage* stage);
} leRawDecodeStage;

typedef struct leRawSourceReadOperation
{
    uint32_t x;
    uint32_t y;
    uint32_t bufferIndex;
    leColor data;
} leRawSourceReadOperation;

// *****************************************************************************
/* Structure:
    struct leRawDecodeState

  Summary:
    Structure defining the state of the raw image decoder
*/
typedef struct leRawDecodeState
{
#if LE_STREAMING_ENABLED == 1
    leStreamManager manager; // so this decoder can act as a streaming manager
                             // if necessary
#endif

    const leImage* source; // the source image
    leImage* target; // destination image for copy/decompress mode

    enum leRawDecoderMode mode; // the current mode of the decoder

    leColorMode targetMode;
    leBool randomRLE; // hint to the decoder that RLE decodes will be random

    leRect sourceRect; // the image source rectangle
    leRect destRect; // the target rectangle dimensions

    int32_t targetX; // the current target X position
    int32_t targetY; // the current target Y position

    int32_t referenceX; // the current target X position
    int32_t referenceY; // the current target Y position

    uint32_t sizeX; // the size in X of the new data
    uint32_t sizeY; // the size in Y of the new data

    int32_t angle; // rotation angle
    lePoint sourceOrigin; // rotation origin
    lePoint targetOrigin; // rotation origin

    uint32_t rowIterator; // the row iterator
    uint32_t colIterator; // the column iterator

    leRawSourceReadOperation readOperation[LE_IMAGEDECODER_BLOCK_READ_SIZE]; // source data read requests
    uint32_t readCount;
    uint32_t readIndex;

    leColor writeColor;

    uint32_t globalAlpha; // a global alpha state
    uint32_t pixelAlpha; // a pixel alpha state (not used yet)

    const lePixelBuffer* palette; // pointer to a lookup table if needed

    leImageFilterMode filterMode; // resize filter mode

    leBool needToLookupMaskColor;

    leRawDecodeStage* stages[LE_IMAGEDECODER_RAW_MAX_STAGES];
    int32_t currentStage;
    int32_t done;

} leRawDecodeState;

#endif /* LE_ENABLE_RAW_DECODER */

#endif /* LE_IMAGEDECODER_RAW_H */

