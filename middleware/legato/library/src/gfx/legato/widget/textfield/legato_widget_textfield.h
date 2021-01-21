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
    legato_widget_textfield.h

  Summary:


  Description:
    This module implements text field widget functions.
*******************************************************************************/

/** \file legato_widget_textfield.h
 * @brief Textfield Widget functions and definitions.
 *
 * @details This module implements text field widget functions.
 */

#ifndef LEGATO_TEXTFIELD_H
#define LEGATO_TEXTFIELD_H

#include "gfx/legato/common/legato_common.h"

#if LE_TEXTFIELD_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_editwidget.h"
#include "gfx/legato/string/legato_dynamicstring.h"

typedef struct leTextFieldWidget leTextFieldWidget;

// *****************************************************************************
/* Function Pointer:
    leTextFieldWidget_TextChangedCallback

  Summary:
    Text changed event function callback type
*/
/**
 * @brief This function represents a text changed event callback.
 * @details The callback is used indicate a text changed has occured.
 */
typedef void (*leTextFieldWidget_TextChangedCallback)(leTextFieldWidget*);

// *****************************************************************************
/* Function Pointer:
    leTextFieldWidget_FocusChangedCallback

  Summary:
    Focus changed event function callback type
*/
/**
 * @brief This function represents a focus change callback.
 * @details The callback is used indicate a focus change has occured.
 */
typedef void (*leTextFieldWidget_FocusChangedCallback)(leTextFieldWidget*, leBool);

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leTextFieldWidget leTextFieldWidget;

#define LE_TEXTFIELDWIDGET_VTABLE(THIS_TYPE) \
    LE_EDITWIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t   (*getCursorDelay)(const THIS_TYPE* _this); \
    leResult   (*setCursorDelay)(THIS_TYPE* _this, uint32_t dt); \
    leBool     (*getCursorEnabled)(const THIS_TYPE* _this); \
    leResult   (*setCursorEnabled)(THIS_TYPE* _this, leBool en); \
    uint32_t   (*getCursorPosition)(const THIS_TYPE* _this); \
    leResult   (*setCursorPosition)(THIS_TYPE* _this, uint32_t pos); \
    const leString* (*getString)(const THIS_TYPE* txt); \
    leResult   (*setString)(THIS_TYPE* _this, const leString* str); \
    leFont*    (*getFont)(const THIS_TYPE* _this); \
    leResult   (*setFont)(THIS_TYPE* _this, const leFont* fnt); \
    leString*  (*getHintString)(const THIS_TYPE* txt); \
    leResult   (*setHintString)(THIS_TYPE* _this, const leString* str); \
    leResult   (*setClearValueOnFirstEdit)(THIS_TYPE* _this, leBool clr); \
    leTextFieldWidget_TextChangedCallback (*getTextChangedEventCallback)(const THIS_TYPE* _this); \
    leResult   (*setTextChangedEventCallback)(THIS_TYPE* _this, leTextFieldWidget_TextChangedCallback cb); \
    leTextFieldWidget_FocusChangedCallback (*getFocusChangedEventCallback)(const THIS_TYPE* _this); \
    leResult   (*setFocusChangedEventCallback)(THIS_TYPE* _this, leTextFieldWidget_FocusChangedCallback cb); \
    
typedef struct leTextFieldWidgetVTable
{
	LE_TEXTFIELDWIDGET_VTABLE(leTextFieldWidget)
} leTextFieldWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/**
 * @brief This struct represents a text field widget.
 * @details A text field widget is a widget that is capable of displaying a
 * single line of editable text.  This widget is capable of receiving
 * edit events from the Legato edit event system.  It can also display a
 * blinking cursor.
 */
typedef struct leTextFieldWidget
{
    leEditWidget editWidget; // edit widget base class

    const leTextFieldWidgetVTable* fn;

    const leString* hintText;
    leDynamicString text; // the text to edit

    uint32_t cursorPos; // current cursor position
    uint32_t cursorDelay; // cursor blink delay
    uint32_t cursorTime; // current cursor tick counter
    leBool cursorEnable; // cursor enabled flag
    leBool cursorVisible; // cursor visbility flag
    leBool clearOnFirstEdit; // needs clear on first edit

    leTextFieldWidget_TextChangedCallback textChangedEvent; // text changed event
    leTextFieldWidget_FocusChangedCallback focusChangedEvent; // focus changed event
} leTextFieldWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leTextFieldWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leTextFieldWidget* wgt = leTextFieldWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leTextFieldWidget* leTextFieldWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leTextFieldWidget <span class="param">wgt</span>.
 * @code
 * leTextFieldWidget* wgt;
 * leTextFieldWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leTextFieldWidget_Constructor(leTextFieldWidget* wgt);


#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget


// *****************************************************************************
/**
 * @brief Get cursor blink delay.
 * @details Gets the cursor blink delay from  <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * uint32_t delay = _this->fn->getCursorDelay(_this);
 * @endcode
 * @param  _this is the widget to query
 * @return the blink delay.
 */
virtual uint32_t getCursorDelay(const leTextFieldWidget* _this);


// *****************************************************************************
/**
 * @brief Set cursor blink delay.
 * @details Sets the cursor blink delay for <span class="param">wgt</span>.
 * @code
 * leTextFieldWidget* wgt;
 * uint32_t dt;
 * leResult res = _this->fn->setCursorDelay(_this, dt);
 * @endcode
 * @param wgt is the widget to modify.
 * @param dt the the enabled state.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCursorDelay(leTextFieldWidget* _this,
                                uint32_t dt);

// *****************************************************************************
/**
 * @brief Get cursor enabled state.
 * @details Gets the cursor enabled state from <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * leBool enabled = _this->fn->getCursorEnabled(_this);
 * @endcode
 * @param  _this is the widget to query
 * @return the enabled state.
 */
virtual leBool getCursorEnabled(const leTextFieldWidget* _this);

// *****************************************************************************
/**
 * @brief Set cursor enabled state.
 * @details Sets the cursor enabled state for <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * leBool en;
 * leResult res = _this->fn->setCursorEnabled(_this, en);
 * @endcode
 * @param _this is the widget to modify.
 * @param en the the enabled state.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCursorEnabled(leTextFieldWidget* _this,
                                  leBool en);


// *****************************************************************************
/**
 * @brief Get cursor position.
 * @details Gets the cursor position from  <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * uint32_t pos = _this->fn->getString(_this);
 * @endcode
 * @param  _this is the widget to query
 * @return the cursor position.
 */
virtual uint32_t getCursorPosition(const leTextFieldWidget* _this);


// *****************************************************************************
/**
 * @brief Set cursor position.
 * @details Sets the cursor position for <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * uint32_t pos;
 * leResult res = _this->fn->setCursorPosition(_this, pos);
 * @endcode
 * @param _this is the widget to modify.
 * @param pos the cursor position.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setCursorPosition(leTextFieldWidget* _this,
                                   uint32_t pos);


// *****************************************************************************
/**
 * @brief Get pointer to the text field value string.
 * @details Gets the pointer to the text field value string
 * from <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * leString* str = _this->fn->getString(_this);
 * @endcode
 * @param  _this is the widget to query
 * @return string value pointer.
 */
virtual const leString* getString(const leTextFieldWidget* txt);


// *****************************************************************************
/**
 * @brief Set text field font pointer.
 * @details Sets the text field font pointer for <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * const leString* str;
 * leResult res = _this->fn->setString(_this, str);
 * @endcode
 * @param _this is the widget to modify.
 * @param str the string pointer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setString(leTextFieldWidget* _this,
                           const leString* str);

// *****************************************************************************
/**
 * @brief Get text field font pointer.
 * @details Gets the text field font pointer from  <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * leFont* fnt = _this->fn->getFont(_this);
 * @endcode
 * @param _this is the widget to query
 * @return the font pointer.
 */
virtual leFont* getFont(const leTextFieldWidget* _this);


// *****************************************************************************
/**
 * @brief Set text field font pointer.
 * @details Sets the text field font pointer for <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * const leFont* fnt;
 * leResult res = _this->fn->setFont(_this, fnt);
 * @endcode
 * @param _this is the widget to modify.
 * @param fnt the font pointer.
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFont(leTextFieldWidget* _this,
                         const leFont* fnt);


// *****************************************************************************
/**
 * @brief Get hint string.
 * @details Gets the hint string from <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * leString* str = _this->fn->getHintString(_this);
 * @endcode
 * @param  _this is the widget to query
 * @return the hint string.
 */
virtual leString* getHintString(const leTextFieldWidget* _this);


// *****************************************************************************
/**
 * @brief Set hint string.
 * @details Sets the hint string for <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * const leString* str;
 * leResult res = _this->fn->setHintString(_this, str);
 * @endcode
 * @param _this is the widget to modify.
 * @param str the string pointer
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setHintString(leTextFieldWidget* _this,
                               const leString* str);

// *****************************************************************************
/**
 * @brief Clears initial string value on first edit.
 * @details Clears the initial string value on first edit for <span class="param">_this</span>.
 * @remark This is a Virtual Member Function
 * @code
 * leTextFieldWidget* _this;
 * leBool clr;
 * leResult res = _this->fn->setClearValueOnFirstEdit(_this, clr);
 * @endcode
 * @param _this is the widget to modify.
 * @param clr
 * @return LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setClearValueOnFirstEdit(leTextFieldWidget* _this,
                                          leBool clr);


/**
 * @brief Get text changed event callback pointer.
 * @details Gets the text changed event callback pointer using <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * leTextFieldWidget_TextChangedCallback cb = _this->fn->getTextChangedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leTextFieldWidget_TextChangedCallback getTextChangedEventCallback
                                            (const leTextFieldWidget* _this);

/**
 * @brief Set text changed event callback pointer.
 * @details Sets the text changed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * leTextFieldWidget_TextChangedCallback cb;
 * leResult res = _this->fn->setTextChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setTextChangedEventCallback(leTextFieldWidget* _this,
                                             leTextFieldWidget_TextChangedCallback cb);

/**
 * @brief Get focus changed event callback pointer.
 * @details Gets the focus changed event callback pointer using <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * leTextFieldWidget_FocusChangedCallback cb = _this->fn->getFocusChangedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leTextFieldWidget_FocusChangedCallback getFocusChangedEventCallback
                                                (const leTextFieldWidget* _this);

/**
 * @brief Set focus changed event callback pointer.
 * @details Sets the focus changed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leTextFieldWidget* _this;
 * leTextFieldWidget_FocusChangedCallback cb;
 * leResult res = _this->fn->setFocusChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setFocusChangedEventCallback(leTextFieldWidget* _this,
                                              leTextFieldWidget_FocusChangedCallback cb);

#undef THIS_TYPE
#endif

#endif // LE_TEXTFIELD_WIDGET_ENABLED
#endif /* LEGATO_TEXTFIELD_H */
