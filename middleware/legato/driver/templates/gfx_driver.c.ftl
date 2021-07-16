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


#include <math.h>
#include "gfx/driver/gfx_driver.h"

typedef gfxColor (*color_convert_fn_ptr)(gfxColor);

static gfxColor no_conv(gfxColor color);

int32_t gfxScaleIntegerSigned(int32_t num, int32_t oldMax, int32_t newMax);
uint32_t gfxScaleInteger(uint32_t num, uint32_t oldMax, uint32_t newMax);


/* color conversion function definitions */
static gfxColor gs8_rgb332(gfxColor color);
static gfxColor gs8_rgb565(gfxColor color);
static gfxColor gs8_rgba5551(gfxColor color);
static gfxColor gs8_rgb888(gfxColor color);
static gfxColor gs8_rgba8888(gfxColor color);
static gfxColor gs8_argb8888(gfxColor color);

static gfxColor rgb332_gs8(gfxColor color);
static gfxColor rgb332_rgb565(gfxColor color);
static gfxColor rgb332_rgba5551(gfxColor color);
static gfxColor rgb332_rgb888(gfxColor color);
static gfxColor rgb332_rgba8888(gfxColor color);
static gfxColor rgb332_argb8888(gfxColor color);

static gfxColor rgb565_gs8(gfxColor color);
static gfxColor rgb565_rgb332(gfxColor color);
static gfxColor rgb565_rgba5551(gfxColor color);
static gfxColor rgb565_rgb888(gfxColor color);
static gfxColor rgb565_rgba8888(gfxColor color);
static gfxColor rgb565_argb8888(gfxColor color);

static gfxColor rgba5551_gs8(gfxColor color);
static gfxColor rgba5551_rgb332(gfxColor color);
static gfxColor rgba5551_rgb565(gfxColor color);
static gfxColor rgba5551_rgb888(gfxColor color);
static gfxColor rgba5551_rgba8888(gfxColor color);
static gfxColor rgba5551_argb8888(gfxColor color);

static gfxColor rgb888_gs8(gfxColor color);
static gfxColor rgb888_rgb332(gfxColor color);
static gfxColor rgb888_rgb565(gfxColor color);
static gfxColor rgb888_rgba5551(gfxColor color);
static gfxColor rgb888_rgba8888(gfxColor color);
static gfxColor rgb888_argb8888(gfxColor color);

static gfxColor rgba8888_gs8(gfxColor color);
static gfxColor rgba8888_rgb332(gfxColor color);
static gfxColor rgba8888_rgb565(gfxColor color);
static gfxColor rgba8888_rgba5551(gfxColor color);
static gfxColor rgba8888_rgb888(gfxColor color);
static gfxColor rgba8888_argb8888(gfxColor color);

static gfxColor argb8888_rgba5551(gfxColor color);
static gfxColor argb8888_rgb888(gfxColor color);
static gfxColor argb8888_rgba8888(gfxColor color);


/* color conversion function matrix */
color_convert_fn_ptr gfx_color_convert_table[GFX_COLOR_MODE_COUNT][GFX_COLOR_MODE_COUNT] =
{
//                GS8            RGB332            RGB565            RGBA_5551           RGB_888           RGBA_8888           ARGB_8888           LUT1      LUT4      LUT8
/* GS8       */ { &no_conv,      &gs8_rgb332,      &gs8_rgb565,      &gs8_rgba5551,      &gs8_rgb888,      &gs8_rgba8888,      &gs8_argb8888,      &no_conv, &no_conv, &no_conv },
/* RGB332    */ { &rgb332_gs8,   &no_conv,         &rgb332_rgb565,   &rgb332_rgba5551,   &rgb332_rgb888,   &rgb332_rgba8888,   &rgb332_argb8888,   &no_conv, &no_conv, &no_conv },
/* RGB565    */ { &rgb565_gs8,   &rgb565_rgb332,   &no_conv,         &rgb565_rgba5551,   &rgb565_rgb888,   &rgb565_rgba8888,   &rgb565_argb8888,   &no_conv, &no_conv, &no_conv },
/* RGBA_5551 */ { &rgba5551_gs8, &rgba5551_rgb332, &rgba5551_rgb565, &no_conv,           &rgba5551_rgb888, &rgba5551_rgba8888, &rgba5551_argb8888, &no_conv, &no_conv, &no_conv },
/* RGB_888   */ { &rgb888_gs8,   &rgb888_rgb332,   &rgb888_rgb565,   &rgb888_rgba5551,   &no_conv,         &rgb888_rgba8888,   &rgb888_argb8888,   &no_conv, &no_conv, &no_conv },
/* RGBA_8888 */ { &rgba8888_gs8, &rgba8888_rgb332, &rgba8888_rgb565, &rgba8888_rgba5551, &rgba8888_rgb888, &no_conv,           &rgba8888_argb8888, &no_conv, &no_conv, &no_conv },
/* ARGB_8888 */ { &rgb888_gs8,   &rgb888_rgb332,   &rgb888_rgb565,   &argb8888_rgba5551, &argb8888_rgb888, &argb8888_rgba8888, &no_conv,           &no_conv, &no_conv, &no_conv },
/* LUT1      */ { &no_conv,      &no_conv,         &no_conv,         &no_conv,           &no_conv,         &no_conv,           &no_conv,           &no_conv, &no_conv, &no_conv },
/* LUT4      */ { &no_conv,      &no_conv,         &no_conv,         &no_conv,           &no_conv,         &no_conv,           &no_conv,           &no_conv, &no_conv, &no_conv },
/* LUT8      */ { &no_conv,      &no_conv,         &no_conv,         &no_conv,           &no_conv,         &no_conv,           &no_conv,           &no_conv, &no_conv, &no_conv },
};

/*
  From RGB to YUV

   Y = 0.299R + 0.587G + 0.114B
   U = 0.492 (B-Y)
   V = 0.877 (R-Y)

   It can also be represented as:

   Y =  0.299R + 0.587G + 0.114B
   U = -0.147R - 0.289G + 0.436B
   V =  0.615R - 0.515G - 0.100B


   From YUV to RGB

   R = Y + 1.140V
   G = Y - 0.395U - 0.581V
   B = Y + 2.032U
*/

static gfxColor no_conv(gfxColor color)
{
    return color;
}

/*
grayscale conversion formula
0.21 R + 0.72 G + 0.07 B
*/

static gfxColor gs8_rgb332(gfxColor color)
{
    gfxColor res;

    // red
    res = color & RGB_332_RED_MASK;

    // green
    res |= res >> 3;

    // blue
    res |= res >> 6;

    return res;
}

static gfxColor gs8_rgb565(gfxColor color)
{
    gfxColor res;

    // red
    res = (color & 0xF8) << 8;

    // green
    res |= (color & 0xFC) << 3;

    // blue
    res |= (color & 0xF8) >> 3;

    return res;
}

static gfxColor gs8_rgba5551(gfxColor color)
{
    gfxColor res;

    // red
    res = (color & 0xF8) << 8;

    // green
    res |= (color & 0xF8) << 3;

    // blue
    res |= (color & 0xF8) >> 2;

    // alpha
    res |= RGBA_5551_ALPHA_MASK;

    return res;
}

static gfxColor gs8_rgb888(gfxColor color)
{
    // blue
    gfxColor res = color;

    // red
    res = color << 16;

    // green
    res |= color << 8;

    // green
    res |= color;

    return res;
}

static gfxColor gs8_rgba8888(gfxColor color)
{
    gfxColor res;

    // red
    res = color << 24;

    // green
    res |= color << 16;

    // blue
    res |= color << 8;

    // alpha
    res |= 0xFF;

    return res;
}

static gfxColor gs8_argb8888(gfxColor color)
{
    gfxColor res;

    // red
    res = color << 16;

    // green
    res |= color << 8;

    // blue
    res |= color;

    // alpha
    res |= 0xFF000000;

    return res;
}

static gfxColor rgb332_gs8(gfxColor color)
{
    //0.21 R + 0.72 G + 0.07 B
    uint32_t red, green, blue;

    red = (color & RGB_332_RED_MASK) >> 5;
    red = gfxScaleInteger(red, RGB_3_BITS, 55);

    green = (color & RGB_332_GREEN_MASK) >> 2;
    green = gfxScaleInteger(green, RGB_3_BITS, 183);

    blue = (color & RGB_332_BLUE_MASK);
    blue = gfxScaleInteger(blue, RGB_2_BITS, 17);

    return red + green + blue;
}

static gfxColor rgb332_rgb565(gfxColor color)
{
    uint32_t red, green, blue;

    red = ((color & RGB_332_RED_MASK) << 8) | 0x1800;
    green = ((color & RGB_332_GREEN_MASK) << 6) | 0xE0;
    blue = ((color & RGB_332_BLUE_MASK) << 3) | 0x3;

    return red | green | blue;
}

static gfxColor rgb332_rgba5551(gfxColor color)
{
    uint32_t red, green, blue;

    red = ((color & RGB_332_RED_MASK) << 8) | 0x1800;
    green = ((color & RGB_332_GREEN_MASK) << 6) | 0xC0;
    blue = ((color & RGB_332_BLUE_MASK) << 4) | 0x6;

    return red | green | blue;
}

static gfxColor rgb332_rgb888(gfxColor color)
{
    uint32_t red, green, blue;

    red = ((color & RGB_332_RED_MASK) << 16) | 0x1F0000;
    green = ((color & RGB_332_GREEN_MASK) << 11) | 0x1F00;
    blue = ((color & RGB_332_BLUE_MASK) << 6) | 0x1F;

    return red | green | blue;
}

static gfxColor rgb332_rgba8888(gfxColor color)
{
    uint32_t red, green, blue;

    red = ((color & RGB_332_RED_MASK) << 24) | 0x1F000000;
    green = ((color & RGB_332_GREEN_MASK) << 19) | 0x1F0000;
    blue = ((color & RGB_332_BLUE_MASK) << 14) | 0x1F00;

    return red | green | blue | 0xFF;
}

static gfxColor rgb332_argb8888(gfxColor color)
{
    uint32_t red, green, blue;

    red = ((color & RGB_332_RED_MASK) << 16) | 0x1F0000;
    green = ((color & RGB_332_GREEN_MASK) << 11) | 0x1F00;
    blue = ((color & RGB_332_BLUE_MASK) << 6) | 0x1F;

    return 0xFF000000 | red | green | blue;
}

static gfxColor rgb565_gs8(gfxColor color)
{
    //0.21 R + 0.72 G + 0.07 B
    uint32_t red, green, blue;

    red = (color & RGB_565_RED_MASK) >> 11;
    red = gfxScaleInteger(red, 7, 55);

    green = (color & RGB_565_GREEN_MASK) >> 5;
    green = gfxScaleInteger(green, 7, 183);

    blue = (color & RGB_565_BLUE_MASK);
    blue = gfxScaleInteger(blue, 7, 17);

    return red + green + blue;
}

static gfxColor rgb565_rgb332(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & 0xE000) >> 8;
    green = (color & 0x700) >> 6;
    blue = (color & 0x18) >> 3;

    return red | green | blue;
}

static gfxColor rgb565_rgba5551(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & RGB_565_RED_MASK);
    green = (color & RGBA_5551_GREEN_MASK);
    blue = (color & RGB_565_BLUE_MASK) << 1;

    return red | green | blue | RGBA_5551_ALPHA_MASK;
}

static gfxColor rgb565_rgb888(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & RGB_565_RED_MASK) << 8;
    green = (color & RGB_565_GREEN_MASK) << 5;
    blue = (color & RGB_565_BLUE_MASK) << 3;

    return red | green | blue;
}

static gfxColor rgb565_rgba8888(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & RGB_565_RED_MASK) << 16;
    green = (color & RGB_565_GREEN_MASK) << 13;
    blue = (color & RGB_565_BLUE_MASK) << 11;

    return red | green | blue | 0xFF;
}

static gfxColor rgb565_argb8888(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & RGB_565_RED_MASK) << 8;
    green = (color & RGB_565_GREEN_MASK) << 5;
    blue = (color & RGB_565_BLUE_MASK) << 3;

    return 0xFF000000 | red | green | blue;
}

static gfxColor rgba5551_gs8(gfxColor color)
{
    return color;
}

static gfxColor rgba5551_rgb332(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & 0xE000) >> 8;
    green = (color & 0x700) >> 6;
    blue = (color & 0x30) >> 4;

    return red | green | blue;
}

static gfxColor rgba5551_rgb565(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & RGBA_5551_RED_MASK);
    green = (color & RGBA_5551_GREEN_MASK) | 0x20;
    blue = (color & RGBA_5551_BLUE_MASK) >> 1;

    return red | green | blue;
}

static gfxColor rgba5551_rgb888(gfxColor color)
{
    uint32_t red, green, blue;

    red = ((color & RGBA_5551_RED_MASK) << 8) | 0x70000;
    green = ((color & RGBA_5551_GREEN_MASK) << 5) | 0x700;
    blue = ((color & RGBA_5551_BLUE_MASK) << 2) | 0x7;

    return red | green | blue;
}

static gfxColor rgba5551_rgba8888(gfxColor color)
{
    uint32_t red, green, blue;

    red = ((color & RGBA_5551_RED_MASK) << 16) | 0x7000000;
    green = ((color & RGBA_5551_GREEN_MASK) << 13) | 0x70000;
    blue = ((color & RGBA_5551_BLUE_MASK) << 10) | 0x700;

    return red | green | blue | ((color & RGBA_5551_ALPHA_MASK) > 0 ? 0xFF : 0x0);
}

static gfxColor rgba5551_argb8888(gfxColor color)
{
    uint32_t red, green, blue;

    red = ((color & RGBA_5551_RED_MASK) << 8) | 0x70000;
    green = ((color & RGBA_5551_GREEN_MASK) << 5) | 0x700;
    blue = ((color & RGBA_5551_BLUE_MASK) << 2) | 0x7;

    return ((color & RGBA_5551_ALPHA_MASK) > 0 ? 0xFF000000 : 0x0) | red | green | blue;
}

static gfxColor rgb888_gs8(gfxColor color)
{
    return color;
}

static gfxColor rgb888_rgb332(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & 0xE00000) >> 17;
    green = (color & 0xE000) >> 8;
    blue = (color & 0xC0) >> 4;

    return red | green | blue;
}

static gfxColor rgb888_rgb565(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & 0xF80000) >> 8;
    green = (color & 0xFC00) >> 5;
    blue = (color & 0xF8) >> 3;

    return red | green | blue;
}

static gfxColor rgb888_rgba5551(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & 0xF80000) >> 8;
    green = (color & 0xF800) >> 5;
    blue = (color & 0xF8) >> 2;

    return red | green | blue | RGBA_5551_ALPHA_MASK;
}

static gfxColor rgb888_rgba8888(gfxColor color)
{
    return (color << 8) | 0xFF;
}

static gfxColor rgb888_argb8888(gfxColor color)
{
    return color | 0xFF000000;
}

static gfxColor rgba8888_gs8(gfxColor color)
{
    return color;
}

static gfxColor rgba8888_rgb332(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & 0xE0000000) >> 24;
    green = (color & 0xE00000) >> 19;
    blue = (color & 0xC000) >> 14;

    return red | green | blue;
}

static gfxColor rgba8888_rgb565(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & 0xF8000000) >> 16;
    green = (color & 0xFC0000) >> 13;
    blue = (color & 0xF800) >> 11;

    return red | green | blue;
}

static gfxColor rgba8888_rgba5551(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & 0xF8000000) >> 16;
    green = (color & 0xF80000) >> 13;
    blue = (color & 0xF800) >> 10;

    return red | green | blue | ((color & RGBA_8888_ALPHA_MASK) > 0 ? RGBA_5551_ALPHA_MASK : 0);
}

static gfxColor rgba8888_rgb888(gfxColor color)
{
    gfxColor res;

    // red
    res  = (color & 0xFF000000) >> 8;

    // green
    res |= (color & 0x00FF0000) >> 8;

    // blue
    res |= (color & 0x0000FF00) >> 8;

    return res;
}

static gfxColor rgba8888_argb8888(gfxColor color)
{
    return (color >> 8) | (((color & 0xFF) << 24));
}

static gfxColor argb8888_rgba5551(gfxColor color)
{
    uint32_t red, green, blue;

    red = (color & 0xF80000) >> 8;
    green = (color & 0xF800) >> 5;
    blue = (color & 0xF8) >> 2;

    return red | green | blue | ((color & RGBA_8888_ALPHA_MASK) > 0 ? RGBA_5551_ALPHA_MASK : 0);
}

static gfxColor argb8888_rgb888(gfxColor color)
{
    return color & 0xFFFFFF;
}

static gfxColor argb8888_rgba8888(gfxColor color)
{
    return (color << 8) | ((color & 0xFF000000) >> 24);
}


const gfxColorModeInfo gfxColorInfoTable[] =
{

    {1,8,  GFX_BPP8,  {0x0,0x0,0x0,0x0},{0x0,0x0,0x0,0x0}},                        // GFX_COLOR_MODE_GS_8
    {1,8,  GFX_BPP8,  {0xE0,0x1C,0x3,0x0},{0x0,0x0,0x0,0x0}},                      // GFX_COLOR_MODE_RGB_332
    {2,16, GFX_BPP16, {0xF800,0x07E0,0x001F,0x0},{11,5,0,0}},                      // GFX_COLOR_MODE_RGB_565
    {2,16, GFX_BPP16, {0xF800,0x07C0,0x003E,0x1},{11,6,1,0}},                      // GFX_COLOR_MODE_RGBA_5551
    {3,24, GFX_BPP24, {0xFF0000,0x00FF00,0x0000FF,0x0},{16,8,0,0}},                // GFX_COLOR_MODE_RGB_888
    {4,32, GFX_BPP32, {0xFF000000,0x00FF0000,0x0000FF00,0x000000FF},{24,16,8,0}},  // GFX_COLOR_MODE_RGBA_8888
    {4,32, GFX_BPP32, {0x00FF0000,0x0000FF00,0x000000FF,0xFF000000},{16,8,0,24}},  // GFX_COLOR_MODE_ARGB_8888
    {1,1,  GFX_BPP1,  {0x0,0x0,0x0,0x0},{0x0,0x0,0x0,0x0}},                        // GFX_COLOR_MODE_INDEX_1
    {1,4,  GFX_BPP4,  {0x0,0x0,0x0,0x0},{0x0,0x0,0x0,0x0}},                        // GFX_COLOR_MODE_INDEX_4
    {1,8,  GFX_BPP8,  {0x0,0x0,0x0,0x0},{0x0,0x0,0x0,0x0}},                        // GFX_COLOR_MODE_INDEX_8
    {1,1,  GFX_BPP1,  {0x0,0x0,0x0,0x0},{0x0,0x0,0x0,0x0}},                        // GFX_COLOR_MODE_MONO
};

uint32_t gfxColorChannelRed(gfxColor clr, gfxColorMode mode)
{
    return (clr & gfxColorInfoTable[mode].mask.red) >> gfxColorInfoTable[mode].shift.red;
}

uint32_t gfxColorChannelGreen(gfxColor clr, gfxColorMode mode)
{
    return (clr & gfxColorInfoTable[mode].mask.green) >> gfxColorInfoTable[mode].shift.green;
}

uint32_t gfxColorChannelBlue(gfxColor clr, gfxColorMode mode)
{
    return (clr & gfxColorInfoTable[mode].mask.blue) >> gfxColorInfoTable[mode].shift.blue;
}

uint32_t gfxColorChannelAlpha(gfxColor clr, gfxColorMode mode)
{
    if(mode == GFX_COLOR_MODE_RGBA_5551 ||
       mode == GFX_COLOR_MODE_RGBA_8888 ||
       mode == GFX_COLOR_MODE_ARGB_8888)
        return (clr & gfxColorInfoTable[mode].mask.alpha) >> gfxColorInfoTable[mode].shift.alpha;

    return 0xFF;
}

int32_t gfxScaleIntegerSigned(int32_t num, int32_t oldMax, int32_t newMax)
{
    float percent;

    percent = (float)num / (float)oldMax;
    percent *= newMax;

    return (int32_t)percent;
}


uint32_t gfxScaleInteger(uint32_t num, uint32_t oldMax, uint32_t newMax)
{
    float percent;

    percent = (float)num / (float)oldMax;
    percent *= newMax;

    return (uint32_t)percent;
}

static gfxColor gfxIndexGet1bpp(const gfxPixelBuffer* const buffer,
                            const uint32_t idx)
{
    uint32_t offs;
    uint8_t byte;
    gfxColor color;
    
    offs = (idx % 8);
            
    byte = *(((uint8_t*)buffer->pixels) + (idx >> 3));
    
    color = byte & (0x80 >> offs);
    
    color >>= 7 - offs;
    
    return color;
}

static gfxColor gfxIndexGet4bpp(const gfxPixelBuffer* const buffer,
                              const uint32_t idx)
{
    uint32_t offs;
    uint8_t byte;
    gfxColor color;
    
    byte = *(((uint8_t*)buffer->pixels) + (idx >> 1));
    
    if(idx % 2 > 0)
    {
        offs = 0xF;
        color = byte & offs;
    }
    else
    {
        offs = 0xF0;
        color = (byte & offs) >> 0x4;
    }
    
    return color;
}

static gfxColor gfxIndexGet(const gfxPixelBuffer* const buffer,
                          const uint32_t idx)
{
    uint8_t* buf_ptr;
    gfxColor color = 0;
    uint32_t i;

    buf_ptr = ((uint8_t*)buffer->pixels) + (gfxColorInfoTable[buffer->mode].bpp >> 3) * idx;
    
    for(i = 0; i < gfxColorInfoTable[buffer->mode].bpp >> 3; i++)
        ((uint8_t*)&color)[i] = buf_ptr[i];
    
    return color;
}
                               
typedef gfxColor (*PixelBufferIndexGet_FnPtr)(const gfxPixelBuffer* const,
                                               const uint32_t);
                                               
PixelBufferIndexGet_FnPtr gfxIndexGet_FnTable[] =
{
    &gfxIndexGet1bpp,
    &gfxIndexGet4bpp,
    &gfxIndexGet,
    &gfxIndexGet,
    &gfxIndexGet,
    &gfxIndexGet
};

typedef gfxColor (*GetPixel_FnPtr)(uint8_t*);

static gfxColor gfxGet1byte(uint8_t* ptr)
{
    gfxColor clr = ptr[0];

    return clr;
}

static gfxColor gfxGet2byte(uint8_t* ptr)
{
    gfxColor clr = ((uint16_t*)ptr)[0];

    return clr;
}

static gfxColor gfxGet3byte(uint8_t* ptr)
{
    gfxColor clr;
    
    clr = ptr[0];
    clr |= ptr[1] << 8;
    clr |= ptr[2] << 16;
    
    return clr;
}

static gfxColor gfxGet4byte(uint8_t* ptr)
{
    gfxColor clr = ((uint32_t*)ptr)[0];
    
    return clr;
}

GetPixel_FnPtr gfxPixelGet_FnTable[] =
{
    &gfxGet1byte,
    &gfxGet2byte,
    &gfxGet3byte,
    &gfxGet4byte
};

typedef gfxResult (*SetPixel_FnPtr)(uint8_t*, gfxColor);

static gfxResult gfxSet1byte(uint8_t* ptr, gfxColor clr)
{
    ptr[0] = (uint8_t)clr;
    
    return GFX_SUCCESS;
}

static gfxResult gfxSet2byte(uint8_t* ptr, gfxColor clr)
{
    *((uint16_t*)ptr) = (uint16_t)clr;
    
    return GFX_SUCCESS;
}

static gfxResult gfxSet3byte(uint8_t* ptr, gfxColor clr)
{
    ptr[0] = (uint8_t)(clr & 0xFF);
    ptr[1] = (uint8_t)((clr & 0xFF00) >> 8);
    ptr[2] = (uint8_t)((clr & 0xFF0000) >> 16);
    
    return GFX_SUCCESS;
}

static gfxResult gfxSet4byte(uint8_t* ptr, gfxColor clr)
{
    *((uint32_t*)ptr) = clr;
    
    return GFX_SUCCESS;
}

SetPixel_FnPtr gfxPixelSet_FnTable[] =
{
    &gfxSet1byte,
    &gfxSet2byte,
    &gfxSet3byte,
    &gfxSet4byte
};

gfxResult gfxPixelBufferCreate(const int32_t width,
                             const int32_t height,
                             const gfxColorMode mode,
                             const void* const address,
                             gfxPixelBuffer* buffer)
{
    if(buffer == NULL)
        return GFX_FAILURE;

    buffer->size.width = width;
    buffer->size.height = height;
    buffer->pixel_count = width * height;
    buffer->flags = GFX_BF_NONE;

    buffer->mode = mode;

    switch(mode)
    {
        case GFX_COLOR_MODE_INDEX_1:
        case GFX_COLOR_MODE_MONOCHROME:
        {
            buffer->buffer_length = (width * height) / 8;
            
            if((width * height) % 8 > 0)
                buffer->buffer_length += 1;
                        
            break;
        }
        case GFX_COLOR_MODE_INDEX_4:
        {
            buffer->buffer_length = (width * height) / 2;
            
            if((width * height) % 2 > 0)
                buffer->buffer_length += 1;
        
            break;
        }
        case GFX_COLOR_MODE_INDEX_8:
        {
            buffer->buffer_length = (width * height);
        
            break;
        }
        default:
        {
            buffer->buffer_length = width * height * ((gfxColorInfoTable[buffer->mode].bpp >> 3));
        }
    }

    buffer->pixels = (void*)address;

    return GFX_SUCCESS;
}

gfxBuffer gfxPixelBufferOffsetGet(const gfxPixelBuffer* const buffer,
                                uint32_t x,
                                uint32_t y)
{
    uint8_t* buf_ptr;

    if(buffer == NULL)
        return NULL;

    buf_ptr = (uint8_t*)buffer->pixels;
    
    if(x * y >= buffer->pixel_count)
        return NULL;
    
    return buf_ptr +
            gfxColorInfoTable[buffer->mode].size *
            (x + (y * buffer->size.width));
}

gfxBuffer gfxPixelBufferOffsetGet_Unsafe(const gfxPixelBuffer* const buffer,
                                       uint32_t x,
                                       uint32_t y)
{
    uint8_t* buf_ptr;

    buf_ptr = (uint8_t*)buffer->pixels;
    
    return buf_ptr +
           gfxColorInfoTable[buffer->mode].size *
           (x + (y * buffer->size.width));
}

gfxColor gfxPixelBufferGet(const gfxPixelBuffer* const buffer,
                         uint32_t x,
                         uint32_t y)
{
    if(buffer == NULL || x * y >= buffer->pixel_count)
        return 0x0;
        
    return gfxPixelBufferGet_Unsafe(buffer, x, y);
}

gfxColor gfxPixelBufferGet_Unsafe(const gfxPixelBuffer* const buffer,
                                uint32_t x,
                                uint32_t y)
{
    uint8_t* offs_ptr;
    gfxColor color = 0;
    uint32_t offs;
    uint32_t idx;
    uint8_t byte;

    if(buffer->mode == GFX_COLOR_MODE_MONOCHROME)
    {
        idx = ((x + (y * buffer->size.width)));
        offs = 1 << (7 - (idx % 8));

        byte = *(((uint8_t*)buffer->pixels) + (idx / 8));

        color = byte & offs;

        color >>= (7 - (idx % 8));

        return color;
    }
    
    offs_ptr = (uint8_t*)gfxPixelBufferOffsetGet(buffer, x, y);
    
    color = gfxPixelGet_FnTable[gfxColorInfoTable[buffer->mode].size - 1](offs_ptr);
            
    return color;
}

gfxColor gfxPixelBufferGetIndex(const gfxPixelBuffer* const buffer,
                              const uint32_t idx)
{
    if(idx >= buffer->pixel_count)
        return 0;
        
    return gfxIndexGet_FnTable[gfxColorInfoTable[buffer->mode].bppOrdinal](buffer, idx);
}

gfxColor gfxPixelBufferGetIndex_Unsafe(const gfxPixelBuffer* const buffer,
                                     const uint32_t idx)
{       
    return gfxIndexGet_FnTable[gfxColorInfoTable[buffer->mode].bppOrdinal](buffer, idx);
}

gfxBool gfxPixelBuffer_IsLocked(const gfxPixelBuffer* const buffer)
{
    if(buffer == NULL)
        return GFX_FALSE;

    return (buffer->flags & GFX_BF_LOCKED) > 0;
}

gfxResult gfxPixelBuffer_SetLocked(gfxPixelBuffer* buffer,
                                 gfxBool locked)
{
    if(buffer == NULL)
        return GFX_FAILURE;

    if(locked == GFX_TRUE)
    {
        buffer->flags |= GFX_BF_LOCKED;
    }
    else
    {
        buffer->flags &= ~(GFX_BF_LOCKED);
    }

    return GFX_SUCCESS;
}

gfxColor gfxColorConvert(gfxColorMode mode_in,
                           gfxColorMode mode_out,
                           gfxColor color)
{
    return gfx_color_convert_table[mode_in][mode_out](color);
}
