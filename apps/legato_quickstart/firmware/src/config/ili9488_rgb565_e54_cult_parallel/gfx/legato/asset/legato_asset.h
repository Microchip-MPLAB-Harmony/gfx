// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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

/*******************************************************************************
 Module for Microchip Graphics Library - Legato Graphics Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_asset.h

  Summary:
    Global defines for legato assets.

  Description:
    Global defines for legato assets.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LE_ASSET_H
#define LE_ASSET_H

#include "gfx/legato/common/legato_common.h"



#define LE_ASSET_LOCATION_ID_INTERNAL 0

typedef struct leAssetHeader leAssetHeader;

// *****************************************************************************
/* enum:
    leAssetType

  Summary:
    Enumerates known asset types.
*/ 
typedef enum leAssetType
{
    LE_ASSET_TYPE_IMAGE = 0,
    LE_ASSET_TYPE_PALETTE,
    LE_ASSET_TYPE_FONT,
    LE_ASSET_TYPE_BINARY,
    LE_ASSET_TYPE_STRINGTABLE
} leAssetType;

// *****************************************************************************
/* Structure:
    leAssetHeader

  Summary:
    Defines a common header for all assets supported by the generic decoder 
    interface.
    
  Description:
    type - leAssetType - indicates the type of the asset
    dataLocation - indicates the location of the asset data.  0 is always
                   internal flash.  any other number must be understood
                   by the application
    dataAddress - the address at which the data resides.  may be a local pointer
                  or a location in some external storage location not in the
                  local memory map.
    dataSize - the size of the asset data in bytes
*/
typedef struct leAssetHeader
{
    uint32_t type;
    uint32_t dataLocation;
    void* dataAddress;
    uint32_t dataSize;
} leAssetHeader; 

#if LE_ASSET_STREAMING_ENABLED == 1

// *****************************************************************************
/* enum:
    leExternalAssetReaderStatus

  Summary:
    Enumerates external reader state machine states.
    
    Invalid - state machine hasn't been initialized
    Read - state machine is ready to begin processing but hasn't been run yet
    Waiting - state machine is waiting for a memory operation to complete
    Drawing - state machine is waiting for a drawing operation to complete
    Finished - state machine has finished drawing the entire asset
    Aborted - state machine encountered an error and has aborted
*/ 
typedef enum leAssetStreamReaderStatus
{
    LE_READER_STATUS_INVALID = 0,
    LE_READER_STATUS_READY,
    LE_READER_STATUS_WAITING,
    LE_READER_STATUS_DRAWING,
    LE_READER_STATUS_FINISHED,
    LE_READER_STATUS_ABORTED,
    LE_READER_STATUS_FAILED
} leAssetStreamReaderStatus;


// *****************************************************************************
/* Structure:
    struct leAssetStreamReader

  Summary:
    Defines the base description of an streaming asset reader.  Specific decoder
    implementations will build on this foundation for each decoder type.

  Description:
    asset - the asset being streamed
    status - the overall status of the decoder state machine
    state -  defines the current state of the decoder, internal use only
    exec -   the exec function that must be called periodically to allow the state
             machine to process to completion.
    dataReady - signals that the requested data is ready.  if NULL then the stream
                is a blocking-type stream and cannot preempt itself

    onFinished - a callback that can be set to provide notification as to when
                 the reader reaches a final state

    userData - any kind of user or application data attached to this reader

*/
typedef struct leAssetStreamReader
{
    leAssetHeader* asset;

    leAssetStreamReaderStatus status;

    uint32_t state;
    int32_t result;

    leResult (*exec)(struct leAssetStreamReader* rdr);
    leResult (*abort)(struct leAssetStreamReader* rdr);
    leResult (*dataReady)(struct leAssetStreamReader* rdr);

    void (*onFinished)(struct leAssetStreamReader* rdr);

    void* userData;
} leAssetStreamReader;

// *****************************************************************************
/* typedef:
    leApplication_MediaOpenRequest

  Summary:
    A callback that indicates that a media decoder wishes to read from an
    external media source and that the application should prepare that source.
    
    If the result is false then the decoder will abort.

    reader - the reader that is requesting to stream data
*/
leResult leApplication_MediaOpenRequest(leAssetStreamReader* reader);

// *****************************************************************************
/* typedef:
    leMemoryReadRequest_FnPtr

  Summary:
    A memory read request function is a request that is issued by anything that
    needs external support to access data that is stored in some external
    location.  For instance, an image may be stored in an SPI memory chip.
    A JPEG decoder has no knowledge of what SPI is.  This read request is issued
    to the application so that the application can then assume responsibility for
    communication with the peripherial to retrieve the data.  If the reader pointer
    or the callback pointer are null then the read request must be serviced 
    immediately.  In other words, the requesting decoder does not support
    non-blocking streaming.
    
    From the reader the handler can determine which asset is being decoded and
    which media it resides in.

    reader - the reader requesting the media data
    address - the requested source address
    size - the requested data size
    buf - the destination data address
*/                                                 
leResult leApplication_MediaReadRequest(leAssetStreamReader* reader, // asset reader
                                        void* address,  // address
                                        uint32_t size,  // dest size
                                        uint8_t* buf);  // dest buffer);

// *****************************************************************************
/* typedef:
    leMediaCloseRequest_FnPtr

  Summary:
    A callback that indicates that a media decoder is finished with a given
    media location and that the application can close if it.
    
    reader - the reader that was streaming data.
*/
void leApplication_MediaCloseRequest(leAssetStreamReader* reader);


#endif /* LE_EXTERNAL_STREAMING_ENABLED */

#endif /* LE_ASSET_H */