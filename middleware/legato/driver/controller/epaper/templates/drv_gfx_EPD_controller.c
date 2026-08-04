/* 
 * @file drv_gfx_EPD_controller.c
 * @brief Top-level Legato interface for EPD-type controllers
 * 
 * @details Handlers for both public and private (protected) 
 *          entry points for EPD functions and utilities.
 */

/*******************************************************************************
* Copyright (C) 2026 Microchip Technology Inc. and its subsidiaries.
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

#include <stdint.h>

#include "definitions.h"

#include "gfx/interface/drv_gfx_disp_intf.h"
#include "./drv_gfx_EPD_controller.h"
#include "./drv_gfx_EPD_controller_local.h"
#include "gfx/driver/gfx_driver.h"

#include "system/time/sys_time.h"
#include <stdint.h>
#include <xc.h>
#define NOP()   _nop()

#define ALENGTH(x)  (sizeof(x)/sizeof(x[0]))

#include "./drv_gfx_EPD_config.h"

/* ************************************************************************** */

/* Singleton */
EPD_driver_state_t drv;

void DRV_EPD_DelayMS(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

/** Toggle the hardware reset signal -- local package
*/
void DRV_EPD_Reset(void)
{
    DRV_EPD_Reset_Assert();
    DRV_EPD_DelayMS(10);
    DRV_EPD_Reset_Deassert();
    DRV_EPD_DelayMS(30);
}

/** Wait while the hardware is busy -- local package
*/
void DRV_EPD_waitBusy(int ms)
{
    while(nBUSY_Get()) // non-blocking wait
    { DRV_EPD_DelayMS(1); }
    
    NOP(); NOP(); NOP();
    if (0 != ms) // hdwe recovery delay
    { DRV_EPD_DelayMS(ms); }
    NOP(); NOP(); NOP();
}


int DRV_writeData(GFX_Disp_Intf intf, uint8_t * bytes, int count)
{
    GFX_Disp_Intf_WriteData(intf,bytes,count);
    return count;
}


int DRV_writeRepeatedData(GFX_Disp_Intf intf, uint8_t * byte, int count)
{
    for (uint32_t x = 0; x < count; x++)
        GFX_Disp_Intf_WriteData(intf,byte,1);
    return 0;
}

int DRV_test_writeRotate(GFX_Disp_Intf intf, uint8_t * byte, int count)
{
    uint16_t rotated = *byte;
    for (uint32_t x = 0; x < count; x++)
    {
        GFX_Disp_Intf_WriteData(intf,(uint8_t*)&rotated,1);
        rotated <<= 1;
        if (0x100 & rotated) rotated |= 1;
        rotated &= 0xFF;
    }
    return count;
}


int DRV_EPD_Initialize(void)
{
    drv.state = INIT;
    return 0;
}


/** Driver-specific implementation of GFX HAL update function.
 *
 * @details On GFX update, this function flushes any pending pixels to the Custom.
 * @return operation status (GFX_SUCCESS or GFX_FAILURE)
 * @note This is one (of 5) primary Legato entry points.
*/
void DRV_EPD_Update(void)
{
    uint32_t openVal;

    /* Verify that the driver has been initialized */
    if(drv.state == INIT)
    {
        openVal = GFX_Disp_Intf_Open();
        drv.port_priv = (void *)openVal;
        if (drv.port_priv == 0)
        {
            drv.state = ERROR;
            return;
        }

        __attribute__((unused))
        GFX_Disp_Intf intf = (GFX_Disp_Intf) drv.port_priv;

        /* Allow the custom driver to initialize
         * and possibly load a look-up table (LUT). */
        EPD_START_CONFIG(&intf);
        EPD_SET_LUT(&intf,0);

        /* Standard operating procedure */
        DRV_EPD_DelayMS(1000);
        NOP(); NOP(); NOP();
        
        /* We are complete; allow Legato to send data. */
        drv.state = RUN;

        /* FIXME: this section should load black or white
         * data if the display requires that type of initialization. */
#if DRV_EPD_LOAD_CONSTANT_FULL
        MIKROE_2n13_display_constant(intf,0xFF);
        drv.loadFullImage = 0;
#else
        drv.loadFullImage = 1; // prep for a full rewrite of the screen
#endif

        /* FIXME: reserved for debug of new drivers after initialization */
    }
}

#if LOG_LEGATO
/* Optional feature to record Legato commands in sequence.
 * Define and set up the necessary data structures. */
typedef struct DRV_gfx_bufferLog_s
{ 
    uint32_t x; uint32_t y; 
    gfxPixelBuffer buffer; 
    uint8_t lutSelector;
} DRV_gfx_bufferLog_t;

/* These need to be global for the debugger */
volatile uint32_t DRV_gfx_bufferLogIndex = 0;
volatile DRV_gfx_bufferLog_t DRV_gfx_bufferLog[50] = {0};
#endif

/** Driver-specific implementation of 
 *  GFX entry point to write a section of the display.
 *
 * @details Load pixel data to the specified coordinates
 *          using a mode defined by IOCTL
 * @return operation status (GFX_SUCCESS or GFX_FAILURE)
 * @note Often, data is a subset of the layer.
 * @note This is one (of 5) primary Legato entry points. */
gfxResult DRV_EPD_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf)
{ 
    if (drv.state != RUN)
    { return GFX_FAILURE; }
    
    GFX_Disp_Intf intf = (GFX_Disp_Intf) drv.port_priv;

#if LOG_LEGATO
    /* For testing, record the first few drawing requests in case
     * there is something gone sour with the display. */
    if (DRV_gfx_bufferLogIndex < ALENGTH(DRV_gfx_bufferLog))
    { 
        volatile DRV_gfx_bufferLog_t * b = &DRV_gfx_bufferLog[DRV_gfx_bufferLogIndex++];
        b->x = x; b->y = y; 
        b->buffer = *buf; 
        b->lutSelector = drv.refreshFastControlTable;
        NOP(); NOP(); NOP();
    }
#endif
    
    /* This section is controlled by IOCTL variables. */
    avdisplay_load_t fullPartial = AVDISPLAY_LOAD_PARTIAL;
    avdisplay_buffer_t displayMemory = AVDISPLAY_BUFFER_NONE;
    if (0 != drv.loadFullImage)
    {
        /* We have been asked to load to the "old" memory buffer but we
         * only do that for a full image, otherwise ignore the request. */
        if ((0 == x) && (0 == y)
         && ((DISPLAY_WIDTH*DISPLAY_HEIGHT) == buf->pixel_count))
        { 
            fullPartial = AVDISPLAY_LOAD_FULL_DISPLAY;
            displayMemory = AVDISPLAY_BUFFER_OLDorBW;
        }
    }
    
    /* Reset the request so subsequent images go to the "new" buffer. */
    drv.loadFullImage = 0;

    /* Load the image to the display, possibly multiple times. */
    if (0 == drv.updateOverdraw)
    {
        DRV_EPD_Blit(intf, DRV_writeData, 
                        fullPartial, displayMemory, x, y, buf); 
    }
    else for (int i = 0; i < drv.updateOverdraw; i++)
    {
        DRV_EPD_Blit(intf, DRV_writeData, 
                        fullPartial, displayMemory, x, y, buf); 
    }
    
    /* There is magic here. We bump up from the slow to the fast LUT for 
     * subsequent data transfers; if this is not intended, send the IOCTL again. */
    if (0 == drv.refreshFastControlTable)
    { drv.refreshFastControlTable = 1; }
    
    return GFX_SUCCESS;
}

/** Driver I/O data and configuration interface
 * @param request Legato-defined function request
 * @param arg Optional argument pointer for input and returned data
 * @note Most information is provided by drv_gfx_EPD_customize.h */
gfxDriverIOCTLResponse DRV_EPD_IOCTL_Extended(gfxDriverIOCTLRequestExtended request,
                                     void* arg)
{
    switch(request)
    {
        default:
        { NOP(); NOP(); NOP(); break; }
        
        case GFX_IOCTL_EPD_FAST_REFRESH:
        {
            drv.refreshFastControlTable = ((gfxIOCTLArg_Value*)arg)->value.v_uint;
            break;
        }    
        case GFX_IOCTL_EPD_LOAD_FULL_IMAGE:
        {
            drv.loadFullImage = ((gfxIOCTLArg_Value*)arg)->value.v_uint;
            break;
        }    
        case GFX_IOCTL_EPD_OVERDRAW:
        {
            uint32_t v = ((gfxIOCTLArg_Value*)arg)->value.v_uint;
            drv.updateOverdraw = (3 <= v)? v : 0;
            break;
        }
    }
    
    return GFX_IOCTL_UNSUPPORTED;
}

/** Driver I/O data and configuration interface
 * @param request Legato-defined function request
 * @param arg Optional argument pointer for input and returned data
 * @note Most information is provided by drv_gfx_EPD_customize.h */
gfxDriverIOCTLResponse DRV_EPD_IOCTL(gfxDriverIOCTLRequest request,
                                     void* arg)
{
    gfxIOCTLArg_Value* val;
    gfxIOCTLArg_DisplaySize* disp;
    gfxIOCTLArg_LayerRect* rect;

    switch(request)
    {
        default:
        { 
            return DRV_EPD_IOCTL_Extended((gfxDriverIOCTLRequestExtended) request, arg);
        }
        
        case GFX_IOCTL_GET_COLOR_MODE:
        {
            val = (gfxIOCTLArg_Value*)arg;

            val->value.v_colormode = PIXEL_BUFFER_COLOR_MODE;

            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_BUFFER_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;

            val->value.v_uint = 1;

            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_DISPLAY_SIZE:
        {
            disp = (gfxIOCTLArg_DisplaySize*)arg;

            disp->width = DISPLAY_WIDTH;
            disp->height = DISPLAY_HEIGHT;

            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_LAYER_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;

            val->value.v_uint = 1;

            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value*)arg;

            val->value.v_uint = 0;

            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_LAYER_RECT:
        {
            rect = (gfxIOCTLArg_LayerRect*)arg;

            rect->layer.id = 0;
            rect->x = 0;
            rect->y = 0;
            rect->width = DISPLAY_WIDTH;
            rect->height = DISPLAY_HEIGHT;

            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_STATUS:
		{
            val = (gfxIOCTLArg_Value*)arg;

            if (drv.state == RUN)
                val->value.v_uint = 0;
            else
                val->value.v_uint = 1;

            return GFX_IOCTL_OK;
	    }
    }

    return GFX_IOCTL_UNSUPPORTED;
}
