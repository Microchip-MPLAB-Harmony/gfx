#include "gfx/legato/core/legato_real_i16.h"

#include "gfx/legato/core/legato_real_u8.h"

//#define WHOLE_BITS 15
#define WHOLE_EXP (1 << WHOLE_BITS)
#define WHOLE_MASK (0xFFFF0000)
//#define FRAC_BITS (31-WHOLE_BITS)
//#define FRAC_EXP (1 << FRAC_BITS)
#define FRAC_MASK (FRAC_EXP - 1)
#define WHOLE_WIDTH ((1 << WHOLE_BITS) - 1)
#define TWO_TO_MINUS_10 0x10
#define ACOS_SHIFT      0x400000
#define ATAN_CONST_A   ((leReal)0x3EA4) // 0.2447
#define ATAN_CONST_B   ((leReal)0x10F9) // 0.0663

static leReal_i16 _cosTable[91] =
{
    0x00010000, 0x0000FFF6, 0x0000FFD8, 0x0000FFA6, 0x0000FF60, 0x0000FF06, 0x0000FE98, 0x0000FE17,
    0x0000FD82, 0x0000FCD9, 0x0000FC1C, 0x0000FB4B, 0x0000FA67, 0x0000F970, 0x0000F865, 0x0000F746,
    0x0000F615, 0x0000F4D0, 0x0000F378, 0x0000F20D, 0x0000F08F, 0x0000EEFF, 0x0000ED5B, 0x0000EBA6,
    0x0000E9DE, 0x0000E803, 0x0000E617, 0x0000E419, 0x0000E208, 0x0000DFE7, 0x0000DDB3, 0x0000DB6F,
    0x0000D919, 0x0000D6B3, 0x0000D43B, 0x0000D1B3, 0x0000CF1B, 0x0000CC73, 0x0000C9BB, 0x0000C6F3,
    0x0000C41B, 0x0000C134, 0x0000BE3E, 0x0000BB39, 0x0000B826, 0x0000B504, 0x0000B1D5, 0x0000AE97,
    0x0000AB4C, 0x0000A7F3, 0x0000A48D, 0x0000A11B, 0x00009D9B, 0x00009A10, 0x00009679, 0x000092D5,
    0x00008F27, 0x00008B6D, 0x000087A8, 0x000083D9, 0x00008000, 0x00007C1C, 0x0000782F, 0x00007438,
    0x00007039, 0x00006C30, 0x0000681F, 0x00006406, 0x00005FE6, 0x00005BBE, 0x0000578E, 0x00005358,
    0x00004F1B, 0x00004AD8, 0x00004690, 0x00004241, 0x00003DEE, 0x00003996, 0x00003539, 0x000030D8,
    0x00002C74, 0x0000280C, 0x000023A0, 0x00001F32, 0x00001AC2, 0x0000164F, 0x000011DB, 0x00000D65,
    0x000008EF, 0x00000477, 0x00000000,
};

static leReal_i16 _acosTable[128] =
{
    0x0003243F, 0x0002F6C1, 0x0002E3D3, 0x0002D53E, 0x0002C8E6, 0x0002BDFA, 0x0002B410,
    0x0002AAE9, 0x0002A25B, 0x00029A49, 0x0002929F, 0x00028B4C, 0x00028446, 0x00027D81,
    0x000276F7, 0x000270A0, 0x00026A77, 0x00026478, 0x00025E9F, 0x000258E8, 0x00025350,
    0x00024DD6, 0x00024876, 0x0002432E, 0x00023DFD, 0x000238E1, 0x000233D9, 0x00022EE3,
    0x000229FE, 0x00022529, 0x00022063, 0x00021BAB, 0x00021700, 0x00021262, 0x00020DCF,
    0x00020947, 0x000204C9, 0x00020054, 0x0001FBE9, 0x0001F786, 0x0001F32C, 0x0001EED8,
    0x0001EA8C, 0x0001E646, 0x0001E207, 0x0001DDCD, 0x0001D999, 0x0001D56A, 0x0001D13F,
    0x0001CD19, 0x0001C8F7, 0x0001C4D9, 0x0001C0BE, 0x0001BCA6, 0x0001B891, 0x0001B47E,
    0x0001B06E, 0x0001AC5F, 0x0001A853, 0x0001A447, 0x0001A03D, 0x00019C34, 0x0001982B,
    0x00019423, 0x0001901B, 0x00018C13, 0x0001880A, 0x00018401, 0x00017FF7, 0x00017BEC,
    0x000177DF, 0x000173D1, 0x00016FC0, 0x00016BAE, 0x00016799, 0x00016381, 0x00015F66,
    0x00015B47, 0x00015725, 0x000152FF, 0x00014ED5, 0x00014AA5, 0x00014671, 0x00014237,
    0x00013DF8, 0x000139B2, 0x00013566, 0x00013113, 0x00012CB8, 0x00012855, 0x000123EA,
    0x00011F76, 0x00011AF8, 0x00011670, 0x000111DD, 0x00010D3E, 0x00010893, 0x000103DB,
    0x0000FF15, 0x0000FA40, 0x0000F55B, 0x0000F065, 0x0000EB5D, 0x0000E641, 0x0000E110,
    0x0000DBC9, 0x0000D669, 0x0000D0EE, 0x0000CB57, 0x0000C5A0, 0x0000BFC6, 0x0000B9C7,
    0x0000B39F, 0x0000AD48, 0x0000A6BD, 0x00009FF9, 0x000098F2, 0x000091A0, 0x000089F6,
    0x000081E4, 0x00007956, 0x0000702E, 0x00006645, 0x00005B59, 0x00004F01, 0x0000406C,
    0x00002D7E, 0x00000000,
};

static leReal_i16 _atanTable[256] =
{
    0x00000000, 0x00000101, 0x00000201, 0x00000302, 0x00000403, 0x00000504, 0x00000605, 0x00000706,
    0x00000807, 0x00000908, 0x00000A08, 0x00000B09, 0x00000C09, 0x00000D0A, 0x00000E0A, 0x00000F0A,
    0x0000100A, 0x0000110A, 0x0000120A, 0x0000130A, 0x00001409, 0x00001508, 0x00001608, 0x00001707,
    0x00001805, 0x00001904, 0x00001A03, 0x00001B01, 0x00001BFF, 0x00001CFD, 0x00001DFA, 0x00001EF8,
    0x00001FF5, 0x000020F2, 0x000021EE, 0x000022EB, 0x000023E7, 0x000024E3, 0x000025DE, 0x000026DA,
    0x000027D5, 0x000028CF, 0x000029CA, 0x00002AC4, 0x00002BBD, 0x00002CB7, 0x00002DB0, 0x00002EA9,
    0x00002FA1, 0x00003099, 0x00003191, 0x00003288, 0x0000337F, 0x00003476, 0x0000356C, 0x00003661,
    0x00003757, 0x0000384C, 0x00003940, 0x00003A35, 0x00003B28, 0x00003C1C, 0x00003D0E, 0x00003E01,
    0x00003EF3, 0x00003FE4, 0x000040D6, 0x000041C6, 0x000042B6, 0x000043A6, 0x00004495, 0x00004584,
    0x00004672, 0x00004760, 0x0000484D, 0x0000493A, 0x00004A26, 0x00004B12, 0x00004BFE, 0x00004CE8,
    0x00004DD3, 0x00004EBC, 0x00004FA5, 0x0000508E, 0x00005176, 0x0000525E, 0x00005345, 0x0000542B,
    0x00005511, 0x000055F7, 0x000056DB, 0x000057C0, 0x000058A3, 0x00005986, 0x00005A69, 0x00005B4B,
    0x00005C2C, 0x00005D0D, 0x00005DED, 0x00005ECD, 0x00005FAC, 0x0000608B, 0x00006168, 0x00006246,
    0x00006322, 0x000063FE, 0x000064DA, 0x000065B5, 0x0000668F, 0x00006768, 0x00006841, 0x0000691A,
    0x000069F2, 0x00006AC9, 0x00006B9F, 0x00006C75, 0x00006D4A, 0x00006E1F, 0x00006EF3, 0x00006FC6,
    0x00007099, 0x0000716B, 0x0000723D, 0x0000730D, 0x000073DE, 0x000074AD, 0x0000757C, 0x0000764A,
    0x00007718, 0x000077E5, 0x000078B1, 0x0000797D, 0x00007A48, 0x00007B12, 0x00007BDC, 0x00007CA5,
    0x00007D6D, 0x00007E35, 0x00007EFC, 0x00007FC3, 0x00008088, 0x0000814E, 0x00008212, 0x000082D6,
    0x00008399, 0x0000845C, 0x0000851E, 0x000085DF, 0x0000869F, 0x0000875F, 0x0000881F, 0x000088DD,
    0x0000899B, 0x00008A58, 0x00008B15, 0x00008BD1, 0x00008C8C, 0x00008D47, 0x00008E01, 0x00008EBB,
    0x00008F73, 0x0000902B, 0x000090E3, 0x0000919A, 0x00009250, 0x00009305, 0x000093BA, 0x0000946E,
    0x00009522, 0x000095D5, 0x00009687, 0x00009738, 0x000097E9, 0x0000989A, 0x00009949, 0x000099F8,
    0x00009AA7, 0x00009B55, 0x00009C02, 0x00009CAE, 0x00009D5A, 0x00009E05, 0x00009EB0, 0x00009F5A,
    0x0000A003, 0x0000A0AC, 0x0000A154, 0x0000A1FB, 0x0000A2A2, 0x0000A348, 0x0000A3EE, 0x0000A493,
    0x0000A537, 0x0000A5DB, 0x0000A67E, 0x0000A720, 0x0000A7C2, 0x0000A864, 0x0000A904, 0x0000A9A4,
    0x0000AA44, 0x0000AAE2, 0x0000AB81, 0x0000AC1E, 0x0000ACBB, 0x0000AD58, 0x0000ADF4, 0x0000AE8F,
    0x0000AF29, 0x0000AFC3, 0x0000B05D, 0x0000B0F6, 0x0000B18E, 0x0000B226, 0x0000B2BD, 0x0000B353,
    0x0000B3E9, 0x0000B47F, 0x0000B513, 0x0000B5A7, 0x0000B63B, 0x0000B6CE, 0x0000B761, 0x0000B7F3,
    0x0000B884, 0x0000B915, 0x0000B9A5, 0x0000BA35, 0x0000BAC4, 0x0000BB52, 0x0000BBE0, 0x0000BC6E,
    0x0000BCFB, 0x0000BD87, 0x0000BE13, 0x0000BE9E, 0x0000BF29, 0x0000BFB3, 0x0000C03C, 0x0000C0C5,
    0x0000C14E, 0x0000C1D6, 0x0000C25E, 0x0000C2E4, 0x0000C36B, 0x0000C3F1, 0x0000C476, 0x0000C4FB,
    0x0000C57F, 0x0000C603, 0x0000C687, 0x0000C709, 0x0000C78C, 0x0000C80D, 0x0000C88F, 0x0000C90F
};

leReal_i16 leReal_i16_Min(leReal_i16 l, leReal_i16 r)
{
    if(l < r)
    {
        return l;
    }

    return r;
}

leReal_i16 leReal_i16_Max(leReal_i16 l, leReal_i16 r)
{
    if(l > r)
    {
        return l;
    }

    return r;
}

leReal_i16 leReal_i16_FromInt(int32_t i)
{
    if(i < 0)
    {
        i = ~(i) + 1;
        i <<= LE_REAL_I16_FRACTION_BITS;
        i = ~(i) + 1;
    }
    else
    {
        i <<= LE_REAL_I16_FRACTION_BITS;
    }

    return (leReal_i16)i;
}

int32_t leReal_i16_ToInt(leReal_i16 rl)
{
    if(rl < 0)
    {
        rl = ~(rl) + 1;
        rl >>= LE_REAL_I16_FRACTION_BITS;
        rl = ~(rl) + 1;
    }
    else
    {
        rl >>= LE_REAL_I16_FRACTION_BITS;
    }

    return (int32_t)rl;
}

leReal_i16 leReal_i16_Round(leReal_i16 rl)
{
    uint32_t t;

    if(rl < 0)
    {
        t = (uint32_t)(~(rl) + 1);

        if((t & LE_REAL_I16_ONE_OVER_TWO) > 0)
        {
            t &= 0x7FFF0000;
            t += LE_REAL_I16_ONE;
        }
        else
        {
            t &= 0x7FFF0000;
            t -= LE_REAL_I16_ONE;
        }

        return (leReal_i16)(~(t) + 1);
    }
    else
    {
        t = (uint32_t)rl;

        if((rl & LE_REAL_I16_ONE_OVER_TWO) > 0)
        {
            t &= 0x7FFF0000;
            t += LE_REAL_I16_ONE;
        }
        else
        {
            t &= 0x7FFF0000;
        }

        return (leReal_i16)(t);
    }
}

int32_t leReal_i16_Floor(leReal_i16 rl)
{
    uint32_t t;

    if(rl < 0)
    {
        t = (uint32_t)(~(rl) + 1);
        t &= 0x7FFF0000;

        return (leReal_i16)(~(t) + 1);
    }
    else
    {
        t = (uint32_t)rl;
        t &= 0x7FFF0000;

        return (leReal_i16)(t);
    }
}

int32_t leReal_i16_Ceiling(leReal_i16 rl)
{
    uint32_t t;

    if(rl < 0)
    {
        t = (uint32_t)(~(rl) + 1);

        if((t & 0xFFFF) > 0)
        {
            t &= 0x7FFF0000;
            t += LE_REAL_I16_ONE;
        }

        return (leReal_i16)(~(t) + 1);
    }
    else
    {
        t = (uint32_t)rl;

        if((t & 0xFFFF) > 0)
        {
            t &= 0x7FFF0000;
            t += LE_REAL_I16_ONE;
        }

        return (leReal_i16)(t);
    }
}

int32_t leReal_i16_Middle(leReal_i16 rl)
{
    uint32_t t;

    if(rl < 0)
    {
        t = (uint32_t)(~(rl) + 1);
        t &= 0x7FFF0000;
        t |= 0x8000;

        return (leReal_i16)(~(t) + 1);
    }
    else
    {
        t = (uint32_t)rl;
        t &= 0x7FFF0000;
        t |= 0x8000;

        return (leReal_i16)(t);
    }
}

leReal_i16 leReal_i16_AbsoluteValue(leReal_i16 rl)
{
    if(rl < 0)
        return ~(rl) + 1;

    return rl;
}

leReal_u8  leReal_i16_Square(leReal_i16 rl)
{
    uint64_t val;

    if(rl < 0)
    {
        val = ~(rl) + 1;
    }
    else
    {
        val = rl;
    }

    val = (val * val) / LE_REAL_I16_FRACTION_SCL;

    val >>= (LE_REAL_I16_FRACTION_BITS - LE_REAL_U8_FRACTION_BITS);

    return val;
}

leReal_i16 leReal_i16_SquareRoot(leReal_i16 rl)
{
    leReal_i16 root;
    leReal_i16 x;
    uint32_t val;

    x = leReal_i16_AbsoluteValue(rl);

    if(x == 0)
        return 0;

    while(1)
    {
        root = (leReal_i16)(((int64_t)rl * LE_REAL_I16_FRACTION_SCL) / x);
        root += x;
        root = (leReal_i16)(((int64_t)root * LE_REAL_I16_ONE_OVER_TWO) / LE_REAL_I16_FRACTION_SCL);

        //printf("%f\n", LE_REAL_I16_TO_FLOAT(root));

        val = root - x;

        if(val & 0x80000000)
        {
            val = ~(val) + 1;
        }

        if(val < TWO_TO_MINUS_10) // if value is less than 2^(-10) or 0.000244141
            break;

        //printf("%f, %f, %f, %u\n", LE_REAL_I16_TO_FLOAT(rl), LE_REAL_I16_TO_FLOAT(root), LE_REAL_I16_TO_FLOAT(x), val);

        x = root;
    }

    return root;
}

leReal_i16 leReal_i16_InverseSquareRoot(leReal_i16 rl)
{
    leReal_i16 root = leReal_i16_SquareRoot(rl);

    return LE_REAL_I16_DIVIDE(LE_REAL_I16_ONE, root);
}

leReal_i16 leReal_i16_NormalizeAngle(leReal_i16 r)
{
    while(r > LE_REAL_I16_TWO_PI)
    {
        r -= LE_REAL_I16_TWO_PI;
    }

    while(r < LE_REAL_I16_TWO_PI)
    {
        r += LE_REAL_I16_TWO_PI;
    }

    return r;
}

leReal_i16 leReal_i16_Sin(leReal_i16 rad)
{
    if(rad < 0)
    {
        return -leReal_i16_Cos(rad + LE_REAL_I16_PI_OVER_TWO);
    }
    else
    {
        return leReal_i16_Cos(rad - LE_REAL_I16_PI_OVER_TWO);
    }
}

leReal_i16 leReal_i16_Cos(leReal_i16 rad)
{
    leReal_i16 angle;

    angle = rad;

    while(angle < 0)
    {
        angle += LE_REAL_I16_TWO_PI;
    }

    angle = LE_REAL_I16_MULTIPLY(angle, LE_REAL_I16_DEGREES);
    angle = leReal_i16_Round(angle);
    angle = leReal_i16_ToInt(angle);

    while(angle > 360)
    {
        angle -= 360;
    }

    while(angle < -360)
    {
        angle += 360;
    }

    if(angle > 180)
    {
        angle = 360 - angle;
    }

    if(angle > 90)
    {
        return -_cosTable[90 - (angle - 90)];
    }
    else
    {
        return _cosTable[angle];
    }
}

leReal_i16 leReal_i16_Acos(leReal_i16 val)
{
    size_t idx;

    if(val > LE_REAL_I16_ONE || val < LE_REAL_I16_MINUS_ONE)
        return 0;

    val = LE_REAL_I16_MULTIPLY(val, ACOS_SHIFT);
    val += ACOS_SHIFT;

    idx = (val >> LE_REAL_I16_FRACTION_BITS);

    return _acosTable[idx];
}

#if 0
static float atan2Test(float x)
{
    return M_PI_4*x - x*(fabs(x) - 1)*(0.2447 + 0.0663*fabs(x));
}

// PI/4 * x - x * (fabs(x) - 1) * (0.2447 + 0.0663 * fabs(x));

// t0 - x * (fabs(x) - 1) * (0.2447 + 0.0663 * fabs(x));
// t0 - x * (t1) * (0.2447 + 0.0663 * fabs(x));
// t0 - t1 * (0.2447 + 0.0663 * fabs(x));
// t0 - t1 * (0.2447 + t2);
// t0 - t1 * t2;
// t0 - t1;
// r

leReal_i16 leReal_i16_Atan(leReal_i16 val)
{
    leReal_i16 r;
    leReal_i16 absVal = val;

    leReal_i16 t0;
    leReal_i16 t1;
    leReal_i16 t2;

    if(val < 0)
    {
        absVal = ~(val) + 1;
    }

    t0 = LE_REAL_I16_MULTIPLY(LE_REAL_I16_PI_OVER_FOUR, val);

    t1 = LE_REAL_I16_MULTIPLY(val, absVal - LE_REAL_I16_ONE);
    t1 = LE_REAL_I16_MULTIPLY(val, t1);

    t2 = LE_REAL_I16_MULTIPLY(ATAN_CONST_B, absVal);
    t2 = ATAN_CONST_A + t2;

    t1 = LE_REAL_I16_MULTIPLY(t1, t2);

    r = t0 - t1;

    return r;
}
#endif

leReal_i16 leReal_i16_Atan2(leReal_i16 y, leReal_i16 x)
{
    leReal_i16 absx, absy, val;

    if (x == 0 && y == 0)
        return 0;

    absy = leReal_i16_AbsoluteValue(y);
    absx = leReal_i16_AbsoluteValue(x);

    if(absy - absx == absy)
        return y < 0 ? -LE_REAL_I16_PI_OVER_TWO : LE_REAL_I16_PI_OVER_TWO;

    if(absx - absy == absx)
    {
        if(x == LE_REAL_I16_MINUS_ONE)
        {
            return LE_REAL_I16_PI;
        }
        else if(absx == 0)
        {
            return 0;
        }

        val = 0;
    }
    else
    {
        // quadrant 1 or 2
        if(y > 0)
        {
            // 1st octant
            if(absx > absy)
            {
                val = LE_REAL_I16_MULTIPLY(LE_REAL_I16_255, absy);
                val = LE_REAL_I16_DIVIDE(val, absx);
                val = _atanTable[leReal_i16_ToInt(val)];
            }
            // 2nd octant
            else
            {
                val = LE_REAL_I16_MULTIPLY(LE_REAL_I16_255, absx);
                val = LE_REAL_I16_DIVIDE(val, absy);
                val = LE_REAL_I16_PI_OVER_TWO - _atanTable[leReal_i16_ToInt(val)];
            }

            // shift to quadrant 2 from 1
            if(x < 0)
            {
                val = (LE_REAL_I16_PI - val);
            }
        }
        // quadrant 3 or 4
        else
        {
            // 8th octant
            if(absx > absy)
            {
                val = LE_REAL_I16_MULTIPLY(LE_REAL_I16_255, absy);
                val = LE_REAL_I16_DIVIDE(val, absx);
                val = -_atanTable[leReal_i16_ToInt(val)];
            }
            // 7th octant
            else
            {
                val = LE_REAL_I16_MULTIPLY(LE_REAL_I16_255, absx);
                val = LE_REAL_I16_DIVIDE(val, absy);
                val = -LE_REAL_I16_PI_OVER_TWO + _atanTable[leReal_i16_ToInt(val)];
            }

            // shift to quatdrant 3 from 4
            if(x < 0)
            {
                val = LE_REAL_I16_MINUS_PI - val;
            }
        }
    }

    return val;
}

leReal_i16 leReal_i16_FromU8(leReal_u8 u)
{
    //uint32_t val =
    leReal_i16 i = (leReal_i16)((u) << (LE_REAL_I16_FRACTION_BITS - LE_REAL_U8_FRACTION_BITS));

    if(u != 0 && i == 0)
    {
        i = 1;
    }

    return i;
}