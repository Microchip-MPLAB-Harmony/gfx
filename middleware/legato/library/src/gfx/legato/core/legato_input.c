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


#include "gfx/legato/core/legato_input.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/widget/legato_widget.h"
#include "legato_state.h"
#include "gfx/legato/renderer/legato_renderer.h"

#include <string.h>

static leInputState _state; // the global input state

leInputState* _leGetInputState()
{
    return &_state;
}

leResult leInput_Init()
{
    memset(&_state, 0, sizeof(leInputState));
    
    _state.enabled = LE_TRUE;
    
    return LE_SUCCESS;
}

void leInput_Shutdown()
{
    
}

leBool leInput_GetEnabled()
{
    return _state.enabled;
}

leResult leInput_SetEnabled(leBool enable)
{
    _state.enabled = enable;

    return LE_SUCCESS;
}

leResult leInput_InjectTouchDown(uint32_t id, int32_t x, int32_t y)
{
    leWidgetEvent_TouchDown* evt;
    lePoint pnt;
#if LE_TOUCH_ORIENTATION != 0
    leSize dispSize;
#endif

    if(_state.enabled == LE_FALSE ||               // inputs are disabled
       id >= LE_MAX_TOUCH_STATES ||                // don't overrun array
       _state.touch[id].valid == LE_TRUE)  // touch id must be invalid
    {
        return LE_FAILURE;
    }

    // reorient touch coordinates if the user interface is rotated
#if LE_TOUCH_ORIENTATION != 0
    dispSize.width = 0;
    dispSize.height = 0;

    leRenderer_DisplaySize(&dispSize);
#endif

#if LE_TOUCH_ORIENTATION == 0
    pnt.x = x;
    pnt.y = y;
#elif LE_TOUCH_ORIENTATION == 90 // 90 degrees
    pnt.y = dispSize.width - 1 - x;
	pnt.x = y;
#elif LE_TOUCH_ORIENTATION == 180 // 180 degrees
    pnt.x = dispSize.width - 1 - x;
    pnt.y = dispSize.height - 1 - y;
#else // 270 degrees
    pnt.y = x;
    pnt.x = dispSize.height - 1 - y;
#endif

    // dispatch the event
    _state.touch[id].valid = LE_TRUE;
    _state.touch[id].x = pnt.x;
    _state.touch[id].y = pnt.y;

    evt = LE_MALLOC(sizeof(leWidgetEvent_TouchDown));

    if(evt == NULL)
        return LE_FAILURE;
    
    evt->event.owner = NULL;
    evt->event.id = LE_EVENT_TOUCH_DOWN;
    evt->touchID = id;
    evt->x = pnt.x;
    evt->y = pnt.y;

#ifdef INPUT_EVENT_DEBUG
    printf("adding touch down event %i, %i\n", evt->x, evt->y);
#endif

    if(leEvent_AddEvent((leEvent*)evt) == LE_FAILURE)
    {
        LE_FREE(evt);

        return LE_FAILURE;
    }    

    return LE_SUCCESS;
}

leResult leInput_InjectTouchUp(uint32_t id, int32_t x, int32_t y)
{
    leWidgetEvent_TouchUp* evt;
    lePoint pnt;
#if LE_TOUCH_ORIENTATION != 0
    leSize dispSize;
#endif

    if(id >= LE_MAX_TOUCH_STATES ||         // don't overrun array
       _state.touch[id].valid == LE_FALSE)  // touch id must be valid
    {
        return LE_FAILURE;
    }
    
    _state.touch[id].valid = LE_FALSE;

    // reorient touch coordinates if the user interface is rotated
#if LE_TOUCH_ORIENTATION != 0
    dispSize.width = 0;
    dispSize.height = 0;

    leRenderer_DisplaySize(&dispSize);
#endif

#if LE_TOUCH_ORIENTATION == 0
    pnt.x = x;
    pnt.y = y;
#elif LE_TOUCH_ORIENTATION == 90 // 90 degrees
    pnt.y = dispSize.width - 1 - x;
	pnt.x = y;
#elif LE_TOUCH_ORIENTATION == 180 // 180 degrees
    pnt.x = dispSize.width - 1 - x;
    pnt.y = dispSize.height - 1 - y;
#else // 270 degrees
    pnt.y = x;
    pnt.x = dispSize.height - 1 - y;
#endif

    // dispatch event
    evt = LE_MALLOC(sizeof(leWidgetEvent_TouchUp));

    if(evt == NULL)
        return LE_FAILURE;

    evt->event.owner = NULL;
    evt->event.id = LE_EVENT_TOUCH_UP;
    evt->touchID = id;
    evt->x = pnt.x;
    evt->y = pnt.y;
    
#ifdef INPUT_EVENT_DEBUG
    printf("adding touch up event %i, %i\n", evt->x, evt->y);
#endif

    if(leEvent_AddEvent((leEvent*)evt) == LE_FAILURE)
    {
        LE_FREE(evt);

        return LE_FAILURE;
    }    

    return LE_SUCCESS;
}

leResult leInput_InjectTouchMoved(uint32_t id, int32_t x, int32_t y)
{
    leWidgetEvent_TouchMove* evt;
    leEvent* evtPtr;
    lePoint pnt;
    leListNode* node;
#if LE_TOUCH_ORIENTATION != 0
    leSize dispSize;
#endif

    if(id >= LE_MAX_TOUCH_STATES ||         // don't overrun array
       _state.touch[id].valid == LE_FALSE)  // touch id must be valid
        return LE_FAILURE;

#if LE_TOUCH_ORIENTATION != 0
    dispSize.width = 0;
    dispSize.height = 0;

    leRenderer_DisplaySize(&dispSize);
#endif

    // find any existing touch moved event and overwrite
    node = _leGetEventState()->events.head;
    
    while(node != NULL)
    {
        evtPtr = (leEvent*)node->val;
        
        if(evtPtr->id == LE_EVENT_TOUCH_MOVE)
        {
            evt = (leWidgetEvent_TouchMove*)evtPtr;
         
            if(evt->touchID == id)
            {   
#ifdef INPUT_EVENT_DEBUG
                printf("overwriting previous move event\n");
#endif

                // reorient touch coordinates if the user interface is rotated
#if LE_TOUCH_ORIENTATION == 0
                pnt.x = x;
                pnt.y = y;
#elif LE_TOUCH_ORIENTATION == 90 // 90 degrees
                pnt.y = dispSize.width - 1 - x;
	            pnt.x = y;
#elif LE_TOUCH_ORIENTATION == 180 // 180 degrees
                pnt.x = dispSize.width - 1 - x;
                pnt.y = dispSize.height - 1 - y;
#else // 270 degrees
                pnt.y = x;
                pnt.x = dispSize.height - 1 - y;
#endif
                
                evt->x = x;
                evt->y = y;
                
                evt->x = pnt.x;
                evt->y = pnt.y;

                _state.touch[id].x = pnt.x;
                _state.touch[id].y = pnt.y;
                
                return LE_SUCCESS;
            }
        }
        
        node = node->next;
    }

    evt = LE_MALLOC(sizeof(leWidgetEvent_TouchMove));

    if(evt == NULL)
        return LE_FAILURE;

    // reorient touch coordinates if the user interface is rotated
#if LE_TOUCH_ORIENTATION == 0
    pnt.x = x;
    pnt.y = y;
#elif LE_TOUCH_ORIENTATION == 90 // 90 degrees
    pnt.y = dispSize.width - 1 - x;
	pnt.x = y;
#elif LE_TOUCH_ORIENTATION == 180 // 180 degrees
    pnt.x = dispSize.width - 1 - x;
    pnt.y = dispSize.height - 1 - y;
#else // 270 degrees
    pnt.y = x;
    pnt.x = dispSize.height - 1 - y;
#endif

    evt->event.owner = NULL;
    evt->event.id = LE_EVENT_TOUCH_MOVE;
    evt->touchID = id;
    evt->prevX = _state.touch[id].x;
    evt->prevY = _state.touch[id].y;
    evt->x = pnt.x;
    evt->y = pnt.y;

    _state.touch[id].x = pnt.x;
    _state.touch[id].y = pnt.y;

#ifdef INPUT_EVENT_DEBUG
    printf("adding move event %i, %i, %i, %i\n", evt->x, evt->y, evt->prevX, evt->prevY);
#endif

    if(leEvent_AddEvent((leEvent*)evt) == LE_FAILURE)
    {
        LE_FREE(evt);

        return LE_FAILURE;
    }    

    return LE_SUCCESS;
}

leEventResult handleTouchDown(leWidgetEvent_TouchDown* evt)
{
    leWidget* targetWidget = NULL;
    int32_t x = 0;
    int32_t y = 0;
    int32_t i;
    leState* state = leGetState();
    leLayerState* layerState;

#if LE_DRIVER_LAYER_MODE == 1
    gfxIOCTLArg_LayerRect layerRect;
#endif
    
    // find the topmost widget on the topmost layer for the touch event
    for(i = leGetState()->layerList.size - 1; i >= 0; i--)
    {
        layerState = (leLayerState*)leList_Get(&state->layerList, i);

        x = evt->x;
        y = evt->y;


// all layers look like they're at 0, 0 to legato
// however, the driver could be rendering them at different coordinates
// the event coordinates may need to be adjusted to account for this
#if LE_DRIVER_LAYER_MODE == 1
        layerRect.base.id = i;
        layerRect.x = 0;
        layerRect.y = 0;
        layerRect.width = 0;
        layerRect.height = 0;

        leRenderer_DisplayInterface()->ioctl(GFX_IOCTL_GET_LAYER_RECT, &layerRect);

        // need to remember this offset for subsequent touch down/move events
        _state.driverAdjustX = layerRect.x;
        _state.driverAdjustY = layerRect.y;

        x -= layerRect.x;
        y -= layerRect.y;
#endif
        
        targetWidget = leUtils_PickFromWidget(&layerState->root, x, y);
        
        if(targetWidget != NULL)
            break; 
    }

    // no widget qualified
    if(targetWidget == NULL)
    {
        leSetFocusWidget(NULL);

        return LE_EVENT_HANDLED;
    }

    if(leIsDrawing() == LE_TRUE)
        return LE_EVENT_DEFERRED;

    evt->x = x;
    evt->y = y;    
    
    while(targetWidget != NULL)
    {
        targetWidget->fn->_handleEvent(targetWidget, (leEvent*)evt);
        
        if(evt->event.accepted == LE_TRUE)
            break;

        targetWidget = targetWidget->parent;
    }

    if(evt->event.owner != NULL)
    {
        leSetFocusWidget(evt->event.owner);
    }
    // no widget claimed the event
    else
    {
        leSetFocusWidget(NULL);
    }

#ifdef INPUT_EVENT_DEBUG  
    printf("handling touch down event %i, %i\n", evt->x, evt->y);
#endif
    
    return LE_EVENT_HANDLED;
}

leBool handleTouchUp(leWidgetEvent_TouchUp* evt)
{
    leWidget* wgt = leGetFocusWidget();
    
    if(wgt == NULL)
        return LE_EVENT_HANDLED;
        
    if(leIsDrawing() == LE_FALSE)
    {
#if LE_DRIVER_LAYER_MODE == 1
        evt->x -= _state.driverAdjustX;
        evt->y -= _state.driverAdjustY;
#endif

        wgt->fn->_handleEvent(wgt, (leEvent*)evt);

#ifdef INPUT_EVENT_DEBUG
        printf("handling touch up event %i, %i\n", evt->x, evt->y);
#endif       
        
        return LE_EVENT_HANDLED;
    }
    
    return LE_EVENT_DEFERRED;
}

leBool handleTouchMoved(leWidgetEvent_TouchMove* evt)
{
    leWidget* wgt = leGetFocusWidget();

    if(wgt == NULL)
        return LE_EVENT_HANDLED;
        
    if(leIsDrawing() == LE_FALSE)
    {
#if LE_DRIVER_LAYER_MODE == 1
        evt->x -= _state.driverAdjustX;
        evt->y -= _state.driverAdjustY;
#endif
       wgt->fn->_handleEvent(wgt, (leEvent*)evt);

#ifdef INPUT_EVENT_DEBUG        
        printf("handling touch move event %i, %i, %i, %i\n", evt->x, evt->y, evt->prevX, evt->prevY);
#endif
        
        return LE_EVENT_HANDLED;
    }

    return LE_EVENT_DEFERRED;
}

leEventResult _leInput_HandleInputEvent(leEvent* evt)
{
    switch(evt->id)
    {
        case LE_EVENT_TOUCH_DOWN:
        {
            return handleTouchDown((leWidgetEvent_TouchDown*)evt);

#ifdef INPUT_EVENT_DEBUG                
            printf("handled touch down\n");
#endif                
                
            break;
        }
        case LE_EVENT_TOUCH_UP:
        {
            return handleTouchUp((leWidgetEvent_TouchUp*)evt);

#ifdef INPUT_EVENT_DEBUG               
            printf("handled touch up\n");
#endif                
                
            break;
        }
        case LE_EVENT_TOUCH_MOVE:
        {
            return handleTouchMoved((leWidgetEvent_TouchMove*)evt);
                return LE_FAILURE;

#ifdef INPUT_EVENT_DEBUG                
            printf("handled touch move\n");
#endif                
                
            break;
        }
        default:
            break;
    }

    return LE_SUCCESS;
}
