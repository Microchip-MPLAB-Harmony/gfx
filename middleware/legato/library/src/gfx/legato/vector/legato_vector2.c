#include "gfx/legato/vector/legato_vector2.h"

void leVector2_FromPoint(const struct lePoint* p,
                         struct leVector2* o)
{
    o->x = leReal_i16_FromInt(p->x);
    o->y = leReal_i16_FromInt(p->y);
}

void leVector2_ToPoint(const struct leVector2* v,
                       struct lePoint* o)
{
    o->x = leReal_i16_ToInt(v->x);
    o->y = leReal_i16_ToInt(v->y);
}

void leVector2_Negate(const struct leVector2* v,
                      struct leVector2* o)
{
    struct leVector2 res;

    res.x = -v->x;
    res.y = -v->y;

    *o = res;
}

leBool leVector2_Equals(const struct leVector2* l,
                        const struct leVector2* r)
{
    return(LE_REAL_I16_EQUALS(l->x, r->x) &&
           LE_REAL_I16_EQUALS(l->y, r->y));
}

void leVector2_Add(const struct leVector2* l,
                   const struct leVector2* r,
                   struct leVector2* o)
{
    struct leVector2 res;

    res.x = l->x + r->x;
    res.y = l->y + r->y;

    *o = res;
}

void leVector2_Subtract(const struct leVector2* l,
                        const struct leVector2* r,
                        struct leVector2* o)
{
    struct leVector2 res;

    res.x = l->x - r->x;
    res.y = l->y - r->y;

    *o = res;
}

void leVector2_Multiply(const struct leVector2* l,
                        const struct leVector2* r,
                        struct leVector2* o)
{
    struct leVector2 res;

    res.x = LE_REAL_I16_MULTIPLY(l->x, r->x);
    res.y = LE_REAL_I16_MULTIPLY(l->y, r->y);

    *o = res;
}

void leVector2_Divide(const struct leVector2* l,
                      const struct leVector2* r,
                      struct leVector2* o)
{
    struct leVector2 res;

    res.x = LE_REAL_I16_DIVIDE(l->x, r->x);
    res.y = LE_REAL_I16_DIVIDE(l->y, r->y);

    *o = res;
}

void leVector2_Translate(const struct leVector2* v,
                         leReal_i16 x,
                         leReal_i16 y,
                         struct leVector2* o)
{
    o->x = v->x + x;
    o->y = v->y + y;
}

void leVector2_TranslateAlongVector(const struct leVector2* v,
                                    const struct leVector2* dir,
                                    leReal_i16 mag,
                                    struct leVector2* o)
{
    o->x = v->x + LE_REAL_I16_MULTIPLY(dir->x, mag);
    o->y = v->y + LE_REAL_I16_MULTIPLY(dir->y, mag);
}

void leVector2_Scale(const struct leVector2* v,
                     leReal_i16 mag,
                     struct leVector2* o)
{
    struct leVector2 res;

    res.x = LE_REAL_I16_MULTIPLY(v->x, mag);
    res.y = LE_REAL_I16_MULTIPLY(v->y, mag);

    *o = res;
}

void leVector2_Normalize(const struct leVector2* v,
                         struct leVector2* o)
{
    leReal_i16 f;
    struct leVector2 res;
    leReal_u8 x2;
    leReal_u8 y2;
    leReal_u8 uroot;

    if(v->x == 0 && v->y == 0)
    {
        o->x = v->x;
        o->y = v->y;

        return;
    }

    if(LE_REAL_I16_EQUALS(v->x, 0))
    {
        res.x = 0;

        if(v->y < 0)
        {
            res.y = LE_REAL_I16_MINUS_ONE;
        }
        else
        {
            res.y = LE_REAL_I16_ONE;
        }
    }
    else if(LE_REAL_I16_EQUALS(v->y, 0))
    {
        res.y = 0;

        if(v->x < 0)
        {
            res.x = LE_REAL_I16_MINUS_ONE;
        }
        else
        {
            res.x = LE_REAL_I16_ONE;
        }
    }
    else
    {
        x2 = leReal_i16_Square(v->x);
        y2 = leReal_i16_Square(v->y);

        uroot = leReal_u8_SquareRoot(x2 + y2);

        if(uroot != 0)
        {
            f = leReal_i16_FromU8(uroot);

            f = LE_REAL_I16_DIVIDE(LE_REAL_I16_ONE, f);

            res.x = LE_REAL_I16_MULTIPLY(v->x, f);
            res.y = LE_REAL_I16_MULTIPLY(v->y, f);
        }
        else
        {
            res.x = v->x;
            res.y = v->y;
        }
    }

    *o = res;
}

leReal_i16 leVector2_Length(const struct leVector2* v)
{
    return LE_REAL_U8_TO_I16(leReal_u8_SquareRoot(leVector2_SquaredLength(v)));
}

leReal_i16 leVector2_ReciprocalLength(const struct leVector2* v)
{
    leReal_i16 rt;

    rt = leVector2_Length(v);

    return LE_REAL_I16_DIVIDE(LE_REAL_I16_ONE, rt);
}

leReal_u8 leVector2_SquaredLength(const struct leVector2* v)
{
    leReal_u8 x2;
    leReal_u8 y2;

    x2 = leReal_i16_Square(v->x);
    y2 = leReal_i16_Square(v->y);

    return x2 + y2;
}

void leVector2_Rotate(const struct leVector2* v,
                      leReal_i16 rad,
                      struct leVector2* o)
{
    leReal_i16 s, c;
    struct leVector2 res;

    s = leReal_i16_Sin(rad);
    c = leReal_i16_Cos(rad);

    res.x = LE_REAL_I16_MULTIPLY(c, v->x) - LE_REAL_I16_MULTIPLY(s, v->y);
    res.y = LE_REAL_I16_MULTIPLY(s, v->x) + LE_REAL_I16_MULTIPLY(c, v->y);

    *o = res;
}

leReal_i16 leVector2_Dot(const struct leVector2* l,
                         const struct leVector2* r)
{
    return LE_REAL_I16_MULTIPLY(l->x, r->x) + LE_REAL_I16_MULTIPLY(l->y, r->y);
}

void leVector2_ProjectOnto(const struct leVector2* l,
                           const struct leVector2* r,
                           struct leVector2* o)
{
    leReal_i16 f;
    struct leVector2 res;

    f = leVector2_Dot(l, r);
    f = leReal_i16_AbsoluteValue(f);
    f = LE_REAL_I16_DIVIDE(f, leVector2_Length(r));

    //f = leReal_AbsoluteValue(leVector2_Dot(l, r)) / leVector2_Length(r);

    res = *r;

    res.x *= f;
    res.y *= f;

    *o = res;
}

leReal_i16 leVector2_AngleBetween(const struct leVector2* l,
                                  const struct leVector2* r)
{
    leReal_i16 length;
    leReal_i16 val;

    length = leVector2_Length(l) * leVector2_Length(r);

    val = leVector2_Dot(l, r);
    val = LE_REAL_I16_DIVIDE(val, length);

    return leReal_i16_Acos(val);
}

leReal_i16 leVector2_AngleOf(const struct leVector2* v)
{
    leReal_i16 val;
    struct leVector2 nm;

    leVector2_Normalize(v, &nm);

    val = leReal_i16_Atan2(nm.y, nm.x);

    return val;
}

void leVector2_CounterClockwiseNormal(const struct leVector2* v,
                                      struct leVector2* o)
{
    o->x = v->y;
    o->y = -v->x;
}

void leVector2_ClockwiseNormal(const struct leVector2* v,
                               struct leVector2* o)
{
    o->x = -v->y;
    o->y = v->x;
}

void leVector2_NormalizedDirectionVector(const struct leVector2* s,
                                         const struct leVector2* t,
                                         struct leVector2* o)
{
    leVector2_Subtract(s, t, o);
    leVector2_Normalize(o, o);
}

void leVector2_DirectionVector(const struct leVector2* s,
                               const struct leVector2* t,
                               struct leVector2* o)
{
    leVector2_Subtract(s, t, o);
}

const struct leVector2 LE_VECTOR2_ZERO =
{
    .x = 0,
    .y = 0
};

const struct leVector2 LE_VECTOR2_UNIT =
{
    .x = LE_REAL_I16_ONE,
    .y = LE_REAL_I16_ONE
};

const struct leVector2 LE_VECTOR2_UNIT_X =
{
    .x = LE_REAL_I16_ONE,
    .y = 0
};

const struct leVector2 LE_VECTOR2_UNIT_Y =
{
    .x = 0,
    .y = LE_REAL_I16_ONE
};

const struct leVector2 LE_VECTOR2_NEG_UNIT =
{
    .x = LE_REAL_I16_MINUS_ONE,
    .y = LE_REAL_I16_MINUS_ONE
};

const struct leVector2 LE_VECTOR2_NEG_UNIT_X =
{
    .x = LE_REAL_I16_MINUS_ONE,
    .y = 0
};

const struct leVector2 LE_VECTOR2_NEG_UNIT_Y =
{
    .x = 0,
    .y = LE_REAL_I16_MINUS_ONE
};