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
    legato_widget_radiobutton.h

  Summary:
    

  Description:
    This module implements radio button widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_RADIOBUTTON_H
#define LEGATO_RADIOBUTTON_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_RADIOBUTTON_WIDGET_ENABLED == 1

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/widget/radiobutton/legato_radiobutton_group.h"
#include "gfx/legato/string/legato_string.h"

typedef struct leRadioButtonWidget leRadioButtonWidget;
typedef struct leRadioButtonGroup leRadioButtonGroup;

// *****************************************************************************
/* Function Pointer:
    leRadioButtonWidget_SelectedEvent

  Summary:
    Radio button selected function callback type
*/
typedef void (*leRadioButtonWidget_SelectedEvent)(leRadioButtonWidget*);

// *****************************************************************************
/* Function Pointer:
    leRadioButtonWidget_DeselectedEvent

  Summary:
    Radio button deselected function callback type
*/
typedef void (*leRadioButtonWidget_DeselectedEvent)(leRadioButtonWidget*);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// DOM-IGNORE-BEGIN
typedef struct leRadioButtonWidget leRadioButtonWidget;

#define LE_RADIOBUTTONWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leRadioButtonGroup* (*getGroup)(const THIS_TYPE* _this); \
    leBool              (*getSelected)(const THIS_TYPE* _this); \
    leResult            (*setSelected)(THIS_TYPE* _this); \
    void                (*select)(THIS_TYPE* _this); \
    void                (*deselect)(THIS_TYPE* _this); \
    leString*           (*getString)(const THIS_TYPE* _this); \
    leResult            (*setString)(THIS_TYPE* _this, const leString* str); \
    leImage*            (*getSelectedImage)(const THIS_TYPE* _this); \
    leResult            (*setSelectedImage)(THIS_TYPE* _this, const leImage* img); \
    leImage*            (*getUnselectedImage)(const THIS_TYPE* _this); \
    leResult            (*setUnselectedImage)(THIS_TYPE* _this, const leImage* img); \
    leRelativePosition  (*getImagePosition)(const THIS_TYPE* _this); \
    leResult            (*setImagePosition)(THIS_TYPE* _this, leRelativePosition pos); \
    uint32_t            (*getImageMargin)(const THIS_TYPE* _this); \
    leResult            (*setImageMargin)(THIS_TYPE* _this, uint32_t mg); \
    leRadioButtonWidget_SelectedEvent (*getSelectedEventCallback)(const THIS_TYPE* _this); \
    leResult            (*setSelectedEventCallback)(THIS_TYPE* _this, leRadioButtonWidget_SelectedEvent cb); \
    leRadioButtonWidget_DeselectedEvent (*getDeselectedEventCallback)(const THIS_TYPE* _this); \
    leResult            (*setDeselectedEventCallback)(THIS_TYPE* _this, leRadioButtonWidget_DeselectedEvent cb); \
    uint32_t            (*getCircleButtonSize)(const THIS_TYPE* _this); \
    leResult            (*setCircleButtonSize)(THIS_TYPE* _this, uint32_t sz); \
    
typedef struct leRadioButtonWidgetVTable
{
	LE_RADIOBUTTONWIDGET_VTABLE(leRadioButtonWidget)
} leRadioButtonWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Enumeration:
    leRadioButtonWidget

  Summary:
    Implementation of a radio button widget struct

  Description:
    A radio button is similar to a checkbox widget in that it has an on and
    off state.  It is further capable of being added to a radio button group.
    This group provides a mutually exclusive selection capability so that
    only one radio button may be selected at any one time.

  Remarks:
    None.
*/
typedef struct leRadioButtonWidget
{
    leWidget widget; // widget base class

    const leRadioButtonWidgetVTable* fn;

    leBool selected; // indicates if the radio button is selected

    const leString* string; // radio button text

    const leImage* selectedImage; // button custom selected image
    const leImage* unselectedImage; // buton custom unselected image
    
    leRelativePosition imagePosition; // image icon relative position
    
    uint32_t imageMargin; // image margin
    uint32_t circleButtonSize; // size of radio circle button in pixels

    leRadioButtonWidget_SelectedEvent selectedEvent; // button selected event callback
    leRadioButtonWidget_DeselectedEvent deselectedEvent; // button deselected event callback

    leRadioButtonGroup* group; // radio button group
} leRadioButtonWidget;


// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leRadioButtonWidget* leRadioButtonWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:
    

  Parameters:
    
  Returns:
    leRadioButtonWidget*
    
  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leRadioButtonWidget* leRadioButtonWidget_New();

/* Function:
    void leRadioButtonWidget_Constructor(leRadioButtonWidget* wgt)

  Summary:
    Initializes an leRadioButtonWidget widget pointer.

  Description:
    Initializes an leRadioButtonWidget widget pointer.

  Parameters:
    leRadioButtonWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leRadioButtonWidget_Constructor(leRadioButtonWidget* btn);

// *****************************************************************************
/* Virtual Member Function:
    leRadioButtonGroup* getGroup(const leRadioButtonWidget* _this)

  Summary:
     Gets the button's group pointer

  Description:
     Gets the button's group pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getGroup(_this);

  Returns:
    leRadioButtonGroup* - the radio button group
*/

// *****************************************************************************
/* Virtual Member Function:
    leBool getSelected(const leRadioButtonWidget* _this)

  Summary:
     Gets the button's selection state

  Description:
     Gets the button's selection state

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getSelected(_this);

  Returns:
    leBool - the selected state
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSelected(leRadioButtonWidget* _this)

  Summary:
     Selects the button

  Description:
     Selects the button

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->setSelected(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    void select(leRadioButtonWidget* _this)

  Summary:
     internal use only
*/

// *****************************************************************************
/* Virtual Member Function:
    void deselect(leRadioButtonWidget* _this)

  Summary:
     internal use only
*/

// *****************************************************************************
/* Virtual Member Function:
    leString* getString(const leRadioButtonWidget* _this)

  Summary:
     Gets the text string pointer

  Description:
     Gets the text string pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getString(_this);

  Returns:
    leString* - the string pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setString(leRadioButtonWidget* _this,
                       const leString* str)

  Summary:
     Sets the text string pointer

  Description:
     Sets the text string pointer

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    const leString* str - the string pointer

  Remarks:
    Usage - _this->fn->setString(_this, str);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leImage* getSelectedImage(const leRadioButtonWidget* _this)

  Summary:
     Gets the selected image pointer

  Description:
     Gets the selected image pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getSelectedImage(_this);

  Returns:
    leImage* - the image pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSelectedImage(leRadioButtonWidget* _this,
                              const leImage* img)

  Summary:
     Sets the selected image pointer

  Description:
     Sets the selected image pointer

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setSelectedImage(_this, img);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leImage* getUnselectedImage(const leRadioButtonWidget* _this)

  Summary:
     Gets the unselected image pointer

  Description:
     Gets the unselected image pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getUnselectedImage(_this);

  Returns:
    leImage* - the image pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setUnselectedImage(leRadioButtonWidget* _this,
                                const leImage* img)

  Summary:
     Sets the unselected image pointer

  Description:
     Sets the unselected image pointer

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setUnselectedImage(_this, img);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leRelativePosition getImagePosition(const leRadioButtonWidget* _this)

  Summary:
     Gets the image position

  Description:
     Gets the image position

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getImagePosition(_this);

  Returns:
    leRelativePosition - the position
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setImagePosition(leRadioButtonWidget* _this,
                              leRelativePosition pos)

  Summary:
     Sets the image position

  Description:
     Sets the image position

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    leRelativePosition pos - the position value

  Remarks:
    Usage - _this->fn->setImagePosition(_this, pos);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getImageMargin(const leRadioButtonWidget* _this)

  Summary:
     Gets the image margin

  Description:
     Gets the image margin

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getImageMargin(_this);

  Returns:
    uint32_t - the margin value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setImageMargin(leRadioButtonWidget* _this,
                            uint32_t mg)

  Summary:
     Sets the image margin

  Description:
     Sets the image margin

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    uint32_t mg - the margin value

  Remarks:
    Usage - _this->fn->setImageMargin(_this, mg);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leRadioButtonWidget_SelectedEvent getSelectedEventCallback(const leRadioButtonWidget* _this)

  Summary:
     Gets the selected event callback pointer

  Description:
     Gets the selected event callback pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getSelectedEventCallback(_this);

  Returns:
    leRadioButtonWidget_SelectedEvent - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setSelectedEventCallback(leRadioButtonWidget* _this,
                                      leRadioButtonWidget_SelectedEvent cb)

  Summary:
     Sets the selected event callback pointer

  Description:
     Sets the selected event callback pointer

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    leRadioButtonWidget_SelectedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setSelectedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leRadioButtonWidget_DeselectedEvent getDeselectedEventCallback(const leRadioButtonWidget* _this)

  Summary:
     Gets the deselected event callback pointer

  Description:
     Gets the deselected event callback pointer

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getDeselectedEventCallback(_this);

  Returns:
    leRadioButtonWidget_DeselectedEvent - the callback pointer
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setDeselectedEventCallback(leRadioButtonWidget* _this,
                                        leRadioButtonWidget_DeselectedEvent cb)

  Summary:
     Sets the deselected event callback pointer

  Description:
     Sets the deselected event callback pointer

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    leRadioButtonWidget_DeselectedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setDeselectedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getCircleButtonSize(const leRadioButtonWidget* _this)

  Summary:
     Gets the circle button size

  Description:
     Gets the circle button size

  Parameters:
    const leRadioButtonWidget* _this - The radio button widget to operate on

  Remarks:
    Usage - _this->fn->getCircleButtonSize(_this);

  Returns:
    uint32_t - the circle button size
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setCircleButtonSize(leRadioButtonWidget* _this,
                                 uint32_t sz)

  Summary:
     Sets the circle button size

  Description:
     Sets the circle button size

  Parameters:
    leRadioButtonWidget* _this - The radio button widget to operate on
    uint32_t sz - the size value

  Remarks:
    Usage - _this->fn->setCircleButtonSize(_this, sz);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_RADIOBUTTON_WIDGET_ENABLED
#endif /* LEGATO_RADIOBUTTON_H */
