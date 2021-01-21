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
  Custom ${ControllerName}Display Top-Level Driver Source File

  File Name:
    drv_gfx_custom_external.c

  Summary:
    Top level driver for ${ControllerName}.

  Description:
    Build-time generated implementation for the ${ControllerName}Driver.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

<#if gfx_hal_le??>

<#assign Val_Width = gfx_hal_le.DisplayWidth>
<#assign Val_Height = gfx_hal_le.DisplayHeight>

<#else>

<#assign Val_Width = DisplayWidth>
<#assign Val_Height = DisplayHeight>

</#if>

#include "definitions.h"

#include "gfx/interface/drv_gfx_disp_intf.h"
#include "drv_gfx_external_controller.h"

#include "system/time/sys_time.h"

<#if PassiveDriver == false>
// Default max width/height of ${ControllerName}frame
#define DISPLAY_DEFAULT_WIDTH   480
#define DISPLAY_DEFAULT_HEIGHT  800

#define DISPLAY_WIDTH   ${Val_Width}
#define DISPLAY_HEIGHT  ${Val_Height}

#define PIXEL_BUFFER_COLOR_MODE GFX_COLOR_MODE_RGB_565
#define SCREEN_WIDTH DISPLAY_WIDTH
#define SCREEN_HEIGHT DISPLAY_HEIGHT
</#if>

<#if ResetEnable == true>
#ifdef GFX_DISP_INTF_PIN_RESET_Clear
#define DRV_${ControllerName}_Reset_Assert()      GFX_DISP_INTF_PIN_RESET_Clear()
#define DRV_${ControllerName}_Reset_Deassert()    GFX_DISP_INTF_PIN_RESET_Set()
#else
#error "ERROR: GFX_DISP_INTF_PIN_RESET not defined. Please define in Pin Manager."
#define DRV_${ControllerName}_Reset_Assert()
#define DRV_${ControllerName}_Reset_Deassert()
#endif
</#if>

#define DRV_${ControllerName}_NCSAssert(intf)   GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_CLEAR)

#define DRV_${ControllerName}_NCSDeassert(intf) GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_SET)
<#if PassiveDriver == false>
<#if DataWriteSize == "8">
    <#if PixelDataTxSize8Bit == "2 (Little-Endian)">
#define PIXEL_BUFFER_BYTES_PER_PIXEL 2
    <#elseif PixelDataTxSize8Bit == "3">
#define PIXEL_BUFFER_BYTES_PER_PIXEL 3
    <#else>
#define PIXEL_BUFFER_BYTES_PER_PIXEL 2
    </#if>
<#if PixelDataTxSize8Bit != "2 (Little-Endian)">
static uint8_t pixelBuffer[SCREEN_WIDTH * PIXEL_BUFFER_BYTES_PER_PIXEL];
</#if>
</#if>
</#if>

typedef enum
{
    INIT = 0,
    RUN,
    ERROR,
} DRV_STATE;

typedef struct ILI9488_DRV 
{   
    /* Driver state */
    DRV_STATE state;
        
    /* Port-specific private data */
    void *port_priv;
} ${ControllerName}_DRV;

${ControllerName}_DRV drv;

<#if PassiveDriver == false>
static uint32_t swapCount = 0;
</#if>


/* ************************************************************************** */

/**
  Function:
    static void DRV_${ControllerName}_DelayMS(int ms)

  Summary:
    Delay helper function.

  Description:
    This is a helper function for delay using the system tick timer.

  Parameters:
    ms      - Delay in milliseconds

  Returns:
    None.

*/

static inline void DRV_${ControllerName}_DelayMS(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

<#if ResetEnable == true>
/**
  Function:
    static void DRV_${ControllerName}_Reset(void)

  Summary:
    Toggles the hardware reset to the ${ControllerName}.

  Description:
    This function toggles the GPIO pin for asserting reset to the ${ControllerName}.

  Parameters:
    None

  Returns:
    None

*/
static void DRV_${ControllerName}_Reset(void)
{
    DRV_${ControllerName}_Reset_Assert();
    DRV_${ControllerName}_DelayMS(${ResetAssertDuration});
    DRV_${ControllerName}_Reset_Deassert();
    DRV_${ControllerName}_DelayMS(${PostResetDelay});
}
</#if>

int DRV_${ControllerName}_Initialize(void)
{
    drv.state = INIT;

    return 0;
}

static int DRV_${ControllerName}_Configure(${ControllerName}_DRV *drvPtr)
{
    GFX_Disp_Intf intf = (GFX_Disp_Intf) drvPtr->port_priv;
<#if InitCommandsCount != 0>
    uint8_t cmd;
    uint8_t parms[${MaXNumParms}];
</#if>

<#if InitCommandsCount != 0>
    DRV_${ControllerName}_NCSAssert(intf);

    <#list 0.. (InitCommandsCount - 1) as i>
        <#assign COMMAND = "Command" + i>
        <#assign COMMENT = "Command" + i + "Comment">
        <#assign DELAY = "Command" + i + "Delay">
        <#assign PARMSCOUNT = "Command" + i + "ParmsCount">
        <#if .vars[COMMENT] != "">
    //${.vars[COMMENT]}
        </#if>
    cmd = 0x${.vars[COMMAND]};
    <#if .vars[PARMSCOUNT] != 0>
        <#list 0.. (.vars[PARMSCOUNT] - 1) as j>
            <#assign PARM = "Command" + i + "Parm" + j>
    parms[${j}] = 0x${.vars[PARM]};
        </#list>
    </#if>
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    <#if .vars[PARMSCOUNT] != 0>
    GFX_Disp_Intf_WriteData(intf, parms, ${.vars[PARMSCOUNT]});
    </#if>
    <#if .vars[DELAY] != 0>
    DRV_${ControllerName}_DelayMS(${.vars[DELAY]});
    </#if>

    </#list>
    DRV_${ControllerName}_NCSDeassert(intf);
</#if>

    return 0;
}


/**
  Function:
    static void DRV_${ControllerName}_Update(void)

  Summary:
    Driver-specific implementation of GFX HAL update function.

  Description:
    On GFX update, this function flushes any pending pixels to the ${ControllerName}.

  Parameters:
    None.

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
void DRV_${ControllerName}_Update(void)
{
    uint32_t openVal;
    
    if(drv.state == INIT)
    {
        openVal = GFX_Disp_Intf_Open();
        
        drv.port_priv = (void *)openVal;
        
        if (drv.port_priv == 0)
        {
            drv.state = ERROR;
            return;
        }

<#if ResetEnable == true>
        DRV_${ControllerName}_Reset();
</#if>

        DRV_${ControllerName}_Configure(&drv);

        drv.state = RUN;
    }
}

<#if PassiveDriver == false>

gfxResult DRV_${ControllerName}_BlitBuffer(int32_t x,
                                           int32_t y,
                                           gfxPixelBuffer* buf)
{

<#if BlitBufferFunctionGenerateMode == "Use Bulk Write">
<#if DataWriteSize == "8" && PixelDataTxSize8Bit != "2 (Little-Endian)">
    int row;
    uint16_t clr;
</#if>
    uint16_t* ptr;
    uint8_t parm[4];

    GFX_Disp_Intf intf;
    
    if (drv.state != RUN)
        return GFX_FAILURE;
    
    intf = (GFX_Disp_Intf) drv.port_priv;

<#if XAddressOffset != 0>
    //Add X offset
    x += ${XAddressOffset};
</#if>
<#if YAddressOffset != 0>
    //Add Y offset
    y += ${YAddressOffset};
</#if>

    DRV_${ControllerName}_NCSAssert(intf);

<#if FrameAddressSize == "2-bytes">
    //Write X/Column Address
    parm[0] = x>>8;
    parm[1] = x;
    parm[2] = (x + buf->size.width - 1) >>8;
    parm[3] = (x + buf->size.width - 1);
    GFX_Disp_Intf_WriteCommand(intf, 0x${SetXAddressCommand});
    GFX_Disp_Intf_WriteData(intf, parm, 4);
    
    //Write Y/Page Address
    parm[0] = y>>8;
    parm[1] = y;
    parm[2] = (y + buf->size.height - 1)>>8;
    parm[3] = (y + buf->size.height - 1);
    GFX_Disp_Intf_WriteCommand(intf, 0x${SetYAddressCommand});
    GFX_Disp_Intf_WriteData(intf, parm, 4);
<#else>
    //Write X/Column Address
    parm[0] = x;
    parm[1] = (x + buf->size.width - 1);
    GFX_Disp_Intf_WriteCommand(intf, 0x${SetXAddressCommand});
    GFX_Disp_Intf_WriteData(intf, parm, 2);
    
    //Write Y/Page Address
    parm[0] = y;
    parm[1] = (y + buf->size.height - 1);
    GFX_Disp_Intf_WriteCommand(intf, 0x${SetYAddressCommand});
    GFX_Disp_Intf_WriteData(intf, parm, 2);
</#if>

    //Start Memory Write
    GFX_Disp_Intf_WriteCommand(intf, 0x${MemoryWriteCommand});

<#if DataWriteSize == "16">
    ptr = gfxPixelBufferOffsetGet_Unsafe(buf, 0, 0);
    GFX_Disp_Intf_WriteData16(intf, (uint16_t *) ptr, buf->size.width * buf->size.height);
<#elseif DataWriteSize == "8">
<#if PixelDataTxSize8Bit == "2 (Little-Endian)">
    ptr = gfxPixelBufferOffsetGet_Unsafe(buf, 0, 0);
    GFX_Disp_Intf_WriteData(intf,
                            (uint8_t *) ptr,
                            PIXEL_BUFFER_BYTES_PER_PIXEL *
                            buf->size.width *
buf->size.height);
<#else>

    for(row = 0; row < buf->size.height; row++)
    {
        int col, dataIdx;
        ptr = gfxPixelBufferOffsetGet_Unsafe(buf, 0, row);
<#if PixelDataTxSize8Bit != "2 (Little-Endian)">
        for(col = 0, dataIdx = 0; col < buf->size.width; col++)
        {
            clr = ptr[col];
<#if PixelDataTxSize8Bit == "3">
            pixelBuffer[dataIdx++] = (uint8_t) ((clr & 0xf800) >> 8);
            pixelBuffer[dataIdx++] = (uint8_t) ((clr & 0x07e0) >> 3 );
            pixelBuffer[dataIdx++] = (uint8_t) ((clr & 0x001f) << 3);
</#if>
<#if PixelDataTxSize8Bit == "2 (Normal)">
            pixelBuffer[dataIdx++] = (uint8_t) (clr >> 8);
            pixelBuffer[dataIdx++] = (uint8_t) (uint8_t) (clr & 0xff);
</#if>
        }
</#if>
        GFX_Disp_Intf_WriteData(intf,
                                pixelBuffer,
                                PIXEL_BUFFER_BYTES_PER_PIXEL *
                                buf->size.width);
    }
</#if>
</#if>
    DRV_${ControllerName}_NCSDeassert(intf);
<#elseif StubGenerateBuildErrorDisable != true>
#error "Blit buffer procedure is not complete. Please complete definition of blit function."
</#if>

    return GFX_SUCCESS;
}

gfxDriverIOCTLResponse DRV_${ControllerName}_IOCTL(gfxDriverIOCTLRequest request,
                                     void* arg)
{
    gfxIOCTLArg_Value* val;
    gfxIOCTLArg_DisplaySize* disp;
    gfxIOCTLArg_LayerRect* rect;
    
    switch(request)
    {
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
            
            rect->base.id = 0;
            rect->x = 0;
            rect->y = 0;
            rect->width = DISPLAY_WIDTH;
            rect->height = DISPLAY_HEIGHT;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_VSYNC_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = swapCount;
            
            return GFX_IOCTL_OK;
        }
        default:
        { }
    }
    
    return GFX_IOCTL_UNSUPPORTED;
}
</#if>

