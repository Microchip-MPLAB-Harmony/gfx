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
    legato_list.h

  Summary:
    A linked list implementation for the Legato user interface library

  Description:
    This is a linked list implementation that is used internally by the 
    Legato user interface library.  
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_LIST_H
#define LEGATO_LIST_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

// *****************************************************************************
/* Structure:
    leListNode

  Summary:
    Linked list node definition

  Remarks:
    None.
*/
typedef struct leListNode
{
    struct leListNode* next;
    void*  val;
} leListNode;


// *****************************************************************************
/* Structure:
    leList

  Summary:
    Linked list definition

  Remarks:
    None.
*/
typedef struct leList
{
    leListNode* head;
    leListNode* tail;
    size_t size;
} leList;

// *****************************************************************************
/* Function:
    int32_t leList_Create(leList* list)

   Summary:
    Initializes a new linked list

   Parameters:
    leList* list - pointer to the list to initilaize

  Returns:
    int32_t - 0 if success, -1 if failure

  Remarks:        
*/
int32_t leList_Create(leList* list);

// *****************************************************************************
/* Function:
    int32_t leList_Assign(leList* list, size_t idx, void* val)

   Summary:
    Assignes a new pointer to an index in the list

   Parameters:
    leList* list - pointer to the list to modify
    size_t idx - the index to modify
    void* val - the new value of the node
    
  Returns:
    int32_t - 0 if success, -1 if failure
*/
int32_t leList_Assign(leList* list, size_t idx, void* val);

// *****************************************************************************
/* Function:
    int32_t leList_PushFront(leList* list, void* val)

   Summary:
    Pushes a new node onto the front of the list

   Parameters:
    leList* list - pointer to the list to modify
    void* val - the new value of the node
    
  Returns:
    int32_t - 0 if success, -1 if failure
*/
int32_t leList_PushFront(leList* list, void*);

// *****************************************************************************
/* Function:
    void leList_PopFront(leList* list)

   Summary:
    Removes the first value from the list

   Parameters:
    leList* list - pointer to the list to modify
*/
void leList_PopFront(leList* list);

// *****************************************************************************
/* Function:
    int32_t leList_PushBack(leList* list, void* val)

   Summary:
    Pushes a new node onto the back of the list

   Parameters:
    leList* list - pointer to the list to modify
    void* val - the new value of the node
    
  Returns:
    int32_t - 0 if success, -1 if failure
*/
int32_t leList_PushBack(leList* list, void* val);

// *****************************************************************************
/* Function:
    void leList_PopBack(leList* list)

   Summary:
    Removes the last value from the list

   Parameters:
    leList* list - pointer to the list to modify
*/
int32_t leList_PopBack(leList* list);

// *****************************************************************************
/* Function:
    void* leList_Get(leList* list, uint32_t idx)

   Summary:
    Retrieves a value from the list

   Parameters:
    leList* list - pointer to the list to reference
    uint32_t idx - the index of the value to retrieve
   
   Returns:
     void* - the retrieved value
*/
void* leList_Get(const leList* list, uint32_t idx);

// *****************************************************************************
/* Function:
    int32_t leList_Find(leList* list, void* val)

   Summary:
    Retrieves the index of a value from the list

   Parameters:
    leList* list - pointer to the list to reference
    void* val - the value to search for
   
   Returns:
    int32_t - the index of the value searched for
*/
int32_t leList_Find(const leList* list, void* val);

// *****************************************************************************
/* Function:
    int32_t leList_InsertAt(leList* list,
                            void* val,
                            uint32_t idx)

   Summary:
    Inserts an item into a list at a given index.  All existing from index
    are shifted right one place.

   Parameters:
    leList* list - pointer to the list to modify
    void* val - the value to insert
    uint32_t idx - the position to insert the value
   
   Returns:
    int32_t - 0 if success, -1 if failure
*/
int32_t leList_InsertAt(leList* list,
                        void* val,
                        uint32_t idx);

// *****************************************************************************
/* Function:
    int32_t leList_Remove(leList* list, void*)

   Summary:
    Removes an item from the list

   Parameters:
    leList* list - pointer to the list to modify
    void* val - the value to remove
   
   Returns:
    int32_t - 0 if success, -1 if failure
*/
int32_t leList_Remove(leList* list, void*);

// *****************************************************************************
/* Function:
    int32_t leList_Remove(leList* list, uint32_t idx)

   Summary:
    Removes an item from the list at an index

   Parameters:
    leList* list - pointer to the list to modify
    uint32_t idx - the index of the value to remove
   
   Returns:
    int32_t - 0 if success, -1 if failure
*/
int32_t leList_RemoveAt(leList* list, uint32_t idx);

// *****************************************************************************
/* Function:
    int32_t leList_Copy(leList* l, leList* r)

   Summary:
    Creates a duplicate of an existing list

   Parameters:
    leList* l - the source list
    leList* r - the result list
   
   Returns:
    int32_t - 0 if success, -1 if failure
*/
int32_t leList_Copy(leList* l, leList* r);

// *****************************************************************************
/* Function:
    void leList_Clear(leList* list)

   Summary:
    Removes all nodes from a given list

   Parameters:
    leList* list - the list to modify
       
   Returns:
    void
*/
void leList_Clear(leList* list);

// *****************************************************************************
/* Function:
    void leList_Destroy(leList* list)

   Summary:
    Removes all nodes from a given list and frees the data of each node

   Parameters:
    leList* list - the list to modify
       
   Returns:
    void
*/
void leList_Destroy(leList* list);

#endif /* LEGATO_LIST_H */
