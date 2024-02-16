/*******************************************************************************
  MPLAB Harmony Generated Display Driver Async Implementation File

  File Name:
    raspi_touch_display.c

  Summary:
    Implements display and touch driver support for the Raspberry Pi Touch Display.
    https://www.raspberrypi.com/products/raspberry-pi-touch-display/

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
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

#include "gfx/display/rpi_touch_display.h"
#include "string.h"

/* Utility Macros */
#if (DRV_I2C_CLIENTS_NUMBER_IDX${I2CIdx} < 2)
#error "This driver utilizes 2 client objects"
#endif

/* Maximum Simultaneous Touches */
#define MAX_TOUCH_POINTS    (DISP_RX_BUF_SIZE / 6)

/* Set Bit */
#define BIT(__v) (1UL << (__v))

/* MCU I2C Client Registers */
typedef enum {
	REG_ID = 0x80,
	REG_PORTA,
	REG_PORTB,
	REG_PORTC,
	REG_PORTD,
	REG_POWERON,
	REG_PWM,
	REG_DDRA,
	REG_DDRB,
	REG_DDRC,
	REG_DDRD,
	REG_TEST,
	REG_WR_ADDRL,
	REG_WR_ADDRH,
	REG_READH,
	REG_READL,
	REG_WRITEH,
	REG_WRITEL,
	REG_ID2,
} REG_ADDR;

/* Driver Data */
DISP_DATA disp_data;

/* Local Functions */
/* Handle I2C events from all clients */
static void DISP_I2CEventHandler(DRV_I2C_TRANSFER_EVENT event, DRV_I2C_TRANSFER_HANDLE transferHandle, uintptr_t context)
{
    for (int client_id = 0; client_id < DISP_MAX_CLIENTS; client_id++)
    {
        if (context == client_id)
        {
            switch(event)
            {
                case DRV_I2C_TRANSFER_EVENT_COMPLETE:
                {
                    disp_data.transferBusy[client_id] = false;
                    disp_data.transferNack[client_id] = false;

                    break;
                }
                case DRV_I2C_TRANSFER_EVENT_ERROR:
                {
                    DRV_I2C_ERROR err = DRV_I2C_ErrorGet(transferHandle);

                    if(err == DRV_I2C_ERROR_NACK)
                    {
                        disp_data.transferBusy[client_id] = false;
                        disp_data.transferNack[client_id] = true;
                    }
                    else if(err == DRV_I2C_ERROR_BUS)
                    {
                        disp_data.state = DISP_STATE_ERROR;
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
}

/* Utility Wrappers */
/* Write to DSI to DPI Bridge using DCS Generic Long Write*/
static void DISP_TC358762XBG_Write(uint16_t reg, uint32_t val) {
    DSI_GENERIC_HEADER hdr;
    DSI_GENERIC_PAYLOAD pld[2];

    memset(&hdr, 0, sizeof(hdr));
    memset(pld, 0, sizeof(pld));

    hdr.longPacket.dataType = 0x29;
    hdr.longPacket.virtualChannelID = 0;
    hdr.longPacket.size = 6;

    pld[0].dataU16[0] = reg;
    pld[0].dataU16[1] = val;
    pld[1].dataU16[0] = val >> 16;

    DSI_Write(&hdr, pld);
}

/* Delay Wrappers */
/* Simple blocking delay */
static void DISP_DelayMS(uint32_t timeMillis)
{
    SYS_TIME_HANDLE delay = SYS_TIME_HANDLE_INVALID;
    SYS_TIME_DelayMS(timeMillis, &delay);
    while (SYS_TIME_DelayIsComplete(delay) == false);
}

/* Wait and go to next state */
static void DISP_DelayNextState(uint32_t timeMillis, DISP_STATES stateNext)
{
    disp_data.dlyHandle = SYS_TIME_HANDLE_INVALID;
    SYS_TIME_DelayMS(timeMillis, &disp_data.dlyHandle);
    disp_data.state = DISP_STATE_DELAY;
    disp_data.stateNext = stateNext;
}

/* Transfer Wrappers */
/* Display Register Write */
static void DISP_WriteRegister(uint8_t reg, uint8_t val, DISP_CLIENT_ID client_id)
{
    /* Set Busy Flag */
    disp_data.transferBusy[client_id] = true;

    /* Prepare Buffers */
    disp_data.txBuffer[client_id][0] = reg;
    disp_data.txBuffer[client_id][1] = val;

    DRV_I2C_WriteTransferAdd(disp_data.i2cHandle[client_id],
                             disp_data.i2cAddress[client_id],
                             disp_data.txBuffer[client_id], 2,
                             &disp_data.transferHandle[client_id]);
}

/* Display Register Read */
static void DISP_ReadRegister(uint8_t reg, DISP_CLIENT_ID client_id)
{
    /* Set Busy Flag */
    disp_data.transferBusy[client_id] = true;

    /* Prepare Buffers */
    disp_data.txBuffer[client_id][0] = reg;
    disp_data.rxBuffer[client_id][0] = 0;

    DRV_I2C_WriteReadTransferAdd(disp_data.i2cHandle[client_id],
                                 disp_data.i2cAddress[client_id],
                                 disp_data.txBuffer[client_id], 1,
                                 disp_data.rxBuffer[client_id], 1,
                                 &disp_data.transferHandle[client_id]);
}

/* Display Register Read Multiple */
static void DISP_ReadRegisterMultiple(uint8_t reg, uint8_t size, DISP_CLIENT_ID client_id)
{
    if (size > DISP_RX_BUF_SIZE)
        size = DISP_RX_BUF_SIZE;

    /* Set Busy Flag */
    disp_data.transferBusy[client_id] = true;

    /* Prepare Buffers */
    disp_data.txBuffer[client_id][0] = reg;

    DRV_I2C_WriteReadTransferAdd(disp_data.i2cHandle[client_id],
                                 disp_data.i2cAddress[client_id],
                                 disp_data.txBuffer[client_id], 1,
                                 disp_data.rxBuffer[client_id], size,
                                 &disp_data.transferHandle[client_id]);
}

/* Check if client is busy */
static bool DISP_TransferBusy(DISP_CLIENT_ID client_id)
{
    return disp_data.transferBusy[client_id];
}

/* Check if client NACK'd transfer, only invoke after transfer busy check */
static bool DISP_TransferNack(DISP_CLIENT_ID client_id)
{
    return disp_data.transferNack[client_id];
}

/* Get received client data byte */
static uint8_t DISP_GetRxDataByte(DISP_CLIENT_ID client_id, uint32_t offset)
{
    if (offset > DISP_RX_BUF_SIZE)
        offset = DISP_RX_BUF_SIZE;

    return disp_data.rxBuffer[client_id][offset];
}

/* Driver Interface Functions */
void DISP_RPi_Initialize(void)
{
    memset(&disp_data, 0, sizeof(disp_data));

    disp_data.state = DISP_STATE_INIT;

    disp_data.i2cAddress[DISP_CLIENT_ID_MCU] = 0x45;
    disp_data.i2cAddress[DISP_CLIENT_ID_TOUCH] = 0x38;
}

bool DISP_RPi_SetBrightness(uint8_t brightness)
{
    if (!DISP_TransferBusy(DISP_CLIENT_ID_MCU))
    {
        DISP_WriteRegister(REG_PWM, brightness, DISP_CLIENT_ID_MCU);
        return false;
    }

    return true;
}

void DISP_RPi_Update(void)
{
    switch(disp_data.state)
    {
        case DISP_STATE_INIT:
        {
            /* Open I2C driver clients */
            for (int client_id=0; client_id< DISP_MAX_CLIENTS; client_id++)
            {
                disp_data.i2cHandle[client_id] = DRV_I2C_Open(DRV_I2C_INDEX_${I2CIdx}, DRV_IO_INTENT_READWRITE);

                if (disp_data.i2cHandle[client_id] != DRV_HANDLE_INVALID)
                {
                    /* Register the I2C Drivers event callback */
                    DRV_I2C_TransferEventHandlerSet(disp_data.i2cHandle[client_id], DISP_I2CEventHandler, client_id);
                }
                else
                {
                    disp_data.state = DISP_STATE_ERROR;
                }
            }

            if (disp_data.state != DISP_STATE_ERROR)
            {
                /* Send Power-On Command */
                DISP_WriteRegister(REG_POWERON, 1, DISP_CLIENT_ID_MCU);

                /* Next State */
                disp_data.state = DISP_STATE_POWER_ON;
            }

            break;
        }
        case DISP_STATE_POWER_ON:
        {
            /* Wait for transfer to complete */
            if (!DISP_TransferBusy(DISP_CLIENT_ID_MCU))
            {
                if (!DISP_TransferNack(DISP_CLIENT_ID_MCU))
                {
                    DISP_DelayNextState(60, DISP_STATE_WAIT_POWER_ON);
                }
                else
                {
                    /* Check FFC cable, power to display and power cycle the kit
                     * if code hangs here
                     */
                    DISP_WriteRegister(REG_POWERON, 1, DISP_CLIENT_ID_MCU);
                    DISP_DelayNextState(60, DISP_STATE_POWER_ON);
                }
            }

            break;
        }
        case DISP_STATE_WAIT_POWER_ON:
        {
            /* Wait for transfer to complete */
            if (!DISP_TransferBusy(DISP_CLIENT_ID_MCU))
            {
                uint8_t val = DISP_GetRxDataByte(DISP_CLIENT_ID_MCU, 0);

                if ((val & BIT(0)) == 1)
                {
                    if (!DISP_TransferNack(DISP_CLIENT_ID_MCU))
                        DISP_DelayNextState(60, DISP_STATE_SET_HFLIP);
                }
                else
                {
                    DISP_ReadRegister(REG_PORTB, DISP_CLIENT_ID_MCU);
                }
            }

            break;
        }
        case DISP_STATE_SET_HFLIP:
        {
            /* Wait for transfer to complete */
            if (!DISP_TransferBusy(DISP_CLIENT_ID_MCU))
            {
                DISP_WriteRegister(REG_PORTA, ${HorFlip?then('4', '8')}, DISP_CLIENT_ID_MCU);

                if (!DISP_TransferNack(DISP_CLIENT_ID_MCU))
                    DISP_DelayNextState(60, DISP_STATE_CONFIG_MIPI_HOST);
            }

            break;
        }
        case DISP_STATE_CONFIG_MIPI_HOST:
        {
            /* Enter DSI Command Mode */
            DSI_CommandMode();

            /* Configure TC358762XBG Bridge */
            DISP_TC358762XBG_Write(0x0210, 0x03);
            DISP_TC358762XBG_Write(0x0164, 0x05);
            DISP_TC358762XBG_Write(0x0168, 0x05);
            DISP_TC358762XBG_Write(0x0144, 0x00);
            DISP_TC358762XBG_Write(0x0148, 0x00);
            DISP_TC358762XBG_Write(0x0114, 0x03);
            DISP_TC358762XBG_Write(0x0450, 0x00);
            DISP_TC358762XBG_Write(0x0420, 0x00100150);
            DISP_TC358762XBG_Write(0x0464, 0x040f);
            DISP_DelayMS(100);
            DISP_TC358762XBG_Write(0x0104, 0x01);
            DISP_TC358762XBG_Write(0x0204, 0x01);
            DISP_DelayMS(100);

            /* Enter DSI Video Mode */
            DSI_VideoMode();

            DISP_DelayNextState(${DelayBL}, DISP_STATE_SET_BRIGHTNESS);

            break;
        }
        case DISP_STATE_SET_BRIGHTNESS:
        {
            /* Wait for transfer to complete */
            if (!DISP_TransferBusy(DISP_CLIENT_ID_MCU))
            {
                DISP_WriteRegister(REG_PWM, ${DefaultBL}, DISP_CLIENT_ID_MCU);
                if (!DISP_TransferNack(DISP_CLIENT_ID_MCU))
                    disp_data.state = DISP_STATE_IDLE;
            }

            break;
        }
        case DISP_STATE_PROCESS_TOUCH:
        {
            /* Wait for transfer to complete */
            if (!DISP_TransferBusy(DISP_CLIENT_ID_TOUCH))
            {
                /* Process Touch Data */
                uint8_t xh, xl, yh, yl;
                uint16_t x, y;
                uint32_t touch_id = 0, temp_status = 0;
                uint32_t TouchPts = 0;

                TouchPts = DISP_GetRxDataByte((DISP_CLIENT_ID_TOUCH), 0x2);

                
                /* Resolve all points */
                if(TouchPts > 0)
                {
                    if(TouchPts > 5) TouchPts = 5;

                    for ( int idx = 0; idx < TouchPts; idx++)
                    {
                        xh = DISP_GetRxDataByte(DISP_CLIENT_ID_TOUCH, 0x3 + (6 * idx));
                        xl = DISP_GetRxDataByte(DISP_CLIENT_ID_TOUCH, 0x4 + (6 * idx));
                        yh = DISP_GetRxDataByte(DISP_CLIENT_ID_TOUCH, 0x5 + (6 * idx));
                        yl = DISP_GetRxDataByte(DISP_CLIENT_ID_TOUCH, 0x6 + (6 * idx));
                        
                        touch_id = yh >> 4;
                    
                        temp_status |= 1 << touch_id;
                    
                        x = (xh & 0xF) << 8 | xl;
                        y = (yh & 0xF) << 8 | yl;

                        <#if HorFlip>
                        /* Handle Horizontal Flip */
                        disp_data.touchX[idx] = 800 - x;
                        disp_data.touchY[idx] = 480 - y;
                        </#if>
                                        
                    }
                }
                
                for(int idx = 0; idx < MAX_TOUCH_POINTS; idx++)
                {
                    if(temp_status & (1<<idx))
                    {
                        if (disp_data.touchID[idx] == 0)
                        {
                            SYS_INP_InjectTouchDown(idx,
                                                    disp_data.touchX[idx],
                                                    disp_data.touchY[idx]);

                            disp_data.touchID[idx] = 1;
                        }
                        else
                        {
                            SYS_INP_InjectTouchMove(idx,
                                                    disp_data.touchX[idx],
                                                    disp_data.touchY[idx]);
                        }
                    }
                    else if (disp_data.touchID[idx] != 0)
                    {
                        SYS_INP_InjectTouchUp(idx,
                                              disp_data.touchX[idx],
                                              disp_data.touchY[idx]);

                        disp_data.touchID[idx] = 0;
                    }

                }

                disp_data.state = DISP_STATE_IDLE;
            }

            break;
        }
        case DISP_STATE_DELAY:
        {
            if(SYS_TIME_DelayIsComplete(disp_data.dlyHandle))
            {
                disp_data.state = disp_data.stateNext;
            }

            break;
        }
        case DISP_STATE_ERROR:
        {
            break;
        }
        case DISP_STATE_IDLE:
        {
            /* Wait for transfer to complete (for e.g. DISP_RPi_SetBrightness)*/
            if (!DISP_TransferBusy(DISP_CLIENT_ID_MCU))
            {
                DISP_ReadRegisterMultiple(0x00, 32, DISP_CLIENT_ID_TOUCH);
                disp_data.state = DISP_STATE_PROCESS_TOUCH;
            }

            break;
        }
        default:
        {
            break;
        }
    }
}
