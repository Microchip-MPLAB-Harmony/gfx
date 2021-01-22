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

#include "gfx/legato/common/legato_math.h"

void _leRawImageDecoder_InjectStage(leRawDecodeState* state,
                                    leRawDecodeStage* stage);

static struct ScaleNearestNeighborPreReadStage
{
    leRawDecodeStage base;

    int32_t stretchRatioX; // x stretch ratio for resizing
    int32_t stretchRatioY; // y stretch ratio for resizing
} scaledNearestNeighborPreReadStage;

static leResult stage_scaledNearestNeighborPreRead(struct ScaleNearestNeighborPreReadStage* stage)
{
    stage->base.state->readCount = 1;

    // calculate nearest neighbor index
    stage->base.state->readOperation[0].x = (lePercentOf(stage->base.state->referenceX, stage->stretchRatioX) + 50) / 100;

    if(stage->base.state->readOperation[0].x >= (uint32_t)stage->base.state->source->buffer.size.width)
    {
        stage->base.state->readOperation[0].x = stage->base.state->source->buffer.size.width - 1;
    }

    stage->base.state->readOperation[0].y = (lePercentOf(stage->base.state->referenceY, stage->stretchRatioY) + 50) / 100;

    if(stage->base.state->readOperation[0].y >= (uint32_t)stage->base.state->source->buffer.size.height)
    {
        stage->base.state->readOperation[0].y = stage->base.state->source->buffer.size.height - 1;
    }

    // calculate source index
    stage->base.state->readOperation[0].bufferIndex = (stage->base.state->readOperation[0].x) +
                                                      ((stage->base.state->readOperation[0].y) *
                                                       stage->base.state->source->buffer.size.width);

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_ScaleNearestNeighborPreReadStage(leRawDecodeState* state)
{
    memset(&scaledNearestNeighborPreReadStage, 0, sizeof(scaledNearestNeighborPreReadStage));

    scaledNearestNeighborPreReadStage.base.state = state;
    scaledNearestNeighborPreReadStage.base.exec = (leResult(*)(struct leRawDecodeStage *))stage_scaledNearestNeighborPreRead;

    // calculate stretching ratios
    scaledNearestNeighborPreReadStage.stretchRatioX = lePercent(state->source->buffer.size.width, state->sizeX);
    scaledNearestNeighborPreReadStage.stretchRatioY = lePercent(state->source->buffer.size.height, state->sizeY);

    _leRawImageDecoder_InjectStage(state, (void*)&scaledNearestNeighborPreReadStage);

    return LE_SUCCESS;
}

static struct ScaleBilinearPreReadStage
{
    leRawDecodeStage base;

    int32_t stretchRatioX; // x stretch ratio for resizing
    int32_t stretchRatioY; // y stretch ratio for resizing

    int32_t px, py;
} scaledBilinearPreReadStage;

static leResult stage_bilinearPreRead(struct ScaleBilinearPreReadStage* stage)
{
    int32_t pxi, pyi;
    leRawDecodeState* state = stage->base.state;

    state->readCount = 4;

    // calculate offsets
    // calculate percentages and indices
    stage->px = lePercentOf(state->referenceX, stage->stretchRatioX);
    pxi = stage->px / 100;

    if(pxi >= stage->base.state->source->buffer.size.width)
    {
        pxi = stage->base.state->source->buffer.size.width - 1;
    }

    stage->py = lePercentOf(state->referenceY, stage->stretchRatioY);
    pyi = stage->py / 100;

    if(pyi >= stage->base.state->source->buffer.size.height)
    {
        pyi = stage->base.state->source->buffer.size.height - 1;
    }

    //pxi += stage->base.state->sourceRect.x;
    //pyi += stage->base.state->sourceRect.y;

    // first pixel read
    state->readOperation[0].x = pxi;
    state->readOperation[0].y = pyi;

    state->readOperation[0].bufferIndex = state->readOperation[0].x +
                                          (state->readOperation[0].y *
                                           state->source->buffer.size.width);

    // second pixel read
    state->readOperation[1].x = pxi;
    state->readOperation[1].y = pyi;

    if(state->readOperation[1].x < (uint32_t)stage->base.state->source->buffer.size.width - 1)
    {
        state->readOperation[1].x += 1;
    }

    state->readOperation[1].bufferIndex = state->readOperation[1].x +
                                         (state->readOperation[1].y *
                                          state->source->buffer.size.width);

    // third pixel read
    state->readOperation[2].x = pxi;
    state->readOperation[2].y = pyi;

    if(state->readOperation[2].y < (uint32_t)stage->base.state->source->buffer.size.height - 1)
    {
        state->readOperation[2].y += 1;
    }

    state->readOperation[2].bufferIndex = state->readOperation[2].x +
                                         (state->readOperation[2].y *
                                          state->source->buffer.size.width);

    // fourth pixel read
    state->readOperation[3].x = pxi;
    state->readOperation[3].y = pyi;

    if(state->readOperation[3].x < (uint32_t)stage->base.state->source->buffer.size.width - 1)
    {
        state->readOperation[3].x += 1;
    }

    if(state->readOperation[3].y < (uint32_t)stage->base.state->source->buffer.size.height - 1)
    {
        state->readOperation[3].y += 1;
    }

    state->readOperation[3].bufferIndex = state->readOperation[3].x +
                                         (state->readOperation[3].y *
                                          state->source->buffer.size.width);

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_ScaleBilinearPreReadStage(leRawDecodeState* state)
{
    memset(&scaledBilinearPreReadStage, 0, sizeof(scaledBilinearPreReadStage));

    scaledBilinearPreReadStage.base.state = state;
    scaledBilinearPreReadStage.base.exec = (leResult(*)(struct leRawDecodeStage *))stage_bilinearPreRead;

    // calculate stretching ratios
    scaledBilinearPreReadStage.stretchRatioX = lePercent(state->source->buffer.size.width, state->sizeX);
    scaledBilinearPreReadStage.stretchRatioY = lePercent(state->source->buffer.size.height, state->sizeY);

    _leRawImageDecoder_InjectStage(state, (void*)&scaledBilinearPreReadStage);

    return LE_SUCCESS;
}

static struct PostReadStage
{
    leRawDecodeStage base;
} postReadStage;

static leResult stage_postRead(leRawDecodeStage* stage)
{
    stage->state->writeColor = stage->state->readOperation[0].data;

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_PostReadStage(leRawDecodeState* state)
{
    memset(&postReadStage, 0, sizeof(postReadStage));

    postReadStage.base.state = state;
    postReadStage.base.exec = stage_postRead;

    _leRawImageDecoder_InjectStage(state, (void*)&postReadStage);

    return LE_SUCCESS;
}

static struct ScaleBilinearPostReadStage
{
    leRawDecodeStage base;
} scaledBilinearPostReadStage;

static leResult stage_scaledBilinearPostRead(leRawDecodeStage* stage)
{
    stage->state->writeColor = leColorBilerp(stage->state->readOperation[0].data,
                                             stage->state->readOperation[1].data,
                                             stage->state->readOperation[2].data,
                                             stage->state->readOperation[3].data,
                                             scaledBilinearPreReadStage.px % 100,
                                             scaledBilinearPreReadStage.py % 100,
                                             stage->state->source->buffer.mode);

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_ScaleBilinearPostReadStage(leRawDecodeState* state)
{
    memset(&scaledBilinearPostReadStage, 0, sizeof(scaledBilinearPostReadStage));

    scaledBilinearPostReadStage.base.state = state;
    scaledBilinearPostReadStage.base.exec = stage_scaledBilinearPostRead;

    _leRawImageDecoder_InjectStage(state, (void*)&scaledBilinearPostReadStage);

    return LE_SUCCESS;
}

#endif /* LE_ENABLE_RAW_DECODER */

