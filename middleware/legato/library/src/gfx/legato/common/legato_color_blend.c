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


#include "gfx/legato/common/legato_color.h"


#define RGBA8888_to_RxB8x8(color)   ((color & (RGBA_8888_RED_MASK | RGBA_8888_BLUE_MASK)) >> 8)
#define RGBA8888_to_xGxx8x(color)   ((color & RGBA_8888_GREEN_MASK) >> 8)

#define ARGB8888_to_RxB8x8(color)   (color & (ARGB_8888_RED_MASK | ARGB_8888_BLUE_MASK))
#define ARGB8888_to_xGxx8x(color)   (color & ARGB_8888_GREEN_MASK)


static uint32_t calculateBlendColor(uint32_t ca,
                                    uint32_t aa,
                                    uint32_t cb,
                                    uint32_t ab,
                                    uint32_t ar)
{
    uint32_t cr;

    // color_result = ((color_top*alpha_top) +
    //                 (color_bottom*alpha_bottom*(1 - alpha_top))) /
    //                alpha_result
    
    cr = ca * aa;
    cr += ((cb * ab * (255-aa)) + 127) / 255;
    cr /= ar;

    return cr;
}

leColor leColorBlend_RGBA_8888(leColor c1, leColor c2)
{
    leColor result;

    uint32_t c1a = c1 & RGBA_8888_ALPHA_MASK;
    uint32_t c2a = c2 & RGBA_8888_ALPHA_MASK;

    if(c1a == 0)
    {
        // transparent foreground
        result = c2;
    }
    else if((c1a == 255) || (c2a == 0))
    {
        // opaque foreground or transparent background
        result = c1;
    }
    else if(c2a == 255)
    {
        // opaque background - optimized blend
        result =  ((RGBA8888_to_RxB8x8(c1) * c1a) +
                   (RGBA8888_to_RxB8x8(c2) * (255-c1a))) &
                  (RGBA_8888_RED_MASK | RGBA_8888_BLUE_MASK);

        result |= ((RGBA8888_to_xGxx8x(c1) * c1a) +
                   (RGBA8888_to_xGxx8x(c2) * (255-c1a))) &
                  RGBA_8888_GREEN_MASK;

        result |= RGBA_8888_ALPHA_MASK;
    }
    else
    {
        // alpha_result = alpha_top + alpha_bottom*(1 - alpha_top)
        uint32_t calpha = ((c1a * 255) + (c2a * (255-c1a)) + 127) / 255;

        if(calpha == 0)
            return c2;

        // red channel
        result = calculateBlendColor((c1 & RGBA_8888_RED_MASK) >> 24, c1a,
                                     (c2 & RGBA_8888_RED_MASK) >> 24, c2a,
                                     calpha) << 24;

        // green channel
        result |= calculateBlendColor((c1 & RGBA_8888_GREEN_MASK) >> 16, c1a,
                                      (c2 & RGBA_8888_GREEN_MASK) >> 16, c2a,
                                      calpha) << 16;

        // blue channel
        result |= calculateBlendColor((c1 & RGBA_8888_BLUE_MASK) >> 8, c1a,
                                      (c2 & RGBA_8888_BLUE_MASK) >> 8, c2a,
                                      calpha) << 8;

        // alpha channel
        result |= calpha;
    }

    return result;
}

leColor leColorBlend_ARGB_8888(leColor c1, leColor c2)
{
    leColor result;

    uint32_t c1a = (c1 & ARGB_8888_ALPHA_MASK) >> 24;
    uint32_t c2a = (c2 & ARGB_8888_ALPHA_MASK) >> 24;

    if(c1a == 0)
    {
        // transparent foreground
        result = c2;
    }
    else if((c1a == 255) || (c2a == 0))
    {
        // opaque foreground or transparent background
        result = c1;
    }
    else if(c2a == 255)
    {
        // opaque background - optimized blend
        result =  (((ARGB8888_to_RxB8x8(c1) * c1a) +
                    (ARGB8888_to_RxB8x8(c2) * (255-c1a))) >> 8)  &
                  (ARGB_8888_RED_MASK | ARGB_8888_BLUE_MASK);

        result |= (((ARGB8888_to_xGxx8x(c1) * c1a) +
                    (ARGB8888_to_xGxx8x(c2) * (255-c1a))) >> 8) &
                  ARGB_8888_GREEN_MASK;

        result |= ARGB_8888_ALPHA_MASK;        
    }
    else
    {
        // alpha_result = alpha_top + alpha_bottom*(1 - alpha_top)
        uint32_t calpha = ((c1a * 255) + (c2a * (255-c1a)) + 127) / 255;

        if(calpha == 0)
            return c2;

        // alpha channel
        result = calpha << 24;

        // red channel
        result |= calculateBlendColor((c1 & ARGB_8888_RED_MASK) >> 16, c1a,
                                      (c2 & ARGB_8888_RED_MASK) >> 16, c2a,
                                      calpha) << 16;

        // green channel
        result |= calculateBlendColor((c1 & ARGB_8888_GREEN_MASK) >> 8, c1a,
                                      (c2 & ARGB_8888_GREEN_MASK) >> 8, c2a,
                                      calpha) << 8;

        // blue channel
        result |= calculateBlendColor((c1 & ARGB_8888_BLUE_MASK), c1a,
                                      (c2 & ARGB_8888_BLUE_MASK), c2a,
                                      calpha);
    }
    
    return result;
}
