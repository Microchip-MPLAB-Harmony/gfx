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


#include "gfx/legato/image/mono/legato_imagedecoder_mono.h"

#if LE_ENABLE_MONO_DECODER == 1

void _leMonoImageDecoder_InjectStage(leMonoDecodeState* state,
                                     leMonoDecodeStage* stage);

struct InternalReadStage
{
    leMonoDecodeStage base;
};

static struct InternalReadStage LE_COHERENT_ATTR internalReadStage;

static leResult stage_decode(struct InternalReadStage* stage)
{
    (void)stage; // unused

    leMonoSourceReadOperation* op = &internalReadStage.base.state->readOperation[internalReadStage.base.state->readIndex];

    // get a pixel from the sourced image
    stage->base.state->writeColor = lePixelBufferGet_Unsafe(&internalReadStage.base.state->source->buffer,
                                                            op->x,
                                                            op->y);

    return LE_SUCCESS;
}

leResult _leMonoImageDecoder_ReadStage_Internal(leMonoDecodeState* state)
{
    memset(&internalReadStage, 0, sizeof(internalReadStage));

    //internalReadStage.dataSize = leColorInfoTable[state->source->buffer.mode].size;
    //internalReadStage.imgBPP = leColorInfoTable[state->source->buffer.mode].bppOrdinal;

    internalReadStage.base.state = state;
    internalReadStage.base.exec = (leResult(*)(struct leMonoDecodeStage*)) stage_decode;

    _leMonoImageDecoder_InjectStage(state, (void*)&internalReadStage);

    return LE_SUCCESS;
}

#endif /* LE_ENABLE_MONO_DECODER */