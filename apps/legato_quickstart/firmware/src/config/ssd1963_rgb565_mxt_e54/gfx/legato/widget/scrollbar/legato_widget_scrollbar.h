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
    legato_widget_scrollbar.h

  Summary:


  Description:
    This module implements scroll bar widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_SCROLLBAR_H
#define LEGATO_SCROLLBAR_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_SCROLLBAR_WIDGET_ENABLED == 1

#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leScrollBarState

  Summary:
    Defines the various scroll bar state values

  Description:


  Remarks:
    None.
*/
typedef enum leScrollBarState
{
    LE_SCROLLBAR_STATE_NONE,
    LE_SCROLLBAR_STATE_TOP_PRESSED,
    LE_SCROLLBAR_STATE_TOP_INSIDE,
    LE_SCROLLBAR_STATE_BOTTOM_PRESSED,
    LE_SCROLLBAR_STATE_BOTTOM_INSIDE,
    LE_SCROLLBAR_STATE_HANDLE_DOWN
} leScrollBarState;

typedef struct leScrollBarWidget leScrollBarWidget;

// *****************************************************************************
/* Function Pointer:
    leScrollBarWidget_ValueChangedEvent

  Summary:
    Value changed event function callback type
*/
typedef void (*leScrollBarWidget_ValueChangedEvent)(leScrollBarWidget*);

// DOM-IGNORE-BEGIN
typedef struct leScrollBarWidget leScrollBarWidget;

#define LE_SCROLLBARWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leOrientation (*getOrientation)(const THIS_TYPE* _this); \
    leResult      (*setOrientation)(THIS_TYPE* _this, leOrientation align, leBool swapDimensions); \
    uint32_t      (*getMaximumValue)(const THIS_TYPE* _this); \
    leResult      (*setMaximumValue)(THIS_TYPE* _this, uint32_t val); \
    uint32_t      (*getExtentValue)(const THIS_TYPE* _this); \
    leResult      (*setExtentValue)(THIS_TYPE* _this, uint32_t val); \
    uint32_t      (*getScrollPercentage)(const THIS_TYPE* _this); \
    leResult      (*setScrollPercentage)(THIS_TYPE* _this, uint32_t val); \
    uint32_t      (*getScrollValue)(const THIS_TYPE* _this); \
    leResult      (*setScrollValue)(THIS_TYPE* _this, uint32_t val); \
    uint32_t      (*getStepSize)(const THIS_TYPE* _this); \
    leResult      (*setStepSize)(THIS_TYPE* _this, uint32_t val); \
    leResult      (*stepBackward)(THIS_TYPE* _this); \
    leResult      (*stepForward)(THIS_TYPE* _this); \
    leScrollBarWidget_ValueChangedEvent (*getValueChangedEventCallback)(const THIS_TYPE* _this); \
    leResult      (*setValueChangedEventCallback)(THIS_TYPE* _this, leScrollBarWidget_ValueChangedEvent cb); \
    
typedef struct leScrollBarWidgetVTable
{
	LE_SCROLLBARWIDGET_VTABLE(leScrollBarWidget)
} leScrollBarWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leScrollBarWidget

  Summary:
    Implementation of a scroll bar widget.

  Description:
    A scroll bar is a widget that is capable of displaying a range and a scroll
    handle.  The handle can grow and shrink in size depending on the scroll
    range and visible scroll space and can be interacted with to scroll through
    the available space.

  Remarks:
    None.
*/
typedef struct leScrollBarWidget
{
    leWidget widget; // widget base class

    const leScrollBarWidgetVTable* fn;

    leScrollBarState state; // scrollbar input state
    leOrientation alignment; // scroll bar direction

    uint32_t max; // maximum scroll value
    uint32_t extent; // visible space/handle size
    uint32_t value; // current scroll value
    uint32_t step; // discreet scroll stepping value

    leScrollBarWidget_ValueChangedEvent valueChangedEvent; // value changed callback

    lePoint handleDownOffset;
} leScrollBarWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leScrollBarWidget* leScrollBarWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:


  Parameters:

  Returns:
    leScrollBarWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leScrollBarWidget* leScrollBarWidget_New();

/* Function:
    void leScrollBarWidget_Constructor(leScrollBarWidget* wgt)

  Summary:
    Initializes an leScrollBarWidget widget pointer.

  Description:
    Initializes an leScrollBarWidget widget pointer.

  Parameters:
    leScrollBarWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leScrollBarWidget_Constructor(leScrollBarWidget* bar);

// *****************************************************************************
/* Virtual Member Function:
    leOrientation getOrientation(const leScrollBarWidget* _this)

  Summary:
     Gets the scroll bar orientation

  Description:
     Gets the scroll bar orientation

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getOrientation(_this);

  Returns:
    leOrientation - the orientation value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setOrientation(leScrollBarWidget* _this,
                            leOrientation align,
                            leBool swapDimensions)

  Summary:
     Sets the scroll bar orientation

  Description:
     Sets the scroll bar orientation

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    leOrientation align - the orientation value
    leBool swapDimensions - swap the width and height values when changing this

  Remarks:
    Usage - _this->fn->setOrientation(_this, align, swapDimensions);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMaximumValue(const leScrollBarWidget* _this)

  Summary:
     Gets the maximum scroll value

  Description:
     Gets the maximum scroll value

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getMaximumValue(_this);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setMaximumValue(leScrollBarWidget* _this,
                             uint32_t val)

  Summary:
     Sets the maximum scroll value

  Description:
     Sets the maximum scroll value

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setMaximumValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getExtentValue(const leScrollBarWidget* _this)

  Summary:
     Gets the extent value

  Description:
     Gets the extent value

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getExtentValue(_this);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setExtentValue(leScrollBarWidget* _this,
                            uint32_t val)

  Summary:
     Sets the extent value

  Description:
     Sets the extent value

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setExtentValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getScrollPercentage(const leScrollBarWidget* _this)

  Summary:
     Gets the scroll value as a percentage

  Description:
     Gets the scroll value as a percentage

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getScrollPercentage(_this);

  Returns:
    uint32_t - the scroll value percentage
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setScrollPercentage(leScrollBarWidget* _this,
                                 uint32_t val)

  Summary:
     Sets the scroll value using a percentage

  Description:
     Sets the scroll value using a percentage

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the percentage value

  Remarks:
    Usage - _this->fn->setScrollPercentage(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getScrollValue(const leScrollBarWidget* _this)

  Summary:
     Gets the scroll value

  Description:
     Gets the scroll value

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getScrollValue(_this);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setScrollValue(leScrollBarWidget* _this,
                            uint32_t val)

  Summary:
     Sets the scroll value

  Description:
     Sets the scroll value

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setScrollValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getStepSize(const leScrollBarWidget* _this)

  Summary:
     Gets the step size

  Description:
     Gets the step size

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getStepSize(_this);

  Returns:
    uint32_t - the step size
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setStepSize(leScrollBarWidget* _this,
                         uint32_t val)

  Summary:
     Sets the step size

  Description:
     Sets the step size

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    uint32_t val - the step size

  Remarks:
    Usage - _this->fn->setStepSize(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult stepBackward(leScrollBarWidget* _this)

  Summary:
     Steps the value backwards

  Description:
     Steps the value backwards

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->stepBackward(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult stepForward(leScrollBarWidget* _this)

  Summary:
     Steps the value forwards

  Description:
     Steps the value forwards

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->stepForward(_this);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leScrollBarWidget_ValueChangedEvent getValueChangedEventCallback(const leScrollBarWidget* _this)

  Summary:
     Gets the value changed event callback pointer

  Description:
     Gets the value changed event callback pointer

  Parameters:
    const leScrollBarWidget* _this - The scroll bar widget to operate on

  Remarks:
    Usage - _this->fn->getValueChangedEventCallback(_this);

  Returns:
    leScrollBarWidget_ValueChangedEvent - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueChangedEventCallback(leScrollBarWidget* _this,
                                          leScrollBarWidget_ValueChangedEvent cb)

  Summary:
     Sets the value changed event callback pointer

  Description:
     Sets the value changed event callback pointer

  Parameters:
    leScrollBarWidget* _this - The scroll bar widget to operate on
    leScrollBarWidget_ValueChangedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setValueChangedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_SCROLLBAR_WIDGET_ENABLED
#endif /* LEGATO_SCROLLBAR_H */
