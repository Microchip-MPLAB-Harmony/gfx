/*******************************************************************************
  I/O reconfiguration for PIC32 SERCOM.SPI peripheral

  Company:
    Microchip Technology Inc.

  File Name:
    PIC32CM_SPI_1wire.h

  Summary:
    Source code for allow half-duplex (1-wire) SPI data transfers.

  Description:
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#include "../../../../../device.h"

/* Touch this for our specific project */
#define SERCOM_REGS    SERCOM3_REGS

/* Adapt for explicit master-mode registers. */
#ifdef SERCOM_SPIM_CTRLA_ENABLE_Msk
#define sercom_spi_registers_t      sercom_spim_registers_t
#define PIC32CM_SPI_DOPO_drive      SERCOM_SPIM_CTRLA_DOPO_PAD0_Val
#define PIC32CM_SPI_DOPO_tristate   SERCOM_SPIM_CTRLA_DOPO_PAD3_Val
#define PIC32CM_SPI_DIPO_PAD0       SERCOM_SPIM_CTRLA_DIPO_PAD0_Val
#else
#define PIC32CM_SPI_DOPO_drive      SERCOM_SPI_CTRLA_DOPO_PAD0_Val
#define PIC32CM_SPI_DOPO_tristate   SERCOM_SPI_CTRLA_DOPO_PAD2_Val
#endif

/** Reconfigure the SPI DO and DI ports for 1-wire operation. 
 * @param spi SPI register object pointer
 * @param SERCOM_SPI_CTRLA_DOPO  Mux DOPO control value.
 * @param SERCOM_SPI_CTRLA_DIPO  Mux DIPO control value.
 * @details The mux control should be one of 
 *    * SERCOM_SPI_CTRLA_DOPO_MUX0_Val = DO on PAD0 drive PAD0
 *    * SERCOM_SPI_CTRLA_DOPO_MUX2_Val = DO on PAD2 tristate PAD0
 * 
 *  * where PAD2 is not connected to an I/O pin.
 *  * It is assumed that DI is already mapped to PAD0.
 *  */
extern void PIC32CM_set_SPI_DxPO(sercom_spi_registers_t * spi, 
                   uint8_t SERCOM_SPI_CTRLA_DOPO, uint8_t SERCOM_SPI_CTRLA_DIPO);

/** Load the baud rate divider with the given value.
 * @param spi SPI register object pointer
 * @param baud new value for the baud rate divider register 
 * @note TODO: map SPI and SPIM syntax so this works in other PIC32 devices. */
extern void PIC32CM_SPI_baud(sercom_spi_registers_t *spi, uint8_t baud);
