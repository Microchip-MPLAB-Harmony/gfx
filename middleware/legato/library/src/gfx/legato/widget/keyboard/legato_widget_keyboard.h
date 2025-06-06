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
    legato_widget_keyboard.h

  Summary:


  Description:
    This module implements keyboard widget functions.
*******************************************************************************/

/** \file legato_widget_keyboard.h
 * @brief Keypad widget functions and definitions.
 *
 * @details This module implements keyboard widget functions.
 */

#ifndef LEGATO_KEYPAD_H
#define LEGATO_KEYPAD_H

#include "gfx/legato/common/legato_common.h"

#if LE_KEYBOARD_WIDGET_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/widget/legato_editwidget.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/string/legato_fixedstring.h"

#define KEY_AREA_GAP_RATIO 16

#define NUM_KEYS_ROW0 10
#define NUM_KEYS_ROW1 9
#define NUM_KEYS_ROW2 9
#define NUM_KEYS_ROW3 5
#define NUM_KEY_ROWS 4

#define MAX_KEYSTR_LENGTH 8

typedef struct leButtonWidget leButtonWidget;
typedef struct leKeyboardWidget leKeyboardWidget;

// *****************************************************************************
/* Function Pointer:
    leKeyboardWidget_KeyClickEvent

  Summary:
    Key click event function callback type
*/
typedef void (* leKeyboardWidget_KeyClickEvent)(leKeyboardWidget*,
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
    leKeyboardCellAction

  Summary:
    Defines an assigned action to a keyboard cell

  Description:


  Remarks:
    None.
*/
typedef enum leKeyboardKeyType
{
    LE_KEYBOARD_KEY_TEXT = 0,
    /* Special Keys */
    LE_KEYBOARD_KEY_SHIFT = (1 << 16),
    LE_KEYBOARD_KEY_BACKSPACE,
    LE_KEYBOARD_KEY_SYMBOL1,  //"?123"
    LE_KEYBOARD_KEY_SYMBOL2,
    LE_KEYBOARD_KEY_ABC,      //"ABC"
    LE_KEYBOARD_KEY_RETURN,
    LE_KEYBOARD_KEY_SPECIAL_MASK = 0xffff0000,
} leKeyboardKeyType;

typedef enum leKeyboardKeyMode
{
    LE_KEYBOARD_MODE_DEFAULT,
    LE_KEYBOARD_MODE_SHIFT,
    LE_KEYBOARD_MODE_SYMBOL1,
    LE_KEYBOARD_MODE_SYMBOL2,
    LE_KEYBOARD_MODE_MAX,
} leKeyboardKeyMode;

typedef enum leKeyboardKeyState
{
    LE_KEYBOARD_KEY_RELEASED,
    LE_KEYBOARD_KEY_PRESSED,
} leKeyboardKeyState;

// *****************************************************************************
/* Structure:
    leKeyboardActionTrigger

  Summary:
    Defines the trigger for keyboard action and events

  Description:


  Remarks:
    None.
*/
typedef enum leKeyboardActionTrigger
{
    LE_KEYBOARD_TRIGGER_KEYRELEASED,
    LE_KEYBOARD_TRIGGER_KEYPRESSED,
} leKeyboardActionTrigger;

// *****************************************************************************
/* Structure:
    leKeyboardKey

  Summary:
    Defines a keyboard key

  Description:
    A keyboard is made up of an array of keyboard keys.  Each key is individually
    is an active area on the widget that responds to touch events and affects the 
    edit widget in focus

  Remarks:
    None.
*/
typedef struct leKeyboardKey
{
    leRect rect;                //The relative rectangle area of the key
    leKeyboardKeyType type;     //The type of the key
    leKeyboardKeyState state;   //Pressed and released state of the key
} leKeyboardKey;


/* internal use only */
/**
  * @cond INTERNAL
  *
  */
#define LE_KEYBOARDWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leKeyboardActionTrigger (*getKeyboardActionTrigger)(const THIS_TYPE* _this); \
    leResult              (*setKeyboardActionTrigger)(THIS_TYPE* _this, leKeyboardActionTrigger trg); \
    leKeyboardWidget_KeyClickEvent (*getKeyClickEventCallback)(const THIS_TYPE* _this); \
    leResult              (*setKeyClickEventCallback)(THIS_TYPE* _this, leKeyboardWidget_KeyClickEvent cb); \
    leResult              (*setKeyFillStyle)(THIS_TYPE* _this, leFillStyle* style); \
    leResult              (*setKeyFont)(THIS_TYPE* _this, const leFont* font); \
    leResult              (*setKeyMap)(THIS_TYPE* _this, uint32_t row, leKeyboardKeyType * type, uint32_t numkeys); \
    leResult              (*setKeyMode)(THIS_TYPE* _this, leKeyboardKeyMode mode); \

typedef struct leKeyboardWidgetVTable
{
    LE_KEYBOARDWIDGET_VTABLE(leKeyboardWidget)
} leKeyboardWidgetVTable;

/**
  * @endcond
  *
  */

// *****************************************************************************
/**
 * @brief This struct represents a keyboard widget
 * @details A keyboard is a widget that is comprised of an array of
 * keys. This widget serves to issue edit events based on
 * application or input interaction.  Receptor edit widgets can then
 * receive these edit events and react accordingly.
 */
typedef struct leKeyboardWidget
{
    leWidget widget; // widget base class

    const leKeyboardWidgetVTable* fn;

    leKeyboardActionTrigger trigger; //trigger for action and events
    leKeyboardWidget_KeyClickEvent clickEvt; // key click callback event
    
    leFillStyle* keyStyle; //The fill style for the keys

    const leFont* keyFont; //The font used for the key label

    leKeyboardKey keysRow0[NUM_KEYS_ROW0]; //Array of keys in top row
    leKeyboardKey keysRow1[NUM_KEYS_ROW1]; //Array of keys in second row
    leKeyboardKey keysRow2[NUM_KEYS_ROW2]; //Array of keys in third row
    leKeyboardKey keysRow3[NUM_KEYS_ROW3]; //Array of keys in fourth row

    leKeyboardKey * currFocusKey; //The currently pressed key
    leKeyboardKeyMode mode; //The keyboard mode (normal, shift, num, sym, etc))

    /* internal use only */
    leFixedString _keyString;  
    leChar _keyStringChar[MAX_KEYSTR_LENGTH];
} leKeyboardWidget;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */

const leRect * _leKeyboardWidget_getKeyRect(leKeyboardWidget* _this, uint32_t row, uint32_t idx);
leKeyboardKey * _leKeyboardWidget_getKey(leKeyboardWidget* _this, uint32_t row, uint32_t idx);
uint32_t _leKeyboardWidget_getKeyCount(leKeyboardWidget* _this, uint32_t row);
const leString * _leKeyboardWidget_getKeyString(leKeyboardWidget* _this, leKeyboardKey * key);
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leKeyboardWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leKeyboardWidget* wgt = leKeyboardWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leKeyboardWidget* leKeyboardWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leKeyboardWidget <span class="param">wgt</span>.
 * @code
 * leKeyboardWidget* wgt;
 * leKeyboardWidget_Constructor(leKeyboardWidget* wgt);
 * @endcode
 * @param wgt is the widget to initialize.

 * @return void.
 */
void leKeyboardWidget_Constructor(leKeyboardWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/* Virtual Member Function:
    leKeyboardActionTrigger getKeyboardActionTrigger(const leKeyboardWidget* _this)

  Summary:
     Gets the keyboard trigger action

  Description:
     Gets the keyboard trigger action

  Parameters:
    const leKeyboardWidget* _this - The keyboard widget to operate on

  Remarks:
    Usage - _this->fn->getKeyboardActionTrigger(_this);

  Returns:
    leKeyboardActionTrigger - the trigger action
*/

virtual leKeyboardActionTrigger getKeyboardActionTrigger(const THIS_TYPE* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyboardActionTrigger(leKeyboardWidget* _this,
                                    leKeyboardActionTrigger trg)

  Summary:
     Sets the keyboard trigger action

  Description:
     Sets the keyboard trigger action

  Parameters:
    leKeyboardWidget* _this - The keyboard widget to operate on
    leKeyboardActionTrigger trg - the trigger action

  Remarks:
    Usage - _this->fn->setKeyboardActionTrigger(_this, trg);

  Returns:
    leResult - the result of the operation
*/

virtual leResult (*setKeyboardActionTrigger)(THIS_TYPE* _this, leKeyboardActionTrigger trg);

/**
 * @brief Get key click event callback pointer.
 * @details Gets the key click event callback pointer using <span class="param">_this</span>.
 * @code
 * leKeyboardWidget* _this;
 * leKeyboardWidget_KeyClickEvent cb = _this->fn->getKeyClickEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the callback pointer.
 */
virtual leKeyboardWidget_KeyClickEvent getKeyClickEventCallback(const leKeyboardWidget* _this);

/**
 * @brief Set key click event callback pointer.
 * @details Sets the key click event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leKeyboardWidget* _this;
 * leKeyboardWidget_KeyClickEvent cb;
 * leResult res = _this->fn->setKeyClickEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setKeyClickEventCallback(leKeyboardWidget* _this,
                                          leKeyboardWidget_KeyClickEvent cb);

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyClickEventCallback(leKeyboardWidget* _this,
                                      leKeyboardWidget_KeyClickEvent cb)

  Summary:
     Sets the key click event callback

  Description:
     Sets the key click event callback

  Parameters:
    leKeyboardWidget* _this - The keyboard widget to operate on
    leKeyboardWidget_KeyClickEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setKeyClickEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyFillStyle(THIS_TYPE* _this, 
                             leFillStyle* style)

  Summary:
     Sets the fill style of the keyboard keys

  Description:
     Sets the fill style of the keyboard keys

  Parameters:
    leKeyboardWidget* _this - The keyboard widget to operate on
    leFillStyle * style - the fill style

  Remarks:
    Usage - _this->fn->setKeyFillStyle(_this, style);

  Returns:
    leResult - the result of the operation
*/
virtual leResult setKeyFillStyle(THIS_TYPE* _this, leFillStyle* style);

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyFont(THIS_TYPE* _this, const leFont* font)

  Summary:
     Sets the font for the key labels

  Description:
     Sets the font for the key labels

  Parameters:
    leKeyboardWidget* _this - The keyboard widget to operate on
    const leFont* font - the font

  Remarks:
    Usage - _this->fn->setKeyFont(_this, const leFont* font);

  Returns:
    leResult - the result of the operation
*/
virtual leResult setKeyFont(THIS_TYPE* _this, const leFont* font);

// *****************************************************************************
/* Virtual Member Function:
    leResult setKeyMap(THIS_TYPE* _this, uint32_t row, leKeyboardKeyType * type, uint32_t numkeys);

  Summary:
     Sets the key mapping for the keys in the specified row

  Description:
     Sets the key mapping for the keys in the specified row

  Parameters:
    leKeyboardWidget* _this - The keyboard widget to operate on
    uint32_t row - the row of keys 
    leKeyboardKeyType * type - pointer to array of keytype for each key in the row
    uint32_t numkeys - the number of keys

  Remarks:
    Usage - _this->fn->setKeyMap(_this, row, type, numkeys)

  Returns:
    leResult - the result of the operation
*/
virtual leResult setKeyMap(THIS_TYPE* _this, uint32_t row, leKeyboardKeyType * type, uint32_t numkeys);

/* Virtual Member Function:
    leResult setKeyMode(THIS_TYPE* _this, leKeyboardKeyMode mode)

  Summary:
     Sets the mode of the keyboard

  Description:
     Sets the mode of the keyboard

  Parameters:
    leKeyboardWidget* _this - The keyboard widget to operate on
    leKeyboardKeyMode mode - the mode

  Remarks:
    Usage - _this->fn->setKeyMode(THIS_TYPE* _this, leKeyboardKeyMode mode);

  Returns:
    leResult - the result of the operation
*/
virtual leResult setKeyMode(THIS_TYPE* _this, leKeyboardKeyMode mode);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_WIDGET_KEYBOARD_ENABLED
#endif /* LEGATO_KEYBOARD_H */
