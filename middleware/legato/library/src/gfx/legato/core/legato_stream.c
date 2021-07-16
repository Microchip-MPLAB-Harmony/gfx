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

#include "gfx/legato/core/legato_stream.h"
#include "legato_stream.h"

#if LE_STREAMING_ENABLED == 1

void leStream_Init(leStream* stream,
                   const leStreamDescriptor* desc,
                   uint32_t cacheSize,
                   uint8_t* cacheBuf,
                   void* userData)
{
    if(stream == NULL)
        return;

    stream->state = LE_STREAM_CLOSED;
    stream->desc = desc;
    stream->cache.ptr = cacheBuf;
    stream->cache.physicalSize = cacheSize;
    stream->userData = userData;
}

leResult leStream_Open(leStream* stream)
{
    if(stream == NULL ||
       stream->desc == NULL ||
       stream->state != LE_STREAM_CLOSED)
    {
        return LE_FAILURE;
    }

    stream->flags = SF_NONE;

    if(stream->desc->location != LE_STREAM_LOCATION_ID_INTERNAL)
    {
        if(leApplication_MediaOpenRequest(stream) == LE_FAILURE)
            return LE_FAILURE;

        stream->state = LE_STREAM_READY;
    }

    return LE_SUCCESS;
}

leBool leStream_IsOpen(leStream* stream)
{
    if(stream == NULL ||
      stream->desc == NULL ||
      stream->state == LE_STREAM_CLOSED)
    {
        return LE_FALSE;
    }

    return LE_TRUE;
}

leResult leStream_Read(leStream* stream,
                       size_t addr,
                       uint32_t size,
                       uint8_t* buf,
                       leStream_DataReadyCallback cb)
{
    uint32_t remaining;
    uint32_t copyOffs;

    if(stream == NULL ||
        buf == NULL ||
        (stream->state != LE_STREAM_READY && stream->state != LE_STREAM_DATAREADY))
    {
        return LE_FAILURE;
    }

    if(stream->desc->location == LE_STREAM_LOCATION_ID_INTERNAL)
    {
        memcpy(buf, (void*)addr, size);

        if(cb != NULL)
        {
            cb(stream);
        }
    }
    else
    {
        // cache is enabled
        if(stream->cache.ptr != NULL)
        {
            // requested data is in the local cache
            if((uint32_t)addr >= stream->cache.baseAddress &&
               (uint32_t)addr - stream->cache.baseAddress + size < stream->cache.logicalSize)
            {
                copyOffs = (uint32_t)addr - stream->cache.baseAddress;

                memcpy(buf,
                       &stream->cache.ptr[copyOffs],
                       size);

                if(cb != NULL)
                {
                    cb(stream);
                }
            }
            // fill cache with new data
            else
            {
                remaining = stream->cache.physicalSize;

                if(stream->desc->size - (addr - (size_t)stream->desc->address) < remaining)
                {
                    remaining = stream->desc->size - (addr - (size_t)stream->desc->address);
                }

                stream->cache.baseAddress = (size_t)addr;
                stream->cache.logicalSize = remaining;

                stream->state = LE_STREAM_WAITING;

                stream->readRequest.size = size;
                stream->readRequest.buf = buf;
                stream->readRequest.dataReadyCB = cb;

                if(leApplication_MediaReadRequest(stream,
                                                  stream->cache.baseAddress,
                                                  stream->cache.logicalSize,
                                                  stream->cache.ptr) == LE_FAILURE)
                {
                    return LE_FAILURE;
                }
            }
        }
        // read from data source directly
        else
        {
            stream->state = LE_STREAM_WAITING;

            stream->readRequest.size = size;
            stream->readRequest.buf = buf;
            stream->readRequest.dataReadyCB = cb;

            if(leApplication_MediaReadRequest(stream, addr, size, buf) == LE_FAILURE)
                return LE_FAILURE;

            if((stream->flags & SF_BLOCKING) > 0)
            {
                leStream_DataReady(stream);
            }
        }
    }

    return LE_SUCCESS;
}

leBool leStream_IsBlocking(leStream* stream)
{
    return stream != NULL && (stream->flags & SF_BLOCKING) > 0;
}

leBool leStream_IsDataReady(leStream* stream)
{
    return stream != NULL && stream->state == LE_STREAM_DATAREADY;
}

leResult leStream_DataReady(leStream* stream)
{
    if(stream == NULL || stream->state != LE_STREAM_WAITING)
        return LE_FAILURE;

    stream->state = LE_STREAM_DATAREADY;

    // this was a cache fill operation
    if(stream->cache.ptr != NULL)
    {
        // fill the read buffer from the cache
        memcpy(stream->readRequest.buf,
               stream->cache.ptr,
               stream->readRequest.size);
    }

    // call the callback
    if(stream->readRequest.dataReadyCB != NULL)
    {
        stream->readRequest.dataReadyCB(stream);
    }

    return LE_SUCCESS;
}

leResult leStream_Close(leStream* stream)
{
    if(stream == NULL || stream->state == LE_STREAM_CLOSED)
        return LE_FAILURE;

    if(stream->desc->location != LE_STREAM_LOCATION_ID_INTERNAL)
    {
        leApplication_MediaCloseRequest(stream);

        stream->state = LE_STREAM_CLOSED;
    }

    return LE_SUCCESS;
}

#endif /* LE_STREAMING_ENABLED */
