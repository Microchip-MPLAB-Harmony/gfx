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
  MPLAB Harmony Nano2D Generated HAL Wrapper

  File Name:
    libnano2D_hal.c

  Summary:
    Build-time generated implementation for the Nano2D Generated HAL Wrapper.

  Description:
    Build-time generated implementation for the Nano2D Generated HAL Wrapper.

    Created with MPLAB Harmony Version 2.03
*******************************************************************************/

#include "gfx/hal/inc/gfx_common.h"
#include "gfx/hal/inc/gfx_processor_interface.h"
#include "gfx/driver/processor/gfx2d/drv_gfx2d.h"

const char* driverName = "GFX 2D";

/*


#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 480
extern uint32_t  __attribute__ ((aligned (64))) blitbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT] ;
extern uint32_t  __attribute__ ((aligned (64))) maskbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT] ;
*/

GFX2D_PIXEL_FORMAT p2dFormats[GFX_COLOR_MODE_COUNT] =
{
    -1, // GFX_COLOR_MODE_GS_8
    -1, // GFX_COLOR_MODE_RGB_332,
    GFX2D_RGB16, // GFX_COLOR_MODE_RGB_565
    -1, // GFX_COLOR_MODE_RGBA_5551
    -1, // GFX_COLOR_MODE_RGB_888
    GFX2D_ARGB32, // GFX_COLOR_MODE_RGBA_8888
    -1, // GFX_COLOR_MODE_ARGB_8888
    -1, // GFX_COLOR_MODE_YUV
    -1, // GFX_COLOR_MODE_INDEX_1
    -1, //GFX_COLOR_MODE_INDEX_4
    GFX2D_IDX8, // GFX_COLOR_MODE_INDEX_8
};

static GFX_Result fillRect(const GFX_Rect* rect,
                           const GFX_DrawState* state)
{
    struct gpu_buffer    buffer;
    GFX_Rect lrect;

#if GFX_LAYER_CLIPPING_ENABLED || GFX_BOUNDS_CLIPPING_ENABLED
    GFX_Rect clipRect;
#endif

    lrect = *rect;
    
#if GFX_LAYER_CLIPPING_ENABLED
    // clip rect against target rect
    GFX_RectClip(&state->targetClipRect,
                 &lrect,
                 &clipRect);
                 
    lrect = clipRect;        
#endif
    
#if GFX_BOUNDS_CLIPPING_ENABLED
    if((state->clipEnable == GFX_TRUE) && 
        GFX_RectIntersects(&state->clipRect, &lrect) == GFX_FALSE)
        return GFX_FAILURE;
        
    // clip rect against global clipping rect
    if(state->clipEnable == GFX_TRUE)
    {
        GFX_RectClip(&state->clipRect,
                     &lrect,
                     &clipRect);
                     
        lrect = clipRect; 
    }
#endif   

    buffer.width = state->target->size.width;
    buffer.height = state->target->size.height;
    buffer.format = p2dFormats[state->target->mode];
    buffer.dir = GFX2D_XY01;
    buffer.addr = (uint32_t)state->target->pixels;
    
    DRV_GFX2D_Fill(&buffer,
             (GFX2D_RECTANGLE*)&lrect,
             GFX_ColorConvert(state->colorMode, GFX_COLOR_MODE_RGBA_8888, state->color));
    
    return GFX_SUCCESS;
}

static GFX_Result drawBlit(const GFX_PixelBuffer* source,
                           const GFX_Rect* srcRect,
                           const GFX_Point* drawPoint,
                           const GFX_DrawState* state)
{
    struct gpu_buffer src_buffer, dest_buffer;
    GFX_Rect dest_rect;
    GFX2D_TRANSFER_DIRECTION orientation = GFX2D_XY00;
    GFX_Context* context = GFX_ActiveContext();
    
    switch(context->orientation)
    {
        case GFX_ORIENTATION_0:
        {
            orientation = GFX2D_XY00;
            break;
        }
        case GFX_ORIENTATION_90:
        {
            orientation = GFX2D_XY01;
            break;
        }
        case GFX_ORIENTATION_180:
        {
            orientation = GFX2D_XY10;
            break;
        }
        case GFX_ORIENTATION_270:
        {
            orientation = GFX2D_XY11;
            break;
        }
        default:
        {
            orientation = GFX2D_XY00;
            break;
        }
    }

    // craft source buffer descriptor
    src_buffer.width = source->size.width;
    src_buffer.height = source->size.height;
    src_buffer.format = p2dFormats[source->mode];
    src_buffer.dir = orientation;
    src_buffer.addr = (uint32_t)source->pixels;
       
    // craft dest buffer descriptor
    dest_buffer.width = state->target->size.width;
    dest_buffer.height = state->target->size.height;
    dest_buffer.format = p2dFormats[state->target->mode];
    dest_buffer.dir = orientation;
    dest_buffer.addr = (uint32_t)state->target->pixels;
    
    dest_rect.x = drawPoint->x;
    dest_rect.y = drawPoint->y;
    dest_rect.width = srcRect->width;
    dest_rect.height = srcRect->height;
    
    /*
    GFX2D_BUFFER target;
    if(state->maskEnable == GFX_TRUE)
    {
       GFX2D_ROP rop;
        
       rop.high = 0xA;
       rop.low = 0xC;
       rop.mode = GFX2D_ROP4;
       
       target.addr = (uint32_t)blitbuffer;
       target.width = dest_buffer.width;
       target.height = dest_buffer.height;
       target.format = dest_buffer.format;
       
       mask_buffer.addr = (uint32_t)maskbuffer;
        
       DRV_GFX2D_Rop(&target, 
                       (GFX2D_RECTANGLE*)&dest_rect, 
                       &dest_buffer, 
                       (GFX2D_RECTANGLE*)&dest_rect, 
                       &src_buffer, 
                       (GFX2D_RECTANGLE*)&dest_rect,
                       &mask_buffer,
                       rop); 
       src_buffer.addr =  target.addr;
    }    

    if ((state->alphaEnable == GFX_TRUE) && 
        ((state->blendMode & GFX_BLEND_CHANNEL) == 0))
    {
        GFX2D_BLEND blend;
        
        blend.spe=0;
        blend.func=GFX2D_FUNCTION_ADD;
        blend.dfact=GFX2D_FACTOR_ONE_MINUS_SRC_ALPHA;
        blend.sfact=GFX2D_FACTOR_ONE;

        DRV_GFX2D_Blend(&target, (GFX2D_RECTANGLE*)&dest_rect, &dest_buffer, 
            (GFX2D_RECTANGLE*)&dest_rect, &src_buffer, (GFX2D_RECTANGLE*)&dest_rect,
             blend);
        src_buffer.addr =  target.addr;
    }
    */

    DRV_GFX2D_Copy(&dest_buffer,
             (GFX2D_RECTANGLE*)&dest_rect,
             &src_buffer,
             (GFX2D_RECTANGLE*)srcRect);
    
    return GFX_SUCCESS;
}

/*
static GFX_Result drawStretchBlit(const GFX_PixelBuffer* source,
                                  const GFX_Rect* srcRect,
                                  const GFX_Rect* destRect,
                                  const GFX_DrawState* state)
{
    return GFX_SUCCESS;
}
*/

// function that returns the information for this driver
GFX_Result procGFX2DInfoGet(GFX_ProcessorInfo* info)
{
	if(info == NULL)
        return GFX_FAILURE;

	// populate info struct
    strcpy(info->name, driverName);
    info->color_formats = GFX_COLOR_MASK_RGBA_8888;
    
    return GFX_SUCCESS;
}

// function that initialized the driver context
GFX_Result procGFX2DContextInitialize(GFX_Context* context)
{
    /* initialize gpu hardware and peripheral software */
    DRV_GFX2D_Initialize();

	// GPU Pipeline
        //context->hal.drawPipeline[GFX_PIPELINE_GPU].drawLine[GFX_DRAW_LINE][GFX_ANTIALIAS_OFF] = &drawLine;
        //context->hal.drawPipeline[GFX_PIPELINE_GPU].drawLine[GFX_DRAW_FILL][GFX_ANTIALIAS_OFF] = &drawLine;
	
	context->hal.drawPipeline[GFX_PIPELINE_GPU].drawRect[GFX_DRAW_FILL][GFX_ANTIALIAS_OFF] = &fillRect;
	
        context->hal.drawPipeline[GFX_PIPELINE_GPU].drawBlit = &drawBlit;
		
    // according to nano2d documentation the stretch blit is a fast resize, override nearest neighbor version
    //context->hal.drawPipeline[GFX_PIPELINE_GPU].drawStretchBlit[GFX_RESIZE_NEARESTNEIGHBOR] = &drawStretchBlit;
		
	// GCU/GPU Pipeline
        //context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].drawLine[GFX_DRAW_LINE][GFX_ANTIALIAS_OFF] = &drawLine;
        //context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].drawLine[GFX_DRAW_FILL][GFX_ANTIALIAS_OFF] = &drawLine;
	
	context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].drawRect[GFX_DRAW_FILL][GFX_ANTIALIAS_OFF] = &fillRect;
	
        context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].drawBlit = &drawBlit;
	
    // according to nano2d documentation the stretch blit is a fast resize, override nearest neighbor version
    //context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].drawStretchBlit[GFX_RESIZE_NEARESTNEIGHBOR] = &drawStretchBlit;
	
	return GFX_SUCCESS;
}
