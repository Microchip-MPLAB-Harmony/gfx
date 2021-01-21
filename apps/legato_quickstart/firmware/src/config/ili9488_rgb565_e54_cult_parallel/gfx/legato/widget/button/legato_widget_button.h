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
    legato_widget_button.h

  Summary:
    Defines a button widget
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_BUTTON_H
#define LEGATO_BUTTON_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_BUTTON_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/string/legato_string.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leButtonState

  Summary:
    Controls the button pressed state
*/

typedef enum leButtonState
{
    LE_BUTTON_STATE_UP,
    LE_BUTTON_STATE_DOWN,
    LE_BUTTON_STATE_TOGGLED
} leButtonState;

typedef struct leButtonWidget leButtonWidget;

// *****************************************************************************
/* Function Pointer:
    leButtonWidget_PressedEvent

  Summary:
    Button pressed event function callback type
*/
typedef void (*leButtonWidget_PressedEvent)(leButtonWidget*);

// *****************************************************************************
/* Function Pointer:
    leButtonWidget_ReleasedEvent

  Summary:
    Button released event function callback type
*/
typedef void (*leButtonWidget_ReleasedEvent)(leButtonWidget*);


// DOM-IGNORE-BEGIN
typedef struct leButtonWidget leButtonWidget;

#define LE_BUTTONWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leBool                       (*getToggleable)(const THIS_TYPE* _this); \
    leResult                     (*setToggleable)(THIS_TYPE* _this, leBool toggleable); \
    leBool                       (*getPressed)(const THIS_TYPE* _this); \
    leResult                     (*setPressed)(THIS_TYPE* _this, leBool pressed); \
    leString*                    (*getString)(const THIS_TYPE* _this); \
    leResult                     (*setString)(THIS_TYPE* _this, const leString* str); \
    leImage*                     (*getPressedImage)(const THIS_TYPE* _this); \
    leResult                     (*setPressedImage)(THIS_TYPE* _this, leImage* img); \
    leImage*                     (*getReleasedImage)(const THIS_TYPE* _this); \
    leResult                     (*setReleasedImage)(THIS_TYPE* _this, leImage* img); \
    leRelativePosition           (*getImagePosition)(const THIS_TYPE* _this); \
    leResult                     (*setImagePosition)(THIS_TYPE* _this, leRelativePosition pos); \
    uint32_t                     (*getImageMargin)(const THIS_TYPE* _this); \
    leResult                     (*setImageMargin)(THIS_TYPE* _this, uint32_t mg); \
    int32_t                      (*getPressedOffset)(const THIS_TYPE* _this); \
    leResult                     (*setPressedOffset)(THIS_TYPE* _this, int32_t offs); \
    leButtonWidget_PressedEvent  (*getPressedEventCallback)(const THIS_TYPE* _this); \
    leResult                     (*setPressedEventCallback)(THIS_TYPE* _this, leButtonWidget_PressedEvent cb); \
    leButtonWidget_ReleasedEvent (*getReleasedEventCallback)(const THIS_TYPE* _this); \
    leResult                     (*setReleasedEventCallback)(THIS_TYPE* _this, leButtonWidget_ReleasedEvent cb); \
    
    
typedef struct leButtonWidgetVTable
{
	LE_BUTTONWIDGET_VTABLE(leButtonWidget)
} leButtonWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leButtonWidget

  Summary:
    Implementation of a button widget.  A button is an interactive element
    that simulates a typical button with a pressed an released state.

  Description:


  Remarks:
    None.
*/
typedef struct leButtonWidget
{
    leWidget widget;  // base widget header

    const leButtonWidgetVTable* fn;

    leButtonState state; // button state
    uint8_t toggleable; // indicates if the button is toggleable

    const leString* string; // the string that holds the button text

    const leImage* pressedImage; // button pressed icon image
    const leImage* releasedImage; // button released icon image

    leRelativePosition imagePosition; // icon position in relation to text
    uint32_t imageMargin; // distance between text and icon
    int32_t pressedOffset; // pressed text offset distance

    leButtonWidget_PressedEvent pressedEvent; // pressed event callback
    leButtonWidget_ReleasedEvent releasedEvent; // released event callback
} leButtonWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leButtonWidget* leButtonWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Description:
    Creates a new button widget instance.  Invokes the button constructor

  Returns:
    leButtonWidget* - pointer to a new button widget instance

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leButtonWidget* leButtonWidget_New();

/* Function:
    void leButtonWidget_Constructor(leButtonWidget* wgt)

  Summary:
    Initializes an leButtonWidget widget pointer.

  Description:
    Initializes an leButtonWidget widget pointer.

  Parameters:
    leButtonWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leButtonWidget_Constructor(leButtonWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    leBool getToggleable(const leButtonWidget* _this)

  Summary:
     Gets the togglable setting

  Description:
     Gets the togglable setting

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getToggleable(_this);

  Returns:
    leBool - the toggleable state
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setToggleable(leButtonWidget* _this,
                           leBool toggleable)

  Summary:
     Sets the togglable setting

  Description:
     Sets the togglable setting

  Parameters:
    leButtonWidget* _this - The button to operate on
    leBool toggleable - the toggleable state

  Remarks:
    Usage - _this->fn->setToggleable(_this, toggleable);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getPressed(const leButtonWidget* _this)

  Summary:
     Gets the pressed state of the button

  Description:
     Gets the pressed state of the button

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getPressed(_this);

  Returns:
    leBool - the pressed state
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setPressed(leButtonWidget* _this,
                        leBool pressed)

  Summary:
     Sets the pressed state of the button

  Description:
     Sets the pressed state of the button

  Parameters:
    leButtonWidget* _this - The button to operate on
    leBool pressed - the pressed state

  Remarks:
    Usage - _this->fn->setPressed(_this, pressed);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leString* getString(const leButtonWidget* _this)

  Summary:
     Gets the string assigned to the button

  Description:
     Gets the string assigned to the button

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getString(_this);

  Returns:
    leString* - the string pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setString(leButtonWidget* _this,
                       const leString* str)

  Summary:
     Assigns a string to the button

  Description:
     Assigns a string to the button

  Parameters:
    leButtonWidget* _this - The button to operate on
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setString(_this, str);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leImage* getPressedImage(const leButtonWidget* _this)

  Summary:
     Gets the pressed image pointer

  Description:
     Gets the pressed image pointer

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getPressedImage(_this);

  Returns:
    leImage* - the image pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setPressedImage(leButtonWidget* _this,
                             leImage* img)

  Summary:
     Sets the pressed image pointer

  Description:
     Sets the pressed image pointer

  Parameters:
    leButtonWidget* _this - The button to operate on
    leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setPressedImage(_this, img);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leImage* getReleasedImage(const leButtonWidget* _this)

  Summary:
     Gets the released image pointer

  Description:
     Gets the released image pointer

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getReleasedImage(_this);

  Returns:
    leImage* - the image pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setReleasedImage(leButtonWidget* _this,
                              leImage* img)

  Summary:
     Sets the released image pointer

  Description:
     Sets the released image pointer

  Parameters:
    leButtonWidget* _this - The button to operate on
    leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setReleasedImage(_this, img);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leRelativePosition getImagePosition(const leButtonWidget* _this)

  Summary:
     Gets the relative position of the image to the text

  Description:
     Gets the relative position of the image to the text

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getImagePosition(_this);

  Returns:
    leRelativePosition - the relative position
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setImagePosition(leButtonWidget* _this,
                              leRelativePosition pos)

  Summary:
     Gets the relative position of the image to the text

  Description:
     Gets the relative position of the image to the text

  Parameters:
    leButtonWidget* _this - The button to operate on
    leRelativePosition pos - the relative position

  Remarks:
    Usage - _this->fn->setImagePosition(_this, pos);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getImageMargin(const leButtonWidget* _this)

  Summary:
     Gets the image margin

  Description:
     Gets the image margin

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getImageMargin(_this);

  Returns:
    uint32_t - the margin value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setImageMargin(leButtonWidget* _this,
                            uint32_t mg)

  Summary:
     Sets the image margin

  Description:
     Sets the image margin

  Parameters:
    leButtonWidget* _this - The button to operate on
    uint32_t mg - the margin value

  Remarks:
    Usage - _this->fn->setImageMargin(_this, mg);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    int32_t getPressedOffset(const leButtonWidget* _this)

  Summary:
     Gets the pressed offset value

  Description:
     Gets the pressed offset value

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getPressedOffset(_this);

  Returns:
    int32_t - the offset value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setPressedOffset(leButtonWidget* _this,
                              int32_t offs)

  Summary:
     Sets the pressed offset value

  Description:
     Sets the pressed offset value

  Parameters:
    leButtonWidget* _this - The button to operate on
    int32_t offs - the offset value

  Remarks:
    Usage - _this->fn->setPressedOffset(_this, offs);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leButtonWidget_PressedEvent getPressedEventCallback(const leButtonWidget* _this)

  Summary:
     Gets the pressed event callback pointer

  Description:
     Gets the pressed event callback pointer

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getPressedEventCallback(_this);

  Returns:
    leButtonWidget_PressedEvent - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setPressedEventCallback(leButtonWidget* _this,
                                     leButtonWidget_PressedEvent cb)

  Summary:
     Sets the pressed event callback pointer

  Description:
     Sets the pressed event callback pointer

  Parameters:
    leButtonWidget* _this - The button to operate on
    leButtonWidget_PressedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setPressedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leButtonWidget_ReleasedEvent getReleasedEventCallback(const leButtonWidget* _this)

  Summary:
     Gets the released event callback pointer

  Description:
     Gets the released event callback pointer

  Parameters:
    const leButtonWidget* _this - The button to operate on

  Remarks:
    Usage - _this->fn->getReleasedEventCallback(_this);

  Returns:
    leButtonWidget_ReleasedEvent - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setReleasedEventCallback(leButtonWidget* _this,
                                      leButtonWidget_ReleasedEvent cb)

  Summary:
     Sets the released event callback pointer

  Description:
     Sets the released event callback pointer

  Parameters:
    leButtonWidget* _this - The button to operate on
    leButtonWidget_ReleasedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setReleasedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/



#endif /* LE_BUTTON_WIDGET_ENABLED */
#endif /* LEGATO_BUTTON_H */

