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


  Description:
    This module implements button widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_CHECKBOX_H
#define LEGATO_CHECKBOX_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_CHECKBOX_WIDGET_ENABLED == 1

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

typedef struct leCheckBoxWidget leCheckBoxWidget;

// *****************************************************************************
/* Function Pointer:
    leCheckBoxWidget_CheckedEvent

  Summary:
    Checkbox checked event function callback type
*/
typedef void (*leCheckBoxWidget_CheckedEvent)(leCheckBoxWidget*);

// *****************************************************************************
/* Function Pointer:
    leCheckBoxWidget_UncheckedEvent

  Summary:
    Checkbox unchecked event function callback type
*/
typedef void (*leCheckBoxWidget_UncheckedEvent)(leCheckBoxWidget*);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
typedef struct leCheckBoxWidget leCheckBoxWidget;

#define LE_CHECKBOXWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leBool           (*getChecked)(const THIS_TYPE* _this); \
    leResult         (*setChecked)(THIS_TYPE* _this, leBool toggleable); \
    leImage*         (*getCheckedImage)(const THIS_TYPE* _this); \
    leResult         (*setCheckedImage)(THIS_TYPE* _this, leImage* img); \
    leImage*         (*getUncheckedImage)(const THIS_TYPE* _this); \
    leResult         (*setUncheckedImage)(THIS_TYPE* _this, leImage* img); \
    leRelativePosition  (*getImagePosition)(const THIS_TYPE* _this); \
    leResult         (*setImagePosition)(THIS_TYPE* _this, leRelativePosition pos); \
    uint32_t         (*getImageMargin)(const THIS_TYPE* _this); \
    leResult         (*setImageMargin)(THIS_TYPE* _this, uint32_t mg); \
    leString*        (*getString)(const THIS_TYPE* _this); \
    leResult         (*setString)(THIS_TYPE* _this, const leString* str); \
    leCheckBoxWidget_CheckedEvent   (*getCheckedEventCallback)(const THIS_TYPE* _this); \
    leResult         (*setCheckedEventCallback)(THIS_TYPE* _this, leCheckBoxWidget_CheckedEvent cb); \
    leCheckBoxWidget_UncheckedEvent (*getUncheckedEventCallback)(const THIS_TYPE* _this); \
    leResult         (*setUncheckedEventCallback)(THIS_TYPE* _this, leCheckBoxWidget_UncheckedEvent cb); \
    
    
typedef struct leCheckBoxWidgetVTable
{
	LE_CHECKBOXWIDGET_VTABLE(leCheckBoxWidget)
} leCheckBoxWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leCheckBoxWidget

  Summary:
    Implementation of a checkbox widget.

  Description:
    A check box widget contains an interactive two-state box indicating on or
    off.  The check box may also contain descriptive text.  Custom images for
    the check box may be used in place of the default box graphic.

  Remarks:
    None.
*/
typedef struct leCheckBoxWidget
{
    leWidget widget; // base class properties

    const leCheckBoxWidgetVTable* fn;

    leBool checked; // the state of the box

    const leString* string; // the text of the box

    const leImage* checkedImage; // pointer to a custom image to use for the
                                 // checked image
    const leImage* uncheckedImage; // pointer to a custom image to use for the
                                   // unchecked image

    leRelativePosition imagePosition; // postition of the image relative to the text
                                      // of the box
    uint32_t imageMargin; // the distance between the image and the text

    leCheckBoxWidget_CheckedEvent checkedEvent; // callback for checked events
    leCheckBoxWidget_CheckedEvent uncheckedEvent; // callback for unchecked events
} leCheckBoxWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leCheckBoxWidget* leCheckBoxWidget_New()

  Summary:
    Allocates memory for and initializes a new widget of this type.  The
    application is responsible for the managment of this memory until the
    widget is added to a widget tree.

  Description:


  Parameters:
    void

  Returns:
    leCheckBoxWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leCheckBoxWidget* leCheckBoxWidget_New();

/* Function:
    void leCheckBoxWidget_Constructor(leCheckBoxWidget* wgt)

  Summary:
    Initializes an CheckBox widget pointer.

  Description:
    Initializes an CheckBox widget pointer.

  Parameters:
    leCheckBoxWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leCheckBoxWidget_Constructor(leCheckBoxWidget* wgt);

// *****************************************************************************
/* Virtual Member Function:
    leBool getChecked(const leCheckBoxWidget* _this)

  Summary:
     Gets the checked state

  Description:
     Gets the checked state

  Parameters:
    const leCheckBoxWidget* _this - The checkbox to operate on

  Remarks:
    Usage - _this->fn->getChecked(_this);

  Returns:
    leBool - the checked state
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setChecked(leCheckBoxWidget* _this,
                        leBool toggleable)

  Summary:
     Sets the checked state

  Description:
     Sets the checked state

  Parameters:
    leCheckBoxWidget* _this - The checkbox to operate on
    leBool toggleable - the togglable state

  Remarks:
    Usage - _this->fn->setChecked(_this, toggleable);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leImage* getCheckedImage(const leCheckBoxWidget* _this)

  Summary:
     Gets the checked image pointer

  Description:
     Gets the checked image pointer

  Parameters:
    const leCheckBoxWidget* _this - The checkbox to operate on

  Remarks:
    Usage - _this->fn->getCheckedImage(_this);

  Returns:
    leImage* - the image pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCheckedImage(leCheckBoxWidget* _this,
                             leImage* img)

  Summary:
     Sets the checked image pointer

  Description:
     Sets the checked image pointer

  Parameters:
    leCheckBoxWidget* _this - The checkbox to operate on
    leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setCheckedImage(_this, img);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leImage* getUncheckedImage(const leCheckBoxWidget* _this)

  Summary:
     Gets the unchecked image pointer

  Description:
     Gets the unchecked image pointer

  Parameters:
    const leCheckBoxWidget* _this - The checkbox to operate on

  Remarks:
    Usage - _this->fn->getUncheckedImage(_this);

  Returns:
    leImage* - the image pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setUncheckedImage(leCheckBoxWidget* _this,
                               leImage* img)

  Summary:
     Sets the unchecked image pointer

  Description:
     Sets the unchecked image pointer

  Parameters:
    leCheckBoxWidget* _this - The checkbox to operate on
    leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setUncheckedImage(_this, img);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leRelativePosition getImagePosition(const leCheckBoxWidget* _this)

  Summary:
     Gets the relative position of the image to the text

  Description:
     Gets the relative position of the image to the text

  Parameters:
    const leCheckBoxWidget* _this - The checkbox to operate on

  Remarks:
    Usage - _this->fn->getImagePosition(_this);

  Returns:
    leRelativePosition -
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setImagePosition(leCheckBoxWidget* _this,
                              leRelativePosition pos)

  Summary:
     Sets the text/image relative position

  Description:
     Sets the text/image relative position

  Parameters:
    leCheckBoxWidget* _this - The checkbox to operate on
    leRelativePosition pos - the position value

  Remarks:
    Usage - _this->fn->setImagePosition(_this, pos);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getImageMargin(const leCheckBoxWidget* _this)

  Summary:
     Gets the image margin

  Description:
     Gets the image margin

  Parameters:
    const leCheckBoxWidget* _this - The checkbox to operate on

  Remarks:
    Usage - _this->fn->getImageMargin(_this);

  Returns:
    uint32_t - the margin value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setImageMargin(leCheckBoxWidget* _this,
                            uint32_t mg)

  Summary:
     Sets the image margin

  Description:
     Sets the image margin

  Parameters:
    leCheckBoxWidget* _this - The checkbox to operate on
    uint32_t mg - the margin value

  Remarks:
    Usage - _this->fn->setImageMargin(_this, mg);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leString* getString(const leCheckBoxWidget* _this)

  Summary:
     Gets the current string pointer

  Description:
     Gets the current string pointer

  Parameters:
    const leCheckBoxWidget* _this - The checkbox to operate on

  Remarks:
    Usage - _this->fn->getString(_this);

  Returns:
    leString* - the string pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setString(leCheckBoxWidget* _this,
                       const leString* str)

  Summary:
     Sets the current string pointer

  Description:
     Sets the current string pointer

  Parameters:
    leCheckBoxWidget* _this - The checkbox to operate on
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setString(_this, str);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leCheckBoxWidget_CheckedEvent getCheckedEventCallback(const leCheckBoxWidget* _this)

  Summary:
     Gets the checked event callback

  Description:
     Gets the checked event callback

  Parameters:
    const leCheckBoxWidget* _this - The checkbox to operate on

  Remarks:
    Usage - _this->fn->getCheckedEventCallback(_this);

  Returns:
    leCheckBoxWidget_CheckedEvent - the event callback
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCheckedEventCallback(leCheckBoxWidget* _this,
                                     leCheckBoxWidget_CheckedEvent cb)

  Summary:
     Sets the checked event callback

  Description:
     Sets the checked event callback

  Parameters:
    leCheckBoxWidget* _this - The checkbox to operate on
    leCheckBoxWidget_CheckedEvent cb - the event callback

  Remarks:
    Usage - _this->fn->setCheckedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leCheckBoxWidget_UncheckedEvent getUncheckedEventCallback(const leCheckBoxWidget* _this)

  Summary:
     Gets the unchecked event callback

  Description:
     Gets the unchecked event callback

  Parameters:
    const leCheckBoxWidget* _this - The checkbox to operate on

  Remarks:
    Usage - _this->fn->getUncheckedEventCallback(_this);

  Returns:
    leCheckBoxWidget_UncheckedEvent - the event callback
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setUncheckedEventCallback(leCheckBoxWidget* _this,
                                       leCheckBoxWidget_UncheckedEvent cb)

  Summary:
     Sets the unchecked event callback

  Description:
     Sets the unchecked event callback

  Parameters:
    leCheckBoxWidget* _this - The checkbox to operate on
    leCheckBoxWidget_UncheckedEvent cb - the event callback

  Remarks:
    Usage - _this->fn->setUncheckedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/


#endif // LE_CHECKBOX_WIDGET_ENABLED
#endif /* LEGATO_BUTTON_H */
