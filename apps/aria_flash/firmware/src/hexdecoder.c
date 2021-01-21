#include "hexdecoder.h"

#include <string.h>

#define MAX_FLASH_ATTEMPT  5

static void asciiToHex(uint8_t* asciiVal, uint8_t* hexVal)
{
	uint8_t lch, rch;

	// determine if the ASCII values are in the alpha numeric range
	if(asciiVal[0] < 0x3A)
	{
		// numerical
		lch = asciiVal[0] & 0x0F;
	}
	else
	{
		// alphabetical
		lch = 0x09 + (asciiVal[0] & 0x0F);
	}
	
	if(asciiVal[1] < 0x3A)
	{
		// numerical
		rch = asciiVal[1] & 0x0F;
	}
	else
	{
		// alphabetical
		rch = 0x09 + (asciiVal[1] & 0x0F);
	}

	// convert 2 bytes of ASCII to 1 byte of hex.
	*hexVal = ((lch << 4) & 0xF0) | (rch);
}

int32_t HexDecoder_Initialize(HexDecoder* dec,
                              uint32_t recordCount,
                              uint8_t* writeBuffer,
                              HexDecoder_RecordRead_FnPtr readPtr,
                              HexDecoder_DataWrite_FnPtr writePtr)
{
    if(dec == NULL || readPtr == NULL || writePtr == NULL || 
       writeBuffer == NULL || recordCount == 0)
        return -1;
        
    dec->readPtr = readPtr;
    dec->writePtr = writePtr;
    
    dec->recordCount = recordCount;
    dec->currentRecord = 0;
    
    dec->writeBuffer = writeBuffer;
    dec->segmentAddress = 0;
    dec->linearAddress = 0;
    
    return 0;
}

int32_t HexDecoder_Decode(HexDecoder* dec)
{
    uint8_t hexVals[HEXDECODER_MAX_RECORD_SIZE / 2];
    uint32_t len, i;
    uint8_t* ptr;
    
    uint8_t count;
    uint16_t address;
    uint8_t type;
    
    uint8_t checksumCalc;
    
    if(dec->currentRecord >= dec->recordCount)
        return 0;
    
    memset(dec->recordBuffer, 0, HEXDECODER_MAX_RECORD_SIZE);
    memset(hexVals, 0, HEXDECODER_MAX_RECORD_SIZE / 2);

    if(dec->readPtr(dec, dec->currentRecord, dec->recordBuffer) != 0)
        return -1;

    ptr = dec->recordBuffer;

    // skip colon
    if(ptr[0] != ':')
        return -1;

    ptr++;

    // calculate size
    asciiToHex(ptr, &hexVals[0]);
    ptr += 2;
    
    len = hexVals[0] + 5;

    for(i = 1; i < len; i++)
    {
        asciiToHex(ptr, &hexVals[i]);
        ptr += 2;
    }

    // read hard values
    count = hexVals[0];
    address = hexVals[1] << 8;
    address |= hexVals[2];
    type = hexVals[3];
    

    checksumCalc = 0;

    // calculate checksum
    for(i = 0; i < len; i++)
    {
        checksumCalc += hexVals[i];
        
        hexVals[i + 100] = checksumCalc;
    }
    
    // compare checksums
    if(checksumCalc != 0)
        return -1;

    // set ptr to data values
    ptr = &hexVals[4];

    // process record
    switch(type)
    {
        case 0x00: // data record
        {
            if(count >= HEXDECODER_MAX_RECORD_SIZE)
                return -1;

            memcpy(dec->writeBuffer, ptr, count);

            // if result != 0, attempt to flash again
            if(dec->writePtr(dec,
                            (address +
                               (dec->segmentAddress) +
                               (dec->linearAddress)),
                             dec->writeBuffer,
                             count) == -1)
            {
                if(dec->flashAttempt == MAX_FLASH_ATTEMPT)
                    return -1;
                
                dec->flashAttempt++;
                
                return 0;
            }
            
            dec->flashAttempt = 0;
            
            break;
        }
        case 0x01: // end of file record
        {
            break;
        }
        case 0x02: // extended segment address record
        {
            dec->segmentAddress = (uint32_t)hexVals[5];
            dec->segmentAddress |= (hexVals[4] << 8);

            dec->linearAddress = 0;

            break;
        }
        case 0x03: // start segment address record
        {
            // not used
            break;
        }
        case 0x04: // extended linear address record
        {
            dec->linearAddress = (uint32_t)hexVals[5];
            dec->linearAddress |= (hexVals[4] << 8);
            dec->linearAddress <<= 16;

            dec->segmentAddress = 0;

            break;
        }
        case 0x05: // start linear address record
        {
            // not used
            break;
        }
    }
    
    return ++dec->currentRecord;
}