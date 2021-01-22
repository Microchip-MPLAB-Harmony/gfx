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

#include "gfx/legato/image/legato_palette.h"
#include "gfx/legato/renderer/legato_renderer.h"

void _leRawImageDecoder_InjectStage(leRawDecodeState* state,
                                    leRawDecodeStage* stage);

struct ConvertStage
{
    leRawDecodeStage base;

    leColorMode sourceMode;
};

static LE_COHERENT_ATTR struct ConvertStage convertStage;

static leResult stage_convertColor(leRawDecodeStage* stage)
{
    // convert the pixel to the destination format if necessary
    stage->state->writeColor = leColorConvert(convertStage.sourceMode,
                                              stage->state->targetMode,
                                              stage->state->writeColor);

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_ConvertStage(leRawDecodeState* state)
{
    // translate the source color mode to the destination color mode
    if((state->source->palette != NULL && state->source->palette->buffer.mode != state->targetMode) ||
        (state->source->buffer.mode != state->targetMode))
    {
        memset(&convertStage, 0, sizeof(convertStage));

        if(state->source->palette != NULL)
        {
            convertStage.sourceMode = state->source->palette->buffer.mode;
        }
        else
        {
            convertStage.sourceMode = state->source->buffer.mode;
        }

        convertStage.base.state = state;
        convertStage.base.exec = stage_convertColor;

        _leRawImageDecoder_InjectStage(state, (void*)&convertStage);
    }

    return LE_SUCCESS;
}

#endif /* LE_ENABLE_RAW_DECODER */