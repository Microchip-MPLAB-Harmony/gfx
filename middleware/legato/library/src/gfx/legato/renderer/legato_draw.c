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

#include "gfx/legato/legato_config.h"

#if LE_DEBUG_RENDERER == 0

#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/renderer/legato_gpu.h"


static struct DrawFunctionState
{
    leResult (*putPixel)(int32_t x, int32_t y, leColor clr, uint32_t a);
    leResult (*blendPixel)(int32_t x, int32_t y, leColor clr, uint32_t a);
    leResult (*fill)(int32_t x, int32_t y, uint32_t width, uint32_t height, leColor clr, uint32_t a);
    leResult (*blendFill)(int32_t x, int32_t y, uint32_t width, uint32_t height, leColor clr, uint32_t a);
} _drawFunction;

static leResult _RGBPutPixel(int32_t x,
                             int32_t y,
                             leColor clr,
                             uint32_t a)
{
    (void)a; // unused

#if LE_RENDER_ORIENTATION != 0
    leUtils_PointLogicalToScratch((int16_t*)&x, (int16_t*)&y);
#endif

    lePixelBufferSet_Unsafe(leGetRenderBuffer(),
                            x,
                            y,
                            clr);

    return LE_SUCCESS;
}

#if LE_ALPHA_BLENDING_ENABLED == 1
static leResult _RGBBlendPixel(int32_t x,
                               int32_t y,
                               leColor clr,
                               uint32_t a)
{
    leColor nativeSource;
    leColor rgbaSource;
    leColor rgbaDest;
    leColor resultClr;
    uint32_t currentAlpha;
    uint32_t alphaPercent;

#if LE_RENDER_ORIENTATION != 0
    leUtils_PointLogicalToScratch((int16_t*)&x, (int16_t*)&y);
#endif

    // upscale to alpha channel type
    nativeSource = lePixelBufferGet_Unsafe(leGetRenderBuffer(), x, y);

    rgbaDest = leColorConvert(leRenderer_CurrentColorMode(),
                              LE_COLOR_MODE_RGBA_8888,
                              nativeSource);

    rgbaSource = leColorConvert(leRenderer_CurrentColorMode(),
                                LE_COLOR_MODE_RGBA_8888,
                                clr);

    // blend existing alpha channel value with alpha argument value
    switch(leRenderer_CurrentColorMode())
    {
        case LE_COLOR_MODE_ARGB_8888:
        case LE_COLOR_MODE_RGBA_8888:
        case LE_COLOR_MODE_RGBA_5551:
        {

            currentAlpha = rgbaSource & 0xFF;
            alphaPercent = lePercentWholeRounded(a, 255);
            currentAlpha = lePercentOf(currentAlpha, alphaPercent);

            rgbaSource &= ~(RGBA_8888_ALPHA_MASK);
            rgbaSource |= currentAlpha;

            break;
        }
        default:
        {
            rgbaSource &= ~(RGBA_8888_ALPHA_MASK);
            rgbaSource |= a;
        }
    }

    if((rgbaSource & RGBA_8888_ALPHA_MASK) == 0)
        return LE_SUCCESS;

    resultClr = leColorBlend_RGBA_8888(rgbaSource, rgbaDest);

    // convert to destination format
    clr = leColorConvert(LE_COLOR_MODE_RGBA_8888,
                         leRenderer_CurrentColorMode(),
                         resultClr);

    lePixelBufferSet_Unsafe(leGetRenderBuffer(),
                            x,
                            y,
                            clr);

    return LE_SUCCESS;
}
#endif

static leResult _MonoPutPixel(int32_t x,
                              int32_t y,
                              leColor clr,
                              uint32_t a)
{
    (void)a; // unused

#if LE_RENDER_ORIENTATION != 0
    leUtils_PointLogicalToScratch((int16_t*)&x, (int16_t*)&y);
#endif

    lePixelBufferSet_Unsafe(leGetRenderBuffer(),
                            x,
                            y,
                            clr > 0 ? 1 : 0);

    return LE_SUCCESS;
}

static leResult _RGBFill(int32_t x,
                         int32_t y,
                         uint32_t width,
                         uint32_t height,
                         leColor clr,
                         uint32_t a)
{
    lePoint pnt;
    leRect fillRect, frameRect;
    (void)a; // unused

    // adjust for rectangle position
    leRenderer_GetFrameRect(&frameRect);

    pnt.x = x - frameRect.x;
    pnt.y = y - frameRect.y;

    fillRect.x = pnt.x;
    fillRect.y = pnt.y;
    fillRect.width = width;
    fillRect.height = height;

    if(leGPU_FillRect(&fillRect, clr, 255) == LE_SUCCESS)
        return LE_SUCCESS;

#if LE_RENDER_ORIENTATION != 0
        leRect rotRect;
        rotRect.x = pnt.x;
        rotRect.y = pnt.y;
        rotRect.width = width;
        rotRect.height = height;

        leUtils_RectLogicalToScratch(&rotRect);

        pnt.x = rotRect.x;
        pnt.y = rotRect.y;
        width = rotRect.width;
        height = rotRect.height;
#endif

    lePixelBufferAreaFill(leGetRenderBuffer(),
                          pnt.x,
                          pnt.y,
                          width,
                          height,
                          clr);

    return LE_SUCCESS;
}

#if LE_ALPHA_BLENDING_ENABLED == 1
static leResult _RGBBlendFill(int32_t x,
                              int32_t y,
                              uint32_t width,
                              uint32_t height,
                              leColor clr,
                              uint32_t a)
{
    uint32_t w, h;

    lePoint pnt;
    leRect fillRect, frameRect;

    //if(a == 0)
    //    return LE_SUCCESS;

    //if(a == 255)
    //    return _RGBFill(x, y, width, height, clr, 0);

    // adjust for rectangle position
    leRenderer_GetFrameRect(&frameRect);

    pnt.x = x - frameRect.x;
    pnt.y = y - frameRect.y;

    fillRect.x = pnt.x;
    fillRect.y = pnt.y;
    fillRect.width = width;
    fillRect.height = height;

    if(leGPU_FillRect(&fillRect, clr, a) == LE_SUCCESS)
        return LE_SUCCESS;

    for(h = 0; h < height; h++)
    {
        for(w = 0; w < width; w++)
        {
            // blend pixel will adjust for the rectangle
            leRenderer_BlendPixel_Safe(x + w,
                                       y + h,
                                       clr,
                                       a);
        }
    }

    return LE_SUCCESS;
}
#endif

static leResult _MonoFill(int32_t x,
                          int32_t y,
                          uint32_t width,
                          uint32_t height,
                          leColor clr,
                          uint32_t a)
{
    lePoint pnt;
    leRect frameRect;
    (void)a; // unused

    // adjust for rectangle position
    leRenderer_GetFrameRect(&frameRect);

    pnt.x = x - frameRect.x;
    pnt.y = y - frameRect.y;

#if LE_RENDER_ORIENTATION != 0
    leRect rotRect;
    rotRect.x = pnt.x;
    rotRect.y = pnt.y;
    rotRect.width = width;
    rotRect.height = height;

    leUtils_RectLogicalToScratch(&rotRect);

    pnt.x = rotRect.x;
    pnt.y = rotRect.y;
    width = rotRect.width;
    height = rotRect.height;
#endif

    lePixelBufferAreaFill_Unsafe(leGetRenderBuffer(),
                                 pnt.x,
                                 pnt.y,
                                 width,
                                 height,
                                 clr > 0 ? 1 : 0);

    return LE_SUCCESS;
}

leColor leRenderer_GetPixel(int32_t x,
                            int32_t y)
{
    leRect frameRect;

    // adjust for rectangle position
    leRenderer_GetFrameRect(&frameRect);

    x -= frameRect.x;
    y -= frameRect.y;

#if LE_RENDER_ORIENTATION != 0
    leUtils_PointLogicalToScratch((int16_t*)&x, (int16_t*)&y);
#endif

    return lePixelBufferGet_Unsafe(leGetRenderBuffer(),
                                   x,
                                   y);
}

leResult leRenderer_GetPixel_Safe(int32_t x,
                                  int32_t y,
                                  leColor* clr)
{
    leRect frameRect;

    if(leRenderer_CullDrawXY(x, y) == LE_TRUE)
        return LE_FAILURE;

    // adjust for rectangle position
    leRenderer_GetFrameRect(&frameRect);

    x -= frameRect.x;
    y -= frameRect.y;

#if LE_RENDER_ORIENTATION != 0
    leUtils_PointLogicalToScratch((int16_t*)&x, (int16_t*)&y);
#endif

    *clr = lePixelBufferGet_Unsafe(leGetRenderBuffer(),
                                   x,
                                   y);

    return LE_SUCCESS;
}


leResult leRenderer_PutPixel(int32_t x,
                             int32_t y,
                             leColor clr)
{
    leRect frameRect;

    // adjust for rectangle position
    leRenderer_GetFrameRect(&frameRect);

    x -= frameRect.x;
    y -= frameRect.y;
    
    return _drawFunction.putPixel(x, y, clr, 0);
}

leResult leRenderer_PutPixel_Safe(int32_t x,
                                  int32_t y,
                                  leColor clr)
{
    leRect frameRect;

    if(leRenderer_CullDrawXY(x, y) == LE_TRUE)
        return LE_FAILURE;

    // adjust for rectangle position
    leRenderer_GetFrameRect(&frameRect);

    x -= frameRect.x;
    y -= frameRect.y;

    return _drawFunction.putPixel(x, y, clr, 0);
}

leResult leRenderer_BlendPixel(int32_t x,
                               int32_t y,
                               leColor clr,
                               uint32_t a)
{
    leRect frameRect;

    // adjust for rectangle position
    leRenderer_GetFrameRect(&frameRect);

    x -= frameRect.x;
    y -= frameRect.y;
    
    return _drawFunction.blendPixel(x, y, clr, a);
}

leResult leRenderer_BlendPixel_Safe(int32_t x,
                                    int32_t y,
                                    leColor clr,
                                    uint32_t a)
{
    leRect frameRect;

    if(leRenderer_CullDrawXY(x, y) == LE_TRUE)
        return LE_FAILURE;

    // adjust for rectangle position
    leRenderer_GetFrameRect(&frameRect);

    x -= frameRect.x;
    y -= frameRect.y;

    return _drawFunction.blendPixel(x, y, clr, a);
}

leResult leRenderer_FillArea(int32_t x,
                             int32_t y,
                             uint32_t width,
                             uint32_t height,
                             leColor clr,
                             uint32_t a)
{
    return _drawFunction.blendFill(x, y, width, height, clr, a);
}

void _leRenderer_InitDrawForMode(leColorMode mode)
{
    switch(mode)
    {
        case LE_COLOR_MODE_GS_8:
        case LE_COLOR_MODE_INDEX_1:
        case LE_COLOR_MODE_INDEX_4:
        case LE_COLOR_MODE_INDEX_8:
        case LE_COLOR_MODE_RGB_332:
        case LE_COLOR_MODE_RGB_565:
        case LE_COLOR_MODE_RGB_888:
        case LE_COLOR_MODE_RGBA_5551:
        case LE_COLOR_MODE_RGBA_8888:
        case LE_COLOR_MODE_ARGB_8888:
        {
#if LE_ALPHA_BLENDING_ENABLED == 1
            _drawFunction.putPixel = _RGBPutPixel;
            _drawFunction.blendPixel = _RGBBlendPixel;
            _drawFunction.fill = _RGBFill;
            _drawFunction.blendFill = _RGBBlendFill;
#else
            _drawFunction.putPixel = _RGBPutPixel;
            _drawFunction.blendPixel = _RGBPutPixel;
            _drawFunction.fill = _RGBFill;
            _drawFunction.blendFill = _RGBFill;
#endif

            break;
        }
        case LE_COLOR_MODE_MONOCHROME:
        {
            _drawFunction.putPixel = _MonoPutPixel;
            _drawFunction.blendPixel = _MonoPutPixel;
            _drawFunction.fill = _MonoFill;
            _drawFunction.blendFill = _MonoFill;

            break;
        }
        default:
        { }
    }
}

#endif // LE_DEBUG_RENDERER