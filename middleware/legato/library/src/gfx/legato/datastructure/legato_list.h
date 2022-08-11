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
    legato_list.h

  Summary:
    A linked list implementation for the Legato user interface library

  Description:
    This is a linked list implementation that is used internally by the 
    Legato user interface library.  
*******************************************************************************/

/** \file legato_list.h
 * @brief A linked list implementation.
 *
 * @details This is a linked list implementation that is used internally
 * by the Legato user interface library.
 */

#ifndef LEGATO_LIST_H
#define LEGATO_LIST_H

#include "gfx/legato/common/legato_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
/* Structure:
    leListNode

  Summary:
    Linked list node definition

  Remarks:
    None.
*/

/**
 * @brief This struct represents a list node .
 * @details List node is used to hold a list of items.
 */
typedef struct leListNode
{
    struct leListNode* next; /**< The next list node. */
    void* val; /**< The value at a node. */
} leListNode;


// *****************************************************************************
/* Structure:
    leList

  Summary:
    Linked list definition

  Remarks:
    None.
*/

/**
 * @brief This struct represents a list.
 * @details List is used use to hold a size limit of list nodes.
 */
typedef struct leList
{
    leListNode* head; /**< The head list node. */
    leListNode* tail; /**< The tail list node. */
    size_t size; /**< The size of the list. */
} leList;

// *****************************************************************************
/**
 * @brief Create a new list.
 * @details Creates a new list using <span class="param">list</span>.
 * @code
 * leArray* list;
 * int32_t res = leList_Create(list)
 * @endcode
 * @param Pointer to the list to create.
 * @return 0 if success, -1 if failure.
 */
int32_t leList_Create(leList* list);

// *****************************************************************************
/**
 * @brief Assignes a new pointer.
 * @details Assigns <span class="param">val</span>
 * at the specified <span class="param">index</span> for the
 * specified <span class="param">list</span>.
 * @code
 * leList* list;
 * size_t idx;
 * void* val;
 * int32_t res = leList_Assign(list, idx, val);
 * @endcode
 * @param list is the array to modify.
 * @param idx is the location to update.
 * @param val is the value to assign.
 * @return 0 if success, -1 if failure.
 */
int32_t leList_Assign(leList* list, size_t idx, void* val);

// *****************************************************************************
/**
 * @brief Push value on front
 * @details Pushes <span class="param">val</span> onto the
 * front of the specified <span class="param">list</span>.
 * This function shuffles all other values backward one index.
 * @code
 * leList* list;
 * void* val;
 * int32_t res = leList_Create(list, idx, val);
 * @endcode
 * @param list is the array to modify.
 * @param val is the value to push.
 * @return 0 if success, -1 if failure.
 */
int32_t leList_PushFront(leList* list, void*);

// *****************************************************************************
/**
 * @brief Pop value from front
 * @details Removes the first value from the front
 * of <span class="param">list</span>.
 * This function shuffles all other values forward one index.
 * @code
 * leList* list;
 * leList_PopFront(list);
 * @endcode
 * @param list is the array to modify.
 * @return void.
 */
void leList_PopFront(leList* list);

// *****************************************************************************
/**
 * @brief Push value on back.
 * @details Pushes <span class="param">val</span> onto the
 * back of <span class="param">list</span>.
 * @code
 * leList* list;
 * void* val;
 * int32_t res = leList_PushBack(list, val);
 * @endcode
 * @param list is the array to modify.
 * @param val is the value to push.
 * @return 0 if success, -1 if failure.
 */
int32_t leList_PushBack(leList* list, void* val);

// *****************************************************************************
/**
 * @brief Pop value from back
 * @details Pops a value from the back
 * <span class="param">list</span>. This is the last value from the list.
 * of the array pointed to by <span class="param">list</span>.
 * @code
 * leList* list;
 * int32_t val = leList_PopBack(list);
 * @endcode
 * @param list is the array to modify.
 * @return last value.
 */
int32_t leList_PopBack(leList* list);

// *****************************************************************************
/**
 * @brief Get a value from the list.
 * @details Gets <span class="param">val</span> from
 * <span class="param">list</span> at
 * the specified <span class="param">idx</span>.
 * All existing values from index are shifted right one place.
 * @code
 * leList* list;
 * void* value = leList_Get(list);
 * @endcode
 * @param pointer to the list to reference.
 * @param the index of the value to retrieve.
 * @return pointer to the retrieved value.
 */
void* leList_Get(const leList* list, uint32_t idx);

// *****************************************************************************
/**
 * @brief Find index of a value.
 * @details Finds the index of <span class="param">val</span> from
 * <span class="param">list</span>.
 * All existing values from index are shifted right one place.
 * @code
 * leList* list;
 * void* val;
 * int32_t index = leList_Get(list, val);
 * @endcode
 * @param pointer to the list to reference.
 * @param the value to search for.
 * @return the index of the value searched for.
 */
int32_t leList_Find(const leList* list, void* val);

// *****************************************************************************
/**
 * @brief Insert value at index
 * @details Inserts <span class="param">val</span> into
 * <span class="param">list</span> at
 * the specified <span class="param">idx</span>.
 * All existing values from index are shifted right one place.
 * @code
 * leList* list;
 * void* val;
 * uint32_t idx;
 * int32_t res = leList_InsertAt(list, val, idx);
 * @endcode
 * @param list is the array to modify.
 * @param val is value to insert.
 * @param idx is the location at which to insert.
 * @return 0 if success, -1 if failure.
 */
int32_t leList_InsertAt(leList* list,
                        void* val,
                        uint32_t idx);

// *****************************************************************************
/**
 * @brief Remove item
 * @details Removes the first instance of <span class="param">val</span>
 * from <span class="param">list</span>.
 * The function shuffles all values left to fill the gap.
 * @code
 * leList* list;
 * void* val;
 * int32_t index = leList_Remove(list, val);
 * @endcode
 * @param list is the array to modify.
 * @param val is the value removed.
 * @return 0 if success, -1 if failure.
 */
int32_t leList_Remove(leList* list, void* val);

// *****************************************************************************
/**
 * @brief Remove item at index
 * @details Removes an item from
 * <span class="param">list</span> at
 * the specified <span class="param">idx</span>.
 * @code
 * leList* list;
 * uint32_t idx;
 * int32_t res = leList_Remove(list, idx);
 * @endcode
 * @param list is the array to modify.
 * @param idx is the location at which to remove.
 * @return 0 if success, -1 if failure.
 */
int32_t leList_RemoveAt(leList* list, uint32_t idx);

// *****************************************************************************
/**
 * @brief Copy list
 * @details Copies the contents of <span class="param">l</span>
 * to the array pointed to by <span class="param">r</span>.
 * @code
 * leList* l;
 * leList* r;
 * int32_t index = leList_Copy(l, r);
 * @endcode
 * @param l the source list.
 * @param r the result list.
 * @return 0 if success, -1 if failure.
 */
int32_t leList_Copy(leList* l, leList* r);

// *****************************************************************************
/**
 * @brief Clear array.
 * @details Removes all values from <span class="param">list</span>.
 * The array capacity remains the same.
 * @code
 * leList* list;
 * leList_Clear(list);
 * @endcode
 * @param list the list to modify.
 * @return void.
 */
void leList_Clear(leList* list);

// *****************************************************************************
/**
 * @brief Remove array.
 * @details Removes all values from <span class="param">list</span>.
 * The function frees the data of each node.
 * @code
 * leList* list;
 * leList_Destroy(list);
 * @endcode
 * @param list the list to modify.
 * @return void.
 */
void leList_Destroy(leList* list);

#ifdef __cplusplus
}
#endif

#endif /* LEGATO_LIST_H */
