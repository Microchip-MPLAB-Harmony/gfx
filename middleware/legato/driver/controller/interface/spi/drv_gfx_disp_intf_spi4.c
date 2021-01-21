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
    drv_gfx_intf_spi4.c

  Summary:
    Implements the display interface driver over SPI

  Description:
    Implements the 4-line SPI interface driver

    Created with MPLAB Harmony Version 3.0
 *******************************************************************************/

#include "driver/spi/drv_spi.h"

#include "../drv_gfx_disp_intf.h"

#ifndef GFX_DISP_INTF_PIN_CS_Set
#error "GFX_DISP_INTF_PIN_CS (chip select) GPIO pin must be defined in pin manager"
#endif

#ifndef GFX_DISP_INTF_PIN_RSDC_Set
#error "GFX_DISP_INTF_PIN_RSDC (data/command select) GPIO pin must be defined in pin manager"
#endif

#define DRV_SPI_INDEX 0

/** SPI_TRANS_STATUS

  Summary:
    Enum type of SPI transaction status

 */
typedef enum 
{
    SPI_TRANS_IDLE,
    SPI_TRANS_CMD_WR_PENDING,
    SPI_TRANS_CMD_RD_PENDING,
    SPI_TRANS_FAIL,
} GFX_DISP_INTF_SPI_TRANS_STATUS;

/** GFX_INTF_SPI

  Summary:
    Structure contains status and handles for SPI interface.
    
 */
typedef struct
{    
    /* SPI Device handle */
    DRV_HANDLE drvSPIHandle;
    
    /* Transfer handle */
    DRV_SPI_TRANSFER_HANDLE drvSPITransferHandle;
    
    /* SPI transaction status */
    volatile GFX_DISP_INTF_SPI_TRANS_STATUS drvSPITransStatus;
    
    /* SPI interface callback */
    GFX_Disp_Intf_Callback callback;

    /* SPI interface callback param*/
    void * callback_parm;
} GFX_DISP_INTF_SPI;

static GFX_DISP_INTF_SPI spiIntf;

/* ************************************************************************** */

/** 
  Function:
    static void GFX_Disp_Intf_CallBack(DRV_SPI_BUFFER_EVENT event, 
                                       DRV_SPI_BUFFER_HANDLE bufferHandle, 
                                       void * context )

  Summary:
    Callback function called by SPI driver to deliver events.

  Description:

    This callback will set the ILI9488 SPI driver's SPI transaction status 
    based on the event.


  Parameters:
    event           - SPI buffer event
    bufferHandle    - SPI buffer handle
    context         - SPI transaction context

  Returns:
    None

 */
static void GFX_Disp_Intf_CallBack(DRV_SPI_TRANSFER_EVENT event,
                                   DRV_SPI_TRANSFER_HANDLE bufferHandle,
                                   uintptr_t context)
{
    volatile GFX_DISP_INTF_SPI_TRANS_STATUS *status = 
                    ((GFX_DISP_INTF_SPI_TRANS_STATUS *) context);

   if (!status)
        return;

    switch (event) 
    {
        case DRV_SPI_TRANSFER_EVENT_COMPLETE:
        {
            *status = SPI_TRANS_IDLE;

            if (spiIntf.callback != NULL)
            {
                spiIntf.callback((GFX_Disp_Intf) &spiIntf,
                                       GFX_DISP_INTF_TX_DONE,
                                       spiIntf.callback_parm);
            }            

            break;
        }
        case DRV_SPI_TRANSFER_EVENT_ERROR:
        {
            *status = SPI_TRANS_FAIL;
        }
        default:
        {
            break;
        }
    }
}

GFX_Disp_Intf GFX_Disp_Intf_Open(void)
{   
    spiIntf.drvSPIHandle = DRV_SPI_Open(${DRV_INTERFACE_SPI_INDEX}, DRV_IO_INTENT_READWRITE);
    
    if (DRV_HANDLE_INVALID == spiIntf.drvSPIHandle)
    {
        return -1;
    }

    DRV_SPI_TransferEventHandlerSet(spiIntf.drvSPIHandle,
                                    GFX_Disp_Intf_CallBack,
                                    (uintptr_t)&spiIntf.drvSPITransStatus );

    return (GFX_Disp_Intf)&spiIntf;
}

void GFX_Disp_Intf_Close(GFX_Disp_Intf intf)
{
    DRV_SPI_Close(((GFX_DISP_INTF_SPI *)intf)->drvSPIHandle);
}

int GFX_Disp_Intf_PinControl(GFX_Disp_Intf intf,
                                 GFX_DISP_INTF_PIN pin,
                                 GFX_DISP_INTF_PIN_VALUE value)
{
    int res = -1;

    if (((GFX_DISP_INTF_SPI *) intf) == NULL)
        return -1;
        
    switch(pin)
    {
        case GFX_DISP_INTF_PIN_CS:
        {
#ifdef GFX_DISP_INTF_PIN_CS_Set
            if(value == GFX_DISP_INTF_PIN_CLEAR)
            {
                GFX_DISP_INTF_PIN_CS_Clear();
            }
            else
            {
                GFX_DISP_INTF_PIN_CS_Set();
            }
            
            res = 0;
#endif
            break;
        }
        case GFX_DISP_INTF_PIN_WR:
        {
#ifdef GFX_DISP_INTF_PIN_WR_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
            {
                GFX_DISP_INTF_PIN_WR_Clear();
            }
            else
            {
                GFX_DISP_INTF_PIN_WR_Set();
            }
            
            res = 0;            
#endif
            break;
        }
        case GFX_DISP_INTF_PIN_RD:
        {
#ifdef GFX_DISP_INTF_PIN_RD_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
            {
                GFX_DISP_INTF_PIN_RD_Clear();
            }
            else
            {
                GFX_DISP_INTF_PIN_RD_Set();
            }
            
            res = 0;
#endif
            break;
        }
        case GFX_DISP_INTF_PIN_RSDC:
        {
#ifdef GFX_DISP_INTF_PIN_RSDC_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
            {
                GFX_DISP_INTF_PIN_RSDC_Clear();
            }
            else
            {
                GFX_DISP_INTF_PIN_RSDC_Set();
            }
            
            res = 0;
#endif            
            break;
        }
        default:
        {
            break;
        }
    }
    
    return res;
}

int GFX_Disp_Intf_WriteCommand(GFX_Disp_Intf intf, uint8_t cmd)
{
    GFX_DISP_INTF_PIN_RSDC_Clear();
    
    return GFX_Disp_Intf_Write(intf, &cmd, 1);
}

int GFX_Disp_Intf_WriteData(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_RSDC, GFX_DISP_INTF_PIN_SET);
    
    return GFX_Disp_Intf_Write(intf, data, bytes);
}

int GFX_Disp_Intf_ReadData(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_PIN_RSDC_Set();
    
    return GFX_Disp_Intf_Read(intf, data, bytes);
}

DRV_GFX_DEPRECATED int GFX_Disp_Intf_ReadCommandData(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * data, int num_data)
{
    int retval;
    
    retval = GFX_Disp_Intf_WriteCommand(intf, cmd);
    if (retval != 0)
        return -1;

    GFX_DISP_INTF_PIN_RSDC_Set();
    
    return GFX_Disp_Intf_Read(intf, data, num_data);
}

DRV_GFX_DEPRECATED int GFX_Disp_Intf_WriteCommandParm(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * parm, int num_parms)
{
    int retval = -1;
    
    GFX_DISP_INTF_PIN_RSDC_Clear();
    
    retval = GFX_Disp_Intf_Write(intf, &cmd, 1);
    if (retval != 0)
        return -1;
    
    if (num_parms > 0 && parm != NULL)
    {
        GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_RSDC, GFX_DISP_INTF_PIN_SET);
         retval = GFX_Disp_Intf_Write(intf, parm, num_parms);
    }

    return retval;
}

int GFX_Disp_Intf_Write(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_SPI * spiIntfPtr = (GFX_DISP_INTF_SPI *) intf;
    
    if (spiIntfPtr == NULL || bytes == 0 || data == NULL)
        return -1;
    
    spiIntfPtr->drvSPITransStatus = SPI_TRANS_CMD_WR_PENDING;
    
    DRV_SPI_WriteTransferAdd(spiIntfPtr->drvSPIHandle,
                            (void *) data,
                            (size_t) bytes,
                            (void *) &spiIntfPtr->drvSPITransferHandle);
                            
    if (DRV_SPI_TRANSFER_HANDLE_INVALID == spiIntfPtr->drvSPITransferHandle)
    {
        return -1;
    }
    
<#if BlockingTransfers == true>
    while(!GFX_Disp_Intf_Ready(intf));
</#if>
    
    return 0;
}


int GFX_Disp_Intf_Read(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_SPI * spiIntfPtr = (GFX_DISP_INTF_SPI *) intf;
    
    if (spiIntfPtr == NULL || bytes == 0 || data == NULL)
        return -1;
    
    // Read the valid pixels
    spiIntfPtr->drvSPITransStatus = SPI_TRANS_CMD_RD_PENDING;
    
    DRV_SPI_ReadTransferAdd(spiIntfPtr->drvSPIHandle,
                            (void *) data,
                            bytes,
                            (void *) &spiIntfPtr->drvSPITransferHandle);
                            
    if (DRV_SPI_TRANSFER_HANDLE_INVALID == spiIntfPtr->drvSPITransferHandle)
        return -1;

<#if BlockingTransfers == true>
    while(!GFX_Disp_Intf_Ready(intf));
</#if>
    
    return 0;
}

int GFX_Disp_Intf_WriteData16(GFX_Disp_Intf intf, uint16_t* data, int num)
{
    //Not supported
    return -1;
}

int GFX_Disp_Intf_ReadData16(GFX_Disp_Intf intf, uint16_t* data, int num)
{
    //Not supported
    return -1;
}

int GFX_Disp_Intf_Ready(GFX_Disp_Intf intf)
{
    return (SPI_TRANS_IDLE == ((GFX_DISP_INTF_SPI *) intf)->drvSPITransStatus);
}

int GFX_Disp_Intf_Set_Callback(GFX_Disp_Intf intf, GFX_Disp_Intf_Callback cb, void * parm)
{
    if (((GFX_DISP_INTF_SPI *) intf) == NULL)
        return -1;

    ((GFX_DISP_INTF_SPI *) intf)->callback = cb;
    ((GFX_DISP_INTF_SPI *) intf)->callback_parm = parm;

    return 0;
}

/* *****************************************************************************
 End of File
 */
