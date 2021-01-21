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

/*******************************************************************************
  MPLAB Harmony LCDC Generated Driver Implementation File

  File Name:
    drv_gfx_lcdc.c

  Summary:
    Build-time generated implementation for the LCDC Driver for PIC32C MPU/MCUs.

  Description:
    Build-time generated implementation for the LCDC Driver for PIC32C MPU/MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/





#include "gfx/driver/controller/lcdc/drv_gfx_lcdc.h"
#include "definitions.h"

#define BUFFER_PER_LAYER    1

#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 480

#define PIXEL_CLOCK_DIV 7
#define LCDC_OUTPUT_COLOR_MODE LCDC_OUTPUT_COLOR_MODE_24BPP
#define LCDC_DISPLAY_GUARD_NUM_FRAMES 30
#define LCDC_SYNC_EDGE LCDC_SYNC_EDGE_FIRST
#define LCDC_PWM_POLARITY LCDC_POLARITY_POSITIVE
#define GFX_LCDC_BACKGROUND_COLOR 0xffffffff
#define GFX_LCDC_LAYERS 3
#define LCDC_DEFAULT_BRIGHTNESS_PCT 100


#define LCDC_DEFAULT_GFX_COLOR_MODE LCDC_INPUT_COLOR_MODE_RGBA_8888
#define FRAMEBUFFER_PTR_TYPE    uint32_t*
#define FRAMEBUFFER_PIXEL_TYPE    uint32_t

typedef enum
{
    LAYER_UNLOCKED,
    LAYER_LOCKED,
    LAYER_LOCKED_PENDING,
} LAYER_LOCK_STATUS;

#define LCDC_VSYNC_POLARITY LCDC_POLARITY_POSITIVE

#define LCDC_HSYNC_POLARITY LCDC_POLARITY_POSITIVE


#define LCDC_PWM_CLOCK_SOURCE LCDC_PWM_SOURCE_SYSTEM
#define LCDC_PWM_PRESCALER 5



typedef struct
{
  uint32_t addr;
  uint32_t ctrl;
  uint32_t next;
} LCDC_DMA_DESC;

//Layer Parameters
//--------------------------------------------------------------------------
typedef struct __display_layer {
    LCDC_LAYER_ID hwLayerID;
    FRAMEBUFFER_PIXEL_TYPE  *baseaddr[BUFFER_PER_LAYER];
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
    uint32_t   colorspace;
    uint16_t   color;
    LCDC_DMA_DESC * desc;
    uint32_t frameOffset;
    bool enabled;
    volatile LAYER_LOCK_STATUS updateLock;
} DISPLAY_LAYER;

//This array defines the z-order of the hw layers in the GLCD
//This must be dynamically configured for the GLCD periperal on the part
static LCDC_LAYER_ID lcdcLayerZOrder[GFX_LCDC_LAYERS] =
{
    LCDC_LAYER_BASE,
    LCDC_LAYER_OVR1,
    LCDC_LAYER_OVR2,
};

FRAMEBUFFER_PIXEL_TYPE  __attribute__ ((section(".region_nocache"), aligned (32))) framebuffer_0[DISPLAY_WIDTH * DISPLAY_HEIGHT];
FRAMEBUFFER_PIXEL_TYPE  __attribute__ ((section(".region_nocache"), aligned (32))) framebuffer_1[DISPLAY_WIDTH * DISPLAY_HEIGHT];
FRAMEBUFFER_PIXEL_TYPE  __attribute__ ((section(".region_nocache"), aligned (32))) framebuffer_2[DISPLAY_WIDTH * DISPLAY_HEIGHT];


LCDC_DMA_DESC __attribute__ ((section(".region_nocache"), aligned (64))) channelDesc0;
LCDC_DMA_DESC __attribute__ ((section(".region_nocache"), aligned (64))) channelDesc1;
LCDC_DMA_DESC __attribute__ ((section(".region_nocache"), aligned (64))) channelDesc2;

const char* DRIVER_NAME = "LCDC";
uint32_t state;

static gfxBool useGPU = false;
static unsigned int vsyncCount = 0;
static gfxPixelBuffer pixelBuffer[GFX_LCDC_LAYERS];
static unsigned int activeLayer = 0;

static DISPLAY_LAYER drvLayer[GFX_LCDC_LAYERS];
static volatile int32_t waitForAlphaSetting[GFX_LCDC_LAYERS] = {0};

/**** Hardware Abstraction Interfaces ****/
enum
{
    INIT = 0,
    RUN
};

static int DRV_GFX_LCDC_Start();
static gfxResult DRV_LCDC_UpdateLayer(unsigned int layer);

void _IntHandlerVSync(uintptr_t context);

static void LCDCUpdateDMADescriptor(LCDC_DMA_DESC * desc, uint32_t addr, uint32_t ctrl, uint32_t next)
{
    desc->addr = addr;
    desc->ctrl = ctrl;
    desc->next = next;
}

void DRV_LCDC_Update()
{
    if(state == INIT)
    {
        if(DRV_GFX_LCDC_Start() != 0)
            return;

        state = RUN;
    }
}

static LCDC_INPUT_COLOR_MODE getLCDCColorModeFromGFXColorMode(gfxColorMode mode)
{
    switch(mode)
    {
        case GFX_COLOR_MODE_GS_8:
            return LCDC_INPUT_COLOR_MODE_LUT8;
            break;
        case GFX_COLOR_MODE_RGB_565:
            return LCDC_INPUT_COLOR_MODE_RGB_565;
            break;
        case GFX_COLOR_MODE_RGB_888:
            return LCDC_INPUT_COLOR_MODE_RGB_888;
            break;
        case GFX_COLOR_MODE_RGBA_8888:
            return LCDC_INPUT_COLOR_MODE_RGBA_8888;
            break;
        case GFX_COLOR_MODE_ARGB_8888:
            return LCDC_INPUT_COLOR_MODE_ARGB_8888;
            break;
        case GFX_COLOR_MODE_INDEX_1:
        case GFX_COLOR_MODE_INDEX_4:
        case GFX_COLOR_MODE_INDEX_8:
        case GFX_COLOR_MODE_RGB_332:
            return LCDC_INPUT_COLOR_MODE_UNSUPPORTED;
            break;
        default:
            return LCDC_INPUT_COLOR_MODE_RGBA_8888;
            break;
    }
}

gfxResult LCDC_BrightnessRangeGet(uint32_t *low, uint32_t *high)
{
    *low = 0;
    *high = 100;

    return GFX_SUCCESS;
}

gfxResult LCDC_BrightnessSet(uint32_t brightness)
{

    LCDC_SetPWMCompareValue(brightness * 0xff / 100);

    return GFX_SUCCESS;
}

gfxResult DRV_LCDC_Initialize()
{
    uint32_t      xResolution;
    uint32_t      yResolution;
    uint32_t      rightMargin;
    uint32_t      lowerMargin;
    uint32_t      hsyncLength;
    uint32_t      vsyncLength;
    uint32_t      leftMargin;
    uint32_t      upperMargin;
    uint32_t      layerCount;
    uint32_t      bufferCount;
    
    //Clear the descriptor and structures
    memset(drvLayer, 0, sizeof(drvLayer));

    /* set temporary information */
    xResolution     = 800;
    yResolution     = 480;
    rightMargin     = 40;
    leftMargin      = 40;
    hsyncLength     = 48;
    vsyncLength     = 10;
    upperMargin     = 29;
    lowerMargin     = 10;


    /* LCDC initialization */
    //1. Configure the LCD timing parameters
    LCDC_WaitForSyncInProgress();
    LCDC_SetPWMClockSourceSelection(LCDC_PWM_CLOCK_SOURCE);
    LCDC_SetClockDivider(PIXEL_CLOCK_DIV);

    //Disable all layers for now
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_BASE, false); 
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_OVR1, false); 
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_OVR2, false);
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_HEO, false);
    LCDC_SetLayerClockGatingDisable(LCDC_LAYER_PP, false);

    LCDC_WaitForSyncInProgress();
    LCDC_SetHSYNCPulseWidth(hsyncLength); //Set the pulse widths
    LCDC_SetVSYNCPulseWidth(vsyncLength);

    LCDC_WaitForSyncInProgress();
    LCDC_SetVerticalFrontPorchWidth(lowerMargin); //Set the vertical porches
    LCDC_SetVerticalBackPorchWidth(upperMargin);
    
    LCDC_WaitForSyncInProgress();
    LCDC_SetHorizontalFrontPorchWidth(rightMargin); //Set the horizontal porches
    LCDC_SetHorizontalBackPorchWidth(leftMargin);
    
    LCDC_WaitForSyncInProgress();
    LCDC_SetNumActiveRows(yResolution);
    LCDC_SetNumPixelsPerLine(xResolution);
    
    LCDC_WaitForSyncInProgress();
    LCDC_SetDisplayGuardTime(LCDC_DISPLAY_GUARD_NUM_FRAMES);
    LCDC_SetOutputMode(LCDC_OUTPUT_COLOR_MODE);
    LCDC_SetDisplaySignalSynchronization(true);
    LCDC_SetVSYNCPulseStart(LCDC_SYNC_EDGE);
    LCDC_SetVSYNCPolarity(LCDC_VSYNC_POLARITY);
    LCDC_SetHSYNCPolarity(LCDC_HSYNC_POLARITY);

    LCDC_WaitForSyncInProgress();
    LCDC_SetPWMCompareValue(LCDC_DEFAULT_BRIGHTNESS_PCT * 0xFF / 100);
    LCDC_SetPWMSignalPolarity(LCDC_PWM_POLARITY);
    LCDC_SetPWMPrescaler(LCDC_PWM_PRESCALER);

    //2. Enable the Pixel Clock
    LCDC_WaitForSyncInProgress();
    LCDC_SetPixelClockEnable(true);
    
    //3. Poll CLKSTS field of the LCDC_LCDSR register to check that the clock is running.
    LCDC_WaitForClockRunning();
    
    // 4. Enable Horizontal and Vertical Synchronization by writing one to the
    // SYNCEN field of the LCDC_LCDEN register.
    LCDC_WaitForSyncInProgress();
    LCDC_SetSYNCEnable(true);
    
    // 5. Check that synchronization is up. */    
    LCDC_WaitForSynchronization();
    
    //6. Enable the display power signal 
    LCDC_WaitForSyncInProgress();
    LCDC_SetDISPSignalEnable(true);
    
    //7. Wait for power signal to be activated
    LCDC_WaitForDISPSignal();
    
    //8. Enable the backlight
    LCDC_WaitForSyncInProgress();
    LCDC_SetPWMEnable(true);

    drvLayer[0].baseaddr[0] = framebuffer_0;
    drvLayer[1].baseaddr[0] = framebuffer_1;
    drvLayer[2].baseaddr[0] = framebuffer_2;
    drvLayer[0].desc = &channelDesc0;
    drvLayer[1].desc = &channelDesc1;
    drvLayer[2].desc = &channelDesc2;
    
    for (layerCount = 0; layerCount < GFX_LCDC_LAYERS; layerCount++)
    {
        drvLayer[layerCount].hwLayerID = lcdcLayerZOrder[layerCount];
        drvLayer[layerCount].resx       = xResolution;
        drvLayer[layerCount].resy       = yResolution;
        drvLayer[layerCount].startx     = 0;
        drvLayer[layerCount].starty     = 0;
        drvLayer[layerCount].sizex      = drvLayer[layerCount].resx;
        drvLayer[layerCount].sizey      = drvLayer[layerCount].resy;
        drvLayer[layerCount].alpha      = 255;
        drvLayer[layerCount].colorspace = LCDC_DEFAULT_GFX_COLOR_MODE;
        drvLayer[layerCount].frameOffset = 0;
        LCDCUpdateDMADescriptor(drvLayer[layerCount].desc, 
                                (uint32_t) drvLayer[layerCount].baseaddr[0],
                                0x01,
                                (uint32_t) drvLayer[layerCount].desc);
        
        //Clear frame buffer
        for(bufferCount = 0; bufferCount < BUFFER_PER_LAYER; ++bufferCount)
        {
            memset(drvLayer[layerCount].baseaddr[bufferCount], 0, sizeof(FRAMEBUFFER_PIXEL_TYPE) * DISPLAY_WIDTH * DISPLAY_HEIGHT);
        }
        
        LCDC_SetLayerClockGatingDisable(drvLayer[layerCount].hwLayerID, false);
        LCDC_SetWindowPosition(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].startx, drvLayer[layerCount].starty);
        LCDC_SetWindowSize(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].resx, drvLayer[layerCount].resy);
        LCDC_SetUseDMAPathEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_SetRGBModeInput(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].colorspace);
        LCDC_SetDMAAddressRegister(drvLayer[layerCount].hwLayerID, drvLayer[layerCount].desc->addr);
        LCDC_SetDMADescriptorNextAddress(drvLayer[layerCount].hwLayerID, (uint32_t) drvLayer[layerCount].desc);
        LCDC_SetDMAHeadPointer(drvLayer[layerCount].hwLayerID, (uint32_t) drvLayer[layerCount].desc);
        LCDC_SetTransferDescriptorFetchEnable(drvLayer[layerCount].hwLayerID, true);

        //Note: Blender APIs don't do anything to the base layer
        LCDC_SetBlenderOverlayLayerEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_SetBlenderDMALayerEnable(drvLayer[layerCount].hwLayerID, true); //Enable blender DMA
        LCDC_SetBlenderLocalAlphaEnable(drvLayer[layerCount].hwLayerID, true); //Use local alpha
        LCDC_SetBlenderIteratedColorEnable(drvLayer[layerCount].hwLayerID, true); //Enable iterated color
        LCDC_SetBlenderUseIteratedColor(drvLayer[layerCount].hwLayerID, true); //Use iterated color        
        LCDC_UpdateOverlayAttributesEnable(drvLayer[layerCount].hwLayerID);
        LCDC_UpdateAttribute(drvLayer[layerCount].hwLayerID); //Apply the attributes

        LCDC_SetChannelEnable(drvLayer[layerCount].hwLayerID, true);
        LCDC_IRQ_Enable(LCDC_INTERRUPT_BASE + drvLayer[layerCount].hwLayerID);

        gfxPixelBufferCreate(xResolution,
                    yResolution,
                    DRV_LCDC_GetColorMode(),
                    drvLayer[layerCount].baseaddr[0],
                    &pixelBuffer[layerCount]);
    }


    //Register the interrupt handler
    LCDC_IRQ_CallbackRegister(_IntHandlerVSync, (uintptr_t) NULL);
    
    return GFX_SUCCESS;
}

void _IntHandlerVSync(uintptr_t context)
{
    uint32_t i, status;
    
    for (i = 0; i < GFX_LCDC_LAYERS; i++)
    {
        LCDC_LAYER_ID layerID = lcdcLayerZOrder[i];
        status = LCDC_LAYER_IRQ_Status(layerID);

        if (status && drvLayer[i].updateLock == LAYER_LOCKED_PENDING)
        {
            LCDC_LAYER_IRQ_Disable(layerID, LCDC_LAYER_INTERRUPT_DMA);

            DRV_LCDC_UpdateLayer(i);
            drvLayer[i].updateLock = LAYER_UNLOCKED;
        }
    }
}

/**** End Hardware Abstraction Interfaces ****/


static int DRV_GFX_LCDC_Start()
{
    return 0;
}

gfxPixelBuffer * DRV_LCDC_GetFrameBuffer(int32_t idx)
{
    return &pixelBuffer[activeLayer];
}

gfxLayerState DRV_LCDC_GetLayerState(uint32_t idx)
{
    gfxLayerState layerState;

    layerState.rect.x = drvLayer[idx].startx;
    layerState.rect.y = drvLayer[idx].starty;
    layerState.rect.width = drvLayer[idx].sizex;
    layerState.rect.height = drvLayer[idx].sizey;

    layerState.enabled = drvLayer[idx].enabled;

    return layerState;
}

gfxColorMode DRV_LCDC_GetColorMode()
{
	return GFX_COLOR_MODE_RGBA_8888;
}

uint32_t DRV_LCDC_GetBufferCount()
{
    return BUFFER_PER_LAYER;
}

uint32_t DRV_LCDC_GetDisplayWidth()
{
    return DISPLAY_WIDTH;
}

uint32_t DRV_LCDC_GetDisplayHeight()
{
    return DISPLAY_HEIGHT;
}

uint32_t DRV_LCDC_GetLayerCount()
{
	return GFX_LCDC_LAYERS;
}

uint32_t DRV_LCDC_GetActiveLayer()
{
	return activeLayer;
}

gfxResult DRV_LCDC_SetActiveLayer(uint32_t idx)
{
    activeLayer = idx;

    return GFX_SUCCESS;
}

void DRV_LCDC_Swap(void)
{

}

uint32_t DRV_LCDC_GetVSYNCCount(void)
{
	return vsyncCount;
}

void DRV_LCDC_SetUseGPU(gfxBool onOff)
{
	useGPU = onOff;
}

gfxResult DRV_LCDC_BlitBuffer(int32_t x,
                             int32_t y,
                             gfxPixelBuffer* buf)
{
    if (state != RUN)
        return GFX_FAILURE;

    {
    	void* srcPtr;
    	void* destPtr;
    	uint32_t row, rowSize;

        rowSize = buf->size.width * gfxColorInfoTable[buf->mode].size;

    	for(row = 0; row < buf->size.height; row++)
    	{
        	srcPtr = gfxPixelBufferOffsetGet(buf, 0, row);
        	destPtr = gfxPixelBufferOffsetGet(&pixelBuffer[activeLayer], x, y + row);

        	memcpy(destPtr, srcPtr, rowSize);
    	}
    }

    return GFX_SUCCESS;
}

static gfxResult DRV_LCDC_UpdateLayer(unsigned int layer)
{
    if (drvLayer[layer].enabled == true)
	    LCDC_SetUseDMAPathEnable(drvLayer[layer].hwLayerID, true);
	else
	    LCDC_SetUseDMAPathEnable(drvLayer[layer].hwLayerID, false);

    //Update the active layer's color mode and stride
    LCDC_SetRGBModeInput(drvLayer[layer].hwLayerID, drvLayer[layer].colorspace);
    LCDC_SetDMAAddressRegister(drvLayer[layer].hwLayerID, drvLayer[layer].desc->addr);
    LCDC_SetDMADescriptorNextAddress(drvLayer[layer].hwLayerID, (uint32_t) drvLayer[layer].desc);
    LCDC_SetDMAHeadPointer(drvLayer[layer].hwLayerID, (uint32_t) drvLayer[layer].desc);
    LCDC_SetTransferDescriptorFetchEnable(drvLayer[layer].hwLayerID, true);
        
    LCDC_UpdateAddToQueue(drvLayer[layer].hwLayerID);

    return GFX_SUCCESS;
}

static gfxResult DRV_LCDC_LayerConfig(ctlrCfg request, unsigned int layer, void * arg)
{
    //Make sure layer is locked before accepting changes
    if (layer >= GFX_LCDC_LAYERS)
        return GFX_FAILURE;
    
    if (request == GFX_CTRLR_SET_LAYER_LOCK &&
        drvLayer[layer].updateLock == LAYER_UNLOCKED)
    {
        drvLayer[layer].updateLock = LAYER_LOCKED;
        
        return GFX_SUCCESS;
    }
    
    //Layer should be locked 
    if (drvLayer[layer].updateLock != LAYER_LOCKED)
        return GFX_FAILURE;
    
    if (request == GFX_CTRLR_SET_LAYER_UNLOCK)
    {
        drvLayer[layer].updateLock = LAYER_LOCKED_PENDING;
        
		LCDC_WaitForSyncInProgress();
		LCDC_SetSYNCEnable(true);
        
        //Wait for the changes to be applied
        while (drvLayer[layer].updateLock == LAYER_LOCKED_PENDING);
        
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
                    getLCDCColorModeFromGFXColorMode(((argSetValue *) arg)->value);
            
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

gfxResult DRV_LCDC_SetPalette(gfxBuffer* palette,
                              gfxColorMode mode,
                              uint32_t colorCount)
{
    return GFX_FAILURE;
}

gfxResult DRV_LCDC_CtrlrConfig(ctlrCfg request, void * arg)
{
    if (request >= GFX_CTRLR_LAYER_START && 
        request < GFX_CTRLR_LAYER_END)
    {
        return DRV_LCDC_LayerConfig(request, (unsigned int) *((uint32_t *) arg), arg);
    }
    
    return GFX_SUCCESS;
}

/*******************************************************************************
 End of File
*/
