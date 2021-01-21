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

/* renderer function use only */
extern leRenderState _rendererState;

typedef leResult (*PixelPutFn)(int32_t, int32_t, leColor, uint32_t);

static leResult putPixel(int32_t x,
                         int32_t y,
                         leColor clr,
                         uint32_t);

#if LE_ALPHA_BLENDING_ENABLED == 1
static leResult blendPixel(int32_t x,
                           int32_t y,
                           leColor clr,
                           uint32_t a);
#endif

leColor leRenderer_GetPixel(int32_t x,
                            int32_t y)
{
    x -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].x;
    y -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].y;

    return lePixelBufferGet_Unsafe(_rendererState.renderBuffer,
                                   x,
                                   y);
}

leResult leRenderer_GetPixel_Safe(int32_t x,
                                  int32_t y,
                                  leColor* clr)
{
    if(leRenderer_CullDrawXY(x, y) == LE_TRUE)
        return LE_FAILURE;

    x -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].x;
    y -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].y;

    return lePixelBufferGet_Unsafe(_rendererState.renderBuffer,
                                   x,
                                   y);
}

static PixelPutFn pixelPutFn = putPixel;

leResult leRenderer_PutPixel(int32_t x,
                             int32_t y,
                             leColor clr)
{
    x -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].x;
    y -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].y;
    
    return pixelPutFn(x, y, clr, 0);
}

leResult leRenderer_PutPixel_Safe(int32_t x,
                                  int32_t y,
                                  leColor clr)
{
    if(leRenderer_CullDrawXY(x, y) == LE_TRUE)
        return LE_FAILURE;

    x -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].x;
    y -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].y;

    return pixelPutFn(x, y, clr, 0);
}

leResult leRenderer_BlendPixel(int32_t x,
                               int32_t y,
                               leColor clr,
                               uint32_t a)
{
    x -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].x;
    y -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].y;
    
#if LE_ALPHA_BLENDING_ENABLED == 1
    return blendPixel(x, y, clr, a);
#else
    return putPixel(x, y, clr, a);
#endif
}

leResult leRenderer_BlendPixel_Safe(int32_t x,
                                    int32_t y,
                                    leColor clr,
                                    uint32_t a)
{
    if(leRenderer_CullDrawXY(x, y) == LE_TRUE)
        return LE_FAILURE;

    x -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].x;
    y -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].y;

#if LE_ALPHA_BLENDING_ENABLED == 1
    return blendPixel(x, y, clr, a);
#else
    return putPixel(x, y, clr, a);
#endif
}

#if LE_ALPHA_BLENDING_ENABLED == 1
void leRenderer_SetAlphaEnabled(leBool enabled, uint8_t amt)
{    
    if(_rendererState.alphaEnable == enabled)
        return;
        
    _rendererState.alphaEnable = enabled;
    _rendererState.alpha = amt;
    
    if(enabled == LE_TRUE)
    {
        pixelPutFn = blendPixel;
    }
    else
    {
        pixelPutFn = putPixel;
    }
}
#endif

leResult putPixel(int32_t x,
                  int32_t y,
                  leColor clr,
                  uint32_t a)
{
    lePixelBufferSet_Unsafe(_rendererState.renderBuffer,
                            x,
                            y,
                            clr);
                            
    return LE_SUCCESS;
}

#if LE_ALPHA_BLENDING_ENABLED == 1
leResult blendPixel(int32_t x, int32_t y, leColor clr, uint32_t a)
{
    leColor nativeSource;
    leColor rgbaSource;
    leColor rgbaDest;
    leColor resultClr;
    
    nativeSource = lePixelBufferGet_Unsafe(_rendererState.renderBuffer, x, y);
    
    rgbaSource = leColorConvert(LE_GLOBAL_COLOR_MODE, LE_COLOR_MODE_RGBA_8888, clr);
    
    // blend with alpha channel
    
    
    // blend with alpha argument value
    rgbaSource &= ~(RGBA_8888_ALPHA_MASK);
    rgbaSource |= a;
    
    rgbaDest = leColorConvert(LE_GLOBAL_COLOR_MODE, LE_COLOR_MODE_RGBA_8888, nativeSource);
    rgbaDest |= RGBA_8888_ALPHA_MASK;
    
    resultClr = leColorBlend_RGBA_8888(rgbaSource, rgbaDest);
    
    // convert to destination format
    clr = leColorConvert(LE_COLOR_MODE_RGBA_8888, LE_GLOBAL_COLOR_MODE, resultClr);
    
    lePixelBufferSet_Unsafe(_rendererState.renderBuffer,
                            x,
                            y,
                            clr);
                            
    return LE_SUCCESS;
}
#endif

leResult leRenderer_FillArea(int32_t x,
                             int32_t y,
                             uint32_t width,
                             uint32_t height,
                             leColor clr,
                             uint32_t a)
{
    uint32_t w, h;
    lePoint pnt;

    x -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].x;
    y -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].y;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255) // also test global alpha value
    {
        for(h = 0; h < height; h++)
        {
            for(w = 0; w < width; w++)
            {
                pnt.x = x + w;
                pnt.y = y + h;

                leRenderer_BlendPixel_Safe(pnt.x, pnt.y, clr, a);
            }
        }
    }
    else
    {
#endif
        lePixelBufferAreaFill_Unsafe(_rendererState.renderBuffer,
                                     x,
                                     y,
                                     width,
                                     height,
                                     clr);
#if LE_ALPHA_BLENDING_ENABLED == 1
    }
#endif

    return LE_SUCCESS;
}

leResult leRenderer_FillArea_Safe(int32_t x,
                                  int32_t y,
                                  uint32_t width,
                                  uint32_t height,
                                  leColor clr,
                                  uint32_t a)
{
    uint32_t w, h;
    lePoint pnt;

    x -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].x;
    y -= _rendererState.frameRectList.rects[_rendererState.frameRectIdx].y;

#if LE_ALPHA_BLENDING_ENABLED == 1
    if(a < 255) // also test global alpha value
    {
        for(h = 0; h < height; h++)
        {
            for(w = 0; w < width; w++)
            {
                pnt.x = x + w;
                pnt.y = y + h;

                leRenderer_BlendPixel(pnt.x, pnt.y, clr, a);
            }
        }
    }
    else
    {
#endif
        lePixelBufferAreaFill(_rendererState.renderBuffer,
                              x,
                              y,
                              width,
                              height,
                              clr);
#if LE_ALPHA_BLENDING_ENABLED == 1
    }
#endif

    return LE_SUCCESS;
}