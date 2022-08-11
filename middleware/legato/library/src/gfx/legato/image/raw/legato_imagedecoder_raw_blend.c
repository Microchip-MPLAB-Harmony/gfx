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

struct InternalBlendStage
{
    leRawDecodeStage base;
};

static LE_COHERENT_ATTR struct InternalBlendStage blendStage;

static leResult stage_BlendRGBA5551(leRawDecodeStage* stage)
{
    // completely transparent, discard
    if((stage->state->writeColor & RGBA_5551_ALPHA_MASK) == 0)
    {
        // reset stage index
        stage->state->currentStage = -1;
    }

    return LE_SUCCESS;
}

static leResult stage_BlendRGBA8888(leRawDecodeStage* stage)
{
    leColor resultClr;

    // completely transparent, discard
    if((stage->state->writeColor & RGBA_8888_ALPHA_MASK) == 0)
    {
        // reset stage index
        stage->state->currentStage = -1;
    }
    else
    {
        resultClr = leRenderer_GetPixel(blendStage.base.state->targetX,
                                        blendStage.base.state->targetY);

        resultClr = leColorConvert(stage->state->targetMode,
                                   LE_COLOR_MODE_RGBA_8888,
                                   resultClr);

        stage->state->writeColor = leColorBlend_RGBA_8888(stage->state->writeColor, resultClr);
    }

    return LE_SUCCESS;
}

static leResult stage_BlendARGB8888(leRawDecodeStage* stage)
{
    leColor resultClr;

    // completely transparent, discard
    if((stage->state->writeColor & ARGB_8888_ALPHA_MASK) == 0)
    {
        // reset stage index
        stage->state->currentStage = -1;
    }
    else
    {
        resultClr = leRenderer_GetPixel(blendStage.base.state->targetX,
                                        blendStage.base.state->targetY);

        resultClr = leColorConvert(stage->state->targetMode,
                                   LE_COLOR_MODE_ARGB_8888,
                                   resultClr);

        stage->state->writeColor = leColorBlend_ARGB_8888(stage->state->writeColor, resultClr);
    }

    return LE_SUCCESS;
}

void _leRawImageDecoder_BlendStage_Internal(leRawDecodeState* state)
{
    memset(&blendStage, 0, sizeof(blendStage));

    if((state->source->flags & LE_IMAGE_USE_ALPHA_MAP) > 0 &&
       state->source->alphaMap != NULL)
    {
        // TODO alpha map support
    }
    else if(state->source->buffer.mode == LE_COLOR_MODE_RGBA_5551)
    {
        blendStage.base.exec = stage_BlendRGBA5551;
    }
    else if(state->source->buffer.mode == LE_COLOR_MODE_RGBA_8888)
    {
        blendStage.base.exec = stage_BlendRGBA8888;
    }
    else if(state->source->buffer.mode == LE_COLOR_MODE_ARGB_8888)
    {
        blendStage.base.exec = stage_BlendARGB8888;
    }
    else
    {
        return;
    }

    blendStage.base.state = state;

    _leRawImageDecoder_InjectStage(state, (void*)&blendStage);
}

#endif /* LE_ENABLE_RAW_DECODER */