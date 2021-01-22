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

struct InternalMaskStage
{
    leRawDecodeStage base;
};

struct LE_COHERENT_ATTR InternalMaskStage maskStage;

static leResult stage_rejectMaskedColor(leRawDecodeStage* stage)
{
    // compare color to mask
    if(stage->state->writeColor == stage->state->source->mask.color)
    {
        // reset stage index
        stage->state->currentStage = -1;
    }

    return LE_SUCCESS;
}

void _leRawImageDecoder_MaskStage_Internal(leRawDecodeState* state)
{
    memset(&maskStage, 0, sizeof(maskStage));

    maskStage.base.state = state;
    maskStage.base.exec = stage_rejectMaskedColor;

    _leRawImageDecoder_InjectStage(state, (void*)&maskStage);
}

#endif /* LE_ENABLE_RAW_DECODER */