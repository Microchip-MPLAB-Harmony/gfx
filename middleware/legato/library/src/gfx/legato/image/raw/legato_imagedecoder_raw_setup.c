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


#include "gfx/legato/image/raw/legato_imagedecoder_raw.h"

#if LE_ENABLE_RAW_DECODER == 1

void _leRawImageDecoder_InjectStage(leRawDecodeState* state,
                                    leRawDecodeStage* stage);

static struct SetupStage
{
    leRawDecodeStage base;
} setupStage;

static leResult stage_sourceIterateSetup(leRawDecodeStage* stage)
{
    leRawDecodeState* state = stage->state;

    if(state->rowIterator >= (uint32_t)state->sourceRect.height)
    {
        state->currentStage = 0;
        state->done = LE_TRUE;

        return LE_SUCCESS;
    }

    state->referenceX = state->sourceRect.x + state->colIterator;
    state->referenceY = state->sourceRect.y + state->rowIterator;

    // calculate target coordinates
    state->targetX = state->destRect.x + state->colIterator;
    state->targetY = state->destRect.y + state->rowIterator;

    state->readCount = 1;

    state->readOperation[0].x = state->sourceRect.x + state->colIterator;
    state->readOperation[0].y = state->sourceRect.y + state->rowIterator;

    // calculate buffer index
    state->readOperation[0].bufferIndex = state->readOperation[0].x +
                                         (state->readOperation[0].y *
                                          stage->state->source->buffer.size.width);

    if(state->colIterator < (uint32_t)state->sourceRect.width - 1)
    {
        state->colIterator += 1;
    }
    else
    {
        state->colIterator = 0;
        state->rowIterator += 1;
    }

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_SourceIterateSetupStage(leRawDecodeState* state)
{
    memset(&setupStage, 0, sizeof(setupStage));

    state->rowIterator = 0;
    state->colIterator = 0;

    setupStage.base.state = state;
    setupStage.base.exec = stage_sourceIterateSetup;

    _leRawImageDecoder_InjectStage(state, (void*)&setupStage);

    return LE_SUCCESS;
}

static leResult stage_targetIterateSetup(leRawDecodeStage* stage)
{
    leRawDecodeState* state = stage->state;

    state->referenceX = state->sourceRect.x + state->colIterator;
    state->referenceY = state->sourceRect.y + state->rowIterator;

    // calculate source offset
    state->targetY = stage->state->destRect.y + state->rowIterator;
    state->targetX = stage->state->destRect.x + state->colIterator;

    if(state->colIterator < (uint32_t)state->destRect.width)
    {
        state->colIterator += 1;
    }

    if(state->colIterator == (uint32_t)state->destRect.width)
    {
        state->colIterator = 0;
        state->rowIterator += 1;

        if(state->rowIterator >= (uint32_t)state->destRect.height)
        {
            state->currentStage = 0;
            state->done = LE_TRUE;

            return LE_SUCCESS;
        }
    }

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_TargetIterateSetupStage(leRawDecodeState* state)
{
    memset(&setupStage, 0, sizeof(setupStage));

    state->rowIterator = 0;
    state->colIterator = 0;

    setupStage.base.state = state;
    setupStage.base.exec = stage_targetIterateSetup;

    _leRawImageDecoder_InjectStage(state, (void*)&setupStage);

    return LE_SUCCESS;
}

#endif /* LE_ENABLE_RAW_DECODER */