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


#include "gfx/libaria/inc/libaria_input.h"
#include "gfx/libaria/inc/libaria_context.h"
#include "gfx/libaria/inc/libaria_layer.h"
#include "gfx/libaria/inc/libaria_screen.h"
#include "gfx/libaria/inc/libaria_utils.h"
#include "gfx/libaria/inc/libaria_widget.h"
#include "gfx/libaria/inc/libaria_input_rtos.h"
#include "gfx/libaria/inc/libaria_event_rtos.h"


//May need to put this in a header file
extern GFX_Point transformCoordinate(int32_t x, int32_t y); 

laResult laInput_SendTouchDown_Ext_RTOS(uint32_t id, int32_t x, int32_t y)
{
    laInput_TouchDownEvent* evt;
    laContext* context = laContext_GetActive();
    GFX_Point pnt;

    if(context == NULL ||                          // need a valid context
       context->input.enabled == LA_FALSE ||       // inputs are disabled
       id >= LA_MAX_TOUCH_STATES ||                // don't overrun array
       context->input.touch[id].valid == LA_TRUE)  // touch id must be invalid
        return GFX_FAILURE;

    // reorient touch coordinates if the user interface is rotated
    pnt = transformCoordinate(x, y);

    // dispatch the event
    context->input.touch[id].valid = LA_TRUE;
    context->input.touch[id].x = pnt.x;
    context->input.touch[id].y = pnt.y;

    evt = context->memIntf.heap.calloc(1, sizeof(laInput_TouchDownEvent));

    if(evt == NULL)
        return GFX_FAILURE;
    
    evt->event.id = LA_EVENT_TOUCH_DOWN;
    evt->touchID = id;
    evt->x = pnt.x;
    evt->y = pnt.y;

#ifdef INPUT_EVENT_DEBUG
    printf("adding touch down event %i, %i\n", evt->x, evt->y);
#endif

    if(laEvent_SendEvent_Ext_RTOS((laEvent*)evt) == GFX_FAILURE)
    {
        context->memIntf.heap.free(evt);

        return GFX_FAILURE;
    }    

    return GFX_SUCCESS;
}

laResult laInput_SendTouchUp_Ext_RTOS(uint32_t id, int32_t x, int32_t y)
{
    laInput_TouchUpEvent* evt;
    laContext* context = laContext_GetActive();
    GFX_Point pnt;

    if(context == NULL ||                           // need a valid context
       id >= LA_MAX_TOUCH_STATES ||                 // don't overrun array
       context->input.touch[id].valid == LA_FALSE)  // touch id must be valid
        return GFX_FAILURE;

    context->input.touch[id].valid = LA_FALSE;

    // reorient touch coordinates if the user interface is rotated
    pnt = transformCoordinate(x, y);

    // dispatch event
    evt = context->memIntf.heap.calloc(1, sizeof(laInput_TouchUpEvent));

    if(evt == NULL)
        return GFX_FAILURE;
    
    evt->event.id = LA_EVENT_TOUCH_UP;
    evt->touchID = id;
    evt->x = pnt.x;
    evt->y = pnt.y;
    
#ifdef INPUT_EVENT_DEBUG
    printf("adding touch up event %i, %i\n", evt->x, evt->y);
#endif

    if(laEvent_SendEvent_Ext_RTOS((laEvent*)evt) == GFX_FAILURE)
    {
        context->memIntf.heap.free(evt);

        return GFX_FAILURE;
    }    

    return GFX_SUCCESS;
}

laResult laInput_SendTouchMoved_Ext_RTOS(uint32_t id, int32_t x, int32_t y)
{
    laInput_TouchMovedEvent* evt;
    laEvent* evtPtr;
    laContext* context = laContext_GetActive();
    GFX_Point pnt;
    laListNode* node;

    if(context == NULL ||                           // need a valid context
       id >= LA_MAX_TOUCH_STATES ||                 // don't overrun array
       context->input.touch[id].valid == LA_FALSE)  // touch id must be valid
        return GFX_FAILURE;
        
    // find any existing touch moved event and overwrite
    node = context->event.events.head;
    
    while(node != NULL)
    {
        evtPtr = (laEvent*)node->val;
        
        if(evtPtr->id == LA_EVENT_TOUCH_MOVED)
        {
            evt = (laInput_TouchMovedEvent*)evtPtr;
         
            if(evt->touchID == id)
            {   
#ifdef INPUT_EVENT_DEBUG
                printf("overwriting previous move event\n");
#endif                
                
                // reorient touch coordinates if the user interface is rotated
                pnt = transformCoordinate(x, y);
                
                evt->x = x;
                evt->y = y;
                
                evt->x = pnt.x;
                evt->y = pnt.y;

                context->input.touch[id].x = pnt.x;
                context->input.touch[id].y = pnt.y;
                
                return LA_SUCCESS;
            }
        }
        
        node = node->next;
    }

    evt = context->memIntf.heap.calloc(1, sizeof(laInput_TouchMovedEvent));

    if(evt == NULL)
        return GFX_FAILURE;

    // reorient touch coordinates if the user interface is rotated
    pnt = transformCoordinate(x, y);

    evt->event.id = LA_EVENT_TOUCH_MOVED;
    evt->touchID = id;
    evt->prevX = context->input.touch[id].x;
    evt->prevY = context->input.touch[id].y;
    evt->x = pnt.x;
    evt->y = pnt.y;

    context->input.touch[id].x = pnt.x;
    context->input.touch[id].y = pnt.y;

#ifdef INPUT_EVENT_DEBUG
    printf("adding move event %i, %i, %i, %i\n", evt->x, evt->y, evt->prevX, evt->prevY);
#endif

    if(laEvent_SendEvent_Ext_RTOS((laEvent*)evt) == GFX_FAILURE)
    {
        context->memIntf.heap.free(evt);

        return GFX_FAILURE;
    }    

    return GFX_SUCCESS;
}
