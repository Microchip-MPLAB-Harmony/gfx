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
    legato_widget_keypad.h

  Summary:


  Description:
    This module implements keypad widget functions.
*******************************************************************************/

/** \file legato_widget_keypad.h
 * @brief Keypad widget functions and definitions.
 *
 * @details This module implements keypad widget functions.
 */

#ifndef LEGATO_KEYPAD_H
#define LEGATO_KEYPAD_H

#include "gfx/legato/common/legato_common.h"

#if LE_KEYPAD_WIDGET_ENABLED == 1 && LE_BUTTON_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/widget/legato_editwidget.h"
#include "gfx/legato/string/legato_string.h"

typedef struct leKeyPadWidget leKeyPadWidget;
typedef struct leButtonWidget leButtonWidget;

// *****************************************************************************
/* Function Pointer:
    leKeyPadWidget_KeyClickEvent

  Summary:
    Key click event function callback type
*/
typedef void (* leKeyPadWidget_KeyClickEvent)(leKeyPadWidget*,
                                              leButtonWidget*,
                                              uint32_t,
                                              uint32_t);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Structure:
    leKeyPadCellAction

  Summary:
    Defines an assigned action to a key pad cell

  Description:


  Remarks:
    None.
*/
typedef enum leKeyPadCellAction
{
    LE_KEYPAD_CELL_ACTION_NONE,
    LE_KEYPAD_CELL_ACTION_APPEND,
    LE_KEYPAD_CELL_ACTION_SET,
    LE_KEYPAD_CELL_ACTION_BACKSPACE,
    LE_KEYPAD_CELL_ACTION_CLEAR,
    LE_KEYPAD_CELL_ACTION_ACCEPT
} leKeyPadCellAction;

// *****************************************************************************
/* Structure:
    leKeyPadActionTrigger

  Summary:
    Defines the trigger for keypad action and events

  Description:


  Remarks:
    None.
*/
typedef enum leKeyPadActionTrigger
{
    LE_KEYPAD_TRIGGER_KEYRELEASED,
    LE_KEYPAD_TRIGGER_KEYPRESSED,
} leKeyPadActionTrigger;

// *****************************************************************************
/* Structure:
    leKeyPadCell

  Summary:
    Defines a key pad cell struct

  Description:
    A key pad is made up of an array of key pad cells.  Each cell is individually
    an leButtonWidget, an action, a value, and a few other options.

  Remarks:
    None.
*/
typedef struct leKeyPadCell
{
    leBool enabled; // indicates if the cell should be drawn
    leButtonWidget* button; // the button that handles the cell input events
    // and rendering
    leKeyPadCellAction action; // the action that occurs when the cell is
    // activated
    const leString* value; // the value that is passed to the edit event system
} leKeyPadCell;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
#define LE_KEYPADWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leResult              (*setCellArraySize)(THIS_TYPE* _this, uint32_t rows, uint32_t cols); \
    leKeyPadActionTrigger (*getKeyPadActionTrigger)(const THIS_TYPE* _this); \
    leResult              (*setKeyPadActionTrigger)(THIS_TYPE* _this, leKeyPadActionTrigger trg); \
    leKeyPadWidget_KeyClickEvent (*getKeyClickEventCallback)(const THIS_TYPE* _this); \
    leResult              (*setKeyClickEventCallback)(THIS_TYPE* _this, leKeyPadWidget_KeyClickEvent cb); \
    leResult              (*setKeyVisible)(THIS_TYPE* _this, uint32_t row, uint32_t col, leBool b); \
    leKeyPadCellAction    (*getKeyAction)(const THIS_TYPE* _this, uint32_t row, uint32_t col); \
    leResult              (*setKeyAction)(THIS_TYPE* _this, uint32_t row, uint32_t col, leKeyPadCellAction action); \
    leString*             (*getKeyValue)(const THIS_TYPE* _this, uint32_t row, uint32_t col); \
    leResult              (*setKeyValue)(THIS_TYPE* _this, uint32_t row, uint32_t col, const leString* val); \
    leButtonWidget*       (*getCellButton)(const THIS_TYPE* _this, uint32_t row, uint32_t col); \

typedef struct leKeyPadWidgetVTable
{
    LE_KEYPADWIDGET_VTABLE(leKeyPadWidget)
} leKeyPadWidgetVTable;

/**
  * @endcond
  *
  */

// *****************************************************************************
/**
 * @brief This struct represents a key pad widget
 * @details A key pad is a widget that is comprised of an array of
 * leButtonWidgets. This widget serves to issue edit events based on
 * application or input interaction.  Receptor edit widgets can then
 * receive these edit events and react accordingly.
 */
typedef struct leKeyPadWidget
{
    leWidget widget; // widget base class

    const leKeyPadWidgetVTable* fn;

    uint32_t rows; // number of button rows
    uint32_t cols; // number of button columns

    leKeyPadActionTrigger trigger; //trigger for action and events

    leKeyPadCell** cells; // key cell array

    leKeyPadWidget_KeyClickEvent clickEvt; // key click callback event
} leKeyPadWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leKeyPadWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leKeyPadWidget* wgt = leKeyPadWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leKeyPadWidget* leKeyPadWidget_New(uint32_t rows,
                                   uint32_t cols);

/**
 * @brief Initialize widget.
 * @details Initializes the leKeyPadWidget <span class="param">wgt</span>.
 * @code
 * leKeyPadWidget* wgt;
 * uint32_t rows;
 * uint32_t cols;
 * leKeyPadWidget_Constructor(wgt, rows, cols);
 * @endcode
 * @param wgt is the widget to initialize.
 * @param rows is the number of rows.
 * @param cols is the number of columns.
 * @return void.
 */
void leKeyPadWidget_Constructor(leKeyPadWidget* wgt,
                                uint32_t rows,
                                uint32_t cols);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/* Virtual Member Function:
    leKeyPadActionTrigger getKeyPadActionTrigger(const leKeyPadWidget* _this)

  Summary:
     Gets the keypad trigger action

  Description:
     Gets the keypad trigger action

  Parameters:
    const leKeyPadWidget* _this - The keypad widget to operate on

  Remarks:
    Usage - _this->fn->getKeyPadActionTrigger(_this);

  Returns:
    leKeyPadActionTrigger - the trigger action
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyPadActionTrigger(leKeyPadWidget* _this,
                                    leKeyPadActionTrigger trg)

  Summary:
     Sets the keypad trigger action

  Description:
     Sets the keypad trigger action

  Parameters:
    leKeyPadWidget* _this - The keypad widget to operate on
    leKeyPadActionTrigger trg - the trigger action

  Remarks:
    Usage - _this->fn->setKeyPadActionTrigger(_this, trg);

  Returns:
    leResult - the result of the operation
*/

/**
 * @brief Get key click event callback pointer.
 * @details Gets the key click event callback pointer using <span class="param">_this</span>.
 * @code
 * leKeyPadWidget* _this;
 * leKeyPadWidget_KeyClickEvent cb = _this->fn->getKeyClickEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the callback pointer.
 */
virtual leKeyPadWidget_KeyClickEvent getKeyClickEventCallback(const leKeyPadWidget* _this);

/**
 * @brief Set key click event callback pointer.
 * @details Sets the key click event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leKeyPadWidget* _this;
 * leKeyPadWidget_KeyClickEvent cb;
 * leResult res = _this->fn->setKeyClickEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setKeyClickEventCallback(leKeyPadWidget* _this,
                                          leKeyPadWidget_KeyClickEvent cb);

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyClickEventCallback(leKeyPadWidget* _this,
                                      leKeyPadWidget_KeyClickEvent cb)

  Summary:
     Sets the key click event callback

  Description:
     Sets the key click event callback

  Parameters:
    leKeyPadWidget* _this - The keypad widget to operate on
    leKeyPadWidget_KeyClickEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setKeyClickEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyVisible(leKeyPadWidget* _this,
                           uint32_t row,
                           uint32_t col,
                           leBool b)

  Summary:
     Sets the enabled state of a key

  Description:
     Sets the enabled state of a key

  Parameters:
    leKeyPadWidget* _this - The keypad widget to operate on
    uint32_t row - the row index
    uint32_t col - the column index
    leBool b -

  Remarks:
    Usage - _this->fn->setKeyVisible(_this, row, col, b);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leKeyPadCellAction getKeyAction(const leKeyPadWidget* _this,
                                    uint32_t row,
                                    uint32_t col)

  Summary:
     Gets the key action setting

  Description:
     Gets the key action setting

  Parameters:
    const leKeyPadWidget* _this - The keypad widget to operate on
    uint32_t row - the row index
    uint32_t col - the column index

  Remarks:
    Usage - _this->fn->getKeyAction(_this, row, col);

  Returns:
    leKeyPadCellAction - the cell action value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyAction(leKeyPadWidget* _this,
                          uint32_t row,
                          uint32_t col,
                          leKeyPadCellAction action)

  Summary:
     Sets the key action setting

  Description:
     Sets the key action setting

  Parameters:
    leKeyPadWidget* _this - The keypad widget to operate on
    uint32_t row - the row index
    uint32_t col - the column index
    leKeyPadCellAction action - the cell action value

  Remarks:
    Usage - _this->fn->setKeyAction(_this, row, col, action);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leString* getKeyValue(const leKeyPadWidget* _this,
                          uint32_t row,
                          uint32_t col)

  Summary:
     Gets the key value

  Description:
     Gets the key value

  Parameters:
    const leKeyPadWidget* _this - The keypad widget to operate on
    uint32_t row - the row index
    uint32_t col - the column index

  Remarks:
    Usage - _this->fn->getKeyValue(_this, row, col);

  Returns:
    leString* - the string pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyValue(leKeyPadWidget* _this,
                         uint32_t row,
                         uint32_t col,
                         const leString* val)

  Summary:
     Sets the key value

  Description:
     Sets the key value

  Parameters:
    leKeyPadWidget* _this - The keypad widget to operate on
    uint32_t row - the row index
    uint32_t col - the column index
    const leString* val - the string pointer

  Remarks:
    Usage - _this->fn->setKeyValue(_this, row, col, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leButtonWidget* getCellButton(const leKeyPadWidget* _this,
                                  uint32_t row,
                                  uint32_t col)

  Summary:
     Get a button widget pointer from the keypad

  Description:
     Get a button widget pointer from the keypad

  Parameters:
    const leKeyPadWidget* _this - The keypad widget to operate on
    uint32_t row - the row index
    uint32_t col - the column index

  Remarks:
    Usage - _this->fn->getCellButton(_this, row, col);

  Returns:
    leButtonWidget* - pointer to the button widget
*/

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_WIDGET_KEYPAD_ENABLED && LE_WIDGET_BUTTON_ENABLED
#endif /* LEGATO_KEYPAD_H */
