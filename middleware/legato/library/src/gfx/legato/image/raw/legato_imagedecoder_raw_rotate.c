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

#include "gfx/legato/common/legato_math.h"

void _leRawImageDecoder_InjectStage(leRawDecodeState* state,
                                    leRawDecodeStage* stage);

static struct RotateNearestNeighborPreReadStage
{
    leRawDecodeStage base;

} rotateNearestNeighborPreReadStage;

//#include <stdio.h>

static leResult stage_rotateNearestNeighborPreRead(struct RotateNearestNeighborPreReadStage* stage)
{
    lePoint pnt;

    stage->base.state->readCount = 1;

    pnt.x = stage->base.state->colIterator;
    pnt.y = stage->base.state->rowIterator;

    // transform into cartesian space
    pnt.x -= stage->base.state->destRect.width / 2;
    pnt.y -= stage->base.state->destRect.height / 2;
    pnt.y *= -1;

    leRotatePoint(pnt,
                  lePoint_Zero,
                  stage->base.state->angle,
                  &pnt);

    // transform into source image space
    pnt.y *= -1;
    pnt.x += (stage->base.state->source->buffer.size.width / 2) - 1;
    pnt.y += (stage->base.state->source->buffer.size.height / 2) - 1;

    /*if(stage->base.state->referenceX == 0 && stage->base.state->referenceY == 0)
    {
        printf("[%i, %i] : %i : [%i, %i]\n", stage->base.state->referenceX, stage->base.state->referenceY, stage->base.state->angle, pnt.x, pnt.y);
    }*/

    if(pnt.x < 0 || pnt.x >= stage->base.state->source->buffer.size.width ||
       pnt.y < 0 || pnt.y >= stage->base.state->source->buffer.size.height)
    {
        // this pixel position is invalid, reset the pipeline
        stage->base.state->currentStage = -1;

        return LE_SUCCESS;
    }

    stage->base.state->readOperation[0].x = pnt.x;
    stage->base.state->readOperation[0].y = pnt.y;

    // calculate source index
    stage->base.state->readOperation[0].bufferIndex = (stage->base.state->readOperation[0].x) +
                                                      ((stage->base.state->readOperation[0].y) *
                                                       stage->base.state->source->buffer.size.width);

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_RotateNearestNeighborPreReadStage(leRawDecodeState* state)
{
    memset(&rotateNearestNeighborPreReadStage, 0, sizeof(rotateNearestNeighborPreReadStage));

    rotateNearestNeighborPreReadStage.base.state = state;
	rotateNearestNeighborPreReadStage.base.exec = (leResult(*)(struct leRawDecodeStage *))stage_rotateNearestNeighborPreRead;

    _leRawImageDecoder_InjectStage(state, (void*)&rotateNearestNeighborPreReadStage);

    return LE_SUCCESS;
}

static struct RotateBilinearPreReadStage
{
    leRawDecodeStage base;

    int32_t px, py;
} rotateBilinearPreReadStage;

static leResult stage_bilinearPreRead(struct RotateBilinearPreReadStage* stage)
{
    leRawDecodeState* state = stage->base.state;
    lePoint readPoint, filterPoint;

    readPoint.x = stage->base.state->colIterator;
    readPoint.y = stage->base.state->rowIterator;

    // transform into cartesian space
    readPoint.x -= stage->base.state->destRect.width / 2;
    readPoint.y -= stage->base.state->destRect.height / 2;
    readPoint.y *= -1;

    leRotatePoint(readPoint,
                  lePoint_Zero,
                  stage->base.state->angle,
                  &readPoint);

    // transform into source image space
    readPoint.y *= -1;
    readPoint.x += (stage->base.state->source->buffer.size.width / 2) - 1;
    readPoint.y += (stage->base.state->source->buffer.size.height / 2) - 1;

    if(readPoint.x < 0 || readPoint.x >= stage->base.state->source->buffer.size.width ||
       readPoint.y < 0 || readPoint.y >= stage->base.state->source->buffer.size.height)
    {
        // this pixel position is invalid, reset the pipeline
        stage->base.state->currentStage = -1;

        return LE_SUCCESS;
    }

    // if we're right on the edge, do a nearest neighbor instead of a bilinear blend
    if(readPoint.x == stage->base.state->source->buffer.size.width - 1 ||
       readPoint.y == stage->base.state->source->buffer.size.height - 1)
    {
        state->readCount = 1;

        stage->base.state->readOperation[0].x = readPoint.x;
        stage->base.state->readOperation[0].y = readPoint.y;

        state->readOperation[0].bufferIndex = readPoint.x + (readPoint.y * state->source->buffer.size.width);
    }
    else
    {
        state->readCount = 4;

        // first point
        filterPoint = readPoint;

        stage->base.state->readOperation[0].x = filterPoint.x;
        stage->base.state->readOperation[0].y = filterPoint.y;

        state->readOperation[0].bufferIndex = readPoint.x + (readPoint.y * state->source->buffer.size.width);

        // second
        filterPoint = readPoint;

        if(filterPoint.x < stage->base.state->source->buffer.size.width - 1)
        {
            filterPoint.x += 1;
        }

        stage->base.state->readOperation[1].x = filterPoint.x;
        stage->base.state->readOperation[1].y = filterPoint.y;

        state->readOperation[1].bufferIndex = filterPoint.x + (filterPoint.y * state->source->buffer.size.width);

        // third
        filterPoint = readPoint;

        if(filterPoint.y < stage->base.state->source->buffer.size.height - 1)
        {
            filterPoint.y += 1;
        }

        stage->base.state->readOperation[2].x = filterPoint.x;
        stage->base.state->readOperation[2].y = filterPoint.y;

        state->readOperation[2].bufferIndex = filterPoint.x + (filterPoint.y * state->source->buffer.size.width);

        // fourth
        filterPoint = readPoint;

        if(filterPoint.x < stage->base.state->source->buffer.size.width - 1)
        {
            filterPoint.x += 1;
        }

        if(filterPoint.y < stage->base.state->source->buffer.size.height - 1)
        {
            filterPoint.y += 1;
        }

        stage->base.state->readOperation[3].x = filterPoint.x;
        stage->base.state->readOperation[3].y = filterPoint.y;

        state->readOperation[3].bufferIndex = filterPoint.x + (filterPoint.y * state->source->buffer.size.width);
    }

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_RotateBilinearPreReadStage(leRawDecodeState* state)
{
    memset(&rotateBilinearPreReadStage, 0, sizeof(rotateBilinearPreReadStage));

    rotateBilinearPreReadStage.base.state = state;
    rotateBilinearPreReadStage.base.exec = (leResult(*)(struct leRawDecodeStage *))stage_bilinearPreRead;

    _leRawImageDecoder_InjectStage(state, (void*)&rotateBilinearPreReadStage);

    return LE_SUCCESS;
}

static struct RotateBilinearPostReadStage
{
    leRawDecodeStage base;
} rotateBilinearPostReadStage;

static leResult stage_rotateBilinearPostRead(leRawDecodeStage* stage)
{
    if(stage->state->readCount == 1)
    {
        stage->state->writeColor = stage->state->readOperation[0].data;
    }
    else
    {
        stage->state->writeColor = leColorBilerp(stage->state->readOperation[0].data,
                                                 stage->state->readOperation[1].data,
                                                 stage->state->readOperation[2].data,
                                                 stage->state->readOperation[3].data,
                                                 50,
                                                 50,
                                                 stage->state->source->buffer.mode);
    }

    return LE_SUCCESS;
}

leResult _leRawImageDecoder_RotateBilinearPostReadStage(leRawDecodeState* state)
{
    memset(&rotateBilinearPostReadStage, 0, sizeof(rotateBilinearPostReadStage));

    rotateBilinearPostReadStage.base.state = state;
    rotateBilinearPostReadStage.base.exec = stage_rotateBilinearPostRead;

    _leRawImageDecoder_InjectStage(state, (void*)&rotateBilinearPostReadStage);

    return LE_SUCCESS;
}