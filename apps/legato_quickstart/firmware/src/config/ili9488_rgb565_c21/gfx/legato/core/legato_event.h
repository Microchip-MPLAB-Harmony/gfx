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
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_event.h

  Summary:
    Defines events that are used in the UI library.  Events are created and
    stored for later processing during a library context's update loop.

*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_EVENT_H
#define LEGATO_EVENT_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/datastructure/legato_list.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leEventID

  Summary:
    Defines internal event type IDs
*/
typedef enum leEventID
{
    // internal events
    LE_EVENT_NONE,
    LE_EVENT_TOUCH_DOWN,
    LE_EVENT_TOUCH_UP,
    LE_EVENT_TOUCH_MOVE,
    LE_EVENT_LANGUAGE_CHANGED,
    
    // widget events
    LE_WIDGET_EVENT_PAINT = 100,
    LE_WIDGET_EVENT_MOVED,
    LE_WIDGET_EVENT_RESIZED,
    LE_WIDGET_EVENT_FOCUS_GAINED,
    LE_WIDGET_EVENT_FOCUS_LOST,
} leEventID;

typedef struct leWidget leWidget;

// *****************************************************************************
/* Structure:
    leEvent

  Summary:
    Basic UI event definition
*/
typedef struct leEvent
{
    leEventID id;
} leEvent;

// *****************************************************************************
/* Function Pointer:
    leEvent_FilterEvent

  Summary:
    Function pointer to define an event filter.  Event filters allow a
    receiver to discard undesirable events
*/
typedef leBool (*leEvent_FilterEvent)(leEvent*);

// *****************************************************************************
/* Structure:
    leEventState

  Summary:
    Structure to manage the event lists, state and call back pointers

  Remarks:
    None.
*/
typedef struct leEventState
{
#ifndef _WIN32
#ifdef LEGATO_USE_OSAL
    OSAL_SEM_HANDLE_TYPE eventCountSem;
    OSAL_MUTEX_HANDLE_TYPE eventLock;
#endif
#endif
    leList events;
    leEvent_FilterEvent filter;
} leEventState;

// DOM-IGNORE-BEGIN
/* internal use only */
leEventState* _leGetEventState();

leResult leEvent_Init();
void leEvent_Shutdown();

// DOM-IGNORE-END


// *****************************************************************************
/* Enumeration:
    leEventResult

  Summary:
    Defines what happened when processing an event
*/
typedef enum leEventResult
{
    // internal events
    LE_EVENT_HANDLED,     // the event was handled
    LE_EVENT_DEFERRED,    // the event needs to wait
    LE_EVENT_RESET_QUEUE  // the entire event queue should be flushed and reset
} leEventResult;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    uint32_t leEvent_GetCount()

  Summary:
    Returns the number of events listed in the current context

  Description:
    Returns the number of events listed in the current context

  Parameters:
    void
    
  Returns:
    uint32_t
    
  Remarks:
    
*/
uint32_t leEvent_GetCount();

// *****************************************************************************
/* Function:
    leResult leEvent_SetFilter(leEvent_FilterEvent cb)

  Summary:
    Set callback pointer for current context filter event 

  Description:
    Set callback pointer for current context filter event 
  Parameters:
    leEvent_FilterEvent
    
  Returns:
    leResult
  Remarks:
    
*/
LIB_EXPORT leResult leEvent_SetFilter(leEvent_FilterEvent cb); 

// *****************************************************************************
/* Function:
    leResult leEvent_AddEvent(leEvent* evt)

  Summary:
    Add the mentioned event callback to the list of events maintained by the current context

  Description:
    Add the mentioned event callback to the list of events maintained by the current context

  Parameters:
    leEvent*
    
  Returns:
    leResult
    
  Remarks:
    
*/
leResult leEvent_AddEvent(leEvent* evt);

// *****************************************************************************
/* Function:
    leResult leEvent_ClearList()

  Summary:
    Clear the event list maintained by the current context.

  Description:
    Clear the event list maintained by the current context.

  Parameters:
    void
    
  Returns:
    leResult
    
  Remarks:
    
*/
leResult leEvent_ClearList();

// *****************************************************************************
/* Function:
    leResult leEvent_ProcessEvents()

  Summary:
    Processes any pending events

  Description:
    Processes any pending events

  Parameters:
    void
    
  Returns:
    leResult

  Remarks:
    
*/
leResult leEvent_ProcessEvents();

#endif /* LEGATO_EVENT_H */