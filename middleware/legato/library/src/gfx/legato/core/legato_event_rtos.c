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


#include "gfx/libaria/core/libaria_event.h"
#include "gfx/libaria/core/libaria_event_rtos.h"

#include "gfx/libaria/core/libaria_input.h"

static laEventResult processEvent_RTOS(laEvent* evt);

laResult laEvent_RTOS_Initialize(void)
{
    laContext* context = laContext_GetActive();
    
    OSAL_SEM_Create(&context->event.eventCountSem,
            OSAL_SEM_TYPE_COUNTING,
            100,
            0);
    
    OSAL_MUTEX_Create(&context->event.eventLock);
            
    return LA_SUCCESS;
}

laResult laEvent_RTOS_DeInitialize(void)
{
    laContext* context = laContext_GetActive();
    
    OSAL_SEM_Delete(&context->event.eventCountSem);
    
    OSAL_MUTEX_Delete(&context->event.eventLock);
            
    return LA_SUCCESS;
}

uint32_t laEvent_GetCount_RTOS()
{
    laContext* context = laContext_GetActive();
    uint32_t size = 0;
    
    if(context == NULL)
        return 0;

    OSAL_MUTEX_Lock(&context->event.eventLock, OSAL_WAIT_FOREVER);
    
    size = context->event.events.size;
    
    OSAL_MUTEX_Unlock(&context->event.eventLock);
    
    return size;
}

/*laResult laEvent_GetEvent(uint32_t index, laEvent* evt)
{
    laContext* context = laContext_GetActive();
    
    if(context == NULL || index >= context->event.events.size)
        return LA_FAILURE;

    laList_GetAtIndex(

    *evt = *context->event.events.elements[index];

    return LA_SUCCESS;
}*/

laResult laEvent_SetFilter_RTOS(laEvent_FilterEvent cb)
{
    laContext* context = laContext_GetActive();
    
    if(context == NULL)
        return LA_FAILURE;

    OSAL_MUTEX_Lock(&context->event.eventLock, OSAL_WAIT_FOREVER);
    
    context->event.filter = cb;
    
    OSAL_MUTEX_Unlock(&context->event.eventLock);

    return LA_SUCCESS;
}

laResult laEvent_ClearList_RTOS()
{
    laContext* context = laContext_GetActive();

    if(context == NULL)
        return LA_FAILURE;

    OSAL_MUTEX_Lock(&context->event.eventLock, OSAL_WAIT_FOREVER);
    
    laList_Destroy(&context->event.events);
    
    OSAL_MUTEX_Unlock(&context->event.eventLock);
    
    return LA_SUCCESS;
}

laResult laEvent_AddEvent_RTOS(laEvent* evt)
{
    laContext* context = laContext_GetActive();

    if(context == NULL || evt == NULL)
        return LA_FAILURE;

    laList_PushBack(&context->event.events, evt);
    
    //Increment the event semaphore
    OSAL_SEM_Post(&context->event.eventCountSem);


    return LA_SUCCESS;
}

laResult laEvent_SendEvent_Ext_RTOS(laEvent* evt)
{
    laContext* context = laContext_GetActive();

    if(context == NULL || evt == NULL)
        return LA_FAILURE;

    OSAL_MUTEX_Lock(&context->event.eventLock, OSAL_WAIT_FOREVER);
    
    laList_PushBack(&context->event.events, evt);
    
    OSAL_MUTEX_Unlock(&context->event.eventLock);
    
    //Increment the event semaphore
    OSAL_SEM_Post(&context->event.eventCountSem);

    return LA_SUCCESS;
}

laResult laEvent_ProcessEvents_RTOS()
{
   laContext* context = laContext_GetActive();
    laEvent* evt;
    laListNode* node;
    laEventResult res;

    if(context == NULL)
        return LA_FAILURE;

    if(context->event.events.size == 0)
        return LA_FAILURE;
        
    node = context->event.events.head;
    
    if(context->event.events.size > 1)
    {
        node = node;
    }
    
    while(node != NULL)
    {
        //evt = context->event.events.values[i];
        evt = (laEvent*)node->val;

        if(context->event.filter != NULL &&
           context->event.filter(evt) == LA_FALSE)
           continue;

        node = node->next;

        res = processEvent_RTOS(evt);
        
        if(res == LA_EVENT_HANDLED)
        {
            laList_Remove(&context->event.events, evt);
            
            context->memIntf.heap.free(evt);
        }
        else if(res == LA_EVENT_RESET_QUEUE)
        {
            laList_Destroy(&context->event.events);
            
            node = NULL;
        }
    }

    return LA_SUCCESS;
}

static laEventResult processEvent_RTOS(laEvent* evt)
{
    switch(evt->id)
    {
        case LA_EVENT_SCREEN_CHANGE:
        {
            return _laContext_HandleScreenChangeEvent(evt);
            
            break;
        }
        case LA_EVENT_TOUCH_DOWN:
        case LA_EVENT_TOUCH_UP:
        case LA_EVENT_TOUCH_MOVED:
        {
            return _laInput_HandleInputEvent(evt);
        }
        default:
            break;
    }
    
    return LA_EVENT_HANDLED;
}
