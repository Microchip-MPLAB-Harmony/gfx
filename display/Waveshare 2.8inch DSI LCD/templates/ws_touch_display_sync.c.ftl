/*******************************************************************************
  MPLAB Harmony Generated Display Driver Sync Implementation File

  File Name:
    ws_touch_display.c

  Summary:
    Implements display and touch driver support for the
    Waveshare 2.8inch DSI LCD Touch Display.
    https://www.waveshare.com/2.8inch-dsi-lcd.htm

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

#include "gfx/display/ws_touch_display.h"
#include "string.h"

/* Utility Macros */
#if (DRV_I2C_CLIENTS_NUMBER_IDX${I2CIdx} < 2)
#error "This driver utilizes 2 client objects"
#endif

/* Driver Data */
DISP_DATA disp_data;

/* Touch Chip ID */
#define DISP_TCID_BYTES 3
static const uint8_t chipID[DISP_TCID_BYTES] = {
    '9', '1', '1',
};

/* Touch Chip Firmware */
#define DISP_FW_BYTES 187
static uint8_t touchFw[DISP_FW_BYTES] =
{
    0x80, 0x47, // Register Address
    0x5d, 0x00, 0x10, 0x00, 0x10, 0x05, 0x3d, 0x20, // FW w/ Extended Resolution
    0x22, 0x08, 0x28, 0x0a, 0x5a, 0x3c, 0x03, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x23, 0x13, 0x00, 0x18,
    0x1a, 0x1e, 0x14, 0x85, 0x25, 0x08, 0xcd, 0xcf,
    0xb2, 0x04, 0x00, 0x00, 0x00, 0xfa, 0x02, 0x1d,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xb4, 0xee, 0x9e, 0xd2, 0xf4,
    0x08, 0x00, 0x00, 0x04, 0x89, 0xb9, 0x00, 0x84,
    0xc3, 0x00, 0x7f, 0xcf, 0x00, 0x7a, 0xda, 0x00,
    0x78, 0xe7, 0x00, 0x76, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x11, 0x00, 0x00,
    0x1a, 0x18, 0x16, 0x14, 0x12, 0x10, 0x0e, 0x0c,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x20,
    0x1f, 0x1e, 0x1d, 0x00, 0x02, 0x04, 0x06, 0x08,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1b, // Checksum
};

/* Display Initialization Sequence */
#define DISP_INIT_BYTES 4
static uint8_t initSeq[DISP_INIT_BYTES][2] = {
    {0xC0, 0x1},
    {0xC2, 0x1},
    {0xAC, 0x1},
    {0xAD, 0x1},
};

/* Display Brightness Sequence */
#define DISP_BRT_BYTES 2
static uint8_t brightSeq[DISP_BRT_BYTES][2] = {
    {0xAB, ${(255-DefaultBL)?c}},
    {0xAA, 0x1},
};

/* Delay Wrappers */
/* Wait and go to next state */
static void DISP_DelayNextState(uint32_t timeMillis, DISP_STATES stateNext)
{
    disp_data.dlyHandle = SYS_TIME_HANDLE_INVALID;
    SYS_TIME_DelayMS(timeMillis, &disp_data.dlyHandle);
    disp_data.state = DISP_STATE_DELAY;
    disp_data.stateNext = stateNext;
}

/* Transfer Wrappers */
/* Display Write Touch Firmware */
static bool DISP_WriteTouchFw(void)
{
    return DRV_I2C_WriteTransfer(disp_data.i2cHandle[DISP_CLIENT_ID_TOUCH],
                                 disp_data.i2cAddress[DISP_CLIENT_ID_TOUCH],
                                 touchFw,
                                 DISP_FW_BYTES);
}

/* Display Register Write */
static bool DISP_WriteRegister(uint16_t reg, uint8_t val, DISP_CLIENT_ID client_id)
{
    if (client_id == DISP_CLIENT_ID_MCU)
    {
        /* Prepare Buffers */
        disp_data.txBuffer[client_id][0] = reg & 0xFF;
        disp_data.txBuffer[client_id][1] = val;
    }
    else if (client_id == DISP_CLIENT_ID_TOUCH)
    {
        /* Prepare Buffers */
        disp_data.txBuffer[client_id][0] = reg >> 8;
        disp_data.txBuffer[client_id][1] = reg & 0xFF;
        disp_data.txBuffer[client_id][2] = val;
    }

    return DRV_I2C_WriteTransfer(disp_data.i2cHandle[client_id],
                                 disp_data.i2cAddress[client_id],
                                 disp_data.txBuffer[client_id],
                                 2 + client_id);
}

/* Display Register Read Multiple */
static bool DISP_ReadRegisterMultiple(uint16_t reg, uint8_t size, DISP_CLIENT_ID client_id)
{
    if (size > DISP_RX_BUF_SIZE)
        size = DISP_RX_BUF_SIZE;

    if (client_id == DISP_CLIENT_ID_MCU)
    {
        /* Prepare Buffers */
        disp_data.txBuffer[client_id][0] = reg & 0xFF;
    }
    else if (client_id == DISP_CLIENT_ID_TOUCH)
    {
        /* Prepare Buffers */
        disp_data.txBuffer[client_id][0] = reg >> 8;
        disp_data.txBuffer[client_id][1] = reg & 0xFF;
    }

    return DRV_I2C_WriteReadTransfer(disp_data.i2cHandle[client_id],
                                     disp_data.i2cAddress[client_id],
                                     disp_data.txBuffer[client_id],
                                     1 + client_id,
                                     disp_data.rxBuffer[client_id],
                                     size);
}

/* Get received client data byte */
static uint8_t DISP_GetRxByte(DISP_CLIENT_ID client_id, uint32_t offset)
{
    if (offset > DISP_RX_BUF_SIZE)
        offset = DISP_RX_BUF_SIZE;

    return disp_data.rxBuffer[client_id][offset];
}

/* Calculate checksum of received bytes */
static uint8_t DISP_GetChecksum(DISP_CLIENT_ID client_id, uint32_t size)
{
    uint8_t checksum = 0;

    for (int i = 0; i < size; i++)
    {
        checksum += DISP_GetRxByte(client_id, i);
    }

    return ((~checksum) + 1);
}

/* Driver Interface Functions */
bool DISP_WS_SetBrightness(uint8_t brightness)
{
    if (disp_data.updateBrightness)
    {
        /* Previous Update Pending */
        return 1;
    }
    else
    {
        /* Update Brightness */
        brightSeq[0][1] = 0xFF - brightness;
        disp_data.updateBrightness = true;
        return 0;
    }
}

void DISP_WS_Initialize(void)
{
    memset(&disp_data, 0, sizeof(disp_data));

    disp_data.state = DISP_STATE_INIT;
    disp_data.i2cAddress[DISP_CLIENT_ID_MCU] = 0x45;
    disp_data.i2cAddress[DISP_CLIENT_ID_TOUCH] = 0x14;
}

void DISP_WS_Update(void)
{
    switch(disp_data.state)
    {
        case DISP_STATE_INIT:
        {
            /* Open I2C driver clients */
            for (int client_id=0; client_id< DISP_MAX_CLIENTS; client_id++)
            {
                disp_data.i2cHandle[client_id] = DRV_I2C_Open(DRV_I2C_INDEX_${I2CIdx}, DRV_IO_INTENT_READWRITE);

                if (disp_data.i2cHandle[client_id] == DRV_HANDLE_INVALID)
                {
                    disp_data.state = DISP_STATE_ERROR;
                    return;
                }
            }

            /* Next State */
            disp_data.state = DISP_STATE_GET_TOUCH_ID;

            break;
        }
        case DISP_STATE_GET_TOUCH_ID:
        {
            if(DISP_ReadRegisterMultiple(0x8140, 3, DISP_CLIENT_ID_TOUCH))
            {
                /* Delay Next State */
                DISP_DelayNextState(1, DISP_STATE_VERIFY_TOUCH_ID);
            }

            break;
        }
        case DISP_STATE_VERIFY_TOUCH_ID:
        {
            for(int i = 0; i < DISP_TCID_BYTES; i++)
            {
                if(DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, i) != chipID[i])
                {
                    disp_data.state = DISP_STATE_ERROR;
                    return;
                }
            }

            /* Next State */
            disp_data.state = DISP_STATE_WRITE_TOUCH_FW;

            break;
        }
        case DISP_STATE_WRITE_TOUCH_FW:
        {
            if(DISP_WriteTouchFw())
            {
                /* Delay Next State */
                DISP_DelayNextState(5, DISP_STATE_APPLY_TOUCH_FW);
            }

            break;
        }
        case DISP_STATE_APPLY_TOUCH_FW:
        {
            /* Set Config Fresh Register to 0x1 */
            if(DISP_WriteRegister(0x8100, 0x1, DISP_CLIENT_ID_TOUCH))
            {
                /* Delay Next State */
                DISP_DelayNextState(10, DISP_STATE_READ_TOUCH_FW);
            }

            break;
        }
        case DISP_STATE_READ_TOUCH_FW:
        {
            /* Read Touch Firmware Configuration */
            if(DISP_ReadRegisterMultiple(0x8047, 186, DISP_CLIENT_ID_TOUCH))
            {
                /* Delay Next State */
                DISP_DelayNextState(5, DISP_STATE_PROCESS_TOUCH_FW);
            }

            break;
        }
        case DISP_STATE_PROCESS_TOUCH_FW:
        {
            if (DISP_GetChecksum(DISP_CLIENT_ID_TOUCH, 184) == DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 184))
            {
                /* Get touch resolution */
                disp_data.touchXRes = DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 2) << 8 |
                                      DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 1);
                disp_data.touchYRes = DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 4) << 8 |
                                      DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 3);
            }
            else
            {
                disp_data.state = DISP_STATE_ERROR;
                return;
            }

            /* Next State */
            disp_data.state = DISP_STATE_POWER_ON;

            break;
        }
        case DISP_STATE_POWER_ON:
        {
            /* Iterate over the sequence */
            if (disp_data.count < DISP_INIT_BYTES)
            {
                if(DISP_WriteRegister(initSeq[disp_data.count][0],
                                      initSeq[disp_data.count][1],
                                      DISP_CLIENT_ID_MCU))
                {
                    disp_data.count++;
                }
                /* Delay next transaction */
                DISP_DelayNextState(1, DISP_STATE_POWER_ON);
            }
            else
            {
                /* Reset count */
                disp_data.count = 0;
                /* Set DSI Video Mode */
                DSI_VideoMode();
                /* Delay Next State */
                DISP_DelayNextState(${DelayBL}, DISP_STATE_SET_BRIGHTNESS);
            }

            break;
        }
        case DISP_STATE_SET_BRIGHTNESS:
        {
            /* Iterate over the sequence */
            if (disp_data.count < DISP_BRT_BYTES)
            {
                if(DISP_WriteRegister(brightSeq[disp_data.count][0],
                                      brightSeq[disp_data.count][1],
                                      DISP_CLIENT_ID_MCU))
                {
                    disp_data.count++;
                }
                /* Delay next transaction */
                DISP_DelayNextState(1, DISP_STATE_SET_BRIGHTNESS);
            }
            else
            {
                /* Reset count */
                disp_data.count = 0;
                /* Update flag */
                disp_data.updateBrightness = false;
                /* Next State */
                disp_data.state = DISP_STATE_IDLE;
            }

            break;
        }
        case DISP_STATE_PROCESS_TOUCH:
        {
            uint32_t touch_points = 0, offset = 0, touch_id = 0;
            uint32_t temp_x = 0, temp_y = 0, temp_status = 0;

            if(DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 0) & 0x80)
            {
                touch_points = DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 0) & 0xF;
                temp_status = 0;

                if(touch_points > 0)
                {
                    if(touch_points > 5)
                    {
                        touch_points = 5;
                    }

                    for(int i = 0; i < touch_points ; i++)
                    {
                        offset = 8 * i;

                        touch_id = DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 1 + offset);
                        temp_x = DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 3 + offset) << 8 |
                                DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 2 + offset);
                        temp_y = DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 5 + offset) << 8 |
                                DISP_GetRxByte(DISP_CLIENT_ID_TOUCH, 4 + offset);

                        temp_status |= 1 << touch_id;

                        temp_x = (temp_x / disp_data.touchXRes) * ${DisplayHeight};
                        temp_y = (temp_y / disp_data.touchYRes) * ${DisplayWidth};

                        <#if TouchOrient == "0">
                        disp_data.touchX[touch_id] = ${DisplayWidth} - 1 - temp_y;
                        disp_data.touchY[touch_id] = temp_x;
                        <#elseif TouchOrient == "90">
                        disp_data.touchX[touch_id] = temp_x;
                        disp_data.touchY[touch_id] = temp_y;
                        <#elseif TouchOrient == "180">
                        disp_data.touchX[touch_id] = temp_y;
                        disp_data.touchY[touch_id] = ${DisplayHeight} - 1 - temp_x;
                        <#elseif TouchOrient == "270">
                        disp_data.touchX[touch_id] = ${DisplayHeight} - 1 - temp_x;
                        disp_data.touchY[touch_id] = ${DisplayWidth} - 1 - temp_y;
                        </#if>
                    }
                }

                for (int i = 0; i < 5; i++)
                {
                    if(temp_status & (1 << i))
                    {
                        if (disp_data.touchStatus[i] == 0)
                        {
                            SYS_INP_InjectTouchDown(i,
                                                    disp_data.touchX[i],
                                                    disp_data.touchY[i]);

                            disp_data.touchStatus[i] = 1;
                        }
                        else
                        {
                            SYS_INP_InjectTouchMove(i,
                                                    disp_data.touchX[i],
                                                    disp_data.touchY[i]);
                        }
                    }
                    else if (disp_data.touchStatus[i] != 0)
                    {
                        SYS_INP_InjectTouchUp(i,
                                              disp_data.touchX[i],
                                              disp_data.touchY[i]);

                        disp_data.touchStatus[i] = 0;
                    }
                }

                DISP_WriteRegister(0x814E, 0 , DISP_CLIENT_ID_TOUCH);
            }

            /* Delay Next State */
            DISP_DelayNextState(1, DISP_STATE_IDLE);

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
            if(disp_data.updateBrightness)
            {
                disp_data.state = DISP_STATE_SET_BRIGHTNESS;
            }
            else if(DISP_ReadRegisterMultiple(0x814E, 1 + 8 * 5, DISP_CLIENT_ID_TOUCH))
            {
                /* Delay Next State */
                DISP_DelayNextState(1, DISP_STATE_PROCESS_TOUCH);
            }

            break;
        }
        default:
        {
            break;
        }
    }
}
