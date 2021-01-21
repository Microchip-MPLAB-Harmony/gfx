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
    legato_widget_list.h

  Summary:


  Description:
    This module implements list box widget functions.
*******************************************************************************/

/** \file legato_widget_list.h
 * @brief List widget functions and definitions.
 *
 * @details This module implements list box widget functions.
 */

#ifndef LEGATO_LISTWIDGET_H
#define LEGATO_LISTWIDGET_H

#include "gfx/legato/common/legato_common.h"

#if LE_LIST_WIDGET_ENABLED == 1 && LE_SCROLLBAR_WIDGET_ENABLED == 1

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/list/legato_widget_list.h"
#include "gfx/legato/widget/scrollbar/legato_widget_scrollbar.h"


typedef struct leListWidget leListWidget;

// *****************************************************************************
/* Function Pointer:
    leListWidget_SelectedItemChangedEvent

  Summary:
    Selected item changed event function callback type
*/
/**
 * @brief Used to define a item selected changed event callback function.
 * @details .
 */
typedef void (*leListWidget_SelectedItemChangedEvent)(leListWidget*,
                                                      uint32_t idx,
                                                      leBool selected);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leListWidget_SelectionMode

  Summary:
    Defines the list selection modes

  Description:
    Single - a single selection from the list is allowed at any one time
    Multiple - any number of selected items is allowed at any one time
    Contiguous - any number of selected items in a contiguous serious is allowed
                 at any one time

  Remarks:
    None.
*/
/**
 * @brief Defines the selection modes
 * @details Single - a single selection from the list is allowed at any one
 * time Multiple - any number of selected items is allowed at any one time Contiguous -
 * any number of selected items in a contiguous serious is allowed at any one time
 */
typedef enum leListWidget_SelectionMode
{
    LE_LIST_WIDGET_SELECTION_MODE_SINGLE,
    LE_LIST_WIDGET_SELECTION_MODE_MULTIPLE,
    LE_LIST_WIDGET_SELECTION_MODE_CONTIGUOUS
} leListWidget_SelectionMode;

// *****************************************************************************
/* Structure:
    leListItem

  Summary:
    Defines a list item struct

  Description:


  Remarks:
    None.
*/
/**
 * @brief Defines a list item
 * @details
 */
typedef struct leListItem
{
    const leString* string; // list item string
    const leImage* icon; // list item icon
    leBool selected; // list item selected flag
    leRect rowRect; // list item row rectangle
    leBool enabled; //enable or disable the item
} leListItem;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
typedef struct leListWidget leListWidget;

#define LE_LISTWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leListWidget_SelectionMode (*getSelectionMode)(const THIS_TYPE* _this); \
    leResult      (*setSelectionMode)(THIS_TYPE* _this, leListWidget_SelectionMode mode); \
    leBool        (*getAllowEmptySelection)(const THIS_TYPE* _this); \
    leResult      (*setAllowEmptySelection)(THIS_TYPE* _this, leBool allow); \
    leRelativePosition  (*getIconPosition)(const THIS_TYPE* _this); \
    leResult      (*setIconPosition)(THIS_TYPE* _this, leRelativePosition pos); \
    uint32_t      (*getIconMargin)(const THIS_TYPE* _this); \
    leResult      (*setIconMargin)(THIS_TYPE* _this, uint32_t mg); \
    uint32_t      (*getItemCount)(const THIS_TYPE* _this); \
    int32_t       (*appendItem)(THIS_TYPE* _this); \
    int32_t       (*insertItem)(THIS_TYPE* _this, int32_t idx); \
    leResult      (*removeItem)(THIS_TYPE* _this, int32_t idx); \
    leResult      (*removeAllItems)(THIS_TYPE* _this); \
    leBool        (*getItemSelected)(const THIS_TYPE* _this, int32_t idx); \
    leResult      (*setItemSelected)(THIS_TYPE* _this, int32_t idx, leBool selected); \
    leResult      (*toggleItemSelected)(THIS_TYPE* _this, int32_t idx); \
    leResult      (*selectAll)(THIS_TYPE* _this); \
    leResult      (*deselectAll)(THIS_TYPE* _this); \
    int32_t       (*getFirstSelectedItem)(const THIS_TYPE* _this); \
    int32_t       (*getLastSelectedItem)(const THIS_TYPE* _this); \
    uint32_t      (*getSelectionCount)(const THIS_TYPE* _this); \
    leString*     (*getItemString)(const THIS_TYPE* _this, int32_t idx); \
    leResult      (*setItemString)(THIS_TYPE* _this, int32_t idx, const leString* str); \
    leImage*      (*getItemIcon)(const THIS_TYPE* _this, int32_t idx); \
    leResult      (*setItemIcon)(THIS_TYPE* _this, int32_t idx, const leImage* img); \
    leBool        (*getItemEnable)(const THIS_TYPE* _this, int32_t idx); \
    leResult      (*setItemEnable)(THIS_TYPE* _this, int32_t idx, leBool b); \
    leResult      (*setItemVisible)(THIS_TYPE* _this, int32_t idx); \
    leListWidget_SelectedItemChangedEvent (*getSelectedItemChangedEventCallback)(const THIS_TYPE* _this); \
    leResult      (*setSelectedItemChangedEventCallback)(THIS_TYPE* _this, leListWidget_SelectedItemChangedEvent cb); \
    
typedef struct leListWidgetVTable
{
	LE_LISTWIDGET_VTABLE(leListWidget)
} leListWidgetVTable; 

    /**
      * @endcond
      *
      */

// *****************************************************************************
/**
 * @brief This struct represents a list widget
 * @details A list widget is a widget that contains a series of vertical nodes.
 * Each node can have text, an image, or both, and can be selected or not.  The
 * list has a built-in scrollbar.  This allows the list to be larger than the
 * visible area of the widget.
 */
typedef struct leListWidget
{
    leWidget widget; // list base class

    const leListWidgetVTable* fn;

    leListWidget_SelectionMode mode; // list selection mode
    leBool allowEmpty; // indicates if the list must always have at least one
                       // selected item

    leArray items; // list containing the list items

    leRelativePosition iconPos; // icon position for the list icons
    uint32_t iconMargin; // margin for the list icons

    int32_t itemDown; // tracks whether an input event is in process

    leScrollBarWidget* scrollbar; // internal scrollbar for this widget

    leListWidget_SelectedItemChangedEvent cb; // item selected changed event
} leListWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leListWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leListWidget* wgt = leListWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leListWidget* leListWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leListWidget <span class="param">wgt</span>.
 * @code
 * leListWidget* wgt;
 * leListWidget_Constructor(wgt);
 * @endcode
 * @param lst is the widget to initialize
 * @return void.
 */
void leListWidget_Constructor(leListWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget


// *****************************************************************************
/* Virtual Member Function:
    leListWidget_SelectionMode getSelectionMode(const leListWidget* _this)

  Summary:
     Gets the current selection mode

  Description:
     Gets the current selection mode

  Parameters:
    const leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->getSelectionMode(_this);

  Returns:
    leListWidget_SelectionMode -
*/
/**
 * @brief Get current selection mode.
 * @details Gets current selection mode using  <span class="param">wgt</span>.
 * @code
 * leListWidget* _this;
 * leListWidget_SelectionMode mode = _this->fn->getSelectionMode(_this);
 * @endcode
 * @param _this is the widget to query
 * @return selection mode.
 */
virtual leListWidget_SelectionMode getSelectionMode(const leListWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setSelectionMode(leListWidget* _this,
                              leListWidget_SelectionMode mode)

  Summary:
     Sets the current selection mode

  Description:
     Sets the current selection mode

  Parameters:
    leListWidget* _this - The list widget to operate on
    leListWidget_SelectionMode mode -

  Remarks:
    Usage - _this->fn->setSelectionMode(_this, mode);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set current selection mode.
 * @details Sets the current selection mode using <span class="param">wgt</span>.
 * @code
 * leListWidget* _this;
 * leListWidget_SelectionMode mode;
 * leResult res = wgt->fn->setSelectionMode(_this, mode);
 * @endcode
 * @param mode is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setSelectionMode(leListWidget* _this,
                                  leListWidget_SelectionMode mode);


// *****************************************************************************
/* Virtual Member Function:
    leBool getAllowEmptySelection(const leListWidget* _this)

  Summary:
     Gets the allow empty setting value

  Description:
     Gets the allow empty setting value

  Parameters:
    const leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->getAllowEmptySelection(_this);

  Returns:
    leBool - the setting value
*/
/**
 * @brief Get allow empty setting value.
 * @details Gets the allow empty setting value using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * leBool val = _this->fn->getAllowEmptySelection(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getAllowEmptySelection(const leListWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setAllowEmptySelection(leListWidget* _this,
                                    leBool allow)

  Summary:
     Sets the allow empty setting value

  Description:
     Sets the allow empty setting value

  Parameters:
    leListWidget* _this - The list widget to operate on
    leBool allow - the setting value

  Remarks:
    Usage - _this->fn->setAllowEmptySelection(_this, allow);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set current selection mode.
 * @details Sets the current selection mode using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * leBool allow;
 * leResult res = wgt->fn->setAllowEmptySelection(_this, allow);
 * @endcode
 * @param allow is the selection mode
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setAllowEmptySelection(leListWidget* _this,
                                        leBool allow);


// *****************************************************************************
/* Virtual Member Function:
    leRelativePosition getIconPosition(const leListWidget* _this)

  Summary:
     Gets the image icon position

  Description:
     Gets the image icon position

  Parameters:
    const leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->getIconPosition(_this);

  Returns:
    leRelativePosition - the position
*/
/**
 * @brief Get image icon position.
 * @details Gets the image icon position using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * leBool val = _this->fn->getIconPosition(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leRelativePosition getIconPosition(const leListWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setIconPosition(leListWidget* _this,
                             leRelativePosition pos)

  Summary:
     Sets the image icon position

  Description:
     Sets the image icon position

  Parameters:
    leListWidget* _this - The list widget to operate on
    leRelativePosition pos - the position value

  Remarks:
    Usage - _this->fn->setIconPosition(_this, pos);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image icon position.
 * @details Sets the image icon position to <span class="param">pos</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * leRelativePosition pos;
 * leResult res = wgt->fn->setIconPosition(_this, pos);
 * @endcode
 * @param pos is the position
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setIconPosition(leListWidget* _this,
                                 leRelativePosition pos);


// *****************************************************************************
/* Virtual Member Function:
    uint32_t getIconMargin(const leListWidget* _this)

  Summary:
     Gets the icon margin

  Description:
     Gets the icon margin

  Parameters:
    const leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->getIconMargin(_this);

  Returns:
    uint32_t - the margin value
*/
/**
 * @brief Get icon margin.
 * @details Gets the icon margin using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * uint32_t margin = _this->fn->getIconMargin(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual uint32_t getIconMargin(const leListWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setIconMargin(leListWidget* _this,
                           uint32_t mg)

  Summary:
     Sets the icon margin

  Description:
     Sets the icon margin

  Parameters:
    leListWidget* _this - The list widget to operate on
    uint32_t mg - the margin value

  Remarks:
    Usage - _this->fn->setIconMargin(_this, mg);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set icon margin.
 * @details Sets the icon margin to <span class="param">mg</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * uint32_t mg;
 * leResult res = wgt->fn->setIconMargin(_this, mg);
 * @endcode
 * @param mg is the position
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setIconMargin(leListWidget* _this,
                               uint32_t mg);


// *****************************************************************************
/* Virtual Member Function:
    uint32_t getItemCount(const leListWidget* _this)

  Summary:
     Gets the item count

  Description:
     Gets the item count

  Parameters:
    const leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->getItemCount(_this);

  Returns:
    uint32_t - the item count
*/
/**
 * @brief Get item count.
 * @details Gets the item count using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * uint32_t cnt = _this->fn->getItemCount(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual uint32_t getItemCount(const leListWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    int32_t appendItem(leListWidget* _this)

  Summary:
     Appends an item to the list

  Description:
     Appends an item to the list

  Parameters:
    leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->appendItem(_this);

  Returns:
    int32_t - the index of the new item
*/
/**
 * @brief Append item to the list.
 * @details Appends an item to the list using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * uint32_t cnt = _this->fn->appendItem(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual int32_t appendItem(leListWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    int32_t insertItem(leListWidget* _this,
                       int32_t idx)

  Summary:
     Inserts an item into the list

  Description:
     Inserts an item into the list

  Parameters:
    leListWidget* _this - The list widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->insertItem(_this, idx);

  Returns:
    int32_t - the index of the new item
*/
/**
 * @brief Insert item into the list.
 * @details Inserts an item into the list using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * uint32_t cnt = _this->fn->insertItem(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual int32_t insertItem(leListWidget* _this,
                           int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult removeItem(leListWidget* _this,
                        int32_t idx)

  Summary:
     Removes an item from the list

  Description:
     Removes an item from the list

  Parameters:
    leListWidget* _this - The list widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->removeItem(_this, idx);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Remove item at index from the list.
 * @details Removes the item at <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * uint32_t mg;
 * leResult res = _this->fn->removeItem(_this, idx);
 * @endcode
 * @param idx is the index
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult removeItem(leListWidget* _this,
                            int32_t idx);


// *****************************************************************************
/* Virtual Member Function:
    leResult removeAllItems(leListWidget* _this)

  Summary:
     Removes all items from the list

  Description:
     Removes all items from the list

  Parameters:
    leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->removeAllItems(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Remove all items from the list.
 * @details Removes all items from the list
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * leResult res = _this->fn->removeAllItems(_this);
 * @endcode
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult removeAllItems(leListWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leBool getItemSelected(const leListWidget* _this,
                           int32_t idx)

  Summary:
     Gets the currently selected item

  Description:
     Gets the currently selected item

  Parameters:
    const leListWidget* _this - The list widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getItemSelected(_this, idx);

  Returns:
    leBool - the selected state
*/
/**
 * @brief Get currently selected item.
 * @details Gets the currently selected item using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * int32_t idx;
 * leBool selected = _this->fn->getItemSelected(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getItemSelected(const leListWidget* _this,
                               int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setItemSelected(leListWidget* _this,
                             int32_t idx,
                             leBool selected)

  Summary:
     Sets the currently selected item

  Description:
     Sets the currently selected item

  Parameters:
    leListWidget* _this - The list widget to operate on
    int32_t idx - the index
    leBool selected - the selected state

  Remarks:
    Usage - _this->fn->setItemSelected(_this, idx, selected);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set currently selected item.
 * @details Sets the item at <span class="param">idx</span> to
 * <span class="param">selected</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * uint32_t idx;
 * leResult res = wgt->fn->setItemSelected(_this, idx, selected);
 * @endcode
 * @param mg is the position
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setItemSelected(leListWidget* _this,
                                 int32_t idx,
                                 leBool selected);



// *****************************************************************************
/* Virtual Member Function:
    leResult toggleItemSelected(leListWidget* _this,
                                int32_t idx)

  Summary:
     Toggles an item selection

  Description:
     Toggles an item selection

  Parameters:
    leListWidget* _this - The list widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->toggleItemSelected(_this, idx);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Toggle item selection.
 * @details Toggles an item selection using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * int32_t idx;
 * leResult res = _this->fn->toggleItemSelected(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leResult toggleItemSelected(leListWidget* _this,
                                    int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult selectAll(leListWidget* _this)

  Summary:
     Selects all items

  Description:
     Selects all items

  Parameters:
    leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->selectAll(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Select  all items.
 * @details Selects all items
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * uint32_t idx;
 * leResult res = _this->fn->selectAll(_this);
 * @endcode
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult selectAll(leListWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult deselectAll(leListWidget* _this)

  Summary:
     Deselects all items

  Description:
     Deselects all items

  Parameters:
    leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->deselectAll(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Deselect  all items.
 * @details Deselects all items
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * leResult res = _this->fn->deselectAll(_this);
 * @endcode
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult deselectAll(leListWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    int32_t getFirstSelectedItem(const leListWidget* _this)

  Summary:
     Gets the first selected item

  Description:
     Gets the first selected item

  Parameters:
    const leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->getFirstSelectedItem(_this);

  Returns:
    int32_t - the selected item index
*/
/**
 * @brief Get first selected item.
 * @details Gets the first selected item using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * int32_t idx;
 * int32_t selected = _this->fn->getFirstSelectedItem(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual int32_t getFirstSelectedItem(const leListWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    int32_t getLastSelectedItem(const leListWidget* _this)

  Summary:
     Gets the last selected item

  Description:
     Gets the last selected item

  Parameters:
    const leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->getLastSelectedItem(_this);

  Returns:
    int32_t - the selected item index
*/
/**
 * @brief Get last selected item.
 * @details Gets the last selected item using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * int32_t selected = _this->fn->getLastSelectedItem(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual int32_t getLastSelectedItem(const leListWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getSelectionCount(const leListWidget* _this)

  Summary:
     Gets the number of selected items

  Description:
     Gets the number of selected items

  Parameters:
    const leListWidget* _this - The list widget to operate on

  Remarks:
    Usage - _this->fn->getSelectionCount(_this);

  Returns:
    uint32_t - the number of selected items
*/
/**
 * @brief Get last selected item.
 * @details Gets the last selected item using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * int32_t cnt = _this->fn->getSelectionCount(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the number of selected items
 */
virtual uint32_t getSelectionCount(const leListWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leString* getItemString(const leListWidget* _this,
                            int32_t idx)

  Summary:
     Gets the string pointer for an item

  Description:
     Gets the string pointer for an item

  Parameters:
    const leListWidget* _this - The list widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getItemString(_this, idx);

  Returns:
    leString* - the string pointer
*/
/**
 * @brief Get last selected item.
 * @details Gets the last selected item using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * leString* str = _this->fn->getItemString(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer
 */
virtual leString* getItemString(const leListWidget* _this,
                                int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setItemString(leListWidget* _this,
                           int32_t idx,
                           const leString* str)

  Summary:
     Sets the string pointer for an item

  Description:
     Sets the string pointer for an item

  Parameters:
    leListWidget* _this - The list widget to operate on
    int32_t idx - the index
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setItemString(_this, idx, str);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set string pointer for an item.
 * @details Sets string pointer <span class="param">str</span> at
 *  <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * uint32_t idx;
 * const leString* str;
 * leResult res = wgt->fn->setItemString(_this, idx, str);
 * @endcode
 * @param idx is the position
 * @param str is the string.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setItemString(leListWidget* _this,
                               int32_t idx,
                               const leString* str);

// *****************************************************************************
/* Virtual Member Function:
    leImage* getItemIcon(const leListWidget* _this,
                         int32_t idx)

  Summary:
     Gets the image pointer for an item

  Description:
     Gets the image pointer for an item

  Parameters:
    const leListWidget* _this - The list widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getItemIcon(_this, idx);

  Returns:
    leImage* - the image pointer
*/
/**
 * @brief Get image pointer for an item.
 * @details Gets the image pointer for an item using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * int32_t idx;
 * leString* str = _this->fn->getItemIcon(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the string pointer
 */
virtual leImage* getItemIcon(const leListWidget* _this,
                             int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setItemIcon(leListWidget* _this,
                         int32_t idx,
                         const leImage* img)

  Summary:
     Sets the image pointer for an item

  Description:
     Sets the image pointer for an item

  Parameters:
    leListWidget* _this - The list widget to operate on
    int32_t idx - the index
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setItemIcon(_this, idx, img);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image pointer for an item.
 * @details Sets image pointer for item at <span class="param">idx</span> to
 *  <span class="param">img</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * uint32_t idx;
 * const leImage* img;
 * leResult res = wgt->fn->setItemIcon(_this, idx, img);
 * @endcode
 * @param idx is the position
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setItemIcon(leListWidget* _this,
                             int32_t idx,
                             const leImage* img);

// *****************************************************************************
/* Virtual Member Function:
    leBool getItemEnable(const leListWidget* _this,
                         int32_t idx)

  Summary:
     Gets an item enable state

  Description:
     Gets an item enable state

  Parameters:
    const leListWidget* _this - The list widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getItemEnable(_this, idx);

  Returns:
    leBool - the enable state
*/
/**
 * @brief Get item enable state.
 * @details Gets the item enable state using <span class="param">_this</span>.
 * @code
 * const leListWidget* _this;
 * int32_t idx;
 * leBool enable = _this->fn->getItemEnable(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getItemEnable(const leListWidget* _this,
                            int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setItemEnable(leListWidget* _this,
                           int32_t idx,
                           leBool b)

  Summary:
     Sets an item enable state

  Description:
     Sets an item enable state

  Parameters:
    leListWidget* _this - The list widget to operate on
    int32_t idx - the index
    leBool b - the enable state

  Remarks:
    Usage - _this->fn->setItemEnable(_this, idx, b);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set item enable state.
 * @details Sets item enable state for item at <span class="param">idx</span> to
 *  <span class="param">b</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * uint32_t idx;
 * leBool b;
 * leResult res = wgt->fn->setItemEnable(_this, idx, b);
 * @endcode
 * @param idx is the position
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setItemEnable(leListWidget* _this,
                               int32_t idx,
                               leBool b);



/**
 * @brief Get selected item changed event callback pointer.
 * @details Gets the selected item changed event callback pointer using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * leListWidget_SelectedItemChangedEvent cb = _this->fn->getSelectedItemChangedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the callback pointer.
 */
virtual leListWidget_SelectedItemChangedEvent getSelectedItemChangedEventCallback
                                                    (const leListWidget* _this);

/**
 * @brief Set selected item changed event callback pointer.
 * @details Sets the selected item changed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leListWidget* _this;
 * leListWidget_SelectedItemChangedEvent cb;
 * leResult res = _this->fn->setSelectedItemChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setSelectedItemChangedEventCallback(leListWidget* _this,
                                                     leListWidget_SelectedItemChangedEvent cb);

#undef THIS_TYPE
#endif


#endif // LE_LIST_WIDGET_ENABLED && LE_SCROLLBAR_WIDGET_ENABLED
#endif /* LEGATO_LISTWIDGET_H */
