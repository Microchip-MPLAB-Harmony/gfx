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

#include "gfx/legato/renderer/legato_gpu.h"
#include "gfx/legato/renderer/legato_renderer.h"

#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/datastructure/legato_rectarray.h"

extern leRenderState _rendererState;

leResult leRenderer_HorzLine(int32_t x,
                             int32_t y,
                             int32_t w,
                             leColor clr,
                             uint32_t a)
{
    leRect clipRect;
    leRect drawRect;
    lePoint pnt;
    int32_t i;

    drawRect.x = x;
    drawRect.y = y;
    drawRect.width = w;
    drawRect.height = 1;

    if(leRenderer_CullDrawRect(&drawRect) == LE_TRUE)
        return LE_FAILURE;

    leRenderer_ClipDrawRect(&drawRect, &clipRect);

    pnt.y = clipRect.y;

    a = leClampi(0, 255, a);

    if(a == 0)
    {
        return LE_SUCCESS;
    }
    else if(leGPU_DrawLine(drawRect.x,
                           drawRect.x,
                           w,
                           1,
                           &clipRect,
                           clr,
                           a) == LE_SUCCESS)
    {
        return LE_SUCCESS;
    }
    else if(a < 255)
    {
        for(i = 0; i < clipRect.width; i++)
        {
            pnt.x = clipRect.x + i;

            leRenderer_BlendPixel(pnt.x, pnt.y, clr, a);
        }
    }
    else
    {
        for(i = 0; i < clipRect.width; i++)
        {
            pnt.x = clipRect.x + i;

            leRenderer_PutPixel(pnt.x, pnt.y, clr);
        }
    }

    return LE_SUCCESS;
}

leResult leRenderer_VertLine(int32_t x,
                             int32_t y,
                             int32_t h,
                             leColor clr,
                             uint32_t a)
{
    leRect clipRect;
    leRect drawRect;
    lePoint pnt;
    int32_t i;

    drawRect.x = x;
    drawRect.y = y;
    drawRect.width = 1;
    drawRect.height = h;

    if(leRenderer_CullDrawRect(&drawRect) == LE_TRUE)
        return LE_FAILURE;

    leRenderer_ClipDrawRect(&drawRect, &clipRect);

    pnt.x = clipRect.x;

    a = leClampi(0, 255, a);

    if(a == 0)
    {
        return LE_SUCCESS;
    }
    else if(leGPU_DrawLine(drawRect.x,
                           drawRect.y,
                           1,
                           h,
                           &clipRect,
                           clr,
                           a) == LE_SUCCESS)
    {
        return LE_SUCCESS;
    }
    else if(a < 255)
    {
        for(i = 0; i < clipRect.height; i++)
        {
            pnt.y = clipRect.y + i;

            leRenderer_BlendPixel(pnt.x, pnt.y, clr, a);
        }
    }
    else
    {
        for(i = 0; i < clipRect.height; i++)
        {
            pnt.y = clipRect.y + i;

            leRenderer_PutPixel(pnt.x, pnt.y, clr);
        }
    }

    return LE_SUCCESS;
}

typedef void (*LinePutPixelFn)(int32_t, int32_t, leColor, uint32_t);

static void linePutPixel(int32_t x,
                         int32_t y,
                         leColor clr,
                         uint32_t a)
{
    (void)a; // unused

    leRenderer_PutPixel(x, y, clr);
}

static void lineBlendPixel(int32_t x,
                           int32_t y,
                           leColor clr,
                           uint32_t a)
{
    leRenderer_BlendPixel(x, y, clr, a);
}

leResult leRenderer_DrawLine(int32_t x0,
                             int32_t y0,
                             int32_t x1,
                             int32_t y1,
                             leColor clr,
                             uint32_t a)
{
    int32_t dx, sx, dy, sy, e2, err;
    lePoint lp1, lp2, drawPoint;

    LinePutPixelFn putPixelFn;

    a = leClampi(0, 255, a);

    if(a == 0)
    {
        return LE_SUCCESS;
    }
    else if(leGPU_DrawLine(x0,
                           y0,
                           x1,
                           y1,
                           &_rendererState.drawRect,
                           clr,
                           a) == LE_SUCCESS)
    {
        return LE_SUCCESS;
    }
    else if(a < 255)
    {
        putPixelFn = lineBlendPixel;
    }
    else
    {
        putPixelFn = linePutPixel;
    }

    lp1.x = x0;
    lp1.y = y0;

    lp2.x = x1;
    lp2.y = y1;

    dx = lp2.x - lp1.x;

    if(dx < 0)
        dx *= -1;

    sx = lp1.x < lp2.x ? 1 : -1;

    dy = lp2.y - lp1.y;

    if(dy < 0)
        dy *= -1;

    dy *= -1;

    sy = lp1.y < lp2.y ? 1 : -1;
    err = dx + dy;

    for(;;)
    {
        drawPoint.x = lp1.x;
        drawPoint.y = lp1.y;

        if(leRenderer_CullDrawPoint(&drawPoint) == LE_FALSE)
        {
            putPixelFn(drawPoint.x, drawPoint.y, clr, a);
        }

        if(lp1.x == lp2.x && lp1.y == lp2.y)
            break;

        e2 = 2 * err;

        if(e2 >= dy)
        {
            err += dy;
            lp1.x += sx;
        }

        if(e2 <= dx)
        {
            err += dx;
            lp1.y += sy;
        }
    }

    return LE_SUCCESS;
}

leResult leRenderer_EllipseLine(int32_t x,
                                int32_t y,
                                int32_t a,
                                int32_t b,
                                int32_t theta,
                                int32_t startAngle,
                                int32_t centerAngle,
                                leColor clr,
                                uint32_t alpha)
{
    int32_t sa = startAngle;
    lePoint p;
    lePoint lp = lePoint_Zero;

    while (sa != (startAngle + centerAngle))
    {
        leEllipsePoint(sa, a, b, theta, &p);

        p.x += x;
        p.y += y;

#if LE_ALPHA_BLENDING_ENABLED == 1
        leRenderer_BlendPixel_Safe(p.x, p.y, clr, alpha);
#else
        leRenderer_PutPixel_Safe(p.x, p.y, clr);
#endif

        if (sa != startAngle)
        {
            leRenderer_DrawLine(lp.x,
                                lp.y,
                                p.x,
                                p.y,
                                clr,
                                alpha);
        }

        lp.x = p.x;
        lp.y = p.y;

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
