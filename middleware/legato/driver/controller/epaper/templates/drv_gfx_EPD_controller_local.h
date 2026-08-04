/** 
 * @file drv_gfx_EPD_controller_local.h
 * @brief EPD Controller utility functions and definitions.
 *
 * @details This header file contains the function prototypes 
            and definitions of the data types and constants 
            that provide utility service to the driver(s).
 */

/*******************************************************************************
* Copyright (C) 2026 Microchip Technology Inc. and its subsidiaries.
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

#ifndef DRV_GFX_EPD_CONTROLLER_LOCAL_H
#define DRV_GFX_EPD_CONTROLLER_LOCAL_H

#include "definitions.h"
#include "gfx/driver/gfx_driver.h"

#ifdef __cplusplus
    extern "C" {
#endif

/** Non-blocking delay using the system tick timer.
 * @param ms Delay in milliseconds
 */
extern void DRV_EPD_DelayMS(int ms);

/** EPD interface reset
 * @details Assert and deassert the reset signal using
 *          the standard pulse width. */
extern void DRV_EPD_Reset(void);

/** Non-blocking wait while the EPD interface is busy then wait a moment
 * @param ms Delay in milliseconds imposed after reset is complete
 */
extern void DRV_EPD_waitBusy(int ms);

/** Transfer delegate to stream data to the EPD data interface
 * @param intf display interface definition
 * @param bytes pointer to uint8_t data stream
 * @param count number of bytes to send
 * @note Transfer delegates are used for function injection */
extern int DRV_writeData(GFX_Disp_Intf intf, uint8_t * bytes, int count);

/** Transfer delegate to repeatedly send one byte to the EPD data interface
 * @param intf display interface definition
 * @param bytes pointer to (the single) uint8_t data byte
 * @param count number of times to send that byte
 * @note Transfer delegates are used for function injection */
extern int DRV_writeRepeatedData(GFX_Disp_Intf intf, uint8_t * byte, int count);

/** Transfer delegate to stream data to the EPD data interface while rotating the data value
 * @param intf display interface definition
 * @param bytes pointer to uint8_t data stream
 * @param count number of bytes to send
 * @note Transfer delegates are used for function injection */
extern int DRV_test_writeRotate(GFX_Disp_Intf intf, uint8_t * byte, int count);

/** Transfer delegate type */
typedef int DRV_transferFunction_t(GFX_Disp_Intf, uint8_t *, int);

/** Enumeration of EPD driver states. */
typedef enum
{
    INIT = 0,
    RUN,
    ERROR,
} DRV_STATE;

/** State and control parameters for driver operation. */
typedef struct EPD_driver_state_s
{
    /** Driver state */
    DRV_STATE state;

    /** Port-specific private data */
    void *port_priv;
    
    /** Preset when the next load is for non-partial image. */
    uint8_t loadFullImage;
    
    /** Selector for available fast-refresh LUTs */
    uint8_t refreshFastControlTable;
    
    /** Number of times to draw an update image; zero and one are the same */
    uint updateOverdraw:1;
} EPD_driver_state_t;

extern EPD_driver_state_t drv;

/* Standard hardware functions *************************** */

/* TODO: route this through the SPI interface layer. */
#ifdef GFX_DISP_INTF_PIN_RESET_Clear

/** Assert RESET (low) using the SPI interface layer. */
#define DRV_EPD_Reset_Assert()      GFX_DISP_INTF_PIN_RESET_Clear()

/** Deassert RESET (high) using the SPI interface layer. */
#define DRV_EPD_Reset_Deassert()    GFX_DISP_INTF_PIN_RESET_Set()

/* EPD data transfer standard is that MOSI and MISO are the same wire,
 * but PIC32 SERCOM does not implicitly support half-duplex operation.
 * Consult the PIC32CM_magic folder for a work-around. */
#define READ_STOP           0
#define READ_REVISION       0
#define READ_STATUS         0
#define nBUSY_Get()     (GFX_DISP_INTF_PIN_nBUSY_Get()? 0:1)
#else
#error "ERROR: GFX_DISP_INTF_PIN_RESET not defined. Please define in Pin Manager."
#define DRV_EPD_Reset_Assert()
#define DRV_EPD_Reset_Deassert()
#endif

/** Assert CS (low) using the SPI interface layer. */
#define DRV_EPD_NCSAssert(intf)   \
  GFX_Disp_Intf_PinControl(intf,GFX_DISP_INTF_PIN_CS, GFX_DISP_INTF_PIN_CLEAR)

/** Deassert CS (high) using the SPI interface layer. */
#define DRV_EPD_NCSDeassert(intf) \
  GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_CS, GFX_DISP_INTF_PIN_SET)

/** Set DC for Command (low) using the SPI interface layer. */
#define DRV_EPD_DCasCommand(intf) \
    GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_RSDC, GFX_DISP_INTF_PIN_CLEAR)

/** Set DC for Data (high) using the SPI interface layer. */
#define DRV_EPD_DCDasData(intf) \
  GFX_Disp_Intf_PinControl(intf, GFX_DISP_INTF_PIN_RSDC, GFX_DISP_INTF_PIN_SET)

#ifdef __cplusplus
    }
#endif
    
#endif // DRV_GFX_EPD_CONTROLLER_LOCAL_H
