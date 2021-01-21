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
/********************************************************************************
  GFX GLCD Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_glcd.c.ftl

  Summary:
    Source code for the GFX GLCD driver static implementation.

  Description:
    This file contains the source code for the static implementation of the
    GFX GLCD driver.
*******************************************************************************/





// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "peripheral/evic/plib_evic.h"
#include "gfx/driver/controller/glcd/plib_glcd.h"
#include "gfx/driver/controller/glcd/drv_gfx_glcd.h"
#include "definitions.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

#define BUFFER_PER_LAYER    1

#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 480
#define GFX_GLCD_LAYERS 1
#define GFX_GLCD_BACKGROUND_COLOR 0xFFFFFF00
#define GFX_GLCD_CONFIG_CONTROL 0x80000000
#define GFX_GLCD_CONFIG_CLK_DIVIDER 10
#define GFX_GLCD_GLOBAL_PALETTE_COLOR_COUNT 256

/*** GLCD Layer 0 Configuration ***/
#define  GFX_GLCD_LAYER0_BASEADDR                      0xA8000000
#define  GFX_GLCD_LAYER0_DBL_BASEADDR                  0xA8465000

#define LCDC_DEFAULT_GFX_COLOR_MODE GLCD_LAYER_COLOR_MODE_RGBA8888
#define FRAMEBUFFER_PTR_TYPE    uint32_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint32_t

typedef enum
{
    LAYER_UNLOCKED,
    LAYER_LOCKED,
    LAYER_LOCKED_PENDING,
} LAYER_LOCK_STATUS;

const char* DRIVER_NAME = "GLCD";
//static uint32_t supported_color_format = (GFX_COLOR_MASK_GS_8 |
//                                          GFX_COLOR_MASK_RGB_565 |
//                                          GFX_COLOR_MASK_RGBA_8888);


static uint32_t state;
static unsigned int vsyncCount = 0;
static gfxPixelBuffer pixelBuffer[GFX_GLCD_LAYERS];

static unsigned int activeLayer = 0;

volatile int32_t waitForAlphaSetting[GFX_GLCD_LAYERS] = {0};

//Layer Parameters
//--------------------------------------------------------------------------
typedef struct __display_layer {
    FRAMEBUFFER_PTR_TYPE  baseaddr[BUFFER_PER_LAYER];
    int        draw;
    int        frame;
    uint32_t   resx;
    uint32_t   resy;
    uint32_t   buscfg;
    uint32_t   format;
    uint32_t   stride;
    uint32_t   startx;
    uint32_t   starty;
    uint32_t   sizex;
    uint32_t   sizey;
    uint32_t   alpha;
    uint32_t   dblend;
    uint32_t   sblend;
    uint32_t   colorspace;
    uint16_t   color;
    bool enabled;
    volatile LAYER_LOCK_STATUS updateLock;
} DISPLAY_LAYER;
static DISPLAY_LAYER drvLayer[GFX_GLCD_LAYERS];


/**** Hardware Abstraction Interfaces ****/
enum
{
    INIT = 0,
    RUN
};

static int DRV_GFX_GLCD_Start();
static gfxResult DRV_GLCD_UpdateLayer(unsigned int layer);

void DRV_GLCD_Update()
{
    if(state == INIT)
    {
        if(DRV_GFX_GLCD_Start() != 0)
            return ;

        state = RUN;
    }
}

static uint32_t getColorModeStrideSize(GLCD_LAYER_COLOR_MODE mode)
{
    switch(mode)
    {
        case GLCD_LAYER_COLOR_MODE_LUT8:
            return sizeof(uint8_t);
            break;
        case GLCD_LAYER_COLOR_MODE_RGB332:
            return sizeof(uint8_t);
            break;
        case GLCD_LAYER_COLOR_MODE_RGB565:
            return sizeof(uint16_t);
            break;
        case GLCD_LAYER_COLOR_MODE_RGB888:
            return sizeof(uint32_t);
            break;
        case GLCD_LAYER_COLOR_MODE_RGBA8888:
            return sizeof(uint32_t);
            break;
        case GLCD_LAYER_COLOR_MODE_ARGB8888:
            return sizeof(uint32_t);
            break;
        case GLCD_LAYER_COLOR_MODE_L1:
            return sizeof(uint8_t);
            break;
        case GLCD_LAYER_COLOR_MODE_L4:
            return sizeof(uint8_t);
            break;
        case GLCD_LAYER_COLOR_MODE_L8:
            return sizeof(uint8_t);
            break;
        default:
            return sizeof(uint32_t);
            break;
    }
}

static GLCD_LAYER_COLOR_MODE getGLCDColorModeFromGFXColorMode(gfxColorMode mode)
{
    switch(mode)
    {
        case GFX_COLOR_MODE_GS_8:
            return GLCD_LAYER_COLOR_MODE_LUT8;
        case GFX_COLOR_MODE_RGB_332:
            return GLCD_LAYER_COLOR_MODE_RGB332;
        case GFX_COLOR_MODE_RGB_565:
            return GLCD_LAYER_COLOR_MODE_RGB565;
        case GFX_COLOR_MODE_RGBA_5551:
            return GLCD_LAYER_COLOR_MODE_RGBA5551;
        case GFX_COLOR_MODE_RGB_888:
            return GLCD_LAYER_COLOR_MODE_RGB888;
        case GFX_COLOR_MODE_ARGB_8888:
            return GLCD_LAYER_COLOR_MODE_ARGB8888;
        case GFX_COLOR_MODE_INDEX_1:
            return GLCD_LAYER_COLOR_MODE_L1;
        case GFX_COLOR_MODE_INDEX_4:
            return GLCD_LAYER_COLOR_MODE_L4;
        case GFX_COLOR_MODE_INDEX_8:
            return GLCD_LAYER_COLOR_MODE_L8;
        case GFX_COLOR_MODE_RGBA_8888:
        default:
            return GLCD_LAYER_COLOR_MODE_RGBA8888;
    }
}

void DRV_GLCD_Initialize()
{
    uint32_t      xResolution;
    uint32_t      yResolution;
    uint32_t      rightMargin;
    uint32_t      lowerMargin;
    uint32_t      hsyncLength;
    uint32_t      vsyncLength;
    uint32_t      leftMargin;
    uint32_t      upperMargin;
    uint32_t      stride;
    uint32_t      layerCount;
    uint32_t      bufferCount;

    // general default initialization
    //if(defInitialize(context) == LE_FAILURE)
    //        return LE_FAILURE;


    /* set temporary information */
    xResolution     = 800;
    yResolution     = 480;
    rightMargin     = 40;
    leftMargin      = 40;
    hsyncLength     = 48;
    vsyncLength     = 10;
    upperMargin     = 29;
    lowerMargin     = 10;


    /* glcd initialization */
    PLIB_GLCD_Disable();
    PLIB_GLCD_BackgroundColorSet(GFX_GLCD_BACKGROUND_COLOR);
    PLIB_GLCD_VSyncInterruptDisable();
    PLIB_GLCD_HSyncInterruptDisable();
    PLIB_GLCD_RGBSequentialModeSet(1<<31);

    PLIB_GLCD_FrontPorchXYSet(xResolution + rightMargin, yResolution + lowerMargin);
    PLIB_GLCD_BlankingXYSet(xResolution + rightMargin + hsyncLength, yResolution + lowerMargin + vsyncLength);
    PLIB_GLCD_BackPorchXYSet(xResolution + rightMargin + hsyncLength + leftMargin, yResolution + lowerMargin + vsyncLength + upperMargin);

    PLIB_GLCD_ClockDividerSet(GFX_GLCD_CONFIG_CLK_DIVIDER);
    PLIB_GLCD_ResolutionXYSet(xResolution, yResolution);

    PLIB_GLCD_SignalPolaritySet( GLCD_VSYNC_POLARITY_NEGATIVE | GLCD_HSYNC_POLARITY_NEGATIVE );

    PLIB_GLCD_PaletteGammaRampDisable();

    PLIB_GLCD_Enable();

    drvLayer[0].baseaddr[0] = (FRAMEBUFFER_PTR_TYPE)GFX_GLCD_LAYER0_BASEADDR;

    for (layerCount = 0; layerCount < GFX_GLCD_LAYERS; layerCount++)
    {
        drvLayer[layerCount].resx       = xResolution;
        drvLayer[layerCount].resy       = yResolution;
        drvLayer[layerCount].startx     = 0;
        drvLayer[layerCount].starty     = 0;
        drvLayer[layerCount].sizex      = drvLayer[layerCount].resx;
        drvLayer[layerCount].sizey      = drvLayer[layerCount].resy;
        drvLayer[layerCount].alpha      = 255;
        drvLayer[layerCount].dblend     = GLCD_LAYER_DEST_BLEND_INV_SRCGBL;
        drvLayer[layerCount].sblend     = GLCD_LAYER_SRC_BLEND_ALPHA_SRCGBL;
        drvLayer[layerCount].colorspace = LCDC_DEFAULT_GFX_COLOR_MODE;
        drvLayer[layerCount].enabled    = true;
        drvLayer[layerCount].updateLock = LAYER_LOCKED;
        //Clear frame buffer
        for(bufferCount = 0; bufferCount < BUFFER_PER_LAYER; ++bufferCount)
        {
            memset(drvLayer[layerCount].baseaddr[bufferCount], 0, sizeof(FRAMEBUFFER_PIXEL_TYPE) * DISPLAY_WIDTH * DISPLAY_HEIGHT);
        }
        stride = getColorModeStrideSize(drvLayer[layerCount].colorspace);

        PLIB_GLCD_LayerBaseAddressSet(layerCount, (uint32_t)drvLayer[layerCount].baseaddr[0]);
        PLIB_GLCD_LayerStrideSet(layerCount, drvLayer[layerCount].resx * stride );
        PLIB_GLCD_LayerResXYSet(layerCount, drvLayer[layerCount].resx, drvLayer[layerCount].resy );
        PLIB_GLCD_LayerStartXYSet(layerCount, drvLayer[layerCount].startx, drvLayer[layerCount].starty );
        PLIB_GLCD_LayerSizeXYSet(layerCount, drvLayer[layerCount].sizex, drvLayer[layerCount].sizey);
        PLIB_GLCD_LayerGlobalAlphaSet(layerCount, drvLayer[layerCount].alpha);
        PLIB_GLCD_LayerDestBlendFuncSet(layerCount, drvLayer[layerCount].dblend );
        PLIB_GLCD_LayerSrcBlendFuncSet(layerCount, drvLayer[layerCount].sblend );
        PLIB_GLCD_LayerColorModeSet(layerCount, drvLayer[layerCount].colorspace );

        PLIB_GLCD_LayerEnable(layerCount);

        gfxPixelBufferCreate(xResolution,
                    yResolution,
                    DRV_GLCD_GetColorMode(),
                    drvLayer[layerCount].baseaddr[0],
                    &pixelBuffer[layerCount]);

        drvLayer[layerCount].updateLock = LAYER_UNLOCKED;
    }

    EVIC_SourceStatusClear(INT_SOURCE_GLCD);
    EVIC_SourceEnable(INT_SOURCE_GLCD);

}

gfxPixelBuffer * DRV_GLCD_GetFrameBuffer(int32_t idx)
{
    return &pixelBuffer[activeLayer];
}

void GLCD_Interrupt_Handler(void)
{
    unsigned int i = 0;
    
    EVIC_SourceStatusClear(INT_SOURCE_GLCD);
    
    //Update GLCD during blanking period
    for (i = 0; i < GFX_GLCD_LAYERS; i++)
    {
        if (drvLayer[i].updateLock == LAYER_LOCKED_PENDING)
        {
            DRV_GLCD_UpdateLayer(i);
            drvLayer[i].updateLock = LAYER_UNLOCKED;
        }
    }

    PLIB_GLCD_VSyncInterruptDisable();
}

/**** End Hardware Abstraction Interfaces ****/


static int DRV_GFX_GLCD_Start()
{
    return 0;
}

gfxColorMode DRV_GLCD_GetColorMode()
{
    return GFX_COLOR_MODE_RGBA_8888;
}

uint32_t DRV_GLCD_GetBufferCount()
{
    return BUFFER_PER_LAYER;
}

uint32_t DRV_GLCD_GetDisplayWidth()
{
    return DISPLAY_WIDTH;
}

uint32_t DRV_GLCD_GetDisplayHeight()
{
    return DISPLAY_HEIGHT;
}

uint32_t DRV_GLCD_GetLayerCount()
{
    return GFX_GLCD_LAYERS;
}

uint32_t DRV_GLCD_GetActiveLayer()
{
    return activeLayer;
}

gfxResult DRV_GLCD_SetActiveLayer(uint32_t idx)
{
    activeLayer = idx;

    return GFX_SUCCESS;
}

gfxLayerState DRV_GLCD_GetLayerState(uint32_t idx)
{
    gfxLayerState layerState;

    layerState.rect.x = drvLayer[idx].startx;
    layerState.rect.y = drvLayer[idx].starty;
    layerState.rect.width = drvLayer[idx].sizex;
    layerState.rect.height = drvLayer[idx].sizey;

    layerState.enabled = drvLayer[idx].enabled;

    return layerState;
}

void DRV_GLCD_Swap(void)
{

}

uint32_t DRV_GLCD_GetVSYNCCount(void)
{
    return vsyncCount;
}

gfxResult DRV_GLCD_BlitBuffer(int32_t x,
                             int32_t y,
                             gfxPixelBuffer* buf)
{
    void* srcPtr;
    void* destPtr;
    uint32_t row, rowSize;

    if (state != RUN)
        return GFX_FAILURE;

    rowSize = buf->size.width * gfxColorInfoTable[buf->mode].size;

    for(row = 0; row < buf->size.height; row++)
    {
        srcPtr = gfxPixelBufferOffsetGet(buf, 0, row);
        destPtr = gfxPixelBufferOffsetGet(&pixelBuffer[activeLayer], x, y + row);

        memcpy(destPtr, srcPtr, rowSize);
    }

    return GFX_SUCCESS;
}

static gfxResult DRV_GLCD_UpdateLayer(unsigned int layer)
{
    PLIB_GLCD_LayerStrideSet(layer,
                             drvLayer[layer].resx *
                             getColorModeStrideSize(drvLayer[layer].colorspace));
        
    PLIB_GLCD_LayerBaseAddressSet(layer,
                                 (uint32_t)drvLayer[layer].baseaddr[0]);
                
    PLIB_GLCD_LayerStartXYSet(layer,
                              drvLayer[layer].startx,
                              drvLayer[layer].starty );
        
        //Set layer size
    PLIB_GLCD_LayerSizeXYSet(layer,
                             drvLayer[layer].sizex,
                             drvLayer[layer].sizey);
            
    PLIB_GLCD_LayerResXYSet(layer,
                            drvLayer[layer].sizex,
                            drvLayer[layer].sizey);
            
    PLIB_GLCD_LayerGlobalAlphaSet(layer,
                            drvLayer[layer].alpha);
        
    PLIB_GLCD_LayerColorModeSet(layer,
                            drvLayer[layer].colorspace);   
    
    if (drvLayer[layer].enabled == true)
        PLIB_GLCD_LayerEnable(layer);
    else
        PLIB_GLCD_LayerDisable(layer);
    
    return GFX_SUCCESS;
}

static gfxResult DRV_GLCD_LayerConfig(ctlrCfg request, unsigned int layer, void * arg)
{
    //Make sure layer is locked before accepting changes
    if (layer >= GFX_GLCD_LAYERS)
        return GFX_FAILURE;
    
    //Attempt to lock
    if (request == GFX_CTRLR_SET_LAYER_LOCK)
    {
        PLIB_GLCD_VSyncInterruptDisable();
        drvLayer[layer].updateLock = LAYER_LOCKED;
        return GFX_SUCCESS;
    }
    
    //Layer should be locked 
    if (drvLayer[layer].updateLock != LAYER_LOCKED)
        return GFX_FAILURE;
    
    if (request == GFX_CTRLR_SET_LAYER_UNLOCK)
    {
        drvLayer[layer].updateLock = LAYER_LOCKED_PENDING;
        PLIB_GLCD_VSyncInterruptEnable();
        
        return GFX_SUCCESS;
    }
    
    switch(request)
    {
        case GFX_CTRLR_SET_LAYER_SIZE:
        {
            drvLayer[layer].resx = ((argSetSize *) arg)->width;
            drvLayer[layer].resy = ((argSetSize *) arg)->height;
            
            break;
        }
        case GFX_CTRLR_SET_LAYER_ALPHA:
        {
            drvLayer[layer].alpha = ((argSetValue *) arg)->value;
            break;
        }
        case GFX_CTRLR_SET_LAYER_WINDOW_SIZE:
        {
            drvLayer[layer].sizex = ((argSetSize *) arg)->width;
            drvLayer[layer].sizey = ((argSetSize *) arg)->height;
            break;
        }
        case GFX_CTRLR_SET_LAYER_WINDOW_POSITION:
        {
            drvLayer[layer].startx = ((argSetPosition*) arg)->xpos;
            drvLayer[layer].starty = ((argSetPosition*) arg)->ypos;
            
            break;
        }
        case GFX_CTRLR_SET_LAYER_BASE_ADDRESS:
        {
            drvLayer[layer].baseaddr[0] = 
                    (FRAMEBUFFER_PTR_TYPE) ((argSetValue *) arg)->value;
            break;
        }
        case GFX_CTRLR_SET_LAYER_COLOR_MODE:
        {
            drvLayer[layer].colorspace = 
                    getGLCDColorModeFromGFXColorMode(((argSetValue *) arg)->value);
            
            break;
        }
        case GFX_CTRLR_SET_LAYER_ENABLE:
        {
            drvLayer[layer].enabled = true;
            break;
        }
        case GFX_CTRLR_SET_LAYER_DISABLE:
        {
            drvLayer[layer].enabled = false;
            break;
        }
        default:
            break;
    }
    
    return GFX_SUCCESS;
}

gfxResult DRV_GLCD_SetPalette(gfxBuffer* palette,
                              gfxColorMode mode,
                              uint32_t colorCount)
{
    return GFX_FAILURE;
}

gfxResult DRV_GLCD_CtrlrConfig(ctlrCfg request, void * arg)
{
    if (request >= GFX_CTRLR_LAYER_START && 
        request < GFX_CTRLR_LAYER_END)
    {
        return DRV_GLCD_LayerConfig(request, (unsigned int) *((uint32_t *) arg), arg);
    }
    
    return GFX_SUCCESS;
}

/*******************************************************************************
 End of File
*/
