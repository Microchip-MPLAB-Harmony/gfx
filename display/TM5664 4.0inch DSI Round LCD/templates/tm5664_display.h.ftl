/*******************************************************************************
  MPLAB Harmony Generated Display Driver Header File

  File Name:
    tm5664_display.h

  Summary:
    Build-time generated header file that
    adds display and touch driver support for the
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

#ifndef _TM5664_TOUCH_DISPLAY_H
#define _TM5664_TOUCH_DISPLAY_H

#include "driver/i2c/drv_i2c.h"
#include "system/time/sys_time.h"
#include "system/input/sys_input.h"
#include "gfx/driver/controller/xlcdc/bridge/dsi/plib_dsi.h"

/* Buffer Size */
#define DISP_RX_BUF_SIZE    384
#define DISP_TX_BUF_SIZE    4

/* maXTouch IIC Chip Address */
#define MXT_CHIP_ADDRESS    0x4B

/* maXTouch T100 Object Register Offsets */
#define MXT_T100_CFG1_OFFSET    1       // Configuration register 1 (Orientation)
#define MXT_T100_NUMTCH_OFFSET  6       // Number of touches to report
#define MXT_T100_XRANGE_OFFSET  13      // X-axis range (width)
#define MXT_T100_YRANGE_OFFSET  24      // Y-axis range (height)

/* T100 Object Configuration Bits */
#define T100_CFG_INVERTX    (1 << 7)    // Bit 7 to invert X-axis
#define T100_CFG_INVERTY    (1 << 6)    // Bit 6 to invert Y-axis
#define T100_CFG_SWITCHXY   (1 << 5)    // Bit 5 to swap X and Y axes

/* maXTouch on device rotation */
#define MXT_ORIENT_0    (T100_CFG_INVERTX | \
                         T100_CFG_INVERTY | \
                         T100_CFG_SWITCHXY)
#define MXT_ORIENT_90   (T100_CFG_INVERTY)
#define MXT_ORIENT_180  (T100_CFG_SWITCHXY)
#define MXT_ORIENT_270  (T100_CFG_INVERTX)

/* maXTouch User T100 Configuration */
#define MXT_DISPLAY_WIDTH   ${DisplayWidth}
#define MXT_DISPLAY_HEIGHT  ${DisplayHeight}
#define MXT_FINGER_LIMIT    ${TouchMaxPts}
<#if TouchOrient == "0">
#define MXT_DISPLAY_ORIENT  MXT_ORIENT_0
<#elseif TouchOrient == "90">
#define MXT_DISPLAY_ORIENT  MXT_ORIENT_90
<#elseif TouchOrient == "180">
#define MXT_DISPLAY_ORIENT  MXT_ORIENT_180
<#elseif TouchOrient == "270">
#define MXT_DISPLAY_ORIENT  MXT_ORIENT_270
</#if>

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

    /* Initialize display */
    DISP_STATE_DSI_INIT,

    /* Power on display */
    DISP_STATE_DSI_POWER_ON,

    /* Read maXTouch controller info */
    DISP_STATE_MXT_READ_INFO,

    /* Process maXTouch controller information */
    DISP_STATE_MXT_PROCESS_INFO,

    /* Read maXTouch object table */
    DISP_STATE_MXT_READ_OBJECT_TABLE,

    /* Write maXTouch configuration */
    DISP_STATE_MXT_CONFIG_WRITE,

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
  maXTouch Data Structures

  Summary:
    Defines maXTouch data.
*/
typedef struct
{
	uint8_t family_id;
	uint8_t variant_id;
	uint8_t version;
	uint8_t build;
	uint8_t matrix_xsize;
	uint8_t matrix_ysize;
	uint8_t object_num;
} MXT_INFO;

typedef struct
{
	uint8_t type;
	uint16_t start_address;
	uint8_t size_minus_one;
	uint8_t instances_minus_one;
	uint8_t num_report_ids;
} __attribute__((packed)) MXT_OBJECT;

typedef struct
{
    uint16_t   reg_offset;
    uint16_t   value;
    bool       is_16bit;
} MXT_T100_USER_CONFIG;

/*******************************************************************************
  Display Driver Data

  Summary:
    Holds application data.
*/
typedef struct
{
    /* Current state */
    DISP_STATES  state;

    /* Next state */
    DISP_STATES  stateNext;

    /* Handle to delay timer */
    SYS_TIME_HANDLE dlyHandle;

    /* Handle to periodic system timer */
    SYS_TIME_HANDLE tmrHandle;

    /* Flag to indicate timer expiry */
    volatile bool tmrExpired;

    /* I2C driver client handle */
    DRV_HANDLE i2cHandle;

    /* I2C driver client address */
    uint8_t i2cAddress;

    /* I2C driver transfer handle */
    DRV_I2C_TRANSFER_HANDLE transferHandle;

    /* I2C Receive Buffer */
    uint8_t rxBuffer[DISP_RX_BUF_SIZE];

    /* I2C Transmit Buffer */
    uint8_t txBuffer[DISP_TX_BUF_SIZE];

    /* Set transfer busy state */
    volatile bool transferBusy;

    /* Set transfer NACK state */
    volatile bool transferNack;

    /* maXTouch firmware display width */
    float touchXRes;

    /* maXTouch firmware display height */
    float touchYRes;

    /* maXTouch parsed data */
    MXT_INFO mxtInfoBlock;
    uint16_t mxtT5Address;
    uint8_t  mxtT5MsgSize;
    uint16_t mxtT100Address;
    uint8_t  mxtT100ReportIDMin;
    uint8_t  mxtT100ReportIdMax;

    /* maXTouch user configuration index */
    uint8_t  mxtUserConfigIdx;

    /* maXTouch X coordinates */
    uint32_t mxtTouchX[MXT_FINGER_LIMIT];

    /* maXTouch Y coordinates */
    uint32_t mxtTouchY[MXT_FINGER_LIMIT];
} DISP_DATA;

/*******************************************************************************
  Function:
    void DISP_TM5664_Initialize(void)

  Summary:
    Initializes the display.
*/
void DISP_TM5664_Initialize(void);

/*******************************************************************************
  Function:
    void DISP_TM5664_Update(void)

  Summary:
    Polls for touch events and processes it.
*/
void DISP_TM5664_Update(void);

/*******************************************************************************
  Function:
    bool DISP_TM5664_SetBrightness(uint8_t brightness)

  Summary:
    Sets the display brightness.

  Parameters:
    brightness - 8-bit value from 0 (off) to 255 (brightest).

  Returns:
    None.
*/
void DISP_TM5664_SetBrightness(uint8_t brightness);

#ifdef __cplusplus
    }
#endif

#endif
