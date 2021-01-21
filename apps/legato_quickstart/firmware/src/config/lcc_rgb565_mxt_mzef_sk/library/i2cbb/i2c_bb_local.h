/*******************************************************************************
  I2C BITBANG Library Interface Header File

  Company
    Microchip Technology Inc.

  File Name
    I2C_BB_local.h

  Summary
    I2C BITBANG library interface.

  Description

  Remarks:

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef I2C_BB_LOCAL_H
#define I2C_BB_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/*  This section lists the other files that are included in this file.
*/
#include <stdbool.h>
#include <stddef.h>
#include "system/ports/sys_ports.h"
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END
#define OUTPUT              0
#define INPUT               1

#define LOW                 0
#define HIGH                1

#define M_ACK               0
#define M_NACK              1

#define I2C_BB_IRQn   9
#define I2C_BB_TMR_CLOCK_FREQUENCY  100000000

#define I2C_BB_DATA_PIN SYS_PORT_PIN_RA15
#define I2C_BB_CLK_PIN  SYS_PORT_PIN_RA14

#define TIME_HW_COUNTER_WIDTH            32
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

typedef enum
{
    I2CBB_BUS_STATE_NULL_STATE = 0,

    /* Start condition on I2C BUS with SDA low */
    I2CBB_BUS_STATE_SDA_LOW_START,

    /* I2C SDA low start check */
    I2CBB_BUS_STATE_SDA_LOW_START_CHECK,

    /* Start condition on I2C Bus with SCL low after SDA low */
    I2CBB_BUS_STATE_SCL_LOW_START,

    /* I2C SCL low start check */
    I2CBB_BUS_STATE_SCL_LOW_START_CHECK,

    /* setting SDA high on RESTART */
    I2CBB_BUS_STATE_SDA_HIGH_RESTART,

    /* check SDA high RESTART */
    I2CBB_BUS_STATE_SDA_HIGH_RESTART_CHECK,

    /* setting SCL high on RESTART */
    I2CBB_BUS_STATE_SCL_HIGH_RESTART,

    /* check SCL high on RESTART */
    I2CBB_BUS_STATE_SCL_HIGH_RESTART_CHECK,

    /* SCL High during data transfer to ensure Data is not changing */
    I2CBB_BUS_STATE_SCL_HIGH_DATA,

    /* High data check */
    I2CBB_BUS_STATE_SCL_HIGH_DATA_CHECK,

    /* SCL Low during data transfer where data can change */
    I2CBB_BUS_STATE_SCL_LOW_DATA,

    /* SCL low data check */
    I2CBB_BUS_STATE_SCL_LOW_DATA_CHECK,

    /* keep SCL and SDA low for 1 BRG time */
    I2CBB_BUS_STATE_SCL_SDA_LOW_STOP,

    /* keep SCL and SDA low check */
    I2CBB_BUS_STATE_SCL_SDA_LOW_STOP_CHECK,

    /* SCL going high during STOP condition */
    I2CBB_BUS_STATE_SCL_HIGH_STOP,

    /* SCL going HIGH check*/
    I2CBB_BUS_STATE_SCL_HIGH_STOP_CHECK,

    /* SDA going low during STOP condition */
    I2CBB_BUS_STATE_SDA_HIGH_STOP,

    /* SDA going high STOP check */
    I2CBB_BUS_STATE_SDA_HIGH_STOP_CHECK

} I2CBB_BUS_STATE;

typedef enum
{
    /* No error has occurred. */
   I2CBB_ERROR_NONE,

    /* A bus transaction was NAK'ed */
   I2CBB_ERROR_NAK,

    /* A bus error has occurred. */
   I2CBB_ERROR_BUS,

} I2CBB_ERROR;

typedef enum
{

  I2CBB_TRANSFER_STATE_READ,

  I2CBB_TRANSFER_STATE_WRITE,

  I2CBB_TRANSFER_STATE_WRITE_READ,

  I2CBB_TRANSFER_STATE_DONE,

}I2CBB_TRANSFER_STATE;

typedef void (* I2CBB_CALLBACK)( uintptr_t context );

typedef void (*I2C_BB_TMR_PLIB_CALLBACK)(uint32_t, uintptr_t);

typedef void (*I2C_BB_TMR_PLIB_START)(void);

typedef void (*I2C_BB_TMR_PLIB_STOP)(void);

typedef void (*I2C_BB_TMR_PLIB_CALLBACK_REGISTER)( I2C_BB_TMR_PLIB_CALLBACK callback, uintptr_t context);

#if (TIME_HW_COUNTER_WIDTH == 16)
typedef void (*I2C_BB_TMR_PLIB_PERIOD_SET)(uint16_t period);
#else
typedef void (*I2C_BB_TMR_PLIB_PERIOD_SET)(uint32_t period);
#endif
typedef struct
{
    I2C_BB_TMR_PLIB_START                   timerStart;

    I2C_BB_TMR_PLIB_STOP                    timerStop;

    I2C_BB_TMR_PLIB_PERIOD_SET              timerPeriodset;

    I2C_BB_TMR_PLIB_CALLBACK_REGISTER       timerCallbackRegister;
}I2C_BB_TMR_PLIB_INTERFACE;

typedef struct
{
    const I2C_BB_TMR_PLIB_INTERFACE*       i2cbbTmrPlib;

    SYS_PORT_PIN                           i2cbbSDAPin;

    SYS_PORT_PIN                           i2cbbSCLPin;

    uint32_t                               i2cClockSpeed;
}I2C_BB_INIT;

typedef struct
{
    /* I2C Clock Speed */
    uint32_t clkSpeed;

} I2CBB_TRANSFER_SETUP;

typedef struct _I2CBB_OBJ
{
    I2CBB_BUS_STATE        i2cState;

    uint32_t               i2cClockSpeed;

    uint32_t               timerSrcClkFreq;

    uint16_t               I2CSWCounter;

    uint16_t               I2CSWData;

    bool                   I2CNACKOut;

    bool                   I2CACKStatus;

    uint16_t               I2CReadData;

    uint32_t               errorTimeOut;

    bool                  _i2c_bit_written;

    bool                  ACKSTATUS_M;

    uint16_t              address;

    uint8_t               *writeBuffer;

    uint8_t               *readBuffer;

    size_t                writeSize;

    size_t                readSize;

    size_t                writeCount;

    size_t                readCount;

    volatile              I2CBB_TRANSFER_STATE transferState;

    volatile              I2CBB_ERROR errorStatus;

    I2CBB_CALLBACK        callback;

    uintptr_t context;

} I2CBB_OBJ;

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif // I2C_BB_LOCAL_H

/*******************************************************************************
 End of File
*/
