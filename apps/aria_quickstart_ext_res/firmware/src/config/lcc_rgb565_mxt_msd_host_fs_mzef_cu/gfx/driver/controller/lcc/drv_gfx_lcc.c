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

/*******************************************************************************
  MPLAB Harmony LCC Generated Driver Implementation File

  File Name:
    drv_gfx_lcc_generic.c

  Summary:
    Build-time generated implementation for the LCC Driver for PIC32C MCUs.

  Description:
    Build-time generated implementation for the LCC Driver for PIC32C MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/




#include "gfx/driver/controller/lcc/drv_gfx_lcc.h"
#include "definitions.h"

#define MAX_LAYER_COUNT 1
#define BUFFER_COUNT    1
#define DISPLAY_WIDTH   480
#define DISPLAY_HEIGHT  272

#define EBI_CS_INDEX  0

#define EBI_BASE_ADDR __KSEG2_EBI_DATA_MEM_BASE


#define FRAMEBUFFER_COLOR_MODE GFX_COLOR_MODE_GS_8
#define FRAMEBUFFER_TYPE uint8_t
#define FRAMEBUFFER_PIXEL_BYTES 2

const char* DRIVER_NAME = "LCC SMC";
static uint32_t supported_color_formats = (GFX_COLOR_MASK_RGB_565 | GFX_COLOR_MASK_RGB_332);

#define FRAMEBUFFER_ATTRIBUTE __attribute__((coherent, aligned(FRAMEBUFFER_PIXEL_BYTES*8)))

FRAMEBUFFER_TYPE frameBuffer[BUFFER_COUNT][DISPLAY_WIDTH * DISPLAY_HEIGHT];

uint16_t FRAMEBUFFER_ATTRIBUTE frameLine[DISPLAY_WIDTH];

#define DRV_GFX_LCC_DMA_CHANNEL_INDEX DMAC_CHANNEL_0
#define DRV_GFX_DMA_EVENT_TYPE DMAC_TRANSFER_EVENT

#ifndef GFX_DISP_INTF_PIN_RESET_Set
#error "GFX_DISP_INTF_PIN_RESET GPIO must be defined in the Pin Settings"
#endif


#ifndef GFX_DISP_INTF_PIN_BACKLIGHT_Set
#warning "GFX_DISP_INTF_PIN_BACKLIGHT GPIO must be defined in the Pin Settings"
#define GFX_DISP_INTF_PIN_BACKLIGHT_Set()
#endif

#ifndef GFX_DISP_INTF_PIN_VSYNC_Set
#error "GFX_DISP_INTF_PIN_VSYNC GPIO must be defined in the Pin Settings"
#endif

#ifndef GFX_DISP_INTF_PIN_DE_Set
#error "GFX_DISP_INTF_PIN_DE GPIO must be defined in the Pin Settings"
#endif

#ifndef GFX_DISP_INTF_PIN_HSYNC_Set
#error "GFX_DISP_INTF_PIN_HSYNC GPIO must be defined in the Pin Settings"
#endif

/**** Hardware Abstraction Interfaces ****/
enum
{
    INIT = 0,
    RUN
};

static int DRV_GFX_LCC_Start();
static void DRV_GFX_LCC_DisplayRefresh(void);
void dmaIntHandler (DRV_GFX_DMA_EVENT_TYPE status,
                    uintptr_t contextHandle);

GFX_Context* cntxt;

uint16_t HBackPorch;
uint32_t VER_BLANK;

uint32_t DISP_HOR_FRONT_PORCH;
uint32_t DISP_HOR_RESOLUTION;
uint32_t DISP_HOR_BACK_PORCH;
uint32_t DISP_HOR_PULSE_WIDTH;

uint32_t DISP_VER_FRONT_PORCH;
uint32_t DISP_VER_RESOLUTION;
uint32_t DISP_VER_BACK_PORCH;
uint32_t DISP_VER_PULSE_WIDTH;

int16_t line = 0;
uint32_t offset = 0;
uint16_t pixels = 0;
uint32_t hSyncs = 0;
    
uint32_t vsyncPeriod = 0;
uint32_t vsyncPulseDown = 0;
uint32_t vsyncPulseUp = 0;
uint32_t vsyncEnd = 0;


// function that returns the information for this driver
GFX_Result driverLCCInfoGet(GFX_DriverInfo* info)
{
    if(info == NULL)
        return GFX_FAILURE;

    // populate info struct
    strcpy(info->name, DRIVER_NAME);
    info->color_formats = supported_color_formats;
    info->layer_count = MAX_LAYER_COUNT;
    
    return GFX_SUCCESS;
}

static GFX_Result lccUpdate()
{
    static uint32_t state;
    GFX_Context* context = GFX_ActiveContext();
   
    if(context == NULL)
        return GFX_FAILURE;
    
    if(state == INIT)
    {
        if(DRV_GFX_LCC_Start() != 0)
            return GFX_FAILURE;
        
        state = RUN;
    }
    
    return GFX_SUCCESS;
}

static void lccDestroy(GFX_Context* context)
{    
    // driver specific shutdown tasks
    if(context->driver_data != GFX_NULL)
    {
        context->memory.free(context->driver_data);
        context->driver_data = GFX_NULL;
    }
    
    // general default shutdown
    defDestroy(context);
}

static GFX_Result layerBufferCountSet(uint32_t count)
{
    count = count;
        
    return GFX_SUCCESS;
}

static GFX_Result layerBufferAddressSet(uint32_t idx, GFX_Buffer address)
{
    idx = 0;
    address = address;
    
    return GFX_SUCCESS;
}

static GFX_Result layerBufferAllocate(uint32_t idx)
{
    idx = 0;
    
    return GFX_SUCCESS;
}


static GFX_Result lccBacklightBrightnessSet(uint32_t brightness)
{
    if (brightness == 0)
    {
        GFX_DISP_INTF_PIN_BACKLIGHT_Clear();
    }
    else
    {
        GFX_DISP_INTF_PIN_BACKLIGHT_Set();
    }

    return GFX_SUCCESS;

}

static GFX_Result lccInitialize(GFX_Context* context)
{
    uint32_t i, j;
    
    cntxt = context;
    
    // general default initialization
    if(defInitialize(context) == GFX_FAILURE)
        return GFX_FAILURE;    
        
    // override default HAL functions with LCC specific implementations
    context->hal.update = &lccUpdate;
    context->hal.destroy = &lccDestroy;
    context->hal.layerBufferCountSet = &layerBufferCountSet;
    context->hal.layerBufferAddressSet = &layerBufferAddressSet;
    context->hal.layerBufferAllocate = &layerBufferAllocate;
    context->hal.brightnessSet = &lccBacklightBrightnessSet;
    
    // driver specific initialization tasks    
    // initialize all layer color modes
    for(i = 0; i < MAX_LAYER_COUNT; i++)
    {
        context->layer.layers[i].buffer_count = BUFFER_COUNT;

        for(j = 0; j < BUFFER_COUNT; j++)
        {
            GFX_PixelBufferCreate(DISPLAY_WIDTH,
                                  DISPLAY_HEIGHT,
                                  FRAMEBUFFER_COLOR_MODE,
                                  frameBuffer[j],
                                  &context->layer.layers[i].buffers[j].pb);
            
            context->layer.layers[i].buffers[j].state = GFX_BS_MANAGED;
        }
    }
    
    VER_BLANK = context->display_info->attributes.vert.pulse_width +
                context->display_info->attributes.vert.back_porch +
                context->display_info->attributes.vert.front_porch - 1;
    
    HBackPorch = context->display_info->attributes.horz.pulse_width +
                 context->display_info->attributes.horz.back_porch;
    
    DISP_HOR_FRONT_PORCH = context->display_info->attributes.horz.front_porch;
    DISP_HOR_RESOLUTION = DISPLAY_WIDTH;
    DISP_HOR_BACK_PORCH = context->display_info->attributes.horz.back_porch;
    DISP_HOR_PULSE_WIDTH = context->display_info->attributes.horz.pulse_width;
    
    DISP_VER_FRONT_PORCH = context->display_info->attributes.vert.front_porch;
    DISP_VER_RESOLUTION = DISPLAY_HEIGHT;
    DISP_VER_BACK_PORCH = context->display_info->attributes.vert.back_porch;
    DISP_VER_PULSE_WIDTH = context->display_info->attributes.vert.pulse_width;

    vsyncPeriod = DISP_VER_FRONT_PORCH + DISP_VER_RESOLUTION + DISP_VER_BACK_PORCH;  

    GFX_DISP_INTF_PIN_RESET_Set();

    /*Turn Backlight on*/

    lccBacklightBrightnessSet(100);

    return GFX_SUCCESS;
}

// function that initialized the driver context
GFX_Result driverLCCContextInitialize(GFX_Context* context)
{
    // set driver-specific data initialization function address
    context->hal.initialize = &lccInitialize; 
    
    // set driver-specific destroy function address
    context->hal.destroy = &lccDestroy;
    
    return GFX_SUCCESS;
}

/**** End Hardware Abstraction Interfaces ****/

static void lccDMAStartTransfer(const void *srcAddr, size_t srcSize,
                                       const void *destAddr)
{
    DMAC_ChannelTransfer(DRV_GFX_LCC_DMA_CHANNEL_INDEX,
                         srcAddr,
                         srcSize,
                         destAddr,
                         FRAMEBUFFER_PIXEL_BYTES,
                         srcSize);
}

static int DRV_GFX_LCC_Start()
{
    DMAC_ChannelCallbackRegister(DRV_GFX_LCC_DMA_CHANNEL_INDEX, dmaIntHandler, 0);
    
    lccDMAStartTransfer(frameBuffer, 
                        FRAMEBUFFER_PIXEL_BYTES, 
                        (const void*) EBI_BASE_ADDR);

    return 0;
}

static void DRV_GFX_LCC_DisplayRefresh(void)
{
    GFX_Point drawPoint;
    GFX_PixelBuffer* buffer;
    uint8_t * bufferPtr;
    uint16_t* palette;
    uint32_t i;

    typedef enum
    {
        HSYNC_FRONT_PORCH,
        HSYNC_PULSE,
        HSYNC_BACK_PORCH,
        HSYNC_DATA_ENABLE,
        HSYNC_DATA_ENABLE_OVERFLOW        
    } HSYNC_STATES;

    typedef enum
    {
        VSYNC_FRONT_PORCH,
        VSYNC_PULSE,
        VSYNC_BACK_PORCH,
        VSYNC_BLANK        
    } VSYNC_STATES;

    static HSYNC_STATES hsyncState = HSYNC_FRONT_PORCH;
    static VSYNC_STATES vsyncState = VSYNC_BLANK;

    switch(vsyncState)
    {
        case VSYNC_FRONT_PORCH:
        {
            if (hSyncs > vsyncPulseDown)
            {
                GFX_DISP_INTF_PIN_VSYNC_Set();

                vsyncPulseUp = hSyncs + DISP_VER_PULSE_WIDTH;
                vsyncState = VSYNC_PULSE;

                if(cntxt->layer.active->vsync == GFX_TRUE
                    && cntxt->layer.active->swap == GFX_TRUE)
                {
                    GFX_LayerSwap(cntxt->layer.active);
                }

                line = 0;
            }
            
            break;
        }
        case VSYNC_PULSE:
        {
            if (hSyncs >= vsyncPulseUp)
            {
                GFX_DISP_INTF_PIN_VSYNC_Clear();
                vsyncEnd = hSyncs + DISP_VER_BACK_PORCH;
                vsyncState = VSYNC_BACK_PORCH;
            }
            
            break;
        }
        case VSYNC_BACK_PORCH:
        {
            if (hSyncs >= vsyncEnd)
                vsyncState = VSYNC_BLANK;
            
            break;
        }
        case VSYNC_BLANK:
        {
            break;
        }
    }

    switch (hsyncState)
    {
        case HSYNC_FRONT_PORCH:
        {
            GFX_DISP_INTF_PIN_DE_Clear();

            hsyncState = HSYNC_PULSE;

            if (DISP_HOR_FRONT_PORCH > 0)
            {
                pixels = DISP_HOR_FRONT_PORCH;
                break;
            }
        }
        case HSYNC_PULSE:
        {
            GFX_DISP_INTF_PIN_HSYNC_Set();

            if (hSyncs >= vsyncPeriod)
            {
                vsyncPeriod = hSyncs + DISP_VER_PULSE_WIDTH + DISP_VER_FRONT_PORCH + DISP_VER_RESOLUTION + DISP_VER_BACK_PORCH;
                vsyncPulseDown = hSyncs + DISP_VER_FRONT_PORCH;
                vsyncState = VSYNC_FRONT_PORCH;
            }

            hSyncs++; 

            pixels = DISP_HOR_PULSE_WIDTH;
            hsyncState = HSYNC_BACK_PORCH;  

            break;
        }
        case HSYNC_BACK_PORCH:
        {
            GFX_DISP_INTF_PIN_HSYNC_Clear();

            hsyncState = HSYNC_DATA_ENABLE; 

            if (DISP_HOR_BACK_PORCH > 0)
            {
                pixels = DISP_HOR_BACK_PORCH;
                break;
            }
        }
        case HSYNC_DATA_ENABLE:
        {
            if (vsyncState == VSYNC_BLANK)
            {
                GFX_DISP_INTF_PIN_DE_Set();
                drawPoint.x = 0;
                drawPoint.y = line++;

                buffer = &cntxt->layer.active->buffers[cntxt->layer.active->buffer_read_idx].pb;

                bufferPtr = GFX_PixelBufferOffsetGet_Unsafe(buffer, &drawPoint);
                
                palette = (uint16_t*)GFX_ActiveContext()->globalPalette;
                
                for(i = 0; i < DISPLAY_WIDTH; i++)
                    frameLine[i] = palette[bufferPtr[i]];

            }

            pixels = DISP_HOR_RESOLUTION;
            hsyncState = HSYNC_FRONT_PORCH;

            break;
        }
        case HSYNC_DATA_ENABLE_OVERFLOW:
        {
            hsyncState = HSYNC_FRONT_PORCH;
            
            break;
        }
    }

    lccDMAStartTransfer(frameLine,
                        (pixels * 2), //2 bytes per pixel
                        (uint32_t*) EBI_BASE_ADDR);
}

void dmaIntHandler (DRV_GFX_DMA_EVENT_TYPE status,
                    uintptr_t contextHandle)
{
    DRV_GFX_LCC_DisplayRefresh();
}
