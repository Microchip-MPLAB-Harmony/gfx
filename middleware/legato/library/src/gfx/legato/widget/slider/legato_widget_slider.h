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
    legato_widget_slider.h

  Summary:

  Description:
    This module implements slider control widget functions.
*******************************************************************************/

/** \file legato_widget_slider.h
 * @brief Slider widget functions and definitions.
 *
 * @details This module implements slider widget functions.
 */

#ifndef LEGATO_SLIDER_H
#define LEGATO_SLIDER_H

#include "gfx/legato/common/legato_common.h"

#if LE_SLIDER_WIDGET_ENABLED == 1

#ifdef __cplusplus
extern "C" {
#endif

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
/**
 * @brief This enum represents slider state.
 * @details Used to define possible slider states.
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
/**
 * @brief This function represents a value change event callback.
 * @details The callback is used indicate a value has changed.
 * @details .
 */
typedef void (* leSliderWidget_ValueChangedEvent)(leSliderWidget*);

/* internal use only */
/**
  * @cond INTERNAL
  *
  */typedef struct leSliderWidget leSliderWidget;

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

/**
  * @endcond
  *
  */

/**
 * @brief This struct represents a slider widget.
 * @details A slider bar is a widget that is capable of displaying a range
 * and a slider handle.  The slider can be moved between two discreet
 * values and can have a variable min and max range.
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
/**
 * @brief Create widget.
 * @details Creates a new leSliderWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leSliderWidget* wgt = leSliderWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leSliderWidget* leSliderWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leSliderWidget <span class="param">wgt</span>.
 * @code
 * leSliderWidget* wgt;
 * leSliderWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leSliderWidget_Constructor(leSliderWidget* sld);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

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
/**
 * @brief Get scroll bar orientation.
 * @details Gets the scroll bar orientation using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * leOrientation orient = _this->fn->getOrientation(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the orientation value.
 */
leOrientation getOrientation(const leSliderWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult
setOrientation(leSliderWidget* _this,
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
/**
 * @brief Set scroll bar orientation.
 * @details Sets the scroll bar orientation to <span class="param">align</span>
 * with <span class="param">swapDimensions</span>
 * using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * leOrientation align;
 * leBool swapDimensions;
 * leResult res = _this->fn->setOrientation(_this, align, swapDimensions);
 * @endcode
 * @param _this is the widget to modify
 * @param align the orientation value.
 * @param swapDimensions swap the width and height values when changing this
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setOrientation(leSliderWidget* _this,
                                leOrientation align,
                                leBool swapDimensions);

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
/**
 * @brief Get maximum slider value.
 * @details Gets the maximum slider value using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * uint32_t val = _this->fn->getMaximumValue(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the maximum slider value.
 */
virtual uint32_t getMaximumValue(const leSliderWidget* _this);

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
/**
 * @brief Set maximum slider value.
 * @details Sets the maximum slider value to <span class="param">val</span>
 * using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * uint32_t val;
 * leResult res = _this->fn->setMaximumValue(_this, val);
 * @endcode
 * @param _this is the widget to modify
 * @param val the value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult setMaximumValue(leSliderWidget* _this,
                         uint32_t val);


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
/**
 * @brief Get maximum slider value.
 * @details Gets the maximum slider value using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * uint32_t val = _this->fn->getMaximumValue(_this);
 * @endcode
 * @param _this is the widget to query
 * @return the max value.
 */
virtual uint32_t getMaximumValue(const leSliderWidget* _this);


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
/**
 * @brief Set maximum slider value.
 * @details Sets the maximum slider value to <span class="param">val</span>
 * using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * uint32_t val;
 * leResult res = _this->fn->setMaximumValue(_this, val);
 * @endcode
 * @param _this is the widget to modify
 * @param val the value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setMaximumValue(leSliderWidget* _this,
                                 uint32_t val);


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
/**
 * @brief Get value as a percentage.
 * @details Gets the value as a percentage using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * uint32_t val = _this->fn->getPercentage(_this);
 * @endcode
 * @param _this is the widget to query
 * @return the percentage value.
 */
virtual uint32_t getPercentage(const leSliderWidget* _this);


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
/**
 * @brief Set value using a percentage.
 * @details Sets the value using a percentage to <span class="param">val</span>
 * using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * uint32_t val;
 * leResult res = _this->fn->setPercentage(_this, val);
 * @endcode
 * @param _this is the widget to modify
 * @param val the value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPercentage(leSliderWidget* _this,
                               uint32_t val);


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
/**
 * @brief Get slider value.
 * @details Gets the slider value using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * uint32_t val = _this->fn->getValue(_this);
 * @endcode
 * @param _this is the widget to query
 * @return the slider value.
 */
virtual uint32_t getValue(const leSliderWidget* _this);


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
/**
 * @brief Set value using a percentage.
 * @details Sets the value using a percentage to <span class="param">val</span>
 * using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * uint32_t val;
 * leResult res = _this->fn->setPercentage(_this, val);
 * @endcode
 * @param _this is the widget to modify
 * @param val the value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPercentage(leSliderWidget* _this,
                               uint32_t val);


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
/**
 * @brief Set slider by an amounte.
 * @details Sets the slider by an amount to <span class="param">amt</span>
 * using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * int32_t amt;
 * leResult res = _this->fn->step(_this, amt);
 * @endcode
 * @param _this is the widget to modify
 * @param amt the step amount.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult step(leSliderWidget* _this,
                      int32_t amt);

/**
 * @brief Get value changed event callback pointer.
 * @details Gets the value changed event callback pointer using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * leSliderWidget_ValueChangedEvent cb = _this->fn->getValueChangedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leSliderWidget_ValueChangedEvent getValueChangedEventCallback
                                          (const leSliderWidget* _this);

/**
 * @brief Set value changed event callback pointer.
 * @details Sets the value changed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leSliderWidget* _this;
 * leSliderWidget_ValueChangedEvent cb;
 * leResult res = _this->fn->setValueChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setValueChangedEventCallback(leSliderWidget* _this,
                                              leSliderWidget_ValueChangedEvent cb);

#undef THIS_TYPE
#endif

#ifdef __cplusplus
}
#endif

#endif // LE_SLIDER_WIDGET_ENABLED
#endif /* LEGATO_SLIDER_H */
