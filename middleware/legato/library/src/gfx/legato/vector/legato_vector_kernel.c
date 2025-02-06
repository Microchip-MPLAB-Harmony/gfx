#include "gfx/legato/vector/legato_vector_kernel.h"
#include "gfx/legato/renderer/legato_renderer.h"

extern leReal_i16* _AA_SampleData[5];
extern leReal_i16 __AA_SampleWeights[5];
extern uint32_t _AA_SampleCount[5];

static leBool _testPixel(struct leVector_Kernel* krn)
{
    uint32_t idx;
    uint32_t offset = (1 << ((uint32_t)krn->context->aaMode)) * 2;
    int32_t aaOffs = (int32_t)krn->context->aaMode;

    for(idx = 0; idx < offset; idx += 2)
    {
        krn->testPoint.x = krn->pixelCenter.x + _AA_SampleData[aaOffs][idx + 0];
        krn->testPoint.y = krn->pixelCenter.y + _AA_SampleData[aaOffs][idx + 1];

        krn->evaluatePoint(krn);
    }

    return krn->hitCount != 0 ? LE_TRUE : LE_FALSE;
}

static uint8_t _shadeFragment(struct leVector_Kernel* krn)
{
    leReal_i16 val;

    if(krn->hitCount == _AA_SampleCount[(int32_t)krn->context->aaMode])
        return 255;

    val = LE_REAL_I16_MULTIPLY(__AA_SampleWeights[(int32_t)krn->context->aaMode],
                               LE_REAL_I16_FROM_INT(krn->hitCount));
    val = LE_REAL_I16_MULTIPLY(val, LE_REAL_I16_255);
    val = leReal_i16_Round(val);

    return (uint8_t)(leReal_i16_ToInt(val));
}

void leVector_Kernel_Exec(struct leVector_Kernel* krn)
{
    uint8_t a;
    leBool res;
    uint32_t xOffs = 0;
    leReal_i16 pixelAlpha;

    if(krn == NULL ||
       krn->context == NULL ||
       krn->evaluatePoint == NULL)
        return;

    for(krn->scanPoint.y = krn->scanArea.y;
        krn->scanPoint.y < krn->scanArea.y + krn->scanArea.height;
        ++krn->scanPoint.y)
    {
        krn->pixelCenter.y = (leReal_i16_FromInt(krn->scanPoint.y)  + LE_REAL_I16_ONE_OVER_TWO) - krn->origin.y;

        for(krn->scanPoint.x = krn->scanArea.x + (int32_t)xOffs;
            krn->scanPoint.x < krn->scanArea.x + krn->scanArea.width;
            ++krn->scanPoint.x)
        {
            krn->pixelCenter.x = (leReal_i16_FromInt(krn->scanPoint.x)  + LE_REAL_I16_ONE_OVER_TWO) - krn->origin.x;

            krn->hitCount = 0;

            res = _testPixel(krn);

            if(res == LE_FALSE)
                continue;

            if(krn->shadeFragment != NULL)
            {
                a = krn->shadeFragment(krn);
            }
            else
            {
                if(krn->context->aaMode == LE_ANTIALIASING_NONE)
                {
                    a = 255;
                }
                else
                {
                    a = _shadeFragment(krn);
                }
            }

            if(a == 0)
            {
                continue;
            }
            else if(a == 0xFF)
            {
                a = LE_REAL_I16_TO_INT(LE_REAL_I16_MULTIPLY(krn->context->alpha, LE_REAL_I16_255));
            }
            else
            {
                pixelAlpha = krn->context->alpha;
                pixelAlpha = LE_REAL_I16_MULTIPLY(pixelAlpha, LE_REAL_I16_DIVIDE(leReal_i16_FromInt(a), LE_REAL_I16_255));
                pixelAlpha = LE_REAL_I16_MULTIPLY(pixelAlpha, LE_REAL_I16_255);

                a = (uint8_t)(leReal_i16_ToInt(pixelAlpha));
            }

            if(a == 0)
                continue;

            leRenderer_BlendPixel(krn->scanPoint.x,
                                  krn->scanPoint.y,
                                  krn->context->color,
                                  a);
        }
    }
}