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
#include "gfx/legato/common/legato_math.h"

static uint8_t calculateBlendColor(uint8_t ca,
                                   uint32_t aa,
                                   uint8_t cb,
                                   uint32_t ab,
                                   uint32_t ar)
{
    uint32_t caaa = lePercentOf(ca, aa);
    uint32_t cbab = lePercentOf(cb, ab);
    uint32_t cr;

    cbab = lePercentOf(cbab, 100 - aa);
    caaa += cbab;

    cr = caaa * 100;
    cr /= ar;

    if(cr > 255)
        return 255;

    return (uint8_t)cr;

    /*float lclr, rclr, lalpha, ralpha, calpha;

    lclr = (float)ca / 255.0f;
    lalpha = (float)aa / 100.0f;
    rclr = (float)cb / 255.0f;
    ralpha = (float)ab / 100.0f;
    calpha = ar / 100.0f;

    float caaa = lclr * lalpha; // Ca αa
    float cbab = (rclr * ralpha) * (1.0f - lalpha); // Cb αb
    float cr;

    caaa += cbab; // Ca αa + (Cb αb (1 - αa))
    cr = caaa / calpha;

    cr *= 255.0f;

    return (uint8_t)cr;*/
}

// αr = αa + αb (1 - αa)
// Cr = (Ca αa + Cb αb (1 - αa)) / αr
leColor leColorBlend_RGBA_8888(leColor c1, leColor c2)
{
    uint32_t result = 0;
    uint32_t c1a, c2a;
    uint32_t calpha;

    c1a = c1 & RGBA_8888_ALPHA_MASK;
    c2a = c2 & RGBA_8888_ALPHA_MASK;

    c1a = lePercentWholeRounded(c1a, 255);
    c2a = lePercentWholeRounded(c2a, 255);

    calpha = c1a + (lePercentOf(c2a, (100 - c1a)));

    if(calpha == 0)
        return c2;

    // red channel
    result |= calculateBlendColor((c1 & RGBA_8888_RED_MASK) >> 24,
                                  c1a,
                                  (c2 & RGBA_8888_RED_MASK) >> 24,
                                  c2a,
                                  calpha) << 24;

    // green channel
    result |= calculateBlendColor((c1 & RGBA_8888_GREEN_MASK) >> 16,
                                  c1a,
                                  (c2 & RGBA_8888_GREEN_MASK) >> 16,
                                  c2a,
                                  calpha) << 16;

    // blue channel
    result |= calculateBlendColor((c1 & RGBA_8888_BLUE_MASK) >> 8,
                                  c1a,
                                  (c2 & RGBA_8888_BLUE_MASK) >> 8,
                                  c2a,
                                  calpha) << 8;

    result &= ~(RGBA_8888_ALPHA_MASK);
    result |= (uint8_t)(lePercentOf(255, calpha));

    return result;
}

leColor leColorBlend_ARGB_8888(leColor c1, leColor c2)
{
    leColor result;
    uint32_t c1a, c2a;
    uint32_t calpha;

    result = 0;

    c1a = (c1 & ARGB_8888_ALPHA_MASK);
    c1a >>= 24;

    c2a = (c2 & ARGB_8888_ALPHA_MASK);
    c2a >>= 24;

    c1a = lePercentWholeRounded(c1a, 255);
    c2a = lePercentWholeRounded(c2a, 255);

    calpha = c1a + (lePercentOf(c2a, (100 - c1a)));

    // alpha channel
    result |= calculateBlendColor((c1 & ARGB_8888_ALPHA_MASK) >> 24,
                                  c1a,
                                  (c2 & ARGB_8888_ALPHA_MASK) >> 24,
                                  c2a,
                                  calpha) << 24;

    // red channel
    result |= calculateBlendColor((c1 & ARGB_8888_RED_MASK) >> 16,
                                  c1a,
                                  (c2 & ARGB_8888_RED_MASK) >> 16,
                                  c2a,
                                  calpha) << 16;

    // green channel
    result |= calculateBlendColor((c1 & ARGB_8888_GREEN_MASK) >> 8,
                                  c1a,
                                  (c2 & ARGB_8888_GREEN_MASK) >> 8,
                                  c2a,
                                  calpha) << 8;

    // blue channel
    result |= calculateBlendColor((c1 & ARGB_8888_BLUE_MASK),
                                  c1a,
                                  (c2 & ARGB_8888_BLUE_MASK),
                                  c2a,
                                  calpha);

    return result;
}