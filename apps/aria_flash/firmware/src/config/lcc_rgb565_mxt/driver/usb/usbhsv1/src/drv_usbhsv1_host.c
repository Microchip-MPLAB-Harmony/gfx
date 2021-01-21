/*******************************************************************************
  USB Host Controller Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usbhsv1_host.c

  Summary:
    USB Host Driver Implementation for PIC32C.

  Description:
    This file implements the Host mode operation of the High Speed USB Driver.
    This file should be included in the application if USB Host mode operation
    is desired.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
//DOM-IGNORE-END

#include "configuration.h"
#include "driver/usb/drv_usb.h"
#include "driver/usb/usbhsv1/drv_usbhsv1.h"
#include "driver/usb/usbhsv1/src/drv_usbhsv1_local.h"
#include "usb/usb_host.h"
#include "usb/usb_host_client_driver.h"


/**********************************************************
 * This structure is a set of pointer to the USBFS driver
 * functions. It is provided to the host and device layer
 * as the interface to the driver.
 * *******************************************************/

DRV_USB_HOST_INTERFACE gDrvUSBHSV1HostInterface =
{
    .open = DRV_USBHSV1_Open,
    .close = DRV_USBHSV1_Close,
    .eventHandlerSet = DRV_USBHSV1_ClientEventCallBackSet,
    .hostIRPSubmit = DRV_USBHSV1_HOST_IRPSubmit,
    .hostIRPCancel = DRV_USBHSV1_HOST_IRPCancel,
    .hostPipeSetup = DRV_USBHSV1_HOST_PipeSetup,
    .hostPipeClose = DRV_USBHSV1_HOST_PipeClose,
    .hostEventsDisable = DRV_USBHSV1_HOST_EventsDisable,
	.endpointToggleClear = DRV_USBHSV1_HOST_EndpointToggleClear,
    .hostEventsEnable = DRV_USBHSV1_HOST_EventsEnable,
    .rootHubInterface.rootHubPortInterface.hubPortReset = DRV_USBHSV1_HOST_ROOT_HUB_PortReset,
    .rootHubInterface.rootHubPortInterface.hubPortSpeedGet = DRV_USBHSV1_HOST_ROOT_HUB_PortSpeedGet,
    .rootHubInterface.rootHubPortInterface.hubPortResetIsComplete = DRV_USBHSV1_HOST_ROOT_HUB_PortResetIsComplete,
    .rootHubInterface.rootHubPortInterface.hubPortSuspend = DRV_USBHSV1_HOST_ROOT_HUB_PortSuspend,
    .rootHubInterface.rootHubPortInterface.hubPortResume = DRV_USBHSV1_HOST_ROOT_HUB_PortResume,
    .rootHubInterface.rootHubMaxCurrentGet = DRV_USBHSV1_HOST_ROOT_HUB_MaximumCurrentGet,
    .rootHubInterface.rootHubPortNumbersGet = DRV_USBHSV1_HOST_ROOT_HUB_PortNumbersGet,
    .rootHubInterface.rootHubSpeedGet = DRV_USBHSV1_HOST_ROOT_HUB_BusSpeedGet,
    .rootHubInterface.rootHubInitialize = DRV_USBHSV1_HOST_ROOT_HUB_Initialize,
    .rootHubInterface.rootHubOperationEnable = DRV_USBHSV1_HOST_ROOT_HUB_OperationEnable,
    .rootHubInterface.rootHubOperationIsEnabled = DRV_USBHSV1_HOST_ROOT_HUB_OperationIsEnabled,
};

/*****************************************
 * Pool of pipe objects that is used by
 * all driver instances.
 *****************************************/
DRV_USBHSV1_HOST_PIPE_OBJ gDrvUSBHostPipeObj[10];

/****************************************
 * The driver object
 ****************************************/
extern DRV_USBHSV1_OBJ gDrvUSBObj[];

// ****************************************************************************
// ****************************************************************************
// Local Functions
// ****************************************************************************
// ****************************************************************************

void _DRV_USBHSV1_HOST_AttachDetachStateMachine (DRV_USBHSV1_OBJ * hDriver)
{
    /* In the host mode, we perform attach de-bouncing */
    
    bool interruptWasEnabled;
    
    switch(hDriver->attachState)
    {
        case DRV_USBHSV1_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH:
            /* If no device is attached, then there is nothing to do
             * If device is attached, then move to next state */
            if(hDriver->deviceAttached)
            {
                hDriver->attachState = DRV_USBHSV1_HOST_ATTACH_STATE_DETECTED;
            }

            break;
            
        case DRV_USBHSV1_HOST_ATTACH_STATE_DETECTED:
                /* Disable the driver interrupt as
                 * we do not want this section to be interrupted. */
                interruptWasEnabled = _DRV_USBHSV1_InterruptSourceDisable(hDriver->interruptSource);

                if(hDriver->deviceAttached)
                {
                    /* Yes the device is still attached. Enumerate
                     * this device. usbHostDeviceInfo is the ID of
                     * this root hub. */
                    hDriver->attachedDeviceObjHandle = USB_HOST_DeviceEnumerate(hDriver->usbHostDeviceInfo, 0);
                    hDriver->attachState = DRV_USBHSV1_HOST_ATTACH_STATE_READY;

                }
                else
                {
                    /* The device is not attached any more. This was a false attach 
                     */
                    hDriver->attachState = DRV_USBHSV1_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH;
                }

                if(interruptWasEnabled)
                {
                    /* Re-enable the interrupt if it was originally
                     * enabled. */
                    _DRV_USBHSV1_InterruptSourceEnable(hDriver->interruptSource);
                }

            break;

        case DRV_USBHSV1_HOST_ATTACH_STATE_READY:

            /* De-bouncing is done and device ready. We can check
             * here if the device is detached */
            if(!hDriver->deviceAttached)
            {
                /* Device is not attached */
                hDriver->attachState = DRV_USBHSV1_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH;
            }
            break;

        default:
            break;
    }
}

void _DRV_USBHSV1_HOST_ResetStateMachine(DRV_USBHSV1_OBJ * hDriver)
{
    /* Check if reset is needed */
    switch(hDriver->resetState)
    {
        case DRV_USBHSV1_HOST_RESET_STATE_NO_RESET:

            /* No reset signaling is request */
        break;

        case DRV_USBHSV1_HOST_RESET_STATE_START:
            /* Trigger USB Reset */
            hDriver->usbID->USBHS_HSTCTRL |= USBHS_HSTCTRL_RESET_Msk;
            hDriver->resetState = DRV_USBHSV1_HOST_RESET_STATE_WAIT_FOR_COMPLETE;
        break;

        case DRV_USBHSV1_HOST_RESET_STATE_WAIT_FOR_COMPLETE:

            /* Check if the reset has completed */
            if(USBHS_HSTCTRL_RESET_Msk != (USBHS_HSTCTRL_RESET_Msk & hDriver->usbID->USBHS_HSTCTRL))
            {
                /* Reset has completed */
                hDriver->resetState = DRV_USBHSV1_HOST_RESET_STATE_NO_RESET;

                /* Clear the flag */
                hDriver->isResetting = false;

                /* Now that reset is complete, we can find out the
                 * speed of the attached device. */
                switch ((USBHS_SR_SPEED_Msk & hDriver->usbID->USBHS_SR) >> USBHS_SR_SPEED_Pos)
                {
                    case 0x0:
                        hDriver->deviceSpeed = USB_SPEED_FULL;
                    break;
                    case 0x1:
                        hDriver->deviceSpeed = USB_SPEED_HIGH;
                    break;
                    case 0x2:
                        hDriver->deviceSpeed = USB_SPEED_LOW;
                    break;    
                    default:
                    break;
                }
            }
        break;

        default:
        break;
    }
}

void _DRV_USBHSV1_HOST_IRPTransmitFIFOLoad
(
    volatile usbhs_registers_t * usbID, 
    USB_HOST_IRP_LOCAL * irp,
    uint8_t hPipe
)
{
    /* This function will copy data from the irp to the fifo
     * base on the number of bytes that were completed and
     * then trigger the transmit */

    uint8_t *data, *ptrEPData;
    unsigned int count, pendingBytes;
    DRV_USBHSV1_HOST_PIPE_OBJ * pipe = (DRV_USBHSV1_HOST_PIPE_OBJ *)(irp->pipe);
    
    /* Load the FIFO */
    pendingBytes = irp->size - irp->completedBytes;
    count = (pendingBytes > pipe->endpointSize) ? pipe->endpointSize : pendingBytes;
    data = (uint8_t *)((uint8_t *)irp->data + irp->completedBytes);
    ptrEPData = (uint8_t *) &drv_usbhsv1_get_pipe_fifo_access(hPipe);
   
    if(0 == hPipe)
    {
        /* Configure OUT Token for Pipe 0 */
        usbID->USBHS_HSTPIPCFG[0] = (usbID->USBHS_HSTPIPCFG[0] & ~(uint32_t)(USBHS_HSTPIPCFG_PTOKEN_Msk))
                | USBHS_HSTPIPCFG_PTOKEN(2);
    }
    __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
    __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
    /* Load the endpoint FIFO with the user data */
    for(uint16_t i = 0; i < count; i ++)
    {
        *ptrEPData++ = *data++;
    }
                
    /* Check if the cache is enabled */
    if( (SCB->CCR & SCB_CCR_DC_Msk) == SCB_CCR_DC_Msk)
    {  
        ptrEPData = (uint8_t *) &drv_usbhsv1_get_pipe_fifo_access(hPipe);

        SCB_CleanDCache_by_Addr((uint32_t *) ptrEPData, count);
    }
    
    /* Update the irp with the byte count loaded */
    irp->completedBytes += count;

    /* Clear the TXOUT interrupt */
    usbID->USBHS_HSTPIPICR[hPipe] = USBHS_HSTPIPICR_TXOUTIC_Msk;    
    /* Enable Pipe out ready interrupt */
    usbID->USBHS_HSTPIPIER[hPipe] = USBHS_HSTPIPIER_TXOUTES_Msk;
    __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
    __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
    /* Clear FIFOCON and Unfreeze pipe */
    usbID->USBHS_HSTPIPIDR[hPipe] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
    usbID->USBHS_HSTPIPIDR[hPipe] = USBHS_HSTPIPIDR_PFREEZEC_Msk;

}/* end of _DRV_USBHSV1_HOST_IRPTransmitFIFOLoad() */

void _DRV_USBHSV1_HOST_IRPTransmitSetupPacket
(
    volatile usbhs_registers_t * usbID,
    USB_HOST_IRP_LOCAL * irp
)
{
    /* This function will load the irp setup packet into 
     * pipe0 FIFO and then transmit the setup packet. */

    uint8_t * data = (uint8_t *)irp->setup;
    volatile uint8_t *ptrEPData;
    
    /* Configure Setup Token for Pipe 0 */
    usbID->USBHS_HSTPIPCFG[0] = (usbID->USBHS_HSTPIPCFG[0] & ~(uint32_t)(USBHS_HSTPIPCFG_PTOKEN_Msk))
            | USBHS_HSTPIPCFG_PTOKEN(0);
    __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
    __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
    /* Clear Setup Ready interrupt */
    usbID->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_TXSTPIC_Msk;
    ptrEPData =   (volatile uint8_t *)&drv_usbhsv1_get_pipe_fifo_access(0);

   /* Load the endpoint FIFO with the user data */
    for(uint16_t i = 0; i < 8; i ++)
    {
        *ptrEPData++ = *data++;
    }
                
    /* Check if the cache is enabled */
    if( (SCB->CCR & SCB_CCR_DC_Msk) == SCB_CCR_DC_Msk)
    {  
        ptrEPData = (uint8_t *) &drv_usbhsv1_get_pipe_fifo_access(0);

        SCB_CleanDCache_by_Addr((uint32_t *) ptrEPData, 8);
    }
   
    /* Enable setup ready interrupt */
    usbID->USBHS_HSTPIPIER[0] = USBHS_HSTPIPIER_TXSTPES_Msk;
    __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
    __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
    /* Clear FIFOCON and Unfreeze pipe */
    usbID->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
    usbID->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_PFREEZEC_Msk;

}/* end of _DRV_USBHSV1_HOST_IRPTransmitSetupPacket() */

unsigned int _DRV_USBHSV1_HOST_IRPReceiveFIFOUnload 
(
    volatile usbhs_registers_t * usbID,
    USB_HOST_IRP_LOCAL * irp,
    uint8_t hPipe,
    bool * pisDMAUsed
)
{
    /* This function will recover the count of the received data/
     * and then unload the pipe FIFO. */

    uint32_t  count;
    uint8_t * data;
    uint8_t * ptrEPData;

    /* Copy the data from the FIFO0 to the application
     * buffer and then update the complete byte count
     * and clear the RX packet ready bit */
    ptrEPData = (uint8_t *) &drv_usbhsv1_get_pipe_fifo_access(hPipe);
    data = (uint8_t *)((uint8_t *)irp->data + irp->completedBytes);
    /* Get byte count to read data */
    count = (USBHS_HSTPIPISR_PBYCT_Msk & usbID->USBHS_HSTPIPISR[hPipe]) >> USBHS_HSTPIPISR_PBYCT_Pos;

    /* Check if the cache is enabled */
    if( (SCB->CCR & SCB_CCR_DC_Msk) == SCB_CCR_DC_Msk)
    {  
        SCB_InvalidateDCache_by_Addr((uint32_t *) ptrEPData, count);
    }

    for(uint16_t i = 0; i < count; i ++)
    {
        *data++ = *ptrEPData++;
    }
    irp->completedBytes += count;
    *pisDMAUsed = false;
    __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
    __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
    /* Clear FIFO Status */
    usbID->USBHS_HSTPIPIDR[hPipe] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
    return (count);
}/* end of _DRV_USBHSV1_HOST_IRPReceiveFIFOUnload() */

void _DRV_USBHSV1_HOST_Initialize
(
    DRV_USBHSV1_OBJ * drvObj, 
    SYS_MODULE_INDEX index
)
{
    volatile usbhs_registers_t * usbMod = drvObj->usbID;
   
    /* No device attached */
    drvObj->deviceAttached = false;
    /* Initialize the device handle */
    drvObj->attachedDeviceObjHandle = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;
    /* Set VBUS Hardware Control
     * VBUSHWC: VBUS Hardware Control Must be set to ?1?.
     * UID: UID Pin Enable Must be set to ?0?. */
    usbMod->USBHS_CTRL = USBHS_CTRL_USBE_Msk | USBHS_CTRL_VBUSHWC_Msk;
    /* Requests VBus activation: Must be set to ?1?. */
    usbMod->USBHS_SFR |= USBHS_SFR_VBUSRQS_Msk;

    /* Initialize the host specific members in the driver object */
    drvObj->isResetting     = false;
    drvObj->usbHostDeviceInfo = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;
    drvObj->operationEnabled = false;
}/* end of _DRV_USBHSV1_HOST_Initialize() */

USB_ERROR DRV_USBHSV1_HOST_IRPSubmit
(
    DRV_USBHSV1_HOST_PIPE_HANDLE  hPipe, 
    USB_HOST_IRP * inputIRP
)
{
    USB_HOST_IRP_LOCAL * irpIterator;
    DRV_USBHSV1_HOST_TRANSFER_GROUP * controlTransferGroup;
    bool interruptWasEnabled = false;
    unsigned int direction;
    uint8_t hostPipe;
	

    USB_HOST_IRP_LOCAL * irp        = (USB_HOST_IRP_LOCAL *)inputIRP;
    DRV_USBHSV1_HOST_PIPE_OBJ * pipe = (DRV_USBHSV1_HOST_PIPE_OBJ *)(hPipe);
    DRV_USBHSV1_OBJ * hDriver;
    volatile usbhs_registers_t * usbMod;
    USB_ERROR returnValue = USB_ERROR_PARAMETER_INVALID;
	SYS_TIME_HANDLE timer;

    if((pipe == NULL) || (hPipe == (DRV_USBHSV1_HOST_PIPE_HANDLE_INVALID)))
    {
        /* This means an invalid pipe was specified.  Return with an error */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Pipe handle is not valid");
    }
    else
    {
        hDriver = (DRV_USBHSV1_OBJ *)(pipe->hClient);
        usbMod = hDriver->usbID;
        controlTransferGroup = &hDriver->controlTransferGroup;

        /* Assign owner pipe */
        irp->pipe = hPipe;
        irp->status = USB_HOST_IRP_STATUS_PENDING;
        irp->tempState = DRV_USBHSV1_HOST_IRP_STATE_PROCESSING;
        hostPipe = pipe->hostPipeN;
        direction = (pipe->endpointAndDirection & 0x80) >> 7;

        /* We need to disable interrupts was the queue state
         * does not change asynchronously */

        if(!hDriver->isInInterruptContext)
        {
            if(OSAL_MUTEX_Lock(&(hDriver->mutexID), OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
            {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Mutex lock failed in DRV_USBHSV1_HOST_IRPSubmit()");
                    returnValue = USB_ERROR_OSAL_FUNCTION;
            }
            else
            {
                interruptWasEnabled = _DRV_USBHSV1_InterruptSourceDisable(hDriver->interruptSource);
            }
        }

        if(USB_ERROR_OSAL_FUNCTION != returnValue)
        {
           /* This needs to be done for all irp irrespective
             * of type or if there IRP is immediately processed */

            irp->next = NULL;
            irp->completedBytes = 0;
            irp->status = USB_HOST_IRP_STATUS_PENDING;

            if(pipe->irpQueueHead == NULL)
            {
                /* This means that there are no IRPs on this pipe. We can add
                 * this IRP directly */

                irp->previous = NULL;
                pipe->irpQueueHead = irp;

                if(pipe->pipeType == USB_TRANSFER_TYPE_CONTROL)
                {
					/* USB Host stack must give enough time for the attached 
					 * USB Device to be ready to receive data on the Control
					 * Endpoint. The below code waits for 50 micro seconds  
					 * before scheduling a control transfer */   
					if ( pipe->speed == USB_SPEED_HIGH )
                    {
                        if (SYS_TIME_DelayUS( 50, &timer) != SYS_TIME_SUCCESS)
                        {
                            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Handle error ");
                        }
                        else if (SYS_TIME_DelayIsComplete(timer) != true)
                        {
                            /* Wait till the delay expired */
                            while (SYS_TIME_DelayIsComplete(timer) == false);
                        }
                    }
					
                    /* Set the initial stage of the IRP */
                    irp->tempState = DRV_USBHSV1_HOST_IRP_STATE_SETUP_STAGE;

                    /* We need to update the flags parameter of the IRP to
                     * indicate the direction of the control transfer. */

                    if(*((uint8_t*)(irp->setup)) & 0x80)
                    {
                        /* This means the data stage moves from device to host.
                         * Set bit 15 of the flags parameter */

                        irp->flags |= 0x80;
                    }
                    else
                    {
                        /* This means the data stage moves from host to device.
                         * Clear bit 15 of the flags parameter. */

                        irp->flags &= 0x7F;
                    }

                    /* We need to check if the endpoint 0 is free and if so
                     * then start processing the IRP */

                    if(controlTransferGroup->currentIRP == NULL)
                    {
                        /* This means that no IRPs are being processed
                         * So we should start the IRP processing. Else
                         * the IRP processing will start in interrupt.
                         * We start by copying the setup command */

                        controlTransferGroup->currentIRP = irp;
                        controlTransferGroup->currentPipe = pipe;
                        irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;

                        /* Send the setup packet to device */
                        _DRV_USBHSV1_HOST_IRPTransmitSetupPacket(usbMod, irp);
                    }
                }
                else
                {
                    /* For non control transfers, if this is the first irp in
                     * the queue, then we can start the irp */

                    irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;

                    if(USB_DATA_DIRECTION_HOST_TO_DEVICE == direction)
                    {
                        /* Data is moving from host to device. We need to copy
                         * data into the FIFO and then and set the TX request
                         * bit. If the IRP size is greater than endpoint size
                         * then we must packetize. */

                        /* Clear Tx Out Ready Interrupt */
                        usbMod->USBHS_HSTPIPICR[hostPipe] = USBHS_HSTPIPICR_TXOUTIC_Msk;
                        _DRV_USBHSV1_HOST_IRPTransmitFIFOLoad(usbMod, irp, hostPipe);
                    }
                    else
                    {
                        /* Data is moving from device to host
                         * We need to set the Rx Packet Request
                         * bit */

                        /* Perform 1 IN requests before freezing the pipe. */
                        usbMod->USBHS_HSTPIPINRQ[hostPipe] = 0;
                        /* Clear RX IN Interrupt */
                        usbMod->USBHS_HSTPIPICR[hostPipe] = USBHS_HSTPIPICR_RXINIC_Msk;
                        /* Enable Rx IN Interrupt */
                        usbMod->USBHS_HSTPIPIER[hostPipe] = USBHS_HSTPIPIER_RXINES_Msk;
                        __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                        __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                        /* Clear FIFOCON and Unfreeze pipe */
                        usbMod->USBHS_HSTPIPIDR[hostPipe] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
                        usbMod->USBHS_HSTPIPIDR[hostPipe] = USBHS_HSTPIPIDR_PFREEZEC_Msk;
                    }
                }
            }
            else
            {
                /* We need to add the irp to the last irp
                * in the pipe queue (which is a linked list) */
               irpIterator = pipe->irpQueueHead;

               /* Find the last IRP in the linked list*/
               while(irpIterator->next != 0)
               {
                   irpIterator = irpIterator->next;
               }

               /* Add the item to the last irp in the linked list */
               irpIterator->next = irp;
               irp->previous = irpIterator;
            }

            if(!hDriver->isInInterruptContext)
            {
                if(interruptWasEnabled)
                {
                    _DRV_USBHSV1_InterruptSourceEnable(hDriver->interruptSource);
                }

                if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Mutex unlock failed");
                }
            }
            returnValue = USB_ERROR_NONE;
        }
    }

    return (returnValue);
}/* end of DRV_USBHSV1_HOST_IRPSubmit() */

void DRV_USBHSV1_HOST_IRPCancel
(
    USB_HOST_IRP * inputIRP
)
{
    /* This function cancels an IRP */

    USB_HOST_IRP_LOCAL * irp = (USB_HOST_IRP_LOCAL *) inputIRP;
    DRV_USBHSV1_OBJ * hDriver;
    DRV_USBHSV1_HOST_PIPE_OBJ * pipe;
    bool interruptWasEnabled = false;

    if(irp->pipe == DRV_USBHSV1_HOST_PIPE_HANDLE_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Invalid pipe");
    }
    else if(irp->status <= USB_HOST_IRP_STATUS_COMPLETED_SHORT)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: IRP is not pending or in progress");
    }
    else
    {
        pipe = (DRV_USBHSV1_HOST_PIPE_OBJ *)irp->pipe;
        hDriver = (DRV_USBHSV1_OBJ *) pipe->hClient;

        if(!hDriver->isInInterruptContext)
        {
            if(OSAL_MUTEX_Lock(&(hDriver->mutexID), OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Mutex lock failed");
            }
            interruptWasEnabled = _DRV_USBHSV1_InterruptSourceDisable(hDriver->interruptSource);
        }

        if(irp->previous == NULL)
        {
            /* This means this was the first irp in the queue. Update the pipe
             * queue head directly */

            pipe->irpQueueHead = irp->next;
             if(irp->next != NULL)
            {
                irp->next->previous = NULL;
            }
        }
        else
        {
            /* Remove the IRP from the linked list */
            irp->previous->next = irp->next;

            if(irp->next != NULL)
            {
                /* This applies if this is not the last
                 * irp in the linked list */
                irp->next->previous = irp->previous;
            }
        }

        if(irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)
        {
            /* If the irp is already in progress then
             * we set the temporary state. This will get
             * caught in _DRV_USBHSV1_HOST_ControlXferProcess()
             * and _DRV_USBHSV1_HOST_NonControlIRPProcess() functions. */

            irp->tempState = DRV_USBHSV1_HOST_IRP_STATE_ABORTED;
        }
        else
        {
            irp->status = USB_HOST_IRP_STATUS_ABORTED;
            if(irp->callback != NULL)
            {
                irp->callback((USB_HOST_IRP *)irp);
            }
        }

        if(!hDriver->isInInterruptContext)
        {
            if(interruptWasEnabled)
            {
                _DRV_USBHSV1_InterruptSourceEnable(hDriver->interruptSource);
            }

            if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Mutex unlock failed");
            }
        }
    }
}/* end of DRV_USBHSV1_HOST_IRPCancel() */

void DRV_USBHSV1_HOST_PipeClose
(
    DRV_USBHSV1_HOST_PIPE_HANDLE pipeHandle
)
{
    /* This function closes an open pipe */

    bool interruptWasEnabled = false;

    DRV_USBHSV1_OBJ * hDriver;
    USB_HOST_IRP_LOCAL  * irp;
    DRV_USBHSV1_HOST_PIPE_OBJ       * pipe;
    DRV_USBHSV1_HOST_TRANSFER_GROUP * transferGroup;
    DRV_USBHSV1_HOST_ENDPOINT_OBJ   * endpointObj;
    volatile usbhs_registers_t * usbMod;

    /* Make sure we have a valid pipe */
    if( ( pipeHandle == 0 )  || pipeHandle == DRV_USBHSV1_HOST_PIPE_HANDLE_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Invalid pipe handle");
    }
    else
    {
        pipe = (DRV_USBHSV1_HOST_PIPE_OBJ*) pipeHandle;

        /* Make sure that we are working with a pipe in use */
        if(pipe->inUse != true)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Pipe is not in use");
        }
        else
        {
            hDriver = (DRV_USBHSV1_OBJ *)pipe->hClient;
            usbMod = hDriver->usbID;

            /* Disable the interrupt */

            if(!hDriver->isInInterruptContext)
            {
                if(OSAL_MUTEX_Lock(&hDriver->mutexID, OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Mutex lock failed");
                }
                interruptWasEnabled = _DRV_USBHSV1_InterruptSourceDisable(hDriver->interruptSource);
            }

            if(USB_TRANSFER_TYPE_CONTROL == pipe->pipeType)
            {
                transferGroup = &hDriver->controlTransferGroup;

                if(pipe->previous == NULL)
                {
                    /* The previous pipe could be null if this was the first pipe in the
                     * transfer group */

                    transferGroup->pipe = pipe->next;
                    if(pipe->next != NULL)
                    {
                        pipe->next->previous = NULL;
                    }
                }
                else
                {
                    /* Remove this pipe from the linked list */

                    pipe->previous->next = pipe->next;
                    if(pipe->next != NULL)
                    {
                        pipe->next->previous = pipe->previous;
                    }
                }

                if(transferGroup->nPipes != 0)
                {
                    /* Reduce the count only if its not zero already */

                    transferGroup->nPipes --;
                }
            }
            else
            {
                /* Non control transfer pipes are not stored as groups.  We deallocate
                 * the endpoint object that this pipe used */

                endpointObj = &hDriver->hostEndpointTable[pipe->hostPipeN];
                endpointObj->endpoint.inUse = false;
                endpointObj->endpoint.pipe = NULL;

                /* Clear the error status */
                usbMod->USBHS_HSTPIPERR[pipe->hostPipeN] = 0;
            }

            /* Now we invoke the call back for each IRP in this pipe and say that it is
             * aborted.  If the IRP is in progress, then that IRP will be actually
             * aborted on the next SOF This will allow the USB module to complete any
             * transaction that was in progress. */

            irp = (USB_HOST_IRP_LOCAL *)pipe->irpQueueHead;
            while(irp != NULL)
            {
                irp->pipe = DRV_USBHSV1_HOST_PIPE_HANDLE_INVALID;

                if(irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)
                {
                    /* If the IRP is in progress, then we set the temp IRP state. This
                     * will be caught in the _DRV_USBHSV1_HOST_NonControlTransferProcess() and
                     * _DRV_USBHSV1_HOST_ControlTransferProcess() functions */

                    irp->status = USB_HOST_IRP_STATUS_ABORTED;

                    if(irp->callback != NULL)
                    {
                        irp->callback((USB_HOST_IRP*)irp);
                    }
                    irp->tempState = DRV_USBHSV1_HOST_IRP_STATE_ABORTED;
                }
                else
                {
                    /* IRP is pending */
                    irp->status = USB_HOST_IRP_STATUS_ABORTED;

                    if(irp->callback != NULL)
                    {
                        irp->callback((USB_HOST_IRP*)irp);
                    }
                }
                irp = irp->next;
            }

            /* Now we return the pipe back to the driver */
            pipe->inUse = false ;

            /* Enable the interrupts */
            if(!hDriver->isInInterruptContext)
            {
                if(interruptWasEnabled)
                {
                    _DRV_USBHSV1_InterruptSourceEnable(hDriver->interruptSource);
                }

                if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Mutex unlock failed");
                }
            }
        }
    }
}/* end of DRV_USBHSV1_HOST_PipeClose() */

DRV_USBHSV1_HOST_PIPE_HANDLE DRV_USBHSV1_HOST_PipeSetup
(
    DRV_HANDLE client,
    uint8_t deviceAddress,
    USB_ENDPOINT endpointAndDirection,
    uint8_t hubAddress,
    uint8_t hubPort,
    USB_TRANSFER_TYPE pipeType,
    uint8_t bInterval,
    uint16_t wMaxPacketSize,
    USB_SPEED speed
)
{
    bool epFound = false;
    uint8_t endpoint;
    unsigned int epDirection;
    int pipeIter = 0;
    uint32_t hstPipeCfg;

    DRV_USBHSV1_OBJ * hDriver;
    DRV_USBHSV1_HOST_PIPE_OBJ * pipe = NULL;
    DRV_USBHSV1_HOST_PIPE_HANDLE pipeHandle = DRV_USBHSV1_HOST_PIPE_HANDLE_INVALID;
  
    if((client == DRV_HANDLE_INVALID) || (((DRV_USBHSV1_OBJ *)client) == NULL))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Invalid client handle");
    }

    else if(((DRV_USBHSV1_OBJ *)client)->inUse)
    {
        if((speed == USB_SPEED_LOW) || (speed == USB_SPEED_FULL) || (speed == USB_SPEED_HIGH))
        {
            /* wMaxPacketSize can be less than 8 for Non control endpoints. E.g.
             * Interrupt IN endpoints can be 4 bytes for HID Mouse device. This
             * USB module requires minimum 2 to the power 3 i.e. 8 bytes as the
             * wMaxPacketSize.
             * 
             * For Control endpoints the minimum has to be 8 bytes.  */

            if(pipeType != USB_TRANSFER_TYPE_CONTROL)
            {
                if(wMaxPacketSize < 8)
                {
                    wMaxPacketSize = 8;
                }
            }

            if((wMaxPacketSize < 8) ||(wMaxPacketSize > 4096))
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Invalid pipe endpoint size");
            }
            else
            {

                hDriver = (DRV_USBHSV1_OBJ *)client;
                endpoint = (endpointAndDirection & 0x0F);
                epDirection = (endpointAndDirection & 0x80) >> 7;


                if(OSAL_MUTEX_Lock(&hDriver->mutexID, OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Mutex lock failed");
                }
                else
                {
                    if(pipeType == USB_TRANSFER_TYPE_CONTROL)
                    {
                        /* Set pipeIter to zero to indicate that we must use pipe 0
                         * for control transfers. We also add the control transfer pipe
                         * to the control transfer group. */
                        pipeIter = 0;

                        if (wMaxPacketSize >= 8)
                        {
                            /* Reset Pipe */
                            Set_bits(hDriver->usbID->USBHS_HSTPIP, ((1 << 16) << (pipeIter)));
                            Clr_bits(hDriver->usbID->USBHS_HSTPIP, ((1 << 16) << (pipeIter)));

                            /* Enable Pipe */
                            Set_bits(hDriver->usbID->USBHS_HSTPIP, (1 << (pipeIter)));
                            /* Configure Pipe */
                            hstPipeCfg = ((0 << 2)|((drv_usbhsv1_format_pipe_size(wMaxPacketSize) & 7) << 4)|
                                          (0 << 8)|(0 << 10)|(0 << 12)|(pipeIter << 16)|(0 << 24));

                            /* Enable PINGEN for HS Control Pipe */ 
                            if((USB_TRANSFER_TYPE_CONTROL == pipeType) && \
                               (USB_SPEED_HIGH == hDriver->deviceSpeed))
                            {
                                hstPipeCfg |= (1<<20);
                            }
        
                            /* Allocate the Pipe Memory */
                            hstPipeCfg |= (USBHS_HSTPIPCFG_ALLOC_Msk);
                            hDriver->usbID->USBHS_HSTPIPCFG[pipeIter]  = hstPipeCfg;

                            /* Check if Pipe configuration status is OK */
                            if(USBHS_HSTPIPISR_CFGOK_Msk != (USBHS_HSTPIPISR_CFGOK_Msk & hDriver->usbID->USBHS_HSTPIPISR[pipeIter]))
                            {
                                /* Disable Pipe */
                                Clr_bits(hDriver->usbID->USBHS_HSTPIP, (1 << (pipeIter)));
                            }
                            else
                            {
                                /* Configure address of Pipe */
                                Wr_bitfield((&hDriver->usbID->USBHS_HSTADDR1)[(pipeIter)>>2], 0x7F << (((pipeIter)&0x03)<<3), deviceAddress);

                                /* Always enable stall and error interrupts of control endpoint */
                                /* Enable Stall Interrupt */
                                hDriver->usbID->USBHS_HSTPIPIER[pipeIter] = USBHS_HSTPIPIER_RXSTALLDES_Msk;
                                /* Enable Pipe Error Interrupt */
                                hDriver->usbID->USBHS_HSTPIPIER[pipeIter] = USBHS_HSTPIPIER_PERRES_Msk;
                                /* Enable Pipe Interrupt */
                                Set_bits(hDriver->usbID->USBHS_HSTIER, ((1 << 8) << (pipeIter)));
                                epFound = true;
                                pipe = &gDrvUSBHostPipeObj[pipeIter];
                            }
                        }
                    }
                    else
                    {
                        /* Pipe allocation for non-control transfer */
                        for(pipeIter = 1; pipeIter < 10; pipeIter ++)
                        {
                            if((false == gDrvUSBHostPipeObj[pipeIter].inUse) && \
                               (false == Tst_bits(hDriver->usbID->USBHS_HSTPIP, (1 << (pipeIter)))))
                            {
                                /* This means we have found a free pipe object */
                                uint16_t ep_dir;
                                ep_dir = (epDirection == USB_DATA_DIRECTION_DEVICE_TO_HOST)?(0x1):(0x2);
                                /* Reset Pipe*/
                                Set_bits(hDriver->usbID->USBHS_HSTPIP, ((1 << 16) << (pipeIter)));
                                Clr_bits(hDriver->usbID->USBHS_HSTPIP, ((1 << 16) << (pipeIter)));

                                /* Enable Pipe */
                                Set_bits(hDriver->usbID->USBHS_HSTPIP, (1 << (pipeIter)));

                                /* Configure Pipe */
                                hstPipeCfg = ((0 << 2)|((drv_usbhsv1_format_pipe_size(wMaxPacketSize) & 7) << 4)|
                                (ep_dir << 8)|(0 << 10)|(pipeType << 12)|(endpoint << 16)|(bInterval << 24));

                                /* Enable PINGEN for HS BULK-OUT Pipe */ 
                                if((0x2 == ep_dir) && (USB_TRANSFER_TYPE_BULK == pipeType) && \
                                   (USB_SPEED_HIGH == hDriver->deviceSpeed))
                                {
                                    hstPipeCfg |= (1<<20);
                                }

                                /* Allocate the Pipe Memory */
                                hstPipeCfg |= (USBHS_HSTPIPCFG_ALLOC_Msk);
                                hDriver->usbID->USBHS_HSTPIPCFG[pipeIter]  = hstPipeCfg;

                                /* Check if Pipe configuration status is OK */
                                if(USBHS_HSTPIPISR_CFGOK_Msk != (USBHS_HSTPIPISR_CFGOK_Msk & hDriver->usbID->USBHS_HSTPIPISR[pipeIter]))
                                {
                                    /* Disable Pipe */
                                    Clr_bits(hDriver->usbID->USBHS_HSTPIP, (1 << (pipeIter)));
                                }
                                else
                                {
                                    /* Configure address of Pipe */
                                    Wr_bitfield((&hDriver->usbID->USBHS_HSTADDR1)[(pipeIter)>>2], 0x7F << (((pipeIter)&0x03)<<3), deviceAddress);

                                    /* Enable Stall Interrupt */
                                    hDriver->usbID->USBHS_HSTPIPIER[pipeIter] = USBHS_HSTPIPIER_RXSTALLDES_Msk;
                                    /* Enable Pipe Error Interrupt */
                                    hDriver->usbID->USBHS_HSTPIPIER[pipeIter] = USBHS_HSTPIPIER_PERRES_Msk;
                                    /* Enable Pipe Interrupt */
                                    Set_bits(hDriver->usbID->USBHS_HSTIER, ((1 << 8) << (pipeIter)));

                                    epFound = true;
                                    pipe = &gDrvUSBHostPipeObj[pipeIter];
                                    hDriver->hostEndpointTable[pipeIter].endpoint.inUse = true;
                                    hDriver->hostEndpointTable[pipeIter].endpoint.pipe = pipe;                
                                    break;
                                }
                            }                            
                        } /* for */
                    } 

                    if(!epFound)
                    {
                        /* This means we could not find a spare endpoint for this
                         * non control transfer. */
                        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Could not allocate endpoint");
                    }
                    if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
                    { 
                        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Mutex unlock failed");
                    }
                }

                if (pipe != NULL)
                {
                    /* Setup the pipe object */
                    pipe->inUse         = true;
                    pipe->deviceAddress = deviceAddress;
                    pipe->irpQueueHead  = NULL;
                    pipe->bInterval     = bInterval;
                    pipe->speed         = speed;
                    pipe->hubAddress    = hubAddress;
                    pipe->hubPort       = hubPort;
                    pipe->pipeType      = pipeType;
                    pipe->hClient       = client;
                    pipe->endpointSize  = wMaxPacketSize;
                    pipe->intervalCounter = bInterval;
                    pipe->hostPipeN     = pipeIter;
                    pipe->endpointAndDirection   = endpointAndDirection;
                    pipeHandle = (DRV_USBHSV1_HOST_PIPE_HANDLE)pipe;
                }
            }
        }
    }
    return(pipeHandle);

}/* end of DRV_USBHSV1_HOST_PipeSetup() */

void _DRV_USBHSV1_HOST_ControlTransferProcess(DRV_USBHSV1_OBJ * hDriver)
{
    /* This function is called every time there is an endpoint 0
     * interrupt. This means that a stage of the current control IRP has been
     * completed. This function is called from an interrupt context */

    USB_HOST_IRP_LOCAL * irp;
    DRV_USBHSV1_HOST_PIPE_OBJ * pipe, * iterPipe;
    DRV_USBHSV1_HOST_TRANSFER_GROUP * transferGroup;
    bool endIRP = false;
    unsigned int count, i;
    bool foundIRP = false;
    bool isDmaUsed = false;
    volatile usbhs_registers_t * usbMod;

    transferGroup = &hDriver->controlTransferGroup;
    usbMod = hDriver->usbID;

    /* First check if the IRP was aborted */
    irp = transferGroup->currentIRP;
    pipe = transferGroup->currentPipe;

    /* If current IRP is null, or current pipe is null then we have unknown
     * failure. We just quit.  Nothing we can do.*/

    if((irp == NULL) || (pipe == NULL) ||
            (pipe == (DRV_USBHSV1_HOST_PIPE_OBJ *)DRV_USBHSV1_HOST_PIPE_HANDLE_INVALID))
    {
        /* Unknown error */
        return;
    }

    /* Disable setup, IN and OUT interrupts of control endpoint */
    usbMod->USBHS_HSTPIPIDR[0] = (USBHS_HSTPIPIDR_TXSTPEC_Msk | USBHS_HSTPIPIDR_RXINEC_Msk | USBHS_HSTPIPIDR_TXOUTEC_Msk);

    /* If here means, we have a valid IRP and pipe.  Check the status register.
     * The IRP could have been aborted. This would be known in the temp state.
     */

    if(irp->tempState == DRV_USBHSV1_HOST_IRP_STATE_ABORTED)
    {
        /* This means the application has aborted this
         IRP.*/

        endIRP = true;
        irp->status = USB_HOST_IRP_STATUS_ABORTED;
    }
    else if(USBHS_HSTPIPISR_RXSTALLDI_Msk == (USBHS_HSTPIPISR_RXSTALLDI_Msk & usbMod->USBHS_HSTPIPISR[0]))
    {
        /* This means the packet was stalled. Set the error status and then
         * clear the stall bit */

        endIRP = true;
        irp->status = USB_HOST_IRP_STATUS_ERROR_STALL;
        /* Clear Stall Interrupt */
        usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_RXSTALLDIC_Msk;
        /* Reset DATA Toggle */
        usbMod->USBHS_HSTPIPIER[0] = USBHS_HSTPIPIER_RSTDTS_Msk;
        
    }
    else if(USBHS_HSTPIPISR_PERRI_Msk == (USBHS_HSTPIPISR_PERRI_Msk & usbMod->USBHS_HSTPIPISR[0]))
    {
        /* This means there was a pipe error. Set the error status and then
         * clear the error bits */

        endIRP = true;
        irp->status = USB_HOST_IRP_STATUS_ERROR_DATA;
        /* Ack all errors */
        usbMod->USBHS_HSTPIPERR[0] = 0;
    }
    else
    {
        /* This means there was no error. We should check the current state of
         * the IRP and then proceed accordingly */

        switch(irp->tempState)
        {
             case DRV_USBHSV1_HOST_IRP_STATE_SETUP_STAGE:
                if (USBHS_HSTPIPISR_TXSTPI_Msk == (USBHS_HSTPIPISR_TXSTPI_Msk & usbMod->USBHS_HSTPIPISR[0]))
                {
                    /* SETUP packet sent */
                    /* Freeze Pipe */
                    usbMod->USBHS_HSTPIPIER[0] = USBHS_HSTPIPIER_PFREEZES_Msk;
                    /* Clear Tx Setup Ready Interrupt */
                    usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_TXSTPIC_Msk;
                }
                else
                {
                    return;
                }
 
               /* We got an interrupt after this stage. This means that the
                * setup stage has completed.  We need to check if a data stage
                * is required and then start the data stage. */

               if((irp->data == NULL) || (irp->size == 0))
               {
                   /* This means there is no data stage. We can proceed to the
                    * handshake stage. In a case where there isnt a data stage,
                    * we need to send an IN token to the device */

                    irp->tempState = DRV_USBHSV1_HOST_IRP_STATE_HANDSHAKE_SENT;
                   
                    /* Configure pipe for IN token */
                    usbMod->USBHS_HSTPIPCFG[0] = (usbMod->USBHS_HSTPIPCFG[0] & ~(uint32_t)(USBHS_HSTPIPCFG_PTOKEN_Msk))
                                                | USBHS_HSTPIPCFG_PTOKEN(1);
                    /* Clear IN Rx Interrupt */
                    usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_RXINIC_Msk;
                    /* Enable IN Rx Interrupt */
                    usbMod->USBHS_HSTPIPIER[0] = USBHS_HSTPIPIER_RXINES_Msk;
                    __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                    __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                    /* Clear FIFOCON and Unfreeze pipe */
                    usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
                    usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_PFREEZEC_Msk;
               }
               else
               {
                   /* This means that a data stage is required. We can find out
                    * the direction of the data stage by investigating the flags
                    * parameter of the  IRP. */

                   irp->tempState = DRV_USBHSV1_HOST_IRP_STATE_DATA_STAGE_SENT;
                   if(irp->flags & 0x80)
                   {
                       /* This means the data stage moves from device to host.
                        * So the host would have to send an IN token.  */
                       /* Configure pipe for IN token */
                       usbMod->USBHS_HSTPIPCFG[0] = (usbMod->USBHS_HSTPIPCFG[0] & ~(uint32_t)(USBHS_HSTPIPCFG_PTOKEN_Msk))
                                                | USBHS_HSTPIPCFG_PTOKEN(1);
                       /* Clear IN Rx Interrupt */
                       usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_RXINIC_Msk;
                       /* Enable IN Rx Interrupt */
                       usbMod->USBHS_HSTPIPIER[0] = USBHS_HSTPIPIER_RXINES_Msk;
                       __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                       __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                       /* Clear FIFOCON and Unfreeze pipe */
                       usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
                       usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_PFREEZEC_Msk;
                   }
                   else
                   {
                       /* This function loads the fifo and sends the packet. The
                        * completed bytes field in the IRP will be updated. */
                       _DRV_USBHSV1_HOST_IRPTransmitFIFOLoad(usbMod, irp, 0);
                   }
               }
               break;

           case DRV_USBHSV1_HOST_IRP_STATE_DATA_STAGE_SENT:

                if (USBHS_HSTPIPISR_RXINI_Msk == (USBHS_HSTPIPISR_RXINI_Msk & usbMod->USBHS_HSTPIPISR[0]))
                {
                    /* IN packet received */
                    /* In case of low USB speed and with a high CPU frequency,
                     * a ACK from host can be always running on USB line
                     * then wait end of ACK on IN pipe */
                    if (USBHS_SR_SPEED_LOW_SPEED == (USBHS_SR_SPEED_LOW_SPEED & usbMod->USBHS_SR))
                    {    
                        while(USBHS_HSTPIPIMR_PFREEZE_Msk != (USBHS_HSTPIPIMR_PFREEZE_Msk & usbMod->USBHS_HSTPIPIMR[0]));
                    }    
                    /* Clear IN Rx Interrupt */        
                    usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_RXINIC_Msk;
                }
                else if (USBHS_HSTPIPISR_TXOUTI_Msk == (USBHS_HSTPIPISR_TXOUTI_Msk & usbMod->USBHS_HSTPIPISR[0]))
                {
                    /* OUT packet sent */
                    /* Clear Tx Setup Ready Interrupt */
                    usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_TXOUTIC_Msk;
                }
                else
                {
                    return;
                }

               /* We reach here after an interrupt which means that a data stage
                * interaction was completed. Find out what was the direction the
                * data stage */

               if(irp->flags & 0x80)
               {
                   /* This means the data was moving from device to host. We got
                    * an interrupt, which means we have received data. Start by
                    * checking how much data we received from the device */

                   count = _DRV_USBHSV1_HOST_IRPReceiveFIFOUnload(usbMod, irp, 0, &isDmaUsed);
                        
                   if((count < pipe->endpointSize) ||
                           (irp->completedBytes >= irp->size))
                   {
                       /* This means that we either received a short packet or
                         * we received the amount of data that we needed. We
                         * should move to the handshake stage. */
                        irp->tempState = DRV_USBHSV1_HOST_IRP_STATE_HANDSHAKE_SENT;
                        /* Configure pipe for OUT token */
                        usbMod->USBHS_HSTPIPCFG[0] = (usbMod->USBHS_HSTPIPCFG[0] & ~(uint32_t)(USBHS_HSTPIPCFG_PTOKEN_Msk))
                                                    | USBHS_HSTPIPCFG_PTOKEN(2);
                        __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                        __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                        /* Clear Tx Out Ready Interrupt */
                        usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_TXOUTIC_Msk;
                        /* Enable Pipe out ready interrupt */
                        usbMod->USBHS_HSTPIPIER[0] = USBHS_HSTPIPIER_TXOUTES_Msk;
                        __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                        __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                        /* Clear FIFOCON and Unfreeze pipe */
                        usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
                        usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_PFREEZEC_Msk;
                   }
                   else
                   {
                        /* This means this is a multi stage control read
                         * transfer. Issue another IN token */
                        /* Configure pipe for IN token */
                        usbMod->USBHS_HSTPIPCFG[0] = (usbMod->USBHS_HSTPIPCFG[0] & ~(uint32_t)(USBHS_HSTPIPCFG_PTOKEN_Msk))
                                                | USBHS_HSTPIPCFG_PTOKEN(1);
                        __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                        __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                        /* Clear IN Rx Interrupt */
                        usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_RXINIC_Msk;
                        /* Enable IN Rx Interrupt */
                        usbMod->USBHS_HSTPIPIER[0] = USBHS_HSTPIPIER_RXINES_Msk;
                        __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                        __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                        /* Clear FIFOCON and Unfreeze pipe */
                        usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
                        usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_PFREEZEC_Msk;
                    }
               }
               else
               {
                   /* Data stage was moving from host to device.  Check if we
                    * need to send more data */

                   if(irp->completedBytes < irp->size)
                   {
                        _DRV_USBHSV1_HOST_IRPTransmitFIFOLoad(usbMod, irp, 0);
                   }
                   else
                   {
                        /* We can move to the status stage */
                        irp->tempState = DRV_USBHSV1_HOST_IRP_STATE_HANDSHAKE_SENT;
                        usbMod->USBHS_HSTPIPCFG[0] = (usbMod->USBHS_HSTPIPCFG[0] & ~(uint32_t)(USBHS_HSTPIPCFG_PTOKEN_Msk))
                                                | USBHS_HSTPIPCFG_PTOKEN(1);
                        __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                        __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                        /* Clear IN Rx Interrupt */
                        usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_RXINIC_Msk;
                        /* Enable IN Rx Interrupt */
                        usbMod->USBHS_HSTPIPIER[0] = USBHS_HSTPIPIER_RXINES_Msk;
                        __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                        __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                        /* Clear FIFOCON and Unfreeze pipe */
                        usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
                        usbMod->USBHS_HSTPIPIDR[0] = USBHS_HSTPIPIDR_PFREEZEC_Msk;
                   }
               }
               break;

           case DRV_USBHSV1_HOST_IRP_STATE_HANDSHAKE_SENT:

               /* If we have reached here, it means that status stage has
                * completed. Check the direction of the data stage, update the
                * irp status flag and then end the irp. */

                   if (USBHS_HSTPIPISR_RXINI_Msk == (USBHS_HSTPIPISR_RXINI_Msk & usbMod->USBHS_HSTPIPISR[0]))
                {
                    /* IN packet received */
                    /* IN packet received */
                    /* In case of low USB speed and with a high CPU frequency,
                     * a ACK from host can be always running on USB line
                     * then wait end of ACK on IN pipe */
                    if (USBHS_SR_SPEED_LOW_SPEED == (USBHS_SR_SPEED_LOW_SPEED & usbMod->USBHS_SR))
                    {    
                        while(USBHS_HSTPIPIMR_PFREEZE_Msk != (USBHS_HSTPIPIMR_PFREEZE_Msk & usbMod->USBHS_HSTPIPIMR[0]));
                    }                     
                    /* Clear IN Rx Interrupt */
                    usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_RXINIC_Msk;
                }
                else if (USBHS_HSTPIPISR_TXOUTI_Msk == (USBHS_HSTPIPISR_TXOUTI_Msk & usbMod->USBHS_HSTPIPISR[0]))
                {
                    /* OUT packet sent */
                    /* Clear Tx Setup Ready Interrupt */
                    usbMod->USBHS_HSTPIPICR[0] = USBHS_HSTPIPICR_TXOUTIC_Msk;
                }
                else
                {
                    return;
                }
               irp->status = USB_HOST_IRP_STATUS_COMPLETED;
               
               if(irp->flags & 0x80)
               {
                  
                   /* This means the data stage moved from device to host. We
                    * need to check the number of bytes the host sent. If it was
                    * less than expected, then irp status should say so */

                   if(irp->completedBytes < irp->size)
                   {
                       irp->size = irp->completedBytes;
                       irp->status = USB_HOST_IRP_STATUS_COMPLETED_SHORT;
                   }
               }
               else
               {
                   /* We need to clear the Status Packet bit and
                    * the Rx packet ready bit */
               }
               endIRP = true;
               break;

           default:
               break;
       }
    }

    if(endIRP)
    {
        /* This means this IRP needs to be terminated and new one started. */

        if(irp->callback != NULL)
        {
            irp->callback((USB_HOST_IRP *)irp);
        }

        /* Expire the IRP */

        pipe->irpQueueHead = irp->next;

        /* Now we need to check if any more IRPs are in this group are pending.
         * We start searching from the current pipe onwards. If we dont find
         * another pipe with an IRP, we should land back on the current pipe and
         * check if we have a IRP to process */

        iterPipe = transferGroup->currentPipe->next;
        for(i = 0; i < transferGroup->nPipes; i ++)
        {
            if(iterPipe == NULL)
            {
                /* We have reached the end of the pipe group. Rewind the pipe
                 * iterator to the start of the pipe group. */

                iterPipe = transferGroup->pipe;
            }

            /* This means that we have a valid pipe.  Now check if there is irp
             * to be processed. */

            if(iterPipe->irpQueueHead != NULL)
            {
                foundIRP = true;
                transferGroup->currentPipe = iterPipe;
                transferGroup->currentIRP = iterPipe->irpQueueHead;
                break;
            }

            iterPipe = iterPipe->next;
        }

        if(foundIRP)
        {
            /* This means we have found another IRP to process. We must load the
             * endpoint. */
            
            irp = transferGroup->currentIRP;
            pipe = transferGroup->currentPipe;
            irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
            irp->tempState = DRV_USBHSV1_HOST_IRP_STATE_SETUP_STAGE;

            /* We need to update the flags parameter of the IRP to indicate the
             * direction of the control transfer. */

            if(*((uint8_t*)(irp->setup)) & 0x80)
            {
                /* This means the data stage moves from device to host. Set bit
                 * 15 of the flags parameter */
               irp->flags |= 0x80;
            }
            else
            {
                /* This means the data stage moves from host to device. Clear
                 * bit 15 of the flags parameter. */
                irp->flags &= 0x7F;
            }

            /* Send the setup packet to the device */
            _DRV_USBHSV1_HOST_IRPTransmitSetupPacket(usbMod, irp);
        }
        else
        {
            /* This means we dont have an IRP. Set the current IRP and current
             * pipe to NULL to indicate that we dont have any active IRP */

            transferGroup->currentPipe = NULL;
            transferGroup->currentIRP = NULL;
        }
    }

    return;
}/* end of _DRV_USBHSV1_HOST_ControlTransferProcess() */

void _DRV_USBHSV1_HOST_NonControlTransferProcess
(
    DRV_USBHSV1_OBJ * hDriver,
    uint8_t hostPipe
)
{
    /* This function processes non-zero endpoint transfers which
     * could be any of bulk, interrupt and isochronous transfers */

    DRV_USBHSV1_HOST_ENDPOINT_OBJ * endpointTable;
    DRV_USBHSV1_HOST_PIPE_OBJ * pipe;
    USB_HOST_IRP_LOCAL * irp;
    volatile usbhs_registers_t * usbMod;
    bool endIRP = false;
    bool isDmaUsed = false;
    bool endIRPOut = false;
    unsigned int count;

    endpointTable = &(hDriver->hostEndpointTable[hostPipe]);
    usbMod = hDriver->usbID;
    pipe = endpointTable->endpoint.pipe; 
    
    if((!endpointTable->endpoint.inUse) ||
       (pipe == NULL) ||
       (pipe == (DRV_USBHSV1_HOST_PIPE_OBJ *)(DRV_USBHSV1_HOST_PIPE_HANDLE_INVALID)))
    {
        /* This means the pipe was closed. We don't do anything */
        return;
    }

    irp = pipe->irpQueueHead;
    
    /* We got an interrupt for data moving from host to to device. Check if
     * there were any transmission errors. Then check if there is any more
     * data to be sent in the IRP. If we are done, then end the IRP and
     * start a new one. */

    /* Check if the IRP was aborted */
    if(irp->tempState == DRV_USBHSV1_HOST_IRP_STATE_ABORTED)
    {
        endIRP = true;
        irp->status = USB_HOST_IRP_STATUS_ABORTED;
    }
    else if(USBHS_HSTPIPISR_RXSTALLDI_Msk == (USBHS_HSTPIPISR_RXSTALLDI_Msk & usbMod->USBHS_HSTPIPISR[hostPipe]))
    {
        /* This means the packet was stalled. Set the error status and then
         * clear the stall bit */

        endIRP = true;
        irp->status = USB_HOST_IRP_STATUS_ERROR_STALL;
        /* Clear Stall Interrupt */
        usbMod->USBHS_HSTPIPICR[hostPipe] = USBHS_HSTPIPICR_RXSTALLDIC_Msk;
        /* Reset DATA Toggle */
        usbMod->USBHS_HSTPIPIER[hostPipe] = USBHS_HSTPIPIER_RSTDTS_Msk;
    }
    else if(USBHS_HSTPIPISR_PERRI_Msk == (USBHS_HSTPIPISR_PERRI_Msk & usbMod->USBHS_HSTPIPISR[hostPipe]))
    {
        /* This means there was an bus error. The packet was tried three
         * times and was not successfully processed */

        endIRP = true;
        irp->status = USB_HOST_IRP_STATUS_ERROR_DATA;
        /* Ack all errors */
        usbMod->USBHS_HSTPIPERR[hostPipe] = 0;
       
    }
    else if(USBHS_HSTPIPISR_TXOUTI_Msk == (USBHS_HSTPIPISR_TXOUTI_Msk & usbMod->USBHS_HSTPIPISR[hostPipe]))
    {
        /* This means this transaction completed successfully.  We should
         * check if there are any spare bytes remaining to be sent and then
         * send it */
            
        /* Clear Tx Setup Ready Interrupt */
        usbMod->USBHS_HSTPIPICR[hostPipe] = USBHS_HSTPIPICR_TXOUTIC_Msk;
        if(irp->completedBytes >= irp->size)
        {
            endIRP = true;
            endIRPOut = true;
            irp->status = USB_HOST_IRP_STATUS_COMPLETED;
        }
        else
        {
            /* This means we have more data to send */
            endIRP = false;

            /* This function will load the next packet for this IRP into the
             * endpoint FIFO and then transmit it. */
            /* Clear Tx Setup Ready Interrupt */
            usbMod->USBHS_HSTPIPICR[hostPipe] = USBHS_HSTPIPICR_TXOUTIC_Msk;
            _DRV_USBHSV1_HOST_IRPTransmitFIFOLoad(usbMod, irp, hostPipe);
        }
    }
    else if(USBHS_HSTPIPISR_RXINI_Msk == (USBHS_HSTPIPISR_RXINI_Msk & usbMod->USBHS_HSTPIPISR[hostPipe]))
    {
        /* In case of low USB speed and with a high CPU frequency,
        * a ACK from host can be always running on USB line
        * then wait end of ACK on IN pipe */
        if (USBHS_SR_SPEED_LOW_SPEED == (USBHS_SR_SPEED_LOW_SPEED & usbMod->USBHS_SR))
        {    
            while(USBHS_HSTPIPIMR_PFREEZE_Msk != (USBHS_HSTPIPIMR_PFREEZE_Msk & usbMod->USBHS_HSTPIPIMR[hostPipe]));
        }  
        usbMod->USBHS_HSTPIPICR[hostPipe] = USBHS_HSTPIPICR_RXINIC_Msk;
        /* This means that data was received without errors. */
        count = _DRV_USBHSV1_HOST_IRPReceiveFIFOUnload(usbMod, irp, hostPipe, &isDmaUsed);
        if(isDmaUsed == false)
        {
            if((count < pipe->endpointSize) ||
                    (irp->completedBytes >= irp->size))
            {
                endIRP = true;
                irp->status = USB_HOST_IRP_STATUS_COMPLETED;

                if(irp->completedBytes < irp->size)
                {
                    /* This means the device ended the transfer and and we
                     * have a short transfer */
                    irp->status = USB_HOST_IRP_STATUS_COMPLETED_SHORT;
                }

                /* Update the actual amount of data received */
                irp->size = irp->completedBytes;
            }
            else
            {
                /* This means we have more data to send We request another
                 * packet */

                endIRP = false;
                /* Enable IN Rx Interrupt */
                usbMod->USBHS_HSTPIPIER[hostPipe] = USBHS_HSTPIPIER_RXINES_Msk;
                __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
                __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
                /* Clear FIFOCON and Unfreeze pipe */
                usbMod->USBHS_HSTPIPIDR[hostPipe] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
                usbMod->USBHS_HSTPIPIDR[hostPipe] = USBHS_HSTPIPIDR_PFREEZEC_Msk;
            }
        }
        else
        {
            /* DMA has been used. Do not end the IRP here.
             * It will be done in DMA ISR handler */
            endIRP = false;
        }
    }
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Unknown Pipe Interrupt");
    }
        

    if(endIRP)
    {
        /* This means we need to end the IRP */
        pipe->irpQueueHead = irp->next;
        if(irp->callback)
        {
            /* Invoke the call back*/
            irp->callback((USB_HOST_IRP *)irp);
        }
        irp = pipe->irpQueueHead;
        if((irp != NULL) && (!(irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)) && (endIRPOut != false) )
        {
            /* We do have another IRP to process. */
            irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
            /* Clear Tx Setup Ready Interrupt */
            usbMod->USBHS_HSTPIPICR[hostPipe] = USBHS_HSTPIPICR_TXOUTIC_Msk;
            _DRV_USBHSV1_HOST_IRPTransmitFIFOLoad(usbMod, irp, hostPipe);
        }

        /* A IRP could have been submitted in the callback. If that is the
         * case and the IRP status would indicate that it already in
         * progress. If the IRP in the queue head is not in progress then we
         * should initiate the transaction */

        if((irp != NULL) && (!(irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)) && (endIRPOut == false) )
        {
            irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
                
            /* We do have another IRP to process. Request for
             * an IN packet. */
            /* Clear IN Rx Interrupt */
            usbMod->USBHS_HSTPIPICR[hostPipe] = USBHS_HSTPIPICR_RXINIC_Msk;
            /* Enable IN Rx Interrupt */
            usbMod->USBHS_HSTPIPIER[hostPipe] = USBHS_HSTPIPIER_RXINES_Msk;
            __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
            __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */
            /* Clear FIFOCON and Unfreeze pipe */
            usbMod->USBHS_HSTPIPIDR[hostPipe] = USBHS_HSTPIPIDR_FIFOCONC_Msk;
            usbMod->USBHS_HSTPIPIDR[hostPipe] = USBHS_HSTPIPIDR_PFREEZEC_Msk;
        }
    }
}/* end of _DRV_USBHSV1_HOST_NonControlTransferProcess() */

void _DRV_USBHSV1_HOST_Tasks_ISR(DRV_USBHSV1_OBJ * hDriver)
{
    uint8_t intPipe;
       
    __DSB(); /* The DMB instruction ensures the observed ordering of memory accesses. */
    __ISB(); /* The ISB instruction flushes the processor pipeline fetch buffers. */

    if((USBHS_HSTISR_DDISCI_Msk == (USBHS_HSTISR_DDISCI_Msk & hDriver->usbID->USBHS_HSTISR)) 
    && (USBHS_HSTIMR_DDISCIE_Msk == (USBHS_HSTIMR_DDISCIE_Msk & hDriver->usbID->USBHS_HSTIMR)))
    {
        /* Manage Device Disconnection Interrupt 
         */
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nDRV USBHSV1: Interrupt: DDISC");

        /* Stop reset signal, in case of disconnection during reset */
        hDriver->usbID->USBHS_HSTCTRL &= ~USBHS_HSTCTRL_RESET_Msk;
        /* Disable SOF */
        hDriver->usbID->USBHS_HSTCTRL &= ~USBHS_HSTCTRL_SOFE_Msk;
        /* Clear Device Disconnection Interrupt */
        hDriver->usbID->USBHS_HSTICR = USBHS_HSTICR_DDISCIC_Msk;
        /* Disable wakeup/resumes interrupts,
         * in case of disconnection during suspend mode */
        hDriver->usbID->USBHS_HSTIDR = (USBHS_HSTIDR_HWUPIEC_Msk
                                      | USBHS_HSTIDR_RSMEDIEC_Msk
                                      | USBHS_HSTIDR_RXRSMIEC_Msk);
        /* Clear Wake-up Interrupt */
        hDriver->usbID->USBHS_HSTICR = USBHS_HSTICR_HWUPIC_Msk;
        /* Enable Host Wakeup Interrupt */
        hDriver->usbID->USBHS_HSTIER = USBHS_HSTIER_HWUPIES_Msk;

        if (hDriver->usbID->USBHS_HSTISR & USBHS_HSTISR_DCONNI_Msk) 
        {
            /* Both connection and disconnection interrupts are set
             * and to sort this out the connection flag is cleared
             * at cost of loss the connection interrupt
             * Clear Connection Interrupt */
            hDriver->usbID->USBHS_HSTICR = USBHS_HSTICR_DCONNIC_Msk;
        }
        hDriver->deviceAttached = false;
        if(hDriver->attachedDeviceObjHandle != USB_HOST_DEVICE_OBJ_HANDLE_INVALID)
        {
            /* Ask the host layer to de-enumerate this device. The de-enumeration
             * must be done in the interrupt context. */
            USB_HOST_DeviceDenumerate (hDriver->attachedDeviceObjHandle);
        }
        hDriver->attachedDeviceObjHandle = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;        
    }
    else if((USBHS_HSTISR_DCONNI_Msk == (USBHS_HSTISR_DCONNI_Msk & hDriver->usbID->USBHS_HSTISR)) 
         && (USBHS_HSTIMR_DCONNIE_Msk == (USBHS_HSTIMR_DCONNIE_Msk & hDriver->usbID->USBHS_HSTIMR)))
    {
        /* Manage Device Connection Interrupt
         */
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, " DCONN");  
        /* Clear Connection Interrupt */
        hDriver->usbID->USBHS_HSTICR = USBHS_HSTICR_DCONNIC_Msk;
        /* Enable Disconnection Interrupt */
        hDriver->usbID->USBHS_HSTIER = USBHS_HSTIER_DDISCIES_Msk;
         hDriver->deviceAttached = true;
     }
    /* If Wakeup interrupt is enabled and triggered and the usb is in disconnected state */
    else if (((USBHS_HSTISR_HWUPI_Msk == (USBHS_HSTISR_HWUPI_Msk & hDriver->usbID->USBHS_HSTISR)) 
          && (USBHS_HSTIMR_HWUPIE_Msk == (USBHS_HSTIMR_HWUPIE_Msk & hDriver->usbID->USBHS_HSTIMR))) 
          && (hDriver->deviceAttached == false))
    {
        /* Manage Host Wakeup Interrupt. This interrupt is generated 
         * even if the clock is frozen
         */
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, " WAKEUP");
        /* Unfreeze clock */
        hDriver->usbID->USBHS_CTRL |= USBHS_CTRL_FRZCLK_Msk;
        
        /* Here the wakeup interrupt has been used to detect connection
         * with an asynchronous interrupt */
        /* Disable Wake-up Interrupt */
        hDriver->usbID->USBHS_HSTIDR = USBHS_HSTIDR_HWUPIEC_Msk;
    }
    else if (((USBHS_HSTISR_HWUPI_Msk | USBHS_HSTISR_RSMEDI_Msk | USBHS_HSTISR_RXRSMI_Msk) 
                == ((USBHS_HSTISR_HWUPI_Msk | USBHS_HSTISR_RSMEDI_Msk | USBHS_HSTISR_RXRSMI_Msk) & hDriver->usbID->USBHS_HSTISR)) 
          && (USBHS_HSTIMR_HWUPIE_Msk == (USBHS_HSTIMR_HWUPIE_Msk & hDriver->usbID->USBHS_HSTIMR))) 
    {  
        /* Wake up */
        /* Unfreeze clock */
        hDriver->usbID->USBHS_CTRL |= USBHS_CTRL_FRZCLK_Msk;
        /* Disable Wake-up Interrupt / resumes */
        hDriver->usbID->USBHS_HSTIDR = (USBHS_HSTIDR_HWUPIEC_Msk
                                      | USBHS_HSTIDR_RSMEDIEC_Msk
                                      | USBHS_HSTIDR_RXRSMIEC_Msk);
        /* Enable SOF */
        hDriver->usbID->USBHS_HSTCTRL |= USBHS_HSTCTRL_SOFE_Msk;

        if ((!(USBHS_HSTISR_RSMEDI_Msk & hDriver->usbID->USBHS_HSTISR))
         && (!(USBHS_HSTISR_DDISCI_Msk & hDriver->usbID->USBHS_HSTISR))) 
        {
            /* It is a upstream resume
             * Note: When the CPU exits from a deep sleep mode, the event
             * upstream_resume can be not detected because the USB clock are not available.
             * In High Speed mode a downstream resume must be sent
             * after a upstream to avoid a disconnection. */
            if (hDriver->deviceSpeed == USB_SPEED_HIGH)
            {
                hDriver->usbID->USBHS_HSTCTRL |= USBHS_HSTCTRL_RESUME_Msk;
            }
            /* Wait 50ms before restarting transfer */
            /* TODO */
        }
    }
    else if((USBHS_HSTISR_RSTI_Msk == (USBHS_HSTISR_RSTI_Msk & hDriver->usbID->USBHS_HSTISR)) && (USBHS_HSTIMR_RSTIE_Msk == (USBHS_HSTIMR_RSTIE_Msk & hDriver->usbID->USBHS_HSTIMR)))
    {
        /* Manage USB Reset Sent Interrupt  */
        /* Clear USB Reset Sent Interrupt */
        hDriver->usbID->USBHS_HSTICR = USBHS_HSTICR_RSTIC_Msk;
        /* Clear the flag */
        hDriver->isResetting = false;
        /* Now that reset is complete, we can find out the
         * speed of the attached device. */
        switch ((USBHS_SR_SPEED_Msk & hDriver->usbID->USBHS_SR) >> USBHS_SR_SPEED_Pos)
        {
            case 0x0:
                hDriver->deviceSpeed = USB_SPEED_FULL;
            break;
            case 0x1:
                hDriver->deviceSpeed = USB_SPEED_HIGH;
            break;
            case 0x2:
                hDriver->deviceSpeed = USB_SPEED_LOW;
            break;
            default:
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Unknown Speed at Reset");
            break;
        }
    }
    else if(0 != (hDriver->usbID->USBHS_HSTISR & 0x3FF00))
    {
        /* Manage pipe interrupts */
        /* Get the lowest Pipe number generating
         * a Pipe Interrupt */
        intPipe = ctz(((hDriver->usbID->USBHS_HSTISR & hDriver->usbID->USBHS_HSTIMR) >> 8) | (1 << 10));
        if (intPipe == 0) 
        {
             /* Manage control pipe */
            _DRV_USBHSV1_HOST_ControlTransferProcess(hDriver);
        }
        else
        {
            /* Manage Non-control pipe */
            _DRV_USBHSV1_HOST_NonControlTransferProcess(hDriver, intPipe);
        }
    } 
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Unknown Interrupt");
    }   

}/* end of _DRV_USBHSV1_HOST_Tasks_ISR() */

void DRV_USBHSV1_HOST_StartOfFrameControl(DRV_HANDLE client, bool control)
{
    /* At the point this function does not do any thing.
     * The Start of frame signaling in this HCD is controlled
     * automatically by the module. */
}/* end of DRV_USBHSV1_HOST_StartOfFrameControl() */

USB_SPEED DRV_USBHSV1_HOST_DeviceCurrentSpeedGet(DRV_HANDLE client)
{
    /* This function returns the current device speed */

    DRV_USBHSV1_OBJ * hDriver;

    if((client == DRV_HANDLE_INVALID) || (((DRV_USBHSV1_OBJ *)client) == NULL))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Invalid client");
    }

    hDriver = (DRV_USBHSV1_OBJ *)client;
    return(hDriver->deviceSpeed);

}/* end of DRV_USBHSV1_HOST_DeviceCurrentSpeedGet() */

// ****************************************************************************
/* Function:
    bool DRV_USBHSV1_HOST_EventDisable
    (
        DRV_HANDLE handle
    );
    
  Summary:
    Disables host mode events.
    
  Description:
    This function disables the host mode events. This function is called by the
    Host Layer when it wants to execute code atomically. 
    
  Remarks:
    Refer to drv_usbfs.h for usage information.
*/

bool DRV_USBHSV1_HOST_EventsDisable
(
    DRV_HANDLE handle
)
{
    DRV_USBHSV1_OBJ * pUSBDrvObj;
    bool result = false; 
    
    if((DRV_HANDLE_INVALID != handle) && (0 != handle))
    {
        pUSBDrvObj = (DRV_USBHSV1_OBJ *)(handle);
        result = _DRV_USBHSV1_InterruptSourceDisable(pUSBDrvObj->interruptSource);
    }

    return(result);
}

// ****************************************************************************
/* Function:
    void DRV_USBHSV1_HOST_EventsDisable
    (
        DRV_HANDLE handle
        bool eventRestoreContext
    );
    
  Summary:
    Restores the events to the specified the original value.
    
  Description:
    This function will restore the enable disable state of the events.
    eventRestoreContext should be equal to the value returned by the
    DRV_USBHSV1_HOST_EventsDisable() function.
    
  Remarks:
    Refer to drv_usbfs.h for usage information.
*/

void DRV_USBHSV1_HOST_EventsEnable
(
    DRV_HANDLE handle, 
    bool eventContext
)
{
    DRV_USBHSV1_OBJ * pUSBDrvObj;
   
    if((DRV_HANDLE_INVALID != handle) && (0 != handle))
    {
        pUSBDrvObj = (DRV_USBHSV1_OBJ *)(handle);
        if(false == eventContext)
        {
            _DRV_USBHSV1_InterruptSourceDisable(pUSBDrvObj->interruptSource);
        }
        else
        {
            _DRV_USBHSV1_InterruptSourceEnable(pUSBDrvObj->interruptSource);
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Root Hub Driver Routines
// *****************************************************************************
// *****************************************************************************

void DRV_USBHSV1_HOST_ROOT_HUB_OperationEnable(DRV_HANDLE handle, bool enable)
{
    /* Start of local variable */
    DRV_USBHSV1_OBJ * pUSBDrvObj = (DRV_USBHSV1_OBJ *)handle;
    /* End of local variable */
    
    volatile usbhs_registers_t * usbMod = pUSBDrvObj->usbID;

    /* Check if the handle is valid or opened */
    if((handle == DRV_HANDLE_INVALID) || (!(pUSBDrvObj->isOpened)))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else
    {
        if(false == enable)
        {
             /* If the root hub operation is disable, we disable detach and
             * attached event and switch off the port power. */

            _DRV_USBHSV1_InterruptSourceClear(pUSBDrvObj->interruptSource);
            pUSBDrvObj->operationEnabled = false;

        }
        else
        {
            /* The USB Global interrupt and USB module is already enabled at
             * this point. We enable the attach interrupt to detect attach
             */
            pUSBDrvObj->operationEnabled = true;
            /* Enable Device Connection Interrupt */
            usbMod->USBHS_HSTIER = USBHS_HSTIER_DCONNIES_Msk;
            /* Unfreeze clock */
            usbMod->USBHS_CTRL &= ~USBHS_CTRL_FRZCLK_Msk;
        }
    }
}

bool DRV_USBHSV1_HOST_ROOT_HUB_OperationIsEnabled(DRV_HANDLE hClient)
{
    DRV_USBHSV1_OBJ * hDriver;
    if((hClient == DRV_HANDLE_INVALID) || (((DRV_USBHSV1_OBJ *)hClient) == NULL))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Invalid client");
    }
    hDriver = (DRV_USBHSV1_OBJ *)hClient;
    return(hDriver->operationEnabled);

}/* end of DRV_USBHSV1_HOST_OperationIsEnabled() */

// ****************************************************************************
/* Function:
    void DRV_USBHSV1_HOST_ROOT_HUB_Initialize
    (
        DRV_HANDLE handle,
        USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo,
    )

  Summary:
    This function instantiates the root hub driver.

  Description:
    This function initializes the root hub driver. It is called by the host
    layer at the time of processing root hub devices. The host layer assigns a
    USB_HOST_DEVICE_OBJ_HANDLE reference to this root hub driver. This
    identifies the relationship between the root hub and the host layer.

  Remarks:
    None.
*/

void DRV_USBHSV1_HOST_ROOT_HUB_Initialize
(
    DRV_HANDLE handle,
    USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo
)
{
    DRV_USBHSV1_OBJ * pUSBDrvObj = (DRV_USBHSV1_OBJ *)handle;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else
    {
        pUSBDrvObj->usbHostDeviceInfo = usbHostDeviceInfo;
    }
}

// ****************************************************************************
/* Function:
    uint8_t DRV_USBHSV1_HOST_ROOT_HUB_PortNumbersGet(DRV_HANDLE handle);

  Summary:
    Returns the number of ports this root hub contains.

  Description:
    This function returns the number of ports that this root hub contains.

  Remarks:
    None.
*/

uint8_t DRV_USBHSV1_HOST_ROOT_HUB_PortNumbersGet(DRV_HANDLE handle)
{
    DRV_USBHSV1_OBJ * pUSBDrvObj = (DRV_USBHSV1_OBJ *)handle;
    uint8_t result = 0;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else
    {
        result = 1;
    }

    return(result);
}

// ****************************************************************************
/* Function:
    uint32_t DRV_USBHSV1_HOST_ROOT_HUB_MaximumCurrentGet(DRV_HANDLE);

  Summary:
    Returns the maximum amount of current that this root hub can provide on the
    bus.

  Description:
    This function returns the maximum amount of current that this root hubn can
    provide on the bus.

  Remarks:
    Refer to drv_usbfs.h for usage information.
*/

uint32_t DRV_USBHSV1_HOST_ROOT_HUB_MaximumCurrentGet(DRV_HANDLE handle)
{
    DRV_USBHSV1_OBJ * pUSBDrvObj = (DRV_USBHSV1_OBJ *)handle;
    uint32_t result = 0;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else
    {
        result = pUSBDrvObj->rootHubInfo.rootHubAvailableCurrent;
    }

    return(result);
}

// ****************************************************************************
/* Function:
    USB_SPEED DRV_USBHSV1_HOST_ROOT_HUB_BusSpeedGet(DRV_HANDLE handle);

  Summary:
    Returns the speed at which the bus to which this root hub is connected is
    operating.

  Description:
    This function returns the speed at which the bus to which this root hub is
    connected is operating.

 Remarks:
    None.
*/

USB_SPEED DRV_USBHSV1_HOST_ROOT_HUB_BusSpeedGet(DRV_HANDLE handle)
{
    DRV_USBHSV1_OBJ * pUSBDrvObj = (DRV_USBHSV1_OBJ *)handle;
    USB_SPEED speed = USB_SPEED_ERROR;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");

    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else
    {
        /* Return the bus speed. This is speed at which the root hub is
         * operating. */
        speed = pUSBDrvObj->operationSpeed;
    }

    return(speed);
}

// ****************************************************************************
/* Function:
    void DRV_USBHSV1_ROOT_HUB_PortResume(DRV_HANDLE handle, uint8_t port );

  Summary:
    Resumes the specified root hub port.

  Description:
    This function resumes the root hub. The resume duration is defined by
    DRV_USBHSV1_ROOT_HUB_RESUME_DURATION. The status of the resume signalling can
    be checked using the DRV_USBHSV1_ROOT_HUB_PortResumeIsComplete() function.

  Remarks:
    The root hub on this particular hardware only contains one port - port 0.
*/

USB_ERROR DRV_USBHSV1_HOST_ROOT_HUB_PortResume(DRV_HANDLE handle, uint8_t port)
{
    /* The functionality is yet to be implemented. */
    return(USB_ERROR_NONE);
}

// ****************************************************************************
/* Function:
    void DRV_USBHSV1_ROOT_HUB_PortSuspend(DRV_HANDLE handle, uint8_t port );

  Summary:
    Suspends the specified root hub port.

  Description:
    This function suspends the root hub port.

  Remarks:
    The root hub on this particular hardware only contains one port - port 0.
*/

USB_ERROR DRV_USBHSV1_HOST_ROOT_HUB_PortSuspend(DRV_HANDLE handle, uint8_t port)
{
    /* The functionality is yet to be implemented. */
    return (USB_ERROR_NONE);
}

// ****************************************************************************
/* Function:
    void DRV_USBHSV1_ROOT_HUB_PortResetIsComplete
    (
        DRV_HANDLE handle,
        uint8_t port
    );

  Summary:
    Returns true if the root hub has completed the port reset operation.

  Description:
    This function returns true if the port reset operation has completed. It
    should be called after the DRV_USB_HOST_ROOT_HUB_PortReset() function to
    check if the reset operation has completed.

  Remarks:
    Refer to drv_usbfs.h for usage information.
 */

bool DRV_USBHSV1_HOST_ROOT_HUB_PortResetIsComplete
(
    DRV_HANDLE handle,
    uint8_t port
)
{
    DRV_USBHSV1_OBJ * pUSBDrvObj = (DRV_USBHSV1_OBJ *)handle;
    bool result = true;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else
    {
        /* Return false if the driver is still resetting*/
        result = (pUSBDrvObj->isResetting) ? false : true;
    }

    return(result);
}

// ****************************************************************************
/* Function:
    void DRV_USBHSV1_ROOT_HUB_PortReset(DRV_HANDLE handle, uint8_t port );
    
  Summary:
    Resets the specified root hub port.
    
  Description:
    This function resets the root hub port. The reset duration is defined by
    DRV_USBHSV1_ROOT_HUB_RESET_DURATION. The status of the reset signaling can be
    checked using the DRV_USBHSV1_ROOT_HUB_PortResetIsComplete() function.
    
  Remarks:
    Refer to drv_usbfs.h for usage information.
*/

USB_ERROR DRV_USBHSV1_HOST_ROOT_HUB_PortReset(DRV_HANDLE handle, uint8_t port)
{
    DRV_USBHSV1_OBJ * pUSBDrvObj = (DRV_USBHSV1_OBJ *)handle;
    USB_ERROR result = USB_ERROR_NONE;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
        result = USB_ERROR_PARAMETER_INVALID;

    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
        result = USB_ERROR_PARAMETER_INVALID;
    }
    else if(pUSBDrvObj->isResetting)
    {
        /* This means a reset is already in progress. Lets not do anything. */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Reset already in progress");

    }
    else
    {
        /* Start the reset signal. Set the driver flag to indicate the reset
         * signal is in progress. Start generating the reset signal.
         */
        
        pUSBDrvObj->isResetting = true;
        pUSBDrvObj->resetState = DRV_USBHSV1_HOST_RESET_STATE_START;
        /* Enable Reset sent interrupt */
        pUSBDrvObj->usbID->USBHS_HSTIER |= USBHS_HSTIER_RSTIES_Msk;
        /* Start Reset */
        pUSBDrvObj->usbID->USBHS_HSTCTRL |= USBHS_HSTCTRL_RESET_Msk;
    }

    return(result);
}

// ****************************************************************************
/* Function:
    USB_SPEED DRV_USBHSV1_HOST_ROOT_HUB_PortSpeedGet
    (
        DRV_HANDLE handle,
        uint8_t port
    );

  Summary:
    Returns the speed of at which the port is operating.

  Description:
    This function returns the speed at which the port is operating.

  Remarks:
    Refer to drv_usbhs.h for usage information.
*/

USB_SPEED DRV_USBHSV1_HOST_ROOT_HUB_PortSpeedGet(DRV_HANDLE handle, uint8_t port)
{
    DRV_USBHSV1_OBJ * pUSBDrvObj = (DRV_USBHSV1_OBJ *)handle;
    USB_SPEED speed = USB_SPEED_ERROR;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Bad Client or client closed");
    }
    else
    {
        /* The driver will not check if a device is connected. It is assumed
         * that the client has issued a port reset before calling this function
         */
        speed = pUSBDrvObj->deviceSpeed;
    }

    return(speed);
}

// ****************************************************************************
/* Function:
    void DRV_USBHSV1_HOST_EndpointToggleClear
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    Facilitates in resetting of endpoint data toggle to 0 for Non Control
    endpoints.

  Description:
    Facilitates in resetting of endpoint data toggle to 0 for Non Control
    endpoints.

  Remarks:
    Refer to drv_usbhsv1.h for usage information.
*/

void DRV_USBHSV1_HOST_EndpointToggleClear
(
    DRV_HANDLE client,
    USB_ENDPOINT endpointAndDirection
)
{
    /* Start of local variables */
    DRV_USBHSV1_OBJ * hDriver = NULL;
    uint8_t epIter = 0;
    
    /* End of local variables */
    if((client == DRV_HANDLE_INVALID) || (((DRV_USBHSV1_OBJ *)client) == NULL))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USBHSV1: Invalid client handle");
    }
    else
    {
        hDriver = (DRV_USBHSV1_OBJ *)client;
            
        /* Now map the device endpoint to host endpoint. This is required to
         * jump to the appropriate entry in the endpoint table */
        for(epIter = 1; epIter < DRV_USBHSV1_HOST_MAXIMUM_ENDPOINTS_NUMBER; epIter++)
        {
            if(true == hDriver->hostEndpointTable[epIter].endpoint.inUse)
            {
                /* Please not that for a single non control endpoint there cannot
                 * be multiple pipes. Hence there should be only 1 pipe object
                 * that can be linked to this "endpointAndDirection". */
                if((hDriver->hostEndpointTable[epIter].endpoint.pipe)->endpointAndDirection
                        == endpointAndDirection)
                {
                    /* Got the entry in the host endpoint table. We can exit
                     * from this loop now for further processing */
                    hDriver->usbID->USBHS_HSTPIPIER[epIter] = USBHS_HSTPIPIER_RSTDTS_Msk;
                }
            }
        }
        
       
    }
} /* end of DRV_USBHSV1_HOST_EndpointToggleClear() */


