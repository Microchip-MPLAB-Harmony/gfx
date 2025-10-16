/*******************************************************************************
  MPLAB Harmony Generated Display Driver Async Implementation File

  File Name:
    tm5664_display.h

  Summary:
    Implements display and touch driver support for the
    TM5664 4.0inch DSI Round LCD w/ maXTouch Display.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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

#include "gfx/display/tm5664_display.h"
#include "string.h"

/* Driver Data */
DISP_DATA disp_data;

/* The array of commands to be written to the maXTouch controller */
static const MXT_T100_USER_CONFIG mxtUsrCfg[] =
{
    //Register Offset,          Value,                       is_16bit
    { MXT_T100_CFG1_OFFSET,     MXT_DISPLAY_ORIENT,          false }, // Orientation
    { MXT_T100_NUMTCH_OFFSET,   MXT_FINGER_LIMIT,            false }, // Max Fingers
    { MXT_T100_XRANGE_OFFSET,   MXT_DISPLAY_WIDTH - 1,       true  }, // X Resolution
    { MXT_T100_YRANGE_OFFSET,   MXT_DISPLAY_HEIGHT - 1,      true  }, // Y Resolution
};

#define NUM_CONFIG_PARAMS (sizeof(mxtUsrCfg) / sizeof(mxtUsrCfg[0]))

/* Local Functions */
/* Handle I2C events from all clients */
static void DISP_I2CEventHandler(DRV_I2C_TRANSFER_EVENT event, DRV_I2C_TRANSFER_HANDLE transferHandle, uintptr_t context)
{
    switch(event)
    {
        case DRV_I2C_TRANSFER_EVENT_COMPLETE:
        {
            disp_data.transferBusy = false;
            disp_data.transferNack = false;

            break;
        }
        case DRV_I2C_TRANSFER_EVENT_ERROR:
        {
            DRV_I2C_ERROR err = DRV_I2C_ErrorGet(transferHandle);

            if(err == DRV_I2C_ERROR_NACK)
            {
                disp_data.transferBusy = false;
                disp_data.transferNack = true;
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

/* Display Driver Helper for DCS Writes */
static void DISP_DSI_DCS_Write(uint8_t * data, size_t size) {
    DSI_GENERIC_HEADER header;
    DSI_GENERIC_PAYLOAD payload[64];

    memset(&header, 0, sizeof(header));
    memset(payload, 0, sizeof(payload));

    switch (size) {
        case 0:
        {
            break;
        }
        case 1:
        {
            header.shortPacket.dataType = 0x05; // DCS SP, no parameter
            header.shortPacket.data[0] = data[0];
            DSI_Write(&header, payload);

            break;
        }
        case 2:
        {
            header.shortPacket.dataType = 0x15; // DCS SP, 1 parameter
            header.shortPacket.data[0] = data[0];
            header.shortPacket.data[1] = data[1];
            DSI_Write(&header, payload);

            break;
        }
        default:
        {
            header.longPacket.dataType = 0x39; // DCS LP, no parameter
            header.longPacket.size = size;
            memcpy(payload, data, size);
            DSI_Write(&header, payload);

            break;
        }
    }
}

/* Macro for DCS Sequences */
#define DISP_DSI_DCS_WriteSequence(seq...)\
({\
	static uint8_t x[] = { seq };\
	DISP_DSI_DCS_Write(x, sizeof(x));\
})

/* Helper to initialize the LCD display. */
static void DISP_DSI_LCD_Init()
{
    // Enable User Command Set (SETEXTC)
    DISP_DSI_DCS_WriteSequence(0xB9, 0xF1, 0x12, 0x83);

    // Command B1
    DISP_DSI_DCS_WriteSequence(0xB1, 0x00, 0x00, 0x00, 0xDA, 0x80);

    // Set Display Resolution (SETDISP)
    DISP_DSI_DCS_WriteSequence(0xB2, 0x3C, 0x12, 0x30);

    // Set RGB Interface (SETRGBIF)
    DISP_DSI_DCS_WriteSequence(0xB3, 0x10, 0x10, 0x78, 0x78, 0x03, 0xFF,
                               0x00, 0x00, 0x00, 0x00);

    // Set Panel Inversion (SETCYC)
    DISP_DSI_DCS_WriteSequence(0xB4, 0x80);

    // Set BGP
    DISP_DSI_DCS_WriteSequence(0xB5, 0x0F, 0x0F);

    // Set VCOM
    DISP_DSI_DCS_WriteSequence(0xB6, 0x4D, 0x4D);

    // Set ECP (SETPOWER_EXT)
    DISP_DSI_DCS_WriteSequence(0xB8, 0x26, 0x22, 0xF0, 0x63);

    // Set DSI (SETMIPI)
    DISP_DSI_DCS_WriteSequence(0xBA, 0x31, 0x81, 0x05, 0xF9, 0x0E, 0x0E,
                               0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x44, 0x25, 0x00, 0x90, 0x0A, 0x00,
                               0x00, 0x01, 0x4F, 0x01, 0x00, 0x00, 0x37);

    // Set VDC (SETVDC)
    DISP_DSI_DCS_WriteSequence(0xBC, 0x47);

    // Set PCR
    DISP_DSI_DCS_WriteSequence(0xBF, 0x02, 0x11, 0x00);

    // Set Source Control (SETSCR)
    DISP_DSI_DCS_WriteSequence(0xC0, 0x73, 0x73, 0x50, 0x50, 0x00, 0x00,
                               0x12, 0x70, 0x00);

    // Set Power (SETPOWER)
    DISP_DSI_DCS_WriteSequence(0xC1, 0x25, 0x00, 0x32, 0x32, 0x77, 0xE4,
                               0xFF, 0xFF, 0xCC, 0xCC, 0x77, 0x77);

    // Command C6
    DISP_DSI_DCS_WriteSequence(0xC6, 0x82, 0x00, 0xBF, 0xFF, 0x00, 0xFF);

    // Command C7
    DISP_DSI_DCS_WriteSequence(0xC7, 0xB8, 0x00, 0x0A, 0x00, 0x00, 0x00);

    // Command C8
    DISP_DSI_DCS_WriteSequence(0xC8, 0x10, 0x40, 0x1E, 0x02);

    // Set Panel (SETPANEL)
    DISP_DSI_DCS_WriteSequence(0xCC, 0x0B);

    // Set Gamma 2.2 (SETGAMMA)
    DISP_DSI_DCS_WriteSequence(0xE0, 0x00, 0x0E, 0x1A, 0x29, 0x3A, 0x3F,
                               0x4C, 0x3E, 0x06, 0x0D, 0x0F, 0x13, 0x15,
                               0x13, 0x14, 0x11, 0x18, 0x00, 0x0E, 0x1A,
                               0x29, 0x3A, 0x3F, 0x4C, 0x3E, 0x06, 0x0D,
                               0x0F, 0x13, 0x14, 0x13, 0x15, 0x11, 0x18);

    // Set EQ (SETEQ)
    DISP_DSI_DCS_WriteSequence(0xE3, 0x03, 0x03, 0x03, 0x03, 0x00, 0x03,
                               0x00, 0x00, 0x00, 0x00, 0xFF, 0x80, 0xC0,
                               0x10);

    // Set GIP1 (SETGIP1)
    DISP_DSI_DCS_WriteSequence(0xE9, 0xC8, 0x10, 0x0A, 0x10, 0x0E, 0x80,
                               0x38, 0x12, 0x31, 0x23, 0x4F, 0x86, 0x80,
                               0x38, 0x47, 0x08, 0x18, 0x00, 0x10, 0x00,
                               0x00, 0x00, 0x18, 0x00, 0x10, 0x00, 0x00,
                               0x00, 0x88, 0xFF, 0x44, 0x46, 0x60, 0x02,
                               0x28, 0x88, 0x8F, 0xF0, 0x28, 0x88, 0xFF,
                               0x45, 0x57, 0x71, 0x13, 0x38, 0x88, 0x8F,
                               0xF1, 0x38, 0x00, 0x00, 0x00, 0x01, 0x00,
                               0x80, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00);

    // Set GIP2 (SETGIP2)
    DISP_DSI_DCS_WriteSequence(0xEA, 0x96, 0x18, 0x00, 0x00, 0x00, 0xB4,
                               0x01, 0x0B, 0x41, 0x01, 0x02, 0x00, 0xFF,
                               0x88, 0x03, 0x31, 0x17, 0x75, 0x58, 0x88,
                               0x8F, 0xF2, 0x08, 0xFF, 0x88, 0x02, 0x20,
                               0x06, 0x64, 0x48, 0x88, 0x8F, 0xF2, 0x08,
                               0x23, 0x00, 0x00, 0x02, 0x58, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x07, 0x8B, 0x00, 0x00, 0x40, 0x80,
                               0x81, 0x40, 0x80, 0x81, 0x00);

    // Set EQ2
    DISP_DSI_DCS_WriteSequence(0xEF, 0xFF, 0xFF, 0x01);

    // Sleep Out
    DISP_DSI_DCS_WriteSequence(0x11);
}

/* Helper to power-on the LCD display. */
static void DISP_DSI_LCD_PowerOn()
{
    // DBV=${DefaultBL} (Display Brightness)
    DISP_DSI_DCS_WriteSequence(0x51, ${DefaultBL});

    // BCTRL=1 (Brightness Control ON), BL=1 (Backlight ON)
    DISP_DSI_DCS_WriteSequence(0x53, 0x24);

    // CABC PWM output
    DISP_DSI_DCS_WriteSequence(0xC7, 0xA0);

    // Display On
    DISP_DSI_DCS_WriteSequence(0x29);
}

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
/* Display Register Write */
static void DISP_WriteRegister(uint16_t reg, uint16_t val, bool is_val_16bit)
{
    /* Set Busy Flag */
    disp_data.transferBusy = true;

    /* Prepare Buffers */
    disp_data.txBuffer[0] = reg & 0xFF;
    disp_data.txBuffer[1] = reg >> 8;
    disp_data.txBuffer[2] = val & 0xFF;
    disp_data.txBuffer[3] = val >> 8;

    DRV_I2C_WriteTransferAdd(disp_data.i2cHandle,
                             disp_data.i2cAddress,
                             disp_data.txBuffer,
                             is_val_16bit ? 4 : 3,
                             &disp_data.transferHandle);
}

/* Display Register Read Multiple */
static void DISP_ReadRegisterMultiple(uint16_t reg, uint16_t size)
{
    if (size > DISP_RX_BUF_SIZE)
        size = DISP_RX_BUF_SIZE;

    // Set Busy Flag
    disp_data.transferBusy = true;

    // Prepare Buffers
    disp_data.txBuffer[0] = reg & 0xFF;
    disp_data.txBuffer[1] = reg >> 8;

    DRV_I2C_WriteReadTransferAdd(disp_data.i2cHandle,
                                 disp_data.i2cAddress,
                                 disp_data.txBuffer,
                                 2,
                                 disp_data.rxBuffer,
                                 size,
                                 &disp_data.transferHandle);
}

/* Check if client is busy */
static bool DISP_TransferBusy()
{
    return disp_data.transferBusy;
}

/* Check if client NACK'd transfer, only invoke after transfer busy check */
static bool DISP_TransferNack()
{
    return disp_data.transferNack;
}

/* Get received client data byte */
static uint8_t DISP_GetRxByte(uint32_t offset)
{
    if (offset > DISP_RX_BUF_SIZE)
        offset = DISP_RX_BUF_SIZE;

    return disp_data.rxBuffer[offset];
}

/* Helper function to parse the maXTouch object table */
static void DISP_MXT_ParseObjectTable(void)
{
    uint8_t reportid = 1;
    MXT_OBJECT* object_table = (MXT_OBJECT*)(disp_data.rxBuffer + sizeof(MXT_INFO));

    // Iterate through all objects and cache addresses
	for (int i = 0; i < disp_data.mxtInfoBlock.object_num; i++)
    {
		MXT_OBJECT *object = object_table + i;
        uint8_t instances = object->instances_minus_one + 1;
        uint8_t min_id, max_id;

        // Calculate the report ID range for objects that have them.
		if (object->num_report_ids > 0)
        {
            min_id = reportid;
            reportid += object->num_report_ids * instances;
            max_id = reportid - 1;
        }
        else
        {
            min_id = 0;
            max_id = 0;
        }

        // Identify objects by their type.
        switch(object->type)
        {
            case 5: // MXT_GEN_MESSAGE_T5
            {
                disp_data.mxtT5Address = object->start_address;
                // Message size is object size minus 1 byte for CRC
                disp_data.mxtT5MsgSize = (object->size_minus_one + 1) - 1;

                break;
            }

            case 100: // MXT_TOUCH_MULTITOUCHSCREEN_T100
            {
                disp_data.mxtT100Address = object->start_address;
                disp_data.mxtT100ReportIDMin = min_id;
                disp_data.mxtT100ReportIdMax = max_id;

                break;
            }

            default:
            {
                break;
            }
        }
	}
}

/* Driver Interface Functions */
void DISP_TM5664_SetBrightness(uint8_t brightness)
{
    uint8_t brt_seq[2] = { 0x51, 0 };
    brt_seq[1] = brightness;
    DISP_DSI_DCS_Write(brt_seq, 2);
}

void DISP_TM5664_Initialize(void)
{
    memset(&disp_data, 0, sizeof(disp_data));

    disp_data.state = DISP_STATE_INIT;
    disp_data.i2cAddress = MXT_CHIP_ADDRESS;
}

void DISP_TM5664_Update(void)
{
    static uint16_t table_size = 0;

    switch(disp_data.state)
    {
        case DISP_STATE_INIT:
        {
            /* Open I2C driver client */
            disp_data.i2cHandle = DRV_I2C_Open(DRV_I2C_INDEX_${I2CIdx}, DRV_IO_INTENT_READWRITE);

            if (disp_data.i2cHandle != DRV_HANDLE_INVALID)
            {
                /* Register the I2C Drivers event callback */
                DRV_I2C_TransferEventHandlerSet(disp_data.i2cHandle, DISP_I2CEventHandler, 0);
            }
            else
            {
                disp_data.state = DISP_STATE_ERROR;
                return;
            }

            /* Enable DSI Video Mode */
            DSI_VideoMode();

            /* Next State */
            disp_data.state = DISP_STATE_DSI_INIT;

            break;
        }
        case DISP_STATE_DSI_INIT:
        {
            DISP_DSI_LCD_Init();
            DISP_DelayNextState(${DelayBL}, DISP_STATE_DSI_POWER_ON);

            break;
        }
        case DISP_STATE_DSI_POWER_ON:
        {
            DISP_DSI_LCD_PowerOn();

            // Forward read MXT Block
            DISP_ReadRegisterMultiple(0, sizeof(MXT_INFO));

            DISP_DelayNextState(50, DISP_STATE_MXT_READ_INFO);

            break;
        }
        case DISP_STATE_MXT_READ_INFO:
        {
            if (!DISP_TransferBusy())
            {
                if (DISP_TransferNack())
                {
                    /* Retry on NACK */
                    DISP_ReadRegisterMultiple(0, sizeof(MXT_INFO));
                    DISP_DelayNextState(10, DISP_STATE_MXT_READ_INFO);
                }
                else
                {
                    /* Read was successful, now process the info */
                    disp_data.state = DISP_STATE_MXT_PROCESS_INFO;
                }
            }

            break;
        }
        case DISP_STATE_MXT_PROCESS_INFO:
        {
            /* Copy the info block from the generic RX buffer */
            memcpy(&disp_data.mxtInfoBlock,
                   disp_data.rxBuffer,
                   sizeof(MXT_INFO));

            /* Now we know how many objects there are, read the full table */
            table_size = disp_data.mxtInfoBlock.object_num * sizeof(MXT_OBJECT);
            if (table_size > 0 && table_size < DISP_RX_BUF_SIZE)
            {
                DISP_ReadRegisterMultiple(0, sizeof(MXT_INFO) + table_size);
                DISP_DelayNextState(50, DISP_STATE_MXT_READ_OBJECT_TABLE);
            }
            else
            {
                disp_data.state = DISP_STATE_ERROR; // Invalid size
            }

            break;
        }
        case DISP_STATE_MXT_READ_OBJECT_TABLE:
        {
            if (!DISP_TransferBusy())
            {
                if (DISP_TransferNack())
                {
                    /* Retry on NACK */
                    DISP_ReadRegisterMultiple(0, sizeof(MXT_INFO) + table_size);
                    DISP_DelayNextState(50, DISP_STATE_MXT_READ_OBJECT_TABLE);
                }
                else
                {
                    /* Read was successful, now process the info. */
                    DISP_MXT_ParseObjectTable();
                    disp_data.state = DISP_STATE_MXT_CONFIG_WRITE;
                }
            }

            break;
        }
        case DISP_STATE_MXT_CONFIG_WRITE:
        {
            if (DISP_TransferBusy())
            {
                break;
            }

            if (!DISP_TransferNack())
            {
                disp_data.mxtUserConfigIdx++;
            }

            if (disp_data.mxtUserConfigIdx > NUM_CONFIG_PARAMS)
            {
                disp_data.state = DISP_STATE_IDLE;
                break;
            }

            const MXT_T100_USER_CONFIG* cfg = &mxtUsrCfg[disp_data.mxtUserConfigIdx - 1];

            DISP_WriteRegister(disp_data.mxtT100Address + cfg->reg_offset,
                               cfg->value,
                               cfg->is_16bit);

            break;
        }
        case DISP_STATE_PROCESS_TOUCH:
        {
            if(DISP_TransferBusy())
            {
                break;
            }

            uint8_t reportID = DISP_GetRxByte(0);

            // reportID == 0xFF means no new messages
            if (reportID != 0xFF && disp_data.mxtT100ReportIDMin > 0)
            {
                // Check if the message is from the T100 multi-touch object
                if (reportID >= disp_data.mxtT100ReportIDMin && reportID <= disp_data.mxtT100ReportIdMax)
                {
                    uint8_t touch_id = reportID - disp_data.mxtT100ReportIDMin - 2;

                    if (touch_id < MXT_FINGER_LIMIT)
                    {
                        uint8_t status = DISP_GetRxByte(1);
                        uint16_t temp_x = (DISP_GetRxByte(3) << 8) | DISP_GetRxByte(2);
                        uint16_t temp_y = (DISP_GetRxByte(5) << 8) | DISP_GetRxByte(4);

                        uint8_t event = status & 0x0F;

                        switch(event)
                        {
                            case 0x4: // Touch Down
                                SYS_INP_InjectTouchDown(touch_id, temp_x, temp_y);
                                break;
                            case 0x5: // Touch Up
                                SYS_INP_InjectTouchUp(touch_id, temp_x, temp_y);
                                break;
                            case 0x1: // Touch Move
                                SYS_INP_InjectTouchMove(touch_id, temp_x, temp_y);
                                break;
                        }
                    }
                }
            }

            // Go back to idle after a short delay to poll for the next message
            DISP_DelayNextState(10, DISP_STATE_IDLE);

            break;
        }
        case DISP_STATE_IDLE:
        {
            if(DISP_TransferBusy())
            {
                break;
            }

            if (disp_data.mxtT5Address > 0)
            {
                /* Read from the maXTouch T5 message object */
                DISP_ReadRegisterMultiple(disp_data.mxtT5Address,
                                          disp_data.mxtT5MsgSize);

                /* Transition to the processing state */
                disp_data.state = DISP_STATE_PROCESS_TOUCH;
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
        default:
        {
            break;
        }
    }
}
