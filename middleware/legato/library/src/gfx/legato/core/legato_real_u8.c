#include "gfx/legato/core/legato_real_u8.h"

leReal_u8 leReal_u8_Min(leReal_u8 l, leReal_u8 r)
{
    if(l < r)
    {
        return l;
    }

    return r;
}

leReal_u8 leReal_u8_Max(leReal_u8 l, leReal_u8 r)
{
    if(l > r)
    {
        return l;
    }

    return r;
}

leReal_u8 leReal_u8_Round(leReal_u8 rl)
{
    uint32_t t;

    t = (uint32_t)rl;

    if((rl & LE_REAL_U8_ONE_OVER_TWO) > 0)
    {
        t &= 0xFFFFFF00;
        t += LE_REAL_U8_ONE;
    }
    else
    {
        t &= 0xFFFFFF00;
    }

    return (leReal_u8)(t);
}

uint32_t leReal_u8_Ceiling(leReal_u8 rl)
{
    uint32_t t;

    t = (uint32_t)rl;

    if((t & 0xFFFF) > 0)
    {
        t &= 0xFFFFFF00;
        t += LE_REAL_U8_ONE;
    }

    return (leReal_u8)(t);
}

uint32_t leReal_u8_Middle(leReal_u8 rl)
{
    uint32_t t;

    t = (uint32_t)rl;
    t &= 0xFFFFFFF00;
    t |= LE_REAL_U8_ONE_OVER_TWO;

    return (leReal_u8)(t);
}

leReal_u8 leReal_u8_Square(leReal_u8 r)
{
    return LE_REAL_U8_MULTIPLY(r, r);
}

leReal_u8 leReal_u8_SquareRoot(leReal_u8 rl)
{
    leReal_u8 root;
    leReal_u8 x;
    leReal_u8 val;

    x = rl;

    if(x == 0)
        return 0;

    while(1)
    {
        root = (leReal_u8)(((uint64_t)rl * LE_REAL_U8_FRACTION_SCL) / x);
        root += x;
        root = (leReal_u8)(((uint64_t)root * LE_REAL_U8_ONE_OVER_TWO) / LE_REAL_U8_FRACTION_SCL);

        //printf("%f\n", LE_REAL_U8_TO_FLOAT(root));

        if(root < x)
        {
            val = x - root;
        }
        else
        {
            val = root - x;
        }

        /*val = (int32_t)root - (int32_t)x;

        if(val & 0x80000000)
        {
            val = ~(val) + 1;
        }*/

        if(val < 0x4) // if value is less than 2^(-5) or 0.03125
            break;

        //printf("%f, %f, %f, %u\n", LE_REAL_U8_TO_FLOAT(rl), LE_REAL_U8_TO_FLOAT(root), LE_REAL_U8_TO_FLOAT(x), val);

        x = root;
    }

    return root;
}