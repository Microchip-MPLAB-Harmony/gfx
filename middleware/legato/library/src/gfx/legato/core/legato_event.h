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

/** \file legato_event.h
 * @brief Legato event definitions.
 *
 * @details Defines events that are used in the UI library.  Events
 * are created and stored for later processing during a library context's update loop.
 */

#ifndef LEGATO_EVENT_H
#define LEGATO_EVENT_H

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/datastructure/legato_list.h"

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

/**
 * @brief This enum represents valid event IDs.
 * @details Event IDs are used for internal and widget events are as follows:
 */
typedef enum leEventID
{
    // internal events
    LE_EVENT_NONE, /**< No event found. */
    LE_EVENT_TOUCH_DOWN, /**< Touch down event. */
    LE_EVENT_TOUCH_UP, /**< Touch up event. */
    LE_EVENT_TOUCH_MOVE, /**< Touch move event. */
    LE_EVENT_LANGUAGE_CHANGED, /**< Language changed. */

    // widget events
    LE_WIDGET_EVENT_PAINT = 100, /**< Paint event. */
    LE_WIDGET_EVENT_MOVED, /**< Moved event. */
    LE_WIDGET_EVENT_RESIZED, /**< Resized event. */
    LE_WIDGET_EVENT_FOCUS_GAINED, /**< Focus gained event. */
    LE_WIDGET_EVENT_FOCUS_LOST, /**< Focus lost event. */
} leEventID;


// *****************************************************************************
/**
 * @brief This enum presents a event.
 * An event contains valid event ids.
 */
typedef struct leEvent
{
    leEventID id; /**< Event id#id. */
} leEvent;

// *****************************************************************************
/**
 * @brief FilterEvent function pointer
 *
 * @details Function pointer to define an event filter.
 * Event filters allow a receiver to discard undesirable events
 */
typedef leBool (* leEvent_FilterEvent)(leEvent*);

// *****************************************************************************

/**
 * @brief This struct represents an event state.
 * @details Structure to manage the event lists, state and call back pointers.
 *
 */
typedef struct leEventState
{
#ifndef _WIN32
#ifdef LEGATO_USE_OSAL
    OSAL_SEM_HANDLE_TYPE eventCountSem;
    OSAL_MUTEX_HANDLE_TYPE eventLock;
#endif
#endif
    leList events; /**< Some documentation for the member BoxStruct#a. */
    leEvent_FilterEvent filter; /**< Some documentation for the member BoxStruct#a. */
} leEventState;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
leEventState* _leGetEventState(void);
leResult leEvent_Init(void);
void leEvent_Shutdown(void);
/**
  * @endcond
  *
  */

// *****************************************************************************
/* Enumeration:
    leEventResult

  Summary:
    Defines what happened when processing an event
*/

/**
 * @brief This enum represents an event result.
 * @details Event result is used to define the possible internal event states.
 */
typedef enum leEventResult
{
    // internal events
    LE_EVENT_HANDLED,     /**< the event was handled. */
    LE_EVENT_DEFERRED,    /**< the event needs to wait. */
    LE_EVENT_RESET_QUEUE  /**< the entire event queue should be flushed and reset. */
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

/**
 * @brief Get Event Count.
 * @details Returns the number of events listed in the current context.
 * @code
 * uint32_t nbr = leEvent_GetCount();
 * @endcode
 * @return number of events.
 */
uint32_t leEvent_GetCount(void);

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

/**
 * @brief Set filter event callback.
 * @details Set filter event callback to <span class="param">cb</span>.
 * @code
 * leEvent_FilterEvent cb;
 * leResult result = leEvent_SetFilter(cb);
 * @param cb the event callback
 * @endcode
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leEvent_SetFilter(leEvent_FilterEvent cb);

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

/**
 * @brief Add event.
 * @details Adds <span class="param">event</span> to the list of events
 * maintained by the current context.
 * @code
 * leResult result = leEvent_AddEvent(evt);
 * @endcode
 * @param void.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
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

/**
 * @brief Clear event list.
 * @details Clears the event list maintained by the current context.
 * @code
 * leResult result = leEvent_ClearList();
 * @endcode
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leEvent_ClearList(void);

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

/**
 * @brief Process events.
 * @details Processes any pending events.
 * @code
 * leResult result = leEvent_ProcessEvents();
 * @endcode
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult leEvent_ProcessEvents(void);

#ifdef __cplusplus
}
#endif

#endif /* LEGATO_EVENT_H */
