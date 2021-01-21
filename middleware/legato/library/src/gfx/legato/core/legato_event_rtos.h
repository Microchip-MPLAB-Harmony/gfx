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

/*******************************************************************************
 Module for Microchip Graphics Library - Aria User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    libaria_event_rtos.h

  Summary:
    Defines events and APIs that can be safely used for communicating with the UI
    library in an RTOS environment. All RTOS tasks should use these APIs for thread-safe
    operation.

*******************************************************************************/

/** \file legato_event_rtos.h
 * @brief Legato RTOS event definitions.
 *
 * @details Defines events and APIs that can be safely used for
 * communicating with the UI library in an RTOS environment. All RTOS
 * tasks should use these APIs for thread-safe operation.
 */


#ifndef LIBARIA_EVENT_RTOS_H
#define LIBARIA_EVENT_RTOS_H

#include "gfx/libaria/common/libaria_common.h"
#include "gfx/libaria/inc/libaria_list.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* internal use only */
/**
  * @cond INTERNAL
typedef enum
{
    LA_EXT_EVENT_NONE,
    LA_EXT_EVENT_SCREEN_UPDATE,
} laExternalEventID;

typedef struct laExternalEvent_t
{
    laExternalEventID id;
} laExternalEvent;

laResult laEvent_RTOS_Initialize(void);
laResult laEvent_RTOS_DeInitialize(void);
uint32_t laEvent_GetCount_RTOS();
laResult laEvent_SetFilter_RTOS(laEvent_FilterEvent cb);
laResult laEvent_ClearList_RTOS();
laResult laEvent_AddEvent_RTOS(laEvent* evt);
laResult laEvent_SendEvent_Ext_RTOS(laEvent* evt);
laResult laEvent_ProcessEvents_RTOS();

* @endcond
*
*/
#ifdef __cplusplus
    }
#endif

#endif //LIBARIA_EVENT_RTOS_H
