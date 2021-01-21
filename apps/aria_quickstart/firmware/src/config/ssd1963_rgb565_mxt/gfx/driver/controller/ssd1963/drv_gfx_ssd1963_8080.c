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
    drv_gfx_ssd1963_intf.c

  Summary:
    Implements DBIB parallel interface for the SSD1963

  Description:
    Implements DBIB parallel interface for the SSD1963. 

    Created with MPLAB Harmony Version 3.0
 *******************************************************************************/

#include "definitions.h"

#include "gfx/hal/inc/gfx_common.h"
#include "gfx/hal/inc/gfx_context.h"

#include "drv_gfx_ssd1963_cmd_defs.h"
#include "drv_gfx_ssd1963_common.h"

#include "drv_gfx_disp_intf.h"

#define SSD1963_NCSAssert(intf)   GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_CLEAR)

#define SSD1963_NCSDeassert(intf) GFX_Disp_Intf_PinControl(intf, \
                                    GFX_DISP_INTF_PIN_CS, \
                                    GFX_DISP_INTF_PIN_SET)

//Width of data bus
typedef uint16_t DBUS_WIDTH_T;

/** 
  Function:
    static GFX_Result SSD1963_Intf_Read(struct SSD1963_DRV *drv, 
                                       uint8_t cmd, 
                                       uint8_t *data,
                                       int bytes)

  Summary:
    Sends read command and returns response from the SSD1963 device.

  Description:
    This function will do a write operation over the parallel interface to send 
    the read command to the SSD1963, and then do a read operation to read the 
    response.

  Parameters:
    drv         - SSD1963 driver handle
    cmd         - Read command
    data        - Buffer to store read data
    bytes       - Number of bytes to read
 
Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed


 */
static GFX_Result SSD1963_Intf_Read(struct SSD1963_DRV *drv,
                                    uint8_t cmd,
                                    uint8_t *data,
                                    int bytes) 
{
    GFX_Disp_Intf intf;
    GFX_Result retval = GFX_FAILURE;

    if ((!drv) || (!data) || (bytes <= 0))
        return GFX_FAILURE;

    intf = (GFX_Disp_Intf) drv->port_priv;
    
    SSD1963_NCSAssert(intf);

    retval = GFX_Disp_Intf_Write(intf, &cmd, 1);
    if (retval == GFX_FAILURE)
    {
        SSD1963_NCSDeassert(intf);
        return GFX_FAILURE;
    }
    
    retval = GFX_Disp_Intf_ReadData(intf, data, bytes);
    
    SSD1963_NCSDeassert(intf);

    return retval;
}

/** 
  Function:
    GFX_Result SSD1963_Intf_WriteCmd(struct SSD1963_DRV *drv,
                                    uint8_t cmd,
                                    uint8_t *parms,
                                    int num_parms)

  Summary:
    Sends write command and parameters to the SSD1963 device.

  Description:
    This function will do a write operation to send the write command 
    and its parameters to the SSD1963.


  Parameters:
    drv         - SSD1963 driver handle
    cmd         - Read command
    parms       - Pointer to array of 8-bit parameters
    bytes       - Number of command parameters
 
Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

 
 */
GFX_Result SSD1963_Intf_WriteCmd(struct SSD1963_DRV *drv,
                                uint8_t cmd,
                                uint8_t *parms,
                                int num_parms) 
{
    GFX_Result returnValue = GFX_FAILURE;
    GFX_Disp_Intf intf;

    if (!drv)
        return GFX_FAILURE;

    intf = (GFX_Disp_Intf ) drv->port_priv;
    
    SSD1963_NCSAssert(intf);

    // Write the command and parameters
    returnValue = GFX_Disp_Intf_WriteCommandParm(intf, cmd, parms, num_parms);
   
    SSD1963_NCSDeassert(intf);

    return returnValue;
}

/** 
  Function:
    GFX_Result SSD1963_Intf_WritePixels(struct SSD1963_DRV *drv,
                                              uint32_t start_x,
                                              uint32_t start_y,
                                              uint8_t *data,
                                              unsigned int num_pixels)

  Summary:
    Writes pixel data to SSD1963 GRAM from specified position.

  Description:
    This function will first write the start column, page information, then 
    write the pixel data to the SSD1963 GRAM.

  Parameters:
    drv             - SSD1963 driver handle
    start_x         - Start column position
    start_y         - Start page position
    data            - Array of 8-bit pixel data (8-bit/pixel RGB)
    num_pixels      - Number of pixels
 
  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

 */
GFX_Result SSD1963_Intf_WritePixels(struct SSD1963_DRV *drv,
                                   uint32_t start_x,
                                   uint32_t start_y,
                                   uint8_t *data,
                                   unsigned int num_pixels)
{
    GFX_Result returnValue = GFX_FAILURE;
    GFX_Disp_Intf intf;
    
    if (!drv)
        return GFX_FAILURE;
    
    intf = (GFX_Disp_Intf) drv->port_priv;
    
    SSD1963_NCSAssert(intf);
    

    SSD1963_NCSDeassert(intf);

    return returnValue;
}

/** 
  Function:
    GFX_Result SSD1963_Intf_ReadPixels(struct SSD1963_DRV *drv,
                                      uint32_t x,
                                      uint32_t y,
                                      uint16_t *value,
                                      unsigned int num_pixels)

  Summary:
    Read pixel data from specified position in SSD1963 GRAM.

  Description:
    This function will first write the start column, page information, then
    read the pixel data from the SSD1963 GRAM.

  Parameters:
    drv             - SSD1963 driver handle
    x               - Column position
    y               - Page position
    value           - Value to store the read pixel color (8-bit/pixel RGB)
    num_pixels      - Number of pixels to read
 
  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

 */
GFX_Result SSD1963_Intf_ReadPixels(struct SSD1963_DRV *drv,
                                  uint32_t x,
                                  uint32_t y,
                                  uint8_t *value,
                                  unsigned int num_pixels)
{
    if (!drv)
        return GFX_FAILURE;
    
    return GFX_SUCCESS;
}

/** 
  Function:
    GFX_Result SSD1963_Intf_ReadCmd(struct SSD1963_DRV *drv, 
                                          uint8_t cmd, 
                                          uint8_t *data,
                                          int bytes);

  Summary:
    Sends read command and reads response from SSD1963.

  Description:
    This function will fist write the the read command and then read back the 
    response from the SSD1963 GRAM.

  Parameters:
    drv             - SSD1963 driver handle
    cmd             - Read command
    data            - Buffer to store the read data to
    bytes           - Number of bytes to read
 
  Returns:
    * GFX_SUCCESS       Operation successful
    * GFX_FAILURE       Operation failed
 
  Remarks:
    This function only supports 8-, 24- or 32-bit reads.

 */
GFX_Result SSD1963_Intf_ReadCmd(struct SSD1963_DRV *drv,
                               uint8_t cmd,
                               uint8_t *data,
                               int bytes)
{
    GFX_Result returnValue = GFX_FAILURE;
    uint8_t buff[5];
    GFX_Disp_Intf intf;    

    //API supports only 8-, 24-, or 32-bit reads
    if ((!drv) || (!data) ||
        ((bytes != 1) && (bytes != 3) && (bytes != 4)))
        return GFX_FAILURE;
    
    intf = (GFX_Disp_Intf) drv->port_priv;
    
    SSD1963_NCSAssert(intf);
    returnValue = SSD1963_Intf_Read(drv, cmd, buff, bytes + 1);
    SSD1963_NCSDeassert(intf);    
    
    
    return returnValue;
}

// *****************************************************************************

/** 
  Function:
    GFX_Result SSD1963_Intf_Open(SSD1963_DRV *drv, unsigned int index)

  Summary:
    Opens the specified port to the SSD1963 device.

  Description:
    In SPI mode, this function will open the SPI port, allocate the port-specific
    data structures and set the port operation handler functions. When done 
    using the port, SSD1963_Intf_Close must be called to free up the data 
    structures and close the port.

  Parameters:
    drv         - SSD1963 driver handle
    index       - Port index
 
  Returns:
    * GFX_SUCCESS       - Operation successful
    * GFX_FAILURE       - Operation failed

 */
GFX_Result SSD1963_Intf_Open(SSD1963_DRV *drv, unsigned int index)
{
    GFX_Disp_Intf intf;
    
    if (!drv)
        return GFX_FAILURE;
    
    drv->port_priv = (void *) GFX_Disp_Intf_Open(drv->gfx, index);
    
    intf = (GFX_Disp_Intf) drv->port_priv;

    SSD1963_NCSDeassert(intf);

    return GFX_SUCCESS;
}

/** 
  Function:
    void SSD1963_Intf_Close(SSD1963_DRV *drv)

  Summary:
    Closes the HW interface to the SSD1963 device.

  Description:
    This function will close the specified interface, free the port-specific
    data structures and unset the port operation handler functions.

  Parameters:
    drv         - SSD1963 driver handle
 
  Returns:
    None.

 */
void SSD1963_Intf_Close(SSD1963_DRV *drv) 
{
    GFX_Disp_Intf intf;
    
    if (!drv)
        return;

    intf = (GFX_Disp_Intf) drv->port_priv;
    
    SSD1963_NCSDeassert(intf);
    
    GFX_Disp_Intf_Close((GFX_Disp_Intf) drv->port_priv);

    drv->port_priv = NULL;
}
/* *****************************************************************************
 End of File
 */
