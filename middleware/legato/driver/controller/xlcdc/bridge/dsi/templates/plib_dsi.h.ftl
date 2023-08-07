/*******************************************************************************
  DSI Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_dsi.h

  Summary:
    Contains DSI peripheral library function declarations.

  Description:
    This library implements a SUBSET of the register configurations
    for the DSI peripheral.

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

#ifndef _PLIB_DSI_H
#define _PLIB_DSI_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
    extern "C" {
#endif

/*******************************************************************************
  Union:
    DSI_GENERIC_HEADER

  Summary:
    DSI DCS/Generic Interface Header.

  Description:
    For Short packets, Data 0 and Data 1 bytes are sent in the header itself.
    For Long packets, WC needs to be specified in bytes.
    ECC is handled by hardware.

  Remarks:
    None.
*/
typedef union {
    uint32_t headerU32;
    struct {
        uint32_t dataType : 6; // DT
        uint32_t virtualChannelID : 2; // VC
        uint8_t data[2]; // Data 0, Data 1
    } shortPacket;
    struct {
        uint32_t dataType : 6; // DT
        uint32_t virtualChannelID : 2; // VC
        uint32_t size : 16; // WC
    } longPacket;
} DSI_GENERIC_HEADER;

/*******************************************************************************
  Union:
    DSI_GENERIC_PAYLOAD

  Summary:
    DSI DCS/Generic Interface Payload for long packets.

  Description:
    The payload needs to be written 4 bytes at a time.
    This union provides different data alignments to do the above.

  Remarks:
    None.
*/
typedef union {
    uint32_t dataU32;
    uint16_t dataU16[2];
    uint8_t  data[4];
} DSI_GENERIC_PAYLOAD;

/*******************************************************************************
  Function:
    void DSI_Initialize(void)

  Summary:
    Initializes the on-chip DSI peripheral.

  Remarks:
    None.
*/
void DSI_Initialize(void);

/*******************************************************************************
  Function:
    void DSI_CommandMode(void)

  Summary:
    Switch to DSI Command Mode.

  Remarks:
    None.
*/
void DSI_CommandMode(void);

/*******************************************************************************
  Function:
    void DSI_VideoMode(void)

  Summary:
    Switch to DSI Video Mode.

  Remarks:
    None.
*/
void DSI_VideoMode(void);

/*******************************************************************************
  Function:
    bool DSI_Write(DSI_GENERIC_HEADER * hdr, DSI_GENERIC_PAYLOAD * pld)

   Summary:
    Can be used to transmit data to an external DSI peripheral.

   Description:
    None.

   Precondition:
    Max payload size is limited by the hardware FIFO size.

   Parameters:
    hdr - The header structure of type DSI_GENERIC_HEADER.
    pld - The payload structure (or array of structures)
          of type DSI_GENERIC_PAYLOAD.

   Returns:
    0, on success. 1, on failure.

   Remarks:
    None.
*/
bool DSI_Write(DSI_GENERIC_HEADER * hdr, DSI_GENERIC_PAYLOAD * pld);

#ifdef __cplusplus
    }
#endif

#endif
