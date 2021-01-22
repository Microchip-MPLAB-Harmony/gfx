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

#include "gfx/legato/renderer/legato_renderer.h"

void _leRawImageDecoder_InjectStage(leRawDecodeState* state,
                                    leRawDecodeStage* stage);

static struct FrameBufferWriteStage
{
    leRawDecodeStage base;
} frameBufferWriteStage;

//#include <stdio.h>

#if LE_ALPHA_BLENDING_ENABLED == 1
static leResult stage_FrameBufferWrite(leRawDecodeStage* stage)
{
    // write color
    leRenderer_BlendPixel(stage->state->targetX,
                          stage->state->targetY,
                          stage->state->writeColor,
                          stage->state->globalAlpha);

    /*printf("%i, %i, %u\n", stage->state->targetX,
                           stage->state->targetY,
                           stage->state->writeColor);*/

    return LE_SUCCESS;
}
#else
static leResult stage_FrameBufferWrite(leRawDecodeStage* stage)
{
    // write color
    leRenderer_PutPixel(stage->state->targetX,
                        stage->state->targetY,
                        stage->state->writeColor);

    return LE_SUCCESS;
}
#endif

leResult _leRawImageDecoder_FrameBufferWriteStage(leRawDecodeState* state)
{
    memset(&frameBufferWriteStage, 0, sizeof(frameBufferWriteStage));

    frameBufferWriteStage.base.state = state;
    frameBufferWriteStage.base.exec = stage_FrameBufferWrite;

    _leRawImageDecoder_InjectStage(state, (void*) &frameBufferWriteStage);

    return LE_SUCCESS;
}

static struct ImageWriteStage
{
    leRawDecodeStage base;
} imageWriteStage;

static leResult stage_ImageWrite(leRawDecodeStage* stage)
{
    // write color
    lePixelBufferSet_Unsafe(&stage->state->target->buffer,
                            stage->state->targetX,
                            stage->state->targetY,
                            stage->state->writeColor);

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_ImageWriteStage(leRawDecodeState* state)
{
    memset(&imageWriteStage, 0, sizeof(imageWriteStage));

    imageWriteStage.base.state = state;
    imageWriteStage.base.exec = stage_ImageWrite;

    _leRawImageDecoder_InjectStage(state, (void*)&imageWriteStage);

    return LE_SUCCESS;
}

#endif /* LE_ENABLE_RAW_DECODER */