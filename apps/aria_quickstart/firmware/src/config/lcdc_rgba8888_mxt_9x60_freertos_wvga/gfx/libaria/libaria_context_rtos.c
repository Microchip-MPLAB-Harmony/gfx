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
 Module for Microchip Graphics Library - Graphic Object Layer

  Company:
    Microchip Technology Inc.

  File Name:
    libaria_context_rtos.c

  Summary:
    This file contains the RTOS extensions of libaria_context.c

  Description:
    This module implements the common routines for the Graphics Object Layer
    of the Microchip Graphics Library. The routines are independent of the
    Display Driver Layer and should be compatible with any Display Driver
    that is compliant with the requirements of the Display Driver
    Layer of the Graphics Library.
    The module utilizes the Graphics Primitive Layer to render the objects.
*******************************************************************************/

#include "osal/osal.h"
#include "gfx/libaria/inc/libaria_context.h"

#include "gfx/hal/gfx.h"
#include "gfx/utils/gfx_utils.h"

#include "gfx/libaria/inc/libaria_draw.h"
#include "gfx/libaria/inc/libaria_layer.h"
#include "gfx/libaria/inc/libaria_list.h"
#include "gfx/libaria/inc/libaria_screen.h"
#include "gfx/libaria/inc/libaria_utils.h"

#include "gfx/libaria/inc/libaria_event_rtos.h"

extern laContext* _activeContext;

typedef struct laContext_ScreenChangeEvent_t
{
    laEvent event;
    
    uint32_t index;
} laContext_ScreenChangeEvent;

typedef struct laContext_ScreenRefreshEvent_t
{
    laEvent event;
} laContext_ScreenRefreshEvent;

laResult laContext_Create_RTOS(laContext* context)
{
    return laEvent_RTOS_Initialize();
}

laResult laContext_Destroy_RTOS(laContext* context)
{
    return laEvent_RTOS_DeInitialize();
}

laResult laContext_RefreshActiveScreen_RTOS()
{
    laContext_ScreenRefreshEvent * evt;
    
    if(_activeContext == NULL)
        return LA_FAILURE;

    evt = _activeContext->memIntf.heap.malloc(sizeof(laContext_ScreenRefreshEvent));
        
    //Send a dummy event for now
    evt->event.id = LA_EVENT_NONE;;
    
    laEvent_AddEvent_RTOS((laEvent*)evt);

    return LA_SUCCESS;
}

laResult laContext_RefreshActiveScreen_Ext_RTOS()
{
    laContext_ScreenRefreshEvent * evt;
    
    if(_activeContext == NULL)
        return LA_FAILURE;

    evt = _activeContext->memIntf.heap.malloc(sizeof(laContext_ScreenRefreshEvent));
        
    //Send a dummy event for now
    evt->event.id = LA_EVENT_NONE;;
    
    laEvent_SendEvent_Ext_RTOS((laEvent*)evt);

    return LA_SUCCESS;
}

laResult laContext_SetActiveScreen_RTOS(uint32_t idx)
{
    laContext_ScreenChangeEvent* evt;
    
    if(_activeContext == NULL || idx < 0 || idx >= _activeContext->screenList.size)
        return LA_FAILURE;

    if(laContext_GetActiveScreenIndex() == idx)
        return LA_SUCCESS;
    
    evt = _activeContext->memIntf.heap.malloc(sizeof(laContext_ScreenChangeEvent));
        
    evt->event.id = LA_EVENT_SCREEN_CHANGE;
    evt->index = idx;
        
    laEvent_AddEvent_RTOS((laEvent*)evt);
    
    return LA_SUCCESS;
}

laResult laContext_SendSetActiveScreenEvent_Ext_RTOS(uint32_t idx)
{
    laContext_ScreenChangeEvent* evt;
    
    if(_activeContext == NULL || idx < 0 || idx >= _activeContext->screenList.size)
        return LA_FAILURE;

    if(laContext_GetActiveScreenIndex() == idx)
        return LA_SUCCESS;
    
    evt = _activeContext->memIntf.heap.malloc(sizeof(laContext_ScreenChangeEvent));
        
    evt->event.id = LA_EVENT_SCREEN_CHANGE;
    evt->index = idx;
        
    laEvent_SendEvent_Ext_RTOS((laEvent*)evt);
    
    return LA_SUCCESS;
}

void laContext_Update_RTOS(laBool fullBlock, uint32_t dt)
{
    laResult result;
    laContextUpdateState updateState;
    
    if(_activeContext == NULL)
        return;
    
    if (fullBlock == LA_TRUE)
    {
        OSAL_SEM_Pend(&_activeContext->event.eventCountSem, OSAL_WAIT_FOREVER);
    }
    
    do
    {
        OSAL_MUTEX_Lock(&_activeContext->event.eventLock, OSAL_WAIT_FOREVER);
    
        //Process as much events as we can, before we block at the event semaphore
        result = laEvent_ProcessEvents_RTOS();
        
        updateState = _laContext_Update(dt);
        //Context has pending updates for next cycle, send a future refresh event
        if (updateState == LA_CONTEXT_UPDATE_PENDING)
        {
            laContext_RefreshActiveScreen_RTOS();
        }
        
        //TODO: explore possibility of having the paint loop in a separate thread
        while (_activeContext->frameState != LA_CONTEXT_FRAME_READY)
        {
            _laContext_Paint();
        }
        
        OSAL_MUTEX_Unlock(&_activeContext->event.eventLock);
        
    } while (result != LA_FAILURE);
}