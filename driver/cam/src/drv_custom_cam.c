/*******************************************************************************
  Camera Sensor Driver Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_custom_cam.c

  Summary:
    Source code for the custom camera driver to be used for ISC/ISI applications.

  Description:
    This file contains the source code for setup of the  custom camera sensor module. 
    It services the drv_cam.c driver.
    For a different camera sensor, this driver should be implemented using this 
    as an example.
  
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "driver/cam/drv_cam.h"
#include "driver/i2c/drv_i2c.h"
#include "peripheral/pio/plib_pio.h"
#include "system/time/sys_time.h"
#include "driver/cam/drv_custom_cam_regs.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
/* I2C device address */
//Define the Camera sensor slave address
#define CUSTOM_CAM_I2C_ADDR  0x01

/* CHIP ID */
/* NOTE: Define the Camera sensor CHIP ID */
#define CUSTOM_CAM_CHIP_ID  0x01

/* I2C bus driver handle */
static DRV_HANDLE drvI2CHandle;

/* I2C Buffer handle */
static DRV_I2C_TRANSFER_HANDLE _transferHandle;

static volatile bool _transferLock;
static volatile bool _transferSuccess;

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
static inline void custom_sleep(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    
    while (SYS_TIME_DelayIsComplete(timer) == false);
}



static void _i2cEventHandler(DRV_I2C_TRANSFER_EVENT evt,
                             DRV_I2C_TRANSFER_HANDLE hdl,
                             uintptr_t data)
{
    _transferSuccess = (evt == DRV_I2C_TRANSFER_EVENT_COMPLETE);
    
    _transferLock = false;
}

static bool _read(uint16_t addr,
                  uint8_t* readBuf,
                  uint32_t sz)
{
    uint8_t addrBuf[2];
    
    addrBuf[0] = (addr & 0x7F00) >> 8;
    addrBuf[1] = addr & 0xFF;
    
    _transferHandle = DRV_I2C_TRANSFER_HANDLE_INVALID;
    _transferLock = true;
    
    DRV_I2C_TransferEventHandlerSet(drvI2CHandle,
                                    _i2cEventHandler,
                                    0);
    
    DRV_I2C_WriteReadTransferAdd(drvI2CHandle, 
                              CUSTOM_CAM_I2C_ADDR,
                              addrBuf,
                              2,
                              readBuf,
                              sz,
                              &_transferHandle);
    
    if(_transferHandle == DRV_I2C_TRANSFER_HANDLE_INVALID)
        return false;
    
    while(_transferLock == true);
    
    return _transferSuccess;
}

static bool _write(uint16_t addr,
                   uint8_t val)
{
    uint8_t buf[3];
    buf[0] = (addr & 0x7F00) >> 8;
    buf[1] = (addr & 0xFF);
    buf[2] = val;
    uint32_t timeout = 0;
    
    _transferHandle = DRV_I2C_TRANSFER_HANDLE_INVALID;
    _transferLock = true;
    
    DRV_I2C_TransferEventHandlerSet(drvI2CHandle,
                                    _i2cEventHandler,
                                    0);
    
    DRV_I2C_WriteTransferAdd(drvI2CHandle,
                             CUSTOM_I2C_ADDR,
                             buf,
                             3,
                             &_transferHandle);
    
    if(_transferHandle == DRV_I2C_TRANSFER_HANDLE_INVALID)
        return false;
    
    while(_transferLock == true)
    {
        timeout += 1;
        
        if(timeout == 100000)
        {
            break;
        }
    }
    
    return _transferSuccess;
}

static bool _writeTable(const RegVal* vals)
{
    uint32_t i = 0;
    
    while(!((vals[i].address == 0xFF)&&(vals[i].value == 0xFF)))
    {
        if(_write(vals[i].address,
                  vals[i].value) == false)
            return false;
        
        i += 1;
        
    }
    
    return true;
}

static void drv_custom_cam_setup(void)
{
    _writeTable(initVals);
}

static bool drv_custom_cam_verifyChipID(void)
{
    uint8_t buf[2];
    uint16_t id;

    buf[0] = 0;
    buf[1] = 0;
    
    _read(CUSTOM_CAM_CHIP_ID_HIGH_BYTE,
          buf,
          2);
    
    id = buf[1];
    id |= buf[0] << 8;
    
    if(id != CUSTOM_CAM_CHIP_ID)
        return false;
    
    return true;
}

static void drv_custom_cam_bsp_init(void)
{
    /* NOTE: Perform any sensor specific initialization */
    custom_sleep(10);
    drvI2CHandle = DRV_I2C_Open(0, DRV_IO_INTENT_READWRITE);
}

// *****************************************************************************
// *****************************************************************************
// Section: Custom Camera Driver functions called by drv_cam driver
// *****************************************************************************
// *****************************************************************************
/******************************************************************************
  Function:
    drv_imgsensor_setup ( void )

  Summary:
    The drv_cam driver calls this function to set up the camera being used.
    This driver implements the camera specific setup function.

 */
void drv_imgsensor_setup(void)
{
    drv_custom_cam_setup();
}

/******************************************************************************
  Function:
    drv_imgsensor_verifyChipID ( void )

  Summary:
    The drv_cam driver calls this function to detect the supported camera by
    verifying the chip ID.
    This driver implements the camera specific function.

 */
bool drv_imgsensor_verifyChipID(void)
{
    return drv_custom_cam_verifyChipID();
}

/******************************************************************************
  Function:
    drv_imgsensor_bsp_init ( void )

  Summary:
    The drv_cam driver calls this function to perform any camera specific init.
    This driver implements the camera specific init function.

 */
void drv_imgsensor_bsp_init(void)
{
    drv_custom_cam_bsp_init();
}