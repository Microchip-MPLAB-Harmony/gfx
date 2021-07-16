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
    legato_imagedecoder_mono.h

  Summary:
    Monochrome Image decoder

  Description:
    Internal library use only
*******************************************************************************/

/** \file legato_imagedeocder_mono.h
* @brief Monochrome Image decoder
*
* @details Internal library use only
*
*/

// DOM-IGNORE-BEGIN

#ifndef LE_IMAGEDECODER_MONO_H
#define LE_IMAGEDECODER_MONO_H

#include "gfx/legato/image/legato_image.h"

#if LE_ENABLE_MONO_DECODER == 1

#include "gfx/legato/common/legato_pixelbuffer.h"

#define LE_IMAGEDECODER_MONO_MAX_STAGES 16
#define LE_IMAGEDECODER_BLOCK_READ_SIZE 4

#if LE_STREAMING_ENABLED == 1

// the cache used for streaming image source data
extern uint8_t leMonoImageDecoderScratchBuffer[LE_ASSET_DECODER_PIXEL_CACHE_SIZE];

#endif

// *****************************************************************************
/* Enumeration:
    enum leMonoDecoderMode

  Summary:
    Indicates the current mode of the mono image decoder
*/
enum leMonoDecoderMode
{
    LE_MONO_MODE_NONE,
    LE_MONO_MODE_DRAW,
    LE_MONO_MODE_COPY,
};

struct leMonoDecodeState;

// *****************************************************************************
/* Structure:
    struct leMonoDecodeStage

  Summary:
    Structure defining an individual mono image decoding stage

    struct leMonoDecodeState* state - pointer to the decoder state

    exec - function that runs this stage
    cleanup - function that cleans up this stage
*/
typedef struct leMonoDecodeStage
{
    struct leMonoDecodeState* state;

    leResult (*exec)(struct leMonoDecodeStage* stage);
    void (*cleanup)(struct leMonoDecodeStage* stage);
} leMonoDecodeStage;

typedef struct leMonoSourceReadOperation
{
    uint32_t x;
    uint32_t y;
    uint32_t bufferIndex;
    leColor data;
} leMonoSourceReadOperation;

// *****************************************************************************
/* Structure:
    struct leMonoDecodeState

  Summary:
    Structure defining the state of the mono image decoder
*/
typedef struct leMonoDecodeState
{
#if LE_STREAMING_ENABLED == 1
    leStreamManager manager; // so this decoder can act as a streaming manager
                             // if necessary
#endif

    const leImage* source; // the source image
    leImage* target; // destination image for copy/decompress mode

    enum leMonoDecoderMode mode; // the current mode of the decoder

    leRect sourceRect; // the image source rectangle
    leRect destRect; // the target rectangle dimensions

    int32_t targetX; // the current target X position
    int32_t targetY; // the current target Y position

    int32_t referenceX; // the current target X position
    int32_t referenceY; // the current target Y position

    uint32_t rowIterator; // the row iterator
    uint32_t colIterator; // the column iterator

    leMonoSourceReadOperation readOperation[LE_IMAGEDECODER_BLOCK_READ_SIZE]; // source data read requests
    uint32_t readCount;
    uint32_t readIndex;

    leColor writeColor;

    leMonoDecodeStage* stages[LE_IMAGEDECODER_MONO_MAX_STAGES];
    int32_t currentStage;
    int32_t done;

} leMonoDecodeState;

#endif /* LE_ENABLE_MONO_DECODER */

#endif /* LE_IMAGEDECODER_MONO_H */

