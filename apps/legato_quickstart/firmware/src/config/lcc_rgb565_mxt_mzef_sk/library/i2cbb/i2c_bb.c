/*******************************************************************************
  I2C BIT BANG Library Source File

  Company
    Microchip Technology Inc.

  File Name
    i2c_bb.c

  Summary
    I2C BIT BANG library interface.

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

// *****************************************************************************
// *****************************************************************************
// Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "i2c_bb.h"
#include "definitions.h"
// *****************************************************************************
// *****************************************************************************
// Local Data Type Definitions
// *****************************************************************************
// *****************************************************************************
#define ERROR_TIMEOUT_COUNT         2000000


// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************

static I2CBB_OBJ i2c_bbObj;

const I2C_BB_TMR_PLIB_INTERFACE i2cTimerPlibAPI = {
    .timerStart = (I2C_BB_TMR_PLIB_START)TMR2_Start,

    .timerStop = (I2C_BB_TMR_PLIB_STOP)TMR2_Stop ,

    .timerPeriodset = (I2C_BB_TMR_PLIB_PERIOD_SET)TMR2_PeriodSet,

    .timerCallbackRegister = (I2C_BB_TMR_PLIB_CALLBACK_REGISTER)TMR2_CallbackRegister
};


const I2C_BB_INIT i2cBBInitData =
{
    .i2cbbTmrPlib        = &i2cTimerPlibAPI,
    .i2cbbSDAPin         = I2C_BB_DATA_PIN,
    .i2cbbSCLPin         = I2C_BB_CLK_PIN,
    .i2cClockSpeed       = 50000

};

static void I2C_BB_InitiateTransfer(void)
{
    uint32_t timerPeriod;
    I2C_BB_INIT* pInitData =(I2C_BB_INIT*)&i2cBBInitData;

    timerPeriod = i2c_bbObj.timerSrcClkFreq/((i2c_bbObj.i2cClockSpeed)<<2);

    i2c_bbObj.writeCount = 0;
    i2c_bbObj.readCount = 0;
    /* send START command only if SCL and SDA are pulled high */
    if ((SYS_PORT_PinRead(pInitData->i2cbbSCLPin) == HIGH) && (SYS_PORT_PinRead(pInitData->i2cbbSDAPin) == HIGH))
    {
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SDA_LOW_START;
        pInitData->i2cbbTmrPlib->timerStop();
        pInitData->i2cbbTmrPlib->timerPeriodset(timerPeriod);
        pInitData->i2cbbTmrPlib->timerStart();
        i2c_bbObj.I2CNACKOut = false;
        i2c_bbObj.I2CACKStatus = false;

        i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT;
    }
}

static void I2C_BB_tasks(void)
{
    I2C_BB_INIT* pInitData =(I2C_BB_INIT*)&i2cBBInitData;

    switch(i2c_bbObj.i2cState)
    {
      case I2CBB_BUS_STATE_SDA_LOW_START:
        SYS_PORT_PinOutputEnable(pInitData->i2cbbSDAPin);
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SDA_LOW_START_CHECK;
        break;

      case I2CBB_BUS_STATE_SDA_LOW_START_CHECK:
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_LOW_START;
        break;

      case I2CBB_BUS_STATE_SCL_LOW_START:
        SYS_PORT_PinOutputEnable(pInitData->i2cbbSCLPin);
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_LOW_START_CHECK;
        break;

      case I2CBB_BUS_STATE_SCL_LOW_START_CHECK:

        if ((i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_WRITE_READ) &&
          (i2c_bbObj.writeCount > (i2c_bbObj.writeSize))) {
          i2c_bbObj.transferState = I2CBB_TRANSFER_STATE_READ;
        }
        i2c_bbObj.I2CSWData = i2c_bbObj.address;
        if (i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_READ) {
          i2c_bbObj.I2CSWData |= 0x01;
        }
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_LOW_DATA_CHECK;
        break;

      case I2CBB_BUS_STATE_SDA_HIGH_RESTART:
        SYS_PORT_PinInputEnable(pInitData->i2cbbSDAPin);
        i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT;
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SDA_HIGH_RESTART_CHECK;
        break;

      case I2CBB_BUS_STATE_SDA_HIGH_RESTART_CHECK:
        if (SYS_PORT_PinRead(pInitData->i2cbbSDAPin) == INPUT) {
          i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_HIGH_RESTART;
        } else {
          if (!(i2c_bbObj.errorTimeOut--)) {
            i2c_bbObj.errorStatus = I2CBB_ERROR_BUS;
            pInitData->i2cbbTmrPlib->timerStop();
             pInitData->i2cbbTmrPlib->timerPeriodset(0);
            i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT;
          } else {
            SYS_PORT_PinInputEnable(pInitData->i2cbbSDAPin);
          }
        }
        break;

      case I2CBB_BUS_STATE_SCL_HIGH_RESTART:
        SYS_PORT_PinInputEnable(pInitData->i2cbbSCLPin);
        i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT;
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_HIGH_RESTART_CHECK;
        break;

      case I2CBB_BUS_STATE_SCL_HIGH_RESTART_CHECK:
        if (SYS_PORT_PinRead(pInitData->i2cbbSCLPin) == INPUT) {
          i2c_bbObj.i2cState = I2CBB_BUS_STATE_SDA_LOW_START;
        } else {
          if (!(i2c_bbObj.errorTimeOut--)) {
            i2c_bbObj.errorStatus = I2CBB_ERROR_BUS;
            pInitData->i2cbbTmrPlib->timerStop();
             pInitData->i2cbbTmrPlib->timerPeriodset(0);
            i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT;
          } else {
            SYS_PORT_PinInputEnable(pInitData->i2cbbSCLPin);
          }
        }
        break;

      case I2CBB_BUS_STATE_SCL_LOW_DATA_CHECK:
        SYS_PORT_PinOutputEnable(pInitData->i2cbbSCLPin);
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_LOW_DATA;
        break;

      case I2CBB_BUS_STATE_SCL_LOW_DATA:
        if (i2c_bbObj.I2CSWCounter > 1) {
          if ((bool)(i2c_bbObj.I2CSWData & 0x80)) {
            SYS_PORT_PinInputEnable(pInitData->i2cbbSDAPin);
          } else {
            SYS_PORT_PinOutputEnable(pInitData->i2cbbSDAPin);
          }
        }
        // just before the 9th clock prepare for an acknowledge.
        else if (i2c_bbObj.I2CSWCounter == 1) {
          i2c_bbObj._i2c_bit_written = 0;
          if ((i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_READ) &&
            ((i2c_bbObj.readCount != i2c_bbObj.readSize)
              &&
              (i2c_bbObj.I2CSWData == 0xFF00))) {
            SYS_PORT_PinOutputEnable(pInitData->i2cbbSDAPin);
          } else {
            SYS_PORT_PinInputEnable(pInitData->i2cbbSDAPin);
          }
        }
        // After the 9th clock
        if (i2c_bbObj.I2CSWCounter == 0) {
          if ((i2c_bbObj.writeCount != (i2c_bbObj.writeSize)) &&
            ((i2c_bbObj.I2CACKStatus == M_ACK))
            &&((i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_WRITE) ||
              (i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_WRITE_READ))) {
            i2c_bbObj.I2CSWData = * i2c_bbObj.writeBuffer++;
            i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_LOW_DATA;
            i2c_bbObj.I2CSWCounter = 9;
            i2c_bbObj.writeCount++;
          }
          else if ((i2c_bbObj.readCount != (i2c_bbObj.readSize)) &&
            (i2c_bbObj.I2CACKStatus == M_ACK) &&
            (i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_READ)) {
            i2c_bbObj.I2CSWData = 0xFF;
            i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_LOW_DATA;
            i2c_bbObj.I2CSWCounter = 9;
            i2c_bbObj.readCount++;
            if ((i2c_bbObj.readCount != (i2c_bbObj.readSize))) {
              i2c_bbObj.I2CNACKOut = 0x01;
            } else {
              i2c_bbObj.I2CNACKOut = 0;
            }
          }
          else if ((i2c_bbObj.writeCount == (i2c_bbObj.writeSize)) &&
            (i2c_bbObj.readCount != (i2c_bbObj.readSize)) &&
            (i2c_bbObj.I2CACKStatus == M_ACK) &&
            (i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_WRITE_READ)) {
            i2c_bbObj.transferState = I2CBB_TRANSFER_STATE_READ;
            i2c_bbObj.i2cState = I2CBB_BUS_STATE_SDA_HIGH_RESTART;
            i2c_bbObj.I2CSWCounter = 9;
          }
          else {
            i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_SDA_LOW_STOP;
          }
        } else {
          i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_HIGH_DATA; //if clock count is not 0
          i2c_bbObj.I2CSWCounter--; //decrement clock counter
          i2c_bbObj.I2CSWData <<= 1; //shift data byte
        }
        break;

      case I2CBB_BUS_STATE_SCL_HIGH_DATA:
        if ((i2c_bbObj._i2c_bit_written == 1)) {
          /* check the value of bit that is just written onto the bus */
          if ((bool)(SYS_PORT_PinRead(pInitData->i2cbbSDAPin)) == (bool) i2c_bbObj._i2c_bit_written) {
            SYS_PORT_PinInputEnable(pInitData->i2cbbSCLPin);
            i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_HIGH_DATA_CHECK;
          } else {
            i2c_bbObj.errorStatus = I2CBB_ERROR_BUS;
            pInitData->i2cbbTmrPlib->timerStop();
            pInitData->i2cbbTmrPlib->timerPeriodset(0);
            i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT;
            SYS_PORT_PinInputEnable(pInitData->i2cbbSCLPin);
          }
        } else {
          SYS_PORT_PinInputEnable(pInitData->i2cbbSCLPin);
          i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_HIGH_DATA_CHECK;
        }
        break;

      case I2CBB_BUS_STATE_SCL_HIGH_DATA_CHECK:
        if (SYS_PORT_PinRead(pInitData->i2cbbSCLPin) == INPUT) {
          if ((i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_READ) &&
            (i2c_bbObj.I2CSWCounter > 0) &&
            (i2c_bbObj.readSize > 0)) {
            i2c_bbObj.I2CReadData <<= 1;
            i2c_bbObj.I2CReadData |= SYS_PORT_PinRead(pInitData->i2cbbSDAPin);
          }
          if (i2c_bbObj.I2CSWCounter == 0) {
            i2c_bbObj.I2CACKStatus = SYS_PORT_PinRead(pInitData->i2cbbSDAPin);
            if ((i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_READ) &&
              (i2c_bbObj.I2CSWData == 0xFE00)) {
              * i2c_bbObj.readBuffer++ = i2c_bbObj.I2CReadData;
            }
          }
          i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_LOW_DATA_CHECK;
          i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT;
        } else {
          if (!(i2c_bbObj.errorTimeOut--)) {
            i2c_bbObj.errorStatus = I2CBB_ERROR_BUS;
            pInitData->i2cbbTmrPlib->timerStop(); //stop and clear Timer
            pInitData->i2cbbTmrPlib->timerPeriodset(0);
            i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT;
          }
        }
        break;

      case I2CBB_BUS_STATE_SCL_SDA_LOW_STOP:
        SYS_PORT_PinOutputEnable(pInitData->i2cbbSCLPin);
        SYS_PORT_PinOutputEnable(pInitData->i2cbbSDAPin);
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_SDA_LOW_STOP_CHECK;
        break;
      case I2CBB_BUS_STATE_SCL_SDA_LOW_STOP_CHECK:
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_HIGH_STOP;
        break;
      case I2CBB_BUS_STATE_SCL_HIGH_STOP:
        SYS_PORT_PinInputEnable(pInitData->i2cbbSCLPin);
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SCL_HIGH_STOP_CHECK;
        i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT;
        break;

      case I2CBB_BUS_STATE_SCL_HIGH_STOP_CHECK:
        if (SYS_PORT_PinRead(pInitData->i2cbbSCLPin) == INPUT) {
          i2c_bbObj.i2cState = I2CBB_BUS_STATE_SDA_HIGH_STOP;
        } else {
          if (!(i2c_bbObj.errorTimeOut--)) //decrement error counter
          {
            i2c_bbObj.errorStatus = I2CBB_ERROR_BUS;
            pInitData->i2cbbTmrPlib->timerStop(); //stop and clear Timer
             pInitData->i2cbbTmrPlib->timerPeriodset(0);
            i2c_bbObj.errorTimeOut = ERROR_TIMEOUT_COUNT; //reset error counter
          } else {
            SYS_PORT_PinInputEnable(pInitData->i2cbbSCLPin);
          }
        }
        break;
      case I2CBB_BUS_STATE_SDA_HIGH_STOP:
        SYS_PORT_PinInputEnable(pInitData->i2cbbSDAPin);
        i2c_bbObj.i2cState = I2CBB_BUS_STATE_SDA_HIGH_STOP_CHECK;
        break;
      case I2CBB_BUS_STATE_SDA_HIGH_STOP_CHECK:
        if ((i2c_bbObj.writeCount == i2c_bbObj.writeSize) && (i2c_bbObj.readCount == i2c_bbObj.readSize)) {
          i2c_bbObj.transferState = I2CBB_TRANSFER_STATE_DONE;
        }else if(i2c_bbObj.I2CACKStatus != M_ACK)
        {
            i2c_bbObj.errorStatus = I2CBB_ERROR_NAK;
            pInitData->i2cbbTmrPlib->timerStop(); //stop and clear Timer
            pInitData->i2cbbTmrPlib->timerPeriodset(0);
            i2c_bbObj.i2cState = I2CBB_BUS_STATE_NULL_STATE;
            if (i2c_bbObj.callback != NULL) {
              i2c_bbObj.callback(i2c_bbObj.context);
            }
        }



        if ((i2c_bbObj.transferState == I2CBB_TRANSFER_STATE_DONE) || (i2c_bbObj.errorStatus == I2CBB_ERROR_BUS)) {
          pInitData->i2cbbTmrPlib->timerStop(); //stop and clear Timer
          pInitData->i2cbbTmrPlib->timerPeriodset(0);
          i2c_bbObj.i2cState = I2CBB_BUS_STATE_NULL_STATE;

           if (i2c_bbObj.callback != NULL) {
          i2c_bbObj.callback(i2c_bbObj.context);
        }
        }
        break;

      default:
        break;
    }

}

static void I2C_BB_eventHandler(uint32_t status, uintptr_t context)
{
    I2C_BB_tasks();
}

void I2C_BB_Initialize(void)
{
    I2C_BB_INIT* pInitData =(I2C_BB_INIT*)&i2cBBInitData;

    /* Save the timer clock frequency at the time of configuration */
    i2c_bbObj.timerSrcClkFreq = I2C_BB_TMR_CLOCK_FREQUENCY;

    /* Save the initial configured I2C clock speed */
    i2c_bbObj.i2cClockSpeed = pInitData->i2cClockSpeed;

    pInitData->i2cbbTmrPlib->timerCallbackRegister(I2C_BB_eventHandler,(uintptr_t)0);
}

bool I2C_BB_IsBusy(void)
{
    if( i2c_bbObj.i2cState != I2CBB_BUS_STATE_NULL_STATE )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool I2C_BB_Read(uint16_t address, uint8_t *pdata, size_t length)
{
    // Check for ongoing transfer
    if( i2c_bbObj.i2cState != I2CBB_BUS_STATE_NULL_STATE )
    {
        return false;
    }

    i2c_bbObj.address=address << 1;
    i2c_bbObj.readBuffer=pdata;
    i2c_bbObj.readSize=length;
    i2c_bbObj.writeBuffer=NULL;
    i2c_bbObj.writeSize=0;
    i2c_bbObj.transferState= I2CBB_TRANSFER_STATE_READ;
    i2c_bbObj.errorStatus = I2CBB_ERROR_NONE;

    I2C_BB_InitiateTransfer();
    i2c_bbObj.I2CSWCounter = 9;

    return true;
}

bool I2C_BB_Write(uint16_t address, uint8_t *pdata, size_t length)
{
    // Check for ongoing transfer
    if( i2c_bbObj.i2cState != I2CBB_BUS_STATE_NULL_STATE )
    {
        return false;
    }

    i2c_bbObj.address=address << 1;
    i2c_bbObj.readBuffer=NULL;
    i2c_bbObj.readSize=0;
    i2c_bbObj.writeBuffer=pdata;
    i2c_bbObj.writeSize=length;
    i2c_bbObj.transferState= I2CBB_TRANSFER_STATE_WRITE;
    i2c_bbObj.errorStatus = I2CBB_ERROR_NONE;

    I2C_BB_InitiateTransfer();
    i2c_bbObj.I2CSWCounter = 9;
    return true;
}

bool I2C_BB_WriteRead(uint16_t address, uint8_t *wdata, size_t wlength, uint8_t *rdata, size_t rlength)
{

    // Check for ongoing transfer
    if( i2c_bbObj.i2cState != I2CBB_BUS_STATE_NULL_STATE )
    {
        return false;
    }

    i2c_bbObj.address=address << 1;
    i2c_bbObj.readBuffer=rdata;
    i2c_bbObj.readSize=rlength;
    i2c_bbObj.writeBuffer=wdata;
    i2c_bbObj.writeSize=wlength;
    i2c_bbObj.transferState= I2CBB_TRANSFER_STATE_WRITE_READ;
    i2c_bbObj.errorStatus = I2CBB_ERROR_NONE;

    I2C_BB_InitiateTransfer();
    i2c_bbObj.I2CSWCounter = 9;

    return true;
}

void I2C_BB_CallbackRegister(I2CBB_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    i2c_bbObj.callback = callback;
    i2c_bbObj.context = contextHandle;
}



I2CBB_ERROR I2C_BB_ErrorGet(void)
{
    I2CBB_ERROR error;

    error = i2c_bbObj.errorStatus;
    i2c_bbObj.errorStatus = I2CBB_ERROR_NONE;

    return error;
}

bool I2C_BB_TransferSetup(I2CBB_TRANSFER_SETUP* setup, uint32_t tmrSrcClkFreq )
{
    if (setup == NULL)
    {
        return false;
    }

    /* Save the tmrSrcClkFreq if it is non-zero */
    if( tmrSrcClkFreq != 0)
    {
        i2c_bbObj.timerSrcClkFreq = tmrSrcClkFreq;
    }

    /* Save the new I2C clock speed value */
    i2c_bbObj.i2cClockSpeed = setup->clkSpeed;

    /* Note: The timer period will be changed based on the new I2C clock speed, when an I2C request is submitted */

    return true;
}
