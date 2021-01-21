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
    legato_widget_slider.h

  Summary:


  Description:
    This module implements slider control widget functions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
#ifndef LEGATO_SLIDER_H
#define LEGATO_SLIDER_H
//DOM-IGNORE-END

#include "gfx/legato/common/legato_common.h"

#if LE_SLIDER_WIDGET_ENABLED

#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leSliderState

  Summary:
    Describes various slider states

  Description:


  Remarks:
    None.
*/
typedef enum leSliderState
{
    LE_SLIDER_STATE_NONE,
    LE_SLIDER_STATE_HANDLE_DOWN,
    LE_SLIDER_STATE_AREA_DOWN
} leSliderState;

typedef struct leSliderWidget leSliderWidget;

// *****************************************************************************
/* Function Pointer:
    leSliderWidget_ValueChangedEvent

  Summary:
    Value changed event function callback type
*/
typedef void (*leSliderWidget_ValueChangedEvent)(leSliderWidget*);

// DOM-IGNORE-BEGIN
typedef struct leSliderWidget leSliderWidget;

#define LE_SLIDERWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    leOrientation (*getOrientation)(const THIS_TYPE* _this); \
    leResult  (*setOrientation)(THIS_TYPE* _this, leOrientation align, leBool swapDimensions); \
    uint32_t  (*getGripSize)(const THIS_TYPE* _this); \
    leResult  (*setGripSize)(THIS_TYPE* _this, uint32_t sz); \
    uint32_t  (*getMininumValue)(const THIS_TYPE* _this); \
    leResult  (*setMinimumValue)(THIS_TYPE* _this, uint32_t min); \
    uint32_t  (*getMaximumValue)(const THIS_TYPE* _this); \
    leResult  (*setMaximumValue)(THIS_TYPE* _this, uint32_t max); \
    uint32_t  (*getPercentage)(const THIS_TYPE* _this); \
    leResult  (*setPercentage)(THIS_TYPE* _this, uint32_t per); \
    int32_t   (*getValue)(const THIS_TYPE* _this); \
    leResult  (*setValue)(THIS_TYPE* _this, int32_t val); \
    leResult  (*step)(THIS_TYPE* _this, int32_t amt); \
    leSliderWidget_ValueChangedEvent (*getValueChangedEventCallback)(const THIS_TYPE* _this); \
    leResult  (*setValueChangedEventCallback)(THIS_TYPE* _this, leSliderWidget_ValueChangedEvent cb);
    
typedef struct leSliderWidgetVTable
{
	LE_SLIDERWIDGET_VTABLE(leSliderWidget)
} leSliderWidgetVTable; 

// DOM-IGNORE-END

// *****************************************************************************
/* Structure:
    leSliderWidget

  Summary:
    Implementation of a slider widget struct

  Description:
    A slider bar is a widget that is capable of displaying a range and a slider
    handle.  The slider can be moved between two discreet values and can have
    a variable min and max range.

  Remarks:
    None.
*/
typedef struct leSliderWidget
{
    leWidget widget; // widget base class

    const leSliderWidgetVTable* fn;

    leSliderState state; // slider state
    leOrientation alignment; // slider alignment

    int32_t min; // slider min value
    int32_t max; // slider max value
    int32_t value; // slider current value
    uint32_t grip; // slider grip size

    leSliderWidget_ValueChangedEvent valueChangedEvent; // value changed event

    lePoint handleDownOffset;
} leSliderWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    leSliderWidget* leSliderWidget_New()

  Summary:
    Allocates memory for a new widget of this type.  The application is
    responsible for the managment of this memory until the widget is added to
    a widget tree.

  Description:


  Parameters:

  Returns:
    leSliderWidget*

  Remarks:
    Use leWidget_Delete() to free this pointer.
*/
LIB_EXPORT leSliderWidget* leSliderWidget_New();

/* Function:
    void leSliderWidget_Constructor(leSliderWidget* wgt)

  Summary:
    Initializes an leSliderWidget widget pointer.

  Description:
    Initializes an leSliderWidget widget pointer.

  Parameters:
    leSliderWidget* wgt - the pointer to initialize

  Returns:
    void

  Remarks:

*/
LIB_EXPORT void leSliderWidget_Constructor(leSliderWidget* sld);

// *****************************************************************************
/* Virtual Member Function:
    leOrientation getOrientation(const leSliderWidget* _this)

  Summary:
     Gets the slider orientation

  Description:
     Gets the slider orientation

  Parameters:
    const leSliderWidget* _this - The Slider widget to operate on

  Remarks:
    Usage - _this->fn->getOrientation(_this);

  Returns:
    leOrientation - the orientation value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setOrientation(leSliderWidget* _this,
                            leOrientation align,
                            leBool swapDimensions)

  Summary:
     Sets the slider orientation

  Description:
     Sets the slider orientation

  Parameters:
    leSliderWidget* _this - The Slider widget to operate on
    leOrientation align - the orientation value
    leBool swapDimensions - swap the width and height values when changing this

  Remarks:
    Usage - _this->fn->setOrientation(_this, align, swapDimensions);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMaximumValue(const leSliderWidget* _this)

  Summary:
     Gets the minimum slider value

  Description:
     Gets the minimum slider value

  Parameters:
    const leSliderWidget* _this - The Slider widget to operate on

  Remarks:
    Usage - _this->fn->getMaximumValue(_this);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setMaximumValue(leSliderWidget* _this,
                             uint32_t val)

  Summary:
     Sets the minimum slider value

  Description:
     Sets the minimum slider value

  Parameters:
    leSliderWidget* _this - The Slider widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setMaximumValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getMaximumValue(const leSliderWidget* _this)

  Summary:
     Gets the maximum slider value

  Description:
     Gets the maximum slider value

  Parameters:
    const leSliderWidget* _this - The Slider widget to operate on

  Remarks:
    Usage - _this->fn->getMaximumValue(_this);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setMaximumValue(leSliderWidget* _this,
                             uint32_t val)

  Summary:
     Sets the maximum slider value

  Description:
     Sets the maximum slider value

  Parameters:
    leSliderWidget* _this - The Slider widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setMaximumValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getPercentage(const leSliderWidget* _this)

  Summary:
     Gets the value as a percentage

  Description:
     Gets the value as a percentage

  Parameters:
    const leSliderWidget* _this - The Slider widget to operate on

  Remarks:
    Usage - _this->fn->getPercentage(_this);

  Returns:
    uint32_t - the percentage value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setPercentage(leSliderWidget* _this,
                           uint32_t val)

  Summary:
     Sets the value using a percentage

  Description:
     Sets the value using a percentage

  Parameters:
    leSliderWidget* _this - The Slider widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setPercentage(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getValue(const leSliderWidget* _this)

  Summary:
     Gets the slider value

  Description:
     Gets the slider value

  Parameters:
    const leSliderWidget* _this - The Slider widget to operate on

  Remarks:
    Usage - _this->fn->getValue(_this);

  Returns:
    uint32_t - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValue(leSliderWidget* _this,
                      uint32_t val)

  Summary:
     Sets the slider value

  Description:
     Sets the slider value

  Parameters:
    leSliderWidget* _this - The Slider widget to operate on
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setValue(_this, val);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult step(leSliderWidget* _this,
                  int32_t amt)

  Summary:
     Step the slider by an amount

  Description:
     Step the slider by an amount

  Parameters:
    leSliderWidget* _this - The Slider widget to operate on
    int32_t amt - the step amount

  Remarks:
    Usage - _this->fn->step(_this, amt);

  Returns:
    leResult - the result of the operation
*/

// *****************************************************************************
/* Virtual Member Function:
    leSliderWidget_ValueChangedEvent getValueChangedEventCallback(const leSliderWidget* _this)

  Summary:
     Gets the value changed event callback pointer

  Description:
     Gets the value changed event callback pointer

  Parameters:
    const leSliderWidget* _this - The Slider widget to operate on

  Remarks:
    Usage - _this->fn->getValueChangedEventCallback(_this);

  Returns:
    leSliderWidget_ValueChangedEvent - the value
*/

// *****************************************************************************
/* Virtual Member Function:
    leResult setValueChangedEventCallback(leSliderWidget* _this,
                                          leSliderWidget_ValueChangedEvent cb)

  Summary:
     Sets the value changed event callback pointer

  Description:
     Sets the value changed event callback pointer

  Parameters:
    leSliderWidget* _this - The Slider widget to operate on
    leSliderWidget_ValueChangedEvent cb - the callback pointer

  Remarks:
    Usage - _this->fn->setValueChangedEventCallback(_this, cb);

  Returns:
    leResult - the result of the operation
*/



#endif // LE_SLIDER_WIDGET_ENABLED
#endif /* LEGATO_SLIDER_H */
