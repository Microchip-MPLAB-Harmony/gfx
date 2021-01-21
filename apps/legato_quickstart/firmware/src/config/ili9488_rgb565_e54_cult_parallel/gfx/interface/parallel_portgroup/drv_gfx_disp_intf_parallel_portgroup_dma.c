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
    drv_gfx_intf_iogroup.c

  Summary:
    Implements the parallel display interface driver over IO group

  Description:
    Implements the parallel display interface driver over IO group

    Created with MPLAB Harmony Version 3.0
 *******************************************************************************/

#include "definitions.h"

#include "gfx/interface/drv_gfx_disp_intf.h"

#define GFX_DISP_INTF_PORT  (PORT_GROUP_2)
#define GFX_INTF_DMA_CHANNEL 0
#define DATABUS_PTR (&(PORT_REGS->GROUP[2].PORT_OUT))

#ifdef GFX_DISP_INTF_PIN_RSDC_Set
#define GFX_DISP_RSDC_Control(intf, value) GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_RSDC, value)
#else
#error "GFX_DISP_INTF_PIN_RSDC DCx pin is not defined. Please define in Pin Manager"
#endif

typedef enum
{
    GFX_INTF_DMA_STATUS_NO_TRANSFER = 0,
    GFX_INTF_DMA_STATUS_CMD_TRANSFER,
    GFX_INTF_DMA_STATUS_DATA_TRANSFER
} GFX_INTF_DMA_TRANSFER_STATUS_TYPE;

/** GFX_DISP_INTF_PORTGROUP

  Summary:
    Structure contains status and handles for SPI interface.
    
 */
typedef struct
{
    /* port to use */
    PORT_GROUP port;

    /* interface lock */
    int locked;
    
    volatile GFX_INTF_DMA_TRANSFER_STATUS_TYPE transferStatus;
    uint32_t data_buffer_size;
    uint8_t *data_buffer_ptr;
    uint8_t partial_frames;
    GFX_Disp_Intf_Callback callback;
    void * callback_parm;
} GFX_DISP_INTF_PORTGROUP;

uintptr_t ContextHandle;

GFX_DISP_INTF_PORTGROUP portGroupIntf =
{
        .locked = 0,
        .transferStatus = GFX_INTF_DMA_STATUS_NO_TRANSFER,
        .callback = NULL
};

static void GFX_Disp_Intf_DMATransferCallback(DMAC_TRANSFER_EVENT event,
                                              uintptr_t contextHandle);


GFX_Disp_Intf GFX_Disp_Intf_Open(void)
{   
    GFX_DISP_INTF_PORTGROUP * intf = &portGroupIntf;

    if (intf == NULL || intf->locked == 1)
        return 0;
    
    intf->port = GFX_DISP_INTF_PORT;

    intf->locked = 1;
    
    DMAC_ChannelCallbackRegister(GFX_INTF_DMA_CHANNEL,
                                 GFX_Disp_Intf_DMATransferCallback,
                                 (uintptr_t) &ContextHandle);

    return (GFX_Disp_Intf) intf;
}

static inline bool _GFX_Disp_Intf_Ready(void)
{
    return (portGroupIntf.transferStatus == GFX_INTF_DMA_STATUS_NO_TRANSFER);
}

void GFX_Disp_Intf_Close(GFX_Disp_Intf intf)
{
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL)
        return;

    ((GFX_DISP_INTF_PORTGROUP *) intf)->locked = 0;
}

int GFX_Disp_Intf_PinControl(GFX_Disp_Intf intf, GFX_DISP_INTF_PIN pin, GFX_DISP_INTF_PIN_VALUE value)
{
    int res = -1;
    
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL)
        return -1;
    
    switch(pin)
    {
        case GFX_DISP_INTF_PIN_CS:
#ifdef GFX_DISP_INTF_PIN_CS_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_CS_Clear();
            else
                GFX_DISP_INTF_PIN_CS_Set();
            
            res = 0;
#endif
            break;
        case GFX_DISP_INTF_PIN_WR:
#ifdef GFX_DISP_INTF_PIN_WR_Set
            //Not supported            
            res = -1;           
#endif
            break;
        case GFX_DISP_INTF_PIN_RD:
#ifdef GFX_DISP_INTF_PIN_RD_Set
            //Not supported            
            res = -1;
#endif
            break;
        case GFX_DISP_INTF_PIN_RSDC:
#ifdef GFX_DISP_INTF_PIN_RSDC_Set
            if (value == GFX_DISP_INTF_PIN_CLEAR)
                GFX_DISP_INTF_PIN_RSDC_Clear();
            else
                GFX_DISP_INTF_PIN_RSDC_Set();
            
            res = 0;
#endif            
            break;
        default:
            break;
    }
    
    return res;
}

void GFX_Disp_Intf_StartTransfer(uint8_t * cmd_buffer,
                                 unsigned int cmd_buffer_size,
                                 uint8_t * data_buffer,
                                 unsigned int data_buffer_size)
{
    portGroupIntf.data_buffer_ptr = data_buffer;
    portGroupIntf.partial_frames = 1;
                
    for (uint8_t idx = 2; data_buffer_size > 0xFFFF; idx *= 2)
    {
        data_buffer_size /= 2;
        portGroupIntf.partial_frames = idx;
    }
    
    portGroupIntf.data_buffer_size = data_buffer_size;

    TC1_CompareStart();
    
    if (cmd_buffer != NULL && cmd_buffer_size > 0)
    {
        portGroupIntf.transferStatus = GFX_INTF_DMA_STATUS_CMD_TRANSFER;
        DMAC_ChannelTransfer(GFX_INTF_DMA_CHANNEL,
                        (void*)cmd_buffer,
                        (void*)DATABUS_PTR,
                        cmd_buffer_size);
    }
    else if (data_buffer != NULL && data_buffer_size > 0)
    {
        portGroupIntf.partial_frames--;
        portGroupIntf.transferStatus = GFX_INTF_DMA_STATUS_DATA_TRANSFER;
        DMAC_ChannelTransfer(GFX_INTF_DMA_CHANNEL,
                        (void*)data_buffer,
                        (void*)DATABUS_PTR,
                        data_buffer_size);
    }
}

int GFX_Disp_Intf_WriteCommand(GFX_Disp_Intf intf, uint8_t cmd)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    
    if (ioGroupIntf == NULL ||
        (!_GFX_Disp_Intf_Ready()))
        return -1;

    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_CLEAR);
    //Start Transfer
    GFX_Disp_Intf_StartTransfer(&cmd, 1, NULL, 0);

    while(!_GFX_Disp_Intf_Ready());
        
    return 0;
}

int GFX_Disp_Intf_WriteData(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    
    if (ioGroupIntf == NULL ||
        data == NULL ||
        bytes == 0 ||
        (!_GFX_Disp_Intf_Ready()))
        return -1;
    
    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);
    //Start Transfer
    GFX_Disp_Intf_StartTransfer(NULL, 0, data, bytes);

    while(!_GFX_Disp_Intf_Ready());
    
    return 0;
}

int GFX_Disp_Intf_WriteData16(GFX_Disp_Intf intf, uint16_t * data, int num)
{
    GFX_DISP_INTF_PORTGROUP * ioGroupIntf = (GFX_DISP_INTF_PORTGROUP *) intf;
    
    if ((ioGroupIntf) == NULL ||
        data == NULL ||
        num == 0 ||
        (!_GFX_Disp_Intf_Ready()))
        return -1;

    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);
    //Start Transfer
    GFX_Disp_Intf_StartTransfer(NULL, 0, (uint8_t*) data, num);

    while(!_GFX_Disp_Intf_Ready());
    
    return 0;
}

int GFX_Disp_Intf_ReadData16(GFX_Disp_Intf intf, uint16_t * data, int num)
{
    //Not supported
    return -1;
}

int GFX_Disp_Intf_ReadData(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    //Not supported
    return -1;
}

int GFX_Disp_Intf_ReadCommandData(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * data, int num_data)
{
    //Not supported
    return -1;
}

DRV_GFX_DEPRECATED int GFX_Disp_Intf_WriteCommandParm(GFX_Disp_Intf intf,
                                   uint8_t cmd,
                                   uint8_t * parm,
                                   int num_parms)
{
    int retval;
   
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL ||
        (!_GFX_Disp_Intf_Ready()))
        return -1;
    
    retval = GFX_Disp_Intf_WriteCommand(intf, cmd);
    if (parm != NULL &&
        num_parms > 0 && 
        retval == 0)
    {
        retval = GFX_Disp_Intf_WriteData(intf, parm, num_parms);
    }
    
    return retval;
}

int GFX_Disp_Intf_Write(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL ||
        data == NULL ||
        bytes == 0 ||
        (!_GFX_Disp_Intf_Ready()))
        return -1;

    //Start Transfer
    GFX_Disp_Intf_StartTransfer(NULL, 0, (uint8_t*) data, bytes);

    while(!_GFX_Disp_Intf_Ready());
    
    return 0;
}

int GFX_Disp_Intf_WriteDataByte(GFX_Disp_Intf intf, uint8_t data)
{
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL  ||
        (!_GFX_Disp_Intf_Ready()))
        return -1;

    GFX_DISP_RSDC_Control(intf, GFX_DISP_INTF_PIN_SET);
    
    //Start single byte data write
    GFX_Disp_Intf_StartTransfer(NULL, 0, (uint8_t*) &data, 1);

    while(!_GFX_Disp_Intf_Ready());
    
    return 0;
}

int GFX_Disp_Intf_Read(GFX_Disp_Intf intf, uint8_t * data, int bytes)
{
    //Not supported    
    return -1;
}

int GFX_Disp_Intf_Ready(GFX_Disp_Intf intf)
{
    return _GFX_Disp_Intf_Ready();
}

static void GFX_Disp_Intf_DMATransferCallback(DMAC_TRANSFER_EVENT event,
                                              uintptr_t contextHandle)
{    
    switch(portGroupIntf.transferStatus)
    {
        default:
        case GFX_INTF_DMA_STATUS_NO_TRANSFER:
        {
            break;
        }
        case GFX_INTF_DMA_STATUS_CMD_TRANSFER:
        {
            if (portGroupIntf.data_buffer_ptr == NULL)
            {
                portGroupIntf.transferStatus = GFX_INTF_DMA_STATUS_NO_TRANSFER;

                TC1_CompareStop();

                if (portGroupIntf.callback != NULL)
                {
                    portGroupIntf.callback((GFX_Disp_Intf) &portGroupIntf,
                                           GFX_DISP_INTF_TX_DONE,
                                           portGroupIntf.callback_parm);
                }


            }
            else
            {
                /* Set D/C pin output value high to prepare data transfer */
                GFX_DISP_INTF_PIN_RSDC_Set();

                portGroupIntf.partial_frames--;
                DMAC_ChannelTransfer(GFX_INTF_DMA_CHANNEL, (void*)(portGroupIntf.data_buffer_ptr), (void*)DATABUS_PTR, portGroupIntf.data_buffer_size); 
                portGroupIntf.transferStatus = GFX_INTF_DMA_STATUS_DATA_TRANSFER;
            }
            break;
        }
        case GFX_INTF_DMA_STATUS_DATA_TRANSFER:
        {
            if (portGroupIntf.partial_frames > 0)
            {
                portGroupIntf.partial_frames--;
                portGroupIntf.data_buffer_ptr += portGroupIntf.data_buffer_size;
                DMAC_ChannelTransfer(GFX_INTF_DMA_CHANNEL, (void*)(portGroupIntf.data_buffer_ptr), (void*)DATABUS_PTR, portGroupIntf.data_buffer_size); 
            }
            else
            {                
                portGroupIntf.transferStatus = GFX_INTF_DMA_STATUS_NO_TRANSFER;

                TC1_CompareStop();

                if (portGroupIntf.callback != NULL)
                {
                    portGroupIntf.callback((GFX_Disp_Intf) &portGroupIntf,
                                           GFX_DISP_INTF_TX_DONE,
                                           portGroupIntf.callback_parm);
                }
            }
            break;
        }
    }
}

int GFX_Disp_Intf_Set_Callback(GFX_Disp_Intf intf, GFX_Disp_Intf_Callback cb, void * parm)
{
    if (((GFX_DISP_INTF_PORTGROUP *) intf) == NULL)
        return -1;

    ((GFX_DISP_INTF_PORTGROUP *) intf)->callback = cb;
    ((GFX_DISP_INTF_PORTGROUP *) intf)->callback_parm = parm;

    return 0;
}


/* *****************************************************************************
 End of File
 */
