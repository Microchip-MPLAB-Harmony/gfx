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

#include "gfx/legato/renderer/legato_renderer.h"

#include "gfx/legato/common/legato_math.h"

#include <math.h>
#include <limits.h>

#define ARC_MAX 50000

typedef struct ArcDrawState
{
    leRect arcArea;
    int32_t x;
    int32_t y;
    float fiCirRad;
    float foCirRad;
    lePoint topRectPt;
    lePoint botRectPt;
    uint32_t thickness;
    leColor clr;
    uint32_t alpha;
} ArcDrawState;

leResult leRenderer_CircleDraw(const leRect* rect,
                               uint32_t thickness,
                               leColor clr,
                               uint32_t alpha)
{
    leRenderer_ArcFill(rect,
                       0,
                       360,
                       thickness,
                       LE_FALSE,
                       clr,
                       LE_FALSE,
                       alpha);

    return LE_SUCCESS;
}

leResult leRenderer_CircleFill(const leRect* rect,
                               uint32_t thickness,
                               leColor borderClr,
                               leColor fillClr,
                               uint32_t alpha)
{
    uint32_t radius = rect->width / 2;



    leRect fillRect;
    fillRect.x = rect->x + 1 + rect->width / 4;
    fillRect.y = rect->y + 1 + rect->height / 4;
    fillRect.width = radius;
    fillRect.height = radius;

    leRenderer_ArcFill(&fillRect,
                       0,
                       360,
                       radius,
                       LE_FALSE,
                       fillClr,
                       LE_FALSE,
                       alpha);

    leRenderer_ArcFill(rect,
                       0,
                       360,
                       thickness,
                       LE_FALSE,
                       borderClr,
                       LE_FALSE,
                       alpha);

    return LE_SUCCESS;
}

leResult leRenderer_ArcLine(int32_t x,
                            int32_t y, 
                            int32_t r,
                            int32_t startAngle,
                            int32_t centerAngle,
                            leColor clr,
                            uint32_t a)
{
    int32_t sa = startAngle;
    lePoint p;
    leRect drawRect;
    (void)a; // unused

    leRenderer_GetClipRect(&drawRect);

    while (sa != (startAngle + startAngle))
    {
        lePolarToXY(r, sa, &p);
        
        p.x += x;
        p.y += y;
        
        if(leRectContainsPoint(&drawRect, &p) == LE_TRUE)
        {
            leRenderer_PutPixel(p.x, p.y, clr);
        }
        
        //Clockwise
        if (centerAngle < 0)
        {
            sa--;
        }
        //Counter-clockwise
        else
        {
            sa++;
        }
    }

    return LE_SUCCESS;
}

static void drawQ1(const ArcDrawState* state,
                   int32_t startAngle,
                   int32_t endAngle)
{
    lePoint testPt;
    lePoint line0, line1;
    int32_t x, y;

    int32_t width = state->botRectPt.x;
    int32_t height = state->botRectPt.y;

    int32_t centerX = (state->arcArea.x + state->arcArea.width / 2);
    int32_t centerY = (state->arcArea.y + state->arcArea.height / 2);

    leBool partial = !(startAngle <= 0 && endAngle >= 90);

    lePoint pt0Sin;
    lePoint pt1Sin;

    if(partial == LE_TRUE)
    {
        if(startAngle == 0)
        {
            line0.x = 100;
            line0.y = 0;
        }
        else
        {
            lePolarToXY(100, startAngle, &line0);
        }

        if(endAngle >= 90)
        {
            line1.x = 0;
            line1.y = 100;
        }
        else
        {
            lePolarToXY(100, endAngle, &line1);
        }

        pt0Sin.x = -ARC_MAX;
        pt0Sin.y = ARC_MAX;
        pt1Sin.x = ARC_MAX;
        pt1Sin.y = -ARC_MAX;
    }

    for(y = 1; y <= height; y++)
    {
        testPt.y = y;

        for(x = 1; x <= width; x++)
        {
            int32_t mag = (x * x) + (y * y);
            float rad = leSqrt((float)mag);
            rad = leRound(rad);

            // if point is outside outer circle, skip
            if(!(rad >= state->fiCirRad && rad <= state->foCirRad))
                continue;

            testPt.x = x;

            if(partial == LE_TRUE)
            {
                if(lePointOnLineSide(&testPt, &line0, &pt0Sin) == LE_FALSE ||
                   lePointOnLineSide(&testPt, &line1, &pt1Sin) == LE_FALSE)
                    continue;
            }

#if LE_ALPHA_BLENDING_ENABLED == 1
            leRenderer_BlendPixel_Safe(centerX + x - 1, centerY - y, state->clr, state->alpha);
#else
            leRenderer_PutPixel_Safe(centerX + x - 1, centerY - y, state->clr);
#endif
        }
    }
}

static void drawQ2(const ArcDrawState* state,
                   int32_t startAngle,
                   int32_t endAngle)
{
    lePoint testPt;
    lePoint line0, line1;
    int32_t x, y;

    int32_t width = state->botRectPt.x;
    int32_t height = state->botRectPt.y;

    int32_t centerX = (state->arcArea.x + state->arcArea.width / 2);
    int32_t centerY = (state->arcArea.y + state->arcArea.height / 2);

    leBool partial = !(startAngle <= 90 && endAngle >= 180);

    lePoint pt0Sin;
    lePoint pt1Sin;

    if(partial == LE_TRUE)
    {
        if(startAngle <= 90)
        {
            line0.x = 0;
            line0.y = 100;
        }
        else
        {
            lePolarToXY(100, startAngle, &line0);
        }

        if(endAngle >= 180)
        {
            line1.x = -100;
            line1.y = 0;
        }
        else
        {
            lePolarToXY(100, endAngle, &line1);
        }

        pt0Sin.x = -ARC_MAX;
        pt0Sin.y = -ARC_MAX;
        pt1Sin.x = ARC_MAX;
        pt1Sin.y = ARC_MAX;
    }

    for(y = 1; y <= height; y++)
    {
        testPt.y = y;

        for(x = -width; x < 0; x++)
        {
            int32_t mag = (x * x) + (y * y);
            float rad = leSqrt((float)mag);
            rad = leRound(rad);

            // if point is outside outer circle, skip
            if(!(rad >= state->fiCirRad && rad <= state->foCirRad))
                continue;

            testPt.x = x;

            if(partial == LE_TRUE)
            {
                if(lePointOnLineSide(&testPt, &line0, &pt0Sin) == LE_FALSE ||
                   lePointOnLineSide(&testPt, &line1, &pt1Sin) == LE_FALSE)
                    continue;
            }

#if LE_ALPHA_BLENDING_ENABLED == 1
            leRenderer_BlendPixel_Safe(centerX + x, centerY - y, state->clr, state->alpha);
#else
            leRenderer_PutPixel_Safe(centerX + x, centerY - y, state->clr);
#endif
        }
    }
}

static void drawQ3(const ArcDrawState* state,
                   int32_t startAngle,
                   int32_t endAngle)
{
    lePoint testPt;
    lePoint line0, line1;
    int32_t x, y;

    int32_t width = state->botRectPt.x;
    int32_t height = state->botRectPt.y;

    int32_t centerX = (state->arcArea.x + state->arcArea.width / 2);
    int32_t centerY = (state->arcArea.y + state->arcArea.height / 2);

    leBool partial = !(startAngle <= 180 && endAngle >= 270);

    lePoint pt0Sin;
    lePoint pt1Sin;

    if(partial == LE_TRUE)
    {
        if(startAngle <= 180)
        {
            line0.x = -100;
            line0.y = 0;
        }
        else
        {
            lePolarToXY(100, startAngle, &line0);
        }

        if(endAngle >= 270)
        {
            line1.x = 0;
            line1.y = -100;
        }
        else
        {
            lePolarToXY(100, endAngle, &line1);
        }

        pt0Sin.x = ARC_MAX;
        pt0Sin.y = -ARC_MAX;
        pt1Sin.x = -ARC_MAX;
        pt1Sin.y = ARC_MAX;
    }

    for(y = 0; y <= height; y++)
    {
        testPt.y = -y;

        for(x = -width; x < 0; x++)
        {
            int32_t mag = (x * x) + (y * y);
            float rad = leSqrt((float)mag);
            rad = leRound(rad);

            // if point is outside outer circle, skip
            if(!(rad >= state->fiCirRad && rad <= state->foCirRad))
                continue;

            testPt.x = x;

            if(partial == LE_TRUE)
            {
                if(lePointOnLineSide(&testPt, &line0, &pt0Sin) == LE_FALSE ||
                   lePointOnLineSide(&testPt, &line1, &pt1Sin) == LE_FALSE)
                    continue;
            }

#if LE_ALPHA_BLENDING_ENABLED == 1
            leRenderer_BlendPixel_Safe(centerX + x, centerY + y - 1, state->clr, state->alpha);
#else
            leRenderer_PutPixel_Safe(centerX + x, centerY + y - 1, state->clr);
#endif
        }
    }
}

static void drawQ4(const ArcDrawState* state,
                   int32_t startAngle,
                   int32_t endAngle)
{
    lePoint testPt;
    lePoint line0, line1;
    int32_t x, y;

    int32_t width = state->botRectPt.x;
    int32_t height = state->botRectPt.y;

    int32_t centerX = (state->arcArea.x + state->arcArea.width / 2);
    int32_t centerY = (state->arcArea.y + state->arcArea.height / 2);

    leBool partial = !(startAngle <= 270 && endAngle >= 360);

    lePoint pt0Sin;
    lePoint pt1Sin;

    if(partial == LE_TRUE)
    {
        if(startAngle <= 270)
        {
            line0.x = 0;
            line0.y = -100;
        }
        else
        {
            lePolarToXY(100, startAngle, &line0);
        }

        if(endAngle >= 360)
        {
            line1.x = 100;
            line1.y = 0;
        }
        else
        {
            lePolarToXY(100, endAngle, &line1);
        }

        pt0Sin.x = ARC_MAX;
        pt0Sin.y = ARC_MAX;
        pt1Sin.x = -ARC_MAX;
        pt1Sin.y = -ARC_MAX;
    }

    for(y = 1; y <= height; y++)
    {
        testPt.y = -y;

        for(x = 1; x <= width; x++)
        {
            int32_t mag = (x * x) + (y * y);
            float rad = leSqrt((float)mag);
            rad = leRound(rad);

            // if point is outside outer circle, skip
            if(!(rad >= state->fiCirRad && rad <= state->foCirRad))
                continue;

            testPt.x = x;

            if(partial == LE_TRUE)
            {
                if(lePointOnLineSide(&testPt, &line0, &pt0Sin) == LE_FALSE ||
                   lePointOnLineSide(&testPt, &line1, &pt1Sin) == LE_FALSE)
                    continue;
            }

#if LE_ALPHA_BLENDING_ENABLED == 1
            leRenderer_BlendPixel_Safe(centerX + x - 1, centerY + y - 1, state->clr, state->alpha);
#else
            leRenderer_PutPixel_Safe(centerX + x - 1, centerY + y - 1, state->clr);
#endif
        }
    }
}

leResult leRenderer_ArcFill(const leRect* drawRect,
                            int32_t startAngle,
                            int32_t spanAngle,
                            uint32_t thickness,
                            leBool rounded,
                            leColor clr,
                            leBool antialias,
                            uint32_t a)
{
    ArcDrawState state;
    int32_t r = drawRect->width / 2;
    (void)antialias; // unused

    leRect arcDrawRect = *drawRect;
    arcDrawRect.x -= thickness / 2;
    arcDrawRect.y -= thickness / 2;
    arcDrawRect.width += thickness;
    arcDrawRect.height += thickness;

    if(leRenderer_CullDrawRect(&arcDrawRect) == LE_TRUE)
        return LE_FAILURE;

    state.arcArea = *drawRect;

    // don't care about wrapping
    if (spanAngle == 0 || thickness == 0)
        return LE_SUCCESS;

    leResolvedAngleRanges ranges;

    leResolveAngles(startAngle, spanAngle, &ranges);

    // determine bounding rectangle points
    if(thickness <= 1)
    {
        state.topRectPt.x = -(r);
        state.topRectPt.y = -(r);
        state.botRectPt.x = (r);
        state.botRectPt.y = (r);

        state.foCirRad = r;
        state.fiCirRad = r;
    }
    else if(thickness % 2 > 0)
    {
        int32_t adjThick = (thickness / 2) + 1;

        state.topRectPt.x = -(r + adjThick);
        state.topRectPt.y = -(r + adjThick);
        state.botRectPt.x = (r + adjThick);
        state.botRectPt.y = (r + adjThick);

        state.foCirRad = r + adjThick - 1;
        state.fiCirRad = r - (thickness / 2);
    }
    else
    {
        state.topRectPt.x = -(r + thickness / 2);
        state.topRectPt.y = -(r + thickness / 2);
        state.botRectPt.x = (r + thickness / 2);
        state.botRectPt.y = (r + thickness / 2);

        state.foCirRad = r + (thickness / 2) - 1;
        state.fiCirRad = r - (thickness / 2);
    }

    state.x = drawRect->x;
    state.y = drawRect->y;
    state.clr = clr;
    state.alpha = a;

    if(ranges.angleCount >= 1)
    {
        if(ranges.angle0.quadrants.q1 == LE_TRUE)
        {
            drawQ1(&state,
                   ranges.angle0.startAngle,
                   ranges.angle0.endAngle);
        }

        if(ranges.angle0.quadrants.q2 == LE_TRUE)
        {
            drawQ2(&state,
                   ranges.angle0.startAngle,
                   ranges.angle0.endAngle);
        }

        if(ranges.angle0.quadrants.q3 == LE_TRUE)
        {
            drawQ3(&state,
                   ranges.angle0.startAngle,
                   ranges.angle0.endAngle);
        }

        if(ranges.angle0.quadrants.q4 == LE_TRUE)
        {
            drawQ4(&state,
                   ranges.angle0.startAngle,
                   ranges.angle0.endAngle);
        }
    }

    if(ranges.angleCount == 2)
    {
        if(ranges.angle1.quadrants.q1 == LE_TRUE)
        {
            drawQ1(&state,
                   ranges.angle1.startAngle,
                   ranges.angle1.endAngle);
        }

        if(ranges.angle1.quadrants.q2 == LE_TRUE)
        {
            drawQ2(&state,
                   ranges.angle1.startAngle,
                   ranges.angle1.endAngle);
        }

        if(ranges.angle1.quadrants.q3 == LE_TRUE)
        {
            drawQ3(&state,
                   ranges.angle1.startAngle,
                   ranges.angle1.endAngle);
        }

        if(ranges.angle1.quadrants.q4 == LE_TRUE)
        {
            drawQ4(&state,
                   ranges.angle1.startAngle,
                   ranges.angle1.endAngle);
        }
    }

    if(rounded == LE_TRUE)
    {
        lePoint point;
        leRect tipRect;

        lePointOnCircle((drawRect->width / 2) + 1,
                        startAngle,
                        &point);

        point.y *= -1;

        tipRect.x = drawRect->x + (drawRect->width / 2) + point.x;
        tipRect.y = drawRect->y + (drawRect->height / 2) + point.y;

        tipRect.x -= thickness / 4;
        tipRect.y -= thickness / 4;
        tipRect.width = thickness / 2;
        tipRect.height = thickness / 2;

        leRenderer_CircleFill(&tipRect,
                              thickness / 2,
                              clr,
                              clr,
                              a);

        lePointOnCircle((drawRect->width / 2) + 1,
                        startAngle + spanAngle,
                        &point);

        point.y *= -1;

        tipRect.x = drawRect->x + (drawRect->width / 2) + point.x;
        tipRect.y = drawRect->y + (drawRect->height / 2) + point.y;

        tipRect.x -= thickness / 4;
        tipRect.y -= thickness / 4;
        tipRect.width = thickness / 2;
        tipRect.height = thickness / 2;

        leRenderer_CircleFill(&tipRect,
                              thickness / 2,
                              clr,
                              clr,
                              a);
    }

    return LE_SUCCESS;
}
