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


#include "gfx/legato/core/legato_event.h"

#include <string.h>

#include "gfx/legato/core/legato_input.h"
#include "gfx/legato/memory/legato_memory.h"

static leEventState _state; // the global event state

static leEventResult processEvent(leEvent* evt);

// DOM-IGNORE-BEGIN
/* internal use only */
leEventState* _leGetEventState()
{
    return &_state;
}

leResult leEvent_Init()
{
    leList_Create(&_state.events);
    
    _state.filter = NULL;
    
    return LE_SUCCESS;
}

void leEvent_Shutdown()
{
    leList_Destroy(&_state.events);
}

// DOM-IGNORE-END

uint32_t leEvent_GetCount()
{
    return _state.events.size;
}

leResult leEvent_SetFilter(leEvent_FilterEvent cb)
{
    _state.filter = cb;

    return LE_SUCCESS;
}

leResult leEvent_AddEvent(leEvent* evt)
{
    if(evt == NULL)
        return LE_FAILURE;

    leList_PushBack(&_state.events, evt);

    return LE_SUCCESS;
}

leResult leEvent_ClearList()
{
    leList_Destroy(&_state.events);

    return LE_SUCCESS;
}

leResult leEvent_ProcessEvents()
{
    leEvent* evt;
    leListNode* node;
    leEventResult res;

    if(_state.events.size == 0)
        return LE_SUCCESS;
        
    node = _state.events.head;
    
    if(_state.events.size > 1)
    {
        node = node;
    }
    
    while(node != NULL)
    {
        //evt = _state.events.values[i];
        evt = (leEvent*)node->val;

        if(_state.filter != NULL &&
           _state.filter(evt) == LE_FALSE)
           continue;

        node = node->next;

        res = processEvent(evt);
        
        if(res == LE_EVENT_HANDLED)
        {
            leList_Remove(&_state.events, evt);
            
            LE_FREE(evt);
        }
        else if(res == LE_EVENT_RESET_QUEUE)
        {
            leList_Destroy(&_state.events);
            
            node = NULL;
        }
    }

    return LE_SUCCESS;
}

static leEventResult processEvent(leEvent* evt)
{
    switch(evt->id)
    {
        case LE_EVENT_TOUCH_DOWN:
        case LE_EVENT_TOUCH_UP:
        case LE_EVENT_TOUCH_MOVE:
        {
            return _leInput_HandleInputEvent(evt);
        }
        default:
            break;
    }
    
    return LE_EVENT_HANDLED;
}

void leWidgetEvent_Accept(leWidgetEvent* evt, leWidget* owner)
{
    if(evt == NULL)
        return;

    evt->accepted = LE_TRUE;
    evt->owner = owner;
}