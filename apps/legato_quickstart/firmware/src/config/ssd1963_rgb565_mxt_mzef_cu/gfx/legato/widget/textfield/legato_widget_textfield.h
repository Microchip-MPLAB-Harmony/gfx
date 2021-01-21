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
    legato_widget_textfield.h

  Summary:


  Description:
    This module implements text field widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN

#ifndef LEGATO_TEXTFIELD_H
#define LEGATO_TEXTFIELD_H
//DOM-IGNORE-END

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
typedef void (*leTextFieldWidget_TextChangedCallback)(leTextFieldWidget*);

// *****************************************************************************
/* Function Pointer:
    leTextFieldWidget_FocusChangedCallback

  Summary:
    Focus changed event function callback type
*/
typedef void (*leTextFieldWidget_FocusChangedCallback)(leTextFieldWidget*, leBool);

// DOM-IGNORE-BEGIN
typedef struct leTextFieldWidget leTextFieldWidget;

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
/* Enumeration:
    leTextFieldWidget

  Summary:
    Implementation of a text field widget.

  Description:
    A text field widget is a widget that is capable of displaying a single line
    of editable text.  This widget is capable of receiving edit events from the
    Legato edit event system.  It can also display a blinking cursor.

  Remarks:
    None.
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

// *****************************************************************************
/* Function:
    leTextFieldWidget* leTextFieldWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:


  Parameters:

  Returns:
    leTextFieldWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leTextFieldWidget* leTextFieldWidget_New();

/* Function:
    void leTextFieldWidget_Constructor(leTextFieldWidget* wgt)

  Summary:
    Initializes an leTextFieldWidget widget pointer.

  Description:
    Initializes an leTextFieldWidget widget pointer.

  Parameters:
    leTextFieldWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leTextFieldWidget_Constructor(leTextFieldWidget* txt);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getCursorDelay(const leTextFieldWidget* _this)

  Summary:
     Gets the cursor blink delay

  Description:
     Gets the cursor blink delay

  Parameters:
    const leTextFieldWidget* _this - The text field widget to operate on

  Remarks:
    Usage - _this->fn->getCursorDelay(_this);

  Returns:
    uint32_t - the blink delay
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCursorDelay(leTextFieldWidget* _this,
                            uint32_t dt)

  Summary:
     Sets the cursor blink delay

  Description:
     Sets the cursor blink delay

  Parameters:
    leTextFieldWidget* _this - The text field widget to operate on
    uint32_t dt - the blink delay

  Remarks:
    Usage - _this->fn->setCursorDelay(_this, dt);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getCursorEnabled(const leTextFieldWidget* _this)

  Summary:
     Gets the cursor enabled state

  Description:
     Gets the cursor enabled state

  Parameters:
    const leTextFieldWidget* _this - The text field widget to operate on

  Remarks:
    Usage - _this->fn->getCursorEnabled(_this);

  Returns:
    leBool - the enabled state
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCursorEnabled(leTextFieldWidget* _this,
                              leBool en)

  Summary:
     Sets the cursor enabled state

  Description:
     Sets the cursor enabled state

  Parameters:
    leTextFieldWidget* _this - The text field widget to operate on
    leBool en - the enabled state

  Remarks:
    Usage - _this->fn->setCursorEnabled(_this, en);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getCursorPosition(const leTextFieldWidget* _this)

  Summary:
     Gets the cursor position

  Description:
     Gets the cursor position

  Parameters:
    const leTextFieldWidget* _this - The text field widget to operate on

  Remarks:
    Usage - _this->fn->getCursorPosition(_this);

  Returns:
    uint32_t - the cursor position
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCursorPosition(leTextFieldWidget* _this,
                               uint32_t pos)

  Summary:
     Sets the cursor position

  Description:
     Sets the cursor position

  Parameters:
    leTextFieldWidget* _this - The text field widget to operate on
    uint32_t pos - the cursor position

  Remarks:
    Usage - _this->fn->setCursorPosition(_this, pos);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    const leString* getString(const leTextFieldWidget* txt)

  Summary:
     Gets the pointer to the text field value string

  Description:
     Gets the pointer to the text field value string

  Parameters:
    const leTextFieldWidget* txt - The text field widget to operate on

  Remarks:
    Usage - _this->fn->getString(_this);

  Returns:
    const leString* - the string value pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setString(leTextFieldWidget* _this,
                       const leString* str)

  Summary:
     Sets the text field value to a given string

  Description:
     Sets the text field value to a given string

  Parameters:
    leTextFieldWidget* _this - The text field widget to operate on
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setString(_this, str);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leFont* getFont(const leTextFieldWidget* _this)

  Summary:
     Gets the text field font pointer

  Description:
     Gets the text field font pointer

  Parameters:
    const leTextFieldWidget* _this - The text field widget to operate on

  Remarks:
    Usage - _this->fn->getFont(_this);

  Returns:
    leFont* - the font pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFont(leTextFieldWidget* _this,
                     const leFont* fnt)

  Summary:
     Sets the text field font pointer

  Description:
     Sets the text field font pointer

  Parameters:
    leTextFieldWidget* _this - The text field widget to operate on
    const leFont* fnt - the font pointer

  Remarks:
    Usage - _this->fn->setFont(_this, fnt);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leString* getHintString(const leTextFieldWidget* txt)

  Summary:
     Gets the hint string

  Description:
     Gets the hint string

  Parameters:
    const leTextFieldWidget* txt - The text field widget to operate on

  Remarks:
    Usage - _this->fn->getHintString(_this);

  Returns:
    leString* - the string pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setHintString(leTextFieldWidget* _this,
                           const leString* str)

  Summary:
     Sets the hint string

  Description:
     Sets the hint string

  Parameters:
    leTextFieldWidget* _this - The text field widget to operate on
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setHintString(_this, str);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setClearValueOnFirstEdit(leTextFieldWidget* _this,
                                      leBool clr)

  Summary:
     Clears the initial string value on first edit

  Description:
     Clears the initial string value on first edit

  Parameters:
    leTextFieldWidget* _this - The text field widget to operate on
    leBool clr -

  Remarks:
    Usage - _this->fn->setClearValueOnFirstEdit(_this, clr);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leTextFieldWidget_TextChangedCallback getTextChangedEventCallback(const leTextFieldWidget* _this)

  Summary:
     Gets the text changed event callback pointer

  Description:
     Gets the text changed event callback pointer

  Parameters:
    const leTextFieldWidget* _this - The text field widget to operate on

  Remarks:
    Usage - _this->fn->getTextChangedEventCallback(_this);

  Returns:
    leTextFieldWidget_TextChangedCallback - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setTextChangedEventCallback(leTextFieldWidget* _this,
                                         leTextFieldWidget_TextChangedCallback cb)

  Summary:
     Sets the text changed event callback pointer

  Description:
     Sets the text changed event callback pointer

  Parameters:
    leTextFieldWidget* _this - The text field widget to operate on
    leTextFieldWidget_TextChangedCallback cb - the callback pointer

  Remarks:
    Usage - _this->fn->setTextChangedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leTextFieldWidget_FocusChangedCallback getFocusChangedEventCallback(const leTextFieldWidget* _this)

  Summary:
     Gets the focus changed event callback pointer

  Description:
     Gets the focus changed event callback pointer

  Parameters:
    const leTextFieldWidget* _this - The text field widget to operate on

  Remarks:
    Usage - _this->fn->getFocusChangedEventCallback(_this);

  Returns:
    leTextFieldWidget_FocusChangedCallback - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setFocusChangedEventCallback(leTextFieldWidget* _this,
                                          leTextFieldWidget_FocusChangedCallback cb)

  Summary:
     Sets the focus changed event callback pointer

  Description:
     Sets the focus changed event callback pointer

  Parameters:
    leTextFieldWidget* _this - The text field widget to operate on
    leTextFieldWidget_FocusChangedCallback cb - the callback pointer

  Remarks:
    Usage - _this->fn->setFocusChangedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_TEXTFIELD_WIDGET_ENABLED
#endif /* LEGATO_TEXTFIELD_H */
