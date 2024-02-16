/*******************************************************************************
  MPLAB Harmony Generated Display Driver Header File

  File Name:
    raspi_touch_display.h

  Summary:
    Build-time generated header file that
    adds display and touch driver support for the Raspberry Pi Touch Display.
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

#ifndef _RPI_TOUCH_DISPLAY_H
#define _RPI_TOUCH_DISPLAY_H

#include "driver/i2c/drv_i2c.h"
#include "system/time/sys_time.h"
#include "system/input/sys_input.h"
#include "gfx/driver/controller/xlcdc/bridge/dsi/plib_dsi.h"

/* Buffer Size */
#define DISP_RX_BUF_SIZE    32
#define DISP_TX_BUF_SIZE    4

#ifdef __cplusplus
    extern "C" {
#endif

/*******************************************************************************
  Display Driver State

  Summary:
    Holds application data.
*/
typedef enum
{
    /* Initial state. */
    DISP_STATE_INIT,

    /* Power on display */
    DISP_STATE_POWER_ON,

    /* Wait for Power on */
    DISP_STATE_WAIT_POWER_ON,

    /* Set Display Horizontal Orientation*/
    DISP_STATE_SET_HFLIP,

    /* Configure MIPI Host Bridge */
    DISP_STATE_CONFIG_MIPI_HOST,

    /* Set Display Brightness */
    DISP_STATE_SET_BRIGHTNESS,

    /* Process touch */
    DISP_STATE_PROCESS_TOUCH,

    /* Wait for specified time */
    DISP_STATE_DELAY,

    /* Error */
    DISP_STATE_ERROR,

    /* Idle */
    DISP_STATE_IDLE
} DISP_STATES;

/*******************************************************************************
  Client ID

  Summary:
    Selects Client ID.
*/
typedef enum
{
    DISP_CLIENT_ID_MCU = 0,
    DISP_CLIENT_ID_TOUCH,
    DISP_MAX_CLIENTS
} DISP_CLIENT_ID;

/*******************************************************************************
  Display Driver Data

  Summary:
    Holds application data.
*/
typedef struct
{
    /* Current state */
    DISP_STATES  state;

    /* Current state */
    DISP_STATES  stateNext;

    /* Handle to delay timer */
    SYS_TIME_HANDLE dlyHandle;

    /* Handle to periodic system timer */
    SYS_TIME_HANDLE tmrHandle;

    /* Flag to indicate timer expiry */
    volatile bool tmrExpired;

    /* I2C driver client handle */
    DRV_HANDLE i2cHandle[DISP_MAX_CLIENTS];

    /* I2C driver client address */
    uint8_t i2cAddress[DISP_MAX_CLIENTS];

    /* I2C driver transfer handle */
    DRV_I2C_TRANSFER_HANDLE transferHandle[DISP_MAX_CLIENTS];

    /* I2C Receive Buffer */
    uint8_t rxBuffer[DISP_MAX_CLIENTS][DISP_RX_BUF_SIZE];

    /* I2C Transmit Buffer */
    uint8_t txBuffer[DISP_MAX_CLIENTS][DISP_TX_BUF_SIZE];

    /* Set transfer busy state */
    volatile bool transferBusy[DISP_MAX_CLIENTS];

    /* Set transfer NACK state */
    volatile bool transferNack[DISP_MAX_CLIENTS];

    /* Touch X coordinates */
    uint32_t touchX[5];

    /* Touch Y coordinates */
    uint32_t touchY[5];
    
    /* Touch point ID */
    uint32_t touchID[5];

} DISP_DATA;

/*******************************************************************************
  Function:
    void DISP_RPi_Initialize(void)

  Summary:
    Initializes the display.
*/
void DISP_RPi_Initialize(void);

/*******************************************************************************
  Function:
    void DISP_RPi_Update(void)

  Summary:
    Polls for touch events and processes it.
*/
void DISP_RPi_Update(void);

/*******************************************************************************
  Function:
    bool DISP_RPi_SetBrightness(uint8_t brightness)

  Summary:
    Sets the display brightness.

  Parameters:
    brightness - 8-bit value from 0 (off) to 255 (brightest).

  Returns:
    0, on success. 1, on failure.
*/
bool DISP_RPi_SetBrightness(uint8_t brightness);

#ifdef __cplusplus
    }
#endif

#endif
