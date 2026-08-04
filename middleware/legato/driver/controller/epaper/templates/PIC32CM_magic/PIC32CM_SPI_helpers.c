/*******************************************************************************
  I/O reconfiguration for PIC32 SERCOM.SPI peripheral

  Company:
    Microchip Technology Inc.

  File Name:
    PIC32CM_SPI_1wire.c

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

#include "PIC32CM_SPI_helpers.h"
#include "../../../../../device.h"

/* Adapt for explicit master-mode registers. */
#ifdef SERCOM_SPIM_CTRLA_ENABLE_Msk
#define SERCOM_SPI_CTRLA_ENABLE_Msk SERCOM_SPIM_CTRLA_ENABLE_Msk
#define SERCOM_SPI_CTRLA_DOPO_Msk   SERCOM_SPIM_CTRLA_DOPO_Msk
#define SERCOM_SPI_CTRLA_DOPO_Pos   SERCOM_SPIM_CTRLA_DOPO_Pos
#define SERCOM_SPI_CTRLA_DIPO_Msk   SERCOM_SPIM_CTRLA_DIPO_Msk
#define SERCOM_SPI_CTRLA_DIPO_Pos   SERCOM_SPIM_CTRLA_DIPO_Pos
#endif

void PIC32CM_set_SPI_DxPO(sercom_spi_registers_t * spi, 
            uint8_t SERCOM_SPI_CTRLA_DOPO, uint8_t SERCOM_SPI_CTRLA_DIPO)
{
    /* The CTRLA register is enable-guarded */
    spi->SERCOM_CTRLA &= ~SERCOM_SPI_CTRLA_ENABLE_Msk;
    while(spi->SERCOM_SYNCBUSY & SERCOM_SPI_CTRLA_ENABLE_Msk){}

    /* Reconfigure the data-out control to move DO to a different PAD.
     * This assumes that DI is mapped to PAD0 already. */
    register uint32_t ctrla = spi->SERCOM_CTRLA;
    ctrla &= ~(SERCOM_SPI_CTRLA_DOPO_Msk|SERCOM_SPI_CTRLA_DIPO_Msk);
    ctrla |= (SERCOM_SPI_CTRLA_DOPO << SERCOM_SPI_CTRLA_DOPO_Pos);
    ctrla |= (SERCOM_SPI_CTRLA_DIPO << SERCOM_SPI_CTRLA_DIPO_Pos);
    spi->SERCOM_CTRLA = ctrla;

    /* Restore SPI operation */
    spi->SERCOM_CTRLA |= SERCOM_SPI_CTRLA_ENABLE_Msk;
    while(spi->SERCOM_SYNCBUSY & SERCOM_SPI_CTRLA_ENABLE_Msk){}
}


void PIC32CM_SPI_baud(sercom_spi_registers_t *spi, uint8_t baud)
{
    spi->SERCOM_CTRLA &= ~SERCOM_SPI_CTRLA_ENABLE_Msk;
    while(SERCOM_SPI_CTRLA_ENABLE_Msk & spi->SERCOM_SYNCBUSY){}
    
    volatile uint8_t brr = spi->SERCOM_BAUD;
    spi->SERCOM_BAUD = (uint8_t)SERCOM_SPIM_BAUD_BAUD(baud);
    brr = spi->SERCOM_BAUD;
    if (brr != baud) while(1);
    
    spi->SERCOM_CTRLA |= SERCOM_SPI_CTRLA_ENABLE_Msk;
    while(SERCOM_SPI_CTRLA_ENABLE_Msk & spi->SERCOM_SYNCBUSY){}
}

