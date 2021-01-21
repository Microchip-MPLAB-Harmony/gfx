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
  MPLAB Harmony Generated Driver Implementation File

  File Name:
    drv_gfx_generic.c

  Summary:
    Build-time generated driver.

  Description:
    This is a hardware-agnostic generated driver.  It can be used as a starting
	point to create a hardware specific driver.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

#include "gfx/driver/controller/generic/drv_gfx_generic.h"

#include <xc.h>
#include <sys/attribs.h>

// include any peripheral header files

// define any custom values
#define LAYER_COUNT     1

uint32_t state;

#define DISPLAY_WIDTH   ${gfx_hal.DisplayWidth}
#define DISPLAY_HEIGHT  ${gfx_hal.DisplayHeight}

const char* DRIVER_NAME = "Generic";

<#if UseStaticBuffer>
<#if gfx_hal.ColorModeHint = "GFX_COLOR_MODE_GS_8">
<#assign COLOR_MASK="GFX_COLOR_MASK_GS_8">
<#assign BPP="1">
<#elseif gfx_hal.ColorModeHint = "GFX_COLOR_MODE_RGB_332">
<#assign COLOR_MASK="GFX_COLOR_MASK_RGB_332">
<#assign BPP="1">
<#elseif gfx_hal.ColorModeHint = "GFX_COLOR_MODE_RGB_565">
<#assign COLOR_MASK="GFX_COLOR_MASK_RGB_565">
<#assign BPP="2">
<#elseif gfx_hal.ColorModeHint = "GFX_COLOR_MODE_RGBA_5551">
<#assign COLOR_MASK="GFX_COLOR_MASK_RGBA_5551">
<#assign BPP="2">
<#elseif gfx_hal.ColorModeHint = "GFX_COLOR_MODE_RGB_888">
<#assign COLOR_MASK="GFX_COLOR_MASK_RGB_888">
<#assign BPP="3">
<#elseif gfx_hal.ColorModeHint = "GFX_COLOR_MODE_RGBA_8888">
<#assign COLOR_MASK="GFX_COLOR_MASK_RGBA_8888">
<#assign BPP="4">
<#elseif gfx_hal.ColorModeHint = "GFX_COLOR_MODE_ARGB_8888">
<#assign COLOR_MASK="GFX_COLOR_MASK_ARGB_8888">
<#assign BPP="4">
</#if>
static uint32_t supported_color_formats = ${COLOR_MASK};

uint8_t __attribute__((coherent, aligned(32))) frameBuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT * ${BPP}];
<#else>
static uint32_t supported_color_formats = GFX_COLOR_MASK_ALL;
</#if>

/**** Hardware Abstraction Interfaces ****/
enum
{
    INIT = 0,
    RUN
};

// function that returns the information for this driver
GFX_Result driverGenericInfoGet(GFX_DriverInfo* info)
{
	if(info == NULL)
        return GFX_FAILURE;

	// populate info struct
    strcpy(info->name, DRIVER_NAME);
    info->color_formats = supported_color_formats;
    info->layer_count = LAYER_COUNT;
    
    return GFX_SUCCESS;
}

static GFX_Result genericUpdate()
{
    GFX_Context* context = GFX_ActiveContext();
   
    if(context == NULL)
        return GFX_FAILURE;
    
    if(state == INIT)
    {
        // perform driver initialization here
        
        state = RUN;
    }
    
    return GFX_SUCCESS;
}

static void genericDestroy(GFX_Context* context)
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

<#if UseStaticBuffer>
static GFX_Result layerBufferCountSet(uint32_t count)
{
    count = count;
        
    return GFX_FAILURE;
}

static GFX_Result layerBufferAddressSet(uint32_t idx, GFX_Buffer address)
{
    idx = 0;
    address = address;
    
    return GFX_FAILURE;
}

static GFX_Result layerBufferAllocate(uint32_t idx)
{
    idx = 0;
    
    return GFX_FAILURE;
}
</#if>

static GFX_Result genericInitialize(GFX_Context* context)
{
<#if UseStaticBuffer>
	int i;

</#if>
	// general default initialization
	if(defInitialize(context) == GFX_FAILURE)
		return GFX_FAILURE;	
	
<#if UseStaticBuffer>
	// driver specific initialization tasks	
	// initialize all layer color modes
    for(i = 0; i < LAYER_COUNT; i++)
    {   
		context->layer.layers[i].buffer_count = 1;
		
		GFX_PixelBufferCreate(DISPLAY_WIDTH,
							  DISPLAY_HEIGHT,
							  ${gfx_hal.ColorModeHint},
							  frameBuffer,
							  &context->layer.layers[i].buffers[0].pb);
	
		// mark buffer state as managed so that it can't be freed
		// by anything but the driver
		context->layer.layers[i].buffers[0].state = GFX_BS_MANAGED;
	}
</#if>
	
	return GFX_SUCCESS;
}

// function that initialized the driver context
GFX_Result driverGenericContextInitialize(GFX_Context* context)
{
	// set driver-specific data initialization function address
	context->hal.initialize = &genericInitialize; 	
    
	// override essential hal functions
	context->hal.destroy = &genericDestroy;
	context->hal.update = &genericUpdate;
	
	// override default HAL functions with driver specific implementations
	// any HAL function can be overridden by a driver.  care must be taken
	// that essential functionality is reimplemented by the new function
	// or that the default implementation is called by the new function
    
<#if UseStaticBuffer>
	// the application should not be allowed to free or modify
	// a static frame buffer, override these functions
	context->hal.layerBufferCountSet = &layerBufferCountSet;
    context->hal.layerBufferAddressSet = &layerBufferAddressSet;
    context->hal.layerBufferAllocate = &layerBufferAllocate;
</#if>
	
	return GFX_SUCCESS;
}

/**** End Hardware Abstraction Interfaces ****/
