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
  Custom ExternalControllerDisplay Top-Level Driver Source File

  File Name:
    drv_gfx_custom_external.c

  Summary:
    Top level driver for ExternalController.

  Description:
    Build-time generated implementation for the ExternalControllerDriver.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/




#include "definitions.h"

#include "gfx/interface/drv_gfx_disp_intf.h"
#include "drv_gfx_external_controller.h"

#include "system/time/sys_time.h"

// Default max width/height of ExternalControllerframe
#define DISPLAY_DEFAULT_WIDTH   480
#define DISPLAY_DEFAULT_HEIGHT  800

#define DISPLAY_WIDTH   480
#define DISPLAY_HEIGHT  272

#define PIXEL_BUFFER_COLOR_MODE LE_COLOR_MODE_RGB_565
#define SCREEN_WIDTH DISPLAY_WIDTH
#define SCREEN_HEIGHT DISPLAY_HEIGHT

#ifdef GFX_DISP_INTF_PIN_RESET_Clear
#define DRV_ExternalController_Reset_Assert()      GFX_DISP_INTF_PIN_RESET_Clear()
#define DRV_ExternalController_Reset_Deassert()    GFX_DISP_INTF_PIN_RESET_Set()
#else
#error "ERROR: GFX_DISP_INTF_PIN_RESET not defined. Please define in Pin Manager."
#define DRV_ExternalController_Reset_Assert()
#define DRV_ExternalController_Reset_Deassert()
#endif

#define DRV_ExternalController_NCSAssert(intf)   GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_CLEAR)

#define DRV_ExternalController_NCSDeassert(intf) GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_SET)

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
} ExternalController_DRV;

ExternalController_DRV drv;

static uint32_t swapCount = 0;


/* ************************************************************************** */

/**
  Function:
    static void DRV_ExternalController_DelayMS(int ms)

  Summary:
    Delay helper function.

  Description:
    This is a helper function for delay using the system tick timer.

  Parameters:
    ms      - Delay in milliseconds

  Returns:
    None.

*/

static inline void DRV_ExternalController_DelayMS(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

/**
  Function:
    static void DRV_ExternalController_Reset(void)

  Summary:
    Toggles the hardware reset to the ExternalController.

  Description:
    This function toggles the GPIO pin for asserting reset to the ExternalController.

  Parameters:
    None

  Returns:
    None

*/
static void DRV_ExternalController_Reset(void)
{
    DRV_ExternalController_Reset_Assert();
    DRV_ExternalController_DelayMS(10);
    DRV_ExternalController_Reset_Deassert();
    DRV_ExternalController_DelayMS(30);
}

int DRV_ExternalController_Initialize(void)
{
    drv.state = INIT;
        
    return 0;
}

static int DRV_ExternalController_Configure(ExternalController_DRV *drv)
{
    GFX_Disp_Intf intf = (GFX_Disp_Intf) drv->port_priv;
    uint8_t cmd;
    uint8_t parms[16];
    
//    DRV_ExternalController_NCSAssert(intf);
    
    DRV_ExternalController_NCSDeassert(intf);
    DRV_ExternalController_DelayMS(10);

    DRV_ExternalController_NCSAssert(intf);
    DRV_ExternalController_DelayMS(10);

    //PLL_MN
    cmd = 0xe2;
    parms[0] = 0x1d;
    parms[1] = 0x2;
    parms[2] = 0x54;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 3);
    DRV_ExternalController_DelayMS(10);

    //PLL_START
    cmd = 0xe0;
    parms[0] = 0x01;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 1);
    DRV_ExternalController_DelayMS(10);

    //PLL_START
    cmd = 0xe0;
    parms[0] = 0x03;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 1);
    DRV_ExternalController_DelayMS(10);

    //SOFT_RESET
    cmd = 0x01;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    DRV_ExternalController_DelayMS(10);

    //SET_PCLK
    cmd = 0xe6;
    parms[0] = 0x2;
    parms[1] = 0x66;
    parms[2] = 0x64;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 3);

    //SET_PANEL_MODE
    cmd = 0xb0;
    parms[0] = 0x20;
    parms[1] = 0x0;
    parms[2] = 0x01;
    parms[3] = 0xdf;
    parms[4] = 0x1;
    parms[5] = 0xf;
    parms[6] = 0x0;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 7);

    //SET_HOR_PERIOD
    cmd = 0xb4;
    parms[0] = 0x2;
    parms[1] = 0xc;
    parms[2] = 0x0;
    parms[3] = 0x2a;
    parms[4] = 0x28;
    parms[5] = 0x0;
    parms[6] = 0x0;
    parms[7] = 0x0;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 8);

    //SET_VER_PERIOD
    cmd = 0xb6;
    parms[0] = 0x1;
    parms[1] = 0x1d;
    parms[2] = 0x0;
    parms[3] = 0xb;
    parms[4] = 0x9;
    parms[5] = 0x0;
    parms[6] = 0x0;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 7);

    //SET_PIXEL_FORMAT
    cmd = 0x3a;
    parms[0] = 0x55;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 1);

    //SET_DATA_INTERFACE
    cmd = 0xf0;
    parms[0] = 0x03;
    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 1);

    //ON_DISPLAY
    cmd = 0x29;
    GFX_Disp_Intf_WriteCommand(intf, cmd);

    DRV_ExternalController_DelayMS(1);

    DRV_ExternalController_NCSDeassert(intf);

    //PWM_CONF
    cmd = 0xBE;
    parms[0] = 0x6;
    parms[1] = 0xff;
    parms[2] = 0x01;
    parms[3] = 0x00;
    parms[4] = 0x00;
    
        DRV_ExternalController_NCSAssert(intf);

    GFX_Disp_Intf_WriteCommand(intf, cmd);
    GFX_Disp_Intf_WriteData(intf, parms, 5);

    DRV_ExternalController_NCSDeassert(intf);

    return 0;
}


/**
  Function:
    static void DRV_ExternalController_Update(void)

  Summary:
    Driver-specific implementation of GFX HAL update function.

  Description:
    On GFX update, this function flushes any pending pixels to the ExternalController.

  Parameters:
    None.

  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

*/
void DRV_ExternalController_Update(void)
{
    if(drv.state == INIT)
    {
        drv.port_priv = (void *) GFX_Disp_Intf_Open();
        if (drv.port_priv == 0)
        {
            drv.state = ERROR;
            return;
        }

        DRV_ExternalController_Reset();

        DRV_ExternalController_Configure(&drv);

        drv.state = RUN;
    }
}

gfxColorMode DRV_ExternalController_GetColorMode(void)
{
    return PIXEL_BUFFER_COLOR_MODE;
}

uint32_t DRV_ExternalController_GetBufferCount(void)
{
    return 1;
}

uint32_t DRV_ExternalController_GetDisplayWidth(void)
{
    return SCREEN_WIDTH;
}

uint32_t DRV_ExternalController_GetDisplayHeight(void)
{
    return SCREEN_HEIGHT;
}

uint32_t DRV_ExternalController_GetLayerCount()
{
    return 1;
}

uint32_t DRV_ExternalController_GetActiveLayer()
{
    return 0;
}

gfxResult DRV_ExternalController_SetActiveLayer(uint32_t idx)
{
    return GFX_SUCCESS;
}

gfxResult DRV_ExternalController_BlitBuffer(int32_t x,
                                int32_t y,
                                gfxPixelBuffer* buf,
								gfxBlend gfx)
{

    uint16_t* ptr;
    uint8_t parm[4];

    GFX_Disp_Intf intf;
    
    if (drv.state != RUN)
        return GFX_FAILURE;
    
    intf = (GFX_Disp_Intf) drv.port_priv;


    DRV_ExternalController_NCSAssert(intf);

    //Write X/Column Address
    parm[0] = x>>8;
    parm[1] = x;
    parm[2] = (x + buf->size.width - 1) >>8;
    parm[3] = (x + buf->size.width - 1);
    GFX_Disp_Intf_WriteCommand(intf, 0x2a);
    GFX_Disp_Intf_WriteData(intf, parm, 4);
    
    //Write Y/Page Address
    parm[0] = y>>8;
    parm[1] = y;
    parm[2] = (y + buf->size.height - 1)>>8;
    parm[3] = (y + buf->size.height - 1);
    GFX_Disp_Intf_WriteCommand(intf, 0x2b);
    GFX_Disp_Intf_WriteData(intf, parm, 4);

    //Start Memory Write
    GFX_Disp_Intf_WriteCommand(intf, 0x0);

    ptr = gfxPixelBufferOffsetGet_Unsafe(buf, 0, 0);
    GFX_Disp_Intf_WriteData16(intf, (uint16_t *) ptr, buf->size.width * buf->size.height);
    DRV_ExternalController_NCSDeassert(intf);

    return GFX_SUCCESS;
}

void DRV_ExternalController_Swap(void)
{
    swapCount++;
}

uint32_t DRV_ExternalController_GetSwapCount(void)
{
    return swapCount;
}

