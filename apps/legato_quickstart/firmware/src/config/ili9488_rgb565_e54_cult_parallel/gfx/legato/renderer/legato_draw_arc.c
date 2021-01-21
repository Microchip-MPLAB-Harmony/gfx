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

/* renderer function use only */
extern leRenderState _rendererState;

typedef enum 
{
    ON_LINE,
    LEFT_OF_LINE,
    RIGHT_OF_LINE,
} POINT_LINE_POS;

typedef struct ArcDrawState
{
    leRect arcArea;
    int32_t x;
    int32_t y;
    int32_t endAngle;
    uint32_t iCirRadSqrd;
    uint32_t oCirRadSqrd;
    lePoint arcStartPt0;
    lePoint arcStartPt1;
    lePoint arcEndPt0;
    lePoint arcEndPt1;
    lePoint topRectPt;
    lePoint botRectPt;
    uint32_t absStartAngle;
    uint32_t absEndAngle;
    leArcDir dir;
    uint32_t thickness;
    leColor clr;
    uint32_t alpha;
} ArcDrawState;

leResult leRenderer_CircleDraw(const leRect* rect,
                               int32_t x,
                               int32_t y,
                               uint32_t radius,
                               uint32_t thickness,
                               leColor clr,
                               uint32_t alpha)
{
    leRenderer_ArcFill(rect,
                       x,
                       y,
                       radius,
                       0,
                       360,
                       thickness,
                       clr,
                       LE_FALSE,
                       alpha);

    return LE_SUCCESS;
}

leResult leRenderer_CircleFill(const leRect* rect,
                               int32_t x,
                               int32_t y,
                               uint32_t radius,
                               uint32_t thickness,
                               leColor borderClr,
                               leColor fillClr,
                               uint32_t alpha)
{
    leRenderer_ArcFill(rect,
                       x,
                       y,
                       radius,
                       0,
                       360,
                       thickness,
                       borderClr,
                       LE_FALSE,
                       alpha);

    if(thickness < radius)
    {
        leRenderer_ArcFill(rect,
                           x,
                           y,
                           radius - thickness,
                           0,
                           360,
                           radius - thickness,
                           fillClr,
                           LE_FALSE,
                           alpha);
    }

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

    while (sa != (startAngle + startAngle))
    {
        lePolarToXY(r, sa, &p);
        
        p.x += x;
        p.y += y;
        
        if(leRectContainsPoint(&_rendererState.drawRect, &p) == LE_TRUE)
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

// This function returns the relative horizontal position of a point from a line.
// Works only if test point and line are in the same half plane (Q1 & Q2, or Q3 & Q4).
POINT_LINE_POS pointRelPositionFromLine(const lePoint* linePt0,
                                        const lePoint* linePt1,
                                        const lePoint* point)
{
    int sign = (linePt1->x - linePt0->x) * (point->y - linePt0->y) - 
               (linePt1->y - linePt0->y) * (point->x - linePt0->x);

    if (linePt1->y < 0)
    {
        sign = -sign;
    }
    
    if (sign > 0)
    {
        return LEFT_OF_LINE;
    }
    else if (sign < 0)
    {
        return RIGHT_OF_LINE;
    }

    return ON_LINE;
}

/*
static leColor getArcSoftEdgeColor(uint32_t oRadSqd, 
                                   uint32_t iRadSqd, 
                                   uint32_t thicknessSqd,
                                   uint32_t ptRadSqd,
                                   leGradient* gradient)
{
    leColor color;

    //test with outer radius
    if ((oRadSqd - ptRadSqd) < 8)
    {
        color = leColorLerp(gradient->c0,
                            gradient->c1,
                            20,
                            LE_GLOBAL_COLOR_MODE);
    }
    else if ((oRadSqd - ptRadSqd) < 32)
    {
        color = leColorLerp(gradient->c0,
                            gradient->c1,
                            40,
                            LE_GLOBAL_COLOR_MODE);
    }
    else if ((oRadSqd - ptRadSqd) < 50)
    {
        color = leColorLerp(gradient->c0,
                            gradient->c1,
                            60,
                            LE_GLOBAL_COLOR_MODE);
    }    
    else if ((oRadSqd - ptRadSqd) < 72)
    {
        color = leColorLerp(gradient->c0,
                            gradient->c1,
                            80,
                            LE_GLOBAL_COLOR_MODE);
    }
    //Do not test inner edge if full circle
    else if (thicknessSqd == oRadSqd)
    {
        color = gradient->c1;
    }
    else if ((ptRadSqd - iRadSqd) < 8)
    {
        color = leColorLerp(gradient->c0,
                            gradient->c1,
                            20,
                            LE_GLOBAL_COLOR_MODE);
    }
    else if ((ptRadSqd - iRadSqd) < 32)
    {
        color = leColorLerp(gradient->c0,
                            gradient->c1,
                            40,
                            LE_GLOBAL_COLOR_MODE);
    }
    else if ((ptRadSqd - iRadSqd) < 50)
    {
        color = leColorLerp(gradient->c0,
                            gradient->c1,
                            60,
                            LE_GLOBAL_COLOR_MODE);
    }    
    else if ((ptRadSqd - iRadSqd) < 72)
    {
        color = leColorLerp(gradient->c0,
                            gradient->c1,
                            80,
                            LE_GLOBAL_COLOR_MODE);
    }
    else
    {
        color = gradient->c1;
    }

    return color;
}
*/

static void drawQ1(const ArcDrawState* state)
{
    lePoint drawPt;
    lePoint scanPt;
    uint32_t ptRadiusSqrd;
    
    for (scanPt.y = state->topRectPt.y; scanPt.y >= 0; scanPt.y--)
    {
        for (scanPt.x = 0; scanPt.x < state->botRectPt.x; scanPt.x++)
        {
            ptRadiusSqrd = (scanPt.x * scanPt.x) + (scanPt.y * scanPt.y);

            //If point is outside outer circle, skip
            if (ptRadiusSqrd < state->iCirRadSqrd)
                continue;

            //If point is outside outer circle, done scanning x
            if (ptRadiusSqrd > state->oCirRadSqrd)
                break;

            if (state->absStartAngle == state->absEndAngle)
            {
                //Circle, do not filter, draw all points
            }
            else if ((state->absStartAngle <= 90 && state->absStartAngle >= 0) &&
                     (state->absEndAngle <= 90 && state->absEndAngle >= 0))
            {
                //If both start and end angles are in Q1
                //If CCW, exclude points RIGHT of start line and LEFT of end line
                if (state->dir == LE_CCW)
                {
                    if (state->absStartAngle < state->absEndAngle)
                    {
                        //Include points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != RIGHT_OF_LINE) &&
                            (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) != LEFT_OF_LINE))
                        {
                            //Do nothing
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (state->absStartAngle > state->absEndAngle)
                    {
                        //exclude points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == RIGHT_OF_LINE) &&
                            (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == LEFT_OF_LINE))
                            continue;
                    }
                    else
                    {
                        if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != ON_LINE)
                            continue;
                    }
                }
                else
                {
                    if (state->absStartAngle > state->absEndAngle)
                    {
                        //Include points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != LEFT_OF_LINE) &&
                            (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) != RIGHT_OF_LINE))
                        {
                            //Do nothing
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (state->absStartAngle < state->absEndAngle)
                    {
                        //exclude points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == LEFT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == RIGHT_OF_LINE))
                            continue;
                    }
                    else
                    {
                        if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != ON_LINE)
                            continue;
                    }
                }
            }
            else if (state->absStartAngle <= 90 && state->absStartAngle >= 0)
            {
                //If CCW, exclude points RIGHT of start line
                if (state->dir == LE_CCW)
                {
                    if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == RIGHT_OF_LINE)
                        continue;
                }
                //If CW, exclude points LEFT of start line
                else
                {
                    if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == LEFT_OF_LINE)
                        continue;
                }
            }
                //If end angle is in Q1, test agains Q1
            else if (state->absEndAngle <= 90 && state->absEndAngle >= 0)
            {
                //If CCW, exclude points LEFT of start line
                if (state->dir == LE_CCW)
                {
                    if (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == LEFT_OF_LINE)
                        continue;
                }
                //If CW, exclude points RIGHT of start line
                else
                {
                    if (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == RIGHT_OF_LINE)
                        continue;
                }
            }

            drawPt.x = state->arcArea.x + state->x + scanPt.x;
            drawPt.y = state->arcArea.y + state->y - scanPt.y;

            if(leRenderer_CullDrawPoint(&drawPt) == LE_FALSE)
            {
#if LE_ALPHA_BLENDING_ENABLED == 1
                leRenderer_BlendPixel(drawPt.x, drawPt.y, state->clr, state->alpha);
#else
                leRenderer_PutPixel(drawPt.x, drawPt.y, state->clr);
#endif
            }
        }
    }
}

static void drawQ2(const ArcDrawState* state)
{
    lePoint drawPt;
    lePoint scanPt;
    uint32_t ptRadiusSqrd;

    for (scanPt.y = state->topRectPt.y; scanPt.y >= 0; scanPt.y--)
    {
        for (scanPt.x = state->topRectPt.x; scanPt.x < 0; scanPt.x++)
        {
            ptRadiusSqrd = (scanPt.x * scanPt.x) + (scanPt.y * scanPt.y);

            //If point is outside outer circle, skip
            if (ptRadiusSqrd > state->oCirRadSqrd)
                continue;

            //If point is outside inner circle, done scanning x
            if (ptRadiusSqrd < state->iCirRadSqrd)
                break;

            if (state->absStartAngle == state->absEndAngle)
            {
                //Circle, do not filter, draw all points
            }
            else if ((state->absStartAngle <= 180 && state->absStartAngle > 90) &&
                     (state->absEndAngle <= 180 && state->absEndAngle > 90))
            {
                //If both start and end angles are in Q2
                //If CCW, exclude points RIGHT of start line and LEFT of end line
                if(state->dir == LE_CCW)
                {
                    if (state->absStartAngle < state->absEndAngle)
                    {
                        //Include points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != RIGHT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) != LEFT_OF_LINE))
                        {
                            //Do nothing
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (state->absStartAngle > state->absEndAngle)
                    {
                        //exclude points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == RIGHT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == LEFT_OF_LINE))
                            continue;
                    }
                    else
                    {
                        if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != ON_LINE)
                            continue;
                    }
                }
                    //If CW, exclude points LEFT of start line and RIGHT of end line
                else
                {
                    if (state->absStartAngle > state->absEndAngle)
                    {
                        //Include points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != LEFT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) != RIGHT_OF_LINE))
                        {
                            //Do nothing
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (state->absStartAngle < state->absEndAngle)
                    {
                        //exclude points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == LEFT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == RIGHT_OF_LINE))
                            continue;
                    }
                    else
                    {
                        if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != ON_LINE)
                            continue;
                    }
                }
            }
            else if (state->absStartAngle <= 180 && state->absStartAngle > 90)
            {
                //If CCW, exclude points RIGHT of start line
                if (state->dir == LE_CCW)
                {
                    if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == RIGHT_OF_LINE)
                        continue;
                }
                    //If CW, exclude points LEFT of start line
                else
                {
                    if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == LEFT_OF_LINE)
                        continue;
                }
            }
                //If end angle is in Q2, test agains Q2
            else if (state->absEndAngle <= 180 && state->absEndAngle > 90)
            {
                //If CCW, exclude points LEFT of start line
                if (state->dir == LE_CCW)
                {
                    if (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == LEFT_OF_LINE)
                        continue;
                }
                    //If CCW, exclude points RIGHT of start line
                else
                {
                    if (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == RIGHT_OF_LINE)
                        continue;
                }
            }

            //Soften edge colors if anti-aliased


            drawPt.x = state->arcArea.x + state->x + scanPt.x;
            drawPt.y = state->arcArea.y + state->y - scanPt.y;

            if(leRenderer_CullDrawPoint(&drawPt) == LE_FALSE)
            {
#if LE_ALPHA_BLENDING_ENABLED == 1
                leRenderer_BlendPixel(drawPt.x, drawPt.y, state->clr, state->alpha);
#else
                leRenderer_PutPixel(drawPt.x, drawPt.y, state->clr);
#endif
            }
        }
    }
}

static void drawQ3(const ArcDrawState* state)
{
    lePoint drawPt;
    lePoint scanPt;
    uint32_t ptRadiusSqrd;
    
    for (scanPt.y = -1; scanPt.y > state->botRectPt.y; scanPt.y--)
    {
        for (scanPt.x = state->topRectPt.x; scanPt.x < 0; scanPt.x++)
        {
            ptRadiusSqrd = (scanPt.x * scanPt.x) + (scanPt.y * scanPt.y);

            //If point is outside outer circle, skip
            if (ptRadiusSqrd > state->oCirRadSqrd)
                continue;

            //If point is outside inner circle, done scanning x
            if (ptRadiusSqrd < state->iCirRadSqrd)
                break;

            if (state->absStartAngle == state->absEndAngle)
            {
                //Circle, do not filter, draw all points
            }
            else if ((state->absStartAngle <= 270 && state->absStartAngle > 180) &&
                (state->absEndAngle <= 270 && state->absEndAngle > 180))
            {
                //If CCW, exclude points RIGHT of start line and LEFT of end line
                if (state->dir == LE_CCW)
                {
                    if (state->absStartAngle < state->absEndAngle)
                    {
                        //Include points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != LEFT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) != RIGHT_OF_LINE))
                        {
                            //Do nothing
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (state->absStartAngle > state->absEndAngle)
                    {
                        //exclude points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == LEFT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == RIGHT_OF_LINE))
                            continue;
                    }
                    else
                    {
                        if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != ON_LINE)
                            continue;
                    }
                }
                else
                {
                    if (state->absStartAngle > state->absEndAngle)
                    {
                        //Include points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != RIGHT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) != LEFT_OF_LINE))
                        {
                            //Do nothing
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (state->absStartAngle < state->absEndAngle)
                    {
                        //exclude points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == RIGHT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == LEFT_OF_LINE))
                            continue;
                    }
                    else
                    {
                        if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != ON_LINE)
                            continue;
                    }
                }
            }
            else if (state->absStartAngle <= 270 && state->absStartAngle > 180)
            {
                //If CCW, exclude points LEFT of start line
                if (state->dir == LE_CCW)
                {
                    if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == LEFT_OF_LINE)
                        continue;
                }
                    //If CW, exclude points RIGHT of start line
                else
                {
                    if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == RIGHT_OF_LINE)
                        continue;
                }
            }
            else if (state->absEndAngle <= 270 && state->absEndAngle > 180)
            {
                //If CCW, exclude points RIGHT of end line
                if (state->dir == LE_CCW)
                {
                    if (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == RIGHT_OF_LINE)
                        continue;
                }
                    //If CW, exclude points LEFT of end line
                else
                {
                    //corner case: if radius is too small, and end angle is close to 180, 
                    //the end line approximates to a horizontal line. in this case, 
                    //just draw all points in the quadrant
                    if (state->arcEndPt0.y == 0 && state->arcEndPt1.y == 0)
                    {
                        //draw all points
                    }
                    else if (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == LEFT_OF_LINE)
                        continue;
                }
            }

            drawPt.x = state->arcArea.x + state->x + scanPt.x;
            drawPt.y = state->arcArea.y + state->y - scanPt.y;

            if(leRenderer_CullDrawPoint(&drawPt) == LE_FALSE)
            {
#if LE_ALPHA_BLENDING_ENABLED == 1
                leRenderer_BlendPixel(drawPt.x, drawPt.y, state->clr, state->alpha);
#else
                leRenderer_PutPixel(drawPt.x, drawPt.y, state->clr);
#endif
            }
        }
    }
}

static void drawQ4(const ArcDrawState* state)
{
    lePoint drawPt;
    lePoint scanPt;
    uint32_t ptRadiusSqrd;
    
    for (scanPt.y = -1; scanPt.y > state->botRectPt.y; scanPt.y--)
    {
        for (scanPt.x = 0; scanPt.x < state->botRectPt.x; scanPt.x++)
        {
            ptRadiusSqrd = (scanPt.x * scanPt.x) + (scanPt.y * scanPt.y);

            //If point is outside outer circle, skip
            if (ptRadiusSqrd < state->iCirRadSqrd)
                continue;

            //If point is outside outer circle, done scanning x
            if (ptRadiusSqrd > state->oCirRadSqrd)
                break;

            if (state->absStartAngle == state->absEndAngle)
            {
                //Circle or almost a circle, do not filter, draw all points
            }
            else if ((state->absStartAngle < 360 && state->absStartAngle > 270) &&
                (state->absEndAngle < 360 && state->absEndAngle > 270))
            {
                //If CCW, exclude points RIGHT of start line and LEFT of end line
                if (state->dir == LE_CCW)
                {
                    if (state->absStartAngle < state->absEndAngle)
                    {
                        //Include points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != LEFT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) != RIGHT_OF_LINE))
                        {
                            //Do nothing
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (state->absStartAngle > state->absEndAngle)
                    {
                        //exclude points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == LEFT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == RIGHT_OF_LINE))
                            continue;
                    }
                    else
                    {
                        if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != ON_LINE)
                            continue;
                    }
                }
                else
                {
                    if (state->absStartAngle > state->absEndAngle)
                    {
                        //Include points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != RIGHT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) != LEFT_OF_LINE))
                        {
                            //Do nothing
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (state->absStartAngle < state->absEndAngle)
                    {
                        //exclude points between the two angles
                        if ((pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == RIGHT_OF_LINE)
                            && (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == LEFT_OF_LINE))
                            continue;
                    }
                    else
                    {
                        if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) != ON_LINE)
                            continue;
                    }
                }
            }
            else if (state->absStartAngle < 360 && state->absStartAngle > 270)
            {
                //If CCW, exclude points LEFT of start line
                if (state->dir == LE_CCW)
                {
                    if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == LEFT_OF_LINE)
                        continue;
                }
                    //If CW, exclude points RIGHT of start line
                else
                {
                    if (pointRelPositionFromLine(&state->arcStartPt0, &state->arcStartPt1, &scanPt) == RIGHT_OF_LINE)
                        continue;
                }
            }
            else if (state->absEndAngle < 360 && state->absEndAngle > 270)
            {
                //If CCW, exclude points RIGHT of end line
                if (state->dir == LE_CCW)
                {
                    //corner case: if radius is too small, and end angle is close to 360, 
                    //the end line approximates to a horizontal line. in this case, 
                    //just draw all points in the quadrant
                    if (state->arcEndPt0.y == 0 && state->arcEndPt1.y == 0)
                    {
                        //draw all points
                    }
                    else if (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == RIGHT_OF_LINE)
                        continue;
                }
                //If CW, exclude points LEFT of end line
                else
                {
                    if (pointRelPositionFromLine(&state->arcEndPt0, &state->arcEndPt1, &scanPt) == LEFT_OF_LINE)
                        continue;
                }
            }

            drawPt.x = state->arcArea.x + state->x + scanPt.x;
            drawPt.y = state->arcArea.y + state->y - scanPt.y;

            if(leRenderer_CullDrawPoint(&drawPt) == LE_FALSE)
            {
#if LE_ALPHA_BLENDING_ENABLED == 1
                leRenderer_BlendPixel(drawPt.x, drawPt.y, state->clr, state->alpha);
#else
                leRenderer_PutPixel(drawPt.x, drawPt.y, state->clr);
#endif
            }
        }
    }
}

//This function does a scan fill from +y -> -y, -x -> +x or
// Q2, Q1, Q3, Q4 in terms of quadrants. Points outside the 
// arc fill area are discarded.
leResult leRenderer_ArcFill(const leRect* drawRect,
                            int32_t x,
                            int32_t y,
                            int32_t r,
                            int32_t startAngle,
                            int32_t centerAngle,
                            uint32_t thickness,
                            leColor clr,
                            leBool antialias,
                            uint32_t a)
{
    ArcDrawState state;

    /*lePoint topRectPt, botRectPt; //Top and bottom points of arc rectangle
    lePoint arcStartPt0, arcStartPt1, arcEndPt0, arcEndPt1; //Points of arc edges
    lePoint drawPt; //Point to draw
    lePoint scanPt;
    int32_t endAngle;
    uint32_t ptRadiusSqrd, oCirRadSqrd, iCirRadSqrd; //Square of radii
    uint32_t absStartAngle, absEndAngle; //start and end angles in positive angles
    leArcDir dir;*/
    //uint32_t thicknessSqd;
    
    if(leRenderer_CullDrawRect(drawRect) == LE_TRUE)
        return LE_FAILURE;

    state.arcArea = *drawRect;

    thickness = (thickness < (uint32_t)r) ? thickness : (uint32_t)r;
    
    //thicknessSqd = thickness * thickness;

    //Don't care about wrapping 
    if (centerAngle == 0)
        return LE_SUCCESS;

    startAngle %= 360;
    centerAngle %= 360;

    //Determine points of arc edges
    if (startAngle >= 0)
    {
        state.absStartAngle = startAngle;
    }
    else
    {
        state.absStartAngle = 360 + startAngle;
    }
    
    lePolarToXY(r, state.absStartAngle, &state.arcStartPt1);
    lePolarToXY(r - thickness, state.absStartAngle, &state.arcStartPt0);

    state.endAngle = startAngle + centerAngle;
    
    if (state.endAngle >= 0)
    {
        state.absEndAngle = state.endAngle;
    }
    else
    {
        state.absEndAngle = 360 + state.endAngle;
    }

    state.absEndAngle %= 360;

    lePolarToXY(r, state.absEndAngle, &state.arcEndPt1);
    lePolarToXY(r - thickness, state.absEndAngle, &state.arcEndPt0);

    if (centerAngle > 0)
    {
        state.dir  = LE_CCW;
    }
    else
    {
        state.dir = LE_CW;
    }

    //Determine bounding rectangle points
    state.topRectPt.x = - ((int32_t)r);
    state.topRectPt.y = r;
    state.botRectPt.x = r;
    state.botRectPt.y = -((int32_t)r);

    state.oCirRadSqrd = r * r;
    state.iCirRadSqrd = (r - thickness) * (r - thickness);

    state.x = x;
    state.y = y;
    state.clr = clr;
    state.alpha = a;

    //Scan thru the points in arc rectangle, per quadrant and filter points that are outside
    //Only scan if the arc overlaps with the quadrant
    if (leArcsOverlapQuadrant(state.absStartAngle,
                              state.absEndAngle,
                              state.dir,
                              LE_Q2) == LE_TRUE)
    {
        drawQ2(&state);
    }

    //Don't scan thru Q1 points if not needed. Optimize?
    if(leArcsOverlapQuadrant(state.absStartAngle,
                             state.absEndAngle,
                             state.dir, 
                             LE_Q1) == LE_TRUE)
    {
        drawQ1(&state);
    }

    //Don't scan thru Q3 points if not needed. Optimize?
    if(leArcsOverlapQuadrant(state.absStartAngle,
                             state.absEndAngle,
                             state.dir,
                             LE_Q3) == LE_TRUE)
    {
        drawQ3(&state);
    }

    //Don't scan thru Q4 points if not needed. Optimize?
    if(leArcsOverlapQuadrant(state.absStartAngle,
                             state.absEndAngle,
                             state.dir,
                             LE_Q4) == LE_TRUE)
    {
        drawQ4(&state);
    }

    return LE_SUCCESS;
}

