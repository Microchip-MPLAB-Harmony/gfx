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

#include "gfx/legato/common/legato_pixelbuffer.h"
#include "gfx/legato/image/legato_palette.h"
#include "gfx/legato/renderer/legato_renderer.h"

void _leRawImageDecoder_InjectStage(leRawDecodeState* state,
                                    leRawDecodeStage* stage);

struct InternalPaletteStage
{
    leRawDecodeStage base;

    leColor lookupIndex;

    uint32_t paletteSize;
};

static LE_COHERENT_ATTR struct InternalPaletteStage paletteStage;

static leResult stage_lookup(struct InternalPaletteStage* stage)
{
    paletteStage.lookupIndex = stage->base.state->writeColor;

    // look up the actual color from the palette
    memcpy(&stage->base.state->writeColor,
           (void*)((uint8_t*)stage->base.state->source->palette->header.address + (paletteStage.lookupIndex * paletteStage.paletteSize)),
           paletteStage.paletteSize);

    return LE_SUCCESS;
}

void _leRawImageDecoder_LookupStage_Internal(leRawDecodeState* state)
{
    memset(&paletteStage, 0, sizeof(paletteStage));

    paletteStage.base.state = state;

	paletteStage.base.exec = (leResult(*)(struct leRawDecodeStage *))stage_lookup;
    paletteStage.paletteSize = leColorInfoTable[state->source->palette->buffer.mode].size;

    _leRawImageDecoder_InjectStage(state, (void*)&paletteStage);
}

static struct ImageRenderPostLookupStage
{
    leRawDecodeStage base;
} imageRenderPostLookupStage;

static leResult stage_imageRenderPostLookup(leRawDecodeStage* stage)
{
    if(stage->state->needToLookupMaskColor == LE_TRUE &&
       paletteStage.lookupIndex == stage->state->target->mask.color)
    {
        stage->state->target->mask.color = stage->state->writeColor;

        stage->state->needToLookupMaskColor = LE_FALSE;
    }

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_ImageRenderPostLookupStage(leRawDecodeState* state)
{
    memset(&imageRenderPostLookupStage, 0, sizeof(imageRenderPostLookupStage));

    state->needToLookupMaskColor = LE_TRUE;

    imageRenderPostLookupStage.base.state = state;
    imageRenderPostLookupStage.base.exec = stage_imageRenderPostLookup;

    _leRawImageDecoder_InjectStage(state, (void*)&imageRenderPostLookupStage);

    return LE_SUCCESS;
}

#endif /* LE_ENABLE_RAW_DECODER */