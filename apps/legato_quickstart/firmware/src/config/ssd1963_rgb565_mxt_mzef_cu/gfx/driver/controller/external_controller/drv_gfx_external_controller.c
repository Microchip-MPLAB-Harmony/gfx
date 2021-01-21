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
  Custom SSD1963Display Top-Level Driver Source File

  File Name:
    drv_gfx_custom_external.c

  Summary:
    Top level driver for SSD1963.

  Description:
    Build-time generated implementation for the SSD1963Driver.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/
#include "definitions.h"

#include "gfx/interface/drv_gfx_disp_intf.h"
#include "drv_gfx_external_controller.h"

#include "system/time/sys_time.h"

// Default max width/height of SSD1963frame
#define DISPLAY_DEFAULT_WIDTH   480
#define DISPLAY_DEFAULT_HEIGHT  800

#define DISPLAY_WIDTH   480
#define DISPLAY_HEIGHT  272

#define PIXEL_BUFFER_COLOR_MODE LE_COLOR_MODE_RGB_565
#define SCREEN_WIDTH DISPLAY_WIDTH
#define SCREEN_HEIGHT DISPLAY_HEIGHT

#ifdef GFX_DISP_INTF_PIN_RESET_Clear
#define DRV_SSD1963_Reset_Assert()      GFX_DISP_INTF_PIN_RESET_Clear()
#define DRV_SSD1963_Reset_Deassert()    GFX_DISP_INTF_PIN_RESET_Set()
#else
#error "ERROR: GFX_DISP_INTF_PIN_RESET not defined. Please define in Pin Manager."
#define DRV_SSD1963_Reset_Assert()
#define DRV_SSD1963_Reset_Deassert()
#endif

#define DRV_SSD1963_NCSAssert(intf)   GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_CLEAR)

#define DRV_SSD1963_NCSDeassert(intf) GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_SET)

typedef enum
{
    INIT = 0,
    RUN
} DRV_STATE;

typedef struct ILI9488_DRV 
{   
    /* Driver state */
    DRV_STATE state;
        
    /* Port-specific private data */
    void *port_priv;
} SSD1963_DRV;

SSD1963_DRV drv;

static uint32_t swapCount = 0;


/* ************************************************************************** */

/**
  Function:
    static void DRV_SSD1963_DelayMS(int ms)

  Summary:
    Delay helper function.

  Description:
    This is a helper function for delay using the system tick timer.

  Parameters:
    ms      - Delay in milliseconds

  Returns:
    None.

*/

static inline void DRV_SSD1963_DelayMS(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

/**
  Function:
    static void DRV_SSD1963_Reset(void)

  Summary:
    Toggles the hardware reset to the SSD1963.

  Description:
    This function toggles the GPIO pin for asserting reset to the SSD1963.

  Parameters:
    None

  Returns:
    None

*/
static void DRV_SSD1963_Reset(void)
{
    DRV_SSD1963_Reset_Assert();
    DRV_SSD1963_DelayMS(10);
    DRV_SSD1963_Reset_Deassert();
    DRV_SSD1963_DelayMS(30);
}

int DRV_SSD1963_Initialize(void)
{
    drv.state = INIT;

    drv.port_priv = (void *) GFX_Disp_Intf_Open();
    if (drv.port_priv == 0)
        return -1;
    
    return 0;
}

static int DRV_SSD1963_Configure(SSD1963_DRV *drv)
{
    GFX_Disp_Intf intf = (GFX_Disp_Intf) drv->port_priv;
    uint8_t cmd;
    uint8_t parms[16];

    DRV_SSD1963_NCSAssert(intf);

    //Set PLL Config
    cmd = 0xe2;
    parms[0] = 0x1d;
    parms[1] = 0x2;
    parms[2] = 0x54;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 3);
    DRV_SSD1963_DelayMS(10);

    //Enable PLL
    cmd = 0xe0;
    parms[0] = 0x1;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 1);
    DRV_SSD1963_DelayMS(10);

    //Use PLL as System Clock
    cmd = 0xe0;
    parms[0] = 0x3;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 1);
    DRV_SSD1963_DelayMS(10);

    //Software Reset
    cmd = 0x1;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 0);
    DRV_SSD1963_DelayMS(10);

    //Set Pixel Clock to 15MHz
    cmd = 0xe6;
    parms[0] = 0x2;
    parms[1] = 0x66;
    parms[2] = 0x64;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 3);

    //Set Panel Mode, 480x272, RGB
    cmd = 0xb0;
    parms[0] = 0x20;
    parms[1] = 0x0;
    parms[2] = 0x1;
    parms[3] = 0xdf;
    parms[4] = 0x1;
    parms[5] = 0xf;
    parms[6] = 0x0;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 7);

    //Set Horizontal Period
    cmd = 0xb4;
    parms[0] = 0x2;
    parms[1] = 0xc;
    parms[2] = 0x0;
    parms[3] = 0x2a;
    parms[4] = 0x28;
    parms[5] = 0x0;
    parms[6] = 0x0;
    parms[7] = 0x0;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 8);

    //Set Vertical Period
    cmd = 0xb6;
    parms[0] = 0x1;
    parms[1] = 0x1d;
    parms[2] = 0x0;
    parms[3] = 0xb;
    parms[4] = 0x9;
    parms[5] = 0x0;
    parms[6] = 0x0;
    parms[7] = 0x0;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 8);

    //Set Pixel Format RGB565 (16bpp)
    cmd = 0x3a;
    parms[0] = 0x55;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 1);

    //Set Data Interface
    cmd = 0xf0;
    parms[0] = 0x3;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 1);

    //Turn on backlight
    cmd = 0xbe;
    parms[0] = 0x6;
    parms[1] = 0xff;
    parms[2] = 0x1;
    parms[3] = 0x0;
    parms[4] = 0x0;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 5);

    //Turn on the display
    cmd = 0x29;
    GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, 0);

    DRV_SSD1963_NCSDeassert(intf);

    return 0;
}


/**
  Function:
    static void DRV_SSD1963_Update(void)

  Summary:
    Driver-specific implementation of GFX HAL update function.

  Description:
    On GFX update, this function flushes any pending pixels to the SSD1963.

  Parameters:
    None.

  Returns:
    * LE_SUCCESS       - Operation successful
    * LE_FAILURE       - Operation failed

*/
void DRV_SSD1963_Update(void)
{
    if(drv.state == INIT)
    {
        DRV_SSD1963_Reset();

        DRV_SSD1963_Configure(&drv);

        drv.state = RUN;
    }
}

leColorMode DRV_SSD1963_GetColorMode(void)
{
    return PIXEL_BUFFER_COLOR_MODE;
}

uint32_t DRV_SSD1963_GetBufferCount(void)
{
    return 1;
}

uint32_t DRV_SSD1963_GetDisplayWidth(void)
{
    return SCREEN_WIDTH;
}

uint32_t DRV_SSD1963_GetDisplayHeight(void)
{
    return SCREEN_HEIGHT;
}

uint32_t DRV_SSD1963_GetLayerCount()
{
    return 1;
}

uint32_t DRV_SSD1963_GetActiveLayer()
{
    return 0;
}

leResult DRV_SSD1963_SetActiveLayer(uint32_t idx)
{
    return LE_SUCCESS;
}

leResult DRV_SSD1963_BlitBuffer(int32_t x,
                                int32_t y,
                                lePixelBuffer* buf)
{

    uint16_t* ptr;
    uint8_t parm[4];

    GFX_Disp_Intf intf;
    
    if (drv.state != RUN)
        return LE_FAILURE;
    
    intf = (GFX_Disp_Intf) drv.port_priv;

    DRV_SSD1963_NCSAssert(intf);

    //Write X/Column Address
    parm[0] = x>>8;
    parm[1] = x;
    parm[2] = (x + buf->size.width - 1) >>8;
    parm[3] = (x + buf->size.width - 1);
    GFX_Disp_Intf_WriteCommandParm(intf, 0x2a, parm, 4);
    
    //Write Y/Page Address
    parm[0] = y>>8;
    parm[1] = y;
    parm[2] = (y + buf->size.height - 1)>>8;
    parm[3] = (y + buf->size.height - 1);
    GFX_Disp_Intf_WriteCommandParm(intf, 0x2b, parm, 4);

    //Start Memory Write
    GFX_Disp_Intf_WriteCommand(intf, 0x2c);

    ptr = lePixelBufferOffsetGet_Unsafe(buf, 0, 0);
    GFX_Disp_Intf_WriteData16(intf, (uint16_t *) ptr, buf->size.width * buf->size.height);
    DRV_SSD1963_NCSDeassert(intf);

    return LE_SUCCESS;
}

void DRV_SSD1963_Swap(void)
{
    swapCount++;
}

uint32_t DRV_SSD1963_GetSwapCount(void)
{
    return swapCount;
}

