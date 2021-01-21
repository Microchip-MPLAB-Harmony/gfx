/*******************************************************************************
  System Definitions

  File Name:
    hexdecoder.h

  Summary:
    Intel HEX file decoder

  Description:
    This file contains a decoder for Intel HEX files.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef HEX_DECODER_H   
#define HEX_DECODER_H

#include <stdint.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define HEXDECODER_MAX_RECORD_SIZE    4096
    
typedef struct HexDecoder_t HexDecoder;
    
typedef int32_t(*HexDecoder_RecordRead_FnPtr)(HexDecoder* dec,
                                              uint32_t recordNum,
                                              uint8_t record[HEXDECODER_MAX_RECORD_SIZE]);

typedef int32_t(*HexDecoder_DataWrite_FnPtr)(HexDecoder* dec,
                                             uint32_t address,
                                             uint8_t* buffer,
                                             uint32_t size);

typedef struct HexDecoder_t
{
    uint8_t recordBuffer[HEXDECODER_MAX_RECORD_SIZE];
    
    uint8_t* writeBuffer;
    
    uint32_t recordCount;
    uint32_t currentRecord;
    
    uint32_t segmentAddress;
    uint32_t linearAddress;
    
    uint32_t flashAttempt;
    
    HexDecoder_RecordRead_FnPtr readPtr;
    HexDecoder_DataWrite_FnPtr writePtr;
} HexDecoder;

int32_t HexDecoder_Initialize(HexDecoder* dec,
                             uint32_t recordCount,
                             uint8_t writeBuffer[HEXDECODER_MAX_RECORD_SIZE],
                             HexDecoder_RecordRead_FnPtr readPtr,
                             HexDecoder_DataWrite_FnPtr writePtr);
                           
int32_t HexDecoder_Decode(HexDecoder* dec);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* HEX_DECODER_H */

/* *****************************************************************************
 End of File
 */
