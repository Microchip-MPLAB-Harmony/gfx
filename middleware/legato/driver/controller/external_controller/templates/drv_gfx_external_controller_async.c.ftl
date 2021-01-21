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
    IDLE,
    BLIT_COLUMN_CMD,
    BLIT_COLUMN_DATA,
    BLIT_PAGE_CMD,
    BLIT_PAGE_DATA,
    BLIT_WRITE_CMD,
    BLIT_WRITE_DATA,
    BLIT_DONE,
    ERROR,
} DRV_STATE;

typedef struct ${ControllerName}_DRV 
{   
    /* Driver state */
    DRV_STATE state;
        
    /* Port-specific private data */
    void *port_priv;

    struct
    {
        int32_t x;
        int32_t y;
        gfxPixelBuffer* buf;
    } blitParms;
} ${ControllerName}_DRV;

typedef struct 
{
    /* Command */
    uint8_t cmd;
    
    /* Number of command parameters */
    uint8_t parmCount;
    
    /* Command parameters, max of 16 */
    uint8_t parms[16];
    
    /* delay */
    unsigned int delayms;
    
} ${ControllerName}_CMD_PARAM;

${ControllerName}_DRV drv;

<#if PassiveDriver == false>
static uint32_t swapCount = 0;
</#if>

const ${ControllerName}_CMD_PARAM initCmdParm[] =
{
<#list 0.. (InitCommandsCount - 1) as i>
    <#assign VALUE = "">
    <#assign COMMAND = "Command" + i>
    <#assign COMMENT = "Command" + i + "Comment">
    <#assign DELAY = "Command" + i + "Delay">
    <#assign PARMSCOUNT = "Command" + i + "ParmsCount">
    {0x${.vars[COMMAND]}, ${.vars[PARMSCOUNT]}, {<#if .vars[PARMSCOUNT] != 0><#list 0.. (.vars[PARMSCOUNT] - 1) as j><#assign PARM = "Command" + i + "Parm" + j>0x${.vars[PARM]},</#list><#else>0x0</#if>}, ${.vars[DELAY]}},<#if .vars[COMMENT] != ""> //${.vars[COMMENT]}</#if>
</#list>
};

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

static int DRV_${ControllerName}_Configure(${ControllerName}_DRV *drvPtr,
                                           const ${ControllerName}_CMD_PARAM *initVals,
                                           int numVals)
{
    GFX_Disp_Intf intf = (GFX_Disp_Intf) drvPtr->port_priv;
    unsigned int i, returnValue;

    DRV_${ControllerName}_NCSAssert(intf);
    
    for (i = 0; i < numVals; i++, initVals++)
    {
        returnValue = GFX_Disp_Intf_WriteCommand(intf, initVals->cmd);
        if (0 != returnValue)
            break;
        
        while (GFX_Disp_Intf_Ready(intf) == false);
        
        if (initVals->parms != NULL &&
            initVals->parmCount > 0)
        {
            returnValue = GFX_Disp_Intf_WriteData(intf, 
                                                 (uint8_t *) initVals->parms,
                                                 initVals->parmCount);
            if (0 != returnValue)
                break;
        
            while (GFX_Disp_Intf_Ready(intf) == false);
        }
        
        if (initVals->delayms > 0)
        {
            DRV_${ControllerName}_DelayMS(initVals->delayms);
        }
    }

    DRV_${ControllerName}_NCSDeassert(intf);

    return 0;
}

void DRV_${ControllerName}_Transfer(GFX_Disp_Intf intf)
{
<#if BlitBufferFunctionGenerateMode == "Use Bulk Write">
<#if DataWriteSize == "8" && PixelDataTxSize8Bit != "2 (Little-Endian)">
    static int row;
    uint16_t clr;
</#if>
    uint16_t* ptr;
    uint8_t parm[4];
    
    switch (drv.state)
    {
        case BLIT_COLUMN_CMD:
        {
<#if BlitType == "Driver Asynchronous">
            if (GFX_Disp_Intf_Ready(intf) == false)
                break;
</#if>
            
            DRV_${ControllerName}_NCSAssert(intf);
                    
            drv.state = BLIT_COLUMN_DATA;
            
            GFX_Disp_Intf_WriteCommand(intf, 0x${SetXAddressCommand});
                   
            break;
        }
        case BLIT_COLUMN_DATA:
        {
            uint32_t x = drv.blitParms.x;

<#if BlitType == "Driver Asynchronous">
            if (GFX_Disp_Intf_Ready(intf) == false)
                break;
</#if>

            drv.state = BLIT_PAGE_CMD;

            <#if XAddressOffset != 0>
            //Add X offset
            x += ${XAddressOffset};

            </#if>
<#if FrameAddressSize == "2-bytes">
            //Write X/Column Address
            parm[0] = x>>8;
            parm[1] = x;
            parm[2] = (x + drv.blitParms.buf->size.width - 1)>>8;
            parm[3] = (x + drv.blitParms.buf->size.width - 1);
            GFX_Disp_Intf_WriteData(intf, parm, 4);
<#else>
            //Write X/Column Address
            parm[0] = x;
            parm[1] = (x + drv.blitParms.buf->size.width - 1);
            GFX_Disp_Intf_WriteData(intf, parm, 2);
</#if>

            break;
        }
        case BLIT_PAGE_CMD:
        {
<#if BlitType == "Driver Asynchronous">
            if (GFX_Disp_Intf_Ready(intf) == false)
                break;
</#if>
                        
            drv.state = BLIT_PAGE_DATA;
            
            GFX_Disp_Intf_WriteCommand(intf, 0x${SetYAddressCommand});
                   
            break;
        }
        case BLIT_PAGE_DATA:
        {
            uint32_t y = drv.blitParms.y;

<#if BlitType == "Driver Asynchronous">
            if (GFX_Disp_Intf_Ready(intf) == false)
                break;
</#if>

            drv.state = BLIT_WRITE_CMD;

            <#if YAddressOffset != 0>
            //Add Y offset
            y += ${YAddressOffset};

            </#if>
<#if FrameAddressSize == "2-bytes">
            //Write Y/Page Address
            parm[0] = y>>8;
            parm[1] = y;
            parm[2] = (y + drv.blitParms.buf->size.height - 1)>>8;
            parm[3] = (y + drv.blitParms.buf->size.height - 1);
            GFX_Disp_Intf_WriteData(intf, parm, 4);
<#else>
            //Write Y/Page Address
            parm[0] = y;
            parm[1] = (y + drv.blitParms.buf->size.height - 1);
            GFX_Disp_Intf_WriteData(intf, parm, 2);
</#if>

            break;
        }
        case BLIT_WRITE_CMD:
        {
<#if BlitType == "Driver Asynchronous">
            if (GFX_Disp_Intf_Ready(intf) == false)
                break;
</#if>
            
            drv.state = BLIT_WRITE_DATA;
            
            //Start Memory Write
            GFX_Disp_Intf_WriteCommand(intf, 0x${MemoryWriteCommand});

<#if DataWriteSize == "8" && PixelDataTxSize8Bit != "2 (Little-Endian)">
        row = 0;
</#if>

            break;
        }
        case BLIT_WRITE_DATA:
        {
<#if BlitType == "Driver Asynchronous">
            if (GFX_Disp_Intf_Ready(intf) == false)
                break;
</#if>

<#if DataWriteSize == "16">
            ptr = gfxPixelBufferOffsetGet_Unsafe(drv.blitParms.buf, 0, 0);
            GFX_Disp_Intf_WriteData16(intf, (uint16_t *) ptr,
                                      drv.blitParms.buf->size.width *
                                      drv.blitParms.buf->size.height);
            drv.state = BLIT_DONE;
<#elseif DataWriteSize == "8">
<#if PixelDataTxSize8Bit == "2 (Little-Endian)">
            ptr = gfxPixelBufferOffsetGet_Unsafe(drv.blitParms.buf, 0, 0);
            GFX_Disp_Intf_WriteData(intf,
                                    (uint8_t *) ptr,
                                    PIXEL_BUFFER_BYTES_PER_PIXEL *
                                    drv.blitParms.buf->size.width *
                                    drv.blitParms.buf->size.height);
            drv.state = BLIT_DONE;
<#else>
            if (row < drv.blitParms.buf->size.height)
            {
                int col, dataIdx;
                ptr = gfxPixelBufferOffsetGet_Unsafe(drv.blitParms.buf, 0, row);
<#if PixelDataTxSize8Bit != "2 (Little-Endian)">
                for(col = 0, dataIdx = 0; col < drv.blitParms.buf->size.width; col++)
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
                                        drv.blitParms.buf->size.width);
                row++;
            }
           
            if (row >= drv.blitParms.buf->size.height)
            {
                drv.state = BLIT_DONE;
            }
            
            break;
</#if>
</#if>
        }
        case BLIT_DONE:
        {
<#if BlitType == "Driver Asynchronous">
            if (GFX_Disp_Intf_Ready(intf) == false)
                break;
</#if>

            DRV_${ControllerName}_NCSDeassert(intf); 
            gfxPixelBuffer_SetLocked(drv.blitParms.buf, GFX_FALSE);
            drv.state = IDLE;
            break;
        }
        case IDLE:
        case ERROR:
        default:
        {
            break;
        }        
    }
<#elseif StubGenerateBuildErrorDisable != true>
#error "Blit buffer procedure is not complete. Please complete definition of blit function."
</#if>
}

<#if BlitType == "Interface Asynchronous">
void DRV_${ControllerName}_Intf_Callback(GFX_Disp_Intf intf, GFX_DISP_INTF_STATUS status, void * param)
{
    if (status == GFX_DISP_INTF_TX_DONE)
    {
        DRV_${ControllerName}_Transfer((GFX_Disp_Intf) drv.port_priv);
    }
}
</#if>

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

        DRV_${ControllerName}_Configure(&drv,
                              initCmdParm,
                              sizeof(initCmdParm)/sizeof(${ControllerName}_CMD_PARAM));

<#if BlitType == "Interface Asynchronous">
        GFX_Disp_Intf_Set_Callback((GFX_Disp_Intf) drv.port_priv,
                                   DRV_${ControllerName}_Intf_Callback,
                                   NULL);
</#if>

        drv.state = IDLE;
    }
<#if BlitType == "Driver Asynchronous">
    else if (drv.state != IDLE)
    {
        DRV_${ControllerName}_Transfer((GFX_Disp_Intf) drv.port_priv);
    }
</#if>
}

<#if PassiveDriver == false>
gfxResult DRV_${ControllerName}_BlitBuffer(int32_t x,
                                int32_t y,
                                gfxPixelBuffer* buf)
{

    if(drv.state != IDLE)
        return GFX_FAILURE;

    drv.blitParms.x = x;
    drv.blitParms.y = y;
    drv.blitParms.buf = buf;
    drv.state = BLIT_COLUMN_CMD;
    
    gfxPixelBuffer_SetLocked(buf, GFX_TRUE);

<#if BlitType == "Interface Asynchronous">
    DRV_${ControllerName}_Transfer((GFX_Disp_Intf) drv.port_priv);
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

